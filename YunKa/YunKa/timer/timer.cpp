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
				(*m_handler)(m_id, m_this);
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

void CTimer::Start(int time, TimerHandler func, int id, LPVOID pThis)
{
	m_interval = time;
	m_handler = func;
	m_id = id;
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
	map<int, CTimer*>::iterator iter = m_mapTimers.begin();
	for (iter; iter != m_mapTimers.end();iter++)
	{
		iter->second->Stop();
		delete iter->second;
	}
	m_mapTimers.clear();
}

void CTimerManager::SetTimer(int time, int timerId)
{
	map<int, CTimer*>::iterator iter = m_mapTimers.find(timerId);
	if (iter == m_mapTimers.end())
	{
		CTimer* timer = new CTimer();
		timer->Start(time, m_handler, timerId, m_this);
		m_mapTimers.insert(map<int, CTimer*>::value_type(timerId, timer));
	}
}

void CTimerManager::KillTimer(int timerId)
{
	map<int, CTimer*>::iterator iter = m_mapTimers.find(timerId);
	if (iter != m_mapTimers.end())
	{
		iter->second->Stop();
		delete iter->second;
		m_mapTimers.erase(iter);
	}
}
