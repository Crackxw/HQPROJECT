#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "AuctionList.h"
#include "AuctionMarket.h"

#include "CSiegeUniteTable.h"

// 정적변수들.
clVillageAttackInfo		SOVillage::clVAInfo;	// 마을의 공격 정보 
SOWorld					*SOVillage::clWorld;	




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-08-14 오후 5:44:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage::SOVillage()
{	
	ZeroMemory(szHNSID, sizeof(szHNSID));							// 행수 아이디 초기화 
	ZeroMemory(VillageCapital, sizeof(VillageCapital));				// 10년동안의 마을의 수익 초기화 
	
//	pstBankData						=	NULL;						// 전장의 데이타.
	pstINNData						=	NULL;						// 주막의 데이타.	
	

	siVillageInOutPlayerYetYear		=	0;							// 작년의 마을 출입자 수 
	siVillageInOutPlayerNowYear		=	0;							// 올해의 마을 출입자 수 

	uiGuildID						=	0;							// 마을을 소유하고 있는 길드의 아이디 
	siGuildFlag						=	0;							// 길들의 깃발 아이디 

	CurrentCapital					=	VillageCapital;				// 올해의 마을 수익 
	dwLastAOVillageBeAttacked		=	0;							// 마을이 공격당했을때 가장 최근에 서버에 알린 시간 


	m_pMarket						=	NULL;
	m_pIndustrialComplexs			=	NULL;
	m_Barrack						=	NULL;
	m_Hospital						=	NULL;
	m_Wharf							=	NULL;
	m_Government					=	NULL;	
	m_pBank							=	NULL;
	m_bIsFall                       =   FALSE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 소멸자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOVillage::~SOVillage()
{
/*	if(pstBankData)
	{
		delete pstBankData;
		pstBankData		=	NULL;
	}*/

	if(pstINNData)
	{
		delete pstINNData;
		pstINNData		=	NULL;
	}

	if( m_pMarket )
	{
		delete m_pMarket;
		m_pMarket	=	NULL;
	}

	if( m_pIndustrialComplexs )
	{
		delete	m_pIndustrialComplexs;
		m_pIndustrialComplexs	=	NULL;
	}

	if( m_Barrack )
	{
		delete m_Barrack;
		m_Barrack	=	NULL;
	}

	if( m_Hospital )
	{
		delete m_Hospital;
		m_Hospital	=	NULL;
	}

	if( m_Wharf )
	{
		delete m_Wharf;
		m_Wharf		=	NULL;
	}

	if( m_Government )
	{
		delete m_Government;
		m_Government	=	NULL;
	}

	if( m_pBank )
	{
		delete m_pBank;
		m_pBank				=	NULL;
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을을 초기화 한다.(DB로 부터 현 상황까지 받아온다.)
//	수정 일자	:	2002-08-14 오후 5:46:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::Init(VillageHeader *pVillageHeader)
{	
	DBIKVillageEnterPlauerGet		TempDBIKVillageEnterPlauerGet;
	DBIKVillageEnterPlauerGetInfo	TempDBIKVillageEnterPlauerGetInfo;

	
	// 마을의 기본 정보를 설정한다.
	
	// 마을의 X, Y 좌표를 기준으로 아디디를 설정한다 
	uiVillageCode	=	(pVillageHeader->siX << 16) | pVillageHeader->siY; 	
	siVillageX		=	pVillageHeader->siX;
	siVillageY		=	pVillageHeader->siY;
	uiVillageCode	=	pVillageHeader->uiVillageCode;
	uiMapID			=	pVillageHeader->siMapIndex;
	uiVillageName	=	pVillageHeader->siVillageName;

	m_uiLimitMinPopulation	=	pVillageHeader->uiMinPop;
	m_uiLimitMaxPopulation	=	pVillageHeader->uiMaxPop;

	DBIKVillageInfo	TempVillageInfo;

	strcpy(TempVillageInfo.szName,clWorld->clOnlineText.Get((UI32)pVillageHeader->siVillageName));
	TempVillageInfo.siX				=	pVillageHeader->siX;
	TempVillageInfo.siY				=	pVillageHeader->siY;
	TempVillageInfo.uiViillageCode	=	pVillageHeader->uiVillageCode;
	TempVillageInfo.siPopulation	=	pVillageHeader->uiPop;

	if(pVillageHeader->uiNation == ON_COUNTRY_KOREA)
		TempVillageInfo.szNation = 'K';
	if(pVillageHeader->uiNation == ON_COUNTRY_JAPAN)
		TempVillageInfo.szNation = 'J';
	if(pVillageHeader->uiNation == ON_COUNTRY_CHINA)
		TempVillageInfo.szNation = 'C';
	if(pVillageHeader->uiNation == ON_COUNTRY_TAIWAN)
		TempVillageInfo.szNation = 'T';

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGE_INFO,&TempVillageInfo,sizeof(DBIKVillageInfo));

/*
	clWorld->clServer->WriteInfo( "VillageInfo.txt", "Code : [ %u ], X : [ %d ], Y : [ %d ], Name : [ %s ]",
								uiVillageCode, siVillageX, siVillageY, clWorld->clOnlineText.Get( uiVillageName ) );
*/	
	// 이 마을 있는 건물들을 저장한다.
	memcpy(siStructureKinds, pVillageHeader->siStructureKinds, sizeof(siStructureKinds));

	// DB에서 이 마을을 소유하고 있는 상단을 얻어온다.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEGUILDID_GET, LPVOID( uiVillageCode ), LPVOID( &uiGuildID ) ) == FALSE )		return FALSE;		
	
	if(uiGuildID != 0)
	{
		// 깃발의 아이디를 얻어온다 
		siGuildFlag	= clWorld->clIGuild.GetFlag(uiGuildID);

		// 이 마을을 해당 상단에 소속 시킨다.
		clWorld->clIGuild.AddVillage(uiGuildID, this, TRUE);
	}
	else
		siGuildFlag	=	0;

	// DB에서 행수의 이름을 얻어온다.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEHNSNAME_GET, LPVOID( uiVillageCode ), LPVOID( szHNSID ) ) == FALSE )			return FALSE;
	
	// DB에서 마을의 방어력을 얻어온다.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEDEFENCE_GET, LPVOID( uiVillageCode ), LPVOID( &vdDefenceInDB ) ) == FALSE )	return FALSE;		

	// 방어력을 저장한다.
	vdDefence	=	vdDefenceInDB; 
	
	// 그 마을의 인구를 DB로 부터 얻어온다.
	// 최대 마을 인구수 마을 방어력 마을인구수 마을 수비병수를 저장한다.
	if(GetDBPopulation() == FALSE) return FALSE;
	
	// 이 마을에 있는 모든 건물들을 초기화 한다.
	if( InitStructure( pVillageHeader ) == FALSE )		return	FALSE;	

	// 마을 안내문과 국가 코드들 얻어온다
	memset(szDescription,0, sizeof(szDescription));		
	GetDescription();									
	GetNationCode();					

	// DB에서 마을의 10 년간 이익을 얻어온다.
	GetVillageCapital();

	// DB 에서 마을 연간 출입자 수를 얻어옴
	siVillageInOutPlayerYetYear = 0;
	siVillageInOutPlayerNowYear = 0;

	m_siVillageAttackType	  = ON_VILLAGE_ATTACK_NONE;
	m_dwPrevAttackdelay		  =	0;	 //다음 기술을 사용하기 까지 딜레이
	m_dwPrevHitTime			  = 0;  //기술의 발생 시간 체크.
	m_dwSkillDelayTime		  = 0;
	m_bAttackTypeDelay        = FALSE;

	m_bVillageAttack			=	FALSE;
	m_dwVillageDamageUpdateTime	=	0;

	m_bVillageInLimit			=	FALSE;
//	strcpy(m_szCharName,"");
	memset(m_szCharName, 0, 100 * 11);


	TempDBIKVillageEnterPlauerGet.uiVillageCode     =	uiVillageCode;
	TempDBIKVillageEnterPlauerGetInfo.uiPlayerNum = 0;

	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEENTERPLAYERGET, LPVOID(&TempDBIKVillageEnterPlauerGet), LPVOID(&TempDBIKVillageEnterPlauerGetInfo));
	siVillageInOutPlayerYetYear = TempDBIKVillageEnterPlauerGetInfo.uiPlayerNum;

	
	memset(m_stAttackGuild, 0, sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_VILLAGE_DAMAGE,LPVOID(uiVillageCode),LPVOID(this));

	InitDamageAttackedByGuild();

	// 다가올 장날을 설정한다. (서버를 켠후 최초의 장날이 되겠지..)
	InitMarketDay();

	return TRUE;
}

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	은행정보를 DB에서 업데이트한다.(시동시 & 매일)
//	수정 일자	:	2002-08-14 오후 5:55:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::UpdateBankInfo()
{	
	DBIKBankInfo	BankInfo;
	
	BankInfo.uiVillageCode = uiVillageCode;

	if(clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKINFO, 
									  LPVOID(&BankInfo), NULL) == FALSE) return FALSE;

	pstBankData->fInterest			=	BankInfo.siInterest;
	pstBankData->siBankCapital		=	BankInfo.siBankCapital;
	pstBankData->siTradeCapital		=	BankInfo.siTradeCapital;
	
	return	TRUE;

}
*/

//----------------------------------------------------------------------------------------------------------------
//	설명	:	플레이어가 마을에서 로그 아웃 했을 경우 호출해 준다.
//----------------------------------------------------------------------------------------------------------------
VOID	SOVillage::PlayerLogOut(SOPlayer *pPlayer)
{
	// 건물에서도 빠져나온다.
	LeaveStructure( pPlayer );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Polling
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::Poll()
{
	SI32						TempCount;
	SI32						TempCount1;
	SI32						TempCount2;
	UI16						TempUserCount;
	SI32						TempMaxTicketCount;
	SI32						TempMaxTicketCountKind[128];
	SI32						TempMaxTicketCountUser[128];
	UI16						TempUserInfo[1024];
	SI32						TempUserTicketInfo[1024];
	UI16						TempUserInfo1;
	SI32						TempUserTicketInfo1;
	TicketHeader*				lpTicketHeader;
	SI32						i;	
	DBIKVillageEnterPlauerSet	TempDBIKVillageEnterPlauerSet;	


	if(clWorld->bDayChanged == TRUE)
	{
		// 장날이 되면 마을의 아이템 수량을 변화시키고 다음 장날을 설정한다. (마을마다 장날이 다름)
		//if((clWorld->CurrentTime.Day == 1) || (clWorld->CurrentTime.Day == 8) || (clWorld->CurrentTime.Day == 15) || (clWorld->CurrentTime.Day == 22))
		if( GetNextMarketDay() == clWorld->CurrentTime.Day )
		{
			m_pMarket->ProcessProduceReduceItem( uiPopulation );
			SetNextMarketDay();															// 다음 장날을 설정한다.
		}

		// 만약에 월이 바꼈다면 아이템의 특성에 따라 생산 및 소비한다.
		if(clWorld->bMonthChanged == TRUE)
		{
			//VillageInvestment_Updata(1);
			if(clWorld->bYearChanged == TRUE)
			{
				for(i = ON_STATISTICS_YEAR; i > 1; i--)
					memmove(&VillageCapital[i-1], &VillageCapital[i-2], sizeof(stCapital));

//				memcpy(&VillageCapital[0], CurrentCapital, sizeof(stCapital));

				// 올해의 이익금 초기화.
				memset(CurrentCapital, 0, sizeof(stCapital));
				CurrentCapital->Year	=	clWorld->CurrentTime.Year;
			}
		}

		// 하루가 지났다.

		// 생산 시설물이 있다면 일주일동안 가동 안한 시설물은 없애고 
		// 인구수 / 500 만큼 노동을 해준다 최고 1000.
		if( DoesFactoryExist() == TRUE )
		{
			UI32 uiNPCLabor;
			uiNPCLabor = min(1000,uiPopulation / 500);
			m_pIndustrialComplexs->DayChanged( clWorld ,uiNPCLabor);
		}
		

		// 마을에 은행이 있다면 pstBankData 업데이트 함수...
//		UpdateBankInfo();

		// 하루마다 현재 마을을 다녀간 사람들 저장
		TempDBIKVillageEnterPlauerSet.uiVillageCode		=	uiVillageCode;
		TempDBIKVillageEnterPlauerSet.uiPlayerNum		=	GetVillageInOutPlayer();
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEENTERPLAYERSET, LPVOID(&TempDBIKVillageEnterPlauerSet), sizeof(TempDBIKVillageEnterPlauerSet));
	}
	if(clWorld->bHourChanged == TRUE)
	{
		if(clWorld->CurrentTime.Hour >= 12)
		{
			SI16	TempHour;

			TempHour = (uiVillageCode % 10) + 12;

			if(clWorld->CurrentTime.Hour == TempHour)
			{
				if(clWorld->IsAttackVillageTimeZone() == FALSE)
				{
					//=====================================================================================
					// 현재 마을에 최고 투자자 검사
					m_Government->VillageInvestment_CheckBestInvestmentor();

					//=====================================================================================
					// 길드 처리
					UI32		TempMaxInvestmentorGuild;

					TempMaxInvestmentorGuild = m_Government->GetInvestmentMaxInvestmentorGuild();				

					if(uiGuildID != 0)
					{
						//=====================================================================================
						// 현재 마을에 길드가 있을경우
						// 최고 투자자 검사
						if(TempMaxInvestmentorGuild != uiGuildID)
						{
							// 최고 투자자가 마을의 길드원이 아닐때, 그 마을을 길드에서 해방시킨다.
							clWorld->clIGuild.DelVillage(uiGuildID, this, FALSE);
							uiGuildID = 0;
						}					
					}
           
					if(uiGuildID == 0)
					{
						//=====================================================================================
						// 현재 마을에 길드가 없을경우
						// 최고 투자자가 있을경우, 최고 투자자가 길드에 가입돼어 있으면, 마을을 그 길드의 소속으로 바꿔게 함
						if(TempMaxInvestmentorGuild != 0)
						{
							clWorld->clIGuild.AddVillage(TempMaxInvestmentorGuild, this, FALSE);
						}
					}
				}
			}
		}

		// 부두 대기실 처리(출항시간이 돼면 그 표를 가진 사람들을 골라서 출항시킴)
		TempUserCount = 0;
		for(TempCount = 0; TempCount < ON_MAX_CONNECTION; TempCount++)
		{
			if(clWorld->clPlayer[TempCount])
			{
				if(clWorld->clPlayer[TempCount]->uiStructerInPos == STRUCTERPOS_WHARFWAITTINGROOM)
				{
					if(clWorld->clPlayer[TempCount]->stAccountInfo.pVillage == this)
					{
						if(clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind != -1)
						{
							// 출항시간인지 검사
							TicketHeader*		lpTicketHeader;

							lpTicketHeader = clWorld->clTicketParser.GetTicket(clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind);
							if(lpTicketHeader)
							{
								if((clWorld->CurrentTime.Hour % lpTicketHeader->siStartTime) == 0)
								{
									// 표를 삭제
									DBIKResetTicket			TempDBIKResetTicket;

									TempDBIKResetTicket.DBAccount    = clWorld->clPlayer[TempCount]->stAccountInfo.siDBAccount;
									TempDBIKResetTicket.uiSlot       = clWorld->clPlayer[TempCount]->stAccountInfo.uiSlot;
									clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_RESETTICKET, LPVOID(&TempDBIKResetTicket), sizeof(TempDBIKResetTicket));
									clWorld->clPlayer[TempCount]->uiStructerInPos = STRUCTERPOS_NONE;

									// 출항 Message
									//clWorld->clPlayer[TempCount]->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

									// 이동하려는 배 List에 추가
									TempUserTicketInfo[TempUserCount] = clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind;
									TempUserInfo[TempUserCount]       = clWorld->clPlayer[TempCount]->uiAccount;
									TempUserCount++;

									// 사람들을 마을 밖으로 이동
									clWorld->clPlayer[TempCount]->LeaveVillage();
								}
							}
						}
					}
				}
			}
		}

		// 이동하는 사람들이 있으면, 배 List에 추가
		if(TempUserCount)
		{
			// 우선 배 Kind 로 정렬
			for(TempCount = 0; TempCount < (TempUserCount - 1); TempCount++)
			{
				for(TempCount1 = (TempCount + 1); TempCount1 < TempUserCount; TempCount1++)
				{
					if(TempUserTicketInfo[TempCount1] < TempUserTicketInfo[TempCount])
					{
						TempUserInfo1                  = TempUserInfo[TempCount1];
						TempUserTicketInfo1            = TempUserTicketInfo[TempCount1];

						TempUserInfo[TempCount1]       = TempUserInfo[TempCount];
						TempUserTicketInfo[TempCount1] = TempUserTicketInfo[TempCount];

						TempUserInfo[TempCount]        = TempUserInfo1;
						TempUserTicketInfo[TempCount]  = TempUserTicketInfo1;
					}
				}
			}

			// 정렬된 Kind를 검사
			TempMaxTicketCount        = 0;
			TempMaxTicketCountKind[0] = TempUserTicketInfo[0];
			TempMaxTicketCountUser[0] = 0;
									
			for(TempCount = 1; TempCount < TempUserCount; TempCount++)
			{
				if(TempMaxTicketCountKind[TempMaxTicketCount] != TempUserTicketInfo[TempCount])
				{
					TempMaxTicketCount++;
					TempMaxTicketCountKind[TempMaxTicketCount] = TempUserTicketInfo[TempCount];
					TempMaxTicketCountUser[TempMaxTicketCount] = TempCount;
				}
			}
			TempMaxTicketCount++;
			TempMaxTicketCountKind[TempMaxTicketCount] = -1;
			TempMaxTicketCountUser[TempMaxTicketCount] = TempUserCount;

			// 배 이동
			for(TempCount = 0; TempCount < TempMaxTicketCount; TempCount++)
			{
				lpTicketHeader = clWorld->clTicketParser.GetTicket(TempMaxTicketCountKind[TempCount]);
				if(lpTicketHeader)
				{
					UI16	StartVillageCode;
					UI16	EndVillageCode;

					StartVillageCode = 0;
					EndVillageCode   = 0;
					for(TempCount2 = 0; TempCount2 < clWorld->siTotalVillageNum; TempCount2++)
					{
						if(clWorld->pclVillage[TempCount2].uiVillageCode == lpTicketHeader->uiStartVillageCode)
						{
							StartVillageCode = clWorld->pclVillage[TempCount2].uiVillageCode;
						}
						if(clWorld->pclVillage[TempCount2].uiVillageCode == lpTicketHeader->uiEndVillageCode)
						{
							EndVillageCode = clWorld->pclVillage[TempCount2].uiVillageCode;
						}
					}
					
					clWorld->clSOShip.Init(uiMapID
						, lpTicketHeader->siShipID
						, StartVillageCode
						, EndVillageCode
						, lpTicketHeader->siShipPassID
						, TempMaxTicketCountUser[TempCount + 1] - TempMaxTicketCountUser[TempCount]
						, (UI16*)&TempUserInfo[TempMaxTicketCountUser[TempCount]]);
				}
			}
		}

		// 마을의 방어력을 DB에 저장한다. (공성 시간일경우에만)
		if( clWorld->IsAttackVillageTimeZone() == TRUE )
		{			
			SaveDefenceToDB();
//			SaveSoldierToDB();
		}
	}

	//일정 시간이 되면 Village Attack Type을 원래대로 수정한다.
	if(GetVillageAttackType() != ON_VILLAGE_ATTACK_NONE)
	{
		if(GetVillageAttackType() == ON_VILLAGE_ATTACK_OIL)
		{
			if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(10000))
			{
				 //마을의 AttackType을 원래대로 돌린다.
				 InitializeVillageAttackType();
				 //마을 주위에 플레이어들에게 AttackType이 원래대로 돌아왔다는것을 알린다.
				 SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			}
		}
		else if(GetVillageAttackType() == ON_VILLAGE_ATTACK_DEFENCE)
		{
			//if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(15000))
			//{
				 //마을의 AttackType을 원래대로 돌린다.
				 InitializeVillageAttackType();
				 //마을 주위에 플레이어들에게 AttackType이 원래대로 돌아왔다는것을 알린다.
				 //SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			//}
		}
		else if(GetVillageAttackType() == ON_VILLAGE_ATTACK_STONE)
		{
			if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(15000))
			{
				 //마을의 AttackType을 원래대로 돌린다.
				 InitializeVillageAttackType();
				 //마을 주위에 플레이어들에게 AttackType이 원래대로 돌아왔다는것을 알린다.
				 SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			}
		}
	}

	if(m_bAttackTypeDelay == TRUE)
	{
		if(GETTIMEGAP(m_dwPrevAttackdelay, clWorld->dwtimeGetTime) >= m_dwSkillDelayTime)
		{
			m_bAttackTypeDelay = FALSE;
			m_dwPrevAttackdelay		=	0;		
			m_dwSkillDelayTime		=	0;
		}
	}
	//공성을 하고 있는 마을이라면.
	if(m_bVillageAttack == TRUE)
	{
		if(GETTIMEGAP(m_dwVillageDamageUpdateTime,clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			m_dwVillageDamageUpdateTime = clWorld->dwtimeGetTime;
			
			if(uiGuildID > 0)//마을을 소유하고 있는 길드가 있는지 검사
			{
				SI32 siTempGuildIndex = clWorld->clIGuild.CheckVillage(uiVillageCode);
				if(clWorld->clIGuild.GetGuildIDByIndex(siTempGuildIndex) == uiGuildID)
				{
					for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM ; i++)
					{
						if(clWorld->clIGuild.GetGuildIDByIndex(i) == 0)continue;
						if((m_stAttackGuild[i].uiGuildID != 0) && (m_stAttackGuild[i].siDamage != 0))
						{
							DBIKVillageDamage	TempDBIKVillageDamage;
							TempDBIKVillageDamage.uiGuildID			= m_stAttackGuild[i].uiGuildID;
							TempDBIKVillageDamage.siVillageCode		= uiVillageCode;
							TempDBIKVillageDamage.siDamage			= m_stAttackGuild[i].siDamage;

							writeInfoToFile("GuildDamage.txt", "마을 소속 길드: %d, VillageID:%d, AttackGuild:%d, Damage:%d", 
								             uiGuildID, uiVillageCode, m_stAttackGuild[i].uiGuildID, m_stAttackGuild[i].siDamage);	
							
							clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_VILLAGE_DAMAGE, &TempDBIKVillageDamage, sizeof(DBIKVillageDamage));
						}
					}
				}
			}
			
		}
	}

	if(clWorld->bUpdateTime)		// 새벽 11시에 TRUE 상태가 된다.
	{
		//인구수 계산,마을 방어력 계산등을 서버에서 한다.
		printf("인구수, 방어력 업데이트 시작");
		SetDBPopulation();
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을에 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::EnterVillage(SOPlayer *pPlayer)
{
	siVillageInOutPlayerNowYear++;

	return TRUE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을이 있는 맵의 id를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	SOVillage::GetMapID()
{
	return uiMapID;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	DB에서 마을 안내문을 가져온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::GetDescription()
{
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEDESCRIPT, LPVOID(uiVillageCode),LPVOID(&szDescription)) == FALSE)	
	{
		memset(szDescription,0, sizeof(szDescription));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	DB에서 마을의 국가 코드를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::SetNationCode(CHAR NationCode)
{
	DBIKVillageNation		VillageNation;
	VillageNation.VillageCode	=	uiVillageCode;
	VillageNation.Code			=	NationCode;
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGE_SETNATION, &VillageNation, sizeof(DBIKVillageNation));
	
	cNation	=	NationCode;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	DB에서 마을의 국가 코드를 가져온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::GetNationCode()
{
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGE_GETNATION, LPVOID(uiVillageCode),LPVOID(&cNation));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 이름을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	SOVillage::GetVillageName()
{
	return clWorld->clOnlineText.Get(uiVillageName);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해당 건물이 이 마을 안에 있는가?
//	수정 일자	:	2002-10-04 오후 6:59:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DoesStructureExist(SI32 siStructureKind)
{
	SI32	 i;

	for(i = 0; i < ON_VILLAGE_MAX_BUILDING; i++)
	{
		if(siStructureKinds[i] == siStructureKind)
			return TRUE;	
	}
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 출입자 수를 계산한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage::GetVillageInOutPlayer(void)
{
	if(siVillageInOutPlayerYetYear > siVillageInOutPlayerNowYear) return siVillageInOutPlayerYetYear;

	return siVillageInOutPlayerNowYear;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	1년이 지났을때 마을의 출입자수를 갱신한다 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void	SOVillage::PrepareVillageInOutPlayer(void)
{
	siVillageInOutPlayerYetYear = siVillageInOutPlayerNowYear;
	siVillageInOutPlayerNowYear = 0;

	// DB 에 저장
	DBIKVillageEnterPlauerSet		TempDBIKVillageEnterPlauerSet;

	TempDBIKVillageEnterPlauerSet.uiVillageCode		=	uiVillageCode;
	TempDBIKVillageEnterPlauerSet.uiPlayerNum		=	siVillageInOutPlayerYetYear;
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEENTERPLAYERSET, LPVOID(&TempDBIKVillageEnterPlauerSet), sizeof(TempDBIKVillageEnterPlauerSet));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에 있는 길드의 숫자 ID를 얻어온다.
//	수정 일자	:	2002-02-27 오전 11:09:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	SOVillage::GetGuildNID()
{
	return uiGuildID;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에 있는 길드의 문자 ID를 얻어온다.
//	수정 일자	:	2002-02-27 오전 11:09:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━	
CHAR*	SOVillage::GetGuildSID()
{
	return 	clWorld->clIGuild.GetGuildName(uiGuildID);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을이 소속되는 상단을 설정한다.
//	수정 일자	:	2002-05-10 오후 9:32:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::SetOwnerGuild(UI32 uiguild, BOOL bInit)
{
	DBIKGuildOwnerVillageInfo	GuildOwnerVillageInfo;

	// 이 마을의 상단의 ID를 저장한다.
	uiGuildID		=		uiguild;

	// 초기화 중이면 그냥 리턴한다.
	if(bInit == TRUE)	return;	

	GuildOwnerVillageInfo.uiVillageCode	=	uiVillageCode;
	GuildOwnerVillageInfo.uiGuildID		=	uiGuildID;
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEGUILDID_SET, &GuildOwnerVillageInfo, sizeof(DBIKGuildOwnerVillageInfo));
	
	// 이 마을의 최고 투자자를 행수로 지정해준다.
	SetHNSID(m_Government->GetInvestmentMaxInvestmentorName());	
	// 이 마을의 깃발은 소속 상단의 깃발을 가지게 된다.
	ChangeGuildFlag(clWorld->clIGuild.GetFlag(uiguild));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	국가 코드를 준다.(상단이 없으면, 현재 국가상태로 있으면, 상단 상태로)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage::GetVillageState()
{
	switch(cNation)
	{
		case NATION_CODE_KOREA:
			return ON_COUNTRY_KOREA;

		case NATION_CODE_CHINA:
			return ON_COUNTRY_CHINA;

		case NATION_CODE_JAPAN:
			return ON_COUNTRY_JAPAN;

		case NATION_CODE_TAIWAN:
			return ON_COUNTRY_TAIWAN;
	}

	return ON_COUNTRY_NONE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에 새로운 '행수'를 지정한다.
//	수정 일자	:	2002-03-20 오전 11:35:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::SetHNSID(CHAR *pszHNSID)
{
	SI32				siHNSClassCode	=	clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS);	

	DBIKVillageMaster	VillageMaster;

	// 현재 상단이 없으면 리턴한다.
	if(uiGuildID == 0)					return	FALSE;
	if(pszHNSID == NULL)				return	FALSE;
	if(strcmp(szHNSID, pszHNSID) == 0)	return	FALSE;

	// 현재의 '행수'를 해고한다.
	DismissalHNS();

	// 새로운 '행수'의 이름을 DB에 저장한다.
	ZeroMemory(&VillageMaster, sizeof(DBIKVillageMaster));
	VillageMaster.VillageCode	=	uiVillageCode;
	strcpy( VillageMaster.Name, pszHNSID );
	
	// 길드생성이 아닐때	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEHNSNAME_SET, &VillageMaster, sizeof(DBIKVillageMaster));
	
	
	// '행수'의 이름을 마을에 저장한다.
	ZeroMemory(szHNSID, sizeof(szHNSID));
	strncpy(szHNSID, pszHNSID, ON_ID_LENGTH);
	

	cltGuild *pGuild = clWorld->clIGuild.GetGuildByID(uiGuildID);

	strGuildCommonMemberInfo MemberInfo;

	// 그 캐릭터의 최대 직급을 구한다.
	if (pGuild != NULL)
	{
		if (pGuild->m_GuildMemberManager.GetGuildMemberByCharName(pszHNSID,&MemberInfo) == FALSE)
		{
			return FALSE;
		}

	}
	else
	{
		return FALSE;
	}
	
	// 만약 직급 코드가 행수보다 크거나 같으면 변경할 필요가 없다.
	if(clWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siHNSClassCode) == SO_GUILD_CLASSCOMP_LOW)
	{
	
		// 행수로 승급시켜준다.

		// DB에 저장을 하고
		// 접속을 하고 있다면 직급이 변경되었다는 메시지를 보내준다.	
		clWorld->clIGuild.ChangeClass( uiGuildID, pszHNSID, siHNSClassCode );	
	}

	return	TRUE;
		
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에서 이 캐릭터의 최고 직급을 얻어온다.
//	수정 일자	:	2002-03-25 오전 10:13:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage::GetHighestGuildClass(CHAR *pszID)
{
	// 아직은 행수 밖에 없다.
	if(strcmp(szHNSID, pszID) == 0)
		return clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS);
	else
	{
		// 나머지는 사환이다.
		return clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);
	}

	return	0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을의 최고 투자자이면서 상단에 가입되어 있는지를 
//					체크한다.
//	수정 일자	:	2004-02-04 오전 - 이민수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL   SOVillage::IsVestInvestmentorAndGuildMember(const CHAR* pszName)
{
	m_Government->VillageInvestment_CheckBestInvestmentor();	
	UI32		TempMaxInvestmentorGuild;
	TempMaxInvestmentorGuild = m_Government->GetInvestmentMaxInvestmentorGuild();

	return ((TempMaxInvestmentorGuild != 0 ) &&
		(strcmp(pszName, m_Government->GetInvestmentMaxInvestmentorName()) == 0)) ? TRUE : FALSE;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을에서 이 캐릭터가 가지고 있는 직급을 초기화 한다.
//					해당 직급을 삭제하고, 뒷처리를 해준다.
//					만약 이 마을이 상단에서 삭제되어야 한다면 TRUE를 리턴한다.
//					그렇지 않으면 FALSE를 리턴한다.
//	수정 일자	:	2002-05-09 오후 7:53:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DeleteGuildClass(CHAR *pszName)
{
	///////////////////////////////////////////////////////////////////////
	//BOOL bVal = IsVestInvestmentorAndGuildMember(pszName);	
	///////////////////////////////////////////////////////////////////////
	
	BOOL bRetVal = FALSE;

	// 이름이 없다면 그냥 리턴한다.
	if(strcmp(pszName, "") == 0) bRetVal = FALSE;

	else if(strcmp(szHNSID, pszName) == 0)
	{		
		// 행수인 pszName의 직급을 없앤다는것은 
		// 즉 이 마을을 상단에서 제거한다는것이다.
		// 동시에 pszName의 캐릭터는 행수의 권한이 없어진다.

		// 행수를 해고한다.			
		DismissalHNS();

		bRetVal = TRUE;
	}

	// 마을의 행수 이름이 잘못되어 있는경우.
	/*if(bVal == TRUE && bRetVal == FALSE)
	{
		writeInfoToFile( "DeleteGuildError.txt", 
			"Invalid Hangsu name. Hangsu name is %s, MaxInvestmentor name is %s", 
			szHNSID, pszName);
	}*/

	return	bRetVal;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 마을의 길드가 삭제되었다.
//	수정 일자	:	2002-03-25 오전 11:04:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::DeleteGuild()
{	
	DBIKGuildOwnerVillageInfo	GuildOwnerVillageInfo;

	GuildOwnerVillageInfo.uiVillageCode	=	uiVillageCode;
	GuildOwnerVillageInfo.uiGuildID		=	uiGuildID;

//	m_siDefenceSoldier					=	0;
//	m_siMaxDefenceSoldier				=	0;	
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEOWNER_DELETE, &GuildOwnerVillageInfo, sizeof(DBIKGuildOwnerVillageInfo));

	// 행수를 삭제한다.
	DeleteGuildClass(szHNSID);

	
	// 상단과 상단의 깃발을 초기화 한다.
	uiGuildID		=	0;
	siGuildFlag		=	0;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재의 행수를 해고한다.	
//	수정 일자	:	2002-04-30 오후 9:36:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::DismissalHNS()
{
	SI32		siHighestClassCode;	

	SI32		siHNSClassCode	=	clWorld->clIGuildClass.GetHNSCode();					// '행수'의 코드를 미리 가지고 있는다.
	SI32		siDHSClassCode	=	clWorld->clIGuildClass.GetDHSCode();					// '대행수'의 코드를 미리 가지고 있는다.
	CHAR		szPrevHNSID[ON_ID_LENGTH + 1];

	// 현재 상단이 없거나 행수가 없으면 리턴한다.
	if(uiGuildID == 0)					return;		
	if(szHNSID[0] == NULL)				return;		

	// 이 마을의 행수를 미리 기억해둔다.
	ZeroMemory(szPrevHNSID, sizeof(szPrevHNSID));
	strncpy(szPrevHNSID, szHNSID, ON_ID_LENGTH);	 

	// 이 마을의 행수를 없앤다.(이 부분은 반드시 아래 부분 보다 위에 있어야한다. 아래의 GetHighestGuildClass 함수에서 이 변수를 참조 하기 때문이다.) 	
	ZeroMemory(szHNSID, sizeof(szHNSID));

	// 행수였던 사람의 최대 직급을 구한다.
	cltGuild *pGuild = clWorld->clIGuild.GetGuildByID(uiGuildID);
	if (pGuild != NULL)
	{

		strGuildCommonMemberInfo MemberInfo;
		if (pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szPrevHNSID,&MemberInfo) == FALSE) return;			
				
		// 아직 지원자였을 경우 직급 변동이없다.
		if (MemberInfo.siClassInGuild == 90) return;

		// 마을의 최투였던 사람이 행수보다 높은 직책이 아니었을 경우에만... 대방이나 대행수는 마을을 잃어도 직책을 유지하므로..
		if( clWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siHNSClassCode) != SO_GUILD_CLASSCOMP_HIGH )	// 여태까지 '행수' 였으니 최소한 '행수'가 나올 것이다.
		{			
			// 현재 게임 상에서 이 캐릭터의 최대 직급을 얻어온다.
			siHighestClassCode	=	clWorld->GetHighestGuildClass(szPrevHNSID);		// 이 캐릭터가 행수인 마을이 있는지 모든 마을 검사
			// 현재 직급이 저장된 직급과 다르다면 바꾸어준다.
			if(siHighestClassCode != MemberInfo.siClassInGuild) clWorld->clIGuild.ChangeClass(uiGuildID, szPrevHNSID, siHighestClassCode);			
		}
	}
	else
	{
		return;
	}				
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 정보를 얻어간다.
//	수정 일자	:	2002-09-30 오후 1:12:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	SOVillage::GetVillageInfo(SOPlayer *pPlayer, OnRecvVillageInfo *pVillageInfo)
{
	CHAR	*pszInvestmentorName;

	if(pVillageInfo)
	{
		ZeroMemory(pVillageInfo, sizeof(OnRecvVillageInfo));

		pVillageInfo->uiPopulation	=	uiPopulation;

		// 최고 투자자의 정보를 얻어온다.
		if((pszInvestmentorName = m_Government->GetInvestmentMaxInvestmentorName()) != NULL)
			strcpy(pVillageInfo->m_MaxInvestmentID, pszInvestmentorName);
		
		// 마을의 방어력을 얻어온다.
		pVillageInfo->vdVillageDefence		=	vdDefence;

		// 마을의 최대 방어력을 얻어온다.
		pVillageInfo->vdVillageMaxDefence	=	vdMaxDefence;
//		pVillageInfo->siDefenceSoldier		=	m_siDefenceSoldier;
//		pVillageInfo->siMaxDefenceSoldier	=	m_siMaxDefenceSoldier;
		pVillageInfo->siAttackType			=	m_siVillageAttackType;
	
		// 상단의 정보를 얻어온다.
		if(uiGuildID != 0)
		{
			// 상단이 있다면 상단의 정보를 얻어온다.
			strncpy(pVillageInfo->szGuildID, GetGuildSID(), ON_GUILDNAME_LENGTH);
		}

		// 플레이어와 상단간의 관계를 얻어온다.
		pVillageInfo->dwGuildRelation	=	ON_RET_GLD_NEUTRAL;

		// 길드 멤버인지 알아온다.
		if((pPlayer->uiGuildID != 0) && (uiGuildID != 0) && (pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode()))
		{
			if(pPlayer->uiGuildID == uiGuildID)							
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_MINE;	// 이 마을의 상단원이다.
			else if(clWorld->clIGuild.IsHostilityRelation(uiGuildID, pPlayer->uiGuildID) == TRUE)
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_ENEMY;	// 적대적인 마을의 상단이다.
			else if(clWorld->clIGuild.IsEnemyRelationWar(uiGuildID, pPlayer->uiGuildID) == TRUE)
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_WAR;	// 공성 하는 날이다.
		}

		// 국가 코드를 얻어온다.
		pVillageInfo->cNationCode	=	cNation;
	
		// 깃발을 얻어온다.	
		pVillageInfo->siGuildFlag	=	siGuildFlag;

		return	sizeof( OnRecvVillageInfo );
	}
	else
	{
		return	0;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	깃발을 얻어간다
//	수정 일자	:	2002-04-04 오후 2:52:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage::GetGuildFlag()
{
	return siGuildFlag;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	깃발을 변경한다 
//	수정 일자	:	2002-04-04 오후 3:43:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::ChangeGuildFlag(SI32 siFlag)
{
	siGuildFlag	=	siFlag;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	서버 On 시 초기 마을 자본을 읽어 들인다.
//	수정 일자	:	2002-04-04 오후 5:00 - 이성재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::GetVillageCapital()
{	
	BOOL bRet;
	bRet = clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETVILLAGECAPITAL, this, NULL);

	return bRet;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	플레이어가 소유한 생산건물을 얻어온다.
//	수정 일자	:	2002-10-04 오후 7:23:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stFarmData*	SOVillage::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	if( DoesFactoryExist() == FALSE )			return	NULL;
	
//	return	m_pFactory->GetOwnFactory( siPlayerDBAccount, uiPlayerDBSlot );	
	return	NULL;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수를 초기화 한다.
//	수정 일자	:	2002-08-07 오후 2:36:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::InitStaticVariable(SOWorld *pWorld)
{
	clWorld		=	pWorld;

	SOVillage_Market::InitStaticVariable( &clWorld->clItem, &clWorld->clIProfit, clWorld->clDatabase );
	SOVillage_IndustrialComplexs::InitStaticVariable( &clWorld->clItem, clWorld->clDatabase, &clWorld->clBuilding );
	SOVillage_Bank::InitStaticVariable( clWorld->clDatabase, &clWorld->clOnlineEconomy );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에 있는 건물들을 초기화 한다.
//	수정 일자	:	2002-10-01 오후 7:12:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::InitStructure( VillageHeader *pVillageHeader )
{
	SI32							siStructureIndex;

	CHAR							*pszPlantCode;
	SI32							siStructureKind;
	CHAR							cStructureKind[ 4 ];

	m_Barrack = NULL;
	for( siStructureIndex = 0; siStructureIndex < ON_VILLAGE_MAX_BUILDING; siStructureIndex++ )
	{
		siStructureKind		=	pVillageHeader->siStructureKinds[ siStructureIndex ];
		memcpy( cStructureKind, pVillageHeader->szStructureKinds[ siStructureIndex ], sizeof( cStructureKind ) );

		switch( siStructureKind	)
		{
			
		case ON_VIL_ST_KIND_MARKET :						// 시전이다.
		case ON_VIL_ST_KIND_WSTORE : 
		case ON_VIL_ST_KIND_FSTORE :
		case ON_VIL_ST_KIND_DSTORE :
			m_pMarket	=	new	SOVillage_Market;
			
			// 시전을 초기화 한다.		
			if( m_pMarket->Init( uiVillageCode, pVillageHeader->uiItemID, pVillageHeader->siProperty, pVillageHeader->siQuantity ) == FALSE )	return	FALSE;
			
			break;
			

		case ON_VIL_ST_KIND_INN :							// 주막이다.

			break;
			

		case ON_VIL_ST_KIND_BANK :							// 전장이다.
		
			m_pBank		=	new	SOVillage_Bank();

			if( m_pBank->Init( uiVillageCode ) == FALSE )		return	FALSE;
/*
			// 그 마을에 은행이 있다면 은행의 데이타를 생성한다.
			pstBankData		=	new	stBankData;
			ZeroMemory(pstBankData, sizeof(stBankData));
			UpdateBankInfo();
*/
			
			break;


		case ON_VIL_ST_KIND_WHARF :							// 부두이다.
			m_Wharf		=	new	SOVillage_Wharf;
			
			// 시전을 초기화 한다.		
			if( m_Wharf->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;
			break;

		// 경매소이다.
		case ON_VIL_ST_KIND_AUCTION :	
			break;


		case ON_VIL_ST_KIND_FARM :							// 생산 시설물이다.
		case ON_VIL_ST_KIND_WEAPON : 
		case ON_VIL_ST_KIND_ARMOR :
		case ON_VIL_ST_KIND_PASTURE :
		case ON_VIL_ST_KIND_FACTORY :

			switch( siStructureKind )
			{
			case ON_VIL_ST_KIND_FARM :		pszPlantCode	=	ON_VIL_ST_KIND_CODE_FARM;			break;
			case ON_VIL_ST_KIND_WEAPON :	pszPlantCode	=	ON_VIL_ST_KIND_CODE_WEAPON;			break;
			case ON_VIL_ST_KIND_ARMOR :		pszPlantCode	=	ON_VIL_ST_KIND_CODE_ARMOR;			break;
			case ON_VIL_ST_KIND_PASTURE :	pszPlantCode	=	ON_VIL_ST_KIND_CODE_PASTURE;		break;
			case ON_VIL_ST_KIND_FACTORY :	pszPlantCode	=	ON_VIL_ST_KIND_CODE_FACTORY;		break;
			}

			m_pIndustrialComplexs = new SOVillage_IndustrialComplexs;

			// 생산 시설을 초기화 한다.
			//if( m_pFactory->Init( uiVillageCode, clWorld->clBuilding.GetPlants( pszPlantCode ) ) == FALSE )		return	FALSE;			
			if( m_pIndustrialComplexs->Init( uiVillageCode, cStructureKind ) == FALSE )							return	FALSE;			

			// 생산 시설의 대한 데이타를 DB에서 얻어온다.
			if( GetFarmData() == FALSE )																		return	FALSE;

			break;

		case ON_VIL_ST_KIND_HOSPITAL:
			m_Hospital	=	new	SOVillage_Hospital;
			
			// 시전을 초기화 한다.		
			if( m_Hospital->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this) == FALSE )	return	FALSE;
			break;

		case ON_VIL_ST_KIND_BARRACK:
		case ON_VIL_ST_KIND_GBARRACK:
		case ON_VIL_ST_KIND_MBARRACK:
			if(m_Barrack == NULL)
			{
				m_Barrack	=	new	SOVillage_Barrack;
				
				// 시전을 초기화 한다.		
				if( m_Barrack->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;
			}
			break;

		case ON_VIL_ST_KIND_GOVERNMENT:
			m_Government	=	new	SOVillage_Government;
			
			// 시전을 초기화 한다.		
			if( m_Government->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;			
			break;
		}		
	}
	
	return	TRUE;
}	

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 건물에 들어간다.
//	수정 일자	:	2002-10-02 오후 7:47:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::EnterStructure( SOPlayer *pPlayer, SI32 siStructureKind, LPARAM lParam1, LPARAM lParam2 )
{ 
	
	switch( siStructureKind )
	{

	case ON_VIL_ST_KIND_MARKET :						// 시전이다.
	case ON_VIL_ST_KIND_WSTORE : 
	case ON_VIL_ST_KIND_FSTORE :
	case ON_VIL_ST_KIND_DSTORE :
		
		if( m_pMarket->EnterPlayer( pPlayer ) == FALSE )							return	FALSE;
		
		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_MARKET;

		break;	
		
	case ON_VIL_ST_KIND_FARM :							// 생산 시설물이다.
	case ON_VIL_ST_KIND_WEAPON : 
	case ON_VIL_ST_KIND_ARMOR :
	case ON_VIL_ST_KIND_PASTURE :
	case ON_VIL_ST_KIND_FACTORY :

		if( m_pIndustrialComplexs->EnterPlayer( pPlayer, lParam2 ) == FALSE )					return	FALSE;

		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_FACTORY;

		break;

	case ON_VIL_ST_KIND_HOSPITAL:
		if( m_Hospital->EnterPlayer( pPlayer ) == FALSE )					return	FALSE;

		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_HOSPITAL;
		break;

	case ON_VIL_ST_KIND_BARRACK:
		if( m_Barrack->EnterPlayer( pPlayer ) == FALSE )					return	FALSE;

		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_BARRACK;
		break;

	case ON_VIL_ST_KIND_WHARF:
		if( m_Wharf->EnterPlayer( pPlayer ) == FALSE )					return	FALSE;

		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_WHARF;
		break;

	case ON_VIL_ST_KIND_GOVERNMENT:
		if( m_Government->EnterPlayer( pPlayer ) == FALSE )					return	FALSE;

		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_GOVERNMENT;		
		break;

	case ON_VIL_ST_KIND_BANK:
		if( m_pBank->EnterPlayer( pPlayer ) == FALSE )						return	FALSE;

		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_BANK;		

		break;


	default:

		return	FALSE;

	}
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 건물에서 나간다.	
//	수정 일자	:	2002-10-02 오후 7:47:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::LeaveStructure( SOPlayer *pPlayer )
{
	if( m_pMarket != NULL )				if( m_pMarket->IsEntered( pPlayer ) == TRUE )				return	m_pMarket->LeavePlayer( pPlayer );
	if( m_pIndustrialComplexs != NULL )	if( m_pIndustrialComplexs->IsEntered( pPlayer ) == TRUE )	return	m_pIndustrialComplexs->LeavePlayer( pPlayer );
	if( m_Barrack != NULL )				if( m_Barrack->IsEntered( pPlayer ) == TRUE )				return	m_Barrack->LeavePlayer( pPlayer );
	if( m_Hospital != NULL )			if( m_Hospital->IsEntered( pPlayer ) == TRUE )				return	m_Hospital->LeavePlayer( pPlayer );
	if( m_Wharf != NULL )				if( m_Wharf->IsEntered( pPlayer ) == TRUE )					return	m_Wharf->LeavePlayer( pPlayer );
	if( m_Government != NULL )			if( m_Government->IsEntered( pPlayer ) == TRUE )			return	m_Government->LeavePlayer( pPlayer );
	if( m_pBank != NULL )				if( m_pBank->IsEntered( pPlayer ) == TRUE )					return	m_pBank->LeavePlayer( pPlayer );

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에 은행이 있는지 검사한다.
//	수정 일자	:	2002-10-04 오후 6:56:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DoesHospitalExist()
{
	return	DoesStructureExist( ON_VIL_ST_KIND_HOSPITAL );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에 시전이 있는지 검사한다.
//	수정 일자	:	2002-10-04 오후 6:59:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DoesMarketExist()
{
	if( DoesStructureExist( ON_VIL_ST_KIND_MARKET ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_WSTORE ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_FSTORE ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_DSTORE ) == TRUE )		return	TRUE;

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에 생산 시설물이 있는지 검사한다.
//	수정 일자	:	2002-10-04 오후 7:00:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DoesFactoryExist()
{
	if( DoesStructureExist( ON_VIL_ST_KIND_FARM ) == TRUE )			return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_WEAPON ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_ARMOR ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_FACTORY ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_PASTURE ) == TRUE )		return	TRUE;

	return	FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을에 전장이 있는지 검사한다.
//	수정 일자	:	2002-10-18 오후 3:41:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DoesBankExist()
{
	return	DoesStructureExist( ON_VIL_ST_KIND_BANK );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 정보를 얻어온다.
//	수정 일자	:	2002-11-27 오후 2:50:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::GetInfo( OnGuildVillageInfo *pGuildVillageInfo )
{
	CHAR		*pMaxInvestmentorName;

	ZeroMemory( pGuildVillageInfo, sizeof( OnGuildVillageInfo ) );

	// 마을의 코드 저장
	pGuildVillageInfo->uiVillageCode			=	uiVillageCode;	
	
	if( m_Government != NULL )
	{
		// 마을의 최고 투자자 저장.
		if( ( pMaxInvestmentorName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )
		{
			strncpy( pGuildVillageInfo->szBestInvestorName, pMaxInvestmentorName, ON_ID_LENGTH );
		}			

		// 마을의 최고 투자금 저장.
		pGuildVillageInfo->mnBestInvestorMoney		=	m_Government->GetInvestmentMaxInvestmentorMoney();
	}	

	// 마을의 방어력 저장.
	pGuildVillageInfo->vdVillageDefencePower	=	vdDefence;

	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	적 상단의 공격에 의해서 받은 데미지를 초기화 한다.
//	수정 일자	:	2003-01-01 오전 5:11:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::InitDamageAttackedByGuild()
{
	ZeroMemory( m_stAttackGuild, sizeof( m_stAttackGuild ) );	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	
//	수정 일자	:	2003-01-01 오전 5:16:39 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::AddDamageAttackedByGuild( UI32 uilocal_GuildID, SI32 siDamage )
{
	if(siDamage <= 0) return;
	SI32	siSlot;

	if( ( siSlot = GetAttackGuildSlot( uilocal_GuildID ) ) == -1 ) return;
	if( uiGuildID == uilocal_GuildID) return;

	if( m_stAttackGuild[ siSlot ].uiGuildID == 0 )
	{
		m_stAttackGuild[ siSlot ].uiGuildID	=	uilocal_GuildID;
	}

	if( ( INT_MAX - m_stAttackGuild[ siSlot ].siDamage ) >= siDamage )
	{
		m_stAttackGuild[ siSlot ].siDamage	+=	siDamage;
	}
	else
	{
		m_stAttackGuild[ siSlot ].siDamage	=	INT_MAX;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	
//	수정 일자	:	2003-01-01 오전 5:28:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::RewardToGuildForDestroyVillage(MONEY mnMoney)
{
	SI16 siMaxIndex = 0;
	SI32 siMaxDamage = 0;
	cltGuild	*pGuild;
	SI16 siPercent = 0;
//	SI16 siRewardSupply	= 10000;

	siPercent = (SI16)(50 - (float)(50 * ( (float)vdDefence/vdMaxDefence)) + 0.5);

	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM; i++ )
	{
		if(m_stAttackGuild[i].uiGuildID <= 0) continue;
		if(m_stAttackGuild[i].uiGuildID == uiGuildID) continue;
		if(clWorld->clIGuild.GetGuildIDByIndex(i) != m_stAttackGuild[i].uiGuildID) continue;

		if(m_stAttackGuild[i].siDamage > siMaxDamage)
		{
			if(clWorld->clIGuild.GetGuildIndexByID(m_stAttackGuild[i].uiGuildID) != -1)
			{
				siMaxIndex = i;
				siMaxDamage = m_stAttackGuild[i].siDamage;
			}
		}
	}
//	clWorld->clIGuild.RewardToGuildDAIForDestroyVillage(m_stAttackGuild[siMaxIndex].uiGuildID, mnMoney, uiVillageCode,siRewardSupply);
	clWorld->clIGuild.RewardToGuildDAIForDestroyVillage(m_stAttackGuild[siMaxIndex].uiGuildID, mnMoney, uiVillageCode);
	//if(m_stAttackGuild[siMaxIndex].uiGuildID != 0)	continue;

	pGuild = clWorld->clIGuild.GetGuildByID(m_stAttackGuild[siMaxIndex].uiGuildID);
//	pGuild->IncreaseGuildSupply(siRewardSupply);

	for(i = 0 ; i < ON_MAX_GUILD_RESULT;i++)
	{
		if(pGuild->m_stGuildResult[i].GetVillageCode() == 0)
		{
			pGuild->m_stGuildResult[i].SetVillageCode((SI16)uiVillageCode);
			pGuild->m_stGuildResult[i].SetCharName("");
			pGuild->m_stGuildResult[i].SetPercent(siPercent);
			goto END;
		}
	}
END:

	DBIKGuildResult		TempGuildResult;

	TempGuildResult.siVillageCode	=	(SI16)uiVillageCode;
	TempGuildResult.siPercent		=	siPercent;
	TempGuildResult.uiGuildID		=	m_stAttackGuild[siMaxIndex].uiGuildID;
	strcpy(TempGuildResult.szCharName,"");

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_RESULT, LPVOID(&TempGuildResult), sizeof(DBIKGuildResult));

	/*
	SI32	i, j;
	SI32	siGuildIndex;
	DWORD	dwMaxDamage, dwTempDamageAttackedByGuild, dwTotalMaxDamage;	
	CHAR	szTempDBArg[ 1024 ];
	CHAR	szTempDBArg1[ 1024 ], szTempDBArg2[ 1024 ];
	CHAR	szDBArg[ 1024 ];
	MONEY	mnReward;
	
	if( mnTotalRewardMoney == 0 )	return;

	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	ZeroMemory( szTempDBArg1, sizeof( szTempDBArg1 ) );
	ZeroMemory( szTempDBArg2, sizeof( szTempDBArg2 ) );
	
	dwTotalMaxDamage	=	0;
	for( i = 0; i < SO_TOTAL_GUILD_NUM; i++ )	dwTotalMaxDamage	+=	m_stAttackGuild[ i ].dwDamage;	
	
	if( dwTotalMaxDamage == 0 )		return;	

	// 10개의 상단에게만 보상을 해준다.	
	for( i = 0; i < 10; i++ )
	{
		siGuildIndex		=	-1;
		dwMaxDamage			=	0;

		for( j = 0; j < SO_TOTAL_GUILD_NUM; j++ )
		{
			if( m_stAttackGuild[ j ].uiGuildID == 0 )		continue;

			dwTempDamageAttackedByGuild		=	m_stAttackGuild[ j ].dwDamage;

			if( dwTempDamageAttackedByGuild > dwMaxDamage )
			{
				dwMaxDamage		=	dwTempDamageAttackedByGuild;
				siGuildIndex	=	j;
			}
		}

		if( siGuildIndex != -1 )
		{
			mnReward	=	( mnTotalRewardMoney * dwMaxDamage ) / dwTotalMaxDamage;

			// 상단의 대방에게 보상금을 준다.
			if( clWorld->clIGuild.RewardToGuildDAIForDestroyVillage( m_stAttackGuild[ siGuildIndex ].uiGuildID, mnReward, uiVillageCode ) == TRUE )
			{
				ZeroMemory( szTempDBArg, sizeof( szTempDBArg ) );
				sprintf( szTempDBArg, "%u ", m_stAttackGuild[ siGuildIndex ].uiGuildID );
				strcat( szTempDBArg1, szTempDBArg );

				ZeroMemory( szTempDBArg, sizeof( szTempDBArg ) );
				sprintf( szTempDBArg, "%I64d ", mnReward );
				strcat( szTempDBArg2, szTempDBArg );

			}

			ZeroMemory( &m_stAttackGuild[ siGuildIndex ], sizeof( stAttackGuild ) );			
		}			
	}

	sprintf( szDBArg, "%u, '%s', '%s'", uiVillageCode, szTempDBArg1, szTempDBArg2 );
	clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_REWARDTODAIFORDESTROYVILLAGE, szDBArg, strlen( szDBArg ) + 1 );
	*/
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을을 공격한 상단의 슬롯을 얻어온다.
//	수정 일자	:	2003-01-03 오전 10:43:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage::GetAttackGuildSlot( UI32 uilocal_GuildID )
{
	SI32	i;

	for( i = 0; i < SO_TOTAL_GUILD_NUM; i++ )
	{
		if( m_stAttackGuild[ i ].uiGuildID == uilocal_GuildID )
		{
			return	i;
		}
	}

	for( i = 0; i < SO_TOTAL_GUILD_NUM; i++ )
	{
		if( m_stAttackGuild[ i ].uiGuildID == 0 )
		{
			return	i;
		}
	}

	return	-1;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 공성이 시작되었다.
//	수정 일자	:	2003-01-14 오후 3:51:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::EnterAttackVillageTimeZone()
{	
	InitDamageAttackedByGuild();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을 공성이 시작되었다.
//	수정 일자	:	2003-01-14 오후 3:51:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::LeaveAttackVillageTimeZone()
{
	m_bVillageAttack = FALSE;
	m_dwVillageDamageUpdateTime = 0;
	ZeroMemory(m_stAttackGuild,sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	다음 장날을 얻어온다.
//	수정 일자	:	2003-04-11 오전 11:00: - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	SOVillage::GetNextMarketDay()
{
	return m_siNextMarketDay;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	다음 장날을 설정한다.
//	수정 일자	:	2003-04-11 오전 11:03: - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::SetNextMarketDay()
{
	SI32		siMaxDay;

	// 3일후, 5일후 또는 7일후중 랜덤하게 선택하여 장날 설정
	switch( rand() % 3 )
	{
	case 0:
		m_siNextMarketDay += 3;			break;
	case 1:
		m_siNextMarketDay += 5;			break;
	case 2:
		m_siNextMarketDay += 7;			break;
	}

	// 현재 달의 일수를 구한다.
	siMaxDay	=	clWorld->Timer.GetDaysInMonth( clWorld->CurrentTime );

	// 달의 마지막 날을 넘어가는 경우 다음달의 일자로 바꾼다.
	if( m_siNextMarketDay > siMaxDay )	m_siNextMarketDay	-=	siMaxDay;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장날을 초기화한다.
//	수정 일자	:	2003-04-11 오후 02:30: - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::InitMarketDay()
{
	// 현재 날짜로 초기화한 다음 다음 장날을 설정해둔다.
	m_siNextMarketDay	=	clWorld->CurrentTime.Day;
	SetNextMarketDay();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 인구 관련 함수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	SOVillage::GetPopulation()
{
	return uiPopulation;
}

BOOL	SOVillage::CheckDecreasePopulation(UI32 uiDecreasePopulation)
{
	UI32 uiLoadPopul = uiPopulation * 0.3;

	if(uiLoadPopul >= uiDecreasePopulation)
	{
		return TRUE;
	}
//	UI32 uiTempPopulation = uiMaxPopulation - uiPopulation + uiDecreasePopulation;
	
	//고용가능 인구수 (최대인구수의 30% 이내)를 초과하지 않으면
/*	if(m_uiLoadQuantity >= uiTempPopulation)
	{
		return TRUE;
	}
	*/
	return FALSE;
}

BOOL	SOVillage::LimitDecreasePopulation(UI32	uiDecreasePopulation)
{
	// 현재 인구에서 인구를 감소한다.
	UI32 uiTempPopulation = uiPopulation - uiDecreasePopulation;

	// 감소한 인구가 마을의 최저 인구보다 작으면
	if(uiTempPopulation < m_uiLimitMinPopulation)
		return FALSE;
	return TRUE;
}

void	SOVillage::DecreasePopulation(UI32 uiDecreasePopulation)
{
	uiPopulation -= uiDecreasePopulation;
}

void	SOVillage::IncreasePopulation(UI32	uiIncreasePopulation)
{	
	uiPopulation += uiIncreasePopulation;
}

//DB에서 마을 인구수, 마을 수비병,마을 방어력에 관한 정보를 얻어온다.
BOOL	SOVillage::GetDBPopulation()
{	
	DOUBLE	dTempDefence;
	DBIKGetVillageInfo TempGetVillageInfo;
	if(clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEPOPULATION, 
									  LPVOID(&uiVillageCode), LPVOID(&TempGetVillageInfo)) == TRUE)
	{	
		//마을 인구수를 바탕으로 최대 방어력과 최대 차출할수 있는 공성유닛의 수를 얻어온다.
		uiPopulation	   = (UI32)TempGetVillageInfo.uiPopulation;				 // 현재 인구수.
//		m_siDefenceSoldier = (SI32)TempGetVillageInfo.siDefenceSoldier;			 // 현재 마을 수비병 
//		m_siMaxDefenceSoldier = m_siDefenceSoldier;								 // 마을의 최대 수비병.
//		m_siDefenceSoldierInDB = m_siDefenceSoldier;
		vdMaxDefence	   = (UI32)TempGetVillageInfo.uiMaxPopulation/100 * 70;  // 최대 방어력을 업데이트한다.
		if(vdMaxDefence < 100000)
		{
			vdMaxDefence = 100000;
		}
		else if((vdMaxDefence >= 100000) && (vdMaxDefence < 300000))
		{
			dTempDefence	= (DOUBLE)(vdMaxDefence-100000 )/100000;
			vdMaxDefence = (VDEF)(dTempDefence * 10000) + 100000;
		}
		else if((vdMaxDefence >= 300000) && (vdMaxDefence < 500000))
		{
			dTempDefence	= (DOUBLE)(vdMaxDefence-300000 )/100000;
			vdMaxDefence = (VDEF)(dTempDefence * 10000) + 300000;
		}
		else if((vdMaxDefence >= 500000))
		{
			dTempDefence	= (DOUBLE)(vdMaxDefence-500000 )/100000;
			vdMaxDefence = (VDEF)(dTempDefence * 10000) + 500000;
		}

		m_uiLoadQuantity   = (UI32)TempGetVillageInfo.uiMaxPopulation/100 * 30;	 // 마을에서 차출 할수 있는 최대 인구수.
		uiMaxPopulation	   = (UI32)TempGetVillageInfo.uiMaxPopulation;           // 마을의 최대 인구수.

		return TRUE;
	}
	return FALSE;
}

// 마을 인구수,최대 인구수를 저장한다.
void	SOVillage::SetDBPopulation()
{
	UI32	TempIncreasePopulation;
	DOUBLE	dTempDefence;

//	TempIncreasePopulation = uiPopulation/100*5;
	TempIncreasePopulation = uiPopulation * 0.01;

	//증가하는 인구수가 마을의 최대 인구수를 넘어서면 최대 인구수로 고정시킨다.
	if(TempIncreasePopulation + uiPopulation > m_uiLimitMaxPopulation)
	{
		TempIncreasePopulation = m_uiLimitMaxPopulation - uiPopulation;
	}

	//인구수 
	uiPopulation     += TempIncreasePopulation;
	
	//최대 인구수
	uiMaxPopulation  = uiPopulation; 
	
	//고용 할수 있는 최대 인구수.
	m_uiLoadQuantity = uiMaxPopulation/100*30;
	
	//마을의 최대 방어력.
	vdMaxDefence     = uiMaxPopulation/100*70;
	if(vdMaxDefence < 100000)
	{
		vdMaxDefence = 100000;
	}
	else if((vdMaxDefence >= 100000) && (vdMaxDefence < 300000))
	{
		dTempDefence	= (DOUBLE)(vdMaxDefence-100000 )/100000;
		vdMaxDefence = (VDEF)(dTempDefence * 10000) + 100000;
	}
	else if((vdMaxDefence >= 300000) && (vdMaxDefence < 500000))
	{
		dTempDefence	= (DOUBLE)(vdMaxDefence-300000 )/100000;
		vdMaxDefence = (VDEF)(dTempDefence * 10000) + 300000;
	}
	else if((vdMaxDefence >= 500000))
	{
		dTempDefence	= (DOUBLE)(vdMaxDefence-500000 )/100000;
		vdMaxDefence = (VDEF)(dTempDefence * 10000) + 500000;
	}

	//마을 인구수를 바탕으로 마을의 최대 방어력,최대 차출할수 있는 공성유닛의수 등을 저장한다.
	vdDefence = vdMaxDefence;
	SaveDefenceToDB();

	
	DBIKSetVillageInfo	TempSetVillageInfo;
	memset(&TempSetVillageInfo, 0, sizeof(DBIKSetVillageInfo));

	TempSetVillageInfo.uiVillageCode	=	uiVillageCode;
	TempSetVillageInfo.uiPopulation		=	uiPopulation;
	TempSetVillageInfo.uiMaxPopulation	=	uiMaxPopulation;

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASE_POPULATION, LPVOID(&TempSetVillageInfo), sizeof(DBIKSetVillageInfo));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을 공성 관련 함수.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
void	SOVillage::IncreaseDefenceSoldier(SI32 siDefenceSoldier)
{
	m_siDefenceSoldier += siDefenceSoldier;
	m_siMaxDefenceSoldier = m_siDefenceSoldier;
}

void	SOVillage::DecreaseDefenceSoldier(SI32	siDefenceSoldier)
{
	m_siDefenceSoldier -= siDefenceSoldier;

	if(m_siDefenceSoldier <= 0)
		m_siDefenceSoldier = 0;
}

SI32	SOVillage::GetDefenceSoldier()
{
	return m_siDefenceSoldier;
}
*/

void	SOVillage::SetVillageAttackType(SI16 siAttackType)
{
	//m_siVillageAttackType Village가 가지고 있는 마을의 공격형태.
	m_siVillageAttackType	  = siAttackType;
	m_dwPrevAttackdelay		  =	clWorld->dwtimeGetTime;	 //다음 기술을 사용하기 까지 딜레이
	m_dwPrevHitTime			  = clWorld->dwtimeGetTime;  //기술의 발생 시간 체크.

	//마을 방어력일 경우 20초
	if(m_siVillageAttackType == ON_VILLAGE_ATTACK_DEFENCE)
		m_dwSkillDelayTime  = 30 * 1000;

//		m_dwSkillDelayTime	= 20000;
	//돌 공격, 기름 공격 10초
//	else
//		m_dwSkillDelayTime  = 10000;

	m_bAttackTypeDelay        = TRUE;
}


void	SOVillage::InitializeVillageAttackType()
{
	m_siVillageAttackType	=   ON_VILLAGE_ATTACK_NONE;
	m_dwPrevHitTime			=	0;
}

SI16	SOVillage::GetVillageAttackType()
{
	return m_siVillageAttackType;
}

SI16	SOVillage::SendVillageToPlayerAtAttackType(SI16 siAttackType)
{
	UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	SI32		siSendCharNum;
	SI32		i;
	SOPlayer	*pPlayer;

	// 메시지를 받을 플레이어를 찾는다.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	if(siSendCharNum != 0)
	{
		// 필드에서 메시지를 보낼 주체를 찾는다.
		for(i = 0; i < siSendCharNum; i++)
		{
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// 이 마을이 보이는 모든 플레이어들에게 메시지를 보내준다.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_ATTACK_TYPE, SO_SFM_CUSTOM, LPVOID(uiVillageCode), LPVOID(siAttackType));
				break;
			}	
		}	
	}
	return 0;
}

BOOL	SOVillage::GetVillageAttackTypeDelayCheck()
{
	return m_bAttackTypeDelay;
}

BOOL SOVillage::AttackDamage(SOPlayer *pPlayer, SI16 AttackType, BOOL IsLong)
{
	if(pPlayer == NULL) return FALSE;
	if(pPlayer->uiKind < 0) return FALSE;

	CHAR*				pszHighestInvestmentorPlayerName;
	SOPlayer*			pHighestInvestmentorPlayer;
	bool bIsVillageDestroyed = FALSE;
	SI16				siAttackType = ON_RET_NONE_POWER;

	//죽은 유닛인지 검사한다.
	if(pPlayer->FieldAttackDead()) return FALSE;

	if(m_bIsFall)     return FALSE;

	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(pPlayer->uiKind);
	if(siUnitCode < 0) return FALSE;

	DWORD dwNowTick  = GetTickCount();
	DWORD dwTempTime = dwNowTick - pPlayer->m_dwAttackDelay;
	if(dwTempTime < CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode))
	{
		dwTempTime = CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode) - dwTempTime;
		return FALSE; 
	}
	pPlayer->m_dwAttackDelay =  dwNowTick;//현재 시간을 설정해준다.

	int nDamage = CSiegeUnitTable::GetInstance()->GetDamage(siUnitCode);

	if(AttackType == ON_ATTACK_TYPE_NEARCAPTAIN)
	{
		double dTemp = nDamage * 1.2; 
		nDamage = (int)(dTemp + 0.5);
	}
	
	if(pPlayer->GetCaptainUnit() == TRUE)
		nDamage = nDamage * 2;

	SI16 siAttackPower  = clWorld->m_AttackPowerParser.GetAttackPercent(pPlayer->GetFightingPower());
	if((pPlayer->siClassInGuild	==	clWorld->clIGuildClass.GetHNDCode()) || (pPlayer->siVMercenarySlot > 0))
	{
		siAttackPower = 5;
	}
	
	SI16 siRand = rand()%100 + 1;
	//견고한 공격이 히트되었다면.
	if(siAttackPower >= siRand)
	{	
		//원래 주려던 데미지의 1.5배의 데미지를 준다.
		nDamage = nDamage * 1.5;
		siAttackType = ON_RET_ATTACK_POWER;
	}

//	nDamage = 10000;
	DecreaseVillageDefence( nDamage );
	AddDamageAttackedByGuild(pPlayer->uiGuildID, (SI32)nDamage);

	//현제 시간을 설정 해준다.
	m_bVillageAttack		 = TRUE;

	if(vdDefence <= 0)
	{
		if(!m_bIsFall)
		{
			MONEY	TempMoney = 0;
			// 마을이 완전히 파괴되었다.
			// 최투의 투자금만 50%깍는다.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
		
			// 마을을 공격한 상단의 대방에게 보상을 해준다.		
			RewardToGuildForDestroyVillage(TempMoney);
			bIsVillageDestroyed		=	TRUE;
			m_bIsFall               =   TRUE;
		}
	}
	else
	{
		DWORD dwNowTick = GetTickCount();
		if(dwNowTick - pPlayer->m_dwMessageTime <= 30)
		{	// 마을이 공격 당한다는 메시지를 보내준다.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// 마을의 행수에게도 알려준다.
			if( ( pszHighestInvestmentorPlayerName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )				
			{
				if( ( pHighestInvestmentorPlayer = clWorld->GetPlayer( pszHighestInvestmentorPlayerName ) ) != NULL )
				{
					pHighestInvestmentorPlayer->SendFieldMsg( ON_MYVILLAGE_ATTACKED, SO_SFM_ONLYME, LPVOID( uiVillageCode ) );
				}				
			}

			pPlayer->m_dwMessageTime = dwNowTick;
		}
	}

	// 공격에 대한 응답을 보내준다.
	pPlayer->SendFieldMsg(ON_RESPONSE_ATTACK_VILLAGE, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiKind),  //공성유닛속성
						LPVOID(this),			  //마을			
						LPVOID(nDamage),          //데미지값
						LPVOID(siAttackType),     //공격타입  
						LPVOID(IsLong));          //근거리인지 원거리인지...

	return bIsVillageDestroyed;
}

/*
//마을이 근거리 공격을 당한다.
//마을은 플레이어에게 반격할수 있다.
BOOL	 SOVillage::AttackedByPlayer( SOPlayer *pPlayer, SI16 siAttackType)
{
	BOOL				bIsVillageDestroyed = FALSE;	
	SI32				siPlayerNewFightPower = 0;
	SI32				siAttackID = 0;
	SI16				siCastleDamage  = 0;
	SI16				siDefenceDamage = 0;
	CHAR*				pszHighestInvestmentorPlayerName;
	SOPlayer*			pHighestInvestmentorPlayer;
	OnlineFollower*		pFollower = pPlayer->IFollower.GetFollower(0);
	SI16				siAttackPowerType = ON_RET_NONE_POWER;

	// 플레이어가 유효하지 않다면 리턴한다.
	if(pPlayer == NULL)		return FALSE;	

	siCastleDamage = 
/*	siCastleDamage = clWorld->m_FieldBattleParser.GetCalcDamage(clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),
			FIELD_BATTLE_CASTLE,siAttackType);

	siDefenceDamage = clWorld->m_FieldBattleParser.GetCalcDamage(clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),
			FIELD_BATTLE_DEFENSE,siAttackType);	


	SI16 siAttackPower  = clWorld->m_AttackPowerParser.GetAttackPercent(pPlayer->GetFightingPower());

	if((pPlayer->siClassInGuild == clWorld->clIGuildClass.GetHNDCode()) || (pPlayer->siVMercenarySlot > 0))
	{
		siAttackPower  = 5;
	}


	SI16 siRand = rand()%100 + 1;

	//공3업 공격
	if(siAttackPower >= siRand)
	{	
		//원래 주려던 데미지의 1.5배의 데미지를 준다.
		float fTempDamage = (float)siCastleDamage * 1.5;
		siCastleDamage    = (SI16)fTempDamage;

		fTempDamage		   = (float)siDefenceDamage * 1.5;
		siDefenceDamage    = (SI16)fTempDamage;

		siAttackPowerType = ON_RET_ATTACK_POWER;
	}

	if((siCastleDamage > 0) && (vdDefence > 0))
		DecreaseVillageDefence( siCastleDamage );

//	if(siDefenceDamage > 0 && (GetDefenceSoldier() > 0))
//		DecreaseDefenceSoldier((SI16)siDefenceDamage );

	AddDamageAttackedByGuild(pPlayer->uiGuildID,(SI32)siDefenceDamage);

	// 만약 방어력이 0이 되었다면 소속된 상단에서 제거하도록 하고	
	if(m_siDefenceSoldier <= 0)
	{	
		if(clWorld->m_FieldBattleParser.GetUnitKind(pPlayer->uiKind) == FIELD_BATTLE_SWORD)
		{
			MONEY	TempMoney = 0;
			// 마을이 완전히 파괴되었다.
			// 최투의 투자금만 50%깍는다.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
			// 마을을 공격한 상단의 대방에게 보상을 해준다.		
			RewardToGuildForDestroyVillage(TempMoney);
			//최투를 검사.
//			m_Government->VillageInvestment_CheckBestInvestmentor();
			bIsVillageDestroyed		=	TRUE;
		}
	}		
	else
	{		
		// 마을이아직 파괴되지 않았다.

		if(GETTIMEGAP(dwLastAOVillageBeAttacked, clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			dwLastAOVillageBeAttacked	=	clWorld->dwtimeGetTime;

			// 마을이 공격 당한다는 메시지를 보내준다.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// 마을의 행수에게도 알려준다.
			if( ( pszHighestInvestmentorPlayerName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )				
			{
				if( ( pHighestInvestmentorPlayer = clWorld->GetPlayer( pszHighestInvestmentorPlayerName ) ) != NULL )
				{
					pHighestInvestmentorPlayer->SendFieldMsg( ON_MYVILLAGE_ATTACKED, SO_SFM_ONLYME, LPVOID( uiVillageCode ) );
				}				
			}
		}
	}

	m_bVillageAttack			= TRUE;
	m_dwVillageDamageUpdateTime = clWorld->dwtimeGetTime;

	//수비병수  = 수비병의 체력/100 
	SI16 siDefenceSoldier = (SI16)(GetDefenceSoldier()/100);
	//마을이 플레이어를 공격한 데미지를 던져준다.
	//1.수비병수 2.PlayerID 3.VillageAttackType
	SI16 siPlayerDamage= clWorld->m_FieldBattleParser.GetCalcDamageByDefenceSoldier(siDefenceSoldier,
		clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),m_siVillageAttackType);

	pPlayer->DecreaseFieldHp(siPlayerDamage);


	// 공격에 대한 응답을 보내준다.
	pPlayer->SendFieldMsg(ON_RESPONSE_ATTACK_VILLAGE, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiKind),
						LPVOID(this),						
						LPVOID(siCastleDamage),
						LPVOID(siDefenceDamage),
						LPVOID(siPlayerDamage),
						LPVOID(siAttackPowerType));

	return	bIsVillageDestroyed;
}

//마을이 원거리 공격을 당한다.
BOOL	SOVillage::LongDistanceAttack( SOPlayer *pPlayer, SI16 siAttackType)
{
	BOOL				bIsVillageDestroyed = FALSE;	
	SI32				siPlayerNewFightPower = 0;
	SI32				siAttackID = 0;
	SI16				siCastleDamage  = 0;
	SI16				siDefenceDamage = 0;
	CHAR*				pszHighestInvestmentorPlayerName;
	SOPlayer*			pHighestInvestmentorPlayer;
	OnlineFollower*		pFollower = pPlayer->IFollower.GetFollower(0);
	SI16				siAttackPowerType = ON_RET_NONE_POWER;


	siCastleDamage = clWorld->m_FieldBattleParser.GetCalcDamage(clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),
			FIELD_BATTLE_CASTLE,siAttackType);

	siDefenceDamage = clWorld->m_FieldBattleParser.GetCalcDamage(clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),
			FIELD_BATTLE_DEFENSE,siAttackType);	


	// 플레이어가 유효하지 않다면 리턴한다.
	if(pPlayer == NULL)		return FALSE;
	
	SI16 siAttackPower  = clWorld->m_AttackPowerParser.GetAttackPercent(pPlayer->GetFightingPower());

	SI16 siRand = rand()%100 + 1;

	if((pPlayer->siClassInGuild == clWorld->clIGuildClass.GetHNDCode()) || (pPlayer->siVMercenarySlot > 0))
	{
		siAttackPower  = 5;
	}


	//공3업 공격
	if(siAttackPower >= siRand)
	{	
		//원래 주려던 데미지의 1.5배의 데미지를 준다.
		float fTempDamage = (float)siCastleDamage * 1.5;
		siCastleDamage    = (SI16)fTempDamage;

		fTempDamage		   = (float)siDefenceDamage * 1.5;
		siDefenceDamage    = (SI16)fTempDamage;

		siAttackPowerType = ON_RET_ATTACK_POWER;
	}
	

	//마을 방어력을 줄여준다.
	if((siCastleDamage > 0) && (vdDefence > 0))
		DecreaseVillageDefence( siCastleDamage );

	if(siDefenceDamage > 0 && (GetDefenceSoldier() > 0))
		DecreaseDefenceSoldier((SI16)siDefenceDamage );

	AddDamageAttackedByGuild(pPlayer->uiGuildID,(SI32)siDefenceDamage);

	//마을 수비병 수가 0보다 작거나 같으면.
	if(m_siDefenceSoldier <= 0)
	{	
		if(clWorld->m_FieldBattleParser.GetUnitKind(pPlayer->uiKind) == FIELD_BATTLE_SWORD)
		{
			MONEY	TempMoney = 0;
			// 마을이 완전히 파괴되었다.
			// 최투의 투자금만 50%깍는다.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
			// 마을을 공격한 상단에게 보상을 해준다.
			RewardToGuildForDestroyVillage(TempMoney);
			//m_Government->VillageInvestment_CheckBestInvestmentor();
			bIsVillageDestroyed		=	TRUE;	
		}
	}		
	else
	{		
		// 마을이아직 파괴되지 않았다.

		if(GETTIMEGAP(dwLastAOVillageBeAttacked, clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			dwLastAOVillageBeAttacked	=	clWorld->dwtimeGetTime;

			// 마을이 공격 당한다는 메시지를 보내준다.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// 마을의 행수에게도 알려준다.
			if( ( pszHighestInvestmentorPlayerName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )				
			{
				if( ( pHighestInvestmentorPlayer = clWorld->GetPlayer( pszHighestInvestmentorPlayerName ) ) != NULL )
				{
					pHighestInvestmentorPlayer->SendFieldMsg( ON_MYVILLAGE_ATTACKED, SO_SFM_ONLYME, LPVOID( uiVillageCode ) );
				}				
			}
		}
	}

	//수비병수  = 수비병의 체력/100 
	SI16 siDefenceSoldier = (SI16)(GetDefenceSoldier()/100);
	//마을이 플레이어를 공격한 데미지를 던져준다.
	//1.수비병수 2.PlayerID 3.VillageAttackType
	SI16 siPlayerDamage= clWorld->m_FieldBattleParser.GetCalcDamageByDefenceSoldier(siDefenceSoldier,
		clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),m_siVillageAttackType);

	pPlayer->DecreaseFieldHp(siPlayerDamage);

	// 공격에 대한 응답을 보내준다.
	pPlayer->SendFieldMsg(ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiAccount), 
						LPVOID(this),
						LPVOID(siCastleDamage),
						LPVOID(siDefenceDamage),
						LPVOID(siPlayerDamage),
						LPVOID(siAttackPowerType));
	return bIsVillageDestroyed;
}
*/
VOID	SOVillage::RepairVillageDefence( VDEF vddefence, BOOL bSendToDB )
{
	UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	SI32		siSendCharNum;	
	SI32		i;
	SOPlayer	*pPlayer;

	// 현재 Defence가 Max이면 그냥 리턴한다.
	if(vdDefence == vdMaxDefence)	return;

	IncreaseVillageDefence(vddefence);

	// 메시지를 받을 플레이어를 찾는다.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	// 마을의 방어력을 저장한다.	
	if( bSendToDB == TRUE )
	{
		SaveDefenceToDB();
	}

	if(siSendCharNum != 0)
	{
		// 필드에서 메시지를 보낼 주체를 찾는다.
		for(i = 0; i < siSendCharNum; i++)
		{		
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// 이 마을이 보이는 모든 플레이어들에게 메시지를 보내준다.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_DEFENCE, SO_SFM_CUSTOM, LPVOID(uiVillageCode), LPVOID(&vdDefence), LPVOID(siSendCharNum), LPVOID(uiCharUniIDs));
				break;
			}	
		}	
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	파괴된 마을을 uiIncreasePercent pecent 만큼  피를 채워준다.
//	수정 일자	:	2004-04-06 - 이민수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::AutoRestorationDefence(UI08 uiIncreasePercent)
{
	if(vdDefence > 0)
		return FALSE;

	vdDefence = 0;

	VDEF IncreaseDefence = 0;
	IncreaseDefence = vdMaxDefence/2;//(uiIncreasePercent/100);

	if((0x7fffffffffffffff - vdDefence) >= IncreaseDefence)
	{
		vdDefence	=	min(vdDefence + IncreaseDefence, vdMaxDefence);		
	}
	else
		vdDefence	=	0x7fffffffffffffff;    // 10진수로 하면 약922경(922 3372 0368 5477 5807)

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	상승한 마을의 방어력 정보를 보내준다.
//	수정 일자	:	2004-04-06 - 이민수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::AutoRestorationVillageDefence(BOOL bSendToDB)
{
	if(AutoRestorationDefence(50) == FALSE)
		return;

	UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	SI32		siSendCharNum;
	SI32		i;
	SOPlayer	*pPlayer;

	// 메시지를 받을 플레이어를 찾는다.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	// 마을의 방어력을 저장한다.	
	if( bSendToDB == TRUE )
	{
		SaveDefenceToDB();
	}

	if(siSendCharNum != 0)
	{
		// 필드에서 메시지를 보낼 주체를 찾는다.
		for(i = 0; i < siSendCharNum; i++)
		{
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// 이 마을이 보이는 모든 플레이어들에게 메시지를 보내준다.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_DEFENCE, SO_SFM_ONLYME, LPVOID(uiVillageCode), LPVOID(&vdDefence), LPVOID(siSendCharNum), LPVOID(uiCharUniIDs));
				break;
			}	
		}	
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	마을의 방어력을 DB에 저장한다.
//	수정 일자	:	2002-03-29 오후 3:02:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::SaveDefenceToDB()
{
	DBIKVillageDurability	VillageDurability;

	// 저장을할 수 있는 상황인지 체크한다.
	if(vdDefence == vdDefenceInDB)	return;		// 바뀌지 않았다면 저장할 필요가 없다.

	VillageDurability.VillageCode	=	uiVillageCode;
	VillageDurability.Durability	=	vdDefence;

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEDEFENCE_SET, &VillageDurability, sizeof(DBIKVillageDurability));
	
	vdDefenceInDB	=	vdDefence;	
}
/*
VOID	SOVillage::SaveSoldierToDB()
{
	DBIKDecreaseDefenceSoldier	DecreaseDefenceSoldier;

	if(m_siDefenceSoldierInDB == m_siDefenceSoldier) return;

	DecreaseDefenceSoldier.VillageCode      = uiVillageCode;
	DecreaseDefenceSoldier.siDefenceSoldier = m_siDefenceSoldier;

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_SOLDIER, &DecreaseDefenceSoldier, sizeof(DBIKDecreaseDefenceSoldier));

	m_siDefenceSoldierInDB = m_siDefenceSoldier;
}
*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	방어력을 증가 시킨다.
//	수정 일자	:	2002-03-30 오후 2:30:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::IncreaseVillageDefence(VDEF IncreaseDefence)
{
	if((0x7fffffffffffffff - vdDefence) >=  IncreaseDefence)
	{
		vdDefence	=	min(vdDefence + IncreaseDefence, vdMaxDefence);		
	}		
	else
		vdDefence	=	0x7fffffffffffffff;    // 10진수로 하면 약922경(922 3372 0368 5477 5807)
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	방어력을 뺀다.
//	수정 일자	:	2002-03-30 오후 2:30:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOVillage::DecreaseVillageDefence(VDEF DecreaseDefence)
{
	if(vdDefence >= DecreaseDefence)
	{
		vdDefence	-=	DecreaseDefence;	
	}
	else
	{
		vdDefence	=	0;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	방어력을 얻어온다.
//	수정 일자	:	2002-04-01 오후 3:58:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VDEF	SOVillage::GetVillageDefence()
{
	return	vdDefence;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

int     SOVillage::GetEmptySlot()
{
	int nSlot = -1;
	for(int i = 0; i < 100; i++)
	{
		if(strlen(m_szCharName[i]) <= 0) 
		{
			nSlot = i;
			break;
		}
	}

	return nSlot;
}

int SOVillage::SetCharName(char *szName)
{
	if(szName == NULL) return -1;
	int nEmpty = GetEmptySlot();
	if(nEmpty < 0) return -1;

	int i = FindCharName(szName);
	if(i >= 0) return -1;
	
	memcpy(m_szCharName[nEmpty], szName, ON_ID_LENGTH + 1);
	
	return 1;
}

int SOVillage::FindCharName(char *szName)
{
	if(szName == NULL) return -2;
	for(int i = 0; i < 100; i ++)
	{
		if(strcmp(m_szCharName[i], szName) == 0)
		{
			return i;
		}
	}

	return -1;
}

int   SOVillage::DeletCharName(char *szName)
{
	if(szName == NULL) return -1;

	for(int i = 0; i < 100; i ++)
	{
		if(strcmp(m_szCharName[i], szName) == 0)
		{
			strcpy(m_szCharName[i], "");
			break;
		}
	}

	return 1;
}

VOID SOVillage::CheckBestInvestment()
{
	MONEY  mnMoney = 0;
	char   szID[ON_ID_LENGTH + 1] = {0,};
	SOPlayer *pPlayer = NULL;

	if(m_Government->CheckBestInvestment(szID, mnMoney))
	{
		pPlayer = clWorld->GetPlayer( szID );
		if(pPlayer == NULL )
		{
			DBIKGuildFallVillage              TempDBIKGuildFallVillage;
		
			TempDBIKGuildFallVillage.uiVillageCode = uiVillageCode;
			TempDBIKGuildFallVillage.uiGuildID     = 0;
			TempDBIKGuildFallVillage.mnMoney       = mnMoney;

			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_REWARDTODAIFORDESTROYVILLAGE,
												 &TempDBIKGuildFallVillage, sizeof(DBIKGuildFallVillage));
		}
		else
		{			
			DBIKGuildReward		TempDBIKGuildReward;

			TempDBIKGuildReward.siAccount			=	pPlayer->stAccountInfo.siDBAccount;
			TempDBIKGuildReward.uiSlot				=	pPlayer->stAccountInfo.uiSlot;
			TempDBIKGuildReward.mnMoney				=	pPlayer->ModifySaving(mnMoney);
			TempDBIKGuildReward.uiGuildID			=	pPlayer->uiGuildID;
			TempDBIKGuildReward.siIncreaseSupply	=	0;

			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_WAR_PROFIT, &TempDBIKGuildReward, sizeof(DBIKGuildReward));
		}
	}
}

BOOL SOVillage::CheckGuild()
{
	//상단이 이마을을 소유하고 있는지 검사 한다.
	return clWorld->clIGuild.CheckVillage(uiVillageCode);
}
