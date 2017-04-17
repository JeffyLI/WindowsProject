// Sudoku.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Sudoku.h"

#define MAX_LOADSTRING 100
#define SIDE_WIDTH 10
#define SQUARE_SIZE 50

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

int a[81];
int b[81];
int k;
int speed;
HFONT hf;
HFONT hg;
BOOL Timer;
int map[81] = 
{ 8, 0, 0, 0, 0, 0, 0, 0, 0
, 0, 0, 3, 6, 0, 0, 0, 0, 0
, 0, 7, 0, 0, 9, 0, 2, 0, 0
, 0, 5, 0, 0, 0, 7, 0, 0, 0
, 0, 0, 0, 0, 4, 5, 7, 0, 0
, 0, 0, 0, 1, 0, 0, 0, 3, 0
, 0, 0, 1, 0, 0, 0, 0, 6, 8
, 0, 0, 8, 5, 0, 0, 0, 1, 0
, 0, 9, 0, 0, 0, 0, 4, 0, 0 };

/*
1,6,8,7,4,3,9,2,5,
3,4,9,2,5,8,7,6,1,
7,5,2,6,1,9,4,3,8,
0,0,1,0,7,6,5,8,4,
6,9,0,0,8,4,3,0,2,
0,8,0,0,3,2,6,0,0,
8,2,6,4,9,5,1,7,3,
9,1,5,3,2,7,8,4,6,
4,7,3,8,6,1,2,5,9
*/

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
	LoadString(hInstance, IDC_SUDOKU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUDOKU));

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

BOOL Judge(int k)
{
	int t = a[k];
	if (t>9) return FALSE;
	int m[9] = { 0, 1, 2, 9, 10, 11, 18, 19, 20 };
	int pos = ((k % 9) / 3) * 3 + (k / 27) * 27;
	for (int i = 1; i<9; i++)
	{
		if (a[(k + i) % 9 + k / 9 * 9] == t || a[(k + i * 9) % 81] == t) return FALSE;
		if (a[pos + m[i - 1]] == t && ((pos + m[i - 1]) != k))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CaculateTheAnswer(HWND hWnd)
{
	BOOL jdg;
	if (k >= 0)
	{
		if (a[k] <= 9)
		{
			if (!b[k]) a[k]++;
			else
			{
				if (k == 80)
				{
					return TRUE;
				}
				k++;
				return FALSE;
			}
			jdg = Judge(k);
			if (jdg&&k == 80)
			{
				return TRUE;
			}
			else if (jdg)
			{
				k++;
				return FALSE;
			}
			return FALSE;
		}
		else
		{
			a[k] = 0;
			while (b[k - 1]) k--;
			k--;
			return FALSE;
		}
	}
	else
	{
		return TRUE;
	}
}

VOID DirectAnswer()
{
	k = 0;
	bool flag = false,jdg=false;
	while (k >= 0)
	{
		while (a[k] <= 9)
		{
			if (!b[k]) a[k]++;
			else
			{
				if (k == 80)
				{
					flag = true;
					break;
				}
				k++;
				continue;
			}
			jdg = Judge(k);
			if (jdg&&k == 80)
			{
				flag = true;
				break;
			}
			else if (jdg)
			{
				k++;
			}
		}
		if (flag) break;
		a[k] = 0;
		while (b[k - 1]) k--;
		k--;
	}
}

VOID InitGame()
{
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	for (int i = 0; i < 81; i++)
	{
		if (map[i] != 0)
		{
			a[i] = map[i];
			b[i] = 1;
		}
	}
	k = 0;
	Timer = FALSE;
	speed = 100;
	hf = CreateFont(40, 25, 0, 0, FW_THIN, true, false, false,
		CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
		FF_MODERN, TEXT("宋体"));
	hg = CreateFont(20, 10, 0, 0, FW_THIN, true, false, false,
		CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
		FF_MODERN, TEXT("宋体"));
}

VOID DrawDigital(HDC hdc)
{
	int x = 0, y = 0;
	TCHAR szBuf[4];
	RECT rect;
	for (int i = 0; i < 81; i++)
	{
		if ((a[i]%10) != 0)
		{
			x = i % 9;
			y = i / 9;
			rect.left = x*SQUARE_SIZE + (x / 3) * 5 + SIDE_WIDTH;
			rect.top = y*SQUARE_SIZE + (y / 3) * 5 + SIDE_WIDTH;
			rect.right = rect.left + SQUARE_SIZE;
			rect.bottom = rect.top + SQUARE_SIZE;
			if(b[i]) SetTextColor(hdc, RGB(0, 0, 0));
			else SetTextColor(hdc, RGB(255, 0, 0));
			SelectObject(hdc, hf);
			SetBkMode(hdc, TRANSPARENT);
			wsprintf(szBuf, TEXT("%d"), a[i]);
			DrawText(hdc, szBuf, wcslen(szBuf), &rect, DT_CENTER);
		}
	}
}

VOID DrawBackground(HDC hdc)
{
	HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
	int w = 0;
	SelectObject(hdc, hPen);
	for (int i = 0; i < 9; i++)
	{
		if (i % 3 == 0) w += 5;
		for (int j = 0; j < 9; j++)
		{
			Rectangle(hdc, j*SQUARE_SIZE + SIDE_WIDTH+(j/3)*5, i*SQUARE_SIZE + w,
				(j + 1)*SQUARE_SIZE + SIDE_WIDTH + (j / 3)*5, (i + 1)*SQUARE_SIZE + w);
		}
	}
}

VOID DrawGame(HDC hdc)
{
	DrawBackground(hdc);
	DrawDigital(hdc);
	TCHAR szBuf[1024];
	wsprintf(szBuf, TEXT("ESC初始化，END键出结果，空格键演示。\r\n当前的演示每步延时：%d（按↑键增加，↓键减少）"), speed);
	RECT rect;
	rect.top = 4 * SIDE_WIDTH + 9 * SQUARE_SIZE;
	rect.bottom = rect.top + 100;
	rect.left = 0;
	rect.right = 5 * SIDE_WIDTH + 9 * SQUARE_SIZE;
	SetTextColor(hdc, RGB(0, 0, 255));
	SelectObject(hdc, hg);
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, szBuf, wcslen(szBuf), &rect, DT_CENTER);
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUDOKU));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SUDOKU);
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
	int x, y;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		x = 5 * SIDE_WIDTH + 9 * SQUARE_SIZE;
		y = 6 * SIDE_WIDTH + 9 * SQUARE_SIZE+100;
		SetWindowPos(hWnd, NULL, 400, 100, x, y, NULL);
		InitGame();
		break;
	case WM_TIMER:
		if (CaculateTheAnswer(hWnd))
		{
			KillTimer(hWnd, 1);
		}
		rect.left = SIDE_WIDTH;
		if(k>9) rect.top = ((k / 9)-1)*SQUARE_SIZE;
		else rect.top = SIDE_WIDTH;
		rect.right = 5 * SIDE_WIDTH + 9 * SQUARE_SIZE;
		rect.bottom = rect.top + 3*SQUARE_SIZE+2*SIDE_WIDTH;
		InvalidateRect(hWnd, &rect, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			if (!Timer)
			{
				Timer = TRUE;
				SetTimer(hWnd, 1, speed, NULL);
			}
			break;
		case VK_UP:
			speed += 10;
			if (Timer&&k<80)
			{
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, speed, NULL);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN:
			if(speed>10) speed -= 10;
			if (Timer&&k<80)
			{
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, speed, NULL);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_END:
			if (Timer) KillTimer(hWnd, 1);
			Timer = FALSE;
			InitGame();
			DirectAnswer();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_ESCAPE:
			InitGame();
			InvalidateRect(hWnd, NULL, TRUE);
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
		DrawGame(hdc);
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
