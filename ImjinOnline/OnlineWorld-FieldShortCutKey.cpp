// ����� : �� �� ��
// ����Ű�� ó�� �Ѵ�.
#include <GSL.h>
#include	<Mouse.h>

#include <OnlineWorld.h>
#include <capture.h>
#include <OnlinenumberControl.h>
#include <OnlineMiniMap.h>
#include <clgame.h>

#include "OnlineTradeBook.h"
#include "OnlineTip.h"
#include "OnlineKeyboard.h"
#include "OnlineFieldChat.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineImoticon.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"
#include "OnlineFieldEffect.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlineSetting.h"
#include "OnlineTrade.h"
#include "OnlineMessenger.h"
#include "OnlineVillage.h"
#include "OnlineFieldAuction.h"
#include "music.h"
#include "OnlineCharUI.h"
#include "OnlineParty.h"
#include "OnlineQuestInterface.h"
#include "OnlineMercenary.h"
//�ؼ����� �߰��κ� 
//#include "JSWnd.h"
//#include "InvenList.h"

//#include "JSWndMgr.h"
//#include "JSTestWnd.h"

#include	"SiegeWarfareUnitUI.h"

extern _clCapture			Capture;							// ȭ�� ĸ�� ���� Ŭ���� 
extern _InputDevice				IpD;
extern _clGame* pGame;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʵ� �󿡼��� ����Ű�� ó���Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FieldShortCutKey()
{
	// ��Ű� ���������� ����Ű�� ��������
	if (pOnlineFieldAuction->IsEntered() || pOnlineReception->IsActive())
	{
		return;
	}

	if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_HOME)))
		pOnlinePannel->m_bHomeKey = TRUE;

	if( Handler_OnlineEditControlEx::IsFocus(pOnlinePannel->GetHOECChat()) == FALSE && !ShortKey )
	{
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0xBF)))
		{
			OnlineEditControlEx	*pOecEx;
			pOecEx = Handler_OnlineEditControlEx::GetControl(pOnlinePannel->GetHOECChat());

			Handler_OnlineEditControlEx::SetFocus( pOnlinePannel->GetHOECChat());
			pOecEx->SetString("/");
			// actdoll (2004/08/05 14:47) : �������� ���� ��� �������� �����ϰ� �Ѵ�.
//			pOecEx->GetIMEBase()->SetIMEStatus( false );
//			pOecEx->GetIMEBase()->SetInputStatus( 0 ); 

			//pOnlinePannel->pOnlineChatEditControl->SetFocus();
			//pOnlinePannel->pOnlineChatEditControl->SetString("/");

		}

		// �ŷ� ���/���� X
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x58)))
		{
			if(pOnlineTrade->m_bPermitTrade)			
				pOnlinePannel->TradeChange(TRUE);
			else
				pOnlinePannel->TradeChange(FALSE);
		}

		// ���� ��ε����� ���� �ٷΰ��� R
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x52)))
		{
			if(pOnlineMiniMap->IsActive())
			{
				if(pOnlineMiniMap->ISFullMode())
					pOnlineMiniMap->SetActive(FALSE);
				else
					pOnlineMiniMap->ChangeMode(TRUE);
			}
			else
			{
				pOnlineMiniMap->ChangeMode(TRUE);
				pOnlineMiniMap->SetActive(TRUE);
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_INVENTORY))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x49)) && !pOnlineTrade->IsActive() )//&& //siGamePollStatus == ON_GAMEPOLL_FIELD)					// �κ��丮
			{
				//if(pOnlineMiniMap->IsActive()) 
				//	pOnlineMiniMap->SetActive(FALSE);
				if(pOnlineTip->IsAction())     
					pOnlineTip->SetAction(FALSE);

				if(pOnlineInventory->IsActive())			
					pOnlineInventory->SetActive(FALSE);
				else										
					pOnlineInventory->SetActive(TRUE);
			}
		}

		if (FALSE == pMyData->IsInVillageDefenceInterface())	// ���� �������̽� �󿡼��� �뺴â ���� �Ұ���
		{			
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x46)))						// �뺴 F
			{
				m_pMercenary->OnButtonPush();
			}
		} 

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_TRADEBOOK))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x45)))					// ���(ģ�����) E
			{
				if(pOnlineTip->IsAction())
					pOnlineTip->SetAction(FALSE);
				if(pOnlineTradeBook->IsAction())			
				{
					if( pOnlineTradeBook->GetStatus() == ON_BOOK_FRIEND )
						pOnlineTradeBook->SetAction(FALSE);
					else
						pOnlineTradeBook->SetStatus( ON_BOOK_FRIEND );
				}					
				else										
					pOnlineTradeBook->SetAction(TRUE, ON_BOOK_FRIEND );
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_TRADEBOOK))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x57)))					// ��� B
			{
				if(pOnlineTip->IsAction())
					pOnlineTip->SetAction(FALSE);
				if(pOnlineTradeBook->IsAction())			
				{
					if( pOnlineTradeBook->GetStatus() == ON_BOOK_TOTAL_MONEY )
						pOnlineTradeBook->SetAction(FALSE);
					else
						pOnlineTradeBook->SetStatus( ON_BOOK_TOTAL_MONEY );
				}					
				else										
					pOnlineTradeBook->SetAction(TRUE, ON_BOOK_TOTAL_MONEY );
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_TRADEBOOK))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x51)))					// Quest Q			
				m_pQuestInterface->OnButtonPush();
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_TRADEBOOK))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x53)))					// ��� T
			{
				if(pOnlineTip->IsAction())
					pOnlineTip->SetAction(FALSE);
				if(pOnlineTradeBook->IsAction())
				{
					if( pOnlineTradeBook->GetStatus() == ON_BOOK_STATUS )
						pOnlineTradeBook->SetAction(FALSE);
					else
						pOnlineTradeBook->SetStatus( ON_BOOK_STATUS );
				}
				else										
					pOnlineTradeBook->SetAction(TRUE, ON_BOOK_STATUS );
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_TRADEBOOK))
		{
			/*
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x55)))					// ������ ����â ���(u)
			{
				if(pOnlineTip->IsAction())
					pOnlineTip->SetAction(FALSE);
				if(pOnlineTradeBook->IsAction())
				{
					if( pOnlineTradeBook->GetStatus() == ON_BOOK_INVESTMENT )
						pOnlineTradeBook->SetAction(FALSE);
					else
						pOnlineTradeBook->SetStatus( ON_BOOK_INVESTMENT );
				}
				else										
					pOnlineTradeBook->SetAction(TRUE, ON_BOOK_INVESTMENT );
			}
			*/
		}

		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x50)))						// ��Ƽ P
		{
			/*
			if(pOnlineParty->GetPartyCharBaseInfo() == TRUE)
			{
				pOnlineParty->SetPartyCharInfo(FALSE);
			}
			else if(pOnlineParty->GetPartyCharBaseInfo() == FALSE)
			{
				pOnlineParty->SetPartyCharInfo(TRUE);
			}*/
		}

		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F1)))					// Tip
		{
			// tip
			if( pOnlineTradeBook->IsAction() )		
				pOnlineTradeBook->SetAction( FALSE );
			if( pOnlineMiniMap->IsActive() )		
				pOnlineMiniMap->SetActive( FALSE );
			if(pOnlineTip->IsAction()) {
				pOnlineTip->SetAction(FALSE);
			}
			else if(pOnlineSetting->IsAction())
			{
				pOnlineSetting->SetAction(FALSE);
				pOnlineTip->SetAction(TRUE);
			}
			else if(pOnlineInventory->IsActive() && !pOnlineBooth->IsActionIn())
			{
				pOnlineInventory->SetActive( FALSE );
				pOnlineTip->SetAction(TRUE);
			}
			else if(pOnlineInventory->IsActive() && pOnlineBooth->IsActionIn())
			{
				pOnlineInventory->SetActive( TRUE );
				pOnlineTip->SetAction(FALSE);
			}
			else
			{
				pOnlineTip->SetAction(TRUE);
			}
		}

		if(pOnlineKeyboard->GetKeyStatus(VK_CONTROL) && LOBYTE(pOnlineKeyboard->GetKeyStatus(0x59)))					// ���λ���
		{
			if( !pOnlineCharUI->IsAction() )	return;

			// ���λ��� ����
			// Skill(�������� ��ų�� �ִ��� �˻�)
			if(!pMyData->IsGotoPortal() && !pMyData->IsWaitGotoPortalResponse())
			{
				if(pOnlineMap->m_MapIndex != 0 && pOnlineMap->m_MapIndex != 34 && 
					pOnlineMap->m_MapIndex != 35 && pOnlineMap->m_MapIndex != 36)
				{
					if(pMyData->GetTradeGrade() >= 20)
					{
						if(!pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()))
						{
							if(pOnlineTradeBook->IsAction())			
								pOnlineTradeBook->SetAction(FALSE);
							if(pOnlineTip->IsAction())
								pOnlineTip->SetAction(FALSE);
							if(pOnlineMiniMap->IsActive())
								pOnlineMiniMap->SetActive(FALSE);
							if(!pOnlineBooth->IsActionIn())
							{
								pOnlineBooth->SetActionIn(TRUE);
								pOnlineBooth->SetActionOut(FALSE);
								pOnlineBooth->SetActionFieldBooth(TRUE);
								bCharMove = FALSE;
							}
						}
					}
					else
						pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), pOnlineText->Get(ON_TEXT_BOOTHINFO_TRADELIMIT));
				}
				else
					pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTCRATEFIELD));
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_ATTACK))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x41)))					// ���� ��ư A
			{
				if( IsPeace() )								pOnlinePannel->ModeChange( TRUE );
				else										pOnlinePannel->ModeChange( FALSE );
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_MINIMAP))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x4D)))					// �̴ϸ� M
			{
				if(pOnlineTip->IsAction())
					pOnlineTip->SetAction(FALSE);

				if(pOnlineMiniMap->IsActive())
				{
					if(pOnlineMiniMap->ISFullMode())
						pOnlineMiniMap->SetActive(FALSE);
					else
						pOnlineMiniMap->ChangeMode(TRUE);
				}
				else
				{
					pOnlineMiniMap->ChangeMode(FALSE);
					pOnlineMiniMap->SetActive(TRUE);
				}
			}
		}
		if(pOnlineMiniMap->IsActive())
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_MULTIPLY)))		// �̴ϸʿ��� �ڽ��� ��ġ�� �߽�����
			{
				pOnlineMiniMap->MovePosition();
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_ADD)))
			{
				pOnlineMiniMap->MiniMap_AddView();
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_SUBTRACT)))
			{
				pOnlineMiniMap->MiniMap_SubTractView();
			}
		}
		
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x4E)))	// N ä��â �Ϲ�
		{
			pOnlinePannel->SetChatOpen();
			pOnlinePannel->ChangeChattingMode(CHATTINGMODE_NORMAL);
		}
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x47)))	// G ä��â ���
		{
			if(pMyData->GetGuildClass() != 0 && pMyData->GetGuildClass() != 90)
			{				
				if( pOnlinePannel->ChangeChattingMode(CHATTINGMODE_GUILD) == TRUE )
				{
					pOnlinePannel->SetChatOpen();
				}
			}
		}

		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_SPACE)))																// ���â �ݱ�
		{
			if( pOnlineInventory->IsActive() && siGamePollStatus == ON_GAMEPOLL_FIELD)		
			{
				// �ŷ��� �������̶�� Space Bar�� �������� �κ��丮 â�� ���� �ʴ´�.				
				if( pOnlineTrade->IsActive() == FALSE )
				{

					pOnlineInventory->SetActive( FALSE );
					

				}
			}

			// �ŷ��߿��� ä�� â�� ���� �ʴ´�.
			if( pOnlineTrade->IsActive() == FALSE )
			{
				pOnlinePannel->SetChatOpen(FALSE);
			}


			if( pOnlineTradeBook->IsAction() )		
				pOnlineTradeBook->SetAction( FALSE );
			if( pOnlineMiniMap->IsActive() )		
				pOnlineMiniMap->SetActive( FALSE );
			if( m_pMercenary->IsAction() )	
				m_pMercenary->OnButtonPush();
			if(pOnlineTip->IsAction())				
				pOnlineTip->SetAction(FALSE);

			
		}
		

		// actdoll (2004/01/02 20:33) : �뺴 ����Ű ���� ���� ����
		if(!m_bFollowerSelLock)	// �뺴 ����
		{
			SI32	siIndex = 0;
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 1 ) ) )		{ siIndex = 1; }		// 1 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 2 ) ) )		{ siIndex = 2; }		// 2 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 3 ) ) )		{ siIndex = 3; }		// 3 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 4 ) ) )		{ siIndex = 4; }		// 4 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 5 ) ) )		{ siIndex = 5; }		// 5 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 6 ) ) )		{ siIndex = 6; }		// 6 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 7 ) ) )		{ siIndex = 7; }		// 7 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 8 ) ) )		{ siIndex = 8; }		// 8 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 9 ) ) )		{ siIndex = 9; }		// 9 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 0 ) ) )		{ siIndex = 10; }		// 0 Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0xBD + 0 ) ) )		{ siIndex = 11; }		// - Ű
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0xBB + 0 ) ) )		{ siIndex = 12; }		// = Ű

			// �뺴 ���� Ű�� ���������ٸ�
			if( siIndex )
			{
				siIndex--;
				if( m_pMercenary->Is_ValidSlot( siIndex ) )		// �ش� �뺴â ������ ��ȿ�ϴٸ�
				{
					// ��Ÿ �۾��� ���� DB ���� ��ȣ�� �޾ƿ���
					SI32	siDBSlot	= m_pMercenary->Convert_SlotToDBSlot( siIndex );
					if( pMyData->GetFollowerParameterKind( siDBSlot ) >= 0 )
					{
						if( pOnlineInventory->IsActive() )	pOnlineInventory->DragFail();
						
						// �뺴 ������ �뺴â ���� ��ȣ�� �������� ����
						m_pMercenary->Set_SelectedSlot( siIndex, false );
					
						// �ٸ� ���������� DB ���� ��ȣ�� �������� ����
						pOnlineInventory->SetMercenarySlot( siDBSlot );
						pOnlineInventory->FillItemListBox();						
						pOnlineTradeBook->SetSelect( siDBSlot );
					}
				}
			}
		}
		
		
		// Imoticon
		if ((pOnlineKeyboard->GetKeyStatus(VK_CONTROL))
			&& (pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_IMOTICON)))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F5)))
			{
				pOnlineImoticon->SetImoticonKey(0);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F6)))
			{
				pOnlineImoticon->SetImoticonKey(1);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F7)))
			{
				pOnlineImoticon->SetImoticonKey(2);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F8)))
			{
				pOnlineImoticon->SetImoticonKey(3);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F9)))
			{
				pOnlineImoticon->SetImoticonKey(4);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F10)))
			{
				pOnlineImoticon->SetImoticonKey(5);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F11)))
			{
				pOnlineImoticon->SetImoticonKey(6);
			}
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F12)))
			{
				pOnlineImoticon->SetImoticonKey(7);
			}
		}

		// ������ �߿� ������ ������ ���� ���� ��� ����Ű ó��
		if(pOnlineKeyboard->GetKeyStatus(VK_CONTROL))
		{
			// robypark 2004/11/13 12:30
			// Ctrl + V : ������ �߿� ������ ������ ���� ���� ��� ����Ű ó��
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x56)))	// 0x56(V)
			{
				// ���� ���� ���� ���� ���̱� ��ư ��밡���� ���(���� ����),
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// ���� ���� ���� ���� ���� ���̱� ���
					if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
					{
						// ���� ���� ���� ���� ���� �Ⱥ��̱�� ����
						m_scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}
					else
					{
						// ���� ���� ���� ���� ���� ���̱�� ����
						m_scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}
				}
			}

			// robypark 2005/2/18 18:25
			// Ctrl + X: ���� ���� �������̽� minimize or restore
			if (LOBYTE(pOnlineKeyboard->GetKeyStatus(0x58)))	// 0x58(X)
			{
				// ���� �����̸�, 
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// ������ ������ ��� �������̽� ���
					CSiegeWarfareUnitUI	*pSiegeWarfareUnitUI = pOnlinePannel->GetSiegeWarfareUnitUIHandle();

					if (pSiegeWarfareUnitUI)
					{
						// ���� ���� �������̽� Ȱ��ȭ�� ���
						if (pSiegeWarfareUnitUI->IsVisibleWindow())
						{
							// �ּ�ȭ
							pSiegeWarfareUnitUI->ShowWindow(SW_MINIMIZE);
						}
						// �ּ�ȭ �� ���
						else if (pSiegeWarfareUnitUI->IsMinimizeWindow())
						{
							// ����
							pSiegeWarfareUnitUI->ShowWindow(SW_NORMAL);
						}
					}
				}
			}
		}
		
		/*
		if(m_GMFlag != ON_USERGRADE_NORMAL)
		{
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x4E)))
		{
		if( pOnlineChatEditControl->IsFocus() == FALSE)
		{
		pOnlineChatEditControl->SetString("/notice ");
		pOnlineChatEditControl->SetFocus();
		pOnlineChatEditControl->SetDelay(10);
		}
		}
		}
		*/
		
		/*
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x47)))
		{
		if( pOnlineChatEditControl->IsFocus() == FALSE)
		{
		pOnlineChatEditControl->SetString("/guild ");
		pOnlineChatEditControl->SetFocus();
		pOnlineChatEditControl->SetDelay(20);
		}
		}
		*/
		
		if( LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)) && siGamePollStatus == ON_GAMEPOLL_FIELD)
		{
			if(!pOnlineBooth->IsActionIn() && pOnlineBooth->GetStatusOut() != ON_BENCH_OUT_INVENTORY_POLL && !pOnlineTrade->IsActive())
			{
				// ���� â�� �� ������ �ݴ´�.
				if(pOnlineTip->IsAction())
					pOnlineTip->SetAction(FALSE);
				if(!pOnlineSetting->IsAction() && !pOnlineTip->IsAction())
					pOnlineSetting->SetAction(TRUE);
				else
					pOnlineSetting->SetAction(FALSE);
			}
		}
		
		if((pOnlineNumPad->pNumberControl->IsShow() || this->pOnlineMiniMap->GetfEditBox()) == FALSE)
		{
//			if( pOnlinePannel->pOnlineChatEditControl->IsEnableWindow() == FALSE )	
//				pOnlinePannel->pOnlineChatEditControl->EnableWindow( TRUE );
		}
	}
	// ä�ÿ���Ʈ���� ��Ŀ���� �������
	else
	{
		// actdoll (2004/01/26 20:37) : ** �����κ� **
		//		�Ϻ��� ����϶��� �� �κ��� �������� �ʴ´�.
		//		�� ��� ��ü�� �����Ϸ��� �������� �ƴ� �� �� ��ȣ�� ���� ���
		//		IME_Japan�̳� korea���� �Ʒ��� �Լ��� �����ǵ��� ���ش�.
//		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
//			pOnlinePannel->pOnlineChatEditControl->SetString("");
	}
	
	BOOL	bMessengerShortKey = TRUE;

	if(!pOnlineKeyboard->GetKeyStatus(VK_CONTROL) && bMessengerShortKey)
	{
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F2)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(0);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F3)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(1);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F4)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(2);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F5)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(3);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F6)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(4);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F7)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(5);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F8)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetTabStatus(6);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F9)))
		{
			if(pOnlineMessenger->IsAction())
			{
				if(pOnlineMessenger->IsMini())
					pOnlineMessenger->SetMini(FALSE);
				else
					pOnlineMessenger->SetMini(TRUE);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F10)))
		{
			if(pOnlineMessenger->IsAction())
			{
				pOnlineMessenger->SetAction(FALSE);
			}
		}
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F12)))					// ȭ�� ĸ��
		{
			Capture.ScreenCapture();
		}
	}
}
