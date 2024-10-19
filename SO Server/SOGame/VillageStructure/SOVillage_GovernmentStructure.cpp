#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "SODatabase.h"
#include "SOVillage_GovernmentStructure.h"


SOVillage_Government::SOVillage_Government()
{
	m_uiVillageCode = 0;

	m_lpVillage             = NULL;
	m_lpstVillageInvestment = NULL;
}

SOVillage_Government::~SOVillage_Government()
{
	Free();
}

BOOL SOVillage_Government::Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage)
{
	m_uiVillageCode = uiVillageCode;
	m_lpVillage   = lpVillage;

	SOVillage_BaseStructure::Init(uiVillageCode, dwMaxPlayer);

	m_lpstVillageInvestment = new stVillageInvestment;
	ZeroMemory(m_lpstVillageInvestment, sizeof(stVillageInvestment));
	VillageInvestment_DBGetData();

	return TRUE;
}

VOID SOVillage_Government::Free()
{
	m_uiVillageCode = 0;

	m_lpVillage             = NULL;
	if(m_lpstVillageInvestment)
	{
		delete m_lpstVillageInvestment;
		m_lpstVillageInvestment = NULL;
	}

	SOVillage_BaseStructure::Free();	
}

BOOL SOVillage_Government::VillageInvestment_DBGetData(void)
{
	UI08							TempName[21];
	UI08							TempFlag;
	SI32							TempUpdateTime;
	UI08							TempName1[21];
	UI08							TempFlag1;
	SI32							TempUpdateTime1;
	int								TempCount;
	DBIKInvestorGetInfo				TempDBIKInvestorGetInfo;
	DBIKInvestorGetInfo_Result		TempDBIKInvestorGetInfo_Result;
	DBIKInvestGetInvestment			TempDBIKGetInvestment;
	DBIKInvestGetInvestment_Result	TempDBIKGetInvestment_Result;
	
	// Base Data
	ZeroMemory(m_lpstVillageInvestment, sizeof(stVillageInvestment));

	m_lpstVillageInvestment->m_BestInvestorInvestmentIndex          = -1;
	m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime      = 0;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex     = -1;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = 0;

	// Get List
	TempDBIKGetInvestment.uiVillageCode = m_lpVillage->uiVillageCode;
	ZeroMemory(&TempDBIKGetInvestment_Result, sizeof(DBIKInvestGetInvestment_Result));
	m_lpVillage->clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_INVEST_GETINVESTMENT, LPVOID(&TempDBIKGetInvestment), LPVOID(&TempDBIKGetInvestment_Result));
	m_lpstVillageInvestment->m_InvestorDataCount = TempDBIKGetInvestment_Result.siInvestmentCount;
	CopyMemory(m_lpstVillageInvestment->m_InvestorData, TempDBIKGetInvestment_Result.lpstVillageInvestorData, sizeof(stVillageInvestorData) * m_lpstVillageInvestment->m_InvestorDataCount);
	
	// 최고 투자자 정보 얻어옴
	TempDBIKInvestorGetInfo.uiVillageCode = m_uiVillageCode;
	ZeroMemory(&TempDBIKInvestorGetInfo_Result, sizeof(DBIKInvestorGetInfo_Result));
	m_lpVillage->clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_INVESTORGETINFO, LPVOID(&TempDBIKInvestorGetInfo), LPVOID(&TempDBIKInvestorGetInfo_Result));
	CopyMemory(TempName, TempDBIKInvestorGetInfo_Result.szRealBestInvestorID, 21);
	TempFlag        = TempDBIKInvestorGetInfo_Result.uiRealBestInvestorType;
	TempUpdateTime  = TempDBIKInvestorGetInfo_Result.uiRealBestInvestorUpdateTime;
	CopyMemory(TempName1, TempDBIKInvestorGetInfo_Result.szNowBestInvestorID, 21);
	TempFlag1       = TempDBIKInvestorGetInfo_Result.uiNowBestInvestorType;
	TempUpdateTime1 = TempDBIKInvestorGetInfo_Result.uiNowBestInvestorUpdateTime;

	m_lpstVillageInvestment->m_BestInvestorInvestmentIndex          = -1;
	m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime      = 0;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex     = -1;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = 0;

	// 얻어온 정보로 검색한다.
	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType != 2)
		{
			if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType == TempFlag)
			{
				if(strncmp((char*)m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorID, (char*)TempName, 21) == 0)
				{
					m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex          = TempCount;
					m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime      = TempUpdateTime;
				}
			}

			if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType == TempFlag1)
			{
				if(strncmp((char*)m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorID, (char*)TempName1, 21) == 0)
				{
					m_lpstVillageInvestment->m_BestInvestorInvestmentIndex          = TempCount;
					m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime      = TempUpdateTime1;
				}
			}
		}
	}

	// 현재 마을에 아무도 주식을 가지고 있지 않으면, 처음으로 국가에서 인구만큼의 주식을 넣는다.
	if(m_lpstVillageInvestment->m_InvestorDataCount == 0)
	{
		DBIKInvestSetInvestment			TempDBIKInvestSetInvestment;

		m_lpstVillageInvestment->m_InvestorData[0].m_siInvestorDBAccount = 0;
		m_lpstVillageInvestment->m_InvestorData[0].m_uiInvestorSlot      = 0;
		m_lpstVillageInvestment->m_InvestorData[0].m_InvestorType        = 2;
		m_lpstVillageInvestment->m_InvestorData[0].m_Percent             = 100.0f;
		m_lpstVillageInvestment->m_InvestorData[0].m_Money               = GetBaseInvestment();
		m_lpstVillageInvestment->m_InvestorData[0].m_UpdateTime          = MakeTimeInt(m_lpVillage->clWorld->CurrentTime);
		sprintf((char*)m_lpstVillageInvestment->m_InvestorData[0].m_InvestorID, "Basic");

		ZeroMemory(&TempDBIKInvestSetInvestment, sizeof(TempDBIKInvestSetInvestment));
		TempDBIKInvestSetInvestment.uiVillageCode         = m_lpVillage->uiVillageCode;
		TempDBIKInvestSetInvestment.siInvestorDBAccount   = m_lpstVillageInvestment->m_InvestorData[0].m_siInvestorDBAccount;
		TempDBIKInvestSetInvestment.uiInvestorSlot        = m_lpstVillageInvestment->m_InvestorData[0].m_uiInvestorSlot;
		CopyMemory(TempDBIKInvestSetInvestment.szInvestorID, m_lpstVillageInvestment->m_InvestorData[0].m_InvestorID, 21);
		TempDBIKInvestSetInvestment.uiInvestorType        = m_lpstVillageInvestment->m_InvestorData[0].m_InvestorType;
//		TempDBIKInvestSetInvestment.moInvestmentMoney     = 500000;
		TempDBIKInvestSetInvestment.moInvestmentMoney     = m_lpstVillageInvestment->m_InvestorData[0].m_Money;
		TempDBIKInvestSetInvestment.moInvestmentSaveMoney = 0;
		TempDBIKInvestSetInvestment.uiInvestorUpdateTime  = m_lpstVillageInvestment->m_InvestorData[0].m_UpdateTime;

		m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVEST_SETINVESTMENT, LPVOID(&TempDBIKInvestSetInvestment), sizeof(TempDBIKInvestSetInvestment));
		
		m_lpstVillageInvestment->m_InvestorDataCount++;
	}

	VillageInvestment_Updata();

	if(TempDBIKGetInvestment_Result.lpstVillageInvestorData)
	{
		delete[] TempDBIKGetInvestment_Result.lpstVillageInvestorData;
		TempDBIKGetInvestment_Result.lpstVillageInvestorData = NULL;
	}

	return TRUE;
}

BOOL	SOVillage_Government::VillageInvestment_ReLoad()
{
	int							i;
	MONEY						mnTempBestInvestMoney; 
	int							NowBestInvestIndex = -1;
	MONEY						NowBestInvestMoney = 0;
	DBIKInvestorSetInfo			TempDBIKInvestorSetInfo;
	__int64						MaxMoney;

	MaxMoney = 0;
	for(i = 0; i < m_lpstVillageInvestment->m_InvestorDataCount; i++)
	{
		MaxMoney += m_lpstVillageInvestment->m_InvestorData[i].m_Money;
	}

	m_lpstVillageInvestment->m_NowAllInvestmentMoney = 0;
	for(i = 0; i < m_lpstVillageInvestment->m_InvestorDataCount; i++)
	{	// 비울 계산c
		m_lpstVillageInvestment->m_InvestorData[i].m_Percent = (float)(100.0 * (double)m_lpstVillageInvestment->m_InvestorData[i].m_Money / (double)MaxMoney);
		m_lpstVillageInvestment->m_NowAllInvestmentMoney += m_lpstVillageInvestment->m_InvestorData[i].m_Money;
	}


	//공성후 마을 최투의 50%가 빠진후의 최투가 가진돈.
	int nBestIndex = m_lpstVillageInvestment->m_BestInvestorInvestmentIndex;
	mnTempBestInvestMoney = m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_Money;

	for(i = 0; i < m_lpstVillageInvestment->m_InvestorDataCount; i++)
	{
		if(i == nBestIndex) continue;
		if(m_lpstVillageInvestment->m_InvestorData[i].m_InvestorType != 2)
		{	//가진 돈이 50%보다 많으면.
			if(m_lpstVillageInvestment->m_InvestorData[i].m_Money > mnTempBestInvestMoney)
			{	//그 중에 가장 높은놈을 찾는다.
				if(NowBestInvestMoney < m_lpstVillageInvestment->m_InvestorData[i].m_Money)
				{
					NowBestInvestIndex = i;
					NowBestInvestMoney = m_lpstVillageInvestment->m_InvestorData[i].m_Money;
				}
			}		
		}
	}

	//만약 50% 빠졌는데도 현재 최투가 이전 최투 그대로이면.
	if(NowBestInvestIndex == -1)
	{
		m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex = m_lpstVillageInvestment->m_BestInvestorInvestmentIndex;
		m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime;

		//최고 투자자라는 이름은 일단 날린다.(한달후 다시 생긴다.)
		m_lpstVillageInvestment->m_BestInvestorInvestmentIndex = -1;
		m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime = 0;
		//MakeTimeInt(m_lpVillage->clWorld->CurrentTime);
	}
	else //최투가 변경되었다.
	{
		m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex     = NowBestInvestIndex;
		m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = MakeTimeInt(m_lpVillage->clWorld->CurrentTime);

		m_lpstVillageInvestment->m_BestInvestorInvestmentIndex = -1;
		m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime = 0;
	}

	ZeroMemory(&TempDBIKInvestorSetInfo, sizeof(DBIKInvestorSetInfo));
	TempDBIKInvestorSetInfo.uiVillageCode	=	m_uiVillageCode;

	strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, " ");
	TempDBIKInvestorSetInfo.uiNowBestInvestorType = 100;
	TempDBIKInvestorSetInfo.uiNowBestInvestorUpdateTime = m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime;

	strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID,
		   (char*)m_lpstVillageInvestment->m_InvestorData[NowBestInvestIndex].m_InvestorID);
	
	TempDBIKInvestorSetInfo.uiRealBestInvestorType = m_lpstVillageInvestment->m_InvestorData[NowBestInvestIndex].m_InvestorType;
	TempDBIKInvestorSetInfo.uiRealBestInvestorUpdateTime = m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime;

	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVESTORSETINFO, LPVOID(&TempDBIKInvestorSetInfo), sizeof(TempDBIKInvestorSetInfo));

/*
	strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, " ");
	TempDBIKInvestorSetInfo.uiNowBestInvestorType = 100;
	TempDBIKInvestorSetInfo.uiNowBestInvestorUpdateTime = m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime;

	strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID,
		   (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex].m_InvestorID);
	TempDBIKInvestorSetInfo.uiRealBestInvestorType = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex].m_InvestorType;
	TempDBIKInvestorSetInfo.uiRealBestInvestorUpdateTime = m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime;

	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVESTORSETINFO, LPVOID(&TempDBIKInvestorSetInfo), sizeof(TempDBIKInvestorSetInfo));
*/
	return TRUE;
}



BOOL SOVillage_Government::VillageInvestment_Updata(void)
{
	int							TempCount;
	__int64						MaxMoney;
	__int64						MinMoney;
	__int64						TempMaxMoney;
	int							TempBestInvestorInvestmentIndex;
	int							TempBestInvestorInvestmenUpdateTime;
	DBIKInvestDelInvestment		TempDBIKInvestDelInvestment;

	//==========================================================================================
	// 최대 주식계산
	MaxMoney = 0;
	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		MaxMoney += m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money;
	}

	//==========================================================================================
	// 1% 이하나 돈이 0이면 삭제
	MinMoney = (MaxMoney / 100);
	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType != 2)
		{
			if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money <= MinMoney)
			{
				// 최고 투자자 Index 검사
				if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex == TempCount)
				{
					m_lpstVillageInvestment->m_BestInvestorInvestmentIndex     = -1;
					m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime = 0;
				}
				else if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex > TempCount)
				{
					m_lpstVillageInvestment->m_BestInvestorInvestmentIndex--;
				}

				if(m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex == TempCount)
				{
					m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex     = -1;
					m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = 0;
				}
				else if(m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex > TempCount)
				{
					m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex--;
				}

				// DB에 저장
				TempDBIKInvestDelInvestment.uiVillageCode       = m_lpVillage->uiVillageCode;
				TempDBIKInvestDelInvestment.siInvestorDBAccount = m_lpstVillageInvestment->m_InvestorData[TempCount].m_siInvestorDBAccount;
				TempDBIKInvestDelInvestment.uiInvestorSlot      = m_lpstVillageInvestment->m_InvestorData[TempCount].m_uiInvestorSlot;
				m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVEST_DELINVESTMENT, LPVOID(&TempDBIKInvestDelInvestment), sizeof(TempDBIKInvestDelInvestment));

				// Buffer 밀기
				if((TempCount + 1) < m_lpstVillageInvestment->m_InvestorDataCount)
				{
					memmove(&m_lpstVillageInvestment->m_InvestorData[TempCount], &m_lpstVillageInvestment->m_InvestorData[TempCount + 1]
						, sizeof(stVillageInvestorData) * (m_lpstVillageInvestment->m_InvestorDataCount - (TempCount + 1)));

					TempCount--;
				}

				m_lpstVillageInvestment->m_InvestorDataCount--;
			}
		}
	}

	//==========================================================================================
	// % 검사
	m_lpstVillageInvestment->m_NowAllInvestmentMoney = 0;
	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		// 비울 계산
		m_lpstVillageInvestment->m_InvestorData[TempCount].m_Percent = (float)(100.0 * (double)m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money / (double)MaxMoney);

		m_lpstVillageInvestment->m_NowAllInvestmentMoney += m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money;
	}

	//==========================================================================================
	// 최대 주식 투자자 검사
	TempBestInvestorInvestmentIndex     = m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex;
	TempBestInvestorInvestmenUpdateTime = m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex = -1;
	TempMaxMoney                                                = 0;

	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType != 2)
		{
			if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money > TempMaxMoney)
			{
				m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex     = TempCount;
				m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = m_lpstVillageInvestment->m_InvestorData[TempCount].m_UpdateTime;

				TempMaxMoney                                                    = m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money;
			}		
		}
	}

	if(m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex == TempBestInvestorInvestmentIndex)
	{
		m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = TempBestInvestorInvestmenUpdateTime;
	}

	//==========================================================================================
	// 최고 투자자 정보 저장
	DBIKInvestorSetInfo			TempDBIKInvestorSetInfo;

	ZeroMemory(&TempDBIKInvestorSetInfo, sizeof(DBIKInvestorSetInfo));
	TempDBIKInvestorSetInfo.uiVillageCode	=	m_uiVillageCode;
	if(m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex == -1)
	{
		strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID, " ");
		TempDBIKInvestorSetInfo.uiRealBestInvestorType = 100;
	}
	else
	{
		strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID, (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex].m_InvestorID);
		TempDBIKInvestorSetInfo.uiRealBestInvestorType = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex].m_InvestorType;
	}
	TempDBIKInvestorSetInfo.uiRealBestInvestorUpdateTime = m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime;

	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex == -1)
	{
		strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, " ");
		TempDBIKInvestorSetInfo.uiNowBestInvestorType = 100;
	}
	else
	{
		strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID);
		TempDBIKInvestorSetInfo.uiNowBestInvestorType = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorType;
	}
	TempDBIKInvestorSetInfo.uiNowBestInvestorUpdateTime = m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime;

	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVESTORSETINFO, LPVOID(&TempDBIKInvestorSetInfo), sizeof(TempDBIKInvestorSetInfo));

	return TRUE;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Name - 이민수
// Date - 2004.01.29
// Desc - 두 시간으로 일수 차를 구한다.
// (단, 시간이 변활때 호출해야만 유효함, 그러므로 년 변경은 고려 안함)
// 
BOOL SOVillage_Government::CheckInHourPitchDay(SOnlineTime YetTime, SOnlineTime NowTime, int nDay)
{
	int		iPitch = 0;
	int		iDay = 0;

	int		iYetMaxDay = m_lpVillage->clWorld->Timer.GetDaysInMonth(YetTime);
	int		iNowMaxDay = m_lpVillage->clWorld->Timer.GetDaysInMonth(NowTime);
	int		iTempYetDay = iYetMaxDay - YetTime.Day;
	int		iTempNowDay = iNowMaxDay - NowTime.Day;
	
	iPitch =  NowTime.Month - YetTime.Month;
	// 달이 변경하면.
	if(iPitch != 0)
	{
		iPitch = iTempNowDay + iTempYetDay;
		iDay += iPitch;
	}
	else
	{
		iDay += (NowTime.Day - YetTime.Day);
	}

	return iDay >= nDay ? TRUE : FALSE;
}

BOOL SOVillage_Government::VillageInvestment_CheckBestInvestmentor(void)
{
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex)
	{
		SOnlineTime		TempTime;
		int				PitchMonth = 0;
		BOOL			bPitchDay = 0;
		
		TempTime   = MakeTimeOnlineTime(m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime);

		//////////////////////////////////////////////////////////////////////////////
		// 한달로 적용할때.
		PitchMonth = m_lpVillage->clWorld->Timer.CheckPitchMonth(TempTime, m_lpVillage->clWorld->CurrentTime);
		if((PitchMonth >= 2) || ((PitchMonth == 1) && (m_lpVillage->clWorld->CurrentTime.Day >= TempTime.Day)))
		//////////////////////////////////////////////////////////////////////////////
		// 보름으로 적용할때.
//		bPitchDay = CheckInHourPitchDay(TempTime, m_lpVillage->clWorld->CurrentTime);

//		if((bPitchDay == TRUE) && 
//			(m_lpVillage->clWorld->CurrentTime.Day >= TempTime.Day))
		//////////////////////////////////////////////////////////////////////////////
		{
			m_lpstVillageInvestment->m_BestInvestorInvestmentIndex     = m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex;
			m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime = m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime;

			// 최고 투자자 정보를 저장한다.
			DBIKInvestorSetInfo			TempDBIKInvestorSetInfo;

			ZeroMemory(&TempDBIKInvestorSetInfo, sizeof(DBIKInvestorSetInfo));
			TempDBIKInvestorSetInfo.uiVillageCode	=	m_uiVillageCode;
			if(m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex == -1)
			{
				strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID, " ");
				TempDBIKInvestorSetInfo.uiRealBestInvestorType = 100;
			}
			else
			{
				strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID, (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex].m_InvestorID);
				TempDBIKInvestorSetInfo.uiRealBestInvestorType = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex].m_InvestorType;
			}
			TempDBIKInvestorSetInfo.uiRealBestInvestorUpdateTime = m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime;

			if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex == -1)
			{
				strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, " ");
				TempDBIKInvestorSetInfo.uiNowBestInvestorType = 100;
			}
			else
			{
				strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID);
				TempDBIKInvestorSetInfo.uiNowBestInvestorType = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorType;
			}
			TempDBIKInvestorSetInfo.uiNowBestInvestorUpdateTime = m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime;

			m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVESTORSETINFO, LPVOID(&TempDBIKInvestorSetInfo), sizeof(TempDBIKInvestorSetInfo));

			// 행수를 지정한다.
			m_lpVillage->clWorld->clIGuild.SetHNS(m_lpVillage);

			// 최고 투자자가 변경되었다는 메시지를 보내준다.
			m_lpVillage->clWorld->OfficialAnnouncement(ON_RET_OA_MAXINVESTMENT_CHANGED, 
							LPARAM(m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID),
							LPARAM(m_lpVillage->uiVillageCode));

			// 마을 방어력을 올린다.
			/*
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			????????????????????????????
			VDEF		TempUpDefence;
			MONEY		DefPerMoney;
			MONEY		MaxMoney;

			DefPerMoney = 4;
			MaxMoney    = m_lpstVillageInvestment->m_LeftInvestmentMoney;
			if(m_lpVillage->vdDefence < m_lpVillage->vdMaxDefence)
			{
				TempUpDefence = m_lpVillage->vdMaxDefence - m_lpVillage->vdDefence;

				if((MaxMoney / DefPerMoney) < TempUpDefence)
				{
					TempUpDefence = MaxMoney / DefPerMoney;
				}

				m_lpVillage->RepairVillageDefence(TempUpDefence);
				m_lpstVillageInvestment->m_LeftInvestmentMoney -= TempUpDefence * DefPerMoney;

				DBIKVillageSetCapital		TempDBIKVillageSetCapital;

				TempDBIKVillageSetCapital.uiVillageCode		= m_lpVillage->uiVillageCode;
				TempDBIKVillageSetCapital.moVillageCapital	= m_lpstVillageInvestment->m_LeftInvestmentMoney;
				m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGESETCAPITAL, LPVOID(&TempDBIKVillageSetCapital), sizeof(TempDBIKVillageSetCapital));
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
		}
	}

	return TRUE;
}

BOOL SOVillage_Government::GovernmentInvestmentEnter(SOPlayer *pPlayer)
{
	// 들어가는것을 허락한다.
	pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(0), LPVOID(GetInvestmentMinPercent()), LPVOID(GetInvestmentMaxPercent()));

	// 바로 다음에 List Message를 보낸다.
	GovernmentInvestmentGetList(pPlayer);

	return TRUE;
}

BOOL SOVillage_Government::GovernmentInvestmentLeave(SOPlayer *pPlayer)
{
	pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	return TRUE;
}

BOOL SOVillage_Government::GovernmentInvestmentGetList(SOPlayer *pPlayer)
{
	__int64							TempGainingsMoney;
	DWORD							TempType;

	TempGainingsMoney = GetGainingsMoney();
	TempType          = 0 | (InvestmentGetOutF() << 16) | (InvestmentGetOutTexP() << 24);
	pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST, SO_SFM_ONLYME, LPVOID(GetInvestmentMaxInvestmentorName())
		, LPVOID(TempType), LPVOID(&TempGainingsMoney), LPVOID(m_lpstVillageInvestment->m_InvestorDataCount), LPVOID(m_lpstVillageInvestment->m_InvestorData));
	
	return TRUE;
}

BOOL SOVillage_Government::GovernmentInvestmentSetInvestment(SOPlayer *pPlayer, __int64 InvestmentMoney,__int64 InvestmentSaveMoney)
{
	SI32							TempCount;
	SI32							SelectIndex;
	SI32							EmptyIndex;
	MONEY							TempTestInvestmentMoney;
	SI32							TempNowTime;
	DBIKInvestSetInvestment			TempDBIKInvestSetInvestment;

	if(pPlayer->siMoney >= InvestmentMoney)
	{
		// 이 캐릭터의 ID로 Village Investment List에서의 위치 검사
		SelectIndex = -1;
		for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
		{
			if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType == 0)
			{
				if(strncmp((char*)m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorID, (char*)pPlayer->szID, ON_ID_LENGTH) == 0)
				{
					if((m_lpstVillageInvestment->m_InvestorData[TempCount].m_siInvestorDBAccount == pPlayer->stAccountInfo.siDBAccount) && (m_lpstVillageInvestment->m_InvestorData[TempCount].m_uiInvestorSlot == pPlayer->stAccountInfo.uiSlot))
					{
						SelectIndex = TempCount;
						break;
					}
					else
					{
						pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
						return TRUE;
					}
				}
			}
		}

		//폭투권한을 받지 않았다면.체크.
		if(pPlayer->m_bInvestmentPower == FALSE)
		{
			// 투자한지 한달이 지났는지 검사
			if(SelectIndex != -1)
			{
				TempNowTime = MakeTimeInt(m_lpVillage->clWorld->CurrentTime);
				if(TempNowTime < MakeOneMonthAfter(m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_UpdateTime))
				{
					pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NOTINVESTMENT_TIMEOVER));
					return TRUE;
				}
			}
			// 총투자금액의 10%를 초과하였는지 검사
			TempTestInvestmentMoney = 0;
			for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
			{
				TempTestInvestmentMoney += m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money;
			}
			if(InvestmentMoney > ((TempTestInvestmentMoney * 10) / 100))
			{
				pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NOTINVESTMENT_MANYMONEY));
				return TRUE;
			}
		}

		// Village Investment List에서 없으면 새로운 칸을 만듬
		if(SelectIndex == -1)
		{
			EmptyIndex  = m_lpstVillageInvestment->m_InvestorDataCount;
			SelectIndex = EmptyIndex;
			m_lpstVillageInvestment->m_InvestorDataCount++;

			ZeroMemory(&m_lpstVillageInvestment->m_InvestorData[EmptyIndex], sizeof(stVillageInvestorData));
			
			m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_InvestorType        = 0;
			strncpy((char*)m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_InvestorID, pPlayer->szID, ON_ID_LENGTH);
			m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_UpdateTime          = 0;
			m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_Percent             = 0.0f;
			m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_Money               = 0;
			m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_siInvestorDBAccount = pPlayer->stAccountInfo.siDBAccount;
			m_lpstVillageInvestment->m_InvestorData[EmptyIndex].m_uiInvestorSlot      = pPlayer->stAccountInfo.uiSlot;
		}

		// 받은 메세지를 계산한다.
		m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_Money      += InvestmentMoney;
		m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_Money      += InvestmentSaveMoney;
		m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_UpdateTime =  MakeTimeInt(m_lpVillage->clWorld->CurrentTime);

		ZeroMemory(&TempDBIKInvestSetInvestment, sizeof(TempDBIKInvestSetInvestment));
		TempDBIKInvestSetInvestment.uiVillageCode        = m_lpVillage->uiVillageCode;
		TempDBIKInvestSetInvestment.siInvestorDBAccount  = m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_siInvestorDBAccount;
		TempDBIKInvestSetInvestment.uiInvestorSlot       = m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_uiInvestorSlot;
		CopyMemory(TempDBIKInvestSetInvestment.szInvestorID, m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_InvestorID, 21);
		TempDBIKInvestSetInvestment.uiInvestorType       = m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_InvestorType;
		TempDBIKInvestSetInvestment.moInvestmentMoney		= InvestmentMoney;
		TempDBIKInvestSetInvestment.moInvestmentSaveMoney    = InvestmentSaveMoney;
		TempDBIKInvestSetInvestment.uiInvestorUpdateTime = m_lpstVillageInvestment->m_InvestorData[SelectIndex].m_UpdateTime;
		m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVEST_SETINVESTMENT, LPVOID(&TempDBIKInvestSetInvestment), sizeof(TempDBIKInvestSetInvestment));

		//폭투 권한이 있다면 폭투를 하고 DB에 폭투권한을 삭제하라고 알려준다.
		if(pPlayer->m_bInvestmentPower == TRUE)
		{
			cltGuild*							pclGuild;

			pPlayer->m_bInvestmentPower = FALSE;
			DBIKGuildInvestVillage	TempDBIKGuildInvestVillage;
			TempDBIKGuildInvestVillage.uiVillageCode	=	m_lpVillage->uiVillageCode;
			TempDBIKGuildInvestVillage.uiGuildID		=	pPlayer->uiGuildID;
			pclGuild = m_lpVillage->clWorld->clIGuild.GetGuildByID(pPlayer->uiGuildID);
			
			for(SI16 i = 0; i < ON_MAX_GUILD_RESULT;i++)
			{
				if(pclGuild->m_stGuildResult[i].GetVillageCode() == m_lpVillage->uiVillageCode)
				{
					pclGuild->m_stGuildResult[i].SetCharName("");
					pclGuild->m_stGuildResult[i].SetPercent(0);
					pclGuild->m_stGuildResult[i].SetVillageCode(0);
				}	
			}
			

			m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_DELETE_INVESTMENT,LPVOID(&TempDBIKGuildInvestVillage),sizeof(DBIKGuildInvestVillage));
		}

		// List Update
		VillageInvestment_Updata();

		// Player의 돈 빼기
		pPlayer->DecreaseMoney(InvestmentMoney, FALSE);
		pPlayer->m_MoneySaving -= InvestmentSaveMoney;

		// 성공 Message를 보낸다.
		pPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

		// List Message를 보낸다.
		GovernmentInvestmentGetList(pPlayer);

		return TRUE;
	}

	return FALSE;
}

MONEY SOVillage_Government::GovernmentInvestmentClearInvestment(void)
{
	SI32								TempCount;
	MONEY								mnMoney;
//	DBIKInvestDelAll10PInvestment		TempDBIKInvestDelAll10PInvestment;
//	memset(&TempDBIKInvestDelAll10PInvestment, 0, sizeof(DBIKInvestDelAll10PInvestment));
	
//	TempDBIKInvestDelAll10PInvestment.uiVillageCode = m_lpVillage->uiVillageCode;
	//진짜 최투의 돈을 뺀다.
//	int nBestIndex = m_lpstVillageInvestment->m_BestInvestorInvestmentIndex;
/*	TempDBIKInvestDelAll10PInvestment.siInvestorDBAccount = m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_siInvestorDBAccount;
	TempDBIKInvestDelAll10PInvestment.uiInvestorSlot      = m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_uiInvestorSlot;
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT, &TempDBIKInvestDelAll10PInvestment, sizeof(TempDBIKInvestDelAll10PInvestment));
*/

	printf("최투의 투자금을 빼기 시작");
	int nBestIndex = m_lpstVillageInvestment->m_BestInvestorInvestmentIndex;
	TempCount = m_lpstVillageInvestment->m_InvestorDataCount;
	m_lpstVillageInvestment->m_NowAllInvestmentMoney = 0;

	//최투의 인덱스를 저장해놓는다.(최투가 바뀔수도 있으므로...)
	m_lpstVillageInvestment->m_OldBestInvestorIndex = nBestIndex;
	
	MONEY tempMoney = 0;
	for(int i = 0; i < TempCount; i++)
	{
		if(m_lpstVillageInvestment->m_InvestorData[i].m_InvestorType == 2) continue;
		
		//모든 투자자금을 10%씩 뺀다.
//		m_lpstVillageInvestment->m_InvestorData[i].m_Money = m_lpstVillageInvestment->m_InvestorData[i].m_Money * 0.9;

		DelInvestMoney(i, 1, TRUE);
/*		TempDBIKInvestDelAll10PInvestment.uiVillageCode = m_lpVillage->uiVillageCode;
		TempDBIKInvestDelAll10PInvestment.siInvestorDBAccount = m_lpstVillageInvestment->m_InvestorData[i].m_siInvestorDBAccount;
		TempDBIKInvestDelAll10PInvestment.uiInvestorSlot      = m_lpstVillageInvestment->m_InvestorData[i].m_uiInvestorSlot;
		TempDBIKInvestDelAll10PInvestment.siPercent = 1;
		m_lpVillage->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT, 
			                                            &TempDBIKInvestDelAll10PInvestment, 
														sizeof(TempDBIKInvestDelAll10PInvestment));
		*/
		if(i == nBestIndex)
		{   //최투의 30%를 뺀다.
//			tempMoney = DelInvestMoney(i, 3);
//			tempMoney = m_lpstVillageInvestment->m_InvestorData[i].m_Money * 0.3;
//			m_lpstVillageInvestment->m_InvestorData[i].m_Money *= 0.7;
			
			//보상금은 30%중 40%만 준다.
			mnMoney = DelInvestMoney(i, 3, TRUE) * 0.4;

//			DelInvestMoney(i, 3);

/*			DBIKInvestDelAll10PInvestment             delInvestment;

			memset(&delInvestment, 0, sizeof(DBIKInvestDelAll10PInvestment));
			delInvestment.uiVillageCode       = m_lpVillage->uiVillageCode;
			delInvestment.siInvestorDBAccount = m_lpstVillageInvestment->m_InvestorData[i].m_siInvestorDBAccount;
			delInvestment.uiInvestorSlot      = m_lpstVillageInvestment->m_InvestorData[i].m_uiInvestorSlot;
			delInvestment.siPercent = 3;
			m_lpVillage->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT, 
				                                            &delInvestment, 
															sizeof(TempDBIKInvestDelAll10PInvestment));
*/		}

		m_lpstVillageInvestment->m_NowAllInvestmentMoney += m_lpstVillageInvestment->m_InvestorData[i].m_Money;
	}

	printf("최투의 투자금 빼기 끝\n");
/*	
	// 최투의 50%를 뺀다.
	{   
		MONEY   mn10 = 0, mn30 = 0;

		mn10 = m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_Money;				
		mn30 = mn10 * 0.3;
		m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_Money = mn30;
//		mnMoney = m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_Money * 0.3;
		mnMoney = mn30 * 0.12;

		m_lpstVillageInvestment->m_NowAllInvestmentMoney -= mn30;
	}
*/
	// 최고투자자를 삭제한다.
	//m_lpstVillageInvestment->m_BestInvestorInvestmentIndex          = -1;
	//m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime      = 0;
	//m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = MakeTimeInt(m_lpVillage->clWorld->CurrentTime);

	// 투자자 정보를 갱신한다.
	//VillageInvestment_Updata();
	printf("투자정보 갱신 시작");

	VillageInvestment_ReLoad();

	printf("투자정보 갱신 끝");
	return mnMoney;
}

/*
BOOL SOVillage_Government::GovernmentInvestmentClearInvestment(void)
{
	SI32								TempCount;
	DBIKInvestDelAll10PInvestment		TempDBIKInvestDelAll10PInvestment;
	SI32								TempPercent;

	TempDBIKInvestDelAll10PInvestment.uiVillageCode = m_lpVillage->uiVillageCode;
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT, &TempDBIKInvestDelAll10PInvestment, sizeof(TempDBIKInvestDelAll10PInvestment));


	// 10% 만 삭제한다.
	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		TempPercent = (TempCount == m_lpstVillageInvestment->m_BestInvestorInvestmentIndex)? 8 : 9;

		m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money = ((m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money * TempPercent) / 10);
		if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money < 0) m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money = 0;
	}

	// 최고투자자를 삭제한다.
	m_lpstVillageInvestment->m_BestInvestorInvestmentIndex          = -1;
	m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime      = 0;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = MakeTimeInt(m_lpVillage->clWorld->CurrentTime);

	// 투자자 정보를 갱신한다.
	VillageInvestment_Updata();

	return TRUE;
}
*/

__int64 SOVillage_Government::GetGainingsMoney(void)
{
	return (__int64)m_lpVillage->GetPopulation();
}

SI32 SOVillage_Government::GetInvestmentMinPercent(void)
{
	return 2;
}

SI32 SOVillage_Government::GetInvestmentMaxPercent(void)
{
	return 10;
}

__int64 SOVillage_Government::GetBaseInvestment(void)
{
	return (__int64)m_lpVillage->GetPopulation();
}

__int64 SOVillage_Government::GetDeleteInvestment(void)
{
	return 3000;
}

MONEY SOVillage_Government::GetNowAllInvestment(void)
{
	return m_lpstVillageInvestment->m_NowAllInvestmentMoney;
}

MONEY SOVillage_Government::GetNowAllInvestmentP(SI32 P)
{
	return (GetNowAllInvestment() * P) / 100;
}

char* SOVillage_Government::GetInvestmentMaxInvestmentorName(void)
{
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != -1)
	{
		return (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID;
	}

	return NULL;
}

BOOL SOVillage_Government::GetInvestmentMaxInvestmentorAccountSlot(SI32 *pAccount,UI08 *pSlot)
{
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != -1)
	{
		*pAccount = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_siInvestorDBAccount;
		*pSlot = m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_uiInvestorSlot;
		return TRUE;
	}

	return FALSE;
}


UI32 SOVillage_Government::GetInvestmentMaxInvestmentorGuild(void)
{
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != -1)
	{
		switch(m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorType)
		{
			case 0:			// 사람
				UI32		GuildID;
				SI32		GuildClass;

				if(m_lpVillage->clWorld->clIGuild.GetGuildIDByCharName((char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID, &GuildID, &GuildClass) == FALSE)
				{
					return 0;
				}

				if(GuildID != 0)
				{					
					if(GuildClass == m_lpVillage->clWorld->clIGuildClass.GetJWJCode())
					{
						GuildID = 0;
					}
				}

				return GuildID;
				break;

			case 1:			// 길드
				return m_lpVillage->clWorld->clIGuild.GetGuildID((char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID);
				break;
		}
	}

	return 0;
}

__int64 SOVillage_Government::GetInvestmentMaxInvestmentorMoney(void)
{
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != -1)
	{
		return m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_Money;
	}

	return NULL;
}

BOOL SOVillage_Government::CheckInvestmentMaxInvestmentorName(char* lpID, int Type)
{
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != -1)
	{
		if(m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorType == Type)
		{
			if(strncmp((char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID, lpID, ON_ID_LENGTH) == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

stVillageInvestorData* SOVillage_Government::GetInvestmentDataByID( char* lpID )
{
	int		TempCount;

	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType == 0)
		{
			if(strncmp((char*)m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorID, lpID, 21) == 0)
			{
				return &m_lpstVillageInvestment->m_InvestorData[TempCount];
			}
		}
	}

	return NULL;
}


BOOL SOVillage_Government::WriteDescription(SOPlayer *pPlayer, char* Descript)
{
	CHAR						TempBuf[1024];
	DBIKVillageDescript			*pVillageDescript;
	CHAR						*DescPointer;
	pVillageDescript			=	(DBIKVillageDescript*)	TempBuf;

	// 최고투자자도 아니고 운영자도 아니면 안내문을 적을 수 없다.
	if ( ( CheckInvestmentMaxInvestmentorName( pPlayer->szID, 0 ) == FALSE ) && pPlayer->USGrade != ON_USERGRADE_ADMIN5 )		//뒤에 ON_USERGRADE_ADMIN5 이상민 수정(2003. 4. 22)
	//최고 투자가 이거나, 운영자이면 수정 가능. 아니면 리턴
	//if ( ! (CheckInvestmentMaxInvestmentorName( pPlayer->szID, 0 )  || pPlayer->USGrade == ON_USERGRADE_ADMIN5 ))		//뒤에 ON_USERGRADE_ADMIN5 이상민 수정(2003. 4. 22)
	{
		return ON_RET_DESCRIPT_NOTBEST;
	}
	
		

	ZeroMemory(TempBuf, 1024);
	pVillageDescript->VillageCode	=	m_uiVillageCode;
	pVillageDescript->Length		=	strlen(Descript);
	DescPointer		=	(CHAR*)TempBuf + sizeof(DBIKVillageDescript);
	strcpy(DescPointer, Descript );

	/*
	if( clWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_WRITE_DESCRIPT, TempBuf, NULL) == FALSE )
	{		
		return ON_RET_DESCRIPT_NO;
	}*/
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_WRITE_DESCRIPT, TempBuf, sizeof(TempBuf));
	
	strcpy( m_lpVillage->szDescription, Descript);
	m_lpVillage->szDescription[MAX_DESCRIPT_LENGTH-1]	=	'\0';

	return ON_RET_DESCRIPT_OK;
}

stCapital*	SOVillage_Government::GetVillageCapital()
{
	return m_lpVillage->VillageCapital;
}

void SOVillage_Government::SendVillageReqMaxVillageDefenceInvestment(SOPlayer *pPlayer)
{
	VDEF		NowDef;
	VDEF		MaxDef;
	MONEY		DefPerMoney;
	MONEY		MaxMoney;

	NowDef      = m_lpVillage->vdDefence;
	MaxDef      = m_lpVillage->vdMaxDefence;
	DefPerMoney = 1;
	MaxMoney    = GetNowAllInvestment() - 100000;

	if(MaxMoney < 0) MaxMoney = 0;

	pPlayer->SendFieldMsg(ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&NowDef), LPVOID(&MaxDef), LPVOID(&DefPerMoney), LPVOID(&MaxMoney));
}

void SOVillage_Government::SendVillageReqSetVillageDefenceInvestment(SOPlayer *pPlayer, MONEY InvestmentMoney)
{
	// 최고 투자자인지 검사
	// 행수(최고투자자)이거나 대행수인지 검사
	if(m_lpstVillageInvestment->m_BestInvestorInvestmentIndex != -1)
	{
		if((strncmp((char*)pPlayer->szID, (char*)m_lpstVillageInvestment->m_InvestorData[m_lpstVillageInvestment->m_BestInvestorInvestmentIndex].m_InvestorID, ON_ID_LENGTH) == 0)
			|| ((pPlayer->GetClassInGuild() == pPlayer->pMyWorld->clIGuildClass.GetDHSCode()) && (pPlayer->uiGuildID == m_lpVillage->uiGuildID)))
		{
			if(InvestmentMoney > 0)
			{
				// 투자할 돈이 되는지 검사
				if(InvestmentMoney <= (GetNowAllInvestment() - 100000))
				{
					VDEF		TempUpDefence;

					// 투자할 방어력이 넘는지 검사
					TempUpDefence = InvestmentMoney / 1;
					if((TempUpDefence + m_lpVillage->vdDefence) <= m_lpVillage->vdMaxDefence)
					{
						// 마을 수리
						m_lpVillage->RepairVillageDefence(TempUpDefence, FALSE);

						// 투자자들에게 돈 빼기
						DBIKUpDurabilityOfVillage		TempDBIKUpDurabilityOfVillage;
						SI32							TempRate1;
						SI32							TempRate2;
						SI32							TempCount;
						MONEY							TempRate1_i64;
						MONEY							TempRate2_i64;
						
						TempRate1_i64 = GetNowAllInvestment();	// 마을 총투자금
						TempRate2_i64 = InvestmentMoney;		// 방어력에 투자한 금액
						while(TempRate1_i64 > 2100000000)
						{
							TempRate1_i64 /= 2;
							TempRate2_i64 /= 2;
						}

						TempRate1 = TempRate1_i64;						// DB로 날릴 값
						TempRate2 = TempRate2_i64;						// DB로 날릴 값
						//TempRate2_i64 = TempRate1_i64 - TempRate2_i64;	// 방어하고 남은 금액의 비율 계산 변수

						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 투자금이 커질 경우 (투자금 * T1) / T2 의 공식은 overflow를 초래한다. 주작의 부산 투자금이 사라지는 경우가 그것이다 -_-; (곱셈에서 overflow)
						// 아래 "비율만큼 삭제" 부분에서 (투자금 * 방어후남은금액 / 방어전금액) 공식을 쓰지 말고
						// 투자금 - (투자금 * 방어에소모된금액 / 방어전금액) 공식을 쓰도록 바꾼다.

						for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
						{
							// 비율만큼 삭제.. TempRate2_i64값은 작으므로 곱해도 된다.
							m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money -= m_lpstVillageInvestment->m_InvestorData[TempCount].m_Money  * TempRate2_i64 / TempRate1_i64;
						}

						TempDBIKUpDurabilityOfVillage.siDBAccount    = pPlayer->stAccountInfo.siDBAccount;
						TempDBIKUpDurabilityOfVillage.uiDBSlot       = pPlayer->stAccountInfo.uiSlot;
						TempDBIKUpDurabilityOfVillage.uiVillageCode  = m_lpVillage->uiVillageCode;
						TempDBIKUpDurabilityOfVillage.siUpDurability = TempUpDefence;
						TempDBIKUpDurabilityOfVillage.siRate1        = TempRate1;
						TempDBIKUpDurabilityOfVillage.siRate2        = TempRate2;
						m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDURABILITYOFVILLAGE, LPVOID(&TempDBIKUpDurabilityOfVillage), sizeof(TempDBIKUpDurabilityOfVillage));

						VillageInvestment_Updata();

						// 메세지 전송
						pPlayer->SendFieldMsg(ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&m_lpVillage->vdDefence));
					}
					else
					{
						pPlayer->SendFieldMsg(ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
					}
				}
				else
				{
					pPlayer->SendFieldMsg(ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
				}
			}
			else
			{
				pPlayer->SendFieldMsg(ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			}
		}
		else
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

BOOL SOVillage_Government::InvestmentGetOut(SOPlayer* lpPlayer, MONEY GetOutMoney)
{
	if(InvestmentGetOutF() == 0)
	{
		lpPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT, SO_SFM_ONLYME, LPVOID(2));
		return FALSE;
	}

	int			TempCount;
	int			TempIndex;
	MONEY		TempTotalMoney;
	MONEY		TempGetOutMoney;
	MONEY		TempTexMoney;
	int			TempBestInvestorInvestmentIndex;

	// 사람 찾기
	TempIndex = -1;
	for(TempCount = 0; TempCount < m_lpstVillageInvestment->m_InvestorDataCount; TempCount++)
	{
		if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_InvestorType == 0)
		{
			if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_siInvestorDBAccount == lpPlayer->stAccountInfo.siDBAccount)
			{
				if(m_lpstVillageInvestment->m_InvestorData[TempCount].m_uiInvestorSlot == lpPlayer->stAccountInfo.uiSlot)
				{
					TempIndex = TempCount;
					break;
				}
			}
		}
	}

	if(TempIndex == -1)
	{
		lpPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT, SO_SFM_ONLYME, LPVOID(2));
		return FALSE;
	}

	TempTotalMoney  = GetOutMoney;
	TempTexMoney    = (GetOutMoney * InvestmentGetOutTexP()) / 100;
	TempGetOutMoney = TempTotalMoney - TempTexMoney;
	
	// 돈이 충분한지 검사
	if(m_lpstVillageInvestment->m_InvestorData[TempIndex].m_Money < TempTotalMoney)
	{
		lpPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT, SO_SFM_ONLYME, LPVOID(1));
		return FALSE;
	}

	// 인출함
	m_lpstVillageInvestment->m_InvestorData[TempIndex].m_Money -= TempTotalMoney;
	lpPlayer->m_MoneySaving                                    += TempGetOutMoney;

	DBIKGetOutInvestment		TempDBIKGetOutInvestment;

	TempDBIKGetOutInvestment.uiVillageCode    = m_lpVillage->uiVillageCode;
	TempDBIKGetOutInvestment.siDBAccount      = lpPlayer->stAccountInfo.siDBAccount;
	TempDBIKGetOutInvestment.uiDBSlot         = lpPlayer->stAccountInfo.uiSlot;
	TempDBIKGetOutInvestment.GetOutInvestment = TempGetOutMoney;
	TempDBIKGetOutInvestment.Tex              = TempTexMoney;
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GETOUTINVERTMENT, LPVOID(&TempDBIKGetOutInvestment), sizeof(TempDBIKGetOutInvestment));
	TempBestInvestorInvestmentIndex = m_lpstVillageInvestment->m_BestInvestorInvestmentIndex;

	lpPlayer->SendFieldMsg(ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT, SO_SFM_ONLYME, LPVOID(0), LPVOID(&TempGetOutMoney), LPVOID(&TempTexMoney));

	VillageInvestment_Updata();

	// 투자 리스트를 보내줌
	GovernmentInvestmentGetList(lpPlayer);

	//=====================================================================================
	// 현재 마을에 최고 투자자 검사
	VillageInvestment_CheckBestInvestmentor();



	//=====================================================================================
	// 길드 처리
	UI32		TempMaxInvestmentorGuild;

	TempMaxInvestmentorGuild = GetInvestmentMaxInvestmentorGuild();
	if(m_lpVillage->uiGuildID != 0)
	{
		//=====================================================================================
		// 현재 마을에 길드가 있을경우
		// 최고 투자자 검사
		if(TempMaxInvestmentorGuild != m_lpVillage->uiGuildID)
		{
			// 최고 투자자가 마을의 길드원이 아닐때, 그 마을을 길드에서 해방시킨다.
			m_lpVillage->clWorld->clIGuild.DelVillage(m_lpVillage->uiGuildID, m_lpVillage, FALSE);
			m_lpVillage->uiGuildID = 0;
		}
	}

	if(m_lpVillage->uiGuildID == 0)
	{
		//=====================================================================================
		// 현재 마을에 길드가 없을경우
		// 최고 투자자가 있을경우, 최고 투자자가 길드에 가입돼어 있으면, 마을을 그 길드의 소속으로 바꿔게 함
		if(TempMaxInvestmentorGuild != 0)
		{
			m_lpVillage->clWorld->clIGuild.AddVillage(TempMaxInvestmentorGuild, m_lpVillage, FALSE);
		}
	}

	// 최고 투자자가 바꿔면 보내줌
	if(TempBestInvestorInvestmentIndex != m_lpstVillageInvestment->m_BestInvestorInvestmentIndex)
	{
		lpPlayer->RecvRequestVillageInfo();
	}

	return TRUE;
}

BYTE SOVillage_Government::InvestmentGetOutF(void)
{
	return 1;
}

BYTE SOVillage_Government::InvestmentGetOutTexP(void)
{
	return 70;
}

BOOL SOVillage_Government::CheckBestInvestment(char *id, MONEY GetOutMoney)
{
	//최고투자자의 정보를 가져온다.
	int nBestIndex = m_lpstVillageInvestment->m_BestInvestorInvestmentIndex;
	if(nBestIndex < 0) return FALSE;

	//최고 투자자가 상단에 가입되어 있는지 검사.
	if(m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_InvestorType == 1) return FALSE;

	//최고 투자자가 된 시간을 얻어온다.
	SOnlineTime soTime = MakeTimeOnlineTime(m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_UpdateTime);

	//현재 시간과 비교를 한다. 상단에 가입 없이 5일이 지났으면 최투에서 빠진다.
	if(CheckInHourPitchDay(soTime, m_lpVillage->clWorld->CurrentTime, 5) == FALSE) return FALSE;

	//최투의 돈을 다 빼서 전장에 넣어준다.
	MONEY mnBestMoney = 0;
	GetOutMoney = m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_Money;
	m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_Money = 0;
	strcpy(id, (char*)m_lpstVillageInvestment->m_InvestorData[nBestIndex].m_InvestorID);

	//새로운 최고 투자자를 찾는다.
	int i = 0, nTempIndex = -1;
	for(; i < m_lpstVillageInvestment->m_InvestorDataCount; i++)
	{
		if(i == nBestIndex) continue;
		if(m_lpstVillageInvestment->m_InvestorData[i].m_InvestorType != 2)
		{
			if(mnBestMoney < m_lpstVillageInvestment->m_InvestorData[i].m_Money)
			{
				nTempIndex = i;
				mnBestMoney = m_lpstVillageInvestment->m_InvestorData[i].m_Money;
			}
		}
	}


	int nNowTime = MakeTimeInt(m_lpVillage->clWorld->CurrentTime);
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmentIndex     = nTempIndex;
	m_lpstVillageInvestment->m_Real_BestInvestorInvestmenUpdateTime = nNowTime;

	m_lpstVillageInvestment->m_BestInvestorInvestmentIndex = nTempIndex;
	m_lpstVillageInvestment->m_BestInvestorInvestmenUpdateTime = nNowTime;


	DBIKInvestorSetInfo TempDBIKInvestorSetInfo;
	ZeroMemory(&TempDBIKInvestorSetInfo, sizeof(DBIKInvestorSetInfo));
	TempDBIKInvestorSetInfo.uiVillageCode	=	m_uiVillageCode;

	if(nTempIndex < 0)
	{
		strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, "");
	    TempDBIKInvestorSetInfo.uiNowBestInvestorType = 2;
	    TempDBIKInvestorSetInfo.uiNowBestInvestorUpdateTime = nNowTime;

		strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID, "");
		TempDBIKInvestorSetInfo.uiRealBestInvestorType = 2;
		TempDBIKInvestorSetInfo.uiRealBestInvestorUpdateTime = nNowTime;
	}
	else
	{
		strcpy((char*)TempDBIKInvestorSetInfo.szNowBestInvestorID, 
			   (char*)m_lpstVillageInvestment->m_InvestorData[nTempIndex].m_InvestorID);
	    
		TempDBIKInvestorSetInfo.uiNowBestInvestorType = m_lpstVillageInvestment->m_InvestorData[nTempIndex].m_InvestorType;
	    TempDBIKInvestorSetInfo.uiNowBestInvestorUpdateTime = nNowTime;

		
		strcpy((char*)TempDBIKInvestorSetInfo.szRealBestInvestorID, 
			   (char*)m_lpstVillageInvestment->m_InvestorData[nTempIndex].m_InvestorID);
		
		TempDBIKInvestorSetInfo.uiRealBestInvestorType = m_lpstVillageInvestment->m_InvestorData[nTempIndex].m_InvestorType;
		TempDBIKInvestorSetInfo.uiRealBestInvestorUpdateTime = nNowTime;
	}

	m_lpVillage->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_INVESTORSETINFO, 
													LPVOID(&TempDBIKInvestorSetInfo), sizeof(TempDBIKInvestorSetInfo));
	
		
	return TRUE;
}

//마을 함락시 저장해놓았던 최투의 인덱스를 가져온다.
SI32 SOVillage_Government::GetOldBestInvestIndex()
{
	return m_lpstVillageInvestment->m_OldBestInvestorIndex;
}

//uiIndex의 해당하는 투자자의 투자금을 uiPercent만큼 감소시킨다. 
//bFlag가 TRUE인경우 투자금으 변경하고 DB에 저장한다.
MONEY SOVillage_Government::DelInvestMoney(UI32 uiIndex, UI16 uiPercent,  BOOL bFlag)
{
	if(bFlag)
	{
		double tempPercent = 1 - (uiPercent * 0.1);
		MONEY mnTempMoney = m_lpstVillageInvestment->m_InvestorData[uiIndex].m_Money * tempPercent;
		m_lpstVillageInvestment->m_InvestorData[uiIndex].m_Money = mnTempMoney;	

		DBIKInvestDelAll10PInvestment		delInvestment;
		memset(&delInvestment, 0, sizeof(DBIKInvestDelAll10PInvestment));

		delInvestment.uiVillageCode       = m_lpVillage->uiVillageCode;
		delInvestment.siInvestorDBAccount = m_lpstVillageInvestment->m_InvestorData[uiIndex].m_siInvestorDBAccount;
		delInvestment.uiInvestorSlot      = m_lpstVillageInvestment->m_InvestorData[uiIndex].m_uiInvestorSlot;
		delInvestment.siPercent           = uiPercent;
		
		m_lpVillage->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT, 
														&delInvestment, 
														sizeof(DBIKInvestDelAll10PInvestment));
	}

	return m_lpstVillageInvestment->m_InvestorData[uiIndex].m_Money * (uiPercent * 0.1);
}
