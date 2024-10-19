//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - SMS 메시지 출력부
//	File Name		: OnlineSMS.cpp
//	Birth Date		: 2004. 06. 14.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 패널에서 SMS를 표시하는 부분입니다.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-06-14)
// Desc: 포함 헤더
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <mouse.h>
#include <clGame.h>

#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineFieldArea.h"
#include "OnlineSMS.h"

const	SI32	DF_POS_ONLINE_SMS_FRAME_X			= 201;		// SMS창 x좌표

const	SI32	DF_POS_ONLINE_SMS_BTN_DIST_X		= 380;		// SMS창 본체에서 버튼까지 좌측 X위치
const	SI32	DF_POS_ONLINE_SMS_BTN_DIST_Y		= 4;		// SMS창 본체에서 버튼까지 상단 Y위치
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_DIST_X		= 8;		// SMS창 본체에서 메시지 창까지 좌측 X위치
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_DIST_Y		= 8;		// SMS창 본체에서 메시지 창까지 상단 Y위치
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_WIDTH		= 307;		// 메시지 창 너비
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_HEIGHT		= 27;		// 메시지 창 높이

const	SI32	DF_POS_ONLINE_SMS_TEXT1_DIST_X		= 325;		// SMS창 본체에서 텍스트 1번 출력 좌측 X위치
const	SI32	DF_POS_ONLINE_SMS_TEXT1_DIST_Y		= 7;		// SMS창 본체에서 텍스트 1번 출력 상단 Y위치
const	SI32	DF_POS_ONLINE_SMS_TEXT2_DIST_X		= 325;		// SMS창 본체에서 텍스트 2번 출력 좌측 X위치
const	SI32	DF_POS_ONLINE_SMS_TEXT2_DIST_Y		= 23;		// SMS창 본체에서 텍스트 2번 출력 상단 Y위치
const	SI32	DF_POS_ONLINE_SMS_TEXT3_DIST_X		= 91;		// SMS창 본체에서 텍스트 3번 출력 좌측 X위치
const	SI32	DF_POS_ONLINE_SMS_TEXT3_DIST_Y		= 5;		// SMS창 본체에서 텍스트 3번 출력 상단 Y위치

const	SI32	DF_TIME_ONLINE_SMS_MSGSHOW_LIMIT	= 7000;		// SMS창에 출력되는 메시지가 표시되는 제한 시간
const	SI32	DF_TIME_ONLINE_SMS_CHARDRAW_LIMIT	= 50;		// 메시지가 한 글자가 찍힐 때의 딜레이

const	char	*DF_TEXT_ONLINE_SMS_TEXT1			= "2000-814";										// 활성화시 우측부분 첫째 줄 텍스트
const	char	*DF_TEXT_ONLINE_SMS_TEXT2			= "유료/200원";										// 활성화시 우측부분 둘째 줄 텍스트
const	char	*DF_TEXT_ONLINE_SMS_TEXT3			= "[사통팔달] 국번없이 2000-814 (유료/건당200원)";	// 비활성화시 줄 텍스트

extern	_clGame* pGame;
extern _InputDevice				IpD;

//-----------------------------------------------------------------------------
// Name: OnlineSMS()
// Code: actdoll (2004-06-14)
// Desc: 생성자
//-----------------------------------------------------------------------------
OnlineSMS::OnlineSMS()
{
	m_pMyOnlineWorld	= NULL;

	ZeroMemory( &m_pt_Frame, sizeof(m_pt_Frame) );
	ZeroMemory( &m_rt_Message, sizeof(m_rt_Message) );
	
	m_bIsHideMode			= false;
	m_bIsModeJustChanged	= false;
	m_bIsFirstStart			= false;

	m_bMouseDown			= false;
	m_siFieldBlock			= -1;
	m_siFieldBlock_Button	= -1;

	ZeroMemory( m_SMSBuffer, sizeof(m_SMSBuffer) );
	m_siSMSBuffer_PopPos	= 0;
	m_siSMSBuffer_PushPos	= 0;
	m_siSMSBuffer_Count		= 0;

	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );
	ZeroMemory( m_pszShowName, sizeof(m_pszShowName) );
	m_siShowPosition		= 0;
	m_bWriteFinish			= false;
	m_dwSMSStartTime		= 0;
	m_dwTextStartTime		= 0;
	m_dwTextColor			= 0;
	m_dwNameColor			= 0;
}

//-----------------------------------------------------------------------------
// Name: ~OnlineSMS()
// Code: actdoll (2004-06-14)
// Desc: 소멸자
//-----------------------------------------------------------------------------
OnlineSMS::~OnlineSMS()
{
	Free();

	m_pMyOnlineWorld	= NULL;

	ZeroMemory( &m_pt_Frame, sizeof(m_pt_Frame) );
	ZeroMemory( &m_rt_Message, sizeof(m_rt_Message) );
	
	m_bIsHideMode			= false;
	m_bIsModeJustChanged	= false;
	m_bIsFirstStart			= false;

	m_bMouseDown			= false;
	m_siFieldBlock			= -1;
	m_siFieldBlock_Button	= -1;

	ZeroMemory( m_SMSBuffer, sizeof(m_SMSBuffer) );
	m_siSMSBuffer_PopPos	= 0;
	m_siSMSBuffer_PushPos	= 0;
	m_siSMSBuffer_Count		= 0;

	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );
	ZeroMemory( m_pszShowName, sizeof(m_pszShowName) );
	m_siShowPosition		= 0;
	m_bWriteFinish			= false;
	m_dwSMSStartTime		= 0;
	m_dwTextStartTime		= 0;
	m_dwTextColor			= 0;
	m_dwNameColor			= 0;
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-06-14)
// Desc: 데이터 생성
//-----------------------------------------------------------------------------
void	OnlineSMS::Init( cltOnlineWorld *pOnlineWorld )
{
	m_pMyOnlineWorld	= pOnlineWorld;
	m_siFieldBlock		= -1;

	// 스프라이트 로드
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Panel_1.Spr",		m_SprImg_FrameNormal );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Panel_2.Spr",		m_SprImg_FrameHide );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Button_Up.Spr",	m_SprBtn_SetNormalSize );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Button_Down.Spr", m_SprBtn_SetHideSize );

	// 구획 지정
	Initialize();

	// 버튼 생성
	SI32	siX, siY;
	siX = DF_POS_ONLINE_SMS_FRAME_X + DF_POS_ONLINE_SMS_BTN_DIST_X;
	siY = ON_SCREEN_YSIZE - m_SprImg_FrameHide.Header.Ysize		+ DF_POS_ONLINE_SMS_BTN_DIST_Y;
	m_Btn_SetNormalSize.Create	( siX, siY, m_SprBtn_SetNormalSize.Header.Xsize,	m_SprBtn_SetNormalSize.Header.Ysize,	NULL,	BUTTON_PUT_LEFT, TRUE );
	siY = ON_SCREEN_YSIZE - m_SprImg_FrameNormal.Header.Ysize	+ DF_POS_ONLINE_SMS_BTN_DIST_Y;
	m_Btn_SetHideSize.Create	( siX, siY, m_SprBtn_SetHideSize.Header.Xsize,		m_SprBtn_SetHideSize.Header.Ysize,		NULL,	BUTTON_PUT_LEFT, TRUE );	
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: actdoll (2004-06-14)
// Desc: 데이터 소거
//-----------------------------------------------------------------------------
void	OnlineSMS::Free()
{
	clGrp.FreeXspr( m_SprImg_FrameNormal );
	clGrp.FreeXspr( m_SprImg_FrameHide );
	clGrp.FreeXspr( m_SprBtn_SetNormalSize );
	clGrp.FreeXspr( m_SprBtn_SetHideSize );
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2004-06-14)
// Desc: 데이터 초기화
//-----------------------------------------------------------------------------
void	OnlineSMS::Initialize()
{
	// 초기화시 일반 모드를 기준으로 세팅한다.
	m_bIsHideMode		= false;
	m_bIsFirstStart		= true;

	m_pt_Frame.x			= DF_POS_ONLINE_SMS_FRAME_X;							// 프레임 본체
	m_pt_Frame.y			= ON_SCREEN_YSIZE - m_SprImg_FrameNormal.Header.Ysize;
	SetRect( &m_rt_Message, m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_DIST_Y,				// 메시지 찍는 부분
							m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_WIDTH,	m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_HEIGHT );

	if( m_siFieldBlock != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siFieldBlock );
	m_siFieldBlock = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_Frame.x, m_pt_Frame.y, m_pt_Frame.x + m_SprImg_FrameNormal.Header.Xsize, m_pt_Frame.y + m_SprImg_FrameNormal.Header.Ysize );

	ZeroMemory( m_SMSBuffer, sizeof(m_SMSBuffer) );
	m_siSMSBuffer_PopPos	= 0;
	m_siSMSBuffer_PushPos	= 0;
	m_siSMSBuffer_Count		= 0;	

	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );
	ZeroMemory( m_pszShowName, sizeof(m_pszShowName) );
	m_siShowPosition		= 0;
	m_bWriteFinish			= false;
	m_dwSMSStartTime		= 0;
	m_dwTextStartTime		= 0;
	m_dwTextColor			= 0;
	m_dwNameColor			= 0;

	// 일단 새로운 데이터가 없는지 확인
	PopSMSData();
}

//-----------------------------------------------------------------------------
// Name: Action()
// Code: actdoll (2004-06-14)
// Desc: 동작
//-----------------------------------------------------------------------------
void	OnlineSMS::Action()
{
	m_bIsModeJustChanged	= false;

	if( m_bIsHideMode && m_Btn_SetNormalSize.Process( m_bMouseDown ) )		// 일반 사이즈 버튼이 눌렸다면
	{
		OnToggleShowMode();
	}
	
	if( !m_bIsHideMode && m_Btn_SetHideSize.Process( m_bMouseDown ) )		// 최소 사이즈 버튼이 눌렸다면
	{
		OnToggleShowMode();
	}

	// 보여주는 상황일 때 메시지가 있을 경우 그 작업을 처리한다.
	if( !m_bIsHideMode )	Action_MessageWork();

	if( IpD.LeftPressSwitch )			m_bMouseDown	=	TRUE;
	else								m_bMouseDown	=	FALSE;
}

//-----------------------------------------------------------------------------
// Name: Action_MessageWork()										[private]
// Code: actdoll (2004-06-14)
// Desc: 동작 - 메시지 출력 처리
//-----------------------------------------------------------------------------
void	OnlineSMS::Action_MessageWork()
{
	// 처음 보이는 거라면 시간 체크
	if( m_bIsFirstStart )
	{
		m_dwSMSStartTime	= GetTickCount();
		m_bIsFirstStart		= false;
	}

	if( m_bWriteFinish )		// 메시지가 다 찍힌 상태일때
	{
		// 제한시간을 넘어갔다면 지우고 그 다음메시지로
		if( GetTickCount() - m_dwSMSStartTime > (DWORD)DF_TIME_ONLINE_SMS_MSGSHOW_LIMIT )
		{
			// 다음 데이터가 존재한다면 넘기고 없다면 넘기지 않는다.
			PopSMSData();
		}
	}
	else						// 메시지가 찍히고 있는 경우라면
	{
		// 한 글자 찍는 시간이 넘어갔다면
		if( GetTickCount() - m_dwTextStartTime > (DWORD)DF_TIME_ONLINE_SMS_CHARDRAW_LIMIT )
		{
			// 찍히고 있는 버퍼의 위치를 알고
			char	*pszPos = &m_pszShowMessage[m_siShowPosition];
			pszPos	= CharNextExA( pGame->m_uiCodePage, pszPos, 0 );
			
			// NULL값이 오거나 똑같은 위치를 발견했을 때는 다 찍은 것이다.
			if( !pszPos || pszPos == &m_pszShowMessage[m_siShowPosition] )
			{
				m_dwTextStartTime	= 0;
				m_bWriteFinish		= true;
				return;
			}

			// 그렇지 않다면 하나를 넘겨준다.
			m_siShowPosition	= pszPos - m_pszShowMessage;
			m_dwTextStartTime	= GetTickCount();
		}
	}

}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-06-14)
// Desc: 그리기
//-----------------------------------------------------------------------------
void	OnlineSMS::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;
	// 그림그리기
	if(clGrp.LockSurface(pSurface) == TRUE)
	{	
		if( m_bIsHideMode )			// 가라앉아있는 모드면 화면에 박스만 찍는다.
		{
			// 프레임 찍고
			clGrp.PutSpriteT( m_pt_Frame.x, m_pt_Frame.y, m_SprImg_FrameHide.Header.Xsize, m_SprImg_FrameHide.Header.Ysize, m_SprImg_FrameHide.Image );
			// 크기 변경 버튼 찍고
			m_Btn_SetNormalSize.Put( &m_SprBtn_SetNormalSize, 1, 2, 1, BUTTON_PUT_NOMOVE );
		}
		else
		{
			// 프레임 찍고
			clGrp.PutSpriteT( m_pt_Frame.x, m_pt_Frame.y, m_SprImg_FrameNormal.Header.Xsize, m_SprImg_FrameNormal.Header.Ysize, m_SprImg_FrameNormal.Image );
			// 크기 변경 버튼 찍고
			m_Btn_SetHideSize.Put( &m_SprBtn_SetHideSize, 1, 2, 1, BUTTON_PUT_NOMOVE );
		}
		clGrp.UnlockSurface(pSurface);
	}

	// 텍스트 찍기
	if( pSurface->GetDC(&hdc) == DD_OK )
	{
		SetBkMode	( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );
		
		// 일반 모드일 경우
		if( !m_bIsHideMode )
		{
			// 베이스 텍스트 찍기
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT1_DIST_X + 1, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT1_DIST_Y + 1, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT1, RGB( 0, 0, 0 ) );	
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT1_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT1_DIST_Y, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT1, RGB( 238, 238, 238 ) );	
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT2_DIST_X + 1, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT2_DIST_Y + 1, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT2, RGB( 0, 0, 0 ) );	
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT2_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT2_DIST_Y, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT2, RGB( 238, 238, 238 ) );	

			// SMS 메시지 출력
			char	pszTemp[256] = {0,};
			strncpy( pszTemp, m_pszShowMessage, m_siShowPosition );
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2( hdc, m_rt_Message.left, m_rt_Message.top, m_rt_Message.right - m_rt_Message.left, (char*)pszTemp, m_dwTextColor );	// 메시지
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hdc, m_rt_Message.right + 5, m_rt_Message.top + 13, (char*)m_pszShowName, m_dwNameColor );								// 보낸사람
		}
		else
		{
			// 베이스 텍스트 찍기
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT3_DIST_X + 1, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT3_DIST_Y + 1,
																	215, 13, (char*)DF_TEXT_ONLINE_SMS_TEXT3, RGB( 0, 0, 0 ) );
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT3_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT3_DIST_Y,
																	215, 13, (char*)DF_TEXT_ONLINE_SMS_TEXT3, RGB( 238, 238, 238 ) );
		}

		pSurface->ReleaseDC(hdc);
	}
}

//-----------------------------------------------------------------------------
// Name: OnToggleShowMode()
// Code: actdoll (2004-06-14)
// Desc: 한번 누를때마다 설정을 토글한다.
//-----------------------------------------------------------------------------
void	OnlineSMS::OnToggleShowMode()
{
	if( m_siFieldBlock != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siFieldBlock );

	if( m_bIsHideMode )		// 숨어있었다면 큰걸로 만든다.
	{
		m_pt_Frame.y			= ON_SCREEN_YSIZE - m_SprImg_FrameNormal.Header.Ysize;
		SetRect( &m_rt_Message, m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_DIST_Y,				// 메시지 찍는 부분
								m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_WIDTH,	m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_HEIGHT );
		m_siFieldBlock = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_Frame.x, m_pt_Frame.y, m_pt_Frame.x + m_SprImg_FrameNormal.Header.Xsize, m_pt_Frame.y + m_SprImg_FrameNormal.Header.Ysize );

		// 들어있는 메시지를 갱신시킨다.
		PopSMSData();
	}
	else					// 보통이라면 숨은 형태로 만든다.
	{
		m_pt_Frame.y			= ON_SCREEN_YSIZE - m_SprImg_FrameHide.Header.Ysize;
		ZeroMemory( &m_rt_Message, sizeof(m_rt_Message) );
		m_siFieldBlock = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_Frame.x, m_pt_Frame.y, m_pt_Frame.x + m_SprImg_FrameHide.Header.Xsize, m_pt_Frame.y + m_SprImg_FrameHide.Header.Ysize );
	}

	m_bIsHideMode			= !m_bIsHideMode;
	m_bIsModeJustChanged	= true;
}

//-----------------------------------------------------------------------------
// Name: PopSMSData()												[private]
// Code: actdoll (2004-06-14)
// Desc: 버퍼에서 출력할 메시지를 새로 꺼낸다.
//-----------------------------------------------------------------------------
BOOL	OnlineSMS::PopSMSData()
{
	// 공통 초기화
	ZeroMemory( m_pszShowName, sizeof(m_pszShowName) );
	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );

	// 만약 출력할 데이터가 하나도 없다면
	if( m_siSMSBuffer_Count <= 0 )			return FALSE;

	// 공통 초기화2
	m_bWriteFinish		= false;
	m_siShowPosition	= 0;
	m_dwSMSStartTime	= GetTickCount();

	ST_SMS_DATA *pSMS = m_SMSBuffer + m_siSMSBuffer_PopPos;

	// 전화번호 변조(쓸데없는 문자가 들어오늘걸 막기 위해 atoi로 한번 변조한다.)
	int		iLen, iNumber;
	iLen = strlen( pSMS->m_pszTelNumber );
	if( iLen <= 4 )	iNumber = atoi( pSMS->m_pszTelNumber );
	else			iNumber = atoi( &pSMS->m_pszTelNumber[iLen - 4] );

	// 버퍼에 적재
	sprintf( m_pszShowName, "[%04d]", iNumber );
	m_dwTextColor		= RGB( 221, 221, 221 );		// 일반 메시지 텍스트 색
	m_dwNameColor		= RGB( 0, 200, 0 );			// 일반 메시지 ID색

	// 엔터나 탭 키가 있을 경우 변조시킴
	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );
	char	*pRes = strtok( pSMS->m_pszMessage, "\n\r\t" );
	if( pRes )
	{
		// 이건 맨 앞단에 해당 단어들이 왔을 때다
		if( pRes != pSMS->m_pszMessage )	strcat( m_pszShowMessage, " " );
		strcat( m_pszShowMessage, pRes );
	}

	while( pRes )
	{
		pRes = strtok( NULL, "\n\r\t" );
		if( !pRes )		break;
		strcat( m_pszShowMessage, " " );
		strcat( m_pszShowMessage, pRes );
	}
	
	// 욕설필터 작동
	m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank( m_pszShowMessage );

	// 팝 카운트 올리고 갯수 줄인다
	m_siSMSBuffer_Count--;
	m_siSMSBuffer_PopPos++;
	if( m_siSMSBuffer_PopPos >= DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT )	m_siSMSBuffer_PopPos = 0;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: PushSMSData()
// Code: actdoll (2004-06-14)
// Desc: 버퍼에서 출력할 메시지를 저장한다.
//-----------------------------------------------------------------------------
BOOL	OnlineSMS::PushSMSData( char *pszRecvTime, char *pszTelNumber, char *pszMessage )
{
	if( m_bIsHideMode )														return FALSE;
	if( m_siSMSBuffer_Count >= DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT )		return FALSE;
	
	ST_SMS_DATA *pSMS = m_SMSBuffer + m_siSMSBuffer_PushPos;

	// 버퍼에 적재
	ZeroMemory( pSMS, sizeof(ST_SMS_DATA) );
	strncpy( pSMS->m_pszRecvTime, pszRecvTime, 15 );
	strncpy( pSMS->m_pszTelNumber, pszTelNumber, 31 );
	strncpy( pSMS->m_pszMessage, pszMessage, 199 );
	
	// 푸쉬 카운트 올리고 갯수 늘린다
	m_siSMSBuffer_Count++;
	m_siSMSBuffer_PushPos++;
	if( m_siSMSBuffer_PushPos >= DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT )	m_siSMSBuffer_PopPos = 0;

	return TRUE;
}

