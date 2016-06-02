#include "../stdafx.h"
#include "face_sel_dlg.h"

CFaceSelDlg::CFaceSelDlg(void)
{
	m_lpFaceList = NULL;
	m_nSelFaceId = -1;
	m_nSelFaceIndex = -1;
	m_strSelFaceFileName = _T("");

	m_firstCreate = false;
}

CFaceSelDlg::~CFaceSelDlg(void)
{
}

LPCTSTR CFaceSelDlg::GetWindowClassName() const
{
	return _T("DUI_WINDOW");
}

void CFaceSelDlg::Init()
{
	m_pFaceCtrl = static_cast<CFaceCtrl*>(m_PaintManager.FindControl(_T("FaceCtrl1")));

	//m_pFaceCtrl->SetBgColor(RGB(255, 255, 255));
	m_pFaceCtrl->SetLineColor(RGB(223, 230, 246));
	m_pFaceCtrl->SetFocusBorderColor(RGB(0, 0, 255));
	m_pFaceCtrl->SetZoomBorderColor(RGB(0, 138, 255));
	m_pFaceCtrl->SetRowAndCol(8, 15);
	m_pFaceCtrl->SetItemSize(28, 28);
	m_pFaceCtrl->SetZoomSize(84, 84);
	m_pFaceCtrl->SetFaceList(m_lpFaceList);
	m_pFaceCtrl->SetCurPage(0);

	m_nSelFaceId = -1;
	m_nSelFaceIndex = -1;
	m_strSelFaceFileName = _T("");

	m_firstCreate = true;
}

CDuiString CFaceSelDlg::GetSkinFile()
{

	return _T("FaceSelDlg.xml");
}

CDuiString CFaceSelDlg::GetSkinFolder()
{
	return  _T("SkinRes\\");

	//return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("SkinRes\\");
}

CControlUI* CFaceSelDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FaceCtrl")) == 0)
		return new CFaceCtrl;
	return NULL;
}

LRESULT CFaceSelDlg::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CFaceSelDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

LRESULT CFaceSelDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ACTIVATE)
	{
		if (WA_INACTIVE == (UINT)LOWORD(wParam))
			::PostMessage(m_hWnd, WM_FACE_CLOSE_DLG_2, NULL, NULL);
	}

	else if (uMsg == WM_FACE_CLOSE_DLG)
	{
		::PostMessage(::GetParent(m_hWnd), WM_FACE_CTRL_SEL, NULL, NULL);
		//::DestroyWindow(m_hWnd);

		::ShowWindow(m_hWnd, SW_HIDE);
		return 0;
	}

	else if (uMsg == WM_FACE_CLOSE_DLG_2)
	{
		::ShowWindow(m_hWnd, SW_HIDE);
		return 0;
	}


	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CFaceSelDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void CFaceSelDlg::SetFaceList(CFaceList * lpFaceList)
{
	m_lpFaceList = lpFaceList;
}

int CFaceSelDlg::GetSelFaceId()
{
	return m_nSelFaceId;
}

int CFaceSelDlg::GetSelFaceIndex()
{
	return m_nSelFaceIndex;
}

tstring CFaceSelDlg::GetSelFaceFileName()
{
	return m_strSelFaceFileName;
}

void CFaceSelDlg::GetFileNameById(int id, tstring &name)
{
	CFaceInfo *info = m_lpFaceList->GetFaceInfoById(id);
	name = info->m_strFileName;

}

void CFaceSelDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pFaceCtrl)
		{
			int nSelIndex = (int)msg.lParam;
			CFaceInfo * lpFaceInfo = m_pFaceCtrl->GetFaceInfo(nSelIndex);
			if (lpFaceInfo != NULL)
			{
				m_nSelFaceId = lpFaceInfo->m_nId;
				m_nSelFaceIndex = lpFaceInfo->m_nIndex;
				m_strSelFaceFileName = lpFaceInfo->m_strFileName;
			}
			::PostMessage(m_hWnd, WM_FACE_CLOSE_DLG, NULL, NULL);
		}
	}
}

UILIB_RESOURCETYPE CFaceSelDlg::GetResourceType() const
{
	return UILIB_FILE;
}
