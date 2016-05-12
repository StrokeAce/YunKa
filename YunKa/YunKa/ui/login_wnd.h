#ifndef _LOGIN_WND_
#define _LOGIN_WND_

#pragma once

#include "small_menu.h"
#include "chat_manager.h"





class CLoginWnd : public CWindowWnd, public INotifyUI, public IMessageFilterUI, public IBaseMsgs
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
	// 收到一个坐席用户的信息,用来初始化坐席列表
	virtual void RecvOneUserInfo(CUserObject* obj){}
	// 收到一个新建的会话消息
	virtual void RecvCreateChat(CWebUserObject* obj){}
	// 收到一个会话消息
	virtual void RecvChatInfo(CWebUserObject* obj){}
	// 收到更新用户的在线状态
	virtual void RecvUserStatus(CUserObject* obj){}
	virtual string GetLastError(){ return ""; }
	// 收到一条消息
	virtual void RecvOneMsg(IBaseObject* pObj, int msgFrom, string msgId, int msgType, int msgDataType,
		string msgContent, string msgTime, CUserObject* pAssistUser, WxMsgBase* msgContentWx, string msgExt){}
	// 坐席上线消息
	virtual void RecvOnline(CUserObject* obj){}
	// 坐席下线消息
	virtual void RecvOffline(CUserObject* obj){}
	// 会话关闭
	virtual void RecvCloseChat(CWebUserObject* pWebUser){}

	virtual void RecvAcceptChat(CUserObject* pUser, CWebUserObject* pWebUser) {}

	void StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd);

private:
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


};




#endif