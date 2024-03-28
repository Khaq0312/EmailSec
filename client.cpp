#include "function.h"
string username = "", password = "", smtp_port= "", pop3_port="", server_ip="";
int autoload = 0;
char buffer[BUF_SIZE] = {0};
int recvMessage = 0;

int main()
{
    int sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 == -1)
    {
        perror("socket");
        return 1;
    }

    int sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd2 == -1)
    {
        perror("socket");
        return 1;
    }

    config(username, password, smtp_port, pop3_port, server_ip, autoload);

    struct addrinfo *addrResult = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(server_ip.c_str(), smtp_port.c_str(), &hints, &addrResult);
    if (result != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << gai_strerror(result) << std::endl;
        close(sockfd1);
        return 1;
    }

    result = connect(sockfd1, addrResult->ai_addr, addrResult->ai_addrlen);
    if (result == -1)
    {
        perror("server is not working");
        close(sockfd1);
        return 1;
    }

    result = getaddrinfo(server_ip.c_str(), pop3_port.c_str(), &hints, &addrResult);
    if (result != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << gai_strerror(result) << std::endl;
        close(sockfd2);
        return 1;
    }

    result = connect(sockfd2, addrResult->ai_addr, addrResult->ai_addrlen);
    if (result == -1)
    {
        perror("server is not working");
        close(sockfd2);
        return 1;
    }

    recvMessage = read(sockfd1, buffer, BUF_SIZE);
    buffer[recvMessage] = '\0';

    recvMessage = read(sockfd2, buffer, BUF_SIZE);
    buffer[recvMessage] = '\0';
    
    //pop3
    string command = "USER " + username + "\r\n"; 
    send(sockfd2, command.c_str(), strlen(command.c_str()),0);
    recvMessage = read(sockfd2, buffer, BUF_SIZE);
    buffer[recvMessage] = '\0';

    command = "PASS " + password + "\r\n";
    send(sockfd2, command.c_str(), strlen(command.c_str()),0);
    recvMessage = read(sockfd2, buffer, BUF_SIZE);
    buffer[recvMessage] = '\0';

    int choice;
    time_t lastTime = 0;
    while(1)
    {
        time_t currentTime = time(nullptr);
        if(currentTime - lastTime >= autoload)
        {
            //pop3 list
            lastTime = currentTime;
        }
        
        menu();
        cout<<"Ban chon: ";
        cin>>choice;

        if(choice == 1)
        {
            cout <<"\nDay la thong tin soan email: (neu khong dien, nhan enter de bo qua)"<<endl;
            send_data(sockfd1, username, buffer);
        }
        if(choice == 2)
        {

        }
        if(choice == 3)
        {
            break;
        }
        sleep(10);
    }
            
    close(sockfd1);
    close(sockfd2);
    freeaddrinfo(addrResult);
    return 0;
}




