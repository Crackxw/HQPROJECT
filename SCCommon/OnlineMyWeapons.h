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
	//	����0	����1	����2	�㸮��3		�尩4	�Ź�5	�����չ���6		�޼չ���7	��ȣ��8
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
	VOID	Init();											// �ʱ�ȭ
	UI16	GetWearItem(UI08	WearLoc);					// WearLoc ��ġ�� �����ϰ� �ִ� �������� �����Ѵ�.
	SI16	GetWearItemLimitTime(UI08	WearLoc);
	VOID	SetWearItem(UI08	WearLoc, UI16	uiItem, SI16	siLimitTime);	// WearLoc ��ġ�� �����ϰ� �ִ� �������� �ٲ۴�.
	BOOL	GetDBWeapons();									//	DB���� ���� ���� �´�.
	BOOL	IsEmpty();
	SI32	CheckItemCount(UI16 uiItemID);					// Ư�� �������� ���� �˻��Ѵ�.
	BOOL	GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM]);	// ���� ������ �����۵��� ����Ʈ�� ���´�.
};

#endif