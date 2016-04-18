#pragma once

#include "./socket.h"
#include "./auth/auth_client.h"
#include "./common/comobject.h"
#include "./common/comfunc.h"
#include "./common/commcom.h"
#include "./socket.h"

typedef void (*CALL_BACK_PROGRESS)(int percent);

class CChatManager;

class CLogin : public IBaseReceive
{
public:
	CLogin();
	~CLogin();

	friend class CMySocket;

public:
	void GetPreLoginInfo(tstring& loginName, tstring& password,	bool& isAutoLogin, bool& isKeepPwd); // 获取上一次登录信息

	void StartLogin(tstring loginName, tstring password, bool isAutoLogin, bool isKeepPwd, CALL_BACK_PROGRESS pCallBack);	// 开始登录

private:
	virtual void OnReceive(void* pHead, void* pData);

	bool CheckLoginInfo(tstring loginName, tstring password,bool isAutoLogin, bool isKeepPwd);

	int GetTqAuthToken(unsigned int &uin, const char *szStrid, const char *szPassWord);

	bool LoadINIResource();

	bool CheckLoginFlag(unsigned long uin, const tstring& strID, bool bCreate = false, bool bFree = false);

	bool ReadSystemConfig();

	void SetSystemConfigByInitconfig();

	void SolveUserdefineFilter(FILTER_USERDEFINE &filter, char *sfilter);

	bool LoadUserConfig();  //按照坐席个人在加载一份配置文件

	bool LoginToRealServer(tstring strServer, int nPort, unsigned int uin);

	int ConnectToServer(tstring sip, unsigned short port);

	int SendLoginInfo(unsigned int uin);

	int SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid = 0, unsigned long recvsock = 0, HWND hWnd = NULL);

	int RecvSrvConfLogon(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvRepUserinfo(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype);

	int UnPack(CPackInfo *pPackInfo, char *buff, int len);
private:
	CALL_BACK_PROGRESS m_callbackProgress;
	tstring m_lastError;	

public:
	CChatManager* m_manager;
	CSysConfigFile* m_sysConfig;
	CTqAuthClient *m_pTqAuthClient;
	char m_szAuthtoken[MAX_256_LEN + 1];//改成公共的
	unsigned long m_authadminid;
	int m_unreadnoterecnum;
	int m_unreadphonerecnum;
	int m_nSendAuthToken;
	INIT_CONF m_InitConfig;
	int m_nLoginBy;					//当前的登陆类型
	CUserObject m_UserInfo;
	unsigned short	m_usCltRand;	//本次运行的随机数
	unsigned short	m_usSrvRand;	//服务器的随机数
	tstring m_loginName;			// 登录名
	tstring m_password;				// 登录密码
	bool m_isAutoLogin;				// 是否自动登录
	bool m_isKeepPwd;				// 是否记住密码
	tstring	m_server;
	int		m_port;

public:
	CMySocket	m_Socket;	// 消息连接
	CMySocket	m_SocketEx; // 访客接待连接
};