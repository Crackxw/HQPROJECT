/************************************************************************************************
	だ橾貲 : OnlineVillageStruct-Wharf.cpp
	氬渡濠 : 檜彌戮

	葆擊 睡舒 籀葬
************************************************************************************************/
#include <GSL.h>

#include <Main.h>
#include <Mouse.h>

#include "charanimation.h"
#include <kindinfo.h>
#include <myfile.h>
#include <etc.h>
 
#include "OnlineWorld.h"
#include "OnlineVillageManager.h"
#include "OnlineMegaText.h"
#include "OnlineVillage.h"
#include "OnlineVillageStruct-Wharf.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "OnlineETC.h"
#include "OnlineTicket-Parser.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "OnlineVillage-Parser.h"
#include "OnlineMercenary.h"

extern _InputDevice				IpD;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureWharf::OnlineVillageStructureWharf() : OnlineVillageBaseStructure()
{
	m_NowWork = WHARF_NOWWORK_MAIN_START;

	// Image
	ZeroMemory(&m_Image_MainInterface_BackSelectTicket, sizeof(XSPR));
	ZeroMemory(&m_Image_CheckInterface,          sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_EM,           sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_BM,           sizeof(XSPR));
	ZeroMemory(&m_Image_Portrait,                sizeof(XSPR));
	ZeroMemory(&m_Image_MainInterface_ScrollBar, sizeof(XSPR));	
	ZeroMemory(&m_Image_CheckInterface_Line,     sizeof(XSPR));

	// Solder Data
	ZeroMemory(m_TicketList, sizeof(OnTicketInfo) * MAX_TICKET_LIST);
	m_MaxSelectTicket = 0;
	m_NowSelectTicket = -1;

	// Scroll
	m_NowScroll = 0;

	m_DiscountPercent = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureWharf::~OnlineVillageStructureWharf()
{	
	Free();
}

BOOL	OnlineVillageStructureWharf::Action()
{
	return FALSE;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureWharf::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{	
	Free(); 

	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	// Load Image
	clGrp.LoadXspr("Online\\GameSpr\\Village\\Town_Clinic.Spr", m_Image_MainInterface_BackSelectTicket);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\Small_Window_Line.Spr",m_Image_CheckInterface_Line);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW.Spr", m_Image_CheckInterface);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_Image_Button_All_EM);	//
	clGrp.LoadXspr("Online\\GameSpr\\Book\\main_window_button.Spr", m_Image_Button_All_BM);	//釭陛晦虜 餌辨.

	clGrp.LoadXspr("Online\\VillageBack\\AllImage_ScrollM.Spr", m_Image_MainInterface_ScrollBar);

	m_siX = 134;
	m_siY = 45;

	m_siCheckX = (800 - m_Image_CheckInterface.Header.Xsize)/2;
	m_siCheckY = (600 - m_Image_CheckInterface.Header.Ysize)/2;


	
	// Button
	BExit.Create(m_siX + 232, m_siY + 394,m_Image_Button_All_BM.Header.Xsize, m_Image_Button_All_BM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Buy.Create(m_siX + 132, m_siY + 358,        m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_CHARGE), BUTTON_PUT_LEFT, TRUE);
	m_Button_GoWaitRoom.Create(m_siX + 322, m_siY + 358, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_MOVEWAITROOM), BUTTON_PUT_LEFT, TRUE);
	m_Button_BuyOK.Create(m_siCheckX + 50,m_siCheckY + 148,      m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_CHARGE),BUTTON_PUT_LEFT, TRUE);
	m_Button_BuyCancel.Create(m_siCheckX + 170,m_siCheckY + 148,  m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE), BUTTON_PUT_LEFT, TRUE);


	// Initialize
	m_DiscountPercent = 0;
	m_MaxSelectTicket = 0;
	m_NowSelectTicket = -1;
	m_NowScroll       = 0;

	siKind	          = ON_VIL_ST_KIND_WHARF;

	if((pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) < 6) && (pMyOnlineWorld->pMyData->GetTradeGrade() < 6))
	{
		// 濠晦 Level檜 10 檜ж檜賊
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_LEVELCONFIDENCE));
		m_NowWork         = WHARF_NOWWORK_MAIN_LOWLEVEL;
	}
	else
	{
		m_NowWork         = WHARF_NOWWORK_MAIN_START;
		pMyOnlineWorld->pOnlineClient->SendRequestWharfEnter();
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureWharf::Free()
{
	OnlineVillageBaseStructure::Free();

	if(m_Image_MainInterface_BackSelectTicket.Image) {clGrp.FreeXspr(m_Image_MainInterface_BackSelectTicket); ZeroMemory(&m_Image_MainInterface_BackSelectTicket, sizeof(XSPR));}
	if(m_Image_CheckInterface.Image)         {clGrp.FreeXspr(m_Image_CheckInterface);		  ZeroMemory(&m_Image_CheckInterface,         sizeof(XSPR));}
	if(m_Image_CheckInterface_Line.Image)	 {clGrp.FreeXspr(m_Image_CheckInterface_Line);	  ZeroMemory(&m_Image_CheckInterface_Line,    sizeof(XSPR));} 

	if(m_Image_Button_All_EM.Image)        {clGrp.FreeXspr(m_Image_Button_All_EM);        ZeroMemory(&m_Image_Button_All_EM,        sizeof(XSPR));}
	if(m_Image_Button_All_BM.Image)        {clGrp.FreeXspr(m_Image_Button_All_BM);        ZeroMemory(&m_Image_Button_All_BM,        sizeof(XSPR));}

	if(m_Image_Portrait.Image)             {clGrp.FreeXspr(m_Image_Portrait);             ZeroMemory(&m_Image_Portrait,             sizeof(XSPR));}

	if(m_Image_MainInterface_ScrollBar.Image) {clGrp.FreeXspr(m_Image_MainInterface_ScrollBar); ZeroMemory(&m_Image_MainInterface_ScrollBar, sizeof(XSPR));}

	m_NowWork         = 0;

	m_MaxSelectTicket = 0;
	m_NowSelectTicket = -1;
	m_NowScroll       = 0;

	siKind	          = 0;

	m_DiscountPercent = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureWharf::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI32			TempCount;
	SI32			TempCount1;
	BOOL			fLiveMouse;
	BOOL			TempbMouseDown;
	SI32			TempYPos;
	float			TempFloat;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// 葆辦蝶 謝ル蒂 掘и棻.

	TempbMouseDown = bMouseDown;
	fLiveMouse     = TRUE;
	if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE)
	{
		TempbMouseDown = FALSE;
		fLiveMouse     = FALSE;
	}

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(m_NowWork)
	{
		case WHARF_NOWWORK_MAIN_START:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

//			Draw_Main(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case WHARF_NOWWORK_MAIN_STARTPOOL:
//			Draw_Main(pSurface, 0);
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			// 橾薑 衛除檜 雖釭賊 Pool 煎 檜翕
			//if(m_StartTickCount < (GetTickCount() - 450))
			//{
			m_NowWork        = WHARF_NOWWORK_MAIN_POOL;
			//	m_StartTickCount = GetTickCount();
			//}
			break;

		case WHARF_NOWWORK_MAIN_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
				// 釭陛晦
				if( ( BExit.Process(TempbMouseDown) == TRUE ) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )
				{
					pMyOnlineWorld->pOnlineClient->SendRequestWharfExit();

					m_NowWork  = WHARF_NOWWORK_MAIN_END;
					m_NextWork = WHARF_NOWWORK_EXIT_POOL;
				}

				// 渠晦褒煎 檜翕
				if(m_Button_GoWaitRoom.Process(TempbMouseDown) == TRUE)
				{
					if(siTicketKind == -1)
					{
						// ⑷營 陛霞 Ticket檜 橈戲賊
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_SHIPTICKETNO));
					}
					else if(CheckTicketUse())
					{
						// ⑷營 Ticket檜 檜 睡舒渠晦褒縑憮 噩賅橈戲賊
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_SHIPTICKETNO1));
					}
					else
					{
						// 檜翕
						m_NowWork  = WHARF_NOWWORK_MAIN_END;
						m_NextWork = WHARF_NOWWORK_WAITROOM_START;

						pMyOnlineWorld->pOnlineClient->SendRequestWharfExit();
					}
				}

				// ル 餌晦
				m_Button_Buy.SetAction(TRUE);
				if(m_NowSelectTicket == -1)                m_Button_Buy.SetAction(FALSE);
				if(m_NowSelectTicket >= m_MaxSelectTicket) m_Button_Buy.SetAction(FALSE);
				if(m_Button_Buy.Process(TempbMouseDown) == TRUE)
				{
					m_NowWork = WHARF_NOWWORK_CHECK_POOL;
				}

				// Scroll 摹鷗
				if(m_Button_ScrollUp.Process(TempbMouseDown) == TRUE)
				{
					m_NowScroll--;
					if(m_NowScroll < 0) m_NowScroll = 0;
				}
				if(m_Button_ScrollDown.Process(TempbMouseDown) == TRUE)
				{
					if(m_MaxSelectTicket > 7)
					{
						m_NowScroll++;
						if(m_NowScroll > (m_MaxSelectTicket - 6)) m_NowScroll = (m_MaxSelectTicket - 6);
					}
				}

				// Scroll Bar
				if(TempbMouseDown)
				{
					if(m_MaxSelectTicket > 7)
					{
						if((ptMouse.x >= 754) && (ptMouse.x < 776) && (ptMouse.y >= 242) && (ptMouse.y < 385))
						{
							TempYPos = ptMouse.y;
							if(TempYPos <  242) TempYPos = 242;
							if(TempYPos >= 372) TempYPos = 372;

							TempYPos -= 242;
							TempFloat   = (float)TempYPos / (float)(371 - 242);
							m_NowScroll = (SI32)((float)(m_MaxSelectTicket - 6) * TempFloat); 
						}
					}
				}

				// Ticket 摹鷗
				if(TempbMouseDown)
				{
					// Ticket List
					for(TempCount = 0; TempCount < 7; TempCount++)
					{
						TempCount1 = TempCount + m_NowScroll;
						if(TempCount1 < m_MaxSelectTicket)
						{
							if((ptMouse.x >= m_siX + 50) && (ptMouse.x < m_siX + 500) && (ptMouse.y >= ((TempCount * 20) + m_siY + 190)) && (ptMouse.y < ((TempCount * 20) + m_siY + 210)))
							{
								m_NowSelectTicket = TempCount1;
							}
						}
					}
				}
				if(IpD.IsDoubleClick()==TRUE)
				{
					// Ticket List
					for(TempCount = 0; TempCount < 7; TempCount++)
					{
						TempCount1 = TempCount + m_NowScroll;
						if(TempCount1 < m_MaxSelectTicket)
						{
							if((ptMouse.x >= m_siX + 50) && (ptMouse.x < m_siX + 500) && (ptMouse.y >= ((TempCount * 20) + m_siY + 190)) && (ptMouse.y < ((TempCount * 20) + m_siY + 210)))
							{
								m_NowSelectTicket = TempCount1;

								m_NowWork         = WHARF_NOWWORK_CHECK_POOL;
								break;
							}
						}
					}
				}
			}

//			Draw_Main(pSurface, 0);
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case WHARF_NOWWORK_CHECK_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
				if(m_Button_BuyOK.Process(TempbMouseDown) == TRUE)
				{
					// Send Message
					pMyOnlineWorld->pOnlineClient->SendRequestWharfBuyTicket(m_TicketList[m_NowSelectTicket].m_siTicketKind);

					m_NowWork = WHARF_NOWWORK_CHECK_POOL_RETURNWAIT;
				}
				if(m_Button_BuyCancel.Process(TempbMouseDown) == TRUE)
				{
					m_NowWork = WHARF_NOWWORK_MAIN_POOL;
				}
			}

//			Draw_Main(pSurface, 0);
			Draw_Select(pSurface);
			Draw_Check(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case WHARF_NOWWORK_CHECK_POOL_RETURNWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

//			Draw_Main(pSurface, 0);
			Draw_Select(pSurface);
			Draw_Check(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case WHARF_NOWWORK_MAIN_END:
			{
				// Server縑憮 �挫� Message匐餌
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
				
//				Draw_Main(pSurface, 0);
				Draw_Select(pSurface);
				pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			}
			break;

		case WHARF_NOWWORK_MAIN_ENDPOOL:
//			Draw_Main(pSurface, 0);
			Draw_Select(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			// 橾薑 衛除檜 雖釭賊 Pool 煎 檜翕
			//if(m_StartTickCount < (GetTickCount() - 450))
			//{
			m_NowWork = m_NextWork;
			if(m_NowWork == WHARF_NOWWORK_WAITROOM_START) pMyOnlineWorld->pOnlineClient->SendRequestWharfWaitRoomEnter();
			//}
			break;

		case WHARF_NOWWORK_WAITROOM_START:
			// Server縑憮 �挫� Message匐餌
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_MOVEWAITROOM), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

//			Draw_Main(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case WHARF_NOWWORK_WAITROOM_STARTPOOL:
//			Draw_Main(pSurface, 0);
			Draw_WaitRoom(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			// 橾薑 衛除檜 雖釭賊 Pool 煎 檜翕
			//if(m_StartTickCount < (GetTickCount() - 450))
			//{
				m_NowWork        = WHARF_NOWWORK_WAITROOM_POOL;
			//	m_StartTickCount = GetTickCount();
			//}
			break;

		case WHARF_NOWWORK_WAITROOM_POOL:
			if(CheckMoveInShip() == TRUE)
			{
				m_NowWork  = WHARF_NOWWORK_WAITROOM_END;
				m_NextWork = WHARF_NOWWORK_SHIP_POOL;
			}

			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() == FALSE)
			{
				// 釭陛晦
				if(BExit.Process(TempbMouseDown) == TRUE)
				{
					m_NowWork  = WHARF_NOWWORK_WAITROOM_END;
					m_NextWork = WHARF_NOWWORK_MAIN_START;

					pMyOnlineWorld->pOnlineClient->SendRequestWharfWaitRoomExit();
				}
			}

//			Draw_Main(pSurface, 0);
			Draw_WaitRoom(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case WHARF_NOWWORK_WAITROOM_END:
			{
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
				
//				Draw_Main(pSurface, 0);
				Draw_WaitRoom(pSurface);
				pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			}
			break;

		case WHARF_NOWWORK_WAITROOM_ENDPOOL:
//			Draw_Main(pSurface, 0);
			Draw_WaitRoom(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			// 橾薑 衛除檜 雖釭賊 Pool 煎 檜翕
			//if(m_StartTickCount < (GetTickCount() - 450))
			//{
				m_NowWork = m_NextWork;
				if(m_NowWork == WHARF_NOWWORK_MAIN_START) pMyOnlineWorld->pOnlineClient->SendRequestWharfEnter();
			//}
			break;

		case WHARF_NOWWORK_SHIP_POOL:
			GoShip();
			break;

		case WHARF_NOWWORK_EXIT_POOL:
			Free();
			return TRUE;
			break;

		case WHARF_NOWWORK_MAIN_LOWLEVEL:
			if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
			{
				m_NowWork = WHARF_NOWWORK_EXIT_POOL;
			}
			break;
	}

	// 辨煽
	if(pMyOnlineWorld->m_pMercenary->IsAction())
	{
		pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 檣漸饜葬.
	if(pMyOnlineWorld->pOnlineInventory->IsActive())
	{
		pMyOnlineWorld->pOnlineInventory->SetFlagVillage(TRUE);
		pMyOnlineWorld->pOnlineInventory->Action();
		pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 濰睡
	if(pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
		pMyOnlineWorld->pOnlineTradeBook->Poll();
		pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 嘐棲裘
	if(pMyOnlineWorld->pOnlineMiniMap->IsActive())
	{
		pMyOnlineWorld->pOnlineMiniMap->Pool();
		pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// Pannel擊 轎溘и棻.
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON);

	if(IpD.LeftPressSwitch) bMouseDown = TRUE;
	else				    bMouseDown = FALSE;

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw Back
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
/*BOOL	OnlineVillageStructureWharf::Draw_Main(LPDIRECTDRAWSURFACE7 pSurface, DWORD TickCount)
{
	return TRUE;
}*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw Main
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureWharf::Draw_Select(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC					hDC;
	SI16				StartPos;
	SI32				TempCount;
	SI32				TempCount1;
	BYTE				TempBuffer[256];
	BYTE				TempBuffer1[256];
	TicketHeader*		lpTicketHeader;
	VillageHeader*		pVillageHeader1;
	BYTE				fFlag;				// 0 : Normal 轎溘, 1 : 殘⑵霞 蝶觼煤, 2 : 雲橫雖朝 蝶觼煤
	SI32				TempYPos;

	// 嬪纂 啗骯
//	if(TickCount > 450) TickCount = 450;
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case WHARF_NOWWORK_MAIN_STARTPOOL:
//			TempFloat = (float)sin((90.0f - ((float)(TickCount) / 5.0f)) * 0.017444f);
//			StartPos  = -(int)(TempFloat * TempFloat * 592.0f);
			fFlag     = 1;
			break;

		case WHARF_NOWWORK_MAIN_ENDPOOL:
//			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
//			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			fFlag     = 0;
			break;

		default:
//			if((GetTickCount() - m_StartTickCount) < 600)
//			{
				fFlag = 2;
//			}
			break;
	}

//	BExit.SetX(BExit.GetX() - StartPos);
//	m_Button_Buy.SetX(m_Button_Buy.GetX() - StartPos);
//	m_Button_GoWaitRoom.SetX(m_Button_GoWaitRoom.GetX() - StartPos);

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 蝶觼煤 轎溘
		clGrp.PutSpriteT(m_siX, m_siY, m_Image_MainInterface_BackSelectTicket.Header.Xsize, m_Image_MainInterface_BackSelectTicket.Header.Ysize, m_Image_MainInterface_BackSelectTicket.Image);

		// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 45, NPC_KIND_HARVOR, pMyOnlineWorld->pOnlineVillage->GetNation() );

		// 堅辨ж晦 幗が 轎溘
		if(m_NowWork == WHARF_NOWWORK_MAIN_POOL) m_Button_Buy.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                     m_Button_Buy.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);
		if(m_NowWork == WHARF_NOWWORK_MAIN_POOL) m_Button_GoWaitRoom.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                     m_Button_GoWaitRoom.Put(&m_Image_Button_All_EM, 0, 0, 0, BUTTON_PUT_NOMOVE);

		// Scroll Bar
		TempYPos = (SI32)((float)(371 - 242) * ((float)m_NowScroll / (float)(m_MaxSelectTicket - 6))) + 242;
		clGrp.PutSpriteT(749,209,
			m_Image_MainInterface_ScrollBar.Header.Xsize, m_Image_MainInterface_ScrollBar.Header.Ysize, m_Image_MainInterface_ScrollBar.Image);
			

		// 釭陛晦 幗が 轎溘
		if(m_NowWork == WHARF_NOWWORK_MAIN_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                     BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		// 輿檣濰曖 蜓
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109, m_siY + 50, 402, (char*)pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_OWNERTEXT1));
		
		// ル衛
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_THEOTHERS));
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 40 , m_siY + 150,  (char*)TempBuffer, RGB(0, 255, 0));
		sprintf((char*)TempBuffer, "----------------------------------------------------------------------------");
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 40 , m_siY + 170, (char*)TempBuffer, RGB(0, 255, 0));

		// ⑷營 摹鷗й熱 氈朝 辨煽菟 List 轎溘
		for(TempCount = 0; TempCount < 7; TempCount++)
		{
			TempCount1 = TempCount + m_NowScroll;
			if(TempCount1 < m_MaxSelectTicket)
			{
				lpTicketHeader  = pMyOnlineWorld->pTicketParser->GetTicket(m_TicketList[TempCount1].m_siTicketKind);
				if(lpTicketHeader)
				{
					pVillageHeader1 = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpTicketHeader->uiEndVillageCode);

					MakeMoneyText(lpTicketHeader->siMoney - ((lpTicketHeader->siMoney * m_DiscountPercent) / 100), (char*)TempBuffer1);
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_THEOTHERS1),pMyOnlineWorld->pOnlineText->Get(pVillageHeader1->siVillageName)
						, lpTicketHeader->siStartTime
						, lpTicketHeader->siMoveTime
						, TempBuffer1);

					if(TempCount1 == m_NowSelectTicket)
					{
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 40 , m_siY + 190 + (TempCount * 20), (char*)TempBuffer, RGB(0, 255, 0));
					}
					else
					{
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 40 , m_siY + 190 + (TempCount * 20), (char*)TempBuffer, RGB(200, 200, 200));
					}
				}
			}
		}

/** ⑷營 濛撩醞檣 囀萄 2004-08-11 16:48 (sagolboss)
    幗斜蒂 濩擊溥堅 ж朝 濛機檜艘戲釭 濛機擊 Drop 衛鑒溥賊 雖辦賊 脾 
		for( int nTicketLoopCount = 0; nTicketLoopCount < 7; ++nTicketLoopCount)
		{
			int nSelectedCount = nTicketLoopCount + m_NowScroll;
			if(nSelectedCount < m_MaxSelectTicket)
			{
				lpTicketHeader = pMyOnlineWorld->pTicketParser->GetTicket(m_TicketList[nTicketLoopCount].m_siTicketKind);
			}
		}	
*/

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// 堅辨ж晦 幗が Text 轎溘
		m_Button_Buy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Button_GoWaitRoom.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 睡舒 旋濠 轎溘
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207, m_siY + 7,118,13,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF), RGB(200, 200, 200));

		// 釭陛晦 幗が Text 轎溘
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC(hDC);
	}

//	BExit.SetX(BExit.GetX() );
//	m_Button_Buy.SetX(m_Button_Buy.GetX());
//	m_Button_GoWaitRoom.SetX(m_Button_GoWaitRoom.GetX());

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw CheckBox
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureWharf::Draw_Check(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC					hDC;
	BYTE				TempBuffer[256];
	BYTE				TempBuffer1[256];
	TicketHeader*		lpTicketHeader;
	VillageHeader*		pVillageHeader1;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siCheckX,m_siCheckY, m_Image_CheckInterface_Line.Header.Xsize, m_Image_CheckInterface_Line.Header.Ysize, m_Image_CheckInterface_Line.Image);
		// 夥鰻 轎溘
		clGrp.PutSpriteJin(m_siCheckX,m_siCheckY, m_Image_CheckInterface.Header.Xsize, m_Image_CheckInterface.Header.Ysize, m_Image_CheckInterface.Image);

		// 餌晦 幗が 轎溘
		m_Button_BuyOK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 鏃模ж晦 幗が 轎溘
		m_Button_BuyCancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);

		// 髦溥朝 Ticket 薑爾 轎溘
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		// 髦溥朝 Ticket 薑爾 轎溘
		if(m_NowSelectTicket != -1)
		{
			lpTicketHeader  = pMyOnlineWorld->pTicketParser->GetTicket(m_TicketList[m_NowSelectTicket].m_siTicketKind);
			pVillageHeader1 = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpTicketHeader->uiEndVillageCode);
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_DESTINATION), pMyOnlineWorld->pOnlineText->Get(pVillageHeader1->siVillageName));
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 20, m_siCheckY + 50, (char*)TempBuffer, RGB(200, 200, 200));
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_STARTTIME), lpTicketHeader->siStartTime);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 20, m_siCheckY + 70, (char*)TempBuffer, RGB(200, 200, 200));
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_WAYTIME), lpTicketHeader->siMoveTime);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 20, m_siCheckY + 90, (char*)TempBuffer, RGB(200, 200, 200));
			MakeMoneyText(lpTicketHeader->siMoney - ((lpTicketHeader->siMoney * m_DiscountPercent) / 100), (char*)TempBuffer1);
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_PRICE), TempBuffer1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 20, m_siCheckY + 110, (char*)TempBuffer, RGB(200, 200, 200));
		}
			
		// ⑷營 濠褐檜 陛霞 Ticket 薑爾蒂 轎溘
		if(siTicketKind == -1)
		{
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_THISTICKERBUY));
		}
		else
		{
			lpTicketHeader  = pMyOnlineWorld->pTicketParser->GetTicket(siTicketKind);
			if(lpTicketHeader)
			{
				pVillageHeader1 = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpTicketHeader->uiEndVillageCode);
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_THISTICKERBUY1), pMyOnlineWorld->pOnlineText->Get(pVillageHeader1->siVillageName));
			}
			else
			{
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_THISTICKERBUY));
			}
		}
		//sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_STARTTIME), lpTicketHeader->siStartTime);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siCheckX + 20, m_siCheckY + 30, (char*)TempBuffer, RGB(200, 200, 200));
		//pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 263, 222,200, (char*)TempBuffer, RGB(200, 200, 200));

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// 餌晦 幗が Text 轎溘
		m_Button_BuyOK.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 鏃模ж晦 幗が Text 轎溘
		m_Button_BuyCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Draw WaitRoom
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureWharf::Draw_WaitRoom(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC					hDC;
	SI16				StartPos;
	BYTE				TempBuffer[256];
	BYTE				fFlag;				// 0 : Normal 轎溘, 1 : 殘⑵霞 蝶觼煤, 2 : 雲橫雖朝 蝶觼煤
	TicketHeader*		lpTicketHeader;
	VillageHeader*		pVillageHeader1;

	// 嬪纂 啗骯
//	if(TickCount > 450) TickCount = 450;
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case WHARF_NOWWORK_WAITROOM_STARTPOOL:
//			TempFloat = (float)sin((90.0f - ((float)(TickCount) / 5.0f)) * 0.017444f);
//			StartPos  = -(int)(TempFloat * TempFloat * 592.0f);
			fFlag     = 1;
			break;

		case WHARF_NOWWORK_WAITROOM_ENDPOOL:
//			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
//			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			fFlag     = 0;
			break;

		default:
//			if((GetTickCount() - m_StartTickCount) < 600)
//			{
				fFlag = 2;
//			}
			break;
	}

//	BExit.SetX(BExit.GetX());


	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 蝶觼煤 轎溘
		clGrp.PutSpriteT(m_siX , m_siY, m_Image_MainInterface_BackSelectTicket.Header.Xsize,m_Image_MainInterface_BackSelectTicket.Header.Ysize,m_Image_MainInterface_BackSelectTicket.Image);

		// 釭陛晦 幗が 轎溘
		if(m_NowWork == WHARF_NOWWORK_MAIN_POOL) BExit.Put(&m_Image_Button_All_BM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		else                                     BExit.Put(&m_Image_Button_All_BM, 0, 0, 0, BUTTON_PUT_NOMOVE);		

		// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 45, NPC_KIND_HARVOR, pMyOnlineWorld->pOnlineVillage->GetNation() );

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hDC, TRANSPARENT);

		SI32					TempStartTime;
		SOnlineTime				NowTime;

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		NowTime         = pMyOnlineWorld->pTimer->GetNowTime();
		lpTicketHeader  = pMyOnlineWorld->pTicketParser->GetTicket(siTicketKind);
		pVillageHeader1 = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpTicketHeader->uiEndVillageCode);

		NowTime.Hour--;
		TempStartTime = lpTicketHeader->siStartTime + ((NowTime.Hour / lpTicketHeader->siStartTime) * lpTicketHeader->siStartTime);

		TempStartTime++;
		if(TempStartTime > 24)
		{
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_NEXTSHIP), pMyOnlineWorld->pOnlineText->Get(pVillageHeader1->siVillageName), 1);
		}
		else
		{
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_NEXTSHIP1), pMyOnlineWorld->pOnlineText->Get(pVillageHeader1->siVillageName), TempStartTime);
		}
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX + 109 , m_siY + 50, 402, (char*)TempBuffer);

		// 睡舒 渠晦褒 旋濠 轎溘
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 207, m_siY + 7,118,13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHARF_WAITROOM), RGB(200, 200, 200));

		// 釭陛晦 幗が Text 轎溘
		BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC(hDC);
	}

//	BExit.SetX(BExit.GetX() );

	return TRUE;
}

BOOL	OnlineVillageStructureWharf::CheckMoveInShip(void)
{
	OnResWharfWaitRoom_MoveInShip* pOnResWharfWaitRoom_MoveInShip = (OnResWharfWaitRoom_MoveInShip*)pMyOnlineWorld->pOnlineClient->GetMsgBuffer(ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP);
	if(pOnResWharfWaitRoom_MoveInShip)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL	OnlineVillageStructureWharf::CheckTicketUse(void)
{
	TicketHeader*		lpTicketHeader;

	// ⑷營 Ticket曖 轎嫦雖陛 罹晦檣雖 匐餌
	if(siTicketKind == -1) return TRUE;

	lpTicketHeader = pMyOnlineWorld->pTicketParser->GetTicket(siTicketKind);
	if(lpTicketHeader == NULL) return TRUE;
	if(lpTicketHeader->uiStartVillageCode != pVillageHeader->uiVillageCode)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL	OnlineVillageStructureWharf::GoShip(void)
{
	return FALSE;
}
/*
void CTicket::Add(const POINT& ptPos, const char* lpszContent, const DWORD dwColor)
{
	CTicket::TICKET_COLUMN_INFO TicketColumnInfo;
	memset(&TicketColumnInfo, 0, sizeof(TicketColumnInfo));

	TicketColumnInfo.ptPos		= ptPos;
	TicketColumnInfo.strContent = lpszContent;
	TicketColumnInfo.dwColor    = dwColor;
}

void CTicket::Draw(HDC hDC)
{
	list<TICKET_COLUMN_INFO*>::itorerator itor;
	for( itor = m_ListTicketColumnInfo.begin(); itor != m_ListTicketColumnInfo.end(); ++itor)
	{
		TICKET_COLUMN_INFO* pInfo = *itor;
		assert(pInfo);

		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, pInfo->ptPos.x, pInfo->ptPos.y, 
											      pInfo->strContent.c_str(), pInfo->dwColor);		
	}
}
*/