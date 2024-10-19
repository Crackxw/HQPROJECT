/************************************************************************************************
	���ϸ� : Timer.cpp
	����� : ������

	Game���� ����ϴ� �ð��� �˻���
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
	// ����: 1440000�� 2880000�� 2����̴�.
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
	// ���� �ð� �˻�(�ð��� 64bit �� ���)
	RealTime     = m_YetDayTime;
	RealTimeHigh = m_YetDayTimeHigh;
	CheckTick    = GetTickCount();
	if(RealTime > CheckTick) RealTimeHigh++;
	RealTime     = CheckTick;

	//=====================================================================================================================================
	// �ð��� ���
	NowTime           = ((__int64)RealTime - (__int64)m_StartTime)  + ((__int64)((__int64)RealTimeHigh - (__int64)m_StartTimeHigh)  << 32);	// NowTime�� ó�� �ð��� ����� �ð����� ���� �и�����Ʈ Time�� ������.
	PitchTime         = ((__int64)RealTime - (__int64)m_YetDayTime) + ((__int64)((__int64)RealTimeHigh - (__int64)m_YetDayTimeHigh) << 32);
	PitchDayPitchTime = NowTime % (m_iHourTickCount);																	// PitchDayPitchTime�� ���� ������ ����ϱ� ���� �˻��ϴ� ���� �ð��̴�.
	PitchTime         = PitchTime - PitchDayPitchTime;
	if(PitchTime >= m_iHourTickCount) PitchTime         = PitchTime / (m_iHourTickCount);																// PitchTime�� ���� �˻��� �ð����� ���� �ϼ��� ������.
	else                           PitchTime         = 0;

	//=====================================================================================================================================
	// �ð� ���
	CopyMemory(&TempSOnlineTime, &m_YetTime, sizeof(SOnlineTime));
	OnlineCheckTime(TempSOnlineTime, PitchTime);
	TempSOnlineTime.Hour  = ((int)(NowTime % m_iHourTickCount) / (m_iHourTickCount / 24));

	if(TempSOnlineTime.Year > 1592)
	{
//		DWriteStringToFile("-- Time -- : Yet Time : %d�� %d�� %d�� %d��  %u %u Tick -> Now Time : %d�� %d�� %d�� %d��  %u %u Tick", m_YetTime.Year, m_YetTime.Month, m_YetTime.Day, m_YetTime.Hour
//			, m_YetDayTime, m_YetDayTimeHigh
//			, TempSOnlineTime.Year, TempSOnlineTime.Month, TempSOnlineTime.Day, TempSOnlineTime.Hour
//			, RealTime, RealTimeHigh);
	}
	//=====================================================================================================================================
	// �ð� ����
	CopyMemory(&m_YetTime, &TempSOnlineTime, sizeof(SOnlineTime));
	TempTime          = (__int64)RealTime + ((__int64)(RealTimeHigh) << 32);
	TempTime          = TempTime - PitchDayPitchTime;
	m_YetDayTime      = (DWORD)(TempTime & 0xffffffff);
	m_YetDayTimeHigh  = (DWORD)((TempTime & 0xffffffff00000000) >> 32);

	return TempSOnlineTime;
}

//=============================================
// COnlineTimer::CheckTime() NowTime���� Next Day��ŭ ���� �� ��¥�� �˻��Ѵ�.
SOnlineTime	COnlineTimer::CheckTime(SOnlineTime NowTime, int NextDay)
{
	OnlineCheckTime(NowTime, (__int64)NextDay);
	return NowTime;
}

//=============================================
// COnlineTimer::CheckPitchMonth() �� �ð������� �޼��� �����Ѵ�.
int COnlineTimer::CheckPitchMonth(SOnlineTime YetTime, SOnlineTime NowTime)
{
	int		Pitch;
	int		Month;

	Month = 0;

	// �켱 �⵵�� �˻��Ѵ�.
	Pitch =  NowTime.Year - YetTime.Year;
	Month += Pitch * 12;

	// �޼��� �˻��Ѵ�.
	Pitch =  NowTime.Month - YetTime.Month;
	Month += Pitch;

	return Month;
}

//=============================================
// COnlineTimer::CheckPitchHour() �� �ð������� �ð��� �����Ѵ�.
int COnlineTimer::CheckPitchHour(SOnlineTime YetTime, SOnlineTime NowTime)
{
	BOOL		fYetFirst;
	int			iPitch;
	SOnlineTime TempTime;
	int			iPitchDay;
	int			iPitchHour;
	BOOL		fLeapYear;
	int			TempCount;

	// �켱 �ð��� ������� �������� �˻�
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

	// �ð����� ����Ѵ�.
	iPitchHour = NowTime.Hour - YetTime.Hour;

	// �켱 �� �ð����� ������ ���Ѵ�.
	iPitchDay = NowTime.Day - YetTime.Day;
	while(1)
	{
		// ���� ��������
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

	// �� �ð����� ������ �ð����� ����Ѵ�.
	iPitchDay  = (iPitchDay * 24);
	iPitch     = iPitchDay + iPitchHour;

	if(fYetFirst) iPitch = -iPitch;
	return iPitch;
}

//=============================================
// COnlineTimer::OnlineCheckTime() Time�� �ϼ��� PitchDay��ŭ �ϼ��� ����
void COnlineTimer::OnlineCheckTime(SOnlineTime& Time, __int64 PitchDay)
{
	int		MaxDay;

	while(PitchDay)
	{
		// �ϼ��� ����
		Time.Day++;

		// �ϼ��� �´� �޼����� ���
		MaxDay = g_MonthMaxDay[Time.Month - 1];
		if(IsLeap(Time.Year)) MaxDay = g_LMonthMaxDay[Time.Month - 1];
		
		if(Time.Day > MaxDay)
		{
			Time.Day -= MaxDay;
			Time.Month++;
		}

		// �޼��� �´� ������� ���
		if(Time.Month > 12)
		{
			Time.Month -= 12;
			Time.Year++;
		}

		PitchDay--;
	}
}

//=============================================
// COnlineTimer::IsLeap() ���� üũ
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
// MakeTimeInt() SOnlineTime ������ Data�� Int �� ��ü�Ѵ�.
int MakeTimeInt(SOnlineTime NowTime)
{
	int		TempTime;

	TempTime = NowTime.Hour + (NowTime.Day * 100) + (NowTime.Month * 10000) + (NowTime.Year * 1000000);
	return TempTime;
}

//=============================================
// MakeTimeOnlineTime() Int ������ Data�� SOnlineTime �� ��ü�Ѵ�.
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
// MakeOneMonthAfter() �Ѵ����� �ð� �˻�
SI32 MakeOneMonthAfter(SI32 NowTime)
{
	SI32		TempInvestmentTime;
	SI32		TempInvestmentTimeYear;
	SI32		TempInvestmentTimeMonth;
	SI32		TempInvestmentTimeDay;

	// ������ ���ڵ� �ð��� ����
	TempInvestmentTime      = NowTime;
	TempInvestmentTimeYear  = (TempInvestmentTime / 1000000);
	TempInvestmentTimeMonth = (TempInvestmentTime / 10000) % 100;
	TempInvestmentTimeDay   = (TempInvestmentTime / 100) % 100;

	// ���⿡ �Ѵ��� ����
	TempInvestmentTimeMonth += 1;
	if(TempInvestmentTimeMonth > 12)
	{
		TempInvestmentTimeMonth =  1;
		TempInvestmentTimeYear  += 1;
	}

	TempInvestmentTime = (TempInvestmentTimeYear * 1000000) + (TempInvestmentTimeMonth * 10000) + (TempInvestmentTimeDay * 100);

	return TempInvestmentTime;
}