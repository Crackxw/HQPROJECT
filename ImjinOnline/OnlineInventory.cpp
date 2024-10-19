//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 인벤토리
//	File Name		: OnlineInventory.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		인벤토리 표시, 처리, 작업 등을 총괄하는 클래스다.
//
//===================================================================================================


//-----------------------------------------------------------------------------
// Name: 
// Code: All (2004-06-07)
// Desc: 헤더 및 정의 선언
//-----------------------------------------------------------------------------
#include <GSL.h>

#include	<Mouse.h>
#include	<OnlineWorld.h>
#include	<kindinfo.h>

#include <vector>

#include	"OnlineListControl.h"
#include	"OnlineItemList.h"
#include	"OnlinePortrait.h"		// actdoll (2004/01/05 18:31) : 초상관련
#include	"OnlineInventory.h"
#include	"OnlineMegaText.h"
#include	"OnlineHelp.h"
#include	"OnlineNumberControl.h"
#include	"OnlineTrade.h"
#include	"OnlineTradeBook.h"
#include	"OnlineMiniMap.h"
#include	"OnlineFieldArea.h"
#include	"OnlineCharUI.h"
#include	"OnlineBooth.h"
#include    "OnlineFieldAuction.h"
#include    "OnlineSmith.h"
#include	"OnlineMercenary-Parser.h"
#include	"Onlineetc.h"
#include	"..\Gersang\Music.h"
#include	"OnlineMercenary.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 

const	R32		DF_VALUE_CHAR_MOVE_SPEED_IN_SPECIAL_BOOTS	= 1.5f;		// 시간제 신발 아이템 착용 시 이동 속도(기존의 1.5배)
// H2-S-05.02.02 //
// 인베토리 활성화 될때의 미니맵 정보 저장 변수 //
BOOL	g_fMiniMapAct = FALSE;
// H2-E-05.02.02 //
enum
{   
	ON_INVENTORY_STATUS_NOACTION		= 0,
//	ON_INVENTORY_STATUS_ITEMVERIFY,
//	ON_INVENTORY_STATUS_RECEIVEMYITEMLIST,
	ON_INVENTORY_STATUS_MAIN_INITPOLL,
	ON_INVENTORY_STATUS_MAIN_POLL,
	ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW,
	ON_INVENTORY_STATUS_MAIN_ITEMTHROW,
	ON_INVENTORY_STATUS_RESPONSEITEMMOVE,
	ON_INVENTORY_STATUS_MAIN_INIT_ITEMPICKUP,
	ON_INVENTORY_STATUS_MAIN_ITEMPICKUP,
	ON_INVENTORY_STATUS_ATTACH_ITEM,
	ON_INVENTORY_STATUS_STRIP_ITEM,
	ON_INVENTORY_STATUS_MOVE_ITEM,
	ON_INVENTORY_STATUS_MOVE_ITEMSEND,
	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY,
	ON_INVENTORY_STATUS_MAIN_INPUT_ITEMPICKUP,
	ON_INVENTORY_STATUS_INIT_TRADE,
	ON_INVENTORY_STATUS_ITEMTHROW,
	ON_INVENTORY_STATUS_USE_ITEM_WAIT,
	ON_INVENTORY_STATUS_WEARITEMLIST_SELECT,
	ON_INVENTORY_STATUS_VILLAGELIST_SELECT,
	ON_INVENTORY_STATUS_LISTINFO_SHOW
};


#define ON_LOCAL_STATUS_USE_ITEM_POLL				0
#define ON_LOCAL_STATUS_USE_ITEM_OKNO				1
#define ON_LOCAL_STATUS_USE_ITEM_SEND				2


#define ON_INVENTORY_MAINBOARD_X			457
#define ON_INVENTORY_MAINBOARD_Y			45





//-----------------------------------------------------------------------------
// Name: OnlineInventory()
// Code: actdoll (2004-06-07)
// Desc: 생성자
//-----------------------------------------------------------------------------
OnlineInventory::OnlineInventory()
{	
	m_pMyOnlineWorld					=	NULL;
	m_bIsVillage						=	FALSE;
	m_bIsAction							=	FALSE;
	
	

	m_siMyUniqueID						=	0;	
	m_hNoField							=	-1;
	m_bIsAction							=	FALSE;
	m_bPushMouseL						=	FALSE;	
	m_bPushMouseR						=	FALSE;
	m_bIsItemPickingUp					=	FALSE;
	m_siEnteringBuildingType			=	0;
//	MaxWeight							=	0;
	m_uiSelectedSlot					=	0;
//	dwDelay								=	0;
	m_bIsItemDragging					=	0;
	m_siItemMovingDelay					=	0;
	m_pLCItemPointer					=	NULL;
	m_pItemSlotPointer					=	NULL;

	m_uiPickUpItemID					=	0;
	m_siPickUpItemMapPosX				=	0;
	m_siPickUpItemMapPosY				=	0;
	m_siPollStatus						=	ON_INVENTORY_STATUS_NOACTION;
	m_siMoveItemIndex					=	-1;

	m_uiWearTimerItemID					=	0;
	m_uiWearTimerItemDestFollower		=	0;
	m_siWearTimerItemDestSlot			=	-1;
	m_siWearTimerItemSettingType		=	-1;
	
	m_bIsItemMoving						=	FALSE;
	m_bMsgBoxFlag_VanishFieldMercenary	=	FALSE;
	m_bMsgBoxFlag_WearTimerItem			=	FALSE;

	m_pOnlineItemList					=	NULL;
}

//-----------------------------------------------------------------------------
// Name: ~OnlineInventory()
// Code: actdoll (2004-06-07)
// Desc: 소멸자
//-----------------------------------------------------------------------------
OnlineInventory::~OnlineInventory()
{
	
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-06-07)
// Desc: 처음 시작 시 가동부
//		siID			- 서버에서의 내 어카운트 번호
//		pOnlineWorld	- OnlineWorld 구조체
//-----------------------------------------------------------------------------
VOID	OnlineInventory::Init(SI32 siID, cltOnlineWorld	*pOnlineWorld)
{
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main.Spr", m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_over.Spr", m_MOverImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr", m_MBut1Image);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button2.Spr", m_MBut2Image);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_hlight.Spr", m_MHlightImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_wear.Spr", m_MWearImage);

	m_siMyUniqueID			=	siID;
	m_siPollStatus			=	ON_INVENTORY_STATUS_NOACTION;
	m_siLocalStatus			=	0;
	m_pMyOnlineWorld		=	pOnlineWorld;
	m_bPushMouseL			=	FALSE;
	
	m_siWidth				=	m_MainImage.Header.Xsize;
	m_siHeight				=	m_MainImage.Header.Ysize;
	m_siX					=	ON_INVENTORY_MAINBOARD_X;
	m_siY					=	ON_INVENTORY_MAINBOARD_Y;

	m_pOnlineItemList		= new OnlineItemList(pOnlineWorld);
	m_pOnlineItemList->Init();

	if( !m_ListControl.empty() )
	{
		for( vector<OnlineListControl*>::iterator i = m_ListControl.begin(); i != m_ListControl.end(); ++i )
		{
			delete (*i);
			(*i) = NULL;
		}

		m_ListControl.clear();
	}

	// 케릭터 정보를 받는다.
	m_siPortraitFontNo	=	m_pMyOnlineWorld->pIOnlineChar->GetImjinKind(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID());
	m_siPortraitFontNo	=	KI[m_siPortraitFontNo].GetOnlinePortraitFont();

	// 숫자판을 초기화
	m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();

	// 아이템 리스트 박스를 생성한다.
	m_pLCItemInventory	= new OnlineListControl(pOnlineWorld);
	m_pLCItemInventory->Init(60, 60, 5, 2, m_siX + 10, m_siY + 235, 0);

	m_pLCWearItemWeapon		= new OnlineListControl(pOnlineWorld);	
	m_pLCWearItemArmor		= new OnlineListControl(pOnlineWorld);	
	m_pLCWearItemAccessoryR	= new OnlineListControl(pOnlineWorld);	
	m_pLCWearItemAccessoryL	= new OnlineListControl(pOnlineWorld);	
	m_pLCWearItemHelmet		= new OnlineListControl(pOnlineWorld);	
	m_pLCWearItemBelt		= new OnlineListControl(pOnlineWorld);		
	m_pLCWearItemGloves		= new OnlineListControl(pOnlineWorld);	
	m_pLCWearItemBoots		= new OnlineListControl(pOnlineWorld);
	m_pLCWearItemGuardian	= new OnlineListControl(pOnlineWorld);

	// 장착용 리스트 컨트롤을 생성한다.	

	m_pLCWearItemGuardian->Init		(60, 60, 1, 1, m_siX + 121, m_siY + 35, ON_ITEM_GUARDIAN);				// 수호부
	m_pLCWearItemHelmet->Init		(60, 60, 1, 1, m_siX + 185, m_siY + 35, ON_ITEM_HELMAT);				// 헬멧
	m_pLCWearItemGloves->Init		(60, 60, 1, 1, m_siX + 249, m_siY + 35, ON_ITEM_GLOVES);				// 장갑

	m_pLCWearItemWeapon->Init		(60, 60, 1, 1, m_siX + 121, m_siY + 99, ON_ITEM_SWORD);					// 무기
	m_pLCWearItemArmor->Init		(60, 60, 1, 1, m_siX + 185, m_siY + 99, ON_ITEM_ARMOR | ON_ITEM_DRESS);	// 갑옷
	m_pLCWearItemBelt->Init			(60, 60, 1, 1, m_siX + 249, m_siY + 99, ON_ITEM_BELT);					// 벨트	

	m_pLCWearItemAccessoryL->Init	(60, 60, 1, 1, m_siX + 121, m_siY + 163, ON_ITEM_RING | ON_ITEM_LRING);	// 좌반지
	m_pLCWearItemBoots->Init		(60, 60, 1, 1, m_siX + 185, m_siY + 163, ON_ITEM_BOOTS);				// 부츠
	m_pLCWearItemAccessoryR->Init	(60, 60, 1, 1, m_siX + 249, m_siY + 163, ON_ITEM_RING | ON_ITEM_RRING);	// 우반지

	// actdoll (2005/02/01 19:29) : 
	//	인벤토리 아이템 컨트롤의 순서와 ON_ITEM_...시리즈의 인덱스 위치를 일치하기 위해 순서대로 작업했다.
	//	점선 내에는 절대로 push 위치 바꾸지 말것!	=======================
	m_ListControl.push_back(m_pLCWearItemHelmet);
	m_ListControl.push_back(m_pLCWearItemWeapon);
	m_ListControl.push_back(m_pLCWearItemArmor);
	m_ListControl.push_back(m_pLCWearItemBelt);
	m_ListControl.push_back(m_pLCWearItemGloves);
	m_ListControl.push_back(m_pLCWearItemBoots);
	m_ListControl.push_back(m_pLCWearItemAccessoryR);
	m_ListControl.push_back(m_pLCWearItemAccessoryL);
	m_ListControl.push_back(m_pLCWearItemGuardian);
	//	점선 내에는 절대로 push 위치 바꾸지 말것!	=======================
	m_ListControl.push_back(m_pLCItemInventory);
		
	m_BSelFollower.Create(m_siX + 25, m_siY + 99, m_MBut2Image.Header.Xsize, m_MBut2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_FOLLOWER_NOT_SHOW), BUTTON_PUT_LEFT, FALSE);
	m_BWearItem.Create(m_siX + 25, m_siY + 187, m_MBut2Image.Header.Xsize, m_MBut2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000500), BUTTON_PUT_LEFT, TRUE);
	m_BItemVillage.Create(m_siX + 25, m_siY + 207, m_MBut2Image.Header.Xsize, m_MBut2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000501), BUTTON_PUT_LEFT, TRUE);
	m_BExit.Create(m_siX + 139, m_siY + 394, m_MBut1Image.Header.Xsize, m_MBut1Image.Header.Ysize+1, m_pMyOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE);

	m_uiSelectedSlot			= 0;
	m_uiPickUpItemID			= 0;
	m_siPickUpItemMapPosX		= 0;
	m_siPickUpItemMapPosY		= 0;

	// 각 각의 컨트롤위에 버튼을 만들어 마우스 상태를 체크한다.
	m_pLCItemInventory->StopDrag();
}
 
//-----------------------------------------------------------------------------
// Name: Free()
// Code: All (2004-06-07)
// Desc: 인벤토리 리소스 종료시 가동
//-----------------------------------------------------------------------------
VOID	OnlineInventory::Free()
{
	if(m_pOnlineItemList)
	{
		delete m_pOnlineItemList;
		m_pOnlineItemList = NULL;
	}

	if(!m_ListControl.empty())
	{
		for( vector<OnlineListControl*>::iterator i = m_ListControl.begin(); i != m_ListControl.end(); ++i )
		{
			delete (*i);
			(*i) = NULL;
		}

		m_ListControl.clear();
	}
	clGrp.FreeXspr(m_MainImage);
	clGrp.FreeXspr(m_MOverImage);
	clGrp.FreeXspr(m_MBut1Image);
	clGrp.FreeXspr(m_MBut2Image);
	clGrp.FreeXspr(m_MHlightImage);
	clGrp.FreeXspr(m_MWearImage);
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2004-06-07)
// Desc: 초기화
//-----------------------------------------------------------------------------
VOID	OnlineInventory::Initialize()
{
	SetActive(FALSE);

	m_siMyUniqueID						=	0;	
	m_hNoField							=	-1;
	m_bIsAction							=	FALSE;
	m_bPushMouseL						=	FALSE;	
	m_bPushMouseR						=	FALSE;
	m_bIsItemPickingUp					=	FALSE;
	m_siEnteringBuildingType			=	0;
//	MaxWeight							=	0;
	m_uiSelectedSlot					=	0;
//	dwDelay								=	0;
	m_bIsItemDragging					=	0;
	m_siItemMovingDelay					=	0;
				
	m_uiPickUpItemID					=	0;
	m_siPickUpItemMapPosX				=	0;
	m_siPickUpItemMapPosY				=	0;
	m_siMoveItemIndex					=	-1;
	m_siPollStatus						=	ON_INVENTORY_STATUS_NOACTION;
	m_siLocalStatus						=	0;

	m_bIsItemMoving						=	FALSE;
	m_bMsgBoxFlag_VanishFieldMercenary	=	FALSE;
	m_bMsgBoxFlag_WearTimerItem			=	FALSE;

	m_pOnlineItemList					=	NULL;
}

//-----------------------------------------------------------------------------
// Name: Action()
// Code: All (2004-06-07)
// Desc: 프로세스 동작부
//-----------------------------------------------------------------------------
VOID	OnlineInventory::Action()
{
	POINT			ptMouse;
	SI32			siSelectedSell;
	SI16			siThrowItemIndexOfLC;
	BOOL			IsSetInActive	=	FALSE;
	const ItemInfo* pItemInfo;

	if(IsActive() == FALSE && !m_bIsItemPickingUp)		return;

	ptMouse.x = IpD.Mouse_X;
	ptMouse.y = IpD.Mouse_Y;
	
	SI08 siMercenarySlot = m_pMyOnlineWorld->pMyData->GetMercenary();
	if(siMercenarySlot == m_uiSelectedSlot)
		m_BSelFollower.SetAction(TRUE);

	// 마우스가 인벤토리 안에 있다면 인벤토리에 맞게 마우스을 변경 한다.
	if(IpD.Mouse_X >= m_siX && IpD.Mouse_Y <= (m_siX + m_siWidth - 1) && IpD.Mouse_Y >= m_siY && IpD.Mouse_Y <= (m_siY + m_siHeight - 1))	
		m_pMyOnlineWorld->pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);		

	// 아이템 리스트 액션
	m_pOnlineItemList->Action();

	switch(m_siPollStatus)
	{
	case ON_INVENTORY_STATUS_MAIN_INITPOLL:
		m_BWearItem.SetAction(TRUE);
		m_BItemVillage.SetAction(TRUE);	
		m_siPollStatus	=	ON_INVENTORY_STATUS_MAIN_POLL;
		break;

	case ON_INVENTORY_STATUS_MOVE_ITEM:
		if( m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )
		{
			// 만약 이동한 아이템이 있다면,,, 서버로 보내 보내준다.
			if( m_pMyOnlineWorld->pOnlineNumPad->siCount != 0 )
			{
				m_siPollStatus			= ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
				CItemHeader	*pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem( m_uiMoveItemID );

				/********************************************************************************/
				// robypark 2005/1/13 18:39
				// 소집권 아이템의 용병 슬롯간 이동은 소집권 아이템 생성 시작 시간부터 
				// 공성 종료시간 까지만이다.
				// 즉, 공성 소집권 아이템을 상단원에게 나누어 줄 수 있는 시간과 같다
				if (pItem)
				{
					// 소집권 아이템인지 검사
					if (NULL != pItem->m_pImperium)
					{
						/*
						// 공성 소집권 아이템을 다른 용병 슬롯으로 옮길 수 있는 시간인가?
						if(FALSE == m_pMyOnlineWorld->IsInTimeGiveArmyItem())
						*/
						{
							// 옮길 수 있는 시간이 아니다.
							m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
							m_pLCItemInventory->ShowAllItems();
							m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
							m_pLCItemInventory->StopDrag();				
							m_pMyOnlineWorld->m_pMercenary->UnLock();
							m_bIsItemMoving = FALSE;
							break;
						}
					}
				}
				/********************************************************************************/

				m_uiSelectedSlot = m_uiMoveItemDestFollower;
				FillItemListBox();
				m_pLCItemInventory->ShowAllItems();

				m_siMoveItemSlotAfter = m_pLCItemInventory->GetEmptySlot(m_uiMoveItemID);
				m_uiSelectedSlot = m_uiMoveItemSrcFollower;
				FillItemListBox();

				if(m_siMoveItemSlotAfter != -1)
				{
					BOOL bCanMoveItem = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->CanMoveItem(m_uiMoveItemSrcFollower, m_uiMoveItemID, m_pMyOnlineWorld->pOnlineNumPad->siCount, m_uiMoveItemDestFollower, m_siMoveItemSlotAfter);

					if(bCanMoveItem)
					{
						m_siPollStatus = ON_INVENTORY_STATUS_MOVE_ITEMSEND;
						// 서버로 아이템이 이동한것을 알린다.
						m_pMyOnlineWorld->pOnlineClient->SendItemPosInInventory(m_uiMoveItemSrcFollower, m_uiMoveItemDestFollower, m_uiMoveItemID, m_siMoveItemSlotAfter,(SI16)m_pMyOnlineWorld->pOnlineNumPad->siCount );
					}
					else
					{
						m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
						m_pLCItemInventory->ShowAllItems();
						m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
						m_pLCItemInventory->StopDrag();				
						m_pMyOnlineWorld->m_pMercenary->UnLock();
						m_bIsItemMoving = FALSE;
					}
				}
				else
				{
					m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
					m_pLCItemInventory->ShowAllItems();
					m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
					m_pLCItemInventory->StopDrag();				
					m_pMyOnlineWorld->m_pMercenary->UnLock();
					m_bIsItemMoving = FALSE;
				}
			}
			else
			{
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
				m_pLCItemInventory->ShowAllItems();
				m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
				m_pLCItemInventory->StopDrag();				
				m_pMyOnlineWorld->m_pMercenary->UnLock();
				m_bIsItemMoving = FALSE;
			}
		}
		break;

	case ON_INVENTORY_STATUS_MOVE_ITEM_DELAY:
		m_siItemMovingDelay++;
		if( m_siItemMovingDelay >= 3 )
		{
			m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_POLL;
			m_siItemMovingDelay	=	0;
		}
		break;

	case ON_INVENTORY_STATUS_INIT_TRADE:
		if( m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )
		{
			m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_POLL;
			m_bIsItemDragging	=	0;

			if( m_pMyOnlineWorld->pOnlineNumPad->siCount == 0 )
			{
				DragFail();
				break;
			}

			const ItemInfo *pTempInfo = m_pLCItemInventory->GetSlotInfo(m_siMoveItemIndex);
			m_uiMoveItemID		  =	pTempInfo->uiItemID;
			m_siMoveItemSlotBefore = m_pLCItemInventory->GetSlot(pTempInfo->uiItemID);
			m_siMoveItemSlotAfter = m_pMyOnlineWorld->pOnlineTrade->GetList()->GetEmptySlot();

			// 삽입이 가능하다면...
			// 서버로 알린다.
			if( m_siMoveItemSlotAfter != -1 )
			{			
				m_siPollStatus	=	ON_INVENTORY_STATUS_MAIN_POLL;
				ItemInfo TempAddItem;
				TempAddItem.siType = m_pMyOnlineWorld->pOnlineTrade->m_siAddType = LIST_ITEM;
				TempAddItem.uiItemID = m_uiMoveItemID;
				TempAddItem.ItemCount = m_pMyOnlineWorld->pOnlineNumPad->siCount;
				
				m_pMyOnlineWorld->pOnlineTrade->AddItem( m_siMoveItemSlotBefore, m_siMoveItemSlotAfter, m_uiMoveItemID, (UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount, m_uiSelectedSlot);

				// 잠시 인벤토리에서 지워 준다.				
				m_pLCItemInventory->StopDrag();
				m_siMoveItemSlotBefore = m_siMoveItemSlotAfter = m_uiMoveItemID = 0;
			}

			m_bIsItemMoving = FALSE;
		}
		break;

	case ON_INVENTORY_STATUS_WEARITEMLIST_SELECT:
		{
			if(m_BExit.Process(m_bPushMouseL))
			{
				IsSetInActive = TRUE;
			}

			if(IpD.LeftPressSwitch)
			{
				m_siPollStatus = ON_INVENTORY_STATUS_LISTINFO_SHOW;

				if( m_pLCWearItemWeapon->GetSelectSlot() != -1 )
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_SWORD, 0);
				else if( m_pLCWearItemArmor->GetSelectSlot() != -1 )
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_ARMOR, 0);
				else if( m_pLCWearItemHelmet->GetSelectSlot() != -1 )
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_HELMAT, 0);
				else if( m_pLCWearItemBelt->GetSelectSlot() != -1 )	
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_BELT, 0);
				else if( m_pLCWearItemGloves->GetSelectSlot() != -1 )
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_GLOVES, 0);
				else if( m_pLCWearItemBoots->GetSelectSlot() != -1 )
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_BOOTS, 0);
				else if( (m_pLCWearItemAccessoryR->GetSelectSlot() != -1) || (m_pLCWearItemAccessoryL->GetSelectSlot() != -1) )
					m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_RING, 0);
				else if( m_pLCWearItemGuardian->GetSelectSlot() != -1 )
				{
					if(m_uiSelectedSlot == 0)
						m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_GUARDIAN,0);
				}
				else
					m_siPollStatus = ON_INVENTORY_STATUS_WEARITEMLIST_SELECT;

				for(vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
					(*ix)->StopDrag();

				IpD.LeftPressSwitch = 0;
			}
		}
		break;
	case ON_INVENTORY_STATUS_VILLAGELIST_SELECT:
		{
			if(m_BExit.Process(m_bPushMouseL))
			{
				IsSetInActive = TRUE;
			}

			if(!IpD.LeftPressSwitch)		break;

			SI16 siIndex;
			for(vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			{
				if((siIndex = (*ix)->GetSelectSlot()) != -1 && !m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					m_pItemSlotPointer	= (*ix)->GetSlotInfo((const)siIndex);

					if(!m_pItemSlotPointer)	continue;

					if(m_pMyOnlineWorld->IsFindNow())			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_BOARD_SERCHING), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MERITNOTENO));
					else
					{
						if(m_pLCItemInventory->GetSelectSlot() != -1)
						{
							IpD.LeftPressSwitch = 0;
							m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_VILLAGELIST, 0, m_pItemSlotPointer->uiItemID);
							m_siPollStatus = ON_INVENTORY_STATUS_LISTINFO_SHOW;
							break;
						}
					}
				}
			}
		}
		break;
	case ON_INVENTORY_STATUS_LISTINFO_SHOW:
		{
			if(!m_pOnlineItemList->IsAction())
			{
				m_BWearItem.SetAction(TRUE);
				m_BItemVillage.SetAction(TRUE);
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
			}

			if(m_BExit.Process(m_bPushMouseL))
			{
				IsSetInActive = TRUE;
			}
		}
		break;

		//----------------------------------------------------------------------------
		// actdoll (2004/07/25 0:22) : 인벤토리 메인 폴
		//----------------------------------------------------------------------------
	case ON_INVENTORY_STATUS_MAIN_POLL:
		{
			SI16			siIndex;
			CItemHeader		*pItem;

			// 장착할 수 있는 아이템 리스트 버튼이 눌렸을 때
			if(m_BWearItem.Process(m_bPushMouseL))	
			{
				m_BWearItem.SetAction(FALSE);
				m_BItemVillage.SetAction(FALSE);
				m_siPollStatus = ON_INVENTORY_STATUS_WEARITEMLIST_SELECT;
				break;
			}
			// 아이템 취급 마을 리스트 버튼이 눌렸을 때
			else if(m_BItemVillage.Process(m_bPushMouseL))
			{
				m_BWearItem.SetAction(FALSE);
				m_BItemVillage.SetAction(FALSE);
				m_siPollStatus = ON_INVENTORY_STATUS_VILLAGELIST_SELECT;
				break;
			}
			
			// 마우스로 선택한 아이템의 간단 정보창 출력 세팅
			for(vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			{
				if((siIndex = (*ix)->GetSelectSlot()) != -1 && !m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					m_pItemSlotPointer	= (*ix)->GetSlotInfo((const)siIndex);
					pItem		= m_pItemSlotPointer == NULL ? NULL : m_pMyOnlineWorld->pItemsInfo->GetItem( m_pItemSlotPointer->uiItemID );
					
					if(!pItem)				continue;
					
					if(m_pItemSlotPointer->bHide)	continue;
					
					// actdoll (2005/02/14 15:52) : 시간제 아이템용 표기를 위한 추가정보
					// 현재 장착한 용병의 정보를 얻고
					OnlineFollower *pOF = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetFollower( m_uiSelectedSlot );

					// 무기 장착 위치별로 체크 들어간다.
					SI16	siItemType	= -1;
					if		( (*ix) == m_pLCWearItemWeapon		)	{	siItemType	= ON_WEAR_WEAPON_ITEM;		}
					else if	( (*ix) == m_pLCWearItemArmor		)	{	siItemType	= ON_WEAR_ARMOR_ITEM;		}
					else if	( (*ix) == m_pLCWearItemHelmet		)	{	siItemType	= ON_WEAR_HELMET_ITEM;		}
					else if	( (*ix) == m_pLCWearItemGuardian	)	{	siItemType	= ON_WEAR_GUARDIAN_ITEM;	}
					else if	( (*ix) == m_pLCWearItemBelt		)	{	siItemType	= ON_WEAR_BELT_ITEM;		}
					else if	( (*ix) == m_pLCWearItemGloves		)	{	siItemType	= ON_WEAR_GLOVE_ITEM;		}
					else if	( (*ix) == m_pLCWearItemBoots		)	{	siItemType	= ON_WEAR_SHOES_ITEM;		}
					else if	( (*ix) == m_pLCWearItemAccessoryL	)	{	siItemType	= ON_WEAR_LRING_ITEM;		}
					else if	( (*ix) == m_pLCWearItemAccessoryR	)	{	siItemType	= ON_WEAR_RRING_ITEM;		}
					
					m_pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo*>(m_pItemSlotPointer), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_DOWN, FALSE, FALSE, pOF, siItemType );
				}
			}

			// 바닥에서 집어든 아이템이 있다면
			if(m_uiPickUpItemID != 0)
			{
				// 마을 안이라면 잘못된 상황이다요
				if( m_bIsVillage )		break;	

//				if(!m_bIsVillage)
//				{				
				// 설정된 좌표의 맵 위에 있는 아이템의 갯수를 받고
				// 이 아이템을 받을 용병 슬롯 번호를 얻은 뒤
				// 이 용병이 아이템을 주워담을 수 있는 갯수를 받는다.
				SI16 siQuantity				= m_pMyOnlineWorld->pOnlineMap->GetItemQuantity(m_siPickUpItemMapPosX, m_siPickUpItemMapPosY);
				m_uiPickUpItemDestFollower	= m_uiSelectedSlot;
				m_uiPichUpItemQuantity		= m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake(m_uiPickUpItemDestFollower, m_uiPickUpItemID);		

				// 시작 ////////////////////////
				// robypark 2004/11/27 10:59
				// 바닥에 떨어져 있는 공성전 소집권 아이템은 운영자나 운영자 권한5만 집을 수 있다.
				CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiPickUpItemID);

				if (pItemHeader)
				{
					if (NULL != pItemHeader->m_pImperium)
					{
#ifndef _DEBUG
						// 운영자 등급 5이거나, 운영자 캐릭터일 경우에만 바닥에 버려진 소집권 아이템을 집을 수 있다.
						if ((m_pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5) ||
								(0 == strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pMyOnlineWorld->pOnlineText->Get(8000495))))
						{
							// 너 영자구나~~
						}
						else
						{
							//m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NO_GAIN));
							m_uiPickUpItemDestFollower	= m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
							m_bIsItemPickingUp			= FALSE;
							m_siPollStatus				=	ON_INVENTORY_STATUS_MAIN_INITPOLL;
							m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
							m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
							break;
						}
#endif // _DEBUG
					}
				}
				// 끝 ////////////////////////
				
				// 더이상 아이템을 들 수 없는 상황이라면 메시지 띄우고 모조리 초기화 시킨다.
				if( m_uiPichUpItemQuantity == 0 )
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NO_GAIN));
					m_uiPickUpItemDestFollower	= m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
					m_bIsItemPickingUp			= FALSE;
					m_siPollStatus				=	ON_INVENTORY_STATUS_MAIN_INITPOLL;
					m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
					m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
				}
				// 떨어진 아이템 모두를 가져갈 수 있다면
				else if( siQuantity <= m_uiPichUpItemQuantity )
				{
					m_uiPichUpItemQuantity = siQuantity;
					m_siPollStatus = ON_INVENTORY_STATUS_MAIN_INIT_ITEMPICKUP;					
				}
				// 일부만 가져갈 수 있다면
				else if( siQuantity > m_uiPichUpItemQuantity )
				{
					m_siPollStatus = ON_INVENTORY_STATUS_MAIN_INPUT_ITEMPICKUP;
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( m_uiPichUpItemQuantity );
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit( NUMBERCONTROL_UNIT_THING );
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );
				}
				// actdoll (2004/07/25 0:40) : 기존 코드긴 한데.. 위 조건식에서 여기까지 떨어질 수 있을까? ㅡ_ㅡ;
				else
				{
					m_uiPickUpItemDestFollower = m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
					m_bIsItemPickingUp = FALSE;
					m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_INITPOLL;
					m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
					m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
				}
//				}
			}
			// 바닥에서 집어든 아이템이 없다면
			else
			{
				// 장착 가능 아이템 리스트 창이 떠 있다면 드래그는 중지다.
				if( m_pOnlineItemList->IsAction() )
				{
					m_pLCItemInventory->StopDrag();
					break;
				}

				// 특수 아이템들에 대한 사용 처리
				CheckUseItem();

				// 바닥에 던질 아이템 정보가 있다면
				if((siThrowItemIndexOfLC = GetThrowItemIndex()) != -1)
				{
					// 마을 안일 경우
					if(m_bIsVillage)
					{
						// 주막 보내기 모드일 경우 중지
						if(m_pMyOnlineWorld->m_siInnStatus == 2)		break;

						// 용병창이 활성화 되어있다면
						if(m_pMyOnlineWorld->m_pMercenary->IsAction())
						{
							// 마우스가 올라가 있는 용병창의 DB 슬롯 번호를 받았다면 폴 스테이터스를 아이템 던지기로 바꾼다.
							if((siSelectedSell = m_pMyOnlineWorld->m_pMercenary->GetMouseOverSlotNo(TRUE)) != -1)
							{
								m_siPollStatus =	ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW;
							}
							// 아니라면 던지기는 정지다!
							else
							{
								m_pLCItemInventory->StopDrag();
								m_bIsItemDragging = 0;
							}
						}
						// 용병창 비활성화라면 던지기는 정지다!
						else
						{
							m_pLCItemInventory->StopDrag();
							m_bIsItemDragging = 0;
						}
					}
					// 마을 밖일 경우
					else
					{
						//if( m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )		break;
						if( m_siEnteringBuildingType != 0 )							break;	// 마을 안에서는 아이템을 버릴수가 없다
						if( m_pMyOnlineWorld->pOnlineReception->IsActive())			break;	// 경매 상황이냐?
						if( m_pMyOnlineWorld->pOnlineSmith->IsActive())				break;	// 대장장이 상황이냐?
						if( m_pMyOnlineWorld->pOnlineTrade->IsActive())				break;	// 거래 상황이냐?
						m_siPollStatus =	ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW;		// 폴 스테이터스를 아이템 던지기로 바꾼다.
					}
				}
				// 바닥에 던질 아이템 정보가 없다면
				else
				{
					// 용병 보이기/안보이기 버튼 눌렀을 때
					if(m_BSelFollower.Process(m_bPushMouseL))
					{
						// 따라다니는 용병의 슬롯 번호가 일치한다면 메시지 띄우고 확인 사살
						if( m_pMyOnlineWorld->pMyData->GetMercenary() == m_uiSelectedSlot && m_uiSelectedSlot )
						{
							m_bMsgBoxFlag_VanishFieldMercenary = TRUE;
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_SHOW_FOLLOWER_NOTICE));
						}
					}

					// 메시지 박스에서 OK 명령이 떨어졌을 경우
					if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
					{
						// 용병 제거 메시지 박스가 뜬 상태라면
						if(m_bMsgBoxFlag_VanishFieldMercenary)
						{
							m_bMsgBoxFlag_VanishFieldMercenary = FALSE;
							m_BSelFollower.SetAction(FALSE);
							m_pMyOnlineWorld->pOnlineClient->SendRequestDeleteVMercenary();
						}
					}
					// 메시지 박스에서 취소 명령이 떨어졌을 경우
					else if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL)
					{
						m_bMsgBoxFlag_VanishFieldMercenary = FALSE;
					}

					// 인벤토리 창 끄기라면 죽인다.
					if(m_BExit.Process(m_bPushMouseL))
					{
						IsSetInActive = TRUE;
						/*
						if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
						{
							m_pMyOnlineWorld->pOnlineBooth->SetStatusIn(ON_BENCH_IN_EXIT_INIT);
							m_pMyOnlineWorld->SetShortKey( FALSE);	
							m_pMyOnlineWorld->pOnlineBooth->SetActionIn(FALSE);						
						}
						*/
					}
					// 아니라면
					else
					{
						// 아이템 드래그를 시작했는지 확인
						if( m_bIsItemDragging == 0																		&&	// 아이템 드래그 중이 아니고
							( m_pMyOnlineWorld->fLeftOneSwitch == 1 || m_pMyOnlineWorld->fVillageLeftOneSwitch == 1 ) 	&&	// 왼쪽 마우스를 누른 상태이고
							!m_pMyOnlineWorld->pOnlineMsgBox->IsActive()												&&	// 메시지 박스 활성화 상태가 아니고
							m_pMyOnlineWorld->pOnlineBooth->GetList()->GetDragSlot() == -1								&&	// 좌판에서 아이템을 드래그 중이 아니고
							!m_pMyOnlineWorld->m_bItemDraging															&&	// 아이템 드래그 중이 아니며
							!m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive()										&&	// 거래 메시지 박스가 떠있는 상태가 아니고
							!m_pMyOnlineWorld->pOnlineBooth->GetFollowerSlot(m_uiSelectedSlot) )							// 좌판에서 교환하고자 하는 용병 슬롯이 없다
						{
							if(m_pMyOnlineWorld->m_bEvent == FALSE)
							{
								for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
								{
									// 아이템 드래그 중인 것이 확인됐다면
									if( (*ix)->DragUp() )	
									{
										m_bIsItemDragging = 1;

										// actdoll (2004/07/20 8:39) : 장착한 시간제 아이템의 경우 드래그가 불가능하도록 세팅한다.
										if( (*ix) != m_pLCItemInventory )	// 인벤토리 상황이 아니라면
										{
											// 시간제 아이템일 경우는 무시
											const ItemInfo	*pII		= (*ix)->GetSlotInfo(0);
											if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( pII->uiItemID ) )
											{
												(*ix)->StopDrag();
												m_bIsItemDragging = 0;
											}
										}
										break;
									}
								}

								// 드래그 중이 맞다면
								if( m_bIsItemDragging == 1 )
								{
									m_siPollStatus			=	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
									m_bPushMouseL			=	FALSE;
									m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( FALSE );
								}
							}

						}
						// 드래그 중인 아이템이 내려졌는지 확인
						else if( m_bIsItemDragging == 1	&&																// 아이템은 드래그 중이고
								(m_pMyOnlineWorld->fLeftOneSwitch==1 || m_pMyOnlineWorld->fVillageLeftOneSwitch ==1))	// 왼쪽 버튼이 눌렸는가?
						{
							if(m_pMyOnlineWorld->m_bEvent == FALSE)
							{
								if(m_pLCItemInventory->GetDragSlot() != -1)	AttachItem();	// 인벤토리에서부터 아이템이 움직였다면 장착 처리 들어간다.
								else										StripItem();	// 그것이 아니라면 장착 해제 처리 들어간다.
							}
							//else
							//	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "이벤트 기간중에는 무기를 착용하거나 옮길수 없습니다.");
						}
						
					}
				}
			}
		}// EOF case ON_INVENTORY_STATUS_MAIN_POLL:
		break;

	case ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW:
		{			
			SI16	siTempThrowItemIndex = m_pLCItemInventory->GetDragSlot();

			// 만약 지금 거래 중이라면 
			if(m_pMyOnlineWorld->pOnlineTrade->IsActive())
			{
				OnlineListControl	*pTempList = m_pMyOnlineWorld->pOnlineTrade->GetList();						

				// 거래창 안에 아이템이 있다면
				if(pTempList->GetSelectSlot() != -1)
				{
					// 시작 // 이 부분으로 오지 않는다. /////////////////////////////////
					// robypark 2004/11/24 11:37
					// 소집권 아이템 거래는 같은 상단원 사이만 가능하다.
					// 거래하려는 아이템 정보 헤더 얻기
					const ItemInfo	*pInfo	 =	m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex);

					// 슬롯 아이템 정보 유효한지 검사
					if (NULL == pInfo)
					{
						m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
						m_bIsItemDragging	=	0;
						m_pLCItemInventory->StopDrag();
						break;
					}

					CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(pInfo->uiItemID);

					if (pItemHeader)
					{
						// 소집권 아이템인지 검사
						if (NULL != pItemHeader->m_pImperium)
						{
							/*
							// 공성 소집권 아이템을 상단원에게 나누어 줄 수 있는 시간인가?
							if(m_pMyOnlineWorld->IsInTimeGiveArmyItem())
							*/
							{
								// 소집권 아이템 거래는 같은 상단원 사이만 가능하다.
								if (!m_pMyOnlineWorld->IsMemberOfSameGuild(m_pMyOnlineWorld->pOnlineTrade->FriendInfo.uiDestUniqueID))
								{
									m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
									m_bIsItemDragging	=	0;
									m_pLCItemInventory->StopDrag();
									break;
								}
							}
						}
					}
					// 끝 ///////////////////////////////////

					// 용병의 아이템은 거래불가
					siThrowItemIndexOfLC = m_pLCItemInventory->GetDragSlot();

					m_siPollStatus		=	ON_INVENTORY_STATUS_INIT_TRADE;
					pItemInfo			=	m_pLCItemInventory->GetSlotInfo(siThrowItemIndexOfLC);
					m_siMoveItemIndex	=	siThrowItemIndexOfLC;
					m_bIsItemDragging	=	0;

					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( pItemInfo->ItemCount );
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );					
				}
				else
				{
					m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
					m_bIsItemDragging	=	0;
					m_pLCItemInventory->StopDrag();
				}

				break;
			}

			// 캐릭간 아이템 이동
			if(m_pMyOnlineWorld->m_pMercenary->IsAction())
			{
				if((siSelectedSell = m_pMyOnlineWorld->m_pMercenary->GetMouseOverSlotNo(TRUE)) != -1)
				{
					if((siSelectedSell != m_uiSelectedSlot) )
					{							
						m_uiMoveItemDestFollower	=	siSelectedSell;
						m_uiMoveItemSrcFollower		=	m_uiSelectedSlot;
						const ItemInfo	*pInfo		=	m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex);
						m_siPollStatus				=	ON_INVENTORY_STATUS_MOVE_ITEM;

						if(!pInfo)
						{
							m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
							m_pLCItemInventory->StopDrag();
							break;
						}
						m_uiMoveItemID				=	pInfo->uiItemID;
						m_siMoveItemSlotBefore		=	siTempThrowItemIndex;
						m_siMoveItemIndex			=	siTempThrowItemIndex;
						m_bIsItemDragging			=	0;						

						UI32 uiTempQuantity	= min(pInfo->ItemCount, m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake(m_uiMoveItemDestFollower, m_uiMoveItemID));

						if(uiTempQuantity == 0)
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_ITEMMOVE), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTWEIGHT));
							m_pLCItemInventory->StopDrag();
							m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
							m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
							break;
						}

						/********************************************************************************/
						// robypark 2005/1/13 18:39
						// 소집권 아이템의 용병 슬롯간 이동은 소집권 아이템 생성 시작 시간부터 
						// 공성 종료시간 까지만이다.
						// 즉, 공성 소집권 아이템을 상단원에게 나누어 줄 수 있는 시간과 같다
						CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiMoveItemID);

						if (pItemHeader)
						{
							// 소집권 아이템인지 검사
							if (NULL != pItemHeader->m_pImperium)
							{
								/*
								// 공성 소집권 아이템을 다른 용병 슬롯으로 옮길 수 있는 시간인가?
								if(FALSE == m_pMyOnlineWorld->IsInTimeGiveArmyItem())
								*/
								{
									// 옮길 수 있는 시간이 아니다.
									m_pLCItemInventory->StopDrag();
									m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
									m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
									break;
								}
							}
						}
						/********************************************************************************/

						m_pLCItemInventory->HideItem(siTempThrowItemIndex);
						
						m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(uiTempQuantity);
						m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
						m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );
						m_pMyOnlineWorld->m_pMercenary->Lock();
						m_bIsItemMoving =	TRUE;
						break;
					}

					m_pLCItemInventory->StopDrag();
					break;
				}
			}

			// 노점상태일때
			if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
			{
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
				break;
			}

			// 시작 ///////////////////////////////////
			// robypark 2004/11/24 11:37
			// 바닥에 버릴 수 있는 아이템인지 검사하기
			// 거래하려는 아이템 정보 헤더 얻기
			const ItemInfo	*pInfo	 =	m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex);

			// 슬롯 아이템 정보 유효한지 검사
			if (NULL == pInfo)
			{
				// 소집권 아이템은 버릴 수 없다
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
				//m_bIsItemDragging	=	0;
				//m_pLCItemInventory->StopDrag();
				break;
			}

			CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(pInfo->uiItemID);

			if (pItemHeader)
			{
				// 소집권 아이템인지 검사
				if (NULL != pItemHeader->m_pImperium)
				{
#ifndef _DEBUG
					// 운영자 등급 5이거나, 운영자 캐릭터일 경우에만 소집권 아이템을 바닥에 버릴 수 있다.
					if ((m_pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5) ||
						(0 == strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pMyOnlineWorld->pOnlineText->Get(8000495))))
					{
					}
					else
					{
						// 소집권 아이템은 버릴 수 없다
						m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
						//m_bIsItemDragging	=	0;
						//m_pLCItemInventory->StopDrag();
						break;
					}
#endif // _DEBUG
				}
			}
			else
			{
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
				//m_bIsItemDragging	=	0;
				//m_pLCItemInventory->StopDrag();
				break;
			}
			// 끝 ///////////////////////////////////

			// 아이템을 버린다.			
			m_siPollStatus		=	ON_INVENTORY_STATUS_ITEMTHROW;
			m_bIsItemDragging	=	0;
			
			// 숫자판을 보인다.
			siTempThrowItemIndex = m_pLCItemInventory->GetDragSlot();
			SetThrowItemInfo(siTempThrowItemIndex);


			if(siTempThrowItemIndex != -1 )
			{
				if(m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex))
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex)->ItemCount);
				else
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(0);
				m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
				m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow((SI16)ptMouse.x, (SI16)ptMouse.y);
				m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
			}

			else
			{
				m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_POLL;
			}
		}
		break;

	case ON_INVENTORY_STATUS_ITEMTHROW:
		if( m_siEnteringBuildingType != 0 )	
		{
			m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_POLL;
			break;
		}

		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
		
		// 버리는 아이템을 얻어온다.
		siThrowItemIndexOfLC	=	GetThrowItemInfo();

		m_bIsItemMoving = TRUE;

		if(m_bIsVillage == FALSE)
		{
			if(siThrowItemIndexOfLC != -1)
			{
				if((m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() == TRUE) && ( m_pMyOnlineWorld->pOnlineNumPad->siCount != 0 ) )
				{
					pItemInfo	=	m_pLCItemInventory->GetSlotInfo(siThrowItemIndexOfLC);			
				
					if(pItemInfo)
					{
						if( m_pMyOnlineWorld->pOnlineNumPad->siCount <= pItemInfo->ItemCount )
						{
							// 버리는 아이템 정보 기억
							m_uiThrowItemSrcFollower	= m_uiSelectedSlot;
							m_uiThrowItemID	= pItemInfo->uiItemID;
							m_uiThrowItemQuantity	= m_pMyOnlineWorld->pOnlineNumPad->siCount;

							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.dwMsgType        = ON_REQUEST_THROWITEM;
							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.uiItemID         = pItemInfo->uiItemID;
							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.uiQuantity       = (SI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;
							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.uiFollowerID     = m_uiSelectedSlot;
							m_pMyOnlineWorld->pOnlineClient->SendThrowItem(&m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg);					// 서버 요청
						}

						m_bIsItemMoving = FALSE;
						m_siPollStatus  =	ON_INVENTORY_STATUS_MAIN_ITEMTHROW;
					}
				}
				else if((m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() == TRUE) && ( m_pMyOnlineWorld->pOnlineNumPad->siCount == 0 ) )
				{
					m_siPollStatus	=	ON_INVENTORY_STATUS_MAIN_INITPOLL;	
					
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_pLCItemInventory->StopDrag();
					m_bIsItemMoving = FALSE;
				}
			}
			else
			{				
				m_siPollStatus  =	ON_INVENTORY_STATUS_MAIN_INITPOLL;
				m_bIsItemMoving = FALSE;
			}
		}
		else
		{			
			m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
			m_bIsItemMoving = FALSE;
			m_siPollStatus  =	ON_INVENTORY_STATUS_MAIN_INITPOLL;
		}
		break;

	case ON_INVENTORY_STATUS_MAIN_ITEMTHROW:		
		break;
	case ON_INVENTORY_STATUS_MAIN_INIT_ITEMPICKUP:
		if(m_uiPickUpItemID != 0)
		{
			m_bIsItemMoving = TRUE;
			m_siPickUpItemDestSlot = m_pLCItemInventory->GetEmptySlot(m_uiPickUpItemID);

			if(m_siPickUpItemDestSlot != -1)
			{
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.dwMsgType        = ON_REQUEST_PICKUPITEM;
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiX              = m_siPickUpItemMapPosX;
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiY              = m_siPickUpItemMapPosY;
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiItemID         = m_uiPickUpItemID;			
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiFollowerID     = m_uiPickUpItemDestFollower;
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiQuantity		  = m_uiPichUpItemQuantity;
				m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiPosInInventory = m_siPickUpItemDestSlot;
				
				// Server에 Item을 줍었다고 메세지
				m_pMyOnlineWorld->pOnlineClient->SendPickUpItem(&m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg);
				
				m_siPollStatus			 	 = ON_INVENTORY_STATUS_MAIN_ITEMPICKUP;	
			}
			else
			{
				m_siPickUpItemDestSlot = m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
				m_siPollStatus				 = ON_INVENTORY_STATUS_MAIN_INITPOLL;
				
				m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
				m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
			}
		}
		else
		{
			m_siPickUpItemDestSlot = m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
			m_siPollStatus				 = ON_INVENTORY_STATUS_MAIN_INITPOLL;

			m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}
		break;

	case ON_INVENTORY_STATUS_MAIN_INPUT_ITEMPICKUP:
		if( m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )
		{
			if( m_pMyOnlineWorld->pOnlineNumPad->siCount == 0 )
			{
				m_uiPickUpItemDestFollower = m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
				m_siPollStatus				 = ON_INVENTORY_STATUS_MAIN_POLL;

				m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
				m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);				
				m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				
				m_bIsItemPickingUp		= FALSE;
				break;
			}
			else
			{
				m_uiPichUpItemQuantity = m_pMyOnlineWorld->pOnlineNumPad->siCount;
				m_siPickUpItemDestSlot = m_pLCItemInventory->GetEmptySlot(m_uiPickUpItemID);
				if(m_siPickUpItemDestSlot == -1)
				{
					m_uiPickUpItemDestFollower = m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
					m_siPollStatus				 = ON_INVENTORY_STATUS_MAIN_POLL;
					
					m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
					m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);				
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					
					m_bIsItemPickingUp		= FALSE;
					break;
				}
			}

			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.dwMsgType        = ON_REQUEST_PICKUPITEM;
			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiX              = m_siPickUpItemMapPosX;
			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiY              = m_siPickUpItemMapPosY;
			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiItemID         = m_uiPickUpItemID;
			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiPosInInventory = m_siPickUpItemDestSlot;
			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiFollowerID     = m_uiPickUpItemDestFollower;
			m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg.uiQuantity		  = m_uiPichUpItemQuantity;
				
			// Server에 Item을 줍었다고 메세지
			m_pMyOnlineWorld->pOnlineClient->SendPickUpItem(&m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg);

			m_siPollStatus			 	 = ON_INVENTORY_STATUS_MAIN_ITEMPICKUP;
			
		}
		break;

		//----------------------------------------------------------------------------
		// actdoll (2004/07/25 0:22) : 인벤토리 아이템 착용 시
		//----------------------------------------------------------------------------
	case ON_INVENTORY_STATUS_ATTACH_ITEM:
		{
			// actdoll (2004/07/26 12:27) : 시간제 아이템 장착 메시지 처리
			if( m_bMsgBoxFlag_WearTimerItem )	// 아이템 장착 에시지가 떠 있다면
			{
				if( m_uiWearTimerItemID )
				{
					// 메시지 박스에서 OK 명령이 떨어졌을 경우
					if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
					{
						m_bMsgBoxFlag_WearTimerItem = false;
						m_pMyOnlineWorld->pOnlineClient->SendRequestAttachItem( m_uiWearTimerItemDestFollower, m_siWearTimerItemDestSlot, m_uiWearTimerItemID, m_siWearTimerItemSettingType );
					}
					// 메시지 박스에서 취소 명령이 떨어졌을 경우
					else if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL)
					{
						m_bMsgBoxFlag_WearTimerItem		= false;
						m_uiPickUpItemDestFollower		= m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
						StopDrag();
						m_siPollStatus =	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
					}
				}
			}
		}
		break;
	}

	
	// actdoll (2005/02/02 1:32) : 
	//	만약 5개 컨트롤중에 한개라두 이동중인 아이템이 있다면 마우스를 숨긴다.
	//	그러나, 메시지 박스에 대답하는 형태라면 마우스를 보여준다.
	m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
	for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
	{
		if(!(*ix))		continue;
		if((*ix)->GetDragSlot() != -1)
		{
			m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( m_bMsgBoxFlag_WearTimerItem );
			break;
		}		
	}

	if(IpD.LeftPressSwitch)				m_bPushMouseL = TRUE;	
	else								m_bPushMouseL = FALSE;	


	if(IpD.RightPressSwitch)			m_bPushMouseR = TRUE;
	else								m_bPushMouseR = FALSE;

	// 비활성화 시킨다.
	if(IsSetInActive == TRUE)
	{
		SetActive(FALSE);
	}
}


//-----------------------------------------------------------------------------
// Name: Draw()
// Code: All (2004-06-07)
// Desc: 그리기 동작부
//-----------------------------------------------------------------------------
VOID	OnlineInventory::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!IsActive())			return;

	POINT	ptMouse;
	SI32	siClipLeft, siClipTop, siClipRight, siClipBottom;

	CHAR	szWeight[256];
	HDC		hDC;
	int		i, j;

	clGrp.GetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
	clGrp.SetClipFullScreen();	

	SI32	siDBID, siHP, siDrawFlag = 0;
	MyData	*pMD	= m_pMyOnlineWorld->pMyData;
	siDBID			= pMD->GetFollowerParameterKind( m_uiSelectedSlot );
	siHP			= pMD->GetFollowerParameterLife( m_uiSelectedSlot );

	// 인벤토리 보더를 그린다.
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siX, m_siY, m_MainImage.Header.Xsize, m_MainImage.Header.Ysize, m_MainImage.Image);

		// actdoll (2004/01/05 18:29) : 새로운 방법의 초상그리기
		if( siHP <= 0 )		siDrawFlag = DF_FLAG_PORTRAIT_HALFALPHA;	// 죽어있다면 반투명으로 그려라
		
		// robypark 2004/11/19 16:59
		// 주인공 캐릭터이며,
		if (0 == m_uiSelectedSlot)
		{
			// 또한 공성전 유닛이다
			if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit()  & ON_GAMEOPTION_SIEGEWARFAREUNIT)
			{
				// 변신 전 캐릭터 DB ID 설정
				siDBID = pMD->GetPrevCharOnlineKind();
			}
		}

		m_pMyOnlineWorld->m_pPortrait->DrawAsNormal( m_siX + 25, m_siY + 35, siDBID, true, siDrawFlag );
		
		if(!m_pLCWearItemGuardian->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 121, m_siY + 35, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[0]]);
		if(!m_pLCWearItemHelmet->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 185, m_siY + 35, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[1]]);
		if(!m_pLCWearItemGloves->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 249, m_siY + 35, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[2]]);
		if(!m_pLCWearItemWeapon->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 121, m_siY + 99, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[3]]);
		if(!m_pLCWearItemArmor->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 185, m_siY + 99, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[4]]);
		if(!m_pLCWearItemBelt->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 249, m_siY + 99, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[5]]);
		if(!m_pLCWearItemAccessoryL->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 121, m_siY + 163, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[6]]);
		if(!m_pLCWearItemBoots->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 185, m_siY + 163, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[7]]);
		if(!m_pLCWearItemAccessoryR->GetSlotInfo(0))
			clGrp.PutSpriteT(m_siX + 249, m_siY + 163, m_MWearImage.Header.Xsize, m_MWearImage.Header.Ysize, &m_MWearImage.Image[m_MWearImage.Header.Start[8]]);
		

		if(m_siPollStatus == ON_INVENTORY_STATUS_WEARITEMLIST_SELECT)
		{
			for(i = 0; i < 3; i++)
			{
				if(m_uiSelectedSlot == 0)
				{
					clGrp.PutSpriteT(m_siX + 121, m_siY + 35 + 64 * i, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[0]]);
					clGrp.PutSpriteT(m_siX + 185, m_siY + 35 + 64 * i, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[0]]);
					clGrp.PutSpriteT(m_siX + 249, m_siY + 35 + 64 * i, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[0]]);
				}
				else
				{
					if(i!=0)
						clGrp.PutSpriteT(m_siX + 121, m_siY + 35 + 64 * i, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[0]]);
					clGrp.PutSpriteT(m_siX + 185, m_siY + 35 + 64 * i, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[0]]);
					clGrp.PutSpriteT(m_siX + 249, m_siY + 35 + 64 * i, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[0]]);
				}
				//
			}

		}
		else if(m_siPollStatus == ON_INVENTORY_STATUS_VILLAGELIST_SELECT)
		{
			for(i = 0; i < m_pLCItemInventory->GetXnum(); i++)
				for(j = 0; j < m_pLCItemInventory->GetYnum(); j++)
				{
					const ItemInfo *pInfo = m_pLCItemInventory->GetSlotInfo(i*m_pLCItemInventory->GetYnum() + j);
					if(pInfo)
					{
						clGrp.PutSpriteT(m_siX + 10 + 64 * i, m_siY + 235 + 64 * j, m_MHlightImage.Header.Xsize, m_MHlightImage.Header.Ysize, &m_MHlightImage.Image[m_MHlightImage.Header.Start[1]]);
					}
				}
		}

		m_BWearItem.Put(&m_MBut2Image, 0, 1, 1, BUTTON_PUT_NOMOVE);
		m_BItemVillage.Put(&m_MBut2Image, 0, 1, 1, BUTTON_PUT_NOMOVE);
		m_BExit.Put(&m_MBut1Image, 0, 2, 1, BUTTON_PUT_NOMOVE);

		if(m_uiSelectedSlot != 0)
		{
			SI08 siMercenarySlot = pMD->GetMercenary();
			if(siMercenarySlot == m_uiSelectedSlot)
			{
				m_BSelFollower.Put(&m_MBut2Image, 0, 1, 1, BUTTON_PUT_NOMOVE);
			}
		}

		// 아이템 리스트를 볼때는 인벤토리를 덮어준다.
		if(m_pOnlineItemList->IsAction())
			clGrp.PutSpriteJin(m_siX, m_siY, m_MOverImage.Header.Xsize, m_MOverImage.Header.Ysize, m_MOverImage.Image);
		
		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		// 무게 출력
		sprintf((char*)szWeight, "%d / %d", pMD->GetFollowersInterface()->GetTotalItemWeight(m_uiSelectedSlot), 
											pMD->GetFollowersInterface()->GetMaxWeight(m_uiSelectedSlot));

		if(pMD->GetFollowersInterface()->IsItemWeightOverflowed((UI08)m_uiSelectedSlot))
		{				
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 239, m_siY + 369, 81, 14, szWeight, RGB(10, 10, 10));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 238, m_siY + 370, 81, 14, szWeight, RGB(255, 0, 0));
		}
		else
		{
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 239, m_siY + 369, 81, 14, szWeight, RGB(10, 10, 10));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 238, m_siY + 370, 81, 14, szWeight, RGB(250, 250, 250));
		}

		// 소지금
		MakeMoneyText(pMD->GetMoney(), (char*)szWeight);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 50, m_siY + 369, 114, 14, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 49, m_siY + 370, 114, 14, szWeight, RGB(250, 250, 250));

		// 캐릭터 정보를 찍어 준다.
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 114, m_siY + 7, 118, 13, pMD->GetFollowerCharName(m_uiSelectedSlot), RGB( 250, 250, 250 ) );

		sprintf(szWeight, "LV.%d", pMD->GetFollowerParameterLevel(m_uiSelectedSlot));	// 레벨
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 36, m_siY + 128, 60, 13, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 35, m_siY + 129, 60, 13, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "HP");			// 체력
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 31, m_siY + 145, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 30, m_siY + 146, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "%d/%d", pMD->GetFollowerParameterLife(m_uiSelectedSlot), pMD->GetFollowerParameterMaxLife(m_uiSelectedSlot));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 51, m_siY + 145, 46, 13, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 50, m_siY + 146, 46, 13, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "MP");			// 마법력
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 31, m_siY + 159, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 30, m_siY + 160, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "%d/%d", pMD->GetFollowerParameterMana(m_uiSelectedSlot), pMD->GetFollowerParameterMaxMana(m_uiSelectedSlot));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 51, m_siY + 159, 46, 13, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 50, m_siY + 160, 46, 13, szWeight, RGB(250, 250, 250));

		m_BWearItem.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BItemVillage.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		if(m_uiSelectedSlot != 0)
		{
			SI08 siMercenarySlot = pMD->GetMercenary();
			if(siMercenarySlot == m_uiSelectedSlot)
				m_BSelFollower.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}

		pSurface->ReleaseDC(hDC);
	}
	


	m_pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);

	// 장착 혹은 가지고 있는 아이템들의 그림 표시
	vector<OnlineListControl*>::iterator ix, ixOverDraw = m_ListControl.end();
	for( ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
	{
		(*ix)->Draw( pSurface );
		if( (*ix)->GetDragSlot() != -1 )	ixOverDraw = ix;
	}
	if( ixOverDraw != m_ListControl.end() )	(*ixOverDraw)->Draw( pSurface );	// 드래그중인 아이템이 있을 경우는 젤 위로 그려준다.


	// actdoll (2004/07/19 14:52) : = 시간제 아이템 = 아이템 남은 일수 표기
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAR2));

		BOOL	bIsTimerItem;
		UI16	uiItemID;
		SI16	siRemainDate, siLimitDate;
		SI16	siAddX, siAddY;
		OnlineFollower *pOF = pMD->GetFollowersInterface()->GetFollower( m_uiSelectedSlot );
		for( i=ON_WEAR_HELMET_ITEM; i<WEAR_ITEM_NUM; i++ )
		{
			uiItemID	= pOF->GetWearItem( i, bIsTimerItem, siRemainDate );
			siLimitDate	= m_pMyOnlineWorld->pItemsInfo->GetItemUsingDay( uiItemID );
			if( bIsTimerItem )		// 타이머 아이템일 경우 해당 슬롯의 왼쪽 하단에 남은 날짜를 표기해준다.
			{
				DWORD	dwColor			= 0;
				char	pszString[32]	= {0,};
				switch( i ) 
				{
					case ON_WEAR_GUARDIAN_ITEM	:	siAddX	= 121;	siAddY	= 35;	break;	// 좌상단
					case ON_WEAR_HELMET_ITEM	:	siAddX	= 185;	siAddY	= 35;	break;	// 정상단
					case ON_WEAR_GLOVE_ITEM		:	siAddX	= 249;	siAddY	= 35;	break;	// 우상단
					case ON_WEAR_WEAPON_ITEM	:	siAddX	= 121;	siAddY	= 99;	break;	// 좌중단
					case ON_WEAR_ARMOR_ITEM		:	siAddX	= 185;	siAddY	= 99;	break;	// 정중단
					case ON_WEAR_BELT_ITEM		:	siAddX	= 249;	siAddY	= 99;	break;	// 우중단
					case ON_WEAR_LRING_ITEM		:	siAddX	= 121;	siAddY	= 163;	break;	// 좌하단
					case ON_WEAR_SHOES_ITEM		:	siAddX	= 185;	siAddY	= 163;	break;	// 정하단
					case ON_WEAR_RRING_ITEM		:	siAddX	= 249;	siAddY	= 163;	break;	// 우하단
				}

				// actdoll (2005/02/04 9:11) : 시간제 아이템 관련 출력 변경
				if( siLimitDate < siRemainDate )		// 기간 시작 전
				{
					dwColor	= RGB( 0, 250, 250 );
					strcpy( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_READY_TO_START) );
				}
				else if( siRemainDate < 0 )				// 만약 기일이 음수라면 삭제 예정 아이템이란다.
				{
					dwColor	= RGB( 250, 120, 0 );
					strcpy( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_IS_EXPIRED) );
				}
				else if( siRemainDate <= 5 )			// 5일 이하 남았을 때
				{
					dwColor	= RGB( 250, 250, 0 );
					sprintf( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_REMAIN_TIME), siRemainDate );
				}
				else									// 일반적일때
				{
					dwColor	= m_pMyOnlineWorld->PaletteWhiteColor;
					sprintf( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_REMAIN_TIME), siRemainDate );
				}

				// 글씨 출력
				m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + siAddX + 1, m_siY + siAddY + 1, pszString, 0 );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + siAddX, m_siY + siAddY, pszString, dwColor );
			}
		}

		pSurface->ReleaseDC(hDC);
	}

	// 장착가능 아이템 리스트 출력
	m_pOnlineItemList->Draw(pSurface);

	// 현재 필드에서 들어올린 아이템이 있다면
	if(m_uiPickUpItemID != 0)
	{
		// 만약 Field Item을 들고 있다면 그 Field Item을 출력한다.
		XSPR			*pItemSpr		= NULL;
		CItemHeader		*pItem			= NULL;
		SI16			TempX;
		SI16			TempY;

		pItem    = m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiPickUpItemID);
		if(pItem)
		{
			pItemSpr = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);

			if(pItemSpr)
			{
				TempX = ptMouse.x - (m_pLCItemInventory->GetWidth() / 2);
				TempY = ptMouse.y - (m_pLCItemInventory->GetHeight() / 2);			
				
				clGrp.PutSpriteT(TempX, TempY, pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[pItemSpr->Header.Start[pItem->m_siSpriteFont]]);
			}
		}
	}
	
	// robypark 2004/10/28 11:29
	// cltOnlineWorld::FieldDraw()에서 pOnlineMsgBox를 그려주는 루틴이 이미 존재한다.
	// 같은 메시지 박스를 두 번 그리게됨으로 반 투명 대화상자가 아니라 불투명 대화상자가 된다. 시간제 아이템 무영품 착용시 등장하는 메시지 박스.
	// 이로 인하여 다음 코드를 주석처리한다.
//	m_pMyOnlineWorld->pOnlineMsgBox->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));

	clGrp.SetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
	
}

//-----------------------------------------------------------------------------
// Name: SetActive()
// Code: All (2004-06-07)
// Desc: 이 클래스의 동작 설정 여부 확인
//		bActive		- 동작시킬려면 true, 아니라면 false
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetActive( BOOL bActive )
{ 
	if( !m_pLCItemInventory )		return;

	m_bIsAction	= bActive;
	
	// 활성화 시켜라
	if(bActive == TRUE) 
	{
		m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_INITPOLL;
		m_bIsItemDragging	=	0;
		g_fMiniMapAct		=	FALSE;
		// 노 필드 지역을 설정한다.
		if( m_hNoField != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_hNoField );
		m_hNoField	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + m_siWidth, m_siY + m_siHeight);
		
		if(!m_bIsItemPickingUp)
		{
			if( m_pMyOnlineWorld->pOnlineMiniMap->IsActive() )
			{
				// H2-S-05.02.15 //
				// 인벤토리 활성화 시키기 전에 미니맵의 상태를 저정한다.
				// 풀맵 상태이면 맵 비활성화
				if(m_pMyOnlineWorld->pOnlineMiniMap->ISFullMode())
					g_fMiniMapAct = FALSE;
				else
				// 맵이 미니맵 상태이면 그 상태를 저장
					g_fMiniMapAct = m_pMyOnlineWorld->pOnlineMiniMap->IsActive();					
				// H2-E-05.02.15 //
			}
		}
		
		// 아이템 리스트 컨트롤을 초기화 시킨다.
		m_pLCItemInventory->Clear();
		
		// 버리는 아이템을 초기화 시켜준다.
		m_siThrowItemIndex	=	-1;		
		
		// 아이템을 채워준다.
		FillItemListBox();
		
		SI08 siMercenarySlot = m_pMyOnlineWorld->pMyData->GetMercenary();
		if(siMercenarySlot == m_uiSelectedSlot)
			m_BSelFollower.SetAction(TRUE);
		
		
		// '창닫기' 버튼의 상태를 설정한다.
		m_BExit.SetAction( TRUE );
		
		// 현재 사용자간의 거래 중이라면 '창닫기' 버튼을 이용할 수 없도록 한다.
		if( m_pMyOnlineWorld->pOnlineTrade->IsActive() == TRUE )
		{
			m_BExit.SetAction( FALSE );
		}
	}
	// 비활성화 시켜라
	else
	{
		// 계산기가 떠있을 경우 해제
		if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
		{
			m_bIsAction = TRUE;
			return;
		}
		
		// 드래그 중인 아이템이 있을 경우 해제
		for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			(*ix)->StopDrag();
		if(m_uiPickUpItemID != 0)
		{
			m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
			
			m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}

		// actdoll (2004/07/27 11:20) : 만약 무언가 인벤토리용 질문이 떴을 경우 모두 해제해준다.
		if( m_bMsgBoxFlag_VanishFieldMercenary || m_bMsgBoxFlag_WearTimerItem )
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetAction( false );
			m_bMsgBoxFlag_WearTimerItem = m_bMsgBoxFlag_VanishFieldMercenary = false;
			StopDrag();
			m_bIsItemDragging = 0;
		}
		
		// 노 필드 지역을 해제한다.
		if( m_hNoField != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_hNoField);
		m_hNoField	=	-1;	
		
		// 마우스를 보이도록 설정한다.
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
		
		// 아이템 리스트가 떠 있을 경우 해제
		if(m_pOnlineItemList->IsAction())	m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_NOACTION, 0, 0);
		// H2-S-05.02.02 //
		// 인벤토리 비활성화 시킬때 활성화 시켰을때 저장된 미니맵 상태로 돌려준다.
		m_pMyOnlineWorld->pOnlineMiniMap->SetActive(g_fMiniMapAct);
		// H2-E-05.02.02 //
	}		
}


//-----------------------------------------------------------------------------
// Name: FillItemListBox()
// Code: All (2004-06-07)
// Desc: 현재 내가 가지고 있는 정보대로 아이템 리스트를 갱신한다.
//-----------------------------------------------------------------------------
VOID	OnlineInventory::FillItemListBox()
{
	SI32			i;	
	MyItemData		MyItemData;
	ItemInfo		AddItemInfo;
	CItemHeader		*pItem;
	
	// 아이템 리스트 컨트롤을 초기화 시킨다.
	for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
		(*ix)->Clear();


	// 아이템 리스트 컨트롤에 아이템을 넣는다. 
	// 좌표가 있는 것부터 우선 준다.
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
	{
		if(m_pMyOnlineWorld->pMyData->GetItem(m_uiSelectedSlot, i, &MyItemData))
		{		
			if(MyItemData.siPosInInventory != -1)
			{
				AddItemInfo.uiItemID = MyItemData.uiID;
				AddItemInfo.ItemCount = MyItemData.uiQuantity;
				pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(MyItemData.uiID);
				if(pItem)				
				{
					if(pItem->m_siGroupCode & ON_ITEM_DRESS)
					{
						if(m_uiSelectedSlot == 0 )
							AddItemInfo.siColor = m_pMyOnlineWorld->pMyData->GetTradeGrade() >= pItem->m_siReqLv ? LIST_COLOR_NOT : LIST_COLOR_RED;
						else
							AddItemInfo.siColor = LIST_COLOR_NOT;
					}
					else
						AddItemInfo.siColor = m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(m_uiSelectedSlot) >= pItem->m_siReqLv ? LIST_COLOR_NOT : LIST_COLOR_RED;
				}
				m_pLCItemInventory->AddItem(AddItemInfo, MyItemData.siPosInInventory);
			}
		}
	}


	// 아이템을 장착해 준다.
	// 지금은 주인공만 장착이 가능하다.
	UI16 uiWearItem[WEAR_ITEM_NUM];
	if(m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetWearItems(m_uiSelectedSlot, uiWearItem))
	{
		for( i=0; i<WEAR_ITEM_NUM ; i++ )
		{
			if(uiWearItem[i] != 0)
			{
				AddItemInfo.uiItemID = uiWearItem[i];
				AddItemInfo.ItemCount = 1;
				AddItemInfo.siColor = LIST_COLOR_NOT;

				if		( i == ON_WEAR_HELMET_ITEM		)	m_pLCWearItemHelmet->AddItem(		AddItemInfo, 0 );
				else if	( i == ON_WEAR_ARMOR_ITEM		)	m_pLCWearItemArmor->AddItem(		AddItemInfo, 0 );
				else if	( i == ON_WEAR_BELT_ITEM		)	m_pLCWearItemBelt->AddItem(			AddItemInfo, 0 );
				else if	( i == ON_WEAR_GLOVE_ITEM		)	m_pLCWearItemGloves->AddItem(		AddItemInfo, 0 );
				else if	( i == ON_WEAR_SHOES_ITEM		)	m_pLCWearItemBoots->AddItem(		AddItemInfo, 0 );
				else if	( i == ON_WEAR_LRING_ITEM		)	m_pLCWearItemAccessoryL->AddItem(	AddItemInfo, 0 );
				else if	( i == ON_WEAR_RRING_ITEM		)	m_pLCWearItemAccessoryR->AddItem(	AddItemInfo, 0 );
				else if	( i == ON_WEAR_WEAPON_ITEM		)	m_pLCWearItemWeapon->AddItem(		AddItemInfo, 0 );
				else if	( i == ON_WEAR_GUARDIAN_ITEM	)	m_pLCWearItemGuardian->AddItem(		AddItemInfo, 0 );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name: GetThrowItemIndex()
// Code: All (2004-06-07)
// Desc: 땅바닥에 버릴 아이템의 인덱스를 가져온다.
//		return	- 버릴려고 하는 아이템의 인덱스
//-----------------------------------------------------------------------------
SI32	OnlineInventory::GetThrowItemIndex()
{	

	SI16	siItemIndex	=	m_pLCItemInventory->GetDragSlot();
	POINT	ptMouse;

	if( siItemIndex == -1 )	{ return -1;}			// 현재 아이템이 없다.	

	if( m_pMyOnlineWorld->fLeftOneSwitch == 1 || m_pMyOnlineWorld->fVillageLeftOneSwitch == 1)
	{
		
		m_pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );
		
		// 현재 아이템이 인벤토리 밖으로 나간다면
		RECT	rcRect = { m_siX, m_siY, m_siX + m_siWidth, m_siY + m_siHeight };
		if( PtInRect( &rcRect, ptMouse ) == FALSE)
			return siItemIndex;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Name: SetFieldItemDrag()
// Code: All (2004-06-07)
// Desc: 끄집어 올린 필드 아이템의 정보를 세팅한다.
//		uiItemID	- 해당 아이템 ID
//		siItemX		- 아이템을 주운 x좌표
//		siItemX		- 아이템을 주운 y좌표
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetFieldItemDrag(UI16 uiItemID, SI16 siItemX, SI16 siItemY)
{
	// Drag 모드로 변환
	m_uiPickUpItemID	= uiItemID;
	m_siPickUpItemMapPosX	= siItemX;
	m_siPickUpItemMapPosY	= siItemY;
}

//-----------------------------------------------------------------------------
// Name: SetItemPickUp()
// Code: All (2004-06-07)
// Desc: 아이템을 들어올린 상태로 세팅한다.
//		bFlag	- 들어올렸다면 true, 아니라면 false
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetItemPickUp(BOOL bFlag)
{ 
	m_bIsItemPickingUp	= bFlag; 
	m_siPollStatus		= ON_INVENTORY_STATUS_MAIN_INITPOLL; 
}

//-----------------------------------------------------------------------------
// Name: GetItemPickUpMapPos()
// Code: Actdoll (2004-07-23)
// Desc: 필드에서 아이템을 들어올린 좌표의 위치를 리턴한다.
//		ptPos	- 들어올린 아이템이 있던 맵 좌표
//-----------------------------------------------------------------------------
VOID	OnlineInventory::GetItemPickUpMapPos( POINT &ptPos )
{ 
	ptPos.x = m_siPickUpItemMapPosX; 
	ptPos.y = m_siPickUpItemMapPosY; 
}

//-----------------------------------------------------------------------------
// Name: IsWearDrag()
// Code: All (2004-06-07)
// Desc: 현재 입고 있는 장비나 아이템을 드래그중에 있는가?
//		return	- 맞다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineInventory::IsWearDrag()
{
	// actdoll (2005/02/04 15:12) : 
	//	여기 거쳐가시는 앞으로의 거상 프로그래머분들 필독하시오...
	//	STL로 for문(과 비스무리한것들 포함해서) 돌려서 정보 검색할때는 이터레이터 꼭 써야함다..-_-;;
	//	2002년도 7월 중순에 작성된, 오래되고 오래된 아래 몇줄의 소스가
	//	지금 본섭 백섭이라는 초유의 사태를 불러일으켰소...-_-+
	//	이미 한참 전에 나가신 분이라 마음껏 씹는거지만...
	//	벡터에서 이터레이터 별로 비싸지도 않소.. 
	//	STL을 사이즈로 포문돌리거나 어레이 써서 찾는건 인덱스 정확하게 심어져 있을 때만이라오... 
	//	그렇게 쓰는 것도 STL 연구진들이 피해야 할 방법이라고 강조하고 있소
	//	뒷사람 고생 안시키고 나중에 욕 안먹을라면 제발 정확히 알고 쓰시구랴...T^T)
	//	존나게 욕먹고 자존심 구겨져서리 1시간동안 디버깅하다 
	//	졸라 어이없는 부분에서 찾고나니 빡돌아서 몇글자 적습니다...T^T)9
	//	내 기존 소스 주석 칠테니 저 부분에서의 암묵적 버그를 찾아서 느껴보시구랴...
	//	p.s 참고로 예전에는 인벤토리 아이템이 제일 먼저 푸쉬되었소 -_-;;;

//	int j = m_ListControl.size();
//	for( SI16 i=1 ; i<m_ListControl.size(); i++)
//	{
//		if(m_ListControl[i]->GetDragSlot() != -1)
//		{
//			return TRUE;
//		}
//	}

	for( vector<OnlineListControl*>::iterator i = m_ListControl.begin(); i != m_ListControl.end(); ++i )
	{
		// 현재는 인벤토리 외에는 모두 장착하는 아이템류다.
		if( (*i) == m_pLCItemInventory )		continue;
		if( (*i)->GetDragSlot() != -1 )			return TRUE;
	}
	return FALSE;	
}

//-----------------------------------------------------------------------------
// Name: GetDragInfo()
// Code: All (2004-06-07)
// Desc: 현재 인벤토리 칸으로부터 드래그중인 아이템의 정보를 리턴한다.
//		siSelectLB	- 현재 선택된 용병창의 DB 슬롯 번호를 리턴한다.
//		siPos		- 현재 드래그중인 아이템의 인벤토리 번호를 리턴한다.
//		return		- 현재 드래그중인 아이템 ID를 리턴한다.
//-----------------------------------------------------------------------------
UI32	OnlineInventory::GetDragInfo( SI16 &siSelectLB, SI16 &siPos )
{
	SI32		siIndex		=	GetDragItemInfo();

	if( siIndex == -1 )		return 0;

	const ItemInfo *pItemInfo	=	m_pLCItemInventory->GetSlotInfo((const SI16)siIndex);
	siSelectLB	=	(SI16)m_uiSelectedSlot;
	siPos		=	m_pLCItemInventory->GetSlot(pItemInfo->uiItemID);

	return pItemInfo->uiItemID;
}

//-----------------------------------------------------------------------------
// Name: DragFail()
// Code: All (2004-06-07)
// Desc: 인벤토리 칸으로부터 드래그 중인 작업을 중지시킨다.
//-----------------------------------------------------------------------------
VOID	OnlineInventory::DragFail()
{
	m_bIsItemDragging	=	0;
	m_pLCItemInventory->StopDrag();
}

//-----------------------------------------------------------------------------
// Name: StopDrag()
// Code: All (2004-06-07)
// Desc: 모든 드래그 작업을 중지시키고 원래 정보를 갱신한다.
//-----------------------------------------------------------------------------
VOID	OnlineInventory::StopDrag()
{ 
	if(m_pLCItemPointer)	
	{
		m_pLCItemPointer->Clear(); 
		m_pLCItemPointer = NULL;
	}

	m_pLCItemInventory->StopDrag(); 
	FillItemListBox(); 
	m_uiPickUpItemID = 0; 
}
//-----------------------------------------------------------------------------
// Name: GetDragItemNum()
// Code: All (2004-06-07)
// Desc: 현재 인벤토리 칸으로부터 드래그중인 아이템의 갯수를 얻는다.
//		return	- 아이템 갯수. 정보가 없다면 0 리턴
//-----------------------------------------------------------------------------
SI16	OnlineInventory::GetDragItemNum(UI32 uiID)
{
	const ItemInfo *pInfo = m_pLCItemInventory->GetSlotInfo(m_pLCItemInventory->GetSlot((const)uiID));

	if( pInfo )		return pInfo->ItemCount;
	else			return 0;
}

//-----------------------------------------------------------------------------
// Name: SetMercenarySlot()
// Code: All (2004-06-07)
// Desc: 현재 선택한 용병의 DB 슬롯 번호를 세팅한다.
//		uiDBSlot	- 선택한 용병의 DB 슬롯 번호
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetMercenarySlot( UI08 uiDBSlot )
{
	m_uiSelectedSlot	=	uiDBSlot;
	m_siPortraitFontNo  =	TransKindOnlineToImjin( m_pMyOnlineWorld->pMyData->GetFollowerParameterKind(m_uiSelectedSlot) );
	m_siPortraitFontNo	=	KI[m_siPortraitFontNo].GetOnlinePortraitFont();

	// actdoll (2004/07/27 11:20) : 만약 무언가 인벤토리용 질문이 떴을 경우 슬롯이 바뀌었다면 모두 해제해준다.
	for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )	(*ix)->StopDrag();
	if( m_bMsgBoxFlag_VanishFieldMercenary || m_bMsgBoxFlag_WearTimerItem )
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetAction( false );
		m_bMsgBoxFlag_WearTimerItem = m_bMsgBoxFlag_VanishFieldMercenary = false;
		m_uiPickUpItemDestFollower		= m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
		StopDrag();
		m_bIsItemDragging	= 0;
		m_siPollStatus =	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
	}
}

//-----------------------------------------------------------------------------
// Name: GetWeight()
// Code: All (2004-06-07)
// Desc: 현재 선택한 용병이 가지고 있는 아이템 무게의 합을 계산한다.
//		uiFollower	- 계산을 원하는 용병의 DB 슬롯 번호
//		return		- 해당 용병이 가지고 있는 아이템의 총 무게
//-----------------------------------------------------------------------------
SI32	OnlineInventory::GetWeight(UI08 uiFollower)
{
	SI32			siTempWeight = 0, i;
	MyItemData		MyItemData;
	CItemHeader		*pItem;


	// 아이템 리스트 컨트롤에 아이템을 넣는다. 
	// 좌표가 있는 것부터 우선 준다.
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
	{
		if(m_pMyOnlineWorld->pMyData->GetItem(uiFollower, i, &MyItemData) == TRUE)
		{
			pItem			=	m_pMyOnlineWorld->pItemsInfo->GetItem( MyItemData.uiID );
			siTempWeight	+=	pItem->m_uiWeight * MyItemData.uiQuantity;
		}
	}

	return siTempWeight;
}

//-----------------------------------------------------------------------------
// Name: StripItem()
// Code: All (2004-07-19)
// Desc: 드래그중인 아이템을 옮겨놓는 작업을 한다.
//	return	- 섭에 제대로 신호 날렸다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineInventory::AttachItem()
{
	CItemHeader	*pItemHeader;
	SI16		siSelectSlot;
	SI16		siSelectLCType	= -1;
	SI08		uiErrorMsg		= -1;

	// 인벤토리에서부터 드래그중인 슬롯의 정보가 없다면 에러다.
	if( !m_pLCItemInventory->GetSlotInfo( m_pLCItemInventory->GetDragSlot() ) )
	{
		StopDrag();
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		m_bIsItemDragging = 0;
		return FALSE;
	}

	// robypark 2005/1/6 11:50
	// 공성전 유닛인 상태에서는 아이템을 벗을 수 없다.
	// 공성 유닛 정보 헤더 얻기
	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetFollowerCharID(m_uiSelectedSlot));
	if (NULL != lpsSiegeUnitTable)	// 공성전 유닛이다.
	{
		StopDrag();
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		m_bIsItemDragging = 0;
		return FALSE;
	}

	// 거래창이 활성화되어 있는 상태라면 제자리에 놓는다.
	if(m_pMyOnlineWorld->pOnlineTrade->IsActive())
	{
		m_pLCItemInventory->DragDown();
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		m_siPollStatus		=	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
		m_bIsItemDragging	=	0;
		FillItemListBox();
		return FALSE;
	}

	// 무기 장착 위치별로 체크 들어간다.
	if(		 ( siSelectSlot = m_pLCWearItemWeapon->GetSelectSlot()		) != -1 ){	siSelectLCType	= ON_WEAR_WEAPON_ITEM;	m_pLCItemPointer = m_pLCWearItemWeapon;		}
	else if( ( siSelectSlot = m_pLCWearItemArmor->GetSelectSlot()		) != -1 ){	siSelectLCType	= ON_WEAR_ARMOR_ITEM;	m_pLCItemPointer = m_pLCWearItemArmor;		}
	else if( ( siSelectSlot = m_pLCWearItemHelmet->GetSelectSlot()		) != -1 ){	siSelectLCType	= ON_WEAR_HELMET_ITEM;	m_pLCItemPointer = m_pLCWearItemHelmet;		}
	else if( ( siSelectSlot = m_pLCWearItemGuardian->GetSelectSlot()	) != -1 ){	siSelectLCType	= ON_WEAR_GUARDIAN_ITEM;m_pLCItemPointer = m_pLCWearItemGuardian;	}
	else if( ( siSelectSlot = m_pLCWearItemBelt->GetSelectSlot()		) != -1 ){	siSelectLCType	= ON_WEAR_BELT_ITEM;	m_pLCItemPointer = m_pLCWearItemBelt;		}
	else if( ( siSelectSlot = m_pLCWearItemGloves->GetSelectSlot()		) != -1 ){	siSelectLCType	= ON_WEAR_GLOVE_ITEM;	m_pLCItemPointer = m_pLCWearItemGloves;		}
	else if( ( siSelectSlot = m_pLCWearItemBoots->GetSelectSlot()		) != -1 ){	siSelectLCType	= ON_WEAR_SHOES_ITEM;	m_pLCItemPointer = m_pLCWearItemBoots;		}
	else if( ( siSelectSlot = m_pLCWearItemAccessoryL->GetSelectSlot()	) != -1 ){	siSelectLCType	= ON_WEAR_LRING_ITEM;	m_pLCItemPointer = m_pLCWearItemAccessoryL;	}
	else if( ( siSelectSlot = m_pLCWearItemAccessoryR->GetSelectSlot()	) != -1 ){	siSelectLCType	= ON_WEAR_RRING_ITEM;	m_pLCItemPointer = m_pLCWearItemAccessoryR;	}
	else if( ( siSelectSlot = m_pLCItemInventory->DragDown()			) != -1 ){	siSelectLCType	= 100;																}

	// 장착부쪽에 내려졌다면
	BOOL	bWorkSuccess	= true;
	if( siSelectLCType > -1 && siSelectLCType < WEAR_ITEM_NUM )
	{
		m_bIsItemDragging			= 0;																			// 드래그 상태는 끝
		m_uiPickUpItemID			= m_pLCItemInventory->GetSlotInfo(m_pLCItemInventory->GetDragSlot())->uiItemID;	// 집어든 아이템의 ID를 받는다.
		m_siPickUpItemDestSlot		= m_pLCItemInventory->GetSlot(m_uiPickUpItemID);								// 집어든 아이템의 대상 슬롯
		pItemHeader					= m_pMyOnlineWorld->pItemsInfo->GetItem((const)m_uiPickUpItemID);				// 아이템 본체 정보 가져온다.
		if( !pItemHeader || m_siPickUpItemDestSlot == -1 )	siSelectLCType	= -1;									// 아이템 정보가 잘못됐거나 슬롯이 없다면 실패

		// 아이템 정보 확인 사살
		switch( siSelectLCType )
		{
			case ON_WEAR_WEAPON_ITEM:
				{
					UI08	uiWeapon	= m_pMyOnlineWorld->pIOnlineCharKI->GetWeaponType( m_pMyOnlineWorld->pMyData->GetFollowerCharID( m_uiSelectedSlot ) );
					if( !pItemHeader->pWeaponHeader || uiWeapon == 0 )									// 무기 아이템 정보가 없거나 0이면 실패
					{
						siSelectLCType = -1;	
						break;
					}
					if( uiWeapon != ON_ITEM_MISSLE_TYPE_FREE && 
						uiWeapon != pItemHeader->pWeaponHeader->siWeaponType )	siSelectLCType = -1;	// 장착할 수 있는 무기가 아니라면 실패
				}
				break;
			case ON_WEAR_ARMOR_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_ARMOR) && 
						!(pItemHeader->m_siGroupCode & ON_ITEM_DRESS) )			siSelectLCType = -1;	// 방어구 아이템이 아니라면 실패
				}
				break;
			case ON_WEAR_HELMET_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_HELMAT) )		siSelectLCType = -1;	// 투구 아이템이 아니라면 실패
				}
				break;
			case ON_WEAR_GUARDIAN_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_GUARDIAN) )		siSelectLCType = -1;	// 수호부 아이템이 아니라면 실패
				}
				break;
			case ON_WEAR_BELT_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_BELT) )			siSelectLCType = -1;	// 혁대 아이템이 아니라면 실패
				}
				break;
			case ON_WEAR_GLOVE_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_GLOVES) )		siSelectLCType = -1;	// 장갑 아이템이 아니라면 실패
				}
				break;
			case ON_WEAR_SHOES_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_BOOTS) )			siSelectLCType = -1;	// 신발 아이템이 아니라면 실패
				}
				break;
			case ON_WEAR_RRING_ITEM:
			case ON_WEAR_LRING_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_RING) )			siSelectLCType = -1;	// 장신구 아이템이 아니라면 실패
				}
				break;
		}// EOF switch( siSelectLCType )

		// 조건식들을 무사히 통과했다면
		if( siSelectLCType != -1 )
		{
			// 해당 용병에 장착할 수 있는가 확인
			if( m_pMyOnlineWorld->pMyData->GetFollowersInterface()->CanPutOnItem(m_uiSelectedSlot, siSelectLCType, m_uiPickUpItemID, m_siPickUpItemDestSlot, m_pMyOnlineWorld->pMyData->GetTradeGrade(), uiErrorMsg) )
			{
				m_siWearSettingType	= siSelectLCType;					// 장착 타입 정해주고
				m_siPollStatus		= ON_INVENTORY_STATUS_ATTACH_ITEM;	// 폴을 아이템 장착으로 돌리고

				// actdoll (2004/07/26 11:32) : 만약 시간제 아이템일 경우 질문을 던진다.
				if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( m_uiPickUpItemID ) )
				{
					m_uiWearTimerItemDestFollower	= m_uiSelectedSlot;
					m_uiWearTimerItemID				= m_uiPickUpItemID;
					m_siWearTimerItemDestSlot		= m_siPickUpItemDestSlot;
					m_siWearTimerItemSettingType	= m_siWearSettingType;

					m_bMsgBoxFlag_WearTimerItem		= true;
					char	pszString[256];
					sprintf( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEM_ASK_WEAR_TIMERITEM), 
										m_pMyOnlineWorld->pOnlineText->Get(pItemHeader->m_siItemName), m_pMyOnlineWorld->pItemsInfo->GetItemUsingDay( m_uiPickUpItemID ) );
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TIMERITEM), pszString );
				}
				// 그게 아니라면 서버로 장착 신호를 보낸다.
				else
				{
					m_pMyOnlineWorld->pOnlineClient->SendRequestAttachItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
				}
				return TRUE;
			}

			// 에러 메시지가 있다면 체크
			ErrorMsg(uiErrorMsg);
		}// EOF if( siSelectLCType != -1 )

		// 여기까지 내려왔다면 조건식에서 어긋났다는 뜻이므로 작동을 해제해 준다.
		StopDrag();

		return FALSE;
	}// EOF if( siSelectLCType > -1 && siSelectLCType < WEAR_ITEM_NUM )
	// 인벤토리쪽에 내려졌다면
	else if( siSelectLCType == 100 )
	{
		const SI16 siYetPos = m_pLCItemInventory->GetOldDragSlot();	// 예전 슬롯을 받고
		// 이동이 맞다면
		if(siYetPos != siSelectSlot)
		{
			BOOL bCanMoveItem = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->CanMoveItem(m_uiSelectedSlot, m_pLCItemInventory->GetSlotInfo(siSelectSlot)->uiItemID, m_pLCItemInventory->GetSlotInfo(siSelectSlot)->ItemCount, m_uiSelectedSlot, siSelectSlot);
			// 아이템 이동이 가능하다면 설정
			if(bCanMoveItem)
			{
				POINT ptMouse;
				ptMouse.x = IpD.Mouse_X;
				ptMouse.y = IpD.Mouse_Y;				
				
				m_siPollStatus			= ON_INVENTORY_STATUS_RESPONSEITEMMOVE;
				m_bIsItemDragging		= 0;
				m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );

				m_uiMoveItemID				= m_pLCItemInventory->GetSlotInfo(siSelectSlot)->uiItemID;
				m_uiMoveItemDestFollower	= m_uiSelectedSlot;
				m_siMoveItemSlotBefore		= siYetPos;
				m_siMoveItemSlotAfter		= siSelectSlot;
				
				m_pMyOnlineWorld->pOnlineClient->SendItemPosInInventory(m_uiSelectedSlot, m_uiSelectedSlot, m_pLCItemInventory->GetSlotInfo(siSelectSlot)->uiItemID, siSelectSlot, m_pLCItemInventory->GetSlotInfo(siSelectSlot)->ItemCount );
				return TRUE;
			}
			// 불가능하다면 원래대로 돌린다.
			else
			{
				FillItemListBox();
			}
		}

	}// else if( siSelectLCType == 100 )

	// 아무것도 아니라면 원래대로 초기화
	m_siPollStatus		=	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
	m_bIsItemDragging	=	0;
	m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
	m_pLCItemInventory->StopDrag();

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: StripItem()
// Code: All (2004-07-19)
// Desc: 장착하고 있던 장비를 벗는 작업을 진행한다.
//	return	- 섭에 제대로 신호 날렸다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineInventory::StripItem()
{
	// 마우스가 선택한 슬롯이 없다면 에러
	if( m_pLCItemInventory->GetSelectSlot() == -1 )
	{
		for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )	(*ix)->StopDrag();
		m_bIsItemDragging	= 0;
		return FALSE;
	}

	// robypark 2005/1/6 11:50
	// 공성전 유닛인 상태에서는 아이템을 벗을 수 없다.
	// 공성 유닛 정보 헤더 얻기
	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetFollowerCharID(m_uiSelectedSlot));
	if (NULL != lpsSiegeUnitTable)	// 공성전 유닛이다.
	{
		for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			(*ix)->StopDrag();

		m_bIsItemDragging	= 0;

		return FALSE;
	}

	IOnlineFollower	*pIOF	= m_pMyOnlineWorld->pMyData->GetFollowersInterface();	// 용병 그룹 객체
	OnlineClient	*pOC	= m_pMyOnlineWorld->pOnlineClient;						// 클라이언트 송수신 모듈 객체
	
	m_bIsItemDragging	=	0;
	m_siPollStatus		= ON_INVENTORY_STATUS_STRIP_ITEM;
	m_pLCItemPointer	= NULL;		
	
	// 어느 타입에 위치한 장비가 드래그 중인 것인지 확인하여 정보를 얻는다.
	if(		 ( m_pLCWearItemWeapon->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemWeapon;		m_siWearSettingType = ON_WEAR_WEAPON_ITEM;	}
	else if( ( m_pLCWearItemArmor->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemArmor;		m_siWearSettingType = ON_WEAR_ARMOR_ITEM;	}
	else if( ( m_pLCWearItemHelmet->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemHelmet;		m_siWearSettingType = ON_WEAR_HELMET_ITEM;	}
	else if( ( m_pLCWearItemBelt->GetDragSlot()			) != -1 )	{ m_pLCItemPointer = m_pLCWearItemBelt;			m_siWearSettingType = ON_WEAR_BELT_ITEM;	}
	else if( ( m_pLCWearItemGloves->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemGloves;		m_siWearSettingType = ON_WEAR_GLOVE_ITEM;	}
	else if( ( m_pLCWearItemBoots->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemBoots;		m_siWearSettingType = ON_WEAR_SHOES_ITEM;	}
	else if( ( m_pLCWearItemAccessoryL->GetDragSlot()	) != -1 )	{ m_pLCItemPointer = m_pLCWearItemAccessoryL;	m_siWearSettingType = ON_WEAR_LRING_ITEM;	}
	else if( ( m_pLCWearItemAccessoryR->GetDragSlot()	) != -1 )	{ m_pLCItemPointer = m_pLCWearItemAccessoryR;	m_siWearSettingType = ON_WEAR_RRING_ITEM;	}
	else if( ( m_pLCWearItemGuardian->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemGuardian;		m_siWearSettingType = ON_WEAR_GUARDIAN_ITEM;}
	if( !m_pLCItemPointer )		return FALSE;	// 정보가 없다면 작업 실패다.
	
	m_uiPickUpItemID			= m_pLCItemPointer->GetSlotInfo(m_pLCItemPointer->GetDragSlot())->uiItemID;	// 아이템 아이디 받고
	m_siPickUpItemDestSlot	= m_pLCItemInventory->GetSelectSlot();										// 마우스가 올라와 있는 슬롯을 받는다.
	
	// 인벤토리에 동일한 아이템이 있을때는 그 아이템 슬롯에 현재 드래그중인 아이템을 더한다.
	if( m_pLCItemInventory->GetSlot( m_uiPickUpItemID ) != -1 )
	{
		m_siPickUpItemDestSlot = m_pLCItemInventory->GetSlot( m_uiPickUpItemID );
		if( pIOF->CanTakeOffItem( m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot ) )				
		{
			pOC->SendRequestStripItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
			return TRUE;
		}
	}
	// 마우스가 올라와 있는 슬롯이 빈 슬롯이라면 거기에 넣는다.
	else if( m_pLCItemInventory->IsEmptySlot( m_siPickUpItemDestSlot ) == -1 )
	{				
		if( pIOF->CanTakeOffItem( m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot ) )				
		{
			pOC->SendRequestStripItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
			return TRUE;
		}				
	}
	// 아무 빈 슬롯을 찾아 거기에 넣는다.
	else
	{
		m_siPickUpItemDestSlot = m_pLCItemInventory->GetEmptySlot( m_uiPickUpItemID );
		if( m_siPickUpItemDestSlot != -1 )
		{
			if( pIOF->CanTakeOffItem( m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot ) )				
			{
				pOC->SendRequestStripItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
				return TRUE;
			}
		}
	}
	
	// 문제가 있다면 드래그는 멈추고 메인폴로 세팅
	for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )	(*ix)->StopDrag();
	m_siPollStatus		= ON_INVENTORY_STATUS_MAIN_POLL;
	m_bIsItemDragging	= 0;
	m_uiPickUpItemID	= 0;
	
	DragFail();

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: CheckUseItem()
// Code: All (2004-07-19)
// Desc: 아이템 사용에 대한 작업 진행의 중간 개체(사용 확인이나 기타 등등)
//-----------------------------------------------------------------------------
VOID	OnlineInventory::CheckUseItem()
{
	BOOL bSucess;
	SI16 TempUnitCount = 0;
	SI16 TempCount = 0;
	SI16 MaxFollower = 0;
	SI16 siQuantity = 1;

	
	switch(m_siLocalStatus)
	{
	case ON_LOCAL_STATUS_USE_ITEM_POLL:
		{
			if(!((m_pMyOnlineWorld->fRightOneSwitch == 1 || m_pMyOnlineWorld->fVillageRightOneSwitch == 1) && m_pItemSlotPointer 
				&& (!m_pMyOnlineWorld->pOnlineBooth->IsActionIn()) && !m_pMyOnlineWorld->pOnlineTrade->IsActive()
				&& m_pLCItemInventory->GetDragSlot() == -1))		break;
			
			SI16 siIndex;
			CItemHeader		*pItem = NULL;	
			for(vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			{
				if((siIndex = (*ix)->GetSelectSlot()) != -1 && !m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					m_pItemSlotPointer	= (*ix)->GetSlotInfo((const)siIndex);
					if(m_pItemSlotPointer)
						pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem(m_pItemSlotPointer->uiItemID);
				}
			}
			
			
			// 아이템 소모
			if(!pItem)		return;
			
			if(!(pItem->m_Attribute & ON_ITEM_USE_POSSIBLE))	return;
			
			// 체력이 0	일때에는 부활아이템이 아니면 못쓴다.
			if( m_pMyOnlineWorld->pMyData->GetFollowerParameterLife( m_uiSelectedSlot ) == 0 && (pItem->m_siGroupCode & ON_ITEM_REVIVE) == 0)	return;
			
			bSucess = TRUE;
			
			// 특수 아이템
			if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
			{
				switch(pItem->m_siSpecial)
				{
				case ON_ITEM_SPECIAL_INITIALIZE:
					// robyprk 2004/11/5 18:26
					// 불사조의 깃털은 공성전 유닛인 상태에서는 사용이 불가능하다
					if (TRUE == m_pMyOnlineWorld->pIOnlineChar->IsSiegeWarfareUnit(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_USE_ITEM_IN_SIEGEWARFAREUNIT));
						bSucess = FALSE;
					}
					break;

				case ON_ITEM_SPECIAL_SHOWMERCENARY:
					if(!m_uiSelectedSlot)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_FOLLOWER_ONLY_USE));
						bSucess = FALSE;
					}
					break;
				case ON_ITEM_SPECIAL_CHANGESEX:
					{
						UI16 uiUniqueID, uiEquipWeapon[WEAR_ITEM_NUM];
						uiUniqueID = m_pMyOnlineWorld->pMyData->GetMyCharUniqueID();
						m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);

						// robyprk 2004/11/19 18:20
						// 규화보전은 공성전 유닛인 상태에서는 사용이 불가능하다
						if (TRUE == m_pMyOnlineWorld->pIOnlineChar->IsSiegeWarfareUnit(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_USE_ITEM_IN_SIEGEWARFAREUNIT));
							bSucess = FALSE;
							break;
						}
						
						if(uiEquipWeapon[ON_WEAR_WEAPON_ITEM])
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
								m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_WEAPON));
							bSucess = FALSE;
						}
						else if(m_uiSelectedSlot)
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
								m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_FOLLOWER));
							bSucess = FALSE;
						}
					}
					break;
				case ON_ITEM_SPECIAL_IMPERIUM:
					{
						// 소집권 사용이 불가능하다.
						bSucess = FALSE;
						break;

						// robypark 2004/11/3 17:3
						// 공성전 소집권 사용이 가능한 지역인지 검사
						if (TRUE == m_pMyOnlineWorld->SiegeWarfare_IsInAreaDisabledTransform())
						{
							// 소집권 사용이 불가능하다.
							bSucess = FALSE;
							break;
						}
						// else
							// 소집권 사용 가능한 지역, 시간, 등이다.
/*
#ifndef _DEBUG
						// robypark 2004/12/7 14:58
						// 공성전 유닛이 죽는 경우 다시 공성전 유닛으로 변신할 있는 지연(Penalty)시간이 경과했는지 검사
						if (m_pMyOnlineWorld->pMyData->GetSiegeWarfare_TransformDeadDelay())
						{
							// 소집권 사용이 불가능하다.
							bSucess = FALSE;

							//소집권을 사용할때 대방이거나 행수일경우(대장 유닛)에는 지연시간은 10분이다.
							if(m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
								m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
							{
								// 에러 메시지 박스 출력
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HAVE_PENALTY_TRANSFORM_CAPTAIN));
							}
							else
							{
								// 에러 메시지 박스 출력
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HAVE_PENALTY_TRANSFORM_UNIT));
							}
							break;
						}
						// else
							// Penalty가 없다. 변신 가능
#endif // _DEBUG
*/
						// robypark 2004/11/10 14:44
						// 선택한 공성전 소집권 아이템 정보 헤더 얻기
						CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItem(m_pItemSlotPointer->uiItemID);

						// robypark 2004/11/10 14:44
						// 명시된 캐릭터로 변신 가능한지 검사
						if (FALSE == (pItemHeader->m_pImperium->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
						{
							// 변신 불가능
							bSucess = FALSE;

							// 에러 메시지 박스 출력
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_USE_ITEM_TO_TRANSFORM_SIEGE));

							break;
						}

						//소집권을 사용할때 대방이거나 행수일경우엔 대장 유닛이 될수 있다.
						if(m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
							m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
						{
							// robypark 2004/11/10 14:44
							// 명시된 캐릭터로 된 대장 유닛으로 변신 불가능(대방 및 행수 사용 불가능)
							if (pItemHeader->m_pImperium->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
							{
								// 변신 불가능
								bSucess = FALSE;

								// 에러 메시지 박스 출력
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_USE_ITEM_TRANSFORM_CAPTAIN));

								break;
							}

							MyItemData* pMyItemData =  m_pMyOnlineWorld->pMyData->GetItem(m_uiSelectedSlot,m_pItemSlotPointer->uiItemID,m_pLCItemInventory->GetSlot(m_pItemSlotPointer->uiItemID));
							if(pMyItemData->uiQuantity >= 2)
								siQuantity = 2;
							else
							{
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEED_MORE_ITEM_TO_CAPTAIN));
								bSucess = FALSE;
							}
						}
						else
							siQuantity = 1;
					}
					break;
				case ON_ITEM_SPECIAL_CHANGENATION:
					{
						UI16 uiUniqueID, uiEquipWeapon[WEAR_ITEM_NUM];
						uiUniqueID = m_pMyOnlineWorld->pMyData->GetMyCharUniqueID();
						m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);

						// robyprk 2004/11/19 18:20
						// 귀화의 서약은 공성전 유닛인 상태에서는 사용이 불가능하다
						if (TRUE == m_pMyOnlineWorld->pIOnlineChar->IsSiegeWarfareUnit(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_USE_ITEM_IN_SIEGEWARFAREUNIT));
							bSucess = FALSE;
							break;
						}
						
						if(m_uiSelectedSlot)
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_FOLLOWER));
							bSucess = FALSE;
						}
						else if(uiEquipWeapon[ON_WEAR_WEAPON_ITEM])
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_WEAPON));
							bSucess = FALSE;
						}
						else if(pItem->m_siSpecialParam == m_pMyOnlineWorld->pMyData->GetFollowerParameterNation(0))
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_SAME_NATION));
							bSucess = FALSE;
						}
					}
					break;
				case ON_ITEM_SPECIAL_TACTICS:	// 손자 병법서 2003.08.28 수정
					if( m_pMyOnlineWorld->pMyData->siMaxGeneral >= ON_MAXGENERAL_HIGH )
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GENERAL_MAX_MSG));
						bSucess = FALSE;
						break;
					}
					break;
				case ON_ITEM_SPECIAL_DONKEY:	// 당나귀.
					{
						if(m_pMyOnlineWorld->pMyData->GetFollowerNum() >= ON_MAX_FOLLOWER_NUM)
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DONKEY_MSG));
							bSucess = FALSE;
						}
						for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
						{
							if(m_pMyOnlineWorld->pMyData->IsValidFollower(TempCount) == FALSE)
							{
								TempUnitCount++;
							}
						}
						MaxFollower = m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
						if((ON_MAX_FOLLOWER_NUM - TempUnitCount) > MaxFollower)
						{
							
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "",m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DONKEY_SLOT_MSG));
							bSucess = FALSE;
						}
					}
					break;
				case ON_ITEM_SPECIAL_MAGICSTONERED:
				case ON_ITEM_SPECIAL_MAGICSTONEBLUE:
				case ON_ITEM_SPECIAL_MAGICSTONEBLACK:
					{
						UI16 uiEquipWeapon[WEAR_ITEM_NUM];
						m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);
						CItemHeader	*pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(uiEquipWeapon[ON_WEAR_WEAPON_ITEM]);

						// 업그레이드 가능한 고급무기를 차고있는지 체크
						if(pItem)
						{
							if(pItem->pWeaponHeader->uiGrade == 10)
							{
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "더이상 업그레이드 할 수 없습니다.");
								bSucess = FALSE;
							}
							else if(pItem->m_Attribute & ON_ITEM_UPGRADE_POSSIBLE)
							{
								if(pItem->m_siSpecial == ON_ITEM_SPECIAL_MAGICSTONERED)
								{
									m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_OKNO;
									m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", "업그레이드 하시겠습니까?");
								}
								else if(pItem->m_siSpecial == ON_ITEM_SPECIAL_MAGICSTONEBLUE)
								{
									// +0 ~ +4 까지만 사용할 수 있다.
									if(pItem->pWeaponHeader->uiGrade >= 0 && pItem->pWeaponHeader->uiGrade <= 4)
									{
										m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_OKNO;
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", "업그레이드 하시겠습니까?");
									}
									else
									{
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "푸른마석은 무기등급 +4까지만 사용할 수 있습니다.");
										bSucess = FALSE;
									}
								}
								else if(pItem->m_siSpecial == ON_ITEM_SPECIAL_MAGICSTONEBLACK)
								{
									// +7, +8 만 사용할 수 있다.
									if(pItem->pWeaponHeader->uiGrade == 7 || pItem->pWeaponHeader->uiGrade == 8)
									{
										m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_OKNO;
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", "업그레이드 하시겠습니까?");
									}
									else
									{
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "검은마석은 무기등급 +7, +8 만 사용할 수 있습니다.");
										bSucess = FALSE;
									}
								}
							}
							else
							{
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "고급무기만 업그레이드 할 수 있습니다.");
								bSucess = FALSE;
							}
						}
						else
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "무기를 장착해야 업그레이드 할 수 있습니다.");
							bSucess = FALSE;
						}
					}
					break;
				}
			}

			if(bSucess)
			{
				if(m_siLocalStatus != ON_LOCAL_STATUS_USE_ITEM_OKNO)	m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_SEND;
				m_uiUseItemDestFollower	= m_uiSelectedSlot;
				m_uiUseItemID		= m_pItemSlotPointer->uiItemID;
				m_siUseItemSrcSlot	= m_pLCItemInventory->GetSlot(m_pItemSlotPointer->uiItemID);
				m_siUseQuantity		= siQuantity;
			}
		}
		break;
	case ON_LOCAL_STATUS_USE_ITEM_OKNO:
		{
			if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
				m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_SEND;
			else
				m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_POLL;
		}
		break;
	case ON_LOCAL_STATUS_USE_ITEM_SEND:
		{
			m_pMyOnlineWorld->pOnlineClient->SendRequestUseItem(m_uiUseItemDestFollower, m_siUseItemSrcSlot, m_uiUseItemID, m_siUseQuantity);
			m_bPushMouseR	= FALSE;
			m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_POLL;
			m_siPollStatus = ON_INVENTORY_STATUS_USE_ITEM_WAIT;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Name: ErrorMsg()
// Code: All (2004-07-19)
// Desc: 에러메시지 출력 함수
//		ErrorCode	- 출력할 에러메시지의 코드
//-----------------------------------------------------------------------------
VOID	OnlineInventory::ErrorMsg(SI08 ErrorCode)
{
	switch(ErrorCode)
	{
	case CREDIT_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORTRADE));
		break;
	case LEVEL_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_NOT1LV));
		break;
	case FAN_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_ITEM1));
		break;
	case DOUBLESTICK_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_ITEM2));
		break;
	case SPINSTONE_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_ITEM3));
		break;
	case SCYTHE_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_ITEM4));
		break;
	case BUDDIST_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_ITEM5));
		break;
	case NOFIND_ERROR:
		break;
	case CANTPUTONDRESS_ERROR:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_WEAR_CLOTH));
		break;
	case CANTWEARGAURDIAN_ERROR:		// 수호부 장착 에러
	case CANTWEARTIMELIMITITEM_ERROR:	// 아이템 장착 에러
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEM_ONLY_HERO) );
		break;
	}
}




//===================================================================================================
//	서버로부터 받은 메시지에 대한 처리 부분 START
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: WearItem()
// Code: All (2004-07-19)
// Desc: 응답 - 아이템의 장착
//		pMsg	- OnResWearItemMsg 객체
//-----------------------------------------------------------------------------
void OnlineInventory::WearItem(BYTE *pMsg)
{
	OnResWearItemMsg *pOnResWearItemMsg = (OnResWearItemMsg*)pMsg;

	if( pOnResWearItemMsg )
	{				
		CItemHeader					*pItem;
		SI32						siEquip = -1;

		m_siPollStatus =	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;


		// 아이템 작업이 성공일 경우
		if( pOnResWearItemMsg->dwResponse == ON_RET_WEARITEM_OK )
		{
			// 장착한 아이템 정보를 가져온다.
			pItem						=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiPickUpItemID);
			
			// 없을 경우는 그냥 갱신
			if( pItem == NULL )
			{
				m_pLCItemPointer->Clear();
				FillItemListBox();					
				m_pLCItemPointer	=	NULL;
				return;
			}

			// 나라에 대한 정보와 남자 여자 구분을 알수가 없다
			// 임시
			////////////////////////////////////////////////////
//			siEquip		=	pItem->m_siDrawID;

			// 내 자신에 대한 정보라면
			if( m_uiSelectedSlot == 0 )	
			{
				UI16 Equip[WEAR_ITEM_NUM];

				// 장비 데이터를 받고
				m_pMyOnlineWorld->pIOnlineChar->GetEquip(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Equip);
				// 해당 장비의 인덱스에 새로 장착한 장비를 세팅
				Equip[m_pMyOnlineWorld->pItemsInfo->GetWearLoc(m_uiPickUpItemID)] = m_uiPickUpItemID;
				// 내 시스템의 장착 장비를 세팅한다.
				m_pMyOnlineWorld->pIOnlineChar->SetEquipItem(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Equip);
//				m_pMyOnlineWorld->pIOnlineChar->SetEquip(TRUE, m_pMyOnlineWorld->pItemsInfo->GetWearLoc(m_uiPickUpItemID), siEquip, m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), TRUE);

				// actdoll (2005/02/14 16:11) : 무영풍 신발 속도에 대한 계산식을 %식으로 수정하여 처리
				// 시간제한 아이템에 대한 처리
				SI32	uiCharIndex		= m_pMyOnlineWorld->pIOnlineChar->GetID( m_pMyOnlineWorld->pMyData->GetMyCharUniqueID() );
				if( m_siWearSettingType == ON_WEAR_SHOES_ITEM )		// 신발 아이템일 경우만 체크
				{
					SI32	siCharKindIndex = m_pMyOnlineWorld->pIOnlineCharKI->GetIndex( m_pMyOnlineWorld->pMyData->GetMyCharKind() );
					if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( m_uiPickUpItemID ) )
					{
						// robypark 2004/11/4 11:26
						// 공성전 유닛이 아닐 경우 이동속도를 올려주는 아이템(무영풍)에 영향을 받지 않도록 수정
						if (FALSE == m_pMyOnlineWorld->pIOnlineChar->IsSiegeWarfareUnit(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))
						{
							m_pMyOnlineWorld->pIOnlineChar->GetOnlineChar()[uiCharIndex].SetMoveSpeed( m_pMyOnlineWorld->pIOnlineCharKI->GetMoveSpeed(siCharKindIndex) * DF_VALUE_CHAR_MOVE_SPEED_IN_SPECIAL_BOOTS );
						}
						else
						{
							m_pMyOnlineWorld->pIOnlineChar->GetOnlineChar()[uiCharIndex].SetMoveSpeed( m_pMyOnlineWorld->pIOnlineCharKI->GetMoveSpeed(siCharKindIndex) );
						}
					}
					else
					{
						m_pMyOnlineWorld->pIOnlineChar->GetOnlineChar()[uiCharIndex].SetMoveSpeed( m_pMyOnlineWorld->pIOnlineCharKI->GetMoveSpeed(siCharKindIndex) );
					}
				}

			}
		
			// 아이템 장착 사운드 출력
			m_pMyOnlineWorld->PlayItemEffectSound( pItem->m_siGroupCode,pItem->m_siSound );

			ItemInfo AddItem;
			AddItem.uiItemID = m_uiPickUpItemID;
			AddItem.ItemCount = 1;

			// 해당 용병에 대하여 아이템을 장착했다는 신호를 보냄
			m_pMyOnlineWorld->pMyData->GetFollowersInterface()->PutOnItem(m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot, NULL);

			// 인벤토리 작업
			m_pLCItemPointer->AddItem(AddItem, 0);
			m_pLCItemInventory->DeleteSlot(m_siPickUpItemDestSlot, 1);				
			m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
			m_pLCItemInventory->StopDrag();

			// actdoll (2004/07/20 14:44) : 장착 완료된 아이템이 시간제 아이템일 경우 메시지를 띄운다.
			if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( AddItem.uiItemID ) )
			{
				char	pszText[256];
				sprintf( pszText, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEM_CONFIRM_WEAR_TIMERITEM), 
					m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), m_pMyOnlineWorld->pItemsInfo->GetItemUsingDay( AddItem.uiItemID ) );
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TIMERITEM), pszText );
			}
		}
		else
		{
			if( m_pLCItemPointer )		StopDrag();
		}
	}
}

//-----------------------------------------------------------------------------
// Name: StripItem()
// Code: All (2004-07-19)
// Desc: 응답 - 아이템의 탈의
//		pMsg	- OnResStripItemMsg 객체
//-----------------------------------------------------------------------------
void OnlineInventory::StripItem(BYTE *pMsg)
{
	OnResStripItemMsg			*pOnResStripItemMsg = (OnResStripItemMsg*)pMsg;
	if( pOnResStripItemMsg )
	{
		if( pOnResStripItemMsg->dwResponse == ON_RET_OK )
		{
			SI32    siEquip = -1;

			m_siPollStatus = ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;

			if( pOnResStripItemMsg->dwResponse == ON_RET_WEARITEM_OK )
			{	
				if( m_uiSelectedSlot == 0 )	
				{
					UI16 Equip[WEAR_ITEM_NUM];

					m_pMyOnlineWorld->pIOnlineChar->GetEquip(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Equip);

					Equip[m_pMyOnlineWorld->pItemsInfo->GetWearLoc(m_uiPickUpItemID)] = 0;
					m_pMyOnlineWorld->pIOnlineChar->SetEquipItem(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Equip);

//					m_pMyOnlineWorld->pIOnlineChar->SetEquip(FALSE, m_siWearSettingType, -1, m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), TRUE);
				}
				

				ItemInfo AddItem;
				const ItemInfo *pTempItem = m_pLCItemInventory->GetSlotInfo(m_siPickUpItemDestSlot);
				AddItem.ItemCount = pTempItem != NULL ? pTempItem->ItemCount + 1 : 1;
				AddItem.uiItemID = m_uiPickUpItemID;
			
				// 아이템을 넣어 준다.
				m_pLCItemInventory->AddItem(AddItem, m_siPickUpItemDestSlot);			
				m_pMyOnlineWorld->pMyData->GetFollowersInterface()->TakeOffItem(m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot, NULL);
				m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
				m_pLCItemPointer->StopDrag();
				m_pLCItemPointer->Clear();						
			}
			else
			{				
				if( m_pLCItemPointer )		StopDrag();
			}
		}
		else
		{
			if( m_pLCItemPointer )
			{
				m_pLCItemPointer->StopDrag();						
				FillItemListBox();	
				m_pLCItemPointer =	NULL;
				StopDrag();
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name: MoveItem()
// Code: All (2004-07-19)
// Desc: 응답 - 아이템의 이동
//		pMsg	- OnResMoveItemMsg 객체
//-----------------------------------------------------------------------------
void OnlineInventory::MoveItem(BYTE *pMsg)
{
	OnResMoveItemMsg *pOnResMoveItemMsg = (OnResMoveItemMsg*)pMsg;

	if(pOnResMoveItemMsg)
	{

		MyItemData		MyItemData;

		if( pOnResMoveItemMsg->dwResponse == ON_RET_OK )
		{
			if( m_siPollStatus != ON_INVENTORY_STATUS_MOVE_ITEMSEND )
			{
				OnlineReceivedMyItemInfo*		lpItemData;
				OnlineReceivedMyItemInfo		AddItemData, DelItemData;

				m_siPollStatus   = ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;

				lpItemData = m_pMyOnlineWorld->pMyData->GetItem(m_uiMoveItemDestFollower, m_uiMoveItemID, m_siMoveItemSlotBefore);					

				if(lpItemData)
				{
					AddItemData.uiID = lpItemData->uiID;
					AddItemData.uiQuantity = lpItemData->uiQuantity;
					AddItemData.siPosInInventory = m_siMoveItemSlotAfter;

					memcpy( &DelItemData, lpItemData, sizeof(OnlineReceivedMyItemInfo));

					m_pMyOnlineWorld->pMyData->DelItem(m_uiMoveItemDestFollower, &DelItemData);
					m_pMyOnlineWorld->pMyData->AddItem(m_uiMoveItemDestFollower, &AddItemData);				
				}

				FillItemListBox();
				m_pLCItemInventory->StopDrag();

				return;

			}

			else
			{				
				m_siPollStatus    = ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
				m_siMoveItemIndex	= -1;

				// 이전에 아이템을 지운다.
				MyItemData.siPosInInventory	=	m_siMoveItemSlotBefore;
				MyItemData.uiID				=	m_uiMoveItemID;
				MyItemData.uiQuantity		=	(UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount;
				
				m_pMyOnlineWorld->pMyData->DelItem( m_uiMoveItemSrcFollower, &MyItemData);
				SetMercenarySlot( m_uiMoveItemSrcFollower );
				FillItemListBox();
				
				// 새로이 아이템을 추가 
				MyItemData.siPosInInventory	=	m_siMoveItemSlotAfter;
				MyItemData.uiID				=	m_uiMoveItemID;
				MyItemData.uiQuantity		=	(UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount;

				m_pMyOnlineWorld->m_pMercenary->UnLock();
				m_pMyOnlineWorld->pMyData->AddItem( m_uiMoveItemDestFollower, &MyItemData);			
				SetMercenarySlot( m_uiMoveItemDestFollower );
				FillItemListBox();

				SetMercenarySlot( m_uiMoveItemSrcFollower );
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( (int)m_uiMoveItemSrcFollower );		//.actdoll (021224)
				FillItemListBox();

				m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
				m_pLCItemInventory->StopDrag();
			}
		}
		else
		{
			m_pLCItemInventory->StopDrag();
			m_pLCItemInventory->DeleteSlot(m_siMoveItemIndex);
			m_pMyOnlineWorld->m_pMercenary->UnLock();
			SetMercenarySlot( m_uiMoveItemSrcFollower );
			m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( (int)m_uiMoveItemSrcFollower );		//.actdoll (021224)
			FillItemListBox();

			m_siMoveItemIndex	= -1;
			m_uiMoveItemDestFollower = m_uiMoveItemSrcFollower = 0;
		}
	}
	m_bIsItemMoving = FALSE;

}

//-----------------------------------------------------------------------------
// Name: ThrowItem()
// Code: All (2004-07-19)
// Desc: 응답 - 아이템의 버리기
//		pMsg	- OnResThrowItemMsg 객체
//-----------------------------------------------------------------------------
void OnlineInventory::ThrowItem(BYTE *pMsg)
{
	m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
	OnResThrowItemMsg	*pThrowItemMsg = (OnResThrowItemMsg*)pMsg;
	if(pThrowItemMsg)
	{
		SI16			siThrowItemIndexOfLC;
		MyItemData		MyItemData;

		if( pThrowItemMsg->dwResponse == ON_RET_OK )
		{
			// 버리려고 했던 아이템을 다시 얻어온다.
			siThrowItemIndexOfLC		=	GetThrowItemInfo();

			// 내 아이템 리스트에서 해당 아이템을 없앤다.				
			MyItemData.siPosInInventory =	siThrowItemIndexOfLC;
			MyItemData.uiID				=	m_uiThrowItemID;
			MyItemData.uiQuantity		=	m_uiThrowItemQuantity;

			// 나의 데이타에서 아이템을 지워준다.
			m_pMyOnlineWorld->pMyData->DelItem(m_uiThrowItemSrcFollower, &MyItemData );
			m_pMyOnlineWorld->pOnlineTradeBook->GetBookMoney(3);
			m_pLCItemInventory->DeleteSlot(siThrowItemIndexOfLC, m_uiThrowItemQuantity);
			SetMercenarySlot( m_uiThrowItemSrcFollower );
			FillItemListBox();
			m_pLCItemInventory->StopDrag();

			// 효과음 연주
			PushEffect(EFFECT_ON_DROP_ITEM, 0, 0);

			m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
			m_bIsItemMoving = FALSE;			
			m_siPollStatus  =	ON_INVENTORY_STATUS_MAIN_INITPOLL;
		}
		else
		{
			// 내 아이템 리스트에 원래 아이템을 다시 넣어준다.
			m_bIsItemMoving = FALSE;
			m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
		}
		m_uiThrowItemSrcFollower	= 0;
		m_uiThrowItemID	= 0;
		m_uiThrowItemQuantity	= 0;
	}
}

//-----------------------------------------------------------------------------
// Name: PickupItem()
// Code: All (2004-07-19)
// Desc: 응답 - 아이템의 줍기
//		pMsg	- OnResPickUpItemMsg 객체
//-----------------------------------------------------------------------------
void OnlineInventory::PickupItem(BYTE *pMsg)
{
	OnResPickUpItemMsg	*pPickUpItemMsg = (OnResPickUpItemMsg*)pMsg;

	m_bIsItemPickingUp = FALSE;

	if( pPickUpItemMsg->dwResponse == ON_RET_OK )
	{
		MyItemData MyItemData;

		// Item 추가
		MyItemData.siPosInInventory	=	m_siPickUpItemDestSlot;
		MyItemData.uiID				=	m_uiPickUpItemID;
		MyItemData.uiQuantity		=	m_uiPichUpItemQuantity;
		
		m_pMyOnlineWorld->pMyData->AddItem(m_uiPickUpItemDestFollower, &MyItemData);					
		m_pMyOnlineWorld->pOnlineTradeBook->GetBookMoney(3);

		SetFieldItemDrag(0, 0, 0);
		
		m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		
		PushEffect(EFFECT_ON_GET_ITEM, 0, 0);
		
		// 갱신
		FillItemListBox();
		
		m_siPollStatus				 = ON_INVENTORY_STATUS_MAIN_INITPOLL;
		m_bIsItemMoving				 = FALSE;
	}
	else
	{
		m_siPollStatus		 		 = ON_INVENTORY_STATUS_MAIN_INITPOLL;
		m_bIsItemMoving				 = FALSE;

		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
	}
	m_uiPickUpItemDestFollower = m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;

}

//-----------------------------------------------------------------------------
// Name: SpendItem()
// Code: All (2004-07-19)
// Desc: 응답 - 아이템의 소비
//		pMsg	- OnResSpendItemMsg 객체
//-----------------------------------------------------------------------------
void OnlineInventory::SpendItem(BYTE *pMsg)
{
	OnResSpendItemMsg	*pOnResSpendItemMsg = (OnResSpendItemMsg*)pMsg;	
	
	m_siPollStatus = ON_INVENTORY_STATUS_MAIN_INITPOLL;
	
	if(pOnResSpendItemMsg->dwResponse == ON_RET_OK)
	{
		MyItemData MyItemData;

		MyItemData.siPosInInventory	=	m_siUseItemSrcSlot;
		MyItemData.uiID				=	m_uiUseItemID;
		MyItemData.uiQuantity		=	m_siUseQuantity;
		
		CItemHeader	*pItem = NULL;
		pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiUseItemID);
		
		// 특수 아이템
		if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
		{
			switch(pItem->m_siSpecial)
			{
			case ON_ITEM_SPECIAL_INITIALIZE:
				// 불사조의 깃털
				m_pMyOnlineWorld->pMyData->ChangeFollowerParameterToBonus(m_uiUseItemDestFollower, m_pMyOnlineWorld->pOnlineCharParser, 
					m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetFollowerParameterKind(m_uiUseItemDestFollower), m_pMyOnlineWorld->pItemsInfo);
				break;
			case ON_ITEM_SPECIAL_SHOWMERCENARY:
				// 용병 보이기
				m_BSelFollower.SetAction(TRUE);
				break;
			case ON_ITEM_SPECIAL_CHANGENATION:
				m_pMyOnlineWorld->pMyData->GetFollowersInterface()->SetFollowerParameterNation(0, pItem->m_siSpecialParam);
				break;
			case ON_ITEM_SPECIAL_CREDITUP:
				m_pMyOnlineWorld->pMyData->SetTradeCredit(m_pMyOnlineWorld->pMyData->GetTradeCredit() + pItem->m_siSpecialParam, 0);
				break;
			case ON_ITEM_SPECIAL_TACTICS:
				m_pMyOnlineWorld->pMyData->siMaxGeneral++;
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(2285));					
				break;
			case ON_ITEM_SPECIAL_DONKEY://당나귀.
				break;
			case ON_ITEM_SPECIAL_IMPERIUM://소집권 발동.
				break;
			case ON_ITEM_SPECIAL_CHANGESEX:			break;
			case ON_ITEM_SPECIAL_MAGICSTONERED:		break;		// 성공, 실패, 부서짐 메세지 띄워주기(pItem->m_siSpecialParam참고)
			case ON_ITEM_SPECIAL_MAGICSTONEBLUE:	break;
			case ON_ITEM_SPECIAL_MAGICSTONEBLACK:	break;		// 주위플레이어한테 보여주기->ON_CHANGE_CHARACTERCODE 참고, 무기장착시 주고받는 패킷 참고
			default:
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_UNKNOWN_SPECIAL_ITEM));
				break;
			}
		}
		// 전체에 적용
		else if (pItem->m_siGroupCode & ON_ITEM_TOALL)
		{
			// 맨뒤 parameter는 부활아이템의 여부
			if( pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER )
				m_pMyOnlineWorld->pMyData->UseItemToAllPer( pItem->m_siHpGain, pItem->m_siMpGain, pItem->m_siHealth );
			else
				m_pMyOnlineWorld->pMyData->UseItemToAll( pItem->m_siHpGain, pItem->m_siMpGain, (pItem->m_siGroupCode & ON_ITEM_REVIVE) ? TRUE : FALSE );
		}
		// 일부에 적용
		else
		{
			m_pMyOnlineWorld->pMyData->UseItem( m_uiUseItemDestFollower, pItem->m_siHpGain, pItem->m_siMpGain, pItem->m_siHealth );
		}
		m_pMyOnlineWorld->pMyData->DelItem( m_uiUseItemDestFollower, &MyItemData );
		
		// 음식 또는 약먹는 소리를 낸다.
		if (pItem->m_siSound != 0) //m_pMyOnlineWorld->PlayItemEffectSound( pItem->m_siGroupCode,pItem->m_siSound );
			m_pMyOnlineWorld->PlayFoodEatEffectSound(pItem->m_siSound);
		FillItemListBox();
	}
	else
	{
		CItemHeader	*pItem = NULL;
		pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiUseItemID);
		
		if(!pItem)		return;
		
		// 특수 아이템
		if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
		{
			switch(pItem->m_siSpecial)
			{
			case ON_ITEM_SPECIAL_CHANGESEX:
				if(!m_uiUseItemDestFollower)
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_WEAPON));
				else
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_FOLLOWER));
				break;
			case ON_ITEM_SPECIAL_CHANGENATION:
				if(!m_uiUseItemDestFollower)
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_WEAPON_SAMENATION));
				else
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_USE_AS_FOLLOWER));
				break;
			}
		}
		else
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_EAT_ITEM));
	}

}

//-----------------------------------------------------------------------------
// Name: AddFollower()
// Code: All (2004-07-19)
// Desc: 응답 - 용병의 장착
//		pMsg	- OnAddFollower 객체
//-----------------------------------------------------------------------------
void OnlineInventory::AddFollower(BYTE *pMsg)
{
	OnAddFollower*  pOnAddFollower = (OnAddFollower*)pMsg ;

	OnFollowerInfo* pOnFollowerInfo = (OnFollowerInfo*)&pOnAddFollower->FollowerInfo;

	m_pMyOnlineWorld->pMyData->InitFollower(pOnFollowerInfo->uiSlot, pOnFollowerInfo);
}


//===================================================================================================
//	서버로부터 받은 메시지에 대한 처리 부분 END
//===================================================================================================
