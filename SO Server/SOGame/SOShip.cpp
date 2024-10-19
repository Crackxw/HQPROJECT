#include "SOMain.h"
#include "SOWorld.h"
#include "SOShip.h"
#include "OnlineShipKI.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltSOShip::~cltSOShip()
{
	CustomerID.Release();
	CustomerDBAccount.Release();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� �ο����� �� ���� Ż ���� ����.
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
		
	// ���� ������ ���´�.
	uiMoveSpeed						=	pMyWorld->clIOnlineShipKI.GetMoveSpeed(uiKind);
	uiMaxMovePointNum				=	pMyWorld->clOnlineSMI.GetPathPointNum(simovepathid);	
		
	dwMoveDelayTime					=	SO_CONVERSION_CHARMOVEPSEED(uiMoveSpeed);
	dwPrevMoveTime					=	timeGetTime();
	dwRealPrevMoveTime				=	dwPrevMoveTime;

	// CustomerID�� CustomerDBAccount�� �ʱ�ȭ �Ѵ�.	
	CustomerID.DeleteAll();
	CustomerDBAccount.DeleteAll();

	for(i = 0; i < uiCustomerNum; i++)
	{
		uiPlayerAccount		=	puiCustomerArray[i];
		CustomerID.Add(uiPlayerAccount);

		pPlayer					=	pMyWorld->pPlayerOnAccount[uiPlayerAccount];	

		// �迡 ž���� �÷��̾�鿡�� �踦 ���ٰ� �˷��ش�.
		pPlayer->BoardShip(uiAccount);

		CustomerDBAccount.Add(pPlayer->stAccountInfo.siDBAccount);			
	}	

	
/*
	// �켱 ù ��ġ�� ���ͼ� �踦 �װ����� �����Ѵ�.	
	if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, 0, &siTempX, &siTempY) == FALSE)
		printf("GetPoint : [%d] :  0", siMovePathID);

	// ���� ��ġ�� ���ͼ� ���� ��ǥ ��ġ�� �����Ѵ�.
	if(pMyWorld->clOnlineSMI.GetPoint(siMovePathID, 1, &siDX, &siDY) == FALSE)
		printf("GetPoint : [%d] :  1", siMovePathID);
*/
	pMyWorld->clOnlineSMI.GetMovePoints(siMovePathID, &ptDeparture, &ptDepartureWarp, &ptDestinationWarp, &ptDestination);

	ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, ptDeparture.x, ptDeparture.y, ON_MAP_SI_CHAR, uiMoveableTileInfo);
	
	// ��ǥ ����
	siX		=	ptPos.x;
	siY		=	ptPos.y;	
	siDX	=	ptDepartureWarp.x;
	siDY	=	ptDepartureWarp.y;	

	uiMovePointCounter				=	0;						

	// ���� �ʿ��ٰ� �ڱ⸦ �����Ѵ�.
	if(SetIDOnMap( 16 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "9");	
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "9");	
	else
	{

//		pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], �ʿ� ����, �ʱ�ȭ.", uiAccount );
	}
	
	// ������ ĳ���Ϳ� �������� ������ ���´�.
	uiAroundCharNum		=	pMyWorld->clISOMap.GetNearSectorPlayerList( uiMapID, siX, siY, uiAroundCharInfo, ON_MAP_CHARID_FIELD );
	uiAroundItemNum		=	pMyWorld->clISOMap.GetNearSectorItemList( uiMapID, siX, siY, AroundFieldItemInfo );		

	SendFieldMsg(ON_BOARDSHIP, SO_SFM_CUSTOM, LPVOID(uiAroundCharNum), LPVOID(uiAroundItemNum), LPVOID(uiAroundCharInfo), LPVOID(AroundFieldItemInfo));

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltSOShip::Free()
{	
	// �ʿ��� ���� �����Ѵ�.
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
//		pMyWorld->clServer->WriteInfo( "..\\ShipLog.txt", "Account : [ %u ], �ʿ��� ����, ����.", uiAccount );		
	}

	// Account�� 0���� �Ѵ�.
	uiAccount	=	0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Poll
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltSOShip::Poll()
{
	SI16							siMX, siMY;
	BOOL							bArrive = FALSE;
	POINT							ptPos;
	UI16							uiCharFItemInfo[2];			// 0���� ĳ���� ��, 1���� ������ ��,	
	SOPlayer						*pPlayer;
	UI16							uiPlayerAccount;
	SI32							siPlayerDBAccount;
	SI32							i;
	SI32							siBoardPlayerNum;
	UI16							uiBoardPlayerAccount[SO_SHIP_MAX_CUSTOMER];
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	// �̵��� �������� �Ǵ��Ѵ�.
	if(GETTIMEGAP(dwPrevMoveTime, timeGetTime()) >= dwMoveDelayTime)
	{
		// �̵��� �����̴�.	
		dwPrevMoveTime	=	timeGetTime();
 
		// ��ã�⸦ �����ϰ� ���� �������� �̵��� �Ѵ�.
		pMyWorld->clSOFindPath.FindPathL(siX, siY, siDX, siDY, &siMX, &siMY, NULL, 160, uiMapID, ON_TILE_KIND_SEA);		
		

		// ���� �� �Դµ� ��ŷ �Ǿ��ٸ�
		// �׳� �����ߴٰ� ģ��.
		// 5�� ���� ������ �ѹ��� �� ���������� üũ�Ѵ�.
		// ���� ��ġ�� ���������� ���̰� 10ĭ �̳����� üũ�Ѵ�.,				
		if((GETTIMEGAP(dwRealPrevMoveTime, timeGetTime()) >= 5000) && (max(abs(siX - ptDestination.x), abs(siY - ptDestination.y)) <= 10 ))
			bArrive = TRUE;		

		// ��� ������ 10�� ���� �谡 �� �ִٸ� �׳� �����ϰ� �Ѵ�.
		if((GETTIMEGAP(dwRealPrevMoveTime, timeGetTime()) >= 10000))
			bArrive = TRUE;		

		if((siX != siMX || siY != siMY))
		{	
			if(pMyWorld->clISOMap.MoveChar(this, siMX, siMY, FALSE,uiMoveableTileInfo) == TRUE)
			{
				// ������ �������� �Դ��� �˾ƿ´�.
				// ������ �Ѵ�.
				if(siX == ptDepartureWarp.x && siY == ptDepartureWarp.y)
				{
					if(DelIDOnMap( 18 ) == FALSE)
						writeInfoToFile("DelIDOnMap Failed.txt", "8 [%u]", uiAccount);
//						pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "8 [%u]", uiAccount);
					else
					{	
//						pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], �ʿ��� ����, ���� ����", uiAccount );			
					}

					ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, ptDestinationWarp.x, ptDestinationWarp.y, ON_MAP_SI_CHAR, uiMoveableTileInfo);				

					// ���� ��ġ�� �� �����ϰ� �������� �ٽ� �����Ѵ�.
					siX		=	ptPos.x;
					siY		=	ptPos.y;
					siDX	=	ptDestination.x;					
					siDY	=	ptDestination.y;
					
					if(SetIDOnMap( 19 ) == FALSE)
						writeInfoToFile("SetIDOnMap Failed.txt", "10");
//						pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "10");
					else
					{
//						pMyWorld->clServer->WriteInfo("..\\ShipLog.txt", "Account : [ %u ], �ʿ� ����, ���� ����", uiAccount );
					}

					// ������ ��� �������� ���´�.							
					uiCharFItemInfo[0]		=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD); 
					uiCharFItemInfo[1]		=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);										

					// ���� ���� ��ȿ�� ����鸸 ��󳽴�.
					siBoardPlayerNum		=	0;
					for(i = 0; i < CustomerID.m_NowUseDataCount; i++)
					{
						uiPlayerAccount		=	CustomerID.m_Data[i];
						siPlayerDBAccount	=	CustomerDBAccount.m_Data[i];						

						if((pPlayer = pMyWorld->pPlayerOnAccount[uiPlayerAccount]) != NULL)
						{
							if(pPlayer->stAccountInfo.siDBAccount == siPlayerDBAccount)
							{
								// ��ȿ�� ĳ�����̴�.
								uiBoardPlayerAccount[siBoardPlayerNum]		=	uiPlayerAccount;
								siBoardPlayerNum++;
							}
						}
					}

					// �迡 ź ����鿡�� ��� �޽����� �����ش�.,
					SendFieldMsg(ON_RESPONSE_WARP, SO_SFM_CUSTOM, LPVOID(ON_RET_OK), LPVOID(uiCharFItemInfo), 
										LPVOID(uiCharUniIDs), LPVOID(FieldItemInfo), LPVOID(siBoardPlayerNum), LPVOID(uiBoardPlayerAccount));					
				}			
				else
				{
					// ���� �̵��Ϸ��� ����Ʈ���� ���� �ߴ��� üũ�Ѵ�.
					if((siMX == ptDestination.x) && (siMY == ptDestination.y))
					{				
						bArrive		=	TRUE;
					}		

		// �ؿ� �κ��� ���߿� �ٽ� ���� ���� �����Ƿ� �������� �ȵȴ�.
		/*			
					// ���� �̵��Ϸ��� ����Ʈ���� ���� �ߴ��� üũ�Ѵ�.
					if(((siMX == siDX) && (siMY == siDY)) || (bArrive == TRUE))
					{
						// �����ߴ�.			
						uiMovePointCounter++;

						
						// ������ �������� �Դ��� üũ�Ѵ�
						if((uiMaxMovePointNum - 1) == uiMovePointCounter)
						{
							// ������ �����Ͽ���.
							Arrive();

							// �� ������ ��� ���ߴ�
							return TRUE;
						}
						else
						{					
							// �� ���� ������ ���´�.
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
			// ������ �����Ͽ���.
			Arrive();

			// �� ������ ��� ���ߴ�
			return TRUE;								
		}
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� �����Ͽ��� ���� ȣ��Ǵ� �Լ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltSOShip::Arrive()
{	
	SI32		i;
	SOVillage	*pVillage;
	SOPlayer	*pPlayer;

	// �ش� ������ �����͸� ��´�.
	pVillage	=	pMyWorld->GetVillageByCode(uiDestinationVillageCode);

	// ���� ž���� �ִ� ��� �°����� ������ �鿩������.
	for(i = 0; i < 	CustomerID.m_NowUseDataCount; i++)
	{	
		// ���� ž���� �ִ� �°��̴�.
		pPlayer	=	pMyWorld->pPlayerOnAccount[CustomerID.m_Data[i]];

		// �ش� ������ �鿩������.
		if(pPlayer != NULL)		
			pPlayer->ArriveVillageByShip(pVillage);		
	}	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�踦 ź ���¿��� ����ڰ� �α׾ƿ��� �ߴ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltSOShip::LogOutPlayer(UI16 uiAccount, SI32 siDBAccount)
{	
	// �÷��̾ �����Ѵ�.
	CustomerID.DeleteType(uiAccount);
	CustomerDBAccount.DeleteType(siDBAccount);		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ���´�.
//	���� ����	:	2002-08-15 ���� 1:11:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltSOShip::GetInfo(OnCharShipInfo *pShipInfo)
{	
	BaseChar::GetInfo((OnCharBaseInfo*)pShipInfo);
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�¼��� ������� ������ ���´�.
//	���� ����	:	2002-07-27 ���� 7:02:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

