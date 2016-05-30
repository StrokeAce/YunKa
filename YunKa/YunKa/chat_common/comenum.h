#ifndef _COM_ENUM_H_
#define _COM_ENUM_H_

// �Ự�رյ�ԭ��
enum CLOSE_CHAT_REASON
{
	REASON_TIME_OUT,			// �ͷ���ʱ��û��Ӧ��ʱ��
	REASON_CLIENT_OFFLINE,		// �ͷ�������
	REASON_CLIENT_RELEASE		// �ͷ������ͷ��˿ͻ�
};

// �ÿ͵ĶԻ�״̬
enum TALKSTATUS
{
	TALKSTATUS_NO,			// û�жԻ�
	TALKSTATUS_AUTOINVITE,	// �Զ�������
	TALKSTATUS_REQUEST,		// ������
	TALKSTATUS_REQUESTRESP,	// ����ظ��У���ʾ����
	TALKSTATUS_INVITE,		// ������
	TALKSTATUS_INVITERESP,	// �ظ��У���ʾ����
	TALKSTATUS_TRANSFER,	// ת����
	TALKSTATUS_TRANSFERRESP,// ת����, ��ʾ����
	TALKSTATUS_WELLCOME,	// ��������wellcome�Ի���Ϣ
	TALKSTATUS_TALK,		// �Ի���
};

enum USER_STATUS
{
	STATUS_UNDEFINE=-1,
	STATUS_USERDEFINE,
	STATUS_OFFLINE,
	STATUS_ONLINE,
	STATUS_HIDE,
	STATUS_WORK,
	STATUS_EATING,
	STATUS_LEAVE,
	STATUS_ROBOT,
	STATUS_SEND_SMS,
	STATUS_SEND_MAIL,
	STATUS_BUSY,
	STATUS_SEND_MSG=12,
	STATUS_REFUSE_NEWWEBMSG,
	STATUS_UNKNOWN   //�û�״̬δ֪
};

/** ������Ϣ��Դ�û����� */
enum MSG_FROM_TYPE
{
	MSG_FROM_CLIENT=1,	// ��ϯ�û�
	MSG_FROM_USER,		// ΢�Ż�web�û�
	MSG_FROM_ASSIST,	// Э������
	MSG_FROM_SYS		// ϵͳ��ʾ��Ϣ
};

/** ������Ϣ�����û����� */
enum USER_TYPE
{
	USER_TYPE_CLIENT=1,	// ��ϯ�û�
	USER_TYPE_WX,		// ΢���û�
	USER_TYPE_WEB		// ��ҳ�û�
};

/** ��Ϣ�������� */
enum MSG_TYPE
{
	MSG_TYPE_NORMAL=2,		/**< ��ͨ��Ϣ */
	MSG_TYPE_PREV			/**< Ԥ֪��Ϣ */
};

/** ��Ϣ�������� */
enum MSG_DATA_TYPE
{
	MSG_DATA_TYPE_TEXT=1,		/**< ���� */
	MSG_DATA_TYPE_IMAGE,	/**< ͼƬ */
	MSG_DATA_TYPE_VOICE,	/**< ���� */
	MSG_DATA_TYPE_VIDEO,	/**< ��Ƶ */
	MSG_DATA_TYPE_LOCATION,	/**< λ�� */
	MSG_DATA_TYPE_LINK,		/**< ���� */
	MSG_DATA_TYPE_EVENT,	/**< �¼� */
	MSG_DATA_TYPE_FILE		/**< �ļ� */
};

#endif