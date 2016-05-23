// Pack.h: interface for the CPack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined _PACK_EM34563456
#define _PACK_EM34563456

#include "error_info.h"
#include "../chat_common/comstruct.h"
#include <tchar.h>

class CPack : public CErrorInfo
{

public:
	CPack();
	CPack(unsigned char *pBuff, unsigned short nSize,int langtype);
	CPack(CPack &pack,int langtype);
	 ~CPack();

private:
	//指向缓冲区头部
	unsigned char *m_pStart;
	
	//指向缓冲区当前操作位置
	unsigned char *m_pCurrent;
	
	//缓冲区长度
	unsigned short	m_nTotalLen;
	//包的长度
	unsigned short	m_nPackLen;
	//缓冲区当前的位置
	unsigned short	m_nIndex;

public:
	int     m_nlangtype;//字符串编码方式：1utf8 ,0 gbk
	int Encode();
	unsigned short GetPackLength();
	unsigned char * GetPackBuff();
	unsigned char * GetCurrentPackBuff();

	//直接拷贝某一块区域到一个包
	bool CopyBuff(unsigned char *pBuff, unsigned short nSize,int langtype);
	bool CopyBuff(CPack &pack,int langtype);

	//打包函数
	int Decode();
	bool operator<<(unsigned char a);
	bool operator<<(unsigned short a);
	bool operator<<(unsigned int a);
	bool packString(const char *a, int maxLen);

	//解包函数
	bool operator>>(unsigned char &a);
	bool operator>>(unsigned short &a);
	bool operator>>(unsigned int &a);
	bool unpackString(char *a, unsigned short &len, int maxLen=BUFFMAXLEN);
	void Clear();
};

#endif
