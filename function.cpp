#include "function.h"

void menu(){
    cout<<"Vui long chon menu: "<<endl;
    cout<<"1. De gui email"<<endl;
    cout<<"2. De xem danh sach email da nhan"<<endl;
    cout<<"3. Thoat"<<endl;
}

void mailbox_menu()
{
    cout<<"\nDay la danh sach folder trong mailbox cua ban"<<endl;
    cout<<"1. Inbox"<<endl;
    cout<<"2. Project"<<endl;
    cout<<"3. Important"<<endl;
    cout<<"4. Work"<<endl;
    cout<<"5. Spam"<<endl;
}
void send_data(int sockfd, string username, char buffer[])
{
    string to, CC, BCC, subject, content, attachment;
    int wanna_attach, n_file;
    vector<string> file;
    cin.ignore();
    cout<<"To: ";
    getline(cin,to);
    cout<<"CC: ";
    getline(cin, CC);
    cout<<"BCC: ";
    getline(cin, BCC);
    cout<<"Subject: ";
    getline(cin, subject);
    cout<<"Content: ";
    getline(cin, content);
    cout<<"Co dinh kem file (1: co, 2:khong): ";
    cin>>wanna_attach;
    while(wanna_attach != 1 && wanna_attach != 2)
    {
        cout<<"Khong hop le, vui long nhap lai"<<endl;
        cout<<"Co dinh kem file: ";
        cin>>wanna_attach;
    }
    if(wanna_attach == 1)
    {
        cout<<"So luong file muon gui: ";
        cin>>n_file;
        if(n_file != 0)
        {
            for(int i=0;i<n_file;++i)
            {
                cout<<"Cho biet duong dan file thu "<<i+1<<": ";
                getline(cin, attachment);
                file.push_back(attachment);
            }
        }
    }

    while(to == "" && BCC == "" && CC == "")
    {
        cout<<"\nYeu cau nhap email nguoi nhan"<<endl;
        cout<<"To: ";
        getline(cin,to);
        cout<<"CC: ";
        getline(cin, CC);
        cout<<"BCC: ";
        getline(cin, BCC);
    }
    string command = "MAIL FROM:<" +username+">\r\n";
    send(sockfd,command.c_str(), strlen(command.c_str()),0);
    int recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';
    string rcpt = "";
    if(to != "")
        rcpt += to;
    if(CC != "")
        if(rcpt == "")
            rcpt+=CC;
        else 
            rcpt = rcpt + ", " + CC;
    if(BCC != "")
        if(rcpt == "")
            rcpt+= BCC;
        else
            rcpt = rcpt + ", " + BCC;

    size_t pos = rcpt.find(", ");
    size_t old_pos = 0;
    while(pos != std::string::npos)
    {
        string temp = rcpt.substr(old_pos,pos);
        command = "RCPT TO:<" + temp + ">\r\n";
        send(sockfd, command.c_str(), strlen(command.c_str()),0);
        recvMessage = read(sockfd,buffer,BUF_SIZE);
        buffer[recvMessage] = '\0';
        old_pos = pos + 2;
        rcpt = rcpt.substr(old_pos);
        pos = rcpt.find(", ");
    }
    command = "RCPT TO:<" + rcpt + ">\r\n";
    send(sockfd, command.c_str(), strlen(command.c_str()),0);
    recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';
    

    send(sockfd, "DATA\r\n", 6,0);
    recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';

    command = "Subject: " + subject + "\nContent: " + content + "\r\n";
    send(sockfd, command.c_str(), strlen(command.c_str()),0);

    send(sockfd, ".\r\n", 3,0);
    recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';

    cout<<"\nDa gui email thanh cong\n";
    send(sockfd, "QUIT\r\n", 6,0);
    recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';
}

void config(string &username, string &password, string &smtp_port, string &pop3_port, string &server_ip, int &autoload){
    string path = "config.json";
    ifstream fIn(path);
    Json::Value root;
    Json::Reader reader;
    reader.parse(fIn,root);
    username = root["username"].asString();
    password = root["password"].asString();
    smtp_port = root["SMTP"].asString();
    pop3_port = root["POP3"].asString();
    server_ip = root["server"].asString();
    autoload = root["autoload"].asInt();
}