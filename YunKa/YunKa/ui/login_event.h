#pragma once
#include "small_menu.h"



class CLoginEvent 
{
public:
	CLoginEvent(CPaintManagerUI &pm);
	~CLoginEvent();


public:

	void Init(HWND hwnd);
	int  Start(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);

	LRESULT CLoginEvent::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	


protected:

	CSmallMenu m_hLoginMenu;

	CPaintManagerUI &m_loginEvpm;



};

