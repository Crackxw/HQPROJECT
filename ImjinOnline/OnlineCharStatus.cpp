//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> ĳ���� ���� â
//	File Name		: OnlineCharStatus.cpp
//	Birth Date		: 2001. 12. 28.
//	Creator			: �� �� ��, �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ����� �ֿܼ��� ĳ���� ����â�� ����ִ� �κ��Դϴ�.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: OnlineCharStatus
// Code: All (2004-02-16)
// Desc: ���� ��� ����
//-----------------------------------------------------------------------------
#include <GSL.h>

#include "Mouse.h"
#include "clGame.h"

#include "OnlineCharStatus.h"
#include "OnlineMegaText.h"
#include "OnlineTradeBook.h"
#include "OnlineFieldArea.h"
#include <Common.h>


//-----------------------------------------------------------------------------
// Name: OnlineCharStatus
// Code: All (2004-02-16)
// Desc: ���� ���� �� ���ǵ� ����
//-----------------------------------------------------------------------------
extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 
extern _clGame* pGame;

#define	ON_CHAR_STATUS_NOACTION		0x00000000
#define	ON_CHAR_STATUS_INITMAIN		0x00000001
#define	ON_CHAR_STATUS_MAIN			0x00000002
#define	ON_CHAR_STATUS_SHOWBUTTON	0x00000004


//-----------------------------------------------------------------------------
// Name: OnlineCharStatus()
// Code: All (2004-02-16)
// Desc: ������
//-----------------------------------------------------------------------------
OnlineCharStatus::OnlineCharStatus()
{
	m_siNoFieldHandle	=	-1;
	m_siX				=	0;
	m_siY				=	0;
	m_siWidth			=	0;
	m_siHeight			=	0;
	m_siAccountNo		=	0;
	m_pMyOnlineWorld	=	NULL;
	m_pMyFollower		=	NULL;
	
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//	m_pEditBox_Rename	=	NULL;
	m_hOecRename		=	NULL;

	m_siStatus			=	ON_CHAR_STATUS_NOACTION;
	m_bMousePushStatus	=	FALSE;	
}


//-----------------------------------------------------------------------------
// Name: ~OnlineCharStatus()
// Code: All (2004-02-16)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
OnlineCharStatus::~OnlineCharStatus()
{
	Free();
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: All (2004-02-16)
// Desc: Ŭ���� �ʱ�ȭ. ���� ������ �ѹ��� ȣ���Ѵ�.
//		siAccountNo		- �������� �ο��� Ŭ���̾�Ʈ�� ���� ��ȣ
//		pOnlineWorld	- �ܺο��� ����� ���� ��ü ������
//		pFollower		- �ܺο��� ����� �뺴 ���� ������
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::Init( SI32 siAccountNo, cltOnlineWorld *pOnlineWorld, IOnlineFollower *pFollower )
{
//	SI32	siImKind;
	
	clGrp.LoadXspr( "Online\\Gamespr\\Book\\BOOK_1.Spr",				m_Spr_Frm );			// ���� ������
	clGrp.LoadXspr( "Online\\Gamespr\\Book\\book_1_statup_button.Spr",	m_Spr_Btn_GrowUp );		// �ɷ�ġ �� ��ư
	clGrp.LoadXspr( "Online\\GameSpr\\Book\\enter_button.Spr",			m_Spr_Btn_Rename );		// ���� �Է� ��ư
	clGrp.LoadXspr( "Online\\GameSpr\\Book\\main_window_button.Spr",	m_Spr_Btn_Common );		// �Ϲ� ��ư

	m_siAccountNo			=	siAccountNo;
	m_pMyOnlineWorld		=	pOnlineWorld;
	m_pMyFollower			=	pFollower;
//	siImKind				=	TransKindOnlineToImjin( m_pMyOnlineWorld->pMyData->GetMyCharKind() );
//	siFont					=	KI[siImKind].GetOnlinePortraitFont();
	m_siX					=	95;
	m_siY					=	45;
	m_siSelectedDBSlotNo		=	0;
	m_siWidth				=	m_Spr_Frm.Header.Xsize;
	m_siHeight				=	m_Spr_Frm.Header.Ysize;
	m_bMousePushStatus		=	FALSE;
	m_siRenameBtnStatus				=	0;

	SetRect( &m_rtBook, m_siX, m_siY, m_siX + m_Spr_Frm.Header.Xsize, m_siY + m_Spr_Frm.Header.Ysize );

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	// ���� �ٲٱ� ����Ʈ �ڽ� ����
//	if(m_pEditBox_Rename)
//	{
//		delete m_pEditBox_Rename;
//		m_pEditBox_Rename = NULL;
//	}
//	m_pEditBox_Rename		=	new OnlineEditControl;
//	m_pEditBox_Rename->CreateEditBox( 1000, 1000, 100, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_ID_LENGTH );
//	m_pEditBox_Rename->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
//	m_pEditBox_Rename->SetString("");
//	m_pEditBox_Rename->SetKeyLock('\'', TRUE);
//	m_pEditBox_Rename->SetKeyLock('\"', TRUE);
//	SendMessage( m_pEditBox_Rename->GethWnd(), EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)m_pMyOnlineWorld->pMyData->GetMyCharName() );

	OnlineEditControlEx	*pOecEx;
	if( m_hOecRename )	Handler_OnlineEditControlEx::KillControl( m_hOecRename );
	RECT	rcRect = { m_siX + 60, m_siY + 35, m_siX + 60 + 100, m_siY + 35 + 15, };
	m_hOecRename	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, ON_ID_LENGTH );
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecRename );
	pOecEx->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );
	pOecEx->SetColor( RGB(216, 201, 183) );
	pOecEx->SetString( m_pMyOnlineWorld->pMyData->GetMyCharName() );
	pOecEx->SetExceptionChar( '\'' );
	pOecEx->SetExceptionChar( '\"' );

	
	ZeroMemory( m_pszCharName, sizeof( m_pszCharName ) );

	// ���� �ٲٱ� ��ư ����
	m_Btn_ReName.Create( m_siX + 144, m_siY + 34, m_Spr_Btn_Rename.Header.Xsize, m_Spr_Btn_Rename.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );

	// â�ݱ� ��ư ����
	m_Btn_Close.Create(m_siX + 85,m_siY + 394, m_Spr_Btn_Common.Header.Xsize, m_Spr_Btn_Common.Header.Ysize + 2, m_pMyOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE );

	// �ɷ�ġ �� ��ư ����
	m_Btn_GrowUp[0].Create( m_siX + 88, m_siY + 162, m_Spr_Btn_GrowUp.Header.Xsize, m_Spr_Btn_GrowUp.Header.Ysize, "", BUTTON_PUT_LEFT, FALSE );
	m_Btn_GrowUp[1].Create( m_siX + 88, m_siY + 232, m_Spr_Btn_GrowUp.Header.Xsize, m_Spr_Btn_GrowUp.Header.Ysize, "", BUTTON_PUT_LEFT, FALSE );
	m_Btn_GrowUp[2].Create( m_siX + 88, m_siY + 280, m_Spr_Btn_GrowUp.Header.Xsize, m_Spr_Btn_GrowUp.Header.Ysize, "", BUTTON_PUT_LEFT, FALSE );
	m_Btn_GrowUp[3].Create( m_siX + 88, m_siY + 307, m_Spr_Btn_GrowUp.Header.Xsize, m_Spr_Btn_GrowUp.Header.Ysize, "", BUTTON_PUT_LEFT, FALSE );
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: All (2004-02-16)
// Desc: Ŭ���� ����. ���� �Ҹ�� �ѹ��� ȣ���Ѵ�.
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::Free()
{
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//	if( m_pEditBox_Rename )
//	{
//		m_pEditBox_Rename->Free();
//		delete m_pEditBox_Rename;
//		m_pEditBox_Rename	=	NULL;
//	}
	Handler_OnlineEditControlEx::KillControl( m_hOecRename );

	clGrp.FreeXspr(m_Spr_Frm);
	clGrp.FreeXspr(m_Spr_Btn_GrowUp);
	clGrp.FreeXspr(m_Spr_Btn_Rename);
	clGrp.FreeXspr(m_Spr_Btn_Common);
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: All (2004-02-16)
// Desc: ���� ���. 
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	SI32	siClipLeft, siClipTop, siClipRight, siClipBottom;
	HDC		hdc;
	CHAR	szTemp[100];

	COLORREF			rgb;
	SI16				siAR,siDR;
	OnlineEditControlEx	*pOecEx;

	// Ŭ�� ���� ���
	clGrp.GetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
	clGrp.SetClipFullScreen();

	// ����â ������ �׸���.
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT( m_siX, m_siY, m_Spr_Frm.Header.Xsize, m_Spr_Frm.Header.Ysize, m_Spr_Frm.Image );
		if( m_siStatus & ON_CHAR_STATUS_SHOWBUTTON )
		{
			//for(i=0; i<3; i++)
			//{
			if(m_Btn_GrowUp[0].IsAction())
				m_Btn_GrowUp[0].Put( &m_Spr_Btn_GrowUp, 0, 2, 1, BUTTON_PUT_NOMOVE );
			if(m_Btn_GrowUp[1].IsAction())
				m_Btn_GrowUp[1].Put( &m_Spr_Btn_GrowUp, 0, 2, 1, BUTTON_PUT_NOMOVE );
			if(m_Btn_GrowUp[2].IsAction())
				m_Btn_GrowUp[2].Put( &m_Spr_Btn_GrowUp, 0, 2, 1, BUTTON_PUT_NOMOVE );
			//}

			//������ ä��� ���� ���¶��.
			if( m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) != 0 )
				m_Btn_GrowUp[3].Put( &m_Spr_Btn_GrowUp, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}
		
		if( m_siSelectedDBSlotNo != 0 )	m_Btn_ReName.Put( &m_Spr_Btn_Rename, 0, 2, 1, BUTTON_PUT_NOMOVE );

		m_Btn_Close.Put(&m_Spr_Btn_Common,0,2,1, BUTTON_PUT_NOMOVE );

		clGrp.UnlockSurface( pSurface );
	}

	// �ؽ�Ʈ ���
	if(pSurface->GetDC(&hdc) == DD_OK)
	{	
		SI16	siTemp1, siTemp2, siTemp3;
		SI32	siMaxWeight = 0;

		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 60,m_siY + 8, 118, 12, m_pMyOnlineWorld->pOnlineText->Get(1100105), RGB( 216, 201, 183 ));				// LV

		sprintf(szTemp,"LV . %d",m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( m_siSelectedDBSlotNo ));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_siX + 177,m_siY + 35, 48, 15,szTemp,RGB( 216, 201, 183 ));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 20, m_siY + 59, 60, 12, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_EXP ), RGB( 216, 201, 183 ));				// ����ġ
		sprintf( szTemp,"%d / %d" , m_pMyFollower->GetFollowerParameterCurExp( m_siSelectedDBSlotNo ), m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp( m_siSelectedDBSlotNo ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 86,m_siY + 59 , 133,12 ,szTemp, RGB( 216, 201, 183 ));					
		
		if( m_siSelectedDBSlotNo == 0 )
		{
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 20, m_siY + 84, 60 ,15, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_CREDITGRADE ), RGB( 216, 201, 183 ));				// �ſ���
			sprintf( szTemp, "%d", m_pMyOnlineWorld->pMyData->GetTradeGrade());
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 86, m_siY + 84 ,133,15, szTemp, RGB( 216, 201, 183 ));					
			
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 20, m_siY + 107,60 , 12, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_FAME ), RGB( 216, 201, 183 ));				// �ſ뵵
			sprintf( szTemp, "%d / %d",m_pMyOnlineWorld->pMyData->GetTradeCredit(), m_pMyOnlineWorld->pMyData->GetNextTradeCredit(m_pMyOnlineWorld->pMyData->GetTradeGrade()));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 86, m_siY + 107 ,133,12, szTemp, RGB( 216, 201, 183 ));
	
		}
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_siX + 20, m_siY + 132 ,60 ,15, m_pMyOnlineWorld->pOnlineText->Get(1100001), RGB( 216, 201, 183 ));				// ������
		siMaxWeight = m_pMyOnlineWorld->pMyData->GetFollowerParameterBattlePower(m_siSelectedDBSlotNo);
		sprintf(szTemp, "%d",siMaxWeight);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 86, m_siY + 132 ,133,15, szTemp, RGB( 216, 201, 183 ));

	
		siTemp1	=	m_pMyFollower->GetFollowerParameterLife( m_siSelectedDBSlotNo );
		siTemp2	=	m_pMyFollower->GetFollowerParameterMaxLife( m_siSelectedDBSlotNo );
		siTemp3	=	(SI16)(((float)siTemp1 + 0.0001 ) / ((float)siTemp2 + 0.0001) * 100);

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 113, m_siY + 282,39,16,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_LIFE ), RGB( 216, 201, 183 ));				// ü��
		
		sprintf(szTemp,"%d / %d",siTemp1,siTemp2);
		if( siTemp3 > 20 )			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 158,m_siY + 282,68,16, szTemp, RGB( 216, 201, 183 ));
		else						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 158,m_siY + 282,68,16, szTemp, RGB( 200, 0, 0 ));
		

		siTemp1	=	m_pMyFollower->GetFollowerParameterMana(m_siSelectedDBSlotNo);
		siTemp2	=	m_pMyFollower->GetFollowerParameterMaxMana(m_siSelectedDBSlotNo);
		siTemp3	=	(SI16)(((float)siTemp1 + 0.0001 ) / ((float)siTemp2 + 0.0001) * 100);
		sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_MANA ));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 113, m_siY + 309 , 39,16, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_MANA ), RGB( 216, 201, 183 ));				// ������
		sprintf(szTemp,"%d / %d",siTemp1, siTemp2 );
		if( siTemp3 > 20 )			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 158, m_siY + 309,68,16, szTemp, RGB( 216, 201, 183 ));
		else						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 158, m_siY + 309,68,16, szTemp, RGB( 200, 0, 0 ));

		////////////////////////////////////////////////////////////////////////////////////////////////////

		if( m_pMyFollower->GetFollowerParameterTotalStr( m_siSelectedDBSlotNo ) == m_pMyFollower->GetFollowerBaseParameterSTR( m_siSelectedDBSlotNo ) )			rgb = RGB( 216, 201, 183 );
		else if( m_pMyFollower->GetFollowerParameterTotalStr( m_siSelectedDBSlotNo ) < m_pMyFollower->GetFollowerBaseParameterSTR( m_siSelectedDBSlotNo ) )		rgb = RGB( 200, 0, 0 );
		else if( m_pMyFollower->GetFollowerParameterTotalStr( m_siSelectedDBSlotNo ) > m_pMyFollower->GetFollowerBaseParameterSTR( m_siSelectedDBSlotNo ) )		rgb = RGB( 0, 135, 135 );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 12,m_siY + 163, 46,20,  m_pMyOnlineWorld->pOnlineText->Get( 6000007 ), RGB( 216, 201, 183 ));	     		// �� 
		sprintf( szTemp,"%d", m_pMyFollower->GetFollowerParameterTotalStr( m_siSelectedDBSlotNo ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 60,m_siY + 163, 26, 20,    szTemp, rgb);								// �� 


		if( m_pMyFollower->GetFollowerParameterTotalDex( m_siSelectedDBSlotNo ) == m_pMyFollower->GetFollowerBaseParameterDEX( m_siSelectedDBSlotNo ) )			rgb = RGB( 216, 201, 183 );
		else if( m_pMyFollower->GetFollowerParameterTotalDex( m_siSelectedDBSlotNo ) < m_pMyFollower->GetFollowerBaseParameterDEX( m_siSelectedDBSlotNo ) )		rgb = RGB( 200, 0, 0 );
		else if( m_pMyFollower->GetFollowerParameterTotalDex( m_siSelectedDBSlotNo ) > m_pMyFollower->GetFollowerBaseParameterDEX( m_siSelectedDBSlotNo ) )		rgb = RGB( 0, 135, 135 );
	
		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 12,m_siY + 232,46, 20,  m_pMyOnlineWorld->pOnlineText->Get( 6000008 ), RGB( 216, 201, 183 ));				// ��ø
		sprintf( szTemp,"%d", m_pMyFollower->GetFollowerParameterTotalDex( m_siSelectedDBSlotNo ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 60,m_siY + 232,26 ,20,  szTemp, rgb);								// ��ø


		if( m_pMyFollower->GetFollowerParameterTotalVit( m_siSelectedDBSlotNo ) == m_pMyFollower->GetFollowerBaseParameterVIT( m_siSelectedDBSlotNo ) )			rgb = RGB( 216, 201, 183 );
		else if( m_pMyFollower->GetFollowerParameterTotalVit( m_siSelectedDBSlotNo ) < m_pMyFollower->GetFollowerBaseParameterVIT( m_siSelectedDBSlotNo ) )		rgb = RGB( 200, 0, 0 );
		else if( m_pMyFollower->GetFollowerParameterTotalVit( m_siSelectedDBSlotNo ) > m_pMyFollower->GetFollowerBaseParameterVIT( m_siSelectedDBSlotNo ) )		rgb = RGB( 0, 135, 135 );

		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 12,m_siY + 280,46,20,  m_pMyOnlineWorld->pOnlineText->Get( 6000009 ), RGB( 216, 201, 183 ));				// �ǰ�
		sprintf( szTemp,"%d", m_pMyFollower->GetFollowerParameterTotalVit( m_siSelectedDBSlotNo ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 60,m_siY + 280,26,20,  szTemp, rgb);								// �ǰ�


		if( m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) == m_pMyFollower->GetFollowerBaseParameterINT( m_siSelectedDBSlotNo ) )			rgb = RGB( 216, 201, 183 );
		else if( m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) < m_pMyFollower->GetFollowerBaseParameterINT( m_siSelectedDBSlotNo ) )		rgb = RGB( 200, 0, 0 );
		else if( m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) > m_pMyFollower->GetFollowerBaseParameterINT( m_siSelectedDBSlotNo ) )		rgb = RGB( 0, 135, 135 );

		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 12,m_siY + 307 ,46,20, m_pMyOnlineWorld->pOnlineText->Get( 6000010 ), RGB( 216, 201, 183 ));				// ����
		sprintf( szTemp,"%d", m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 60,m_siY + 307,26,20,  szTemp, rgb);								


		if( m_pMyFollower->GetFollowerBaseParameterBonus( m_siSelectedDBSlotNo ) )
		{
			
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 12,m_siY + 334,46,20, m_pMyOnlineWorld->pOnlineText->Get(1000131), RGB( 216, 201, 183 ));
			sprintf( szTemp,"%d", m_pMyFollower->GetFollowerBaseParameterBonus( m_siSelectedDBSlotNo ) );
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 60,m_siY + 334,26,20, szTemp, RGB( 216, 201, 183 ) );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////


		siTemp1	=	m_pMyFollower->GetFollowerParameterTotalEquipMinDamage( m_siSelectedDBSlotNo );
		siTemp2	=	m_pMyFollower->GetFollowerParameterTotalEquipMaxDamage( m_siSelectedDBSlotNo );
		sprintf( szTemp,"%d - %d", siTemp1, siTemp2 );				// ���ݷ�
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 113, m_siY + 165,39 ,16, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_ATTACKRATE ), RGB( 216, 201, 183 ));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 158, m_siY + 165,68,16, szTemp, RGB( 216, 201, 183 ));


		siTemp1	=	m_pMyFollower->GetFollowerParameterTotalAC( m_siSelectedDBSlotNo );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 113,m_siY + 186,39,  16, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_DEFENCERATE ), RGB( 216, 201, 183 ));
		sprintf( szTemp,"%d", siTemp1 );						// ����
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 158,m_siY + 186, 68, 16, szTemp, RGB( 216, 201, 183 ));


		// ���� ���
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 113, m_siY + 207, 39,16, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_TOTALWEIGHT ), RGB( 216, 201, 183 ));					// �ִ� ����
		sprintf(szTemp,"%d / %d", m_pMyFollower->GetTotalItemWeight(m_siSelectedDBSlotNo), m_pMyFollower->GetMaxWeight(m_siSelectedDBSlotNo));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX+158,  m_siY + 207, 68,16, szTemp, RGB( 0, 135, 135 ));


		siAR = (SI16)((m_pMyFollower->GetFollowerParameterLevel(m_siSelectedDBSlotNo) + m_pMyFollower->GetFollowerParameterTotalDex( m_siSelectedDBSlotNo ))/4);
		//���߷�
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 113, m_siY + 234, 39,16, m_pMyOnlineWorld->pOnlineText->Get(8000496), RGB( 216, 201, 183 ));					
		sprintf(szTemp,"%d",siAR);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX+158,  m_siY + 234, 68,16, szTemp, RGB( 0, 135, 135 ));

		siDR = (SI16)(siAR + (m_pMyFollower->GetFollowerParameterTotalAC( m_siSelectedDBSlotNo ) + ( m_pMyFollower->GetFollowerParameterLevel(m_siSelectedDBSlotNo) * 3 / 2) 
			+ (m_pMyFollower->GetFollowerParameterTotalStr(m_siSelectedDBSlotNo) / 5)));
		//ȸ����
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 113, m_siY + 255, 39,16, m_pMyOnlineWorld->pOnlineText->Get(8000497), RGB( 216, 201, 183 ));
		sprintf(szTemp,"%d",siDR);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX+158,  m_siY + 255, 68,16, szTemp, RGB( 0, 135, 135 ));




		siTemp1	=	m_pMyFollower->GetFollowerParameterTotalDamageResist( m_siSelectedDBSlotNo );		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 23,m_siY + 366,49, 15, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_DAMAGEREGIST ),RGB( 216, 201, 183 ));
		sprintf( szTemp, "%d%%", siTemp1 );						// Ÿ�� ����
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 78,m_siY + 366,32,15, szTemp, RGB( 216, 201, 183 ));


		siTemp1	=	m_pMyFollower->GetFollowerParameterTotalMagicResist( m_siSelectedDBSlotNo );		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 125,m_siY + 366,49,15, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_MANAREGIST ), RGB( 216, 201, 183 ));
		sprintf(szTemp,"%d%%",siTemp1);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hdc, m_siX + 180,m_siY + 366,32,15, szTemp, RGB( 216, 201, 183 ));


		if( m_siSelectedDBSlotNo != 0 )					m_Btn_ReName.Put( hdc );
		m_Btn_Close.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
		// �̸�
//		m_pEditBox_Rename->GetString( m_pszCharName);
		//		m_pMyOnlineWorld->pOnlineMegaText->DrawLineCenter(hdc, m_siX + 28, m_siX + 96, m_siY + 38, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_NAME), RGB(216, 201, 183));
//		if(m_siSelectedDBSlotNo != 0)
//		{
//			m_pEditBox_Rename->DrawText(hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHATLIST), m_pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHATLIST), RGB(216, 201, 183), m_siX + 60, m_siY + 37, NULL, m_pEditBox_Rename->IsFocus());
//		}
//		else 
//		{
//			m_pEditBox_Rename->DrawText(hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHATLIST), m_pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHATLIST), RGB(216, 201, 183), m_siX + 60, m_siY + 37, NULL, m_pEditBox_Rename->IsFocus());
//		}

		pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecRename );
		strcpy( m_pszCharName, pOecEx->GetString() );
		pOecEx->Draw( hdc );
		
 
		pSurface->ReleaseDC(hdc);
	}

	clGrp.SetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
}

//-----------------------------------------------------------------------------
// Name: SetActive()
// Code: All (2004-02-16)
// Desc: Ȱ��ȭ ��ų�������� ����.
//		bActive	- Ȱ��ȭ�� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::SetActive(BOOL bActive)
{ 
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOecRename );
	if(bActive == TRUE) 
	{	
		m_siStatus	=	ON_CHAR_STATUS_INITMAIN;

		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//		m_pEditBox_Rename->SetString("");
//	//		if(m_pMyOnlineWorld->pMyData->GetFollowerCharName( m_siSelectedDBSlotNo ))	return;
//		SendMessage( m_pEditBox_Rename->GethWnd(), EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)m_pMyOnlineWorld->pMyData->GetFollowerCharName( m_siSelectedDBSlotNo ) );
		pOecEx->Clear();
		pOecEx->SetString( m_pMyOnlineWorld->pMyData->GetFollowerCharName( m_siSelectedDBSlotNo ) );

		if( m_siNoFieldHandle != -1 )		
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
			
		m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);
	}
	else
	{
		m_siStatus	=	ON_CHAR_STATUS_NOACTION;
		// ���콺�� ���̵��� �����Ѵ�.
		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);


		m_Btn_ReName.SetAction( TRUE );
		m_Btn_Close.SetAction(TRUE);
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//		//m_Btn_ReName.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGENAME));
//		m_pEditBox_Rename->SetString("");
//		::SetFocus( pGame->Hwnd );
		pOecEx->Clear();
		Handler_OnlineEditControlEx::ClearCurrentFocus();

		m_pMyOnlineWorld->SetShortKey( FALSE );

		m_siRenameBtnStatus	= 0;
		m_uiRenameDBSlotNo	= 0;

		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );		
		m_siNoFieldHandle = -1;

	}
}

//-----------------------------------------------------------------------------
// Name: IsActive()
// Code: All (2004-02-16)
// Desc: Ȱ��ȭ�Ǿ� �ִ°�?
//		return	- Ȱ��ȭ�� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineCharStatus::IsActive()
{
	if(m_siStatus != ON_CHAR_STATUS_NOACTION)	return TRUE;
	
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: Action()
// Code: All (2004-02-16)
// Desc: ���� ó����
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::Action()
{	
	SI16	i;	

	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecRename );
	if(m_Btn_Close.Process(m_bMousePushStatus))
	{
		//SetActive(FALSE);
		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
	}
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//	if(m_pEditBox_Rename->IsDoneMessage() == TRUE)
	if( pOecEx->IsPushEnter() )
	{
		// ����Ű�� ���
		m_pMyOnlineWorld->SetShortKey( FALSE );
		m_Btn_ReName.SetAction( FALSE );
		m_siRenameBtnStatus			= 2;
		m_uiRenameDBSlotNo	= (UI08)m_siSelectedDBSlotNo;	

		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
		// ����� �̸��� ������ ���� �ش�.
//		m_pEditBox_Rename->GetString(m_pszCharName);
		strcpy( m_pszCharName, pOecEx->GetString() );

		if( m_pszCharName[0] != 0 )
		{
			if(CheckLeadByte(m_pszCharName))
				m_pMyOnlineWorld->pOnlineClient->SendRequestSetRename( m_pszCharName, m_uiRenameDBSlotNo );
		}
		else
		{
			m_siRenameBtnStatus = 0;
			m_Btn_ReName.SetAction( TRUE );
			//m_Btn_ReName.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGENAME));
			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//			m_pEditBox_Rename->SetString("");
//			::SetFocus( pGame->Hwnd );
			pOecEx->Clear();
			Handler_OnlineEditControlEx::ClearCurrentFocus();
		}
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//		m_pEditBox_Rename->InitDoneMessage();
	}

	if( m_Btn_ReName.Process( m_bMousePushStatus ) && (m_siRenameBtnStatus == 0) && (m_siSelectedDBSlotNo != 0 ))
	{
		//m_Btn_ReName.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_OK ) );
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//		m_pEditBox_Rename->SetFocus();
		Handler_OnlineEditControlEx::SetFocus( m_hOecRename );

		// ����Ű�� ���´�.
		m_pMyOnlineWorld->SetShortKey( TRUE );
		m_siRenameBtnStatus	= 1;
	}
	else if( m_Btn_ReName.Process( m_bMousePushStatus ) && (m_siRenameBtnStatus == 1) )
	{		 
		// ����Ű�� ���
		m_pMyOnlineWorld->SetShortKey( FALSE );
		m_Btn_ReName.SetAction( FALSE );
		m_siRenameBtnStatus			= 2;
		m_uiRenameDBSlotNo	= (UI08)m_siSelectedDBSlotNo;	

		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
		// ����� �̸��� ������ ���� �ش�.
//		m_pEditBox_Rename->GetString(m_pszCharName);
		strcpy( m_pszCharName, pOecEx->GetString() );

		if( m_pszCharName[0] != 0 )
		{
			if(CheckLeadByte(m_pszCharName))
				m_pMyOnlineWorld->pOnlineClient->SendRequestSetRename( m_pszCharName, m_uiRenameDBSlotNo );
		}
		else
		{
			m_siRenameBtnStatus = 0;
			m_Btn_ReName.SetAction( TRUE );
			//m_Btn_ReName.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGENAME));
			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//			m_pEditBox_Rename->SetString("");
//			::SetFocus( pGame->Hwnd );
			pOecEx->Clear();
			Handler_OnlineEditControlEx::ClearCurrentFocus();
		}		
	}

	// ������ �ϸ� ���ʽ� ��ġ�� �־� ����.
	// ���� ���ʽ� ��ġ�� ���� �ִٸ� ���ݸ� ������ �� �� �ִٰ�
	if( m_pMyFollower->GetFollowerBaseParameterBonus( m_siSelectedDBSlotNo ) > 0 )
	{
		for( i=0 ; i<ON_CHAR_STATUS_NUM; i++ )
		{
			m_Btn_GrowUp[i].SetAction( TRUE );

			if( m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) == 0 && i == 3)
				m_Btn_GrowUp[3].SetAction( FALSE );

			if( m_Btn_GrowUp[i].Process( m_bMousePushStatus ) )
			{
				m_uiGrownUpDBSlotNo	=	m_siSelectedDBSlotNo;
				m_uiGrownUpAbility	=	(UI08)i;
				m_pMyOnlineWorld->pOnlineClient->SendBouns( m_uiGrownUpDBSlotNo, (AbilityKindType)i );

				m_Btn_GrowUp[i].SetAction( FALSE );				// ON_ABILITY_KIND_STR
				break;
			}
		}
		
		m_siStatus |= ON_CHAR_STATUS_SHOWBUTTON;
	}
	else
	{
		m_siStatus &= ~ON_CHAR_STATUS_SHOWBUTTON;
	}

	if( IpD.LeftPressSwitch )				m_bMousePushStatus = TRUE;
	else									m_bMousePushStatus = FALSE;
}

//-----------------------------------------------------------------------------
// Name: SetSelect()
// Code: All (2004-02-16)
// Desc: �ش� DB ������ �뺴�� ������ ������ ����
//		uiDBSlotNo	- ������ DB ���� ��ȣ
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::SetSelect( UI08 uiDBSlotNo )
{
	m_siSelectedDBSlotNo	= uiDBSlotNo;	

	// ����Ű�� ���
	m_pMyOnlineWorld->SetShortKey( FALSE );
	m_siRenameBtnStatus		= 0;
	m_Btn_ReName.SetAction( TRUE );
//	m_Btn_ReName.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGENAME));
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//	m_pEditBox_Rename->SetString("");

	//	siFont	=	TransKindOnlineToImjin( m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( uiDBSlotNo ) );
	//	siFont	=	KI[siFont].GetOnlinePortraitFont();

//	SendMessage( m_pEditBox_Rename->GethWnd(), EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)m_pMyOnlineWorld->pMyData->GetFollowerCharName( uiDBSlotNo ) );
//	::SetFocus( pGame->Hwnd );

	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecRename );
	pOecEx->Clear();
	pOecEx->SetString( m_pMyOnlineWorld->pMyData->GetFollowerCharName( uiDBSlotNo ) );
	Handler_OnlineEditControlEx::ClearCurrentFocus();

}

//VOID	OnlineCharStatus::SetX(SI16 six)
//{
//	OnlineCharStatus::m_siX = six;
//	for( SI16 i=0 ; i<ON_CHAR_STATUS_NUM; i++ )	
//	{
//		m_Btn_GrowUp[i].SetX( siX + 180 );
//		m_Btn_ReName.SetX( siX + 170 );
//	}
//}




//===================================================================================================
//	�������� ���� ���� �� ó��	- START
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: RecvMsg_GrownUp()
// Code: All (2004-02-16)
// Desc: �������� ���� ���� - �ش� �뺴�� ���� ������ ��ġ�� �÷��� ��� �������� ���� ���� ó��
//		bFlag	- ���������� �ݿ��Ǿ��� ��� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::RecvMsg_GrownUp( BOOL bFlag )
{
	if( bFlag )
		m_pMyFollower->Bonus_FollowerParameterUp( m_uiGrownUpDBSlotNo, (AbilityKindType)m_uiGrownUpAbility, 1, m_pMyOnlineWorld->pItemsInfo );
	
	m_Btn_GrowUp[0].SetAction( TRUE );
	m_Btn_GrowUp[1].SetAction( TRUE );
	m_Btn_GrowUp[2].SetAction( TRUE );
	if( m_pMyFollower->GetFollowerParameterTotalInt( m_siSelectedDBSlotNo ) != 0 )
		m_Btn_GrowUp[3].SetAction( TRUE );
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_Rename()
// Code: All (2004-02-16)
// Desc: �������� ���� ���� - �ش� �뺴�� ���� �̸� ������ �õ����� ��� �������� ���� ���� ó��
//		bFlag	- ���������� �ݿ��Ǿ��� ��� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
VOID	OnlineCharStatus::RecvMsg_Rename( BOOL bFlag )
{
	m_siRenameBtnStatus = 0;
	m_Btn_ReName.SetAction( TRUE );

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	//	m_Btn_ReName.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGENAME));	
//	::SetFocus( pGame->Hwnd );
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecRename );
	Handler_OnlineEditControlEx::ClearCurrentFocus();

	// ����
	if( bFlag )
	{
		SI16	i;		

		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		{
			if(m_pMyOnlineWorld->pMyData->IsValidFollower((UI08)i) == TRUE)
			{
				if( i == m_uiRenameDBSlotNo )
				{
					// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//					m_pEditBox_Rename->GetString(m_pszCharName);
//					m_pEditBox_Rename->SetString("");
//					SendMessage( m_pEditBox_Rename->GethWnd(), EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)m_pszCharName );
					strcpy( m_pszCharName, pOecEx->GetString() );
					pOecEx->Clear();
					pOecEx->SetString( m_pszCharName );
					
					m_pMyOnlineWorld->pMyData->SetFollowerCharName( (UI08)i, m_pszCharName );
					m_uiRenameDBSlotNo	=	0;
					ZeroMemory( m_pszCharName, sizeof(m_pszCharName));
					break;
				}
			}
		}
	}
	else
	{
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
//		m_pEditBox_Rename->SetString("");
//		SendMessage( m_pEditBox_Rename->GethWnd(), EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)m_pMyOnlineWorld->pMyData->GetFollowerCharName( m_siSelectedDBSlotNo ) );
		pOecEx->Clear();
		pOecEx->SetString( m_pMyOnlineWorld->pMyData->GetFollowerCharName( m_siSelectedDBSlotNo ) );

		m_uiRenameDBSlotNo	=	0;
		ZeroMemory( m_pszCharName, sizeof(m_pszCharName));
	}
}

//===================================================================================================
//	�������� ���� ���� �� ó��	- END
//===================================================================================================
