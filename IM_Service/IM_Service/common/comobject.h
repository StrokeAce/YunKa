#ifndef _COMOBJECT_EMC
#define _COMOBJECT_EMC

#include "comdef.h"
#include "tstring.h"
#include "comstruct.h"

#include <list>     
using namespace std;

//初始设置
class CSysConfigFile
{
public:
	CSysConfigFile();
	~CSysConfigFile();

	void ResetValue();
	bool LoadUserData(char *sFilename, unsigned short ver);
	bool LoadData(char *sFilename, unsigned short ver);
	bool SaveData(char *sFilename, unsigned short ver);
	void SetWndInitPos(bool bAlways);
	void DeleteAll();
	void DeleteAllLoginInfo();
	void DeleteAllAlertInfo();

	ALERT_INFO *SetAllDefaultAlertInfo(int type = -1);
	ALERT_INFO *CSysConfigFile::GetAlertInfo(int type);

public:
	char m_sConfigname[MAX_FILENAME_LEN];

public:
	//通过登录号获得登录字符串
	tstring GetStringFromUID(unsigned long uid);
	unsigned long GetUIDFromString(int type, tstring strID);

	//查看该用户在列表中的位置
	int GetLoginInfoIndex(int type, unsigned long uid, tstring strID);
	LOGIN_INFO* GetLoginInfo(int type, unsigned long uid, tstring sid);

	//增加一个新的对象
	LOGIN_INFO *AddOneLoginInfo(unsigned long uid, tstring sid, tstring pass, tstring compid);
	LOGIN_INFO *AddOneLoginInfo(LOGIN_INFO *LoginInfo);

	//增加一个登录服务器对象
	void AddServerAddress(tstring strServer, bool bMoveToTop = true);
	void CSysConfigFile::AddKeyWordSerchString(tstring strSearch, bool bMoveToTop);

	void CSysConfigFile::AddOrDeleteForbidWebuserSid(tstring sid, bool bAdd);
	bool CSysConfigFile::IsWebuserSidForbid(tstring sid);

	tstring CSysConfigFile::CombineFilterString();
	void CSysConfigFile::ParseFilterString(tstring strString);

	int nStartFlag, nEndFlag;

	bool bIsFirstRun;
	bool bFindSrv;

	bool bAutoSearchKeyword;

	bool bSavePass;			//是否保存密码
	bool bAutoLogin;		//是否自动登录
	bool bHideLogin;		//是否隐身登录
	int nKeySendType;		//0 enter; 1 alt+enter

	int nClientCopyInfo;	//版本信息 
	int nServerCopyInfo;	//版本信息

	unsigned long dwLastUpdateTime;
	int nCopyCheckType;				//0 not check 1 auto 2 timer
	int nCopyCheckTimeMWD;				//0 month day 1 week 2 day
	int nCopyCheckTimeMonth, nCopyCheckTimeWeek, nCopyCheckTimeDay;

	int nCopyDownType;			//0 后台下载 1 提示下载

	//初始窗口大小和类型
	int nWndDock;	//窗口的停靠位置
	int x, y, w, h;
	bool bThickMainWnd;
	int colorIndex;	//tree的颜色

	int nInfoHeight;
	int nSendMsgHeight;
	int nPreSendMsgHeight;

	//最后登录的类型
	int lastloginby;
	unsigned long lastloginuid;
	char lastloginstr[MAX_SID_LEN];
	char lastloginpass[MAX_PASSWORD_LEN];
	int lastloginstatus;
	char lastloginsstatus[MAX_STATUS_LEN];
	char lastlogincompid[MAX_STATUS_LEN];

	//最后登录的服务器信息
	int nConnectType;	//连接类型
	tstring strServer;
	int nServerPort;

	//访客接待
	tstring strVisitorServer;
	int nVisitorServerPort;

	tstring strRealServer;
	int nRealServerPort;

	int nProxyType;
	tstring strProxyServer;
	int nProxyPort;
	int nProxyLocalPort;
	tstring strProxyUser, strProxyPass;

	//保存登录过的登录信息
	list<LOGIN_INFO*> m_listLogin;

	//保存提醒方式
	list<ALERT_INFO*> m_listAlertInfo;

	//保存登陆过的服务器地址
	list<tstring> m_listServerAddress;

	//保存搜索的字
	list<tstring> m_listKeyWordSearchString;

	//保存评蔽的访客标示
	list<tstring> m_listForbidWebUserSid;

	int	 filtertype;

	FILTER_USERDEFINE infofilter;
	FILTER_USERDEFINE tailfilter;

	bool	m_bAutoResp;
	tstring	m_strWellcomeMsg;

	int		m_nUserTimeoutTime;
	tstring	m_strUserTimeoutMsg;

	int		m_nVisitorTimeoutTime;
	tstring	m_strVisitorTimeoutMsg;

	bool	m_bVisotorTimeoutClose;
	int		m_nVisitorTimeoutCloseTime;
	tstring	m_strVisitorTimeoutCloseMsg;

	bool	m_bAutoRespUnnormalStatus;
	tstring	m_strUnnormalStatusMsg[5];

	tstring	m_strInviteWords;
	tstring	m_strInviteTitle;

	//保存自动邀请语
	list<tstring> m_listInviteWords;
	int		m_nInviteType;

	int m_nKeywordsSort;
	int m_nKeywordsUser;
	tstring m_strKeywordsFind;
};

class CBaseObject
{
public:
	CBaseObject(){};
	~CBaseObject(){};

	virtual bool Load(unsigned short ver)=0;
	virtual bool Save(unsigned short ver)=0;
};

class CUserObject : public CBaseObject
{
public:
	CUserObject();
	~CUserObject();

	bool Load(unsigned short ver);
	bool Save(unsigned short ver);

public:
	USER_INFO UserInfo;
	unsigned char talkstatus;

	int	m_nWaitTimer;		//邀请， 消息发送等待时的定时处理

	unsigned char status;
	char sstatus[MAX_STATUS_LEN];

	unsigned long talkuid;
	unsigned long fd;	//他的socket连接，用来消息转发

	tstring m_strMsgHtml;
	tstring m_strMsgHtmlEx;	//这一部分内容，保存如果当前不是该用户的显示区，等待切换到该用户时，添加到m_strMsgHtml中
	list<ONE_MSG_INFO> m_strMsgs;// 个人聊天记录
	tstring m_strPreSendMsg;
	bool m_bConnected;

	bool m_bKeywordsChange;
	bool m_bKeywordsGet;

public:
	int	nTimer;  //定时器

	int nVisitNum;
	int nTalkNum;
	int nLastVisitTime;

	int nFlashFirstOnline;			//第一次上线闪烁标志 0 不闪，1 -> 开始闪

	unsigned long nbak;
	tstring sbak;

	bool bVisitMeAfterLogon;		//该用户在启动后没有交流过
	bool bTalkMeAfterLogon;			//该用户在启动后没有交流过

	int m_nFlag;					//程序处理时使用变量
	bool m_bOpen;
	bool m_bReset;

	bool m_bFriend;					//是否有好对象
	bool m_bInnerTalk;				//是否内部对话
};

class IBaseReceive
{
public:
	virtual void OnReceive(void* pHead, void* pData) = 0;
};

#endif