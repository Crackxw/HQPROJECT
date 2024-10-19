#include <GSL.h>

#include	<Mouse.h>
#include	<clGame.h>

#include	"OnlineWorld.h"
#include	"OnlineFieldAuction.h"
#include	"OnlineText.h"
#include	"OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineFieldArea.h"
#include    "OnlineHelp.h"
#include	"OnlineFont.h"
#include	"OnlineMegaText.h"
#include    "OnlineVillage.h"
#include    "OnlineTrade.h"
#include    "OnlineTradeBook.h"
#include    "OnlineKeyboard.h"
#include    "OnlineFieldChat.h"
#include    "OnlineCharUI.h"
#include    "OnlineMiniMap.h"
#include    "OnlineSmith.h"
#include    "OnlineSmithParser.h"
#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include	"..\Gersang\music.h"
#include	"OnlineMercenary.h"

const int SMITH_POS_X					= 80;
const int SMITH_POS_Y		            = 85;

const int SMITH_OK_BUTTON_X             = 5;
const int SMITH_OK_BUTTON_Y             = 258;
const int SMITH_CANCEL_BUTTON_X         = 185;
const int SMITH_CANCEL_BUTTON_Y         = 258;

const int SMITH_ITEM_SLOT_X             = 38;
const int SMITH_ITEM_SLOT_Y             = 150;

const int SMITH_TEXT_WIDTH				= 224;


enum
{   
	SMITH_MAIN           = 0,
	SMITH_WAITING,
	SMITH_WAITING_REPLY
};


extern _InputDevice				IpD;
extern _clGame* pGame;


OnlineSmith::OnlineSmith()
{
	m_pMyOnlineWorld = NULL;
	m_pButtonSpr = NULL;
	m_siNoField = -1;
	m_siInvenSlot = -1;
	m_WaitTickCount = 0;
	m_uiItemID = 0;
	m_siStatus = SMITH_MAIN;
}

OnlineSmith::~OnlineSmith()
{
	Free();
}

VOID OnlineSmith::Initialize()
{
	SetAction(FALSE);

	m_siInvenSlot = -1;
	m_WaitTickCount = 0;
	m_uiItemID = 0;
	m_siStatus = SMITH_MAIN;
}

void OnlineSmith::Init(cltOnlineWorld *pWorld)
{
	m_pMyOnlineWorld = pWorld;
	
	m_pButtonSpr		=	m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_EXITBUTTON );
	
	m_BOk.Create(  0, 0, m_pButtonSpr[0].Header.Xsize, m_pButtonSpr[0].Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_APPLY), BUTTON_PUT_LEFT, TRUE );
	m_BCancel.Create(  0, 0, m_pButtonSpr[0].Header.Xsize, m_pButtonSpr[0].Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE );

	clGrp.LoadXspr( "Online\\GameSpr\\Reception_board.Spr", m_BoardSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Item_Slot.Spr", m_SlotSpr );

	m_siX = SMITH_POS_X;
	m_siY = SMITH_POS_Y;
	m_pSmithList = new OnlineListControl(m_pMyOnlineWorld);
	m_pSmithList->Init(60, 60, 1, 1, m_siX, m_siY, 0);

	SetPos(SMITH_POS_X,SMITH_POS_Y);
	SetAction(FALSE);
}

void OnlineSmith::Free()
{
	if (m_BoardSpr.Image) { clGrp.FreeXspr(m_BoardSpr); }
	if (m_SlotSpr.Image) { clGrp.FreeXspr(m_SlotSpr); }

	if(m_pSmithList)
	{
		delete m_pSmithList;
		m_pSmithList = NULL;
	}
	m_pButtonSpr = NULL;
}

void OnlineSmith::SetPos(SI32 siX,SI32 siY)
{
	m_siX = siX;
	m_siY = siY;

	m_BOk.SetX(m_siX + SMITH_OK_BUTTON_X);
	m_BOk.SetY(m_siY + SMITH_OK_BUTTON_Y);
	m_BCancel.SetX(m_siX + SMITH_CANCEL_BUTTON_X);
	m_BCancel.SetY(m_siY + SMITH_CANCEL_BUTTON_Y);

	m_pSmithList->SetPos(m_siX + SMITH_ITEM_SLOT_X,m_siY + SMITH_ITEM_SLOT_Y);

	
	if (m_siNoField != -1) {
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
		m_BoardSpr.Header.Xsize, m_siY + m_BoardSpr.Header.Ysize);
	}
}


void OnlineSmith::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	static int showcaret = 0;
	static int  caretcount = 0;
	CHAR szTemp[512];

	caretcount++;
	if (caretcount >= 10)
	{
		caretcount = 0;
		showcaret ^= 1;

	}
	HDC hDC;

	if(!m_bActive)					return;
	if(m_siStatus != SMITH_MAIN)	return;
	
	if (pSurface)
	{
		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			clGrp.PutSpriteT( m_siX, m_siY, m_BoardSpr.Header.Xsize, m_BoardSpr.Header.Ysize, m_BoardSpr.Image );
			clGrp.PutSpriteT( m_siX + SMITH_ITEM_SLOT_X - 4,m_siY + SMITH_ITEM_SLOT_Y - 4,
				m_SlotSpr.Header.Xsize,m_SlotSpr.Header.Ysize,m_SlotSpr.Image);
			
			if (m_pButtonSpr) {
				m_BOk.Put		( m_pButtonSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
				m_BCancel.Put	( m_pButtonSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
			}
			
			clGrp.UnlockSurface( pSurface );
		}
		
		m_pSmithList->Draw(pSurface);
		
		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			SetBkMode(hDC, TRANSPARENT);			
			SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
			
			m_BOk.Put( hDC );
			m_BCancel.Put( hDC );
			
			sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_DESCRIPT1));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+36, m_siY+40, SMITH_TEXT_WIDTH, szTemp, RGB(0, 0, 0));
			sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_DESCRIPT2));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+36, m_siY+70, SMITH_TEXT_WIDTH, szTemp, RGB(0, 0, 0));
			
			if (m_pSmithList->GetItemNum() == 1)
			{
				ItemInfo const *pTempItemInfo = NULL;
				pTempItemInfo = const_cast<const ItemInfo *>(m_pSmithList->GetSlotInfo(0));
				SmithItemHeader* TempSmithItemHeader = m_pMyOnlineWorld->pOnlineSmithParser->GetSmithItem(pTempItemInfo->uiItemID);
				
				if(TempSmithItemHeader)
				{
					sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_DESCRIPT3), TempSmithItemHeader->moNeedMoney);
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+36, m_siY+105, SMITH_TEXT_WIDTH, szTemp, RGB(0, 0, 0));
				}
			}
			
			pSurface->ReleaseDC( hDC );
		}
	}
}

void OnlineSmith::SetAction(BOOL bActive)
{
	m_bActive = bActive;
	m_bMouseSwitch = FALSE;
	// 활성화가 될때는 움직이지 못하게 하고 비활성화 될때는 움직이게 한다.
	if (m_bActive)
	{
		// 인벤토리가 닫혀 있으면 열어 준다.
		if(!m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);
		// 장부가 열려 있으면 닫아준다.
		if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

		m_pSmithList->Clear();

		m_siInvenSlot = -1;

		if (m_siNoField != -1) {
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		}
		m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
						m_BoardSpr.Header.Xsize, m_siY + m_BoardSpr.Header.Ysize);

		m_siStatus = SMITH_MAIN;
		m_uiFollowerID = -1;

		m_BOk.SetAction(FALSE);

		// 못움직이고 기타 커맨드 전부 사용불가!!!
		m_pMyOnlineWorld->bCharMove = FALSE;
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);
	}
	else
	{
		if(m_pMyOnlineWorld->pOnlineInventory->IsActive()) m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

		if (m_siNoField != -1) {
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		}

		m_siNoField = -1;

		// 이동 가능 및 기타커맨드 사용가능!!!
		m_pMyOnlineWorld->bCharMove = TRUE;
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
	}
}

void OnlineSmith::Poll()
{
	if (m_bActive)
	{
		switch(m_siStatus) {
			
		case SMITH_MAIN:
			// Ok 버튼을 눌렀을때
			if (m_BOk.Process(m_bMouseSwitch))
			{
				// 만약 올려놓은 아이템이 있다면 경매 신청 메시지를 보낸다.
				if (m_pSmithList->GetItemNum() == 1)
				{
					ItemInfo const *pTempItemInfo = NULL;
					pTempItemInfo = const_cast<const ItemInfo *>(m_pSmithList->GetSlotInfo(0));
					m_uiItemID = pTempItemInfo->uiItemID;

					SmithItemHeader* TempSmithItemHeader = m_pMyOnlineWorld->pOnlineSmithParser->GetSmithItem(m_uiItemID);

					// 제련 가능한 아이템인지
					if(!TempSmithItemHeader)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_PRE_FAIL1));
						SetAction(FALSE);
						break;
					}

					// 요금 지불할수 있는지
					DWORD fee = (DWORD)TempSmithItemHeader->moNeedMoney;
					if (m_pMyOnlineWorld->pMyData->GetMoney() < fee)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_PRE_FAIL2));
						SetAction(FALSE);
						break;
					}

					// 빈슬롯 하나 이상있는지
					m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( m_uiFollowerID );		//.actdoll (021224)
					m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(m_uiFollowerID);
					m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetItemNum() == 12)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_PRE_FAIL3));
						SetAction(FALSE);
						break;
					}
					
					m_siStatus = SMITH_WAITING;
					m_WaitTickCount = GetTickCount();
					m_BOk.SetAction(FALSE);
				}
			}
			else if (m_BCancel.Process(m_bMouseSwitch))
			{
				SetAction(FALSE);
			}

			// 인벤토리에서 드래그 하고있는 아이템 체크
			// 리스트가 비어있을 때만 체크
			if (m_pSmithList->GetItemNum() == 0)
			{
				if(IpD.LeftPressSwitch)
				{
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->DragUp())
						m_siInvenSlot = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot();
				}
			}
			
			// 인벤토리 칸에 집어넣는다.
			if (m_pSmithList->GetSelectSlot() != -1 && IpD.LeftPressSwitch)
			{
				// 드래그 멈춤!
				m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();

				// 리스트가 비어있고
				if (m_pSmithList->GetItemNum() == 0)
				{
					// 들고있는 아이템이 있으면
					if (m_siInvenSlot != -1)
					{
						m_uiFollowerID = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();

						ItemInfo const *pTempItemInfo = NULL;	
						pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
						ItemInfo TempItemInfo;
						if(pTempItemInfo)
						{
							TempItemInfo.uiItemID		= pTempItemInfo->uiItemID;
							TempItemInfo.ItemCount		= 1;
						}
						m_pSmithList->AddItem(TempItemInfo,0);
						
						// 경매 신청버튼을 누를수있다~
						m_BOk.SetAction(TRUE);
					}
				}
			}
			break;
		case SMITH_WAITING:
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_WAIT));

				if((GetTickCount() - m_WaitTickCount) >= 3000)
				{
					m_WaitTickCount = 0;
					// 서버로 제련할 아이템을 보낸다.
					m_pMyOnlineWorld->pOnlineClient->SendRequestRefineItem(m_uiFollowerID, m_uiItemID);
					m_siStatus = SMITH_WAITING_REPLY;
				}
			}
			break;
		case SMITH_WAITING_REPLY:
			{
			}
			break;
		}

		if (IpD.LeftPressSwitch) 			m_bMouseSwitch = TRUE;
		else								m_bMouseSwitch = FALSE;
	}
}

void OnlineSmith::RefineItem(BYTE *pMsg)
{
	OnResponseRefineItem *pOnResponseRefineItem = (OnResponseRefineItem *)pMsg;

	if(pOnResponseRefineItem->uiResult == 0)
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_RESULT_FAIL1));
	}
	else
	{
		CHAR	Temp[1024];
		ZeroMemory(Temp, sizeof(Temp));
		CItemHeader *pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(pOnResponseRefineItem->uiNewItemID);
		if(pItem)
			sprintf(Temp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_RESULT_SUCCESS), m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
		else
			sprintf(Temp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_RESULT_FAIL2));

		switch(pOnResponseRefineItem->uiResult)
		{
		case 1:		// 고급아이템
			// 효과음 출력
			PushEffect(EFFECT_GETITEM,     0, 0);
			PushEffect(EFFECT_QUEST_CLEAR, 0, 0);
			break;
		case 2:		// 일반아이템
			// 효과음 출력
			PushEffect(EFFECT_GETITEM,     0, 0);
			PushEffect(EFFECT_QUEST_CLEAR, 0, 0);
			break;
		case 3:		// 낡은아이템
			sprintf(Temp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_RESULT_FAIL3));

			// 효과음 출력
			PushEffect(EFFECT_GETITEM,     0, 0);
			PushEffect(EFFECT_QUEST_CLEAR, 0, 0);
			break;
		case 4:		// 부서짐
			sprintf(Temp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SMITH_MANUFACTURE_RESULT_FAIL4));

			// 효과음 출력
			PushEffect(EFFECT_GETITEM,     0, 0);
			PushEffect(EFFECT_QUEST_CLEAR, 0, 0);
			break;
		}

		// 결과 메세지를 보여준다.
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"", Temp);

		// 말풍선에 찍어준다.
		m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), Temp);
		m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendChatting(Temp);

		if(pOnResponseRefineItem->uiResult == 1 || pOnResponseRefineItem->uiResult == 2)
		{
			// 생산된 아이템 추가
			MyItemData MyItemData;
			MyItemData.siPosInInventory	=	pOnResponseRefineItem->uiNewItemSlot;
			MyItemData.uiID				=	pOnResponseRefineItem->uiNewItemID;
			MyItemData.uiQuantity		=	1;				
			m_pMyOnlineWorld->pMyData->AddItem((UI08)pOnResponseRefineItem->uiFollowerSlot, &MyItemData);
			m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( (UI08)pOnResponseRefineItem->uiFollowerSlot );		//.actdoll (021224)
			m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot((UI08)pOnResponseRefineItem->uiFollowerSlot);
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
		}

		ItemInfo const *pTempItemInfo = NULL;	
		pTempItemInfo = const_cast<const ItemInfo *>(m_pSmithList->GetSlotInfo(0));

		if(pOnResponseRefineItem->uiResult != 3)
		{
			// 올려놓은 아이템을 빼준다
			m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(m_uiFollowerID);
			
			MyItemData TempItemData;
			TempItemData.siPosInInventory	= m_siInvenSlot;
			TempItemData.uiID				= pTempItemInfo->uiItemID;
			TempItemData.uiQuantity			= 1;
			
			m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->DeleteSlot(m_siInvenSlot, 1);
			m_pMyOnlineWorld->pMyData->DelItem(m_uiFollowerID, &TempItemData);
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
		}
		
		// 요금지불
		SmithItemHeader* TempSmithItemHeader = m_pMyOnlineWorld->pOnlineSmithParser->GetSmithItem(pTempItemInfo->uiItemID);
		DWORD fee = (DWORD)TempSmithItemHeader->moNeedMoney;
		m_pMyOnlineWorld->pMyData->SetMoney(m_pMyOnlineWorld->pMyData->GetMoney() - fee);
	}
	SetAction(FALSE);
	
}