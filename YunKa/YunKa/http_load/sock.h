/*
						file:	sock.h
						desc:	proxy socket class, support socks5 and http, can be used in linux and windows
						author: chen hua 
						create:	2005-03-16
*/
//---------------------------------------------------------------------------

#ifndef SockUnitH
#define SockUnitH

#include "../auth/sockbase.h"


#pragma warning (disable: 4786)

#include <vector>
#include <map>
using namespace std;

#define SOCKET_TIMEOUT 30
#define MAX_RECV_BUFFERSIZE 65530

class SwitchSock;

//!Socket基本功能封装类
class BaseSock
{
public:
	SOCKET m_sock;

public:
	BaseSock();
	virtual ~BaseSock();

	bool Create(bool bUDP=false);

	virtual bool Connect(const string& host,unsigned short port);

	virtual bool isConnected();
	virtual bool isUDP();
	virtual bool Bind(unsigned short nPort);
	virtual bool Accept(BaseSock& client);
	virtual void Close();

	virtual long Send(const char* buf,long buflen);
	virtual long Recv(char* buf,long buflen);
	virtual long SendTo(const char* buf,int len,const struct sockaddr_in* toaddr,int tolen);
	virtual long RecvFrom(char* buf,int len,struct sockaddr_in* fromaddr,int* fromlen);

	virtual bool GetPeerName(string& strIP,unsigned short &nPort);
	virtual bool GetLocalName(string& strIP,unsigned short &nPort);
	virtual bool GetPeerName(struct sockaddr_in* fromaddr,int* fromlen);
	virtual bool GetLocalName(struct sockaddr_in* fromaddr,int* fromlen);

	SOCKET GetHandle();
private:
	bool m_bUDP;
	bool m_bConnected;

	string m_strHost;
	unsigned short m_nPort;

	friend class CHttpSwitch;
	friend class SwitchSock;
};


#endif
