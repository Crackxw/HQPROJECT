#include "SOMain.h"
#include "SOWorld.h"
#include "SOShip.h"
#include "OnlineShipKI.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltSOShip::cltSOShip() : BaseChar()
{	
	uiDepartureVillageCode		=	0;
	uiDestinationVillageCode	=	0;
	siMovePathID				=	0;
	uiMovePointCounter			=	0;	
	uiMaxMovePointNum			=	0;
	uiGuildID					=	0;
	siClassInGuild				=	0;

	CustomerID.Create(SO_SHIP_MAX_CUSTOMER);
	CustomerDBAccount.Create(SO_SHIP_MAX_CUSTOMER);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltSOShip::~cltSOShip()
{
	CustomerID.Release();
	CustomerDBAccount.Release();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltSOShip::Init(UI16 uiaccount, UI16 uimapid, UI16 uishipid, UI32 uideparturevillagecode, UI32 uidestinationvillagecode, SI32 simovepathid, UI16 uiCustomerNum, UI16 *puiCustomerArray)
{	
	UI16						uiMoveSpeed;		
	SOPlayer					*pPlayer;
	SI32						i;	
	UI16						uiPlayerAccount;
	UI16						uiAroundCharInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	OnlineReceiveFieldItemInfo	AroundFieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	UI16						uiAroundCharNum;
	UI16						uiAroundItemNum;
	//SI32						siTempX, siTempY;
	POINT						ptPos;

	// 熱辨 檣錳爾棻 渦 號檜 驍 熱朝 橈棻.
	if( pMyWorld->clIOnlineShipKI.GetMaxCustomer( uishipid ) < uiCustomerNum )
	{
//		pMyWorld->clServer->WriteInfo("..\\CustomerNum Exceed.txt", "[ %d ]", uiCustomerNum);			
		writeInfoToFile("CustomerNum Exceed.txt", "[ %d ]", uiCustomerNum);			
		return FALSE;
	}

	BaseChar::Init(uiaccount, uimapid, 0, 0, uishipid, NULL);		
	
	uiDepartureVillageCode			=	uideparturevillagecode;
	uiDestinationVillageCode		=	uidestinationvillagecode;
	siMovePathID					=	simovepathid;		
		
	// 寡曖 薑爾蒂 橢橫螞棻.
	uiMoveSpeed						=	pMyWorld->clIOnlineShipKI.GetMoveSpeed(uiKind);
	uiMaxMovePointNum				=	pMyWorld->clOnlineSMI.GetPathPointNum(simovepathid);	
		
	dwMoveDelayTime					=	SO_CONVERSION_CHARMOVEPSEED(uiMoveSpeed);
	dwPrevMoveTime					=	timeGetTime();
	dwRealPrevMoveTime				=	dwPrevMoveTime;

	// CustomerID諦 CustomerDBAccount蒂 蟾晦�� и棻.	
	CustomerID.DeleteAll();
	CustomerDBAccount.DeleteAll();

	for(i = 0; i < uiCustomerNum; i++)
	{
		uiPlayerAccount		=	puiCustomerArray[i];
		CustomerID.Add(uiPlayerAccount);

		pPlayer					=	pMyWorld->pPlayerOnAccount[uiPlayerAccount];	

		// 寡縑 鱉蝓и Ы溯檜橫菟縑啪 寡蒂 鰾棻堅 憲溥遽棻.
		pPlayer->BoardShip(uiAccount);

		CustomerDBAccount.Add(pPlayer->stAccountInfo.siDBAccount);			
	}	

	
/*
	// 辦摹 羅 嬪纂蒂 橢橫諦憮 寡蒂 斜夠縑憮 撲薑и棻.	
	if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, 0, &siTempX, &siTempY) == FALSE)
		printf("GetPoint : [%d] :  0", siMovePathID);

	// 棻擠 嬪纂蒂 橢橫諦憮 寡曖 跡ル 嬪纂蒂 撲薑и棻.
	if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, 1, &siDX, &siDY) == FALSE)
		printf("GetPoint : [%d] :  1", siMovePathID);
*/
	pMyWorld->clOnlineSMI.GetMovePoints(siMovePathID, &ptDeparture, &ptDepartureWarp, &ptDestinationWarp, &ptDestination);

	ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, ptDeparture.x, ptDeparture.y, ON_MAP_SI_CHAR, uiMoveableTileInfo);
	
	// 謝ル 撲薑
	siX		=	ptPos.x;
	siY		=	ptPos.y;	
	siDX	=	ptDepartureWarp.x;
	siDY	=	ptDepartureWarp.y;	

	uiMovePointCounter				=	0;						

	// ⑷營 裘縑棻陛 濠晦蒂 撲薑и棻.
	if(SetIDOnMap( 16 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "9");	
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "9");	
	else
	{

//		pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], 裘縑 撲薑, 蟾晦��.", uiAccount );
	}
	
	// 輿嬪縑 議葛攪諦 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
	uiAroundCharNum		=	pMyWorld->clISOMap.GetNearSectorPlayerList( uiMapID, siX, siY, uiAroundCharInfo, ON_MAP_CHARID_FIELD );
	uiAroundItemNum		=	pMyWorld->clISOMap.GetNearSectorItemList( uiMapID, siX, siY, AroundFieldItemInfo );		

	SendFieldMsg(ON_BOARDSHIP, SO_SFM_CUSTOM, LPVOID(uiAroundCharNum), LPVOID(uiAroundItemNum), LPVOID(uiAroundCharInfo), LPVOID(AroundFieldItemInfo));

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltSOShip::Free()
{	
	// 裘縑憮 釭蒂 餉薯и棻.
	if(DelIDOnMap( 17 ) == FALSE)
	{
//		pMyWorld->clServer->WriteInfo( "..\\DelIDOnMap Failed.txt", "7 [ uiAccount : %u ] [ siX : %d ] [ siY : %d ] [ uiDepartureVillageCode : %d ] [ uiDestinationVillageCode : %d ] [ siMovePathID : %d ] [ ptDeparture : %d, %d ] [ ptDepartureWarp : %d, %d ] [ ptDestinationWarp : %d, %d ] [ ptDestination : %d, %d ]", 
		writeInfoToFile( "DelIDOnMap Failed.txt", "7 [ uiAccount : %u ] [ siX : %d ] [ siY : %d ] [ uiDepartureVillageCode : %d ] [ uiDestinationVillageCode : %d ] [ siMovePathID : %d ] [ ptDeparture : %d, %d ] [ ptDepartureWarp : %d, %d ] [ ptDestinationWarp : %d, %d ] [ ptDestination : %d, %d ]", 
									uiAccount, siX, siY,
									uiDepartureVillageCode, 
									uiDestinationVillageCode, 
									siMovePathID,
									ptDeparture.x, ptDeparture.y,
									ptDepartureWarp.x, ptDepartureWarp.y,
									ptDestinationWarp.x, ptDestinationWarp.y, 
									ptDestination.x, ptDestination.y );
	}
	else
	{
//		pMyWorld->clServer->WriteInfo( "..\\ShipLog.txt", "Account : [ %u ], 裘縑憮 餉薯, п薯.", uiAccount );		
	}

	// Account蒂 0戲煎 и棻.
	uiAccount	=	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Poll
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltSOShip::Poll()
{
	SI16							siMX, siMY;
	BOOL							bArrive = FALSE;
	POINT							ptPos;
	UI16							uiCharFItemInfo[2];			// 0廓擎 議葛攪 熱, 1廓擎 嬴檜蠱 熱,	
	SOPlayer						*pPlayer;
	UI16							uiPlayerAccount;
	SI32							siPlayerDBAccount;
	SI32							i;
	SI32							siBoardPlayerNum;
	UI16							uiBoardPlayerAccount[SO_SHIP_MAX_CUSTOMER];
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	// 檜翕й 衛薄檣雖 っ欽и棻.
	if(GETTIMEGAP(dwPrevMoveTime, timeGetTime()) >= dwMoveDelayTime)
	{
		// 檜翕й 衛薄檜棻.	
		dwPrevMoveTime	=	timeGetTime();
 
		// 望瓊晦蒂 熱чж堅 棻擠 雖薄戲煎 檜翕擊 и棻.
		pMyWorld->clSOFindPath.FindPathL(siX, siY, siDX, siDY, &siMX, &siMY, NULL, 160, uiMapID, ON_TILE_KIND_SEA);		
		

		// 剪曖 棻 諮朝等 綰楣韁 腎歷棻賊
		// 斜傖 紫雜ц棻堅 耀棻.
		// 5蟾 翕寰 褒薯煎 и廓紫 寰 遺霜艘朝雖 羹觼и棻.
		// ⑷營 嬪纂陛 跡瞳雖諦曖 離檜陛 10蘊 檜頂檣雖 羹觼и棻.,				
		if((GETTIMEGAP(dwRealPrevMoveTime, timeGetTime()) >= 5000) && (max(abs(siX - ptDestination.x), abs(siY - ptDestination.y)) <= 10 ))
			bArrive = TRUE;		

		// 橫集и 檜嶸煎 10蟾 薑紫 寡陛 憮 氈棻賊 斜傖 紫雜ж啪 и棻.
		if((GETTIMEGAP(dwRealPrevMoveTime, timeGetTime()) >= 10000))
			bArrive = TRUE;		

		if((siX != siMX || siY != siMY))
		{	
			if(pMyWorld->clISOMap.MoveChar(this, siMX, siMY, FALSE,uiMoveableTileInfo) == TRUE)
			{
				// 錶Щй 雖薄梱雖 諮朝雖 憲嬴螞棻.
				// 錶Щ蒂 и棻.
				if(siX == ptDepartureWarp.x && siY == ptDepartureWarp.y)
				{
					if(DelIDOnMap( 18 ) == FALSE)
						writeInfoToFile("DelIDOnMap Failed.txt", "8 [%u]", uiAccount);
//						pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "8 [%u]", uiAccount);
					else
					{	
//						pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], 裘縑憮 餉薯, 錶Щ 撩奢", uiAccount );			
					}

					ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, ptDestinationWarp.x, ptDestinationWarp.y, ON_MAP_SI_CHAR, uiMoveableTileInfo);				

					// ⑷營 嬪纂蒂 營 撲薑ж堅 跡瞳雖紫 棻衛 撲薑и棻.
					siX		=	ptPos.x;
					siY		=	ptPos.y;
					siDX	=	ptDestination.x;					
					siDY	=	ptDestination.y;
					
					if(SetIDOnMap( 19 ) == FALSE)
						writeInfoToFile("SetIDOnMap Failed.txt", "10");
//						pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "10");
					else
					{
//						pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], 裘縑 撲薑, 錶Щ 撩奢", uiAccount );
					}

					// 輿嬪縑 賅萇 薑爾菟擊 橢橫螞棻.							
					uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD); 
					uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);										

					// ⑷營 嬴霜 嶸�褲� 餌塋菟虜 埤塭魚棻.
					siBoardPlayerNum		=	0;
					for(i = 0; i < CustomerID.m_NowUseDataCount; i++)
					{
						uiPlayerAccount		=	CustomerID.m_Data[i];
						siPlayerDBAccount	=	CustomerDBAccount.m_Data[i];						

						if((pPlayer = pMyWorld->pPlayerOnAccount[uiPlayerAccount]) != NULL)
						{
							if(pPlayer->stAccountInfo.siDBAccount == siPlayerDBAccount)
							{
								// 嶸�褲� 議葛攪檜棻.
								uiBoardPlayerAccount[siBoardPlayerNum]		=	uiPlayerAccount;
								siBoardPlayerNum++;
							}
						}
					}

					// 寡縑 驕 餌塋菟縑啪 賅舒 詭衛雖蒂 爾頂遽棻.,
					SendFieldMsg(ON_RESPONSE_WARP, SO_SFM_CUSTOM, LPVOID(ON_RET_OK), LPVOID(uiCharFItemInfo), 
										LPVOID(uiCharUniIDs), LPVOID(FieldItemInfo), LPVOID(siBoardPlayerNum), LPVOID(uiBoardPlayerAccount));					
				}			
				else
				{
					// ⑷營 檜翕ж溥朝 ん檣お梱雖 紫雜 ц朝雖 羹觼и棻.
					if((siMX == ptDestination.x) && (siMY == ptDestination.y))
					{				
						bArrive		=	TRUE;
					}		

		// 壽縑 睡碟擎 釭醞縑 棻衛 噙橾 熱紫 氈戲嘎煎 雖錶憮朝 寰脹棻.
		/*			
					// ⑷營 檜翕ж溥朝 ん檣お梱雖 紫雜 ц朝雖 羹觼и棻.
					if(((siMX == siDX) && (siMY == siDY)) || (bArrive == TRUE))
					{
						// 紫雜ц棻.			
						uiMovePointCounter++;

						
						// 葆雖虞 雖薄梱雖 諮朝雖 羹觼и棻
						if((uiMaxMovePointNum - 1) == uiMovePointCounter)
						{
							// 葆擊縑 紫雜ж艘棻.
							Arrive();

							// 薯 羲й擊 賅舒 棻ц棻
							return TRUE;
						}
						else
						{					
							// 傳 棻擠 雖薄擊 橢橫螞棻.
							if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, uiMovePointCounter + 1, &siDX, &siDY) == FALSE)
								printf("GetPoint : [%d] :  %d", uiMovePointCounter + 1);
						}				
					}		
		*/					
				}

				dwRealPrevMoveTime				=	dwPrevMoveTime;
			}			
		}

		if(bArrive == TRUE)
		{
			// 葆擊縑 紫雜ж艘棻.
			Arrive();

			// 薯 羲й擊 賅舒 棻ц棻
			return TRUE;								
		}
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跡瞳雖縑 紫雜ж艘擊 陽縑 ��轎腎朝 л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltSOShip::Arrive()
{	
	SI32		i;
	SOVillage	*pVillage;
	SOPlayer	*pPlayer;

	// п渡 葆擊曖 ん檣攪蒂 橢朝棻.
	pVillage	=	pMyWorld->GetVillageByCode(uiDestinationVillageCode);

	// ⑷營 鱉蝓п 氈朝 賅萇 蝓偌菟擊 葆擊煎 菟罹爾魚棻.
	for(i = 0; i < 	CustomerID.m_NowUseDataCount; i++)
	{	
		// 嬴霜 鱉蝓п 氈朝 蝓偌檜棻.
		pPlayer	=	pMyWorld->pPlayerOnAccount[CustomerID.m_Data[i]];

		// п渡 葆擊煎 菟罹爾魚棻.
		if(pPlayer != NULL)		
			pPlayer->ArriveVillageByShip(pVillage);		
	}	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	寡蒂 驕 鼻鷓縑憮 餌辨濠陛 煎斜嬴醒擊 ц棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltSOShip::LogOutPlayer(UI16 uiAccount, SI32 siDBAccount)
{	
	// Ы溯檜橫蒂 薯剪и棻.
	CustomerID.DeleteType(uiAccount);
	CustomerDBAccount.DeleteType(siDBAccount);		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寡曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-08-15 螃�� 1:11:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltSOShip::GetInfo(OnCharShipInfo *pShipInfo)
{	
	BaseChar::GetInfo((OnCharBaseInfo*)pShipInfo);
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝓摹и 餌塋菟曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-27 螃�� 7:02:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	cltSOShip::GetCustomers( UI16 *puiAccounts )
{
	if( CustomerID.m_NowUseDataCount != 0 )	
	{
		memcpy( puiAccounts, CustomerID.m_Data, sizeof( UI16 ) * CustomerID.m_NowUseDataCount );

		/*
		{
			int		siDataCounter;
			
			for( siDataCounter = 0; siDataCounter < CustomerID.m_NowUseDataCount; siDataCounter++ )
			{
				if( CustomerID.m_Data[ siDataCounter ] == 0 )
				{
					pMyWorld->clServer->WriteInfo( "..\\ZeroPlayerID.txt", "222 - [ %u, %d, %d ]", uiMapID, siX, siY );							
				}
			}
		}
		*/
	}

	return	CustomerID.m_NowUseDataCount;
}

