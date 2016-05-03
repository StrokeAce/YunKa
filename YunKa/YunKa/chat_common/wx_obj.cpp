#include "wx_obj.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "comfunc.h"
#pragma warning(disable:4786)


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
bool WxUserInfo::ToSendJson(Json::Value &os) const
{
	if (!WxObj::ToSendJson(os))
	{
		return false;
	}

	os["subscribe"]=subscribe;
	if (subscribe)
	{   //如果没有关注一下信息应该为空
		os["sex"]=sex;
		os["openid"]=openid;
		os["nickname"]=nickname;
		os["remark"] = remark;
		os["city"]=city;
		os["country"]=country;
		os["province"]=province;
		os["language"]=language;
		os["headimgurl"]=headimgurl;

		os["subscribe_time"]=(unsigned int)subscribe_time;
		os["unionid"]=unionid;
	}
	os["fromwxname"] = fromwxname;
	char  szLbuf[50] = { 0 };
	sprintf(szLbuf, "%llu", ullCometimes);
	os["cometimes"] = szLbuf;

	return true;

}

bool WxUserInfo::ParseFromJson(const Json::Value &jv)
{
	subscribe=GetIntFromJson(jv,"subscribe");
	sex=GetIntFromJson(jv,"sex");
	openid=GetStrFromJson(jv,"openid");
	nickname=GetStrFromJson(jv,"nickname");
	remark = GetStrFromJson(jv, "remark");
	city=GetStrFromJson(jv,"city");
	country=GetStrFromJson(jv,"country");
	province=GetStrFromJson(jv,"province");
	language=GetStrFromJson(jv,"language");
	headimgurl=GetStrFromJson(jv,"headimgurl");
	unionid=GetStrFromJson(jv,"unionid");

	fromwxname = GetStrFromJson(jv, "fromwxname");
	ullCometimes = GetUllFromJson(jv, "cometimes");


	subscribe_time=GetUintFromJson(jv,"subscribe_time");

	return true;
}


////////////////////////////wxmsg接口//////////////////////////////////////////////


bool WxObj::ToSendJson(Json::Value &os) const
{
	
	if (MsgType.empty()||ToUserName.empty())
	{
		return false;
	}
	os["touser"]=ToUserName;
	os["fromuser"]=FromUserName;  //目前发送消息微信实际上不需要fromuser， 2015年10月6日
	os["msgtype"]=MsgType;

	if (!AutoAnswerMsg.empty())
	{
		os["AutoAnswerMsg"] = AutoAnswerMsg;
	}

	return true;
}
bool WxObj::ParseFromJson(const Json::Value &os)
{
	if (MsgType.empty())
	{
		return false;
	}

	ToUserName=GetStrFromJson(os,"touser");
	FromUserName=GetStrFromJson(os,"fromuser");
	MsgType=GetStrFromJson(os,"msgtype");

	AutoAnswerMsg = GetStrFromJson(os, "AutoAnswerMsg");
	return !MsgType.empty();
}
//////////////////////////////////////////////////////////////////////////

bool WxAccessTokenInfo::ToSendJson(Json::Value &jv) const
{
	if (!WxObj::ToSendJson(jv))
	{
		return false;
	}

	jv["expires_in"] = expires_in;
	jv["actoken"] = szAcToken;
	jv["wxbaseurl"] = szWxBaseUrl;

	return true;
}
bool WxAccessTokenInfo::ParseFromJson(const Json::Value &jv)
{
	expires_in = GetIntFromJson(jv, "expires_in");
	szAcToken = GetStrFromJson(jv, "actoken");
	szWxBaseUrl = GetStrFromJson(jv, "wxbaseurl");

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool WxMsgBase::ParseFromJson(const Json::Value &os)
{
	if (!WxObj::ParseFromJson(os))
	{
		return false;
	}

	MsgId=GetUllFromJson(os,"msgid");

	return true;
}

bool WxMsgBase::ToSendJson(Json::Value &os) const
{
	if (!WxObj::ToSendJson(os)){
		return false;
	}


	 //发送消息微信服务器不需要id,由于json不支持int64 转为16进制字符串处理

	char  szLbuf[50]={0};
	sprintf(szLbuf,"%llu",MsgId);
	os["msgid"]=szLbuf; 
	return true;
}


//////////////////////////////////////////////////////////////////////////

/*
{
	"touser":"OPENID",
		"msgtype":"text",
		"text":
	{
		"content":"Hello World"
	}
}
*/

bool WxMsgText::ParseFromJson(const Json::Value &os)
{
	if (!WxMsgBase::ParseFromJson(os))
	{
		return false;
	}

	Json::Value textNode=os["text"];
	if (textNode != Json::Value::null)
	{
		Content = GetStrFromJson(textNode, "content");
		if (!Content.empty())
			return true;
	}

	return false;
}

bool WxMsgText::ToSendJson(Json::Value &os) const
{
	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}
	Json::Value vtext;
	vtext["content"]=Content;

	os["text"]=vtext;

	return true;
}


bool WxMsgImage::ParseFromJson(const Json::Value &os)
{
	if (!WxMsgBase::ParseFromJson(os))
	{
		return false;
	}

	Json::Value image=os["image"];
	if (image!=Json::Value::null)
	{
		MediaId=GetStrFromJson(image,"media_id");
		PicUrl=GetStrFromJson(image,"picurl");
		MediaUrl = GetStrFromJson(image, "media_url");
		return true;
	}

	return false;
}
/*
{
"touser":"OPENID",
"msgtype":"image",
"image":
{
"media_id":"MEDIA_ID"
}
}
*/

bool WxMsgImage::ToSendJson(Json::Value &os) const
{

	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}
	Json::Value image;

	image["media_id"]=MediaId;
	image["picurl"]=PicUrl;	
	image["media_url"] = MediaUrl;
	os["image"]=image;	
	return true;
}


bool WxMsgVoice::ParseFromJson(const Json::Value &os)
{
	if (!WxMsgBase::ParseFromJson(os))
	{
		return false;
	}

	Json::Value voice=os["voice"];
	if (voice!=Json::Value::null)
	{
		MediaId=GetStrFromJson(voice,"media_id");
		MediaUrl=GetStrFromJson(voice,"media_url");
		return true;
	}

	return false;
}

/*
{
"touser":"OPENID",
"msgtype":"voice",
"voice":
{
"media_id":"MEDIA_ID"
}
}
*/

bool WxMsgVoice::ToSendJson(Json::Value &os) const
{

	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}

	Json::Value voice;
	voice["media_id"]=MediaId;
	voice["media_url"]=MediaUrl;
	os["voice"]=voice;

	return true;
}



/*
{
    "touser":"OPENID",
    "msgtype":"video",
    "video":
    {
      "media_id":"MEDIA_ID",
      "thumb_media_id":"MEDIA_ID",
      "title":"TITLE",
      "description":"DESCRIPTION"
    }
}
*/

bool WxMsgVideo::ParseFromJson(const Json::Value &os)
{
	if (!WxMsgBase::ParseFromJson(os))
	{
		return false;
	}

	Json::Value video=os["video"];
	if (video!=Json::Value::null)
	{
		MediaId=GetStrFromJson(video,"media_id");
		MediaUrl=GetStrFromJson(video,"media_url");
		ThumbMediaId=GetStrFromJson(video,"thumb_media_id");
		ThumbMediaUrl=GetStrFromJson(video,"thumb_media_url");
		Title=GetStrFromJson(video,"title");
		Description=GetStrFromJson(video,"description");

		return true;
	}

	return false;
}

bool WxMsgVideo::ToSendJson(Json::Value &os) const
{

	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}

	Json::Value video;
	video["media_id"]=MediaId;
	video["media_url"]=MediaUrl;

	video["thumb_media_id"]=ThumbMediaId;
	video["thumb_media_url"]=ThumbMediaUrl;

	video["title"]=Title;
	video["description"]=Description;

	os["video"]=video;

	return true;
}



//只提供给tq客户端使用，微信服务器不支持这个消息
bool WxMsgLocation::ToSendJson(Json::Value &os) const
{

	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}

	os["location_x"]=Location_X;
	os["location_y"]=Location_Y;

	os["scale"]=Scale;
	os["label"]=Label;

	return true;
}

bool WxMsgLocation::ParseFromJson(const Json::Value &os)
{
	if (!WxMsgBase::ParseFromJson(os)){
		return false;
	}

	Location_X = GetStrFromJson(os, "location_x");
	Location_Y = GetStrFromJson(os, "location_y");
	Scale = GetStrFromJson(os, "scale");
	Label = GetStrFromJson(os, "label");

	return true;
}


//只提供给tq客户端使用，微信服务器不支持这个消息
bool WxMsgLink::ToSendJson(Json::Value &os) const
{

	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}

	os["title"]=Title;
	os["description"]=Description;

	os["url"]=Url;

	return true;
}

//////////////////////////////////////////////////////////////////////////

/*
{
"touser":"OPENID",
"msgtype":"music",
"music":
{
"title":"MUSIC_TITLE",
"description":"MUSIC_DESCRIPTION",
"musicurl":"MUSIC_URL",
"hqmusicurl":"HQ_MUSIC_URL",
"thumb_media_id":"THUMB_MEDIA_ID" 
}
}

*/


bool WxMsgMusic::ParseFromJson(const Json::Value &os)
{
	if (!WxObj::ParseFromJson(os))
	{
		return false;
	}

	Json::Value music=os["music"];
	if (music!=Json::Value::null)
	{
		Title=GetStrFromJson(music,"title");
		Description=GetStrFromJson(music,"description");
		MusicURL=GetStrFromJson(music,"musicurl");
		HQMusicUrl=GetStrFromJson(music,"hqmusicurl");
		ThumbMediaId=GetStrFromJson(music,"thumb_media_id");

		return true;
	}

	return false;
}

bool WxMsgMusic::ToSendJson(Json::Value &os) const
{

	if (!WxObj::ToSendJson(os)){
		return false;
	}

	Json::Value music;

	music["title"]=Title;
	music["description"]=Description;
	music["musicurl"]=MusicURL;
	music["hqmusicurl"]=HQMusicUrl;
	music["thumb_media_id"]=ThumbMediaId;

	os["music"]=music;


	return true;
}

//////////////////////////////////////////////////////////////////////////

bool WxNewsItem::ParseFromJson(const Json::Value &os)
{

	Title=GetStrFromJson(os,"title");
	Description=GetStrFromJson(os,"description");
	Url=GetStrFromJson(os,"url");
	PicUrl=GetStrFromJson(os,"picurl");

	return true;
}

bool WxNewsItem::ToSendJson(Json::Value &os) const
{

	os["title"]=Title;  
	os["description"]=Description;  
	os["url"]=Url;  //通过上传多媒体文件，得到的id 
	os["picurl"]=PicUrl;  //通过上传多媒体文件，得到的id 
	return true;
}


/*
{
	"touser":"OPENID",
		"msgtype":"news",
		"news":{
			"articles": [
			{
				"title":"Happy Day",
					"description":"Is Really A Happy Day",
					"url":"URL",
					"picurl":"PIC_URL"
			},
			{
				"title":"Happy Day",
					"description":"Is Really A Happy Day",
					"url":"URL",
					"picurl":"PIC_URL"
			}
			]
		}
}
*/

bool WxMsgNews::ParseFromJson(const Json::Value &os)
{
	if (!WxMsgBase::ParseFromJson(os))
	{
		return false;
	}

	Json::Value news=os["news"];
	if (news!=Json::Value::null)
	{
		Json::Value articles=news["articles"];
		if (articles!=Json::Value::null)
		{
			for (unsigned int i=0;i<articles.size();++i)
			{
				WxNewsItem wn;
				if (wn.ParseFromJson(articles[i]))
				{
					News.push_back(wn);
				}
				
			}
			return !News.empty();
		}
	}

	return false;
}

bool WxMsgNews::ToSendJson(Json::Value &os) const
{

	if (!WxMsgBase::ToSendJson(os)){
		return false;
	}

	Json::Value  articles;
	for (size_t i=0;i<News.size();++i)
	{

		Json::Value wz;
		News[i].ToSendJson(wz);

		articles.append(wz);
	}

	Json::Value news;
	news["articles"]=articles;

	os["news"]=news;

	return true;
}


bool WxMsgNews::AddArticle(const WxNewsItem &a)
{
	if (News.size()>=10)
	{
		return false;
	}
	News.push_back(a);
	return true;
}
////////////////////////////wxevent接口//////////////////////////////////////////////


bool WxEventBase::ParseFromJson(const Json::Value &os)
{

	if (!WxObj::ParseFromJson(os)){
		return false;
	}

	Event=GetStrFromJson(os,"event");

	return !Event.empty();
}

//只提供给tq客户端使用，微信服务器不支持这个消息
bool WxEventBase::ToSendJson(Json::Value &os) const
{

	if (!WxObj::ToSendJson(os)){
		return false;
	}

	os["event"]=Event;

	return true;
}

//////////////////////////////////////////////////////////////////////////


bool WxEventSubscribe::ParseFromJson(const Json::Value &os)
{

	if (!WxEventBase::ParseFromJson(os)){
		return false;
	}

	EventKey=GetStrFromJson(os,"eventkey");
	Ticket=GetStrFromJson(os,"ticket");

	return !Event.empty();
}

//只提供给tq客户端使用，微信服务器不支持这个消息
bool WxEventSubscribe::ToSendJson(Json::Value &os) const
{

	if (!WxEventBase::ToSendJson(os)){
		return false;
	}

	os["eventkey"]=EventKey;
	os["ticket"]=Ticket;

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool WxEventLocation::ParseFromJson(const Json::Value &os)
{
	if (!WxEventBase::ParseFromJson(os))
	{
		return false;
	}

	local.Latitude=GetStrFromJson(os,"latitude");
	local.Longitude=GetStrFromJson(os,"longitude");
	local.Precision=GetStrFromJson(os,"precision");

	return !Event.empty();
}

bool WxEventLocation::ToSendJson(Json::Value &os) const
{

	if (!WxEventBase::ToSendJson(os)){
		return false;
	}

	os["latitude"]=local.Latitude;
	os["longitude"]=local.Longitude;
	os["precision"]=local.Precision;

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool WxEventMenu::ParseFromJson(const Json::Value &os)
{
	if (!WxEventBase::ParseFromJson(os))
	{
		return false;
	}

	EventKey=GetStrFromJson(os,"eventkey");


	return !Event.empty();
}

bool WxEventMenu::ToSendJson(Json::Value &os) const
{

	if (!WxEventBase::ToSendJson(os)){
		return false;
	}

	os["eventkey"]=EventKey;

	return true;
}

bool WxEventMasssendjobfinish::ParseFromJson(const Json::Value &os)
{

	if (!WxEventBase::ParseFromJson(os)) {
		return false;
	}

	MsgID = GetUllFromJson(os, "MsgID");
	TotalCount = GetUllFromJson(os, "TotalCount");
	FilterCount = GetUllFromJson(os, "FilterCount");
	SentCount = GetUllFromJson(os, "SentCount");
	ErrorCount = GetUllFromJson(os, "ErrorCount");

	return !Event.empty();
}


//////////////////////////////////////////////////////////////////////////


