#if !defined _COMMCLT_EM
#define _COMMCLT_EM

//用户登录
//max size : 196
class CLT_LOGON : public CPackInfo
{
public:
	CLT_LOGON(unsigned short ver);
	virtual ~CLT_LOGON();


	bool unpack();  //read
	bool pack();  //write
 
	void SetOnlineStatus(int nStatus);

	int GetOnlineStatus();
	CLT_LOGON& operator =(CLT_LOGON& pLogon);

public:
	unsigned short	seq;					//2  登录序号			

	unsigned char	type_loginby;			//1  0  UID; 1 tstring 2, phone, 3 mail, 50 web
	//是否第一次登陆，如果第一次登陆，需要发送离线联系人列表
	unsigned char	type_loginflag;			//1  0  正常登陆，1 第一次登陆
	//断线登陆成功后， 就发登陆成功包，上线朋友包

	/*登录方式
	#define LOGIN_NORMAL			0  //正常登录
	#define LOGIN_OFFLINE			1  //离线后再登录
	#define LOGIN_SERVERDOWN		2  //服务启重启后自动登录
	#define LOGIN_OFFLINEAUTO		3  //离线后自动再登录
	#define LOGIN_LITE				10  //精简上线，上线时mp只发登陆成功、失败包，不发其他包（好友分组，好友列表，在线列表） 2010.1.12
	*/
	unsigned short  type_loginsort;	

	unsigned int 	uin;					//4  Tra号码登陆				4
	char			strid[MAX_SID_LEN];	//60 字符串登陆				60
	char			pwd[MAX_PASSWORD_LEN];  //20 用户密码 

/*	  0 - 4  online status
	    5    video status
      6 - 7  bak
*/
	unsigned short	status;					//2  在线状态 3 hide,  2 online
	char			strstatus[MAX_STATUS_LEN];	//40 状态

	unsigned short	connecttype;				//2  connect type :0 udp; 1 mutiping udp; 2 proxy
	CProxyInfo		proxyinfo;					//58

	unsigned short langtype;				//2
	unsigned short terminaltype;			//2  
	unsigned short bak;                     //

	char   szRand[MAX_WEBCLIENID_LEN+1];//访客唯一标示
	char   szThirdid[MAX_THIRDID_LEN+1];//第三方id

};

//#define CMD_CLT_REG							0x03FC  // 1020 用户注册
//max size :  1078
class CLT_REG : public CPackInfo
{
public:
	CLT_REG(unsigned short ver);
	~CLT_REG();
	
	bool unpack();  //read
	bool pack();  //write

	

public:
	unsigned short	seq;					//2 注册序列号

	unsigned short	type_loginby;			//2  0  UID; 1 tstring 2 mobile 3 mail, 如果为2，3登陆的字符串在user_info中
	char			strid[MAX_SID_LEN];	//60 字符串登陆
	char			pwd[MAX_PASSWORD_LEN];	//20 passowrd

	unsigned short	ConnectType;			//2  connect type :0 udp; 1 mutiping udp; 2 proxy
	CProxyInfo		ProxyInfo;				//58

	CUserInfo		UserInfo;				//934  用户信息
	CTradeInfo		TradeInfo;				//0    用户商务信息

};

//#define CMD_CLT_LOGOFF						0x0438  // 1080 用户离线
//用户离线发出的包
//CMD_CLT_LOGOFF
class CLT_LOGOFF:public CPackInfo
{
public:
	CLT_LOGOFF(unsigned short ver);
	~CLT_LOGOFF();

	bool unpack();  //read
	bool pack();  //write

public:
	unsigned short seq;              //离线序号，缺省为1

	unsigned int uin;						//离线用户编号.

	unsigned int online_flag;				//离线后设的在线标志位	
};

//#define CMD_CLT_SEARCH_FRD					0x041A  // 1050 用UIN查询
//通过UIN号码或登陆名, 邮件地址查朋友
//查询谁加我
class CLT_SEARCH_FRD:public CPackInfo
{
public:
	CLT_SEARCH_FRD(unsigned short ver);
	~CLT_SEARCH_FRD();

	bool unpack();  //read
	bool pack();  //write

	

public:
	unsigned short	seq;
	unsigned short	type;					//0  UID; 1 tstring  2 strmobile  3 strmail

	unsigned int	uin;

	char			strid[MAX_EMAIL_LEN];	//字符串登陆				60
};

//#define CMD_CLT_SEARCH_MUTIFRD				0x0424  // 1060 用信息查询
//通过基本信息查询用户（组合查询，里面可以有为空的)
class CLT_SEARCH_MUTIFRD:public CPackInfo
{
public:
	CLT_SEARCH_MUTIFRD(unsigned short ver);
	~CLT_SEARCH_MUTIFRD();

	bool unpack();  //read
	bool pack();  //write

	

public:
	unsigned short	seq;				//查询序号
	unsigned short  sort;				//查询分页

	char	nickname[MAX_USERNAME_LEN];	//用户名称
	char	company[MAX_COMPANY_LEN];	//公司名称

	unsigned int		deptid;		//部门
	unsigned short		country;		//国家 0表示全部	
	unsigned short		province;		//省份 0表示全部
	unsigned short		sex;			//性别 0 all, 1 man ,  2feman
	unsigned short		onlineuser;		//0:所有人，1：在线用户

	unsigned int		nextpos;		//继续查询的标志值

	//add at 8.23 phenix
	unsigned int agentID;
};

//#define CMD_CLT_GET_INFO					0x044C  // 1100 
//获得服务器的某一些信息资料(包括列表信息)
class CLT_GET_INFO:public CPackInfo
{
public:
	CLT_GET_INFO(unsigned short ver, unsigned short cmd);
	CLT_GET_INFO(unsigned short ver);
	~CLT_GET_INFO();

	bool unpack();  //read
	bool pack();  //write

	
public:
	unsigned short	seq;			//查询号
	unsigned short	cmd;			//返回的包的命令
	unsigned short	cmdtype;		//查询类型，用户、群组、公告、会议等,和cmd选一个
	unsigned short	type;			//每一个对象内部的分类

	unsigned int   id;				//要查询的对象的编号
	char strid[MAX_SID_LEN];		


};

//#define CMD_CLT_CHECKPROXY					0x0474  // 1140 检查代理服务器
class CLT_CHECKPROXY:public CPackInfo
{
public :
	CLT_CHECKPROXY(unsigned short ver);
	~CLT_CHECKPROXY();

	bool unpack();  //read
	bool pack();  //write

public:
	unsigned short seq;
	unsigned short connecttype; //connect type :0 udp; 1 mutiping udp; 2 proxy

	CProxyInfo proxyinfo;
};

//#define CMD_CLT_TRANSFERCLIENT				0x047E  // 1150 转移临时用户
class CLT_TRANSFERCLIENT:public CPackInfo
{
public :
	CLT_TRANSFERCLIENT(unsigned short ver);
	~CLT_TRANSFERCLIENT();

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

//#define CMD_CLT_GROUP_INVITE				0x05A0	// 1440 邀请你加入组,(0，2)；保留
//	CMD_COM_UPDATE_FRDLIST	0x2000
class CLT_UPDATE_FRDLIST:public CPackInfo
{
public:
	CLT_UPDATE_FRDLIST(unsigned short ver);
	~CLT_UPDATE_FRDLIST();

	bool unpack();
	bool pack();

public:
	unsigned short seq;
	unsigned int uin;		//

	unsigned short num;
	unsigned int IDList[MAX_UPDATE_FRDLIST_NUM];
	char NameList[MAX_UPDATE_FRDLIST_NUM][MAX_USERNAME_LEN];
	unsigned short sex[MAX_UPDATE_FRDLIST_NUM];
};

#endif