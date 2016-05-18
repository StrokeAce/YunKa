#include "socket.h"
#include "../chat_common/commsg.h"
#include "../chat_common/comcom.h"
#include "../chat_common/comfunc.h"
#include "chat_manager.h"
#include "encrypt.h"
#include <process.h>
#include <mutex>

#define  MAXSOCKPACKSIZE 32000

CMySocket::CMySocket()
{
	m_MsgRecvSuccID = WM_SOCKET_RECVSUCC;
	m_MsgRecvFailID = WM_SOCKET_RECVFAIL;

	m_MsgSocketCloseID = WM_SOCKET_CLOSE;
	m_bIM = true;

	m_sLine = NULL;
	m_nLineMaxLen = 0;

	m_hSocket = INVALID_SOCKET;

	m_bRecvThread = true;
	m_szdomian = NULL;
	m_port = 0;

	m_bfirstpack = true;
	m_bZlib = false;

	_beginthread(RecvThread, 0, this);
}

CMySocket::~CMySocket()
{
	
}

void CMySocket::RecvThread(void *param)
{
	CMySocket *pthis = (CMySocket*)param;

	while (pthis->m_bRecvThread)
	{
		if (pthis->m_hSocket == INVALID_SOCKET)
		{
			Sleep(10);
			continue;
		}
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvThread create fd:%d", pthis->m_hSocket);

		while (pthis->m_hSocket != INVALID_SOCKET)
		{
			fd_set fd;
			FD_ZERO(&fd);
			FD_SET(pthis->m_hSocket, &fd);
			struct timeval val = { 10, 0 };
			int ret = select(pthis->m_hSocket + 1, &fd, NULL, NULL, &val);
			if (ret <= 0)
			{
				continue;
			}

			if (!pthis->OnReceive())
			{
				g_WriteLog.WriteLog(C_LOG_ERROR, "OnReceive failed");
				break;
			}
		}

		if (pthis->TestContinue())
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "TestContinue successed,newfd=%d", pthis->m_hSocket);
			continue; //接续连接
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "TestContinue failed");
		}

		if (pthis->m_hSocket != INVALID_SOCKET)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "socket失效，掉线");
			pthis->Close();
			pthis->OnClose();
		}
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvThread exited");
	_endthread();
}

bool  CMySocket::TestContinue()
{
	if (!m_bIM || !ConnectServer())
	{
		return false;
	}

	//发送ping包接续连接试试
	COM_SEND_PING SendInfo(VERSION);
	SendInfo.o.online_flag = ((CChatManager*)m_receiveObj)->m_userInfo.UserInfo.onlineflag;
	((CChatManager*)m_receiveObj)->SendPackTo(&SendInfo, 0, 0);

	fd_set fd;;
	FD_ZERO(&fd);
	FD_SET(m_hSocket, &fd);
	struct timeval val = { 10, 0 };
	int ret = select(m_hSocket + 1, &fd, NULL, NULL, &val);
	if (ret <= 0)
	{
		return false;
	}

	unsigned short plen = (0);
	int n = recv(m_hSocket, (char*)&plen, 2, MSG_PEEK);
	if (n != 2 || 0 == plen)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "TestContinue recv:%d,packlen:%hd", n, plen);

		return false;
	}

	return true;
}

bool CMySocket::OnReceive()
{
	bool isOk = false;
	if (m_hSocket <= 0)
		return false;

	static const int onerecvbufsize = 8091;
	char sztempbuf[onerecvbufsize + 1] = { 0 };

	int nThisRecvNum(0);

	while (true)
	{
		int nRecv = recv(m_hSocket, sztempbuf, onerecvbufsize, 0);
		if (nRecv <= 0)
		{
			if (nThisRecvNum > 0)
			{
				break;
			}
			else
			{
				int nerrid = WSAGetLastError();
				if (!nRecv || (nerrid != WSAEWOULDBLOCK&&nerrid != WSAECONNABORTED))
				{
					g_WriteLog.WriteLog(C_LOG_TRACE, "OnReceive recv=%d nerrid:%d,%s", nRecv, nerrid, FormatLastError(nerrid));
					return false;
				}
				return true;
			}

		}
		else
		{
			nThisRecvNum += nRecv;
			if (!m_bIM&&m_bZlib)
			{
				if (Z_OK != m_zlibIn.InflateData(sztempbuf, nRecv, m_sRecvBuf))
				{
					//数据错误了，需要断网重连
					g_WriteLog.WriteLog(C_LOG_ERROR, "OnRecvWebPack InflateData error, domain:%s, port:%hd", m_szdomian, m_port);
					return false;
				}
			}
			else
			{
				m_sRecvBuf.append(sztempbuf, nRecv);
			}
		}
	}


	if (m_bIM)
	{
		isOk = OnRecvImPack();
	}
	else
	{
		isOk = OnRecvWebPack();
	}
	return isOk;
}


bool CMySocket::OnRecvImPack()
{
	int ndatasize = m_sRecvBuf.size();
	g_WriteLog.WriteLog(C_LOG_TRACE, "the recv buffersize is %d", ndatasize);

	int nTcpPackHeadLen = sizeof(TCP_PACK_HEADER);
	while (ndatasize > nTcpPackHeadLen)
	{
		const TCP_PACK_HEADER *pTcpPackHead = (const TCP_PACK_HEADER *)m_sRecvBuf.c_str();
		if (pTcpPackHead->len > MAXSOCKPACKSIZE - 2)
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "tcp head size:%d> MAXSOCKPACKSIZE", pTcpPackHead->len);
			return false;
		}

		int nPacklen = pTcpPackHead->len + nTcpPackHeadLen;
		if (ndatasize < nPacklen)
		{
			break;
		}
		::lock((char*)m_sRecvBuf.c_str() + nTcpPackHeadLen, pTcpPackHead->len);

		PACK_HEADER *phead = (PACK_HEADER*)(m_sRecvBuf.c_str() + nTcpPackHeadLen);

		static  unsigned  int  npacknumseq = 0;

		g_WriteLog.WriteLog(C_LOG_TRACE, "OnRecvImPack recv cmd:%.4x ,%u", phead->cmd, ++npacknumseq);

		m_receiveObj->OnReceive((void*)pTcpPackHead, (void*)(m_sRecvBuf.c_str() + nTcpPackHeadLen));

		m_sRecvBuf.erase(0, nPacklen);
		ndatasize -= nPacklen;
	}

	return ndatasize < MAXSOCKPACKSIZE;
}

bool CMySocket::OnRecvWebPack()
{
	while (!m_sRecvBuf.empty())
	{
		string::size_type posend = m_sRecvBuf.find("\n");
		if (posend == string::npos)
		{
			break;
		}
		++posend;
		string spack = m_sRecvBuf.substr(0, posend);
		m_sRecvBuf.erase(0, posend);

		g_WriteLog.WriteLog(C_LOG_TRACE, "OnRecvWebPack recv %s", spack.c_str());

		if (m_bfirstpack)
		{
			m_bfirstpack = false;
			if (spack.find("<ZLIB>1</ZLIB>") != string::npos)
			{
				m_bZlib = true;
				std::string temp = m_sRecvBuf;
				m_sRecvBuf.clear();
				if (Z_OK != m_zlibIn.InflateData(temp.c_str(), temp.size(), m_sRecvBuf))
				{
					//数据出错了，不应该出错
					g_WriteLog.WriteLog(C_LOG_ERROR, "OnRecvWebPack InflateData error, domain:%s, port:%hd", m_szdomian, m_port);
					return false;
				}
			}
		}
		m_receiveObj->OnReceive((void*)&posend, (void*)(spack.c_str()));
	}
	return m_sRecvBuf.size() < MAXSOCKPACKSIZE;
}

char* CMySocket::FormatLastError(int errorid, char * szErrbuffer, int nBufferSize)
{
	static char szErrBuf[256] = { 0 };
	szErrBuf[0] = 0;
	if (!szErrbuffer)
	{
		szErrbuffer = szErrBuf;
		nBufferSize = 256;
	}

	LPVOID lpMsgBuf;
	if (!FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorid,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL))
	{
		// Handle the error.
		return szErrBuf;
	}

	strncpy_s(szErrbuffer, _TRUNCATE, (const char*)lpMsgBuf, nBufferSize - 1); // ??
	// Free the buffer.s
	LocalFree(lpMsgBuf);

	return szErrbuffer;
}


bool CMySocket::Connect(const char* szDomain, unsigned short port)
{
	if (m_szdomian) free(m_szdomian);
	m_szdomian = _strdup(szDomain);
	m_port = port;

	return ConnectServer();
}

bool CMySocket::ConnectServer()
{
	Close();
	m_bfirstpack = true;
	m_bZlib = false;
	m_zlibIn.Init();
	m_zlibDe.Init();
	m_sRecvBuf.clear();

	struct sockaddr_in peer;
	if (!set_address(m_szdomian, m_port, &peer))
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "set_address error, domain:%s, port:%hd", m_szdomian, m_port);
		return FALSE;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		int nerrid = WSAGetLastError();
		g_WriteLog.WriteLog(C_LOG_ERROR, "create sock error, errno=%d, %s", errno, FormatLastError(nerrid));
		return FALSE;
	}

	if (connect(s, (struct sockaddr *)&peer, sizeof(peer)))
	{
		int nerrid = WSAGetLastError();
		g_WriteLog.WriteLog(C_LOG_ERROR, "connect to %s:%hu,errno=%d, %s", m_szdomian, m_port, errno, FormatLastError(nerrid));
		closesocket(s);
		s = INVALID_SOCKET;
		return FALSE;
	}

	setNonblock(s);
	setNoDelay(s, true);


	m_hSocket = s; //赋值后导致recv线程开始执行

	return m_hSocket != INVALID_SOCKET;
}


void CMySocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

void CMySocket::OnClose()
{
	g_WriteLog.WriteLog(C_LOG_TRACE, "CMySocket::OnClose m_bIM:%d", m_bIM);
	//::SendMessage(m_hWnd, m_MsgSocketCloseID, 0, 0);
}

void CMySocket::SetReceiveObject(IBaseReceive* receiveObj)
{
	m_receiveObj = receiveObj;
}

int CMySocket::SendImPack(char *sbuff, TCP_PACK_HEADER tcppackhead, int &nError)
{

	AutoLock  ilock(m_sendlock);

	return SendOnePack(m_hSocket, sbuff, tcppackhead, nError);
}

bool CMySocket::SendBuff(char *sbuff, int len, int &nError)
{
	AutoLock  ilock(m_sendlock);

	if (m_bIM || !m_bZlib) 
	{
		return len == SendAllBuff(m_hSocket, sbuff, len, nError);
	}
	else 
	{
		std::string szZout;
		if (Z_OK != m_zlibDe.DeFlateData(sbuff, len, szZout))
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "SendBuff DeFlateData error, errno=%d, %s", len, sbuff);
			return false;
		}

		return szZout.size() == SendAllBuff(m_hSocket, szZout.c_str(), szZout.size(), nError);
	}
	return true;
}
