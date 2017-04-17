// GESnake.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GESnake.h"
#include <vector>
#include<ctime>

#define MAX_LOADSTRING 100
#define SNAKE_SIZE 10
#define MAX_SNAKELEN 120
#define BOUND 10
#define GAME_WIDTH 50
#define GAME_HEIGHT 40
#define INFO_WIDTH 20
#define MY_TIMER 1
#define PAUSE_ID 1


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
	LoadString(hInstance, IDC_GESNAKE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GESNAKE));

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

std::vector<POINT> mSnake;
bool glose = false;
bool pause = false;
UCHAR msnake_len = 4;
UCHAR msnake_tail = 0;
UCHAR msnake_head = 4;
UINT my_time = 100;
POINT snake_direct = { 1, 0 };
POINT food;
int Score = 0;

void GaveFood()
{
	srand(unsigned(time(0)));
	bool foodflag=false;
	int x = rand() % 49 + 1;
	int y = rand() % 39 + 1;
	int n;
	for (int j = 0; j < msnake_len; j++)
	{
		n = (msnake_tail + j) % MAX_SNAKELEN;
		if (mSnake[n].x == x&&mSnake[n].y == y)
		{
			foodflag = true;
			break;
		}
	}
	if (foodflag){
		GaveFood();
	}
	else
	{
		food.x = x;
		food.y = y;
	}
}

POINT &GetSnakeNode(int index)
{
	int i = msnake_tail + index;
	i = i%MAX_SNAKELEN;
	return mSnake[i];
}

void MoveSnake()
{
	POINT pNewHead;
	pNewHead.x = GetSnakeNode(msnake_len - 1).x + snake_direct.x;
	pNewHead.y = GetSnakeNode(msnake_len - 1).y + snake_direct.y;

	if (pNewHead.x < 0 || pNewHead.y < 0 || pNewHead.x >= 50 || pNewHead.y >= 40)
	{
		glose = true;
		return;
	}
	int n;
	for (int j = 0; j < msnake_len; j++)
	{
		n = (msnake_tail + j) % MAX_SNAKELEN;
		if (mSnake[n].x == pNewHead.x&&mSnake[n].y == pNewHead.y)
		{
			glose = true;
			return;
		}
	}

	if (pNewHead.x == food.x&&pNewHead.y == food.y)
	{
		mSnake[msnake_head].x = pNewHead.x;
		mSnake[msnake_head].y = pNewHead.y;
		msnake_head++;
		if (msnake_head == MAX_SNAKELEN) msnake_head = 0;
		if(msnake_len<MAX_SNAKELEN-1) msnake_len++;
		Score += 100;
		if (my_time > 10) my_time--;
		GaveFood();
		return;
	}

	mSnake[msnake_head].x = pNewHead.x;
	mSnake[msnake_head].y = pNewHead.y;
	msnake_head++;
	msnake_tail++;
	if (msnake_head == MAX_SNAKELEN) msnake_head = 0;
	if (msnake_tail == MAX_SNAKELEN) msnake_tail = 0;
	
}


void DrawSnake(HDC hdc)
{
	POINT pNode;
	for (int i = 0; i < msnake_len; i++)
	{
		pNode = GetSnakeNode(i);
		Rectangle(hdc, BOUND + pNode.x*SNAKE_SIZE, BOUND + pNode.y*SNAKE_SIZE,
			BOUND + (pNode.x + 1)*SNAKE_SIZE, BOUND + (pNode.y + 1)*SNAKE_SIZE
			);
	}
}

void InitSnake()
{
	mSnake.clear();
	mSnake.resize(MAX_SNAKELEN);
	pause = false;
	Score = 0;
	msnake_len = 4;
	msnake_tail = 0;
	msnake_head = 4;
	my_time = 100;
	glose = false;
	for (int i = 0; i < msnake_len; i++)
	{
		mSnake[i].x = i;
		mSnake[i].y = 1;
	}
	GaveFood();
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GESNAKE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GESNAKE);
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
	HWND hPause;

	switch (message)
	{
	case WM_CREATE:
		MoveWindow(hWnd, 0, 0,
			3 * BOUND + (GAME_WIDTH + INFO_WIDTH)*SNAKE_SIZE+20,
			2 * BOUND + GAME_HEIGHT*SNAKE_SIZE+ 60,
			true);
		hPause = CreateWindow(TEXT("Button"), TEXT("Pause"), WS_CHILD|WS_VISIBLE, 
			2 * BOUND + SNAKE_SIZE*GAME_WIDTH+5, BOUND+5, 10 * SNAKE_SIZE, 5 * SNAKE_SIZE,
			hWnd, (HMENU)PAUSE_ID, hInst, NULL);
		InitSnake();
		SetTimer(hWnd, MY_TIMER, my_time, NULL);
		break;
	case WM_TIMER:
		MoveSnake();
		if (glose)
		{
			KillTimer(hWnd, MY_TIMER);
			MessageBox(NULL, TEXT("You Lose"), TEXT("File"), MB_OK);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			if (snake_direct.y == 0)
			{
				snake_direct.x = 0;
				snake_direct.y = -1;
			}
			break;
		case VK_DOWN:
			if (snake_direct.y == 0)
			{
				snake_direct.x = 0;
				snake_direct.y = 1;
			}
			break;
		case VK_LEFT:
			if (snake_direct.x == 0)
			{
				snake_direct.x = -1;
				snake_direct.y = 0;
			}
			break;
		case VK_RIGHT:
			if (snake_direct.x == 0)
			{
				snake_direct.x = 1;
				snake_direct.y = 0;
			}
			break;
		case VK_ESCAPE:
			if (pause){
				pause = false;
				//SetWindowText(hPause, TEXT("Pause"));
				SetTimer(hWnd, MY_TIMER, my_time, NULL);
			}
			else{
				pause = true;
				//SetWindowText(hPause, TEXT("Start"));
				KillTimer(hWnd, MY_TIMER);
			}
			break;
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
		HBRUSH hbrush;
		// TODO:  在此添加任意绘图代码...

		Rectangle(hdc, BOUND, BOUND, BOUND + GAME_WIDTH*SNAKE_SIZE, BOUND + GAME_HEIGHT*SNAKE_SIZE);
		Rectangle(hdc, 2 * BOUND + GAME_WIDTH*SNAKE_SIZE, BOUND, 2 * BOUND + (GAME_WIDTH + INFO_WIDTH)*SNAKE_SIZE, BOUND + GAME_HEIGHT*SNAKE_SIZE);

		hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(hdc, hbrush);
		Rectangle(hdc, BOUND + food.x*SNAKE_SIZE, BOUND + food.y*SNAKE_SIZE, BOUND + (food.x + 1)*SNAKE_SIZE, BOUND +(food.y + 1)*SNAKE_SIZE);

		hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(hdc, hbrush);
		DrawSnake(hdc);
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
