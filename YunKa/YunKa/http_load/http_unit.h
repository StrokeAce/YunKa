#ifndef _HTTP_UNIT_H_
#define _HTTP_UNIT_H_

#include "curl/curl.h"

using namespace std;

class IHttpFileEvent
{
public:
	virtual void OnProgress(int nIndex, int nTransferd, int nTotal) = 0;
	virtual void OnReturn(int nIndex, char* lpszRetVal) = 0;
	virtual void OnError(int nIndex, int nCode) = 0;
};

int HttpDownload(string url,
				 string post,
				 string &body,
				 HWND hWnd,
				 const string& AdditionHead = "",
				 const char* pszProxyip = NULL,
				 unsigned short proxyport =0);

// http下载文件
DWORD HttpDownloadFile(int nIndex,
					char* lpszUrl,
					char* lpszLocalName,
					char* lpszHeader = NULL,
					IHttpFileEvent* pEvent = NULL);


class CHttpLoad
{
public:
	CHttpLoad();
	~CHttpLoad();

	/**
	* http下载或上传文件
	* @param[in|out] szUrl 请求的url
	* @param[in|out] szReffer "referer"头的字符串
	* @param[in]     requestType REQUEST_TYPE_POST:下载,REQUEST_TYPE_GET:下载
	* @param[in]     szFilePath 上传或下载文件的路劲
	* @param[in|out] szContent 执行结果的返回码
	* @return true 设置成功，false 失败
	*/
	bool HttpLoad(
		const string& szUrl,
		const string& szReffer,
		int requestType,
		string szFilePath,
		string& resultCode
		);

	bool Init();
	void Cleanup();
private:

	bool SetHeadOptLoad(
		const string szUrl,
		const string szReffer,
		int requestType,
		void* szFilePath,
		string& szContent
		);

	static size_t WriteFunc(char *data, size_t size, size_t nmemb, void* writerData);
	static size_t DownLoadFunc(char *data, size_t size, size_t nmemb, void* writerData);

private:
	CURL *m_curl;
};

#endif
