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

//����ת����
class CConvert{
public:
        static string LoadFile(const string &path);
        static bool SaveFile(const string &path,const char* content,unsigned long len);
        static bool LoadListFromFile(string path,vector<string>& result);
        static bool GetNextTag(string& content,string& tagname,const string& replacevalue,int &start,int &stop);
	/**
	 * ��ȡע�������ֵ.
	 * @return ������ֵ
	 * @param name ����������
	 */
	static unsigned long GetRegInt(string name,char* base=REGBASE);

	/**
	 * ��ȡע����ַ���ֵ.
	 * @return ������ֵ
	 * @param name ����������
	 */
	static string	GetRegStr(string name, char* base = REGBASE);


	/**
	 * ����ע�������ֵ.
	 * @param name ����������
	 * @param value ������ֵ
	 */
	static void SetRegInt(string name , unsigned long value, char* base=REGBASE);

	/**
	 * ����ע����ַ���ֵ.
	 * @param name ����������
	 * @param value ������ֵ
	 */
	static void SetRegStr(string name, string value, char* base = REGBASE);

	/*
	*	��������
	*	\param long0 ��һ��long
	*	\param long1 �ڶ���long
	*/
	static void Swp(unsigned long &long0,unsigned long &long1){unsigned long swp=long1;long1=long0;long0=swp;}

	/*
	*	64λ�ԳƼ����㷨
	*	\param pdata ����������
	*	\param len	 ���ݳ���,�����4�ֽڵ�������
	*	\param key0  ǰ32λ��Կ
	*	\param kye1  ��32λ��Կ
	*/
	static void Encrypt(unsigned char* pdata,int len,unsigned long key0,unsigned long key1);

	/*
	*	64λ�ԳƼ����㷨
	*	\param pdata ����������
	*	\param len	 ���ݳ���,�����4�ֽڵ�������
	*	\param key   �ַ�������Կ
	*/
	static void Encrypt(unsigned char* pdata,int len,string key);


/***************************************************
 ��ʮ������ת��Ϊ����16�����ַ���ֻ����0-15����
 Return Value: 
	����0-9��ֱ�ӷ���ԭ���ֶ�Ӧ���ַ�'0'-'9'��
	����10-15���򷵻�'A'-'F'
	������������0�������ַ�'0'��
 Parameters:
	n: ��ת����ʮ������
 Remoarks:
 ***************************************************/
	static char dec2hexChar(short int n);

/***************************************************
 ��16�����������ַ�ת��Ϊ10������
 Return Value: 
	����'0'-'9'��ֱ�ӷ���ԭ���ֶ�Ӧ���ַ�0-9��
	����'A'-'F'����'a'-'f'���򷵻�10-15
	�����ַ�������-1
 Parameters:
	h: ��ת����16������
 Remoarks:
 ***************************************************/
	static short int hexChar2dec(char c);

/***************************************************
 ���ַ����в�����URL����涨���ַ��Ļ����룬
 ����ո��Ϊ%20.
 ���С��ֱ�Ϊ%D6%D0
 Return Value: 
	ת����ı���
 Parameters:
	URL: ��ת����URL�ַ���
 Remoarks:
 ***************************************************/
	static std::string escapeURL(const std::string &URL);

/***************************************************
 ��escapeURL�������෴��
 ���Ѿ��任�ķ���URL����涨���ַ���ת����ԭ�ģ�
 ����%20��Ϊ�ո�.
 %D6%D0��Ϊ���С���
 Return Value: 
	ת������ַ���
 Parameters:
	URL: ��ת����URL����
 Remoarks:
 ***************************************************/
	static std::string deescapeURL(const std::string &URL);

/**
 * Coding inbuf by base64 Encoding.
 * Turn inbuf into base64 Encoding
 * Reference RFC 1341 for more info about BASE64 encoding
 * @return the string after encoding
 * @param inbuf ��ת���Ķ�������
 * @param inbufLen ��ת���Ķ��������ĳ���
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
 * ����base64������ַ������룬�õ�ԭ�����ַ���
 * ����BASE64���룬���Բμ�RFC 1341
 * ת������ֽ�����ԭ�����ȵ�3/4����3/4��-1��3/4��-2��
 * ����Ӧ����֤outbuf�ĳ���������src��3/4��
 * @return ת������ֽ���
 * @param src ��base64������ַ���
 * @param outbuf ת�����
 */
	static int deBase64( string src, char* outbuf); 
	static string deBase64( string src); 
/*!
 *	�任tm�ṹΪ�ַ���
 */
	static string tmtostring(tm);

/*!
 *	�任�ַ���Ϊtm�ṹ
 */
	static tm stringtotm(string strtime);

/*!
 * ��һ��'Ϊ����''
 */
	static string TransformQuotation(string str);

        /*!�з�������ֵת��Ϊʮ�����ַ���
        *	\param iValue �з�������ֵ
        *	\return ��ӦiValue���ַ���
        */
        static std::string IntToStr(unsigned long iValue);

        /*!ʮ�����ַ���ת��Ϊ�з�������ֵ
        *	\param strValue ʮ�����ַ���
        *	\param Value ��Ӧ���з�������ֵ
        *	\return ת���Ƿ�ɹ�
        */
        static unsigned long  StrToInt(const std::string &strValue);

protected:

/***************************************************
 ����enBase64�����ĸ���������
 �ܹ��������ַ�����Ϊ4���ַ���
 Return Value: 
	��
 Parameters:
	chasc��δ����Ķ����ƴ���
	chuue���������Base64����
 Remarks:
	����BASE64���룬���Բμ�RFC 1341
	����chascΪ��ֵ������Ϊ�������ַ�
 ***************************************************/
	static void enBase64Help(unsigned char chasc[3], unsigned char chuue[4]);

/***************************************************
 deBase64�����ĸ���������
 �ܹ����ĸ��ַ����ַ������н��롣
 Return Value: 
	��
 Parameters:
	chuue���������Base64����
	chasc�������Ķ����ƴ���
 Remarks:
	����BASE64���룬���Բμ�RFC 1341
	����chuue���м���Ƿ�Ϸ�
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
	*	\param	Low	    Size�ĵ��ֽ�
	*	\param	High    Size�ĸ��ֽ�
	*/
	ResourceSize(long Low, long High)
	{
		SizeLow = Low;
		SizeHigh = High;
	}
	/*!
	*	Operator +=
	*	\param	rs	    Ҫ�ӵĴ�С
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
