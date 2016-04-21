#ifndef _DBCONNECT_H
#define _DBCONNECT_H

#include "../StdAfx.h"
#include "sql.h"
#include "sqlext.h"
#include "time.h"
#include "../include/comdef.h"

#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

class CDBConnect
{
protected:
	//odbc 
	SQLHANDLE	m_hEnv;
	SQLHANDLE	m_hCon;

	//定义ADO连接、命令、记录集变量指针
	_ConnectionPtr	m_pConnection;

//	void CloseODBCEnv();
	bool CreateODBCEnv();

	char m_Error[256];
private:
	int m_nConnectType;

public:
	//连接出错了，需要重新连接
	bool m_bConnectError;

	CDBConnect();
	~CDBConnect();
	char *GetLastError();

	void CloseConnect();
	bool Connect(char *sDBCONNECT, char *sUser, char *sPass);

	bool ConnectODBC(char *sDBCONNECT, char *sUser, char *sPass);
	bool ConnectADO(char *sDBCONNECT, char *sUser, char *sPass);

//	HANDLE GetODBCEnvHandle();
	bool DBIsConnect();
	HANDLE GetODBCConnectHandle();
	_ConnectionPtr GetADOConnectHandle();

	void SetDBConnectType(int type);
	int GetDBConnectType();

};

class CDBRecordSet
{
private:
//	SQLHANDLE	m_hCon;

public:
	CDBRecordSet();
	~CDBRecordSet();

	void SetDBConnectType(int type);
	int GetDBConnectType();

	void SetLockTime(unsigned long ultime);
	unsigned long GetLockTime();

	void SetDBConnect(CDBConnect *pDBConnect);
	bool DBIsConnect();

private:
	CDBConnect *m_pDBConnect;
	int m_nConnectType;

	//odbc
	SQLHANDLE m_hStmt;
	//ado
	_RecordsetPtr m_pRecordset;

	char szTmpField[MAX_FIELD_LEN];

	int m_nCurField;
	int m_nCurRow;
	long num_row;
	int num_field;

	//锁定正在使用
	bool bLock;
	unsigned long ulLockTime;

	char m_Error[256];
	
	//用于ado
	BOOL bFirstBind;
public:
	void Lock();
	void Unlock();
	bool IsLock();

	bool Create();
	bool CreateStmtHandle();
	bool CreateADORecordSet();
	void FreeRecordSet();

	bool ExecSQL(char *src);
	long SelectData(char *src);
	bool BindData();
	bool FreeData();
	char *Fetch(int nMaxLen=1024);

	bool ExecSQLODBC(char *src);
	bool ExecSQLADO(char *src);

	long SelectDataODBC(char *src);
	long SelectDataADO(char *src);
	bool BindDataODBC();
	bool BindDataADO();
	char *FetchODBC(int nMaxLen=1024);
	char *FetchADO(int nMaxLen=1024);
//	bool FreeODBC();
//	char *FetchODBC(int nMaxLen=1024);

	time_t FetchDate();
	int  FetchInt();
	unsigned long FetchDWord();
	unsigned short FetchWord();
	unsigned char FetchByte();

	int GetRecordLength(char *src, bool bDirect=true);
	BOOL GetMaxID(char *src, unsigned long &maxid);

	bool CDBRecordSet::IsDBDisConnectType(SQLCHAR *SqlState);

private:
	struct tm ParseTimeString(char *stime);
//	time_t ParseTimeString(char *stime);

};

#endif