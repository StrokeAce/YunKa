#ifndef MUTEX_H_
#define MUTEX_H_

#ifdef WIN32
#include "windows.h"
#else
#include "pthread.h"
#endif

class MMutex
{
public:

    MMutex()
	{
#ifdef WIN32
	    InitializeCriticalSection(&fMutex);
#else
		pthread_mutex_init(&fMutex, NULL);
#endif
	}

	~MMutex()
	{
#ifdef WIN32
		DeleteCriticalSection(&fMutex);
#else
		pthread_mutex_destroy(&fMutex);
#endif
	}

    inline void Lock() 
	{
#ifdef WIN32
		::EnterCriticalSection(&fMutex);
#else
		(void)pthread_mutex_lock(&fMutex);
#endif
	};

    inline void Unlock() 
	{
#ifdef WIN32
	    ::LeaveCriticalSection(&fMutex);
#else
		pthread_mutex_unlock(&fMutex);
#endif
	};
    
private:

#ifdef WIN32
    CRITICAL_SECTION fMutex;                     
#else
    pthread_mutex_t fMutex;       
#endif       
};

#endif