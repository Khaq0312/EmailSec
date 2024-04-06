#include "stdafx.h"
#include "module_user.h"
#include "module_mail.h"
#include "Resource.h"

bool compareByNewest(const string& a, const string& b) {
	return a > b;
}

void listmail(int sockfd) {
	string folder_path = data_dir + string(from_user);

	vector<string> filename;
	filename.clear();
	for (const auto& entry : fs::directory_iterator(folder_path)) {
		if (fs::is_regular_file(entry)) {
			filename.push_back(entry.path().filename().string());
		}
	}
	//sort: new -> old
	std::sort(filename.begin(), filename.end(),compareByNewest);
	
	int file_count = filename.size();
	char buffer[1024];
	sprintf(buffer, "%d", file_count);
	send(sockfd, buffer, strlen(buffer), 0);

	int recvMessage = recv(sockfd, buffer, 1024, 0);
	buffer[recvMessage] = '\0';

	Sleep(10);
	for (int i = 0; i < filename.size(); ++i) 
	{
		cout << filename[i] << endl;
		send(sockfd, filename[i].c_str(), strlen(filename[i].c_str()), 0);
		recvMessage = recv(sockfd, buffer, 1024, 0);
		buffer[recvMessage] = '\0';

	}
}


//get mail content
void retrieve(int sockfd) {
	char buffer[BUF_SIZE];
	send(sockfd, "OK", 2, 0);
	int recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';
	string filename = string(buffer);
	string path = ".\\Data\\" + string(from_user) + "\\";

	fs::current_path("Data");
	fs::current_path(from_user);

	size_t pos = filename.find("_");
	if (pos != std::string::npos)
	{
		string not_sign_file = filename.substr(0,pos);

		string domain = filename.substr(pos + 1);
		domain = domain.substr(0, domain.length() - 4);
		string pubDkim = getDkim(domain);
		int dkim_check = verifyKey(pubDkim, not_sign_file, filename);

		if (dkim_check == 0)
		{
			send(sockfd, "SPAM", 4, 0);
			recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
		}
		else
		{
			send(sockfd, "NOT SPAM", 8, 0);
			recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
		}
		std::remove(filename.c_str());
		filename = not_sign_file;

	}
	else //sender receiver are same domain
	{
		send(sockfd, "NOT SPAM", 8, 0);
		recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
		buffer[recvMessage] = '\0';
	}
	string is_spam = string(buffer);
	fs::current_path("..");
	fs::current_path("..");

	fstream file;
	path += filename;
	file.open(path, ios::in);

	if (!file.is_open()) {
		std::cerr << "Error opening file: " << path << std::endl;
		return;
	}
	
	string cipher, aes_key;
	char byte;
	while (file.get(byte))
	{
		cipher += byte;
	}
	file.close();
	std::remove(path.c_str());

	send(sockfd, cipher.c_str(), strlen(cipher.c_str()), 0);
	recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';

	path = ".\\key\\" + filename;
	file.open(path, ios::in);
	if (file.is_open())
	{
		while (file.get(byte))
		{
			aes_key += byte;
		}
		file.close();
	}

	send(sockfd, aes_key.c_str(), strlen(aes_key.c_str()), 0);
	recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';

	recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';
	send(sockfd, "OK", 2, 0);
	if (pos != std::string::npos)
	{
		int spf = checkSPF(string(buffer));
		if (spf == 0)
		{
			send(sockfd, "SPAM", 4, 0);
			recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
		}
		else
		{
			send(sockfd, "NOT SPAM", 8, 0);
			recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
			buffer[recvMessage] = '\0';
		}
	}
	else
	{
		send(sockfd, "NOT SPAM", 8, 0);
		recvMessage = recv(sockfd, buffer, BUF_SIZE, 0);
		buffer[recvMessage] = '\0';
	}
	
	//delete temp file after retrieve to local
	std::remove(path.c_str());
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