#include "../stdafx.h"
#include "code_convert.h"

CCodeConvert::CCodeConvert()
{

}

CCodeConvert::~CCodeConvert()
{

}

char CCodeConvert::hexTodec(char ch)
{
	char ret;
	if (ch=='A'||ch=='a')
	{
		ret=10;
	}else if (ch=='B'||ch=='b')
	{
		ret=11;
	}else if (ch=='C'||ch=='c')
	{
		ret=12;
	}else if (ch=='D'||ch=='d')
	{
		ret=13;
	}else if (ch=='E'||ch=='e')
	{
		ret=14;
	}else if (ch=='F'||ch=='f')
	{
		ret=15;
	} else{
		char t[2];
		t[0]=ch;
		t[1]='\0';
		ret=atoi(t);
	}
	return ret;
}

//文本形式转为 二进制形式(如：%C7%A2%CC%B8%CD%A8123->洽谈通123)
int CCodeConvert::TextToBit(char *out,const char *pText)
{
	bool bFirst=1;
	char temp[2],*p;
	int len,nret=0;
	strcpy (out,"");
	p=strtok((char*)pText,"%");
	while(p!=NULL)
	{
		if(bFirst&&p==pText)
		{
			strcat(out,p);
		}
		else{
			len=strlen(p);
			if (len<2)
			{
				p=strtok(NULL,"%");
				nret=2;
				bFirst=false;
				continue;
			}else {

				temp[0]=hexTodec(*p)*16+hexTodec(*(p+1));
				temp[1]='\0';
				if (bFirst) 
				{
					bFirst=false;
					if (*p=='E'||*p=='e')
					{
						nret=1;
					}

				}
				
				strcat(out,temp);
				if(len>2)
				{
					strcat(out,p+2);
				}
			}
		
		}
		p=strtok(NULL,"%");
	}
	return nret;
}

//判断一个字符串是不是utf8编码
bool CCodeConvert::IsUtf8(const char *pdata,int len)
{
	if(len <= 2)
		return false;
	int i;
	int nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
	for(i=0;i<len;i++)
	{
		chr= *(pdata+i);
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii= false;
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				//else if(chr>=0xC0)
				//	nBytes=2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr >= 0x80) && (chr < 0xC0) )
			{
				nBytes--;
			}
			else
			{
				return false;
			}
		}
	}

	if( nBytes > 0 ) //违返规则
	{
		return false;
	}

	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return false;
	}
	return true;

}

// 把UTF-8转换成Unicode
void CCodeConvert::UTF_8ToUnicode(WCHAR* pOut,const char *pText,int utf8len)
{
	switch(utf8len)
	{
	case 1:
		*pOut=*pText;
		break;
	case 2:
		*pOut = (*pText & 0x1f) << 6;
		*pOut |= ( *(pText+1) & 0x3f );
		break;
	case 3:
		*pOut = *pText << 12;
		*pOut |= ( (*(pText+1) & 0x3f) << 6 );
		*pOut |= (*(pText+2) & 0x3f);
		break;

	}


	
}
// Unicode 转换成UTF-8 
void CCodeConvert::UnicodeToUTF_8(char* pOut,const WCHAR* pText,int utf8len)
{

	switch(utf8len)
	{
	case 1:
		*pOut=(char)*pText;
		break;
	case 2:
		*pOut=((*pText&0x7FF)>>6)|0xC0;
		*(pOut+1)=((*pText&0x3F))|0x80;
		break;
	case 3:
		*pOut=(*pText>>12)|0xE0;
		*(pOut+1)=((*pText>>6)&0x3F)|0x80;
		*(pOut+2)=(*pText&0x3F)|0x80;
	}

	
}


//GB2312 转为 UTF-8
void CCodeConvert::Gb2312ToUTF_8(std::string& pOut,const char *pText, int pLen)
{
	LocalToUTF_8(936,pOut,pText,pLen);
}
//ansi 转为 UTF-8
void CCodeConvert::AnsiToUTF_8(std::string& pOut,const char *pText, int pLen)
{
	LocalToUTF_8(CP_ACP,pOut,pText,pLen);
}


//GB2312 转为 UTF-8
void CCodeConvert::LocalToUTF_8(UINT CodePage,std::string& pOut,const char *pText, int pLen)
{
	int len=pLen*2 + 1;
	char* rst = new char[len];
	memset(rst,0,len);
	
	char buf[4];
	memset(buf,0,4);
	int i = 0;
	int j = 0;      
	while(i < pLen)
	{
		//如果是英文直接复制就可以
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			WCHAR pbuffer;
			LocalToUnicode(CodePage,&pbuffer,pText+i);
			
			UnicodeToUTF_8(buf,&pbuffer);
			
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];
			
			
			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';
	
	//返回结果
	pOut = rst;             
	delete []rst; 
	rst = NULL;

	return;
}

// 把Unicode 转换成 GB2312 
inline void CCodeConvert::UnicodeToAnsi(char* pOut,WCHAR uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
}     


// 本地码 转换成　Unicode
inline void CCodeConvert::LocalToUnicode(UINT CodePage,WCHAR* pOut,const char *gbBuffer)
{
	::MultiByteToWideChar(CodePage,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
}

//UTF-8 转为 GB2312
void CCodeConvert::UTF_8ToAnsi(std::string &pOut, const char *pText, int pLen)
{
	char * newBuf = new char[pLen+1];
	memset(newBuf,0,pLen+1);

	char Ctemp[4];
	memset(Ctemp,0,4);
	int i =0;
	int j = 0;
	
	while(i < pLen)
	{
		if(pText[i] > 0)
		{
			newBuf[j++] = pText[i++];                       
		}
		else                 
		{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp,pText + i);
			
			UnicodeToAnsi(Ctemp,Wtemp);
			
			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];
			
			i += 3;    
			j += 2;   
		}
	}
	newBuf[j] = '\0';
	
	pOut = newBuf;
	delete []newBuf;
	newBuf = NULL;

	return; 
}

int CCodeConvert::utf8ToUcs2(const char *s, int len, WCHAR *wpout, int wbuf_len)
{
	int i=0,j=0;

	unsigned char c2,c3;
	WCHAR Wtemp;
	while(i < len&&j<wbuf_len-1)
	{
		if(*(s+i) > 0)
		{
			wpout[j++] = s[i++];   
		}
		else                 
		{
			c2=*(s+i)&0xE0;
			c3=*(s+i)&0xF0;
			if (c2==0xC0)
			{//两个字节
				if (i>len-2)
				{
					break;
				}
				UTF_8ToUnicode(&Wtemp,s + i,2);
				i+=2;
			}else  if (c3==0xE0)
			{//三个字节
				if (i>len-3)
				{
					break;
				}
				UTF_8ToUnicode(&Wtemp,s + i,3);
				i += 3;    
			}else
			{//有错误
				i++;
				continue;
			}
			wpout[j++] = Wtemp;
		}
	}

	wpout[j]=0;
	return j;
}

int CCodeConvert::Ucs2Toutf8(const  WCHAR *wpin, int inlen, char *pout, int outlen)
{
	int i=0,j=0;

	char buf[4];
	while(i < inlen&&j<outlen-1)
	{
		//如果是英文直接复制就可以
		if( *(wpin + i) <0x80)
		{
			pout[j++] =(char) wpin[i];
		}
		else if (*(wpin + i)>=0x800)
		{//三个字节
			if (j>outlen-4)
			{
				break;
			}
	
			UnicodeToUTF_8(buf,&wpin[i],3);
			pout[j] = buf[0];
			pout[j+1] = buf[1];
			pout[j+2] = buf[2];
			
			j += 3;

		}else
		{//两个字节
			if (j>outlen-3)
			{
				break;
			}
			UnicodeToUTF_8(buf,&wpin[i],2);
			pout[j] = buf[0];
			pout[j+1] = buf[1];
			j+=2;

		}

		i++;
	}
	pout[j] = '\0';

	return j;
}

std::string CCodeConvert::URLEncode(const char *code)
{
	
	std::string strRet;// phl 09-02-06
	char tmp[4];
	while (*code)
	{
		if( 42 == *code || 45 == *code || 46 == *code ||
			(48 <= *code && *code <= 57) ||
			(65 <= *code && *code <= 90) ||
			95 == *code || 
			(97 <= *code && *code <= 122))
		{
			strRet += *code;
		}
		else
		{
			sprintf(tmp, "%c%.2X", '%', *code&0xFF);
			strRet += tmp;
		}		
		code++;
	}
	strRet += "";
	return strRet;
}

std::string CCodeConvert::URLDecode(const std::string szToDecode)
{
	std::string result;
	int hex = 0;
	for (size_t i = 0; i < szToDecode.length(); ++i)
	{
		switch (szToDecode[i])
		{
		case '+':
			result += ' ';
			break;
		case '%':
			if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
			{
				std::string hexStr = szToDecode.substr(i + 1, 2);
				hex = strtol(hexStr.c_str(), 0, 16);
				//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]  
				//可以不经过编码直接用于URL  
				if (!((hex >= 48 && hex <= 57) || //0-9  
					(hex >= 97 && hex <= 122) ||   //a-z  
					(hex >= 65 && hex <= 90) ||    //A-Z  
					//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
					hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
					|| hex == 0x2a || hex == 0x2b || hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
					|| hex == 0x3A || hex == 0x3B || hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
					))
				{
					result += char(hex);
					i += 2;
				}
				else result += '%';
			}
			else {
				result += '%';
			}
			break;
		default:
			result += szToDecode[i];
			break;
		}
	}
	return result;
}

BYTE fromHex(const BYTE &x)
{
	return isdigit(x) ? x - '0' : x - 'A' + 10;
}

std::string CCodeConvert::URLDecodeALL(const std::string szToDecode)
{
	std::string sOut;
	for (size_t ix = 0; ix < szToDecode.size(); ix++)
	{
		BYTE ch = 0;
		if (szToDecode[ix] == '%')
		{
			ch = (fromHex(szToDecode[ix + 1]) << 4);
			ch |= fromHex(szToDecode[ix + 2]);
			ix += 2;
		}
		else if (szToDecode[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = szToDecode[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}
