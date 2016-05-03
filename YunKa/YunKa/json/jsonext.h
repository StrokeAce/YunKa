#pragma  once

#include "../StdAfx.h"
#include "json.h"
#include <string>

using namespace  std;


string GetStrFromJson(const Json::Value &root,const char* key,const char* szDefault="");

unsigned int GetUintFromJson(const Json::Value &root,const char* key,unsigned int uDefault=0);

int GetIntFromJson(const Json::Value &root,const char* key,int uDefault=0);

ULONGLONG GetUllFromJson(const Json::Value &root,const char* key,ULONGLONG uDefault=0);

bool ParseJson(const string& szJsonStr,Json::Value &jv);

bool GetboolFromJson(const Json::Value &root, const char* key);//gxl

