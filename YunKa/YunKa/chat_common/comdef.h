#ifndef _COMMDEF_EMC
#define _COMMDEF_EMC
#include <list>

#define SOLVEFLAG_NULL			0
#define SOLVEFLAG_UPDATE		1
#define SOLVEFLAG_ADD			2
#define SOLVEFLAG_DELETE		3

#define EXMIS_NORMAL			0
#define EXMIS_POPUP				1
#define EXMIS_TRIANGLE			2

//回复请求结果
#define RESPINVITE_SUCC		0
#define RESPINVITE_FAIL		1
#define RESPINVITE_TIMEOUT	2

//图标的分类
#define ICON_MAINFRAME		0	//主框架窗口的图标(在线)

//Tree的使用类型
#define TREE_NORMAL		0	//用户
#define TREE_USER		1	//用户
#define TREE_KEYWORD	2	//快捷方式

//文件格式的开始类型
#define FLAG_COPYNO				1000
#define FLAG_FILE_START			0x454d
#define FLAG_FILE_END			0x55AA

//版本更新分类
#define COPY_NOTCHECK			0
#define COPY_AUTOCHECK			1
#define COPY_TIMECHECK			2

//窗口的停靠位置
#define WNDDOCK_NO		0
#define WNDDOCK_TOP		1
#define WNDDOCK_LEFT	2
#define WNDDOCK_RIGHT	3

//自动回复, 快捷回复的内容条数
#define MAX_CONTENT_RESEND	10  
#define MAX_LOGININFO		10  

//皮肤窗口使用
#define EMMIS_NORMAL			0
#define EMMIS_POPUP				1
#define EMMIS_TRIANGLE			2

//static 定义的类型
#define STATIC_NORMAL	0
#define STATIC_JUMPURL	1
#define STATIC_JUMPCMD	2

//皮肤类型
#define FACE_WIN			0
#define FACE_EM2005			1

//托盘使用的字符串
#define TRAY_STRING_FIRST		0	//从未登录成功过
#define TRAY_STRING_NORMAL		1	//zh
#define TRAY_STRING_NEWMSG		2

//系统用户
#define SYS_ALLUSER				997		//表示为群消息， 公司所有人都可以收到
#define SYS_WEBUSER				998		//表示为访客，具体标示由sid来区分
#define SYS_SYSUSER				1000

//用户名显示类型
#define DISPLAYUSER_NAME			0	//用户名
#define DISPLAYUSER_NAMEUID			1	//用户名(编号)
#define DISPLAYUSER_NAMESID			2	//用户名(登陆名)
#define DISPLAYUSER_NAMESIDUID		3	//用户名(登陆名:编号)

//欢迎语
#define WELLCOME_ALL				0	//这里用来获取所有的开场白信息
#define WELLCOME_NORMAL				1	//欢迎语-打开对话框显示的第一条消息内容。
#define WELLCOME_CONNECTED			2	//联线自动回复--连接成功后发送的欢迎词。
#define WELLCOME_REQTIMEOUT			3	//客人请求超时--客人发送第一句话开始，没有客服回应。
#define WELLCOME_WAITRESPTIMEOUT	4	//客人等待答复超时--客人发送消息后等待超时发送的消息
#define WELLCOME_RESPTIMEOUT		5	//客人答复超时--客人没有发送消息的时间
#define WELLCOME_RESPTIMEOUTCLOSE	6	//客人答复超时关闭对话--客人没有发送消息的时间超过后终止对话

#define IM_NORMAL			0
#define IM_QQ				1
#define IM_MSN				2

#define TALKDLGTOOL_GIF			0
#define TALKDLGTOOL_CUTSCREEN	1
#define TALKDLGTOOL_FILE		2
#define TALKDLGTOOL_QUICKREPEAT	3
#define TALKDLGTOOL_LANGUAGEEN	4
#define TALKDLGTOOL_APPRAISE	5
#define TALKDLGTOOL_CALL		6
#define TALKDLGTOOL_SAVE		7
#define TALKDLGTOOL_LANGUAGEZH	8
#define TALKDLGTOOL_QUESTION	9
#define TALKDLGTOOL_PRINT		10

#define LISTUSER_ALL		0
#define LISTUSER_ONLINE		1
#define LISTUSER_OFFLINE	2

#define LISTUSER_FORBID		3

#define	NEXT_NULL	0
#define	NEXT_UPFILE	1
#define	NEXT_SAVE	2


#define	GETANDSAVE_NULL	0
#define	GETANDSAVE_GET	1
#define	GETANDSAVE_SAVE	2

#define UPDATA_STATUS_NULL	0
#define UPDATA_STATUS_FILE	1
#define UPDATA_STATUS_INFO	2
#define UPDATA_STATUS_SUCC	3

#define WEBUSERICON_NULL			-1
#define WEBUSERICON_NORMAL			0
#define WEBUSERICON_VISIT1			1
#define WEBUSERICON_VISIT10			2

#define WEBUSERICON_SEARCHURL		3
#define WEBUSERICON_SEARCHURLMID	4
#define WEBUSERICON_SEARCHURLTOP	5
#define WEBUSERICON_FRDURL			6
#define WEBUSERICON_UNIONURL		7
#define WEBUSERICON_INOUT			8	// 通过联盟转出后返回的访客 [12/9/2010 SC]
#define WEBUSERICON_DIRECTURL		9
#define WEBUSERICON_ALERT			10
#define WEBUSERICON_REFUSE			11
#define WEBUSERICON_MUTIUSER		12
#define WEBUSERICON_OFFLINE			13
#define WEBUSERICON_FORBID			14
#define WEBUSETICON_ALERTMSG		15
#define WEBUSETICON_ALERTPHONE		16

/* 0xFF特殊要求特殊对待
*  没办法
*/
#define WEBUSERICON_OTHERURL		0xFF



//统计分析类别
#define TOTAL_OVERVIEW			001

#define TOTAL_SEARCHURL			101
#define TOTAL_SEARCHKEY			102

#define TOTAL_FROMCURURL		201
#define TOTAL_FROMPREVURL		202
#define TOTAL_FROMPREVURLSORT	203

#define TOTAL_DATETIMEMONTH		301
#define TOTAL_DATETIMEDAY		302
#define TOTAL_DATETIMETIME		303

#define TOTAL_DEPTNUM			401
#define TOTAL_DEPTURL			402
#define TOTAL_DEPTTIME			403

#define TOTAL_CLIENTAREA		501
#define TOTAL_CLIENTBROWSER		502
#define TOTAL_CLIENTSIZE		503
#define TOTAL_CLIENTOS			504

#define TOTAL_TALKUSERINFO		601
#define TOTAL_TALKUSERACCEPT	602


//错误类型
#define	ERROR_READ_FILE			400
#define	ERROR_FILE_EXT			401

//声音类型
#define SOUND_ONLINE					0
#define SOUND_INVITE					1
#define SOUND_MSG						2
#define SOUND_RING						3
#define SOUND_ALERT						4

#define COLOR_MAINBORDER			RGB(171,198,221)
#define COLOR_MAINBACKGROUND		RGB( 240, 244, 248)

//背景色
#define COLOR_BACKGROUND_ALL			RGB(255,255,255)
//item选中的背景色 
#define COLOR_BACKGROUND_ITEMSELECT		RGB(0x39, 0x5b, 0xaa )

//item头的背景色
#define COLOR_ITEMHEAD_BACKGROUND		RGB( 240, 244, 248)	//f0f4f8
//item头的文本色
#define COLOR_ITEMHEDA_TEXT				RGB( 13,19,23 )

//文本的正常色和选中焦点色
#define COLOR_TEXT_NORMAL		RGB(0,0,0)
#define COLOR_TEXT_SELECT		RGB(255,255,255)
#define COLOR_TEXT_TALKED		RGB(0,0,255)
#define COLOR_TEXT_FLASH		RGB(253,194,72)		//fdc248

//用户项的背景色
#define COLOR_USERITEM_BKCOLOR	RGB(255,255,255)

//离线组和item的背景色
#define COLOR_OFFLINE_GROUP		RGB(0x80, 0xa7, 0xff)	//f0f4f8
#define COLOR_OFFLINE_SUBGROUP	RGB(0xcd, 0xdd, 0xff)	//f0f4f8
#define COLOR_OFFLINE_ITEM		RGB(240, 232, 219)

//在线组和item的背景色
#define COLOR_ONLINE_GROUP		RGB(0x80, 0xa7, 0xff)	//#becff0
#define COLOR_ONLINE_SUBGROUP	RGB(0xcd, 0xdd, 0xff)	//#e1e9f7
#define COLOR_ONLINE_ITEM		RGB(254, 240, 201)

//等待应答item背景色
#define COLOR_WAITRESP_ITEM		RGB(254, 0, 0)


//从第一级开始， 用以下数查找每一个项。

#define	TREELPARAM_ALERT_MSG		50  // 留言提醒
#define	TREELPARAM_ALERT_PHONE		51	// 来电弹屏
#define TREELPARAM_ONLINVISITOR		52	// 在线访客

#define	TREELPARAM_WAIT_GROUP		100	// 等待开始
#define	TREELPARAM_WAIT_RESP		101	// 等待应答
				
#define	TREELPARAM_USER_TALK		200	// 对话中
#define	TREELPARAM_USER_TRANSFER	201	// 转接中
#define	TREELPARAM_USER_INVITE		202	// 邀请中
#define	TREELPARAM_USER_INNERTALK	203	// 内部对话

#define	TREELPARAM_VISIT_AUTOINVITE	300	// 自动邀请
#define	TREELPARAM_VISIT_ONLINE		301	// 访问中
#define	TREELPARAM_VISIT_OFFLINE	302	// 已离开

#define	TREELPARAM_MAX_VALUE		999

#define TYPESELECT_TXT				0
#define TYPESELECT_URL				1
#define TYPESELECT_FILE				2

#define TYPESELECT_MSG				0 // 第一个选项卡
#define TYPESELECT_INFO				1 // 访客信息
#define TYPESELECT_CLIENT			2 // 聊天信息
#define TYPESELECT_CHATID			3 // 会话登单
#define TYPESELECT_CLIENTINFO		4 // 客户信息
#define TYPESELECT_NOTICE			5 // 新建订单
#define TYPESELECT_ORDER			6 // 查询订单

#define TYPESELECT_WEBURL			10 // 显示历史记录等页面
#define TYPESELECT_WEBADMIN			11 // 显示管理中心

//服务器的连接类型
#define CONNECT_UDP				0x0000		//UDP 连接UI
#define CONNECT_TCP				0x0001		//TCP 连接UI
#define CONNECT_SSL				0x0002		//SSL 连接UI
#define CONNECT_HTTP			0x0003		//HTTP 连接proxy01.tq.cn
#define CONNECT_HTTPS			0x0004		//HTTP 连接proxy01.tq.cn
#define CONNECT_SOCKET			0x0005		//socket 连接UI
#define CONNECT_TCPPROXY		0x0101	// TCP 同时  proxy 0x0100|CONNECT_TCP
#define CONNECT_SSLPROXY		0x0102	// SSL 同时  proxy 0x0100|CONNECT_SSL
#define CONNECT_HTTPPROXY		0x0103	// HTTP 同时 proxy 0x0100|CONNECT_HTTP

//代理服务器的连接类型
#define PROXY_NO				0
#define PROXY_SOCKET			1
#define PROXY_HTTP				2

//用于NAT用户连接的测试命令
#define P2PCONNECTNAT_SEND			0	//send 
#define P2PCONNECTNAT_RESP			1	//resp direct
#define P2PCONNECTNAT_RESPEX		2	//resp by server

//用于NAT用户连接的类型
#define P2PCONNECTNAT_NORMAL		0	//msg
#define P2PCONNECTNAT_VIDEO			1	//video
#define P2PCONNECTNAT_AUDIO			2	//audio
#define P2PCONNECTNAT_FILE			3	//file

//对话状态
#define TALKSTATUS_NO			0	//没有对话
#define TALKSTATUS_AUTOINVITE	1	//自动邀请中
#define TALKSTATUS_REQUEST		2	//请求中
#define TALKSTATUS_REQUESTRESP	3	//请求回复中，表示主动
#define TALKSTATUS_INVITE		4	//邀请中
#define TALKSTATUS_INVITERESP	5	//回复中，表示被动
#define TALKSTATUS_TRANSFER		6	//转接中
#define TALKSTATUS_TRANSFERRESP	7	//转接中, 表示被动
#define TALKSTATUS_WELLCOME		8	//当代发送wellcome对话消息
#define TALKSTATUS_TALK			9	//对话中

#define HASTALKED		1	//通过话
#define INTALKING		2	//正在通话中


//在线图标数状态
#define STATUS_LEN				10


#define ANNOUNCEMENTEVENT_RINGING		1
#define ANNOUNCEMENTEVENT_FREEPHONE		2
#define ANNOUNCEMENTEVENT_WEBNOTE		3
#define ANNOUNCEMENTEVENT_BROADCAST		4

//userObject中获取在线信息需要
#define ONLINE_INFO_CONNECTTYPE		0
#define ONLINE_INFO_CONNECTTYPE_LEN	4
#define ONLINE_INFO_USERTYPE		4
#define ONLINE_INFO_USERTYPE_LEN	2
#define ONLINE_INFO_STATUS			8
#define ONLINE_INFO_STATUS_LEN		4
#define ONLINE_INFO_VIDEO			16
#define ONLINE_INFO_VIDEO_LEN		1

#define STATUS_UNDEFINE			-1
#define STATUS_USERDEFINE		0
#define STATUS_OFFLINE		    1
#define STATUS_ONLINE			2
#define STATUS_HIDE				3
#define STATUS_WORK				4
#define STATUS_EATING           5
#define STATUS_LEAVE			6
#define STATUS_ROBOT            7
#define STATUS_SEND_SMS         8
#define STATUS_SEND_MAIL        9
#define STATUS_BUSY				10
#define STATUS_SEND_MSG         12
#define STATUS_REFUSE_NEWWEBMSG	13
#define STATUS_UNKNOWN	14   //用户状态未知


//访客过滤标示
//该标示只对等待接待，访问中和已离开有效
#define VISITORFILTER_ALL			0
#define VISITORFILTER_MYVISITOR		1	//只显示我自己的访客，不显示访问中和已离开的人，在等待接待和显示
#define VISITORFILTER_ALLVISITOR	2	//显示全部客服的访客，
#define VISITORFILTER_1MINUTES		3
#define VISITORFILTER_3MINUTES		4
#define VISITORFILTER_5MINUTES		5
#define VISITORFILTER_10MINUTES		6
#define VISITORFILTER_USERDEFINE	10

//一个通讯包中的列表最大数
#define MAX_LIST_DEPARTMENT		25
#define MAX_LIST_FRDGROUP			100		//djz
#define MAX_LIST_FRD			500		//djz
#define MAX_LIST_ONLINEFRD		500		//非定义状态的联系人呢 50 ->500
#define MAX_LIST_ONLINEFRDEX	250		//自定义状态的联系人呢 25->250
#define MAX_LIST_QUERYFRD		12		//查找用户
#define MAX_LIST_URL			4		//URL的最大数
#define MAX_LIST_GROUP			25		//组、公告和会议
#define MAX_LIST_QUERYGROUP		15		//查找群用户
#define MAX_LIST_MUTIUSER		50		//群呼消息的用户数
#define MAX_LIST_FILE			3		//发送文件包中文件的个数
#define MAX_LIST_FILEEX			15		//发送共享文件名称使用,文件名称限制在60个以内
#define MAX_LIST_UIN			100		//群呼消息总接收的个数
#define MAX_LIST_NAMEID			50		//name + id

#define MAX_OFFLINE_MSG	20
#define MAX_UPDATE_FRDLIST_NUM 100

//用户终端类型
#define TERMINAL_PC					0
#define TERMINAL_PHONE				1
#define TERMINAL_WEBUI				2
#define TERMINAL_PCEX				0

//登录名称的选择 
#define LOGIN_BYUID				0
#define LOGIN_BYSTRING			1
#define LOGIN_BYMOBILE			2
#define LOGIN_BYMAIL			3
#define LOGIN_BYWEB				50

//登录方式
#define LOGIN_NORMAL			0  //正常登录
#define LOGIN_OFFLINE			1  //离线后再登录
#define LOGIN_SERVERDOWN		2  //服务启重启后自动登录
#define LOGIN_OFFLINEAUTO		3  //离线后自动再登录
#define LOGIN_LITE				10  //精简上线，上线时mp只发登陆成功、失败包，不发其他包（好友分组，好友列表，在线列表） 2010.1.12

#define OFFLINE_STATUS_MSG		10	//离线时置的标志位
#define OFFLINE_STATUS_SMS		11		


//服务器的连接类型
#define MSG_UDP					0
#define MSG_NET					1
#define MSG_PROXY				2
#define MSG_WEBUI				3
#define MSG_TCPUI				4//从tcp ui
#define MSG_SSLUI				5//ssl ui
#define MSG_HTTP				6 //从proxy01.tq.cn登录

//查询用户的类型
#define SEARFRD_ID		0
#define SEARFRD_STRID	1
#define SEARFRD_MOBILE	2
#define SEARFRD_EMAIL	3
#define SEARFRD_ADDME	100

//发送的消息类型
#define MSG_NORMAL			0 // 普通消息

#define MSG_FLOAT_NORMAL	2	// 普通消息
#define MSG_FLOAT_PRV		3	// 预知消息
#define MSG_FLOAT_INPUTING	4	// 客服正在输入消息
#define MSG_WX				50	// 来自微信端的json消息，服务器将xml转化为json

#define MSG_WORKBILL       101  //工单号
#define MSG_GET_WORKBILL   102  //获取工单号
#define MSG_FONT           103 //字体设置信息
#define MSG_GET_EVALUATE   104 //获取评价信息
#define MSG_CLIENT_EXIT		105 //网页客人已离开
#define MSG_CLIENT_COME    106  // 网页客人上线了
#define MSG_TRANSFER_REQUEST 107 //会话转移请求
#define MSG_TRANSFER_ANSWER  108 //会话转移应答

//------客户端ver>MULTI_TERMINAL_HTTPBLOCK_SYSDEF(4223)支持------------
#define MSG_CLIENT_EXIT_MSG   109 	    	//访客离开时，给坐席的提示消息息，该客人离开了
#define MSG_CLIENT_APPRAISE_MSG   110 		//访客对客户的评价提示消息
#define MSG_CLIENT_CHANGE_ADDRESS_MSG   111 	//访客对客户的评价提示消息
#define MSG_CLIENT_SENDFILE_FAILED_MSG   112 	//访客发送文件失败提示消息
#define MSG_CLIENT_SENDFILE_SUCCESS_MSG   113 	//访客发送文件成功提示消息
#define MSG_CLIENT_TRANSFER_IN_MSG   114 	//[系统消息] 访客选择了与您通话
#define MSG_CLIENT_TRANSFER_OUT_MSG   115 	//[系统消息] 访客已经将会话转移到%s(%u)。
#define MSG_CLIENT_ALREADYEXIT_MSG   116 	//该客人已离开 您的消息将无法准确送达 请选择另外的通知方式
#define MSG_CLIENT_WELCOME_MSG   117 		//网页客人上线了给客户端的欢迎语提示消息 你的网站来客人了
#define MSG_CLIENT_COLLECT_MSG   118 		//js发给客服的访客收集页面收集到的提示消息
#define MSG_KEFU_GET_CHAT_HISMSG   120 		//坐席向webui索要chat会话的历史消息，2015年11月22日
#define MSG_WEBUI_SEND_CHAT_HISMSG   121	//webui向坐席发送chat会话的历史消息，2015年11月22日
#define MSG_KEFU_GET_WX_USERINFO	122	//坐席向webui索要微信用户信息，2015年11月26日
#define MSG_KEFU_GET_WX_ACCESSTOKEN		124	//坐席向webui索要微信actoken，2015年11月26日


#define MSG_SEND_FAIL       1000 //消息发送失败

#define MSG_ADDFRD			200
#define MSG_DELETEFRD		201

#define MSG_RESPADDEFRD		205		//同意加入
#define MSG_RESPDENYFRD		206		//拒绝加入

//发送的消息格式
#define MSG_TXT				0  //文本消息
#define MSG_RTF				1  //rtf格式文本消息
#define MSG_HTML			2  //html格式消息
#define MSG_RTFFILE			3  //以rtf格式保存的文件消息，此时消息内容为文件名


//发送的消息格式
#define MSG_TXT				0  //文本消息
#define MSG_RTF				1  //rtf格式文本消息
#define MSG_HTML			2  //html格式消息
#define MSG_RTFFILE			3  //以rtf格式保存的文件消息，此时消息内容为文件名

//MSG这个包的BAK备用字段的使用约定dw
#define MSG_BAK_NORMAL    0   //BAK字段的默认值
#define MSG_BAK_INPUTING  1   //对方正在输入
#define MSG_BAK_AUTOANSER 2   //自动答复，包括机器人
#define MSG_BAK_TRANSFER_ACCEPT	3 // 接受会话转移
#define MSG_BAK_TRANSFER_TIMEOUT 4 // 会话转移超时

//nameid包含的内容
#define NAMEID_ID				0  //uid 
#define NAMEID_IDNAME			1  //uid + name
#define NAMEID_IDNAMEIPPORT		2  //uid + name + ip + port
#define NAMEID_IDONLINE			3  //uid + ip + port + videoip, videoport
#define NAMEID_IDNAMESTATUS		4  //uid + name + status

//性别标志
#define SEX_ALL			0
#define SEX_MAN			1
#define SEX_WOMAN		2

//登陆状态
#define LOGINSTATUS_ONLINE			0
#define LOGINSTATUS_ONLINE_LEN		4
#define LOGINSTATUS_VIDEO			5
#define LOGINSTATUS_VIDEO_LEN		1

//服务器收到包没有进入处理
#define COMM_ERROR_NO					0
#define COMM_ERROR_UNKNOWN				100	//other
#define COMM_ERROR_SERVERBUSY			1	//server is busy 服务器忙
#define COMM_ERROR_ERRORCOPY			2	//copy is not correct 服务器运行标志和客户端不一样
#define COMM_ERROR_ERRORCMD				3	//
#define COMM_ERROR_ERRORUID				4	//
#define COMM_ERROR_ERRORPACKSIZE		5	//
#define COMM_ERROR_ERRORGETDB			6	//
#define COMM_ERROR_ERRORUNPACK			7

#define COMM_ERROR_COMPISNOTEXIST		8
#define COMM_ERROR_USERISNOTEXIST		9
#define COMM_ERROR_ERRORPASS			10
#define COMM_ERROR_TOOMANYUSER			11
#define COMM_ERROR_FORBIDHIDELOGON		12
#define COMM_ERROR_FORBIDUSER			13
#define COMM_ERROR_TOOMANYRANDUSER		14
#define COMM_ERROR_CATCHERROR			15
#define COMM_ERROR_DENYRANDUSER			16
#define COMM_ERROR_FAILGETRANDUID		17
#define COMM_ERROR_FAILADD				18
#define COMM_ERROR_SENDPACK				19
#define COMM_ERROR_RECVPACK				20
#define COMM_ERROR_WEBUSERISNOTEXIST	21
#define COMM_ERROR_SENDWEB				22
#define COMM_ERROR_RECVWEB				23
#define COMM_ERROR_TRANSFERERROR		24
#define COMM_ERROR_EXPIREDADE			25
#define COMM_ERROR_DBSQL				26


#define DENYLOGIN_USERISNOTEXIST	1   //用户不存在
#define DENYLOGIN_PASSWORDERROR		2   //密码错误
#define DENYLOGIN_TOOMANY			3   //服务器用户数太多了
#define DENYLOGIN_FORBIDHIDELOGON	4   //该用户不能隐身登陆
#define DENYLOGIN_FORBIDUSE			5   //用户已禁用
#define DENYLOGIN_TOOMANYCLIENT		6   //服务器用户数量已超过许可
#define DENYLOGIN_SERVERBUSY		8   //服务器繁忙，一般是获取数据库连接失败了
#define DENYLOGIN_OLDCOPY			10  //客户端版本太低，服务器拒绝登陆!
#define DENYLOGIN_INBLACKLIST		17   //用户在黑名单中
#define DENYLOGIN_TOOOFTEN		    18   //登陆太频繁了
#define DENYLOGIN_SERVERERROR		19   //服务器内部错误
#define DENYLOGIN_LOGONTYPEERROR	20   //登陆方式不识别
#define DENYLOGIN_UNPACKERROR	    21   //解包出错

//是否第一次登陆，本地没有保存信息
#define LOGIN_FIRST				1  //正常登录

//过滤设置最大字符串长度
#define MAX_ALLSTRINGFILTER_LEN		250
//过滤设置最大字符串个数, 用','分开
#define MAX_STRINGFILTER_NUM		10
#define MAX_STRINGFILTER_LEN		50
#define MAX_MSG_LEN					910
#define MAX_SID_LEN					60

#define MAX_TIMEZONE_LEN	100
#define MAX_BROWSER_LEN		20
#define MAX_LANGUAGE_LEN	100
#define MAX_VSIZE_LEN		20

#define MAX_SCRIPTFLAG_LEN	40
#define MAX_USERNAME_LEN	60
#define MAX_PASSWORD_LEN	33
#define MAX_STATUS_LEN		60
#define MAX_FILENAME_LEN	256 
#define MAX_PHONE_LEN		20
#define MAX_URL_LEN			256 // 120- 256 有些地址被截断 [12/27/2010 SC]
#define MAX_AREA_LEN		50
#define MAX_ZIPCODE_LEN		10 
#define MAX_EMAIL_LEN		120
#define MAX_COMPANY_LEN		40		//公司
#define MAX_CITY_LEN		30
#define MAX_MSGNICKNAME_LEN	66		// djz 消息包中昵称的长度 07.08.7
#define MAX_MSGFONT_LEN		60		// djz 消息包中字体格式字符串的长度 09.05.13
#define MAX_MEMO_LEN		255		//备注信息
#define MAX_IPSTRING_LEN	120
#define MAX_IPNUM_LEN		20
#define MAX_RESPONSE_LEN	255
#define MAX_ADDRESS_LEN		60

#define MAX_BANK_LEN		40
#define MAX_BANKNO_LEN		30
#define MAX_TAXNO_LEN		30
#define MAX_USERMEMO_LEN	100  //个人信息介绍
#define MAX_TRADEMEMO_LEN	500  //公司介绍
#define MAX_VERIFYMSG_LEN	40	//验证信息长度

#define MAX_MSG_RECVLEN			8000 //
#define MAX_MSG_SENDLEN			5000 
#define MAX_SMS_LEN			150

#define MAX_FCT_LEN					1000		//发送的文件数据校验表的长度
#define MAX_FDT_LEN					1000		//发送的文件数据没一个包的长度

#define MAX_128_LEN			128 
#define MAX_256_LEN			256 
#define MAX_512_LEN			512 
#define MAX_1024_LEN		1024 
#define MAX_2048_LEN		2048 
#define MAX_4096_LEN		4096
#define MAX_8192_LEN		8192
#define MAX_VISITORID_LEN	60	//访客的

#define MAX_FACELINK_LEN	100
#define MAX_GROUPUSER_NUM		128		//组的用户最大数
#define MAX_GROUPNAME_LEN	20 

#define MAX_CHATID_LEN		30
#define MAX_WEBCLIENID_LEN	60   //会话中访客唯一标示，rand或者微信id比较长
#define MAX_THIRDID_LEN     100  //第三方id长度


#define MAX_TITLE_LEN	50 
#define MAX_TAIL_LEN	100 
#define MAX_COLOR_LEN	20 

#define MAX_SEARCHKEYWORDS_NUM	10	//搜索引擎恶意报警关键词的最大数
#define MAX_SEARCHKEYWORDS_LEN	40	//搜索引擎每一条恶意报警关键词的长度

//userflag标示
#define USERFLAG_ISINVITE			0
#define USERFLAG_ISTALKED			1

//0为自由设置的值
#define USERAUTH_ROLE_ADMIN			1
#define USERAUTH_ROLE_ADVANCE		2
#define USERAUTH_ROLE_PROJECT		3
#define USERAUTH_ROLE_USER			4

#define USERAUTH_VIEWVISITEROFFLINEMSG		0 //查看访客留言
#define USERAUTH_VIEWVISITERHISTORYMSG		1 //查看历史对话记录
#define USERAUTH_VIEWOTHERUSERMSG			2 //查看其他客服的对话记录
#define USERAUTH_VIEWTOTAL					3 //查看统计分析
#define USERAUTH_VIEWVISITERINFO			4 //查看客户管理, 可以查看客户的资料信息
#define USERAUTH_ADDTOOTHERUSERTALK			5 //加入别的客服的对话
#define USERAUTH_SETTALKAUTORULE			6 //设置对话自动分配规则
#define USERAUTH_SORTVISITER				7 //设置客人类别列表
#define USERAUTH_CHANGEOTHERUSERINFO		8 //修改登录名，只允许修改自己的登录名密码
#define USERAUTH_CHANGEVISITERNAME			9 //修改客人名称
#define USERAUTH_DELETEHISTORYMSG			10 //删除对话历史记录
#define USERAUTH_DELETEVISITEROFFLINEMSG	11 //删除访客留言
#define USERAUTH_ADDUSER					12 //设置操作员
#define USERAUTH_SETFLOATADANDINFO			13 //设置对话窗口内容和广告
#define USERAUTH_SETFLOATDEFAULTINFO		14 //设置浮动图标默认参数
#define USERAUTH_SETCOMMONKEYWORD			15 //设置公用快捷回复内容
#define USERAUTH_SETFLOATTITLE				16 //设置网页标题
#define USERAUTH_SETINVITETYPE				17 //设置主动邀请样式
#define USERAUTH_OUTHISTORYANDTOTAL			18 //允许导出历史记录和导出统计报表
#define USERAUTH_LOGIN						19 //允许登录

#define USERAUTH_INVITEVISITER				20 //在设置为自动邀请的页面上能够主动邀请访客
#define USERAUTH_VIEWALARM					21 //恶意报警
#define USERAUTH_AUTOINVITE					22 //设置自动邀请
#define USERAUTH_OFFLINERESP				23 //设置离线回复（说明：这个是针对离线后，把用户消息发到手机短信上）
#define USERAUTH_SETWELLCOME				24 //开场白设置
#define USERAUTH_SETASKEDQUESTION			25 //设置常见问题
#define USERAUTH_BINDIM						26 //QQ、MSN捆绑
#define USERAUTH_FASTREPLY					27 //修改公用快捷回复

#define WEBUSERAUTH_FORBID					0 //被屏蔽

//服务器类型
#define TRACQTYPE_TRACQ		0	//用于电子名片
#define TRACQTYPE_OAM		1	//专用于OA Messenger

#define TRACQTYPE_SP		10	//专用于sp定制
#define TRACQTYPE_IT168		11	//专用于it168定制
#define TRACQTYPE_QIJI2000	12	//专用于qiji2000定制
#define TRACQTYPE_17U		13	//专用于同程定制
#define TRACQTYPE_UNI		14	//专用于东软定制

//登陆后的处理类型
#define SUBLOGON_NORMAL		0	//正常
#define SUBLOGON_UDP		1	//继续通过发包获取
#define SUBLOGON_TCP		2	//通过TCP来获取

//默认的通讯端口
#define EM_PORT						8001
#define EM_DBPORT					8005		//数据库消息监测端口
#define EM_VIDEOPORT				8006		//视频通讯端口
#define PROXY_PORT					80			//代理服务器的端口
#define PROXY_LOCALPORT				6666		//代理服务器的本地端口
#define SMTP_PORT					25		//smtp port
#define POP_PORT					110		//pop port
#define EMVISITOR_PORT				9005

//定义通讯包的大小
#define PACKMAXLEN					8192//
#define PACKNORMALLEN				8192//
//定义默认的字符串的大小
#define BUFFMAXLEN					1024   //1.k


#define SYS_ERROR_MEMORY			100000
#define SYS_ERROR_SEND_BEFORE_LOGIN	100001 // 发送了登录后才能发送的消息类型

#define COMM_ERROR_SEG				400000
#define COMM_DENYLOGIN_SEG			410000

#define SYS_ERROR_SOCKETCREATEFAIL	800001
#define SYS_ERROR_SOCKETCONNECTFAIL	800002
#define SYS_ERROR_PACKHEAD			800003
#define SYS_ERROR_PACKINFO			800004
#define SYS_ERROR_SENDFAIL			800005
#define SYS_ERROR_RECVFAIL			800006
#define SYS_ERROR_INVALIDCMD		800007
#define SYS_ERROR_PACKINVALIDLEN	800008
#define SYS_ERROR_UNPACKHEAD		800009
#define SYS_ERROR_UNPACKINFO		800010
#define SYS_ERROR_SOCKETCLOSE		800011
#define SYS_ERROR_TIMEOUT			800012
#define SYS_ERROR_OPENFILE			800013
#define SYS_ERROR_READFILE			800014
#define SYS_ERROR_INVALIDAUTH		800015

//打包和解包过程中的错误
#define PACK_SUCCESS				0	//没有错误
#define PACK_ERROR_SLOPOVER			1	//指针越界
#define PACK_ERROR_NULLSTRING		2	//空字符串
#define PACK_ERROR_PACKTOOLARGE		3	//包太大
#define PACK_ERROR_STRINGTOOLONG	4	//捷包时字符串超过包的大小

//打包和解包过程中的警告信息
#define PACK_WARNING_STRINGTOOLONG	101	//字符串超长


//2015年10月4日 4216->6001
#define VERSION						6010 
#define VISITOR_VERSION				3006 

//用户信息中状态位布尔类型状态的索引
#define USER_AUTH_VERIFY_ADDFRD		0
#define USER_AUTH_SENDSMS			1
#define USER_AUTH_SENDSMS_LEN		2
#define USER_AUTH_MUTISEND			3
#define USER_AUTH_MUTISEND_LEN		2
#define USER_AUTH_GROUP				5
#define USER_AUTH_PROJECT			6
#define USER_AUTH_DENYADD			7		//禁止其他人加我为联系人
#define USER_AUTH_FILEBYSERVER		8
#define USER_AUTH_VIDEOBYSERVER		9
#define USER_AUTH_WEBCQ				10
#define USER_AUTH_COMMOPEN			12
#define USER_AUTH_COMMOPEN_LEN		4
#define USER_AUTH_TRADEOPEN			16
#define USER_AUTH_TRADEOPEN_LEN		4

#define USER_AUTH_HIDELOGON			20
#define USER_AUTH_HIDELOGON_LEN		1
#define USER_AUTH_HIDEINFO			21
#define USER_AUTH_HIDEINFO_LEN		1
#define USER_AUTH_SENDBYSERVER		22
#define USER_AUTH_SENDBYSERVER_LEN	1

#define USER_AUTH_MEETINGCREATE		23
#define USER_AUTH_MEETINGCREATE_LEN	1

#define USER_AUTH_FORBIDCHANGEPASS		25
#define USER_AUTH_FORBIDCHANGEPASS_LEN	1
#define USER_AUTH_FORBIDCHANGEINFO		26
#define USER_AUTH_FORBIDCHANGEINFO_LEN	1

#define USER_AUTH_ICONNO			24
#define USER_AUTH_ICONNO_LEN		8

#define TRADE_AUTH_TRACARD			0
#define TRADE_AUTH_SEARCH			1
#define TRADE_AUTH_FACE				12
#define TRADE_AUTH_FACE_LEN			4
#define TRADE_AUTH_GROUPNO			16
#define TRADE_AUTH_GROUPNO_LEN		8

#define ONLINE_CONNECTTYPE		0
#define ONLINE_CONNECTTYPE_LEN	4
#define ONLINE_USERTYPE			4
#define ONLINE_USERTYPE_LEN		2
#define ONLINE_STATUS			8
#define ONLINE_STATUS_LEN		4
#define ONLINE_VIDEO			16
#define ONLINE_VIDEO_LEN		1

//更新资料的类别
#define UPDATE_STATUS			0	//更新状态
#define UPDATE_PASS				1	//更新密码
#define UPDATE_VIDEO			2	//更新视频
#define UPDATE_LOGINSTRINGID	3	//更新登录名称
#define UPDATE_CONNECTTYPE		4	//连接类型	
#define UPDATE_USERTYPE			5	//用户类型
#define UPDATE_USERINFO			6	//更新用户信息
#define UPDATE_TRADEINFO		7	//更新商务信息
#define UPDATE_ALLINFO			8	//更新用户信息和商务信息， 不包括新闻
#define UPDATE_COMPANYNEWS		9	//更新新闻
#define UPDATE_ALLFLAG			10	//更新所有的标志位信息
#define UPDATE_COMMFLAG			11	//更新标志位信息
#define UPDATE_EXTFLAG			12	//更新扩展标志位信息
#define UPDATE_ONLINEFLAG		13	//更新在线标志位信息
#define UPDATE_USERIMAGE		14	//更新用户图片
#define UPDATE_GROUP			15	//更新用户好友分组信息


//语言定义类型
#define LANGUAGE_NORMAL			0
#define LANGUAGE_GB				1
#define LANGUAGE_BIG5			2
#define LANGUAGE_EN				3
#define LANGUAGE_UTF8			4

//定义某一个对象的类型
#define INFO_USER		0
#define INFO_DEPT		1
#define INFO_BOARD		2
#define INFO_GROUP		3
#define INFO_MEETING	4

#define INFO_MOBILE		5

//多重进度条的类型
#define PROGRESS_HORZ		0
#define PROGRESS_VERT		1

//emmis窗口的类型
#define EMMISWND_NORMAL				0
#define EMMISWND_GROUPTITLE			1
#define EMMISWND_GROUP				2

#define EMMISWND_DISPLAYNORMAL		0
#define EMMISWND_DISPLAYLINE		1
#define EMMISWND_DISPLAYLIST		2
#define EMMISWND_DISPLAYEDIT		3


#define XPLISTCTRL_TYPE					0
#define XPLISTCTRL_LISTALLPATHFILE		1
#define XPLISTCTRL_LISTPATHFILE			2

#define CLIP_NULL	0
#define CLIP_COPY	1
#define CLIP_CUT	2

//消息可读否
#define MSGRESP_NO			0
#define MSGRESP_NORMAL		1

#define SRVURL_COMP			0  //公司的主页
#define SRVURL_DOWN			1  //下载地址

#define LCSB_CLIENTDATA				1
#define LCSB_NCOVERRIDE				2

//XP back bmp size
#define	XPBUTTON_WIDTH			38
#define	XPBUTTON_HEIGHT			19

//Tab back bmp size
#define	TABWND_WIDTH			44
#define	TABWND_HEIGHT			15

//控件的五种状态
#define	CTRL_NORMAL			0
#define	CTRL_FOCUS			1
#define	CTRL_DOWN			2
#define	CTRL_DISABLE		3
#define	CTRL_DEACTIVE		4
#define	CTRL_SELECT			5

//dialogbar的类型
#define	DIALOGBAR_NORMAL			0
#define	DIALOGBAR_SEARCHBAR			1

//窗口的图标
#define	WND_BUTTON_MIN				0
#define	WND_BUTTON_NORMAL			1
#define	WND_BUTTON_MAX				2
#define	WND_BUTTON_CLOSE			3

//按钮的大小，相互的间隔
#define	WND_BUTTON_H				19
#define	WND_BUTTON_W				19
#define	WND_BUTTON_D				3

//程序处理错误
#define	PACKSOLVE_SUCCESS			0
#define	PACKSOLVE_NULL				1
#define	PACKSOLVE_UNPACKERROR		2

//一些系统需要的图标
#define HTMLTOOLICON_SEND				0
#define HTMLTOOLICON_CUTSCREEN			2
#define HTMLTOOLICON_ICONFACE			4
#define HTMLTOOLICON_HTMLTXT			6
#define HTMLTOOLICON_SAVE				8
#define HTMLTOOLICON_FONT				10
#define HTMLTOOLICON_TXTCOLOR			12
#define HTMLTOOLICON_BOLD				14
#define HTMLTOOLICON_ITALY				16
#define HTMLTOOLICON_UNDERLINE			18
#define HTMLTOOLICON_BACKGROUNDCOLOR	20
#define HTMLTOOLICON_NUMINDEX			22
#define HTMLTOOLICON_PRJINDEX			24
#define HTMLTOOLICON_LEFT				26
#define HTMLTOOLICON_CENTER				28
#define HTMLTOOLICON_RIGHT				30
#define HTMLTOOLICON_ADDINDENT			32
#define HTMLTOOLICON_DELINDENT			34
#define HTMLTOOLICON_ADDHYPERLINK		36
#define HTMLTOOLICON_CANCELHYPERLINK	38
#define HTMLTOOLICON_CANCELFORMAT		40
#define HTMLTOOLICON_QQ					42
#define HTMLTOOLICON_MSN				44
#define HTMLTOOLICON_SENDFILE			46
#define HTMLTOOLICON_UNCHECK			48
#define HTMLTOOLICON_CHECK				50

typedef enum{
	VISITORTALK_TOOLBAR_SPLITTER,		
		VISITORTALK_TOOLBAR_SORT,		//分类查询
		VISITORTALK_TOOLBAR_HISTORY,	//对话记录
		VISITORTALK_TOOLBAR_UNON,		//按联盟对话
		VISITORTALK_TOOLBAR_SEARCH,		//搜索引擎
		VISITORTALK_TOOLBAR_FRD,		//友情链接
		VISITORTALK_TOOLBAR_USERDEFINE,	//自定义查询
		VISITORTALK_TOOLBAR_COLUMSETUP,
		VISITORTALK_TOOLBAR_OUT
}TOOLBAR_ID;

#define STRING_WARN				"警告"
#define STRING_ERROR			"错误"
#define STRING_QUESTION			"询问"
#define STRING_MSG				"消息"
#define STRING_SYSMSG			"系统消息"
#define STRING_TIPS				"提示"

//图标的类型
#define	ICON_16					16
#define	ICON_32					32
#define	ICON_48					48

//弹出菜单的序号
#define POPUP_NULL				-1
#define POPUP_SENDBTN				0
#define POPUP_SYSTRAY				1
#define POPUP_USERTREEMYSELF		2	//自己的菜单项

#define POPUP_USERTREEUSER			3	//根目录好友的菜单项
#define POPUP_USERTREEUSERTALK		4	//对话的好友项的菜单项

#define POPUP_USERTREEVISITOR			5	//没有对话的访客项
#define POPUP_USERTREEVISITORREQUEST	6	//邀请， 转接中的访客项
#define POPUP_USERTREEVISITORTALK		7	//对话的访客项，包括邀请、转接和对话

#define POPUP_USERTREEBLANK			8
#define POPUP_AUTOINVITE			9
#define POPUP_ALERT					10
#define POPUP_SORTUSER				11
#define POPUP_WNDSETUP				12
#define POPUP_TALKSTATUS_REQUEST    13
#define POPUP_HTMLTOOLBAR		    14

//对象类型，目前可有三种对象
#define OBJECT_UNKNOWN		0
#define OBJECT_USER			1
#define OBJECT_WEBUSER		2 // web用户或者是微信用户

//声音文件的类型
#define SOUND_USERONLINE		0
#define SOUND_USERMSG			1
#define SOUND_SYSMSG			2
#define SOUND_CALL				3
#define SOUND_KEYCLICK			4
#define SOUND_BACK				5
#define SOUND_SUCCESS			6
#define SOUND_FAIL				7
#define SOUND_NUDGE				8
#define SOUND_TIPS				9

//系统进程间相互通知消息
#define EVENT_OFFLINE		0			
#define EVENT_ONLINE		1			
#define EVENT_INSTALLHOOK	2
#define EVENT_UNINSTALLHOOK	3
#define EVENT_DELETETEMP	4
#define EVENT_FINDSRV		5
#define EVENT_RESPSRV		6
#define EVENT_CLOSESYSTEM	10			//关闭系统

//区域的类型
#define RECT_DOCKCENTER		0		//中
#define RECT_DOCKLEFT		1		//左
#define RECT_DOCKRIGHT		2		//右
#define RECT_DOCKLEFTRIGHT	3		//左右
#define RECT_DOCKTOP		1		//上
#define RECT_DOCKBOTTOM		2		//下
#define RECT_DOCKTOPBOTTOM	3		//上下

//区域绘制类型
#define TYPE_TEXT			0
#define TYPE_ICON			1
#define TYPE_ICONTEXT		2
#define TYPE_EDITEXT		3

#define TYPE_SPLITTER		4
#define TYPE_MORE			5

//绘制渐变色类型
#define GRADIENT_TB			0	//normal color - bottom color
#define GRADIENT_BT			1	//bottom color - normal color
#define GRADIENT_TBT		2	//normal - bottom - normal
#define GRADIENT_BTB		3	//bottom - normal - bottom

#define TRIANGLE_LEFT		1		//左
#define TRIANGLE_RIGHT		2		//左
#define TRIANGLE_TOP		3		//左
#define TRIANGLE_BOTTOM		4		//左

#define LISTUSER_BYSTATUS_ALL		0
#define LISTUSER_BYSTATUS_OFFLINE	1
#define LISTUSER_BYSTATUS_ONLINE	2

#define STRING_HTML_META "<meta http-equiv='Content-Type' content='text/html; charset=gb2312'>"
#define STRING_HTML_BASE "<base target='_blank'>"
#define STRING_HTML_STYLE "<style type='text/css'> <!--td{font-size: 12px;}body{OVERFLOW: auto;margin: 5px; padding: 0px 0px 0px 0px; border: 0px;font-family:宋体, sans-serif; font-size: 12px;}p { margin-top:5px;margin-bottom:0px; font-size: 10pt;line-height:17px;}font{line-height:110%;}li { margin-top:5px;margin-bottom:0px; font-size: 10pt;}.sysfont{ font-size: 11pt;color: Salmon;}.operatornamefont{ font-size: 11pt;color: Blue;}.operatorfont{ font-size: 11pt;color: Black;padding-left:8px;}.clientnamefont{ font-size: 11pt;color: Black;}.clientnamefont1{ font-size: 11pt;color: Green; font-weight:bold;}.clientfont{ font-size: 11pt;color: black;padding-left:8px;}.clientfont1{ font-size: 11pt;color: Purple;}.trackfont{ font-size: 10pt;color: DarkCyan;}--></style>"

#define STRING_HISTORYHTML_HEAD1 "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\r\n<base target='_blank'>\r\n\
<script language=\"javascript\">function Display(obj){sid=obj.parentNode.parentNode.parentNode.childNodes[1];\
if (sid.style.display == 'none'){ sid.style.display='';obj.innerText='隐藏详细';}else{ sid.style.display='none';obj.innerText='显示详细';}}\
function SelectTab(selectid,showdiv,showheader,tabcount,isselectbg,isnotselectbg){for(i=0;i<tabcount;i++){if(i==selectid) {document.getElementById(showheader[i]).style.backgroundImage=\"url(\"+isselectbg+\")\";document.getElementById(showdiv[i]).style.display=\"\";}else{document.getElementById(showheader[i]).style.backgroundImage=\"url(\"+isnotselectbg+\")\";document.getElementById(showdiv[i]).style.display=\"none\";}}}\
function ChangeTab(selectid,n){var showdiv=new Array(\"rdcxtable_a_\"+n,\"rdcxtable_b_\"+n,\"rdcxtable_c_\"+n,\"rdcxtable_d_\"+n);var showheader=new Array(\"rdcxtd_a_\"+n,\"rdcxtd_b_\"+n,\"rdcxtd_c_\"+n,\"rdcxtd_d_\"+n);"


#define STRING_HISTORYHTML_HEAD2 "var tabcount=4;SelectTab(selectid,showdiv,showheader,tabcount,isselectbg,isnotselectbg);}\
function killErrors(){return true;}window.onerror = killErrors;</script>\
<style type=\"text/css\"> <!--td{font-size: 12px;}\
body{OVERFLOW: auto;margin: 5px; padding: 0px 0px 0px 0px; border: 0px;font-family:宋体, Arial, Helvetica, sans-serif; font-size: 12px;}\
p { margin-top:5px;margin-bottom:0px; font-size: 10pt;line-height:140%;}\
li { margin-top:5px;margin-bottom:0px; font-size: 10pt;}\
.sysfont{ font-size: 11pt;color: Salmon;}\
.operatornamefont{ font-size: 11pt;color: Blue;}\
.operatorfont{ font-size: 11pt;color: Black;}\
.clientnamefont{ font-size: 11pt;color: Black;}\
.clientnamefont1{ font-family: 宋体;font-size: 11pt;color: Black;}\
.clientfont{ font-size: 11pt;color: black;}\
.clientfont1{ font-size: 11pt;color: Purple;}\
.trackfont{ font-size: 10pt;color: DarkCyan;}--></style></head><body></body></html>"

#define GETINFO_COMING "正在获取数据，请等待..."
#define GETINFO_FAIL "无法获取数据！"
#define GETINFO_SUCC "成功获取数据！"

//选中的对象编辑类型
#define INPLACE_NOEDIT		0
#define INPLACE_EDIT		1
#define INPLACE_PASS		2
#define INPLACE_COMBOX		3
#define INPLACE_DATE		4
#define INPLACE_TIME		5
#define INPLACE_DATETIME	6

#define IDC_INPLACE_COMBOX_EDIT         1002

#define MAX_VISITORSORT_NUM	15
#define MAX_ICONINDEX1STRING_NUM	13

static const char *SORT_VISITOR[] = {"粉桃色类别","橄榄色类别","蝎紫红色类别","黑色类别","黄色类别",\
"灰色类别","绿色类别","青灰色类别","青色类别","深橙色类别",\
"深橄榄色类别","深褐紫红色类别","深黄色类别","深灰色类别","深蓝色类别",\
"深绿色类别","深青灰色类别","深青色类别","深桃红色类别","深紫色类别"};

static const char *ICONINDEX1_STRING[] = {"意向", "需跟进", "已成交", "回访","随便看看", \
"同行", "骚扰", "恶意点击", "多次访问", "深度访问", \
"后续服务", "投诉建议", "其它"};

#define ACCERESP_TYPE_TXT				0
#define ACCERESP_TYPE_URL				1
#define ACCERESP_TYPE_FILE				2

//上传文件类型
#define SENDFILE_FILE		0
#define SENDFILE_IMAGE		1
#define SENDFILE_ACCELRESPFILE		2
#define SENDFILE_UPUSERFILE			3
#define SENDFILE_ICONFILE			4


#define APPLY_UNKNOWN	-1	//请求弹出询问；
#define APPLY_ASK		0	//请求弹出询问；
#define APPLY_OPEN		1	//直接请求对话框；
#define MYGETNOTE		2	//请求用户留言


//邀请类型
#define	REQUSTTYPE_NORMAL					0
#define	REQUSTTYPE_VISITOR					1
#define	REQUSTTYPE_USER						2
#define	REQUSTTYPE_DIRECT					3	//直接对话

#define	INVITE_TYPE_NORMAL					0	//快速邀请
#define	INVITE_TYPE_WELLCOME				1	//带问候语邀请
#define	INVITE_TYPE_DIRECTTALK				2	//直接对话

#define INVITE_TRANSFER			10	//转移用户
#define INVITE_HELP				11	//请求协助
#define INVITE_HELP_REFUSE		12	//请求协助拒绝
#define INVITE_HELP_ACCEPT		13	//请求协助同意

#define MAX_UIN_USER    2300000000
#define WEBUSER_UIN     2200000000U
#define CLIENT_UIN      1000000000U

#define CHATCLOSE_OFFLINE	0
#define CHATCLOSE_TIMEOUT	1
#define CHATCLOSE_USER		2
#define CHATCLOSE_GP		3
#define CHATCLOSE_WEBUI		4
#define CHATCLOSE_UNSUBSCRIBE 6	//微信用户取消关注关闭会话
#define CHATCLOSE_INVISTEXIT 10

#define INVITE_NORMAL		0
#define INVITE_USERDEFINE	1
#define INVITE_DIRECTTALK	2
#define INVITE_OFFLINEMSG	3


#define MAX_FILEDS	24
#define NEED_PARSE_ITEM 8
#define VISITORTALK_MAXLIST_TOTALURL		10
typedef std::list<void *>	PtrList;

#define USER_REPORT_COUNT	10	//最多可定义10个自定义查询
#define USER_REPORT_MAXID	0xFF
#define USER_REPORT_MINID	(USER_REPORT_MAXID-10)	
#define USER_REPORT_MENUID	(USER_REPORT_MAXID-11)	//自定义查询菜单ID
#define	INTERNAL_REPORT_HISTORY		(USER_REPORT_MAXID-12)//对话记录
#define INTERNAL_REPORT_UNON		(USER_REPORT_MAXID-13)//按联盟对话
#define INTERNAL_REPORT_SEARCH		(USER_REPORT_MAXID-14)//搜索引擎
#define INTERNAL_REPORT_FRD			(USER_REPORT_MAXID-15)//友情链接

/** http请求方式 */
#define REQUEST_TYPE_GET 1	/**< get方式 */ 
#define REQUEST_TYPE_POST 2	/**< post方式 */ 

/** 消息发送类型 */
#define MSG_TYPE_NORMAL		1		/**< 普通消息 */ 
#define MSG_TYPE_PREV		2		/**< 预知消息 */

/** 消息数据类型 */
#define MSG_DATA_TYPE_TEXT 1		/**< 文字 */ 
#define MSG_DATA_TYPE_IMAGE 2		/**< 图片 */ 
#define MSG_DATA_TYPE_VOICE 3		/**< 语音 */ 
#define MSG_DATA_TYPE_VIDEO 4		/**< 视频 */ 
#define MSG_DATA_TYPE_LOCATION 5	/**< 位置 */ 
#define MSG_DATA_TYPE_LINK 6		/**< 链接 */ 
#define MSG_DATA_TYPE_EVENT 7		/**< 事件 */
#define MSG_DATA_TYPE_FILE 8		/**< 文件 */

/** web页面标示符 */
#define Handler_WebUrl "WebUrl"	// 历史记录等页面
#define Handler_ListMsg "ListMsg" // 聊天内容显示页面
#define Handler_WebAdmin "WebAdmin" // 管理中心页面
#define Handler_DisplayIamge "DisplayIamge" // 查看大图
#define Handler_VisitorRelated "VisitorRelated" // 访客相关,包括访客信息、访客订单、查询订单、会话登单、聊天信息、辅助应答

/** Js调C++方法名 */
#define Js_Call_MFC_Func_RestartSession "RestartSession"	// 重启二次会话
#define Js_Call_MFC_Func_StartRecord "StartRecord" // 发送录音
#define Js_Call_MFC_Func_SendAudio "SendAudio" // 发送录音
#define Js_Call_MFC_Func_CancelRecord "CancelRecord" // 取消录音
#define Js_Call_MFC_Func_ViewDetails "ViewDetails"  // 查看大图
#define Js_Call_MFC_Func_ReSendFile "ReSendFile" // 重新发送文件
#define Js_Call_MFC_Func_ReRecvFile "ReRecvFile" // 重新接收文件
#define Js_Call_MFC_Func_ChangeChatObject "ChangeChatObject" // 切换聊天对象
#define Js_Call_MFC_Func_ReSendMsg "ReSendMsg" // 重新发送文字消息或json串消息

/** Format请求的url的类型名 */
#define Request_Url_RestartSession "RestartSession"	// 重启二次会话
#define Request_Url_AccessToken "AccessToken" // 获取微信公众号token

/** 聊天用户的类型区分 */
#define  MSG_FROM_CLIENT	1		// 坐席用户
#define  MSG_FROM_USER		2		// 微信或web用户
#define  MSG_FROM_ASSIST	3		// 协助对象
#define  MSG_FROM_SYS		4		// 系统提示消息

// 录音操作的返回码
#define CODE_AUDIO_SUCCESS 0 // 成功
#define CODE_AUDIO_NO_DEVICE 1 // 没有录音设备
#define CODE_AUDIO_FAIL 2 // 失败

// 定时器名字
#define TIMER_NAME_NORMAL "TIMER_NORMAL"
#define TIMER_NAME_LOGIN "TIMER_LOGIN"



#define FACE_PREFIX "<IMG src=\""
#define FACE_ALT ".gif\" alt=\""
#define FACE_SUFFIX "\" />"

#endif
