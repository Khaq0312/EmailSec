#include "module.h"
#include "Resource.h"
#include "Demo_Server.h"
#include "stdafx.h"
//// send data by socket
//void send_data(int sockfd, const char* data) {
//	if (data != NULL) {
//		send(sockfd, data, strlen(data), 0);
//		cout << "Reply stream: " << data;
//	}
//}
//
//// recieve mail contents
//void mail_data(int sockfd) {
//	Sleep(1);
//	char buf[BUF_SIZE];
//	memset(buf, 0, sizeof(buf));
//	recv(sockfd, buf, sizeof(buf), 0); // recieve mail contents
//	cout << "Mail Contents: \n" << buf << endl;
//
//	//mail content and format check
//
//	//mail content store
//	int tm = time(NULL), i;
//	char file[80], tp[20];
//
//	for (i = 0; i < rcpt_user_num; i++) {
//		strcpy(file, data_dir);
//		strcat(file, rcpt_user[i]);
//		if (access(file, 0) == -1) {
//			_mkdir(file);
//		}
//		sprintf(tp, "/%d", tm);
//		strcat(file, tp);
//
//		FILE* fp = fopen(file, "w+");
//		if (fp != NULL) {
//			fwrite(buf, 1, strlen(buf), fp);
//			fclose(fp);
//		}
//		else {
//			cout << "File open error!" << endl;
//		}
//	}
//	send_data(sockfd, reply_code[6]);
//}
//
//// decode base64 streams
//char* base64_decode(char* s) {
//	char* p = s, * e, * r, * _ret;
//	int len = strlen(s);
//	unsigned char i, unit[4];
//
//	e = s + len;
//	len = len / 4 * 3 + 1;
//	r = _ret = (char*)malloc(len);
//
//	while (p < e) {
//		memcpy(unit, p, 4);
//		if (unit[3] == '=')
//			unit[3] = 0;
//		if (unit[2] == '=')
//			unit[2] = 0;
//		p += 4;
//
//		for (i = 0; unit[0] != B64[i] && i < 64; i++)
//			;
//		unit[0] = i == 64 ? 0 : i;
//		for (i = 0; unit[1] != B64[i] && i < 64; i++)
//			;
//		unit[1] = i == 64 ? 0 : i;
//		for (i = 0; unit[2] != B64[i] && i < 64; i++)
//			;
//		unit[2] = i == 64 ? 0 : i;
//		for (i = 0; unit[3] != B64[i] && i < 64; i++)
//			;
//		unit[3] = i == 64 ? 0 : i;
//		*r++ = (unit[0] << 2) | (unit[1] >> 4);
//		*r++ = (unit[1] << 4) | (unit[2] >> 2);
//		*r++ = (unit[2] << 6) | unit[3];
//	}
//	*r = 0;
//
//	return _ret;
//}
//void* mail_proc(void* param) {
//	int client_sockfd, len;
//	char buf[BUF_SIZE];
//
//	memset(buf, 0, sizeof(buf));
//	client_sockfd = *(int*)param;
//
//	send_data(client_sockfd, reply_code[4]); //send 220
//	mail_stat = 1;
//
//	while (1) {
//		memset(buf, 0, sizeof(buf));
//		len = recv(client_sockfd, buf, sizeof(buf), 0);
//		if (len > 0) {
//			cout << "Request stream: " << buf;
//			respond(client_sockfd, buf);
//		}
//		else {
//			// cout << "S: no data received from client. The server exit permanently.\n";
//			//break;
//			Sleep(1);
//			continue;
//		}
//	}
//	cout << "S:[" << client_sockfd << "] socket closed by client." << endl;
//	cout << "============================================================\n\n";
//	return NULL;
//}
//
//// respond to request from the client
//void respond(int client_sockfd, char* request) {
//	char output[1024];
//	memset(output, 0, sizeof(output));
//
//	//smtp
//	if (strncmp(request, "HELO", 4) == 0) {
//		if (mail_stat == 1) {
//			send_data(client_sockfd, reply_code[6]);
//			rcpt_user_num = 0;
//			memset(rcpt_user, 0, sizeof(rcpt_user));
//			mail_stat = 2;
//		}
//		else {
//			send_data(client_sockfd, reply_code[15]);
//		}
//	}
//	else if (strncmp(request, "MAIL FROM", 9) == 0) {
//		if (mail_stat == 2 || mail_stat == 13) {
//			char* pa, * pb;
//			pa = strchr(request, '<');
//			pb = strchr(request, '>');
//			strncpy(from_user, pa + 1, pb - pa - 1);
//			if (check_user()) {
//				send_data(client_sockfd, reply_code[6]);
//				mail_stat = 3;
//			}
//			else {
//				send_data(client_sockfd, reply_code[15]);
//			}
//		}
//		else if (mail_stat == 12) {
//			send_data(client_sockfd, reply_code[23]);
//		}
//		else {
//			send_data(client_sockfd, "503 Error: send HELO/EHLO first\r\n");
//		}
//	}
//	else if (strncmp(request, "RCPT TO", 7) == 0) {
//		if ((mail_stat == 3 || mail_stat == 4) && rcpt_user_num < MAX_RCPT_USR) {
//			char* pa, * pb;
//			pa = strchr(request, '<');
//			pb = strchr(request, '>');
//			strncpy(rcpt_user[rcpt_user_num++], pa + 1, pb - pa - 1);
//			send_data(client_sockfd, reply_code[6]);
//			mail_stat = 4;
//		}
//		else {
//			send_data(client_sockfd, reply_code[16]);
//		}
//	}
//	else if (strncmp(request, "DATA", 4) == 0) {
//		if (mail_stat == 4) {
//			send_data(client_sockfd, reply_code[8]);
//			mail_data(client_sockfd);
//			mail_stat = 5;
//		}
//		else {
//			send_data(client_sockfd, reply_code[16]);
//		}
//	}
//	else if (strncmp(request, "RSET", 4) == 0) {
//		mail_stat = 1;
//		send_data(client_sockfd, reply_code[6]);
//	}
//	else if (strncmp(request, "NOOP", 4) == 0) {
//		send_data(client_sockfd, reply_code[6]);
//	}
//	else if (strncmp(request, "QUIT", 4) == 0) {
//		mail_stat = 0;
//		user_quit();
//		send_data(client_sockfd, reply_code[5]);
//		ExitThread(1);
//	}
//	//esmpt
//	else if (strncmp(request, "EHLO", 4) == 0) {
//		if (mail_stat == 1) {
//			send_data(client_sockfd, reply_code[24]);
//			mail_stat = 12;
//		}
//		else {
//			send_data(client_sockfd, reply_code[15]);
//		}
//	}
//	else if (strncmp(request, "AUTH LOGIN", 10) == 0) {
//		auth(client_sockfd);
//	}
//	else if (strncmp(request, "AUTH LOGIN PLAIN", 10) == 0) {
//		auth(client_sockfd);
//	}
//	else if (strncmp(request, "AUTH=LOGIN PLAIN", 10) == 0) {
//		auth(client_sockfd);
//	}
//	else {
//		send_data(client_sockfd, reply_code[15]);
//	}
//}
//
//
////module user
//int check_user() {
//	FILE* fp;
//	char file[80] = "";
//	char data[60];
//
//	strcpy(file, data_dir);
//	strcat(file, userinfo);
//
//	fp = fopen(file, "r");
//	while (fgets(data, sizeof(data), fp) != NULL) {
//		if (strncmp(from_user, data, strlen(from_user)) == 0) // valid user
//			return 1;
//	}
//	return 0;
//}
//
//// user authentication
//void auth(int sockfd) {
//	char ename[50], epass[50];
//	char* name, * pass;
//	int len;
//
//	send_data(sockfd, reply_code[25]); // require username
//	Sleep(3);
//	len = recv(sockfd, ename, sizeof(ename), 0);
//	if (len > 0) {
//		cout << "Request stream: " << ename << endl;
//		// name = base64_decode(ename);
//		name = ename;
//		cout << "Decoded username: " << name << endl;
//		send_data(sockfd, reply_code[26]); // require passwd
//		Sleep(3);
//		len = recv(sockfd, epass, sizeof(epass), 0);
//		if (len > 0) {
//			cout << "Request stream: " << epass << endl;
//			// pass = base64_decode(epass);
//			pass = epass;
//			cout << "Decoded password: " << pass << endl;
//			if (check_name_pass(ename, epass)) { // check username and passwd
//				mail_stat = 13;
//				send_data(sockfd, reply_code[27]);
//			}
//			else {
//				send_data(sockfd, reply_code[16]);
//			}
//		}
//		else {
//			send_data(sockfd, reply_code[16]);
//		}
//	}
//	else {
//		send_data(sockfd, reply_code[16]);
//	}
//}
//
//// check username and passwd from file
//int check_name_pass(char* name, char* pass) {
//	cout << "Checking pass" << endl;
//	name[strlen(name) - 2] = '\0';
//	pass[strlen(pass) - 2] = '\0';
//	FILE* fp;
//	char file[80], data[60];
//
//	strcpy(file, data_dir);
//	strcat(file, userinfo);
//	fp = fopen(file, "r");
//	while (fgets(data, sizeof(data), fp) != NULL) {
//		// cout<<data<<"---------"<<endl;
//
//		char* token;
//		token = strtok(data, " ");
//		// test username
//		// cout<<token<<" "<<strlen(token)<<endl;
//		// cout<<name<<" "<<strlen(name)<<endl;
//		if (strncmp(token, name, strlen(name)) == 0) { // find username
//			cout << "found username" << endl;
//			char* p;
//			token = strtok(nullptr, " ");
//			// cout<<token<<" "strlen(token)<<endl;
//			// cout<<pass<<" "strlen(pass)<<endl;
//			if (strncmp(token, pass, strlen(pass)) == 0) { // valid passwd
//				cout << "True password" << endl;
//				fclose(fp);
//				strcpy(file, data_dir);
//				strcat(file, userstat);
//				fp = fopen(file, "w+");
//				strcat(name, " on"); // change the status of the user to ON
//				fwrite(name, 1, strlen(name), fp);
//				fclose(fp);
//				return 1; // success
//			}
//			else { // invalid passwd
//				cout << "invalid password" << endl;
//				return 0;
//			}
//		}
//	}
//	return 0; // invalid username
//}
//
//// user logout
//void user_quit() {
//	FILE* fp;
//	char file[80], data[60];
//	int flag = 0, len;
//
//	strcpy(file, data_dir);
//	strcat(file, userstat);
//	fp = fopen(file, "w+");
//	while (fgets(data, sizeof(data), fp) != NULL) {
//		if (strncmp(data, from_user, strlen(from_user)) == 0) {
//			flag = 1;
//		}
//		if (flag) {
//			len = strlen(data);
//			if (fgets(data, sizeof(data), fp) != NULL) {
//				len = -len;
//				fseek(fp, len, SEEK_CUR);
//				fputs(data, fp);
//				len = strlen(data);
//				fseek(fp, len, SEEK_CUR);
//			}
//		}
//	}
//	fclose(fp);
//}
