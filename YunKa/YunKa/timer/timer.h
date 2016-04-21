#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class CTimer
{
	typedef void(CALLBACK *Timerfunc)(void* p);
	typedef Timerfunc TimerHandler;
public:
	CTimer();

	void Start(int time, TimerHandler func, void* lparam);

	void Stop();

private:
	static DWORD WINAPI ThreadProc(LPVOID p);

	void Run();

private:
	TimerHandler	m_handler;
	int             m_interval;
	void*			m_parameter;	
	bool			m_stopFlag;
	HANDLE			m_hThread;	
};


#endif