#include "stdafx.h"
#include "chat_manager.h"


CChatManager::CChatManager()
{
	m_isLoginSuccess = false;
	m_login = new CLogin();
	m_login->m_manager = this;
	m_chatObjects = NULL;
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

CLogin* CChatManager::GetLogin()
{
	return m_login;
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



