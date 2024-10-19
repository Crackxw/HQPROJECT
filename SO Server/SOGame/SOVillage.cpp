#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "AuctionList.h"
#include "AuctionMarket.h"

#include "CSiegeUniteTable.h"

// ����������.
clVillageAttackInfo		SOVillage::clVAInfo;	// ������ ���� ���� 
SOWorld					*SOVillage::clWorld;	




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-08-14 ���� 5:44:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage::SOVillage()
{	
	ZeroMemory(szHNSID, sizeof(szHNSID));							// ��� ���̵� �ʱ�ȭ 
	ZeroMemory(VillageCapital, sizeof(VillageCapital));				// 10�⵿���� ������ ���� �ʱ�ȭ 
	
//	pstBankData						=	NULL;						// ������ ����Ÿ.
	pstINNData						=	NULL;						// �ָ��� ����Ÿ.	
	

	siVillageInOutPlayerYetYear		=	0;							// �۳��� ���� ������ �� 
	siVillageInOutPlayerNowYear		=	0;							// ������ ���� ������ �� 

	uiGuildID						=	0;							// ������ �����ϰ� �ִ� ����� ���̵� 
	siGuildFlag						=	0;							// ����� ��� ���̵� 

	CurrentCapital					=	VillageCapital;				// ������ ���� ���� 
	dwLastAOVillageBeAttacked		=	0;							// ������ ���ݴ������� ���� �ֱٿ� ������ �˸� �ð� 


	m_pMarket						=	NULL;
	m_pIndustrialComplexs			=	NULL;
	m_Barrack						=	NULL;
	m_Hospital						=	NULL;
	m_Wharf							=	NULL;
	m_Government					=	NULL;	
	m_pBank							=	NULL;
	m_bIsFall                       =   FALSE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �Ҹ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ʱ�ȭ �Ѵ�.(DB�� ���� �� ��Ȳ���� �޾ƿ´�.)
//	���� ����	:	2002-08-14 ���� 5:46:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::Init(VillageHeader *pVillageHeader)
{	
	DBIKVillageEnterPlauerGet		TempDBIKVillageEnterPlauerGet;
	DBIKVillageEnterPlauerGetInfo	TempDBIKVillageEnterPlauerGetInfo;

	
	// ������ �⺻ ������ �����Ѵ�.
	
	// ������ X, Y ��ǥ�� �������� �Ƶ�� �����Ѵ� 
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
	// �� ���� �ִ� �ǹ����� �����Ѵ�.
	memcpy(siStructureKinds, pVillageHeader->siStructureKinds, sizeof(siStructureKinds));

	// DB���� �� ������ �����ϰ� �ִ� ����� ���´�.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEGUILDID_GET, LPVOID( uiVillageCode ), LPVOID( &uiGuildID ) ) == FALSE )		return FALSE;		
	
	if(uiGuildID != 0)
	{
		// ����� ���̵� ���´� 
		siGuildFlag	= clWorld->clIGuild.GetFlag(uiGuildID);

		// �� ������ �ش� ��ܿ� �Ҽ� ��Ų��.
		clWorld->clIGuild.AddVillage(uiGuildID, this, TRUE);
	}
	else
		siGuildFlag	=	0;

	// DB���� ����� �̸��� ���´�.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEHNSNAME_GET, LPVOID( uiVillageCode ), LPVOID( szHNSID ) ) == FALSE )			return FALSE;
	
	// DB���� ������ ������ ���´�.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEDEFENCE_GET, LPVOID( uiVillageCode ), LPVOID( &vdDefenceInDB ) ) == FALSE )	return FALSE;		

	// ������ �����Ѵ�.
	vdDefence	=	vdDefenceInDB; 
	
	// �� ������ �α��� DB�� ���� ���´�.
	// �ִ� ���� �α��� ���� ���� �����α��� ���� ���񺴼��� �����Ѵ�.
	if(GetDBPopulation() == FALSE) return FALSE;
	
	// �� ������ �ִ� ��� �ǹ����� �ʱ�ȭ �Ѵ�.
	if( InitStructure( pVillageHeader ) == FALSE )		return	FALSE;	

	// ���� �ȳ����� ���� �ڵ�� ���´�
	memset(szDescription,0, sizeof(szDescription));		
	GetDescription();									
	GetNationCode();					

	// DB���� ������ 10 �Ⱓ ������ ���´�.
	GetVillageCapital();

	// DB ���� ���� ���� ������ ���� ����
	siVillageInOutPlayerYetYear = 0;
	siVillageInOutPlayerNowYear = 0;

	m_siVillageAttackType	  = ON_VILLAGE_ATTACK_NONE;
	m_dwPrevAttackdelay		  =	0;	 //���� ����� ����ϱ� ���� ������
	m_dwPrevHitTime			  = 0;  //����� �߻� �ð� üũ.
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

	// �ٰ��� �峯�� �����Ѵ�. (������ ���� ������ �峯�� �ǰ���..)
	InitMarketDay();

	return TRUE;
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���������� DB���� ������Ʈ�Ѵ�.(�õ��� & ����)
//	���� ����	:	2002-08-14 ���� 5:55:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//	����	:	�÷��̾ �������� �α� �ƿ� ���� ��� ȣ���� �ش�.
//----------------------------------------------------------------------------------------------------------------
VOID	SOVillage::PlayerLogOut(SOPlayer *pPlayer)
{
	// �ǹ������� �������´�.
	LeaveStructure( pPlayer );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Polling
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// �峯�� �Ǹ� ������ ������ ������ ��ȭ��Ű�� ���� �峯�� �����Ѵ�. (�������� �峯�� �ٸ�)
		//if((clWorld->CurrentTime.Day == 1) || (clWorld->CurrentTime.Day == 8) || (clWorld->CurrentTime.Day == 15) || (clWorld->CurrentTime.Day == 22))
		if( GetNextMarketDay() == clWorld->CurrentTime.Day )
		{
			m_pMarket->ProcessProduceReduceItem( uiPopulation );
			SetNextMarketDay();															// ���� �峯�� �����Ѵ�.
		}

		// ���࿡ ���� �ٲ��ٸ� �������� Ư���� ���� ���� �� �Һ��Ѵ�.
		if(clWorld->bMonthChanged == TRUE)
		{
			//VillageInvestment_Updata(1);
			if(clWorld->bYearChanged == TRUE)
			{
				for(i = ON_STATISTICS_YEAR; i > 1; i--)
					memmove(&VillageCapital[i-1], &VillageCapital[i-2], sizeof(stCapital));

//				memcpy(&VillageCapital[0], CurrentCapital, sizeof(stCapital));

				// ������ ���ͱ� �ʱ�ȭ.
				memset(CurrentCapital, 0, sizeof(stCapital));
				CurrentCapital->Year	=	clWorld->CurrentTime.Year;
			}
		}

		// �Ϸ簡 ������.

		// ���� �ü����� �ִٸ� �����ϵ��� ���� ���� �ü����� ���ְ� 
		// �α��� / 500 ��ŭ �뵿�� ���ش� �ְ� 1000.
		if( DoesFactoryExist() == TRUE )
		{
			UI32 uiNPCLabor;
			uiNPCLabor = min(1000,uiPopulation / 500);
			m_pIndustrialComplexs->DayChanged( clWorld ,uiNPCLabor);
		}
		

		// ������ ������ �ִٸ� pstBankData ������Ʈ �Լ�...
//		UpdateBankInfo();

		// �Ϸ縶�� ���� ������ �ٳణ ����� ����
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
					// ���� ������ �ְ� ������ �˻�
					m_Government->VillageInvestment_CheckBestInvestmentor();

					//=====================================================================================
					// ��� ó��
					UI32		TempMaxInvestmentorGuild;

					TempMaxInvestmentorGuild = m_Government->GetInvestmentMaxInvestmentorGuild();				

					if(uiGuildID != 0)
					{
						//=====================================================================================
						// ���� ������ ��尡 �������
						// �ְ� ������ �˻�
						if(TempMaxInvestmentorGuild != uiGuildID)
						{
							// �ְ� �����ڰ� ������ ������ �ƴҶ�, �� ������ ��忡�� �ع��Ų��.
							clWorld->clIGuild.DelVillage(uiGuildID, this, FALSE);
							uiGuildID = 0;
						}					
					}
           
					if(uiGuildID == 0)
					{
						//=====================================================================================
						// ���� ������ ��尡 �������
						// �ְ� �����ڰ� �������, �ְ� �����ڰ� ��忡 ���Եž� ������, ������ �� ����� �Ҽ����� �ٲ�� ��
						if(TempMaxInvestmentorGuild != 0)
						{
							clWorld->clIGuild.AddVillage(TempMaxInvestmentorGuild, this, FALSE);
						}
					}
				}
			}
		}

		// �ε� ���� ó��(���׽ð��� �Ÿ� �� ǥ�� ���� ������� ��� ���׽�Ŵ)
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
							// ���׽ð����� �˻�
							TicketHeader*		lpTicketHeader;

							lpTicketHeader = clWorld->clTicketParser.GetTicket(clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind);
							if(lpTicketHeader)
							{
								if((clWorld->CurrentTime.Hour % lpTicketHeader->siStartTime) == 0)
								{
									// ǥ�� ����
									DBIKResetTicket			TempDBIKResetTicket;

									TempDBIKResetTicket.DBAccount    = clWorld->clPlayer[TempCount]->stAccountInfo.siDBAccount;
									TempDBIKResetTicket.uiSlot       = clWorld->clPlayer[TempCount]->stAccountInfo.uiSlot;
									clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_RESETTICKET, LPVOID(&TempDBIKResetTicket), sizeof(TempDBIKResetTicket));
									clWorld->clPlayer[TempCount]->uiStructerInPos = STRUCTERPOS_NONE;

									// ���� Message
									//clWorld->clPlayer[TempCount]->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

									// �̵��Ϸ��� �� List�� �߰�
									TempUserTicketInfo[TempUserCount] = clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind;
									TempUserInfo[TempUserCount]       = clWorld->clPlayer[TempCount]->uiAccount;
									TempUserCount++;

									// ������� ���� ������ �̵�
									clWorld->clPlayer[TempCount]->LeaveVillage();
								}
							}
						}
					}
				}
			}
		}

		// �̵��ϴ� ������� ������, �� List�� �߰�
		if(TempUserCount)
		{
			// �켱 �� Kind �� ����
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

			// ���ĵ� Kind�� �˻�
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

			// �� �̵�
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

		// ������ ������ DB�� �����Ѵ�. (���� �ð��ϰ�쿡��)
		if( clWorld->IsAttackVillageTimeZone() == TRUE )
		{			
			SaveDefenceToDB();
//			SaveSoldierToDB();
		}
	}

	//���� �ð��� �Ǹ� Village Attack Type�� ������� �����Ѵ�.
	if(GetVillageAttackType() != ON_VILLAGE_ATTACK_NONE)
	{
		if(GetVillageAttackType() == ON_VILLAGE_ATTACK_OIL)
		{
			if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(10000))
			{
				 //������ AttackType�� ������� ������.
				 InitializeVillageAttackType();
				 //���� ������ �÷��̾�鿡�� AttackType�� ������� ���ƿԴٴ°��� �˸���.
				 SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			}
		}
		else if(GetVillageAttackType() == ON_VILLAGE_ATTACK_DEFENCE)
		{
			//if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(15000))
			//{
				 //������ AttackType�� ������� ������.
				 InitializeVillageAttackType();
				 //���� ������ �÷��̾�鿡�� AttackType�� ������� ���ƿԴٴ°��� �˸���.
				 //SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			//}
		}
		else if(GetVillageAttackType() == ON_VILLAGE_ATTACK_STONE)
		{
			if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(15000))
			{
				 //������ AttackType�� ������� ������.
				 InitializeVillageAttackType();
				 //���� ������ �÷��̾�鿡�� AttackType�� ������� ���ƿԴٴ°��� �˸���.
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
	//������ �ϰ� �ִ� �����̶��.
	if(m_bVillageAttack == TRUE)
	{
		if(GETTIMEGAP(m_dwVillageDamageUpdateTime,clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			m_dwVillageDamageUpdateTime = clWorld->dwtimeGetTime;
			
			if(uiGuildID > 0)//������ �����ϰ� �ִ� ��尡 �ִ��� �˻�
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

							writeInfoToFile("GuildDamage.txt", "���� �Ҽ� ���: %d, VillageID:%d, AttackGuild:%d, Damage:%d", 
								             uiGuildID, uiVillageCode, m_stAttackGuild[i].uiGuildID, m_stAttackGuild[i].siDamage);	
							
							clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_VILLAGE_DAMAGE, &TempDBIKVillageDamage, sizeof(DBIKVillageDamage));
						}
					}
				}
			}
			
		}
	}

	if(clWorld->bUpdateTime)		// ���� 11�ÿ� TRUE ���°� �ȴ�.
	{
		//�α��� ���,���� ���� ������ �������� �Ѵ�.
		printf("�α���, ���� ������Ʈ ����");
		SetDBPopulation();
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::EnterVillage(SOPlayer *pPlayer)
{
	siVillageInOutPlayerNowYear++;

	return TRUE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �ִ� ���� id�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	SOVillage::GetMapID()
{
	return uiMapID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	DB���� ���� �ȳ����� �����´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::GetDescription()
{
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEDESCRIPT, LPVOID(uiVillageCode),LPVOID(&szDescription)) == FALSE)	
	{
		memset(szDescription,0, sizeof(szDescription));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	DB���� ������ ���� �ڵ带 �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::SetNationCode(CHAR NationCode)
{
	DBIKVillageNation		VillageNation;
	VillageNation.VillageCode	=	uiVillageCode;
	VillageNation.Code			=	NationCode;
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGE_SETNATION, &VillageNation, sizeof(DBIKVillageNation));
	
	cNation	=	NationCode;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	DB���� ������ ���� �ڵ带 �����´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::GetNationCode()
{
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGE_GETNATION, LPVOID(uiVillageCode),LPVOID(&cNation));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �̸��� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
CHAR*	SOVillage::GetVillageName()
{
	return clWorld->clOnlineText.Get(uiVillageName);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� �ǹ��� �� ���� �ȿ� �ִ°�?
//	���� ����	:	2002-10-04 ���� 6:59:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ������ ���� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOVillage::GetVillageInOutPlayer(void)
{
	if(siVillageInOutPlayerYetYear > siVillageInOutPlayerNowYear) return siVillageInOutPlayerYetYear;

	return siVillageInOutPlayerNowYear;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	1���� �������� ������ �����ڼ��� �����Ѵ� 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
void	SOVillage::PrepareVillageInOutPlayer(void)
{
	siVillageInOutPlayerYetYear = siVillageInOutPlayerNowYear;
	siVillageInOutPlayerNowYear = 0;

	// DB �� ����
	DBIKVillageEnterPlauerSet		TempDBIKVillageEnterPlauerSet;

	TempDBIKVillageEnterPlauerSet.uiVillageCode		=	uiVillageCode;
	TempDBIKVillageEnterPlauerSet.uiPlayerNum		=	siVillageInOutPlayerYetYear;
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEENTERPLAYERSET, LPVOID(&TempDBIKVillageEnterPlauerSet), sizeof(TempDBIKVillageEnterPlauerSet));
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ �ִ� ����� ���� ID�� ���´�.
//	���� ����	:	2002-02-27 ���� 11:09:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	SOVillage::GetGuildNID()
{
	return uiGuildID;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ �ִ� ����� ���� ID�� ���´�.
//	���� ����	:	2002-02-27 ���� 11:09:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������	
CHAR*	SOVillage::GetGuildSID()
{
	return 	clWorld->clIGuild.GetGuildName(uiGuildID);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ �ҼӵǴ� ����� �����Ѵ�.
//	���� ����	:	2002-05-10 ���� 9:32:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::SetOwnerGuild(UI32 uiguild, BOOL bInit)
{
	DBIKGuildOwnerVillageInfo	GuildOwnerVillageInfo;

	// �� ������ ����� ID�� �����Ѵ�.
	uiGuildID		=		uiguild;

	// �ʱ�ȭ ���̸� �׳� �����Ѵ�.
	if(bInit == TRUE)	return;	

	GuildOwnerVillageInfo.uiVillageCode	=	uiVillageCode;
	GuildOwnerVillageInfo.uiGuildID		=	uiGuildID;
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEGUILDID_SET, &GuildOwnerVillageInfo, sizeof(DBIKGuildOwnerVillageInfo));
	
	// �� ������ �ְ� �����ڸ� ����� �������ش�.
	SetHNSID(m_Government->GetInvestmentMaxInvestmentorName());	
	// �� ������ ����� �Ҽ� ����� ����� ������ �ȴ�.
	ChangeGuildFlag(clWorld->clIGuild.GetFlag(uiguild));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ڵ带 �ش�.(����� ������, ���� �������·� ������, ��� ���·�)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ ���ο� '���'�� �����Ѵ�.
//	���� ����	:	2002-03-20 ���� 11:35:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::SetHNSID(CHAR *pszHNSID)
{
	SI32				siHNSClassCode	=	clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS);	

	DBIKVillageMaster	VillageMaster;

	// ���� ����� ������ �����Ѵ�.
	if(uiGuildID == 0)					return	FALSE;
	if(pszHNSID == NULL)				return	FALSE;
	if(strcmp(szHNSID, pszHNSID) == 0)	return	FALSE;

	// ������ '���'�� �ذ��Ѵ�.
	DismissalHNS();

	// ���ο� '���'�� �̸��� DB�� �����Ѵ�.
	ZeroMemory(&VillageMaster, sizeof(DBIKVillageMaster));
	VillageMaster.VillageCode	=	uiVillageCode;
	strcpy( VillageMaster.Name, pszHNSID );
	
	// �������� �ƴҶ�	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEHNSNAME_SET, &VillageMaster, sizeof(DBIKVillageMaster));
	
	
	// '���'�� �̸��� ������ �����Ѵ�.
	ZeroMemory(szHNSID, sizeof(szHNSID));
	strncpy(szHNSID, pszHNSID, ON_ID_LENGTH);
	

	cltGuild *pGuild = clWorld->clIGuild.GetGuildByID(uiGuildID);

	strGuildCommonMemberInfo MemberInfo;

	// �� ĳ������ �ִ� ������ ���Ѵ�.
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
	
	// ���� ���� �ڵ尡 ������� ũ�ų� ������ ������ �ʿ䰡 ����.
	if(clWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siHNSClassCode) == SO_GUILD_CLASSCOMP_LOW)
	{
	
		// ����� �±޽����ش�.

		// DB�� ������ �ϰ�
		// ������ �ϰ� �ִٸ� ������ ����Ǿ��ٴ� �޽����� �����ش�.	
		clWorld->clIGuild.ChangeClass( uiGuildID, pszHNSID, siHNSClassCode );	
	}

	return	TRUE;
		
	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �������� �� ĳ������ �ְ� ������ ���´�.
//	���� ����	:	2002-03-25 ���� 10:13:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOVillage::GetHighestGuildClass(CHAR *pszID)
{
	// ������ ��� �ۿ� ����.
	if(strcmp(szHNSID, pszID) == 0)
		return clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS);
	else
	{
		// �������� ��ȯ�̴�.
		return clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);
	}

	return	0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ �ְ� �������̸鼭 ��ܿ� ���ԵǾ� �ִ����� 
//					üũ�Ѵ�.
//	���� ����	:	2004-02-04 ���� - �̹μ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL   SOVillage::IsVestInvestmentorAndGuildMember(const CHAR* pszName)
{
	m_Government->VillageInvestment_CheckBestInvestmentor();	
	UI32		TempMaxInvestmentorGuild;
	TempMaxInvestmentorGuild = m_Government->GetInvestmentMaxInvestmentorGuild();

	return ((TempMaxInvestmentorGuild != 0 ) &&
		(strcmp(pszName, m_Government->GetInvestmentMaxInvestmentorName()) == 0)) ? TRUE : FALSE;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �������� �� ĳ���Ͱ� ������ �ִ� ������ �ʱ�ȭ �Ѵ�.
//					�ش� ������ �����ϰ�, ��ó���� ���ش�.
//					���� �� ������ ��ܿ��� �����Ǿ�� �Ѵٸ� TRUE�� �����Ѵ�.
//					�׷��� ������ FALSE�� �����Ѵ�.
//	���� ����	:	2002-05-09 ���� 7:53:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DeleteGuildClass(CHAR *pszName)
{
	///////////////////////////////////////////////////////////////////////
	//BOOL bVal = IsVestInvestmentorAndGuildMember(pszName);	
	///////////////////////////////////////////////////////////////////////
	
	BOOL bRetVal = FALSE;

	// �̸��� ���ٸ� �׳� �����Ѵ�.
	if(strcmp(pszName, "") == 0) bRetVal = FALSE;

	else if(strcmp(szHNSID, pszName) == 0)
	{		
		// ����� pszName�� ������ ���شٴ°��� 
		// �� �� ������ ��ܿ��� �����Ѵٴ°��̴�.
		// ���ÿ� pszName�� ĳ���ʹ� ����� ������ ��������.

		// ����� �ذ��Ѵ�.			
		DismissalHNS();

		bRetVal = TRUE;
	}

	// ������ ��� �̸��� �߸��Ǿ� �ִ°��.
	/*if(bVal == TRUE && bRetVal == FALSE)
	{
		writeInfoToFile( "DeleteGuildError.txt", 
			"Invalid Hangsu name. Hangsu name is %s, MaxInvestmentor name is %s", 
			szHNSID, pszName);
	}*/

	return	bRetVal;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ������ ��尡 �����Ǿ���.
//	���� ����	:	2002-03-25 ���� 11:04:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::DeleteGuild()
{	
	DBIKGuildOwnerVillageInfo	GuildOwnerVillageInfo;

	GuildOwnerVillageInfo.uiVillageCode	=	uiVillageCode;
	GuildOwnerVillageInfo.uiGuildID		=	uiGuildID;

//	m_siDefenceSoldier					=	0;
//	m_siMaxDefenceSoldier				=	0;	
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEOWNER_DELETE, &GuildOwnerVillageInfo, sizeof(DBIKGuildOwnerVillageInfo));

	// ����� �����Ѵ�.
	DeleteGuildClass(szHNSID);

	
	// ��ܰ� ����� ����� �ʱ�ȭ �Ѵ�.
	uiGuildID		=	0;
	siGuildFlag		=	0;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ����� �ذ��Ѵ�.	
//	���� ����	:	2002-04-30 ���� 9:36:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::DismissalHNS()
{
	SI32		siHighestClassCode;	

	SI32		siHNSClassCode	=	clWorld->clIGuildClass.GetHNSCode();					// '���'�� �ڵ带 �̸� ������ �ִ´�.
	SI32		siDHSClassCode	=	clWorld->clIGuildClass.GetDHSCode();					// '�����'�� �ڵ带 �̸� ������ �ִ´�.
	CHAR		szPrevHNSID[ON_ID_LENGTH + 1];

	// ���� ����� ���ų� ����� ������ �����Ѵ�.
	if(uiGuildID == 0)					return;		
	if(szHNSID[0] == NULL)				return;		

	// �� ������ ����� �̸� ����صд�.
	ZeroMemory(szPrevHNSID, sizeof(szPrevHNSID));
	strncpy(szPrevHNSID, szHNSID, ON_ID_LENGTH);	 

	// �� ������ ����� ���ش�.(�� �κ��� �ݵ�� �Ʒ� �κ� ���� ���� �־���Ѵ�. �Ʒ��� GetHighestGuildClass �Լ����� �� ������ ���� �ϱ� �����̴�.) 	
	ZeroMemory(szHNSID, sizeof(szHNSID));

	// ������� ����� �ִ� ������ ���Ѵ�.
	cltGuild *pGuild = clWorld->clIGuild.GetGuildByID(uiGuildID);
	if (pGuild != NULL)
	{

		strGuildCommonMemberInfo MemberInfo;
		if (pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szPrevHNSID,&MemberInfo) == FALSE) return;			
				
		// ���� �����ڿ��� ��� ���� �����̾���.
		if (MemberInfo.siClassInGuild == 90) return;

		// ������ �������� ����� ������� ���� ��å�� �ƴϾ��� ��쿡��... ����̳� ������� ������ �Ҿ ��å�� �����ϹǷ�..
		if( clWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siHNSClassCode) != SO_GUILD_CLASSCOMP_HIGH )	// ���±��� '���' ������ �ּ��� '���'�� ���� ���̴�.
		{			
			// ���� ���� �󿡼� �� ĳ������ �ִ� ������ ���´�.
			siHighestClassCode	=	clWorld->GetHighestGuildClass(szPrevHNSID);		// �� ĳ���Ͱ� ����� ������ �ִ��� ��� ���� �˻�
			// ���� ������ ����� ���ް� �ٸ��ٸ� �ٲپ��ش�.
			if(siHighestClassCode != MemberInfo.siClassInGuild) clWorld->clIGuild.ChangeClass(uiGuildID, szPrevHNSID, siHighestClassCode);			
		}
	}
	else
	{
		return;
	}				
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ����.
//	���� ����	:	2002-09-30 ���� 1:12:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage::GetVillageInfo(SOPlayer *pPlayer, OnRecvVillageInfo *pVillageInfo)
{
	CHAR	*pszInvestmentorName;

	if(pVillageInfo)
	{
		ZeroMemory(pVillageInfo, sizeof(OnRecvVillageInfo));

		pVillageInfo->uiPopulation	=	uiPopulation;

		// �ְ� �������� ������ ���´�.
		if((pszInvestmentorName = m_Government->GetInvestmentMaxInvestmentorName()) != NULL)
			strcpy(pVillageInfo->m_MaxInvestmentID, pszInvestmentorName);
		
		// ������ ������ ���´�.
		pVillageInfo->vdVillageDefence		=	vdDefence;

		// ������ �ִ� ������ ���´�.
		pVillageInfo->vdVillageMaxDefence	=	vdMaxDefence;
//		pVillageInfo->siDefenceSoldier		=	m_siDefenceSoldier;
//		pVillageInfo->siMaxDefenceSoldier	=	m_siMaxDefenceSoldier;
		pVillageInfo->siAttackType			=	m_siVillageAttackType;
	
		// ����� ������ ���´�.
		if(uiGuildID != 0)
		{
			// ����� �ִٸ� ����� ������ ���´�.
			strncpy(pVillageInfo->szGuildID, GetGuildSID(), ON_GUILDNAME_LENGTH);
		}

		// �÷��̾�� ��ܰ��� ���踦 ���´�.
		pVillageInfo->dwGuildRelation	=	ON_RET_GLD_NEUTRAL;

		// ��� ������� �˾ƿ´�.
		if((pPlayer->uiGuildID != 0) && (uiGuildID != 0) && (pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode()))
		{
			if(pPlayer->uiGuildID == uiGuildID)							
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_MINE;	// �� ������ ��ܿ��̴�.
			else if(clWorld->clIGuild.IsHostilityRelation(uiGuildID, pPlayer->uiGuildID) == TRUE)
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_ENEMY;	// �������� ������ ����̴�.
			else if(clWorld->clIGuild.IsEnemyRelationWar(uiGuildID, pPlayer->uiGuildID) == TRUE)
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_WAR;	// ���� �ϴ� ���̴�.
		}

		// ���� �ڵ带 ���´�.
		pVillageInfo->cNationCode	=	cNation;
	
		// ����� ���´�.	
		pVillageInfo->siGuildFlag	=	siGuildFlag;

		return	sizeof( OnRecvVillageInfo );
	}
	else
	{
		return	0;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ����
//	���� ����	:	2002-04-04 ���� 2:52:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOVillage::GetGuildFlag()
{
	return siGuildFlag;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� �����Ѵ� 
//	���� ����	:	2002-04-04 ���� 3:43:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::ChangeGuildFlag(SI32 siFlag)
{
	siGuildFlag	=	siFlag;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� On �� �ʱ� ���� �ں��� �о� ���δ�.
//	���� ����	:	2002-04-04 ���� 5:00 - �̼���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::GetVillageCapital()
{	
	BOOL bRet;
	bRet = clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETVILLAGECAPITAL, this, NULL);

	return bRet;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ������ ����ǹ��� ���´�.
//	���� ����	:	2002-10-04 ���� 7:23:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
stFarmData*	SOVillage::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	if( DoesFactoryExist() == FALSE )			return	NULL;
	
//	return	m_pFactory->GetOwnFactory( siPlayerDBAccount, uiPlayerDBSlot );	
	return	NULL;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-08-07 ���� 2:36:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::InitStaticVariable(SOWorld *pWorld)
{
	clWorld		=	pWorld;

	SOVillage_Market::InitStaticVariable( &clWorld->clItem, &clWorld->clIProfit, clWorld->clDatabase );
	SOVillage_IndustrialComplexs::InitStaticVariable( &clWorld->clItem, clWorld->clDatabase, &clWorld->clBuilding );
	SOVillage_Bank::InitStaticVariable( clWorld->clDatabase, &clWorld->clOnlineEconomy );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ִ� �ǹ����� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-01 ���� 7:12:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
			
		case ON_VIL_ST_KIND_MARKET :						// �����̴�.
		case ON_VIL_ST_KIND_WSTORE : 
		case ON_VIL_ST_KIND_FSTORE :
		case ON_VIL_ST_KIND_DSTORE :
			m_pMarket	=	new	SOVillage_Market;
			
			// ������ �ʱ�ȭ �Ѵ�.		
			if( m_pMarket->Init( uiVillageCode, pVillageHeader->uiItemID, pVillageHeader->siProperty, pVillageHeader->siQuantity ) == FALSE )	return	FALSE;
			
			break;
			

		case ON_VIL_ST_KIND_INN :							// �ָ��̴�.

			break;
			

		case ON_VIL_ST_KIND_BANK :							// �����̴�.
		
			m_pBank		=	new	SOVillage_Bank();

			if( m_pBank->Init( uiVillageCode ) == FALSE )		return	FALSE;
/*
			// �� ������ ������ �ִٸ� ������ ����Ÿ�� �����Ѵ�.
			pstBankData		=	new	stBankData;
			ZeroMemory(pstBankData, sizeof(stBankData));
			UpdateBankInfo();
*/
			
			break;


		case ON_VIL_ST_KIND_WHARF :							// �ε��̴�.
			m_Wharf		=	new	SOVillage_Wharf;
			
			// ������ �ʱ�ȭ �Ѵ�.		
			if( m_Wharf->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;
			break;

		// ��ż��̴�.
		case ON_VIL_ST_KIND_AUCTION :	
			break;


		case ON_VIL_ST_KIND_FARM :							// ���� �ü����̴�.
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

			// ���� �ü��� �ʱ�ȭ �Ѵ�.
			//if( m_pFactory->Init( uiVillageCode, clWorld->clBuilding.GetPlants( pszPlantCode ) ) == FALSE )		return	FALSE;			
			if( m_pIndustrialComplexs->Init( uiVillageCode, cStructureKind ) == FALSE )							return	FALSE;			

			// ���� �ü��� ���� ����Ÿ�� DB���� ���´�.
			if( GetFarmData() == FALSE )																		return	FALSE;

			break;

		case ON_VIL_ST_KIND_HOSPITAL:
			m_Hospital	=	new	SOVillage_Hospital;
			
			// ������ �ʱ�ȭ �Ѵ�.		
			if( m_Hospital->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this) == FALSE )	return	FALSE;
			break;

		case ON_VIL_ST_KIND_BARRACK:
		case ON_VIL_ST_KIND_GBARRACK:
		case ON_VIL_ST_KIND_MBARRACK:
			if(m_Barrack == NULL)
			{
				m_Barrack	=	new	SOVillage_Barrack;
				
				// ������ �ʱ�ȭ �Ѵ�.		
				if( m_Barrack->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;
			}
			break;

		case ON_VIL_ST_KIND_GOVERNMENT:
			m_Government	=	new	SOVillage_Government;
			
			// ������ �ʱ�ȭ �Ѵ�.		
			if( m_Government->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;			
			break;
		}		
	}
	
	return	TRUE;
}	

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ǹ��� ����.
//	���� ����	:	2002-10-02 ���� 7:47:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::EnterStructure( SOPlayer *pPlayer, SI32 siStructureKind, LPARAM lParam1, LPARAM lParam2 )
{ 
	
	switch( siStructureKind )
	{

	case ON_VIL_ST_KIND_MARKET :						// �����̴�.
	case ON_VIL_ST_KIND_WSTORE : 
	case ON_VIL_ST_KIND_FSTORE :
	case ON_VIL_ST_KIND_DSTORE :
		
		if( m_pMarket->EnterPlayer( pPlayer ) == FALSE )							return	FALSE;
		
		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_MARKET;

		break;	
		
	case ON_VIL_ST_KIND_FARM :							// ���� �ü����̴�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ǹ����� ������.	
//	���� ����	:	2002-10-02 ���� 7:47:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-04 ���� 6:56:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DoesHospitalExist()
{
	return	DoesStructureExist( ON_VIL_ST_KIND_HOSPITAL );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-04 ���� 6:59:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DoesMarketExist()
{
	if( DoesStructureExist( ON_VIL_ST_KIND_MARKET ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_WSTORE ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_FSTORE ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_DSTORE ) == TRUE )		return	TRUE;

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���� �ü����� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-04 ���� 7:00:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DoesFactoryExist()
{
	if( DoesStructureExist( ON_VIL_ST_KIND_FARM ) == TRUE )			return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_WEAPON ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_ARMOR ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_FACTORY ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_PASTURE ) == TRUE )		return	TRUE;

	return	FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-18 ���� 3:41:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DoesBankExist()
{
	return	DoesStructureExist( ON_VIL_ST_KIND_BANK );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ���´�.
//	���� ����	:	2002-11-27 ���� 2:50:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::GetInfo( OnGuildVillageInfo *pGuildVillageInfo )
{
	CHAR		*pMaxInvestmentorName;

	ZeroMemory( pGuildVillageInfo, sizeof( OnGuildVillageInfo ) );

	// ������ �ڵ� ����
	pGuildVillageInfo->uiVillageCode			=	uiVillageCode;	
	
	if( m_Government != NULL )
	{
		// ������ �ְ� ������ ����.
		if( ( pMaxInvestmentorName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )
		{
			strncpy( pGuildVillageInfo->szBestInvestorName, pMaxInvestmentorName, ON_ID_LENGTH );
		}			

		// ������ �ְ� ���ڱ� ����.
		pGuildVillageInfo->mnBestInvestorMoney		=	m_Government->GetInvestmentMaxInvestmentorMoney();
	}	

	// ������ ���� ����.
	pGuildVillageInfo->vdVillageDefencePower	=	vdDefence;

	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ����� ���ݿ� ���ؼ� ���� �������� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2003-01-01 ���� 5:11:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::InitDamageAttackedByGuild()
{
	ZeroMemory( m_stAttackGuild, sizeof( m_stAttackGuild ) );	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	
//	���� ����	:	2003-01-01 ���� 5:16:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	
//	���� ����	:	2003-01-01 ���� 5:28:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// 10���� ��ܿ��Ը� ������ ���ش�.	
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

			// ����� ��濡�� ������� �ش�.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ����� ������ ���´�.
//	���� ����	:	2003-01-03 ���� 10:43:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ���۵Ǿ���.
//	���� ����	:	2003-01-14 ���� 3:51:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::EnterAttackVillageTimeZone()
{	
	InitDamageAttackedByGuild();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ���۵Ǿ���.
//	���� ����	:	2003-01-14 ���� 3:51:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::LeaveAttackVillageTimeZone()
{
	m_bVillageAttack = FALSE;
	m_dwVillageDamageUpdateTime = 0;
	ZeroMemory(m_stAttackGuild,sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �峯�� ���´�.
//	���� ����	:	2003-04-11 ���� 11:00: - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOVillage::GetNextMarketDay()
{
	return m_siNextMarketDay;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �峯�� �����Ѵ�.
//	���� ����	:	2003-04-11 ���� 11:03: - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::SetNextMarketDay()
{
	SI32		siMaxDay;

	// 3����, 5���� �Ǵ� 7������ �����ϰ� �����Ͽ� �峯 ����
	switch( rand() % 3 )
	{
	case 0:
		m_siNextMarketDay += 3;			break;
	case 1:
		m_siNextMarketDay += 5;			break;
	case 2:
		m_siNextMarketDay += 7;			break;
	}

	// ���� ���� �ϼ��� ���Ѵ�.
	siMaxDay	=	clWorld->Timer.GetDaysInMonth( clWorld->CurrentTime );

	// ���� ������ ���� �Ѿ�� ��� �������� ���ڷ� �ٲ۴�.
	if( m_siNextMarketDay > siMaxDay )	m_siNextMarketDay	-=	siMaxDay;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�峯�� �ʱ�ȭ�Ѵ�.
//	���� ����	:	2003-04-11 ���� 02:30: - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::InitMarketDay()
{
	// ���� ��¥�� �ʱ�ȭ�� ���� ���� �峯�� �����صд�.
	m_siNextMarketDay	=	clWorld->CurrentTime.Day;
	SetNextMarketDay();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �α� ���� �Լ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	
	//��밡�� �α��� (�ִ��α����� 30% �̳�)�� �ʰ����� ������
/*	if(m_uiLoadQuantity >= uiTempPopulation)
	{
		return TRUE;
	}
	*/
	return FALSE;
}

BOOL	SOVillage::LimitDecreasePopulation(UI32	uiDecreasePopulation)
{
	// ���� �α����� �α��� �����Ѵ�.
	UI32 uiTempPopulation = uiPopulation - uiDecreasePopulation;

	// ������ �α��� ������ ���� �α����� ������
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

//DB���� ���� �α���, ���� ����,���� ���¿� ���� ������ ���´�.
BOOL	SOVillage::GetDBPopulation()
{	
	DOUBLE	dTempDefence;
	DBIKGetVillageInfo TempGetVillageInfo;
	if(clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEPOPULATION, 
									  LPVOID(&uiVillageCode), LPVOID(&TempGetVillageInfo)) == TRUE)
	{	
		//���� �α����� �������� �ִ� ���°� �ִ� �����Ҽ� �ִ� ���������� ���� ���´�.
		uiPopulation	   = (UI32)TempGetVillageInfo.uiPopulation;				 // ���� �α���.
//		m_siDefenceSoldier = (SI32)TempGetVillageInfo.siDefenceSoldier;			 // ���� ���� ���� 
//		m_siMaxDefenceSoldier = m_siDefenceSoldier;								 // ������ �ִ� ����.
//		m_siDefenceSoldierInDB = m_siDefenceSoldier;
		vdMaxDefence	   = (UI32)TempGetVillageInfo.uiMaxPopulation/100 * 70;  // �ִ� ������ ������Ʈ�Ѵ�.
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

		m_uiLoadQuantity   = (UI32)TempGetVillageInfo.uiMaxPopulation/100 * 30;	 // �������� ���� �Ҽ� �ִ� �ִ� �α���.
		uiMaxPopulation	   = (UI32)TempGetVillageInfo.uiMaxPopulation;           // ������ �ִ� �α���.

		return TRUE;
	}
	return FALSE;
}

// ���� �α���,�ִ� �α����� �����Ѵ�.
void	SOVillage::SetDBPopulation()
{
	UI32	TempIncreasePopulation;
	DOUBLE	dTempDefence;

//	TempIncreasePopulation = uiPopulation/100*5;
	TempIncreasePopulation = uiPopulation * 0.01;

	//�����ϴ� �α����� ������ �ִ� �α����� �Ѿ�� �ִ� �α����� ������Ų��.
	if(TempIncreasePopulation + uiPopulation > m_uiLimitMaxPopulation)
	{
		TempIncreasePopulation = m_uiLimitMaxPopulation - uiPopulation;
	}

	//�α��� 
	uiPopulation     += TempIncreasePopulation;
	
	//�ִ� �α���
	uiMaxPopulation  = uiPopulation; 
	
	//��� �Ҽ� �ִ� �ִ� �α���.
	m_uiLoadQuantity = uiMaxPopulation/100*30;
	
	//������ �ִ� ����.
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

	//���� �α����� �������� ������ �ִ� ����,�ִ� �����Ҽ� �ִ� ���������Ǽ� ���� �����Ѵ�.
	vdDefence = vdMaxDefence;
	SaveDefenceToDB();

	
	DBIKSetVillageInfo	TempSetVillageInfo;
	memset(&TempSetVillageInfo, 0, sizeof(DBIKSetVillageInfo));

	TempSetVillageInfo.uiVillageCode	=	uiVillageCode;
	TempSetVillageInfo.uiPopulation		=	uiPopulation;
	TempSetVillageInfo.uiMaxPopulation	=	uiMaxPopulation;

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASE_POPULATION, LPVOID(&TempSetVillageInfo), sizeof(DBIKSetVillageInfo));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ���� ���� �Լ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	//m_siVillageAttackType Village�� ������ �ִ� ������ ��������.
	m_siVillageAttackType	  = siAttackType;
	m_dwPrevAttackdelay		  =	clWorld->dwtimeGetTime;	 //���� ����� ����ϱ� ���� ������
	m_dwPrevHitTime			  = clWorld->dwtimeGetTime;  //����� �߻� �ð� üũ.

	//���� ������ ��� 20��
	if(m_siVillageAttackType == ON_VILLAGE_ATTACK_DEFENCE)
		m_dwSkillDelayTime  = 30 * 1000;

//		m_dwSkillDelayTime	= 20000;
	//�� ����, �⸧ ���� 10��
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

	// �޽����� ���� �÷��̾ ã�´�.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	if(siSendCharNum != 0)
	{
		// �ʵ忡�� �޽����� ���� ��ü�� ã�´�.
		for(i = 0; i < siSendCharNum; i++)
		{
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// �� ������ ���̴� ��� �÷��̾�鿡�� �޽����� �����ش�.
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

	//���� �������� �˻��Ѵ�.
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
	pPlayer->m_dwAttackDelay =  dwNowTick;//���� �ð��� �������ش�.

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
	//�߰��� ������ ��Ʈ�Ǿ��ٸ�.
	if(siAttackPower >= siRand)
	{	
		//���� �ַ��� �������� 1.5���� �������� �ش�.
		nDamage = nDamage * 1.5;
		siAttackType = ON_RET_ATTACK_POWER;
	}

//	nDamage = 10000;
	DecreaseVillageDefence( nDamage );
	AddDamageAttackedByGuild(pPlayer->uiGuildID, (SI32)nDamage);

	//���� �ð��� ���� ���ش�.
	m_bVillageAttack		 = TRUE;

	if(vdDefence <= 0)
	{
		if(!m_bIsFall)
		{
			MONEY	TempMoney = 0;
			// ������ ������ �ı��Ǿ���.
			// ������ ���ڱݸ� 50%��´�.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
		
			// ������ ������ ����� ��濡�� ������ ���ش�.		
			RewardToGuildForDestroyVillage(TempMoney);
			bIsVillageDestroyed		=	TRUE;
			m_bIsFall               =   TRUE;
		}
	}
	else
	{
		DWORD dwNowTick = GetTickCount();
		if(dwNowTick - pPlayer->m_dwMessageTime <= 30)
		{	// ������ ���� ���Ѵٴ� �޽����� �����ش�.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// ������ ������Ե� �˷��ش�.
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

	// ���ݿ� ���� ������ �����ش�.
	pPlayer->SendFieldMsg(ON_RESPONSE_ATTACK_VILLAGE, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiKind),  //�������ּӼ�
						LPVOID(this),			  //����			
						LPVOID(nDamage),          //��������
						LPVOID(siAttackType),     //����Ÿ��  
						LPVOID(IsLong));          //�ٰŸ����� ���Ÿ�����...

	return bIsVillageDestroyed;
}

/*
//������ �ٰŸ� ������ ���Ѵ�.
//������ �÷��̾�� �ݰ��Ҽ� �ִ�.
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

	// �÷��̾ ��ȿ���� �ʴٸ� �����Ѵ�.
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

	//��3�� ����
	if(siAttackPower >= siRand)
	{	
		//���� �ַ��� �������� 1.5���� �������� �ش�.
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

	// ���� ������ 0�� �Ǿ��ٸ� �Ҽӵ� ��ܿ��� �����ϵ��� �ϰ�	
	if(m_siDefenceSoldier <= 0)
	{	
		if(clWorld->m_FieldBattleParser.GetUnitKind(pPlayer->uiKind) == FIELD_BATTLE_SWORD)
		{
			MONEY	TempMoney = 0;
			// ������ ������ �ı��Ǿ���.
			// ������ ���ڱݸ� 50%��´�.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
			// ������ ������ ����� ��濡�� ������ ���ش�.		
			RewardToGuildForDestroyVillage(TempMoney);
			//������ �˻�.
//			m_Government->VillageInvestment_CheckBestInvestmentor();
			bIsVillageDestroyed		=	TRUE;
		}
	}		
	else
	{		
		// �����̾��� �ı����� �ʾҴ�.

		if(GETTIMEGAP(dwLastAOVillageBeAttacked, clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			dwLastAOVillageBeAttacked	=	clWorld->dwtimeGetTime;

			// ������ ���� ���Ѵٴ� �޽����� �����ش�.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// ������ ������Ե� �˷��ش�.
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

	//���񺴼�  = ������ ü��/100 
	SI16 siDefenceSoldier = (SI16)(GetDefenceSoldier()/100);
	//������ �÷��̾ ������ �������� �����ش�.
	//1.���񺴼� 2.PlayerID 3.VillageAttackType
	SI16 siPlayerDamage= clWorld->m_FieldBattleParser.GetCalcDamageByDefenceSoldier(siDefenceSoldier,
		clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),m_siVillageAttackType);

	pPlayer->DecreaseFieldHp(siPlayerDamage);


	// ���ݿ� ���� ������ �����ش�.
	pPlayer->SendFieldMsg(ON_RESPONSE_ATTACK_VILLAGE, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiKind),
						LPVOID(this),						
						LPVOID(siCastleDamage),
						LPVOID(siDefenceDamage),
						LPVOID(siPlayerDamage),
						LPVOID(siAttackPowerType));

	return	bIsVillageDestroyed;
}

//������ ���Ÿ� ������ ���Ѵ�.
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


	// �÷��̾ ��ȿ���� �ʴٸ� �����Ѵ�.
	if(pPlayer == NULL)		return FALSE;
	
	SI16 siAttackPower  = clWorld->m_AttackPowerParser.GetAttackPercent(pPlayer->GetFightingPower());

	SI16 siRand = rand()%100 + 1;

	if((pPlayer->siClassInGuild == clWorld->clIGuildClass.GetHNDCode()) || (pPlayer->siVMercenarySlot > 0))
	{
		siAttackPower  = 5;
	}


	//��3�� ����
	if(siAttackPower >= siRand)
	{	
		//���� �ַ��� �������� 1.5���� �������� �ش�.
		float fTempDamage = (float)siCastleDamage * 1.5;
		siCastleDamage    = (SI16)fTempDamage;

		fTempDamage		   = (float)siDefenceDamage * 1.5;
		siDefenceDamage    = (SI16)fTempDamage;

		siAttackPowerType = ON_RET_ATTACK_POWER;
	}
	

	//���� ������ �ٿ��ش�.
	if((siCastleDamage > 0) && (vdDefence > 0))
		DecreaseVillageDefence( siCastleDamage );

	if(siDefenceDamage > 0 && (GetDefenceSoldier() > 0))
		DecreaseDefenceSoldier((SI16)siDefenceDamage );

	AddDamageAttackedByGuild(pPlayer->uiGuildID,(SI32)siDefenceDamage);

	//���� ���� ���� 0���� �۰ų� ������.
	if(m_siDefenceSoldier <= 0)
	{	
		if(clWorld->m_FieldBattleParser.GetUnitKind(pPlayer->uiKind) == FIELD_BATTLE_SWORD)
		{
			MONEY	TempMoney = 0;
			// ������ ������ �ı��Ǿ���.
			// ������ ���ڱݸ� 50%��´�.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
			// ������ ������ ��ܿ��� ������ ���ش�.
			RewardToGuildForDestroyVillage(TempMoney);
			//m_Government->VillageInvestment_CheckBestInvestmentor();
			bIsVillageDestroyed		=	TRUE;	
		}
	}		
	else
	{		
		// �����̾��� �ı����� �ʾҴ�.

		if(GETTIMEGAP(dwLastAOVillageBeAttacked, clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			dwLastAOVillageBeAttacked	=	clWorld->dwtimeGetTime;

			// ������ ���� ���Ѵٴ� �޽����� �����ش�.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// ������ ������Ե� �˷��ش�.
			if( ( pszHighestInvestmentorPlayerName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )				
			{
				if( ( pHighestInvestmentorPlayer = clWorld->GetPlayer( pszHighestInvestmentorPlayerName ) ) != NULL )
				{
					pHighestInvestmentorPlayer->SendFieldMsg( ON_MYVILLAGE_ATTACKED, SO_SFM_ONLYME, LPVOID( uiVillageCode ) );
				}				
			}
		}
	}

	//���񺴼�  = ������ ü��/100 
	SI16 siDefenceSoldier = (SI16)(GetDefenceSoldier()/100);
	//������ �÷��̾ ������ �������� �����ش�.
	//1.���񺴼� 2.PlayerID 3.VillageAttackType
	SI16 siPlayerDamage= clWorld->m_FieldBattleParser.GetCalcDamageByDefenceSoldier(siDefenceSoldier,
		clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),m_siVillageAttackType);

	pPlayer->DecreaseFieldHp(siPlayerDamage);

	// ���ݿ� ���� ������ �����ش�.
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

	// ���� Defence�� Max�̸� �׳� �����Ѵ�.
	if(vdDefence == vdMaxDefence)	return;

	IncreaseVillageDefence(vddefence);

	// �޽����� ���� �÷��̾ ã�´�.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	// ������ ������ �����Ѵ�.	
	if( bSendToDB == TRUE )
	{
		SaveDefenceToDB();
	}

	if(siSendCharNum != 0)
	{
		// �ʵ忡�� �޽����� ���� ��ü�� ã�´�.
		for(i = 0; i < siSendCharNum; i++)
		{		
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// �� ������ ���̴� ��� �÷��̾�鿡�� �޽����� �����ش�.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_DEFENCE, SO_SFM_CUSTOM, LPVOID(uiVillageCode), LPVOID(&vdDefence), LPVOID(siSendCharNum), LPVOID(uiCharUniIDs));
				break;
			}	
		}	
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ı��� ������ uiIncreasePercent pecent ��ŭ  �Ǹ� ä���ش�.
//	���� ����	:	2004-04-06 - �̹μ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		vdDefence	=	0x7fffffffffffffff;    // 10������ �ϸ� ��922��(922 3372 0368 5477 5807)

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ������ ���� ������ �����ش�.
//	���� ����	:	2004-04-06 - �̹μ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::AutoRestorationVillageDefence(BOOL bSendToDB)
{
	if(AutoRestorationDefence(50) == FALSE)
		return;

	UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	SI32		siSendCharNum;
	SI32		i;
	SOPlayer	*pPlayer;

	// �޽����� ���� �÷��̾ ã�´�.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	// ������ ������ �����Ѵ�.	
	if( bSendToDB == TRUE )
	{
		SaveDefenceToDB();
	}

	if(siSendCharNum != 0)
	{
		// �ʵ忡�� �޽����� ���� ��ü�� ã�´�.
		for(i = 0; i < siSendCharNum; i++)
		{
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// �� ������ ���̴� ��� �÷��̾�鿡�� �޽����� �����ش�.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_DEFENCE, SO_SFM_ONLYME, LPVOID(uiVillageCode), LPVOID(&vdDefence), LPVOID(siSendCharNum), LPVOID(uiCharUniIDs));
				break;
			}	
		}	
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ DB�� �����Ѵ�.
//	���� ����	:	2002-03-29 ���� 3:02:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::SaveDefenceToDB()
{
	DBIKVillageDurability	VillageDurability;

	// �������� �� �ִ� ��Ȳ���� üũ�Ѵ�.
	if(vdDefence == vdDefenceInDB)	return;		// �ٲ��� �ʾҴٸ� ������ �ʿ䰡 ����.

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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���� ��Ų��.
//	���� ����	:	2002-03-30 ���� 2:30:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::IncreaseVillageDefence(VDEF IncreaseDefence)
{
	if((0x7fffffffffffffff - vdDefence) >=  IncreaseDefence)
	{
		vdDefence	=	min(vdDefence + IncreaseDefence, vdMaxDefence);		
	}		
	else
		vdDefence	=	0x7fffffffffffffff;    // 10������ �ϸ� ��922��(922 3372 0368 5477 5807)
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ����.
//	���� ����	:	2002-03-30 ���� 2:30:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���´�.
//	���� ����	:	2002-04-01 ���� 3:58:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VDEF	SOVillage::GetVillageDefence()
{
	return	vdDefence;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������

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
	//����� �̸����� �����ϰ� �ִ��� �˻� �Ѵ�.
	return clWorld->clIGuild.CheckVillage(uiVillageCode);
}
