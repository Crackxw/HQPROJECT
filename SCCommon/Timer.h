/************************************************************************************************
	���ϸ� : Timer.h
	����� : ������
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

	SOnlineTime	CheckTime(SOnlineTime NowTime, int NextDay);				// NowTime���� Next Day��ŭ ���� �� ��¥�� �˻��Ѵ�.
	int			CheckPitchMonth(SOnlineTime YetTime, SOnlineTime NowTime);	// �� �ð������� �޼��� �����Ѵ�.	
	int			CheckPitchHour(SOnlineTime YetTime, SOnlineTime NowTime);	// �� �ð������� �ð��� �����Ѵ�.

	void		OnlineCheckTime(SOnlineTime& Time, __int64 PitchDay);	// Time�� �ϼ��� PitchDay��ŭ �ϼ��� ����
	bool		IsLeap(int Year);										// ���� üũ
	int			GetDaysInMonth(SOnlineTime& Time);
};


int			MakeTimeInt(SOnlineTime NowTime);			// SOnlineTime ������ Data�� Int �� ��ü�Ѵ�.
SOnlineTime MakeTimeOnlineTime(int NowTime);			// Int ������ Data�� SOnlineTime �� ��ü�Ѵ�.
SI32		MakeOneMonthAfter(SI32 NowTime);			// �Ѵ����� �ð� �˻�


#endif