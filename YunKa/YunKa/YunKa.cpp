// MDuiTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "YunKa.h"
#include "login_wnd.h"
#include "main_frame.h"
#include "path.h"
#include "utils.h"
#include "chat_manager.h"
#include "cef_browser/cefclient.h"


CefRefPtr<ClientApp> m_cefApp;

Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken;

CGlobalSetting _globalSetting;




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	m_cefApp = new ClientApp();
	if (!(m_cefApp->Init(hInstance) < 0))
		return FALSE;


	wstring strFileName = ZYM::CPath::GetAppPath() + _T("ImageOleCtrl.dll");

	BOOL bRet = DllRegisterServer(strFileName.c_str());	// 注册COM组件
	if (!bRet)
	{
		::MessageBox(NULL, _T("COM组件注册失败，应用程序无法完成初始化操作！"), _T("提示"), MB_OK);
		return 0;
	}

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);	// 初始化GDI+
	HMODULE hRichEditDll = ::LoadLibrary(_T("Riched20.dll"));	// 加载RichEdit控件DLL

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("SkinRes"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;


	CLoginWnd* pLoginFrame = new CLoginWnd();

	CMainFrame *pWndFrame = new CMainFrame(pLoginFrame->m_manager);
	pLoginFrame->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginFrame->CenterWindow();

	int result = pLoginFrame->ShowModal();

	if (result == 1)
	{
		
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


	if (hRichEditDll != NULL)					// 卸载RichEdit控件DLL
		::FreeLibrary(hRichEditDll);

	Gdiplus::GdiplusShutdown(g_gdiplusToken);	// 反初始化GDI+
	::OleUninitialize();


	m_cefApp->Exit();
	m_cefApp = NULL;

	return 0;
}


