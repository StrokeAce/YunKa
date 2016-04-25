// client common struct : ������ʹ�õ�һЩ���õĽṹ����
//
//////////////////////////////////////////////////////////////////////
#ifndef _CCOMMSTRUCT_EMS
#define _CCOMMSTRUCT_EMS

#include "../stdafx.h"
#include "comdef.h"
#include <map>
#include "tstring.h"
using namespace std ;

class CXPEdit; 
class CXPComboBox;
class CWnd;
class CBitmap;
class CRect;


typedef struct IDBUFF_INFO{
	unsigned int id;
	char *pBuff;
}IDBUFF_INFO_T, *IDBUFF_INFO_PT;

typedef struct UPFILETHREAD_STRUCT
{
	CWnd *pWnd;

	int userType;
	char strPathFile[256];
	char strFileName[256];
	char strDownURL[256];
	unsigned int recvuid;

	char visitorid[MAX_USERNAME_LEN];

	DWORD dwData;
}UPFILETHREADSTRUCT;


//�йس�ʼ�����õ�һЩ������
typedef struct INIT_CONF
{
	//���ڰ����˵��е�����
//	char sProgrameName[256];			//����ĳ������ơ�����"����...."��
	char sProgrameNameMini[256];		//����ĳ�����
	char sHelpUrl[256];					//��ת��URL

	char sCopyID[256];					//�汾
	char sHelpAbout[4][256];			//���ڶԻ����е�����
	char sHelpAboutURL[4][256];			//���ڶԻ����е�����������ת

	//0 name, 1 name(online/all), 2 name(online), 3 name(all)
//	int	 nDisplayGroupNameType;			//�黧����ʾ��Ϣ
	//0 nickname, 1 nickname(uid), 2 nickname(sid), 3 nickname(uid:strid)
	int	 nDisplayUserNameType;			//�û�����ʾ��Ϣ

	//�ÿͽӴ�
	char sVisitorServer[256];
	int nVisitorServerPort;
	
	char sDefaultServer[256];			//��½��Ĭ�Ϸ�����
	int nDefaultPort;					//��½��Ĭ�϶˿�
	char sListServer[10][256];			//����ʮ�����������б�

	int bSavePass;						//��������
	int bAutoLogin;					//�Զ���½

	int	 visitorfilter;
	//���¸�ʽΪ : 0, ����, ���
	//1 ��ʾ�������´ʻ�
	//0 ��ʾ���������´ʻ�
	char infofilter[256];
	char tailfilter[256];
	
	char sAuthAddr[256];
	int  nAuthPort;
	
	// �˳�
	char webpage_lgout[256];

	// laidian
	char webpage_webphone[256];
	// ��ѯ����
	char webpage_querywebphone[256];
	// ����
	char webpage_note[256];
	// �ͻ�����
	char webpage_crm[256];
	// ͳ�Ʒ���
	char webpage_analyze[256];
	// ��������
	char webpage_mgmt[256];
	// ���㴰��
	char webpage_news[256];
	// ע�����û�
	char webpage_regnewuser[256];
	int webpage_regnewuser_w;
	int webpage_regnewuser_h;
	
	// ����ָ��
	char webpage_tqhelp[256];
	// ͳһ����
	char webpage_UniIdioms[256];
	// ʵʱ���
	char webpage_RtmMsg[256];
	// ������Ϣ
	char webpage_SvrMsg[256];
	
	// TQ����
	char webpage_TQadviser[256];
	// �ϴ�ͷ��
	char webpage_UploadHeadImage[256];
	// ����ͷ��
	char webpage_DownloadHeadImage[256];
	// ����
	char webpage_FaceImage[256];
	// ϵͳ��Ϣ
	char webpage_SysMessage[256];
	// Ͷ�߽���
	char webpage_complaint[256];

	char webpage_SendWebFileLink[256];

	//��ȡ��˾�û�
	char webpage_companyuser[256];

	char webpage_iconurl[256];

	char webpage_faqInfo[256];

	char webpage_faqSort[256];

	char webpage_faqall[256];

	char webpage_sendurl[256];

	char webpage_repickchaturl[256];//�����Ự

	char webpage_evaluate[256];//��������

	char webpage_accesstoken[256]; // ��ȡ΢��token
	char webpage_workbillurl[256]; //����
	//�ͻ�����
	char visitorpage_visitortail[256];
	//�Ự�ǵ�
	char visitorpage_visitorbill[256];
	//�ͻ���Ϣ
	char visitorpage_visitorinfo[256];
	//ͨ����Ϣ
	char visitorpage_notice[256];
	//�¶���
	char visitorpage_visitororder[256];

	// ��½���ڵ� ע�� ��������[12/24/2010 SC]
	char registerAccount[256];
	char forgetPassword[256];
	int forgetPSVisible;

	bool bZlibEnabled; // ��visit������ͨ�ţ��ͻ����Ƿ�����ѹ��

	char query_history_records[256]; // ��ѯ��ʷ��¼
	char wechat_media_upload[256]; // ΢�ŷ�����ý���ļ��ϴ�url
	char wechat_static_map[256]; // ��Ѷ��ͼ��̬ͼurl
	char wechat_map_location[256]; // ��Ѷ��ͼ�ص��עurl
	char fileserver_media_download[256]; // �ļ����������ص�΢�ŷ�������ý���ļ���url
	char fileserver_media_upload[256]; // �ļ��������ϴ�url
	char fileserver_media_task[256]; // ���ļ���������ȡ�ļ�id��url��
	char fileserver_media_fileid[256]; // �ļ��ϴ���΢�ŷ������������ļ�����������ͬ�ļ���url��

	//������ʾĳһЩ��
//	bool bHideOfflineVisiterGroup;
}INIT_CONF_T, *INIT_CONF_PT;

//һ���������ṹ
struct FILTER_USERDEFINE
{
	//�����ַ�����Ŀ
	int num;

	//���������ֻ��ǲ�����������
	int include;

	//��������, ���10��,ÿһ����󳤶�50
	char text[MAX_STRINGFILTER_NUM][MAX_STRINGFILTER_LEN];
};

//���������
typedef struct RECT_STRUCT{
	int			type;
	int			dockhorz;			//ˮƽ����ͣ��
	int			dockvert;			//��ֱ����ͣ��

	CBitmap		*m_pNormalBitmap;	//�����ĵ�ͼ
	CBitmap		*m_pDownBitmap;	//���µĵ�ͼ
	CBitmap		*m_pFocusBitmap;	//�����ͼ
	CBitmap		*m_pDisableBitmap;	//����ʹ�õĵ�ͼ
	CBitmap		*m_pSelectBitmap;	//ѡ�еĵ�ͼ

	//Rect		m_SrcRt;		//Դͼ��λ��
	//Rect		m_DeskRt;		//���Ƶ���ͼ��λ��

	int			m_Stretch;		//�Ƿ��������С�ı���
	char 		strText[256];
	char		strTips[256];		//��ʾ�ı�

	//-1��ʾ�ô���ͳһɫ
	COLORREF	m_TextColor;		//�ı�����ɫ
	COLORREF	m_TextFocusColor;		//�ı�����ɫ
	COLORREF	m_TextDisableColor;		//�ı�����ɫ

	HICON		hIcon, hFocusIcon, hDisableIcon;
	int			iconwidth,iconheight;
	int			trianglepos;

	bool		m_bTextColor;	//�Ƿ������ı���ɫ
	bool		m_bVisible;
	bool		m_bEnable;
	bool		m_bRead;
	bool		m_bCheck, m_bIsCheck;

	int 		m_nPopupMenu;		//0 no 1 all  2 only tri
	bool		m_bDrawTriangle;

	//�ط���Ϣ
	int			index;		//���
	DWORD		WMMsg;
	WPARAM		wParam;
	LPARAM		lParam;

	CXPEdit		*m_pEdit;
	CXPComboBox		*m_pComboBox;
	bool		m_bEditFocus;
//	CEdit		*m_pEdit;
}RECT_STRUCT_T, *RECT_STRUCT_PT;

struct LOGIN_INFO
{
	//������Ϣ
	unsigned int uid;	
	
	char sid[MAX_SID_LEN+1];			//60
	char pass[MAX_PASSWORD_LEN+1];		//20

	char compid[MAX_STATUS_LEN+1];
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

//�Ի��Ӵ���Ϣ
struct TALKTOTAL_INFO
{
	int m_nOnlineTimeTotal;	//������ʱ��
	int m_ntalknum;			//�Ի�����
	int m_ntalktimetotal;	//�ܶԻ�ʱ��
	int m_ntalktimenum;		//����Ч�Ի�ʱ��ĶԻ���

	int m_nInviteNum;		//�������
	int m_nacceptinvitenum;	//���ܴ���
	int m_nrefuseinvitenum;	//�ܾ�����

	int m_nrequestnum;			//�������
	int m_nacceprequestnum;		//�Ӵ��������
	int m_nnotacceptrequestnum;	//û�нӴ��������

	int m_nnovisitormsgnum;		//�ÿ�û�з�����Ϣ
	int m_nnousermsgnum;		//�ͷ�û�з�����Ϣ

	int m_ngoodtalknum;		//һ��Ի�
	int m_nbettertalknum;	//�ϺöԻ�
	int m_nbesttalknum;		//���ѶԻ�
};

//ͳ�Ʒ�����Ϣ
typedef struct TOTAL_INFO{
	unsigned short	visittotalnum;
	unsigned short	fromsearchnum;	//����������
	unsigned short	fromfriendnum;	//���Ѻ�����
	unsigned short	frominputnum;	//ֱ������

	unsigned short	searchurlnum;	//�����MAX_LIST_TOTALURL�� ��ֻ����MAX_LIST_TOTALURL��
	unsigned int	searchurltime[VISITORTALK_MAXLIST_TOTALURL];
	char			searchurl[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			searchurlhost[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			searchurlvar[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];

	unsigned short	friendurlnum;
	unsigned int	friendurltime[VISITORTALK_MAXLIST_TOTALURL];
	char			friendurl[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			friendurlhost[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];
	char			friendurlvar[VISITORTALK_MAXLIST_TOTALURL][MAX_128_LEN];

	//�Ի�Ч��ͳ��
	unsigned short	notalknum; //û�н��жԻ��ķ��ʴ���
	unsigned short  invalidtalknum; //��Ч�Ի��Ĵ��� 

	unsigned short  talknum0;	//һ��Ի����� 2  
	unsigned short  talknum1;	//�ϺöԻ��Ĵ��� 0  
	unsigned short  talknum2;	//���ѶԻ��Ĵ��� 0 

	//�Ի�ͳ��
	unsigned short	requesttalknum;	//�ۼ���������Ի��Ĵ��� 1 ���������Զ����� 
	unsigned short	sendmsgnum;		//�����ۼƷ���ѶϢ���� 3  
	unsigned short	persendmsgnum;	//ƽ��ÿ�ζԻ�����ѶϢ���� 1  
	unsigned short	sendmsgmaxnum;	//��󵥴ζԻ�����ѶϢ���� 2  
	unsigned short  sendmsgminnum;	//������С���ζԻ�����ѶϢ 0  
	unsigned short  sendmsgtalknum;		//�ۼ��з��͹�ѶϢ�ĶԻ����� 2 

	//����ͳ��
	unsigned short	invitetalknum;	//�ۼƽ��ܿͷ��ֶ�������� 1 �����������Զ����� 
	unsigned short	refusetalk;		//�ܾ��ͷ��ֶ�����Ĵ��� 0 �������ܾ��Զ����� 

	//����ͳ��
	unsigned short	visitnum;		//�ۼƷ��ʴ��� 2  
	unsigned short	visitpagenum;	//�ۼƷ���ҳ�� 6  
	unsigned short	pervisitpagenum;	//ƽ��ÿ�η���ҳ�� 3  
	unsigned short	visitpagemaxnum;	//��󵥴η���ҳ�� 3  
	unsigned short	visitpageminnum;	//��С���η���ҳ�� 3 

}TOTAL_INFO_T, *TOTAL_INFO_PT;

struct COMBOX_ITEMINFO { 
	tstring strText;
	DWORD dwData; 
};

//�û�Ȩ�޽�ɫ�ֶεĽṹ
typedef struct ROLEAUTH_INFO{
	unsigned int id;
	unsigned int sort;
	
	char name[MAX_USERNAME_LEN];
	
	unsigned int comauth;
	unsigned int extauth;
	
	unsigned char invalid;
	unsigned char cbak;
	unsigned short usbak;
	
}ROLEAUTH_INFO_T, *ROLEAUTH_INFO_PT;

//����ṹ
typedef struct CATEGORIES_INFO{
	unsigned int id;
	
	char name[MAX_USERNAME_LEN+1];
	char	invalid;
	
	//������Ϣ�������������������
	char	solve;	//������������
}CATEGORIES_INFO_T, *CATEGORIES_INFO_PT;

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

//��������Ϣ
//size : 42
struct PROXY_INFO
{
	//dest ip and port
	unsigned int ip;						//4
	unsigned short port;					//2
	
	unsigned short proxyport;				//2
	
	//auth for proxy server
	TCHAR strUser[MAX_USERNAME_LEN];			//20
	TCHAR strPass[MAX_PASSWORD_LEN];			//20
};

//�й��û�����Ϣ��Ϣ
//max size : 670
struct USER_INFO
{
	//������Ϣ
	unsigned int uid;	
	
	unsigned char usertype;

	char sid[MAX_SID_LEN+1];			//60
	char pass[MAX_PASSWORD_LEN+1];		//20

//	char compid[MAX_STATUS_LEN+1];
	unsigned int	compid;

	char			nickname[MAX_USERNAME_LEN+1];		//����					20
	char			username[MAX_USERNAME_LEN+1];		//��ʵ����					20(40)

	char	 		mobile[MAX_PHONE_LEN+1];			//�ֻ�						20(230)
	char			phone[MAX_PHONE_LEN+1];			//�绰						20(252)
	char			email[MAX_URL_LEN+1];				//�����ʼ�					120(580)

	unsigned char	roleid;		//Ȩ�޽�ɫ
	unsigned int	comauth;						//����Ȩ��(��λ��ʾ)		4(658)
	unsigned int	extauth;						//��������״̬(��λ��ʾ)		4(662)

	unsigned int	regdate; //ע������ʱ�䣬��int���ݱ�ʾ����1970�꿪ʼ������
	unsigned char	areaadminid;			//���Ϊ��˾�ͷ�����ֵ��ʾ��˾��Ӧ��������

	unsigned short	langtype;				//������Ϣ�����԰汾				2(668)
	unsigned short	bak;					//									2(670)		
/*
	//������Ϣ
	TCHAR			nickname[MAX_USERNAME_LEN];		//��������					20
	TCHAR			username[MAX_USERNAME_LEN];		//��ʵ����					20(40)
	unsigned short	sex;							//�Ա�						2(42)

	unsigned int 	birthday;						//��������					4(46)
	TCHAR 			memo[MAX_USERMEMO_LEN];			//���						100(146)	
  
	//��ҵ�����ñ�Ź�˾�����Ż�ȡ����
	unsigned int	companyid;						//��˾���					4(150)
	unsigned int	departmentid;					//���ű��					4(154)
	
	unsigned short	country;						//��������					2(156)
	unsigned short	province;						//ʡ������					2(158)
	TCHAR			city[MAX_CITY_LEN];				//��������					30(188)
	TCHAR			zipcode[MAX_ZIPCODE_LEN];		//�ʱ����					10(198)
	  
	TCHAR	 		mobile[MAX_PHONE_LEN];			//�ֻ�						20(218)
	TCHAR			phone[MAX_PHONE_LEN];			//�绰						20(238)
	TCHAR			fax[MAX_PHONE_LEN];				//�������					20(258)
		
	TCHAR			address[MAX_ADDRESS_LEN];	    //��ַ						60(318)			
	TCHAR			homepage[MAX_URL_LEN];			//��ҳ						256(574)
	TCHAR			email[MAX_URL_LEN];				//�����ʼ�					256(830)
	TCHAR 			email_bak[MAX_URL_LEN];			//�����ʼ�(��ǰ�У���δ��)  256(δ��)
		  
	//����������ֱ�ӻ�ȡְ��͹�˾����
	TCHAR 			dutyname[MAX_DUTY_LEN];			//ְ������		            30(860)
	TCHAR 			company[MAX_COMPANY_LEN];		//��˾����					40(900)
			
//	Ȩ�ޱ�־�� 0xFFFF
//		λ    ����     
//		0     �û������Ƿ���Ҫ��֤��	0 ����Ҫ�� 1 ��Ҫ
//		1-2    ������Ȩ��				0 ���ɷ��� 1 �ɷ�ע���û���2 �����ⷢ��
//		3-4    �Ƿ���Ⱥ����Ȩ����	    0 û�У� 1 ��
//		5	   �Ƿ��з���5��������	    0 û�У� 1 ��
//		6	   �Ƿ��д���������         0 ���ɣ� 1 ����
//		7	   ��ֹ�����˼���Ϊ��ϵ��	0 ����ֹ��1��ֹ 
//		8     �Ƿ�ɷ�������ת�����ļ� 0 ���ɣ� 1 ����
//		9     �Ƿ�ɷ�������ת������Ƶ 0 ���ɣ� 1 ����
//		10     �Ƿ���Ժ���ʱ�ͻ�ͨѶ   0 ���ɣ� 1 ����ͨѶ
			  
//		12 - 15  �û���Ϣ����             0 ����; 1 ����ϵ�˹���; 2 ����
//		16     �Ƿ������������� 		0 �������� 1����
//		17     �Ƿ��������� 			0 �����أ� 1����
//		18 - 19  δ��	
//		20	   �Ƿ�����������½			0 �������� 1����
//		21	   �Ƿ�����������Ϣ			0 �������� 1����
//		22	   �Ƿ������ͨѶ��¼		0 ����ˣ� 1���	
//		23     �Ƿ���Դ�������  		0 ���ɣ� 1��	
//		25     �Ƿ��ֹ�޸�����  		0 ����ֹ�� 1��ֹ	
//		26     �Ƿ��ֹ�޸ĸ�����Ϣ     0 ����ֹ�� 1��ֹ
	unsigned int	commflag;						//����Ȩ��(��λ��ʾ)		4(904)
	
	//��������״̬(��λ��ʾ)	4(908)
//��־  0xFFFF
//		λ    ����
//		0     �Ƿ����ܵ�����Ƭ�ķ���	0 �����ܣ� 1 ����
//		1     �Ƿ������������     	0 �����ҵ��� 1 ���ҵ�
//		0 - 11  ����
//		12 - 15  ����(16)��0 ���飻 1�� ���ģ�2�� ���ƣ�3�� ��ŭ��4�� ʹ��
//		16 - 23  ��������(256), 0��ʾΪ�ͻ��˱������(>256������)
//		24 - 31  ͼ����(256)  
	unsigned int	extflag;

  //����״̬��ʾ
//	0 - 3   �û���������(16) (djz 081213 ��0-1 ��Ϊ0-3)		0�� ֱ�ӽ��� 1�� ���������룻 2����������
//	4 - 5  �û����										0�� ��ע���û��� 1 ע��ͻ�;  2����ʱ�û�
//	8 - 12  �û�״̬(16)										0 �û�����״̬��1�����ߣ�2�����ߣ�3����ȥһ�£�
//	4���벻Ҫ����
//	16      �Ƿ���Ƶ�ر�/��		0 û�У�   1 ��
	
	unsigned short	langtype;				//������Ϣ�����԰汾			2(914)
	unsigned short	externid;					//2 ������˾���
	unsigned short	bak;
*/
	unsigned int	onlineflag;				//�����ֶ�						4(912)
};

//�й��û���������Ϣ�����ݾɰ汾
struct TRADE_INFO
{
	TCHAR 			company[MAX_COMPANY_LEN];				//��˾����   40
	TCHAR 			leader[MAX_USERNAME_LEN];				//����		 δ��(20)
	unsigned short	duty;									//ְ������   2
	unsigned short	size;									//��˾��ģ   2
	unsigned short	trade;									//������ҵ   2
	
	TCHAR 			bankcode[MAX_BANK_LEN];					//��������	40		   
	TCHAR 			bank_num[MAX_BANKNO_LEN];				//�����˺�	30
	TCHAR 		    tax_num[MAX_TAXNO_LEN];					//˰�����	30
	
	TCHAR 			mobile[MAX_PHONE_LEN];					//�绰		δ��
	TCHAR 			phone[MAX_PHONE_LEN];					//�绰		20
	TCHAR 			fax[MAX_PHONE_LEN];						//����		20
	TCHAR 			homepage[MAX_URL_LEN];					//��ҳ����  60
	TCHAR 			mailbox[MAX_EMAIL_LEN];					//			60
	
	TCHAR 			bak1[MAX_VERIFYMSG_LEN];				//			40
	unsigned short	langtype;								//������Ϣ�����԰汾
	unsigned short 	companylangtype;						//			4
	TCHAR 			memo[MAX_TRADEMEMO_LEN];				//��˾���  500
};

//�ÿ͹켣��Ϣ
struct WEBUSERTAIL_INFO
{
	unsigned int intime;						//4
	unsigned int outtime;						//4
	char url[MAX_URL_LEN];					//ҳ��
	char title[MAX_URL_LEN];					//ҳ��

	char prevurl[MAX_URL_LEN];
	char prevurlhost[MAX_URL_LEN];
	char prevurlvar[MAX_URL_LEN];
};

//���������Ϣ
struct WEBBROWSER_INFO
{
	char					useragent[MAX_URL_LEN];	//�û���ͷ

	unsigned short			vcolor;					//�ͻ�����ɫ
	unsigned short          vnum;					//���ʴ���
	unsigned short          vpagenum;				//����ҳ����

	unsigned int			visittime;				//����ʱ��

	WEBUSERTAIL_INFO		tailinfo;	//����켣��Ϣ

	char					language[MAX_LANGUAGE_LEN];//���ʵ�����
//	char					timezone[MAX_TIMEZONE_LEN];//���ʵ�ʱ��
	unsigned short			vzone;

	char					browsertype[MAX_BROWSER_LEN];	//���������
	char					ostype[MAX_BROWSER_LEN];		//����ϵͳ
	char					vsize[MAX_VSIZE_LEN];			//��Ļ�������С

	//��������ͳ����ʷ��¼��
	char					sid[MAX_VISITORID_LEN];
	unsigned int			id;
	unsigned int			ip;
	char					ipfrom[MAX_USERNAME_LEN]; //��ʡһ��������

	unsigned short			requestnum;	//������
	unsigned short			invitenum;	//������
	unsigned short			userend;	//�ͷ�������
	unsigned short			clientend;	//���˽�����
	unsigned short			talknum;	//�Ի���

	char					solve;
};

//ͬһ̨���ԣ��ÿͿ��ܻ��½���ͬһ����ŵ��û�
struct WEBONLINE_INFO
{
	WEBONLINE_INFO(){bInvited=false;}
	int				fd;		//tcp����
	int				talkstatus;
	bool		    bInvited; //�����������

	unsigned int	timevisit;		//�û�����ʱ��
	unsigned int	timerequest;	//�û�����Ի�ʱ��
	unsigned int	timetalkstart;	//�û��Ի���ʼʱ��
	unsigned int	timetalkend;	//�û��Ի�����ʱ��
	unsigned int	timeleave;		//�û��뿪ҳ��ʱ�䣬Ϊ0һ���ʾ��������״̬��>0��ʾsocket�ѶϿ����������µ����ӽ׶�
	int				evalation;		//���ζԻ�����

	unsigned int	random;			//�������ߺ�������
	//���������ݿ��е����µ�id
	unsigned int	trafficid;

	unsigned char	hasmsg;			//����Ϣ�����ڻ����С�
	unsigned char	isonline;		//�Ƿ�����

	unsigned int	visitortalkid;	//������������Ϣ�ı��еı��

	unsigned char	searchkeywordsnum[4];	//ʶ������Źؼ��ʵ���Ŀ
	unsigned char	alertit;				//���û���Ҫ������ʾ�� �ڶ೤����֮�ڣ����������˶��
};

//�ÿ͵Ļ�����Ϣ�� �����Ψһ��
struct WEBUSER_INFO
{
	WEBUSER_INFO()
	{
		memset(this,0,sizeof(WEBUSER_INFO));
	}
	unsigned int	uid;				//������û�������ˣ�Ϊ�̶���SYS_WEBUSER = 9999
	unsigned int	compid;				//��˾���
	unsigned int	siteid;
	char			sid[MAX_WEBCLIENID_LEN+1];	//���Ϊ�ÿ�Ψһ��ʾ
	char			thirdid[MAX_THIRDID_LEN+1]; //������id
	char	name[MAX_USERNAME_LEN+1];
	char   nameflag;	//��¼�����Ƿ��޸� 1 ���޸�

//	char	scriptflag[MAX_SCRIPTFLAG_LEN];
//	char	
	
	unsigned char	status;		// 0 TALKSTATUS_NO, 1 STATUS_OFFLINE ���ߣ� 2 STATUS_ONLINE ���ߣ� 9 TALKSTATUS_TALK �Ի���
	char			sip[MAX_IPNUM_LEN+1];
//	char			fromip[MAX_IPSTRING_LEN];
	char			ipfromname[MAX_USERNAME_LEN+1];			//ip��Դ

	char	url[MAX_URL_LEN+1];		//��Դ��url
	char	title[MAX_URL_LEN+1];		//��Դ��url
	char	chatfrom[MAX_CHATID_LEN + 1];
};

struct WEBUSER_UPINFO
{
	//<SYSTEM><COMMAND>UP</COMMAND><IECOPYRIGHT>MSIE 7.0</IECOPYRIGHT><SYSTEMINFO>Windows 2003</SYSTEMINFO>
	//<LANGUAGE>zh-cn,zh;q=0.5(����)</LANGUAGE><VISIONCOLOR>32</VISIONCOLOR>
	//<VISIONSIZE>1024*768</VISIONSIZE><ADMINID>8008115</ADMINID>
	//<VISIT>http://www.tq.cn/?minimess_cn</VISIT><SOURCE>����ʡ�����</SOURCE><
	//SCRIPTFLAG>14641046576931075</SCRIPTFLAG><IP>125.74.161.222</IP><PORT>1230</PORT>
	//<CLIENTID>82632633301763783</CLIENTID>
	//<LASTVISIT>http://www.baoyuan28.com/index.asp?id=128</LASTVISIT>
	//<WEBTITLE> TQǢ̸ͨ</WEBTITLE><NICKNAME>����</NICKNAME><CTIMES>3</CTIMES><TTIMES>1</TTIMES>
	//<LASTVTIME>2010-8-9,12:44:55</LASTVTIME><LASTTTIME>2010-8-9,13:44:55</LASTTTIME>
	//<LASTTUIN>8071236</LASTTUIN><ISAUTOINVIT>1</ISAUTOINVIT></SYSTEM>
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
//	unsigned int	uid;
//	unsigned int userid;					//������
//	char	username[MAX_USERNAME_LEN];//������

	char		  sid[MAX_SID_LEN+1];	//���Ϊ�ÿ�Ψһ��ʾ
	char	name[MAX_USERNAME_LEN+1];		//��webuser_infoһ����

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

//�ϴ����ļ���Ϣ
struct UPFILE_INFO
{
	char	filename[MAX_FILENAME_LEN];
	char	memo[MAX_256_LEN];
	char	downurl[MAX_FILENAME_LEN];

	unsigned int uptime;
	unsigned int senduid;
	char	sendname[MAX_USERNAME_LEN];
};


//��Ϣ���б��ṹ
//950
typedef  struct MSG_LISTINFO{
	unsigned char	msgtype;					//1 ��Ϣ���
	unsigned char	msgformat;					//1 ��Ϣ��ʽ
	unsigned short	usbak1;						//2
	
	unsigned int	senduid;					//4 �����ߵ�uid����
	char			sendname[MAX_USERNAME_LEN+2];
	
	unsigned int	trafficid;
	unsigned int	talkid;
	
	//���recvuid=9999, visitorid��ʾΪ�ÿ͵�Ψһ��ʾ
	//�������ÿͶԷÿͷ�����Ϣ,recvuid senduid����ͬʱΪ9999
	char			visitorid[MAX_VISITORID_LEN+2];	//26	
	
	unsigned int	sendtime;					//4 ����ʱ��
	
	char			msg[MAX_MSG_LEN+2];			//910 ��Ϣ����
	
	unsigned short	langtype;					//2
	unsigned short	usbak2;						//2
	
}MSG_LISTINFO_T, *MSG_LISTINFO_PT;

//һ�ζԻ��Ļ�����Ϣ
typedef struct VISITORTALK_INFO{
	unsigned int 	id;
	unsigned int trafficid;
	
	//���˵ı�ź�����
	char sid[MAX_SID_LEN];
	char visitorname[MAX_USERNAME_LEN];
	
	//��ʼ�Ի��������Ի�������
	unsigned char	starttype;  //1 �����������룬�ͷ��������� 
	unsigned char	endtype; //1 ���������رգ��ͷ������ر�
	
	//�Ӵ��Ŀͷ���ź�����
	unsigned int	talkuserid;
	char	talkusername[MAX_USERNAME_LEN];
	
	//����
	unsigned char	evalulation;
	unsigned char	ucbak;
	
	unsigned int	visittime;
	unsigned int	requesttime;
	unsigned int	starttime;
	unsigned int	endtime;
	
	unsigned short	visitormsgnum;
	unsigned short	usermsgnum;
	
	//��Դip
	unsigned int	ip;
	char			ipfromname[MAX_USERNAME_LEN];			//ip��Դ
	char prevurlhost[MAX_URL_LEN];	//��Դ����
	char prevurlvar[MAX_URL_LEN]; //��������Ĳ���
	
	
	char			solve;
	
}VISITORTALK_INFO_T, *VISITORTALK_INFO_PT;

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

//�ļ�����ʹ��
//20 + 24 + 130 = 174
typedef struct FileAttr {
	
	char			szFileTitle[MAX_FILENAME_LEN];		//12	8 �ļ��ı�����
	unsigned int	dwFileAttributes;                   //4	�ļ�������
	FILETIME		ftCreationTime;                     //8 �ļ��Ĵ���ʱ��
	FILETIME		ftLastAccessTime;                   //8 �ļ���������ʱ��
	FILETIME		ftLastWriteTime;                    //8 �ļ�������޸�ʱ��
	
    unsigned int	nFileSizeHigh;                      //4 �ļ���С�ĸ�λ˫��
	unsigned int	nFileSizeLow;                       //4 �ļ���С�ĵ�λ˫��
	unsigned int	dwReserved0;                        //4 ������Ϊ0
	unsigned int	dwReserved1;                        //4 ������Ϊ0
} FILEATTR, *PFILEATTR;

//�����ļ�����Ϣ
//125
struct FILE_INFO
{
	TCHAR			strFile[MAX_FILENAME_LEN];	//256
	unsigned int	dwSize;						//4
	
	unsigned char	IsDirectory;				//1 �Ƿ�Ϊһ��Ŀ¼
};

//�Զ���������
typedef struct AutoInviteAttr {
	unsigned int	siteid;
	
	unsigned char	autoinvite;
	unsigned short	invitenum;
	
	unsigned short	deltime[10];
}AUTOINVITE_INFO, *AUTOINVITE_INFO_PT;

//�����ؼ�����Ϣ
typedef struct SEARCHKEYWORDS_INFO{
	unsigned int siteid;
	
	unsigned char keywordnum[4];
	char keywords[4][MAX_SEARCHKEYWORDS_NUM][MAX_SEARCHKEYWORDS_LEN];
	
	//��������
	unsigned char  alertopen;
	unsigned short alertsearchkeywordsnum;		//����ô��εĽ���
	unsigned short alertsearchkeywordstimes;	//�ڸ�ʱ�����
	
	unsigned short usbak;
	
}SEARCHKEYWORDS_INFO_T, *SEARCHKEYWORDS_INFO_PT;

//float msg info
struct FLOAT_MSG_INFO
{
	//������Ϣ
	unsigned int	id;						//��ҳ�ı��
	unsigned int	compid;					//��˾�ı��
	unsigned int	uid;					//�����û����
	
	//��ͨǰ����ʾ��
	char			wellcome[MAX_MSG_LEN+2];	//��ͨǰ��ӭ��
	char			connectedwellcome[MAX_MSG_LEN+2];	//�����Զ��ظ�
	
	//��������ʱ
	unsigned char	reqtimeout;			//��ʱʱ�䣬0Ϊ������		
	char			reqtimeoutmsg[MAX_MSG_LEN+2];
	
	//���˵ȴ��𸴳�ʱ
	unsigned char	waitresptimeout;
	char			waitresptimeoutmsg[MAX_MSG_LEN+2];
	
	//���˴𸴳�ʱ
	unsigned char	resptimeout;
	char			resptimeoutmsg[MAX_MSG_LEN+2];
	
	//���˴�ʱ�رնԻ�
	unsigned char	resptimeoutclose;
	char			resptimeoutclosemsg[MAX_MSG_LEN+2];
};

//float info
//size 300
struct FLOAT_BASIC_INFO
{
	//������Ϣ
	unsigned int	id;						//��ҳ�ı��
	unsigned int	compid;					//��˾�ı��
	
	char			url[MAX_URL_LEN];		//��վ����
	
	//0 Ư��������			0 �Ի���Ư���� 1 ͼƬƯ����
	//1 �Ի����λ��		0 ��ǰҳ��򿪣�1 �¿��Ի�����
	//2 �Ƿ�����������		0 û�У�1 ��
	//3 �Ƿ���Թر�		0 �����ԣ�1 ����
	//4 �Ƿ�����϶�		0 �����ԣ�1 ����
	//5 - 6 ���������      0 ���м���ʾѯ�ʿ� 1 �ڵ�ǰλ����ʾ�Ի���
	unsigned int	comauth;
	
	unsigned int	extauth;
	
	//Ư����λ������Ϊ0, left topȡֵ0 1 ��ʾ��/�� ��/�¶���
	//Ư����λ������Ϊ1, left top�ֱ�Ϊ��߿���룬��ֵΪ��/�ײ��ľ���
	int				left;	
	int				top;
	
	//��ɫ����Ĭ��16����ɫ����ѡ��
	int				colorindex;
	
	//�Ի�ʽƯ�����logo�ļ�, ����
	char			logofile[MAX_FILENAME_LEN];
	char			title[MAX_TITLE_LEN];
	char			tail[MAX_TAIL_LEN];
	
	//�������Ƿ���ʾ�ı�ʾ�����ַ���0111��ʾ��˳���ʾͼ���λ�ã�0��ʾ��ʾ��1��ʾ����ʾ
	char			showtoolicon[20];
	
	//�������������
	unsigned char	invitetype;	//����
	char			invitetitle[MAX_URL_LEN];
	char			inviteword[MAX_MSG_LEN];
	char			invitebgfile[MAX_FILENAME_LEN];
	char			inviteacceptfile[MAX_FILENAME_LEN];
	char			inviterefusefile[MAX_FILENAME_LEN];
	unsigned int	inviteposx;	//��λ�ֱ�ʾ���ͣ� ��λ�ֱ�ʾ����
	unsigned int	inviteposy;
	
	//ͼƬʽƯ�����ͼƬ�ļ�
	char			icononlinefile[MAX_128_LEN];
	char			iconofflinefile[MAX_128_LEN];
	
	char			icononlinealt[MAX_128_LEN];
	char			iconofflinealt[MAX_128_LEN];
	
	char			iconurl[MAX_128_LEN];
	
	//ҳ�����Ա���
	char			language[50];
	
	unsigned char	initshow;			//��ʼ��ʾ��ʽ
	
	//�������
	char			adicon[MAX_FILENAME_LEN+2];
	char			adurl[MAX_URL_LEN+2];
	
	AUTOINVITE_INFO autoinviteinfo;
	
	SEARCHKEYWORDS_INFO searchkeywordsinfo;
	
	FLOAT_MSG_INFO msginfo;	//�����׵�������Ϣ
};

struct LISTCHATINFO
{
	
	char			chatid[MAX_CHATID_LEN+1];
	unsigned int uKefuUin;	  //�ͷ��ĺ���,0:�ȴ�Ӧ��״̬��>0:���ںͿͷ�ͨ��
	
};

/*
struct RECVCHATINFO
{
	unsigned int uFromAmdind; //�Ự��Դ����Ա����
	unsigned short sFromSort; //�Ự��ԴƯ����id
	
	char			chatid[MAX_CHATID_LEN+1];
	
	unsigned int tStartTime;		  //�Ự��ʼʱ��
	unsigned int uWebUin;	  //web�����ĺ���
	unsigned int uKefuUin;	  //�ͷ��ĺ���,0:�ȴ�Ӧ��״̬��>0:���ںͿͷ�ͨ��
	
	unsigned int uadminuin;	 //����Ա����
	unsigned short sSort;	 //Ư����id
	char	strClientId[MAX_VISITORID_LEN+1];//�ÿ�Ψһ��ʾ
	char	webnickname[MAX_USERNAME_LEN+1];//�ÿ�����
	
	// �Ựת��״̬
	unsigned int Transfertoadminuin;	 //�Ự���ܷ�����Ա����
	unsigned int TransfertoKefu;	     //�Ự���ܷ�Ư����id
	
};
*/

//��ʷ��¼��������
typedef struct VISITORTALK_FILTER
{
	//2��ʱ����Ϊ��ѯ����ʱ��ʹ��
	unsigned int starttime;
	unsigned int endtime;

	char talktype;
	char talkstarttype;
	char talkendtype;
	
	unsigned int firstuin;
	unsigned int transferuin;
	
	char iptype[256];
	char searchurl[256];
	
	char searchkeywords[256];
	char keywords;
	
	char friendurl[256];
	char talkurl[256];
	char language[256];
	
	char clientsort;	//�������
	char evaluation;	//��������
	
	char frominfo[256];
	char fromtail[256];
	char fromnameandip[256];
	char fromtalkmsg[256];
	char frommemo[256];
	
	DWORD dwData[10];
	char sbak[10][256];
}VISITORTALKFILTER;

typedef struct WORKBILLTB_ITEM
{
	char  chatid[30];			//--�������(��Ӧtqdb03,WORKBILL�е�id)

	int billtype;			//--�������ͣ�����1~20����������20~30��������ѯ:1,����Ͷ��:2,��Ʒά��������ѯ3,Ա����ѯ 4,�ͻ����� 5,����Ͷ��:21,���Խ���:22,������ѯ:23��
	int billsubtype;

	unsigned int adminuin;	// --����Ա����
	unsigned int kefuuin;	// --�ṩ����Ŀͷ�UIN
	unsigned int deptid;	// --����id

	unsigned int clientuin;	// --�ÿͻỰuin
	char clientname[100];		// --�ͻ�����
	char clienttype[50];		// --�ͻ�Ʒ��
	int  clientlevel;			// --�ͻ�����
	char clientproduct[30];		// --�ͻ���Ʒ
	char clientmobile[15];		// --�ͻ��ֻ�
	char clientphone[20];		// --�ͻ��̶��绰
	char clientemail[50];		// --�ͻ�����
	int clientemailaccpet;		//  --�û�email�Ƿ����ҵ���Ƽ���Ϣ��1���գ�0 ������
	char clientotherinfo[100];	// --������ϵ����ʽ �������ز�����ĵ绰
	char clientid[40];			// --������id
		
	unsigned int talkstarttime;	// --����Ựʱ��
	unsigned int talkendtime;		// --�����Ựʱ��
	unsigned int talktime;			// --����ʱ��
	
//	DEAL_TIME           numeric(28), --�ͻ�Ҫ����ʱ�䣬���ǻز����룬��Ϊ�ͻ�Ҫ��ز���ʱ��
//	DEAL_TIME_LATE      numeric(28), --�ز������ʱ��
		
//		MESSAGE_TITLE       TEXT(50),  --���Ա���
//		SUMMARY             TEXT(100), --����ժҪ
//		TALK_CONTENT        Memo,  --�Ự����
	int usermsgnum;			// --���λỰ,�ͷ��Ļظ���(TALK_CONTENT�пͷ�˵���Ĵ���)
	int visitorcluesnum;	// --���λỰ������
//		REQUEST_REPLY_TYPE  TEXT(50),  --�ͻ�Ҫ��Ĵ𸴷�ʽ
//		REPLY_TYPE          TEXT(50),  --�ͷ�ʵ�ʴ𸴷�ʽ
//		REPLY_CONTENT       Memo,  --�ͷ�������
		
		
//		LAW_ADDRESS         TEXT(50),  --Ͷ�ߵص�
//		LAW_SIGNAL          TEXT(10),  --�ֻ��ź�����
//		LAW_CONNECT         TEXT(10),  --ͨ������
//		LAW_FREQUENCY       TEXT(10),  --���Ϸ���Ƶ��	

	int billresult;			// --���������δ������0���Ѵ�����1��ת�����ˣ�3��
	int evalution;			// --����(��ӦABCD�ļ���)
	int transferok;			// --ת���Ƿ�ɹ�(0.û��ת�ƣ�1.���ɹ� 2.�ɹ�)
	char transferpath[200];	// --ת��·��(��80008115->8362009)
	char memo[1024];
		
	int validtalk;			// --�Ự��Ч,��Ч��1����Ч��0
	int losttalk;			// --�Ự��ʧ,��ʧ��1��δ��ʧ��0
		
	unsigned int clientrand;	// --�ÿ�Ψһ��ʾ
	unsigned int visitorip;	// --�ÿ���Դip
	char visitorregion[100];	// --ip��������

	char visitorentry[50];		//--�ÿͳ�ʼ����Դ(Ŀǰֻ����������)
	char visitorkeywords[50];	// --���������еĲ��ҹؼ���

	char visitorprevurl[200];	// --�ÿ��ϸ�ҳ��
	char visitorurl[200];		// --��ǰҳ��(����������ҳ��)
		
	char outworkbillid[50];		//50  --�ⲿ������
		
	unsigned int billadminuin;	// --���������ˣ�Ĭ��Ϊkefu_uin,�Ựת��ʱ�޸Ĵ��ֶ�
	int talkfrom;				// --�Ự��Դ(0 ���� 1 ������Զ����� 2�ͻ����Զ����� 3�ͻ����ֶ�����)
	int worktype; 
}WORKBILLTBITEM;

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
	tstring filaPath;			// ���͵��ļ�·��
	int userType;				// �����ļ����û�����
	tstring msgId;				// ��Ϣ�����к�
	int msgDataType;			// ��Ϣ����������
};

// ���½���ý���ļ�ʱ�����ն������Ϣ����
struct RERECV_FILE_PARAMS
{
	tstring filaPath;			// �ļ��Ĵ洢·��
	tstring url;				// �����ļ���url
	int userType;				// ������Ϣ���û�����
	tstring msgId;				// ����Ϣ�����к�
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
	tstring msgId;	// ��Ϣid
	tstring msg;	// ��Ϣ����
};

// ���·�����Ϣ������������Ϣ��ý���ļ���json����Ϣ
struct RESEND_MSG_PARAMS
{
	tstring msgId;			// ��Ϣid
	unsigned long userId;	// ������Ϣ�û���id
	int userType;			// �û�����
	string mediaId;			// ý���ļ���΢�ŷ�������id,��Ϣ���������ֵ�ʱ�򣬸ò�������������Ϣ����
	int msgDataType;		// ��Ϣ��������
	string fileId;			// ý���ļ����ļ���������id
	tstring filePath;		// �ļ��ڱ��ش洢·��
};

#endif