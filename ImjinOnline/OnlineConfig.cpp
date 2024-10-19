
/********************************************************************************************
	파일명	:	OnlineConfig.cpp
	작성자	:	정 진 욱
	담담자	:	정 진 욱
	작성일	:	2001.05.23
	수정일	:	2002.04.04
	내  용  :	Config 파일 작성
********************************************************************************************/

#include <GSL.h>
#include "Online.h"
#include "Defines.h"
#include "OnlineConfig.h"

OnlineConfig::OnlineConfig()
{
	ZeroMemory( szID_Name, ON_ACCOUNT_LENGTH + 1 );
	uiMiniMapSize	=	0;
	uiServerType	=	1;
	bOpen			=	FALSE;
}

OnlineConfig::~OnlineConfig()
{
}
//------------------------------------------------------------------------------------------------
//	함수명	:	OpenConfig
//  내  용  :	config 파일을 열어 준다.
//------------------------------------------------------------------------------------------------
BOOL OnlineConfig::OpenConfig()
{
	if(bOpen)		return FALSE;
	else			bOpen = TRUE;

	FILE	*fp	  =  NULL;
	ZeroMemory(szID_Name, ON_ACCOUNT_LENGTH + 1);
	uiMiniMapSize	=	0;
	uiServerType	=	1;
	
	SetFileAttributes("Online\\hq.cfg", FILE_ATTRIBUTE_ARCHIVE);

	fp = fopen( "Online\\hq.cfg", "rb" );


	if( fp == NULL ) return FALSE;

	ZeroMemory( szID_Name, sizeof( szID_Name ) );
	
	// 만약 config파일이 있다면...
	if( fp != NULL )
	{
		// 아디를 먼저 읽는다.
		fread((void *) szID_Name, sizeof(szID_Name) * sizeof(char), 1, fp );	
		
		// 미니맵 사이즈를 읽어 준다.
		fread( &uiMiniMapSize, sizeof( UI08 ), 1, fp );
		
		// 서버타입을 읽어 준다.
		fread( &uiServerType, sizeof( UI08 ), 1, fp );
		
		fclose( fp );
		
		// 암호를 푼다.
		DeCode( (UI08*)szID_Name, ON_ACCOUNT_LENGTH );
	}		

	return TRUE;
}
//------------------------------------------------------------------------------------------------
//	함수명	:	SetConfig
//  내  용  :	config파일을 생성
//------------------------------------------------------------------------------------------------
void OnlineConfig::SetConfig( char *pID )
{
	if(bOpen)
		strcpy( szID_Name, pID );
}
//------------------------------------------------------------------------------------------------
//	함수명	:	DeCode
//  내  용  :	헤독 & 암호화
//------------------------------------------------------------------------------------------------
void OnlineConfig::DeCode( UI08 *pData, UINT nSize )
{
	for( UINT i=0 ; i<nSize ; i++)
	{
		if( pData[i] & 0x10 )		pData[i] &=~ 0x10;
		else						pData[i] |=  0x10;
	}
}

//------------------------------------------------------------------------------------------------
//	함수명	:	SaveConfig
//  내  용  :	파일에 config 파일을 저장
//------------------------------------------------------------------------------------------------
void OnlineConfig::SaveConfig()
{
	if(!bOpen)		return;
	else			bOpen = FALSE;

	FILE   *fp = NULL;
	
	fp	       = fopen( "Online\\hq.cfg", "wb" );
	
	if( fp != NULL )
	{
		// 암호화
		DeCode( (UI08*)szID_Name, ON_ACCOUNT_LENGTH );		
		
		// 아이디
		fwrite( szID_Name, ON_ACCOUNT_LENGTH, 1, fp );
		// 미니맵 사이즈
		fwrite( &uiMiniMapSize, sizeof(UI08), 1, fp );
		// 서버타입
		fwrite( &uiServerType, sizeof(UI08), 1, fp );		

		fclose( fp );
	}
}
