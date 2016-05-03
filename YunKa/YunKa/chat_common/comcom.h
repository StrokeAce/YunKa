#if !defined _COMMCOM_EM
#define _COMMCOM_EM

#include "comdef.h"
#include "comstruct.h"
#include "pack.h"
#include "comtran.h"

//通讯包的头HEADER
class COM_HEAD_PACK
{
public:
	COM_HEAD_PACK(unsigned short ver_t,
		unsigned short sendrandom,
		unsigned int uin_t, 
		unsigned short random_t,
		unsigned short cmd_t, 
		unsigned short seq_t, 
		unsigned short langtype_t,
		unsigned short seqresp_t,
		unsigned short flag);
	COM_HEAD_PACK();
	~COM_HEAD_PACK();

	static int GetHeadLen();
	bool unpack(CPack &pack);  //read
	bool pack(CPack &pack);  //write

	void SetValue(unsigned short ver_t,
		unsigned short sendrandom,
		unsigned int uin_t, 
		unsigned short random_t,
		unsigned short cmd_t, 
		unsigned short seq_t, 
		unsigned short langtype_t,
		unsigned short seqresp_t,
		unsigned short flag);
	void SetValue(PACK_HEADER head);

public:
	PACK_HEADER			head;

public:

};

//通讯包基类
class CPackInfo
{
public:
	CPackInfo();
	~CPackInfo();

	virtual bool unpack();  //read
	virtual bool pack();  //write

	virtual bool unpackhead();  //read
	virtual bool packhead();  //write

public:
	unsigned short version;
	bool bSendToServer;

	COM_HEAD_PACK	m_Head;	//包的头部
	CPack			m_Pack;	//包结构

};

//有关代理的设置
//max size : 48
class CProxyInfo
{
public:
	CProxyInfo();
	~CProxyInfo();

	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);  //write

public:
	PROXY_INFO	info;

};

//有关用户的消息信息
//max size : 914
class CUserInfo
{
public:
	CUserInfo();
	~CUserInfo();

private:
	//pack and unpack for versin less 4.0
	bool UnpackForm(CPack &pack);
	bool PackForm(CPack &pack);  //write

public:
	//pack and unpack
	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);  //write

	void ClearInfo();
	void CopyInfo(CUserInfo *pInfo);
public:
	USER_INFO	info;
};

//有关用户的商务信息，兼容旧版本
class CTradeInfo
{
public :
	CTradeInfo();
	~CTradeInfo();
	
	//pack and unpack
	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);  //write

	void ClearInfo();
	void CopyInfo(CTradeInfo *pInfo);


public:
	TRADE_INFO		info;
};

//通用朋友信息（不在线的朋友用一个包发送，里面包含用户的如下信息）
//max size : 40(new) 62(old) 
class FRD_COMMON_INFO
{
public:
	FRD_COMMON_INFO();
	~FRD_COMMON_INFO();

	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);

	bool ConvertGBToBIG5(bool bGBToBIG5);

	void ClearInfo();
	void CopyInfo(FRD_COMMON_INFO *pInfo);
public:
	unsigned int	uin;						//4 用户的UIN号

	char			mobile[MAX_PHONE_LEN];		//20 城市（新版未用）
	char			nickname[MAX_USERNAME_LEN]; //20 姓名

	unsigned int	deptid;						//4
	//externid =	0 同一公司的用户
	//				1 不同公司的用户
	//				>=1000 外联公司的用户，外联公司编号为大于等于1000，主服务器的编号为100
	unsigned short	externid;					//2 外联公司编号
	unsigned short	langtype;					//2

//	16 - 23 组的ID
	unsigned char	groupid;					//1
//	24 - 31 图像编号
	unsigned char	iconid;						//1

	unsigned char	sex;						//1
	unsigned char	bak;						//1	备用

	unsigned char cgroupid;						//用户所在的好友分组
};

//在线的朋友信息
//max size : 20(new) 40(new ex) 60(old) 
class FRD_ONLINE_INFO
{
public:
	FRD_ONLINE_INFO();
	~FRD_ONLINE_INFO();

	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);


	void ClearInfo();
	void CopyInfo(FRD_ONLINE_INFO *pInfo);

public:
	unsigned int	uin;						//4 UIN号码
	char			nickname[40];	//20 姓名(not use on new copy)

	unsigned int	ip;							//4 ip
	unsigned short	port;						//2 port	

	unsigned short	version;					//2 用户的版本
	unsigned short	langtype;					//2 

	unsigned int	onlinestatus;				//4 在线状态位
	char 			strstatus[30];	//20 在线状态描述
	char			strcompany[40];

	unsigned short terminaltype;
	
};

//查询到的朋友信息
//size : new 76 size
class FRD_QUERY_INFO
{
public:
	FRD_QUERY_INFO();
	~FRD_QUERY_INFO();

	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);


	void ClearInfo();
	void CopyInfo(FRD_QUERY_INFO *pInfo);
public:
	unsigned int	uin;						//4 UIN号码
	char			nickname[MAX_USERNAME_LEN];	//20 姓名

	char			email[MAX_EMAIL_LEN];		//255 EMAIL	not used

	//如果department == 0 则company保存公司名称和部门名称
	char			company[MAX_COMPANY_LEN];	//40 公司名称

	unsigned int	deptid;						//4 部门
	unsigned short	country;					//2 国家 not used
	unsigned short	province;					//2 省份 not used

	char			city[MAX_CITY_LEN];			//30 城市 not used
	unsigned int	commstatus;					//4 标志位 not used
	unsigned int	slavestatus;				//4  not used
	unsigned int	onlinestatus;				//4 在线状态位not used

//	unsigned char	sex;						//1 性别
	unsigned short	sex;						//1 性别
//	unsigned char	online;						//1 是否在线
	unsigned short	langtype;					//2 
	//size : 12 + 20 +40 + 4 = 76
};


//消息的通用结构
//950
class MSG_INFO
{
public:
	MSG_INFO();
	~MSG_INFO();

	bool unpack(CPack &pack, unsigned short ver);
	bool pack(CPack &pack, unsigned short ver);

public:
	unsigned short	seq;						//2

	unsigned short	msgtype;					//1 消息类别 0 普通消息  1短信
	unsigned int	recvuin;					//4 接收者的UIN号码
	unsigned int	senduin;					//4 发送者的UIN号码  
	unsigned int	sendtime;					//4 发送时间
	unsigned short	isread;						//2 是否被读过
	char			strmsg[MAX_MSG_RECVLEN+1];		//910 消息内容
	char			strmobile[MAX_MSGNICKNAME_LEN+1];	//66 手机号码, 发短消息使用，用户接受信息时用于姓名
	char			strfontinfo[MAX_MSGFONT_LEN+1]; //HTML,JSON=WX,CHSET=%u,H=%u,EF=%u,CR=%u,FN=%s

	unsigned short	langtype;					//2
	unsigned int	bak;						//4	公告消息的编号
	//40 + 910 = 950
};

//有关一个组、部门、用户的通用朋友信息
//max size : 32
class NAMEID_INFO
{
public:
	NAMEID_INFO();
	~NAMEID_INFO();

public:
	unsigned int	id;						//4 用户的UIN号

	char			name[MAX_USERNAME_LEN+1]; //20 姓名

};

//#define CMD_COM_ACK							0x000A	//  10 收到包后回应
class COM_ACK : public CPackInfo
{
protected:
	bool bClient;

public:
	COM_ACK(unsigned short version);
	COM_ACK();
	~COM_ACK();
	
	bool unpack();  //read
	bool pack();  //write

public:
	
};

//#define CMD_COM_ACKEX						0x000A	//  11 扩充的回应，服务器收到后，必须发送一个ack包给另一个用户。
class COM_ACKEX : public CPackInfo
{
protected:

public:
	COM_ACKEX(unsigned short version);
	COM_ACKEX();
	~COM_ACKEX();
	
	bool unpack();  //read
	bool pack();  //write

public:
	

public:
	unsigned short seq;		//对应的包的序号

	unsigned int uin;		//转发的编号
	unsigned int ip;		//要转发ack包到最终用户的地址
	unsigned short port;	//要转发ack包到最终用户的端口
};

union  ONLINEFLAGUNION
{

	unsigned int    online_flag;
	struct STATUS
	{
		enum PH_TYPE  //电话登录类型	
		{
			DIRECT = 0, // 直线
			GATEWAY,	// 网关
			SIP			// SIP
		};

		enum PH_STATUS  //电话状态
		{
			OFFLINE = 0, // 离线
			IDLE,		 // 空闲
			BUSY		 // 繁忙
		};

		unsigned int nConnecttype : 4;   //连接方式
		unsigned int ch_amt : 1;        //1:会话手动设置繁忙，0为自动繁忙

		unsigned int nbak : 3;
		unsigned int nOnlineStatus : 4;  //在线状态
		unsigned int nVideoStatus : 1;   //视频状态

		//ph_type 电话登录类型	
		unsigned int ph_type : 2;


		//ph_amt: 1为会话手动设置繁忙，0为自动繁忙
		unsigned int ph_amt : 1;

		// 电话状态
		unsigned int ph_status : 4;
		unsigned int nbak2 : 12;
	}stStatus;
};

class COM_SEND_PING : public CPackInfo
{
public:
	COM_SEND_PING(unsigned short ver);
	~COM_SEND_PING();
	bool unpack();
	bool pack();
public:
	unsigned short seq;

	unsigned short connecttype;//连接类型
	unsigned int localip;  //内网ip，http连接设置为0即可
	unsigned short localport;//内网端口
	unsigned int  onlinestatus; // 在线状态

	ONLINEFLAGUNION  o;
};

class COM_SEND_MULTIPING : public CPackInfo
{
public:
	COM_SEND_MULTIPING(unsigned short ver);
	~COM_SEND_MULTIPING();
	bool unpack();
	bool pack();
public:
	unsigned short seq;

	unsigned short	first;
	unsigned int uin;
	char nickname[MAX_USERNAME_LEN];

	unsigned int ip;
	unsigned short port;
};

//发送消息
class COM_SEND_MSG:public CPackInfo
{
public:
	COM_SEND_MSG(unsigned short version);
	~COM_SEND_MSG();

	bool unpack();  //read
	bool pack();  //write

	
public:
	MSG_INFO		msg;			//950;

	//如果sendip sendport为0，表示为离线消息
	unsigned int	sendip;			//4;
	unsigned short	sendport;		//2;

	//客户端版本大于6000
	char   strChatid[MAX_CHATID_LEN+1]; //会话编号  替换以前的sbak
	char	strRand[MAX_WEBCLIENID_LEN+1];  	//访客唯一标示 服务器版本>=3073支持
	char	strThirdid[MAX_THIRDID_LEN+1];	//第三方id 服务器版本>=3073支持

};

//max size : 295
class COM_ADD_FRD:public CPackInfo
{
public:
	COM_ADD_FRD(unsigned short version);
	~COM_ADD_FRD();

	bool unpack();  //read
	bool pack();  //write
	
	
public:
	unsigned short	seq;		//2
	unsigned int	ip;			//4 发送者的地址
	unsigned short	port;		//2	发送者的端口

	unsigned int	senduin;			//4 发送者的UIN号码
	char sendname[MAX_USERNAME_LEN];	//20 发送者的名称
	unsigned int	recvuin;				//4 他的UIN号码
	unsigned short	addtype;				//2 0：删除朋友, 1:增加朋友 , 2 delete add me
	unsigned char cgroupid;					//填加好友到分组的id
	//没有加验证，和加了验证但验证信息为空意义不一样，前者表示客户不需要验证，
	//服务器通知他有人加他为好友
    unsigned short	isauth;				//2 是否加了验证  0  没有增加；1 增加
    char	strauth[MAX_RESPONSE_LEN];	//255 验证信息 

	//max sze : 20 + 255 + 20 = 295
};

//用于联接的命令
class COM_P2PCONNECTNAT:public CPackInfo
{
public :
	COM_P2PCONNECTNAT(unsigned short version);
	~COM_P2PCONNECTNAT();

	bool unpack();  //read
	bool pack();  //write

public:
	unsigned short seq;
	unsigned short type1;			//send or resp
	unsigned short type2;			//msg video audio file... 

	unsigned int senduin;
	unsigned int sendip;
	unsigned short sendport;

	unsigned int recvuin;
	unsigned int recvip;
	unsigned short recvport;

	unsigned int sendlocalip;
	unsigned short sendlocalport;

	unsigned int recvlocalip;
	unsigned short recvlocalport;
};

class CLT_GET_AP_LIST:public CPackInfo
{
public:
	CLT_GET_AP_LIST(unsigned short ver);
	~CLT_GET_AP_LIST();

	bool unpack();  //read
	bool pack();  //write

	
public:
	unsigned short	seq;		//返回收到的包序号
	
};

//#define CMD_SRV_GROUP_LIST					0x02C6 	//获取登陆用户的好友分组组列表
class CLT_GET_FRIENDGROUP_LIST:public CPackInfo
{
public:
	CLT_GET_FRIENDGROUP_LIST()
	{
		m_Head.head.cmd=CMD_SRV_GROUP_LIST;
	};

	~CLT_GET_FRIENDGROUP_LIST();

	bool unpack(){return true;};  //read
	bool pack(){return true;};  //write

};

//#define CMD_SRV_LIST_FRD					0x0212 		//获取登陆用户的好友列表
class CLT_GET_FRIEND_LIST:public CPackInfo
{
public:
	CLT_GET_FRIEND_LIST()
	{
		m_Head.head.cmd=CMD_SRV_LIST_FRD;
	};

	~CLT_GET_FRIEND_LIST();

	bool unpack(){return true;};  //read
	bool pack(){return true;};  //write

};

//#define CMD_SRV_LIST_ONLINEFRD					 0x0226  //获取登陆用户的在线好友列表
class CLT_GET_ONLINEFRIEND_LIST:public CPackInfo
{
public:
	CLT_GET_ONLINEFRIEND_LIST()
	{
		m_Head.head.cmd=CMD_SRV_LIST_ONLINEFRD;
	};

	~CLT_GET_ONLINEFRIEND_LIST();

	bool unpack(){return true;};  //read
	bool pack(){return true;};  //write
};

//#define CMD_SRV_SEND_MSG_OFFLINE					 0x00DC   //获取登陆用户的离线留言
class CLT_GET_OFFLINEMSG:public CPackInfo
{
public:
	CLT_GET_OFFLINEMSG()
	{
		m_Head.head.cmd=CMD_SRV_SEND_MSG_OFFLINE;
	};

	~CLT_GET_OFFLINEMSG();

	bool unpack(){return true;};  //read
	bool pack(){return true;};  //write

};

#define MAX_CIRCLE_AP_NUM	30  //最大圈子AP个数
//#define CMD_COM_AP_LIST					0x8100		//获取以及返回AP服务器列表
class AP_LIST:public CPackInfo
{
public:
	AP_LIST(unsigned short ver);
	~AP_LIST();

	bool unpack();  //read
	bool pack();  //write

	
public:
	unsigned short	seq;		//返回收到的包序号
	unsigned int  ap_audiovideo_ip;  //语音视频主服务器
	unsigned short  ap_audiovideo_port;

	unsigned int  ap_file_ip;	//文件主服务器
	unsigned short  ap_file_port;


	unsigned short ap_circle_num; // 圈子服务器个数
	unsigned int ap_circle_ip[MAX_CIRCLE_AP_NUM];	// 圈子服务器i的ip     	
	unsigned short ap_circle_port[MAX_CIRCLE_AP_NUM];	//圈子服务器i的port
	unsigned int begin_id[MAX_CIRCLE_AP_NUM];		//该服务器处理的开始圈子编号
	unsigned int end_id[MAX_CIRCLE_AP_NUM];		//该服务器处理的最大圈子编号
private:
	unsigned short max_ap_circle_num;		//圈子服务器最大个数
						
		
	
};

//#define CMD_COM_SENDINVITE					0x2010	//8208	邀请
//#define CMD_COM_RESPINVITE					0x2020	//8224	回复
class COM_INVITE:public CPackInfo
{
public:
	COM_INVITE(unsigned short version, unsigned short cmd);
	//	COM_INVITE(unsigned short version, bool binvite = true);
	~COM_INVITE();
	
	bool unpack();  //read
	bool pack();  //write
	
public:
	//	unsigned int	inviteid;	//主动邀请的用户编号
	unsigned int	recvuid;	//在回复时，该信息为发送的用户，不是该用户收到后为通知消息
	unsigned int	senduid;
	unsigned int	compid;
	
	//	#define	INVITE_PTPTALK					0	//邀请对方对话
	//	#define	INVITE_MUTITALK					1	//邀请对方加入到对某一个访客的对话
	unsigned char	type;
	unsigned char	ucbak;
	unsigned short	usbak;
	
	//如果recvuid=9999, visitorid表示为访客的唯一表示
	//如果senduid recvuid都不为9999, visitorid也不为空，表示邀请用户加入到共同接待一个访客
	//不允许访客邀请访客发送消息,recvuid senduid不能同时为9999
	char			visitorid[MAX_VISITORID_LEN+2];	//26
	char			scritpflag[MAX_SCRIPTFLAG_LEN+2];	//26
	char			msg[MAX_MSG_LEN];//邀请附言
	char			msgex[MAX_MSG_LEN];//邀请附言
	
	unsigned char	type1;
	unsigned char	type2;
	
	unsigned char	error;		//0正确，> 0错误		
	unsigned char	response;	//0 收到包  1  同意  2 拒绝 3 超时
};

//#define CMD_EVENT_ANNOUNCEMENT:				0x7fff  //!事件通告命令字 djz 2010-3-24
class COM_EVENT_ANNOUNCEMENT:public CPackInfo
{
public:
	COM_EVENT_ANNOUNCEMENT(unsigned short version);
	~COM_EVENT_ANNOUNCEMENT();
	
	bool unpack();  //read
	bool pack();  //write
	
public:
	unsigned int myuin;
	unsigned short gpid;
	
	unsigned short type;

	char msgbuf[MAX_MSG_RECVLEN+1];
};

#endif