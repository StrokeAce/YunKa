#ifndef  _TSTRING_H_
#define  _TSTRING_H_


#ifdef _UNICODE	
#define tstring wstring	
#define tstringstream wstringstream
#define _taccess _waccess
#else	
#define tstring string	
#define tstringstream stringstream
#define _taccess _access

#endif

wstring  convertstring(const string& s);

#endif