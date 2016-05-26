// client common struct : ������ʹ�õ�һЩ���õĽṹ����
//
//////////////////////////////////////////////////////////////////////
#ifndef _CCOMMSTRUCT_EMS
#define _CCOMMSTRUCT_EMS

#include "sockbase.h"
#include "../stdafx.h"
#include "comdef.h"
#include "comenum.h"
#include <map>
using namespace std ;

class CWebUserObject;
class CUserObject;

typedef struct IDBUFF_INFO{
	unsigned int id;
	char *pBuff;
}IDBUFF_INFO_T, *IDBUFF_INFO_PT;

//�йس�ʼ�����õ�һЩ������
typedef struct INIT_CONF
{
	//���ڰ����˵��е�����
	char sProgrameNameMini[256];		// ����ĳ�����
	char sHelpUrl[256];					// ��ת��URL
	char sCopyID[256];					// �汾
	char sHelpAbout[4][256];			// ���ڶԻ����е�����
	char sHelpAboutURL[4][256];			// ���ڶԻ����е�����������ת
	int	 nDisplayUserNameType;			// �û�����ʾ��Ϣ
	char sVisitorServer[256];
	int nVisitorServerPort;
	char sDefaultServer[256];			// ��½��Ĭ�Ϸ�����
	int nDefaultPort;					// ��½��Ĭ�϶˿�
	char sListServer[10][256];			// ����ʮ�����������б�
	int bSavePass;						// ��������
	int bAutoLogin;						// �Զ���½
	int	visitorfilter;
	char infofilter[256];				// ���¸�ʽΪ : 0, ����, ���
	char sAuthAddr[256];
	int  nAuthPort;
	char webpage_lgout[256];			// �˳�
	char webpage_webphone[256];
	char webpage_querywebphone[256];	// ��ѯ����
	char webpage_note[256];				// ����
	char webpage_crm[256];				// �ͻ�����
	char webpage_analyze[256];			// ͳ�Ʒ���
	char webpage_mgmt[256];				// ��������
	char webpage_news[256];				// ���㴰��
	char webpage_regnewuser[256];		// ע�����û�
	int webpage_regnewuser_w;
	int webpage_regnewuser_h;	
	char webpage_tqhelp[256];			// ����ָ��	
	char webpage_UniIdioms[256];		// ͳһ����	
	char webpage_RtmMsg[256];			// ʵʱ���	
	char webpage_SvrMsg[256];			// ������Ϣ
	char webpage_TQadviser[256];		// TQ����	
	char webpage_UploadHeadImage[256];	// �ϴ�ͷ��
	char webpage_DownloadHeadImage[256];// ����ͷ��
	char webpage_FaceImage[256];		// ����
	char webpage_SysMessage[256];		// ϵͳ��Ϣ
	char webpage_complaint[256];		// Ͷ�߽���
	char webpage_SendWebFileLink[256];	
	char webpage_companyuser[256];		// ��ȡ��˾�û�
	char webpage_iconurl[256];
	char webpage_faqInfo[256];
	char webpage_faqSort[256];
	char webpage_faqall[256];
	char webpage_sendurl[256];
	char webpage_repickchaturl[256];	// �����Ự
	char webpage_evaluate[256];			// ��������
	char webpage_accesstoken[256];		// ��ȡ΢��token
	char webpage_workbillurl[256];		// ����
	char visitorpage_visitortail[256];	// �ͻ�����
	char visitorpage_visitorbill[256];	// �Ự�ǵ�
	char visitorpage_visitorinfo[256];	// �ͻ���Ϣ
	char visitorpage_notice[256];		// ͨ����Ϣ
	char visitorpage_visitororder[256];	// �¶���
	char registerAccount[256];			// ��½���ڵ� ע�� ��������[12/24/2010 SC]
	char forgetPassword[256];
	int forgetPSVisible;
	bool bZlibEnabled;					// ��visit������ͨ�ţ��ͻ����Ƿ�����ѹ��
	char query_history_records[256];	// ��ѯ��ʷ��¼
	char wechat_media_upload[256];		// ΢�ŷ�����ý���ļ��ϴ�url
	char wechat_static_map[256];		// ��Ѷ��ͼ��̬ͼurl
	char wechat_map_location[256];		// ��Ѷ��ͼ�ص��עurl
	char fileserver_media_download[256];// �ļ����������ص�΢�ŷ�������ý���ļ���url
	char fileserver_media_upload[256];	// �ļ��������ϴ�url
	char fileserver_media_task[256];	// ���ļ���������ȡ�ļ�id��url��
	char fileserver_media_fileid[256];	// �ļ��ϴ���΢�ŷ������������ļ�����������ͬ�ļ���url��
}INIT_CONF_T, *INIT_CONF_PT;

//һ���������ṹ
struct FILTER_USERDEFINE
{
	int num;//�����ַ�����Ŀ
	int include;//���������ֻ��ǲ�����������
	char text[MAX_STRINGFILTER_NUM][MAX_STRINGFILTER_LEN];//��������, ���10��,ÿһ����󳤶�50
};

//������Ϣ
struct LOGIN_INFO
{	
	unsigned int uid;	
	char sid[MAX_SID_LEN+1];			//60
	char pass[MAX_PASSWORD_LEN+1];		//20
	bool bAutoLogin;
	bool bKeepPwd;
};

//���ѷ�ʽ�ṹ
struct ALERT_INFO
{
	int type;
	int tray;
	int showwnd;
	int sound;
	char soundfilename[MAX_FILENAME_LEN];
};

//�û��ֵ���Ľṹ
typedef struct KEYWORDGROUP_INFO{
	unsigned int id;
	unsigned char type;
	unsigned int sort;
	
	unsigned int compid;
	unsigned int userid;
	
	unsigned int parentid;
	
	char name[MAX_USERNAME_LEN+1];
	char	invalid;

	unsigned int level;
	
	//������Ϣ�������������������
	char	solve;	//������������
}KEYWORDGROUP_INFO_T, *KEYWORDGROUP_INFO_PT;

//�û��ֵ�Ľṹ
typedef struct KEYWORD_INFO{
	unsigned int id;
	unsigned char type;
	unsigned int sort;
	unsigned int groupid;
	unsigned int compid;
	unsigned int userid;
	
	char name[MAX_USERNAME_LEN + 2];
	char memo[MAX_8192_LEN + 2];
	
	char	invalid;
	int 	hotkey;
	bool	alt, shift, ctrl;
	char	code;
	
	unsigned int level;

	//������Ϣ�������������������
	char	solve;	//������������
}KEYWORD_INFO_T, *KEYWORD_INFO_PT;

//�������Ϣ
//size : 42
struct PROXY_INFO
{
	unsigned int ip;						//4
	unsigned short port;					//2
	unsigned short proxyport;				//2
	char strUser[MAX_USERNAME_LEN];			//20
	char strPass[MAX_PASSWORD_LEN];			//20
};

//�й��û�����Ϣ��Ϣ
//max size : 670
struct USER_INFO
{
	//������Ϣ
	unsigned int uid;	
	unsigned char usertype;
	char sid[MAX_SID_LEN+1];						//60
	char pass[MAX_PASSWORD_LEN+1];					//20
	unsigned int	compid;
	char			nickname[MAX_USERNAME_LEN+1];	//����					20
	char			username[MAX_USERNAME_LEN+1];	//��ʵ����					20(40)
	char	 		mobile[MAX_PHONE_LEN+1];		//�ֻ�						20(230)
	char			phone[MAX_PHONE_LEN+1];			//�绰						20(252)
	char			email[MAX_URL_LEN+1];			//�����ʼ�					120(580)
	unsigned char	roleid;							//Ȩ�޽�ɫ
	unsigned int	comauth;						//����Ȩ��(��λ��ʾ)		4(658)
	unsigned int	extauth;						//��������״̬(��λ��ʾ)		4(662)
	unsigned int	regdate;						//ע������ʱ�䣬��int���ݱ�ʾ����1970�꿪ʼ������
	unsigned char	areaadminid;					//���Ϊ��˾�ͷ�����ֵ��ʾ��˾��Ӧ��������
	unsigned short	langtype;						//������Ϣ�����԰汾				2(668)
	unsigned short	bak;							//									2(670)		
	unsigned int	onlineflag;						//�����ֶ�						4(912)
};

//�й��û���������Ϣ�����ݾɰ汾
struct TRADE_INFO
{
	char 			company[MAX_COMPANY_LEN];				//��˾����   40
	char 			leader[MAX_USERNAME_LEN];				//����		 δ��(20)
	unsigned short	duty;									//ְ������   2
	unsigned short	size;									//��˾��ģ   2
	unsigned short	trade;									//������ҵ   2
	char 			bankcode[MAX_BANK_LEN];					//��������	40		   
	char 			bank_num[MAX_BANKNO_LEN];				//�����˺�	30
	char 		    tax_num[MAX_TAXNO_LEN];					//˰�����	30
	char 			mobile[MAX_PHONE_LEN];					//�绰		δ��
	char 			phone[MAX_PHONE_LEN];					//�绰		20
	char 			fax[MAX_PHONE_LEN];						//����		20
	char 			homepage[MAX_URL_LEN];					//��ҳ����  60
	char 			mailbox[MAX_EMAIL_LEN];					//			60
	char 			bak1[MAX_VERIFYMSG_LEN];				//			40
	unsigned short	langtype;								//������Ϣ�����԰汾
	unsigned short 	companylangtype;						//			4
	char 			memo[MAX_TRADEMEMO_LEN];				//��˾���  500
};

struct WEBONLINE_INFO
{
	WEBONLINE_INFO(){ bInvited = false; }
	int				fd;						// tcp����
	TALKSTATUS		talkstatus;
	bool		    bInvited;				// �����������
	unsigned int	timevisit;				// �û�����ʱ��
	unsigned int	timerequest;			// �û�����Ի�ʱ��
	unsigned int	timetalkstart;			// �û��Ի���ʼʱ��
	unsigned int	timetalkend;			// �û��Ի�����ʱ��
	unsigned int	timeleave;				// �û��뿪ҳ��ʱ�䣬Ϊ0һ���ʾ��������״̬��>0��ʾsocket�ѶϿ����������µ����ӽ׶�
	int				evalation;				// ���ζԻ�����
	unsigned int	random;					// �������ߺ�������	
	unsigned int	trafficid;				// ���������ݿ��е����µ�id
	unsigned char	hasmsg;					// ����Ϣ�����ڻ����С�
	unsigned char	isonline;				// �Ƿ�����
	unsigned int	visitortalkid;			// ������������Ϣ�ı��еı��
	unsigned char	searchkeywordsnum[4];	// ʶ������Źؼ��ʵ���Ŀ
	unsigned char	alertit;				// ���û���Ҫ������ʾ�� �ڶ೤����֮�ڣ����������˶��
};

//�ÿ͵Ļ�����Ϣ�� �����Ψһ��
struct WEBUSER_INFO
{
	WEBUSER_INFO()
	{
		memset(this,0,sizeof(WEBUSER_INFO));
	}
	unsigned int	uid;							//������û�������ˣ�Ϊ�̶���SYS_WEBUSER = 9999
	unsigned int	compid;							//��˾���
	unsigned int	siteid;
	char			sid[MAX_WEBCLIENID_LEN+1];		//���Ϊ�ÿ�Ψһ��ʾ
	char			thirdid[MAX_THIRDID_LEN+1];		//������id
	char			name[MAX_USERNAME_LEN+1];
	char			nameflag;						//��¼�����Ƿ��޸� 1 ���޸�
	unsigned char	status;							// 0 TALKSTATUS_NO, 1 STATUS_OFFLINE ���ߣ� 2 ���ߣ� 9 �Ի���
	char			sip[MAX_IPNUM_LEN+1];
	char			ipfromname[MAX_USERNAME_LEN+1];	//ip��Դ
	char			url[MAX_URL_LEN+1];				//��Դ��url
	char			title[MAX_URL_LEN+1];
	char			chatfrom[MAX_CHATID_LEN + 1];
};

struct WEBUSER_UPINFO
{
	char iecopyright[50];
	char systeminfo[50];
	char language[50];
	int visioncolor;
	char visionsize[20];
	unsigned int adminid;
	char visiturl[MAX_URL_LEN];
	char source[50];
	char scriptflag[MAX_SCRIPTFLAG_LEN];
	char sip[20];
	int port;
	char sid[20];
	char lastvisit[MAX_URL_LEN];
	char webtitle[50];
	char nickname[50];
	int ctimes, ttimes;
	char lastvtime[MAX_URL_LEN];
	char lastttime[MAX_URL_LEN];
	unsigned int lastuin;
	int isautoinvit;
};

//�ÿ͵��û�������Ϣ
struct WEBUSEREX_INFO
{
	char		  sid[MAX_SID_LEN + 1];	//���Ϊ�ÿ�Ψһ��ʾ
	char	name[MAX_USERNAME_LEN + 1];		//��webuser_infoһ����
	unsigned char	age;	//0 unknow
	unsigned char	sex;	//0 unknow
	char	area[MAX_AREA_LEN];
	char	phone[MAX_PHONE_LEN];
	char	email[MAX_EMAIL_LEN];	//email
	char	im[MAX_128_LEN];	//msn/qq
	char	address[MAX_128_LEN];
	char	zipcode[MAX_ZIPCODE_LEN];
	char	companyname[MAX_128_LEN];
	char	companyurl[MAX_128_LEN];
	char	keywords[MAX_128_LEN];
	unsigned char	iconindex1;
	unsigned char	iconindex2;
	unsigned char	iconindex3;
	unsigned char	iconindex4;
	unsigned int comauth;
	unsigned int extauth;
	unsigned int createtime;
};

typedef struct TCP_PACK_HEADER
{
	unsigned short   len;			//���Ĵ�С
}TCP_PACK_HEADER_T,*TCP_PACK_HEADER_PT;

typedef struct PACK_HEADER
{
	unsigned short		ver;			//Э��汾
	unsigned short		sendrandom;		//���ͷ������ ��floatchat�м�¼�ڼ�����Ϣ
    unsigned int		uin;			//�û��ı��
    unsigned short		random;			//�����   ,��floatchat�м�¼gpid
	unsigned short		cmd;			//��������
    unsigned short		seq;			//�������
	unsigned short		langtype;		//������������(0 ansi,1 utf8)
	unsigned short		seqresp;		//��Ӧ�İ������
	unsigned short		terminal;		//MULTI_TERMINAL_ONLINE = 4221֮���ʾ�ն�����1 
}PACK_HEADER_T,*PACK_HEADER_PT;

struct LISTCHATINFO
{
	char			chatid[MAX_CHATID_LEN+1];
	unsigned int	uKefuUin;	  //�ͷ��ĺ���,0:�ȴ�Ӧ��״̬��>0:���ںͿͷ�ͨ��
};

// ����ý���ļ�ʱ�����ն������Ϣ����
struct SEND_FILE_PARAMS
{
	unsigned long userId;	// ������Ϣ�û���id
	int userType;			// �û�����
};

// ���·���ý���ļ�ʱ�����ն������Ϣ����
struct RESEND_FILE_PARAMS
{
	unsigned long userId;		// ������Ϣ���û�id
	string filaPath;			// ���͵��ļ�·��
	int userType;				// �����ļ����û�����
	string msgId;				// ��Ϣ�����к�
	int msgDataType;			// ��Ϣ����������
};

// ���½���ý���ļ�ʱ�����ն������Ϣ����
struct RERECV_FILE_PARAMS
{
	string filaPath;			// �ļ��Ĵ洢·��
	string url;					// �����ļ���url
	int userType;				// ������Ϣ���û�����
	string msgId;				// ����Ϣ�����к�
	int msgDataType;			// ��Ϣ����������
	unsigned long userId;		// ������Ϣ���û�id
	unsigned long webUserId;	// ??�ýṹ����Ż�
	unsigned long groupUserId;
};

// ˫��ͷ��,�л��������ʱ���л��������Ϣ����
struct CHANGE_CHAT_OBJECT_PARAMS
{
	unsigned long userId;	// ������Ϣ�û���id
	int userType;			// �û�����
};

// һ����Ϣ
struct ONE_MSG_INFO
{
	string msgId;	// ��Ϣid
	string msg;	// ��Ϣ����
};

// ���·�����Ϣ������������Ϣ��ý���ļ���json����Ϣ
struct RESEND_MSG_PARAMS
{
	string msgId;			// ��Ϣid
	unsigned long userId;	// ������Ϣ�û���id
	int userType;			// �û�����
	string mediaId;			// ý���ļ���΢�ŷ�������id,��Ϣ���������ֵ�ʱ�򣬸ò�������������Ϣ����
	int msgDataType;		// ��Ϣ��������
	string fileId;			// ý���ļ����ļ���������id
	string filePath;		// �ļ��ڱ��ش洢·��
};

struct WEBUSER_URL_INFO
{
	string url;				// ���ʵ�ַ
	unsigned long dwtime;	// ����ʱ��
};

struct UPLOAD_INFO
{
	void* pThis;				// ���ýṹ������������ָ��
	string filePath;			// �ϴ��ļ�·��
	unsigned long userId;		// ��Ϣid
	MSG_DATA_TYPE msgDataType;	// ��Ϣ����������
	USER_TYPE userType;			// ��Ϣ�����û�����
};

struct DOWNLOAD_INFO
{
	void* pThis;				// ���ýṹ������������ָ��
	string filePath;			// �����ļ���·��
	string downLoadUrl;			// �����ļ���url
	CWebUserObject *pWebUser;	// ��Ϣ����Դ�û�
	string time;				// ����ʱ��
	CUserObject* pUser;			// Э������,��Ϊ��
	MSG_DATA_TYPE msgDataType;  // ��Ϣ����������
};

typedef struct {
	const string raw, enc;
} wx_face_t;

static const string wx_face_null = string();

static const wx_face_t wx_faces[] = {
	{ "/::)", wx_face_null },	//΢Ц
	{ "/::~", wx_face_null },	//Ʋ��
	{ "/::B", wx_face_null },	//ɫ����Ů
	{ "/::|", wx_face_null },	//����
	{ "/:8-)", wx_face_null },	//ī��
	{ "/::<", "/::&lt;" },		//����
	{ "/::$", wx_face_null },	//����
	{ "/::X", wx_face_null },	//���죬��
	{ "/::Z", wx_face_null },	//˯��
	{ "/::'(", wx_face_null },	//��
	{ "/::-|", wx_face_null },	//���Σ���
	{ "/::@", wx_face_null },	//��ŭ
	{ "/::P", wx_face_null },	//��Ƥ
	{ "/::D", wx_face_null },	//����Ц
	{ "/::O", wx_face_null },	//����

	{ "/::(", wx_face_null },	//�ѹ�
	{ "/::+", wx_face_null },	//��
	{ "/:--b", wx_face_null },	//�亹
	{ "/::Q", wx_face_null },	//ץ��
	{ "/::T", wx_face_null },	//��
	{ "/:,@P", wx_face_null },	//͵Ц
	{ "/:,@-D", wx_face_null },	//���
	{ "/::d", wx_face_null },	//����
	{ "/:,@o", wx_face_null },	//����
	{ "/::g", wx_face_null },	//��
	{ "/:|-)", wx_face_null },	//��
	{ "/::!", wx_face_null },	//����
	{ "/::L", wx_face_null },	//����
	{ "/::>", "/::&gt;" },		//��Ц
	{ "/::,@", wx_face_null },	//����

	{ "/:,@f", wx_face_null },	//�ܶ�
	{ "/::-S", wx_face_null },	//����
	{ "/:?", wx_face_null },	//����
	{ "/:,@x", wx_face_null },	//�꣬����
	{ "/:,@@", wx_face_null },	//��
	{ "/::8", wx_face_null },	//����
	{ "/:,@!", wx_face_null },	//˥
	{ "/:!!!", wx_face_null },	//����
	{ "/:xx", wx_face_null },	//�ô�
	{ "/:bye", wx_face_null },	//�ټ�
	{ "/:wipe", wx_face_null },	//����
	{ "/:dig", wx_face_null },	//�ٱ�
	{ "/:handclap", wx_face_null },//����
	{ "/:&-(", wx_face_null },	//�ܴ���
	{ "/:B-)", wx_face_null },	//��Ц

	{ "/:<@", "/:&lt;@" },		//��ߺ�
	{ "/:@>", "/:@&gt;" },		//�Һߺ�
	{ "/::-O", wx_face_null },	//��Ƿ
	{ "/:>-|", "/:&gt;-|" },	//����
	{ "/:P-(", wx_face_null },	//ί��
	{ "/::'|", wx_face_null },	//�����
	{ "/:X-)", wx_face_null },	//����
	{ "/::*", wx_face_null },	//��
	{ "/:@x", wx_face_null },	//��
	{ "/:8*", wx_face_null },	//����
	{ "/:pd", wx_face_null },	//�˵�
	{ "/:<W>", "/:&lt;W&gt;" },	//����
	{ "/:beer", wx_face_null },	//ơ��
	{ "/:basketb", wx_face_null },//����
	{ "/:oo", wx_face_null },	//ƹ��

	{ "/:coffee", wx_face_null },//����
	{ "/:eat", wx_face_null },	//�׷�
	{ "/:pig", wx_face_null },	//��ͷ
	{ "/:rose", wx_face_null },	//�ʻ�
	{ "/:fade", wx_face_null },	//��л
	{ "/:showlove", wx_face_null },//��
	{ "/:heart", wx_face_null },//����
	{ "/:break", wx_face_null },//����
	{ "/:cake", wx_face_null },	//����
	{ "/:li", wx_face_null },	//����
	{ "/:bome", wx_face_null },	//ը��
	{ "/:kn", wx_face_null },	//��
	{ "/:footb", wx_face_null },//����
	{ "/:ladybug", wx_face_null },//ư��
	{ "/:shit", wx_face_null },	//���

	{ "/:moon", wx_face_null },	//����
	{ "/:sun", wx_face_null },	//̫��
	{ "/:gift", wx_face_null },	//����
	{ "/:hug", wx_face_null },	//ӵ��
	{ "/:strong", wx_face_null },//��
	{ "/:weak", wx_face_null },	//��
	{ "/:share", wx_face_null },//����
	{ "/:v", wx_face_null },	//ʤ��
	{ "/:@)", wx_face_null },	//��ȭ
	{ "/:jj", wx_face_null },	//����
	{ "/:@@", wx_face_null },	//ȭͷ
	{ "/:bad", wx_face_null },	//�
	{ "/:lvu", wx_face_null },	//����
	{ "/:no", wx_face_null },	//NO
	{ "/:ok", wx_face_null },	//OK

	{ "/:love", wx_face_null },	//����
	{ "/:<L>", "/:&lt;L&gt;" },	//����
	{ "/:jump", wx_face_null },	//��
	{ "/:shake", wx_face_null },//����
	{ "/:<O>", "/:&lt;O&gt;" },	//���
	{ "/:circle", wx_face_null },//תȦ
	{ "/:kotow", wx_face_null },//��ͷ
	{ "/:turn", wx_face_null },	//��ͷ
	{ "/:skip", wx_face_null },	//����
	{ "/:&>", "/:&&gt;" },	//̫��
	{ "/:#-0", wx_face_null },	//����
	{ "/:hiphot", wx_face_null },//��
	{ "/:kiss", wx_face_null },	//����
	{ "/:<&", "/:&lt;&" },	//�٤
	{ "/:oY", wx_face_null },	//Ͷ��
};


#endif