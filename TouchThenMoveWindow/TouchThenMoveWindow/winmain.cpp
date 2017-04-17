#include<windows.h>.


void test1()
{
	HWND hWnd = FindWindowW(L"TXGuiFoundation", L"QQ");;
	POINT pt = { 0 };
	RECT rect = { 0 };
	while (hWnd)
	{
		hWnd = FindWindowW(L"TXGuiFoundation", L"QQ");
		GetCursorPos(&pt);
		GetWindowRect(hWnd, &rect);
		if (pt.x>rect.left&&pt.x<rect.right&&pt.y>rect.top&&pt.y<rect.bottom)
			MoveWindow(hWnd, pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		Sleep(100);
	}
}

BOOL CALLBACK EnumQQWindow(HWND hWnd, LPARAM lParam)
{
	wchar_t szName[MAXBYTE] = { 0 };
	GetClassName(hWnd, szName, MAXBYTE);
	if (0 == wcscmp(szName, L"TXGuiFoundation"))
	{
		POINT pt = { 0 };
		RECT rect = { 0 };
		if (GetCursorPos(&pt))
		{
			if (GetWindowRect(hWnd, &rect))
			   if (pt.x>rect.left&&pt.x<rect.right&&pt.y>rect.top&&pt.y<rect.bottom)
				  MoveWindow(hWnd, pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
	}

	return true;
}

void test2()
{
	while (true)
	{
		EnumWindows(EnumQQWindow, 0);
	}
}

void test3()
{
	while (TRUE)
	{
		HWND hWnd = GetForegroundWindow();
		POINT pt = { 0 };
		RECT rect = { 0 };
		if (GetCursorPos(&pt))
		{
			if (GetWindowRect(hWnd, &rect))
				if (pt.x>rect.left&&pt.x<rect.right&&pt.y>rect.top&&pt.y<rect.bottom)
					MoveWindow(hWnd, pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
	}
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//test1(); ������ȱ�ݣ����ĵ��ڴ�̫����
	test2();  //�ص�������ǿ��
	//test3();  //�޶����ͣ������ĸ����ھͶ��ĸ�����
	return 0;
}