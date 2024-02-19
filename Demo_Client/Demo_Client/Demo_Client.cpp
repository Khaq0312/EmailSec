// Demo_Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Demo_Client.h"
#include "afxsock.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib, "ws2_32.lib")

// The one and only application object

CWinApp theApp;

using namespace std;

const char* mailFromCommand = "MAIL FROM:<alice@example.com>";
const char* rcptToCommand = "RCPT TO:<bob@example.com>";
const char* dataCommand = "DATA";
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed with error: " << result << std::endl;
		return 1;
	}

	// Create socket
	SOCKET client = INVALID_SOCKET;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET) {
		std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	struct addrinfo* addrResult = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &addrResult);
	if (result != 0) {
		std::cerr << "getaddrinfo failed with error: " << WSAGetLastError() << std::endl;
		closesocket(client);
		WSACleanup();
		return 1;
	}

	result = connect(client, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
		closesocket(client);
		WSACleanup();
		return 1;
	}

	const char* helo = "HELO";
	send(client, helo, sizeof(helo), 0);

	char buffer[BUF_SIZE];
	int message = recv(client, buffer, BUF_SIZE, 0);
	buffer[message] = '\0';
	cout << buffer << std::endl;
	int option;
	do {
		
		/*int message = recv(client, buffer, BUF_SIZE, 0);
		if (message > 0) {
			buffer[message] = '\0';
			std::cout << buffer << std::endl;
		}*/
		cout << "continue? (0:No, 1: MAIL): ";
		cin >> option;
		if (option != 0)
		{
			send(client, mailFromCommand, strlen(mailFromCommand), 0);
			send(client, rcptToCommand, strlen(rcptToCommand), 0);
			send(client, dataCommand, strlen(dataCommand), 0);
		}
		
		/*
		else if (message == 0) {
			std::cerr << "Connection closing..." << std::endl;
		}
		else {
			std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;}*/
		
	} while (option != 0);

	
	const char* quit = "0";
	send(client, quit, strlen(quit), 0);

	message = recv(client, buffer, BUF_SIZE, 0);
	buffer[message] = '\0';
	cout << buffer << endl;

	closesocket(client);
	WSACleanup();
	return 0;
	
}
