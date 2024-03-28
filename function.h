#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <ctime>
using namespace std;

#define DEFAULT_PORT "2225"
#define BUF_SIZE 8192

void menu();
void mailbox_menu();

void send_data(int sockfd, string username, char buffer[]);
void config(string &username, string &password, string &smtp_port, string &pop3_port, string &server_ip, int &autoload);







