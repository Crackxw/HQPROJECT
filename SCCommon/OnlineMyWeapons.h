#ifndef	_ONLINE_MYWEAPON_HEADER
#define _ONLINE_MYWEAPON_HEADER

#include	"GSCDefine.h"

struct		WearItemInfo
{
	UI16	uiItemID;	
	SI16	siItemLimitTime;
};

class	MyWeapons
{
private:	
//public:
	//UI16		uiWearItem[WEAR_ITEM_NUM];
	WearItemInfo	stWearItemInfo[WEAR_ITEM_NUM];
	//	투구0	무기1	갑옷2	허리띠3		장갑4	신발5	오른손반지6		왼손반지7	수호부8
	/*	
		ON_WEAR_HELMET_ITEM				0
		ON_WEAR_WEAPON_ITEM				1
		ON_WEAR_ARMOR_ITEM				2
		ON_WEAR_BELT_ITEM				3
		ON_WEAR_GLOVE_ITEM				4
		ON_WEAR_SHOES_ITEM				5
		ON_WEAR_RRING_ITEM				6
		ON_WEAR_LRING_ITEM				7
		ON_WEAR_GUARDIAN_ITEM			8
	*/
public:
	VOID	Init();											// 초기화
	UI16	GetWearItem(UI08	WearLoc);					// WearLoc 위치에 착용하고 있는 아이템을 리턴한다.
	SI16	GetWearItemLimitTime(UI08	WearLoc);
	VOID	SetWearItem(UI08	WearLoc, UI16	uiItem, SI16	siLimitTime);	// WearLoc 위치에 착용하고 있는 아이템을 바꾼다.
	BOOL	GetDBWeapons();									//	DB에서 값을 가져 온다.
	BOOL	IsEmpty();
	SI32	CheckItemCount(UI16 uiItemID);					// 특정 아이템의 수를 검사한다.
	BOOL	GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM]);	// 현재 장착된 아이템들의 리스트를 얻어온다.
};

#endif