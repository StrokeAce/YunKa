#if !defined _COMMTRAN_EM
#define _COMMTRAN_EM

//程序使用的命令
#define CMD_COM_UNKNOWN						0x0000	// 0 预留
//系统程序处理的一些命令
#define CMD_COM_SENDFAIL					0x0001	// 1 发送失败
#define	CMD_COM_TIMEOUT						0x0002	// 2 通讯超时
#define	CMD_COM_PACKERROR					0x0003	// 3 解包打包错误
#define	CMD_COM_SOCKINITERROR				0x0004	// 4 sock初始话错误
#define	CMD_COM_IPERROR						0x0005	// 5 非法的ip


/*************************************************************************/
/*******              		命令定义		******************/
/*************************************************************************/

//一些通用的命令
#define CMD_COM_ACK							0x000A	// 10 收到包后回应
#define CMD_COM_ACKEX						0x000B	// 11 扩充的回应，服务器收到后，必须发送一个ack包给另一个用户。

//该命令没有包结构
#define CMD_COM_PING						0x042E  // 1070 用户PING服务器
#define CMD_COM_MULTIPING					0x0442  // 1090 局域网用户不停刺激

//用于联接的命令
#define CMD_COM_P2PCONNECTNAT				0x1388  // 5000	用于联接的测试连接

//服务器为主的命令
#define CMD_SRV_ERROR_CLTSEND				0x0000	// 0  客户端端传送一个错误的命令
#define CMD_SRV_RESP_FAIL					0x0014	// 20 服务器回应处理某一个包失败
#define CMD_SRV_SERVER_COPY					0x001E	// 30 服务器运行标志和客户端不一样
#define CMD_SRV_CONF_LOGOFF					0x0028	// 40 确认某人离线
#define CMD_SRV_SERVER_BUSY					0x003C	// 60 服务器忙

#define CMD_SRV_CONF_REG					0x0046	// 70 确认注册新用户
#define CMD_SRV_DENY_REG					0x0050	// 80 拒绝注册新用户
#define CMD_SRV_CONF_LOGON					0x005A	// 90 确认用户登录
#define CMD_SRV_DENY_LOGON					0x0064	// 100 拒绝用户登录

//old
#define CMD_SRV_STATUS_FRDONLINE			0x006E	// 110 通知我我的朋友上线
#define CMD_SRV_STATUS_FRDOFFLINE			0x0078	// 120 通知我我的朋友离线
#define CMD_SRV_STATUS_USER					0x01A4	// 130 用户在线状态

#define CMD_SRV_SEARCH_SUCC					0x008C	// 140 查询朋友结果
#define CMD_SRV_SEARCH_FAIL					0x0096	// 150 查询朋友失败

#define CMD_SRV_UPDATE_SUCC					0x00C8	// 200 更新成功
#define CMD_SRV_UPDATE_FAIL					0x00D2	// 210 更新失败
#define CMD_SRV_SEND_MSG_OFFLINE			0x00DC	// 220	离线消息

#define CMD_SRV_REP_USERINFO				0x0118	// 280 返回用户信息
#define CMD_SRV_REP_SMS						0x0122	// 290 返回短信收到的信息
#define CMD_SRV_REP_IDNAME					0x0230	// 300 返回某一些用户的简单信息

//old
#define CMD_SRV_STATUS_USER_FORM			0x01A4	// 420 用户在线状态

#define CMD_SRV_REP_TRANSFERCLIENT			0x01B8  // 440 转移临时用户成功失败
#define CMD_SRV_TRANSFERCLIENT				0x01C2  // 450 通知接收者，转移临时用户

#define CMD_SRV_LIST_URL					0x01FE	// 510 WebOA设置
#define CMD_SRV_LIST_DEPARTMENT				0x0208	// 520 部门的列表
#define CMD_SRV_LIST_FRD					0x0212	// 530 所有朋友列表
#define CMD_SRV_LIST_ONLINEFRD				0x0226  // 550 在线朋友列表

//关于模仿大连钟氏的一些会话处理命令
#define CMD_FLOAT_CREATECHAT				0x0230	// 560 创建广播会话对象 webui->GP->C
#define CMD_FLOAT_LISTCHAT					0x0231	// 561 在线会话列表    C->GP
#define CMD_FLOAT_CHATMSG					0x0232	// 562 广播到漂浮框指定的人员webui<->GP<->C
#define CMD_FLOAT_CHATMSG_ACK				0x0233	// 563 GP对发生的CMD_FLOAT_CHATMSG进行ack  GP->C
#define CMD_FLOAT_STARTORSTOP_MSG			0x0234	// 564 客户端通知服务器广不广播消息到客户端C->GP
#define CMD_FLOAT_ACCEPTCHAT				0x0235	// 565 事件类型0：客服接受会话 1：会话转移；webui->GP<-C
#define CMD_FLOAT_TRANSQUEST				0x0236	// 566 会话邀请请求GP<-C
#define CMD_FLOAT_TRANSFAILED				0x0237	// 567 会话邀请超时失败GP->C
#define CMD_FLOAT_CLOSECHAT					0x0238	// 568 会话终止包     webui->GP<-C
#define CMD_FLOAT_GETCHATMSG				0x0239	// 569 获取会话消息   C->GP
#define CMD_FLOAT_INVITE_REQUEST			0x023A	// 570 邀请某些人进入会话，只能邀请同一管理员下面的客服
#define CMD_FLOAT_INVITE_RESULT				0x023B	// 571 被邀请方返回邀请结果
#define CMD_FLOAT_RELEASE				0x023C	// 572 客服应答消息超时        GP->C
#define CMD_FLOAT_CMDERROR					0x023F	// 575 获取会话中的消息      GP->C
#define CMD_FLOAT_CHATINFO					0x023D	// 573 会话组详细信息
#define CMD_FLOAT_SHARELIST					0x0250	// 592 获取可分享消息对象列表（友好对象列表）

#define CMD_SRV_GROUP_LIST					0x02C6	// 710 返回组列表 类似CMD_SRV_LIST_GROUP,(0，1，2)；
#define CMD_SRV_GROUP						0x02D0	// 720 返回组内容,(0，1，2)；
#define CMD_SRV_GROUP_NOTICE				0x02DA  // 730 变更组主持人、视频分配等的通知,(0，2)；

//客户端为主的命令
#define CMD_CLT_LOGON						0x03E8  // 1000 用户登录
//客户端的包的处理
//old
#define CMD_CLT_SEND_ALLMSG					0x03F2  // 1010 发送群呼消息

#define CMD_CLT_REG							0x03FC  // 1020 用户注册
#define CMD_CLT_LOGOFF						0x0438  // 1080 用户离线

#define CMD_CLT_SEARCH_FRD					0x041A  // 1050 用UIN查询
#define CMD_CLT_SEARCH_MUTIFRD				0x0424  // 1060 用信息查询

//new
#define CMD_CLT_GET_INFO					0x044C  // 1100 获得服务器的某一些信息资料(包括列表信息)

//old
#define CMD_CLT_GET_CLT_INFO				0x0460  // 1120 获得个人资料

#define CMD_CLT_CHECKPROXY					0x0474  // 1140 检查代理服务器
#define CMD_CLT_TRANSFERCLIENT				0x047E  // 1150 转移临时用户

//old
#define CMD_CLT_UPDATE_STATUS				0x04D8  // 1240 更新状态
#define CMD_CLT_UPDATE_INFO					0x0500  // 1280 更新用户资料
#define CMD_CLT_UPDATE_IMAGE				0x050A  // 1290 更新个人图片
#define CMD_CLT_UPDATE_PW					0x0514  // 1300 更新密码
#define CMD_CLT_UPDATE_LOGINSTRING			0x051E  // 1310 更新登录的名称字符串

#define CMD_CLT_GROUP_INVITE				0x05A0	// 1440 邀请你加入组,(0，2)；保留


//客户端服务器都要使用的命令
#define CMD_COM_SEND_MSG					0x0F00	// 3840 用户发送留言
#define CMD_CLT_SEND_SMS					0x0F28  // 3880 发送短信

#define CMD_COM_AUTH_NEED					0x0FC3  // 3900 对方需要认证
#define CMD_COM_AUTH_RESP					0x0F46	// 3910 回复验证信息, 是否同意
#define CMD_COM_ADD_GROUP                   0x0F4F  // 3920 客户端增加组，删除组，更改组,更好友所在的组fendjz
#define CMD_COM_ADD_FRD						0x0F50  // 3920 增加删除某人

#define CMD_SRV_ALL_FRIEND_OFFLINE			0x1100	// 4352 设置所有联系人都离线,用于轻量登陆,zht

//文件共享的命令
#define CMD_COM_FILE_GETLIST				0x0F64  // 3940	客户端请求发送共享文件列表
//old 
#define CMD_COM_FILE_GET					0x0F6E  // 3950 客户端发送请求文件,可能需要身份验证
#define CMD_COM_FILE_SENDLIST				0x0F78  // 3960 客户端发送共享文件列表
//old
#define CMD_COM_FILE_SEND					0x0F8C  // 3980 客户端请求发送某一个文件
#define CMD_COM_SCREEN						0x0F96	// 3990 发送解图

//视频音频邀请命令
#define CMD_COM_VIDEOVOICE_INVITE			0x0FA0	// 4000	邀请浏览视频，声音
#define CMD_COM_VIDEOVOICE_GET				0x0FAA  // 4010	浏览某人的视频，听取某人声音

//视音频包使用
#define CMD_COM_VIDEOVOICE					0x0FB4  // 4020	视频音频的通讯包

//视音频包使用旧
#define CMD_COM_VIDEO_FORM					0x109A  // 4250	 
#define CMD_COM_AUDIO_FORM					0x10A4  // 4260	 
#define CMD_COM_VIDEOTEXT_FORM				0x10AE  // 4270	
#define CMD_COM_GETPORT						0x10B8  // 4280	在局域网接入时，处理端口
#define CMD_COM_SENDPORT					0x10C2  // 4290	在局域网接入时，发送本地端口给其他用户
#define CMD_COM_HANG						0x10CC  // 4300	挂断连接

//文件共享的命令
#define CMD_COM_FILE_START					0x1194  // 4500 客户端请求发送/下载某一个文件(第一次、继续和停止)
#define CMD_COM_FILE_SENDATTR				0x119E  // 4510 发送文件的一些属性，包括名称、日期、大小等
#define CMD_COM_FILE_SENDFCT 				0x11A8  // 4520 发送文件的校验表
#define CMD_COM_FILE_SENDDATA 				0x11B2  // 4530 发送文件的数据
#define CMD_COM_FILE_SENDEND 				0x11BC  // 4540 结束一个文件块的传输
#define CMD_COM_FILE_RESP 					0x11C6  // 4550 接收方收到4510,4520,4530,4540四个命令后的回复。


#define CMD_COM_UPDATE_FRDLIST				0x2000	// 8192	成批的更新
#define CMD_COM_SENDINVITE					0x2010	// 8208	邀请
#define CMD_COM_RESPINVITE					0x2020	// 8224	回复

#define CMD_EVENT_ANNOUNCEMENT				0x7fff  // 32767 事件通告命令字 djz 2010-3-24

#define CMD_COM_AP_LIST						0x8100	// 33024 获取以及返回AP服务器列表
#define CMD_COM_MANY_USER_INFO				0x8110	// 33040 批量获取用户信息

#endif