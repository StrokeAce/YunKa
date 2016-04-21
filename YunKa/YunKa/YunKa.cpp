// MDuiTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "YunKa.h"
#include "login_wnd.h"
#include "main_frame.h"



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("SkinRes"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

#if 1
	CLoginWnd* pLoginFrame = new CLoginWnd();
#else


#endif

	pLoginFrame->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginFrame->CenterWindow();
	int result = pLoginFrame->ShowModal();

	if (result == 1)
	{
		//CMicUpWnd * pMainFram = new CMicUpWnd();
		//if (pMainFram == NULL) { return 0; }
		//pMainFram->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		//pMainFram->CenterWindow();
		//pMainFram->ShowWindow(true);
		//pMainFram->ShowModal();




		CMainFrame *pWndFrame = new CMainFrame();
		pWndFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
		pWndFrame->CenterWindow();
		pWndFrame->ShowModal();

	}
	else
	{
		//登录失败
	}


	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}


