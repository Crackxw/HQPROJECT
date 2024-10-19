//------0--------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "..\Gersang\message.h"
#include "..\Gersang\music.h"
#include <chat.h>
#include "..\Gersang\text.h"
#include <mynet-Game.h>


extern _clGame *pGame;

extern  _Chat Chat;
extern SHORT MessageMode;			// 메시지가 누구에게 가는가?
extern _Text Text;
extern clock_t StartClock, CurrentClock, PrevClock;
extern _MyNetGame MyNet;
extern	SI32	UserPlayerNumber;					// 사용자 관련 


// 캐릭터 그림용
extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;

#ifdef _DEBUG
// robypark 2004/8/4 18:39
// 전투에서 몇 가지 치트키를 사용하기 위한 처리 루틴 추가
void ProcessCheatMessage(char *szCheatMessage);

// robypark 2004/10/15 18:53
// 전투 중에 그룹으로 유닛을 선택할 경우 변경한 내용을 위하여
// 원래 있던 채팅 리스트 박스 위치를 미니맵 위로 이동 시킨
// 좌표 값들의 상수 선언
#define MAX_CHAT_CONTENT_LINES_IN_LISTBOX_FOR_BATTLE_CHAT					6
#define MAX_CHAT_CONTENT_ONE_LINE_LENGTH_IN_LISTBOX_FOR_BATTLE_CHAT			45
#define MAX_BATTLE_CHAT_BOX_POSITION_X										2
#define MAX_BATTLE_CHAT_BOX_POSITION_Y										420
#endif


_Chat::_Chat()
{
	
}

void _Chat::Init()
{
#ifndef _DEBUG
	pGame->m_ChatList.Init(pGame->pOnlineWorld,ON_INN_ID_CONTENT,	// cltOnlineWorld *pOnlineWorld, SI32 siKind, 
							ON_GAMESPR_CHATTING_BACK, ON_GAMESPR_PANNEL_SCROLL_BAR,		// SI32 siBoardImg, SI32 siScrollBarImg, 
							4, 60,		// SI32 siMaxStrLine, SI32 siMaxStrLen,
							201, 509,	// SI32 siBoardX, SI32 siBoardY,
							201+386, 509+4,		// SI32 siScrollBarX, SI32 siScrollBarY,
							201+386, 509+4, 9, 59,		// SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siScrollAreaWidth, SI32 siScrollAreaHeight,
							201+9, 509+9, 13);	// SI32 siTextX, SI32 siTextY, SI32 siTextHeight,
#else	// _DEBUG
	// robypark 2004/10/15 18:53
	// 전투 중에 그룹으로 유닛을 선택할 경우 변경한 내용을 위하여
	// 원래 있던 채팅 리스트 박스 위치를 미니맵 위로 이동 시켰으며,
	// 스크롤이나 배경이미지를 지원 및 사용하지 않는다.
	pGame->m_ChatList.Init(pGame->pOnlineWorld,ON_INN_ID_CONTENT,	// cltOnlineWorld *pOnlineWorld, SI32 siKind, 
							-1, -1,		// SI32 siBoardImg, SI32 siScrollBarImg, 
							MAX_CHAT_CONTENT_LINES_IN_LISTBOX_FOR_BATTLE_CHAT, MAX_CHAT_CONTENT_ONE_LINE_LENGTH_IN_LISTBOX_FOR_BATTLE_CHAT,		// SI32 siMaxStrLine, SI32 siMaxStrLen,
							MAX_BATTLE_CHAT_BOX_POSITION_X, MAX_BATTLE_CHAT_BOX_POSITION_Y,	// SI32 siBoardX, SI32 siBoardY,
							MAX_BATTLE_CHAT_BOX_POSITION_X+386, MAX_BATTLE_CHAT_BOX_POSITION_Y+4,		// SI32 siScrollBarX, SI32 siScrollBarY,
							MAX_BATTLE_CHAT_BOX_POSITION_X+386, MAX_BATTLE_CHAT_BOX_POSITION_Y+4, 9, 59,		// SI32 siScrollAreaX, SI32 siScrollAreaY, SI32 siScrollAreaWidth, SI32 siScrollAreaHeight,
							MAX_BATTLE_CHAT_BOX_POSITION_X+9, MAX_BATTLE_CHAT_BOX_POSITION_Y+9, 13);	// SI32 siTextX, SI32 siTextY, SI32 siTextHeight,
#endif	// _DEBUG
}

void _Chat::Initialize()
{
	pGame->m_ChatList.Initialize();
}

void _Chat::SetInputModeSwitch(int mode, int max_string_size)
{
	if(mode == TRUE)
	{
		// ===============================================================================
//		pGame->pBattle->m_EditControl.SetFocus();
//
//		// 입력 글자의 최대 길이를 제한한다. 
//		SendMessage(pGame->pBattle->m_EditControl.GethWnd(), EM_LIMITTEXT, max_string_size, 0);			// 입력할 수 있는 글자 수를 제한함.	
//
//		// 이전에 저장되어 있던 정보를 모두 지운다. 
//		pGame->pBattle->m_EditControl.SetString("");
		// ===============================================================================
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
		OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat );
		pOecEx->Clear();
		pOecEx->SetMaxStringSize( max_string_size );
		Handler_OnlineEditControlEx::SetFocus( pGame->pBattle->m_hOecChat );
		// ===============================================================================
	}
	else
	{
		Handler_OnlineEditControlEx::ClearCurrentFocus();
	}

}


// 입력중인 문장을 표시한다. 
void _Chat::DrawInputText()

{
	HDC hdc;
	
	if(lpScreenBuffer->GetDC(&hdc)==DD_OK)
	{
		// ===============================================================================
//		SelectObject(hdc, pGame->pBattle->HelpFont);
		
//		SetBkColor(hdc, RGB( 0, 0, 255 ) );
		
//		SetTextColor(hdc, NORMAL_TEXT_COLOR );
		
//		SetBkMode(hdc, TRANSPARENT);
		
		// 입력중인 문장을 표시한다. 
//		if(pGame->pBattle->m_EditControl.IsFocus() == TRUE)
//		{
//			char buffer[FILE_NAME_SIZE ];
//			
//			//if(MessageMode==MESSAGEMODE_ALLY) wsprintf(buffer, "%s>", Text.AllyPlayer.Get());
//			//else                              wsprintf(buffer, "%s>", Text.AllPlayer.Get());
//			
//			pGame->pBattle->m_EditControl.DrawText(hdc, pGame->pBattle->HelpFont, pGame->pBattle->HelpFont_Underline, NORMAL_TEXT_COLOR, 233, 582, (char*)buffer, TRUE);
//		}
		// ===============================================================================

		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
		if( Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
		{
			SetBkColor	(hdc, RGB( 0, 0, 255 ) );
			SetBkMode	( hdc, TRANSPARENT );
			OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat );
			pOecEx->SetFont( pGame->pBattle->HelpFont );
			pOecEx->SetRectOfDraw( 233, 580, 233+330, 580+15 );
			pOecEx->SetColor( NORMAL_TEXT_COLOR );
			pOecEx->Draw( hdc );
		}
		// ===============================================================================

		lpScreenBuffer->ReleaseDC(hdc); 
	}
}


// 게임중에 입력을 받는다.
void InputOperateInGame()
{
	// ===============================================================================
	// 입력이 진행중인 상태에서 
//	if(pGame->pBattle->m_EditControl.IsFocus())
//	{
//
//		// 엔터키가 눌린 것을 확인하면,
//		if(pGame->pBattle->m_EditControl.IsDoneMessage() == TRUE)
//		{
//			char buffer[512];
//			
//			pGame->pBattle->m_EditControl.GetDoneMessage(buffer);
//
//			// 메시지를 띄운다. 
//			//SetMessage(buffer);
//
//			
//			// 여럿이 하기 중일때에는 
//			MyNet.SendGameChatMessage(buffer, MessageMode);
//			
//			pGame->m_ChatList.Add(MyNet.PlayersName.Name[UserPlayerNumber],buffer);
//
//			::SetFocus(pGame->Hwnd);
//
//			pGame->pBattle->m_EditControl.SetString("");
//			pGame->pBattle->m_EditControl.InitDoneMessage();
//
//			// 영어 모드
//			ChangeLanguage(FALSE);
//		
//		}
//	}
	// ===============================================================================

	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
	// 입력 맞춰진 상태에서 
	if( Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	{
		OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat );
		// 엔터키가 눌렸다면
		if( pOecEx->IsPushEnter() )
		{
			if( pOecEx->GetStringLen() > 0 )	
			{
				MyNet.SendGameChatMessage( (char*)pOecEx->GetString(), MessageMode );
				pGame->m_ChatList.Add(MyNet.PlayersName.Name[UserPlayerNumber], (char*)pOecEx->GetString() );
			#ifdef _DEBUG
				// robypark 2004/8/4 18:39
				// 전투에서 몇 가지 치트키를 사용하기 위한 처리 루틴 추가
				ProcessCheatMessage((char *)pOecEx->GetString());				
			#endif
				pOecEx->Clear();
			}
			// 포커스를 죽인다.
			// actdoll (2004/11/05 16:15) : 
			//	개조한 IME에서는 포커스를 죽이면 자동으로 영문모드로 돌아간다.
			Handler_OnlineEditControlEx::ClearCurrentFocus();
		}
	}
	// ===============================================================================
}

void ChangeLanguage(BOOL flag)
{
	//new Routine By OrionYoung jan-14-2004
/*	HIMC mHIMC = ImmGetContext(pGame->Hwnd);
	if (mHIMC==NULL)
		return;
	BOOL bIMC = ImmGetOpenStatus(mHIMC);
	if (flag != bIMC)
	{
		ImmSetOpenStatus(mHIMC,flag);
	}
	ImmReleaseContext(pGame->Hwnd,mHIMC);
*/
	// ===============================================================================
//	if(flag == TRUE) // native language 한국어가 될수도 있고 아닐수도 있고 
//		ImmSetConversionStatus(ImmGetContext(pGame->Hwnd), IME_CMODE_NATIVE, 0);
//	else             // 영어로 전환 
//		ImmSetConversionStatus(ImmGetContext(pGame->Hwnd), IME_CMODE_ALPHANUMERIC, 0);
	// ===============================================================================
}
