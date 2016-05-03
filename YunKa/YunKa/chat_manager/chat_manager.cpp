#include "../stdafx.h"
#include "chat_manager.h"
#include "../chat_common/comclt.h"
#include "../chat_common/comsrv.h"
#include "../chat_common/comfloat.h"
#include "../chat_common/comform.h"
#include "../chat_common/comfunc.h"
#include "../chat_common/comdef.h"
#include "../chat_common/comstruct.h"
#include "chat_history_msg.h"
#include <cctype>
#include <algorithm>
using namespace std;

#define CLIENTVERSION	20000
#define  HASINSERTRIGHT   FALSE

unsigned short g_packSeq = 0;


CChatManager::CChatManager(IBaseMsgs* iBaseMsgs)
{
	m_isLoginSuccess = false;
	
	m_chatObjects = NULL;
	m_baseMsgs = iBaseMsgs;
	m_server = "tcp01.tq.cn";
	m_port = 443;
	m_usSrvRand = 0;
	m_usCltRand = (unsigned short)(rand() & 0xFFFF);
	m_tResentVisitPackTime = 0;
	m_socket.SetReceiveObject(this);
	m_login = new CLogin();
	m_login->m_manager = this;
	m_sysConfig = new CSysConfigFile();
	m_timers = new CTimerManager(CChatManager::TimerProc, this);
	m_msgId = 0;

	LoadINIResource();
}

CChatManager::~CChatManager()
{
	delete m_timers;
	delete m_sysConfig;
	delete m_login;
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
	}
	return m_chatObjects;
}

void CChatManager::GetPreLoginInfo(string& loginName, string& password, bool& isAutoLogin, bool& isKeepPwd)
{
	loginName = m_sysConfig->lastloginuid;
	password = m_sysConfig->lastloginpass;
	isAutoLogin = m_sysConfig->bAutoLogin;
	isKeepPwd = m_sysConfig->bSavePass;
}

void CChatManager::StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{
	m_loginName = loginName;
	m_password = password;
	m_isAutoLogin = isAutoLogin;
	m_isKeepPwd = isKeepPwd;

	// 开始登录时，启动一个判断登录超时的定时器
	m_timers->SetTimer(5000, TIMER_NAME_LOGIN);

	m_login->StartLogin(loginName, password, isAutoLogin, isKeepPwd);
}

bool CChatManager::ReadSystemConfig()
{
	char sFile[MAX_256_LEN];
	string strFile = GetCurrentPath();

	sprintf(sFile, "%sconfig.dat", strFile.c_str());

	if (m_sysConfig->LoadData(sFile, CLIENTVERSION))
	{
	}
	else
	{
		SetSystemConfigByInitconfig();
	}

	m_sysConfig->strVisitorServer = m_initConfig.sVisitorServer;
	m_sysConfig->nVisitorServerPort = m_initConfig.nVisitorServerPort;
	return false;
}

void CChatManager::SetSystemConfigByInitconfig()
{
	int i;

	m_sysConfig->strServer = m_initConfig.sDefaultServer;
	m_sysConfig->nServerPort = m_initConfig.nDefaultPort;

	for (i = 0; i<10; i++)
	{
		if (strlen(m_initConfig.sListServer[i]) > 0)
		{
			m_sysConfig->m_listServerAddress.push_back(m_initConfig.sListServer[i]);
		}
	}

	m_sysConfig->filtertype = m_initConfig.visitorfilter;
	SolveUserdefineFilter(m_sysConfig->infofilter, m_initConfig.infofilter);
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
		if (m_baseMsgs)
		{
			m_baseMsgs->LoginProgress(80);
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
	case CMD_FLOAT_CREATECHAT: // 接入会话
		nError = RecvFloatCreateChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATINFO: // 会话详细信息
		nError = RecvFloatChatInfo(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_COM_SEND_MSG:
		nError = RecvComSendMsg(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
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
	if (m_baseMsgs)
	{
		m_baseMsgs->LoginProgress(-1);
	}
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

	m_userInfo.UserInfo.uid = RecvInfo.uin;
	strcpy(m_userInfo.UserInfo.sid, RecvInfo.strid);
	strcpy(m_userInfo.UserInfo.pass, m_password.c_str());

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

	if (!m_isLoginSuccess)
	{
		// 收到登录用户的信息包，登录成功
		if (m_baseMsgs)
		{
			m_baseMsgs->LoginProgress(100);
		}
		m_nOnLineStatus = STATUS_ONLINE;
		m_isLoginSuccess = true;
		m_timers->SetTimer(1000, TIMER_NAME_NORMAL);

		pUser = AddUserObject(RecvInfo.uin, RecvInfo.strid, RecvInfo.UserInfo.info.nickname, STATUS_ONLINE, -1);

		SendTo_UpdateOnlineStatus(m_nOnLineStatus);
	}
	else
	{
		// 登录成功后收到用户信息包处理
		if (packhead.langtype == LANGUAGE_UTF8)
		{
			// 转码
			ConvertMsg(RecvInfo.UserInfo.info.nickname, sizeof(RecvInfo.UserInfo.info.nickname) - 1);
		}

		g_WriteLog.WriteLog(C_LOG_TRACE, "recvsrvrepuserinfo uid=%u,sid=%s,nickname=%s,username=%s,langtype=%u", RecvInfo.uin,
			RecvInfo.UserInfo.info.sid, RecvInfo.UserInfo.info.nickname, RecvInfo.UserInfo.info.username, RecvInfo.UserInfo.info.langtype);

		if (RecvInfo.uin > WEBUSER_UIN)
		{
			//pWebUser = GetWebUserObjectByID(RecvInfo.uin);
			//
			//if (pWebUser != NULL)
			//{
			//	strcpy(pWebUser->info.name, RecvInfo.UserInfo.info.nickname);
			//	m_pFormUser->InvalidateTreeItem((DWORD)pWebUser, pWebUser->info.name);
			//	if (strlen(pWebUser->info.name) < 2)
			//	{
			//		g_WriteLog.WriteLog(C_LOG_ERROR, "name length is null：RecvSrvRepUserinfo() name length：%d", strlen(pWebUser->info.name));
			//	}
			//}
		}
		else
		{
			ONLINEFLAGUNION onlineflag;
			onlineflag.online_flag = RecvInfo.UserInfo.info.onlineflag;


			pUser = GetUserObjectByUid(RecvInfo.uin);
			if (pUser == NULL)
			{
				pUser = AddUserObject(RecvInfo.uin, RecvInfo.strid, RecvInfo.UserInfo.info.nickname, onlineflag.stStatus.nOnlineStatus, -1);

				if (pUser == NULL)
				{
					goto RETURN;
				}

				pUser->m_nFlag = 1;
			}
			else
			{
				if (pUser->status != onlineflag.stStatus.nOnlineStatus
					|| strcmp(pUser->UserInfo.nickname, RecvInfo.UserInfo.info.nickname) != 0)
				{
					bool bexpand = (pUser->status != onlineflag.stStatus.nOnlineStatus);

					pUser->status = onlineflag.stStatus.nOnlineStatus;
					strcpy(pUser->UserInfo.nickname, RecvInfo.UserInfo.info.nickname);
					
				}
			}

			m_baseMsgs->RecvOneUserInfo(pUser);

			// 下载头像
			//pUser->DownLoadFace();
		}
	}
	

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype)
{
	m_socket.Close();

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

	LoadIniString("common", "programe name mini", m_initConfig.sProgrameNameMini, len, sFile, NULL);

	LoadIniString("common", "help url", m_initConfig.sHelpUrl, len, sFile);

	LoadIniString("common", "copy id", m_initConfig.sCopyID, len, sFile);

	for (i = 0; i < 3; i++)
	{
		sprintf(sKey, "help about %d", i + 1);
		LoadIniString("common", sKey, m_initConfig.sHelpAbout[i], len, sFile);

		sprintf(sKey, "help about url %d", i + 1);
		LoadIniString("common", sKey, m_initConfig.sHelpAboutURL[i], len, sFile);
	}

	LoadIniInt("common", "display username type", m_initConfig.nDisplayUserNameType, sFile);

	LoadIniString("server", "address", m_initConfig.sDefaultServer, len, sFile);

	LoadIniInt("server", "port", m_initConfig.nDefaultPort, sFile);

	LoadIniString("visitor", "address", m_initConfig.sVisitorServer, len, sFile);

	LoadIniInt("visitor", "port", m_initConfig.nVisitorServerPort, sFile);

	int temp(0);
	LoadIniInt("visitor", "ZlibEnabled", temp, sFile);
	m_initConfig.bZlibEnabled = (1 == temp);

	for (i = 0; i < 10; i++)
	{
		sprintf(sKey, "list %d", i + 1);
		LoadIniString("server", sKey, m_initConfig.sListServer[i], len, sFile);
	}

	LoadIniInt("login", "save pass", m_initConfig.bSavePass, sFile);
	LoadIniInt("login", "auto login", m_initConfig.bAutoLogin, sFile);

	//visitor filter
	LoadIniInt("visitor filter", "filter", m_initConfig.visitorfilter, sFile, "0");
	LoadIniString("visitor filter", "info filter", m_initConfig.infofilter, len, sFile, "");
	LoadIniString("visitor filter", "info filter", m_initConfig.tailfilter, len, sFile, "");
	LoadIniString("AuthClient", "AuthAddr", m_initConfig.sAuthAddr, len, sFile, "passport.tq.cn");
	LoadIniInt("AuthClient", "AuthPort", m_initConfig.nAuthPort, sFile, "80");
	LoadIniString("WebPages", "logout", m_initConfig.webpage_lgout, len, sFile, "http://vip.tq.cn/vip/logout.do");
	LoadIniString("WebPages", "webphone", m_initConfig.webpage_webphone, len, sFile, "http://vip.tq.cn/vip/screenIndex.do?a=1");

	// 查询来电
	LoadIniString("WebPages", "querywebphone", m_initConfig.webpage_querywebphone, len, sFile, "http://vip.tq.cn/vip/screenPhoneRecord.do?action=query&deal_state=0");

	// 留言
	LoadIniString("WebPages", "note", m_initConfig.webpage_note, len, sFile, "http://vip.tq.cn/vip/viewLiuyan.do?a=1");

	// 客户管理
	LoadIniString("WebPages", "crm", m_initConfig.webpage_crm, len, sFile, "http://vip.tq.cn/vip/visitorInfomation.do?a=1");

	// 统计分析
	LoadIniString("WebPages", "analyze", m_initConfig.webpage_analyze, len, sFile, "http://vip.tq.cn/vip/serviceStutasE.do?a=1");

	// 管理中心
	LoadIniString("WebPages", "mgmt", m_initConfig.webpage_mgmt, len, sFile, "http://vip.tq.cn/vip/clientIndex.do?a=1");

	// 焦点窗口
	LoadIniString("WebPages", "news", m_initConfig.webpage_news, len, sFile, "http://vip.tq.cn/vip/rss/tq_mini_loading.jsp?a=");

	// 注册新用户
	LoadIniString("WebPages", "regnewuser", m_initConfig.webpage_regnewuser, len, sFile, "http://www.tq.cn/vip/prenewqttclt.do");
	LoadIniInt("WebPages", "regnewuser_w", m_initConfig.webpage_regnewuser_w, sFile, "520");
	LoadIniInt("WebPages", "regnewuser_h", m_initConfig.webpage_regnewuser_h, sFile, "410");

	// 新手指南
	LoadIniString("WebPages", "tqhelp", m_initConfig.webpage_tqhelp, len, sFile, "http://www.tq.cn/help_3.html");

	// 统一用语
	LoadIniString("WebPages", "UniIdioms", m_initConfig.webpage_UniIdioms, len, sFile, "http://vip.tq.cn/vip/GetRequestXMLAction.do?cmd=NEWQUICKREPLY");

	// 实时监控
	LoadIniString("WebPages", "RtmMsg", m_initConfig.webpage_RtmMsg, len, sFile, "http://vip.tq.cn/vip/realTime.do?a=");

	// 漫游消息
	LoadIniString("WebPages", "SvrMsg", m_initConfig.webpage_SvrMsg, len, sFile, "http://vip.tq.cn/vip/historyDialog.do?a=");

	// TQ顾问
	LoadIniString("WebPages", "TQadviser", m_initConfig.webpage_TQadviser, len, sFile, "http://vip.tq.cn/vip/myTqGuWen.do?a=");

	// 上传头像
	LoadIniString("WebPages", "UploadHeadImage", m_initConfig.webpage_UploadHeadImage, len, sFile, "http://vip.tq.cn/vip/preuploadfacelink.do?a=");

	// 下载头像
	LoadIniString("WebPages", "DownloadHeadImage", m_initConfig.webpage_DownloadHeadImage, len, sFile, "http://vip.tq.cn/vip/preuploadfacelink.do?a=");

	// 表情
	LoadIniString("WebPages", "FaceImage", m_initConfig.webpage_FaceImage, len, sFile, "http://vip.tq.cn/clientimages/face/images");

	// 系统消息
	LoadIniString("WebPages", "SysMessage", m_initConfig.webpage_SysMessage, len, sFile, "http://sysmessage.tq.cn/SysMsg/getmesg.do?strid=%s&uin=%lu&departId=%d");

	// 投诉建议
	LoadIniString("WebPages", "complaint", m_initConfig.webpage_complaint, len, sFile, "http://211.151.52.62:8081/zhengquan/preLeaveMsg.do?uin=%lu");

	//发送文件
	LoadIniString("WebPages", "SendWebFileLink", m_initConfig.webpage_SendWebFileLink, len, sFile, "http://vip.tq.cn/upload/upload2.do?version=100");


	LoadIniString("WebPages", "iconurl", m_initConfig.webpage_iconurl, len, sFile, "http://sysimages.tq.cn/clientimages/face/ywt_face.html");
	LoadIniString("WebPages", "faqInfo", m_initConfig.webpage_faqInfo, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqInfo");
	LoadIniString("WebPages", "faqSort", m_initConfig.webpage_faqSort, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqSort");
	LoadIniString("WebPages", "faqAll", m_initConfig.webpage_faqall, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqAll");
	LoadIniString("WebPages", "repickchat", m_initConfig.webpage_repickchaturl, len, sFile, "http://106.120.108.230:8031/wxcreatechat?uin=%lu&signature=%s&timestamp=%s&nonce=%s&admiuin=%lu&%s&msg=hello");
	//邀请评价evaluate
	LoadIniString("WebPages", "evaluate", m_initConfig.webpage_evaluate, len, sFile, "http://106.120.108.230:8524/vip/scorebill.do?billid=%s&admin_uin=%u&action=scoreedit");

	LoadIniString("WebPages", "sendurl", m_initConfig.webpage_sendurl, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=url");

	//获取公司用户
	LoadIniString("WebPages", "companyuser", m_initConfig.webpage_companyuser, len, sFile, "http://vip.tq.cn/vip/GetRequestXMLData?cmd=TRANSFER");

	// 获取微信token
	LoadIniString("WebPages", "accesstoken", m_initConfig.webpage_accesstoken, len, sFile, "http://106.120.108.230:8031/wxkfgetac?uin=%lu&signature=%s&timestamp=%s&nonce=%s&{$MSG_WORKBILL.UserDefineParams}");

	//工单
	LoadIniString("WebPages", "workbillurl", m_initConfig.webpage_workbillurl, len, sFile, "http://106.120.108.230:8524/vip/workOrder.do?r=%s");

	// 客户履历
	LoadIniString("VisitorPages", "visitortail", m_initConfig.visitorpage_visitortail, len, sFile, "http://vip.tq.cn/vip/ClientDialog.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");

	//会话登单
	LoadIniString("VisitorPages", "visitorbill", m_initConfig.visitorpage_visitorbill, len, sFile, "http://vip.tq.cn/vip/recrodworkbill.do?action=prerecord&billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");

	//客户信息
	LoadIniString("VisitorPages", "visitorinfo", m_initConfig.visitorpage_visitorinfo, len, sFile, "http://vip.tq.cn/vip/visitorinfo.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");
	//下订单
	LoadIniString("VisitorPages", "visitororder", m_initConfig.visitorpage_visitororder, len, sFile, "http://106.120.108.230:8524/vip/visitorinfo.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s&clientid=%s&type=0");

	//通告信息
	LoadIniString("VisitorPages", "visitornotice", m_initConfig.visitorpage_notice, len, sFile, "http://www.tq.cn");
	LoadIniString("WebPages", "RegisterAccount", m_initConfig.registerAccount, len, sFile, "http://www.tq.cn");
	LoadIniString("WebPages", "ForgetPassword", m_initConfig.forgetPassword, len, sFile, "http://www.tq.cn");
	LoadIniInt("WebPages", "ForgetPSVisible", m_initConfig.forgetPSVisible, sFile);

	// 查询历史记录
	LoadIniString("WebPages", "QueryHistoryRecords", m_initConfig.query_history_records, len, sFile, "http://106.120.108.230:8524/vip/getbilllist.do?adminuin=%lu&kefu_uin=%lu&client_id=%s&weixin_id=%s&current_id=%s&timestamp=%s&token=%s&pagesize=1");

	// 微信服务器媒体文件上传url
	LoadIniString("WebPages", "WeChatMediaUpload", m_initConfig.wechat_media_upload, len, sFile, "http://file.api.weixin.qq.com/cgi-bin/media/upload?access_token=%s&type=%s");

	// 腾讯地图静态图url
	LoadIniString("WebPages", "WeChatStaticMap", m_initConfig.wechat_static_map, len, sFile, "http://apis.map.qq.com/ws/staticmap/v2/?key=JRYBZ-QIAWS-GJ3OB-6GXXF-F3WMZ-RNBGV&size=500x400&center=%s,%s&zoom=12");

	// 腾讯地图地点标注url
	LoadIniString("WebPages", "WeChatMapLocation", m_initConfig.wechat_map_location, len, sFile, "http://apis.map.qq.com/uri/v1/marker?marker=coord:%s,%s;title:%s;addr:%s&referer=myapp");

	// 自己的文件服务器下载的微信服务器的媒体文件的url
	LoadIniString("WebPages", "FileServerMediaDownload", m_initConfig.fileserver_media_download, len, sFile, "http://106.120.108.230:8531/media/get?access_token=%s&media_id=%s");

	// 自己的文件服务器上传url
	LoadIniString("WebPages", "FileServerMediaUpload", m_initConfig.fileserver_media_upload, len, sFile, "http://106.120.108.230:8531/media/upload/%s");

	// 微信文件下载url
	LoadIniString("WebPages", "FileServerMediaTask", m_initConfig.fileserver_media_task, len, sFile, "http://file.api.weixin.qq.com/cgi-bin/media/get?access_token=%s&media_id=%s");

	// 微信文件下载url
	LoadIniString("WebPages", "FileServerMediaFileId", m_initConfig.fileserver_media_fileid, len, sFile, "http://wxm.tq.cn/media/%s");

	return true;
}

int CChatManager::SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid, unsigned long recvsock, HWND hWnd)
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
		m_userInfo.UserInfo.uid,
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

	nSendLen = m_socket.SendImPack((char *)(pPackInfo->m_Pack.GetPackBuff()), TcpPackHead, nError);
	if (nSendLen != TcpPackHead.len + nTcpPackHeadLen)
	{
		nrtn = SYS_ERROR_SENDFAIL;

		g_WriteLog.WriteLog(C_LOG_ERROR, "SendOnePack failed %d,fd=%d", TcpPackHead.len, m_socket.getfd());
		goto RETURN;
	}

	nrtn = 0;

RETURN:
	return nrtn;
}

bool CChatManager::LoadUserConfig()
{
	//char sFile[MAX_256_LEN];
	string strFile = GetCurrentPath();

	//sprintf(sFile, "%sconfig_%u.dat", strFile, m_userInfo.UserInfo.uid);
	//this->m_sysConfig->LoadUserData(sFile, CLIENTVERSION);

	return false;
}

int CChatManager::RecvSrvConfLogOff(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvStatusFrdOnline(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_STATUS_FRDONLINE RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0 || RecvInfo.m_Head.head.random != m_usSrvRand)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvsrvstatusfrdonline unpack failed");
		return nError;
	}

	if (packhead.langtype == LANGUAGE_UTF8)
	{
		ConvertMsg(RecvInfo.nickname, sizeof(RecvInfo.nickname) - 1);
	}

	if (RecvInfo.uin > WEBUSER_UIN)
	{
		//访客对话开始
		pWebUser = GetWebUserObjectByUid(RecvInfo.uin);
		if (pWebUser == NULL)
		{
			//GetWebUserID(RecvInfo.uin,NULL);
		}
	}
	else
	{

		pUser = GetUserObjectByUid(RecvInfo.uin);
		if (pUser == NULL)
		{
			//这里不考虑很复杂，如果有新的用户，需要自动的刷新以下，以后再做实时地处理
			pUser = AddUserObject(RecvInfo.uin, "", RecvInfo.nickname, RecvInfo.status, -1);

			pUser->status = STATUS_ONLINE;
			strcpy(pUser->UserInfo.nickname, RecvInfo.nickname);

			//this->m_pFormUser->AddNewUserToTree(pUser);
			pUser->m_nFlag = 1;

			//if (m_pUserAndAuthDlg != NULL && m_pUserAndAuthDlg->IsWindowVisible())
			//{
			//	m_pUserAndAuthDlg->InitialSetup();
			//}
			//
			//m_pFormUser->ExpandUserItem(pUser);
		}
		else
		{
			if (pUser->status == STATUS_OFFLINE)
			{
				pUser->status = STATUS_ONLINE;
				strcpy(pUser->UserInfo.nickname, RecvInfo.nickname);

				//m_pFormUser->MoveUserToOnlineItem(pUser, true);
			}
			else
			{
				if (strcmp(pUser->UserInfo.nickname, RecvInfo.nickname) != 0 || pUser->status != RecvInfo.status)
				{
					bool bexpand = (pUser->status != STATUS_ONLINE);

					pUser->status = STATUS_ONLINE;	//RecvInfo.status;
					strcpy(pUser->UserInfo.nickname, RecvInfo.nickname);
					//m_pFormUser->InvalidateTreeItem((DWORD)pUser, pUser->GetUserDisplayName());

					//if (bexpand)
						//m_pFormUser->ExpandUserItem(pUser);
				}
			}
		}

		if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
		{
			m_nOnLineStatus = RecvInfo.status;
		}

		m_baseMsgs->RecvOnline(pUser);
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvSrvStatusFrdOffline(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_STATUS_FRDOFFLINE RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0 || RecvInfo.m_Head.head.random != m_usSrvRand)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvsrvstatusfrdoffline unpack failed");
		return nError;
	}
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvStatusFrdOffline uin:%u,externid:%u, bak:%u",
		RecvInfo.uin, RecvInfo.externid, RecvInfo.bak);
	if (RecvInfo.uin > WEBUSER_UIN)
	{
		//访客对话结束
		pWebUser = GetWebUserObjectByUid(RecvInfo.uin);
		if (pWebUser != NULL)
		{
			pUser = GetUserObjectByUid(pWebUser->talkuid);

			pWebUser->m_bNewComm = false;
			pWebUser->cTalkedSatus = HASTALKED;
			pWebUser->transferuid = 0;
			if (pWebUser->m_bConnected)
			{
				//SendWebuserTalkEnd(pWebUser);

				pWebUser->m_bConnected = true;
				pWebUser->m_nWaitTimer = -20;
				pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;

				//m_pFormMain->RecvWebUserStartTalk(pWebUser, false);
				//m_pFormUser->MoveWebUserToOnlineItem(pWebUser, pWebUser->IsOnline());
			}
			//结束会话，要从用户群列表中删除
			//DelWebUserInMultiList(pWebUser);
		}
	}
	else
	{
		pUser = GetUserObjectByUid(RecvInfo.uin);
		if (pUser != NULL)
		{
			bool bexpand = false;
			if (pUser->status != STATUS_OFFLINE)
			{
				bexpand = true;
				pUser->m_bConnected = true;
				pUser->talkstatus = TALKSTATUS_NO;
				pUser->status = STATUS_OFFLINE;//RecvInfo.status;

				//m_pFormUser->RemoveUserAllActiveTalkUser(pUser);
				//m_pFormUser->MoveUserToOnlineItem(pUser, false);
			}

			//m_pFormUser->InvalidateTreeItem((DWORD)pUser, pUser->GetUserDisplayName());
			//if (bexpand)
				//m_pFormUser->ExpandUserItem(pUser);

			if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
			{
				g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvStatusFrdOffline--SetOfflineStatus uid%u", m_userInfo.UserInfo.uid);
				m_nOnLineStatus = STATUS_OFFLINE;
				m_nOnLineStatusEx = STATUS_OFFLINE;
			}
		}
		m_baseMsgs->RecvOffline(pUser);
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvSrvStatusUserForm(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_STATUS_USER_FORM RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvsrvstatususerform unpack failed");
		return nError;
	}

	pUser = GetUserObjectByUid(RecvInfo.uin);
	if (pUser != NULL)
	{
		pUser->status = RecvInfo.GetOnlineStatus();
		//m_pFormUser->InvalidateTreeItem((DWORD)pUser, pUser->GetUserDisplayName());

		//m_pFormUser->ExpandUserItem(pUser);
	}

	m_baseMsgs->RecvUserStatus(pUser);

	nError = 0;

	return nError;
}

int CChatManager::RecvFloatShareList(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_SHARELIST RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;

	std::vector<SHAREUSEROBJ>::iterator iter;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatShareList unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}

	ONLINEFLAGUNION onlineflag;
	string szsharelsit, sztemp;
	for (iter = RecvInfo.sharememlist.begin(); iter != RecvInfo.sharememlist.end(); iter++)
	{
		SHAREUSEROBJ ShareUserOb = (SHAREUSEROBJ)(*iter);

		onlineflag.online_flag = ShareUserOb.onlineflag;

		pUser = GetUserObjectByUid(ShareUserOb.uin);
		if (pUser == NULL)
		{
			pUser = AddUserObject(ShareUserOb.uin, "", "", onlineflag.stStatus.nOnlineStatus, -1);
			//pUser->DownLoadFace();
		}

		//sztemp.Format("uin=%u:OnlineStatus=%d,", ShareUserOb.uin, onlineflag.stStatus.nOnlineStatus);
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatShareList:%s", sztemp);
		pUser->m_bFriend = true;
	}

	SendTo_GetAllUserInfo();

	nError = 0;

	return nError;
}

int CChatManager::RecvComSendMsg(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_SEND_MSG RecvInfo(packhead.ver);
	int nError = 0;
	int msgType = MSG_TYPE_NORMAL;
	int msgFrom = MSG_FROM_USER;
	char msgTime[MAX_256_LEN];
	CWebUserObject *pWebUser = NULL;
	CUserObject *pUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);

	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendMsg unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComSendMsg chatid:%s,Rand:%s,Thirdid:%s,recvuin:%u,senduin:%u,strfontinfo:%s,msgtype:%d,msg:%s",
		RecvInfo.strChatid, RecvInfo.strRand, RecvInfo.strThirdid, RecvInfo.msg.recvuin,
		RecvInfo.msg.senduin, RecvInfo.msg.strfontinfo, RecvInfo.msg.msgtype, RecvInfo.msg.strmsg);

	//先显示内容， 再移动item
	if (packhead.langtype == 4)
	{
		ConvertMsg(RecvInfo.msg.strmsg, sizeof(RecvInfo.msg.strmsg) - 1);

		ConvertMsg(RecvInfo.msg.strmobile, sizeof(RecvInfo.msg.strmobile) - 1);
	}

	if (RecvInfo.msg.senduin > WEBUSER_UIN)
	{
		//访客发来消息
		switch (RecvInfo.msg.msgtype)
		{
		case MSG_WORKBILL:
		case MSG_CLIENT_COME:
			RecvComSendWorkBillMsg(RecvInfo.msg.senduin, RecvInfo.msg.recvuin, RecvInfo.msg.strmsg, RecvInfo.msg.strmobile);
			break;
		case MSG_WEBUI_SEND_CHAT_HISMSG:   //收到webui发过来的会话历史消息，可能多个包,需要客户端发送请求MSG_KEFU_GET_CHAT_HISMSG
			RecvComSendNormalChatidHisMsg(RecvInfo.msg.senduin, RecvInfo.msg.recvuin, RecvInfo);
			break;
		case MSG_TRANSFER_REQUEST://会话转移请求
			RecvComTransRequest(packhead.uin, RecvInfo);
			break;
		case MSG_TRANSFER_ANSWER://会话转移请求
			RecvComTransAnswer(packhead.uin, RecvInfo);
			break;
		case MSG_NORMAL:
		case MSG_WX:
			pWebUser = GetWebUserObjectByUid(RecvInfo.msg.senduin);
			if (pWebUser == NULL)
			{
				//需要去服务器获取该访客信息
				string l_msg = RecvInfo.msg.strmsg;
				if (l_msg != "null")
				{
					SendTo_GetWebUserInfo(RecvInfo.msg.senduin, RecvInfo.strChatid);

					int pos = l_msg.find("\"msgtype\":\"userinfo\"");
					int pos1 = l_msg.find("\"msgtype\":\"wxactoken\"");
					if (pos < 0 && pos1 < 0)
					{
						SaveEarlyMsg(&(RecvInfo.msg));
					}
					else
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "userinfo信息进入消息记录bug：%s", RecvInfo.msg.strmsg);
					}
				}
			}
			else
			{
				if (strcmp(RecvInfo.msg.strfontinfo, "JSON=WX") == 0)
				{
					//微信消息类型，utf8编码类型
					WxObj *pwxobj = ParseWxJsonMsg(RecvInfo.msg.strmsg);

					if (pwxobj != NULL)
					{
						pWebUser->m_bIsFrWX = true;
						if ("userinfo" == pwxobj->MsgType)
						{
							// 此处注意，如果第二次收到userinfo，应当将前面收到那次信息析构掉
							if (pWebUser->m_pWxUserInfo == NULL)
							{
								pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
							}
							else
							{
								delete pWebUser->m_pWxUserInfo;
								pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
							}
							nError = true;
							pWebUser->m_bIsGetInfo = true;
							// 此处微信用户信息不析构，保存并后面使用
							goto RETURN;
						}
						else if ("wxactoken" == pwxobj->MsgType)
						{
							//todo:收到服务器发过来的微信的access_token消息
							WxTokens::iterator iter = m_mapTokens.find(RecvInfo.strThirdid);
							if (iter == m_mapTokens.end())
							{
								m_mapTokens.insert(map<string, string>::value_type(RecvInfo.strThirdid, ((WxAccessTokenInfo*)pwxobj)->szAcToken));
							}
							else
							{
								iter->second = ((WxAccessTokenInfo*)pwxobj)->szAcToken;
							}
							delete pwxobj;
							nError = true;
							goto RETURN;
						}
					}
					else
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendMsg NOT the weixin Json:%s", RecvInfo.msg.strmsg);
						goto RETURN;
					}
				}
				//网页过来的会话，可能chatid会变化，需要及时更新
				if (strcmp(pWebUser->chatid, RecvInfo.strChatid) != 0 && strlen(RecvInfo.strChatid) > 0)
					strcpy(pWebUser->chatid, RecvInfo.strChatid);
				pWebUser->m_bNewComm = false;

				if (strstr(RecvInfo.msg.strmsg, "[系统消息]") != NULL
					&& strstr(RecvInfo.msg.strmsg, "您网站来访客了") != NULL)
				{
					SendTo_GetWebUserInfo(RecvInfo.msg.senduin, RecvInfo.strChatid);
				}

				if (pWebUser->info.name[0] == '\0')
				{
					strcpy(pWebUser->info.name, RecvInfo.msg.strmobile);
					if (strlen(pWebUser->info.name) < 2)
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "name length is null：RecvComSendMsg1() name length：%d", strlen(pWebUser->info.name));
					}
				}
				else if (!pWebUser->info.nameflag && strcmp(RecvInfo.msg.strmobile, pWebUser->info.name) != 0)
				{
					if (RecvInfo.msg.strmobile[0] != '\0')
					{
						strcpy(pWebUser->info.name, RecvInfo.msg.strmobile);
					}
					else
					{
						if (strlen(pWebUser->info.name) < 2)
						{
							g_WriteLog.WriteLog(C_LOG_ERROR, "name length is null：RecvComSendMsg2() name length：%d", strlen(pWebUser->info.name));
						}
					}
				}

				if (m_sysConfig->IsWebuserSidForbid(pWebUser->info.sid))
				{
					//消息屏蔽了，直接跳出不处理
					goto RETURN;
				}

				if (!(strstr(RecvInfo.msg.strmsg, "[系统消息]") != NULL && strstr(RecvInfo.msg.strmsg, "客人离开") != NULL))
				{
					pWebUser->m_resptimeoutmsgtimer = -1;
					pWebUser->m_resptimeoutclosetimer = -1;
					if (pWebUser->m_waitresptimeouttimer < 0)
						pWebUser->m_waitresptimeouttimer = 0;	// 该客服说话了 [12/29/2010 SC]

					m_baseMsgs->RecvOneMsg(pWebUser, msgFrom, GetMsgId(), msgType, MSG_DATA_TYPE_TEXT, 
						RecvInfo.msg.strmsg,msgTime, NULL, NULL, "");

					if ((RecvInfo.msg.bak == MSG_BAK_NORMAL) && !pWebUser->m_bConnected)
					{
						SendWebuserTalkBegin(pWebUser);
						pWebUser->cTalkedSatus = INTALKING;
						pWebUser->talkuid = m_userInfo.UserInfo.uid;

						pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
						pWebUser->transferuid = 0;
						pWebUser->m_nWaitTimer = 0;
						pWebUser->m_bConnected = TRUE;
						//访客来了，肯定不是离线
						if (pWebUser->info.status == STATUS_OFFLINE)
							pWebUser->info.status = STATUS_ONLINE;

						// 提示
					}
					else if (RecvInfo.msg.bak == MSG_BAK_NORMAL)
					{
						pWebUser->cTalkedSatus = INTALKING;

						// 提示 
					}

					if (m_sysConfig->m_bAutoRespUnnormalStatus)
					{
						string strResp;

						switch (m_nOnLineStatus)
						{
						case STATUS_BUSY:
						case STATUS_WORK:
						case STATUS_REFUSE_NEWWEBMSG:
							strResp = m_sysConfig->m_strUnnormalStatusMsg[0];
							break;
						case STATUS_LEAVE:
						case STATUS_EATING:
						case STATUS_OFFLINE:
							strResp = m_sysConfig->m_strUnnormalStatusMsg[1];
							break;
						}

						if (!strResp.empty())
						{
							SendAutoRespMsg(pWebUser, strResp.c_str());
						}
					}
				}
				else
				{
					pWebUser->cTalkedSatus = HASTALKED;
					SendWebuserTalkEnd(pWebUser);
					if (pWebUser->m_bConnected)
					{
						pWebUser->m_bConnected = FALSE;
						pWebUser->m_nWaitTimer = -20;
						pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
					}
				}

				pWebUser->m_nWaitTimer = 0;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//客服发来消息
		pUser = GetUserObjectByUid(RecvInfo.msg.senduin);
		if (pUser == NULL)
		{
			pUser = AddUserObject(RecvInfo.msg.senduin, "", RecvInfo.msg.strmobile, STATUS_ONLINE, -1);
			// 下载该坐席的头像
			//pUser->DownLoadFace();
			if (pUser == NULL)
			{
				goto RETURN;
			}
			pUser->m_nWaitTimer = 0;

			m_baseMsgs->RecvOneMsg(pUser, msgFrom, GetMsgId(), msgType, MSG_DATA_TYPE_TEXT, RecvInfo.msg.strmsg,
				msgTime, NULL, NULL, "");
			if ((RecvInfo.msg.bak == MSG_BAK_NORMAL || RecvInfo.msg.bak == MSG_BAK_AUTOANSER) && !(pUser->m_bInnerTalk))
			{
				pUser->m_bInnerTalk = true;
			}
		}
		else
		{
			strncpy(pUser->UserInfo.nickname, RecvInfo.msg.strmobile, MAX_USERNAME_LEN);//此处会返回“系统”两字
			pUser->m_nWaitTimer = 0;

			m_baseMsgs->RecvOneMsg(pUser, msgFrom, GetMsgId(), msgType, MSG_DATA_TYPE_TEXT, RecvInfo.msg.strmsg, msgTime, NULL, NULL, "");

			if ((RecvInfo.msg.bak == MSG_BAK_NORMAL || RecvInfo.msg.bak == MSG_BAK_AUTOANSER) && !(pUser->m_bInnerTalk))
			{
				pUser->m_bInnerTalk = true;
			}
		}
		
		// 提示
	}

RETURN:
	SendAckEx(RecvInfo.msg.seq, RecvInfo.msg.senduin, RecvInfo.sendip, RecvInfo.sendport);
	nError = 0;
	return 0;
}

int CChatManager::RecvFloatCreateChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CREATECHAT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatcreatechat unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "**RecvFloatCreateChat uAdminId:%u,usort:%hu,uWebuin:%u,chatid:%s,clienttid:%s,webname:%s,uKefu:%u,uFromAdmin:%u,uFromSort:%hu",
			RecvInfo.uAdminId, RecvInfo.usort, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.clienttid, RecvInfo.webname, RecvInfo.uKefu, RecvInfo.uFromAdmin, RecvInfo.uFromSort);
	}
	if (0 == RecvInfo.clienttid[0])
	{
		//处理cookie中没有clientid对话类型
		sprintf(RecvInfo.clienttid, "%lu", RecvInfo.uWebuin);
		g_WriteLog.WriteLog(C_LOG_TRACE, "recvfloatcreatechat clientid is empty,set as webuin:%u", RecvInfo.uWebuin);
	}
	pWebUser = GetWebUserObjectBySid(RecvInfo.clienttid);
	if (pWebUser == NULL)
	{
		pWebUser = AddWebUserObject(RecvInfo.clienttid, RecvInfo.thirdid, RecvInfo.webname, "", "", STATUS_ONLINE, 0);
		pWebUser->m_onlinetime = 0;
		pWebUser->m_bIsShow = false;
	}

	if (RecvInfo.webname[0] != '\0' && strcmp(RecvInfo.webname, pWebUser->info.name) != 0)
	{
		strcpy(pWebUser->info.name, RecvInfo.webname);
	}

	//GetWxUserInfoAndToken(pWebUser);

	pWebUser->cTalkedSatus = INTALKING;
	if (RecvInfo.uFromAdmin == 0)
	{
		pWebUser->nVisitFrom = WEBUSERICON_DIRECTURL;
	}
	else if (RecvInfo.uFromAdmin == m_login->m_authAdminid)
	{

	}
	else
	{
		pWebUser->nVisitFrom = WEBUSERICON_UNIONURL;
	}

	pWebUser->webuserid = RecvInfo.uWebuin;
	if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
		strcpy(pWebUser->chatid, RecvInfo.chatid);
	pWebUser->floatadminuid = RecvInfo.uAdminId;
	pWebUser->floatfromadminuid = RecvInfo.uFromAdmin;
	pWebUser->floatfromsort = RecvInfo.uFromSort;
	pWebUser->gpid = packhead.random;
	pWebUser->m_bNewComm = true;
	pWebUser->m_sNewSeq = packhead.sendrandom; //这是消息id
	pWebUser->info.status = STATUS_ONLINE;
	pWebUser->talkuid = 0;//等待应答进来的会话，还没有接入客服
	if (RecvInfo.uKefu == 0)
	{
		if (pWebUser->onlineinfo.talkstatus != TALKSTATUS_REQUEST)
		{
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_REQUEST;
			pWebUser->m_nWaitTimer = 0;

			string str;
			//str = "%s 用户申请对话!" + pWebUser->info.name;
			

			// 提示音??
		}
	}
	else
	{
		//pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;

		//pUser = GetUserObjectByUid(RecvInfo.uKefu);

		//if (pUser != NULL &&
		//	(pUser->UserInfo.uid == m_userInfo.UserInfo.uid || pUser->m_bFriend)
		//	)
		//{

		//	pWebUser->transferuid = 0;

		//	if (RecvInfo.uKefu == m_userInfo.UserInfo.uid)
		//	{
		//		//是自己邀请的访客
		//		pWebUser->m_nWaitTimer = 0;
		//		pWebUser->m_bConnected = true;
		//	}
		//	else
		//	{
		//		//其他人邀请的访客
		//		pWebUser->m_nWaitTimer = -20;
		//		pWebUser->m_bConnected = HASINSERTRIGHT;
		//	}

		//	pWebUser->talkuid = RecvInfo.uKefu;

		//	//读取会话历史消息，并接受新的消息
		//	StartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);

		//	//访客邀请
		//	m_pFormMain->RecvUserAcceptChat(pWebUser, pUser);
		//	g_WriteLog.WriteLog(C_LOG_TRACE, "Recvfloatcreatechat kefuName：%s,webuserName:%s", pUser->UserInfo.nickname, pWebUser->info.name);
		//	//访客移到客服的对话列表
		//	//m_pFormUser->MoveWebUserToUserTalkItem(pWebUser, pUser);
		//	int nRet = this->m_pFormUser->MoveWebUserToUserTalkItem(pWebUser, pUser);
		//	if (nRet < 0)
		//		m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
		//	else if (RecvInfo.uKefu == m_userInfo.UserInfo.uid)
		//		AddMultiWebUserToList(pWebUser);//把接入的会话移动到会话列表中

		//}
	}

	if (RecvInfo.memlist.find(m_userInfo.UserInfo.uid) != RecvInfo.memlist.end())
		pWebUser->m_bNotResponseUser = 0;
	else
		pWebUser->m_bNotResponseUser = 1;

	m_baseMsgs->RecvCreateChat(pWebUser);

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatChatInfo(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CHATINFO RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;
	std::vector<unsigned int>::iterator iter;
	char strMsg[MAX_256_LEN];

	strcpy(strMsg, "waiter");

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatinfo unpack failed,Cmd:%.4x", packhead.cmd);

		goto RETURN;
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatChatInfo uAdminId:%u,usort:%hu,uWebuin:%u,chatid:%s,clienttid:%s,webname:%s,uKefu:%u,uFromAdmin:%u,uFromSort:%hu",
			RecvInfo.uAdminId, RecvInfo.sSort, RecvInfo.uWebUin, RecvInfo.chatid, RecvInfo.strClientId, RecvInfo.webnickname, RecvInfo.uKefuUin, RecvInfo.uFromAmdind, RecvInfo.sFromSort);

	}

	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebUin);
	if (pWebUser == NULL)
	{
		pWebUser = AddWebUserObject(RecvInfo.strClientId, RecvInfo.strThirdId, RecvInfo.webnickname, "", "", STATUS_UNDEFINE, 0);
		pWebUser->webuserid = RecvInfo.uWebUin;
	}
	else
	{
		//ChangeWebUserSid(pWebUser, RecvInfo.strClientId, RecvInfo.strThirdId);
	}

	// 只要收到会话信息，一律去服务器获取work_bill包
	//GetWebUserID(pWebUser->webuserid, RecvInfo.chatid, strMsg.GetBuffer(0));
	if (pWebUser->m_nEMObType != OBJECT_WEBUSER)
	{
		pWebUser->m_nEMObType = OBJECT_WEBUSER;
	}
	pWebUser->m_bNewComm = true;
	pWebUser->cTalkedSatus = INTALKING;

	pWebUser->floatadminuid = RecvInfo.uAdminId;
	if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
		strcpy(pWebUser->chatid, RecvInfo.chatid);

	pWebUser->floatfromadminuid = RecvInfo.uFromAmdind;
	pWebUser->floatfromsort = RecvInfo.sFromSort;
	pWebUser->gpid = packhead.random;

	if (RecvInfo.uKefuUin == 0)
	{
		//等待应答
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_REQUEST;
		pWebUser->info.status = STATUS_ONLINE;
		pWebUser->m_nWaitTimer = 0;

		//m_pFormMain->RecvWebUserInvite(pWebUser, "");
		//m_pFormUser->MoveWebUserToWaitItem(pWebUser);//等待应答的，移动到等待应答队列
		//StartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);
	}
	else
	{
		if (m_nNextInviteWebuserUid == pWebUser->webuserid)
		{
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_INVITE;
			pWebUser->inviteuid = m_userInfo.UserInfo.uid;
			pWebUser->info.status = STATUS_ONLINE;
			pWebUser->frominviteuid = m_nNextInviteUid;

			pWebUser->m_nWaitTimer = 0;

			CUserObject *pInviteUser = GetUserObjectByUid(m_nNextInviteUid);

			//这里必须先在htmleditor中显示，然后再移动位置，因为移动位置可能会导致区域的切换，其他地方同样考虑
			//this->m_pFormMain->SendInviteToWebUser(pInviteUser, pWebUser, INVITE_HELP, m_userInfo);
			//m_pFormUser->MoveWebUserToUserInviteItem(pWebUser, m_userInfo);

			m_nNextInviteWebuserUid = 0;
			m_nNextInviteUid = 0;
		}
		else
		{
			pUser = NULL;
			if (m_userInfo.UserInfo.uid != RecvInfo.uKefuUin)
			{
				for (int i = 0; i < RecvInfo.InviteUin.size(); ++i)
				{
					if (RecvInfo.InviteUin[i] == m_userInfo.UserInfo.uid)
					{
						pUser = GetUserObjectByUid(m_userInfo.UserInfo.uid);
						pWebUser->onlineinfo.bInvited = true;
						break;
					}
				}
			}

			if (pUser == NULL)
			{
				pUser = GetUserObjectByUid(RecvInfo.uKefuUin);
				pWebUser->onlineinfo.bInvited = false;
			}

			if (RecvInfo.uTansferingToKefu == 0)
			{
				if (pUser != NULL)
				{
					if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid ||
						pWebUser->onlineinfo.bInvited)
					{
						//是自己转移的访客
						pWebUser->m_nWaitTimer = 0;
						pWebUser->m_bConnected = true;
					}
					else
					{
						//其他人转移的访客
						pWebUser->m_bConnected = false;
						pWebUser->m_nWaitTimer = -20;
					}


					//正在对话
					pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
					pWebUser->info.status = STATUS_ONLINE;
					pWebUser->transferuid = 0;



					pWebUser->talkuid = pUser->UserInfo.uid;//邀请用户的要改变talkid为自己的

					//访客转移
					//m_pFormMain->RecvUserAcceptChat(pWebUser, pUser);

					//访客移到客服的对话列表
					//int nRet = m_pFormUser->MoveWebUserToUserTalkItem(pWebUser, pUser);
					//if (nRet < 0)
					//m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
					//else if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
					//AddMultiWebUserToList(pWebUser);//把接入的会话移动到会话列表中


					if (RecvInfo.uKefuUin&&pUser->m_bFriend
						|| !RecvInfo.uKefuUin&&!pWebUser->m_bNotResponseUser)
					{
						//读取会话历史消息，并接受新的消息
						//StartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);
					}
				}
			}
			else
			{
				//正在转移
				pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
				pWebUser->info.status = STATUS_ONLINE;
				pWebUser->transferuid = RecvInfo.uTansferingToKefu;
				CUserObject *pAcceptUser = GetUserObjectByUid(RecvInfo.uTansferingToKefu);

				//这里必须先在htmleditor中显示，然后再移动位置，因为移动位置可能会导致区域的切换，其他地方同样考虑
				if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
					strcpy(pWebUser->chatid, RecvInfo.chatid);
				pWebUser->floatadminuid = RecvInfo.uAdminId;

				//if (pUser != NULL)
				//	m_pFormMain->SendInviteToWebUser(pUser, pWebUser, INVITE_TRANSFER, pAcceptUser);
				//else if (pAcceptUser != NULL)
				//	m_pFormMain->RecvTransferWebUser(pWebUser, pAcceptUser->UserInfo.nickname);

				//m_pFormUser->DeleteChildItem((DWORD)pUser, TREELPARAM_USER_TALK, (DWORD)pWebUser);
				//m_pFormUser->MoveWebUserToUserTransferItem(pWebUser, pAcceptUser);


				//这里处理是不是需要用户手动接受
				if (RecvInfo.uTansferingToKefu == this->m_userInfo.UserInfo.uid)
				{

				}
			}
		}
	}

	pWebUser->m_bNotResponseUser = 1;
	for (iter = RecvInfo.webRecvUin.begin(); iter != RecvInfo.webRecvUin.end(); iter++)
	{
		unsigned long uid = (unsigned long)(*iter);

		if (uid == m_userInfo.UserInfo.uid)
		{
			pWebUser->m_bNotResponseUser = 0;
			break;
		}
	}

	for (iter = RecvInfo.InviteUin.begin(); iter != RecvInfo.InviteUin.end(); iter++)
	{
		unsigned long uid = (unsigned long)(*iter);

		pWebUser->AddCommonTalkId(uid);
	}

	m_baseMsgs->RecvChatInfo(pWebUser);

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatChatMsg(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	if (packhead.uin == m_userInfo.UserInfo.uid)
		return 0;

	COM_FLOAT_CHATMSG RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	int msgType = MSG_TYPE_NORMAL;
	int msgFrom = MSG_FROM_USER;
	char msgTime[MAX_256_LEN];
	WxMsgBase* msgContentWx = NULL;
	CUserObject *pAssistUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatmsg unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatmsg getwebuserobjectbyid(%u) failed", RecvInfo.webuin);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatChatMsg chatid:%s,uAdminId:%u,uWebuin:%u,msgtype:%d,sendname:%s,strfontinfo:%s,msg:%s",
		RecvInfo.chatid, RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.nMsgType, RecvInfo.nickname, RecvInfo.strfontinfo, RecvInfo.strmsg);

	if (pWebUser->m_sWxAppid.empty()) //WxAppid为空，去服务端获取
	{
		SendTo_GetWebUserInfo(pWebUser->webuserid, RecvInfo.chatid);
	}

	WxMsgBase* pWxMsg = NULL;
	if (strcmp(RecvInfo.strfontinfo, "JSON=WX") == 0)
	{
		pWebUser->m_bIsFrWX = true;
		pWxMsg = ParseWxMsg(pWebUser, RecvInfo);
	}
	else
	{
		std::string content = RecvInfo.strmsg;

		bool isUrl = false;// PathIsURL(content.c_str());
		if (isUrl && pWebUser->m_bIsFrWX)
		{
			content = "<a href=\"" + content + "\" target=\"blank\">" + content + "</a>";
		}

		TransforFaceMsg(content);
		strncpy(RecvInfo.strmsg, content.c_str(), MAX_MSG_RECVLEN);
	}

	if (RecvInfo.strRand[0] != 0)
	{
		strcpy(pWebUser->info.sid, RecvInfo.strRand);
	}

	if (RecvInfo.strThirdid[0] != 0)
	{
		strcpy(pWebUser->info.thirdid, RecvInfo.strThirdid);
	}

	if (packhead.uin != RecvInfo.webuin)
	{
		// 不是微信用户发来的消息，就是协助对象发来的消息
		pAssistUser = GetUserObjectByUid(packhead.uin);
		if (pAssistUser == NULL)
		{
			// 根据uin和昵称先初始化坐席简单信息
			pAssistUser = AddUserObject(packhead.uin, "", RecvInfo.nickname, STATUS_ONLINE, -1);

			// 然后更新该坐席的最新信息
			//GetUserInfoFromSrv(packhead.uin);
		}
	}

	if (packhead.langtype == LANGUAGE_UTF8)
	{
		ConvertMsg(RecvInfo.strmsg, sizeof(RecvInfo.strmsg) - 1);

		ConvertMsg(RecvInfo.nickname, sizeof(RecvInfo.nickname) - 1);
	}
	//访客发来消息
	switch (RecvInfo.nMsgType)
	{
	case MSG_FLOAT_NORMAL:
	case MSG_FLOAT_PRV:
	case MSG_FLOAT_INPUTING:
	case MSG_WX://微信消息 
	{
		if (pWebUser->m_nWaitTimer<0)
		{
			pWebUser->m_nWaitTimer = 0;
		}

		// 排队等待接受后发消息不能屏蔽 [12/9/2010 SC]
		if (m_sysConfig->IsWebuserSidForbid(pWebUser->info.sid))
		{
			//消息屏蔽了，直接跳出不处理
			goto RETURN;
		}

		if (RecvInfo.nMsgType == 2)
		{
			string strMsg = RecvInfo.strmsg;
			transform(strMsg.begin(), strMsg.end(), strMsg.begin(), ::toupper);
			if (strMsg.find("userfile") > -1 && strMsg.find("收到文件") > -1)
			{
				// 接待的坐席给微信对象发的					
				if (strMsg.find(".jpg") > -1 || strMsg.find(".jpeg") > -1 || strMsg.find(".bmp") > -1 || strMsg.find(".png") > -1)
				{
					RecvInfo.nMsgDataType = MSG_DATA_TYPE_IMAGE;
				}
				else
				{
					RecvInfo.nMsgDataType = MSG_DATA_TYPE_FILE;
				}
			}
			else if (strMsg.find("http:") > -1 && (strMsg.find(".jpg") > -1 || strMsg.find(".jpeg") > -1 ||
				strMsg.find(".bmp") > -1 || strMsg.find(".png") > -1) || strMsg.find("收到一个图片") > -1)
			{
				RecvInfo.nMsgDataType = MSG_DATA_TYPE_IMAGE;
			}
			else if (strMsg.find("userfile") > -1)
			{
				// 接待的坐席给web用户发的
				RecvInfo.nMsgDataType = MSG_DATA_TYPE_FILE;
			}

			if (pAssistUser == NULL)
			{
				// 访客发来消息时，其未应答等待时间清零
				pWebUser->m_resptimeoutmsgtimer = -1;
				pWebUser->m_resptimeoutclosetimer = -1;

				if (pWebUser->m_waitresptimeouttimer < 0)
					pWebUser->m_waitresptimeouttimer = 0;
			}
			else
			{
				msgFrom = MSG_FROM_ASSIST;
			}

			// 界面应做来消息提示
		}
		else
		{
			msgType = MSG_TYPE_PREV;
		}

		pWebUser->m_sNewSeq = packhead.sendrandom;

		if (RecvInfo.tMsgTime == 0)
		{
			// 获取系统当前时间
		}

		GetTimeStringMDAndHMS(RecvInfo.tMsgTime, msgTime);

		m_baseMsgs->RecvOneMsg((IBaseObject*)pWebUser, msgFrom, GetMsgId(),	msgType, RecvInfo.nMsgDataType, 
			RecvInfo.strmsg, msgTime, pAssistUser, msgContentWx, "");


		// 同步更新关联词
		//if (m_sysConfig.bAutoSearchKeyword)
		{
			//CUserObject *pUser = m_pFormKeyWord->GetCurSelUserOb();
			//m_pFormKeyWord->ResetKeyWord((m_nOnLineStatus != STATUS_OFFLINE), pUser, RecvInfo.strmsg);
		}

		if (m_sysConfig->m_bAutoRespUnnormalStatus)
		{
			string strResp;

			switch (this->m_nOnLineStatus)
			{
			case STATUS_BUSY:
			case STATUS_WORK:
			case STATUS_REFUSE_NEWWEBMSG:
				strResp = m_sysConfig->m_strUnnormalStatusMsg[0];
				break;
			case STATUS_LEAVE:
			case STATUS_EATING:
			case STATUS_OFFLINE:
				strResp = m_sysConfig->m_strUnnormalStatusMsg[1];
				break;
			}

			if (!strResp.empty())
			{
				//SendAutoRespMsg(pWebUser, LPCTSTR(strResp));
			}
		}
	}
		break;
	case MSG_WORKBILL:
	case MSG_CLIENT_COME:
		g_WriteLog.WriteLog(C_LOG_TRACE, "--recvfloatchatmsg MSG_WORKBILL msgtype--");
		//RecvComSendWorkBillMsg(RecvInfo.webuin, -1, RecvInfo.strmsg, RecvInfo.nickname);
		break;
	default:
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatmsg unkown msgtype:%d", RecvInfo.nMsgType);
		break;
	}

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatChatMsgAck(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CHATMSG_ACK RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatmsgack unpack failed,Cmd:%.4x", packhead.cmd);

		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "recvfloatchatmsgack uAdminId:%u,uWebuin:%u,chatid:%s,msgseq:%hu",
		RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.chatid, RecvInfo.chatseq);

	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);

	if (pWebUser != NULL)
		pWebUser->m_sNewSeq = RecvInfo.chatseq;

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatAcceptChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_ACCEPTCHAT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;
	int nRet = 0;
	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatacceptchat unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "recvfloatacceptchat sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s,nickname:%s",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.nickname);


	pUser = GetUserObjectByUid(packhead.uin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatacceptchat GetWebUserObjectByID(%u) failed", RecvInfo.uWebuin);
		goto RETURN;
	}
	if (m_userInfo.UserInfo.uid != packhead.uin)//接受者不是当前坐席
	{
		pWebUser->m_nWaitTimer = -20;
		pWebUser->m_bConnected = HASINSERTRIGHT;
	}
	pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
	pWebUser->transferuid = 0;
	pWebUser->talkuid = packhead.uin;
	if (pUser == NULL || (m_userInfo.UserInfo.uid != pUser->UserInfo.uid&&!pUser->m_bFriend))
	{
		pWebUser->cTalkedSatus = HASTALKED;
		//m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
		g_WriteLog.WriteLog(C_LOG_ERROR, "delete user name is :%s, chatid: %s", pWebUser->info.name, pWebUser->chatid);
	}
	else
	{
		pWebUser->cTalkedSatus = INTALKING;
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
		if (packhead.uin == m_userInfo.UserInfo.uid)
		{
			//是自己接受邀请的访客
			pWebUser->m_nWaitTimer = 0;
			pWebUser->m_bConnected = TRUE;
		}

		//其他人邀请的访客
		//pWebUser->RemoveMutiUser(pWebUser->talkuid);
		pWebUser->inviteuid = 0;
		//访客邀请
		//m_pFormMain->RecvUserAcceptChat(pWebUser, pUser);

		//访客移到接受客服的对话列表
		//nRet = m_pFormUser->MoveWebUserToUserTalkItem(pWebUser, pUser);
		//if (nRet < 0)
		//	m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
		//else if (packhead.uin == m_pUserInfo->UserInfo.uid)
		//	AddMultiWebUserToList(pWebUser);//把接入的会话移动到会话列表中
		////读取会话历史消息，并接受新的消息
		//StartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);

	}
	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatTransQuest(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_TRANSREQUEST RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pInviteUser = NULL;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;
	char sztime[256] = { 0 };
	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloattransquest unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatTransQuest sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid);


	pAcceptUser = GetUserObjectByUid(RecvInfo.uToKefu);
	pInviteUser = GetUserObjectByUid(packhead.uin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	if (pAcceptUser == NULL)//获取不到，就不是自己的协助对象
	{
		goto RETURN;
	}
	//转移会话，要从用户群列表中删除
	//DelWebUserInMultiList(pWebUser);

	if (pWebUser == NULL)
	{
		//SendTo_GetWebUserInfo(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid);
	}
	else if (RecvInfo.uToKefu == m_userInfo.UserInfo.uid)//当前坐席是接受者，移动到转接中。。。
	{
		if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
			strcpy(pWebUser->chatid, RecvInfo.chatid);
		pWebUser->floatadminuid = RecvInfo.uAdminId;
		if (!pWebUser->m_bNewComm)
		{
			pWebUser->m_bNewComm = true;
		}
		pWebUser->gpid = packhead.random;
		pWebUser->info.status = STATUS_ONLINE;
		pWebUser->onlineinfo.bInvited = false;//转接过来，就不是邀请协助了
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
		pWebUser->transferuid = RecvInfo.uToKefu;

		//if (pInviteUser != NULL)
		//	m_pFormMain->SendInviteToWebUser(pInviteUser, pWebUser, INVITE_TRANSFER, pAcceptUser);
		//else
		//	m_pFormMain->RecvTransferWebUser(pWebUser, pAcceptUser->UserInfo.nickname);

		//m_pFormUser->DeleteChildItem((DWORD)pInviteUser, TREELPARAM_USER_TALK, (DWORD)pWebUser);
		//m_pFormUser->MoveWebUserToUserTransferItem(pWebUser, pAcceptUser);

	}
	if (packhead.uin == m_userInfo.UserInfo.uid)//当前坐席是发起转接者
	{
		GetTimeStringMDAndHMS(0, sztime);
		//CString strTip = "";
		//已发起转接请求到%s
		//strTip.Format("已发起转接请求到%s", pAcceptUser->UserInfo.nickname);
		//m_pFormMain->AddToMsgList(pWebUser, "", sztime, strTip);

	}
	nError = 0;
RETURN:

	return nError;
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
	COM_FLOAT_CLOSECHAT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatclosechat unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCloseChat sender:%s(%u),uAdminId:%u,uWebuin:%u,chatid:%s,usType:%hu",
		RecvInfo.nickname, packhead.uin, RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.chatid, RecvInfo.usType);

	pUser = GetUserObjectByUid(packhead.uin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);
	if (pWebUser == NULL || strcmp(pWebUser->chatid, RecvInfo.chatid) != 0)
	{
		if (pWebUser == NULL)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatclosechat GetWebUserObjectByID(%u) failed", RecvInfo.webuin);
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatclosechat chatid(local:%s,pack:%s) not same", pWebUser->chatid, RecvInfo.chatid);
		}
		return nError;
	}

	if (CHATCLOSE_INVISTEXIT == RecvInfo.usType)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "recvfloatclosechat 被邀请坐席离线(%u)", packhead.uin);
		//被邀请的人退出邀请
		//pWebUser->RemoveMutiUser(packhead.uin);
		//添加提示
		//if (pUser != NULL)
		//{
		//	m_pFormMain->RecvInvestUserExit(pWebUser, packhead.uin, pUser->UserInfo.nickname);
		//}
		//else
		//	m_pFormMain->RecvInvestUserExit(pWebUser, packhead.uin, "");


		//pWebUser->RemoveMutiUser(packhead.uin);
		//if (!pWebUser->IsMutiUser())
		{
			//更改图标
			//m_pFormMain->m_pFormUser->ResetWebUserIcon(pWebUser);
		}
	}
	else
	{
		//结束会话，清空协助坐席
		//if (pWebUser->IsMutiUser())
		//	pWebUser->RemoveAllMutiUser();
		pWebUser->onlineinfo.bInvited = false;
		pWebUser->m_bNewComm = false;
		pWebUser->m_bConnected = true;
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
		pWebUser->cTalkedSatus = HASTALKED;
		string strMsg = "";
		if (CHATCLOSE_UNSUBSCRIBE == RecvInfo.usType)
		{
			strMsg = "微信用户取消关注结束会话";
		}
		else
		{
			strMsg = "访客会话已结束";
		}
		//m_pFormMain->AddToMsgList(pWebUser, "", "", strMsg);
		if (!pWebUser->IsOnline())//这里是不是该用户彻底离线了
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "recvfloatclosechat 坐席离线(%u)访客离线", packhead.uin);

			//SetVisitorOffline(pWebUser);
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "recvfloatclosechat 坐席离线(%u)访客访问中", packhead.uin);
			//访客退回到在线列表
			pWebUser->m_nWaitTimer = -20;
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
			//m_pFormUser->MoveWebUserToOnlineItem(pWebUser, pWebUser->IsOnline());
		}
		//结束会话，要从用户群列表中删除
		//DelWebUserInMultiList(pWebUser);
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvFloatListChat(PACK_HEADER packhead, char *pRecvBuff, int len)
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

int CChatManager::SendTo_GetShareList()
{
	int nError = SYS_ERROR_SEND_BEFORE_LOGIN;
	if (m_isLoginSuccess)
	{
		COM_FLOAT_SHARELIST SendInfo(VERSION);

		nError = SendPackTo(&SendInfo);
	}
	return nError;
}

int CChatManager::SendTo_GetListChatInfo()
{
	int nError = SYS_ERROR_SEND_BEFORE_LOGIN;
	if (m_isLoginSuccess)
	{
		COM_FLOAT_GETLIST SendInfo(VERSION);

		SendInfo.uAdminId = m_login->m_authAdminid;

		nError = SendPackTo(&SendInfo);
	}
	return nError;
}

CUserObject * CChatManager::AddUserObject(unsigned long id, char *sid, char *name, unsigned char status, unsigned long fd)
{
	CUserObject *pUser = GetUserObjectByUid(id);

	if (pUser == NULL)
	{
		pUser = new CUserObject();

		pUser->UserInfo.uid = id;

		m_mapUsers.insert(map<unsigned long, CUserObject*>::value_type(id, pUser));
		pUser->m_nFlag = 2;
	}
	else
	{
		pUser->m_nFlag = 1;
	}

	strcpy(pUser->UserInfo.sid, sid);
	strcpy(pUser->UserInfo.nickname, name);
	pUser->status = status;
	pUser->fd = fd;

	if (m_myInfoIsGet >= 0 && pUser->UserInfo.uid != 0 && pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
	{
		m_myInfoIsGet = -20;
	}
	return pUser;
}

CUserObject * CChatManager::GetUserObjectByUid(unsigned long id)
{
	if (id == 0)
	{
		return &m_CommUserInfo;
	}

	if (m_userInfo.UserInfo.uid != 0 && m_userInfo.UserInfo.uid == id)
		return &m_userInfo;

	CUserObject *pUser = NULL;

	map <unsigned long, CUserObject*>::iterator iter_user;
	iter_user = m_mapUsers.find(id);
	if (iter_user != m_mapUsers.end())
	{
		pUser = iter_user->second;
		return pUser;
	}
	else
	{
		return NULL;
	}
}

void CChatManager::TimerProc(string timeName, LPVOID pThis)
{
	CChatManager* chat_manager = (CChatManager*)pThis;
	if (timeName == TIMER_NAME_NORMAL)
	{
		if (chat_manager->m_nOnLineStatus == STATUS_OFFLINE)
			return;
		if (chat_manager->m_nMyInfoIsGet >= 0)
		{
			chat_manager->m_nMyInfoIsGet++;

			if (chat_manager->m_nMyInfoIsGet >= 3)
			{
				//如果自己的信息没有获取到，那么需要重复的去获取
				chat_manager->m_nMyInfoIsGet = 0;
				chat_manager->SendGetClkInfo(chat_manager->m_userInfo.UserInfo.uid, chat_manager->m_userInfo.UserInfo.sid, CMD_SRV_REP_USERINFO);
			}
		}
	}
	else if (timeName == TIMER_NAME_LOGIN)
	{
		if (!chat_manager->m_isLoginSuccess)
		{
			chat_manager->m_lastError = "登录超时";
		}
		chat_manager->m_timers->KillTimer(TIMER_NAME_LOGIN);
	}
}

int CChatManager::SendTo_GetAllUserInfo()
{
	int nError = SYS_ERROR_SEND_BEFORE_LOGIN;
	if (m_isLoginSuccess)
	{
		CUserObject *pUser;
		map <unsigned long, CUserObject*>::iterator iter_user;
		for (iter_user = m_mapUsers.begin(); iter_user != m_mapUsers.end(); iter_user++)
		{
			pUser = iter_user->second;
			if (pUser != NULL && strlen(pUser->UserInfo.nickname) == NULL)
			{
				nError = SendTo_GetUserInfo(pUser->UserInfo.uid);
			}
		}
	}
	return nError;
}

int CChatManager::SendTo_GetUserInfo(unsigned long uid)
{
	int nError = SYS_ERROR_SEND_BEFORE_LOGIN;
	if (m_isLoginSuccess)
	{
		if (uid == 0)
		{
			nError = 0;
		}
		else
		{
			CLT_GET_CLT_INFO SendInfo(VERSION);

			SendInfo.seq = GetPackSeq();
			SendInfo.uin = uid;
			SendInfo.type = UPDATE_ALLINFO;

			nError = SendPackTo(&SendInfo);
		}
	}

	return nError;
}

unsigned short CChatManager::GetPackSeq()
{
	++g_packSeq;
	if (g_packSeq == 0)
	{
		++g_packSeq;
	}
	return g_packSeq;
}

int CChatManager::SendGetClkInfo(unsigned long id, char *strid, unsigned short cmd, unsigned short cmdtype, unsigned short type)
{
	int nError = 0;
	CLT_GET_INFO SendInfo(VERSION, cmd);

	SendInfo.cmdtype = cmdtype;
	SendInfo.type = type;

	SendInfo.id = id;
	strcpy(SendInfo.strid, strid);

	nError = SendPackTo(&SendInfo);

	return nError;
}

void CChatManager::TimerSolveAck()
{
	if (m_nSendPing < 0)
		return;

	m_nSendPing++;

	//30秒发送一次刺激包
	if (m_nSendPing > 60)
	{
		if (m_nOnLineStatus != STATUS_OFFLINE)
		{
			// 发送ping命令包，跟服务端进程能约定的ping包
			if (SendPing() != 0)
			{
				m_nSendPingFail++;
			}
			else
			{
				m_nSendPingFail = 0;
			}

			if (m_nSendPingFail > 3)
			{
				// 累计3次连接不上，做掉线处理
				g_WriteLog.WriteLog(C_LOG_TRACE, "SendPingFailed3times--SetOfflineStatus ");
				SetOfflineStatus();

				CloseAllSocket();
			}
		}

		if (m_nOnLineStatusEx != STATUS_OFFLINE)
		{
			// 发送网络层级的ping包，确认服务器是否能连接
			if (SendPingToVisitorServer() != 0)
			{
				// 连接不上处理
				m_nOnLineStatusEx = STATUS_OFFLINE;
				m_nLoginToVisitor = 0;
			}
		}

		// 时间清零
		m_nSendPing = 0;
	}
}

int CChatManager::SendPing()
{
	int nError = 0;

	COM_SEND_PING SendInfo(VERSION);
	SendInfo.o.online_flag = m_userInfo.UserInfo.onlineflag;

	nError = SendPackTo(&SendInfo, 0, 0);

	return nError;
}

void CChatManager::SetOfflineStatus()
{
	
}

void CChatManager::CloseAllSocket()
{

}

int CChatManager::SendPingToVisitorServer()
{
	char sbuff[512];
	int nError(0);

	if (time(NULL) - m_tResentVisitPackTime > 60)
	{
		time(&m_tResentVisitPackTime);

		sprintf(sbuff, "<PING><PING>\r\n");
		nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	}
	return nError;
}

int CChatManager::SendBuffToVisitorServer(char *sbuff, int len)
{
	int nrtn = 0;
	int nError;
	g_VisitLog.WriteLog(C_LOG_TRACE, "send:%s", sbuff);

	if (!m_socketEx.SendBuff(sbuff, len, nError))
	{
		nrtn = SYS_ERROR_SENDFAIL;

		g_VisitLog.WriteLog(C_LOG_TRACE, "send visit pack failed:%s!", sbuff);
	}

	return nrtn;
}

int CChatManager::SendTo_UpdateOnlineStatus(unsigned short status)
{
	int nError = SYS_ERROR_SEND_BEFORE_LOGIN;
	if (m_isLoginSuccess)
	{
		unsigned int dwstatus = 0;

		CLT_UPDATE_STATUS SendInfo(VERSION);

		SendInfo.seq = GetPackSeq();
		SendInfo.bSendToServer = true;
		SendInfo.type = UPDATE_STATUS;
		SendInfo.onlinestatus = SetMutiByte(dwstatus, ONLINE_INFO_STATUS, ONLINE_INFO_STATUS_LEN, (DWORD)status);

		nError = SendPackTo(&SendInfo);
	}
	return nError;
}

CWebUserObject * CChatManager::GetWebUserObjectBySid(char *sid)
{
	return NULL;
}

CWebUserObject * CChatManager::AddWebUserObject(char *sid, char *thirdid, char *name, char *scriptflag, char *url, unsigned char status, unsigned char floatauth)
{
	return NULL;
}

CWebUserObject * CChatManager::GetWebUserObjectByUid(unsigned long uid)
{
	return NULL;
}

int CChatManager::SendTo_GetWebUserInfo(unsigned long webuserid, const char *chatid, char *szMsg, unsigned int chatkefuid)
{
	int nError = 0;
	COM_SEND_MSG SendInfo(VERSION);
	Json::Value jv;
	if (szMsg != NULL && chatkefuid > 0)
	{
		jv["ReturnParameters"] = szMsg;
		jv["ChatKefu"] = chatkefuid;
	}
	else if (chatkefuid > 0)
	{
		jv["ChatKefu"] = chatkefuid;
	}
	else if (szMsg != NULL){
		jv["ReturnParameters"] = szMsg;
	}

	SendInfo.msg.msgtype = MSG_GET_WORKBILL;
	SendInfo.msg.recvuin = webuserid;
	SendInfo.msg.sendtime = 0;
	strncpy(SendInfo.strChatid, chatid, MAX_CHATID_LEN);
	SendInfo.msg.bak = 0;
	SendInfo.msg.seq = GetPackSeq();
	SendInfo.version = VERSION;
	SendInfo.msg.senduin = m_userInfo.UserInfo.uid;
	strncpy(SendInfo.msg.strmsg, jv.toStyledString().c_str(), MAX_MSG_RECVLEN);
	g_WriteLog.WriteLog(C_LOG_TRACE, "GET_WORKBILL: recvuin:%u,senduin:%u，szMsg=%s", SendInfo.msg.recvuin, SendInfo.msg.senduin, szMsg);

	nError = SendPackTo(&SendInfo);

	return nError;
}

void CChatManager::TransforFaceMsg(string& str)
{
	char buf[8];
	std::string::size_type pos, n;
	const std::string *fs;
	const wx_face_t *face = wx_faces;
	for (int i = 0; i < _countof(wx_faces); ++i, ++face)
	{
		for (pos = 0;;)
		{
			fs = &face->raw;
			if (std::string::npos == (n = str.find(*fs, pos)))
			{
				fs = &face->enc;
				if (face->enc.empty() || std::string::npos == (n = str.find(*fs, pos)))
					break;
			}
			pos = n;
			_itoa(i, buf, 10);
			str.insert(pos, FACE_PREFIX);
			pos += sizeof(FACE_PREFIX)-1;
			str.insert(pos, m_initConfig.webpage_FaceImage);
			pos += strlen(m_initConfig.webpage_FaceImage);
			str.insert(pos, buf);
			pos += strlen(buf);
			str.replace(pos, fs->length(), FACE_ALT FACE_SUFFIX);
			pos += sizeof(FACE_ALT FACE_SUFFIX) - 1;
		}
	}

	// deal with android/ios emoji face
	int len = str.length() - 3;
	for (int i = 0; i < len; ++i)
	{
		if (str[i] == -16 && str[i + 1] == -97)
		{
			str[i] = -18;
			str[i + 1] = -112;
			str[i + 2] = -116;
			str.erase(i + 3, 1);
		}
	}
}

WxMsgBase* CChatManager::ParseWxMsg(CWebUserObject* pWebUser, COM_FLOAT_CHATMSG& recvInfo)
{
	//微信消息类型，utf8编码类型 
	WxMsgBase* msgBase = NULL;
	WxObj *pwxobj = ParseWxJsonMsg(recvInfo.strmsg);

	if (pwxobj != NULL)
	{
		if ("userinfo" == pwxobj->MsgType)
		{
			// 此处注意，如果第二次收到userinfo，应当将前面收到那次信息析构掉
			if (pWebUser->m_pWxUserInfo == NULL)
			{
				pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
			}
			else
			{
				delete pWebUser->m_pWxUserInfo;
				pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
			}
			pWebUser->m_bIsGetInfo = true;
		}
		else if ("wxactoken" == pwxobj->MsgType)
		{
			//todo:收到服务器发过来的微信的access_token消息
			WxTokens::iterator iter = m_mapTokens.find(recvInfo.strThirdid);
			if (iter == m_mapTokens.end())
			{
				m_mapTokens.insert(map<string, string>::value_type(recvInfo.strThirdid, ((WxAccessTokenInfo*)pwxobj)->szAcToken));
			}
			else
			{
				iter->second = ((WxAccessTokenInfo*)pwxobj)->szAcToken;
			}
			delete pwxobj;
		}
		else if ("image" == pwxobj->MsgType)
		{
			//GetWxUserInfoAndToken(pWebUser);
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_IMAGE;
		}
		else if ("voice" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_VOICE;
		}
		else if ("video" == pwxobj->MsgType || "shortvideo" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_VIDEO;
		}
		else if ("location" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_LOCATION;
		}
		else if ("link" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_LINK;
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatmsg() unknow weixin Json:%s", recvInfo.strmsg);
			delete pwxobj;
		}
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "recvfloatchatmsg() NOT the weixin Json:%s", recvInfo.strmsg);
	}
	return msgBase;
}

string CChatManager::GetMsgId()
{
	char msgid[256];
	sprintf(msgid, "id_%d", m_msgId);
	m_idLock.Lock();
	m_msgId++;
	m_idLock.Unlock();
	return msgid;
}

void CChatManager::SaveEarlyMsg(MSG_INFO *pMsgInfo)
{
	MSG_INFO *pInfo = new MSG_INFO;

	memcpy(pInfo, pMsgInfo, sizeof(MSG_INFO));

	m_listEarlyMsg.push_back(pInfo);
}

int CChatManager::SendAckEx(unsigned short seq, unsigned long uid /*= 0*/, unsigned long ip /*= 0*/, unsigned short port /*= 0*/)
{
	int nError = 0;
	COM_ACKEX SendInfo(VERSION);

	SendInfo.seq = seq;
	SendInfo.uin = uid;
	SendInfo.ip = ip;
	SendInfo.port = port;

	nError = SendPackTo(&SendInfo);

	return nError;
}

int CChatManager::SendAutoRespMsg(CWebUserObject *pWebUser, const char *msg, BOOL bClearTimer /*= true*/)
{
	int rtn = SYS_ERROR_SENDFAIL;
	if (pWebUser == NULL || msg == NULL || strlen(msg) <= 0)
		return rtn;

	if (!pWebUser->m_bConnected)
	{
		return rtn;
	}

	//rtn = SendMsgToWebUser(pWebUser, msg);
	if (rtn == 0)
	{
		//m_pFormMain->SendComMsg(pWebUser, LPCTSTR(msg));

		if (bClearTimer && pWebUser != NULL)
		{
			pWebUser->m_resptimeoutmsgtimer = -1;
			pWebUser->m_resptimeoutclosetimer = -1;
			pWebUser->m_waitresptimeouttimer = -1; // 0 to -1 自己发送消息后 就只等待客户应答 [12/29/2010 SC]
		}
	}
	else
	{
		//m_pFormMain->AddSysBuffToListMsg(pWebUser, "自动回复消息失败!");
	}

	return rtn;
}

int CChatManager::SendWebuserTalkBegin(CWebUserObject *pWebUser)
{
	if (pWebUser == NULL)
		return SYS_ERROR_SENDFAIL;

	char sbuff[512];
	int nError;

	sprintf(sbuff, "<SCRIPTMSG><COMMAND>TALKBEG</COMMAND><ADMINID>%lu</ADMINID><CLIENTID>%s</CLIENTID><SERVICEUIN>%lu</SERVICEUIN><NICKNAME>%s</NICKNAME></SCRIPTMSG>\r\n",
		pWebUser->floatadminuid, pWebUser->info.sid, m_userInfo.UserInfo.uid, m_userInfo.UserInfo.nickname);

	nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	return nError;
}

int CChatManager::SendWebuserTalkEnd(CWebUserObject *pWebUser)
{
	if (pWebUser == NULL)
		return SYS_ERROR_SENDFAIL;

	char sbuff[512];
	int nError;

	sprintf(sbuff, "<SCRIPTMSG><COMMAND>TALKEND</COMMAND><ADMINID>%lu</ADMINID><CLIENTID>%s</CLIENTID><SERVICEUIN>%lu</SERVICEUIN><NICKNAME>%s</NICKNAME></SCRIPTMSG>\r\n",
		pWebUser->floatadminuid, pWebUser->info.sid, m_userInfo.UserInfo.uid, m_userInfo.UserInfo.nickname);

	nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	return nError;
}

void CChatManager::RecvComSendWorkBillMsg(unsigned long senduid, unsigned long recvuid, char *msg, char* mobile)
{
	char sid[MAX_WEBCLIENID_LEN + 1] = { 0 }, billid[MAX_CHATID_LEN + 1] = { 0 }, szReturnParameters[51] = { 0 };
	CWebUserObject *pWebUser;
	string strMsg = "", strReturnParameters = "";
	strMsg = msg;
	int nPos = strMsg.find("ReturnParameters:");
	strReturnParameters = strMsg.substr(0, strMsg.size() - nPos - 17);
	if (!GetContentBetweenString(msg, "WorkBillID:", "\n", billid))
	{
		billid[0] = 0;
	}
	if (GetContentBetweenString(msg, "rand=", "&", sid))
	{
		char thirdid[MAX_THIRDID_LEN + 1] = { 0 };
		GetContentBetweenString(msg, "rand=", "&", sid);
		GetContentBetweenString(msg, "clientid=", "&", thirdid);

		char chatfrom[MAX_CHATID_LEN + 1] = { 0 };
		GetContentBetweenString(msg, "chatfrom=", "&", chatfrom);

		char wxappid[100] = { 0 };
		GetContentBetweenString(msg, "wxappid=", "&", wxappid);
		GetContentBetweenString(msg, "ReturnParameters:", "\0", szReturnParameters);
		pWebUser = GetWebUserObjectByUid(senduid);
		if (pWebUser != NULL)
		{
			//打开webui时如果get行没有参数rand，webui将rand设为clientuin
			if (strlen(sid) > 0 || ((unsigned long)atol(sid)) != senduid)
			{
				//ChangeWebUserSid(pWebUser, sid, thirdid);
			}
			//来了访客，肯定是在线的
			if (pWebUser->info.status == STATUS_OFFLINE)
				pWebUser->info.status = STATUS_ONLINE;
			if (senduid > WEBUSER_UIN && pWebUser->webuserid != senduid)
			{
				pWebUser->webuserid = senduid;
			}
		}
		else
		{

			pWebUser = GetWebUserObjectBySid(sid);

			if (pWebUser == NULL)
			{
				pWebUser = AddWebUserObject(sid, "", mobile, "", "", STATUS_ONLINE, 0);

				SendTo_GetUserInfo(senduid);
			}

			pWebUser->webuserid = senduid;
			SolveWebUserEarlyMsg(pWebUser);
		}

		if (pWebUser != NULL&&billid[0] != 0)
		{
			strncpy(pWebUser->info.chatfrom, chatfrom, MAX_CHATID_LEN);
			strncpy(pWebUser->chatid, billid, MAX_CHATID_LEN);
			pWebUser->m_sWxAppid = wxappid;

			// 微信用户，主动获取微信的userinfo
			if (!pWebUser->m_sWxAppid.empty())
			{
				pWebUser->m_bIsFrWX = true;
				//GetWxUserInfoAndToken(pWebUser);
			}

			time_t tnow = time(NULL);

			if (!pWebUser->GetNormalChatHisMsgSuccess
				&&tnow - pWebUser->tGetNormalChatHismsgTime > 3) //判断超时这个最好是有定时器判断下，防止收到不服务器回应吧
			{
				//GetNormalChatHisMsg(senduid, billid);// 如果是非等待应答的才需要去获取消息记录
				pWebUser->tGetNormalChatHismsgTime = tnow;
			}
		}

		if (strReturnParameters.find("transfer") >= 0)//转接中
		{
			//正在转移
			//strReturnParameters.Remove('\\');
			//strReturnParameters.Remove('\n');
			Json::Value jv;
			//if (!ParseJson(strReturnParameters.GetBuffer(0), jv))
			{
				return;
			}
			//判断返回成功失败结果
			unsigned long uTransuid = GetIntFromJson(jv, "transfer");
			if (pWebUser->transferuid != m_userInfo.UserInfo.uid)
			{
				pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
				pWebUser->info.status = STATUS_ONLINE;
				pWebUser->transferuid = m_userInfo.UserInfo.uid;
				pWebUser->talkuid = uTransuid;

				//m_pFormMain->RecvTransferWebUser(pWebUser, m_userInfo.UserInfo.nickname);
				//m_pFormUser->MoveWebUserToUserTransferItem(pWebUser, m_pUserInfo);
				//m_tranferList[pWebUser->webuserid] = 0;
				//KillTimer(TIMER_TRANS_TIMEOUT);
				//SetTimer(TIMER_TRANS_TIMEOUT, 1000, NULL);
			}
		}
		else if (strReturnParameters.find("waiter") >= 0) {
		}
		else if ((pWebUser->cTalkedSatus != INTALKING || !pWebUser->m_bConnected)
			&& m_userInfo.UserInfo.uid == recvuid && !pWebUser->m_bNewComm)//非等待应答的会话
		{
			SendWebuserTalkBegin(pWebUser);

			pWebUser->cTalkedSatus = INTALKING;

			pWebUser->talkuid = m_userInfo.UserInfo.uid;

			pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
			pWebUser->transferuid = 0;
			pWebUser->m_nWaitTimer = 0;
			pWebUser->m_bConnected = TRUE;

			//int nRet = this->m_pFormUser->MoveWebUserToUserTalkItem(pWebUser, m_pUserInfo);
			//if (nRet < 0)
			//	m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
			//else
			//	AddMultiWebUserToList(pWebUser);//把接入的会话移动到会话列表中
			
			// 提示
		}
	}
}

void CChatManager::SolveWebUserEarlyMsg(CWebUserObject *pWebUser)
{
	list<MSG_INFO*>::iterator iter = m_listEarlyMsg.begin();

	for (iter; iter != m_listEarlyMsg.end(); iter++)
	{
		if (pWebUser->webuserid == (*iter)->senduin)
		{
			pWebUser->m_nWaitTimer = 0;
			//this->m_pFormMain->RecvComMsg(pWebUser, msginfo, true);
			if (strlen(pWebUser->info.name) <= 0)
			{
				//strcpy(pWebUser->info.name, msginfo->strmobile);
				//if (strlen(pWebUser->info.ipfromname) <= 0)
				//	strcpy(pWebUser->info.ipfromname, pWebUser->info.name);
				if (strlen(pWebUser->info.name) < 2)
				{
					g_WriteLog.WriteLog(C_LOG_ERROR, "name length is null：SolveWebUserEarlyMsg() name length：%d", strlen(pWebUser->info.name));
				}
			}
		}
	}

	
	for (iter = m_listEarlyMsg.begin(); iter != m_listEarlyMsg.end(); iter++)
	{
		if (pWebUser->webuserid == (*iter)->senduin)
		{
			delete (*iter);
			m_listEarlyMsg.erase(iter);
			return;
		}
	}
}

void CChatManager::RecvComSendNormalChatidHisMsg(unsigned long senduid, unsigned long recvuid, COM_SEND_MSG& RecvInfo)
{
	if (!g_NomalChatHistoryMsgManage.ParseJsonHisMsg(RecvInfo.strChatid, RecvInfo.msg.strmsg))
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendNormalChatidHisMsg failed,chatid:%s,msg:%s", RecvInfo.strChatid, RecvInfo.msg.strmsg);
		return;
	}
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComSendNormalChatidHisMsg msg:%s", RecvInfo.msg.strmsg);
	
	
	CNChatHisMsg nchatmsg;
	if (CNomalChatHistoryMsgManage::NHISMSGTYPE::SUCCESS != g_NomalChatHistoryMsgManage.GetCompleteMsgs(RecvInfo.strChatid, nchatmsg))
	{
		return;
	}
	
	//显示历史消息，并设置已获取历史消息状态
	
	CWebUserObject *pWebUser = GetWebUserObjectBySid(RecvInfo.strChatid);
	if (pWebUser != NULL)
	{
		pWebUser->GetNormalChatHisMsgSuccess = true; // 二次会话需要重置变量
	}
}

int CChatManager::RecvComTransRequest(unsigned long senduid, COM_SEND_MSG& RecvInfo)
{
	int nError = 0;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComTransRequest ：%s", RecvInfo.msg.strmsg);

	pAcceptUser = GetUserObjectByUid(RecvInfo.msg.recvuin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.msg.senduin);
	if (pAcceptUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComTransRequest GetUserObject failed");
		goto RETURN;
	}

	if (pWebUser == NULL)
	{
		pWebUser = GetWebUserObjectBySid(RecvInfo.strRand);
	}

	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComTransRequest pWebUser==NULL");
		char strMsg[MAX_256_LEN];
		sprintf(strMsg, "{\"transfer\":%u}", senduid);
		SendTo_GetWebUserInfo(RecvInfo.msg.senduin, RecvInfo.strChatid, strMsg, senduid);
	}
	else if (RecvInfo.msg.recvuin == m_userInfo.UserInfo.uid && pWebUser->transferuid != m_userInfo.UserInfo.uid)//当前坐席是接受者，移动到转接中。。。
	{
		if (strcmp(pWebUser->chatid, RecvInfo.strChatid) != 0 && strlen(RecvInfo.strChatid) > 0)
			strcpy(pWebUser->chatid, RecvInfo.strChatid);
		pWebUser->talkuid = senduid;
		pWebUser->m_bNewComm = false;
		pWebUser->info.status = STATUS_ONLINE;
		if (RecvInfo.msg.senduin > WEBUSER_UIN && pWebUser->webuserid != RecvInfo.msg.senduin)
		{
			pWebUser->webuserid = RecvInfo.msg.senduin;
		}

		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
		pWebUser->transferuid = m_userInfo.UserInfo.uid;

		//m_pFormMain->RecvTransferWebUser(pWebUser, pAcceptUser->UserInfo.nickname);

		//m_pFormUser->MoveWebUserToUserTransferItem(pWebUser, pAcceptUser);
		
		//获取历史消息

		//设置超时定时器
		//m_TranfserList[pWebUser->webuserid] = 0;
		//KillTimer(TIMER_TRANS_TIMEOUT);
		//SetTimer(TIMER_TRANS_TIMEOUT, 1000, NULL);
	}

	nError = 0;
RETURN:
	return nError;
}

int CChatManager::RecvComTransAnswer(unsigned long senduid, COM_SEND_MSG& RecvInfo)
{
	int nError = 0;
	CUserObject *pAcceptUser = NULL;//主转接坐席
	CWebUserObject *pWebUser = NULL;
	string strMsg = "";
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComTransAnswer ：%s,recvuin:%u", RecvInfo.msg.strmsg, RecvInfo.msg.recvuin);

	pAcceptUser = GetUserObjectByUid(senduid);
	pWebUser = GetWebUserObjectByUid(RecvInfo.msg.senduin);
	if (senduid == 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComTransAnswer GetUserObject获取被转接人失败：%u", senduid);
		nError = -1;
		goto RETURN;
	}
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComTransAnswer GetWebUserObject获取用户失败：chatid%s", RecvInfo.strChatid);
		nError = -2;
		goto RETURN;
	}

	strMsg = RecvInfo.msg.strmsg;
	if (strMsg.find("OK") >= 0)//同意转接
	{
		//坐席同意后发送CLT_TRANSFERCLIENT包
		strMsg = pWebUser->info.name;
		strMsg = "对方同意您对访客[%s]的转接" + strMsg;
		//SendTransferClinet(pAcceptUser, pWebUser, senduid);
	}
	else if (strMsg.find("NO") >= 0)//拒绝转接
	{
		strMsg = pWebUser->info.name;
		strMsg = "对方拒绝您对访客[%s]的转接" + strMsg;
	}
	else if (strMsg.find("TIMEOUT") >= 0)//超时
	{
		strMsg = pWebUser->info.name;
		strMsg = "您对访客[%s]的转接超时" + strMsg;
	}

RETURN:
	return nError;
}

int CChatManager::SendTo_GetWebUserChatInfo(unsigned short gpid, unsigned long adminid, char *chatid)
{
	int nError = 0;
	COM_FLOAT_CHATINFO SendInfo(VERSION, gpid);

	SendInfo.uAdminId = adminid;
	strcpy(SendInfo.chatid, chatid);

	nError = SendPackTo(&SendInfo);
	g_WriteLog.WriteLog(C_LOG_TRACE, "SendTo_GetWebUserChatInfo chatid:%s,gpid:%u", chatid, gpid);
	return nError;
}
