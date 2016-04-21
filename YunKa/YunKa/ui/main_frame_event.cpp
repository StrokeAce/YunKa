#include "stdafx.h"
#include "main_frame_event.h"


CMainFrameEvent::CMainFrameEvent(CPaintManagerUI &pm) : m_mainFrameEvpm(pm)
{
}


CMainFrameEvent::~CMainFrameEvent()
{

}


void CMainFrameEvent::Notify(TNotifyUI& msg)
{

	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}

	if (msg.pSender->GetName() == DEF_CLOSE_WND_BUTTON)
	{

		m_frameSmallMenu.DeleteSmallIcon();
	}
}


LRESULT CMainFrameEvent::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	m_frameSmallMenu.HandleCustomMessage(uMsg, wParam, lParam);



	return 0;
}

void CMainFrameEvent::OnTimer(TNotifyUI& msg)
{

}

void CMainFrameEvent::OnPrepare(TNotifyUI& msg)
{



}

void CMainFrameEvent::InitWindow(HWND hwnd)
{


	m_frameSmallMenu.Init();
	m_frameSmallMenu.CreateSmallIcon(hwnd, DEFINE_SMALL_ICON_PATH);

}


void CMainFrameEvent::OnClick(TNotifyUI& msg)
{



}


