#include "../StdAfx.h"
#include "message_request.h"




CMessageRequest::CMessageRequest(IBaseMsgs* iBaseMsgs)
{


	m_manager = CChatManager::GetInstance(iBaseMsgs);

}


CMessageRequest::~CMessageRequest()
{
}


void CMessageRequest::StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{

	//bool isAutoLogin = false;
	//string loginName = "9692111";
	//string password = "123";
	//bool isKeepPwd = false;

	//string error;

	m_manager->StartLogin(loginName, password, isAutoLogin, isKeepPwd);

}