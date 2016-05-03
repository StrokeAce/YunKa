#include "timer.h"

CTimer::CTimer()
{
	m_stopFlag = false;
}

void CTimer::Run()
{
	unsigned long tickNow = ::GetTickCount();
	unsigned long tickLastTime = tickNow;

	while (!m_stopFlag)
	{
		tickNow = ::GetTickCount();
		if (tickNow - tickLastTime > m_interval)
		{
			if (m_handler)
			{
				(*m_handler)(m_parameter, m_this);
			}
			tickLastTime = ::GetTickCount();
		}

		::Sleep(1);
	}
}

void CTimer::Stop()
{
	m_stopFlag = true;

	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hThread, INFINITE) != WAIT_ABANDONED)
		{
			CloseHandle(m_hThread);
		}
		m_hThread = INVALID_HANDLE_VALUE;
	}
}

DWORD WINAPI CTimer::ThreadProc(LPVOID p)
{
	CTimer* timer = (CTimer*)p;
	timer->Run();

	CloseHandle(timer->m_hThread);
	timer->m_hThread = INVALID_HANDLE_VALUE;

	return 0;
}

void CTimer::Start(int time, TimerHandler func, string lparam, LPVOID pThis)
{
	m_interval = time;
	m_handler = func;
	m_parameter = lparam;
	m_this = pThis;
	m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
}

CTimerManager::CTimerManager(TimerHandler func, LPVOID pThis)
{
	m_handler = func;
	m_this = pThis;
}

CTimerManager::~CTimerManager()
{
	map<string, CTimer*>::iterator iter = m_mapTimers.begin();
	for (iter; iter != m_mapTimers.end();iter++)
	{
		iter->second->Stop();
		delete iter->second;
		iter->second = NULL;
	}
	m_mapTimers.clear();
}

void CTimerManager::SetTimer(int time, string timerName)
{
	map<string, CTimer*>::iterator iter = m_mapTimers.find(timerName);
	if (iter == m_mapTimers.end())
	{
		CTimer* timer = new CTimer();
		m_mapTimers.insert(map<string, CTimer*>::value_type(timerName, timer));
		timer->Start(time, m_handler, timerName, m_this);
	}
}

void CTimerManager::KillTimer(string timerName)
{
	map<string, CTimer*>::iterator iter = m_mapTimers.find(timerName);
	if (iter != m_mapTimers.end())
	{
		iter->second->Stop();
		delete iter->second;
		m_mapTimers.erase(iter);
	}
}
