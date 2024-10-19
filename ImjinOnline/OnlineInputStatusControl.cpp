//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �Է±� ���� ǥ�ñ�
//	File Name		: OnlineInputStatusControl.cpp
//	Birth Date		: 2004. 11. 01.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Է� ���¿� ���� ǥ�ø� �����ϴ� Ŭ�����Դϴ�.(�ѱ�/����/�߹� ��)
//		��� ������ ��/�� ���� �Է� ��� ǥ��, ����â, ����â ���� ǥ���մϴ�.
//		��� �� ��´��� ���� ��ܿ� ǥ�õǾ�� �ϴ� ���Դϴ�.
//		GS_IMEBase�� �ݵ�� ���ԵǾ�� �մϴ�.
//
//===================================================================================================

#include <GSL.h>
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

#include <clgame.h>
#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineInputStatusControl.h"


//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-11-01)
// Desc: ���ǵ�
//-----------------------------------------------------------------------------
const	DWORD	DF_INDEX_ICON_LOCALE_KOREAN					= 0;	// �����ϰ�	- �ѱ���
const	DWORD	DF_INDEX_ICON_LOCALE_JAPANESE				= 1;	// �����ϰ�	- �Ϻ���
const	DWORD	DF_INDEX_ICON_LOCALE_CHINESE_SIMP			= 2;	// �����ϰ�	- �߱��� ��ü
const	DWORD	DF_INDEX_ICON_LOCALE_CHINESE_TRAD			= 3;	// �����ϰ�	- �߱��� ��ü
const	DWORD	DF_INDEX_ICON_LOCALE_ENGLISH				= 4;	// �����ϰ�	- ����/��ġ�ȵ�
const	DWORD	DF_INDEX_ICON_LOCALE_UNKNOWN				= 9;	// �����ϰ�	- �ǵ��ȵ�
	
const	DWORD	DF_INDEX_ICON_IME_NONE						= 8;	// IME��	- ��ġ�ȵ�
const	DWORD	DF_INDEX_ICON_IME_UNKNOWN					= 9;	// IME��	- �� �� ����

const	DWORD	DF_INDEX_ICON_IME_KOR_ENGLISH				= 10;	// IME��	- �ѱ���	- ����
const	DWORD	DF_INDEX_ICON_IME_KOR_HANGUL				= 11;	// IME��	- �ѱ���	- �ѱ�
const	DWORD	DF_INDEX_ICON_IME_KOR_HANJA					= 12;	// IME��	- �ѱ���	- ����

const	DWORD	DF_INDEX_ICON_IME_JPN_ENGLISH				= 20;	// IME��	- �Ϻ���	- ����
const	DWORD	DF_INDEX_ICON_IME_JPN_HIRAGANA				= 21;	// IME��	- �Ϻ���	- ���󰡳�
const	DWORD	DF_INDEX_ICON_IME_JPN_KATAKANA				= 22;	// IME��	- �Ϻ���	- īŸī��

const	DWORD	DF_INDEX_ICON_IME_CHS_DEFAULT				= 8;	// IME��	- �߹���ü	- Default
const	DWORD	DF_INDEX_ICON_IME_CHS_USKEYBOARD			= 8;	// IME��	- �߹���ü	- ��ü US Keyboard
const	DWORD	DF_INDEX_ICON_IME_CHS_NEIMA					= 30;	// IME��	- �߹���ü	- Neima
const	DWORD	DF_INDEX_ICON_IME_CHS_QUANPIN				= 31;	// IME��	- �߹���ü	- QuanPin
const	DWORD	DF_INDEX_ICON_IME_CHS_SHUANGPIN				= 32;	// IME��	- �߹���ü	- ShuangPin
const	DWORD	DF_INDEX_ICON_IME_CHS_ZHENGMA				= 33;	// IME��	- �߹���ü	- ZhengMa
const	DWORD	DF_INDEX_ICON_IME_CHS_MSPY					= 34;	// IME��	- �߹���ü	- Microsoft Pinyin IME 3.0
const	DWORD	DF_INDEX_ICON_IME_CHS_INTELABC				= 35;	// IME��	- �߹���ü	- ���� ABC

const	DWORD	DF_INDEX_ICON_IME_CHT_DEFAULT				= 8;	// IME��	- �߹���ü	- Default
const	DWORD	DF_INDEX_ICON_IME_CHT_USKEYBOARD			= 8;	// IME��	- �߹���ü	- ��ü US Keyboard
const	DWORD	DF_INDEX_ICON_IME_CHT_ALPHANUMERIC			= 40;	// IME��	- �߹���ü	- AlphaNumeric
const	DWORD	DF_INDEX_ICON_IME_CHT_ARRAY					= 41;	// IME��	- �߹���ü	- Array
const	DWORD	DF_INDEX_ICON_IME_CHT_BIG5CODE				= 42;	// IME��	- �߹���ü	- Big5 Code
const	DWORD	DF_INDEX_ICON_IME_CHT_CHANGJIE				= 43;	// IME��	- �߹���ü	- ChangJie
const	DWORD	DF_INDEX_ICON_IME_CHT_DAYI					= 44;	// IME��	- �߹���ü	- Dayi
const	DWORD	DF_INDEX_ICON_IME_CHT_NEWCHANGJIE			= 45;	// IME��	- �߹���ü	- New ChangJie
const	DWORD	DF_INDEX_ICON_IME_CHT_PONETIC				= 46;	// IME��	- �߹���ü	- Ponetic
const	DWORD	DF_INDEX_ICON_IME_CHT_QUICK					= 47;	// IME��	- �߹���ü	- Quick
const	DWORD	DF_INDEX_ICON_IME_CHT_UNICODE				= 48;	// IME��	- �߹���ü	- Unicode
const	DWORD	DF_INDEX_ICON_IME_CHT_NEWPONETIC			= 49;	// IME��	- �߹���ü	- New Ponetic


const	DWORD	DF_VALUE_COLOR_READINGSTRING		= RGB(250, 250, 250);	// ����â �ؽ�Ʈ ����
const	DWORD	DF_VALUE_COLOR_CANDIDATE_SELECTED	= RGB(0, 0, 250);		// ���õ� �ؽ�Ʈ ����
const	DWORD	DF_VALUE_COLOR_CANDIDATE_NORMAL		= RGB(250, 250, 250);	// 1�� ����â �ؽ�Ʈ ����
const	DWORD	DF_VALUE_COLOR_CANDIDATE_EX			= RGB(0, 250, 0);		// 2�� ����â �ؽ�Ʈ ����

cltOnlineWorld*		OnlineInputStatusControl::m_pOnlineWorld			=	NULL;

XSPR				OnlineInputStatusControl::m_sprImeIconBase			= {0,};
XSPR				OnlineInputStatusControl::m_sprImeIcon				= {0,};
XSPR				OnlineInputStatusControl::m_sprCandidate			= {0,};
XSPR				OnlineInputStatusControl::m_sprReadingString		= {0,};


//-----------------------------------------------------------------------------
// Name: OnlineInputStatusControl()
// Code: actdoll (2004-11-01)
// Desc: ������
//-----------------------------------------------------------------------------
OnlineInputStatusControl::OnlineInputStatusControl()
{

}

//-----------------------------------------------------------------------------
// Name: ~OnlineInputStatusControl()
// Code: actdoll (2004-11-01)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
OnlineInputStatusControl::~OnlineInputStatusControl()
{

}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-11-01)
// Desc: �ʱ�ȭ �κ�, �ѹ��� ���ش�.
//-----------------------------------------------------------------------------
void	OnlineInputStatusControl::Init( cltOnlineWorld *pOnlineWorld )
{
	// �ܺ� ���� �ޱ�
	m_pOnlineWorld			= pOnlineWorld;

	// ��������Ʈ �ε�
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_IconBase.Spr",			m_sprImeIconBase );
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_Icon.Spr",				m_sprImeIcon );
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_CandidateWindow.Spr",	m_sprCandidate );
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_ReadingWindow.Spr",		m_sprReadingString );
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: actdoll (2004-11-01)
// Desc: ������ ���� �κ�, �ѹ��� ���ش�.
//-----------------------------------------------------------------------------
void	OnlineInputStatusControl::Free()
{
	if( m_sprImeIconBase.Image )
	{
		clGrp.FreeXspr( m_sprImeIconBase );
		ZeroMemory( &m_sprImeIconBase, sizeof(m_sprImeIconBase) );
	}
	if( m_sprImeIcon.Image )
	{
		clGrp.FreeXspr( m_sprImeIcon );
		ZeroMemory( &m_sprImeIcon, sizeof(m_sprImeIcon) );
	}
	if( m_sprCandidate.Image )
	{
		clGrp.FreeXspr( m_sprCandidate );
		ZeroMemory( &m_sprCandidate, sizeof(m_sprCandidate) );
	}
	if( m_sprReadingString.Image )
	{
		clGrp.FreeXspr( m_sprReadingString );
		ZeroMemory( &m_sprReadingString, sizeof(m_sprReadingString) );
	}
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-11-01)
// Desc: ���
//-----------------------------------------------------------------------------
void	OnlineInputStatusControl::Draw( LPDIRECTDRAWSURFACE7 lpSurface )
{
	HDC		hDC;
	POINT	ptIconBase;
	POINT	ptLocaleIcon;
	POINT	ptImeIcon;
	POINT	ptReadingString;
	POINT	ptCandidate;
	int		iReadingBarCount;
	int		iCandidateBarCount;
	BOOL	bEnableDrawCandidate;
	DWORD	dwCurCandidateColor;
	char	pszCandidateBuff[256];

	// �ϴ� ���� ��Ŀ���� ������ ����Ʈ ��Ʈ���� ã�´�.
	OnlineEditControlEx		*pOecEx;
	pOecEx = (OnlineEditControlEx*)Handler_OnlineEditControlEx::GetFocus();
	if( !pOecEx )						return;			// ��Ŀ���� ���ٸ� ����
	if( pOecEx->IsTypeOnlyNumeric() )	return;			// ���� ��ġ �Է¸� �����ϴٸ� ����

	// ���� ������ IME�� �Ϻ� �����̶�� ����â�� �ȱ׷��ش�.
	bEnableDrawCandidate	= true;

	//----------------
	//	��ǥ�� ����
	//----------------
	// ������ ���̽� ����(�̰��� ���ϴ� �� ���̴�.)
	ptIconBase.x	= 3;
	ptIconBase.y	= ON_SCREEN_YSIZE - m_sprImeIconBase.Header.Ysize - 23;
	// �Է� ������ ������ ����(�̰��� ���ϴ� �� ���̴�.)
	ptLocaleIcon.x	= ptIconBase.x + 2;
	ptLocaleIcon.y	= ptIconBase.y + 2;
	// IME ������ ����(�Է� ������ ������ �ٷ� ���� ����)
	ptImeIcon.x		= ptLocaleIcon.x + m_sprImeIcon.Header.Xsize;
	ptImeIcon.y		= ptLocaleIcon.y;
	// ����â ũ�� ����
	if( GS_IMEBase::IsOpenReadingString() )
	{
		// �� �κ��� ����Ʈ �ڽ��� �ص��� �ѾƴٴϹǷ� ������ �ణ Ʋ����.
		POINT	*ptTemp	= pOecEx->GetCaretDrawPos();
		// ���� �̳��� �߹� ��ü ���� ABC�Է¹��̶�� ����� �÷��ش�.
		if( GS_IMEBase::GetCurrentKeyboardLocale() == DF_VALUE_HKL_CHS_INTELABC )
		{
			if( lpSurface->GetDC( &hDC ) == DD_OK )
			{
				// ��Ʈ ����
				::SelectObject( hDC, m_pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
				::SetBkMode( hDC, TRANSPARENT );
				
				// ����â ���ڼ� ������
				SIZE	TextLen;
				GetTextExtentPoint32( hDC, GS_IMEBase::GetReadingStringData(), strlen(GS_IMEBase::GetReadingStringData()), &TextLen );
				
				// �ִ� ���̿� �ʿ��� ����â ĭ ������ ����Ѵ�.
				iReadingBarCount	= ( TextLen.cx / m_sprReadingString.Header.Xsize ) + 2;
				if( iReadingBarCount < 3 )	iReadingBarCount = 3;
				
				lpSurface->ReleaseDC( hDC );
			}
		}
		// �װ� �ƴ϶��
		else
		{
			iReadingBarCount	= 3;
		}
		ptReadingString.x	= ( ptTemp->x + ( m_sprReadingString.Header.Xsize * iReadingBarCount ) < ON_SCREEN_XSIZE ) 
			? ptTemp->x : ON_SCREEN_XSIZE - ( m_sprReadingString.Header.Xsize * iReadingBarCount ) - 1;
		ptReadingString.y	= ( ptTemp->y + ( m_sprReadingString.Header.Ysize << 1 ) < ON_SCREEN_YSIZE ) 
			? ptTemp->y + m_sprReadingString.Header.Ysize : ptTemp->y - m_sprReadingString.Header.Ysize - 6;
	}
	// ����â ������ ����
	if( bEnableDrawCandidate && GS_IMEBase::IsOpenCandidate() )
	{
		SIZE	TextLen;
		char	*pszCandidate;
		int		iMaxWidth = 0;
		if( lpSurface->GetDC( &hDC ) == DD_OK )
		{
			// ��Ʈ ����
			::SelectObject( hDC, m_pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
			::SetBkMode( hDC, TRANSPARENT );

			// ���빰�� ��Ʈ�� �����͸� Ȯ���Ѵ�.
			for( DWORD index=0; index<GS_IMEBase::GetCandidateCount(); index++ )
			{
				// ��Ʈ�� ������ �߿��� ���� �� ���� ã�´�.
				if( pszCandidate = (char*)GS_IMEBase::GetCandidateString( index ) )
				{
					GetTextExtentPoint32( hDC, pszCandidate, strlen(pszCandidate), &TextLen );
					if( TextLen.cx > iMaxWidth )	iMaxWidth = TextLen.cx;
				}
			}

			// �ִ� ���̿� �ʿ��� ����â ĭ ������ ����Ѵ�
			iCandidateBarCount	= iMaxWidth / m_sprCandidate.Header.Xsize;

			ptCandidate.x	= ON_SCREEN_XSIZE - ( (iCandidateBarCount + 2) * m_sprCandidate.Header.Xsize ) - 3;	// ��ũ�� �ִ�ũ�� - ��µ� ĭ ������ ����
			ptCandidate.y	= ON_SCREEN_YSIZE - m_sprCandidate.Header.Ysize - 23;								// ��ũ�� �ִ�ũ�� - ĭ������

			lpSurface->ReleaseDC( hDC );
		}
	}

	//----------------
	//	�׸� ���
	//----------------
	if( clGrp.LockSurface( lpSurface ) )
	{
		DWORD	dwFontNo;
		// ������ ���̽� ǥ��
		clGrp.PutSpriteT(	ptIconBase.x,					ptIconBase.y, 
							m_sprImeIconBase.Header.Xsize,	m_sprImeIconBase.Header.Ysize, 
							m_sprImeIconBase.Image );

		// �Է� ������ �����׸� ǥ��
		dwFontNo	= GetCurrentLocaleIconIndex();
		clGrp.PutSpriteT(	ptLocaleIcon.x,				ptLocaleIcon.y, 
							m_sprImeIcon.Header.Xsize,	m_sprImeIcon.Header.Ysize, 
							&m_sprImeIcon.Image[m_sprImeIcon.Header.Start[dwFontNo]] );

		// IME ������ ǥ��
		dwFontNo = GetCurrentImeIconIndex();
		clGrp.PutSpriteT(	ptImeIcon.x,				ptImeIcon.y, 
							m_sprImeIcon.Header.Xsize,	m_sprImeIcon.Header.Ysize, 
							&m_sprImeIcon.Image[m_sprImeIcon.Header.Start[dwFontNo]] );

		// ����â ���
		if( GS_IMEBase::IsOpenReadingString() )
		{
			// �� ������ ���
			clGrp.PutSpriteT(	ptReadingString.x,					ptReadingString.y, 
								m_sprReadingString.Header.Xsize,	m_sprReadingString.Header.Ysize, 
								&m_sprReadingString.Image[m_sprReadingString.Header.Start[0]] );

			// ��µ� ������ŭ ǥ��
			for( DWORD index=0; index<iReadingBarCount-2; index++ )
			{
				clGrp.PutSpriteT(	ptReadingString.x + ( (index+1) * m_sprReadingString.Header.Xsize ),	ptReadingString.y, 
									m_sprReadingString.Header.Xsize,										m_sprReadingString.Header.Ysize, 
									&m_sprReadingString.Image[m_sprReadingString.Header.Start[1]] );
			}

			// �� ������ ���
			clGrp.PutSpriteT(	ptReadingString.x + ( (index+1) * m_sprReadingString.Header.Xsize ),		ptReadingString.y, 
								m_sprReadingString.Header.Xsize,											m_sprReadingString.Header.Ysize, 
								&m_sprReadingString.Image[m_sprReadingString.Header.Start[2]] );
		}
		
		// ����â ���
		if( bEnableDrawCandidate && GS_IMEBase::IsOpenCandidate() )
		{
			// �� ������ ���
			clGrp.PutSpriteT(	ptCandidate.x,					ptCandidate.y, 
								m_sprCandidate.Header.Xsize,	m_sprCandidate.Header.Ysize, 
								&m_sprCandidate.Image[m_sprCandidate.Header.Start[0]] );

			// ��µ� ������ŭ ǥ��
			for( DWORD index=0; index<iCandidateBarCount; index++ )
			{
				clGrp.PutSpriteT(	ptCandidate.x + ( (index+1) * m_sprCandidate.Header.Xsize ),	ptCandidate.y, 
									m_sprCandidate.Header.Xsize,									m_sprCandidate.Header.Ysize, 
									&m_sprCandidate.Image[m_sprCandidate.Header.Start[1]] );
			}

			// �� ������ ���
			clGrp.PutSpriteT(	ptCandidate.x + ( (index+1) * m_sprCandidate.Header.Xsize ),		ptCandidate.y, 
								m_sprCandidate.Header.Xsize,										m_sprCandidate.Header.Ysize, 
								&m_sprCandidate.Image[m_sprCandidate.Header.Start[2]] );

		}
		clGrp.UnlockSurface( lpSurface );
	}

	//----------------
	//	�ؽ�Ʈ ���
	//----------------
	if( lpSurface->GetDC( &hDC ) == DD_OK )
	{
		// ��Ʈ ����
		::SelectObject( hDC, m_pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
		::SetBkMode( hDC, TRANSPARENT );

		// ����â ���
		if( GS_IMEBase::IsOpenReadingString() )
		{
			m_pOnlineWorld->pOnlineMegaText->DrawText( hDC, ptReadingString.x + 5, ptReadingString.y + 4, 
														(char*)GS_IMEBase::GetReadingStringData(), DF_VALUE_COLOR_READINGSTRING );
		}

		// ����â ���
		if( bEnableDrawCandidate && GS_IMEBase::IsOpenCandidate() )
		{
			// ��µ� ������ŭ ǥ��
			for( DWORD index=0; index<GS_IMEBase::GetCandidateCount(); index++ )
			{
				dwCurCandidateColor		= DF_VALUE_COLOR_CANDIDATE_NORMAL;
				// ���� 2�� ����â�� ���
				if( GS_IMEBase::IsCandidateExType() )				dwCurCandidateColor	= DF_VALUE_COLOR_CANDIDATE_EX;
				// ���� ���õ� ����â�� ���
				if( GS_IMEBase::GetCandidateSelection() == index )	dwCurCandidateColor	= DF_VALUE_COLOR_CANDIDATE_SELECTED;

				// ���� ���ս��Ѽ� ���
				sprintf( pszCandidateBuff, "%d", (index+1)%10 );

				// �ؽ�Ʈ ���
				m_pOnlineWorld->pOnlineMegaText->DrawText( hDC, ptCandidate.x + 7, ptCandidate.y + (index * 15) + 2, pszCandidateBuff, dwCurCandidateColor );

				// �ؽ�Ʈ ���
				m_pOnlineWorld->pOnlineMegaText->DrawText( hDC, ptCandidate.x + 20, ptCandidate.y + (index * 15) + 2, (char*)GS_IMEBase::GetCandidateString( index ), dwCurCandidateColor );
			}
		}
		lpSurface->ReleaseDC( hDC );
	}
}

//-----------------------------------------------------------------------------
// Name: GetCurrentLocaleIconIndex()								[private]
// Code: actdoll (2004-11-01)
// Desc: ���� �ε��� �Է±� ���¿� ���� �ش� ICON�� ��������Ʈ ��Ʈ��ȣ�� ���´�.
//-----------------------------------------------------------------------------
DWORD	OnlineInputStatusControl::GetCurrentLocaleIconIndex()
{
	// ���� IME ��ü�� ����� ���°� �ƴ϶�� �������� ������ ����
	if( !GS_IMEBase::IsImeRun() )			return DF_INDEX_ICON_LOCALE_ENGLISH;

	// �ѱ����� ���
	if( GS_IMEBase::IsLocaleKorean() )		return DF_INDEX_ICON_LOCALE_KOREAN;

	// �Ϻ����� ���
	if( GS_IMEBase::IsLocaleJapanese() )	return DF_INDEX_ICON_LOCALE_JAPANESE;

	// �߹� ������ ���
	if( GS_IMEBase::IsLocaleSimpChinese() )	return DF_INDEX_ICON_LOCALE_CHINESE_SIMP;

	// �߹� ������ ���
	if( GS_IMEBase::IsLocaleTradChinese() )	return DF_INDEX_ICON_LOCALE_CHINESE_TRAD;

	// �� �� ���� ���� ���
	return DF_INDEX_ICON_LOCALE_UNKNOWN;
}

//-----------------------------------------------------------------------------
// Name: GetCurrentImeIconIndex()									[private]
// Code: actdoll (2004-11-01)
// Desc: ���� �ε��� �Է±� ���¿� ���� �ش� ICON�� ��������Ʈ ��Ʈ��ȣ�� ���´�.
//-----------------------------------------------------------------------------
DWORD	OnlineInputStatusControl::GetCurrentImeIconIndex()
{
	// ���� IME ��ü�� ����� ���°� �ƴ϶�� �������� ������ ����
	if( !GS_IMEBase::IsImeRun() )			return DF_INDEX_ICON_IME_NONE;

	// �ѱ����� ���
	if( GS_IMEBase::IsLocaleKorean() )
	{
		// �ѱ�? ����?
		return ( ( !GS_IMEBase::IsInputAlphabet() ) ? DF_INDEX_ICON_IME_KOR_HANGUL : DF_INDEX_ICON_IME_KOR_ENGLISH );
	}

	// �Ϻ����� ���
	if( GS_IMEBase::IsLocaleJapanese() )
	{
		// �Ϻ��� �迭�� ���
		if( !GS_IMEBase::IsInputAlphabet() )
		{
			// īŸī��? ���󰡳�?
			return ( ( GS_IMEBase::IsInputKatakana() ) ? DF_INDEX_ICON_IME_JPN_KATAKANA : DF_INDEX_ICON_IME_JPN_HIRAGANA );
		}
		// ���� �Է��� ���
		else										return DF_INDEX_ICON_IME_JPN_ENGLISH;
	}

	// �߹� ������ ���
	HKL	hCurrentKL	= GS_IMEBase::GetCurrentKeyboardLocale();
	if( GS_IMEBase::IsLocaleTradChinese() )
	{
		// Ű���� ���̾ƿ��� ���� Ʋ����.
		if		( hCurrentKL == DF_VALUE_HKL_CHT_DEFAULT		)	return DF_INDEX_ICON_IME_CHT_DEFAULT;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_NEWPHONETIC	)	return DF_INDEX_ICON_IME_CHT_NEWPONETIC;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_NEWCHANGJIE	)	return DF_INDEX_ICON_IME_CHT_NEWCHANGJIE;
		else if	( hCurrentKL == DF_VALUE_HKL_CHT_PHONETIC		)	return DF_INDEX_ICON_IME_CHT_PONETIC;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_CHANGJIE		)	return DF_INDEX_ICON_IME_CHT_CHANGJIE;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_QUICK			)	return DF_INDEX_ICON_IME_CHT_QUICK;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_BIG5CODE		)	return DF_INDEX_ICON_IME_CHT_BIG5CODE;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_ARRAY			)	return DF_INDEX_ICON_IME_CHT_ARRAY;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_DAYI			)	return DF_INDEX_ICON_IME_CHT_DAYI;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_ALPHANUMERIC	)	return DF_INDEX_ICON_IME_CHT_ALPHANUMERIC;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_UNICODE		)	return DF_INDEX_ICON_IME_CHT_UNICODE;
		else if ( hCurrentKL == DF_VALUE_HKL_CHT_USKEYBOARD		)	return DF_INDEX_ICON_IME_CHT_USKEYBOARD;
	}

	// �߹� ������ ���
	if( GS_IMEBase::IsLocaleSimpChinese() )
	{
		if		( hCurrentKL == DF_VALUE_HKL_CHS_DEFAULT		)	return DF_INDEX_ICON_IME_CHS_DEFAULT;
		else if ( hCurrentKL == DF_VALUE_HKL_CHS_INTELABC		)	return DF_INDEX_ICON_IME_CHS_INTELABC;
		else if	( hCurrentKL == DF_VALUE_HKL_CHS_QUANPIN		)	return DF_INDEX_ICON_IME_CHS_QUANPIN;
		else if ( hCurrentKL == DF_VALUE_HKL_CHS_SHUANGPIN		)	return DF_INDEX_ICON_IME_CHS_SHUANGPIN;
		else if ( hCurrentKL == DF_VALUE_HKL_CHS_ZHENGMA		)	return DF_INDEX_ICON_IME_CHS_ZHENGMA;
		else if ( hCurrentKL == DF_VALUE_HKL_CHS_NEIMA			)	return DF_INDEX_ICON_IME_CHS_NEIMA;
		else if ( hCurrentKL == DF_VALUE_HKL_CHS_MSPY			)	return DF_INDEX_ICON_IME_CHS_MSPY;
		else if	( hCurrentKL == DF_VALUE_HKL_CHS_USKEYBOARD		)	return DF_INDEX_ICON_IME_CHS_USKEYBOARD;
	}

	// �� �� ���� ���� ���
	return DF_INDEX_ICON_IME_UNKNOWN;
}
