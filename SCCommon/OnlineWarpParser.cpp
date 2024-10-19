#include <stdio.h>
#include <stdlib.h>

#include <GSL.h>
#include "OnlineText.h"
#include "OnlineWarpParser.h"
#include "bindjxfile.h"
#include "OnlineCommonParser.h"

//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : ������
//������������������������������������������������������������������������������������������������������������������������������������������������
cltOnlineWarpParser::cltOnlineWarpParser()
{
	m_siInfoCount		=	0;
	m_pstWarpInfoList	=	NULL;
}


//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : �Ҹ���
//������������������������������������������������������������������������������������������������������������������������������������������������
cltOnlineWarpParser::~cltOnlineWarpParser()
{
	Free();	
}


//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : �ʱ�ȭ 
//������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineWarpParser::Init( CHAR *pszFileName, OnlineText *pOnlineText )
{
	if( !pszFileName || !pOnlineText )										return FALSE;
	FILE	*fp = NULL;
	SI32	i;		
	OnlineCommonParser		ocp;
	
	// �ϴ� �����͸� ���� �� ���ٸ� �����̴�.
	if( (fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL )	return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "OnlineWarpParser Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �������� ������ �޴´�.
	m_siInfoCount = ocp.GetMaxDataLineCount();
	
	// �޸𸮸� �Ҵ� �޴´�.
	m_pstWarpInfoList	=	new	strWarpInfo[m_siInfoCount];
	ZeroMemory( m_pstWarpInfoList, sizeof(strWarpInfo) * m_siInfoCount );

	// �ڷḦ �޴´�.
	i = 0;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ������ �ϴ� �����ϰ�

		// �÷����� ������ �ڷḦ ���������� ����ִ´�.
		int		iLineNo, iColNo;
		strWarpInfo	*pWI	= m_pstWarpInfoList + i;

		// ! ���� ! - GetValue�� ����ϱ� �����ߴٸ� �ش� ������ �ڷ�� �����ؼ� �ѹ��� �޵��� �Ѵ�.
		ocp.GetValue( (SI32&)pWI->siCode,					iLineNo, iColNo );	// ��ȣ
		ocp.GetValue( pWI->szDest,	MAX_WARP_DEST_LENGTH,	iLineNo, iColNo );	// �����
		ocp.GetValue( pWI->siOnlineTextNo,					iLineNo, iColNo );	// �¶��� �ؽ�Ʈ������ ��ȣ
		ocp.GetValue( (SI32&)pWI->cType,					iLineNo, iColNo );	// ���� Ÿ��
		ocp.GetValue( (SI32&)pWI->uiX,						iLineNo, iColNo );	// ��ǥ X
		ocp.GetValue( (SI32&)pWI->uiY,						iLineNo, iColNo );	// ��ǥ Y
		ocp.GetValue( (SI32&)pWI->uiMapIndex,				iLineNo, iColNo );	// �� ��ȣ

		// actdoll (2004/06/08 14:23) : ������ �¶��� �ؽ�Ʈ ��ȣ�� ��Ʈ���ϱ� ������ szDest�� ������ �ٽ� �ٲ��ش�.
		strncpy( pWI->szDest, pOnlineText->Get( pWI->siOnlineTextNo ), MAX_WARP_DEST_LENGTH );

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineWarpParser Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )		break;

		// �Ѿ���� ī��Ʈ �ø���
		i++;
	}

	ocp.Release();
	fclose( fp );

	return TRUE;
}

//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : ���� �������� ���� �ڵ带 ����.
//������������������������������������������������������������������������������������������������������������������������������������������������
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

//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : ���� �������� ���� ������ ����.
//������������������������������������������������������������������������������������������������������������������������������������������������
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

//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : ���� �ڵ�� ���� ������ ����.
//������������������������������������������������������������������������������������������������������������������������������������������������
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


//������������������������������������������������������������������������������������������������������������������������������������������������
// Desc : ����
//������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWarpParser::Free()
{
	if( m_pstWarpInfoList != NULL )
	{
		delete [] m_pstWarpInfoList;
		m_pstWarpInfoList	=	NULL;
	}

	m_siInfoCount	=	0;
}