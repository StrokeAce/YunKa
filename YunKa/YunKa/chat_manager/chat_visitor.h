#ifndef _CHAT_VISITOR_H_
#define _CHAT_VISITOR_H_

#include "socket.h"

class CChatManager;

class CChatVisitor : public IBaseReceive
{
public:
	CChatVisitor();
	~CChatVisitor();

public:
	// 发送开始通话到访客接待服务器
	int SendWebuserTalkBegin(CWebUserObject *pWebUser);

	// 发送结束通话包到访客接待服务器
	int SendWebuserTalkEnd(CWebUserObject *pWebUser);

	int SendPingToVisitorServer();

	int SendBuffToVisitorServer(char *sbuff, int len);

	int SendStartRecvMsgToVisitorServer();

	bool ConnectAndLoginToVisitorServer();

	bool ConnectToVisitorServer();

	int LoginToVisitorServer();

	void SolveVisitorSystem(char *pInitBuff);

	void SolveVisitorSCRIPTMSG(char *pInitBuff);

	//访客不同意对话
	void SolveVisitorSCRIPTMSGApplyFail(char *pInitBuff);

	//更改访客名称
	void SolveVisitorSCRIPTMSGModiName(char *pInitBuff);

	//开始通话
	void SolveVisitorSCRIPTMSGTalkBegin(char *pInitBuff);

	//结束通话
	void SolveVisitorSCRIPTMSGTalkEnd(char *pInitBuff);

	void SolveVisitorSystemAdmin(char *pInitBuff);

	//访客上线
	void SolveVisitorSystemUp(char *pInitBuff);

	void SetVisitorOffline(CWebUserObject *pWebUser);

	//访客下线
	void SolveVisitorSystemDown(char *pInitBuff);

	void SolveVisitorSystemStopRecvMsg(char *pInitBuff);

	void SolveVisitorSystemAlreadyApply(char *pInitBuff);

	void GetInviteSysMsg(char* msg, CWebUserObject *pWebUser, char *nickname, int result);

	void GetWebUserSysMsg(char* msg, CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo, const string& strscriptflag);

	void RecvSrvRespVisitorInfo(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo);

	string SolveVisitorInfoHtmlTxt(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo);

	void SolvePrevURL(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo);

	void SolveWebUserTipsTail(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo);

	void SolveWebUserOnlineTipsTail(CWebUserObject *pWebUser, WEBUSER_UPINFO *pInfo);

private:
	/***************     继承接口的函数实现    *****************/

	virtual void OnReceive(void* wParam, void* lParam);

public:
	CChatManager*	m_manager;
	CMySocket		m_socketEx;				// 访客接待连接
	time_t			m_tResentVisitPackTime;	//
private:
	friend class CMySocket;

private:
	
};

#endif