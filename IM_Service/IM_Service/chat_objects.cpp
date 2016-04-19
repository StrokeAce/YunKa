#include "stdafx.h"
#include "chat_objects.h"


CChatObjects::CChatObjects()
{
}


CChatObjects::~CChatObjects()
{
}

void CChatObjects::OnReceive(void* pHead, void* pData)
{
	TCP_PACK_HEADER TcpPackHead;

	int nPackHeadLen = COM_HEAD_PACK::GetHeadLen();
	int nTcpPackHeadLen = sizeof(TCP_PACK_HEADER);

	int nError = 0;
	int nErrType = 0;
	CPackInfo packinfo;
	bool bLast = false;

	TcpPackHead = *((TCP_PACK_HEADER *)pHead);
	assert(TcpPackHead.len <= PACKMAXLEN);


	char *RecvBuf = (char*)pData;

	PACK_HEADER head = *((PACK_HEADER*)RecvBuf);

	g_WriteLog.WriteLog(C_LOG_TRACE, "Mainfrm recv Cmd:%.4x", head.cmd);

	switch (head.cmd)
	{
	case CMD_COM_ACK:
		break;
	case CMD_SRV_REP_USERINFO:  //收到用户信息包
		//nError = RecvSrvRepUserinfo(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_CONF_LOGOFF:   //登出确认包
		//nError = RecvSrvConfLogOff(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_FRDONLINE: //好友上线
		//nError = RecvSrvStatusFrdOnline(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_FRDOFFLINE: //好友下线
		//nError = RecvSrvStatusFrdOffline(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_STATUS_USER_FORM:   //好友更改在线状态
		//nError = RecvSrvStatusUserForm(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_SHARELIST:  //收到好友对象列表
		//nError = RecvFloatShareList(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_COM_SEND_MSG:
		//nError = RecvComSendMsg(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CREATECHAT: //接入会话
		//nError = RecvFloatCreateChat(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATMSG:  //会话消息
		//nError = RecvFloatChatMsg(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATMSG_ACK: //发送消息应答
		//nError = RecvFloatChatMsgAck(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_ACCEPTCHAT:  //接受会话
		//nError = RecvFloatAcceptChat(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_TRANSQUEST:		//等待应答  会话转移请求
		//nError = RecvFloatTransQuest(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_TRANSFAILED:   //等待应答  会话转移失败
		//nError = RecvFloatTransFailed(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_INVITE_REQUEST://等待应答  邀请请求
		//nError = RecvInviteRequest(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_INVITE_RESULT://等待应答  邀请结果
		//nError = RecvInviteResult(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_KEFU_RELEASE: //坐席主动释放会话
		//nError = RecvFloatKefuRelease(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CMDERROR:  //等待应答  命令失败
		//nError = RecvFloatCMDError(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CLOSECHAT: //会话关闭
		//nError = RecvFloatCloseChat(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_LISTCHAT:  //会话列表
		//nError = RecvFloatListChat(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_FLOAT_CHATINFO:  //会话详细信息
		//nError = RecvFloatChatInfo(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_EVENT_ANNOUNCEMENT:  //收到消息通告
		//nError = RecvEventAnnouncement(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_UPDATE_SUCC:  //更新信息成功
		//nError = RecvSrvUpdateSucc(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_UPDATE_FAIL:  //更新信息失败
		//nError = RecvSrvUpdateFail(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_SERVER_COPY:   //用户在异地登陆
		//nError = RecvSrvDonw(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_REP_TRANSFERCLIENT://			0x01B8  // 440 转移临时用户成功失败
		//nError = RecvRepTransferClient(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;
	case CMD_SRV_TRANSFERCLIENT:	//0x01C2  接收转移会话的用户处理
		//nError = RecvTransferClient(head, RecvBuf + nPackHeadLen, TcpPackHead.len - nPackHeadLen);
		break;

	default:
		break;
	}
	return;
}
