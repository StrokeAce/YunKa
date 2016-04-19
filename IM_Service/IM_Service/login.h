#pragma once

#include "./socket.h"
#include "./auth/auth_client.h"
#include "./common/comobject.h"
#include "./common/comfunc.h"
#include "./common/commcom.h"
#include "./socket.h"

class CChatManager;

class CLogin
{
public:
	CLogin();
	~CLogin();

public:
	void StartLogin(tstring loginName, tstring password, bool isAutoLogin, bool isKeepPwd);	// 开始登录

private:

	bool CheckLoginInfo(tstring loginName, tstring password,bool isAutoLogin, bool isKeepPwd);

	int GetTqAuthToken(unsigned int &uin, const char *szStrid, const char *szPassWord);

	bool CheckLoginFlag(unsigned long uin, const tstring& strID, bool bCreate = false, bool bFree = false);

	bool LoginToRealServer(tstring strServer, int nPort, unsigned int uin);

	int ConnectToServer(tstring sip, unsigned short port);

	int SendLoginInfo(unsigned int uin);
	
public:
	CChatManager* m_manager;
	CTqAuthClient *m_pTqAuthClient;
	char m_szAuthtoken[MAX_256_LEN + 1];//改成公共的
	unsigned long m_authadminid;
	int m_nLoginBy;					//当前的登陆类型
	CUserObject m_UserInfo;

public:
	CMySocket	m_Socket;	// 消息连接
	CMySocket	m_SocketEx; // 访客接待连接
};