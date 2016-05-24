#if !defined _COMMFUNCTION_EMC
#define _COMMFUNCTION_EMC

#include "comstruct.h"
#include "wx_obj.h"

//长整形和字符串的互换
string DWORDToString(unsigned long num);
unsigned long StringToDWORD(string str);

bool IsNumber(string lpStr);

char* GetLocalHost(char* szAddress);

//模块的路径
string GetCurrentPath();

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

int SendAllBuff(SOCKET socket, const char *sbuff, int len, int &nError);

void ConvertMsg(char *msg, int buflen);

void ConvertWidecharToChar(const WCHAR *pFrom, int len, char *pTo, int buflen, bool butf8);

/*
解析成功创建微信消息对象，
用户用完需要delete该对象
*/
WxObj* ParseWxJsonMsg(const char* msg);

//获取时间, 格式：MM-DD HH:MM:SS
string GetTimeStringMDAndHMS(unsigned long ntime);

string GetExtDateTimeFormatTime(unsigned long ntime);

char *GetContentBetweenString(const char *str, const char *sstart, const char * send, char *content);

bool GetByte(unsigned int value, int index);
unsigned int SetByte(unsigned int &source, int index, unsigned char ucvalue);

string FullPath(string extPath);

//获得当前的系统时间
unsigned long GetCurrentLongTime();

string GetMd5Str(const string str);

bool IsXMLCommandStringExist(char *pBuff, char *cmditem);

char *GetXMLCommandString(char *pBuff, char *cmd, char *cmditem, int maxlen = 1024);

int GetXMLCommandInt(char *pBuff, char *cmditem);

bool ParseSearchURLHostAndVar(string strurl, string &strHost, string &strVar);

int GetApplyTypeID(string stype);

string GetApplyTypeString(int type);

#endif