#include "../stdafx.h"
#include "global_setting.h"


CGlobalSetting::CGlobalSetting()
{
	memset(m_passWord, 0, sizeof(m_passWord));
	memset(m_userName, 0, sizeof(m_userName));
}


CGlobalSetting::~CGlobalSetting()
{
}
