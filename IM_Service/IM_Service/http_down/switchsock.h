#ifndef _SWITCH_SOCK_H_
#define _SWITCH_SOCK_H_

#include "sock.h"
#include "mutex.h"

#define HTTP_SWITCH_PORT 8080

void http_useswitch(BOOL bUse=true);
void http_useproxy(bool bUse ,const string& Host="", unsigned short nPort=0);
void http_setswitch(string strSwitchHost, unsigned short nSwitchPort);
//---------------------------------------------------------------------------
//!即可以作为普通Socket，又可以对任意数据包打包以Http协议传送，还可以支持Http代理
class SwitchSock
{
public:
	SwitchSock();
	~SwitchSock();

	//!所有数据包都进行HTTP协议打包发送到HttpSwitch再转发出去, 客户端 <--> Http Switch <--> 服务器
	void SetUseHttpSwitch(BOOL bUse=true);

	//!系统内部使用的类型,在HttpSwitch端处理来自客户端的包
	void SetAsHttpSwitch(BOOL bAs=true);

	//!创建Socket，如果是HttpSwitch模式，则连接到Switch，如果需要Http代理，则连接到代理
	bool Create(bool bUDP=false);

	//!创建连接
	virtual bool Connect(const string& host,unsigned short port);
	//!是否已经连接
	virtual bool isConnected();
	//!是否是UDP包
	virtual bool isUDP();
	//!连接目标的地址
	virtual bool GetPeerName(string& strIP,unsigned short &nPort);
	//!连接本地的地址，如果采用了HttpSwitch，则是Switch服务器的Ip和端口
	virtual bool GetLocalName(string& strIP,unsigned short &nPort);

	//!绑定服务端口
	bool Bind(unsigned short nPort);
	//!接收客户端
	bool Accept(SwitchSock& client);

	//!关闭
	virtual void Close();

	//!发送
	virtual long Send(const char* buf,long buflen);
	//!接收
	virtual long Recv(char* buf,long buflen);
	//!发送
	virtual long SendTo(const char* buf,int len,const struct sockaddr_in* toaddr,int tolen);
	virtual long SendTo(const char* buf,int len,const string& strHost,unsigned short nPort);
	//!接收
	virtual long RecvFrom(char* buf,int len,struct sockaddr_in* fromaddr,int* fromlen);
	virtual long RecvFrom(char* buf,int len,string& strHost,unsigned short & nPort);

	//!获得Socket句柄，这个句柄是系统真正收发数据用的Socket，可以进行select,fcntl等操作，不能直接send recv
	SOCKET GetHandle();
private:
	//!标准的Socket，真正用来收发数据
	BaseSock m_sock;

	//!对数据包进行Http协议打包
	bool _SendHttpPacket(const string& param,const string& data);
	//!对数据包进行HTTP协议解包
	bool _RecvHttpPacket(string& param,string& data);

	//!解析收到的Http头
	void _ParseParam(const string& param, map<string,string>& paramlist);
private:
	BOOL m_bUseHttpSwitch;
	BOOL m_bAsHttpSwitch;

	string m_strCurSwitch;

	char* m_pHttpRecvBuffer;
	int m_nHttpRecvLen;

	string m_strPeerHost;
	unsigned short m_nPeerPort;
	friend class CHttpSwitch;

	MMutex m_SendRecvLock;

};

//////////////////////////////////API函数列表///////////////////////////////////
SOCKET http_socket( int af,  int type,  int protocol);

void http_closesocket(SOCKET s);


int http_bind(  SOCKET s,  const struct sockaddr* name,  int namelen);

SOCKET http_accept(  SOCKET s,  struct sockaddr* addr,  int* addrlen);

int http_connect(  SOCKET s,  const struct sockaddr* name,  int namelen);

int http_recv(  SOCKET s,  char* buf,  int len,  int flags);

int http_recvfrom(  SOCKET s,  char* buf,  int len,  int flags,  struct sockaddr* from,  int* fromlen);

int http_send(  SOCKET s,  const char* buf,  int len,  int flags);

int http_sendto(  SOCKET s,  const char* buf,  int len,  int flags,  const struct sockaddr* to,  int tolen);

#endif
