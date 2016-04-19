#pragma once

#include "comm_struct.h"
#include "sys_config.h"
#include "chat_objects.h"
#include "msgs.h"
#include "login.h"
#include "./common/comobject.h"
#include "./common/comstruct.h"

class IBaseMsgs
{
public:
	// 登录的进度,percent=100时表示登录成功
	virtual void LoginProgress(int percent) = 0;

	// 获取上一次错误信息
	virtual tstring GetLastError() = 0;

	// 收到一条消息
	virtual void OnRecvOneMsg() = 0;

	// 坐席上线消息
	virtual void LogingMsg() = 0;

	// 坐席下线消息
};

class CChatManager : public IBaseReceive
{
private:
	CChatManager(IBaseMsgs* iBaseMsgs);
	~CChatManager();

	friend class CLogin;
	friend class CMySocket;

	virtual void OnReceive(void* pHead, void* pData);

public:

	static CChatManager* GetInstance(IBaseMsgs* iBaseMsgs)
	{
		static CChatManager instance(iBaseMsgs);
		return &instance;
	}

public:

	// 获取上一次登录信息
	void GetPreLoginInfo(tstring& loginName, tstring& password, bool& isAutoLogin, bool& isKeepPwd);

	// 开始登录
	void StartLogin(tstring loginName, tstring password, bool isAutoLogin, bool isKeepPwd);

	// 获取系统配置对象
	CSysConfig* GetSysConfig();

	// 获取聊天对象管理类实例
	CChatObjects* GetChatObjects();

	// 获取聊天消息管理类实例
	CMsgs* GetMsgs();

	// 截图
	void ScreenCapture();

	
private:
	/***************     配置文件操作函数     *****************/

	bool ReadSystemConfig();

	void SetSystemConfigByInitconfig();

	void SolveUserdefineFilter(FILTER_USERDEFINE &filter, char *sfilter);


	/***************     消息包处理函数      *****************/

	int RecvSrvConfLogon(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvRepUserinfo(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype);


	/***************     消息包处理函数      *****************/


	int UnPack(CPackInfo *pPackInfo, char *buff, int len);

	bool LoadINIResource();

	int SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid = 0, unsigned long recvsock = 0, HWND hWnd = NULL);

	bool LoadUserConfig();  //按照坐席个人在加载一份配置文件

private:
	CLogin* m_login;				// 登录处理类
	CChatObjects* m_chatObjects;	// 消息处理类
	bool m_isLoginSuccess;			// 是否已登录
	IBaseMsgs* m_baseMsgs;			// 服务端消息接收接口
	CSysConfigFile* m_sysConfig;	// 系统配置文件类
	INIT_CONF m_InitConfig;

	CMySocket	m_Socket;	// 消息连接
	CMySocket	m_SocketEx; // 访客接待连接

	tstring m_loginName;			// 登录名
	tstring m_password;				// 登录密码
	bool m_isAutoLogin;				// 是否自动登录
	bool m_isKeepPwd;				// 是否记住密码
	tstring	m_server;
	int		m_port;

	CUserObject m_UserInfo;
	unsigned short	m_usSrvRand;	//服务器的随机数
	unsigned short	m_usCltRand;	//本次运行的随机数

	tstring m_lastError;
};

