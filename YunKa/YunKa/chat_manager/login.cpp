#include "../stdafx.h"
#include "login.h"
#include "http_parse.h"
#include "./chat_manager.h"
#include "../chat_common/comclt.h"
#include "auth_error.h"

CLogin::CLogin()
{	
	m_nLoginBy = -1;
	m_authAdminid = 0;
	memset(m_szAuthtoken, 0, MAX_256_LEN + 1);
	m_manager = NULL;
	m_pTqAuthClient = NULL;
}

CLogin::~CLogin()
{
}

void CLogin::StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{
	if (!CheckLoginInfo(loginName, password, isAutoLogin, isKeepPwd))
	{
		m_manager->m_handlerLogin->LoginProgress(-1);
		return;
	}

	unsigned int uin(0);

	if (IsNumber(loginName))
	{
		uin = atol(loginName.c_str());
	}

	// 认证
	if (GetTqAuthToken(uin, loginName.c_str(), password.c_str()) != 1)
	{
		m_manager->m_lastError = "认证失败";
		m_manager->m_handlerLogin->LoginProgress(-1);
		return;
	}

	m_manager->m_handlerLogin->LoginProgress(20);

	if (!IsNumber(loginName))
	{
		m_nLoginBy = LOGIN_BYSTRING;
	}
	else
	{
		m_nLoginBy = LOGIN_BYUID;
	}

	if (CheckLoginFlag(uin, loginName))
	{
		m_manager->m_lastError = "该帐号在本地已经登录";
		m_manager->m_handlerLogin->LoginProgress(-1);
		return;
	}

	// 登录
	if (!LoginToRealServer(m_manager->m_server, m_manager->m_port, uin))
	{
		m_manager->m_handlerLogin->LoginProgress(-1);
	}
}

bool CLogin::CheckLoginInfo(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{
	char str[MAX_256_LEN];

	if (loginName.empty())
	{
		m_manager->m_lastError = "登录名不能为空!";
		return false;
	}
	
	if (password.length() > MAX_PASSWORD_LEN)
	{
		sprintf(str, "密码超过其最大的长度：%d个字符", MAX_PASSWORD_LEN);
		m_manager->m_lastError = str;
		return false;
	}
	return true;
}

int CLogin::GetTqAuthToken(unsigned int &uin, const char *szStrid, const char *szPassWord)
{
	int nlen = MAX_4096_LEN;
	char recvbuf[MAX_4096_LEN] = { 0 };
	bool butf8(true);
	char myip[100];

	if (this->m_pTqAuthClient == NULL)
		m_pTqAuthClient = new CTqAuthClient(m_manager->m_initConfig.sAuthAddr, m_manager->m_initConfig.nAuthPort, VERSION);
	else if (strlen(m_szAuthtoken) > 0)
	{
		m_pTqAuthClient->Logout(m_szAuthtoken, recvbuf, nlen, butf8);
		m_szAuthtoken[0] = 0;
	}

	GetLocalHost(myip);

	nlen = 4096;
	int nstate = SESSION_ACTION_ERR_NOTCONNECT;
	if (0 != m_pTqAuthClient->Login(uin, szStrid, szPassWord, 1, myip, recvbuf, nlen, butf8))
	{
		return nstate;
	}

	CHttpParse  p;
	p.ParseHead(recvbuf, nlen);

	string t;
	if (!p.GetPostBodyParams(recvbuf, "state", t))
	{
		return nstate;
	}

	nstate = atoi(t.c_str());
	if (nstate != 1)
	{
		return nstate;
	}

	if (p.GetPostBodyParams(recvbuf, "token", t))
	{
		strncpy(m_szAuthtoken, t.c_str(), MAX_256_LEN);
	}

	if (p.GetPostBodyParams(recvbuf, "adminid", t))
	{
		m_authAdminid = atol(t.c_str());
	}

	if (p.GetPostBodyParams(recvbuf, "uin", t))
	{
		uin = atol(t.c_str());
	}

	return nstate;
}

static HANDLE NameMutex = NULL;
static HANDLE UinMutex = NULL;
bool CLogin::CheckLoginFlag(unsigned long uin, const string& strID, bool bCreate, bool bFree)
{
	bool bRetVal = false;
	HANDLE hUinFlag = NULL, hNameFlag = NULL;

	if (uin != 0)
	{
		char strUin[MAX_256_LEN];
		sprintf_s(strUin, "_TQ_%lu_", uin);

		if (bCreate)
		{
			UinMutex = CreateMutexA(NULL, FALSE, strUin);
		}
		else
		{
			hUinFlag = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, strUin);
		}

		if (hUinFlag)
		{
			bRetVal = TRUE;
			if (hUinFlag)
			{
				CloseHandle(hUinFlag);
				hUinFlag = NULL;
			}
		}
	}

	if (!strID.empty())
	{
		char strName[MAX_256_LEN];
		sprintf_s(strName, "_TQ_%s_", strID);

		if (bCreate)
		{
			NameMutex = CreateMutexA(NULL, FALSE, strName);
		}
		else
		{
			hNameFlag = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, strName);
		}

		if (hNameFlag)
		{
			bRetVal = TRUE;
			if (hNameFlag)
			{
				CloseHandle(hNameFlag);
				hNameFlag = NULL;
			}
		}
	}

	if (bFree)
	{
		if (UinMutex)
		{
			CloseHandle(UinMutex);
			UinMutex = NULL;
		}

		if (NameMutex)
		{
			CloseHandle(NameMutex);
			NameMutex = NULL;
		}
	}

	return bRetVal;
}

bool CLogin::LoginToRealServer(string strServer, int nPort, unsigned int uin)
{
	int nError;

	if ((nError = ConnectToServer(strServer, nPort)) != 0)
		return false;

	m_manager->m_handlerLogin->LoginProgress(40);

	if ((nError = SendLoginInfo(uin)) != 0)
		return false;

	m_manager->m_handlerLogin->LoginProgress(60);
	return true;
}

int CLogin::ConnectToServer(string sip, unsigned short port)
{
	int nError = 0;

	m_manager->m_socket.Close();

	for (int i = 0; i < 2; i++)
	{
		if (m_manager->m_socket.Connect(sip.c_str(), port))
		{
			return nError;
		}
	}

	nError = SYS_ERROR_SOCKETCONNECTFAIL;
	return nError;
}

int CLogin::SendLoginInfo(unsigned int uin)
{
	int nError = 0;

	CLT_LOGON SendInfo(VERSION);


	SendInfo.uin = uin;
	strcpy(SendInfo.strid, "");
	SendInfo.type_loginby = LOGIN_BYUID;

	strcpy(SendInfo.pwd, m_manager->m_password.c_str());

	SendInfo.status = STATUS_ONLINE;
	SendInfo.langtype = 0;
	SendInfo.bak = 0;


	SendInfo.terminaltype = TERMINAL_PCEX;
	SendInfo.connecttype = MSG_TCPUI;

	SendInfo.type_loginsort = 10;

	m_manager->m_userInfo.UserInfo.uid = uin;
	nError = m_manager->SendPackTo(&SendInfo);
	if (nError != 0)
	{
		goto FAIL;
	}

FAIL:

	return nError;
}

void CLogin::TimerSolveAuthToken()
{
	if (m_nSendAuthToken < 0)
		return;
	m_nSendAuthToken++;

	if (m_nSendAuthToken > 600)	//1800 大约30分钟 -> 改为600秒10分钟
	{
		if (m_pTqAuthClient != NULL && strlen(m_szAuthtoken) > 0)
		{

			int nlen = 4096;
			char recvbuf[4096];
			bool butf8(true);

			m_pTqAuthClient->Hearbeat(m_szAuthtoken, recvbuf, nlen, butf8);
			printf("%d,%s\n", butf8, recvbuf);
		}

		m_nSendAuthToken = 0;
	}
}
