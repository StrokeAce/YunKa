#ifndef _LOGIN_WND_
#define _LOGIN_WND_

#pragma once

#include "small_menu.h"
#include "chat_manager.h"
#include "main_frame.h"





class CLoginWnd : public CWindowWnd, public INotifyUI, public IMessageFilterUI, public IHandlerLgoin
{
public:
	CLoginWnd();
	~CLoginWnd();
	LPCTSTR GetWindowClassName() const { return _T("UILoginFrame"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND /*hWnd*/)
	{
		m_pm.RemovePreMessageFilter(this);
		delete this;
	};

	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMenuHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
public:
	virtual void LoginProgress(int percent);

	void StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd);
	void CLoginWnd::OnItemSelected(TNotifyUI& msg);

public:
	CChatManager* m_manager;


public:

	void OnPrepare(TNotifyUI& msg);
	void OnLoginButton();
	int GetLoginResult(int percent);

private:
	CPaintManagerUI m_pm;


	CSmallMenu m_hLoginMenu;

	CButtonUI * m_pLoginBtn, *m_pCancelBtn,*m_pCloseBtn;
	CEditUI *pAccountEdit, *m_pPasswordEdit;
	
	CComboUI *pAccountCombo;
	CCheckBoxUI *m_pSaveWordCheckBox, *m_pAuotoLoginCheckBox;

	ListLoginedInfo m_loginListInfo;


};




#endif