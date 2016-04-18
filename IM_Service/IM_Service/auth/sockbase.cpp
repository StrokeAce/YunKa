#include "sockbase.h"
#include <assert.h>
#include <stdio.h>

bool  InitSock(void)
{
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			WSACleanup( );
			return	false; 
		}
#endif
		return true;
}

void DestroySock(void)
{
#ifdef _WIN32
	WSACleanup();
#endif
}

void inetNtoa( in_addr * addr, char * ip, int size )
{
#ifndef _WIN32
	const unsigned char *p = ( const unsigned char *) addr;
	snprintf( ip, size, "%i.%i.%i.%i", p[0], p[1], p[2], p[3] );
#else
	_snprintf_s(ip, size, _TRUNCATE, "%i.%i.%i.%i", addr->s_net, addr->s_host, addr->s_lh, addr->s_impno); // ??
#endif
}



bool setNoDelay(int s,bool bNodelay)
{
	int nNodelay=bNodelay?1:0;
	return (setsockopt(s,IPPROTO_TCP,TCP_NODELAY,(char*)&nNodelay,sizeof(nNodelay))==0);
}


size_t sendn(SOCKET hsocket,const char *msg,size_t n)
{
	size_t nleft=n;
	const char* ptr=msg;

	int nerr=0;
	int nwritten;

	while(nleft>0)
	{
		if((nwritten=send(hsocket,ptr,nleft,0))<=0)
		{
#ifdef _WIN32
			int nerrno=WSAGetLastError();
			if (nwritten<0&&(errno==WSAEINTR||errno==WSAEWOULDBLOCK))
#else
			if (nwritten<0&&(errno==EAGAIN||errno==EINTR))
#endif
			{
				nwritten=0;
				if (++nerr>10)
				{
					break;
				}
			}else
			{
				return 0;
			}
		}
		nleft-=nwritten;
		ptr+=nwritten;
	}
	return n;
}

int setNonblock( SOCKET fd )
{
#ifdef _WIN32
	unsigned long nonblocking = 1;
	ioctlsocket( fd, FIONBIO, (unsigned long*) &nonblocking );
#else
	int flags;

	flags = fcntl( fd, F_GETFL );
	if( flags < 0 ) return flags;

	flags |= O_NONBLOCK;
	if( fcntl( fd, F_SETFL, flags ) < 0 ) return -1;
#endif

	return 0;
}

int setBlock( SOCKET fd )
{
#ifdef _WIN32

	unsigned long nonblocking = 0;
	ioctlsocket( fd, FIONBIO, (unsigned long*) &nonblocking );

#else

	int flags;

	flags = fcntl( fd, F_GETFL );
	if( flags < 0 ) return flags;

	flags &= ~O_NONBLOCK;
	if( fcntl( fd, F_SETFL, flags ) < 0 ) return -1;
#endif

	return 0;
}


bool set_address( const char* hname, unsigned short sin_port,struct sockaddr_in* sap)
{
	struct hostent* hp;

	memset( sap,0, sizeof( *sap ) );
	if( hname != NULL )
	{
		if( !inet_aton( hname, &sap->sin_addr ) )
		{
			hp = gethostbyname( hname );
			if( hp == NULL )
			{
//				ul_writelog(UL_LOG_ERROR, _T("gethostbyname(%s) failed, errno=%d, %s"), hname, errno, strerror( errno ) );
 				return false;
			}
			sap->sin_addr = *( struct in_addr* )hp->h_addr;
		}
	}
	else
	{
		sap->sin_addr.s_addr = htonl( INADDR_ANY );
	}

	sap->sin_family = AF_INET;
	sap->sin_port=htons(sin_port);
	return true;
}


SOCKET tcpServerSock( const char * ip, unsigned short port )
{
	struct sockaddr_in addr;
	if (!set_address(ip,port,&addr))
	{
		return INVALID_SOCKET;
	}
	
	SOCKET listenFd = socket( AF_INET, SOCK_STREAM, 0 );
	if( listenFd < 0 ) {
//		ul_writelog(UL_LOG_ERROR, _T("listen failed, errno=%d, %s"), errno, strerror( errno ) );
		return INVALID_SOCKET;
	}

	int flags = 1;
	if( setsockopt( listenFd, SOL_SOCKET, SO_REUSEADDR, (char*)&flags, sizeof( flags ) ) < 0 ) 
	{
//		ul_writelog(UL_LOG_ERROR, _T("failed to set setsock to reuseaddr fd=%d"), listenFd);
		closesocket(listenFd);
		return INVALID_SOCKET;
	}


	if( bind( listenFd, (struct sockaddr*)&addr, sizeof( addr ) ) < 0 ) {
//		ul_writelog(UL_LOG_ERROR, _T("bind failed, errno=%d, %s"), errno, strerror( errno ) );
		closesocket(listenFd);
		return INVALID_SOCKET;

	}

	if( ::listen( listenFd, 1024 ) < 0 ) {
//		ul_writelog(UL_LOG_ERROR, _T("listen failed, errno=%d, %s"), errno, strerror( errno ) );
		closesocket(listenFd);
		return INVALID_SOCKET;

	}

	//g_WriteLog.WriteLog(C_LOG_NOTICE, "Listen on port [%d]", port );

	return listenFd;
}


SOCKET tcpClientSock( char *hname,unsigned short uport, 
					 char* proxyname, unsigned short proxyport)
{
	struct sockaddr_in peer;
	SOCKET s;

	set_address( hname, uport, &peer);
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s==INVALID_SOCKET)
	{
//		ul_writelog(UL_LOG_ERROR, _T("create sock error, errno=%d, %s"), errno, strerror( errno ) );
		return s;
	}
	
	if(proxyname != NULL
		&& strlen(proxyname) > 0)
	{
		peer.sin_family = AF_INET;
		peer.sin_addr.s_addr = inet_addr(proxyname);
		if(peer.sin_addr.s_addr == INADDR_NONE)
		{
			struct hostent* phost = gethostbyname(proxyname);
			peer.sin_addr = *((struct   in_addr*)phost->h_addr);
		}
		peer.sin_port = htons(proxyport);
	}

	if ( connect( s, ( struct sockaddr * )&peer,sizeof( peer ) ) )
	{
		DWORD dwErr = WSAGetLastError();
//		ul_writelog(UL_LOG_ERROR, _T("connect to%s:%d,errno=%d, %s"),hname,uport,errno, strerror( errno ) );
		closesocket(s);
		s= INVALID_SOCKET;

		return s;
	}

	if(proxyname != NULL
		&& strlen(proxyname) > 0)
	{
		char buf[2000] = {0};
		sprintf_s(buf,"CONNECT %s:%u HTTP/1.0\r\nAccept: */*\r\nContent-Type: text/html\r\nProxy-Connection: Keep-Alive\r\nContent-length: 0\r\n\r\n",
			hname, uport);
		int nr=send(s,buf,strlen(buf),0);
		if (nr<=0)
		{
			closesocket(s);
			return INVALID_SOCKET; 

		}

		memset(buf, 0, sizeof(buf));
		nr=recv(s,buf,2000,0);
		if (nr<=0)
		{
			closesocket(s);
			return INVALID_SOCKET; 
		}
		buf[nr]='\0';
		if (strstr(buf,"established")==NULL)
		{
//			ul_writelog(UL_LOG_ERROR, _T("Can't throuth proxy server(%s:%d) connect to robot server(%s:%d)."),proxyname, proxyport, hname, uport);
			closesocket(s);
			return INVALID_SOCKET; 	
		}
	}

	return s;
}

SOCKET udpServerSock( const char* hname,const unsigned short port )
{

	SOCKET s;
	struct sockaddr_in local;

	set_address( hname, port, &local );
	s = socket( AF_INET, SOCK_DGRAM, 0 );
	if(s== INVALID_SOCKET)
	{
		//g_WriteLog.WriteLog(C_LOG_ERROR, "create sock error, errno=%d, %s", errno, strerror( errno ) );
		return INVALID_SOCKET;
	}

	if( bind( s, ( struct sockaddr* )&local, sizeof( local ) ) )
	{
		//g_WriteLog.WriteLog(C_LOG_ERROR, "bind error, errno=%d, %s", errno, strerror( errno ) );

		closesocket(s);
		s=INVALID_SOCKET;
	}

	return s;
}

SOCKET udpClientSock( void )
{
	SOCKET s = socket( AF_INET, SOCK_DGRAM, 0 );
	return s;
}

void SetSockTTL(SOCKET fd,int nPackTTL) 
{
	assert (nPackTTL>0);

	int curTTL=0;
	int lenTTL=sizeof(int);
	getsockopt(fd,IPPROTO_IP,IP_TTL,(char   *)&curTTL,   &lenTTL);
	if(curTTL!=nPackTTL)
	{
		setsockopt(fd,IPPROTO_IP,IP_TTL,(char *)&nPackTTL,sizeof(nPackTTL));
	}
}

bool CheckLocalIp(SOCKET fd)
{
	sockaddr_in clientaddr;
	int   namelen=sizeof(clientaddr);

	if (getpeername(fd,(sockaddr*)&clientaddr,&namelen))
	{
		return false;
	}
	unsigned long uip=ntohl(clientaddr.sin_addr.s_addr);
	// 只允许同一局域网网访问（192.168.0.0 192.168.255.255）
	if (uip<3232235520U||uip>3232301055U)
	{
		char ip[20];
		inetNtoa(&clientaddr.sin_addr,ip,20);
		//g_WriteLog.WriteLog(C_LOG_ERROR,"CNotificationEvent ip(%lu,%s) error",uip,ip);
		return false;
	}

	return true;
}

//////////////////////////////only for win32////////////////////////////////////////////
#ifdef _WIN32
int inet_aton(const char *c, struct in_addr* addr)
{
	unsigned int r;
	if (strcmp(c, "255.255.255.255") == 0) {
		addr->s_addr = 0xFFFFFFFFu;
		return 1;
	}
	r = inet_addr(c);
	if (r == INADDR_NONE)
		return 0;
	addr->s_addr = r;
	return 1;
}
#endif
