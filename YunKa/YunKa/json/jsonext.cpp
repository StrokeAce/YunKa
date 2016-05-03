#pragma warning( disable : 4786)

#include "jsonext.h"
#include <stdlib.h>
#ifdef _WIN32
#define  atoll  _atoi64
#endif

string GetStrFromJson(const Json::Value &root,const char* key,const char* szDefault/*=""*/)
{
	string szret=szDefault;
	Json::Value t=root.get(key,Json::Value::null);
	if (t!=Json::Value::null&&t.isString())
	{
		const char *ptemp=t.asCString();
		if (ptemp!=NULL)
		{
			szret=ptemp;
		}
	}

	return szret;
}

//gxl
bool GetboolFromJson(const Json::Value &root, const char* key)
{
	bool bRet = false;
	Json::Value t = root.get(key, Json::Value::null);
	if (t != Json::Value::null&&t.isBool())
	{
		 bRet = t.asBool();
	}

	return bRet;

}


ULONGLONG GetUllFromJson(const Json::Value &root,const char* key,ULONGLONG uDefault/*=0*/)
{
	ULONGLONG uret=uDefault;
	Json::Value t=root.get(key,Json::Value::null);
	if (t!=Json::Value::null)
	{
		if (t.isString())
		{
			const char *ptemp=t.asCString();
			if (ptemp!=NULL)
			{
				uret=atoll(ptemp);
			}
		}else if (t.isUInt())
		{
			uret=t.asUInt();
		}else if (t.isInt())
		{
			uret=t.asInt();
		}
	}

	return uret;
}

unsigned int GetUintFromJson(const Json::Value &root,const char* key,unsigned int uDefault/*=0*/)
{
	return static_cast<unsigned int> ( GetUllFromJson(root,key,uDefault) );
}

int GetIntFromJson(const Json::Value &root,const char* key,int uDefault/*=0*/)
{
	return static_cast<int> ( GetUllFromJson(root,key,uDefault) );
}



bool ParseJson(const string& szJsonStr,Json::Value &jv)
{
	jv.clear();
	Json::Reader reader(Json::Features::strictMode());

	return reader.parse(szJsonStr,jv,false);
}
