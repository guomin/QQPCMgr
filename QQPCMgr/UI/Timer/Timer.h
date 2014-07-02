#pragma once
#include <vector>
using namespace std;

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

	//定显示器相关
	vector<UINT>			m_vecTimer;				// 定时器ID
	static UINT				m_uAccuracy;			// 定义分辨率 
	static BOOL				m_bIsTmierOK;			// 是否可以定时 

	BOOL GetTiemrIsRun(UINT uTimerID);
	int SetTimer(UINT nTimerResolution);
	int KillTimer(UINT uTimerID);
	void KillTimer();
	static void CALLBACK TimerCallProc(UINT uTimerID, UINT msg, DWORD dwData, DWORD dwa, DWORD dwb);
	virtual void OnTimer(UINT uTimerID) = 0;
};
