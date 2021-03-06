// client common struct : 服务器使用的一些公用的结构和类
//
//////////////////////////////////////////////////////////////////////
#ifndef _CCOMMSTRUCT_EMS
#define _CCOMMSTRUCT_EMS

#include "../stdafx.h"
#include "comdef.h"
#include <map>
#include "tstring.h"
using namespace std ;

class CXPEdit; 
class CXPComboBox;
class CWnd;
class CBitmap;
class CRect;


typedef struct IDBUFF_INFO{
	unsigned int id;
	char *pBuff;
}IDBUFF_INFO_T, *IDBUFF_INFO_PT;

typedef struct UPFILETHREAD_STRUCT
{
	CWnd *pWnd;

	int userType;
	char strPathFile[256];
	char strFileName[256];
	char strDownURL[256];
	unsigned int recvuid;

	char visitorid[MAX_USERNAME_LEN];

	DWORD dwData;
}UPFILETHREADSTRUCT;


//有关初始化设置的一些设置项
typedef struct INIT_CONF
{
	//用于帮助菜单中的内容
//	char sProgrameName[256];			//定义的程序名称。用于"关于...."等
	char sProgrameNameMini[256];		//定义的程序简称
	char sHelpUrl[256];					//跳转的URL

	char sCopyID[256];					//版本
	char sHelpAbout[4][256];			//关于对话框中的文字
	char sHelpAboutURL[4][256];			//关于对话框中的文字文字跳转

	//0 name, 1 name(online/all), 2 name(online), 3 name(all)
//	int	 nDisplayGroupNameType;			//组户的显示信息
	//0 nickname, 1 nickname(uid), 2 nickname(sid), 3 nickname(uid:strid)
	int	 nDisplayUserNameType;			//用户的显示信息

	//访客接待
	char sVisitorServer[256];
	int nVisitorServerPort;
	
	char sDefaultServer[256];			//登陆的默认服务器
	int nDefaultPort;					//登陆的默认端口
	char sListServer[10][256];			//保存十个服务器的列表

	int bSavePass;						//保存密码
	int bAutoLogin;					//自动登陆

	int	 visitorfilter;
	//以下格式为 : 0, 北京, 天津
	//1 表示包含以下词汇
	//0 表示不包含以下词汇
	char infofilter[256];
	char tailfilter[256];
	
	char sAuthAddr[256];
	int  nAuthPort;
	
	// 退出
	char webpage_lgout[256];

	// laidian
	char webpage_webphone[256];
	// 查询来电
	char webpage_querywebphone[256];
	// 留言
	char webpage_note[256];
	// 客户管理
	char webpage_crm[256];
	// 统计分析
	char webpage_analyze[256];
	// 管理中心
	char webpage_mgmt[256];
	// 焦点窗口
	char webpage_news[256];
	// 注册新用户
	char webpage_regnewuser[256];
	int webpage_regnewuser_w;
	int webpage_regnewuser_h;
	
	// 新手指南
	char webpage_tqhelp[256];
	// 统一用语
	char webpage_UniIdioms[256];
	// 实时监控
	char webpage_RtmMsg[256];
	// 漫游消息
	char webpage_SvrMsg[256];
	
	// TQ顾问
	char webpage_TQadviser[256];
	// 上传头像
	char webpage_UploadHeadImage[256];
	// 下载头像
	char webpage_DownloadHeadImage[256];
	// 表情
	char webpage_FaceImage[256];
	// 系统消息
	char webpage_SysMessage[256];
	// 投诉建议
	char webpage_complaint[256];

	char webpage_SendWebFileLink[256];

	//获取公司用户
	char webpage_companyuser[256];

	char webpage_iconurl[256];

	char webpage_faqInfo[256];

	char webpage_faqSort[256];

	char webpage_faqall[256];

	char webpage_sendurl[256];

	char webpage_repickchaturl[256];//重启会话

	char webpage_evaluate[256];//邀请评价

	char webpage_accesstoken[256]; // 获取微信token
	char webpage_workbillurl[256]; //工单
	//客户履历
	char visitorpage_visitortail[256];
	//会话登单
	char visitorpage_visitorbill[256];
	//客户信息
	char visitorpage_visitorinfo[256];
	//通告信息
	char visitorpage_notice[256];
	//下订单
	char visitorpage_visitororder[256];

	// 登陆窗口的 注册 忘记密码[12/24/2010 SC]
	char registerAccount[256];
	char forgetPassword[256];
	int forgetPSVisible;

	bool bZlibEnabled; // 和visit服务器通信，客户端是否启用压缩

	char query_history_records[256]; // 查询历史记录
	char wechat_media_upload[256]; // 微信服务器媒体文件上传url
	char wechat_static_map[256]; // 腾讯地图静态图url
	char wechat_map_location[256]; // 腾讯地图地点标注url
	char fileserver_media_download[256]; // 文件服务器下载的微信服务器的媒体文件的url
	char fileserver_media_upload[256]; // 文件服务器上传url
	char fileserver_media_task[256]; // 向文件服务器获取文件id的url串
	char fileserver_media_fileid[256]; // 文件上传到微信服务器后，其在文件服务器上相同文件的url串

	//隐藏显示某一些组
//	bool bHideOfflineVisiterGroup;
}INIT_CONF_T, *INIT_CONF_PT;

//一个过滤器结构
struct FILTER_USERDEFINE
{
	//过滤字符串数目
	int num;

	//包含该文字还是不包含该文字
	int include;

	//文字内容, 最多10个,每一个最大长度50
	char text[MAX_STRINGFILTER_NUM][MAX_STRINGFILTER_LEN];
};

//区域的设置
typedef struct RECT_STRUCT{
	int			type;
	int			dockhorz;			//水平区域停靠
	int			dockvert;			//垂直区域停靠

	CBitmap		*m_pNormalBitmap;	//正常的地图
	CBitmap		*m_pDownBitmap;	//按下的地图
	CBitmap		*m_pFocusBitmap;	//焦点地图
	CBitmap		*m_pDisableBitmap;	//不能使用的地图
	CBitmap		*m_pSelectBitmap;	//选中的地图

	//Rect		m_SrcRt;		//源图的位置
	//Rect		m_DeskRt;		//绘制到地图的位置

	int			m_Stretch;		//是否会拉伸缩小的变量
	char 		strText[256];
	char		strTips[256];		//提示文本

	//-1表示用窗口统一色
	COLORREF	m_TextColor;		//文本的颜色
	COLORREF	m_TextFocusColor;		//文本的颜色
	COLORREF	m_TextDisableColor;		//文本的颜色

	HICON		hIcon, hFocusIcon, hDisableIcon;
	int			iconwidth,iconheight;
	int			trianglepos;

	bool		m_bTextColor;	//是否设置文本颜色
	bool		m_bVisible;
	bool		m_bEnable;
	bool		m_bRead;
	bool		m_bCheck, m_bIsCheck;

	int 		m_nPopupMenu;		//0 no 1 all  2 only tri
	bool		m_bDrawTriangle;

	//回发消息
	int			index;		//序号
	DWORD		WMMsg;
	WPARAM		wParam;
	LPARAM		lParam;

	CXPEdit		*m_pEdit;
	CXPComboBox		*m_pComboBox;
	bool		m_bEditFocus;
//	CEdit		*m_pEdit;
}RECT_STRUCT_T, *RECT_STRUCT_PT;

struct LOGIN_INFO
{
	//基本信息
	unsigned int uid;	
	
	char sid[MAX_SID_LEN+1];			//60
	char pass[MAX_PASSWORD_LEN+1];		//20

	char compid[MAX_STATUS_LEN+1];
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
	tstring strText;
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
	TCHAR strUser[MAX_USERNAME_LEN];			//20
	TCHAR strPass[MAX_PASSWORD_LEN];			//20
};

//有关用户的消息信息
//max size : 670
struct USER_INFO
{
	//基本信息
	unsigned int uid;	
	
	unsigned char usertype;

	char sid[MAX_SID_LEN+1];			//60
	char pass[MAX_PASSWORD_LEN+1];		//20

//	char compid[MAX_STATUS_LEN+1];
	unsigned int	compid;

	char			nickname[MAX_USERNAME_LEN+1];		//名称					20
	char			username[MAX_USERNAME_LEN+1];		//真实名字					20(40)

	char	 		mobile[MAX_PHONE_LEN+1];			//手机						20(230)
	char			phone[MAX_PHONE_LEN+1];			//电话						20(252)
	char			email[MAX_URL_LEN+1];				//电子邮件					120(580)

	unsigned char	roleid;		//权限角色
	unsigned int	comauth;						//常用权限(用位表示)		4(658)
	unsigned int	extauth;						//常用类别和状态(用位表示)		4(662)

	unsigned int	regdate; //注册日期时间，用int数据表示，从1970年开始的秒数
	unsigned char	areaadminid;			//如果为公司客服，该值表示公司对应的区域编号

	unsigned short	langtype;				//个人信息的语言版本				2(668)
	unsigned short	bak;					//									2(670)		
/*
	//基本信息
	TCHAR			nickname[MAX_USERNAME_LEN];		//网络名称					20
	TCHAR			username[MAX_USERNAME_LEN];		//真实名字					20(40)
	unsigned short	sex;							//性别						2(42)

	unsigned int 	birthday;						//出身年月					4(46)
	TCHAR 			memo[MAX_USERMEMO_LEN];			//简介						100(146)	
  
	//企业版需用编号公司、部门获取名称
	unsigned int	companyid;						//公司编号					4(150)
	unsigned int	departmentid;					//部门编号					4(154)
	
	unsigned short	country;						//国家名称					2(156)
	unsigned short	province;						//省份名称					2(158)
	TCHAR			city[MAX_CITY_LEN];				//城市名称					30(188)
	TCHAR			zipcode[MAX_ZIPCODE_LEN];		//邮编号码					10(198)
	  
	TCHAR	 		mobile[MAX_PHONE_LEN];			//手机						20(218)
	TCHAR			phone[MAX_PHONE_LEN];			//电话						20(238)
	TCHAR			fax[MAX_PHONE_LEN];				//传真号码					20(258)
		
	TCHAR			address[MAX_ADDRESS_LEN];	    //地址						60(318)			
	TCHAR			homepage[MAX_URL_LEN];			//主页						256(574)
	TCHAR			email[MAX_URL_LEN];				//电子邮件					256(830)
	TCHAR 			email_bak[MAX_URL_LEN];			//电子邮件(以前有，现未用)  256(未用)
		  
	//互联网需用直接获取职务和公司名称
	TCHAR 			dutyname[MAX_DUTY_LEN];			//职务名称		            30(860)
	TCHAR 			company[MAX_COMPANY_LEN];		//公司名称					40(900)
			
//	权限标志： 0xFFFF
//		位    含义     
//		0     用户加入是否需要验证：	0 不需要； 1 需要
//		1-2    发短信权限				0 不可发； 1 可发注册用户；2 可任意发送
//		3-4    是否有群发的权利：	    0 没有； 1 有
//		5	   是否有发起5人以上组	    0 没有； 1 有
//		6	   是否有创建公告栏         0 不可； 1 可以
//		7	   禁止其他人加我为联系人	0 不禁止；1禁止 
//		8     是否可服务器中转发送文件 0 不可； 1 可以
//		9     是否可服务器中转发送视频 0 不可； 1 可以
//		10     是否可以和临时客户通讯   0 不可； 1 可以通讯
			  
//		12 - 15  用户信息公开             0 公开; 1 对联系人公开; 2 保密
//		16     是否允许隐藏形象 		0 不允许； 1允许
//		17     是否隐藏形象 			0 不隐藏； 1隐藏
//		18 - 19  未用	
//		20	   是否允许隐身登陆			0 不允许； 1允许
//		21	   是否允许隐藏信息			0 不允许； 1允许
//		22	   是否审核其通讯纪录		0 不审核； 1审核	
//		23     是否可以创建会议  		0 不可； 1可	
//		25     是否禁止修改密码  		0 不禁止； 1禁止	
//		26     是否禁止修改个人信息     0 不禁止； 1禁止
	unsigned int	commflag;						//常用权限(用位表示)		4(904)
	
	//常用类别和状态(用位表示)	4(908)
//标志  0xFFFF
//		位    含义
//		0     是否享受电子名片的服务：	0 不享受； 1 享受
//		1     是否可以搜索到：     	0 不可找到； 1 可找到
//		0 - 11  保留
//		12 - 15  表情(16)：0 表情； 1： 开心；2： 郁闷；3： 愤怒；4： 痛苦
//		16 - 23  所属的组(256), 0表示为客户端保存的组(>256程序处理)
//		24 - 31  图像编号(256)  
	unsigned int	extflag;

  //在线状态标示
//	0 - 3   用户接入类型(16) (djz 081213 将0-1 改为0-3)		0： 直接接入 1： 局域网接入； 2：代理接入
//	4 - 5  用户类别：										0： 是注册用户； 1 注册客户;  2：临时用户
//	8 - 12  用户状态(16)										0 用户定义状态；1：离线；2：在线；3：出去一下：
//	4：请不要打扰
//	16      是否视频关闭/打开		0 没有：   1 打开
	
	unsigned short	langtype;				//个人信息的语言版本			2(914)
	unsigned short	externid;					//2 外联公司编号
	unsigned short	bak;
*/
	unsigned int	onlineflag;				//备用字段						4(912)
};

//有关用户的商务信息，兼容旧版本
struct TRADE_INFO
{
	TCHAR 			company[MAX_COMPANY_LEN];				//公司名称   40
	TCHAR 			leader[MAX_USERNAME_LEN];				//法人		 未用(20)
	unsigned short	duty;									//职务名称   2
	unsigned short	size;									//公司规模   2
	unsigned short	trade;									//所处行业   2
	
	TCHAR 			bankcode[MAX_BANK_LEN];					//开户银行	40		   
	TCHAR 			bank_num[MAX_BANKNO_LEN];				//银行账号	30
	TCHAR 		    tax_num[MAX_TAXNO_LEN];					//税务代号	30
	
	TCHAR 			mobile[MAX_PHONE_LEN];					//电话		未用
	TCHAR 			phone[MAX_PHONE_LEN];					//电话		20
	TCHAR 			fax[MAX_PHONE_LEN];						//传真		20
	TCHAR 			homepage[MAX_URL_LEN];					//主页名称  60
	TCHAR 			mailbox[MAX_EMAIL_LEN];					//			60
	
	TCHAR 			bak1[MAX_VERIFYMSG_LEN];				//			40
	unsigned short	langtype;								//个人信息的语言版本
	unsigned short 	companylangtype;						//			4
	TCHAR 			memo[MAX_TRADEMEMO_LEN];				//公司简介  500
};

//访客轨迹信息
struct WEBUSERTAIL_INFO
{
	unsigned int intime;						//4
	unsigned int outtime;						//4
	char url[MAX_URL_LEN];					//页面
	char title[MAX_URL_LEN];					//页面

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

	WEBUSERTAIL_INFO		tailinfo;	//浏览轨迹信息

	char					language[MAX_LANGUAGE_LEN];//访问的语言
//	char					timezone[MAX_TIMEZONE_LEN];//访问的时区
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
	//保存在数据库中的最新的id
	unsigned int	trafficid;

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
	unsigned int	uid;				//这个编号没有意义了，为固定的SYS_WEBUSER = 9999
	unsigned int	compid;				//公司编号
	unsigned int	siteid;
	char			sid[MAX_WEBCLIENID_LEN+1];	//这个为访客唯一标示
	char			thirdid[MAX_THIRDID_LEN+1]; //第三方id
	char	name[MAX_USERNAME_LEN+1];
	char   nameflag;	//记录名字是否被修改 1 已修改

//	char	scriptflag[MAX_SCRIPTFLAG_LEN];
//	char	
	
	unsigned char	status;		// 0 TALKSTATUS_NO, 1 STATUS_OFFLINE 离线， 2 STATUS_ONLINE 在线， 9 TALKSTATUS_TALK 对话中
	char			sip[MAX_IPNUM_LEN+1];
//	char			fromip[MAX_IPSTRING_LEN];
	char			ipfromname[MAX_USERNAME_LEN+1];			//ip来源

	char	url[MAX_URL_LEN+1];		//来源的url
	char	title[MAX_URL_LEN+1];		//来源的url
	char	chatfrom[MAX_CHATID_LEN + 1];
};

struct WEBUSER_UPINFO
{
	//<SYSTEM><COMMAND>UP</COMMAND><IECOPYRIGHT>MSIE 7.0</IECOPYRIGHT><SYSTEMINFO>Windows 2003</SYSTEMINFO>
	//<LANGUAGE>zh-cn,zh;q=0.5(中文)</LANGUAGE><VISIONCOLOR>32</VISIONCOLOR>
	//<VISIONSIZE>1024*768</VISIONSIZE><ADMINID>8008115</ADMINID>
	//<VISIT>http://www.tq.cn/?minimess_cn</VISIT><SOURCE>甘肃省金昌市</SOURCE><
	//SCRIPTFLAG>14641046576931075</SCRIPTFLAG><IP>125.74.161.222</IP><PORT>1230</PORT>
	//<CLIENTID>82632633301763783</CLIENTID>
	//<LASTVISIT>http://www.baoyuan28.com/index.asp?id=128</LASTVISIT>
	//<WEBTITLE> TQ洽谈通</WEBTITLE><NICKNAME>张三</NICKNAME><CTIMES>3</CTIMES><TTIMES>1</TTIMES>
	//<LASTVTIME>2010-8-9,12:44:55</LASTVTIME><LASTTTIME>2010-8-9,13:44:55</LASTTTIME>
	//<LASTTUIN>8071236</LASTTUIN><ISAUTOINVIT>1</ISAUTOINVIT></SYSTEM>
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
//	unsigned int	uid;
//	unsigned int userid;					//创建者
//	char	username[MAX_USERNAME_LEN];//创建者

	char		  sid[MAX_SID_LEN+1];	//这个为访客唯一标示
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
	TCHAR			strFile[MAX_FILENAME_LEN];	//256
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

/*
struct RECVCHATINFO
{
	unsigned int uFromAmdind; //会话来源管理员号码
	unsigned short sFromSort; //会话来源漂浮框id
	
	char			chatid[MAX_CHATID_LEN+1];
	
	unsigned int tStartTime;		  //会话开始时间
	unsigned int uWebUin;	  //web来宾的号码
	unsigned int uKefuUin;	  //客服的号码,0:等待应答状态，>0:正在和客服通信
	
	unsigned int uadminuin;	 //管理员号码
	unsigned short sSort;	 //漂浮框id
	char	strClientId[MAX_VISITORID_LEN+1];//访客唯一标示
	char	webnickname[MAX_USERNAME_LEN+1];//访客名称
	
	// 会话转移状态
	unsigned int Transfertoadminuin;	 //会话接受方管理员号码
	unsigned int TransfertoKefu;	     //会话接受方漂浮框id
	
};
*/

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
	
//	DEAL_TIME           numeric(28), --客户要求处理时间，如是回拨申请，则为客户要求回拨的时间
//	DEAL_TIME_LATE      numeric(28), --回拨的最迟时间
		
//		MESSAGE_TITLE       TEXT(50),  --留言标题
//		SUMMARY             TEXT(100), --内容摘要
//		TALK_CONTENT        Memo,  --会话内容
	int usermsgnum;			// --本次会话,客服的回复数(TALK_CONTENT中客服说话的次数)
	int visitorcluesnum;	// --本次会话线索数
//		REQUEST_REPLY_TYPE  TEXT(50),  --客户要求的答复方式
//		REPLY_TYPE          TEXT(50),  --客服实际答复方式
//		REPLY_CONTENT       Memo,  --客服答复内容
		
		
//		LAW_ADDRESS         TEXT(50),  --投诉地点
//		LAW_SIGNAL          TEXT(10),  --手机信号质量
//		LAW_CONNECT         TEXT(10),  --通信质量
//		LAW_FREQUENCY       TEXT(10),  --故障发生频率	

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
	tstring filaPath;			// 发送的文件路径
	int userType;				// 接收文件的用户类型
	tstring msgId;				// 消息的序列号
	int msgDataType;			// 消息的数据类型
};

// 重新接收媒体文件时，接收对象的信息传参
struct RERECV_FILE_PARAMS
{
	tstring filaPath;			// 文件的存储路径
	tstring url;				// 接收文件的url
	int userType;				// 发送消息的用户类型
	tstring msgId;				// 该消息的序列号
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
	tstring msgId;	// 消息id
	tstring msg;	// 消息内容
};

// 重新发送消息，包括文字消息，媒体文件的json串消息
struct RESEND_MSG_PARAMS
{
	tstring msgId;			// 消息id
	unsigned long userId;	// 接收消息用户的id
	int userType;			// 用户类型
	string mediaId;			// 媒体文件在微信服务器的id,消息类型是文字的时候，该参数代表文字消息内容
	int msgDataType;		// 消息数据类型
	string fileId;			// 媒体文件在文件服务器的id
	tstring filePath;		// 文件在本地存储路径
};

#endif