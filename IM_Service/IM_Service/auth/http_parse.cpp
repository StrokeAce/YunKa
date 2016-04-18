#include "../stdafx.h"
#include "http_parse.h"

#define strncasecmp _strnicmp

CHttpParse::CHttpParse()
:m_nCmdType(0)
,m_bKeepAlive(false)
,m_bGzip(false)
,m_nBodySize(0)
{
}

CHttpParse::~CHttpParse()
{

}

size_t strncspn(char const *s, size_t ssize, char const *reject)
{
	assert(s!=NULL);
	assert(ssize>0);
	assert(reject!=NULL);

	size_t nrej=strlen(reject);
	for(size_t i=0;i<ssize;++i)
	{
		for (size_t j=0;j<nrej;++j)
		{
			if (s[i]==reject[j])
			{
				return i;
			}
		}

	}
	return ssize;
}

bool CHttpParse::ParseHead(const char *phttppack,size_t nhttppacklen)
{
	static char STRGETCMD[]="GET ",STRPOSTCMD[]="POST ";
	static size_t  NGETCMDLEN(sizeof(STRGETCMD)-1),NPOSTCMDLEN(sizeof(STRPOSTCMD)-1);

	if(nhttppacklen>12)
	{
		const char * ppackend=phttppack+nhttppacklen;
		//解析 命令字
		size_t nveluelen;
		if (!strncmp(phttppack,STRGETCMD,sizeof(STRGETCMD) -1))
		{
			nveluelen=NGETCMDLEN;
			m_nCmdType=0;
		}else if (!strncmp(phttppack,STRPOSTCMD,sizeof(STRPOSTCMD) -1))
		{
			nveluelen=NPOSTCMDLEN;
			m_nCmdType=1;
		}else
		{
			return false;
		}

		//解析 uri
		const char *pparsepos=phttppack+nveluelen;
		if (*pparsepos!='/'){//命令后没有请求uri 出错
			return false;		
		}
		nveluelen=strncspn(pparsepos,ppackend-pparsepos," ?\r\n");
		m_strUrl.assign(pparsepos,nveluelen);

		//解析url参数
		if (pparsepos[nveluelen]=='?'){
			pparsepos+=nveluelen+1;//跳过 '?'

			nveluelen=strncspn(pparsepos,ppackend-pparsepos," \r\n");

			//解析uri后面带的参数
			ParseHeadParams(pparsepos,nveluelen);
		}else
		{
			m_mapParamsList.clear();
		}

		//解出http头的各行
		pparsepos=phttppack+nveluelen;
		pparsepos=strstr(pparsepos,"\r\n");
		while(pparsepos!=NULL)
		{
			pparsepos+=2;
			if (pparsepos>=ppackend-2){
				//遇到 字符结尾，或两个回车换行，http头结束
				break;
			}

			if (*pparsepos=='\r'){
				//http头结束
				break;
			}		

			string *pstrvalestr=NULL;
			if (!strncasecmp(pparsepos,"Referer:",8)){
				pstrvalestr=&m_strRefererLine;
				pparsepos+=8;
			}else if (!strncasecmp(pparsepos,"Accept-Language:",16))
			{
				pstrvalestr=&m_strAcceptLanguageLine;
				pparsepos+=16;
			}else if (!strncasecmp(pparsepos,"User-Agent:",11))
			{
				pstrvalestr=&m_strUserAgentLine;
				pparsepos+=11;

			}else if (!strncasecmp(pparsepos,"Connection:",11)){
				pparsepos+=11;
				while(' '==*pparsepos) ++pparsepos;
				if (!strncasecmp(pparsepos,"Keep-Alive",10))
				{
					m_bKeepAlive=true;
				}
			}else if (!strncasecmp(pparsepos,"Accept-Encoding:",16)){
				pparsepos+=16;
				while(' '==*pparsepos) ++pparsepos;
				nveluelen=strncspn(pparsepos,ppackend-pparsepos,"\r\n");
				if (nveluelen>0)
				{
					string acode(pparsepos,nveluelen);
					if (acode.find("gzip")!=string::npos&&acode.find("deflate")!=string::npos)
					{
						m_bGzip=true;
					}
				}

			}else if (!strncasecmp(pparsepos,"Content-Length:",15)){
				pparsepos+=15;
				while(' '==*pparsepos) ++pparsepos;
				m_nBodySize=atoi(pparsepos);

			}else if (!strncasecmp(pparsepos,"Cookie:",7)){
				pstrvalestr=&m_strCollkieLine;
				pparsepos+=7;
			}else if (!strncasecmp(pparsepos,"X-Forwarded-For:",16)){
				pstrvalestr=&m_strXForwardedFor;
				pparsepos+=16;
			}else if (!strncasecmp(pparsepos,"Host:",5)){
				pstrvalestr=&m_strHost;
				pparsepos+=5;
			}

			if (pstrvalestr!=NULL){
				while(' '==*pparsepos) ++pparsepos;

				nveluelen=strncspn(pparsepos,ppackend-pparsepos,"\r\n");
				pstrvalestr->assign(pparsepos,nveluelen);
			}

			pparsepos=strstr(pparsepos,"\r\n");
		}



		return true;
	}
	return	false;
}

bool CHttpParse::GetHeadParams(const char *pstrkey,string & strValue) const
{

	 map<string,string>::const_iterator mpos= m_mapParamsList.find(pstrkey);
	if ( mpos!=m_mapParamsList.end())
	{
		strValue=mpos->second;
		return true;
	}
	return false;
}


bool CHttpParse::GetPostBodyParams(const string& strPostBody,const char *pstrkey,string & strValue)
{
	string skey(pstrkey);
	skey+='=';

	string::size_type posb(0);
	while(1)
	{
		posb=strPostBody.find(skey,posb);
		if (posb==string::npos)
		{
			break;
		}

		if ( (posb==0||'\n'==strPostBody[posb-1] ) )
		{
			posb+=skey.length();
			string::size_type posend=strPostBody.find('\n',posb);
			if (posend!=string::npos)
			{
				if (strPostBody[posend-1]=='\r'){--posend;	}

				strValue=strPostBody.substr(posb,posend-posb);
			}else
			{
				strValue=strPostBody.substr(posb); //到末尾
			}
			return true;
		}
		else
		{
			posb+=skey.length();
		}
	}

	return false;
}

void CHttpParse::ParseHeadParams(const char *pparmstr,size_t nlen)
{
	bool bkey=true;
	string skey,svalue;
	skey.reserve(50);svalue.reserve(100);
	for (size_t i=0;i<=nlen;++i)
	{
		if ('&'==pparmstr[i]||i==nlen)
		{
			if (!skey.empty())
			{
				m_mapParamsList[skey]=svalue;
			}
			skey="";
			svalue="";
//			skey.clear();
//			svalue.clear();
			bkey=true;
		}else if ('='==pparmstr[i]&&bkey)
		{
			bkey=false;
		} else
		{
			if (bkey)
			{
				skey+=pparmstr[i];
			}else
			{
				svalue+=pparmstr[i];
			}
		}
	}

}
