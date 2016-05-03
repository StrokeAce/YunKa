#include "../stdafx.h"
#include "comdef.h"
#include "comfunc.h"
#include "tstring.h"
#include "comtran.h"
#include "pack.h"
#include "comcom.h"
#include "comsrv.h"

//#define CMD_SRV_ERROR_CLTSEND			0x0000	//  0  客户端端传送一个错误的命令
SRV_ERROR_CLTSEND::SRV_ERROR_CLTSEND(unsigned short ver)
{
	this->version = ver;
	this->type = 0;
	this->cmd = 0;
	memset(strMemo, 0, MAX_RESPONSE_LEN);

	this->m_Head.head.cmd = CMD_SRV_ERROR_CLTSEND;
}

SRV_ERROR_CLTSEND::~SRV_ERROR_CLTSEND()
{
	;
}

bool SRV_ERROR_CLTSEND::unpack()  //read
{
	bool bRtn = false;
	unsigned short len = 0;
	
	if(!(m_Pack >> type))
		goto RETURN;
	if(!(m_Pack >> cmd))
		goto RETURN;

	if(!m_Pack.unpackString(strMemo, len, MAX_RESPONSE_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_ERROR_CLTSEND::pack()  //write
{
	bool bRtn = false;

	if(!(m_Pack << type))
		goto RETURN;
	if(!(m_Pack << cmd))
		goto RETURN;

	if(!m_Pack.packString(strMemo, MAX_RESPONSE_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_RESP_FAIL					0x0014	//  20 服务器回应处理某一个包失败
SRV_RESP_FAIL::SRV_RESP_FAIL(unsigned short ver, unsigned short seq, unsigned short cmd)
{
	this->version = ver;
	this->seq = seq;
	this->cmd = cmd;
	this->cmdtype = 0;

	this->type = 0;
	typecmd = 0;
	this->uin = 0;
	this->langtype = LANGUAGE_NORMAL;

	this->deny = 0;
	memset(reason, 0, MAX_RESPONSE_LEN);
	memset(strmemo, 0, MAX_RESPONSE_LEN);

	this->m_Head.head.cmd = CMD_SRV_RESP_FAIL;
}

SRV_RESP_FAIL::SRV_RESP_FAIL(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;
	this->cmd = 0;
	this->cmdtype = 0;

	this->type = 0;
	typecmd = 0;
	this->uin = 0;
	this->langtype = LANGUAGE_NORMAL;

	this->deny = 0;
	memset(reason, 0, MAX_RESPONSE_LEN);
	memset(strmemo, 0, MAX_RESPONSE_LEN);
}


SRV_RESP_FAIL::~SRV_RESP_FAIL()
{
}

bool SRV_RESP_FAIL::unpack()
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
	if(!(m_Pack >> typecmd))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;
	if(!(m_Pack >> langtype))
		goto RETURN;
	if(!(m_Pack >> deny))
		goto RETURN;

	if(!(m_Pack.unpackString(reason, len, MAX_RESPONSE_LEN)))
		goto RETURN;
	if(!(m_Pack.unpackString(strmemo, len, MAX_RESPONSE_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_RESP_FAIL::pack()
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
	if(!(m_Pack << typecmd))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;
	if(!(m_Pack << langtype))
		goto RETURN;
	if(!(m_Pack << deny))
		goto RETURN;

	if(!(m_Pack.packString(reason, MAX_RESPONSE_LEN)))
		goto RETURN;
	if(!(m_Pack.packString(strmemo, MAX_RESPONSE_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}


//#define CMD_SRV_SERVER_COPY					0x001E	//  30 服务器当机
SRV_SERVER_COPY::SRV_SERVER_COPY(unsigned short ver)
{
	this->version = ver;

	this->type = 0;
	this->uin = 0;
	this->seq = 0;

	this->m_Head.head.cmd = CMD_SRV_SERVER_COPY;
}

SRV_SERVER_COPY::~SRV_SERVER_COPY()
{
}


bool SRV_SERVER_COPY::unpack()  //read
{
	bool bRtn = false;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> type))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;


	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_SERVER_COPY::pack()  //write
{
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << type))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_CONF_LOGOFF					0x0028	//  40 确认某人离线
//服务器确认用户已经离线
SRV_CONF_LOGOFF::SRV_CONF_LOGOFF(unsigned short ver)
{
	this->version = ver;
	seq = 0;
	type = 0;
	uin = 0;

	this->m_Head.head.cmd = CMD_SRV_CONF_LOGOFF;
}

SRV_CONF_LOGOFF::~SRV_CONF_LOGOFF()
{
}

bool SRV_CONF_LOGOFF::unpack()
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

bool SRV_CONF_LOGOFF::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}


//#define CMD_SRV_SERVER_BUSY					0x003C	//  60 服务器忙
SRV_SERVER_BUSY::SRV_SERVER_BUSY(unsigned short ver)
{
	this->version = ver;

	this->seq = 0;

	this->m_Head.head.cmd = CMD_SRV_SERVER_BUSY;
}

SRV_SERVER_BUSY::~SRV_SERVER_BUSY()
{
	;
}


bool SRV_SERVER_BUSY::unpack()
{
	bool bRtn = false;

	if(!(m_Pack >> seq))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_SERVER_BUSY::pack()
{
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_CONF_REG					0x0046	//  70 确认注册新用户
//服务器确认注册包
SRV_CONF_REG::SRV_CONF_REG(unsigned short ver)
{
	this->version = ver;

	this->m_Head.head.cmd = CMD_SRV_CONF_REG;
}

SRV_CONF_REG::~SRV_CONF_REG()
{
}


bool SRV_CONF_REG::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> type_loginby))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;
	if(!m_Pack.unpackString(strid,len,MAX_SID_LEN))
		goto RETURN;
	if(!m_Pack.unpackString( pwd,len,MAX_PASSWORD_LEN))
		goto RETURN;

	if(!m_Pack.unpackString(strMemo,len,MAX_FILENAME_LEN))
		goto RETURN;
	if(!m_Pack.unpackString(strAddress,len,MAX_URL_LEN))
		goto RETURN;

	if(this->version > 2555)
	{
		if(!(m_Pack >> videoip))
			goto RETURN;
		if(!(m_Pack >> videoport))
			goto RETURN;
	}
	bRtn = true;

RETURN:
	return bRtn;
}

bool SRV_CONF_REG::pack()  //write
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << type_loginby))
		goto RETURN;
	if(!(m_Pack << uin))
		goto RETURN;

	if(!m_Pack.packString(strid,MAX_SID_LEN))
		goto RETURN;
	if(!m_Pack.packString(pwd,MAX_PASSWORD_LEN))
		goto RETURN;

	if(!m_Pack.packString(strMemo,MAX_FILENAME_LEN))
		goto RETURN;
	if(!m_Pack.packString(strAddress,MAX_URL_LEN))
		goto RETURN;

	if(this->version > 2555)
	{
		if(!(m_Pack << videoip))
			goto RETURN;
		if(!(m_Pack << videoport))
			goto RETURN;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_DENY_REG					0x0050	//  80 拒绝注册新用户
//服务器拒绝注册新用户
SRV_DENY_REG::SRV_DENY_REG(unsigned short ver)
{
	this->version = ver;

	seq = 0;					//2
	type_loginby = LOGIN_BYUID;		//2  0  UID; 1 tstring

	deny = 0;				//2  拒绝的类型 0  字符串登录号存在    5 版本太旧	
	memset(reason, 0, MAX_RESPONSE_LEN);//255  拒绝的类型描述		
	memset(strAddress, 0, MAX_URL_LEN);//255 下载地址

	this->m_Head.head.cmd = CMD_SRV_DENY_REG;
}

SRV_DENY_REG::~SRV_DENY_REG()
{
}


bool SRV_DENY_REG::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> type_loginby))
		goto RETURN;

	if(!(m_Pack >> deny))
		goto RETURN;
	if(!m_Pack.unpackString(reason,len,MAX_RESPONSE_LEN))
		goto RETURN;

	if(!m_Pack.unpackString(strAddress,len,MAX_URL_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_DENY_REG::pack()  //write
{
	//<<write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
		
	if(!(m_Pack << type_loginby))
		goto RETURN;

	if(!(m_Pack << deny))
		goto RETURN;
	if(!m_Pack.packString(reason,MAX_FILENAME_LEN))
		goto RETURN;

	if(!m_Pack.packString(strAddress,MAX_URL_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_CONF_LOGON					0x005A	//  90 确认用户登录
//服务器确认用户登录
SRV_CONF_LOGON::SRV_CONF_LOGON(unsigned short ver)
{
	this->version = ver;

	seq = 0;
	type_loginby = LOGIN_BYUID;				//2  0  UID; 1 tstring
	type_loginflag = LOGIN_NORMAL;
	type_loginsort = LOGIN_NORMAL;

	uin = 0;
	memset(strid, 0, MAX_SID_LEN);
	memset(stridmobile, 0, MAX_PHONE_LEN);
	memset(stridmail, 0, MAX_EMAIL_LEN);
	memset(stridbak, 0, MAX_SID_LEN);

	status  = STATUS_ONLINE;						//2  状态
	memset(strStatus, 0, MAX_STATUS_LEN);//255 描述
	memset(strMemo, 0, MAX_RESPONSE_LEN);
	memset(strAddress, 0, MAX_URL_LEN);//255 下载地址

	videoip = 0;						//4
	videoport = 0;					//2

	ip = 0;							//4
	port = 0;						//2

	servertype = TRACQTYPE_TRACQ;					//1
	sublogon = SUBLOGON_NORMAL;							//1

	tcpip = 0;						//4
	tcpport = 0;						//2

	this->m_Head.head.cmd = CMD_SRV_CONF_LOGON;
}


SRV_CONF_LOGON::~SRV_CONF_LOGON()
{
}



bool SRV_CONF_LOGON::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;

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
	if(!m_Pack.unpackString(strid,len,MAX_SID_LEN))
		bRtn = false;

	if(this->version > 2602)
	{
		if(!m_Pack.unpackString(stridmobile,len,MAX_PHONE_LEN))
			goto RETURN;
		if(!m_Pack.unpackString(stridmail,len,MAX_EMAIL_LEN))
			goto RETURN;
		if(!m_Pack.unpackString(stridbak,len,MAX_EMAIL_LEN))
			goto RETURN;
	}
	
	m_Pack >> this->status;
	if(!m_Pack.unpackString(this->strStatus,len,MAX_STATUS_LEN))
		goto RETURN;

	if(!m_Pack.unpackString(strMemo,len,MAX_RESPONSE_LEN))
		goto RETURN;
	if(!m_Pack.unpackString(strAddress,len,MAX_URL_LEN))
		goto RETURN;

	if(this->version > 2555)
	{
		if(!(m_Pack >> videoip))
			goto RETURN;
		if(!(m_Pack >> videoport))
			goto RETURN;
	}

	if(this->version > 2601)
	{
		if(!(m_Pack >> ip))
			goto RETURN;
		if(!(m_Pack >> port))
			goto RETURN;
	}


	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_CONF_LOGON::pack()  //write
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
	if(!m_Pack.packString(strid,MAX_SID_LEN))
		goto RETURN;

	if(this->version > 2602)
	{
		if(!m_Pack.packString(stridmobile,MAX_PHONE_LEN))
			goto RETURN;
		if(!m_Pack.packString(stridmail,MAX_EMAIL_LEN))
			goto RETURN;
		if(!m_Pack.packString(stridbak,MAX_EMAIL_LEN))
			goto RETURN;
	}

	if(!(m_Pack << status))
		goto RETURN;
	if(!m_Pack.packString(this->strStatus,MAX_STATUS_LEN))
		goto RETURN;

	if(!m_Pack.packString(strMemo,MAX_RESPONSE_LEN))
		goto RETURN;
	if(!m_Pack.packString(strAddress,MAX_URL_LEN))
		goto RETURN;

	if(this->version > 2555)
	{
		if(!(m_Pack << videoip))
			goto RETURN;
		if(!(m_Pack << videoport))
			goto RETURN;
	}

	if(this->version > 2601)
	{
		if(!(m_Pack << ip))
			goto RETURN;
		if(!(m_Pack << port))
			goto RETURN;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_DENY_LOGON					0x0064	// 100 拒绝用户登录
//服务器拒绝登录
//max size : 582
SRV_DENY_LOGON::SRV_DENY_LOGON(unsigned short ver)
{
	this->version = ver;

	seq = 0;						//2

	type_loginby = LOGIN_BYUID;			//2 0  UID; 1 tstring
	type_loginsort = LOGIN_NORMAL;			//2 登录类别  0  normal;  1  offline

	uin = 0;					//4 用户UIN
	memset(strid, 0, MAX_SID_LEN);//60 字符类型的的登录
	deny = 0;					//2 拒绝登录的原因：0: 用户已经在线；1：UIN号码不存在； 2：密码错误  3: 服务器到最大  4:隐身登陆失败  5:其它类型 10 版本太旧
	memset(reason, 0, MAX_RESPONSE_LEN);
	memset(strAddress, 0, MAX_URL_LEN);

	this->m_Head.head.cmd = CMD_SRV_DENY_LOGON;
}

SRV_DENY_LOGON::~SRV_DENY_LOGON()
{
}


bool SRV_DENY_LOGON::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;
	
	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> type_loginby))
		goto RETURN;
	if(!(m_Pack >> type_loginsort))
		goto RETURN;

	if(this->version >= 2558)
	{
		if(!(m_Pack >> uin))
			goto RETURN;

		if(!m_Pack.unpackString(strid,len,MAX_SID_LEN))
			goto RETURN;
	}

	if(!(m_Pack >> deny))
		goto RETURN;
	if(!m_Pack.unpackString(reason,len,MAX_RESPONSE_LEN))
		goto RETURN;

	if(!m_Pack.unpackString(strAddress,len,MAX_URL_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_DENY_LOGON::pack()  //write
{
	//write
	bool bRtn = false;
	
	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << type_loginby))
		goto RETURN;
	if(!(m_Pack << type_loginsort))
		goto RETURN;

	if(this->version >= 2558)
	{
		if(!(m_Pack << uin))
			goto RETURN;
		if(!m_Pack.packString(strid,MAX_SID_LEN))
			goto RETURN;
	}

	if(!(m_Pack << deny))
		goto RETURN;
	if(!m_Pack.packString(reason,MAX_RESPONSE_LEN))
		goto RETURN;

	if(!m_Pack.packString(strAddress,MAX_URL_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_STATUS_USER					0x01A4	// 130 用户在线状态
SRV_STATUS_USER::SRV_STATUS_USER(unsigned short ver)
{
	this->version = ver;

	this->ver = 0;
	langtype = LANGUAGE_NORMAL;

	uin = 0;
	memset(nickname, 0, MAX_USERNAME_LEN);

	ip = 0;
	port = 0;
	status = 0;

	SetVideo(0);
	SetStatus(STATUS_OFFLINE);

	externid = 0;		//外联公司编号
	bak = 0;		//

	this->m_Head.head.cmd = CMD_SRV_STATUS_USER;
}

SRV_STATUS_USER::~SRV_STATUS_USER()
{
}


bool SRV_STATUS_USER::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> uin))
		goto RETURN;

	if(!(m_Pack.unpackString(nickname,len,MAX_USERNAME_LEN)))
		goto RETURN;

	if(!(m_Pack >> ip))
		goto RETURN;
	if(!(m_Pack >> port))
		goto RETURN;

	if(!(m_Pack >> ver))
		goto RETURN;
	if(!(m_Pack >> langtype))
		goto RETURN;

	if(!(m_Pack >> status))
		goto RETURN;
	if(!(m_Pack.unpackString(strstatus,len,MAX_STATUS_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_STATUS_USER::pack()  //write
{
	//write
	bool bRtn = false;

	if(!(m_Pack << uin))
		goto RETURN;

	if(!m_Pack.packString(nickname,MAX_USERNAME_LEN))
		goto RETURN;

	if(!(m_Pack << ip))
		goto RETURN;
	if(!(m_Pack << port))
		goto RETURN;

	if(!(m_Pack << ver))
		goto RETURN;
	if(!(m_Pack << langtype))
		goto RETURN;

	if(!(m_Pack << status))
		goto RETURN;
	if(!m_Pack.packString(strstatus,MAX_STATUS_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

int SRV_STATUS_USER::GetStatus()
{
	return GetMutiByte(status, ONLINE_STATUS, ONLINE_STATUS_LEN);
}

void SRV_STATUS_USER::SetStatus(int flag)
{
	status = SetMutiByte(status, ONLINE_STATUS, ONLINE_STATUS_LEN, (unsigned short )flag);
}

int SRV_STATUS_USER::GetConnectType()
{
	return GetMutiByte(status, ONLINE_CONNECTTYPE, ONLINE_CONNECTTYPE_LEN);
}

void SRV_STATUS_USER::SetConnectType(int flag)
{
	status = SetMutiByte(status, ONLINE_CONNECTTYPE, ONLINE_CONNECTTYPE_LEN, (unsigned short)flag);
}

int SRV_STATUS_USER::GetClientType()
{
	return GetMutiByte(status, ONLINE_USERTYPE, ONLINE_USERTYPE_LEN);
}

void SRV_STATUS_USER::SetClientType(int flag)
{
	status = (unsigned short)SetMutiByte(status, ONLINE_USERTYPE, ONLINE_USERTYPE_LEN, (unsigned short)flag);
}

int SRV_STATUS_USER::GetVideoType()
{
	return GetMutiByte(status, ONLINE_VIDEO, ONLINE_VIDEO_LEN);
}

void SRV_STATUS_USER::SetVideo(int type)
{
	status = SetMutiByte(status, ONLINE_VIDEO, ONLINE_VIDEO_LEN, (unsigned short)type);
}

//#define CMD_SRV_SEARCH_SUCC					0x008C	// 140 查询朋友结果
SRV_SEARCH_SUCC::SRV_SEARCH_SUCC(unsigned short ver)
{
	this->version = ver;
	maxnum = MAX_LIST_QUERYFRD;
	
	seq = 0;
	onlineusernum = 0;
	onlineclientnum = 0;
	
	isnext = 0;
	num = 0;
	nextpos = 0;

	this->m_Head.head.cmd = CMD_SRV_SEARCH_SUCC;
}

SRV_SEARCH_SUCC::~SRV_SEARCH_SUCC()
{
	;
}

bool SRV_SEARCH_SUCC::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;
	int i = 0;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> onlineusernum))
		goto RETURN;
	if(!(m_Pack >> onlineclientnum))
		goto RETURN;
	if(!(m_Pack >> isnext))
		goto RETURN;

	if(!(m_Pack >> num))
		goto RETURN;
	if(num > maxnum)
		num = maxnum;

	for(i = 0; i < num; i++)
	{
		if(!(ListInfo[i].unpack(m_Pack, version)))
			goto RETURN;
	}

	if (num>1) //add by djz
	{
		if(!(m_Pack >> nextpos))
			goto RETURN;
	}
	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_SEARCH_SUCC::pack()
{
	//write
	bool bRtn = false;
	int i = 0;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << onlineusernum))
		goto RETURN;
	if(!(m_Pack << onlineclientnum))
		goto RETURN;
	if(!(m_Pack << isnext))
		goto RETURN;

	if(num > maxnum)
		num = maxnum;

	for(i = 0; i < num; i++)
	{
		if(!(ListInfo[i].pack(m_Pack, version)))
			goto RETURN;
	}

	if (num>1)
	{
		if(!(m_Pack << nextpos))
			goto RETURN;
	}
	bRtn = true;
RETURN:
	return bRtn;
}


bool SRV_SEARCH_SUCC::AddInfo(FRD_QUERY_INFO Info)
{
	if(num >= maxnum)
		return false;
	else
	{
		ListInfo[num].CopyInfo(&Info);

		num ++;
		return true;
	}
}

void SRV_SEARCH_SUCC::ClearInfo()
{
	int i, nMaxNum;

	nMaxNum = __min(maxnum, num);
	for(i = 0; i< nMaxNum; i++)
	{
		ListInfo[i].ClearInfo();
	}

	num = 0;
}

//更新用户信息, 更新密码, 更新状态的服务器结果：成功，失败
//更新成功
SRV_UPDATE_SUCC::SRV_UPDATE_SUCC(unsigned short ver)
{
	this->version = ver;

	this->seq = 0;
	this->type = UPDATE_STATUS;

	this->commstatus = 0;
	this->extstatus = 0;
	this->onlinestatus = 0;
	memset(strstatus, 0, MAX_STATUS_LEN);

	uin = 0;

	this->m_Head.head.cmd = CMD_SRV_UPDATE_SUCC;
}

SRV_UPDATE_SUCC::~SRV_UPDATE_SUCC()
{
}

bool SRV_UPDATE_SUCC::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> type))
		goto RETURN;

	if(!(m_Pack >> commstatus))
		goto RETURN;
	if(!(m_Pack >> extstatus))
		goto RETURN;
	if(!(m_Pack >> onlinestatus))
		goto RETURN;

	if(!m_Pack.unpackString(strstatus,len,MAX_STATUS_LEN))
		bRtn = false;  //在线状态的文字描述	

	if(!(m_Pack >> uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_UPDATE_SUCC::pack()  //write
{
	//<<write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << type))
		goto RETURN;

	if(!(m_Pack << commstatus))
		goto RETURN;
	if(!(m_Pack << extstatus))
		goto RETURN;
	if(!(m_Pack << onlinestatus))
		goto RETURN;

	if(!m_Pack.packString(strstatus,MAX_STATUS_LEN))
		bRtn = false;  //在线状态的文字描述	

	if(!(m_Pack << uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//服务器返回的用户资料
SRV_REP_USERINFO::SRV_REP_USERINFO(unsigned short ver)
{
	version = ver;

	seq = 0;			//2 返回查询号
	nUpdateType = UPDATE_USERINFO;	//2
	uin = 0;			//4 更新的用户号

	ip = 0;				//4
	port = 0;			//2
	
	memset(strStatus, 0, MAX_STATUS_LEN);

	cltVer = VERSION;					//2
	terminaltype = 0;						//2

	this->m_Head.head.cmd = CMD_SRV_REP_USERINFO;
	memset(strFaceLink, 0, MAX_FACELINK_LEN);

	memset(strid, 0, sizeof(strid));
}

SRV_REP_USERINFO::~SRV_REP_USERINFO()
{
}


bool SRV_REP_USERINFO::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> nUpdateType))
		goto RETURN; 
	if(!(m_Pack >> uin))
		goto RETURN;

	if(!(m_Pack >> ip))
		goto RETURN;
	if(!(m_Pack >> port))
		goto RETURN;

	switch(nUpdateType)
	{
	case UPDATE_USERINFO:
		if(!(UserInfo.unpack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_TRADEINFO:
		if(!(TradeInfo.unpack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_ALLINFO:
		if(!(UserInfo.unpack(m_Pack, version)))
			goto RETURN;
//		ul_writelog(UL_LOG_TRACE,_T("point size:%d"),m_Pack.GetCurrentPackBuff()-m_Pack.GetPackBuff());
		if(!(TradeInfo.unpack(m_Pack, version)))
			goto RETURN;
//		ul_writelog(UL_LOG_TRACE,_T("point size:%d,%d,%d"),m_Pack.GetCurrentPackBuff()-m_Pack.GetPackBuff(),TradeInfo.info.langtype,TradeInfo.info.companylangtype);
		break;
	case UPDATE_COMPANYNEWS:
		if(!m_Pack.unpackString(TradeInfo.info.memo, len, MAX_TRADEMEMO_LEN))
			goto RETURN;
		break;
	default:
		return bRtn;
	}

	if(!m_Pack.unpackString(strStatus,len, MAX_STATUS_LEN))
		bRtn = false;

	if(!(m_Pack >> cltVer))
		goto RETURN;

	if(!(m_Pack >> terminaltype))
		goto RETURN;
	
	if(!m_Pack.unpackString(strFaceLink,len, MAX_FACELINK_LEN))
		bRtn = false;

	if(!(m_Pack >> onlinetime))
		bRtn = false;	

	if(version >= 3064)// 只进行解包,打包不对其操作
	{
		if(!m_Pack.unpackString(strid,len, MAX_SID_LEN))
			bRtn = false;
	}
	

	bRtn = true;
RETURN:

	return bRtn;
}

bool SRV_REP_USERINFO::pack()  //write
{
	//<<write
	bool bRtn = false;
	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << nUpdateType) )
		goto RETURN;
	if(!(m_Pack << uin))
		goto RETURN;

	if(!(m_Pack << ip))
		goto RETURN;
	if(!(m_Pack << port))
		goto RETURN;

	switch(nUpdateType)
	{
	case UPDATE_USERINFO:
		if(!(UserInfo.pack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_TRADEINFO:
		if(!(TradeInfo.pack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_ALLINFO:
		if(!UserInfo.pack(m_Pack, version))
			goto RETURN;
		if(!TradeInfo.pack(m_Pack, version))
			goto RETURN;
		break;
	case UPDATE_COMPANYNEWS:
		if(!m_Pack.packString(TradeInfo.info.memo,MAX_TRADEMEMO_LEN))
			goto RETURN;
		break;
	default:
		return bRtn;
	}

	if(!m_Pack.packString(strStatus,MAX_STATUS_LEN))
		goto RETURN;

	if(!(m_Pack << version))
		goto RETURN;
	if(!(m_Pack << terminaltype))
		goto RETURN;

	if(!m_Pack.packString(strFaceLink,MAX_FACELINK_LEN))
		goto RETURN;

	bRtn = true;
RETURN:

	return bRtn;
}

// 290 返回短信收到的信息
SRV_REP_SMS::SRV_REP_SMS(unsigned short version)
{
	this->version = version;
	this->sendtime = 0;
	this->recvtime = 0;
	this->seq = 0;
	memset(sms, 0, MAX_SMS_LEN);
	memset(mobile, 0, MAX_PHONE_LEN);

	this->m_Head.head.cmd = CMD_SRV_REP_SMS;
}

SRV_REP_SMS::~SRV_REP_SMS()
{
}

bool SRV_REP_SMS::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack.unpackString(mobile, len, MAX_PHONE_LEN)))
		goto RETURN;

	if(!(m_Pack.unpackString(sms, len, MAX_SMS_LEN)))
		goto RETURN;

	if(!(m_Pack >> sendtime))
		goto RETURN;
	if(!(m_Pack >> recvtime))
		goto RETURN;
	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_REP_SMS::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack.packString(mobile, MAX_PHONE_LEN)))
		goto RETURN;

	if(!(m_Pack.packString(sms, MAX_SMS_LEN)))
		goto RETURN;

	if(!(m_Pack << sendtime))
		goto RETURN;
	if(!(m_Pack << recvtime))
		goto RETURN;
	bRtn = true;
RETURN:
	return bRtn;
}


//#define CMD_SRV_REP_TRANSFERCLIENT			0x01B8  // 440 转移用户成功失败
SRV_REP_TRANSFERCLIENT::SRV_REP_TRANSFERCLIENT(unsigned short ver)
{
	this->version = ver;

	this->seq = 0;
	memset(reason, 0, MAX_RESPONSE_LEN);
	this->deny = 0;

	this->m_Head.head.cmd = CMD_SRV_REP_TRANSFERCLIENT;

	memset(szChatId,0,sizeof(szChatId));
	memset(szRand,0,sizeof(szRand));
	memset(szThirdid,0,sizeof(szThirdid));
}

SRV_REP_TRANSFERCLIENT::~SRV_REP_TRANSFERCLIENT()
{
}


bool SRV_REP_TRANSFERCLIENT::unpack()  //read
{
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
	
	if(!(m_Pack >> result))
		goto RETURN;
	
	if(!(m_Pack >> deny))
		goto RETURN;
	if(!(m_Pack.unpackString(reason,len,MAX_RESPONSE_LEN)))
			goto RETURN;

	if (version>=3073)
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

bool SRV_REP_TRANSFERCLIENT::pack()  //write
{
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
	
	if(!(m_Pack << result))
		goto RETURN;
	
	if(!(m_Pack << deny))
		goto RETURN;
	if(!(m_Pack.packString(reason,MAX_RESPONSE_LEN)))
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

//#define CMD_SRV_TRANSFERCLIENT				0x01C2  // 450 通知接收者，转移临时用户
SRV_TRANSFERCLIENT::SRV_TRANSFERCLIENT(unsigned short version)
{
	this->version = version;

	this->seq = 0;

	memset(szChatId,0,sizeof(szChatId));
	memset(szRand,0,sizeof(szRand));
	memset(szThirdid,0,sizeof(szThirdid));


	this->m_Head.head.cmd = CMD_SRV_TRANSFERCLIENT;
}

SRV_TRANSFERCLIENT::~SRV_TRANSFERCLIENT()
{
}


bool SRV_TRANSFERCLIENT::unpack()
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
	

	if (version>=3073)
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

bool SRV_TRANSFERCLIENT::pack()
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


//////////////////////////////////////////////////////////////////////////
SRV_LIST_FRIENDGROUP::SRV_LIST_FRIENDGROUP(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;

	all = 0;
	index = 0;

	num = 0;                                        //包里面的用户数目
	maxnum = MAX_LIST_GROUP;
	cur = 0;

	this->m_Head.head.cmd = CMD_SRV_GROUP_LIST;
}
bool SRV_LIST_FRIENDGROUP::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;
	FRD_COMMON_INFO Info;
	int i;

	if(version > 2601)
	{
		if(!(m_Pack >> seq))
			goto RETURN;

		if(!(m_Pack >> all))
			goto RETURN;
		if(!(m_Pack >> index))
			goto RETURN;
	}
	else
	{
		if(!(m_Pack >> all))
			goto RETURN;
		if(!(m_Pack >> index))
			goto RETURN;
	}

	if(!(m_Pack >> num))
		goto RETURN;

	if(num > maxnum)
		num = maxnum;

	unsigned short namelen;
	for(i = 0; i < num&&i<MAX_LIST_FRDGROUP; i++)
	{
		if(!(m_Pack>>ListInfo[i].id))
			goto RETURN;
		if(!m_Pack.unpackString(ListInfo[i].groupname,namelen,MAX_GROUPNAME_LEN))
			goto RETURN;
		ListInfo->groupname[namelen]='\0';
	}
	maxnum=i;
	bRtn = true;
RETURN:
	return bRtn;
}


//////////////////////////////////////////////////////////////////////////
//服务器返回离线用户列表
SRV_LIST_FRD::SRV_LIST_FRD(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;

	all = 0;
	index = 0;

	num = 0;                                        //包里面的用户数目
	maxnum = MAX_LIST_FRD;
	cur = 0;

	this->m_Head.head.cmd = CMD_SRV_LIST_FRD;
}

SRV_LIST_FRD::~SRV_LIST_FRD()
{
}


bool SRV_LIST_FRD::unpack()  //read
{
	//>>read
	bool bRtn = false;
	unsigned short len = 0;
	FRD_COMMON_INFO Info;
	int i;

	if(version > 2601)
	{
		if(!(m_Pack >> seq))
			goto RETURN;

		if(!(m_Pack >> all))
			goto RETURN;
		if(!(m_Pack >> index))
			goto RETURN;
	}
	else
	{
		if(!(m_Pack >> all))
			goto RETURN;
		if(!(m_Pack >> index))
			goto RETURN;
	}

	if(!(m_Pack >> num))
		goto RETURN;

	if(num > maxnum)
		num = maxnum;
	for(i = 0; i < num; i++)
	{
		if(!(Info.unpack(m_Pack, version)))
			goto RETURN;

		this->AddInfo(Info);
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_LIST_FRD::pack()  //write
{
	//<<write
	bool bRtn = false;
	int i;

	if(version > 2601)
	{
		if(!(m_Pack << seq))
			goto RETURN;

		if(!(m_Pack << all))
			goto RETURN;
		if(!(m_Pack << index))
			goto RETURN;
	}
	else
	{
		if(!(m_Pack << all))
			goto RETURN;
		if(!(m_Pack << index))
			goto RETURN;
	}

	if(num > maxnum)
		num = maxnum;
	if(!(m_Pack << num))
		goto RETURN;

	for(i = 0; i < num; i++)
	{
		if(!(ListInfo[i].pack(m_Pack, version)))
			goto RETURN;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_LIST_FRD::AddInfo(FRD_COMMON_INFO Info)
{
	if(cur >= num)//num >= maxnum)
		return false;
	else
	{
//		ListInfo[num].CopyInfo(&Info);
		ListInfo[cur].CopyInfo(&Info);

		cur ++;
//		num ++;
		return true;
	}
}

void SRV_LIST_FRD::ClearInfo()
{
	int i, nMaxNum;

	nMaxNum = __min(maxnum, num);
	for(i = 0; i< nMaxNum; i++)
	{
		ListInfo[i].ClearInfo();
	}

	num = 0;
}

//服务器返回在线用户列表
//CMD_SRV_LIST_ONLINEFRD
SRV_LIST_ONLINEFRD::SRV_LIST_ONLINEFRD(unsigned short ver)
{
	this->version = ver;

	seq = 0;
	all = 0;
	index = 0;
	num = 0;

	this->ClearInfo();
	maxnum = MAX_LIST_ONLINEFRD;

	cur = 0;

	this->m_Head.head.cmd = CMD_SRV_LIST_ONLINEFRD;
}

SRV_LIST_ONLINEFRD::~SRV_LIST_ONLINEFRD()
{
}


bool SRV_LIST_ONLINEFRD::unpack()
{
	//read
	bool bRtn = false;

	int i;
	FRD_ONLINE_INFO Info;

	if(!(m_Pack>>all))
		goto RETURN;
	if(!(m_Pack>>index))
		goto RETURN;

	if(!(m_Pack>>num))
		goto RETURN;

	if(num > maxnum)
		num = maxnum;

	for(i=0; i<num; i++)
	{
		Info.unpack(m_Pack, version);

		this->AddInfo(Info);
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_LIST_ONLINEFRD::pack()  //write
{
	//write
	bool bRtn = false;
	int i;

	if(!(m_Pack << index))
		goto RETURN;
	if(!(m_Pack << all))
		goto RETURN;
	
	if(!(m_Pack << num))
			goto RETURN;

	if(num > maxnum)
		num = maxnum;

	for(i=0; i<num; i++)
	{
		if(!(this->ListInfo[i].pack(m_Pack, version)))
			goto RETURN;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_LIST_ONLINEFRD::AddInfo(FRD_ONLINE_INFO Info)
{
	if(cur >= num)//num >= maxnum)
		return false;
	else
	{
//		ListInfo[num].CopyInfo(&Info);
		ListInfo[cur].CopyInfo(&Info);

		cur ++;
//		num ++;
		return true;
	}
}

void SRV_LIST_ONLINEFRD::ClearInfo()
{
	int i, nMaxNum;

	nMaxNum = __min(maxnum, num);
	for(i = 0; i< nMaxNum; i++)
	{
		ListInfo[i].ClearInfo();
	}

	num = 0;
}



///add at 7.6 phenix 
//离线消息
SRV_OFFLINE_MSG::SRV_OFFLINE_MSG(unsigned short ver)
{
	version = ver;

	MsgNum = 0;

}

SRV_OFFLINE_MSG::~SRV_OFFLINE_MSG()
{}

bool SRV_OFFLINE_MSG::pack()
{
	bool bRtn = false;

	if(MsgNum >MAX_OFFLINE_MSG)
		MsgNum = MAX_OFFLINE_MSG;
	
	m_Pack << MsgNum;
	for(int i= 0; i<MsgNum;i++)
	{
		if(!(MsgList[i].pack(m_Pack, version)))
			goto RETURN;

	}
	bRtn = true;
RETURN:
	return bRtn;

}

bool SRV_OFFLINE_MSG::unpack()
{
	bool bRtn = false;
	m_Pack>>MsgNum;
	if(MsgNum > MAX_OFFLINE_MSG)
		MsgNum = MAX_OFFLINE_MSG;

	for(int i=0;i<MsgNum;i++)
	{
		if(!(MsgList[i].unpack(m_Pack, version)))
			goto RETURN;
	}
	bRtn = true;
RETURN:
	return bRtn;
}
