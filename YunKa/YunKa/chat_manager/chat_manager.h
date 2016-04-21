#pragma once

#include "sys_config.h"
#include "chat_objects.h"
#include "msgs.h"
#include "login.h"
#include "../chat_common/comobject.h"
#include "../chat_common/comstruct.h"
#include "timer.h"

class IBaseMsgs
{
public:
	// 登录的进度,percent=100时表示登录成功
	virtual void LoginProgress(int percent) = 0;

	// 获取上一次错误信息
	virtual string GetLastError() = 0;

	// 收到一条消息
	virtual void OnRecvOneMsg() = 0;

	// 坐席上线消息
	virtual void Online() = 0;

	// 坐席下线消息
	virtual void Offline() = 0;

	// 会话关闭
	virtual void CloseChat() = 0;
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
	void GetPreLoginInfo(string& loginName, string& password, bool& isAutoLogin, bool& isKeepPwd);

	// 开始登录
	void StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd);

	// 发送获取好友对象列表事件
	void SendTo_GetShareList();

	// 发送获取会话列表事件
	void SendTo_GetListChatInfo();

	// 发送获取某个用户的消息
	int SendTo_GetUserInfo(unsigned long uid);

	// 获取系统配置对象
	CSysConfig* GetSysConfig();

	// 获取聊天对象管理类实例
	CChatObjects* GetChatObjects();

	// 获取聊天消息管理类实例
	CMsgs* GetMsgs();

	// 截图
	void ScreenCapture();

	static void CALLBACK TimerProc(void* p);

private:
	/***************     配置文件操作函数     *****************/

	bool ReadSystemConfig();

	void SetSystemConfigByInitconfig();

	void SolveUserdefineFilter(FILTER_USERDEFINE &filter, char *sfilter);

	bool LoadINIResource();

	bool LoadUserConfig();  //按照坐席个人在加载一份配置文件


	/***************     消息包处理函数      *****************/

	int RecvSrvConfLogon(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvRepUserinfo(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype);

	int RecvSrvConfLogOff(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvStatusFrdOnline(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvStatusFrdOffline(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvStatusUserForm(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatShareList(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvComSendMsg(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatCreateChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatChatMsg(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatChatMsgAck(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatAcceptChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatTransQuest(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatTransFailed(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvInviteRequest(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvInviteResult(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatKefuRelease(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatCMDError(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatCloseChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatListChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatChatInfo(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvEventAnnouncement(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvUpdateSucc(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvUpdateFail(PACK_HEADER packhead, char *pRecvBuff, int len);

	//用户在异地登陆
	int RecvSrvDonw(PACK_HEADER packhead, char *pRecvBuff, int len);

	//转移临时用户成功失败
	int RecvRepTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len);

	//接收转移会话的用户处理
	int RecvTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len);


	/***************     消息包处理函数      *****************/

	int UnPack(CPackInfo *pPackInfo, char *buff, int len);

	int SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid = 0, unsigned long recvsock = 0, HWND hWnd = NULL);


	CUserObject *AddUserObject(unsigned long id, char *sid, char *name, unsigned char status, unsigned long fd);

	CUserObject *GetUserObject(unsigned long id);

	int GetAllUser();

	unsigned short GetPackSeq();

private:
	CLogin*					m_login;				// 登录处理类
	CChatObjects*			m_chatObjects;			// 消息处理类	
	IBaseMsgs*				m_baseMsgs;				// 服务端消息接收接口
	CSysConfigFile*			m_sysConfig;			// 用户设置文件类
	INIT_CONF				m_InitConfig;			// 配置文件
	CMySocket				m_Socket;				// 消息连接
	CMySocket				m_SocketEx;				// 访客接待连接
	string					m_loginName;			// 登录名
	string					m_password;				// 登录密码
	string					m_server;				// 服务器地址
	string					m_lastError;			// 上一次错误信息
	bool					m_isAutoLogin;			// 是否自动登录
	bool					m_isKeepPwd;			// 是否记住密码
	bool					m_isLoginSuccess;		// 是否已登录
	int						m_port;					// 服务器端口
	CUserObject				m_UserInfo;				// 登录用户的信息	
	unsigned short			m_usSrvRand;			// 服务器的随机数
	unsigned short			m_usCltRand;			// 本次运行的随机数

	map<unsigned long, CUserObject*> m_mapUsers;
	map<string, CTimer*> m_mapTimers;
	CUserObject m_CommUserInfo;	//公用用户
	int m_myInfoIsGet;		//是否我的信息已经获取到了
	int						m_nOnLineStatus;		// 用户是否在线
	
};

