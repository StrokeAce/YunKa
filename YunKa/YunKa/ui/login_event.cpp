#include "../stdafx.h"
#include "login_event.h"


CLoginEvent::CLoginEvent(CPaintManagerUI &pm):m_loginEvpm(pm)
{
}


CLoginEvent::~CLoginEvent()
{
}


void CLoginEvent::Init(HWND hwnd)
{
	m_hLoginMenu.Init();
	m_hLoginMenu.CreateSmallIcon(hwnd, DEFINE_SMALL_ICON_PATH);







#if 0
	/*
	CComboUI* pAccountCombo = static_cast<CComboUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_COMBO));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
	if (pAccountCombo && pAccountEdit) pAccountEdit->SetText(pAccountCombo->GetText());
	pAccountEdit->SetFocus();
	*/


	//m_hLoginMenu.Init(this->GetHWND());
	//m_hLoginMenu.CreateSmallIcon(this->GetHWND(), _T("E:\\Moliyun\\bin\\SkinRes\\small.ico"));




	string path = GetCurrentPath() + "\\Res\\login\\1.ico";
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
#endif


}
int CLoginEvent::Start(TNotifyUI& msg)
{


	if (msg.pSender->GetName() == DEF_LOGIN_WND_BUTTON && msg.sType == DUI_MSGTYPE_CLICK)
		{
		m_hLoginMenu.DeleteSmallIcon();

		return 0;
			
		}


	return -1;


#if 0

	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_loginEvpm.FindControl(DEF_ACCOUNT_ID_EDIT));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_ACCOUNT_ID_EDIT));
	if (pAccountEdit)
	{
		CDuiString userName = pAccountEdit->GetText();
		strcpy(_GlobalSetting.m_loginRequest.user_name, UnicodeToANSI(userName.GetData()));
	}
	CEditUI* pWordEdit = static_cast<CEditUI*>(m_pm.FindControl(DEF_PASSWORD_TEXT_EDIT));
	if (pWordEdit)
	{
		CDuiString passWord = pWordEdit->GetText();
		strcpy(_GlobalSetting.m_loginRequest.password, UnicodeToANSI(passWord.GetData()));
	}
	_GlobalSetting.m_interference->Login(_GlobalSetting.m_loginRequest, _GlobalSetting.m_loginResponse);

	if (_GlobalSetting.m_loginResponse.code == 1) //µÇÂ¼³É¹¦
	{
		m_hLoginMenu.DeleteSmallIcon();
		Close();
	}
	else
	{

		//Ê§°Ü
	}

#endif






	return 0;

}
void CLoginEvent::Notify(TNotifyUI& msg)
{

	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == DEF_CLOSE_WND_BUTTON || msg.pSender->GetName() == DEF_CANCEL_WND_BUTTON)
		{
			m_hLoginMenu.DeleteSmallIcon();
			PostQuitMessage(0);
			return;
		}


	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender->GetName() == DEF_ACCOUNT_ID_COMBO)
		{
			CEditUI* pAccountEdit = static_cast<CEditUI*>(m_loginEvpm.FindControl(DEF_ACCOUNT_ID_EDIT));
			if (pAccountEdit) pAccountEdit->SetText(msg.pSender->GetText());
		}
	}

}


LRESULT CLoginEvent::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_hLoginMenu.HandleCustomMessage(uMsg, wParam, lParam);



	return 0;
}