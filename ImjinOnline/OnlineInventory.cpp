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


//-----------------------------------------------------------------------------
// Name: 
// Code: All (2004-06-07)
// Desc: ��� �� ���� ����
//-----------------------------------------------------------------------------
#include <GSL.h>

#include	<Mouse.h>
#include	<OnlineWorld.h>
#include	<kindinfo.h>

#include <vector>

#include	"OnlineListControl.h"
#include	"OnlineItemList.h"
#include	"OnlinePortrait.h"		// actdoll (2004/01/05 18:31) : �ʻ����
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

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 

const	R32		DF_VALUE_CHAR_MOVE_SPEED_IN_SPECIAL_BOOTS	= 1.5f;		// �ð��� �Ź� ������ ���� �� �̵� �ӵ�(������ 1.5��)
// H2-S-05.02.02 //
// �κ��丮 Ȱ��ȭ �ɶ��� �̴ϸ� ���� ���� ���� //
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
// Desc: ������
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
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
OnlineInventory::~OnlineInventory()
{
	
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-06-07)
// Desc: ó�� ���� �� ������
//		siID			- ���������� �� ��ī��Ʈ ��ȣ
//		pOnlineWorld	- OnlineWorld ����ü
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

	// �ɸ��� ������ �޴´�.
	m_siPortraitFontNo	=	m_pMyOnlineWorld->pIOnlineChar->GetImjinKind(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID());
	m_siPortraitFontNo	=	KI[m_siPortraitFontNo].GetOnlinePortraitFont();

	// �������� �ʱ�ȭ
	m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();

	// ������ ����Ʈ �ڽ��� �����Ѵ�.
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

	// ������ ����Ʈ ��Ʈ���� �����Ѵ�.	

	m_pLCWearItemGuardian->Init		(60, 60, 1, 1, m_siX + 121, m_siY + 35, ON_ITEM_GUARDIAN);				// ��ȣ��
	m_pLCWearItemHelmet->Init		(60, 60, 1, 1, m_siX + 185, m_siY + 35, ON_ITEM_HELMAT);				// ���
	m_pLCWearItemGloves->Init		(60, 60, 1, 1, m_siX + 249, m_siY + 35, ON_ITEM_GLOVES);				// �尩

	m_pLCWearItemWeapon->Init		(60, 60, 1, 1, m_siX + 121, m_siY + 99, ON_ITEM_SWORD);					// ����
	m_pLCWearItemArmor->Init		(60, 60, 1, 1, m_siX + 185, m_siY + 99, ON_ITEM_ARMOR | ON_ITEM_DRESS);	// ����
	m_pLCWearItemBelt->Init			(60, 60, 1, 1, m_siX + 249, m_siY + 99, ON_ITEM_BELT);					// ��Ʈ	

	m_pLCWearItemAccessoryL->Init	(60, 60, 1, 1, m_siX + 121, m_siY + 163, ON_ITEM_RING | ON_ITEM_LRING);	// �¹���
	m_pLCWearItemBoots->Init		(60, 60, 1, 1, m_siX + 185, m_siY + 163, ON_ITEM_BOOTS);				// ����
	m_pLCWearItemAccessoryR->Init	(60, 60, 1, 1, m_siX + 249, m_siY + 163, ON_ITEM_RING | ON_ITEM_RRING);	// �����

	// actdoll (2005/02/01 19:29) : 
	//	�κ��丮 ������ ��Ʈ���� ������ ON_ITEM_...�ø����� �ε��� ��ġ�� ��ġ�ϱ� ���� ������� �۾��ߴ�.
	//	���� ������ ����� push ��ġ �ٲ��� ����!	=======================
	m_ListControl.push_back(m_pLCWearItemHelmet);
	m_ListControl.push_back(m_pLCWearItemWeapon);
	m_ListControl.push_back(m_pLCWearItemArmor);
	m_ListControl.push_back(m_pLCWearItemBelt);
	m_ListControl.push_back(m_pLCWearItemGloves);
	m_ListControl.push_back(m_pLCWearItemBoots);
	m_ListControl.push_back(m_pLCWearItemAccessoryR);
	m_ListControl.push_back(m_pLCWearItemAccessoryL);
	m_ListControl.push_back(m_pLCWearItemGuardian);
	//	���� ������ ����� push ��ġ �ٲ��� ����!	=======================
	m_ListControl.push_back(m_pLCItemInventory);
		
	m_BSelFollower.Create(m_siX + 25, m_siY + 99, m_MBut2Image.Header.Xsize, m_MBut2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_FOLLOWER_NOT_SHOW), BUTTON_PUT_LEFT, FALSE);
	m_BWearItem.Create(m_siX + 25, m_siY + 187, m_MBut2Image.Header.Xsize, m_MBut2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000500), BUTTON_PUT_LEFT, TRUE);
	m_BItemVillage.Create(m_siX + 25, m_siY + 207, m_MBut2Image.Header.Xsize, m_MBut2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000501), BUTTON_PUT_LEFT, TRUE);
	m_BExit.Create(m_siX + 139, m_siY + 394, m_MBut1Image.Header.Xsize, m_MBut1Image.Header.Ysize+1, m_pMyOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE);

	m_uiSelectedSlot			= 0;
	m_uiPickUpItemID			= 0;
	m_siPickUpItemMapPosX		= 0;
	m_siPickUpItemMapPosY		= 0;

	// �� ���� ��Ʈ������ ��ư�� ����� ���콺 ���¸� üũ�Ѵ�.
	m_pLCItemInventory->StopDrag();
}
 
//-----------------------------------------------------------------------------
// Name: Free()
// Code: All (2004-06-07)
// Desc: �κ��丮 ���ҽ� ����� ����
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
// Desc: �ʱ�ȭ
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
// Desc: ���μ��� ���ۺ�
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

	// ���콺�� �κ��丮 �ȿ� �ִٸ� �κ��丮�� �°� ���콺�� ���� �Ѵ�.
	if(IpD.Mouse_X >= m_siX && IpD.Mouse_Y <= (m_siX + m_siWidth - 1) && IpD.Mouse_Y >= m_siY && IpD.Mouse_Y <= (m_siY + m_siHeight - 1))	
		m_pMyOnlineWorld->pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);		

	// ������ ����Ʈ �׼�
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
			// ���� �̵��� �������� �ִٸ�,,, ������ ���� �����ش�.
			if( m_pMyOnlineWorld->pOnlineNumPad->siCount != 0 )
			{
				m_siPollStatus			= ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
				CItemHeader	*pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem( m_uiMoveItemID );

				/********************************************************************************/
				// robypark 2005/1/13 18:39
				// ������ �������� �뺴 ���԰� �̵��� ������ ������ ���� ���� �ð����� 
				// ���� ����ð� �������̴�.
				// ��, ���� ������ �������� ��ܿ����� ������ �� �� �ִ� �ð��� ����
				if (pItem)
				{
					// ������ ���������� �˻�
					if (NULL != pItem->m_pImperium)
					{
						/*
						// ���� ������ �������� �ٸ� �뺴 �������� �ű� �� �ִ� �ð��ΰ�?
						if(FALSE == m_pMyOnlineWorld->IsInTimeGiveArmyItem())
						*/
						{
							// �ű� �� �ִ� �ð��� �ƴϴ�.
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
						// ������ �������� �̵��Ѱ��� �˸���.
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

			// ������ �����ϴٸ�...
			// ������ �˸���.
			if( m_siMoveItemSlotAfter != -1 )
			{			
				m_siPollStatus	=	ON_INVENTORY_STATUS_MAIN_POLL;
				ItemInfo TempAddItem;
				TempAddItem.siType = m_pMyOnlineWorld->pOnlineTrade->m_siAddType = LIST_ITEM;
				TempAddItem.uiItemID = m_uiMoveItemID;
				TempAddItem.ItemCount = m_pMyOnlineWorld->pOnlineNumPad->siCount;
				
				m_pMyOnlineWorld->pOnlineTrade->AddItem( m_siMoveItemSlotBefore, m_siMoveItemSlotAfter, m_uiMoveItemID, (UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount, m_uiSelectedSlot);

				// ��� �κ��丮���� ���� �ش�.				
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
		// actdoll (2004/07/25 0:22) : �κ��丮 ���� ��
		//----------------------------------------------------------------------------
	case ON_INVENTORY_STATUS_MAIN_POLL:
		{
			SI16			siIndex;
			CItemHeader		*pItem;

			// ������ �� �ִ� ������ ����Ʈ ��ư�� ������ ��
			if(m_BWearItem.Process(m_bPushMouseL))	
			{
				m_BWearItem.SetAction(FALSE);
				m_BItemVillage.SetAction(FALSE);
				m_siPollStatus = ON_INVENTORY_STATUS_WEARITEMLIST_SELECT;
				break;
			}
			// ������ ��� ���� ����Ʈ ��ư�� ������ ��
			else if(m_BItemVillage.Process(m_bPushMouseL))
			{
				m_BWearItem.SetAction(FALSE);
				m_BItemVillage.SetAction(FALSE);
				m_siPollStatus = ON_INVENTORY_STATUS_VILLAGELIST_SELECT;
				break;
			}
			
			// ���콺�� ������ �������� ���� ����â ��� ����
			for(vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			{
				if((siIndex = (*ix)->GetSelectSlot()) != -1 && !m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					m_pItemSlotPointer	= (*ix)->GetSlotInfo((const)siIndex);
					pItem		= m_pItemSlotPointer == NULL ? NULL : m_pMyOnlineWorld->pItemsInfo->GetItem( m_pItemSlotPointer->uiItemID );
					
					if(!pItem)				continue;
					
					if(m_pItemSlotPointer->bHide)	continue;
					
					// actdoll (2005/02/14 15:52) : �ð��� �����ۿ� ǥ�⸦ ���� �߰�����
					// ���� ������ �뺴�� ������ ���
					OnlineFollower *pOF = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetFollower( m_uiSelectedSlot );

					// ���� ���� ��ġ���� üũ ����.
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

			// �ٴڿ��� ����� �������� �ִٸ�
			if(m_uiPickUpItemID != 0)
			{
				// ���� ���̶�� �߸��� ��Ȳ�̴ٿ�
				if( m_bIsVillage )		break;	

//				if(!m_bIsVillage)
//				{				
				// ������ ��ǥ�� �� ���� �ִ� �������� ������ �ް�
				// �� �������� ���� �뺴 ���� ��ȣ�� ���� ��
				// �� �뺴�� �������� �ֿ����� �� �ִ� ������ �޴´�.
				SI16 siQuantity				= m_pMyOnlineWorld->pOnlineMap->GetItemQuantity(m_siPickUpItemMapPosX, m_siPickUpItemMapPosY);
				m_uiPickUpItemDestFollower	= m_uiSelectedSlot;
				m_uiPichUpItemQuantity		= m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake(m_uiPickUpItemDestFollower, m_uiPickUpItemID);		

				// ���� ////////////////////////
				// robypark 2004/11/27 10:59
				// �ٴڿ� ������ �ִ� ������ ������ �������� ��ڳ� ��� ����5�� ���� �� �ִ�.
				CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiPickUpItemID);

				if (pItemHeader)
				{
					if (NULL != pItemHeader->m_pImperium)
					{
#ifndef _DEBUG
						// ��� ��� 5�̰ų�, ��� ĳ������ ��쿡�� �ٴڿ� ������ ������ �������� ���� �� �ִ�.
						if ((m_pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5) ||
								(0 == strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pMyOnlineWorld->pOnlineText->Get(8000495))))
						{
							// �� ���ڱ���~~
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
				// �� ////////////////////////
				
				// ���̻� �������� �� �� ���� ��Ȳ�̶�� �޽��� ���� ������ �ʱ�ȭ ��Ų��.
				if( m_uiPichUpItemQuantity == 0 )
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NO_GAIN));
					m_uiPickUpItemDestFollower	= m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
					m_bIsItemPickingUp			= FALSE;
					m_siPollStatus				=	ON_INVENTORY_STATUS_MAIN_INITPOLL;
					m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
					m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
				}
				// ������ ������ ��θ� ������ �� �ִٸ�
				else if( siQuantity <= m_uiPichUpItemQuantity )
				{
					m_uiPichUpItemQuantity = siQuantity;
					m_siPollStatus = ON_INVENTORY_STATUS_MAIN_INIT_ITEMPICKUP;					
				}
				// �Ϻθ� ������ �� �ִٸ�
				else if( siQuantity > m_uiPichUpItemQuantity )
				{
					m_siPollStatus = ON_INVENTORY_STATUS_MAIN_INPUT_ITEMPICKUP;
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( m_uiPichUpItemQuantity );
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit( NUMBERCONTROL_UNIT_THING );
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );
				}
				// actdoll (2004/07/25 0:40) : ���� �ڵ�� �ѵ�.. �� ���ǽĿ��� ������� ������ �� ������? ��_��;
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
			// �ٴڿ��� ����� �������� ���ٸ�
			else
			{
				// ���� ���� ������ ����Ʈ â�� �� �ִٸ� �巡�״� ������.
				if( m_pOnlineItemList->IsAction() )
				{
					m_pLCItemInventory->StopDrag();
					break;
				}

				// Ư�� �����۵鿡 ���� ��� ó��
				CheckUseItem();

				// �ٴڿ� ���� ������ ������ �ִٸ�
				if((siThrowItemIndexOfLC = GetThrowItemIndex()) != -1)
				{
					// ���� ���� ���
					if(m_bIsVillage)
					{
						// �ָ� ������ ����� ��� ����
						if(m_pMyOnlineWorld->m_siInnStatus == 2)		break;

						// �뺴â�� Ȱ��ȭ �Ǿ��ִٸ�
						if(m_pMyOnlineWorld->m_pMercenary->IsAction())
						{
							// ���콺�� �ö� �ִ� �뺴â�� DB ���� ��ȣ�� �޾Ҵٸ� �� �������ͽ��� ������ ������� �ٲ۴�.
							if((siSelectedSell = m_pMyOnlineWorld->m_pMercenary->GetMouseOverSlotNo(TRUE)) != -1)
							{
								m_siPollStatus =	ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW;
							}
							// �ƴ϶�� ������� ������!
							else
							{
								m_pLCItemInventory->StopDrag();
								m_bIsItemDragging = 0;
							}
						}
						// �뺴â ��Ȱ��ȭ��� ������� ������!
						else
						{
							m_pLCItemInventory->StopDrag();
							m_bIsItemDragging = 0;
						}
					}
					// ���� ���� ���
					else
					{
						//if( m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )		break;
						if( m_siEnteringBuildingType != 0 )							break;	// ���� �ȿ����� �������� �������� ����
						if( m_pMyOnlineWorld->pOnlineReception->IsActive())			break;	// ��� ��Ȳ�̳�?
						if( m_pMyOnlineWorld->pOnlineSmith->IsActive())				break;	// �������� ��Ȳ�̳�?
						if( m_pMyOnlineWorld->pOnlineTrade->IsActive())				break;	// �ŷ� ��Ȳ�̳�?
						m_siPollStatus =	ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW;		// �� �������ͽ��� ������ ������� �ٲ۴�.
					}
				}
				// �ٴڿ� ���� ������ ������ ���ٸ�
				else
				{
					// �뺴 ���̱�/�Ⱥ��̱� ��ư ������ ��
					if(m_BSelFollower.Process(m_bPushMouseL))
					{
						// ����ٴϴ� �뺴�� ���� ��ȣ�� ��ġ�Ѵٸ� �޽��� ���� Ȯ�� ���
						if( m_pMyOnlineWorld->pMyData->GetMercenary() == m_uiSelectedSlot && m_uiSelectedSlot )
						{
							m_bMsgBoxFlag_VanishFieldMercenary = TRUE;
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_NOT_SHOW_FOLLOWER_NOTICE));
						}
					}

					// �޽��� �ڽ����� OK ����� �������� ���
					if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
					{
						// �뺴 ���� �޽��� �ڽ��� �� ���¶��
						if(m_bMsgBoxFlag_VanishFieldMercenary)
						{
							m_bMsgBoxFlag_VanishFieldMercenary = FALSE;
							m_BSelFollower.SetAction(FALSE);
							m_pMyOnlineWorld->pOnlineClient->SendRequestDeleteVMercenary();
						}
					}
					// �޽��� �ڽ����� ��� ����� �������� ���
					else if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL)
					{
						m_bMsgBoxFlag_VanishFieldMercenary = FALSE;
					}

					// �κ��丮 â ������ ���δ�.
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
					// �ƴ϶��
					else
					{
						// ������ �巡�׸� �����ߴ��� Ȯ��
						if( m_bIsItemDragging == 0																		&&	// ������ �巡�� ���� �ƴϰ�
							( m_pMyOnlineWorld->fLeftOneSwitch == 1 || m_pMyOnlineWorld->fVillageLeftOneSwitch == 1 ) 	&&	// ���� ���콺�� ���� �����̰�
							!m_pMyOnlineWorld->pOnlineMsgBox->IsActive()												&&	// �޽��� �ڽ� Ȱ��ȭ ���°� �ƴϰ�
							m_pMyOnlineWorld->pOnlineBooth->GetList()->GetDragSlot() == -1								&&	// ���ǿ��� �������� �巡�� ���� �ƴϰ�
							!m_pMyOnlineWorld->m_bItemDraging															&&	// ������ �巡�� ���� �ƴϸ�
							!m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive()										&&	// �ŷ� �޽��� �ڽ��� ���ִ� ���°� �ƴϰ�
							!m_pMyOnlineWorld->pOnlineBooth->GetFollowerSlot(m_uiSelectedSlot) )							// ���ǿ��� ��ȯ�ϰ��� �ϴ� �뺴 ������ ����
						{
							if(m_pMyOnlineWorld->m_bEvent == FALSE)
							{
								for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
								{
									// ������ �巡�� ���� ���� Ȯ�εƴٸ�
									if( (*ix)->DragUp() )	
									{
										m_bIsItemDragging = 1;

										// actdoll (2004/07/20 8:39) : ������ �ð��� �������� ��� �巡�װ� �Ұ����ϵ��� �����Ѵ�.
										if( (*ix) != m_pLCItemInventory )	// �κ��丮 ��Ȳ�� �ƴ϶��
										{
											// �ð��� �������� ���� ����
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

								// �巡�� ���� �´ٸ�
								if( m_bIsItemDragging == 1 )
								{
									m_siPollStatus			=	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
									m_bPushMouseL			=	FALSE;
									m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( FALSE );
								}
							}

						}
						// �巡�� ���� �������� ���������� Ȯ��
						else if( m_bIsItemDragging == 1	&&																// �������� �巡�� ���̰�
								(m_pMyOnlineWorld->fLeftOneSwitch==1 || m_pMyOnlineWorld->fVillageLeftOneSwitch ==1))	// ���� ��ư�� ���ȴ°�?
						{
							if(m_pMyOnlineWorld->m_bEvent == FALSE)
							{
								if(m_pLCItemInventory->GetDragSlot() != -1)	AttachItem();	// �κ��丮�������� �������� �������ٸ� ���� ó�� ����.
								else										StripItem();	// �װ��� �ƴ϶�� ���� ���� ó�� ����.
							}
							//else
							//	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "�̺�Ʈ �Ⱓ�߿��� ���⸦ �����ϰų� �ű�� �����ϴ�.");
						}
						
					}
				}
			}
		}// EOF case ON_INVENTORY_STATUS_MAIN_POLL:
		break;

	case ON_INVENTORY_STATUS_MAIN_INIT_ITEMTHROW:
		{			
			SI16	siTempThrowItemIndex = m_pLCItemInventory->GetDragSlot();

			// ���� ���� �ŷ� ���̶�� 
			if(m_pMyOnlineWorld->pOnlineTrade->IsActive())
			{
				OnlineListControl	*pTempList = m_pMyOnlineWorld->pOnlineTrade->GetList();						

				// �ŷ�â �ȿ� �������� �ִٸ�
				if(pTempList->GetSelectSlot() != -1)
				{
					// ���� // �� �κ����� ���� �ʴ´�. /////////////////////////////////
					// robypark 2004/11/24 11:37
					// ������ ������ �ŷ��� ���� ��ܿ� ���̸� �����ϴ�.
					// �ŷ��Ϸ��� ������ ���� ��� ���
					const ItemInfo	*pInfo	 =	m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex);

					// ���� ������ ���� ��ȿ���� �˻�
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
						// ������ ���������� �˻�
						if (NULL != pItemHeader->m_pImperium)
						{
							/*
							// ���� ������ �������� ��ܿ����� ������ �� �� �ִ� �ð��ΰ�?
							if(m_pMyOnlineWorld->IsInTimeGiveArmyItem())
							*/
							{
								// ������ ������ �ŷ��� ���� ��ܿ� ���̸� �����ϴ�.
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
					// �� ///////////////////////////////////

					// �뺴�� �������� �ŷ��Ұ�
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

			// ĳ���� ������ �̵�
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
						// ������ �������� �뺴 ���԰� �̵��� ������ ������ ���� ���� �ð����� 
						// ���� ����ð� �������̴�.
						// ��, ���� ������ �������� ��ܿ����� ������ �� �� �ִ� �ð��� ����
						CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiMoveItemID);

						if (pItemHeader)
						{
							// ������ ���������� �˻�
							if (NULL != pItemHeader->m_pImperium)
							{
								/*
								// ���� ������ �������� �ٸ� �뺴 �������� �ű� �� �ִ� �ð��ΰ�?
								if(FALSE == m_pMyOnlineWorld->IsInTimeGiveArmyItem())
								*/
								{
									// �ű� �� �ִ� �ð��� �ƴϴ�.
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

			// ���������϶�
			if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
			{
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
				break;
			}

			// ���� ///////////////////////////////////
			// robypark 2004/11/24 11:37
			// �ٴڿ� ���� �� �ִ� ���������� �˻��ϱ�
			// �ŷ��Ϸ��� ������ ���� ��� ���
			const ItemInfo	*pInfo	 =	m_pLCItemInventory->GetSlotInfo(siTempThrowItemIndex);

			// ���� ������ ���� ��ȿ���� �˻�
			if (NULL == pInfo)
			{
				// ������ �������� ���� �� ����
				m_siPollStatus = ON_INVENTORY_STATUS_MAIN_POLL;
				//m_bIsItemDragging	=	0;
				//m_pLCItemInventory->StopDrag();
				break;
			}

			CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(pInfo->uiItemID);

			if (pItemHeader)
			{
				// ������ ���������� �˻�
				if (NULL != pItemHeader->m_pImperium)
				{
#ifndef _DEBUG
					// ��� ��� 5�̰ų�, ��� ĳ������ ��쿡�� ������ �������� �ٴڿ� ���� �� �ִ�.
					if ((m_pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5) ||
						(0 == strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pMyOnlineWorld->pOnlineText->Get(8000495))))
					{
					}
					else
					{
						// ������ �������� ���� �� ����
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
			// �� ///////////////////////////////////

			// �������� ������.			
			m_siPollStatus		=	ON_INVENTORY_STATUS_ITEMTHROW;
			m_bIsItemDragging	=	0;
			
			// �������� ���δ�.
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
		
		// ������ �������� ���´�.
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
							// ������ ������ ���� ���
							m_uiThrowItemSrcFollower	= m_uiSelectedSlot;
							m_uiThrowItemID	= pItemInfo->uiItemID;
							m_uiThrowItemQuantity	= m_pMyOnlineWorld->pOnlineNumPad->siCount;

							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.dwMsgType        = ON_REQUEST_THROWITEM;
							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.uiItemID         = pItemInfo->uiItemID;
							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.uiQuantity       = (SI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;
							m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg.uiFollowerID     = m_uiSelectedSlot;
							m_pMyOnlineWorld->pOnlineClient->SendThrowItem(&m_pMyOnlineWorld->OnlineTempOnReqThrowItemMsg);					// ���� ��û
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
				
				// Server�� Item�� �ݾ��ٰ� �޼���
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
				
			// Server�� Item�� �ݾ��ٰ� �޼���
			m_pMyOnlineWorld->pOnlineClient->SendPickUpItem(&m_pMyOnlineWorld->OnlineTempOnReqPickUpItemMsg);

			m_siPollStatus			 	 = ON_INVENTORY_STATUS_MAIN_ITEMPICKUP;
			
		}
		break;

		//----------------------------------------------------------------------------
		// actdoll (2004/07/25 0:22) : �κ��丮 ������ ���� ��
		//----------------------------------------------------------------------------
	case ON_INVENTORY_STATUS_ATTACH_ITEM:
		{
			// actdoll (2004/07/26 12:27) : �ð��� ������ ���� �޽��� ó��
			if( m_bMsgBoxFlag_WearTimerItem )	// ������ ���� �������� �� �ִٸ�
			{
				if( m_uiWearTimerItemID )
				{
					// �޽��� �ڽ����� OK ����� �������� ���
					if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
					{
						m_bMsgBoxFlag_WearTimerItem = false;
						m_pMyOnlineWorld->pOnlineClient->SendRequestAttachItem( m_uiWearTimerItemDestFollower, m_siWearTimerItemDestSlot, m_uiWearTimerItemID, m_siWearTimerItemSettingType );
					}
					// �޽��� �ڽ����� ��� ����� �������� ���
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
	//	���� 5�� ��Ʈ���߿� �Ѱ���� �̵����� �������� �ִٸ� ���콺�� �����.
	//	�׷���, �޽��� �ڽ��� ����ϴ� ���¶�� ���콺�� �����ش�.
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

	// ��Ȱ��ȭ ��Ų��.
	if(IsSetInActive == TRUE)
	{
		SetActive(FALSE);
	}
}


//-----------------------------------------------------------------------------
// Name: Draw()
// Code: All (2004-06-07)
// Desc: �׸��� ���ۺ�
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

	// �κ��丮 ������ �׸���.
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siX, m_siY, m_MainImage.Header.Xsize, m_MainImage.Header.Ysize, m_MainImage.Image);

		// actdoll (2004/01/05 18:29) : ���ο� ����� �ʻ�׸���
		if( siHP <= 0 )		siDrawFlag = DF_FLAG_PORTRAIT_HALFALPHA;	// �׾��ִٸ� ���������� �׷���
		
		// robypark 2004/11/19 16:59
		// ���ΰ� ĳ�����̸�,
		if (0 == m_uiSelectedSlot)
		{
			// ���� ������ �����̴�
			if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit()  & ON_GAMEOPTION_SIEGEWARFAREUNIT)
			{
				// ���� �� ĳ���� DB ID ����
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

		// ������ ����Ʈ�� ������ �κ��丮�� �����ش�.
		if(m_pOnlineItemList->IsAction())
			clGrp.PutSpriteJin(m_siX, m_siY, m_MOverImage.Header.Xsize, m_MOverImage.Header.Ysize, m_MOverImage.Image);
		
		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		// ���� ���
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

		// ������
		MakeMoneyText(pMD->GetMoney(), (char*)szWeight);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 50, m_siY + 369, 114, 14, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 49, m_siY + 370, 114, 14, szWeight, RGB(250, 250, 250));

		// ĳ���� ������ ��� �ش�.
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 114, m_siY + 7, 118, 13, pMD->GetFollowerCharName(m_uiSelectedSlot), RGB( 250, 250, 250 ) );

		sprintf(szWeight, "LV.%d", pMD->GetFollowerParameterLevel(m_uiSelectedSlot));	// ����
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 36, m_siY + 128, 60, 13, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 35, m_siY + 129, 60, 13, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "HP");			// ü��
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 31, m_siY + 145, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 30, m_siY + 146, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "%d/%d", pMD->GetFollowerParameterLife(m_uiSelectedSlot), pMD->GetFollowerParameterMaxLife(m_uiSelectedSlot));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 51, m_siY + 145, 46, 13, szWeight, RGB(10, 10, 10));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 50, m_siY + 146, 46, 13, szWeight, RGB(250, 250, 250));

		sprintf(szWeight, "MP");			// ������
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

	// ���� Ȥ�� ������ �ִ� �����۵��� �׸� ǥ��
	vector<OnlineListControl*>::iterator ix, ixOverDraw = m_ListControl.end();
	for( ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
	{
		(*ix)->Draw( pSurface );
		if( (*ix)->GetDragSlot() != -1 )	ixOverDraw = ix;
	}
	if( ixOverDraw != m_ListControl.end() )	(*ixOverDraw)->Draw( pSurface );	// �巡������ �������� ���� ���� �� ���� �׷��ش�.


	// actdoll (2004/07/19 14:52) : = �ð��� ������ = ������ ���� �ϼ� ǥ��
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
			if( bIsTimerItem )		// Ÿ�̸� �������� ��� �ش� ������ ���� �ϴܿ� ���� ��¥�� ǥ�����ش�.
			{
				DWORD	dwColor			= 0;
				char	pszString[32]	= {0,};
				switch( i ) 
				{
					case ON_WEAR_GUARDIAN_ITEM	:	siAddX	= 121;	siAddY	= 35;	break;	// �»��
					case ON_WEAR_HELMET_ITEM	:	siAddX	= 185;	siAddY	= 35;	break;	// �����
					case ON_WEAR_GLOVE_ITEM		:	siAddX	= 249;	siAddY	= 35;	break;	// ����
					case ON_WEAR_WEAPON_ITEM	:	siAddX	= 121;	siAddY	= 99;	break;	// ���ߴ�
					case ON_WEAR_ARMOR_ITEM		:	siAddX	= 185;	siAddY	= 99;	break;	// ���ߴ�
					case ON_WEAR_BELT_ITEM		:	siAddX	= 249;	siAddY	= 99;	break;	// ���ߴ�
					case ON_WEAR_LRING_ITEM		:	siAddX	= 121;	siAddY	= 163;	break;	// ���ϴ�
					case ON_WEAR_SHOES_ITEM		:	siAddX	= 185;	siAddY	= 163;	break;	// ���ϴ�
					case ON_WEAR_RRING_ITEM		:	siAddX	= 249;	siAddY	= 163;	break;	// ���ϴ�
				}

				// actdoll (2005/02/04 9:11) : �ð��� ������ ���� ��� ����
				if( siLimitDate < siRemainDate )		// �Ⱓ ���� ��
				{
					dwColor	= RGB( 0, 250, 250 );
					strcpy( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_READY_TO_START) );
				}
				else if( siRemainDate < 0 )				// ���� ������ ������� ���� ���� �������̶���.
				{
					dwColor	= RGB( 250, 120, 0 );
					strcpy( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_IS_EXPIRED) );
				}
				else if( siRemainDate <= 5 )			// 5�� ���� ������ ��
				{
					dwColor	= RGB( 250, 250, 0 );
					sprintf( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_REMAIN_TIME), siRemainDate );
				}
				else									// �Ϲ����϶�
				{
					dwColor	= m_pMyOnlineWorld->PaletteWhiteColor;
					sprintf( pszString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMINFO_TIMERITEM_REMAIN_TIME), siRemainDate );
				}

				// �۾� ���
				m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + siAddX + 1, m_siY + siAddY + 1, pszString, 0 );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + siAddX, m_siY + siAddY, pszString, dwColor );
			}
		}

		pSurface->ReleaseDC(hDC);
	}

	// �������� ������ ����Ʈ ���
	m_pOnlineItemList->Draw(pSurface);

	// ���� �ʵ忡�� ���ø� �������� �ִٸ�
	if(m_uiPickUpItemID != 0)
	{
		// ���� Field Item�� ��� �ִٸ� �� Field Item�� ����Ѵ�.
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
	// cltOnlineWorld::FieldDraw()���� pOnlineMsgBox�� �׷��ִ� ��ƾ�� �̹� �����Ѵ�.
	// ���� �޽��� �ڽ��� �� �� �׸��Ե����� �� ���� ��ȭ���ڰ� �ƴ϶� ������ ��ȭ���ڰ� �ȴ�. �ð��� ������ ����ǰ ����� �����ϴ� �޽��� �ڽ�.
	// �̷� ���Ͽ� ���� �ڵ带 �ּ�ó���Ѵ�.
//	m_pMyOnlineWorld->pOnlineMsgBox->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));

	clGrp.SetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
	
}

//-----------------------------------------------------------------------------
// Name: SetActive()
// Code: All (2004-06-07)
// Desc: �� Ŭ������ ���� ���� ���� Ȯ��
//		bActive		- ���۽�ų���� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetActive( BOOL bActive )
{ 
	if( !m_pLCItemInventory )		return;

	m_bIsAction	= bActive;
	
	// Ȱ��ȭ ���Ѷ�
	if(bActive == TRUE) 
	{
		m_siPollStatus		=	ON_INVENTORY_STATUS_MAIN_INITPOLL;
		m_bIsItemDragging	=	0;
		g_fMiniMapAct		=	FALSE;
		// �� �ʵ� ������ �����Ѵ�.
		if( m_hNoField != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_hNoField );
		m_hNoField	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + m_siWidth, m_siY + m_siHeight);
		
		if(!m_bIsItemPickingUp)
		{
			if( m_pMyOnlineWorld->pOnlineMiniMap->IsActive() )
			{
				// H2-S-05.02.15 //
				// �κ��丮 Ȱ��ȭ ��Ű�� ���� �̴ϸ��� ���¸� �����Ѵ�.
				// Ǯ�� �����̸� �� ��Ȱ��ȭ
				if(m_pMyOnlineWorld->pOnlineMiniMap->ISFullMode())
					g_fMiniMapAct = FALSE;
				else
				// ���� �̴ϸ� �����̸� �� ���¸� ����
					g_fMiniMapAct = m_pMyOnlineWorld->pOnlineMiniMap->IsActive();					
				// H2-E-05.02.15 //
			}
		}
		
		// ������ ����Ʈ ��Ʈ���� �ʱ�ȭ ��Ų��.
		m_pLCItemInventory->Clear();
		
		// ������ �������� �ʱ�ȭ �����ش�.
		m_siThrowItemIndex	=	-1;		
		
		// �������� ä���ش�.
		FillItemListBox();
		
		SI08 siMercenarySlot = m_pMyOnlineWorld->pMyData->GetMercenary();
		if(siMercenarySlot == m_uiSelectedSlot)
			m_BSelFollower.SetAction(TRUE);
		
		
		// 'â�ݱ�' ��ư�� ���¸� �����Ѵ�.
		m_BExit.SetAction( TRUE );
		
		// ���� ����ڰ��� �ŷ� ���̶�� 'â�ݱ�' ��ư�� �̿��� �� ������ �Ѵ�.
		if( m_pMyOnlineWorld->pOnlineTrade->IsActive() == TRUE )
		{
			m_BExit.SetAction( FALSE );
		}
	}
	// ��Ȱ��ȭ ���Ѷ�
	else
	{
		// ���Ⱑ ������ ��� ����
		if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
		{
			m_bIsAction = TRUE;
			return;
		}
		
		// �巡�� ���� �������� ���� ��� ����
		for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			(*ix)->StopDrag();
		if(m_uiPickUpItemID != 0)
		{
			m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
			
			m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}

		// actdoll (2004/07/27 11:20) : ���� ���� �κ��丮�� ������ ���� ��� ��� �������ش�.
		if( m_bMsgBoxFlag_VanishFieldMercenary || m_bMsgBoxFlag_WearTimerItem )
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetAction( false );
			m_bMsgBoxFlag_WearTimerItem = m_bMsgBoxFlag_VanishFieldMercenary = false;
			StopDrag();
			m_bIsItemDragging = 0;
		}
		
		// �� �ʵ� ������ �����Ѵ�.
		if( m_hNoField != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_hNoField);
		m_hNoField	=	-1;	
		
		// ���콺�� ���̵��� �����Ѵ�.
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);
		
		// ������ ����Ʈ�� �� ���� ��� ����
		if(m_pOnlineItemList->IsAction())	m_pOnlineItemList->SetAction(ON_ITEMLIST_TYPE_NOACTION, 0, 0);
		// H2-S-05.02.02 //
		// �κ��丮 ��Ȱ��ȭ ��ų�� Ȱ��ȭ �������� ����� �̴ϸ� ���·� �����ش�.
		m_pMyOnlineWorld->pOnlineMiniMap->SetActive(g_fMiniMapAct);
		// H2-E-05.02.02 //
	}		
}


//-----------------------------------------------------------------------------
// Name: FillItemListBox()
// Code: All (2004-06-07)
// Desc: ���� ���� ������ �ִ� ������� ������ ����Ʈ�� �����Ѵ�.
//-----------------------------------------------------------------------------
VOID	OnlineInventory::FillItemListBox()
{
	SI32			i;	
	MyItemData		MyItemData;
	ItemInfo		AddItemInfo;
	CItemHeader		*pItem;
	
	// ������ ����Ʈ ��Ʈ���� �ʱ�ȭ ��Ų��.
	for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
		(*ix)->Clear();


	// ������ ����Ʈ ��Ʈ�ѿ� �������� �ִ´�. 
	// ��ǥ�� �ִ� �ͺ��� �켱 �ش�.
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


	// �������� ������ �ش�.
	// ������ ���ΰ��� ������ �����ϴ�.
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
// Desc: ���ٴڿ� ���� �������� �ε����� �����´�.
//		return	- �������� �ϴ� �������� �ε���
//-----------------------------------------------------------------------------
SI32	OnlineInventory::GetThrowItemIndex()
{	

	SI16	siItemIndex	=	m_pLCItemInventory->GetDragSlot();
	POINT	ptMouse;

	if( siItemIndex == -1 )	{ return -1;}			// ���� �������� ����.	

	if( m_pMyOnlineWorld->fLeftOneSwitch == 1 || m_pMyOnlineWorld->fVillageLeftOneSwitch == 1)
	{
		
		m_pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );
		
		// ���� �������� �κ��丮 ������ �����ٸ�
		RECT	rcRect = { m_siX, m_siY, m_siX + m_siWidth, m_siY + m_siHeight };
		if( PtInRect( &rcRect, ptMouse ) == FALSE)
			return siItemIndex;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Name: SetFieldItemDrag()
// Code: All (2004-06-07)
// Desc: ������ �ø� �ʵ� �������� ������ �����Ѵ�.
//		uiItemID	- �ش� ������ ID
//		siItemX		- �������� �ֿ� x��ǥ
//		siItemX		- �������� �ֿ� y��ǥ
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetFieldItemDrag(UI16 uiItemID, SI16 siItemX, SI16 siItemY)
{
	// Drag ���� ��ȯ
	m_uiPickUpItemID	= uiItemID;
	m_siPickUpItemMapPosX	= siItemX;
	m_siPickUpItemMapPosY	= siItemY;
}

//-----------------------------------------------------------------------------
// Name: SetItemPickUp()
// Code: All (2004-06-07)
// Desc: �������� ���ø� ���·� �����Ѵ�.
//		bFlag	- ���÷ȴٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetItemPickUp(BOOL bFlag)
{ 
	m_bIsItemPickingUp	= bFlag; 
	m_siPollStatus		= ON_INVENTORY_STATUS_MAIN_INITPOLL; 
}

//-----------------------------------------------------------------------------
// Name: GetItemPickUpMapPos()
// Code: Actdoll (2004-07-23)
// Desc: �ʵ忡�� �������� ���ø� ��ǥ�� ��ġ�� �����Ѵ�.
//		ptPos	- ���ø� �������� �ִ� �� ��ǥ
//-----------------------------------------------------------------------------
VOID	OnlineInventory::GetItemPickUpMapPos( POINT &ptPos )
{ 
	ptPos.x = m_siPickUpItemMapPosX; 
	ptPos.y = m_siPickUpItemMapPosY; 
}

//-----------------------------------------------------------------------------
// Name: IsWearDrag()
// Code: All (2004-06-07)
// Desc: ���� �԰� �ִ� ��� �������� �巡���߿� �ִ°�?
//		return	- �´ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineInventory::IsWearDrag()
{
	// actdoll (2005/02/04 15:12) : 
	//	���� ���İ��ô� �������� �Ż� ���α׷��Ӻе� �ʵ��Ͻÿ�...
	//	STL�� for��(�� �񽺹����Ѱ͵� �����ؼ�) ������ ���� �˻��Ҷ��� ���ͷ����� �� ����Դ�..-_-;;
	//	2002�⵵ 7�� �߼��� �ۼ���, �����ǰ� ������ �Ʒ� ������ �ҽ���
	//	���� ���� �鼷�̶�� ������ ���¸� �ҷ������׼�...-_-+
	//	�̹� ���� ���� ������ ���̶� ������ �ô°�����...
	//	���Ϳ��� ���ͷ����� ���� ������� �ʼ�.. 
	//	STL�� ������� ���������ų� ��� �Ἥ ã�°� �ε��� ��Ȯ�ϰ� �ɾ��� ���� �����̶��... 
	//	�׷��� ���� �͵� STL ���������� ���ؾ� �� ����̶�� �����ϰ� �ּ�
	//	�޻�� ��� �Ƚ�Ű�� ���߿� �� �ȸ������ ���� ��Ȯ�� �˰� ���ñ���...T^T)
	//	������ ��԰� ������ ���������� 1�ð����� ������ϴ� 
	//	���� ���̾��� �κп��� ã���� �����Ƽ� ����� �����ϴ�...T^T)9
	//	�� ���� �ҽ� �ּ� ĥ�״� �� �κп����� �Ϲ��� ���׸� ã�Ƽ� �������ñ���...
	//	p.s ����� �������� �κ��丮 �������� ���� ���� Ǫ���Ǿ��� -_-;;;

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
		// ����� �κ��丮 �ܿ��� ��� �����ϴ� �����۷���.
		if( (*i) == m_pLCItemInventory )		continue;
		if( (*i)->GetDragSlot() != -1 )			return TRUE;
	}
	return FALSE;	
}

//-----------------------------------------------------------------------------
// Name: GetDragInfo()
// Code: All (2004-06-07)
// Desc: ���� �κ��丮 ĭ���κ��� �巡������ �������� ������ �����Ѵ�.
//		siSelectLB	- ���� ���õ� �뺴â�� DB ���� ��ȣ�� �����Ѵ�.
//		siPos		- ���� �巡������ �������� �κ��丮 ��ȣ�� �����Ѵ�.
//		return		- ���� �巡������ ������ ID�� �����Ѵ�.
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
// Desc: �κ��丮 ĭ���κ��� �巡�� ���� �۾��� ������Ų��.
//-----------------------------------------------------------------------------
VOID	OnlineInventory::DragFail()
{
	m_bIsItemDragging	=	0;
	m_pLCItemInventory->StopDrag();
}

//-----------------------------------------------------------------------------
// Name: StopDrag()
// Code: All (2004-06-07)
// Desc: ��� �巡�� �۾��� ������Ű�� ���� ������ �����Ѵ�.
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
// Desc: ���� �κ��丮 ĭ���κ��� �巡������ �������� ������ ��´�.
//		return	- ������ ����. ������ ���ٸ� 0 ����
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
// Desc: ���� ������ �뺴�� DB ���� ��ȣ�� �����Ѵ�.
//		uiDBSlot	- ������ �뺴�� DB ���� ��ȣ
//-----------------------------------------------------------------------------
VOID	OnlineInventory::SetMercenarySlot( UI08 uiDBSlot )
{
	m_uiSelectedSlot	=	uiDBSlot;
	m_siPortraitFontNo  =	TransKindOnlineToImjin( m_pMyOnlineWorld->pMyData->GetFollowerParameterKind(m_uiSelectedSlot) );
	m_siPortraitFontNo	=	KI[m_siPortraitFontNo].GetOnlinePortraitFont();

	// actdoll (2004/07/27 11:20) : ���� ���� �κ��丮�� ������ ���� ��� ������ �ٲ���ٸ� ��� �������ش�.
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
// Desc: ���� ������ �뺴�� ������ �ִ� ������ ������ ���� ����Ѵ�.
//		uiFollower	- ����� ���ϴ� �뺴�� DB ���� ��ȣ
//		return		- �ش� �뺴�� ������ �ִ� �������� �� ����
//-----------------------------------------------------------------------------
SI32	OnlineInventory::GetWeight(UI08 uiFollower)
{
	SI32			siTempWeight = 0, i;
	MyItemData		MyItemData;
	CItemHeader		*pItem;


	// ������ ����Ʈ ��Ʈ�ѿ� �������� �ִ´�. 
	// ��ǥ�� �ִ� �ͺ��� �켱 �ش�.
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
// Desc: �巡������ �������� �Űܳ��� �۾��� �Ѵ�.
//	return	- ���� ����� ��ȣ ���ȴٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineInventory::AttachItem()
{
	CItemHeader	*pItemHeader;
	SI16		siSelectSlot;
	SI16		siSelectLCType	= -1;
	SI08		uiErrorMsg		= -1;

	// �κ��丮�������� �巡������ ������ ������ ���ٸ� ������.
	if( !m_pLCItemInventory->GetSlotInfo( m_pLCItemInventory->GetDragSlot() ) )
	{
		StopDrag();
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		m_bIsItemDragging = 0;
		return FALSE;
	}

	// robypark 2005/1/6 11:50
	// ������ ������ ���¿����� �������� ���� �� ����.
	// ���� ���� ���� ��� ���
	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetFollowerCharID(m_uiSelectedSlot));
	if (NULL != lpsSiegeUnitTable)	// ������ �����̴�.
	{
		StopDrag();
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		m_bIsItemDragging = 0;
		return FALSE;
	}

	// �ŷ�â�� Ȱ��ȭ�Ǿ� �ִ� ���¶�� ���ڸ��� ���´�.
	if(m_pMyOnlineWorld->pOnlineTrade->IsActive())
	{
		m_pLCItemInventory->DragDown();
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		m_siPollStatus		=	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
		m_bIsItemDragging	=	0;
		FillItemListBox();
		return FALSE;
	}

	// ���� ���� ��ġ���� üũ ����.
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

	// �������ʿ� �������ٸ�
	BOOL	bWorkSuccess	= true;
	if( siSelectLCType > -1 && siSelectLCType < WEAR_ITEM_NUM )
	{
		m_bIsItemDragging			= 0;																			// �巡�� ���´� ��
		m_uiPickUpItemID			= m_pLCItemInventory->GetSlotInfo(m_pLCItemInventory->GetDragSlot())->uiItemID;	// ����� �������� ID�� �޴´�.
		m_siPickUpItemDestSlot		= m_pLCItemInventory->GetSlot(m_uiPickUpItemID);								// ����� �������� ��� ����
		pItemHeader					= m_pMyOnlineWorld->pItemsInfo->GetItem((const)m_uiPickUpItemID);				// ������ ��ü ���� �����´�.
		if( !pItemHeader || m_siPickUpItemDestSlot == -1 )	siSelectLCType	= -1;									// ������ ������ �߸��ưų� ������ ���ٸ� ����

		// ������ ���� Ȯ�� ���
		switch( siSelectLCType )
		{
			case ON_WEAR_WEAPON_ITEM:
				{
					UI08	uiWeapon	= m_pMyOnlineWorld->pIOnlineCharKI->GetWeaponType( m_pMyOnlineWorld->pMyData->GetFollowerCharID( m_uiSelectedSlot ) );
					if( !pItemHeader->pWeaponHeader || uiWeapon == 0 )									// ���� ������ ������ ���ų� 0�̸� ����
					{
						siSelectLCType = -1;	
						break;
					}
					if( uiWeapon != ON_ITEM_MISSLE_TYPE_FREE && 
						uiWeapon != pItemHeader->pWeaponHeader->siWeaponType )	siSelectLCType = -1;	// ������ �� �ִ� ���Ⱑ �ƴ϶�� ����
				}
				break;
			case ON_WEAR_ARMOR_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_ARMOR) && 
						!(pItemHeader->m_siGroupCode & ON_ITEM_DRESS) )			siSelectLCType = -1;	// �� �������� �ƴ϶�� ����
				}
				break;
			case ON_WEAR_HELMET_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_HELMAT) )		siSelectLCType = -1;	// ���� �������� �ƴ϶�� ����
				}
				break;
			case ON_WEAR_GUARDIAN_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_GUARDIAN) )		siSelectLCType = -1;	// ��ȣ�� �������� �ƴ϶�� ����
				}
				break;
			case ON_WEAR_BELT_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_BELT) )			siSelectLCType = -1;	// ���� �������� �ƴ϶�� ����
				}
				break;
			case ON_WEAR_GLOVE_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_GLOVES) )		siSelectLCType = -1;	// �尩 �������� �ƴ϶�� ����
				}
				break;
			case ON_WEAR_SHOES_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_BOOTS) )			siSelectLCType = -1;	// �Ź� �������� �ƴ϶�� ����
				}
				break;
			case ON_WEAR_RRING_ITEM:
			case ON_WEAR_LRING_ITEM:
				{
					if( !(pItemHeader->m_siGroupCode & ON_ITEM_RING) )			siSelectLCType = -1;	// ��ű� �������� �ƴ϶�� ����
				}
				break;
		}// EOF switch( siSelectLCType )

		// ���ǽĵ��� ������ ����ߴٸ�
		if( siSelectLCType != -1 )
		{
			// �ش� �뺴�� ������ �� �ִ°� Ȯ��
			if( m_pMyOnlineWorld->pMyData->GetFollowersInterface()->CanPutOnItem(m_uiSelectedSlot, siSelectLCType, m_uiPickUpItemID, m_siPickUpItemDestSlot, m_pMyOnlineWorld->pMyData->GetTradeGrade(), uiErrorMsg) )
			{
				m_siWearSettingType	= siSelectLCType;					// ���� Ÿ�� �����ְ�
				m_siPollStatus		= ON_INVENTORY_STATUS_ATTACH_ITEM;	// ���� ������ �������� ������

				// actdoll (2004/07/26 11:32) : ���� �ð��� �������� ��� ������ ������.
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
				// �װ� �ƴ϶�� ������ ���� ��ȣ�� ������.
				else
				{
					m_pMyOnlineWorld->pOnlineClient->SendRequestAttachItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
				}
				return TRUE;
			}

			// ���� �޽����� �ִٸ� üũ
			ErrorMsg(uiErrorMsg);
		}// EOF if( siSelectLCType != -1 )

		// ������� �����Դٸ� ���ǽĿ��� ��߳��ٴ� ���̹Ƿ� �۵��� ������ �ش�.
		StopDrag();

		return FALSE;
	}// EOF if( siSelectLCType > -1 && siSelectLCType < WEAR_ITEM_NUM )
	// �κ��丮�ʿ� �������ٸ�
	else if( siSelectLCType == 100 )
	{
		const SI16 siYetPos = m_pLCItemInventory->GetOldDragSlot();	// ���� ������ �ް�
		// �̵��� �´ٸ�
		if(siYetPos != siSelectSlot)
		{
			BOOL bCanMoveItem = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->CanMoveItem(m_uiSelectedSlot, m_pLCItemInventory->GetSlotInfo(siSelectSlot)->uiItemID, m_pLCItemInventory->GetSlotInfo(siSelectSlot)->ItemCount, m_uiSelectedSlot, siSelectSlot);
			// ������ �̵��� �����ϴٸ� ����
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
			// �Ұ����ϴٸ� ������� ������.
			else
			{
				FillItemListBox();
			}
		}

	}// else if( siSelectLCType == 100 )

	// �ƹ��͵� �ƴ϶�� ������� �ʱ�ȭ
	m_siPollStatus		=	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;
	m_bIsItemDragging	=	0;
	m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
	m_pLCItemInventory->StopDrag();

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: StripItem()
// Code: All (2004-07-19)
// Desc: �����ϰ� �ִ� ��� ���� �۾��� �����Ѵ�.
//	return	- ���� ����� ��ȣ ���ȴٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineInventory::StripItem()
{
	// ���콺�� ������ ������ ���ٸ� ����
	if( m_pLCItemInventory->GetSelectSlot() == -1 )
	{
		for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )	(*ix)->StopDrag();
		m_bIsItemDragging	= 0;
		return FALSE;
	}

	// robypark 2005/1/6 11:50
	// ������ ������ ���¿����� �������� ���� �� ����.
	// ���� ���� ���� ��� ���
	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetFollowerCharID(m_uiSelectedSlot));
	if (NULL != lpsSiegeUnitTable)	// ������ �����̴�.
	{
		for( vector<OnlineListControl*>::iterator ix = m_ListControl.begin(); ix != m_ListControl.end(); ++ix )
			(*ix)->StopDrag();

		m_bIsItemDragging	= 0;

		return FALSE;
	}

	IOnlineFollower	*pIOF	= m_pMyOnlineWorld->pMyData->GetFollowersInterface();	// �뺴 �׷� ��ü
	OnlineClient	*pOC	= m_pMyOnlineWorld->pOnlineClient;						// Ŭ���̾�Ʈ �ۼ��� ��� ��ü
	
	m_bIsItemDragging	=	0;
	m_siPollStatus		= ON_INVENTORY_STATUS_STRIP_ITEM;
	m_pLCItemPointer	= NULL;		
	
	// ��� Ÿ�Կ� ��ġ�� ��� �巡�� ���� ������ Ȯ���Ͽ� ������ ��´�.
	if(		 ( m_pLCWearItemWeapon->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemWeapon;		m_siWearSettingType = ON_WEAR_WEAPON_ITEM;	}
	else if( ( m_pLCWearItemArmor->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemArmor;		m_siWearSettingType = ON_WEAR_ARMOR_ITEM;	}
	else if( ( m_pLCWearItemHelmet->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemHelmet;		m_siWearSettingType = ON_WEAR_HELMET_ITEM;	}
	else if( ( m_pLCWearItemBelt->GetDragSlot()			) != -1 )	{ m_pLCItemPointer = m_pLCWearItemBelt;			m_siWearSettingType = ON_WEAR_BELT_ITEM;	}
	else if( ( m_pLCWearItemGloves->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemGloves;		m_siWearSettingType = ON_WEAR_GLOVE_ITEM;	}
	else if( ( m_pLCWearItemBoots->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemBoots;		m_siWearSettingType = ON_WEAR_SHOES_ITEM;	}
	else if( ( m_pLCWearItemAccessoryL->GetDragSlot()	) != -1 )	{ m_pLCItemPointer = m_pLCWearItemAccessoryL;	m_siWearSettingType = ON_WEAR_LRING_ITEM;	}
	else if( ( m_pLCWearItemAccessoryR->GetDragSlot()	) != -1 )	{ m_pLCItemPointer = m_pLCWearItemAccessoryR;	m_siWearSettingType = ON_WEAR_RRING_ITEM;	}
	else if( ( m_pLCWearItemGuardian->GetDragSlot()		) != -1 )	{ m_pLCItemPointer = m_pLCWearItemGuardian;		m_siWearSettingType = ON_WEAR_GUARDIAN_ITEM;}
	if( !m_pLCItemPointer )		return FALSE;	// ������ ���ٸ� �۾� ���д�.
	
	m_uiPickUpItemID			= m_pLCItemPointer->GetSlotInfo(m_pLCItemPointer->GetDragSlot())->uiItemID;	// ������ ���̵� �ް�
	m_siPickUpItemDestSlot	= m_pLCItemInventory->GetSelectSlot();										// ���콺�� �ö�� �ִ� ������ �޴´�.
	
	// �κ��丮�� ������ �������� �������� �� ������ ���Կ� ���� �巡������ �������� ���Ѵ�.
	if( m_pLCItemInventory->GetSlot( m_uiPickUpItemID ) != -1 )
	{
		m_siPickUpItemDestSlot = m_pLCItemInventory->GetSlot( m_uiPickUpItemID );
		if( pIOF->CanTakeOffItem( m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot ) )				
		{
			pOC->SendRequestStripItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
			return TRUE;
		}
	}
	// ���콺�� �ö�� �ִ� ������ �� �����̶�� �ű⿡ �ִ´�.
	else if( m_pLCItemInventory->IsEmptySlot( m_siPickUpItemDestSlot ) == -1 )
	{				
		if( pIOF->CanTakeOffItem( m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot ) )				
		{
			pOC->SendRequestStripItem( m_uiSelectedSlot, m_siPickUpItemDestSlot, m_uiPickUpItemID, m_siWearSettingType );
			return TRUE;
		}				
	}
	// �ƹ� �� ������ ã�� �ű⿡ �ִ´�.
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
	
	// ������ �ִٸ� �巡�״� ���߰� �������� ����
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
// Desc: ������ ��뿡 ���� �۾� ������ �߰� ��ü(��� Ȯ���̳� ��Ÿ ���)
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
			
			
			// ������ �Ҹ�
			if(!pItem)		return;
			
			if(!(pItem->m_Attribute & ON_ITEM_USE_POSSIBLE))	return;
			
			// ü���� 0	�϶����� ��Ȱ�������� �ƴϸ� ������.
			if( m_pMyOnlineWorld->pMyData->GetFollowerParameterLife( m_uiSelectedSlot ) == 0 && (pItem->m_siGroupCode & ON_ITEM_REVIVE) == 0)	return;
			
			bSucess = TRUE;
			
			// Ư�� ������
			if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
			{
				switch(pItem->m_siSpecial)
				{
				case ON_ITEM_SPECIAL_INITIALIZE:
					// robyprk 2004/11/5 18:26
					// �һ����� ������ ������ ������ ���¿����� ����� �Ұ����ϴ�
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
						// ��ȭ������ ������ ������ ���¿����� ����� �Ұ����ϴ�
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
						// ������ ����� �Ұ����ϴ�.
						bSucess = FALSE;
						break;

						// robypark 2004/11/3 17:3
						// ������ ������ ����� ������ �������� �˻�
						if (TRUE == m_pMyOnlineWorld->SiegeWarfare_IsInAreaDisabledTransform())
						{
							// ������ ����� �Ұ����ϴ�.
							bSucess = FALSE;
							break;
						}
						// else
							// ������ ��� ������ ����, �ð�, ���̴�.
/*
#ifndef _DEBUG
						// robypark 2004/12/7 14:58
						// ������ ������ �״� ��� �ٽ� ������ �������� ������ �ִ� ����(Penalty)�ð��� ����ߴ��� �˻�
						if (m_pMyOnlineWorld->pMyData->GetSiegeWarfare_TransformDeadDelay())
						{
							// ������ ����� �Ұ����ϴ�.
							bSucess = FALSE;

							//�������� ����Ҷ� ����̰ų� ����ϰ��(���� ����)���� �����ð��� 10���̴�.
							if(m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
								m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
							{
								// ���� �޽��� �ڽ� ���
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HAVE_PENALTY_TRANSFORM_CAPTAIN));
							}
							else
							{
								// ���� �޽��� �ڽ� ���
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HAVE_PENALTY_TRANSFORM_UNIT));
							}
							break;
						}
						// else
							// Penalty�� ����. ���� ����
#endif // _DEBUG
*/
						// robypark 2004/11/10 14:44
						// ������ ������ ������ ������ ���� ��� ���
						CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItem(m_pItemSlotPointer->uiItemID);

						// robypark 2004/11/10 14:44
						// ��õ� ĳ���ͷ� ���� �������� �˻�
						if (FALSE == (pItemHeader->m_pImperium->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE))
						{
							// ���� �Ұ���
							bSucess = FALSE;

							// ���� �޽��� �ڽ� ���
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_USE_ITEM_TO_TRANSFORM_SIEGE));

							break;
						}

						//�������� ����Ҷ� ����̰ų� ����ϰ�쿣 ���� ������ �ɼ� �ִ�.
						if(m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) ||
							m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
						{
							// robypark 2004/11/10 14:44
							// ��õ� ĳ���ͷ� �� ���� �������� ���� �Ұ���(��� �� ��� ��� �Ұ���)
							if (pItemHeader->m_pImperium->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
							{
								// ���� �Ұ���
								bSucess = FALSE;

								// ���� �޽��� �ڽ� ���
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
						// ��ȭ�� ������ ������ ������ ���¿����� ����� �Ұ����ϴ�
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
				case ON_ITEM_SPECIAL_TACTICS:	// ���� ������ 2003.08.28 ����
					if( m_pMyOnlineWorld->pMyData->siMaxGeneral >= ON_MAXGENERAL_HIGH )
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GENERAL_MAX_MSG));
						bSucess = FALSE;
						break;
					}
					break;
				case ON_ITEM_SPECIAL_DONKEY:	// �糪��.
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

						// ���׷��̵� ������ ��޹��⸦ �����ִ��� üũ
						if(pItem)
						{
							if(pItem->pWeaponHeader->uiGrade == 10)
							{
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "���̻� ���׷��̵� �� �� �����ϴ�.");
								bSucess = FALSE;
							}
							else if(pItem->m_Attribute & ON_ITEM_UPGRADE_POSSIBLE)
							{
								if(pItem->m_siSpecial == ON_ITEM_SPECIAL_MAGICSTONERED)
								{
									m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_OKNO;
									m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", "���׷��̵� �Ͻðڽ��ϱ�?");
								}
								else if(pItem->m_siSpecial == ON_ITEM_SPECIAL_MAGICSTONEBLUE)
								{
									// +0 ~ +4 ������ ����� �� �ִ�.
									if(pItem->pWeaponHeader->uiGrade >= 0 && pItem->pWeaponHeader->uiGrade <= 4)
									{
										m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_OKNO;
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", "���׷��̵� �Ͻðڽ��ϱ�?");
									}
									else
									{
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "Ǫ�������� ������ +4������ ����� �� �ֽ��ϴ�.");
										bSucess = FALSE;
									}
								}
								else if(pItem->m_siSpecial == ON_ITEM_SPECIAL_MAGICSTONEBLACK)
								{
									// +7, +8 �� ����� �� �ִ�.
									if(pItem->pWeaponHeader->uiGrade == 7 || pItem->pWeaponHeader->uiGrade == 8)
									{
										m_siLocalStatus = ON_LOCAL_STATUS_USE_ITEM_OKNO;
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", "���׷��̵� �Ͻðڽ��ϱ�?");
									}
									else
									{
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "���������� ������ +7, +8 �� ����� �� �ֽ��ϴ�.");
										bSucess = FALSE;
									}
								}
							}
							else
							{
								m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "��޹��⸸ ���׷��̵� �� �� �ֽ��ϴ�.");
								bSucess = FALSE;
							}
						}
						else
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", "���⸦ �����ؾ� ���׷��̵� �� �� �ֽ��ϴ�.");
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
// Desc: �����޽��� ��� �Լ�
//		ErrorCode	- ����� �����޽����� �ڵ�
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
	case CANTWEARGAURDIAN_ERROR:		// ��ȣ�� ���� ����
	case CANTWEARTIMELIMITITEM_ERROR:	// ������ ���� ����
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMEQ_EQ), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEM_ONLY_HERO) );
		break;
	}
}




//===================================================================================================
//	�����κ��� ���� �޽����� ���� ó�� �κ� START
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: WearItem()
// Code: All (2004-07-19)
// Desc: ���� - �������� ����
//		pMsg	- OnResWearItemMsg ��ü
//-----------------------------------------------------------------------------
void OnlineInventory::WearItem(BYTE *pMsg)
{
	OnResWearItemMsg *pOnResWearItemMsg = (OnResWearItemMsg*)pMsg;

	if( pOnResWearItemMsg )
	{				
		CItemHeader					*pItem;
		SI32						siEquip = -1;

		m_siPollStatus =	ON_INVENTORY_STATUS_MOVE_ITEM_DELAY;


		// ������ �۾��� ������ ���
		if( pOnResWearItemMsg->dwResponse == ON_RET_WEARITEM_OK )
		{
			// ������ ������ ������ �����´�.
			pItem						=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiPickUpItemID);
			
			// ���� ���� �׳� ����
			if( pItem == NULL )
			{
				m_pLCItemPointer->Clear();
				FillItemListBox();					
				m_pLCItemPointer	=	NULL;
				return;
			}

			// ���� ���� ������ ���� ���� ������ �˼��� ����
			// �ӽ�
			////////////////////////////////////////////////////
//			siEquip		=	pItem->m_siDrawID;

			// �� �ڽſ� ���� �������
			if( m_uiSelectedSlot == 0 )	
			{
				UI16 Equip[WEAR_ITEM_NUM];

				// ��� �����͸� �ް�
				m_pMyOnlineWorld->pIOnlineChar->GetEquip(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Equip);
				// �ش� ����� �ε����� ���� ������ ��� ����
				Equip[m_pMyOnlineWorld->pItemsInfo->GetWearLoc(m_uiPickUpItemID)] = m_uiPickUpItemID;
				// �� �ý����� ���� ��� �����Ѵ�.
				m_pMyOnlineWorld->pIOnlineChar->SetEquipItem(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Equip);
//				m_pMyOnlineWorld->pIOnlineChar->SetEquip(TRUE, m_pMyOnlineWorld->pItemsInfo->GetWearLoc(m_uiPickUpItemID), siEquip, m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), TRUE);

				// actdoll (2005/02/14 16:11) : ����ǳ �Ź� �ӵ��� ���� ������ %������ �����Ͽ� ó��
				// �ð����� �����ۿ� ���� ó��
				SI32	uiCharIndex		= m_pMyOnlineWorld->pIOnlineChar->GetID( m_pMyOnlineWorld->pMyData->GetMyCharUniqueID() );
				if( m_siWearSettingType == ON_WEAR_SHOES_ITEM )		// �Ź� �������� ��츸 üũ
				{
					SI32	siCharKindIndex = m_pMyOnlineWorld->pIOnlineCharKI->GetIndex( m_pMyOnlineWorld->pMyData->GetMyCharKind() );
					if( m_pMyOnlineWorld->pItemsInfo->IsTimeLimitItem( m_uiPickUpItemID ) )
					{
						// robypark 2004/11/4 11:26
						// ������ ������ �ƴ� ��� �̵��ӵ��� �÷��ִ� ������(����ǳ)�� ������ ���� �ʵ��� ����
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
		
			// ������ ���� ���� ���
			m_pMyOnlineWorld->PlayItemEffectSound( pItem->m_siGroupCode,pItem->m_siSound );

			ItemInfo AddItem;
			AddItem.uiItemID = m_uiPickUpItemID;
			AddItem.ItemCount = 1;

			// �ش� �뺴�� ���Ͽ� �������� �����ߴٴ� ��ȣ�� ����
			m_pMyOnlineWorld->pMyData->GetFollowersInterface()->PutOnItem(m_uiSelectedSlot, m_siWearSettingType, m_uiPickUpItemID, m_siPickUpItemDestSlot, NULL);

			// �κ��丮 �۾�
			m_pLCItemPointer->AddItem(AddItem, 0);
			m_pLCItemInventory->DeleteSlot(m_siPickUpItemDestSlot, 1);				
			m_uiPickUpItemID = m_siPickUpItemMapPosX = m_siPickUpItemMapPosY = m_siPickUpItemDestSlot = m_uiPichUpItemQuantity = 0;
			m_pLCItemInventory->StopDrag();

			// actdoll (2004/07/20 14:44) : ���� �Ϸ�� �������� �ð��� �������� ��� �޽����� ����.
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
// Desc: ���� - �������� Ż��
//		pMsg	- OnResStripItemMsg ��ü
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
			
				// �������� �־� �ش�.
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
// Desc: ���� - �������� �̵�
//		pMsg	- OnResMoveItemMsg ��ü
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

				// ������ �������� �����.
				MyItemData.siPosInInventory	=	m_siMoveItemSlotBefore;
				MyItemData.uiID				=	m_uiMoveItemID;
				MyItemData.uiQuantity		=	(UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount;
				
				m_pMyOnlineWorld->pMyData->DelItem( m_uiMoveItemSrcFollower, &MyItemData);
				SetMercenarySlot( m_uiMoveItemSrcFollower );
				FillItemListBox();
				
				// ������ �������� �߰� 
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
// Desc: ���� - �������� ������
//		pMsg	- OnResThrowItemMsg ��ü
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
			// �������� �ߴ� �������� �ٽ� ���´�.
			siThrowItemIndexOfLC		=	GetThrowItemInfo();

			// �� ������ ����Ʈ���� �ش� �������� ���ش�.				
			MyItemData.siPosInInventory =	siThrowItemIndexOfLC;
			MyItemData.uiID				=	m_uiThrowItemID;
			MyItemData.uiQuantity		=	m_uiThrowItemQuantity;

			// ���� ����Ÿ���� �������� �����ش�.
			m_pMyOnlineWorld->pMyData->DelItem(m_uiThrowItemSrcFollower, &MyItemData );
			m_pMyOnlineWorld->pOnlineTradeBook->GetBookMoney(3);
			m_pLCItemInventory->DeleteSlot(siThrowItemIndexOfLC, m_uiThrowItemQuantity);
			SetMercenarySlot( m_uiThrowItemSrcFollower );
			FillItemListBox();
			m_pLCItemInventory->StopDrag();

			// ȿ���� ����
			PushEffect(EFFECT_ON_DROP_ITEM, 0, 0);

			m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
			m_bIsItemMoving = FALSE;			
			m_siPollStatus  =	ON_INVENTORY_STATUS_MAIN_INITPOLL;
		}
		else
		{
			// �� ������ ����Ʈ�� ���� �������� �ٽ� �־��ش�.
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
// Desc: ���� - �������� �ݱ�
//		pMsg	- OnResPickUpItemMsg ��ü
//-----------------------------------------------------------------------------
void OnlineInventory::PickupItem(BYTE *pMsg)
{
	OnResPickUpItemMsg	*pPickUpItemMsg = (OnResPickUpItemMsg*)pMsg;

	m_bIsItemPickingUp = FALSE;

	if( pPickUpItemMsg->dwResponse == ON_RET_OK )
	{
		MyItemData MyItemData;

		// Item �߰�
		MyItemData.siPosInInventory	=	m_siPickUpItemDestSlot;
		MyItemData.uiID				=	m_uiPickUpItemID;
		MyItemData.uiQuantity		=	m_uiPichUpItemQuantity;
		
		m_pMyOnlineWorld->pMyData->AddItem(m_uiPickUpItemDestFollower, &MyItemData);					
		m_pMyOnlineWorld->pOnlineTradeBook->GetBookMoney(3);

		SetFieldItemDrag(0, 0, 0);
		
		m_pMyOnlineWorld->pMyData->SetWaitPickUpItemResponse(FALSE);
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		
		PushEffect(EFFECT_ON_GET_ITEM, 0, 0);
		
		// ����
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
// Desc: ���� - �������� �Һ�
//		pMsg	- OnResSpendItemMsg ��ü
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
		
		// Ư�� ������
		if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
		{
			switch(pItem->m_siSpecial)
			{
			case ON_ITEM_SPECIAL_INITIALIZE:
				// �һ����� ����
				m_pMyOnlineWorld->pMyData->ChangeFollowerParameterToBonus(m_uiUseItemDestFollower, m_pMyOnlineWorld->pOnlineCharParser, 
					m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetFollowerParameterKind(m_uiUseItemDestFollower), m_pMyOnlineWorld->pItemsInfo);
				break;
			case ON_ITEM_SPECIAL_SHOWMERCENARY:
				// �뺴 ���̱�
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
			case ON_ITEM_SPECIAL_DONKEY://�糪��.
				break;
			case ON_ITEM_SPECIAL_IMPERIUM://������ �ߵ�.
				break;
			case ON_ITEM_SPECIAL_CHANGESEX:			break;
			case ON_ITEM_SPECIAL_MAGICSTONERED:		break;		// ����, ����, �μ��� �޼��� ����ֱ�(pItem->m_siSpecialParam����)
			case ON_ITEM_SPECIAL_MAGICSTONEBLUE:	break;
			case ON_ITEM_SPECIAL_MAGICSTONEBLACK:	break;		// �����÷��̾����� �����ֱ�->ON_CHANGE_CHARACTERCODE ����, ���������� �ְ�޴� ��Ŷ ����
			default:
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INVENTORY_UNKNOWN_SPECIAL_ITEM));
				break;
			}
		}
		// ��ü�� ����
		else if (pItem->m_siGroupCode & ON_ITEM_TOALL)
		{
			// �ǵ� parameter�� ��Ȱ�������� ����
			if( pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER )
				m_pMyOnlineWorld->pMyData->UseItemToAllPer( pItem->m_siHpGain, pItem->m_siMpGain, pItem->m_siHealth );
			else
				m_pMyOnlineWorld->pMyData->UseItemToAll( pItem->m_siHpGain, pItem->m_siMpGain, (pItem->m_siGroupCode & ON_ITEM_REVIVE) ? TRUE : FALSE );
		}
		// �Ϻο� ����
		else
		{
			m_pMyOnlineWorld->pMyData->UseItem( m_uiUseItemDestFollower, pItem->m_siHpGain, pItem->m_siMpGain, pItem->m_siHealth );
		}
		m_pMyOnlineWorld->pMyData->DelItem( m_uiUseItemDestFollower, &MyItemData );
		
		// ���� �Ǵ� ��Դ� �Ҹ��� ����.
		if (pItem->m_siSound != 0) //m_pMyOnlineWorld->PlayItemEffectSound( pItem->m_siGroupCode,pItem->m_siSound );
			m_pMyOnlineWorld->PlayFoodEatEffectSound(pItem->m_siSound);
		FillItemListBox();
	}
	else
	{
		CItemHeader	*pItem = NULL;
		pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiUseItemID);
		
		if(!pItem)		return;
		
		// Ư�� ������
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
// Desc: ���� - �뺴�� ����
//		pMsg	- OnAddFollower ��ü
//-----------------------------------------------------------------------------
void OnlineInventory::AddFollower(BYTE *pMsg)
{
	OnAddFollower*  pOnAddFollower = (OnAddFollower*)pMsg ;

	OnFollowerInfo* pOnFollowerInfo = (OnFollowerInfo*)&pOnAddFollower->FollowerInfo;

	m_pMyOnlineWorld->pMyData->InitFollower(pOnFollowerInfo->uiSlot, pOnFollowerInfo);
}


//===================================================================================================
//	�����κ��� ���� �޽����� ���� ó�� �κ� END
//===================================================================================================
