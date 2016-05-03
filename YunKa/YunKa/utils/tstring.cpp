#include "../stdafx.h"
#include "tstring.h"
#include "code_convert.h"

static CCodeConvert f_covet;

wstring convertstring(const string& s)
{
	wstring sret;
	if (!s.empty())
	{
		wchar_t *pout = new wchar_t[s.length() + 1];
		int num = f_covet.utf8ToUcs2(s.c_str(), s.length(), pout, s.length() + 1);

		sret.assign(pout, num);
		delete[] pout;
		pout = NULL;
	}

	return sret;
}