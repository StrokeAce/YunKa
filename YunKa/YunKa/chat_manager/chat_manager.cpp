#include "../stdafx.h"
#include "chat_manager.h"
#include "../chat_common/comclt.h"
#include "../chat_common/comsrv.h"
#include "../chat_common/comfloat.h"
#include "../chat_common/comform.h"
#include "../chat_common/comfunc.h"
#include "../chat_common/comdef.h"
#include "../chat_common/comstruct.h"
#include "../chat_common/commsg.h"
#include "chat_history_msg.h"
#include "http_unit.h"
#include "code_convert.h"
#include <cctype>
#include <algorithm>
#include <process.h>
using namespace std;

#define CLIENTVERSION	20000
#define  HASINSERTRIGHT   false

unsigned short g_packSeq = 0;


CChatManager::CChatManager()
{
	m_bExit = false;
	m_bLoginSuccess = false;
	m_server = "tcp01.tq.cn";
	m_port = 443;
	m_usSrvRand = 0;
	m_usCltRand = (unsigned short)(rand() & 0xFFFF);
	m_socket.SetReceiveObject(this);		
	m_msgId = 0;
	m_nClientIndex = 1;
	m_handlerLogin = NULL;
	m_handlerMsgs = NULL;
	m_login = new CLogin();
	m_login->m_manager = this;
	m_nOnLineStatus = STATUS_OFFLINE;
	m_nOnLineStatusEx = STATUS_OFFLINE;
	m_vistor = new CChatVisitor();
	m_vistor->m_manager = this;
	m_timers = new CTimerManager(CChatManager::TimerProc, this);
	m_sysConfig = new CSysConfigFile();
	LoadINIResource();
	ReadSystemConfig();
}

CChatManager::~CChatManager()
{
	Exit();
}

void CChatManager::ScreenCapture()
{

}

ListLoginedInfo CChatManager::GetPreLoginInfo()
{
	return m_sysConfig->m_cLoginInfoList;
}

void CChatManager::StartLogin(string loginName, string password, bool isAutoLogin, bool isKeepPwd)
{
	assert(m_handlerLogin); // ��¼֮ǰ��Ӧ������Ϣ�ص��ӿ����ú�

	m_sLogin = loginName;
	m_password = password;
	m_bAutoLogin = isAutoLogin;
	m_bKeepPwd = isKeepPwd;

	// ��ʼ��¼ʱ������һ���жϵ�¼��ʱ�Ķ�ʱ��
	m_timers->SetTimer(5000, TIMER_LOGIN);

	m_login->StartLogin(loginName, password, isAutoLogin, isKeepPwd);
}

bool CChatManager::ReadSystemConfig()
{
	// ����Ĭ�ϵ������ļ�
	char sFile[MAX_256_LEN];
	string strFile = GetCurrentPath();
	sprintf(sFile, "%s\\config.dat", strFile.c_str(), m_userInfo.UserInfo.uid);
	if (!m_sysConfig->LoadData(sFile))
	{
		SetSystemConfigByInitconfig();		
	}

	m_sysConfig->m_sVisitorServer = m_initConfig.sVisitorServer;
	m_sysConfig->m_nVisitorServerPort = m_initConfig.nVisitorServerPort;
	return false;
}

void CChatManager::SetSystemConfigByInitconfig()
{
	int i;

	m_sysConfig->m_sStrServer = m_initConfig.sDefaultServer;
	m_sysConfig->m_nServerPort = m_initConfig.nDefaultPort;

	for (i = 0; i<10; i++)
	{
		if (strlen(m_initConfig.sListServer[i]) > 0)
		{
			m_sysConfig->m_cServerAddressList.push_back(m_initConfig.sListServer[i]);
		}
	}

	m_sysConfig->m_nFilterType = m_initConfig.visitorfilter;
	SolveUserdefineFilter(m_sysConfig->m_cWebUserfilter, m_initConfig.infofilter);
}

void CChatManager::SolveUserdefineFilter(FILTER_USERDEFINE &filter, char *sfilter)
{
	if (sfilter == NULL)
		return;

	char sbuff[MAX_256_LEN];

	char seps[] = ",";
	char *token;
	int i;

	memset(&filter, '\0', sizeof(FILTER_USERDEFINE));
	strncpy(sbuff, sfilter, MAX_256_LEN);
	token = strtok(sbuff, seps);
	i = 0;
	while (token != NULL)
	{
		if (i == 0)
		{
			filter.include = atol(token);
		}
		else
		{
			strncpy(filter.text[i - 1], token, MAX_STRINGFILTER_LEN - 2);
			filter.num++;
		}
		if (i > MAX_STRINGFILTER_NUM)
			break;

		token = strtok(NULL, seps);
		i++;
	}

	return;
}

void CChatManager::OnReceive(void* wParam, void* lParam)
{
	if (m_bExit) return;

	int nError = 0;
	TCP_PACK_HEADER TcpPackHead;

	int nPackHeadLen = COM_HEAD_PACK::GetHeadLen();
	int nTcpPackHeadLen = sizeof(TCP_PACK_HEADER);
	int nErrType = 0;

	TcpPackHead = *((TCP_PACK_HEADER *)wParam);
	assert(TcpPackHead.len <= PACKMAXLEN);

	char *RecvBuf = (char*)lParam;

	COM_HEAD_PACK Head;
	Head.head = *((PACK_HEADER*)RecvBuf);
	g_WriteLog.WriteLog(C_LOG_TRACE, "OnReceive logondlg recv Cmd:%.4x", Head.head.cmd);

	switch (Head.head.cmd)
	{
	case CMD_SRV_CONF_LOGON: // ��¼ȷ��
		nError = RecvSrvConfLogon(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		if (nError != 0)
		{
			goto FAIL;
		}
		m_handlerLogin->LoginProgress(80);
		break;
	case CMD_SRV_REP_USERINFO: // �û���Ϣ��
		nError = RecvSrvRepUserinfo(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		if (nError != 0)
		{
			goto FAIL;
		}
		break;
	case CMD_SRV_DENY_LOGON: // �ܾ���¼
		nError = RecvSrvDenyLogon(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen, nErrType);
		if (nError != 0)
		{
			goto FAIL;
		}
		else
		{
			nError = COMM_DENYLOGIN_SEG + nErrType;
			goto FAIL;
		}
		break;
	case CMD_SRV_CONF_LOGOFF:   // �ǳ�ȷ�ϰ�
		nError = RecvSrvConfLogOff(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_FRDONLINE: // ��������
		nError = RecvSrvStatusFrdOnline(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_FRDOFFLINE: // ��������
		nError = RecvSrvStatusFrdOffline(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_USER_FORM: // ���Ѹ�������״̬
		nError = RecvSrvStatusUserForm(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_SHARELIST: // �յ����Ѷ����б�
		nError = RecvFloatShareList(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CREATECHAT: // ����Ự
		nError = RecvFloatCreateChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATINFO: // �Ự��ϸ��Ϣ
		nError = RecvFloatChatInfo(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_COM_SEND_MSG: // �Ự��Ϣ,�ǵȴ�Ӧ�����
		nError = RecvComSendMsg(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATMSG: // �Ự��Ϣ,�ȴ�Ӧ�����
		nError = RecvFloatChatMsg(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATMSG_ACK: // ������ϢӦ��
		nError = RecvFloatChatMsgAck(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_ACCEPTCHAT: // ���ܻỰ
		nError = RecvFloatAcceptChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_TRANSQUEST: // �ȴ�Ӧ��  �Ựת������
		nError = RecvFloatTransQuest(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_TRANSFAILED:   // �ȴ�Ӧ��  �Ựת��ʧ��
		nError = RecvFloatTransFailed(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_INVITE_REQUEST:// �ȴ�Ӧ��  ��������
		nError = RecvInviteRequest(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_INVITE_RESULT:// �ȴ�Ӧ��  ������
		nError = RecvInviteResult(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_RELEASE: // ��ϯ�����ͷŻỰ
		nError = RecvFloatRelease(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CMDERROR: // �ȴ�Ӧ��  ����ʧ��
		nError = RecvFloatCMDError(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CLOSECHAT: // �Ự�ر�
		nError = RecvFloatCloseChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_LISTCHAT: //�Ự�б�
		nError = RecvFloatListChat(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;	
	case CMD_EVENT_ANNOUNCEMENT: // �յ���Ϣͨ��
		nError = RecvEventAnnouncement(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_UPDATE_SUCC: //������Ϣ�ɹ�
		nError = RecvSrvUpdateSucc(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_UPDATE_FAIL: // ������Ϣʧ��
		nError = RecvSrvUpdateFail(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_SERVER_COPY: // �û�����ص�½
		nError = RecvSrvDown(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_REP_TRANSFERCLIENT: // 440 ת����ʱ�û��ɹ�ʧ��
		nError = RecvRepTransferClient(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_TRANSFERCLIENT: // ����ת�ƻỰ���û�����
		nError = RecvTransferClient(Head.head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	default:
		break;
	}

	return;

FAIL:
	m_handlerLogin->LoginProgress(-1);
	return;
}

int CChatManager::RecvSrvConfLogon(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_CONF_LOGON RecvInfo(packhead.ver);
	int nError = 0;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		return nError;
	}

	m_sysConfig->m_sStrServer = m_server;
	m_sysConfig->m_nServerPort = m_port;

	m_sysConfig->m_sStrRealServer = m_server;
	m_sysConfig->m_nRealServerPort = m_port;

	m_userInfo.UserInfo.uid = RecvInfo.uin;
	strcpy(m_userInfo.UserInfo.sid, RecvInfo.strid);
	strcpy(m_userInfo.UserInfo.pass, m_password.c_str());

	//���������ص�RecvInfo.type_loginbyΪ0����
	m_sysConfig->m_nLastLoginBy = LOGIN_BYSTRING;
	if (strlen(RecvInfo.strid) <= 0)
		m_sysConfig->m_sLastLoginStr = m_sLogin;
	else
		m_sysConfig->m_sLastLoginStr = RecvInfo.strid;
	m_sysConfig->m_sLastLoginPass = m_password;

	m_sysConfig->AddServerAddress(m_sysConfig->m_sStrServer);

	m_usSrvRand = RecvInfo.m_Head.head.random;
	nError = 0;

	char lognmame[100] = { 0 };
	sprintf(lognmame, "_%s(%u)_emocss.log", RecvInfo.strid, RecvInfo.uin);
	g_WriteLog.InitLog(GetCurrentPath() + "\\log", lognmame, C_LOG_ALL);

	return nError;
}

int CChatManager::RecvSrvRepUserinfo(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_REP_USERINFO RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		goto RETURN;
	}

	if (!m_bLoginSuccess)
	{
		pUser = AddUserObject(RecvInfo.uin, RecvInfo.strid, RecvInfo.UserInfo.info.nickname, STATUS_ONLINE, -1);

		// ����ͷ��
		pUser->DownLoadFace(m_initConfig.webpage_DownloadHeadImage);

		LoginSuccess();
	}
	else
	{
		// ��¼�ɹ����յ��û���Ϣ������
		if (packhead.langtype == LANGUAGE_UTF8)
		{
			// ת��
			ConvertMsg(RecvInfo.UserInfo.info.nickname, sizeof(RecvInfo.UserInfo.info.nickname) - 1);
		}

		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvRepUserinfo uid=%u,sid=%s,nickname=%s,username=%s,langtype=%u", RecvInfo.uin,
			RecvInfo.UserInfo.info.sid, RecvInfo.UserInfo.info.nickname, RecvInfo.UserInfo.info.username, RecvInfo.UserInfo.info.langtype);

		if (RecvInfo.uin > WEBUSER_UIN)
		{
			pWebUser = GetWebUserObjectByUid(RecvInfo.uin);

			if (pWebUser != NULL)
			{
				strcpy(pWebUser->info.name, RecvInfo.UserInfo.info.nickname);
				if (strlen(pWebUser->info.name) < 2)
				{
					g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvRepUserinfo() name length��%d", strlen(pWebUser->info.name));
				}
			}
		}
		else
		{
			ONLINEFLAGUNION onlineflag;
			onlineflag.online_flag = RecvInfo.UserInfo.info.onlineflag;


			pUser = GetUserObjectByUid(RecvInfo.uin);
			if (pUser == NULL)
			{
				pUser = AddUserObject(RecvInfo.uin, RecvInfo.strid, RecvInfo.UserInfo.info.nickname, onlineflag.stStatus.nOnlineStatus, -1);

				if (pUser == NULL)
				{
					goto RETURN;
				}
				// ����ͷ��
				pUser->DownLoadFace(m_initConfig.webpage_DownloadHeadImage);
				pUser->m_nFlag = 1;
			}
			else
			{
				if (pUser->status != onlineflag.stStatus.nOnlineStatus
					|| strcmp(pUser->UserInfo.nickname, RecvInfo.UserInfo.info.nickname) != 0)
				{
					bool bexpand = (pUser->status != onlineflag.stStatus.nOnlineStatus);

					pUser->status = onlineflag.stStatus.nOnlineStatus;
					strcpy(pUser->UserInfo.nickname, RecvInfo.UserInfo.info.nickname);
					
				}
			}

			m_handlerMsgs->RecvUserInfo(pUser);
		}
	}
	
	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype)
{
	m_socket.Close();

	SRV_DENY_LOGON RecvInfo(packhead.ver);
	int nError = 0;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		goto RETURN;
	}

	errtype = RecvInfo.deny;

RETURN:

	return nError;
}

int CChatManager::UnPack(CPackInfo *pPackInfo, char *buff, int len)
{
	int nError = 0;

	if (pPackInfo == NULL)
	{
		nError = SYS_ERROR_MEMORY;
		goto RETURN;
	}

	pPackInfo->m_Pack.Clear();

	pPackInfo->m_Pack.CopyBuff((unsigned char *)buff, len, 0);

	if (!(pPackInfo->unpack()))
	{
		nError = SYS_ERROR_UNPACKINFO;
		goto RETURN;
	}

	nError = 0;

RETURN:
	return nError;
}

bool CChatManager::LoadINIResource()
{
	char sFile[MAX_256_LEN], sKey[MAX_256_LEN];

	int i, len = MAX_256_LEN;

	sprintf(sFile, "%s\\TQConfig.ini", GetCurrentPath().c_str());

	LoadIniString("common", "programe name mini", m_initConfig.sProgrameNameMini, len, sFile, NULL);

	LoadIniString("common", "help url", m_initConfig.sHelpUrl, len, sFile);

	LoadIniString("common", "copy id", m_initConfig.sCopyID, len, sFile);

	for (i = 0; i < 3; i++)
	{
		sprintf(sKey, "help about %d", i + 1);
		LoadIniString("common", sKey, m_initConfig.sHelpAbout[i], len, sFile);

		sprintf(sKey, "help about url %d", i + 1);
		LoadIniString("common", sKey, m_initConfig.sHelpAboutURL[i], len, sFile);
	}

	LoadIniInt("common", "display username type", m_initConfig.nDisplayUserNameType, sFile);

	LoadIniString("server", "address", m_initConfig.sDefaultServer, len, sFile);

	LoadIniInt("server", "port", m_initConfig.nDefaultPort, sFile);

	LoadIniString("visitor", "address", m_initConfig.sVisitorServer, len, sFile);

	LoadIniInt("visitor", "port", m_initConfig.nVisitorServerPort, sFile);

	int temp(0);
	LoadIniInt("visitor", "ZlibEnabled", temp, sFile);
	m_initConfig.bZlibEnabled = (1 == temp);

	for (i = 0; i < 10; i++)
	{
		sprintf(sKey, "list %d", i + 1);
		LoadIniString("server", sKey, m_initConfig.sListServer[i], len, sFile);
	}

	LoadIniInt("login", "save pass", m_initConfig.bSavePass, sFile);
	LoadIniInt("login", "auto login", m_initConfig.bAutoLogin, sFile);

	//visitor filter
	LoadIniInt("visitor filter", "filter", m_initConfig.visitorfilter, sFile, "0");
	LoadIniString("visitor filter", "info filter", m_initConfig.infofilter, len, sFile, "");
	LoadIniString("AuthClient", "AuthAddr", m_initConfig.sAuthAddr, len, sFile, "passport.tq.cn");
	LoadIniInt("AuthClient", "AuthPort", m_initConfig.nAuthPort, sFile, "80");
	LoadIniString("WebPages", "logout", m_initConfig.webpage_lgout, len, sFile, "http://vip.tq.cn/vip/logout.do");
	LoadIniString("WebPages", "webphone", m_initConfig.webpage_webphone, len, sFile, "http://vip.tq.cn/vip/screenIndex.do?a=1");

	// ��ѯ����
	LoadIniString("WebPages", "querywebphone", m_initConfig.webpage_querywebphone, len, sFile, "http://vip.tq.cn/vip/screenPhoneRecord.do?action=query&deal_state=0");

	// ����
	LoadIniString("WebPages", "note", m_initConfig.webpage_note, len, sFile, "http://vip.tq.cn/vip/viewLiuyan.do?a=1");

	// �ͻ�����
	LoadIniString("WebPages", "crm", m_initConfig.webpage_crm, len, sFile, "http://vip.tq.cn/vip/visitorInfomation.do?a=1");

	// ͳ�Ʒ���
	LoadIniString("WebPages", "analyze", m_initConfig.webpage_analyze, len, sFile, "http://vip.tq.cn/vip/serviceStutasE.do?a=1");

	// ��������
	LoadIniString("WebPages", "mgmt", m_initConfig.webpage_mgmt, len, sFile, "http://vip.tq.cn/vip/clientIndex.do?a=1");

	// ���㴰��
	LoadIniString("WebPages", "news", m_initConfig.webpage_news, len, sFile, "http://vip.tq.cn/vip/rss/tq_mini_loading.jsp?a=");

	// ע�����û�
	LoadIniString("WebPages", "regnewuser", m_initConfig.webpage_regnewuser, len, sFile, "http://www.tq.cn/vip/prenewqttclt.do");
	LoadIniInt("WebPages", "regnewuser_w", m_initConfig.webpage_regnewuser_w, sFile, "520");
	LoadIniInt("WebPages", "regnewuser_h", m_initConfig.webpage_regnewuser_h, sFile, "410");

	// ����ָ��
	LoadIniString("WebPages", "tqhelp", m_initConfig.webpage_tqhelp, len, sFile, "http://www.tq.cn/help_3.html");

	// ͳһ����
	LoadIniString("WebPages", "UniIdioms", m_initConfig.webpage_UniIdioms, len, sFile, "http://vip.tq.cn/vip/GetRequestXMLAction.do?cmd=NEWQUICKREPLY");

	// ʵʱ���
	LoadIniString("WebPages", "RtmMsg", m_initConfig.webpage_RtmMsg, len, sFile, "http://vip.tq.cn/vip/realTime.do?a=");

	// ������Ϣ
	LoadIniString("WebPages", "SvrMsg", m_initConfig.webpage_SvrMsg, len, sFile, "http://vip.tq.cn/vip/historyDialog.do?a=");

	// TQ����
	LoadIniString("WebPages", "TQadviser", m_initConfig.webpage_TQadviser, len, sFile, "http://vip.tq.cn/vip/myTqGuWen.do?a=");

	// �ϴ�ͷ��
	LoadIniString("WebPages", "UploadHeadImage", m_initConfig.webpage_UploadHeadImage, len, sFile, "http://vip.tq.cn/vip/preuploadfacelink.do?a=");

	// ����ͷ��
	LoadIniString("WebPages", "DownloadHeadImage", m_initConfig.webpage_DownloadHeadImage, len, sFile, "http://admin.tq.cn/vip/facelinkimgs/");

	// ����
	LoadIniString("WebPages", "FaceImage", m_initConfig.webpage_FaceImage, len, sFile, "http://vip.tq.cn/clientimages/face/images");

	// ϵͳ��Ϣ
	LoadIniString("WebPages", "SysMessage", m_initConfig.webpage_SysMessage, len, sFile, "http://sysmessage.tq.cn/SysMsg/getmesg.do?strid=%s&uin=%lu&departId=%d");

	// Ͷ�߽���
	LoadIniString("WebPages", "complaint", m_initConfig.webpage_complaint, len, sFile, "http://211.151.52.62:8081/zhengquan/preLeaveMsg.do?uin=%lu");

	//�����ļ�
	LoadIniString("WebPages", "SendWebFileLink", m_initConfig.webpage_SendWebFileLink, len, sFile, "http://vip.tq.cn/upload/upload2.do?version=100");


	LoadIniString("WebPages", "iconurl", m_initConfig.webpage_iconurl, len, sFile, "http://sysimages.tq.cn/clientimages/face/ywt_face.html");
	LoadIniString("WebPages", "faqInfo", m_initConfig.webpage_faqInfo, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqInfo");
	LoadIniString("WebPages", "faqSort", m_initConfig.webpage_faqSort, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqSort");
	LoadIniString("WebPages", "faqAll", m_initConfig.webpage_faqall, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=faqAll");
	LoadIniString("WebPages", "repickchat", m_initConfig.webpage_repickchaturl, len, sFile, "http://106.120.108.230:8031/wxcreatechat?uin=%lu&signature=%s&timestamp=%s&nonce=%s&admiuin=%lu&%s&msg=hello");
	//��������evaluate
	LoadIniString("WebPages", "evaluate", m_initConfig.webpage_evaluate, len, sFile, "http://106.120.108.230:8524/vip/scorebill.do?billid=%s&admin_uin=%u&action=scoreedit");

	LoadIniString("WebPages", "sendurl", m_initConfig.webpage_sendurl, len, sFile, "http://211.151.52.39:8080/vip/DoRequestXMLAction.do?action=url");

	//��ȡ��˾�û�
	LoadIniString("WebPages", "companyuser", m_initConfig.webpage_companyuser, len, sFile, "http://vip.tq.cn/vip/GetRequestXMLData?cmd=TRANSFER");

	// ��ȡ΢��token
	LoadIniString("WebPages", "accesstoken", m_initConfig.webpage_accesstoken, len, sFile, "http://106.120.108.230:8031/wxkfgetac?uin=%lu&signature=%s&timestamp=%s&nonce=%s&{$MSG_WORKBILL.UserDefineParams}");

	//����
	LoadIniString("WebPages", "workbillurl", m_initConfig.webpage_workbillurl, len, sFile, "http://106.120.108.230:8524/vip/workOrder.do?r=%s");

	// �ͻ�����
	LoadIniString("VisitorPages", "visitortail", m_initConfig.visitorpage_visitortail, len, sFile, "http://vip.tq.cn/vip/ClientDialog.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");

	//�Ự�ǵ�
	LoadIniString("VisitorPages", "visitorbill", m_initConfig.visitorpage_visitorbill, len, sFile, "http://vip.tq.cn/vip/recrodworkbill.do?action=prerecord&billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");

	//�ͻ���Ϣ
	LoadIniString("VisitorPages", "visitorinfo", m_initConfig.visitorpage_visitorinfo, len, sFile, "http://vip.tq.cn/vip/visitorinfo.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s");
	//�¶���
	LoadIniString("VisitorPages", "visitororder", m_initConfig.visitorpage_visitororder, len, sFile, "http://106.120.108.230:8524/vip/visitorinfo.do?billid=%s&cuin=%lu&rand=%s&transtype=%d&transfrom=%lu&stime=%s&etime=%s&clientid=%s&type=0");

	//ͨ����Ϣ
	LoadIniString("VisitorPages", "visitornotice", m_initConfig.visitorpage_notice, len, sFile, "http://www.tq.cn");
	LoadIniString("WebPages", "RegisterAccount", m_initConfig.registerAccount, len, sFile, "http://www.tq.cn");
	LoadIniString("WebPages", "ForgetPassword", m_initConfig.forgetPassword, len, sFile, "http://www.tq.cn");
	LoadIniInt("WebPages", "ForgetPSVisible", m_initConfig.forgetPSVisible, sFile);

	// ��ѯ��ʷ��¼
	LoadIniString("WebPages", "QueryHistoryRecords", m_initConfig.query_history_records, len, sFile, "http://106.120.108.230:8524/vip/getbilllist.do?adminuin=%lu&kefu_uin=%lu&client_id=%s&weixin_id=%s&current_id=%s&timestamp=%s&token=%s&pagesize=1");

	// ΢�ŷ�����ý���ļ��ϴ�url
	LoadIniString("WebPages", "WeChatMediaUpload", m_initConfig.wechat_media_upload, len, sFile, "http://file.api.weixin.qq.com/cgi-bin/media/upload?access_token=%s&type=%s");

	// ��Ѷ��ͼ��̬ͼurl
	LoadIniString("WebPages", "WeChatStaticMap", m_initConfig.wechat_static_map, len, sFile, "http://apis.map.qq.com/ws/staticmap/v2/?key=JRYBZ-QIAWS-GJ3OB-6GXXF-F3WMZ-RNBGV&size=500x400&center=%s,%s&zoom=12");

	// ��Ѷ��ͼ�ص��עurl
	LoadIniString("WebPages", "WeChatMapLocation", m_initConfig.wechat_map_location, len, sFile, "http://apis.map.qq.com/uri/v1/marker?marker=coord:%s,%s;title:%s;addr:%s&referer=myapp");

	// �Լ����ļ����������ص�΢�ŷ�������ý���ļ���url
	LoadIniString("WebPages", "FileServerMediaDownload", m_initConfig.fileserver_media_download, len, sFile, "http://106.120.108.230:8531/media/get?access_token=%s&media_id=%s");

	// �Լ����ļ��������ϴ�url
	LoadIniString("WebPages", "FileServerMediaUpload", m_initConfig.fileserver_media_upload, len, sFile, "http://106.120.108.230:8531/media/upload/%s");

	// ΢���ļ�����url
	LoadIniString("WebPages", "FileServerMediaTask", m_initConfig.fileserver_media_task, len, sFile, "http://file.api.weixin.qq.com/cgi-bin/media/get?access_token=%s&media_id=%s");

	// ΢���ļ�����url
	LoadIniString("WebPages", "FileServerMediaFileId", m_initConfig.fileserver_media_fileid, len, sFile, "http://wxm.tq.cn/media/%s");

	return true;
}

int CChatManager::SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid, unsigned long recvsock, HWND hWnd)
{
	int nrtn = 0;

	TCP_PACK_HEADER TcpPackHead;
	int nError;
	int nPackHeadLen = COM_HEAD_PACK::GetHeadLen();
	int nTcpPackHeadLen = sizeof(TCP_PACK_HEADER);
	int nSendLen;

	if (pPackInfo == NULL)
	{
		nrtn = SYS_ERROR_MEMORY;
		goto RETURN;
	}

	pPackInfo->m_Head.SetValue(pPackInfo->version,
		m_usCltRand,
		m_userInfo.UserInfo.uid,
		m_usSrvRand,
		pPackInfo->m_Head.head.cmd, 0,
		pPackInfo->m_Head.head.langtype,
		0,
		TERMINAL_PCEX);

	if (!(pPackInfo->packhead()))
	{
		nrtn = SYS_ERROR_PACKHEAD;
		goto RETURN;
	}
	if (!(pPackInfo->pack()))
	{
		nrtn = SYS_ERROR_PACKINFO;
		goto RETURN;
	}

	//���ܰ�  20090420
	pPackInfo->m_Pack.Encode();

	memset(&TcpPackHead, '\0', sizeof(TCP_PACK_HEADER));
	TcpPackHead.len = pPackInfo->m_Pack.GetPackLength();

	g_WriteLog.WriteLog(C_LOG_TRACE, "SendOnePack send pack cmd:%.4X", pPackInfo->m_Head.head.cmd);

	nSendLen = m_socket.SendImPack((char *)(pPackInfo->m_Pack.GetPackBuff()), TcpPackHead, nError);
	if (nSendLen != TcpPackHead.len + nTcpPackHeadLen)
	{
		nrtn = SYS_ERROR_SENDFAIL;

		g_WriteLog.WriteLog(C_LOG_ERROR, "SendOnePack failed %d,fd=%d", TcpPackHead.len, m_socket.getfd());
		goto RETURN;
	}

	nrtn = 0;

RETURN:
	return nrtn;
}

int CChatManager::RecvSrvConfLogOff(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	return 0;
}

int CChatManager::RecvSrvStatusFrdOnline(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_STATUS_FRDONLINE RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0 || RecvInfo.m_Head.head.random != m_usSrvRand)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvStatusFrdOnline unpack failed");
		return nError;
	}

	if (packhead.langtype == LANGUAGE_UTF8)
	{
		ConvertMsg(RecvInfo.nickname, sizeof(RecvInfo.nickname) - 1);
	}

	if (RecvInfo.uin > WEBUSER_UIN)
	{
		//�ÿͶԻ���ʼ
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvStatusFrdOnline: RecvInfo.uin > WEBUSER_UIN");
	}
	else
	{
		pUser = GetUserObjectByUid(RecvInfo.uin);
		if (pUser == NULL)
		{
			//���ﲻ���Ǻܸ��ӣ�������µ��û�����Ҫ�Զ���ˢ�����£��Ժ�����ʵʱ�ش���
			pUser = AddUserObject(RecvInfo.uin, "", RecvInfo.nickname, RecvInfo.status, -1);

			pUser->status = STATUS_ONLINE;
			strcpy(pUser->UserInfo.nickname, RecvInfo.nickname);
			pUser->m_nFlag = 1;
			pUser->DownLoadFace(m_initConfig.webpage_DownloadHeadImage);
		}
		else
		{
			pUser->status = STATUS_ONLINE;
			strcpy(pUser->UserInfo.nickname, RecvInfo.nickname);
		}

		if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
		{
			m_nOnLineStatus = RecvInfo.status;
		}

		m_handlerMsgs->RecvOnline(pUser);
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvSrvStatusFrdOffline(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_STATUS_FRDOFFLINE RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0 || RecvInfo.m_Head.head.random != m_usSrvRand)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvStatusFrdOffline unpack failed");
		return nError;
	}
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvStatusFrdOffline uin:%u,externid:%u, bak:%u",
		RecvInfo.uin, RecvInfo.externid, RecvInfo.bak);
	if (RecvInfo.uin > WEBUSER_UIN)
	{
		//�ÿͶԻ�����
		pWebUser = GetWebUserObjectByUid(RecvInfo.uin);
		if (pWebUser != NULL)
		{
			pWebUser->m_bNewComm = false;
			pWebUser->cTalkedSatus = HASTALKED;
			pWebUser->transferuid = 0;
			if (pWebUser->m_bConnected)
			{
				m_vistor->SendWebuserTalkEnd(pWebUser);

				pWebUser->m_bConnected = true;
				pWebUser->m_nWaitTimer = -20;
				pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
			}
		}
	}
	else
	{
		pUser = GetUserObjectByUid(RecvInfo.uin);
		if (pUser != NULL)
		{
			bool bexpand = false;
			if (pUser->status != STATUS_OFFLINE)
			{
				bexpand = true;
				pUser->m_bConnected = true;
				pUser->talkstatus = TALKSTATUS_NO;
				pUser->status = STATUS_OFFLINE;
			}

			if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
			{
				g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvStatusFrdOffline uid:%u", m_userInfo.UserInfo.uid);
				m_nOnLineStatus = STATUS_OFFLINE;
				m_nOnLineStatusEx = STATUS_OFFLINE;
			}


			m_handlerMsgs->RecvOffline(pUser);


		}
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvSrvStatusUserForm(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_STATUS_USER_FORM RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvStatusUserForm unpack failed");
		return nError;
	}

	pUser = GetUserObjectByUid(RecvInfo.uin);
	if (pUser != NULL)
	{
		pUser->status = RecvInfo.GetOnlineStatus();
		m_handlerMsgs->RecvUserStatus(pUser);
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvFloatShareList(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SendTo_GetListChatInfo();

	COM_FLOAT_SHARELIST RecvInfo(packhead.ver);
	int nError = 0;
	CUserObject *pUser = NULL;

	std::vector<SHAREUSEROBJ>::iterator iter;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatShareList unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}

	ONLINEFLAGUNION onlineflag;
	for (iter = RecvInfo.sharememlist.begin(); iter != RecvInfo.sharememlist.end(); iter++)
	{
		SHAREUSEROBJ ShareUserOb = (SHAREUSEROBJ)(*iter);

		onlineflag.online_flag = ShareUserOb.onlineflag;

		pUser = GetUserObjectByUid(ShareUserOb.uin);
		if (pUser == NULL)
		{
			pUser = AddUserObject(ShareUserOb.uin, "", "", onlineflag.stStatus.nOnlineStatus, -1);
			pUser->DownLoadFace(m_initConfig.webpage_DownloadHeadImage);
		}

		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatShareList:uin=%u:OnlineStatus=%d", ShareUserOb.uin, onlineflag.stStatus.nOnlineStatus);
		pUser->m_bFriend = true;
	}

	SendTo_GetAllUserInfo();

	nError = 0;

	return nError;
}

int CChatManager::RecvComSendMsg(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_SEND_MSG RecvInfo(packhead.ver);
	int nError = 0;
	MSG_TYPE msgType = MSG_TYPE_NORMAL;
	MSG_FROM_TYPE msgFrom = MSG_FROM_USER;
	char msgTime[MAX_256_LEN];
	CWebUserObject *pWebUser = NULL;
	CUserObject *pUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);

	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendMsg unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComSendMsg chatid:%s,Rand:%s,Thirdid:%s,recvuin:%u,senduin:%u,strfontinfo:%s,msgtype:%d,msg:%s",
		RecvInfo.strChatid, RecvInfo.strRand, RecvInfo.strThirdid, RecvInfo.msg.recvuin,
		RecvInfo.msg.senduin, RecvInfo.msg.strfontinfo, RecvInfo.msg.msgtype, RecvInfo.msg.strmsg);

	//����ʾ���ݣ� ���ƶ�item
	if (packhead.langtype == 4)
	{
		ConvertMsg(RecvInfo.msg.strmsg, sizeof(RecvInfo.msg.strmsg) - 1);

		ConvertMsg(RecvInfo.msg.strmobile, sizeof(RecvInfo.msg.strmobile) - 1);
	}

	if (RecvInfo.msg.senduin > WEBUSER_UIN)
	{
		//�ÿͷ�����Ϣ
		switch (RecvInfo.msg.msgtype)
		{
		case MSG_WORKBILL:
		case MSG_CLIENT_COME:
			RecvComSendWorkBillMsg(RecvInfo.msg.senduin, RecvInfo.msg.recvuin, RecvInfo.msg.strmsg, RecvInfo.msg.strmobile);
			break;
		case MSG_WEBUI_SEND_CHAT_HISMSG:   //�յ�webui�������ĻỰ��ʷ��Ϣ�����ܶ����,��Ҫ�ͻ��˷�������MSG_KEFU_GET_CHAT_HISMSG
			RecvComSendNormalChatidHisMsg(RecvInfo.msg.senduin, RecvInfo.msg.recvuin, RecvInfo);
			break;
		case MSG_TRANSFER_REQUEST://�Ựת������
			RecvComTransRequest(packhead.uin, RecvInfo);
			break;
		case MSG_TRANSFER_ANSWER://�Ựת������
			RecvComTransAnswer(packhead.uin, RecvInfo);
			break;
		case MSG_NORMAL:
		case MSG_WX:
			pWebUser = GetWebUserObjectByUid(RecvInfo.msg.senduin);
			if (pWebUser == NULL)
			{
				//��Ҫȥ��������ȡ�÷ÿ���Ϣ
				string l_msg = RecvInfo.msg.strmsg;
				if (l_msg != "null")
				{
					SendTo_GetWebUserInfo(RecvInfo.msg.senduin, RecvInfo.strChatid);

					int pos = l_msg.find("\"msgtype\":\"userinfo\"");
					int pos1 = l_msg.find("\"msgtype\":\"wxactoken\"");
					if (pos < 0 && pos1 < 0)
					{
						SaveEarlyMsg(&(RecvInfo.msg));
					}
					else
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "userinfo��Ϣ������Ϣ��¼bug��%s", RecvInfo.msg.strmsg);
					}
				}
			}
			else
			{
				if (strcmp(RecvInfo.msg.strfontinfo, "JSON=WX") == 0)
				{
					//΢����Ϣ���ͣ�utf8��������
					WxObj *pwxobj = ParseWxJsonMsg(RecvInfo.msg.strmsg);

					if (pwxobj != NULL)
					{
						pWebUser->m_bIsFrWX = true;
						if ("userinfo" == pwxobj->MsgType)
						{
							// �˴�ע�⣬����ڶ����յ�userinfo��Ӧ����ǰ���յ��Ǵ���Ϣ������
							if (pWebUser->m_pWxUserInfo == NULL)
							{
								pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
							}
							else
							{
								delete pWebUser->m_pWxUserInfo;
								pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
							}
							nError = true;
							pWebUser->m_bIsGetInfo = true;
							// �˴�΢���û���Ϣ�����������沢����ʹ��
							goto RETURN;
						}
						else if ("wxactoken" == pwxobj->MsgType)
						{
							//todo:�յ���������������΢�ŵ�access_token��Ϣ
							MapWxTokens::iterator iter = m_mapTokens.find(RecvInfo.strThirdid);
							if (iter == m_mapTokens.end())
							{
								m_mapTokens.insert(map<string, string>::value_type(RecvInfo.strThirdid, ((WxAccessTokenInfo*)pwxobj)->szAcToken));
							}
							else
							{
								iter->second = ((WxAccessTokenInfo*)pwxobj)->szAcToken;
							}
							delete pwxobj;
							nError = true;
							goto RETURN;
						}
					}
					else
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendMsg NOT the weixin Json:%s", RecvInfo.msg.strmsg);
						goto RETURN;
					}
				}
				//��ҳ�����ĻỰ������chatid��仯����Ҫ��ʱ����
				if (strcmp(pWebUser->chatid, RecvInfo.strChatid) != 0 && strlen(RecvInfo.strChatid) > 0)
					strcpy(pWebUser->chatid, RecvInfo.strChatid);
				pWebUser->m_bNewComm = false;

				if (strstr(RecvInfo.msg.strmsg, "[ϵͳ��Ϣ]") != NULL
					&& strstr(RecvInfo.msg.strmsg, "����վ���ÿ���") != NULL)
				{
					SendTo_GetWebUserInfo(RecvInfo.msg.senduin, RecvInfo.strChatid);
				}

				if (pWebUser->info.name[0] == '\0')
				{
					strcpy(pWebUser->info.name, RecvInfo.msg.strmobile);
					if (strlen(pWebUser->info.name) < 2)
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendMsg1 name length��%d", strlen(pWebUser->info.name));
					}
				}
				else if (!pWebUser->info.nameflag && strcmp(RecvInfo.msg.strmobile, pWebUser->info.name) != 0)
				{
					if (RecvInfo.msg.strmobile[0] != '\0')
					{
						strcpy(pWebUser->info.name, RecvInfo.msg.strmobile);
					}
					else
					{
						if (strlen(pWebUser->info.name) < 2)
						{
							g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendMsg2 name length��%d", strlen(pWebUser->info.name));
						}
					}
				}

				if (m_sysConfig->IsWebuserSidForbid(pWebUser->info.sid))
				{
					//��Ϣ�����ˣ�ֱ������������
					goto RETURN;
				}

				if (!(strstr(RecvInfo.msg.strmsg, "[ϵͳ��Ϣ]") != NULL && strstr(RecvInfo.msg.strmsg, "�����뿪") != NULL))
				{
					pWebUser->m_resptimeoutmsgtimer = -1;
					pWebUser->m_resptimeoutclosetimer = -1;
					if (pWebUser->m_waitresptimeouttimer < 0)
						pWebUser->m_waitresptimeouttimer = 0;	// �ÿͷ�˵���� [12/29/2010 SC]

					m_handlerMsgs->RecvMsg(pWebUser, msgFrom, GetMsgId(), msgType, MSG_DATA_TYPE_TEXT, 
						RecvInfo.msg.strmsg,msgTime, NULL, NULL, "");

					if ((RecvInfo.msg.bak == MSG_BAK_NORMAL) && !pWebUser->m_bConnected)
					{
						m_vistor->SendWebuserTalkBegin(pWebUser);
						pWebUser->cTalkedSatus = INTALKING;
						pWebUser->talkuid = m_userInfo.UserInfo.uid;

						pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
						pWebUser->transferuid = 0;
						pWebUser->m_nWaitTimer = 0;
						pWebUser->m_bConnected = true;
						//�ÿ����ˣ��϶���������
						if (pWebUser->info.status == STATUS_OFFLINE)
							pWebUser->info.status = STATUS_ONLINE;

						// ��ʾ
					}
					else if (RecvInfo.msg.bak == MSG_BAK_NORMAL)
					{
						pWebUser->cTalkedSatus = INTALKING;

						// ��ʾ 
					}

					if (m_sysConfig->m_bAutoRespUnnormalStatus)
					{
						string strResp;

						switch (m_nOnLineStatus)
						{
						case STATUS_BUSY:
						case STATUS_WORK:
						case STATUS_REFUSE_NEWWEBMSG:
							strResp = m_sysConfig->m_sUnnormalStatusMsg;
							break;
						case STATUS_LEAVE:
						case STATUS_EATING:
						case STATUS_OFFLINE:
							strResp = m_sysConfig->m_sUnnormalStatusMsg;
							break;
						}

						if (!strResp.empty())
						{
							SendAutoRespMsg(pWebUser, strResp.c_str());
						}
					}
				}
				else
				{
					pWebUser->cTalkedSatus = HASTALKED;
					m_vistor->SendWebuserTalkEnd(pWebUser);
					if (pWebUser->m_bConnected)
					{
						pWebUser->m_bConnected = false;
						pWebUser->m_nWaitTimer = -20;
						pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
					}
				}

				pWebUser->m_nWaitTimer = 0;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//�ͷ�������Ϣ
		pUser = GetUserObjectByUid(RecvInfo.msg.senduin);
		if (pUser == NULL)
		{
			pUser = AddUserObject(RecvInfo.msg.senduin, "", RecvInfo.msg.strmobile, STATUS_ONLINE, -1);
			// ���ظ���ϯ��ͷ��
			pUser->DownLoadFace(m_initConfig.webpage_DownloadHeadImage);
			if (pUser == NULL)
			{
				goto RETURN;
			}
			pUser->m_nWaitTimer = 0;

			m_handlerMsgs->RecvMsg(pUser, msgFrom, GetMsgId(), msgType, MSG_DATA_TYPE_TEXT, RecvInfo.msg.strmsg,
				msgTime, NULL, NULL, "");
			if ((RecvInfo.msg.bak == MSG_BAK_NORMAL || RecvInfo.msg.bak == MSG_BAK_AUTOANSER) && !(pUser->m_bInnerTalk))
			{
				pUser->m_bInnerTalk = true;
			}
		}
		else
		{
			strncpy(pUser->UserInfo.nickname, RecvInfo.msg.strmobile, MAX_USERNAME_LEN);//�˴��᷵�ء�ϵͳ������
			pUser->m_nWaitTimer = 0;

			m_handlerMsgs->RecvMsg(pUser, msgFrom, GetMsgId(), msgType, MSG_DATA_TYPE_TEXT, RecvInfo.msg.strmsg, msgTime, NULL, NULL, "");

			if ((RecvInfo.msg.bak == MSG_BAK_NORMAL || RecvInfo.msg.bak == MSG_BAK_AUTOANSER) && !(pUser->m_bInnerTalk))
			{
				pUser->m_bInnerTalk = true;
			}
		}
		
		// ��ʾ
	}

RETURN:
	SendAckEx(RecvInfo.msg.seq, RecvInfo.msg.senduin, RecvInfo.sendip, RecvInfo.sendport);
	nError = 0;
	return 0;
}

int CChatManager::RecvFloatCreateChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CREATECHAT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatCreateChat unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCreateChat uAdminId:%u,usort:%hu,uWebuin:%u,chatid:%s,clienttid:%s,webname:%s,uKefu:%u,uFromAdmin:%u,uFromSort:%hu",
			RecvInfo.uAdminId, RecvInfo.usort, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.clienttid, RecvInfo.webname, RecvInfo.uKefu, RecvInfo.uFromAdmin, RecvInfo.uFromSort);
	}
	if (0 == RecvInfo.clienttid[0])
	{
		//����cookie��û��clientid�Ի�����
		sprintf(RecvInfo.clienttid, "%lu", RecvInfo.uWebuin);
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCreateChat clientid is empty,set as webuin:%u", RecvInfo.uWebuin);
	}
	pWebUser = GetWebUserObjectBySid(RecvInfo.clienttid);
	if (pWebUser == NULL)
	{
		pWebUser = AddWebUserObject(RecvInfo.clienttid, RecvInfo.thirdid, RecvInfo.webname, "", "", STATUS_ONLINE, 0);
		pWebUser->m_onlinetime = 0;
		pWebUser->m_bIsShow = false;
	}

	if (RecvInfo.webname[0] != '\0' && strcmp(RecvInfo.webname, pWebUser->info.name) != 0)
	{
		strcpy(pWebUser->info.name, RecvInfo.webname);
	}

	SendTo_GetWebUserInfo(RecvInfo.uWebuin, RecvInfo.chatid);

	pWebUser->cTalkedSatus = INTALKING;
	if (RecvInfo.uFromAdmin == 0)
	{
		pWebUser->nVisitFrom = WEBUSERICON_DIRECTURL;
	}
	else if (RecvInfo.uFromAdmin != m_login->m_authAdminid)
	{
		pWebUser->nVisitFrom = WEBUSERICON_UNIONURL;
	}

	pWebUser->webuserid = RecvInfo.uWebuin;
	if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
		strcpy(pWebUser->chatid, RecvInfo.chatid);
	pWebUser->floatadminuid = RecvInfo.uAdminId;
	pWebUser->floatfromadminuid = RecvInfo.uFromAdmin;
	pWebUser->floatfromsort = RecvInfo.uFromSort;
	pWebUser->gpid = packhead.random;
	pWebUser->m_bNewComm = true;
	pWebUser->m_sNewSeq = packhead.sendrandom; //������Ϣid
	pWebUser->info.status = STATUS_ONLINE;
	pWebUser->talkuid = 0;//�ȴ�Ӧ������ĻỰ����û�н���ͷ�
	if (RecvInfo.uKefu == 0)
	{
		if (pWebUser->onlineinfo.talkstatus != TALKSTATUS_REQUEST)
		{
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_REQUEST;
			pWebUser->m_nWaitTimer = 0;

			string msg = pWebUser->info.name;
			msg += "%s �û�����Ի�!";

			m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
		}
	}
	else
	{
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
		pUser = GetUserObjectByUid(RecvInfo.uKefu);

		if (pUser != NULL &&(pUser->UserInfo.uid == m_userInfo.UserInfo.uid || pUser->m_bFriend))
		{
			pWebUser->transferuid = 0;
			if (RecvInfo.uKefu == m_userInfo.UserInfo.uid)
			{
				//���Լ�����ķÿ�
				pWebUser->m_nWaitTimer = 0;
				pWebUser->m_bConnected = true;
			}
			else
			{
				//����������ķÿ�
				pWebUser->m_nWaitTimer = -20;
				pWebUser->m_bConnected = HASINSERTRIGHT;
			}

			pWebUser->talkuid = RecvInfo.uKefu;

			//��ȡ�Ự��ʷ��Ϣ���������µ���Ϣ
			SendStartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);

			char msg[MAX_256_LEN];
			sprintf(msg, "%s�����˷ÿ�%s�ĻỰ", pUser->UserInfo.nickname, pWebUser->info.name);
			m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg, GetTimeStringMDAndHMS(0));
		}
	}

	if (RecvInfo.memlist.find(m_userInfo.UserInfo.uid) != RecvInfo.memlist.end())
		pWebUser->m_bNotResponseUser = 0;
	else
		pWebUser->m_bNotResponseUser = 1;

	m_handlerMsgs->RecvChatInfo(pWebUser);

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatChatInfo(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CHATINFO RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;
	std::vector<unsigned int>::iterator iter;
	char strMsg[MAX_256_LEN];

	strcpy(strMsg, "waiter");

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatChatInfo unpack failed,Cmd:%.4x", packhead.cmd);

		goto RETURN;
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatChatInfo uAdminId:%u,usort:%hu,uWebuin:%u,chatid:%s,clienttid:%s,webname:%s,uKefu:%u,uFromAdmin:%u,uFromSort:%hu",
			RecvInfo.uAdminId, RecvInfo.sSort, RecvInfo.uWebUin, RecvInfo.chatid, RecvInfo.strClientId, RecvInfo.webnickname, RecvInfo.uKefuUin, RecvInfo.uFromAmdind, RecvInfo.sFromSort);

	}

	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebUin);
	if (pWebUser == NULL)
	{
		pWebUser = AddWebUserObject(RecvInfo.strClientId, RecvInfo.strThirdId, RecvInfo.webnickname, "", "", STATUS_UNDEFINE, 0);
		pWebUser->webuserid = RecvInfo.uWebUin;
	}
	else
	{
		ChangeWebUserSid(pWebUser, RecvInfo.strClientId, RecvInfo.strThirdId);
	}

	// ֻҪ�յ��Ự��Ϣ��һ��ȥ��������ȡwork_bill��
	SendTo_GetWebUserInfo(pWebUser->webuserid, RecvInfo.chatid, strMsg);
	if (pWebUser->m_nEMObType != OBJECT_WEBUSER)
	{
		pWebUser->m_nEMObType = OBJECT_WEBUSER;
	}
	pWebUser->m_bNewComm = true;
	pWebUser->cTalkedSatus = INTALKING;

	pWebUser->floatadminuid = RecvInfo.uAdminId;
	if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
		strcpy(pWebUser->chatid, RecvInfo.chatid);

	pWebUser->floatfromadminuid = RecvInfo.uFromAmdind;
	pWebUser->floatfromsort = RecvInfo.sFromSort;
	pWebUser->gpid = packhead.random;

	if (RecvInfo.uKefuUin == 0)
	{
		//�ȴ�Ӧ��
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_REQUEST;
		pWebUser->info.status = STATUS_ONLINE;
		pWebUser->m_nWaitTimer = 0;

		SendStartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);
	}
	else
	{
		if (m_nNextInviteWebuserUid == pWebUser->webuserid)
		{
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_INVITE;
			pWebUser->inviteuid = m_userInfo.UserInfo.uid;
			pWebUser->info.status = STATUS_ONLINE;
			pWebUser->frominviteuid = m_nNextInviteUid;

			pWebUser->m_nWaitTimer = 0;

			CUserObject *pInviteUser = GetUserObjectByUid(m_nNextInviteUid);

			m_nNextInviteWebuserUid = 0;
			m_nNextInviteUid = 0;
		}
		else
		{
			pUser = NULL;
			if (m_userInfo.UserInfo.uid != RecvInfo.uKefuUin)
			{
				for (int i = 0; i < RecvInfo.InviteUin.size(); ++i)
				{
					if (RecvInfo.InviteUin[i] == m_userInfo.UserInfo.uid)
					{
						pUser = GetUserObjectByUid(m_userInfo.UserInfo.uid);
						pWebUser->onlineinfo.bInvited = true;
						break;
					}
				}
			}

			if (pUser == NULL)
			{
				pUser = GetUserObjectByUid(RecvInfo.uKefuUin);
				pWebUser->onlineinfo.bInvited = false;
			}

			if (RecvInfo.uTansferingToKefu == 0)
			{
				if (pUser != NULL)
				{
					if (pUser->UserInfo.uid == m_userInfo.UserInfo.uid ||
						pWebUser->onlineinfo.bInvited)
					{
						//���Լ�ת�Ƶķÿ�
						pWebUser->m_nWaitTimer = 0;
						pWebUser->m_bConnected = true;
					}
					else
					{
						//������ת�Ƶķÿ�
						pWebUser->m_bConnected = false;
						pWebUser->m_nWaitTimer = -20;
					}

					//���ڶԻ�
					pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
					pWebUser->info.status = STATUS_ONLINE;
					pWebUser->transferuid = 0;
					pWebUser->talkuid = pUser->UserInfo.uid;//�����û���Ҫ�ı�talkidΪ�Լ���

					char msg[MAX_256_LEN];
					sprintf(msg, "%s�����˷ÿ�%s�ĻỰ", pUser->UserInfo.nickname, pWebUser->info.name);

					m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);

					if (RecvInfo.uKefuUin&&pUser->m_bFriend
						|| !RecvInfo.uKefuUin&&!pWebUser->m_bNotResponseUser)
					{
						//��ȡ�Ự��ʷ��Ϣ���������µ���Ϣ
						SendStartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);
					}
				}
			}
			else
			{
				//����ת��
				pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
				pWebUser->info.status = STATUS_ONLINE;
				pWebUser->transferuid = RecvInfo.uTansferingToKefu;
				CUserObject *pAcceptUser = GetUserObjectByUid(RecvInfo.uTansferingToKefu);

				//�����������htmleditor����ʾ��Ȼ�����ƶ�λ�ã���Ϊ�ƶ�λ�ÿ��ܻᵼ��������л��������ط�ͬ������
				if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
					strcpy(pWebUser->chatid, RecvInfo.chatid);
				pWebUser->floatadminuid = RecvInfo.uAdminId;

				char msg[MAX_256_LEN];
				if (pUser != NULL)
					GetInviteChatSysMsg(msg, pUser, pWebUser, INVITE_TRANSFER, pAcceptUser);
				else if (pAcceptUser != NULL)
					sprintf(msg, "�ÿ� %s ת�Ƶ� %s", pWebUser->info.name, pAcceptUser->UserInfo.nickname);

				m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
			}
		}
	}

	pWebUser->m_bNotResponseUser = 1;
	for (iter = RecvInfo.webRecvUin.begin(); iter != RecvInfo.webRecvUin.end(); iter++)
	{
		unsigned long uid = (unsigned long)(*iter);

		if (uid == m_userInfo.UserInfo.uid)
		{
			pWebUser->m_bNotResponseUser = 0;
			break;
		}
	}

	for (iter = RecvInfo.InviteUin.begin(); iter != RecvInfo.InviteUin.end(); iter++)
	{
		unsigned long uid = (unsigned long)(*iter);

		pWebUser->AddCommonTalkId(uid);
	}

	m_handlerMsgs->RecvChatInfo(pWebUser);

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatChatMsg(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	if (packhead.uin == m_userInfo.UserInfo.uid)
		return 0;

	COM_FLOAT_CHATMSG RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	MSG_TYPE msgType = MSG_TYPE_NORMAL;
	MSG_FROM_TYPE msgFrom = MSG_FROM_USER;
	WxMsgBase* msgContentWx = NULL;
	CUserObject *pAssistUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatChatMsg unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatChatMsg getwebuserobjectbyid(%u) failed", RecvInfo.webuin);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatChatMsg chatid:%s,uAdminId:%u,uWebuin:%u,msgtype:%d,sendname:%s,strfontinfo:%s,msg:%s",
		RecvInfo.chatid, RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.nMsgType, RecvInfo.nickname, RecvInfo.strfontinfo, RecvInfo.strmsg);

	if (pWebUser->m_sWxAppid.empty()) //WxAppidΪ�գ�ȥ����˻�ȡ
	{
		SendTo_GetWebUserInfo(pWebUser->webuserid, RecvInfo.chatid);
	}

	WxMsgBase* pWxMsg = NULL;
	if (strcmp(RecvInfo.strfontinfo, "JSON=WX") == 0)
	{
		pWebUser->m_bIsFrWX = true;
		pWxMsg = ParseWxMsg(pWebUser, RecvInfo);
	}
	else
	{
		std::string content = RecvInfo.strmsg;

		bool isUrl = false;// PathIsURL(content.c_str());
		if (isUrl && pWebUser->m_bIsFrWX)
		{
			content = "<a href=\"" + content + "\" target=\"blank\">" + content + "</a>";
		}

		TransforFaceMsg(content);
		strncpy(RecvInfo.strmsg, content.c_str(), MAX_MSG_RECVLEN);
	}

	if (RecvInfo.strRand[0] != 0)
	{
		strcpy(pWebUser->info.sid, RecvInfo.strRand);
	}

	if (RecvInfo.strThirdid[0] != 0)
	{
		strcpy(pWebUser->info.thirdid, RecvInfo.strThirdid);
	}

	if (packhead.uin != RecvInfo.webuin)
	{
		// ����΢���û���������Ϣ������Э������������Ϣ
		pAssistUser = GetUserObjectByUid(packhead.uin);
		if (pAssistUser == NULL)
		{
			// ����uin���ǳ��ȳ�ʼ����ϯ����Ϣ
			pAssistUser = AddUserObject(packhead.uin, "", RecvInfo.nickname, STATUS_ONLINE, -1);
			pAssistUser->DownLoadFace(m_initConfig.webpage_DownloadHeadImage);

			// Ȼ����¸���ϯ��������Ϣ
			SendTo_GetUserInfo(packhead.uin);
		}
	}

	if (packhead.langtype == LANGUAGE_UTF8)
	{
		ConvertMsg(RecvInfo.strmsg, sizeof(RecvInfo.strmsg) - 1);

		ConvertMsg(RecvInfo.nickname, sizeof(RecvInfo.nickname) - 1);
	}
	//�ÿͷ�����Ϣ
	switch (RecvInfo.nMsgType)
	{
	case MSG_FLOAT_NORMAL:
	case MSG_FLOAT_PRV:
	case MSG_FLOAT_INPUTING:
	case MSG_WX://΢����Ϣ 
	{
		if (pWebUser->m_nWaitTimer<0)
		{
			pWebUser->m_nWaitTimer = 0;
		}

		// �Ŷӵȴ����ܺ���Ϣ�������� [12/9/2010 SC]
		if (m_sysConfig->IsWebuserSidForbid(pWebUser->info.sid))
		{
			//��Ϣ�����ˣ�ֱ������������
			goto RETURN;
		}

		if (RecvInfo.nMsgType == 2)
		{
			string strMsg = RecvInfo.strmsg;
			transform(strMsg.begin(), strMsg.end(), strMsg.begin(), ::toupper);
			if ((int)strMsg.find("userfile") > -1 && (int)strMsg.find("�յ��ļ�") > -1)
			{
				// �Ӵ�����ϯ��΢�Ŷ��󷢵�					
				if ((int)strMsg.find(".jpg") > -1 || (int)strMsg.find(".jpeg") > -1 ||
					(int)strMsg.find(".bmp") > -1 || (int)strMsg.find(".png") > -1)
				{
					RecvInfo.nMsgDataType = MSG_DATA_TYPE_IMAGE;
				}
				else
				{
					RecvInfo.nMsgDataType = MSG_DATA_TYPE_FILE;
				}
			}
			else if ((int)strMsg.find("http:") > -1 && ((int)strMsg.find(".jpg") > -1 || (int)strMsg.find(".jpeg") > -1 ||
				(int)strMsg.find(".bmp") > -1 || (int)strMsg.find(".png") > -1) || (int)strMsg.find("�յ�һ��ͼƬ") > -1)
			{
				RecvInfo.nMsgDataType = MSG_DATA_TYPE_IMAGE;
			}
			else if ((int)strMsg.find("userfile") > -1)
			{
				// �Ӵ�����ϯ��web�û�����
				RecvInfo.nMsgDataType = MSG_DATA_TYPE_FILE;
			}

			if (pAssistUser == NULL)
			{
				// �ÿͷ�����Ϣʱ����δӦ��ȴ�ʱ������
				pWebUser->m_resptimeoutmsgtimer = -1;
				pWebUser->m_resptimeoutclosetimer = -1;

				if (pWebUser->m_waitresptimeouttimer < 0)
					pWebUser->m_waitresptimeouttimer = 0;
			}
			else
			{
				msgFrom = MSG_FROM_ASSIST;
			}

			// ����Ӧ������Ϣ��ʾ
		}
		else
		{
			msgType = MSG_TYPE_PREV;
		}

		pWebUser->m_sNewSeq = packhead.sendrandom;

		if (RecvInfo.tMsgTime == 0)
		{
			// ��ȡϵͳ��ǰʱ��
		}

		m_handlerMsgs->RecvMsg((IBaseObject*)pWebUser, msgFrom, GetMsgId(),	msgType, (MSG_DATA_TYPE)RecvInfo.nMsgDataType, 
			RecvInfo.strmsg, GetTimeStringMDAndHMS(RecvInfo.tMsgTime), pAssistUser, msgContentWx, "");


		// ͬ�����¹�����
		//if (m_sysConfig->bAutoSearchKeyword)
		{
			//CUserObject *pUser = m_pFormKeyWord->GetCurSelUserOb();
			//m_pFormKeyWord->ResetKeyWord((m_nOnLineStatus != STATUS_OFFLINE), pUser, RecvInfo.strmsg);
		}

		if (m_sysConfig->m_bAutoRespUnnormalStatus)
		{
			string strResp;

			switch (this->m_nOnLineStatus)
			{
			case STATUS_BUSY:
			case STATUS_WORK:
			case STATUS_REFUSE_NEWWEBMSG:
				strResp = m_sysConfig->m_sUnnormalStatusMsg;
				break;
			case STATUS_LEAVE:
			case STATUS_EATING:
			case STATUS_OFFLINE:
				strResp = m_sysConfig->m_sUnnormalStatusMsg;
				break;
			}

			if (!strResp.empty())
			{
				SendAutoRespMsg(pWebUser, strResp.c_str());
			}
		}
	}
		break;
	case MSG_WORKBILL:
	case MSG_CLIENT_COME:
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatChatMsg MSG_WORKBILL msgtype--");
		RecvComSendWorkBillMsg(RecvInfo.webuin, -1, RecvInfo.strmsg, RecvInfo.nickname);
		break;
	default:
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatChatMsg unkown msgtype:%d", RecvInfo.nMsgType);
		break;
	}

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatChatMsgAck(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CHATMSG_ACK RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatChatMsgAck unpack failed,Cmd:%.4x", packhead.cmd);

		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatChatMsgAck uAdminId:%u,uWebuin:%u,chatid:%s,msgseq:%hu",
		RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.chatid, RecvInfo.chatseq);

	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);

	if (pWebUser != NULL)
		pWebUser->m_sNewSeq = RecvInfo.chatseq;

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatAcceptChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_ACCEPTCHAT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;
	int nRet = 0;
	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatAcceptChat unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatAcceptChat sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s,nickname:%s",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.nickname);


	pUser = GetUserObjectByUid(packhead.uin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatAcceptChat GetWebUserObjectByID(%u) failed", RecvInfo.uWebuin);
		goto RETURN;
	}
	if (m_userInfo.UserInfo.uid != packhead.uin)//�����߲��ǵ�ǰ��ϯ
	{
		pWebUser->m_nWaitTimer = -20;
		pWebUser->m_bConnected = HASINSERTRIGHT;
	}
	pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
	pWebUser->transferuid = 0;
	pWebUser->talkuid = packhead.uin;
	if (pUser == NULL || (m_userInfo.UserInfo.uid != pUser->UserInfo.uid&&!pUser->m_bFriend))
	{
		pWebUser->cTalkedSatus = HASTALKED;
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatAcceptChat delete user name is :%s, chatid: %s", pWebUser->info.name, pWebUser->chatid);
	}
	else
	{
		pWebUser->cTalkedSatus = INTALKING;
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
		if (packhead.uin == m_userInfo.UserInfo.uid)
		{
			//���Լ���������ķÿ�
			pWebUser->m_nWaitTimer = 0;
			pWebUser->m_bConnected = true;
		}

		//����������ķÿ�
		pWebUser->RemoveMutiUser(pWebUser->talkuid);
		pWebUser->inviteuid = 0;

		char msg[MAX_256_LEN];
		sprintf(msg, "%s�����˷ÿ�%s�ĻỰ", pUser->UserInfo.nickname, pWebUser->info.name);

		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL,MSG_DATA_TYPE_TEXT, msg);

		//��ȡ�Ự��ʷ��Ϣ���������µ���Ϣ
		SendStartRecvFloatMsg(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid, pWebUser->m_sNewSeq);
	}

	m_handlerMsgs->RecvAcceptChat(pUser, pWebUser);

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatTransQuest(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_TRANSREQUEST RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pInviteUser = NULL;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;
	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatTransQuest unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatTransQuest sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid);

	pAcceptUser = GetUserObjectByUid(RecvInfo.uToKefu);
	pInviteUser = GetUserObjectByUid(packhead.uin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	if (pAcceptUser == NULL)//��ȡ�������Ͳ����Լ���Э������
	{
		goto RETURN;
	}

	char msg[MAX_256_LEN];
	if (pWebUser == NULL)
	{
		SendTo_GetWebUserChatInfo(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid);
	}
	else if (RecvInfo.uToKefu == m_userInfo.UserInfo.uid)//��ǰ��ϯ�ǽ����ߣ��ƶ���ת���С�����
	{
		if (strcmp(pWebUser->chatid, RecvInfo.chatid) != 0 && strlen(RecvInfo.chatid) > 0)
			strcpy(pWebUser->chatid, RecvInfo.chatid);
		pWebUser->floatadminuid = RecvInfo.uAdminId;
		pWebUser->m_bNewComm = true;
		pWebUser->gpid = packhead.random;
		pWebUser->info.status = STATUS_ONLINE;
		pWebUser->onlineinfo.bInvited = false;//ת�ӹ������Ͳ�������Э����
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
		pWebUser->transferuid = RecvInfo.uToKefu;

		if (pInviteUser != NULL)
			GetInviteChatSysMsg(msg, pInviteUser, pWebUser, INVITE_TRANSFER, pAcceptUser);
		else
			sprintf(msg, "�ÿ� %s ת�Ƶ� %s", pWebUser->info.name, pAcceptUser->UserInfo.nickname);
		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL,MSG_DATA_TYPE_TEXT, msg);
	}
	if (packhead.uin == m_userInfo.UserInfo.uid)//��ǰ��ϯ�Ƿ���ת����
	{
		sprintf(msg, "�ѷ���ת������%s", pAcceptUser->UserInfo.nickname);
		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
	}
	nError = 0;
RETURN:
	return nError;
}

int CChatManager::RecvFloatTransFailed(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_TRANSFAILED RecvInfo(packhead.ver, packhead.random);
	int nError = 0;

	CUserObject *pInviteUser = NULL;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;
	HTREEITEM hItem = NULL;
	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatTransFailed unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatTransFailed sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s,TransFromKefu:%u,to:%u",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.uKefu, RecvInfo.TransToKefu);

	pInviteUser = GetUserObjectByUid(RecvInfo.uKefu);
	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	pAcceptUser = GetUserObjectByUid(RecvInfo.TransToKefu);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatTransFailed GetWebUserObjectByID(%u) failed", RecvInfo.uWebuin);
		goto RETURN;
	}
	pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;//TALKSTATUS_REQUEST --> TALKSTATUS_TALK

	if (pInviteUser != NULL)
	{
		//hItem = m_pFormUser->m_TreeListUser.GetChildItemByData((DWORD)pInviteUser, TREELPARAM_USER_TALK);
	}


	//������Ҫ�ж��ǲ���Ҫ������ʾ���û�
	if (pWebUser->m_bNotResponseUser || hItem == NULL&&pInviteUser != NULL || pInviteUser == NULL)
	{
		pWebUser->cTalkedSatus = HASTALKED;
		//m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
	}
	else
	{
		char msg[MAX_256_LEN];
		sprintf(msg, "�ÿ� %s ת��ʧ��", pWebUser->info.name);
		pWebUser->cTalkedSatus = INTALKING;
		pWebUser->transferuid = 0;
		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);

		// ת�ӳ�ʱ�Ự�ص�����ת����ϯ�����ǵȴ�Ӧ��
		if (m_userInfo.UserInfo.uid != pInviteUser->UserInfo.uid) 
		{
			sprintf(msg, "%s �û�����Ի�!", pWebUser->info.name);
			//SolveAlertInfo(SOUND_INVITE, str);
		}
	}

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvInviteRequest(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_INVITEREQUEST RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pInviteUser = NULL;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;
	char msg[MAX_256_LEN];

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvInviteRequest unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvInviteRequest sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s,uInviteUser:%u",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.uInviteUser);


	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	pInviteUser = GetUserObjectByUid(packhead.uin);
	pAcceptUser = GetUserObjectByUid(RecvInfo.uInviteUser);
	if (pWebUser == NULL)
	{
		if (RecvInfo.uInviteUser == m_userInfo.UserInfo.uid)
		{
			m_nNextInviteUid = packhead.uin;
			m_nNextInviteWebuserUid = RecvInfo.uWebuin;

			SendTo_GetWebUserChatInfo(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid);
		}

		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvInviteRequest GetWebUserObjectByID(%u) failed", RecvInfo.uWebuin);
		goto RETURN;
	}
	else
	{
		pWebUser->frominviteuid = packhead.uin;

	}

	if (RecvInfo.uInviteUser == m_userInfo.UserInfo.uid)
	{
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_INVITE;
		pWebUser->inviteuid = m_userInfo.UserInfo.uid;
		if (!pWebUser->m_bNewComm)
		{
			pWebUser->m_bNewComm = true;
		}
		pWebUser->m_nWaitTimer = 0;

		//�����������htmleditor����ʾ��Ȼ�����ƶ�λ�ã���Ϊ�ƶ�λ�ÿ��ܻᵼ��������л��������ط�ͬ������
		GetInviteChatSysMsg(msg, pInviteUser, pWebUser, INVITE_HELP, pAcceptUser);
	}

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvInviteResult(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_INVITERESULT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pInviteUser = NULL;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvInviteResult unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvInviteResult sResult:%hu,sender:%u,uAdminId:%u,uWebuin:%u,chatid:%s,uInviteFrom:%u",
		RecvInfo.sResult, packhead.uin, RecvInfo.uAdminId, RecvInfo.uWebuin, RecvInfo.chatid, RecvInfo.uInviteFrom);

	pWebUser = GetWebUserObjectByUid(RecvInfo.uWebuin);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvInviteResult uWebuin:%u", RecvInfo.uWebuin);
		return nError;
	}
	pWebUser->cTalkedSatus = INTALKING;
	pInviteUser = GetUserObjectByUid(RecvInfo.uInviteFrom);
	pAcceptUser = GetUserObjectByUid(packhead.uin);
	if (!pWebUser->m_bNewComm)
	{
		pWebUser->m_bNewComm = true;
	}

	char msg[MAX_256_LEN];
	if (!RecvInfo.sResult)//�ܾ�
	{
		if (RecvInfo.uInviteFrom == m_userInfo.UserInfo.uid)
		{
			//���ͷ��յ��Է�Ӧ��
			pWebUser->inviteuid = m_userInfo.UserInfo.uid;

			pWebUser->m_nWaitTimer = 0;

			GetInviteChatSysMsg(msg, pInviteUser, pWebUser, INVITE_HELP_REFUSE, pAcceptUser);
		}

		if (packhead.uin == m_userInfo.UserInfo.uid)
		{
			//���Լ������ľܾ���
			pWebUser->m_bConnected = false;
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;

			pWebUser->inviteuid = 0;

			if (pWebUser->m_bNotResponseUser || pInviteUser == NULL)
			{
				pWebUser->cTalkedSatus = HASTALKED;
			}
		}

		goto RETURN;
	}
	else//����
	{
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;

		GetInviteChatSysMsg(msg, pInviteUser, pWebUser, INVITE_HELP_ACCEPT, pAcceptUser);
		if (!pWebUser->IsMutiUser())
		{
			pWebUser->AddCommonTalkId(RecvInfo.uInviteFrom);
		}

		pWebUser->AddCommonTalkId(packhead.uin);
	}

	//����Ҫ���������ʾ
	if (packhead.uin == m_userInfo.UserInfo.uid)
	{
		//���Լ�����Ϣ
		pWebUser->onlineinfo.bInvited = true;
		pWebUser->m_nWaitTimer = 0;
		pWebUser->m_bConnected = true;
		pWebUser->talkuid = RecvInfo.uInviteFrom;
	}
	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatRelease(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	CFloatChatRelease RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;
	char msg[MAX_256_LEN];

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatRelease unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatRelease sender:%lu,uAdminId:%lu,uWebuin:%lu,chatid:%s,kefuname:%s",
		packhead.uin, RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.chatid, RecvInfo.szKefuName);


	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatRelease GetWebUserObjectByID(%u) failed", RecvInfo.webuin);
		SendTo_GetWebUserChatInfo(packhead.random, RecvInfo.uAdminId, RecvInfo.chatid);
		goto RETURN;
	}
	pUser = GetUserObjectByUid(RecvInfo.uKefu);

	pWebUser->cTalkedSatus = HASTALKED;
	pWebUser->onlineinfo.talkstatus = TALKSTATUS_REQUEST;
	pWebUser->m_bConnected = false;
	pWebUser->RemoveAllMutiUser();
	if (!pWebUser->m_bNewComm)
	{
		pWebUser->m_bNewComm = true;
	}

	switch (RecvInfo.usReason)
	{
	case REASON_CLIENT_RELEASE:    //�ͷ������ͷ��˿ͻ�
		sprintf(msg, "�ͷ� %s(%u) �����ͷ��˻Ự", RecvInfo.szKefuName, RecvInfo.uKefu);
		break;
	default:
		sprintf(msg, "�ͷ� %s(%u) �ͷ��˻Ự", RecvInfo.szKefuName, RecvInfo.uKefu);
		break;

	}

	//������Ҫ�ж��ǲ���Ҫ������ʾ���û�
	if (pWebUser->m_bNotResponseUser)
	{
		pWebUser->cTalkedSatus = HASTALKED;
	}
	else
	{
		pWebUser->cTalkedSatus = INTALKING;
		sprintf(msg, "�ͷ� %s(%lu) �Էÿ� %s �ĽӴ���ֹ", !pUser ? RecvInfo.szKefuName : pUser->UserInfo.nickname, RecvInfo.uKefu, pWebUser->info.name);
		//sprintf(msg, "%s �û�����Ի�!", pWebUser->info.name);
	}
	m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
	m_handlerMsgs->RecvReleaseChat(pWebUser);
	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvFloatCMDError(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_ERROR RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatCMDError unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}
	nError = 0;

RETURN:
	return nError;
}

int CChatManager::RecvFloatCloseChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_CLOSECHAT RecvInfo(packhead.ver, packhead.random);
	int nError = 0;
	CUserObject *pUser = NULL;
	CWebUserObject *pWebUser = NULL;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatCloseChat unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCloseChat sender:%s(%u),uAdminId:%u,uWebuin:%u,chatid:%s,usType:%hu",
		RecvInfo.nickname, packhead.uin, RecvInfo.uAdminId, RecvInfo.webuin, RecvInfo.chatid, RecvInfo.usType);

	pUser = GetUserObjectByUid(packhead.uin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.webuin);
	if (pWebUser == NULL || strcmp(pWebUser->chatid, RecvInfo.chatid) != 0)
	{
		if (pWebUser == NULL)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatCloseChat GetUserObjectByUid(%u) failed", RecvInfo.webuin);
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatCloseChat chatid(local:%s,pack:%s) not same", pWebUser->chatid, RecvInfo.chatid);
		}
		return nError;
	}

	if (CHATCLOSE_INVISTEXIT == RecvInfo.usType)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCloseChat ��������ϯ����(%u)", packhead.uin);

		//����������˳�����
		pWebUser->RemoveMutiUser(packhead.uin);

		//�����ʾ
		char msg[MAX_256_LEN];
		if (pUser != NULL)
		{
			sprintf(msg, "%s(%u)�˳��Ự", pUser->UserInfo.nickname, packhead.uin);
		}
		else
		{
			sprintf(msg, "�û�(%u)�˳��Ự", packhead.uin);
		}

		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg, GetTimeStringMDAndHMS(0));
		pWebUser->RemoveMutiUser(packhead.uin);
	}
	else
	{
		//�����Ự�����Э����ϯ
		if (pWebUser->IsMutiUser())
			pWebUser->RemoveAllMutiUser();
		pWebUser->onlineinfo.bInvited = false;
		pWebUser->m_bNewComm = false;
		pWebUser->m_bConnected = true;
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
		pWebUser->cTalkedSatus = HASTALKED;
		string strMsg = "";
		if (CHATCLOSE_UNSUBSCRIBE == RecvInfo.usType)
		{
			strMsg = "΢���û�ȡ����ע�����Ự";
		}
		else
		{
			strMsg = "�ÿͻỰ�ѽ���";
		}
		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, strMsg.c_str(), GetTimeStringMDAndHMS(0));
		if (!pWebUser->IsOnline())//�����ǲ��Ǹ��û�����������
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCloseChat ��ϯ����(%u)�ÿ�����", packhead.uin);
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatCloseChat ��ϯ����(%u)�ÿͷ�����", packhead.uin);
			//�ÿ��˻ص������б�
			pWebUser->m_nWaitTimer = -20;
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
		}

		m_handlerMsgs->RecvCloseChat(pWebUser);
	}

	nError = 0;

	return nError;
}

int CChatManager::RecvFloatListChat(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	COM_FLOAT_RECVLIST RecvInfo;
	int nError = 0;
	std::vector<LISTCHATINFO>::iterator iter;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvFloatListChat unpack failed,Cmd:%.4x", packhead.cmd);

		return nError;
	}

	string chaidlist;
	for (iter = RecvInfo.recvchatlist.begin(); iter != RecvInfo.recvchatlist.end(); iter++)
	{
		LISTCHATINFO ListChatInfo = (LISTCHATINFO)(*iter);

		SendTo_GetWebUserChatInfo(packhead.random, RecvInfo.uAdminId, ListChatInfo.chatid);

		chaidlist += ListChatInfo.chatid;
		chaidlist += ",";

	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvFloatListChat %s", chaidlist.c_str());

	nError = 0;
	return nError;
}

int CChatManager::RecvEventAnnouncement(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	g_WriteLog.WriteLog(C_LOG_ERROR,"RecvEventAnnouncement");
	return 0;
}

int CChatManager::RecvSrvUpdateSucc(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_UPDATE_SUCC RecvInfo(packhead.ver);
	int nError = 0;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvUpdateSucc unpack failed,Cmd:%.4x", packhead.cmd);
		goto RETURN;
	}

	switch (RecvInfo.type)
	{
	case UPDATE_STATUS:
		m_nOnLineStatus = GetMutiByte(RecvInfo.onlinestatus, ONLINE_INFO_STATUS, ONLINE_INFO_STATUS_LEN);
		m_userInfo.status = m_nOnLineStatus;
		break;
	case UPDATE_PASS:
		
		break;
	default:
		break;
	}

RETURN:
	return nError;
}

int CChatManager::RecvSrvUpdateFail(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	SRV_UPDATE_FAIL RecvInfo(packhead.ver);
	int nError = 0;

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvUpdateFail unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}

	return nError;
}

int CChatManager::RecvSrvDown(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	int nError = 0;
	SRV_SERVER_COPY RecvInfo(packhead.ver);

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvSrvDown unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvDown uin:%u, type:%u, seq", RecvInfo.uin, RecvInfo.type, RecvInfo.seq);
	if (RecvInfo.uin == m_userInfo.UserInfo.uid)
	{
		if (m_nOnLineStatus == STATUS_OFFLINE) 
			return 0;

		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvSrvDown uid%u", m_userInfo.UserInfo.uid);
		m_handlerMsgs->RecvOffline(&m_userInfo);
		if (RecvInfo.type >= SRVNORMAL_IPERROR)
		{
			m_lastError = "���û����������ط����е�½";
		}
	}

	return 0;
}

int CChatManager::RecvRepTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	int nError = 0;
	SRV_REP_TRANSFERCLIENT RecvInfo(packhead.ver);

	CUserObject *pInviteUser = NULL;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;
	char msg[MAX_256_LEN];

	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvRepTransferClient unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvRepTransferClient chatid:%s,rand:%u,szThirdid:%s,recvuid:%u,recvName:%s, webuid:%u,webName:%s,senduid:%u,sendName:%s,result:%u",
		RecvInfo.szChatId, RecvInfo.szRand, RecvInfo.szThirdid, RecvInfo.recvinfo.id, RecvInfo.recvinfo.name, RecvInfo.clientinfo.id, RecvInfo.clientinfo.name, RecvInfo.sendinfo.id, RecvInfo.sendinfo.name, RecvInfo.result);
	//ת��ʧ��
	pWebUser = GetWebUserObjectByUid(RecvInfo.clientinfo.id);
	if (RecvInfo.result != 0)
	{
		sprintf(msg, "ת��ʧ��%s", RecvInfo.reason);
		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvRepTransferClient chatid:%s,rand:%u,szThirdid:%s,recvuid:%u, webuid:%u,senduid:%u,result:%u,reason:%s",
			RecvInfo.szChatId, RecvInfo.szRand, RecvInfo.szThirdid, RecvInfo.recvinfo.id, RecvInfo.clientinfo.id, RecvInfo.sendinfo.id, RecvInfo.result, RecvInfo.reason);
		goto RETURN;
	}

	pAcceptUser = GetUserObjectByUid(RecvInfo.recvinfo.id);
	pInviteUser = GetUserObjectByUid(RecvInfo.sendinfo.id);

	if (pAcceptUser == NULL)
	{
		pWebUser->cTalkedSatus = HASTALKED;
		goto RETURN;
	}

	//ת�Ƴɹ��ĻỰ��Ҫɾ����
	if (pWebUser == NULL)
	{
		
	}
	else if (RecvInfo.sendinfo.id == m_userInfo.UserInfo.uid)
	{
		//��ǰ��ϯ�ɹ�ת�Ƴ�ȥ�ĻỰ
		pWebUser->cTalkedSatus = HASTALKED;//ͨ����
		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
		pWebUser->m_nWaitTimer = 0;
		pWebUser->m_bConnected = false;
		pWebUser->transferuid = 0;
		pWebUser->talkuid = RecvInfo.recvinfo.id;
		if (pInviteUser != NULL)
			GetInviteChatSysMsg(msg, pInviteUser, pWebUser, INVITE_TRANSFER, pAcceptUser);
		else
			sprintf(msg, "�ÿ� %s ת�Ƶ� %s", pWebUser->info.name, pAcceptUser->UserInfo.nickname);
		m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
	}

	nError = 0;
RETURN:

	return nError;
}

int CChatManager::RecvTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len)
{
	int nError = 0;

	//������
	SRV_TRANSFERCLIENT RecvInfo(packhead.ver);
	nError = UnPack(&RecvInfo, pRecvBuff, len);
	if (nError != 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvTransferClient unpack failed,Cmd:%.4x", packhead.cmd);
		return nError;
	}
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvTransferClient chatid:%s,rand:%u,szThirdid:%s,recvuid:%u, webuid:%u,senduid:%u",
		RecvInfo.szChatId, RecvInfo.szRand, RecvInfo.szThirdid, RecvInfo.recvinfo.id, RecvInfo.clientinfo.id, RecvInfo.sendinfo.id);

	CWebUserObject *pWebUser = NULL;
	pWebUser = GetWebUserObjectByUid(RecvInfo.clientinfo.id);

	if (pWebUser == NULL)//��ȡWorkBill
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvTransferClient pWebUser==NULL");
		SendTo_GetWebUserInfo(RecvInfo.clientinfo.id, RecvInfo.szChatId);
	}
	else
	{
		//���û��Ƶ��Լ��Ի���

		char msg[MAX_256_LEN];
		if (strcmp(pWebUser->chatid, RecvInfo.szChatId) != 0 && strlen(RecvInfo.szChatId) > 0)
		{
			strcpy(pWebUser->chatid, RecvInfo.szChatId);
		}
		if (RecvInfo.recvinfo.id == m_userInfo.UserInfo.uid)
		{
			g_WriteLog.WriteLog(C_LOG_TRACE, "RecvTransferClient CurrentKefu is the accept:%u", RecvInfo.recvinfo.id);
			pWebUser->cTalkedSatus = INTALKING;
			pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
			pWebUser->m_nWaitTimer = 0;
			pWebUser->m_bConnected = true;
			pWebUser->inviteuid = 0;
			pWebUser->talkuid = m_userInfo.UserInfo.uid;

			//ת�ƵĻỰ�϶������ߵ�
			if (pWebUser->info.status == STATUS_OFFLINE)
				pWebUser->info.status = STATUS_ONLINE;

			//�ÿ�����
			sprintf(msg, "%s�����˷ÿ�%s�ĻỰ", m_userInfo.UserInfo.nickname, pWebUser->info.name);
			m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, msg);
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "RecvTransferClient recvuid error=����ת�ƻỰ��ϯ���ǵ�ǰ��¼��ϯ,recvuid:%u, localuid:%u",
				RecvInfo.recvinfo.id, m_userInfo.UserInfo.uid);
			nError = -2;
		}
	}

	return nError;
}

int CChatManager::SendTo_GetShareList()
{
	assert(m_handlerMsgs);

	if (!m_bLoginSuccess)
		return SYS_ERROR_BEFORE_LOGIN;

	COM_FLOAT_SHARELIST SendInfo(VERSION);
	return SendPackTo(&SendInfo);
}

int CChatManager::SendTo_GetListChatInfo()
{
	if (!m_bLoginSuccess)
		return SYS_ERROR_BEFORE_LOGIN;

	COM_FLOAT_GETLIST SendInfo(VERSION);
	SendInfo.uAdminId = m_login->m_authAdminid;
	return SendPackTo(&SendInfo);	
}

CUserObject * CChatManager::AddUserObject(unsigned long id, char *sid, char *name, unsigned char status, unsigned long fd)
{
	CUserObject *pUser = GetUserObjectByUid(id);

	if (pUser == NULL)
	{
		pUser = new CUserObject();

		pUser->UserInfo.uid = id;

		m_mapUsers.insert(map<unsigned long, CUserObject*>::value_type(id, pUser));
		pUser->m_nFlag = 2;
	}
	else
	{
		pUser->m_nFlag = 1;
	}

	strcpy(pUser->UserInfo.sid, sid);
	strcpy(pUser->UserInfo.nickname, name);
	pUser->status = status;
	pUser->fd = fd;

	if (m_myInfoIsGet >= 0 && pUser->UserInfo.uid != 0 && pUser->UserInfo.uid == m_userInfo.UserInfo.uid)
	{
		m_myInfoIsGet = -20;
	}
	return pUser;
}

CUserObject * CChatManager::GetUserObjectByUid(unsigned long id)
{
	if (id == 0)
	{
		return &m_commUserInfo;
	}

	if (m_userInfo.UserInfo.uid != 0 && m_userInfo.UserInfo.uid == id)
		return &m_userInfo;

	CUserObject *pUser = NULL;

	map <unsigned long, CUserObject*>::iterator iter_user;
	iter_user = m_mapUsers.find(id);
	if (iter_user != m_mapUsers.end())
	{
		pUser = iter_user->second;
		return pUser;
	}
	else
	{
		return NULL;
	}
}

void CChatManager::TimerProc(int timeId, LPVOID pThis)
{
	CChatManager* chat_manager = (CChatManager*)pThis;
	if (timeId == TIMER_NORMAL)
	{
		if (chat_manager->m_nOnLineStatus == STATUS_OFFLINE)
			return;
		if (chat_manager->m_nMyInfoIsGet >= 0)
		{
			chat_manager->m_nMyInfoIsGet++;

			if (chat_manager->m_nMyInfoIsGet >= 3)
			{
				//����Լ�����Ϣû�л�ȡ������ô��Ҫ�ظ���ȥ��ȡ
				chat_manager->m_nMyInfoIsGet = 0;
				chat_manager->SendGetSelfInfo(chat_manager->m_userInfo.UserInfo.uid, chat_manager->m_userInfo.UserInfo.sid, CMD_SRV_REP_USERINFO);
			}
		}

		chat_manager->TimerSolveAck();
		chat_manager->TimerSolveLoginToVisitorServer();
		chat_manager->m_login->TimerSolveAuthToken();
	}
	else if (timeId == TIMER_LOGIN)
	{
		if (!chat_manager->m_bLoginSuccess)
		{
			chat_manager->m_lastError = "��¼��ʱ";
		}
		chat_manager->m_timers->KillTimer(TIMER_LOGIN);
	}
}

int CChatManager::SendTo_GetAllUserInfo()
{
	if (!m_bLoginSuccess)
		return SYS_ERROR_BEFORE_LOGIN;

	int nError = 0;
	CUserObject *pUser;
	map <unsigned long, CUserObject*>::iterator iter_user;
	for (iter_user = m_mapUsers.begin(); iter_user != m_mapUsers.end(); iter_user++)
	{
		pUser = iter_user->second;
		if (pUser != NULL && strlen(pUser->UserInfo.nickname) == NULL)
		{
			nError = SendTo_GetUserInfo(pUser->UserInfo.uid);
		}
	}
	return nError;
}

int CChatManager::SendTo_GetUserInfo(unsigned long uid)
{
	if (!m_bLoginSuccess) 
		return SYS_ERROR_BEFORE_LOGIN;
	
	if (uid == 0) 
		return SYS_ERROR_PARAMETER;

	CLT_GET_CLT_INFO SendInfo(VERSION);

	SendInfo.seq = GetPackSeq();
	SendInfo.uin = uid;
	SendInfo.type = UPDATE_ALLINFO;

	return SendPackTo(&SendInfo);
}

unsigned short CChatManager::GetPackSeq()
{
	++g_packSeq;
	if (g_packSeq == 0)
	{
		++g_packSeq;
	}
	return g_packSeq;
}

int CChatManager::SendGetSelfInfo(unsigned long id, char *strid, unsigned short cmd, unsigned short cmdtype, unsigned short type)
{
	int nError = 0;
	CLT_GET_INFO SendInfo(VERSION, cmd);

	SendInfo.cmdtype = cmdtype;
	SendInfo.type = type;

	SendInfo.id = id;
	strcpy(SendInfo.strid, strid);

	nError = SendPackTo(&SendInfo);

	return nError;
}

void CChatManager::TimerSolveAck()
{
	if (m_nSendPing < 0)
		return;

	m_nSendPing++;

	//30�뷢��һ�δ̼���
	if (m_nSendPing > 30)
	{
		if (m_nOnLineStatus != STATUS_OFFLINE)
		{
			// ����ping�������������˽�����Լ����ping��
			if (SendPing() != 0)
			{
				m_nSendPingFail++;
			}
			else
			{
				m_nSendPingFail = 0;
			}

			if (m_nSendPingFail > 3)
			{
				// �ۼ�3�����Ӳ��ϣ������ߴ���
				g_WriteLog.WriteLog(C_LOG_TRACE, "SendPingFailed3times--SetOfflineStatus ");
				SetOfflineStatus();

				CloseAllSocket();
			}
		}

		if (m_nOnLineStatusEx != STATUS_OFFLINE)
		{
			// ��������㼶��ping����ȷ�Ϸ������Ƿ�������
			if (m_vistor->SendPingToVisitorServer() != 0)
			{
				// ���Ӳ��ϴ���
				m_nOnLineStatusEx = STATUS_OFFLINE;
				m_nLoginToVisitor = 0;
			}
		}

		// ʱ������
		m_nSendPing = 0;
	}
}

int CChatManager::SendPing()
{
	int nError = 0;

	COM_SEND_PING SendInfo(VERSION);
	SendInfo.o.online_flag = m_userInfo.UserInfo.onlineflag;

	nError = SendPackTo(&SendInfo, 0, 0);

	return nError;
}

void CChatManager::SetOfflineStatus()
{
	m_nOnLineStatus = STATUS_OFFLINE;
	m_nOnLineStatusEx = STATUS_OFFLINE;
	/*SetAllUserOffline();
	DeleteAllSrvInfo();
	CloseAllSocket();

	m_pFormMain->ClearAllInfo();

	this->DeleteAllAnnoucementObject();
	this->m_pFormUser->ResetItem(m_nOnLineStatus != STATUS_OFFLINE);
	this->m_pFormKeyWord->ResetItem(m_nOnLineStatus != STATUS_OFFLINE, NULL);

	SetWindowOnlineStatus();

	if (this->m_pTqAuthClient != NULL && strlen(m_szAuthtoken) > 0)
	{
	int nlen = 200;
	char recvbuf[201];
	bool butf8(true);
	m_pTqAuthClient->Logout(m_szAuthtoken, recvbuf, nlen, butf8);
	strcpy(m_szAuthtoken, "");
	}*/
}

void CChatManager::CloseAllSocket()
{
	m_socket.Close();
	m_vistor->m_socketEx.Close();
}

int CChatManager::SendTo_UpdateOnlineStatus(unsigned short status)
{
	if (!m_bLoginSuccess)
		return SYS_ERROR_BEFORE_LOGIN;

	unsigned int dwstatus = 0;
	CLT_UPDATE_STATUS SendInfo(VERSION);
	SendInfo.seq = GetPackSeq();
	SendInfo.bSendToServer = true;
	SendInfo.type = UPDATE_STATUS;
	SendInfo.onlinestatus = SetMutiByte(dwstatus, ONLINE_INFO_STATUS, ONLINE_INFO_STATUS_LEN, (DWORD)status);
	return SendPackTo(&SendInfo);
}

CWebUserObject * CChatManager::GetWebUserObjectBySid(char *sid)
{
	MapWebUsers::iterator iter = m_mapWebUsers.find(sid);
	if (iter != m_mapWebUsers.end())
	{
		return iter->second;
	}
	return NULL;
}

CWebUserObject * CChatManager::AddWebUserObject(char *sid, char *thirdid, char *name,
	char *scriptflag, char *url, unsigned char status, unsigned char floatauth)
{
	CWebUserObject* pWebUser = new CWebUserObject();

	pWebUser->m_nFlag = 2;
	pWebUser->m_nIndex = m_nClientIndex;
	m_nClientIndex++;
	strcpy(pWebUser->info.sid, sid);
	pWebUser->AddScriptFlag(scriptflag, url);
	strcpy(pWebUser->info.thirdid, thirdid);
	strcpy(pWebUser->info.name, name);
	if (strlen(pWebUser->info.ipfromname) <= 0)
		strcpy(pWebUser->info.ipfromname, name);
	pWebUser->info.status = status;
	pWebUser->SetForbid(m_sysConfig->IsWebuserSidForbid(sid));
	pWebUser->RemoveAllMutiUser();

	m_mapWebUsers.insert(MapWebUsers::value_type(sid, pWebUser));

	if (strlen(pWebUser->info.name) < 2)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "AddWebUserObject name length��%d", strlen(pWebUser->info.name));
	}
	return pWebUser;
}

CWebUserObject * CChatManager::GetWebUserObjectByUid(unsigned long uid)
{
	MapWebUsers::iterator iter = m_mapWebUsers.begin();
	for (iter; iter != m_mapWebUsers.end(); iter++)
	{
		if (iter->second->webuserid == uid)
		{
			return iter->second;
		}
	}
	return NULL;
}

int CChatManager::SendTo_GetWebUserInfo(unsigned long webuserid, const char *chatid, char *szMsg, unsigned int chatkefuid)
{
	int nError = 0;
	COM_SEND_MSG SendInfo(VERSION);
	Json::Value jv;
	if (szMsg != NULL && chatkefuid > 0)
	{
		jv["ReturnParameters"] = szMsg;
		jv["ChatKefu"] = chatkefuid;
	}
	else if (chatkefuid > 0)
	{
		jv["ChatKefu"] = chatkefuid;
	}
	else if (szMsg != NULL){
		jv["ReturnParameters"] = szMsg;
	}

	SendInfo.msg.msgtype = MSG_GET_WORKBILL;
	SendInfo.msg.recvuin = webuserid;
	SendInfo.msg.sendtime = 0;
	strncpy(SendInfo.strChatid, chatid, MAX_CHATID_LEN);
	SendInfo.msg.bak = 0;
	SendInfo.msg.seq = GetPackSeq();
	SendInfo.version = VERSION;
	SendInfo.msg.senduin = m_userInfo.UserInfo.uid;
	strncpy(SendInfo.msg.strmsg, jv.toStyledString().c_str(), MAX_MSG_RECVLEN);
	g_WriteLog.WriteLog(C_LOG_TRACE, "SendTo_GetWebUserInfo recvuin:%u,senduin:%u��szMsg=%s", SendInfo.msg.recvuin, SendInfo.msg.senduin, szMsg);

	nError = SendPackTo(&SendInfo);

	return nError;
}

void CChatManager::TransforFaceMsg(string& str)
{
	char buf[8];
	std::string::size_type pos, n;
	const std::string *fs;
	const wx_face_t *face = wx_faces;
	for (int i = 0; i < _countof(wx_faces); ++i, ++face)
	{
		for (pos = 0;;)
		{
			fs = &face->raw;
			if (std::string::npos == (n = str.find(*fs, pos)))
			{
				fs = &face->enc;
				if (face->enc.empty() || std::string::npos == (n = str.find(*fs, pos)))
					break;
			}
			pos = n;
			_itoa(i, buf, 10);
			str.insert(pos, FACE_PREFIX);
			pos += sizeof(FACE_PREFIX)-1;
			str.insert(pos, m_initConfig.webpage_FaceImage);
			pos += strlen(m_initConfig.webpage_FaceImage);
			str.insert(pos, buf);
			pos += strlen(buf);
			str.replace(pos, fs->length(), FACE_ALT FACE_SUFFIX);
			pos += sizeof(FACE_ALT FACE_SUFFIX) - 1;
		}
	}

	// deal with android/ios emoji face
	int len = str.length() - 3;
	for (int i = 0; i < len; ++i)
	{
		if (str[i] == -16 && str[i + 1] == -97)
		{
			str[i] = -18;
			str[i + 1] = -112;
			str[i + 2] = -116;
			str.erase(i + 3, 1);
		}
	}
}

WxMsgBase* CChatManager::ParseWxMsg(CWebUserObject* pWebUser, COM_FLOAT_CHATMSG& recvInfo)
{
	//΢����Ϣ���ͣ�utf8�������� 
	WxMsgBase* msgBase = NULL;
	WxObj *pwxobj = ParseWxJsonMsg(recvInfo.strmsg);

	if (pwxobj != NULL)
	{
		if ("userinfo" == pwxobj->MsgType)
		{
			// �˴�ע�⣬����ڶ����յ�userinfo��Ӧ����ǰ���յ��Ǵ���Ϣ������
			if (pWebUser->m_pWxUserInfo == NULL)
			{
				pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
			}
			else
			{
				delete pWebUser->m_pWxUserInfo;
				pWebUser->m_pWxUserInfo = (WxUserInfo*)pwxobj;
			}
			pWebUser->m_bIsGetInfo = true;
		}
		else if ("wxactoken" == pwxobj->MsgType)
		{
			//todo:�յ���������������΢�ŵ�access_token��Ϣ
			MapWxTokens::iterator iter = m_mapTokens.find(recvInfo.strThirdid);
			if (iter == m_mapTokens.end())
			{
				m_mapTokens.insert(map<string, string>::value_type(recvInfo.strThirdid, ((WxAccessTokenInfo*)pwxobj)->szAcToken));
			}
			else
			{
				iter->second = ((WxAccessTokenInfo*)pwxobj)->szAcToken;
			}
			delete pwxobj;
		}
		else if ("image" == pwxobj->MsgType)
		{
			//GetWxUserInfoAndToken(pWebUser);
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_IMAGE;
		}
		else if ("voice" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_VOICE;
		}
		else if ("video" == pwxobj->MsgType || "shortvideo" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_VIDEO;
		}
		else if ("location" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_LOCATION;
		}
		else if ("link" == pwxobj->MsgType)
		{
			msgBase = (WxMsgBase*)pwxobj;
			recvInfo.nMsgDataType = MSG_DATA_TYPE_LINK;
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "ParseWxMsg unknow weixin Json:%s", recvInfo.strmsg);
			delete pwxobj;
		}
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "ParseWxMsg NOT the weixin Json:%s", recvInfo.strmsg);
	}
	return msgBase;
}

string CChatManager::GetMsgId()
{
	char msgid[256];
	sprintf(msgid, "id_%d", m_msgId);
	m_idLock.Lock();
	m_msgId++;
	m_idLock.Unlock();
	return msgid;
}

void CChatManager::SaveEarlyMsg(MSG_INFO *pMsgInfo)
{
	MSG_INFO *pInfo = new MSG_INFO;

	memcpy(pInfo, pMsgInfo, sizeof(MSG_INFO));

	m_listEarlyMsg.push_back(pInfo);
}

int CChatManager::SendAckEx(unsigned short seq, unsigned long uid /*= 0*/, unsigned long ip /*= 0*/, unsigned short port /*= 0*/)
{
	int nError = 0;
	COM_ACKEX SendInfo(VERSION);

	SendInfo.seq = seq;
	SendInfo.uin = uid;
	SendInfo.ip = ip;
	SendInfo.port = port;

	nError = SendPackTo(&SendInfo);

	return nError;
}

int CChatManager::SendAutoRespMsg(CWebUserObject *pWebUser, const char *msg, BOOL bClearTimer /*= true*/)
{
	int rtn = SYS_ERROR_SENDFAIL;
	if (pWebUser == NULL || msg == NULL || strlen(msg) <= 0)
		return rtn;

	if (!pWebUser->m_bConnected)
	{
		return rtn;
	}

	if (SendMsg(pWebUser, msg) == 0)
	{
		//m_pFormMain->SendComMsg(pWebUser, LPCTSTR(msg));

		if (bClearTimer && pWebUser != NULL)
		{
			pWebUser->m_resptimeoutmsgtimer = -1;
			pWebUser->m_resptimeoutclosetimer = -1;
			pWebUser->m_waitresptimeouttimer = -1;
		}
	}
	else
	{
		//m_pFormMain->AddSysBuffToListMsg(pWebUser, "�Զ��ظ���Ϣʧ��!");
	}

	return rtn;
}

void CChatManager::RecvComSendWorkBillMsg(unsigned long senduid, unsigned long recvuid, char *msg, char* mobile)
{
	char sid[MAX_WEBCLIENID_LEN + 1] = { 0 };
	char billid[MAX_CHATID_LEN + 1] = { 0 };
	char szReturnParameters[51] = { 0 };
	CWebUserObject *pWebUser;
	string strMsg = "", strReturnParameters = "";
	strMsg = msg;
	int nPos = strMsg.find("ReturnParameters:");
	strReturnParameters = strMsg.substr(nPos + 17, strMsg.size() - nPos - 17);
	if (!GetContentBetweenString(msg, "WorkBillID:", "\n", billid))
	{
		billid[0] = 0;
	}
	if (GetContentBetweenString(msg, "rand=", "&", sid))
	{
		char thirdid[MAX_THIRDID_LEN + 1] = { 0 };
		GetContentBetweenString(msg, "rand=", "&", sid);
		GetContentBetweenString(msg, "clientid=", "&", thirdid);

		char chatfrom[MAX_CHATID_LEN + 1] = { 0 };
		GetContentBetweenString(msg, "chatfrom=", "&", chatfrom);

		char wxappid[100] = { 0 };
		GetContentBetweenString(msg, "wxappid=", "&", wxappid);
		GetContentBetweenString(msg, "ReturnParameters:", "\0", szReturnParameters);
		pWebUser = GetWebUserObjectByUid(senduid);
		if (pWebUser != NULL)
		{
			//��webuiʱ���get��û�в���rand��webui��rand��Ϊclientuin
			if (strlen(sid) > 0 || ((unsigned long)atol(sid)) != senduid)
			{
				ChangeWebUserSid(pWebUser, sid, thirdid);
			}
			//���˷ÿͣ��϶������ߵ�
			if (pWebUser->info.status == STATUS_OFFLINE)
				pWebUser->info.status = STATUS_ONLINE;
			if (senduid > WEBUSER_UIN && pWebUser->webuserid != senduid)
			{
				pWebUser->webuserid = senduid;
			}
		}
		else
		{
			pWebUser = GetWebUserObjectBySid(sid);

			if (pWebUser == NULL)
			{
				pWebUser = AddWebUserObject(sid, "", mobile, "", "", STATUS_ONLINE, 0);
				SendTo_GetUserInfo(senduid);
			}

			pWebUser->webuserid = senduid;
			SolveWebUserEarlyMsg(pWebUser);
		}

		if (pWebUser != NULL&&billid[0] != 0)
		{
			strncpy(pWebUser->info.chatfrom, chatfrom, MAX_CHATID_LEN);
			strncpy(pWebUser->chatid, billid, MAX_CHATID_LEN);
			pWebUser->m_sWxAppid = wxappid;

			// ΢���û���������ȡ΢�ŵ�userinfo
			if (!pWebUser->m_sWxAppid.empty())
			{
				pWebUser->m_bIsFrWX = true;
				SendGetWxUserInfoAndToken(pWebUser);
			}

			time_t tnow = time(NULL);

			if (!pWebUser->GetNormalChatHisMsgSuccess
				&&tnow - pWebUser->tGetNormalChatHismsgTime > 3) //�жϳ�ʱ���������ж�ʱ���ж��£���ֹ�յ�����������Ӧ��
			{
				SendGetChatHisMsg(senduid, billid);// ����Ƿǵȴ�Ӧ��Ĳ���Ҫȥ��ȡ��Ϣ��¼
				pWebUser->tGetNormalChatHismsgTime = tnow;
			}
		}

		nPos = strReturnParameters.find("transfer");
		if (nPos >= 0)//ת����
		{
			//����ת��
			//strReturnParameters.Remove('\\');
			//strReturnParameters.Remove('\n');
			Json::Value jv;
			//if (!ParseJson(strReturnParameters.GetBuffer(0), jv))
			{
				return;
			}
			//�жϷ��سɹ�ʧ�ܽ��
			unsigned long uTransuid = GetIntFromJson(jv, "transfer");
			if (pWebUser->transferuid != m_userInfo.UserInfo.uid)
			{
				pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
				pWebUser->info.status = STATUS_ONLINE;
				pWebUser->transferuid = m_userInfo.UserInfo.uid;
				pWebUser->talkuid = uTransuid;
				char msg[MAX_256_LEN];
				sprintf(msg, "�ÿ� %s ת�Ƶ� %s", pWebUser->info.name, m_userInfo.UserInfo.nickname);
				//m_pFormUser->MoveWebUserToUserTransferItem(pWebUser, m_pUserInfo);
				//m_tranferList[pWebUser->webuserid] = 0;
				//KillTimer(TIMER_TRANS_TIMEOUT);
				//SetTimer(TIMER_TRANS_TIMEOUT, 1000, NULL);
			}
		}
		else if ((pWebUser->cTalkedSatus != INTALKING || !pWebUser->m_bConnected)
			&& m_userInfo.UserInfo.uid == recvuid && !pWebUser->m_bNewComm)//�ǵȴ�Ӧ��ĻỰ
		{
			m_vistor->SendWebuserTalkBegin(pWebUser);

			pWebUser->cTalkedSatus = INTALKING;

			pWebUser->talkuid = m_userInfo.UserInfo.uid;

			pWebUser->onlineinfo.talkstatus = TALKSTATUS_TALK;
			pWebUser->transferuid = 0;
			pWebUser->m_nWaitTimer = 0;
			pWebUser->m_bConnected = true;

			//int nRet = this->m_pFormUser->MoveWebUserToUserTalkItem(pWebUser, m_pUserInfo);
			//if (nRet < 0)
			//	m_pFormUser->m_TreeListUser.DeleteItemByLParam((LPARAM)pWebUser);
			//else
			//	AddMultiWebUserToList(pWebUser);//�ѽ���ĻỰ�ƶ����Ự�б���
			
			// ��ʾ
		}
	}
}

void CChatManager::SolveWebUserEarlyMsg(CWebUserObject *pWebUser)
{
	list<MSG_INFO*>::iterator iter = m_listEarlyMsg.begin();

	for (iter; iter != m_listEarlyMsg.end(); iter++)
	{
		if (pWebUser->webuserid == (*iter)->senduin)
		{
			pWebUser->m_nWaitTimer = 0;
			//this->m_pFormMain->RecvComMsg(pWebUser, msginfo, true);
			if (strlen(pWebUser->info.name) <= 0)
			{
				//strcpy(pWebUser->info.name, msginfo->strmobile);
				//if (strlen(pWebUser->info.ipfromname) <= 0)
				//	strcpy(pWebUser->info.ipfromname, pWebUser->info.name);
				if (strlen(pWebUser->info.name) < 2)
				{
					g_WriteLog.WriteLog(C_LOG_ERROR, "SolveWebUserEarlyMsg name length��%d", strlen(pWebUser->info.name));
				}
			}
		}
	}

	
	for (iter = m_listEarlyMsg.begin(); iter != m_listEarlyMsg.end(); iter++)
	{
		if (pWebUser->webuserid == (*iter)->senduin)
		{
			delete (*iter);
			m_listEarlyMsg.erase(iter);
			return;
		}
	}
}

void CChatManager::RecvComSendNormalChatidHisMsg(unsigned long senduid, unsigned long recvuid, COM_SEND_MSG& RecvInfo)
{
	if (!g_NomalChatHistoryMsgManage.ParseJsonHisMsg(RecvInfo.strChatid, RecvInfo.msg.strmsg))
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendNormalChatidHisMsg failed,chatid:%s,msg:%s", RecvInfo.strChatid, RecvInfo.msg.strmsg);
		return;
	}
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComSendNormalChatidHisMsg msg:%s", RecvInfo.msg.strmsg);
	
	
	CNChatHisMsg nchatmsg;
	if (CNomalChatHistoryMsgManage::NHISMSGTYPE::SUCCESS != g_NomalChatHistoryMsgManage.GetCompleteMsgs(RecvInfo.strChatid, nchatmsg))
	{
		return;
	}
	
	//��ʾ��ʷ��Ϣ���������ѻ�ȡ��ʷ��Ϣ״̬
	
	CWebUserObject *pWebUser = GetWebUserObjectBySid(RecvInfo.strChatid);
	if (pWebUser != NULL)
	{
		pWebUser->GetNormalChatHisMsgSuccess = true; // ���λỰ��Ҫ���ñ���
	}
}

int CChatManager::RecvComTransRequest(unsigned long senduid, COM_SEND_MSG& RecvInfo)
{
	int nError = 0;
	CUserObject *pAcceptUser = NULL;
	CWebUserObject *pWebUser = NULL;

	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComTransRequest ��%s", RecvInfo.msg.strmsg);

	pAcceptUser = GetUserObjectByUid(RecvInfo.msg.recvuin);
	pWebUser = GetWebUserObjectByUid(RecvInfo.msg.senduin);
	if (pAcceptUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComTransRequest GetUserObject failed");
		goto RETURN;
	}

	if (pWebUser == NULL)
	{
		pWebUser = GetWebUserObjectBySid(RecvInfo.strRand);
	}

	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComTransRequest pWebUser==NULL");
		char strMsg[MAX_256_LEN];
		sprintf(strMsg, "{\"transfer\":%u}", senduid);
		SendTo_GetWebUserInfo(RecvInfo.msg.senduin, RecvInfo.strChatid, strMsg, senduid);
	}
	else if (RecvInfo.msg.recvuin == m_userInfo.UserInfo.uid && pWebUser->transferuid != m_userInfo.UserInfo.uid)//��ǰ��ϯ�ǽ����ߣ��ƶ���ת���С�����
	{
		if (strcmp(pWebUser->chatid, RecvInfo.strChatid) != 0 && strlen(RecvInfo.strChatid) > 0)
			strcpy(pWebUser->chatid, RecvInfo.strChatid);
		pWebUser->talkuid = senduid;
		pWebUser->m_bNewComm = false;
		pWebUser->info.status = STATUS_ONLINE;
		if (RecvInfo.msg.senduin > WEBUSER_UIN && pWebUser->webuserid != RecvInfo.msg.senduin)
		{
			pWebUser->webuserid = RecvInfo.msg.senduin;
		}

		pWebUser->onlineinfo.talkstatus = TALKSTATUS_TRANSFER;
		pWebUser->transferuid = m_userInfo.UserInfo.uid;

		//m_pFormMain->RecvTransferWebUser(pWebUser, pAcceptUser->UserInfo.nickname);

		//m_pFormUser->MoveWebUserToUserTransferItem(pWebUser, pAcceptUser);
		
		//��ȡ��ʷ��Ϣ

		//���ó�ʱ��ʱ��
		//m_TranfserList[pWebUser->webuserid] = 0;
		//KillTimer(TIMER_TRANS_TIMEOUT);
		//SetTimer(TIMER_TRANS_TIMEOUT, 1000, NULL);
	}

	nError = 0;
RETURN:
	return nError;
}

int CChatManager::RecvComTransAnswer(unsigned long senduid, COM_SEND_MSG& RecvInfo)
{
	int nError = 0;
	CUserObject *pAcceptUser = NULL;//��ת����ϯ
	CWebUserObject *pWebUser = NULL;
	string strMsg = "";
	g_WriteLog.WriteLog(C_LOG_TRACE, "RecvComTransAnswer ��%s,recvuin:%u", RecvInfo.msg.strmsg, RecvInfo.msg.recvuin);

	pAcceptUser = GetUserObjectByUid(senduid);
	pWebUser = GetWebUserObjectByUid(RecvInfo.msg.senduin);
	if (senduid == 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComTransAnswer GetUserObject��ȡ��ת����ʧ�ܣ�%u", senduid);
		nError = -1;
		goto RETURN;
	}
	if (pWebUser == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComTransAnswer GetWebUserObject��ȡ�û�ʧ�ܣ�chatid%s", RecvInfo.strChatid);
		nError = -2;
		goto RETURN;
	}

	strMsg = RecvInfo.msg.strmsg;
	if ((int)strMsg.find("OK") >= 0)//ͬ��ת��
	{
		//��ϯͬ�����CLT_TRANSFERCLIENT��
		strMsg = pWebUser->info.name;
		strMsg = "�Է�ͬ�����Էÿ�[%s]��ת��" + strMsg;
		//SendTransferClinet(pAcceptUser, pWebUser, senduid);
	}
	else if ((int)strMsg.find("NO") >= 0)//�ܾ�ת��
	{
		strMsg = pWebUser->info.name;
		strMsg = "�Է��ܾ����Էÿ�[%s]��ת��" + strMsg;
	}
	else if ((int)strMsg.find("TIMEOUT") >= 0)//��ʱ
	{
		strMsg = pWebUser->info.name;
		strMsg = "���Էÿ�[%s]��ת�ӳ�ʱ" + strMsg;
	}

RETURN:
	return nError;
}

int CChatManager::SendTo_GetWebUserChatInfo(unsigned short gpid, unsigned long adminid, char *chatid)
{
	int nError = 0;
	COM_FLOAT_CHATINFO SendInfo(VERSION, gpid);

	SendInfo.uAdminId = adminid;
	strcpy(SendInfo.chatid, chatid);

	nError = SendPackTo(&SendInfo);
	g_WriteLog.WriteLog(C_LOG_TRACE, "SendTo_GetWebUserChatInfo chatid:%s,gpid:%u", chatid, gpid);
	return nError;
}

CWebUserObject * CChatManager::ChangeWebUserSid(CWebUserObject *pWebUser, char *sid, char *thirdid)
{
	if (pWebUser == NULL || sid == NULL)
		return NULL;

	if (strlen(pWebUser->info.sid) <= 0)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "ChangeWebUserSid ");
	}
	else
	{
		if (strcmp(pWebUser->info.sid, sid) != 0)
		{
			MapWebUsers::iterator iter = m_mapWebUsers.find(pWebUser->info.sid);
			if (iter != m_mapWebUsers.end())
			{
				m_mapWebUsers.erase(iter);
			}
			sprintf(pWebUser->info.sid, sid);
			m_mapWebUsers.insert(MapWebUsers::value_type(sid, pWebUser));

			pWebUser->SetForbid(m_sysConfig->IsWebuserSidForbid(sid));
		}
	}

	return pWebUser;
}

int CChatManager::SendStartRecvFloatMsg(unsigned short gpid, unsigned long adminid, char *chatid, unsigned short sLastMsgid)
{
	int nError = 0;
	COM_FLOAT_STARTORSTOPRECV StartInfo(VERSION, gpid);

	StartInfo.uAdminId = adminid;
	StartInfo.sAcitonType = 0;
	strcpy(StartInfo.chatid, chatid);
	StartInfo.sLastMsgid = sLastMsgid;


	nError = SendPackTo(&StartInfo);
	g_WriteLog.WriteLog(C_LOG_TRACE, "SendStartRecvFloatMsg chatid:%s,gpid:%u,sLastMsgid:%u", chatid, gpid, sLastMsgid);
	return nError;
}

void CChatManager::SendGetWxUserInfoAndToken(CWebUserObject* pWebUser)
{
	if (pWebUser == NULL || !pWebUser->m_bIsFrWX)
		return;

	if (pWebUser->m_pWxUserInfo == NULL)
	{
		SendGetWxUserInfo(pWebUser->webuserid, pWebUser->chatid);
	}

	MapWxTokens::iterator iter = m_mapTokens.find(pWebUser->info.thirdid);
	if (iter == m_mapTokens.end())
	{
		SendGetWxToken(pWebUser->webuserid, pWebUser->chatid);
	}
}

int CChatManager::SendGetWxUserInfo(unsigned long webuserid, const char *chatid)
{
	int nError = 0;
	COM_SEND_MSG SendInfo(VERSION);

	SendInfo.msg.msgtype = MSG_KEFU_GET_WX_USERINFO;
	SendInfo.msg.recvuin = webuserid;
	SendInfo.msg.sendtime = GetCurrentLongTime();
	strncpy(SendInfo.strChatid, chatid, MAX_CHATID_LEN);
	SendInfo.msg.bak = 0;
	SendInfo.msg.seq = GetPackSeq();
	SendInfo.version = VERSION;
	SendInfo.msg.senduin = m_userInfo.UserInfo.uid;

	g_WriteLog.WriteLog(C_LOG_TRACE, "SendGetWxUserInfo: recvuin:%u,senduin:%u", SendInfo.msg.recvuin, SendInfo.msg.senduin);
	nError = SendPackTo(&SendInfo);
	return nError;
}

int CChatManager::SendGetWxToken(unsigned long webuserid, const char *chatid)
{
	int nError = 0;
	COM_SEND_MSG SendInfo(VERSION);

	SendInfo.msg.msgtype = MSG_KEFU_GET_WX_ACCESSTOKEN;
	SendInfo.msg.recvuin = webuserid;
	SendInfo.msg.sendtime = GetCurrentLongTime();
	strncpy(SendInfo.strChatid, chatid, MAX_CHATID_LEN);
	SendInfo.msg.bak = 0;
	SendInfo.msg.seq = GetPackSeq();
	SendInfo.version = VERSION;
	SendInfo.msg.senduin = m_userInfo.UserInfo.uid;

	g_WriteLog.WriteLog(C_LOG_TRACE, "SendGetWxToken: recvuin:%u,senduin:%u", SendInfo.msg.recvuin, SendInfo.msg.senduin);
	nError = SendPackTo(&SendInfo);
	return nError;
}

void CChatManager::GetInviteChatSysMsg(char* msg, CUserObject *pInviteUser, CWebUserObject *pWebUser, int type, CUserObject *pAcceptUser)
{
	if (pWebUser == NULL)
		return;

	switch (type)
	{
	case APPLY_ASK:
		if (pInviteUser == NULL)
			return;
		sprintf(msg, "%s������һ����������", pInviteUser->UserInfo.nickname);
		break;
	case APPLY_OPEN:
		if (pInviteUser == NULL)
			return;
		sprintf(msg, "%s����ֱ�ӶԻ�", pInviteUser->UserInfo.nickname);
		break;
	case MYGETNOTE:
		if (pInviteUser == NULL)
			return;
		sprintf(msg, "%s��������", pInviteUser->UserInfo.nickname);
		break;
	case INVITE_TRANSFER:
		if (pAcceptUser == NULL)
			return;

		if (pInviteUser == NULL)
			sprintf(msg, "%sת�ӵ��û� %s", pWebUser->info.name, pAcceptUser->UserInfo.nickname);
		else
			sprintf(msg, "%sת���û� %s �� %s", pInviteUser->UserInfo.nickname, pWebUser->info.name, pAcceptUser->UserInfo.nickname);
		break;
	case INVITE_HELP:
		if (pAcceptUser == NULL)
			return;

		if (pInviteUser == NULL)
			sprintf(msg, "%s������Ӵ� %s", pAcceptUser->UserInfo.nickname, pWebUser->info.name);
		else
			sprintf(msg, "%s�����û� %s �Ӵ� %s", pInviteUser->UserInfo.nickname, pAcceptUser->UserInfo.nickname, pWebUser->info.name);
		break;
	case INVITE_HELP_REFUSE:
		if (pAcceptUser == NULL)
			return;

		if (pInviteUser == NULL)
			sprintf(msg, "%s�ܾ�����Ӵ� %s", pAcceptUser->UserInfo.nickname, pWebUser->info.name);
		else
			sprintf(msg, "%s�ܾ��û� %s ������ �Ӵ� %s", pAcceptUser->UserInfo.nickname, pInviteUser->UserInfo.nickname, pWebUser->info.name);
		break;
	case INVITE_HELP_ACCEPT:
		if (pAcceptUser == NULL)
			return;

		if (pInviteUser == NULL)
			sprintf(msg, "%sͬ������Ӵ� %s", pAcceptUser->UserInfo.nickname, pWebUser->info.name);
		else
			sprintf(msg, "%sͬ���û� %s ������ �Ӵ� %s", pAcceptUser->UserInfo.nickname, pInviteUser->UserInfo.nickname, pWebUser->info.name);
		break;
	default:
		sprintf(msg, "����Э�������쳣");
		break;
	}
}

void CChatManager::SetHandlerLogin(IHandlerLgoin* handlerLogin)
{
	m_handlerLogin = handlerLogin;
}

void CChatManager::SetHandlerMsgs(IHandlerMsgs* handlerMsgs)
{
	m_handlerMsgs = handlerMsgs;
}

void CChatManager::Exit()
{
	m_bExit = true;
	m_socket.m_bRecvThread = false;
	m_sysConfig->SaveData();

	if (m_timers)
	{
		delete m_timers;
	}

	if (m_sysConfig)
	{
		delete m_sysConfig;
	}

	if (m_login)
	{
		delete m_login;
	}

	if (m_vistor)
	{
		m_vistor->m_socketEx.m_bRecvThread = false;
		delete m_vistor;
	}
}

CChatManager* CChatManager::GetInstance()
{
	static CChatManager instance;
	return &instance;
}

void CChatManager::LoginSuccess()
{
	m_bLoginSuccess = true;

	m_commUserInfo.UserInfo.compid = m_userInfo.UserInfo.compid;

	// �յ���¼�û�����Ϣ������¼�ɹ�
	m_handlerLogin->LoginProgress(100);
	m_nOnLineStatus = STATUS_ONLINE;	
	m_timers->SetTimer(1000, TIMER_NORMAL);

	if (m_bKeepPwd)
	{
		m_sysConfig->AddLatestLoginInfo(m_userInfo.UserInfo.uid, m_userInfo.UserInfo.sid,
			m_userInfo.UserInfo.pass, m_bAutoLogin, m_bKeepPwd);
	}
	else
	{
		m_sysConfig->AddLatestLoginInfo(m_userInfo.UserInfo.uid, m_userInfo.UserInfo.sid,
			"", m_bAutoLogin, m_bKeepPwd);
	}

	m_sysConfig->m_nLastLoginBy = m_login->m_nLoginBy;
	switch (m_login->m_nLoginBy)
	{
	case LOGIN_BYUID:
		m_sysConfig->m_sLastLoginUid = StringToDWORD(m_sLogin);
		break;
	case LOGIN_BYSTRING:
		m_sysConfig->m_sLastLoginStr = m_sLogin;
		break;
	default:
		break;
	}

	m_commUserInfo.UserInfo.compid = m_userInfo.UserInfo.compid;
	m_nOnLineStatus = STATUS_ONLINE;
	m_nSendPing = 0;
	m_nSendPingFail = 0;

	// ����Ĭ�������ļ�
	m_sysConfig->SaveData();

	char sFile[MAX_256_LEN];
	string strFile = GetCurrentPath();
	sprintf(sFile, "%s\\config_%u.dat", strFile.c_str(), m_userInfo.UserInfo.uid);
	m_sysConfig->LoadData(sFile);

	SendTo_UpdateOnlineStatus(m_nOnLineStatus);

	if (m_vistor->ConnectToVisitorServer())
	{
		m_vistor->LoginToVisitorServer();
	}

	m_login->CheckLoginFlag(m_sysConfig->m_sLastLoginUid, m_sysConfig->m_sLastLoginStr, true);
}

CWebUserObject * CChatManager::GetWebUserObjectByScriptFlag(char *scriptflag)
{
	return NULL;
}

CUserObject* CChatManager::GetMySelfUserInfo()
{
	return &m_userInfo;
}

void CChatManager::TimerSolveLoginToVisitorServer()
{
	if (m_nLoginToVisitor < 0)
		return;
	if (m_nOnLineStatusEx != STATUS_OFFLINE)
	{
		m_nLoginToVisitor = -20;
		return;

	}
	m_nLoginToVisitor++;

	if (m_nLoginToVisitor > 5)
	{
		//��������
		if (m_vistor->ConnectAndLoginToVisitorServer())
		{
			m_nLoginToVisitor = -20;
		}
		else
			m_nLoginToVisitor = 0;
	}
}

void CChatManager::TimerSolveRequestTimerOut()
{
	MapWebUsers::iterator iter = m_mapWebUsers.begin();
	for (iter; iter != m_mapWebUsers.end(); iter++)
	{
		if (!iter->second->m_bIsShow)
			continue;

		switch (iter->second->onlineinfo.talkstatus)
		{
		case TALKSTATUS_TALK:
			if (iter->second->m_bConnected && iter->second->talkuid == m_userInfo.UserInfo.uid)
			{
				if (m_sysConfig->m_bAutoResp && iter->second->m_resptimeoutmsgtimer >= 0)
				{
					iter->second->m_resptimeoutmsgtimer++;
					int resptimeout =m_sysConfig->m_nVisitorTimeoutTime * 60;
					if (iter->second->m_resptimeoutmsgtimer > resptimeout)
					{
						//3	���˴𸴳�ʱ--����û�з�����Ϣ��ʱ��
						SendAutoRespMsg(iter->second, m_sysConfig->m_sVisitorTimeoutMsg.c_str(), false);
						iter->second->m_resptimeoutmsgtimer = -20;
					}
				}

				if (m_sysConfig->m_bAutoResp && iter->second->m_waitresptimeouttimer >= 0)
				{
					iter->second->m_waitresptimeouttimer++;
					int waitresptimeout = m_sysConfig->m_nUserTimeoutTime * 60;
					if (iter->second->m_waitresptimeouttimer > waitresptimeout)
					{
						//2���˵ȴ��𸴳�ʱ--���˷�����Ϣ��ȴ���ʱ���͵���Ϣ
						SendAutoRespMsg(iter->second, m_sysConfig->m_sUserTimeoutMsg.c_str(), false);
						iter->second->m_waitresptimeouttimer = -20;
					}
				}

				if (m_sysConfig->m_bVisotorTimeoutClose && iter->second->m_resptimeoutclosetimer >= 0)
				{
					iter->second->m_resptimeoutclosetimer++;
					int m_resptimeoutclose = m_sysConfig->m_nVisitorTimeoutCloseTime * 60;
					if (iter->second->m_resptimeoutclosetimer > m_resptimeoutclose)
					{
						//4	���˴𸴳�ʱ�رնԻ�--����û�з�����Ϣ��ʱ�䳬������ֹ�Ի�
						SendAutoRespMsg(iter->second, m_sysConfig->m_sVisitorTimeoutCloseMsg.c_str(), false);
						iter->second->m_resptimeoutclosetimer = -20;

						SendTo_CloseChat(iter->second->webuserid, CHATCLOSE_TIMEOUT);
					}
				}
			}
		}
	}
}

void CChatManager::SendTo_CloseChat(unsigned long webuserid, int ntype)
{
	CWebUserObject* pWebUser = GetWebUserObjectByUid(webuserid);
	if (pWebUser == NULL || pWebUser->onlineinfo.talkstatus == TALKSTATUS_NO)
		return;

	pWebUser->onlineinfo.talkstatus = TALKSTATUS_NO;
	pWebUser->m_bConnected = false;
	pWebUser->cTalkedSatus = HASTALKED;
	pWebUser->m_nWaitTimer = -20;
	pWebUser->m_resptimeoutmsgtimer = -20;
	pWebUser->m_resptimeoutclosetimer = -20;
	pWebUser->m_waitresptimeouttimer = -20;

	char msg[MAX_256_LEN];
	GetStopChatSysMsg(msg, pWebUser, ntype, &m_userInfo);
	if (ntype != CHATCLOSE_INVISTEXIT)
	{

		pWebUser->talkuid = 0;
		pWebUser->RemoveAllMutiUser();

		//����Ҫ��ֹͣ�Ի��ı�ʾ
		if (pWebUser->m_bNewComm)
		{
			int res = SendCloseChat(pWebUser, ntype);
			g_WriteLog.WriteLog(C_LOG_TRACE, "SendTo_CloseChat %d", res);
		}
		else
		{
			SendMsg(pWebUser, "|ForceClose()|", MSG_BAK_INPUTING, "");
			m_vistor->SendWebuserTalkEnd(pWebUser);
		}
	}
	else
	{
		//���Ǳ���������
		if (pWebUser->m_bNewComm)
		{
			SendCloseChat(pWebUser, ntype);
		}
	}
}

void CChatManager::GetStopChatSysMsg(char* msg, CWebUserObject *pWebUser, int type, CUserObject *pSendUser)
{
	if (pWebUser == NULL || pWebUser == NULL)
		return;

	string clientName;
	if (pSendUser != NULL)
	{
		clientName = "�ͷ� ";
		clientName += pSendUser->UserInfo.nickname;
	}
	else
	{
		clientName = "�ͷ�";
	}

	switch (type)
	{
	case CHATCLOSE_TIMEOUT:
		sprintf(msg, "��ÿ� %s �ظ���Ϣ��ʱ���Ի�����", pWebUser->info.name);
		break;
	case CHATCLOSE_USER:
		sprintf(msg, "%s ��ֹ�� %s �ĽӴ�", clientName.c_str(), pWebUser->info.name);
		break;
	case CHATCLOSE_INVISTEXIT:
		sprintf(msg, "������ %s �˳��ÿ� %s �ĻỰ", clientName.c_str(), pWebUser->info.name);
	default:
		break;
	}
}

int CChatManager::SendMsg(IBaseObject* pUser, const char *msg, int bak, char *sfont)
{
	if (pUser->m_nEMObType == OBJECT_WEBUSER)
	{
		CWebUserObject* pWebUser = (CWebUserObject*)pUser;
		pWebUser->m_nWaitTimer = -10;

		if (pWebUser->m_bNewComm)
		{
			return SendFloatMsg(pWebUser, msg, sfont);
		}
		else
		{
			return SendComMsg(pWebUser->webuserid, pWebUser->info.sid, msg, pWebUser->chatid, pWebUser->info.thirdid, bak, sfont);
		}
	}
	else
	{
		CUserObject* pObjUser = (CUserObject*)pUser;
		return SendComMsg(pObjUser->UserInfo.uid, "", msg, "", "", bak);
	}
}

int CChatManager::SendTo_Msg(unsigned long userId, USER_TYPE userType, string msgId, MSG_DATA_TYPE msgDataType, char * msg)
{
	int nError = SYS_FAIL;
	if (userId <= 0 || msgId.empty() || msg == NULL)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "SendTo_Msg params error");
		return nError;
	}
	if (userType == OBJECT_USER)
	{
		CUserObject* pUser = GetUserObjectByUid(userId);
		if (pUser == NULL) return nError;
		switch (msgDataType)
		{
		case MSG_DATA_TYPE_TEXT:
			SendMsg(pUser, msg);
			break;
		case MSG_DATA_TYPE_IMAGE:
			break;
		case MSG_DATA_TYPE_VOICE:
			break;
		case MSG_DATA_TYPE_VIDEO:
			break;
		case MSG_DATA_TYPE_LOCATION:
			break;
		case MSG_DATA_TYPE_FILE:
			break;
		default:
			break;
		}
	}
	else if (userType == OBJECT_WEBUSER)
	{
		CWebUserObject* pWebUser = GetWebUserObjectByUid(userId);
		if (pWebUser == NULL) return nError;
		switch (msgDataType)
		{
		case MSG_DATA_TYPE_TEXT:
			SendMsg(pWebUser, msg);
			break;
		case MSG_DATA_TYPE_IMAGE:
			break;
		case MSG_DATA_TYPE_VOICE:
			break;
		case MSG_DATA_TYPE_VIDEO:
			break;
		case MSG_DATA_TYPE_LOCATION:
			break;
		case MSG_DATA_TYPE_FILE:
			break;
		default:
			break;
		}
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "SendTo_Msg userType:error");
	}
	return nError;
}

int CChatManager::SendGetChatHisMsg(unsigned long webuserid, const char *chatid)
{
	int nError = 0;
	COM_SEND_MSG SendInfo(VERSION);

	SendInfo.msg.msgtype = MSG_KEFU_GET_CHAT_HISMSG;
	SendInfo.msg.recvuin = webuserid;
	SendInfo.msg.sendtime = GetCurrentLongTime();

	SendInfo.msg.bak = 0;
	SendInfo.msg.seq = GetPackSeq();
	SendInfo.version = VERSION;
	SendInfo.msg.senduin = m_userInfo.UserInfo.uid;//���˺���
	strncpy(SendInfo.strChatid, chatid, MAX_CHATID_LEN);

	g_WriteLog.WriteLog(C_LOG_TRACE, "SendGetChatHisMsg: chatid:%s,webuserid:%u,senduin:%u",
		SendInfo.strChatid, SendInfo.msg.recvuin, SendInfo.msg.senduin);
	nError = SendPackTo(&SendInfo);

	return nError;
}

std::string CChatManager::GetLastError()
{
	return "";
}

int CChatManager::ReSendTo_Msg(unsigned long webuserid, int userType, string msgId, int msgDataType, char * msg)
{
	return 1;
}

int CChatManager::ReRecv_Msg(string msgId)
{
	return 1;
}

int CChatManager::SendTo_AcceptChat(unsigned long webuserid)
{
	int nError = SYS_FAIL;
	CWebUserObject* pWebUser = GetWebUserObjectByUid(webuserid);
	if (pWebUser)
	{
		if (m_sysConfig->IsWebuserSidForbid(pWebUser->info.sid))
		{
			m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, "�÷ÿ��ѱ������Σ����Ƚ�����κ��ٽ��ܶԻ�����!");
		}
		else
		{
			if (pWebUser->m_bNewComm)
			{
				COM_FLOAT_ACCEPTCHAT SendInfo(VERSION, pWebUser->gpid);

				SendInfo.uFromAdminid = pWebUser->floatfromadminuid;
				SendInfo.uAdminId = pWebUser->floatadminuid;
				SendInfo.uWebuin = pWebUser->webuserid;
				strcpy(SendInfo.chatid, pWebUser->chatid);
				strcpy(SendInfo.nickname, m_userInfo.UserInfo.nickname);

				g_WriteLog.WriteLog(C_LOG_TRACE, "����float����SendAcceptChat[chatid:%s, uAdminId:%u,uWebuin:%u,Acceptname:%s,senduid:%u]",
					SendInfo.chatid, SendInfo.uAdminId, SendInfo.uWebuin, m_userInfo.UserInfo.nickname, m_userInfo.UserInfo.uid);

				nError = SendPackTo(&SendInfo);
			}
		}
	}

	return nError;
}

int CChatManager::SendTo_ReleaseChat(unsigned long webuserid)
{
	int nError = SYS_FAIL;
	CWebUserObject* pWebUser = GetWebUserObjectByUid(webuserid);
	if (pWebUser)
	{
		CFloatChatRelease RelInfo(VERSION, pWebUser->gpid);
		strcpy(RelInfo.chatid, pWebUser->chatid);
		RelInfo.uKefu = m_userInfo.UserInfo.uid;
		RelInfo.uAdminId = m_login->m_authAdminid;
		strcpy(RelInfo.szKefuName, m_userInfo.UserInfo.nickname);
		RelInfo.webuin = pWebUser->webuserid;
		RelInfo.usReason = REASON_CLIENT_RELEASE;

		nError = SendPackTo(&RelInfo);
	}
	return nError;
}

int CChatManager::SendCloseChat(CWebUserObject* pWebUser, int ntype)
{
	int nError = SYS_FAIL;
	if (pWebUser)
	{
		COM_FLOAT_CLOSECHAT SendInfo(VERSION, pWebUser->gpid);

		SendInfo.uAdminId = pWebUser->floatadminuid;
		SendInfo.webuin = pWebUser->webuserid;
		SendInfo.usType = ntype;
		strcpy(SendInfo.chatid, pWebUser->chatid);
		strcpy(SendInfo.nickname, m_userInfo.UserInfo.nickname);

		g_WriteLog.WriteLog(C_LOG_TRACE, "SendCloseChat[chatid:%s, uAdminId:%u,uWebuin:%u,type:%d,senduid:%u]",
			SendInfo.chatid, SendInfo.uAdminId, SendInfo.webuin, ntype, m_userInfo.UserInfo.uid);
		nError = SendPackTo(&SendInfo);
	}
	return nError;
}

int CChatManager::SendTo_InviteWebUser(CWebUserObject *pWebUser, int type, string strText)
{
	int nError = SYS_FAIL;
	if (pWebUser == NULL)
	{
		if (m_sysConfig->IsWebuserSidForbid(pWebUser->info.sid))
		{
			m_handlerMsgs->RecvMsg(pWebUser, MSG_FROM_SYS, GetMsgId(), MSG_TYPE_NORMAL, MSG_DATA_TYPE_TEXT, "�÷ÿ��ѱ������Σ����Ƚ�����κ�������!");
			return nError;
		}

		char sbuff[MAX_1024_LEN];
		WEBUSER_URL_INFO *pOb = pWebUser->GetLastScriptFlagOb();
		if (pOb == NULL)
			return nError;

		string strScriptFlag = pOb->url;
		if (strText.empty())
			strText = m_sysConfig->m_sInviteWords;

		sprintf(sbuff, "<MSG><COMMAND>%s</COMMAND><NEEDNOTE>NO</NEEDNOTE><TEXT>%s</TEXT><SERVICEUIN>%d</SERVICEUIN><SCRIPTFLAG>%s</SCRIPTFLAG><IP>%s</IP><PORT>%d</PORT><AUTO>0</AUTO></MSG>\r\n",
			GetApplyTypeString(type).c_str(), strText.c_str(), m_userInfo.UserInfo.uid, strScriptFlag.c_str(), pWebUser->info.sip, 2450);

		nError = m_vistor->SendBuffToVisitorServer(sbuff, strlen(sbuff));
	}
	
	return nError;
}

int CChatManager::SendTo_InviteUser(CWebUserObject* pWebUser, CUserObject* pUser)
{
	int nError = 0;
	COM_FLOAT_INVITEREQUEST SendInfo(VERSION, pWebUser->gpid);

	SendInfo.uInviteUser = pUser->UserInfo.uid;
	SendInfo.sSecond = 0;

	SendInfo.uAdminId = pWebUser->floatadminuid;
	strcpy(SendInfo.chatid, pWebUser->chatid);
	SendInfo.uWebuin = pWebUser->webuserid;

	g_WriteLog.WriteLog(C_LOG_TRACE, "��������Э������[chatid:%s, InviteUser:%u,uAdminId:%u,uWebuin:%u]",
		SendInfo.chatid, SendInfo.uInviteUser, SendInfo.uAdminId, SendInfo.uWebuin);

	nError = SendPackTo(&SendInfo);

	return nError;
}

int CChatManager::SendTo_TransferUser(CWebUserObject* pWebUser, CUserObject* pUser)
{
	return 1;
}

int CChatManager::SendTo_InviteUserResult(CWebUserObject* pWebUser, CUserObject* pUser, int result)
{
	int nError = 0;
	unsigned long recvuin = 0;

	if (pUser)
	{
		recvuin = pUser->UserInfo.uid;
	}
	else
	{
		recvuin = pWebUser->frominviteuid;
	}

	COM_FLOAT_INVITERESULT SendInfo(VERSION, pWebUser->gpid);
	SendInfo.sResult = result;
	SendInfo.uInviteFrom = recvuin;
	SendInfo.uAdminId = pWebUser->floatadminuid;
	strcpy(SendInfo.chatid, pWebUser->chatid);
	SendInfo.uWebuin = pWebUser->webuserid;

	g_WriteLog.WriteLog(C_LOG_TRACE, "��������Э�����ܰ�[chatid:%s, sResult:%d,uInviteFrom:%u,uAdminId:%u,uWebuin:%u]",
		SendInfo.chatid, SendInfo.sResult, SendInfo.uInviteFrom, SendInfo.uAdminId, SendInfo.uWebuin);
	return SendPackTo(&SendInfo);
}

int CChatManager::SendComMsg(unsigned long recvuid, char *visitorid, const char *msg, char * chatid, char* thirdid, int bak, char *sfontinfo)
{
	int nError = 0;

	COM_SEND_MSG SendInfo(VERSION);

	SendInfo.msg.msgtype = MSG_NORMAL;
	SendInfo.msg.recvuin = recvuid;
	SendInfo.msg.sendtime = GetCurrentLongTime();
	strncpy(SendInfo.msg.strmsg, msg, MAX_MSG_RECVLEN);
	strncpy(SendInfo.msg.strmobile, m_userInfo.UserInfo.nickname, MAX_MSGNICKNAME_LEN);
	strncpy(SendInfo.msg.strfontinfo, sfontinfo, 8);

	SendInfo.msg.bak = bak;
	SendInfo.msg.seq = GetPackSeq();
	SendInfo.version = VERSION;
	SendInfo.msg.senduin = m_userInfo.UserInfo.uid;//���˺���

	if (chatid != NULL)
	{
		strncpy(SendInfo.strChatid, chatid, MAX_CHATID_LEN);
	}
	if (visitorid != NULL)
	{
		strncpy(SendInfo.strRand, visitorid, MAX_WEBCLIENID_LEN);
	}
	if (thirdid != NULL)
	{
		strncpy(SendInfo.strThirdid, thirdid, MAX_THIRDID_LEN);
	}

	g_WriteLog.WriteLog(C_LOG_TRACE, "SendComMsg [ chatid:%s,recvuin:%u,senduin:%u,nickname:%s,strmsg:%s]",
		SendInfo.strChatid, SendInfo.msg.recvuin, SendInfo.msg.senduin, SendInfo.msg.strmobile, SendInfo.msg.strmsg);

	nError = SendPackTo(&SendInfo);

	return nError;
}

int CChatManager::SendFloatMsg(CWebUserObject *pWebUser, const char *msg, char *sfont)
{
	if (pWebUser == NULL)
		return -1;
	int nError = 0;

	COM_FLOAT_CHATMSG SendInfo(VERSION, pWebUser->gpid);

	SendInfo.uAdminId = pWebUser->floatadminuid;
	SendInfo.webuin = pWebUser->webuserid;
	SendInfo.nMsgType = 2;
	SendInfo.tMsgTime = GetCurrentLongTime();
	strcpy(SendInfo.strfontinfo, sfont);

	strncpy(SendInfo.chatid, pWebUser->chatid, MAX_CHATID_LEN);
	strncpy(SendInfo.nickname, m_userInfo.UserInfo.nickname, MAX_USERNAME_LEN);
	strncpy(SendInfo.strmsg, msg, MAX_MSG_RECVLEN);

	strncpy(SendInfo.strRand, pWebUser->info.sid, MAX_WEBCLIENID_LEN);
	strncpy(SendInfo.strThirdid, pWebUser->info.thirdid, MAX_THIRDID_LEN);

	g_WriteLog.WriteLog(C_LOG_TRACE, "SendFloatMsg [chatid:%s, uAdminId:%u,webuin:%u,tMsgTime:%u,nickname:%s,strmsg:%s,strRand:%s,strThirdid:%s]",
		SendInfo.chatid, SendInfo.uAdminId, SendInfo.webuin, SendInfo.tMsgTime, SendInfo.nickname, SendInfo.strmsg, SendInfo.strRand, SendInfo.strThirdid);

	nError = SendPackTo(&SendInfo);

	return nError;
}

// �ϴ��ļ���΢�ŷ�����
static UINT WINAPI UpLoadFileToWxServerThread(void * pUpLoadInfo)
{
	UPLOAD_INFO* upLoadInfo = (UPLOAD_INFO*)pUpLoadInfo;
	string filePath = upLoadInfo->filePath;
	CChatManager* pThis = (CChatManager*)upLoadInfo->pThis;
	unsigned long userId = upLoadInfo->userId;
	MSG_DATA_TYPE msgDataType = upLoadInfo->msgDataType;
	USER_TYPE userType = upLoadInfo->userType;
	delete upLoadInfo;

	string wxToken;
	CHttpLoad load;
	CCodeConvert convert;
	CWebUserObject* pWebUser = pThis->GetWebUserObjectByUid(userId);
	if (pWebUser == NULL)
		return false;

	MapWxTokens::iterator iter = pThis->m_mapTokens.find(pWebUser->info.thirdid);
	if (iter != pThis->m_mapTokens.end())
	{
		string weChatMediaUploadFormat = pThis->m_initConfig.wechat_media_upload;
		char weChatMediaUpload[MAX_256_LEN];
		wxToken = iter->second;
		string t_filePath = filePath;
		if (msgDataType == MSG_DATA_TYPE_IMAGE)
		{
			sprintf(weChatMediaUpload,weChatMediaUploadFormat.c_str(), wxToken.c_str(), "image");
		}
		else if (msgDataType == MSG_DATA_TYPE_VOICE)
		{
			sprintf(weChatMediaUpload, weChatMediaUploadFormat.c_str(), wxToken.c_str(), "voice");
			t_filePath += ".amr";
		}
		// �ϴ����ļ���΢�ŷ�������΢�ŷ������᷵��һ��json��������һ��media_idֵ
		string returnCode;
		string decodeFilePath = convert.URLDecodeALL(t_filePath);
		if (load.HttpLoad(string(weChatMediaUpload), "", REQUEST_TYPE_POST, decodeFilePath, returnCode))
		{
			Json::Value json;
			if (ParseJson(returnCode, json))
			{
				// ��ȡ��media_idֵ
				string mediaID = GetStrFromJson(json, "media_id");
				if (!mediaID.empty())
				{
					string fileServerMediaTaskFormat = pThis->m_initConfig.fileserver_media_task;
					char fileServerMediaTask[MAX_256_LEN];
					returnCode = "";
					CHttpLoad loadTask;
					sprintf(fileServerMediaTask, fileServerMediaTaskFormat.c_str(), wxToken.c_str(), mediaID.c_str());
					// ���ݸ�media_idֵ��tokenֵ��ȥ�Լ����ļ���������һ��task���ʣ�Ҳ����һ��mediaֵ					
					if (loadTask.HttpLoad(string(fileServerMediaTask), "", REQUEST_TYPE_GET, "", returnCode))
					{
						if (msgDataType == MSG_DATA_TYPE_IMAGE || msgDataType == MSG_DATA_TYPE_VOICE)
						{
							pThis->AfterUpload(userId, userType, mediaID, msgDataType, returnCode, filePath);
						}
						else
						{
							g_WriteLog.WriteLog(C_LOG_ERROR, "δ֪��ʽ���ļ��ϴ���%s", returnCode);
						}
						return true;
					}
					else
					{
						g_WriteLog.WriteLog(C_LOG_ERROR, "û�л�ȡ�ϴ��ļ���΢�ŷ�������task��%s", returnCode.c_str());
					}
				}
				else
				{
					g_WriteLog.WriteLog(C_LOG_ERROR, "û�л�ȡ�ϴ��ļ���΢�ŷ�������media_id��%s", returnCode.c_str());
				}
			}
			else
			{
				g_WriteLog.WriteLog(C_LOG_ERROR, "�����ϴ��ļ���΢�ŷ�����������ʧ�ܣ�%s", returnCode.c_str());
			}
		}
		else
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "�ϴ��ļ���΢�ŷ�����ʧ�ܣ�%s,��ǰtokenֵ��: %s", returnCode.c_str(), wxToken.c_str());
		}
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "�ϴ��ļ���΢�ŷ�����ʧ�ܣ���Ϊû��tokenֵ");
	}

	// ֻҪʧ�ܣ�һ�ɻ�ȡtokenֵ
	pThis->SendGetWxToken(userId, pWebUser->chatid);

	// �������ʧ�ܣ����ļ�·�����룬�������·����õ�
	pThis->AfterUpload(userId, userType, "", msgDataType, "", filePath);
	return false;
}

// �ϴ��ļ���������
static UINT WINAPI UpLoadFileToServerThread(void * para)
{
	//UploadFileParam* param = (UploadFileParam*)para;
	//CString filePath = param->filePath;
	//CFormMain* pForm = param->pForm;
	//int msgDataType = param->msgDataType;
	//int userType = param->userType;
	//unsigned long userId = param->userId;
	//void* pUser = ((CEmocssApp *)AfxGetApp())->GetUserObject(param->userId);
	//delete param;

	//GetPageByURL gpurl;
	//CCodeConvert convert;

	//CString fileServerMediaUploadFormat = ((CEmocssApp *)AfxGetApp())->m_InitConfig.fileserver_media_upload;
	//string resultCode;
	//string decodeFilePath = convert.URLDecodeALL((string)filePath);
	//if (gpurl.HttpLoad(string(fileServerMediaUploadFormat), "", REQUEST_TYPE_POST, decodeFilePath, resultCode))
	//{
	//	Json::Value json;
	//	resultCode = resultCode.substr(1, resultCode.length() - 1);
	//	resultCode = resultCode.substr(0, resultCode.length() - 1);
	//	if (ParseJson(resultCode, json))
	//	{
	//		string mediaID = GetStrFromJson(json, "media_id");
	//		if (!mediaID.empty())
	//		{
	//			// Ŀǰֻ�ж���һ����ϯ��ͼƬ��������߼�
	//			pForm->AfterUpload(userId, userType, mediaID, msgDataType, resultCode, filePath);
	//			return true;
	//		}
	//		else
	//		{
	//			g_WriteLog.WriteLog(C_LOG_ERROR, "��ȡ�ϴ��ļ����ļ�������media_idʧ�ܣ�%s", resultCode.c_str());
	//		}
	//	}
	//	else
	//	{
	//		g_WriteLog.WriteLog(C_LOG_ERROR, "�����ϴ��ļ����ļ�������������ʧ�ܣ�%s", resultCode.c_str());
	//	}
	//}
	//else
	//{
	//	g_WriteLog.WriteLog(C_LOG_ERROR, "�ϴ��ļ����ļ�������ʧ�ܣ�%s", resultCode.c_str());
	//}
	//pForm->AfterUpload(userId, userType, "", msgDataType, "", filePath.GetBuffer(0));
	return false;
}

void CChatManager::UpLoadFile(unsigned long userId, USER_TYPE userType, string filePath, MSG_DATA_TYPE msgDataType)
{
	UPLOAD_INFO* upLoadInfo = new UPLOAD_INFO();
	upLoadInfo->filePath = filePath;
	upLoadInfo->pThis = this;
	upLoadInfo->msgDataType = msgDataType;
	upLoadInfo->userType = userType;
	upLoadInfo->userId = userId;
	if (userType == USER_TYPE_WX)
	{
		_beginthreadex(NULL, 0, UpLoadFileToWxServerThread, (void*)upLoadInfo, 0, NULL);
	}
	else if (userType == USER_TYPE_WEB || userType == USER_TYPE_CLIENT)
	{
		_beginthreadex(NULL, 0, UpLoadFileToServerThread, (void*)upLoadInfo, 0, NULL);
	}
}

bool CChatManager::AfterUpload(unsigned long userId, USER_TYPE userType, string mediaID, MSG_DATA_TYPE msgDataType, string fileId, string filePath, string msgId)
{
	return false;
}


