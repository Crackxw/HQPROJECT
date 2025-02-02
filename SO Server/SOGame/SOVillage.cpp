#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "AuctionList.h"
#include "AuctionMarket.h"

#include "CSiegeUniteTable.h"

// 薑瞳滲熱菟.
clVillageAttackInfo		SOVillage::clVAInfo;	// 葆擊曖 奢問 薑爾 
SOWorld					*SOVillage::clWorld;	




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-08-14 螃�� 5:44:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage::SOVillage()
{	
	ZeroMemory(szHNSID, sizeof(szHNSID));							// ч熱 嬴檜蛤 蟾晦�� 
	ZeroMemory(VillageCapital, sizeof(VillageCapital));				// 10喇翕寰曖 葆擊曖 熱櫛 蟾晦�� 
	
//	pstBankData						=	NULL;						// 瞪濰曖 等檜顫.
	pstINNData						=	NULL;						// 輿虞曖 等檜顫.	
	

	siVillageInOutPlayerYetYear		=	0;							// 濛喇曖 葆擊 轎殮濠 熱 
	siVillageInOutPlayerNowYear		=	0;							// 螢п曖 葆擊 轎殮濠 熱 

	uiGuildID						=	0;							// 葆擊擊 模嶸ж堅 氈朝 望萄曖 嬴檜蛤 
	siGuildFlag						=	0;							// 望菟曖 梓嫦 嬴檜蛤 

	CurrentCapital					=	VillageCapital;				// 螢п曖 葆擊 熱櫛 
	dwLastAOVillageBeAttacked		=	0;							// 葆擊檜 奢問渡ц擊陽 陛濰 譆斬縑 憮幗縑 憲萼 衛除 


	m_pMarket						=	NULL;
	m_pIndustrialComplexs			=	NULL;
	m_Barrack						=	NULL;
	m_Hospital						=	NULL;
	m_Wharf							=	NULL;
	m_Government					=	NULL;	
	m_pBank							=	NULL;
	m_bIsFall                       =   FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 模資濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊擊 蟾晦�� и棻.(DB煎 睡攪 ⑷ 鼻�盛轀� 嫡嬴螞棻.)
//	熱薑 橾濠	:	2002-08-14 螃�� 5:46:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::Init(VillageHeader *pVillageHeader)
{	
	DBIKVillageEnterPlauerGet		TempDBIKVillageEnterPlauerGet;
	DBIKVillageEnterPlauerGetInfo	TempDBIKVillageEnterPlauerGetInfo;

	
	// 葆擊曖 晦獄 薑爾蒂 撲薑и棻.
	
	// 葆擊曖 X, Y 謝ル蒂 晦遽戲煎 嬴蛤蛤蒂 撲薑и棻 
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
	// 檜 葆擊 氈朝 勒僭菟擊 盪濰и棻.
	memcpy(siStructureKinds, pVillageHeader->siStructureKinds, sizeof(siStructureKinds));

	// DB縑憮 檜 葆擊擊 模嶸ж堅 氈朝 鼻欽擊 橢橫螞棻.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEGUILDID_GET, LPVOID( uiVillageCode ), LPVOID( &uiGuildID ) ) == FALSE )		return FALSE;		
	
	if(uiGuildID != 0)
	{
		// 梓嫦曖 嬴檜蛤蒂 橢橫螞棻 
		siGuildFlag	= clWorld->clIGuild.GetFlag(uiGuildID);

		// 檜 葆擊擊 п渡 鼻欽縑 模樓 衛鑑棻.
		clWorld->clIGuild.AddVillage(uiGuildID, this, TRUE);
	}
	else
		siGuildFlag	=	0;

	// DB縑憮 ч熱曖 檜葷擊 橢橫螞棻.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEHNSNAME_GET, LPVOID( uiVillageCode ), LPVOID( szHNSID ) ) == FALSE )			return FALSE;
	
	// DB縑憮 葆擊曖 寞橫溘擊 橢橫螞棻.
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_VILLAGEDEFENCE_GET, LPVOID( uiVillageCode ), LPVOID( &vdDefenceInDB ) ) == FALSE )	return FALSE;		

	// 寞橫溘擊 盪濰и棻.
	vdDefence	=	vdDefenceInDB; 
	
	// 斜 葆擊曖 檣掘蒂 DB煎 睡攪 橢橫螞棻.
	// 譆渠 葆擊 檣掘熱 葆擊 寞橫溘 葆擊檣掘熱 葆擊 熱綠煽熱蒂 盪濰и棻.
	if(GetDBPopulation() == FALSE) return FALSE;
	
	// 檜 葆擊縑 氈朝 賅萇 勒僭菟擊 蟾晦�� и棻.
	if( InitStructure( pVillageHeader ) == FALSE )		return	FALSE;	

	// 葆擊 寰頂僥婁 措陛 囀萄菟 橢橫螞棻
	memset(szDescription,0, sizeof(szDescription));		
	GetDescription();									
	GetNationCode();					

	// DB縑憮 葆擊曖 10 喇除 檜櫛擊 橢橫螞棻.
	GetVillageCapital();

	// DB 縑憮 葆擊 翱除 轎殮濠 熱蒂 橢橫褥
	siVillageInOutPlayerYetYear = 0;
	siVillageInOutPlayerNowYear = 0;

	m_siVillageAttackType	  = ON_VILLAGE_ATTACK_NONE;
	m_dwPrevAttackdelay		  =	0;	 //棻擠 晦獎擊 餌辨ж晦 梱雖 裁溯檜
	m_dwPrevHitTime			  = 0;  //晦獎曖 嫦儅 衛除 羹觼.
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

	// 棻陛螢 濰陳擊 撲薑и棻. (憮幗蒂 黨�� 譆蟾曖 濰陳檜 腎啊雖..)
	InitMarketDay();

	return TRUE;
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	擎ч薑爾蒂 DB縑憮 機等檜おи棻.(衛翕衛 & 衙橾)
//	熱薑 橾濠	:	2002-08-14 螃�� 5:55:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	撲貲	:	Ы溯檜橫陛 葆擊縑憮 煎斜 嬴醒 ц擊 唳辦 ��轎п 遽棻.
//----------------------------------------------------------------------------------------------------------------
VOID	SOVillage::PlayerLogOut(SOPlayer *pPlayer)
{
	// 勒僭縑憮紫 緒螳釭螞棻.
	LeaveStructure( pPlayer );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Polling
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 濰陳檜 腎賊 葆擊曖 嬴檜蠱 熱榆擊 滲�亳藷偽� 棻擠 濰陳擊 撲薑и棻. (葆擊葆棻 濰陳檜 棻葷)
		//if((clWorld->CurrentTime.Day == 1) || (clWorld->CurrentTime.Day == 8) || (clWorld->CurrentTime.Day == 15) || (clWorld->CurrentTime.Day == 22))
		if( GetNextMarketDay() == clWorld->CurrentTime.Day )
		{
			m_pMarket->ProcessProduceReduceItem( uiPopulation );
			SetNextMarketDay();															// 棻擠 濰陳擊 撲薑и棻.
		}

		// 虜擒縑 錯檜 夥票棻賊 嬴檜蠱曖 か撩縑 評塭 儅骯 塽 模綠и棻.
		if(clWorld->bMonthChanged == TRUE)
		{
			//VillageInvestment_Updata(1);
			if(clWorld->bYearChanged == TRUE)
			{
				for(i = ON_STATISTICS_YEAR; i > 1; i--)
					memmove(&VillageCapital[i-1], &VillageCapital[i-2], sizeof(stCapital));

//				memcpy(&VillageCapital[0], CurrentCapital, sizeof(stCapital));

				// 螢п曖 檜櫛旎 蟾晦��.
				memset(CurrentCapital, 0, sizeof(stCapital));
				CurrentCapital->Year	=	clWorld->CurrentTime.Year;
			}
		}

		// ж瑞陛 雖陬棻.

		// 儅骯 衛撲僭檜 氈棻賊 橾輿橾翕寰 陛翕 寰и 衛撲僭擎 橈擁堅 
		// 檣掘熱 / 500 虜躑 喻翕擊 п遽棻 譆堅 1000.
		if( DoesFactoryExist() == TRUE )
		{
			UI32 uiNPCLabor;
			uiNPCLabor = min(1000,uiPopulation / 500);
			m_pIndustrialComplexs->DayChanged( clWorld ,uiNPCLabor);
		}
		

		// 葆擊縑 擎ч檜 氈棻賊 pstBankData 機等檜お л熱...
//		UpdateBankInfo();

		// ж瑞葆棻 ⑷營 葆擊擊 棻喪除 餌塋菟 盪濰
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
					// ⑷營 葆擊縑 譆堅 癱濠濠 匐餌
					m_Government->VillageInvestment_CheckBestInvestmentor();

					//=====================================================================================
					// 望萄 籀葬
					UI32		TempMaxInvestmentorGuild;

					TempMaxInvestmentorGuild = m_Government->GetInvestmentMaxInvestmentorGuild();				

					if(uiGuildID != 0)
					{
						//=====================================================================================
						// ⑷營 葆擊縑 望萄陛 氈擊唳辦
						// 譆堅 癱濠濠 匐餌
						if(TempMaxInvestmentorGuild != uiGuildID)
						{
							// 譆堅 癱濠濠陛 葆擊曖 望萄錳檜 嬴棍陽, 斜 葆擊擊 望萄縑憮 п寞衛鑑棻.
							clWorld->clIGuild.DelVillage(uiGuildID, this, FALSE);
							uiGuildID = 0;
						}					
					}
           
					if(uiGuildID == 0)
					{
						//=====================================================================================
						// ⑷營 葆擊縑 望萄陛 橈擊唳辦
						// 譆堅 癱濠濠陛 氈擊唳辦, 譆堅 癱濠濠陛 望萄縑 陛殮腋橫 氈戲賊, 葆擊擊 斜 望萄曖 模樓戲煎 夥脯啪 л
						if(TempMaxInvestmentorGuild != 0)
						{
							clWorld->clIGuild.AddVillage(TempMaxInvestmentorGuild, this, FALSE);
						}
					}
				}
			}
		}

		// 睡舒 渠晦褒 籀葬(轎о衛除檜 腋賊 斜 ル蒂 陛霞 餌塋菟擊 埤塭憮 轎о衛霽)
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
							// 轎о衛除檣雖 匐餌
							TicketHeader*		lpTicketHeader;

							lpTicketHeader = clWorld->clTicketParser.GetTicket(clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind);
							if(lpTicketHeader)
							{
								if((clWorld->CurrentTime.Hour % lpTicketHeader->siStartTime) == 0)
								{
									// ル蒂 餉薯
									DBIKResetTicket			TempDBIKResetTicket;

									TempDBIKResetTicket.DBAccount    = clWorld->clPlayer[TempCount]->stAccountInfo.siDBAccount;
									TempDBIKResetTicket.uiSlot       = clWorld->clPlayer[TempCount]->stAccountInfo.uiSlot;
									clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_RESETTICKET, LPVOID(&TempDBIKResetTicket), sizeof(TempDBIKResetTicket));
									clWorld->clPlayer[TempCount]->uiStructerInPos = STRUCTERPOS_NONE;

									// 轎о Message
									//clWorld->clPlayer[TempCount]->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

									// 檜翕ж溥朝 寡 List縑 蹺陛
									TempUserTicketInfo[TempUserCount] = clWorld->clPlayer[TempCount]->stTicketInfo.siTicketKind;
									TempUserInfo[TempUserCount]       = clWorld->clPlayer[TempCount]->uiAccount;
									TempUserCount++;

									// 餌塋菟擊 葆擊 夤戲煎 檜翕
									clWorld->clPlayer[TempCount]->LeaveVillage();
								}
							}
						}
					}
				}
			}
		}

		// 檜翕ж朝 餌塋菟檜 氈戲賊, 寡 List縑 蹺陛
		if(TempUserCount)
		{
			// 辦摹 寡 Kind 煎 薑溺
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

			// 薑溺脹 Kind蒂 匐餌
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

			// 寡 檜翕
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

		// 葆擊曖 寞橫溘擊 DB縑 盪濰и棻. (奢撩 衛除橾唳辦縑虜)
		if( clWorld->IsAttackVillageTimeZone() == TRUE )
		{			
			SaveDefenceToDB();
//			SaveSoldierToDB();
		}
	}

	//橾薑 衛除檜 腎賊 Village Attack Type擊 錳楚渠煎 熱薑и棻.
	if(GetVillageAttackType() != ON_VILLAGE_ATTACK_NONE)
	{
		if(GetVillageAttackType() == ON_VILLAGE_ATTACK_OIL)
		{
			if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(10000))
			{
				 //葆擊曖 AttackType擊 錳楚渠煎 給萼棻.
				 InitializeVillageAttackType();
				 //葆擊 輿嬪縑 Ы溯檜橫菟縑啪 AttackType檜 錳楚渠煎 給嬴諮棻朝匙擊 憲萼棻.
				 SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			}
		}
		else if(GetVillageAttackType() == ON_VILLAGE_ATTACK_DEFENCE)
		{
			//if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(15000))
			//{
				 //葆擊曖 AttackType擊 錳楚渠煎 給萼棻.
				 InitializeVillageAttackType();
				 //葆擊 輿嬪縑 Ы溯檜橫菟縑啪 AttackType檜 錳楚渠煎 給嬴諮棻朝匙擊 憲萼棻.
				 //SendVillageToPlayerAtAttackType(ON_VILLAGE_ATTACK_NONE);
			//}
		}
		else if(GetVillageAttackType() == ON_VILLAGE_ATTACK_STONE)
		{
			if(GETTIMEGAP(m_dwPrevHitTime, clWorld->dwtimeGetTime) >= DWORD(15000))
			{
				 //葆擊曖 AttackType擊 錳楚渠煎 給萼棻.
				 InitializeVillageAttackType();
				 //葆擊 輿嬪縑 Ы溯檜橫菟縑啪 AttackType檜 錳楚渠煎 給嬴諮棻朝匙擊 憲萼棻.
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
	//奢撩擊 ж堅 氈朝 葆擊檜塭賊.
	if(m_bVillageAttack == TRUE)
	{
		if(GETTIMEGAP(m_dwVillageDamageUpdateTime,clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			m_dwVillageDamageUpdateTime = clWorld->dwtimeGetTime;
			
			if(uiGuildID > 0)//葆擊擊 模嶸ж堅 氈朝 望萄陛 氈朝雖 匐餌
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

							writeInfoToFile("GuildDamage.txt", "葆擊 模樓 望萄: %d, VillageID:%d, AttackGuild:%d, Damage:%d", 
								             uiGuildID, uiVillageCode, m_stAttackGuild[i].uiGuildID, m_stAttackGuild[i].siDamage);	
							
							clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_VILLAGE_DAMAGE, &TempDBIKVillageDamage, sizeof(DBIKVillageDamage));
						}
					}
				}
			}
			
		}
	}

	if(clWorld->bUpdateTime)		// 億漁 11衛縑 TRUE 鼻鷓陛 脹棻.
	{
		//檣掘熱 啗骯,葆擊 寞橫溘 啗骯蛔擊 憮幗縑憮 и棻.
		printf("檣掘熱, 寞橫溘 機等檜お 衛濛");
		SetDBPopulation();
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊縑 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::EnterVillage(SOPlayer *pPlayer)
{
	siVillageInOutPlayerNowYear++;

	return TRUE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊檜 氈朝 裘曖 id蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	SOVillage::GetMapID()
{
	return uiMapID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB縑憮 葆擊 寰頂僥擊 陛螳螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::GetDescription()
{
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEDESCRIPT, LPVOID(uiVillageCode),LPVOID(&szDescription)) == FALSE)	
	{
		memset(szDescription,0, sizeof(szDescription));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB縑憮 葆擊曖 措陛 囀萄蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::SetNationCode(CHAR NationCode)
{
	DBIKVillageNation		VillageNation;
	VillageNation.VillageCode	=	uiVillageCode;
	VillageNation.Code			=	NationCode;
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGE_SETNATION, &VillageNation, sizeof(DBIKVillageNation));
	
	cNation	=	NationCode;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB縑憮 葆擊曖 措陛 囀萄蒂 陛螳螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::GetNationCode()
{
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGE_GETNATION, LPVOID(uiVillageCode),LPVOID(&cNation));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 檜葷擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOVillage::GetVillageName()
{
	return clWorld->clOnlineText.Get(uiVillageName);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 勒僭檜 檜 葆擊 寰縑 氈朝陛?
//	熱薑 橾濠	:	2002-10-04 螃�� 6:59:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 轎殮濠 熱蒂 啗骯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage::GetVillageInOutPlayer(void)
{
	if(siVillageInOutPlayerYetYear > siVillageInOutPlayerNowYear) return siVillageInOutPlayerYetYear;

	return siVillageInOutPlayerNowYear;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	1喇檜 雖陬擊陽 葆擊曖 轎殮濠熱蒂 偵褐и棻 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void	SOVillage::PrepareVillageInOutPlayer(void)
{
	siVillageInOutPlayerYetYear = siVillageInOutPlayerNowYear;
	siVillageInOutPlayerNowYear = 0;

	// DB 縑 盪濰
	DBIKVillageEnterPlauerSet		TempDBIKVillageEnterPlauerSet;

	TempDBIKVillageEnterPlauerSet.uiVillageCode		=	uiVillageCode;
	TempDBIKVillageEnterPlauerSet.uiPlayerNum		=	siVillageInOutPlayerYetYear;
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEENTERPLAYERSET, LPVOID(&TempDBIKVillageEnterPlauerSet), sizeof(TempDBIKVillageEnterPlauerSet));
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑 氈朝 望萄曖 璋濠 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-27 螃瞪 11:09:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	SOVillage::GetGuildNID()
{
	return uiGuildID;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑 氈朝 望萄曖 僥濠 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-27 螃瞪 11:09:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收	
CHAR*	SOVillage::GetGuildSID()
{
	return 	clWorld->clIGuild.GetGuildName(uiGuildID);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊檜 模樓腎朝 鼻欽擊 撲薑и棻.
//	熱薑 橾濠	:	2002-05-10 螃�� 9:32:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::SetOwnerGuild(UI32 uiguild, BOOL bInit)
{
	DBIKGuildOwnerVillageInfo	GuildOwnerVillageInfo;

	// 檜 葆擊曖 鼻欽曖 ID蒂 盪濰и棻.
	uiGuildID		=		uiguild;

	// 蟾晦�� 醞檜賊 斜傖 葬欐и棻.
	if(bInit == TRUE)	return;	

	GuildOwnerVillageInfo.uiVillageCode	=	uiVillageCode;
	GuildOwnerVillageInfo.uiGuildID		=	uiGuildID;
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEGUILDID_SET, &GuildOwnerVillageInfo, sizeof(DBIKGuildOwnerVillageInfo));
	
	// 檜 葆擊曖 譆堅 癱濠濠蒂 ч熱煎 雖薑п遽棻.
	SetHNSID(m_Government->GetInvestmentMaxInvestmentorName());	
	// 檜 葆擊曖 梓嫦擎 模樓 鼻欽曖 梓嫦擊 陛雖啪 脹棻.
	ChangeGuildFlag(clWorld->clIGuild.GetFlag(uiguild));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	措陛 囀萄蒂 遽棻.(鼻欽檜 橈戲賊, ⑷營 措陛鼻鷓煎 氈戲賊, 鼻欽 鼻鷓煎)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑 億煎遴 'ч熱'蒂 雖薑и棻.
//	熱薑 橾濠	:	2002-03-20 螃瞪 11:35:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::SetHNSID(CHAR *pszHNSID)
{
	SI32				siHNSClassCode	=	clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS);	

	DBIKVillageMaster	VillageMaster;

	// ⑷營 鼻欽檜 橈戲賊 葬欐и棻.
	if(uiGuildID == 0)					return	FALSE;
	if(pszHNSID == NULL)				return	FALSE;
	if(strcmp(szHNSID, pszHNSID) == 0)	return	FALSE;

	// ⑷營曖 'ч熱'蒂 п堅и棻.
	DismissalHNS();

	// 億煎遴 'ч熱'曖 檜葷擊 DB縑 盪濰и棻.
	ZeroMemory(&VillageMaster, sizeof(DBIKVillageMaster));
	VillageMaster.VillageCode	=	uiVillageCode;
	strcpy( VillageMaster.Name, pszHNSID );
	
	// 望萄儅撩檜 嬴棍陽	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEHNSNAME_SET, &VillageMaster, sizeof(DBIKVillageMaster));
	
	
	// 'ч熱'曖 檜葷擊 葆擊縑 盪濰и棻.
	ZeroMemory(szHNSID, sizeof(szHNSID));
	strncpy(szHNSID, pszHNSID, ON_ID_LENGTH);
	

	cltGuild *pGuild = clWorld->clIGuild.GetGuildByID(uiGuildID);

	strGuildCommonMemberInfo MemberInfo;

	// 斜 議葛攪曖 譆渠 霜晝擊 掘и棻.
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
	
	// 虜擒 霜晝 囀萄陛 ч熱爾棻 觼剪釭 偽戲賊 滲唳й в蹂陛 橈棻.
	if(clWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siHNSClassCode) == SO_GUILD_CLASSCOMP_LOW)
	{
	
		// ч熱煎 蝓晝衛麵遽棻.

		// DB縑 盪濰擊 ж堅
		// 蕾樓擊 ж堅 氈棻賊 霜晝檜 滲唳腎歷棻朝 詭衛雖蒂 爾頂遽棻.	
		clWorld->clIGuild.ChangeClass( uiGuildID, pszHNSID, siHNSClassCode );	
	}

	return	TRUE;
		
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑憮 檜 議葛攪曖 譆堅 霜晝擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-25 螃瞪 10:13:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage::GetHighestGuildClass(CHAR *pszID)
{
	// 嬴霜擎 ч熱 夤縑 橈棻.
	if(strcmp(szHNSID, pszID) == 0)
		return clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS);
	else
	{
		// 釭該雖朝 餌�素抴�.
		return clWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN);
	}

	return	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊曖 譆堅 癱濠濠檜賊憮 鼻欽縑 陛殮腎橫 氈朝雖蒂 
//					羹觼и棻.
//	熱薑 橾濠	:	2004-02-04 螃瞪 - 檜團熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL   SOVillage::IsVestInvestmentorAndGuildMember(const CHAR* pszName)
{
	m_Government->VillageInvestment_CheckBestInvestmentor();	
	UI32		TempMaxInvestmentorGuild;
	TempMaxInvestmentorGuild = m_Government->GetInvestmentMaxInvestmentorGuild();

	return ((TempMaxInvestmentorGuild != 0 ) &&
		(strcmp(pszName, m_Government->GetInvestmentMaxInvestmentorName()) == 0)) ? TRUE : FALSE;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊縑憮 檜 議葛攪陛 陛雖堅 氈朝 霜晝擊 蟾晦�� и棻.
//					п渡 霜晝擊 餉薯ж堅, 萌籀葬蒂 п遽棻.
//					虜擒 檜 葆擊檜 鼻欽縑憮 餉薯腎橫撿 и棻賊 TRUE蒂 葬欐и棻.
//					斜溢雖 彊戲賊 FALSE蒂 葬欐и棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 7:53:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DeleteGuildClass(CHAR *pszName)
{
	///////////////////////////////////////////////////////////////////////
	//BOOL bVal = IsVestInvestmentorAndGuildMember(pszName);	
	///////////////////////////////////////////////////////////////////////
	
	BOOL bRetVal = FALSE;

	// 檜葷檜 橈棻賊 斜傖 葬欐и棻.
	if(strcmp(pszName, "") == 0) bRetVal = FALSE;

	else if(strcmp(szHNSID, pszName) == 0)
	{		
		// ч熱檣 pszName曖 霜晝擊 橈撻棻朝匙擎 
		// 闊 檜 葆擊擊 鼻欽縑憮 薯剪и棻朝匙檜棻.
		// 翕衛縑 pszName曖 議葛攪朝 ч熱曖 掏и檜 橈橫霞棻.

		// ч熱蒂 п堅и棻.			
		DismissalHNS();

		bRetVal = TRUE;
	}

	// 葆擊曖 ч熱 檜葷檜 澀跤腎橫 氈朝唳辦.
	/*if(bVal == TRUE && bRetVal == FALSE)
	{
		writeInfoToFile( "DeleteGuildError.txt", 
			"Invalid Hangsu name. Hangsu name is %s, MaxInvestmentor name is %s", 
			szHNSID, pszName);
	}*/

	return	bRetVal;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 葆擊曖 望萄陛 餉薯腎歷棻.
//	熱薑 橾濠	:	2002-03-25 螃瞪 11:04:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::DeleteGuild()
{	
	DBIKGuildOwnerVillageInfo	GuildOwnerVillageInfo;

	GuildOwnerVillageInfo.uiVillageCode	=	uiVillageCode;
	GuildOwnerVillageInfo.uiGuildID		=	uiGuildID;

//	m_siDefenceSoldier					=	0;
//	m_siMaxDefenceSoldier				=	0;	
	
	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VILLAGEOWNER_DELETE, &GuildOwnerVillageInfo, sizeof(DBIKGuildOwnerVillageInfo));

	// ч熱蒂 餉薯и棻.
	DeleteGuildClass(szHNSID);

	
	// 鼻欽婁 鼻欽曖 梓嫦擊 蟾晦�� и棻.
	uiGuildID		=	0;
	siGuildFlag		=	0;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營曖 ч熱蒂 п堅и棻.	
//	熱薑 橾濠	:	2002-04-30 螃�� 9:36:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::DismissalHNS()
{
	SI32		siHighestClassCode;	

	SI32		siHNSClassCode	=	clWorld->clIGuildClass.GetHNSCode();					// 'ч熱'曖 囀萄蒂 嘐葬 陛雖堅 氈朝棻.
	SI32		siDHSClassCode	=	clWorld->clIGuildClass.GetDHSCode();					// '渠ч熱'曖 囀萄蒂 嘐葬 陛雖堅 氈朝棻.
	CHAR		szPrevHNSID[ON_ID_LENGTH + 1];

	// ⑷營 鼻欽檜 橈剪釭 ч熱陛 橈戲賊 葬欐и棻.
	if(uiGuildID == 0)					return;		
	if(szHNSID[0] == NULL)				return;		

	// 檜 葆擊曖 ч熱蒂 嘐葬 晦橘п菩棻.
	ZeroMemory(szPrevHNSID, sizeof(szPrevHNSID));
	strncpy(szPrevHNSID, szHNSID, ON_ID_LENGTH);	 

	// 檜 葆擊曖 ч熱蒂 橈撻棻.(檜 睡碟擎 奩萄衛 嬴楚 睡碟 爾棻 嬪縑 氈橫撿и棻. 嬴楚曖 GetHighestGuildClass л熱縑憮 檜 滲熱蒂 霤褻 ж晦 陽僥檜棻.) 	
	ZeroMemory(szHNSID, sizeof(szHNSID));

	// ч熱艘湍 餌塋曖 譆渠 霜晝擊 掘и棻.
	cltGuild *pGuild = clWorld->clIGuild.GetGuildByID(uiGuildID);
	if (pGuild != NULL)
	{

		strGuildCommonMemberInfo MemberInfo;
		if (pGuild->m_GuildMemberManager.GetGuildMemberByCharName(szPrevHNSID,&MemberInfo) == FALSE) return;			
				
		// 嬴霜 雖錳濠艘擊 唳辦 霜晝 滲翕檜橈棻.
		if (MemberInfo.siClassInGuild == 90) return;

		// 葆擊曖 譆癱艘湍 餌塋檜 ч熱爾棻 堪擎 霜疇檜 嬴棲歷擊 唳辦縑虜... 渠寞檜釭 渠ч熱朝 葆擊擊 檠橫紫 霜疇擊 嶸雖ж嘎煎..
		if( clWorld->clIGuildClass.ClassComp(MemberInfo.siClassInGuild, siHNSClassCode) != SO_GUILD_CLASSCOMP_HIGH )	// 罹鷓梱雖 'ч熱' 艘戲棲 譆模и 'ч熱'陛 釭螢 匙檜棻.
		{			
			// ⑷營 啪歜 鼻縑憮 檜 議葛攪曖 譆渠 霜晝擊 橢橫螞棻.
			siHighestClassCode	=	clWorld->GetHighestGuildClass(szPrevHNSID);		// 檜 議葛攪陛 ч熱檣 葆擊檜 氈朝雖 賅萇 葆擊 匐餌
			// ⑷營 霜晝檜 盪濰脹 霜晝婁 棻腦棻賊 夥紱橫遽棻.
			if(siHighestClassCode != MemberInfo.siClassInGuild) clWorld->clIGuild.ChangeClass(uiGuildID, szPrevHNSID, siHighestClassCode);			
		}
	}
	else
	{
		return;
	}				
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 薑爾蒂 橢橫除棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 1:12:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage::GetVillageInfo(SOPlayer *pPlayer, OnRecvVillageInfo *pVillageInfo)
{
	CHAR	*pszInvestmentorName;

	if(pVillageInfo)
	{
		ZeroMemory(pVillageInfo, sizeof(OnRecvVillageInfo));

		pVillageInfo->uiPopulation	=	uiPopulation;

		// 譆堅 癱濠濠曖 薑爾蒂 橢橫螞棻.
		if((pszInvestmentorName = m_Government->GetInvestmentMaxInvestmentorName()) != NULL)
			strcpy(pVillageInfo->m_MaxInvestmentID, pszInvestmentorName);
		
		// 葆擊曖 寞橫溘擊 橢橫螞棻.
		pVillageInfo->vdVillageDefence		=	vdDefence;

		// 葆擊曖 譆渠 寞橫溘擊 橢橫螞棻.
		pVillageInfo->vdVillageMaxDefence	=	vdMaxDefence;
//		pVillageInfo->siDefenceSoldier		=	m_siDefenceSoldier;
//		pVillageInfo->siMaxDefenceSoldier	=	m_siMaxDefenceSoldier;
		pVillageInfo->siAttackType			=	m_siVillageAttackType;
	
		// 鼻欽曖 薑爾蒂 橢橫螞棻.
		if(uiGuildID != 0)
		{
			// 鼻欽檜 氈棻賊 鼻欽曖 薑爾蒂 橢橫螞棻.
			strncpy(pVillageInfo->szGuildID, GetGuildSID(), ON_GUILDNAME_LENGTH);
		}

		// Ы溯檜橫諦 鼻欽除曖 婦啗蒂 橢橫螞棻.
		pVillageInfo->dwGuildRelation	=	ON_RET_GLD_NEUTRAL;

		// 望萄 詹幗檣雖 憲嬴螞棻.
		if((pPlayer->uiGuildID != 0) && (uiGuildID != 0) && (pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode()))
		{
			if(pPlayer->uiGuildID == uiGuildID)							
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_MINE;	// 檜 葆擊曖 鼻欽錳檜棻.
			else if(clWorld->clIGuild.IsHostilityRelation(uiGuildID, pPlayer->uiGuildID) == TRUE)
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_ENEMY;	// 瞳渠瞳檣 葆擊曖 鼻欽檜棻.
			else if(clWorld->clIGuild.IsEnemyRelationWar(uiGuildID, pPlayer->uiGuildID) == TRUE)
				pVillageInfo->dwGuildRelation		=	ON_RET_GLD_WAR;	// 奢撩 ж朝 陳檜棻.
		}

		// 措陛 囀萄蒂 橢橫螞棻.
		pVillageInfo->cNationCode	=	cNation;
	
		// 梓嫦擊 橢橫螞棻.	
		pVillageInfo->siGuildFlag	=	siGuildFlag;

		return	sizeof( OnRecvVillageInfo );
	}
	else
	{
		return	0;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	梓嫦擊 橢橫除棻
//	熱薑 橾濠	:	2002-04-04 螃�� 2:52:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage::GetGuildFlag()
{
	return siGuildFlag;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	梓嫦擊 滲唳и棻 
//	熱薑 橾濠	:	2002-04-04 螃�� 3:43:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::ChangeGuildFlag(SI32 siFlag)
{
	siGuildFlag	=	siFlag;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	憮幗 On 衛 蟾晦 葆擊 濠獄擊 檗橫 菟檣棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 5:00 - 檜撩營
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::GetVillageCapital()
{	
	BOOL bRet;
	bRet = clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETVILLAGECAPITAL, this, NULL);

	return bRet;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 模嶸и 儅骯勒僭擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 7:23:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
stFarmData*	SOVillage::GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot )
{
	if( DoesFactoryExist() == FALSE )			return	NULL;
	
//	return	m_pFactory->GetOwnFactory( siPlayerDBAccount, uiPlayerDBSlot );	
	return	NULL;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-08-07 螃�� 2:36:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::InitStaticVariable(SOWorld *pWorld)
{
	clWorld		=	pWorld;

	SOVillage_Market::InitStaticVariable( &clWorld->clItem, &clWorld->clIProfit, clWorld->clDatabase );
	SOVillage_IndustrialComplexs::InitStaticVariable( &clWorld->clItem, clWorld->clDatabase, &clWorld->clBuilding );
	SOVillage_Bank::InitStaticVariable( clWorld->clDatabase, &clWorld->clOnlineEconomy );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑 氈朝 勒僭菟擊 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 7:12:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
			
		case ON_VIL_ST_KIND_MARKET :						// 衛瞪檜棻.
		case ON_VIL_ST_KIND_WSTORE : 
		case ON_VIL_ST_KIND_FSTORE :
		case ON_VIL_ST_KIND_DSTORE :
			m_pMarket	=	new	SOVillage_Market;
			
			// 衛瞪擊 蟾晦�� и棻.		
			if( m_pMarket->Init( uiVillageCode, pVillageHeader->uiItemID, pVillageHeader->siProperty, pVillageHeader->siQuantity ) == FALSE )	return	FALSE;
			
			break;
			

		case ON_VIL_ST_KIND_INN :							// 輿虞檜棻.

			break;
			

		case ON_VIL_ST_KIND_BANK :							// 瞪濰檜棻.
		
			m_pBank		=	new	SOVillage_Bank();

			if( m_pBank->Init( uiVillageCode ) == FALSE )		return	FALSE;
/*
			// 斜 葆擊縑 擎ч檜 氈棻賊 擎ч曖 等檜顫蒂 儅撩и棻.
			pstBankData		=	new	stBankData;
			ZeroMemory(pstBankData, sizeof(stBankData));
			UpdateBankInfo();
*/
			
			break;


		case ON_VIL_ST_KIND_WHARF :							// 睡舒檜棻.
			m_Wharf		=	new	SOVillage_Wharf;
			
			// 衛瞪擊 蟾晦�� и棻.		
			if( m_Wharf->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;
			break;

		// 唳衙模檜棻.
		case ON_VIL_ST_KIND_AUCTION :	
			break;


		case ON_VIL_ST_KIND_FARM :							// 儅骯 衛撲僭檜棻.
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

			// 儅骯 衛撲擊 蟾晦�� и棻.
			//if( m_pFactory->Init( uiVillageCode, clWorld->clBuilding.GetPlants( pszPlantCode ) ) == FALSE )		return	FALSE;			
			if( m_pIndustrialComplexs->Init( uiVillageCode, cStructureKind ) == FALSE )							return	FALSE;			

			// 儅骯 衛撲曖 渠и 等檜顫蒂 DB縑憮 橢橫螞棻.
			if( GetFarmData() == FALSE )																		return	FALSE;

			break;

		case ON_VIL_ST_KIND_HOSPITAL:
			m_Hospital	=	new	SOVillage_Hospital;
			
			// 衛瞪擊 蟾晦�� и棻.		
			if( m_Hospital->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this) == FALSE )	return	FALSE;
			break;

		case ON_VIL_ST_KIND_BARRACK:
		case ON_VIL_ST_KIND_GBARRACK:
		case ON_VIL_ST_KIND_MBARRACK:
			if(m_Barrack == NULL)
			{
				m_Barrack	=	new	SOVillage_Barrack;
				
				// 衛瞪擊 蟾晦�� и棻.		
				if( m_Barrack->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;
			}
			break;

		case ON_VIL_ST_KIND_GOVERNMENT:
			m_Government	=	new	SOVillage_Government;
			
			// 衛瞪擊 蟾晦�� и棻.		
			if( m_Government->Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM, this ) == FALSE )	return	FALSE;			
			break;
		}		
	}
	
	return	TRUE;
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 勒僭縑 菟橫除棻.
//	熱薑 橾濠	:	2002-10-02 螃�� 7:47:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::EnterStructure( SOPlayer *pPlayer, SI32 siStructureKind, LPARAM lParam1, LPARAM lParam2 )
{ 
	
	switch( siStructureKind )
	{

	case ON_VIL_ST_KIND_MARKET :						// 衛瞪檜棻.
	case ON_VIL_ST_KIND_WSTORE : 
	case ON_VIL_ST_KIND_FSTORE :
	case ON_VIL_ST_KIND_DSTORE :
		
		if( m_pMarket->EnterPlayer( pPlayer ) == FALSE )							return	FALSE;
		
		pPlayer->stAccountInfo.uiStatusInVillage	=	INVILLAGE_STATUS_MARKET;

		break;	
		
	case ON_VIL_ST_KIND_FARM :							// 儅骯 衛撲僭檜棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 勒僭縑憮 釭除棻.	
//	熱薑 橾濠	:	2002-10-02 螃�� 7:47:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑 擎ч檜 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 6:56:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DoesHospitalExist()
{
	return	DoesStructureExist( ON_VIL_ST_KIND_HOSPITAL );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑 衛瞪檜 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 6:59:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DoesMarketExist()
{
	if( DoesStructureExist( ON_VIL_ST_KIND_MARKET ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_WSTORE ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_FSTORE ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_DSTORE ) == TRUE )		return	TRUE;

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑 儅骯 衛撲僭檜 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-04 螃�� 7:00:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DoesFactoryExist()
{
	if( DoesStructureExist( ON_VIL_ST_KIND_FARM ) == TRUE )			return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_WEAPON ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_ARMOR ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_FACTORY ) == TRUE )		return	TRUE;
	if( DoesStructureExist( ON_VIL_ST_KIND_PASTURE ) == TRUE )		return	TRUE;

	return	FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊縑 瞪濰檜 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-18 螃�� 3:41:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DoesBankExist()
{
	return	DoesStructureExist( ON_VIL_ST_KIND_BANK );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-11-27 螃�� 2:50:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::GetInfo( OnGuildVillageInfo *pGuildVillageInfo )
{
	CHAR		*pMaxInvestmentorName;

	ZeroMemory( pGuildVillageInfo, sizeof( OnGuildVillageInfo ) );

	// 葆擊曖 囀萄 盪濰
	pGuildVillageInfo->uiVillageCode			=	uiVillageCode;	
	
	if( m_Government != NULL )
	{
		// 葆擊曖 譆堅 癱濠濠 盪濰.
		if( ( pMaxInvestmentorName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )
		{
			strncpy( pGuildVillageInfo->szBestInvestorName, pMaxInvestmentorName, ON_ID_LENGTH );
		}			

		// 葆擊曖 譆堅 癱濠旎 盪濰.
		pGuildVillageInfo->mnBestInvestorMoney		=	m_Government->GetInvestmentMaxInvestmentorMoney();
	}	

	// 葆擊曖 寞橫溘 盪濰.
	pGuildVillageInfo->vdVillageDefencePower	=	vdDefence;

	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞳 鼻欽曖 奢問縑 曖п憮 嫡擎 等嘐雖蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2003-01-01 螃瞪 5:11:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::InitDamageAttackedByGuild()
{
	ZeroMemory( m_stAttackGuild, sizeof( m_stAttackGuild ) );	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	
//	熱薑 橾濠	:	2003-01-01 螃瞪 5:16:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	
//	熱薑 橾濠	:	2003-01-01 螃瞪 5:28:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 10偃曖 鼻欽縑啪虜 爾鼻擊 п遽棻.	
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

			// 鼻欽曖 渠寞縑啪 爾鼻旎擊 遽棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊擊 奢問и 鼻欽曖 蝸煜擊 橢橫螞棻.
//	熱薑 橾濠	:	2003-01-03 螃瞪 10:43:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 奢撩檜 衛濛腎歷棻.
//	熱薑 橾濠	:	2003-01-14 螃�� 3:51:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::EnterAttackVillageTimeZone()
{	
	InitDamageAttackedByGuild();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊 奢撩檜 衛濛腎歷棻.
//	熱薑 橾濠	:	2003-01-14 螃�� 3:51:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::LeaveAttackVillageTimeZone()
{
	m_bVillageAttack = FALSE;
	m_dwVillageDamageUpdateTime = 0;
	ZeroMemory(m_stAttackGuild,sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	棻擠 濰陳擊 橢橫螞棻.
//	熱薑 橾濠	:	2003-04-11 螃瞪 11:00: - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage::GetNextMarketDay()
{
	return m_siNextMarketDay;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	棻擠 濰陳擊 撲薑и棻.
//	熱薑 橾濠	:	2003-04-11 螃瞪 11:03: - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::SetNextMarketDay()
{
	SI32		siMaxDay;

	// 3橾��, 5橾�� 傳朝 7橾�鐘� 楠渾ж啪 摹鷗ж罹 濰陳 撲薑
	switch( rand() % 3 )
	{
	case 0:
		m_siNextMarketDay += 3;			break;
	case 1:
		m_siNextMarketDay += 5;			break;
	case 2:
		m_siNextMarketDay += 7;			break;
	}

	// ⑷營 殖曖 橾熱蒂 掘и棻.
	siMaxDay	=	clWorld->Timer.GetDaysInMonth( clWorld->CurrentTime );

	// 殖曖 葆雖虞 陳擊 剩橫陛朝 唳辦 棻擠殖曖 橾濠煎 夥羞棻.
	if( m_siNextMarketDay > siMaxDay )	m_siNextMarketDay	-=	siMaxDay;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰陳擊 蟾晦�倆挬�.
//	熱薑 橾濠	:	2003-04-11 螃�� 02:30: - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::InitMarketDay()
{
	// ⑷營 陳瞼煎 蟾晦�倆� 棻擠 棻擠 濰陳擊 撲薑п菩棻.
	m_siNextMarketDay	=	clWorld->CurrentTime.Day;
	SetNextMarketDay();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 檣掘 婦溼 л熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	
	//堅辨陛棟 檣掘熱 (譆渠檣掘熱曖 30% 檜頂)蒂 蟾婁ж雖 彊戲賊
/*	if(m_uiLoadQuantity >= uiTempPopulation)
	{
		return TRUE;
	}
	*/
	return FALSE;
}

BOOL	SOVillage::LimitDecreasePopulation(UI32	uiDecreasePopulation)
{
	// ⑷營 檣掘縑憮 檣掘蒂 馬模и棻.
	UI32 uiTempPopulation = uiPopulation - uiDecreasePopulation;

	// 馬模и 檣掘陛 葆擊曖 譆盪 檣掘爾棻 濛戲賊
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

//DB縑憮 葆擊 檣掘熱, 葆擊 熱綠煽,葆擊 寞橫溘縑 婦и 薑爾蒂 橢橫螞棻.
BOOL	SOVillage::GetDBPopulation()
{	
	DOUBLE	dTempDefence;
	DBIKGetVillageInfo TempGetVillageInfo;
	if(clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_VILLAGEPOPULATION, 
									  LPVOID(&uiVillageCode), LPVOID(&TempGetVillageInfo)) == TRUE)
	{	
		//葆擊 檣掘熱蒂 夥鰻戲煎 譆渠 寞橫溘婁 譆渠 離轎й熱 氈朝 奢撩嶸棉曖 熱蒂 橢橫螞棻.
		uiPopulation	   = (UI32)TempGetVillageInfo.uiPopulation;				 // ⑷營 檣掘熱.
//		m_siDefenceSoldier = (SI32)TempGetVillageInfo.siDefenceSoldier;			 // ⑷營 葆擊 熱綠煽 
//		m_siMaxDefenceSoldier = m_siDefenceSoldier;								 // 葆擊曖 譆渠 熱綠煽.
//		m_siDefenceSoldierInDB = m_siDefenceSoldier;
		vdMaxDefence	   = (UI32)TempGetVillageInfo.uiMaxPopulation/100 * 70;  // 譆渠 寞橫溘擊 機等檜おи棻.
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

		m_uiLoadQuantity   = (UI32)TempGetVillageInfo.uiMaxPopulation/100 * 30;	 // 葆擊縑憮 離轎 й熱 氈朝 譆渠 檣掘熱.
		uiMaxPopulation	   = (UI32)TempGetVillageInfo.uiMaxPopulation;           // 葆擊曖 譆渠 檣掘熱.

		return TRUE;
	}
	return FALSE;
}

// 葆擊 檣掘熱,譆渠 檣掘熱蒂 盪濰и棻.
void	SOVillage::SetDBPopulation()
{
	UI32	TempIncreasePopulation;
	DOUBLE	dTempDefence;

//	TempIncreasePopulation = uiPopulation/100*5;
	TempIncreasePopulation = uiPopulation * 0.01;

	//隸陛ж朝 檣掘熱陛 葆擊曖 譆渠 檣掘熱蒂 剩橫憮賊 譆渠 檣掘熱煎 堅薑衛鑑棻.
	if(TempIncreasePopulation + uiPopulation > m_uiLimitMaxPopulation)
	{
		TempIncreasePopulation = m_uiLimitMaxPopulation - uiPopulation;
	}

	//檣掘熱 
	uiPopulation     += TempIncreasePopulation;
	
	//譆渠 檣掘熱
	uiMaxPopulation  = uiPopulation; 
	
	//堅辨 й熱 氈朝 譆渠 檣掘熱.
	m_uiLoadQuantity = uiMaxPopulation/100*30;
	
	//葆擊曖 譆渠 寞橫溘.
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

	//葆擊 檣掘熱蒂 夥鰻戲煎 葆擊曖 譆渠 寞橫溘,譆渠 離轎й熱 氈朝 奢撩嶸棉曖熱 蛔擊 盪濰и棻.
	vdDefence = vdMaxDefence;
	SaveDefenceToDB();

	
	DBIKSetVillageInfo	TempSetVillageInfo;
	memset(&TempSetVillageInfo, 0, sizeof(DBIKSetVillageInfo));

	TempSetVillageInfo.uiVillageCode	=	uiVillageCode;
	TempSetVillageInfo.uiPopulation		=	uiPopulation;
	TempSetVillageInfo.uiMaxPopulation	=	uiMaxPopulation;

	clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASE_POPULATION, LPVOID(&TempSetVillageInfo), sizeof(DBIKSetVillageInfo));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊 奢撩 婦溼 л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	//m_siVillageAttackType Village陛 陛雖堅 氈朝 葆擊曖 奢問⑽鷓.
	m_siVillageAttackType	  = siAttackType;
	m_dwPrevAttackdelay		  =	clWorld->dwtimeGetTime;	 //棻擠 晦獎擊 餌辨ж晦 梱雖 裁溯檜
	m_dwPrevHitTime			  = clWorld->dwtimeGetTime;  //晦獎曖 嫦儅 衛除 羹觼.

	//葆擊 寞橫溘橾 唳辦 20蟾
	if(m_siVillageAttackType == ON_VILLAGE_ATTACK_DEFENCE)
		m_dwSkillDelayTime  = 30 * 1000;

//		m_dwSkillDelayTime	= 20000;
	//給 奢問, 晦葷 奢問 10蟾
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

	// 詭衛雖蒂 嫡擊 Ы溯檜橫蒂 瓊朝棻.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	if(siSendCharNum != 0)
	{
		// в萄縑憮 詭衛雖蒂 爾鳥 輿羹蒂 瓊朝棻.
		for(i = 0; i < siSendCharNum; i++)
		{
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// 檜 葆擊檜 爾檜朝 賅萇 Ы溯檜橫菟縑啪 詭衛雖蒂 爾頂遽棻.
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

	//避擎 嶸棉檣雖 匐餌и棻.
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
	pPlayer->m_dwAttackDelay =  dwNowTick;//⑷營 衛除擊 撲薑п遽棻.

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
	//唯堅и 奢問檜 �鷝挾Ц�棻賊.
	if(siAttackPower >= siRand)
	{	
		//錳楚 輿溥湍 等嘐雖曖 1.5寡曖 等嘐雖蒂 遽棻.
		nDamage = nDamage * 1.5;
		siAttackType = ON_RET_ATTACK_POWER;
	}

//	nDamage = 10000;
	DecreaseVillageDefence( nDamage );
	AddDamageAttackedByGuild(pPlayer->uiGuildID, (SI32)nDamage);

	//⑷薯 衛除擊 撲薑 п遽棻.
	m_bVillageAttack		 = TRUE;

	if(vdDefence <= 0)
	{
		if(!m_bIsFall)
		{
			MONEY	TempMoney = 0;
			// 葆擊檜 諫瞪�� だ惚腎歷棻.
			// 譆癱曖 癱濠旎虜 50%梧朝棻.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
		
			// 葆擊擊 奢問и 鼻欽曖 渠寞縑啪 爾鼻擊 п遽棻.		
			RewardToGuildForDestroyVillage(TempMoney);
			bIsVillageDestroyed		=	TRUE;
			m_bIsFall               =   TRUE;
		}
	}
	else
	{
		DWORD dwNowTick = GetTickCount();
		if(dwNowTick - pPlayer->m_dwMessageTime <= 30)
		{	// 葆擊檜 奢問 渡и棻朝 詭衛雖蒂 爾頂遽棻.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// 葆擊曖 ч熱縑啪紫 憲溥遽棻.
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

	// 奢問縑 渠и 擬港擊 爾頂遽棻.
	pPlayer->SendFieldMsg(ON_RESPONSE_ATTACK_VILLAGE, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiKind),  //奢撩嶸棉樓撩
						LPVOID(this),			  //葆擊			
						LPVOID(nDamage),          //等嘐雖高
						LPVOID(siAttackType),     //奢問顫殮  
						LPVOID(IsLong));          //斬剪葬檣雖 錳剪葬檣雖...

	return bIsVillageDestroyed;
}

/*
//葆擊檜 斬剪葬 奢問擊 渡и棻.
//葆擊擎 Ы溯檜橫縑啪 奩問й熱 氈棻.
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

	// Ы溯檜橫陛 嶸�褲狫� 彊棻賊 葬欐и棻.
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

	//奢3機 奢問
	if(siAttackPower >= siRand)
	{	
		//錳楚 輿溥湍 等嘐雖曖 1.5寡曖 等嘐雖蒂 遽棻.
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

	// 虜擒 寞橫溘檜 0檜 腎歷棻賊 模樓脹 鼻欽縑憮 薯剪ж紫煙 ж堅	
	if(m_siDefenceSoldier <= 0)
	{	
		if(clWorld->m_FieldBattleParser.GetUnitKind(pPlayer->uiKind) == FIELD_BATTLE_SWORD)
		{
			MONEY	TempMoney = 0;
			// 葆擊檜 諫瞪�� だ惚腎歷棻.
			// 譆癱曖 癱濠旎虜 50%梧朝棻.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
			// 葆擊擊 奢問и 鼻欽曖 渠寞縑啪 爾鼻擊 п遽棻.		
			RewardToGuildForDestroyVillage(TempMoney);
			//譆癱蒂 匐餌.
//			m_Government->VillageInvestment_CheckBestInvestmentor();
			bIsVillageDestroyed		=	TRUE;
		}
	}		
	else
	{		
		// 葆擊檜嬴霜 だ惚腎雖 彊懊棻.

		if(GETTIMEGAP(dwLastAOVillageBeAttacked, clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			dwLastAOVillageBeAttacked	=	clWorld->dwtimeGetTime;

			// 葆擊檜 奢問 渡и棻朝 詭衛雖蒂 爾頂遽棻.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// 葆擊曖 ч熱縑啪紫 憲溥遽棻.
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

	//熱綠煽熱  = 熱綠煽曖 羹溘/100 
	SI16 siDefenceSoldier = (SI16)(GetDefenceSoldier()/100);
	//葆擊檜 Ы溯檜橫蒂 奢問и 等嘐雖蒂 湍螳遽棻.
	//1.熱綠煽熱 2.PlayerID 3.VillageAttackType
	SI16 siPlayerDamage= clWorld->m_FieldBattleParser.GetCalcDamageByDefenceSoldier(siDefenceSoldier,
		clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),m_siVillageAttackType);

	pPlayer->DecreaseFieldHp(siPlayerDamage);


	// 奢問縑 渠и 擬港擊 爾頂遽棻.
	pPlayer->SendFieldMsg(ON_RESPONSE_ATTACK_VILLAGE, SO_SFM_MEANDNEARCHAR, 
						LPVOID(pPlayer->uiKind),
						LPVOID(this),						
						LPVOID(siCastleDamage),
						LPVOID(siDefenceDamage),
						LPVOID(siPlayerDamage),
						LPVOID(siAttackPowerType));

	return	bIsVillageDestroyed;
}

//葆擊檜 錳剪葬 奢問擊 渡и棻.
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


	// Ы溯檜橫陛 嶸�褲狫� 彊棻賊 葬欐и棻.
	if(pPlayer == NULL)		return FALSE;
	
	SI16 siAttackPower  = clWorld->m_AttackPowerParser.GetAttackPercent(pPlayer->GetFightingPower());

	SI16 siRand = rand()%100 + 1;

	if((pPlayer->siClassInGuild == clWorld->clIGuildClass.GetHNDCode()) || (pPlayer->siVMercenarySlot > 0))
	{
		siAttackPower  = 5;
	}


	//奢3機 奢問
	if(siAttackPower >= siRand)
	{	
		//錳楚 輿溥湍 等嘐雖曖 1.5寡曖 等嘐雖蒂 遽棻.
		float fTempDamage = (float)siCastleDamage * 1.5;
		siCastleDamage    = (SI16)fTempDamage;

		fTempDamage		   = (float)siDefenceDamage * 1.5;
		siDefenceDamage    = (SI16)fTempDamage;

		siAttackPowerType = ON_RET_ATTACK_POWER;
	}
	

	//葆擊 寞橫溘擊 還罹遽棻.
	if((siCastleDamage > 0) && (vdDefence > 0))
		DecreaseVillageDefence( siCastleDamage );

	if(siDefenceDamage > 0 && (GetDefenceSoldier() > 0))
		DecreaseDefenceSoldier((SI16)siDefenceDamage );

	AddDamageAttackedByGuild(pPlayer->uiGuildID,(SI32)siDefenceDamage);

	//葆擊 熱綠煽 熱陛 0爾棻 濛剪釭 偽戲賊.
	if(m_siDefenceSoldier <= 0)
	{	
		if(clWorld->m_FieldBattleParser.GetUnitKind(pPlayer->uiKind) == FIELD_BATTLE_SWORD)
		{
			MONEY	TempMoney = 0;
			// 葆擊檜 諫瞪�� だ惚腎歷棻.
			// 譆癱曖 癱濠旎虜 50%梧朝棻.
			TempMoney = m_Government->GovernmentInvestmentClearInvestment();
			// 葆擊擊 奢問и 鼻欽縑啪 爾鼻擊 п遽棻.
			RewardToGuildForDestroyVillage(TempMoney);
			//m_Government->VillageInvestment_CheckBestInvestmentor();
			bIsVillageDestroyed		=	TRUE;	
		}
	}		
	else
	{		
		// 葆擊檜嬴霜 だ惚腎雖 彊懊棻.

		if(GETTIMEGAP(dwLastAOVillageBeAttacked, clWorld->dwtimeGetTime) >= DWORD(300000))
		{
			dwLastAOVillageBeAttacked	=	clWorld->dwtimeGetTime;

			// 葆擊檜 奢問 渡и棻朝 詭衛雖蒂 爾頂遽棻.
			clWorld->OfficialAnnouncement(ON_RET_OA_VILL_ATTACKED, LPARAM(uiVillageCode));

			// 葆擊曖 ч熱縑啪紫 憲溥遽棻.
			if( ( pszHighestInvestmentorPlayerName = m_Government->GetInvestmentMaxInvestmentorName() ) != NULL )				
			{
				if( ( pHighestInvestmentorPlayer = clWorld->GetPlayer( pszHighestInvestmentorPlayerName ) ) != NULL )
				{
					pHighestInvestmentorPlayer->SendFieldMsg( ON_MYVILLAGE_ATTACKED, SO_SFM_ONLYME, LPVOID( uiVillageCode ) );
				}				
			}
		}
	}

	//熱綠煽熱  = 熱綠煽曖 羹溘/100 
	SI16 siDefenceSoldier = (SI16)(GetDefenceSoldier()/100);
	//葆擊檜 Ы溯檜橫蒂 奢問и 等嘐雖蒂 湍螳遽棻.
	//1.熱綠煽熱 2.PlayerID 3.VillageAttackType
	SI16 siPlayerDamage= clWorld->m_FieldBattleParser.GetCalcDamageByDefenceSoldier(siDefenceSoldier,
		clWorld->m_FieldBattleParser.GetUnitKind(pFollower->uiCharID),m_siVillageAttackType);

	pPlayer->DecreaseFieldHp(siPlayerDamage);

	// 奢問縑 渠и 擬港擊 爾頂遽棻.
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

	// ⑷營 Defence陛 Max檜賊 斜傖 葬欐и棻.
	if(vdDefence == vdMaxDefence)	return;

	IncreaseVillageDefence(vddefence);

	// 詭衛雖蒂 嫡擊 Ы溯檜橫蒂 瓊朝棻.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	// 葆擊曖 寞橫溘擊 盪濰и棻.	
	if( bSendToDB == TRUE )
	{
		SaveDefenceToDB();
	}

	if(siSendCharNum != 0)
	{
		// в萄縑憮 詭衛雖蒂 爾鳥 輿羹蒂 瓊朝棻.
		for(i = 0; i < siSendCharNum; i++)
		{		
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// 檜 葆擊檜 爾檜朝 賅萇 Ы溯檜橫菟縑啪 詭衛雖蒂 爾頂遽棻.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_DEFENCE, SO_SFM_CUSTOM, LPVOID(uiVillageCode), LPVOID(&vdDefence), LPVOID(siSendCharNum), LPVOID(uiCharUniIDs));
				break;
			}	
		}	
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	だ惚脹 葆擊擊 uiIncreasePercent pecent 虜躑  Я蒂 瓣錶遽棻.
//	熱薑 橾濠	:	2004-04-06 - 檜團熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		vdDefence	=	0x7fffffffffffffff;    // 10霞熱煎 ж賊 擒922唳(922 3372 0368 5477 5807)

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻蝓и 葆擊曖 寞橫溘 薑爾蒂 爾頂遽棻.
//	熱薑 橾濠	:	2004-04-06 - 檜團熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::AutoRestorationVillageDefence(BOOL bSendToDB)
{
	if(AutoRestorationDefence(50) == FALSE)
		return;

	UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	SI32		siSendCharNum;
	SI32		i;
	SOPlayer	*pPlayer;

	// 詭衛雖蒂 嫡擊 Ы溯檜橫蒂 瓊朝棻.		
	siSendCharNum	=	clWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siVillageX , siVillageY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);

	// 葆擊曖 寞橫溘擊 盪濰и棻.	
	if( bSendToDB == TRUE )
	{
		SaveDefenceToDB();
	}

	if(siSendCharNum != 0)
	{
		// в萄縑憮 詭衛雖蒂 爾鳥 輿羹蒂 瓊朝棻.
		for(i = 0; i < siSendCharNum; i++)
		{
			pPlayer	=	clWorld->pPlayerOnAccount[uiCharUniIDs[i]];

			if(pPlayer != NULL)
			{
				// 檜 葆擊檜 爾檜朝 賅萇 Ы溯檜橫菟縑啪 詭衛雖蒂 爾頂遽棻.
				pPlayer->SendFieldMsg(ON_CHANGED_VILLAGE_DEFENCE, SO_SFM_ONLYME, LPVOID(uiVillageCode), LPVOID(&vdDefence), LPVOID(siSendCharNum), LPVOID(uiCharUniIDs));
				break;
			}	
		}	
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 寞橫溘擊 DB縑 盪濰и棻.
//	熱薑 橾濠	:	2002-03-29 螃�� 3:02:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::SaveDefenceToDB()
{
	DBIKVillageDurability	VillageDurability;

	// 盪濰擊й 熱 氈朝 鼻�窕恔� 羹觼и棻.
	if(vdDefence == vdDefenceInDB)	return;		// 夥船雖 彊懊棻賊 盪濰й в蹂陛 橈棻.

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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寞橫溘擊 隸陛 衛鑑棻.
//	熱薑 橾濠	:	2002-03-30 螃�� 2:30:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::IncreaseVillageDefence(VDEF IncreaseDefence)
{
	if((0x7fffffffffffffff - vdDefence) >=  IncreaseDefence)
	{
		vdDefence	=	min(vdDefence + IncreaseDefence, vdMaxDefence);		
	}		
	else
		vdDefence	=	0x7fffffffffffffff;    // 10霞熱煎 ж賊 擒922唳(922 3372 0368 5477 5807)
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寞橫溘擊 賓棻.
//	熱薑 橾濠	:	2002-03-30 螃�� 2:30:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寞橫溘擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-01 螃�� 3:58:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VDEF	SOVillage::GetVillageDefence()
{
	return	vdDefence;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

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
	//鼻欽檜 檜葆擊擊 模嶸ж堅 氈朝雖 匐餌 и棻.
	return clWorld->clIGuild.CheckVillage(uiVillageCode);
}
