#pragma once

//#ifndef __XSMTP_MODULE_USER_H
//#define __XSMTP_MODULE_USER_H
void listmail(int sockfd);
void retrieve(int sockfd);
void DELT(int sockfd);
int check_user();
void auth(int sockfd);
int check_name_pass(char* name, char* pass);
void user_quit();

extern void send_data(int sockfd, const char* data);
extern char* base64_decode(char* s);

//#endif /* __XSMTP_MODULE_USER_H */

//
//
//#ifndef __XSMTP_MODULE_MAIL_H
//#define __XSMTP_MODULE_MAIL_H

static char B64[64] = {
	'A','B','C','D','E','F','G',
	'H','I','J','K','L','M','N',
	'O','P','Q','R','S','T',
	'U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g',
	'h','i','j','k','l','m','n',
	'o','p','q','r','s','t',
	'u','v','w','x','y','z',
	'0','1','2','3','4','5','6',
	'7','8','9','+','/'
};

//void* mail_proc(void* param);
DWORD WINAPI mail_proc(LPVOID param);
void respond(int client_sockfd, char* request);
void send_data(int sockfd, const char* data);
void mail_data(int sockfd);
char* base64_decode(char* s);

////extern int check_user();
////extern void auth(int sockfd);
////extern void user_quit();
//
//#endif /* __XSMTP_MODULE_MAIL_H */

