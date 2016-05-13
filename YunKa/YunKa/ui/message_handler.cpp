#include "../StdAfx.h"
#include "message_handler.h"




CMessageHandler::CMessageHandler()
{
}

CMessageHandler::~CMessageHandler()
{
}

void CMessageHandler::LoginProgress(int percent)
{
	if (percent == 100)
	{
		m_manager->SendTo_GetShareList();
	}
	else
	{
	}

	SetLoginPercent(percent);



}



// 收到一个坐席用户的信息,用来初始化坐席列表
void CMessageHandler::RecvOneUserInfo(CUserObject* obj)
{}

// 收到一个新建的会话消息
void CMessageHandler::RecvCreateChat(CWebUserObject* obj)
{}

// 收到一个会话消息
void CMessageHandler::RecvChatInfo(CWebUserObject* obj)
{}

// 收到更新用户的在线状态
void CMessageHandler::RecvUserStatus(CUserObject* obj)
{}


// 收到一条消息
void CMessageHandler::RecvOneMsg(IBaseObject* pObj, int msgFrom, string msgId, int msgType, int msgDataType,
	string msgContent, string msgTime, CUserObject* pAssistUser, WxMsgBase* msgContentWx, string msgExt)
{

}

// 坐席上线消息
void CMessageHandler::RecvOnline(CUserObject* obj)
{}

// 坐席下线消息
void CMessageHandler::RecvOffline(CUserObject* obj)
{}

// 会话关闭
void CMessageHandler::RecvCloseChat()
{}

void CMessageHandler::StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{
	m_manager = CChatManager::GetInstance(this);

	//bool isAutoLogin = false;
	//string loginName = "9692111";
	//string password = "123";
	//bool isKeepPwd = false;

	//string error;

	m_manager->StartLogin(loginName, password, isAutoLogin, isKeepPwd);

}