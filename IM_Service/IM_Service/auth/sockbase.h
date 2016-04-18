#ifndef SOCKBASE_H_
#define SOCKBASE_H_

#ifndef WIN32
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define closesocket close

#else

#include <WinSock2.h>
#include <MsWsock.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32") 
int inet_aton(const char *c, struct in_addr* addr);

#endif

/** 初始化话sock环境*/
bool  InitSock(void);
void DestroySock(void);

/**
* @brief <b>Summary:</b>
*        set sockaddr_in structure
* 
* @param[in] hname
*            host name,if hname value is NULL,address will be set to INADDR_ANY
* @param[in] sin_port
*            service port in  network byte order 
* @param[out] sap
*            sockaddr_in pointer to set
* @return int
****************************************************************************************/
bool set_address( const char* hname, unsigned short sin_port,struct sockaddr_in* sap);

/** 网络地址转字符串地址*/
void inetNtoa( in_addr * addr, char * ip, int size );

/** 禁止套接口上的Nagle算法*/
bool setNoDelay(int s,bool bnodely);

/** 循环发送指定字节的数据*/
size_t sendn(SOCKET hsocket,const char *msg,size_t n);

/** 设置套接口为非阻塞模式*/
int setNonblock( SOCKET fd );

/** 设置套接口为阻塞模式*/
int setBlock( SOCKET fd );


/**
 * @brief <b>Summary:</b>
 *        new a tcp server with designated host name and service name
 * 
 * @param[in] hname
 *            host name,if hname value is NULL,address will be set to INADDR_ANY
 * @param[in] uport
 *            port to bind,
 * @return SOCKET if success,return listen socket,else return INVALID_SOCKET
****************************************************************************************/
SOCKET tcpServerSock( const char * ip, unsigned short uport);

/**
* @brief <b>Summary:</b>
*        connect tcp client to designated host name and service name
* 
* @param[in] hname
*            server host name
* @param[in] port
*            server port
* @return SOCKET if success,return connected socket,else eturn INVALID_SOCKET
****************************************************************************************/
SOCKET tcpClientSock( char *hname,  unsigned short uport, 
					 char* proxyname = NULL, unsigned short proxyport = 0);


/**
* @brief <b>Summary:</b>
*        new a udp server with designated host name and service name
* 
* @param[in] hname
*            host name,if hname value is NULL,address will be set to INADDR_ANY
* @param[in] port
*            service port in host byte order,
* @return SOCKET if success,return bind socket,else return INVALID_SOCKET
****************************************************************************************/
SOCKET udpServerSock( const char* hname,const unsigned short port );


/**
* @brief <b>Summary:</b>
*        set udp client to designated host name and service name
* 
* @param[in] hname
*            server host name
* @param[in] sname
*            server service name,
* @return SOCKET if success,return valid socket,else return INVALID_SOCKET
****************************************************************************************/
SOCKET udpClientSock( void );

/**
设置一套接口的TTL值
****************************************************************************************/
void SetSockTTL(SOCKET fd,int nPackTTL) ;


//判断tcpsock 来源是否在同一局域网网访问（192.168.0.0 192.168.255.255）
bool CheckLocalIp(SOCKET fd);

#endif
