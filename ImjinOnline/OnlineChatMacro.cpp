//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 반복 말하기 설정창
//	File Name		: OnlineChatMacro.cpp
//	Birth Date		: 2003. 01. 13.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 사용자의 콘솔에서 반복 말하기 기능을 설정하는 창을 출력하는 클래스입니다.
//
//===================================================================================================


//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: 헤더 선언
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>

#include "OnlineChatMacro.h"

#include "OnlineMegaText.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"			
#include "OnlineFieldArea.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"


//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: 디파인 및 외부 클래스 정의
//-----------------------------------------------------------------------------

// actdoll (2004/06/07 11:14) : OnlineText.dat 말글
const	SI32	DF_TEXT_ONLINE_CHATMACRO_TITLE				= 8000471;		// 반복 말하기
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_CONTROL		= 8000472;		// 속도 조절
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_FASTEST		= 8000473;		// 빠름
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_FASTER		= 8000474;		// 약간 빠름
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_NORMAL		= 8000475;		// 보통
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_SLOWER		= 8000476;		// 약간 느림
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_SLOWEST		= 8000477;		// 느림
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SENTENCE			= 8000478;		// 문장
const	SI32	DF_TEXT_ONLINE_CHATMACRO_CANNOT_USE			= 8000693;		// 신용등급 6이상이 되야 쓸 수 있음.


const	SI32	DF_VALUE_CHATMACRO_DELAY_MIN		= 2000;			// 간격딜레이 최소값(최소 2초)
const	SI32	DF_VALUE_CHATMACRO_DELAY_MAX		= 12000;		// 간격딜레이 최대값(최대값에서 최소값을 땐 사이값이 가장 느린 딜레이값이다.)

extern	_clGame*		pGame;
extern _InputDevice		IpD;


//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2003-12-23)
// Desc: 클래스 초기화. 게임 생성시 한번만 호출한다.
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Init()
{
	m_bIsActive	= FALSE;
	m_bMousePush	= FALSE;
	m_bIsRunning	= FALSE;

	// actdoll (2004/05/07 12:28) : [OECEX]
	// 에디트 박스 UI 생성 및 적재
//	OnlineEditControl *pEdit;
//	for(int i=0; i<MAX_EDIT_BOX; i++)
//	{
//		RECT	rt;
//		pEdit	= new OnlineEditControl;
//		pEdit->CreateEditBox	( 1000, 1000, 210, 30, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 50, false, true );
//		pEdit->SetLanguage		( TRUE );
//		pEdit->SetFont			( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
//		SetRect( &rt, 323, 133 + ( i * 39 ), 323 + 208, 133 + ( i * 39 ) + 30 );
//
//		m_Size_EditBox.push_back( rt );
//		m_EditBox_Slot.push_back( pEdit );
//	}
	RECT				rcRect;
	HOEC				hOecEx, hOecExPrev, hOecExFront;
	OnlineEditControlEx	*pOecEx;
	for( int i=0; i<MAX_EDIT_BOX; i++ )
	{
		SetRect( &rcRect, 323, 133 + ( i * 39 ), 323 + 208, 133 + ( i * 39 ) + 30 );
		hOecEx	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, 50 );
		pOecEx	= Handler_OnlineEditControlEx::GetControl( hOecEx );
		pOecEx->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
		pOecEx->SetColor( RGB( 250, 250, 250 ) );
		
		if( i > 0 )					Handler_OnlineEditControlEx::SetNextHandle( hOecExPrev, hOecEx );
		else 						hOecExFront = hOecEx;
		if( i == MAX_EDIT_BOX - 1 )	Handler_OnlineEditControlEx::SetNextHandle( hOecEx, hOecExFront );
		hOecExPrev	= hOecEx;

		m_hOecSlot.push_back( hOecEx );
	}
	

	// 버튼 생성
	m_Btn_Common[OK].Create(279, 372, 56, 19, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OK), BUTTON_PUT_LEFT, FALSE);
	m_Btn_Common[CANCEL].Create(462, 372, 56, 19, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_CANCEL), BUTTON_PUT_LEFT, FALSE);

	// 버퍼 초기화 시키고
	ZeroMemory(m_pszString, sizeof(m_pszString));

	// 이미지 로드
	clGrp.LoadXspr("Online\\gamespr\\WINDOW_MACRO.Spr", m_Spr_Frm);
	clGrp.LoadXspr("Online\\gamespr\\SCR_HANDLE_1.Spr", m_Spr_Btn_ScrollBar);

	// 스피드 스크롤 구획 맞추고
	SetRect(&m_Size_ScrollBar, 315, 335, 315+170-m_Spr_Btn_ScrollBar.Header.Xsize, 335+12);

	m_dwLastUpdateTime	= 0;
	m_siDrawIndex		= 0;
	m_siNoFieldHandle	= -1;

	// 최초의 시간차에 따른 위치를 설정해준다.
	GetSpeedValue( m_siDrawTimeGap, m_siScrollBarXPos );
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2003-12-23)
// Desc: 클래스 제거. 게임 소멸시 한번만 호출한다.
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Free()
{
	// actdoll (2004/05/07 12:28) : [OECEX]
//	for(vector<OnlineEditControl*>::iterator i=m_EditBox_Slot.begin(); i!=m_EditBox_Slot.end(); ++i)
//	{
//		(*i)->Free();
//		delete (*i);
//	}
//	m_Size_EditBox.clear();
//	m_EditBox_Slot.clear();
	for( vector<HOEC>::iterator i=m_hOecSlot.begin(); i!=m_hOecSlot.end(); ++i )
	{
		Handler_OnlineEditControlEx::KillControl( (*i) );
	}
	m_hOecSlot.clear();

	clGrp.FreeXspr(m_Spr_Btn_ScrollBar);
	clGrp.FreeXspr(m_Spr_Frm);

}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2003-12-23)
// Desc: 데이터 초기화. 계속 호출될 수 있으며 이것이 호출되면 수치는 모두 초기화된다.
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Initialize()
{
	OnlineEditControlEx		*pOecEx;
	for( vector<HOEC>::iterator i=m_hOecSlot.begin(); i!=m_hOecSlot.end(); ++i )
	{
		 pOecEx	= Handler_OnlineEditControlEx::GetControl( (*i) );
		 pOecEx->Clear();
	}

	m_bIsActive		= FALSE;
	m_bMousePush	= FALSE;
	m_bIsRunning	= FALSE;

	// 버퍼 초기화 시키고
	ZeroMemory(m_pszString, sizeof(m_pszString));

	// 기본 값 초기화
	m_dwLastUpdateTime	= 0;
	m_siDrawIndex		= 0;
	if( m_siNoFieldHandle != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
	m_siNoFieldHandle = -1;

	// 최초의 시간차에 따른 위치를 설정해준다.
	GetSpeedValue( m_siDrawTimeGap, m_siScrollBarXPos );
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2003-12-23)
// Desc: 메인 출력. 
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Draw( LPDIRECTDRAWSURFACE7 lpSurface )
{	
	// 설정창이 동작중이라면 설정 화면을 띄워라
	if( IsActive() )		Draw_Settings( lpSurface );

	// 동작중이라면 조건에 맞춰 자동으로 채팅 메시지를 날려라
	if( IsRunning() )
	{
		// 시간차에 따라 채팅 인덱스가 바뀐다.
		if(timeGetTime() - m_dwLastUpdateTime > ( DF_VALUE_CHATMACRO_DELAY_MAX + DF_VALUE_CHATMACRO_DELAY_MIN - m_siDrawTimeGap ) )
		{
			m_siDrawIndex++;

			for( ; m_siDrawIndex<MAX_EDIT_BOX; m_siDrawIndex++ )	if( m_pszString[m_siDrawIndex][0] != 0 )	break;

			if( m_siDrawIndex >= MAX_EDIT_BOX )						m_siDrawIndex = 0;
			
			// 보낼 메시지가 존재한다면 보내라
			if( m_pszString[m_siDrawIndex][0] != 0 )
			{
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_pszString[m_siDrawIndex]);
				m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendChatting(m_pszString[m_siDrawIndex]);
				m_pMyOnlineWorld->pOnlinePannel->AddChat(0, m_pszString[m_siDrawIndex], CHATTINGMODE_NORMAL);
			}

			m_dwLastUpdateTime = timeGetTime();
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Draw_Settings()
// Code: actdoll (2003-12-23)
// Desc: 메인 출력 - 설정 화면. 
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Draw_Settings( LPDIRECTDRAWSURFACE7 lpSurface )
{
	HDC hDC;

	// 프레임 그리고
	if(clGrp.LockSurface(lpSurface))
	{
		clGrp.PutSpriteJin	( 242, 106, m_Spr_Frm.Header.Xsize, m_Spr_Frm.Header.Ysize, m_Spr_Frm.Image);
		clGrp.PutSpriteT	( 315 + m_siScrollBarXPos, 335, m_Spr_Btn_ScrollBar.Header.Xsize, m_Spr_Btn_ScrollBar.Header.Ysize, m_Spr_Btn_ScrollBar.Image);
		clGrp.UnlockSurface	( lpSurface );
	}

	// 텍스트 출력
	if(lpSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode	( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT) );

		// 버튼 출력
		m_Btn_Common[OK].Put(hDC);
		m_Btn_Common[CANCEL].Put(hDC);
		
		// 에디트 박스 출력
		char strText[512];
		int	 nCount = 0;

		// actdoll (2004/01/07 16:45) : 제목표시줄 글
		strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_TITLE ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawLineCenter( hDC, 242, 242 + m_Spr_Frm.Header.Xsize, 110, strText, RGB(255, 255, 255) );

		// actdoll (2004/01/07 16:45) : 타이머
		strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_CONTROL ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, 310, 335, strText, RGB(200, 200, 0) );

		// actdoll (2004/01/07 16:45) : 타이머 수치
		COLORREF	tempRGB;
		SI32		siDelayRate		= ( DF_VALUE_CHATMACRO_DELAY_MAX - DF_VALUE_CHATMACRO_DELAY_MIN ) / 5;

		if		( DF_VALUE_CHATMACRO_DELAY_MIN + siDelayRate * 4 < m_siDrawTimeGap )	{ strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_FASTEST ) );	tempRGB = RGB(255, 64, 64);		}
		else if	( DF_VALUE_CHATMACRO_DELAY_MIN + siDelayRate * 3 < m_siDrawTimeGap )	{ strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_FASTER ) );	tempRGB = RGB(255, 128, 128);	}
		else if	( DF_VALUE_CHATMACRO_DELAY_MIN + siDelayRate * 2 < m_siDrawTimeGap )	{ strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_NORMAL ) );	tempRGB = RGB(255, 64, 255);	}
		else if	( DF_VALUE_CHATMACRO_DELAY_MIN + siDelayRate * 1 < m_siDrawTimeGap )	{ strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_SLOWER ) );	tempRGB = RGB(128, 128, 255);	}
		else																			{ strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_SLOWEST ) );	tempRGB = RGB(64, 64, 255);		}
		m_pMyOnlineWorld->pOnlineMegaText->DrawLineCenter( hDC, 490, 535, 335, strText, tempRGB );

		// actdoll (2004/05/07 12:28) : [OECEX]
//		for(vector<OnlineEditControl*>::iterator i=m_EditBox_Slot.begin(); i!=m_EditBox_Slot.end(); ++i, ++nCount)
//		{
//			// actdoll (2004/01/07 16:45) : 채팅 사이드 문장 구획 글
//			sprintf( strText, "%s %d", m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SENTENCE ), nCount + 1 );
//			m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, 310, 133+(39*nCount), strText, RGB(200, 200, 0) );
//
//			BOOL	bSetCaret	= false;
//			if((*i)->IsFocus())		bSetCaret	= true;
//			(*i)->DrawTextAsBox( hDC, NULL, NULL, RGB(254, 254, 254), 323, 133 + (39*nCount), 208, NULL, bSetCaret );
//		}
		OnlineEditControlEx	*pOecEx;
		for( vector<HOEC>::iterator i=m_hOecSlot.begin(); i!=m_hOecSlot.end(); ++i, ++nCount )
		{
			sprintf( strText, "%s %d", m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SENTENCE ), nCount + 1 );
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, 310, 133 + ( 39 * nCount ) + 10, strText, RGB(200, 200, 0) );

			pOecEx = Handler_OnlineEditControlEx::GetControl( (*i) );
			if( pOecEx )	pOecEx->Draw( hDC );
		}

		lpSurface->ReleaseDC(hDC);
	}
}

//-----------------------------------------------------------------------------
// Name: Action()
// Code: actdoll (2003-12-23)
// Desc: 마우스 등의 동작 신호에 대한 행동 방식을 처리
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Action()
{
	// 좌판 혹은 거래 상태일 경우 비활성화 시킨다.
	if( m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )	End();	// 좌판
	if( m_pMyOnlineWorld->pOnlineTrade->IsActive() )	End();	// 거래
	if( !IsActive() )									return;


	if( m_Btn_Common[OK].Process(m_bMousePush) )		SaveString();
	if( m_Btn_Common[CANCEL].Process(m_bMousePush) )	OnButtonPush();
	if( m_bMousePush )									Action_Settings();	

	m_bMousePush = IpD.LeftPressSwitch;
}

//-----------------------------------------------------------------------------
// Name: Action_Settings()
// Code: actdoll (2003-12-23)
// Desc: 마우스 등의 동작 신호에 대한 행동 방식을 처리 - 설정 화면
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Action_Settings()
{
	POINT pt;

	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	// actdoll (2004/05/07 12:28) : [OECEX]
	// 에디트 박스들을 검사한다.
//	vector<OnlineEditControl*>::iterator edit=m_EditBox_Slot.begin();
//	for(vector<RECT>::iterator i=m_Size_EditBox.begin(); i!=m_Size_EditBox.end(); ++i)
//	{
//		if(PtInRect(&(*i), pt))
//		{
//			(*edit)->SetFocus();
//			break;
//		}
//
//		++edit;
//	}

	OnlineEditControlEx	*pOecEx;
	vector<HOEC>::iterator	iter;
	for( iter=m_hOecSlot.begin(); iter!=m_hOecSlot.end(); ++iter )
	{
		pOecEx	= Handler_OnlineEditControlEx::GetControl( (*iter) );
		if( pOecEx )
		{
			if( pOecEx->IsPointInRect( pt ) )
			{
//				HOEC	hOec;
//				DWORD	dwStatus;
//				BOOL	bOn;
//
//				hOec		= Handler_OnlineEditControlEx::GetFocus();
//				if( hOec )
//				{
//					// 기존에 있던 컨트롤의 다국어 상태 정보를 받고
//					pOecEx2		= Handler_OnlineEditControlEx::GetControl( hOec );
//					dwStatus	= ( pOecEx2->GetIMEBase() )->GetInputStatus();
//					bOn			= ( pOecEx2->GetIMEBase() )->IsIMEOn();
//
//					// 다음에 할당할 포커스에 정보를 준다.
//					( pOecEx->GetIMEBase() )->SetInputStatus	( pGame->Hwnd, dwStatus );
//					( pOecEx->GetIMEBase() )->SetIMEStatus		( pGame->Hwnd, bOn );
//				}

				Handler_OnlineEditControlEx::ClearCurrentFocus();
				Handler_OnlineEditControlEx::SetFocus( (*iter) );
				break;
			}
		}
	}

	// 속도 조절 바를 검사한다.
	if( PtInRect( &m_Size_ScrollBar, pt ) )		GetSpeedValue( m_siDrawTimeGap, m_siScrollBarXPos );
}

//-----------------------------------------------------------------------------
// Name: OnButtonPush()
// Code: actdoll (2003-12-23)
// Desc: 외부에서 이 작업을 호출하는 버튼을 눌러 이벤트가 발생했을 떄
//-----------------------------------------------------------------------------
void	OnlineChatMacro::OnButtonPush()
{
	// 신용등급 6 이상이 되야 이 기능을 쓸 수 있다.
	if( m_pMyOnlineWorld->pMyData->GetTradeGrade() < 6 )
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_CANNOT_USE ) );
		return;
	}

	m_bIsActive		= !m_bIsActive;
	m_bIsRunning	= FALSE;

	if( m_siNoFieldHandle != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
	m_siNoFieldHandle = -1;

	if( IsActive() )	
	{
		m_pMyOnlineWorld->SetShortKey( TRUE );
		SetData_Settings();
	}
	else
	{
		m_pMyOnlineWorld->SetShortKey( FALSE );
	}
}

//-----------------------------------------------------------------------------
// Name: OnRunningToggle()
// Code: actdoll (2003-12-23)
// Desc: 이 매크로를 실행/중지시키는 토글 이벤트가 발생했을 떄
//-----------------------------------------------------------------------------
void OnlineChatMacro::OnRunningToggle()
{
	// 신용등급 6 이상이 되야 이 기능을 쓸 수 있다.
	if( m_pMyOnlineWorld->pMyData->GetTradeGrade() < 6 )
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_CANNOT_USE ) );
		return;
	}

	if( IsRunning() )	{ End(); return; }

	m_bIsRunning		= TRUE;
	m_siDrawIndex		= 0;
	m_dwLastUpdateTime	= timeGetTime();

	if( m_pszString[m_siDrawIndex][0] != 0 )
	{
		m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_pszString[m_siDrawIndex]);
		m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendChatting(m_pszString[m_siDrawIndex]);
		m_pMyOnlineWorld->pOnlinePannel->AddChat(0, m_pszString[m_siDrawIndex], CHATTINGMODE_NORMAL);
	}
}

//-----------------------------------------------------------------------------
// Name: SetData_Settings()
// Code: actdoll (2003-12-23)
// Desc: 설정창에 표시될 정보를 적재한다.
//-----------------------------------------------------------------------------
void	OnlineChatMacro::SetData_Settings()
{
	// 버튼 활성화
	m_Btn_Common[OK].SetAction		( IsActive() );
	m_Btn_Common[CANCEL].SetAction	( IsActive() );

	// 필드 지정 불가 핸들 설정
	m_siNoFieldHandle = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( 242, 106, 242+m_Spr_Frm.Header.Xsize, 106+m_Spr_Frm.Header.Ysize );

	// actdoll (2004/05/07 12:28) : [OECEX]
	// 매크로에 사용될 텍스트들을 적재해 놓는다.
	OnlineEditControlEx	*pOecEx;
	int nCount = 0;
//	for(vector<OnlineEditControl*>::iterator i=m_EditBox_Slot.begin(); i!=m_EditBox_Slot.end(); ++i, ++nCount)
//		(*i)->SetString(m_pszString[nCount]);
	for( vector<HOEC>::iterator i=m_hOecSlot.begin(); i!=m_hOecSlot.end(); ++i, ++nCount )
	{
		pOecEx = Handler_OnlineEditControlEx::GetControl( (*i) );
		if( pOecEx )	pOecEx->SetString( m_pszString[nCount] );
	}
}

//-----------------------------------------------------------------------------
// Name: SaveString()
// Code: actdoll (2003-12-23)
// Desc: 설정창에 표기한 텍스트를 저장한다.
//-----------------------------------------------------------------------------
void OnlineChatMacro::SaveString()
{
	// actdoll (2004/05/07 12:28) : [OECEX]
	// 텍스트를 저장하되, 욕설이 있는지를 체크하고 필터링한다.
	int nCount	 = 0;
//	for(vector<OnlineEditControl*>::iterator i=m_EditBox_Slot.begin(); i!=m_EditBox_Slot.end(); ++i, ++nCount)
//	{
//		(*i)->GetString(m_pszString[nCount]);
//		m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(m_pszString[nCount]);
//	}
	OnlineEditControlEx	*pOecEx;
	for( vector<HOEC>::iterator i=m_hOecSlot.begin(); i!=m_hOecSlot.end(); ++i, ++nCount )
	{
		if( (*i) )	
		{
			pOecEx	= Handler_OnlineEditControlEx::GetControl( (*i) );
			strcpy( m_pszString[nCount], pOecEx->GetString() );
			m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank( m_pszString[nCount] );
		}
	}

	OnButtonPush();
}

//-----------------------------------------------------------------------------
// Name: GetSpeedValue
// Code: actdoll (2003-12-31)
// Desc: 현재 스크롤바의 위치에 따른 수치값을 자동으로 받아온다.
//		siTimeGap		- 현재 설정된 지연시간을 받아올 변수
//		siScrollBarXPos	- 현재 설정된 스크롤 바 증감 위치를 받아올 변수
//-----------------------------------------------------------------------------
void	OnlineChatMacro::GetSpeedValue( SI32 &siTimeGap, SI32 &siScrollBarXPos )
{
	POINT pt;
	pt.x	= IpD.Mouse_X;
	pt.y	= IpD.Mouse_Y;

	R32		fBase		= (R32)( m_Size_ScrollBar.right - m_Size_ScrollBar.left );
	if		( pt.x < m_Size_ScrollBar.left )		pt.x = m_Size_ScrollBar.left;
	else if	( pt.x > m_Size_ScrollBar.right )	pt.x = m_Size_ScrollBar.right;
	R32		fCurrent	= (R32)( pt.x - m_Size_ScrollBar.left );
	R32		fValue		= fCurrent / fBase;
	if		( fValue <= 0.2f )		fValue	= 0.2f;
	else if ( fValue <= 0.4f )		fValue	= 0.4f;
	else if ( fValue <= 0.6f )		fValue	= 0.6f;
	else if ( fValue <= 0.8f )		fValue	= 0.8f;
	else							fValue	= 1.0f;
	
	siTimeGap		= DF_VALUE_CHATMACRO_DELAY_MIN + (SI32)( ( DF_VALUE_CHATMACRO_DELAY_MAX - DF_VALUE_CHATMACRO_DELAY_MIN ) * fValue );
	siScrollBarXPos	= (SI32)( fBase * ( fValue - 0.1f ) );	
}