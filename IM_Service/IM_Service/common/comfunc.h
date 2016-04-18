#if !defined _COMMFUNCTION_EMC
#define _COMMFUNCTION_EMC

#include "tstring.h"
#include "comstruct.h"

unsigned long StringToDWORD(tstring str);

bool IsNumber(tstring lpStr);

char* GetLocalHost(char* szAddress);

//模块的路径
tstring GetCurrentPath();

bool LoadIniString(char *sApp, char *sKey, char *sReturn, int len, char *sFile, char *sDefault = NULL);

bool LoadIniInt(char *sApp, char *sKey, int &nReturn, char *sFile, char *sDefault = NULL);

//获取某几位的值
unsigned long GetMutiByte(unsigned int value, int index, int bytenum);
unsigned short GetMutiByte(unsigned short value, int index, int bytenum);
unsigned char GetMutiByte(unsigned char value, int index, int bytenum);

//设置多位的值, index表示位的开始，bytenum表示多少位
unsigned int SetMutiByte(unsigned int &source, int index, int bytenum, unsigned int value);
unsigned short SetMutiByte(unsigned short &source, int index, int bytenum, unsigned short value);
unsigned char SetMutiByte(unsigned char &source, int index, int bytenum, unsigned char value);

unsigned short SendOnePack(SOCKET socket, char *data, TCP_PACK_HEADER tcppackhead, int &nError);
unsigned short SendOnePack(SOCKET socket, char *data, int len, int &nError, unsigned short cmd,
	unsigned long senduid, unsigned long recvuid, unsigned long sendsock, unsigned long recvsock,
	unsigned short seq);

#endif