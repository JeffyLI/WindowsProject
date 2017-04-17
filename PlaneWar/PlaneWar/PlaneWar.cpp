// PlaneWar.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PlaneWar.h"
#include<list>

#pragma comment(lib, "Msimg32.lib")
#define MAX_LOADSTRING 100
#define SPEED_BG 6


HDC g_hdc;
HDC g_hdcBg;
HDC g_hdcRobot;
HDC g_hdcBuf;
HDC g_hdcBullet;
HDC g_hdcenemy;
HDC g_hdcdead;
BOOL btouch=FALSE;

typedef struct Elum
{
	HDC hdc;
	int x;
	int y;
	int vx;
	int vy;
	int life;
}ELUM_S;
ELUM_S g_strRobot;
UINT iCd;
UINT g_iOffsqt;
std::list<ELUM_S> g_listBullet;
std::list<ELUM_S> g_listEnemy;

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

VOID LoadGame(HWND hWnd);
VOID Paint_Game();
VOID InitGame();
VOID Clear_Game();
VOID Move();
VOID NEW_SHOOT();
VOID NEW_Enemy();
VOID Adjust();

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
	LoadString(hInstance, IDC_PLANEWAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PLANEWAR));

	DWORD tCur = 0, tPre = 0;
	// 主消息循环: 
	while (TRUE)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			tCur = GetTickCount();
			if (tCur - tPre > 50)
			{
				if (g_strRobot.life == 0) break;
				Paint_Game();
				tPre = tCur;
			}
		}
	}
	Clear_Game();
	return (int) msg.wParam;
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PLANEWAR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PLANEWAR);
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
   LoadGame(hWnd);
   InitGame();
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

	switch (message)
	{
	case WM_CREATE:
		MoveWindow(hWnd, 500, 50, 800, 650, TRUE);
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_RIGHT:
			g_strRobot.vx = 20;
			break;
		case VK_LEFT:
			g_strRobot.vx = -20;
			break;
		case VK_DOWN:
			g_strRobot.vy = 20;
			break;
		case VK_UP:
			g_strRobot.vy = -20;
			break;
		case VK_SPACE:
			if (!iCd)
			{
				iCd = 5;
				NEW_SHOOT();
			}
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
			g_strRobot.vx = 0;
			break;
		case VK_UP:
		case VK_DOWN:
			g_strRobot.vy = 0;
			break;
		}
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

VOID InitGame()
{
	g_strRobot.hdc = g_hdcRobot;
	g_strRobot.x = 0;
	g_strRobot.y = 300;
	g_strRobot.vx = 0;
	g_strRobot.vy = 0;
	g_strRobot.life = 1;
}

VOID Clear_Game()
{
	DeleteDC(g_hdcBuf);
	DeleteDC(g_hdc);
	g_listBullet.clear();
	g_listEnemy.clear();
}

VOID LoadGame(HWND hWnd)
{
	HBITMAP hbitmap;

	g_hdc = GetDC(hWnd);

	g_hdcBuf = CreateCompatibleDC(g_hdc);
	HBITMAP g_hBmp = CreateCompatibleBitmap(g_hdc, 800, 600);
	SelectObject(g_hdcBuf, g_hBmp);
	DeleteObject(g_hBmp);

	g_hdcBg = CreateCompatibleDC(g_hdc);
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BGICON));
	SelectObject(g_hdcBg, hbitmap);
	DeleteObject(hbitmap);

	g_hdcRobot = CreateCompatibleDC(g_hdc);
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROBOT));
	SelectObject(g_hdcRobot, hbitmap);
	DeleteObject(hbitmap);

	g_hdcBullet = CreateCompatibleDC(g_hdc);
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Bullet));
	SelectObject(g_hdcBullet, hbitmap);
	DeleteObject(hbitmap);

	g_hdcenemy = CreateCompatibleDC(g_hdc);
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENEMY));
	SelectObject(g_hdcenemy, hbitmap);
	DeleteObject(hbitmap);

	g_hdcdead = CreateCompatibleDC(g_hdc);
	hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEAD));
	SelectObject(g_hdcdead, hbitmap);
	DeleteObject(hbitmap);

	return;
}

VOID Paint_Game()
{
	std::list<ELUM_S>::iterator it;
	BitBlt(g_hdcBuf, 0, 0, 800, 600, g_hdcBg, g_iOffsqt, 0, SRCCOPY);
	BitBlt(g_hdcBuf, 800-g_iOffsqt, 0, 800, 600, g_hdcBg, 0, 0, SRCCOPY);
	g_iOffsqt += SPEED_BG;
	if (g_iOffsqt > 800) g_iOffsqt = 0;
	TransparentBlt(g_hdcBuf, g_strRobot.x, g_strRobot.y, 80, 80, g_strRobot.hdc, 0, 0, 80, 80, RGB(255, 255, 255));
	for (it = g_listBullet.begin(); it != g_listBullet.end(); it++)
	{
		TransparentBlt(g_hdcBuf, it->x, it->y, 40, 20, it->hdc,
			0, 0, 40, 20, RGB(0, 0, 0));
	}

	for (it = g_listEnemy.begin(); it != g_listEnemy.end(); it++)
	{
		if (it->life == 0)
		{
			TransparentBlt(g_hdcBuf, it->x, it->y, 80, 80, it->hdc,
				0, 0, 80, 80, RGB(0,0, 0));
		}
		else
		{
			TransparentBlt(g_hdcBuf, it->x, it->y, 80, 80, it->hdc,
				0, 0, 80, 80, RGB(255, 255, 255));
		}
	}

	Move();
	Adjust();

	BitBlt(g_hdc, 0, 0, 800, 600, g_hdcBuf, 0, 0, SRCCOPY);
}

VOID Move()
{
	std::list<ELUM_S>::iterator it;
	std::list<ELUM_S>::iterator itemp;

	srand(GetTickCount());
	if (!(rand()%10))
	{
		NEW_Enemy();
	}

	g_strRobot.x += g_strRobot.vx;
	g_strRobot.y += g_strRobot.vy;
	if (g_strRobot.x < 0) g_strRobot.x = 0;
	else if (g_strRobot.x > 800 - 80) g_strRobot.x = 800 - 80;
	if (g_strRobot.y < 0) g_strRobot.y = 0;
	else if (g_strRobot.y > 600 - 80) g_strRobot.y = 600 - 80;
	if(iCd) iCd--;

	for (itemp = g_listBullet.begin(); itemp != g_listBullet.end();)
	{
		it = itemp;
		itemp++;
		it->x += it->vx;
		it->y += it->vy;
		if (it->x > 800||it->life==0)
		{
			g_listBullet.erase(it);
		}
	}

	for (itemp = g_listEnemy.begin(); itemp != g_listEnemy.end();)
	{
		it = itemp;
		itemp++;
		it->x += it->vx;
		it->y += it->vy;
		
		if (it->x < 0||it->life==0)
		{
			g_listEnemy.erase(it);
		}
	}

}

VOID Adjust()
{
	std::list<ELUM_S>::iterator itButtle;
	std::list<ELUM_S>::iterator itEnemy;
	int xDiff, yDiff;
	for (itEnemy = g_listEnemy.begin(); itEnemy != g_listEnemy.end(); itEnemy++)
	{
		xDiff = itEnemy->x - g_strRobot.x;
		yDiff = itEnemy->y - g_strRobot.y;
		if (xDiff>-70 && xDiff<70 && yDiff>-70 && yDiff < 70)
		{
			g_strRobot.life--;
			if (g_strRobot.life == 0)
			{
				g_strRobot.hdc = g_hdcdead;
				MessageBox(0, L"Lose", L"T", MB_OK);
			}
		}
		for (itButtle = g_listBullet.begin(); itButtle != g_listBullet.end(); itButtle++)
		{
			xDiff = itEnemy->x - itButtle->x;
			yDiff = itEnemy->y - itButtle->y;
			if (xDiff>-80 && xDiff<40 && yDiff>-80 && yDiff < 20)
			{
				itEnemy->hdc = g_hdcdead;
				itEnemy->life = 0;
				itButtle->life = 0;
			}
		}
	}

}

VOID NEW_Enemy()
{
	ELUM_S stEnemy;

	srand(GetTickCount());
	stEnemy.hdc = g_hdcenemy;
	stEnemy.x = 800;
	stEnemy.y = rand()%600;
	stEnemy.vx = -12;

	if (stEnemy.y>g_strRobot.y) stEnemy.vy = -(rand()%5+1);
	else stEnemy.vy = rand()%5+1;

	stEnemy.life = 1;

	g_listEnemy.push_back(stEnemy);
}

VOID NEW_SHOOT()
{
	ELUM_S stBullet;

	stBullet.hdc = g_hdcBullet;
	stBullet.x = g_strRobot.x+60;
	stBullet.y = g_strRobot.y;
	stBullet.vx = 25;
	stBullet.vy = 0;
	stBullet.life = 1;

	g_listBullet.push_back(stBullet);
}