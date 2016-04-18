/************************************************************************

		Submodule	: dirfileop.h
		Depict		: Common Directory And File Operator Fun
		Language	: ANSI C++
		Version		: 1.0
		Compile		: make
		Compat		: Linux/Windows
		Editor		: yanyg
		Time		: 20067 08 28

************************************************************************/

//dirfileop.h : Common Directory And File Operator Fun

#ifndef __DIRFILEOP_H_
#define __DIRFILEOP_H_


#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include <ctime>
#include <direct.h>
#include <io.h>
#include <windows.h>
#define MakeDirectory(pBuf) _mkdir( (pBuf) )
#define GetCurDirectory(pBuf,len) _getcwd((pBuf),(len))
#define ChangeDirectory(pBuf) _chdir((pBuf))
#define Stat(pBuf,pStat) _stat((pBuf),(pStat))
typedef struct _stat StructStat;

#else

#include <unistd.h>
#define MakeDirectory(pBuf) mkdir( (pBuf) , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )
#define GetCurDirectory(pBuf,len) getcwd((pBuf),(len))
#define ChangeDirectory(pBuf) chdir(pBuf)
#define Stat(pBuf,pStat) lstat((pBuf),(pStat))
typedef struct stat StructStat;

#endif

#endif
