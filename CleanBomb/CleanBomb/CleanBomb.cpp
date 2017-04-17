// CleanBomb.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CleanBomb.h"
#include "windows.h"
#include<time.h>

#define MAX_LOADSTRING 100
#define GAMESIZE 12

int bombsize = 30;
int gamesize = GAMESIZE;
int bombnum = 30;
int check = gamesize*gamesize - bombnum;
int gameflag[GAMESIZE+1][GAMESIZE+1] = { 0 };
int shownum[GAMESIZE+1][GAMESIZE+1] = { 0 };
int clickflag[GAMESIZE + 1][GAMESIZE + 1] = { 0 };
TCHAR szBuf[4];

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLEANBOMB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLEANBOMB));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

void ReflashBomb(int x, int y)
{
	if (x>0 && y>0)				 shownum[x - 1][y - 1]++;
	if (y>0)						 shownum[x][y - 1]++;
	if (x<gamesize&&y>0)			 shownum[x + 1][y - 1]++;
	if (x>0)						 shownum[x - 1][y]++;
	if (x<gamesize)               shownum[x + 1][y]++;
	if (x>0 && y<gamesize)			 shownum[x - 1][y + 1]++;
	if (y<gamesize)				 shownum[x][y + 1]++;
	if (x<gamesize&&y<gamesize)	 shownum[x + 1][y + 1]++;
}

void InitGame()
{
	bombsize = 30;
	gamesize = GAMESIZE;
	bombnum = 20;
	int i, j;
	for (i = 0; i < gamesize; i++)
	{
		for (j = 0; j < gamesize; j++)
		{
			gameflag[i][j] = 0;
			shownum[i][j] = 0;
			clickflag[i][j] = 0;
		}
	}
	check = gamesize*gamesize - bombnum;
	int x, y;
	int xtem;
	srand(unsigned(time(0)));
	int iBomb = bombnum;
	while (iBomb)
	{
		xtem = rand() % (gamesize*gamesize);
		x = xtem / gamesize;
		y = xtem % gamesize;
		if (gameflag[x][y] == 0)
		{
			gameflag[x][y] = 1;
			ReflashBomb(x, y);
			iBomb--;
		}
	}
}


void DrawBomb(HDC hdc,HWND hWnd)
{
	int i, j;
	RECT rect;
	for (i = 0; i < gamesize; i++)
	{
		for (j = 0; j < gamesize; j++)
		{
			Rectangle(hdc, i*bombsize, j*bombsize, (i + 1)*bombsize, (j + 1)*bombsize);
		}
	}

	SetBkMode(hdc, TRANSPARENT);
	for (i = 0; i < gamesize; i++)
	{
		for (j = 0; j < gamesize; j++)
		{
			if (clickflag[i][j] == 1){
				HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				SelectObject(hdc, hBrush);
				if (gameflag[i][j] == 0)
				{
					rect.left = i*bombsize;
					rect.right = (i + 1)*bombsize;
					rect.top = j*bombsize;
					rect.bottom = (j + 1)*bombsize;
					wsprintf(szBuf, TEXT("%d"), shownum[i][j]);
					DrawText(hdc, szBuf, wcslen(szBuf), &rect, DT_CENTER | DT_VCENTER);
					if (check == 0)
					{
						MessageBox(NULL, _T("You Win"), _T("Tip"), MB_OK);
						InitGame();
						InvalidateRect(hWnd, NULL, TRUE);
						return;
					}
				}
				else
				{
					MessageBox(NULL,_T("You Lose"),_T("Tip"),MB_OK);
					Ellipse(hdc, i*bombsize, j*bombsize, (i + 1)*bombsize, (j + 1)*bombsize);
					InitGame();
					InvalidateRect(hWnd, NULL, TRUE);
					return;
				}
			}
			else if (clickflag[i][j] == 0)
			{
				HBRUSH hBrush = (HBRUSH)GetStockObject(3);
				SelectObject(hdc, hBrush);
				Rectangle(hdc, i*bombsize, j*bombsize, (i + 1)*bombsize, (j + 1)*bombsize);
			}
			else
			{
				HBRUSH hBrush = (HBRUSH)GetStockObject(1);
				SelectObject(hdc, hBrush);
				Ellipse(hdc, i*bombsize, j*bombsize, (i + 1)*bombsize, (j + 1)*bombsize);
			}
		}
	}

}


//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLEANBOMB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CLEANBOMB);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	int mywinx, mywiny, x, y;
	int mousex, mousey;

	switch (message)
	{
	case WM_CREATE:
		GetWindowRect(hWnd, &rect);
		mywinx = rect.right - rect.left;
		mywiny = rect.bottom - rect.top;
		GetClientRect(hWnd, &rect);
		mywinx = mywinx - (rect.right - rect.left);
		mywiny = mywiny - (rect.bottom - rect.top);
		x = GetSystemMetrics(SM_CXSCREEN);
		y = GetSystemMetrics(SM_CYSCREEN);
		x = (x - bombsize*gamesize + mywinx) / 2;
		y = (y - bombsize*gamesize + mywiny) / 2;
		SetWindowPos(hWnd, NULL, x, y, bombsize*gamesize + mywinx, bombsize*gamesize + mywiny, SWP_SHOWWINDOW);
		InitGame();
		break;
	case WM_LBUTTONDOWN:
		mousex = LOWORD(lParam)/bombsize;
		mousey = HIWORD(lParam)/bombsize;
		if (clickflag[mousex][mousey] == 0) check--;
		clickflag[mousex][mousey] = 1;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_RBUTTONDOWN:
		mousex = LOWORD(lParam) / bombsize;
		mousey = HIWORD(lParam) / bombsize;
		if (clickflag[mousex][mousey] == 2)
		{
			clickflag[mousex][mousey] = 0;
		}
		else if (clickflag[mousex][mousey] == 0)
		{
			clickflag[mousex][mousey] = 2;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawBomb(hdc,hWnd);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
