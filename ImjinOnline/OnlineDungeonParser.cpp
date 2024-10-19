/*****************************************************************************************************************
	파일명	:	OnlineDungeonParser.cpp
	작성자  :	이 준 석
	담당자	:	이 준 석
	작성일	:	2001.04.09
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
	함수명 : ReadText
	내  용 : MapPosition.txt읽어서 Dungeon 구조체에 넣어 준다.
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

	// 4줄은 그냥 넘어 간다.
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

	// 총 Dungeon 수
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
	함수명 : Free
	내  용 : 헤더를 날린다.
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
	함수명 : GetDungeonHeader
	내  용 : 헤더를 얻는다.
**************************************************************************************************/
DungeonHeader *OnlineDungeonParser::GetDungeonHeader( SI16 siIndex )
{
	return &pDungeonHeader[ siIndex ];
}

/**************************************************************************************************
	함수명 : GetDungeonNum
	내  용 : 총 Dungeon 수를 구한다.
**************************************************************************************************/
UI16 OnlineDungeonParser::GetDungeonNum()
{
	return siDungeonNum;
}

