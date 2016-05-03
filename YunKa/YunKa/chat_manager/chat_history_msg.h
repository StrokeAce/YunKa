#include <string>
#include "Lock.h"
#include <map>
using namespace std;


struct NHisMsgItem
{
	unsigned int fromuin;
	string   fromname;
	time_t   tmsgtime;
	string   sFont;
	string   sMsg;
};

class CNChatHisMsg
{
public:
	CNChatHisMsg() :m_nNeedTotalNum(-1){}
	CNChatHisMsg(const string& cid, int nt) 
		:m_schatid(cid)
		,m_nNeedTotalNum(nt){}

	~CNChatHisMsg(){ Clear(); }

	void SetTotalMsgNum(int nt);
	void AddMsg(int id, NHisMsgItem *phm);
	bool Completed(){ return m_nNeedTotalNum <= (int)m_msgs.size(); }
	void Clear();
	static void Move(CNChatHisMsg&l,CNChatHisMsg &r);

	string				   m_schatid;
	int                    m_nNeedTotalNum; //历史记录总共多少条，收全就可以显示了
	map<int, NHisMsgItem*> m_msgs;            //id->msg 消息列表
	time_t                 m_recvbegintime; //接收开始时间
private:
	CNChatHisMsg(const CNChatHisMsg&); 
	CNChatHisMsg& operator= (const CNChatHisMsg &r);
};


class CNomalChatHistoryMsgManage
{

public:
	enum NHISMSGTYPE
	{
		SUCCESS,
		TIMEOUT,
		NOTFIND
	};

	bool ParseJsonHisMsg(const char* chatid, const char* pjsondata);

	/*
	return:  NHisMsgType
	msgs中数据用完需要delete
	*/
	NHISMSGTYPE GetCompleteMsgs(const string& schatid,CNChatHisMsg &nchatmsgs);
private:
	CLock  m_lock;
	map < string, CNChatHisMsg >  m_chatmsgs;  //chatid ->chatmsgs
};

extern CNomalChatHistoryMsgManage g_NomalChatHistoryMsgManage;