#include "stdafx.h"
#include "module_user.h"
#include "module_mail.h"
#include "Resource.h"

// mailbox
void listmail(int sockfd) {
	string folder_path = data_dir + string(from_user);

	vector<string> filename;
	for (const auto& entry : fs::directory_iterator(folder_path)) {
		if (fs::is_regular_file(entry)) {
			filename.push_back(entry.path().filename().string());
		}
	}
	//sort: new -> old
	sort(filename.rbegin(), filename.rend());

	int file_count = filename.size();
	char buffer[1024];
	sprintf(buffer, "%d", file_count);
	send(sockfd, buffer, strlen(buffer), 0);

	int recvMessage = recv(sockfd, buffer, 1024, 0);
	buffer[recvMessage] = '\0';

	Sleep(10);
	for (int i = 0; i < filename.size(); ++i) 
	{
		send(sockfd, filename[i].c_str(), strlen(filename[i].c_str()), 0);
		recvMessage = recv(sockfd, buffer, 1024, 0);
		buffer[recvMessage] = '\0';
		fstream fIn;
		string path = folder_path + "\\" + filename[i];
		fIn.open(path, ios::in);
		if (fIn.is_open()) {
			string time, from, to, subject;
			getline(fIn, time);
			getline(fIn, from);
			getline(fIn, to);
			getline(fIn, subject);

			int pos = time.find(":");
			time = time.substr(pos + 1);

			pos = subject.find(":");
			subject = subject.substr(pos + 1);

			string title = "";
			string temp = "From:" + string(from_user);
			int space;
			if (from == temp)
			{
				title = time + "      " + to + "  ";
				title += subject;
			}
			else
			{
				title = time + "      " + from + "  ";
				title += subject;
			}
		
			fIn.close();

			send(sockfd, title.c_str(), title.size(), 0);
			recvMessage = recv(sockfd, buffer, 1024, 0);
			buffer[recvMessage] = '\0';
		}
	}
}

//get mail content
void retrieve(int sockfd) {
	char buffer[1024];
	int recvMessage = recv(sockfd, buffer, strlen(buffer), 0);
	buffer[recvMessage] = '\0';
	string path = data_dir + string(from_user) + "\\" + buffer;
	fstream fin;
	fin.open(path, ios::in);

	if (!fin.is_open()) {
		std::cerr << "Error opening file: " << path << std::endl;
		return;
	}

	string temp;
	char byte;
	while (fin.get(byte)) {
		temp += byte;
	}

	fin.close();
	send(sockfd, temp.c_str(), temp.size(), 0);
}
// find if user exists
int check_user() {
	FILE* fp;
	char file[100] = "";
	char data[60];

	strcpy_s(file, sizeof(file), data_dir);
	strcat_s(file, sizeof(file), userinfo);

	errno_t err = fopen_s(&fp, file, "r");
	if (err != 0) {
		std::cerr << "Failed to open file: " << strerror(err) << std::endl;
		return 0;
	}

	while (fgets(data, sizeof(data), fp) != NULL) {
		if (strncmp(from_user, data, strlen(from_user)) == 0) // valid user
			return 1;
	}

	fclose(fp);
	return 0;
}

// user authentication
void auth(int sockfd) {
	char ename[50], epass[50];
	char* name, * pass;
	int len;

	send_data(sockfd, reply_code[25]); // require username
	Sleep(3);
	len = recv(sockfd, ename, sizeof(ename), 0);
	ename[len] = '\0';
	if (len > 0) {
		cout << "Request stream: " << ename << endl;
		// name = base64_decode(ename);
		name = ename;
		cout << "Decoded username: " << name << endl;
		send_data(sockfd, reply_code[26]); // require passwd
		Sleep(3);
		len = recv(sockfd, epass, sizeof(epass), 0);
		epass[len] = '\0';
		if (len > 0) {
			cout << "Request stream: " << epass << endl;
			// pass = base64_decode(epass);
			pass = epass;
			cout << "Decoded password: " << pass << endl;
			if (check_name_pass(ename, epass)) { // check username and passwd
				mail_stat = 13;
				send_data(sockfd, reply_code[27]);
			}
			else {
				send_data(sockfd, reply_code[16]);
			}
		}
		else {
			send_data(sockfd, reply_code[16]);
		}
	}
	else {
		send_data(sockfd, reply_code[16]);
	}
}

// check username and passwd from file
int check_name_pass(char* name, char* pass) {
	cout << "Checking pass" << endl;
	FILE* fp;
	char file[80], data[60];

	strcpy(file, data_dir);
	strcat(file, userinfo);
	fp = fopen(file, "r");
	while (fgets(data, sizeof(data), fp) != NULL) {

		char* token;
		token = strtok(data, " ");
		if ((strncmp(token, name, strlen(token)) == 0) && string(token) == name) { // find username
			cout << "found username" << endl;
			char* p;
			token = strtok(nullptr, " ");
			if (token[strlen(token) - 1] == '\n')
			{
				token[strlen(token) - 1] = '\0';

			}
			if ((strncmp(token, pass, strlen(token)) == 0) && string(token) == pass) { // valid passwd

				cout << "True password" << endl;
				strcpy(from_user, name);

				fclose(fp);
				strcpy(file, data_dir);
				strcat(file, userstat);
				fp = fopen(file, "w+");
				strcat(name, " on"); // change the status of the user to ON
				fwrite(name, 1, strlen(name), fp);
				fclose(fp);
				return 1; // success
			}
			else { // invalid passwd
				cout << "invalid password" << endl;
				return 0;
			}
		}
	}
	return 0; // invalid username
}

// user logout
void user_quit() {
	FILE* fp;
	char file[80], data[60];
	int flag = 0, len;

	strcpy_s(file, sizeof(file), data_dir);
	strcat_s(file, sizeof(file), userstat);
	fp = fopen(file, "w+");
	while (fgets(data, sizeof(data), fp) != NULL) {
		if (strncmp(data, from_user, strlen(from_user)) == 0) {
			flag = 1;
		}
		if (flag) {
			len = strlen(data);
			if (fgets(data, sizeof(data), fp) != NULL) {
				len = -len;
				fseek(fp, len, SEEK_CUR);
				fputs(data, fp);
				len = strlen(data);
				fseek(fp, len, SEEK_CUR);
			}
		}
	}
	fclose(fp);
}