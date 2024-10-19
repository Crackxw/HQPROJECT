/*******************************************************************************************************
	파일명	:	OnlineMonsterParser.cpp
	작성자	:	정 진 욱
	작성일	:	2001.08.03
	수정일	:	2001.11.19
	내  용  :	몬스터 파서
*******************************************************************************************************/

#include	<windows.h>
#include	<stdio.h>
#include	<stdlib.h>


#include	"Directives.h"
#include	"OnlineMonsterParser.h"

#define MAKE32(a,b,c,d)						(MAKELONG(MAKEWORD(a,b),MAKEWORD(c,d)))

OnlineMonsterParser::OnlineMonsterParser()
{
	pMonsterPatternHeader	=	NULL;
	siTotalPattern			=	0;
}

OnlineMonsterParser::~OnlineMonsterParser()
{
	Free();
} 

/*******************************************************************************************************
	 몬스터 텍스를 읽는다.
*******************************************************************************************************/
BOOL	OnlineMonsterParser::Init( CHAR *pMonsterText )
{
	FILE	*fp;
	CHAR	szBuffer[1024];
	CHAR	*pToken = NULL;
	SI32	i, j;

	fp = fopen( pMonsterText, "r" );

	if( fp == NULL )				return FALSE;

	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );

	pToken					=	strtok( szBuffer, "\n\t\r" );
	pToken					=	strtok( NULL, "\n\t\r" );
	siTotalPattern			=	atoi( pToken );															// 총 패턴수

	fgets( szBuffer, 1024, fp );

	pMonsterPatternHeader	=	new MonsterPatternHeader[ siTotalPattern+1 ];
	ZeroMemory( pMonsterPatternHeader, sizeof( MonsterPatternHeader ) * siTotalPattern+1 );

	for( i=1 ; i<siTotalPattern+1 ; i++ )
	{
		fgets( szBuffer, 1024, fp );
		
		pToken									=	strtok( szBuffer, "\n\t\r" );							// Field Pattern ID

		pToken									=	strtok( NULL, "\n\t\r" );								// Name
		
		pToken									=	strtok( NULL, "\n\t\r" );								// X 좌표
		pMonsterPatternHeader[i].siX			=	atoi( pToken );

		
		pToken									=	strtok( NULL, "\n\t\r" );								// Y 좌표
		pMonsterPatternHeader[i].siY			=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// Y 좌표
		pMonsterPatternHeader[i].siMapIndex		=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// 반경
		pMonsterPatternHeader[i].siRadius		=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// 총 몬스터 종류
		pMonsterPatternHeader[i].siMaxKind		=	atoi( pToken );	
		

		pToken										=	strtok( NULL, "\n\t\r" );
		pMonsterPatternHeader[i].siGroupMinQuantity	=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		pMonsterPatternHeader[i].siGroupMaxQuantity	=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// 최대 몬스터 수
		pMonsterPatternHeader[i].siMaxQuantity	=	atoi( pToken );

		
		pToken									=	strtok( NULL, "\n\t\r" );								// 최소 몬스터 수
		pMonsterPatternHeader[i].siMinQuantity	=	atoi( pToken );


		for( j=0 ; j<_ON_MAX_GROUP ; j++ )																// 총 7개의 그룹이 있다
		{
			pToken									=	strtok( NULL, "\n\t\r" );							// 몬스터 ID
			pMonsterPatternHeader[i].uiMonsterID[j]	=	GetID( pToken );

			pToken									=	strtok( NULL, "\n\t\r" );							// 비율
			pMonsterPatternHeader[i].siRate[j]		=	atoi( pToken );


			pToken									=	strtok( NULL, "\n\t\r" );							// 몬스터 레벨
			pMonsterPatternHeader[i].uiMonsterLv[j]	=	atoi( pToken );			
		}
	}

	fclose( fp );
	return TRUE;
}

/*******************************************************************************************************
	 문자열을 아디로 변환
*******************************************************************************************************/
UI32	OnlineMonsterParser::GetID( CHAR *pStr )
{
	UI16	uiVal;
	
	if( pStr[0] == '0' )	return 0;

	uiVal		=	MAKEWORD(pStr[0], atoi(&pStr[1]));	

	return uiVal;
}

/*******************************************************************************************************
	 총 몬스터 패턴을 리턴 한다.
*******************************************************************************************************/
SI32	OnlineMonsterParser::GetTotalPattern()
{
	return siTotalPattern;
}

/*******************************************************************************************************
	 패턴 해더를 리턴한다.
*******************************************************************************************************/
MonsterPatternHeader	*OnlineMonsterParser::GetPatternHeader( UI32 uiIndex )
{
	return &pMonsterPatternHeader[uiIndex];
}

/*******************************************************************************************************
	 날린다.
*******************************************************************************************************/
VOID	OnlineMonsterParser::Free()
{
	if( pMonsterPatternHeader )
	{
		delete [] pMonsterPatternHeader;
		pMonsterPatternHeader	=	NULL;
	}
}
