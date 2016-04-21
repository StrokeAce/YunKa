/************************************************************************

		Submodule	: log.h
		Depict		: Write log Interface
		Language	: ANSI C++
		Version		: 3.0
		Compile		: make
		Compat		: Linux/Windows
		Editor		: yanyg
		Time		: 2007 07 15

************************************************************************/

//log.h: interface for the CLog class.

/************************************************************************

		使	用	说	明

1. 确保使用前初始化;
2. 可自动创建任意多级目录且不会更改程序工作目录;
2. 该日志类已经定义一份名为g_WriteLog实体对象;
3. 该类支持C/C++两种输入;例如:
	int iExample(19831107);
	string strExample("19831107");
	g_WriteLog.WriteLog( C_LOG_SYSNOTICE , "日志类作者是闫永刚.以下是C用法案例:\r\n");
	g_WriteLog.WriteLog( C_LOG_SYSNOTICE , "i = %d ; str = %s .\r\n" , iExample , strExample.c_str() );
	g_WriteLog<<CPP_LOG_SYSNOTICE<<"日志类作者是闫永刚.";
	g_WriteLog<<CPP_LOG_SYSNOTICE<<"i = "<<iExample<<"; str = "<<strExample<<CPP_LOG_END;
   注 : 必须以CPP_LOG_END结尾,否则死锁!!!
4. 经测试该类写日志效率为 200000条/秒(C++流式输入) 和 125000条/秒(C式输入)
	测试信息:
		系统配置 : CPU 2.4HZ / MEM 512M 
		每条字节 : 68Byte
		操作系统 : Linux Redhat9

************************************************************************/

#ifndef __WRITELOG_H_
#define __WRITELOG_H_

#include "mutex.h"

#include <string>
#include <fstream>

enum
{
	C_LOG_NONE = 0 ,		//	no any log will be write
	C_LOG_SYSNOTICE = 1 ,		//	system informational notices
	C_LOG_FATAL = 2 ,		//	fatal errors
	C_LOG_ERROR = 3 ,		//	error!
	C_LOG_WARNING = 4 ,		//	exceptional events
	C_LOG_NOTICE = 5 ,		//	informational notices
	C_LOG_DEBUG = 6 ,		//	full debugging
	C_LOG_TRACE = 7 ,		//	program tracing  
	C_LOG_ALL = 8 ,			//	everything
};

#define CPP_LOG_NONE		CLogLevel<C_LOG_NONE>()		//	no any log will be write
#define CPP_LOG_SYSNOTICE	CLogLevel<C_LOG_SYSNOTICE>()	//	system informational notices
#define CPP_LOG_FATAL		CLogLevel<C_LOG_FATAL>()	//	fatal errors
#define CPP_LOG_ERROR		CLogLevel<C_LOG_ERROR>()	//	error!
#define CPP_LOG_WARNING		CLogLevel<C_LOG_WARNING>()	//	exceptional events
#define CPP_LOG_NOTICE		CLogLevel<C_LOG_NOTICE>()	//	informational notices
#define CPP_LOG_DEBUG		CLogLevel<C_LOG_DEBUG>()	//	full debugging
#define CPP_LOG_TRACE		CLogLevel<C_LOG_TRACE>()	//	program tracing
#define CPP_LOG_ALL		CLogLevel<C_LOG_ALL>()		//	everything
#define CPP_LOG_END		CLogUnLockType()		//	unLock the log stream
#define CPP_LOG_FLUSH_END		CLogFlushUnLockType()		//	 flush and unLock the log stream

template<int ilevel>struct CLogLevel{};
struct CLogUnLockType{};
struct CLogFlushUnLockType{};

class CLog
{
public:
	CLog():m_bWrite(false),m_iLogDay(-1),m_iFlush(C_LOG_ALL){}

	void SetLogLevel( unsigned int uiLogLevel ) { m_uiLogLevel = uiLogLevel <= C_LOG_ALL ? uiLogLevel : C_LOG_ALL; }
	bool InitLog( const std::string& strLogPath , const std::string& strLogName , unsigned int uiLogLevel );
	void WriteLog( unsigned int uiLogLevel , const char* fmt , ... );
	void WriteStr(  const char* strerror ,unsigned int uiLogLevel);


	template< typename T > CLog& operator<<( const T& t ) { if(m_bWrite)m_LogStream<<t; return *this;  }
	CLog& operator<<( const CLogUnLockType ) { return m_LogLock.Unlock(), *this; }
	CLog& operator<<( const CLogFlushUnLockType ) { return m_LogStream.flush(), m_LogLock.Unlock(), *this; }

	template<int iLevel> CLog& operator<<( const CLogLevel<iLevel> ) { return m_LogLock.Lock(),MakeLogTag(iLevel),*this; }

private:
	void MakeLogTag(const unsigned int iLevel );
	bool CreateDir( );
private:
	bool m_bWrite;
	unsigned int m_uiLogLevel;	//日志级别
	int m_iLogDay;			//日期(天):每天更新一次
	int m_iFlush;			//刷新级别
	std::string m_strLogPath;	//日志路径
	std::string m_strlogName;	//日志名
	std::string m_strAllName;	//日志全名 : path+(year+month+day)+name
	std::string m_strBakeName;	//第二天移动第一天日志到bake
	std::ofstream m_LogStream;	//日志文件流
	MMutex m_LogLock;		//Linux日志文件锁
};

extern CLog g_WriteLog,g_VisitLog;

#endif
