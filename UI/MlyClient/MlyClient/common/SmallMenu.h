/********************************************************************
*类名：全局配置信息类
*
*
*
*
*
*********************************************************************/
#ifndef _SMALL_MENU_
#define _SMALL_MENU_

#pragma once

#include <shellapi.h>

class CSmallMenu
{
public:
	CSmallMenu();
	~CSmallMenu();



public:
	HMENU hMenu;
	NOTIFYICONDATA nid;
	void Init();
	void CreateSmallIcon(HWND hWnd, WCHAR *path);
	void DeleteSmallIcon();
	void CreateMyAppMenu(POINT point);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hMenuWnd;




};




#endif
