// Demo_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DnsServer.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
#define MAXFD 10

void fds_add(SOCKET fds[], SOCKET fd)
{
    for (int i = 0; i < MAXFD; ++i)
    {
        if (fds[i] == INVALID_SOCKET)
        {
            fds[i] = fd;
            break;
        }
    }
}

string charPtrToString(char* toConvert)
{
    string str;
    for (int i = 0; toConvert[i]; i++)
        str += toConvert[i];

    return str;
}
string searchDkim(string searchString) {
    fs::current_path("dkim_pub");//cd dkim_pub
    std::ifstream file(searchString + ".txt");//file name ?
    std::string line;
    int lineNumber = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.find(searchString) != std::string::npos) {
                fs::current_path("..");
                return line;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
    }
    fs::current_path("..");
    return "";
}

void removeDkimLine(string searchString) {
    std::ifstream inputFile("dkimpub.txt");
    std::ofstream tempFile("temp.txt");

    if (!inputFile.is_open() || !tempFile.is_open()) {
        std::cerr << "Error: Unable to open files\n";
        return;
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find(searchString) != std::string::npos) {
            tempFile << line << '\n';
        }
    }

    inputFile.close();
    tempFile.close();

    if (std::remove("dkimpub.txt") != 0) {
        std::cerr << "Error: Unable to remove original file\n";
        return;
    }

    if (std::rename("temp.txt", "dkimpub.txt") != 0) {
        std::cerr << "Error: Unable to rename temporary file\n";
    }
}

void writeDkimToFile(string text) {
    //overwrite the exist value
    //removeDkimLine(text);
    string temp = text;
    size_t pos = temp.find("email._domainkey.");
     temp = temp.substr(pos + 17);

    size_t blank_pos = temp.find(". ");
    string domain_name = temp.substr(0, blank_pos);

    fs::current_path("dkim_pub");

    std::ofstream file(domain_name+".txt", std::ios::out);
    if (file.is_open()) {
        file << text << "\n";
        file.close();
        std::cout << "save dkim public key successfully.\n";
    }
    else {
        std::cerr << "Unable to open the file.\n";
    }
    fs::current_path("..");
}

string ip(string domain)
{
    string word;
    string found;
    string notfound = "Website not found";

    ifstream obj("dns.txt");


    string temp = domain;

    while (obj >> word)
    {
        if (word == temp)
        {
            obj >> found;
            return found;
        }
    }
    obj.close();
    return notfound;
}
int main()
{
    cout << "======================== DNS SERVER Started ============================="<<endl;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        cout << "Socket creation failed" << endl;
        return 1;
    }

    int port = 53;

    sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int res = bind(sockfd, (sockaddr*)&saddr, sizeof(saddr));
    if (res == SOCKET_ERROR)
    {
        cout << "Bind failed" << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    listen(sockfd, 5);

    SOCKET fds[MAXFD];
    for (int i = 0; i < MAXFD; ++i)
    {
        fds[i] = INVALID_SOCKET;
    }

    fds_add(fds, sockfd);

    while (true)
    {
        fd_set fdset;
        FD_ZERO(&fdset);

        int maxfd = -1;

        for (int i = 0; i < MAXFD; i++)
        {
            if (fds[i] == INVALID_SOCKET)
                continue;

            FD_SET(fds[i], &fdset);

            if (fds[i] > maxfd)
                maxfd = fds[i];
        }

        timeval tv = { 2, 0 };

        int n = select(maxfd + 1, &fdset, NULL, NULL, &tv);
        if (n == SOCKET_ERROR)
        {
            cout << "Select error" << endl;
            break;
        }

        else if (n == 0)
        {
            // Timeout
        }

        else
        {
            for (int i = 0; i < MAXFD; ++i)
            {
                if (fds[i] == INVALID_SOCKET)
                    continue;

                if (FD_ISSET(fds[i], &fdset))
                {
                    if (fds[i] == sockfd)
                    {
                        sockaddr_in caddr;
                        int len = sizeof(caddr);

                        SOCKET c = accept(sockfd, (sockaddr*)&caddr, &len);
                        if (c == INVALID_SOCKET)
                        {
                            cout << "Accept failed" << endl;
                            continue;
                        }

                        fds_add(fds, c);

                        cout << "Connected to client." << endl;
                    }
                    else
                    {
                        char buff[500] = { 0 };
                        int res = recv(fds[i], buff, 499, 0);
                        string strBuff = string(buff);
                        if (res <= 0)
                        {
                            cout << "Client disconnected" << endl;
                            closesocket(fds[i]);
                            fds[i] = INVALID_SOCKET;
                        }
                        else if (strBuff.substr(0, 16) == "RECEIVE DKIMPUB:") { //send to client
                            string tmp = searchDkim(strBuff.substr(16, strBuff.length() - 16));
                            if (tmp == "")
                                tmp = "Not found";
                            send(fds[i], tmp.c_str(), tmp.size(), 0);
                        }
                        else if (strBuff.substr(0, 13) == "SEND DKIMPUB:") { //receive dkimpub
                            writeDkimToFile(strBuff.substr(13, strBuff.length() - 13));
                            string ip_addr = "OK";
                            send(fds[i], ip_addr.c_str(), ip_addr.size(), 0);
                        }
                        
                        else if (strBuff.substr(0, 3) == "SPF")
                        {
                            send(fds[i], "OK", 2, 0);
                            //get username
                            res = recv(fds[i], buff, 499, 0);
                            buff[res] = '\0';
                            send(fds[i], "OK", 2, 0);

                            string sender = string(buff);
                            string path = "spf\\ip_mapping.txt";
                            fstream file(path);
                            char byte;
                            string ip = "";

                            if (file.is_open())
                            {

                                while (file.get(byte))
                                {
                                    ip += byte;
                                }

                                int pos = ip.find(sender);
                                pos = pos + sender.length() + 1;
                                ip = ip.substr(pos);

                                int end = ip.find("\n");
                                ip = ip.substr(0, end);
                                file.close();
                               
                            }
                            //check spf
                            string sender_domain = sender.substr(sender.find('@') + 1);
                            path = "spf\\" + sender_domain + ".txt";
                            file.open(path);
                            if (file.is_open())
                            {
                                string content = "";
                                while (file.get(byte))
                                {
                                    content += byte;
                                    
                                }
                                int pos = content.find(ip);
                                //if found IP return 1 
                                if (pos != std::string::npos)
                                {
                                    send(fds[i], "1", 1, 0);
                                    res = recv(fds[i], buff, 499, 0);
                                    buff[res] = '\0';
                                    cout << "SPF pass" << endl;
                                }
                                else
                                {
                                    send(fds[i], "0", 1, 0);
                                    res = recv(fds[i], buff, 499, 0);
                                    buff[res] = '\0';
                                    cout << "SPF failed" << endl;
                                }
                                file.close();

                            }
                           
                        }
                        else
                        {
                            cout << "Domain requested: " << buff << endl;
                            string domain = charPtrToString(buff);

                            memset(buff, 0, sizeof(buff));

                            string ip_addr = ip(domain);
                            cout << "Matching IP: " << ip_addr << endl;

                            send(fds[i], ip_addr.c_str(), ip_addr.size(), 0);
                        }
                    }
                }
            }
        }
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}