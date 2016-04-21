#pragma once

#include "sockbase.h"
#include "lock.h"
#include "zlib_stream.h"
#include "../chat_common/comstruct.h"
#include "../chat_common/comobject.h"

class CMySocket
{
public:

	DWORD m_MsgSocketCloseID, m_MsgRecvSuccID, m_MsgRecvFailID;

public:
	CMySocket();
	virtual ~CMySocket();

	bool m_bIM;

	char *m_sLine;
	int m_nLineMaxLen;
	string m_lastreadstr;

	bool   m_bRecvThread;

	bool Connect(const char* szDomain, unsigned short port);
	void Close();

	int SendImPack(char *sbuff, TCP_PACK_HEADER tcppackhead, int &nError);
	bool SendBuff(char *sbuff, int len, int &nError);

	SOCKET getfd(){ return m_hSocket; }

public:
	bool OnReceive();
	void OnClose();
	void SetReceiveObject(IBaseReceive* receiveObj);

protected:
	bool ConnectServer();
	bool  TestContinue();

	char   *m_szdomian;
	unsigned short m_port;
	CLock  m_sendlock;
	SOCKET m_hSocket;


	char* FormatLastError(int errorid, char * szErrbuffer = NULL, int nBufferSize = 0);

	string m_sRecvBuf;   //接收缓冲


	bool          m_bfirstpack;  //是否是第一包
	bool          m_bZlib;       //服务器是否支持zlib压缩 
	CZlibDeflate  m_zlibDe;
	CZlibInflate  m_zlibIn;
	IBaseReceive* m_receiveObj;

	bool OnRecvImPack();
	bool OnRecvWebPack();
	static void RecvThread(void *param);
};

