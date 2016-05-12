#include "../stdafx.h"
#include "login_wnd.h"
#include "common_utility.h"





CLoginWnd::CLoginWnd()
{

	m_pLoginBtn = m_pCancelBtn = m_pCloseBtn = NULL;
	pAccountEdit = m_pPasswordEdit = NULL;
	pAccountCombo = NULL;

	m_pSaveWordCheckBox = m_pAuotoLoginCheckBox = NULL;
}


CLoginWnd::~CLoginWnd()
{
}




void CLoginWnd::OnPrepare(TNotifyUI& msg)
{
	CDuiString accountName;
	//µÇÂ¼È¡Ïû°´Å¥ 
	m_pLoginBtn = static_cast<CButtonUI*>(m_pm.FindControl(DEF_LOGIN_WND_BUTTON));
	m_pCancelBtn = static_cast<CButtonUI*>(m_pm.FindControl(DEF_CANCEL_WND_BUTTON));

	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(DEF_CLOSE_WND_BUTTON));

	pAccountCombo = static_cast<CComboUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_COMBO));
	pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));

	m_pSaveWordCheckBox = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("savePwdCheck")));
	m_pAuotoLoginCheckBox = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("autoLoginCheck")));
	if (m_pSaveWordCheckBox != NULL)
		m_pSaveWordCheckBox->SetCheck(false);
	if (m_pAuotoLoginCheckBox != NULL)
		m_pAuotoLoginCheckBox->SetCheck(true);



	CListLabelElementUI* pElementUsername = new CListLabelElementUI;
	pElementUsername->SetText(DEBUG_USERNAME_TEXT);
	pAccountCombo->Add(pElementUsername);
	//test	
	CListLabelElementUI* pElementTest = new CListLabelElementUI;
	accountName = _T("1111111");
	pElementTest->SetText(accountName);
	pAccountCombo->Add(pElementTest);
	


	//if (pAccountCombo && pAccountEdit)
	//	pAccountEdit->SetText(DEBUG_USERNAME_TEXT);
	pAccountCombo->SelectItem(0);




	m_pPasswordEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_PASSWORD_TEXT_EDIT));
	if (m_pPasswordEdit)
		m_pPasswordEdit->SetText(DEBUG_PASSWORD_TEXT);






}


void CLoginWnd::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}

	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pLoginBtn )
		{
			OnLoginButton();
		}
		else if (msg.pSender == m_pCancelBtn || msg.pSender == m_pCloseBtn)
		{
			m_hLoginMenu.DeleteSmallIcon();
			PostQuitMessage(0);
		}
	}
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender->GetName() == DEF_ACCOUNT_ID_COMBO)
		{
			 pAccountEdit->SetText(msg.pSender->GetText());

		}
	}
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		if (msg.pSender->GetName() == DEF_ACCOUNT_ID_COMBO)
		{

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




	m_hLoginMenu.Init();
	m_hLoginMenu.CreateSmallIcon(this->GetHWND(), DEFINE_SMALL_ICON_PATH);



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
	case WM_MY_MESSAGE_NOTIFYICON:    HandleCustomMessage(uMsg, wParam, lParam,bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CLoginWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{

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
			//PostQuitMessage(0);
			return true;
		}

	}



#if 0
	//	m_hLoginMenu.HandleCustomMessage(uMsg, wParam, lParam);

	if (uMsg == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
			//	case IDM_MENU_QUIT: //ÍË³ö


			break;

			//	case IDM_MENU_RELOGIN: //ÖØÐÂµÇÂ¼

			break;

		default:
			break;
		}
	}
#endif

	return false;
}



LRESULT CLoginWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{


	m_hLoginMenu.HandleCustomMessage(uMsg,wParam,lParam);



	return 0;
}


void CLoginWnd::OnLoginButton()
{

	//bool isAutoLogin = false;
	//string loginName = "9692111";
	//string password = "123";
	//bool isKeepPwd = false;

	//string error;


	//test
	//m_hLoginMenu.DeleteSmallIcon();
	//Close();
	//return;


	CDuiString userName = pAccountEdit->GetText();
	CDuiString passWord = m_pPasswordEdit->GetText();

	UnicodeToANSI(userName, _globalSetting.m_userName);
	UnicodeToANSI(passWord, _globalSetting.m_passWord);

	bool isKeepPwd = m_pSaveWordCheckBox->GetCheck();
	bool isAutoLogin = m_pAuotoLoginCheckBox->GetCheck();

		
	StartLogin(_globalSetting.m_userName, _globalSetting.m_passWord, isAutoLogin, isKeepPwd);



}

void CLoginWnd::LoginProgress(int percent)
{
	if (percent == 100)
	{
		//µÇÂ¼³É¹¦ 

		//m_manager->SendTo_GetShareList();



		m_hLoginMenu.DeleteSmallIcon();
		Close();
	}
	else if (percent < 0 || percent>100)  //·µ»ØÊ§°Ü
	{
		MessageBox(this->GetHWND(),L"µÇÂ¼Ê§°Ü",L"µÇÂ¼Ê§°Ü",0);
	}
}

void CLoginWnd::StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{
	m_manager = CChatManager::GetInstance();

	//bool isAutoLogin = false;
	//string loginName = "9692111";
	//string password = "123";
	//bool isKeepPwd = false;

	//string error;

	m_manager->StartLogin(loginName, password, isAutoLogin, isKeepPwd);

}
	


