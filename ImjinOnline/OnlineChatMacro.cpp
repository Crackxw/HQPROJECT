//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> �ݺ� ���ϱ� ����â
//	File Name		: OnlineChatMacro.cpp
//	Birth Date		: 2003. 01. 13.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: ������� �ֿܼ��� �ݺ� ���ϱ� ����� �����ϴ� â�� ����ϴ� Ŭ�����Դϴ�.
//
//===================================================================================================


//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: ��� ����
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>

#include "OnlineChatMacro.h"

#include "OnlineMegaText.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"			
#include "OnlineFieldArea.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"


//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: ������ �� �ܺ� Ŭ���� ����
//-----------------------------------------------------------------------------

// actdoll (2004/06/07 11:14) : OnlineText.dat ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_TITLE				= 8000471;		// �ݺ� ���ϱ�
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_CONTROL		= 8000472;		// �ӵ� ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_FASTEST		= 8000473;		// ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_FASTER		= 8000474;		// �ణ ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_NORMAL		= 8000475;		// ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_SLOWER		= 8000476;		// �ణ ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SPEED_SLOWEST		= 8000477;		// ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_SENTENCE			= 8000478;		// ����
const	SI32	DF_TEXT_ONLINE_CHATMACRO_CANNOT_USE			= 8000693;		// �ſ��� 6�̻��� �Ǿ� �� �� ����.


const	SI32	DF_VALUE_CHATMACRO_DELAY_MIN		= 2000;			// ���ݵ����� �ּҰ�(�ּ� 2��)
const	SI32	DF_VALUE_CHATMACRO_DELAY_MAX		= 12000;		// ���ݵ����� �ִ밪(�ִ밪���� �ּҰ��� �� ���̰��� ���� ���� �����̰��̴�.)

extern	_clGame*		pGame;
extern _InputDevice		IpD;


//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� �ʱ�ȭ. ���� ������ �ѹ��� ȣ���Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Init()
{
	m_bIsActive	= FALSE;
	m_bMousePush	= FALSE;
	m_bIsRunning	= FALSE;

	// actdoll (2004/05/07 12:28) : [OECEX]
	// ����Ʈ �ڽ� UI ���� �� ����
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
	

	// ��ư ����
	m_Btn_Common[OK].Create(279, 372, 56, 19, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OK), BUTTON_PUT_LEFT, FALSE);
	m_Btn_Common[CANCEL].Create(462, 372, 56, 19, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_CANCEL), BUTTON_PUT_LEFT, FALSE);

	// ���� �ʱ�ȭ ��Ű��
	ZeroMemory(m_pszString, sizeof(m_pszString));

	// �̹��� �ε�
	clGrp.LoadXspr("Online\\gamespr\\WINDOW_MACRO.Spr", m_Spr_Frm);
	clGrp.LoadXspr("Online\\gamespr\\SCR_HANDLE_1.Spr", m_Spr_Btn_ScrollBar);

	// ���ǵ� ��ũ�� ��ȹ ���߰�
	SetRect(&m_Size_ScrollBar, 315, 335, 315+170-m_Spr_Btn_ScrollBar.Header.Xsize, 335+12);

	m_dwLastUpdateTime	= 0;
	m_siDrawIndex		= 0;
	m_siNoFieldHandle	= -1;

	// ������ �ð����� ���� ��ġ�� �������ش�.
	GetSpeedValue( m_siDrawTimeGap, m_siScrollBarXPos );
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ����. ���� �Ҹ�� �ѹ��� ȣ���Ѵ�.
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
// Desc: ������ �ʱ�ȭ. ��� ȣ��� �� ������ �̰��� ȣ��Ǹ� ��ġ�� ��� �ʱ�ȭ�ȴ�.
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

	// ���� �ʱ�ȭ ��Ű��
	ZeroMemory(m_pszString, sizeof(m_pszString));

	// �⺻ �� �ʱ�ȭ
	m_dwLastUpdateTime	= 0;
	m_siDrawIndex		= 0;
	if( m_siNoFieldHandle != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
	m_siNoFieldHandle = -1;

	// ������ �ð����� ���� ��ġ�� �������ش�.
	GetSpeedValue( m_siDrawTimeGap, m_siScrollBarXPos );
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2003-12-23)
// Desc: ���� ���. 
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Draw( LPDIRECTDRAWSURFACE7 lpSurface )
{	
	// ����â�� �������̶�� ���� ȭ���� �����
	if( IsActive() )		Draw_Settings( lpSurface );

	// �������̶�� ���ǿ� ���� �ڵ����� ä�� �޽����� ������
	if( IsRunning() )
	{
		// �ð����� ���� ä�� �ε����� �ٲ��.
		if(timeGetTime() - m_dwLastUpdateTime > ( DF_VALUE_CHATMACRO_DELAY_MAX + DF_VALUE_CHATMACRO_DELAY_MIN - m_siDrawTimeGap ) )
		{
			m_siDrawIndex++;

			for( ; m_siDrawIndex<MAX_EDIT_BOX; m_siDrawIndex++ )	if( m_pszString[m_siDrawIndex][0] != 0 )	break;

			if( m_siDrawIndex >= MAX_EDIT_BOX )						m_siDrawIndex = 0;
			
			// ���� �޽����� �����Ѵٸ� ������
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
// Desc: ���� ��� - ���� ȭ��. 
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Draw_Settings( LPDIRECTDRAWSURFACE7 lpSurface )
{
	HDC hDC;

	// ������ �׸���
	if(clGrp.LockSurface(lpSurface))
	{
		clGrp.PutSpriteJin	( 242, 106, m_Spr_Frm.Header.Xsize, m_Spr_Frm.Header.Ysize, m_Spr_Frm.Image);
		clGrp.PutSpriteT	( 315 + m_siScrollBarXPos, 335, m_Spr_Btn_ScrollBar.Header.Xsize, m_Spr_Btn_ScrollBar.Header.Ysize, m_Spr_Btn_ScrollBar.Image);
		clGrp.UnlockSurface	( lpSurface );
	}

	// �ؽ�Ʈ ���
	if(lpSurface->GetDC(&hDC) == DD_OK)
	{
		SetBkMode	( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT) );

		// ��ư ���
		m_Btn_Common[OK].Put(hDC);
		m_Btn_Common[CANCEL].Put(hDC);
		
		// ����Ʈ �ڽ� ���
		char strText[512];
		int	 nCount = 0;

		// actdoll (2004/01/07 16:45) : ����ǥ���� ��
		strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_TITLE ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawLineCenter( hDC, 242, 242 + m_Spr_Frm.Header.Xsize, 110, strText, RGB(255, 255, 255) );

		// actdoll (2004/01/07 16:45) : Ÿ�̸�
		strcpy( strText, m_pMyOnlineWorld->pOnlineText->Get( DF_TEXT_ONLINE_CHATMACRO_SPEED_CONTROL ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, 310, 335, strText, RGB(200, 200, 0) );

		// actdoll (2004/01/07 16:45) : Ÿ�̸� ��ġ
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
//			// actdoll (2004/01/07 16:45) : ä�� ���̵� ���� ��ȹ ��
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
// Desc: ���콺 ���� ���� ��ȣ�� ���� �ൿ ����� ó��
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Action()
{
	// ���� Ȥ�� �ŷ� ������ ��� ��Ȱ��ȭ ��Ų��.
	if( m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )	End();	// ����
	if( m_pMyOnlineWorld->pOnlineTrade->IsActive() )	End();	// �ŷ�
	if( !IsActive() )									return;


	if( m_Btn_Common[OK].Process(m_bMousePush) )		SaveString();
	if( m_Btn_Common[CANCEL].Process(m_bMousePush) )	OnButtonPush();
	if( m_bMousePush )									Action_Settings();	

	m_bMousePush = IpD.LeftPressSwitch;
}

//-----------------------------------------------------------------------------
// Name: Action_Settings()
// Code: actdoll (2003-12-23)
// Desc: ���콺 ���� ���� ��ȣ�� ���� �ൿ ����� ó�� - ���� ȭ��
//-----------------------------------------------------------------------------
void	OnlineChatMacro::Action_Settings()
{
	POINT pt;

	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	// actdoll (2004/05/07 12:28) : [OECEX]
	// ����Ʈ �ڽ����� �˻��Ѵ�.
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
//					// ������ �ִ� ��Ʈ���� �ٱ��� ���� ������ �ް�
//					pOecEx2		= Handler_OnlineEditControlEx::GetControl( hOec );
//					dwStatus	= ( pOecEx2->GetIMEBase() )->GetInputStatus();
//					bOn			= ( pOecEx2->GetIMEBase() )->IsIMEOn();
//
//					// ������ �Ҵ��� ��Ŀ���� ������ �ش�.
//					( pOecEx->GetIMEBase() )->SetInputStatus	( pGame->Hwnd, dwStatus );
//					( pOecEx->GetIMEBase() )->SetIMEStatus		( pGame->Hwnd, bOn );
//				}

				Handler_OnlineEditControlEx::ClearCurrentFocus();
				Handler_OnlineEditControlEx::SetFocus( (*iter) );
				break;
			}
		}
	}

	// �ӵ� ���� �ٸ� �˻��Ѵ�.
	if( PtInRect( &m_Size_ScrollBar, pt ) )		GetSpeedValue( m_siDrawTimeGap, m_siScrollBarXPos );
}

//-----------------------------------------------------------------------------
// Name: OnButtonPush()
// Code: actdoll (2003-12-23)
// Desc: �ܺο��� �� �۾��� ȣ���ϴ� ��ư�� ���� �̺�Ʈ�� �߻����� ��
//-----------------------------------------------------------------------------
void	OnlineChatMacro::OnButtonPush()
{
	// �ſ��� 6 �̻��� �Ǿ� �� ����� �� �� �ִ�.
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
// Desc: �� ��ũ�θ� ����/������Ű�� ��� �̺�Ʈ�� �߻����� ��
//-----------------------------------------------------------------------------
void OnlineChatMacro::OnRunningToggle()
{
	// �ſ��� 6 �̻��� �Ǿ� �� ����� �� �� �ִ�.
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
// Desc: ����â�� ǥ�õ� ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineChatMacro::SetData_Settings()
{
	// ��ư Ȱ��ȭ
	m_Btn_Common[OK].SetAction		( IsActive() );
	m_Btn_Common[CANCEL].SetAction	( IsActive() );

	// �ʵ� ���� �Ұ� �ڵ� ����
	m_siNoFieldHandle = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( 242, 106, 242+m_Spr_Frm.Header.Xsize, 106+m_Spr_Frm.Header.Ysize );

	// actdoll (2004/05/07 12:28) : [OECEX]
	// ��ũ�ο� ���� �ؽ�Ʈ���� ������ ���´�.
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
// Desc: ����â�� ǥ���� �ؽ�Ʈ�� �����Ѵ�.
//-----------------------------------------------------------------------------
void OnlineChatMacro::SaveString()
{
	// actdoll (2004/05/07 12:28) : [OECEX]
	// �ؽ�Ʈ�� �����ϵ�, �弳�� �ִ����� üũ�ϰ� ���͸��Ѵ�.
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
// Desc: ���� ��ũ�ѹ��� ��ġ�� ���� ��ġ���� �ڵ����� �޾ƿ´�.
//		siTimeGap		- ���� ������ �����ð��� �޾ƿ� ����
//		siScrollBarXPos	- ���� ������ ��ũ�� �� ���� ��ġ�� �޾ƿ� ����
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