#include "SOMain.h"
#include "SOWorld.h"
#include "IOnlineMap.h"
#include "OnlineMapIndex-Parser.h"


SOWorld*	ISOMap::pMyWorld;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
ISOMap::ISOMap()
{
	pMap					=	NULL;
	dwSentCharMoveMsgTime	=	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
ISOMap::~ISOMap()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-07-26 螃�� 5:17:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::Init()
{	
	SI32					i;	
	OnlineMapIndexParser	MapIndexParser;
	OnlineVillageParser		VillageParser;
	BOOL					bRet = FALSE;
	MapIndexHeader			*pMapIndexHeader;	

	// Village Parser蒂 煎萄и棻.	
	if(VillageParser.ReadText("Online\\Village\\VillageInfo.txt", NULL) == FALSE)	goto	END;

	// Map Index �倣狨� 煎萄и棻.	
	if(MapIndexParser.ReadText("Online\\Map\\MapIndex.txt", &VillageParser) == TRUE)
	{
		siTotalMapNum	=	MapIndexParser.GetMapDataCount();
		pMap			=	new	SOMap[MapIndexParser.GetMapDataCount()];
	
		// 裘擊 蟾晦�� и棻.
		for(i = 0; i < siTotalMapNum; i++)
		{			
			pMapIndexHeader	=	MapIndexParser.GetMapData(i);
		
			if(pMapIndexHeader == NULL)	goto	END;

			if(pMap[i].Init(pMapIndexHeader) == FALSE)
				goto END;
		
		}		

		bRet			=	TRUE;
	}

END:
	MapIndexParser.Free();
	VillageParser.Free();

	return bRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOMap::Free()
{
	if(pMap)
	{
		delete [] pMap;
		pMap	=	NULL;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 裘縑 撲薑и棻.
//	熱薑 橾濠	:	2002-11-19 螃瞪 9:35:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::SetItem( UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	return pMap[ uiMapID ].SetItem( siX, siY, uiItemID, uiItemQuantity, bSendToDB );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 裘縑憮 薯剪и棻.
//	熱薑 橾濠	:	2002-11-19 螃瞪 10:14:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收	
BOOL	ISOMap::DelItem( SOPlayer *pPlayer, UI16 uiMapID, SI32 siX, SI32 siY, BOOL bSendToDB )
{
	return pMap[ uiMapID ].DelItem( pPlayer, siX, siY, bSendToDB );
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	裘縑 ID蒂 撲薑и棻
//	熱薑 橾濠	:	2002-06-03 螃瞪 10:22:39 - 曄薑賅.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::SetID(UI16 uiMapID, SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI16 uiCharMoveableTileInfo)
{
	return pMap[uiMapID].SetID(siX, siY, pBaseChar, uiItemID, uiItemQuantity, uiCharMoveableTileInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘縑憮 ID蒂 餉薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOMap::DelID(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siDelInfo)
{
	if(IsInitialize() == FALSE)	return;

	pMap[uiMapID].DelID(siX, siY, siDelInfo);
}
*/
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOMap::GetID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, 
					OnlineReceiveFieldItemInfo *pFieldItemInfo, 
					UI16 *psiCharNum, UI16 *psiItemNum, SI32 siGetCharInfo, UI16 uiExceptionCharID)
{
	pMap[uiMapID].GetID(siSX, siSY, siEX, siEY, pCharUniIDs, pFieldItemInfo, psiCharNum, psiItemNum, siGetCharInfo, uiExceptionCharID);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦溼脹 葆擊 Index Code蒂 橢橫螞棻.
//	濛撩濠	:	檜撩營
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	ISOMap::GetVillageCode(UI16 uiMapID)
{
	UI16		uiVillageCode = 0;

	if(IsValidMapID(uiMapID) == TRUE)		
	{		
		uiVillageCode	=	pMap[uiMapID].GetVillageCode();
	}

	return uiVillageCode;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘縑憮 議葛攪曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	ISOMap::GetCharUniID(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siGetCharInfo)
{
	if(IsInitialize() == FALSE)	return 0;

	return pMap[uiMapID].GetCharUniID(siX, siY, siGetCharInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	橾薑 艙羲縑 氈朝 議葛攪菟曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	ISOMap::GetCharUniID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, UI16 uiMaxGetCharIDNum, SI32 siGetCharInfo)
{
	return pMap[uiMapID].GetCharUniID(siSX, siSY, siEX, siEY, pCharUniIDs, uiMaxGetCharIDNum, siGetCharInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	ISOMap::GetItemID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pItemIDs, UI16 *pItemQuantity)
{
	return pMap[uiMapID].GetItemID(siSX, siSY, siEX, siEY, pItemIDs, pItemQuantity);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и 顫橾縑 撲薑腎橫 氈朝 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::GetItemID(UI16 uiMapID, SI32 siX, SI32 siY, UI16 *puiItemID, UI16 *pItemQuantity)
{
	return pMap[uiMapID].GetItemID(siX, siY, puiItemID, pItemQuantity);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘縑 樓撩擊 撲薑п遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::SetMap(UI16 uiMapID, LoadSaveMapTI* pMapTI)
{
	return pMap[uiMapID].SetMap(pMapTI);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘縑憮 か薑 謝ル蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
POINT	ISOMap::SearchMap(UI16 uiMapID, SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 )
{
	return pMap[uiMapID].SearchMap(siCX, siCY, siSearchInfo, siParam1, siParam2);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB煎睡攪 в萄縑 雲橫螳 氈朝 嬴檜蠱曖 薑爾蒂 橢橫諦憮 裘縑 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOMap::SetItemFromDB()
{
	SI32	i;

	for(i = 0; i < siTotalMapNum; i++)	
		pMap[i].SetItemFromDB();	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 裘 嬴檜蛤 檣雖 獄棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::IsValidMapID(UI16 uiMapID)
{
	if(uiMapID >= 0 && uiMapID < siTotalMapNum)
		return TRUE;
	else
		return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	裘曖 餌檜鍔蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SIZE	ISOMap::GetMapSize(UI16 uiMapID)
{
	SIZE	szMapSize	=	{0, 0};

	if(IsValidMapID(uiMapID) == TRUE)		
	{		
		szMapSize	=	pMap[uiMapID].GetMapSize();
	}

	return szMapSize;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦�降� 腎歷朝陛?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::IsInitialize()
{
	if(pMap != NULL)
		return TRUE;
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	醱給 顫橾檣雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::IsColl(UI16 uiMapID, SI32 siX, SI32 siY)
{
	if(IsValidMapID(uiMapID) == TRUE)		
	{
		return pMap[uiMapID].IsColl(siX, siY);
	}
	else
		return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	醱給 顫橾檣雖 憲嬴螞棻.(か薑 議葛攪 殮濰縑憮)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::IsColl(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb)
{
	if(IsValidMapID(uiMapID) == TRUE)		
	{
		return pMap[uiMapID].IsColl(siX, siY, siCharMoveableTileAtb);
	}
	else
		return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	湍瞪檣陛?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::IsDungeon(UI16 uiMapID)
{
	if(IsValidMapID(uiMapID) == TRUE)
		return pMap[uiMapID].IsDungeon();
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	綠陛 螢 熱 氈朝 雖羲檣陛?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::CanBeRainy( UI16 uiMapID )
{
	return pMap[uiMapID].CanBeRainy();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	換檜 螢 熱 氈朝 雖羲檣陛?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::CanBeSnowy( UI16 uiMapID )
{
	return pMap[uiMapID].CanBeSnowy();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 1:37:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo)
{
	if(IsValidMapID(pBaseChar->uiMapID) == TRUE)	return pMap[pBaseChar->uiMapID].SetChar(pBaseChar, uiCharMoveableTileInfo);
	else											return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪蒂 檜翕 衛鑑棻.	
//	熱薑 橾濠	:	2002-06-03 螃�� 1:39:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::MoveChar(BaseChar *pBaseChar, SI32 siX, SI32 siY, BOOL bWarUnit,UI32 uiCharMoveableTileInfo)
{
	if(IsValidMapID(pBaseChar->uiMapID) == TRUE)	return pMap[pBaseChar->uiMapID].MoveChar(pBaseChar, siX, siY, bWarUnit,uiCharMoveableTileInfo);
	else											return FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪蒂 餉薯и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 1:40:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::DelChar(BaseChar *pBaseChar)
{
	if(IsValidMapID(pBaseChar->uiMapID) == TRUE)	return pMap[pBaseChar->uiMapID].DelChar(pBaseChar);
	else
	{
		// ⑷營 議葛攪陛 氈朝 裘曖 ID陛 澀跤腎歷棻.
//		pMyWorld->clServer->WriteInfo("..\\log_for_debug.txt", "1 [uiMapID : %d]", pBaseChar->uiMapID );
		writeInfoToFile("log_for_debug.txt", "1 [uiMapID : %d]", pBaseChar->uiMapID );
		return FALSE;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Polling
//	熱薑 橾濠	:	2002-06-03 螃�� 4:59:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOMap::Poll()
{	
	SI32	i;
	BOOL	bSendMoveMsg;
	
	if(GETTIMEGAP(dwSentCharMoveMsgTime, pMyWorld->dwtimeGetTime) >= 1000)
	{
		bSendMoveMsg			=	TRUE;
		dwSentCharMoveMsgTime	=	pMyWorld->dwtimeGetTime;
	}
	else																		bSendMoveMsg	=	FALSE;

	for(i = 0; i < 	siTotalMapNum; i++)
		pMap[i].Poll(bSendMoveMsg);	

}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	X, Y蒂 晦遽戲煎 輿嬪 播攪曖 Ы溯檜橫 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 8:46:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	ISOMap::GetNearSectorPlayerList(UI16 uiMapID, SI32 siX, SI32 siY, UI16 *pCharUniIDs, SI32 siGetCharInfo)
{
	if(IsValidMapID(uiMapID) == TRUE)	return	pMap[uiMapID].GetNearSectorPlayerList(siX, siY, pCharUniIDs, siGetCharInfo);
	else								return	0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	X, Y蒂 晦遽戲煎 輿嬪 播攪曖 嬴檜蠱 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-04 螃瞪 10:52:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	ISOMap::GetNearSectorItemList(UI16 uiMapID, SI32 siX, SI32 siY, OnlineReceiveFieldItemInfo *pFieldItemInfo)
{
	if(IsValidMapID(uiMapID) == TRUE)	return	pMap[uiMapID].GetNearSectorItemList(siX, siY, pFieldItemInfo);
	else								return	0;	
}

SI32	ISOMap::GetSectorItemList( UI16 uiMapID, strSector **pSector, OnlineReceiveFieldItemInfo *pFieldItemInfo )
{
	if( IsValidMapID( uiMapID ) ) 
		return	pMap[ uiMapID ].GetSectorItemList( pSector, pFieldItemInfo );
	
	return	0;	
}

SI32	ISOMap::GetSectorPlayerList( UI16 uiMapID, strSector **pSector, UI16 *pCharUniIDs, SI32 siGetCharInfo )
{
	if( IsValidMapID( uiMapID ) ) 
		return	pMap[ uiMapID ].GetSectorPlayerList( pSector, pCharUniIDs, siGetCharInfo );
	
	return	0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼠紫濰檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 4:28:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::IsWarFieldMap(UI16 uiMapID)
{
	if( IsValidMapID( uiMapID ) ) 
		return	pMap[ uiMapID ].IsWarFieldMap();
	
	return	FALSE;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	顫橾曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-18 螃�� 3:28:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOMap::GetTileInfo(UI16 uiMapID, SI32 siX, SI32 siY, CHAR *pszBuffer)
{
	if( IsValidMapID( uiMapID ) ) 
		return	pMap[ uiMapID ].GetTileInfo(siX, siY, pszBuffer);
	
	return	FALSE;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�倆挬�.
//	熱薑 橾濠	:	2002-07-26 螃�� 5:17:32 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOMap::InitStaticVariable(SOWorld *pWorld)
{
	pMyWorld	=		pWorld;

	// SOMap擊 蟾晦�倆挬�.
	SOMap::InitStaticVariable(pWorld);
}
