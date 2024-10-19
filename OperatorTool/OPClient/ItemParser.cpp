
#include <windows.h>
#include <stdio.h>
#include "ItemParser.h"

BOOL ItemParser::LoadItem( char *pItemFile, vector<CItemHeader> *v )
{
	FILE	*fp;
	CHAR	szBuffer[2048];
	CHAR	*pToken	=	NULL;
	SI16	i;

	fp = fopen(pItemFile, "rt");

	if( fp == NULL )		return FALSE;	

	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );	
	fgets( szBuffer, 2048, fp );

	char seps[]   = "\t\n\r";
	CItemHeader	Item;

	while( fgets( szBuffer, 2048, fp ) )
	{
		ZeroMemory( &Item, sizeof( CItemHeader ) );

		pToken								=	strtok( szBuffer, seps );							// ������ �ڵ�
		if( pToken == NULL )	break;

		Item.m_uiID			=	atoi( pToken );
		Item.m_Attribute	=	0;

		pToken								=	strtok( NULL, seps );								// ������ ���̵�
//		Item.m_uiID			=	GetID( pToken );									// ���̵� ���� �޴´�.


		pToken								=	strtok( NULL, seps );								// ������ �̸�
		pToken								=	strtok( NULL, seps );								// Text
		Item.m_siItemName	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// �׷�
		Item.m_siGroupCode	=	GetGroup( pToken , &Item.m_siSpecial);


		pToken								=	strtok( NULL, seps );								// �׷� �ڵ�
		Item.m_siDrawID		=	atoi( pToken );										// ?????


		pToken								=	strtok( NULL, seps );								// ����
		pToken								=	strtok( NULL, seps );								// ���� �ڵ�
		Item.m_siModule		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ����
		pToken								=	strtok( NULL, seps );								// ���� �ڵ�
		Item.m_siItemInfo	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ������ ����
		Item.m_uiValue		=	_atoi64( pToken );


		pToken								=	strtok( NULL, seps );								// ������ ����
		Item.m_uiWeight		=	atoi( pToken );

		pToken								=	strtok( NULL, seps );								// �ִ� ������ ��
		pToken								=	strtok( NULL, seps );								// ū ������ �̹��� ���� �̸�
		strcpy( Item.m_szSpriteFile, pToken );


		pToken								=	strtok( NULL, seps );								// ū ������ ��Ʈ ��ȣ
		Item.m_siSpriteFont	=	atoi( pToken );										


		pToken								=	strtok( NULL, seps );								// ���� ������ ���� �̸�
		strcpy( Item.m_szSm_SpriteFile, pToken );


		pToken								=	strtok( NULL, seps );								// ���� ������ ��Ʈ
		Item.m_siSm_SpriteFont	=	atoi( pToken );


		pToken									=	strtok( NULL, seps );							// ������ �Ӽ�(����)
		if( atoi( pToken ) != 0 )
			Item.m_Attribute	|=	ON_ITEM_ATTACH_BODY_POSSIBLE;					// ���� ����


		pToken								=	strtok( NULL, seps );								// ������ �Ӽ�(��� �����Ѱ�)
		if( atoi( pToken ) != 0 )
			Item.m_Attribute	|=	ON_ITEM_USE_POSSIBLE;


		pToken								=	strtok( NULL, seps );								// ��
		Item.m_siStrength	=	atoi( pToken );	


		pToken								=	strtok( NULL, seps );								// ����
		Item.m_siDex		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ����
		Item.m_siVitality	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ����
		Item.m_siInt		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Hp �̵�
		Item.m_siHpGain		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Hp Max
		Item.m_siHpMax		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Mp �̵�
		Item.m_siMpGain		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Mp Max
		Item.m_siMpMax		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// �ǰ�
		Item.m_siHealth		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// �ǰ� �ִ�ġ
		Item.m_siHealthMax	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ��
		Item.m_siSpecialParam		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ���ݷ�
		Item.m_siAttack		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ����( AC )
		Item.m_siDefense	=	atoi( pToken );


		pToken									=	strtok( NULL, seps );							// Ÿ������
		Item.m_siDamageRegist	=	atoi( pToken );


		pToken									=	strtok( NULL, seps );							// Ÿ������
		Item.m_siMagicRegist	=	atoi( pToken );


		pToken									=	strtok( NULL, seps );							// �������� ���� �Ǵ� ����
		Item.m_siNation			=	atoi( pToken );

		//==============================================================================================================
		//													������ ���� �Ǽ�
		//==============================================================================================================

		Item.pMakeItem					=	new MakeItem;		

		for( i=0 ; i<ON_ITEM_MAX_ADDMAKE; i++ )
		{
			pToken											=	strtok( NULL, seps );
			pToken											=	strtok( NULL, seps );
			Item.pMakeItem->m_uiItemID[i]	=	atoi( pToken );		

			pToken											=	strtok( NULL, seps );
			Item.pMakeItem->m_siItemNum[i]	=	atoi( pToken );
		}

		 
		// ����
		for( i=0 ; i<ON_ITEM_MAX_TOOL ; i++ )
		{
			pToken											=	strtok( NULL, seps );	
			pToken											=	strtok( NULL, seps );	
			Item.pMakeItem->m_uiToolID[i]	=	atoi( pToken );
		}


		pToken												=	strtok( NULL, seps );				
		Item.pMakeItem->m_siLabor			=	atoi( pToken );


		pToken												=	strtok( NULL, seps );
		Item.pMakeItem->m_siProduction		=	atoi( pToken );


		pToken												=	strtok( NULL, seps );
		Item.pMakeItem->m_siMaxProduction	=	atoi( pToken );


		pToken = strtok( NULL, seps );
		pToken = strtok( NULL, seps );
		Item.m_siReqStr = atoi( pToken );

		pToken = strtok( NULL, seps );
		Item.m_siReqDex = atoi( pToken );

		pToken = strtok( NULL, seps );
		Item.m_siReqLv	= atoi( pToken );

		pToken = strtok( NULL, seps );
		Item.m_uiCostChange = atoi( pToken );

		pToken = strtok( NULL, seps );
		Item.m_siSound	= atoi( pToken );

		pToken = strtok( NULL, seps );
		Item.m_uiTradeGrade	= atoi( pToken );

		pToken = strtok( NULL, seps );
		Item.m_siMedic	= atoi( pToken );		

		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );
	}

	fclose( fp );

	AddContainer( v );
	
	return TRUE;
}


//------------------------------------------------------------
//�Լ��� : LoadWeapon
//��  �� : ���� �������� �ε���
//------------------------------------------------------------
BOOL ItemParser::LoadWeapon( char *pWeaponFile )
{
	FILE	*fp;
	CHAR	szBuffer[2048];
	CHAR	*pToken	=	NULL;
	SI16	i;

	fp	=	fopen(pWeaponFile, "rt");



	if( fp == NULL )		return FALSE;

	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );
	fgets( szBuffer, 2048, fp );

	char seps[]   = "\t\n\r";
	CItemHeader	Item;
	
	while( fgets( szBuffer, 2048, fp ) )
	{
		ZeroMemory( &Item, sizeof( CItemHeader ) );

		pToken								=	strtok( szBuffer, seps );								// ������ �ڵ�
		if( pToken == NULL )	break;

		Item.m_uiID			=	atoi( pToken );
		Item.pWeaponHeader	=	new WeaponHeader;										// ���� ����ü ����
		Item.m_Attribute	=	ON_ITEM_SWORD;


		pToken								=	strtok( NULL, seps );									// ������ ���̵�
//		Item.m_uiID			=	GetID( pToken );										// ������ �Ƶ� ���� �޴´�.


		pToken								=	strtok( NULL, seps );									// ������ �̸�
		pToken								=	strtok( NULL, seps );									// ������ �̸� �ڵ�
		Item.m_siItemName	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );									// �׷�
		Item.m_siGroupCode	=	GetGroup( Item.pWeaponHeader, pToken );


		pToken								=	strtok( NULL, seps );									// �׷� �ڵ�
		Item.m_Attribute	|=	GetNation( pToken );									// ���밡���� ���� �����Ѵ�.
		

	
		pToken								=	strtok( NULL, seps );									// ����
		pToken								=	strtok( NULL, seps );									// ���� �ڵ�
		Item.m_siModule		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );									// ������ ����
		pToken								=	strtok( NULL, seps );									// ������ ���� �ڵ�
		Item.m_siItemInfo	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );									// ����
		Item.m_uiValue		=	_atoi64( pToken );


		pToken								=	strtok( NULL, seps );									// ����
		Item.m_uiWeight		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );
		pToken								=	strtok( NULL, seps );									// ū �׸� �̹���
		strcpy( Item.m_szSpriteFile, pToken );


		pToken								=	strtok( NULL, seps );									// ū �׸� ��Ʈ
		Item.m_siSpriteFont	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );									// ū ���� �̹���
		strcpy( Item.m_szSm_SpriteFile, pToken );


		pToken								=	strtok( NULL, seps );									// ū ���� ��Ʈ
		Item.m_siSm_SpriteFont	=	atoi( pToken );



		pToken								=	strtok( NULL, seps );									// ���� �����Ѱ�??		

		if( atoi( pToken ) != 0 )
			Item.m_Attribute	|=	ON_ITEM_ATTACH_BODY_POSSIBLE;						// ���� ����		


		pToken									=	strtok( NULL, seps );								// ��� ���� �Ѱ�?
		if( atoi( pToken ) != 0 )
			Item.m_Attribute	|=		ON_ITEM_USE_POSSIBLE;							// ��� ����

/*
		pToken									=	strtok( NULL, seps );								// ���׷��̵� ���� �Ѱ�?
		if( atoi( pToken ) != 0 )
			Item.m_Attribute	|=		ON_ITEM_UPGRADE_POSSIBLE;						// ���׷��̵� ����
*/

		//==============================================================================================================
		//												���� �ļ� ( �ٸ� �κ�  )
		//==============================================================================================================
		pToken												=	strtok( NULL, seps );					// �ּ� �����
		Item.pWeaponHeader->siMinDam		=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// �ִ� �����
		Item.pWeaponHeader->siMaxDam		=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// �׸��� ���̵�
		Item.m_siDrawID						=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// ���� ����
		Item.pWeaponHeader->siAttackRange	=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// ���� Ÿ��
		if( strcmp( pToken, "Arrow" ) == 0 )			Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_ARROW;
		else if( strcmp( pToken, "Bullet" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_BULLET;
		else if( strcmp( pToken, "Dart" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_DART;
		else if( strcmp( pToken, "sword" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SWORD;
		else if( strcmp( pToken, "spear" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SEPEAR;
		else if( strcmp( pToken, "axe" ) == 0 )			Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_AXE;
		else if( strcmp( pToken, "fan" ) == 0 )			Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_FAN;
		else if( strcmp( pToken, "craw") == 0 )			Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_CRAW;
		else if( strcmp( pToken, "buddist") == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_BUDDIST;		
		else if( strcmp( pToken, "Scythe") == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SCYTHE;
		else if( strcmp( pToken, "SpinStone") == 0 )	Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SPINSTONE;
		else if( strcmp( pToken, "DoubleStick") == 0)	Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_DOUBLESTICK;
		else if( strcmp( pToken, "beads") == 0)			Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_BEADS;
		else if( strcmp( pToken, "empty") == 0)			Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_EMPTY;
		else if( strcmp( pToken, "firegun") == 0)		Item.pWeaponHeader->siWeaponType	=   ON_ITEM_MISSLE_TYPE_FIREGUN;
		else if( strcmp( pToken, "dsword" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_DSWORD;
		else if( strcmp( pToken, "stick" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_STICK;
		else if( strcmp( pToken, "pearl" ) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_PEARL;
		else if( atoi( pToken ) == 1 )					Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_FREE;		
		else											Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_NONE;


		pToken												=	strtok( NULL, seps );					// ���� ������ ��
		Item.m_siReqStr						=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// ���� ������ ��ø
		Item.m_siReqDex						=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// ���� ������ Lv
		Item.m_siReqLv						=	atoi( pToken );


		pToken												=	strtok( NULL, seps );					// ����
		Item.m_siSound						=	atoi( pToken );


		//==============================================================================================================
		//													���� �ļ� ��
		//==============================================================================================================

		pToken								=	strtok( NULL, seps );								// ��
		Item.m_siStrength	=	atoi( pToken );	


		pToken								=	strtok( NULL, seps );								// ����
		Item.m_siDex		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ����
		Item.m_siVitality	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// ����
		Item.m_siInt		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Hp �̵�
		Item.m_siHpGain		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Hp Max
		Item.m_siHpMax		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Mp �̵�
		Item.m_siMpGain		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// Mp Max
		Item.m_siMpMax		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// �ǰ�
		Item.m_siHealth		=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// �ǰ� �ִ�ġ
		Item.m_siHealthMax	=	atoi( pToken );


		pToken										=	strtok( NULL, seps );						// ��޹��� +���
		Item.pWeaponHeader->uiGrade	=	atoi( pToken );


		pToken								=	strtok( NULL, seps );								// �������� ���� �Ǵ� ����
		Item.m_siNation		=	atoi( pToken );

		//==============================================================================================================
		//													������ ���� �Ǽ�
		//==============================================================================================================

		Item.pMakeItem		=	new MakeItem;

		for( i=0 ; i<ON_ITEM_MAX_ADDMAKE; i++ )
		{
			pToken											=	strtok( NULL, seps );
			pToken											=	strtok( NULL, seps );
			Item.pMakeItem->m_uiItemID[i]	=	atoi( pToken );		

			pToken											=	strtok( NULL, seps );
			Item.pMakeItem->m_siItemNum[i]	=	atoi( pToken );
		}

		 
		// ����
		for( i=0 ; i<ON_ITEM_MAX_TOOL ; i++ )
		{
			pToken											=	strtok( NULL, seps );	
			pToken											=	strtok( NULL, seps );	
			Item.pMakeItem->m_uiToolID[i]	=	atoi( pToken );
		}

		pToken												=	strtok( NULL, seps );				
		Item.pMakeItem->m_siLabor			=	atoi( pToken );


		pToken												=	strtok( NULL, seps );				
		Item.pMakeItem->m_siProduction		=	atoi( pToken );


		pToken												=	strtok( NULL, seps );
		Item.pMakeItem->m_siMaxProduction	=	atoi( pToken );


		pToken = strtok( NULL, seps );
		pToken = strtok( NULL, seps );
		Item.m_uiCostChange = atoi( pToken );


		if( Item.pMakeItem->m_uiItemID[0] == 0 )
			Item.m_Attribute |= ON_ITEM_NO_MATERIAL;

		if( Item.pMakeItem->m_uiToolID[0] == 0 )
			Item.m_Attribute |= ON_ITEM_NO_TOOL;

		pToken = strtok( NULL, seps );
		if( strcmp( pToken, "KillShot" )	   == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_KILLSHOT;
		else if( strcmp( pToken, "Seismic")    == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_SEISMIC;
		else if( strcmp( pToken, "Hidden")     == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_HIDDEN;
		else if( strcmp( pToken, "VitalSteel") == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_VITALSTEEL;
		else if( strcmp( pToken, "MoonAxe")    == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_MOONAXE;
		else if( strcmp( pToken, "FireBlow")   == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_FIREBLOW;
		else if( strcmp( pToken, "AstraFire")  == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ASTRAFIRE;
		else if( strcmp( pToken, "FireArrow")  == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_FIREARROW;
		else if( strcmp( pToken, "MagicArrow")  == 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_MAGICARROW;
		else if( strcmp( pToken, "WedgeAttack")== 0)		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_WEDGEATTACK;
		else if( strcmp( pToken, "Bolt")== 0)				Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_BOLT;
		else if( strcmp( pToken, "AstralFire")== 0)			Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ASTRALFIRE;
		else if( strcmp( pToken, "Tornado")== 0)			Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_TORNADO;
		else if( strcmp( pToken, "Hold")== 0)			    Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_HOLD;
		else if( strcmp( pToken, "RunningFire")==0)			Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_RUNNINGFIRE;
		else if( strcmp( pToken, "Energy")==0)				Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ENERGYPA;

//		else if( strcmp( pToken, "longspear")==0)			Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_LONG_SPEAR;
//		else if( strcmp( pToken, "doublesword")==0)			Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_DOUBLE_SWORD;
//		else if( strcmp( pToken, "icebarrier")==0)			Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ICE_BARRIER;

		else												Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_NONE;


		pToken = strtok( NULL, seps );
		pToken = strtok( NULL, seps );
		Item.pWeaponHeader->siMagicDesc = atoi(pToken);

		pToken = strtok( NULL, seps );
		Item.pWeaponHeader->siMagicAttack = atoi(pToken);

		pToken = strtok( NULL, seps );
		Item.m_uiTradeGrade = atoi(pToken);
		
		_Weapon.push_back( Item );
		m_IDBuffer.push_back( Item.m_uiID );
	}


	fclose( fp );

	AddContainer( &_Weapon );

	return TRUE;
}

void ItemParser::Load(OnlineText *Text, char *pFile)
{
	pOnlineText = Text;

	FILE *fp = fopen(pFile, "rb");
	
	CItemHeader Item;

	while(!feof(fp))
	{
		fread(&Item, sizeof(CItemHeader), 1, fp);		

		Item.pMakeItem		= new MakeItem;
		fread(Item.pMakeItem, sizeof(MakeItem), 1, fp);
		

		Item.pWeaponHeader	= new WeaponHeader;		
		ZeroMemory(Item.pWeaponHeader, sizeof(WeaponHeader));


		if(Item.m_Attribute & ON_ITEM_SWORD)
			fread(Item.pWeaponHeader, sizeof(WeaponHeader), 1, fp);
		else
		{
			delete Item.pWeaponHeader;
			Item.pWeaponHeader = NULL;
		}

		_Items.push_back(Item);
	}

	AddContainer(&_Items);
}

void ItemParser::Save()
{
	FILE *fp = fopen("Text\\Item.dat", "wb");

	CItemHeader *pHeader;
	for(int i=0; i<GetCount(); ++i)
	{
		pHeader = GetItemHeader(i);

		fwrite(pHeader, sizeof(CItemHeader), 1, fp);
		fwrite(pHeader->pMakeItem, sizeof(MakeItem), 1, fp);

		if(pHeader->pWeaponHeader)
			fwrite(pHeader->pWeaponHeader, sizeof(WeaponHeader), 1, fp);		
	}

	fclose(fp);
}

void ItemParser::DeleteItem(int nIndex)
{
	map<UI32, CItemHeader*, less<UI32> >::iterator del=_Item.begin();

	for(int i=0; i<nIndex; ++i)
		++del;
	
	_Item.erase(del);	
}
