/********************************************************************************************************
	파일명	:	OnlineTrade.cpp
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2002.01.04
	수정일	:	2003.08.12
	내  용  :	유저간에 거래

	장부 거래 소스 제거함. -진욱-
********************************************************************************************************/
#include <GSL.h>


#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineBaseLB.h"
#include "OnlineTrade.h"
#include "OnlineNumberControl.h"
#include "OnlineTradeBook.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineETC.h"
#include "OnlineVillageSave.h"
#include "OnlineFont.h"
#include "OnlineClient.h"
#include "OnlineItem.h"
#include "OnlineResource.h"
#include "OnlineFieldChat.h"
#include "OnlineMiniMap.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "OnlineCharUI.h"
#include "OnlineProductStructure.h"
#include "OnlineVillage-Parser.h"
#include "OnlineVillageManager.h"
#include "OnlineFieldAuction.h"
#include "OnlineBooth.h"
#include "OnlineSoldierTrade.h"
#include "OnlineMercenary.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"

extern _InputDevice				IpD;


enum
{   
	ON_TRADE_INIT		= 0,
	ON_TRADE_POLL,
	ON_TRADE_MONEY,
	ON_TRADE_ITEM,
	ON_TRADE_WAIT,
	ON_TRADE_ADD_ITEM,
	ON_TRADE_ADD_RESPONSE_WAIT,
	ON_TRADE_DEL_MYLIST,
	ON_TRADE_DELAY,
	ON_TRADE_SELECT_FOLLOWER,
	ON_TRADE_INFO_WAIT
};


#define ON_TRADE_PROPERTY_ITEMID		609		// 장부의 아이템 ID

OnlineTrade::OnlineTrade()
{
	m_pMyOnlineWorld	=	NULL;
	m_pMyList			=	NULL;
	m_pYouList			=	NULL;
	m_pMsgBox			=	NULL;
	m_pYetPos			=	NULL;	
	m_siStatus			=	0;	
	m_uiCharID			=	0;
	m_siCount			=	0;
	m_dwDelayTime		=	0;
	m_siDelay			=	0;
	m_siMsgStatus		=	0;
	m_siNowSendMsgType	=	0;
	m_dwMsgTime			=	0;
	m_siRecvTradeNum	=	0;
	m_siFlag			=	0;
	m_siAddType			=	-1;
	m_bActive			=	FALSE;
	m_bMouseDown		=	FALSE;	
	m_bYouOk1			=	FALSE;
	m_bYouOk2			=	FALSE;
	m_bMyOk1			=	FALSE;
	m_bWait				=	FALSE;		
	m_bIsFirst			=	FALSE;
	m_bSendMsg			=	FALSE;
	m_bRecvTrade		=	FALSE;
	m_bSuccess			=	FALSE;
	m_bProperty			=	FALSE;
	m_bPermitTrade		=	TRUE;
	m_bFirstChangeMoney	=	TRUE;
	m_bTradeStop = FALSE;
	ZeroMemory(m_FollowerItemData, sizeof(MyItemData)*ON_MAX_FOLLOWER_NUM*ON_MYDATA_MAX_ITEM_NUM);

	m_pSoldier = NULL;
}


OnlineTrade::~OnlineTrade()
{
	Free();
}

VOID	OnlineTrade::Free()
{
	clGrp.FreeXspr(m_MainImage);
	clGrp.FreeXspr(m_AcceptImage);
	clGrp.FreeXspr(m_DecideImage);
	clGrp.FreeXspr(m_MoneyImage);
	clGrp.FreeXspr(m_FollowerSelImage);
	clGrp.FreeXspr(m_But1Image);
	clGrp.FreeXspr(m_But2Image);

	if( m_pSoldier)
	{
		delete m_pSoldier;
		m_pSoldier = NULL;
	}

	if( m_pMyList )
	{
		delete m_pMyList;		
		m_pMyList	=	NULL;
	}

	if( m_pYouList )
	{
		delete m_pYouList;
		m_pYouList = NULL;
	}

	if( m_pMsgBox )
	{
		delete m_pMsgBox;
		m_pMsgBox = NULL;
	}

	if( m_pTradeItem )
	{
		delete []m_pTradeItem;
		m_pTradeItem = NULL;
	}

	if( m_pYetPos )
	{
		delete []m_pYetPos;
		m_pYetPos = NULL;
	}
}

VOID	OnlineTrade::Initialize()
{
	m_siStatus			=	0;	
	m_uiCharID			=	0;
	m_siCount			=	0;
	m_dwDelayTime		=	0;
	m_siDelay			=	0;
	m_siMsgStatus		=	0;
	m_siNowSendMsgType	=	0;
	m_dwMsgTime			=	0;
	m_siRecvTradeNum	=	0;
	m_siFlag			=	0;
	m_siAddType			=	-1;
	m_bActive			=	FALSE;
	m_bMouseDown		=	FALSE;	
	m_bYouOk1			=	FALSE;
	m_bYouOk2			=	FALSE;
	m_bMyOk1			=	FALSE;
	m_bWait				=	FALSE;	
	m_bIsFirst			=	FALSE;
	m_bSendMsg			=	FALSE;
	m_bRecvTrade		=	FALSE;
	m_bSuccess			=	FALSE;
	m_bProperty			=	FALSE;
	m_bPermitTrade		=	TRUE;
	m_bFirstChangeMoney	=	TRUE;
	m_bTradeStop = FALSE;
	m_pMsgBox->SetAction(FALSE);
	ZeroMemory(&FriendInfo, sizeof(FriendInfo));
	ZeroMemory(m_FollowerItemData, sizeof(MyItemData)*ON_MAX_FOLLOWER_NUM*ON_MYDATA_MAX_ITEM_NUM);
}

VOID	OnlineTrade::Init( cltOnlineWorld *pOnlineWorld )
{
	m_pMyOnlineWorld	=	pOnlineWorld;
	m_siNoField			=	-1;

	clGrp.LoadXspr("Online\\GameSpr\\Trade\\Trade.Spr", m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Trade\\trade_decide_button.Spr", m_DecideImage);
	clGrp.LoadXspr("Online\\GameSpr\\Trade\\trade_accept_button.Spr", m_AcceptImage);
	clGrp.LoadXspr("Online\\GameSpr\\Trade\\trade_money_button.Spr", m_MoneyImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_hlight.Spr", m_FollowerSelImage);

	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr", m_But1Image);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button2.Spr", m_But2Image);

	m_siX = 95;
	m_siY = 45;

	// 버튼을 생성	
	m_BMyMoney.Create(m_siX + 154, m_siY + 204, m_MoneyImage.Header.Xsize, m_MoneyImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_BMyOk1.Create(m_siX + 291, m_siY + 270, m_DecideImage.Header.Xsize, m_DecideImage.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(1000115), BUTTON_PUT_LEFT, TRUE);
	m_BMyOk2.Create(m_siX + 291, m_siY + 318, m_AcceptImage.Header.Xsize, m_AcceptImage.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000550), BUTTON_PUT_LEFT, TRUE);

	m_BFollower.Create(m_siX + 62, m_siY + 368, m_But2Image.Header.Xsize, m_But2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_SOILDER_CHANGESOILDER), BUTTON_PUT_LEFT, TRUE);
	m_BProperty.Create(m_siX + 146, m_siY + 368, m_But2Image.Header.Xsize, m_But2Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_PROPERTY_CHANGEPROPERTY), BUTTON_PUT_LEFT, FALSE);
	m_BExit.Create(m_siX + 139, m_siY + 394, m_But1Image.Header.Xsize, m_But1Image.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);

	m_pMyList = new OnlineListControl(pOnlineWorld);
	m_pMyList->Init(60, 60, 4, 2, m_siX + 18, m_siY + 233, 0);

	m_pYouList = new OnlineListControl(pOnlineWorld);
	m_pYouList->Init(60, 60, 4, 2, m_siX + 18, m_siY + 64, 0);

	m_pMsgBox = new OnlineMsgBox;
	m_pMsgBox->Init(m_pMyOnlineWorld);

	m_pTradeItem = new TradeItem[4 * 2];
	ZeroMemory(m_pTradeItem, sizeof( TradeItem ) * (4 * 2));

	m_pYetPos = new SI16[4 * 2];
	ZeroMemory(m_pYetPos, sizeof( SI16 ) * (4 * 2));

	// 나의 아이피를 구한다.
	char	SendString[5];
	ZeroMemory(SendString, sizeof(SendString));

	BOOL	bRet = FALSE;

	for(SI16 i=0; i<ON_MAX_FOLLOWER_NUM; i++)	m_bFollowerSlot[i] = FALSE;

	ZeroMemory(&FriendInfo, sizeof(FriendInfo));

	// 거래시 용병 정보 표시
	m_pSoldier = new OnlineSoldierTrade(pOnlineWorld, "Online\\GameSpr\\Inventory\\main.Spr");

	// 캐릭터 이름 영역
	SetRect(&m_rtCharName, m_siX + 18, m_siY + 36, m_siX + 142, m_siY + 54);
}


// 활성화 되어 있는가?
BOOL	OnlineTrade::IsActive()
{
	return m_bActive;
}

// 활성화 시키거나 죽인다.
VOID	OnlineTrade::SetActive( BOOL bFlag )
{
	if( m_bRequest == TRUE )
	{
		m_pMsgBox->Initialize();
		m_bWait = FALSE;
		m_pSoldier->SetHide();
	}

	if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn())	return;
	if(m_bWait)				return;
	if(m_bRecvTrade)		return;

	m_bActive =	bFlag;		

	if( bFlag )
	{		
		m_siStatus			=	ON_TRADE_INIT;

		// 장부가 열려 이뜸 다더 준다.
		if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

		m_bRecvTrade			=	FALSE;
		m_bRecvTrade			=	FALSE;
		m_bSuccess				=	FALSE;
		m_siFlag				=	0;
		m_bSendMsg				=	FALSE;
		m_siNoField				=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + m_MainImage.Header.Xsize, m_siY + m_MainImage.Header.Ysize );

		ZeroMemory( m_szSendMoney, sizeof( m_szSendMoney ) );
		ZeroMemory( m_szTempSendMoney, sizeof( m_szTempSendMoney ) );
		ZeroMemory( m_szResMoney, sizeof( m_szResMoney ) );
		ZeroMemory( m_pTradeItem, sizeof( TradeItem ) * ( 4 * 2 ) );

		m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);
		m_pMyOnlineWorld->pOnlinePannel->SetChatOpen(2);
		m_BMyOk1.SetAction(TRUE);		
	}
	else 
	{
		// 인벤토리에 원래 아이템을 돌려준다.
		for(SI16 i=0 ; i<m_siCount; i++)
		{
			if(m_pTradeItem[i].uiItemCode == ON_TRADE_PROPERTY_ITEMID)	continue;

			MyItemData MyItemData;
			MyItemData.siPosInInventory = m_pYetPos[i];
			MyItemData.uiID				= m_pTradeItem[i].uiItemCode;
			MyItemData.uiQuantity		= m_pTradeItem[i].siQuantity;				

			m_pMyOnlineWorld->pMyData->AddItem((UI08)m_pTradeItem[i].uiFollower, &MyItemData);
			m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( m_pTradeItem[i].uiFollower );		//.actdoll (021224)

			m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot((UI08)m_pTradeItem[i].uiFollower);
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
		}

		m_siMouseStatus	= m_uiCharID = m_siMsgStatus = m_siNowSendMsgType = m_siFlag = 0;
		m_bIsFirst = m_bRecvTrade = m_bWaitOkMsg = m_bSendMsg = m_bMyOk2 = m_bRecvTrade = m_bSuccess =	m_bProperty = FALSE;

		m_pMyList->Clear();
		m_pYouList->Clear();
		ZeroMemory( m_pTradeItem, sizeof( TradeItem ) * (4 * 2));

		m_pMyOnlineWorld->m_pMercenary->NoAction( FALSE );
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);		

		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		m_pMsgBox->Initialize();		

		for(i=0; i<ON_MAX_FOLLOWER_NUM; i++)		m_bFollowerSlot[i] = FALSE;

		m_bFirstChangeMoney = TRUE;
		m_bTradeStop = FALSE;
		ZeroMemory(&FriendInfo, sizeof(FriendInfo));
		ZeroMemory(m_FollowerItemData, sizeof(MyItemData)*ON_MAX_FOLLOWER_NUM*ON_MYDATA_MAX_ITEM_NUM);

		m_pMyOnlineWorld->m_pMercenary->SetHighLight( FALSE );		
	}	

	// 인벤토리 창의 상태를 설정한다.
	// 거래창이 열리시에는 반드시 인벤토리 창도 열려야 하며,
	// 거래창이 닫힐시에도 반드시 인벤토리 창도 닫아주어야 한다.	
	m_pMyOnlineWorld->pOnlineInventory->SetActive( m_bActive );

}

// 액숀
VOID	OnlineTrade::Action()
{
	SI16 siRet	= m_pMsgBox->Process();

	if(m_pMyOnlineWorld->pOnlineFieldAuction->IsEntered())	return;

	if( m_bActive == FALSE )
	{		
		m_bWait		= m_pMsgBox->IsActive() == TRUE ? m_bWait = TRUE : m_bWait = FALSE;

		if( siRet ==  ON_MSGBOX_RETURN_OK  )	
		{
			m_bWait = FALSE;
			SetActive( TRUE );
			m_pMyOnlineWorld->pOnlineClient->SendRequestTrade(TRUE);
		}
		else if( siRet ==  ON_MSGBOX_RETURN_CANCEL )
		{
			m_pMyOnlineWorld->pOnlineClient->SendRequestTrade(FALSE);
			m_bWait		= FALSE;
			m_bActive	= FALSE;
			m_uiCharID	= 0;
		}

		return;
	}

	if(ON_TRADE_INFO_WAIT == m_siStatus)
		m_pSoldier->Actoin();
	

	POINT		pt;
	CItemHeader	*pItem;
	SI16		siIndex;


	pt.x	= IpD.Mouse_X;
	pt.y	= IpD.Mouse_Y;

	siIndex	= m_pMyList->GetSelectSlot();
	if( siIndex != -1 )
	{
		pItem	= 	m_pMyList->GetSlotInfo(siIndex) != NULL ? m_pMyOnlineWorld->pItemsInfo->GetItem( m_pMyList->GetSlotInfo(siIndex)->uiItemID ) : NULL;		
		if( pItem )	
		{
			ItemInfo *pSlot = const_cast<ItemInfo *>(m_pMyList->GetSlotInfo(siIndex));

			if(pSlot->uiItemID == ON_TRADE_PROPERTY_ITEMID)
			{
				if(pSlot->Property.uiVillageCode > 0 && !m_pSoldier->IsAction())
					m_pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo *>(m_pMyList->GetSlotInfo(siIndex)), (SI16)pt.x, (SI16)pt.y, ON_HELP_DOWN, FALSE, FALSE);
			}
			else
				if(!m_pSoldier->IsAction())
					m_pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo *>(m_pMyList->GetSlotInfo(siIndex)), (SI16)pt.x, (SI16)pt.y, ON_HELP_DOWN, FALSE, FALSE);
		}
	}
	else
	{
		siIndex	= m_pYouList->GetSelectSlot();
		if( siIndex != -1 )
		{
			pItem	=	m_pYouList->GetSlotInfo(siIndex) != NULL ? m_pMyOnlineWorld->pItemsInfo->GetItem( m_pYouList->GetSlotInfo(siIndex)->uiItemID ) : NULL;
			if( pItem )
			{
				if(!m_pSoldier->IsAction())
					m_pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo *>(m_pYouList->GetSlotInfo(siIndex)), (SI16)pt.x, (SI16)pt.y, ON_HELP_DOWN, FALSE, FALSE);

				if(m_pYouList->GetSlotInfo(siIndex)->siType == LIST_FOLLOWER)
				{
					if( m_bMouseDown )
					{
						m_pSoldier->SetSoldier(const_cast<ItemInfo *>(m_pYouList->GetSlotInfo(siIndex))->Follower, m_FollowerItemData[siIndex]);
						m_siStatus = ON_TRADE_INFO_WAIT;
					}
				}
			}
		}
	}

	if(PtInRect(&m_rtCharName, pt) == TRUE)
	{
		if(FriendInfo.szCharName[0])
		{
			m_pMyOnlineWorld->pOnlineHelp->SetCharInfo(pt.x, pt.y, FriendInfo.szCharName, 
				FriendInfo.szGuildID, FriendInfo.siGuildClass, FriendInfo.siLevel);
		}
	}

	m_pMyOnlineWorld->pOnlinePannel->SetChatOpen(2);
	
	switch( m_siStatus )
	{ 
	case ON_TRADE_INIT:		
		m_siStatus		=	ON_TRADE_POLL;
		m_bYouOk1		=	FALSE;
		m_bMyOk1		=	FALSE;
		m_bMyOk2		=	FALSE;
		m_bYouOk2		=	FALSE;
		m_siMouseStatus	=	0;
		m_siScrollX		=	(SI16)GetTickCount();
		m_BMyMoney.SetAction(TRUE);
		m_BMyOk2.SetAction( FALSE );		
		m_BFollower.SetAction(TRUE);

		m_pSoldier->SetHide();

		// 부동산을 소지하고 있다면 버튼활성화
		if(m_pMyOnlineWorld->pMyData->uiPropertyVillageCode > 0)
			m_BProperty.SetAction(TRUE);
		else
			m_BProperty.SetAction(FALSE);
				
		break;
	case ON_TRADE_INFO_WAIT:
		{
			int a = 0;
		}
		break;

	case ON_TRADE_POLL:
		if(!m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
		{
			if( m_BExit.Process( m_bMouseDown ) == TRUE )													// 나가기
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequestCancelTrade();			
				m_bRecvTrade = FALSE;
				SetActive( FALSE );
				//if(m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
				break;
			}
			else if( m_BMyOk1.Process( m_bMouseDown ) == TRUE && !m_bMyOk1)												// 거래
			{	
				if(m_bTradeStop)	
				{
					m_BMyOk1.SetAction(FALSE);
					break;
				}

				// 서버에 거래 버튼 알린다.			
				m_bMyOk1		= !m_bMyOk1;
				if(m_bMyOk1)
				{
					for(int i =0; i < 8; i++)
					{
						if(m_pMyList->GetSlotInfo(i))
						{
							if(m_pMyList->GetSlotInfo(i)->uiItemID != 0)
								m_pMyList->SetColor(i, LIST_COLOR_RED);
						}
					}
					m_BMyMoney.SetAction(FALSE);
				}
				else
				{
					for(int i =0; i < 8; i++)
					{
						if(m_pMyList->GetSlotInfo(i))
						{
							if(m_pMyList->GetSlotInfo(i)->ItemCount)
								m_pMyList->SetColor(i, LIST_COLOR_NOT);
						}
					}
				}

				m_pMyOnlineWorld->pOnlineClient->SendRequestConfirmTrade( m_bMyOk1 );
				m_BFollower.SetAction(FALSE);
				m_BProperty.SetAction(FALSE);
				break;
			}
			else if(m_BFollower.Process(m_bMouseDown))
			{
				if(m_bTradeStop)
				{
					m_BFollower.SetAction(FALSE);
					break;
				}

				m_BFollower.SetAction(FALSE);
				if(!m_pMyOnlineWorld->m_pMercenary->IsAction())
					m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
				m_pMyOnlineWorld->m_pMercenary->NoAction(TRUE);
				m_pMyOnlineWorld->m_pMercenary->SetHighLight(TRUE);
				m_siStatus = ON_TRADE_SELECT_FOLLOWER;
				break;
			}
			else if(m_BProperty.Process(m_bMouseDown))
			{
				if(m_bTradeStop)
				{
					m_BProperty.SetAction(FALSE);
					break;
				}

				if(!m_bProperty)
				{
					m_pTradeItem[ m_siCount ].uiFollower  = 0;
					m_pTradeItem[ m_siCount ].siQuantity  = 1;
					m_pTradeItem[ m_siCount ].siTradePos  = m_pMyList->GetEmptySlot();
					m_pTradeItem[ m_siCount ].uiItemCode  = ON_TRADE_PROPERTY_ITEMID;
					m_pYetPos[ m_siCount ]				  = 0;
					m_siCount++;

					// 부동산을 거래 리스트에 올린다.
					m_siAddType	= LIST_PROPERTY;
					m_bProperty = TRUE;
					m_siStatus	= ON_TRADE_ADD_RESPONSE_WAIT;
					m_pMyOnlineWorld->pOnlineClient->SendRequestPreTrade(m_pMyList->GetEmptySlot(), 0, 0, 0, 0, TRUE);
					m_BProperty.SetAction(FALSE);
				}
				else
				{
					// 부동산을 거래 리스트에서 삭제한다.
					m_bProperty = FALSE;
				}
			}
			else if(m_BMyMoney.Process(m_bMouseDown))
			{
				if(!m_bMyOk1)
				{
					m_siStatus	=	ON_TRADE_MONEY;

					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( m_pMyOnlineWorld->pMyData->GetMoney() );
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );
					break;
				}
			}

			if(m_bMouseDown && !m_bMyOk1)
			{
				if( m_siMouseStatus == 0 )
				{
					if((m_siYetPos = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot()) != -1)		
					{
						m_siMouseStatus = 2;
						m_siStatus = ON_TRADE_DELAY;
						m_siDelay = 0;
					}

					m_dwDelayTime = timeGetTime();
				}

				else if(m_siMouseStatus == 1 || m_siMouseStatus == 2)
				{
					DWORD	dwTemp = timeGetTime();

					if( ( dwTemp - m_dwDelayTime ) >= 250 )
					{
						SI16 siNewPos;						

						// 거래창으로 이동 
						if((siNewPos = m_pMyList->GetSelectSlot()) != -1)
						{
							if(m_siMouseStatus == 1)
							{
								m_pMyList->StopDrag();
								m_siMouseStatus = 0;
								m_siStatus		= ON_TRADE_DELAY;
								m_siDelay		= 0;
							}
							else
							{
								const ItemInfo	*pItemSlot = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siYetPos);
								m_uiCurFollower = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
								m_siDragItem	= pItemSlot->uiItemID;

								// 시작 ///////////////////////////////////
								// robypark 2004/12/7 18:47
								// 소집권 아이템 거래는 같은 상단원 사이만 가능하다.
								// 거래하려는 아이템 정보 헤더 얻기
								CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(m_siDragItem);

								if (pItemHeader)
								{
									// 소집권 아이템인지 검사
									if (NULL != pItemHeader->m_pImperium)
									{
										/*
										// 공성 소집권 아이템을 상단원에게 나누어 줄 수 있는 시간인가?
										if(m_pMyOnlineWorld->IsInTimeGiveArmyItem())
										{
											// 소집권 아이템 거래는 같은 상단원 사이만 가능하다.
											if (!m_pMyOnlineWorld->IsMemberOfSameGuild(FriendInfo.szGuildID, FriendInfo.siGuildClass))
											{
												m_pMyList->StopDrag();
												m_siMouseStatus = 0;
												m_siStatus		= ON_TRADE_DELAY;
												m_siDelay		= 0;
												m_pMyOnlineWorld->pOnlineInventory->DragFail();
												break;
											}
										}
										else	// 시간이 아닌 경우
										*/
										{
											m_pMyList->StopDrag();
											m_siMouseStatus = 0;
											m_siStatus		= ON_TRADE_DELAY;
											m_siDelay		= 0;
											m_pMyOnlineWorld->pOnlineInventory->DragFail();
											break;
										}
									}
								}
								// 끝 ///////////////////////////////////

								m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( pItemSlot->ItemCount );
								m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
								m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );
								
								m_siMouseStatus = 0;
								m_siStatus = ON_TRADE_ADD_ITEM;
							}
						}
						// 내 인벤토리로 이동
						else if((siNewPos = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSelectSlot()) != -1)
						{
							if(m_siMouseStatus == 1)
							{
								m_siMouseStatus = 0;
								m_siStatus		= ON_TRADE_DELAY;
								m_siDelay		= 0;
								m_pMyList->StopDrag();
							}
							else
							{
								m_siMouseStatus = 0;
								m_siStatus		= ON_TRADE_DELAY;
								m_siDelay		= 0;
								m_pMyOnlineWorld->pOnlineInventory->DragFail();
							}
						}
						else
						{
							if(m_siMouseStatus == 1)
							{
								m_siMouseStatus = 0;
								m_siStatus		= ON_TRADE_DELAY;
								m_siDelay		= 0;
								m_pMyList->StopDrag();
							}
							else
							{
								m_siMouseStatus = 0;
								m_siStatus		= ON_TRADE_DELAY;
								m_siDelay		= 0;
								m_pMyOnlineWorld->pOnlineInventory->DragFail();
							}
						}
					}
				}
			}
		}

		if(m_bMyOk1 && m_bYouOk1 && !m_bTradeStop)	m_BMyOk2.SetAction(TRUE);
		else										m_BMyOk2.SetAction(FALSE);

		if(m_BMyOk2.Process(m_bMouseDown) && !m_bMyOk2)
		{
			if(m_bTradeStop)
			{
				m_BMyOk2.SetAction(FALSE);
				break;
			}
			
			// 미리 한번 너어 본다.
			SI16	i;

			for( i = 0 ; i < m_pMyList->GetItemNum(); i++ )
				m_pTradeItem[i].siTradePos	=	m_pYetPos[i];

			// 서버로 보낸다.						
			m_pMyOnlineWorld->pOnlineClient->SendRequestTradeSuccess(!m_bMyOk2);
		}
		break;

	case ON_TRADE_DELAY:
		m_siDelay++;
		if( m_siDelay >= 5 )	
		{
			m_siStatus	=	ON_TRADE_POLL;
			m_siDelay	=	0;
		}
		break;

	case ON_TRADE_ADD_ITEM:
		{
			if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess())
			{
				if( m_pMyOnlineWorld->pOnlineNumPad->siCount == 0 )
				{
					m_pMyOnlineWorld->pOnlineInventory->DragFail();
					m_siStatus = ON_TRADE_POLL;
					break;
				}

				if( m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() != m_uiCurFollower)
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKET_FOLLOWER_NO_CHANGE));
					m_pMyOnlineWorld->pOnlineInventory->DragFail();
					m_siStatus = ON_TRADE_POLL;
					break;
				}
				
				m_siNewPos = m_pMyList->GetEmptySlot();
				
				// 삽입이 가능하다면 서버로 알린다.
				if( m_siNewPos != -1 )
				{
					m_siAddType = LIST_ITEM;
					m_siItemCount = (UI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;

					m_pMyOnlineWorld->pOnlineClient->SendRequestPreTrade(m_siNewPos, m_siDragItem, m_siItemCount, 0, m_uiCurFollower);
					m_pMyOnlineWorld->pOnlineInventory->DragFail();
					m_siStatus = ON_TRADE_ADD_RESPONSE_WAIT;
				}
				else
				{
					m_pMyOnlineWorld->pOnlineInventory->DragFail();
					m_siStatus = ON_TRADE_POLL;
				}
			}
		}
		break;
	case ON_TRADE_ADD_RESPONSE_WAIT:
		{
		}
		break;
	case ON_TRADE_DEL_MYLIST:
		if( m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )
		{
			m_siStatus	=	ON_TRADE_POLL;

			if(m_pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				UI32	uiItemID = m_pMyList->GetSlotInfo(m_pMyList->GetDragSlot())->uiItemID;

				// 내 거래 창에 있는 아이템을 지워 준다.
				m_pMyList->DeleteSlot((SI16)m_pMyList->GetDragSlot(), (UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount);

				// 서버로 보내줄 데이타를 지워 준다.
				for(SI16 i=0 ; i<m_siCount; i++)
				{
					if( m_pTradeItem[i].uiItemCode == uiItemID )
					{
						m_pTradeItem[i].siQuantity -=  (SI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;

						// 만약 0이면 아이템이 없다 ...
						if( m_pTradeItem[i].siQuantity == 0 )
						{
							m_pTradeItem[i].uiFollower = 0;
							m_pTradeItem[i].siQuantity = 0;
							m_pTradeItem[i].uiItemCode = 0;
							m_pTradeItem[i].siTradePos = 0;
							m_siCount--;
						}
					}
				}

				// 인벤토리를 갱신 한다.
				SI16			siPos;
				MyItemData		*lpItemData;

				siPos		=	m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(uiItemID);
				lpItemData	=	m_pMyOnlineWorld->pMyData->GetItem( 0, uiItemID, siPos);

				if( lpItemData ) lpItemData->siPosInInventory	=	siPos;
				m_pMyList->StopDrag();
			}
		}
		break;

	case ON_TRADE_MONEY:
		if( m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )						// 돈 입력이 끝이나면
		{
			if(m_pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				m_siStatus	=	ON_TRADE_POLL;			
				m_pMyOnlineWorld->pOnlineClient->SendRequestPreTrade( 0, 0, 0, m_pMyOnlineWorld->pOnlineNumPad->siCount, 0 );
				_ui64toa( m_pMyOnlineWorld->pOnlineNumPad->siCount, m_szTempSendMoney, 10 );
				m_siStatus = ON_TRADE_ADD_RESPONSE_WAIT;
			}
			else
				m_siStatus	=	ON_TRADE_POLL;
		}
		break;
	case ON_TRADE_ITEM:
		break;
	case ON_TRADE_SELECT_FOLLOWER:
		{
			m_pMyOnlineWorld->pOnlineHelp->SetText(m_pMyOnlineWorld->pOnlineText->Get(8000551), pt.x, pt.y+70, ON_HELP_UP, RGB(250, 250, 250), TRUE);
			if(!IpD.LeftPressSwitch)		break;

			SI16 siFollowerSlot = (SI16)m_pMyOnlineWorld->m_pMercenary->GetMouseOverSlotNo(TRUE);

			// robypark 2005/1/8 16:31
			// 소집권을 가지고 있는 용병은 거래할 수 없다.
			if (m_pMyOnlineWorld->IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(siFollowerSlot))
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100114));				
				break;
			}

			if(siFollowerSlot != -1)
			{
				// 용병을 선택했으므로 하이라이트를 해제한다.
				m_pMyOnlineWorld->m_pMercenary->SetHighLight(FALSE);

				IOnlineFollower *pFollower = m_pMyOnlineWorld->pMyData->GetFollowersInterface();
				m_AddFollower.ItemCount = 1;
				m_AddFollower.uiItemID = 1;
				m_AddFollower.siType = LIST_FOLLOWER;
				m_siAddType			 = LIST_FOLLOWER;
				m_AddFollower.Follower.uiKind = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind(siFollowerSlot);
				m_AddFollower.Follower.siLevel = m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(siFollowerSlot);
				m_AddFollower.Follower.siDex = pFollower->GetFollowerBaseParameterDEX(siFollowerSlot);
				m_AddFollower.Follower.siInt = pFollower->GetFollowerBaseParameterINT(siFollowerSlot);
				m_AddFollower.Follower.siStr = pFollower->GetFollowerBaseParameterSTR(siFollowerSlot);
				m_AddFollower.Follower.siVit = pFollower->GetFollowerBaseParameterVIT(siFollowerSlot);
				m_AddFollower.Follower.uiSlot = siFollowerSlot;
				
				// 서버로 보내 준다.
				m_pMyOnlineWorld->pOnlineClient->SendRequestPreTrade(m_pMyList->GetEmptySlot(), 0, 0, 0, siFollowerSlot);
				m_bFollowerSlot[siFollowerSlot] = TRUE;
				m_BFollower.SetAction(TRUE);
				m_pMyOnlineWorld->m_pMercenary->NoAction(FALSE);

				m_siStatus	= ON_TRADE_ADD_RESPONSE_WAIT;
			}
		}
		break;
	}

	if( IpD.LeftPressSwitch )			m_bMouseDown	=	TRUE;
	else								m_bMouseDown	=	FALSE;
}


// 드로우
VOID	OnlineTrade::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	if(!m_bActive)	
	{
		m_pMsgBox->Draw(pSurface);
		return;
	}	

	HDC		hDC;

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT(m_siX, m_siY, m_MainImage.Header.Xsize, m_MainImage.Header.Ysize, m_MainImage.Image);

		// 상대방
		clGrp.PutSpriteT(m_siX + 154, m_siY + 35, m_MoneyImage.Header.Xsize, m_MoneyImage.Header.Ysize, m_MoneyImage.Image);
		if(m_bYouOk1)
			clGrp.PutSpriteT(m_siX + 291, m_siY + 101, m_DecideImage.Header.Xsize, m_DecideImage.Header.Ysize, 
				&m_DecideImage.Image[m_DecideImage.Header.Start[2]]);
		else
			clGrp.PutSpriteT(m_siX + 291, m_siY + 101, m_DecideImage.Header.Xsize, m_DecideImage.Header.Ysize, 
				&m_DecideImage.Image[m_DecideImage.Header.Start[0]]);

		if(m_bYouOk2)
			clGrp.PutSpriteT(m_siX + 291, m_siY + 149, m_AcceptImage.Header.Xsize, m_AcceptImage.Header.Ysize, 
			&m_AcceptImage.Image[m_AcceptImage.Header.Start[2]]);
		else
			clGrp.PutSpriteT(m_siX + 291, m_siY + 149, m_AcceptImage.Header.Xsize, m_AcceptImage.Header.Ysize, 
			&m_AcceptImage.Image[m_AcceptImage.Header.Start[0]]);

		// 본인
		m_BMyMoney.Put(&m_MoneyImage, 0, 0, 1, BUTTON_PUT_NOMOVE);
		if(m_bMyOk1)		m_BMyOk1.Put(&m_DecideImage, 2, 2, 2, BUTTON_PUT_NOMOVE);
		else				m_BMyOk1.Put(&m_DecideImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		if(m_bMyOk2)		m_BMyOk2.Put(&m_AcceptImage, 2, 2, 2, BUTTON_PUT_NOMOVE);
		else				m_BMyOk2.Put(&m_AcceptImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

		m_BFollower.Put(&m_But2Image, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_BProperty.Put(&m_But2Image, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_BExit.Put(&m_But1Image, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		CHAR	Temp[1024];

		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 114, m_siY + 7, 118, 13, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE), RGB( 250, 250, 250 ));

		if( MakeMoneyText( m_szResMoney, Temp ) )
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_siX + 294, m_siY + 38, Temp, RGB(250, 250, 250));		// 상대방이 올려 놓은 돈
		

		if( MakeMoneyText( m_szSendMoney, Temp ) )			
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_siX + 294, m_siY + 207, Temp, RGB(250, 250, 250));		// 내가 올려 놓은 돈			


		// 상대 방의 아이디를 찍어 준다.
		if( FriendInfo.szCharName[0] )
		{
			sprintf(Temp, "%s LV.%d", FriendInfo.szCharName, FriendInfo.siLevel);
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 18, m_siY + 36, 124, 18, Temp, RGB( 250, 250, 250 ));
		}

		// 상대의 결정, 승인 버튼 텍스트
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 291, m_siY + 101, 34, 34, m_pMyOnlineWorld->pOnlineText->Get(1000115), RGB( 250, 250, 250 ));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 291, m_siY + 149, 34, 34, m_pMyOnlineWorld->pOnlineText->Get(8000550), RGB( 250, 250, 250 ));

		// 나의 아이디를 찍어 준다.		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 18, m_siY + 205, 124, 18, m_pMyOnlineWorld->pMyData->GetMyCharName(), RGB( 250, 250, 250 ));

		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_BMyMoney.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BMyOk1.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));		
		m_BMyOk2.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216)); 

		m_BFollower.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));		// 용병 교환 버튼
		m_BProperty.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));		// 부동산 교환 버튼
		m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	m_pYouList->Draw(pSurface);
	m_pMyList->Draw(pSurface);	

	m_pSoldier->Draw(pSurface);							// 용병 정보
}

// 돈을 받는다.
VOID	OnlineTrade::ReceiveMoney( MONEY money, SI16 uiItemNum, UI16 uiItemID, UI08 uiFollowr, UI08 uiSlot, MyItemData *pItemInfo, OnFollowerInfo *pFollower, OnIndustrialComplexTradeInfo *pProperty)
{
	if(money)													// 돈
	{
		_ui64toa( money, m_szResMoney, 10 );
		if(m_bFirstChangeMoney)		
			m_bFirstChangeMoney = FALSE;
		else
		{
			if(!m_pMyOnlineWorld->pOnlineMsgBox->IsActive())
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", /*m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHANGE_TRADE_MONEY)*/
					m_pMyOnlineWorld->pOnlineText->Get(8000549));
				m_bTradeStop = TRUE;
			}
		}
	}
	else if(pFollower)											// 용병
	{
		ItemInfo TempItemInfo;	
		TempItemInfo.siType = LIST_FOLLOWER;
		TempItemInfo.uiItemID = 1;

		memcpy(&(TempItemInfo.Follower), pFollower, sizeof(OnFollowerInfo));

		m_pYouList->AddItem(TempItemInfo, uiSlot);

		// 용병의 인벤정보를 세팅한다.
		for(SI16 i=0; i < pFollower->uiItemNum; i++)
		{
			memcpy(&m_FollowerItemData[uiSlot][i], &pItemInfo[i], sizeof(MyItemData));
		}
		
		m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
		m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(0);
		m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
	}
	else if(uiItemNum)											// 아아템
	{
		// 상대방의 아이템을 너어 준다.
		ItemInfo TempItemInfo;
		TempItemInfo.siType = LIST_ITEM;
		TempItemInfo.uiItemID = uiItemID;
		TempItemInfo.ItemCount = uiItemNum;
		
		m_pYouList->AddItem(TempItemInfo, uiSlot);
	}
	else if(pProperty)											// 부동산
	{
		ItemInfo TempItemInfo;		
		TempItemInfo.ItemCount						= 1;
		TempItemInfo.siType							= LIST_PROPERTY;
		TempItemInfo.uiItemID						= ON_TRADE_PROPERTY_ITEMID;
		memcpy(&TempItemInfo.Property, pProperty, sizeof(OnIndustrialComplexTradeInfo));
		//TempItemInfo.Property.uiVillageCode			= pProperty->uiVillageCode;
		//TempItemInfo.Property.uiIndustrialComplexID	= pProperty->uiIndustrialComplexID;

		m_pYouList->AddItem(TempItemInfo, uiSlot);
	}
}


// 서버에서 받은 상태 방의 수락 버튼 상태
VOID	OnlineTrade::SetOkButton( BOOL bFlag )
{
	m_bMyOk2	= FALSE;
	m_bYouOk1	= bFlag;

	if(m_bYouOk1)
	{
		for(int i =0; i < 8; i++)
			if(m_pYouList->GetSlotInfo(i))	m_pYouList->SetColor(i, LIST_COLOR_RED);
	}
	else
	{
		for(int i =0; i < 8; i++)
			if(m_pYouList->GetSlotInfo(i))	m_pYouList->SetColor(i, LIST_COLOR_NOT);
	}
}

// 거래할 상대의 아이를 받는다.
VOID	OnlineTrade::SetCharID(CHAR *pName)
{
	ZeroMemory(FriendInfo.szCharName, sizeof(FriendInfo.szCharName));

	if(pName)
	{
		strncpy((char*)FriendInfo.szCharName, pName, ON_ID_LENGTH);
	}
}

OnlineListControl	*OnlineTrade::GetList()
{
	return m_pMyList;
}


VOID	OnlineTrade::SetRequest()
{
	m_bRequest = TRUE;
}

// 추가된 아이템에 대한 정보를 모은다.
VOID	OnlineTrade::AddItem(SI16 siYetPos, SI16 siNewPos, UI16 uiItemID, UI32 uiQuantity, UI08 uiFollower)
{
	m_pTradeItem[ m_siCount ].uiFollower  = uiFollower;
	m_pTradeItem[ m_siCount ].siQuantity  = uiQuantity;
	m_pTradeItem[ m_siCount ].siTradePos  = siNewPos;
	m_pTradeItem[ m_siCount ].uiItemCode  = uiItemID;
	m_pYetPos[ m_siCount ]				  = siYetPos;

	if(uiFollower)
		m_bFollowerSlot[uiFollower] = TRUE;

	m_siCount++;
}

// 아이템 교환이 성공
VOID	OnlineTrade::Success(BOOL bFlag)
{
	SI16 i;
	// 성공
	if(bFlag)
	{
		// 돈
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() - _atoi64(m_szSendMoney) );
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() + _atoi64(m_szResMoney) );
	}

	// 상대방 거래 창에 있는 장부를 지워 준다.
	SI16			siSlot	=	m_pYouList->GetSlot(ON_ITEM_TRADEBOOK_ID);
	const ItemInfo	*pInfo	=	m_pYouList->GetSlotInfo(siSlot);
	
	if(pInfo)		m_pYouList->DeleteSlot(siSlot);
			
	// 나의 거래 창에 있는 장부를 지워 준다.
	siSlot	=	m_pMyList->GetSlot(ON_ITEM_TRADEBOOK_ID);
	pInfo	=	m_pMyList->GetSlotInfo(siSlot);	

	if( pInfo )		m_pMyList->DeleteSlot(siSlot);

	m_siCount				=	0;
	m_bSuccess				=	TRUE;

	for(i =0; i < 8; i++)	
	{
		pInfo = m_pMyList->GetSlotInfo(i);
		if(pInfo)
		{
			if(pInfo->Property.uiVillageCode > 0)
				m_pMyOnlineWorld->pMyData->uiPropertyVillageCode = 0;
			m_pMyList->SetColor(i, LIST_COLOR_NOT);	
		}
	}
	for(i =0; i < 8; i++)	
	{
		pInfo = m_pYouList->GetSlotInfo(i);
		if(pInfo)		
		{
			//CItemHeader	*pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(pInfo->uiItemID);
			if(pInfo->Property.uiVillageCode > 0)
				m_pMyOnlineWorld->pMyData->uiPropertyVillageCode = pInfo->Property.uiVillageCode;
			m_pYouList->SetColor(i, LIST_COLOR_NOT);
		}
	}

	SetActive(FALSE);
	m_bFirstChangeMoney = TRUE;
}


// 마지막에 넣은 아이템을 뺀다.
VOID OnlineTrade::ItemAddCancel(BOOL bCancel)
{
	if(bCancel) // 취소
	{
		if(m_siAddType == LIST_ITEM)
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADETEXT), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_ITEM_WEIGHTOVER));
		}
		else if(m_siAddType == LIST_FOLLOWER)
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADETEXT), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_SOILDER_NOTEMPTYSLOT));
		}
		else if(m_siAddType == LIST_PROPERTY)
		{
			if(m_pMyOnlineWorld->pMyData->uiPropertyVillageCode)
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADETEXT), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_PROPERTY_OWN_ERROR));
			else
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADETEXT), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_PROPERTY_NOTFINDPROPERTY));
			m_bProperty = FALSE;
			m_BProperty.SetAction(TRUE);
		}
		m_siCount--;
		m_pTradeItem[m_siCount].siQuantity = -1;
		m_pTradeItem[m_siCount].siQuantity = 0;
		m_pTradeItem[m_siCount].siTradePos = -1;
		m_pTradeItem[m_siCount].uiItemCode = 0;

		m_siAddType = -1;
		m_bFollowerSlot[m_AddFollower.Follower.uiSlot] = FALSE;
	}
	else		// 성공
	{
		// 아이템을 너어 준다.		
		if(m_siAddType == LIST_ITEM)
		{
			if(m_siItemCount == 0 )	return;	

			// 거래아이템 목록에 추가
			AddItem(m_siYetPos, m_siNewPos, m_siDragItem, m_siItemCount, m_uiCurFollower);

			// 인벤토리에서 삭제
			MyItemData	ItemData;
			ItemData.siPosInInventory	=	m_siYetPos;
			ItemData.uiID				=	m_siDragItem;
			ItemData.uiQuantity			=	m_siItemCount;
			
			m_pMyOnlineWorld->pMyData->DelItem( (UI08)m_uiCurFollower, &ItemData );
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
			m_siYetPos = m_siNewPos = m_siDragItem = m_siItemCount = 0;
			
			// 올린 아이템 리스트에 추가
			ItemInfo Add;
			Add.uiItemID = m_pTradeItem[m_siCount-1].uiItemCode;
			Add.ItemCount = m_pTradeItem[m_siCount-1].siQuantity;
			m_pMyList->AddItem(Add, m_pTradeItem[m_siCount-1].siTradePos);
		}
		else if(m_siAddType == LIST_FOLLOWER)
		{
			m_pMyList->AddItem(m_AddFollower, m_pMyList->GetEmptySlot());
		}
		else if(m_siAddType == LIST_PROPERTY)
		{
			m_AddProperty.ItemCount					= 1;
			m_AddProperty.siType					= LIST_PROPERTY;
			m_AddProperty.uiItemID					= ON_TRADE_PROPERTY_ITEMID;
			m_AddProperty.Property.uiVillageCode	= 0;//101;//m_pMyOnlineWorld->pMyData->uiPropertyVillageCode;//101;
			
			//m_bProperty = FALSE;
			m_pMyList->AddItem(m_AddProperty, m_pMyList->GetEmptySlot());
		}

		sprintf( m_szSendMoney, m_szTempSendMoney );
		m_siAddType = -1;
	}
}

VOID	OnlineTrade::SetFriendInfo(CHAR *szGuildName, SI32 siClassInGuildID, SI32 siLevel)
{
	strcpy(FriendInfo.szGuildID, szGuildName);
	FriendInfo.siGuildClass = siClassInGuildID;
	FriendInfo.siLevel		= siLevel;
}

VOID OnlineTrade::ChangeTradeGoods(BYTE *pMsg)
{
	OnResChangeTradeGoods *pOnResChangeTradeGoods = (OnResChangeTradeGoods*)pMsg;
	if(!pOnResChangeTradeGoods)		return ;
	
	if(pOnResChangeTradeGoods->dwResponse == ON_RET_NO)		ItemAddCancel(TRUE);
	else													ItemAddCancel(FALSE);

	m_siStatus = ON_TRADE_POLL;
}

VOID OnlineTrade::SetPushAccept(BOOL bIsMe)
{
	if(bIsMe)
		m_bMyOk2 = TRUE;
	else
		m_bYouOk2 = TRUE;
}