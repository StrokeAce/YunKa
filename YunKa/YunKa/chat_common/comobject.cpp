#include "comobject.h"
#include  <io.h>
#include <fstream>
#include <process.h>
#include "comfunc.h"


CSysConfigFile::CSysConfigFile()
{
	ResetValue();
}

CSysConfigFile::~CSysConfigFile()
{
	DeleteAllLoginInfo();
	DeleteAllAlertInfo();
}

LOGIN_INFO* CSysConfigFile::GetLoginInfo(int type, unsigned long uid, string sid)
{
	LOGIN_INFO* loginInfo = NULL;

	if ((type == LOGIN_BYUID && uid != 0) || (type == LOGIN_BYSTRING && !sid.empty()))
	{
		ListLoginedInfo::iterator iter;
		for (iter = m_cLoginInfoList.begin(); iter != m_cLoginInfoList.end(); ++iter)
		{
			if (type == LOGIN_BYUID)
			{
				if ((*iter)->uid == uid && uid != 0)
				{
					loginInfo = (*iter);
				}
			}
			else if (type == LOGIN_BYSTRING)
			{
				if ((*iter)->sid == sid)
				{
					loginInfo = (*iter);
				}
			}
		}
	}
	return loginInfo;
}

void CSysConfigFile::ResetValue()
{
	m_bSavePass = false;
	m_bAutoLogin = false;
	m_nKeySendType = 0;
	
	SetWndInitPos(true);

	m_nSendMsgHeight = 100;
	m_nPreSendMsgHeight = 35;

	m_nLastLoginBy = LOGIN_BYSTRING;

	m_sLastLoginUid = 0;
	m_sLastLoginStr = "";
	m_sLastLoginPass = "";

	m_sStrServer = "tcp01.tq.cn";
	m_nServerPort = 443;

	m_sVisitorServer = "vipwebscreen.tq.cn";
	m_nVisitorServerPort = EMVISITOR_PORT;

	m_sStrRealServer = "tcp01.tq.cn";
	m_nRealServerPort = 443;

	m_cLoginInfoList.clear();

	SetAllDefaultAlertInfo();

	m_cServerAddressList.clear();
	m_cKeyWordSearchList.clear();
	m_cForbidWebUserList.clear();

	memset(&m_cWebUserfilter, '\0', sizeof(FILTER_USERDEFINE));
	m_nFilterType = VISITORFILTER_ALL;

	m_bAutoResp = false;
	m_sWellcomeMsg = "您好，我是在线客服，很高兴为您服务。";

	m_nUserTimeoutTime = 6;
	m_sUserTimeoutMsg = "您好，我有事暂时离开，您可以留下联系方式或留言，我会尽快给您回复。";

	m_nVisitorTimeoutTime = 8;
	m_sVisitorTimeoutMsg = "还在吗？有什么问题尽管告诉我，我会尽力解决您的问题。";

	m_bVisotorTimeoutClose = false;
	m_nVisitorTimeoutCloseTime = 10;
	m_sVisitorTimeoutCloseMsg = "已经很久没有收到您的讯息，请问您还在电脑旁吗？如果没有其它问题，系统将主动结束本次对话。<br>如果有问题，欢迎您再次找我咨询，祝您今天工作好心情，再见。";

	m_bAutoRespUnnormalStatus = false;
	m_sUnnormalStatusMsg = "您好，我有事暂时离开，您可以留下联系方式或留言，我会尽快给您回复。";

	m_sInviteWords = "您好！我能为您做点什么?";

	//保存自定义邀请语
	m_cInviteWordsList.clear();
	m_cInviteWordsList.push_back("欢迎光临！");
	m_cInviteWordsList.push_back("您好，欢迎访问本网站！");
	m_cInviteWordsList.push_back("能和您聊聊吗？");
	m_cInviteWordsList.push_back("您好，如您遇到什么问题可以直接与我交流！");
	m_cInviteWordsList.push_back("您好，能为您做些什么呢？");
	m_cInviteWordsList.push_back("您好，我是网站管理员！");

	m_nInviteType = INVITE_USERDEFINE;

	m_nKeywordsSort = 0;
	m_nKeywordsUser = 0;
	m_sKeywordsFind = "";
}

bool CSysConfigFile::ReadFile(ifstream& fout)
{
	byte count = 0;

	Read(fout, m_bSavePass);
	Read(fout, m_bAutoLogin);
	Read(fout, m_nKeySendType);
	Read(fout, m_nX);
	Read(fout, m_nY);
	Read(fout, m_nWidth);
	Read(fout, m_nHeight);
	Read(fout, m_nSendMsgHeight);
	Read(fout, m_nPreSendMsgHeight);
	Read(fout, m_nLastLoginBy);
	Read(fout, m_sLastLoginUid);
	Read(fout, m_sLastLoginStr);
	Read(fout, m_sLastLoginPass);
	Read(fout, m_sStrServer);
	Read(fout, m_nServerPort);
	Read(fout, m_sVisitorServer);
	Read(fout, m_nVisitorServerPort);
	Read(fout, m_sStrRealServer);
	Read(fout, m_nRealServerPort);

	// 登录过的信息
	DeleteAllLoginInfo();
	Read(fout, count);
	for (int i = 0; i < (int)count; i++)
	{
		LOGIN_INFO *plg = new LOGIN_INFO();
		Read(fout, plg->uid);
		Read(fout, plg->sid);
		Read(fout, plg->pass);
		Read(fout, plg->compid);
		m_cLoginInfoList.push_back(plg);
	}
	
	DeleteAllAlertInfo();
	Read(fout, count);
	for (int i = 0; i < (int)count; i++)
	{
		ALERT_INFO *par = new ALERT_INFO();
		Read(fout, par->type);
		Read(fout, par->tray);
		Read(fout, par->showwnd);
		Read(fout, par->sound);
		Read(fout, par->soundfilename);
		m_cAlertInfoList.push_back(par);
	}

	m_cServerAddressList.clear();
	Read(fout, count);
	for (int i = 0; i < (int)count; i++)
	{
		string address;
		Read(fout, address);
		m_cServerAddressList.push_back(address);
	}

	m_cKeyWordSearchList.clear();
	Read(fout, count);
	for (int i = 0; i < (int)count; i++)
	{
		string address;
		Read(fout, address);
		m_cKeyWordSearchList.push_back(address);
	}

	m_cForbidWebUserList.clear();
	Read(fout, count);
	for (int i = 0; i < (int)count; i++)
	{
		string address;
		Read(fout, address);
		m_cForbidWebUserList.push_back(address);
	}

	Read(fout, m_cWebUserfilter.num);
	Read(fout, m_cWebUserfilter.include);
	for (int i = 0; i < m_cWebUserfilter.num; i++)
	{
		Read(fout, m_cWebUserfilter.text[i]);
	}

	Read(fout, m_nFilterType);
	Read(fout, m_bAutoResp);
	Read(fout, m_sWellcomeMsg);
	Read(fout, m_nUserTimeoutTime);
	Read(fout, m_sUserTimeoutMsg);
	Read(fout, m_nVisitorTimeoutTime);
	Read(fout, m_sVisitorTimeoutMsg);
	Read(fout, m_bVisotorTimeoutClose);
	Read(fout, m_nVisitorTimeoutCloseTime);
	Read(fout, m_sVisitorTimeoutCloseMsg);
	Read(fout, m_bAutoRespUnnormalStatus);
	Read(fout, m_sUnnormalStatusMsg);
	Read(fout, m_sInviteWords);

	m_cInviteWordsList.clear();
	Read(fout, count);
	for (int i = 0; i < (int)count; i++)
	{
		string address;
		Read(fout, address);
		m_cInviteWordsList.push_back(address);
	}

	Read(fout, m_nInviteType);
	Read(fout, m_nKeywordsSort);
	Read(fout, m_nKeywordsUser);
	Read(fout, m_sKeywordsFind);

	return false;
}

bool CSysConfigFile::WriteFile(ofstream& fin)
{
	byte count = 0;
	Write(fin, m_bSavePass);
	Write(fin, m_bAutoLogin);
	Write(fin, m_nKeySendType);
	Write(fin, m_nX);
	Write(fin, m_nY);
	Write(fin, m_nWidth);
	Write(fin, m_nHeight);
	Write(fin, m_nSendMsgHeight);
	Write(fin, m_nPreSendMsgHeight);
	Write(fin, m_nLastLoginBy);
	Write(fin, m_sLastLoginUid);
	Write(fin, m_sLastLoginStr.c_str());
	Write(fin, m_sLastLoginPass.c_str());
	Write(fin, m_sStrServer.c_str());
	Write(fin, m_nServerPort);
	Write(fin, m_sVisitorServer.c_str());
	Write(fin, m_nVisitorServerPort);
	Write(fin, m_sStrRealServer.c_str());
	Write(fin, m_nRealServerPort);

	// 登录信息列表
	Write(fin, (byte)m_cLoginInfoList.size());
	ListLoginedInfo::iterator iter_login = m_cLoginInfoList.begin();
	for (iter_login; iter_login != m_cLoginInfoList.end(); iter_login++)
	{
		Write(fin, (*iter_login)->uid);
		Write(fin, (*iter_login)->sid);
		Write(fin, (*iter_login)->pass);
		Write(fin, (*iter_login)->compid);
	}

	// 提示音信息
	Write(fin, (byte)m_cAlertInfoList.size());
	ListAlertInfo::iterator iter_alert = m_cAlertInfoList.begin();
	for (iter_alert; iter_alert != m_cAlertInfoList.end(); iter_alert++)
	{
		Write(fin, (*iter_alert)->type);
		Write(fin, (*iter_alert)->tray);
		Write(fin, (*iter_alert)->showwnd);
		Write(fin, (*iter_alert)->sound);
		Write(fin, (*iter_alert)->soundfilename);
	}

	// 服务器地址列表
	Write(fin, (byte)m_cServerAddressList.size());
	list<string>::iterator iter_Address = m_cServerAddressList.begin();
	for (iter_Address; iter_Address != m_cServerAddressList.end(); iter_Address++)
	{
		Write(fin, (*iter_Address).c_str());
	}

	Write(fin, (byte)m_cKeyWordSearchList.size());
	list<string>::iterator iter_wordSearch = m_cKeyWordSearchList.begin();
	for (iter_wordSearch; iter_wordSearch != m_cKeyWordSearchList.end(); iter_wordSearch++)
	{
		Write(fin, (*iter_wordSearch).c_str());
	}

	Write(fin, (byte)m_cForbidWebUserList.size());
	list<string>::iterator iter_forbidWebUser = m_cForbidWebUserList.begin();
	for (iter_forbidWebUser; iter_forbidWebUser != m_cForbidWebUserList.end(); iter_forbidWebUser++)
	{
		Write(fin, (*iter_forbidWebUser).c_str());
	}

	if (m_cWebUserfilter.num > MAX_STRINGFILTER_NUM)
		m_cWebUserfilter.num = MAX_STRINGFILTER_NUM;
	Write(fin, m_cWebUserfilter.num);
	Write(fin, m_cWebUserfilter.include);

	for (int i = 0; i < m_cWebUserfilter.num; i++)
	{
		string tempStr = m_cWebUserfilter.text[i];
		Write(fin, tempStr.c_str());
	}

	Write(fin, m_nFilterType);
	Write(fin, m_bAutoResp);
	Write(fin, m_sWellcomeMsg.c_str());
	Write(fin, m_nUserTimeoutTime);
	Write(fin, m_sUserTimeoutMsg.c_str());
	Write(fin, m_nVisitorTimeoutTime);
	Write(fin, m_sVisitorTimeoutMsg.c_str());
	Write(fin, m_bVisotorTimeoutClose);
	Write(fin, m_nVisitorTimeoutCloseTime);
	Write(fin, m_sVisitorTimeoutCloseMsg.c_str());
	Write(fin, m_bAutoRespUnnormalStatus);
	Write(fin, m_sUnnormalStatusMsg.c_str());
	Write(fin, m_sInviteWords.c_str());

	Write(fin, (byte)m_cInviteWordsList.size());
	list<string>::iterator iter_invite = m_cInviteWordsList.begin();
	for (iter_invite; iter_invite != m_cInviteWordsList.end(); iter_invite++)
	{
		Write(fin, (*iter_invite).c_str());
	}

	Write(fin, m_nInviteType);
	Write(fin, m_nKeywordsSort);
	Write(fin, m_nKeywordsUser);
	Write(fin, m_sKeywordsFind.c_str());

	return false;
}

bool CSysConfigFile::LoadData(char *sFilename)
{
	assert(sFilename != NULL && strlen(sFilename) > 0);

	if (sFilename != NULL && strlen(sFilename) > 0)
	{
		strcpy(m_sConfigname, sFilename);
	}	

	if (_access(m_sConfigname, 0) == -1)
	{
		return false;
	}

	ifstream outfile(sFilename, std::ios::binary);
	if (!outfile)
	{
		return false;
	}

	if (ReadFile(outfile))
	{
		outfile.close();
		return false;
	}

	outfile.close();

	if (m_sStrServer.empty())
	{
		m_sStrServer = "tcp01.tq.cn";
	}

	if (m_nServerPort == 0)
	{
		m_nServerPort = 443;
	}
	return true;
}

bool CSysConfigFile::SaveData(char *sFilename)
{
	if (sFilename != NULL)
	{
		strcpy(m_sConfigname, sFilename);
	}

	assert(m_sConfigname);

	char sTempFile[256];
	sprintf(sTempFile, "%s.bak", m_sConfigname);

	ofstream infile(sTempFile, std::ios::binary);
	if (!infile)
	{
		return false;
	}

	if (!WriteFile(infile))
	{
		infile.close();
		if (CopyFileA(sTempFile, m_sConfigname, false))
		{
			DeleteFileA(sTempFile);
			return true;
		}
		else
		{
			return false;
		}
	}

	infile.close();
	return true;
}

ALERT_INFO *CSysConfigFile::SetAllDefaultAlertInfo(int type)
{
	DeleteAllAlertInfo();

	string strPath = GetCurrentPath();
	int pos;
	if ((pos = strPath.rfind("\\")) != string::npos)
		strPath = strPath.substr(0, pos);
	strPath += "\\sound";

	ALERT_INFO *pInfo, *pRtn;

	pRtn = NULL;
	if (type == -1 || type == SOUND_USERONLINE)
	{
		pInfo = new ALERT_INFO();
		memset(pInfo, '\0', sizeof(ALERT_INFO));
		pInfo->type = SOUND_USERONLINE;
		pInfo->sound = 0;
		pInfo->tray = 0;
		pInfo->showwnd = 0;
		sprintf(pInfo->soundfilename, "%s%s", strPath.c_str(), "\\online.wav");
		m_cAlertInfoList.push_back(pInfo);

		if (type != -1)
		{
			pRtn = pInfo;
		}
	}

	if (type == -1 || type == SOUND_INVITE)
	{
		pInfo = new ALERT_INFO;
		memset(pInfo, '\0', sizeof(ALERT_INFO));
		pInfo->type = SOUND_INVITE;
		pInfo->sound = 0;
		pInfo->tray = 0;
		pInfo->showwnd = 0;
		sprintf(pInfo->soundfilename, "%s%s", strPath.c_str(), "\\invite.wav");
		m_cAlertInfoList.push_back(pInfo);
		if (type != -1)
		{
			pRtn = pInfo;
		}
	}

	if (type == -1 || type == SOUND_MSG)
	{
		pInfo = new ALERT_INFO;
		memset(pInfo, '\0', sizeof(ALERT_INFO));
		pInfo->type = SOUND_MSG;
		pInfo->sound = 0;
		pInfo->tray = 0;
		pInfo->showwnd = 0;
		sprintf(pInfo->soundfilename, "%s%s", strPath.c_str(), "\\msg.wav");
		m_cAlertInfoList.push_back(pInfo);
		if (type != -1)
		{
			pRtn = pInfo;
		}
	}

	if (type == -1 || type == SOUND_RING)
	{
		pInfo = new ALERT_INFO;
		memset(pInfo, '\0', sizeof(ALERT_INFO));
		pInfo->type = SOUND_RING;
		pInfo->sound = 1;
		pInfo->tray = 1;
		pInfo->showwnd = 0;
		sprintf(pInfo->soundfilename, "%s%s", strPath.c_str(), "\\ring.wav");
		m_cAlertInfoList.push_back(pInfo);
		if (type != -1)
		{
			pRtn = pInfo;
		}
	}

	if (type == -1 || type == SOUND_ALERT)
	{
		pInfo = new ALERT_INFO;
		memset(pInfo, '\0', sizeof(ALERT_INFO));
		pInfo->type = SOUND_ALERT;
		pInfo->sound = 1;
		pInfo->tray = 1;
		pInfo->showwnd = 0;
		sprintf(pInfo->soundfilename, "%s%s", strPath.c_str(), "\\alert.wav");
		m_cAlertInfoList.push_back(pInfo);
		if (type != -1)
		{
			pRtn = pInfo;
		}
	}

	return pRtn;
}

void CSysConfigFile::SetWndInitPos(bool bAlways)
{
	int scx, scy;
	scx = GetSystemMetrics(SM_CXSCREEN);
	scy = GetSystemMetrics(SM_CYSCREEN);

	if (bAlways || ((m_nX < 0 || m_nX > scx) || (m_nY < 0 || m_nY >scy) ||
		(m_nWidth < 0 || m_nWidth > scx) || (m_nHeight < 0 || m_nHeight >scy)))
	{
		m_nWidth = 264;
		m_nHeight = 450;
		m_nX = scx - m_nWidth;
		m_nY = (scy - m_nHeight) / 2;
	}
}

void CSysConfigFile::DeleteAllLoginInfo()
{
	ListLoginedInfo::iterator iter = m_cLoginInfoList.begin();
	for (iter; iter != m_cLoginInfoList.end(); iter++)
	{
		delete *iter;
	}
	m_cLoginInfoList.clear();
}

void CSysConfigFile::DeleteAllAlertInfo()
{
	ListAlertInfo::iterator iter = m_cAlertInfoList.begin();
	for (iter; iter != m_cAlertInfoList.end(); iter++)
	{
		delete *iter;
	}
	m_cAlertInfoList.clear();
}

void CSysConfigFile::AddServerAddress(string strServer, bool bMoveToTop)
{
	if (strServer.empty())
		return;

	list<string>::iterator iter;
	for (iter = m_cServerAddressList.begin(); iter != m_cServerAddressList.end(); ++iter)
	{
		if (*iter == strServer)
		{
			return;
		}
	}

	m_cServerAddressList.push_back(strServer);
}

bool CSysConfigFile::IsWebuserSidForbid(string sid)
{
	if (sid.empty())
		return false;

	bool brtn = false;
	
	list<string>::iterator iter = m_cForbidWebUserList.begin();

	for (iter; iter != m_cForbidWebUserList.end(); iter++)
	{
		if ((*iter) == sid)
		{
			brtn = true;
			break;
		}
	}

	return brtn;
}

void CSysConfigFile::Write(ofstream& fin, bool bVal)
{
	fin.write((char*)(&bVal), sizeof(bool));
}

void CSysConfigFile::Write(ofstream& fin, int iVal)
{
	fin.write((char*)(&iVal), sizeof(int));
}

void CSysConfigFile::Write(ofstream& fin, unsigned long lVal)
{
	fin.write((char*)(&lVal), sizeof(unsigned long));
}

void CSysConfigFile::Write(ofstream& fin, const char* sVal)
{
	byte count = 0;	
	count = strlen(sVal);
	fin.write((char*)(&count), sizeof(byte));
	fin.write(sVal, (int)count);
}

void CSysConfigFile::Write(ofstream& fin, byte byVal)
{
	fin.write((char*)(&byVal), sizeof(byte));
}

void CSysConfigFile::Write(ofstream& fin, unsigned int Val)
{
	fin.write((char*)(&Val), sizeof(unsigned int));
}

void CSysConfigFile::Read(ifstream& fout, bool& bVal)
{
	fout.read((char*)(&bVal), sizeof(bool));
}

void CSysConfigFile::Read(ifstream& fout, int& iVal)
{
	fout.read((char*)(&iVal), sizeof(int));
}

void CSysConfigFile::Read(ifstream& fout, unsigned long& lVal)
{
	fout.read((char*)(&lVal), sizeof(unsigned long));
}

void CSysConfigFile::Read(ifstream& fout, unsigned int& Val)
{
	fout.read((char*)(&Val), sizeof(unsigned int));
}

void CSysConfigFile::Read(ifstream& fout, string& sVal)
{
	char sTemp[MAX_256_LEN];
	byte count = 0;
	fout.read((char*)(&count), sizeof(byte));
	fout.read(sTemp, (int)count);
	sTemp[(int)count] = '\0';
	sVal = sTemp;
}

void CSysConfigFile::Read(ifstream& fout, byte& byVal)
{
	fout.read((char*)(&byVal), sizeof(byte));
}

void CSysConfigFile::Read(ifstream& fout, char* chVal)
{
	byte count = 0;
	fout.read((char*)(&count), sizeof(byte));
	fout.read(chVal, (int)count);
	chVal[(int)count] = '\0';
}

LOGIN_INFO * CSysConfigFile::AddOneLoginInfo(unsigned long uid, string sid, string pass, string compid)
{
	LOGIN_INFO* plg = GetLoginInfo(LOGIN_BYUID, uid, "");

	if (plg == NULL)
	{
		plg = new LOGIN_INFO;
		plg->uid = uid;
	}

	strcpy(plg->sid, sid.c_str());
	strcpy(plg->pass, pass.c_str());
	strcpy(plg->compid, compid.c_str());

	return plg;
}

CUserObject::CUserObject()
{
	
}

CUserObject::~CUserObject()
{

}

bool CUserObject::Load(unsigned short ver)
{
	return false;
}

bool CUserObject::Save(unsigned short ver)
{
	return false;
}

// 头像下载线程
static UINT WINAPI DownLoadFaceThread(void * para)
{
	//CString strFaceLinkPath = ((CEmocssApp *)AfxGetApp())->m_InitConfig.webpage_DownloadHeadImage;
	//CString strPath, strPNG;
	//strPath = FullPath(_T("Images\\headimages"));
	//strPNG.Format(_T("%s\\%lu.png"), strPath, ((CUserObject*)para)->UserInfo.uid);

	//static TCHAR* szExt[] = { _T("jpg"), _T("gif"), _T("png"), _T("jpeg"), NULL };

	//CString strFaceLink, strStorePath;
	//DWORD dwRetVal = 0;

	//for (int i = 0; szExt[i] != NULL; i++)
	//{
	//	strFaceLink.Format(_T("%s/logo_%lu.%s"), strFaceLinkPath, ((CUserObject*)para)->UserInfo.uid, szExt[i]);
	//	strStorePath.Format(_T("%s\\%lu.%s"), strPath, ((CUserObject*)para)->UserInfo.uid, szExt[i]);
	//	dwRetVal = HttpDownloadFile(((CUserObject*)para)->UserInfo.uid, strFaceLink, strStorePath);
	//	if (dwRetVal == 0)
	//	{
	//		if (i != 2)
	//		{
	//			// 将图片换成png格式
	//			CImage imgHead;
	//			imgHead.Load(strStorePath);
	//			imgHead.Save(strPNG, Gdiplus::ImageFormatPNG);
	//			DeleteFile(strStorePath);
	//		}
	//		return true;
	//	}
	//}

	return false;
}

void CUserObject::DownLoadFace()
{
	char filePath[MAX_PATH];
	sprintf(filePath, "%s\\%lu.png", FullPath("Images\\headimages").c_str(), UserInfo.uid);
	if (_access(filePath, 0) == 0)
		return;
	HANDLE hDownLoadThread = (HANDLE)_beginthreadex(NULL, 0, DownLoadFaceThread, this, CREATE_SUSPENDED, NULL);
	if (hDownLoadThread)
	{
		SetThreadPriority(hDownLoadThread, THREAD_PRIORITY_LOWEST);
		ResumeThread(hDownLoadThread);
	}
}

CWebUserObject::CWebUserObject()
{
	m_nEMObType = OBJECT_WEBUSER;
	memset(&exinfo, '\0', sizeof(WEBUSEREX_INFO));
	memset(&info, '\0', sizeof(WEBUSER_INFO));
	memset(&onlineinfo, '\0', sizeof(WEBONLINE_INFO));
	info.uid = SYS_WEBUSER;

	m_bNewComm = false;
	m_sNewSeq = 0;

	webuserid = 0;
	talkuid = 0;
	strcpy(prevurl, "");
	strcpy(prevurlhost, "");
	strcpy(prevurlvar, "");

	m_nWaitTimer = -20;
	m_resptimeoutmsgtimer = -20;
	m_resptimeoutclosetimer = -20;
	m_waitresptimeouttimer = -20;

	nTimer = 0;
	nVisitNum = 0;
	nTalkNum = 0;
	nLastVisitTime = 0;

	m_nFlag = 0;

	m_bConnected = false;
	m_bIsGetInfo = false;

	m_onlinetime = 0;
	m_bIsShow = false;

	talkuid = 0;
	transferuid = 0;
	inviteuid = SYS_WEBUSER;
	m_bNotResponseUser = 0;

	floatadminuid = 0;

	floatfromadminuid = 0;
	floatfromsort = 0;

	strcpy(chatid, "");
	gpid = 0;

	nVisitNum = 0;
	nTalkNum = 0;

	refuseinvite = 0;
	nVisitFrom = WEBUSERICON_DIRECTURL;
	m_bIsFrWX = FALSE;
	cTalkedSatus = 0;
	m_pWxUserInfo = NULL;

	GetNormalChatHisMsgSuccess = false;
	tGetNormalChatHismsgTime = 0;
}

CWebUserObject::~CWebUserObject()
{
	
}

bool CWebUserObject::Load(unsigned short ver)
{
	return false;
}

bool CWebUserObject::Save(unsigned short ver)
{
	return false;
}

void CWebUserObject::AddCommonTalkId(unsigned long uid)
{
	if (uid == NULL)
		return;

	if (IsExistCommonTalkId(uid))
		return;

	m_listCommonTalkId.push_back(uid);
}

bool CWebUserObject::IsExistCommonTalkId(unsigned long uid)
{
	list<unsigned long>::iterator iter = m_listCommonTalkId.begin();
	for (iter; iter != m_listCommonTalkId.end(); iter++)
	{
		if ((*iter) == uid)
		{
			return true;
		}
	}
	return false;
}

bool CWebUserObject::IsOnline()
{
	return false;
}

int CWebUserObject::IsForbid()
{
	return ::GetByte(exinfo.comauth, WEBUSERAUTH_FORBID);
}

void CWebUserObject::SetForbid(bool bForbid)
{
	exinfo.comauth = ::SetByte(exinfo.comauth, WEBUSERAUTH_FORBID, bForbid);
}

bool CWebUserObject::IsIDIsMutiUser(unsigned long uid)
{
	list<unsigned long>::iterator iter = m_listCommonTalkId.begin();

	for (iter; iter != m_listCommonTalkId.end(); iter++)
	{
		if (*iter == uid)
		{
			return true;
		}
	}
	return false;
}

void CWebUserObject::AddMutiUser(unsigned long uid)
{
	if (!IsIDIsMutiUser(uid))
	{
		m_listCommonTalkId.push_back(uid);
	}
}

void CWebUserObject::RemoveMutiUser(unsigned long uid)
{
	list<unsigned long>::iterator iter = m_listCommonTalkId.begin();

	for (iter; iter != m_listCommonTalkId.end(); iter++)
	{
		if (*iter == uid)
		{
			m_listCommonTalkId.erase(iter);
			return;
		}
	}
}

bool CWebUserObject::IsMutiUser()
{
	return m_listCommonTalkId.size() > 1;
}

void CWebUserObject::RemoveAllMutiUser()
{
	m_listCommonTalkId.clear();
}