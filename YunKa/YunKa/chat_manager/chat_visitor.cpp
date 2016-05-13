#include "../stdafx.h"
#include "chat_visitor.h"
#include "chat_manager.h"
#include "../chat_common/comdef.h"

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
	if (m_manager->m_bExit)
	{
		return;
	}

	static char visitlogmame[100] = { 0 };
	if (!visitlogmame[0])
	{
		sprintf(visitlogmame, "_%s(%lu)_visit.log", m_manager->m_userInfo.UserInfo.sid, m_manager->m_userInfo.UserInfo.uid);
		g_VisitLog.InitLog(GetCurrentPath() + "log", visitlogmame, C_LOG_ALL);
	}

	//time(&m_tResentVisitPackTime);


	//int len = wParam;
	//char *pBuff = (char *)lParam;

	//g_VisitLog.WriteLog(C_LOG_TRACE, "recv:%s", pBuff);


	/*if (IsXMLCommandStringExist(pBuff, "SYSTEM"))
		SolveVisitorSystem(pBuff);
		else if (IsXMLCommandStringExist(pBuff, "MSG"))
		SolveVisitorMsg(pBuff);
		else if (IsXMLCommandStringExist(pBuff, "TEST"))
		SolveVisitorTest(pBuff);
		else if (IsXMLCommandStringExist(pBuff, "SCRIPTMSG"))
		SolveVisitorSCRIPTMSG(pBuff);
		else if (IsXMLCommandStringExist(pBuff, "QUEUEMSG"))
		SolveVisitorQUEUEMSG(pBuff);
		else if (IsXMLCommandStringExist(pBuff, "PING"))
		SolveVisitorPing(pBuff);*/

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

