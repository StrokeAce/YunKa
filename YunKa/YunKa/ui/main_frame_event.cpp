#include "../stdafx.h"
#include "main_frame_event.h"


CMainFrameEvent::CMainFrameEvent() 
{
	//初始化


	m_pFontBtn = m_pFaceBtn = m_pScreenBtn = NULL;

	//m_pSendEdit = NULL;
}




CMainFrameEvent::~CMainFrameEvent()
{

}


void CMainFrameEvent::SetPaintManager(CPaintManagerUI *val)
{
	m_PaintManager = val;
	m_hPaintMainWnd = m_PaintManager->GetPaintWindow();

	
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

	m_pFontBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnFont")));
	m_pFaceBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnFace")));
	m_pScreenBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("screenshotsbtn")));




}

void CMainFrameEvent::InitWindow(HWND hwnd)
{
	//右下角的 任务栏图标
	m_hMainHwnd = hwnd;

	m_frameSmallMenu.Init();
	m_frameSmallMenu.CreateSmallIcon(m_hMainHwnd, DEFINE_SMALL_ICON_PATH);

	wstring strPath = ZYM::CPath::GetCurDir() +_T("../bin/") _T("SkinRes\\Face\\FaceConfig.xml");
	m_faceList.LoadConfigFile(strPath.c_str());


}

//点击事件的处理
void CMainFrameEvent::OnClick(TNotifyUI& msg,HWND hwnd)
{
	if (msg.pSender == m_pFontBtn)
		OnBtnFont(msg);
	else if (msg.pSender == m_pFaceBtn)
		OnBtnFace(msg,hwnd);
	else if (msg.pSender == m_pScreenBtn)
		OnBtnScreen(msg);
}





void CMainFrameEvent::OnBtnFont(TNotifyUI& msg)
{
	//CContainerUI* pFontbar = static_cast<CContainerUI*>(paint_manager_.FindControl(_T("FontBar")));
	//if (pFontbar != NULL)
		//pFontbar->SetVisible(!pFontbar->IsVisible());




}

void CMainFrameEvent::OnBtnFace(TNotifyUI& msg,HWND hwnd)
{
	m_faceSelDlg.SetFaceList(&m_faceList);
	if (!::IsWindow((HWND)m_faceSelDlg))
	{
		m_faceSelDlg.Create(hwnd, NULL, WS_CHILD | WS_POPUP, WS_EX_TOOLWINDOW);

		RECT rcBtn = m_pFaceBtn->GetPos();
		::ClientToScreen(m_hMainHwnd, (LPPOINT)&rcBtn);

		int cx = 432;
		int cy = 236;
		int x = rcBtn.left - cx / 2;
		int y = rcBtn.top - cy;

		::SetWindowPos((HWND)m_faceSelDlg, NULL, x, y, cx, cy, NULL);
		::ShowWindow((HWND)m_faceSelDlg, SW_SHOW);

		m_faceSelDlg.Init();

		//m_pFaceBtn->SetNormalImage(L"E:\\UI\\YunKa\\bin\\SkinRes\\mainframe\\icon_face.png");
	}

}

void CMainFrameEvent::OnBtnScreen(TNotifyUI& msg)
{


}