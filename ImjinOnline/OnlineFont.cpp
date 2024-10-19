//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - SMS �޽��� ��º�
//	File Name		: OnlineSMS.cpp
//	Birth Date		: 2003. 06.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż��� �ʵ� �κп��� ���̴� ��� �ؽ�Ʈ���� ��Ʈ �������Դϴ�.
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

#include "../SCCommon/OnlineCommonParser.h"		// actdoll (2004/06/21 15:15) : ���� �ļ���


#define ON_FONT_MAX		10

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFonts::OnlineFonts()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFonts::~OnlineFonts()
{
	Free();
}

//-----------------------------------------------------------------------------
// Name: Init_NPC()													[private]
// Code: All (2003-12-23)
// Desc: ��Ʈ ����
//		return	- LoadFont()�� �����ϸ� false
//-----------------------------------------------------------------------------
BOOL	OnlineFonts::Create()
{
	return LoadFont();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��Ʈ�� �����Ѵ�.
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
// Desc: ��Ʈ�� �ؽ�Ʈ ���Ͽ��� �о� �´�.
//		return	- ���ϸ��� ��ã���� ��� ����
//-----------------------------------------------------------------------------
BOOL	OnlineFonts::LoadFont()
{
	FILE				*fp	= NULL;
	DWORD				CharSet;
	OnlineCommonParser	ocp;

	// actdoll (2004/02/23 15:55) : 
	//	�ٸ� ������� �ش� ������ �ε��Ѵ�.
	char	pFileName[256] = {0,};
	strcpy( pFileName, pGame->m_pszHQNation );		// �տ� �ش� ������ ������ ���̰�
	strcat( pFileName, "\\OnlineFontInfo.dat" );	// �ش� ���ϸ��� ���δ�.

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

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pFileName );
	ocp.SetErrFileName( pszErrName );

	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "OnlineFontInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �ޱ�
	int		iCount = 0;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		HFONT	font, fontline;
		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		char	pszFont[32];											// ���� ��Ʈ��, �۲�
		SI32	siIndex, siSize, siDepth;								// ��ȣ, ũ��, ����
		ocp.GetValue( siIndex,					iLineNo, iColNo );		// ��ȣ
		ocp.GetValue( pszFont,			31,		iLineNo, iColNo );		// ���� ��Ʈ��
		ocp.GetValue( siSize,					iLineNo, iColNo );		// ��Ʈ ũ��
		ocp.GetValue( siDepth,					iLineNo, iColNo );		// ��Ʈ ����
		ocp.GetValue( m_szFontName[iCount],	20,	iLineNo, iColNo );		// ��Ʈ �۲�
		
		// actdoll (2004/01/20 15:25) : ��Ʈ ũ�� ���������� ��� ���
		HDC		hdc			= GetDC( NULL );
		SI32	siTemp		= GetDeviceCaps( hdc, LOGPIXELSY );
		SI32	siHeight	= -MulDiv( siSize, siTemp, 72 );
		ReleaseDC( NULL, hdc );

		// ��Ʈ�� ���� �Ѵ�.
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

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}
		
		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )		break;
		
		iCount++;
	}

	ocp.Release();
	fclose( fp );
	fp = NULL;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��Ʈ�� ���Ϲ޴´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HFONT	OnlineFonts::GetFont(SI16 siIndex)
{
	return _Font[siIndex];
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��Ʈ�� ���Ϲ޴´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HFONT	OnlineFonts::GetFontUnderline(SI16 siIndex)
{
	return _FontUnderline[siIndex];
}
	
char*   OnlineFonts::GetFontName(SI16 siIndex)
{
	return m_szFontName[siIndex];
}
