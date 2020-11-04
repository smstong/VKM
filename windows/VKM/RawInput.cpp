#include "framework.h"
#include "RawInput.h"
#include <strsafe.h>
#include "KM.h"
#include "WindowHelper.h"
#include "ini.h"

extern HWND g_hMainWnd;

int g_win_screen_w = 0;
int g_win_screen_h = 0;
int g_linux_screen_w = 0;
int g_linux_screen_h = 0;
int g_linux_screen_x = 0;
int g_linux_screen_y = 0;

// Register RAW INPUT
void RIInit()
{
	ini_t* config = ini_load("config.ini");
	g_win_screen_w = atoi(ini_get(config, "SERVER", "WIN_SCREEN_W"));
	g_win_screen_h = atoi(ini_get(config, "SERVER", "WIN_SCREEN_H"));
	g_linux_screen_w = atoi(ini_get(config, "CLIENT", "LINUX_SCREEN_W"));
	g_linux_screen_h = atoi(ini_get(config, "CLIENT", "LINUX_SCREEN_H"));
	g_linux_screen_x = atoi(ini_get(config, "CLIENT", "LINUX_SCREEN_X"));
	g_linux_screen_y = atoi(ini_get(config, "CLIENT", "LINUX_SCREEN_Y"));

	ini_free(config);

	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = RIDEV_INPUTSINK ;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = g_hMainWnd;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = g_hMainWnd;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
		//registration failed. Call GetLastError for the cause of the error
		int x = GetLastError();
		MessageBox(NULL, L"ERROR", L"reigester()", MB_OK);
		PostQuitMessage(0);
	}
}

// Message Handler for WM_INPUT
LRESULT RIOnWMInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT dwSize = 0;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
		sizeof(RAWINPUTHEADER));
	LPBYTE lpb = (BYTE*)malloc(dwSize);
	if (lpb == NULL)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);;
	}

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
		sizeof(RAWINPUTHEADER)) != dwSize)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);;
	}

	RAWINPUT* raw = (RAWINPUT*)lpb;

	static int firstEnterLinux = 0; // mouse move from Win to Linux
	static int inLinux = 0;			// mouse stay in Linux
	

	// Keyboard input
	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		if (inLinux) {

			int key = raw->data.keyboard.VKey;
			int state = 0;
			if ((raw->data.keyboard.Flags & 0x1) == 0) {
				state = 1;
			}
			if (raw->data.keyboard.Flags & RI_KEY_BREAK) {
				state = 2;
			}
			KMSendKeyEvent(key, state);
		}
	}
	// Mouse input
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);

		static int vx = 0;				// virtual mouse X
		static int vy = 0;				// virtual mouse Y
		
		if (vx< g_win_screen_w) {
			vx = ptCursor.x;
			vy = ptCursor.y;
		}
		int vxOld = vx;					// last pos
		int vyOld = vy;

		vx += raw->data.mouse.lLastX;
		vy += raw->data.mouse.lLastY;
		if (vx > g_win_screen_w + g_linux_screen_w) {
			vx = g_win_screen_w + g_linux_screen_w;
		}
		//wchar_t msg[32];
		//swprintf(msg, 32, L"%d", vx);
		//SetWindowText(g_hMainWnd, msg);

		if (vx >= g_win_screen_w) {
			if (vxOld < g_win_screen_w) {
				firstEnterLinux = 1;
			}
			else {
				firstEnterLinux = 0;
			}
			inLinux = 1;
			SetCursorPos(g_win_screen_w, ptCursor.y); // fix cursor pos
			// Make all input to current transparent window, so other apps not affected.
			FullscreenWindow(g_hMainWnd);
			SetForegroundWindow(g_hMainWnd);
			SetActiveWindow(g_hMainWnd);	// Make current Window deactive
			SetFocus(g_hMainWnd);

		}
		else {
			inLinux = 0;
			ShowWindow(g_hMainWnd, SW_HIDE);
		}

		if (inLinux) {
			int btns[3] = { 0,0,0 };
			int ds = 0;
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				btns[0] = 1;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			{
				btns[0] = 2;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
			{
				btns[1] = 1;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
			{
				btns[1] = 2;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
			{
				btns[2] = 1;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
			{
				btns[2] = 2;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
				ds = (short)raw->data.mouse.usButtonData;
			}
			int ajustx = 0;
			if (firstEnterLinux) {
				ajustx = -g_linux_screen_w;
			}
			KMSendMouseEvent(
				raw->data.mouse.lLastX + ajustx,
				raw->data.mouse.lLastY,
				ds,
				btns);
		}
	}
	if (lpb) {
		free(lpb);
		lpb = NULL;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
