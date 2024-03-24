#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <cstring>
using namespace std;

#define DEFAULT_PORT "2225"
#define BUF_SIZE 8192

void menu();
void send_data();