#include "../stdafx.h"
#include "encrypt.h"
#include "tstring.h"
static unsigned char TS[256]="";
static unsigned char TD[256]="";
static bool binitlock=false;
unsigned char& replace(const unsigned char& ch, const bool& b)
{

	if (b)
	{
		return TS[ch];
	}else
	{
		return TD[ch];
	}
}
static void initlock()
{
	if (!binitlock)
	{
		int i;
		for (i=0;i<256;++i)
		{
			TS[i]=i;
			TD[i]=i;
		}
		for (i=0;i<255;++i)
		{
			TS[T[i].source]=T[i].destination;
			TD[T[i].destination]=T[i].source;
		}
		binitlock=true;
	}
}
string lock(string str)
{

	/*initlock();

	int lLen = str.GetLength();
	char * pdata=(char*)str.GetBuffer(lLen);
	bool b = true;
	lLen*=sizeof(TCHAR);
	for(int i=0;i<lLen;i++)
	{
	pdata[i]=replace(pdata[i],b);
	}

	str.ReleaseBuffer();*/
	return str;
}

string unlock(string str)
{

	/*initlock();
	int lLen = str.GetLength();
	char * pdata=(char*)str.GetBuffer(lLen);
	bool b = false;
	lLen*=sizeof(TCHAR);
	for(int i=0;i<lLen;i++)
	{
	pdata[i]=replace(pdata[i],b);
	}

	str.ReleaseBuffer();*/
	return str;
}

char *lock(char *pChar, int len)
{
	initlock();
	for(int i = 0; i < len; i++)
	{
		pChar[i] = replace(pChar[i], true);
	}
	return pChar;
}

std::string lock(const char *pChar, int len)
{
	initlock();
	std::string szret;
	szret.reserve(len);
	for (int i = 0; i < len; i++)
	{
		szret+= (char&)replace(pChar[i], true);
	}
	return szret;
}

