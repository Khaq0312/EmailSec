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
        cout<<"Co dinh kem file(1: co, 2:khong): ";
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
                cin >> attachment;
                while(checkExistedFile(attachment) != 1 || (fileSize(attachment) / 1024 / 1024) > 3.0)
                {
                    cout<<"Dung luong qua lon hoac duong dan khong ton tai"<<endl;
                    cout<<"Vui long nhap lai duong dan: ";
                    cin >> attachment;
                }
                file.push_back(attachment);               
            }
        }
    }

    while(to == "" && BCC == "" && CC == "")
    {
        cin.ignore();
        cout<<"\nYeu cau nhap email nguoi nhan"<<endl;
        cout<<"To: ";
        getline(cin,to);
        cout<<"CC: ";
        getline(cin, CC);
        cout<<"BCC: ";
        getline(cin, BCC);
    }
    vector<string> to_user, cc_user, bcc_user;
    string temp = "";
    string rcpt = "";
    size_t old_pos = 0;

    if(to != "")
    {
        size_t pos = to.find(",");
        while(pos != std::string::npos)
        {
            temp = to.substr(0,pos);
            to_user.push_back(temp);
            
            if(rcpt.find(temp) == std::string::npos)
            {
                rcpt += temp + ", ";

            }

            old_pos = pos + 2;
            to = to.substr(old_pos);
            pos = to.find(",");

        }
        
        to_user.push_back(to);
        if(rcpt.find(to) == std::string::npos)
        {
            rcpt += to + ", ";
        }  
        
    }

    old_pos = 0;
    if(CC != "")
    {
        size_t pos = CC.find(",");
        while(pos != std::string::npos)
        {
            temp = CC.substr(0,pos);
            cc_user.push_back(temp);
            
            if(rcpt.find(temp) == std::string::npos)
            {
                rcpt += temp + ", ";

            }

            old_pos = pos + 2;
            CC = CC.substr(old_pos);
            pos = CC.find(",");

        }
        
        cc_user.push_back(CC);
        if(rcpt.find(CC) == std::string::npos)
        {
            rcpt += CC + ", ";
        }  
        
    }

    old_pos = 0;
    if(BCC != "")
    {
        size_t pos = BCC.find(",");
        while(pos != std::string::npos)
        {
            temp = BCC.substr(0,pos);
            bcc_user.push_back(temp);
            if(rcpt.find(temp) == std::string::npos)
            {
                rcpt += temp + ", ";

            }
            old_pos = pos + 2;
            BCC = BCC.substr(old_pos);
            pos = BCC.find(",");
        }
        
        bcc_user.push_back(BCC);
        if(rcpt.find(BCC) == std::string::npos)
        {
            rcpt += BCC + ", ";
        }     
    }
    rcpt = rcpt.substr(0,rcpt.length() - 2);

    string command = "MAIL FROM:<" +username+">\r\n";
    send(sockfd,command.c_str(), strlen(command.c_str()),0);
    int recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';
    
    size_t pos = rcpt.find(", ");
    old_pos = 0;
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

    MailMessage message;
    message.setSender(username);
    if(to_user.size() > 0)
    {
        for(int i=0;i<to_user.size();++i)
        {
            message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to_user[i]));
        }
    }
    if(cc_user.size() > 0)
    {
        for(int i=0;i<cc_user.size();++i)
        {
            message.addRecipient(MailRecipient(MailRecipient::CC_RECIPIENT, cc_user[i]));
        }
    }
    if(bcc_user.size() > 0)
    {
        for(int i=0;i<bcc_user.size();++i)
        {
            message.addRecipient(MailRecipient(MailRecipient::BCC_RECIPIENT, bcc_user[i]));
        }
    }

    if(subject != "")
    {
        message.setSubject(subject);

    }

    if(n_file != 0)
    {
        std::string textContent = content;
        message.setContent(textContent, MailMessage::ENCODING_8BIT);

        StringPartSource* textPartSource= new StringPartSource(textContent, "text/plain");
        message.addContent(textPartSource, MailMessage::ENCODING_8BIT);

        for(int i=0;i<n_file;++i)
        {
            Poco::Path filePath(file[i]);
            FilePartSource* filePartSource = new FilePartSource(filePath.toString());
            message.addAttachment(file[i], filePartSource);
        }
    }
    else
    {
        if(content != "")
        {
            std::string textContent = content + "\r\n";
            message.setContent(textContent, MailMessage::ENCODING_8BIT);
        }
    }
    std::ostringstream mimeContent;
    message.write(mimeContent);

    send(sockfd, mimeContent.str().c_str(), strlen(mimeContent.str().c_str()),0);
    send(sockfd, ".\r\n", 3,0);
    recvMessage = read(sockfd,buffer,BUF_SIZE);
    buffer[recvMessage] = '\0';

    cout<<"\nDa gui email thanh cong\n";
    // send(sockfd, "QUIT\r\n", 6,0);
    // recvMessage = read(sockfd,buffer,BUF_SIZE);
    // buffer[recvMessage] = '\0';
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

bool checkExistedFile(string filename)
{
    if(!fs::exists(filename))
        return 0;
    return 1;
}

long double fileSize(string filename)
{
    std::fstream file(filename, std::ios::in | std::ios::ate);
    if (!file.is_open()) {
        return -1;
    }
    return file.tellg();
}