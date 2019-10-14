#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

/******************************************************
*
* Windows Socket Routines
*
*******************************************************/
int SHInit();
void SHExit();
SOCKET SHCreateUDPSocket(const char* port);