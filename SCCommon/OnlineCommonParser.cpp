//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ��Ʈ�� �ļ� ���̽�
//	File Name		: OnlineCommonParser.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�����Ͱ� ÷�ε� ����Ʈ ������ �Ľ��� �� �� �� �ִ� Ŭ�����Դϴ�.
//		�Ľ� ������ �������� �Ǿ����� ��� �ս���(?) �����͸� �̾Ƴ� �� �ֽ��ϴ�.
//		�ϴ� �� �ļ��� ������ �ڷ��� ������ ����, �÷��� ������ ���е˴ϴ�.
//		������� ����, �Ǹ��ִ� ��, �ּ�(;) ���� ���õǸ�, �׻� ������ ������ ���۵Ǳ� ������ �� �ڷ��� �÷���
//		��� �Ǵ����� ���� �÷� ���� ��ɾ�(#)�� �ݵ�� �����ؾ� �մϴ�.
//
//===================================================================================================

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "OnlineCommonParser.h"

const	char *DF_FLAG_OCP_TOKEN_COMMENT	= ";";
const	char *DF_FLAG_OCP_TOKEN_ENTER	= "\n\r";
const	char *DF_FLAG_OCP_TOKEN_COMMAND	= "#";
const	char *DF_FLAG_OCP_TOKEN_COLUMN	= "\t\n\r";

//-----------------------------------------------------------------------------
// Name: OnlineCommonParser()
// Code: actdoll (2004-06-07)
// Desc: ������
//-----------------------------------------------------------------------------
OnlineCommonParser::OnlineCommonParser()
{
	Release();
}

//-----------------------------------------------------------------------------
// Name: ~OnlineCommonParser()
// Code: actdoll (2004-06-07)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
OnlineCommonParser::~OnlineCommonParser()
{
	Release();
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2004-06-07)
// Desc: �ʱ�ȭ ����
//	�Ľ��� ������ ������ �����ش�. 
//	HQ�� ����� ����� ��� �ܺο��� ������ �ε��Ͽ� �� �����͸� ������� �ϹǷ� �Ϻη� FILE�� �����͸� �ްԲ� �ߴ�.
//
//		pFile			- �Ľ��� �ؽ�Ʈ ������ FILE ������(�ܺο��� ����)
//		iErrLineNo		- return�� FALSE�� ������ �߻��� ������ �ٹ�ȣ(�ޱ�)
//		bolumnCountLack	- return�� FALSE�� �ش� ���� �÷��� ���ڶ� �߻��� ������� true, �ƴ϶�� false(�ޱ�)
//		return			- ���������� �ε��Ǹ� �Ľ̵� ������ ������ ����, ������ �ִٸ� 0 ������ ��
//			return�� -1�̶�� pFile�� NULL�̰ų� �̹� �Ľ����� ������ �����Ѵ�. 
//			return�� 0�� �����
//				: ������ ������ �ϳ��� ���ٸ� iRetErrLineNo�� 0
//				: ���ο� ������ ����ٸ� iRetErrLineNo�� bRetColumnLack�� ä������ ����
//				: iRetErrLineNo�� -1�̶�� ��ɾ�(#)������ ã�� ���ߴٴ� ��
//-----------------------------------------------------------------------------
int		OnlineCommonParser::Initialize( FILE *pFile, int &iRetErrLineNo, BOOL &bRetColumnLack )
{
	iRetErrLineNo		= 0;
	bRetColumnLack		= false;
	if( !pFile || m_pFile )				return -1;	// ���� ������ ���ų� ���� �����Ͱ� �̹� �����Ѵٸ� ����

	// �ϴ� ������ ���� �մ����� ������
	fseek( pFile, 0, SEEK_SET );

	// ������ �����Ѵ�.
	m_iMaxAllLineCount	= 0;
	m_iMaxDataLineCount	= 0;
	m_iMaxColumnCount	= 0;

	// ���� ������ ��Ÿ�� ������ ������.
	char	pszLineBuff[DF_VALUE_OCP_BUFFER_SIZE];
	char	pszTempBuff[DF_VALUE_OCP_BUFFER_SIZE];
	char	*pszToken		= NULL;
	FILE	*pErrFile		= NULL;
	BOOL	bFindData		= false;
	BOOL	bFindOrderToken = false;
	while( !feof( pFile ) )
	{
		if( !fgets( pszLineBuff, DF_VALUE_OCP_BUFFER_SIZE, pFile ) )	break;		// ���� ���� �ϳ��� ���ٸ� ������.

		// �ϴ� ĭ �ϳ� �о����� ��ü ī��Ʈ �ø���.
		m_iMaxAllLineCount++;
		
		// �ϴ� �ּ� ���κ��� ����
		pszToken	= strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_COMMENT );
		if( pszToken != pszLineBuff )				continue;	// �� ���� �� ���ٺ��� �ּ��� �޸� ����.

		// ���� Ű�� �ִٸ� ����
		pszToken	= strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_ENTER );
		if( pszToken != pszLineBuff )				continue;	// �� ���� �� ���ٺ��� ������ �޸� ����.

		// �ּ� ���� ���� ��ɾ� ������ �Գ� üũ
		if( !bFindOrderToken )
		{
			// ��ɾ� ������ ������ �� �տ� üũ�ȴ�.
			pszToken	= strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_COMMAND );
			if( pszLineBuff != pszToken )		// ���࿡ ��ɾ� ������ �߰��ߴٸ�
			{
				// ��ɾ� ������ üũ�Ѵ�.
				pszToken	= strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
				while( pszToken )
				{
					m_iMaxColumnCount++;
					pszToken= strtok( NULL, DF_FLAG_OCP_TOKEN_COLUMN );
				}
				bFindOrderToken = true;
				continue;
			}
			else	continue;					// ��ɾ� ������ �߰����� ���ߴٸ� ã�������� ��� ȸ��
		}

		// �ϴ� ������� ����ߴٸ� ���� ����
		strcpy( pszTempBuff, pszLineBuff );

		// Ȥ�� �ǵ鸸 �ִ� �����ΰ�?
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COLUMN );
		if( !pszToken )			continue;

		// ������ ���� ������.
		m_iMaxDataLineCount++;

		// �ּ� ���ΰ� ��ɾ� ������ �������ٸ� ���� �����Ͱ� �°� �� �ִ��� üũ�Ѵ�.
		int		iCurrentColumnNo = 0;
		BOOL	bLack = false, bOver = false;

		// �ϴ� ù��° ĭ�� �����ϰ�
		pszToken = strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_COLUMN );
		while( pszToken && iCurrentColumnNo != m_iMaxColumnCount )
		{
			iCurrentColumnNo++;
			pszToken = strtok( NULL, DF_FLAG_OCP_TOKEN_COLUMN );
		}

		// ���� ������ �Ѿ���� ���
		if( iCurrentColumnNo == m_iMaxColumnCount )
		{
			if( pszToken )
			{
#ifdef	_DEBUG
				// ���� ��� ������ ���ٸ� �ϴ� �����϶�
				if( !pErrFile )
				{
					pErrFile = fopen( m_pszErrFileName, "wt" );
					if( pErrFile )	fprintf( pErrFile, "=== ErrorLine Start ===\n" );
				}

				// �̰� �����ִٸ� ���� ���
				if( pErrFile )
				{
					fprintf( pErrFile, "Line %d\tColumn is Larger!\tDefault:%d\tCurrent:%d\n", m_iMaxAllLineCount, m_iMaxColumnCount, iCurrentColumnNo );
				}
				// ���⸦ ����ߴµ� �ȿ��ȴٴ� ���� ���� ������ ������ ���� ���̹Ƿ� ���붧ó�� �ϳ��� ����
				else
				{
#endif//_DEBUG
					iRetErrLineNo	= m_iMaxAllLineCount;
					bRetColumnLack	= false;
					Release();
					return 0;
#ifdef	_DEBUG
				}
#endif//_DEBUG
			}
		}
		// ���� ������ ���ڸ� ���
		if( !pszToken )
		{
			if( m_iMaxColumnCount > iCurrentColumnNo )	
			{
#ifdef	_DEBUG
				// ���� ��� ������ ���ٸ� �ϴ� �����϶�
				if( !pErrFile )
				{
					pErrFile = fopen( m_pszErrFileName, "wt" );
					if( pErrFile )	fprintf( pErrFile, "=== ErrorLine Start ===\n" );
				}

				// �̰� �����ִٸ� ���� ���
				if( pErrFile )
				{
					fprintf( pErrFile, "Line %d\tColumn is Lower!\tDefault:%d\tCurrent:%d\n", m_iMaxAllLineCount, m_iMaxColumnCount, iCurrentColumnNo );
				}
				// ���⸦ ����ߴµ� �ȿ��ȴٴ� ���� ���� ������ ������ ���� ���̹Ƿ� ���붧ó�� �ϳ��� ����
				else
				{
#endif//_DEBUG
					iRetErrLineNo	= m_iMaxAllLineCount;
					bRetColumnLack	= true;
					Release();
					return 0;
#ifdef	_DEBUG
				}
#endif//_DEBUG
			}
		}
	}
	
	// ��ɾ ��ã�Ҵٸ� ����
	if( !bFindOrderToken )
	{
		Release();
		iRetErrLineNo	 = -1;
		return 0;
	}

#ifdef	_DEBUG
	// ������ �߰ߵ� ��Ȳ�̶��
	if( pErrFile )
	{
		fprintf( pErrFile, "=== ErrorLine End ===\n" );
		fclose( pErrFile );
		pErrFile = NULL;
		PrintError( "Fail to load parsing text. Error list will be update at [%s] file.", m_pszErrFileName );
		Release();
		iRetErrLineNo	 = -1;
		return 0;
	}
#endif//_DEBUG

	// �ϴ� �����Ͱ� �����Ѵٸ�
	if( m_iMaxDataLineCount > 0 )
	{
		m_pFile				= pFile;
		fseek( m_pFile, 0, SEEK_SET );
		m_iCurrentLineNo	= 0;
		m_iCurrentColumnNo	= 0;
	}

	// ������ ���� ī��Ʈ�� �ִٸ� ���� ���ٸ� ����
	return m_iMaxDataLineCount;
}

//-----------------------------------------------------------------------------
// Name: Release()
// Code: actdoll (2004-06-07)
// Desc: ���� ����
//	�Ľ��� �۾��� �����͸� ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineCommonParser::Release()
{
	m_pFile				= NULL;
	
	m_iMaxAllLineCount	= 0;
	m_iMaxDataLineCount	= 0;
	m_iMaxColumnCount	= 0;

	m_iCurrentLineNo	= 0;
	m_iCurrentColumnNo	= 0;
	
	ZeroMemory( m_pszLineString, sizeof(m_pszLineString) );	
	ZeroMemory( m_pszErrFileName, sizeof(m_pszErrFileName) );	
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	���� �ڷᰡ �ִ� ������ ��� ã�´�.
//		return	- ���� Ȯ���� �����Ͷ����� �ٹ�ȣ�� ����. ���� �߻��� ��쳪 ������ ������ ���̻� ��ã�Ҵٸ� 0
//-----------------------------------------------------------------------------
int		OnlineCommonParser::ParseDataLine()
{
	if( !m_pFile )		return 0;

	char	*pszToken;
	char	pszTempBuff[DF_VALUE_OCP_BUFFER_SIZE];
	int		iRet	= 0;
	ZeroMemory( m_pszLineString, sizeof(m_pszLineString) );

	while( !feof( m_pFile ) )
	{
		// ���� ���� �ϳ��� ���ٸ� ������.
		if( !fgets( pszTempBuff, DF_VALUE_OCP_BUFFER_SIZE, m_pFile ) )	break;

		// ���� Ž������ �� ī��Ʈ �ø���
		m_iCurrentLineNo++;

		// �ϴ� �ּ� ���κ��� ����
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COMMENT );
		if( pszToken != pszTempBuff )				continue;	// �� ���� �� ���ٺ��� �ּ��� �޸� ��Ȳ�̴�.

		// ���� Ű�� �ִٸ� ����
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_ENTER );
		if( pszToken != pszTempBuff )				continue;	// �� ���� �� ���ٺ��� ������ �޸� ��Ȳ�̴�.

		// ��ɾ� ���� ����
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COMMAND );
		if( pszToken != pszTempBuff )				continue;	// �� ���� ��ɾ� ������ �߰��� ��Ȳ�̴�.

		// �ϴ� ������� ����ߴٸ� ���� ����
		strcpy( m_pszLineString, pszTempBuff );

		// Ȥ�� �ǵ鸸 �ִ� �����ΰ�?
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COLUMN );
		if( !pszToken )
		{
			ZeroMemory( m_pszLineString, sizeof(m_pszLineString) );
			continue;
		}

		// �Ľ� �ߴٸ� ã�Ҵ�
		iRet	= m_iCurrentLineNo;
		m_iCurrentColumnNo = 0;
		break;
	}

	return iRet;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	�ش� �Ľ� ���ο� ���Ͽ� ���������� ������ ��´�.(��Ʈ��)
//		pszRecvBuff		- �����͸� �޾ƿ� ������ ��
//		iRecvBuffSize	- �����͸� ���� ������ ũ��
//		iLineNo			- �ڷḦ �ް� �ִ� ���� �ѹ�
//		iColumnNo		- �ڷḦ �ް� �ִ� �÷� �ѹ�
//-----------------------------------------------------------------------------
BOOL	OnlineCommonParser::GetValue( char *pszRecvBuff, int iRecvBuffSize, int &iLineNo, int &iColumnNo )
{
	iLineNo = iColumnNo = 0;
	if( !pszRecvBuff || iRecvBuffSize <= 0 )	return FALSE;
	if( !m_pFile )								return FALSE;
	if( strlen( m_pszLineString ) <= 0 )		return FALSE;

	ZeroMemory( pszRecvBuff, iRecvBuffSize );

	char	*pszToken = NULL;
	if( !m_iCurrentColumnNo )	pszToken = m_pszLineString;
	pszToken = strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
	if( !pszToken )								return FALSE;

	m_iCurrentColumnNo++;

	// �����͸� ã�Ҵٸ� ���� �޴´�.
	strncpy( pszRecvBuff, pszToken, iRecvBuffSize );
	iLineNo		= m_iCurrentLineNo;
	iColumnNo	= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	�ش� �Ľ� ���ο� ���Ͽ� ���������� ������ ��´�.(��ġ)
//		chRecvBuff		- �����͸� ���� char ����
//		iLineNo			- �ڷḦ �ް� �ִ� ���� �ѹ�
//		iColumnNo		- �ڷḦ �ް� �ִ� �÷� �ѹ�
//-----------------------------------------------------------------------------
BOOL	OnlineCommonParser::GetValue( char &chRecvBuff, int &iLineNo, int &iColumnNo )
{
	iLineNo = iColumnNo = 0;
	if( !m_pFile )								return FALSE;
	if( strlen( m_pszLineString ) <= 0 )		return FALSE;

	char	*pszToken = NULL;
	if( !m_iCurrentColumnNo )	pszToken = m_pszLineString;
	pszToken = strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
	if( !pszToken )								return FALSE;

	m_iCurrentColumnNo++;

	// �����͸� ã�Ҵٸ� ���� �޴´�.
	chRecvBuff		= (char)atoi( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	�ش� �Ľ� ���ο� ���Ͽ� ���������� ������ ��´�.(��ġ)
//		shRecvBuff		- �����͸� ���� short ����
//		iLineNo			- �ڷḦ �ް� �ִ� ���� �ѹ�
//		iColumnNo		- �ڷḦ �ް� �ִ� �÷� �ѹ�
//-----------------------------------------------------------------------------
BOOL	OnlineCommonParser::GetValue( short &shRecvBuff, int &iLineNo, int &iColumnNo )
{
	iLineNo = iColumnNo = 0;
	if( !m_pFile )								return FALSE;
	if( strlen( m_pszLineString ) <= 0 )		return FALSE;

	char	*pszToken = NULL;
	if( !m_iCurrentColumnNo )	pszToken = m_pszLineString;
	pszToken = strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
	if( !pszToken )								return FALSE;

	m_iCurrentColumnNo++;

	// �����͸� ã�Ҵٸ� ���� �޴´�.
	shRecvBuff		= (short)atoi( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	�ش� �Ľ� ���ο� ���Ͽ� ���������� ������ ��´�.(��ġ)
//		iRecvBuffSize	- �����͸� ���� int ����
//		iLineNo			- �ڷḦ �ް� �ִ� ���� �ѹ�
//		iColumnNo		- �ڷḦ �ް� �ִ� �÷� �ѹ�
//-----------------------------------------------------------------------------
BOOL	OnlineCommonParser::GetValue( int &iRecvBuff, int &iLineNo, int &iColumnNo )
{
	iLineNo = iColumnNo = 0;
	if( !m_pFile )								return FALSE;
	if( strlen( m_pszLineString ) <= 0 )		return FALSE;

	char	*pszToken = NULL;
	if( !m_iCurrentColumnNo )	pszToken = m_pszLineString;
	pszToken = strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
	if( !pszToken )								return FALSE;

	m_iCurrentColumnNo++;

	// �����͸� ã�Ҵٸ� ���� �޴´�.
	iRecvBuff		= atoi( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	�ش� �Ľ� ���ο� ���Ͽ� ���������� ������ ��´�.(��ġ)
//		i64RecvBuffSize	- �����͸� ���� int64 ����
//		iLineNo			- �ڷḦ �ް� �ִ� ���� �ѹ�
//		iColumnNo		- �ڷḦ �ް� �ִ� �÷� �ѹ�
//-----------------------------------------------------------------------------
BOOL	OnlineCommonParser::GetValue( __int64 &i64RecvBuff, int &iLineNo, int &iColumnNo )
{
	iLineNo = iColumnNo = 0;
	if( !m_pFile )								return FALSE;
	if( strlen( m_pszLineString ) <= 0 )		return FALSE;

	char	*pszToken = NULL;
	if( !m_iCurrentColumnNo )	pszToken = m_pszLineString;
	pszToken = strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
	if( !pszToken )								return FALSE;

	m_iCurrentColumnNo++;

	// �����͸� ã�Ҵٸ� ���� �޴´�.
	i64RecvBuff		= _atoi64( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: ��Ʈ������ �޴´�.
//	�ش� �Ľ� ���ο� ���Ͽ� ���������� ������ ��´�.(�Ҽ���)
//		fRecvBuff		- �����͸� ���� double�� ����
//		iLineNo			- �ڷḦ �ް� �ִ� ���� �ѹ�
//		iColumnNo		- �ڷḦ �ް� �ִ� �÷� �ѹ�
//-----------------------------------------------------------------------------
BOOL	OnlineCommonParser::GetValue( double &fRecvBuff, int &iLineNo, int &iColumnNo )
{
	iLineNo = iColumnNo = 0;
	if( !m_pFile )								return FALSE;
	if( strlen( m_pszLineString ) <= 0 )		return FALSE;

	char	*pszToken = NULL;
	if( !m_iCurrentColumnNo )	pszToken = m_pszLineString;
	pszToken = strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
	if( !pszToken )								return FALSE;

	m_iCurrentColumnNo++;

	// �����͸� ã�Ҵٸ� ���� �޴´�.
	fRecvBuff		= atof( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: PrintError()
// Code: actdoll (2004-06-07)
// Desc: ���� ���
//	�ļ����� ������ �߻��� ��� �ܼ��̳� ���� ���¿� ���� ���� ����� �ٸ��� �Ѵ�.
//		pszFormat		- printf������ ��� Ÿ��
//-----------------------------------------------------------------------------
void	OnlineCommonParser::PrintError( char *pszFormat, ... )
{
	char		pszBuffer[1024];
	va_list		marker;

	va_start( marker, pszFormat );
	vsprintf( pszBuffer, pszFormat, marker );
	va_end( marker );

#ifdef	_CONSOLE
	printf( pszBuffer );
	printf( "\nPress any key to continue...\n" );
	getchar();
#else
	MessageBox( NULL, pszBuffer, "OCP ERROR", MB_OK | MB_ICONERROR );
#endif
}

