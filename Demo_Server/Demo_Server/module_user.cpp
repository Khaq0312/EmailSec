#include "stdafx.h"
#include "module_user.h"
#include "module_mail.h"
#include "Resource.h"
// find if user existsvoid checkmail(int sockfd) {
void checkmail(int sockfd) {
	char fname[50];
	int len;

	send_data(sockfd, reply_code[28]); // require mail name
	len = recv(sockfd, fname, sizeof(fname), 0);
	Sleep(1);
	fname[len] = '\0';

	char path[80];
	strcpy(path, data_dir);
	strcat(path, from_user);
	strcat(path, "\\");
	strcat(path, fname);

	FILE* file;
	file = fopen(path, "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	char buffer[1024];
	size_t bytes_read;
	

	while ((bytes_read = fread(buffer, 1, 1024, file)) > 0) {
		send(sockfd, buffer, bytes_read, 0);
	}
	Sleep(10);
	send(sockfd, "", 1, 0);


	//while (fgets(data, sizeof(data), fp) != NULL) {
	//	// cout<<data<<"---------"<<endl;
	//
	//char* token;
	//token = strtok(data, " ");
	//// test username
	//// cout<<token<<" "<<strlen(token)<<endl;
	//// cout<<name<<" "<<strlen(name)<<endl;
	//	if (strncmp(token, name, strlen(name)) == 0) { // find username
	//		cout<<"found username"<<endl;
	//		char *p;
	//		token = strtok(nullptr, " ");
	//		// cout<<token<<" "strlen(token)<<endl;
	//		// cout<<pass<<" "strlen(pass)<<endl;
	//		if (strncmp(token, pass, strlen(pass)) == 0) { // valid passwd
	//			cout<<"True password"<<endl;
	//			fclose(fp);
	//			strcpy(file, data_dir);
	//			strcat(file, userstat);
	//			fp = fopen(file, "w+");
	//			strcat(name, " on"); // change the status of the user to ON
	//			fwrite(name, 1, strlen(name), fp);
	//			fclose(fp);
	//			strcpy(from_user,name);
	//			return 1; // success
	//		} else { // invalid passwd
	//		    cout<<"invalid password"<<endl;
	//			return 0;
	//		}
	//	}
	//}
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