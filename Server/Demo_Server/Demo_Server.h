#pragma once

#include "Resource.h"
int reply(char buffer[], SOCKET clientSocket) {

	if (string(buffer) == "HELO") {
		send(clientSocket, reply_code[6], strlen(reply_code[6]), 0);
		return 1;
	}
	else if (string(buffer) == "QUIT")
	{
		send(clientSocket, reply_code[5], strlen(reply_code[5]), 0);
	}
	else
	{
		send(clientSocket, reply_code[13], strlen(reply_code[13]), 0);
	}
	return 0;
}