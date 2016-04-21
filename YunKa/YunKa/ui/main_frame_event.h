#pragma once
#include "small_menu.h"


class CMainFrameEvent 
{
public:
	CMainFrameEvent(CPaintManagerUI &pm);
	~CMainFrameEvent();

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg);

	void OnTimer(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

	void OnClick(TNotifyUI& msg);


	void InitWindow(HWND hwnd);

protected:

	CSmallMenu m_frameSmallMenu;

	CPaintManagerUI &m_mainFrameEvpm;


};

