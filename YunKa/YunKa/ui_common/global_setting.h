#ifndef _GLOBAL_SETTING_H_
#define _GLOBAL_SETTING_H_



#pragma once

#include "global_setting_define.h"


class CGlobalSetting
{
public:
	CGlobalSetting();
	~CGlobalSetting();

public:




	//用户名和密码
	char m_userName[32];
	char m_passWord[32];

};




#endif