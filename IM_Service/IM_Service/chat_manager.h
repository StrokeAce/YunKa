#pragma once

#include "comm_struct.h"
#include "sys_config.h"
#include "chat_objects.h"
#include "msgs.h"
#include "login.h"

class CChatManager
{
public:
	CChatManager();
	~CChatManager();

	static CChatManager* GetInstance()
	{
		static CChatManager instance;
		return &instance;
	}

public:

	CLogin* GetLogin();

	CSysConfig* GetSysConfig();			// 获取系统配置对象

	CChatObjects* GetChatObjects();		// 获取聊天对象管理类实例

	CMsgs* GetMsgs();					// 获取聊天消息管理类实例

	void ScreenCapture();				// 截图

private:
	CLogin* m_login;
	CChatObjects* m_chatObjects;
	bool m_isLoginSuccess;
};

