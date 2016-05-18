/************************************************************************

	Submodule	: log.cpp
	Depict		: Write Log Implementation
	Language	: ANSI C++
	Version		: 3.0
	Compile		: make 
	Compat		: Linux/Windows
	Editor		: yanyg
	Time		: 2007 07 15

************************************************************************/

//log.cpp: implementation of the CLog class.
#include "../stdafx.h"

#ifdef WIN32
#include <io.h>
#include <direct.h>
#pragma   warning (disable:4786)
#define  vsnprintf _vsnprintf

#else

#include <dirent.h>
#include <unistd.h>
#endif

#include "winlog.h"
#include "dirfileop.h"
#include <cstdarg>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

CLog g_WriteLog,g_VisitLog;

namespace LogTag
{
	int Log_Level[] =
	{ 
		C_LOG_NONE ,
		C_LOG_SYSNOTICE ,
		C_LOG_FATAL ,
		C_LOG_ERROR ,
		C_LOG_WARNING ,
		C_LOG_NOTICE ,
		C_LOG_DEBUG ,
		C_LOG_TRACE ,
		C_LOG_ALL
	};

	std::string Log_Notice[] = 
	{
		"\r\nNONE ",
		"\r\nSYSNOTICE ",
		"\r\nFATAL ",
		"\r\nERROR ",
		"\r\nWARNING ",
		"\r\nNOTICE ",
		"\r\nDEBUG ",
		"\r\nTRACE ",
		"\r\nALL "
	};
};

bool CLog::InitLog( const std::string& strLogPath , const std::string& strLogName , unsigned int uiLogLevel )
{
	m_LogStream.close();
	m_LogStream.clear();
	m_iLogDay=0;

	m_strLogPath = ( strLogPath.empty() ? "./" : *strLogPath.rbegin() == '/' ? strLogPath : (strLogPath+'/') );
	m_strlogName = strLogName;
	m_uiLogLevel = (uiLogLevel<C_LOG_ALL?uiLogLevel:C_LOG_ALL);

	return CreateDir();
}

bool CLog::CreateDir()
{
	std::string strDirName;
	std::string::size_type idxBeg(0);
	std::string::size_type idxEnd;

	while ( true )
	{
		idxEnd = m_strLogPath.find('/',idxBeg);

		if ( std::string::npos == idxEnd )
		{
			strDirName.assign(m_strLogPath);
		}
		else
		{
			strDirName.assign(m_strLogPath,0,idxEnd);
			idxBeg = idxEnd + 1;
		}

		MakeDirectory(strDirName.c_str());
		//mkdir( ,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		if( std::string::npos == idxEnd && idxBeg >= m_strLogPath.length() )
		{
			break;
		}
	}


		return true;
}

enum{ BUFF_SIZE = 8000 };
void CLog::WriteLog( unsigned int uiLogLevel , const char* fmt , ... )
{
	m_LogLock.Lock();

	try
	{
		MakeLogTag(uiLogLevel);

		if ( m_bWrite )
		{
			char buff[BUFF_SIZE+1]={0};
			va_list args;
			va_start( args , fmt);
			if (0>_vsnprintf_s(buff, BUFF_SIZE, fmt, args))
			{
				va_end(args);
				throw 0;
			}
			va_end(args);

			m_LogStream<<buff;
			m_LogStream.flush();
		}
	}
	catch (...)
	{
	}
	m_LogLock.Unlock();
}

void CLog::WriteStr(  const char* strerror ,unsigned int uiLogLevel)
{
	m_LogLock.Lock();
	try{

		MakeLogTag(uiLogLevel);
		m_LogStream<<strerror;
		m_LogStream.flush();
	}catch (...)
	{
	}
	m_LogLock.Unlock();

}


void CLog::MakeLogTag( const unsigned int uiLevel )
{
	m_bWrite = (uiLevel <= m_uiLogLevel);

	if ( m_bWrite )
	{
		tm tmnow;

	//Make struct tm value, check file , or change filename and etc...
		time_t tinow(time(NULL));

#ifndef WIN32
		localtime_r(&tinow,&tmnow);
#else
		localtime_s(&tmnow, &tinow);
#endif
		if (0 != _access(m_strAllName.c_str(), 0))
		{
			m_LogStream.close();
		}

		if ( m_iLogDay != tmnow.tm_mday )
		{
			//close
			m_LogStream.close();

			//move log to bake directory
			//file has exist , need bake...
			MakeDirectory(m_strBakeName.c_str());

			rename(m_strAllName.c_str(),m_strBakeName.c_str());

			m_iLogDay = tmnow.tm_mday;
			char str_time[128];
			strftime(str_time,128,"%y%m%dT%H%M", &tmnow);
			m_strAllName = m_strLogPath + str_time + m_strlogName;
			m_strBakeName = m_strLogPath + "bak/" + str_time + m_strlogName + ".bak";
		}

		if ( !m_LogStream.good() )
		{
			m_LogStream.close();
			m_LogStream.clear();
		}

		if ( !m_LogStream.is_open())
		{
			 m_LogStream.open(m_strAllName.c_str(),std::ios::out|std::ios::app);
			 m_LogStream << "name:" << m_strAllName.c_str() << "\r\nbak name:" << m_strBakeName.c_str() << "\r\n";
		}
		m_bWrite =  m_LogStream.is_open() ;

		//Write:
		if ( m_bWrite ) 
		{
			m_LogStream<<LogTag::Log_Notice[uiLevel].c_str()
			<<tmnow.tm_mon+1<<"-"<<tmnow.tm_mday<<"T"<<tmnow.tm_hour<<":"<<tmnow.tm_min<<":"<<tmnow.tm_sec<<"\t";
		}
	}
}

/******************************* END ***********************************/
