#include "timer.h"

CTimer::CTimer()
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
				(*m_handler)(m_parameter);
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

void CTimer::Start(int time, TimerHandler func, void* lparam)
{
	m_interval = time;
	m_handler = func;
	m_parameter = lparam;
	m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
}
