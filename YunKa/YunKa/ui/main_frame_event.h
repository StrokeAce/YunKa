#pragma once
#include "small_menu.h"
#include "face_list.h"
#include "face_sel_dlg.h"


class CMainFrameEvent 
{
public:
	CMainFrameEvent();
	~CMainFrameEvent();

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg);

	void OnTimer(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg, HWND hwnd);


	void InitWindow(HWND hwnd);

public:
	//自己定义的操作函数
	void OnBtnFont(TNotifyUI& msg);
	void OnBtnFace(TNotifyUI& msg, HWND hwnd);
	void OnBtnScreen(TNotifyUI& msg);


	void SetPaintManager(CPaintManagerUI *val);

protected:

	CSmallMenu m_frameSmallMenu;
	HWND m_hMainHwnd;


	HWND m_hPaintMainWnd;
	CPaintManagerUI *m_PaintManager;

private:
	CButtonUI * m_pFontBtn, *m_pFaceBtn, *m_pScreenBtn;
	CFaceSelDlg m_faceSelDlg;
	CFaceList  m_faceList;






};

