#ifndef __XSMTP_MODULE_MAIL_H
#define __XSMTP_MODULE_MAIL_H


//void* mail_proc(void* param);
DWORD WINAPI mail_proc(LPVOID param);
void respond(int client_sockfd, char* request);
void send_data(int sockfd, const char* data);
void mail_data(int sockfd);
char* base64_decode(char* s);

//extern int check_user();
//extern void auth(int sockfd);
//extern void user_quit();

#endif /* __XSMTP_MODULE_MAIL_H */

// Local Variables:
// mode: C++
// End: