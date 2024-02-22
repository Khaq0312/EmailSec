//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by Demo_Server.rc
//
//#define _WIN32_WINNT 0x0501
#define IDS_APP_TITLE			103
#define PORT "25"  // use port 25, need root privilege
#define MAX_CLIENTS 32
#define MAX_RCPT_USR 50
#define BUF_SIZE 1024
#define DEFAULT_PORT 25
#define MAX_BUFFER_SIZE 50
// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE	101
#define _APS_NEXT_COMMAND_VALUE		40001
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		101
#endif
#endif

#ifndef __XSMTP_CONF_H
#define __XSMTP_CONF_H
// Mail Server configurations
// User information saving configurations
const char data_dir[] = ".\\Data\\";
const char userinfo[] = "userinfo";
const char userstat[] = "userstat";

extern int mail_stat;
extern int rcpt_user_num;
extern char from_user[64];
extern char rcpt_user[MAX_RCPT_USR][30];

#endif


