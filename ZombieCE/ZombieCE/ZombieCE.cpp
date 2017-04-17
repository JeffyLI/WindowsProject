// ZombieCE.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Windows.h>

DWORD WINAPI Cooling(LPARAM lparam)
{
	DWORD cooling = 0;
	DWORD coolingaddr = 0;
	DWORD dwTemp = 0;
	LPVOID phase = (LPVOID)0x00199178;
	DWORD dwRead = 0;
	DWORD failcount = 0;
	HANDLE handle = (HANDLE)lparam;
	while (TRUE)
	{
		if (!ReadProcessMemory(handle, phase, &dwTemp, 4, &dwRead))
		{
			printf_s("读取游戏内存失败，请先进入到游戏界面\r\n");
			Sleep(1000);
			failcount++;
			if (failcount > 5) return 0;
			continue;
		}

		dwTemp = dwTemp + 0x15C;
		if (!ReadProcessMemory(handle, (LPVOID)dwTemp, &dwTemp, 4, &dwRead))
		{
			printf_s("读取游戏内存失败，请先进入到游戏界面\r\n");
			Sleep(1000);
			continue;
		}
		coolingaddr = dwTemp + 0x28 + 0x24;
		if (!ReadProcessMemory(handle, (LPVOID)coolingaddr, &cooling, 4, &dwRead))
		{
			printf_s("读取游戏内存失败，请先进入到游戏界面\r\n");
			Sleep(1000);
			continue;
		}
		failcount = 0;
		for (int i = 0; i < 7; i++)
		{
			printf_s("%d", cooling);
			cooling = 5000;
			WriteProcessMemory(handle, (LPVOID)coolingaddr, &cooling, 4, &dwRead);
			coolingaddr += 0x50;
		}
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI YM(LPARAM lparam)
{
	DWORD ym = 0;
	DWORD ymaddr = 0;
	DWORD dwTemp = 0;
	LPVOID phase = (LPVOID)0x0019FA08;
	DWORD dwRead = 0;
	DWORD dwT;
	DWORD failcount = 0;
	HANDLE handle = (HANDLE)lparam;
	while (TRUE)
	{
		if (!ReadProcessMemory(handle, phase, &dwTemp, 4, &dwRead))
		{
			continue;
		}

		ymaddr = dwTemp + 0x54;
		if (!ReadProcessMemory(handle, (LPVOID)ymaddr, &ym, 4, &dwRead))
		{
			continue;
		}
		failcount = 0;
		if (ym > 2500&&ym<3000)
		{
			ym = 0;
			WriteProcessMemory(handle, (LPVOID)ymaddr, &ym, 4, &dwRead);
			Sleep(1000);
		}
	}
	return 0;
}

VOID Money(HANDLE handle)
{
	DWORD money = 0;
	DWORD moneyaddr = 0;
	DWORD dwTemp=0;
	LPVOID phase = (LPVOID)0x0019F9FC;
	DWORD dwRead = 0;
	
	if (!ReadProcessMemory(handle, phase, &dwTemp, 4, &dwRead))
	{
		printf_s("1读取游戏内存失败，请先进入到游戏界面\r\n");
		return;
	}
	printf_s("%d ", dwTemp);
	
	dwTemp = dwTemp + 0x950;
	if (!ReadProcessMemory(handle, (LPVOID)dwTemp, &dwTemp, 4, &dwRead))
	{
		printf_s("2读取游戏内存失败，请先进入到游戏界面\r\n");
		return;
	}

	moneyaddr = dwTemp + 0x50;
	if (!ReadProcessMemory(handle, (LPVOID)moneyaddr, &money, 4, &dwRead))
	{
		printf_s("3读取游戏内存失败，请先进入到游戏界面\r\n");
		return;
	}

	printf_s("%d", money);
	money += 10000;
	WriteProcessMemory(handle, (LPVOID)moneyaddr, &money, 4, &dwRead);
	Sleep(5000);
}

VOID Sunshine(HANDLE handle)
{
	DWORD sunshine = 0;
	DWORD sunshineaddr = 0;
	DWORD dwTemp = 0;
	LPVOID phase = (LPVOID)0x00199178;
	DWORD dwRead = 0;

	if (!ReadProcessMemory(handle, phase, &dwTemp, 4, &dwRead))
	{
		printf_s("读取游戏内存失败，请先进入到游戏界面\r\n");
		return;
	}

	sunshineaddr = dwTemp + 0x5578;
	if (!ReadProcessMemory(handle, (LPVOID)sunshineaddr, &sunshine, 4, &dwRead))
	{
		printf_s("读取游戏内存失败，请先进入到游戏界面\r\n");
		return;
	}

	if (sunshine < 9990)
	{
		sunshine = 9990;
		WriteProcessMemory(handle, (LPVOID)sunshineaddr, &sunshine, 4, &dwRead);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int op=0;
	HWND hwnd_game = FindWindowA(NULL,"Plants vs. Zombies 1.2.0.1073 RELEASE" );
	while (TRUE)
	{
		if (hwnd_game == NULL)
		{
			printf_s("没有找到要打开的游戏，请先运行游戏");
			break;
		}

		DWORD game_pid;
		GetWindowThreadProcessId(hwnd_game, &game_pid);
		HANDLE handle_game = OpenProcess(PROCESS_ALL_ACCESS, FALSE, game_pid);
		if (handle_game == NULL)
		{
			printf_s("游戏进程打开失败");
			break;
		}
		printf_s("请输入要执行的操作：\r\n1.阳光加满\r\n2.金钱增加10000\r\n3.无冷却\r\n4.疯狂玉米加农炮\r\n5.退出程序\r\n");
		scanf_s("%d", &op);
		if (op == 1)
		{
			Sunshine(handle_game);
		}
		else if (op == 2)
		{
			Money(handle_game);
		}
		else if (op==3)
		{
			HANDLE cooling_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Cooling, handle_game, 0, 0);
		}
		else if (op == 4)
		{
			printf_s("已经开启疯狂加农炮模式");
			HANDLE ym_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)YM, handle_game, 0, 0);
		}
		else
		{
			CloseHandle(handle_game);
			break;
		}
	}


	return 0;
}

