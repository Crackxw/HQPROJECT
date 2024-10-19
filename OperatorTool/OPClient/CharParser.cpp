#include "CharParser.h"
#include "OnlineItem.h"

BOOL CharParser::Init( CHAR *pCharFile )
{
	FILE		*fp		=	NULL;
	CHAR		*pToken	=	NULL;
	CHAR		szBuffer[1024];
	

	fp = fopen(pCharFile, "rt");
	if( fp == NULL )	return FALSE;


	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );


	CharHead	TempCharHead;
	

//#define ON_GETID
#ifdef	ON_GETID
	FILE	*pFile;
	pFile =  fopen( "케릭터 아이디.txt", "w" );
#endif

	while( fgets( szBuffer, 1024, fp ) )
	{
		ZeroMemory( &TempCharHead, sizeof( TempCharHead ) );
		
		// ID
		pToken						=	strtok( szBuffer, "\n\t\r" );

		if( pToken == NULL ) break;

		if(strcmp(pToken, "U101") == 0 )
		{
			int a=0;
		}

		TempCharHead.uiID			=	GetID( pToken );
#ifdef	ON_GETID
		fprintf( pFile, "%s : ", pToken);
#endif

		// 케릭터 속성을 받는다.
		TempCharHead.siAttri		=	GetAttri( pToken );

		// 몬스터 수를 알아 본다.
		if( TempCharHead.siAttri == ON_ATTRI_MONSTER )	
			siMonsterNum++;

		// 케릭터 이름
		pToken							=	strtok( NULL, "\n\t\r" );
#ifdef	ON_GETID
		fprintf( pFile, "%s : %d\n", pToken, TempCharHead.uiID );
#endif
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

		TempCharHead.Parameta.SetBonus(0);
		TempCharHead.Parameta.SetLife(TempCharHead.Parameta.GetMaxLifeByIP());
		TempCharHead.Parameta.SetMana(TempCharHead.Parameta.GetMaxManaByIP());

		m_CharHeader.push_back( TempCharHead );
	}	

	sort( m_CharHeader.begin(), m_CharHeader.end() );
	fclose( fp );

#ifdef	ON_GETID
		fclose( pFile );
#endif

	return TRUE;
}