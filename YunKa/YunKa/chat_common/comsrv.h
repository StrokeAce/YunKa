#if !defined _COMMSRV_EM
#define _COMMSRV_EM

#include "comcom.h"
#include "comdef.h"

class SRV_ERROR_CLTSEND:public CPackInfo
{
public:
	SRV_ERROR_CLTSEND(unsigned short ver);
	~SRV_ERROR_CLTSEND();
	
	bool unpack();  //read
	bool pack();  //write
	;

public:

	unsigned short type;  //错误类型 
/*
		0  命令号错误
		1  uin错误
		2  包大小错误
*/

	unsigned short cmd;					//客户端的命令号
	char	strMemo[MAX_RESPONSE_LEN];	//描述
};

//#define CMD_SRV_RESP_FAIL					0x0014	//  20 服务器回应处理某一个包失败
class SRV_RESP_FAIL:public CPackInfo
{
public:	
	SRV_RESP_FAIL(unsigned short ver);
	SRV_RESP_FAIL(unsigned short ver, unsigned short seq, unsigned short cmd);
	~SRV_RESP_FAIL();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short	seq;		//返回收到的包序号
	unsigned short	cmd;		//哪一个命令处理失败
	unsigned short	cmdtype;	//用非类表示,和cmd选一个
	unsigned short  type;		//包中的分类
	unsigned short  typecmd;	//包中的分类用cmd表示，和type选一个
	unsigned int	uin;		//是哪一个用户有关

	unsigned short	langtype;	//语言标识
	unsigned short	deny;		//失败的序号
	char			reason[MAX_RESPONSE_LEN];		//255  失败的类型描述
	char			strmemo[MAX_RESPONSE_LEN];		//255  失败的类型描述
};

//#define CMD_SRV_SERVER_COPY					0x001E	//  30 服务器当机
class SRV_SERVER_COPY:public CPackInfo
{

public:
	SRV_SERVER_COPY(unsigned short ver);
	~SRV_SERVER_COPY();
	
	bool unpack();  //read
	bool pack();  //write

	;
public:
	unsigned short seq;
	unsigned short type;

	unsigned int uin;
};

//#define CMD_SRV_CONF_LOGOFF					0x0028	//  40 确认某人离线
//服务器确认用户已经离线
class SRV_CONF_LOGOFF:public CPackInfo
{
public:
	SRV_CONF_LOGOFF(unsigned short ver);
	~SRV_CONF_LOGOFF();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;            //服务器返回离线序号  
	unsigned short type;
	unsigned int uin;				//离线用户的编号
};

//#define CMD_SRV_SERVER_BUSY					0x003C	//  60 服务器忙
class SRV_SERVER_BUSY:public CPackInfo
{
public:
	SRV_SERVER_BUSY(unsigned short ver);
	~SRV_SERVER_BUSY();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;            //服务器序号
};

//#define CMD_SRV_CONF_REG					0x0046	//  70 确认注册新用户
//服务器确认注册包
//max size : 614
class SRV_CONF_REG:public CPackInfo
{ 
public:
	SRV_CONF_REG(unsigned short ver);
	~SRV_CONF_REG();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;					//2
	unsigned short  type_loginby;		//2 0  UID; 1 string

	unsigned int uin;					//4 寻呼号
	char strid[MAX_SID_LEN];			//60
	char pwd[MAX_PASSWORD_LEN];			//20 密码

	char strMemo[MAX_RESPONSE_LEN];		//255 描述
	char strAddress[MAX_URL_LEN];		//255 下载地址

	unsigned int videoip;				//4
	unsigned short videoport;			//2
	//14+80+520 = 614
}; 

//#define CMD_SRV_DENY_REG					0x0050	//  80 拒绝注册新用户
//服务器拒绝注册新用户
//max size : 516
class SRV_DENY_REG:public CPackInfo
{ 
public:
	SRV_DENY_REG(unsigned short ver);
	~SRV_DENY_REG();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;					//2
	unsigned short  type_loginby;		//2  0  UID; 1 string

	unsigned short deny;				//2  拒绝的类型 0  字符串登录号存在    5 版本太旧
	char reason[MAX_RESPONSE_LEN];		//255  拒绝的类型描述

	char strAddress[MAX_URL_LEN];		//255 下载地址
}; 

//#define CMD_SRV_CONF_LOGON					0x005A	//  90 确认用户登录
//服务器确认用户登录
//max size : 997 
class SRV_CONF_LOGON:public CPackInfo
{
public:
	SRV_CONF_LOGON(unsigned short ver);
	~SRV_CONF_LOGON();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short	seq;						//2  登录序号	
	unsigned char	type_loginby;				//1  0  UID; 1 string 2, phone, 3 mail, 50 web
	//是否第一次登陆，如果第一次登陆，需要发送离线联系人列表
	unsigned char	type_loginflag;				//1  0  正常登陆，1 第一次登陆

	//断线登录成功后， 就发登录成功包，上线朋友包
	unsigned short type_loginsort;				//2  登录类别  0  normal;  1  offline

	unsigned int	uin;						//4  用户UIN
	char			strid[MAX_SID_LEN];		//60  字符类型的的登录
	char			stridmobile[MAX_PHONE_LEN];	//20  字符类型的的登录
	char			stridmail[MAX_EMAIL_LEN];	//255 字符类型的的登录
	char			stridbak[MAX_SID_LEN];	//60  字符类型的的登录

	unsigned short status ;						//2  状态
	char strStatus[MAX_STATUS_LEN];				//40  状态描述

	char strMemo[MAX_RESPONSE_LEN];				//255 描述
	char strAddress[MAX_URL_LEN];				//255 下载地址

	//视频中转的地址
	unsigned int videoip;						//4
	unsigned short videoport;					//2

	//考虑到多个服务器，该用户的登陆的服务器地址
	unsigned int ip;							//4
	unsigned short port;						//2

	//服务器是TraCQ还是OAM，两者在登陆过程的处理不一样。
	unsigned char servertype;					//1
	unsigned char sublogon;						//1

	//如果发送登陆后需要发送的包信息很多，提示用户通过tcp来获取
	unsigned int tcpip;						//4
	unsigned short tcpport;						//2

	//90+255+60+42+510+12 = 345 + 105 + 522 = 445 + 552 = 997
};

//#define CMD_SRV_DENY_LOGON					0x0064	// 100 拒绝用户登录
//服务器拒绝登录
//max size : 582
class SRV_DENY_LOGON:public CPackInfo
{
public:
	SRV_DENY_LOGON(unsigned short ver);
	~SRV_DENY_LOGON();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;						//2

	unsigned short type_loginby;			//2 0  UID; 1 string
	unsigned short type_loginsort;			//2 登录类别  0  normal;  1  offline

	unsigned int	uin;					//4 用户UIN
	char 			strid[MAX_SID_LEN];	//60 字符类型的的登录

	unsigned short deny;					//2 拒绝登录的原因：0: 用户已经在线；1：UIN号码不存在； 2：密码错误  3: 服务器到最大  4:隐身登陆失败  5:其它类型 10 版本太旧  100 同一机器上申请的tq号码已经达到3个 不能再申请了
	char reason[MAX_RESPONSE_LEN];			//255

	char strAddress[MAX_URL_LEN];			//255下载地址
	//72+510 = 582
};

//#define CMD_SRV_STATUS_USER					0x01A4	// 130 用户在线状态
class SRV_STATUS_USER:public CPackInfo
{
public:
	SRV_STATUS_USER(unsigned short ver);
	~SRV_STATUS_USER();

	bool unpack();  //read
	bool pack();  //write
	;

	int GetStatus();
	void SetStatus(int status);

	int GetConnectType();
	void SetConnectType(int type);

	int GetClientType();
	void SetClientType(int type);

	int GetVideoType();
	void SetVideo(int type);
public:
	unsigned  int	uin;                  //用户的UIN

	char			nickname[MAX_USERNAME_LEN];						//用户网络名
	unsigned  int	ip;                   //用户的现在IP    
	unsigned  short port;                 //用户的现在PORT

	unsigned short ver;
	unsigned short langtype;

	unsigned int status;               //用户的现在状态
	//0-3 在线状态  
	//4-5 接入方式
	//6-7 web/客户端用户

	char	strstatus[MAX_STATUS_LEN];			//在线状态的文字

	unsigned short	externid;					//2 外联公司编号
	unsigned short	bak;		//
};

//#define CMD_SRV_SEARCH_SUCC					0x008C	// 140 查询朋友结果
//size : 930
class SRV_SEARCH_SUCC:public CPackInfo
{
public:	
	SRV_SEARCH_SUCC(unsigned short ver);
	~SRV_SEARCH_SUCC();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short	        seq;						//2	
	unsigned int			onlineusernum;				//4 online user num
	unsigned int			onlineclientnum;			//4 online client num
	unsigned short          isnext;						//2 1 is the next 

	unsigned short          num;						//2
	FRD_QUERY_INFO			ListInfo[MAX_LIST_QUERYFRD];//76*12

	unsigned int			nextpos;					//4

public:
	bool AddInfo(FRD_QUERY_INFO Info);
	void ClearInfo();

	unsigned short GetMaxNum()
	{
		return maxnum;
	}
private:
	unsigned short maxnum;

	//size : 18+912 = 930
};

//更新用户信息, 更新密码, 更新状态的服务器结果：成功，失败
//更新成功
//#define CMD_SRV_UPDATE_SUCC					0x00C8	// 200 更新成功
class SRV_UPDATE_SUCC:public CPackInfo
{
public:
	SRV_UPDATE_SUCC(unsigned short ver);
	~SRV_UPDATE_SUCC();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;			//2
	unsigned short type;		//2

	//更新的状态
	unsigned int commstatus;	//4
	unsigned int extstatus;	//4
	unsigned int onlinestatus;	//4

	char strstatus[MAX_STATUS_LEN];	//120  //在线状态的文字描述,新的密码等

	unsigned int uin;			//4
};

//离线消息
//#define CMD_SRV_SEND_MSG_OFFLINE		0x00DC	//220
class SRV_OFFLINE_MSG : public CPackInfo
{
public:
	SRV_OFFLINE_MSG(unsigned short ver);
	~SRV_OFFLINE_MSG();

	bool unpack();
	bool pack();
public:
	unsigned short	MsgNum;          //消息数目
	MSG_INFO MsgList[MAX_OFFLINE_MSG];         //消息列表

};

//服务器返回的用户资料
//max size : 974
class SRV_REP_USERINFO:public CPackInfo
{
public:
	SRV_REP_USERINFO(unsigned short ver);
	~SRV_REP_USERINFO();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short          seq;			//2 返回查询号
	unsigned short			nUpdateType;	//2
	unsigned int			uin;			//4 更新的用户号

	unsigned int			ip;				//4
	unsigned short			port;			//2
	
	CUserInfo				UserInfo;		//934	用户信息
	CTradeInfo				TradeInfo;		//0			

	char strStatus[MAX_STATUS_LEN];			//20自定义状态的文字描述

	unsigned short cltVer;					//2 客户端版本号
	unsigned short terminaltype;			//2 终端类型 0：pc客户端，1 手机客户端 2 本客户端
	char strFaceLink[MAX_FACELINK_LEN];

	unsigned int onlinetime;	//上线时间
	char strid[MAX_SID_LEN];	// strid
};

//#define CMD_SRV_REP_SMS						0x0122	// 290 返回短信收到的信息
// 290 返回短信收到的信息
class SRV_REP_SMS:public CPackInfo
{

public:
	SRV_REP_SMS(unsigned short version);
	~SRV_REP_SMS();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short	seq;

	char	mobile[MAX_PHONE_LEN];
	char	sms[MAX_SMS_LEN];

	unsigned int sendtime;
	unsigned int recvtime;
};

//#define CMD_SRV_REP_TRANSFERCLIENT			0x01B8  // 440 转移临时用户成功失败
class SRV_REP_TRANSFERCLIENT:public CPackInfo
{
public:
	SRV_REP_TRANSFERCLIENT(unsigned short ver);
	~SRV_REP_TRANSFERCLIENT();

	bool unpack();  //read
	bool pack();  //write
	;

public:
	unsigned short seq;

	NAMEID_INFO	recvinfo;
	NAMEID_INFO clientinfo;
	NAMEID_INFO	sendinfo;

	//0 success, 1 fail
	unsigned short result;

	//when result = 1
	unsigned short deny;
	//error memo
	char reason[MAX_RESPONSE_LEN];

	char   szChatId[MAX_CHATID_LEN+1];  //会话id
	char   szRand[MAX_WEBCLIENID_LEN+1];//访客唯一标示
	char   szThirdid[MAX_THIRDID_LEN+1];//第三方id

};

//#define CMD_SRV_TRANSFERCLIENT				0x01C2  // 450 通知接收者，转移临时用户
class SRV_TRANSFERCLIENT:public CPackInfo
{
public:
	SRV_TRANSFERCLIENT(unsigned short version);
	~SRV_TRANSFERCLIENT();

	bool unpack();  //read
	bool pack();  //write


public:
	unsigned short seq;

	NAMEID_INFO	recvinfo;
	NAMEID_INFO clientinfo;
	NAMEID_INFO	sendinfo;

	char   szChatId[MAX_CHATID_LEN+1];  //会话id
	char   szRand[MAX_WEBCLIENID_LEN+1];//访客唯一标示
	char   szThirdid[MAX_THIRDID_LEN+1];//第三方id
	
};

struct  GROUP_LIST
{
	unsigned char id;//组id
	char groupname[MAX_GROUPNAME_LEN+1]; //组名
};

//#define CMD_SRV_GROUP_LIST					0x02C6 	//  朋友分组列表
class SRV_LIST_FRIENDGROUP:public CPackInfo
{
public:
	SRV_LIST_FRIENDGROUP(unsigned short ver);

	bool unpack();  //read
public:
	unsigned short seq;

	unsigned short all;
	unsigned short index;

	unsigned short	num;                                        //包里面的用户数目
	unsigned short cur;	//当前包号

	GROUP_LIST ListInfo[MAX_LIST_FRDGROUP];		//40*MAX_LIST_FRD

public:
	unsigned short GetMaxNum()
	{
		return maxnum;
	}

private:
	unsigned short maxnum;

};

//#define CMD_SRV_LIST_FRD					0x0212	// 530 所有朋友列表
class SRV_LIST_FRD:public CPackInfo
{
public:
	SRV_LIST_FRD(unsigned short ver);
	~SRV_LIST_FRD();

	bool unpack();  //read
	bool pack();  //write
	;
public:
	unsigned short seq;

	unsigned short all;
	unsigned short index;

	unsigned short	num;                                        //包里面的用户数目
	unsigned short cur;	//当前包号

	FRD_COMMON_INFO ListInfo[MAX_LIST_FRD];		//40*MAX_LIST_FRD

public:
	bool AddInfo(FRD_COMMON_INFO Info);
	void ClearInfo();
	unsigned short GetMaxNum()
	{
		return maxnum;
	}

private:
	unsigned short maxnum;

};

//#define CMD_SRV_LIST_ONLINEFRD				0x0226  // 550 在线朋友列表
//服务器返回在线用户列表
//对非自定义状态，没一个包有50个用户信息， 
//对自定义状态，没一个包有25个用户信息， 
//服务器首先处理非自定义的包，然后处理自定义的包。
//max size : 6 + 50 * 20 = 1006
class SRV_LIST_ONLINEFRD:public CPackInfo
{
public:
	SRV_LIST_ONLINEFRD(unsigned short ver);
	~SRV_LIST_ONLINEFRD();

	bool unpack();  //read
	bool pack();  //write

public:
	unsigned short seq;

	unsigned short all;
	unsigned short index;

	unsigned short	num;                                        //包里面的用户数目

	unsigned short cur;

	FRD_ONLINE_INFO ListInfo[MAX_LIST_ONLINEFRD];		//40*MAX_LIST_ONLINEFRD

public:
	bool AddInfo(FRD_ONLINE_INFO Info);
//	bool AddInfo(USER_TABLE_PT pInfo);
	void ClearInfo();

	unsigned short GetMaxNum()
	{
		return maxnum;
	}
private:
	unsigned short maxnum;
};


#endif