#include "../stdafx.h"
#include "main_frame.h"
#include "login_wnd.h"
#include "global_setting_define.h"
#include "ui_menu.h"

#include "IImageOle.h"
#include "ole_helper.h"

#include "chat_common\comfunc.h"
#include "utils\code_convert.h"

#include <WinUser.h>




CMainFrame::CMainFrame(CChatManager* manager) :m_manager(manager)
{

	//初始化
	m_pFontBtn = m_pFaceBtn = m_pScreenBtn = pSendMsgBtn = NULL;
	m_pSendEdit = NULL;

	pOnlineNode = pWaitForStart = pMySelfeNode = NULL;

	m_currentNumber = 0;


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


	//if (_tcscmp(pstrClass, _T("RichEdit2")) == 0)
	//	return new DuiLib2::CRichEditUI2;

	

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

		OnFaceCtrlSel(uMsg,wParam,lParam);
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

	if (msg.pSender == pUserList)
	{
		//m_upUser.push_back(pWebUser);

		//list<CUserObject* >::iterator  iter = m_upUser.begin();
		//for (; iter != m_upUser.end();iter++)
		//{
			
			//AddUserList(pUserList,*iter);

		//}
		//UserListUI::Node* tempNode = iter->second;



		AddOnlineVisitor(pUserList,NULL,-1);
	    m_PaintManager.KillTimer(pUserList);
	}

}

void CMainFrame::OnCloseBtn(TNotifyUI& msg)
{

	m_frameSmallMenu.DeleteSmallIcon();
	Close();
	PostQuitMessage(0);


}

void CMainFrame::OnMaxBtn(TNotifyUI& msg)
{
	int leftWidth = 0;
	RECT rc;
	RECT sysRect;
	CDuiString formatString;

	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	CControlUI *leftLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("VerticalLayoutUI_LeftFrame")));
	CControlUI *lineLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("VerticalLayoutUI_LeftLine1")));

	leftWidth = leftLayout->GetWidth();
	leftWidth += lineLayout->GetWidth();

	GetWindowRect(this->m_hWnd,&sysRect);
	//SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	int centerWidth = (sysRect.right - leftWidth) /2;

	CControlUI *centerLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("VerticalLayoutUI_CenterFrame")));
	CControlUI *rightLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("VerticalLayoutUI_RightFrame")));

	formatString.Format(_T("%d"), centerWidth);
	centerLayout->SetAttribute(_T("width"), formatString);
	formatString.Format(_T("%d"), sysRect.right - leftWidth - centerWidth);
	rightLayout->SetAttribute(_T("width"), formatString);



	CControlUI *ShowMsgWnd = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HorizontalLayoutUI_ShowMsg")));


	rc = ShowMsgWnd->GetPos();




	m_pListMsgHandler.handler->MoveBrowser( rc);




}

void CMainFrame::OnRestoreBtn(TNotifyUI& msg)
{

	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

}

void CMainFrame::OnMinBtn(TNotifyUI& msg)
{

	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	//this->ShowWindow(FALSE);
	//SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);

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
    pUserList = static_cast<UserListUI*>(m_PaintManager.FindControl(_T("userlist")));
	//UserListUI::Node* pTalkListLable = NULL;
   //(_T("{x 4}{i gameicons.png 18 3}{x 4}对话列表"))
	UserListUI::Node* pWaitForAccept = NULL;
	//pTalkListLable = pUserList->AddNode(_T("{x 4}{x 4}对话列表"));

	pWaitForStart = pUserList->AddNode(_T("{x 4}{i gameicons.png 18 0}{x 4}等待开始"));
	pWaitForAccept = pUserList->AddNode(_T("{x 4}{i gameicons.png 18 16}{x 4}等待应答"),pWaitForStart);


#if 0	
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
#endif



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
		//这里需要根据每个控件的位置  计算起始大小
		//RECT rc = { 308, 144, 308 + 379, 399 + 144 };

		//m_pListMsgHandler.handler->CreateBrowser(this->m_hWnd, rc, utfUrl, Handler_ListMsg);
		CControlUI *ShowMsgWnd = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HorizontalLayoutUI_ShowMsg")));

		RECT rect,padRect;
		rect = ShowMsgWnd->GetPos();
		padRect = ShowMsgWnd->GetPadding();
		int width = ShowMsgWnd->GetWidth();
		int height = ShowMsgWnd->GetHeight();


		m_pListMsgHandler.handler->CreateBrowser(this->m_hWnd, rect, "www.baidu.com", Handler_ListMsg);

	//	m_pListMsgHandler.handler->GetBrowser()->GetMainFrame()->LoadURL("www.baidu.com");
	}

	m_pSendEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richSend")));
	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

	IRichEditOleCallback2* pRichEditOleCallback2 = NULL;
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


	//请求坐席列表
	SendMsgToGetList();

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
		OnCloseBtn(msg);
	}
	else  if (msg.pSender->GetName() == DEF_MIN_WND_BUTTON)
	{
		OnMinBtn(msg);
	}
	else  if (msg.pSender->GetName() == DEF_RESTORE_WND_BUTTON)
	{

		OnRestoreBtn(msg);
	}
	else  if (msg.pSender->GetName() == DEF_MAX_WND_BUTTON)
	{
		OnMaxBtn(msg);
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
void CMainFrame::OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nFaceId = m_faceSelDlg.GetSelFaceId();
	int nFaceIndex = m_faceSelDlg.GetSelFaceIndex();
	tstring strFileName = m_faceSelDlg.GetSelFaceFileName();
	if (!strFileName.empty())
	{
		_RichEdit_InsertFace(m_pSendEdit, strFileName.c_str(), nFaceId, nFaceIndex);
		m_pSendEdit->SetFocus();
	}
	((COptionUI *)m_pFaceBtn)->Selected(false);

}



BOOL CMainFrame::_RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex)
{
	BOOL bRet = FALSE;

	if (NULL == pRichEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pRichEdit->GetTextServices();
	ITextHost * pTextHost = pRichEdit->GetTextHost();
	if (pTextServices != NULL && pTextHost != NULL)
	{
		//if (pRichEdit == m_pRecvEdit)
			//RichEdit_SetStartIndent(pTextServices, 300);
		bRet = RichEdit_InsertFace(pTextServices, pTextHost,
			lpszFileName, nFaceId, nFaceIndex, RGB(255, 255, 255), TRUE, 40);
	}

	if (pTextServices != NULL)
		pTextServices->Release();
	if (pTextHost != NULL)
		pTextHost->Release();

	return bRet;
}


void CMainFrame::OnBtnSendMessage(TNotifyUI& msg)
{

	ITextServices * pTextServices = m_pSendEdit->GetTextServices();

	tstring strText;
	RichEdit_GetText(pTextServices, strText);

	pTextServices->Release();

	if (strText.size() <= 0)
		return;

	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

}


//请求坐席列表
void CMainFrame::SendMsgToGetList()
{

	m_manager->SetHandlerMsgs(this);

	//先请求自己的信息 
	m_mySelfInfo = m_manager->GetMySelfUserInfo();


	AddMyselfToList(pUserList, m_mySelfInfo);



	//获取坐席列表
	m_manager->SendTo_GetShareList();


	m_hMainWnd = m_PaintManager.GetPaintWindow();


	m_PaintManager.SetTimer(pUserList, WM_ADD_ONLINE_DATA_TIMER_ID, DELAY_ADD_ONLINE_DATA_TIME);

}


void CMainFrame::AddOnlineVisitor(UserListUI * ptr, CUserObject *user,int index)
{
	CDuiString nameString, taklString, changeString, acceptString, inTalkString;
	CDuiString onlineString;
	CDuiString strTemp;
	int start = 0;
	//UserListUI::Node* pOnlineNode = NULL;
	UserListUI::Node* pAutoAccept = NULL;
	UserListUI::Node* pVisiting = NULL;
	UserListUI::Node* pOver = NULL;



	nameString.Format(_T("{x 4}{i gameicons.png 18 0}{x 4} 在线访客"));
	//第一个主节点 显示 名称 在线访客
	if (index == -1)
	{
		//pOnlineNode = ptr->AddNode(nameString, m_currentNumber);
		pOnlineNode = ptr->AddNode(nameString);
	}

	else
		pOnlineNode = ptr->AddNode(nameString, index);

	nameString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}自动邀请"));
	pAutoAccept = pUserList->AddNode(nameString, pOnlineNode);

	nameString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}访问中"));
	pVisiting = pUserList->AddNode(nameString, pOnlineNode);


	nameString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}已结束"));
	pOver = pUserList->AddNode(nameString, pOnlineNode);

}

void CMainFrame::AddUserList(UserListUI * ptr, CUserObject *user, int pos)
{
	CDuiString nameString, taklString, changeString, acceptString, inTalkString;
	CDuiString onlineString;
	CDuiString strTemp;

	UserListUI::Node* pUserNameNode = NULL;
	UserListUI::Node* pUserTalkNode = NULL;
	UserListUI::Node* pUserChangeNode = NULL;
	UserListUI::Node* pUserAcceptNode = NULL;
	UserListUI::Node* pUserInTalkNode = NULL;

	if (user->status == STATUS_OFFLINE) //离线
	{
		onlineString = _T("(离线)");
	}
	else  if (user->status == STATUS_ONLINE)                 //在线
	{
		onlineString = _T("(在线)");
	}

	strTemp = AnsiToUnicode(user->UserInfo.nickname);

	//{x 4}{i gameicons.png 18 3}{x 4} {x 4}{i gameicons.png 18 2}{x 4}
	//nameString.Format(_T("{x 20}{i res\\headimage\\default.png 1 0}{x 20} %s %s"), strTemp.GetData(), onlineString);

	nameString.Format(_T("{x 4}{i gameicons.png 18 0}{x 4} %s %s"), strTemp.GetData(), onlineString);

	//第一个主节点 显示 名称 在线状态  //pos  需要插入的位置
	pUserNameNode = ptr->AddNode(nameString,pos);

	taklString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}对话中"));
	pUserTalkNode = pUserList->AddNode(taklString, pUserNameNode);

	changeString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}转接中"));
	pUserChangeNode = pUserList->AddNode(changeString, pUserNameNode);

	acceptString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}邀请中"));
	pUserAcceptNode = pUserList->AddNode(acceptString, pUserNameNode);

	if (user->status == STATUS_OFFLINE) //离线
	{
		m_offlineNodeMap.insert(pair<unsigned int, UserListUI::Node*>(user->UserInfo.uid, pUserNameNode));

		//m_currentNumber = pUserList->GetNodeIndex(pUserNameNode);
	}
	else  if (user->status == STATUS_ONLINE)     //在线
	{
		m_onlineNodeMap.insert(pair<unsigned int, UserListUI::Node*>(user->UserInfo.uid, pUserNameNode));

	}

	pUserList->ExpandNode(pUserNameNode, false);
	
		

}



void CMainFrame::AddMyselfToList(UserListUI * ptr, CUserObject *user)
{
	CDuiString nameString, taklString, changeString, acceptString, inTalkString;
	CDuiString onlineString;
	CDuiString strTemp;

	UserListUI::Node* pUserNameNode = NULL;
	UserListUI::Node* pUserTalkNode = NULL;
	UserListUI::Node* pUserChangeNode = NULL;
	UserListUI::Node* pUserAcceptNode = NULL;
	UserListUI::Node* pUserInTalkNode = NULL;

	if (user->status == STATUS_OFFLINE)  //离线
	{
		onlineString = OFFLINE_TEXT;
	}
	else  if (user->status == STATUS_ONLINE)                   //在线
	{
		onlineString = ONLINE_TEXT;
	}

	strTemp = AnsiToUnicode(user->UserInfo.nickname);

	nameString.Format(_T("{x 4}{i gameicons.png 18 0}{x 4} %s %s"), strTemp.GetData(), onlineString);

	//第一个主节点 显示 名称 在线状态
	pUserNameNode = ptr->AddNode(nameString);

	taklString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}对话中"));
	pUserTalkNode = pUserList->AddNode(taklString, pUserNameNode);

	changeString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}转接中"));
	pUserChangeNode = pUserList->AddNode(changeString, pUserNameNode);


	acceptString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}邀请中"));
	pUserAcceptNode = pUserList->AddNode(acceptString, pUserNameNode);

	inTalkString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}内部对话"));
	pUserInTalkNode = pUserList->AddNode(inTalkString, pUserNameNode);


	pMySelfeNode = pUserNameNode;
	pUserList->ExpandNode(pMySelfeNode, false);

}



void CMainFrame::AddUserList(UserListUI * ptr, CUserObject *user)
{
	CDuiString nameString,taklString,changeString,acceptString,inTalkString;
	CDuiString onlineString;
	CDuiString strTemp;
	int index = 0;

	UserListUI::Node* pUserNameNode = NULL;
	UserListUI::Node* pUserTalkNode = NULL;
	UserListUI::Node* pUserChangeNode = NULL;
	UserListUI::Node* pUserAcceptNode = NULL;
	UserListUI::Node* pUserInTalkNode = NULL;

	if (user->status == STATUS_OFFLINE)  //离线
	{
		onlineString = _T("(离线)");

		strTemp = AnsiToUnicode(user->UserInfo.nickname);
		nameString.Format(_T("{x 4}{i gameicons.png 18 0}{x 4} %s %s"), strTemp.GetData(), onlineString);

		//第一个主节点 显示 名称 在线状态
		pUserNameNode = ptr->AddNode(nameString);
	}
	else  if (user->status == STATUS_ONLINE)                   //在线
	{
		onlineString = _T("(在线)");

		strTemp = AnsiToUnicode(user->UserInfo.nickname);
		nameString.Format(_T("{x 4}{i gameicons.png 18 0}{x 4} %s %s"), strTemp.GetData(), onlineString);


		/*
		if (m_onlineNodeMap.size() > 0)
		{
			map<unsigned int, UserListUI::Node*>::iterator iter = m_onlineNodeMap.end();
			UserListUI::Node* mapNode = iter->second;
			index = pUserList->GetNodeIndex(mapNode);
		}
		else
		{
			index = pUserList->GetNodeIndex(pMySelfeNode);
		}
		pUserNameNode = ptr->AddNode(nameString,index);
		*/
		pUserNameNode = ptr->AddNode(nameString);

	}
	taklString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}对话中"));
	pUserTalkNode = pUserList->AddNode(taklString, pUserNameNode);

	changeString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}转接中"));
	pUserChangeNode = pUserList->AddNode(changeString, pUserNameNode);


	acceptString.Format(_T("{x 4}{i gameicons.png 18 10}{x 4}邀请中"));
	pUserAcceptNode = pUserList->AddNode(acceptString, pUserNameNode);


	if (user->status == STATUS_OFFLINE)    //离线
	{
		m_offlineNodeMap.insert(pair<unsigned int, UserListUI::Node*>(user->UserInfo.uid, pUserNameNode));

		m_currentNumber = pUserList->GetNodeIndex(pUserNameNode);
	}
	else  if (user->status == STATUS_ONLINE) //在线
	{
		m_onlineNodeMap.insert(pair<unsigned int, UserListUI::Node*>(user->UserInfo.uid, pUserNameNode));
		
	}

	pUserList->ExpandNode(pUserNameNode, false);

}


//回调过来的 坐席信息
void CMainFrame::RecvOneUserInfo(CUserObject* pWebUser)
{

	//m_PaintManager.KillTimer(pUserList);
	AddUserList(pUserList,pWebUser);
	//m_PaintManager.SetTimer(pUserList, WM_ADD_ONLINE_DATA_TIMER_ID, DELAY_ADD_ONLINE_DATA_TIME);

	//m_upUser.push_back(pWebUser);

}


// 收到更新用户的在线状态
void CMainFrame::RecvUserStatus(CUserObject* pUser)
{

}

// 坐席上线消息
void CMainFrame::RecvOnline(CUserObject* pUser)
{
	int index = 0;
	if (pUser->status == STATUS_ONLINE)  //当前坐席是离线状态 同时过来的状态是上线状态
	{
		//先删除当前的离线坐席 list 再添加上线的坐席状态

		map<unsigned int, UserListUI::Node*>::iterator  iter = m_offlineNodeMap.find(pUser->UserInfo.uid);
		UserListUI::Node* tempNode = iter->second;

		//先删除
		pUserList->RemoveNode(tempNode);
		m_offlineNodeMap.erase(iter);


	
		//index += pUserList->GetNodeIndex(pWaitForStart);

		if (m_onlineNodeMap.size() > 0)
		{
			iter = m_onlineNodeMap.end();
			UserListUI::Node* mapNode = iter->second;
			index = pUserList->GetNodeIndex(mapNode);
		}
		else
			index = pUserList->GetNodeIndex(pMySelfeNode);

		//再添加
		AddUserList(pUserList, pUser, index);

	}



}

// 坐席下线消息
void CMainFrame::RecvOffline(CUserObject* pUser)
{
	int index = 0;
	if (pUser->status == STATUS_OFFLINE)  //当前坐席是在线状态 同时过来的状态是离线状态
	{
		//先删除当前的在线 坐席 list   再添加离线的坐席状态

		map<unsigned int, UserListUI::Node*>::iterator  iter = m_onlineNodeMap.find(pUser->UserInfo.uid);
		UserListUI::Node* tempNode = iter->second;

		//先删除
		pUserList->RemoveNode(tempNode);
		m_onlineNodeMap.erase(iter);


		//index = pUserList->GetNodeIndex(pMySelfeNode);// +pUserList->GetNodeIndex(pWaitForStart);

		if (m_onlineNodeMap.size() > 0)
		{
			iter = m_onlineNodeMap.end();

			UserListUI::Node* mapNode = iter->second;
			index = pUserList->GetNodeIndex(mapNode);
		}
		else
			index = pUserList->GetNodeIndex(pMySelfeNode);


		//再添加
		AddUserList(pUserList, pUser, index);
	}


}





