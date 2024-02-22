// Demo_Server.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Demo_Server.h"
#include "afxsock.h"
#include "module_user.h"
#include "module_mail.h"

#pragma comment(lib, "Ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int mail_stat = 0;
int rcpt_user_num = 0;
char from_user[64] = "";
char rcpt_user[MAX_RCPT_USR][30] = { "" };
// The one and only application object

CWinApp theApp;

using namespace std;


//DWORD WINAPI function_cal(LPVOID arg)
//{
//	SOCKET* hConnected = (SOCKET*)(arg);
//	SOCKET clientSocket = *hConnected;
//
//
//	char buffer[BUF_SIZE];
//	int recvMessage;
//
//	while (1)
//	{
//		recvMessage = recv(clientSocket, buffer, BUF_SIZE, 0);
//		buffer[recvMessage] = '\0';
//		if (reply(buffer, clientSocket) == 0)
//		{
//			break;
//		}
//	}
//
//	closesocket(clientSocket);
//	delete hConnected;
//	return 0;
//}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed with error: " << result << std::endl;
		return 1;
	}

	if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
		std::cerr << "Fatal Error: MFC initialization failed" << std::endl;
		WSACleanup();
		return 1;
	}

	AfxSocketInit(nullptr);
	//CSocket server, client;
	DWORD threadID;
	HANDLE threadStatus;

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	ADDRINFO* addrResult = NULL;
	SOCKET LISTEN_SOCKET = INVALID_SOCKET;


	result = getaddrinfo("127.0.0.1", PORT, &hints, &addrResult);
	if (result != 0) {
		std::cerr << "getaddrinfo failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	LISTEN_SOCKET = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (LISTEN_SOCKET == INVALID_SOCKET)
	{
		cout << "Socket failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	result = bind(LISTEN_SOCKET, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(addrResult);
		closesocket(LISTEN_SOCKET);
		WSACleanup();
		return 1;
	}

	result = listen(LISTEN_SOCKET, MAX_CLIENTS);
	if (result != 0)
	{
		cout << "Listen socket failed" << endl;
		freeaddrinfo(addrResult);
		return 1;
	}


	cout << "================================================================\n";
	cout << "-XSMTP mail server by started..." << endl;
	do {
		SOCKET clientSocket = INVALID_SOCKET;
		clientSocket = accept(LISTEN_SOCKET, NULL, NULL);
		if (clientSocket != INVALID_SOCKET) {
			// Get the address information of the connected client
			struct sockaddr_in clientAddr;
			int clientAddrLen = sizeof(clientAddr);
			if (getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientAddrLen) == SOCKET_ERROR) {
				printf("getpeername failed with error: %d\n", WSAGetLastError());
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}

			// Convert the binary IP address to a string
			char clientIP[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));

			printf("received a connection from : %s\n", clientIP);

			SOCKET* hConnected = new SOCKET();
			*hConnected = clientSocket;
			//threadStatus = CreateThread(nullptr, 0, function_cal, hConnected, 0, &threadID);
			threadStatus = CreateThread(nullptr, 0, mail_proc, hConnected, 0, &threadID);
		}
		else
		{
			std::cerr << "Failed to accept client connection ";
			closesocket(LISTEN_SOCKET);
			WSACleanup();
			return 1;
		}
	} while (1);
	freeaddrinfo(addrResult);
	closesocket(LISTEN_SOCKET);
	WSACleanup();
	return 0;
}


