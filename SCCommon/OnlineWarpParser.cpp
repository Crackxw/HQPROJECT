#include <stdio.h>
#include <stdlib.h>

#include <GSL.h>
#include "OnlineText.h"
#include "OnlineWarpParser.h"
#include "bindjxfile.h"
#include "OnlineCommonParser.h"

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : »ý¼ºÀÚ
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
cltOnlineWarpParser::cltOnlineWarpParser()
{
	m_siInfoCount		=	0;
	m_pstWarpInfoList	=	NULL;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : ¼Ò¸êÀÚ
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
cltOnlineWarpParser::~cltOnlineWarpParser()
{
	Free();	
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : ÃÊ±âÈ­ 
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
BOOL	cltOnlineWarpParser::Init( CHAR *pszFileName, OnlineText *pOnlineText )
{
	if( !pszFileName || !pOnlineText )										return FALSE;
	FILE	*fp = NULL;
	SI32	i;		
	OnlineCommonParser		ocp;
	
	// ÀÏ´Ü µ¥ÀÌÅÍ¸¦ ÀÐÀ» ¼ö ¾ø´Ù¸é ¸®ÅÏÀÌ´Ù.
	if( (fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL )	return FALSE;

	// actdoll (2004/08/23 14:10) : Ãâ·ÂµÉ ¿¡·¯ÆÄÀÏ¸í ÀûÀç
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "OnlineWarpParser Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// Á¤º¸µéÀÇ °¹¼ö¸¦ ¹Þ´Â´Ù.
	m_siInfoCount = ocp.GetMaxDataLineCount();
	
	// ¸Þ¸ð¸®¸¦ ÇÒ´ç ¹Þ´Â´Ù.
	m_pstWarpInfoList	=	new	strWarpInfo[m_siInfoCount];
	ZeroMemory( m_pstWarpInfoList, sizeof(strWarpInfo) * m_siInfoCount );

	// ÀÚ·á¸¦ ¹Þ´Â´Ù.
	i = 0;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// µ¥ÀÌÅÍ ¶óÀÎÀ» ÀÏ´Ü ÃßÃâÇÏ°í

		// ÄÃ·³º°·Î ÃßÃâÇÑ ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î Áý¾î³Ö´Â´Ù.
		int		iLineNo, iColNo;
		strWarpInfo	*pWI	= m_pstWarpInfoList + i;

		// ! ÁÖÀÇ ! - GetValue¸¦ »ç¿ëÇÏ±â ½ÃÀÛÇß´Ù¸é ÇØ´ç ¶óÀÎÀÇ ÀÚ·á´Â ¿¬¼ÓÇØ¼­ ÇÑ¹ø¿¡ ¹Þµµ·Ï ÇÑ´Ù.
		ocp.GetValue( (SI32&)pWI->siCode,					iLineNo, iColNo );	// ¹øÈ£
		ocp.GetValue( pWI->szDest,	MAX_WARP_DEST_LENGTH,	iLineNo, iColNo );	// ¼³¸í±Û
		ocp.GetValue( pWI->siOnlineTextNo,					iLineNo, iColNo );	// ¿Â¶óÀÎ ÅØ½ºÆ®¿¡¼­ÀÇ ¹øÈ£
		ocp.GetValue( (SI32&)pWI->cType,					iLineNo, iColNo );	// ±¹°¡ Å¸ÀÔ
		ocp.GetValue( (SI32&)pWI->uiX,						iLineNo, iColNo );	// ÁÂÇ¥ X
		ocp.GetValue( (SI32&)pWI->uiY,						iLineNo, iColNo );	// ÁÂÇ¥ Y
		ocp.GetValue( (SI32&)pWI->uiMapIndex,				iLineNo, iColNo );	// ¸Ê ¹øÈ£

		// actdoll (2004/06/08 14:23) : ÀÌÁ¦´Â ¿Â¶óÀÎ ÅØ½ºÆ® ¹øÈ£·Î ÄÁÆ®·ÑÇÏ±â ¶§¹®¿¡ szDestÀÇ Á¤º¸¸¦ ´Ù½Ã ¹Ù²ãÁØ´Ù.
		strncpy( pWI->szDest, pOnlineText->Get( pWI->siOnlineTextNo ), MAX_WARP_DEST_LENGTH );

		// È®ÀÎ»ç»ì - ¸¸¾à Áö±Ý Ã³¸®ÇÑ °ÍÀÌ ¸¶Áö¸· ÄÃ·³ÀÌ¾ú´Ù¸é Á¤»óÀÌ´Ù.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineWarpParser Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}

		// È®ÀÎ »ç»ì - ÀÌ°ÍÀÌ Áö±Ý ¸¶Áö¸· ¶óÀÎÂ°¸¦ Ã³¸®ÇÏ°í ÀÖ´Ù¸é ºê·¹ÀÌÅ©
		if( ocp.IsEndOfLine() )		break;

		// ³Ñ¾î°¬À¸¸é Ä«¿îÆ® ¿Ã¸®°í
		i++;
	}

	ocp.Release();
	fclose( fp );

	return TRUE;
}

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : ¿öÇÁ ¸ñÀûÁö·Î ¿öÇÁ ÄÚµå¸¦ ¾ò¾î³½´Ù.
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
SI32	cltOnlineWarpParser::GetWarpCode( CHAR *szDest )
{
	SI32		i;
	
	for( i = 0; i < m_siInfoCount; i++ )
	{
		if( stricmp( m_pstWarpInfoList[i].szDest, szDest ) == 0 )
		{
			return m_pstWarpInfoList[i].siCode;
		}
	}

	return	-1;
}

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : ¿öÇÁ ¸ñÀûÁö·Î ¿öÇÁ Á¤º¸¸¦ ¾ò¾î³½´Ù.
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
strWarpInfo*	cltOnlineWarpParser::GetWarpInfo( CHAR *szDest )
{
	SI32		i;
	
	for( i = 0; i < m_siInfoCount; i++ )
	{
		if( stricmp( m_pstWarpInfoList[i].szDest, szDest ) == 0 )
		{
			return &m_pstWarpInfoList[i];
		}
	}

	return	NULL;
}

//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : ¿öÇÁ ÄÚµå·Î ¿öÇÁ Á¤º¸¸¦ ¾ò¾î³½´Ù.
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
strWarpInfo*	cltOnlineWarpParser::GetWarpInfo( SI32 siCode )
{
	SI32		i;
	
	for( i = 0; i < m_siInfoCount; i++ )
	{
		if( m_pstWarpInfoList[i].siCode == siCode )
		{
			return &m_pstWarpInfoList[i];
		}
	}

	return	NULL;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
// Desc : ÇØÁ¦
//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
VOID	cltOnlineWarpParser::Free()
{
	if( m_pstWarpInfoList != NULL )
	{
		delete [] m_pstWarpInfoList;
		m_pstWarpInfoList	=	NULL;
	}

	m_siInfoCount	=	0;
}