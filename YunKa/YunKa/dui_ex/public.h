#pragma once

#include <atlstr.h>
#include <string>
#include <vector>

using namespace std;

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

typedef CString String;

namespace Pub

{
	template <class T>
	class CThreadHelper
	{
	public:
		static HANDLE BeginThread(T  *thd, BOOL bCreateSuspend = FALSE)
		{

			HANDLE   hth1;
			unsigned  uiThread1ID;


			hth1 = (HANDLE)_beginthreadex(NULL,         // security  
				0,            // stack size  
				T::ThreadStaticEntryPoint,
				thd,           // arg list  
				CREATE_SUSPENDED,  // so we can later call ResumeThread()  
				&uiThread1ID);


			//if (hth1 == 0)
			//printf("Failed to create thread 1\n");

			DWORD   dwExitCode;
			GetExitCodeThread(hth1, &dwExitCode);  // should be STILL_ACTIVE = 0x00000103 = 259  
			//printf("initial thread 1 exit code = %u\n", dwExitCode);

			if (!bCreateSuspend)
				ResumeThread(hth1);

			return hth1;

			if (!bCreateSuspend)
				CloseHandle(hth1);
		}
	};

	////////////////////////SYSTEM////////////////////////////////////////////////
	BOOL EnumSysFont(std::vector<tstring> * arrSysFont);	// 枚举系统字体
	BOOL FlashWindowEx(HWND hWnd, int nCount);				// 闪烁窗口标题栏
	BOOL GetTrayWndRect(RECT * lpRect);						// 获取系统任务栏区域
	/////////////////////////////////////////////////////////////////////////////////


	////////////////////////PATH or FILE///////////////////////////////////////////////////
	String GetAppDir();
	tstring GetAppDir_();	
	tstring GetDirectoryName(LPCTSTR lpszPath);	// 返回指定路径字符串的目录信息

	String GetAppPath();
	String GetPathFileName(const String &strPath);

	DWORD GetFileSize(const String &filepath);
	BOOL CreateFolder(const String &strPath);
	BOOL FolderExist(const String &strPath);
	tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);	// 返回不具有扩展名的路径字符串的文件名
	BOOL CreateFile(const String &fileName, const  String &content, BOOL canBeEmptyFile);
	//BOOL CreateFile2(const String &fileName, const  String &content, BOOL canBeEmptyFile);
	/////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////TIME////////////////////////////////////////
	const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);
	wstring MillonSecondsSince1970_To_DateTimeStr(INT64 n);
	/////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////CONVERT/////////////////////////////////
	String newGUID();

	string IntToStdStr(const int i);
	int StdStrToInt(const string &s);

	HGLOBAL GlobalAllocByString(const CHAR * lpszText, long nLen = -1);
	HGLOBAL GlobalAllocByString(const WCHAR * lpszText, long nLen = -1);

	BOOL IsDigit(const WCHAR * lpStr);
	void Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr);
	WCHAR ToLower(WCHAR c);
	void ToLower(WCHAR * lpText);
	void ToLower(std::wstring& strText);
	WCHAR ToUpper(WCHAR c);
	void ToUpper(WCHAR * lpText);
	void ToUpper(std::wstring& strText);
	void EncodeHtmlSpecialChars(std::wstring& strText);		// 编码Html特殊字符
	void DecodeHtmlSpecialChars(std::wstring& strText);		// 解码Html特殊字符

	tstring GetBetweenString(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd);
	int GetBetweenInt(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd, int nDefValue = 0);

	tstring GetBetweenString(const WCHAR * pStr, const WCHAR * pStart, const WCHAR * pEnd);
	int GetBetweenInt(const WCHAR * pStr, const WCHAR * pStart,
		const WCHAR * pEnd, int nDefValue = 0);

	BOOL DllRegisterServer(LPCTSTR lpszFileName);
	BOOL DllUnregisterServer(LPCTSTR lpszFileName);

	string StdWStrToStdStr(const wstring& ws);
	BOOL StdWStrToStdStr(const std::wstring &wstr, std::string &str);
	std::string StdWStrToStdStr2(const std::wstring &wstr);

	BOOL StdStrToStdWStr(const std::string &str, std::wstring &wstr);
	std::wstring StdStrToStdWStr(const std::string &str);
	
	char* StdWStrToChar(const wstring& ws);

	void SplitStdString(const string &s, char splitchar, vector<string>& vec);

	CHAR * UnicodeToUtf8(const WCHAR * lpszStr);
	WCHAR * AnsiToUnicode(const CHAR * lpszStr);
	WCHAR * Utf8ToUnicode(const CHAR * lpszStr);
	CHAR * UnicodeToAnsi(const WCHAR * lpszStr);

	string  UTF8ToAnsi(const string &s);
	string AnsiToUTF8(const string &s);

	void StdStrTrim(string &s);
	void StdWStrTrim(wstring &str);
}