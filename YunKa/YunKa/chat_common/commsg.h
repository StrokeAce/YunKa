#ifndef _COMMDEF_COMMMSG
#define _COMMDEF_COMMMSG

//定时器的类型
#define TIMER_SYSTEMMSG		1	//系统消息来后，闪烁在状态条上图标
#define TIMER_TRAYMSG		2	//消息来后，闪烁在托盘上图标
#define TIMER_AFTERINIT		3	//窗口显示后马上调用，以避免窗口等待
#define TIMER_COMMMAXTIME	4	//通讯最大的时间限制
#define TIMER_SENDUDP		5	//发送UDP包的时间间隔
#define TIMER_NORMAL		6	//间隔半秒的定时器，用于ping mutiping idleoffline
#define TIMER_EXIT			7	//退出系统的定时器
#define TIMER_POPUPMENU		9	//处理菜单弹出
#define TIMER_MEETINGPING	10	//会议用户不停的发在线信息给服务器
#define TIMER_STAERTVIDEO	11	//启动视频，以避免窗口等待
#define TIMER_TOPWND		12	//最前面窗口消失时间
#define TIMER_PROXY			14	//监测代理服务器的
#define TIMER_LOGINOFF_TO	15	//登出后再登陆、注册
#define TIMER_AUTO_HIDE		16	//到边界时自动隐藏
#define TIMER_CONNECTMAXTIME	17	//一个连接最大的时间限制
#define TIMER_FIRSTONLINE	18	//第一次用户上线闪烁文字
#define TIMER_SENDVIDEO		19	//间隔一秒的定时器，用于ping mutiping idleoffline
#define TIMER_AUTOLOGIN		20	//自动登录
#define TIMER_DOWNLISTEN	50	//文件下载服务器监听等待时间定时器
#define TIMER_VISITOR_ACCEPT 80
#define TIMER_TRANS_TIMEOUT 81 //转接会话超时处理 gxl
#define TIMER_REFRESH_WEBPAGE 82 // 刷新消息记录等页面
#define TIMER_MAILCHECK		60	//邮件监测定时器 60 -- 70  

//定时器间隔
#define TIME_NORMALTIME			500
//一次通讯最大的时间
#define TIME_COMMMAXTIME		8000
//窗口初始化后马上调用的时间
#define TIME_AFTERINIT			100  
//ping的时间间隔
#define TIME_PING				40000 
//relogo的时间间隔
#define TIME_AUTOLOGON			30000 
//mutiping的时间间隔
#define TIME_MUTIPING			5000  
//关闭会议数据获取线程的定时器
#define TIME_MEETINGCLOSE		3000
//一些提示窗口的停留时间
#define TIME_TIPBOX				3000

//自动离线的定时器, 20秒一次，每一次计时，如果到3，并且1分钟后离线，则离线
#define TIME_IDLEOFFLINE		20000	

#define TIME_PROXY				30000
//定义的消息


//socket读失败
#define	WM_SOCKET_RECVFAIL			(WM_USER + 10)	
#define	WM_SOCKET_RECVSUCC			(WM_USER + 20)
#define	WM_SOCKET_CLOSE				(WM_USER + 30)
#define WM_CLOSE_WND				(WM_USER + 40)

//登陆成功了
#define	WM_COMM_LOGINSUCC			(WM_USER + 60)

#define WM_HTML_TOOLBAR				 (WM_USER + 70)	
#define WM_TOOLBAR_MSG				 (WM_USER + 80)	

//访客接待socket
#define	WM_SOCKETEX_RECVFAIL			(WM_USER + 11)	
#define	WM_SOCKETEX_RECVSUCC			(WM_USER + 21)
#define	WM_SOCKETEX_CLOSE				(WM_USER + 31)

//
#define WM_TRANSFERFILE_PROGRESS	(WM_USER + 80)
#define WM_TRANSFERFILE_FINISH		(WM_USER + 90)
#define WM_TRANSFERFILE_GETSESSION	(WM_USER + 100)

#define WM_PARSE_XMLDATA			(WM_USER + 110)

#define WM_KEYWORD_EDIT				(WM_USER + 120)

#define	WM_NAVIGATE_BEFORE			(WM_USER + 130)
#define WM_NAVIGATE_BEFORE2			(WM_USER + 135)
//#define WM_INPLACE_NOTIFY			(WM_USER + 140)
#define WM_CHECKFALSHITEM			(WM_USER + 140)
#define WM_GETTIPSINFO_TREE			(WM_USER + 150)
#define WM_TREECOLOR_CHANGE			(WM_USER + 160)
/*
//系统消息来后，闪烁在状态条上图标
#define WM_OPENSYSTEM			(WM_USER + 200)
#define WM_LOGIN_SERVER			(WM_USER + 210)
*/
//接收到包并解析后，返回到窗口
#define WM_RESPONSE_DATA		(WM_USER + 220)
/*
//发送数据
#define WM_SEND_DATA			(WM_USER + 230)
//接收到回应包，表示包发送成功
#define WM_SEND_SUCCESS			(WM_USER + 240)
//发送数据失败
#define WM_SEND_FAIL			(WM_USER + 250)
//接收包
#define WM_RECV_DATA			(WM_USER + 260)
//接收到更新信息的包(从服务器获取)
#define	WM_UPDATE_USERINFO		(WM_USER + 270)
//接收到修改信息的包(修改到服务器)
#define	WM_CHANGE_USERINFO		(WM_USER + 280)

//在共享文件管理器中改变共享文件的显示
#define	WM_CHANGE_SHAREFILE		(WM_USER + 200)
//文件下载中分类对象的改变
#define	WM_DOWNFILE_SORT		(WM_USER + 210)
//文件下载中进度改变
#define	WM_DOWNFILE_PROGRESS	(WM_USER + 220)
//文件列表中删除
#define	WM_DOWNFILE_DELETET		(WM_USER + 230)

//在消息管理器中改变某一个好友的显示
#define	WM_CHANGE_UIN_MANAGER	(WM_USER + 250)
//在消息管理器中改变某一个组的显示
#define	WM_CHANGE_GROUP_MANAGER	(WM_USER + 260)
//在消息管理器中改变某一个内容项的显示
#define	WM_MSGMEMO_CHANGE		(WM_USER + 270)
//在消息管理器中改变某一个选择项的显示
#define	WM_MSGITEM_CHANGE		(WM_USER + 280)
//在消息管理器中改变宽度
#define	WM_MSGITEM_CHANGECOUMNE	(WM_USER + 290)
*/
//托盘发送的消息
#define	WM_MSG_NOTIFYICON		(WM_USER + 300)
/*
//消息交流区域的工具条
#define	WM_MSG_TALKMSGTOOLBAR	(WM_USER + 310)
*/
//选中了一个图标
#define	WM_MSG_ICONFACE			(WM_USER + 310)
#define	WM_MSG_KILLFOCUS		(WM_USER + 320)

/*
//字体改变消息
#define	WM_MSG_FONT				(WM_USER + 321)
//字号改变的消息
#define	WM_MSG_SIZE				(WM_USER + 322)
//颜色改变的消息
#define	WM_MSG_COLOR			(WM_USER + 323)
//转发消息
#define	WM_MSG_RESEND			(WM_USER + 324)
//转发客人
#define	WM_MSG_TRANSFERCLIENT	(WM_USER + 325)
//发送截屏文件的消息
#define	WM_MSG_SENDSCREEN		(WM_USER + 326)

//键盘鼠标活动消息
#define	WM_MOURSEKEYBOARDE_ACTIVE	(WM_USER + 350)

//键盘鼠标活动消息
#define	WM_TOPWND_RESULT		(WM_USER + 360)	//最前面窗口的选中对象

//hyperlink的消息
#define	WM_HYPERLINK_SEND				(WM_USER + 500)

//外部添加多个联系人的的消息
#define	WM_ADDFRD_BYEXT					(WM_USER + 600)
*/
//tabctrl定义的消息
#define	WM_TABCTRL_LBUTTONDOWN			(WM_USER + 700)
//em tab window定义的消息
#define	WM_EMTAB_DOWN					(WM_USER + 710)

//em mis 
#define	WM_EMMIS_DOWN					(WM_USER + 740)
#define WM_EMMIS_DROPDOWN				(WM_USER + 741)
#define	WM_EMMIS_OUTOFWND				(WM_USER + 742)

/*
#define	WM_VOLUME_MUTE					(WM_USER + 742)
#define	WM_VOLUME_SIZE					(WM_USER + 743)

#define WM_RECORD_RESULT				(WM_USER + 750)
#define WM_RECORD_START					(WM_USER + 751)
#define WM_RECORD_BLOCK					(WM_USER + 752)
#define WM_RECORD_STOP					(WM_USER + 753)

#define WM_RECORD_VIDEO					(WM_USER + 755)
#define WM_RECORD_AUDIO					(WM_USER + 756)

//change face定义的消息
#define	WM_CHANGE_FACE					(WM_USER + 800)

//tips对话框清除
#define	WM_DELETE_TIPS					(WM_USER + 810)

#define WM_MSG_TALKFUNC					(WM_USER + 820)
#define WM_MSG_EDITLINK					(WM_USER + 830)

#define WM_PTPCONNECT_SUCCESS			(WM_USER + 850)

#define WM_VIDEO_ATTR					(WM_USER + 860)

#define WM_FULLWNDTITLE_BTN				(WM_USER + 870)

#define WM_WND_HIDE						(WM_USER + 880)
#define WM_WND_SHOW						(WM_USER + 881)

//会议消息
#define WM_MEETING_DOWN					(WM_USER + 890)
//用户消息
#define WM_USER_DOWN					(WM_USER + 900)

#define WM_EXSTATIC_DOWN				(WM_USER + 910)

#define WM_SENDRECVVIDEO_FAIL			(WM_USER + 930)
*/
//#define WM_CLOSE_USER					(WM_USER + 950)

//向服务器发送连接包的回复
//#define WM_CONNECT_SERVER				(WM_USER + 960)

//xp编辑框处理消息
#define WM_XPEDIT						(WM_USER + 970)

//#define WM_MOUSE_DBCLICK				(WM_USER + 980)
//被邀请用户离开，或者主持人暂时不收消息（后者以后再加）
//#define WM_TALK_USERLEAVE				(WM_USER + 990)

#define WM_USERTREEITEM_SELECT			(WM_USER + 1000)
#define WM_USERTREEITEM_DBCLICK			(WM_USER + 1001)

#define ID_TRAY_EXIT_EX                 32950

#endif