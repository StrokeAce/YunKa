#ifndef _COMMDEF_EMC
#define _COMMDEF_EMC
#include <list>

#define SOLVEFLAG_NULL			0
#define SOLVEFLAG_UPDATE		1
#define SOLVEFLAG_ADD			2
#define SOLVEFLAG_DELETE		3

#define EXMIS_NORMAL			0
#define EXMIS_POPUP				1
#define EXMIS_TRIANGLE			2

//�ظ�������
#define RESPINVITE_SUCC		0
#define RESPINVITE_FAIL		1
#define RESPINVITE_TIMEOUT	2

//ͼ��ķ���
#define ICON_MAINFRAME		0	//����ܴ��ڵ�ͼ��(����)

//Tree��ʹ������
#define TREE_NORMAL		0	//�û�
#define TREE_USER		1	//�û�
#define TREE_KEYWORD	2	//��ݷ�ʽ

//�ļ���ʽ�Ŀ�ʼ����
#define FLAG_COPYNO				1000
#define FLAG_FILE_START			0x454d
#define FLAG_FILE_END			0x55AA

//�汾���·���
#define COPY_NOTCHECK			0
#define COPY_AUTOCHECK			1
#define COPY_TIMECHECK			2

//���ڵ�ͣ��λ��
#define WNDDOCK_NO		0
#define WNDDOCK_TOP		1
#define WNDDOCK_LEFT	2
#define WNDDOCK_RIGHT	3

//�Զ��ظ�, ��ݻظ�����������
#define MAX_CONTENT_RESEND	10  
#define MAX_LOGININFO		10  

//Ƥ������ʹ��
#define EMMIS_NORMAL			0
#define EMMIS_POPUP				1
#define EMMIS_TRIANGLE			2

//static ���������
#define STATIC_NORMAL	0
#define STATIC_JUMPURL	1
#define STATIC_JUMPCMD	2

//Ƥ������
#define FACE_WIN			0
#define FACE_EM2005			1

//����ʹ�õ��ַ���
#define TRAY_STRING_FIRST		0	//��δ��¼�ɹ���
#define TRAY_STRING_NORMAL		1	//zh
#define TRAY_STRING_NEWMSG		2

//ϵͳ�û�
#define SYS_ALLUSER				997		//��ʾΪȺ��Ϣ�� ��˾�����˶������յ�
#define SYS_WEBUSER				998		//��ʾΪ�ÿͣ������ʾ��sid������
#define SYS_SYSUSER				1000

//�û�����ʾ����
#define DISPLAYUSER_NAME			0	//�û���
#define DISPLAYUSER_NAMEUID			1	//�û���(���)
#define DISPLAYUSER_NAMESID			2	//�û���(��½��)
#define DISPLAYUSER_NAMESIDUID		3	//�û���(��½��:���)

//��ӭ��
#define WELLCOME_ALL				0	//����������ȡ���еĿ�������Ϣ
#define WELLCOME_NORMAL				1	//��ӭ��-�򿪶Ի�����ʾ�ĵ�һ����Ϣ���ݡ�
#define WELLCOME_CONNECTED			2	//�����Զ��ظ�--���ӳɹ����͵Ļ�ӭ�ʡ�
#define WELLCOME_REQTIMEOUT			3	//��������ʱ--���˷��͵�һ�仰��ʼ��û�пͷ���Ӧ��
#define WELLCOME_WAITRESPTIMEOUT	4	//���˵ȴ��𸴳�ʱ--���˷�����Ϣ��ȴ���ʱ���͵���Ϣ
#define WELLCOME_RESPTIMEOUT		5	//���˴𸴳�ʱ--����û�з�����Ϣ��ʱ��
#define WELLCOME_RESPTIMEOUTCLOSE	6	//���˴𸴳�ʱ�رնԻ�--����û�з�����Ϣ��ʱ�䳬������ֹ�Ի�

#define IM_NORMAL			0
#define IM_QQ				1
#define IM_MSN				2

#define TALKDLGTOOL_GIF			0
#define TALKDLGTOOL_CUTSCREEN	1
#define TALKDLGTOOL_FILE		2
#define TALKDLGTOOL_QUICKREPEAT	3
#define TALKDLGTOOL_LANGUAGEEN	4
#define TALKDLGTOOL_APPRAISE	5
#define TALKDLGTOOL_CALL		6
#define TALKDLGTOOL_SAVE		7
#define TALKDLGTOOL_LANGUAGEZH	8
#define TALKDLGTOOL_QUESTION	9
#define TALKDLGTOOL_PRINT		10

#define LISTUSER_ALL		0
#define LISTUSER_ONLINE		1
#define LISTUSER_OFFLINE	2

#define LISTUSER_FORBID		3

#define	NEXT_NULL	0
#define	NEXT_UPFILE	1
#define	NEXT_SAVE	2


#define	GETANDSAVE_NULL	0
#define	GETANDSAVE_GET	1
#define	GETANDSAVE_SAVE	2

#define UPDATA_STATUS_NULL	0
#define UPDATA_STATUS_FILE	1
#define UPDATA_STATUS_INFO	2
#define UPDATA_STATUS_SUCC	3

#define WEBUSERICON_NULL			-1
#define WEBUSERICON_NORMAL			0
#define WEBUSERICON_VISIT1			1
#define WEBUSERICON_VISIT10			2

#define WEBUSERICON_SEARCHURL		3
#define WEBUSERICON_SEARCHURLMID	4
#define WEBUSERICON_SEARCHURLTOP	5
#define WEBUSERICON_FRDURL			6
#define WEBUSERICON_UNIONURL		7
#define WEBUSERICON_INOUT			8	// ͨ������ת���󷵻صķÿ� [12/9/2010 SC]
#define WEBUSERICON_DIRECTURL		9
#define WEBUSERICON_ALERT			10
#define WEBUSERICON_REFUSE			11
#define WEBUSERICON_MUTIUSER		12
#define WEBUSERICON_OFFLINE			13
#define WEBUSERICON_FORBID			14
#define WEBUSETICON_ALERTMSG		15
#define WEBUSETICON_ALERTPHONE		16

/* 0xFF����Ҫ������Դ�
*  û�취
*/
#define WEBUSERICON_OTHERURL		0xFF



//ͳ�Ʒ������
#define TOTAL_OVERVIEW			001

#define TOTAL_SEARCHURL			101
#define TOTAL_SEARCHKEY			102

#define TOTAL_FROMCURURL		201
#define TOTAL_FROMPREVURL		202
#define TOTAL_FROMPREVURLSORT	203

#define TOTAL_DATETIMEMONTH		301
#define TOTAL_DATETIMEDAY		302
#define TOTAL_DATETIMETIME		303

#define TOTAL_DEPTNUM			401
#define TOTAL_DEPTURL			402
#define TOTAL_DEPTTIME			403

#define TOTAL_CLIENTAREA		501
#define TOTAL_CLIENTBROWSER		502
#define TOTAL_CLIENTSIZE		503
#define TOTAL_CLIENTOS			504

#define TOTAL_TALKUSERINFO		601
#define TOTAL_TALKUSERACCEPT	602


//��������
#define	ERROR_READ_FILE			400
#define	ERROR_FILE_EXT			401

//��������
#define SOUND_ONLINE					0
#define SOUND_INVITE					1
#define SOUND_MSG						2
#define SOUND_RING						3
#define SOUND_ALERT						4

#define COLOR_MAINBORDER			RGB(171,198,221)
#define COLOR_MAINBACKGROUND		RGB( 240, 244, 248)

//����ɫ
#define COLOR_BACKGROUND_ALL			RGB(255,255,255)
//itemѡ�еı���ɫ 
#define COLOR_BACKGROUND_ITEMSELECT		RGB(0x39, 0x5b, 0xaa )

//itemͷ�ı���ɫ
#define COLOR_ITEMHEAD_BACKGROUND		RGB( 240, 244, 248)	//f0f4f8
//itemͷ���ı�ɫ
#define COLOR_ITEMHEDA_TEXT				RGB( 13,19,23 )

//�ı�������ɫ��ѡ�н���ɫ
#define COLOR_TEXT_NORMAL		RGB(0,0,0)
#define COLOR_TEXT_SELECT		RGB(255,255,255)
#define COLOR_TEXT_TALKED		RGB(0,0,255)
#define COLOR_TEXT_FLASH		RGB(253,194,72)		//fdc248

//�û���ı���ɫ
#define COLOR_USERITEM_BKCOLOR	RGB(255,255,255)

//�������item�ı���ɫ
#define COLOR_OFFLINE_GROUP		RGB(0x80, 0xa7, 0xff)	//f0f4f8
#define COLOR_OFFLINE_SUBGROUP	RGB(0xcd, 0xdd, 0xff)	//f0f4f8
#define COLOR_OFFLINE_ITEM		RGB(240, 232, 219)

//�������item�ı���ɫ
#define COLOR_ONLINE_GROUP		RGB(0x80, 0xa7, 0xff)	//#becff0
#define COLOR_ONLINE_SUBGROUP	RGB(0xcd, 0xdd, 0xff)	//#e1e9f7
#define COLOR_ONLINE_ITEM		RGB(254, 240, 201)

//�ȴ�Ӧ��item����ɫ
#define COLOR_WAITRESP_ITEM		RGB(254, 0, 0)


//�ӵ�һ����ʼ�� ������������ÿһ���

#define	TREELPARAM_ALERT_MSG		50  // ��������
#define	TREELPARAM_ALERT_PHONE		51	// ���絯��
#define TREELPARAM_ONLINVISITOR		52	// ���߷ÿ�

#define	TREELPARAM_WAIT_GROUP		100	// �ȴ���ʼ
#define	TREELPARAM_WAIT_RESP		101	// �ȴ�Ӧ��
				
#define	TREELPARAM_USER_TALK		200	// �Ի���
#define	TREELPARAM_USER_TRANSFER	201	// ת����
#define	TREELPARAM_USER_INVITE		202	// ������
#define	TREELPARAM_USER_INNERTALK	203	// �ڲ��Ի�

#define	TREELPARAM_VISIT_AUTOINVITE	300	// �Զ�����
#define	TREELPARAM_VISIT_ONLINE		301	// ������
#define	TREELPARAM_VISIT_OFFLINE	302	// ���뿪

#define	TREELPARAM_MAX_VALUE		999

#define TYPESELECT_TXT				0
#define TYPESELECT_URL				1
#define TYPESELECT_FILE				2

#define TYPESELECT_MSG				0 // ��һ��ѡ�
#define TYPESELECT_INFO				1 // �ÿ���Ϣ
#define TYPESELECT_CLIENT			2 // ������Ϣ
#define TYPESELECT_CHATID			3 // �Ự�ǵ�
#define TYPESELECT_CLIENTINFO		4 // �ͻ���Ϣ
#define TYPESELECT_NOTICE			5 // �½�����
#define TYPESELECT_ORDER			6 // ��ѯ����

#define TYPESELECT_WEBURL			10 // ��ʾ��ʷ��¼��ҳ��
#define TYPESELECT_WEBADMIN			11 // ��ʾ��������

//����������������
#define CONNECT_UDP				0x0000		//UDP ����UI
#define CONNECT_TCP				0x0001		//TCP ����UI
#define CONNECT_SSL				0x0002		//SSL ����UI
#define CONNECT_HTTP			0x0003		//HTTP ����proxy01.tq.cn
#define CONNECT_HTTPS			0x0004		//HTTP ����proxy01.tq.cn
#define CONNECT_SOCKET			0x0005		//socket ����UI
#define CONNECT_TCPPROXY		0x0101	// TCP ͬʱ  proxy 0x0100|CONNECT_TCP
#define CONNECT_SSLPROXY		0x0102	// SSL ͬʱ  proxy 0x0100|CONNECT_SSL
#define CONNECT_HTTPPROXY		0x0103	// HTTP ͬʱ proxy 0x0100|CONNECT_HTTP

//�������������������
#define PROXY_NO				0
#define PROXY_SOCKET			1
#define PROXY_HTTP				2

//����NAT�û����ӵĲ�������
#define P2PCONNECTNAT_SEND			0	//send 
#define P2PCONNECTNAT_RESP			1	//resp direct
#define P2PCONNECTNAT_RESPEX		2	//resp by server

//����NAT�û����ӵ�����
#define P2PCONNECTNAT_NORMAL		0	//msg
#define P2PCONNECTNAT_VIDEO			1	//video
#define P2PCONNECTNAT_AUDIO			2	//audio
#define P2PCONNECTNAT_FILE			3	//file


#define HASTALKED		1	//ͨ����
#define INTALKING		2	//����ͨ����


//����ͼ����״̬
#define STATUS_LEN				10


#define ANNOUNCEMENTEVENT_RINGING		1
#define ANNOUNCEMENTEVENT_FREEPHONE		2
#define ANNOUNCEMENTEVENT_WEBNOTE		3
#define ANNOUNCEMENTEVENT_BROADCAST		4

//userObject�л�ȡ������Ϣ��Ҫ
#define ONLINE_INFO_CONNECTTYPE		0
#define ONLINE_INFO_CONNECTTYPE_LEN	4
#define ONLINE_INFO_USERTYPE		4
#define ONLINE_INFO_USERTYPE_LEN	2
#define ONLINE_INFO_STATUS			8
#define ONLINE_INFO_STATUS_LEN		4
#define ONLINE_INFO_VIDEO			16
#define ONLINE_INFO_VIDEO_LEN		1


//�ÿ͹��˱�ʾ
//�ñ�ʾֻ�Եȴ��Ӵ��������к����뿪��Ч
#define VISITORFILTER_ALL			0
#define VISITORFILTER_MYVISITOR		1	//ֻ��ʾ���Լ��ķÿͣ�����ʾ�����к����뿪���ˣ��ڵȴ��Ӵ�����ʾ
#define VISITORFILTER_ALLVISITOR	2	//��ʾȫ���ͷ��ķÿͣ�
#define VISITORFILTER_1MINUTES		3
#define VISITORFILTER_3MINUTES		4
#define VISITORFILTER_5MINUTES		5
#define VISITORFILTER_10MINUTES		6
#define VISITORFILTER_USERDEFINE	10

//һ��ͨѶ���е��б������
#define MAX_LIST_DEPARTMENT		25
#define MAX_LIST_FRDGROUP			100		//djz
#define MAX_LIST_FRD			500		//djz
#define MAX_LIST_ONLINEFRD		500		//�Ƕ���״̬����ϵ���� 50 ->500
#define MAX_LIST_ONLINEFRDEX	250		//�Զ���״̬����ϵ���� 25->250
#define MAX_LIST_QUERYFRD		12		//�����û�
#define MAX_LIST_URL			4		//URL�������
#define MAX_LIST_GROUP			25		//�顢����ͻ���
#define MAX_LIST_QUERYGROUP		15		//����Ⱥ�û�
#define MAX_LIST_MUTIUSER		50		//Ⱥ����Ϣ���û���
#define MAX_LIST_FILE			3		//�����ļ������ļ��ĸ���
#define MAX_LIST_FILEEX			15		//���͹����ļ�����ʹ��,�ļ�����������60������
#define MAX_LIST_UIN			100		//Ⱥ����Ϣ�ܽ��յĸ���
#define MAX_LIST_NAMEID			50		//name + id

#define MAX_OFFLINE_MSG	20
#define MAX_UPDATE_FRDLIST_NUM 100

//�û��ն�����
#define TERMINAL_PC					0
#define TERMINAL_PHONE				1
#define TERMINAL_WEBUI				2
#define TERMINAL_PCEX				0

//��¼���Ƶ�ѡ�� 
#define LOGIN_BYUID				0
#define LOGIN_BYSTRING			1
#define LOGIN_BYMOBILE			2
#define LOGIN_BYMAIL			3
#define LOGIN_BYWEB				50

//��¼��ʽ
#define LOGIN_NORMAL			0  //������¼
#define LOGIN_OFFLINE			1  //���ߺ��ٵ�¼
#define LOGIN_SERVERDOWN		2  //�������������Զ���¼
#define LOGIN_OFFLINEAUTO		3  //���ߺ��Զ��ٵ�¼
#define LOGIN_LITE				10  //�������ߣ�����ʱmpֻ����½�ɹ���ʧ�ܰ������������������ѷ��飬�����б������б� 2010.1.12

#define OFFLINE_STATUS_MSG		10	//����ʱ�õı�־λ
#define OFFLINE_STATUS_SMS		11		


//����������������
#define MSG_UDP					0
#define MSG_NET					1
#define MSG_PROXY				2
#define MSG_WEBUI				3
#define MSG_TCPUI				4//��tcp ui
#define MSG_SSLUI				5//ssl ui
#define MSG_HTTP				6 //��proxy01.tq.cn��¼

//��ѯ�û�������
#define SEARFRD_ID		0
#define SEARFRD_STRID	1
#define SEARFRD_MOBILE	2
#define SEARFRD_EMAIL	3
#define SEARFRD_ADDME	100

//���͵���Ϣ����
#define MSG_NORMAL			0 // ��ͨ��Ϣ

#define MSG_FLOAT_NORMAL	2	// ��ͨ��Ϣ
#define MSG_FLOAT_PRV		3	// Ԥ֪��Ϣ
#define MSG_FLOAT_INPUTING	4	// �ͷ�����������Ϣ
#define MSG_WX				50	// ����΢�Ŷ˵�json��Ϣ����������xmlת��Ϊjson

#define MSG_WORKBILL       101  //������
#define MSG_GET_WORKBILL   102  //��ȡ������
#define MSG_FONT           103 //����������Ϣ
#define MSG_GET_EVALUATE   104 //��ȡ������Ϣ
#define MSG_CLIENT_EXIT		105 //��ҳ�������뿪
#define MSG_CLIENT_COME    106  // ��ҳ����������
#define MSG_TRANSFER_REQUEST 107 //�Ựת������
#define MSG_TRANSFER_ANSWER  108 //�Ựת��Ӧ��

//------�ͻ���ver>MULTI_TERMINAL_HTTPBLOCK_SYSDEF(4223)֧��------------
#define MSG_CLIENT_EXIT_MSG   109 	    	//�ÿ��뿪ʱ������ϯ����ʾ��ϢϢ���ÿ����뿪��
#define MSG_CLIENT_APPRAISE_MSG   110 		//�ÿͶԿͻ���������ʾ��Ϣ
#define MSG_CLIENT_CHANGE_ADDRESS_MSG   111 	//�ÿͶԿͻ���������ʾ��Ϣ
#define MSG_CLIENT_SENDFILE_FAILED_MSG   112 	//�ÿͷ����ļ�ʧ����ʾ��Ϣ
#define MSG_CLIENT_SENDFILE_SUCCESS_MSG   113 	//�ÿͷ����ļ��ɹ���ʾ��Ϣ
#define MSG_CLIENT_TRANSFER_IN_MSG   114 	//[ϵͳ��Ϣ] �ÿ�ѡ��������ͨ��
#define MSG_CLIENT_TRANSFER_OUT_MSG   115 	//[ϵͳ��Ϣ] �ÿ��Ѿ����Ựת�Ƶ�%s(%u)��
#define MSG_CLIENT_ALREADYEXIT_MSG   116 	//�ÿ������뿪 ������Ϣ���޷�׼ȷ�ʹ� ��ѡ�������֪ͨ��ʽ
#define MSG_CLIENT_WELCOME_MSG   117 		//��ҳ���������˸��ͻ��˵Ļ�ӭ����ʾ��Ϣ �����վ��������
#define MSG_CLIENT_COLLECT_MSG   118 		//js�����ͷ��ķÿ��ռ�ҳ���ռ�������ʾ��Ϣ
#define MSG_KEFU_GET_CHAT_HISMSG   120 		//��ϯ��webui��Ҫchat�Ự����ʷ��Ϣ��2015��11��22��
#define MSG_WEBUI_SEND_CHAT_HISMSG   121	//webui����ϯ����chat�Ự����ʷ��Ϣ��2015��11��22��
#define MSG_KEFU_GET_WX_USERINFO	122	//��ϯ��webui��Ҫ΢���û���Ϣ��2015��11��26��
#define MSG_KEFU_GET_WX_ACCESSTOKEN		124	//��ϯ��webui��Ҫ΢��actoken��2015��11��26��


#define MSG_SEND_FAIL       1000 //��Ϣ����ʧ��

#define MSG_ADDFRD			200
#define MSG_DELETEFRD		201

#define MSG_RESPADDEFRD		205		//ͬ�����
#define MSG_RESPDENYFRD		206		//�ܾ�����

//���͵���Ϣ��ʽ
#define MSG_TXT				0  // �ı���Ϣ
#define MSG_RTF				1  // rtf��ʽ�ı���Ϣ
#define MSG_HTML			2  // html��ʽ��Ϣ
#define MSG_RTFFILE			3  // ��rtf��ʽ������ļ���Ϣ����ʱ��Ϣ����Ϊ�ļ���


//���͵���Ϣ��ʽ
#define MSG_TXT				0  // �ı���Ϣ
#define MSG_RTF				1  // rtf��ʽ�ı���Ϣ
#define MSG_HTML			2  // html��ʽ��Ϣ
#define MSG_RTFFILE			3  // ��rtf��ʽ������ļ���Ϣ����ʱ��Ϣ����Ϊ�ļ���

//MSG�������BAK�����ֶε�ʹ��Լ��dw
#define MSG_BAK_NORMAL    0			// BAK�ֶε�Ĭ��ֵ
#define MSG_BAK_INPUTING  1			// �Է���������
#define MSG_BAK_AUTOANSER 2			// �Զ��𸴣�����������
#define MSG_BAK_TRANSFER_ACCEPT	3	// ���ܻỰת��
#define MSG_BAK_TRANSFER_TIMEOUT 4	// �Ựת�Ƴ�ʱ

//nameid����������
#define NAMEID_ID				0  //uid 
#define NAMEID_IDNAME			1  //uid + name
#define NAMEID_IDNAMEIPPORT		2  //uid + name + ip + port
#define NAMEID_IDONLINE			3  //uid + ip + port + videoip, videoport
#define NAMEID_IDNAMESTATUS		4  //uid + name + status

//�Ա��־
#define SEX_ALL			0
#define SEX_MAN			1
#define SEX_WOMAN		2

//��½״̬
#define LOGINSTATUS_ONLINE			0
#define LOGINSTATUS_ONLINE_LEN		4
#define LOGINSTATUS_VIDEO			5
#define LOGINSTATUS_VIDEO_LEN		1

//�������յ���û�н��봦��
#define COMM_ERROR_NO					0
#define COMM_ERROR_UNKNOWN				100	//other
#define COMM_ERROR_SERVERBUSY			1	//server is busy ������æ
#define COMM_ERROR_ERRORCOPY			2	//copy is not correct ���������б�־�Ϳͻ��˲�һ��
#define COMM_ERROR_ERRORCMD				3	//
#define COMM_ERROR_ERRORUID				4	//
#define COMM_ERROR_ERRORPACKSIZE		5	//
#define COMM_ERROR_ERRORGETDB			6	//
#define COMM_ERROR_ERRORUNPACK			7

#define COMM_ERROR_COMPISNOTEXIST		8
#define COMM_ERROR_USERISNOTEXIST		9
#define COMM_ERROR_ERRORPASS			10
#define COMM_ERROR_TOOMANYUSER			11
#define COMM_ERROR_FORBIDHIDELOGON		12
#define COMM_ERROR_FORBIDUSER			13
#define COMM_ERROR_TOOMANYRANDUSER		14
#define COMM_ERROR_CATCHERROR			15
#define COMM_ERROR_DENYRANDUSER			16
#define COMM_ERROR_FAILGETRANDUID		17
#define COMM_ERROR_FAILADD				18
#define COMM_ERROR_SENDPACK				19
#define COMM_ERROR_RECVPACK				20
#define COMM_ERROR_WEBUSERISNOTEXIST	21
#define COMM_ERROR_SENDWEB				22
#define COMM_ERROR_RECVWEB				23
#define COMM_ERROR_TRANSFERERROR		24
#define COMM_ERROR_EXPIREDADE			25
#define COMM_ERROR_DBSQL				26


#define DENYLOGIN_USERISNOTEXIST	1		// �û�������
#define DENYLOGIN_PASSWORDERROR		2		// �������
#define DENYLOGIN_TOOMANY			3		// �������û���̫����
#define DENYLOGIN_FORBIDHIDELOGON	4		// ���û����������½
#define DENYLOGIN_FORBIDUSE			5		// �û��ѽ���
#define DENYLOGIN_TOOMANYCLIENT		6		// �������û������ѳ������
#define DENYLOGIN_SERVERBUSY		8		// ��������æ��һ���ǻ�ȡ���ݿ�����ʧ����
#define DENYLOGIN_OLDCOPY			10		// �ͻ��˰汾̫�ͣ��������ܾ���½!
#define DENYLOGIN_INBLACKLIST		17		// �û��ں�������
#define DENYLOGIN_TOOOFTEN		    18		// ��½̫Ƶ����
#define DENYLOGIN_SERVERERROR		19		// �������ڲ�����
#define DENYLOGIN_LOGONTYPEERROR	20		// ��½��ʽ��ʶ��
#define DENYLOGIN_UNPACKERROR	    21		// �������

//�Ƿ��һ�ε�½������û�б�����Ϣ
#define LOGIN_FIRST				1  //������¼

//������������ַ�������
#define MAX_ALLSTRINGFILTER_LEN		250
//������������ַ�������, ��','�ֿ�
#define MAX_STRINGFILTER_NUM		10
#define MAX_STRINGFILTER_LEN		50
#define MAX_MSG_LEN					910
#define MAX_SID_LEN					60

#define MAX_TIMEZONE_LEN	100
#define MAX_BROWSER_LEN		20
#define MAX_LANGUAGE_LEN	100
#define MAX_VSIZE_LEN		20

#define MAX_SCRIPTFLAG_LEN	40
#define MAX_USERNAME_LEN	60
#define MAX_PASSWORD_LEN	33
#define MAX_STATUS_LEN		60
#define MAX_FILENAME_LEN	256 
#define MAX_PHONE_LEN		20
#define MAX_URL_LEN			256 // 120- 256 ��Щ��ַ���ض� [12/27/2010 SC]
#define MAX_AREA_LEN		50
#define MAX_ZIPCODE_LEN		10 
#define MAX_EMAIL_LEN		120
#define MAX_COMPANY_LEN		40		//��˾
#define MAX_CITY_LEN		30
#define MAX_MSGNICKNAME_LEN	66		// djz ��Ϣ�����ǳƵĳ��� 07.08.7
#define MAX_MSGFONT_LEN		60		// djz ��Ϣ���������ʽ�ַ����ĳ��� 09.05.13
#define MAX_MEMO_LEN		255		//��ע��Ϣ
#define MAX_IPSTRING_LEN	120
#define MAX_IPNUM_LEN		20
#define MAX_RESPONSE_LEN	255
#define MAX_ADDRESS_LEN		60

#define MAX_BANK_LEN		40
#define MAX_BANKNO_LEN		30
#define MAX_TAXNO_LEN		30
#define MAX_USERMEMO_LEN	100  //������Ϣ����
#define MAX_TRADEMEMO_LEN	500  //��˾����
#define MAX_VERIFYMSG_LEN	40	//��֤��Ϣ����

#define MAX_MSG_RECVLEN			8000 //
#define MAX_MSG_SENDLEN			5000 
#define MAX_SMS_LEN			150

#define MAX_FCT_LEN					1000		//���͵��ļ�����У���ĳ���
#define MAX_FDT_LEN					1000		//���͵��ļ�����ûһ�����ĳ���

#define MAX_128_LEN			128 
#define MAX_256_LEN			256 
#define MAX_512_LEN			512 
#define MAX_1024_LEN		1024 
#define MAX_2048_LEN		2048 
#define MAX_4096_LEN		4096
#define MAX_8192_LEN		8192
#define MAX_VISITORID_LEN	60	//�ÿ͵�

#define MAX_FACELINK_LEN	100
#define MAX_GROUPUSER_NUM		128		//����û������
#define MAX_GROUPNAME_LEN	20 

#define MAX_CHATID_LEN		30
#define MAX_WEBCLIENID_LEN	60   //�Ự�зÿ�Ψһ��ʾ��rand����΢��id�Ƚϳ�
#define MAX_THIRDID_LEN     100  //������id����


#define MAX_TITLE_LEN	50 
#define MAX_TAIL_LEN	100 
#define MAX_COLOR_LEN	20 

#define MAX_SEARCHKEYWORDS_NUM	10	//����������ⱨ���ؼ��ʵ������
#define MAX_SEARCHKEYWORDS_LEN	40	//��������ÿһ�����ⱨ���ؼ��ʵĳ���

//userflag��ʾ
#define USERFLAG_ISINVITE			0
#define USERFLAG_ISTALKED			1

//0Ϊ�������õ�ֵ
#define USERAUTH_ROLE_ADMIN			1
#define USERAUTH_ROLE_ADVANCE		2
#define USERAUTH_ROLE_PROJECT		3
#define USERAUTH_ROLE_USER			4

#define USERAUTH_VIEWVISITEROFFLINEMSG		0 //�鿴�ÿ�����
#define USERAUTH_VIEWVISITERHISTORYMSG		1 //�鿴��ʷ�Ի���¼
#define USERAUTH_VIEWOTHERUSERMSG			2 //�鿴�����ͷ��ĶԻ���¼
#define USERAUTH_VIEWTOTAL					3 //�鿴ͳ�Ʒ���
#define USERAUTH_VIEWVISITERINFO			4 //�鿴�ͻ�����, ���Բ鿴�ͻ���������Ϣ
#define USERAUTH_ADDTOOTHERUSERTALK			5 //�����Ŀͷ��ĶԻ�
#define USERAUTH_SETTALKAUTORULE			6 //���öԻ��Զ��������
#define USERAUTH_SORTVISITER				7 //���ÿ�������б�
#define USERAUTH_CHANGEOTHERUSERINFO		8 //�޸ĵ�¼����ֻ�����޸��Լ��ĵ�¼������
#define USERAUTH_CHANGEVISITERNAME			9 //�޸Ŀ�������
#define USERAUTH_DELETEHISTORYMSG			10 //ɾ���Ի���ʷ��¼
#define USERAUTH_DELETEVISITEROFFLINEMSG	11 //ɾ���ÿ�����
#define USERAUTH_ADDUSER					12 //���ò���Ա
#define USERAUTH_SETFLOATADANDINFO			13 //���öԻ��������ݺ͹��
#define USERAUTH_SETFLOATDEFAULTINFO		14 //���ø���ͼ��Ĭ�ϲ���
#define USERAUTH_SETCOMMONKEYWORD			15 //���ù��ÿ�ݻظ�����
#define USERAUTH_SETFLOATTITLE				16 //������ҳ����
#define USERAUTH_SETINVITETYPE				17 //��������������ʽ
#define USERAUTH_OUTHISTORYANDTOTAL			18 //��������ʷ��¼�͵���ͳ�Ʊ���
#define USERAUTH_LOGIN						19 //�����¼

#define USERAUTH_INVITEVISITER				20 //������Ϊ�Զ������ҳ�����ܹ���������ÿ�
#define USERAUTH_VIEWALARM					21 //���ⱨ��
#define USERAUTH_AUTOINVITE					22 //�����Զ�����
#define USERAUTH_OFFLINERESP				23 //�������߻ظ���˵���������������ߺ󣬰��û���Ϣ�����ֻ������ϣ�
#define USERAUTH_SETWELLCOME				24 //����������
#define USERAUTH_SETASKEDQUESTION			25 //���ó�������
#define USERAUTH_BINDIM						26 //QQ��MSN����
#define USERAUTH_FASTREPLY					27 //�޸Ĺ��ÿ�ݻظ�

#define WEBUSERAUTH_FORBID					0 //������

//����������
#define TRACQTYPE_TRACQ		0	//���ڵ�����Ƭ
#define TRACQTYPE_OAM		1	//ר����OA Messenger

#define TRACQTYPE_SP		10	//ר����sp����
#define TRACQTYPE_IT168		11	//ר����it168����
#define TRACQTYPE_QIJI2000	12	//ר����qiji2000����
#define TRACQTYPE_17U		13	//ר����ͬ�̶���
#define TRACQTYPE_UNI		14	//ר���ڶ�����

//��½��Ĵ�������
#define SUBLOGON_NORMAL		0	//����
#define SUBLOGON_UDP		1	//����ͨ��������ȡ
#define SUBLOGON_TCP		2	//ͨ��TCP����ȡ

//Ĭ�ϵ�ͨѶ�˿�
#define EM_PORT						8001
#define EM_DBPORT					8005		//���ݿ���Ϣ���˿�
#define EM_VIDEOPORT				8006		//��ƵͨѶ�˿�
#define PROXY_PORT					80			//����������Ķ˿�
#define PROXY_LOCALPORT				6666		//����������ı��ض˿�
#define SMTP_PORT					25		//smtp port
#define POP_PORT					110		//pop port
#define EMVISITOR_PORT				9005

//����ͨѶ���Ĵ�С
#define PACKMAXLEN					8192//
#define PACKNORMALLEN				8192//
//����Ĭ�ϵ��ַ����Ĵ�С
#define BUFFMAXLEN					1024   //1.k

#define SYS_FAIL					0		// ʧ��
#define SYS_ERROR_MEMORY			100000
#define SYS_ERROR_BEFORE_LOGIN		100001	// �����˵�¼����ܷ��͵���Ϣ����
#define SYS_ERROR_PARAMETER			100002	// ��������

#define COMM_ERROR_SEG				400000
#define COMM_DENYLOGIN_SEG			410000

#define SYS_ERROR_SOCKETCREATEFAIL	800001
#define SYS_ERROR_SOCKETCONNECTFAIL	800002
#define SYS_ERROR_PACKHEAD			800003
#define SYS_ERROR_PACKINFO			800004
#define SYS_ERROR_SENDFAIL			800005
#define SYS_ERROR_RECVFAIL			800006
#define SYS_ERROR_INVALIDCMD		800007
#define SYS_ERROR_PACKINVALIDLEN	800008
#define SYS_ERROR_UNPACKHEAD		800009
#define SYS_ERROR_UNPACKINFO		800010
#define SYS_ERROR_SOCKETCLOSE		800011
#define SYS_ERROR_TIMEOUT			800012
#define SYS_ERROR_OPENFILE			800013
#define SYS_ERROR_READFILE			800014
#define SYS_ERROR_INVALIDAUTH		800015

//����ͽ�������еĴ���
#define PACK_SUCCESS				0	//û�д���
#define PACK_ERROR_SLOPOVER			1	//ָ��Խ��
#define PACK_ERROR_NULLSTRING		2	//���ַ���
#define PACK_ERROR_PACKTOOLARGE		3	//��̫��
#define PACK_ERROR_STRINGTOOLONG	4	//�ݰ�ʱ�ַ����������Ĵ�С

//����ͽ�������еľ�����Ϣ
#define PACK_WARNING_STRINGTOOLONG	101	//�ַ�������


//2015��10��4�� 4216->6001
#define VERSION						6010 
#define VISITOR_VERSION				3006 

//�û���Ϣ��״̬λ��������״̬������
#define USER_AUTH_VERIFY_ADDFRD		0
#define USER_AUTH_SENDSMS			1
#define USER_AUTH_SENDSMS_LEN		2
#define USER_AUTH_MUTISEND			3
#define USER_AUTH_MUTISEND_LEN		2
#define USER_AUTH_GROUP				5
#define USER_AUTH_PROJECT			6
#define USER_AUTH_DENYADD			7		//��ֹ�����˼���Ϊ��ϵ��
#define USER_AUTH_FILEBYSERVER		8
#define USER_AUTH_VIDEOBYSERVER		9
#define USER_AUTH_WEBCQ				10
#define USER_AUTH_COMMOPEN			12
#define USER_AUTH_COMMOPEN_LEN		4
#define USER_AUTH_TRADEOPEN			16
#define USER_AUTH_TRADEOPEN_LEN		4

#define USER_AUTH_HIDELOGON			20
#define USER_AUTH_HIDELOGON_LEN		1
#define USER_AUTH_HIDEINFO			21
#define USER_AUTH_HIDEINFO_LEN		1
#define USER_AUTH_SENDBYSERVER		22
#define USER_AUTH_SENDBYSERVER_LEN	1

#define USER_AUTH_MEETINGCREATE		23
#define USER_AUTH_MEETINGCREATE_LEN	1

#define USER_AUTH_FORBIDCHANGEPASS		25
#define USER_AUTH_FORBIDCHANGEPASS_LEN	1
#define USER_AUTH_FORBIDCHANGEINFO		26
#define USER_AUTH_FORBIDCHANGEINFO_LEN	1

#define USER_AUTH_ICONNO			24
#define USER_AUTH_ICONNO_LEN		8

#define TRADE_AUTH_TRACARD			0
#define TRADE_AUTH_SEARCH			1
#define TRADE_AUTH_FACE				12
#define TRADE_AUTH_FACE_LEN			4
#define TRADE_AUTH_GROUPNO			16
#define TRADE_AUTH_GROUPNO_LEN		8

#define ONLINE_CONNECTTYPE		0
#define ONLINE_CONNECTTYPE_LEN	4
#define ONLINE_USERTYPE			4
#define ONLINE_USERTYPE_LEN		2
#define ONLINE_STATUS			8
#define ONLINE_STATUS_LEN		4
#define ONLINE_VIDEO			16
#define ONLINE_VIDEO_LEN		1

//�������ϵ����
#define UPDATE_STATUS			0	//����״̬
#define UPDATE_PASS				1	//��������
#define UPDATE_VIDEO			2	//������Ƶ
#define UPDATE_LOGINSTRINGID	3	//���µ�¼����
#define UPDATE_CONNECTTYPE		4	//��������	
#define UPDATE_USERTYPE			5	//�û�����
#define UPDATE_USERINFO			6	//�����û���Ϣ
#define UPDATE_TRADEINFO		7	//����������Ϣ
#define UPDATE_ALLINFO			8	//�����û���Ϣ��������Ϣ�� ����������
#define UPDATE_COMPANYNEWS		9	//��������
#define UPDATE_ALLFLAG			10	//�������еı�־λ��Ϣ
#define UPDATE_COMMFLAG			11	//���±�־λ��Ϣ
#define UPDATE_EXTFLAG			12	//������չ��־λ��Ϣ
#define UPDATE_ONLINEFLAG		13	//�������߱�־λ��Ϣ
#define UPDATE_USERIMAGE		14	//�����û�ͼƬ
#define UPDATE_GROUP			15	//�����û����ѷ�����Ϣ


//���Զ�������
#define LANGUAGE_NORMAL			0
#define LANGUAGE_GB				1
#define LANGUAGE_BIG5			2
#define LANGUAGE_EN				3
#define LANGUAGE_UTF8			4

//����ĳһ�����������
#define INFO_USER		0
#define INFO_DEPT		1
#define INFO_BOARD		2
#define INFO_GROUP		3
#define INFO_MEETING	4

#define INFO_MOBILE		5

//���ؽ�����������
#define PROGRESS_HORZ		0
#define PROGRESS_VERT		1

//emmis���ڵ�����
#define EMMISWND_NORMAL				0
#define EMMISWND_GROUPTITLE			1
#define EMMISWND_GROUP				2

#define EMMISWND_DISPLAYNORMAL		0
#define EMMISWND_DISPLAYLINE		1
#define EMMISWND_DISPLAYLIST		2
#define EMMISWND_DISPLAYEDIT		3


#define XPLISTCTRL_TYPE					0
#define XPLISTCTRL_LISTALLPATHFILE		1
#define XPLISTCTRL_LISTPATHFILE			2

#define CLIP_NULL	0
#define CLIP_COPY	1
#define CLIP_CUT	2

//��Ϣ�ɶ���
#define MSGRESP_NO			0
#define MSGRESP_NORMAL		1

#define SRVURL_COMP			0  //��˾����ҳ
#define SRVURL_DOWN			1  //���ص�ַ

#define LCSB_CLIENTDATA				1
#define LCSB_NCOVERRIDE				2

//XP back bmp size
#define	XPBUTTON_WIDTH			38
#define	XPBUTTON_HEIGHT			19

//Tab back bmp size
#define	TABWND_WIDTH			44
#define	TABWND_HEIGHT			15

//�ؼ�������״̬
#define	CTRL_NORMAL			0
#define	CTRL_FOCUS			1
#define	CTRL_DOWN			2
#define	CTRL_DISABLE		3
#define	CTRL_DEACTIVE		4
#define	CTRL_SELECT			5

//dialogbar������
#define	DIALOGBAR_NORMAL			0
#define	DIALOGBAR_SEARCHBAR			1


//��ť�Ĵ�С���໥�ļ��
#define	WND_BUTTON_H				19
#define	WND_BUTTON_W				19
#define	WND_BUTTON_D				3

//���������
#define	PACKSOLVE_SUCCESS			0
#define	PACKSOLVE_NULL				1
#define	PACKSOLVE_UNPACKERROR		2

typedef enum{
	VISITORTALK_TOOLBAR_SPLITTER,		
		VISITORTALK_TOOLBAR_SORT,		//�����ѯ
		VISITORTALK_TOOLBAR_HISTORY,	//�Ի���¼
		VISITORTALK_TOOLBAR_UNON,		//�����˶Ի�
		VISITORTALK_TOOLBAR_SEARCH,		//��������
		VISITORTALK_TOOLBAR_FRD,		//��������
		VISITORTALK_TOOLBAR_USERDEFINE,	//�Զ����ѯ
		VISITORTALK_TOOLBAR_COLUMSETUP,
		VISITORTALK_TOOLBAR_OUT
}TOOLBAR_ID;

#define STRING_WARN				"����"
#define STRING_ERROR			"����"
#define STRING_QUESTION			"ѯ��"
#define STRING_MSG				"��Ϣ"
#define STRING_SYSMSG			"ϵͳ��Ϣ"
#define STRING_TIPS				"��ʾ"

//ͼ�������
#define	ICON_16					16
#define	ICON_32					32
#define	ICON_48					48

//�����˵������
#define POPUP_NULL				-1
#define POPUP_SENDBTN				0
#define POPUP_SYSTRAY				1
#define POPUP_USERTREEMYSELF		2	//�Լ��Ĳ˵���

#define POPUP_USERTREEUSER			3	//��Ŀ¼���ѵĲ˵���
#define POPUP_USERTREEUSERTALK		4	//�Ի��ĺ�����Ĳ˵���

#define POPUP_USERTREEVISITOR			5	//û�жԻ��ķÿ���
#define POPUP_USERTREEVISITORREQUEST	6	//���룬 ת���еķÿ���
#define POPUP_USERTREEVISITORTALK		7	//�Ի��ķÿ���������롢ת�ӺͶԻ�

#define POPUP_USERTREEBLANK			8
#define POPUP_AUTOINVITE			9
#define POPUP_ALERT					10
#define POPUP_SORTUSER				11
#define POPUP_WNDSETUP				12
#define POPUP_TALKSTATUS_REQUEST    13
#define POPUP_HTMLTOOLBAR		    14

//�������ͣ�Ŀǰ�������ֶ���
#define OBJECT_UNKNOWN		0
#define OBJECT_USER			1
#define OBJECT_WEBUSER		2 // web�û�������΢���û�

//�����ļ�������
#define SOUND_USERONLINE		0
#define SOUND_USERMSG			1
#define SOUND_SYSMSG			2
#define SOUND_CALL				3
#define SOUND_KEYCLICK			4
#define SOUND_BACK				5
#define SOUND_SUCCESS			6
#define SOUND_FAIL				7
#define SOUND_NUDGE				8
#define SOUND_TIPS				9

//ϵͳ���̼��໥֪ͨ��Ϣ
#define EVENT_OFFLINE		0			
#define EVENT_ONLINE		1			
#define EVENT_INSTALLHOOK	2
#define EVENT_UNINSTALLHOOK	3
#define EVENT_DELETETEMP	4
#define EVENT_FINDSRV		5
#define EVENT_RESPSRV		6
#define EVENT_CLOSESYSTEM	10			//�ر�ϵͳ

//���������
#define RECT_DOCKCENTER		0		//��
#define RECT_DOCKLEFT		1		//��
#define RECT_DOCKRIGHT		2		//��
#define RECT_DOCKLEFTRIGHT	3		//����
#define RECT_DOCKTOP		1		//��
#define RECT_DOCKBOTTOM		2		//��
#define RECT_DOCKTOPBOTTOM	3		//����

//�����������
#define TYPE_TEXT			0
#define TYPE_ICON			1
#define TYPE_ICONTEXT		2
#define TYPE_EDITEXT		3

#define TYPE_SPLITTER		4
#define TYPE_MORE			5

#define TRIANGLE_LEFT		1		//��
#define TRIANGLE_RIGHT		2		//��
#define TRIANGLE_TOP		3		//��
#define TRIANGLE_BOTTOM		4		//��

#define LISTUSER_BYSTATUS_ALL		0
#define LISTUSER_BYSTATUS_OFFLINE	1
#define LISTUSER_BYSTATUS_ONLINE	2

#define STRING_HTML_META "<meta http-equiv='Content-Type' content='text/html; charset=gb2312'>"
#define STRING_HTML_BASE "<base target='_blank'>"
#define STRING_HTML_STYLE "<style type='text/css'> <!--td{font-size: 12px;}body{OVERFLOW: auto;margin: 5px; padding: 0px 0px 0px 0px; border: 0px;font-family:����, sans-serif; font-size: 12px;}p { margin-top:5px;margin-bottom:0px; font-size: 10pt;line-height:17px;}font{line-height:110%;}li { margin-top:5px;margin-bottom:0px; font-size: 10pt;}.sysfont{ font-size: 11pt;color: Salmon;}.operatornamefont{ font-size: 11pt;color: Blue;}.operatorfont{ font-size: 11pt;color: Black;padding-left:8px;}.clientnamefont{ font-size: 11pt;color: Black;}.clientnamefont1{ font-size: 11pt;color: Green; font-weight:bold;}.clientfont{ font-size: 11pt;color: black;padding-left:8px;}.clientfont1{ font-size: 11pt;color: Purple;}.trackfont{ font-size: 10pt;color: DarkCyan;}--></style>"

#define STRING_HISTORYHTML_HEAD1 "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\r\n<base target='_blank'>\r\n\
<script language=\"javascript\">function Display(obj){sid=obj.parentNode.parentNode.parentNode.childNodes[1];\
if (sid.style.display == 'none'){ sid.style.display='';obj.innerText='������ϸ';}else{ sid.style.display='none';obj.innerText='��ʾ��ϸ';}}\
function SelectTab(selectid,showdiv,showheader,tabcount,isselectbg,isnotselectbg){for(i=0;i<tabcount;i++){if(i==selectid) {document.getElementById(showheader[i]).style.backgroundImage=\"url(\"+isselectbg+\")\";document.getElementById(showdiv[i]).style.display=\"\";}else{document.getElementById(showheader[i]).style.backgroundImage=\"url(\"+isnotselectbg+\")\";document.getElementById(showdiv[i]).style.display=\"none\";}}}\
function ChangeTab(selectid,n){var showdiv=new Array(\"rdcxtable_a_\"+n,\"rdcxtable_b_\"+n,\"rdcxtable_c_\"+n,\"rdcxtable_d_\"+n);var showheader=new Array(\"rdcxtd_a_\"+n,\"rdcxtd_b_\"+n,\"rdcxtd_c_\"+n,\"rdcxtd_d_\"+n);"


#define STRING_HISTORYHTML_HEAD2 "var tabcount=4;SelectTab(selectid,showdiv,showheader,tabcount,isselectbg,isnotselectbg);}\
function killErrors(){return true;}window.onerror = killErrors;</script>\
<style type=\"text/css\"> <!--td{font-size: 12px;}\
body{OVERFLOW: auto;margin: 5px; padding: 0px 0px 0px 0px; border: 0px;font-family:����, Arial, Helvetica, sans-serif; font-size: 12px;}\
p { margin-top:5px;margin-bottom:0px; font-size: 10pt;line-height:140%;}\
li { margin-top:5px;margin-bottom:0px; font-size: 10pt;}\
.sysfont{ font-size: 11pt;color: Salmon;}\
.operatornamefont{ font-size: 11pt;color: Blue;}\
.operatorfont{ font-size: 11pt;color: Black;}\
.clientnamefont{ font-size: 11pt;color: Black;}\
.clientnamefont1{ font-family: ����;font-size: 11pt;color: Black;}\
.clientfont{ font-size: 11pt;color: black;}\
.clientfont1{ font-size: 11pt;color: Purple;}\
.trackfont{ font-size: 10pt;color: DarkCyan;}--></style></head><body></body></html>"

#define GETINFO_COMING "���ڻ�ȡ���ݣ���ȴ�..."
#define GETINFO_FAIL "�޷���ȡ���ݣ�"
#define GETINFO_SUCC "�ɹ���ȡ���ݣ�"

//ѡ�еĶ���༭����
#define INPLACE_NOEDIT		0
#define INPLACE_EDIT		1
#define INPLACE_PASS		2
#define INPLACE_COMBOX		3
#define INPLACE_DATE		4
#define INPLACE_TIME		5
#define INPLACE_DATETIME	6

#define IDC_INPLACE_COMBOX_EDIT         1002

#define MAX_VISITORSORT_NUM	15
#define MAX_ICONINDEX1STRING_NUM	13

static const char *SORT_VISITOR[] = {"����ɫ���","���ɫ���","Ы�Ϻ�ɫ���","��ɫ���","��ɫ���",\
"��ɫ���","��ɫ���","���ɫ���","��ɫ���","���ɫ���",\
"�����ɫ���","����Ϻ�ɫ���","���ɫ���","���ɫ���","����ɫ���",\
"����ɫ���","�����ɫ���","����ɫ���","���Һ�ɫ���","����ɫ���"};

static const char *ICONINDEX1_STRING[] = {"����", "�����", "�ѳɽ�", "�ط�","��㿴��", \
"ͬ��", "ɧ��", "������", "��η���", "��ȷ���", \
"��������", "Ͷ�߽���", "����"};

#define ACCERESP_TYPE_TXT				0
#define ACCERESP_TYPE_URL				1
#define ACCERESP_TYPE_FILE				2

//�ϴ��ļ�����
#define SENDFILE_FILE		0
#define SENDFILE_IMAGE		1
#define SENDFILE_ACCELRESPFILE		2
#define SENDFILE_UPUSERFILE			3
#define SENDFILE_ICONFILE			4


#define APPLY_UNKNOWN	-1	//���󵯳�ѯ�ʣ�
#define APPLY_ASK		0	//���󵯳�ѯ�ʣ�
#define APPLY_OPEN		1	//ֱ������Ի���
#define MYGETNOTE		2	//�����û�����


//��������
#define	REQUSTTYPE_NORMAL					0
#define	REQUSTTYPE_VISITOR					1
#define	REQUSTTYPE_USER						2
#define	REQUSTTYPE_DIRECT					3	//ֱ�ӶԻ�

#define	INVITE_TYPE_NORMAL					0	//��������
#define	INVITE_TYPE_WELLCOME				1	//���ʺ�������
#define	INVITE_TYPE_DIRECTTALK				2	//ֱ�ӶԻ�

#define INVITE_TRANSFER			10	//ת���û�
#define INVITE_HELP				11	//����Э��
#define INVITE_HELP_REFUSE		12	//����Э���ܾ�
#define INVITE_HELP_ACCEPT		13	//����Э��ͬ��

#define MAX_UIN_USER    2300000000
#define WEBUSER_UIN     2200000000U
#define CLIENT_UIN      1000000000U

#define CHATCLOSE_OFFLINE	0		// ���ߵ���
#define CHATCLOSE_TIMEOUT	1		// ��ʱ����
#define CHATCLOSE_USER		2		// ��ϯ�����ر�
#define CHATCLOSE_GP		3		// GP�������ر�
#define CHATCLOSE_WEBUI		4		// WEBUI�������ر�
#define CHATCLOSE_UNSUBSCRIBE 6		// ΢���û�ȡ����ע�رջỰ
#define CHATCLOSE_INVISTEXIT 10		// �Ự����������ϯ���룬����ϯ�����ر�

#define INVITE_NORMAL		0
#define INVITE_USERDEFINE	1
#define INVITE_DIRECTTALK	2
#define INVITE_OFFLINEMSG	3


#define MAX_FILEDS	24
#define NEED_PARSE_ITEM 8
#define VISITORTALK_MAXLIST_TOTALURL		10
typedef std::list<void *>	PtrList;

#define USER_REPORT_COUNT	10	//���ɶ���10���Զ����ѯ
#define USER_REPORT_MAXID	0xFF
#define USER_REPORT_MINID	(USER_REPORT_MAXID-10)	
#define USER_REPORT_MENUID	(USER_REPORT_MAXID-11)	//�Զ����ѯ�˵�ID
#define	INTERNAL_REPORT_HISTORY		(USER_REPORT_MAXID-12)//�Ի���¼
#define INTERNAL_REPORT_UNON		(USER_REPORT_MAXID-13)//�����˶Ի�
#define INTERNAL_REPORT_SEARCH		(USER_REPORT_MAXID-14)//��������
#define INTERNAL_REPORT_FRD			(USER_REPORT_MAXID-15)//��������

/** http����ʽ */
#define REQUEST_TYPE_GET 1	/**< get��ʽ */ 
#define REQUEST_TYPE_POST 2	/**< post��ʽ */ 


/** webҳ���ʾ�� */
#define Handler_WebUrl "WebUrl"	// ��ʷ��¼��ҳ��
#define Handler_ListMsg "ListMsg" // ����������ʾҳ��
#define Handler_WebAdmin "WebAdmin" // ��������ҳ��
#define Handler_DisplayIamge "DisplayIamge" // �鿴��ͼ
#define Handler_VisitorRelated "VisitorRelated" // �ÿ����,�����ÿ���Ϣ���ÿͶ�������ѯ�������Ự�ǵ���������Ϣ������Ӧ��

/** Js��C++������ */
#define Js_Call_MFC_Func_RestartSession "RestartSession"	// �������λỰ
#define Js_Call_MFC_Func_StartRecord "StartRecord" // ����¼��
#define Js_Call_MFC_Func_SendAudio "SendAudio" // ����¼��
#define Js_Call_MFC_Func_CancelRecord "CancelRecord" // ȡ��¼��
#define Js_Call_MFC_Func_ViewDetails "ViewDetails"  // �鿴��ͼ
#define Js_Call_MFC_Func_ReSendFile "ReSendFile" // ���·����ļ�
#define Js_Call_MFC_Func_ReRecvFile "ReRecvFile" // ���½����ļ�
#define Js_Call_MFC_Func_ChangeChatObject "ChangeChatObject" // �л��������
#define Js_Call_MFC_Func_ReSendMsg "ReSendMsg" // ���·���������Ϣ��json����Ϣ

/** Format�����url�������� */
#define Request_Url_RestartSession "RestartSession"	// �������λỰ
#define Request_Url_AccessToken "AccessToken" // ��ȡ΢�Ź��ں�token


// ¼�������ķ�����
#define CODE_AUDIO_SUCCESS 0 // �ɹ�
#define CODE_AUDIO_NO_DEVICE 1 // û��¼���豸
#define CODE_AUDIO_FAIL 2 // ʧ��

#define FACE_PREFIX "<IMG src=\""
#define FACE_ALT ".gif\" alt=\""
#define FACE_SUFFIX "\" />"

#define SRVNORMAL_IPERROR       3

#endif
