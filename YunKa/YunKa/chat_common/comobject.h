#ifndef _COMOBJECT_EMC
#define _COMOBJECT_EMC

#include "comdef.h"
#include "string.h"
#include "comstruct.h"
#include "wx_obj.h"

#include <list>     
using namespace std;

typedef list<LOGIN_INFO*> ListLoginedInfo;
typedef list<ALERT_INFO*> ListAlertInfo;
typedef map<string, WEBUSER_URL_INFO*> MapWebUserFLag;

class IBaseReceive
{
public:
	virtual void OnReceive(void* wParam, void* lParam) = 0;
};

//初始设置
class CSysConfigFile
{
public:
	CSysConfigFile();
	~CSysConfigFile();

	void ResetValue();
	bool LoadData(char *sFilename);
	bool SaveData(char *sFilename=NULL);
	bool ReadFile(ifstream& fout);
	bool WriteFile(ofstream& fin);
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

	//增加一个新的对象
	LOGIN_INFO *AddLatestLoginInfo(unsigned long uid, string sid, string pass, bool bAutoLogin, bool bKeepPwd);

	//增加一个登录服务器对象
	void AddServerAddress(string strServer, bool bMoveToTop = true);
	void CSysConfigFile::AddKeyWordSerchString(string strSearch, bool bMoveToTop);

	void CSysConfigFile::AddOrDeleteForbidWebuserSid(string sid, bool bAdd);
	bool CSysConfigFile::IsWebuserSidForbid(string sid);

	string CSysConfigFile::CombineFilterString();
	void CSysConfigFile::ParseFilterString(string strString);

	void Write(ofstream& fin, bool bVal);
	void Write(ofstream& fin, int iVal);
	void Write(ofstream& fin, unsigned long lVal);
	void Write(ofstream& fin, unsigned int Val);
	void Write(ofstream& fin, const char* sVal);
	void Write(ofstream& fin, byte byVal);

	void Read(ifstream& fout, bool& bVal);
	void Read(ifstream& fout, int& iVal);
	void Read(ifstream& fout, unsigned long& lVal);
	void Read(ifstream& fout, unsigned int& Val);
	void Read(ifstream& fout, string& sVal);
	void Read(ifstream& fout, byte& byVal);
	void Read(ifstream& fout, char* chVal);

	int					m_nKeySendType;				// 发送类型，enter或ctrl+enter
	int					m_nX;						// 客户端起始x坐标
	int					m_nY;						// 客户端起始y坐标
	int					m_nWidth;					// 客户端的宽
	int					m_nHeight;					// 客户端的高
	int					m_nSendMsgHeight;			// 发送框的高度
	int					m_nPreSendMsgHeight;		// 预发消息框的高度
	int					m_nLastLoginBy;				// 最后登陆的类型
	unsigned long		m_sLastLoginUid;			// 最后登陆的用户uin
	string				m_sLastLoginStr;			// 最后登陆的用户名
	string				m_sLastLoginPass;			// 最后登陆的密码
	string				m_sStrServer;				// 登陆的服务器地址
	int					m_nServerPort;				// 登陆的服务器端口
	string				m_sVisitorServer;			// web访客服务器地址
	int					m_nVisitorServerPort;		// web访客服务器端口
	string				m_sStrRealServer;			// 真实登陆的服务器地址
	int					m_nRealServerPort;			// 真实登陆的服务器端口

	ListLoginedInfo		m_cLoginInfoList;			// 登陆过用户的列表

	ListAlertInfo		m_cAlertInfoList;			// 提示音列表

	list<string>		m_cServerAddressList;		// 服务器地址列表

	list<string>		m_cKeyWordSearchList;		// 搜索过的关键字列表

	list<string>		m_cForbidWebUserList;		// 被屏蔽的访客的编号列表

	FILTER_USERDEFINE	m_cWebUserfilter;			// 过滤条件列表

	int					m_nFilterType;				// 默认访客过滤条件序列，只对等待接待、访问中和已离开有效
	bool				m_bAutoResp;				// 自动应答开关
	string				m_sWellcomeMsg;				// 欢迎语
	int					m_nUserTimeoutTime;			// 客服长时间未应答自动回复时限
	string				m_sUserTimeoutMsg;			// 回复语
	int					m_nVisitorTimeoutTime;		// 访客长时间未回应自动回复时限
	string				m_sVisitorTimeoutMsg;		// 回复语
	bool				m_bVisotorTimeoutClose;		// 自动关闭会话开关
	int					m_nVisitorTimeoutCloseTime;	// 访客长时间未回复自动关闭会话时限
	string				m_sVisitorTimeoutCloseMsg;	// 回复语
	bool				m_bAutoRespUnnormalStatus;	// 非正常在线自动应答开关
	string				m_sUnnormalStatusMsg;		// 回复语

	string				m_sInviteWords;				// 邀请语
	list<string>		m_cInviteWordsList;			// 邀请语列表

	int					m_nInviteType;				// 邀请类型
	int					m_nKeywordsSort;			// 辅助应答分类栏序列
	int					m_nKeywordsUser;			// 辅助应答所属栏序列
	string				m_sKeywordsFind;			// 辅助应答查找栏关键字
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

	void DownLoadFace(char* loadUrl);

public:
	USER_INFO UserInfo;
	unsigned char talkstatus;

	int	m_nWaitTimer;		//邀请， 消息发送等待时的定时处理

	unsigned char status;
	char sstatus[MAX_STATUS_LEN];

	unsigned long talkuid;
	unsigned long fd;	//他的socket连接，用来消息转发

	list<ONE_MSG_INFO> m_strMsgs;// 个人聊天记录
	string m_strPreSendMsg;
	bool m_bConnected;

	bool m_bKeywordsChange;
	bool m_bKeywordsGet;
	string m_loadHeadUrl;

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

	int IsForbid();

	bool IsDisplay(CSysConfigFile *pConfig, unsigned long uid = 0);

	void SetForbid(bool bForbid);

	bool IsIDIsMutiUser(unsigned long uid);
	void AddMutiUser(unsigned long uid);
	void RemoveMutiUser(unsigned long uid);
	bool IsMutiUser();
	void RemoveAllMutiUser();

	bool ScriptFlagIsExist(char *scriptflag);
	void AddScriptFlag(char *scriptflag, char *url);
	void DeleteScriptFlag(char *scriptflag);
	void DeleteAllScriptFlag();
	WEBUSER_URL_INFO* GetScriptFlagOb(char *scriptflag);
	WEBUSER_URL_INFO* GetLastScriptFlagOb();

public:
	unsigned char			m_bNewComm;					//新协议
	unsigned short			m_sNewSeq;					//新协议,当前接受的消息id
	int						m_nIndex;					//用于标示用户个数的数字
	WEBUSER_INFO			info;
	WEBONLINE_INFO			onlineinfo;
	WEBUSEREX_INFO			exinfo;
	unsigned long			webuserid;
	unsigned long			floatadminuid;				//漂浮框管理员号码
	int						nlangtype;					//字符集
	unsigned long			floatfromadminuid;			//会话来源网站管理员
	unsigned short			floatfromsort;				//会话来信漂浮框id
	char					chatid[MAX_CHATID_LEN + 1];	//工单id，唯一区别一次会话
	unsigned short			gpid;

	//以下信息用于用户第一次上线是使用
	unsigned long 			talkuid;					//对话的用户 第一次对话用户
	unsigned long 			transferuid;				//要转接的用户
	unsigned long 			inviteuid;					//邀请的用户。默认为SYS_WEBUSER
	unsigned long 			frominviteuid;				//主动邀请的用户。

	unsigned char			cTalkedSatus;				//0没有通过话，HASTALKED 1  通过话,INTALKING	2 正在通话中
	MapWebUserFLag			m_mapUrlAndScriptFlagOb;	// 判断web用户是否在线
	list<unsigned long>		m_listCommonTalkId;			// 共同对话的其他客服用户列表
	unsigned char			m_bNotResponseUser;			//1 自己不是应答客服，0自己是应答客服
	char					prevurl[MAX_URL_LEN];
	char					prevurlhost[MAX_URL_LEN];
	char					prevurlvar[MAX_URL_LEN];
	unsigned char			m_refuseinvite;				//用户拒绝过邀请， 但不包括自动邀请
	int						m_nWaitTimer;				//邀请， 消息发送等待时的定时处理
	int						m_resptimeoutmsgtimer;		//访客未应答自动回复时间
	int						m_resptimeoutclosetimer;	//访客长时间未回复，开始自动关闭对话
	int						m_waitresptimeouttimer;		//客服未应答自动回复时间

	//以下用于自动邀请
	unsigned long			m_onlinetime;				// 上线的时间
	time_t					m_leaveTime;				// 用于排序 [12/14/2010 SC]
	string					m_strInfoHtml;
	string					m_strTail;
	string					m_strInfo;
	string					m_strPreSendMsg;
	list<ONE_MSG_INFO>		m_strMsgs;
	bool					m_bConnected;				// 该用户已经和你建立连接，可以通讯了
	bool					m_bIsGetInfo;				// 是否已获取到信息
public:
	string					m_strHistory;				//总的历史纪录的显示
	string					m_strTotal;
	string					m_strListMsg;
	string					m_strListInfo;
	string					m_strListTail;
	int						nTimer;						//定时器
	int						nVisitFrom;					//访客来源
	int						nVisitNum;
	int						nTalkNum;
	int						nLastVisitTime;
	unsigned long			bak;
	int						m_nFlag;					//程序处理时使用变量
	bool					m_bIsShow;					//是否显示
	bool					m_bIsFrWX;					//是否来自微信
	string					m_sWxAppid;					//会话来源的微信appid
	bool					GetNormalChatHisMsgSuccess;	//控制历史消息
	time_t					tGetNormalChatHismsgTime;

	WxUserInfo*				m_pWxUserInfo;
};


#endif