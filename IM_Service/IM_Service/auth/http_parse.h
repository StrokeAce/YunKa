#ifndef HTTPPARSE_H_
#define  HTTPPARSE_H_

#include <string>
#include <map>
using namespace std;

class CHttpParse
{
public:
	CHttpParse();
	~CHttpParse();
	int     m_nCmdType;			//请求命令0: GET 或 1:POST

	string m_strUrl;
	map<string,string> m_mapParamsList;			//url参数

	string  m_strRefererLine;			//Referer行内容
	string  m_strAcceptLanguageLine;	//Accept-Language行内容
	string  m_strUserAgentLine;		    //User-Agent行内容
	bool    m_bKeepAlive;		        //Connection行内容
	bool    m_bGzip;					//Accept-Encoding行内容
	size_t  m_nBodySize;				//Content-Length
	string  m_strCollkieLine;			//Cookie行内容
	string  m_strXForwardedFor;			//X-Forwarded-For行内容
	string  m_strHost;                  //


	/**
	从http头中取出 url 和url行参数,参数行前后加‘&’方便解析

	phttppack：输入包含包头的http包字符串，必须以null结束
	nhttppacklen：包头长度
	*/

	bool ParseHead(const char *phttppack,size_t nhttppacklen);

	/**
	从参数行中取出某个参数
	pstrkey: 输入get行参数key，key必须以'\0'结束
	pvaluebuf:输出值缓存区
	nvaluebuflen:输出缓存区长度
	返回值: NULL key没找找到，其他找到的key，pvaluebuf保持key对应的字符串值,当key对应的value长度小于nvaluebuflen时，在pvaluebuf未添加0

	*/
	bool GetHeadParams(const char *pstrkey,string & strValue) const;

	/**
	从post体中取出某个参数,要求m_strPostBody中一行一个参数
	*/
	static bool GetPostBodyParams(const string& strPostBody,const char *pstrkey,string & strValue);

private:

	//一次性解析所有url参数
	void ParseHeadParams(const char *pparmstr,size_t nlen);

};

#endif
