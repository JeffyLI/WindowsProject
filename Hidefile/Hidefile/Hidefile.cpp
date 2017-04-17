// Hidefile.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
char * ReadFileContent(char * filename, DWORD *filesize)
{
	HANDLE hfile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Fail opening file\n");
		return NULL;
	}
	DWORD dwRead;
	DWORD dwsize = GetFileSize(hfile, &dwRead);
	*filesize = dwsize;
	char *buf = new char[dwsize];
	RtlZeroMemory(buf, dwsize);
	ReadFile(hfile, buf, dwsize, &dwRead, 0);
	if (dwRead != dwsize)
	{
		printf("Fail reading file");
		return NULL;
	}
	CloseHandle(hfile);
	return buf;
}
bool SaveFile(char * filename, char * buf,int len)
{
	HANDLE hfile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Fail opening file\n");
		return false;
	}
	DWORD dwWrite;
	SetFilePointer(hfile, 0, 0, FILE_BEGIN);
	WriteFile(hfile, buf, len, &dwWrite, 0);
	CloseHandle(hfile);
	return true;
}

bool Hide(char * secretFileName,char * bmpFileName)
{
	DWORD dwBMPSize, dwSecretSize;
	char * pBMP = ReadFileContent(bmpFileName, &dwBMPSize);
	char * pSecreFile = ReadFileContent(secretFileName, &dwSecretSize);
	DWORD * lpFirstPointOffset = (DWORD *)(pBMP + 10);
	printf("第一个偏移量%d\n", *lpFirstPointOffset);
	char * pCurrentBMP = pBMP + *lpFirstPointOffset;
	char * pCurrentSecret = pSecreFile;
	*((DWORD *)pCurrentBMP) = dwSecretSize;
	pCurrentBMP += 6;
	for (; pCurrentBMP < (pBMP + dwBMPSize) && pCurrentSecret <= (pSecreFile + dwSecretSize); pCurrentBMP += 6)
	{
		*pCurrentBMP = *pCurrentSecret;
		*(pCurrentBMP+1) = *(pCurrentSecret+1);
		*(pCurrentBMP + 2) = *(pCurrentSecret + 2);
		pCurrentSecret += 3;
	}
	SaveFile(bmpFileName, pBMP, dwBMPSize);
	delete[] pBMP;
	delete[] pSecreFile;
	return true;
}
bool Recovery(char * bmpFileName,char * secretFileName)
{
	DWORD dwBMPSize;
	char *pBMP = ReadFileContent(bmpFileName, &dwBMPSize);
	DWORD *pFirstPoine = (DWORD *)(pBMP + 10);
	printf("%d\n",*pFirstPoine);
	DWORD dwSecretSize = *((DWORD *)(pBMP + *pFirstPoine));
	char *buf = new char[dwSecretSize];
	char *pCurrentBMP = pBMP + *pFirstPoine+6;
	int i;
	for (i = 0; pCurrentBMP < pBMP + dwBMPSize&&i<dwSecretSize; pCurrentBMP += 6)
	{
		buf[i] = *pCurrentBMP;
		buf[i+1] = *(pCurrentBMP+1);
		buf[i + 2] = *(pCurrentBMP + 2);
		i += 3;
	}
	SaveFile(secretFileName, buf, dwSecretSize);
	delete[] pBMP;
	delete[] buf;

	return true;
}
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc<3)
	{
		printf("输入格式错误\n");
		return -1;
	}
	if (strcmp(argv[1], "Save")==0)
	{
		Hide(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "Relase") == 0)
	{
		Recovery(argv[2], argv[3]);
	}
	else
		printf("Error");

	printf("DONE");
	return 0;
}

