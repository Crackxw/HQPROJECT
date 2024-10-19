#include "SOMain.h"
#include "SOWorld.h"
#include "Game.h"
#include "Element.h"
#include "Battle.h"
#include "BattleMgr.h"
#include "SOBaseChar.h"
#include "GSCDefine.h"



SOWorld		*BaseChar::pMyWorld	    = NULL;
CBattleMgr  *BaseChar::m_pBattleMgr = NULL;



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BaseChar::BaseChar()
{
	Init();	
	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BaseChar::~BaseChar()
{
	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ƽ ������ �ʱ�ȭ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld	 = pWorld;

	m_pBattleMgr =  &pMyWorld->m_BattleMgr;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȿ�� ĳ������ id ���� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::IsValidID(UI16 uiID)
{
	if(ISPLAYER(uiID) == TRUE)
	{	
		if(pMyWorld->pPlayerOnAccount[uiID] != NULL)	return	TRUE;
		else											return	FALSE;
	}
	else if(ISMONSTER(uiID) == TRUE)
	{
		return pMyWorld->clMonster.IsValidID(uiID);
	}
	else if(ISSHIP(uiID) == TRUE)
	{
		return pMyWorld->clSOShip.IsValidID(uiID);
	}

	return FALSE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ ���� ��ġ�� �ʿ� �����Ѵ�.
//					���� �����Ϸ��� ��ġ�� �ݵ�� ������̿��� �Ѵ�.
//	���� ����	:	2002-06-03 ���� 1:32:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::SetIDOnMap( SI32 siParam1 )
{
	// ĳ���Ͱ� �̵����� ���� ���·� �д�.
	bDidSendMoveInfo	=	FALSE;

	if(pMyWorld->clISOMap.SetChar(this, uiMoveableTileInfo) == FALSE)
	{
		// ĳ���͸� �����ϴµ� �����ߴ�.
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap.txt", "Parameter : [ %d ], siX : [ %d ], siY : [ %d ]", siParam1, siX, siY );
		writeInfoToFile("SetIDOnMap.txt", "Parameter : [ %d ], siX : [ %d ], siY : [ %d ]", siParam1, siX, siY );
		return	FALSE;
	}	

	return	TRUE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʿ��� ID�� �����Ѵ�.
//	���� ����	:	2002-07-01 ���� 3:34:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::DelIDOnMap( SI32 siParam1 )
{
	// ĳ���Ͱ� �̵����� ���� ���·� �д�.
	bDidSendMoveInfo	=	FALSE;

	// �켱 �� ��ġ�� ���� �ִ��� Ȯ���Ѵ�.	
	if(pMyWorld->clISOMap.DelChar(this) == FALSE)
	{
		// �ʿ��� ĳ���͸� �����ϴµ� �����ߴ�.
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap.txt", "Parameter : [ %d ]", siParam1 );
		writeInfoToFile("DelIDOnMap.txt", "Parameter : [ %d ]", siParam1 );
		return	FALSE;
	}	

	return	TRUE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ID�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
CHAR*	BaseChar::GetID()
{
	return szID;
}



//�Ź���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	OnlineReceivedPlayerInfo�� �迭 ����ü�� puiCharIDs�� �ִ� ĳ���͵��� ������ ���ͼ� �־��ش�.
//	���� ����	:	2002-11-28 ���� 10:19:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	BaseChar::MakeOnReceivedPlayerInfoPacket(CHAR *pBuffer, SI32 siPlayerNum, UI16 *puiCharIDs, UI16 *puiRealPlayerNum, SI32 siParam1 )
{
	SI32						i;	
	SI32						siTotalLength;
	SI32						siLength;
	SOPlayer					*pPlayer;
	UI16						uiRealPlayerNum;

	siTotalLength = siLength	=	0;
	uiRealPlayerNum				=	0;

	for(i = 0; i < siPlayerNum; i++)
	{
		siLength	=	0;

		if(IsValidID(puiCharIDs[i]) == TRUE)
		{
			if(ISMONSTER(puiCharIDs[i]) == TRUE)
			{	
				pMyWorld->clMonster.GetInfo(puiCharIDs[i], (OnCharMonsterInfo*)pBuffer);				

				// ������ ��쿡�� �߰� ������ ����.
				siLength		=	sizeof(OnCharMonsterInfo);
			}
			else if(ISPLAYER(puiCharIDs[i]) == TRUE)
			{
				pPlayer	=	pMyWorld->pPlayerOnAccount[puiCharIDs[i]];

				if(pPlayer)	
				{
					siLength		=	pPlayer->GetInfo((OnCharPlayerInfo*)pBuffer);
				}
			}					
			else if(ISSHIP(puiCharIDs[i]) == TRUE)
			{
				pMyWorld->clSOShip.GetInfo(puiCharIDs[i], (OnCharShipInfo*)pBuffer);				

				// ���� ��쿡�� �߰� ������ ����.
				siLength		=	sizeof(OnCharShipInfo);				
			}			

			{
				OnCharBaseInfo		*pCharBaseInfo;

				pCharBaseInfo	=	( OnCharBaseInfo* )pBuffer;

				if( pCharBaseInfo->uiKind == 0 )
				{
//					pMyWorld->clServer->WriteInfo("..\\ZeroKind.txt", "ZeroKind : Account : [ %u, %u ], siX : [ %d ], siY : [ %d ], siParam1 : [ %d ]", 
					writeInfoToFile("ZeroKind.txt", "ZeroKind : Account : [ %u, %u ], siX : [ %d ], siY : [ %d ], siParam1 : [ %d ]", 
									puiCharIDs[ i ], pCharBaseInfo->uiAccount, pCharBaseInfo->siX, pCharBaseInfo->siY, siParam1 );
				}
			}

			if( siLength != 0 )
			{
				pBuffer			+=	siLength;
				siTotalLength	+=	siLength;

				// ���� �޽����� �޴� �÷��̾���� ���� ��Ų��.
				uiRealPlayerNum++;
			}			

		}
		else
		{
// 			pMyWorld->clServer->WriteInfo("..\\Invalid ID.txt", "Invalid ID :: [%u], siParam : [ %d ], siPlayerNum : [ %d ]", puiCharIDs[i], siParam1, siPlayerNum );
 			writeInfoToFile("Invalid ID.txt", "Invalid ID :: [%u], siParam : [ %d ], siPlayerNum : [ %d ]", puiCharIDs[i], siParam1, siPlayerNum );
		}
	}

	if(puiRealPlayerNum != NULL)
	{
		// ���� ��ȿ�ߴ� �÷��̾� ���� �����Ѵ�.
		*puiRealPlayerNum	=	uiRealPlayerNum;
	}

	return siTotalLength;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�⺻ ĳ������ ������ ���´�.
//	���� ����	:	2002-08-15 ���� 12:55:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::GetInfo(OnCharBaseInfo *pCharBaseInfo)
{	
	pCharBaseInfo->siX				= siX;
	pCharBaseInfo->siY	       		= siY;
	pCharBaseInfo->uiKind			= uiKind;
	pCharBaseInfo->uiAccount	    = uiAccount;	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ�� �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::Init(UI16 uiaccount, UI16 uimapid, SI16 six, SI16 siy, UI16 uikind, CHAR *pszid)
{
	uiAccount			=	uiaccount;
	uiMapID				=	uimapid;
	siX					=	six;
	siY					=	siy;
	uiKind				=	uikind;
	m_battleKey			=	-1;
	bDidSendMoveInfo	=	FALSE;

	SetAttackMode( FALSE );
	
	if(ISPLAYER(uiAccount) == TRUE)
	{		
		uiMoveableTileInfo	=	ON_TILE_KIND_GROUND;
	}
	else if(ISMONSTER(uiAccount) == TRUE)
	{
		uiMoveableTileInfo	=	ON_TILE_KIND_GROUND;
	}	
	else if(ISSHIP(uiAccount) == TRUE)
	{
		uiMoveableTileInfo	=	ON_TILE_KIND_SEA;
	}

	if(pszid != NULL)
		strncpy(szID, pszid, ON_ID_LENGTH);
	else
		ZeroMemory(szID, sizeof(szID));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ�� �Ѵ�.
//	���� ����	: 2002-02-19 ���� 11:47:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::Init()
{
	SETSTATUS( siStatus, 0);
	siX					=	0;
	siY					=	0;	
	uiKind				=	0;	
	uiAccount			=	0;
	uiMapID				=	0;	
	uiMoveableTileInfo	=	0;
	m_battleKey			=  -1;

	SetAttackMode( FALSE );

	ZeroMemory(szID, sizeof(szID));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� ���� ��Ų��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	POINT		ptWarpXY;					// ������ ����

	// ���� �ݵ�� �ʵ� �� �־�� �Ѵ�.
	//if(siStatus	!= ON_PLAYER_STATUS_INFIELD)	return FALSE;
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;

	ptWarpXY	=	pMyWorld->clISOMap.SearchMap(uimapid, six, siy, ON_MAP_SI_CHAR, uiMoveableTileInfo);

	// ������ ���� ������ �ڸ��� ã�� ���ؼ� �����ߴ�.				
	if((ptWarpXY.x == -1) && (ptWarpXY.y == -1))	return FALSE;

	// ���� ���� ��ġ���� �� ID�� �����.
	if(DelIDOnMap( 1 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "3 [%u]", uiAccount);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "3 [%u]", uiAccount);


	uiMapID		=	uimapid;
	siX			=	SI16(ptWarpXY.x);
	siY			=	SI16(ptWarpXY.y);	

	// ���� ���ο� ��ġ�� �� ID�� �����Ѵ�.
	if(SetIDOnMap( 2 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "2");
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "2");

	return TRUE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	BaseChar::GetKind()
{	
	return uiKind;
}	



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ �ʵ忡���� ��ǥ�� ���´�.
//	���� ����	:	2002-04-15 ���� 12:53:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::GetPos(UI16 *puiMapID, LPPOINT pPoint)
{
	*puiMapID	=	uiMapID;
	pPoint->x	=	siX;
	pPoint->y	=	siY;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ĳ������ ����(�ʵ�, ����, ������)�� ���´�.
//	���� ����	:	2002-05-02 ���� 10:25:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	BaseChar::GetStatus()
{
	return siStatus;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ����
//	���� ����	:	2002-06-20 ���� 2:18:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	BaseChar::GetFightingPower(void)
{
	SI32		TempCount;
	SI32		TempFightingPower;

	TempFightingPower = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IFollower.IsValidFollower(TempCount) == TRUE)
		{		
			TempFightingPower += IFollower.GetFollowerParameterBattlePower(TempCount);
		}
	}

	if(TempFightingPower == 0)
	{
		TempFightingPower = 0;
	}
	return TempFightingPower;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� ĳ�����ΰ�?
//	���� ����	:	2002-06-29 ���� 10:14:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::IsValid()
{
	if(uiAccount != 0)	return	TRUE;
	else				return	FALSE;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����۸���Ʈ�� �����ش�.
//	���� ����	:	2002-07-06 ���� 8:12:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData*		BaseChar::GetItemList(UI08 uiFollowerSlot)
{
	// ĳ���Ͱ� ��ȿ���� ������ NULL�� �����Ѵ�.
	if(IsValid() == FALSE)	return	NULL;
	
	return	IFollower.GetItemList(uiFollowerSlot);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� �߰����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData* BaseChar::AddItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory ,BOOL bSendToDB)
{
	MyItemData			ItemData, *pMyItemData;		
	DBIKPlayerItemInfo	DBPlayerItemInfo;
	UI08				uiRet;	
	SOPlayer			*pPlayer;

	ItemData.uiID				=	uiID;
	ItemData.uiQuantity			=	uiQuantity;
	ItemData.siPosInInventory	=	siPosInInventory;

	uiRet						=	IFollower.AddItem(uiFollowerID, &ItemData);
	switch(uiRet)
	{
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD:
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE:		
		pMyItemData									=	IFollower.GetItem(uiFollowerID, uiID, siPosInInventory);

		// �÷��̾��� ��쿡�� DB�� �����Ѵ�.
		if( ISPLAYER( uiAccount ) == TRUE )
		{		
			pPlayer	=	(SOPlayer*)this;

			DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;
			DBPlayerItemInfo.uiItemPosInInventory	=	pMyItemData->siPosInInventory;
			DBPlayerItemInfo.uiNewQuantity			=	pMyItemData->uiQuantity;
			DBPlayerItemInfo.uiNewPosInInventory	=	pMyItemData->siPosInInventory;

			if (bSendToDB) {				
				if(uiRet == ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD)		
				{			
					//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
				}
				else		
				{		
					//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDITEM, LPVOID(&DBPlayerItemInfo), NULL);		
					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_APPENDITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
				}
			}
		}

		return pMyItemData;
	}

	return NULL;
}	

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� �߰����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData* BaseChar::AddItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory )
{
	MyItemData			ItemData, *pMyItemData;			
	UI08				uiRet;	
	SOPlayer			*pPlayer;

	ItemData.uiID				=	uiID;
	ItemData.uiQuantity			=	uiQuantity;
	ItemData.siPosInInventory	=	siPosInInventory;

	uiRet						=	IFollower.AddItem(uiFollowerID, &ItemData);
	switch(uiRet)
	{
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD:
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE:		
		pMyItemData									=	IFollower.GetItem(uiFollowerID, uiID, ItemData.siPosInInventory);

		// �÷��̾��� ��쿡�� DB�� �����Ѵ�.
		if( ISPLAYER( uiAccount ) == TRUE )
		{		
			pPlayer	=	(SOPlayer*)this;

			/*
			DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;
			DBPlayerItemInfo.uiItemPosInInventory	=	pMyItemData->siPosInInventory;
			DBPlayerItemInfo.uiNewQuantity			=	pMyItemData->uiQuantity;
			DBPlayerItemInfo.uiNewPosInInventory	=	pMyItemData->siPosInInventory;
			
			if(uiRet == ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD)		
			{			
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
			}
			else		
			{		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_APPENDITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
			}
			*/
		}

		return pMyItemData;
	}

	return NULL;
}	



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰����ش�.
//	���� ����	:	2002-07-07 ���� 11:05:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData*		BaseChar::AddItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity )
{
	MyItemData			*pMyItemData;		
	DBIKPlayerItemInfo	DBPlayerItemInfo;
	UI08				uiRet;	
	SOPlayer			*pPlayer;
	SI16				siPosInInventory;
	
	uiRet				=	IFollower.AddItem(uiFollowerID, uiID, uiQuantity, &siPosInInventory);	

	switch(uiRet)
	{
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD:
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE:		
		pMyItemData									=	IFollower.GetItem(uiFollowerID, uiID, siPosInInventory);

		// �÷��̾��� ��쿡�� DB�� �����Ѵ�.
		if( ISPLAYER( uiAccount ) == TRUE )
		{		
			pPlayer	=	(SOPlayer*)this;

			DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;
			DBPlayerItemInfo.uiItemPosInInventory	=	pMyItemData->siPosInInventory;
			DBPlayerItemInfo.uiNewQuantity			=	pMyItemData->uiQuantity;
			DBPlayerItemInfo.uiNewPosInInventory	=	pMyItemData->siPosInInventory;
	
			if(uiRet == ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD)		
			{			
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo) );		
			}
			else		
			{		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_APPENDITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo) );		
			}
		}

		return pMyItemData;
	}

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰����ش�.
//	���� ����	:	2002-07-07 ���� 11:05:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData*		BaseChar::AddItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity )
{
	MyItemData			*pMyItemData;		
	UI08				uiRet;	
	SOPlayer			*pPlayer;
	SI16				siPosInInventory;
	
	uiRet				=	IFollower.AddItem(uiFollowerID, uiID, uiQuantity, &siPosInInventory);	

	switch(uiRet)
	{
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD:
	case ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE:		
		pMyItemData									=	IFollower.GetItem(uiFollowerID, uiID, siPosInInventory);

		// �÷��̾��� ��쿡�� DB�� �����Ѵ�.
		if( ISPLAYER( uiAccount ) == TRUE )
		{		
			pPlayer	=	(SOPlayer*)this;

			/*DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;
			DBPlayerItemInfo.uiItemPosInInventory	=	pMyItemData->siPosInInventory;
			DBPlayerItemInfo.uiNewQuantity			=	pMyItemData->uiQuantity;
			DBPlayerItemInfo.uiNewPosInInventory	=	pMyItemData->siPosInInventory;
	
			if(uiRet == ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD)		
			{			
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo) );		
			}
			else		
			{		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_APPENDITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_APPENDITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo) );		
			}*/
		}

		return pMyItemData;
	}

	return NULL;
}




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-07-13 ���� 10:01:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData* BaseChar::DelItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory )
{	
	MyItemData			ItemData, *pMyItemData = NULL;		
	DBIKPlayerItemInfo	DBPlayerItemInfo;
	UI08				uiRet;	
	SOPlayer			*pPlayer;

	ItemData.uiID				=	uiID;
	ItemData.uiQuantity			=	uiQuantity;
	ItemData.siPosInInventory	=	siPosInInventory;

	pMyItemData					=	IFollower.GetItemByID(uiFollowerID, uiID);

	uiRet						=	IFollower.DelItem(uiFollowerID, &ItemData);
	switch(uiRet)
	{
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL:
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL:	

		if( ISPLAYER( uiAccount ) == TRUE )
		{
			pPlayer	=	(SOPlayer*)this;		

			if(uiRet == ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL)
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// �������� �����ϱ� ���ؼ��� DB���� �˻縦 �ϱ� ���Ͽ� ���� ������ ���� �־���� �Ѵ�
			else					
				DBPlayerItemInfo.uiNewQuantity		=	pMyItemData->uiQuantity;
			

			DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;	
			DBPlayerItemInfo.uiNewPosInInventory	=	ItemData.siPosInInventory;		
			DBPlayerItemInfo.uiItemPosInInventory	=	ItemData.siPosInInventory;
			

			
				if(uiRet == ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL)		
					//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
				
				else		
					//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_DELETEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
			
		}

		return pMyItemData;
	}

	return NULL;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-07-13 ���� 10:01:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData* BaseChar::DelItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory )
{	
	MyItemData			ItemData, *pMyItemData = NULL;		
	UI08				uiRet;	
	SOPlayer			*pPlayer;

	ItemData.uiID				=	uiID;
	ItemData.uiQuantity			=	uiQuantity;
	ItemData.siPosInInventory	=	siPosInInventory;

	pMyItemData					=	IFollower.GetItemByID(uiFollowerID, uiID);

	uiRet						=	IFollower.DelItem(uiFollowerID, &ItemData);
	switch(uiRet)
	{
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL:
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL:	

		if( ISPLAYER( uiAccount ) == TRUE )
		{
			pPlayer	=	(SOPlayer*)this;		

			/*
			if(uiRet == ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL)
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// �������� �����ϱ� ���ؼ��� DB���� �˻縦 �ϱ� ���Ͽ� ���� ������ ���� �־���� �Ѵ�
			else					
				DBPlayerItemInfo.uiNewQuantity		=	pMyItemData->uiQuantity;
			
			DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;
			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;	
			DBPlayerItemInfo.uiNewPosInInventory	=	ItemData.siPosInInventory;		
			DBPlayerItemInfo.uiItemPosInInventory	=	ItemData.siPosInInventory;
			
			if(uiRet == ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL)		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		

			else		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_DELETEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));		
				*/
		}

		return pMyItemData;
	}

	return NULL;	
}




MyItemData*		BaseChar::DelItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity )
{
	MyItemData			*pMyItemData = NULL;		
	UI08				uiRet;	
	SOPlayer			*pPlayer;
	SI16				siPosInInventory;

	pMyItemData			=	IFollower.GetItemByID(uiFollowerID, uiID);

	uiRet				=	IFollower.DelItem(uiFollowerID, uiID, uiQuantity, &siPosInInventory);
	switch(uiRet)
	{
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE:	
		return NULL;
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL:
	case ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL:			
		if( ISPLAYER( uiAccount ) == TRUE )
		{
			pPlayer	=	(SOPlayer*)this;

			/*if(uiRet == ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL)
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// �������� �����ϱ� ���ؼ��� DB���� �˻縦 �ϱ� ���Ͽ� ���� ������ ���� �־���� �Ѵ�
			else					
				DBPlayerItemInfo.uiNewQuantity		=	pMyItemData->uiQuantity;
			

			DBPlayerItemInfo.siDBAccount			=	pPlayer->stAccountInfo.siDBAccount;
			DBPlayerItemInfo.uiSlot					=	pPlayer->stAccountInfo.uiSlot;

			DBPlayerItemInfo.uiFollowerID			=	uiFollowerID;
			DBPlayerItemInfo.uiItemID				=	uiID;	
			DBPlayerItemInfo.uiNewPosInInventory	=	siPosInInventory;		
			DBPlayerItemInfo.uiItemPosInInventory	=	siPosInInventory;
			
			if(uiRet == ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL)		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_UPDATEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_UPDATEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));	
			else		
				//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_DELETEITEM, LPVOID(&DBPlayerItemInfo), NULL);		
				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEITEM, &DBPlayerItemInfo, sizeof(DBPlayerItemInfo));	*/
		}

		return pMyItemData;
	}

	return NULL;	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �ִ��� �˻��ϰ� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void	BaseChar::FindDeleteItem(UI16 uiItemID, SI32 siCount, SI16* lpDelCount, MercenaryItemData* lpMercenaryItemData)
{
	SI32				TempCount;
	SI32				TempCount1;
	SI32				TempCount2;
	OnlineFollower*		lpOnlineFollower;
	
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IFollower.IsValidFollower(TempCount))
		{
			// Invetory Item
			for(TempCount1 = 0; TempCount1 < ON_MYDATA_MAX_ITEM_NUM; TempCount1++)
			{
				lpOnlineFollower = IFollower.GetFollower(TempCount);
				if(lpOnlineFollower->Items[TempCount1].uiID == uiItemID)
				{
					// ã���� �����Ѵ�.
					TempCount2 = lpOnlineFollower->Items[TempCount1].uiQuantity;
					if(TempCount2 > siCount) TempCount2 = siCount;
					
					lpMercenaryItemData[(*lpDelCount)].siSlot = TempCount;
					lpMercenaryItemData[(*lpDelCount)].siFlag = 0;
					lpMercenaryItemData[(*lpDelCount)].siInventoryItemInfo.uiID             = lpOnlineFollower->Items[TempCount1].uiID;
					lpMercenaryItemData[(*lpDelCount)].siInventoryItemInfo.siPosInInventory = lpOnlineFollower->Items[TempCount1].siPosInInventory;
					lpMercenaryItemData[(*lpDelCount)].siInventoryItemInfo.uiQuantity       = TempCount2;
					(*lpDelCount)++;

					DelItem(TempCount, lpOnlineFollower->Items[TempCount1].uiID, TempCount2, lpOnlineFollower->Items[TempCount1].siPosInInventory);

					siCount -= TempCount2;

					if(siCount <= 0) return;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �ִ��� �˻��ϰ� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void	BaseChar::FindDeleteItemNoQuery(UI16 uiItemID, SI32 siCount, SI16* lpDelCount, MercenaryItemData* lpMercenaryItemData)
{
	SI32				TempCount;
	SI32				TempCount1;
	SI32				TempCount2;
	OnlineFollower*		lpOnlineFollower;
	
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IFollower.IsValidFollower(TempCount))
		{
			// Invetory Item
			for(TempCount1 = 0; TempCount1 < ON_MYDATA_MAX_ITEM_NUM; TempCount1++)
			{
				lpOnlineFollower = IFollower.GetFollower(TempCount);
				if(lpOnlineFollower->Items[TempCount1].uiID == uiItemID)
				{
					// ã���� �����Ѵ�.
					TempCount2 = lpOnlineFollower->Items[TempCount1].uiQuantity;
					if(TempCount2 > siCount) TempCount2 = siCount;
					
					lpMercenaryItemData[(*lpDelCount)].siSlot = TempCount;
					lpMercenaryItemData[(*lpDelCount)].siFlag = 0;
					lpMercenaryItemData[(*lpDelCount)].siInventoryItemInfo.uiID             = lpOnlineFollower->Items[TempCount1].uiID;
					lpMercenaryItemData[(*lpDelCount)].siInventoryItemInfo.siPosInInventory = lpOnlineFollower->Items[TempCount1].siPosInInventory;
					lpMercenaryItemData[(*lpDelCount)].siInventoryItemInfo.uiQuantity       = TempCount2;
					(*lpDelCount)++;

					DelItemNoQuery(TempCount, lpOnlineFollower->Items[TempCount1].uiID, TempCount2, lpOnlineFollower->Items[TempCount1].siPosInInventory);

					siCount -= TempCount2;

					if(siCount <= 0) return;
				}
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ���´�.
//	���� ����	:	2002-10-23 ���� 9:56:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData*	BaseChar::GetItem(UI08 uiFollowerID, UI16 uiID)
{
	return IFollower.GetItemByID( uiFollowerID, uiID );	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰��� �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-07 ���� 4:36:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::CanAddItem(UI08 uiFollowerID, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum)
{
	return	IFollower.CanAddItem(uiFollowerID, puiID, puiQuantity, uiItemNum);
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �뺴�� ������ ���´�.
//	���� ����	:	2002-07-07 ���� 1:41:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	BaseChar::GetAllFollowerInfo(OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum)
{
	SI32		i;
	DWORD		dwFollowerSize;
	DWORD		dwAllFollowerSize;

	dwAllFollowerSize	=	0;
	*puiFollowerNum		=	0;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IFollower.IsValidFollower(i) == TRUE)
		{
			// ��ȿ�� �뺴�̴�.
			dwFollowerSize			=	GetFollowerInfo(i, pFollowerInfo);

			dwAllFollowerSize		+=	dwFollowerSize;
			pFollowerInfo			=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerSize);
	
			// �뺴���� ������Ų��.
			(*puiFollowerNum)++;
		}
	}

	return	dwAllFollowerSize;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� ������ ���´�.
//	���� ����	:	2002-07-07 ���� 1:26:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	BaseChar::GetFollowerInfo(UI08 uiFollowerID, OnFollowerInfo *pFollowerInfo)
{	
	OnlineReceivedMyItemInfo	*pItemInfo;
	SI32						i;
	OnlineFollower				*pFollower;
	MyItemData					ItemData;

	// ������ �뺴�� �ƴ϶�� 0�� �����Ѵ�.
	if(IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	0;
	
	pFollowerInfo->uiSlot				=	uiFollowerID;				
	pFollowerInfo->uiKind				=	IFollower.GetFollowerCharID(uiFollowerID);				
	pFollowerInfo->siLevel				=	IFollower.GetFollowerParameterLevel(uiFollowerID);				
	pFollowerInfo->siExp				=	IFollower.GetFollowerBaseParameterCurExp(uiFollowerID);				// ����ġ.
	pFollowerInfo->siStr				=	IFollower.GetFollowerBaseParameterSTR(uiFollowerID);				// ��.
	pFollowerInfo->siDex				=	IFollower.GetFollowerBaseParameterDEX(uiFollowerID);				// Dex
	pFollowerInfo->siVit				=	IFollower.GetFollowerBaseParameterVIT(uiFollowerID);				// Vit
	pFollowerInfo->siInt				=	IFollower.GetFollowerBaseParameterINT(uiFollowerID);				// Int
	pFollowerInfo->siLife				=	IFollower.GetFollowerParameterLife(uiFollowerID);					// Life(HP)
	pFollowerInfo->siMana				=	IFollower.GetFollowerParameterMana(uiFollowerID);					// Mana(MP)			
	pFollowerInfo->siBonus				=	IFollower.GetFollowerBaseParameterBonus(uiFollowerID);				// Bonus
	pFollowerInfo->siLiftConstant       =   IFollower.GetFollowerBaseParameterLiftConstance(uiFollowerID);
	pFollowerInfo->siEquipMinDamage		=	IFollower.GetFollowerBaseParameterEquipMinDamage(uiFollowerID);		// EquipMinDamage;
	pFollowerInfo->siEquipMaxDamage		=	IFollower.GetFollowerBaseParameterEquipMaxDamage(uiFollowerID);		// EqipMaxDamage;
	pFollowerInfo->siAC					=	IFollower.GetFollowerBaseParameterAC(uiFollowerID);					// AC
	pFollowerInfo->siDamageRegist		=	IFollower.GetFollowerBaseParameterDamageResist(uiFollowerID);		// DamageRegist;
	pFollowerInfo->siMagicRegist		=	IFollower.GetFollowerBaseParameterMagicResist(uiFollowerID);		// MagicRegist;

	ZeroMemory(pFollowerInfo->Name, sizeof(pFollowerInfo->Name));
	strncpy(pFollowerInfo->Name, IFollower.GetFollowerName(uiFollowerID), ON_ID_LENGTH);						// Name;

	pFollower	=	IFollower.GetFollower(uiFollowerID);
	for(i = 0; i < WEAR_ITEM_NUM; i++)	
	{
		pFollowerInfo->stWearItemInfo[i].uiItemID	 =	pFollower->Weapon.GetWearItem(i);
		pFollowerInfo->stWearItemInfo[i].siLimitTime =  pFollower->Weapon.GetWearItemLimitTime(i);
	}
		//pFollowerInfo->uiWearItem[i]	=	pFollower->Weapon.GetWearItem(i);	
		
	pItemInfo								=	(OnlineReceivedMyItemInfo*)((CHAR*)pFollowerInfo + sizeof(OnFollowerInfo));
	
	// �������� ������ ���´�.
	pFollowerInfo->uiItemNum			=	0;
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(IFollower.GetItem(uiFollowerID, i, &ItemData) == TRUE)
		{
			pItemInfo[pFollowerInfo->uiItemNum].uiID				=	ItemData.uiID;				// �������ڵ�
			pItemInfo[pFollowerInfo->uiItemNum].uiQuantity			=	ItemData.uiQuantity;			// �������� ����
			pItemInfo[pFollowerInfo->uiItemNum].siPosInInventory	=	ItemData.siPosInInventory;	// �������� ��ġ
			pFollowerInfo->uiItemNum++;
		}							
	}

	return	DWORD(sizeof(OnFollowerInfo) + (sizeof(OnlineReceivedMyItemInfo) * pFollowerInfo->uiItemNum));
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� �����Ѵ�.
//	���� ����	:	2002-07-08 ���� 12:16:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::DeleteFollower(UI08 uiFollowerID)
{
	if(IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	FALSE;

	// �뺴�� �����Ѵ�.
	IFollower.DeleteFollower(uiFollowerID);

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�̵��ߴٰ� �����Ѵ�.
//	���� ����	:	2002-07-26 ���� 4:02:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	BaseChar::SetMoved()
{
	bDidSendMoveInfo	=	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�̵� �����ÿ� �̵� �޽����� ������ �÷��̾�鿡�� ���´°� �˾ƿ´�.
//	���� ����	:	2002-07-26 ���� 4:02:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	BaseChar::DidSendMoveInfo()
{
	if(bDidSendMoveInfo == TRUE)
	{
		bDidSendMoveInfo = FALSE;
		return	TRUE;		
	}
	
	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ش�.
//	���� ����	:	2002-09-30 ���� 5:28:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	BaseChar::GiveItem(	UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, BaseChar *pOppBaseChar, UI08 uiOppFollowerID, SI16 *psiInvenPos, SI16 *psiOppInvenPos )
{
	// ���� ĳ���Ͱ� �������� ���� �� �ִ��� �˻��Ѵ�.
	if( pOppBaseChar->IFollower.CanAddItem( uiOppFollowerID, uiItemID, uiItemQuantity ) == FALSE )								
		return	FALSE;

	// �� �뺴���Լ� �������� ����.
	if( IFollower.DelItem( uiFollowerID, uiItemID, uiItemQuantity, psiInvenPos ) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	
		return	FALSE;

	// ���� �뺴���� �������� �ش�.
	if( pOppBaseChar->IFollower.AddItem( uiOppFollowerID, uiItemID, uiItemQuantity, psiOppInvenPos ) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	
		return	FALSE;


	return	TRUE;
}

BOOL  BaseChar::HasItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory )
{	
	MyItemData			*pMyItemData = NULL;		
	
	if( IFollower.GetItemByPos(uiFollowerID, siPosInInventory, &pMyItemData) == FALSE )		return FALSE;

	if( pMyItemData )
	{
		if( pMyItemData->uiID == uiID )
		{
			if( pMyItemData->uiQuantity >= uiQuantity )
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;	
}

VOID	BaseChar::SetAttackMode( BOOL flag )
{
	m_bAttackMode	=	flag;
}

BOOL	BaseChar::GetAttackMode()
{
	return	m_bAttackMode;
}