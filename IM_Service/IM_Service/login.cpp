#include "stdafx.h"
#include "login.h"
#include "./auth/http_parse.h"
#include "./chat_manager.h"
#include "./common/commclt.h"
#include "./auth/auth_error.h"

CLogin::CLogin()
{	
}

CLogin::~CLogin()
{
}

void CLogin::StartLogin(tstring loginName, tstring password, bool isAutoLogin, bool isKeepPwd)
{
	if (!CheckLoginInfo(loginName, password, isAutoLogin, isKeepPwd))
	{
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
		return;
	}

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
		return;
	}

	// 登录
	if (!LoginToRealServer(m_manager->m_server, m_manager->m_port, uin))
	{
	}
}

bool CLogin::CheckLoginInfo(tstring loginName, tstring password, bool isAutoLogin, bool isKeepPwd)
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
		m_pTqAuthClient = new CTqAuthClient(m_manager->m_InitConfig.sAuthAddr, m_manager->m_InitConfig.nAuthPort, VERSION);
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

	if (p.GetPostBodyParams(recvbuf, "uin", t))
	{
		uin = atol(t.c_str());
	}

	return nstate;
}

static HANDLE NameMutex = NULL;
static HANDLE UinMutex = NULL;
bool CLogin::CheckLoginFlag(unsigned long uin, const tstring& strID, bool bCreate, bool bFree)
{
	bool bRetVal = false;
	HANDLE hUinFlag = NULL, hNameFlag = NULL;

	if (uin != 0)
	{
		char strUin[MAX_256_LEN];
		sprintf_s(strUin, "_TQ_%lu_", uin);

		if (bCreate)
		{
			UinMutex = CreateMutex(NULL, FALSE, strUin);
		}
		else
		{
			hUinFlag = OpenMutex(MUTEX_ALL_ACCESS, FALSE, strUin);
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
			NameMutex = CreateMutex(NULL, FALSE, strName);
		}
		else
		{
			hNameFlag = OpenMutex(MUTEX_ALL_ACCESS, FALSE, strName);
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

bool CLogin::LoginToRealServer(tstring strServer, int nPort, unsigned int uin)
{
	int nError;

	if ((nError = ConnectToServer(strServer, nPort)) != 0)
		return false;

	if ((nError = SendLoginInfo(uin)) != 0)
		return false;
	return true;
}

int CLogin::ConnectToServer(tstring sip, unsigned short port)
{
	int nError = 0;

	for (int i = 0; i < 2; i++)
	{
		if (m_Socket.Connect(sip.c_str(), port))
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

	m_UserInfo.UserInfo.uid = uin;
	nError = m_manager->SendPackTo(&SendInfo);
	if (nError != 0)
	{
		goto FAIL;
	}

FAIL:

	return nError;
}