#ifndef _GLOBAL_SETTING_DEFINE_
#define _GLOBAL_SETTING_DEFINE_
#pragma once

#define  _DEBUG_DATA_

//消息定义
#define WM_INIT_ROOM_USER_LIST_MSG                                                WM_USER+100
#define WM_GET_MYSELF_INFO_MSG                                                    WM_USER+101
#define WM_UPDATE_SELECT_USER_MSG                                                 WM_USER+102

/*login define****************************************************************************************************************************************************************/ 
#define  DEBUG_USERNAME_TEXT                                                         _T("ceshi110")         
#define  DEBUG_PASSWORD_TEXT                                                         _T("123123")
#define  DEBUG_HOST_ADDRESS                                                          "http://api.51weibo.com"
#define  _GLOBAL_LOGIN_UI_CONFIG_FILE                                                _T("login\\login.xml")


//menu
#define WM_MY_MESSAGE_NOTIFYICON													WM_USER+1
#define  IDM_MENU_QUIT																WM_USER+10
#define  IDM_MENU_RELOGIN															WM_USER+11

//user click menu
#define  MENU_CLICK_USER_TO_SPEAK                                                  WM_USER+20
#define  MENU_FIND_ONE_USER_INFO                                                   WM_USER+21

//login wnd control
#define     DEF_CANCEL_WND_BUTTON													_T("cancelBtn")
#define     DEF_LOGIN_WND_BUTTON													_T("loginBtn")
#define     DEF_ACCOUNT_ID_COMBO													_T("accountcombo")
#define     DEF_ACCOUNT_ID_EDIT														_T("accountedit")
#define     DEF_PASSWORD_TEXT_EDIT													_T("pwdedit")

#define     DEF_CLOSE_WND_BUTTON													_T("closebtn")
#define     DEF_CLOSE_WND_BUTTON_2													_T("closebtn2")
#define     DEF_MIN_WND_BUTTON														_T("minbtn")
#define     DEF_RESTORE_WND_BUTTON													_T("restorebtn")
#define     DEF_MAX_WND_BUTTON														_T("maxbtn")






/*end login define***************************************************************************************************************************************************************/







#endif