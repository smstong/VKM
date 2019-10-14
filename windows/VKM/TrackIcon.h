#pragma once
#define WM_TRACKICON_MSG (WM_USER + 1)

BOOL TrackIcon(HWND hWnd, BOOL bTrak);
LRESULT TrackIconHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
