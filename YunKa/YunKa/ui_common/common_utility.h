#ifndef _COMMON_UTILITY_
#define _COMMON_UTILITY_

#pragma once

#include <string>   
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h> 
#include <locale.h> 
#include <vector>



//×Ö·û´®Ìæ»» µ±Ç°×Ö·û  ½«srcStr Ìæ »»Îª destStr
extern void StringReplace(string &curStr, string srcStr, string destStr);
extern char *CU2A(const wchar_t* str, char *dest);
extern wchar_t *CA2U(const char* str, WCHAR *dest);
extern wchar_t *ANSIToUnicode(const char* str, wchar_t *dest);
extern char *UnicodeToANSI(const wchar_t* str, char *dest);
extern wchar_t *UTF8ToUnicode(const char* str, wchar_t *dest);
extern char *UnicodeToUTF8(const wchar_t* str, char *dest);
extern char *ANSIToUTF8(const char* str, char *dest);
extern char *UTF8ToANSI(const char* str, char *destStr);


extern int PrintLog(const char *fmt, ...);
extern std::string GetCurrentPath();
extern WCHAR *GetCurrentPathW();


extern void SplitStringA(char *pSrc, char chMark, std::vector<std::string> &vecStrings, BOOL bOnce = FALSE);
extern void SplitStringA(char *pSrc, char *chMark, std::vector<std::string> &vecStrings);





#endif