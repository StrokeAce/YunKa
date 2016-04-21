#include "../stdafx.h"
#include "comfunc.h"
#include "comdef.h"
//#include <Psapi.h>

//#pragma   comment(lib,"psapi.lib")

//字符串到长整形的互换
DWORD StringToDWORD(string str)
{
	return (DWORD)atoi(str.c_str());
}

bool IsNumber(string lpStr)
{
	if (lpStr.empty())
	{
		return false;
	}

	char ch;

	for (int i = 0; i < lpStr.size(); i++)
	{
		ch = lpStr.at(i);
		if ((ch < '0') || (ch > '9'))
		{
			return false;
		}
	}
	return true;
}

//获得本机的地址
char * GetLocalHost(char * szAddress)
{
	if (gethostname(szAddress, 128) == SOCKET_ERROR)
	{
	int nError = GetLastError();
	return NULL;
	}
	else
		return szAddress;
}

string GetCurrentPath1()
{
	char szMyDllFileName[MAX_1024_LEN];
	string str;

	memset(szMyDllFileName, 0, 1024);
	::GetModuleFileNameA(NULL, szMyDllFileName, MAX_1024_LEN);
	str = szMyDllFileName;

	int pos = str.find_last_of('\\');

	if (pos > -1)
	{
		str = str.substr(0, pos + 1);
	}
	return str;
}

bool LoadIniString(char *sApp, char *sKey, char *sReturn, int len, char *sFile, char *sDefault)
{
	if (sApp == NULL || sKey == NULL || sFile == NULL)
		return false;

	char sDefault1[1024], strString[1024];
	bool brtn = false;

	if (sDefault == NULL)
	{
		strcpy(sDefault1, sReturn);
	}
	else
	{
		strcpy(sDefault1, sDefault);
	}
	strcpy(strString, sDefault1);

	if (GetPrivateProfileStringA(sApp, sKey, sDefault1, strString, len, sFile))
	{
		if (strlen(strString) >= 0)
		{
			strcpy(sReturn, strString);
			brtn = true;
		}
	}

	return brtn;

}

bool LoadIniInt(char *sApp, char *sKey, int &nReturn, char *sFile, char *sDefault)
{
	if (sApp == NULL || sKey == NULL || sFile == NULL)
		return false;

	bool brtn = false;
	char sDefault1[256], strString[256];
	if (sDefault == NULL)
	{
		sprintf(sDefault1, "%d", nReturn);
	}
	else
	{
		sprintf(sDefault1, sDefault);
	}
	strcpy(strString, sDefault1);

	if (GetPrivateProfileStringA(sApp, sKey, sDefault1, strString, 256, sFile))
	{
		if (strlen(strString) > 0)
		{
			nReturn = atol(strString);
			brtn = true;
		}
	}

	return brtn;
}

//获取某几位的值
unsigned long GetMutiByte(unsigned int value, int index, int bytenum)
{
	int byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	unsigned int rtn = value >> index;  //右移
	rtn &= byte;  //1111

	return rtn;
}

unsigned short GetMutiByte(unsigned short value, int index, int bytenum)
{
	int byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	unsigned long rtn = value >> index;  //右移
	rtn &= byte;  //1111

	return (unsigned short)rtn;
}

unsigned char GetMutiByte(unsigned char value, int index, int bytenum)
{
	int byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	unsigned long rtn = value >> index;  //右移
	rtn &= byte;  //1111

	return (unsigned char)rtn;
}

//设置多位的值
unsigned int SetMutiByte(unsigned int &source, int index, int bytenum, unsigned int value)
{
	// 先取出要设置的位，并移到指定的位置
	int byte;

	byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	value &= byte;
	value = value << index;   //左移

	//指定的位全为1 和全为0的情况
	int rtn0, rtn1;
	rtn1 = byte << index;   //左移
	rtn0 = ~rtn1;

	//首先将指定的位置为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source;
}


unsigned short SetMutiByte(unsigned short &source, int index, int bytenum, unsigned short value)
{
	// 先取出要设置的位，并移到指定的位置
	int byte;

	byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	value &= byte;
	value = value << index;   //左移

	//指定的位全为1 和全为0的情况
	int rtn0, rtn1;
	rtn1 = byte << index;   //左移
	rtn0 = ~rtn1;

	//首先将指定的位置为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source & 0xffff;
}

unsigned char SetMutiByte(unsigned char &source, int index, int bytenum, unsigned char value)
{
	// 先取出要设置的位，并移到指定的位置
	int byte;

	byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	value &= byte;
	value = value << index;   //左移

	//指定的位全为1 和全为0的情况
	int rtn0, rtn1;
	rtn1 = byte << index;   //左移
	rtn0 = ~rtn1;

	//首先将指定的位置为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source & 0xff;
}

unsigned short SendOnePack(SOCKET socket, char *sbuff, TCP_PACK_HEADER tcppackhead, int &nError)
{
	if (tcppackhead.len > PACKNORMALLEN)
	{
		nError = -1;
		return -1;
	}

	nError = 0;
	char data[PACKMAXLEN + sizeof(TCP_PACK_HEADER)] = { 0 };

	int ret, idx, tcpheadlen;
	int nLeft, nSend;

	tcpheadlen = sizeof(TCP_PACK_HEADER);
	*(TCP_PACK_HEADER *)data = tcppackhead;
	memcpy((data + tcpheadlen), sbuff, tcppackhead.len);

	nLeft = tcpheadlen + tcppackhead.len;
	idx = 0;
	nSend = 0;
	while (nLeft > 0)
	{
		ret = send(socket, (char *)(data + idx), nLeft, 0);
		if (ret == SOCKET_ERROR)
		{
			nError = GetLastError();
			if (nError == WSAEWOULDBLOCK)
			{
				Sleep(500);
				continue;
			}
			goto RETURN;
		}
		idx += ret;
		nSend += ret;
		nLeft -= ret;
	}

RETURN:
	return nSend;
}

unsigned short SendOnePack(SOCKET socket, char *data, int len, int &nError, unsigned short cmd,
	unsigned long senduid, unsigned long recvuid, unsigned long sendsock, unsigned long recvsock,
	unsigned short seq)
{
	TCP_PACK_HEADER tcppackhead;

	memset((char *)(&tcppackhead), 0, sizeof(tcppackhead));


	tcppackhead.len = len;


	return SendOnePack(socket, data, tcppackhead, nError);
}