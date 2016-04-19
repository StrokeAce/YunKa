// IM_Service.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "login.h"
#include "chat_manager.h"


int _tmain(int argc, _TCHAR* argv[])
{
	IBaseMsgs* baseMsg = NULL;
	CChatManager* manager = CChatManager::GetInstance(baseMsg);

	bool isAutoLogin = false;
	string loginName = "9692111";
	string password = "123";
	bool isKeepPwd = false;

	string error;

	manager->StartLogin(loginName, password, isAutoLogin, isKeepPwd);
	system("pause");
	return 0;
}

