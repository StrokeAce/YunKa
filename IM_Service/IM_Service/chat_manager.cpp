#include "stdafx.h"
#include "chat_manager.h"
#include "./common/commclt.h"
#include "./common/commsrv.h"

#define CLIENTVERSION	20000

CChatManager::CChatManager(IBaseMsgs* iBaseMsgs)
{
	m_isLoginSuccess = false;
	m_login = new CLogin();
	m_login->m_manager = this;
	m_chatObjects = NULL;
	m_baseMsgs = iBaseMsgs;

	m_Socket.SetReceiveObject(this);

	LoadINIResource();
}

CChatManager::~CChatManager()
{
}


CSysConfig* CChatManager::GetSysConfig()
{
	return NULL;
}


CMsgs* CChatManager::GetMsgs()
{
	return NULL;
}

void CChatManager::ScreenCapture()
{

}

CChatObjects* CChatManager::GetChatObjects()
{
	if (m_isLoginSuccess)
	{
		m_chatObjects = new CChatObjects();
		m_chatObjects->m_login = m_login;
		m_login->m_Socket.SetReceiveObject(m_chatObjects);
	}
	return m_chatObjects;
}

void CChatManager::GetPreLoginInfo(tstring& loginName, tstring& password, bool& isAutoLogin, bool& isKeepPwd)
{
	loginName = m_sysConfig->lastloginuid;
	password = m_sysConfig->lastloginpass;
	isAutoLogin = m_sysConfig->bAutoLogin;
	isKeepPwd = m_sysConfig->bSavePass;
}

void CChatManager::StartLogin(tstring loginName, tstring password, bool isAutoLogin, bool isKeepPwd)
{
	m_login->StartLogin(loginName, password, isAutoLogin, isKeepPwd);
}

bool CChatManager::ReadSystemConfig()
{
	char sFile[MAX_256_LEN];
	tstring strFile = GetCurrentPath();

	sprintf(sFile, "%sconfig.dat", strFile.c_str());

	if (m_sysConfig->LoadData(sFile, CLIENTVERSION))
	{
	}
	else
	{
		SetSystemConfigByInitconfig();
	}

	m_sysConfig->strVisitorServer = m_InitConfig.sVisitorServer;
	m_sysConfig->nVisitorServerPort = m_InitConfig.nVisitorServerPort;
	return false;
}

void CChatManager::SetSystemConfigByInitconfig()
{
	int i;

	m_sysConfig->strServer = m_InitConfig.sDefaultServer;
	m_sysConfig->nServerPort = m_InitConfig.nDefaultPort;

	for (i = 0; i<10; i++)
	{
		if (strlen(m_InitConfig.sListServer[i]) > 0)
		{
			m_sysConfig->m_listServerAddress.push_back(m_InitConfig.sListServer[i]);
		}
	}

	m_sysConfig->filtertype = m_InitConfig.visitorfilter;
	SolveUserdefineFilter(m_sysConfig->infofilter, m_InitConfig.infofilter);
}

void CChatManager::SolveUserdefineFilter(FILTER_USERDEFINE &filter, char *sfilter)
{
	if (sfilter == NULL)
		return;

	char sbuff[MAX_256_LEN];

	char seps[] = ",";
	char *token;
	int i;

	memset(&filter, '\0', sizeof(FILTER_USERDEFINE));
	strncpy(sbuff, sfilter, MAX_256_LEN);
	token = strtok(sbuff, seps);
	i = 0;
	while (token != NULL)
	{
		if (i == 0)
		{
			filter.include = atol(token);
		}
		else
		{
			strncpy(filter.text[i - 1], token, MAX_STRINGFILTER_LEN - 2);
			filter.num++;
		}
		if (i > MAX_STRINGFILTER_NUM)
			break;

		token = strtok(NULL, seps);
		i++;
	}

	return;
}

void CChatManager::OnReceive(void* pHead, void* pData)
{
	int nError = 0;
	TCP_PACK_HEADER TcpPackHead;

	int nPackHeadLen = COM_HEAD_PACK::GetHeadLen();
	int nTcpPackHeadLen = sizeof(TCP_PACK_HEADER);
	int nErrType = 0;

	TcpPackHead = *((TCP_PACK_HEADER *)pHead);
	assert(TcpPackHead.len <= PACKMAXLEN);

	char *RecvBuf = (char*)pData;

	COM_HEAD_PACK Head;
	Head.head = *((PACK_HEADER*)RecvBuf);
	g_WriteLog.WriteLog(C_LOG_TRACE, "logondlg recv Cmd:%.4x", Head.head.cmd);

	switch (Head.head.cmd)
	{
	case CMD_SRV_CONF_LOGON: // 登录确认
		nError = RecvSrvConfLogon(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		if (nError != 0)
		{
			goto FAIL;
		}
		break;
	case CMD_SRV_REP_USERINFO: // 用户信息包
		nError = RecvSrvRepUserinfo(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);

		// 保存登录信息
		if (nError != 0)
		{
			goto FAIL;
		}
		break;
	case CMD_SRV_DENY_LOGON: // 拒绝登录
		nError = RecvSrvDenyLogon(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen, nErrType);

		if (nError != 0)
		{
			goto FAIL;
		}
		else
		{
			nError = COMM_DENYLOGIN_SEG + nErrType;
			goto FAIL;
		}
		break;
	case CMD_SRV_CONF_LOGOFF:   // 登出确认包
		nError = RecvSrvConfLogOff(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_FRDONLINE: // 好友上线
		nError = RecvSrvStatusFrdOnline(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_FRDOFFLINE: // 好友下线
		nError = RecvSrvStatusFrdOffline(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_USER_FORM: // 好友更改在线状态
		nError = RecvSrvStatusUserForm(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_SHARELIST: // 收到好友对象列表
		nError = RecvFloatShareList(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_COM_SEND_MSG:
		nError = RecvComSendMsg(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CREATECHAT: // 接入会话
		nError = RecvFloatCreateChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATMSG: // 会话消息
		nError = RecvFloatChatMsg(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATMSG_ACK: // 发送消息应答
		nError = RecvFloatChatMsgAck(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_ACCEPTCHAT: // 接受会话
		nError = RecvFloatAcceptChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_TRANSQUEST: // 等待应答  会话转移请求
		nError = RecvFloatTransQuest(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_TRANSFAILED:   // 等待应答  会话转移失败
		nError = RecvFloatTransFailed(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_INVITE_REQUEST:// 等待应答  邀请请求
		nError = RecvInviteRequest(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_INVITE_RESULT:// 等待应答  邀请结果
		nError = RecvInviteResult(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_KEFU_RELEASE: // 坐席主动释放会话
		nError = RecvFloatKefuRelease(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CMDERROR: // 等待应答  命令失败
		nError = RecvFloatCMDError(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CLOSECHAT: // 会话关闭
		nError = RecvFloatCloseChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_LISTCHAT: //会话列表
		nError = RecvFloatListChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATINFO: // 会话详细信息
		nError = RecvFloatChatInfo(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_EVENT_ANNOUNCEMENT: // 收到消息通告
		nError = RecvEventAnnouncement(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_UPDATE_SUCC: //更新信息成功
		nError = RecvSrvUpdateSucc(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_UPDATE_FAIL: // 更新信息失败
		nError = RecvSrvUpdateFail(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_SERVER_COPY: // 用户在异地登陆
		nError = RecvSrvDonw(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_REP_TRANSFERCLIENT: // 440 转移临时用户成功失败
		nError = RecvRepTransferClient(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_TRANSFERCLIENT: // 接收转移会话的用户处理
		nError = RecvTransferClient(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	default:
		break;
	}

	return;

FAIL:
	//ConnectFail(nError);
	return;
}

int CChatManager::RecvSrvConfLogon(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_CONF_LOGON RecvInfo(packhead.ver);
	int nError = 0;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		return nError;
	}

	m_sysConfig->strServer = m_server;
	m_sysConfig->nServerPort = m_port;

	m_sysConfig->strRealServer = m_server;
	m_sysConfig->nRealServerPort = m_port;

	m_UserInfo.UserInfo.uid = RecvInfo.uin;
	strcpy(m_UserInfo.UserInfo.sid, RecvInfo.strid);
	strcpy(m_UserInfo.UserInfo.pass, m_password.c_str());

	//服务器返回的RecvInfo.type_loginby为0有误
	m_sysConfig->lastloginby = LOGIN_BYSTRING;
	if (strlen(RecvInfo.strid) <= 0)
		strcpy(m_sysConfig->lastloginstr, m_loginName.c_str());
	else
		strcpy(m_sysConfig->lastloginstr, RecvInfo.strid);
	strcpy(m_sysConfig->lastloginpass, m_password.c_str());


	m_sysConfig->AddServerAddress(m_sysConfig->strServer);

	m_usSrvRand = RecvInfo.m_Head.head.random;
	nError = 0;

	char lognmame[100] = { 0 };
	sprintf(lognmame, "_%s(%u)_emocss.log", RecvInfo.strid, RecvInfo.uin);
	g_WriteLog.InitLog(GetCurrentPath() + "log", lognmame, C_LOG_ALL);

	return nError;
}

int CChatManager::RecvSrvRepUserinfo(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_REP_USERINFO RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		goto RETURN;
	}

	// 下载登陆用户的头像

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype)
{
	m_Socket.Close();

	SRV_DENY_LOGON RecvInfo(packhead.ver);
	int nError = 0;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		goto RETURN;
	}

	errtype = RecvInfo.deny;

RETURN:

	return nError;
}

int CChatManager::UnPack(CPackInfo *pPackInfo, char *buff, int len)
{
	int nError = 0;

	if (pPackInfo == NULL)
	{
		nError = SYS_ERROR_MEMORY;
		goto RETURN;
	}

	pPackInfo->m_Pack.Clear();

	pPackInfo->m_Pack.CopyBuff((unsigned char *)buff, len, 0);

	if (!(pPackInfo->unpack()))
	{
		nError = SYS_ERROR_UNPACKINFO;
		goto RETURN;
	}

	nError = 0;

RETURN:
	return nError;
}

bool CChatManager::LoadINIResource()
{
	char sFile[MAX_256_LEN], sKey[MAX_256_LEN];

	int i, len = MAX_256_LEN;

	sprintf(sFile, "%sTQConfig.ini", GetCurrentPath().c_str());

	LoadIniString("common", "programe name mini", m_InitConfig.sProgrameNameMini, len, sFile, NULL);

	LoadIniString("common", "help url", m_InitConfig.sHelpUrl, len, sFile);

	LoadIniString("common", "copy id", m_InitConfig.sCopyID, len, sFile);

	for (i = 0; i < 3; i++)
	{
		sprintf(sKey, "help about %d", i + 1);
		LoadIniString("common", sKey, m_InitConfig.sHelpAbout[i], len, sFile);

		sprintf(sKey, "help about url %d", i + 1);
		LoadIniString("common", sKey, m_InitConfig.sHelpAboutURL[i], len, sFile);
	}

	LoadIniInt("common", "display username type", m_InitConfig.nDisplayUserNameType, sFile);

	LoadIniString("server", "address", m_InitConfig.sDefaultServer, len, sFile);

	LoadIniInt("server", "port", m_InitConfig.nDefaultPort, sFile);

	LoadIniString("visitor", "address", m_InitConfig.sVisitorServer, len, sFile);

	LoadIniInt("visitor", "port", m_InitConfig.nVisitorServerPort, sFile);

	int temp(0);
	LoadIniInt("visitor", "ZlibEnabled", temp, sFile);
	m_InitConfig.bZlibEnabled = (1 == temp);

	for (i = 0; i < 10; i++)
	{
		sprintf(sKey, "list %d", i + 1);
		LoadIniString("server", sKey, m_InitConfig.sListServer[i], len, sFile);
	}

	LoadIniInt("login", "save pass", m_InitConfig.bSavePass, sFile);
	LoadIniInt("login", "auto login", m_InitConfig.bAutoLogin, sFile);

	//visitor filter
	LoadIniInt("visitor filter", "filter", m_InitConfig.visitorfilter, sFile, "0");
	LoadIniString("visitor filter", "info filter", m_InitConfig.infofilter, len, sFile, "");
	LoadIniString("visitor filter", "info filter", m_InitConfig.tailfilter, len, sFile, "");
	LoadIniString("AuthClient", "AuthAddr", m_InitConfig.sAuthAddr, len, sFile, "passport.tq.cn");
	LoadIniInt("AuthClient", "AuthPort", m_InitConfig.nAuthPort, sFile, "80");
	LoadIniString("WebPages", "logout", m_InitConfig.webpage_lgout, len, sFile, "http://vip.tq.cn/vip/logout.do");
	LoadIniString("WebPages", "webphone", m_InitConfig.webpage_webphone, len, sFile, "http://vip.tq.cn/vip/screenIndex.do?a=1");

	// 查询来电
	LoadIniString("WebPages", "querywebphone", m_InitConfig.webpage_querywebphone, len, sFile, "http://vip.tq.cn/vip/screenPhoneRecord.do?action=query&deal_state=0");

	// 留言
	LoadIniString("WebPages", "note", m_InitConfig.webpage_note, len, sFile, "http://vip.tq.cn/vip/viewLiuyan.do?a=1");

	// 客户管理
	LoadIniString("WebPages", "crm", m_InitConfig.webpage_crm, len, sFile, "http://vip.tq.cn/vip/visitorInfomation.do?a=1");

	// 统计分析
	LoadIniString("WebPages", "analyze", m_InitConfig.webpage_analyze, len, sFile, "http://vip.tq.cn/vip/serviceStutasE.do?a=1");

	// 管理中心
	LoadIniString("WebPages", "mgmt", m_InitConfig.webpage_mgmt, len, sFile, "http://vip.tq.cn/vip/clientIndex.do?a=1");

	// 焦点窗口
	LoadIniString("WebPages", "news", m_InitConfig.webpage_news, len, sFile, "http://vip.tq.cn/vip/rss/tq_mini_loading.jsp?a=");

	// 注册新用户
	LoadIniString("WebPages", "regnewuser", m_InitConfig.webpage_regnewuser, len, sFile, "http://www.tq.cn/vip/prenewqttclt.do");
	LoadIniInt("WebPages", "regnewuser_w", m_InitConfig.webpage_regnewuser_w, sFile, "520");
	LoadIniInt("WebPages", "regnewuser_h", m_InitConfig.webpage_regnewuser_h, sFile, "410");

	// 新手指南
	LoadIniString("WebPages", "tqhelp", m_InitConfig.webpage_tqhelp, len, sFile, "http://www.tq.cn/help_3.html");

	// 统一用语
	LoadIniString("WebPages", "UniIdioms", m_InitConfig.webpage_UniIdioms, len, sFile, "http://vip.tq.cn/vip/GetRequestXMLAction.do?cmd=NEWQUICKREPLY");

	// 实时监控
	LoadIniString("WebPages", "RtmMsg", m_InitConfig.webpage_RtmMsg, len, sFile, "http://vip.tq.cn/vip/realTime.do?a=");

	// 漫游消息
	LoadIniString("WebPages", "SvrMsg", m_InitConfig.webpage_SvrMsg, len, sFile, "http://vip.tq.cn/vip/historyDialog.do?a=");

	// TQ顾问
	LoadIniString("WebPages", "TQadviser", m_InitConfig.webpage_TQadviser, len, sFile, "http://vip.tq.cn/vip/myTqGuWen.do?a=");

	// 上传头像
	LoadIniString("WebPages", "UploadHeadImage", m_InitConfig.webpage_UploadHeadImage, len, sFile, "http://vip.tq.cn/vip/preuploadfacelink.do?a=");

	// 下载头像
	LoadIniString("WebPages", "DownloadHeadImage", m_InitConfig.webpage_DownloadHeadImage, len, sFile, "http://vip.tq.cn/vip/preuploadfacelink.do?a=");

	// 表情
	LoadIniString("WebPages", "FaceImage", m_InitConfig.webpage_FaceImage, len, sFile, "http://vip.tq.cn/clientimages/face/images");

	// 系统消息
	LoadIniString("WebPages", "SysMessage", m_InitConfig.webpage_SysMessage, len, sFile, "http://sysmessage.tq.cn/SysMsg/getmesg.do?strid=%s&uin=%lu&departId=%d");

	// 投诉建议
	LoadIniString("WebPages", "complaint", m_InitConfig.webpage_complaint, len, sFile, "http://211.151.52.62:8081/zhengquan/preLeaveMsg.do?uin=%lu");

	//发送文件
	LoadIniString("WebPages", "SendWebFileLink", m_InitConfig.webpage_SendWebFileLink, len, sFile, "http://vip.tq.cn/upload/upload2.do?version=100");


	LoadIniString("WebPages", "iconurl", m_InitConfig.webpage_iconurl, len, sFile, "http://sysimages.tq.cn/clientimages/face/ywt_face.html");
	LoadIniString("WebPages", "faqInfo", m_InitConfig.webpage_faqInfo, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqInfo");
	LoadIniString("WebPages", "faqSort", m_InitConfig.webpage_faqSort, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqSort");
	LoadIniString("WebPages", "faqAll", m_InitConfig.webpage_faqall, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqAll");
	LoadIniString("WebPages", "repickchat", m_InitConfig.webpage_repickchaturl, len, sFile, "http://106.120.108.230:8031/wxcreatechat?uin=%lu&signature=%s&timestamp=%s&nonce=%s&admiuin=%lu&%s&msg=hello");
	//邀请评价evaluate
	LoadIniString("WebPages", "evaluate", m_InitConfig.webpage_evaluate, len, sFile, "http://106.120.108.230:8524/vip/scorebill.do?billid=%s&admin_uin=%u&action=scoreedit");

	LoadIniString("WebPages", "sendurl", m_InitConfig.webpage_sendurl, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=url");

	//获取公司用户
	LoadIniString("WebPages", "companyuser", m_InitConfig.webpage_companyuser, len, sFile, "http://vip.tq.cn/vip/GetRequestXMLData?cmd=TRANSFER");

	// 获取微信token
	LoadIniString("WebPages", "accesstoken", m_InitConfig.webpage_accesstoken, len, sFile, "http://106.120.108.230:8031/wxkfgetac?uin=%lu&signature=%s&timestamp=%s&nonce=%s&{$MSG_WORKBILL.UserDefineParams}");

	//工单
	LoadIniString("WebPages", "workbillurl", m_InitConfig.webpage_workbillurl, len, sFile, "http://106.120.108.230:8524/vip/workOrder.do?r=%s");

	// 客户履历
	LoadIniString("VisitorPages", "visitortail", m_InitConfig.visitorpage_visitortail, len, sFile, "http://vip.tq.cn/vip/ClientDialog.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");

	//会话登单
	LoadIniString("VisitorPages", "visitorbill", m_InitConfig.visitorpage_visitorbill, len, sFile, "http://vip.tq.cn/vip/recrodworkbill.do?action=prerecord&billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");

	//客户信息
	LoadIniString("VisitorPages", "visitorinfo", m_InitConfig.visitorpage_visitorinfo, len, sFile, "http://vip.tq.cn/vip/visitorinfo.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");
	//下订单
	LoadIniString("VisitorPages", "visitororder", m_InitConfig.visitorpage_visitororder, len, sFile, "http://106.120.108.230:8524/vip/visitorinfo.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s&clientid=%s&type=0");

	//通告信息
	LoadIniString("VisitorPages", "visitornotice", m_InitConfig.visitorpage_notice, len, sFile, "http://www.tq.cn");
	LoadIniString("WebPages", "RegisterAccount", m_InitConfig.registerAccount, len, sFile, "http://www.tq.cn");
	LoadIniString("WebPages", "ForgetPassword", m_InitConfig.forgetPassword, len, sFile, "http://www.tq.cn");
	LoadIniInt("WebPages", "ForgetPSVisible", m_InitConfig.forgetPSVisible, sFile);

	// 查询历史记录
	LoadIniString("WebPages", "QueryHistoryRecords", m_InitConfig.query_history_records, len, sFile, "http://106.120.108.230:8524/vip/getbilllist.do?adminuin=%lu&kefu_uin=%lu&client_id=%s&weixin_id=%s&current_id=%s&timestamp=%s&token=%s&pagesize=1");

	// 微信服务器媒体文件上传url
	LoadIniString("WebPages", "WeChatMediaUpload", m_InitConfig.wechat_media_upload, len, sFile, "http://file.api.weixin.qq.com/cgi-bin/media/upload?access_token=%s&type=%s");

	// 腾讯地图静态图url
	LoadIniString("WebPages", "WeChatStaticMap", m_InitConfig.wechat_static_map, len, sFile, "http://apis.map.qq.com/ws/staticmap/v2/?key=JRYBZ-QIAWS-GJ3OB-6GXXF-F3WMZ-RNBGV&size=500x400&center=%s,%s&zoom=12");

	// 腾讯地图地点标注url
	LoadIniString("WebPages", "WeChatMapLocation", m_InitConfig.wechat_map_location, len, sFile, "http://apis.map.qq.com/uri/v1/marker?marker=coord:%s,%s;title:%s;addr:%s&referer=myapp");

	// 自己的文件服务器下载的微信服务器的媒体文件的url
	LoadIniString("WebPages", "FileServerMediaDownload", m_InitConfig.fileserver_media_download, len, sFile, "http://106.120.108.230:8531/media/get?access_token=%s&media_id=%s");

	// 自己的文件服务器上传url
	LoadIniString("WebPages", "FileServerMediaUpload", m_InitConfig.fileserver_media_upload, len, sFile, "http://106.120.108.230:8531/media/upload/%s");

	// 微信文件下载url
	LoadIniString("WebPages", "FileServerMediaTask", m_InitConfig.fileserver_media_task, len, sFile, "http://file.api.weixin.qq.com/cgi-bin/media/get?access_token=%s&media_id=%s");

	// 微信文件下载url
	LoadIniString("WebPages", "FileServerMediaFileId", m_InitConfig.fileserver_media_fileid, len, sFile, "http://wxm.tq.cn/media/%s");

	return true;
}

int CChatManager::SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid /*= 0*/, unsigned long recvsock /*= 0*/, HWND hWnd /*= NULL*/)
{
	int nrtn = 0;

	TCP_PACK_HEADER TcpPackHead;
	int nError;
	int nPackHeadLen = COM_HEAD_PACK::GetHeadLen();
	int nTcpPackHeadLen = sizeof(TCP_PACK_HEADER);
	int nSendLen;

	if (pPackInfo == NULL)
	{
		nrtn = SYS_ERROR_MEMORY;
		goto RETURN;
	}

	pPackInfo->m_Head.SetValue(pPackInfo->version,
		m_usCltRand,
		m_UserInfo.UserInfo.uid,
		m_usSrvRand,
		pPackInfo->m_Head.head.cmd, 0,
		pPackInfo->m_Head.head.langtype,
		0,
		TERMINAL_PCEX);

	if (!(pPackInfo->packhead()))
	{
		nrtn = SYS_ERROR_PACKHEAD;
		goto RETURN;
	}
	if (!(pPackInfo->pack()))
	{
		nrtn = SYS_ERROR_PACKINFO;
		goto RETURN;
	}

	//加密包  20090420
	pPackInfo->m_Pack.Encode();

	memset(&TcpPackHead, '\0', sizeof(TCP_PACK_HEADER));
	TcpPackHead.len = pPackInfo->m_Pack.GetPackLength();

	g_WriteLog.WriteLog(C_LOG_TRACE, "send pack cmd:%.4X", pPackInfo->m_Head.head.cmd);

	nSendLen = m_Socket.SendImPack((char *)(pPackInfo->m_Pack.GetPackBuff()), TcpPackHead, nError);
	if (nSendLen != TcpPackHead.len + nTcpPackHeadLen)
	{
		nrtn = SYS_ERROR_SENDFAIL;

		g_WriteLog.WriteLog(C_LOG_ERROR, "SendOnePack failed %d,fd=%d", TcpPackHead.len, m_Socket.getfd());
		goto RETURN;
	}

	nrtn = 0;

RETURN:
	return nrtn;
}

bool CChatManager::LoadUserConfig()
{
	//char sFile[MAX_256_LEN];
	tstring strFile = GetCurrentPath();

	//sprintf(sFile, "%sconfig_%u.dat", strFile, m_UserInfo.UserInfo.uid);
	//this->m_sysConfig->LoadUserData(sFile, CLIENTVERSION);

	return false;
}

int CChatManager::RecvSrvConfLogOff(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvStatusFrdOnline(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvStatusFrdOffline(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvStatusUserForm(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatShareList(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvComSendMsg(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatCreateChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatChatMsg(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatChatMsgAck(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatAcceptChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatTransQuest(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatTransFailed(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvInviteRequest(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvInviteResult(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatKefuRelease(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatCMDError(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatCloseChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatListChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvFloatChatInfo(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvEventAnnouncement(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvUpdateSucc(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvUpdateFail(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvDonw(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvRepTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}



