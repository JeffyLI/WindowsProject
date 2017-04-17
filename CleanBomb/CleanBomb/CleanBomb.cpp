// CleanBomb.cpp : ����Ӧ�ó������ڵ㡣
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

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLEANBOMB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLEANBOMB));

	// ����Ϣѭ��: 
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��: 
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

// �����ڡ������Ϣ�������
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
