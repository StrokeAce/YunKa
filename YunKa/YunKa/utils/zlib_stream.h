#pragma once

#include <zconf.h>
#include <zlib.h>
#include "winlog.h"

using namespace std;

#ifdef _WIN32
#pragma comment(lib,"zdll")
#endif

#define MaxTempBuflen  4096

//压缩
class CZlibDeflate
{
public:
	CZlibDeflate():m_bInit(false)
	{
		Init();
	}
	~CZlibDeflate()
	{
		if (m_bInit)
		{
			deflateEnd(&m_strm);
			m_bInit=false;
		}
	}
		
	template<typename T> 
	int DeFlateData(const char* pdata,uInt uLen,T &szOut)
	{
		char      m_tempbuf[MaxTempBuflen+1]={0};
		m_strm.avail_in=uLen;
		m_strm.next_in = (Bytef*)pdata;

		do{
			m_strm.avail_out = MaxTempBuflen;
			m_strm.next_out = (Bytef*)m_tempbuf;
			int nret = deflate(&m_strm,Z_SYNC_FLUSH);  
			if (Z_OK!=nret)
			{
				g_WriteLog.WriteLog(C_LOG_ERROR,"deflate failed:%d.%s",nret,pdata);
				return nret;
			}else
			{
				size_t len = MaxTempBuflen - m_strm.avail_out;
				szOut+=string(m_tempbuf,len);
			}

		}while(0==m_strm.avail_out);

		return Z_OK;
	}


	bool  Init()
	{
		if (m_bInit)
		{
			deflateEnd(&m_strm);
			m_bInit=false;
		}
		m_strm.zalloc = Z_NULL;
		m_strm.zfree = Z_NULL;
		m_strm.opaque = Z_NULL;

		/*
		deflateInit returns Z_OK if success,
		Z_MEM_ERROR if there was not enough memory,
		Z_STREAM_ERROR if level is not a valid compression level,
		Z_VERSION_ERROR if the zlib library version (zlib_version) is incompatible with the version assumed by the caller (ZLIB_VERSION).
		msg is set to null if there is no error message. deflateInit does not perform any compression: this will be done by deflate(). 
		*/
		int nret = deflateInit(&m_strm, Z_DEFAULT_COMPRESSION); // 1 效率最好 9压缩率最好
		if (Z_OK!=nret)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR,"deflateInit failed:%d",nret);
			return false;
		}

		m_bInit=true;
		return true;

	}
private:

	bool      m_bInit;
	z_stream  m_strm;
};


//解压缩
class CZlibInflate
{
public:
	CZlibInflate()
		:m_bInit(false)
	{
		Init();
	}
	~CZlibInflate()
	{
		if (m_bInit)
		{
			inflateEnd(&m_strm);
			m_bInit=false;
		}
	}

	template<typename T> 
	int InflateData(const char* pdata,uInt nlen,T &szOut)
	{
		if (0==nlen)
		{
			return Z_OK;
		}
		char      m_tempbuf[MaxTempBuflen+1]={0};
		m_strm.avail_in=nlen;
		m_strm.next_in=(Bytef*)pdata;

		do 
		{
			m_strm.avail_out = MaxTempBuflen;
			m_strm.next_out = (Bytef*)m_tempbuf;
			int nret = inflate(&m_strm, Z_SYNC_FLUSH );

			if (nret!=Z_OK)
			{
				g_WriteLog.WriteLog(C_LOG_ERROR,"inflate failed:%d,%s",nret,m_strm.msg);
				return nret;
			}
			size_t len = MaxTempBuflen - m_strm.avail_out;
			szOut+=string(m_tempbuf,len);

		} while (m_strm.avail_out==0);

		return Z_OK;
	}

	bool Init()
	{
		if (m_bInit)
		{
			inflateEnd(&m_strm);
			m_bInit=false;
		}

		m_strm.zalloc = Z_NULL;
		m_strm.zfree = Z_NULL;
		m_strm.opaque = Z_NULL;
		m_strm.avail_in = 0;
		m_strm.next_in = Z_NULL;
		int nret = inflateInit(&m_strm);
		if (Z_OK!=nret)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR,"inflateInit failed:%d",nret);
			return false;
		}

		m_bInit=true;
		return true;
	}
private:

	bool      m_bInit;
	z_stream  m_strm;
};

