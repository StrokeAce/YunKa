// client common struct : 服务器使用的一些公用的结构和类
//
//////////////////////////////////////////////////////////////////////
#ifndef _CCOMMSTRUCT_EMS
#define _CCOMMSTRUCT_EMS

#include "sockbase.h"
#include "../stdafx.h"
#include "comdef.h"
#include <map>
using namespace std ;

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

//对话接待信息
struct TALKTOTAL_INFO
{
	int m_nOnlineTimeTotal;	//总在线时间
	int m_ntalknum;			//对话次数
	int m_ntalktimetotal;	//总对话时间
	int m_ntalktimenum;		//有有效对话时间的对话数
	int m_nInviteNum;		//邀请次数
	int m_nacceptinvitenum;	//接受次数
	int m_nrefuseinvitenum;	//拒绝次数
	int m_nrequestnum;			//请求次数
	int m_nacceprequestnum;		//接待请求次数
	int m_nnotacceptrequestnum;	//没有接待请求次数
	int m_nnovisitormsgnum;		//访客没有发送消息
	int m_nnousermsgnum;		//客服没有发送消息
	int m_ngoodtalknum;		//一般对话
	int m_nbettertalknum;	//较好对话
	int m_nbesttalknum;		//极佳对话
};

//统计分析信息
typedef struct TOTAL_INFO{
	unsigned short	visittotalnum;
	unsigned short	fromsearchnum;	//从搜索引擎
	unsigned short	fromfriendnum;	//从友好连接
	unsigned short	frominputnum;	//直接输入
	unsigned short	searchurlnum;	//如大于MAX_LIST_TOTALURL， 则只保存MAX_LIST_TOTALURL个
	unsigned int	searchurltime[VISITORTALK_MAXLIST_TOTALURL];
	char			searchurl[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			searchurlhost[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			searchurlvar[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	unsigned short	friendurlnum;
	unsigned int	friendurltime[VISITORTALK_MAXLIST_TOTALURL];
	char			friendurl[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			friendurlhost[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			friendurlvar[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];

	//对话效果统计
	unsigned short	notalknum; //没有进行对话的访问次数
	unsigned short  invalidtalknum; //无效对话的次数 

	unsigned short  talknum0;	//一般对话次数 2  
	unsigned short  talknum1;	//较好对话的次数 0  
	unsigned short  talknum2;	//极佳对话的次数 0 

	//对话统计
	unsigned short	requesttalknum;	//累计主动请求对话的次数 1 包含接受自动邀请 
	unsigned short	sendmsgnum;		//客人累计发送讯息条数 3  
	unsigned short	persendmsgnum;	//平均每次对话发送讯息条数 1  
	unsigned short	sendmsgmaxnum;	//最大单次对话发送讯息条数 2  
	unsigned short  sendmsgminnum;	//客人最小单次对话发送讯息 0  
	unsigned short  sendmsgtalknum;		//累计有发送过讯息的对话次数 2 

	//邀请统计
	unsigned short	invitetalknum;	//累计接受客服手动邀请次数 1 不包含接受自动邀请 
	unsigned short	refusetalk;		//拒绝客服手动邀请的次数 0 不包含拒绝自动邀请 

	//访问统计
	unsigned short	visitnum;		//累计访问次数 2  
	unsigned short	visitpagenum;	//累计访问页数 6  
	unsigned short	pervisitpagenum;	//平均每次访问页数 3  
	unsigned short	visitpagemaxnum;	//最大单次访问页数 3  
	unsigned short	visitpageminnum;	//最小单次访问页数 3 

}TOTAL_INFO_T, *TOTAL_INFO_PT;

struct COMBOX_ITEMINFO { 
	string strText;
	DWORD dwData; 
};

//用户权限角色字段的结构
typedef struct ROLEAUTH_INFO{
	unsigned int id;
	unsigned int sort;
	
	char name[MAX_USERNAME_LEN];
	
	unsigned int comauth;
	unsigned int extauth;
	
	unsigned char invalid;
	unsigned char cbak;
	unsigned short usbak;
	
}ROLEAUTH_INFO_T, *ROLEAUTH_INFO_PT;

//分类结构
typedef struct CATEGORIES_INFO{
	unsigned int id;
	
	char name[MAX_USERNAME_LEN+1];
	char	invalid;
	
	//以下信息不打包，程序用来处理
	char	solve;	//程序用来处理
}CATEGORIES_INFO_T, *CATEGORIES_INFO_PT;

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
	//dest ip and port
	unsigned int ip;						//4
	unsigned short port;					//2
	
	unsigned short proxyport;				//2
	
	//auth for proxy server
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

//访客轨迹信息
struct WEBUSERTAIL_INFO
{
	unsigned int intime;					//4
	unsigned int outtime;					//4
	char url[MAX_URL_LEN];					//页面
	char title[MAX_URL_LEN];				//页面

	char prevurl[MAX_URL_LEN];
	char prevurlhost[MAX_URL_LEN];
	char prevurlvar[MAX_URL_LEN];
};

//浏览器的信息
struct WEBBROWSER_INFO
{
	char					useragent[MAX_URL_LEN];	//用户标头
	unsigned short			vcolor;					//客户端颜色
	unsigned short          vnum;					//访问次数
	unsigned short          vpagenum;				//访问页面数
	unsigned int			visittime;				//访问时间
	WEBUSERTAIL_INFO		tailinfo;				//浏览轨迹信息
	char					language[MAX_LANGUAGE_LEN];//访问的语言
	unsigned short			vzone;
	char					browsertype[MAX_BROWSER_LEN];	//浏览器类型
	char					ostype[MAX_BROWSER_LEN];		//操作系统
	char					vsize[MAX_VSIZE_LEN];			//屏幕浏览器大小

	//以下用来统计历史记录用
	char					sid[MAX_VISITORID_LEN];
	unsigned int			id;
	unsigned int			ip;
	char					ipfrom[MAX_USERNAME_LEN]; //到省一级的名字

	unsigned short			requestnum;	//请求数
	unsigned short			invitenum;	//邀请数
	unsigned short			userend;	//客服结束数
	unsigned short			clientend;	//客人结束数
	unsigned short			talknum;	//对话数
	char					solve;
};

//同一台电脑，访客可能会登陆多个同一个编号的用户
struct WEBONLINE_INFO
{
	WEBONLINE_INFO(){bInvited=false;}
	int				fd;		//tcp连接
	int				talkstatus;
	bool		    bInvited; //被邀请进来的

	unsigned int	timevisit;		//用户访问时间
	unsigned int	timerequest;	//用户请求对话时间
	unsigned int	timetalkstart;	//用户对话开始时间
	unsigned int	timetalkend;	//用户对话结束时间
	unsigned int	timeleave;		//用户离开页面时间，为0一般表示处于连接状态，>0表示socket已断开，正处于新的连接阶段
	int				evalation;		//本次对话评价

	unsigned int	random;			//本次上线后的随机数
	
	unsigned int	trafficid;		//保存在数据库中的最新的id

	unsigned char	hasmsg;			//有消息保存在缓冲中。
	unsigned char	isonline;		//是否在线

	unsigned int	visitortalkid;	//保存在请求信息的表中的编号

	unsigned char	searchkeywordsnum[4];	//识别的热门关键词的数目
	unsigned char	alertit;				//该用户需要报警提示， 在多长分钟之内，连续访问了多次
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
	char	sid[MAX_SID_LEN+1];	//这个为访客唯一标示
	char	name[MAX_USERNAME_LEN+1];		//和webuser_info一样，

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

//上传的文件信息
struct UPFILE_INFO
{
	char	filename[MAX_FILENAME_LEN];
	char	memo[MAX_256_LEN];
	char	downurl[MAX_FILENAME_LEN];

	unsigned int uptime;
	unsigned int senduid;
	char	sendname[MAX_USERNAME_LEN];
};

//消息的列表结构
//950
typedef  struct MSG_LISTINFO{
	unsigned char	msgtype;					//1 消息类别
	unsigned char	msgformat;					//1 消息格式
	unsigned short	usbak1;						//2
	
	unsigned int	senduid;					//4 接收者的uid号码
	char			sendname[MAX_USERNAME_LEN+2];
	
	unsigned int	trafficid;
	unsigned int	talkid;
	
	//如果recvuid=9999, visitorid表示为访客的唯一表示
	//不允许访客对访客发送消息,recvuid senduid不能同时为9999
	char			visitorid[MAX_VISITORID_LEN+2];	//26	
	
	unsigned int	sendtime;					//4 发送时间
	
	char			msg[MAX_MSG_LEN+2];			//910 消息内容
	
	unsigned short	langtype;					//2
	unsigned short	usbak2;						//2
	
}MSG_LISTINFO_T, *MSG_LISTINFO_PT;

//一次对话的汇总信息
typedef struct VISITORTALK_INFO{
	unsigned int 	id;
	unsigned int trafficid;
	
	//客人的编号和名称
	char sid[MAX_SID_LEN];
	char visitorname[MAX_USERNAME_LEN];
	
	//开始对话，结束对话的类型
	unsigned char	starttype;  //1 客人主动邀请，客服主动邀请 
	unsigned char	endtype; //1 客人主动关闭，客服主动关闭
	
	//接待的客服编号和名称
	unsigned int	talkuserid;
	char	talkusername[MAX_USERNAME_LEN];
	
	//评价
	unsigned char	evalulation;
	unsigned char	ucbak;
	
	unsigned int	visittime;
	unsigned int	requesttime;
	unsigned int	starttime;
	unsigned int	endtime;
	
	unsigned short	visitormsgnum;
	unsigned short	usermsgnum;
	
	//来源ip
	unsigned int	ip;
	char			ipfromname[MAX_USERNAME_LEN];			//ip来源
	char prevurlhost[MAX_URL_LEN];	//来源主机
	char prevurlvar[MAX_URL_LEN]; //搜索引擎的参数
	
	
	char			solve;
	
}VISITORTALK_INFO_T, *VISITORTALK_INFO_PT;

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

//文件共享使用
//20 + 24 + 130 = 174
typedef struct FileAttr {
	
	char			szFileTitle[MAX_FILENAME_LEN];		//12	8 文件的标题名
	unsigned int	dwFileAttributes;                   //4	文件的属性
	FILETIME		ftCreationTime;                     //8 文件的创建时间
	FILETIME		ftLastAccessTime;                   //8 文件的最后访问时间
	FILETIME		ftLastWriteTime;                    //8 文件的最后修改时间
	
    unsigned int	nFileSizeHigh;                      //4 文件大小的高位双字
	unsigned int	nFileSizeLow;                       //4 文件大小的低位双字
	unsigned int	dwReserved0;                        //4 保留，为0
	unsigned int	dwReserved1;                        //4 保留，为0
} FILEATTR, *PFILEATTR;

//共享文件的信息
//125
struct FILE_INFO
{
	char			strFile[MAX_FILENAME_LEN];	//256
	unsigned int	dwSize;						//4
	
	unsigned char	IsDirectory;				//1 是否为一个目录
};

//自动邀请设置
typedef struct AutoInviteAttr {
	unsigned int	siteid;
	
	unsigned char	autoinvite;
	unsigned short	invitenum;
	
	unsigned short	deltime[10];
}AUTOINVITE_INFO, *AUTOINVITE_INFO_PT;

//搜索关键词信息
typedef struct SEARCHKEYWORDS_INFO{
	unsigned int siteid;
	
	unsigned char keywordnum[4];
	char keywords[4][MAX_SEARCHKEYWORDS_NUM][MAX_SEARCHKEYWORDS_LEN];
	
	//报警条件
	unsigned char  alertopen;
	unsigned short alertsearchkeywordsnum;		//有这么多次的进入
	unsigned short alertsearchkeywordstimes;	//在该时间段内
	
	unsigned short usbak;
	
}SEARCHKEYWORDS_INFO_T, *SEARCHKEYWORDS_INFO_PT;

//float msg info
struct FLOAT_MSG_INFO
{
	//基本信息
	unsigned int	id;						//网页的编号
	unsigned int	compid;					//公司的编号
	unsigned int	uid;					//备用用户编号
	
	//连通前的提示语
	char			wellcome[MAX_MSG_LEN+2];	//连通前欢迎语
	char			connectedwellcome[MAX_MSG_LEN+2];	//联线自动回复
	
	//客人请求超时
	unsigned char	reqtimeout;			//超时时间，0为不启用		
	char			reqtimeoutmsg[MAX_MSG_LEN+2];
	
	//客人等待答复超时
	unsigned char	waitresptimeout;
	char			waitresptimeoutmsg[MAX_MSG_LEN+2];
	
	//客人答复超时
	unsigned char	resptimeout;
	char			resptimeoutmsg[MAX_MSG_LEN+2];
	
	//客人答复时关闭对话
	unsigned char	resptimeoutclose;
	char			resptimeoutclosemsg[MAX_MSG_LEN+2];
};

//float info
//size 300
struct FLOAT_BASIC_INFO
{
	//基本信息
	unsigned int	id;						//网页的编号
	unsigned int	compid;					//公司的编号
	
	char			url[MAX_URL_LEN];		//网站名称
	
	//0 漂浮框类型			0 对话框漂浮框 1 图片漂浮框
	//1 对话框打开位置		0 当前页面打开，1 新开对话窗口
	//2 是否有主动邀请		0 没有，1 有
	//3 是否可以关闭		0 不可以，1 可以
	//4 是否可以拖动		0 不可以，1 可以
	//5 - 6 邀请框类型      0 在中间显示询问框 1 在当前位置显示对话框
	unsigned int	comauth;
	
	unsigned int	extauth;
	
	//漂浮框位置类型为0, left top取值0 1 表示左/上 右/下对齐
	//漂浮框位置类型为1, left top分别为离边框距离，负值为右/底部的距离
	int				left;	
	int				top;
	
	//颜色，有默认16种颜色可以选择
	int				colorindex;
	
	//对话式漂浮框的logo文件, 标题
	char			logofile[MAX_FILENAME_LEN];
	char			title[MAX_TITLE_LEN];
	char			tail[MAX_TAIL_LEN];
	
	//工具条是否显示的表示，用字符串0111表示，顺序表示图标的位置，0表示显示，1表示不显示
	char			showtoolicon[20];
	
	//主动邀请的语言
	unsigned char	invitetype;	//类型
	char			invitetitle[MAX_URL_LEN];
	char			inviteword[MAX_MSG_LEN];
	char			invitebgfile[MAX_FILENAME_LEN];
	char			inviteacceptfile[MAX_FILENAME_LEN];
	char			inviterefusefile[MAX_FILENAME_LEN];
	unsigned int	inviteposx;	//高位字表示类型， 低位字表示距离
	unsigned int	inviteposy;
	
	//图片式漂浮框的图片文件
	char			icononlinefile[MAX_128_LEN];
	char			iconofflinefile[MAX_128_LEN];
	
	char			icononlinealt[MAX_128_LEN];
	char			iconofflinealt[MAX_128_LEN];
	
	char			iconurl[MAX_128_LEN];
	
	//页面语言编码
	char			language[50];
	
	unsigned char	initshow;			//初始显示格式
	
	//广告连接
	char			adicon[MAX_FILENAME_LEN+2];
	char			adurl[MAX_URL_LEN+2];
	
	AUTOINVITE_INFO autoinviteinfo;
	
	SEARCHKEYWORDS_INFO searchkeywordsinfo;
	
	FLOAT_MSG_INFO msginfo;	//开场白的设置信息
};

struct LISTCHATINFO
{
	
	char			chatid[MAX_CHATID_LEN+1];
	unsigned int uKefuUin;	  //客服的号码,0:等待应答状态，>0:正在和客服通信
	
};

//历史记录过滤条件
typedef struct VISITORTALK_FILTER
{
	//2个时间作为查询条件时不使用
	unsigned int starttime;
	unsigned int endtime;

	char talktype;
	char talkstarttype;
	char talkendtype;
	
	unsigned int firstuin;
	unsigned int transferuin;
	
	char iptype[256];
	char searchurl[256];
	
	char searchkeywords[256];
	char keywords;
	
	char friendurl[256];
	char talkurl[256];
	char language[256];
	
	char clientsort;	//客人类别
	char evaluation;	//客人评价
	
	char frominfo[256];
	char fromtail[256];
	char fromnameandip[256];
	char fromtalkmsg[256];
	char frommemo[256];
	
	DWORD dwData[10];
	char sbak[10][256];
}VISITORTALKFILTER;

typedef struct WORKBILLTB_ITEM
{
	char  chatid[30];			//--工单编号(对应tqdb03,WORKBILL中的id)

	int billtype;			//--工单类型（在线1~20，留言类用20~30，在线咨询:1,在线投诉:2,产品维护故障咨询3,员工咨询 4,客户表扬 5,留言投诉:21,留言建议:22,留言咨询:23）
	int billsubtype;

	unsigned int adminuin;	// --管理员号码
	unsigned int kefuuin;	// --提供服务的客服UIN
	unsigned int deptid;	// --部门id

	unsigned int clientuin;	// --访客会话uin
	char clientname[100];		// --客户姓名
	char clienttype[50];		// --客户品牌
	int  clientlevel;			// --客户级别
	char clientproduct[30];		// --客户产品
	char clientmobile[15];		// --客户手机
	char clientphone[20];		// --客户固定电话
	char clientemail[50];		// --客户邮箱
	int clientemailaccpet;		//  --用户email是否接收业务推荐信息，1接收，0 不接收
	char clientotherinfo[100];	// --其他联系，方式 可用作回拨申请的电话
	char clientid[40];			// --第三方id
		
	unsigned int talkstarttime;	// --开会会话时间
	unsigned int talkendtime;		// --结束会话时间
	unsigned int talktime;			// --工单时长
	int usermsgnum;			// --本次会话,客服的回复数(TALK_CONTENT中客服说话的次数)
	int visitorcluesnum;	// --本次会话线索数
	int billresult;			// --处理结果（未处理：0，已处理：1，转交他人：3）
	int evalution;			// --评分(对应ABCD四级别)
	int transferok;			// --转移是否成功(0.没有转移，1.不成功 2.成功)
	char transferpath[200];	// --转移路径(如80008115->8362009)
	char memo[1024];
		
	int validtalk;			// --会话无效,无效：1，有效：0
	int losttalk;			// --会话流失,流失：1，未流失：0
		
	unsigned int clientrand;	// --访客唯一标示
	unsigned int visitorip;	// --访客来源ip
	char visitorregion[100];	// --ip所属地域

	char visitorentry[50];		//--访客初始化来源(目前只有搜索引擎)
	char visitorkeywords[50];	// --搜索引擎中的查找关键字

	char visitorprevurl[200];	// --访客上个页面
	char visitorurl[200];		// --当前页面(产生工单的页面)
		
	char outworkbillid[50];		//50  --外部工单号
		
	unsigned int billadminuin;	// --工单负责人，默认为kefu_uin,会话转移时修改此字段
	int talkfrom;				// --会话来源(0 其它 1 服务端自动请求 2客户端自动请求 3客户端手动请求)
	int worktype; 
}WORKBILLTBITEM;

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
	string url;				// 接收文件的url
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