#include "../stdafx.h"
#include "main_frame.h"
#include "login_wnd.h"
#include "global_setting_define.h"
#include "ui_menu.h"


DUI_BEGIN_MESSAGE_MAP(CMainFrame, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMRCLICK, OnItemRbClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK, OnHeaderClick)
//DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSELEAVE, OnMouseEnter)			
//DUI_ON_MSGTYPE(DUI_MSGTYPE_MENU, OnMenu)
DUI_END_MESSAGE_MAP()


CMainFrame::CMainFrame()
{
	m_frameEvent = new CMainFrameEvent(m_PaintManager,m_hWnd);
}

CMainFrame::~CMainFrame()
{
	PostQuitMessage(0);
}

LPCTSTR CMainFrame::GetWindowClassName() const
{
	return _T("CMainFrame");
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this;
}


CDuiString CMainFrame::GetSkinFile()
{
	return _T("MainFrame.xml");
}

CDuiString CMainFrame::GetSkinFolder()
{
	return  _T("SkinRes\\");
}

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		//Exit();
		PostQuitMessage(0); //防止有模态窗口时，从任务栏关闭会报错
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}

	return lRes;

}

LRESULT CMainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	m_frameEvent->HandleCustomMessage(uMsg, wParam, lParam);

	return 0;
}


LRESULT CMainFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	//int x = GET_X_LPARAM(lParam);
	//int y = GET_Y_LPARAM(lParam);

	return 0;
}


UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_FILE;
}

LPCTSTR CMainFrame::GetResourceID() const
{
	return MAKEINTRESOURCE(101);
}


#if 0
void CMainFrame::Notify(TNotifyUI& msg)
{

	

	return WindowImplBase::Notify(msg);
}
#endif

void CMainFrame::Notify(TNotifyUI& msg)
{
	m_frameEvent->Notify(msg);

	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}

	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
	}

	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
	}


	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
	}

	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
	}

}



void CMainFrame::OnTimer(TNotifyUI& msg)
{
}

void CMainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}


void CMainFrame::OnPrepare(TNotifyUI& msg)
{


}
#include "IImageOle.h"

void CMainFrame::InitWindow()
{
	m_frameEvent->InitWindow(m_hWnd);


	CRichEditUI* m_pSendEdit;
	m_pSendEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richSend")));



	IRichEditOleCallback2* pRichEditOleCallback2 = NULL;

	/*
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pRecvEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}

	*/

	pRichEditOleCallback2 = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pSendEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}

	IDropTarget *pdt = m_pSendEdit->GetTxDropTarget();
	hr = ::RegisterDragDrop(m_hWnd, pdt);
	pdt->Release();

}

void CMainFrame::OnClick(TNotifyUI& msg)
{

#if 1
	m_frameEvent->OnClick(msg,m_hWnd);
	if (msg.pSender->GetName() == DEF_CLOSE_WND_BUTTON)
	{
		Close();
	}
	else  if (msg.pSender->GetName() == DEF_MIN_WND_BUTTON)
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		//this->ShowWindow(FALSE);
		//SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);

	}
	else  if (msg.pSender->GetName() == DEF_RESTORE_WND_BUTTON)
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

	}
	else  if (msg.pSender->GetName() == DEF_MAX_WND_BUTTON)
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	}

	else  if (msg.pSender->GetName() == L"acceptbutton")
	{

		///CLoginWnd* pLoginFrame = new CLoginWnd();
		//pLoginFrame->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		//pLoginFrame->CenterWindow();
		//int result = pLoginFrame->ShowModal();

		//return;

		//CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
		//CPoint point = msg.ptMouse;

		//CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
		//CPoint point = msg.ptMouse;
		//ClientToScreen(m_hWnd, &point);
		//pMenu->Init(NULL, _T(""), _T("xml"), point);



		//pMenu->Create(NULL, _T(""), WS_POPUP, 0, 0, 0, 0, 0, NULL);
		//pMenu->CenterWindow();
		//pMenu->ShowModal();

	}

#endif

#if 0
	//gMainGate->UserManager()->NotifyMsg(msg);
	//gMainGate->GiftManager()->NotifyMsg(msg);

	//if( msg.pSender == m_pCloseBtn ) {
	//	Close();
	//	return; 
	//}
	//else if( msg.pSender == m_pMinBtn ) 
	//{ 
	//	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
	//this->ShowWindow(FALSE);

	SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);

	return;
}
	else if (msg.pSender == m_pMaxBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (msg.pSender == m_pRestoreBtn) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
	else if (msg.pSender->GetName() == _T("btnAbout"))
	{
		//ShowAboutWnd();
		return;
	}
	else if (msg.pSender->GetName() == _T("btnCloseBroad"))
	{
		//CHorizontalLayoutUI* pH = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("pnlBroad")));
		//if (pH) pH->SetVisible(false);

		//static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optViewer")))->Selected(TRUE);
		//static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optHall")))->Selected(TRUE);

		//CControlUI *pCtrl = m_PaintManager.FindControl(_T("tabMain"));
		///if (pCtrl == NULL) return;

		//static_cast<CTabLayoutUI*>(pCtrl)->SelectItem(0);

		//m_UIChat.CloseRoom();

		//ExitRoom();

		return;
	}
#endif

}

void CMainFrame::OnSelectChanged(TNotifyUI &msg)
{
	//gMainGate->UserManager()->NotifyMsg(msg);
	//gMainGate->GiftManager()->NotifyMsg(msg);

	if (msg.pSender->GetName() == _T("optHall"))
	{
		//CControlUI *pCtrl = m_PaintManager.FindControl(_T("tabMain"));
		//if (pCtrl == NULL) return;

		//static_cast<CTabLayoutUI*>(pCtrl)->SelectItem(0);

		//ShowMainVideoWnd(false);
	}
	else if (msg.pSender->GetName() == _T("optBroad"))
	{
		//CControlUI *pCtrl = m_PaintManager.FindControl(_T("tabMain"));
		//if (pCtrl == NULL) return;

		//static_cast<CTabLayoutUI*>(pCtrl)->SelectItem(1);
		//
		//ShowMainVideoWnd(true);
	}

}

void CMainFrame::OnItemClick(TNotifyUI &msg)
{
}


void CMainFrame::OnItemRbClick(TNotifyUI &msg)
{

}

void CMainFrame::OnItemSelect(TNotifyUI &msg)
{



}




void CMainFrame::OnHeaderClick(TNotifyUI& msg)
{
}

void CMainFrame::OnMouseEnter(TNotifyUI& msg)
{
}




#if 0

void CMainFrame::InitWindow()
{}

DWORD CMainFrame::GetBkColor()
{
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
		return background->GetBkColor();

	return 0;
}

void MainFrame::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();

		SkinChangedParam param;
		param.bkcolor = background->GetBkColor();
		param.bgimage = background->GetBkImage();
		skin_changed_observer_.Broadcast(param);
	}
}

#endif