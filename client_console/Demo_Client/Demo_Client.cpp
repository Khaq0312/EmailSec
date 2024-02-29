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

	char buffer[BUF_SIZE];
	int recvMessage = recv(client, buffer, BUF_SIZE, 0);
	buffer[recvMessage] = '\0';
	cout << "S: " << buffer;

	int login = 0;
	string input;
	do {
		cout << "C: ";
		getline(cin, input);
		send(client, input.c_str(), input.length(), 0);

		recvMessage = recv(client, buffer, BUF_SIZE, 0);
		buffer[recvMessage] = '\0';
		if (strncmp(buffer, "235 Authentication successful", strlen("235 Authentication successful"))) {
			login = 1;
		}
		cout << "S: " << buffer;
		if (input == "MY MAIL" && login == 1) {
			//filename
			cout << "C: ";
			getline(cin, input);
			send(client, input.c_str(), input.length(), 0);
			Sleep(1);

			while (1) {
				recvMessage = recv(client, buffer, BUF_SIZE, 0);
				buffer[recvMessage] = '\0';
				if (recvMessage == 1 && string(buffer) == "") {
					// End of file
					break;
				}
				printf("%.*s", recvMessage, buffer); // Print only received data, not the entire buffer
			}
			cout << endl;
		}

	} while ((string)input != "QUIT");

	closesocket(client);
	WSACleanup();
	return 0;

}
