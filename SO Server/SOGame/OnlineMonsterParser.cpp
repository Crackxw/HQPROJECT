/*******************************************************************************************************
	���ϸ�	:	OnlineMonsterParser.cpp
	�ۼ���	:	�� �� ��
	�ۼ���	:	2001.08.03
	������	:	2001.11.19
	��  ��  :	���� �ļ�
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
	 ���� �ؽ��� �д´�.
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
	siTotalPattern			=	atoi( pToken );															// �� ���ϼ�

	fgets( szBuffer, 1024, fp );

	pMonsterPatternHeader	=	new MonsterPatternHeader[ siTotalPattern+1 ];
	ZeroMemory( pMonsterPatternHeader, sizeof( MonsterPatternHeader ) * siTotalPattern+1 );

	for( i=1 ; i<siTotalPattern+1 ; i++ )
	{
		fgets( szBuffer, 1024, fp );
		
		pToken									=	strtok( szBuffer, "\n\t\r" );							// Field Pattern ID

		pToken									=	strtok( NULL, "\n\t\r" );								// Name
		
		pToken									=	strtok( NULL, "\n\t\r" );								// X ��ǥ
		pMonsterPatternHeader[i].siX			=	atoi( pToken );

		
		pToken									=	strtok( NULL, "\n\t\r" );								// Y ��ǥ
		pMonsterPatternHeader[i].siY			=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// Y ��ǥ
		pMonsterPatternHeader[i].siMapIndex		=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// �ݰ�
		pMonsterPatternHeader[i].siRadius		=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// �� ���� ����
		pMonsterPatternHeader[i].siMaxKind		=	atoi( pToken );	
		

		pToken										=	strtok( NULL, "\n\t\r" );
		pMonsterPatternHeader[i].siGroupMinQuantity	=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		pMonsterPatternHeader[i].siGroupMaxQuantity	=	atoi( pToken );


		pToken									=	strtok( NULL, "\n\t\r" );								// �ִ� ���� ��
		pMonsterPatternHeader[i].siMaxQuantity	=	atoi( pToken );

		
		pToken									=	strtok( NULL, "\n\t\r" );								// �ּ� ���� ��
		pMonsterPatternHeader[i].siMinQuantity	=	atoi( pToken );


		for( j=0 ; j<_ON_MAX_GROUP ; j++ )																// �� 7���� �׷��� �ִ�
		{
			pToken									=	strtok( NULL, "\n\t\r" );							// ���� ID
			pMonsterPatternHeader[i].uiMonsterID[j]	=	GetID( pToken );

			pToken									=	strtok( NULL, "\n\t\r" );							// ����
			pMonsterPatternHeader[i].siRate[j]		=	atoi( pToken );


			pToken									=	strtok( NULL, "\n\t\r" );							// ���� ����
			pMonsterPatternHeader[i].uiMonsterLv[j]	=	atoi( pToken );			
		}
	}

	fclose( fp );
	return TRUE;
}

/*******************************************************************************************************
	 ���ڿ��� �Ƶ�� ��ȯ
*******************************************************************************************************/
UI32	OnlineMonsterParser::GetID( CHAR *pStr )
{
	UI16	uiVal;
	
	if( pStr[0] == '0' )	return 0;

	uiVal		=	MAKEWORD(pStr[0], atoi(&pStr[1]));	

	return uiVal;
}

/*******************************************************************************************************
	 �� ���� ������ ���� �Ѵ�.
*******************************************************************************************************/
SI32	OnlineMonsterParser::GetTotalPattern()
{
	return siTotalPattern;
}

/*******************************************************************************************************
	 ���� �ش��� �����Ѵ�.
*******************************************************************************************************/
MonsterPatternHeader	*OnlineMonsterParser::GetPatternHeader( UI32 uiIndex )
{
	return &pMonsterPatternHeader[uiIndex];
}

/*******************************************************************************************************
	 ������.
*******************************************************************************************************/
VOID	OnlineMonsterParser::Free()
{
	if( pMonsterPatternHeader )
	{
		delete [] pMonsterPatternHeader;
		pMonsterPatternHeader	=	NULL;
	}
}
