/**
为了和当前访客和客服的一对一会话（以下简称 S）进行区别，这种新的会话暂叫多人会话（以下简称 M）。
1.S会话过程有MP负责逻辑处理，M会话有GP负责逻辑处理
2.S会话根据用户号端散列到不同的MP；M会话有无控件服务器（webui）再见会话创建时随机选择一Gp来担当逻辑服务器，所有通信包必须在包头（PACK_HEADER）的nSessionId变量（偏移8，长度2字节）中保存gp的id信息，UI根据此信息进行散列。
  gpid设置为0xFFFF时，UI随机选择一gp将包转发过去，gpid设置为0xFFFE时，UI随机将包广播道所有的GP。
3.Gp收到通信包后，会在包头的nSequence变量（偏移12，长度2字节），设置本次会话中包的唯一序号，缓存包（如nSequence为0，标示是命令包，GP不缓存），并广播包到会话中的在线、且处于接受状态的所有用户（含发送者自身）。
4.会话不支持离线留言
5.由管理员号回chatid唯一确定一个对话
*/

#if !defined _COMMFLOAT_EM
#define _COMMFLOAT_EM

#include "comcom.h"
#include "comdef.h"
#include "comstruct.h"
#include "wx_obj.h"
#include <map>
#include <vector>

using namespace std;

class COM_FLOAT_CHATPACK:public CPackInfo
{
public:
	COM_FLOAT_CHATPACK(){}
	COM_FLOAT_CHATPACK(unsigned short cmd,unsigned short ver,unsigned short Gpid)
	{
		sGpid=Gpid;
		m_Head.head.cmd=cmd;
		m_Head.head.ver=VERSION;
	}

	bool packhead()
	{
		m_Head.head.random=sGpid;
		return m_Head.pack(m_Pack);
	}
protected:
	unsigned short sGpid;   //Gp服务器的id

};

/**
CMD_FLOAT_CREATECHAT=0x0230 //创建广播会话对象
如果GP创建会话成功，将此包广播道所有用户（不含发送者），失败返回发送者CMD_FLOAT_CMDERROR包
此包仅有webui服务器发送
*/
class COM_FLOAT_CREATECHAT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CREATECHAT(unsigned short ver,unsigned short sGpid);

	bool pack();  //write
	bool unpack();  //write

public:

	unsigned int uAdminId;	//管理员号码
	unsigned short usort;	//漂浮框id
	unsigned int uWebuin;	//访客号码
	char chatid[MAX_CHATID_LEN+1];     //工单id，唯一区别一次会话
	char clienttid[MAX_WEBCLIENID_LEN+1];
	char webname[MAX_USERNAME_LEN+1];
	char thirdid[MAX_THIRDID_LEN+1]; //第三方id
	unsigned int uKefu;			//当前客服号码
	unsigned int uFromAdmin; //会话来源网站管理员
	unsigned short uFromSort;//会话来信漂浮框id

	//对应广播人员列表
	map<unsigned int,char> memlist;

};

/**
从GP获取漂浮框在线的会话列表,sGpid应设置为0xFFFE
CMD_FLOAT_LISTCHAT=0x0231  //在线会话列表
gp根据获取uAdminId下所有发送者友好对象的会话列表，格式为COM_FLOAT_RECVLIST，失败返回CMD_FLOAT_CMDERROR

此包仅有客服发送
*/
class COM_FLOAT_GETLIST:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_GETLIST(unsigned short ver,
		unsigned short sGpid=0xFFFE   //默认都个广播道多个GP
		);

	bool pack();  //write

public:
	unsigned int uAdminId;	//管理员号码

};

/*
从GP接受到的漂浮框在线的会话列表信息，每个GP都会返回至少一个这个包
CMD_FLOAT_LISTCHAT=0x0231  在线会话列表
*/
class COM_FLOAT_RECVLIST:public COM_FLOAT_CHATPACK
{
public:
	bool unpack();  //read

public:
	unsigned int uAdminId;	//管理员号码

	vector<LISTCHATINFO>  recvchatlist;
};


//
/*
会话组详细信息包，请求时打包uAdminId、chatid ,GP回复为会话的详细信息
CMD_FLOAT_CHATINFO
*/
class COM_FLOAT_CHATINFO:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CHATINFO(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();
	
public:
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	
	//下面只用于解包
	unsigned short sSort;//漂浮框id
	unsigned int   tStartTime;		  //会话开始时间
	
	unsigned int uFromAmdind; //会话来源管理员号码
	unsigned short sFromSort; //会话来源漂浮框id
	
	unsigned int uWebUin;//web来宾的号码
	unsigned int uKefuUin;//客服的号码,0:等待应答状态，>0:正在和客服通信
	
	char		webnickname[MAX_USERNAME_LEN+1];//访客名称
	char		strClientId[MAX_WEBCLIENID_LEN+1];//访客唯一标示
	char		strThirdId[MAX_THIRDID_LEN+1];		//第三方id

	
	unsigned int uTansferingToAdmin;	//要转移到的目标管理员
	unsigned int uTansferingToKefu;		//要转移到的目标客服
	
	vector<unsigned int> webRecvUin;	  // web应答客服列表
	vector<unsigned int> InviteUin;		  // 邀请进入的客服列表
};

/*
CMD_FLOAT_CHATMSG=0x0232	发送会话消息
如果会话uAdminId chatid对话的会话存在，广播该包到会话中在线、有权查看会话内容的用户（不含发送者），gp发COM_FLOAT_CHATMSG_ACK给发送者，不存在返回CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_CHATMSG:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CHATMSG(unsigned short ver,unsigned short sGpid);

	bool pack();  //write
	bool unpack();  //read

public:
	unsigned int		uAdminId;						//管理员号码
	unsigned int		webuin;							//访客uin
	unsigned short		nMsgType;						//消息类型  2普通消息，3消息预知,4 客服正在输入
	unsigned short		nMsgDataType;					//消息数据类型  1 文字，2 图片，3 语音，4 视频
	unsigned int		tMsgTime;						//消息发送时间
	char				chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	char				nickname[MAX_USERNAME_LEN+1];	// 手机号码, 发短消息使用，用户接受信息时用于姓名
	char				strmsg[MAX_MSG_RECVLEN+1];		// 消息内容
	char				strfontinfo[MAX_MSGFONT_LEN+1]; //字体信息 HTML,JSON=WX,CHSET=%u,H=%u,EF=%u,CR=%u,FN=%s

	//客户端版本大于6000
	char				strRand[MAX_WEBCLIENID_LEN+1];  //访客唯一标示 服务器版本>=3073支持
	char				strThirdid[MAX_THIRDID_LEN+1];	//第三方id 服务器版本>=3073支持

	WxObj* wxMsg;
};

/*
客户端发送COM_FLOAT_CHATMSG消息后服务器给的确认包
CMD_FLOAT_CHATMSG_ACK=0x0239,	//GP对发生的CMD_FLOAT_CHATMSG进行ack
*/
class COM_FLOAT_CHATMSG_ACK:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CHATMSG_ACK(unsigned short ver,unsigned short sGpid);

	bool pack();  //write
	bool unpack();  //read

public:
	unsigned int uAdminId;	//管理员号码
	char chatid[MAX_CHATID_LEN+1];     //工单id，唯一区别一次会话
	unsigned int   webuin;  //访客uin
	unsigned short chatseq; //GP服务器为该条消息分配的id ,chatseq>=1;

};


/*
通知服务器开启和关闭消息广播
CMD_FLOAT_STARTORSTOP_MSG=0x0233,	客户端通知服务器广不广播消息到客户端
更改发送者在会话中的消息接受状态（只影响COM_FLOAT_CHATMSG包），成功回发此包给客服，不存在返回CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_STARTORSTOPRECV:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_STARTORSTOPRECV(unsigned short ver,unsigned short sGpid);

	bool pack();  //write

public:
	unsigned int uAdminId;				//管理员号码
	unsigned short sAcitonType;			// 0:接受消息，1停止接受消息
	unsigned short sLastMsgid;			//最后消息id
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话

};


//!访客关闭会话窗口，通知服务器关闭会话，客服也可利用该包来关闭和访客的通话会话,gp广播到所有人（含自身），会话不存在返回CMD_FLOAT_CMDERROR
//CMD_FLOAT_CLOSECHAT=0x0236,  会话终止包
class COM_FLOAT_CLOSECHAT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CLOSECHAT(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned short usType;			// 0,正常下线，1 webui 关闭会话 ,2客服关闭会话，3 GP关闭会话,10 客服推出邀请
	unsigned int   webuin;  //访客uin
	char nickname[MAX_USERNAME_LEN+1];  //发送者的名称
};


//!从GP获取某个会话中的部分或全部消息
//CMD_FLOAT_GETCHATMSG=0x0237,  获取会话消息，成功返回服务器缓存的各条消息到客户端，失败返回CMD_FLOAT_CMDERROR
class COM_FLOAT_GETCHATMSG:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_GETCHATMSG(unsigned short ver,unsigned short sGpid);
	bool pack();  //write

public:
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned short sBeginSeq;			//开始消息id
	unsigned short sEndSeq;				//结束消息id，服务器返回   大于sBeginSeq，且小于等于sEndSeq之间的所有消息，sEndSeq为0返回大于sBeginSeq的所有消息包

};


/*
会话转移
CMD_FLOAT_TRANSQUEST,  事件类型 0：客服接受会话  1：会话转移； 
如GP成处理会话转移，广播此包到所有人，负责返回发送者CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_TRANSREQUEST:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_TRANSREQUEST(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int uAdminId;					//管理员号码
	char chatid[MAX_CHATID_LEN+1];			//工单id，唯一区别一次会话
	unsigned int   uWebuin;				//访客号码

	unsigned short nTimeOutSecond;			//多少时间内没人接受，会话置为等待状态

	unsigned int   uToAdminId;				//接入管理员号码 0：站内转移，>0  跨站转移
	unsigned int   uToKefu;				//要接入的客服号码

	//接受方对应广播人员列表
	map<unsigned int,char> memlist;		//跨站转移时，如果tokefu为空需要提供应答客服列表
};

/*
会话转移超时取消
*/
class COM_FLOAT_TRANSFAILED:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_TRANSFAILED(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned int   uWebuin;             //访客号码
	unsigned int   uKefu;           //是谁发起的会话转移

	unsigned int   TransToAdmin;          //会话转移给谁
	unsigned int   TransToKefu;           //会话转移给谁
};


/*
本网站内部会话转移
CMD_FLOAT_ACCEPTCHAT   事件类型 0：客服接受会话  1：会话转移； 
如GP成处理会话转移，广播此包到所有人，负责返回发送者CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_ACCEPTCHAT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_ACCEPTCHAT(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  


public:
	unsigned int uFromAdminid,uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned int   uWebuin;             //访客号码
	char nickname[MAX_USERNAME_LEN+1];  //接受会话人的名称

};


#define  RECVCONTENT    1   //接受会话内容
#define  INSERTCONTEN   2   //插话权限
#define  ADMINRIGHT     3    //管理权限

//!获取的可以分享消息的对象列表,请求列表只需要发送包含该命令字的包头
struct SHAREUSEROBJ
{
	unsigned int	uin;
	unsigned char	cRight;//0:只能接受命令包，RECVCONTENT：接受会话内容,INSERTCONTEN：接受会话内容、插话，INSERTCONTEN接受会话内容、插话、管理
	unsigned int	onlineflag;
};

//CMD_FLOAT_SHARELIST   获取可分享消息对象列表（友好对象列表）,sGpid应设置为0xFFFEF。gp上有返回会话列表，没有返回的会话列表数为0
class COM_FLOAT_SHARELIST:public CPackInfo
{
public:
public:
	COM_FLOAT_SHARELIST(unsigned short ver);
	bool pack(){return true;};
	bool unpack();  
	
public:

	//对应广播人员列表
	vector<SHAREUSEROBJ> sharememlist;
};

/**
GP返回的错误
*/

class COM_FLOAT_ERROR:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_ERROR(unsigned short ver,unsigned short sGpid);

	bool pack(){return true;};
	bool unpack();  
public:
	unsigned short errcmd;  //错误的命令字
	unsigned short errornum;//错误代号
	unsigned int uAdminId;  //管理员号
	unsigned int   uWebuin;             //访客号码
	char chatid[MAX_CHATID_LEN+1];//chatid
};

/*
请求会话协助
GP收到此包后，不做任何处理，直接转发到uInviteUser，uInviteUser收此包后应提示用户是否接受邀请请求
*/
class COM_FLOAT_INVITEREQUEST:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_INVITEREQUEST(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int   uInviteUser;         //要邀请的好友的TQ号码
	unsigned short sSecond;              //有效秒数
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned int   uWebuin;             //访客号码
};

/*
被邀请人返回的请求会话协助的结果包
GP收到此包后，sResult为0，直接转发到uInviteFrom；sResult为1时将sender加入会话中邀请用户列表
*/
class COM_FLOAT_INVITERESULT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_INVITERESULT(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned short sResult;				//邀请应答  0：拒绝邀请，1：接受邀请
	unsigned int   uInviteFrom;         //发出邀请人的号码
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned int   uWebuin;             //访客号码


};

///////////////////////////客服应答超时///////////////////////////////////////////////
class CFloatChatRelease:public COM_FLOAT_CHATPACK
{
public:
	CFloatChatRelease(unsigned short ver, unsigned short sGpid);
	bool pack();  
	bool unpack();  
	
public:
	
	unsigned int uAdminId;				//管理员号码
	char chatid[MAX_CHATID_LEN+1];		//工单id，唯一区别一次会话
	unsigned int   webuin;             //访客号码
	
	unsigned int   uKefu;				//释放会话的客服号码
	char			szKefuName[MAX_USERNAME_LEN+1];			//应答超时的客服名称

	unsigned short usReason;           //原因  FloatChatTimeOutReason

};

#endif