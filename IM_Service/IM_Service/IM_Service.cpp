// IM_Service.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "login.h"
#include "chat_manager.h"

void OnProgress(int percent)
{
	int a = 10;
}

int _tmain(int argc, _TCHAR* argv[])
{

	CChatManager* manager = CChatManager::GetInstance();

	CLogin* login = manager->GetLogin();

	bool isAutoLogin = false;
	string loginName = "9692111";
	string password = "123";
	bool isKeepPwd = false;

	string error;

	login->StartLogin(loginName, password, isAutoLogin, isKeepPwd, OnProgress);
	system("pause");
	return 0;
}

