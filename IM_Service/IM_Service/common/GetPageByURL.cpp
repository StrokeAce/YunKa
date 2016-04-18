#include "GetPageByURL.h"
#include <assert.h>
#include "stringfun.h"
//#include "log.h"
#include "wincstring.h"
#include "..\log\winlog.h"
#include "..\Include\comdef.h"

#ifdef _WIN32
	#ifdef _DEBUG
	#pragma comment(lib,"libcurl_imp")//libcurld_imp
	#else
	#pragma comment(lib,"libcurl_imp")
	#endif
#endif

#ifndef CURLOPT_ACCEPT_ENCODING
#define CURLOPT_ACCEPT_ENCODING CURLOPT_ENCODING
#endif

struct curlinit
{
	curlinit()
	{
		curl_global_init(CURL_GLOBAL_ALL);  
	}
	~curlinit()
	{
		curl_global_cleanup();  
	}
};


static curlinit g_curlinit;

GetPageByURL::GetPageByURL(void)
{
	m_curl=NULL;
	Init();
}

GetPageByURL::~GetPageByURL(void)
{
	Cleanup();
}



bool GetPageByURL::SetHeadOpt(const string& szUrl,
							  const string& szReffer,
							  const string & szPostBody,
							  string& szContent)
{
	curl_easy_setopt(m_curl, CURLOPT_URL, szUrl.c_str());


	if (!szReffer.empty())
	{
		curl_easy_setopt(m_curl, CURLOPT_REFERER, szReffer.c_str());  
	}

	if (!szPostBody.empty())
	{
		curl_easy_setopt(m_curl, CURLOPT_POST,1L);
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS ,szPostBody.c_str());
	}

	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &szContent);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteFunc);
	return true;
}

bool GetPageByURL::SetHeadOptLoad(const string szUrl,
								const string szReffer,
								int requestType,
								void* szFilePath,
								string& szContent)
{
	curl_easy_setopt(m_curl, CURLOPT_URL, szUrl.c_str());

	if (!szReffer.empty())
	{
		curl_easy_setopt(m_curl, CURLOPT_REFERER, szReffer.c_str());
	}

	if (requestType == REQUEST_TYPE_POST)
	{
		// 上传方式
		curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
		struct curl_httppost *formpost = 0;
		struct curl_httppost *lastptr = 0;
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, szFilePath, CURLFORM_CONTENTTYPE, "image/jpeg", CURLFORM_END);
		curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &szContent);
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteFunc);

	}
	else
	{
		// 下载方式		
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, szFilePath);
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, DownLoadFunc);
	}
	return true;
}

bool GetPageByURL::HttpDownload(const string& szUrl,
						   const string& szReffer,
						   const string & szPostBody,
						   string& szContent)
{
	szContent.clear();
	szContent.reserve(102400);
	if (szUrl.empty()){	return false;}

	if (!m_curl){	Init();	}


	SetHeadOpt(szUrl,szReffer,szPostBody,szContent);

	CURLcode cret=curl_easy_perform(m_curl);
	
	if (cret==CURLE_OK)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE,"httpdownload [success]:%s,post:%s,recv:%s",szUrl.c_str(),szPostBody.c_str(),szContent.c_str());
		return true;
	}
	else
	{
		const char* err = curl_easy_strerror(cret);
		g_WriteLog.WriteLog(C_LOG_ERROR, "httpdownload [failed]:%s,post:%s,recv:%s,code:%d,errorinfo:%s", szUrl.c_str(), szPostBody.c_str(), szContent.c_str(), cret, err);
		return false;
	}
}

bool GetPageByURL::HttpLoad(const string& szUrl,
							const string& szReffer,
							int requestType,
							string szFilePath,
							string& resultCode)
{
	if (szUrl.empty()){ return false; }

	if (!m_curl){ Init(); }

	CURLcode cret;
	if (requestType == REQUEST_TYPE_POST)
	{
		// 上传时，传入文件路径
		SetHeadOptLoad(szUrl, szReffer, requestType, (void*)szFilePath.c_str(), resultCode);
		cret = curl_easy_perform(m_curl);
	}
	else
	{
		// 下载时，传入文件指针
		FILE* pFile;
		fopen_s(&pFile, szFilePath.c_str(), "wb");
		SetHeadOptLoad(szUrl, szReffer, requestType, (void*)pFile, resultCode);
		cret = curl_easy_perform(m_curl);
		fclose(pFile);
	}

	if (cret == CURLE_OK)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE,"httpupload [success]:%s",szUrl.c_str());
		return true;
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "httpupload [failed]:%s", curl_easy_strerror(cret));
		return false;
	}
}

const char* GetPageByURL::GetDocUrl()
{
	const char * purl=NULL;
	if (!m_curl||
		CURLE_OK!=curl_easy_getinfo(m_curl,CURLINFO_EFFECTIVE_URL,&purl))
	{
		purl=NULL;; 
	}

	return purl;
}
const char* GetPageByURL::GetContentTypeLine()
{
	const char * ptype=NULL;
	if (!m_curl||
		CURLE_OK!=curl_easy_getinfo(m_curl,CURLINFO_CONTENT_TYPE,&ptype))
	{
		ptype=NULL;; 
	}

	return ptype;
}
string  GetPageByURL::GetDocCharsetInHead()
{
	string szCharset;
	const char* p=GetContentTypeLine();
	if (p!=NULL)
	{
		szCharset=GetTagTextPbrk(p,"charset=","\r\n ;");
	}

	return szCharset;
}
long GetPageByURL::GetResponseCode()
{
	long lret;
	if (!m_curl||
		CURLE_OK!=curl_easy_getinfo(m_curl,CURLINFO_RESPONSE_CODE,&lret))
	{
		lret=404;; 
	}

	return lret;
}
time_t GetPageByURL::GetDocModiTime()
{
	long lret;
	if (!m_curl||
		CURLE_OK!=curl_easy_getinfo(m_curl,CURLINFO_FILETIME,&lret))
	{
		lret=404;; 
	}

	return lret;
}
long GetPageByURL::GetRedirectCount()
{
	long lret;
	if (!m_curl||
		CURLE_OK!=curl_easy_getinfo(m_curl,CURLINFO_REDIRECT_COUNT,&lret))
	{
		lret=404;; 
	}

	return lret;
}



void  GetPageByURL::FlushCookies()
{
	curl_easy_setopt(m_curl,CURLOPT_COOKIELIST,"FLUSH");
}

void GetPageByURL::SetCookiesRead(const string &szCookieFile)
{
	if (!m_curl){	Init();	}

	if (!szCookieFile.empty())
	{
		curl_easy_setopt(m_curl,CURLOPT_COOKIEFILE,szCookieFile.c_str());
	}
}

void GetPageByURL::SetCookiesWrite(const string &szCookieFile)
{
	if (!m_curl){	Init();	}

	if (!szCookieFile.empty())
	{
		curl_easy_setopt(m_curl,CURLOPT_COOKIEJAR,szCookieFile.c_str());
	}
}
void GetPageByURL::ClearCookies()
{
	curl_easy_setopt(m_curl, CURLOPT_COOKIELIST, "ALL");
}

bool GetPageByURL::Init()
{
	Cleanup();
	m_curl = curl_easy_init();
	if (!m_curl)
	{
		return false;
	}
	//自动重定向到新的地址
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT,
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322)");

	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, 10L);
	curl_easy_setopt(m_curl, CURLOPT_AUTOREFERER, 10L);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT,6);
	curl_easy_setopt(m_curl, CURLOPT_COOKIEFILE, ""); /* just to start the cookie engine */

#ifdef _DEBUG
	 //curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);  //打印多余的日志
#endif

	//If a zero-length string is set, then an Accept-Encoding: header containing all supported encodings is sent. 
	curl_easy_setopt(m_curl,CURLOPT_ACCEPT_ENCODING, ""); //下载支持的所有压缩 gzip,deflate
	return true;
}


void GetPageByURL::Cleanup()
{
	if(m_curl)
	{
		curl_easy_cleanup(m_curl);
		m_curl = NULL;
	}
}

string GetPageByURL::GetCharsetFromfile(const string& szCentenType,const char* szContent)
{
	string szTemp,tagname;
	if (szCentenType.find("/html")!=string::npos)
	{
		tagname="charset=";
		szTemp=GetTagInnerText(szContent,"<head","</head>",NULL,true);


	}else if (szCentenType.find("/xml")!=string::npos)
	{
		tagname="encoding=";
		szTemp=GetTagInnerText(szContent,"<?xml","?>",NULL,true);
	}

	string szCharset;
	if (!szTemp.empty())
	{
		string::size_type e,p=szTemp.find(tagname);
		if (p!=string::npos)
		{
			p+=tagname.size();
			while(szTemp[p]==' '||szTemp[p]=='\"'||szTemp[p]=='\'') p++;

			e=p;

			while(szTemp[e]!='\"'&&szTemp[e]!='\'') e++;

			szCharset=szTemp.substr(p,e-p);
		}

	}

	return szCharset;
}

string GetPageByURL::GetTagInnerText(const char* szContent,const char* szTag1,const char* szTag2,size_t *nextpos,bool bnocase)
{

	string szRet;
	char* pos1 = (bnocase ? stristr(szContent, szTag1) : strstr((char*)szContent, (char*)szTag1));
	if (pos1==NULL)
	{
		return szRet;
	}
	pos1+=strlen(szTag1);

	char* pos2=(bnocase?stristr(pos1,szTag2):strstr(pos1,szTag2));
	if (pos2==NULL)
	{
		return szRet;
	}

	szRet.assign(pos1,pos2-pos1);
	pos2+=strlen(szTag2);

	if (nextpos!=NULL)
	{
		*nextpos=pos2-szContent;
	}

	return szRet;
}

string GetPageByURL::GetTagOuterText(const char* szContent,const char* szTag1,const char* szTag2,size_t *nextpos,bool bnocase)
{
	string szRet;
	char* pos1 = (bnocase ? stristr(szContent, szTag1) : strstr((char*)szContent, (char*)szTag1));
	if (pos1==NULL)
	{
		return szRet;
	}

	char* pos2=(bnocase?stristr(pos1,szTag2):strstr(pos1,szTag2));
	if (pos2==NULL)
	{
		return szRet;
	}

	pos2+=strlen(szTag2);
	szRet.assign(pos1,pos2-pos1);

	if (nextpos!=NULL)
	{
		*nextpos=pos2-szContent;
	}

	return szRet;
}

string GetPageByURL::GetTagTextPbrk(const char* szContent,const char* szTag1,const char* szTag2,size_t *nextpos,bool bnocase)
{
	string szRet;
	char* pos1 = (bnocase ? stristr(szContent, szTag1) : strstr((char*)szContent, (char*)szTag1));
	if (pos1==NULL)
	{
		return szRet;
	}
	pos1+=strlen(szTag1);

	char* pos2=strpbrk(pos1,szTag2);
	if (pos2==NULL)
	{
		return szRet;
	}

	szRet.assign(pos1,pos2-pos1);
	pos2+=1;

	if (nextpos!=NULL)
	{
		*nextpos=pos2-szContent;
	}

	return szRet;
}

size_t GetPageByURL::WriteFunc(char *pdata,
							   size_t size,
							   size_t nmemb,
							   void * szContent)

{
	if (szContent == NULL)
		return 0;
	size_t len = size*nmemb;
	string* sContent = (string*)szContent;
	sContent->append(pdata, len);

	return len;
}

size_t GetPageByURL::DownLoadFunc(char *data, size_t size, size_t nmemb, void* writerData)
{
	if (writerData == NULL)
		return 0;
	size_t len = size*nmemb;
	FILE* pFile = (FILE*)writerData;
	fwrite(data, size, nmemb, pFile);

	return len;
}

struct curl_slist * GetPageByURL::GetCookieList()
{
	struct curl_slist * listret;
	if (!m_curl||
		CURLE_OK!=curl_easy_getinfo(m_curl,CURLINFO_COOKIELIST,&listret))
	{
		listret=NULL; 
	}

	return listret;
}

