// testmd5.cpp : Defines the entry point for the console application.
//
#include "../stdafx.h"
#include "md5.h"
#include <iostream>
using namespace std;



string getMd5(string str)
{
	unsigned char kmd[16];
	char strstr[33];
	memset(strstr,0,33) ;
	struct MD5Context md5c;
	MD5Init(&md5c);
	MD5Update(&md5c, (unsigned char*)str.c_str (),str.length ());
	MD5Final((unsigned char *)kmd, &md5c);

	dec2hex(kmd,strstr);
	return (string)strstr;
}


