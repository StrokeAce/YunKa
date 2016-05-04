#include "../StdAfx.h"
#include "comdef.h" //bstr必须
#include<time.h>
#include <fstream>
#include "Public.h"
#include "TCHAR.H"
//#include <algorithm>
//#include <functional>

#define MAX_SIZE 1024 

using namespace std;

namespace Pub
{
	String GetAppPath()
	{
		TCHAR   AppPath[MAX_PATH];
		String   strDir, strTmpDir;

		memset(AppPath, 0, MAX_PATH);

		GetModuleFileName(NULL, AppPath, MAX_PATH);
		strDir = AppPath;
		//strDir = strTmpDir.Left(strTmpDir.ReverseFind('\\'));

		return   strDir;
	}

	// 返回指定路径字符串的目录信息
	tstring GetDirectoryName(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);

		/*tstring::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
		if (_T('\\') == *iter)
		*iter = _T('/');
		}*/

		tstring::size_type nPos = strPath.rfind(_T('\\'));
		if (nPos != tstring::npos)
			strPath = strPath.substr(0, nPos + 1);

		return strPath;
	}

	String GetAppDir()
	{
		TCHAR   AppPath[MAX_PATH];
		String   strTmpDir;

		memset(AppPath, 0, MAX_PATH);

		GetModuleFileName(NULL, AppPath, MAX_PATH);
		strTmpDir = AppPath;
		strTmpDir = String(strTmpDir.Left(strTmpDir.ReverseFind('\\')) + "\\");

		return   strTmpDir;
	}

	tstring GetAppDir_()
	{
		static TCHAR szPath[MAX_PATH] = { 0 };

		if (szPath[0] == _T('\0'))
		{
			::GetModuleFileName(NULL, szPath, MAX_PATH);

			tstring strPath = GetDirectoryName(szPath);
			wcsncpy_s(szPath, strPath.c_str(), MAX_PATH);
		}

		return szPath;
	}

	String GetPathFileName(const String &strPath)
	{
		String strName;

		strName = strPath;
		strName = strName.Right(strName.GetLength() - strName.ReverseFind('\\') - 1);
		//strName =  strName.Left(strName.ReverseFind('.') );   

		return strName;
	}

	string IntToStdStr(const int i)
	{
		char ch[MAX_PATH];

		_itoa(i, ch, 10);

		string s(ch, strlen(ch));

		return s;
	}

	int StdStrToInt(const string &s)
	{
		return _atoi64(s.c_str());
	}

	String newGUID()
	{
		String strGuid;
		GUID guid;
		if (S_OK == ::CoCreateGuid(&guid))
		{
			strGuid.Format(_T("%04x%04x%04x%04x%04x%04x%04x%04x"),
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
		}
		return strGuid;
	}

	//目录是否存在检查：

	BOOL FolderExist(const String &strPath)
	{
		WIN32_FIND_DATA wfd;
		BOOL rValue = FALSE;
		HANDLE hFind = FindFirstFile(strPath, &wfd);
		if ((hFind != INVALID_HANDLE_VALUE) &&
			(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			rValue = TRUE;
		}
		FindClose(hFind);
		return rValue;
	}


	//创建目录：
	BOOL CreateFolder(const String &strPath)
	{
		SECURITY_ATTRIBUTES attrib;
		attrib.bInheritHandle = FALSE;
		attrib.lpSecurityDescriptor = NULL;
		attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		//上面定义的属性可以省略
		//直接使用return ::CreateDirectory(path, NULL);即可
		return ::CreateDirectory(strPath, &attrib);
	}

	//文件大小：
	DWORD GetFileSize(const String &filepath)
	{
		WIN32_FIND_DATA fileInfo;
		HANDLE hFind;
		DWORD fileSize;
		String filename;
		filename = filepath;
		hFind = FindFirstFile(filename, &fileInfo);
		if (hFind != INVALID_HANDLE_VALUE)
			fileSize = fileInfo.nFileSizeLow;

		FindClose(hFind);
		return fileSize;
	}

	BOOL CreateFile(const String &fileName, const String &content, BOOL canBeEmptyFile)
	{
		if (content.GetLength() > 0 || canBeEmptyFile)
		{
			ofstream outFile;
			outFile.open(fileName, ios::out);
			wstring ws = (LPWSTR)_bstr_t(content);		//CStringz转Wstring
			const char *ch = StdWStrToChar(ws);
			outFile.write(ch,  content.GetLength());
			outFile.close();

			return TRUE;
		}
		return FALSE;
	}


	tstring GetFileNameWithoutExtension(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);

		tstring::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		tstring::size_type nPos = strPath.rfind(_T('/'));
		if (nPos != tstring::npos)
			strPath = strPath.substr(nPos + 1);

		nPos = strPath.rfind(_T('.'));
		if (nPos != tstring::npos)
			strPath = strPath.substr(0, nPos);

		return strPath;
	}

	WCHAR * AnsiToUnicode(const CHAR * lpszStr)
	{
		WCHAR * lpUnicode;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszStr, -1, NULL, 0);
		if (0 == nLen)
			return NULL;

		lpUnicode = new WCHAR[nLen + 1];
		if (NULL == lpUnicode)
			return NULL;

		memset(lpUnicode, 0, sizeof(WCHAR) * (nLen + 1));
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszStr, -1, lpUnicode, nLen);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		return lpUnicode;
	}

	WCHAR * Utf8ToUnicode(const CHAR * lpszStr)
	{
		WCHAR * lpUnicode;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::MultiByteToWideChar(CP_UTF8, 0, lpszStr, -1, NULL, 0);
		if (0 == nLen)
			return NULL;

		lpUnicode = new WCHAR[nLen + 1];
		if (NULL == lpUnicode)
			return NULL;

		memset(lpUnicode, 0, sizeof(WCHAR) * (nLen + 1));
		nLen = ::MultiByteToWideChar(CP_UTF8, 0, lpszStr, -1, lpUnicode, nLen);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		return lpUnicode;
	}

	CHAR * UnicodeToAnsi(const WCHAR * lpszStr)
	{
		CHAR * lpAnsi;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::WideCharToMultiByte(CP_ACP, 0, lpszStr, -1, NULL, 0, NULL, NULL);
		if (0 == nLen)
			return NULL;

		lpAnsi = new CHAR[nLen + 1];
		if (NULL == lpAnsi)
			return NULL;

		memset(lpAnsi, 0, nLen + 1);
		nLen = ::WideCharToMultiByte(CP_ACP, 0, lpszStr, -1, lpAnsi, nLen, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpAnsi;
			return NULL;
		}

		return lpAnsi;
	}

	HGLOBAL GlobalAllocByString(const CHAR * lpszText, long nLen/* = -1*/)
	{
		if (NULL == lpszText)
			return NULL;

		if (-1 == nLen)
			nLen = strlen(lpszText);

		HGLOBAL hGlobal = ::GlobalAlloc(GHND, nLen + 4);
		if (hGlobal != NULL)
		{
			CHAR * lpData = (CHAR *)::GlobalLock(hGlobal);
			if (lpData != NULL)
			{
				memset(lpData, 0, nLen + 4);
				memcpy(lpData, lpszText, nLen);
				::GlobalUnlock(hGlobal);
			}
		}
		return hGlobal;
	}

	HGLOBAL GlobalAllocByString(const WCHAR * lpszText, long nLen/* = -1*/)
	{
		if (NULL == lpszText)
			return NULL;

		if (-1 == nLen)
			nLen = wcslen(lpszText);

		return GlobalAllocByString((const CHAR *)lpszText, nLen * sizeof(TCHAR));
	}

	// 枚举系统字体回调函数
	int CALLBACK EnumSysFontProc(const LOGFONT *lpelf, const TEXTMETRIC *lpntm, DWORD dwFontType, LPARAM lParam)
	{
		if (dwFontType & TRUETYPE_FONTTYPE)
		{
			std::vector<tstring> * arrSysFont = (std::vector<tstring> *)lParam;
			if (arrSysFont != NULL)
			{
				for (int i = 0; i < (int)arrSysFont->size(); i++)
				{
					if ((*arrSysFont)[i] == lpelf->lfFaceName)
						return TRUE;
				}
				arrSysFont->push_back(lpelf->lfFaceName);
			}
		}

		return TRUE;
	}

	// 枚举系统字体
	BOOL EnumSysFont(std::vector<tstring> * arrSysFont)
	{
		if (NULL == arrSysFont)
			return FALSE;

		HDC hDC = ::GetDC(NULL);
		::EnumFontFamiliesEx(hDC, NULL, EnumSysFontProc, (LPARAM)arrSysFont, 0);
		::ReleaseDC(NULL, hDC);

		return TRUE;
	}

	// 闪烁窗口标题栏
	BOOL FlashWindowEx(HWND hWnd, int nCount)
	{
		FLASHWINFO stFlashInfo = { 0 };
		stFlashInfo.cbSize = sizeof(FLASHWINFO);
		stFlashInfo.hwnd = hWnd;
		stFlashInfo.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
		stFlashInfo.uCount = nCount;
		stFlashInfo.dwTimeout = 0;
		return ::FlashWindowEx(&stFlashInfo);
	}

	// 获取系统任务栏区域
	BOOL GetTrayWndRect(RECT * lpRect)
	{
		if (NULL == lpRect)
			return FALSE;

		HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
		if (hWnd != NULL)
			return ::GetWindowRect(hWnd, lpRect);
		else
			return FALSE;
	}

	// _T("%Y-%m-%d %H:%M:%S")
	const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt)
	{
		time_t timer;
		time(&timer);
		struct tm *lpTimeInfo;
		lpTimeInfo = localtime(&timer);

		static TCHAR cTime[32];

		memset(cTime, 0, sizeof(cTime));

	    localtime_s(lpTimeInfo, &lTime);
		if (lpTimeInfo != NULL)
			_tcsftime(cTime, sizeof(cTime) / sizeof(TCHAR), lpFmt, lpTimeInfo);
		return cTime;
	}

	BOOL IsDigit(const WCHAR * lpStr)
	{
		for (const WCHAR * p = lpStr; *p != _T('\0'); p++)
		{
			if (!isdigit(*p))
				return FALSE;
		}
		return TRUE;
	}

	void Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr)
	{
		if (NULL == lpOldStr || NULL == lpNewStr)
			return;

		int nOldStrLen = wcslen(lpOldStr);
		int nNewStrLen = wcslen(lpNewStr);

		std::wstring::size_type nPos = 0;
		while ((nPos = strText.find(lpOldStr, nPos)) != std::wstring::npos)
		{
			strText.replace(nPos, nOldStrLen, lpNewStr);
			nPos += nNewStrLen;
		}
	}

	inline WCHAR ToLower(WCHAR c)
	{
		return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
	}

	void ToLower(WCHAR * lpText)
	{
		for (WCHAR * p = lpText; *p != _T('\0'); p++)
			*p = ToLower(*p);
	}

	void ToLower(std::wstring& strText)
	{
		for (std::wstring::iterator i = strText.begin(); i != strText.end(); ++i)
			*i = ToLower(*i);
	}

	inline WCHAR ToUpper(WCHAR c)
	{
		return (c >= 'a' && c <= 'z') ? (c + ('A' - 'a')) : c;
	}

	void ToUpper(WCHAR * lpText)
	{
		for (WCHAR * p = lpText; *p != _T('\0'); p++)
			*p = ToUpper(*p);
	}

	void ToUpper(std::wstring& strText)
	{
		for (std::wstring::iterator i = strText.begin(); i != strText.end(); ++i)
			*i = ToUpper(*i);
	}

	// 编码Html特殊字符
	void EncodeHtmlSpecialChars(std::wstring& strText)
	{
		Replace(strText, _T("&"), _T("&amp;"));
		Replace(strText, _T("'"), _T("&#39;"));
		Replace(strText, _T("\""), _T("&quot;"));
		Replace(strText, _T("<"), _T("&lt;"));
		Replace(strText, _T(">"), _T("&gt;"));
		Replace(strText, _T(" "), _T("&nbsp;"));
	}

	// 解码Html特殊字符
	void DecodeHtmlSpecialChars(std::wstring& strText)
	{
		Replace(strText, _T("&#39;"), _T("'"));
		Replace(strText, _T("&quot;"), _T("\""));
		Replace(strText, _T("&lt;"), _T("<"));
		Replace(strText, _T("&gt;"), _T(">"));
		Replace(strText, _T("&nbsp;"), _T(" "));
		Replace(strText, _T("&amp;"), _T("&"));
	}

	tstring GetBetweenString(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd)
	{
		tstring strText;

		if (NULL == pStr)
			return _T("");

		const TCHAR * p1 = _tcschr(pStr, cStart);
		if (NULL == p1)
			return _T("");

		const TCHAR * p2 = _tcschr(p1 + 1, cEnd);
		if (NULL == p2)
			return _T("");

		int nLen = p2 - (p1 + 1);
		if (nLen <= 0)
			return _T("");

		TCHAR * lpText = new TCHAR[nLen + 1];
		if (NULL == lpText)
			return _T("");

		memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
		_tcsncpy(lpText, p1 + 1, nLen);
		strText = lpText;
		delete[]lpText;

		return strText;
	}

	int GetBetweenInt(const TCHAR * pStr, TCHAR cStart, TCHAR cEnd, int nDefValue/* = 0*/)
	{
		tstring strText = GetBetweenString(pStr, cStart, cEnd);
		if (!strText.empty() && IsDigit(strText.c_str()))
			return _tcstol(strText.c_str(), NULL, 10);
		else
			return nDefValue;
	}

	tstring GetBetweenString(const WCHAR * pStr, const WCHAR * pStart, const WCHAR * pEnd)
	{
		tstring strText;

		if (NULL == pStr || NULL == pStart || NULL == pEnd)
			return _T("");

		int nStartLen = _tcslen(pStart);

		const TCHAR * p1 = _tcsstr(pStr, pStart);
		if (NULL == p1)
			return _T("");

		const TCHAR * p2 = _tcsstr(p1 + nStartLen, pEnd);
		if (NULL == p2)
			return _T("");

		int nLen = p2 - (p1 + nStartLen);
		if (nLen <= 0)
			return _T("");

		TCHAR * lpText = new TCHAR[nLen + 1];
		if (NULL == lpText)
			return _T("");

		memset(lpText, 0, (nLen + 1)*sizeof(TCHAR));
		_tcsncpy(lpText, p1 + nStartLen, nLen);
		strText = lpText;
		delete[]lpText;

		return strText;
	}

	int GetBetweenInt(const WCHAR * pStr, const WCHAR * pStart,
		const WCHAR * pEnd, int nDefValue/* = 0*/)
	{
		tstring strText = GetBetweenString(pStr, pStart, pEnd);
		if (!strText.empty() && IsDigit(strText.c_str()))
			return _tcstol(strText.c_str(), NULL, 10);
		else
			return nDefValue;
	}

	BOOL DllRegisterServer(LPCTSTR lpszFileName, BOOL bUnregister)
	{
		typedef HRESULT(WINAPI * FREG)();

		BOOL bRet = FALSE;

		HMODULE hDLL = ::LoadLibrary(lpszFileName);
		if (NULL == hDLL)
			return FALSE;

		CHAR * lpszFuncName;
		if (!bUnregister)
			lpszFuncName = "DllRegisterServer";
		else
			lpszFuncName = "DllUnregisterServer";

		FREG lpfunc = (FREG)::GetProcAddress(hDLL, lpszFuncName);
		if (lpfunc != NULL)
		{
			lpfunc();
			bRet = TRUE;
		}

		::FreeLibrary(hDLL);

		return bRet;
	}

	BOOL DllRegisterServer(LPCTSTR lpszFileName)
	{
		return DllRegisterServer(lpszFileName, FALSE);
	}

	BOOL DllUnregisterServer(LPCTSTR lpszFileName)
	{
		return DllRegisterServer(lpszFileName, TRUE);
	}

	//GUID GetFileTypeGuidByExtension(const WCHAR * lpExtension)
	//{
	//	GUID guid = GUID_NULL;

	//	if (NULL == lpExtension)
	//		return guid;

	//	UINT numEncoders = 0, size = 0;
	//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
	//	if (status != Gdiplus::Ok)
	//		return guid;

	//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
	//	if (NULL == lpEncoders)
	//		return guid;

	//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
	//	if (Gdiplus::Ok == status)
	//	{
	//		for (UINT i = 0; i < numEncoders; i++)
	//		{
	//			BOOL bFind = FALSE;
	//			const WCHAR * pStart = lpEncoders[i].FilenameExtension;
	//			const WCHAR * pEnd = wcschr(pStart, L';');
	//			do
	//			{
	//				if (NULL == pEnd)
	//				{
	//					LPCWSTR lpExt = ::wcsrchr(pStart, L'.');
	//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
	//					{
	//						guid = lpEncoders[i].FormatID;
	//						bFind = TRUE;
	//					}
	//					break;
	//				}

	//				int nLen = pEnd - pStart;
	//				if (nLen < MAX_PATH)
	//				{
	//					WCHAR cBuf[MAX_PATH] = { 0 };
	//					wcsncpy(cBuf, pStart, nLen);
	//					LPCWSTR lpExt = ::wcsrchr(cBuf, L'.');
	//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
	//					{
	//						guid = lpEncoders[i].FormatID;
	//						bFind = TRUE;
	//						break;
	//					}
	//				}
	//				pStart = pEnd + 1;
	//				if (L'\0' == *pStart)
	//					break;
	//				pEnd = wcschr(pStart, L';');
	//			} while (1);
	//			if (bFind)
	//				break;
	//		}
	//	}

	//	free(lpEncoders);

	//	return guid;
	//}

	//CLSID GetEncoderClsidByExtension(const WCHAR * lpExtension)
	//{
	//	CLSID clsid = CLSID_NULL;

	//	if (NULL == lpExtension)
	//		return clsid;

	//	UINT numEncoders = 0, size = 0;
	//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
	//	if (status != Gdiplus::Ok)
	//		return clsid;

	//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
	//	if (NULL == lpEncoders)
	//		return clsid;

	//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
	//	if (Gdiplus::Ok == status)
	//	{
	//		for (UINT i = 0; i < numEncoders; i++)
	//		{
	//			BOOL bFind = FALSE;
	//			const WCHAR * pStart = lpEncoders[i].FilenameExtension;
	//			const WCHAR * pEnd = wcschr(pStart, L';');
	//			do
	//			{
	//				if (NULL == pEnd)
	//				{
	//					LPCWSTR lpExt = ::wcsrchr(pStart, L'.');
	//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
	//					{
	//						clsid = lpEncoders[i].Clsid;
	//						bFind = TRUE;
	//					}
	//					break;
	//				}

	//				int nLen = pEnd - pStart;
	//				if (nLen < MAX_PATH)
	//				{
	//					WCHAR cBuf[MAX_PATH] = { 0 };
	//					wcsncpy(cBuf, pStart, nLen);
	//					LPCWSTR lpExt = ::wcsrchr(cBuf, L'.');
	//					if ((lpExt != NULL) && (wcsicmp(lpExt, lpExtension) == 0))
	//					{
	//						clsid = lpEncoders[i].Clsid;
	//						bFind = TRUE;
	//						break;
	//					}
	//				}
	//				pStart = pEnd + 1;
	//				if (L'\0' == *pStart)
	//					break;
	//				pEnd = wcschr(pStart, L';');
	//			} while (1);
	//			if (bFind)
	//				break;
	//		}
	//	}

	//	free(lpEncoders);

	//	return clsid;
	//}

	//// ImageFormatBMP, ImageFormatJPEG, ImageFormatPNG, ImageFormatGIF, ImageFormatTIFF
	//CLSID GetEncoderClsidByFileType(REFGUID guidFileType)
	//{
	//	CLSID clsid = CLSID_NULL;

	//	UINT numEncoders = 0, size = 0;
	//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
	//	if (status != Gdiplus::Ok)
	//		return clsid;

	//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
	//	if (NULL == lpEncoders)
	//		return clsid;

	//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
	//	if (Gdiplus::Ok == status)
	//	{
	//		for (UINT i = 0; i < numEncoders; i++)
	//		{
	//			if (lpEncoders[i].FormatID == guidFileType)
	//				clsid = lpEncoders[i].Clsid;
	//		}
	//	}

	//	free(lpEncoders);

	//	return clsid;
	//}

	//// image/bmp, image/jpeg, image/gif, image/tiff, image/png
	//CLSID GetEncoderClsidByMimeType(const WCHAR * lpMineType)
	//{
	//	CLSID clsid = CLSID_NULL;

	//	if (NULL == lpMineType)
	//		return clsid;

	//	UINT numEncoders = 0, size = 0;
	//	Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&numEncoders, &size);
	//	if (status != Gdiplus::Ok)
	//		return clsid;

	//	Gdiplus::ImageCodecInfo* lpEncoders = (Gdiplus::ImageCodecInfo*)(malloc(size));
	//	if (NULL == lpEncoders)
	//		return clsid;

	//	status = Gdiplus::GetImageEncoders(numEncoders, size, lpEncoders);
	//	if (Gdiplus::Ok == status)
	//	{
	//		for (UINT i = 0; i < numEncoders; i++)
	//		{
	//			if (wcsicmp(lpEncoders[i].MimeType, lpMineType) == 0)
	//				clsid = lpEncoders[i].Clsid;
	//		}
	//	}

	//	free(lpEncoders);

	//	return clsid;
	//}


BOOL StdStrToStdWStr(const std::string &str, std::wstring &wstr)
	{
		int nLen = (int)str.length();
		wstr.resize(nLen, L' ');

		int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);
		StdWStrTrim(wstr); //去空格啊
		if (nResult == 0)
		{
			return FALSE;
		}

		return TRUE;
	}

	string StdWStrToStdStr(const wstring& ws)
	{
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		return result;
	}

	//只拷贝低字节至string中
	std::string StdWStrToStdStr2(const std::wstring &wstr)
	{
		std::string str(wstr.length(), ' ');
		std::copy(wstr.begin(), wstr.end(), str.begin());
		return str;
	}

	//wstring高字节不为0，返回FALSE
	BOOL StdWStrToStdStr(const std::wstring &wstr, std::string &str)
	{
		int nLen = (int)wstr.length();
		str.resize(nLen, ' ');

		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

		if (nResult == 0)
		{
			return FALSE;
		}

		return TRUE;
	}

	std::wstring StdStrToStdWStr(const std::string &str)
	{
		std::wstring wstr(str.length(), L' ');
		std::copy(str.begin(), str.end(), wstr.begin());
		return wstr;
	}

	char* StdWStrToChar(const wstring& ws)
	{
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		char* charTmp = new char;
		strcpy(charTmp, result.c_str());
		pchar = NULL;
		delete pchar;
		return charTmp;
	}

	void SplitStdString(const string &s, char splitchar, vector<string>& vec)
	{
		if (vec.size() > 0)//保证vec是空的  
			vec.clear();
		int length = s.length();
		int start = 0;
		for (int i = 0; i < length; i++)
		{
			if (s[i] == splitchar && i == 0)//第一个就遇到分割符  
			{
				start += 1;
			}
			else if (s[i] == splitchar)
			{
				vec.push_back(s.substr(start, i - start));
				start = i + 1;
			}
			else if (i == length - 1)//到达尾部  
			{
				vec.push_back(s.substr(start, i + 1 - start));
			}
		}
	}

	/*char* ANSIToUTF8(const char* str)
	{
	return UnicodeToUTF8(ANSIToUnicode(str));
	}

	char* UTF8ToANSI(const char* str)
	{
	return UnicodeToANSI(UTF8ToUnicode(str));
	}*/

	char* QXUnicode2Utf8(const wchar_t* unicode)
	{
		int len;
		len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);
		char *szUtf8 = (char*)malloc(len + 1);
		memset(szUtf8, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUtf8, len, NULL, NULL);
		return szUtf8;
	}

	CHAR * UnicodeToUtf8(const WCHAR * lpszStr)
	{
		CHAR * lpUtf8;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszStr, -1, NULL, 0, NULL, NULL);
		if (0 == nLen)
			return NULL;

		lpUtf8 = new CHAR[nLen + 1];
		
		if (NULL == lpUtf8)
			return NULL;
		
		memset(lpUtf8, 0, nLen + 1);
		nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszStr, -1, lpUtf8, nLen, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpUtf8;
			return NULL;
		}
		//lpUtf8[nLen] = '\0';
		return lpUtf8;
	}

	string AnsiToUTF8(const string &s)
	{
		WCHAR *pwc = AnsiToUnicode(s.c_str());
		char *pc = UnicodeToUtf8(pwc);
		string sRet(pc, strlen(pc));
		delete[] pwc;
		delete[] pc;
		return sRet;
	}

	string  UTF8ToAnsi(const string &s)
	{
		const WCHAR * pwc = Utf8ToUnicode(s.c_str());
		char *pc = UnicodeToAnsi(pwc);
		delete[] pwc;
		string sRet(pc, strlen(pc));
		delete[] pc;
		return sRet;
	}

	void StdStrTrim(string &s)
	{
		int n1 = s.find_first_not_of(" \t");
		int n2 = s.find_last_not_of(" \t");
		s = s.substr(n1, n2 - n1 + 1);
	}

	/*void LTrim(wstring &str)
	{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(),
	std::not1(std::ptr_fun(::isspace))));
	}

	void RTrim(wstring &str)
	{
	str.erase(std::find_if(str.rbegin(), str.rend(),
	std::not1(std::ptr_fun(::isspace))).base(),
	str.end());
	}*/

	void del_sp(char *src)    // 删除C风格字符串中的空格
	{
		char * fp = src;
		while (*src) {
			if (*src != ' ') { // 如果不是空格就复制
				*fp = *src;
				fp++;
			}
			src++;
		}
		*fp = '\0'; //封闭字符串
	}

	//void StdWStrTrim(wstring &s)
	//{ 
	//	string stmp = StdWStrToStdStr(s);
	//	char tmp[MAX_SIZE];
	//	memcpy(&tmp, stmp.c_str(), stmp.size() + 1);
	//	del_sp(tmp);    // 重载del_sp，调用C风格的函数
	//	stmp = tmp;
	//	StdStrToStdWStr(stmp, s);
	//}

	void StdWStrTrim(wstring &str)
	{
		wstring::size_type pos = str.find_last_not_of(TEXT(' '));
		if (pos != wstring::npos)
		{
			str.erase(pos + 1);
			pos = str.find_first_not_of(TEXT(' '));
			if (pos != wstring::npos)
			{
				str.erase(0, pos);
			}
		}
		else
		{
			str.erase(str.begin(), str.end());
		}
	}

	wstring MillonSecondsSince1970_To_DateTimeStr(INT64 n)
	{
		time_t t = n / 1000; //time_t表示秒
		struct tm *p;
		//time(&t); /*当前time_t类型UTC时间*/
		p = localtime(&t); /*转换为本地的tm结构的时间按*/
		t = mktime(p);
		//char ch[32];
		////sprintf_s(ch, "%d-%d-%d %d:%d:%d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
		//sprintf_s(ch, "%d/%d/%d %d:%d:%d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
		//string s(ch, strlen(ch));%Y/%m/%d :%S

		wstring strTime = FormatTime(t, _T("%H:%M"));

		return strTime;
	}


} //namespace
