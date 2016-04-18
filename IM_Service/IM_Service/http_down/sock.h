/*
						file:	sock.h
						desc:	proxy socket class, support socks5 and http, can be used in linux and windows
						author: chen hua 
						create:	2005-03-16
*/
//---------------------------------------------------------------------------

#ifndef SockUnitH
#define SockUnitH
//---------------------------------------------------------------------------
#ifdef WIN32
#include <afxsock.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define SOCKET int
#endif

#pragma warning (disable: 4786)

#include "../Include/tstring.h"
#include <vector>
#include <map>
using namespace std;

#define SOCKET_TIMEOUT 30
#define MAX_RECV_BUFFERSIZE 65530

class SwitchSock;
class CHttpSwitch;

//!Socket基本功能封装类
class BaseSock
{
public:
	SOCKET m_sock;

public:
	BaseSock();
	virtual ~BaseSock();

	bool Create(bool bUDP=false);

	//virtual bool Connect(const tstring& host,unsigned short port);
	virtual bool Connect(const string& host,unsigned short port);  //gaoxiaohu

	virtual bool isConnected();
	virtual bool isUDP();
	virtual bool Bind(unsigned short nPort);
	virtual bool Accept(BaseSock& client);
	virtual void Close();

	//virtual long Send(const TCHAR* buf,long buflen);
	virtual long Send(const char* buf,long buflen); //gaoxiaohu
	//virtual long Recv(TCHAR* buf,long buflen);
	virtual long Recv(char* buf,long buflen);//gaoxiaohu
	//virtual long SendTo(const TCHAR* buf,int len,const struct sockaddr_in* toaddr,int tolen);
	virtual long SendTo(const char* buf,int len,const struct sockaddr_in* toaddr,int tolen);//gaoxiaohu
	//virtual long RecvFrom(TCHAR* buf,int len,struct sockaddr_in* fromaddr,int* fromlen);
	virtual long RecvFrom(char* buf,int len,struct sockaddr_in* fromaddr,int* fromlen);//gaoxiaohu

	//virtual bool GetPeerName(tstring& strIP,unsigned short &nPort);
	virtual bool GetPeerName(string& strIP,unsigned short &nPort);   //gaoxiaohu
	//virtual bool GetLocalName(tstring& strIP,unsigned short &nPort);
	virtual bool GetLocalName(string& strIP,unsigned short &nPort);//gaoxiaohu
	virtual bool GetPeerName(struct sockaddr_in* fromaddr,int* fromlen);
	virtual bool GetLocalName(struct sockaddr_in* fromaddr,int* fromlen);

	SOCKET GetHandle();
private:
	bool m_bUDP;
	bool m_bConnected;

	//tstring m_strHost;
	string m_strHost;
	unsigned short m_nPort;

	friend class CHttpSwitch;
	friend class SwitchSock;
};

extern	tstring IntToStr(unsigned long iValue);

#endif
