#ifndef _MSG_REQUEST_H_
#define _MSG_REQUEST_H_

#include "chat_manager.h"
#include "message_handler.h"




//typedef   int(*GetLoginState)(int);




class CMessageRequest : public CChatManager
{
public:
	CMessageRequest(IBaseMsgs* iBaseMsgs);
	~CMessageRequest();

	void StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd);


private:

	CMessageHandler m_baseMsg;


	CChatManager* m_manager;

	

};







#endif