#pragma  once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace  std;


//去掉字符串两头的空白字符
void TrimString(string &str);

//字符串替换
void string_replace(string & strBig, const string & strsrc, const string &strdst);

//变为大写字符
void string_toupper(string & str);

//变为小写字符
void string_tolower(string & str);


//切分字符串,根据cSepCharList中某个字符分割
int SplitStringByChar(const string& strSrc,const string& cSepCharList,vector<string>& vecDest,bool includeEmpties=true);

//切分字符串,根据cSepStr整个字符串分割
int SplitStringByStr(const string& strSrc  ,const string& cSepStr ,vector<string>& vecDest,bool includeEmpties=true);


//Replaces sets like <, >, ", ' and & with the &lt;, &gt;, &quot;, &#039; and &amp; sets.
string HtmlEscape(const string & str);
string HtmlUnEscape(const string & str);


//Replaces sets like <, >, ", ' and & to &lt;, &gt;, &quot;, &apos; and &amp; sets.
string XmlEscape(const string & str);

//Replaces sets like <, >, ", ' and & to &lt;, &gt;, &quot;, &apos; and &amp; sets.
string WmlEscape(const string & str);

//JSESCAPE :Replaces sets like\, \r,\n, ", ' with the \\, \\r,\\n,\",\' and &amp; sets.
string JsEscape(const string & str);

//从文件路径中提取目录部分
string FileDirectory(const string & str_path);

//求字符串的hash值
unsigned int BKDRHash(const string &str);

//转换16进制输出
string hexstr(unsigned char *str,int len) ;

//在s1的前s1Len字符中查找
const char* strnstr(const char* s1,size_t s1Len,const char* s2, size_t s2len);

//调整字符串失败js变量
string FormatJs(const string &szIn,string &szOut);
 

//不区分大小写查找
char* stristr(const char *pcString1, const char *pcString2);


//字符串hash函数，直接散列比较好
unsigned int BKDRHash(const char *str,size_t len);

//防止越界拷贝
void strMaxNumCp(char * strOut,const char* pin,int nmaxcplen);

