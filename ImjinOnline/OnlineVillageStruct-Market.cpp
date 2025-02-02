// 薑霞遵
// 衛濰

#include <GSL.h>
#include <Mouse.h>
#include "charanimation.h"
#include <kindinfo.h>
#include <etc.h>

#include "OnlineWorld.h"
#include "OnlineVillage.h"
#include "OnlineNumberControl.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineNumberControl.h"
#include "OnlineVillageStruct-Market.h"
#include "OnlineVillageSave.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "OnlineVillageSave.h"
#include "OnlineItemList.h"
#include "IProfit.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "OnlineClient.h"
#include "OnlineMsgBox.h"
#include "OnlineMessenger.h"
#include "OnlineMercenary.h"

#define ON_MAX_ITEMLIST		1024

#define ON_ITEM_SELL		1
#define	ON_ITEM_BUY			2


extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// 議葛攪 謙盟滌 等檜攪 


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureMarket::OnlineVillageStructureMarket() : OnlineVillageBaseStructure()
{
	pPlayerLC			=	NULL;
	pNpcLC				=	NULL;
	pButtonSpr			=	NULL;

	siStatus			=	0;
	siSell				=	0;
	dwDelayMouse1		=	0;
	siDelayTime			=	0;
	siRecvItemNum		=	0;


	siSelectNPCItemIndex    = -1;
	siSelectPlayerItemIndex = -1;
	siSelectItemTime        = 0;
	siMouseStatus			= 0;
	dwDelayMouse2			= 0;
	bTotSelect				= FALSE;
	siDealerKind			= 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureMarket::~OnlineVillageStructureMarket()
{	
	Free();
}

BOOL	OnlineVillageStructureMarket::Action()
{
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMarket::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{
	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	m_siX = 95;
	m_siY = 45;

	siKind		=	ON_VIL_ST_KIND_MARKET;																	// 衛濰檜棻.
	siDragItem	=	-1;
	bExit		=	FALSE;

	clGrp.LoadXspr( "Online\\GameSpr\\Village\\Town_Store.Spr", m_MainImage);
	clGrp.LoadXspr( "Online\\GameSpr\\Inventory\\main_button1.Spr", m_ExitImage);

	// 葬蝶お 鐘お煤 儅撩擊 и棻.
	pPlayerLC	=	pOnlineWorld->pOnlineInventory->GetLCInventory();
	pNpcLC		=	new OnlineListControl(pOnlineWorld);												// 鼻薄辨 葬蝶お 鐘お煤檜棻.

	pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
	pMyOnlineWorld->pOnlineTradeBook->KillScroll();

	pNpcLC->Init(60, 60, 5, 4, m_siX + 15, m_siY + 129, 0);

	// Npc 艙羲 撲薑
	SetRect(&m_rtNpcRect, m_siX + 15, m_siY + 129, m_siX + 15 + 320, m_siY + 129 + 256);
	SetRect(&m_rtPlayerRect, 457 + 10, 45 + 235, 457 + 10 + 320, 45 + 235 + 128);

	m_BExit.Create(m_siX + 139, m_siY + 394, m_ExitImage.Header.Xsize, m_ExitImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);
	
	pMyOnlineWorld->pOnlineClient->SendRequestEnterMarket();

	siStatus	=	ON_MARKET_STATUS_ENTER_MARKET;

	pMyOnlineWorld->pOnlineInventory->SetFlagVillage(TRUE);

	// 籀擠 鼻薄縑 菟橫陛賊 釭螃朝 鼻檣曖 渠餌
	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK1;
	switch(rand() % 6)
	{
		case 0:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK2;	break;
		case 1:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK3;	break;
		case 2:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK4;	break;
		case 3:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK5;	break;
		case 4:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK6;	break;
	}

	if( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation == ON_RET_GLD_ENEMY )
		NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK7;
	// robypark 2004/12/22 17:34
	// 瞳渠鼻欽 模樓 葆擊, 奢撩瞪ж朝 陳曖 葆擊 檣唳辦紫 鼻檣曖 渠餌 滲唳
	else if( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation == ON_RET_GLD_WAR )
		NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK7;

	else if( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation == ON_RET_GLD_MINE )
		NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_NOBUY_NOCREDIT;
		

	siSelectNPCItemIndex    = -1;
	siSelectPlayerItemIndex = -1;
	siSelectItemTime        = 0;	

	uiOldSelect = pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
	pMyOnlineWorld->pOnlineInventory->SetInStruct( ON_STRUCT_MARKET );
	pMyOnlineWorld->pOnlineInventory->SetExitButton(FALSE);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMarket::Free()
{ 		
	if(pNpcLC)	delete pNpcLC;

	clGrp.FreeXspr(m_MainImage);
	clGrp.FreeXspr(m_ExitImage);

	pNpcLC	     			= NULL;
	siSelectNPCItemIndex    = -1;
	siSelectPlayerItemIndex = -1;
	siSelectItemTime        = 0;	

	OnlineVillageBaseStructure::Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureMarket::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC					hDC;
	CItemHeader			*pItem		= NULL;	
	const	ItemInfo	*pSlot		= NULL;
	POINT				ptMouse;
	SI08*				TempText;
	SI16				TempSI;
	const ItemInfo		*pItemSlot	=	NULL;
	const ItemInfo		*pItemSlot1	=	NULL;
	CItemHeader			*lpItemHeader;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);														// 葆辦蝶 謝ル蒂 掘и棻.

	// Pannel Flag 匐餌
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_TRADEBOOK | ONLINEPANNEL_DISABLEFLAG_MINIMAP | ONLINEPANNEL_DISABLEFLAG_IMOTICON | ONLINEPANNEL_DISABLEFLAG_CHAT | ONLINEPANNEL_DISABLEFLAG_OPTION  );

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(siStatus)
	{
	case ON_MARKET_STATUS_ENTER_MARKET:
		break;

	case ON_MARKET_STATUS_DELAY:
		siDelayTime++;
		if( siDelayTime >= 5 )
		{
			siStatus	= ON_MARKET_STATUS_RECEIVE_POLL;
			siDelayTime	= 0;
		}
		break;

	case ON_MARKET_STATUS_RECEIVE_ITEMLIST:		
		break;
		
	case ON_MARKET_STATUS_RECEIVE_POLL:	
		{
			// ⑷營 葆辦蝶陛 氈朝 嬪纂縑 氈朝 NPC陛 陛雖朝 Item 匐餌
			siSelectItemTime++;
			
			// 檣漸饜葬朝 衛濰縑憮 鼠褻勒 �側瘓� 鼻鷓 檜棻.
			if( pMyOnlineWorld->pOnlineInventory->IsActive() == FALSE )	pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );
			
			pMyOnlineWorld->pOnlineInventory->Action();

			if(pNpcLC->GetDragSlot() != -1)
				pPlayerLC->StopDrag();
			
			TempSI = pNpcLC->GetSelectSlot();
			if(TempSI != siSelectNPCItemIndex)
			{
				siSelectNPCItemIndex    = TempSI;
				siSelectPlayerItemIndex = -1;
				siSelectItemTime        = 0;
			}
			TempSI = pPlayerLC->GetSelectSlot();
			if(TempSI != siSelectPlayerItemIndex)
			{
				siSelectNPCItemIndex    = -1;
				siSelectPlayerItemIndex = TempSI;
				siSelectItemTime        = 0;
			}
			
			MoveItem();																								// 嬴檜蠱曖 遺霜歜縑 評塭 嬴檜蠱擊 餌堅 っ棻		
			
			// 棻艇 辨煽擊 贗葛衛 嬴檜蠱 陛問擊 滲唳衛霽
			{
				UI08 uiTempSelect = pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
				
				// 虜擒 棻艇 辨煽擊 摹鷗擊 п評賊..
				if( uiTempSelect != uiOldSelect )
				{
					uiOldSelect = uiTempSelect;
					
					for(SI16 ix=0 ; ix<siRecvItemNum; ix++)
					{
						for(SI16 iy=0; iy<ON_MYDATA_MAX_ITEM_NUM; iy++)
						{
							if(!pPlayerLC->GetSlotInfo(iy))		continue;
							if(!pNpcLC->GetSlotInfo(ix))		continue;
							if(pNpcLC->GetSlotInfo(ix)->uiItemID != pPlayerLC->GetSlotInfo(iy)->uiItemID)	continue;

							ItemInfo TempItemInfo;
							pPlayerLC->CopyData(iy, TempItemInfo);
							TempItemInfo.uiSellPrice = pNpcLC->GetSlotInfo(ix)->uiSellPrice;
							pPlayerLC->AddItem(TempItemInfo, iy);
						}
					}
				}
			}
			
			// 葆擊縑憮 釭陛朝 匙擊 羹觼 л TRUE檜賊 衛濰縑憮 釭除棻.
			if(NowTradesmanTalk == ON_TEXT_ITEMHELP_TRADESMAN_LEAVE)
			{
				NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK1;
				switch(rand() % 6)
				{
				case 0:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK2;	break;
				case 1:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK3;	break;
				case 2:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK4;	break;
				case 3:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK5;	break;
				case 4:	NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK6;	break;
				}
			}
			
			if( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation == ON_RET_GLD_ENEMY )
				NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK7;
			// robypark 2004/12/22 17:34
			// 瞳渠鼻欽 模樓 葆擊, 奢撩瞪ж朝 陳曖 葆擊 檣唳辦紫 鼻檣曖 渠餌 滲唳
			else if( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation == ON_RET_GLD_WAR )
				NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_RANDTALK7;
			
//			else if( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation == ON_RET_GLD_MINE )
//				NowTradesmanTalk =  ON_TEXT_ITEMHELP_TRADESMAN_NOBUY_NOCREDIT;
			
			
			pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);

			if(m_BExit.IsInMouse() == TRUE)
			{
				NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_LEAVE;
			}
			if(m_BExit.Process(bMouseDown) == TRUE)
			{											
				siStatus	=	ON_MARKET_STATUS_LEAVE_MARKET;
				
				// 葆擊縑憮 釭除棻堅 詭衛雖蒂 爾魚棻.			
				pMyOnlineWorld->pOnlineClient->SendRequestLeaveMarket();
				pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
				pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
				pMyOnlineWorld->pOnlineInventory->SetFlagVillage(FALSE);
			}
			if(CheckExit() == TRUE)
			{											
				siStatus	=	ON_MARKET_STATUS_LEAVE_MARKET;
				
				// 葆擊縑憮 釭除棻堅 詭衛雖蒂 爾魚棻.			
				pMyOnlineWorld->pOnlineClient->SendRequestLeaveMarket();		
			}
		}
		break;

	case ON_MARKET_STATUS_ITEMSELL_WAIT:
		if( ( pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() ) && ( pMyOnlineWorld->pOnlineNumPad->siCount != 0 ) )
		{
			SI16	siTempItemNum=0, siTempCount=0;
			if(uiCurFollower != pMyOnlineWorld->pOnlineInventory->GetMercenarySlot())
			{
				uiID										=	0;
				pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
				siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
				siDragItem	=	-1;
				
				pMyOnlineWorld->pOnlineInventory->DragFail();
				
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				pMyOnlineWorld->m_pMercenary->NoAction( FALSE );
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKET_FOLLOWER_NO_CHANGE));
				break;
			}

			// 識 嬴檜蠱 熱榆擊 掘и棻.
			SI16 siSlot		=	pPlayerLC->GetSlot(uiID);
			siTempItemNum	=	pPlayerLC->GetSlotInfo(siSlot)->ItemCount;

			if( pMyOnlineWorld->pOnlineNumPad->siCount >= siTempItemNum )
				pMyOnlineWorld->pOnlineNumPad->siCount	=	siTempItemNum;

			// 憮幗縑 嬴檜蠱擊 っ棻掘 蹂羶擊 и棻.
			pMyOnlineWorld->pOnlineClient->SendSellItem(uiCurFollower, uiID, pMyOnlineWorld->pOnlineNumPad->siCount);

			pMyOnlineWorld->pOnlineInventory->DragFail();
			siStatus		=	ON_MARKET_STATUS_ITEMSELL_OK;
		}
		// 熱榆檜 0檜嘎煎 つ熱陛 橈棻.
		else if(( pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() ) && ( pMyOnlineWorld->pOnlineNumPad->siCount == 0 ) )
		{
			uiID										=	0;
			pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
			siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
			siDragItem	=	-1;

			pMyOnlineWorld->pOnlineInventory->DragFail();

			pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
			pMyOnlineWorld->m_pMercenary->NoAction( FALSE );
		}
		break;

	case ON_MARKET_STATUS_ITEMSELL_OK:						
		break;
		
	case ON_MARKET_STATUS_ITEMBUY_WAIT:
		{
			if( ( pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() ) && ( pMyOnlineWorld->pOnlineNumPad->siCount != 0 ) )
			{		
				OnReqBuyItemMsg*	lpOnReqBuyItemMsg = new OnReqBuyItemMsg;
				ZeroMemory( lpOnReqBuyItemMsg, sizeof( OnReqBuyItemMsg ) );
				
				pSlot =	pNpcLC->GetSlotInfo( siTempDrag );
				CItemHeader	*pTempItem = pMyOnlineWorld->pItemsInfo->GetItem(pSlot->uiItemID);
				
				// 褐辨紫蒂 匐餌и棻.
				if( pTempItem->m_uiTradeGrade > pMyOnlineWorld->pMyData->GetTradeGrade() )
				{
					// 褐辨紫 爾棻 雪戲賊 髦熱陛 橈評
					siDragItem									=	-1;
					uiID										=	0;
					pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
					siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
					
					pNpcLC->StopDrag();
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();

					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORTRADE));
					pMyOnlineWorld->m_pMercenary->NoAction( FALSE );
					
					NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_NOBUY_NOCREDIT;
					break;
				}
				
				if(pMyOnlineWorld->pOnlineNumPad->siCount >= pSlot->ItemCount) pMyOnlineWorld->pOnlineNumPad->siCount = pSlot->ItemCount;
				
				siBuyItemNum      = pMyOnlineWorld->pOnlineNumPad->siCount;
				
				// 偽擎 ITem檜 盪濰脹 盪濰 陛棟и 奢除 瓊晦
				UI16 uiFollower = pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
				if(uiCurFollower != uiFollower)
				{
					siDragItem									=	-1;
					uiID										=	0;
					pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
					siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
					
					pNpcLC->StopDrag();
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					pMyOnlineWorld->m_pMercenary->NoAction( FALSE );
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKET_FOLLOWER_NO_CHANGE));
					break;
				}

				siBuySlot = pPlayerLC->GetEmptySlot(pSlot->uiItemID);
				
				if(siBuySlot != -1)
				{
					lpOnReqBuyItemMsg->uiPosInInventory	= siBuySlot;
					lpOnReqBuyItemMsg->uiItemQuantity   = siBuyItemNum;
				}
				else	// 虜擒 髦奢除檜 橈戲賊
				{
					uiID										=	0;
					pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
					siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
					
					pNpcLC->StopDrag();
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					pMyOnlineWorld->m_pMercenary->NoAction( FALSE );

					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORZONE));
					
					NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_NOBUY_NOSELL;
				}
				
				// ⑷營 鼠啪蒂 匐餌ж罹 斜 虜躑 菟熱 氈朝雖 匐餌
				lpItemHeader = pMyOnlineWorld->pItemsInfo->GetItem(pSlot->uiItemID);
				if(pMyOnlineWorld->pMyData->GetFollowersInterface()->CanAddItem(uiCurFollower, pSlot->uiItemID, (UI16)pMyOnlineWorld->pOnlineNumPad->siCount) == FALSE)
				{
					uiID										=	0;
					pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
					siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
					
					pNpcLC->StopDrag();
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					pMyOnlineWorld->m_pMercenary->NoAction( FALSE );

					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTWEIGHT));
					
					NowTradesmanTalk = ON_TEXT_ITEMHELP_OVER_WEIGHT;
				}
				else
				{
					//lpOnReqBuyItemMsg->uiItemQuantity	= 50000;
					lpOnReqBuyItemMsg->dwMsgType        = ON_REQUEST_BUY_ITEM;
					lpOnReqBuyItemMsg->uiFollowerID     = uiCurFollower;
					lpOnReqBuyItemMsg->uiItemID         = pSlot->uiItemID;
					pMyOnlineWorld->pOnlineClient->SendBuyItem((OnReqBuyItemMsg*)lpOnReqBuyItemMsg);			// 憮幗縑 嬴檜蠱擊 骯棻掘 蹂羶擊 и棻.				
					siStatus	=	ON_MARKET_STATUS_ITEMBUY_OK;
				}

				delete lpOnReqBuyItemMsg;
			}
			else if((pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() ) && ( pMyOnlineWorld->pOnlineNumPad->siCount == 0 ) )											// 熱榆檜 0檜 嘎煎 髦熱陛 橈棻.
			{
				uiID										=	0;
				pMyOnlineWorld->pOnlineNumPad->siCount		=	0;
				siStatus									=	ON_MARKET_STATUS_RECEIVE_POLL;
				
				pNpcLC->StopDrag();
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				pMyOnlineWorld->m_pMercenary->NoAction( FALSE );
			}
		}
		break;

	case ON_MARKET_STATUS_ITEMBUY_OK:
		// MarketBuyItem()
		break;

	case ON_MARKET_STATUS_LEAVE_MARKET:
		// MarketLeave()
		break;

	case ON_MARKET_STATUS_RESPONSEITEMMOVE:
		// MarketMoveItem()
		break;
	}

	CHAR szMarketName[128];
	
	if(siStatus != ON_MARKET_STATUS_ENTER_MARKET && siStatus != ON_MARKET_STATUS_RECEIVE_ITEMLIST)
	{
		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			clGrp.PutSpriteT( m_siX, m_siY, m_MainImage.Header.Xsize, m_MainImage.Header.Ysize, m_MainImage.Image );
			
			// 蟾鼻擊 斜萼棻.
			// - 霞遵 2003.08.14 熱薑 -
			SI16	siDrawProtraitKind;
			
			switch( pVillageManager->GetEnterStructureKind() )
			{
			case ON_VIL_ST_KIND_FSTORE:				// 諄瞪
				{
					siDrawProtraitKind = NPC_KIND_CROPMARKET;
					strcpy(szMarketName, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FSTORE));
				}
				break;
			case ON_VIL_ST_KIND_DSTORE:				// 擒寞
				{
					siDrawProtraitKind = NPC_KIND_DRUGMARKET;
					strcpy(szMarketName, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DSTORE));
				}
				break;
			case ON_VIL_ST_KIND_WSTORE:				// 鼠晦薄
				{
					siDrawProtraitKind = NPC_KIND_WEAPONSHOP;
					strcpy(szMarketName, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WSTORE));
				}
				break;			
			case ON_VIL_ST_KIND_MARKET:				// 衛瞪
				{
					siDrawProtraitKind = NPC_KIND_NORMALMARKET;
					strcpy(szMarketName, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKET));
				}
				break;

			default	:
				break;
			}
			
//			DrawNPCProtrait(siDrawProtraitKind, pMyOnlineWorld->pOnlineVillage->GetNation(), m_siX + 27, m_siY + 18, pSurface, FALSE);
			// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
			pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 33, siDrawProtraitKind, pMyOnlineWorld->pOnlineVillage->GetNation() );


			m_BExit.Put(&m_ExitImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			
			clGrp.UnlockSurface( pSurface );
		}
		
		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			BOOL		fTalk;
			SelectObject( hDC, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );
			SetBkMode( hDC, TRANSPARENT );

			// 鼻薄檜葷 ル衛
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 114, m_siY + 7, 118, 13, szMarketName);

			//=================================================================================================================
			// 輿檣濰曖 渠餌
			SetBkMode(hDC, TRANSPARENT);	
			SetTextColor(hDC, 0x00ffffff);
			
			fTalk = TRUE;
			
			if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow() == FALSE)
			{
				char	TempBBB[1024];
				char*	TempItemInfoStr;
				char*	TempItemNameStr;
				
				// 陛問 薑爾蒂 鼻檣檜 蜓л
				if(siSelectItemTime > 30)
				{
					if(siSelectNPCItemIndex != -1)
					{
						// 鼻檣檜 陛雖堅 氈朝 嬴檜蠱擊 摹鷗衛
						pItemSlot	=	pNpcLC->GetSlotInfo(siSelectNPCItemIndex);
						pItem = pItemSlot == NULL ? NULL : pMyOnlineWorld->pItemsInfo->GetItem(pItemSlot->uiItemID);
						if(pItem)
						{
							if(pItemSlot->ItemCount)
							{
								// 鼻檣曖 嬴檜蠱曖 營堅陛 氈擊衛
								TempItemInfoStr = pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_ITEMINFO);
								TempItemNameStr = pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);
								
								if(TempItemInfoStr && TempItemNameStr)
								{
									sprintf((char*)TempBBB, TempItemInfoStr, TempItemNameStr, pItemSlot->uiBuyPrice, pItemSlot->uiSellPrice);
									pMyOnlineWorld->pOnlineText->Josa((char*)TempBBB);								
									
									pMyOnlineWorld->pOnlineMegaText->DrawTextLine(hDC, m_siX + 115, m_siY + 40, 200, (char*)TempBBB);
									
									fTalk = FALSE;
								}
							}
							else
							{
								// 鼻檣曖 嬴檜蠱曖 營堅陛 橈擊衛
								TempItemInfoStr = pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_HELP_NOITEMTALK);
								TempItemNameStr = pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);
								
								if(TempItemInfoStr && TempItemNameStr)
								{
									sprintf((char*)TempBBB, TempItemInfoStr, TempItemNameStr, pItemSlot->uiSellPrice);
									pMyOnlineWorld->pOnlineText->Josa((char*)TempBBB);
									
									pMyOnlineWorld->pOnlineMegaText->DrawTextLine(hDC, m_siX + 115, m_siY + 40, 200, (char*)TempBBB);
									
									fTalk = FALSE;
								}
							}
						}
					}
					else if(siSelectPlayerItemIndex != -1)
					{
						// Player陛 陛雖堅 氈朝 嬴檜蠱擊 摹鷗衛
						pItemSlot	=	pPlayerLC->GetSlotInfo(siSelectPlayerItemIndex);
						pItem		=	pItemSlot != NULL ?	pMyOnlineWorld->pItemsInfo->GetItem(pItemSlot->uiItemID) : NULL;
						
						if(pItem)
						{
							// 鼻檣檜 偽擎 嬴檜蠱擊 陛雖朝雖 匐餌
							SI16 siSlot =	pNpcLC->GetSlot(pItemSlot->uiItemID);
							pItemSlot1  =	siSlot != -1 ? pNpcLC->GetSlotInfo(siSlot) : NULL;
							
							if(pItemSlot1)
							{
								if(pItemSlot1->ItemCount)
								{
									// 鼻檣曖 嬴檜蠱曖 營堅陛 氈擊衛
									TempItemInfoStr = pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_ITEMINFO);
									TempItemNameStr = pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);
									
									if(TempItemInfoStr && TempItemNameStr)
									{
										if((pItemSlot->uiBuyPrice > 0) && (pItemSlot->uiSellPrice > 0))
										{
											sprintf((char*)TempBBB, TempItemInfoStr, TempItemNameStr, pItemSlot->uiBuyPrice, pItemSlot->uiSellPrice);
											pMyOnlineWorld->pOnlineText->Josa((char*)TempBBB);
											
											pMyOnlineWorld->pOnlineMegaText->DrawTextLine(hDC, m_siX + 115, m_siY + 40, 200, (char*)TempBBB);
											
											fTalk = FALSE;
										}
									}
								}
								else
								{
									// 鼻檣曖 嬴檜蠱曖 營堅陛 橈擊衛
									TempItemInfoStr = pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_HELP_NOITEMTALK);
									TempItemNameStr = pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);
									
									if(TempItemInfoStr && TempItemNameStr)
									{
										if((pItemSlot->uiBuyPrice > 0) && (pItemSlot->uiSellPrice > 0))
										{
											sprintf((char*)TempBBB, TempItemInfoStr, TempItemNameStr, pItemSlot->uiSellPrice);
											pMyOnlineWorld->pOnlineText->Josa((char*)TempBBB);
											
											pMyOnlineWorld->pOnlineMegaText->DrawTextLine(hDC, m_siX + 115, m_siY + 40, 200, (char*)TempBBB);
											
											fTalk = FALSE;
										}
									}
								}
							}
						}
					}
				}
		}
		
		if(fTalk)
		{
			TempText = (SI08*)pMyOnlineWorld->pOnlineText->Get(NowTradesmanTalk);
			if(TempText)
			{
				pMyOnlineWorld->pOnlineMegaText->DrawTextLine(hDC, m_siX + 115, m_siY + 40, 200, (char*)TempText);
			}
		}
		m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC( hDC );
	}
	}

	if( siDragItem != -1 )	pMyOnlineWorld->pIOnlineMouse->ShowMouse(FALSE);

	//=================================================================================================================
	// 檜彌戮 熱薑 : Item 檜翕衛 試盪 鎰朝 牖憮蒂 薑л(檜勘 ж雖 彊戲賊, Item 檜翕衛 檜翕ж朝 Item檜 List Control曖 嬴檜蠱 嬴楚縑 梃葬朝 唳辦陛 儅梯)
	if(pPlayerLC->GetDragSlot() != -1)
	{
		if(siStatus != ON_MARKET_STATUS_ENTER_MARKET || siStatus != ON_MARKET_STATUS_RECEIVE_ITEMLIST )
		{
			pNpcLC->Draw( pSurface );																				// 鼻薄縑 氈朝 嬴檜蠱擊 斜溥 遽棻.
			// 遽鷓 熱薑 //pPlayerLC->Draw( pSurface );
		}

		pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

		pMyOnlineWorld->pOnlineInventory->Draw(pSurface);
	}
	else if(pNpcLC->GetDragSlot() != -1)
	{
		pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

		pMyOnlineWorld->pOnlineInventory->Draw(pSurface);

		if(siStatus != ON_MARKET_STATUS_ENTER_MARKET || siStatus != ON_MARKET_STATUS_RECEIVE_ITEMLIST )
		{
			// 遽鷓 熱薑 //pPlayerLC->Draw( pSurface );
			pNpcLC->Draw( pSurface );																				// 鼻薄縑 氈朝 嬴檜蠱擊 斜溥 遽棻.
		}
	}
	else
	{		
		if(siStatus != ON_MARKET_STATUS_ENTER_MARKET || siStatus != ON_MARKET_STATUS_RECEIVE_ITEMLIST )
		{
			// 遽鷓 熱薑 //pPlayerLC->Draw( pSurface );
			pNpcLC->Draw( pSurface );																				// 鼻薄縑 氈朝 嬴檜蠱擊 斜溥 遽棻.
		}

		pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

		pMyOnlineWorld->pOnlineInventory->Draw(pSurface);
	}

	if(bExit == TRUE)
	{
		pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
		pMyOnlineWorld->pOnlineInventory->SetInStruct( 0 );
		bExit = FALSE;
		Free();
		return TRUE;
	}
	else
		return FALSE;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶 鼻鷓蒂 羹觼п憮 嬴檜蠱曖 剪楚 ж朝 匙擊 憲嬴 魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMarket::MoveItem()
{	
	SI16		i, siTempItemNum;	
	SI16		siIndex;																// ⑦Щ辨
	POINT		ptMouse;

	BOOL		bOk			=	FALSE;

	if(!(pMyOnlineWorld->pOnlineMessenger->IsAction() && !pMyOnlineWorld->pOnlineMessenger->IsMini()))
	{
		if( IpD.LeftPressSwitch )		m_bLMouseDown	=	TRUE;
		else							m_bLMouseDown	=	FALSE;
	}
	
	const ItemInfo	*pItemSlot	=	NULL;
	CItemHeader		*pItem		=	NULL;

	if(siDragItem == -1)
	{
		if( ( siDragItem = pPlayerLC->GetDragSlot() ) != -1 )			siSell	=	ON_ITEM_SELL;	// 嬴檜蠱擊 て
		else if( ( siDragItem = pNpcLC->GetDragSlot() ) != -1 )			
		{
			pItemSlot = pNpcLC->GetSlotInfo( siDragItem );
			if(pItemSlot->ItemCount)
				siSell	=	ON_ITEM_BUY;	// 嬴檜蠱擊 鳴
			else
				pNpcLC->StopDrag();
		}
	}

	pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );

	if(siDragItem == -1)
	{
		if((siIndex = pPlayerLC->GetSelectSlot()) != -1)
		{
			pItemSlot	=	pPlayerLC->GetSlotInfo(siIndex);
			if(pItemSlot)
			{
				if(pItemSlot->uiItemID)
				{
					pItem		=	pMyOnlineWorld->pItemsInfo->GetItem(pItemSlot->uiItemID);
					if(pItem)
					{	
						BOOL bSell = FALSE;
						
						if( IpD.LeftPressSwitch && pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus( VK_CONTROL ) )
							pMyOnlineWorld->pOnlineInventory->GetItemList()->SetAction(ON_ITEMLIST_TYPE_VILLAGELIST, 0, pItemSlot->uiItemID);
						
						for( SI16 i=0 ; i<siRecvItemNum; i++ )
						{
							if(pNpcLC->GetSlotInfo(i))
							{
								if( pNpcLC->GetSlotInfo(i)->uiItemID == pItem->m_uiID )
								{
									pMyOnlineWorld->pOnlineHelp->SetTextItem(pItem, const_cast<ItemInfo *>(pItemSlot), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_DOWN, pItemSlot->uiBuyPrice, pItemSlot->uiSellPrice);
									bSell = TRUE;
									break;
								}
							}
						}
						
						if( !bSell )	pMyOnlineWorld->pOnlineHelp->SetTextNoSell(pItem, const_cast<ItemInfo *>(pItemSlot), (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y, ON_HELP_DOWN);
						
					}
				}
			}
		}
		else if((siIndex = pNpcLC->GetSelectSlot()) != -1)
		{
			pItemSlot	=	pNpcLC->GetSlotInfo(siIndex);
			if(pItemSlot)
			{
				if(pItemSlot->uiItemID)
				{
					pItem		=	pMyOnlineWorld->pItemsInfo->GetItem(pItemSlot->uiItemID);
					if(pItem)
					{
						if(pItemSlot->ItemCount == 0)
							pMyOnlineWorld->pOnlineHelp->SetTextItem(pItem, const_cast<ItemInfo *>(pItemSlot), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_DOWN, 2, TRUE);
						else  
							pMyOnlineWorld->pOnlineHelp->SetTextItem(pItem, const_cast<ItemInfo *>(pItemSlot), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_DOWN, TRUE, TRUE);
					}

					if( IpD.LeftPressSwitch && pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus( VK_CONTROL ) )
						pMyOnlineWorld->pOnlineInventory->GetItemList()->SetAction(ON_ITEMLIST_TYPE_VILLAGELIST, 0, pItemSlot->uiItemID);
				}
			}
		}
	}

	if( (siMouseStatus == 1) && (siDragItem == -1))
		siMouseStatus = 0;

	//===========================================================================================================
	if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_MENU)))		bTotSelect = TRUE;

	if(siStatus != ON_MARKET_STATUS_RESPONSEITEMMOVE)
	{	
		DWORD CurTime = timeGetTime();	
		if( siMouseStatus == 0 && (pMyOnlineWorld->fVillageLeftOneSwitch == 1) && !pMyOnlineWorld->pOnlineMsgBox->IsActive())// 葆辦蝶 豭薹 幗が 援葷
		{
			// 嬴檜蠱 薑爾璽檜 翮溥 氈嗑賊 嬴檜蠱 萄楚斜陛 碳陛棟 ж棻
			if(pMyOnlineWorld->pOnlineInventory->GetItemList()->IsAction() == FALSE)
			{
				if(!pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow() && 	!(pMyOnlineWorld->pOnlineMessenger->IsAction() && !pMyOnlineWorld->pOnlineMessenger->IsMini()))
				{
					if( pPlayerLC->DragUp() )												siMouseStatus = 1;			// Ы溯檜橫(て)
					if( pNpcLC->DragUp() )													siMouseStatus = 1;			// NPC(鳴)		
				}

				if(pMyOnlineWorld->pOnlineInventory->IsWearDrag())
				{
					pPlayerLC->StopDrag();
					siMouseStatus = 0;
				}
				
				dwDelayMouse1	=	timeGetTime();
			}
		}
		else if( siMouseStatus == 1 && (pMyOnlineWorld->fVillageLeftOneSwitch == 1) || bTotSelect)
		{
			if( ( CurTime - dwDelayMouse1 ) < 100 )		return;

			if( ( siStatus == ON_MARKET_STATUS_ITEMBUY_OK ) || ( siStatus == ON_MARKET_STATUS_ITEMBUY_WAIT ) ||
				( siStatus == ON_MARKET_STATUS_ITEMSELL_OK ) || ( siStatus == ON_MARKET_STATUS_ITEMSELL_WAIT ) )
				return;

			switch( siSell )
			{
			case ON_ITEM_SELL:			
				if( siDragItem != -1 )																		// ⑷營 嬴檣蠱 氈棻朝匙
				{	
					POINT ptMouse;
					pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );
					
					if( (PtInRect( &m_rtNpcRect, ptMouse ) == TRUE) && (pMyOnlineWorld->fVillageLeftOneSwitch == 1) || bTotSelect)	// 撩奢ж賊 嬴檜蠱擊 っ棻.
					{	
						uiCurFollower = pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
						pItemSlot	  = pPlayerLC->GetSlotInfo( siDragItem );

						if(!pItemSlot)	return;

						uiID		  =	pItemSlot->uiItemID;												// つ熱 氈朝 嬴檜蠱 嬴檜蛤蒂 橢朝棻.

						// robypark 2004/12/6 13:29
						// 奢撩瞪 模餵掏 嬴檜蠱擎 鼻薄縑 つ 熱 橈棻.
						// 摹鷗и 奢撩瞪 模餵掏 嬴檜蠱 薑爾 ④渦 橢晦
						CItemHeader *pItemHeader = pMyOnlineWorld->pItemsInfo->GetItem(pItemSlot->uiItemID);

						// 模餵掏 嬴檜蠱檜塭賊
						if (NULL != pItemHeader->m_pImperium)
							return;

						for( i=0 ; i<ON_VILLAGE_ITEM_LISE_MAX ; i++)										// 檜 葆擊縑憮 つ熱 氈朝 嬴檜蠱檣陛蒂 憲嬴 獄棻.					
							if( uiItemID[ i ] == uiID )			bOk	=	TRUE;								// 偽擎 嬴檜蛤陛 氈棻賊 だ熱陛 氈棻
						
						if( bOk )
						{						
							// つ熱 氈朝 嬴檜蠱朝 
							siStatus		=	ON_MARKET_STATUS_ITEMSELL_WAIT;
							siSell			=	0;
							siMouseStatus	=	0;
							TempptPosition	=	ptMouse;

							// 識 嬴檜蠱 熱榆擊 掘и棻.
							SI16 siSlot		=	pPlayerLC->GetSlot(uiID);
							siTempItemNum	=	pPlayerLC->GetSlotInfo(siSlot)->ItemCount;
							if(siTempItemNum > ON_MARKET_MAXQUANTITY)		siTempItemNum = ON_MARKET_MAXQUANTITY;

							pMyOnlineWorld->m_pMercenary->NoAction( TRUE );
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siTempItemNum);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
							if(bTotSelect)
							{
								pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetNumber(siTempItemNum);
								bTotSelect = FALSE;
							}
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( (SI16)ptMouse.x, (SI16)ptMouse.y );

							// 剪楚 衛濛衛 渠餌
							NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_SELL;
						}						
						else
						{
							siStatus		=	ON_MARKET_STATUS_ITEMSELL_WAIT;
							siSell			=	0;
							siMouseStatus	=	0;
							TempptPosition	=	ptMouse;

							// 識 嬴檜蠱 熱榆擊 掘и棻.
							SI16 siSlot		=	pPlayerLC->GetSlot(uiID);
							siTempItemNum	=	pPlayerLC->GetSlotInfo(siSlot)->ItemCount;
							if(siTempItemNum > ON_MARKET_MAXQUANTITY)		siTempItemNum = ON_MARKET_MAXQUANTITY;

							pMyOnlineWorld->m_pMercenary->NoAction( TRUE );
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siTempItemNum);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
							if(bTotSelect)
							{
								pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetNumber(siTempItemNum);
								bTotSelect = FALSE;
							}
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( (SI16)ptMouse.x, (SI16)ptMouse.y );
							
							// 剪楚 衛濛衛 渠餌
							NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_SELL2;
						}
					}				
					else
					{
						//pMyOnlineWorld->pOnlineInventory->Action();	

						if(pPlayerLC->GetDragSlot() == -1)
						{
							siDragItem = -1;
						}

						siMouseStatus	= 0;
					}	
				}
				
				siSell = 0;
				break;	
		
			case ON_ITEM_BUY:																				// 嬴檜蠱擊 骯棻.
				if( siDragItem != -1 )
				{
					POINT ptMouse;
					pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );

					if( (PtInRect( &m_rtNpcRect, ptMouse ) == TRUE) && !bTotSelect)
					{
						pNpcLC->StopDrag();
						siDragItem		=	-1;
						siMouseStatus	=	0;
						break;
					}

					pItemSlot = pNpcLC->GetSlotInfo( siDragItem );

					if(!pItemSlot)		
					{
						if(bTotSelect)	bTotSelect = FALSE;
						pNpcLC->StopDrag();
						siDragItem		=	-1;
						siMouseStatus	=	0;
						break;
					}

					if( pItemSlot != NULL )		uiID	 =	pItemSlot->uiItemID;
					if( uiID != 0 )				pItem	 =  pMyOnlineWorld->pItemsInfo->GetItem( uiID );

					if( pNpcLC->DragDown() != -1 )
					{
						bOk				= FALSE;
						siMouseStatus	= 0;
						siStatus		= ON_MARKET_STATUS_DELAY;
						break;
					}
					else if((PtInRect( &m_rtPlayerRect, ptMouse ) == TRUE) && m_bLMouseDown || bTotSelect)
					{
						for( i=0 ; i<ON_VILLAGE_ITEM_LISE_MAX ; i++)											// 檜 葆擊縑憮 つ熱 氈朝 嬴檜蠱檣陛蒂 憲嬴 獄棻.
							if( uiItemID[ i ] == uiID )			bOk	=	TRUE;
						
						
						if( bOk == FALSE )
						{						
							siSell			=	0;
							siDragItem		=	-1;
							siMouseStatus	=	0;
							return;
						}
						
						// 盪濰й 濠葬陛 氈朝雖 匐餌
						if((pPlayerLC->GetEmptySlot(uiID) == -1))
						{	
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORZONE));
							siSell			=	0;
							siDragItem		=	-1;
							siMouseStatus	=	0;
							return;
						}
						
						// 釭曖 褐辨紫蒂 匐餌��
						if( pItem->m_uiTradeGrade > pMyOnlineWorld->pMyData->GetTradeGrade() )							
						{
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORTRADE));
							break;
						}

						// 頂陛 雖旎 渦 菟熱 氈朝 熱榆擊 掘и棻.
						uiCurFollower = pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
						SI32 siResult = pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake(uiCurFollower, uiID);
						if(siResult <= 0)
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ERRORWEIGHT));

						// 模雖旎 羹觼п憮 熱榆 掘ж晦
						MONEY	BuyMoney = siResult * pItemSlot->uiBuyPrice;
						if(pMyOnlineWorld->pMyData->GetMoney() < BuyMoney)
						{
							siResult = pMyOnlineWorld->pMyData->GetMoney() / pItemSlot->uiBuyPrice;
							if(pMyOnlineWorld->pMyData->GetMoney() < pItemSlot->uiBuyPrice)
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ERRORMONEY));
						}

						if( siResult > 0 )
						{
							if(siResult > ON_MARKET_MAXQUANTITY)	siResult = ON_MARKET_MAXQUANTITY;

							pMyOnlineWorld->m_pMercenary->NoAction( TRUE );
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siResult);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
							if(bTotSelect)
							{
								pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetNumber(siResult);
								bTotSelect = FALSE;
							}
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(  (SI16)ptMouse.x, (SI16)ptMouse.y );							
							siStatus		=	ON_MARKET_STATUS_ITEMBUY_WAIT;										// 熱榆擊 僖朝棻.
							siSell			=	0;
							siMouseStatus	=	0;
							siTempDrag		=	siDragItem;
							siDragItem		=	-1;

							TempptPosition = ptMouse;

							// 剪楚 衛濛衛 渠餌
							NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_BUY;
							break;
						}
						else
						{
							NowTradesmanTalk	=	ON_TEXT_ITEMHELP_OVER_WEIGHT;
							siDragItem			=	-1;
							siMouseStatus		=	0;
						}
						break;
					}
					else
					{
						siDragItem		=	-1;
						siMouseStatus	=	0;
						break;
					}
				}

				siSell = 0;
				break;
			default:
				{
					siDragItem = -1;
					siMouseStatus	= 0;
				}
			}
		}
		else
		{
			if(pPlayerLC->GetDragSlot() == -1 && pNpcLC->GetDragSlot() == -1)
			{
				siDragItem = -1;
				siMouseStatus	= 0;
			}
		}
	}
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 等檜顫諦 釭曖 等檜顫蒂 鳶殮 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMarket::InsertData(SI32 siItemNum, OnlineReceivedItemInfo *pRecvItemInfo )
{
	SI16			i;
	CItemHeader		*pItem;	

	strItemPriceInMarket				PriceInMarket;

	// 憮幗縑憮 嫡擎 嬴檜蠱 璋濠
	siRecvItemNum	=	siItemNum;

	// 鼻薄縑憮 だ朝 僭勒擊 鳶殮и棻.
	for(i = 0; i < siItemNum; i++)
	{
		pItem		=	pMyOnlineWorld->pItemsInfo->GetItem( pRecvItemInfo[i].uiID );

		pMyOnlineWorld->pIProfit->GetItemPrice( pRecvItemInfo[i].BasePrice, pRecvItemInfo[i].uiQuantity, &PriceInMarket );			

		
		switch( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation )
		{
		case ON_RET_GLD_WAR:	// robypark 2004/12/22 17:35 螃棺 檜 葆擊 模樓 鼻欽婁 奢撩瞪ж朝 陳檣 唳辦
		case ON_RET_GLD_ENEMY:
			{
				ItemInfo TempItemInfo;
				TempItemInfo.uiItemID		= pItem->m_uiID;
				TempItemInfo.ItemCount		= pRecvItemInfo[i].uiQuantity;
				TempItemInfo.uiBuyPrice		= PriceInMarket.EnemyGuildBuy;
				TempItemInfo.uiSellPrice	= PriceInMarket.EnemyGuildSell;
				TempItemInfo.siColor		= pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade ? LIST_COLOR_PINK : LIST_COLOR_NOT;
				SI16 siSlot					= pNpcLC->GetEmptySlot(pItem->m_uiID);
				
				pNpcLC->AddItem(TempItemInfo, siSlot);
				AddPlayerPrice( pItem->m_uiID, PriceInMarket.EnemyGuildSell );
			}
			break;
			
		case ON_RET_GLD_NEUTRAL:
			{
				ItemInfo TempItemInfo;
				TempItemInfo.uiItemID		= pItem->m_uiID;
				TempItemInfo.ItemCount		= pRecvItemInfo[i].uiQuantity;
				TempItemInfo.uiBuyPrice		= PriceInMarket.Buy;
				TempItemInfo.uiSellPrice	= PriceInMarket.Sell;
				TempItemInfo.siColor		= pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade ? LIST_COLOR_PINK : LIST_COLOR_NOT;
				SI16 siSlot					= pNpcLC->GetEmptySlot(pItem->m_uiID);
				
				pNpcLC->AddItem(TempItemInfo, siSlot);
				AddPlayerPrice( pItem->m_uiID, PriceInMarket.Sell );
			}
			break;
			
		case ON_RET_GLD_MINE:
			{
				ItemInfo TempItemInfo;
				TempItemInfo.uiItemID		= pItem->m_uiID;
				TempItemInfo.ItemCount		= pRecvItemInfo[i].uiQuantity;
				TempItemInfo.uiBuyPrice		= PriceInMarket.Buy;
				TempItemInfo.uiSellPrice	= PriceInMarket.Sell;
				TempItemInfo.siColor		= pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade ? LIST_COLOR_PINK : LIST_COLOR_NOT;
				SI16 siSlot					= pNpcLC->GetEmptySlot(pItem->m_uiID);
				
				pNpcLC->AddItem(TempItemInfo, siSlot);
				AddPlayerPrice( pItem->m_uiID, PriceInMarket.Sell );
			}
			break;
		}
		
		uiItemID[i] =	pItem->m_uiID;																	// 葆擊縑憮 つ熱 氈朝 嬴檜蠱擊 蹺陛п 遽棻.

	}
}

VOID	OnlineVillageStructureMarket::AddPlayerPrice( UI32 uiItemID, MONEY Money )
{
	const SI16 siSlot = pPlayerLC->GetEmptySlot(uiItemID);

	if( siSlot == -1 )	return;

	ItemInfo TempItemInfo;
	pPlayerLC->CopyData(siSlot, TempItemInfo);
	TempItemInfo.uiSellPrice = Money;
	
	pPlayerLC->AddItem(TempItemInfo, siSlot);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Item 陛問 熱薑衛
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMarket::ModifyData(OnChangeItemInfoMsg *pChangedItemInfo)
{
	SI16			i;
	SI16			TempCount;


	strItemPriceInMarket				PriceInMarket;


	for(i = 0; i < pNpcLC->GetItemNum(); i++)
	{
		if(!pNpcLC->GetSlotInfo(i))	continue;

		if(pNpcLC->GetSlotInfo(i)->uiItemID)
		{
			for(TempCount = 0; TempCount <siRecvItemNum ; TempCount++)
			{
				if(pChangedItemInfo->ItemInfo.uiID == pNpcLC->GetSlotInfo(i)->uiItemID)
				{
					pMyOnlineWorld->pIProfit->GetItemPrice( pChangedItemInfo->ItemInfo.BasePrice, pChangedItemInfo->ItemInfo.uiQuantity, &PriceInMarket );

					// 嬴檜蠱 偃熱 滲唳
					ItemInfo TempItemInfo;
					pNpcLC->CopyData(i, TempItemInfo);
					TempItemInfo.ItemCount = pChangedItemInfo->ItemInfo.uiQuantity;
				
					pNpcLC->AddItem(TempItemInfo, i);

					switch( pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.dwGuildRelation )
					{
					// robypark 2004/12/22 17:34
					// 奢撩瞪ж朝 陳曖 葆擊 檣唳辦紫
					case ON_RET_GLD_WAR :
					case ON_RET_GLD_ENEMY:
						{
							ItemInfo TempItemInfo;
							pNpcLC->CopyData(i, TempItemInfo);
							TempItemInfo.uiSellPrice	= PriceInMarket.EnemyGuildSell;
							TempItemInfo.uiBuyPrice		= PriceInMarket.EnemyGuildBuy;
							
							pNpcLC->AddItem(TempItemInfo, i);
						}
						break;
						
					case ON_RET_GLD_NEUTRAL:
						{
							ItemInfo TempItemInfo;
							pNpcLC->CopyData(i, TempItemInfo);
							TempItemInfo.uiSellPrice	= PriceInMarket.Sell;
							TempItemInfo.uiBuyPrice		= PriceInMarket.Buy;
							
							pNpcLC->AddItem(TempItemInfo, i);
						}
						break;
						
					case ON_RET_GLD_MINE:
						{
							ItemInfo TempItemInfo;
							pNpcLC->CopyData(i, TempItemInfo);
							TempItemInfo.uiSellPrice	= PriceInMarket.Sell;
							TempItemInfo.uiBuyPrice		= PriceInMarket.Buy;
							
							pNpcLC->AddItem(TempItemInfo, i);
						}
						break;
					}
				}
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	檜 葆擊縐憮 鏃晝ж朝 賅萇 嬴檜蠱曖 薑爾蒂 だ橾縑 盪濰擊 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureMarket::SaveInfo()
{
	SAVEFILE_DATAHEADER		Header;
	SOnlineTime				Time;
	const ItemInfo			*pItemSlot;
	UI16					*pCount;
	SI16					*pPos, m_siYetCount, siNewCount;
	SI32					i, *pID;
	

	Time					=	pMyOnlineWorld->pTimer->GetNowTime();
	Header.m_siItemNum		=	pNpcLC->GetItemNum();
	Header.m_Date.m_siYear	=   Time.Year;
	Header.m_Date.m_siMonth	=	Time.Month;
	Header.m_Date.m_siDay	=	Time.Day;
	Header.m_Date.m_siHour	=	Time.Hour;


	pID					=	new SI32[ Header.m_siItemNum ];
	pCount				=	new UI16[ Header.m_siItemNum ];
	pPos				=	new SI16[ Header.m_siItemNum ];
	m_siYetCount			=	pMyOnlineWorld->pOnlineVillageSave->GetTotal();
	

	for(i = 0; i < Header.m_siItemNum; i++)
	{
		if(pNpcLC->GetSlotInfo(i))
		{
			pID[i]		=	pNpcLC->GetSlotInfo(i)->uiItemID;
			pCount[i]	=	pNpcLC->GetSlotInfo(i)->ItemCount;
			pPos[i]		=	i;
		}
		else
		{
			pID[i]		=	-1;
			pCount[i]	=	0;
			pPos[i]		=	i;
		}
	}

	for( i=0 ; i<Header.m_siItemNum ; i++ )
	{
		SI16	siSlot	=	pNpcLC->GetSlot(pID[i]);
		pItemSlot		=	pNpcLC->GetSlotInfo(siSlot);
		if(pItemSlot)
		{
			Header.m_pData[i].m_siItemQuantity	=	pCount[i];
			Header.m_pData[i].m_uiItemID		=	pID[i];
			Header.m_pData[i].m_uiBuyValue		=	(UI32)pItemSlot->uiBuyPrice;
			Header.m_pData[i].m_uiSellValue		=	(UI32)pItemSlot->uiSellPrice;
		}
		else
		{
			Header.m_pData[i].m_siItemQuantity	=	pCount[i];
			Header.m_pData[i].m_uiItemID		=	pID[i];
			Header.m_pData[i].m_uiBuyValue		=	0;
			Header.m_pData[i].m_uiSellValue		=	0;
		}
	}
	

	pMyOnlineWorld->pOnlineVillageSave->SaveInfo((UI16)OnlineVillageBaseStructure::uiVillageCode, &Header);

	// 葆擊 熱蒂 匐餌 и棻.	
	siNewCount			=	pMyOnlineWorld->pOnlineVillageSave->GetTotal();


	if( !pMyOnlineWorld->IsFindNow() )
		pMyOnlineWorld->FindTrade();	



	pMyOnlineWorld->m_pMercenary->UnLock();
	pMyOnlineWorld->m_pMercenary->NoAction( FALSE );


	delete [] pID;
	delete [] pCount;
	delete [] pPos;
}