#include "timer.h"

CTimer::CTimer()
{
	m_stopFlag = false;
}

CTimer::~CTimer()
{
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
}

DWORD WINAPI CTimer::ThreadProc(LPVOID p)
{
	CTimer* timer = (CTimer*)p;
	timer->Run();

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
	}
	m_mapTimers.clear();
}

void CTimerManager::SetTimer(int time, string timerName)
{
	map<string, CTimer*>::iterator iter = m_mapTimers.find(timerName);
	if (iter == m_mapTimers.end())
	{
		CTimer* timer = new CTimer();
		timer->Start(time, m_handler, timerName, m_this);
		m_mapTimers.insert(map<string, CTimer*>::value_type(timerName, timer));		
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
