#if !defined(AUTHERROR_H_)
#define AUTHERROR_H_

#define  SESSION_ACTION_SUCCESS 1    //1:成功
static char   szSESSION_ACTION_SUCCESS[]="成功";

#define  SESSION_ACTION_ERR_PASS 0    //0: 密码不对
static char   szSESSION_ACTION_ERR_PASS []="用户密码不对";

#define  SESSION_ACTION_ERR_TOKEN -1    //-1：token不对
static char   szSESSION_ACTION_ERR_TOKEN[]="token不对";

#define  SESSION_ACTION_ERR_UIN  -2     //-2： uin不合法
static char   szSESSION_ACTION_ERR_UIN []="用户号码 不合法";

#define  SESSION_ACTION_ERR_TOKENLEN -3    //-3：token长度不对
static char   szSESSION_ACTION_ERR_TOKENLEN[]="token长度不对";

#define  SESSION_ACTION_ERR_SERVER  -4     //-4：服务器内部错误
static char   szSESSION_ACTION_ERR_SERVER []="服务器内部错误";

#define  SESSION_ACTION_ERR_NOUSER  -5     //-5：用户不存在
static char   szSESSION_ACTION_ERR_NOUSER []="用户不存在";

#define  SESSION_ACTION_ERR_NORIGHT  -8     //-8：用户名和TQ号同时为空了
static char   szSESSION_ACTION_ERR_NORIGHT[]="用户名和用户号同时为空了";

#define  SESSION_ACTION_ERR_TYPE    -14     //-14：认证类型不对
static char   szSESSION_ACTION_ERR_TYPE[]="认证类型不正确";

#define  SESSION_ACTION_ERR_TIME    -15     //-15：时间错误
static char   szSESSION_ACTION_ERR_TIME[]="本地时间错误";

#define  SESSION_ACTION_ERR_SOURCEIP    -16       //-16：来源IP错误
static char   szSESSION_ACTION_ERR_SOURCEIP []="认证来源IP错误";

#define  SESSION_ACTION_ERR_GETADMIN    -17       //-17：获取管理员号码错误
static char   szSESSION_ACTION_ERR_GETADMIN []="获取管理员号码错误";

#define  SESSION_ACTION_ERR_NOTCONNECT    -100       //-100连接认证服务器失败
static char   szSESSION_ACTION_ERR_NOTCONNECT[]="连接认证服务器失败";

static char   szszSESSION_ACTION_ERR_DEFAULT[]="未知错误";


inline const char* GetAuthStrError(int nErrrid)
{
	const char* pe=NULL;
	switch (nErrrid)
	{
	case SESSION_ACTION_SUCCESS:
		pe=szSESSION_ACTION_SUCCESS;
		break;
	case SESSION_ACTION_ERR_PASS:
		pe=szSESSION_ACTION_ERR_PASS;
		break;
	case SESSION_ACTION_ERR_UIN:
		pe=szSESSION_ACTION_ERR_UIN;
		break;
	case SESSION_ACTION_ERR_TOKENLEN:
		pe=szSESSION_ACTION_ERR_TOKENLEN;
		break;
	case SESSION_ACTION_ERR_SERVER:
		pe=szSESSION_ACTION_ERR_SERVER;
		break;
	case SESSION_ACTION_ERR_NOUSER:
		pe=szSESSION_ACTION_ERR_NOUSER;
		break;
	case SESSION_ACTION_ERR_NORIGHT:
		pe=szSESSION_ACTION_ERR_NORIGHT;
		break;
	case SESSION_ACTION_ERR_TYPE:
		pe=szSESSION_ACTION_ERR_TYPE;
		break;
	case SESSION_ACTION_ERR_TIME:
		pe=szSESSION_ACTION_ERR_TIME;
		break;
	case SESSION_ACTION_ERR_SOURCEIP:
		pe=szSESSION_ACTION_ERR_SOURCEIP;
		break;
	case SESSION_ACTION_ERR_GETADMIN:
		pe=szSESSION_ACTION_ERR_GETADMIN;
		break;
	case SESSION_ACTION_ERR_NOTCONNECT:
		pe=szSESSION_ACTION_ERR_NOTCONNECT;
		break;
	default :
		pe=szszSESSION_ACTION_ERR_DEFAULT;
		break;
	}

	return pe;
}


#endif
