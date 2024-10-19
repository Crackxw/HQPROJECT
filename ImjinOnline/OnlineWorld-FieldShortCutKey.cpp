// 담당자 : 이 준 석
// 단축키를 처리 한다.
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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
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
//준석이형 추가부분 
//#include "JSWnd.h"
//#include "InvenList.h"

//#include "JSWndMgr.h"
//#include "JSTestWnd.h"

#include	"SiegeWarfareUnitUI.h"

extern _clCapture			Capture;							// 화면 캡춰 관련 클래스 
extern _InputDevice				IpD;
extern _clGame* pGame;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	필드 상에서의 단축키를 처리한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FieldShortCutKey()
{
	// 경매가 떠있을때는 단축키가 먹지않음
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
			// actdoll (2004/08/05 14:47) : 슬래쉬를 누를 경우 영문으로 동작하게 한다.
//			pOecEx->GetIMEBase()->SetIMEStatus( false );
//			pOecEx->GetIMEBase()->SetInputStatus( 0 ); 

			//pOnlinePannel->pOnlineChatEditControl->SetFocus();
			//pOnlinePannel->pOnlineChatEditControl->SetString("/");

		}

		// 거래 허용/차단 X
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x58)))
		{
			if(pOnlineTrade->m_bPermitTrade)			
				pOnlinePannel->TradeChange(TRUE);
			else
				pOnlinePannel->TradeChange(FALSE);
		}

		// 마을 장부데이터 모드로 바로가기 R
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
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x49)) && !pOnlineTrade->IsActive() )//&& //siGamePollStatus == ON_GAMEPOLL_FIELD)					// 인벤토리
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

		if (FALSE == pMyData->IsInVillageDefenceInterface())	// 수성 인터페이스 상에서는 용병창 보기 불가능
		{			
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x46)))						// 용병 F
			{
				m_pMercenary->OnButtonPush();
			}
		} 

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_TRADEBOOK))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x45)))					// 장부(친구목록) E
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
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x57)))					// 장부 B
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
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x53)))					// 장부 T
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
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x55)))					// 투자자 정보창 장부(u)
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

		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x50)))						// 파티 P
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

		if(pOnlineKeyboard->GetKeyStatus(VK_CONTROL) && LOBYTE(pOnlineKeyboard->GetKeyStatus(0x59)))					// 개인상점
		{
			if( !pOnlineCharUI->IsAction() )	return;

			// 개인상점 개설
			// Skill(노점개설 스킬이 있는지 검사)
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
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x41)))					// 공격 버튼 A
			{
				if( IsPeace() )								pOnlinePannel->ModeChange( TRUE );
				else										pOnlinePannel->ModeChange( FALSE );
			}
		}

		if(pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_MINIMAP))
		{
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x4D)))					// 미니맵 M
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
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_MULTIPLY)))		// 미니맵에서 자신의 위치를 중심으로
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
		
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x4E)))	// N 채팅창 일반
		{
			pOnlinePannel->SetChatOpen();
			pOnlinePannel->ChangeChattingMode(CHATTINGMODE_NORMAL);
		}
		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x47)))	// G 채팅창 상단
		{
			if(pMyData->GetGuildClass() != 0 && pMyData->GetGuildClass() != 90)
			{				
				if( pOnlinePannel->ChangeChattingMode(CHATTINGMODE_GUILD) == TRUE )
				{
					pOnlinePannel->SetChatOpen();
				}
			}
		}

		if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_SPACE)))																// 모든창 닫기
		{
			if( pOnlineInventory->IsActive() && siGamePollStatus == ON_GAMEPOLL_FIELD)		
			{
				// 거래가 진행중이라면 Space Bar를 누르더라도 인벤토리 창을 닫지 않는다.				
				if( pOnlineTrade->IsActive() == FALSE )
				{

					pOnlineInventory->SetActive( FALSE );
					

				}
			}

			// 거래중에는 채팅 창을 닫지 않는다.
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
		

		// actdoll (2004/01/02 20:33) : 용병 단축키 슬롯 구조 변경
		if(!m_bFollowerSelLock)	// 용병 선택
		{
			SI32	siIndex = 0;
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 1 ) ) )		{ siIndex = 1; }		// 1 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 2 ) ) )		{ siIndex = 2; }		// 2 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 3 ) ) )		{ siIndex = 3; }		// 3 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 4 ) ) )		{ siIndex = 4; }		// 4 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 5 ) ) )		{ siIndex = 5; }		// 5 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 6 ) ) )		{ siIndex = 6; }		// 6 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 7 ) ) )		{ siIndex = 7; }		// 7 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 8 ) ) )		{ siIndex = 8; }		// 8 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 9 ) ) )		{ siIndex = 9; }		// 9 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0x30 + 0 ) ) )		{ siIndex = 10; }		// 0 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0xBD + 0 ) ) )		{ siIndex = 11; }		// - 키
			if( !siIndex && LOBYTE( pOnlineKeyboard->GetKeyStatus( 0xBB + 0 ) ) )		{ siIndex = 12; }		// = 키

			// 용병 선택 키가 눌려졌었다면
			if( siIndex )
			{
				siIndex--;
				if( m_pMercenary->Is_ValidSlot( siIndex ) )		// 해당 용병창 슬롯이 유효하다면
				{
					// 기타 작업을 위해 DB 슬롯 번호를 받아오고
					SI32	siDBSlot	= m_pMercenary->Convert_SlotToDBSlot( siIndex );
					if( pMyData->GetFollowerParameterKind( siDBSlot ) >= 0 )
					{
						if( pOnlineInventory->IsActive() )	pOnlineInventory->DragFail();
						
						// 용병 선택은 용병창 슬롯 번호를 기준으로 변경
						m_pMercenary->Set_SelectedSlot( siIndex, false );
					
						// 다른 정보에서는 DB 슬롯 번호를 기준으로 변경
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

		// 공성전 중에 공성전 유닛의 상태 정보 토글 단축키 처리
		if(pOnlineKeyboard->GetKeyStatus(VK_CONTROL))
		{
			// robypark 2004/11/13 12:30
			// Ctrl + V : 공성전 중에 공성전 유닛의 상태 정보 토글 단축키 처리
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(0x56)))	// 0x56(V)
			{
				// 공성 유닛 상태 정보 보이기 버튼 사용가능한 경우(공성 유닛),
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// 현재 공성 유닛 상태 정보 보이기 라면
					if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
					{
						// 현재 공성 유닛 상태 정보 안보이기로 설정
						m_scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}
					else
					{
						// 현재 공성 유닛 상태 정보 보이기로 설정
						m_scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}
				}
			}

			// robypark 2005/2/18 18:25
			// Ctrl + X: 공성 유닛 인터페이스 minimize or restore
			if (LOBYTE(pOnlineKeyboard->GetKeyStatus(0x58)))	// 0x58(X)
			{
				// 공성 유닛이며, 
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// 공성전 유닛일 경우 인터페이스 얻기
					CSiegeWarfareUnitUI	*pSiegeWarfareUnitUI = pOnlinePannel->GetSiegeWarfareUnitUIHandle();

					if (pSiegeWarfareUnitUI)
					{
						// 공성 유닛 인터페이스 활성화된 경우
						if (pSiegeWarfareUnitUI->IsVisibleWindow())
						{
							// 최소화
							pSiegeWarfareUnitUI->ShowWindow(SW_MINIMIZE);
						}
						// 최소화 된 경우
						else if (pSiegeWarfareUnitUI->IsMinimizeWindow())
						{
							// 복원
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
				// 도움말 창이 떠 있으면 닫는다.
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
	// 채팅에디트란에 포커스가 있을경우
	else
	{
		// actdoll (2004/01/26 20:37) : ** 수정부분 **
		//		일본어 모드일때는 이 부분을 실행하지 않는다.
		//		이 기능 자체를 구현하려면 조합중이 아닐 때 이 신호가 왔을 경우
		//		IME_Japan이나 korea에서 아래의 함수가 구현되도록 해준다.
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
		else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_F12)))					// 화면 캡쳐
		{
			Capture.ScreenCapture();
		}
	}
}
