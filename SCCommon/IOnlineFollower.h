/************************************************************************************************
	파일명 : IOnlineFollower.h
	담당자 : 이윤석
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

#define ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE		0					// 실패
#define ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD		1					// 추가
#define ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE		2					// 새로 생성

#define ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE		0					// 실패
#define ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL		1					// 일부만 삭제
#define ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL		2					// 전체 삭제


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

#define	MAX_FOLLOWER_HAVE_ITEM_NUM					SI16( 32000 )		// 용병이 최대 가질 수 있는 아이템의 수.

// 캐릭터가 가지고 있는 부하.(한명의 데이타.)
struct	OnlineFollower
{
	static	CItem					*pCItem;							// Item Class.	

	UI16				uiCharID;											// Character ID

	char				Name[ON_ID_LENGTH + 1];								// Name

	_ParametaBox		ParametaBox;										// 캐릭터의 각종 정보를 가지고 있다.
	SI32				Nation;												// Nation	
	
	MyItemData			Items[ON_MYDATA_MAX_ITEM_NUM];						// 내 아이템의 정보.	
	MyWeapons			Weapon;												// 나의 무기들의 대한 정보.	
	DWORD				dwSaveTime;											// parameter를 DB에 저장한 시간.

	DWORD				dwTotalItemWeight;									// 현재 캐릭터가 장착하고 있는 아이템의 무게.

	// 함수들.
	OnlineFollower();
	~OnlineFollower();
	VOID				Init();
	MyItemData*			GetItem(UI16 uiItemID);	// 아이템을 얻어온다.
	MyItemData*			GetEmptyItem();
	DWORD				AddItem(MyItemData* pMyItem, BOOL bTakeOff = FALSE);										// 아이템을 추가한다.
	DWORD				AddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff = FALSE );							// 아이템을 추가한다.
	DWORD				AddItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bTakeOff = FALSE);		// 아이템을 추가한다.
	DWORD				DelItem(MyItemData* pMyItem, BOOL bPutOn = FALSE);											// 지정한 슬롯에 있는 아이템의 수량을 뺀다.
	DWORD				DelItem(UI16 uiItemID, UI16 uiQuantity, BOOL bPutOn = FALSE);								// 지정한 슬롯에 있는 아이템의 수량을 뺀다.
	DWORD				DelItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bPutOn = FALSE);		// 지정한 슬롯에 있는 아이템의 수량을 뺀다.
//	MyItemData*			WearItem(CItemHeader* pItemHeader, SI08 uiLoc, SI32 siTradeGrade);
	BOOL				CheckWeight(UI32 Weight);
	BOOL				IsEmpty();

	BOOL				CanAddItem(UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory, BOOL bTakeOff = FALSE);	// 아이템을 추가 시킬 수 있느냐?	
	BOOL				CanAddItem(UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum, BOOL bTakeOff = FALSE);			// 아이템을 추가 시킬 수 있느냐?	
	BOOL				CanAddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff = FALSE);							// 아이템을 추가 시킬 수 있느냐?
	BOOL				GetItemByPos( SI16 siPosInInventory, MyItemData **ppItemData );								// 아이템을 얻어온다.

	BOOL				CanPutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode);		// 인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.
	BOOL				CanTakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory);						// 인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.

	DWORD				GetTotalItemWeight();												// 현재 이 캐릭터의 전체 아이템의 무게를 얻어온다.	
	DWORD				GetMaxWeight();														// 이 캐릭터가 최고 들 수 있는 무게.
	DWORD				IsOverflowItemWeight(DWORD dwAddItemWeight);						// 아이템을 추가시켰을 경우 무게를 초과하는지 알아온다.	
	BOOL				IsOverWeight();														// 현재 무게초과로 걷기 힘든 상태인지 알아온다.
	BOOL				AddItemWeight(UI16 uiItemID, UI16 uiItemQuantity);					// 아이템의 무게를 추가시켜준다.
	BOOL				DelItemWeight(UI16 uiItemID, UI16 uiItemQuantity);					// 아이템의 무게를 빼준다.	
	UI16				GetItemQuantityToTake(UI16 uiItemID);								// 이 캐릭터가 최고 가질 수 있는 아이템의 수량을 얻어온다.
	BOOL				IsItemWeightOverflowed();											// 이 캐릭터가 자기가 수용할 수 있는것 보다 아이템을 초과 해서 드는지 얻어온다.
	BOOL				TakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);	// 장착하고 있는 아이템을 벗는다.
	BOOL				PutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);	// 인벤토리에 있는 아이템을 장착한다.
	BOOL				DisAppearItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);
	UI16				GetWearItem( UI08 uiWearLoc, BOOL &bIsTimerItem, SI16 &siRemainDate );
	BOOL				GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM]);												// 현재 장착된 아이템들의 리스트를 얻어온다.
	BOOL				SetWearItem(UI08 uiLoc, UI16 uiItemID,SI16 siItemLimitTime);								// 아이템을 장착한다.	

	BOOL				HasItem( UI16 uiItemID, UI16 uiItemQuantity );						// 아이템을 가지고 있는지 검사한다.
	SI32                GetItemIndex(UI16 uiItemID);
	SI32                GetItemIndex(UI16 uiItemID,UI16 uiItemQuantity);		

	BOOL				CanMoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory );		// 아이템을 옮길 수 있는지 검사한다.
	BOOL				MoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 *psiPosInInventory );		// 아이템을 올긴다.

	BOOL				IsEmptyItemInInventory( SI16 siPosInInventory );								// 인벤토리내의 아이템을 넣는 위치가 비웠는지 알아온다.
	BOOL				IsValidItemPosInInventory( SI16 siPosInInventory );								// 인벤토리내에서 아이템을 넣는 위치가 유효한지 검사한다.

	static	VOID		InitStaticVariable(CItem* pItem);										// 정적 변수 초기화.

	VOID				SetFollowerParameterNation(SI32 siNation)	{ Nation = siNation; };
	VOID				SetCharID(UI16 CharID)	{ uiCharID = CharID; };
	BOOL				CanWeaponBeUpgraded();												// 장착중인 무기가 업그레이드 가능한지 알아온다(고급무기인가?)
	VOID				UpgradeWeapon( UI16 uiItemID );											// 장착중인 무기를 특정 무기로 업그레이드해준다.
	VOID				BrakeWeapon();														// 장착중인 무기를 깨뜨린다.(삭제)
	MyWeapons*			GetMyWeaponsInterface	( void );									// 이 캐릭터의 무기 클래스 인터페이스를 받아온다.
};	

class	IOnlineFollower
{
private:		
	OnlineFollower		Followers[ON_MAX_FOLLOWER_NUM];					// 부하들의 데이타.

public:
	IOnlineFollower();
	~IOnlineFollower();

	VOID	Init();																				// 변수들의 초기화를 수행하고 필요한 메모리를 할당 받는다.
	VOID	Free();																				// 이 클래스에서 사용했던 메모리를 전부 해제한다.

	UI16				FindEmptyItemBuffer(UI08 uiFollowerSlot);								// 아이템 버퍼 중에 비어있는 버퍼를 찾는다.	
	UI16				FindSameItemBuffer(UI08 uiFollowerSlot, UI16 ItemID);					// 아이템 버퍼 중에 같은 아이템 버퍼를 찾는다.
	SI16				FindEmptyFollowerSlot();												// 비어있는 용병 버퍼를 찾는다.

	// 부하 관련 함수.
	BOOL	InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem);				// 부하를 초기화 한다.	
	BOOL	InitFollower(OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem);									// 부하를 초기화 한다.	
	BOOL	UpdataFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem);	// 부하를 Update 한다.	
	BOOL	DeleteFollower(UI08 uiFollowerSlot);															// 부하 삭제
	BOOL	IsValidFollower(UI08 uiFollowerSlot);															// 유효한 부하 Slot 인가?
	UI16	GetFollowerCharID(UI08 uiFollowerSlot);															// 부하 캐릭터의 ID를 얻어온다.	
	VOID	SetFollowerCharID(UI08 uiFollowerSlot, UI16 uiCharID);
	BOOL	GetFollowerTradeInfo(UI08 uiFollowerSlot, OnTradeFollowerInfo *pTradeFollowerInfo);				// 용병의 교역 정보를 얻어온다.

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
						UI16 uiFollowerID ,SI32 *pBonus, CItem* pItem);			// 용병스텟을 보너스 수치로 바꾼다.

	char*				GetFollowerName(UI08 uiFollowerSlot);
	BOOL				SetFollowerName(UI08 uiFollowerSlot, char* lpName);

	
	
	OnlineFollower*		GetFollower(UI08 uiFollowerSlot);
	OnlineFollower*		GetHero();

	BOOL				Bonus_FollowerParameterUp(UI08 uiFollowerSlot, AbilityKindType Type, SI16 Bonus, CItem* pItem);

	// 아이템 처리 관련 함수들.		
	DWORD		AddItem(UI08 uiFollowerSlot, MyItemData *pItem);											// 아이템을 아이템 리스트에 추가한다.
	DWORD		AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity);								// 아이템을 아이템 리스트에 추가한다.
	DWORD		AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory);		// 아이템을 아이템 리스트에 추가한다.	
	DWORD		AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory);		// 아이템을 아이템 리스트에 추가한다.
	DWORD		DelItem(UI08 uiFollowerSlot, MyItemData *pItem);											// 아이템을 아이템 리스트에서 삭제한다.				
	DWORD		DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity);								// 아이템을 아이템 리스트에서 삭제한다.				
	DWORD		DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory);		// 아이템을 아이템 리스트에서 삭제한다.
	BOOL		GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem);								// 아이템을 얻어온다.	
	MyItemData*	GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory);							// 아이템을 얻어온다.
	MyItemData*	GetItemByID(UI08 uiFollowerSlot, UI16 uiItemID);											// 아이템을 얻어온다.
	BOOL		GetItemByPos(UI08 uiFollowerSlot, SI16 siPosInInventory, MyItemData **ppItemData );			// 아이템을 얻어온다.
	BOOL		HasItem( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity );		// 아이템을 가지고 있는지 검사한다.
	SI32		GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity );		// 아이템을 가지고 있는지 검사한다.
	SI32		GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID);		// 아이템을 가지고 있는지 검사한다.
	BOOL		IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory);						// 아이템이 있는지 검사
	BOOL		IsSameItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory);	// 같은 아이템이 있는지 검사.
	MyItemData*	GetItemList(UI08 uiFollowerSlot);										// 아이템리스트를 돌려준다.
	//BOOL		WearItem(UI08 uiFollowerSlot, UI16	uiItemID);
	BOOL		StripItem(UI08	uiFollowerSlot, SI16 siPosInInventory);
	BOOL		TakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);		// 장착하고 있는 아이템을 벗는다.
	BOOL		PutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);			// 인벤토리에 있는 아이템을 장착한다.
	BOOL		DisAppearItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData);
	BOOL		CheckWeight(UI08 uiFollowerSlot, UI32 Weight);
	BOOL		IsOverWeight();															// 무게초과인 용병이 한명이라도 존재하여 걷기 힘든 상태인지 판단
	BOOL		CheckItem(UI16 uiItemID, SI32 siCount);									// 아이템이 있는지 검사
	SI32		GetItemCount(UI16 uiItemID);											// 가지고 있는 Item의 수를 얻어온다.
	BOOL		GetWearItems(UI08 uiFollowerSlot, UI16 puiWearItems[WEAR_ITEM_NUM]);	// 현재 장착된 아이템들의 리스트를 얻어온다.
	BOOL		SetWearItem2(UI08 uiFollowerSlot, UI08 uiLoc, UI16 uiItemID);			// 아이템을 장착한다.
	
	BOOL		AllCharacterDeleteLife(SI32 DeleteLife);							// 모든 Character의 Life를 뺀다.
	BOOL		AllCharacterDeleteLifePoint(SI32 DeleteLife);						// 모든 Character의 Life를 뺀다.
	BOOL		AllCharacterDeleteLifePercent(SI32 DeleteLifeP);					// 모든 Character의 Life를 %만큼 뺀다.
	UI08		GetFollowerNum();													// 용병의 수를 얻어온다.
	DWORD		GetBattleInfo(UI08 uiFollowerSlot, OnImjinBattleFollowerInfo *pFollowerInfo);	// 전투 정보를 얻어온다	
	SI32		GetUseSatietyWarAfter(void);										// 전투후 사용할 포만감 수치 검사

	BOOL		CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory);			// 아이템을 추가 시킬 수 있느냐?
	BOOL		CanAddItem(UI08 uiFollowerSlot, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum);				// 아이템을 추가 시킬 수 있느냐?
	BOOL		CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity );								// 아이템을 추가 시킬 수 있느냐?

	BOOL		CanPutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode);		// 인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.
	BOOL		CanTakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory);						// 인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.

	BOOL		CanMoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory );	// 아이템을 이동 시킬 수 있는지 알아온다.
	BOOL		MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 *psiPosInInventory );	// 아이템을 이동 시킨다.


	BOOL		CanAddFollower(UI08 uiAddFollowerNum);														// 용병을 추가 시킬 수 있느냐?
	DWORD		GetTotalItemWeight(UI08 uiFollowerSlot);													// 현재 이 캐릭터의 전체 아이템의 무게를 얻어온다.	
	DWORD		GetMaxWeight(UI08 uiFollowerSlot);															// 이 캐릭터가 최고 들 수 있는 무게.
	UI16		GetItemQuantityToTake(UI08 uiFollowerSlot, UI16 uiItemID);									// 이 캐릭터가 최고 가질 수 있는 아이템의 수량을 얻어온다.
	BOOL		IsItemWeightOverflowed();																	// 용병들 중에 자기가 수용할 수 있는것 보다 아이템을 초과 해서 드는지 얻어온다.
	BOOL		IsItemWeightOverflowed(UI08 uiFollowerSlot);												// 특정 용병이 자기가 수용할 수 있는것 보다 아이템을 초과 해서 드는지 얻어온다.

	BOOL		CanWeaponBeUpgraded( UI16 uiFollowerSlot );										// 업그레이드 가능한 무기인지 알아온다. (고급무기)
	VOID		UpgradeWeapon( UI16 uiFollowerSlot, UI16 uiItemCode );							// 장착 무기 아이템을 특정 아이템으로 업그레이드 시켜준다.
	VOID		BrakeWeapon( UI16 uiFollowerSlot );												// 장착중인 무기 아이템을 파괴시킨다.

	VOID		SetFullLifeOfAllChar();										// 모든 용병들의 체력을 100% 회복시킨다.
	VOID		SetFullManaOfAllChar();										// 모든 용병들의 마나를 100% 회복시킨다.

	static	VOID	InitStaticVariable(CItem* pItem);								// 정적 변수 초기화.
};

#endif

