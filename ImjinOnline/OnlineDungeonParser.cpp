/*****************************************************************************************************************
	���ϸ�	:	OnlineDungeonParser.cpp
	�ۼ���  :	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.04.09
*****************************************************************************************************************/

#include <GSL.h>
#include <Main.h>

#include "OnlineDungeonParser.h"
#include "OnlineText.h"
#include "bindJxFile.h"


OnlineDungeonParser::OnlineDungeonParser()
{
	pDungeonHeader = NULL;
	siDungeonNum = 0;
}

OnlineDungeonParser::~OnlineDungeonParser()
{
	Free();
}

/**************************************************************************************************
	�Լ��� : ReadText
	��  �� : MapPosition.txt�о Dungeon ����ü�� �־� �ش�.
**************************************************************************************************/
BOOL OnlineDungeonParser::Init( OnlineText *pOnlineText )
{
	FILE	*fp		= NULL;
	CHAR	*pToken = NULL;
	CHAR	szBuffer[1024];
	SI32	i;
	pOnlineText = pOnlineText;

	fp = BindJXFile::GetInstance()->GetFile( "Online\\Map\\MapPosition.txt" );
	if( fp == NULL )	return FALSE;

	// 4���� �׳� �Ѿ� ����.
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	
	while(fgets( szBuffer, 1024, fp ))
		siDungeonNum++;


	fseek(fp, SEEK_CUR, 0);
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );

	pToken	=	strtok( szBuffer, "\t\n\r" );
	pToken	=	strtok( NULL, "\t\n\r" );

	// �� Dungeon ��
//	siDungeonNum	=	atoi( pToken );		




	
	fgets( szBuffer, 1024, fp );
	

	pDungeonHeader	=	new DungeonHeader[ siDungeonNum ];
	ZeroMemory( pDungeonHeader, sizeof(DungeonHeader) * siDungeonNum);

	for( i=0 ; i<siDungeonNum ; i++)
	{
		fgets( szBuffer, 1024, fp );

		pToken								=	strtok( szBuffer, "\t\n\r" );
		pDungeonHeader[ i ].siCode			=   atoi( pToken );

		pToken								=   strtok( NULL, "\t\n\r" );
		pDungeonHeader[ i ].siSelect		=   atoi( pToken );

		pToken								=	strtok( NULL, "\t\n\r" );	
		pDungeonHeader[ i ].siMapXPos		=   atoi( pToken );

		pToken								=	strtok( NULL, "\t\n\r" );	
		pDungeonHeader[ i ].siMapYPos		=   atoi( pToken );

		pToken								=	strtok( NULL, "\t\n\r" );	
		pDungeonHeader[ i ].siMapIndex		=   atoi( pToken );

		pToken								=	strtok( NULL, "\t\n\r" );	
		pToken								=	strtok( NULL, "\t\n\r" );	
		strcpy( pDungeonHeader[ i ].szName, pOnlineText->Get(atoi( pToken )));

		pToken								=	strtok( NULL, "\t\n\r" );	
		pDungeonHeader[ i ].siBattlePoint	=   atoi( pToken );
	}



	fclose(fp);

	return TRUE;
}

/**************************************************************************************************
	�Լ��� : Free
	��  �� : ����� ������.
**************************************************************************************************/
VOID OnlineDungeonParser::Free()
{
	if( pDungeonHeader )
	{
		delete [] pDungeonHeader;
		pDungeonHeader	=	NULL;
	}
}

/**************************************************************************************************
	�Լ��� : GetDungeonHeader
	��  �� : ����� ��´�.
**************************************************************************************************/
DungeonHeader *OnlineDungeonParser::GetDungeonHeader( SI16 siIndex )
{
	return &pDungeonHeader[ siIndex ];
}

/**************************************************************************************************
	�Լ��� : GetDungeonNum
	��  �� : �� Dungeon ���� ���Ѵ�.
**************************************************************************************************/
UI16 OnlineDungeonParser::GetDungeonNum()
{
	return siDungeonNum;
}

