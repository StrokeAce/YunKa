#include "auth_client.h"
#include <windows.h>
#include <string>
#include <assert.h>
#include <time.h>
#include "md5.h"

using namespace std;

#define strncasecmp _strnicmp


CTqAuthClient::CTqAuthClient(const char *domain,unsigned short port,int cversion )
{
	assert(domain != NULL&&strlen(domain) <= MAXDOMAINLEN);
	strcpy_s(m_domain, domain);
	m_port = port;
	m_cversion = cversion;
	memset(m_proxyip, 0, sizeof(m_proxyip));
	m_proxyport = 0;

	srand((int)time(NULL));
	InitSock();
}

CTqAuthClient::~CTqAuthClient(void)
{
	DestroySock();
}

void CTqAuthClient::setproxyinfo(const char* proxyip, unsigned short proxyport)
{
	if(proxyip == NULL)
		return;

	strcpy_s(m_proxyip, proxyip);
	m_proxyport = proxyport;
}

//认证
int CTqAuthClient::Login(unsigned int uin,const char *strid,const char *pass,int type,const char* myip,
		  char * recvbuf,int &nbuflen,bool& butf8, const char* appendinfo)
{
	assert(uin>0||strid!=NULL);
	assert(recvbuf!=NULL);

	SOCKET s=tcpClientSock(m_domain,m_port, m_proxyip, m_proxyport);
	if (s==INVALID_SOCKET)	{		return -1;	}

	char szAppendInfo[30] = {0};

	if(appendinfo != NULL)
	{
		strcpy_s(szAppendInfo, appendinfo);
	}

	char strmd5[33]={0};
	char token[50]={0};
	char getbuf[1024]={0};
	if (uin>0)
	{
		sprintf_s(getbuf,"GET /login.do?type=%d&uin=%u&password=%s&token=%s&loginIp=%s&client=%s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: tq/%d\r\nHost: %s\r\nConnection: close\r\n\r\n",
			type, uin,getMd5(pass,strlen(pass),strmd5),CreateLoginTocken(token,50),myip, szAppendInfo, m_cversion,m_domain);
	}else
	{
		sprintf_s(getbuf, "GET /login.do?type=%d&strid=%s&password=%s&token=%s&loginIp=%s&client=%s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: tq/%d\r\nHost: %s\r\nConnection: close\r\n\r\n",
			type, strid,getMd5(pass,strlen(pass),strmd5),CreateLoginTocken(token,50),myip, szAppendInfo, m_cversion,m_domain);

	}

	size_t nlen=strlen(getbuf);
	if (sendn(s,getbuf,nlen)!=nlen)
	{
		return -2;
	}


	if (!RecvHttpPack(s,recvbuf,nbuflen,butf8))
	{
		return -3;
	}

	return 0;
}

//心跳包
int CTqAuthClient::Hearbeat(const char *ptoken,char * recvbuf,int &nbuflen,bool& butf8)
{
	assert(recvbuf!=NULL);
	SOCKET s=tcpClientSock(m_domain,m_port, m_proxyip, m_proxyport);
	if (s==INVALID_SOCKET)	{		return -1;	}

	char getbuf[1024]={0};
	sprintf_s(getbuf, "GET /heartbeat.do?token=%s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: tq/%d\r\nHost: %s\r\nConnection: close\r\n\r\n",
		ptoken,m_cversion,m_domain);
	size_t nlen=strlen(getbuf);
	if (sendn(s,getbuf,nlen)!=nlen)	{
		return -2;
	}

	if (!RecvHttpPack(s,recvbuf,nbuflen,butf8))
	{
		return -3;
	}
	return 0;
}

//取session数据
int  CTqAuthClient::Validate(const char *ptoken,char * recvbuf,int &nbuflen,bool& butf8)
{
	assert(recvbuf!=NULL);
	SOCKET s=tcpClientSock(m_domain,m_port, m_proxyip, m_proxyport);
	if (s==INVALID_SOCKET)	{		return -1;	}

	char getbuf[1024]={0};
	sprintf_s(getbuf, "GET /validate.do?token=%s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: tq/%d\r\nHost: %s\r\nConnection: close\r\n\r\n",
		ptoken,m_cversion,m_domain);
	size_t nlen=strlen(getbuf);
	if (sendn(s,getbuf,nlen)!=nlen)	{
		return -2;
	}

	if (!RecvHttpPack(s,recvbuf,nbuflen,butf8))
	{
		return -3;
	}
	return 0;
}

//注销session
int CTqAuthClient::Logout(const char *ptoken,char * recvbuf,int &nbuflen,bool& butf8)
{
	assert(recvbuf!=NULL);
	SOCKET s=tcpClientSock(m_domain,m_port, m_proxyip, m_proxyport);
	if (s==INVALID_SOCKET)	{		return -1;	}

	char getbuf[1024]={0};
	sprintf_s(getbuf, "GET /logout.do?token=%s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: tq/%d\r\nHost: %s\r\nConnection: close\r\n\r\n",
		ptoken,m_cversion,m_domain);
	size_t nlen=strlen(getbuf);
	if (sendn(s,getbuf,nlen)!=nlen)	{
		return -2;
	}

	if (!RecvHttpPack(s,recvbuf,nbuflen,butf8))
	{
		return -3;
	}
	return 0;
}


//生成认证需要的校验串       nbuflen必须大于58
char *CTqAuthClient::CreateLoginTocken(char * recvbuf,int nbuflen)
{
	assert(nbuflen>46);
	time_t  tnow=time(NULL);
	char strt[17]={0};
	sprintf_s(strt, "%.8X", tnow);

	int x1=rand()%256,x2=rand()%256,x3=rand()%256;
	sprintf_s(strt + 8, 8, "%.2X%.2X%.2X", x1, x2, x3); // ??

	char strmd5[33];
	getMd5(strt,14,strmd5);

	x1=0;x2=0;
	int i;
	for (i=0;i<42;++i)
	{
		if (i%3==2)
		{
			recvbuf[i]=strt[x1++];
		}else
		{
			recvbuf[i]=strmd5[x2++];
		}
	}

	memcpy(recvbuf+42,strmd5+28,4);
	recvbuf[46]=0;

	for(i=0;i<23;++i)
	{
		x1=i*2;
		swap<char>(recvbuf[x1],recvbuf[x1+1]);
	}

	return recvbuf;

}

bool CTqAuthClient::RecvHttpPack(SOCKET s,char * recvbuf,int &nbuflen,bool &butf8)
{
	//recv超时，接受超时
	int timeout=10000;//ms
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	nbuflen-=1; //末尾留 null 位置

	bool bret=true;
	bool bheadcomplete(false);
	int nrecvednum=0;
	while(nrecvednum<nbuflen)
	{
		int n=recv(s,recvbuf+nrecvednum,nbuflen-nrecvednum,0);
		if (n<=0)
		{
			bret=false;
			break;
		}

		recvbuf[nrecvednum+n]=0;
		if (!bheadcomplete)
		{
			char *pheadend=strstr(recvbuf+(nrecvednum>3?nrecvednum-3:0),"\r\n\r\n");
			if (pheadend!=NULL)
			{
				pheadend+=4;

				char *pcontentlen=strstr(recvbuf,"charset=");
				if (pcontentlen!=NULL)
				{
					pcontentlen+=sizeof("charset=")-1;
					butf8=strncasecmp(pcontentlen,"utf-8",6)==0?true:false;
				}

				bheadcomplete=true;
				pcontentlen=strstr(recvbuf,"Content-Length:");
				if (pcontentlen==NULL)
				{
					bret=false;
					break;
				}else
				{
					pcontentlen+= sizeof("Content-Length:")-1;
					while(' '== *pcontentlen) ++pcontentlen;

					int nbodylen=atoi(pcontentlen);
					nrecvednum+=n;
					
					nrecvednum-=(pheadend-recvbuf);
					memmove(recvbuf,pheadend,nrecvednum);
					nbuflen=nbodylen<nbuflen?nbodylen:nbuflen;
					if (nrecvednum>=nbodylen)
					{
						break;
					}
					continue;
				}
			}
		}
		nrecvednum+=n;
	}
	closesocket(s);
	recvbuf[nbuflen]=0;
	return bret;
}
