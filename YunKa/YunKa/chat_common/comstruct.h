// client common struct : 服务器使用的一些公用的结构和类
//
//////////////////////////////////////////////////////////////////////
#ifndef _CCOMMSTRUCT_EMS
#define _CCOMMSTRUCT_EMS

#include "sockbase.h"
#include "../stdafx.h"
#include "comdef.h"
#include "comenum.h"
#include <map>
using namespace std ;

class CWebUserObject;
class CUserObject;

typedef struct IDBUFF_INFO{
	unsigned int id;
	char *pBuff;
}IDBUFF_INFO_T, *IDBUFF_INFO_PT;

//有关初始化设置的一些设置项
typedef struct INIT_CONF
{
	//用于帮助菜单中的内容
	char sProgrameNameMini[256];		// 定义的程序简称
	char sHelpUrl[256];					// 跳转的URL
	char sCopyID[256];					// 版本
	char sHelpAbout[4][256];			// 关于对话框中的文字
	char sHelpAboutURL[4][256];			// 关于对话框中的文字文字跳转
	int	 nDisplayUserNameType;			// 用户的显示信息
	char sVisitorServer[256];
	int nVisitorServerPort;
	char sDefaultServer[256];			// 登陆的默认服务器
	int nDefaultPort;					// 登陆的默认端口
	char sListServer[10][256];			// 保存十个服务器的列表
	int bSavePass;						// 保存密码
	int bAutoLogin;						// 自动登陆
	int	visitorfilter;
	char infofilter[256];				// 以下格式为 : 0, 北京, 天津
	char sAuthAddr[256];
	int  nAuthPort;
	char webpage_lgout[256];			// 退出
	char webpage_webphone[256];
	char webpage_querywebphone[256];	// 查询来电
	char webpage_note[256];				// 留言
	char webpage_crm[256];				// 客户管理
	char webpage_analyze[256];			// 统计分析
	char webpage_mgmt[256];				// 管理中心
	char webpage_news[256];				// 焦点窗口
	char webpage_regnewuser[256];		// 注册新用户
	int webpage_regnewuser_w;
	int webpage_regnewuser_h;	
	char webpage_tqhelp[256];			// 新手指南	
	char webpage_UniIdioms[256];		// 统一用语	
	char webpage_RtmMsg[256];			// 实时监控	
	char webpage_SvrMsg[256];			// 漫游消息
	char webpage_TQadviser[256];		// TQ顾问	
	char webpage_UploadHeadImage[256];	// 上传头像
	char webpage_DownloadHeadImage[256];// 下载头像
	char webpage_FaceImage[256];		// 表情
	char webpage_SysMessage[256];		// 系统消息
	char webpage_complaint[256];		// 投诉建议
	char webpage_SendWebFileLink[256];	
	char webpage_companyuser[256];		// 获取公司用户
	char webpage_iconurl[256];
	char webpage_faqInfo[256];
	char webpage_faqSort[256];
	char webpage_faqall[256];
	char webpage_sendurl[256];
	char webpage_repickchaturl[256];	// 重启会话
	char webpage_evaluate[256];			// 邀请评价
	char webpage_accesstoken[256];		// 获取微信token
	char webpage_workbillurl[256];		// 工单
	char visitorpage_visitortail[256];	// 客户履历
	char visitorpage_visitorbill[256];	// 会话登单
	char visitorpage_visitorinfo[256];	// 客户信息
	char visitorpage_notice[256];		// 通告信息
	char visitorpage_visitororder[256];	// 下订单
	char registerAccount[256];			// 登陆窗口的 注册 忘记密码[12/24/2010 SC]
	char forgetPassword[256];
	int forgetPSVisible;
	bool bZlibEnabled;					// 和visit服务器通信，客户端是否启用压缩
	char query_history_records[256];	// 查询历史记录
	char wechat_media_upload[256];		// 微信服务器媒体文件上传url
	char wechat_static_map[256];		// 腾讯地图静态图url
	char wechat_map_location[256];		// 腾讯地图地点标注url
	char fileserver_media_download[256];// 文件服务器下载的微信服务器的媒体文件的url
	char fileserver_media_upload[256];	// 文件服务器上传url
	char fileserver_media_task[256];	// 向文件服务器获取文件id的url串
	char fileserver_media_fileid[256];	// 文件上传到微信服务器后，其在文件服务器上相同文件的url串
}INIT_CONF_T, *INIT_CONF_PT;

//一个过滤器结构
struct FILTER_USERDEFINE
{
	int num;//过滤字符串数目
	int include;//包含该文字还是不包含该文字
	char text[MAX_STRINGFILTER_NUM][MAX_STRINGFILTER_LEN];//文字内容, 最多10个,每一个最大长度50
};

//基本信息
struct LOGIN_INFO
{	
	unsigned int uid;	
	char sid[MAX_SID_LEN+1];			//60
	char pass[MAX_PASSWORD_LEN+1];		//20
	bool bAutoLogin;
	bool bKeepPwd;
};

//提醒方式结构
struct ALERT_INFO
{
	int type;
	int tray;
	int showwnd;
	int sound;
	char soundfilename[MAX_FILENAME_LEN];
};

//用户字典组的结构
typedef struct KEYWORDGROUP_INFO{
	unsigned int id;
	unsigned char type;
	unsigned int sort;
	
	unsigned int compid;
	unsigned int userid;
	
	unsigned int parentid;
	
	char name[MAX_USERNAME_LEN+1];
	char	invalid;

	unsigned int level;
	
	//以下信息不打包，程序用来处理
	char	solve;	//程序用来处理
}KEYWORDGROUP_INFO_T, *KEYWORDGROUP_INFO_PT;

//用户字典的结构
typedef struct KEYWORD_INFO{
	unsigned int id;
	unsigned char type;
	unsigned int sort;
	unsigned int groupid;
	unsigned int compid;
	unsigned int userid;
	
	char name[MAX_USERNAME_LEN + 2];
	char memo[MAX_8192_LEN + 2];
	
	char	invalid;
	int 	hotkey;
	bool	alt, shift, ctrl;
	char	code;
	
	unsigned int level;

	//以下信息不打包，程序用来处理
	char	solve;	//程序用来处理
}KEYWORD_INFO_T, *KEYWORD_INFO_PT;

//代理的信息
//size : 42
struct PROXY_INFO
{
	unsigned int ip;						//4
	unsigned short port;					//2
	unsigned short proxyport;				//2
	char strUser[MAX_USERNAME_LEN];			//20
	char strPass[MAX_PASSWORD_LEN];			//20
};

//有关用户的消息信息
//max size : 670
struct USER_INFO
{
	//基本信息
	unsigned int uid;	
	unsigned char usertype;
	char sid[MAX_SID_LEN+1];						//60
	char pass[MAX_PASSWORD_LEN+1];					//20
	unsigned int	compid;
	char			nickname[MAX_USERNAME_LEN+1];	//名称					20
	char			username[MAX_USERNAME_LEN+1];	//真实名字					20(40)
	char	 		mobile[MAX_PHONE_LEN+1];		//手机						20(230)
	char			phone[MAX_PHONE_LEN+1];			//电话						20(252)
	char			email[MAX_URL_LEN+1];			//电子邮件					120(580)
	unsigned char	roleid;							//权限角色
	unsigned int	comauth;						//常用权限(用位表示)		4(658)
	unsigned int	extauth;						//常用类别和状态(用位表示)		4(662)
	unsigned int	regdate;						//注册日期时间，用int数据表示，从1970年开始的秒数
	unsigned char	areaadminid;					//如果为公司客服，该值表示公司对应的区域编号
	unsigned short	langtype;						//个人信息的语言版本				2(668)
	unsigned short	bak;							//									2(670)		
	unsigned int	onlineflag;						//备用字段						4(912)
};

//有关用户的商务信息，兼容旧版本
struct TRADE_INFO
{
	char 			company[MAX_COMPANY_LEN];				//公司名称   40
	char 			leader[MAX_USERNAME_LEN];				//法人		 未用(20)
	unsigned short	duty;									//职务名称   2
	unsigned short	size;									//公司规模   2
	unsigned short	trade;									//所处行业   2
	char 			bankcode[MAX_BANK_LEN];					//开户银行	40		   
	char 			bank_num[MAX_BANKNO_LEN];				//银行账号	30
	char 		    tax_num[MAX_TAXNO_LEN];					//税务代号	30
	char 			mobile[MAX_PHONE_LEN];					//电话		未用
	char 			phone[MAX_PHONE_LEN];					//电话		20
	char 			fax[MAX_PHONE_LEN];						//传真		20
	char 			homepage[MAX_URL_LEN];					//主页名称  60
	char 			mailbox[MAX_EMAIL_LEN];					//			60
	char 			bak1[MAX_VERIFYMSG_LEN];				//			40
	unsigned short	langtype;								//个人信息的语言版本
	unsigned short 	companylangtype;						//			4
	char 			memo[MAX_TRADEMEMO_LEN];				//公司简介  500
};

struct WEBONLINE_INFO
{
	WEBONLINE_INFO(){ bInvited = false; }
	int				fd;						// tcp连接
	TALKSTATUS		talkstatus;
	bool		    bInvited;				// 被邀请进来的
	unsigned int	timevisit;				// 用户访问时间
	unsigned int	timerequest;			// 用户请求对话时间
	unsigned int	timetalkstart;			// 用户对话开始时间
	unsigned int	timetalkend;			// 用户对话结束时间
	unsigned int	timeleave;				// 用户离开页面时间，为0一般表示处于连接状态，>0表示socket已断开，正处于新的连接阶段
	int				evalation;				// 本次对话评价
	unsigned int	random;					// 本次上线后的随机数	
	unsigned int	trafficid;				// 保存在数据库中的最新的id
	unsigned char	hasmsg;					// 有消息保存在缓冲中。
	unsigned char	isonline;				// 是否在线
	unsigned int	visitortalkid;			// 保存在请求信息的表中的编号
	unsigned char	searchkeywordsnum[4];	// 识别的热门关键词的数目
	unsigned char	alertit;				// 该用户需要报警提示， 在多长分钟之内，连续访问了多次
};

//访客的基本信息， 这个是唯一的
struct WEBUSER_INFO
{
	WEBUSER_INFO()
	{
		memset(this,0,sizeof(WEBUSER_INFO));
	}
	unsigned int	uid;							//这个编号没有意义了，为固定的SYS_WEBUSER = 9999
	unsigned int	compid;							//公司编号
	unsigned int	siteid;
	char			sid[MAX_WEBCLIENID_LEN+1];		//这个为访客唯一标示
	char			thirdid[MAX_THIRDID_LEN+1];		//第三方id
	char			name[MAX_USERNAME_LEN+1];
	char			nameflag;						//记录名字是否被修改 1 已修改
	unsigned char	status;							// 0 TALKSTATUS_NO, 1 STATUS_OFFLINE 离线， 2 在线， 9 对话中
	char			sip[MAX_IPNUM_LEN+1];
	char			ipfromname[MAX_USERNAME_LEN+1];	//ip来源
	char			url[MAX_URL_LEN+1];				//来源的url
	char			title[MAX_URL_LEN+1];
	char			chatfrom[MAX_CHATID_LEN + 1];
};

struct WEBUSER_UPINFO
{
	char iecopyright[50];
	char systeminfo[50];
	char language[50];
	int visioncolor;
	char visionsize[20];
	unsigned int adminid;
	char visiturl[MAX_URL_LEN];
	char source[50];
	char scriptflag[MAX_SCRIPTFLAG_LEN];
	char sip[20];
	int port;
	char sid[20];
	char lastvisit[MAX_URL_LEN];
	char webtitle[50];
	char nickname[50];
	int ctimes, ttimes;
	char lastvtime[MAX_URL_LEN];
	char lastttime[MAX_URL_LEN];
	unsigned int lastuin;
	int isautoinvit;
};

//访客的用户输入信息
struct WEBUSEREX_INFO
{
	char		  sid[MAX_SID_LEN + 1];	//这个为访客唯一标示
	char	name[MAX_USERNAME_LEN + 1];		//和webuser_info一样，
	unsigned char	age;	//0 unknow
	unsigned char	sex;	//0 unknow
	char	area[MAX_AREA_LEN];
	char	phone[MAX_PHONE_LEN];
	char	email[MAX_EMAIL_LEN];	//email
	char	im[MAX_128_LEN];	//msn/qq
	char	address[MAX_128_LEN];
	char	zipcode[MAX_ZIPCODE_LEN];
	char	companyname[MAX_128_LEN];
	char	companyurl[MAX_128_LEN];
	char	keywords[MAX_128_LEN];
	unsigned char	iconindex1;
	unsigned char	iconindex2;
	unsigned char	iconindex3;
	unsigned char	iconindex4;
	unsigned int comauth;
	unsigned int extauth;
	unsigned int createtime;
};

typedef struct TCP_PACK_HEADER
{
	unsigned short   len;			//包的大小
}TCP_PACK_HEADER_T,*TCP_PACK_HEADER_PT;

typedef struct PACK_HEADER
{
	unsigned short		ver;			//协议版本
	unsigned short		sendrandom;		//发送方随机数 在floatchat中记录第几条消息
    unsigned int		uin;			//用户的编号
    unsigned short		random;			//随机数   ,在floatchat中记录gpid
	unsigned short		cmd;			//包的命令
    unsigned short		seq;			//包的序号
	unsigned short		langtype;		//包的语言类型(0 ansi,1 utf8)
	unsigned short		seqresp;		//回应的包的序号
	unsigned short		terminal;		//MULTI_TERMINAL_ONLINE = 4221之后表示终端类型1 
}PACK_HEADER_T,*PACK_HEADER_PT;

struct LISTCHATINFO
{
	char			chatid[MAX_CHATID_LEN+1];
	unsigned int	uKefuUin;	  //客服的号码,0:等待应答状态，>0:正在和客服通信
};

// 发送媒体文件时，接收对象的信息传参
struct SEND_FILE_PARAMS
{
	unsigned long userId;	// 接收消息用户的id
	int userType;			// 用户类型
};

// 重新发送媒体文件时，接收对象的信息传参
struct RESEND_FILE_PARAMS
{
	unsigned long userId;		// 接收消息的用户id
	string filaPath;			// 发送的文件路径
	int userType;				// 接收文件的用户类型
	string msgId;				// 消息的序列号
	int msgDataType;			// 消息的数据类型
};

// 重新接收媒体文件时，接收对象的信息传参
struct RERECV_FILE_PARAMS
{
	string filaPath;			// 文件的存储路径
	string url;					// 接收文件的url
	int userType;				// 发送消息的用户类型
	string msgId;				// 该消息的序列号
	int msgDataType;			// 消息的数据类型
	unsigned long userId;		// 发送消息的用户id
	unsigned long webUserId;	// ??该结构体可优化
	unsigned long groupUserId;
};

// 双击头像,切换聊天对象时，切换对象的信息传参
struct CHANGE_CHAT_OBJECT_PARAMS
{
	unsigned long userId;	// 接收消息用户的id
	int userType;			// 用户类型
};

// 一条消息
struct ONE_MSG_INFO
{
	string msgId;	// 消息id
	string msg;	// 消息内容
};

// 重新发送消息，包括文字消息，媒体文件的json串消息
struct RESEND_MSG_PARAMS
{
	string msgId;			// 消息id
	unsigned long userId;	// 接收消息用户的id
	int userType;			// 用户类型
	string mediaId;			// 媒体文件在微信服务器的id,消息类型是文字的时候，该参数代表文字消息内容
	int msgDataType;		// 消息数据类型
	string fileId;			// 媒体文件在文件服务器的id
	string filePath;		// 文件在本地存储路径
};

struct WEBUSER_URL_INFO
{
	string url;				// 访问地址
	unsigned long dwtime;	// 访问时间
};

struct UPLOAD_INFO
{
	void* pThis;				// 传该结构体参数的类对象指针
	string filePath;			// 上传文件路径
	unsigned long userId;		// 消息id
	MSG_DATA_TYPE msgDataType;	// 消息的数据类型
	USER_TYPE userType;			// 消息接收用户类型
};

struct DOWNLOAD_INFO
{
	void* pThis;				// 传该结构体参数的类对象指针
	string filePath;			// 下载文件的路径
	string downLoadUrl;			// 下载文件的url
	CWebUserObject *pWebUser;	// 消息的来源用户
	string time;				// 发送时间
	CUserObject* pUser;			// 协助对象,可为空
	MSG_DATA_TYPE msgDataType;  // 消息的数据类型
};

typedef struct {
	const string raw, enc;
} wx_face_t;

static const string wx_face_null = string();

static const wx_face_t wx_faces[] = {
	{ "/::)", wx_face_null },	//微笑
	{ "/::~", wx_face_null },	//撇嘴
	{ "/::B", wx_face_null },	//色，美女
	{ "/::|", wx_face_null },	//发呆
	{ "/:8-)", wx_face_null },	//墨镜
	{ "/::<", "/::&lt;" },		//流泪
	{ "/::$", wx_face_null },	//害羞
	{ "/::X", wx_face_null },	//闭嘴，哑
	{ "/::Z", wx_face_null },	//睡觉
	{ "/::'(", wx_face_null },	//哭
	{ "/::-|", wx_face_null },	//尴尬，囧
	{ "/::@", wx_face_null },	//愤怒
	{ "/::P", wx_face_null },	//调皮
	{ "/::D", wx_face_null },	//呲牙笑
	{ "/::O", wx_face_null },	//惊讶

	{ "/::(", wx_face_null },	//难过
	{ "/::+", wx_face_null },	//酷
	{ "/:--b", wx_face_null },	//冷汗
	{ "/::Q", wx_face_null },	//抓狂
	{ "/::T", wx_face_null },	//吐
	{ "/:,@P", wx_face_null },	//偷笑
	{ "/:,@-D", wx_face_null },	//愉快
	{ "/::d", wx_face_null },	//白眼
	{ "/:,@o", wx_face_null },	//骄傲
	{ "/::g", wx_face_null },	//饿
	{ "/:|-)", wx_face_null },	//困
	{ "/::!", wx_face_null },	//惊恐
	{ "/::L", wx_face_null },	//流汗
	{ "/::>", "/::&gt;" },		//憨笑
	{ "/::,@", wx_face_null },	//悠闲

	{ "/:,@f", wx_face_null },	//奋斗
	{ "/::-S", wx_face_null },	//咒骂
	{ "/:?", wx_face_null },	//疑问
	{ "/:,@x", wx_face_null },	//嘘，秘密
	{ "/:,@@", wx_face_null },	//晕
	{ "/::8", wx_face_null },	//疯了
	{ "/:,@!", wx_face_null },	//衰
	{ "/:!!!", wx_face_null },	//骷髅
	{ "/:xx", wx_face_null },	//敲打
	{ "/:bye", wx_face_null },	//再见
	{ "/:wipe", wx_face_null },	//擦汗
	{ "/:dig", wx_face_null },	//抠鼻
	{ "/:handclap", wx_face_null },//鼓掌
	{ "/:&-(", wx_face_null },	//糗大了
	{ "/:B-)", wx_face_null },	//坏笑

	{ "/:<@", "/:&lt;@" },		//左哼哼
	{ "/:@>", "/:@&gt;" },		//右哼哼
	{ "/::-O", wx_face_null },	//哈欠
	{ "/:>-|", "/:&gt;-|" },	//鄙视
	{ "/:P-(", wx_face_null },	//委屈
	{ "/::'|", wx_face_null },	//快哭了
	{ "/:X-)", wx_face_null },	//阴险
	{ "/::*", wx_face_null },	//亲
	{ "/:@x", wx_face_null },	//吓
	{ "/:8*", wx_face_null },	//可怜
	{ "/:pd", wx_face_null },	//菜刀
	{ "/:<W>", "/:&lt;W&gt;" },	//西瓜
	{ "/:beer", wx_face_null },	//啤酒
	{ "/:basketb", wx_face_null },//篮球
	{ "/:oo", wx_face_null },	//乒乓

	{ "/:coffee", wx_face_null },//咖啡
	{ "/:eat", wx_face_null },	//米饭
	{ "/:pig", wx_face_null },	//猪头
	{ "/:rose", wx_face_null },	//鲜花
	{ "/:fade", wx_face_null },	//凋谢
	{ "/:showlove", wx_face_null },//唇
	{ "/:heart", wx_face_null },//爱心
	{ "/:break", wx_face_null },//心碎
	{ "/:cake", wx_face_null },	//蛋糕
	{ "/:li", wx_face_null },	//闪电
	{ "/:bome", wx_face_null },	//炸弹
	{ "/:kn", wx_face_null },	//刀
	{ "/:footb", wx_face_null },//足球
	{ "/:ladybug", wx_face_null },//瓢虫
	{ "/:shit", wx_face_null },	//便便

	{ "/:moon", wx_face_null },	//月亮
	{ "/:sun", wx_face_null },	//太阳
	{ "/:gift", wx_face_null },	//礼物
	{ "/:hug", wx_face_null },	//拥抱
	{ "/:strong", wx_face_null },//赞
	{ "/:weak", wx_face_null },	//弱
	{ "/:share", wx_face_null },//握手
	{ "/:v", wx_face_null },	//胜利
	{ "/:@)", wx_face_null },	//抱拳
	{ "/:jj", wx_face_null },	//勾引
	{ "/:@@", wx_face_null },	//拳头
	{ "/:bad", wx_face_null },	//差劲
	{ "/:lvu", wx_face_null },	//爱你
	{ "/:no", wx_face_null },	//NO
	{ "/:ok", wx_face_null },	//OK

	{ "/:love", wx_face_null },	//爱情
	{ "/:<L>", "/:&lt;L&gt;" },	//飞吻
	{ "/:jump", wx_face_null },	//跳
	{ "/:shake", wx_face_null },//发抖
	{ "/:<O>", "/:&lt;O&gt;" },	//怄火
	{ "/:circle", wx_face_null },//转圈
	{ "/:kotow", wx_face_null },//磕头
	{ "/:turn", wx_face_null },	//回头
	{ "/:skip", wx_face_null },	//跳绳
	{ "/:&>", "/:&&gt;" },	//太极
	{ "/:#-0", wx_face_null },	//激动
	{ "/:hiphot", wx_face_null },//舞
	{ "/:kiss", wx_face_null },	//献吻
	{ "/:<&", "/:&lt;&" },	//瑜伽
	{ "/:oY", wx_face_null },	//投降
};


#endif