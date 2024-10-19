//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - SMS �޽��� ��º�
//	File Name		: OnlineSMS.cpp
//	Birth Date		: 2004. 06. 14.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �гο��� SMS�� ǥ���ϴ� �κ��Դϴ�.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-06-14)
// Desc: ���� ���
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <mouse.h>
#include <clGame.h>

#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineFieldArea.h"
#include "OnlineSMS.h"

const	SI32	DF_POS_ONLINE_SMS_FRAME_X			= 201;		// SMSâ x��ǥ

const	SI32	DF_POS_ONLINE_SMS_BTN_DIST_X		= 380;		// SMSâ ��ü���� ��ư���� ���� X��ġ
const	SI32	DF_POS_ONLINE_SMS_BTN_DIST_Y		= 4;		// SMSâ ��ü���� ��ư���� ��� Y��ġ
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_DIST_X		= 8;		// SMSâ ��ü���� �޽��� â���� ���� X��ġ
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_DIST_Y		= 8;		// SMSâ ��ü���� �޽��� â���� ��� Y��ġ
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_WIDTH		= 307;		// �޽��� â �ʺ�
const	SI32	DF_POS_ONLINE_SMS_MSGBOX_HEIGHT		= 27;		// �޽��� â ����

const	SI32	DF_POS_ONLINE_SMS_TEXT1_DIST_X		= 325;		// SMSâ ��ü���� �ؽ�Ʈ 1�� ��� ���� X��ġ
const	SI32	DF_POS_ONLINE_SMS_TEXT1_DIST_Y		= 7;		// SMSâ ��ü���� �ؽ�Ʈ 1�� ��� ��� Y��ġ
const	SI32	DF_POS_ONLINE_SMS_TEXT2_DIST_X		= 325;		// SMSâ ��ü���� �ؽ�Ʈ 2�� ��� ���� X��ġ
const	SI32	DF_POS_ONLINE_SMS_TEXT2_DIST_Y		= 23;		// SMSâ ��ü���� �ؽ�Ʈ 2�� ��� ��� Y��ġ
const	SI32	DF_POS_ONLINE_SMS_TEXT3_DIST_X		= 91;		// SMSâ ��ü���� �ؽ�Ʈ 3�� ��� ���� X��ġ
const	SI32	DF_POS_ONLINE_SMS_TEXT3_DIST_Y		= 5;		// SMSâ ��ü���� �ؽ�Ʈ 3�� ��� ��� Y��ġ

const	SI32	DF_TIME_ONLINE_SMS_MSGSHOW_LIMIT	= 7000;		// SMSâ�� ��µǴ� �޽����� ǥ�õǴ� ���� �ð�
const	SI32	DF_TIME_ONLINE_SMS_CHARDRAW_LIMIT	= 50;		// �޽����� �� ���ڰ� ���� ���� ������

const	char	*DF_TEXT_ONLINE_SMS_TEXT1			= "2000-814";										// Ȱ��ȭ�� �����κ� ù° �� �ؽ�Ʈ
const	char	*DF_TEXT_ONLINE_SMS_TEXT2			= "����/200��";										// Ȱ��ȭ�� �����κ� ��° �� �ؽ�Ʈ
const	char	*DF_TEXT_ONLINE_SMS_TEXT3			= "[�����ȴ�] �������� 2000-814 (����/�Ǵ�200��)";	// ��Ȱ��ȭ�� �� �ؽ�Ʈ

extern	_clGame* pGame;
extern _InputDevice				IpD;

//-----------------------------------------------------------------------------
// Name: OnlineSMS()
// Code: actdoll (2004-06-14)
// Desc: ������
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
// Desc: �Ҹ���
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
// Desc: ������ ����
//-----------------------------------------------------------------------------
void	OnlineSMS::Init( cltOnlineWorld *pOnlineWorld )
{
	m_pMyOnlineWorld	= pOnlineWorld;
	m_siFieldBlock		= -1;

	// ��������Ʈ �ε�
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Panel_1.Spr",		m_SprImg_FrameNormal );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Panel_2.Spr",		m_SprImg_FrameHide );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Button_Up.Spr",	m_SprBtn_SetNormalSize );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\Main_SMS_Button_Down.Spr", m_SprBtn_SetHideSize );

	// ��ȹ ����
	Initialize();

	// ��ư ����
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
// Desc: ������ �Ұ�
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
// Desc: ������ �ʱ�ȭ
//-----------------------------------------------------------------------------
void	OnlineSMS::Initialize()
{
	// �ʱ�ȭ�� �Ϲ� ��带 �������� �����Ѵ�.
	m_bIsHideMode		= false;
	m_bIsFirstStart		= true;

	m_pt_Frame.x			= DF_POS_ONLINE_SMS_FRAME_X;							// ������ ��ü
	m_pt_Frame.y			= ON_SCREEN_YSIZE - m_SprImg_FrameNormal.Header.Ysize;
	SetRect( &m_rt_Message, m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_DIST_Y,				// �޽��� ��� �κ�
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

	// �ϴ� ���ο� �����Ͱ� ������ Ȯ��
	PopSMSData();
}

//-----------------------------------------------------------------------------
// Name: Action()
// Code: actdoll (2004-06-14)
// Desc: ����
//-----------------------------------------------------------------------------
void	OnlineSMS::Action()
{
	m_bIsModeJustChanged	= false;

	if( m_bIsHideMode && m_Btn_SetNormalSize.Process( m_bMouseDown ) )		// �Ϲ� ������ ��ư�� ���ȴٸ�
	{
		OnToggleShowMode();
	}
	
	if( !m_bIsHideMode && m_Btn_SetHideSize.Process( m_bMouseDown ) )		// �ּ� ������ ��ư�� ���ȴٸ�
	{
		OnToggleShowMode();
	}

	// �����ִ� ��Ȳ�� �� �޽����� ���� ��� �� �۾��� ó���Ѵ�.
	if( !m_bIsHideMode )	Action_MessageWork();

	if( IpD.LeftPressSwitch )			m_bMouseDown	=	TRUE;
	else								m_bMouseDown	=	FALSE;
}

//-----------------------------------------------------------------------------
// Name: Action_MessageWork()										[private]
// Code: actdoll (2004-06-14)
// Desc: ���� - �޽��� ��� ó��
//-----------------------------------------------------------------------------
void	OnlineSMS::Action_MessageWork()
{
	// ó�� ���̴� �Ŷ�� �ð� üũ
	if( m_bIsFirstStart )
	{
		m_dwSMSStartTime	= GetTickCount();
		m_bIsFirstStart		= false;
	}

	if( m_bWriteFinish )		// �޽����� �� ���� �����϶�
	{
		// ���ѽð��� �Ѿ�ٸ� ����� �� �����޽�����
		if( GetTickCount() - m_dwSMSStartTime > (DWORD)DF_TIME_ONLINE_SMS_MSGSHOW_LIMIT )
		{
			// ���� �����Ͱ� �����Ѵٸ� �ѱ�� ���ٸ� �ѱ��� �ʴ´�.
			PopSMSData();
		}
	}
	else						// �޽����� ������ �ִ� �����
	{
		// �� ���� ��� �ð��� �Ѿ�ٸ�
		if( GetTickCount() - m_dwTextStartTime > (DWORD)DF_TIME_ONLINE_SMS_CHARDRAW_LIMIT )
		{
			// ������ �ִ� ������ ��ġ�� �˰�
			char	*pszPos = &m_pszShowMessage[m_siShowPosition];
			pszPos	= CharNextExA( pGame->m_uiCodePage, pszPos, 0 );
			
			// NULL���� ���ų� �Ȱ��� ��ġ�� �߰����� ���� �� ���� ���̴�.
			if( !pszPos || pszPos == &m_pszShowMessage[m_siShowPosition] )
			{
				m_dwTextStartTime	= 0;
				m_bWriteFinish		= true;
				return;
			}

			// �׷��� �ʴٸ� �ϳ��� �Ѱ��ش�.
			m_siShowPosition	= pszPos - m_pszShowMessage;
			m_dwTextStartTime	= GetTickCount();
		}
	}

}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-06-14)
// Desc: �׸���
//-----------------------------------------------------------------------------
void	OnlineSMS::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;
	// �׸��׸���
	if(clGrp.LockSurface(pSurface) == TRUE)
	{	
		if( m_bIsHideMode )			// ����ɾ��ִ� ���� ȭ�鿡 �ڽ��� ��´�.
		{
			// ������ ���
			clGrp.PutSpriteT( m_pt_Frame.x, m_pt_Frame.y, m_SprImg_FrameHide.Header.Xsize, m_SprImg_FrameHide.Header.Ysize, m_SprImg_FrameHide.Image );
			// ũ�� ���� ��ư ���
			m_Btn_SetNormalSize.Put( &m_SprBtn_SetNormalSize, 1, 2, 1, BUTTON_PUT_NOMOVE );
		}
		else
		{
			// ������ ���
			clGrp.PutSpriteT( m_pt_Frame.x, m_pt_Frame.y, m_SprImg_FrameNormal.Header.Xsize, m_SprImg_FrameNormal.Header.Ysize, m_SprImg_FrameNormal.Image );
			// ũ�� ���� ��ư ���
			m_Btn_SetHideSize.Put( &m_SprBtn_SetHideSize, 1, 2, 1, BUTTON_PUT_NOMOVE );
		}
		clGrp.UnlockSurface(pSurface);
	}

	// �ؽ�Ʈ ���
	if( pSurface->GetDC(&hdc) == DD_OK )
	{
		SetBkMode	( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );
		
		// �Ϲ� ����� ���
		if( !m_bIsHideMode )
		{
			// ���̽� �ؽ�Ʈ ���
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT1_DIST_X + 1, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT1_DIST_Y + 1, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT1, RGB( 0, 0, 0 ) );	
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT1_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT1_DIST_Y, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT1, RGB( 238, 238, 238 ) );	
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT2_DIST_X + 1, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT2_DIST_Y + 1, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT2, RGB( 0, 0, 0 ) );	
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pt_Frame.x + DF_POS_ONLINE_SMS_TEXT2_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_TEXT2_DIST_Y, 
																	(char*)DF_TEXT_ONLINE_SMS_TEXT2, RGB( 238, 238, 238 ) );	

			// SMS �޽��� ���
			char	pszTemp[256] = {0,};
			strncpy( pszTemp, m_pszShowMessage, m_siShowPosition );
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2( hdc, m_rt_Message.left, m_rt_Message.top, m_rt_Message.right - m_rt_Message.left, (char*)pszTemp, m_dwTextColor );	// �޽���
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hdc, m_rt_Message.right + 5, m_rt_Message.top + 13, (char*)m_pszShowName, m_dwNameColor );								// �������
		}
		else
		{
			// ���̽� �ؽ�Ʈ ���
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
// Desc: �ѹ� ���������� ������ ����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineSMS::OnToggleShowMode()
{
	if( m_siFieldBlock != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siFieldBlock );

	if( m_bIsHideMode )		// �����־��ٸ� ū�ɷ� �����.
	{
		m_pt_Frame.y			= ON_SCREEN_YSIZE - m_SprImg_FrameNormal.Header.Ysize;
		SetRect( &m_rt_Message, m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_DIST_X, m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_DIST_Y,				// �޽��� ��� �κ�
								m_pt_Frame.x + DF_POS_ONLINE_SMS_MSGBOX_WIDTH,	m_pt_Frame.y + DF_POS_ONLINE_SMS_MSGBOX_HEIGHT );
		m_siFieldBlock = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_Frame.x, m_pt_Frame.y, m_pt_Frame.x + m_SprImg_FrameNormal.Header.Xsize, m_pt_Frame.y + m_SprImg_FrameNormal.Header.Ysize );

		// ����ִ� �޽����� ���Ž�Ų��.
		PopSMSData();
	}
	else					// �����̶�� ���� ���·� �����.
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
// Desc: ���ۿ��� ����� �޽����� ���� ������.
//-----------------------------------------------------------------------------
BOOL	OnlineSMS::PopSMSData()
{
	// ���� �ʱ�ȭ
	ZeroMemory( m_pszShowName, sizeof(m_pszShowName) );
	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );

	// ���� ����� �����Ͱ� �ϳ��� ���ٸ�
	if( m_siSMSBuffer_Count <= 0 )			return FALSE;

	// ���� �ʱ�ȭ2
	m_bWriteFinish		= false;
	m_siShowPosition	= 0;
	m_dwSMSStartTime	= GetTickCount();

	ST_SMS_DATA *pSMS = m_SMSBuffer + m_siSMSBuffer_PopPos;

	// ��ȭ��ȣ ����(�������� ���ڰ� �����ð� ���� ���� atoi�� �ѹ� �����Ѵ�.)
	int		iLen, iNumber;
	iLen = strlen( pSMS->m_pszTelNumber );
	if( iLen <= 4 )	iNumber = atoi( pSMS->m_pszTelNumber );
	else			iNumber = atoi( &pSMS->m_pszTelNumber[iLen - 4] );

	// ���ۿ� ����
	sprintf( m_pszShowName, "[%04d]", iNumber );
	m_dwTextColor		= RGB( 221, 221, 221 );		// �Ϲ� �޽��� �ؽ�Ʈ ��
	m_dwNameColor		= RGB( 0, 200, 0 );			// �Ϲ� �޽��� ID��

	// ���ͳ� �� Ű�� ���� ��� ������Ŵ
	ZeroMemory( m_pszShowMessage, sizeof(m_pszShowMessage) );
	char	*pRes = strtok( pSMS->m_pszMessage, "\n\r\t" );
	if( pRes )
	{
		// �̰� �� �մܿ� �ش� �ܾ���� ���� ����
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
	
	// �弳���� �۵�
	m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank( m_pszShowMessage );

	// �� ī��Ʈ �ø��� ���� ���δ�
	m_siSMSBuffer_Count--;
	m_siSMSBuffer_PopPos++;
	if( m_siSMSBuffer_PopPos >= DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT )	m_siSMSBuffer_PopPos = 0;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: PushSMSData()
// Code: actdoll (2004-06-14)
// Desc: ���ۿ��� ����� �޽����� �����Ѵ�.
//-----------------------------------------------------------------------------
BOOL	OnlineSMS::PushSMSData( char *pszRecvTime, char *pszTelNumber, char *pszMessage )
{
	if( m_bIsHideMode )														return FALSE;
	if( m_siSMSBuffer_Count >= DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT )		return FALSE;
	
	ST_SMS_DATA *pSMS = m_SMSBuffer + m_siSMSBuffer_PushPos;

	// ���ۿ� ����
	ZeroMemory( pSMS, sizeof(ST_SMS_DATA) );
	strncpy( pSMS->m_pszRecvTime, pszRecvTime, 15 );
	strncpy( pSMS->m_pszTelNumber, pszTelNumber, 31 );
	strncpy( pSMS->m_pszMessage, pszMessage, 199 );
	
	// Ǫ�� ī��Ʈ �ø��� ���� �ø���
	m_siSMSBuffer_Count++;
	m_siSMSBuffer_PushPos++;
	if( m_siSMSBuffer_PushPos >= DF_VALUE_ONLINE_SMS_MAX_BUFFER_COUNT )	m_siSMSBuffer_PopPos = 0;

	return TRUE;
}

