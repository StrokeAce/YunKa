#include "stringfun.h"
#ifdef WIN32
#include "wincstring.h"
#else
#include <strings.h>
#include <string.h>
#endif



//去掉字符串两头的空白字符
void TrimString(string &str)
{
	static const char* spacechs=" \r\n\t\v\f";
	string::size_type pos = str.find_last_not_of(spacechs);
	if(pos != string::npos) 
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(spacechs);
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}

//字符串替换
void string_replace(string & strBig, const string & strsrc, const string &strdst) 
{
	string::size_type pos=0;
	string::size_type srclen=strsrc.size();
	string::size_type dstlen=strdst.size();
	while( (pos=strBig.find(strsrc, pos)) != string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

//变为大写字符
void mytoupper(char &c)
{
	if (c>='a'&&c<='z')
	{
		c-=32;
	}
} 
void string_toupper(string & str)
{
	for_each(str.begin(),str.end(),mytoupper); 
}

//变为小写字符
static void mylower(char &c)
{
	if (c>='A'&&c<='Z')
	{
		c+=32;
	}
} 
void string_tolower(string & str)
{
	for_each(str.begin(),str.end(),mylower); 
}


//切分字符串,根据cSepCharList中某个字符分割
int SplitStringByChar(const string&  strSrc  ,const string& cSepCharList ,vector<string>& vecDest,bool includeEmpties)
{
	if( strSrc.empty()||cSepCharList.empty())
		return 0;

	int num(0);
	string::size_type size_prev_pos = 0;
	while(1)
	{
		string::size_type size_pos = strSrc.find_first_of( cSepCharList ,size_prev_pos);
		if (size_pos!=string::npos)
		{
			if (includeEmpties||size_pos>size_prev_pos)
			{
				vecDest.push_back(strSrc.substr(size_prev_pos , size_pos-size_prev_pos ));++num;
			}
			size_prev_pos =++ size_pos;
		}else
		{
			string szTemp=strSrc.substr(size_prev_pos);
			if (includeEmpties||!szTemp.empty())
			{
				vecDest.push_back(&strSrc[size_prev_pos]);++num;
			}
			break;
		}
	} 

	return num;

}

//切分字符串,根据cSepStr整个字符串分割
int SplitStringByStr(const string&  strSrc  ,const string& cSepStr ,vector<string>& vecDest,bool includeEmpties)
{
	if( strSrc.empty()||cSepStr.empty())
		return 0;

	int num(0);

	string::size_type nkeylen=cSepStr.size();
	string::size_type size_prev_pos = 0;
	while(1)
	{
		string::size_type size_pos = strSrc.find( cSepStr ,size_prev_pos);
		if (size_pos!=string::npos)
		{
			if (includeEmpties||size_pos>size_prev_pos)
			{
				vecDest.push_back(strSrc.substr(size_prev_pos , size_pos-size_prev_pos ));++num;
			}
			size_prev_pos =nkeylen+size_pos;
		}else
		{
			string szTemp=strSrc.substr(size_prev_pos);
			if (includeEmpties||!szTemp.empty())
			{
				vecDest.push_back(&strSrc[size_prev_pos]);++num;
			}
			break;
		}
	} 

	return num;
}


//Replaces sets like <, >, ", ' and & with the &lt;, &gt;, &quot;, &#039; and &amp; sets.
string HtmlEscape(const string & str)
{
	string szOut;
	string::size_type srclen=str.size();
	for (size_t i=0;i<srclen;++i)
	{
		switch (str[i])
		{
		case  '&':
			szOut+="&amp;";break;
		case  '"':
			szOut+="&quot;";break;
		case  '\'':
			szOut+="&#39;";break;
		case  '<':
			szOut+="&lt;";break;
		case  '>':
			szOut+="&gt;";break;
		default:
			szOut+=str[i];
		}
	}

	return szOut;
}

//Replaces sets like <, >, ", ' and & with the &lt;, &gt;, &quot;, &#039; and &amp; sets.
string HtmlUnEscape(const string & str)
{
	string szOut;
	string::size_type srclen=str.size();
	for (size_t i=0;i<srclen;)
	{
		if ('&'==str[i])
		{
			if (0==strncmp(str.c_str()+i,"&quot;",6))
			{
				szOut+='"';
				i+=6;
			}else if (0==strncmp(str.c_str()+i,"&nbsp;",6))
			{
				szOut+=' ';
				i+=6;
			}else if (0==strncmp(str.c_str()+i,"&amp;",5))
			{
				szOut+='&';
				i+=5;
			}else if (0==strncmp(str.c_str()+i,"&#39;",5))
			{
				szOut+='\'';
				i+=5;
			}else if (0==strncmp(str.c_str()+i,"&lt;",4))
			{
				szOut+='<';
				i+=4;
			}else if (0==strncmp(str.c_str()+i,"&gt;",4))
			{
				szOut+='>';
				i+=4;
			}else
			{
				szOut+=str[i++];
			}
		}else
		{
			szOut+=str[i++];
		}
	}

	return szOut;
}

//Replaces sets like <, >, ", ' and & to &lt;, &gt;, &quot;, &apos; and &amp; sets.
string XmlEscape(const string & str)
{
	string szOut;
	string::size_type srclen=str.size();
	for (size_t i=0;i<srclen;++i)
	{
		switch (str[i])
		{
		case  '&':
			szOut+="&amp;";break;
		case  '"':
			szOut+="&quot;";break;
		case  '\'':
			szOut+" &apos;";break;  //这里和HtmlEscape不同
		case  '<':
			szOut+="&lt;";break;
		case  '>':
			szOut+="&gt;";break;
		default:
			szOut+=str[i];

		}
	}

	return szOut;
}

//Replaces sets like <, >, ", ' and & to &lt;, &gt;, &quot;, &apos; and &amp; sets.
string WmlEscape(const string & str)
{
	string szOut;
	string::size_type srclen=str.size();
	for (size_t i=0;i<srclen;++i)
	{
		switch (str[i])
		{
		case  '&':
			szOut+="&amp;";break;
		case  '"':
			szOut+="&quot;";break;
		case  '\'':
			szOut+" &apos;";break;  
		case  '<':
			szOut+="&lt;";break;
		case  '>':
			szOut+="&gt;";break;
		case  '$':
			szOut+="$$";break;//比较XmlEscape 多一个$
		default:
			szOut+=str[i];
		}
	}
	return szOut;
}

//JSESCAPE :Replaces sets like\, \r,\n, ", ' with the \\, \\r,\\n,\",\' and &amp; sets.
string JsEscape(const string & str)
{
	string szOut;
	string::size_type srclen=str.size();
	for (size_t i=0;i<srclen;++i)
	{
		switch (str[i])
		{
		case  '\\':
			szOut+="\\\\";break;
		case  '\'':
			szOut+="\\'";break;
		case  '"':
			szOut+="\\\"";break;
		case  '\n':
			szOut+="\\n";break;
		case  '\r':
			szOut+="\\r";break;
		default:
			szOut+=str[i];
		}
	}

	return szOut;
}

//从文件路径中提取目录部分
string FileDirectory(const string & str_path)
{
	string str =str_path;
	TrimString(str);
	if (str.empty()) return "";

	string::size_type last_slash_pos = str.find_last_of("\\/");

	// if its just the file name
	if (last_slash_pos == string::npos)
	{
		return "";
	}

	str = str_path.substr(0, last_slash_pos + 1);

	// unix-like path is going to get lost if file is in root 
	if (last_slash_pos == 0)
	{
		str = "/";
	}

	return str;
}

unsigned int BKDRHash(const string &str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	for(size_t i=0;i<str.size();++i)
	{
		hash = hash * seed + str[i];
	}

	return (hash & 0x7FFFFFFF);
}

string hexstr(unsigned char *str,int len) 
{ 
	static const char *set = "0123456789abcdef"; 

	string szOut;
	unsigned char * end = str + len; 
	while (str <end)
	{ 
		szOut+= set[ (*str) >> 4 ]; 
		szOut+= set[ (*str) & 0xF ]; 
		str ++; 
	} 
	return szOut; 
} 

/**
 *在s1的前s1Len字符中查找
*/
const char* strnstr(const char* s1,size_t s1Len,const char* s2, size_t s2len)
{
 if (!s2len||s2==NULL)
    return (char *)s1;

 while (s1Len >= s2len) 
 {
  if (!strncasecmp(s1, s2, s2len))
   return (char *)s1;

  --s1Len;
  s1++;
 }
 return NULL;
}

//调整字符串失败js变量
string FormatJs(const string &szIn,string &szOut)
{
	szOut.clear();
	size_t nlen=szIn.length();
	for (string::size_type i=0;i<nlen;++i)
	{
		if (szIn[i]=='\n'||szIn[i]=='\r')
		{
			continue;
		}

		if ('<'==szIn[i]&&i+4<=nlen&&0==strncasecmp(szIn.c_str()+i,"<br>",4))
		{
			i+=3; //过来<br>  
			continue;
		}

		if (szIn[i]=='\\'||szIn[i]=='\''||szIn[i]=='\"')
		{
			szOut+='\\';
		}
		szOut+=szIn[i];
	}

	return szOut;
}


char* stristr(const char *pcString1, const char *pcString2)
{
	char *pCompareStart = (char *)pcString1;
	char *pCursor_S1, *pCursor_S2;
	char cSrc, cDst;

	// If there is a null source string - this is a "no match"
	if (!pcString1)		return NULL;

	// Null length string 2 - this is a "no match"
	if (!*pcString2)		return NULL;

	// Search from every start pos in the source string
	while (*pCompareStart)
	{
		pCursor_S1 = pCompareStart;
		pCursor_S2 = (char *)pcString2;

		// Scan both string

		while (*pCursor_S1 && *pCursor_S2)
		{
			cSrc = *pCursor_S1;
			cDst = *pCursor_S2;

			// Correct case

			if ((cSrc >= 'A') && (cSrc <= 'Z'))
				cSrc -= ('A' - 'a');

			if ((cDst >= 'A') && (cDst <= 'Z'))
				cDst -= ('A' - 'a');

			if (cSrc != cDst)
				break;

			pCursor_S1++;
			pCursor_S2++;
		}

		// If string 2 is exhausted - there is a match
		if (!*pCursor_S2)
			return(pCompareStart);

		// Offset source and continue
		pCompareStart++;
	}

	return NULL;
}

// BKDR Hash Function
unsigned int BKDRHash(const char *str,size_t len)
{
	if (NULL==str||!len)
	{
		return 0;
	}

	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	for (size_t i=0;i<len;++i)
	{
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}

//防止越界拷贝
void strMaxNumCp(char * strOut,const char* pin,int nmaxcplen)
{
	int nlen=strlen(pin);
	if (nlen>=nmaxcplen)
	{
		strncpy(strOut,pin,nmaxcplen-1);
		strOut[nmaxcplen-1]=0;
	}else
	{
		strcpy(strOut,pin);
	}
	
}
