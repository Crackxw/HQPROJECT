//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �κ��丮
//	File Name		: OnlineInventory.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�κ��丮 ǥ��, ó��, �۾� ���� �Ѱ��ϴ� Ŭ������.
//
//===================================================================================================

#ifndef	_ONLINE_INVENTORY_H
#define _ONLINE_INVENTORY_H

//#include "Online.h"
//#include "OnlineBaseLB.h"
//#include "Button.h"
//#include "OnlineItemList.h"


#define ON_STRUCT_MARKET	1
#define ON_STRUCT_IN		2
#define ON_STRUCT_BANK		3

//#include <iostream>
//#include <vector>

using namespace std;

class	OnlineInventory
{
private:
	cltOnlineWorld				*m_pMyOnlineWorld;					// �¶��� ���� Ŭ����.
	BOOL						m_bIsVillage;						// ���� ������ �����ϴ°�?
	BOOL						m_bIsAction;						// �� Ŭ������ �������ΰ�?

	XSPR						m_MainImage, m_MOverImage, m_MBut1Image, m_MBut2Image, m_MHlightImage, m_MWearImage;

	_Button						m_BWearItem, m_BItemVillage, m_BSelFollower, m_BExit;

	SI16						m_siX;								// ��ü ȭ��󿡼� �κ��丮 �г��� ������ǥ X
	SI16						m_siY;								// ��ü ȭ��󿡼� �κ��丮 �г��� ������ǥ Y
	SI16						m_siWidth;							// ��ü ȭ��󿡼� �κ��丮 �г��� �ʺ�
	SI16						m_siHeight;							// ��ü ȭ��󿡼� �κ��丮 �г��� ����
	SI16						m_hNoField;							// �κ��丮 �г��� ���콺 �ʵ� ��ȣ �������� �ڵ�

	SI32						m_siMyUniqueID;						// �� �κ��丮�� �����ϰ� �ִ� ĳ������ UniqueID.	
	SI16						m_siPortraitFontNo;					// ��µ� �ʻ�ȭ ��Ʈ
	UI08						m_uiSelectedSlot;					// ���� ���õ� �뺴����

	SI32						m_siPollStatus;						// ���� ���� ����.
	SI32						m_siLocalStatus;					// ���� �������� ǥ��Ǵ� �۵� ����
 
	BOOL						m_bPushMouseL;						// ���� ���콺 ���� ����.
	BOOL						m_bPushMouseR;						// ���콺 ��Ŭ��
	BOOL						m_bIsItemPickingUp;					// ������ �ݴ� ��������
	BOOL						m_bIsItemMoving;					// ������ �̵� ��Ȳ����

	SI16						m_siWearSettingType;				// ���⸦ �԰� ���� �� �����̳� ���� ���� Ÿ���� �־� �ش�.

	vector<OnlineListControl*>	m_ListControl;						// �κ��丮�� ����ִ� ��� �������� �����ϴ� ����
	OnlineListControl			*m_pLCItemPointer;					// ������ �������� ����Ű�� �ӽ� ������

	OnlineListControl			*m_pLCItemInventory;				// �κ��丮�� �������� �����۵� ( 5 X 2 )
	OnlineListControl			*m_pLCWearItemWeapon;				// ������ ������ �̴�.(����)
	OnlineListControl			*m_pLCWearItemArmor;				// ������ ������ �̴�.(����)
	OnlineListControl			*m_pLCWearItemAccessoryL;			// ������ ������ �̴�.(��ű���)	
	OnlineListControl			*m_pLCWearItemAccessoryR;			// ������ ������ �̴�.(��ű���)
	OnlineListControl			*m_pLCWearItemHelmet;				// ������ ������ �̴�.(����)
	OnlineListControl			*m_pLCWearItemBelt;					// ������ ������ �̴�.(�㸮��)
	OnlineListControl			*m_pLCWearItemGloves;				// ������ ������ �̴�.(�尩)
	OnlineListControl			*m_pLCWearItemBoots;				// ������ ������ �̴�.(����)	
	OnlineListControl			*m_pLCWearItemGuardian;				// ������(����Ʈ ��Ʈ�� Ȯ��) ������ �̴�.(��ȣ��)
	

	SI16						m_siEnteringBuildingType;			// ���� �� �� �ǹ��� �ε���

	const ItemInfo				*m_pItemSlotPointer;				// ������ �������� ������ ����Ű�� �ӽ� ������
	SI16						m_bIsItemDragging;					// �������� �����Ͽ� �巡�� ���̶�� true(1), �ƴ϶�� false(0)

	UI16						m_uiThrowItemID;					// ������ �������� ID.
	SI16						m_siThrowItemIndex;					// ������ �������� �ε���.
	UI32						m_uiThrowItemQuantity;				// ������ �������� ����.
	UI08						m_uiThrowItemSrcFollower;			// ������ �������� ������ �ִ� �뺴�� DB ���� ��ȣ

	UI16						m_uiPickUpItemID;					// ���� �������� ID
	UI32						m_uiPichUpItemQuantity;				// ���� �������� ����
	UI08						m_uiPickUpItemDestFollower;			// ���� �������� ���� �뺴�� DB ���� ��ȣ
	SI16						m_siPickUpItemDestSlot;				// ���� �������� �� �κ��丮 ĭ�� ���� ��ġ
	SI16						m_siPickUpItemMapPosX;				// ���� �������� �ʻ� ��ġ X
	SI16						m_siPickUpItemMapPosY;				// ���� �������� �ʻ� ��ġ Y
	
	UI16						m_uiMoveItemID;						// �̵����� �������� ID
	SI16						m_siMoveItemIndex;					// �̵����� �������� �ε���
	UI08						m_uiMoveItemSrcFollower;			// �̵����� �������� ���� ���� �뺴
	UI08						m_uiMoveItemDestFollower;			// �̵����� �������� ������ �뺴
	SI16						m_siMoveItemSlotBefore;				// �̵����� �������� ����ִ� �κ��丮 ĭ�� ���� ��ȣ
	SI16						m_siMoveItemSlotAfter;				// �̵����� �������� �� �κ��丮 ĭ�� ���� ��ȣ

	UI16						m_uiUseItemID;						// ����ϴ� �������� ���̵�
	UI08						m_uiUseItemDestFollower;			// ����ϴ� �������� ��� �뺴�� DB ���� ��ȣ
	SI16						m_siUseItemSrcSlot;					// ����ϴ� �������� ����ִ� �κ��丮 ĭ�� ���� ��ȣ
	SI16						m_siUseQuantity;					// ����ϴ� �������� ����.(������ ����)�� ��� 2���� ���ȴ�.

	UI16						m_uiWearTimerItemID;				// �����Ϸ��� �ð��� �������� ���̵�
	UI08						m_uiWearTimerItemDestFollower;		// �����Ϸ��� �ð��� �������� ��� �뺴
	SI16						m_siWearTimerItemDestSlot;			// �����Ϸ��� �ð��� �������� ��� ���� ��ġ
	SI16						m_siWearTimerItemSettingType;		// �����Ϸ��� �ð��� �������� ���� Ÿ��

	SI16						m_siItemMovingDelay;				// �������� �����̰ų� �� ���� ���� ���� ī��Ʈ
	BOOL						m_bMsgBoxFlag_VanishFieldMercenary;	// [�޽����ڽ� �÷���] �ܱ��� �ſ￡ ���� �ʵ� �뺴�� ������ ���ΰ��� ���� �޽��� �ڽ��� ������ �ƴϳ�?
	BOOL						m_bMsgBoxFlag_WearTimerItem;		// [�޽����ڽ� �÷���] �ð��� ������ ������ ���� �޽����� ���°�?

	OnlineItemList				*m_pOnlineItemList;					// ���� ������ ������ ����Ʈ Ŭ����

public:
	OnlineInventory();
	~OnlineInventory();

	VOID	Init				( SI32 siID, cltOnlineWorld	*pOnlineWorld );								// �ʱ�ȭ	
	VOID	Free				( void );																	// �κ��丮���� ���� �ؾ� �ɰ��� �����Ѵ�.
	VOID	Initialize			( void );
	VOID	Action				( void );																	// �κ��丮�� Action
	VOID	Draw				( LPDIRECTDRAWSURFACE7 pSurface );											// �κ��丮�� Draw.

	VOID	SetActive			( BOOL bActive );															// Ȱ��ȭ ��ų�������� ����.
	BOOL	IsActive			( void )			{ return m_bIsAction; }									// Ȱ��ȭ�Ǿ� �ִ°�?
	VOID	FillItemListBox		( void );																	// ������ ����Ʈ �ڽ��� �������� �ִ´�.	
	SI32	GetThrowItemIndex	( void );																	// ���ٴڿ� ���� �������� �ε����� �����´�.
	VOID	SetThrowItemInfo	( SI16 siItemIndex ){ m_siThrowItemIndex = siItemIndex;	}					// ������ ������ �ε����� �����Ѵ�.
	SI32	GetThrowItemInfo	( void )			{ return m_siThrowItemIndex; }							// ������ ������ �ε����� �޴´�.
	SI32	GetDragItemInfo		( void )			{ return m_pLCItemInventory->GetDragSlot(); }			// Drag ���� ������ �ε����� �޴´�.

	VOID	SetFlagVillage		( BOOL fVillage )	{ m_bIsVillage = fVillage; }							// ������ ������ ��� true, �ƴ϶�� false
	VOID	SetFieldItemDrag	( UI16 uiItemID, SI16 siItemX, SI16 siItemY );								// ������ �ø� �ʵ� �������� ������ �����Ѵ�.
	
	VOID	SetItemPickUp		( BOOL bFlag );																// �������� ���ø� ���·� �����Ѵ�.
	BOOL	GetItemPickUp		( void )			{ return m_bIsItemPickingUp; };							// �������� ���÷ȳ� �ƴϳ�?
	UI16	GetItemPickUpID		( void )			{ return m_uiPickUpItemID; }							// ���� ���ø� �������� ID�� �޴´�.
	VOID	GetItemPickUpMapPos	( POINT &ptPos );															// �ʵ忡�� �������� ���ø� ��ǥ�� ��ġ�� �����Ѵ�.
	
	BOOL	IsItemMove			( void )			{ return m_bIsItemMoving; };							// �������� �̵����̳� �ƴϳ�?
	BOOL	IsWearDrag			( void );																	// ���� �԰� �ִ� ��� �������� �巡���߿� �ִ°�?
	UI32	GetDragInfo			( SI16 &siSelectLB, SI16 &siPos );											// ���� �κ��丮 ĭ���κ��� �巡������ �������� ������ �����Ѵ�.
	VOID	DragFail			( void );																	// �κ��丮 ĭ���κ��� �巡�� ���� �۾��� ������Ų��.
	VOID	StopDrag			( void );																	// ��� �巡�� �۾��� ������Ű�� ���� ������ �����Ѵ�.
	SI16	GetDragItemNum		( UI32 uiID );																// �巡�� ���� �������� �Ѽ��� �����Ѵ�.

	VOID	SetInStruct			( SI16 siFlag )		{ m_siEnteringBuildingType	=	siFlag; }				// ���� �ȿ��� �� �ǹ��� �ε����� �����Ѵ�.
	VOID	SetMercenarySlot	( UI08 uiDBSlot );															// ���� ������ �뺴�� DB ���� ��ȣ�� �����Ѵ�.
	UI08	GetMercenarySlot	( void )			{ return (UI08)m_uiSelectedSlot; }						// ���� ���õ� �뺴�� DB ���� ��ȣ�� �����Ѵ�.
	SI32	GetWeight			( UI08 uiFollower );														// ����Ʈ ��Ʈ�ѿ� �ִ� ���� ���� ��	

	SI32	GetStatus			( void )			{ return m_siPollStatus; };								// ���� ���� ���� ��Ȳ�� �޴´�.
	SI16	GetWidth			( void )			{ return m_bIsAction == FALSE ? 0 : (m_siWidth-3); };	// �κ��丮�� ���� �޴´�.
	VOID	SetExitButton		( BOOL bActive )	{ m_BExit.SetAction(bActive); };						// �ݱ� ��ư�� Ȱ��ȭ ������ �����Ѵ�.

	OnlineListControl	*GetLCInventory	( void )	{ return m_pLCItemInventory; };							// �κ��丮ĭ�� ����Ʈ ��Ʈ�� Ŭ������ �����Ѵ�.
	OnlineItemList		*GetItemList	( void )	{ return m_pOnlineItemList; };							// ������ ������ ����ִ� ������ ����Ʈ �ڽ� Ŭ������ �����Ѵ�.

private:
	BOOL	AttachItem			( void );																	// �巡������ �������� �Űܳ��� �۾��� �Ѵ�.
	BOOL	StripItem			( void );																	// �԰� �ִ� �������� ���� �۾��� �Ѵ�.
	VOID	CheckUseItem		( void );																	// ������ ��뿡 ���� �۾� ������ �߰� ��ü(��� Ȯ���̳� ��Ÿ ���)
	VOID	ErrorMsg			( SI08 ErrorCode );															// �����޽��� ��� �Լ�

	//----------------------------------------------------------------------------
	//	�����κ��� ���� �޽����� ó��
	//----------------------------------------------------------------------------
public:
	void	WearItem			( BYTE *pMsg );		// �������� ����
	void	StripItem			( BYTE *pMsg );		// �������� Ż��
	void	MoveItem			( BYTE *pMsg );		// �������� �̵�
	void	ThrowItem			( BYTE *pMsg );		// �������� ������
	void	PickupItem			( BYTE *pMsg );		// �������� �ݱ�
	void	SpendItem			( BYTE *pMsg );		// �������� �Һ�
	void    AddFollower			( BYTE *pMsg );		// �뺴�� ����
};

#endif