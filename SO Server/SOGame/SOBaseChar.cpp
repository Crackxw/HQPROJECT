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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BaseChar::BaseChar()
{
	Init();	
	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BaseChar::~BaseChar()
{
	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蝶鷓す 滲熱蒂 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	BaseChar::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld	 = pWorld;

	m_pBattleMgr =  &pMyWorld->m_BattleMgr;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 議葛攪曖 id 檣雖 獄棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 ⑷營 嬪纂蒂 裘縑 撲薑и棻.
//					⑷營 撲薑ж溥朝 嬪纂朝 奩萄衛 綴奢除檜罹撿 и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 1:32:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::SetIDOnMap( SI32 siParam1 )
{
	// 議葛攪陛 檜翕ж雖 彊擎 鼻鷓煎 菩棻.
	bDidSendMoveInfo	=	FALSE;

	if(pMyWorld->clISOMap.SetChar(this, uiMoveableTileInfo) == FALSE)
	{
		// 議葛攪蒂 撲薑ж朝等 褒ぬц棻.
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap.txt", "Parameter : [ %d ], siX : [ %d ], siY : [ %d ]", siParam1, siX, siY );
		writeInfoToFile("SetIDOnMap.txt", "Parameter : [ %d ], siX : [ %d ], siY : [ %d ]", siParam1, siX, siY );
		return	FALSE;
	}	

	return	TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	裘縑憮 ID蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-07-01 螃�� 3:34:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::DelIDOnMap( SI32 siParam1 )
{
	// 議葛攪陛 檜翕ж雖 彊擎 鼻鷓煎 菩棻.
	bDidSendMoveInfo	=	FALSE;

	// 辦摹 斜 嬪纂縑 頂陛 氈朝雖 �挫恉挬�.	
	if(pMyWorld->clISOMap.DelChar(this) == FALSE)
	{
		// 裘縑憮 議葛攪蒂 餉薯ж朝等 褒ぬц棻.
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap.txt", "Parameter : [ %d ]", siParam1 );
		writeInfoToFile("DelIDOnMap.txt", "Parameter : [ %d ]", siParam1 );
		return	FALSE;
	}	

	return	TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	BaseChar::GetID()
{
	return szID;
}



//褐幗蟒
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	OnlineReceivedPlayerInfo曖 寡翮 掘褻羹縑 puiCharIDs縑 氈朝 議葛攪菟曖 薑爾蒂 橢橫諦憮 厥橫遽棻.
//	熱薑 橾濠	:	2002-11-28 螃瞪 10:19:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

				// 跨蝶攪橾 唳辦縑朝 蹺陛 薑爾陛 橈棻.
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

				// 寡曖 唳辦縑朝 蹺陛 薑爾陛 橈棻.
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

				// 褒薯 詭衛雖蒂 嫡朝 Ы溯檜橫熱蒂 隸陛 衛鑑棻.
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
		// 褒薯 嶸�褲葧� Ы溯檜橫 熱蒂 雖薑и棻.
		*puiRealPlayerNum	=	uiRealPlayerNum;
	}

	return siTotalLength;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	晦獄 議葛攪曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-08-15 螃�� 12:55:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	BaseChar::GetInfo(OnCharBaseInfo *pCharBaseInfo)
{	
	pCharBaseInfo->siX				= siX;
	pCharBaseInfo->siY	       		= siY;
	pCharBaseInfo->uiKind			= uiKind;
	pCharBaseInfo->uiAccount	    = uiAccount;	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦�飛� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦�飛� и棻.
//	熱薑 橾濠	: 2002-02-19 螃瞪 11:47:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 錶Щ 衛鑑棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	POINT		ptWarpXY;					// 錶Щи 雖薄

	// ⑷營 奩萄衛 в萄 鼻縑 氈橫撿 и棻.
	//if(siStatus	!= ON_PLAYER_STATUS_INFIELD)	return FALSE;
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;

	ptWarpXY	=	pMyWorld->clISOMap.SearchMap(uimapid, six, siy, ON_MAP_SI_CHAR, uiMoveableTileInfo);

	// 錶Щй 夠縑 錶Щй 濠葬蒂 瓊雖 跤п憮 褒ぬц棻.				
	if((ptWarpXY.x == -1) && (ptWarpXY.y == -1))	return FALSE;

	// 裘曖 ⑷營 嬪纂縑憮 頂 ID蒂 雖遴棻.
	if(DelIDOnMap( 1 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "3 [%u]", uiAccount);
//		pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "3 [%u]", uiAccount);


	uiMapID		=	uimapid;
	siX			=	SI16(ptWarpXY.x);
	siY			=	SI16(ptWarpXY.y);	

	// 裘曖 億煎遴 嬪纂縑 頂 ID蒂 撲薑и棻.
	if(SetIDOnMap( 2 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "2");
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "2");

	return TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 謙盟蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	BaseChar::GetKind()
{	
	return uiKind;
}	



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 в萄縑憮曖 謝ル蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-15 螃�� 12:53:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	BaseChar::GetPos(UI16 *puiMapID, LPPOINT pPoint)
{
	*puiMapID	=	uiMapID;
	pPoint->x	=	siX;
	pPoint->y	=	siY;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 鼻鷓(в萄, 葆擊, 瞪癱醞)蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-02 螃瞪 10:25:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	BaseChar::GetStatus()
{
	return siStatus;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱溘擊 橢擠
//	熱薑 橾濠	:	2002-06-20 螃�� 2:18:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 議葛攪檣陛?
//	熱薑 橾濠	:	2002-06-29 螃瞪 10:14:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::IsValid()
{
	if(uiAccount != 0)	return	TRUE;
	else				return	FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱葬蝶お蒂 給溥遽棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 8:12:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MyItemData*		BaseChar::GetItemList(UI08 uiFollowerSlot)
{
	// 議葛攪陛 嶸�褲狫� 彊戲賊 NULL擊 葬欐и棻.
	if(IsValid() == FALSE)	return	NULL;
	
	return	IFollower.GetItemList(uiFollowerSlot);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 蹺陛п遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		// Ы溯檜橫橾 唳辦縑虜 DB縑 盪濰и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 蹺陛п遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		// Ы溯檜橫橾 唳辦縑虜 DB縑 盪濰и棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛п遽棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 11:05:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		// Ы溯檜橫橾 唳辦縑虜 DB縑 盪濰и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛п遽棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 11:05:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		// Ы溯檜橫橾 唳辦縑虜 DB縑 盪濰и棻.
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




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 10:01:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// 嬴檜蠱擊 餉薯ж晦 嬪п憮朝 DB縑憮 匐餌蒂 ж晦 嬪ж罹 錳楚 熱榆紫 偽檜 厥橫醜撿 и棻
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 10:01:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// 嬴檜蠱擊 餉薯ж晦 嬪п憮朝 DB縑憮 匐餌蒂 ж晦 嬪ж罹 錳楚 熱榆紫 偽檜 厥橫醜撿 и棻
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
				DBPlayerItemInfo.uiNewQuantity		=	uiQuantity;					// 嬴檜蠱擊 餉薯ж晦 嬪п憮朝 DB縑憮 匐餌蒂 ж晦 嬪ж罹 錳楚 熱榆紫 偽檜 厥橫醜撿 и棻
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
//	撲貲	:	嬴檜蠱檜 氈朝雖 匐餌ж堅 餉薯и棻.
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
					// 瓊戲賊 餉薯и棻.
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
//	撲貲	:	嬴檜蠱檜 氈朝雖 匐餌ж堅 餉薯и棻.
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
					// 瓊戲賊 餉薯и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-23 螃�� 9:56:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MyItemData*	BaseChar::GetItem(UI08 uiFollowerID, UI16 uiID)
{
	return IFollower.GetItemByID( uiFollowerID, uiID );	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 4:36:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::CanAddItem(UI08 uiFollowerID, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum)
{
	return	IFollower.CanAddItem(uiFollowerID, puiID, puiQuantity, uiItemNum);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 辨煽曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-07 螃�� 1:41:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
			// 嶸�褲� 辨煽檜棻.
			dwFollowerSize			=	GetFollowerInfo(i, pFollowerInfo);

			dwAllFollowerSize		+=	dwFollowerSize;
			pFollowerInfo			=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerSize);
	
			// 辨煽熱蒂 隸陛衛鑑棻.
			(*puiFollowerNum)++;
		}
	}

	return	dwAllFollowerSize;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-07 螃�� 1:26:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	BaseChar::GetFollowerInfo(UI08 uiFollowerID, OnFollowerInfo *pFollowerInfo)
{	
	OnlineReceivedMyItemInfo	*pItemInfo;
	SI32						i;
	OnlineFollower				*pFollower;
	MyItemData					ItemData;

	// 嶸辨и 辨煽檜 嬴棲塭賊 0擊 葬欐и棻.
	if(IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	0;
	
	pFollowerInfo->uiSlot				=	uiFollowerID;				
	pFollowerInfo->uiKind				=	IFollower.GetFollowerCharID(uiFollowerID);				
	pFollowerInfo->siLevel				=	IFollower.GetFollowerParameterLevel(uiFollowerID);				
	pFollowerInfo->siExp				=	IFollower.GetFollowerBaseParameterCurExp(uiFollowerID);				// 唳я纂.
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
	
	// 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
	pFollowerInfo->uiItemNum			=	0;
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(IFollower.GetItem(uiFollowerID, i, &ItemData) == TRUE)
		{
			pItemInfo[pFollowerInfo->uiItemNum].uiID				=	ItemData.uiID;				// 嬴檜蠱囀萄
			pItemInfo[pFollowerInfo->uiItemNum].uiQuantity			=	ItemData.uiQuantity;			// 嬴檜蠱曖 熱榆
			pItemInfo[pFollowerInfo->uiItemNum].siPosInInventory	=	ItemData.siPosInInventory;	// 嬴檜蠱曖 嬪纂
			pFollowerInfo->uiItemNum++;
		}							
	}

	return	DWORD(sizeof(OnFollowerInfo) + (sizeof(OnlineReceivedMyItemInfo) * pFollowerInfo->uiItemNum));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽擊 餉薯и棻.
//	熱薑 橾濠	:	2002-07-08 螃瞪 12:16:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::DeleteFollower(UI08 uiFollowerID)
{
	if(IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	FALSE;

	// 辨煽擊 餉薯и棻.
	IFollower.DeleteFollower(uiFollowerID);

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜翕ц棻堅 撲薑и棻.
//	熱薑 橾濠	:	2002-07-26 螃�� 4:02:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	BaseChar::SetMoved()
{
	bDidSendMoveInfo	=	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜翕 ц擊衛縑 檜翕 詭衛雖蒂 輿嬪縑 Ы溯檜橫菟縑啪 爾麻朝陛 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-26 螃�� 4:02:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	BaseChar::DidSendMoveInfo()
{
	if(bDidSendMoveInfo == TRUE)
	{
		bDidSendMoveInfo = FALSE;
		return	TRUE;		
	}
	
	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 遽棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 5:28:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	BaseChar::GiveItem(	UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, BaseChar *pOppBaseChar, UI08 uiOppFollowerID, SI16 *psiInvenPos, SI16 *psiOppInvenPos )
{
	// 鼻渠寞 議葛攪陛 嬴檜蠱擊 嫡擊 熱 氈朝雖 匐餌и棻.
	if( pOppBaseChar->IFollower.CanAddItem( uiOppFollowerID, uiItemID, uiItemQuantity ) == FALSE )								
		return	FALSE;

	// 頂 辨煽縑啪憮 嬴檜蠱擊 賓棻.
	if( IFollower.DelItem( uiFollowerID, uiItemID, uiItemQuantity, psiInvenPos ) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	
		return	FALSE;

	// 鼻渠寞 辨煽縑啪 嬴檜蠱擊 遽棻.
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