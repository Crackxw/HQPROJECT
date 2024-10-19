/************************************************************************************************
	だ橾貲 : OnlineVillageStruct-Inn.cpp
	氬渡濠 : 檜彌戮

	葆擊 罹婦 籀葬
************************************************************************************************/
#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>

#include "OnlineWorld.h"
#include "OnlineVillage.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineVillageStruct-Inn.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineMercenary.h"
#include "OnlineClient.h"
#include "OnlineNumberControl.h"
#include "OnlineFieldArea.h"
#include <etc.h>




extern	_clGame* pGame;
extern _InputDevice				IpD;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureInn::OnlineVillageStructureInn() : OnlineVillageBaseStructure()
{
	m_siMainStatus	=	ON_INN_STATUS_MAIN_START;
	m_siStatus		=	0;
	m_bCommissionOk =	FALSE;

	m_pItemList		=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureInn::~OnlineVillageStructureInn()
{	
	Free();
}

BOOL	OnlineVillageStructureInn::Action()
{
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureInn::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{		
	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_INN_MENU.Spr", m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.Spr", m_ButtonImage);	
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr", m_ExitImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_INN_GET.Spr", m_InvenImage);

	m_siMainStatus	=	ON_INN_STATUS_MAIN_START;
	m_siStatus		=	0;
	m_bCommissionOk =	FALSE;
	m_bUsableInn	=	FALSE;

	m_siX = 200;
	m_siY = 140;
	m_siInvenX = 94;
	m_siInvenY = 45;

	m_BGetItem.Create(m_siX + 127, m_siY + 162,  m_ButtonImage.Header.Xsize, m_ButtonImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INN_RECEIVE_ITEM),   BUTTON_PUT_LEFT, FALSE);
	m_BDepositFollower.Create(m_siX + 127, m_siY + 196, m_ButtonImage.Header.Xsize, m_ButtonImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(8000525), BUTTON_PUT_LEFT, FALSE);
	m_BExit.Create(m_siX + 132, m_siY + 254, m_ExitImage.Header.Xsize,  m_ExitImage.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);
	m_BSubExit.Create(m_siInvenX + 139, m_siInvenY + 394, m_ExitImage.Header.Xsize,  m_ExitImage.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);

	// 謝っ檣漸饜葬 葬蝶お 儅撩
	m_pItemList = new OnlineListControl(pOnlineWorld);
	m_pItemList->Init(60, 60, 5, 2, m_siInvenX + 15, m_siInvenY + 193, 0);

	// 檣漸饜葬 艙羲 撲薑
	SetRect(&m_rtInvenRect, 467, 280, 467+64*5, 280+64*2);

	// 嘐棲裘擊 殘朝棻.
	if( pMyOnlineWorld->pOnlineMiniMap->IsActive() )
		pMyOnlineWorld->pOnlineMiniMap->SetActive( FALSE );

	// 濰睡蒂 殘朝棻.
	if( pMyOnlineWorld->pOnlineTradeBook->IsAction() )
		pMyOnlineWorld->pOnlineTradeBook->SetAction( FALSE );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureInn::Free()
{
	clGrp.FreeXspr(m_MainImage);
	clGrp.FreeXspr(m_ButtonImage);
	clGrp.FreeXspr(m_ExitImage);
	clGrp.FreeXspr(m_InvenImage);

	if( m_pItemList )
	{
		delete m_pItemList;
		m_pItemList = NULL;
	}
	
	OnlineVillageBaseStructure::Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureInn::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	pMyOnlineWorld->pOnlineInventory->Action();

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);					// 葆辦蝶 謝ル蒂 掘и棻.

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(m_siMainStatus)
	{
		case ON_INN_STATUS_MAIN_START:
			{
				if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK && !m_bUsableInn)
				{
					m_siMainStatus	= ON_INN_STATUS_MAIN_EXIT;
					break;
				}

				m_bUsableInn = TRUE;
				int siCode = g_stConnectServerInfo.m_WorldID;
				if(!pMyOnlineWorld->pOnlineMsgBox->IsActive())
				{
					if(siCode > 3)		// 1.寥��  2.輿濛  3.⑷鼠
					{
						m_BDepositFollower.SetAction(FALSE);
					}
					else
						m_BDepositFollower.SetAction(FALSE);
				}

				if(!m_bUsableInn)		break;

				if(pMyOnlineWorld->pOnlineInventory->IsActive())	pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

				if(pMyOnlineWorld->m_siReceivedItemCount > 0)		
				{
					m_BGetItem.SetAction(TRUE);
					//m_BDepositFollower.SetAction(TRUE);
					if(pMyOnlineWorld->m_siReceivedItemCount == 10)		m_BDepositFollower.SetAction(FALSE);
				}
				else
				{
					m_BGetItem.SetAction(FALSE);
					//m_BDepositFollower.SetAction(TRUE);
				}

				m_siMainStatus = ON_INN_STATUS_MAIN_POLL;
			}
			break;
		case ON_INN_STATUS_MAIN_POLL:
			{
				if(pMyOnlineWorld->pOnlineInventory->IsActive())	pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

				if(m_BGetItem.Process(m_bMouseDown))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestInnItemList();
					m_siMainStatus = ON_INN_STATUS_MAIN_GET_ITEM;
					break;
				}
				else if(m_BDepositFollower.Process(m_bMouseDown))
				{
					m_siMainStatus = ON_INN_STATUS_MAIN_SEND_ITEM;
					break;
				}
				else if(m_BExit.Process(m_bMouseDown) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					m_siMainStatus = ON_INN_STATUS_MAIN_EXIT;
					break;
				}
			}
			break;
		case ON_INN_STATUS_MAIN_GET_ITEM:
			{
				ActionGetItem();
			}
			break;
		case ON_INN_STATUS_MAIN_SEND_ITEM:
			{
				ActionDepositFollower();
			}
			break;
		case ON_INN_STATUS_MAIN_EXIT:
			{
				m_siMainStatus = ON_INN_STATUS_MAIN_START;
				Free();
				return TRUE;
			}
			break;
	}
	
	Draw(pSurface);

	// Pannel擊 轎溘и棻.
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_TRADEBOOK
		| ONLINEPANNEL_DISABLEFLAG_MINIMAP | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON);

	if(IpD.LeftPressSwitch) m_bMouseDown = TRUE;	
	else				    m_bMouseDown = FALSE;

	return FALSE;
}

VOID	OnlineVillageStructureInn::ActionGetItem()
{
	switch(m_siStatus)
	{
	case ON_INN_STATUS_GET_ITEM_START:
		break;
	case ON_INN_STATUS_GET_ITEM_POLL:
		{
			// 葆辦蝶蒂 嬴檜蠱 嬪煎 螢溥場懊釭?
			SI16 siSlot;
			if((siSlot = m_pItemList->GetSelectSlot()) != -1)
			{
				const ItemInfo *TempItemInfo = m_pItemList->GetSlotInfo(siSlot);
				CItemHeader	*pItem = TempItemInfo == NULL ? NULL : pMyOnlineWorld->pItemsInfo->GetItem( TempItemInfo->uiItemID );

				if(pItem)
				{
					pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo*>(TempItemInfo), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_DOWN, FALSE, FALSE);
				}
			}

			if(pMyOnlineWorld->fVillageLeftOneSwitch == 1)
			{
				// 輿虞 嬴檜蠱 萄楚梵ж賊憮 檣漸饜葬 嬴檜蠱 贗葛衛 萄楚梵 腎朝唳辦
				if(pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1 && m_pItemList->GetDragSlot() != -1)
				{
					if(PtInRect(&m_rtInvenRect, ptMouse) == TRUE)
					{
						pMyOnlineWorld->pOnlineInventory->DragFail();
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_pItemList->GetSlotInfo(m_siInnSlot)->ItemCount);
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(IpD.Mouse_X, IpD.Mouse_Y);
						m_siStatus = ON_INN_STATUS_GET_ITEM_GET;
					}
				}
				// 輿虞 嬴檜蠱 萄楚梵醞橾 陽
				else if(m_pItemList->GetDragSlot() != -1)
				{
					if(PtInRect(&m_rtInvenRect, ptMouse) == TRUE)
					{
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_pItemList->GetSlotInfo(m_siInnSlot)->ItemCount);
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
						pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(IpD.Mouse_X, IpD.Mouse_Y);
						m_siStatus = ON_INN_STATUS_GET_ITEM_GET;
					}
					else
					{
						pMyOnlineWorld->m_bItemDraging = FALSE;
						m_pItemList->StopDrag();
					}
				}
				// 欽牖�� 葆辦蝶 贗葛й 陽
				else
				{
					if(m_pItemList->DragUp())
					{
						pMyOnlineWorld->m_bItemDraging = TRUE;
						m_siInnSlot = m_pItemList->GetDragSlot();
					}
				}
			}

			// 檣漸饜葬蒂 殘戲賊 釭除棻.
			if(!pMyOnlineWorld->pOnlineInventory->IsActive() || m_BSubExit.Process(m_bMouseDown))
				m_siStatus = ON_INN_STATUS_GET_ITEM_EXIT;
		}
		break;
	case ON_INN_STATUS_GET_ITEM_GET:
		// 啗骯晦 殮溘擊 嫡朝棻.
		if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() == TRUE)
		{
			if( pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				// 嫡擊 嬴檜蠱縑 婦и薑爾
				m_uiFollowerID					= pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
				const ItemInfo *TempItemInfo	= m_pItemList->GetSlotInfo(m_siInnSlot);
				m_uiItemID						= TempItemInfo->uiItemID;
				m_siInvenSlot					= pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(m_uiItemID);
				m_uiQuantity					= pMyOnlineWorld->pOnlineNumPad->siCount;
				
				// 鼠啪蟾婁 匐餌
				if(!pMyOnlineWorld->pMyData->GetFollowersInterface()->CanAddItem(m_uiFollowerID, m_uiItemID, m_uiQuantity))
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_WEIGHTOVER));
					pMyOnlineWorld->m_bItemDraging = FALSE;
					m_pItemList->StopDrag();
					m_siStatus = ON_INN_STATUS_GET_ITEM_POLL;
					break;
				}
				
				// 檣漸饜葬縑 奢除檜 氈朝雖 匐餌
				if(m_siInvenSlot == -1)
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTITEMSLOT));
					pMyOnlineWorld->m_bItemDraging = FALSE;
					m_pItemList->StopDrag();
					m_siStatus = ON_INN_STATUS_GET_ITEM_POLL;
					break;
				}

				// 嫡擎 餌褒擊 憮幗縑 憲溥遽棻.
				pMyOnlineWorld->pOnlineClient->SendMoveInnItemToInven(m_uiFollowerID, m_uiItemID, m_uiQuantity);
				
				m_siStatus = ON_INN_STATUS_GET_ITEM_GET_WAIT;
			}
			else
			{
				// 嬴檜蠱 錳楚 嬪纂煎 檜翕
				pMyOnlineWorld->m_bItemDraging = FALSE;
				m_pItemList->StopDrag();
				m_siStatus = ON_INN_STATUS_GET_ITEM_POLL;
			}
		}
		break;
	case ON_INN_STATUS_GET_ITEM_GET_WAIT:
		break;
	case ON_INN_STATUS_GET_ITEM_EXIT:
		// 葬蝶お 鐘お煤 嬪纂褻薑
		m_pItemList->Clear();
		pMyOnlineWorld->m_siInnStatus = -1;

		if(pMyOnlineWorld->m_siReceivedItemCount <= 0)		m_BGetItem.SetAction(FALSE);

		m_siStatus = ON_INN_STATUS_GET_ITEM_START;
		m_siMainStatus = ON_INN_STATUS_MAIN_POLL;
		break;
	}
}

VOID	OnlineVillageStructureInn::ActionDepositFollower()
{
}


VOID	OnlineVillageStructureInn::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC	hDC;

	switch(m_siMainStatus)
	{
		case ON_INN_STATUS_MAIN_GET_ITEM:		DrawGetItem(pSurface);			break;
		case ON_INN_STATUS_MAIN_SEND_ITEM:		DrawDepositFollower(pSurface);	break;
		case ON_INN_STATUS_MAIN_START:											break;
		case ON_INN_STATUS_MAIN_EXIT:											break;
		default:
			if( clGrp.LockSurface( pSurface ) == TRUE )
			{
				clGrp.PutSpriteT(m_siX, m_siY, m_MainImage.Header.Xsize, m_MainImage.Header.Ysize, m_MainImage.Image);

				// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
				pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 25, m_siY + 45, NPC_KIND_CROPMARKET, pMyOnlineWorld->pOnlineVillage->GetNation() );
				
				m_BGetItem.Put(&m_ButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
				m_BDepositFollower.Put(&m_ButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
				m_BExit.Put(&m_ExitImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
				
				clGrp.UnlockSurface( pSurface );
			}
			
			if( pSurface->GetDC( &hDC ) == DD_OK )
			{
				SetBkMode(hDC, TRANSPARENT);			
				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 107, m_siY + 7, 118, 13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INN_INNNAME), RGB( 200, 200, 200 ) );	
				m_BGetItem.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_BDepositFollower.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				pSurface->ReleaseDC(hDC);
			}

			

			break;
	}

	// 辨煽
	pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	// 檣漸饜葬
	if(pMyOnlineWorld->pOnlineInventory->IsActive())
	{
		pMyOnlineWorld->pOnlineInventory->SetFlagVillage(TRUE);
		
		pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(pMyOnlineWorld));
		m_pItemList->Draw( pSurface );
	}
}

VOID	OnlineVillageStructureInn::DrawGetItem(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC	hDC;
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		if(pMyOnlineWorld->pOnlineInventory->IsActive())
		{
			clGrp.PutSpriteT(m_siInvenX, m_siInvenY, m_InvenImage.Header.Xsize, m_InvenImage.Header.Ysize, m_InvenImage.Image );

			// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
			pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siInvenX + 15, m_siInvenY + 35, NPC_KIND_CROPMARKET, pMyOnlineWorld->pOnlineVillage->GetNation() );

			m_BSubExit.Put(&m_ExitImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}
		clGrp.UnlockSurface( pSurface ); 
	}
	
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode(hDC, TRANSPARENT);			
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		if(pMyOnlineWorld->pOnlineInventory->IsActive())
			m_BSubExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC( hDC );
	}
}

VOID	OnlineVillageStructureInn::DrawDepositFollower(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC	hDC;
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		if(pMyOnlineWorld->pOnlineInventory->IsActive())
			clGrp.PutSpriteT(m_siInvenX, m_siInvenY, m_InvenImage.Header.Xsize, m_InvenImage.Header.Ysize, m_InvenImage.Image );

		clGrp.UnlockSurface( pSurface );
	}
	
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode(hDC, TRANSPARENT);			
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		pSurface->ReleaseDC( hDC );
	}
}