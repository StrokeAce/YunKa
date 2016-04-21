#include "../stdafx.h"
#include "small_menu.h"
#include <WinUser.h>
#include <io.h>
#include "ui_menu.h"



CSmallMenu::CSmallMenu()
{

}

CSmallMenu::~CSmallMenu()
{

}

void CSmallMenu::Init()
{

	//hMenu = CreatePopupMenu();

	//AppendMenu(hMenu, MF_STRING, IDM_MENU_QUIT, _T("退出"));
	//AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	//AppendMenu(hMenu, MF_STRING, IDM_MENU_RELOGIN, _T("重新登录"));

}


void CSmallMenu::CreateSmallIcon(HWND hWnd, WCHAR *path)
{

	m_hMenuWnd = hWnd;
	HICON hIconSmall = (HICON)::LoadImage(GetModuleHandle(NULL),
		path,
		IMAGE_ICON,
		48,//::GetSystemMetrics(SM_CXSMICON),
		48,//::GetSystemMetrics(SM_CYSMICON),
		LR_LOADFROMFILE);

	nid.cbSize = sizeof(NOTIFYICONDATA);//定义structure大小
	nid.hWnd = m_hMenuWnd;                  //当前程序窗口句柄，可以在CreateWindowEx的返回值获得
	nid.uID = NULL;                     //再有多个ico文件时使用，这里设成NULL(没有试过，以后可以尝试一下）
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; //选择那几个参数被使用
	nid.uCallbackMessage = WM_MY_MESSAGE_NOTIFYICON;     //用户自定义事件 #define WM_NOTIFYICON WM_USER+1(可以是任何整数）
	nid.hIcon = hIconSmall;                 //托盘图标，可以是和window图标相同，也可以用LoadIcon来定义
	lstrcpyn(nid.szTip, _T("界面测试程序"), sizeof(nid.szTip));
	//szTip是char[64] 不能直接和 string划等号，所以要借助strcpy函数来赋值
	//设置好DOTIFYICONDATA之后，就可以调用Shell_NotifyIcon函数了 其中 dwMessage 是有固定的几个选择，要添加托盘图标我们用NIM_ADD，第二个参数是一个指向NOTIFYICONDATA的结构指针
	Shell_NotifyIcon(NIM_ADD, &nid);


}

void CSmallMenu::DeleteSmallIcon()
{

	Shell_NotifyIcon(NIM_DELETE, &nid);

}



void CSmallMenu::CreateMyAppMenu(POINT point)
{

	//TrackPopupMenu(hMenu, TPM_LEFTALIGN, point.x, point.y - 5, 0, m_hMenuWnd, NULL);


	CMenuWnd* pMenu = new CMenuWnd(m_hMenuWnd);
	CPoint cpoint = point;

	cpoint.y -= 65;
	//ClientToScreen(m_hMenuWnd, &cpoint);
	pMenu->Init(NULL, _T(""), _T("xml"), cpoint);

}

LRESULT CSmallMenu::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int id = wParam;
	int uMouseMsg = lParam;

	if (uMsg == WM_MY_MESSAGE_NOTIFYICON)
	{
		//如果是单击左键，则切换窗口显示状态
		if (uMouseMsg == WM_LBUTTONDOWN)
		{
			printf("切换窗口状态");
		}

		// 如果是单击右键，则显示菜单
		else if (uMouseMsg == WM_RBUTTONDOWN)
		{
			// 显示响应菜单
			printf("显示弹出菜单");
		}
		//如果是左键双击,显示主窗体
		else if (uMouseMsg == WM_LBUTTONDBLCLK)
		{
			printf("显示主窗口");
			SendMessage(m_hMenuWnd, WM_ACTIVATE, SC_RESTORE, 0);
			ShowWindow(m_hMenuWnd, SW_SHOW);
			return 0L;
		}
		//右键弹起时，显示菜单
		else if (uMouseMsg == WM_RBUTTONUP)
		{
			POINT lpoint;
			GetCursorPos(&lpoint);//得到鼠标位置
			CreateMyAppMenu(lpoint);
		}

	}
	else if (uMsg == WM_COMMAND)
	{

		switch (LOWORD(wParam))
		{
		case IDM_MENU_QUIT:

			PostQuitMessage(0);
			break;

		case IDM_MENU_RELOGIN:

			break;

		default:
			break;
		}
	}

	return 0L;
}