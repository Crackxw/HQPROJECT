//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 입력기 상태 표시기
//	File Name		: OnlineInputStatusControl.cpp
//	Birth Date		: 2004. 11. 01.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		입력 상태에 대한 표시를 전달하는 클래스입니다.(한글/영문/중문 등)
//		출력 내역은 한/영 등의 입력 언어 표시, 숙어창, 발음창 등을 표시합니다.
//		출력 시 출력단의 가장 상단에 표시되어야 하는 놈입니다.
//		GS_IMEBase가 반드시 포함되어야 합니다.
//
//===================================================================================================

#include <GSL.h>
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
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
// Desc: 정의들
//-----------------------------------------------------------------------------
const	DWORD	DF_INDEX_ICON_LOCALE_KOREAN					= 0;	// 로케일값	- 한국어
const	DWORD	DF_INDEX_ICON_LOCALE_JAPANESE				= 1;	// 로케일값	- 일본어
const	DWORD	DF_INDEX_ICON_LOCALE_CHINESE_SIMP			= 2;	// 로케일값	- 중국어 간체
const	DWORD	DF_INDEX_ICON_LOCALE_CHINESE_TRAD			= 3;	// 로케일값	- 중국어 번체
const	DWORD	DF_INDEX_ICON_LOCALE_ENGLISH				= 4;	// 로케일값	- 영문/설치안됨
const	DWORD	DF_INDEX_ICON_LOCALE_UNKNOWN				= 9;	// 로케일값	- 판독안됨
	
const	DWORD	DF_INDEX_ICON_IME_NONE						= 8;	// IME값	- 설치안됨
const	DWORD	DF_INDEX_ICON_IME_UNKNOWN					= 9;	// IME값	- 알 수 없음

const	DWORD	DF_INDEX_ICON_IME_KOR_ENGLISH				= 10;	// IME값	- 한국어	- 영문
const	DWORD	DF_INDEX_ICON_IME_KOR_HANGUL				= 11;	// IME값	- 한국어	- 한글
const	DWORD	DF_INDEX_ICON_IME_KOR_HANJA					= 12;	// IME값	- 한국어	- 한자

const	DWORD	DF_INDEX_ICON_IME_JPN_ENGLISH				= 20;	// IME값	- 일본어	- 영문
const	DWORD	DF_INDEX_ICON_IME_JPN_HIRAGANA				= 21;	// IME값	- 일본어	- 히라가나
const	DWORD	DF_INDEX_ICON_IME_JPN_KATAKANA				= 22;	// IME값	- 일본어	- 카타카나

const	DWORD	DF_INDEX_ICON_IME_CHS_DEFAULT				= 8;	// IME값	- 중문간체	- Default
const	DWORD	DF_INDEX_ICON_IME_CHS_USKEYBOARD			= 8;	// IME값	- 중문간체	- 번체 US Keyboard
const	DWORD	DF_INDEX_ICON_IME_CHS_NEIMA					= 30;	// IME값	- 중문간체	- Neima
const	DWORD	DF_INDEX_ICON_IME_CHS_QUANPIN				= 31;	// IME값	- 중문간체	- QuanPin
const	DWORD	DF_INDEX_ICON_IME_CHS_SHUANGPIN				= 32;	// IME값	- 중문간체	- ShuangPin
const	DWORD	DF_INDEX_ICON_IME_CHS_ZHENGMA				= 33;	// IME값	- 중문간체	- ZhengMa
const	DWORD	DF_INDEX_ICON_IME_CHS_MSPY					= 34;	// IME값	- 중문간체	- Microsoft Pinyin IME 3.0
const	DWORD	DF_INDEX_ICON_IME_CHS_INTELABC				= 35;	// IME값	- 중문간체	- 지능 ABC

const	DWORD	DF_INDEX_ICON_IME_CHT_DEFAULT				= 8;	// IME값	- 중문번체	- Default
const	DWORD	DF_INDEX_ICON_IME_CHT_USKEYBOARD			= 8;	// IME값	- 중문번체	- 간체 US Keyboard
const	DWORD	DF_INDEX_ICON_IME_CHT_ALPHANUMERIC			= 40;	// IME값	- 중문번체	- AlphaNumeric
const	DWORD	DF_INDEX_ICON_IME_CHT_ARRAY					= 41;	// IME값	- 중문번체	- Array
const	DWORD	DF_INDEX_ICON_IME_CHT_BIG5CODE				= 42;	// IME값	- 중문번체	- Big5 Code
const	DWORD	DF_INDEX_ICON_IME_CHT_CHANGJIE				= 43;	// IME값	- 중문번체	- ChangJie
const	DWORD	DF_INDEX_ICON_IME_CHT_DAYI					= 44;	// IME값	- 중문번체	- Dayi
const	DWORD	DF_INDEX_ICON_IME_CHT_NEWCHANGJIE			= 45;	// IME값	- 중문번체	- New ChangJie
const	DWORD	DF_INDEX_ICON_IME_CHT_PONETIC				= 46;	// IME값	- 중문번체	- Ponetic
const	DWORD	DF_INDEX_ICON_IME_CHT_QUICK					= 47;	// IME값	- 중문번체	- Quick
const	DWORD	DF_INDEX_ICON_IME_CHT_UNICODE				= 48;	// IME값	- 중문번체	- Unicode
const	DWORD	DF_INDEX_ICON_IME_CHT_NEWPONETIC			= 49;	// IME값	- 중문번체	- New Ponetic


const	DWORD	DF_VALUE_COLOR_READINGSTRING		= RGB(250, 250, 250);	// 발음창 텍스트 색깔
const	DWORD	DF_VALUE_COLOR_CANDIDATE_SELECTED	= RGB(0, 0, 250);		// 선택된 텍스트 색깔
const	DWORD	DF_VALUE_COLOR_CANDIDATE_NORMAL		= RGB(250, 250, 250);	// 1차 숙어창 텍스트 색깔
const	DWORD	DF_VALUE_COLOR_CANDIDATE_EX			= RGB(0, 250, 0);		// 2차 숙어창 텍스트 색깔

cltOnlineWorld*		OnlineInputStatusControl::m_pOnlineWorld			=	NULL;

XSPR				OnlineInputStatusControl::m_sprImeIconBase			= {0,};
XSPR				OnlineInputStatusControl::m_sprImeIcon				= {0,};
XSPR				OnlineInputStatusControl::m_sprCandidate			= {0,};
XSPR				OnlineInputStatusControl::m_sprReadingString		= {0,};


//-----------------------------------------------------------------------------
// Name: OnlineInputStatusControl()
// Code: actdoll (2004-11-01)
// Desc: 생성자
//-----------------------------------------------------------------------------
OnlineInputStatusControl::OnlineInputStatusControl()
{

}

//-----------------------------------------------------------------------------
// Name: ~OnlineInputStatusControl()
// Code: actdoll (2004-11-01)
// Desc: 소멸자
//-----------------------------------------------------------------------------
OnlineInputStatusControl::~OnlineInputStatusControl()
{

}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-11-01)
// Desc: 초기화 부분, 한번만 해준다.
//-----------------------------------------------------------------------------
void	OnlineInputStatusControl::Init( cltOnlineWorld *pOnlineWorld )
{
	// 외부 정보 받기
	m_pOnlineWorld			= pOnlineWorld;

	// 스프라이트 로딩
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_IconBase.Spr",			m_sprImeIconBase );
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_Icon.Spr",				m_sprImeIcon );
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_CandidateWindow.Spr",	m_sprCandidate );
	clGrp.LoadXspr( "Online\\GameSpr\\IME\\IME_ReadingWindow.Spr",		m_sprReadingString );
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: actdoll (2004-11-01)
// Desc: 데이터 해제 부분, 한번만 해준다.
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
// Desc: 출력
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

	// 일단 현재 포커스가 맞춰진 에디트 컨트롤을 찾는다.
	OnlineEditControlEx		*pOecEx;
	pOecEx = (OnlineEditControlEx*)Handler_OnlineEditControlEx::GetFocus();
	if( !pOecEx )						return;			// 포커스가 없다면 리턴
	if( pOecEx->IsTypeOnlyNumeric() )	return;			// 오직 수치 입력만 가능하다면 리턴

	// 만약 현재의 IME가 일본 기준이라면 숙어창은 안그려준다.
	bEnableDrawCandidate	= true;

	//----------------
	//	좌표계 설정
	//----------------
	// 아이콘 베이스 세팅(이것은 좌하단 맨 끝이다.)
	ptIconBase.x	= 3;
	ptIconBase.y	= ON_SCREEN_YSIZE - m_sprImeIconBase.Header.Ysize - 23;
	// 입력 로케일 아이콘 세팅(이것은 좌하단 맨 끝이다.)
	ptLocaleIcon.x	= ptIconBase.x + 2;
	ptLocaleIcon.y	= ptIconBase.y + 2;
	// IME 아이콘 세팅(입력 로케일 아이콘 바로 옆에 세팅)
	ptImeIcon.x		= ptLocaleIcon.x + m_sprImeIcon.Header.Xsize;
	ptImeIcon.y		= ptLocaleIcon.y;
	// 발음창 크기 세팅
	if( GS_IMEBase::IsOpenReadingString() )
	{
		// 이 부분은 에디트 박스의 밑둥을 쫓아다니므로 계산식이 약간 틀리다.
		POINT	*ptTemp	= pOecEx->GetCaretDrawPos();
		// 만약 이놈이 중문 간체 지능 ABC입력법이라면 사이즈를 늘려준다.
		if( GS_IMEBase::GetCurrentKeyboardLocale() == DF_VALUE_HKL_CHS_INTELABC )
		{
			if( lpSurface->GetDC( &hDC ) == DD_OK )
			{
				// 폰트 세팅
				::SelectObject( hDC, m_pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
				::SetBkMode( hDC, TRANSPARENT );
				
				// 발음창 글자수 얻어오고
				SIZE	TextLen;
				GetTextExtentPoint32( hDC, GS_IMEBase::GetReadingStringData(), strlen(GS_IMEBase::GetReadingStringData()), &TextLen );
				
				// 최대 길이에 필요한 발음창 칸 갯수를 계산한다.
				iReadingBarCount	= ( TextLen.cx / m_sprReadingString.Header.Xsize ) + 2;
				if( iReadingBarCount < 3 )	iReadingBarCount = 3;
				
				lpSurface->ReleaseDC( hDC );
			}
		}
		// 그게 아니라면
		else
		{
			iReadingBarCount	= 3;
		}
		ptReadingString.x	= ( ptTemp->x + ( m_sprReadingString.Header.Xsize * iReadingBarCount ) < ON_SCREEN_XSIZE ) 
			? ptTemp->x : ON_SCREEN_XSIZE - ( m_sprReadingString.Header.Xsize * iReadingBarCount ) - 1;
		ptReadingString.y	= ( ptTemp->y + ( m_sprReadingString.Header.Ysize << 1 ) < ON_SCREEN_YSIZE ) 
			? ptTemp->y + m_sprReadingString.Header.Ysize : ptTemp->y - m_sprReadingString.Header.Ysize - 6;
	}
	// 숙어창 사이즈 세팅
	if( bEnableDrawCandidate && GS_IMEBase::IsOpenCandidate() )
	{
		SIZE	TextLen;
		char	*pszCandidate;
		int		iMaxWidth = 0;
		if( lpSurface->GetDC( &hDC ) == DD_OK )
		{
			// 폰트 세팅
			::SelectObject( hDC, m_pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
			::SetBkMode( hDC, TRANSPARENT );

			// 내용물의 스트링 데이터를 확인한다.
			for( DWORD index=0; index<GS_IMEBase::GetCandidateCount(); index++ )
			{
				// 스트링 데이터 중에서 가장 긴 놈을 찾는다.
				if( pszCandidate = (char*)GS_IMEBase::GetCandidateString( index ) )
				{
					GetTextExtentPoint32( hDC, pszCandidate, strlen(pszCandidate), &TextLen );
					if( TextLen.cx > iMaxWidth )	iMaxWidth = TextLen.cx;
				}
			}

			// 최대 길이에 필요한 숙어창 칸 갯수를 계산한다
			iCandidateBarCount	= iMaxWidth / m_sprCandidate.Header.Xsize;

			ptCandidate.x	= ON_SCREEN_XSIZE - ( (iCandidateBarCount + 2) * m_sprCandidate.Header.Xsize ) - 3;	// 스크린 최대크기 - 출력될 칸 갯수의 길이
			ptCandidate.y	= ON_SCREEN_YSIZE - m_sprCandidate.Header.Ysize - 23;								// 스크린 최대크기 - 칸사이즈

			lpSurface->ReleaseDC( hDC );
		}
	}

	//----------------
	//	그림 출력
	//----------------
	if( clGrp.LockSurface( lpSurface ) )
	{
		DWORD	dwFontNo;
		// 아이콘 베이스 표시
		clGrp.PutSpriteT(	ptIconBase.x,					ptIconBase.y, 
							m_sprImeIconBase.Header.Xsize,	m_sprImeIconBase.Header.Ysize, 
							m_sprImeIconBase.Image );

		// 입력 로케일 바탕그림 표시
		dwFontNo	= GetCurrentLocaleIconIndex();
		clGrp.PutSpriteT(	ptLocaleIcon.x,				ptLocaleIcon.y, 
							m_sprImeIcon.Header.Xsize,	m_sprImeIcon.Header.Ysize, 
							&m_sprImeIcon.Image[m_sprImeIcon.Header.Start[dwFontNo]] );

		// IME 아이콘 표시
		dwFontNo = GetCurrentImeIconIndex();
		clGrp.PutSpriteT(	ptImeIcon.x,				ptImeIcon.y, 
							m_sprImeIcon.Header.Xsize,	m_sprImeIcon.Header.Ysize, 
							&m_sprImeIcon.Image[m_sprImeIcon.Header.Start[dwFontNo]] );

		// 발음창 출력
		if( GS_IMEBase::IsOpenReadingString() )
		{
			// 맨 좌측부 출력
			clGrp.PutSpriteT(	ptReadingString.x,					ptReadingString.y, 
								m_sprReadingString.Header.Xsize,	m_sprReadingString.Header.Ysize, 
								&m_sprReadingString.Image[m_sprReadingString.Header.Start[0]] );

			// 출력된 갯수만큼 표기
			for( DWORD index=0; index<iReadingBarCount-2; index++ )
			{
				clGrp.PutSpriteT(	ptReadingString.x + ( (index+1) * m_sprReadingString.Header.Xsize ),	ptReadingString.y, 
									m_sprReadingString.Header.Xsize,										m_sprReadingString.Header.Ysize, 
									&m_sprReadingString.Image[m_sprReadingString.Header.Start[1]] );
			}

			// 맨 우측부 출력
			clGrp.PutSpriteT(	ptReadingString.x + ( (index+1) * m_sprReadingString.Header.Xsize ),		ptReadingString.y, 
								m_sprReadingString.Header.Xsize,											m_sprReadingString.Header.Ysize, 
								&m_sprReadingString.Image[m_sprReadingString.Header.Start[2]] );
		}
		
		// 숙어창 출력
		if( bEnableDrawCandidate && GS_IMEBase::IsOpenCandidate() )
		{
			// 맨 좌측부 출력
			clGrp.PutSpriteT(	ptCandidate.x,					ptCandidate.y, 
								m_sprCandidate.Header.Xsize,	m_sprCandidate.Header.Ysize, 
								&m_sprCandidate.Image[m_sprCandidate.Header.Start[0]] );

			// 출력된 갯수만큼 표기
			for( DWORD index=0; index<iCandidateBarCount; index++ )
			{
				clGrp.PutSpriteT(	ptCandidate.x + ( (index+1) * m_sprCandidate.Header.Xsize ),	ptCandidate.y, 
									m_sprCandidate.Header.Xsize,									m_sprCandidate.Header.Ysize, 
									&m_sprCandidate.Image[m_sprCandidate.Header.Start[1]] );
			}

			// 맨 우측부 출력
			clGrp.PutSpriteT(	ptCandidate.x + ( (index+1) * m_sprCandidate.Header.Xsize ),		ptCandidate.y, 
								m_sprCandidate.Header.Xsize,										m_sprCandidate.Header.Ysize, 
								&m_sprCandidate.Image[m_sprCandidate.Header.Start[2]] );

		}
		clGrp.UnlockSurface( lpSurface );
	}

	//----------------
	//	텍스트 출력
	//----------------
	if( lpSurface->GetDC( &hDC ) == DD_OK )
	{
		// 폰트 세팅
		::SelectObject( hDC, m_pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
		::SetBkMode( hDC, TRANSPARENT );

		// 발음창 출력
		if( GS_IMEBase::IsOpenReadingString() )
		{
			m_pOnlineWorld->pOnlineMegaText->DrawText( hDC, ptReadingString.x + 5, ptReadingString.y + 4, 
														(char*)GS_IMEBase::GetReadingStringData(), DF_VALUE_COLOR_READINGSTRING );
		}

		// 숙어창 출력
		if( bEnableDrawCandidate && GS_IMEBase::IsOpenCandidate() )
		{
			// 출력된 갯수만큼 표기
			for( DWORD index=0; index<GS_IMEBase::GetCandidateCount(); index++ )
			{
				dwCurCandidateColor		= DF_VALUE_COLOR_CANDIDATE_NORMAL;
				// 만약 2차 숙어창일 경우
				if( GS_IMEBase::IsCandidateExType() )				dwCurCandidateColor	= DF_VALUE_COLOR_CANDIDATE_EX;
				// 만약 선택된 숙어창일 경우
				if( GS_IMEBase::GetCandidateSelection() == index )	dwCurCandidateColor	= DF_VALUE_COLOR_CANDIDATE_SELECTED;

				// 숫자 조합시켜서 찍고
				sprintf( pszCandidateBuff, "%d", (index+1)%10 );

				// 텍스트 출력
				m_pOnlineWorld->pOnlineMegaText->DrawText( hDC, ptCandidate.x + 7, ptCandidate.y + (index * 15) + 2, pszCandidateBuff, dwCurCandidateColor );

				// 텍스트 출력
				m_pOnlineWorld->pOnlineMegaText->DrawText( hDC, ptCandidate.x + 20, ptCandidate.y + (index * 15) + 2, (char*)GS_IMEBase::GetCandidateString( index ), dwCurCandidateColor );
			}
		}
		lpSurface->ReleaseDC( hDC );
	}
}

//-----------------------------------------------------------------------------
// Name: GetCurrentLocaleIconIndex()								[private]
// Code: actdoll (2004-11-01)
// Desc: 현재 로딩된 입력기 상태에 따라 해당 ICON의 스프라이트 폰트번호를 얻어온다.
//-----------------------------------------------------------------------------
DWORD	OnlineInputStatusControl::GetCurrentLocaleIconIndex()
{
	// 만약 IME 자체가 적재된 상태가 아니라면 아이콘은 무조건 영문
	if( !GS_IMEBase::IsImeRun() )			return DF_INDEX_ICON_LOCALE_ENGLISH;

	// 한국어일 경우
	if( GS_IMEBase::IsLocaleKorean() )		return DF_INDEX_ICON_LOCALE_KOREAN;

	// 일본어일 경우
	if( GS_IMEBase::IsLocaleJapanese() )	return DF_INDEX_ICON_LOCALE_JAPANESE;

	// 중문 간자일 경우
	if( GS_IMEBase::IsLocaleSimpChinese() )	return DF_INDEX_ICON_LOCALE_CHINESE_SIMP;

	// 중문 번자일 경우
	if( GS_IMEBase::IsLocaleTradChinese() )	return DF_INDEX_ICON_LOCALE_CHINESE_TRAD;

	// 알 수 없는 놈일 경우
	return DF_INDEX_ICON_LOCALE_UNKNOWN;
}

//-----------------------------------------------------------------------------
// Name: GetCurrentImeIconIndex()									[private]
// Code: actdoll (2004-11-01)
// Desc: 현재 로딩된 입력기 상태에 따라 해당 ICON의 스프라이트 폰트번호를 얻어온다.
//-----------------------------------------------------------------------------
DWORD	OnlineInputStatusControl::GetCurrentImeIconIndex()
{
	// 만약 IME 자체가 적재된 상태가 아니라면 아이콘은 무조건 영문
	if( !GS_IMEBase::IsImeRun() )			return DF_INDEX_ICON_IME_NONE;

	// 한국어일 경우
	if( GS_IMEBase::IsLocaleKorean() )
	{
		// 한글? 영문?
		return ( ( !GS_IMEBase::IsInputAlphabet() ) ? DF_INDEX_ICON_IME_KOR_HANGUL : DF_INDEX_ICON_IME_KOR_ENGLISH );
	}

	// 일본어일 경우
	if( GS_IMEBase::IsLocaleJapanese() )
	{
		// 일본어 계열일 경우
		if( !GS_IMEBase::IsInputAlphabet() )
		{
			// 카타카나? 히라가나?
			return ( ( GS_IMEBase::IsInputKatakana() ) ? DF_INDEX_ICON_IME_JPN_KATAKANA : DF_INDEX_ICON_IME_JPN_HIRAGANA );
		}
		// 영문 입력일 경우
		else										return DF_INDEX_ICON_IME_JPN_ENGLISH;
	}

	// 중문 번자일 경우
	HKL	hCurrentKL	= GS_IMEBase::GetCurrentKeyboardLocale();
	if( GS_IMEBase::IsLocaleTradChinese() )
	{
		// 키보드 레이아웃에 따라 틀리다.
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

	// 중문 간자일 경우
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

	// 알 수 없는 놈일 경우
	return DF_INDEX_ICON_IME_UNKNOWN;
}
