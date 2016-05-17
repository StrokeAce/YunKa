#ifndef _HTTP_UNIT_H_
#define _HTTP_UNIT_H_
#include "switch_sock.h"

using namespace std;

class IHttpFileEvent
{
public:
	virtual void OnProgress(int nIndex, int nTransferd, int nTotal) = 0;
	virtual void OnReturn(int nIndex, char* lpszRetVal) = 0;
	virtual void OnError(int nIndex, int nCode) = 0;
};

void UseSwitchInHttpDownload(bool bUse = true);

int HttpDownload(string url,
				 string post,
				 string &body,
				 HWND hWnd,
				 const string& AdditionHead = "",
				 const char* pszProxyip = NULL,
				 unsigned short proxyport =0);

// httpÏÂÔØÎÄ¼þ
DWORD HttpDownloadFile(int nIndex,
					char* lpszUrl,
					char* lpszLocalName,
					char* lpszHeader = NULL,
					IHttpFileEvent* pEvent = NULL);
//---------------------------------------------------------------------------
#endif
