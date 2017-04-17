// D_Tetris.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "D_Tetris.h"
#include<ctime>
#include<string.h>
#include<tchar.h>
#include<stdio.h>

#define MAX_LOADSTRING 100

#define TETRIS_SIZE 30
#define BOUND_SIZE 10
#define GAME_X 10
#define GAME_Y 20
#define INFO_X 6
#define MY_TIMER 1

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
	LoadString(hInstance, IDC_D_TETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D_TETRIS));

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

UINT timespeed=500;
int Socre = 0;
bool bPause = false;
bool bIsOver = false;
int level = 0;

bool myTetris[][4][4]
{
	{ { 1, 1, 1, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 1, 0 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

bool currentTetris[4][4];
bool GameTetris[GAME_Y][GAME_X];
int startx;
int starty;

void initGame()
{
	int i;
	bIsOver = false;
	srand(unsigned(time(0)));
	startx = BOUND_SIZE + 3 * TETRIS_SIZE;
	starty = BOUND_SIZE;
	i = rand() % 7;
	memcpy(currentTetris, myTetris[i], sizeof(currentTetris));
}

int CDECL MessageBoxPrintf(TCHAR *szCaption ,TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf_s(szBuffer, sizeof(szBuffer), szFormat, pArgList);
	va_end(pArgList);
	return MessageBox(NULL, szBuffer, szCaption, 0);
}

TCHAR *MyPrintf(TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf_s(szBuffer, sizeof(szBuffer), szFormat, pArgList);
	va_end(pArgList);
	return szBuffer;
}


void DrawMyCurrent(HDC hdc,int x,int y,bool dcurTet[4][4])
{
	HPEN hpen = (HPEN)GetStockObject(BLACK_PEN);
	HBRUSH hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	SelectObject(hdc, hpen);
	SelectObject(hdc, hbrush);
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dcurTet[i][j]){
				Rectangle(hdc, x + j*TETRIS_SIZE, y + i*TETRIS_SIZE, x + (j+1)*TETRIS_SIZE, y + (i+1)*TETRIS_SIZE);
			}
		}
	}
}

void CleanGame()
{
	bool tempGameT[GAME_Y][GAME_X] = { 0 };
	int ty = GAME_Y-1;
	int x, y;
	bool flag;
	for (y = GAME_Y - 1; y >= 0; y--)
	{
		flag = false;
		for (x = GAME_X - 1; x >= 0; x--)
		{
			if (!GameTetris[y][x]){
				flag = true;
				break;
			}
		}
		if (flag)
		{
			for (x = GAME_X - 1; x >= 0; x--)
			{
				tempGameT[ty][x] = GameTetris[y][x];
			}
			ty--;
		}
		else
		{
			Socre += 20;
			if(timespeed > 10) timespeed -= 10;
			level = Socre / 200;
		}
	}
	memcpy(GameTetris, tempGameT, sizeof(tempGameT));
}


bool checkOver()
{
	for (int i = 0; i < GAME_X; i++)
	{
		if (GameTetris[0][i]) return true;
	}
	return false;
}

bool checkMove(int x, int y, bool dcurTet[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (dcurTet[i][j])
			{
				if (GameTetris[(y - BOUND_SIZE) / TETRIS_SIZE+i][(x - BOUND_SIZE) / TETRIS_SIZE+j]) return FALSE;
				if (x + j*TETRIS_SIZE < BOUND_SIZE) return FALSE;
				if (x + (j + 1)*TETRIS_SIZE > BOUND_SIZE + TETRIS_SIZE*GAME_X) return FALSE;
				if (y + (i + 1)*TETRIS_SIZE > BOUND_SIZE + TETRIS_SIZE*GAME_Y) return FALSE;
			}
		}
	}

	return TRUE;
}

void RotateTetris(int x, int y, bool dcurTer[4][4])
{
	bool tempTetris[4][4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tempTetris[i][j] = dcurTer[3 - j][i];
		}
	}

	if (checkMove(x, y, tempTetris))
	{
		memcpy(dcurTer, tempTetris, sizeof(tempTetris));
	}
}


//靠左和上的旋转，但只能转两个方向
void RotateTetris2(int x, int y, bool dcurTer[4][4])
{
	bool tempTetris[4][4] = { 0 };
	bool cpyTetris[4][4] = { 0 };
	int i, j, n, m;
	bool tflag;
	memcpy(cpyTetris, dcurTer, sizeof(cpyTetris));
	for (i = 0,n=0; i < 4; i++)
	{
		tflag = false;
		for (j = 0; j < 4; j++)
		{
			tempTetris[n][j] = cpyTetris[j][i];
			if (tempTetris[n][j])
			{
				tflag = true;
			}
		}
		if (tflag)
		{
			n++;
		}
	}
	memset(cpyTetris, 0, sizeof(tempTetris));
	for (j = 0, m = 0; j < 4; j++)
	{
		tflag = false;
		for (i = 0; i < 4; i++)
		{
			cpyTetris[i][m] = tempTetris[i][j];
			if (cpyTetris[i][m])
			{
				tflag = true;
			}
		}
		if (tflag)
		{
			m++;
		}
	}

	if (checkMove(x, y, cpyTetris))
	{
		memcpy(dcurTer, cpyTetris, sizeof(cpyTetris));
	}
}

void DrawBackGround(HDC hdc)
{
	HPEN hpen = (HPEN)GetStockObject(BLACK_PEN);
	HBRUSH hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	SelectObject(hdc, hpen);
	SelectObject(hdc, hbrush);

	Rectangle(hdc, BOUND_SIZE, BOUND_SIZE, BOUND_SIZE + TETRIS_SIZE*GAME_X, BOUND_SIZE + TETRIS_SIZE*GAME_Y);

	hpen = (HPEN)GetStockObject(NULL_PEN);
	hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	SelectObject(hdc, hpen);
	SelectObject(hdc, hbrush);
	for (int i = 0; i < GAME_Y; i++)
	{
		for (int j = 0; j < GAME_X; j++)
		{
			if (GameTetris[i][j])
				Rectangle(hdc, BOUND_SIZE + j*TETRIS_SIZE, BOUND_SIZE + i*TETRIS_SIZE, BOUND_SIZE + (j + 1)*TETRIS_SIZE, BOUND_SIZE + (i + 1)*TETRIS_SIZE);
		}
	}
}

void DrawInfoGround(HDC hdc)
{
	HPEN hpen = (HPEN)GetStockObject(BLACK_PEN);
	HBRUSH hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	SelectObject(hdc, hpen);
	SelectObject(hdc, hbrush);
	Rectangle(hdc, 2 * BOUND_SIZE + TETRIS_SIZE*GAME_X, BOUND_SIZE,
		2*BOUND_SIZE + TETRIS_SIZE*(GAME_X+INFO_X), BOUND_SIZE + TETRIS_SIZE*GAME_Y);
	RECT rect;
	rect.left = 2 * BOUND_SIZE + TETRIS_SIZE*GAME_X;
	rect.top = BOUND_SIZE;
	rect.right = 2 * BOUND_SIZE + TETRIS_SIZE*(GAME_X + INFO_X);
	rect.bottom = BOUND_SIZE + TETRIS_SIZE*GAME_Y;
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT); //透明背景 
	DrawText(hdc, MyPrintf(TEXT("Game Over\r\n得分：%i\r\n级别：%i"), Socre,level), -1, &rect, DT_LEFT);
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D_TETRIS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_D_TETRIS);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_MINIMIZEBOX|WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   initGame();
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

	switch (message)
	{
	case WM_CREATE:
		MoveWindow(hWnd, 0, 0,
			3 * BOUND_SIZE + TETRIS_SIZE*(GAME_X + INFO_X)+20,
			2 * BOUND_SIZE + TETRIS_SIZE*GAME_Y+60,
			true);
		SetTimer(hWnd, MY_TIMER, timespeed, NULL);
		break;
	case WM_TIMER:
		if (checkMove(startx, starty + TETRIS_SIZE, currentTetris))
		{
			starty+=TETRIS_SIZE;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (currentTetris[i][j])
					{
						GameTetris[(starty - BOUND_SIZE) / TETRIS_SIZE + i][(startx - BOUND_SIZE) / TETRIS_SIZE + j] = 1;
						if (checkOver())
						{
							KillTimer(hWnd, MY_TIMER);
							MessageBoxPrintf(_T("Over"), TEXT("Game Over\r\n得分：%i"),Socre , MB_OK);
							bIsOver = true;
							i = 4;
						}
					}
				}
			}
			if (!bIsOver)
			{
				CleanGame();
				initGame();
				SetTimer(hWnd, MY_TIMER, timespeed, NULL);
			}
		}
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (checkMove(startx - TETRIS_SIZE, starty, currentTetris))
			{
				startx -= TETRIS_SIZE;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_RIGHT:
			if (checkMove(startx + TETRIS_SIZE, starty, currentTetris))
			{
				startx += TETRIS_SIZE;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_UP:
			RotateTetris(startx,starty,currentTetris);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN:
			SetTimer(hWnd, MY_TIMER, 30, NULL);
			break;
		case VK_F12:
			if (bPause){
				SetTimer(hWnd, MY_TIMER, timespeed, NULL);
				bPause = false;
			}
			else{
				KillTimer(hWnd, MY_TIMER);
				bPause = true;
			}

		default:
			break;
		}
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
		DrawBackGround(hdc);
		DrawInfoGround(hdc);
		DrawMyCurrent(hdc, startx, starty,currentTetris);
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
