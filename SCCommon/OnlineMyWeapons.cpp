#include <GSL.h>
#include "OnlineMyWeapons.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyWeapons::Init()
{
	ZeroMemory(&stWearItemInfo,sizeof(WearItemInfo) * WEAR_ITEM_NUM);
	/*uiHelmetID	=	0;				//	����
	uiWeaponID	=	0;				//	����
	uiArmorID	=	0;				//	����
	uiBeltID	=	0;				//	�㸮��.
	uiGloveID	=	0;				//	�尩
	uiShoesID	=	0;				//	�Ź�
	uiRRingID	=	0;				//	������ ����
	uiLRingID	=	0;				//	�޼� ����.
	uiGuardianID=	0;				//	��ȣ��.*/
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �����۵��� ����Ʈ�� ���´�.
//	���� ����	:	2002-07-13 ���� 3:00:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	MyWeapons::GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM])
{
	//memcpy(puiWearItems, uiWearItem, sizeof(UI16) * WEAR_ITEM_NUM);
	for(SI16 i = 0; i < WEAR_ITEM_NUM ; i++)
		puiWearItems[i]	= stWearItemInfo[i].uiItemID;
	return	TRUE;
}