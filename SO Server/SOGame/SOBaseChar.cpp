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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BaseChar::BaseChar()
{
	Init();	
	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BaseChar::~BaseChar()
{
	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	스태틱 변수를 초기화 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	BaseChar::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld	 = pWorld;

	m_pBattleMgr =  &pMyWorld->m_BattleMgr;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	유효한 캐릭터의 id 인지 본다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 현재 위치를 맵에 설정한다.
//					현재 설정하려는 위치는 반드시 빈공간이여야 한다.
//	수정 일자	:	2002-06-03 오후 1:32:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::SetIDOnMap( SI32 siParam1 )
{
	// 캐릭터가 이동하지 않은 상태로 둔다.
	bDidSendMoveInfo	=	FALSE;

	if(pMyWorld->clISOMap.SetChar(this, uiMoveableTileInfo) == FALSE)
	{
		// 캐릭터를 설정하는데 실패했다.
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap.txt", "Parameter : [ %d ], siX : [ %d ], siY : [ %d ]", siParam1, siX, siY );
		writeInfoToFile("SetIDOnMap.txt", "Parameter : [ %d ], siX : [ %d ], siY : [ %d ]", siParam1, siX, siY );
		return	FALSE;
	}	

	return	TRUE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	맵에서 ID를 삭제한다.
//	수정 일자	:	2002-07-01 오후 3:34:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::DelIDOnMap( SI32 siParam1 )
{
	// 캐릭터가 이동하지 않은 상태로 둔다.
	bDidSendMoveInfo	=	FALSE;

	// 우선 그 위치에 내가 있는지 확인한다.	
	if(pMyWorld->clISOMap.DelChar(this) == FALSE)
	{
		// 맵에서 캐릭터를 삭제하는데 실패했다.
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap.txt", "Parameter : [ %d ]", siParam1 );
		writeInfoToFile("DelIDOnMap.txt", "Parameter : [ %d ]", siParam1 );
		return	FALSE;
	}	

	return	TRUE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	ID를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	BaseChar::GetID()
{
	return szID;
}



//신버젼
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	OnlineReceivedPlayerInfo의 배열 구조체에 puiCharIDs에 있는 캐릭터들의 정보를 얻어와서 넣어준다.
//	수정 일자	:	2002-11-28 오전 10:19:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

				// 몬스터일 경우에는 추가 정보가 없다.
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

				// 배의 경우에는 추가 정보가 없다.
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

				// 실제 메시지를 받는 플레이어수를 증가 시킨다.
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
		// 실제 유효했던 플레이어 수를 지정한다.
		*puiRealPlayerNum	=	uiRealPlayerNum;
	}

	return siTotalLength;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	기본 캐릭터의 정보를 얻어온다.
//	수정 일자	:	2002-08-15 오후 12:55:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	BaseChar::GetInfo(OnCharBaseInfo *pCharBaseInfo)
{	
	pCharBaseInfo->siX				= siX;
	pCharBaseInfo->siY	       		= siY;
	pCharBaseInfo->uiKind			= uiKind;
	pCharBaseInfo->uiAccount	    = uiAccount;	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화를 한다.
//	수정 일자	: 2002-02-19 오전 11:47:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 워프 시킨다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	POINT		ptWarpXY;					// 워프한 지점

	// 현재 반드시 필드 상에 있어야 한다.
	//if(siStatus	!= ON_PLAYER_STATUS_INFIELD)	return FALSE;
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;

	ptWarpXY	=	pMyWorld->clISOMap.SearchMap(uimapid, six, siy, ON_MAP_SI_CHAR, uiMoveableTileInfo);

	// 워프할 곳에 워프할 자리를 찾지 못해서 실패했다.				
	if((ptWarpXY.x == -1) && (ptWarpXY.y == -1))	return FALSE;

	// 맵의 현재 위치에서 내 ID를 지운다.
	if(DelIDOnMap( 1 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "3 [%u]", uiAccount);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "3 [%u]", uiAccount);


	uiMapID		=	uimapid;
	siX			=	SI16(ptWarpXY.x);
	siY			=	SI16(ptWarpXY.y);	

	// 맵의 새로운 위치에 내 ID를 설정한다.
	if(SetIDOnMap( 2 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "2");
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "2");

	return TRUE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 종류를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	BaseChar::GetKind()
{	
	return uiKind;
}	



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 필드에서의 좌표를 얻어온다.
//	수정 일자	:	2002-04-15 오후 12:53:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	BaseChar::GetPos(UI16 *puiMapID, LPPOINT pPoint)
{
	*puiMapID	=	uiMapID;
	pPoint->x	=	siX;
	pPoint->y	=	siY;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	캐릭터의 상태(필드, 마을, 전투중)를 얻어온다.
//	수정 일자	:	2002-05-02 오전 10:25:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	BaseChar::GetStatus()
{
	return siStatus;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투력을 얻음
//	수정 일자	:	2002-06-20 오후 2:18:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 캐릭터인가?
//	수정 일자	:	2002-06-29 오전 10:14:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::IsValid()
{
	if(uiAccount != 0)	return	TRUE;
	else				return	FALSE;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템리스트를 돌려준다.
//	수정 일자	:	2002-07-06 오후 8:12:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MyItemData*		BaseChar::GetItemList(UI08 uiFollowerSlot)
{
	// 캐릭터가 유효하지 않으면 NULL을 리턴한다.
	if(IsValid() == FALSE)	return	NULL;
	
	return	IFollower.GetItemList(uiFollowerSlot);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템을 추가해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

		// 플레이어일 경우에만 DB에 저장한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템을 추가해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

		// 플레이어일 경우에만 DB에 저장한다.
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가해준다.
//	수정 일자	:	2002-07-07 오전 11:05:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

		// 플레이어일 경우에만 DB에 저장한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가해준다.
//	수정 일자	:	2002-07-07 오전 11:05:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

		// 플레이어일 경우에만 DB에 저장한다.
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




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 삭제한다.
//	수정 일자	:	2002-07-13 오후 10:01:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// 아이템을 삭제하기 위해서는 DB에서 검사를 하기 위하여 원래 수량도 같이 넣어줘야 한다
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 삭제한다.
//	수정 일자	:	2002-07-13 오후 10:01:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// 아이템을 삭제하기 위해서는 DB에서 검사를 하기 위하여 원래 수량도 같이 넣어줘야 한다
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
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// 아이템을 삭제하기 위해서는 DB에서 검사를 하기 위하여 원래 수량도 같이 넣어줘야 한다
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
//	설명	:	아이템이 있는지 검사하고 삭제한다.
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
					// 찾으면 삭제한다.
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
//	설명	:	아이템이 있는지 검사하고 삭제한다.
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
					// 찾으면 삭제한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 정보를 얻어온다.
//	수정 일자	:	2002-10-23 오후 9:56:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MyItemData*	BaseChar::GetItem(UI08 uiFollowerID, UI16 uiID)
{
	return IFollower.GetItemByID( uiFollowerID, uiID );	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가할 수 있는지 알아온다.
//	수정 일자	:	2002-07-07 오전 4:36:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::CanAddItem(UI08 uiFollowerID, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum)
{
	return	IFollower.CanAddItem(uiFollowerID, puiID, puiQuantity, uiItemNum);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	모든 용병의 정보를 얻어온다.
//	수정 일자	:	2002-07-07 오후 1:41:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
			// 유효한 용병이다.
			dwFollowerSize			=	GetFollowerInfo(i, pFollowerInfo);

			dwAllFollowerSize		+=	dwFollowerSize;
			pFollowerInfo			=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerSize);
	
			// 용병수를 증가시킨다.
			(*puiFollowerNum)++;
		}
	}

	return	dwAllFollowerSize;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병의 정보를 얻어온다.
//	수정 일자	:	2002-07-07 오후 1:26:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	BaseChar::GetFollowerInfo(UI08 uiFollowerID, OnFollowerInfo *pFollowerInfo)
{	
	OnlineReceivedMyItemInfo	*pItemInfo;
	SI32						i;
	OnlineFollower				*pFollower;
	MyItemData					ItemData;

	// 유용한 용병이 아니라면 0을 리턴한다.
	if(IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	0;
	
	pFollowerInfo->uiSlot				=	uiFollowerID;				
	pFollowerInfo->uiKind				=	IFollower.GetFollowerCharID(uiFollowerID);				
	pFollowerInfo->siLevel				=	IFollower.GetFollowerParameterLevel(uiFollowerID);				
	pFollowerInfo->siExp				=	IFollower.GetFollowerBaseParameterCurExp(uiFollowerID);				// 경험치.
	pFollowerInfo->siStr				=	IFollower.GetFollowerBaseParameterSTR(uiFollowerID);				// 힘.
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
	
	// 아이템의 정보를 얻어온다.
	pFollowerInfo->uiItemNum			=	0;
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(IFollower.GetItem(uiFollowerID, i, &ItemData) == TRUE)
		{
			pItemInfo[pFollowerInfo->uiItemNum].uiID				=	ItemData.uiID;				// 아이템코드
			pItemInfo[pFollowerInfo->uiItemNum].uiQuantity			=	ItemData.uiQuantity;			// 아이템의 수량
			pItemInfo[pFollowerInfo->uiItemNum].siPosInInventory	=	ItemData.siPosInInventory;	// 아이템의 위치
			pFollowerInfo->uiItemNum++;
		}							
	}

	return	DWORD(sizeof(OnFollowerInfo) + (sizeof(OnlineReceivedMyItemInfo) * pFollowerInfo->uiItemNum));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병을 삭제한다.
//	수정 일자	:	2002-07-08 오전 12:16:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::DeleteFollower(UI08 uiFollowerID)
{
	if(IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	FALSE;

	// 용병을 삭제한다.
	IFollower.DeleteFollower(uiFollowerID);

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이동했다고 설정한다.
//	수정 일자	:	2002-07-26 오후 4:02:17 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	BaseChar::SetMoved()
{
	bDidSendMoveInfo	=	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이동 했을시에 이동 메시지를 주위에 플레이어들에게 보냈는가 알아온다.
//	수정 일자	:	2002-07-26 오후 4:02:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	BaseChar::DidSendMoveInfo()
{
	if(bDidSendMoveInfo == TRUE)
	{
		bDidSendMoveInfo = FALSE;
		return	TRUE;		
	}
	
	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 준다.
//	수정 일자	:	2002-09-30 오후 5:28:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	BaseChar::GiveItem(	UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, BaseChar *pOppBaseChar, UI08 uiOppFollowerID, SI16 *psiInvenPos, SI16 *psiOppInvenPos )
{
	// 상대방 캐릭터가 아이템을 받을 수 있는지 검사한다.
	if( pOppBaseChar->IFollower.CanAddItem( uiOppFollowerID, uiItemID, uiItemQuantity ) == FALSE )								
		return	FALSE;

	// 내 용병에게서 아이템을 뺀다.
	if( IFollower.DelItem( uiFollowerID, uiItemID, uiItemQuantity, psiInvenPos ) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	
		return	FALSE;

	// 상대방 용병에게 아이템을 준다.
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