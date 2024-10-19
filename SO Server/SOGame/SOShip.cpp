#include "SOMain.h"
#include "SOWorld.h"
#include "SOShip.h"
#include "OnlineShipKI.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼Ò¸êÀÚ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
cltSOShip::~cltSOShip()
{
	CustomerID.Release();
	CustomerDBAccount.Release();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÃÊ±âÈ­.	
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

	// ¼ö¿ë ÀÎ¿øº¸´Ù ´õ ¸¹ÀÌ Å» ¼ö´Â ¾ø´Ù.
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
		
	// ¹èÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
	uiMoveSpeed						=	pMyWorld->clIOnlineShipKI.GetMoveSpeed(uiKind);
	uiMaxMovePointNum				=	pMyWorld->clOnlineSMI.GetPathPointNum(simovepathid);	
		
	dwMoveDelayTime					=	SO_CONVERSION_CHARMOVEPSEED(uiMoveSpeed);
	dwPrevMoveTime					=	timeGetTime();
	dwRealPrevMoveTime				=	dwPrevMoveTime;

	// CustomerID¿Í CustomerDBAccount¸¦ ÃÊ±âÈ­ ÇÑ´Ù.	
	CustomerID.DeleteAll();
	CustomerDBAccount.DeleteAll();

	for(i = 0; i < uiCustomerNum; i++)
	{
		uiPlayerAccount		=	puiCustomerArray[i];
		CustomerID.Add(uiPlayerAccount);

		pPlayer					=	pMyWorld->pPlayerOnAccount[uiPlayerAccount];	

		// ¹è¿¡ Å¾½ÂÇÑ ÇÃ·¹ÀÌ¾îµé¿¡°Ô ¹è¸¦ ÅÀ´Ù°í ¾Ë·ÁÁØ´Ù.
		pPlayer->BoardShip(uiAccount);

		CustomerDBAccount.Add(pPlayer->stAccountInfo.siDBAccount);			
	}	

	
/*
	// ¿ì¼± Ã¹ À§Ä¡¸¦ ¾ò¾î¿Í¼­ ¹è¸¦ ±×°÷¿¡¼­ ¼³Á¤ÇÑ´Ù.	
	if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, 0, &siTempX, &siTempY) == FALSE)
		printf("GetPoint : [%d] :  0", siMovePathID);

	// ´ÙÀ½ À§Ä¡¸¦ ¾ò¾î¿Í¼­ ¹èÀÇ ¸ñÇ¥ À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù.
	if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, 1, &siDX, &siDY) == FALSE)
		printf("GetPoint : [%d] :  1", siMovePathID);
*/
	pMyWorld->clOnlineSMI.GetMovePoints(siMovePathID, &ptDeparture, &ptDepartureWarp, &ptDestinationWarp, &ptDestination);

	ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, ptDeparture.x, ptDeparture.y, ON_MAP_SI_CHAR, uiMoveableTileInfo);
	
	// ÁÂÇ¥ ¼³Á¤
	siX		=	ptPos.x;
	siY		=	ptPos.y;	
	siDX	=	ptDepartureWarp.x;
	siDY	=	ptDepartureWarp.y;	

	uiMovePointCounter				=	0;						

	// ÇöÀç ¸Ê¿¡´Ù°¡ ÀÚ±â¸¦ ¼³Á¤ÇÑ´Ù.
	if(SetIDOnMap( 16 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "9");	
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "9");	
	else
	{

//		pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], ¸Ê¿¡ ¼³Á¤, ÃÊ±âÈ­.", uiAccount );
	}
	
	// ÁÖÀ§¿¡ Ä³¸¯ÅÍ¿Í ¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
	uiAroundCharNum		=	pMyWorld->clISOMap.GetNearSectorPlayerList( uiMapID, siX, siY, uiAroundCharInfo, ON_MAP_CHARID_FIELD );
	uiAroundItemNum		=	pMyWorld->clISOMap.GetNearSectorItemList( uiMapID, siX, siY, AroundFieldItemInfo );		

	SendFieldMsg(ON_BOARDSHIP, SO_SFM_CUSTOM, LPVOID(uiAroundCharNum), LPVOID(uiAroundItemNum), LPVOID(uiAroundCharInfo), LPVOID(AroundFieldItemInfo));

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇØÁ¦ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltSOShip::Free()
{	
	// ¸Ê¿¡¼­ ³ª¸¦ »èÁ¦ÇÑ´Ù.
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
//		pMyWorld->clServer->WriteInfo( "..\\ShipLog.txt", "Account : [ %u ], ¸Ê¿¡¼­ »èÁ¦, ÇØÁ¦.", uiAccount );		
	}

	// Account¸¦ 0À¸·Î ÇÑ´Ù.
	uiAccount	=	0;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Poll
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	cltSOShip::Poll()
{
	SI16							siMX, siMY;
	BOOL							bArrive = FALSE;
	POINT							ptPos;
	UI16							uiCharFItemInfo[2];			// 0¹øÀº Ä³¸¯ÅÍ ¼ö, 1¹øÀº ¾ÆÀÌÅÛ ¼ö,	
	SOPlayer						*pPlayer;
	UI16							uiPlayerAccount;
	SI32							siPlayerDBAccount;
	SI32							i;
	SI32							siBoardPlayerNum;
	UI16							uiBoardPlayerAccount[SO_SHIP_MAX_CUSTOMER];
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	// ÀÌµ¿ÇÒ ½ÃÁ¡ÀÎÁö ÆÇ´ÜÇÑ´Ù.
	if(GETTIMEGAP(dwPrevMoveTime, timeGetTime()) >= dwMoveDelayTime)
	{
		// ÀÌµ¿ÇÒ ½ÃÁ¡ÀÌ´Ù.	
		dwPrevMoveTime	=	timeGetTime();
 
		// ±æÃ£±â¸¦ ¼öÇàÇÏ°í ´ÙÀ½ ÁöÁ¡À¸·Î ÀÌµ¿À» ÇÑ´Ù.
		pMyWorld->clSOFindPath.FindPathL(siX, siY, siDX, siDY, &siMX, &siMY, NULL, 160, uiMapID, ON_TILE_KIND_SEA);		
		

		// °ÅÀÇ ´Ù ¿Ô´Âµ¥ ºí·°Å· µÇ¾ú´Ù¸é
		// ±×³É µµÂøÇß´Ù°í Ä£´Ù.
		// 5ÃÊ µ¿¾È ½ÇÁ¦·Î ÇÑ¹øµµ ¾È ¿òÁ÷¿´´ÂÁö Ã¼Å©ÇÑ´Ù.
		// ÇöÀç À§Ä¡°¡ ¸ñÀûÁö¿ÍÀÇ Â÷ÀÌ°¡ 10Ä­ ÀÌ³»ÀÎÁö Ã¼Å©ÇÑ´Ù.,				
		if((GETTIMEGAP(dwRealPrevMoveTime, timeGetTime()) >= 5000) && (max(abs(siX - ptDestination.x), abs(siY - ptDestination.y)) <= 10 ))
			bArrive = TRUE;		

		// ¾î¶°ÇÑ ÀÌÀ¯·Î 10ÃÊ Á¤µµ ¹è°¡ ¼­ ÀÖ´Ù¸é ±×³É µµÂøÇÏ°Ô ÇÑ´Ù.
		if((GETTIMEGAP(dwRealPrevMoveTime, timeGetTime()) >= 10000))
			bArrive = TRUE;		

		if((siX != siMX || siY != siMY))
		{	
			if(pMyWorld->clISOMap.MoveChar(this, siMX, siMY, FALSE,uiMoveableTileInfo) == TRUE)
			{
				// ¿öÇÁÇÒ ÁöÁ¡±îÁö ¿Ô´ÂÁö ¾Ë¾Æ¿Â´Ù.
				// ¿öÇÁ¸¦ ÇÑ´Ù.
				if(siX == ptDepartureWarp.x && siY == ptDepartureWarp.y)
				{
					if(DelIDOnMap( 18 ) == FALSE)
						writeInfoToFile("DelIDOnMap Failed.txt", "8 [%u]", uiAccount);
//						pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "8 [%u]", uiAccount);
					else
					{	
//						pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], ¸Ê¿¡¼­ »èÁ¦, ¿öÇÁ ¼º°ø", uiAccount );			
					}

					ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, ptDestinationWarp.x, ptDestinationWarp.y, ON_MAP_SI_CHAR, uiMoveableTileInfo);				

					// ÇöÀç À§Ä¡¸¦ Àç ¼³Á¤ÇÏ°í ¸ñÀûÁöµµ ´Ù½Ã ¼³Á¤ÇÑ´Ù.
					siX		=	ptPos.x;
					siY		=	ptPos.y;
					siDX	=	ptDestination.x;					
					siDY	=	ptDestination.y;
					
					if(SetIDOnMap( 19 ) == FALSE)
						writeInfoToFile("SetIDOnMap Failed.txt", "10");
//						pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "10");
					else
					{
//						pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], ¸Ê¿¡ ¼³Á¤, ¿öÇÁ ¼º°ø", uiAccount );
					}

					// ÁÖÀ§¿¡ ¸ðµç Á¤º¸µéÀ» ¾ò¾î¿Â´Ù.							
					uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD); 
					uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);										

					// ÇöÀç ¾ÆÁ÷ À¯È¿ÇÑ »ç¶÷µé¸¸ °ñ¶ó³½´Ù.
					siBoardPlayerNum		=	0;
					for(i = 0; i < CustomerID.m_NowUseDataCount; i++)
					{
						uiPlayerAccount		=	CustomerID.m_Data[i];
						siPlayerDBAccount	=	CustomerDBAccount.m_Data[i];						

						if((pPlayer = pMyWorld->pPlayerOnAccount[uiPlayerAccount]) != NULL)
						{
							if(pPlayer->stAccountInfo.siDBAccount == siPlayerDBAccount)
							{
								// À¯È¿ÇÑ Ä³¸¯ÅÍÀÌ´Ù.
								uiBoardPlayerAccount[siBoardPlayerNum]		=	uiPlayerAccount;
								siBoardPlayerNum++;
							}
						}
					}

					// ¹è¿¡ Åº »ç¶÷µé¿¡°Ô ¸ðµÎ ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.,
					SendFieldMsg(ON_RESPONSE_WARP, SO_SFM_CUSTOM, LPVOID(ON_RET_OK), LPVOID(uiCharFItemInfo), 
										LPVOID(uiCharUniIDs), LPVOID(FieldItemInfo), LPVOID(siBoardPlayerNum), LPVOID(uiBoardPlayerAccount));					
				}			
				else
				{
					// ÇöÀç ÀÌµ¿ÇÏ·Á´Â Æ÷ÀÎÆ®±îÁö µµÂø Çß´ÂÁö Ã¼Å©ÇÑ´Ù.
					if((siMX == ptDestination.x) && (siMY == ptDestination.y))
					{				
						bArrive		=	TRUE;
					}		

		// ¹Ø¿¡ ºÎºÐÀº ³ªÁß¿¡ ´Ù½Ã ¾²ÀÏ ¼öµµ ÀÖÀ¸¹Ç·Î Áö¿ö¼­´Â ¾ÈµÈ´Ù.
		/*			
					// ÇöÀç ÀÌµ¿ÇÏ·Á´Â Æ÷ÀÎÆ®±îÁö µµÂø Çß´ÂÁö Ã¼Å©ÇÑ´Ù.
					if(((siMX == siDX) && (siMY == siDY)) || (bArrive == TRUE))
					{
						// µµÂøÇß´Ù.			
						uiMovePointCounter++;

						
						// ¸¶Áö¸· ÁöÁ¡±îÁö ¿Ô´ÂÁö Ã¼Å©ÇÑ´Ù
						if((uiMaxMovePointNum - 1) == uiMovePointCounter)
						{
							// ¸¶À»¿¡ µµÂøÇÏ¿´´Ù.
							Arrive();

							// Á¦ ¿ªÇÒÀ» ¸ðµÎ ´ÙÇß´Ù
							return TRUE;
						}
						else
						{					
							// ¶Ç ´ÙÀ½ ÁöÁ¡À» ¾ò¾î¿Â´Ù.
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
			// ¸¶À»¿¡ µµÂøÇÏ¿´´Ù.
			Arrive();

			// Á¦ ¿ªÇÒÀ» ¸ðµÎ ´ÙÇß´Ù
			return TRUE;								
		}
	}

	return FALSE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸ñÀûÁö¿¡ µµÂøÇÏ¿´À» ¶§¿¡ È£ÃâµÇ´Â ÇÔ¼ö.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltSOShip::Arrive()
{	
	SI32		i;
	SOVillage	*pVillage;
	SOPlayer	*pPlayer;

	// ÇØ´ç ¸¶À»ÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò´Â´Ù.
	pVillage	=	pMyWorld->GetVillageByCode(uiDestinationVillageCode);

	// ÇöÀç Å¾½ÂÇØ ÀÖ´Â ¸ðµç ½Â°´µéÀ» ¸¶À»·Î µé¿©º¸³½´Ù.
	for(i = 0; i < 	CustomerID.m_NowUseDataCount; i++)
	{	
		// ¾ÆÁ÷ Å¾½ÂÇØ ÀÖ´Â ½Â°´ÀÌ´Ù.
		pPlayer	=	pMyWorld->pPlayerOnAccount[CustomerID.m_Data[i]];

		// ÇØ´ç ¸¶À»·Î µé¿©º¸³½´Ù.
		if(pPlayer != NULL)		
			pPlayer->ArriveVillageByShip(pVillage);		
	}	
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¹è¸¦ Åº »óÅÂ¿¡¼­ »ç¿ëÀÚ°¡ ·Î±×¾Æ¿ôÀ» Çß´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltSOShip::LogOutPlayer(UI16 uiAccount, SI32 siDBAccount)
{	
	// ÇÃ·¹ÀÌ¾î¸¦ Á¦°ÅÇÑ´Ù.
	CustomerID.DeleteType(uiAccount);
	CustomerDBAccount.DeleteType(siDBAccount);		
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¹èÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-08-15 ¿ÀÈÄ 1:11:21 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltSOShip::GetInfo(OnCharShipInfo *pShipInfo)
{	
	BaseChar::GetInfo((OnCharBaseInfo*)pShipInfo);
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	½Â¼±ÇÑ »ç¶÷µéÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-07-27 ¿ÀÈÄ 7:02:39 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

