// PictureMatching.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PictureMatching.h"
#include<algorithm>
#include<time.h>
#include<stack>

#define MAX_LOADSTRING 100

#define PICTURE_SIZE 80
#define GAME_WIDTH 10
#define GAME_HIGHT 6
#define GAME_LINE_WIDTH 20
#define TIME_LENGTH 600
#define PICTURE_NUM 9
HBITMAP g_hBitmap[PICTURE_NUM];
HDC g_hdc[PICTURE_NUM];
UINT g_GameMap[GAME_HIGHT+2][GAME_WIDTH+2] = {-1};
int timelength = TIME_LENGTH;
POINT g_nowMouse, g_oldMouse;
RECT gameRect;
BOOL bRet;
BOOL bWin;
RECT timeRect;

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
	LoadString(hInstance, IDC_PICTUREMATCHING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PICTUREMATCHING));

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

BOOL CheckOver()
{
	for (int x = 1; x < GAME_WIDTH + 1; x++)
	{
		for (int y = 1; y < GAME_HIGHT + 1; y++)
		{
			if (g_GameMap[y][x] != 10) return FALSE;
		}
	}
	return TRUE;
}

BOOL Loadimage()
{
	int i;
	TCHAR szName[255];
	for (i = 0; i < PICTURE_NUM; i++)
	{
		wsprintf(szName, TEXT("image\\%d.bmp"), i);
		g_hBitmap[i] = (HBITMAP)LoadImage(0, szName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (g_hBitmap[i]==NULL)
		{
			return FALSE;
		}
		g_hdc[i] = CreateCompatibleDC(0);
		SelectObject(g_hdc[i], g_hBitmap[i]);
	}
	return TRUE;
}

VOID InitGame()
{
	timelength = TIME_LENGTH;
	g_nowMouse = { -1, -1 };
	g_oldMouse = { -1, -1 };
	bWin = FALSE;
	bRet = FALSE;
	int temp[GAME_HIGHT*GAME_WIDTH] = { 0 };
	for (int x=0; x < GAME_WIDTH + 2; x++)
	{
		for (int y=0; y < GAME_HIGHT + 2; y++)
		{
			g_GameMap[y][x] = 10;
		}
	}
	srand(time(0));
	for (int i = 0; i < GAME_HIGHT*GAME_WIDTH; i+=2)
	{
		temp[i] = rand()%PICTURE_NUM;
		temp[i + 1] = temp[i];
	}
	std::random_shuffle(temp, temp + GAME_HIGHT*GAME_WIDTH);
	for (int y = 0; y < GAME_HIGHT; y++)
	{
		for (int x = 0; x < GAME_WIDTH; x++)
		{
			g_GameMap[y+1][x+1] = temp[y*GAME_WIDTH + x];
		}
	}
}

VOID Release()
{
	for (int i = 0; i < PICTURE_NUM; i++)
	{
		if (g_hBitmap[i]) DeleteObject(g_hBitmap[i]);
		if (g_hdc[i]) DeleteObject(g_hdc[i]);
	}
}

VOID DrawGameWindow(HDC hdc)
{
	HBRUSH hbrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc,hbrush);
	Rectangle(hdc, 0, GAME_LINE_WIDTH * 2 + PICTURE_SIZE*GAME_HIGHT, 800, GAME_LINE_WIDTH * 3 + PICTURE_SIZE*GAME_HIGHT);
	hbrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(hdc, hbrush);
	Rectangle(hdc, 0, GAME_LINE_WIDTH * 2 + PICTURE_SIZE*GAME_HIGHT + 1, timelength, GAME_LINE_WIDTH * 3 + PICTURE_SIZE*GAME_HIGHT - 1);
	DeleteObject(hbrush);

}

BOOL Search(int x,int y,int value,int tx,int ty,int d,int n)
{
	BOOL flag = FALSE;
	if (n == 4) return FALSE;
	if (x >= 0 && x < GAME_WIDTH + 2 && y >= 0 && y < GAME_HIGHT + 2)
	{
		if (g_GameMap[y][x] == value&&x==tx && y==ty) flag= TRUE;
		else if (g_GameMap[y][x] == 10)
		{
			g_GameMap[y][x] = 11;
			if (
				Search(x + 1, y, value, tx, ty,1,1==d?n:(n+1))
				|| Search(x - 1, y, value, tx, ty, 1, 1 == d ? n : (n + 1))
				|| Search(x, y - 1, value, tx, ty, 2, 2 == d ? n : (n + 1))
				|| Search(x, y + 1, value, tx, ty, 2, 2 == d ? n : (n + 1))
				)
				flag = TRUE;
			g_GameMap[y][x] = 10;
		}
	}
	return flag;
}


BOOL Check(POINT oldPoint,POINT newPoint)
{
	if (g_GameMap[oldPoint.y + 1][oldPoint.x + 1] == g_GameMap[newPoint.y + 1][newPoint.x + 1] && g_GameMap[oldPoint.y + 1][oldPoint.x + 1]!=10)
	{
		return Search(newPoint.x + 2, newPoint.y + 1, g_GameMap[newPoint.y + 1][newPoint.x + 1], oldPoint.x + 1, oldPoint.y + 1,1,1)
			|| Search(newPoint.x, newPoint.y + 1, g_GameMap[newPoint.y + 1][newPoint.x + 1], oldPoint.x + 1, oldPoint.y + 1,1,1)
			|| Search(newPoint.x + 1, newPoint.y + 2, g_GameMap[newPoint.y + 1][newPoint.x + 1], oldPoint.x + 1, oldPoint.y + 1,2,1)
			|| Search(newPoint.x + 1, newPoint.y, g_GameMap[newPoint.y + 1][newPoint.x + 1], oldPoint.x + 1, oldPoint.y + 1,2,1);
	}
	return FALSE;
}

VOID DrawPlayArea(HDC hdc)
{
	if (g_nowMouse.x != g_oldMouse.x || g_oldMouse.y != g_nowMouse.y)
	{
		if (g_nowMouse.x >= 0 && g_nowMouse.x < GAME_WIDTH && g_nowMouse.y >= 0 && g_nowMouse.y < GAME_HIGHT)
		{
			if (Check(g_oldMouse,g_nowMouse))
			{
				g_GameMap[g_nowMouse.y+1][g_nowMouse.x+1] = 10;
				g_GameMap[g_oldMouse.y+1][g_oldMouse.x+1] = 10;
				if (CheckOver()) bWin = TRUE;
				g_oldMouse = { -1, -1 };
				g_nowMouse = { -1, -1 };
				timelength += 10;
			}
			else
			{
				g_oldMouse.x = g_nowMouse.x;
				g_oldMouse.y = g_nowMouse.y;
				bRet = TRUE;
			}
		}
	}
	for (int y = 0; y < GAME_HIGHT; y++)
	{
		for (int x = 0; x < GAME_WIDTH; x++)
		{
			if (g_GameMap[y+1][x+1] == 10) continue;
			BitBlt(hdc, GAME_LINE_WIDTH + x*PICTURE_SIZE, GAME_LINE_WIDTH + y*PICTURE_SIZE,
				PICTURE_SIZE, PICTURE_SIZE, g_hdc[g_GameMap[y+1][x+1]],
				0, 0, SRCCOPY);
		}
	}
	if (bRet)
	{
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		SelectObject(hdc, hPen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		if (g_GameMap[g_nowMouse.y+1][g_nowMouse.x+1]!=10)
			Rectangle(hdc, GAME_LINE_WIDTH + g_nowMouse.x*PICTURE_SIZE, GAME_LINE_WIDTH + g_nowMouse.y*PICTURE_SIZE,
				GAME_LINE_WIDTH + (g_nowMouse.x + 1)*PICTURE_SIZE, GAME_LINE_WIDTH + (g_nowMouse.y + 1)*PICTURE_SIZE
					);
		bRet = FALSE;
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PICTUREMATCHING));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PICTUREMATCHING);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU|WS_MINIMIZEBOX,
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
	int screanx, screany;
	int winx, winy,clientx,clienty;

	switch (message)
	{
	case WM_CREATE:
		RECT rect;
		screanx = GetSystemMetrics(SM_CXSCREEN);
		screany = GetSystemMetrics(SM_CYSCREEN);
		GetWindowRect(hWnd, &rect);
		winx = rect.right - rect.left;
		winy = rect.bottom - rect.top;
		GetClientRect(hWnd, &rect);
		clientx = rect.right - rect.left;
		clienty = rect.bottom - rect.top;
		gameRect.left = GAME_LINE_WIDTH;
		gameRect.top = GAME_LINE_WIDTH;
		gameRect.right = GAME_LINE_WIDTH + PICTURE_SIZE*GAME_WIDTH;
		gameRect.bottom = GAME_LINE_WIDTH + PICTURE_SIZE*GAME_HIGHT;
		timeRect.left = 0;
		timeRect.top = GAME_LINE_WIDTH *2+ PICTURE_SIZE*GAME_HIGHT;
		timeRect.right = GAME_LINE_WIDTH*2 + PICTURE_SIZE*GAME_WIDTH;
		timeRect.bottom = GAME_LINE_WIDTH *3+ PICTURE_SIZE*GAME_HIGHT;
		MoveWindow(hWnd, (screanx - winx + GAME_LINE_WIDTH*2) / 2, (screany - winy - GAME_LINE_WIDTH) / 2,
			GAME_LINE_WIDTH * 2 + PICTURE_SIZE*GAME_WIDTH + (winx - clientx),
			GAME_LINE_WIDTH * 3 + PICTURE_SIZE*GAME_HIGHT + (winy - clienty),
			TRUE
			);
		if (!Loadimage())
		{
			MessageBox(hWnd,L"W",L"T",MB_OK);
		}
		InitGame();
		SetTimer(hWnd, 1, 1000,0 );
		break;
	case WM_TIMER:
		timelength -= 20;
		if (timelength == 0)
		{
			InitGame();
			MessageBox(hWnd, L"Lose", L"T", MB_OK);
			InvalidateRect(hWnd, &gameRect, TRUE);
		}
		if (bWin)
		{
			InitGame();
			MessageBox(hWnd,L"Win",L"T",MB_OK);
			InvalidateRect(hWnd, &gameRect, TRUE);
		}
		InvalidateRect(hWnd, &timeRect, TRUE);
		break;
	case WM_LBUTTONDOWN:
		POINT temppoint;
		temppoint.x = LOWORD(lParam);
		temppoint.y = HIWORD(lParam);
		g_nowMouse.x = (temppoint.x - GAME_LINE_WIDTH) / PICTURE_SIZE;
		g_nowMouse.y = (temppoint.y - GAME_LINE_WIDTH) / PICTURE_SIZE;
		InvalidateRect(hWnd, &gameRect, TRUE);
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
		// TODO:  在此添加任意绘图代码...
		DrawGameWindow(hdc);
		DrawPlayArea(hdc);
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
