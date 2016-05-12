#include "../stdafx.h"
#include "main_frame.h"
#include "login_wnd.h"
#include "global_setting_define.h"
#include "ui_menu.h"

#include "IImageOle.h"
#include "ole_helper.h"

#include "chat_common\comfunc.h"
#include "utils\code_convert.h"
#include "user_list.h"
#include "rich_edit_help.h"




CMainFrame::CMainFrame(CChatManager* manager) :m_manager(manager)
{

	//初始化
	m_pFontBtn = m_pFaceBtn = m_pScreenBtn = pSendMsgBtn = NULL;

}



CMainFrame::~CMainFrame()
{
	delete m_manager;
}

LPCTSTR CMainFrame::GetWindowClassName() const
{
	return _T("CMainFrame");
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{


	if (_tcscmp(pstrClass, _T("RichEdit2")) == 0)
		return new DuiLib2::CRichEditUI2;

	

	if (_tcscmp(pstrClass, _T("UserList")) == 0) 
		return new UserListUI;

	return NULL;
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{

	WindowImplBase::OnFinalMessage(hWnd);

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

	//::RevokeDragDrop(m_hWnd);
	//m_faceList.Reset();

	return 0;
}


LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) 
	{
		PostQuitMessage(0); //防止有模态窗口时，从任务栏关闭会报错
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxBtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restoreBtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxBtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restoreBtn")));
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
    if (uMsg == WM_FACE_CTRL_SEL)
	{

		GetSelectFaceId();
	}



	if (uMsg == WM_MOUSEMOVE)
		OnMouseMove(uMsg, wParam, lParam);

	if (uMsg == WM_RBUTTONDOWN)
	{
//		LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
//		OnRButtonDown(uMsg, wParam, lParam);
	//	return lRes;
	}

//	if (uMsg == WM_LBUTTONDBLCLK)
//		OnLButtonDblClk(uMsg, wParam, lParam);

//	if ((m_pSendEdit != NULL) && m_pSendEdit->IsFocused()
//		&& (uMsg == WM_KEYDOWN) && (wParam == 'V') && (lParam & VK_CONTROL))	// 发送消息框的Ctrl+V消息
//	{
//		m_pSendEdit->PasteSpecial(CF_TEXT);
//		return TRUE;
//	}

//	if (uMsg == WM_NOTIFY && EN_PASTE == ((LPNMHDR)lParam)->code)
//	{
	//	ITextServices * pTextServices = m_pSendEdit->GetTextServices();
	//	if ((UINT)pTextServices == ((LPNMHDR)lParam)->idFrom)
	//	{
	//		AddMsgToSendEdit(((NMRICHEDITOLECALLBACK *)lParam)->lpszText);
//		}
//		if (pTextServices != NULL)
//			pTextServices->Release();
	//}

	//if (uMsg == WM_MENU)
	//{
	//	CControlUI * pControl = (CControlUI *)lParam;
	//	if (pControl != NULL)
	//		this->paint_manager_.SendNotify(pControl, _T("menu_msg"), wParam, lParam);
	//}

	//if (uMsg == FACE_CTRL_SEL)
		//return OnFaceCtrlSel(uMsg, wParam, lParam);

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//bHandled = FALSE;


	m_frameSmallMenu.HandleCustomMessage(uMsg, wParam, lParam);

	return 0;
}


LRESULT CMainFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	return 0;
}


UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_FILE;
}




void CMainFrame::Notify(TNotifyUI& msg)
{

	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}

	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		OnClick(msg);
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
		UserListUI* pUserList = static_cast<UserListUI*>(m_PaintManager.FindControl(_T("userlist")));
		if (pUserList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0) {
				UserListUI::Node* node = (UserListUI::Node*)msg.pSender->GetTag();
				pUserList->ExpandNode(node, !node->data()._expand);
				if (node->data()._level == 3) {
					COptionUI* pControl = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("roomswitch")));
					if (pControl) {
						CHorizontalLayoutUI* pH = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("roomswitchpanel")));
						if (pH) pH->SetVisible(true);
						pControl->SetText(node->parent()->parent()->data()._text);
						pControl->Activate();
					}
				}
			}
		}
	}

	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{

		UserListUI* pUserList = static_cast<UserListUI*>(m_PaintManager.FindControl(_T("userlist")));
		if (pUserList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0) {
				UserListUI::Node* node = (UserListUI::Node*)msg.pSender->GetTag();

				POINT pt = { 0 };
				::GetCursorPos(&pt);
				::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
				pt.x -= msg.pSender->GetX();
				SIZE sz = pUserList->GetExpanderSizeX(node);
				if (pt.x >= sz.cx && pt.x < sz.cy)
					pUserList->ExpandNode(node, !node->data()._expand);
			}
		}
	}



}



void CMainFrame::OnTimer(TNotifyUI& msg)
{
}

void CMainFrame::OnExit(TNotifyUI& msg)
{
	m_manager->Exit();
	m_frameSmallMenu.DeleteSmallIcon();
	Close();
	PostQuitMessage(0);


}


void CMainFrame::OnPrepare(TNotifyUI& msg)
{
	CDuiString nameString = _T("");
	CDuiString typeString[4] = { _T("对话中"), _T("转接中"), _T("邀请中"), _T("内部对话") };





	//聊天窗口 初始化
	m_pListMsgHandler.handler = NULL;
	m_pListMsgHandler.handleName = Handler_ListMsg;
	m_pListMsgHandler.isLoaded = false;
	m_pListMsgHandler.isCreated = false;


	m_hMainWnd = m_PaintManager.GetPaintWindow();
	m_pSendEdit = static_cast<CRichEditUI2*>(m_PaintManager.FindControl(_T("richSend")));


	//m_pSendEdit->DoInit();
	//IRichEditOleCallback2* pRichEditOleCallback2 = NULL;
	//HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
	//	__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	//if (SUCCEEDED(hr))
	//{
	//	pRichEditOleCallback2->SetNotifyHwnd(m_hMainWnd);
	//	ITextServices * pTextServices = m_pSendEdit->GetTextServices();
	//	pRichEditOleCallback2->SetTextServices(pTextServices);
	//	pTextServices->Release();
	//	//m_pSendEdit->iri
	//	pRichEditOleCallback2->Release();
	//}
	//
	//IDropTarget *pdt = m_pSendEdit->GetTxDropTarget();
	//HRESULT  hrer = ::RegisterDragDrop(m_hMainWnd, pdt);
	//pdt->Release();


	//中间栏按钮
	m_pFontBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnFont")));
	m_pFaceBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnFace")));
	m_pScreenBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screenshotsbtn")));
	pSendMsgBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("sendMsgBtn")));

	//右下角小图标
	m_frameSmallMenu.Init();
	m_frameSmallMenu.CreateSmallIcon(this->m_hWnd, DEFINE_SMALL_ICON_PATH);


	//表情包显示
	wstring strPath = ZYM::CPath::GetCurDir() + _T("../bin/") _T("SkinRes\\Face\\FaceConfig.xml");
	m_faceList.LoadConfigFile(strPath.c_str());



	//左侧用户列表显示
	UserListUI* pUserList = static_cast<UserListUI*>(m_PaintManager.FindControl(_T("userlist")));

	

	UserListUI::Node* pCategoryNode = NULL;
	UserListUI::Node* pGameNode = NULL;
	UserListUI::Node* pServerNode = NULL;
	UserListUI::Node* pRoomNode = NULL;
	pCategoryNode = pUserList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}对话列表"));

	nameString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}等待开始"));
	pGameNode = pUserList->AddNode(nameString, pCategoryNode);

	for (int i = 0; i < 4; ++i)
	{
		nameString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}测试%d"), i + 1);
		pGameNode = pUserList->AddNode(nameString, pCategoryNode);
		for (int i = 0; i < 3; ++i)
		{
			nameString = _T("{x 4}{i gameicons.png 18 10}{x 4}");
			
			nameString += typeString[i];
		
			pServerNode = pUserList->AddNode(nameString, pGameNode);
			//for (int i = 0; i < 3; ++i)
			//{
			//	pRoomNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}测试房间"), pServerNode);
			//}
		}
	}




	//显示聊天内容的libcef窗口
	m_pListMsgHandler.handler = new ClientHandler();
	m_pListMsgHandler.handler->m_isDisplayRefresh = false;

	if (!m_pListMsgHandler.isCreated)
	{
		string localUrl = GetCurrentPath();    
		localUrl += ("\\html\\list.html");
		CCodeConvert f_covet;
		string utfUrl;
		f_covet.Gb2312ToUTF_8(utfUrl, localUrl.c_str(), localUrl.length());
		RECT rc = {304,164,304+378,398+164};

		//RECT rc = { 0, 0, 0, 0 };

		//m_pListMsgHandler.handler->CreateBrowser(this->m_hWnd, rc, "www.baidu.com", Handler_ListMsg);

	//	m_pListMsgHandler.handler->GetBrowser()->GetMainFrame()->LoadURL("www.baidu.com");
	}



	m_manager->SetHandlerMsgs(this);
}





void CMainFrame::OnClick(TNotifyUI& msg)
{

	if (msg.pSender == m_pFontBtn)
		OnBtnFont(msg);
	else if (msg.pSender == m_pFaceBtn)
		OnBtnFace(msg);
	else if (msg.pSender == m_pScreenBtn)
		OnBtnScreen(msg);
	else  if (msg.pSender == pSendMsgBtn)
		OnBtnSendMessage(msg);

	if (msg.pSender->GetName() == DEF_CLOSE_WND_BUTTON)
	{
		OnExit(msg);
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



}

void CMainFrame::OnSelectChanged(TNotifyUI &msg)
{

	if (msg.pSender->GetName() == _T("optHall"))
	{

	}
	else if (msg.pSender->GetName() == _T("optBroad"))
	{
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





void CMainFrame::OnBtnFont(TNotifyUI& msg)
{





}

void CMainFrame::OnBtnFace(TNotifyUI& msg)
{
	if (!m_faceSelDlg.m_firstCreate)
	{
		m_faceSelDlg.SetFaceList(&m_faceList);
		if (!::IsWindow((HWND)m_faceSelDlg))
		{
			m_faceSelDlg.Create(this->m_hWnd, NULL, WS_CHILD | WS_POPUP, WS_EX_TOOLWINDOW);

			RECT rcBtn = m_pFaceBtn->GetPos();
			::ClientToScreen(this->m_hWnd, (LPPOINT)&rcBtn);

			int cx = 432;
			int cy = 236;
			int x = rcBtn.left - cx / 2;
			int y = rcBtn.top - cy;

			::SetWindowPos((HWND)m_faceSelDlg, NULL, x, y, cx, cy, NULL);
			::ShowWindow((HWND)m_faceSelDlg, SW_SHOW);

			m_faceSelDlg.Init();

		}
	}
	else
	{
		::ShowWindow((HWND)m_faceSelDlg, SW_SHOW);
	}

	

}

void CMainFrame::OnBtnScreen(TNotifyUI& msg)
{


}

//选择表情后的处理
void CMainFrame::GetSelectFaceId()
{
	//首先获取 当前发送框的内容 然后加上表情内容
	//m_sendMsgString = m_pSendEdit->GetText();

//	m_pSendEdit->m_pTwh->GetTextServices()

	m_sendMsgString = m_pSendEdit->GetTextRange(0, 100);

	CDuiString path = m_faceSelDlg.m_strSelFaceFileName.c_str();
	m_pSendEdit->InsertImage(path, 0);

	m_sendMsgString = m_pSendEdit->GetTextRange(20, 100);


	m_pSendEdit->Clear();

}

void CMainFrame::OnBtnSendMessage(TNotifyUI& msg)
{

//	m_sendMsgString = { m_pstr = 0x066e0f7c L"sdsdsdsdsdsds￼" m_szBuffer = 0x066e0f7c L"sdsdsdsdsdsds￼" }

	//m_sendMsgString = m_pSendEdit->GetText();




	//ITextServices * pTextServices = m_pSendEdit->GetTextServices();


	//ITextServices * pTextServices = m_pSendEdit->GetTextServices();

	//tstring strText;
	//RichEdit_GetText(pTextServices, strText);

	//pTextServices->Release();

	//if (strText.size() <= 0)
	//	return;


}

BOOL CMainFrame::_RichEdit_InsertFace(CRichEditUI2 * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex)
{
	BOOL bRet = FALSE;

	if (NULL == pRichEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pRichEdit->GetTextServices();


	//ITextHost * pTextHost = pRichEdit->GetTextHost();
	//if (pTextServices != NULL && pTextHost != NULL)
	//{
	//	bRet = RichEdit_InsertFace(pTextServices, pTextHost,
	//		lpszFileName, nFaceId, nFaceIndex, RGB(255, 255, 255), TRUE, 40);
	//}

	//if (pTextServices != NULL)
	//	pTextServices->Release();
	//if (pTextHost != NULL)
	//	pTextHost->Release();

	//return bRet;


	return 0;
}


void CMainFrame::_RichEdit_SetNotify(CRichEditUI2* pRichEdit, HWND hWnd)
{
	IRichEditOleCallback2* pRichEditOleCallback2 = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
#if 0
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(hWnd);
		ITextServices * pTextServices =  pRichEdit->m_p;
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
//		pRichEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}
#endif
}


