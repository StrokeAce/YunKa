#include "../stdafx.h"
#include "comfloat.h"
#include "comtran.h"

COM_FLOAT_CREATECHAT::COM_FLOAT_CREATECHAT(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_CREATECHAT,ver,sGpid)
{
	uFromAdmin=0;
	uFromSort=0;

	uAdminId=0;
	usort=0;
	uWebuin=0;
	uKefu=0;
	chatid[0]=0;
	clienttid[0]=0;
	webname[0]=0;

	thirdid[0]=0;
}
bool COM_FLOAT_CREATECHAT::pack()
{
	m_Pack<<uFromAdmin;
	m_Pack<<uFromSort;

	m_Pack<<uAdminId;
	m_Pack<<usort;
	m_Pack<<uWebuin;
	m_Pack<<uKefu;

	m_Pack.packString(chatid,MAX_CHATID_LEN);

	m_Pack.packString(clienttid,MAX_WEBCLIENID_LEN);

	m_Pack.packString(webname,MAX_USERNAME_LEN);

	unsigned short num=memlist.size();
	m_Pack<<num;

	for ( map<unsigned int,char> ::iterator it=memlist.begin();it!=memlist.end();++it)
	{
		m_Pack<<it->first;
	}

	m_Pack.packString(thirdid,MAX_THIRDID_LEN);

	return true;
}

bool COM_FLOAT_CREATECHAT::unpack()
{
	m_Pack>>uFromAdmin;
	m_Pack>>uFromSort;

	m_Pack>>uAdminId;
	m_Pack>>usort;
	m_Pack>>uWebuin;
	m_Pack>>uKefu;


	unsigned short len;

	m_Pack.unpackString(chatid,len,MAX_CHATID_LEN);

	m_Pack.unpackString(clienttid,len,MAX_WEBCLIENID_LEN);

	m_Pack.unpackString(webname,len,MAX_USERNAME_LEN);

	unsigned short num(0);
	m_Pack>>num;

	unsigned int uin;
	for ( int i=0;i<num;++i)
	{
		if (m_Pack>>uin)
		{
			memlist[uin]=0;
		}else
		{
			break;
		}
	}

	if (m_Head.head.ver>=6000)
	{
		m_Pack.unpackString(thirdid,len,MAX_THIRDID_LEN);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
COM_FLOAT_GETLIST::COM_FLOAT_GETLIST(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_LISTCHAT,ver,sGpid )
{
	uAdminId=0;
}
bool COM_FLOAT_GETLIST::pack()
{
	m_Pack<<uAdminId;
	return true;
}


//////////////////////////////////////////////////////////////////////////

bool COM_FLOAT_RECVLIST::unpack()
{

	if (m_Pack.GetPackLength()<2)
	{
		return false;
	}
	m_Pack>>uAdminId;
	unsigned short num(0);
	if (m_Pack>>num)
	{
		LISTCHATINFO  rc;
		unsigned short len;

		for (int i=0;i<num;++i)
		{
			m_Pack>>rc.uKefuUin;
			if (!m_Pack.unpackString(rc.chatid,len,MAX_CHATID_LEN))
			{
				break;
			}
			recvchatlist.push_back(rc);
		}
	}
	

	return true;
}

//----------------会话组详细信息包----------------------
COM_FLOAT_CHATINFO::COM_FLOAT_CHATINFO(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_CHATINFO,ver,sGpid)
{
	uAdminId=0;
	chatid[0]=0;
	sSort=0;
	tStartTime=0;uFromAmdind=0; sFromSort=0;
	uWebUin=0;
	uKefuUin=0;
	memset(webnickname, 0, sizeof(webnickname));
	memset(strClientId, 0, sizeof(strClientId));
	
	uTansferingToAdmin=0;
	uTansferingToKefu=0;

	strThirdId[0]=0;
	
}
bool COM_FLOAT_CHATINFO::pack()
{
	m_Pack<<uAdminId;
	m_Pack.packString(chatid,MAX_CHATID_LEN);
	return true;
}

bool COM_FLOAT_CHATINFO::unpack()
{
	if (m_Pack.GetPackLength()<10)
	{
		return false;
	}
	m_Pack>>uAdminId;
	unsigned short len;
	m_Pack.unpackString(chatid,len,MAX_CHATID_LEN);
	
	m_Pack>>sSort;
	m_Pack>>tStartTime;
	m_Pack>>uFromAmdind;
	m_Pack>>sFromSort;
	
	m_Pack>>uWebUin;
	m_Pack>>uKefuUin;
	
	m_Pack.unpackString(webnickname,len,MAX_USERNAME_LEN);
	m_Pack.unpackString(strClientId,len,MAX_WEBCLIENID_LEN);
	
	m_Pack>>uTansferingToAdmin;
	m_Pack>>uTansferingToKefu;
	
	unsigned short webuinnum;
	if (m_Pack>>webuinnum)
	{
		unsigned int webuin;
		for (int i=0;i<webuinnum;++i)
		{
			if (m_Pack>>webuin)
			{
				webRecvUin.push_back(webuin);
			}else
			{
				break;
			}
		}
	}
	
	unsigned short inviteuinnum(0);
	if (m_Pack>>inviteuinnum)
	{
		unsigned int inviteuin;
		for (int i=0;i<inviteuinnum;++i)
		{
			if (m_Pack>>inviteuin)
			{
				InviteUin.push_back(inviteuin);
			}else
			{
				break;
			}
		}
	}

	m_Pack.unpackString(strThirdId,len,MAX_THIRDID_LEN);

	return true;
}

//--------------------------------------
COM_FLOAT_CHATMSG::COM_FLOAT_CHATMSG(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_CHATMSG,ver,sGpid)
{
	uAdminId=0;
	nMsgType=0;
	nMsgDataType = MSG_DATA_TYPE_TEXT;
	chatid[0]=0;
	nickname[0]=0;
	strmsg[0]=0;
	strfontinfo[0]=0;

	strRand[0]=0;
	strThirdid[0]=0;
	//wxMsg = NULL;
}
bool COM_FLOAT_CHATMSG::pack()
{
	m_Pack<<uAdminId;
	m_Pack<<webuin;
	m_Pack<<nMsgType;
	m_Pack.packString(chatid,MAX_CHATID_LEN);
	m_Pack.packString(nickname,MAX_USERNAME_LEN);
	m_Pack<<tMsgTime;
	if (!m_Pack.packString(strmsg,MAX_MSG_RECVLEN))
	{
		return false;
	}
	if (!m_Pack.packString(strfontinfo,MAX_MSGFONT_LEN))
	{
		return false;
	}

	if (version>=6000)
	{
		
		if(!(m_Pack.packString(strRand,MAX_WEBCLIENID_LEN)))
			return false;
		
		if(!(m_Pack.packString(strThirdid,MAX_THIRDID_LEN)))
			return false;
	}

	return true;
}

bool COM_FLOAT_CHATMSG::unpack()
{
	if (m_Pack.GetPackLength()<10)
	{
		return false;
	}
	m_Pack>>uAdminId;
	m_Pack>>webuin;
	m_Pack>>nMsgType;

	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	if (!m_Pack.unpackString(nickname,len,MAX_USERNAME_LEN))
	{
		return false;
	}
	m_Pack>>tMsgTime;

	if (!m_Pack.unpackString(strmsg,len,MAX_MSG_RECVLEN))
	{
		return false;
	}
	if (!m_Pack.unpackString(strfontinfo,len,MAX_MSGFONT_LEN))
	{
		return false;
	}

	if (version>=6000)
	{
		m_Pack.unpackString(strRand,len,MAX_WEBCLIENID_LEN);
		m_Pack.unpackString(strThirdid,len,MAX_THIRDID_LEN);
	}

	return true;
}
//--------------------------------------
COM_FLOAT_CHATMSG_ACK::COM_FLOAT_CHATMSG_ACK(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_CHATMSG_ACK,ver,sGpid)
{
	uAdminId=0;
	webuin=0;
	chatid[0]=0;
	chatseq=0;

}
bool COM_FLOAT_CHATMSG_ACK::pack()
{
	m_Pack<<uAdminId;
	m_Pack.packString(chatid,MAX_CHATID_LEN);
	m_Pack<<webuin;
	m_Pack<<chatseq;

	return true;
}

bool COM_FLOAT_CHATMSG_ACK::unpack()
{
	if (m_Pack.GetPackLength()<12)
	{
		return false;
	}
	m_Pack>>uAdminId;
	unsigned short len;
	m_Pack.unpackString(chatid,len,MAX_CHATID_LEN);
	m_Pack>>webuin;
	if (m_Pack>>chatseq)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
COM_FLOAT_STARTORSTOPRECV::COM_FLOAT_STARTORSTOPRECV(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_STARTORSTOP_MSG,ver,sGpid )
{
	uAdminId=0;
	sAcitonType=0;
	sLastMsgid=0;
}
bool COM_FLOAT_STARTORSTOPRECV::pack()
{
	m_Pack<<uAdminId;
	m_Pack<<sAcitonType;
	m_Pack<<sLastMsgid;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
COM_FLOAT_CLOSECHAT::COM_FLOAT_CLOSECHAT(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_CLOSECHAT,ver,sGpid )
{
	uAdminId=0;
	chatid[0]=0;
	usType=0;
	webuin=0;

}
bool COM_FLOAT_CLOSECHAT::pack()
{
	m_Pack<<uAdminId;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack<<usType;
	m_Pack<<webuin;
	if (!m_Pack.packString(nickname,MAX_USERNAME_LEN))
	{
		return false;
	}

	return true;
}
bool COM_FLOAT_CLOSECHAT::unpack()
{
	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack>>usType;
	m_Pack>>webuin;
	if (!m_Pack.unpackString(nickname,len,MAX_USERNAME_LEN))
	{
		nickname[0]=0;
	}
	return true;
}



//////////////////////////////////////////////////////////////////////////
COM_FLOAT_GETCHATMSG::COM_FLOAT_GETCHATMSG(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_GETCHATMSG,ver,sGpid )
{
	uAdminId=0;
	chatid[0]=0;
	sBeginSeq=0;
	sEndSeq=0;
}
bool COM_FLOAT_GETCHATMSG::pack()
{
	m_Pack<<uAdminId;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack<<sBeginSeq;
	m_Pack<<sEndSeq;


	return true;
}



/////////////////////会话转移请求/////////////////////////////////////////////////////
COM_FLOAT_TRANSREQUEST::COM_FLOAT_TRANSREQUEST(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_TRANSQUEST,ver,sGpid )
{
	uAdminId=0;
	chatid[0]=0;
	uWebuin=0;
	uToAdminId=0;
	uToKefu=0;
}
bool COM_FLOAT_TRANSREQUEST::pack()
{
	m_Pack<<uAdminId;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack<<uWebuin;
	m_Pack<<nTimeOutSecond;

	m_Pack<<uToAdminId;
	m_Pack<<uToKefu;

	unsigned short num=memlist.size();
	m_Pack<<num;

	for ( map<unsigned int,char> ::iterator it=memlist.begin();it!=memlist.end();++it)
	{
		m_Pack<<it->first;
	}

	return true;
}
bool COM_FLOAT_TRANSREQUEST::unpack()
{
	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack>>uWebuin;
	m_Pack>>nTimeOutSecond;

	m_Pack>>uToAdminId;
	m_Pack>>uToKefu;

	unsigned short num(0);
	m_Pack>>num;

	unsigned int uin;
	for ( int i=0;i<num;++i)
	{
		if (m_Pack>>uin)
		{
			memlist[uin]=0;
		}else
		{
			break;
		}
	}
	return true;
}

/////////////////////会话转移失败/////////////////////////////////////////////////////
COM_FLOAT_TRANSFAILED::COM_FLOAT_TRANSFAILED(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_TRANSFAILED,ver,sGpid )
{
	uAdminId=0;
	chatid[0]=0;
	uWebuin=0;
	uKefu = 0;
	TransToAdmin = 0;
	TransToKefu = 0;
}
bool COM_FLOAT_TRANSFAILED::pack()
{
	m_Pack<<uAdminId;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack<<uWebuin;

	m_Pack << uKefu;
	m_Pack << TransToAdmin;
	m_Pack << TransToKefu;

	return true;
}
bool COM_FLOAT_TRANSFAILED::unpack()
{
	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack>>uWebuin;


	m_Pack >> uKefu;
	m_Pack >> TransToAdmin;
	m_Pack >> TransToKefu;

	return true;
}
//////////////////////////////接受会话////////////////////////////////////////////
COM_FLOAT_ACCEPTCHAT::COM_FLOAT_ACCEPTCHAT(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_ACCEPTCHAT,ver,sGpid )
{
	uFromAdminid=0;
	uAdminId=0;
	chatid[0]=0;
	uWebuin=0;
}
bool COM_FLOAT_ACCEPTCHAT::pack()
{
	m_Pack<<uFromAdminid;
	m_Pack<<uAdminId;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack<<uWebuin;
	if (!m_Pack.packString(nickname,MAX_USERNAME_LEN))
	{
		return false;
	}
	return true;
}
bool COM_FLOAT_ACCEPTCHAT::unpack()
{
	m_Pack>>uFromAdminid;
	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack>>uWebuin;
	if (!m_Pack.unpackString(nickname,len,MAX_USERNAME_LEN))
	{
		nickname[0]=0;
	}	
	return true;
}


//////////////////////////////////////////////////////////////////////////
COM_FLOAT_SHARELIST::COM_FLOAT_SHARELIST(unsigned short ver)
{
	version=ver;
	m_Head.head.cmd=CMD_FLOAT_SHARELIST;
}

bool COM_FLOAT_SHARELIST::unpack()
{
	unsigned short sharelistnum(0);
	m_Pack>>sharelistnum;
	
	SHAREUSEROBJ so;
	for (int i=0;i<sharelistnum;++i)
	{
		m_Pack>>so.uin;
		m_Pack>>so.cRight;
		if (! (m_Pack>>so.onlineflag))
		{
			break;
		}
		sharememlist.push_back(so);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
COM_FLOAT_ERROR::COM_FLOAT_ERROR(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_CMDERROR,ver,sGpid )
{
	errcmd=errornum=uAdminId=0;
	chatid[0]=0;
}
bool COM_FLOAT_ERROR::unpack()
{
	m_Pack>>errcmd;
	m_Pack>>errornum;
	m_Pack>>uAdminId;
	m_Pack>>uWebuin;
	
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	return true;
}

/////////////////////请求会话协助/////////////////////////////////////////////////////
COM_FLOAT_INVITEREQUEST::COM_FLOAT_INVITEREQUEST(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_INVITE_REQUEST,ver,sGpid )
{
	uInviteUser=0;
	uAdminId=0;
	uWebuin=0;
	chatid[0]=0;
	sSecond=60;
}
bool COM_FLOAT_INVITEREQUEST::pack()
{
	m_Pack<<uInviteUser;
	m_Pack<<sSecond;

	m_Pack<<uAdminId;
	m_Pack.packString(chatid,MAX_CHATID_LEN);
	m_Pack<<uWebuin;

	return true;
}
bool COM_FLOAT_INVITEREQUEST::unpack()
{
	m_Pack>>uInviteUser;
	m_Pack>>sSecond;

	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}

	if (!(m_Pack>>uWebuin))
		return false;

	return true;
}
/////////////////////会话协助应答/////////////////////////////////////////////////////
COM_FLOAT_INVITERESULT::COM_FLOAT_INVITERESULT(unsigned short ver,unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_INVITE_RESULT,ver,sGpid )
{
	sResult=60;
	uInviteFrom=0;
	uAdminId=0;
	uWebuin=0;
	chatid[0]=0;
}
bool COM_FLOAT_INVITERESULT::pack()
{
	m_Pack<<sResult;
	m_Pack<<uInviteFrom;

	m_Pack<<uAdminId;
	m_Pack.packString(chatid,MAX_CHATID_LEN);

	m_Pack<<uWebuin;

	return true;
}
bool COM_FLOAT_INVITERESULT::unpack()
{
	m_Pack>>sResult;
	m_Pack>>uInviteFrom;

	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}

	if (!(m_Pack>>uWebuin))
		return false;

	return true;
}

///////////////////////////客服应答超时///////////////////////////////////////////////
CFloatChatRelease::CFloatChatRelease(unsigned short ver, unsigned short sGpid)
:COM_FLOAT_CHATPACK(CMD_FLOAT_RELEASE,ver,sGpid )
{
	uAdminId=0;
	chatid[0]=0;
	webuin=0;
	uKefu=0;
	szKefuName[0]=0;

	usReason = REASON_TIME_OUT;           //原因
// 	nNewKefu=0;           //如果会话已邀请协助，一般为最后一位协助对象
}
bool CFloatChatRelease::pack()
{
	m_Pack<<uAdminId;
	if (!m_Pack.packString(chatid,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack<<webuin;
	
	m_Pack<<uKefu;
	
	if (!m_Pack.packString(szKefuName,MAX_USERNAME_LEN))
	{
		return false;
	}	

	/*
	客户端主动释放会话不用打包
	usReason，nNewKefu,服务器不解析这两个字段
	*/

	return true;
}
bool CFloatChatRelease::unpack()
{
	m_Pack>>uAdminId;
	unsigned short len;
	if (!m_Pack.unpackString(chatid,len,MAX_CHATID_LEN))
	{
		return false;
	}
	m_Pack>>webuin;
	
	m_Pack>>uKefu;

	if (!m_Pack.unpackString(szKefuName,len,MAX_USERNAME_LEN))
	{
		szKefuName[0]=0;
	}	

	if (version>=3075) //服务器版本
	{
		unsigned short nReason;
		m_Pack>>usReason;
	}

	return true;
}

