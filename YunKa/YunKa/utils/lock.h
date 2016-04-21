#ifndef __LOCK_H__
#define __LOCK_H__

#pragma once

#include <windows.h>

class CLock
{
public:
	CLock(){ InitializeCriticalSection(&m_cs); }
	~CLock(){ DeleteCriticalSection(&m_cs); }

	__inline void Lock(){ EnterCriticalSection(&m_cs); }
	__inline void Unlock(){ LeaveCriticalSection(&m_cs); }

private:
	CRITICAL_SECTION m_cs;
};

class AutoLock
{
public:
	AutoLock(CLock &l)
		:m_lock(l)
	{
		m_lock.Lock();
	}
	~AutoLock()
	{
		m_lock.Unlock();
	}

private:
	CLock &m_lock;

};

class CILock
{
public:
	CILock(CRITICAL_SECTION& cs){ m_cs = cs; EnterCriticalSection(&cs); }
	~CILock(){ LeaveCriticalSection(&m_cs); }
	static void Init(CRITICAL_SECTION& cs){ InitializeCriticalSection(&cs); }
	static void Free(CRITICAL_SECTION& cs){ DeleteCriticalSection(&cs); }
private:
	CRITICAL_SECTION m_cs;
};

class CTimeLock
{
public:
	CTimeLock()
	{
		m_hMutex = CreateMutex(NULL, FALSE, NULL);
	}
	~CTimeLock()
	{
		if (m_hMutex)
		{
			CloseHandle(m_hMutex);
		}
	}

	BOOL Lock(DWORD dwTimeOut = 0)
	{
		if (m_hMutex == NULL)
			return TRUE;

		DWORD dwRet = WaitForSingleObject(m_hMutex, dwTimeOut);

		return (dwRet == WAIT_OBJECT_0);
	}

	void UnLock()
	{
		if (m_hMutex == NULL)
			return;

		ReleaseMutex(m_hMutex);
	}

private:
	HANDLE m_hMutex;
};

class CSignalEvent
{
public:
	CSignalEvent()
	{
		m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~CSignalEvent()
	{
		if (m_hEvent)
		{
			CloseHandle(m_hEvent);
		}
	}

	BOOL Lock(DWORD dwTimeOut = 0)
	{
		if (m_hEvent == NULL)
			return TRUE;

		DWORD dwRet = WaitForSingleObject(m_hEvent, dwTimeOut);

		return (dwRet == WAIT_OBJECT_0);
	}

	void SetEvent()
	{
		if (m_hEvent != NULL)
			::SetEvent(m_hEvent);
	}

	void ResetEvent()
	{
		if (m_hEvent != NULL)
			::ResetEvent(m_hEvent);
	}

private:
	HANDLE m_hEvent;
};

#endif
