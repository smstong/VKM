#include "framework.h"
#include <shellapi.h>
#include "TrackIcon.h"
#include "Resource.h"

// Hide the main window and show a icon on taskbar
BOOL TrackIcon(HWND hWnd, BOOL bTrak)
{
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 0;
	if (bTrak)
	{
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_TRACKICON_MSG;
		nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_VKM));
		swprintf(nid.szTip, sizeof(nid.szTip)/sizeof(wchar_t), L"%s", L"VKM");

		return Shell_NotifyIcon(NIM_ADD, &nid);
	}
	else
	{
		ShowWindow(hWnd, SW_SHOWNA);
		return Shell_NotifyIcon(NIM_DELETE, &nid);
	}

}
// Hanlde message from icon
LRESULT TrackIconHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_LBUTTONDBLCLK:
		//TrackIcon(hWnd, FALSE);
		//ShowWindow(hWnd, SW_SHOWNORMAL);
		//
		HMENU hMenu = CreatePopupMenu();
		if (hMenu) {
			AppendMenu(hMenu, MF_STRING, 1, L"EXIT");
			AppendMenu(hMenu, MF_STRING, 2, L"About");

			POINT pt;
			GetCursorPos(&pt);
			int sel = TrackPopupMenuEx(hMenu, TPM_LEFTBUTTON | TPM_LEFTALIGN | TPM_RETURNCMD,
				pt.x, pt.y, hWnd, NULL);
			DestroyMenu(hMenu);
			
			if (sel == 1) {
				DestroyWindow(hWnd);
			}
			else if (sel == 2) {
				MessageBox(NULL,
					L"VKM (Virtual Keyboard Mouse)\r\njinqiangzhao2017@gmail.com",
					L"About", MB_OK);
			}
			break;
		}
	}
	
	return 0;
}
