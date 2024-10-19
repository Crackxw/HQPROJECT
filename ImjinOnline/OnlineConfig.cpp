
/********************************************************************************************
	���ϸ�	:	OnlineConfig.cpp
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.05.23
	������	:	2002.04.04
	��  ��  :	Config ���� �ۼ�
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
//	�Լ���	:	OpenConfig
//  ��  ��  :	config ������ ���� �ش�.
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
	
	// ���� config������ �ִٸ�...
	if( fp != NULL )
	{
		// �Ƶ� ���� �д´�.
		fread((void *) szID_Name, sizeof(szID_Name) * sizeof(char), 1, fp );	
		
		// �̴ϸ� ����� �о� �ش�.
		fread( &uiMiniMapSize, sizeof( UI08 ), 1, fp );
		
		// ����Ÿ���� �о� �ش�.
		fread( &uiServerType, sizeof( UI08 ), 1, fp );
		
		fclose( fp );
		
		// ��ȣ�� Ǭ��.
		DeCode( (UI08*)szID_Name, ON_ACCOUNT_LENGTH );
	}		

	return TRUE;
}
//------------------------------------------------------------------------------------------------
//	�Լ���	:	SetConfig
//  ��  ��  :	config������ ����
//------------------------------------------------------------------------------------------------
void OnlineConfig::SetConfig( char *pID )
{
	if(bOpen)
		strcpy( szID_Name, pID );
}
//------------------------------------------------------------------------------------------------
//	�Լ���	:	DeCode
//  ��  ��  :	�쵶 & ��ȣȭ
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
//	�Լ���	:	SaveConfig
//  ��  ��  :	���Ͽ� config ������ ����
//------------------------------------------------------------------------------------------------
void OnlineConfig::SaveConfig()
{
	if(!bOpen)		return;
	else			bOpen = FALSE;

	FILE   *fp = NULL;
	
	fp	       = fopen( "Online\\hq.cfg", "wb" );
	
	if( fp != NULL )
	{
		// ��ȣȭ
		DeCode( (UI08*)szID_Name, ON_ACCOUNT_LENGTH );		
		
		// ���̵�
		fwrite( szID_Name, ON_ACCOUNT_LENGTH, 1, fp );
		// �̴ϸ� ������
		fwrite( &uiMiniMapSize, sizeof(UI08), 1, fp );
		// ����Ÿ��
		fwrite( &uiServerType, sizeof(UI08), 1, fp );		

		fclose( fp );
	}
}
