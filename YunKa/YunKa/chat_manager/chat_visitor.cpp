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

void CChatVisitor::OnReceive(void* pHead, void* pData)
{
	if (m_manager->m_bExit) return;

	TCP_PACK_HEADER TcpPackHead;

	static char visitlogmame[100] = { 0 };
	if (!visitlogmame[0])
	{
		sprintf(visitlogmame, "_%s(%lu)_visit.log", m_manager->m_userInfo.UserInfo.sid, m_manager->m_userInfo.UserInfo.uid);
		g_VisitLog.InitLog(GetCurrentPath() + "\\log", visitlogmame, C_LOG_ALL);
	}

	time(&m_tResentVisitPackTime);

	TcpPackHead = *((TCP_PACK_HEADER *)pHead);
	assert(TcpPackHead.len <= PACKMAXLEN);
	char *pBuff = (char *)pData;

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

int CChatVisitor::ConnectToVisitorServer()
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
	if (m_manager->m_userInfo.UserInfo.uid == 0)
	{
		return 0;
	}

	char sbuff[512];
	int nError;

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

	if (strTemp.find("MYADMIN") == 0)
	{
		SolveVisitorSystemAdmin(pInitBuff);
	}
	else if (strTemp.find("UP") == 0)
	{
		SolveVisitorSystemUp(pInitBuff);
	}
	else if (strTemp.find("DOWN") == 0)
	{
		SolveVisitorSystemDown(pInitBuff);
	}
	else if (strTemp.find("SYSTEMSET") == 0)
	{
		SolveVisitorSystemSystenset(pInitBuff);
	}
	else if (strTemp.find("STOPRECVMSG") == 0)
	{
		SolveVisitorSystemStopRecvMsg(pInitBuff);
	}
	else if (strTemp.find("ALREADYAPPLY") == 0)
	{
		SolveVisitorSystemAlreadyApply(pInitBuff);
	}
}

void CChatVisitor::SolveVisitorSCRIPTMSG(char *pInitBuff)
{
	char cmd[100];
	string strTemp;

	strTemp = GetXMLCommandString(pInitBuff, cmd, "COMMAND", 100);

	if (strTemp.find("APPLYFAIL_DISAGREE") == 0)
	{
		SolveVisitorSCRIPTMSGApplyFail(pInitBuff);
	}
	else if (strTemp.find("MODINAME") == 0)
	{
		//更改访客名称
		SolveVisitorSCRIPTMSGModiName(pInitBuff);
	}
	else if (strTemp.find("TALKBEG") == 0)
	{
		//开始会话
		SolveVisitorSCRIPTMSGTalkBegin(pInitBuff);
	}
	else if (strTemp.find("TALKEND") == 0)
	{
		//结束会话
		SolveVisitorSCRIPTMSGTalkEnd(pInitBuff);
	}
}

void CChatVisitor::SolveVisitorSCRIPTMSGApplyFail(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSCRIPTMSGModiName(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSCRIPTMSGTalkBegin(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSCRIPTMSGTalkEnd(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSystemAdmin(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSystemUp(char *pInitBuff)
{

}

void CChatVisitor::SetVisitorOffline(CWebUserObject *pWebUser)
{

}

void CChatVisitor::SolveVisitorSystemDown(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSystemSystenset(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSystemStopRecvMsg(char *pInitBuff)
{

}

void CChatVisitor::SolveVisitorSystemAlreadyApply(char *pInitBuff)
{

}



