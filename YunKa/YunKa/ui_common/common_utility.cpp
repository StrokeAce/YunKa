#include "../stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "common_utility.h"
#include <Iphlpapi.h>
#include <TlHelp32.h>
#include <Windows.h>
#include <Psapi.h>
#include <ShellAPI.h>

using namespace std;

#define BUFF_SIZE 1024 


wchar_t *CA2U(const char* str, WCHAR *dest)
{
	int textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)dest, textlen);
	return dest;
}

char *CU2A(const wchar_t* str, char *dest)
{
	int textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, str, -1, dest, textlen, NULL, NULL);
	return dest;
}


wchar_t *ANSIToUnicode(const char* str, wchar_t *dest)
{
	int textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)dest, textlen);
	return dest;
}

char *UnicodeToANSI(const wchar_t* str,char *dest)
{
	int textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, str, -1, dest, textlen, NULL, NULL);
	return dest;
}
wchar_t *UTF8ToUnicode(const char* str, wchar_t *dest)
{
	int textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)dest, textlen);
	return dest;
}


char *UnicodeToUTF8(const wchar_t* str, char *dest)
{
	int textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, dest, textlen, NULL, NULL);
	return dest;
}


char *ANSIToUTF8(const char* str, char *dest)
{
	wchar_t buf[1024 * 100];
	memset(buf,0,sizeof(buf));
	ANSIToUnicode(str, buf);
	return UnicodeToUTF8(buf, dest);
}


char *UTF8ToANSI(const char* str,char *destStr)
{
	wchar_t buf[1024 * 100];
	memset(buf, 0, sizeof(buf));
	UTF8ToUnicode(str, buf);
	return UnicodeToANSI(buf,destStr);
}


int PrintLog(const char *fmt, ...)
{
	va_list args;
	int n;
	/*char sprint_buf[1024 * 10] = {0};
	char write_buf[2048 * 10] = {0};*/

	char *sprint_buf = new char[2048 * 100];
	char *write_buf = new char[2048 * 100];

	FILE *fp;
	char timeBuf[100] = {0};

	va_start(args, fmt);//初始化参数指针 
	n = vsprintf(sprint_buf, fmt, args);/*函数放回已经处理的字符串长度*/
	va_end(args);//与va_start 配对出现，处理ap指针

	//if (console_ops.write) 
	//	console_ops.write(sprint_buf, n);/*调用控制台的结构中的write函数，将sprintf_buf中的内容输出n个字节到设备*/ 

	//信息输出窗口
	strcpy(write_buf, sprint_buf);
	//strcat(write_buf, "\n");
	OutputDebugStringA(write_buf);

	//是否显示日志时间
#if 1
	memset(write_buf, 0, sizeof(write_buf));
	//日志文件
	SYSTEMTIME t = { 0 };
	GetLocalTime(&t);
	sprintf_s(timeBuf, "%d-%d-%d_%d:%d:%d",
		t.wYear,
		t.wMonth,
		t.wDay,
		t.wHour,
		t.wMinute,
		t.wSecond);

	int length = 24 - strlen(timeBuf);
	strcpy(write_buf, timeBuf);
	for (int i = 0; i <length; i++)
	{
		strcat(write_buf, " ");
	}
	strcat(write_buf, sprint_buf);
#endif

	string path = GetCurrentPath() + "\\debug.log";
	if ((fp = fopen(path.c_str(), "a+")) == NULL)
	{
		printf("打开文件失败\n");

		delete sprint_buf;
		delete write_buf;
		return n;
	}

	fprintf(fp, "%s\n", write_buf);
	fclose(fp);


	printf("%s\n", write_buf);

	delete []sprint_buf;
	delete []write_buf;
	return n;
}




string GetCurrentPath()
{
	WCHAR wPath[MAX_PATH] = {};
	if (0 == GetModuleFileName(NULL, wPath, MAX_PATH))
	{
		return {0};
	}
	int nLen = WideCharToMultiByte(CP_ACP, 0, wPath, -1, NULL, 0, NULL, NULL);
	char* cPath = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, wPath, -1, cPath, nLen, NULL, NULL);
	string strTmp = string(cPath);
	int pos = strTmp.find_last_of('\\');
	string currentPath(strTmp.substr(0, pos));
	delete []cPath;
	return currentPath;
}

WCHAR *GetCurrentPathW()
{
	WCHAR wPath[MAX_PATH] = {};
	string path = GetCurrentPath();

	ANSIToUnicode(path.c_str(), wPath);

	return wPath;
}









void SplitStringA(char *pSrc, char *chMark, std::vector<std::string> &vecStrings)
{
	vecStrings.clear();

	if (!pSrc)
		return;
	char *p = NULL;
	p = strtok(pSrc, chMark);

	if (p != NULL)
	{
		vecStrings.push_back(p);
		while(1)
		{
			p = strtok(NULL, chMark);
			if (p != NULL)
			   vecStrings.push_back(p);
			else
				break;	
		}  
	}	
	else
	{
		vecStrings.push_back(pSrc);
	}

}

void SplitStringA(char *pSrc, char chMark, std::vector<std::string> &vecStrings, BOOL bOnce)
{
	vecStrings.clear();

	if (!pSrc)
		return;

	char *pFront = pSrc;
	char *pBack = pSrc;
	char *pEnd = pSrc + (strlen(pSrc) - 1);

	if (FALSE == bOnce)
	{
		for (; pBack <= pEnd; ++pBack)
		{
			if (*pBack == chMark)
			{
				char chTmp = *pBack;
				*pBack = '\0';

				vecStrings.push_back(pFront);
				*pBack = chTmp;

				pFront = pBack + 1;
			}
			else if (pBack == pEnd)
			{
				vecStrings.push_back(pFront);
				break;
			}
		}
	}
	else
	{
		for (; pBack <= pEnd; ++pBack)
		{
			if (*pBack == chMark)
			{
				*pBack = '\0';

				vecStrings.push_back(pFront);
				pFront = pBack + 1;
				vecStrings.push_back(pFront);

				break;
			}
		}
	}
}


void ConvertUtf8ToGBKString(std::string& strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL, NULL);
	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}

void ConvertGBKToUtf8String(std::string& strGBK) {
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.c_str(), -1, NULL, 0);
	unsigned short * wszUtf8 = new unsigned short[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len);
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, (LPSTR)szUtf8, len, NULL, NULL);
	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
}


//字符串替换 当前字符  将srcStr 替 换为 destStr
void StringReplace(string &curStr,string srcStr,string destStr)
{
	string::size_type pos = 0;
	string::size_type src = srcStr.size();
	string::size_type dest = destStr.size();
	while ((pos = curStr.find(srcStr, pos)) != string::npos)
	{
		curStr.replace(pos, src, destStr);
		pos += dest;
	}
}

