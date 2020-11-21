// Project1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Project1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	wchar_t exePath[256] = { 0 };
	wchar_t folder[256] = { 0 };
	GetModuleFileName(NULL, exePath, sizeof(exePath)/sizeof(exePath[0])-1);
	int lastBackSlash = 0;
	int i = 0;
	for (i = 0; i < lstrlen(exePath); i++)
	{
		if (exePath[i] == L'\\') {
			lastBackSlash = i;
		}
	}
	for (i = 0; i < lastBackSlash; i++) {
		folder[i] = exePath[i];
	}
	folder[i] = L'\0';

	SetCurrentDirectory(folder);

	SetCurrentDirectory(folder);
	GetCurrentDirectory(255, folder);

	MessageBox(NULL, folder, L"TEST", MB_OK);


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   LONG nRet = GetWindowLong(hWnd, GWL_EXSTYLE);
   nRet = nRet | WS_EX_LAYERED;
   SetWindowLong(hWnd, GWL_EXSTYLE, nRet);


   LONG lWindowStyle = GetWindowLong(hWnd, GWL_STYLE);
   lWindowStyle &= ~WS_SYSMENU; //去掉系统菜单
   lWindowStyle &= ~WS_CAPTION; //去掉标题栏
   SetWindowLong(hWnd, GWL_STYLE, lWindowStyle);

   SetTimer(hWnd, NULL, 1000, NULL);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_LBUTTONDOWN:
		SetLayeredWindowAttributes(hWnd, 0, 123, LWA_ALPHA);
		ShowWindow(hWnd, SW_MAXIMIZE);
	/*{
		HMENU hMenu = CreatePopupMenu();
		if (hMenu) {
			AppendMenu(hMenu, MF_STRING, 1, L"EXIT");
			AppendMenu(hMenu, MF_STRING, 2, L"About");

			POINT pt;
			GetCursorPos(&pt);
			int sel = TrackPopupMenuEx(hMenu, TPM_LEFTBUTTON | TPM_LEFTALIGN | TPM_RETURNCMD,
				pt.x, pt.y, hWnd, NULL);
			DestroyMenu(hMenu);

			WCHAR msg[32];
			swprintf(msg, 32, L"%d", sel);
			MessageBox(NULL, msg, L"", MB_OK);
			if (sel == 1) {
				PostQuitMessage(0);
			}
			else if (sel == 2) {
				MessageBox(NULL,
					L"VKM (Virtual Keyboard Mouse)\r\njinqiangzhao2017@gmail.com",
					L"About", MB_OK);
			}
		}
	}*/
		break;
	case WM_TIMER:
		wchar_t msg[256];
		POINT pt;
		GetCursorPos(&pt);
		swprintf(msg, 256, L"%d,%d", pt.x, pt.y);
		SetWindowText(hWnd, msg);
		SetFocus(GetDesktopWindow());
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
