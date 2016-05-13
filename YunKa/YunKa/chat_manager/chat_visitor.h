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
	//发送开始通话到访客接待服务器
	int SendWebuserTalkBegin(CWebUserObject *pWebUser);

	//发送结束通话包到访客接待服务器
	int SendWebuserTalkEnd(CWebUserObject *pWebUser);

	int SendPingToVisitorServer();

	int SendBuffToVisitorServer(char *sbuff, int len);

private:
	/***************     继承接口的函数实现    *****************/

	virtual void OnReceive(void* pHead, void* pData);

public:
	CChatManager*	m_manager;
	CMySocket		m_socketEx;				// 访客接待连接
	time_t			m_tResentVisitPackTime;	//

private:
	friend class CMySocket;

private:
	
};

#endif