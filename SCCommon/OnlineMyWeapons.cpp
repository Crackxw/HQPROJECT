#include <GSL.h>
#include "OnlineMyWeapons.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	초기화
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyWeapons::Init()
{
	ZeroMemory(&stWearItemInfo,sizeof(WearItemInfo) * WEAR_ITEM_NUM);
	/*uiHelmetID	=	0;				//	투구
	uiWeaponID	=	0;				//	무기
	uiArmorID	=	0;				//	갑옷
	uiBeltID	=	0;				//	허리띠.
	uiGloveID	=	0;				//	장갑
	uiShoesID	=	0;				//	신발
	uiRRingID	=	0;				//	오른손 반지
	uiLRingID	=	0;				//	왼손 반지.
	uiGuardianID=	0;				//	수호부.*/
}


UI16	MyWeapons::GetWearItem(UI08	WearLoc)
{
	if(WearLoc < WEAR_ITEM_NUM)
		return stWearItemInfo[WearLoc].uiItemID;
	return 0;
}

SI16	MyWeapons::GetWearItemLimitTime(UI08 WearLoc)
{
	if(WearLoc < WEAR_ITEM_NUM)
		return stWearItemInfo[WearLoc].siItemLimitTime;
	return 0;
}

VOID	MyWeapons::SetWearItem(UI08	WearLoc, UI16	uiItem, SI16 siLimitTime)
{
	if(WearLoc < WEAR_ITEM_NUM)
	{
		stWearItemInfo[WearLoc].uiItemID		=	uiItem;
		stWearItemInfo[WearLoc].siItemLimitTime =	siLimitTime; 
	}
}


BOOL	MyWeapons::IsEmpty()
{
	for(int i=0; i< WEAR_ITEM_NUM; i++)
	{
		if(stWearItemInfo[i].uiItemID != 0 )
			return FALSE;
	}
	return TRUE;
}

SI32	MyWeapons::CheckItemCount(UI16 uiItemID)
{
	SI32		TempCount;
	SI32		TempAddCount;

	TempAddCount = 0;
	for(TempCount = 0; TempCount < WEAR_ITEM_NUM; TempCount++)
	{
		if(stWearItemInfo[TempCount].uiItemID == uiItemID)
		{
			TempAddCount++;
		}
	}

	return TempAddCount;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 장착된 아이템들의 리스트를 얻어온다.
//	수정 일자	:	2002-07-13 오후 3:00:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyWeapons::GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM])
{
	//memcpy(puiWearItems, uiWearItem, sizeof(UI16) * WEAR_ITEM_NUM);
	for(SI16 i = 0; i < WEAR_ITEM_NUM ; i++)
		puiWearItems[i]	= stWearItemInfo[i].uiItemID;
	return	TRUE;
}