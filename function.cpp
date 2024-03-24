#include "function.h"
void menu(){
    cout<<"Vui long chon menu: "<<endl;
    cout<<"1. De gui email"<<endl;
    cout<<"2. De xem danh sach email da nhan"<<endl;
    cout<<"3. Thoat"<<endl;
}

void send_data(int sockfd, string username)
{
    string to, CC, BCC, subject, content, attachment;
    int wanna_attach, n_file;
    vector<string> file;
    cout<<"To: ";
    cin>>to;
    cout<<"CC: ";
    cin>>CC;
    cout<<"BCC: ";
    cin>>BCC;
    cout<<"Subject: ";
    cin>>subject;
    cout<<"content: ";
    cin>>content;
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
                cin>>attachment;
                file.push_back(attachment);
            }
        }
    }

    while(to == "\n" && BCC == "\n" && CC == "\n")
    {
        cout<<"\nYeu cau nhap email nguoi nhan"<<endl;
        cout<<"To: ";
        cin>>to;
        cout<<"CC: ";
        cin>>CC;
        cout<<"BCC: ";
        cin>>BCC;

    }
    string command = "MAIL FROM:<" +username+">";
    send(sockfd,command.c_str(), strlen(command.c_str()),0);

    string rcpt = to + ", " + CC + ", " + BCC;



}