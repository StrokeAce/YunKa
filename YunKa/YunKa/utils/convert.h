#ifndef CONVERT_H_
#define CONVERT_H_
#pragma warning(disable:4786)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string>
#include <vector>


#define REGBASE  "Software\\X-Maze\0"


#ifdef _UNICODE	
#define tstring wstring	
#define tstringstream wstringstream
#define _taccess _waccess
#else	
#define tstring string	
#define tstringstream stringstream
#define _taccess _access

#endif

//代码转换类
class CConvert{
public:
        static string LoadFile(const string &path);
        static bool SaveFile(const string &path,const char* content,unsigned long len);
        static bool LoadListFromFile(string path,vector<string>& result);
        static bool GetNextTag(string& content,string& tagname,const string& replacevalue,int &start,int &stop);
	/**
	 * 获取注册表整数值.
	 * @return 参数的值
	 * @param name 参数的名字
	 */
	static unsigned long GetRegInt(string name,char* base=REGBASE);

	/**
	 * 获取注册表字符串值.
	 * @return 参数的值
	 * @param name 参数的名字
	 */
	static string	GetRegStr(string name, char* base = REGBASE);


	/**
	 * 设置注册表整数值.
	 * @param name 参数的名字
	 * @param value 参数的值
	 */
	static void SetRegInt(string name , unsigned long value, char* base=REGBASE);

	/**
	 * 设置注册表字符串值.
	 * @param name 参数的名字
	 * @param value 参数的值
	 */
	static void SetRegStr(string name, string value, char* base = REGBASE);

	/*
	*	交互数字
	*	\param long0 第一个long
	*	\param long1 第二个long
	*/
	static void Swp(unsigned long &long0,unsigned long &long1){unsigned long swp=long1;long1=long0;long0=swp;}

	/*
	*	64位对称加密算法
	*	\param pdata 二进制数据
	*	\param len	 数据长度,最好是4字节的整数倍
	*	\param key0  前32位秘钥
	*	\param kye1  后32位秘钥
	*/
	static void Encrypt(unsigned char* pdata,int len,unsigned long key0,unsigned long key1);

	/*
	*	64位对称加密算法
	*	\param pdata 二进制数据
	*	\param len	 数据长度,最好是4字节的整数倍
	*	\param key   字符串的秘钥
	*/
	static void Encrypt(unsigned char* pdata,int len,string key);


/***************************************************
 将十进制数转换为单个16进制字符，只处理0-15的数
 Return Value: 
	对于0-9，直接返回原数字对应的字符'0'-'9'；
	对于10-15，则返回'A'-'F'
	其它数，返回0（不是字符'0'）
 Parameters:
	n: 待转换的十进制数
 Remoarks:
 ***************************************************/
	static char dec2hexChar(short int n);

/***************************************************
 将16进制数单个字符转换为10进制数
 Return Value: 
	对于'0'-'9'，直接返回原数字对应的字符0-9；
	对于'A'-'F'或者'a'-'f'，则返回10-15
	其它字符，返回-1
 Parameters:
	h: 待转换的16进制数
 Remoarks:
 ***************************************************/
	static short int hexChar2dec(char c);

/***************************************************
 将字符串中不符合URL编码规定的字符改换编码，
 比如空格变为%20.
 “中”字变为%D6%D0
 Return Value: 
	转换后的编码
 Parameters:
	URL: 待转换的URL字符串
 Remoarks:
 ***************************************************/
	static std::string escapeURL(const std::string &URL);

/***************************************************
 和escapeURL功能正相反，
 将已经变换的符合URL编码规定的字符串转换会原文，
 比如%20变为空格.
 %D6%D0变为“中”字
 Return Value: 
	转换后的字符串
 Parameters:
	URL: 待转换的URL编码
 Remoarks:
 ***************************************************/
	static std::string deescapeURL(const std::string &URL);

/**
 * Coding inbuf by base64 Encoding.
 * Turn inbuf into base64 Encoding
 * Reference RFC 1341 for more info about BASE64 encoding
 * @return the string after encoding
 * @param inbuf 待转换的二进制流
 * @param inbufLen 待转换的二进制流的长度
 * @see enBase64Help()
 */
	static string enBase64( const char* inbuf, int inbufLen );

/**
 * Coding inbuf by base64 Encoding.
 * Turn inbuf into base64 Encoding
 * Reference RFC 1341 for more info about BASE64 encoding
 * @return the string after encoding
 * @param inbuf The string to be encoded
 * @see enBase64Help()
 */
	static string enBase64( const string &inbuf);

/**
 * Decoding with Base64.
 * 将用base64编码的字符串解码，得到原来的字符流
 * 关于BASE64编码，可以参见RFC 1341
 * 转换后的字节数是原来长度的3/4倍或3/4倍-1或3/4倍-2，
 * 所以应当保证outbuf的长度至少是src的3/4倍
 * @return 转换后的字节数
 * @param src 用base64编码的字符串
 * @param outbuf 转换结果
 */
	static int deBase64( string src, char* outbuf); 
	static string deBase64( string src); 
/*!
 *	变换tm结构为字符串
 */
	static string tmtostring(tm);

/*!
 *	变换字符串为tm结构
 */
	static tm stringtotm(string strtime);

/*!
 * 变一个'为两个''
 */
	static string TransformQuotation(string str);

        /*!有符号整型值转换为十进制字符串
        *	\param iValue 有符号整型值
        *	\return 对应iValue的字符串
        */
        static std::string IntToStr(unsigned long iValue);

        /*!十进制字符串转换为有符号整型值
        *	\param strValue 十进制字符串
        *	\param Value 对应的有符号整型值
        *	\return 转换是否成功
        */
        static unsigned long  StrToInt(const std::string &strValue);

protected:

/***************************************************
 帮助enBase64函数的辅助函数，
 能够对三个字符编码为4个字符。
 Return Value: 
	无
 Parameters:
	chasc：未编码的二进制代码
	chuue：编码过的Base64代码
 Remarks:
	关于BASE64编码，可以参见RFC 1341
	无论chasc为何值，均认为是三个字符
 ***************************************************/
	static void enBase64Help(unsigned char chasc[3], unsigned char chuue[4]);

/***************************************************
 deBase64函数的辅助函数，
 能够对四个字符的字符串进行解码。
 Return Value: 
	无
 Parameters:
	chuue：编码过的Base64代码
	chasc：解码后的二进制代码
 Remarks:
	关于BASE64编码，可以参见RFC 1341
	不对chuue进行检查是否合法
 ***************************************************/
	static void deBase64Help(unsigned char chuue[4], unsigned char chasc[3]);


};

class ResourceSize
{
public:
	unsigned long SizeLow;
	unsigned long SizeHigh;
	/*!
	*	Create a ResourceSize
	*	\param	Low	    Size的低字节
	*	\param	High    Size的高字节
	*/
	ResourceSize(long Low, long High)
	{
		SizeLow = Low;
		SizeHigh = High;
	}
	/*!
	*	Operator +=
	*	\param	rs	    要加的大小
	*/
	void ResourceSize::operator +=(ResourceSize rs)
	{
		if (0xFFFFFFFF - SizeLow < rs.SizeLow )
		{
			SizeLow += rs.SizeLow ;
			SizeHigh += rs.SizeHigh + 1 ;
		}
		else
		{
			SizeLow += rs.SizeLow ;
			SizeHigh += rs.SizeHigh ;
		}

	}
	string ResourceSize::ToString(char metric=0)
	{
		string result="";
		char buf[20];
		switch (metric)
		{
		case 'G':
			sprintf(buf,"%.2f",SizeLow*1.0/1024/1024/1024+SizeHigh*4);
			result += buf;
			break;
		case 'M':
			sprintf(buf,"%.2f",SizeLow*1.0/1024/1024+SizeHigh*4*1024);
			result += buf;
			break;
		case 'K':
			sprintf(buf,"%.2f",SizeLow*1.0/1024+SizeHigh*4*1024);
			result += buf;
			break;
		case ' ':
			sprintf(buf,"%d",SizeLow+SizeHigh*4*1024*1024*1024);
			result += buf;
			break;
		case 0:
			if ((SizeLow>=1024*1024*1024)||(SizeHigh>0))
				result += ToString('G');
			else
			if ((SizeLow<1024)&&(SizeHigh==0))
				result += ToString(' ');
                        else
			if ((SizeLow<1024*1024)&&(SizeHigh==0))
				result += ToString('K');
			else
                        if ((SizeLow<1024*1024*1024)&&(SizeHigh==0))
				result += ToString('M');
			break;
		}
                if((metric!=0)&&(metric!=' '))
        		result += metric;
		return result;
	}
};

	
#endif
