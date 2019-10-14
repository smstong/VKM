#include "framework.h"

void TransparentWindow(HWND hWnd, int alpha)
{
	LONG nRet = GetWindowLong(hWnd, GWL_EXSTYLE);
	nRet = nRet | WS_EX_LAYERED;
	SetWindowLong(hWnd, GWL_EXSTYLE, nRet);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
}

void FullscreenWindow(HWND hWnd)
{
	LONG lWindowStyle = GetWindowLong(hWnd, GWL_STYLE);
	lWindowStyle &= ~WS_SYSMENU; // remove system menu
	lWindowStyle &= ~WS_CAPTION; // remove caption
	SetWindowLong(hWnd, GWL_STYLE, lWindowStyle);

	ShowWindow(hWnd, SW_MAXIMIZE);
}