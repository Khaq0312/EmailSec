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
#include <ctime>

#include <jsoncpp/json/json.h>

#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Path.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include <filesystem>
using namespace std;
using namespace Poco::Net;
namespace fs = std::filesystem;
#define DEFAULT_PORT "2225"
#define BUF_SIZE 8192

void menu();
void mailbox_menu();

void send_data(int sockfd, string username, char buffer[]);
void config(string &username, string &password, string &smtp_port, string &pop3_port, string &server_ip, int &autoload);
bool checkExistedFile(string filename);
long double fileSize(string filename);





