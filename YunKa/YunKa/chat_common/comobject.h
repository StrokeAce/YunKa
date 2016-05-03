#ifndef _COMOBJECT_EMC
#define _COMOBJECT_EMC

#include "comdef.h"
#include "string.h"
#include "comstruct.h"
#include "wx_obj.h"

#include <list>     
using namespace std;

class IBaseReceive
{
public:
	virtual void OnReceive(void* pHead, void* pData) = 0;
};

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
	string GestringFromUID(unsigned long uid);
	unsigned long GetUIDFromString(int type, string strID);

	//查看该用户在列表中的位置
	int GetLoginInfoIndex(int type, unsigned long uid, string strID);
	LOGIN_INFO* GetLoginInfo(int type, unsigned long uid, string sid);

	//增加一个新的对象
	LOGIN_INFO *AddOneLoginInfo(unsigned long uid, string sid, string pass, string compid);
	LOGIN_INFO *AddOneLoginInfo(LOGIN_INFO *LoginInfo);

	//增加一个登录服务器对象
	void AddServerAddress(string strServer, bool bMoveToTop = true);
	void CSysConfigFile::AddKeyWordSerchString(string strSearch, bool bMoveToTop);

	void CSysConfigFile::AddOrDeleteForbidWebuserSid(string sid, bool bAdd);
	bool CSysConfigFile::IsWebuserSidForbid(string sid);

	string CSysConfigFile::CombineFilterString();
	void CSysConfigFile::ParseFilterString(string strString);

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
	string strServer;
	int nServerPort;

	//访客接待
	string strVisitorServer;
	int nVisitorServerPort;

	string strRealServer;
	int nRealServerPort;

	int nProxyType;
	string strProxyServer;
	int nProxyPort;
	int nProxyLocalPort;
	string strProxyUser, strProxyPass;

	//保存登录过的登录信息
	list<LOGIN_INFO*> m_listLogin;

	//保存提醒方式
	list<ALERT_INFO*> m_listAlertInfo;

	//保存登陆过的服务器地址
	list<string> m_listServerAddress;

	//保存搜索的字
	list<string> m_listKeyWordSearchString;

	//保存评蔽的访客标示
	list<string> m_listForbidWebUserSid;

	int	 filtertype;

	FILTER_USERDEFINE infofilter;
	FILTER_USERDEFINE tailfilter;

	bool	m_bAutoResp;
	string	m_strWellcomeMsg;

	int		m_nUserTimeoutTime;
	string	m_strUserTimeoutMsg;

	int		m_nVisitorTimeoutTime;
	string	m_strVisitorTimeoutMsg;

	bool	m_bVisotorTimeoutClose;
	int		m_nVisitorTimeoutCloseTime;
	string	m_strVisitorTimeoutCloseMsg;

	bool	m_bAutoRespUnnormalStatus;
	string	m_strUnnormalStatusMsg[5];

	string	m_strInviteWords;
	string	m_strInviteTitle;

	//保存自动邀请语
	list<string> m_listInviteWords;
	int		m_nInviteType;

	int m_nKeywordsSort;
	int m_nKeywordsUser;
	string m_strKeywordsFind;
};

class IBaseObject
{
public:
	IBaseObject(){};
	~IBaseObject(){};

	virtual bool Load(unsigned short ver)=0;
	virtual bool Save(unsigned short ver)=0;

	int m_nEMObType;
};

class CUserObject : public IBaseObject
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

	string m_strMsgHtml;
	string m_strMsgHtmlEx;	//这一部分内容，保存如果当前不是该用户的显示区，等待切换到该用户时，添加到m_strMsgHtml中
	list<ONE_MSG_INFO> m_strMsgs;// 个人聊天记录
	string m_strPreSendMsg;
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
	string sbak;

	bool bVisitMeAfterLogon;		//该用户在启动后没有交流过
	bool bTalkMeAfterLogon;			//该用户在启动后没有交流过

	int m_nFlag;					//程序处理时使用变量
	bool m_bOpen;
	bool m_bReset;

	bool m_bFriend;					//是否有好对象
	bool m_bInnerTalk;				//是否内部对话
};

class CWebUserObject : public IBaseObject
{
public:
	CWebUserObject();
	~CWebUserObject();

	//个人信息
	bool Load(unsigned short ver);
	bool Save(unsigned short ver);

	void AddCommonTalkId(unsigned long uid);

	bool IsExistCommonTalkId(unsigned long uid);

	bool IsOnline();

public:
	unsigned char	m_bNewComm;					//新协议
	unsigned short  m_sNewSeq;					//新协议,当前接受的消息id
	int				m_nIndex;					//用于标示用户个数的数字
	WEBUSER_INFO	info;
	WEBONLINE_INFO	onlineinfo;
	unsigned long	webuserid;
	unsigned long	floatadminuid;				//漂浮框管理员号码
	int				nlangtype;					//字符集
	unsigned long	floatfromadminuid;			//会话来源网站管理员
	unsigned short	floatfromsort;				//会话来信漂浮框id
	char			chatid[MAX_CHATID_LEN + 1];	//工单id，唯一区别一次会话
	unsigned short	gpid;

	//以下信息用于用户第一次上线是使用
	unsigned long 	talkuid;					//对话的用户 第一次对话用户
	unsigned long 	transferuid;				//要转接的用户
	unsigned long 	inviteuid;					//邀请的用户。默认为SYS_WEBUSER
	unsigned long 	frominviteuid;				//主动邀请的用户。

	unsigned char	cTalkedSatus;				//0没有通过话，HASTALKED 1  通过话,INTALKING	2 正在通话中
	//CMapStringToOb m_ListUrlAndScriptFlagOb;	// 判断web用户是否在线
	list<unsigned long> m_listCommonTalkId;		// 共同对话的其他客服用户列表
	unsigned char	m_bNotResponseUser;			//1 自己不是应答客服，0自己是应答客服
	char prevurl[MAX_URL_LEN];
	char prevurlhost[MAX_URL_LEN];
	char prevurlvar[MAX_URL_LEN];
	unsigned char	refuseinvite;				//用户拒绝过邀请， 但不包括自动邀请
	int	m_nWaitTimer;							//邀请， 消息发送等待时的定时处理
	int m_resptimeoutmsgtimer;					//访客未应答自动回复时间
	int m_resptimeoutclosetimer;				//访客长时间未回复，开始自动关闭对话
	int m_waitresptimeouttimer;					//客服未应答自动回复时间
	//以下用于自动邀请
	unsigned long m_onlinetime;					//上线的时间
	time_t m_leaveTime;							// 用于排序 [12/14/2010 SC]
	string m_strInfoHtml;
	string m_strTail;
	string m_strInfo;
	string m_strPreSendMsg;
	list<ONE_MSG_INFO> m_strMsgs;
	bool m_bConnected;							//该用户已经和你建立连接，可以通讯了
	bool m_bIsGetInfo;							// 是否已获取到信息
public:
	string m_strHistory;						//总的历史纪录的显示
	string m_strTotal;
	string m_strListMsg;
	string m_strListInfo;
	string m_strListTail;
	int	nTimer;									//定时器
	int nVisitFrom;								//访客来源
	int nVisitNum;
	int nTalkNum;
	int nLastVisitTime;
	unsigned long bak;
	int m_nFlag;								//程序处理时使用变量
	bool m_bIsShow;								//是否显示
	bool m_bIsFrWX;								//是否来自微信
	string m_sWxAppid;							//会话来源的微信appid
	bool GetNormalChatHisMsgSuccess;			//控制历史消息
	time_t tGetNormalChatHismsgTime;

	WxUserInfo* m_pWxUserInfo;
};


#endif