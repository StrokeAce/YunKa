#include "../stdafx.h"
#include "convert.h"
#include <time.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

string CConvert::GetRegStr(string name,char* base)
//得到name的字串值s, result必须先初始化
{
#ifndef _LINUX_
	HKEY hResult;
	char result[256];
	strcpy(result,"");
	if(ERROR_SUCCESS!=RegOpenKeyA(HKEY_LOCAL_MACHINE,base,&hResult))
	{
		return "";
	}
	DWORD dwcount=255;
	RegQueryValueExA(hResult,name.c_str(),0,0,(BYTE*)&result[0],&dwcount);
	RegCloseKey(hResult);
	return result;
#else
	return "";//GetInitStr(name);
#endif
}

unsigned long CConvert::GetRegInt(string name,char* base)//得到name的长整型值
{
#ifndef _LINUX_
	HKEY hResult;
	int result=0;
	if(ERROR_SUCCESS!=RegOpenKeyA(HKEY_LOCAL_MACHINE,base,&hResult))
	{
		return 0;
	}
	DWORD dwcount=sizeof(int);
	RegQueryValueExA(hResult,name.c_str (),0,0,(BYTE*)&result,&dwcount);
	RegCloseKey(hResult);
	return result;
#else
	return 0;//GetInitInt (name);
#endif
}

void CConvert::SetRegStr(string name,string result,char* base)
//设置name的字串值result
{
#ifndef _LINUX_
	HKEY hResult;
	if(ERROR_SUCCESS!=RegCreateKeyA(HKEY_LOCAL_MACHINE,base,&hResult))
	       return;
	DWORD dwcount=result.length ();
	RegSetValueExA(hResult,name.c_str (),0,REG_SZ ,(BYTE*)result.c_str (),dwcount);
	RegCloseKey(hResult);
#else
//	SetInitStr(name,result);
#endif
}
void CConvert::SetRegInt(string name,unsigned long result,char* base)//设置name的长整型值
{
#ifndef _LINUX_
	HKEY hResult;
	if(ERROR_SUCCESS!=RegCreateKeyA(HKEY_LOCAL_MACHINE,base,&hResult))
	       return;
	DWORD dwcount=sizeof(int);
	RegSetValueExA(hResult,name.c_str (), 0,REG_DWORD ,(BYTE*)&result,dwcount);
	RegCloseKey(hResult);
#else
//	SetInitInt(name,result);
#endif
}


void CConvert::Encrypt(unsigned char* pdata,int len,unsigned long key0,unsigned long key1)
{
	union sDWORD
	{
		unsigned long ldata;
		unsigned char b[4];
	};

	union sWORD
	{
		short a;
		struct
		{
		unsigned char b0;
		unsigned char b1;
		}b;
	};

	union DISPLAY
	{
		struct{	unsigned char v;	}B;
		struct	{
			unsigned char b0	:1;	unsigned char b1	:1;	unsigned char b2	:1;	unsigned char b3	:1;	unsigned char b4	:1;	unsigned char b5	:1;	unsigned char b6	:1;	unsigned char b7	:1;
		}b;
	};

	unsigned char *ps=pdata;
	int ByteLengthBak=len;
	union sDWORD p;
	union sDWORD k;
	DISPLAY c;
	p.ldata=key1;
	k.ldata=key0;
	char buffer1,buffer2;
	unsigned char RBKEY=p.b[0];
	unsigned char RBKEY1=p.b[3];
	unsigned char RBKEY2=p.b[1];
	unsigned char RBKEY3=p.b[2];
	RBKEY^=key0;

	buffer1=p.b[1];
	p.b[1]=k.b[3];
	buffer2=p.b[3];
	p.b[3]=k.b[1];
	k.b[1]=buffer1;
	k.b[3]=buffer2;

	int count=0;
	if (len < 0)
		return;
	while(len)
	{
		if (count==8) count=0;
		if(count<4)
		{
	*(pdata++)^=k.b[count];
		}
		else
		{
	*(pdata++)^=p.b[count-4];
		}
		count++;
		len--;
	}
	for(int ii=0;ii<ByteLengthBak;ii++)
	{
		ps[ii]=ps[ii]^(RBKEY);
		c.B.v=ps[ii];
		c.b.b0^=(RBKEY1&0x1);
		c.b.b2^=((RBKEY1&0x2)>>1);
		c.b.b4^=((RBKEY1&0x4)>>2);
		c.b.b6^=((RBKEY1&0x8)>>3);
		c.b.b7^=((RBKEY1&0x80)>>7);
		c.b.b1^=c.b.b3;
		c.b.b3^=c.b.b5;
		c.b.b5^=c.b.b7;

		c.b.b5^=p.b[0];
		c.b.b3^=p.b[1];
		c.b.b1^=p.b[2];
		c.b.b7^=p.b[3];

		c.b.b0^=(RBKEY2&0x1);
		c.b.b2^=((RBKEY3&0x4)>>1);
		c.b.b4^=((RBKEY2&0x8)>>3);
		c.b.b6^=((RBKEY3&0x80)>>7);
	}
}

void CConvert::Encrypt(unsigned char* pdata,int len,string key)
{
/*	CMD5 md5;
	md5.GenerateMD5 ((unsigned char*)key.c_str (),key.length ());
	Encrypt(pdata,len,md5.m_data[0]^md5.m_data[3],md5.m_data[1]^md5.m_data[2]);
*/
  }

char CConvert::dec2hexChar(short int n) {
	if ( 0 <= n && n <= 9 ) {
		return char( short('0') + n );
	} else if ( 10 <= n && n <= 15 ) {
		return char( short('A') + n - 10 );
	} else {
		return char(0);
	}
}

short int CConvert::hexChar2dec(char c) {
	if ( '0'<=c && c<='9' ) {
		return short(c-'0');
	} else if ( 'a'<=c && c<='f' ) {
		return ( short(c-'a') + 10 );
	} else if ( 'A'<=c && c<='F' ) {
		return ( short(c-'A') + 10 );
	} else {
		return -1;
	}
}

string CConvert::escapeURL(const string &URL)
{
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) {
		char c = URL[i];
		if ( 
			( '0'<=c && c<='9' ) ||
			( 'a'<=c && c<='z' ) ||
			( 'A'<=c && c<='Z' ) ||
			c=='/' || c=='.' 
			) {
			result += c;
		} else {
			int j = (short int)c;
			if ( j < 0 ) {
				j += 256;
			}
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0); 
		}
	}
	return result;
}

std::string CConvert::deescapeURL(const std::string &URL) {
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) {
		char c = URL[i];
		if ( c != '%' ) {
			result += c;
		} else {
			char c1 = URL[++i];
			char c0 = URL[++i];
			int num = 0;
			num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
			result += char(num);
		}
	}
	return result;
}

void CConvert::enBase64Help(unsigned char chasc[3],unsigned char chuue[4])
{
	int i, k=2;
	unsigned char t = 0; 
	for(i=0; i<3; i++)
	{
		*(chuue+i) = *(chasc+i)>>k;
		*(chuue+i) |= t;
		t = *(chasc+i)<<(8-k);
		t >>= 2;
		k += 2;
	}
	*(chuue+3) = *(chasc+2)&63;
	
	for ( i=0; i<4; i++ ) {
		if ( (*(chuue+i)>=0) && (*(chuue+i)<=25) ) {
			*(chuue+i) += 65;	// 'A'-'Z'
		} else if ( (*(chuue+i)>=26) && (*(chuue+i)<=51) ) {
			*(chuue+i) += 71;	// 'a'-'z'
		} else if ( (*(chuue+i)>=52) && (*(chuue+i)<=61) ) {
			*(chuue+i) -= 4;	// 0-9
		} else if ( *(chuue+i)==62 ) {
			*(chuue+i) = 43;	// +
		} else if ( *(chuue+i)==63 ) {
			*(chuue+i) = 47;	// /
		}
	}
}

void CConvert::deBase64Help(unsigned char chuue[4],unsigned char chasc[3]) {
	int i,k=2;
	unsigned char t=0;

	for( i=0; i<4; i++) {
		if ( (*(chuue+i)>=65) && (*(chuue+i)<=90)) 
			*(chuue+i) -= 65;	// 'A'-'Z' -> 0-25
		else if ( (*(chuue+i)>=97)&&(*(chuue+i)<=122) ) 
			*(chuue+i) -= 71;	// 'a'-'z' -> 26-51
		else if ( (*(chuue+i)>=48)&&(*(chuue+i)<=57) ) 
			*(chuue+i) += 4;	// '0'-'9' -> 52-61
		else if ( *(chuue+i)==43 ) 
			*(chuue+i) = 62;	// + -> 62
		else if ( *(chuue+i)==47 ) 
			*(chuue+i) = 63;	// / -> 63
		else if ( *(chuue+i)==61 ) 
			*(chuue+i) = 0;		// = -> 0  Note: 'A'和'='都对应了0
	}
	for ( i=0; i<3; i++ ) {
		*(chasc+i) = *(chuue+i) << k;
		k += 2;
		t = *(chuue+i+1) >> (8-k);
		*(chasc+i) |= t;
	}
}

string CConvert::enBase64( const char* inbuf, int inbufLen ) {
	string outStr;
	unsigned char in[8];
	unsigned char out[8];
	out[4] = 0;
	int blocks = inbufLen / 3;
	for ( int i=0; i<blocks; i++ ) {
		in[0] = inbuf[i*3];
		in[1] = inbuf[i*3+1];
		in[2] = inbuf[i*3+2];
		enBase64Help(in,out);
		outStr += out[0];
		outStr += out[1];
		outStr += out[2];
		outStr += out[3];
	}
	if ( inbufLen % 3 == 1 ) {
		in[0] = inbuf[inbufLen-1];
		in[1] = 0;
		in[2] = 0;
		enBase64Help(in,out);
		outStr += out[0];
		outStr += out[1];
		outStr += '=';
		outStr += '=';
	} else if ( inbufLen % 3 == 2 ) {
		in[0] = inbuf[inbufLen-2];
		in[1] = inbuf[inbufLen-1];
		in[2] = 0;
		enBase64Help(in,out);
		outStr += out[0];
		outStr += out[1];
		outStr += out[2];
		outStr += '=';
	}
	return string(outStr);
}

string CConvert::enBase64( const string &inbuf)
{
	if(inbuf=="")
		return "";
	return CConvert::enBase64( inbuf.c_str(), inbuf.size() );
}

int CConvert::deBase64( string src, char* outbuf ) {

	// Break when the incoming base64 coding is wrong
	if((src.size() % 4 )!= 0 )
        {
                return 0;
        }

	unsigned char in[4];
	unsigned char out[3];

	int blocks = src.size()/4;
	for ( int i=0; i<blocks; i++ ) {
		in[0] = src[i*4];
		in[1] = src[i*4+1];
		in[2] = src[i*4+2];
		in[3] = src[i*4+3];
		deBase64Help(in,out);
		outbuf[i*3]	  = out[0];
		outbuf[i*3+1] = out[1];
		outbuf[i*3+2] = out[2];
	}
	int length = src.size() / 4 * 3;
	if ( src[src.size()-1] == '=' ) {
		length--;
		if ( src[src.size()-2] == '=' ) {
			length--;
		}
	}
	return length;
}
string CConvert::deBase64( string src)
{
	if(src=="")
		return "";

        char * buf=new char[src.length()*2];
        int len=deBase64(src,buf);
        buf[len]='\0';
        string result=string(buf,len);
        delete [] buf;
        return result;
}

// 变换tm为字符串
string CConvert::tmtostring(tm t)
{
	char buf[100];
	strftime(buf,25,"%Y-%m-%d %H:%M:%S ",&t);
	return string(buf);
}

// 变换字符串为tm结构
tm CConvert::stringtotm(string strtime)
{
	tm t;
	sscanf(strtime.c_str(),"%d-%d-%d %d:%d:%d",&t.tm_year, &t.tm_mon, &t.tm_mday , &t.tm_hour , &t.tm_min, &t.tm_sec );
	t.tm_mon -=  1;
    t.tm_year -= 1900;
	return t;
}

//变一个'为两个''
string CConvert::TransformQuotation(string str)
{
	string result="";
	int i = str.length()-1;
	//while ((i>=0)&&(str[i]==' ')) 
	//	i--;
	for(;i>=0;i--)
	{
		if (str[i]=='\'')
			result.insert(0,"''");
		else
			result.insert(0,str.substr(i,1));
	}
	return result;
}

std::string CConvert::IntToStr(unsigned long iValue)
{
        char szBuf[80];
        sprintf(szBuf, "%u", iValue);
        return szBuf;
}

unsigned long CConvert::StrToInt(const std::string &strValue)
{
        return atol(strValue.c_str());
}

string CConvert::LoadFile(const string &path)
{
	FILE* f=fopen(path.c_str(),"rb");
	if(f==NULL)
		return "";
	fseek(f,0,SEEK_END);
	int flen=ftell(f);
	fseek(f,0,SEEK_SET);
	char* buf=new char[flen+1];
	flen=fread(buf,1,flen,f);
	buf[flen]='\0';
	fclose(f);
	string result=string(buf,flen);
	delete [] buf;
	return result;
}
bool CConvert::SaveFile(const string &path,const char* content, unsigned long len)//modify by zyl
{
	FILE* f=fopen(path.c_str(),"wb");
	if(f==NULL)
		return false;
	fwrite(content,1,len,f);
	fclose(f);
	return true;
}

bool CConvert::LoadListFromFile(string path,vector<string> &result)
{
	FILE* srcf=fopen(path.c_str(),"r");
	if(srcf==NULL)
	{
		return false;
	}
	
	while(!feof(srcf))
	{
		char aline[10240];
		aline[0]='\0';
		fgets(aline,10240,srcf);
		if(aline[0]=='\0')
			continue;
		while((strlen(aline)>0)&&(aline[strlen(aline)-1]=='\r')||(aline[strlen(aline)-1]=='\n')||(aline[strlen(aline)-1]==' '))
			aline[strlen(aline)-1]='\0';
		
		if(aline[0]!='\0')
		{
			result.push_back(aline);
		}
	}
	fclose(srcf);
	
	return true;
}

bool CConvert::GetNextTag(string& content,string& tagname,const string& replacevalue,int &start,int &stop)
{
	if((start>0)&&(stop>0))
	{
		content=content.substr(0,start)+replacevalue+content.substr(stop+2);
		start=start+replacevalue.length();
	}
	if(start>=content.length ())
		return false;
	start=content.find ("<%",start);
	if(start<0)
		return false;
	
	stop=content.find("%>",start);
	if(stop<0)
		return false;
	
	tagname=content.substr (start+2,stop-start-2);
	return true;
}


