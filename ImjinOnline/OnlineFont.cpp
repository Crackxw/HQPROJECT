//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - SMS 메시지 출력부
//	File Name		: OnlineSMS.cpp
//	Birth Date		: 2003. 06.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상의 필드 부분에서 쓰이는 모든 텍스트들의 폰트 데이터입니다.
//
//===================================================================================================

#include <GSL.h>

#ifndef _IMJINONLINESERVER
#include	<clGame.h>

extern _clGame* pGame;
#endif



#include "Online.h"
#include <OnlineFont.h>
#include "BindJXFile.h"

#include "../SCCommon/OnlineCommonParser.h"		// actdoll (2004/06/21 15:15) : 공통 파서기


#define ON_FONT_MAX		10

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFonts::OnlineFonts()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFonts::~OnlineFonts()
{
	Free();
}

//-----------------------------------------------------------------------------
// Name: Init_NPC()													[private]
// Code: All (2003-12-23)
// Desc: 폰트 생성
//		return	- LoadFont()가 실패하면 false
//-----------------------------------------------------------------------------
BOOL	OnlineFonts::Create()
{
	return LoadFont();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	폰트를 해제한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineFonts::Free()
{
	if( !_Font.empty() )
	{
		for( vector<HFONT>::iterator i=_Font.begin(); i!=_Font.end(); ++i)		
			DeleteObject(i);

		for( i=_FontUnderline.begin(); i!=_FontUnderline.end(); ++i)
			DeleteObject(i);		
	}
}

//-----------------------------------------------------------------------------
// Name: LoadFont()													[private]
// Code: All (2003-12-23)
// Desc: 폰트를 텍스트 파일에서 읽어 온다.
//		return	- 파일명을 못찾았을 경우 리턴
//-----------------------------------------------------------------------------
BOOL	OnlineFonts::LoadFont()
{
	FILE				*fp	= NULL;
	DWORD				CharSet;
	OnlineCommonParser	ocp;

	// actdoll (2004/02/23 15:55) : 
	//	다른 방식으로 해당 정보를 로딩한다.
	char	pFileName[256] = {0,};
	strcpy( pFileName, pGame->m_pszHQNation );		// 앞에 해당 국가의 폴더명 붙이고
	strcat( pFileName, "\\OnlineFontInfo.dat" );	// 해당 파일명을 붙인다.

	CharSet =   HANGUL_CHARSET;
	switch(pGame->LanguageCode)
	{
		case ELANGAGECODE_KOREAN:										CharSet =   HANGUL_CHARSET;			break;
		case ELANGAGECODE_TAIWAN:										CharSet =   CHINESEBIG5_CHARSET;	break;
		case ELANGAGECODE_JAPAN:										CharSet =   SHIFTJIS_CHARSET;		break;
		case ELANGAGECODE_HONGKONG:										CharSet =   CHINESEBIG5_CHARSET;	break;
		case ELANGAGECODE_CHINA:										CharSet =   DEFAULT_CHARSET;		break;
		case ELANGAGECODE_INDONESIA:									CharSet =   DEFAULT_CHARSET;		break;
	}
		
	if( (fp = BindJXFile::GetInstance()->GetFile( pFileName )) == NULL )
	{
		clGrp.Error("OnlineFontInfo", "%s - File Open Error!", pFileName);
		return FALSE;
	}

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pFileName );
	ocp.SetErrFileName( pszErrName );

	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "OnlineFontInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 받기
	int		iCount = 0;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		HFONT	font, fontline;
		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		char	pszFont[32];											// 지정 폰트명, 글꼴
		SI32	siIndex, siSize, siDepth;								// 번호, 크기, 굵기
		ocp.GetValue( siIndex,					iLineNo, iColNo );		// 번호
		ocp.GetValue( pszFont,			31,		iLineNo, iColNo );		// 지정 폰트명
		ocp.GetValue( siSize,					iLineNo, iColNo );		// 폰트 크기
		ocp.GetValue( siDepth,					iLineNo, iColNo );		// 폰트 굵기
		ocp.GetValue( m_szFontName[iCount],	20,	iLineNo, iColNo );		// 폰트 글꼴
		
		// actdoll (2004/01/20 15:25) : 폰트 크기 정상적으로 찍는 방법
		HDC		hdc			= GetDC( NULL );
		SI32	siTemp		= GetDeviceCaps( hdc, LOGPIXELSY );
		SI32	siHeight	= -MulDiv( siSize, siTemp, 72 );
		ReleaseDC( NULL, hdc );

		// 폰트를 생성 한다.
		font		=	CreateFont( siHeight, 0, 0, 0, siDepth, FALSE, FALSE, 0, CharSet, 0, 0, 0, 0, m_szFontName[iCount] );
		if(font == NULL)
		{
			clGrp.Error("Error Font", "Count : %d, Not Find Font : %s, %d, %d", siIndex, m_szFontName[iCount], siHeight, siDepth);
			font	=	CreateFont( siHeight, 0, 0, 0, siDepth, FALSE, FALSE, 0, CharSet, 0, 0, 0, 0, NULL );
		}

		fontline	=	CreateFont( siHeight, 0, 0, 0, siDepth, FALSE, TRUE, 0, CharSet, 0, 0, 0, 0, m_szFontName[iCount] );
		if(fontline == NULL)
		{
			clGrp.Error("Error Font", "Count : %d, Not Find Font : %s, %d, %d", siIndex, m_szFontName[iCount], siHeight, siDepth);
			fontline	=	CreateFont( siHeight, 0, 0, 0, siDepth, FALSE, TRUE, 0, CharSet, 0, 0, 0, 0, NULL );
		}

		_Font.push_back( font );
		_FontUnderline.push_back( fontline );

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )		break;
		
		iCount++;
	}

	ocp.Release();
	fclose( fp );
	fp = NULL;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	폰트를 리턴받는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HFONT	OnlineFonts::GetFont(SI16 siIndex)
{
	return _Font[siIndex];
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	폰트를 리턴받는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HFONT	OnlineFonts::GetFontUnderline(SI16 siIndex)
{
	return _FontUnderline[siIndex];
}
	
char*   OnlineFonts::GetFontName(SI16 siIndex)
{
	return m_szFontName[siIndex];
}
