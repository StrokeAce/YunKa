#include "../stdafx.h"
#include <time.h>
#include "../log/winlog.h"
#include "chat_history_msg.h"
#include "../json/jsonext.h"

CNomalChatHistoryMsgManage g_NomalChatHistoryMsgManage;

void CNChatHisMsg::SetTotalMsgNum(int nt)
{
	if (m_nNeedTotalNum>=0&&m_nNeedTotalNum!=nt)
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "ntototalnum error,chatid:%s,old:%d,new:%d", m_schatid.c_str(), m_nNeedTotalNum, nt);
	}

	if (nt>m_nNeedTotalNum)
	{
		m_nNeedTotalNum = nt;
	}
}
void CNChatHisMsg::AddMsg(int id, NHisMsgItem *phm)
{
	map<int, NHisMsgItem*>::iterator it=m_msgs.find(id);
	if (it==m_msgs.end())
	{
		m_msgs[id] = phm;
	}
	else
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "repeat hismsg,chatid:%s,old:%d,new:%d", m_schatid.c_str(), m_nNeedTotalNum, m_nNeedTotalNum);
		delete it->second;
		it->second = phm;
	}

}

void  CNChatHisMsg::Clear()
{
	m_schatid.clear();
	m_nNeedTotalNum = -1;
	m_recvbegintime = 0;
	for (map<int, NHisMsgItem*>::iterator it = m_msgs.begin(); it != m_msgs.end();++it)
	{
		delete it->second;
	}
	m_msgs.clear();
}


void CNChatHisMsg::Move(CNChatHisMsg &l,CNChatHisMsg &r)
{
	l.Clear();
	l.m_schatid = r.m_schatid;
	l.m_nNeedTotalNum = r.m_nNeedTotalNum;
	l.m_recvbegintime = r.m_recvbegintime;
	l.m_msgs = r.m_msgs;
	r.Clear();

}
//////////////////////////////////////////////////////////////////////////

bool CNomalChatHistoryMsgManage::ParseJsonHisMsg(const char* chatid, const char* pjsondata)
{

	Json::Value jv;
	if (!ParseJson(pjsondata, jv))
	{
		g_WriteLog.WriteLog(C_LOG_ERROR, "RecvComSendNormalChatidHisMsg parse json failed,chatid:%s,msg:%s", chatid, pjsondata);
		return false;
	}

	AutoLock  _l(m_lock);


	map<int, NHisMsgItem*> mapmsgs;

	int ntotalmsgnum = GetUintFromJson(jv, "msgnum");

	m_chatmsgs[chatid].m_schatid = chatid;
	m_chatmsgs[chatid].SetTotalMsgNum(ntotalmsgnum);

	Json::Value msgs = jv["msgs"];
	if (msgs != Json::Value::null)
	{
		for (unsigned int i = 0; i < msgs.size(); ++i)
		{
			Json::Value &m = msgs[i];
			NHisMsgItem*  phm = new NHisMsgItem;

			int curid = GetUintFromJson(m, "id");
			phm->fromuin = GetUintFromJson(m, "u");
			phm->fromname = GetStrFromJson(m, "n");//msgcontent
			phm->sMsg = GetStrFromJson(m, "m");//msgcontent
			phm->tmsgtime = GetUintFromJson(m, "t"); //msg send time
			phm->sFont = GetStrFromJson(m, "f");    //msg fontinfo
			m_chatmsgs[chatid].AddMsg(curid, phm);
		}
	}


	return true;
}

/*
return:  NHisMsgType
msgs中数据用完需要delete
*/
CNomalChatHistoryMsgManage::NHISMSGTYPE CNomalChatHistoryMsgManage::GetCompleteMsgs(const string& schatid,CNChatHisMsg &nchatmsgs)
{
	time_t tnow = time(NULL);
	NHISMSGTYPE ret = NOTFIND;
	AutoLock  _l(m_lock);
	map< string, CNChatHisMsg >::iterator it=m_chatmsgs.find(schatid);
	if (it!=m_chatmsgs.end())
	{
		if (it->second.Completed())
		{
			CNChatHisMsg::Move(nchatmsgs,it->second);
			m_chatmsgs.erase(schatid);
			ret = SUCCESS;
		}else if (tnow-it->second.m_recvbegintime>6)
		{
			ret = TIMEOUT;
		}
	}

	for (map< string, CNChatHisMsg >::iterator it = m_chatmsgs.begin(); it != m_chatmsgs.end();)
	{
		if (tnow-it->second.m_recvbegintime>30)
		{
			m_chatmsgs.erase(it++);
		}
		else
		{
			++it;
		}
	}


	return ret;
}