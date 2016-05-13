#ifndef _MSG_TRANSFER_H_
#define _MSG_TRANSFER_H_

#include "chat_manager.h"




//typedef   int(*GetLoginState)(int);




class CMessageHandler : public IBaseMsgs
{
public:
	CMessageHandler();
	~CMessageHandler();


	virtual void LoginProgress(int percent);


	// 收到一个坐席用户的信息,用来初始化坐席列表
	virtual void RecvOneUserInfo(CUserObject* obj);

	// 收到一个新建的会话消息
	virtual void RecvCreateChat(CWebUserObject* obj);

	// 收到一个会话消息
	virtual void RecvChatInfo(CWebUserObject* obj);

	// 收到更新用户的在线状态
	virtual void RecvUserStatus(CUserObject* obj);

	virtual string GetLastError(){ return ""; }

	// 收到一条消息
	virtual void RecvOneMsg(IBaseObject* pObj, int msgFrom, string msgId, int msgType, int msgDataType,
		string msgContent, string msgTime, CUserObject* pAssistUser, WxMsgBase* msgContentWx, string msgExt);

	// 坐席上线消息
	virtual void RecvOnline(CUserObject* obj);

	// 坐席下线消息
	virtual void RecvOffline(CUserObject* obj);

	// 会话关闭
	virtual void RecvCloseChat();

	void StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd);

	int GetLoginPercent(int vel)
	{
		return m_loginPercent;
	}
	int SetLoginPercent(int vel)
	{
		return m_loginPercent;
	}



private:

	int m_loginPercent;
	CChatManager* m_manager;

	


};











#endif