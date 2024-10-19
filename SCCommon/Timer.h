/************************************************************************************************
	파일명 : Timer.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef __ONLINETIMER__
#define __ONLINETIMER__


class COnlineTimer
{
public:
	const	int			m_iHourTickCount;
	SOnlineTime			m_BaseTime;
	DWORD				m_StartTime;
	DWORD				m_StartTimeHigh;

	SOnlineTime			m_YetTime;
	DWORD				m_YetDayTime;
	DWORD				m_YetDayTimeHigh;

public:
	COnlineTimer( BOOL bIsTestServer = FALSE );		// Constructor
	virtual ~COnlineTimer();						// Destroyer

	bool		Create(SOnlineTime Time);			// Create
	void		Release(void);						// Release

	bool		SetNowTime(SOnlineTime Time);		// Set Now Time
	SOnlineTime GetNowTime(void);					// Get Now Time

	SOnlineTime	CheckTime(SOnlineTime NowTime, int NextDay);				// NowTime에서 Next Day만큼 지난 후 날짜를 검색한다.
	int			CheckPitchMonth(SOnlineTime YetTime, SOnlineTime NowTime);	// 두 시간사이의 달수를 측정한다.	
	int			CheckPitchHour(SOnlineTime YetTime, SOnlineTime NowTime);	// 두 시간사이의 시간를 측정한다.

	void		OnlineCheckTime(SOnlineTime& Time, __int64 PitchDay);	// Time의 일수에 PitchDay만큼 일수를 더함
	bool		IsLeap(int Year);										// 윤년 체크
	int			GetDaysInMonth(SOnlineTime& Time);
};


int			MakeTimeInt(SOnlineTime NowTime);			// SOnlineTime 형태의 Data를 Int 로 교체한다.
SOnlineTime MakeTimeOnlineTime(int NowTime);			// Int 형태의 Data를 SOnlineTime 로 교체한다.
SI32		MakeOneMonthAfter(SI32 NowTime);			// 한달후의 시간 검사


#endif