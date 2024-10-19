/************************************************************************************************
	���ϸ� : IOnlineFollower.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_FOLLOWERDATA_H
#define _ONLINE_FOLLOWERDATA_H

#include "Defines.h"
#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include "Parametabox.h"
#include "OnlineMyWeapons.h"
#include "OnlineItem.h"
#include "OnlineChar-Parser.h"


#define ONLINEFOLLOWER_LIFTPOWERCONSTANCE			2					// str * ???

#define ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE		0					// ����
#define ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD		1					// �߰�
#define ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE		2					// ���� ����

#define ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE		0					// ����
#define ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL		1					// �Ϻθ� ����
#define ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL		2					// ��ü ����


#define CREDIT_ERROR			0
#define LEVEL_ERROR				1
#define FAN_ERROR				2
#define DOUBLESTICK_ERROR		3
#define SPINSTONE_ERROR			4
#define SCYTHE_ERROR			5
#define BUDDIST_ERROR			6
#define NOFIND_ERROR			7
#define CANTPUTONDRESS_ERROR    8
#define CANTWEARGAURDIAN_ERROR	9
#define CANTWEARTIMELIMITITEM_ERROR	10

#define	MAX_FOLLOWER_HAVE_ITEM_NUM					SI16( 32000 )		// �뺴�� �ִ� ���� �� �ִ� �������� ��.

// ĳ���Ͱ� ������ �ִ� ����.(�Ѹ��� ����Ÿ.)
struct	OnlineFollower
{
	static	CItem					*pCItem;							// Item Class.	

	UI16				uiCharID;											// Character ID

	char				Name[ON_ID_LENGTH + 1];								// Name

	_ParametaBox		ParametaBox;										// ĳ������ ���� ������ ������ �ִ�.
	SI32				Nation;												// Nation	
	
	MyItemData			Items[ON_MYDATA_MAX_ITEM_NUM];						// �� �������� ����.	
	MyWeapons			Weapon;												// ���� ������� ���� ����.	
	DWORD				dwSaveTime;											// parameter�� DB�� ������ �ð�.

	DWORD				dwTotalItemWeight;									// ���� ĳ���Ͱ� �����ϰ� �ִ� �������� ����.

	// �Լ���.
	OnlineFollower();
	~OnlineFollower();
	VOID				Init();
	MyItemData*			GetItem(UI16 uiItemID);	// �������� ���´�.
	MyItemData*			GetEmptyItem();
	DWORD				AddItem(MyItemData* pMyItem, BOOL bTakeOff = FALSE);										// �������� �߰��Ѵ�.
	DWORD				AddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff = FALSE );							// �������� �߰��Ѵ�.
	DWORD				AddItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bTakeOff = FALSE);		// �������� �߰��Ѵ�.
	DWORD				DelItem(MyItemData* pMyItem, BOOL bPutOn = FALSE);											// ������ ���Կ� �ִ� �������� ������ ����.
	DWORD				DelItem(UI16 uiItemID, UI16 uiQuantity, BOOL bPutOn = FALSE);								// ������ ���Կ� �ִ� �������� ������ ����.
	DWORD				DelItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bPutOn = FALSE);		// ������ ���Կ� �ִ� �������� ������ ����.
//	MyItemData*			WearItem(CItemHeader* pItemHeader, SI08 uiLoc, SI32 siTradeGrade);
	BOOL				CheckWeight(UI32 Weight);
	BOOL				IsEmpty();

	BOOL				CanAddItem(UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory, BOOL bTakeOff = FALSE);	// �������� �߰� ��ų �� �ִ���?	
	BOOL				CanAddItem(UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum, BOOL bTakeOff = FALSE);			// �������� �߰� ��ų �� �ִ���?	
	BOOL				CanAddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff = FALSE);							// �������� �߰� ��ų �� �ִ���?
	BOOL				GetItemByPos( SI16 siPosInInventory, MyItemData **ppItemData );								// �������� ���´�.

	BOOL				CanPutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode);		// �κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.
	BOOL				CanTakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory);						// �κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.

	DWORD				GetTotalItemWeight();												// ���� �� ĳ������ ��ü �������� ���Ը� ���´�.	
	DWORD				GetMaxWeight();														// �� ĳ���Ͱ� �ְ� �� �� �ִ� ����.
	DWORD				IsOverflowItemWeight(DWORD dwAddItemWeight);						// �������� �߰������� ��� ���Ը� �ʰ��ϴ��� �˾ƿ´�.	
	BOOL				IsOverWeight();														// ���� �����ʰ��� �ȱ� ���� �������� �˾ƿ´�.
	BOOL				AddItemWeight(UI16 uiItemID, UI16 uiItemQuantity);					// �������� ���Ը� �߰������ش�.
	BOOL				DelItemWeight(UI16 uiItemID, UI16 uiItemQuantity);					// �������� ���Ը� ���ش�.	
	UI16				GetItemQuantityToTake(UI16 uiItemID);								// �� ĳ���Ͱ� �ְ� ���� �� �ִ� �������� ������ ���´�.
	BOOL				IsItemWeightOverflowed();											// �� ĳ���Ͱ� �ڱⰡ ������ �� �ִ°� ���� �������� �ʰ� �ؼ� ����� ���´�.
	BOOL				TakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);	// �����ϰ� �ִ� �������� ���´�.
	BOOL				PutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);	// �κ��丮�� �ִ� �������� �����Ѵ�.
	BOOL				DisAppearItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);
	UI16				GetWearItem( UI08 uiWearLoc, BOOL &bIsTimerItem, SI16 &siRemainDate );
	BOOL				GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM]);												// ���� ������ �����۵��� ����Ʈ�� ���´�.
	BOOL				SetWearItem(UI08 uiLoc, UI16 uiItemID,SI16 siItemLimitTime);								// �������� �����Ѵ�.	

	BOOL				HasItem( UI16 uiItemID, UI16 uiItemQuantity );						// �������� ������ �ִ��� �˻��Ѵ�.
	SI32                GetItemIndex(UI16 uiItemID);
	SI32                GetItemIndex(UI16 uiItemID,UI16 uiItemQuantity);		

	BOOL				CanMoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory );		// �������� �ű� �� �ִ��� �˻��Ѵ�.
	BOOL				MoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 *psiPosInInventory );		// �������� �ñ��.

	BOOL				IsEmptyItemInInventory( SI16 siPosInInventory );								// �κ��丮���� �������� �ִ� ��ġ�� ������� �˾ƿ´�.
	BOOL				IsValidItemPosInInventory( SI16 siPosInInventory );								// �κ��丮������ �������� �ִ� ��ġ�� ��ȿ���� �˻��Ѵ�.

	static	VOID		InitStaticVariable(CItem* pItem);										// ���� ���� �ʱ�ȭ.

	VOID				SetFollowerParameterNation(SI32 siNation)	{ Nation = siNation; };
	VOID				SetCharID(UI16 CharID)	{ uiCharID = CharID; };
	BOOL				CanWeaponBeUpgraded();												// �������� ���Ⱑ ���׷��̵� �������� �˾ƿ´�(��޹����ΰ�?)
	VOID				UpgradeWeapon( UI16 uiItemID );											// �������� ���⸦ Ư�� ����� ���׷��̵����ش�.
	VOID				BrakeWeapon();														// �������� ���⸦ ���߸���.(����)
	MyWeapons*			GetMyWeaponsInterface	( void );									// �� ĳ������ ���� Ŭ���� �������̽��� �޾ƿ´�.
};	

class	IOnlineFollower
{
private:		
	OnlineFollower		Followers[ON_MAX_FOLLOWER_NUM];					// ���ϵ��� ����Ÿ.

public:
	IOnlineFollower();
	~IOnlineFollower();

	VOID	Init();																				// �������� �ʱ�ȭ�� �����ϰ� �ʿ��� �޸𸮸� �Ҵ� �޴´�.
	VOID	Free();																				// �� Ŭ�������� ����ߴ� �޸𸮸� ���� �����Ѵ�.

	UI16				FindEmptyItemBuffer(UI08 uiFollowerSlot);								// ������ ���� �߿� ����ִ� ���۸� ã�´�.	
	UI16				FindSameItemBuffer(UI08 uiFollowerSlot, UI16 ItemID);					// ������ ���� �߿� ���� ������ ���۸� ã�´�.
	SI16				FindEmptyFollowerSlot();												// ����ִ� �뺴 ���۸� ã�´�.

	// ���� ���� �Լ�.
	BOOL	InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem);				// ���ϸ� �ʱ�ȭ �Ѵ�.	
	BOOL	InitFollower(OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem);									// ���ϸ� �ʱ�ȭ �Ѵ�.	
	BOOL	UpdataFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem);	// ���ϸ� Update �Ѵ�.	
	BOOL	DeleteFollower(UI08 uiFollowerSlot);															// ���� ����
	BOOL	IsValidFollower(UI08 uiFollowerSlot);															// ��ȿ�� ���� Slot �ΰ�?
	UI16	GetFollowerCharID(UI08 uiFollowerSlot);															// ���� ĳ������ ID�� ���´�.	
	VOID	SetFollowerCharID(UI08 uiFollowerSlot, UI16 uiCharID);
	BOOL	GetFollowerTradeInfo(UI08 uiFollowerSlot, OnTradeFollowerInfo *pTradeFollowerInfo);				// �뺴�� ���� ������ ���´�.

	// Get Follower Parameter Data
	SI32				GetFollowerBaseParameterCurExp(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterSTR(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterDEX(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterVIT(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterINT(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterAC(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterBonus(UI08 uiFollowerSlot);
	BOOL				SetFollowerBaseParameterBonus(UI08 uiFollowerSlot, SI32 siBonus);
	SI32				GetFollowerBaseParameterDamageResist(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterMagicResist(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterEquipMinDamage(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterEquipMaxDamage(UI08 uiFollowerSlot);
	SI32				GetFollowerBaseParameterLiftConstance(UI08 uiFollowerSlot);

	SI32				GetFollowerParameterLevel(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterCurExp(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterPrevExp(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterNextExp(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterNation(UI08 uiFollowerSlot);
	VOID				SetFollowerParameterNation(UI08 uiFollowerSlot, SI32 Nation);
	SI32				GetFollowerParameterTotalStr(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalDex(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalVit(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalInt(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalAC(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalDamageResist(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalMagicResist(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalEquipMinDamage(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterTotalEquipMaxDamage(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterLife(UI08 uiFollowerSlot);
	BOOL				SetFollowerParameterLife(UI08 uiFollowerSlot, SI32 Life);
	SI32				GetFollowerParameterMana(UI08 uiFollowerSlot);
	BOOL				SetFollowerParameterMana(UI08 uiFollowerSlot, SI32 Mana);
	SI32				GetFollowerParameterMoveSpeed(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterMaxWeight(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterCurWeight(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterCredit(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterLiftPower(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterHitRate(UI08 uiFollowerSlot);
	SI16				GetFollowerParameterBonus(UI08 uiFollowerSlot);
	BOOL				SetFollowerParameterBonusSubTract1(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterMaxLife(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterMaxMana(UI08 uiFollowerSlot);
	SI32				GetFollowerParameterBattlePower(UI08 uiFollowerSlot);	
	SI32				GetFollowerParameterBattlePower(void);	
	SI32				GetFollowerParameterKind(UI08 uiFollowerSlot);	

	BOOL				ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser *pCharParser, 
						UI16 uiFollowerID ,SI32 *pBonus, CItem* pItem);			// �뺴������ ���ʽ� ��ġ�� �ٲ۴�.

	char*				GetFollowerName(UI08 uiFollowerSlot);
	BOOL				SetFollowerName(UI08 uiFollowerSlot, char* lpName);

	
	
	OnlineFollower*		GetFollower(UI08 uiFollowerSlot);
	OnlineFollower*		GetHero();

	BOOL				Bonus_FollowerParameterUp(UI08 uiFollowerSlot, AbilityKindType Type, SI16 Bonus, CItem* pItem);

	// ������ ó�� ���� �Լ���.		
	DWORD		AddItem(UI08 uiFollowerSlot, MyItemData *pItem);											// �������� ������ ����Ʈ�� �߰��Ѵ�.
	DWORD		AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity);								// �������� ������ ����Ʈ�� �߰��Ѵ�.
	DWORD		AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory);		// �������� ������ ����Ʈ�� �߰��Ѵ�.	
	DWORD		AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory);		// �������� ������ ����Ʈ�� �߰��Ѵ�.
	DWORD		DelItem(UI08 uiFollowerSlot, MyItemData *pItem);											// �������� ������ ����Ʈ���� �����Ѵ�.				
	DWORD		DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity);								// �������� ������ ����Ʈ���� �����Ѵ�.				
	DWORD		DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory);		// �������� ������ ����Ʈ���� �����Ѵ�.
	BOOL		GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem);								// �������� ���´�.	
	MyItemData*	GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory);							// �������� ���´�.
	MyItemData*	GetItemByID(UI08 uiFollowerSlot, UI16 uiItemID);											// �������� ���´�.
	BOOL		GetItemByPos(UI08 uiFollowerSlot, SI16 siPosInInventory, MyItemData **ppItemData );			// �������� ���´�.
	BOOL		HasItem( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity );		// �������� ������ �ִ��� �˻��Ѵ�.
	SI32		GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity );		// �������� ������ �ִ��� �˻��Ѵ�.
	SI32		GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID);		// �������� ������ �ִ��� �˻��Ѵ�.
	BOOL		IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory);						// �������� �ִ��� �˻�
	BOOL		IsSameItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory);	// ���� �������� �ִ��� �˻�.
	MyItemData*	GetItemList(UI08 uiFollowerSlot);										// �����۸���Ʈ�� �����ش�.
	//BOOL		WearItem(UI08 uiFollowerSlot, UI16	uiItemID);
	BOOL		StripItem(UI08	uiFollowerSlot, SI16 siPosInInventory);
	BOOL		TakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);		// �����ϰ� �ִ� �������� ���´�.
	BOOL		PutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);			// �κ��丮�� �ִ� �������� �����Ѵ�.
	BOOL		DisAppearItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);
	BOOL		CheckWeight(UI08 uiFollowerSlot, UI32 Weight);
	BOOL		IsOverWeight();															// �����ʰ��� �뺴�� �Ѹ��̶� �����Ͽ� �ȱ� ���� �������� �Ǵ�
	BOOL		CheckItem(UI16 uiItemID, SI32 siCount);									// �������� �ִ��� �˻�
	SI32		GetItemCount(UI16 uiItemID);											// ������ �ִ� Item�� ���� ���´�.
	BOOL		GetWearItems(UI08 uiFollowerSlot, UI16 puiWearItems[WEAR_ITEM_NUM]);	// ���� ������ �����۵��� ����Ʈ�� ���´�.
	BOOL		SetWearItem2(UI08 uiFollowerSlot, UI08 uiLoc, UI16 uiItemID);			// �������� �����Ѵ�.
	
	BOOL		AllCharacterDeleteLife(SI32 DeleteLife);							// ��� Character�� Life�� ����.
	BOOL		AllCharacterDeleteLifePoint(SI32 DeleteLife);						// ��� Character�� Life�� ����.
	BOOL		AllCharacterDeleteLifePercent(SI32 DeleteLifeP);					// ��� Character�� Life�� %��ŭ ����.
	UI08		GetFollowerNum();													// �뺴�� ���� ���´�.
	DWORD		GetBattleInfo(UI08 uiFollowerSlot, OnImjinBattleFollowerInfo *pFollowerInfo);	// ���� ������ ���´�	
	SI32		GetUseSatietyWarAfter(void);										// ������ ����� ������ ��ġ �˻�

	BOOL		CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory);			// �������� �߰� ��ų �� �ִ���?
	BOOL		CanAddItem(UI08 uiFollowerSlot, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum);				// �������� �߰� ��ų �� �ִ���?
	BOOL		CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity );								// �������� �߰� ��ų �� �ִ���?

	BOOL		CanPutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode);		// �κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.
	BOOL		CanTakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory);						// �κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.

	BOOL		CanMoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory );	// �������� �̵� ��ų �� �ִ��� �˾ƿ´�.
	BOOL		MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 *psiPosInInventory );	// �������� �̵� ��Ų��.


	BOOL		CanAddFollower(UI08 uiAddFollowerNum);														// �뺴�� �߰� ��ų �� �ִ���?
	DWORD		GetTotalItemWeight(UI08 uiFollowerSlot);													// ���� �� ĳ������ ��ü �������� ���Ը� ���´�.	
	DWORD		GetMaxWeight(UI08 uiFollowerSlot);															// �� ĳ���Ͱ� �ְ� �� �� �ִ� ����.
	UI16		GetItemQuantityToTake(UI08 uiFollowerSlot, UI16 uiItemID);									// �� ĳ���Ͱ� �ְ� ���� �� �ִ� �������� ������ ���´�.
	BOOL		IsItemWeightOverflowed();																	// �뺴�� �߿� �ڱⰡ ������ �� �ִ°� ���� �������� �ʰ� �ؼ� ����� ���´�.
	BOOL		IsItemWeightOverflowed(UI08 uiFollowerSlot);												// Ư�� �뺴�� �ڱⰡ ������ �� �ִ°� ���� �������� �ʰ� �ؼ� ����� ���´�.

	BOOL		CanWeaponBeUpgraded( UI16 uiFollowerSlot );										// ���׷��̵� ������ �������� �˾ƿ´�. (��޹���)
	VOID		UpgradeWeapon( UI16 uiFollowerSlot, UI16 uiItemCode );							// ���� ���� �������� Ư�� ���������� ���׷��̵� �����ش�.
	VOID		BrakeWeapon( UI16 uiFollowerSlot );												// �������� ���� �������� �ı���Ų��.

	VOID		SetFullLifeOfAllChar();										// ��� �뺴���� ü���� 100% ȸ����Ų��.
	VOID		SetFullManaOfAllChar();										// ��� �뺴���� ������ 100% ȸ����Ų��.

	static	VOID	InitStaticVariable(CItem* pItem);								// ���� ���� �ʱ�ȭ.
};

#endif

