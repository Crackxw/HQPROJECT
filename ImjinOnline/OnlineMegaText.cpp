/********************************************************************************************
	파일명	:	OnlineMegaText.cpp
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2001.05.23
	수정일	:	2001.10.15
	내  용  :	텍스트 출력을 담당
********************************************************************************************/

#include <GSL.h>
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include "GSCDefine.h"
#include "Utility.h"

BYTE OnlineMegaText::m_ColorTable[256];

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineMegaText::OnlineMegaText()
{
	m_pBitInfo  = NULL;
	m_pOnlineResource = NULL;
	m_pOnlineFonts = NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineMegaText::~OnlineMegaText()
{
	ReMoveAll();
}

VOID	OnlineMegaText::Init()
{
	SI16	siBuffer = 0;
	int		nUsage = DIB_RGB_COLORS;
	SI16	i;

	if( !m_pBitInfo )
	{
		m_pBitInfo = (BITMAPINFO*)new LPBITMAPINFO[ sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256) ];
		ZeroMemory( m_pBitInfo, sizeof( BYTE ) * ( sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 ) );

		m_pBitInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
		m_pBitInfo->bmiHeader.biWidth		= ON_TEXT_XSIZE;
		m_pBitInfo->bmiHeader.biHeight		= -ON_TEXT_YSIZE;
		m_pBitInfo->bmiHeader.biBitCount	= ON_TEXT_BITCOUNT;
		m_pBitInfo->bmiHeader.biCompression	= BI_RGB;
		m_pBitInfo->bmiHeader.biPlanes		= 1;

		for( i=0 ; i<256 ; i++)
		{
			m_pBitInfo->bmiColors[i].rgbRed		= clGrp.Palette[i * 3 + 0] * 4 ;
			m_pBitInfo->bmiColors[i].rgbGreen	= clGrp.Palette[i * 3 + 1] * 4 ;
			m_pBitInfo->bmiColors[i].rgbBlue	= clGrp.Palette[i * 3 + 2] * 4 ;
		}
	}


	m_MegaDC = ::CreateCompatibleDC(NULL);

	SetBkMode( m_MegaDC, TRANSPARENT );
	SetTextColor( m_MegaDC, RGB( 240, 240, 240 ) );
	SelectObject( m_MegaDC, m_pOnlineFonts->GetFont( ON_FONT_CHAT ) );

	m_hBmp	= ::CreateDIBSection(m_MegaDC, m_pBitInfo, nUsage, (void **)&m_pData, NULL, 0);
	hOldBmp	= (HBITMAP)SelectObject(m_MegaDC, m_hBmp);

	unsigned char HalfR = (clGrp.PaletteRGB[ 0 ][0] >>1);
	unsigned char HalfG = (clGrp.PaletteRGB[ 0 ][1] >>1);
	unsigned char HalfB = (clGrp.PaletteRGB[ 0 ][2] >>1);


	for( i=0; i<256; i++ )
	{
		m_ColorTable[i] = clGrp.Palette16[ (clGrp.PaletteRGB[ i ][0]>>1) + HalfR]
							  		     [ (clGrp.PaletteRGB[ i ][1]>>1) + HalfG]
									     [ (clGrp.PaletteRGB[ i ][2]>>1) + HalfB];
	}


	for( i=0; i<ON_TEXT_MAX_BUFFER; i++ )
	{
		_TextBuffer[i].pTextBuffer = new BYTE[ ON_TEXT_XSIZE * ON_TEXT_YSIZE ];		
		_TextBuffer[i].uiID = 0;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	초기화
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineMegaText::Initialize(OnlineResource *pOnlineResource, OnlineFonts *pOnlineFonts)
{
	m_pOnlineResource = pOnlineResource;
	m_pOnlineFonts = pOnlineFonts;

	Init();

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트를 출력한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::DrawText( HDC hdc, SI16 x, SI16 y, char *pString, COLORREF rgb )
{
	if( pString == NULL )	return;

	SetBkMode( hdc, TRANSPARENT );	
	SetTextColor( hdc, rgb );
	TextOut( hdc, x, y, pString, strlen( pString ) );
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	말풍선
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::DrawText(UINT uiID, char *pString, COLORREF rgb, BOOL bFlag)
{
	SIZE	TextSize;			
	SI16	siCnt			 =	0;	
	SI16	LineLength	     =  0;

	UI08	*pData			 =	NULL;
	BOOL	bSkip			 =	FALSE;

	XSPR	*pChat_Spr		 =  NULL;

	MEGATEXTBUFFER	*pBuffer = FindTextBuffer( uiID );

	if( pBuffer == NULL )	    				 return;

	// 만약 버퍼가 널이면 .. 다시 생성해 준다.
	if( !pBuffer->pTextBuffer )
	{
		Free(uiID);
		CreateGDIPage(uiID);

		pBuffer = FindTextBuffer( uiID );
	}
	
	pChat_Spr = m_pOnlineResource->GetxsprGame( ON_GAMESPR_BALLOON );

	memset( m_pData, 254, ON_TEXT_XSIZE * ON_TEXT_YSIZE );

	GetTextExtentPoint32(m_MegaDC, pString, strlen( pString ), &TextSize);											// 먼저 총 글자의 픽셀 수를 얻어 온다.

	pBuffer->siTextWidth  = (SI16)TextSize.cx;																		// 텍스트의 가로 길이를 구해온다.
	
	if( pBuffer->siTextWidth  >= ON_CHAT_MAX_BOX )		pBuffer->siTextWidth = ON_CHAT_MAX_BOX;						// 말풍선의 최대 크기
	if( pBuffer->siTextWidth  <= ON_CHAT_MIN_BOX )		pBuffer->siTextWidth = ON_CHAT_MIN_BOX;						// 말풍선의 최소 크기
						
	LineLength = SI16( (float)(TextSize.cx - 5)  / (float)ON_CHAT_MAX_BOX );										// 총 몇 라인 인가를 알아본다.		
	
	if( LineLength == 0 )		pBuffer->siTextHeight = (SI16)TextSize.cy + 4;
	else						pBuffer->siTextHeight = ( LineLength + 1 ) * ON_CHAT_TEXT_CAP + 4;					// 말 풍선의 세로 크기를 구한다.	
	
	// 말풍선 = TRUE이면 말풍선을 출력을 한다.
	if( bFlag )
	{
		if( ( clGrp.LockSurface(m_pData, ON_TEXT_XSIZE, ON_TEXT_YSIZE) ) == TRUE )
		{	
			// 한 줄일 경우			
			if( LineLength == 0 )
			{	
				// 가로
				for( SI16 ii=0; ii <= pBuffer->siTextWidth / pChat_Spr->Header.Xsize; ii++)
				{
					clGrp.PutSpriteT( 2+ii * pChat_Spr->Header.Xsize, 0           , pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[pChat_Spr->Header.Start[ON_CHAT_BAR_TOP]]    );
					clGrp.PutSpriteT( 2+ii * pChat_Spr->Header.Xsize, pBuffer->siTextHeight, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[pChat_Spr->Header.Start[ON_CHAT_BAR_BOTTOM]] );
				}


				clGrp.PutSpriteT( 0              , pChat_Spr->Header.Ysize, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize - 1, &pChat_Spr->Image[pChat_Spr->Header.Start[ ON_CHAT_BAR_RIGTH ] ]);
				clGrp.PutSpriteT( pBuffer->siTextWidth + 3, pChat_Spr->Header.Ysize, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize - 1, &pChat_Spr->Image[pChat_Spr->Header.Start[ ON_CHAT_BAR_LEFT ] ] );

				clGrp.PutSpriteT( pBuffer->siTextWidth + 3, 0           , pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_RIGTH_UP ] ]  );		
				clGrp.PutSpriteT( pBuffer->siTextWidth + 3, pBuffer->siTextHeight, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_RIGTH_DOWN ] ]);		

				clGrp.PutSpriteT( 0              , 0           , pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_LEFT_UP ] ]   );
				clGrp.PutSpriteT( 0              , pBuffer->siTextHeight, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_LEFT_DOWN ] ] );
				clGrp.PutSpriteT( pBuffer->siTextWidth / 2, pBuffer->siTextHeight, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_CENTER ] ]    );								


			}

			// 두줄 
			else
			{
				// 가로
				for( SI16 ii=0; ii <= pBuffer->siTextWidth / pChat_Spr->Header.Xsize; ii++)
				{
					clGrp.PutSpriteT( 3+ii * pChat_Spr->Header.Xsize, 0               , pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[pChat_Spr->Header.Start[ON_CHAT_BAR_TOP]]    );
					clGrp.PutSpriteT( 3+ii * pChat_Spr->Header.Xsize, pBuffer->siTextHeight - 5, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[pChat_Spr->Header.Start[ON_CHAT_BAR_BOTTOM]] );
				}

				// 세로
				for( ii=0 ; ii <= LineLength; ii++)
				{
					clGrp.PutSpriteT( 0              , (ii+1) * pChat_Spr->Header.Ysize, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[pChat_Spr->Header.Start[ ON_CHAT_BAR_RIGTH ] ]);
					clGrp.PutSpriteT( pBuffer->siTextWidth + 7, (ii+1) * pChat_Spr->Header.Ysize, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[pChat_Spr->Header.Start[ ON_CHAT_BAR_LEFT ] ] );
				}

				clGrp.PutSpriteT( pBuffer->siTextWidth + 7, 0               , pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_RIGTH_UP ] ]  );		
				clGrp.PutSpriteT( pBuffer->siTextWidth + 7, pBuffer->siTextHeight - 5, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_RIGTH_DOWN ] ]);
				
				clGrp.PutSpriteT( 0                  , 0               , pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_LEFT_UP ] ]   );
				clGrp.PutSpriteT( 0                  , pBuffer->siTextHeight - 5, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_LEFT_DOWN ] ] );
				clGrp.PutSpriteT( pBuffer->siTextWidth / 2 - 5, pBuffer->siTextHeight - 5, pChat_Spr->Header.Xsize, pChat_Spr->Header.Ysize, &pChat_Spr->Image[ pChat_Spr->Header.Start[ ON_CHAT_CENTER ] ]    );								

			}
		}
	}

	pData = m_pData;

	// 말풍선 색칠 하기..
	for( SI16 iy = 0 ; iy < ON_TEXT_YSIZE ; iy++)
	{
		for( SI16 ix = 0 ; ix < ON_TEXT_XSIZE ; ix++)
		{				
			if( pData[ix] == 255 )							bSkip = TRUE;
			if( pData[ix] == 242 || pData[ix] == 241 )		bSkip = FALSE;			
			if( bSkip )										pData[ix] = 0;
		}			

		pData += ON_TEXT_XSIZE;
	}

	// 만약 한줄에 찍을 수가 있다면
	SetTextColor(m_MegaDC, rgb);
	if(LineLength == 0)	DrawTextCenter( m_MegaDC, 0, 0, TextSize.cx + 15, TextSize.cy + 12, pString, rgb);	
	else			  	DrawTextLine( m_MegaDC, 6, 5, pBuffer->siTextWidth, pString, rgb);	

	memcpy( pBuffer->pTextBuffer, m_pData, ON_TEXT_XSIZE * ON_TEXT_YSIZE );

}

MEGATEXTBUFFER	*OnlineMegaText::FindTextBuffer( UINT uiID )
{
	for( int i=0; i<ON_TEXT_MAX_BUFFER; i++ )
	{
		if( _TextBuffer[i].uiID == uiID )
			return &_TextBuffer[i];
	}
	
	return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	ID를 받아 ID와 동일한 ID를 날린다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::Free(UINT uiID)
{	
	MEGATEXTBUFFER *pBuffer = FindTextBuffer(uiID);
	if( pBuffer )	pBuffer->uiID = 0;
}

VOID	OnlineMegaText::destroyBuffer(MEGATEXTBUFFER *b)
{
	if( b->pTextBuffer )	delete []b->pTextBuffer;
	b->pTextBuffer = NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	모두 날림.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::ReMoveAll()
{
	for( int i=0; i<ON_TEXT_MAX_BUFFER; i++ )
		destroyBuffer( &_TextBuffer[i] );
	

	if( m_hBmp )	DeleteObject( m_hBmp );
	if( m_MegaDC )	SelectObject( m_MegaDC, hOldBmp );
	if( m_MegaDC )	DeleteDC( m_MegaDC );

	if( m_pBitInfo )
	{
		delete []m_pBitInfo;
		m_pBitInfo	=	NULL;
	}

	m_hBmp = NULL;
	m_MegaDC = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트 버퍼 DC 에서 임의 DC로 전송
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::Show( UINT uiID, LPDIRECTDRAWSURFACE7 lpSurface, SI16 x, SI16 y )
{
	MEGATEXTBUFFER	*pBuffer = FindTextBuffer( uiID );

	if( pBuffer == NULL )	return;

	UI08	*pDst		= NULL;
	UI08	*pSrc		= pBuffer->pTextBuffer;
	SI16	ix;

	if(!pSrc)	return;


	SI16	siHeight	= ON_TEXT_YSIZE;
	SI16	siWidth		= ON_TEXT_XSIZE;
	SI16	siPitch		= clGrp.GetSurfaceWidth();


	y	=	y - pBuffer->siTextHeight;
	x	=	x - ( pBuffer->siTextWidth / 2 );

	// 준태 조금 최적화

	//클리핑 처리 쩝..
	if(x + siWidth < 0 || y + siHeight < 0 || x >= ON_SCREEN_XSIZE || y >= ON_SCREEN_YSIZE)
		return;

	if(y < 0)					//항상 Y부터 클리핑 시작 왜?? 그림이 꺼꾸로~~~
	{			
		pSrc	-= siWidth*y;
		siHeight += y;
		y		 = 0;
	}

	if(y + siHeight >= ON_SCREEN_YSIZE)	
		siHeight = ON_SCREEN_YSIZE - y - 1;

	SI16	nl_Width = siWidth;

	if(x < 0)
	{
		pSrc     -= x;
		nl_Width += x;
		x		  = 0;
	}

	if(x + siWidth >= ON_SCREEN_XSIZE)
		nl_Width = ON_SCREEN_XSIZE - x - 1;
	

	if( clGrp.LockSurface( lpSurface ) == TRUE )
	{
		pDst  = clGrp.Screen8;		
	
		pDst += ( y * siPitch ) + x;		
		
		while( siHeight )
		{
			for( ix=nl_Width-1 ; ix>=0; --ix)
			{
				if( pSrc[ix] == TRANSCOLOR )	continue;

				pDst[ix] = pSrc[ix] == 0 ? pDst[ix] = m_ColorTable[pDst[ix]] : pSrc[ix];
			}

			pDst += siPitch;
			pSrc += siWidth;

			--siHeight;
		}
		clGrp.UnlockSurface( lpSurface );
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트 버퍼 생성
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID OnlineMegaText::CreateGDIPage( UINT uiID )
{	
	if(uiID > 0 && uiID <= MAX_PLAYER_NUM+1)
	{
		for( int i=0; i<ON_TEXT_MAX_BUFFER; i++ )
		{
			if( _TextBuffer[i].uiID == 0 )
			{
				_TextBuffer[i].uiID = uiID;
				return;
			}
		}
	}	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	TEXT를 중간에 찍는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::DrawTextCenter( HDC hDC, SI16 x, SI16 y, SI16 siWidth, SI16 siHeight, char *pString, COLORREF rgb )
{
	if( pString == NULL )	return;

	SIZE	TextSize;
	SI16	siCenterX;
	SI16	siCenterY;

	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );

	siCenterX = ( siWidth  - TextSize.cx ) / 2;
	siCenterY = ( siHeight - TextSize.cy ) / 2;

	SetBkColor(hDC, TRANSPARENT);
	SetTextColor( hDC, rgb );
	TextOut( hDC, x + siCenterX, y + siCenterY, pString, strlen( pString ) );
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	TEXT를 줄넘기며 찍는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::DrawTextLine( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb)
{
	SIZE	TextSize;	
	SI16	siCnt  = 0;
	SI16	siLine = 0;
	SI16	siHeight;
	char	szLineBuffer[ON_TEXT_MAX_BUFFER];

	if(!hDC)	return;

	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );
	siHeight = (SI16)TextSize.cy;

	while( *pString != NULL )
	{			
		ZeroMemory( szLineBuffer, ON_TEXT_MAX_BUFFER );
		
		while( pString[siCnt] != NULL )
		{				
			if( UI08(pString[siCnt]) > 127 )											// 만약 글자가 한글이면 한 바이트더 읽어 들인다.
			{	
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.

				szLineBuffer[siCnt] = pString[siCnt];
				siCnt++;
				szLineBuffer[siCnt] = pString[siCnt];
				siCnt++;
			}
			else																		// 특수 문자 나 영문
			{
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.
				// 줄바꾸기 문자~
				if(  pString[siCnt] == 10)            {
					 pString[siCnt] = 0x20;
					 szLineBuffer[siCnt] = pString[siCnt++];					
					break;
				}
				
				szLineBuffer[siCnt] = pString[siCnt++];					
			}							
		}
		
		pString += siCnt;
		siCnt    = 0;		
		
		// 첫번째 글자가 공백이면 한칸씩 밀어 준다.
		if( szLineBuffer[0] == ' '  || szLineBuffer[0] == ',' || szLineBuffer[0] == '.' || szLineBuffer[0] == 0)
			memmove( szLineBuffer, szLineBuffer + 1, ON_TEXT_MAX_BUFFER - 1 );		
		
		SetTextColor(hDC, rgb);
		TextOut( hDC, x, y + ( siLine * ( siHeight + 2 ) ),szLineBuffer, strlen( szLineBuffer ) );
		siLine++;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트를 한줄한줄 출력 & 리턴를 체크 함	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI16	OnlineMegaText::DrawTextLine2( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb)
{
	SIZE	TextSize;	
	SI16	siCnt  = 0;
	SI16	siLine = 0;
	SI16	siHeight;
	char	szLineBuffer[ON_TEXT_MAX_BUFFER];

	SetTextColor( hDC, rgb );
	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );
	siHeight = (SI16)TextSize.cy;

	while( *pString != NULL )
	{			
//		if( siLine > 10 )									break;

		ZeroMemory( szLineBuffer, ON_TEXT_MAX_BUFFER );
		
		while( pString[siCnt] != NULL )
		{				
			if( UI08(pString[siCnt]) > 127 )											// 만약 글자가 한글이면 한 바이트더 읽어 들인다.
			{	
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.
				
				szLineBuffer[siCnt] = pString[siCnt++];
				szLineBuffer[siCnt] = pString[siCnt++];					
			}
			else																		// 특수 문자 나 영문
			{
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.

				if( pString[siCnt] == '\t' )
				{
					pString[siCnt] = ' ';
				}
				

				if( pString[siCnt] == '\n' || pString[siCnt] == '\r' )
					if( pString[siCnt+1] == '\n' || pString[siCnt+1] == '\r' )
					{
						siCnt += 2;
						break;
					}

				szLineBuffer[siCnt] = pString[siCnt++];
			}							
		}
		
		pString += siCnt;
		siCnt    = 0;		
		
		// 첫번째 글자가 공백이면 한칸씩 밀어 준다.
		if( szLineBuffer[0] == ' '  || szLineBuffer[0] == ',' || szLineBuffer[0] == '.' )
			memmove( szLineBuffer, szLineBuffer + 1, ON_TEXT_MAX_BUFFER - 1 );
		
		TextOut( hDC, x, y + ( siLine * ( siHeight + 2 ) ),szLineBuffer, strlen( szLineBuffer ) );
		siLine++;
	}
	return siLine;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트를 한줄한줄 출력 & 리턴를 체크 함	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::DrawTextLine3( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString, COLORREF rgb)
{
	SIZE	TextSize;	
	SI16	siCnt  = 0;
	SI16	siLine = 0;
	SI16	siHeight;
	char	szLineBuffer[ON_TEXT_MAX_BUFFER];

	SetTextColor( hDC, rgb );
	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );
	siHeight = (SI16)TextSize.cy;

	while( *pString != NULL )
	{			
		if( siLine > 10 )									break;

		ZeroMemory( szLineBuffer, ON_TEXT_MAX_BUFFER );
		
		while( pString[siCnt] != NULL )
		{				
			if( UI08(pString[siCnt]) > 127 )											// 만약 글자가 한글이면 한 바이트더 읽어 들인다.
			{	
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.
				
				szLineBuffer[siCnt] = pString[siCnt++];
				szLineBuffer[siCnt] = pString[siCnt++];					
			}
			else																		// 특수 문자 나 영문
			{
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.

				if( pString[siCnt] == '\t' )
				{
					pString[siCnt] = ' ';
				}
				

				if( pString[siCnt] == '\n' || pString[siCnt] == '\r' )
				{
					szLineBuffer[siCnt] = NULL;
					siCnt += 1;					

					if( pString[siCnt+1] == '\n' || pString[siCnt+1] == '\r' )
					{
						siCnt += 1;
					}

					break;
				}						

				szLineBuffer[siCnt] = pString[siCnt++];
			}							
		}
		
		pString += siCnt;
		siCnt    = 0;		
		
		// 첫번째 글자가 공백이면 한칸씩 밀어 준다.
		if( szLineBuffer[0] == ' '  || szLineBuffer[0] == ',' || szLineBuffer[0] == '.' )
			memmove( szLineBuffer, szLineBuffer + 1, ON_TEXT_MAX_BUFFER - 1 );
		
		TextOut( hDC, x, y + ( siLine * ( siHeight + 2 ) ),szLineBuffer, strlen( szLineBuffer ) );
		siLine++;
	}
}

//-----------------------------------------------------------------------------
// Name: DrawTextInBox()
// Code: actdoll (2004-08-24)
// Desc: 텍스트를 박스 형태의 공간 내에 출력시킨다.
//		hDC				- 출력대상 DC
//		x, y			- 출력 박스의 좌상단 좌표
//		width, height	- 출력 박스의 너비/높이
//		pDestRect		- 출력 범위를 가지고 있는 RECT의 포인터, 이것이 NULL이면 x, y, width, height가 사용되고, 이것이 존재한다면 우선된다.
//		pString			- 출력할 문자열
//		rgb				- 출력 문자의 색깔
//		dwSortFlag		- 좌중우, 상중하 출력 플래그, DT_LEFT / DT_CENTER / DT_RIGHT | DT_TOP / DT_VCENTER / DT_BOTTOM
//-----------------------------------------------------------------------------
VOID	OnlineMegaText::DrawTextInBox( HDC hDC, SI16 x, SI16 y, SI16 width, SI16 height, LPRECT pDestRect, char *pString, COLORREF rgb, DWORD dwSortFlag )
{
	RECT	rcDest;
	
	if( !pDestRect )	SetRect( &rcDest, x, y, x + width, y + height );
	else				rcDest = *pDestRect;

	dwSortFlag |= DT_NOPREFIX | DT_WORDBREAK | DT_MODIFYSTRING | DT_WORD_ELLIPSIS | DT_EXPANDTABS;

	SetTextColor( hDC, rgb );

	DrawTextEx( hDC, pString, -1, &rcDest, dwSortFlag, NULL );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트를 한줄한줄 출력 & 리턴를 체크 한 줄 길이를 구함	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	OnlineMegaText::CheckTextLine2( HDC hDC, SI16 x, SI16 y, SI16 siWidth, char *pString )
{
	SIZE	TextSize;	
	SI16	siCnt    = 0;
	SI16	siLine   = 0;
	SI16	siHeight = 0;;
	char	szLineBuffer[ON_TEXT_MAX_BUFFER];

	SetTextColor( hDC, RGB( 210, 210, 210 ) );
	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );
	siHeight = (SI16)TextSize.cy;

	while( *pString != NULL )
	{			
		if( siLine > 10 )									break;

		ZeroMemory( szLineBuffer, ON_TEXT_MAX_BUFFER );
		
		while( pString[siCnt] != NULL )
		{				
			if( UI08(pString[siCnt]) > 127 )											// 만약 글자가 한글이면 한 바이트더 읽어 들인다.
			{	
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.
				
				szLineBuffer[siCnt] = pString[siCnt++];
				szLineBuffer[siCnt] = pString[siCnt++];					
			}
			else																		// 특수 문자 나 영문
			{
				GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// 계속해서 버퍼에 있는 텍스트 길이를 구한다.
				
				if( TextSize.cx >= siWidth )				break;						// 텍스트 길이 보다 버퍼에 길이가 크면 다음 줄로 넘긴다.

				if( pString[siCnt] == '\t' )
				{
					pString[siCnt] = ' ';
				}
				

				if( pString[siCnt] == '\n' || pString[siCnt] == '\r' )
					if( pString[siCnt+1] == '\n' || pString[siCnt+1] == '\r' )
					{
						siCnt += 2;
						break;
					}
						

				szLineBuffer[siCnt] = pString[siCnt++];					
			}							
		}
		
		pString += siCnt;
		siCnt    = 0;		
		
		// 첫번째 글자가 공백이면 한칸씩 밀어 준다.
		if( szLineBuffer[0] == ' '  || szLineBuffer[0] == ',' || szLineBuffer[0] == '.' )
			memmove( szLineBuffer, szLineBuffer + 1, ON_TEXT_MAX_BUFFER - 1 );		
			
		siLine++;
	}

	return (siLine * (siHeight + 2));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	텍스트를 오른쪽으로에 마춰서 찍는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineMegaText::DrawTextRight( HDC hDC, SI16 x, SI16 y, char *pString, COLORREF rgb )
{
	SIZE	TextSize;

	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );
	DrawText( hDC, x - (SI16)TextSize.cx, y, pString, rgb );
}

VOID	OnlineMegaText::DrawLineCenter( HDC hDC, SI16 x1, SI16 x2, SI16 y, char *pString, COLORREF rgb )
{
	SIZE TextSize;
	SI16 iLineSize;
	iLineSize = x2 - x1;

	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );
	DrawText( hDC, x1 + (iLineSize/2) - (SI16)(TextSize.cx/2), y, pString, rgb );
	
}