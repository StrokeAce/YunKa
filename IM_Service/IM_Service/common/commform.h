// commit pack commform.h: 数据传输中公用包的头文件
//	本类主要兼容以前版本的命令
//////////////////////////////////////////////////////////////////////
#if !defined _COMMFORM_EM
#define _COMMFORM_EM

//#define CMD_SRV_STATUS_FRDONLINE			0x006E	// 110 通知我我的朋友上线
//服务器告我，我的朋友在线
//CMD_SRV_INFORM_FRD_ONLINE
class SRV_STATUS_FRDONLINE:public CPackInfo
{
public:
	SRV_STATUS_FRDONLINE(unsigned short ver);
	~SRV_STATUS_FRDONLINE();

	bool unpack();  //read
	bool pack();  //write
	

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

	TCHAR			nickname[MAX_USERNAME_LEN];						//用户网络名
	unsigned  int	ip;                   //用户的现在IP    
	unsigned  short port;                 //用户的现在PORT

	unsigned short ver;
	unsigned short langtype;

	unsigned int status;               //用户的现在状态
	//0-3 在线状态  
	//4-5 接入方式
	//6-7 web/客户端用户

	TCHAR	strstatus[MAX_STATUS_LEN+1];			//在线状态的文字

	unsigned short	sterminaltype;					//2 外联公司编号

	unsigned short connecttype; //连接类型 服务器版本>=3070支持
	unsigned short mpid;        //用户所在mpid  服务器版本>=3070支持


	TCHAR   szRand[MAX_WEBCLIENID_LEN+1];  //rand或者微信用户id  mpver>=3073
	TCHAR   szThirdId[MAX_THIRDID_LEN+1];  //第三方id或者微信公众号id
};

//#define CMD_SRV_STATUS_FRDOFFLINE			0x0078	// 120 通知我我的朋友离线
//服务器告我， 我的朋友离线
class SRV_STATUS_FRDOFFLINE:public CPackInfo
{
public:
	SRV_STATUS_FRDOFFLINE(unsigned short ver);
	~SRV_STATUS_FRDOFFLINE();

	bool unpack();  //read
	bool pack();  //write
	

public:

	unsigned  int	uin;     //离线用户的UIN号码

	unsigned short	externid;					//2 外联公司编号
	unsigned short	bak;
};

//#define CMD_SRV_SEARCH_FAIL					0x0096	// 150 查询朋友失败
class SRV_SEARCH_FAIL:public CPackInfo
{
public:	
	SRV_SEARCH_FAIL(unsigned short ver);
	~SRV_SEARCH_FAIL();

	bool unpack();  //read
	bool pack();  //write
	

public:
	unsigned short		seq;   //返回查询号

	unsigned short      type;             //0 user is not exit
	TCHAR				reason[MAX_RESPONSE_LEN];		//255  拒绝的类型描述
};

//#define CMD_SRV_UPDATE_FAIL					0x00D2	// 210 更新失败
class SRV_UPDATE_FAIL:public CPackInfo
{
public:
	SRV_UPDATE_FAIL(unsigned short ver);
	~SRV_UPDATE_FAIL();

	bool unpack();  //read
	bool pack();  //write
	

public:
	unsigned short	seq;
	unsigned short	type;

	unsigned int	uin;

	unsigned short	deny;
	TCHAR			reason[MAX_RESPONSE_LEN];
};

//#define CMD_SRV_STATUS_USER_FORM					0x01A4	// 420 用户在线状态
class SRV_STATUS_USER_FORM:public CPackInfo
{
public:
	SRV_STATUS_USER_FORM(unsigned short version);
	~SRV_STATUS_USER_FORM();

	int GetOnlineStatus();
	int GetConnectType();
	int GetUserType();

	bool unpack();  //read
	bool pack();  //write
	

public:
	unsigned short	type;	//状态类型备用
    unsigned int	uin;			//放回那个用户的状态
	
private:
	unsigned int	status;			//状态
	/*
	参见USER_INFO 的onlineflag
		0 - 1   用户接入类型(4)：		0： 直接接入 1： 局域网接入； 2：代理接入
		4 - 5  用户类别：				0： 是注册用户； 1 注册客户;  2：临时用户
		8 - 12  用户状态(16)				0 用户定义状态；1：离线；2：在线；3：出去一下：
									4：请不要打扰
		  16      是否视频关闭/打开		0 没有：   1 打开
	*/
public:
	TCHAR strstatus[MAX_STATUS_LEN];  //在线状态文字

	unsigned int	ip;	//待加，如果为上线， 加地址
	unsigned short	port;		//待加，如果为上线， 加端口
};

/*
//old
//#define CMD_SRV_FINISH_CLTLOGON				0x0244	// 580 用户登录发送的包结束
class SRV_FINISH_CLTLOGON:public CPackInfo
{
public:
	SRV_FINISH_CLTLOGON(unsigned short version);
	~SRV_FINISH_CLTLOGON();

	bool unpack();  //read
	bool pack();  //write
	

public:
	unsigned short	seq; 
	unsigned int	ip;		//自己的ip地址
	unsigned short	port;
};
*/

//客户端的包的处理
//old
//#define CMD_CLT_SEND_ALLMSG					0x03F2  //  1010 发送群呼消息
//发送群呼留言
class CLT_SEND_ALLMSG:public CPackInfo
{
public:
	CLT_SEND_ALLMSG(unsigned short ver);
	~CLT_SEND_ALLMSG();

	bool unpack();  //read
	bool pack();  //write

public:
    unsigned short			num;			//接收者的UIN的个数
	unsigned int listuin[MAX_LIST_UIN];	//接收者的UIN

	//消息内部的uin无效
	MSG_INFO	msg;

//	unsigned short langtype; 
//	unsigned short bak; 

private:
	unsigned maxnum;
};

//#define CMD_CLT_GET_CLT_INFO				0x0460  // 1120 获得个人资料
//用户查询个人资料
//CMD_CLT_GET_CLT_INFO
class CLT_GET_CLT_INFO:public CPackInfo
{
public:
	CLT_GET_CLT_INFO(unsigned short ver);
	~CLT_GET_CLT_INFO();

	bool unpack();  //read
	bool pack();  //write

public:
	unsigned short	seq;             //查询号
	unsigned int   uin;                 //要查询的人的UIN号码
	unsigned short  type; 
};


//#define CMD_CLT_UPDATE_STATUS				0x04D8  // 1240 更新状态
//更新在线状态
class CLT_UPDATE_STATUS:public CPackInfo
{
public:
	CLT_UPDATE_STATUS(unsigned short ver);
	~CLT_UPDATE_STATUS();

	bool unpack();  //read
	bool pack();  //write

	
public:
	unsigned short seq;
	unsigned short type ; 
	/* 类型
			0	在线状态
			2   视频状态
			10  全部标志位信息
			11	commstatus
			12	extstatus
			13	onlinestatus
	*/

	unsigned int commstatus;
	unsigned int extstatus;
	unsigned int onlinestatus;

	TCHAR strstatus[MAX_STATUS_LEN];  //在线状态的文字描述
 };

//#define CMD_CLT_UPDATE_INFO					0x0500  // 1280 更新用户资料
//更新用户资料
class CLT_UPDATE_INFO:public CPackInfo
{
public:
	CLT_UPDATE_INFO(unsigned short ver);
	~CLT_UPDATE_INFO();

	bool unpack();  //read
	bool pack();  //write

	

public:
	unsigned short seq;

	unsigned short type;//0  用户信息  1  商务信息  2全部

	CUserInfo userinfo;                     //要更新的用户信息
	CTradeInfo	tradeinfo;
};

 //#define CMD_CLT_UPDATE_IMAGE				0x050A  // 1290 更新个人图片

//#define CMD_CLT_UPDATE_PW					0x0514  // 1300 更新密码
//更新密码
class CLT_UPDATE_PW:public CPackInfo
{
public:
	CLT_UPDATE_PW(unsigned short ver);
	~CLT_UPDATE_PW();

	bool unpack();  //read
	bool pack();  //write

	

public:

	unsigned short seq;
	TCHAR	pw[MAX_PASSWORD_LEN];
};

//#define CMD_CLT_UPDATE_LOGINSTRING			0x051E  // 1310 更新登录的名称字符串
//更新登陆的名称
class CLT_UPDATE_LOGINSTRING:public CPackInfo
{
public:
	CLT_UPDATE_LOGINSTRING(unsigned short ver);
	~CLT_UPDATE_LOGINSTRING();

	bool unpack();  //read
	bool pack();  //write

	

public:
	unsigned short seq;
	TCHAR	strid[MAX_SID_LEN];
};



//#define CMD_COM_FILE_SEND					0x0F8C  // 3980 客户端请求发送某一个文件
class COM_FILE_SEND:public CPackInfo
{
public:
	COM_FILE_SEND(unsigned short version);
	~COM_FILE_SEND();

	bool unpack();  //read
	bool pack();  //write

	
public:
	unsigned short	seq;						//2 序列号

	unsigned int	recvuin;					//4 他的UIN号码
	unsigned int	senduin;					//4 发送者的UIN号码
	TCHAR			sendname[MAX_USERNAME_LEN];	//20 发送者的名称

	unsigned short	sendtoserver;				//2
	unsigned int	random;						//4 区分唯一性

	TCHAR			strip[MAX_IPNUM_LEN];		//20 发送者的地址
	unsigned short	port;						//2

	unsigned int	localip;					//2
	unsigned short	localport;					//2 发送者的端口

	unsigned short	filetotal;					//2 文件的数目,目录为一个文件
	unsigned int	filesize;					//4

	unsigned short	filenum;					//2
	FILE_INFO		m_ListFile[MAX_LIST_FILE];	//5 * 125

	unsigned int	startpos;					//4 开始的位置, 0表示为第一次发送, 对个文件只对第一个有效
	unsigned short	isauth;						//2 是否加了验证  0  没有增加；1 增加
	char			strauth[MAX_RESPONSE_LEN];	//255 验证信息 

private:
	unsigned short	maxfilenum;

};

//视音频包使用,包一样格式不一样
//#define CMD_COM_VIDEO_FORM					0x109A  // 4250	 
//#define CMD_COM_AUDIO_FORM					0x10A4  // 4260	 
//#define CMD_COM_VIDEOTEXT_FORM				0x10AE  // 4270	
//#define CMD_COM_GETPORT						0x10B8  // 4280		在局域网接入时，处理端口
//#define CMD_COM_SENDPORT					0x10C2  // 4290		在局域网接入时，发送本地端口给其他用户
//#define CMD_COM_HANG						0x10CC  // 4300		挂断连接
class COM_VIDEOAUDIOTEXT_FORM:public CPackInfo
{
public :
	COM_VIDEOAUDIOTEXT_FORM(unsigned short version);
	~COM_VIDEOAUDIOTEXT_FORM();

	bool unpack();  //read
	bool pack();  //write

	
	void SetBuff(int len, TCHAR *pBuff);
public :
	unsigned int recvip[4];	//	recvip[0]   recvip[1] 服务器ip recvip[2] 对外ip recvip [3] 本地ip
	unsigned short recvport[4];	//

	unsigned short seq; //序号
	unsigned short num;	//该包的分包总数
	unsigned short index;		//该包的分包序号

	TCHAR sBuff[BUFFMAXLEN];		//数据

public:
	bool sendtoserver;
	int len;

	unsigned int uin;
	unsigned short port;

	TCHAR	strip[MAX_IPNUM_LEN];
};

#endif