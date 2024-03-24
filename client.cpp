#include "function.h"

char buffer[BUF_SIZE] = {0};
int recvMessage = 0;

int main()
{
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1)
    {
        perror("socket");
        return 1;
    }

    struct addrinfo *addrResult = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &addrResult);
    if (result != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << gai_strerror(result) << std::endl;
        close(client);
        return 1;
    }

    result = connect(client, addrResult->ai_addr, addrResult->ai_addrlen);
    if (result == -1)
    {
        perror("server is not working");
        close(client);
        return 1;
    }

    recvMessage = read(client, buffer, BUF_SIZE);
    buffer[recvMessage] = '\0';
    
    
    int choice;
    string a;

    while(1)
    {
        menu();
        cout<<"Ban chon: ";
        cin>>choice;

        if(choice == 1)
        {
            cout <<"\nDay la thong tin soan email: (neu khong dien, nhan enter de bo qua)"<<endl;
            cin>>a;
            while(a == "\n")
                cin>>a;
        }
        if(choice == 2)
        {

        }
        if(choice == 0)
        {
            break;
        }
    }
            
    close(client);
    freeaddrinfo(addrResult);
    return 0;
}




