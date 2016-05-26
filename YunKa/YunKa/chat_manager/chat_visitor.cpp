#include "../stdafx.h"
#include "chat_visitor.h"
#include "chat_manager.h"
#include "../chat_common/comdef.h"
#include "../chat_common/commsg.h"

CChatVisitor::CChatVisitor()
{
	m_tResentVisitPackTime = 0;
	m_socketEx.SetReceiveObject(this);
}

CChatVisitor::~CChatVisitor()
{

}

void CChatVisitor::OnReceive(void* wParam, void* lParam)
{
	if (m_manager->m_bExit) return;

	static char visitlogmame[100] = { 0 };
	if (!visitlogmame[0])
	{
		sprintf(visitlogmame, "_%s(%lu)_visit.log", m_manager->m_userInfo.UserInfo.sid, m_manager->m_userInfo.UserInfo.uid);
		g_VisitLog.InitLog(GetCurrentPath() + "\\log", visitlogmame, C_LOG_ALL);
	}

	time(&m_tResentVisitPackTime);

	int len = *(int*)wParam;
	assert(len <= PACKMAXLEN);
	char *pBuff = (char *)lParam;

	g_VisitLog.WriteLog(C_LOG_TRACE, "recv:%s", pBuff);

	if (IsXMLCommandStringExist(pBuff, "SYSTEM"))
		SolveVisitorSystem(pBuff);
	else if (IsXMLCommandStringExist(pBuff, "SCRIPTMSG"))
		SolveVisitorSCRIPTMSG(pBuff);

	return;
}

int CChatVisitor::SendPingToVisitorServer()
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

int CChatVisitor::SendBuffToVisitorServer(char *sbuff, int len)
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

int CChatVisitor::SendWebuserTalkBegin(CWebUserObject *pWebUser)
{
	if (pWebUser == NULL)
		return SYS_ERROR_SENDFAIL;

	char sbuff[512];
	int nError;

	sprintf(sbuff, "<SCRIPTMSG><COMMAND>TALKBEG</COMMAND><ADMINID>%lu</ADMINID><CLIENTID>%s</CLIENTID><SERVICEUIN>%lu</SERVICEUIN><NICKNAME>%s</NICKNAME></SCRIPTMSG>\r\n",
		pWebUser->floatadminuid, pWebUser->info.sid, m_manager->m_userInfo.UserInfo.uid, m_manager->m_userInfo.UserInfo.nickname);

	nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	return nError;
}

int CChatVisitor::SendWebuserTalkEnd(CWebUserObject *pWebUser)
{
	if (pWebUser == NULL)
		return SYS_ERROR_SENDFAIL;

	char sbuff[512];
	int nError;

	sprintf(sbuff, "<SCRIPTMSG><COMMAND>TALKEND</COMMAND><ADMINID>%lu</ADMINID><CLIENTID>%s</CLIENTID><SERVICEUIN>%lu</SERVICEUIN><NICKNAME>%s</NICKNAME></SCRIPTMSG>\r\n",
		pWebUser->floatadminuid, pWebUser->info.sid, m_manager->m_userInfo.UserInfo.uid, m_manager->m_userInfo.UserInfo.nickname);

	nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	return nError;
}

bool CChatVisitor::ConnectToVisitorServer()
{
	m_socketEx.m_MsgRecvSuccID = WM_SOCKETEX_RECVSUCC;
	m_socketEx.m_MsgRecvFailID = WM_SOCKETEX_RECVFAIL;
	m_socketEx.m_MsgSocketCloseID = WM_SOCKETEX_CLOSE;
	m_socketEx.m_bIM = false;

	m_socketEx.Close();

	for (int i = 0; i < 2; i++)
	{
		if (m_socketEx.Connect(m_manager->m_sysConfig->m_sVisitorServer.c_str(), 
			m_manager->m_sysConfig->m_nVisitorServerPort))
			return true;
	}

	return false;
}

int CChatVisitor::LoginToVisitorServer()
{
	int nError = SYS_FAIL;
	if (m_manager->m_userInfo.UserInfo.uid == 0)
	{
		return nError;
	}

	char sbuff[512];
	string pass = GetMd5Str(m_manager->m_userInfo.UserInfo.pass);
	sprintf(sbuff, "<SYSTEM><COMMAND>UP</COMMAND><USERUIN>%d</USERUIN><POSW>%s</POSW><VERSION>%d</VERSION><AUTOUP>YES</AUTOUP><UTF8>1</UTF8><ZLIB>%d</ZLIB></SYSTEM>\r\n",
		m_manager->m_userInfo.UserInfo.uid, pass.c_str(), VISITOR_VERSION, (m_manager->m_initConfig.bZlibEnabled ? 1 : 0));

	nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	return nError;
}

void CChatVisitor::SolveVisitorSystem(char *pInitBuff)
{
	char cmd[512];
	string strTemp;

	strTemp = GetXMLCommandString(pInitBuff, cmd, "COMMAND");

	if ((int)strTemp.find("MYADMIN") == 0)
	{
		SolveVisitorSystemAdmin(pInitBuff);
	}
	else if ((int)strTemp.find("UP") == 0)
	{
		SolveVisitorSystemUp(pInitBuff);
	}
	else if ((int)strTemp.find("DOWN") == 0)
	{
		SolveVisitorSystemDown(pInitBuff);
	}
	else if ((int)strTemp.find("SYSTEMSET") == 0)
	{
		
	}
	else if ((int)strTemp.find("STOPRECVMSG") == 0)
	{
		SolveVisitorSystemStopRecvMsg(pInitBuff);
	}
	else if ((int)strTemp.find("ALREADYAPPLY") == 0)
	{
		SolveVisitorSystemAlreadyApply(pInitBuff);
	}
}

void CChatVisitor::SolveVisitorSCRIPTMSG(char *pInitBuff)
{
	char cmd[100];
	string strTemp;

	strTemp = GetXMLCommandString(pInitBuff, cmd, "COMMAND", 100);

	if ((int)strTemp.find("APPLYFAIL_DISAGREE") == 0)
	{
		SolveVisitorSCRIPTMSGApplyFail(pInitBuff);
	}
	else if ((int)strTemp.find("MODINAME") == 0)
	{
		//更改访客名称
		SolveVisitorSCRIPTMSGModiName(pInitBuff);
	}
	else if ((int)strTemp.find("TALKBEG") == 0)
	{
		//开始会话
		SolveVisitorSCRIPTMSGTalkBegin(pInitBuff);
	}
	else if ((int)strTemp.find("TALKEND") == 0)
	{
		//结束会话
		SolveVisitorSCRIPTMSGTalkEnd(pInitBuff);
	}
}

void CChatVisitor::SolveVisitorSCRIPTMSGApplyFail(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;
	CUserObject *pTalkUser = NULL;

	char sid[120], scriptflag[120], invitename[100];
	unsigned long talkuid;

	GetXMLCommandString(pInitBuff, sid, "CLIENTID");
	GetXMLCommandString(pInitBuff, invitename, "NICKNAME");
	GetXMLCommandString(pInitBuff, scriptflag, "SCRIPTFLAG");
	talkuid = GetXMLCommandInt(pInitBuff, "SERVICEUIN");

	pTalkUser = m_manager->GetUserObjectByUid(talkuid);
	pWebUser = m_manager->GetWebUserObjectBySid(sid);

	if (pWebUser == NULL || pWebUser->cTalkedSatus == INTALKING)
	{
		return;
	}

	if (pWebUser->onlineinfo.talkstatus == TALKSTATUS_INVITE)
		pWebUser->m_refuseinvite = true;

	pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
	pWebUser->m_nWaitTimer = -20;


	//访客退回到在线列表
	char msg[MAX_256_LEN];
	GetInviteSysMsg(msg, pWebUser, invitename, RESPINVITE_FAIL);

	return;
}

void CChatVisitor::SolveVisitorSCRIPTMSGModiName(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;
	CUserObject *pTalkUser = NULL;
	char sid[120] = { 0 };
	GetXMLCommandString(pInitBuff, sid, "CLIENTID");


	pWebUser = m_manager->GetWebUserObjectBySid(sid);
	if (pWebUser == NULL)
	{
		//有之前上线的访客， 这里是不是要显示?
		return;
	}

	char newname[MAX_USERNAME_LEN + 1] = { 0 }, szKefuNmae[MAX_USERNAME_LEN + 1] = { 0 };
	unsigned int uKefu = GetXMLCommandInt(pInitBuff, "SERVICEUIN");
	GetXMLCommandString(pInitBuff, szKefuNmae, "SERVICENAME");
	GetXMLCommandString(pInitBuff, newname, "NICKNAME");

	if (!szKefuNmae[0])
	{
		CUserObject *pkefu = m_manager->GetUserObjectByUid(uKefu);
		if (pkefu != NULL)
		{
			strcpy(szKefuNmae, pkefu->UserInfo.nickname);
		}
	}

	char msg[MAX_256_LEN];
	sprintf(msg, "%s 客服 %s(%u) 将访客名称改为 %s", GetTimeStringMDAndHMS(0), szKefuNmae, uKefu, newname);

	strncpy(pWebUser->info.name, newname, MAX_USERNAME_LEN);
	if (strlen(pWebUser->info.name) < 2)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "name length is null：SolveVisitorSCRIPTMSGModiName name length：%d", strlen(pWebUser->info.name));
	}
	pWebUser->info.nameflag = 1;
}

void CChatVisitor::SolveVisitorSCRIPTMSGTalkBegin(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;
	CUserObject *pTalkUser = NULL;
	char sid[120] = { 0 };
	GetXMLCommandString(pInitBuff, sid, "CLIENTID");

	pWebUser = m_manager->GetWebUserObjectBySid(sid);
	if (pWebUser == NULL || pWebUser->cTalkedSatus == INTALKING)
	{
		//有之前上线的访客， 这里是不是要显示?
		return;
	}

	char szKefuNmae[MAX_USERNAME_LEN + 1] = { 0 };
	unsigned int uKefu = GetXMLCommandInt(pInitBuff, "SERVICEUIN");
	GetXMLCommandString(pInitBuff, szKefuNmae, "NICKNAME");

	CUserObject *pkefu = m_manager->GetUserObjectByUid(uKefu);
	if (!szKefuNmae[0] && pkefu != NULL)
	{
		strcpy(szKefuNmae, pkefu->UserInfo.nickname);
	}

	char msg[MAX_256_LEN];
	sprintf(msg, "%s 客服 %s(%u) 已与访客建立通话", GetTimeStringMDAndHMS(0), szKefuNmae, uKefu);

	//重绘
	if (pkefu != NULL&&pkefu->m_bFriend)
	{
		pWebUser->cTalkedSatus = INTALKING;
		pWebUser->info.status = TALKSTATUS_TALK;
		pWebUser->talkuid = uKefu;
	}
	else
	{
		pWebUser->cTalkedSatus = HASTALKED;
		pWebUser->info.status = TALKSTATUS_NO;
		pWebUser->talkuid = 0;
	}
}

void CChatVisitor::SolveVisitorSCRIPTMSGTalkEnd(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;
	CUserObject *pTalkUser = NULL;
	char sid[120] = { 0 };
	GetXMLCommandString(pInitBuff, sid, "CLIENTID");

	pWebUser = m_manager->GetWebUserObjectBySid(sid);
	if (pWebUser == NULL)
	{
		//有之前上线的访客， 这里是不是要显示?
		return;
	}

	char szKefuNmae[MAX_USERNAME_LEN + 1] = { 0 };
	unsigned int uKefu = GetXMLCommandInt(pInitBuff, "SERVICEUIN");
	GetXMLCommandString(pInitBuff, szKefuNmae, "NICKNAME");

	CUserObject *pkefu = m_manager->GetUserObjectByUid(uKefu);
	if (!szKefuNmae[0] && pkefu != NULL)
	{
		strcpy(szKefuNmae, pkefu->UserInfo.nickname);
	}

	char msg[MAX_256_LEN];
	sprintf(msg, "%s 客服 %s(%u) 与访客的通话已结束", GetTimeStringMDAndHMS(0), szKefuNmae, uKefu);

	pWebUser->cTalkedSatus = HASTALKED;
	pWebUser->info.status = TALKSTATUS_NO;
	pWebUser->talkuid = 0;
}

void CChatVisitor::SolveVisitorSystemAdmin(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;

	char cmdvalue[512];

	GetXMLCommandString(pInitBuff, cmdvalue, "IP");

	if (strlen(cmdvalue) > 0)
	{
		m_manager->m_sysConfig->m_sVisitorServer = cmdvalue;

		GetXMLCommandString(pInitBuff, cmdvalue, "PORT");
		m_manager->m_sysConfig->m_nVisitorServerPort = atol(cmdvalue);

		ConnectAndLoginToVisitorServer();
	}
	else
	{
		m_manager->m_nOnLineStatusEx = STATUS_ONLINE;

		SendStartRecvMsgToVisitorServer();
	}
}

void CChatVisitor::SolveVisitorSystemUp(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;
	WEBUSER_UPINFO webuser_upinfo;

	memset(&webuser_upinfo, '\0', sizeof(WEBUSER_UPINFO));
	GetXMLCommandString(pInitBuff, webuser_upinfo.sid, "CLIENTID");
	GetXMLCommandString(pInitBuff, webuser_upinfo.source, "SOURCE");

	GetXMLCommandString(pInitBuff, webuser_upinfo.iecopyright, "IECOPYRIGHT", 50);
	GetXMLCommandString(pInitBuff, webuser_upinfo.systeminfo, "SYSTEMINFO", 50);
	GetXMLCommandString(pInitBuff, webuser_upinfo.language, "LANGUAGE", 50);
	webuser_upinfo.visioncolor = GetXMLCommandInt(pInitBuff, "VISIONCOLOR");

	GetXMLCommandString(pInitBuff, webuser_upinfo.visionsize, "VISIONSIZE", 20);
	webuser_upinfo.adminid = GetXMLCommandInt(pInitBuff, "ADMINID");
	GetXMLCommandString(pInitBuff, webuser_upinfo.visiturl, "VISIT", MAX_URL_LEN);
	GetXMLCommandString(pInitBuff, webuser_upinfo.source, "SOURCE", 50);
	GetXMLCommandString(pInitBuff, webuser_upinfo.scriptflag, "SCRIPTFLAG", 50);
	GetXMLCommandString(pInitBuff, webuser_upinfo.sip, "IP", 20);
	webuser_upinfo.port = GetXMLCommandInt(pInitBuff, "PORT");

	GetXMLCommandString(pInitBuff, webuser_upinfo.lastvisit, "LASTVISIT", MAX_URL_LEN);
	GetXMLCommandString(pInitBuff, webuser_upinfo.webtitle, "WEBTITLE", 50);
	GetXMLCommandString(pInitBuff, webuser_upinfo.nickname, "NICKNAME", 20);

	int nameflag = 1;
	if (strlen(webuser_upinfo.nickname) <= 0)
	{
		strcpy(webuser_upinfo.nickname, webuser_upinfo.source);
		nameflag = 0;
	}

	webuser_upinfo.ctimes = GetXMLCommandInt(pInitBuff, "CTIMES");
	webuser_upinfo.ttimes = GetXMLCommandInt(pInitBuff, "TTIMES");
	GetXMLCommandString(pInitBuff, webuser_upinfo.lastvtime, "LASTVTIME", MAX_URL_LEN);
	GetXMLCommandString(pInitBuff, webuser_upinfo.lastttime, "LASTTTIME", MAX_URL_LEN);

	webuser_upinfo.lastuin = GetXMLCommandInt(pInitBuff, "LASTTUIN");
	webuser_upinfo.isautoinvit = GetXMLCommandInt(pInitBuff, "ISAUTOINVIT");

	pWebUser = m_manager->GetWebUserObjectBySid(webuser_upinfo.sid);
	if (pWebUser == NULL)
	{
		pWebUser = m_manager->AddWebUserObject(webuser_upinfo.sid, "", webuser_upinfo.nickname, webuser_upinfo.scriptflag, webuser_upinfo.visiturl, STATUS_ONLINE, 0);
		g_VisitLog.WriteLog(C_LOG_TRACE, "SolveVisitorSystemUp sid=%s,nickname=%s, scriptflag=%s, visiturl=%s", 
			webuser_upinfo.sid, webuser_upinfo.nickname, webuser_upinfo.scriptflag, webuser_upinfo.visiturl);
		pWebUser->info.nameflag = nameflag;
		pWebUser->m_onlinetime = ::GetCurrentLongTime();
		pWebUser->m_bIsShow = false;
	}

	if (nameflag)
	{
		//更新访客名称
		strcpy(pWebUser->info.name, webuser_upinfo.nickname);
		pWebUser->info.nameflag = 1;
		if (strlen(pWebUser->info.name) < 2)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "name length is null：SolveVisitorSystemUp name length：%d", strlen(pWebUser->info.name));
		}
	}
	strcpy(pWebUser->info.ipfromname, webuser_upinfo.source);

	strcpy(pWebUser->info.url, webuser_upinfo.visiturl);
	strcpy(pWebUser->info.title, webuser_upinfo.webtitle);

	strcpy(pWebUser->info.sip, webuser_upinfo.sip);
	pWebUser->AddScriptFlag(webuser_upinfo.scriptflag, webuser_upinfo.visiturl);
	pWebUser->info.status = STATUS_ONLINE;
	pWebUser->floatadminuid = webuser_upinfo.adminid;

	pWebUser->nVisitNum = webuser_upinfo.ctimes;
	pWebUser->nTalkNum = webuser_upinfo.ttimes;

	SolvePrevURL(pWebUser, &webuser_upinfo);

	if (pWebUser->cTalkedSatus != INTALKING)
	{
		if (webuser_upinfo.isautoinvit != 0)
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_AUTOINVITE;
		else
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
	}
	
	char msg[MAX_256_LEN];
	GetWebUserSysMsg(msg, pWebUser, &webuser_upinfo, webuser_upinfo.scriptflag);
	RecvSrvRespVisitorInfo(pWebUser, &webuser_upinfo);

	SolveWebUserTipsTail(pWebUser, &webuser_upinfo);
	SolveWebUserOnlineTipsTail(pWebUser, &webuser_upinfo);

	if (pWebUser->IsDisplay(m_manager->m_sysConfig, m_manager->m_userInfo.UserInfo.uid))
	{
		string str;

		switch (pWebUser->onlineinfo.talkstatus)
		{
		case TALKSTATUS_NO:
		case TALKSTATUS_AUTOINVITE:
		default:
			str = pWebUser->info.name;
			str += " 用户访问网页！";
			break;
		}
	}

	return;
}

void CChatVisitor::SetVisitorOffline(CWebUserObject *pWebUser)
{
	if (pWebUser == NULL)
		return;
	
	char msg[MAX_256_LEN];
	if (!pWebUser->IsOnline())
	{
		sprintf(msg, "访客离开了");
	}
	
	if (pWebUser->IsDisplay(m_manager->m_sysConfig, m_manager->m_userInfo.UserInfo.uid))
	{
		pWebUser->m_bConnected = FALSE;
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
		pWebUser->info.status = STATUS_OFFLINE;
	}
	
	pWebUser->m_bIsShow = true;
}

void CChatVisitor::SolveVisitorSystemDown(char *pInitBuff)
{
	CWebUserObject *pWebUser = NULL;
	
	char sid[120], scriptflag[50];
	WEBUSER_URL_INFO *pUrlInfo;
	
	GetXMLCommandString(pInitBuff, sid, "CLIENTID");
	GetXMLCommandString(pInitBuff, scriptflag, "SCRIPTFLAG");
	
	pWebUser = m_manager->GetWebUserObjectBySid(sid);
	if (pWebUser == NULL)
	{
		pWebUser = m_manager->GetWebUserObjectByScriptFlag(scriptflag);
	}
	
	if (pWebUser == NULL)
		goto RETURN;
	
	pUrlInfo = pWebUser->GetScriptFlagOb(scriptflag);
	if (pUrlInfo != NULL)
	{
		char msg[MAX_256_LEN];
		if (!pWebUser->IsOnline())
		{
			sprintf(msg, "访客离开了");
		}
		pWebUser->DeleteScriptFlag(scriptflag);
	}
	
	if (!pWebUser->IsOnline())
	{
		SetVisitorOffline(pWebUser);
	}
	else if (pWebUser->m_mapUrlAndScriptFlagOb.size() == 0)
	{
		g_VisitLog.WriteLog(C_LOG_TRACE, "访客离开，但会话在线 %d", pWebUser->onlineinfo.talkstatus);
	}

RETURN:
	return;
}

void CChatVisitor::SolveVisitorSystemStopRecvMsg(char *pInitBuff)
{
	if (m_manager->m_nOnLineStatusEx != STATUS_OFFLINE)
	{
		m_manager->m_nOnLineStatusEx = STATUS_OFFLINE;
		m_manager->m_nLoginToVisitor = 0;
	}
}

void CChatVisitor::SolveVisitorSystemAlreadyApply(char *pInitBuff)
{
	char sid[MAX_SID_LEN];
	char stype[50];
	char msg[MAX_256_LEN];
	
	unsigned long uid;
	CWebUserObject *pWebUser = NULL;
	CUserObject *pInviteUser = NULL;
	int type;
	
	uid = GetXMLCommandInt(pInitBuff, "USERUIN");
	GetXMLCommandString(pInitBuff, sid, "CLIENTID", MAX_SID_LEN);
	GetXMLCommandString(pInitBuff, stype, "TYPE", 50);
	
	type = GetApplyTypeID(stype);
	
	pInviteUser = m_manager->GetUserObjectByUid(uid);
	pWebUser = m_manager->GetWebUserObjectBySid(sid);
	if (pWebUser == NULL)
	{
		return;
	}
	
	if (pWebUser->cTalkedSatus == INTALKING)
	{
		return;
	}
	
	if (uid != m_manager->m_userInfo.UserInfo.uid && (pInviteUser == NULL || !pInviteUser->m_bFriend))
	{
		//被别人接走了就删除
		pWebUser->cTalkedSatus = HASTALKED;
		return;
	}
	
	switch (type)
	{
	case APPLY_ASK:
	
		//表示客服的请求包，服务器处理了，将访客移到邀请中
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_INVITE;
	
		if (uid == m_manager->m_userInfo.UserInfo.uid)
		{
			pWebUser->m_nWaitTimer = 0;
		}
	
		//这里必须先在htmleditor中显示，然后再移动位置，因为移动位置可能会导致区域的切换，其他地方同样考虑
		m_manager->GetInviteChatSysMsg(msg, pInviteUser, pWebUser, APPLY_ASK);
		break;
	case APPLY_OPEN:
		break;
	case MYGETNOTE:
		break;
	}

	return;
}

void CChatVisitor::GetInviteSysMsg(char* msg, CWebUserObject *pWebUser, char *nickname, int result)
{
	if (pWebUser == NULL || nickname == NULL)
		return;

	switch (result)
	{
	case RESPINVITE_SUCC:
		sprintf(msg, "%s接受了%s邀请的对话", pWebUser->info.name, nickname);
		break;
	case RESPINVITE_FAIL:
		sprintf(msg, "%s拒绝了%s邀请的对话", pWebUser->info.name, nickname);
		break;
	case RESPINVITE_TIMEOUT:
		sprintf(msg, "超时%s没有响应%s邀请的对话", pWebUser->info.name, nickname);
		break;
	default:
		sprintf(msg, "邀请协助出现异常，请程序员来看一下1");
		break;
	}
}

int CChatVisitor::SendStartRecvMsgToVisitorServer()
{
	char sbuff[100] = { 0 };
	int nError;

	sprintf(sbuff, "<SYSTEM><COMMAND>STARTRECVMSG</COMMAND></SYSTEM>\r\n");

	nError = SendBuffToVisitorServer(sbuff, strlen(sbuff));
	return nError;
}

void CChatVisitor::GetWebUserSysMsg(char* msg, CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo, const string& strscriptflag)
{

}

void CChatVisitor::RecvSrvRespVisitorInfo(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo)
{
	if (pWebUser == NULL)
		return;

	pWebUser->m_strInfoHtml = SolveVisitorInfoHtmlTxt(pWebUser, pInfo);
	pWebUser->m_bIsGetInfo = true;

	//pWebUser = m_pCurSelectWebUser;
	if (pWebUser == NULL)
		return;

	if (strcmp(pWebUser->info.sid, pInfo->sid) != 0)
		return;
}

string CChatVisitor::SolveVisitorInfoHtmlTxt(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo)
{
	if (pWebUser == NULL || pInfo == NULL)
		return "";

	string str;
	char str1[MAX_1024_LEN];
	CUserObject *pUser;

	str = "<html><head>";
	str += STRING_HTML_META;
	str += STRING_HTML_BASE;
	str += STRING_HTML_STYLE;
	str += "</head><body>";

	sprintf(str1, "<table width=100% border=0  cellspacing=0>");
	str += str1;

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>此客人的永久身份：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pWebUser->info.sid);
	str += str1;
	str += "</td>";
	str += "</tr>";
	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	pUser = m_manager->GetUserObjectByUid(pInfo->lastuin);
	if (pInfo->lastuin == 0)
		sprintf(str1, "<LI><SPAN class=clientnamefont1>上次接待此客人的客服人员：</SPAN><SPAN class=clientfont1>没有</SPAN></LI>");
	else if (pUser != NULL)
		sprintf(str1, "<LI><SPAN class=clientnamefont1>上次接待此客人的客服人员：</SPAN><SPAN class=clientfont1>%s(%d)</SPAN></LI>", pUser->UserInfo.nickname, pInfo->lastuin);
	else
		sprintf(str1, "<LI><SPAN class=clientnamefont1>上次接待此客人的客服人员：</SPAN><SPAN class=clientfont1>(%d)</SPAN></LI>", pInfo->lastuin);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>客人名称：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pWebUser->info.name);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>IP：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->sip);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>IP定位：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->source);
	str += str1;
	str += "</td>";
	str += "</tr>";

	if (strlen(pWebUser->prevurlvar) > 0)
	{
		str += "<tr>";
		str += "<td colspan=2 width=100% >";
		sprintf(str1, "<LI><SPAN class=clientnamefont1>访问者来源搜索引擎：</SPAN><SPAN class=clientfont1><A title=%s href=\"%s\">%s</A></SPAN></LI>",
			pWebUser->prevurl, pWebUser->prevurl, pWebUser->prevurlhost);
		str += str1;
		str += "</td>";
		str += "</tr>";

		str += "<tr>";
		str += "<td colspan=2 width=100% >";
		sprintf(str1, "<LI><SPAN class=clientnamefont1>搜索引擎关键词：</SPAN><SPAN class=clientfont1><A title=%s href=\"%s\">%s</A></SPAN></LI>",
			pWebUser->prevurl, pWebUser->prevurl, pWebUser->prevurlvar);
		str += str1;
		str += "</td>";
		str += "</tr>";
	}

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>访问次数：</SPAN><SPAN class=clientfont1>%d</SPAN></LI>", pWebUser->nVisitNum);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>对话次数：</SPAN><SPAN class=clientfont1>%d</SPAN></LI>", pWebUser->nTalkNum);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "</td>";
	str += "<td width=50%>";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>上次访问时间：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->lastvtime);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>当前网页：</SPAN><SPAN class=clientfont1><A title=%s href=\"%s\">%s</A></SPAN></LI>",
		pInfo->webtitle, pInfo->visiturl, pInfo->visiturl);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>分辨率大小：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->visionsize);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>显示器颜色深度：</SPAN><SPAN class=clientfont1>%d真彩色</SPAN></LI>", pInfo->visioncolor);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>语言设置：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->language);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>操作系统：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->systeminfo);
	str += str1;
	str += "</td>";
	str += "</tr>";

	str += "<tr>";
	str += "<td colspan=2 width=100% >";
	sprintf(str1, "<LI><SPAN class=clientnamefont1>浏览器版本：</SPAN><SPAN class=clientfont1>%s</SPAN></LI>", pInfo->iecopyright);
	str += str1;
	str += "</td>";
	str += "</tr>";

	sprintf(str1, "</table>");
	str += "</body></html>";

	return str;
}

void CChatVisitor::SolvePrevURL(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo)
{
	if (pWebUser == NULL || pInfo == NULL)
		return;

	string strURL, strHost, strVar;

	strURL = pInfo->lastvisit;

	strncpy(pWebUser->prevurl, pInfo->lastvisit, MAX_URL_LEN);
	if (ParseSearchURLHostAndVar(strURL, strHost, strVar))
	{
		strncpy(pWebUser->prevurlhost, strHost.c_str(), MAX_URL_LEN);
		strncpy(pWebUser->prevurlvar, strVar.c_str(), MAX_URL_LEN);

		if (strVar.empty())
		{
			pWebUser->nVisitFrom = WEBUSERICON_FRDURL;
		}
		else
		{
			pWebUser->nVisitFrom = WEBUSERICON_SEARCHURL;
		}
	}
	else if (strstr(pWebUser->prevurl, "fromad") != NULL)
	{
		pWebUser->nVisitFrom = WEBUSERICON_UNIONURL;
	}
	else if (strlen(pWebUser->prevurl) > 0)
	{
		pWebUser->nVisitFrom = WEBUSERICON_OTHERURL;
	}
	else
	{
		pWebUser->nVisitFrom = WEBUSERICON_DIRECTURL;
	}

	return;
}

void CChatVisitor::SolveWebUserTipsTail(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo)
{
	if (pWebUser == NULL || pInfo == NULL)
		return;

	char str[MAX_256_LEN];
	
	sprintf(str, "IP:%s", pInfo->sip);
	sprintf(str, "\r\nIP定位:%s", pInfo->nickname);
	sprintf(str, "\r\n语言设置:%s", pInfo->language);
	pWebUser->m_strInfo = str;

	return;
}

void CChatVisitor::SolveWebUserOnlineTipsTail(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo)
{
	if (pWebUser == NULL || pInfo == NULL)
		return;

	char strTail[MAX_1024_LEN] = {0};

	string stime = GetExtDateTimeFormatTime(0);

	switch (pWebUser->nVisitFrom)
	{
	case WEBUSERICON_SEARCHURL:
	case WEBUSERICON_SEARCHURLMID:
	case WEBUSERICON_SEARCHURLTOP:
		sprintf(strTail, "%s 从[搜索引擎,搜索关键词:%s]:%s进入%s", stime.c_str(), pWebUser->prevurlvar, pWebUser->prevurlhost, pWebUser->info.url);
		break;
	case WEBUSERICON_FRDURL:
		sprintf(strTail, "%s 从友情连接:%s进入%s", stime.c_str(), pWebUser->prevurlhost, pWebUser->info.url);
		break;
	case WEBUSERICON_UNIONURL:
		sprintf(strTail, "%s 从联盟连接:%s进入%s", stime.c_str(), pWebUser->prevurlhost, pWebUser->info.url);
		break;
	case WEBUSERICON_OTHERURL:
		sprintf(strTail, "%s 从<A title=%s href=\"%s\">%s</A>进入<A title=%s href=\"%s\">%s</A></SPAN></LI>",
			stime.c_str(),
			pWebUser->prevurl, pWebUser->prevurl, pWebUser->prevurl,
			pWebUser->info.title, pWebUser->info.url, pWebUser->info.title);
		break;
	case WEBUSERICON_DIRECTURL:
		sprintf(strTail, "%s 进入%s", stime.c_str(), pWebUser->info.url);
		break;
	}

	if (!pWebUser->m_strTail.empty())
		pWebUser->m_strTail += "\r\n";
	pWebUser->m_strTail += strTail;
}

bool CChatVisitor::ConnectAndLoginToVisitorServer()
{
	if (ConnectToVisitorServer())
	{
		return LoginToVisitorServer();
	}
	else
		return false;
}



