/********************************************************************************************
	파일명	:	OnlineChar-Parser.cpp
	작성자	:	정 진 욱
	담당자  :   이 상 원
	작성일	:	2001.06.30
	수정일	:	2001.12.24
	내  용	:	케릭터 정보를 텍스트에서 읽어 온다.( Character List.txt )
********************************************************************************************/


#include <GSL.h>
#include	<stdlib.h>
#include	<search.h>


#include	"Directives.h"
#include	"OnlineChar-Parser.h"
#include	"OnLineItem.h"
#include	"bindJXFile.h"

//#define ON_GETID //(주석처리해야함) 캐릭터 숫자 아이디 텍스트 출력

/********************************************************************************************
	함수명 : Init
	내  용 : 케릭터 구조체에 데이타를 삽입
********************************************************************************************/
BOOL	OnlineCharParser::Init( CHAR *pCharFile )
{
	FILE		*fp		=	NULL;
	CHAR		*pToken	=	NULL;
	CHAR		szBuffer[1024];


	siTotalCharNum = 0;
	

	fp = BindJXFile::GetInstance()->GetFile( pCharFile );
	if( fp == NULL )	return FALSE;


	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );


	CharHead	TempCharHead;
	

//#define ON_GETID
/*#ifdef	ON_GETID
	FILE	*pFile;
	pFile =  fopen( "케릭터 아이디.txt", "w" );
#endif*/

	while( fgets( szBuffer, 1024, fp ) )
	{
		ZeroMemory( &TempCharHead, sizeof( TempCharHead ) );
		
		// ID
		pToken						=	strtok( szBuffer, "\n\t\r" );

		if( pToken == NULL ) break;

		TempCharHead.uiID			=	GetID( pToken );
/*#ifdef	ON_GETID
		fprintf( pFile, "%s : ", pToken);
#endif*/

		// 케릭터 속성을 받는다.
		TempCharHead.siAttri		=	GetAttri( pToken );

		// 몬스터 수를 알아 본다.
		if( TempCharHead.siAttri == ON_ATTRI_MONSTER )	
			siMonsterNum++;

		// 케릭터 이름
		pToken							=	strtok( NULL, "\n\t\r" );
/*#ifdef	ON_GETID
		fprintf( pFile, "%s : %d\n", pToken, TempCharHead.uiID );
#endif*/
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.uiCharName	=	atoi( pToken );


		// 경험치 다음
		pToken								=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetCurExp(atoi( pToken ));

		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.uiMyExp		=	atoi( pToken );

		// 국가
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.siNation		=	atoi( pToken );

		// 힘
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetStr(atoi( pToken ));

		// 민첩
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetDex(atoi( pToken ));

		// 생명		
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetVit(atoi( pToken ));

		// 지능
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetInt(atoi( pToken ));

		
		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetAC( atoi( pToken ));


		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.siLv			=	atoi( pToken );


		pToken							=	strtok( NULL, "\n\t\r" );
		TempCharHead.siCon		=	atoi( pToken );

		
		pToken									 =	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.DamageResist =	atoi( pToken );

		
		pToken									 =	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.MagicResist  =	atoi( pToken );

		
		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.EquipMinDamage	=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.EquipMaxDamage	=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.siMoveSpeed				=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.Parameta.SetLife(atoi( pToken ));



		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.siManaMax				=	atoi( pToken );
		TempCharHead.Parameta.SetMana(TempCharHead.siManaMax);

		
		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.siCost					=	atoi( pToken );		


		pToken										=	strtok( NULL, "\n\t\r" );
		TempCharHead.siNeedLv					=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );

		if( strcmp( pToken, "Arrow" ) == 0 )			TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_ARROW;
		else if( strcmp( pToken, "Bullet" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_BULLET;
		else if( strcmp( pToken, "Dart" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_DART;
		else if( strcmp( pToken, "sword" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SWORD;
		else if( strcmp( pToken, "spear" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SEPEAR;
		else if( strcmp( pToken, "axe" ) == 0 )			TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_AXE;
		else if( strcmp( pToken, "fan" ) == 0 )			TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_FAN;
		else if( strcmp( pToken, "craw" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_CRAW;
		else if( strcmp( pToken, "buddist" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_BUDDIST;
		else if( strcmp( pToken, "beads") == 0)			TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_BEADS;
		else if( strcmp( pToken, "DoubleStick") == 0)	TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_DOUBLESTICK;
		else if( strcmp( pToken, "Scythe") == 0 )       TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SCYTHE;
		else if( strcmp( pToken, "SpinStone") == 0)		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SPINSTONE;
		else if( strcmp( pToken, "firegun") == 0)		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_FIREGUN;
		else if( strcmp( pToken, "empty") == 0)			TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_EMPTY;
		else if( strcmp( pToken, "dsword" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_DSWORD;
		else if( strcmp( pToken, "stick" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_STICK;
		else if( strcmp( pToken, "pearl" ) == 0 )		TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_PEARL;
		
		else if( atoi( pToken ) == 1 )					TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_FREE;
		else											TempCharHead.uiWeaponType	=	ON_ITEM_MISSLE_TYPE_NONE;

		pToken					  =	strtok( NULL, "\n\t\r" );
		TempCharHead.siNeedCredit =	atoi( pToken );


		for( SI16 i=0 ; i<4; i++ )
		{
			pToken = strtok( NULL, "\n\t\r" );
			TempCharHead.siItemDrop[i] = atoi(pToken);

			pToken = strtok( NULL, "\n\t\r" );
			TempCharHead.siDropPercent[i] = atoi(pToken);
		}

		pToken = strtok( NULL, "\n\t\r" );
		TempCharHead.siDescText = atoi(pToken);

		pToken = strtok( NULL, "\n\t\r" );
		TempCharHead.siGeneral = atoi(pToken);

		pToken = strtok( NULL, "\n\t\r" );
		pToken = strtok( NULL, "\n\t\r" );
		TempCharHead.siMonsterType = atoi(pToken);

		TempCharHead.Parameta.SetBonus(0);
		TempCharHead.Parameta.SetLife(TempCharHead.Parameta.GetMaxLifeByIP());
		TempCharHead.Parameta.SetMana(TempCharHead.Parameta.GetMaxManaByIP());

		siTotalCharNum++;

		m_CharHeader.push_back( TempCharHead );
	}	

	sort( m_CharHeader.begin(), m_CharHeader.end() );
	fclose( fp );

/*#ifdef	ON_GETID
		fclose( pFile );
#endif*/

	return TRUE;
}
/********************************************************************************************
	함수명 : GetID
	내  용 : 텍스트에서 2바이트 ID을 만든다.
********************************************************************************************/
UI16	OnlineCharParser::GetID( CHAR *pToken )
{
	UI16	uiVal;

	uiVal		=	MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}
/********************************************************************************************
	함수명 : GetID
	내  용 : 케릭터가 몬스터 인가 플레이어 인가 알아 온다.
********************************************************************************************/
SI16	OnlineCharParser::GetAttri( char *pToken )
{
	switch( pToken[0] )
	{
	case 'C':	return ON_ATTRI_PLAYER;
	case 'M':	return ON_ATTRI_MONSTER;
	case 'U':	return ON_ATTRI_MERCENARY;
	}

	return -1;
}

CharHead *OnlineCharParser::GetCharHeaderID(UI16 uiID)
{	
	// actdoll (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
	//	캐릭터 헤더 ID를 통해 해당 정보를 얻고자 할 때 비교하게되는 Temp값이 완전히 초기화 되지 않은 상태로 uiID만
	//	적재하여 데이터를 받을 경우 쓰레기 값으로 인하여 STL문 안에서 죽는 것으로 생각된다.
	//	따라서 Temp값을 0으로 초기화시킨 후 데이터를 비교하도록 했다.
	//	다음 버그 리포팅에 이 부분에서 버그가 안나오거나 조치에 따른 특별한 문제가 없을 경우 이 주석을 제거해도 좋다.
	CharHead	Temp;
	ZeroMemory( &Temp, sizeof(Temp) );
	Temp.uiID			= uiID;
	VWlterPair p		= equal_range( m_CharHeader.begin(), m_CharHeader.end(), Temp );
	return ( ( p.first != p.second )	?	p.first		:	NULL );
}
