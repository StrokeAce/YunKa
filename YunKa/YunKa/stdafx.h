// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include "Resource.h"
#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息

// C 运行时头文件

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>


using namespace std;

#include <objbase.h>


#include "UIlib.h"


#include "global_setting.h"

extern CGlobalSetting _globalSetting;


using namespace DuiLib;

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "../lib/DuiLib.lib")
#   else
#       pragma comment(lib, "..\\..\\DuiLib\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\DuiLib\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\DuiLib\\Lib\\DuiLib.lib")
#   endif
#endif


// TODO:  在此处引用程序需要的其他头文件
