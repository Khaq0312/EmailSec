 #include "pch.h"
//#include "function.h"
//
//using namespace std;
//
//string resolveDNS(string domain) {
//    
//    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (sockfd == INVALID_SOCKET) {
//        cerr << "Socket creation failed" << endl;
//        WSACleanup();
//        return "";
//    }
//
//    int port = 53;
//    string server_ip = "127.0.0.1";
//
//    sockaddr_in serverAddr;
//    memset(&serverAddr, 0, sizeof(serverAddr));
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(port);
//    inet_pton(AF_INET, server_ip.c_str(), &serverAddr.sin_addr);
//    int res = connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
//    if (res == SOCKET_ERROR) {
//        cerr << "Connection failed" << endl;
//        closesocket(sockfd);
//        WSACleanup();
//        return "";
//    }
//
//    // Send domain name to server
//    send(sockfd, domain.c_str(), domain.size(), 0);
//
//    string result;
//    // Receive IP address from server
//    char ip_buffer[500] = { 0 };
//    int bytes_received = recv(sockfd, ip_buffer, sizeof(ip_buffer), 0);
//    if (bytes_received > 0) {
//        //cout << "IP address: " << ip_buffer << endl;
//        result = string(ip_buffer);
//    }
//    else {
//        //cerr << "Failed to receive IP address from server" << endl;
//        result = "Not found";
//    }
//
//    closesocket(sockfd);
//    WSACleanup();
//
//    return result;
//}
//
//string extractDKIMPUB(string input) {
//    // Find the position of "p=" in the input string
//    size_t pos = input.find("p=");
//    if (pos == std::string::npos) {
//        return "Not found"; // "p=" not found
//    }
//
//    // Extract the substring starting from "p="
//    std::string publicKey = input.substr(pos + 2); // Skip "p=" (2 characters)
//
//    // Remove any trailing whitespace or semicolon
//    size_t endPos = publicKey.find('"');
//    if (endPos != std::string::npos) {
//        publicKey = publicKey.substr(0, endPos); // Exclude the double quote
//    }
//
//    return publicKey;
//}
//
//string exec(const char* cmd) {
//    std::array<char, 128> buffer;
//    std::string result;
//    std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
//    if (!pipe) {
//        throw std::runtime_error("popen() failed!");
//    }
//    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
//        result += buffer.data();
//    }
//    return result;
//}
//
//string generateRSAKey() {
//    string command = "openssl genrsa -out dkim_private.pem 2048";
//    int result = system(command.c_str());
//    return exec("openssl rsa -in dkim_private.pem -pubout -outform der 2> nul | openssl base64 -A");
//}
//
//int verifyKey(string key, string filemail, string filesignature) {
//    //string command = "echo "+key+" | openssl base64 -d -A -in -inform der -out publickey.pem";
//    //int result = system(command.c_str());
//    string command = "echo " + key + " > tmp";
//    system(command.c_str());
//    command = "openssl enc -base64 -d -in tmp -out public.key";
//    system(command.c_str());
//    string cmd = "openssl dgst -sha256 -verify public.key -signature " + filesignature + " " + filemail;
//    string result = exec(cmd.c_str());
//    if (result == "Verified OK\n")
//        return 1;
//    return 0;
//}
//int signMail(string filemail, string filesignature) {
//    string command = "openssl dgst -sha256 -sign dkim_private.pem -out " + filesignature + " " + filemail;
//    system(command.c_str());
//    return 0;
//}
//void sendPub(string domain) {
//    //string p = generateRSAKey();
//    string selector = "email";
//    string data = "SEND DKIMPUB:" + selector + "._domainkey." + domain + ". 86400 IN TXT \"v = DKIM1; k = rsa; p=" + p + "\"";
//    resolveDNS(data);
//}
//string getDkim() {
//    string tmp = resolveDNS("RECEIVE DKIMPUB:sheng.domain");
//    return extractDKIMPUB(tmp);
//}
//
//int checKDKIM(string sender_domain) {
//    char buffer[1024];
//    WSADATA wsaData;
//    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (result != 0) {
//        //std::cerr << "WSAStartup failed with error: " << result << std::endl;
//        return 1;
//    }
//
//    // Create socket
//    int client1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (client1 == INVALID_SOCKET) {
//        //std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
//        WSACleanup();
//        return 1;
//    }
//
//    struct addrinfo* addrResult = NULL;
//    struct addrinfo hints;
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//
//    result = getaddrinfo("127.0.0.1", "53", &hints, &addrResult);
//    if (result != 0) {
//        //std::cerr << "getaddrinfo failed with error: " << WSAGetLastError() << std::endl;
//        closesocket(client1);
//        WSACleanup();
//        return false;
//    }
//
//
//    result = connect(client1, addrResult->ai_addr, (int)addrResult->ai_addrlen);
//    if (result == SOCKET_ERROR) {
//        //std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
//        closesocket(client1);
//        WSACleanup();
//        AfxMessageBox(_T("Server is not working"), MB_OK | MB_ICONERROR);
//        return false;
//
//    }
//
//    send(client1, "DKIM", 4, 0);
//    int recvMessage = recv(client1, buffer, 1024, 0);
//    buffer[recvMessage] = '\0';
//    
//    send(client1, sender_domain.c_str(), strlen(sender_domain.c_str()), 0);
//    recvMessage = recv(client1, buffer, 1024, 0);
//    buffer[recvMessage] = '\0';
//
//    recvMessage = recv(client1, buffer, 1024, 0);
//    buffer[recvMessage] = '\0';
//    send(client1, "OK", 2, 0);
//
//    closesocket(client1);
//    WSACleanup();
//
//    if (std::string(buffer) == "1")
//        return 1;
//    else
//        return 0;
//}
//
//int checkSPF(std::string sender_domain)
//{
//    char buffer[1024];
//    WSADATA wsaData;
//    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (result != 0) {
//        //std::cerr << "WSAStartup failed with error: " << result << std::endl;
//        return 1;
//    }
//
//    // Create socket
//    int client1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (client1 == INVALID_SOCKET) {
//        //std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
//        WSACleanup();
//        return 1;
//    }
//
//    struct addrinfo* addrResult = NULL;
//    struct addrinfo hints;
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//
//    result = getaddrinfo("127.0.0.1", "53", &hints, &addrResult);
//    if (result != 0) {
//        //std::cerr << "getaddrinfo failed with error: " << WSAGetLastError() << std::endl;
//        closesocket(client1);
//        WSACleanup();
//        return false;
//    }
//
//  
//    result = connect(client1, addrResult->ai_addr, (int)addrResult->ai_addrlen);
//    if (result == SOCKET_ERROR) {
//        //std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
//        closesocket(client1);
//        WSACleanup();
//        AfxMessageBox(_T("Server is not working"), MB_OK | MB_ICONERROR);
//        return false;
//
//    }
//
//    send(client1, "SPF", 3, 0);
//    int recvMessage = recv(client1, buffer, 1024, 0);
//    buffer[recvMessage] = '\0';
//
//    send(client1, sender_domain.c_str(), strlen(sender_domain.c_str()), 0);
//    recvMessage = recv(client1, buffer, 1024, 0);
//    buffer[recvMessage] = '\0';
//
//    recvMessage = recv(client1, buffer, 1024, 0);
//    buffer[recvMessage] = '\0';
//    send(client1, "OK", 2, 0);
//    
//    closesocket(client1);
//    WSACleanup();
//
//    if (std::string(buffer) == "1")
//        return 1;
//    else
//        return 0;
//}