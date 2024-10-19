//------0--------------------------------------------------------------
//  ��� : ���°�
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
extern SHORT MessageMode;			// �޽����� �������� ���°�?
extern _Text Text;
extern clock_t StartClock, CurrentClock, PrevClock;
extern _MyNetGame MyNet;
extern	SI32	UserPlayerNumber;					// ����� ���� 


// ĳ���� �׸���
extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;

#ifdef _DEBUG
// robypark 2004/8/4 18:39
// �������� �� ���� ġƮŰ�� ����ϱ� ���� ó�� ��ƾ �߰�
void ProcessCheatMessage(char *szCheatMessage);

// robypark 2004/10/15 18:53
// ���� �߿� �׷����� ������ ������ ��� ������ ������ ���Ͽ�
// ���� �ִ� ä�� ����Ʈ �ڽ� ��ġ�� �̴ϸ� ���� �̵� ��Ų
// ��ǥ ������ ��� ����
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
	// ���� �߿� �׷����� ������ ������ ��� ������ ������ ���Ͽ�
	// ���� �ִ� ä�� ����Ʈ �ڽ� ��ġ�� �̴ϸ� ���� �̵� ��������,
	// ��ũ���̳� ����̹����� ���� �� ������� �ʴ´�.
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
//		// �Է� ������ �ִ� ���̸� �����Ѵ�. 
//		SendMessage(pGame->pBattle->m_EditControl.GethWnd(), EM_LIMITTEXT, max_string_size, 0);			// �Է��� �� �ִ� ���� ���� ������.	
//
//		// ������ ����Ǿ� �ִ� ������ ��� �����. 
//		pGame->pBattle->m_EditControl.SetString("");
		// ===============================================================================
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
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


// �Է����� ������ ǥ���Ѵ�. 
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
		
		// �Է����� ������ ǥ���Ѵ�. 
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
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
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


// �����߿� �Է��� �޴´�.
void InputOperateInGame()
{
	// ===============================================================================
	// �Է��� �������� ���¿��� 
//	if(pGame->pBattle->m_EditControl.IsFocus())
//	{
//
//		// ����Ű�� ���� ���� Ȯ���ϸ�,
//		if(pGame->pBattle->m_EditControl.IsDoneMessage() == TRUE)
//		{
//			char buffer[512];
//			
//			pGame->pBattle->m_EditControl.GetDoneMessage(buffer);
//
//			// �޽����� ����. 
//			//SetMessage(buffer);
//
//			
//			// ������ �ϱ� ���϶����� 
//			MyNet.SendGameChatMessage(buffer, MessageMode);
//			
//			pGame->m_ChatList.Add(MyNet.PlayersName.Name[UserPlayerNumber],buffer);
//
//			::SetFocus(pGame->Hwnd);
//
//			pGame->pBattle->m_EditControl.SetString("");
//			pGame->pBattle->m_EditControl.InitDoneMessage();
//
//			// ���� ���
//			ChangeLanguage(FALSE);
//		
//		}
//	}
	// ===============================================================================

	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	// �Է� ������ ���¿��� 
	if( Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	{
		OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat );
		// ����Ű�� ���ȴٸ�
		if( pOecEx->IsPushEnter() )
		{
			if( pOecEx->GetStringLen() > 0 )	
			{
				MyNet.SendGameChatMessage( (char*)pOecEx->GetString(), MessageMode );
				pGame->m_ChatList.Add(MyNet.PlayersName.Name[UserPlayerNumber], (char*)pOecEx->GetString() );
			#ifdef _DEBUG
				// robypark 2004/8/4 18:39
				// �������� �� ���� ġƮŰ�� ����ϱ� ���� ó�� ��ƾ �߰�
				ProcessCheatMessage((char *)pOecEx->GetString());				
			#endif
				pOecEx->Clear();
			}
			// ��Ŀ���� ���δ�.
			// actdoll (2004/11/05 16:15) : 
			//	������ IME������ ��Ŀ���� ���̸� �ڵ����� �������� ���ư���.
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
//	if(flag == TRUE) // native language �ѱ�� �ɼ��� �ְ� �ƴҼ��� �ְ� 
//		ImmSetConversionStatus(ImmGetContext(pGame->Hwnd), IME_CMODE_NATIVE, 0);
//	else             // ����� ��ȯ 
//		ImmSetConversionStatus(ImmGetContext(pGame->Hwnd), IME_CMODE_ALPHANUMERIC, 0);
	// ===============================================================================
}
