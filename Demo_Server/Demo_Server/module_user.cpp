#include "stdafx.h"
#include "module_user.h"
#include "module_mail.h"
#include "Resource.h"
// mailbox
void listmail(int sockfd) {
	string folder_path = data_dir + string(from_user);

	int file_count = 0;
	vector <string>filename;
	for (const auto& entry : fs::directory_iterator(folder_path)) {
		if (fs::is_regular_file(entry)) {
			filename.push_back(entry.path().filename().string());
			file_count++;
		}
	}
	char buffer[1024];
	std::sprintf(buffer, "%d", file_count);
	send(sockfd, buffer, strlen(buffer), 0);
	Sleep(10);
	for (int i = 0; i < filename.size(); ++i) {
		send(sockfd, filename[i].c_str(), strlen(filename[i].c_str()), 0);
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

	
	FILE* file;
	file = fopen(path.c_str(), "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	int bytes_read;
	string temp = "";
	while (fread(buffer, 1, strlen(buffer), file) > 0) {
		temp += string(buffer);
	}
	temp += '\0';
	send(sockfd, temp.c_str(), strlen(temp.c_str()), 0);
	//buffer[bytes_read] = '\0';
	fclose(file);
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
		// cout<<data<<"---------"<<endl;

		char* token;
		token = strtok(data, " ");
		// test username
		// cout<<token<<" "<<strlen(token)<<endl;
		// cout<<name<<" "<<strlen(name)<<endl;
		if (strncmp(token, name, strlen(name)) == 0) { // find username
			cout << "found username" << endl;
			char* p;
			token = strtok(nullptr, " ");
			// cout<<token<<" "strlen(token)<<endl;
			// cout<<pass<<" "strlen(pass)<<endl;
			if (strncmp(token, pass, strlen(pass)) == 0) { // valid passwd
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