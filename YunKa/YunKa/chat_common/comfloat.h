/**
Ϊ�˺͵�ǰ�ÿͺͿͷ���һ��һ�Ự�����¼�� S���������������µĻỰ�ݽж��˻Ự�����¼�� M����
1.S�Ự������MP�����߼�����M�Ự��GP�����߼�����
2.S�Ự�����û��Ŷ�ɢ�е���ͬ��MP��M�Ự���޿ؼ���������webui���ټ��Ự����ʱ���ѡ��һGp�������߼�������������ͨ�Ű������ڰ�ͷ��PACK_HEADER����nSessionId������ƫ��8������2�ֽڣ��б���gp��id��Ϣ��UI���ݴ���Ϣ����ɢ�С�
  gpid����Ϊ0xFFFFʱ��UI���ѡ��һgp����ת����ȥ��gpid����Ϊ0xFFFEʱ��UI��������㲥�����е�GP��
3.Gp�յ�ͨ�Ű��󣬻��ڰ�ͷ��nSequence������ƫ��12������2�ֽڣ������ñ��λỰ�а���Ψһ��ţ����������nSequenceΪ0����ʾ���������GP�����棩�����㲥�����Ự�е����ߡ��Ҵ��ڽ���״̬�������û�����������������
4.�Ự��֧����������
5.�ɹ���Ա�Ż�chatidΨһȷ��һ���Ի�
*/

#if !defined _COMMFLOAT_EM
#define _COMMFLOAT_EM

#include "comcom.h"
#include "comdef.h"
#include "comstruct.h"
#include "wx_obj.h"
#include <map>
#include <vector>

using namespace std;

class COM_FLOAT_CHATPACK:public CPackInfo
{
public:
	COM_FLOAT_CHATPACK(){}
	COM_FLOAT_CHATPACK(unsigned short cmd,unsigned short ver,unsigned short Gpid)
	{
		sGpid=Gpid;
		m_Head.head.cmd=cmd;
		m_Head.head.ver=VERSION;
	}

	bool packhead()
	{
		m_Head.head.random=sGpid;
		return m_Head.pack(m_Pack);
	}
protected:
	unsigned short sGpid;   //Gp��������id

};

/**
CMD_FLOAT_CREATECHAT=0x0230 //�����㲥�Ự����
���GP�����Ự�ɹ������˰��㲥�������û������������ߣ���ʧ�ܷ��ط�����CMD_FLOAT_CMDERROR��
�˰�����webui����������
*/
class COM_FLOAT_CREATECHAT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CREATECHAT(unsigned short ver,unsigned short sGpid);

	bool pack();  //write
	bool unpack();  //write

public:

	unsigned int uAdminId;	//����Ա����
	unsigned short usort;	//Ư����id
	unsigned int uWebuin;	//�ÿͺ���
	char chatid[MAX_CHATID_LEN+1];     //����id��Ψһ����һ�λỰ
	char clienttid[MAX_WEBCLIENID_LEN+1];
	char webname[MAX_USERNAME_LEN+1];
	char thirdid[MAX_THIRDID_LEN+1]; //������id
	unsigned int uKefu;			//��ǰ�ͷ�����
	unsigned int uFromAdmin; //�Ự��Դ��վ����Ա
	unsigned short uFromSort;//�Ự����Ư����id

	//��Ӧ�㲥��Ա�б�
	map<unsigned int,char> memlist;

};

/**
��GP��ȡƯ�������ߵĻỰ�б�,sGpidӦ����Ϊ0xFFFE
CMD_FLOAT_LISTCHAT=0x0231  //���߻Ự�б�
gp���ݻ�ȡuAdminId�����з������Ѻö���ĻỰ�б���ʽΪCOM_FLOAT_RECVLIST��ʧ�ܷ���CMD_FLOAT_CMDERROR

�˰����пͷ�����
*/
class COM_FLOAT_GETLIST:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_GETLIST(unsigned short ver,
		unsigned short sGpid=0xFFFE   //Ĭ�϶����㲥�����GP
		);

	bool pack();  //write

public:
	unsigned int uAdminId;	//����Ա����

};

/*
��GP���ܵ���Ư�������ߵĻỰ�б���Ϣ��ÿ��GP���᷵������һ�������
CMD_FLOAT_LISTCHAT=0x0231  ���߻Ự�б�
*/
class COM_FLOAT_RECVLIST:public COM_FLOAT_CHATPACK
{
public:
	bool unpack();  //read

public:
	unsigned int uAdminId;	//����Ա����

	vector<LISTCHATINFO>  recvchatlist;
};


//
/*
�Ự����ϸ��Ϣ��������ʱ���uAdminId��chatid ,GP�ظ�Ϊ�Ự����ϸ��Ϣ
CMD_FLOAT_CHATINFO
*/
class COM_FLOAT_CHATINFO:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CHATINFO(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();
	
public:
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	
	//����ֻ���ڽ��
	unsigned short sSort;//Ư����id
	unsigned int   tStartTime;		  //�Ự��ʼʱ��
	
	unsigned int uFromAmdind; //�Ự��Դ����Ա����
	unsigned short sFromSort; //�Ự��ԴƯ����id
	
	unsigned int uWebUin;//web�����ĺ���
	unsigned int uKefuUin;//�ͷ��ĺ���,0:�ȴ�Ӧ��״̬��>0:���ںͿͷ�ͨ��
	
	char		webnickname[MAX_USERNAME_LEN+1];//�ÿ�����
	char		strClientId[MAX_WEBCLIENID_LEN+1];//�ÿ�Ψһ��ʾ
	char		strThirdId[MAX_THIRDID_LEN+1];		//������id

	
	unsigned int uTansferingToAdmin;	//Ҫת�Ƶ���Ŀ�����Ա
	unsigned int uTansferingToKefu;		//Ҫת�Ƶ���Ŀ��ͷ�
	
	vector<unsigned int> webRecvUin;	  // webӦ��ͷ��б�
	vector<unsigned int> InviteUin;		  // �������Ŀͷ��б�
};

/*
CMD_FLOAT_CHATMSG=0x0232	���ͻỰ��Ϣ
����ỰuAdminId chatid�Ի��ĻỰ���ڣ��㲥�ð����Ự�����ߡ���Ȩ�鿴�Ự���ݵ��û������������ߣ���gp��COM_FLOAT_CHATMSG_ACK�������ߣ������ڷ���CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_CHATMSG:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CHATMSG(unsigned short ver,unsigned short sGpid);

	bool pack();  //write
	bool unpack();  //read

public:
	unsigned int		uAdminId;						//����Ա����
	unsigned int		webuin;							//�ÿ�uin
	unsigned short		nMsgType;						//��Ϣ����  2��ͨ��Ϣ��3��ϢԤ֪,4 �ͷ���������
	unsigned short		nMsgDataType;					//��Ϣ��������  1 ���֣�2 ͼƬ��3 ������4 ��Ƶ
	unsigned int		tMsgTime;						//��Ϣ����ʱ��
	char				chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	char				nickname[MAX_USERNAME_LEN+1];	// �ֻ�����, ������Ϣʹ�ã��û�������Ϣʱ��������
	char				strmsg[MAX_MSG_RECVLEN+1];		// ��Ϣ����
	char				strfontinfo[MAX_MSGFONT_LEN+1]; //������Ϣ HTML,JSON=WX,CHSET=%u,H=%u,EF=%u,CR=%u,FN=%s

	//�ͻ��˰汾����6000
	char				strRand[MAX_WEBCLIENID_LEN+1];  //�ÿ�Ψһ��ʾ �������汾>=3073֧��
	char				strThirdid[MAX_THIRDID_LEN+1];	//������id �������汾>=3073֧��

	WxObj* wxMsg;
};

/*
�ͻ��˷���COM_FLOAT_CHATMSG��Ϣ�����������ȷ�ϰ�
CMD_FLOAT_CHATMSG_ACK=0x0239,	//GP�Է�����CMD_FLOAT_CHATMSG����ack
*/
class COM_FLOAT_CHATMSG_ACK:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CHATMSG_ACK(unsigned short ver,unsigned short sGpid);

	bool pack();  //write
	bool unpack();  //read

public:
	unsigned int uAdminId;	//����Ա����
	char chatid[MAX_CHATID_LEN+1];     //����id��Ψһ����һ�λỰ
	unsigned int   webuin;  //�ÿ�uin
	unsigned short chatseq; //GP������Ϊ������Ϣ�����id ,chatseq>=1;

};


/*
֪ͨ�����������͹ر���Ϣ�㲥
CMD_FLOAT_STARTORSTOP_MSG=0x0233,	�ͻ���֪ͨ�������㲻�㲥��Ϣ���ͻ���
���ķ������ڻỰ�е���Ϣ����״̬��ֻӰ��COM_FLOAT_CHATMSG�������ɹ��ط��˰����ͷ��������ڷ���CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_STARTORSTOPRECV:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_STARTORSTOPRECV(unsigned short ver,unsigned short sGpid);

	bool pack();  //write

public:
	unsigned int uAdminId;				//����Ա����
	unsigned short sAcitonType;			// 0:������Ϣ��1ֹͣ������Ϣ
	unsigned short sLastMsgid;			//�����Ϣid
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ

};


//!�ÿ͹رջỰ���ڣ�֪ͨ�������رջỰ���ͷ�Ҳ�����øð����رպͷÿ͵�ͨ���Ự,gp�㲥�������ˣ����������Ự�����ڷ���CMD_FLOAT_CMDERROR
//CMD_FLOAT_CLOSECHAT=0x0236,  �Ự��ֹ��
class COM_FLOAT_CLOSECHAT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_CLOSECHAT(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned short usType;			// 0,�������ߣ�1 webui �رջỰ ,2�ͷ��رջỰ��3 GP�رջỰ,10 �ͷ��Ƴ�����
	unsigned int   webuin;  //�ÿ�uin
	char nickname[MAX_USERNAME_LEN+1];  //�����ߵ�����
};


//!��GP��ȡĳ���Ự�еĲ��ֻ�ȫ����Ϣ
//CMD_FLOAT_GETCHATMSG=0x0237,  ��ȡ�Ự��Ϣ���ɹ����ط���������ĸ�����Ϣ���ͻ��ˣ�ʧ�ܷ���CMD_FLOAT_CMDERROR
class COM_FLOAT_GETCHATMSG:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_GETCHATMSG(unsigned short ver,unsigned short sGpid);
	bool pack();  //write

public:
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned short sBeginSeq;			//��ʼ��Ϣid
	unsigned short sEndSeq;				//������Ϣid������������   ����sBeginSeq����С�ڵ���sEndSeq֮���������Ϣ��sEndSeqΪ0���ش���sBeginSeq��������Ϣ��

};


/*
�Ựת��
CMD_FLOAT_TRANSQUEST,  �¼����� 0���ͷ����ܻỰ  1���Ựת�ƣ� 
��GP�ɴ���Ựת�ƣ��㲥�˰��������ˣ����𷵻ط�����CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_TRANSREQUEST:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_TRANSREQUEST(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int uAdminId;					//����Ա����
	char chatid[MAX_CHATID_LEN+1];			//����id��Ψһ����һ�λỰ
	unsigned int   uWebuin;				//�ÿͺ���

	unsigned short nTimeOutSecond;			//����ʱ����û�˽��ܣ��Ự��Ϊ�ȴ�״̬

	unsigned int   uToAdminId;				//�������Ա���� 0��վ��ת�ƣ�>0  ��վת��
	unsigned int   uToKefu;				//Ҫ����Ŀͷ�����

	//���ܷ���Ӧ�㲥��Ա�б�
	map<unsigned int,char> memlist;		//��վת��ʱ�����tokefuΪ����Ҫ�ṩӦ��ͷ��б�
};

/*
�Ựת�Ƴ�ʱȡ��
*/
class COM_FLOAT_TRANSFAILED:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_TRANSFAILED(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned int   uWebuin;             //�ÿͺ���
	unsigned int   uKefu;           //��˭����ĻỰת��

	unsigned int   TransToAdmin;          //�Ựת�Ƹ�˭
	unsigned int   TransToKefu;           //�Ựת�Ƹ�˭
};


/*
����վ�ڲ��Ựת��
CMD_FLOAT_ACCEPTCHAT   �¼����� 0���ͷ����ܻỰ  1���Ựת�ƣ� 
��GP�ɴ���Ựת�ƣ��㲥�˰��������ˣ����𷵻ط�����CMD_FLOAT_CMDERROR
*/
class COM_FLOAT_ACCEPTCHAT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_ACCEPTCHAT(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  


public:
	unsigned int uFromAdminid,uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned int   uWebuin;             //�ÿͺ���
	char nickname[MAX_USERNAME_LEN+1];  //���ܻỰ�˵�����

};


#define  RECVCONTENT    1   //���ܻỰ����
#define  INSERTCONTEN   2   //�廰Ȩ��
#define  ADMINRIGHT     3    //����Ȩ��

//!��ȡ�Ŀ��Է�����Ϣ�Ķ����б�,�����б�ֻ��Ҫ���Ͱ����������ֵİ�ͷ
struct SHAREUSEROBJ
{
	unsigned int	uin;
	unsigned char	cRight;//0:ֻ�ܽ����������RECVCONTENT�����ܻỰ����,INSERTCONTEN�����ܻỰ���ݡ��廰��INSERTCONTEN���ܻỰ���ݡ��廰������
	unsigned int	onlineflag;
};

//CMD_FLOAT_SHARELIST   ��ȡ�ɷ�����Ϣ�����б��Ѻö����б�,sGpidӦ����Ϊ0xFFFEF��gp���з��ػỰ�б�û�з��صĻỰ�б���Ϊ0
class COM_FLOAT_SHARELIST:public CPackInfo
{
public:
public:
	COM_FLOAT_SHARELIST(unsigned short ver);
	bool pack(){return true;};
	bool unpack();  
	
public:

	//��Ӧ�㲥��Ա�б�
	vector<SHAREUSEROBJ> sharememlist;
};

/**
GP���صĴ���
*/

class COM_FLOAT_ERROR:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_ERROR(unsigned short ver,unsigned short sGpid);

	bool pack(){return true;};
	bool unpack();  
public:
	unsigned short errcmd;  //�����������
	unsigned short errornum;//�������
	unsigned int uAdminId;  //����Ա��
	unsigned int   uWebuin;             //�ÿͺ���
	char chatid[MAX_CHATID_LEN+1];//chatid
};

/*
����ỰЭ��
GP�յ��˰��󣬲����κδ���ֱ��ת����uInviteUser��uInviteUser�մ˰���Ӧ��ʾ�û��Ƿ������������
*/
class COM_FLOAT_INVITEREQUEST:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_INVITEREQUEST(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned int   uInviteUser;         //Ҫ����ĺ��ѵ�TQ����
	unsigned short sSecond;              //��Ч����
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned int   uWebuin;             //�ÿͺ���
};

/*
�������˷��ص�����ỰЭ���Ľ����
GP�յ��˰���sResultΪ0��ֱ��ת����uInviteFrom��sResultΪ1ʱ��sender����Ự�������û��б�
*/
class COM_FLOAT_INVITERESULT:public COM_FLOAT_CHATPACK
{
public:
	COM_FLOAT_INVITERESULT(unsigned short ver,unsigned short sGpid);
	bool pack();  
	bool unpack();  

public:
	unsigned short sResult;				//����Ӧ��  0���ܾ����룬1����������
	unsigned int   uInviteFrom;         //���������˵ĺ���
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned int   uWebuin;             //�ÿͺ���


};

///////////////////////////�ͷ�Ӧ��ʱ///////////////////////////////////////////////
class CFloatChatRelease:public COM_FLOAT_CHATPACK
{
public:
	CFloatChatRelease(unsigned short ver, unsigned short sGpid);
	bool pack();  
	bool unpack();  
	
public:
	
	unsigned int uAdminId;				//����Ա����
	char chatid[MAX_CHATID_LEN+1];		//����id��Ψһ����һ�λỰ
	unsigned int   webuin;             //�ÿͺ���
	
	unsigned int   uKefu;				//�ͷŻỰ�Ŀͷ�����
	char			szKefuName[MAX_USERNAME_LEN+1];			//Ӧ��ʱ�Ŀͷ�����

	unsigned short usReason;           //ԭ��  FloatChatTimeOutReason

};

#endif