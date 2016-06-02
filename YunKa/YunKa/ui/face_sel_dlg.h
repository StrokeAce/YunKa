#pragma once

#include "face_ctrl.h"

#define		WM_FACE_CTRL_SEL		WM_USER + 2000

#define		WM_FACE_CLOSE_DLG		WM_USER + 2001

#define		WM_FACE_CLOSE_DLG_2		WM_USER + 2002

class CFaceSelDlg : public WindowImplBase
{
public:
	CFaceSelDlg(void);
	~CFaceSelDlg(void);

public:
	LPCTSTR GetWindowClassName() const;	
	virtual void Init();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual UILIB_RESOURCETYPE GetResourceType() const;

public:
	void SetFaceList(CFaceList * lpFaceList);
	int GetSelFaceId();
	int GetSelFaceIndex();
	tstring GetSelFaceFileName();

	void GetFileNameById(int id, tstring &name);

	BOOL m_firstCreate;
	int m_nSelFaceId;
	int m_nSelFaceIndex;
	wstring m_strSelFaceFileName;

protected:
	void Notify(TNotifyUI& msg);

private:
	CFaceCtrl * m_pFaceCtrl;
	CFaceList * m_lpFaceList;

};
