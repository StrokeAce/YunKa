//---------------------------------------------------------------------------

#ifndef HttpUnitH
#define HttpUnitH
#include "switchsock.h"
//#include "tstring.h"
using namespace std;

class IHttpFileEvent
{
public:
	virtual void OnProgress(int nIndex, int nTransferd, int nTotal) = 0;
	virtual void OnReturn(int nIndex, LPCTSTR lpszRetVal) = 0;
	virtual void OnError(int nIndex, int nCode) = 0;
};

void UseSwitchInHttpDownload(bool bUse = true);

//!下载一个URL并放到body中，如果有代理设置，会使用代理, 返回HTTP代码
//int HttpDownload(tstring URL,tstring &body,const tstring& post=_T(""),const tstring& AdditionHead=_T(""));
//int HttpDownload(string URL,string &body,const string& post="",const string& AdditionHead="");//gaoxiaohu
int HttpDownloadFile(string url,
				 string &body,
				 string &urlfile,
				 CString postfile,
				 HWND hWnd,
				 const string& AdditionHead = "",
				 const char* pszProxyip = NULL,
				 unsigned short proxyport =0);

int HttpDownload(string url,
				 string post,
				 string &body,
				 HWND hWnd,
				 const string& AdditionHead = "",
				 const char* pszProxyip = NULL,
				 unsigned short proxyport =0);

CString GetModuleFileDir();

// http下载文件
DWORD HttpDownloadFile(int nIndex,
					LPCTSTR lpszUrl,
					LPCTSTR lpszLocalName,
					LPCTSTR lpszHeader = NULL,
					IHttpFileEvent* pEvent = NULL);
//---------------------------------------------------------------------------
#endif
