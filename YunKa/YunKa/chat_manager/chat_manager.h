#pragma once

#include "login.h"
#include "timer.h"
#include "chat_visitor.h"
#include "../chat_common/comobject.h"
#include "../chat_common/comstruct.h"
#include "../chat_common/comfloat.h"

typedef map<string/*thirdId*/, string/*公众号token*/> WxTokens; // 公众号的thirdid和token一一对应
typedef map<unsigned long, CUserObject*> MapUsers; // 保存坐席用户
typedef map<char*, CWebUserObject*> MapWebUsers; // 保存访客

// 登录消息的回调接口
class IHandlerLgoin
{
public:
	// 登录的进度,percent=100时表示登录成功
	virtual void LoginProgress(int percent) = 0;
};

// 登录后通信消息的回调接口
class IHandlerMsgs
{
public:
	
	// 收到一个坐席用户的信息,用来初始化坐席列表
	virtual void RecvOneUserInfo(CUserObject* pWebUser) = 0;

	// 收到一个新建的会话消息
	virtual void RecvCreateChat(CWebUserObject* pWebUser) = 0;

	// 收到一个会话消息
	virtual void RecvChatInfo(CWebUserObject* pWebUser) = 0;

	// 收到更新用户的在线状态
	virtual void RecvUserStatus(CUserObject* pUser) = 0;

	// 坐席上线消息
	virtual void RecvOnline(CUserObject* pUser) = 0;

	// 坐席下线消息
	virtual void RecvOffline(CUserObject* pUser) = 0;

	//************************************
	// Method:    RecvAcceptChat
	// Qualifier: 坐席接受会话的通知消息
	// Parameter: pUser 接受会话的坐席对象
	// Parameter: pWebUser 被接受的访客对象
	//************************************
	virtual void RecvAcceptChat(CUserObject* pUser, CWebUserObject* pWebUser) = 0;

	//************************************
	// Method:    RecvCloseChat
	// Qualifier: 会话关闭通知消息
	// Parameter: pWebUser 会话关闭相关的访客
	//************************************
	virtual void RecvCloseChat(CWebUserObject* pWebUser) = 0;

	// 获取上一次错误信息
	virtual string GetLastError() = 0;

	//************************************
	// Method:    RecvOneMsg
	// Qualifier: 收到一条消息
	// Parameter: pObj 聊天的对象，其他坐席、web访客、微信访客
	// Parameter: msgFrom 消息的发送者类型，其他坐席、访客(微信或web)、协助对象(另一个坐席)
	// Parameter: msgId 消息的唯一id
	// Parameter: msgType 消息类型，预知消息(主要针对web访客)、普通消息
	// Parameter: msgDataType 消息的数据类型，文字(包括表情)、语音、图片、坐标、视频等
	// Parameter: msgContent 消息的具体内容
	// Parameter: msgTime 收到消息的时间
	// Parameter: pAssistUser 协助对象，当消息为协助对象发来时，需要该参数
	// Parameter: msgContentWx 微信消息，当非文字的微信消息时，需要该参数
	// Parameter: msgExt 预留的参数
	//************************************
	virtual void RecvOneMsg(IBaseObject* pObj, int msgFrom, string msgId, int msgType, int msgDataType,	string msgContent, 
		string msgTime = "", CUserObject* pAssistUser = NULL, WxMsgBase* msgContentWx = NULL, string msgExt = "") = 0;
};

class CChatManager : public IBaseReceive
{
public:

	~CChatManager();

	// 获取通信管理类实例，单例模式
	static CChatManager* GetInstance();

	// 设置接收登录消息的接口
	void SetHandlerLogin(IHandlerLgoin* handlerLogin);

	// 设置接收登录后通信消息的接口
	void SetHandlerMsgs(IHandlerMsgs* handlerMsgs);

public:

	// 获取上一次登录信息,调用者不用关心析构
	ListLoginedInfo GetPreLoginInfo();

	// 开始登录
	void StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd);

	// 发送在线状态的消息
	int SendTo_UpdateOnlineStatus(unsigned short status);

	// 发送获取好友对象列表的消息
	int SendTo_GetShareList();

	// 发送获取会话列表的消息
	int SendTo_GetListChatInfo();

	// 发送获取所有用户的信息的消息
	int SendTo_GetAllUserInfo();

	// 发送获取某个坐席信息的消息
	int SendTo_GetUserInfo(unsigned long uid);

	// 发送获取某个访客信息的消息
	int SendTo_GetWebUserInfo(unsigned long webuserid, const char *chatid, char *szMsg = "", unsigned int chatkefuid = 0);

	// 发送获取某个会话信息的消息
	int SendTo_GetWebUserChatInfo(unsigned short gpid, unsigned long adminid, char *chatid);

	// 截图
	void ScreenCapture();
	
	// 退出，程序退出时调用
	void Exit();

private:
	CChatManager();
	friend class CLogin;
	friend class CMySocket;

	/***************     继承接口的函数实现    *****************/

	virtual void OnReceive(void* pHead, void* pData);


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
	int RecvSrvDown(PACK_HEADER packhead, char *pRecvBuff, int len);

	//转移临时用户成功失败
	int RecvRepTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len);

	//接收转移会话的用户处理
	int RecvTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len);

	void RecvComSendNormalChatidHisMsg(unsigned long senduid, unsigned long recvuid, COM_SEND_MSG& RecvInfo);

	int RecvComTransRequest(unsigned long senduid, COM_SEND_MSG& RecvInfo);

	int RecvComTransAnswer(unsigned long senduid, COM_SEND_MSG& RecvInfo);

	void RecvComSendWorkBillMsg(unsigned long senduid, unsigned long recvuid, char *msg, char* mobile);


	/***************     坐席和访客信息管理函数      *****************/

	int UnPack(CPackInfo *pPackInfo, char *buff, int len);

	int SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid = 0, unsigned long recvsock = 0, HWND hWnd = NULL);

	CUserObject *AddUserObject(unsigned long id, char *sid, char *name, unsigned char status, unsigned long fd);

	CUserObject *GetUserObjectByUid(unsigned long id);

	CWebUserObject *GetWebUserObjectBySid(char *sid);

	CWebUserObject *GetWebUserObjectByUid(unsigned long uid);

	CWebUserObject *AddWebUserObject(char *sid, char *thirdid, char *name, char *scriptflag, char *url,
		unsigned char status, unsigned char floatauth);

	unsigned short GetPackSeq();	

	void TimerSolveAck();

	int SendPing();

	void SetOfflineStatus();

	void CloseAllSocket();


	/***************     类内使用的发送到服务端的消息      *****************/

	int Send_GetInfo(unsigned long id, char *strid, unsigned short cmd, unsigned short cmdtype = 0, unsigned short type = 0);

	int SendAckEx(unsigned short seq, unsigned long uid = 0, unsigned long ip = 0, unsigned short port = 0);

	int SendAutoRespMsg(CWebUserObject *pWebUser, const char *msg, BOOL bClearTimer = true);

	//开始接受会话消息
	int SendStartRecvFloatMsg(unsigned short gpid, unsigned long adminid, char *chatid, unsigned short sLastMsgid);

	// 获取userinfo和token
	void SendGetWxUserInfoAndToken(CWebUserObject* pWebUser);

	// 获取微信用户信息
	int SendGetWxUserInfo(unsigned long webuserid, const char *chatid);

	// 获取微信公众号token
	int SendGetWxToken(unsigned long webuserid, const char *chatid);

	// 文字消息中的表情字符转换
	void TransforFaceMsg(string& str);

	// 微信消息的解析
	WxMsgBase* ParseWxMsg(CWebUserObject* pWebUser, COM_FLOAT_CHATMSG& recv);

	string GetMsgId();

	void SaveEarlyMsg(MSG_INFO *pMsgInfo);

	void SolveWebUserEarlyMsg(CWebUserObject *pWebUser);

	void SaveUserConfig();

	CWebUserObject *ChangeWebUserSid(CWebUserObject *pWebUser, char *sid, char *thirdid);

	void GetChatSysMsg(char* msg, CUserObject *pInviteUser, CWebUserObject *pWebUser, int type, CUserObject *pAcceptUser = NULL);

	// 定时器事件处理函数
	static void CALLBACK TimerProc(string timeName, LPVOID pThis); 

public:
	int						m_nOnLineStatus;		// 用户是否在线
	int						m_nOnLineStatusEx;
	bool					m_bExit;
	CUserObject				m_userInfo;				// 登录用户的信息

private:
	CLogin*					m_login;				// 登录处理类	
	IHandlerLgoin*			m_handlerLogin;			// 登录消息接收接口
	IHandlerMsgs*			m_handlerMsgs;			// 通信消息接收接口
	CSysConfigFile*			m_sysConfig;			// 用户设置文件类
	INIT_CONF				m_initConfig;			// 软件配置文件类
	CMySocket				m_socket;				// 消息连接
	CChatVisitor*			m_vistor;				// 访客接待处理对象
	string					m_loginName;			// 登录名
	string					m_password;				// 登录密码
	string					m_server;				// 服务器地址
	string					m_lastError;			// 上一次错误信息
	bool					m_isAutoLogin;			// 是否自动登录
	bool					m_isKeepPwd;			// 是否记住密码
	bool					m_isLoginSuccess;		// 是否已登录
	int						m_port;					// 服务器端口		
	unsigned short			m_usSrvRand;			// 服务器的随机数
	unsigned short			m_usCltRand;			// 本次运行的随机数
	MapUsers				m_mapUsers;				// 协助对象的存储集合
	MapWebUsers				m_mapWebUsers;			// 访客的存储集合
	CUserObject				m_CommUserInfo;			// 公用用户
	int						m_myInfoIsGet;			// 是否我的信息已经获取到了	
	CTimerManager*			m_timers;				// 定时器管理类
	int						m_nMyInfoIsGet;			// 是否我的信息已经获取到了
	int						m_nSendPing;			// 心跳包发送的次数
	int						m_nSendPingFail;		// 心跳包发送失败次数 
	int						m_nLoginToVisitor;		// 
	 
	unsigned long			m_nNextInviteWebuserUid;// 邀请的访客信息
	unsigned long			m_nNextInviteUid;		// 主动邀请的用户
	WxTokens				m_mapTokens;			// 公众号token存储集合
	MMutex					m_idLock;				// 生成消息id的锁
	int						m_msgId;				// 消息id，自增
	list<MSG_INFO*>			m_listEarlyMsg;			// 保存还未初始化访客对象之前收到的消息
	int						m_nClientIndex;			// 访客的序列号，自增
};

