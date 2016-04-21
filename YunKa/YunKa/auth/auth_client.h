#ifndef TQAUTHCLIENT_H_
#define TQAUTHCLIENT_H_

#include "sockbase.h"

#define  MAXDOMAINLEN 100

class CTqAuthClient
{

	char m_domain[MAXDOMAINLEN+1];
	unsigned short m_port;
	char m_proxyip[MAXDOMAINLEN+1];
	unsigned short m_proxyport;
	int            m_cversion;
public:
	CTqAuthClient(
		const char *domain,      //IN 认证服务器地址
		unsigned short port,     //IN 认证服务器端口
		int	 cversion			 //IN  本地版本号

		);
	~CTqAuthClient(void);


	//!认证
	int Login(unsigned int uin,  //IN 用户号码
		const char *strid,		 //uin为0，使用strid,uin>0时忽略此参数
		const char *pass,		 //IN  密码
		int type,				 //IN  类型： 1.客户端 ，2 java
		const char* myip,		 //IN  来源ip地址
		char * recvbuf,			 //OUT 存放服务器返回数据
		int &nbuflen,			 //INOUT 输入recvbuf的大小，返回接受到数据大小
		bool& butf8,				 //OUT   recvbuf的编码方式，1：utf-8   0：gbk
		const char* appendinfo = NULL);// 附加客户端登陆信息，区分项目

	//!心跳包，需半个小时调用一次
	int Hearbeat(const char *ptoken,//IN Login中返回的token参数
		char * recvbuf,				//OUT 存放服务器返回数据
		int &nbuflen,				//INOUT 输入recvbuf的大小，返回接受到数据大小
		bool& butf8);				//OUT   recvbuf的编码方式，1：utf-8   0：gbk

	//!取session数据
	int  Validate(const char *ptoken,//IN Login中返回的token参数
		char * recvbuf,				//OUT 存放服务器返回数据
		int &nbuflen,				//INOUT 输入recvbuf的大小，返回接受到数据大小
		bool& butf8					//OUT   recvbuf的编码方式，1：utf-8   0：gbk
		);

	//!注销session
	int Logout(const char *ptoken,	//IN Login中返回的token参数
		char * recvbuf,				//OUT 存放服务器返回数据
		int &nbuflen,				//INOUT 输入recvbuf的大小，返回接受到数据大小
		bool& butf8					//OUT   recvbuf的编码方式，1：utf-8   0：gbk
		);

	void setproxyinfo(const char* proxyip, unsigned short proxyport);

private:
	//生成认证需要的校验串       nbuflen必须大于46
	static char* CreateLoginTocken(char * recvbuf,int nbuflen);

	//接受服务器返回的http数据
	bool RecvHttpPack(SOCKET s,char * recvbuf,int &nbuflen,bool &butf8);

};

#endif 

