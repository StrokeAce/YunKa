#ifndef _WX_OBJ_H_
#define _WX_OBJ_H_

#include "../json/jsonext.h"
#include "comdef.h"


struct WxLocation
{
	string Latitude;	//  地理位置纬度  
	string Longitude;	//  地理位置经度  
	string Precision;	//  地理位置精度  
	string label;		//  地理位置标注  

	bool empty()
	{
		return Latitude.empty()&&Longitude.empty()&&Precision.empty();
	}

	bool operator !=(const WxLocation& r)
	{
		if (Latitude==r.Latitude&&Longitude==r.Longitude&&Precision==r.Precision)
		{
			return true;
		}
		return false;
	}
};

struct WxObj
{
	WxObj(const string& sztype) :MsgType(sztype), CreateTime(0){ LanguageType = LANGUAGE_NORMAL; }
	virtual ~WxObj(){}
	string MsgType;			// text  
	string ToUserName;		// 开发者微信号  
	string FromUserName;	// 发送方帐号（一个OpenID）  
	int LanguageType;
	string AutoAnswerMsg;	// 服务器自动回复的消息，消息格式json字符串

	unsigned int CreateTime;							//消息创建时间 （整型）  
	virtual bool ParseFromJson(const Json::Value &os);  //从json格式文本中解析消息
	virtual bool ToSendJson(Json::Value &os) const;     //产生客服发送消息

};

struct WxUserInfo:public WxObj
{
	WxUserInfo():WxObj("userinfo"),subscribe(0),sex(0){
		FromUserName="webui";
	}
	
	void Clear()
	{
		subscribe=sex=0;
		subscribe_time=0;
		openid=nickname=city=country=province=language=headimgurl=unionid="";
	}

	short subscribe;		//用户是否订阅该公众号标识，值为0时，代表此用户没有关注该公众号，拉取不到其余信息。
	short sex;				//用户的性别，值为1时是男性，值为2时是女性，值为0时是未知
	string openid;			//用户的标识，对当前公众号唯一,加密后的微信号，fromusername
	string nickname;		//用户的昵称
	string remark;			//用户的昵称
	string city;			//用户所在城市
	string country;			//用户所在国家
	string province;		//用户所在省份
	string language;		//用户的语言，简体中文为zh_CN
	string headimgurl;		//用户头像，最后一个数值代表正方形头像大小（有0、46、64、96、132数值可选，0代表640*640正方形头像），用户没有头像时该项为空
	time_t subscribe_time;  //用户关注时间，为时间戳。如果用户曾多次关注，则取最后关注时间
	string unionid;			//只有在用户将公众号绑定到微信开放平台帐号后，才会出现该字段。详见：获取用户个人信息（UnionID机制）
	string fromwxname;      //来源公众号后台配置的名称
	unsigned long long ullCometimes;  //来访次数，以会话、留言为准



	virtual bool ToSendJson(Json::Value &jv) const;     //产生json格式发送消息
	virtual bool ParseFromJson(const Json::Value &jv);     //从json格式文本中解析消息

};

struct WxAccessTokenInfo :public WxObj
{
	WxAccessTokenInfo()
	:WxObj("wxactoken")
	{
		expires_in = 0;
		FromUserName = "webui";
	}


	int expires_in;			//超时时长
	string szAcToken;		//token
	string szWxBaseUrl;		//微信公众号地址


	virtual bool ToSendJson(Json::Value &jv) const;			//产生json格式发送消息
	virtual bool ParseFromJson(const Json::Value &jv);		//从json格式文本中解析消息
};

struct WxMsgBase:public WxObj
{
	WxMsgBase(const string& sztype):WxObj(sztype){}

	ULONGLONG MsgId;	//消息id，64位整型  
	virtual bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	virtual bool ToSendJson(Json::Value &os) const;     //产生客服发送消息

};

struct WxMsgText:public WxMsgBase
{
	WxMsgText(const string& sztype):WxMsgBase(sztype){
		assert("text"==sztype);
	}

	string Content; //消息内容
	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;	    //兼容老的客户端的文本消息  

};

struct WxMsgImage:public WxMsgBase
{
	WxMsgImage(const string& sztype):WxMsgBase(sztype){
		assert("image"==sztype);
	}

	string PicUrl;		//  图片链接  
	string MediaId;		//  图片消息媒体id，可以调用多媒体文件下载接口拉取数据。
	string MediaUrl;	//MediaId媒体对应的url
	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;
	    //兼容老的客户端的文本消息  

};

struct WxMsgVoice:public WxMsgBase
{
	WxMsgVoice(const string& sztype)
		:WxMsgBase(sztype){
		assert("voice"==sztype);
	}

	string Format;//   语音格式，如amr，speex等    
	string MediaId;//  语音消息媒体id，可以调用多媒体文件下载接口拉取数据。 
	string MediaUrl;        //媒体对应的url

	string Recognition;//开通语音识别功能，用户每次发送语音给公众号时，微信会在推送的语音消息XML数据包中，增加一个Recongnition字段。UTF8编码

	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;
	

};

struct WxMsgVideo :public WxMsgBase
{
	WxMsgVideo(const string& sztype)
		:WxMsgBase(sztype){
			assert("video"==sztype||"shortvideo"==sztype);
		}

	string ThumbMediaId;		//   视频消息缩略图的媒体id，可以调用多媒体文件下载接口拉取数据。 
	string ThumbMediaUrl;       //媒体对应的url
	string MediaId;				// 视频消息媒体id，可以调用多媒体文件下载接口拉取数据。 
	string MediaUrl;			//媒体对应的url

	//只用于向微信服务器发送消息
	string Title;				// 视频消息的标题  
	string Description;			//视频消息的描述 

	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;
	

};

struct WxMsgLocation:public WxMsgBase
{
	WxMsgLocation(const string& sztype)
		:WxMsgBase(sztype){
			assert("location"==sztype);
		}


	string Location_X;	// 地理位置维度  
	string 	Location_Y;	//  地理位置经度  
	string 	Scale;		//  地图缩放大小  
	string 	Label;		//  地理位置信息  

	bool ToSendJson(Json::Value &os) const;
	
	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息
};


/*
目前微信不会推送该类消息过来
坐席可以发送该类消息
*/
struct WxMsgLink:public WxMsgBase
{
	WxMsgLink(const string& sztype)
		:WxMsgBase(sztype){
			assert("link"==sztype);
		}

	string Title;		//  消息标题  
	string Description;	//  消息描述  
	string Url;			//  消息链接  
	bool ToSendJson(Json::Value &os) const;
};

/*
目前微信不会推送该类消息过来
坐席可以发送该类消息
*/
struct WxMsgMusic:public WxObj
{
	WxMsgMusic(const string& sztype)
		:WxObj(sztype){
			assert("music"==sztype);
		}

	string ThumbMediaId;//  是  缩略图的媒体id，通过上传多媒体文件，得到的id  

	string Title;  //否  音乐标题  
	string Description; // 否  音乐描述  
	string MusicURL; // 否  音乐链接  
	string HQMusicUrl;//  否  高质量音乐链接，WIFI环境优先使用该链接播放音乐  

	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToResponseXml(ostringstream &os) const;
	bool ToSendJson(Json::Value &os) const;
	

};

struct WxNewsItem
{
	string Title;  //  否  图文消息标题  
	string Description;  //  否  图文消息描述  
	string PicUrl;  //  否  图片链接，支持JPG、PNG格式，较好的效果为大图360*200，小图200*200  
	string Url;  //  否  点击图文消息跳转链接  
	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息
	bool ToSendJson(Json::Value &os) const;
	


};
struct WxMsgNews:public WxMsgBase
{
	WxMsgNews(const string& sztype)
		:WxMsgBase(sztype){
			assert("news"==sztype);
		}

	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;
	

	vector<WxNewsItem> News;
	bool AddArticle(const WxNewsItem &a);


};

////////////////////////////wxevent接口//////////////////////////////////////////////

/*
关注/取消关注事件
<xml>
<ToUserName><![CDATA[toUser]]></ToUserName>
<FromUserName><![CDATA[FromUser]]></FromUserName>
<CreateTime>123456789</CreateTime>
<MsgType><![CDATA[event]]></MsgType>
<Event><![CDATA[subscribe]]></Event>
</xml>

MsgType:event
*/
struct WxEventBase:public WxObj
{
	WxEventBase(const string& sztype):WxObj(sztype){}

	string Event; 
	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;

};

/*
Event: subscribe(订阅)、unsubscribe(取消订阅),SCAN(用户已关注时的事件推送 )
只提供给客户端使用，微信服务器不接受该类消息
*/
struct WxEventSubscribe:public WxEventBase
{
	WxEventSubscribe(const string& sztype):WxEventBase(sztype){}

	/*
	Event为subscribe 时：	事件KEY值，qrscene_为前缀，后面为二维码的参数值 
	Event为SCAN 时     ：	事件KEY值，是一个32位无符号整数，即创建二维码时的二维码scene_id 
	*/
	string EventKey;

	string Ticket;		//二维码的ticket，可用来换取二维码图片 


	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;

	

};

/*
上报地理位置事件
通了上报地理位置接口的公众号，用户在关注后进入公众号会话时，会弹框让用户确认是否允许公众号使用其地理位置。弹框只在关注后出现一次，用户以后可以在公众号详情页面进行操作。 
用户同意上报地理位置后，每次进入公众号会话时，都会在进入时上报地理位置，上报地理位置以推送XML数据包到开发者填写的URL来实现。 
Event: LOCATION
*/
struct WxEventLocation:public WxEventBase
{
	WxEventLocation(const string& sztype):WxEventBase(sztype){}


	WxLocation  local;

	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;
	
};

/*
Event: CLICK 点击菜单拉取消息时的事件推送 
Event: VIEW 点击菜单跳转链接时的事件推送 
*/
struct WxEventMenu:public WxEventBase
{
	WxEventMenu(const string& sztype):WxEventBase(sztype){}
	/*
	CLICK:事件KEY值，与自定义菜单接口中KEY值对应 
	VIEW:事件KEY值，设置的跳转URL 
	*/
	string EventKey ;
	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息

	bool ToSendJson(Json::Value &os) const;
};


/*事件推送群发结果
<xml>
<ToUserName><![CDATA[gh_3e8adccde292]]>< / ToUserName>
<FromUserName><![CDATA[oR5Gjjl_eiZoUpGozMo7dbBJ362A]]>< / FromUserName>
< CreateTime>1394524295 < / CreateTime >
<MsgType><![CDATA[event]]>< / MsgType>
<Event><![CDATA[MASSSENDJOBFINISH]]>< / Event>
< MsgID>1988 < / MsgID >
<Status><![CDATA[sendsuccess]]>< / Status>
< TotalCount>100 < / TotalCount >
< FilterCount>80 < / FilterCount >
< SentCount>75 < / SentCount >
< ErrorCount>5 < / ErrorCount >
< / xml>

群发的结构，为“send success”或“send fail”或“err(num)”。但send success时，也有可能因用户拒收公众号的消息、系统错误等原因造成少量用户接收失败。
err(num)是审核失败的具体原因，可能的情况如下：
err(10001),涉嫌广告
err(20001), 涉嫌政治
err(20004), 涉嫌社会
err(20002), 涉嫌色情
err(20006), 涉嫌违法犯罪
err(20008),涉嫌欺诈
err(20013),涉嫌版权
err(22000), 涉嫌互推(互相宣传)
err(21000), //涉嫌其他
*/
struct WxEventMasssendjobfinish :public WxEventBase
{
	WxEventMasssendjobfinish(const string& sztype) :WxEventBase(sztype) {}


	unsigned long long MsgID;		//群发的消息ID
	unsigned long long TotalCount;	//group_id下粉丝数；或者openid_list中的粉丝数
	unsigned long long FilterCount; // 过滤（过滤是指，有些用户在微信设置不接收该公众号的消息）后，准备发送的粉丝数，原则上，FilterCount = SentCount + ErrorCount
	unsigned long long SentCount; //发送成功的粉丝数
	unsigned long long ErrorCount; //发送失败的粉丝数


	bool ParseFromJson(const Json::Value &os);   //从json格式文本中解析消息
};
//////////////////////////////////////////////////////////////////////////

struct GroupInfo
{
	int id;			//  分组id，由微信分配  
	string	name;	//  分组名字，UTF8编码  
    int count;		//  分组内用户数量  
};

#endif