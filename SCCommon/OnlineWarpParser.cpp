#include <stdio.h>
#include <stdlib.h>

#include <GSL.h>
#include "OnlineText.h"
#include "OnlineWarpParser.h"
#include "bindjxfile.h"
#include "OnlineCommonParser.h"

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : 儅撩濠
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
cltOnlineWarpParser::cltOnlineWarpParser()
{
	m_siInfoCount		=	0;
	m_pstWarpInfoList	=	NULL;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : 模資濠
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
cltOnlineWarpParser::~cltOnlineWarpParser()
{
	Free();	
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : 蟾晦�� 
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
BOOL	cltOnlineWarpParser::Init( CHAR *pszFileName, OnlineText *pOnlineText )
{
	if( !pszFileName || !pOnlineText )										return FALSE;
	FILE	*fp = NULL;
	SI32	i;		
	OnlineCommonParser		ocp;
	
	// 橾欽 等檜攪蒂 檗擊 熱 橈棻賊 葬欐檜棻.
	if( (fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL )	return FALSE;

	// actdoll (2004/08/23 14:10) : 轎溘腆 縑楝だ橾貲 瞳營
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// だ諒擊 衛濛и棻.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "OnlineWarpParser Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 薑爾菟曖 偎熱蒂 嫡朝棻.
	m_siInfoCount = ocp.GetMaxDataLineCount();
	
	// 詭賅葬蒂 й渡 嫡朝棻.
	m_pstWarpInfoList	=	new	strWarpInfo[m_siInfoCount];
	ZeroMemory( m_pstWarpInfoList, sizeof(strWarpInfo) * m_siInfoCount );

	// 濠猿蒂 嫡朝棻.
	i = 0;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 等檜攪 塭檣擊 橾欽 蹺轎ж堅

		// 鏽歲滌煎 蹺轎и 濠猿蒂 牖離瞳戲煎 餵橫厥朝棻.
		int		iLineNo, iColNo;
		strWarpInfo	*pWI	= m_pstWarpInfoList + i;

		// ! 輿曖 ! - GetValue蒂 餌辨ж晦 衛濛ц棻賊 п渡 塭檣曖 濠猿朝 翱樓п憮 и廓縑 嫡紫煙 и棻.
		ocp.GetValue( (SI32&)pWI->siCode,					iLineNo, iColNo );	// 廓��
		ocp.GetValue( pWI->szDest,	MAX_WARP_DEST_LENGTH,	iLineNo, iColNo );	// 撲貲旋
		ocp.GetValue( pWI->siOnlineTextNo,					iLineNo, iColNo );	// 螞塭檣 臢蝶お縑憮曖 廓��
		ocp.GetValue( (SI32&)pWI->cType,					iLineNo, iColNo );	// 措陛 顫殮
		ocp.GetValue( (SI32&)pWI->uiX,						iLineNo, iColNo );	// 謝ル X
		ocp.GetValue( (SI32&)pWI->uiY,						iLineNo, iColNo );	// 謝ル Y
		ocp.GetValue( (SI32&)pWI->uiMapIndex,				iLineNo, iColNo );	// 裘 廓��

		// actdoll (2004/06/08 14:23) : 檜薯朝 螞塭檣 臢蝶お 廓�ㄦ� 鐘お煤ж晦 陽僥縑 szDest曖 薑爾蒂 棻衛 夥脯遽棻.
		strncpy( pWI->szDest, pOnlineText->Get( pWI->siOnlineTextNo ), MAX_WARP_DEST_LENGTH );

		// �挫弇蝏� - 虜擒 雖旎 籀葬и 匙檜 葆雖虞 鏽歲檜歷棻賊 薑鼻檜棻.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineWarpParser Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}

		// �挫� 餌髦 - 檜匙檜 雖旎 葆雖虞 塭檣簞蒂 籀葬ж堅 氈棻賊 粽溯檜觼
		if( ocp.IsEndOfLine() )		break;

		// 剩橫鬲戲賊 蘋遴お 螢葬堅
		i++;
	}

	ocp.Release();
	fclose( fp );

	return TRUE;
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : 錶Щ 跡瞳雖煎 錶Щ 囀萄蒂 橢橫魚棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : 錶Щ 跡瞳雖煎 錶Щ 薑爾蒂 橢橫魚棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : 錶Щ 囀萄煎 錶Щ 薑爾蒂 橢橫魚棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Desc : п薯
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
VOID	cltOnlineWarpParser::Free()
{
	if( m_pstWarpInfoList != NULL )
	{
		delete [] m_pstWarpInfoList;
		m_pstWarpInfoList	=	NULL;
	}

	m_siInfoCount	=	0;
}