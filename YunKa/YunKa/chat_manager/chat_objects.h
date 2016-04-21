#pragma once

#include "login.h"

class CChatObjects :public IBaseReceive
{
public:
	CChatObjects();
	~CChatObjects();

	friend class CMySocket;

	// 接收该用户对话

	// 拒绝此对话

	// 更改该用户名称
private:
	virtual void OnReceive(void* pHead, void* pData);

public:
	CLogin* m_login;
};

