/********************************************************************************************
	파일명	:	OnlineHelp.cpp
	작성자	:	정 진 욱
	담당자  :	정 진 욱
	작성일	:	2001.07.23
	수정일	:	2002.03.16
	내  용  :	도움말 출력
********************************************************************************************/


#include <GSL.h>
//#include "Main.h"		// actdoll (2004/03/18 13:31) : 없어도 컴파일이 되는 헤더. 진짜 필요없던가 상위 헤더 내에 선언되어 있는 경우임. 이것 때문에 문제 발생시 다시 켜기 바람


#include "OnlineWorld.h"
#include "OnlineItem.h"
#include "OnlineHelp.h"
#include "OnlineTrade.h"
#include "DebugTool.h"
#include "OnlineMegaText.h"
#include "OnlineETC.h"
#include "OnlineFont.h"
#include "OnlineMyData.h"
#include "OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineProductStructure.h"
#include "OnlineVillage-Parser.h"
#include "OnlineVillageManager.h"
#include "OnlineKeyboard.h"
#include "OnlineBooth.h"
#include "OnlineMercenary.h"			// actdoll (2004/01/13 13:47) : 추가
#include "onlinevillagestructmanager.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"


#define ON_HELP_MAX_STRING				1000

#define ON_HELP_WIDTH					240
#define ON_HELP_REALWIDTH				230
#define ON_HELP_HEIGHT					600
#define	ON_HELP_BACK_COLOR				0


OnlineHelp::OnlineHelp()
{
	siLine		=	0;
	siDirection	=	0;
	bAction		=	FALSE;

	m_lpSelectCharacterFlagRenderPosX = NULL;
	m_lpSelectCharacterFlagRenderPosY = NULL;
}

OnlineHelp::~OnlineHelp()
{
	FreeHelp();
}
/*********************************************************************************************
	함수명	:	Init
	내  용	:	도움말 클래스를 초기화 한다.
*********************************************************************************************/
BOOL	OnlineHelp::Init( cltOnlineWorld *pOnlieWorld )
{
	pMyOnlineWorld	=	pOnlieWorld;	
	int		 nUsage =	DIB_RGB_COLORS;
	
	MegaDC = ::CreateCompatibleDC(NULL);

	m_pBitInfo = (BITMAPINFO*)new LPBITMAPINFO[ sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256) ];

	ZeroMemory(m_pBitInfo, sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256));
	m_pBitInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	m_pBitInfo->bmiHeader.biWidth		= ON_HELP_WIDTH;
	m_pBitInfo->bmiHeader.biHeight		= -ON_HELP_HEIGHT;
	m_pBitInfo->bmiHeader.biBitCount	= 8;
	m_pBitInfo->bmiHeader.biCompression	= BI_RGB;
	m_pBitInfo->bmiHeader.biPlanes		= 1;

	for( SI16 i=0 ; i<256 ; i++)
	{
		m_pBitInfo->bmiColors[i].rgbRed		= clGrp.Palette[i * 3 + 0] * 4 ;
		m_pBitInfo->bmiColors[i].rgbGreen	= clGrp.Palette[i * 3 + 1] * 4 ;
		m_pBitInfo->bmiColors[i].rgbBlue	= clGrp.Palette[i * 3 + 2] * 4 ;
	}

	SetBkMode( MegaDC, TRANSPARENT );
	SelectObject( MegaDC, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );

	hBmp		= ::CreateDIBSection(MegaDC, m_pBitInfo, nUsage, (void **)&pTextBuffer, NULL, 0);
	hOldBmp		= (HBITMAP)SelectObject(MegaDC, hBmp);

	return TRUE;
}
/*********************************************************************************************
	함수명	:	Init
	내  용	:	도움말 클래스 날린다.
*********************************************************************************************/
VOID	OnlineHelp::FreeHelp()
{
	if( MegaDC )
	{
		DeleteObject( hBmp );
		SelectObject( MegaDC, hOldBmp );
		DeleteDC( MegaDC );

		delete [] m_pBitInfo;

		MegaDC = NULL;
	}
}
/*********************************************************************************************
	함수명	:	Show
	내  용	:	도움말을 출력을 한다.
	리턴값  :   화면크기에 의해 좌표 변동이 있었을때 x축 좌표 변동값을 리턴한다. (2002-11-13 박준태 추가)
*********************************************************************************************/
int 	OnlineHelp::Show( LPDIRECTDRAWSURFACE7	lpSurface, SI16 *siDrawedPosX/* = NULL*/, SI16 *siDrawedPosY/* = NULL*/)
{
	int     ret         = 0;
	UI08	*pDst		= NULL;
	UI08	*pSrc		= pTextBuffer;
	SI16	ix, iy;
	SI16	x, y;

	if( bAction == FALSE )		return 0;

	SI16 siHeight, siWidth;

	siHeight = siTextHeight * siLine + 25;
	siWidth = ON_HELP_WIDTH;

	SI16	siPitch;

	SetDirection( x, y );																			// 출력 방향을 결정을 한다.

	if( x < 0 )					{ret = -x; x = 0;}																// X축으로 0을 넘어 갔을때
	if( y < 0 )					y = 0;																// Y축으로 0을 넘어 갔을때
	if(( y + siHeight) > ON_SCREEN_YSIZE)			y = ON_SCREEN_YSIZE - siHeight - 1;				// Y가 화면을 넘어 가면 넘어간 만큼 빼준다.
	if(( x + siTextWidth ) > ON_SCREEN_XSIZE )		{ret = ON_SCREEN_XSIZE - (x + siTextWidth ); x = (ON_SCREEN_XSIZE - siTextWidth - 1); }			// X가 화면을 넘어 가면 넘어간 만큼 빼준다.


	if( clGrp.LockSurface( lpSurface ) == TRUE )
	{
		siPitch	= clGrp.GetSurfaceWidth();

		pDst	= clGrp.Screen8;		
		pDst	+= ( y * siPitch ) + x;		

		for( iy = 0 ;iy<siHeight; ++iy)
		{
			for( ix=0 ; ix<siWidth; ++ix)
			{

				if( pSrc[ix] != TRANSCOLOR )
				{
					pDst[ix] = pSrc[ix] == 0 ?
						pDst[ix] = clGrp.Palette16[ (clGrp.PaletteRGB[ pDst[ix] ][0]>>1) + (clGrp.PaletteRGB[ pSrc[ix] ][0]>>1)]
												  [ (clGrp.PaletteRGB[ pDst[ix] ][1]>>1) + (clGrp.PaletteRGB[ pSrc[ix] ][1]>>1)]
												  [ (clGrp.PaletteRGB[ pDst[ix] ][2]>>1) + (clGrp.PaletteRGB[ pSrc[ix] ][2]>>1)]
									: pSrc[ix];
				}
			}

			pDst += siPitch;
			pSrc += siWidth;
		}

		clGrp.UnlockSurface( lpSurface );
	}

	// 도움말이 출력된 위치
	if (siDrawedPosX)
		*siDrawedPosX = x;

	// 도움말이 출력된 위치
	if (siDrawedPosY)
		*siDrawedPosY = y;

	return ret;
}
/*********************************************************************************************
	함수명	:	Initialize
	내  용	:	버퍼를 초기화 한다.
*********************************************************************************************/
VOID	OnlineHelp::Initialize()
{
	if( pTextBuffer )	memset( pTextBuffer, 254, ON_HELP_WIDTH * ON_HELP_HEIGHT );
	siLine		=	0;
	bAction		=	FALSE;

	m_lpSelectCharacterFlagRenderPosX = NULL;
	m_lpSelectCharacterFlagRenderPosY = NULL;
}
/*********************************************************************************************
	함수명	:	Initialize
	내  용	:	도움말이 출력될 좌표를 정한다.
*********************************************************************************************/
VOID	OnlineHelp::SetDirection( SI16 &x, SI16 &y )
{
	x	=	siX;
	y	=	siY;

	// 가로축 관련 플래그 처리
	if( siDirection & ON_HELP_LEFT )
	{
		x -= siTextWidth + 10;
	}
	else if (siDirection & ON_HELP_RIGHT)
	{
		x += 20;
	}
	else if (siDirection & ON_HELP_CENTER)
	{
		x -= (siTextWidth + 10) / 2;
	}

	// 세로축 관련 플래그 처리
	if( siDirection & ON_HELP_UP )
	{
		y -= ( siTextHeight * siLine ) + 30;
	}
	else if( siDirection & ON_HELP_DOWN )
	{
		y += 20;
	}
	else if (siDirection & ON_HELP_VCENTER)
	{
		y -= ((( siTextHeight * siLine ) + 30) / 2);
	}

	// 상단 깃발 그려지는 위치를 위한 값 수정
	if(m_lpSelectCharacterFlagRenderPosX)
	{
		*m_lpSelectCharacterFlagRenderPosX = x;
	}
	if(m_lpSelectCharacterFlagRenderPosY)
	{
		*m_lpSelectCharacterFlagRenderPosY = y;
	}
}


/*********************************************************************************************
	함수명	:	IsActive
	내  용	:	현재 활성화 되어 있는가?
*********************************************************************************************/
BOOL	OnlineHelp::IsActive()
{
	return bAction;
}



//================================================================================================================//
/*********************************************************************************************
// robypark 2005/1/6 11:6
// 타이틀 텍스트, 내용 텍스트를 구분하여 다른 색으로 출력한다.
// CHAR *pTitleString: 타이틀 텍스트
// CHAR *pContentsString: 내용 텍스트
// SI16 siX: 그려질 위치
// SI16 siY: 그려질 위치
// SI16 siDirect: 텍스트 출력 위치 (default: ON_HELP_LEFT)
// BOOL fMultiLine: 내용 텍스트를 여러 줄로 나누어 보주는가? (default: TRUE)
// DWORD dwColorTitle: 타이틀 텍스트 칼라 (default: RGB(252, 252, 212))
// DWORD dwColorContents: 내용 텍스트 칼라 (default: RGB(255, 255, 255))
*********************************************************************************************/
VOID	OnlineHelp::SetTextHasTitle(CHAR *pTitleString, CHAR *pContentsString, SI16 siX, SI16 siY, SI16 siDirect/* = ON_HELP_LEFT*/, BOOL fMultiLine/* = TRUE*/, DWORD dwColorTitle/* = RGB(252, 252, 212)*/, DWORD dwColorContents/* = RGB(255, 255, 255)*/)
{
	SIZE	TextSizeTitle = {0, 0};
	SIZE	TextSizeContent = {0, 0};	
	UI08	*pTemp;
	SI16	i, j;	

	if( pContentsString == NULL )
		return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	// 문자열들의 그려질 텍스트 크기 구하기
	// 타이틀 텍스트가 있는 경우
	if (NULL != pTitleString)
		GetTextExtentPoint32( MegaDC, pTitleString, strlen( pTitleString ), &TextSizeTitle );	// title string

	GetTextExtentPoint32( MegaDC, pContentsString, strlen( pContentsString ), &TextSizeContent );	// contents string

	Initialize();	// 버퍼를 지워준다.

	// 처리할 문자열의 가로 길이 구하기
	siTextWidth		=	(SI16)((TextSizeTitle.cx < TextSizeContent.cx) ? TextSizeContent.cx : TextSizeTitle.cx);

	if(fMultiLine && (siTextWidth > (SI16)ON_HELP_WIDTH))
		siTextWidth = (SI16)ON_HELP_WIDTH;

	if (NULL != pTitleString)
	{
		siTextHeight	=	(SI16)((TextSizeTitle.cy < TextSizeContent.cy) ? TextSizeContent.cy : TextSizeTitle.cy);
		siLine			=	2;
	}
	else
	{
		siTextHeight	=	(SI16)TextSizeContent.cy;
		siLine			=	1;
	}

	pTemp			=	pTextBuffer;

	if(fMultiLine)
	{
		SI32 siTempTextHeight = pMyOnlineWorld->pOnlineMegaText->CheckTextLine2( MegaDC, 5, 5, siTextWidth - 20, pContentsString);

		if (NULL != pTitleString)
			siLine       = (SI16)((siTempTextHeight + siTextHeight - 1) / siTextHeight) + 1;
		else
			siLine       = (SI16)((siTempTextHeight + siTextHeight - 1) / siTextHeight);
	}

	siDirection		=	siDirect;
	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j<(siTextHeight * siLine ) + 10; j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<siTextWidth + 10 ; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j<(siTextHeight * siLine ) + 20; j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}
	
	// 타이틀 문자열이 있는 경우
	if (NULL != pTitleString)
	{
		// 타이틀 문자열 설정
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, 5, 5, siTextWidth, siTextHeight, pTitleString, dwColorTitle );

		// 내용 문자열 설정
		if(fMultiLine)
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, siTextHeight + 10, siTextWidth - 20, pContentsString, dwColorContents);
		else
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, siTextHeight + 10, pContentsString, dwColorContents );
	}
	else	// 타이틀 문자열이 없는 경우 내용만 찍는다.
	{
		if(fMultiLine)
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, 5, 220, pContentsString, dwColorContents);
		else
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pContentsString, dwColorContents );
	}
}

// robypark 2005/1/29 16:40
// 변신하려는 공성 유닛의 간략한 정보 출력
// LPSSIEGEUNITTABLE lpsSiegeUnitTable: 병과 정보 구조체
// SI16 siX: 그려질 위치
// SI16 siY: 그려질 위치
// BOOL bCaptain: 대장 변신 가능한 사용자인가?
// SI16 siDirect: 텍스트 출력 위치 (default: ON_HELP_LEFT)
VOID	OnlineHelp::SetTextSiegeUnit(LPSSiegeUnitTable lpsSiegeUnitTable, SI16 siX, SI16 siY, BOOL bCaptain, SI16 siDirect/* = ON_HELP_LEFT*/)
{
	SIZE	TextSize;	
	char	*pTitleString;
	UI08	*pTemp;
	SI16	i, j;	
	SI32	TempXPos;
	SI32	TempYPos;
	CHAR	szTemp[1024], szTemp1[256];
	ZeroMemory(szTemp, sizeof(szTemp));
	ZeroMemory(szTemp1, sizeof(szTemp1));

	if (NULL == lpsSiegeUnitTable)
		return;

	COLORREF	rgb;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY; 

	siDirection		=	siDirect;

	Initialize();																				// 버퍼를 지워준다.

	pTitleString = pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siUnitNameTextIdx);

	GetTextExtentPoint32( MegaDC, pTitleString, strlen( pTitleString ), &TextSize );
	TextSize.cy += 1;

	siTextHeight	=	(SI16)TextSize.cy;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	( TextSize.cx / ( ON_HELP_WIDTH - 14 ) ) + 1;							// 총 몇 라인 인가 알아 본다.
	bAction			=	TRUE;

	//===============================================================================================
	// 미리 Line 계산
	// 대장 유닛 변신 불가능한 경우 
	if (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
	{
		strcpy(szTemp, pMyOnlineWorld->pOnlineText->Get(8100122));

		SI32 siTempTextHeight = pMyOnlineWorld->pOnlineMegaText->CheckTextLine2( MegaDC, 5, 5, ON_HELP_REALWIDTH - 5, szTemp);

		siLine += (SI16)((siTempTextHeight + TextSize.cy - 1) / TextSize.cy) + 1;
	}

	// 공성 유닛 설명 라인 계산
	strcpy(szTemp, pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siArmyKindDescTextID));
	SI32 siTempTextHeight = pMyOnlineWorld->pOnlineMegaText->CheckTextLine2( MegaDC, 5, 5, ON_HELP_REALWIDTH - 5, szTemp);
	siLine += (SI16)((siTempTextHeight + TextSize.cy - 1) / TextSize.cy);

	// 레벨, 비용 출력 라인 계산
	siLine += 2;

	//===============================================================================================
	// 초기화
	for(j = 0; j < ((TextSize.cy * siLine) + 20); j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	//===============================================================================================
	// Text 출력
	// 병과 출력
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pTitleString, RGB(0, 255, 0));

	TempXPos = 5;
	TempYPos = 2;

	// 변신 가능한 레벨
	sprintf( szTemp, "%s: %d", pMyOnlineWorld->pOnlineText->Get(1100019), lpsSiegeUnitTable->m_scTransformLevelLimit);
	rgb = pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) >= (UI32)lpsSiegeUnitTable->m_scTransformLevelLimit ? RGB(255, 255, 255) : RGB( 255, 0, 0 );
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, rgb);
	TempYPos++;

	// 변신 비용
	SI32 siCost = lpsSiegeUnitTable->m_siTransformCost;

	// 대장일 경우 변신 비용은 두 배
	if (bCaptain)
		siCost = siCost * 2;

	// comma 추가
	MakeMoneyText(siCost, szTemp1);

	// 변신에 필요한 돈
	sprintf( szTemp, "%s: %s %s", pMyOnlineWorld->pOnlineText->Get(8100127), szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
	rgb = pMyOnlineWorld->pMyData->GetMoney() >= siCost ? RGB(255, 255, 255) : RGB( 255, 0, 0 );
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, rgb);
	TempYPos++;

	// 공성 유닛 설명 출력
	strcpy(szTemp, pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siArmyKindDescTextID));
	TempYPos += pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH - 5, szTemp, RGB(255, 255, 255));

	// 대장 유닛 변신 불가능한 경우 
	if (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL)
	{
		strcpy(szTemp, pMyOnlineWorld->pOnlineText->Get(8100126));

		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, TempXPos, TextSize.cy * TempYPos + 5, ON_HELP_REALWIDTH - 5, szTemp, RGB(255, 255, 255));
	}
}

//-----------------------------------------------------------------------------
// Name: SetTextFieldItem()
// Code: actdoll (2005-02-02)
// Desc: 헬프투명창 출력
//	사용하는 곳 - 필드 위의 아이템에 마우스를 올려놨을 때
//		pItem			- 설명하고자 하는 아이템의 CItemHeader 포인터
//		siItemCount		- 해당 아이템의 떨어져있는 갯수
//		siX, siY		- 출력할 좌상단 좌표
//-----------------------------------------------------------------------------
VOID	OnlineHelp::SetTextFieldItem( CItemHeader *pItem, SI16 siItemCount, SI16 siX, SI16 siY )
{
	SIZE	TextSize, MaxSize = {0,};
	CHAR	pszCount[128], pszTimerItem[128];
	SI16	j,i;
	UI08	*pTemp;
	CHAR	*pString;
	
	// 아이템 이름
	pString	=	pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName );
	GetTextExtentPoint32( MegaDC, pString, strlen( pString ), &TextSize );
	MaxSize	= TextSize;

	// 시간제 아이템일 경우
	if( pItem->m_bTimeLimit )
	{
		// 시간제 아이템의 축약어
		sprintf( pszCount, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );
		sprintf( pszTimerItem, "%s - %s", pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_SIMPLE ), pszCount );
		GetTextExtentPoint32( MegaDC, pszTimerItem, strlen( pszTimerItem ), &TextSize );
		MaxSize.cx	= max( TextSize.cx, MaxSize.cx );
	}

	// 아이템 수량
	wsprintf( pszCount, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_COUNT2), siItemCount );	// 아이템 수량
	GetTextExtentPoint32( MegaDC, pszCount, strlen( pszCount ), &TextSize );
	MaxSize.cx	= max( TextSize.cx, MaxSize.cx );

	siTextWidth		=	(SI16)MaxSize.cx + 12;
	siTextHeight	=	(SI16)MaxSize.cy;
	pTemp			=	pTextBuffer;	
	siDirection		=	ON_HELP_LEFT;	

	Initialize();

	bAction				=	TRUE;
	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;
	siLine				=	3;
	

	for( j=0 ; j<(MaxSize.cy * siLine ) + 13; j++ )	
	{
		for( i=0 ;i<siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;

		pTemp	+= ON_HELP_WIDTH;
	}

	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, RGB( 0, 255, 0 ) );
	if( pItem->m_bTimeLimit)	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5 + MaxSize.cy + 2, pszTimerItem, RGB( 0, 255, 0 ) );
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5 + ( MaxSize.cy + 2 ) * 2, pszCount, pMyOnlineWorld->PaletteWhiteColor);
}

VOID	OnlineHelp::SetText(CHAR *pString, char *pdesc, char *pdesc2, char *pdesc3, SI16 siX, SI16 siY)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY; 

	GetTextExtentPoint32( MegaDC, pString, strlen( pString ), &TextSize );

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)TextSize.cx + 50;
	siTextHeight	=	(SI16)TextSize.cy + 5;
	pTemp			=	pTextBuffer;
	siLine			=	5;
	siDirection		=	ON_HELP_DOWN | ON_HELP_LEFT;
	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j<(siTextHeight * siLine ); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<siTextWidth; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j<(siTextHeight * siLine ); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}	
	
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, RGB( 240, 240, 240 ));
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5+(TextSize.cy*2), pdesc, RGB( 240, 240, 240 ));
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5+(TextSize.cy*3), pdesc2, RGB( 240, 240, 240 ));
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5+(TextSize.cy*4), pdesc3, RGB( 240, 240, 240 ));
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	텍스트를 버퍼에 입력을 한다.
			actdoll - 이 부분은 몹과 하단의 기타 정보에 올릴 경우 발생
*********************************************************************************************/
VOID	OnlineHelp::SetText( CHAR *pString, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, BOOL fMultiLine )
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	

	if( pString == NULL )			return;


	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	GetTextExtentPoint32( MegaDC, pString, strlen( pString ), &TextSize );

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)TextSize.cx;
	if(fMultiLine && (siTextWidth > (SI16)ON_HELP_WIDTH)) siTextWidth = (SI16)ON_HELP_WIDTH;
	siTextHeight	=	(SI16)TextSize.cy;	
	pTemp			=	pTextBuffer;
	siLine			=	1;	
	if(fMultiLine)
	{
		siTextHeight = pMyOnlineWorld->pOnlineMegaText->CheckTextLine2( MegaDC, 5, 5, siTextWidth, pString);
		siLine       = (SI16)((siTextHeight + TextSize.cy - 1) / TextSize.cy);
		siTextHeight = (int)( (siTextHeight / siLine) + 1 );
	}
	siDirection		=	siDirect;
	

	bAction			=	TRUE;
	
	if( TextSize.cx < ON_HELP_WIDTH )
	{
		for( j=0 ; j<(TextSize.cy * siLine ) + 10; j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<TextSize.cx + 10 ; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j<(TextSize.cy * siLine ) + 20; j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}
	

	if( siColor & ON_HELP_RED )
	{
		siColor = 255;

		if(fMultiLine)
		{
			SetTextColor(MegaDC, RGB(siColor, 0, 0));			
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, 5, 220, pString);
		}
		else
		{
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, RGB( siColor, 0, 0 ) );
		}
	}

	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;

		if(fMultiLine)
		{
			SetTextColor(MegaDC, RGB(0, 0, siColor));			
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, 5, 220, pString);
		}
		else
		{
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, RGB( 0, 0, siColor ) );
		}
	}

	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;

		if(fMultiLine)
		{
			SetTextColor(MegaDC, RGB(0, siColor, 0));			
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, 5, 220, pString);
		}
		else
		{
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, RGB( 0, siColor, 0 ) );
		}
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		if(fMultiLine)
		{
			SetTextColor(MegaDC, pMyOnlineWorld->PaletteWhiteColor);
			int	iLen = strlen( pString );
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, 5, 220, pString);
		}
		else
		{
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, pMyOnlineWorld->PaletteWhiteColor);
		}
	}
	else if( siColor & ON_HELP_WHITERED )	
	{
		if(fMultiLine)
		{
			SetTextColor(MegaDC, RGB( 255, 255 / 4, 255 / 8));
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, 5, 220, pString);
		}
		else
		{
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, pString, RGB( 255, 255 / 4, 255 / 8));
		}
	}
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	텍스트를 버퍼에 입력을 한다.
			actdoll - 이 부분은 포탈 및 건물에 올릴 경우 발생
*********************************************************************************************/
VOID	OnlineHelp::SetText( CHAR *pString, char *pdesc, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor )
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY; 

	GetTextExtentPoint32( MegaDC, pString, strlen( pString ), &TextSize );

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)240;
	siTextHeight	=	(SI16)TextSize.cy + 10 + pMyOnlineWorld->pOnlineMegaText->CheckTextLine2(MegaDC, 5, (SI32)(10 + TextSize.cy), 220, pdesc);
	pTemp			=	pTextBuffer;
	siLine			=	1;	
	siDirection		=	siDirect;
	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j<(siTextHeight * siLine ); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<siTextWidth; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j<(siTextHeight * siLine ); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}
	
	if( siColor & ON_HELP_RED )
	{
		siColor = 255;
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( siColor, 0, 0 ) );
	}
	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( 0, 0, siColor ) );
	}
	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( 0, siColor, 0 ) );
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, pMyOnlineWorld->PaletteWhiteColor);
	}
	else if( siColor & ON_HELP_WHITERED )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( 255, 255 / 4, 255 / 8));
	}

	pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, (SI16)(10 + TextSize.cy), 220, pdesc);
}

/*********************************************************************************************
	함수명	:	SetTextMark
	내  용	:	오브젝트의 설명(MARK)표시를 할깨 사용.
*********************************************************************************************/
// by kuzimo
VOID	OnlineHelp::SetTextMark(CHAR *pString, SI16 siX, SI16 siY, SI32 siColor)
{
/*
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY; 

	GetTextExtentPoint32( MegaDC, pString, strlen( pString ), &TextSize );

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)240;
	siTextHeight	=	(SI16)TextSize.cy + 10 + pMyOnlineWorld->pOnlineMegaText->CheckTextLine2(MegaDC, 5, (SI32)(10 + TextSize.cy), 220, pdesc);
	pTemp			=	pTextBuffer;
	siLine			=	1;	
	siDirection		=	siDirect;
	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j<(siTextHeight * siLine ); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<siTextWidth; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j<(siTextHeight * siLine ); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}
	
	if( siColor & ON_HELP_RED )
	{
		siColor = 255;
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( siColor, 0, 0 ) );
	}
	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( 0, 0, siColor ) );
	}
	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( 0, siColor, 0 ) );
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, pMyOnlineWorld->PaletteWhiteColor);
	}
	else if( siColor & ON_HELP_WHITERED )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5 + ((230 - TextSize.cx) / 2), 5, pString, RGB( 255, 255 / 4, 255 / 8));
	}

	pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 5, (SI16)(10 + TextSize.cy), 220, pdesc);
*/
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	텍스트를 버퍼에 입력을 한다.
*********************************************************************************************/
VOID	OnlineHelp::SetTextID( CHAR *pString, CHAR *pString1, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor)
{
	SIZE	TextSize;	
	SIZE	TextSize1;	
	UI08	*pTemp;
	SI16	i, j;	

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	GetTextExtentPoint32(MegaDC, pString,  strlen(pString),  &TextSize);
	GetTextExtentPoint32(MegaDC, pString1, strlen(pString1), &TextSize1);

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)(TextSize.cx > TextSize1.cx ? TextSize.cx : TextSize1.cx) + 10;
	if(siTextWidth > ON_HELP_WIDTH ) siTextWidth = ON_HELP_WIDTH;
	siTextHeight	=	(SI16)(TextSize.cy > TextSize1.cy ? TextSize.cy : TextSize1.cy);
	pTemp			=	pTextBuffer;
	siLine			=	3;	
	siDirection		=	siDirect;
	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j< ((siTextHeight * 2) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ; i< siTextWidth ; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j< ((siTextHeight * 2) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}
	

	if( siColor & ON_HELP_RED )
	{
		siColor = 255;
		
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize.cx / 2),  5,               pString,  RGB( siColor, 0, 0 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize1.cx / 2), TextSize.cy + 5, pString1, RGB( siColor, 0, 0 ) );
	}

	else if( siColor & ON_HELP_WHITERED )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize.cx / 2),  5,               pString,  RGB( siColor, siColor / 4, siColor / 8) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize1.cx / 2), TextSize.cy + 5, pString1, RGB( siColor, siColor / 4, siColor / 8) );
	}

	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize.cx / 2),  5,               pString,  RGB( 0, 0, siColor ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize1.cx / 2), TextSize.cy + 5, pString1, RGB( 0, 0, siColor ) );
	}

	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize.cx / 2),  5,               pString,  RGB( 0, siColor, 0 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize1.cx / 2), TextSize.cy + 5, pString1, RGB( 0, siColor, 0 ) );
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize.cx / 2),  5,               pString,  pMyOnlineWorld->PaletteWhiteColor);
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, (siTextWidth / 2) - (TextSize1.cx / 2), TextSize.cy + 5, pString1, pMyOnlineWorld->PaletteWhiteColor);
	}
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	텍스트를 버퍼에 입력을 한다.(ID 입력시 깃발위치까지 검사)
*********************************************************************************************/
VOID	OnlineHelp::SetTextID(CHAR *pString, CHAR *pString1, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, SI16 FlagXSize, SI16 FlagYSize, SI16* FlagRenderPosX, SI16* FlagRenderPosY)
{
	SIZE	TextSize;	
	SIZE	TextSize1;	
	UI08	*pTemp;
	SI16	i, j;	

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	GetTextExtentPoint32(MegaDC, pString,  strlen(pString),  &TextSize);
	GetTextExtentPoint32(MegaDC, pString1, strlen(pString1), &TextSize1);

	Initialize();																				// 버퍼를 지워준다.

	m_lpSelectCharacterFlagRenderPosX = FlagRenderPosX;
	m_lpSelectCharacterFlagRenderPosY = FlagRenderPosY;

	siTextWidth		=	(SI16)(TextSize.cx > TextSize1.cx ? TextSize.cx : TextSize1.cx) + 10;
	siTextWidth		+=	FlagXSize + 5;
	if(siTextWidth > ON_HELP_WIDTH ) siTextWidth = ON_HELP_WIDTH;
	siTextHeight	=	(SI16)(TextSize.cy > TextSize1.cy ? TextSize.cy : TextSize1.cy);
	pTemp			=	pTextBuffer;
	siLine			=	3;	
	siDirection		=	siDirect;
	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j< ((siTextHeight * 2) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ; i< siTextWidth ; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j< ((siTextHeight * 2) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}
	

	if( siColor & ON_HELP_RED )
	{
		siColor = 255;
		
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( siColor, 0, 0 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( siColor, 0, 0 ) );
	}

	else if( siColor & ON_HELP_WHITERED )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( siColor, siColor / 6, siColor / 8) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( siColor, siColor / 6, siColor / 8) );
	}

	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( 0, 0, siColor ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( 0, 0, siColor ) );
	}

	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( 0, siColor, 0 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( 0, siColor, 0 ) );
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  pMyOnlineWorld->PaletteWhiteColor);
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, pMyOnlineWorld->PaletteWhiteColor);
	}
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextBuySolder(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Life, SI32 Mana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT, SI32 MinLevel, SI32 NowLevel, SI32 MinCredit, SI32 NowCredit)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	TempBuffer[1024];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));
	SI32	MaxSize;

	if( pName == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSize = 0;

	sprintf((char*)TempBuffer, " %s", pName);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), STR);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_DEX), DEX);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_VIT), VIT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_INT), INT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_LIFE), Life);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_MANA), Mana);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)MaxSize;
	siTextHeight	=	(SI16)TextSize.cy;
	pTemp			=	pTextBuffer;
	siLine			=	8;
	siDirection		=	ON_HELP_LEFT | ON_HELP_UP;
	
	bAction			=	TRUE;
	
	for( j=0 ; j< ((siTextHeight * 8) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for( i=0 ; i< siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	// Name
	sprintf((char*)TempBuffer, " %s", pName);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	
	// STR
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), STR);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 2) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// DEX
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_DEX), DEX);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 3) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// VIT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_VIT), VIT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 4) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// INT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_INT), INT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 5) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Life
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_LIFE), Life);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 6) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Mana
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_MANA), Mana);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 7) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextBuyMonster(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Life, SI32 Mana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT, SI32 MinLevel, SI32 NowLevel, SI32 MinCredit, SI32 NowCredit, char* ItemName, SI32 UseItem)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	TempBuffer[1024];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));
	SI32	MaxSize;

	if( pName == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSize = 0;

	sprintf((char*)TempBuffer, " %s", pName);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), STR);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_DEX), DEX);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_VIT), VIT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_INT), INT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_LIFE), Life);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_MANA), Mana);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_TEXT_BUY_USEITEM), ItemName, UseItem);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)MaxSize;
	siTextHeight	=	(SI16)TextSize.cy;
	pTemp			=	pTextBuffer;
	siLine			=	9;
	siDirection		=	ON_HELP_LEFT | ON_HELP_UP;
	
	bAction			=	TRUE;
	
	for( j=0 ; j< ((siTextHeight * 9) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for( i=0 ; i< siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	// Name
	sprintf((char*)TempBuffer, " %s", pName);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	
	// STR
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), STR);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 2) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// DEX
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_DEX), DEX);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 3) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// VIT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_VIT), VIT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 4) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// INT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_INT), INT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 5) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Life
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_LIFE), Life);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 6) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Mana
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_MANA), Mana);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 7) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Use Item
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MBARRACK_TEXT_BUY_USEITEM), ItemName, UseItem);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 8) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));	
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextDismissalSolder(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Lv, SI32 Life, SI32 MaxLife, SI32 Mana, SI32 MaxMana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	TempBuffer[1024];
	char	TempBuffer1[1024];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));
	ZeroMemory(TempBuffer1, sizeof(TempBuffer1));
	SI32	MaxSize;

	if( pName == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSize = 0;
	sprintf((char*)TempBuffer, " %s", pName);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	MakeMoneyText(iCost, (char*)TempBuffer1);
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COMPENSATION), TempBuffer1);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LV), Lv);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;
	
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_STR), STR);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_DEX), DEX);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_VIT), VIT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_INT), INT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LIFE), Life, MaxLife);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_MANA), Mana, MaxMana);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)MaxSize;
	siTextHeight	=	(SI16)TextSize.cy;
	pTemp			=	pTextBuffer;
	siLine			=	10;
	siDirection		=	ON_HELP_LEFT | ON_HELP_UP;
	
	bAction			=	TRUE;
	
	for( j=0 ; j< ((siTextHeight * 10) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for( i=0 ; i< siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	// Name
	sprintf((char*)TempBuffer, " %s", pName);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	
	// Cost
	MakeMoneyText(iCost, (char*)TempBuffer1);
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COMPENSATION), TempBuffer1);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 2) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// 레벨
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LV), Lv);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 3) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// STR
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_STR), STR);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 4) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// DEX
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_DEX), DEX);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 5) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// VIT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_VIT), VIT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 6) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// INT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_INT), INT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 7) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Life
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LIFE), Life, MaxLife);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 8) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Mana
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_MANA), Mana, MaxMana);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 9) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextChangeJobSolder(SI16 siX, SI16 siY, SI32 Slot, SI32* lpJobList, UI08* lpJobListFlag, SI32* lpJobListLevel)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	TempBuffer[1024];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));
	SI32	MaxSize;
	SI32	TempCount;
	SI32	TempLine;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSize  = 0;
	TempLine = 1;
	for(TempCount = 0; TempCount < 4; TempCount++)
	{
		if(lpJobList[TempCount])
		{
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_CHANGEGENERALHELP), pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->pIOnlineCharKI->GetName(pMyOnlineWorld->pIOnlineCharKI->GetIndex(lpJobList[TempCount]))), lpJobListLevel[TempCount]);

			GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
			TextSize.cx += 10;
			if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

			TempLine++;
		}
	}

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)MaxSize;
	siTextHeight	=	(SI16)TextSize.cy;
	pTemp			=	pTextBuffer;
	siLine			=	TempLine;
	siDirection		=	ON_HELP_LEFT | ON_HELP_UP;
	
	bAction			=	TRUE;
	
	for( j=0 ; j< ((siTextHeight * siLine) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for( i=0 ; i< siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	TempLine = 0;
	COLORREF	rgb;
	for(TempCount = 0; TempCount < 4; TempCount++)
	{
		if(lpJobList[TempCount])
		{
			if(lpJobListFlag[TempCount])		rgb = RGB( 255, 255, 255);
			else								rgb = RGB( 255, 0, 0);

			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_FOLLOWER_INFO), pMyOnlineWorld->pMyData->GetFollowerCharName(Slot), pMyOnlineWorld->pMyData->GetFollowerParameterLevel(Slot));
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5 + (TempLine * siTextHeight), (char*)TempBuffer,  rgb);
			TempLine++;

			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GBARRACK_TEXTHELP_CHANGEGENERALHELP), pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->pIOnlineCharKI->GetName(pMyOnlineWorld->pIOnlineCharKI->GetIndex(lpJobList[TempCount]))), lpJobListLevel[TempCount]);
			pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5 + (TempLine * siTextHeight), (char*)TempBuffer,  rgb);
			TempLine++;
		}
	}
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextChangeJobGeneral(SI16 siX, SI16 siY, CHAR *pName, SI32 Lev, SI32 MaxLife, SI32 MaxMana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	TempBuffer[1024];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));
	SI32	MaxSize;

	if( pName == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSize = 0;

	sprintf((char*)TempBuffer, " %s", pName);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	if(Lev > 0)
	{
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LV), Lev);
		GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
		TextSize.cx += 10;
		if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;
	}

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), STR);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_DEX), DEX);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_VIT), VIT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_INT), INT);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_LIFE), MaxLife);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_MANA), MaxMana);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)MaxSize;
	siTextHeight	=	(SI16)TextSize.cy;
	pTemp			=	pTextBuffer;
	siLine			=	8;
	siDirection		=	ON_HELP_LEFT | ON_HELP_UP;
	if(Lev > 0)		siLine++;
	bAction			=	TRUE;
	
	for( j=0 ; j< ((siTextHeight * siLine) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for( i=0 ; i< siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	i=1;
	// Name
	sprintf((char*)TempBuffer, " %s", pName);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;

	// Lev
	if(Lev > 0)
	{
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LV), Lev);
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
		i++;
	}
	
	// STR
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), STR);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;

	// DEX
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_DEX), DEX);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;

	// VIT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_VIT), VIT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;

	// INT
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_INT), INT);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;

	// Life
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_LIFE), MaxLife);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;

	// Mana
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_MANA), MaxMana);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * i) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	i++;
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextHospital(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, MONEY iRCost, SI32 Life, SI32 MaxLife, SI32 Mana, SI32 MaxMana)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	TempBuffer[1024];
	char	TempBuffer1[512];
	char	TempBuffer2[512];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));
	ZeroMemory(TempBuffer1, sizeof(TempBuffer1));
	ZeroMemory(TempBuffer2, sizeof(TempBuffer2));
	SI32	MaxSize;

	if( pName == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSize = 0;
	sprintf((char*)TempBuffer, " %s", pName);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	MakeMoneyText(iCost,  (char*)TempBuffer1);
	MakeMoneyText(iRCost, (char*)TempBuffer2);
	if(iRCost > 0) sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HEALMONEY), TempBuffer1, TempBuffer2);
	else           sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HEALMONEY1), TempBuffer1);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_LIFE), Life, MaxLife);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_MAGIC), Mana, MaxMana);
	GetTextExtentPoint32(MegaDC, (char*)TempBuffer, strlen((char*)TempBuffer), &TextSize);
	TextSize.cx += 10;
	if(MaxSize < TextSize.cx) MaxSize = TextSize.cx;

	Initialize();																				// 버퍼를 지워준다.

	siTextWidth		=	(SI16)MaxSize;
	siTextHeight	=	(SI16)TextSize.cy;
	pTemp			=	pTextBuffer;
	siLine			=	5;
	siDirection		=	ON_HELP_LEFT | ON_HELP_UP;
	
	bAction			=	TRUE;
	
	for( j=0 ; j< ((siTextHeight * 5) + 10); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for( i=0 ; i< siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	// Name
	sprintf((char*)TempBuffer, " %s", pName);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 5, (char*)TempBuffer,  RGB( 255, 255, 255));
	
	// Cost
	MakeMoneyText(iCost,  (char*)TempBuffer1);
	MakeMoneyText(iRCost, (char*)TempBuffer2);
	if(iRCost > 0) sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HEALMONEY), TempBuffer1, TempBuffer2);
	else           sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HEALMONEY1), TempBuffer1);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 2) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Life
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_LIFE), Life, MaxLife);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 3) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));

	// Mana
	sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_MAGIC), Mana, MaxMana);
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, (TextSize.cy * 4) + 5, (char*)TempBuffer,  RGB( 255, 255, 255));
}

/*********************************************************************************************
	함수명	:	SetText
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetTextMercenary( SI16 siX, SI16 siY, SI32 siFollowerDBSlot )
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	char	pszNameAndExp[1024];
	char	pszHP[256];
	char	pszMP[256];
	char	pszEXP[256];
	SI32	MaxSizeX;

	if( siFollowerDBSlot < 0 || siFollowerDBSlot >= ON_MAX_FOLLOWER_NUM )	return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	MaxSizeX = 0;
	COLORREF	colorName, colorStatus;
	colorName	= RGB( 233, 233, 233 );
	colorStatus	= RGB( 200, 200, 200 );

	// 이름과 레벨 표시
	sprintf( pszNameAndExp, pMyOnlineWorld->pOnlineText->Get			( ON_TEXT_HELP_LVPP ),
					pMyOnlineWorld->pMyData->GetFollowerCharName		( siFollowerDBSlot ), 
					pMyOnlineWorld->pMyData->GetFollowerParameterLevel	( siFollowerDBSlot ) );
	GetTextExtentPoint32( MegaDC, (char*)pszNameAndExp, strlen( pszNameAndExp ), &TextSize );
	TextSize.cx += 10;
	if(MaxSizeX < TextSize.cx) MaxSizeX = TextSize.cx;

	// 여백을 위해 한줄 추가


	// HP표시
	SI32	siCurrent	= pMyOnlineWorld->pMyData->GetFollowerParameterLife		( siFollowerDBSlot );
	SI32	siMax		= pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife	( siFollowerDBSlot );
	R32		fRate		= (R32)siCurrent / (R32)siMax;
	sprintf( pszHP, "%s   %d / %d", 
					pMyOnlineWorld->pOnlineText->Get					( ON_TEXT_ITEMHELP_HP ), 
					siCurrent,
					siMax );
	GetTextExtentPoint32( MegaDC, (char*)pszHP, strlen( pszHP ), &TextSize);
	TextSize.cx += 10;
	if(MaxSizeX < TextSize.cx) MaxSizeX = TextSize.cx;

	// MP표시
	sprintf( pszMP, "%s %d / %d", 
					pMyOnlineWorld->pOnlineText->Get					( ON_TEXT_ITEMHELP_MP ), 
					pMyOnlineWorld->pMyData->GetFollowerParameterMana	( siFollowerDBSlot ),
					pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana( siFollowerDBSlot ) );
	GetTextExtentPoint32( MegaDC, (char*)pszMP, strlen( pszMP ), &TextSize);
	TextSize.cx += 10;
	if(MaxSizeX < TextSize.cx) MaxSizeX = TextSize.cx;

	// EXP표시
	sprintf( pszEXP, "%s %d / %d", 
					pMyOnlineWorld->pOnlineText->Get					( ON_TEXT_HELP_EXPPP ), 
					pMyOnlineWorld->pMyData->GetFollowerParameterCurExp	( siFollowerDBSlot ),
					pMyOnlineWorld->pMyData->GetFollowerParameterNextExp( siFollowerDBSlot ) );
	GetTextExtentPoint32( MegaDC, (char*)pszEXP, strlen( pszEXP ), &TextSize);
	TextSize.cx += 10;
	if(MaxSizeX < TextSize.cx) MaxSizeX = TextSize.cx;

	Initialize();																				// 버퍼를 지워준다.

	pTemp			=	pTextBuffer;
	siLine			=	5;
	siDirection		=	0;
	siTextWidth		=	(SI16)MaxSizeX;
	siTextHeight	=	(SI16)TextSize.cy;
	
	bAction			=	TRUE;
	
	// 텍스트 크기 만큼 뒷배경을 칠해준다.
//	for( j=siY + 3 ; j<siY + 3 + MaxSizeY; j++ )										
//	{
//		for( i=siX + 5; i<siY + 5 + siTextWidth; i++ )
//			pTemp[i] = ON_HELP_BACK_COLOR;
//		
//		pTemp	+= ON_HELP_WIDTH;
//	}		

	// 텍스트 크기 만큼 뒷배경을 칠해준다.
	for( j=0 ; j<(siTextHeight*siLine) + 6; j++ )										
	{
		for( i=0; i<siTextWidth; i++ )
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}		

	// 이름과 레벨
//	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	6,	4,							(char*)pszNameAndExp,	colorShadow );
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	5,	3,							(char*)pszNameAndExp,	colorName );

	// 폰트 사이즈 잠깐 변경
	SelectObject( MegaDC, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT2 ) );

	// HP
//	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	6,	4 + TextSize.cy,			(char*)pszHP,			colorShadow );
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	5,	3 + ( TextSize.cy * 2 ),	(char*)pszHP,			colorStatus );

	// MP
//	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	6,	4 + ( TextSize.cy * 2 ),	(char*)pszMP,			colorShadow );
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	5,	3 + ( TextSize.cy * 3 ),	(char*)pszMP,			colorStatus );

	// EXP
//	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	6,	4 + ( TextSize.cy * 3 ),	(char*)pszEXP,			colorShadow );
	pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC,	5,	3 + ( TextSize.cy * 4 ),	(char*)pszEXP,			colorStatus );

	SelectObject( MegaDC, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHATLIST ) );
}

//-----------------------------------------------------------------------------
// Name: SetTextItem()
// Code: All (2004-06-07)
// Desc: 헬프투명창 출력
//	사용하는 곳 - 아이템 위에 올려놨을 때
//		pItem			- 설명하고자 하는 아이템의 CItemHeader 포인터
//		pSlot			- 아이템이 위치한 인벤토리 슬롯 포인터
//		siX, siY		- 출력할 좌상단 좌표
//		siDirect		- 출력할 좌상단 좌표를 기준으로 어떤 방향으로 출력할 것인가?
//		fBuyPrice		- 구매 가격을 출력하려면 true, 아니라면 false
//		fSellPrice		- 판매 가격을 출력하려면 true, 아니라면 false
//		pOF				- 인벤토리 내 출력시에만 사용, 해당 아이템을 들고 있는 용병 정보
//		siItemWearType	- 인벤토리 내 출력시에만 사용, 해당 아이템을 입은 위치, -1일 경우 장착하지 않고 인벤토리에만 있다는 뜻이다
//-----------------------------------------------------------------------------
VOID	OnlineHelp::SetTextItem( CItemHeader *pItem, ItemInfo *pSlot,SI16 siX,SI16 siY,SI16 siDirect, BOOL fBuyPrice, BOOL fSellPrice, OnlineFollower *pOF, SI16 siItemWearType )
{
	MONEY		BuyPrice, SellPrice;

	Initialize();

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;
	siDirection			=	siDirect;


	if(pSlot->siType == LIST_FOLLOWER)			SetFollower(pSlot);
	else if(pSlot->siType == LIST_PROPERTY)		SetProperty(pSlot);
	else
	{
		BuyPrice  = -1;
		SellPrice = -1;
		switch(fBuyPrice)
		{
			case TRUE:
				BuyPrice  = pSlot->uiBuyPrice;
				break;

			case 2:
				BuyPrice  = -2;
				break;
		}		
		if(fSellPrice) SellPrice = pSlot->uiSellPrice;

		if (pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_SHIFT))
		{
			if(pMyOnlineWorld->pOnlineBooth->GetStatusOut() == ON_BENCH_OUT_INVENTORY_POLL)
			{
				if(pItem->pWeaponHeader)								WeaponHelp( pItem, BuyPrice, SellPrice, pOF, siItemWearType );		// 무기류
				else if(pItem->m_siGroupCode & ON_ITEM_TRADE )			TradeHelp(pItem, pSlot);					// 장부
				else													OthersHelp( pItem, BuyPrice, SellPrice, pOF, siItemWearType );		// 기타		
			}
			else
				DescriptionHelp( pItem, pOF, siItemWearType );																				// 해당 아이템에 대한 설명
		}
		else 
		{
			if(pItem->pWeaponHeader)								WeaponHelp( pItem, BuyPrice, SellPrice, pOF, siItemWearType );		// 무기류
			else if(pItem->m_siGroupCode & ON_ITEM_TRADE )			TradeHelp(pItem, pSlot);					// 장부
			else													OthersHelp( pItem, BuyPrice, SellPrice, pOF, siItemWearType );		// 기타		
		}
	}
}

//-----------------------------------------------------------------------------
// Name: SetTextItem()
// Code: All (2004-06-07)
// Desc: 헬프투명창 출력
//	사용하는 곳 - 경매소에서 팔고있는 아이템 위에 올려놨을 때
//		pItem			- 설명하고자 하는 아이템의 CItemHeader 포인터
//		siX, siY		- 출력할 좌상단 좌표
//		siDirect		- 출력할 좌상단 좌표를 기준으로 어떤 방향으로 출력할 것인가?
//-----------------------------------------------------------------------------
VOID	OnlineHelp::SetTextAuctionItem( CItemHeader *pItem, SI16 siX,SI16 siY,SI16 siDirect )
{
	MONEY		BuyPrice, SellPrice;

	Initialize();

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;
	siDirection			=	siDirect;

	BuyPrice  = -1;
	SellPrice = -1;
	
	if (pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_SHIFT))
	{
		DescriptionHelp( pItem );				
	}
	else {
		if(pItem->pWeaponHeader)								WeaponHelp(pItem,  BuyPrice, SellPrice);		// 무기류		
		else													OthersHelp(pItem,  BuyPrice, SellPrice);		// 기타		
	}
	
}

/*********************************************************************************************
	함수명	:	
	내  용	:	
*********************************************************************************************/
VOID	OnlineHelp::SetText(CHAR *pName, CHAR *pDesc, SI16 siX, SI16 siY )
{
	UI08	*pTemp;
	SI16	i, j;


	Initialize();																				// 버퍼를 지워준다.


	siTextWidth			=	ON_HELP_WIDTH;
	siTextHeight		=	20;
	pTemp				=	pTextBuffer;
	siLine				=	2;
	siDirection			=	ON_HELP_DOWN;
	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;
	bAction				=	TRUE;
	
	for( j=0 ; j<(siTextHeight * siLine ); j++ )
	{
		for( i=0 ;i<siTextWidth ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;

		pTemp	+= ON_HELP_WIDTH;
	}


	if( pDesc == NULL )
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, 5, 0, siTextWidth, siTextHeight * siLine, pName, RGB( 150, 150, 150 ) );
	else
	{
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, 5, 0, siTextWidth, siTextHeight, pName, RGB( 150, 150, 150 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, 5, 20, pDesc, RGB( 150, 150, 150 ) );
	}
}

VOID	OnlineHelp::SetTextNoSell(CItemHeader *pItem, ItemInfo *pSlot,SI16 siX,SI16 siY,SI16 siDirect)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;
	SI32	TempXPos;
	SI32	TempYPos;
	CHAR	*pItemName	=	NULL;	
	CHAR	*pItemInfo	=	NULL;	

	//===============================================================================================
	// Item 정보 얻어옴
	pItemName		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);					// 아이템 이름
	pItemInfo		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemInfo);					// 아이템 설명

	GetTextExtentPoint32( MegaDC, pItemInfo, strlen( pItemInfo ), &TextSize );
	TextSize.cy += 1;

	siTextHeight	=	(SI16)TextSize.cy;
	siTextWidth		=	ON_HELP_WIDTH + 60;
	pTemp			=	pTextBuffer;
	siLine			=	( TextSize.cx / ( ON_HELP_WIDTH - 14 ) ) + 6;							// 총 몇 라인 인가 알아 본다.
	bAction			=	TRUE;
	TempXPos		=	5;
	TempYPos		=	0;
	siDirection		=	siDirect;


	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;


	for( j=0 ; j<(TextSize.cy * siLine ) + 20; j++ )	
	{
		for( i=0 ;i<ON_HELP_WIDTH ; i++)
			pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp	+= ON_HELP_WIDTH;
	}


	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pItemName, RGB(0, 255, 0));	
	TempYPos += 2;

	pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH - 10, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_HELP_NOSELL), RGB( 240, 240, 0 ) );
	TempYPos++;

/*
	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMSELLPRICE), pItem->m_uiValue / 3);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 240, 0 ) );
	TempYPos++;		
*/
}

/*********************************************************************************************
	함수명	:	WeaponHelp
	내  용	:	무기에 대한 설명을 설정해 준다.
*********************************************************************************************/
VOID	OnlineHelp::WeaponHelp( CItemHeader *pItem, MONEY siBuyPrice, MONEY siSellPrice, OnlineFollower *pOF, SI16 siItemWearType )
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	SI32	TempXPos;
	SI32	TempYPos;
	CHAR	*pItemName	=	NULL;	
	CHAR	*pItemInfo	=	NULL;	
	CHAR	szTemp[1024], szItemName[512], szTemp1[256];
	ZeroMemory(szTemp, sizeof(szTemp));
	ZeroMemory(szItemName, sizeof(szItemName));

	//===============================================================================================
	// Item 정보 얻어옴
	pItemName		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);					// 아이템 이름
	if(pItem->pWeaponHeader->uiGrade)
		sprintf(szItemName, "%s + %d", (CHAR*)pItemName, pItem->pWeaponHeader->uiGrade);
	else
		sprintf(szItemName, pItemName);
	pItemInfo		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemInfo);					// 아이템 설명

	GetTextExtentPoint32( MegaDC, pItemInfo, strlen( pItemInfo ), &TextSize );
	TextSize.cy += 1;

	siTextHeight	=	(SI16)TextSize.cy;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	( TextSize.cx / ( ON_HELP_WIDTH - 14 ) ) + 1;							// 총 몇 라인 인가 알아 본다.
	bAction			=	TRUE;

	//===============================================================================================
	// 미리 Line 계산
	TempXPos = 5;
	TempYPos = 2;
	// actdoll (2005/02/01 17:31) : 시간제 아이템에 대한 내용
	if(pItem->m_bTimeLimit)								TempYPos+=4;
	if((siBuyPrice != -1) || (siSellPrice != -1))		TempYPos++;
	if(pItem->pWeaponHeader->siMaxDam)					TempYPos++;
//	if(pItem->m_siReqStr)								TempYPos++;
//	if(pItem->m_siReqDex)								TempYPos++;
	if(pItem->m_siReqLv)								TempYPos++;
	if(pItem->m_siDefense)								TempYPos++;
	if(pItem->m_siStrength)								TempYPos++;
	if(pItem->m_siDex)									TempYPos++;
	if(pItem->m_siVitality)								TempYPos++;
	if(pItem->m_siInt)									TempYPos++;
	if(pItem->m_siHpGain)								TempYPos++;
	if(pItem->m_siMpGain)								TempYPos++;
	if(pItem->m_siHealth)								TempYPos++;
	
	if( siSellPrice != -1 || siBuyPrice != -1 )
	{
		if( pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade )
		{
			TempYPos++;
			TempYPos++;
		}
	}

	TempYPos += 1;
	siLine   += TempYPos;

	if( siSellPrice != -1 || siBuyPrice != -1 )
	{
		if( ( pMyOnlineWorld->pMyData->GetTradeCredit() * ON_BUY_CREDIT ) < siBuyPrice )
			siLine	+=	2;
	}

	//===============================================================================================
	// 초기화
	for(j = 0; j < ON_HELP_HEIGHT; j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	//===============================================================================================
	// Text 출력
	// Item Data출력

	// 아이템명
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szItemName, RGB(0, 255, 0));

	// 공백 좀 띄우고
	TempXPos = 5;
	TempYPos = 2;

	// actdoll (2005/02/01 17:36) : 시간제 아이템의 경우 베이스 기일을 띄운다.
	if( pItem->m_bTimeLimit )
	{
		DWORD	dwColor = 0;
		BOOL	bEquip	= false;

		// 시간 제한 아이템 표기
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, 
														pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM ), RGB(0, 255, 0) );
		TempYPos++;

		// 이 하위로 들어왔다면 인벤토리 기준이다.
		if( pOF )
		{
			// 현재 어디인가 장착중이라면
			if( siItemWearType != -1 )
			{
				BOOL	bDummy;
				SI16	siRemainDate;

				// 사용기간 및 상태 표기
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );	// ..일용
				strcat( szTemp, " " );
				strcat( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_WORKING ) );						// 사용중
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
				TempYPos++;

				// 남은 날짜에 따라 실질적인 현재 상태를 표기한다.
				pOF->GetWearItem( siItemWearType, bDummy, siRemainDate );

				// 이것은 장착한지 하루가 지나지 않은 것
				if		( pItem->m_siDay < siRemainDate )	{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_READY_TO_START ) );			dwColor = RGB(0, 250, 250);					}
				else if ( siRemainDate < 0 )				{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_IS_EXPIRED ) );				dwColor = RGB(250, 120, 0);					}
				else if	( siRemainDate <= 5 )				{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_REMAIN_TIME ), siRemainDate );dwColor = RGB(250, 250, 0);					}
				else										{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_REMAIN_TIME ), siRemainDate );dwColor = pMyOnlineWorld->PaletteWhiteColor;}
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, dwColor );
				TempYPos++;

				bEquip	= true;
			}
		}

		// 그렇지 않다면 다른 곳에서 사용되고 있는 것이다.
		if( !bEquip )
		{
			// 사용기간 및 상태 표기
			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );		// ..일용
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
			TempYPos+=2;
		}
		TempYPos++;
	}

	if((siBuyPrice != -1) || (siSellPrice != -1))
	{
		if(siBuyPrice != -1)
		{
			if(siBuyPrice > 0)
			{
				MakeMoneyText(siBuyPrice, szTemp1);
				sprintf( szTemp, "%s[%s]", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMPRICE), szTemp1);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 240, 0 ) );
			}
			else
			{
				if(siBuyPrice > 0)
				{
					// 재고없음
					sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_HELP_NOITEM));
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 0, 0 ) );
				}
			}
			TempYPos++;
		}
		if(siSellPrice != -1)
		{
			if(siSellPrice > 0)
			{
				MakeMoneyText(siSellPrice, szTemp1);
				sprintf( szTemp, "%s[%s]", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMSELLPRICE), szTemp1);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 240, 0 ) );
				TempYPos++;
			}			
		}		
	}

	if( siSellPrice != -1 || siBuyPrice != -1 )
	{
		if( pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade )
		{
			TempYPos++;
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_NOTTRADECREDIT), RGB(240, 0, 0));
			TempYPos++;
			
			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEEDCREDIT), pItem->m_uiTradeGrade );
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}
	}
//	else
	{
		if(pItem->pWeaponHeader->siMaxDam)
		{
			wsprintf( szTemp, "%s : %d - %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_ATTACK), pItem->pWeaponHeader->siMinDam, pItem->pWeaponHeader->siMaxDam);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}


		if(pItem->m_siDefense)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEFENCE), pItem->m_siDefense );
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siStrength)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_STR), pItem->m_siStrength);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siDex)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEX), pItem->m_siDex);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siVitality)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_VIT), pItem->m_siVitality);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siInt)
		{
			wsprintf(szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_INT), pItem->m_siInt);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}


		if(pItem->m_siHpGain)
		{
			wsprintf( szTemp, "%s :+%d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HP), pItem->m_siHpGain);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siMpGain)
		{
			wsprintf( szTemp, "%s : +%d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_MP), pItem->m_siMpGain);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siHealth)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HEALTH), pItem->m_siHealth);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_REQUEST ), pMyOnlineWorld->PaletteWhiteColor );
		TempYPos++;

		if(pItem->m_siReqLv)
		{
			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_REQLV), pItem->m_siReqLv);

			if( pMyOnlineWorld->pMyData->GetFollowerParameterLevel( pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() ) >= pItem->m_siReqLv )
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			else
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 255, 0, 0 ) );

			TempYPos++;
		}


		// 수량과 무게
		sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_WEIGHT), pItem->m_uiWeight);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
		TempYPos++;

		if(pItem->pWeaponHeader)
		{
			if(pItem->pWeaponHeader->uiMagic != 0 )
			{				
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pMyOnlineWorld->pOnlineText->Get(pItem->pWeaponHeader->siMagicDesc), pMyOnlineWorld->PaletteWhiteColor);
			}
		}
	}
}

/*********************************************************************************************
	함수명	:	OthersHelp
	내  용	:	기타(갑옷, 악세사리, 소모품) 대한 도움말을
*********************************************************************************************/
VOID	OnlineHelp::OthersHelp( CItemHeader *pItem, MONEY siBuyPrice, MONEY siSellPrice, OnlineFollower *pOF, SI16 siItemWearType )
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	SI32	TempXPos;
	SI32	TempYPos;
	CHAR	*pItemName	=	NULL;	
	CHAR	*pItemInfo	=	NULL;	
	CHAR	szTemp[1024], szTemp1[256];
	ZeroMemory(szTemp, sizeof(szTemp));
	ZeroMemory(szTemp1, sizeof(szTemp1));

	COLORREF	rgb;

	//===============================================================================================
	// Item 정보 얻어옴
	pItemName		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);					// 아이템 이름
	pItemInfo		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemInfo);					// 아이템 설명

	GetTextExtentPoint32( MegaDC, pItemInfo, strlen( pItemInfo ), &TextSize );
	TextSize.cy += 1;

	siTextHeight	=	(SI16)TextSize.cy;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	( TextSize.cx / ( ON_HELP_WIDTH - 14 ) ) + 1;							// 총 몇 라인 인가 알아 본다.
	bAction			=	TRUE;
	//===============================================================================================
	// 미리 Line 계산
	TempXPos = 5;
	TempYPos = 2;
	// actdoll (2005/02/01 17:31) : 시간제 아이템에 대한 내용
	if(pItem->m_bTimeLimit)									TempYPos+=4;
	if((siBuyPrice != -1) || (siSellPrice != -1))			TempYPos++;
	if(pItem->m_siDefense)									TempYPos++;
	if(pItem->m_siMagicRegist)								TempYPos++;
	if(pItem->m_siDamageRegist)								TempYPos++;
	if(pItem->m_siStrength)									TempYPos++;
	if(pItem->m_siDex)										TempYPos++;
	if(pItem->m_siVitality)									TempYPos++;
	if(pItem->m_siInt)										TempYPos++;
	if(pItem->m_siHpGain)									TempYPos++;	
	if(pItem->m_siMpGain)									TempYPos++;
	if(pItem->m_siHealth)									TempYPos++;
	if(pItem->m_siGroupCode & ON_ITEM_REVIVE)               TempYPos++;	
	if(pItem->m_siReqLv)									TempYPos++;


	TempYPos += 1;	
	siLine   += TempYPos;	

	if( siSellPrice != -1 || siBuyPrice != -1 )
	{
		if( pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade )
		{
			siLine	-=	TempYPos;
			siLine	+=	8;
		}
	}
	
	//===============================================================================================
	// 초기화
	for(j = 0; j < ((TextSize.cy * siLine) + 20); j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	//===============================================================================================
	// Text 출력
	// Item Data출력

	// 아이템명
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pItemName, RGB(0, 255, 0));

	// 공백 좀 띄우고
	TempXPos = 5;
	TempYPos = 2;

	// actdoll (2005/02/01 17:36) : 시간제 아이템의 경우 베이스 기일을 띄운다.
	if( pItem->m_bTimeLimit )
	{
		DWORD	dwColor = 0;
		BOOL	bEquip	= false;

		// 시간 제한 아이템 표기
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, 
														pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM ), RGB(0, 255, 0) );
		TempYPos++;

		// 이 하위로 들어왔다면 인벤토리 기준이다.
		if( pOF )
		{
			// 현재 어디인가 장착중이라면
			if( siItemWearType != -1 )
			{
				BOOL	bDummy;
				SI16	siRemainDate;

				// 사용기간 및 상태 표기
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );	// ..일용
				strcat( szTemp, " " );
				strcat( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_WORKING ) );						// 사용중
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
				TempYPos++;

				// 남은 날짜에 따라 실질적인 현재 상태를 표기한다.
				pOF->GetWearItem( siItemWearType, bDummy, siRemainDate );

				// 이것은 장착한지 하루가 지나지 않은 것
				if		( pItem->m_siDay < siRemainDate )	{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_READY_TO_START ) );			dwColor = RGB(0, 250, 250);					}
				else if ( siRemainDate < 0 )				{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_IS_EXPIRED ) );				dwColor = RGB(250, 120, 0);					}
				else if	( siRemainDate <= 5 )				{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_REMAIN_TIME ), siRemainDate );dwColor = RGB(250, 250, 0);					}
				else										{	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_REMAIN_TIME ), siRemainDate );dwColor = pMyOnlineWorld->PaletteWhiteColor;}
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, dwColor );
				TempYPos++;

				bEquip	= true;
			}
		}

		// 그렇지 않다면 다른 곳에서 사용되고 있는 것이다.
		if( !bEquip )
		{
			// 사용기간 및 상태 표기
			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );		// ..일용
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
			TempYPos+=2;
		}
		TempYPos++;
	}

	if((siBuyPrice != -1) || (siSellPrice != -1))
	{
		if(siBuyPrice != -1)
		{
			if(siBuyPrice > 0)
			{
				MakeMoneyText(siBuyPrice, szTemp1);
				sprintf( szTemp, "%s[%s]", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMPRICE), szTemp1);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 240, 0 ) );
			}
			else
			{
				// 재고없음
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_HELP_NOITEM));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 0, 0 ) );
			}
			TempYPos++;
		}
		if(siSellPrice != -1)
		{
			if(siSellPrice > 0)
			{
				MakeMoneyText(siSellPrice, szTemp1);
				sprintf( szTemp, "%s[%s]", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMSELLPRICE), szTemp1);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 240, 0 ) );
			}
			else
			{
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TRADESMAN_HELP_NOITEM));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 0, 0 ) );
			}
			
			TempYPos++;
		}		
	}


	if( siSellPrice != -1 || siBuyPrice != -1 )
	{
		if( pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade )
		{
			TempYPos++;
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_NOTTRADECREDIT), RGB(240, 0, 0));
			TempYPos++;

			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEEDCREDIT), pItem->m_uiTradeGrade );
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}
	}
//	else
	{
/*
		if(pItem->m_siAttack)
		{
			wsprintf( szTemp, "%s %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_ATTACK), pItem->m_siAttack);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}
*/
		if(pItem->m_siDefense)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEFENCE), pItem->m_siDefense );
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siMagicRegist)
		{
			wsprintf( szTemp, "%s : %d%%",pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_MANAREGIST), pItem->m_siMagicRegist );
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siDamageRegist)
		{
			wsprintf( szTemp, "%s : %d%%",pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DAMAGEREGIST), pItem->m_siDamageRegist );
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siStrength)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_STR), pItem->m_siStrength);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siDex)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEX), pItem->m_siDex);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siVitality)
		{
			wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_VIT), pItem->m_siVitality);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siInt)
		{
			wsprintf(szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_INT), pItem->m_siInt);
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siHpGain)
		{

			if (pItem->m_siGroupCode & ON_ITEM_TOALL)
			{
				if( pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER )
					wsprintf( szTemp, "%s %s : %d%%", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_ALL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HP), pItem->m_siHpGain);
				else
					wsprintf( szTemp, "%s %s : +%d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_ALL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HP), pItem->m_siHpGain);
			}
			else
			{
				wsprintf( szTemp, "%s : +%d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HP), pItem->m_siHpGain);
			}
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

		if(pItem->m_siMpGain)
		{
			if (pItem->m_siGroupCode & ON_ITEM_TOALL) 
			{
				if( pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER )
					wsprintf( szTemp, "%s %s :%d%%", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_ALL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_MP), pItem->m_siMpGain);
				else
					wsprintf( szTemp, "%s %s :+%d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_ALL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_MP), pItem->m_siMpGain);
			}
			else
			{
				
				wsprintf( szTemp, "%s :+%d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_MP), pItem->m_siMpGain);
			}
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}
	
		if(pItem->m_siHealth)
		{
			if( pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER )
				wsprintf( szTemp, "%s : %d%%", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HEALTH), pItem->m_siHealth);
			else
				wsprintf( szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_HEALTH), pItem->m_siHealth);
			
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

        if(pItem->m_siGroupCode & ON_ITEM_REVIVE)
		{
			if( pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER )
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(1897));
			else
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_CAN_REVIVE));
			
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
			TempYPos++;
		}

	
		if(pItem->m_siGroupCode & ON_ITEM_DRESS)
		{
			if(pItem->m_siReqStr)
			{
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_DEFENCEUP), pItem->m_siReqStr);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;
			}
		
			if(pItem->m_siReqDex)
			{
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_ATTACKUP), pItem->m_siReqDex);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;
			}
		}

		if(pItem->m_siReqLv)
		{
			if( pItem->m_siGroupCode & ON_ITEM_DRESS )
			{
				sprintf( szTemp, "%s : %d",pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_CREDITRATE), pItem->m_siReqLv);
				rgb = pMyOnlineWorld->pMyData->GetTradeGrade() >= (UI32)pItem->m_siReqLv ? pMyOnlineWorld->PaletteWhiteColor : RGB( 255, 0, 0 );
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, rgb);
				TempYPos++;
			}
			else
			{
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_REQLV), pItem->m_siReqLv);
				rgb = pMyOnlineWorld->pMyData->GetFollowerParameterLevel( pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() ) >= pItem->m_siReqLv ? pMyOnlineWorld->PaletteWhiteColor : RGB( 255, 0, 0 );
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, rgb);
				TempYPos++;
			}
		}

		if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
		{
			switch (pItem->m_siSpecial)
			{
			case ON_ITEM_SPECIAL_INITIALIZE:
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEM_SPECIAL_INITIALIZE));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;			
				break;
			case ON_ITEM_SPECIAL_SHOWMERCENARY:
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEM_SPECIAL_SHOWMERCENARY));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;			
				break;
			case ON_ITEM_SPECIAL_TACTICS:
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(1931));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;
				break;
			case ON_ITEM_SPECIAL_CREDITUP:
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(1847));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;
				break;
			case ON_ITEM_SPECIAL_DONKEY:			//당나귀 고용.
				wsprintf( szTemp, "%s",pMyOnlineWorld->pOnlineText->Get(4000291));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
				TempYPos++;
				break;
			}
		}


		// 수량과 무게
		sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_WEIGHT), pItem->m_uiWeight);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, pMyOnlineWorld->PaletteWhiteColor);
		TempYPos++;
	}
}

/*********************************************************************************************
	함수명	:	TradeHelp
	내  용	:	장부용
*********************************************************************************************/
VOID	OnlineHelp::TradeHelp(CItemHeader *pItem, ItemInfo *pSlot)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	CHAR	*pItemName	=	NULL;		
	CHAR	szTemp[1024];
	ZeroMemory(szTemp, sizeof(szTemp));

	//===============================================================================================
	// Item 정보 얻어옴
	pItemName		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);					// 아이템 이름
	

	GetTextExtentPoint32( MegaDC, pItemName, strlen( pItemName ), &TextSize );
	TextSize.cy += 1;

	siTextHeight	=	(SI16)TextSize.cy;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	4;
	bAction			=	TRUE;


	//===============================================================================================
	// 초기화
	for(j = 0; j < ((TextSize.cy * siLine) + 20); j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	
	// 아이템 이름
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pItemName, RGB(0, 255, 0));

	sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MERITNOTE), pSlot->uiBuyPrice );
	
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 25, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB( 240, 240, 0 ) );
}
/*********************************************************************************************
	함수명	:	DescriptionHelp
	내  용	:	아이템 설명을 써줌
*********************************************************************************************/
VOID	OnlineHelp::DescriptionHelp( CItemHeader *pItem, OnlineFollower *pOF, SI16 siItemWearType )
{
	SIZE	TextSize;	
	UI08	*pTemp;
	SI16	i, j;	
	SI32	TempXPos;
	SI32	TempYPos;
	CHAR	*pItemName	=	NULL;	
	CHAR	*pItemInfo	=	NULL;	
	CHAR	szTemp[1024];

	//===============================================================================================
	// Item 정보 얻어옴
	pItemName		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);					// 아이템 이름
	pItemInfo		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemInfo);					// 아이템 설명

	GetTextExtentPoint32( MegaDC, pItemInfo, strlen( pItemInfo ), &TextSize );
	TextSize.cy += 1;

	siTextHeight	=	(SI16)TextSize.cy;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	( TextSize.cx / ( ON_HELP_WIDTH - 14 ) ) + 1;							// 총 몇 라인 인가 알아 본다.
	bAction			=	TRUE;
	//===============================================================================================
	// 미리 Line 계산
	TempXPos = 5;
	TempYPos = 2;

	// actdoll (2005/02/01 17:31) : 시간제 아이템에 대한 내용
	if(pItem->m_bTimeLimit)									TempYPos+=3;
	TempYPos += 1;	
	siLine   += TempYPos;	
	
	//===============================================================================================
	// 초기화
	for(j = 0; j < ((TextSize.cy * siLine) + 20); j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	//===============================================================================================
	// Text 출력
	// Item Data출력
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pItemName, RGB(0, 255, 0));

	// 공백 좀 띄우고
	TempXPos = 5;
	TempYPos = 2;

	// actdoll (2005/02/01 17:36) : 시간제 아이템의 경우 베이스 기일을 띄운다.
	if( pItem->m_bTimeLimit )
	{
		DWORD	dwColor = 0;
		BOOL	bEquip	= false;

		// 시간 제한 아이템 표기
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, 
														pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM ), RGB(0, 255, 0) );
		TempYPos++;

		// 이 하위로 들어왔다면 인벤토리 기준이다.
		if( pOF )
		{
			// 현재 어디인가 장착중이라면
			if( siItemWearType != -1 )
			{
				// 사용기간 및 상태 표기
				sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );	// ..일용
				strcat( szTemp, " " );
				strcat( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_WORKING ) );						// 사용중
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
				bEquip	= true;
			}
		}

		// 그렇지 않다면 다른 곳에서 사용되고 있는 것이다.
		if( !bEquip )
		{
			// 사용기간 및 상태 표기
			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );		// ..일용
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
		}
		TempYPos+=2;
	}

	pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, TempXPos, TextSize.cy * TempYPos, ON_HELP_REALWIDTH - 5, pItemInfo, RGB(255, 255, 255));
}


VOID	OnlineHelp::SetFollower(ItemInfo *pSlot)
{
	UI08	*pTemp;
	SI16	i, j;	
	CHAR	szTemp[1024];
	ZeroMemory(szTemp, sizeof(szTemp));
	
	siTextHeight	=	20;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	5;
	bAction			=	TRUE;

	//===============================================================================================
	// 초기화
	for(j = 0; j < ((20 * siLine) + 20); j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}	

	sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_REQLV), pSlot->Follower.siLevel);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 15, ON_HELP_REALWIDTH, 15, szTemp, RGB(240, 240, 240));

	sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(6000075) , pSlot->Follower.siStr);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 30, ON_HELP_REALWIDTH, 15, szTemp, RGB(240, 240, 240));

	sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(6000076), pSlot->Follower.siDex);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 45, ON_HELP_REALWIDTH, 15, szTemp, RGB(240, 240, 240));

	sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(6000077), pSlot->Follower.siVit);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 60, ON_HELP_REALWIDTH, 15, szTemp, RGB(240, 240, 240));

	sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(6000078), pSlot->Follower.siInt);
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 75, ON_HELP_REALWIDTH, 15, szTemp, RGB(240, 240, 240));
}

VOID	OnlineHelp::SetProperty(ItemInfo *pSlot)
{
	UI08	*pTemp;
	SI16	i, j;	
	CHAR	szTemp[1024];
	ZeroMemory(szTemp, sizeof(szTemp));
	
	siTextHeight	=	20;
	siTextWidth		=	ON_HELP_WIDTH;
	pTemp			=	pTextBuffer;
	siLine			=	4;
	bAction			=	TRUE;

	//===============================================================================================
	// 초기화
	for(j = 0; j < ((20 * siLine) + 20); j++)												// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < ON_HELP_WIDTH; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	UI16 uiPlantID = 0;
	OnlineVillageParser *pVillageParser = pMyOnlineWorld->pOnlineVillageManager->GetVillageParser();
	VillageHeader		*pVillageHeader = pVillageParser->GetVillageHeaderCode(pSlot->Property.uiVillageCode);

	if(pVillageHeader)
	{
		for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
		{
			switch( pVillageHeader->siStructureKinds[i])
			{
			case ON_VIL_ST_KIND_FARM:
				uiPlantID = ON_VIL_ST_KIND_FARM;
				break;
			case ON_VIL_ST_KIND_PASTURE:
				uiPlantID = ON_VIL_ST_KIND_PASTURE;
				break;
			case ON_VIL_ST_KIND_FACTORY:
				uiPlantID = ON_VIL_ST_KIND_FACTORY;
				break;
			case ON_VIL_ST_KIND_ARMOR:
				uiPlantID = ON_VIL_ST_KIND_ARMOR;
				break;
			case ON_VIL_ST_KIND_WEAPON:
				uiPlantID = ON_VIL_ST_KIND_WEAPON;
				break;
			}
		}
		CHAR TempPlantKind[4];
		pVillageParser->GetStructKind(uiPlantID, *TempPlantKind);
		stPlants *pPlants = pMyOnlineWorld->pOnlineBuilding->GetPlants(TempPlantKind);

		if(pPlants)
		{
			sprintf(szTemp, "%s : %s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOWN), pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));
			pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 40, 10, szTemp, RGB(240, 240, 240));
			sprintf(szTemp, "%s : %d%s %s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_NAME), pSlot->Property.uiIndustrialComplexID + 1
				, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT), pMyOnlineWorld->pOnlineText->Get(pPlants->siTextCode));
			pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 40, 27, szTemp, RGB(240, 240, 240));

			// 건물리스트
			stFacility *pFacility;
			SI08 siCount = 0;
			sprintf(szTemp, "%s : ", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MINIMAP_STRUCTER));
			for(i = 0; i < ON_MAX_FACTORY_NUM; i++)
			{
				pFacility = pMyOnlineWorld->pOnlineBuilding->GetFacility(pSlot->Property.cFactoryKind[i]);
				if(pFacility)
				{
					if(pFacility->siTextCode)
					{
						if(siCount == 0)
						{
							strcat(szTemp, pMyOnlineWorld->pOnlineText->Get(pFacility->siTextCode));
							siCount++;
						}
						else
						{
							strcat(szTemp, ", ");
							strcat(szTemp, pMyOnlineWorld->pOnlineText->Get(pFacility->siTextCode));
							siCount++;
						}
					}
				}
			}
			if(!siCount)
				strcat(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_NONE));
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine(MegaDC, 40, 44, 160, szTemp, RGB(240, 240, 240));
		}
		else
		{
			sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_PROPERTY_NOTFINDPROPERTY));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, 15, szTemp, RGB(240, 240, 240));
		}
	}
}

VOID	OnlineHelp::SetTextBooth(SellItemInfo *pItemInfo, SI16 siX, SI16 siY)
{
	SIZE	TextSize;	
	UI08	*pTemp;
	CItemHeader *pItem = NULL;
	SI16	i, j, TempLine = 1;	
	CHAR	*pItemName		=	NULL;		
	CHAR	*pItemInfoDesc	=	NULL;
	CHAR	szTemp[1024], szTemp2[256];
	ZeroMemory(szTemp2, sizeof(szTemp2));
	ZeroMemory(szTemp, sizeof(szTemp));

	//===============================================================================================
	// Item, 용병 이름 얻기
	if(pItemInfo->siType == LIST_ITEM)
	{
		pItem	=	pMyOnlineWorld->pItemsInfo->GetItem(pItemInfo->Item.uiItemID);
		if(pItem)
		{
			pItemName		=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName);		// 아이템 이름
			pItemInfoDesc	=	pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemInfo);		// 아이템 설명
		}
	}
	else if(pItemInfo->siType == LIST_FOLLOWER)
	{
		UI32 TextID = pMyOnlineWorld->pIOnlineCharKI->GetName(pMyOnlineWorld->pIOnlineCharKI->GetIndex(pItemInfo->Follower.uiKind));
		pItemName = pMyOnlineWorld->pOnlineText->Get(TextID);
	}

	if(pItemInfoDesc)
		GetTextExtentPoint32( MegaDC, pItemInfoDesc, strlen( pItemInfoDesc ), &TextSize );
	else if(pItemName)
		GetTextExtentPoint32( MegaDC, pItemName, strlen( pItemName ), &TextSize );
	TextSize.cy += 1;

	siTextHeight		=	(SI16)TextSize.cy + 2;
	siTextWidth			=	ON_HELP_WIDTH;
	pTemp				=	pTextBuffer;
	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;
	siLine				=	( TextSize.cx / ( ON_HELP_WIDTH - 14 ) ) + 1;
	siLine				+=	8;
	bAction				=	TRUE;
	siDirection			=	ON_HELP_DOWN;


	//===============================================================================================
	// 초기화
	for(j = 0; j < ((TextSize.cy * siLine) + 15); j++)											// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		for(i = 0; i < (ON_HELP_WIDTH) ; i++) pTemp[i] = ON_HELP_BACK_COLOR;
		
		pTemp += ON_HELP_WIDTH;
	}

	// 아이템, 용병 이름
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 5, ON_HELP_REALWIDTH, (SI16)TextSize.cy, pItemName, RGB(0, 255, 0));

	if(pItemInfo->siType == LIST_ITEM)
	{
		
		// 시간제 아이템일 경우
		if( pItem && pItem->m_bTimeLimit )
		{
			// 시간 제한 아이템 표기
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 25, ON_HELP_REALWIDTH, (SI16)TextSize.cy, 
															pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM ), RGB(0, 255, 0));
			// 사용기간 및 상태 표기
			sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_TIMERITEM_LIMIT_RANGE ), pItem->m_siDay );		// ..일용
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 5, 25 + TextSize.cy, ON_HELP_REALWIDTH, (SI16)TextSize.cy, szTemp, RGB(0, 255, 0) );
		}

		// 수량
		sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_COUNT), pItemInfo->Item.uiItemCount);
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 55, szTemp, RGB( 240, 240, 240 ) );
		
		// 가격
		MakeMoneyText(pItemInfo->siItemPrice, szTemp2);
		sprintf( szTemp, "%s%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_PRICE), szTemp2);
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 70, szTemp, RGB( 240, 240, 240 ) );
		
		// 설명
		ZeroMemory(szTemp, sizeof(szTemp));
		sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_DESC), pItemInfo->szItemDesc);
		TempLine = pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 25, 85, ON_HELP_REALWIDTH - 35, szTemp, RGB( 240, 240, 240 ) );
		
		// 아이템 설명
		if(pItemInfoDesc)
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 25, 85 + 15 * TempLine, ON_HELP_REALWIDTH - 35, pItemInfoDesc, RGB(240, 240, 240));
	}
	else
	{
		// 가격
		MakeMoneyText(pItemInfo->siItemPrice, szTemp2);
		sprintf( szTemp, "%s%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_PRICE), szTemp2);
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 25, szTemp, RGB( 240, 240, 240 ) );
		
		// 설명
		ZeroMemory(szTemp, sizeof(szTemp));
		sprintf( szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_DESC), pItemInfo->szItemDesc);
		TempLine = pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(MegaDC, 25, 45, ON_HELP_REALWIDTH - 35, szTemp, RGB( 240, 240, 240 ) );
	}
}

VOID	OnlineHelp::SetCharInfo(SI16 siX, SI16 siY, CHAR *pCharName, CHAR *pGuildName, SI32	siGuildClass, SI32 siLevel)
{
	UI08	*pTemp;
	SI16	j;	
	CHAR	szTemp[1024];
	ZeroMemory(szTemp, sizeof(szTemp));
	
	siTextHeight	=	20;
	pTemp			=	pTextBuffer;
	bAction			=	TRUE;
	siDirection		=	ON_HELP_DOWN;

	OnlineHelp::siX	=	siX;
	OnlineHelp::siY	=	siY;

	if(pGuildName[0])
	{
		siTextWidth	= 170;
		siLine = 5;
	}
	else
	{
		siTextWidth = 120;
		siLine = 3;
	}

	//===============================================================================================
	// 초기화
	for(j = 0; j < siTextHeight * siLine; j++)				// 텍스트 크기 만큼 뒷배경을 칠해준다.
	{
		memset(pTemp, ON_HELP_BACK_COLOR, siTextWidth);
		
		pTemp += ON_HELP_WIDTH;
	}	

	// 캐릭터 이름
	pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(MegaDC, 0, 5, siTextWidth, 20, pCharName, RGB(240, 240, 240));

	if(pGuildName[0])
	{
		// 상단이름
		sprintf(szTemp, "%s : %s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), pGuildName);
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 30, szTemp, RGB(240, 240, 240));
		
		// 직급
		sprintf(szTemp, "%s : %s", pMyOnlineWorld->pOnlineText->Get(8000499), pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(siGuildClass)));
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 50, szTemp, RGB(240, 240, 240));
		
		// 레벨
		sprintf(szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_LV), siLevel);
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 70, szTemp, RGB(240, 240, 240));
	}
	else
	{
		// 레벨
		sprintf(szTemp, "%s : %d", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_LV), siLevel);
		pMyOnlineWorld->pOnlineMegaText->DrawText(MegaDC, 25, 30, szTemp, RGB(240, 240, 240));
	}
}

// robypark 2004/11/8 14:48
// 커서 아래 캐릭터가 공성전 유닛일 경우 풍선 도움말 텍스트 설정
// [IN]  CHAR *pString: 캐릭터 이름 정보
// [IN]  SI16 siX: 출력할 위치
// [IN]  SI16 siY: 출력할 위치
// [IN]  SI16 siDirect: 출력할 방향 플래그
// [IN]  SI32 siColor: 텍스트 칼라
// [OUT] SI08 &siDrawGaugeHealthPosX: 체력 바 위치
// [OUT] SI08 &siDrawGaugeHealthPosY: 체력 바 위치
// [IN]  bool bDrawSupplyGoods: 공성전 유닛 남은 보급품(%)을 그려줘야하는지. 보급수레만 해당된다.
// [IN]  SI16 FlagXSize = -1: 상단 깃발 크기
// [IN]  SI16 FlagYSize = -1: 상단 깃발 크기
// [OUT] SI16* FlagRenderPosX = NULL: 상단 깃발 그려지는 위치
// [OUT] SI16* FlagRenderPosY = NULL: 상단 깃발 그려지는 위치
VOID	OnlineHelp::SetSiegeWarfareUnitToolTipText(CHAR *pString, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, 
													   SI08 &siDrawGaugeHealthPosX, SI08 &siDrawGaugeHealthPosY, bool bDrawSupplyGoods,
													   SI16 FlagXSize/* = -1*/, SI16 FlagYSize/* = -1*/,
													   SI16* FlagRenderPosX/* = NULL*/, SI16* FlagRenderPosY/* = NULL*/)
{
	SIZE	TextSize;	
//	SIZE	TextSize1;	
	UI08	*pTemp;
	SI16	i, j;	
	SI08	scSpace = 5;	// 여백

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	GetTextExtentPoint32(MegaDC, pString,  strlen(pString),  &TextSize);

	Initialize();																				// 버퍼를 지워준다.

	// 상단 깃발이 있을 경우 좌표 처리
	if (-1 != FlagXSize)
	{
		m_lpSelectCharacterFlagRenderPosX = FlagRenderPosX;
		m_lpSelectCharacterFlagRenderPosY = FlagRenderPosY;
	}

//	siTextWidth		=	(SI16)(TextSize.cx > TextSize1.cx ? TextSize.cx : TextSize1.cx) + 10;
	siTextWidth		=	(SI16)(TextSize.cx) + 10;
	siTextWidth		+=	FlagXSize + 5;
	if(siTextWidth > ON_HELP_WIDTH ) siTextWidth = ON_HELP_WIDTH;
//	siTextHeight	=	(SI16)(TextSize.cy > TextSize1.cy ? TextSize.cy : TextSize1.cy);
	siTextHeight	=	(SI16)(TextSize.cy) + 5;
	pTemp			=	pTextBuffer;

	// 라인수 계산
	siLine			=	2;

	// 보급품 퍼센트를 표시해야하는 경우라면
	if (bDrawSupplyGoods)
		scSpace = 10;
	else
		siLine		=	2;

	siDirection		=	siDirect;	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j< ((siTextHeight * siLine) + scSpace); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ; i< siTextWidth ; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j< ((siTextHeight * siLine) + scSpace); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}

	// 상단 깃발이 있는 경우
	if (-1 != FlagXSize)
	{
		// 공성전 유닛 체력 게이지 위치 설정
		siDrawGaugeHealthPosX   = 5 + FlagXSize + 5;
		siDrawGaugeHealthPosY   = TextSize.cy + 7;
	}
	else
	{
		// 공성전 유닛 체력 게이지 위치 설정
		siDrawGaugeHealthPosX   = 5;
		siDrawGaugeHealthPosY   = TextSize.cy + 7;
	}

	if( siColor & ON_HELP_RED )
	{
		siColor = 255;
		
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( siColor, 0, 0 ) );
//		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( siColor, 0, 0 ) );
	}
	else if( siColor & ON_HELP_WHITERED )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( siColor, siColor / 6, siColor / 8) );
//		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( siColor, siColor / 6, siColor / 8) );
	}
	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( 0, 0, siColor ) );
//		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( 0, 0, siColor ) );
	}
	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( 0, siColor, 0 ) );
//		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, RGB( 0, siColor, 0 ) );
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  pMyOnlineWorld->PaletteWhiteColor);
//		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pString1, pMyOnlineWorld->PaletteWhiteColor);
	}
}

// robypark 2005/2/17 11:50
// 커서 아래 캐릭터가 상단에 가입한 공성전 유닛일 경우 풍선 도움말 텍스트 설정
// [IN]  CHAR *pString: 캐릭터 이름 정보
// [IN]	 CHAR *pGuildName: 상단정보
// [IN]  SI16 siX: 출력할 위치
// [IN]  SI16 siY: 출력할 위치
// [IN]  SI16 siDirect: 출력할 방향 플래그
// [IN]  SI32 siColor: 텍스트 칼라
// [OUT] SI08 &siDrawGaugeHealthPosX: 체력 바 위치
// [OUT] SI08 &siDrawGaugeHealthPosY: 체력 바 위치
// [IN]  bool bDrawSupplyGoods: 공성전 유닛 남은 보급품(%)을 그려줘야하는지. 보급수레만 해당된다.
// [IN]  SI16 FlagXSize = -1: 상단 깃발 크기
// [IN]  SI16 FlagYSize = -1: 상단 깃발 크기
// [OUT] SI16* FlagRenderPosX = NULL: 상단 깃발 그려지는 위치
// [OUT] SI16* FlagRenderPosY = NULL: 상단 깃발 그려지는 위치
VOID	OnlineHelp::SetSiegeWarfareUnitToolTipText(CHAR *pString, CHAR *pGuildName, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, 
													   SI08 &siDrawGaugeHealthPosX, SI08 &siDrawGaugeHealthPosY, bool bDrawSupplyGoods,
													   SI16 FlagXSize/* = -1*/, SI16 FlagYSize/* = -1*/,
													   SI16* FlagRenderPosX/* = NULL*/, SI16* FlagRenderPosY/* = NULL*/)
{
	SIZE	TextSize;	
	SIZE	TextSize1;	
	UI08	*pTemp;
	SI16	i, j;	
	SI08	scSpace = 5;	// 여백

	if( pString == NULL )			return;

	OnlineHelp::siX		=	siX;
	OnlineHelp::siY		=	siY;

	GetTextExtentPoint32(MegaDC, pString,  strlen(pString),  &TextSize);
	GetTextExtentPoint32(MegaDC, pGuildName, strlen(pGuildName), &TextSize1);

	Initialize();																				// 버퍼를 지워준다.

	// 상단 깃발이 있을 경우 좌표 처리
	if (-1 != FlagXSize)
	{
		m_lpSelectCharacterFlagRenderPosX = FlagRenderPosX;
		m_lpSelectCharacterFlagRenderPosY = FlagRenderPosY;
	}

	siTextWidth		=	(SI16)(TextSize.cx > TextSize1.cx ? TextSize.cx : TextSize1.cx) + 10;
//	siTextWidth		=	(SI16)(TextSize.cx) + 10;
	siTextWidth		+=	FlagXSize + 5;

	if(siTextWidth > ON_HELP_WIDTH )
		siTextWidth = ON_HELP_WIDTH;

	siTextHeight	=	(SI16)(TextSize.cy > TextSize1.cy ? TextSize.cy : TextSize1.cy);
	siTextHeight	=	(SI16)(TextSize.cy) + 5;
	pTemp			=	pTextBuffer;

	// 라인수 계산
	siLine			=	3;

	// 보급품 퍼센트를 표시해야하는 경우라면
	if (bDrawSupplyGoods)
		scSpace = 10;
	else
		siLine		=	2;

	siDirection		=	siDirect;	

	bAction			=	TRUE;
	
	if( siTextWidth < ON_HELP_WIDTH )
	{
		for( j=0 ; j< ((siTextHeight * siLine) + scSpace); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ; i< siTextWidth ; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp	+= ON_HELP_WIDTH;
		}		
	}
	else		
	{
		for( j=0 ; j< ((siTextHeight * siLine) + scSpace); j++ )										// 텍스트 크기 만큼 뒷배경을 칠해준다.
		{
			for( i=0 ;i<ON_HELP_WIDTH; i++)
				pTemp[i] = ON_HELP_BACK_COLOR;
			
			pTemp += ON_HELP_WIDTH;
		}		

	}

	// 상단 깃발이 있는 경우
	if (-1 != FlagXSize)
	{
		// 공성전 유닛 체력 게이지 위치 설정
		siDrawGaugeHealthPosX   = 5 + FlagXSize + 5;
		siDrawGaugeHealthPosY   = TextSize.cy + TextSize.cy + 9;
	}
	else
	{
		// 공성전 유닛 체력 게이지 위치 설정
		siDrawGaugeHealthPosX   = 5;
		siDrawGaugeHealthPosY   = TextSize.cy + TextSize.cy + 9;
	}

	if( siColor & ON_HELP_RED )
	{
		siColor = 255;
		
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( siColor, 0, 0 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pGuildName, RGB( siColor, 0, 0 ) );
	}
	else if( siColor & ON_HELP_WHITERED )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( siColor, siColor / 6, siColor / 8) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pGuildName, RGB( siColor, siColor / 6, siColor / 8) );
	}
	else if( siColor & ON_HELP_BLUE )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( 0, 0, siColor ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pGuildName, RGB( 0, 0, siColor ) );
	}
	else if( siColor & ON_HELP_GREEN )
	{
		siColor = 255;

		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  RGB( 0, siColor, 0 ) );
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pGuildName, RGB( 0, siColor, 0 ) );
	}
	else if( siColor & ON_HELP_WHITE )	
	{
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, 5,               pString,  pMyOnlineWorld->PaletteWhiteColor);
		pMyOnlineWorld->pOnlineMegaText->DrawText( MegaDC, FlagXSize + 5, TextSize.cy + 5, pGuildName, pMyOnlineWorld->PaletteWhiteColor);
	}
}