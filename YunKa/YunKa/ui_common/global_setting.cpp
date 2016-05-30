#include "../stdafx.h"
#include "global_setting.h"
#include "common_utility.h"
#include <Shlwapi.h>


CGlobalSetting::CGlobalSetting()
{
	memset(m_passWord, 0, sizeof(m_passWord));
	memset(m_userName, 0, sizeof(m_userName));


}


CGlobalSetting::~CGlobalSetting()
{
}

//查看指定文件是否存在
int CGlobalSetting::FindFileExist(char *str)
{
	WCHAR path[1024] = { 0 };

	ANSIToUnicode(str, path);
	return PathFileExists(path);
}

int CGlobalSetting::FindFileExist(WCHAR *str)
{

	return PathFileExists(str);
}
