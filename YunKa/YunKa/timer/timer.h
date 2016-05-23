#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>
#include <string>
#include <map>

using namespace std;

typedef void(CALLBACK *Timerfunc)(int timeId, LPVOID pThis);
typedef Timerfunc TimerHandler;

class CTimer
{
public:
	CTimer();

	~CTimer();

	void Start(int time, TimerHandler func, int id, LPVOID pThis);

	void Stop();

private:
	static DWORD WINAPI ThreadProc(LPVOID p);

	void Run();

private:
	TimerHandler	m_handler;
	int             m_interval;
	int				m_id;	
	bool			m_stopFlag;
	HANDLE			m_hThread;
	LPVOID			m_this;
};

class CTimerManager 
{
public:
	CTimerManager(TimerHandler func, LPVOID pThis);
	~CTimerManager();

	void SetTimer(int time, int timerId);

	void KillTimer(int timerId);

private:
	map<int, CTimer*>		m_mapTimers;
	TimerHandler			m_handler;
	LPVOID					m_this;
};

#endif