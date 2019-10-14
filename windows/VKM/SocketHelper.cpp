#include "SocketHelper.h"
#pragma comment(lib, "Ws2_32.lib")

int SHInit()
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		MessageBox(NULL, L"WSAStartup failed\n", L"Error", MB_OK);
		return 1;
	}
	return 0;
}
void SHExit()
{
	WSACleanup();
}

SOCKET SHCreateUDPSocket(const char* port)
{
	SOCKET udpSock = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	LRESULT iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		MessageBox(NULL, L"getaddrinfo() failed\n", L"Error", MB_OK);
		return NULL;
	}
	
	udpSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (udpSock == INVALID_SOCKET) {
		MessageBox(NULL, L"socket() failed\n", L"Error", MB_OK);
		return NULL;
	}

	iResult = bind(udpSock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		MessageBox(NULL, L"bind() failed\n", L"Error", MB_OK);
		return NULL;
	}

	freeaddrinfo(result);
	return udpSock;
}

