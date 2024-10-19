/********************************************************************************************
	파일명	:	OnlineMessenger.h
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2002.07.06
	수정일  :	
********************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>

#include "OnlineWorld.h"
#include "OnlineTradeBook.h"
#include "OnlineMessenger.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineFieldArea.h"
#include "OnlineKeyboard.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"


extern _InputDevice		IpD;
extern _clGame*			pGame;
extern SHORT			GameStatus;


OnlineMessenger::OnlineMessenger()
{
	m_bAction			=	FALSE;
	m_bMouseDown		=	FALSE;
	m_bBlink			=	FALSE;
	m_bMini				=	FALSE;
	m_siNoField			=	-1;
	m_siStatus			=	MESSENGER_POLL;
	m_siTabStatus		=	0;
	m_bWhisperOn		=	TRUE;
	m_siDragDifX		=	0;
	m_siDragDifY		=	0;

	for(int i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
		m_bIsNewMessage[i] = FALSE;
}

OnlineMessenger::~OnlineMessenger()
{
	Free();
}

VOID	OnlineMessenger::Free()
{
	//if( m_pInputEditBox )
	//{
	//	m_pInputEditBox->Free();
	//	delete m_pInputEditBox;
	//	m_pInputEditBox = NULL;
	//}

	Handler_OnlineEditControlEx::KillControl( m_hOecEditBox);

	clGrp.FreeXspr(m_MainSpr);
	clGrp.FreeXspr(m_NameButSpr);
	clGrp.FreeXspr(m_MiniSpr);
	clGrp.FreeXspr(m_CloseSpr);
	clGrp.FreeXspr(m_ScrBarSpr);
}

VOID	OnlineMessenger::Init(cltOnlineWorld *pOnlineWorld)
{
	SI16	i;
	m_siX				=	150;
	m_siY				=	150;
	m_pMyOnlineWorld	=	pOnlineWorld;

	for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
	{
		ZeroMemory(&m_FriendInfo[i], sizeof(m_FriendInfo[i]));
	}

	clGrp.LoadXspr("Online\\GameSpr\\Messenger\\WHISPER.Spr", m_MainSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Messenger\\WHISPER_NAMETAB.Spr", m_NameButSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Messenger\\whisper_button_min.SPR", m_MiniSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Messenger\\whisper_button_close.SPR", m_CloseSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Messenger\\WHISPER_CTR_BAR.SPR", m_ScrBarSpr);

	for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
	{
		m_BTab[i].Create(m_siX, m_siY+19+18*i, m_NameButSpr.Header.Xsize, m_NameButSpr.Header.Ysize+1, "", BUTTON_PUT_LEFT, FALSE);
	}

	m_BMini.Create(m_siX + 304,	m_siY + 3, m_MiniSpr.Header.Xsize, m_MiniSpr.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );
	m_BClose.Create(m_siX + 323, m_siY + 3, m_CloseSpr.Header.Xsize, m_CloseSpr.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );

	// 채팅 목록 박스 초기화
	for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
	{
		m_ChatList[i].Init(pOnlineWorld, ON_INN_ID_CONTENT, -1, ON_GAMESPR_QUEST_SCR_HANDLE, 10, 42, m_siX+77, m_siY+26, 
			m_siX+330, m_siY+31, m_siX+330, m_siY+31, 12, 136, m_siX+77, m_siY+26, 15, 
			"Online\\Gamespr\\Book\\book_scr_Up.Spr", "Online\\Gamespr\\Book\\book_scr_Down.Spr");
	}

	// 에디트박스 영역
//	m_pInputEditBox = new OnlineEditControl;
//	m_pInputEditBox->CreateEditBox(1000, 1000, 280, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_MAX_CHAT_CONTENT );
//	SendMessage( m_pInputEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)(ON_MAX_CHAT_CONTENT-1), 0 );
//	m_pInputEditBox->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

	OnlineEditControlEx	*pOecEx;
	if( m_hOecEditBox )	Handler_OnlineEditControlEx::KillControl( m_hOecEditBox );
	RECT	rcRect  = { m_siX + 77, m_siY + 180, m_siX + 320, m_siY + 196};
	m_hOecEditBox	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, ON_MAX_CHAT_CONTENT);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecEditBox );
	pOecEx->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(200, 200, 200) );

	SetRect(&m_rtEditRect, m_siX + 77, m_siY + 179, m_siX + 320, m_siY + 196);
	SetRect(&m_rtDragRect, m_siX, m_siY, m_siX + 296, m_siY + 16);


	}

VOID	OnlineMessenger::Action()
{
	if( !m_bAction )	return;

	SI16	i;
	CHAR	szBuffer[1024];
	ZeroMemory(szBuffer, 1024);
	OnlineEditControlEx	*pOecEx;

	POINT	m_ptMouse;
	m_pMyOnlineWorld->pIOnlineMouse->GetPos( &m_ptMouse );	
	pOecEx	= Handler_OnlineEditControlEx::GetControl(m_hOecEditBox);
	
	switch(m_siStatus)
	{
	case MESSENGER_POLL:
		{
			if(!m_bMini)
			{
				// 본인대화 버퍼에 받기
				ZeroMemory(m_szSendContent, ON_MAX_CHAT_CONTENT + 1);
				strcpy(m_szSendContent,pOecEx->GetString());
				
				// 메신저 클릭
				for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
				{
					if(m_BTab[i].Process(m_bMouseDown) == TRUE)
					{
						if(m_FriendInfo[i].szFriendName[0] != 0)
						{
							m_siTabStatus = i;
							if(m_bIsNewMessage[i])	m_bIsNewMessage[i] = FALSE;
						}
					}
				}
				
				// 드래그 영역
				if((PtInRect(&m_rtDragRect, m_ptMouse) == TRUE) && (m_pMyOnlineWorld->fLeftOneSwitch == 1))
				{
					if(!m_pMyOnlineWorld->bCharMove)		break;

					m_dwTime = timeGetTime();
					
					m_pMyOnlineWorld->bCharMove = FALSE;
					m_siDragDifX = IpD.Mouse_X - m_siX;
					m_siDragDifY = IpD.Mouse_Y - m_siY;
					m_siStatus	= MESSENGER_DRAGING;
					
					if( m_siNoField != -1 )
					{
						m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);
						m_siNoField	= -1;
					}
				}
				
				// 에디트박스 영역
				if((PtInRect(&m_rtEditRect, m_ptMouse) == TRUE) && m_bMouseDown)
				{
					//m_pInputEditBox->SetFocus();
					Handler_OnlineEditControlEx::SetFocus( m_hOecEditBox );
				}

				// 닫기 버튼
				if(m_BClose.Process(m_bMouseDown) == TRUE)
					SetAction(FALSE);
				
				// 최소화 버튼
				if(m_BMini.Process(m_bMouseDown) == TRUE)
					SetMini(TRUE);
				
//				m_pInputEditBox->SetNextWindow(m_pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->GethWnd());
//				m_pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->SetNextWindow(m_pInputEditBox->GethWnd());
				
				// 엔터 쳤을경우
				//pOecEx	= Handler_OnlineEditControlEx::GetControl(m_hOecEditBox);
				//if(m_pInputEditBox->IsDoneMessage() == TRUE)
				if(pOecEx->IsPushEnter())
				{
					//if(m_pInputEditBox->IsFocus())
					if(Handler_OnlineEditControlEx::IsFocus(m_hOecEditBox))
						m_siStatus	= MESSENGER_SEND;
					//else 
						//m_pInputEditBox->SetFocus();
					else
						Handler_OnlineEditControlEx::SetFocus( m_hOecEditBox );

					
					//m_pInputEditBox->InitDoneMessage();
				}
			}
//			else
//			{
//				if(m_pInputEditBox->IsDoneMessage() == TRUE)
//				{
//					m_pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->SetFocus();
//					m_pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->SetString("");
//					m_pInputEditBox->InitDoneMessage();
//				}
//			}
		}
		break;
	case MESSENGER_SEND:
		{
			// 내용이 비었다면
			if(m_szSendContent[0] == 0)
			{
				m_siStatus = MESSENGER_POLL;
				break;
			}

			// 자기 메신저에 표시
			m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank((char*)m_szSendContent);
			AddChat(NULL, m_szSendContent);

			// 상대방에게 보내기
			sprintf(szBuffer, "/to %s %s", m_FriendInfo[m_siTabStatus].szFriendName, m_szSendContent);
			m_pMyOnlineWorld->pOnlinePannel->Process_ManagerOrder(szBuffer);
									
			//m_pInputEditBox->SetString("");
			pOecEx->Clear();


			m_siStatus = MESSENGER_POLL;
		}
		break;
	case MESSENGER_RECEIVE:
		{
			// PacketProc에서 AddChat처리 해줌.
			m_siStatus = MESSENGER_POLL;
		}
		break;
	case MESSENGER_DRAGING:
		{
			if(m_bMini)		break;
			
			DWORD CurTime = timeGetTime();
			m_siX = IpD.Mouse_X - m_siDragDifX;
			m_siY = IpD.Mouse_Y - m_siDragDifY;
			
			if( m_siX < 0)	m_siX = 0;
			if( m_siY < 0)	m_siY = 0;
			if((m_siX + m_MainSpr.Header.Xsize) > 800)	m_siX = 800 - m_MainSpr.Header.Xsize;
			if((m_siY + m_MainSpr.Header.Ysize) > 600)	m_siY = 600 - m_MainSpr.Header.Ysize;
			
			// m_siX, m_siY 좌표가 이동됨에 따라 영역 다시 설정
			SI16 siNameCount = -1;
			for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
			{
				m_BTab[i].SetX(m_siX);
				m_BTab[i].SetY(m_siY+19+18*i);

				if(m_BTab[i].IsAction())
					siNameCount++;
			}
			
			SetRect(&m_rtEditRect, m_siX + 77, m_siY + 179, m_siX + 320, m_siY + 196);
			SetRect(&m_rtDragRect, m_siX, m_siY, m_siX + 296, m_siY + 16);

			for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
			{
				m_ChatList[i].SetXY(m_siX+77, m_siY+26, m_siX+330, m_siY+31, m_siX+330, m_siY+31, 
					m_siX+77, m_siY+26);
			}
			m_BMini.SetX(m_siX + 304);		m_BMini.SetY(m_siY + 3);
			m_BClose.SetX(m_siX + 323);		m_BClose.SetY(m_siY + 3);

			if((m_pMyOnlineWorld->fLeftOneSwitch == 1) && ((CurTime - m_dwTime) > 50))
			{
				m_pMyOnlineWorld->bCharMove = TRUE;
				m_siStatus = MESSENGER_POLL;
				
				m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + m_MainSpr.Header.Xsize, m_siY + m_MainSpr.Header.Ysize);
			}
		}
		break;
	}

	if( IpD.LeftPressSwitch )		m_bMouseDown	=	TRUE;
	else							m_bMouseDown	=	FALSE;
}

VOID	OnlineMessenger::SetAction(BOOL bFlag, CHAR *pFriendName)
{
//	if(GameStatus != GS_ONLINEWORLD)		return;
	if(m_siStatus == MESSENGER_DRAGING)		return;

	SI08	i;
	BOOL	bIsSameName = FALSE;
	m_bAction	= bFlag;
	m_siStatus	= MESSENGER_POLL;

	if(m_bAction)
	{
		if(pFriendName)
		{
			// 같은 이름이 있나 체크
			for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
			{
				if(strcmp(pFriendName, m_FriendInfo[i].szFriendName) == 0)
				{
					bIsSameName = TRUE;
					m_siTabStatus = i;
					break;
				}
			}
			
			if(!bIsSameName)
			{
				for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
				{
					if(m_FriendInfo[i].szFriendName[0] == 0)
					{
						strcpy(m_FriendInfo[i].szFriendName, pFriendName);
						m_siTabStatus = i;
						break;
					}
				}
			}
			m_BTab[m_siTabStatus].SetMenuName(pFriendName);
			m_BTab[m_siTabStatus].SetAction(TRUE);
		}

		m_bIsNewMessage[m_siTabStatus] = FALSE;

		if(!m_bMini)
		{
			if( m_siNoField == -1 )
				m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + m_MainSpr.Header.Xsize, m_siY + m_MainSpr.Header.Ysize);

			SI16 siNameCount = -1;
			for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
			{
				if(m_BTab[i].IsAction())
					siNameCount++;
			}
			
			//m_pInputEditBox->EnableWindow( TRUE );
			if(GameStatus == GS_ONLINEWORLD)
			{
				m_pMyOnlineWorld->SetShortKey( TRUE );
				//m_pInputEditBox->SetFocus();
				//if(!Handler_OnlineEditControlEx::IsActive(m_pMyOnlineWorld->pOnlinePannel->GetHOECChat()))
				if(Handler_OnlineEditControlEx::GetFocus() == NULL)
					Handler_OnlineEditControlEx::SetFocus( m_hOecEditBox );
			}
		}
	}
	else
	{
		m_pMyOnlineWorld->SetShortKey(FALSE);
		m_siTabStatus	= 0;
		m_bWhisperOn	= TRUE;
		
		// 채팅리스트 문자열 초기화
		for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
		{
			m_ChatList[i].Initialize(); 

			m_BTab[i].SetAction(FALSE);
			m_BTab[i].SetMenuName("");
			m_bIsNewMessage[i] = FALSE;
			ZeroMemory(&m_FriendInfo[i], sizeof(m_FriendInfo[i]));
		}
		
		if( m_siNoField != -1 )
		{
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);
			m_siNoField	=	-1;
		}

//		m_pInputEditBox->EnableWindow( FALSE );
//		::SetFocus(pGame->Hwnd);

		Handler_OnlineEditControlEx::ClearCurrentFocus();

//		m_pInputEditBox->Initialize();
	}
}

VOID	OnlineMessenger::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;
	SI08	i;
	static	UI16	frame;
	char	szTemp[256];
	ZeroMemory(szTemp, 256);

	if(!m_bAction)		return;
	if(m_bMini)			return;
	
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		frame++;

		if(frame > 15)
		{
			m_bBlink = !m_bBlink;
			frame = 0;
		}

		if( m_siX < 0)	m_siX = 0;
		if( m_siY < 0)	m_siY = 0;
		if((m_siX + m_MainSpr.Header.Xsize) > 800)	m_siX = 800 - m_MainSpr.Header.Xsize;
		if((m_siY + m_MainSpr.Header.Ysize) > 600)	m_siY = 600 - m_MainSpr.Header.Ysize;
		clGrp.PutSpriteT(m_siX+330, m_siY+31, m_ScrBarSpr.Header.Xsize, m_ScrBarSpr.Header.Ysize, m_ScrBarSpr.Image);
		clGrp.PutSpriteJin(m_siX, m_siY, m_MainSpr.Header.Xsize, m_MainSpr.Header.Ysize, m_MainSpr.Image);
		
		for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
		{
			if((m_FriendInfo[i].szFriendName[0] != 0))
			{
				if(m_BTab[i].IsAction())
				{
					if(m_bIsNewMessage[i])
					{
						if(m_bBlink)	m_BTab[i].Put(&m_NameButSpr, 0, 0, 0, BUTTON_PUT_NOMOVE);
						else			m_BTab[i].Put(&m_NameButSpr, 1, 1, 1, BUTTON_PUT_NOMOVE);
					}
					else
						m_BTab[i].Put(&m_NameButSpr, 0, 1, 0, BUTTON_PUT_NOMOVE);
				}
			}
		}

		m_BMini.Put(&m_MiniSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_BClose.Put(&m_CloseSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}
	
	if( pSurface->GetDC( &hdc ) == DD_OK )
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );
		
		if(!m_bMini)
		{	
			// 대화상대 정보출력
			SI32 siGuildInGrade = m_FriendInfo[m_siTabStatus].siClassInGuildID;
			if(m_FriendInfo[m_siTabStatus].szGuildName[0])
			{
				sprintf((char*)szTemp, "%s [%s][%s]", m_FriendInfo[m_siTabStatus].szFriendName, m_FriendInfo[m_siTabStatus].szGuildName, m_pMyOnlineWorld->pOnlineText->Get(m_pMyOnlineWorld->m_cltIGuildClass.GetClassName(siGuildInGrade)));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX, m_siY+3, 320, 16, szTemp, RGB(250, 250, 250));
			}
			else
			{
				sprintf((char*)szTemp, "%s", m_FriendInfo[m_siTabStatus].szFriendName);
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX, m_siY+3, 320, 16, szTemp, RGB(250, 250, 250));
			}
			
			for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
			{
				if((m_FriendInfo[i].szFriendName[0] != 0))
				{
					if(m_siTabStatus == i)
						m_BTab[i].Put(hdc, (DWORD)0, RGB(0, 250, 0), RGB(0, 250, 0));
					else
						m_BTab[i].Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				}
			}
			
			// 에디트박스 대화 출력
//			m_pInputEditBox->DrawText(hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
//				m_pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT2), RGB(200, 200, 200), 
//				m_siX + 77, m_siY + 182, NULL, m_pInputEditBox->IsFocus());

			if(Handler_OnlineEditControlEx::IsFocus(m_hOecEditBox))
			{
				OnlineEditControlEx	*pOecEx;
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecEditBox );
				pOecEx->SetRectOfDraw(m_siX + 77,m_siY + 182,m_siX + 320, m_siY + 196);
				pOecEx->Draw( hdc );
			}
		}

		m_BMini.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BClose.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC( hdc );
	}

	// 채팅
	if(!m_bMini)
		m_ChatList[m_siTabStatus].Process(pSurface, ON_FONT_CHATLIST);
}

BOOL	OnlineMessenger::AddChat(CHAR *szID, CHAR *pStr)
{
	// 채팅 목록 버퍼에 삽입을 한다.
	SI08	i, siTabStatus = -1;
	CHAR	*pCharName = NULL;
	BOOL	bIsSameName = FALSE;
	
	if( szID == NULL )
	{
		// 내가 말한거
		pCharName	=	m_pMyOnlineWorld->pMyData->GetMyCharName();
		m_ChatList[m_siTabStatus].Add( pCharName, pStr);						
	}
	else
	{
		// 다른 사람이 말한거

		// 신규메세지가 도착했음을 판넬표시에 알림
		m_pMyOnlineWorld->pOnlinePannel->m_dwDistDelay = timeGetTime();
		m_pMyOnlineWorld->pOnlinePannel->m_bMessageShow = TRUE;	

		// 같은 이름이 있나 체크
		for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
		{
			if(strcmp(szID, m_FriendInfo[i].szFriendName) == 0)
			{
				bIsSameName = TRUE;
				siTabStatus = i;
				break;
			}
		}
		
		if(!bIsSameName)
		{
			// 처음 대화온 사람이므로 상단정보 요청
			m_pMyOnlineWorld->pOnlineClient->SendRequestGuildInfo((char*)szID);

			for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
			{
				if(m_FriendInfo[i].szFriendName[0] == 0)
				{
					strcpy(m_FriendInfo[i].szFriendName, szID);
					siTabStatus = i;
					break;
				}
			}

			// MESSENGER_FRIEND_MAX_NUM칸이 다 찼다면 이미 읽어본 Tab을 찾는다.
			if(siTabStatus == -1)
			{
				for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
				{
					if(!m_bIsNewMessage[i] && m_siTabStatus != i)
					{
						strcpy(m_FriendInfo[i].szFriendName, szID);
						siTabStatus = i;
						break;
					}
				}
				
				if(siTabStatus == -1)
				{
					for(i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
					{
						if(m_siTabStatus != i)
						{
							strcpy(m_FriendInfo[i].szFriendName, szID);
							siTabStatus = i;
							break;
						}
					}
				}
			}
		}
		
		m_ChatList[siTabStatus].Add(szID, pStr);
		m_BTab[siTabStatus].SetMenuName(szID);
		m_BTab[siTabStatus].SetAction(TRUE);

		// 새로온 메세지가 현재 대화 상대가 아니라면
		if(m_siTabStatus != siTabStatus || m_bMini)
		{
			m_bIsNewMessage[siTabStatus] = TRUE;
		}
	}
	return TRUE;
}

VOID	OnlineMessenger::SetFocusEditBox()
{
	if(!m_bMini)
	{
//		if(!m_pInputEditBox->IsFocus())
//			m_pInputEditBox->SetFocus();
		if(!Handler_OnlineEditControlEx::IsFocus(m_hOecEditBox))
			Handler_OnlineEditControlEx::SetFocus( m_hOecEditBox );
	}
	else
	{
//		m_pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->SetFocus();
//		m_pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->SetString("");
	}
}

VOID	OnlineMessenger::SetTabStatus(SI08 siStatus)
{
	if(m_FriendInfo[siStatus].szFriendName[0] != 0)
	{
		m_siTabStatus = siStatus;
		if(m_bIsNewMessage[siStatus])	m_bIsNewMessage[siStatus] = FALSE;
	}
}

VOID	OnlineMessenger::SetMini(BOOL bMini)
{
	if(m_siStatus == MESSENGER_DRAGING)		return;

	m_bMini = bMini;
	if(m_bMini)
	{
		m_pMyOnlineWorld->SetShortKey(FALSE);
//		m_pInputEditBox->EnableWindow(FALSE);
//		::SetFocus(pGame->Hwnd);
		Handler_OnlineEditControlEx::ClearCurrentFocus();

		// 전체화면의 이미지 노필드를 제거한다.
		if(m_siNoField != -1)
		{
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);
			m_siNoField = -1;
		}
	}
	else
		SetAction(TRUE);
}

VOID	OnlineMessenger::SetGuildInfo(CHAR *szID, CHAR *szGuildName, SI32 siClassInGuildID, SI32 siLevel)
{
	// 같은 이름이 있나 체크
	for(int i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
	{
		if(strcmp(szID, m_FriendInfo[i].szFriendName) == 0)
		{
			strcpy(m_FriendInfo[i].szGuildName, szGuildName);
			m_FriendInfo[i].siClassInGuildID = siClassInGuildID;
			m_FriendInfo[i].siLevel			 = siLevel;
			break;
		}
	}
}

BOOL	OnlineMessenger::IsNewMessage()
{
	for(SI08 i = 0; i < MESSENGER_FRIEND_MAX_NUM; i++)
	{
		if(m_bIsNewMessage[i])	return TRUE;
	}
	return FALSE;
}