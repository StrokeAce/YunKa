#include "stdafx.h"
#include "login_wnd.h"




CLoginWnd::CLoginWnd() 
{ 
}


CLoginWnd::~CLoginWnd()
{
}


void CLoginWnd::Init() {
	/*
	CComboUI* pAccountCombo = static_cast<CComboUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_COMBO));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
	if (pAccountCombo && pAccountEdit) pAccountEdit->SetText(pAccountCombo->GetText());
	pAccountEdit->SetFocus();
	*/


	m_hLoginMenu.Init();
	m_hLoginMenu.CreateSmallIcon(this->GetHWND(), _T("E:\\Moliyun\\bin\\SkinRes\\small.ico"));

#if 0
	string path = GetCurrentPath()+"\\Res\\login\\1.ico";
	WCHAR pathIcon[256];
	_tcscpy(pathIcon, ANSIToUnicode(path.c_str()));
	m_hLoginMenu.Init();
	m_hLoginMenu.CreateSmallIcon(this->GetHWND(), pathIcon);


	CComboUI* pAccountCombo = static_cast<CComboUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_COMBO));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
	if (pAccountCombo && pAccountEdit)
		pAccountEdit->SetText(DEBUG_USERNAME_TEXT);

	CEditUI* pWordEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_PASSWORD_TEXT_EDIT));
	if (pWordEdit)
		pWordEdit->SetText(DEBUG_PASSWORD_TEXT);
	//pAccountEdit->SetFocus();

#endif


	/*
	CDuiString strIcon(_T("res\\2.ico"));


	HICON hIconSmall = (HICON)::LoadImage(GetModuleHandle(NULL),
		strIcon.GetData(),
		IMAGE_ICON,
		48,//::GetSystemMetrics(SM_CXSMICON),
		48,//::GetSystemMetrics(SM_CYSMICON),
		LR_LOADFROMFILE);

	::SetIcon(hIconSmall,false);
	*/


}

void CLoginWnd::LoginStart()
{
#if 0
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
	if (pAccountEdit)
	{
		CDuiString userName = pAccountEdit->GetText();
		strcpy(_GlobalSetting.m_loginRequest.user_name , UnicodeToANSI(userName.GetData()));
	}
	CEditUI* pWordEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_PASSWORD_TEXT_EDIT));
	if (pWordEdit)
	{
		CDuiString passWord = pWordEdit->GetText();
		strcpy(_GlobalSetting.m_loginRequest.password , UnicodeToANSI(passWord.GetData()));
	}
	_GlobalSetting.m_interference->Login(_GlobalSetting.m_loginRequest, _GlobalSetting.m_loginResponse);

	if (_GlobalSetting.m_loginResponse.code == 1 ) //登录成功
	{
		m_hLoginMenu.DeleteSmallIcon();
		Close();
	}
	else
	{

		//失败
	}

#endif


	m_hLoginMenu.DeleteSmallIcon();

	Close();
	
}


void CLoginWnd::Notify(TNotifyUI& msg)
{

	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == DEF_CLOSE_WND_BUTTON || msg.pSender->GetName() == DEF_CANCEL_WND_BUTTON)
		{
			//m_hLoginMenu.DeleteSmallIcon();
			PostQuitMessage(0);
			return;
		}
		else if (msg.pSender->GetName() == DEF_LOGIN_WND_BUTTON)
		{
			LoginStart();
			return;
		}
	
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT) 
	{
		if (msg.pSender->GetName() == DEF_ACCOUNT_ID_COMBO) 
		{
			CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
			if (pAccountEdit) pAccountEdit->SetText(msg.pSender->GetText());
		}
	}

}

LRESULT CLoginWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	
	CControlUI* pRoot = builder.Create(_GLOBAL_LOGIN_UI_CONFIG_FILE, (UINT)0, 0, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	//SetIcon(101);

	Init();
	return 0;
}

LRESULT CLoginWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CLoginWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLoginWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLoginWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CLoginWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CLoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MY_MESSAGE_NOTIFYICON:    lRes = m_hLoginMenu.HandleCustomMessage(uMsg, wParam, lParam); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CLoginWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
#if 0
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_RETURN) {
			CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
			if (pEdit->GetText().IsEmpty()) pEdit->SetFocus();
			else {
				pEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_PASSWORD_TEXT_EDIT));
				if (pEdit->GetText().IsEmpty()) pEdit->SetFocus();
				else Close();
			}
			return true;
		}
		else if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
			return true;
		}

	}

#endif


		m_hLoginMenu.HandleCustomMessage(uMsg, wParam, lParam);

#if 0
//	m_hLoginMenu.HandleCustomMessage(uMsg, wParam, lParam);

	if (uMsg == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
	//	case IDM_MENU_QUIT: //退出

			
			break;

	//	case IDM_MENU_RELOGIN: //重新登录

			break;

		default:
			break;
		}
	}
#endif

	return false;
}

