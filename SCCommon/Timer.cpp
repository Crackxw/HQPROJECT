/************************************************************************************************
	파일명 : Timer.cpp
	담당자 : 이윤석

	Game에서 사용하는 시간을 검사함
************************************************************************************************/
#include <GSL.h>
#include "OnlineCommonStruct.h"
#include <DebugTool.h>
#include "Timer.h"


//const int	HOURTICKCOUNT = 2880000;

const int	g_MonthMaxDay[13]  = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 365};
const int	g_LMonthMaxDay[13] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 366};


//=============================================
// COnlineTimer::COnlineTimer() Constructor
COnlineTimer::COnlineTimer( BOOL bIsTestServer ) : m_iHourTickCount( bIsTestServer ? 1440000 : 2880000 )
//COnlineTimer::COnlineTimer( BOOL bIsTestServer ) : m_iHourTickCount( bIsTestServer ? 1440000 : 288000 )
{
	// 참고: 1440000이 2880000의 2배속이다.
	ZeroMemory(&m_BaseTime, sizeof(SOnlineTime));
	m_StartTime         = 0;
	m_StartTimeHigh     = 0;

	ZeroMemory(&m_YetTime, sizeof(SOnlineTime));
	m_YetDayTime        = 0;
	m_YetDayTimeHigh    = 0;
}

//=============================================
// COnlineTimer::~COnlineTimer() Destroyer
COnlineTimer::~COnlineTimer()
{
	Release();
}

//=============================================
// COnlineTimer::Create() Create
bool COnlineTimer::Create(SOnlineTime Time)
{
	__int64		StartTime;
	__int64		BackTime;

	BackTime  =  (__int64)Time.Hour * (__int64)(m_iHourTickCount / 24);
	StartTime =  GetTickCount();
	if(StartTime < BackTime) StartTime += ((__int64)m_iHourTickCount << 32);
	StartTime -= BackTime;
	Time.Hour = 0;

	m_StartTime         = (DWORD)(StartTime & 0xffffffff);
	m_StartTimeHigh     = (DWORD)((StartTime & 0xffffffff00000000) >> 32);
	CopyMemory(&m_BaseTime, &Time, sizeof(SOnlineTime));

	m_YetDayTime        = (DWORD)(StartTime & 0xffffffff);
	m_YetDayTimeHigh    = (DWORD)((StartTime & 0xffffffff00000000) >> 32);
	CopyMemory(&m_YetTime, &Time, sizeof(SOnlineTime));

	return true;
}

//=============================================
// COnlineTimer::Release() Release
void COnlineTimer::Release(void)
{
	ZeroMemory(&m_BaseTime, sizeof(SOnlineTime));
	m_StartTime         = 0;
	m_StartTimeHigh     = 0;

	ZeroMemory(&m_YetTime, sizeof(SOnlineTime));
	m_YetDayTime        = 0;
	m_YetDayTimeHigh    = 0;
}

//=============================================
// COnlineTimer::SetNowTime() Set Now Time
bool COnlineTimer::SetNowTime(SOnlineTime Time)
{
	__int64		StartTime;
	__int64		BackTime;

	BackTime  =  (__int64)Time.Hour * (__int64)(m_iHourTickCount / 24);
	StartTime =  GetTickCount();
	if(StartTime < BackTime) StartTime += ((__int64)m_iHourTickCount << 32);
	StartTime -= BackTime;
	Time.Hour =  0;

	m_StartTime         = (DWORD)(StartTime & 0xffffffff);
	m_StartTimeHigh     = (DWORD)((StartTime & 0xffffffff00000000) >> 32);
	CopyMemory(&m_BaseTime, &Time, sizeof(SOnlineTime));

	m_YetDayTime        = (DWORD)(StartTime & 0xffffffff);
	m_YetDayTimeHigh    = (DWORD)((StartTime & 0xffffffff00000000) >> 32);
	CopyMemory(&m_YetTime, &Time, sizeof(SOnlineTime));

	return true;
}

//=============================================
// COnlineTimer::GetNowTime() Get Now Time
SOnlineTime COnlineTimer::GetNowTime(void)
{
	DWORD		CheckTick;
	DWORD		RealTime;
	DWORD		RealTimeHigh;
	SOnlineTime	TempSOnlineTime;
	__int64		NowTime;
	__int64		PitchTime;
	__int64		PitchDayPitchTime;
	__int64		TempTime;

	//=====================================================================================================================================
	// 현재 시간 검사(시간은 64bit 로 사용)
	RealTime     = m_YetDayTime;
	RealTimeHigh = m_YetDayTimeHigh;
	CheckTick    = GetTickCount();
	if(RealTime > CheckTick) RealTimeHigh++;
	RealTime     = CheckTick;

	//=====================================================================================================================================
	// 시간차 계산
	NowTime           = ((__int64)RealTime - (__int64)m_StartTime)  + ((__int64)((__int64)RealTimeHigh - (__int64)m_StartTimeHigh)  << 32);	// NowTime은 처음 시간을 계산한 시간부터 지난 밀리세컨트 Time을 가진다.
	PitchTime         = ((__int64)RealTime - (__int64)m_YetDayTime) + ((__int64)((__int64)RealTimeHigh - (__int64)m_YetDayTimeHigh) << 32);
	PitchDayPitchTime = NowTime % (m_iHourTickCount);																	// PitchDayPitchTime은 현재 날수를 계산하기 위해 검사하는 남은 시간이다.
	PitchTime         = PitchTime - PitchDayPitchTime;
	if(PitchTime >= m_iHourTickCount) PitchTime         = PitchTime / (m_iHourTickCount);																// PitchTime은 지난 검사한 시간보다 지난 일수를 가진다.
	else                           PitchTime         = 0;

	//=====================================================================================================================================
	// 시간 계산
	CopyMemory(&TempSOnlineTime, &m_YetTime, sizeof(SOnlineTime));
	OnlineCheckTime(TempSOnlineTime, PitchTime);
	TempSOnlineTime.Hour  = ((int)(NowTime % m_iHourTickCount) / (m_iHourTickCount / 24));

	if(TempSOnlineTime.Year > 1592)
	{
//		DWriteStringToFile("-- Time -- : Yet Time : %d년 %d월 %d일 %d시  %u %u Tick -> Now Time : %d년 %d월 %d일 %d시  %u %u Tick", m_YetTime.Year, m_YetTime.Month, m_YetTime.Day, m_YetTime.Hour
//			, m_YetDayTime, m_YetDayTimeHigh
//			, TempSOnlineTime.Year, TempSOnlineTime.Month, TempSOnlineTime.Day, TempSOnlineTime.Hour
//			, RealTime, RealTimeHigh);
	}
	//=====================================================================================================================================
	// 시간 저장
	CopyMemory(&m_YetTime, &TempSOnlineTime, sizeof(SOnlineTime));
	TempTime          = (__int64)RealTime + ((__int64)(RealTimeHigh) << 32);
	TempTime          = TempTime - PitchDayPitchTime;
	m_YetDayTime      = (DWORD)(TempTime & 0xffffffff);
	m_YetDayTimeHigh  = (DWORD)((TempTime & 0xffffffff00000000) >> 32);

	return TempSOnlineTime;
}

//=============================================
// COnlineTimer::CheckTime() NowTime에서 Next Day만큼 지난 후 날짜를 검색한다.
SOnlineTime	COnlineTimer::CheckTime(SOnlineTime NowTime, int NextDay)
{
	OnlineCheckTime(NowTime, (__int64)NextDay);
	return NowTime;
}

//=============================================
// COnlineTimer::CheckPitchMonth() 두 시간사이의 달수를 측정한다.
int COnlineTimer::CheckPitchMonth(SOnlineTime YetTime, SOnlineTime NowTime)
{
	int		Pitch;
	int		Month;

	Month = 0;

	// 우선 년도를 검사한다.
	Pitch =  NowTime.Year - YetTime.Year;
	Month += Pitch * 12;

	// 달수를 검사한다.
	Pitch =  NowTime.Month - YetTime.Month;
	Month += Pitch;

	return Month;
}

//=============================================
// COnlineTimer::CheckPitchHour() 두 시간사이의 시간를 측정한다.
int COnlineTimer::CheckPitchHour(SOnlineTime YetTime, SOnlineTime NowTime)
{
	BOOL		fYetFirst;
	int			iPitch;
	SOnlineTime TempTime;
	int			iPitchDay;
	int			iPitchHour;
	BOOL		fLeapYear;
	int			TempCount;

	// 우선 시간이 어느쪽이 앞쪽인지 검사
	fYetFirst = FALSE;
	if(YetTime.Year > NowTime.Year)
	{
		fYetFirst = TRUE;
	}
	else if(YetTime.Year == NowTime.Year)
	{
		if(YetTime.Month > NowTime.Month)
		{
			fYetFirst = TRUE;
		}
		else if(YetTime.Month == NowTime.Month)
		{
			if(YetTime.Day > NowTime.Day)
			{
				fYetFirst = TRUE;
			}
			else if(YetTime.Day == NowTime.Day)
			{
				if(YetTime.Hour > NowTime.Hour)
				{
					fYetFirst = TRUE;
				}
				else if(YetTime.Hour == NowTime.Hour)
				{
					return 0;
				}
			}
		}
	}

	if(fYetFirst)
	{
		CopyMemory(&TempTime, &YetTime,  sizeof(SOnlineTime));
		CopyMemory(&YetTime,  &NowTime,  sizeof(SOnlineTime));
		CopyMemory(&NowTime,  &TempTime, sizeof(SOnlineTime));
	}

	// 시간차를 계산한다.
	iPitchHour = NowTime.Hour - YetTime.Hour;

	// 우선 두 시간차의 날수를 구한다.
	iPitchDay = NowTime.Day - YetTime.Day;
	while(1)
	{
		// 현재 윤년인지
		fLeapYear = IsLeap(YetTime.Year);

		if(YetTime.Year == NowTime.Year)
		{
			for(TempCount = (YetTime.Month - 1); TempCount < (NowTime.Month - 1); TempCount++)
			{
				if(fLeapYear) iPitchDay += g_LMonthMaxDay[TempCount];
				else          iPitchDay += g_MonthMaxDay[TempCount];
			}
			break;
		}
		else
		{
			for(TempCount = (YetTime.Month - 1); TempCount < 12; TempCount++)
			{
				if(fLeapYear) iPitchDay += g_LMonthMaxDay[TempCount];
				else          iPitchDay += g_MonthMaxDay[TempCount];
			}

			YetTime.Year++;
			YetTime.Month = 1;
		}
	}

	// 두 시간차의 날수와 시간차를 계산한다.
	iPitchDay  = (iPitchDay * 24);
	iPitch     = iPitchDay + iPitchHour;

	if(fYetFirst) iPitch = -iPitch;
	return iPitch;
}

//=============================================
// COnlineTimer::OnlineCheckTime() Time의 일수에 PitchDay만큼 일수를 더함
void COnlineTimer::OnlineCheckTime(SOnlineTime& Time, __int64 PitchDay)
{
	int		MaxDay;

	while(PitchDay)
	{
		// 일수를 더함
		Time.Day++;

		// 일수에 맞는 달수인지 계산
		MaxDay = g_MonthMaxDay[Time.Month - 1];
		if(IsLeap(Time.Year)) MaxDay = g_LMonthMaxDay[Time.Month - 1];
		
		if(Time.Day > MaxDay)
		{
			Time.Day -= MaxDay;
			Time.Month++;
		}

		// 달수에 맞는 년수인지 계산
		if(Time.Month > 12)
		{
			Time.Month -= 12;
			Time.Year++;
		}

		PitchDay--;
	}
}

//=============================================
// COnlineTimer::IsLeap() 윤년 체크
bool COnlineTimer::IsLeap(int Year)
{
    return (((Year % 400) == 0) || ((Year % 100) != 0) && ((Year % 4) == 0));
}

//=============================================
// COnlineTimer::GetDaysInMonth()
int COnlineTimer::GetDaysInMonth(SOnlineTime& Time)
{
	if(IsLeap(Time.Year)) return g_LMonthMaxDay[Time.Month - 1];
	else                  return g_MonthMaxDay[Time.Month - 1];
}

//=============================================
// MakeTimeInt() SOnlineTime 형태의 Data를 Int 로 교체한다.
int MakeTimeInt(SOnlineTime NowTime)
{
	int		TempTime;

	TempTime = NowTime.Hour + (NowTime.Day * 100) + (NowTime.Month * 10000) + (NowTime.Year * 1000000);
	return TempTime;
}

//=============================================
// MakeTimeOnlineTime() Int 형태의 Data를 SOnlineTime 로 교체한다.
SOnlineTime MakeTimeOnlineTime(int NowTime)
{
	SOnlineTime		TempTime;

	TempTime.Year  = NowTime / 1000000;
	TempTime.Month = (NowTime / 10000) % 100;
	TempTime.Day   = (NowTime / 100) % 100;
	TempTime.Hour  = NowTime % 100;

	return TempTime;
}

//=============================================
// MakeOneMonthAfter() 한달후의 시간 검사
SI32 MakeOneMonthAfter(SI32 NowTime)
{
	SI32		TempInvestmentTime;
	SI32		TempInvestmentTimeYear;
	SI32		TempInvestmentTimeMonth;
	SI32		TempInvestmentTimeDay;

	// 마지막 투자된 시간을 얻어옴
	TempInvestmentTime      = NowTime;
	TempInvestmentTimeYear  = (TempInvestmentTime / 1000000);
	TempInvestmentTimeMonth = (TempInvestmentTime / 10000) % 100;
	TempInvestmentTimeDay   = (TempInvestmentTime / 100) % 100;

	// 여기에 한달을 더함
	TempInvestmentTimeMonth += 1;
	if(TempInvestmentTimeMonth > 12)
	{
		TempInvestmentTimeMonth =  1;
		TempInvestmentTimeYear  += 1;
	}

	TempInvestmentTime = (TempInvestmentTimeYear * 1000000) + (TempInvestmentTimeMonth * 10000) + (TempInvestmentTimeDay * 100);

	return TempInvestmentTime;
}