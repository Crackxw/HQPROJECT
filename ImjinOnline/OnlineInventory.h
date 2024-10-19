//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 인벤토리
//	File Name		: OnlineInventory.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: 정 진 욱 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		인벤토리 표시, 처리, 작업 등을 총괄하는 클래스다.
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
	cltOnlineWorld				*m_pMyOnlineWorld;					// 온라인 월드 클래스.
	BOOL						m_bIsVillage;						// 현재 마을에 존재하는가?
	BOOL						m_bIsAction;						// 이 클래스가 가동중인가?

	XSPR						m_MainImage, m_MOverImage, m_MBut1Image, m_MBut2Image, m_MHlightImage, m_MWearImage;

	_Button						m_BWearItem, m_BItemVillage, m_BSelFollower, m_BExit;

	SI16						m_siX;								// 전체 화면상에서 인벤토리 패널의 시작좌표 X
	SI16						m_siY;								// 전체 화면상에서 인벤토리 패널의 시작좌표 Y
	SI16						m_siWidth;							// 전체 화면상에서 인벤토리 패널의 너비
	SI16						m_siHeight;							// 전체 화면상에서 인벤토리 패널의 높이
	SI16						m_hNoField;							// 인벤토리 패널의 마우스 필드 신호 방지값의 핸들

	SI32						m_siMyUniqueID;						// 이 인벤토리를 소유하고 있는 캐릭터의 UniqueID.	
	SI16						m_siPortraitFontNo;					// 출력될 초상화 폰트
	UI08						m_uiSelectedSlot;					// 현재 선택된 용병슬롯

	SI32						m_siPollStatus;						// 폴의 현재 상태.
	SI32						m_siLocalStatus;					// 현재 게임중의 표기되는 작동 상태
 
	BOOL						m_bPushMouseL;						// 왼쪽 마우스 누름 상태.
	BOOL						m_bPushMouseR;						// 마우스 우클릭
	BOOL						m_bIsItemPickingUp;					// 아이템 줍는 상태인지
	BOOL						m_bIsItemMoving;					// 아이템 이동 상황인지

	SI16						m_siWearSettingType;				// 무기를 입고 벗을 때 갑옷이나 무기 등의 타입을 넣어 준다.

	vector<OnlineListControl*>	m_ListControl;						// 인벤토리에 들어있는 모든 아이템을 관장하는 벡터
	OnlineListControl			*m_pLCItemPointer;					// 장착용 아이템을 가리키는 임시 포인터

	OnlineListControl			*m_pLCItemInventory;				// 인벤토리에 보관중인 아이템들 ( 5 X 2 )
	OnlineListControl			*m_pLCWearItemWeapon;				// 장착용 아이템 이다.(무기)
	OnlineListControl			*m_pLCWearItemArmor;				// 장착용 아이템 이다.(갑옷)
	OnlineListControl			*m_pLCWearItemAccessoryL;			// 장착용 아이템 이다.(장신구좌)	
	OnlineListControl			*m_pLCWearItemAccessoryR;			// 장착용 아이템 이다.(장신구우)
	OnlineListControl			*m_pLCWearItemHelmet;				// 장착용 아이템 이다.(투구)
	OnlineListControl			*m_pLCWearItemBelt;					// 장착용 아이템 이다.(허리띠)
	OnlineListControl			*m_pLCWearItemGloves;				// 장착용 아이템 이다.(장갑)
	OnlineListControl			*m_pLCWearItemBoots;				// 장착용 아이템 이다.(부츠)	
	OnlineListControl			*m_pLCWearItemGuardian;				// 장착용(리스트 컨트롤 확장) 아이템 이다.(수호부)
	

	SI16						m_siEnteringBuildingType;			// 마을 안 들어간 건물의 인덱스

	const ItemInfo				*m_pItemSlotPointer;				// 선택한 아이템의 슬롯을 가리키는 임시 포인터
	SI16						m_bIsItemDragging;					// 아이템을 선택하여 드래그 중이라면 true(1), 아니라면 false(0)

	UI16						m_uiThrowItemID;					// 버리는 아이템의 ID.
	SI16						m_siThrowItemIndex;					// 버리는 아이템의 인덱스.
	UI32						m_uiThrowItemQuantity;				// 버리는 아이템의 수량.
	UI08						m_uiThrowItemSrcFollower;			// 버리는 아이템을 가지고 있는 용병의 DB 슬롯 번호

	UI16						m_uiPickUpItemID;					// 주은 아이템의 ID
	UI32						m_uiPichUpItemQuantity;				// 주은 아이템의 수량
	UI08						m_uiPickUpItemDestFollower;			// 주은 아이템을 받을 용병의 DB 슬롯 번호
	SI16						m_siPickUpItemDestSlot;				// 주은 아이템이 들어갈 인벤토리 칸의 슬롯 위치
	SI16						m_siPickUpItemMapPosX;				// 주은 아이템의 맵상 위치 X
	SI16						m_siPickUpItemMapPosY;				// 주은 아이템의 맵상 위치 Y
	
	UI16						m_uiMoveItemID;						// 이동중인 아이템의 ID
	SI16						m_siMoveItemIndex;					// 이동중인 아이템의 인덱스
	UI08						m_uiMoveItemSrcFollower;			// 이동중인 아이템의 원래 소유 용병
	UI08						m_uiMoveItemDestFollower;			// 이동중인 아이템을 소유할 용병
	SI16						m_siMoveItemSlotBefore;				// 이동중인 아이템이 들어있던 인벤토리 칸의 슬롯 번호
	SI16						m_siMoveItemSlotAfter;				// 이동중인 아이템이 들어갈 인벤토리 칸의 슬롯 번호

	UI16						m_uiUseItemID;						// 사용하는 아이템의 아이디
	UI08						m_uiUseItemDestFollower;			// 사용하는 아이템의 대상 용병의 DB 슬롯 번호
	SI16						m_siUseItemSrcSlot;					// 사용하는 아이템이 들어있는 인벤토리 칸의 슬롯 번호
	SI16						m_siUseQuantity;					// 사용하는 아이템의 갯수.(소집권 대장)의 경우 2개가 사용된다.

	UI16						m_uiWearTimerItemID;				// 장착하려는 시간제 아이템의 아이디
	UI08						m_uiWearTimerItemDestFollower;		// 장착하려는 시간제 아이템의 대상 용병
	SI16						m_siWearTimerItemDestSlot;			// 장착하려는 시간제 아이템의 대상 슬롯 위치
	SI16						m_siWearTimerItemSettingType;		// 장착하려는 시간제 아이템의 장착 타입

	SI16						m_siItemMovingDelay;				// 아이템을 움직이거나 할 때의 루프 여유 카운트
	BOOL						m_bMsgBoxFlag_VanishFieldMercenary;	// [메시지박스 플래그] 단군의 거울에 의한 필드 용병을 제거할 것인가를 묻는 메시지 박스가 떴느냐 아니냐?
	BOOL						m_bMsgBoxFlag_WearTimerItem;		// [메시지박스 플래그] 시간제 아이템 장착에 대한 메시지가 떴는가?

	OnlineItemList				*m_pOnlineItemList;					// 장착 가능한 아이템 리스트 클래스

public:
	OnlineInventory();
	~OnlineInventory();

	VOID	Init				( SI32 siID, cltOnlineWorld	*pOnlineWorld );								// 초기화	
	VOID	Free				( void );																	// 인벤토리에서 해제 해야 될것을 해제한다.
	VOID	Initialize			( void );
	VOID	Action				( void );																	// 인벤토리의 Action
	VOID	Draw				( LPDIRECTDRAWSURFACE7 pSurface );											// 인벤토리의 Draw.

	VOID	SetActive			( BOOL bActive );															// 활성화 시킬것인지의 여부.
	BOOL	IsActive			( void )			{ return m_bIsAction; }									// 활성화되어 있는가?
	VOID	FillItemListBox		( void );																	// 아이템 리스트 박스에 아이템을 넣는다.	
	SI32	GetThrowItemIndex	( void );																	// 땅바닥에 버릴 아이템의 인덱스를 가져온다.
	VOID	SetThrowItemInfo	( SI16 siItemIndex ){ m_siThrowItemIndex = siItemIndex;	}					// 버리는 아이템 인덱스를 세팅한다.
	SI32	GetThrowItemInfo	( void )			{ return m_siThrowItemIndex; }							// 버리는 아이템 인덱스를 받는다.
	SI32	GetDragItemInfo		( void )			{ return m_pLCItemInventory->GetDragSlot(); }			// Drag 중인 아이템 인덱스를 받는다.

	VOID	SetFlagVillage		( BOOL fVillage )	{ m_bIsVillage = fVillage; }							// 마을에 들어왔을 경우 true, 아니라면 false
	VOID	SetFieldItemDrag	( UI16 uiItemID, SI16 siItemX, SI16 siItemY );								// 끄집어 올린 필드 아이템의 정보를 세팅한다.
	
	VOID	SetItemPickUp		( BOOL bFlag );																// 아이템을 들어올린 상태로 세팅한다.
	BOOL	GetItemPickUp		( void )			{ return m_bIsItemPickingUp; };							// 아이템을 들어올렸냐 아니냐?
	UI16	GetItemPickUpID		( void )			{ return m_uiPickUpItemID; }							// 현재 들어올린 아이템의 ID를 받는다.
	VOID	GetItemPickUpMapPos	( POINT &ptPos );															// 필드에서 아이템을 들어올린 좌표의 위치를 리턴한다.
	
	BOOL	IsItemMove			( void )			{ return m_bIsItemMoving; };							// 아이템을 이동중이냐 아니냐?
	BOOL	IsWearDrag			( void );																	// 현재 입고 있는 장비나 아이템을 드래그중에 있는가?
	UI32	GetDragInfo			( SI16 &siSelectLB, SI16 &siPos );											// 현재 인벤토리 칸으로부터 드래그중인 아이템의 정보를 리턴한다.
	VOID	DragFail			( void );																	// 인벤토리 칸으로부터 드래그 중인 작업을 중지시킨다.
	VOID	StopDrag			( void );																	// 모든 드래그 작업을 중지시키고 원래 정보를 갱신한다.
	SI16	GetDragItemNum		( UI32 uiID );																// 드래그 중인 아이템이 총수를 리턴한다.

	VOID	SetInStruct			( SI16 siFlag )		{ m_siEnteringBuildingType	=	siFlag; }				// 마을 안에서 들어간 건물의 인덱스를 설정한다.
	VOID	SetMercenarySlot	( UI08 uiDBSlot );															// 현재 선택한 용병의 DB 슬롯 번호를 세팅한다.
	UI08	GetMercenarySlot	( void )			{ return (UI08)m_uiSelectedSlot; }						// 현재 선택된 용병의 DB 슬롯 번호를 리턴한다.
	SI32	GetWeight			( UI08 uiFollower );														// 리스트 컨트롤에 있는 아템 무게 합	

	SI32	GetStatus			( void )			{ return m_siPollStatus; };								// 현재 폴의 진행 상황을 받는다.
	SI16	GetWidth			( void )			{ return m_bIsAction == FALSE ? 0 : (m_siWidth-3); };	// 인벤토리의 폭을 받는다.
	VOID	SetExitButton		( BOOL bActive )	{ m_BExit.SetAction(bActive); };						// 닫기 버튼의 활성화 유무를 설정한다.

	OnlineListControl	*GetLCInventory	( void )	{ return m_pLCItemInventory; };							// 인벤토리칸의 리스트 컨트롤 클래스를 리턴한다.
	OnlineItemList		*GetItemList	( void )	{ return m_pOnlineItemList; };							// 아이템 정보가 들어있는 아이템 리스트 박스 클래스를 리턴한다.

private:
	BOOL	AttachItem			( void );																	// 드래그중인 아이템을 옮겨놓는 작업을 한다.
	BOOL	StripItem			( void );																	// 입고 있던 아이템을 벗는 작업을 한다.
	VOID	CheckUseItem		( void );																	// 아이템 사용에 대한 작업 진행의 중간 개체(사용 확인이나 기타 등등)
	VOID	ErrorMsg			( SI08 ErrorCode );															// 에러메시지 출력 함수

	//----------------------------------------------------------------------------
	//	서버로부터 받은 메시지의 처리
	//----------------------------------------------------------------------------
public:
	void	WearItem			( BYTE *pMsg );		// 아이템의 장착
	void	StripItem			( BYTE *pMsg );		// 아이템의 탈의
	void	MoveItem			( BYTE *pMsg );		// 아이템의 이동
	void	ThrowItem			( BYTE *pMsg );		// 아이템의 버리기
	void	PickupItem			( BYTE *pMsg );		// 아이템의 줍기
	void	SpendItem			( BYTE *pMsg );		// 아이템의 소비
	void    AddFollower			( BYTE *pMsg );		// 용병의 장착
};

#endif