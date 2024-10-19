//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 스트링 파서 베이스
//	File Name		: OnlineCommonParser.cpp
//	Birth Date		: 2004. 06. 07.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		데이터가 첨부된 리스트 파일을 파싱할 때 쓸 수 있는 클래스입니다.
//		파싱 기준이 공통으로 되어있을 경우 손쉽게(?) 데이터를 뽑아낼 수 있습니다.
//		일단 이 파서의 기준은 자료의 라인은 엔터, 컬럼은 탭으로 구분됩니다.
//		공백란의 엔터, 탭만있는 줄, 주석(;) 등은 무시되며, 항상 데이터 라인이 시작되기 전에는 이 자료의 컬럼이
//		몇개나 되는지에 대한 컬럼 공지 명령어(#)가 반드시 존재해야 합니다.
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
// Desc: 생성자
//-----------------------------------------------------------------------------
OnlineCommonParser::OnlineCommonParser()
{
	Release();
}

//-----------------------------------------------------------------------------
// Name: ~OnlineCommonParser()
// Code: actdoll (2004-06-07)
// Desc: 소멸자
//-----------------------------------------------------------------------------
OnlineCommonParser::~OnlineCommonParser()
{
	Release();
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2004-06-07)
// Desc: 초기화 구문
//	파싱을 개시할 파일을 물려준다. 
//	HQ로 압축된 놈들의 경우 외부에서 파일을 로딩하여 그 포인터를 물려줘야 하므로 일부러 FILE의 포인터를 받게끔 했다.
//
//		pFile			- 파싱할 텍스트 파일의 FILE 포인터(외부에서 연결)
//		iErrLineNo		- return이 FALSE시 에러가 발생한 라인의 줄번호(받기)
//		bolumnCountLack	- return이 FALSE시 해당 줄의 컬럼이 모자라서 발생한 에러라면 true, 아니라면 false(받기)
//		return			- 성공적으로 로딩되면 파싱된 데이터 라인의 갯수, 문제가 있다면 0 이하의 값
//			return이 -1이라면 pFile이 NULL이거나 이미 파싱중인 파일이 존재한다. 
//			return이 0일 경우라면
//				: 데이터 라인이 하나도 없다면 iRetErrLineNo는 0
//				: 라인에 문제가 생겼다면 iRetErrLineNo과 bRetColumnLack이 채워져서 리턴
//				: iRetErrLineNo가 -1이라면 명령어(#)라인을 찾지 못했다는 뜻
//-----------------------------------------------------------------------------
int		OnlineCommonParser::Initialize( FILE *pFile, int &iRetErrLineNo, BOOL &bRetColumnLack )
{
	iRetErrLineNo		= 0;
	bRetColumnLack		= false;
	if( !pFile || m_pFile )				return -1;	// 들어온 파일이 없거나 파일 포인터가 이미 존재한다면 에러

	// 일단 파일을 가장 앞단으로 돌리고
	fseek( pFile, 0, SEEK_SET );

	// 값들을 정리한다.
	m_iMaxAllLineCount	= 0;
	m_iMaxDataLineCount	= 0;
	m_iMaxColumnCount	= 0;

	// 파일 끝까지 스타팅 정보를 모은다.
	char	pszLineBuff[DF_VALUE_OCP_BUFFER_SIZE];
	char	pszTempBuff[DF_VALUE_OCP_BUFFER_SIZE];
	char	*pszToken		= NULL;
	FILE	*pErrFile		= NULL;
	BOOL	bFindData		= false;
	BOOL	bFindOrderToken = false;
	while( !feof( pFile ) )
	{
		if( !fgets( pszLineBuff, DF_VALUE_OCP_BUFFER_SIZE, pFile ) )	break;		// 파일 줄이 하나도 없다면 나간다.

		// 일단 칸 하나 읽었으니 전체 카운트 올린다.
		m_iMaxAllLineCount++;
		
		// 일단 주석 라인부터 배제
		pszToken	= strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_COMMENT );
		if( pszToken != pszLineBuff )				continue;	// 이 경우는 맨 앞줄부터 주석이 달린 경우다.

		// 엔터 키가 있다면 배제
		pszToken	= strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_ENTER );
		if( pszToken != pszLineBuff )				continue;	// 이 경우는 맨 앞줄부터 리턴이 달린 경우다.

		// 주석 라인 말고 명령어 라인이 왔나 체크
		if( !bFindOrderToken )
		{
			// 명령어 라인은 무조건 맨 앞에 체크된다.
			pszToken	= strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_COMMAND );
			if( pszLineBuff != pszToken )		// 만약에 명령어 라인을 발견했다면
			{
				// 명령어 라인을 체크한다.
				pszToken	= strtok( pszToken, DF_FLAG_OCP_TOKEN_COLUMN );
				while( pszToken )
				{
					m_iMaxColumnCount++;
					pszToken= strtok( NULL, DF_FLAG_OCP_TOKEN_COLUMN );
				}
				bFindOrderToken = true;
				continue;
			}
			else	continue;					// 명령어 라인을 발견하지 못했다면 찾을때까지 계속 회전
		}

		// 일단 여기까지 통과했다면 라인 복사
		strcpy( pszTempBuff, pszLineBuff );

		// 혹시 탭들만 있는 라인인가?
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COLUMN );
		if( !pszToken )			continue;

		// 데이터 라인 증가다.
		m_iMaxDataLineCount++;

		// 주석 라인과 명령어 라인을 지나갔다면 실제 데이터가 맞게 들어가 있는지 체크한다.
		int		iCurrentColumnNo = 0;
		BOOL	bLack = false, bOver = false;

		// 일단 첫번째 칸을 조사하고
		pszToken = strtok( pszLineBuff, DF_FLAG_OCP_TOKEN_COLUMN );
		while( pszToken && iCurrentColumnNo != m_iMaxColumnCount )
		{
			iCurrentColumnNo++;
			pszToken = strtok( NULL, DF_FLAG_OCP_TOKEN_COLUMN );
		}

		// 만약 갯수가 넘어버린 경우
		if( iCurrentColumnNo == m_iMaxColumnCount )
		{
			if( pszToken )
			{
#ifdef	_DEBUG
				// 에러 출력 파일이 없다면 일단 생성하라
				if( !pErrFile )
				{
					pErrFile = fopen( m_pszErrFileName, "wt" );
					if( pErrFile )	fprintf( pErrFile, "=== ErrorLine Start ===\n" );
				}

				// 이게 열려있다면 에러 출력
				if( pErrFile )
				{
					fprintf( pErrFile, "Line %d\tColumn is Larger!\tDefault:%d\tCurrent:%d\n", m_iMaxAllLineCount, m_iMaxColumnCount, iCurrentColumnNo );
				}
				// 여기를 통과했는데 안열렸다는 경우는 파일 생성에 문제가 있을 때이므로 보통때처럼 하나씩 리턴
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
		// 만약 갯수가 모자른 경우
		if( !pszToken )
		{
			if( m_iMaxColumnCount > iCurrentColumnNo )	
			{
#ifdef	_DEBUG
				// 에러 출력 파일이 없다면 일단 생성하라
				if( !pErrFile )
				{
					pErrFile = fopen( m_pszErrFileName, "wt" );
					if( pErrFile )	fprintf( pErrFile, "=== ErrorLine Start ===\n" );
				}

				// 이게 열려있다면 에러 출력
				if( pErrFile )
				{
					fprintf( pErrFile, "Line %d\tColumn is Lower!\tDefault:%d\tCurrent:%d\n", m_iMaxAllLineCount, m_iMaxColumnCount, iCurrentColumnNo );
				}
				// 여기를 통과했는데 안열렸다는 경우는 파일 생성에 문제가 있을 때이므로 보통때처럼 하나씩 리턴
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
	
	// 명령어를 못찾았다면 에러
	if( !bFindOrderToken )
	{
		Release();
		iRetErrLineNo	 = -1;
		return 0;
	}

#ifdef	_DEBUG
	// 에러가 발견된 상황이라면
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

	// 일단 데이터가 존재한다면
	if( m_iMaxDataLineCount > 0 )
	{
		m_pFile				= pFile;
		fseek( m_pFile, 0, SEEK_SET );
		m_iCurrentLineNo	= 0;
		m_iCurrentColumnNo	= 0;
	}

	// 데이터 라인 카운트가 있다면 성공 없다면 실패
	return m_iMaxDataLineCount;
}

//-----------------------------------------------------------------------------
// Name: Release()
// Code: actdoll (2004-06-07)
// Desc: 해제 구문
//	파싱이 작업된 데이터를 모두 해제한다.
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
// Desc: 스트링으로 받는다.
//	실제 자료가 있는 라인을 계속 찾는다.
//		return	- 현재 확보한 데이터라인의 줄번호를 리턴. 에러 발생의 경우나 데이터 라인을 더이상 못찾았다면 0
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
		// 파일 줄이 하나도 없다면 나간다.
		if( !fgets( pszTempBuff, DF_VALUE_OCP_BUFFER_SIZE, m_pFile ) )	break;

		// 현재 탐색중인 줄 카운트 올리고
		m_iCurrentLineNo++;

		// 일단 주석 라인부터 배제
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COMMENT );
		if( pszToken != pszTempBuff )				continue;	// 이 경우는 맨 앞줄부터 주석이 달린 상황이다.

		// 엔터 키가 있다면 배제
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_ENTER );
		if( pszToken != pszTempBuff )				continue;	// 이 경우는 맨 앞줄부터 리턴이 달린 상황이다.

		// 명령어 라인 배제
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COMMAND );
		if( pszToken != pszTempBuff )				continue;	// 이 경우는 명령어 라인을 발견한 상황이다.

		// 일단 여기까지 통과했다면 라인 복사
		strcpy( m_pszLineString, pszTempBuff );

		// 혹시 탭들만 있는 라인인가?
		pszToken	= strtok( pszTempBuff, DF_FLAG_OCP_TOKEN_COLUMN );
		if( !pszToken )
		{
			ZeroMemory( m_pszLineString, sizeof(m_pszLineString) );
			continue;
		}

		// 파싱 했다면 찾았다
		iRet	= m_iCurrentLineNo;
		m_iCurrentColumnNo = 0;
		break;
	}

	return iRet;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: 스트링으로 받는다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(스트링)
//		pszRecvBuff		- 데이터를 받아올 버퍼의 값
//		iRecvBuffSize	- 데이터를 받을 버퍼의 크기
//		iLineNo			- 자료를 받고 있는 라인 넘버
//		iColumnNo		- 자료를 받고 있는 컬럼 넘버
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

	// 데이터를 찾았다면 값을 받는다.
	strncpy( pszRecvBuff, pszToken, iRecvBuffSize );
	iLineNo		= m_iCurrentLineNo;
	iColumnNo	= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: 스트링으로 받는다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(수치)
//		chRecvBuff		- 데이터를 받을 char 버퍼
//		iLineNo			- 자료를 받고 있는 라인 넘버
//		iColumnNo		- 자료를 받고 있는 컬럼 넘버
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

	// 데이터를 찾았다면 값을 받는다.
	chRecvBuff		= (char)atoi( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: 스트링으로 받는다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(수치)
//		shRecvBuff		- 데이터를 받을 short 버퍼
//		iLineNo			- 자료를 받고 있는 라인 넘버
//		iColumnNo		- 자료를 받고 있는 컬럼 넘버
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

	// 데이터를 찾았다면 값을 받는다.
	shRecvBuff		= (short)atoi( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: 스트링으로 받는다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(수치)
//		iRecvBuffSize	- 데이터를 받을 int 버퍼
//		iLineNo			- 자료를 받고 있는 라인 넘버
//		iColumnNo		- 자료를 받고 있는 컬럼 넘버
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

	// 데이터를 찾았다면 값을 받는다.
	iRecvBuff		= atoi( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: 스트링으로 받는다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(수치)
//		i64RecvBuffSize	- 데이터를 받을 int64 버퍼
//		iLineNo			- 자료를 받고 있는 라인 넘버
//		iColumnNo		- 자료를 받고 있는 컬럼 넘버
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

	// 데이터를 찾았다면 값을 받는다.
	i64RecvBuff		= _atoi64( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetValue()
// Code: actdoll (2004-06-07)
// Desc: 스트링으로 받는다.
//	해당 파싱 라인에 대하여 순차적으로 정보를 얻는다.(소수점)
//		fRecvBuff		- 데이터를 받을 double형 버퍼
//		iLineNo			- 자료를 받고 있는 라인 넘버
//		iColumnNo		- 자료를 받고 있는 컬럼 넘버
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

	// 데이터를 찾았다면 값을 받는다.
	fRecvBuff		= atof( pszToken );
	iLineNo			= m_iCurrentLineNo;
	iColumnNo		= m_iCurrentColumnNo;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: PrintError()
// Code: actdoll (2004-06-07)
// Desc: 에러 출력
//	파서에서 문제가 발생한 경우 콘솔이나 윈도 상태에 따라 에러 출력을 다르게 한다.
//		pszFormat		- printf형태의 출력 타입
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

