#include "framework.h"
#include "SocketHelper.h"
#include "KM.h"
#include <stdio.h>
#include "Config.h"

static sockaddr_in clAddr;
static SOCKET udpSock;

void KMInit()
{
	clAddr.sin_family = AF_INET;
	clAddr.sin_port = htons(8888);
	inet_pton(AF_INET, CLIENT_IP, &clAddr.sin_addr);
	udpSock = SHCreateUDPSocket(UDP_PORT);
	if (udpSock == NULL) {
		MessageBox(NULL, L"CreateUDPSocket() failed\n", L"ERROR", MB_OK);
		exit(1);
	}
}

void KMExit()
{
	if (udpSock) {
		closesocket(udpSock);
		udpSock = NULL;
	}
}

void KMSendMouseEvent(int x, int y, int ds, int btn[3])
{
	char buf[256] = { 0 };
	sprintf_s(buf, sizeof(buf), "type=%d,dx=%d,dy=%d,ds=%d,btn_l=%d,btn_r=%d,btn_m=%d", 
		1, x, y, ds, btn[0], btn[1],btn[2]);
	sendto(udpSock, buf, strlen(buf), 0, (sockaddr*)&clAddr, sizeof(clAddr));
}

void KMSendKeyEvent(int key, int state)
{
	char buf[256] = { 0 };
	sprintf_s(buf, sizeof(buf), "type=%d,key=%d,state=%d",
		2, key, state);
	sendto(udpSock, buf, strlen(buf), 0, (sockaddr*)&clAddr, sizeof(clAddr));
}