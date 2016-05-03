#include "../stdafx.h"

#include "comdef.h"
#include "comfunc.h"
#include "pack.h"
#include "comcom.h"
#include "comclt.h"
#include "tstring.h"

//用户登录
CLT_LOGON::CLT_LOGON(unsigned short ver)
{
	this->version = ver;
	seq = 0;

	type_loginsort = LOGIN_NORMAL;		//登录类别  0  normal;  1  offline
	type_loginflag = LOGIN_NORMAL;			//1  0  正常登陆，1 第一次登陆

	type_loginby = LOGIN_BYSTRING;		//0  UID; 1 tstring

	uin = 0;					//4  Tra号码登陆				4
	strcpy(strid, "");
	strcpy(pwd,"");

	status = STATUS_ONLINE;          //在线状态 3 hide,  2 online
	strcpy(strstatus, "");

	connecttype = CONNECT_UDP;
	langtype = LANGUAGE_NORMAL; 
	bak = 0; 

	memset(szRand,0,sizeof(szRand));
	memset(szThirdid,0,sizeof(szThirdid));


	this->m_Head.head.cmd = CMD_CLT_LOGON;

}
CLT_LOGON& CLT_LOGON::operator =(CLT_LOGON& pLogon)
{
	
	seq = pLogon.seq;

	type_loginsort = pLogon.type_loginsort;		//登录类别  0  normal;  1  offline
	type_loginflag = pLogon.type_loginflag;			//1  0  正常登陆，1 第一次登陆

	type_loginby = pLogon.type_loginby;		//0  UID; 1 tstring

	uin = pLogon.uin;					//4  Tra号码登陆				4
	strcpy(strid, pLogon.strid);
	strcpy(pwd,pLogon.pwd);

	status = pLogon.status;          //在线状态 3 hide,  2 online
	strcpy(strstatus, pLogon.strstatus);

	connecttype = pLogon.connecttype;
	langtype = pLogon.langtype; 
	bak = pLogon.bak; 

	this->m_Head.head.cmd = CMD_CLT_LOGON;
	return *this;

}
CLT_LOGON::~CLT_LOGON()
{
}

void CLT_LOGON::SetOnlineStatus(int nNew)
{
	status = SetMutiByte(status, LOGINSTATUS_ONLINE, LOGINSTATUS_ONLINE_LEN, (unsigned short)nNew);
}

int CLT_LOGON::GetOnlineStatus()
{
	return GetMutiByte(status, LOGINSTATUS_ONLINE, LOGINSTATUS_ONLINE_LEN);
}




bool CLT_LOGON::unpack()
{
	//read
//	return this->operator<<(m_Pack);

	bool bRtn = false;
	unsigned short len = 0;
	CPack pack = m_Pack;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> type_loginby))
		goto RETURN;
	if(!(m_Pack >> type_loginflag))
		goto RETURN;
	if(!(m_Pack >> type_loginsort))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;
	if(!m_Pack.unpackString(strid, len, MAX_SID_LEN))
		goto RETURN;
	if(!m_Pack.unpackString(pwd, len, MAX_PASSWORD_LEN))
		goto RETURN;

	if(!(m_Pack >> status))
		goto RETURN;
	if(!m_Pack.unpackString(strstatus, len, MAX_STATUS_LEN))
		goto RETURN;

	if(!(m_Pack >> connecttype))
		goto RETURN;

	if (version>=4210)
	{
		m_Pack>>terminaltype;//终端类型 20090429 djz

	}else
	{
		terminaltype=2;
	}

	if (version>=6000)
	{
		if(!(m_Pack.unpackString(szRand,len,MAX_WEBCLIENID_LEN)))
			szRand[0]=0;
		if(!(m_Pack.unpackString(szThirdid,len,MAX_THIRDID_LEN)))
			szThirdid[0]=0;
	}

	if(!(proxyinfo.unpack(m_Pack, version)))
		goto RETURN;

	if(this->version > 2700)
	{
		if(!(m_Pack >> langtype))
			goto RETURN;
		if(!(m_Pack >> bak))
			goto RETURN;
	}

	bRtn = true;
RETURN:
	return bRtn;

}

bool CLT_LOGON::pack()  //write
{
	//<<write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << type_loginby))
		goto RETURN;

	if(!(m_Pack << type_loginflag))
		goto RETURN;

	if(!(m_Pack << type_loginsort))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;
	if(!m_Pack.packString(strid, MAX_SID_LEN))
		goto RETURN;
	if(!m_Pack.packString(pwd, MAX_PASSWORD_LEN))
		goto RETURN;

	if(!(m_Pack << status))
		goto RETURN;
	if(!m_Pack.packString(strstatus, MAX_STATUS_LEN))
		goto RETURN;

	if(!(m_Pack << connecttype))
		goto RETURN;

	if (version>=4210)
	{
		m_Pack<<terminaltype;//终端类型 20090429 djz
	}
	if (version>=6000)
	{
		if(!(m_Pack.packString(szRand,MAX_WEBCLIENID_LEN)))
			goto RETURN;
		if(!(m_Pack.packString(szThirdid,MAX_THIRDID_LEN)))
		goto RETURN;
	}

	if(!(proxyinfo.pack(m_Pack, version)))
		goto RETURN;

	if(this->version > 2700)
	{
		if(!(m_Pack << langtype))
			goto RETURN;
		if(!(m_Pack << bak))
			goto RETURN;
	}

	bRtn = true;
RETURN:
	return bRtn;
}


//#define CMD_CLT_REG							0x03FC  // 1020 用户注册
CLT_REG::CLT_REG(unsigned short ver)
{
	this->version = ver;
	seq = 0;

	type_loginby = LOGIN_BYUID;	
	strcpy(strid, "");	
	strcpy(pwd, "");	

	ConnectType = CONNECT_UDP;

	this->m_Head.head.cmd = CMD_CLT_REG;
}

CLT_REG::~CLT_REG()
{
}

bool CLT_REG::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> type_loginby))
		goto RETURN;

	if(!m_Pack.unpackString(strid, len, MAX_SID_LEN))
		goto RETURN;
	if(!m_Pack.unpackString(pwd, len, MAX_PASSWORD_LEN))
		goto RETURN;

	if(!(m_Pack >> ConnectType))
		goto RETURN;
	if(!(ProxyInfo.unpack(m_Pack, version)))
		goto RETURN;

	if(!(this->UserInfo.unpack(m_Pack, version)))
		goto RETURN;
	if(!(this->TradeInfo.unpack(m_Pack, version)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_REG::pack()
{
	//<<write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << type_loginby))
		goto RETURN;
	if(!m_Pack.packString(strid, MAX_SID_LEN))
		goto RETURN;
	if(!m_Pack.packString(pwd, MAX_PASSWORD_LEN))
		goto RETURN;

	if(!(m_Pack << ConnectType))
		goto RETURN;
	if(!(ProxyInfo.pack(m_Pack, version)))
		goto RETURN;

	if(!(this->UserInfo.pack(m_Pack, version)))
		goto RETURN;
	if(!(this->TradeInfo.pack(m_Pack, version)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_CLT_LOGOFF						0x0438  // 1080 用户离线
//用户离线发出的包
//CMD_CLT_LOGOFF
CLT_LOGOFF::CLT_LOGOFF(unsigned short ver)
{
	this->version = ver;
	seq = 0;
	uin = 0;
	online_flag = OFFLINE_STATUS_MSG;

	this->m_Head.head.cmd = CMD_CLT_LOGOFF;
}

CLT_LOGOFF::~CLT_LOGOFF()
{
}

bool CLT_LOGOFF::unpack()
{
	//read
	bool bRtn = false;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_LOGOFF::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << online_flag))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_CLT_SEARCH_FRD					0x041A  // 1050 用UIN查询
//通过UIN号码或登陆名, 邮件地址查朋友
//查询谁加我
//通过UIN号码查朋友
CLT_SEARCH_FRD::CLT_SEARCH_FRD(unsigned short ver)
{
	this->version = ver;
	seq = 0;
	type = SEARFRD_ID;

	uin = 0;
	strcpy(strid, "");

	this->m_Head.head.cmd = CMD_CLT_SEARCH_FRD;

}

CLT_SEARCH_FRD::~CLT_SEARCH_FRD()
{
}

bool CLT_SEARCH_FRD::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> type))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;

	switch(type)
	{
	case SEARFRD_ID:
	case SEARFRD_STRID:
	case SEARFRD_ADDME:
	default:
		if(!m_Pack.unpackString(strid, len, MAX_SID_LEN))
			goto RETURN;
		break;
	case SEARFRD_MOBILE:
		if(!m_Pack.unpackString(strid, len, MAX_PHONE_LEN))
			goto RETURN;
		break;
	case SEARFRD_EMAIL:
		if(!m_Pack.unpackString(strid, len, MAX_EMAIL_LEN))
			goto RETURN;
		break;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_SEARCH_FRD::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << type))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;

	switch(type)
	{
	case SEARFRD_ID:
	case SEARFRD_STRID:
	case SEARFRD_ADDME:
	default:
		if(!m_Pack.packString(strid, MAX_SID_LEN))
			goto RETURN;
		break;
	case SEARFRD_MOBILE:
		if(!m_Pack.packString(strid, MAX_PHONE_LEN))
			goto RETURN;
		break;
	case SEARFRD_EMAIL:
		if(!m_Pack.packString(strid, MAX_EMAIL_LEN))
			goto RETURN;
		break;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_CLT_SEARCH_MUTIFRD				0x0424  // 1060 用信息查询
//通过基本信息查询用户（组合查询，里面可以有为空的)
CLT_SEARCH_MUTIFRD::CLT_SEARCH_MUTIFRD(unsigned short ver)
{
	this->version = ver;
	seq = 0;				//查询序号
	sort = 0;				//查询分页

	strcpy(nickname, "");
	strcpy(company, "");

	deptid = 0;		//部门
	country = 0;		//国家 0表示全部	
	province = 0;		//省份 0表示全部
	
	sex = SEX_ALL;			//性别 0 all, 1 man ,  2feman
	onlineuser = 0;	//0:所有人，1：在线用户

	nextpos = 0;		//继续查询的标志值

	this->m_Head.head.cmd = CMD_CLT_SEARCH_MUTIFRD;

	agentID = 0;

}

CLT_SEARCH_MUTIFRD::~CLT_SEARCH_MUTIFRD()
{
	;
}

bool CLT_SEARCH_MUTIFRD::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> sort))
		goto RETURN;

	if(!m_Pack.unpackString(nickname, len, MAX_USERNAME_LEN))
		goto RETURN;
	if(!m_Pack.unpackString(company, len, MAX_COMPANY_LEN))
		goto RETURN;

	if(!(m_Pack >> deptid))
		goto RETURN;
	if(!(m_Pack >> country))
		goto RETURN;
	if(!(m_Pack >> province))
		goto RETURN;
	if(!(m_Pack >> sex))
		goto RETURN;
	if(!(m_Pack >> onlineuser))
		goto RETURN;
	if(!(m_Pack >> nextpos))
		goto RETURN;

	//add at 8.23
	if(!(m_Pack >> agentID))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_SEARCH_MUTIFRD::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << sort))
		goto RETURN;

	if(!m_Pack.packString(nickname, MAX_USERNAME_LEN))
		goto RETURN;
	if(!m_Pack.packString(company, MAX_COMPANY_LEN))
		goto RETURN;

	if(!(m_Pack << deptid))
		goto RETURN;
	if(!(m_Pack << country))
		goto RETURN;
	if(!(m_Pack << province))
		goto RETURN;
	if(!(m_Pack << sex))
		goto RETURN;
	if(!(m_Pack << onlineuser))
		goto RETURN;
	if(!(m_Pack << nextpos))
		goto RETURN;

	//add at 8.23 phenix
	if(!(m_Pack << agentID))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}


//#define CMD_CLT_GET_INFO					0x044C  // 1100 获得服务器的某一些信息资料(包括列表信息)
CLT_GET_INFO::CLT_GET_INFO(unsigned short ver, unsigned short cmd)
{
	this->version = ver;
	this->cmd = cmd;

	this->seq = 0;
	this->cmdtype = 0;
	this->type = 0;

	this->id = 0;
	strcpy(strid, "");

	this->m_Head.head.cmd = CMD_CLT_GET_INFO;

}

CLT_GET_INFO::~CLT_GET_INFO()
{
}

bool CLT_GET_INFO::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> cmd))
		goto RETURN;
	if(!(m_Pack >> cmdtype))
		goto RETURN;

	if(!(m_Pack >> type))
		goto RETURN;
	if(!(m_Pack >> id))
		goto RETURN;

	if(!m_Pack.unpackString(strid, len, MAX_SID_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_GET_INFO::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << cmd))
		goto RETURN;
	if(!(m_Pack << cmdtype))
		goto RETURN;

	if(!(m_Pack << type))
		goto RETURN;
	if(!(m_Pack << id))
		goto RETURN;

	if(!m_Pack.packString(strid, MAX_SID_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}



//#define CMD_CLT_CHECKPROXY					0x0474  // 1140 检查代理服务器
CLT_CHECKPROXY::CLT_CHECKPROXY(unsigned short ver)
{
	this->version = ver;
	connecttype = CONNECT_UDP;
	seq = 0;

	this->m_Head.head.cmd = CMD_CLT_CHECKPROXY;
}

CLT_CHECKPROXY::~CLT_CHECKPROXY()
{
	;
}

bool CLT_CHECKPROXY::unpack()
{
	bool bRtn = false;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> connecttype))
		goto RETURN;

	if(!(proxyinfo.unpack(m_Pack, version)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_CHECKPROXY::pack()
{
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << connecttype))
		goto RETURN;

	if(!(proxyinfo.pack(m_Pack, version)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_CLT_TRANSFERCLIENT				0x01C2  // 450 通知接收者，转移临时用户
CLT_TRANSFERCLIENT::CLT_TRANSFERCLIENT(unsigned short version)
{
	this->version = version;


	this->m_Head.head.cmd = CMD_CLT_TRANSFERCLIENT;

	memset(szChatId,0,sizeof(szChatId));  //会话id
	memset(szRand,0,sizeof(szRand));  //访客唯一标示
	memset(szThirdid,0,sizeof(szThirdid));  //第三方id

}

CLT_TRANSFERCLIENT::~CLT_TRANSFERCLIENT()
{
}


bool CLT_TRANSFERCLIENT::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;
	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> recvinfo.id))
		goto RETURN;
	if(!(m_Pack.unpackString(recvinfo.name, len, MAX_USERNAME_LEN)))
		goto RETURN;

	if(!(m_Pack >> clientinfo.id))
		goto RETURN;
	if(!(m_Pack.unpackString(clientinfo.name, len, MAX_USERNAME_LEN)))
		goto RETURN;

	if(!(m_Pack >> sendinfo.id))
		goto RETURN;
	if(!(m_Pack.unpackString(sendinfo.name, len, MAX_USERNAME_LEN)))
		goto RETURN;

	if (version>=6000)
	{
		if(!(m_Pack.unpackString(szChatId,len,MAX_CHATID_LEN)))
			szChatId[0]=0;
		if(!(m_Pack.unpackString(szRand,len,MAX_WEBCLIENID_LEN)))
			szRand[0]=0;
		if(!(m_Pack.unpackString(szThirdid,len,MAX_THIRDID_LEN)))
			szThirdid[0]=0;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_TRANSFERCLIENT::pack()
{
	//write
	bool bRtn = false;
	if(!(m_Pack << seq))
		goto RETURN;
	
	if(!(m_Pack << recvinfo.id))
		goto RETURN;
	if(!(m_Pack.packString(recvinfo.name, MAX_USERNAME_LEN)))
		goto RETURN;
	
	if(!(m_Pack << clientinfo.id))
		goto RETURN;
	if(!(m_Pack.packString(clientinfo.name, MAX_USERNAME_LEN)))
		goto RETURN;
	
	if(!(m_Pack << sendinfo.id))
		goto RETURN;
	if(!(m_Pack.packString(sendinfo.name, MAX_USERNAME_LEN)))
		goto RETURN;
	
	if(!(m_Pack.packString(szChatId,MAX_CHATID_LEN)))
		goto RETURN;
	if(!(m_Pack.packString(szRand,MAX_WEBCLIENID_LEN)))
		goto RETURN;
	if(!(m_Pack.packString(szThirdid,MAX_THIRDID_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_CLT_TRANSFERCLIENT				0x047E  // 1150 转移临时用户

//add at 7.6 phenix
CLT_UPDATE_FRDLIST::CLT_UPDATE_FRDLIST(unsigned short ver)
{
	version = ver;
	uin = 0;
	num = 0;
}

CLT_UPDATE_FRDLIST::~CLT_UPDATE_FRDLIST()
{}

bool CLT_UPDATE_FRDLIST::pack()
{
	bool bRet = false;
	int i;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << uin))
		goto RETURN;
	if(num > MAX_UPDATE_FRDLIST_NUM)
		num = MAX_UPDATE_FRDLIST_NUM;
	if(!(m_Pack << num))
		goto RETURN;
	for(i=0;i<num;i++)
	{
		if(!(m_Pack << IDList[i]))
			goto RETURN;
	}
	bRet = true;
RETURN:
	return bRet;
}

bool CLT_UPDATE_FRDLIST::unpack()
{
	bool bRet = false;

	unsigned short len = 0;
	int i;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> uin))
		goto RETURN;
	if(!(m_Pack >> num))
		goto RETURN;
	if(num > MAX_UPDATE_FRDLIST_NUM)
		num = MAX_UPDATE_FRDLIST_NUM;
	for(i=0;i<num;i++)
	{
		if(!(m_Pack >> IDList[i]))
			goto RETURN;
		if(!(m_Pack.unpackString(NameList[i], len, MAX_USERNAME_LEN)))
			goto RETURN;
		if(!(m_Pack >> sex[i]))
			goto RETURN;
	}
	bRet = true;
RETURN:
	return bRet;
}