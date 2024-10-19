//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	������ �¶��� [�Ż�] - ������ �����ͺ��̽� ���
//	File Name		:	OnlineItem.cpp
//	Birth Date		:	2001. 04. 20.
//	Creator			:	�� �� ��, �� �� ��
//	Editer			:	�� �� ��
//	Copyright		:	Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�����۰� ���õ� ó���� �Ѱ��ϴ� Ŭ�����Դϴ�. ������ Ŭ���̾�Ʈ ���ʿ��� ����մϴ�.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: 
// Code: All (2004-07-07)
// Desc: ��� �� ���� ����
//-----------------------------------------------------------------------------
#include <GSL.h>
#include	<search.h>
#include	<io.h>
#include	"OnLineItem.h"
#include	"bindJXFile.h"
#include	"utility.h"
#include	"OnlineCommonParser.h"		// actdoll (2004/07/07 17:54) : ���� �ļ� �ý���



//-----------------------------------------------------------------------------
// Name: CItem()
// Code: All (2004-07-07)
// Desc: ������
//-----------------------------------------------------------------------------
CItem::CItem()
{
	m_siWeaponNum		=	0;
	m_siArmorNum		=	0;	
	m_siBootsNum		=	0;
	m_siGlovesNum		=	0;
	m_siHelmatNum		=	0;
	m_siBeltNum			=	0;
	m_siRing			=	0;
	m_siDress			=	0;
	m_siGuardian		=	0;
}

//-----------------------------------------------------------------------------
// Name: ~CItem()
// Code: All (2004-07-07)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
CItem::~CItem()
{
	ReMoveAll();	
	m_IDBuffer.clear();
}

//-----------------------------------------------------------------------------
// Name: ReMoveAll()
// Code: All (2004-07-07)
// Desc: �޸𸮿��� ��� �����۸� ������.
//-----------------------------------------------------------------------------
void CItem::ReMoveAll()
{
	if( !_Item.empty() )
	{
		Remove( &_Weapon );
		Remove( &_Items );

		_Item.clear();
	}
}

//-----------------------------------------------------------------------------
// Name: ItemLoad()
// Code: All (2004-07-07)
// Desc: ������ �̹����� TEXT �� �ε���.
//-----------------------------------------------------------------------------
BOOL CItem::ItemLoad(	OnlineText *ponlinetext, char *pWeaponFile, char *pArmorFile, char *pAccessaryFile, 
						char *pAnimal, char *pDress, char *pElement, char *pFood, char *pLiquor, 
						char *pMedicine, char *pTool, char *pTrade, char *pGuardian, char* pImperium)
{
//	if( LoadImperium(pImperium, &_Items) == FALSE)
//		return FALSE;

	if( LoadGuardian(pGuardian, &_Items) == FALSE)
		return FALSE;

	if( LoadWeapon( pWeaponFile ) == FALSE )
		return FALSE;

	if( LoadItem( pArmorFile, &_Items ) == FALSE )
		return FALSE;

	if( LoadItem( pAccessaryFile, &_Items ) == FALSE )
		return FALSE;

	if( LoadItem( pAnimal, &_Items ) == FALSE )	
		return FALSE;

	if( LoadItem( pDress, &_Items ) == FALSE )	
		return FALSE;

	if( LoadItem( pElement, &_Items ) == FALSE )	
		return FALSE;

	if( LoadItem( pFood, &_Items ) == FALSE )	
		return FALSE;

	if( LoadItem( pLiquor, &_Items ) == FALSE )
		return FALSE;

	if( LoadItem( pMedicine, &_Items ) == FALSE )
		return FALSE;

	if( LoadItem( pTool, &_Items ) == FALSE )
		return FALSE;

	if( LoadItem( pTrade, &_Items ) == FALSE )
		return FALSE;

	pOnlineText	=	ponlinetext;
	return TRUE;
}

//------------------------------------------------------------
//�Լ��� : GetItem
//��  �� : ID�� �޾� ID�� ������ �������� ������ ���ش�
//------------------------------------------------------------
CItemHeader* CItem::GetItem( UINT nID )
{
	if( nID <= 0 )	return NULL;

	map<UI32, CItemHeader*, less<UI32> >::iterator	Find = _Item.lower_bound( nID );

	UI32 i = Find->first;

	if( Find->first == nID )
	{
		return Find->second;
	}

	return NULL;
}

//------------------------------------------------------------
//�Լ��� : GetItem
//��  �� : ������ �̸��� �޾� ID�� ������ �������� ������ ���ش�
//------------------------------------------------------------
CItemHeader* CItem::GetItem( CHAR *pszItemName )
{
	CHAR	*pszText;

	for( map<UI32, CItemHeader*, less<UI32> >::iterator i=_Item.begin(); i!=_Item.end(); ++i )
	{
		if((pszText	= pOnlineText->Get(i->second->m_siItemName)) != NULL)
		{
			if(stricmp(pszText, pszItemName) == 0)
				return i->second;
		}		
	}

	return NULL;
}

//------------------------------------------------------------
//�Լ��� : GetItemHeader
//��  �� : ��� �������� ��Ƴ��� pItemsInfo�� ����� ����(index)�� ���� �������� �������ش�
//------------------------------------------------------------
CItemHeader*	CItem::GetItemHeader( UI32 index )
{
	if( index == -1 ) return NULL;
	map<UI32, CItemHeader*, less<UI32> >::iterator i = _Item.begin();
	for( int ix=0; ix<index; ++ix, ++i );
	return i->second;
}

//------------------------------------------------------------
//�Լ��� : SetItemFile
//��  �� : ���ҽ��� �ε� �Ǿ� �ִ� �������� ������ �ε����� ���� ���ش�.
//------------------------------------------------------------
VOID CItem::SetFile(SI32 siFile, CHAR *pFileName, SI16 siXsize, SI16 siYsize, BOOL bSm)
{
	if( bSm == FALSE )
	{
		// ��� ������ ����� �˻��ؼ� ���� ���ϸ��� �ִٸ� ���ڷ� ���� ���ҽ��� �ε��� ��ȣ�� �־� �ش�.
		for( map<UI32, CItemHeader*, less<UI32> >::iterator i=_Item.begin(); i!=_Item.end(); ++i )
		{
			CItemHeader *p = i->second;

			if( ( stricmp( i->second->m_szSpriteFile, pFileName ) ) == 0 )
				i->second->m_siSpriteFile = siFile;
		}
	}
	
	for( map<UI32, CItemHeader*, less<UI32> >::iterator i=_Item.begin(); i!=_Item.end(); ++i )
	{
		if( ( stricmp( i->second->m_szSm_SpriteFile, pFileName ) ) == 0 )
			i->second->m_siSm_SpriteFile = siFile;
	}
}

//------------------------------------------------------------
// robypark 2004/11/9 21:54
//�Լ��� : GetItemHeaderFromImperium
//�Ķ����: SI32 siKindOnlineId: �������� ���Ǵ� �¶��� ĳ���� ID
//��  �� : �������� ���� �Լ��� ���ŵǴ� ĳ����(Online) ID��
//		   �̿��Ͽ� ���� ���������� ����� ã�´�.
//------------------------------------------------------------
CItemHeader* CItem::GetItemHeaderFromImperium( SI32 siKindOnlineID )
{
	if (-1 == siKindOnlineID)
		return NULL;

	for( map<UI32, CItemHeader*, less<UI32> >::iterator i=_Item.begin(); i!=_Item.end(); ++i )
	{
		Imperium* pImperium = i->second->m_pImperium;
		if (NULL != pImperium)
		{
			if (pImperium->m_uiKind == siKindOnlineID)
				return i->second;
		}
	}

	return NULL;
}

//------------------------------------------------------------
// robypark 2004/12/13 13:39
//�Լ��� : GetItemHeaderFromImperiumByIndex
//�Ķ����: SI32 siImperiumIndex: ������ �������� �ε���
//��  �� : �������� ���� �Լ��� ���ŵǴ� ������ �������� �ε���
//			�� �̿��Ͽ� ������ ���� ����� ã�´�.
//------------------------------------------------------------
CItemHeader* CItem::GetItemHeaderFromImperiumByIndex( SI32 siImperiumIndex )
{
	if (-1 == siImperiumIndex)
		return NULL;

	for( map<UI32, CItemHeader*, less<UI32> >::iterator i=_Item.begin(); i!=_Item.end(); ++i )
	{
		Imperium* pImperium = i->second->m_pImperium;
		if (NULL != pImperium)
		{
			if (pImperium->m_siID == siImperiumIndex)
				return i->second;
		}
	}

	return NULL;
}

//------------------------------------------------------------
//�Լ��� : GetSpriteFileName
//��  �� : ������� �������� �̸��� ���� �޴´�.
//------------------------------------------------------------
VOID CItem::GetSpriteFileName( SI32 siIndex, CHAR *pFileName, CHAR *SmallFileName )
{
	map<UI32, CItemHeader*, less<UI32> >::iterator i = _Item.begin();
	for( int ix=0; ix<siIndex; ++ix, ++i );
	strcpy( pFileName, i->second->m_szSpriteFile );
	strcpy( SmallFileName, i->second->m_szSm_SpriteFile );
}



//==============================================================================================================================

//------------------------------------------------------------
//�Լ��� : LoadItem
//��  �� : ���⸦ ������ �������� �ε���
//------------------------------------------------------------
BOOL	CItem::LoadItem( char *pItemFile, vector<CItemHeader> *v )
{
	FILE				*fp;
	SI16				i;
	OnlineCommonParser	ocp;

	// ���� �а�
	fp = BindJXFile::GetInstance()->GetFile( pItemFile );
	if( fp == NULL )		return FALSE;	

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pItemFile );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "LoadItem Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pItemFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item	= {0,};
		Item.pMakeItem		= new MakeItem;
		Item.m_Attribute	= 0;

		ocp.GetValue( (int&)Item.m_uiID,					iLineNo, iColNo );		// ������ �ڵ�
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// ������ ���̵�(������)
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// ������ �̸�(������)
		ocp.GetValue( Item.m_siItemName,					iLineNo, iColNo );		// ������ �̸� �ڵ�
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// ������ ����
		Item.m_siGroupCode	=	GetGroup( pszTemp , &Item.m_siSpecial);
		ocp.GetValue( Item.m_siDrawID,						iLineNo, iColNo );		// �׸��� ID(?)
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// ����(������)
		ocp.GetValue( Item.m_siModule,						iLineNo, iColNo );		// ���� �ڵ�
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// ������ ����(������)
		ocp.GetValue( Item.m_siItemInfo,					iLineNo, iColNo );		// ������ ���� �ڵ�
		ocp.GetValue( (__int64&)Item.m_uiValue,				iLineNo, iColNo );		// ����
		ocp.GetValue( (char&)Item.m_uiWeight,				iLineNo, iColNo );		// ����
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// �ִ�������ɰ���(������)
		ocp.GetValue( Item.m_szSpriteFile,		15,			iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSpriteFont,					iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,			iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSm_SpriteFont,				iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// ���� �����Ѱ� �Ұ��Ѱ�?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_ATTACH_BODY_POSSIBLE;
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// ��� �����Ѱ� �Ұ��Ѱ�?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_USE_POSSIBLE;
		ocp.GetValue( Item.m_siStrength,					iLineNo, iColNo );		// �� ������
		ocp.GetValue( Item.m_siDex,							iLineNo, iColNo );		// ��ø�� ������
		ocp.GetValue( Item.m_siVitality,					iLineNo, iColNo );		// ���� ������
		ocp.GetValue( Item.m_siInt,							iLineNo, iColNo );		// ���� ������
		ocp.GetValue( Item.m_siHpGain,						iLineNo, iColNo );		// HP �̵�
		ocp.GetValue( Item.m_siHpMax,						iLineNo, iColNo );		// HP �ִ뷮
		ocp.GetValue( Item.m_siMpGain,						iLineNo, iColNo );		// MP �̵�
		ocp.GetValue( Item.m_siMpMax,						iLineNo, iColNo );		// MP �ִ뷮
		ocp.GetValue( Item.m_siHealth,						iLineNo, iColNo );		// �ǰ�
		ocp.GetValue( Item.m_siHealthMax,					iLineNo, iColNo );		// �ǰ� �ִ뷮
		ocp.GetValue( Item.m_siSpecialParam,				iLineNo, iColNo );		// ��
		ocp.GetValue( Item.m_siAttack,						iLineNo, iColNo );		// ���ݷ�
		ocp.GetValue( Item.m_siDefense,						iLineNo, iColNo );		// ����
		ocp.GetValue( Item.m_siDamageRegist,				iLineNo, iColNo );		// Ÿ������
		ocp.GetValue( Item.m_siMagicRegist,					iLineNo, iColNo );		// ��������
		ocp.GetValue( Item.m_siNation,						iLineNo, iColNo );		// ���걹��

		for( i=0 ; i<ON_ITEM_MAX_ADDMAKE; i++ )
		{
			ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// �ʿ� �ڿ� ��Ī(������)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiItemID[i],	iLineNo, iColNo );		// �ʿ� �ڿ� ID
			ocp.GetValue( Item.pMakeItem->m_siItemNum[i],		iLineNo, iColNo );		// �ʿ� �ڿ��� ����
		}
		for( i=0 ; i<ON_ITEM_MAX_TOOL ; i++ )
		{
			ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// �ʿ� ���� ��Ī(������)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiToolID[i],	iLineNo, iColNo );		// �ʿ� ���� ID
		}
		ocp.GetValue( (int&)Item.pMakeItem->m_siLabor,		iLineNo, iColNo );		// �뵿��
		ocp.GetValue( Item.pMakeItem->m_siProduction,		iLineNo, iColNo );		// ���귮
		ocp.GetValue( Item.pMakeItem->m_siMaxProduction,	iLineNo, iColNo );		// �ִ���귮
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// �����ڵ�(������)

//		ocp.GetValue( Item.m_siReqStr,						iLineNo, iColNo );		// �� �䱸ġ
//		ocp.GetValue( Item.m_siReqDex,						iLineNo, iColNo );		// ��ø�� �䱸ġ
//		ocp.GetValue( Item.m_siReqLv,						iLineNo, iColNo );		// ���� �䱸ġ
		ocp.GetValue( Item.m_siReqStr,						iLineNo, iColNo );		// �� �䱸ġ
		ocp.GetValue( Item.m_siReqDex,						iLineNo, iColNo );		// ��ø�� �䱸ġ
		ocp.GetValue( Item.m_siReqLv,						iLineNo, iColNo );		// ���� �䱸ġ
		ocp.GetValue( (char&)Item.m_uiCostChange,			iLineNo, iColNo );		// Cost Change
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// ȿ����
		Item.m_siSound						=	GetSoundID( Item.m_siGroupCode, pszTemp );
		ocp.GetValue( (char&)Item.m_uiTradeGrade,			iLineNo, iColNo );		// �ʿ� �ſ� ���
//		ocp.GetValue( Item.m_siMedic,						iLineNo, iColNo );		// ???
		ocp.GetValue( Item.m_bTimeLimit,					iLineNo, iColNo	);		// �ð����� �ֳ� ����
		ocp.GetValue( Item.m_siDay,							iLineNo, iColNo );		// ���� �ϱ�.

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "LoadItem Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pItemFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )		break;
	}

	ocp.Release();
	fclose( fp );
	AddContainer( v );
	
	return TRUE;
}


//------------------------------------------------------------
//�Լ��� : LoadWeapon
//��  �� : ���� �������� �ε���
//------------------------------------------------------------
BOOL	CItem::LoadWeapon( char *pWeaponFile )
{
	FILE				*fp;
	SI16				i;
	OnlineCommonParser	ocp;

	// ���� �а�
	fp	=	BindJXFile::GetInstance()->GetFile( pWeaponFile );
	if( fp == NULL )		return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pWeaponFile );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Weapon Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pWeaponFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item	= {0,};
		Item.pWeaponHeader	= new WeaponHeader;
		Item.pMakeItem		= new MakeItem;
		Item.m_Attribute	= ON_ITEM_SWORD;

		ocp.GetValue( (int&)Item.m_uiID,						iLineNo, iColNo );		// ������ �ڵ�
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ������ ���̵�(������)
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ������ �̸�(������)
		ocp.GetValue( Item.m_siItemName,						iLineNo, iColNo );		// ������ �̸� �ڵ�
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ������ ����
		Item.m_siGroupCode	=	GetGroup( Item.pWeaponHeader, pszTemp );
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ������ ���� �ڵ�
		Item.m_Attribute	|=	GetNation( pszTemp );
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ����(������)
		ocp.GetValue( Item.m_siModule,							iLineNo, iColNo );		// ���� �ڵ�
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ������ ����(������)
		ocp.GetValue( Item.m_siItemInfo,						iLineNo, iColNo );		// ������ ���� �ڵ�
		ocp.GetValue( (__int64&)Item.m_uiValue,					iLineNo, iColNo );		// ����
		ocp.GetValue( (char&)Item.m_uiWeight,					iLineNo, iColNo );		// ����
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// �ִ�������ɰ���(������)
		ocp.GetValue( Item.m_szSpriteFile,		15,				iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSpriteFont,						iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,				iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSm_SpriteFont,					iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// ���� �����Ѱ� �Ұ��Ѱ�?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_ATTACH_BODY_POSSIBLE;
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// ��� �����Ѱ� �Ұ��Ѱ�?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_USE_POSSIBLE;

		ocp.GetValue( Item.pWeaponHeader->siMinDam,				iLineNo, iColNo );		// ������ �ּ� �����
		ocp.GetValue( Item.pWeaponHeader->siMaxDam,				iLineNo, iColNo );		// ������ �ִ� �����
		ocp.GetValue( Item.m_siDrawID,							iLineNo, iColNo );		// �ش� �׸��� ID
		ocp.GetValue( Item.pWeaponHeader->siAttackRange,		iLineNo, iColNo );		// ���� ����
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ���� Ÿ��
		if(			stricmp	( pszTemp, "Arrow"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_ARROW;
		else if(	stricmp	( pszTemp, "Bullet"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_BULLET;
		else if(	stricmp	( pszTemp, "Dart"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_DART;
		else if(	stricmp	( pszTemp, "sword"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SWORD;
		else if(	stricmp	( pszTemp, "spear"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SEPEAR;
		else if(	stricmp	( pszTemp, "axe"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_AXE;
		else if(	stricmp	( pszTemp, "fan"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_FAN;
		else if(	stricmp	( pszTemp, "craw"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_CRAW;
		else if(	stricmp	( pszTemp, "buddist"	) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_BUDDIST;		
		else if(	stricmp	( pszTemp, "Scythe"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SCYTHE;
		else if(	stricmp	( pszTemp, "SpinStone"	) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_SPINSTONE;
		else if(	stricmp	( pszTemp, "DoubleStick") == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_DOUBLESTICK;
		else if(	stricmp	( pszTemp, "beads"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_BEADS;
		else if(	stricmp	( pszTemp, "empty"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_EMPTY;
		else if(	stricmp	( pszTemp, "firegun"	) == 0 )		Item.pWeaponHeader->siWeaponType	=   ON_ITEM_MISSLE_TYPE_FIREGUN;
		else if(	stricmp	( pszTemp, "dsword"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_DSWORD;
		else if(	stricmp	( pszTemp, "stick"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_STICK;
		else if(	stricmp	( pszTemp, "pearl"		) == 0 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_PEARL;
		else if(	atoi	( pszTemp				) == 1 )		Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_FREE;		
		else														Item.pWeaponHeader->siWeaponType	=	ON_ITEM_MISSLE_TYPE_NONE;
		ocp.GetValue( Item.m_siReqStr,							iLineNo, iColNo );		// �� �䱸ġ
		ocp.GetValue( Item.m_siReqDex,							iLineNo, iColNo );		// ��ø�� �䱸ġ
		ocp.GetValue( Item.m_siReqLv,							iLineNo, iColNo );		// ���� �䱸ġ
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// ȿ����
		Item.m_siSound						=	GetSoundID( Item.m_siGroupCode, pszTemp );
		
		ocp.GetValue( Item.m_siStrength,						iLineNo, iColNo );		// �� ������
		ocp.GetValue( Item.m_siDex,								iLineNo, iColNo );		// ��ø�� ������
		ocp.GetValue( Item.m_siVitality,						iLineNo, iColNo );		// ���� ������
		ocp.GetValue( Item.m_siInt,								iLineNo, iColNo );		// ���� ������
		ocp.GetValue( Item.m_siHpGain,							iLineNo, iColNo );		// HP �̵�
		ocp.GetValue( Item.m_siHpMax,							iLineNo, iColNo );		// HP �ִ뷮
		ocp.GetValue( Item.m_siMpGain,							iLineNo, iColNo );		// MP �̵�
		ocp.GetValue( Item.m_siMpMax,							iLineNo, iColNo );		// MP �ִ뷮
		ocp.GetValue( Item.m_siHealth,							iLineNo, iColNo );		// �ǰ�
		ocp.GetValue( Item.m_siHealthMax,						iLineNo, iColNo );		// �ǰ� �ִ뷮
		ocp.GetValue( (char&)Item.pWeaponHeader->uiGrade,		iLineNo, iColNo );		// ��޹��� +���
		ocp.GetValue( Item.m_siNation,							iLineNo, iColNo );		// ���걹��

		for( i=0 ; i<ON_ITEM_MAX_ADDMAKE; i++ )
		{
			ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// �ʿ� �ڿ� ��Ī(������)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiItemID[i],		iLineNo, iColNo );		// �ʿ� �ڿ� ID
			ocp.GetValue( Item.pMakeItem->m_siItemNum[i],			iLineNo, iColNo );		// �ʿ� �ڿ��� ����
		}
		for( i=0 ; i<ON_ITEM_MAX_TOOL ; i++ )
		{
			ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// �ʿ� ���� ��Ī(������)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiToolID[i],		iLineNo, iColNo );		// �ʿ� ���� ID
		}
		ocp.GetValue( (int&)Item.pMakeItem->m_siLabor,			iLineNo, iColNo );		// �뵿��
		ocp.GetValue( Item.pMakeItem->m_siProduction,			iLineNo, iColNo );		// ���귮
		ocp.GetValue( Item.pMakeItem->m_siMaxProduction,		iLineNo, iColNo );		// �ִ���귮
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// �����ڵ�(������)
		ocp.GetValue( (char&)Item.m_uiCostChange,				iLineNo, iColNo );		// Cost Change
		if( Item.pMakeItem->m_uiItemID[0] == 0 )
			Item.m_Attribute |= ON_ITEM_NO_MATERIAL;
		if( Item.pMakeItem->m_uiToolID[0] == 0 )
			Item.m_Attribute |= ON_ITEM_NO_TOOL;
		ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// ���� Ÿ��
		if		( stricmp( pszTemp, "KillShot"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_KILLSHOT;
		else if	( stricmp( pszTemp, "Seismic"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_SEISMIC;
		else if	( stricmp( pszTemp, "Hidden"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_HIDDEN;
		else if	( stricmp( pszTemp, "VitalSteel"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_VITALSTEEL;
		else if	( stricmp( pszTemp, "MoonAxe"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_MOONAXE;
		else if	( stricmp( pszTemp, "FireBlow"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_FIREBLOW;
		else if	( stricmp( pszTemp, "AstraFire"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ASTRAFIRE;
		else if	( stricmp( pszTemp, "FireArrow"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_FIREARROW;
		else if	( stricmp( pszTemp, "MagicArrow"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_MAGICARROW;
		else if	( stricmp( pszTemp, "WedgeAttack"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_WEDGEATTACK;
		else if	( stricmp( pszTemp, "Bolt"			)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_BOLT;
		else if	( stricmp( pszTemp, "AstralFire"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ASTRALFIRE;
		else if	( stricmp( pszTemp, "Tornado"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_TORNADO;
		else if	( stricmp( pszTemp, "Hold"			)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_HOLD;
		else if	( stricmp( pszTemp, "RunningFire"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_RUNNINGFIRE;
		else if	( stricmp( pszTemp, "Energy"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ENERGYPA;
		else if	( stricmp( pszTemp, "MonkeyStick"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_MONKEYSTICK;
//		else if	( stricmp( pszTemp, "longspear"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_LONG_SPEAR;
//		else if	( stricmp( pszTemp, "doublesword"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_DOUBLE_SWORD;
//		else if	( stricmp( pszTemp, "icebarrier"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_ICE_BARRIER;
//		else if	( stricmp( pszTemp, "FlowerRain"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGIC_FLOWER_NEEDLE_RAIN;
//		else if	( stricmp( pszTemp, "SoulChange"	)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGIC_SOUL_CHANGE;
//		else if	( stricmp( pszTemp, "Stone"			)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGIC_STONE_MAGIC;
//		else if	( stricmp( pszTemp, "GodSpear"		)	== 0 )		Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGIC_GOD_SPEAR;
		else													Item.pWeaponHeader->uiMagic	= ON_ITEM_MAGICATTACK_NONE;
		ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// ���� Ÿ�� ����(������)
		ocp.GetValue( Item.pWeaponHeader->siMagicDesc,			iLineNo, iColNo );		// ���� Ÿ�� ���� �ڵ�
		ocp.GetValue( Item.pWeaponHeader->siMagicAttack,		iLineNo, iColNo );		// ���� ���ݷ�
		ocp.GetValue( (char&)Item.m_uiTradeGrade,				iLineNo, iColNo );		// �ʿ� �ſ� ���
		// actdoll (2005/02/01 11:32) : �ð��� ������ ���� �߰��۾�
		ocp.GetValue( Item.m_bTimeLimit,					iLineNo, iColNo	);		// �ð����� �ֳ� ����
		ocp.GetValue( Item.m_siDay,							iLineNo, iColNo );		// ���� �ϱ�.

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Weapon Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pWeaponFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		_Weapon.push_back( Item );
		m_IDBuffer.push_back( Item.m_uiID );

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )		break;
	}

	ocp.Release();
	fclose( fp );
	AddContainer( &_Weapon );

	return TRUE;
}


//------------------------------------------------------------
//�Լ��� : GetGroup
//��  �� : �������� ������ ������
//------------------------------------------------------------
SI32	CItem::GetGroup( char *pToken ,SI32 *pSpecial)
{
	SI16	siLen	=	strlen( pToken );
	SI32	siRet	=	0;


	switch( pToken[0] )
	{
	case 'W':	siRet	|=	ON_ITEM_SWORD;							break;								// ���� 
	case 'Y':	siRet	|=	ON_ITEM_ACCESSARY;						break;								// �Ǽ��縮
	case 'T':	siRet	|=	ON_ITEM_TOOL;							break;								// ����
	case 'F':	siRet	|=	ON_ITEM_FOOD;							break;								// �Դ°�
	case 'E':	siRet	|=	ON_ITEM_ELEMENT;						break;								// ������ ���۽� ����
	case 'C':	siRet	|=	ON_ITEM_DRESS;		m_siDress++;		break;								// �Ƿ�
	case 'A':	siRet	|=	ON_ITEM_ARMOR;		m_siArmorNum++;		break;								// ����
	case 'O':	siRet	|=	ON_ITEM_ANIMAL;							break;								// ����
	case 'B':	siRet	|=	ON_ITEM_BELT;		m_siBeltNum++;		break;								// ��Ʈ
	case 'S':	siRet	|=	ON_ITEM_BOOTS;		m_siBootsNum++;		break;								// ����
	case 'R':	siRet	|=	ON_ITEM_RING;		m_siRing++;			break;								// ����
	case 'H':	siRet	|=	ON_ITEM_HELMAT;		m_siHelmatNum++;	break;								// ���̹�
	case 'G':	siRet	|=	ON_ITEM_GLOVES;		m_siGlovesNum++;	break;								// �尩
	case 'U':	siRet	|=  ON_ITEM_GUARDIAN;	m_siGuardian++;		break;								// ��ȣ��.
	case 'L':	siRet	|=	ON_ITEM_REVIVE;							break;								// ��Ƴ��� ������
	case 'Z':	siRet	|=	ON_ITEM_TRADE;							break;								// ���
    case 'I':	siRet	|=	ON_ITEM_SPECIAL;						break;								// ���� �ʱ�ȭ	

	}

	if ( siLen >= 2)
	{
		if (siRet & ON_ITEM_SPECIAL)
		{
			switch(pToken[1])
			{
			case 'R':
				*pSpecial = ON_ITEM_SPECIAL_INITIALIZE;
				break;
			case 'V':
				*pSpecial = ON_ITEM_SPECIAL_SHOWMERCENARY;
				break;
			case 'S':
				*pSpecial = ON_ITEM_SPECIAL_CHANGESEX;
				break;
			case 'N':
				*pSpecial = ON_ITEM_SPECIAL_CHANGENATION;
				break;
			case 'C':
				*pSpecial = ON_ITEM_SPECIAL_CREDITUP;
				break;
			case 'M':
				*pSpecial = ON_ITEM_SPECIAL_SEALINGBEAD;		// ������ ����
				break;
			case 'A':
				*pSpecial = ON_ITEM_SPECIAL_MAGICSTONERED;		// ���� ����
				break;
			case 'B':
				*pSpecial = ON_ITEM_SPECIAL_MAGICSTONEBLUE;		// Ǫ�� ����
				break;
			case 'D':
				*pSpecial = ON_ITEM_SPECIAL_MAGICSTONEBLACK;	// ���� ����
				break;
			case 'T':
				*pSpecial = ON_ITEM_SPECIAL_TACTICS;			// ���� ������
				break;
			case 'K':
				*pSpecial = ON_ITEM_SPECIAL_DONKEY;				// �糪��
				break;
			case 'H':
				*pSpecial = ON_ITEM_SPECIAL_IMPERIUM;
				break;
			}

		}
		else 
		{
			switch(pToken[1])
			{
			case 'A':
				siRet |= ON_ITEM_TOALL;
				break;
			case 'F':
				siRet |= ON_ITEM_TOALL;
				siRet |= ON_ITEM_ALL_RECOVER;
				siRet |= ON_ITEM_REVIVE;
				break;
			}
		}
	}
	return siRet;
}

//------------------------------------------------------------
//�Լ��� : GetWearLoc
//��  �� : �������� �����Ҽ� �ִ� ��ġ�� �����Ե� ( Kind )
//------------------------------------------------------------
UI08	CItem::GetWearLoc( UI32 uiID )
{
	CItemHeader		*pItem;

	pItem		=	GetItem( uiID );

	if( pItem )
	{
		if( pItem->m_Attribute & ON_ITEM_ATTACH_BODY_POSSIBLE )
		{
			if( pItem->m_siGroupCode & ON_ITEM_SWORD )				return ON_WEAR_WEAPON_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_ARMOR )			return ON_WEAR_ARMOR_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_DRESS )			return ON_WEAR_ARMOR_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_BOOTS )			return ON_WEAR_SHOES_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_RING )			return ON_WEAR_RRING_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_BELT )			return ON_WEAR_BELT_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_GLOVES )		return ON_WEAR_GLOVE_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_HELMAT )		return ON_WEAR_HELMET_ITEM;
			else if( pItem->m_siGroupCode & ON_ITEM_GUARDIAN )		return ON_WEAR_GUARDIAN_ITEM;
		}
	}
	return ON_WEAR_NO;
}

//------------------------------------------------------------
//�Լ��� : GetNation
//��  �� : ���밡���� ���� ����
//------------------------------------------------------------
SI32	CItem::GetNation( char *pToken )
{
	SI16	i;	
	SI32	siRet = 0;

	if( atoi( pToken ) == 0 )	return 0;

	for( i=0 ; i<3 ; i++ )
	{
		if( pToken[i] != 'X' )					// ���� 
		{
			switch( pToken[i] )
			{
			case 'K':	siRet	|=	ON_ITEM_M_KOREA;				break;
			case 'C':	siRet	|=	ON_ITEM_M_CHINA;				break;
			case 'J':	siRet	|=	ON_ITEM_M_JAPAN;				break;
			case 'T':	siRet	|=	ON_ITEM_M_TAIWAN;				break;
			}
		}
		if( pToken[i+1] != 'X' )				// ����
		{
			switch( pToken[i+1] )
			{
			case 'K':	siRet	|=	ON_ITEM_F_KOREA;				break;
			case 'C':	siRet	|=	ON_ITEM_F_CHINA;				break;
			case 'J':	siRet	|=	ON_ITEM_F_JAPAN;				break;
			case 'T':	siRet	|=	ON_ITEM_F_TAIWAN;				break;
			}
		}
	}

	return siRet;
}


//------------------------------------------------------------
//�Լ��� : GetNation
//��  �� : ������ ���̵� �޾� ������ �׸��� ���̵�� ��ȯ �Ѵ�.
//------------------------------------------------------------
CItemHeader	*CItem::GetItemInfo( UI32 uiItemID, SI16 &siEquip )
{
	CItemHeader		*pItem;


	siEquip		=	-1;
	
	if( uiItemID == 0 )		return NULL;


	pItem =	GetItem( uiItemID );


	if( pItem != NULL )
		siEquip	=	pItem->m_siDrawID;


	return pItem;
}

//------------------------------------------------------------------------
// ��    �� : ������ ���̵� �޾� ���׷��̵� ������ ���������� �˻��Ѵ�.
// �������� : 2003-3-31 18:15
// �� �� �� : �����
//------------------------------------------------------------------------
BOOL	CItem::IsUpgradable( UI32 uiItemID )
{
	CItemHeader		*pItem;
	
	if( uiItemID == 0 )		return FALSE;

	pItem =	GetItem( uiItemID );

	if( pItem->m_Attribute & ON_ITEM_UPGRADE_POSSIBLE )		return TRUE;

	return FALSE;
}

//------------------------------------------------------------
//�Լ��� : GetGroup
//��  �� : �������� ������ ������
//------------------------------------------------------------
SI16	CItem::GetGroup( WeaponHeader *Weapon, char *pToken )
{
	SI16	siLen	=	strlen( pToken );
	SI32	siRet	=	0;
	char	szTemp[4];


	strcpy( szTemp, pToken + 1 );

	switch( pToken[0] )
	{
	case 'W':	siRet	|=	ON_ITEM_SWORD;		break;								// ���� 
	case 'Y':	siRet	|=	ON_ITEM_ACCESSARY;	break;								// �Ǽ��縮
	case 'T':	siRet	|=	ON_ITEM_TOOL;		break;								// ����
	case 'F':	siRet	|=	ON_ITEM_FOOD;		break;								// �Դ°�
	case 'E':	siRet	|=	ON_ITEM_ELEMENT;	break;								// ������ ���۽� ����
	case 'C':	siRet	|=	ON_ITEM_DRESS;		break;								// �Ƿ�
	case 'A':	siRet	|=	ON_ITEM_DRESS;		break;								// ����
	case 'O':	siRet	|=	ON_ITEM_ANIMAL;		break;								// ����
	case 'B':	siRet	|=	ON_ITEM_BELT;		break;								// ��Ʈ
	case 'S':	siRet	|=	ON_ITEM_BOOTS;		break;								// ����
	case 'R':	siRet	|=	ON_ITEM_RING;		break;								// ����
	case 'H':	siRet	|=	ON_ITEM_HELMAT;		break;								// ���̹�
	case 'G':	siRet	|=	ON_ITEM_GLOVES;		break;								// �尩
	case 'U':	siRet	|=  ON_ITEM_GUARDIAN;	break;								// ��ȣ��
	}

	Weapon->siAttackAtb = atoi( szTemp );
	return siRet;
}

SI16	CItem::GetSoundID(SI16 siGroupCode,char *pToken)
{
	SI16 siRet;

	if(strcmp(pToken,"Vegetable") == 0) siRet = ON_FOOD_VEGETABLE;
	else if(strcmp(pToken,"Tea") == 0)  siRet = ON_FOOD_TEA;
	else if(strcmp(pToken,"Special") == 0)   siRet = ON_FOOD_SPECIAL;
	else if(strcmp(pToken,"Nuddle") == 0)    siRet = ON_FOOD_NUDDLE;
	else if(strcmp(pToken,"Meat") == 0)		 siRet = ON_FOOD_MEAT;
	else if(strcmp(pToken,"Fruits") == 0)    siRet = ON_FOOD_FRUITS;
	else if(strcmp(pToken,"Food_ETC") == 0)  siRet = ON_FOOD_ETC;

	else if(strcmp(pToken,"ArmorCloth") == 0) siRet = ON_ITEM_ARMOR_CLOTH;			
	else if(strcmp(pToken,"ArmorMetal") == 0) siRet = ON_ITEM_ARMOR_METAL;		
	else if(strcmp(pToken,"ArmorRare") == 0) siRet = ON_ITEM_ARMOR_RARE;		
	else if(strcmp(pToken,"ArmorWooden") == 0) siRet = ON_ITEM_ARMOR_WOODEN;    
	else if(strcmp(pToken,"ArmorCloth") == 0) siRet = ON_ITEM_ARMOR_CLOTH;
	else if(strcmp(pToken,"BeltMetal") == 0) siRet = ON_ITEM_BELT_METAL;
	else if(strcmp(pToken,"BeltCloth") == 0) siRet = ON_ITEM_BELT_CLOTH;
	else if(strcmp(pToken,"BeltRare") == 0) siRet = ON_ITEM_BELT_RARE;
	else if(strcmp(pToken,"GloveCloth") == 0) siRet = ON_ITEM_GLOVE_CLOTH;
	else if(strcmp(pToken,"GloveMetal") == 0) siRet = ON_ITEM_GLOVE_METAL;
	else if(strcmp(pToken,"GloveRare") == 0) siRet = ON_ITEM_GLOVE_RARE;
	else if(strcmp(pToken,"GloveWooden") == 0) siRet = ON_ITEM_GLOVE_WOODEN;
	else if(strcmp(pToken,"HelmetCloth") == 0) siRet = ON_ITEM_HELMET_CLOTH;
	else if(strcmp(pToken,"HelmetMetal") == 0) siRet = ON_ITEM_HELMET_METAL;
	else if(strcmp(pToken,"HelmetRare") == 0) siRet = ON_ITEM_HELMET_RARE;
	else if(strcmp(pToken,"HelmetWooden") == 0) siRet = ON_ITEM_HELMET_WOODEN;
	else if(strcmp(pToken,"HelmetHat") == 0) siRet = ON_ITEM_HELMET_HAT;
	else if(strcmp(pToken,"ShoesCloth") == 0) siRet = ON_ITEM_SHOES_CLOTH;
	else if(strcmp(pToken,"ShoesEtc") == 0) siRet = ON_ITEM_SHOES_ETC;
	else if(strcmp(pToken,"ShoesMetal") == 0) siRet = ON_ITEM_SHOES_METAL;
	else if(strcmp(pToken,"ShoesWooden") == 0) siRet = ON_ITEM_SHOES_WOODEN;
	else if(strcmp(pToken,"Dart") == 0) siRet = ON_ITEM_WEAPON_DART;
	else if(strcmp(pToken,"Gun") == 0) siRet = ON_ITEM_WEAPON_GUN;
	else if(strcmp(pToken,"Arrow") == 0) siRet = ON_ITEM_WEAPON_ARROW;
	else if(strcmp(pToken,"Ring") == 0) siRet = ON_ITEM_WEAPON_RING;
	else if(strcmp(pToken,"Sword") == 0) siRet = ON_ITEM_WEAPON_SWORD;
	else if(strcmp(pToken,"ETC") == 0 )	siRet = ON_ITEM_ETC;
	else siRet = ON_ITEM_ETC;

	return siRet;

}

void CItem::AddContainer( vector<CItemHeader> *v )
{
	for( vector<CItemHeader>::iterator i=v->begin(); i!=v->end(); ++i)
	{
		efficientAddOrUpdata( _Item, i->m_uiID, i );
	}
}

void CItem::Remove( vector<CItemHeader> *v )
{
	for( vector<CItemHeader>::iterator i=v->begin(); i!=v->end(); ++i)
	{
		if( i->pMakeItem )		delete i->pMakeItem;
		if( i->m_pGuardian )	delete i->m_pGuardian;
		if( i->pWeaponHeader )	delete i->pWeaponHeader;
		if( i->m_pImperium )	delete i->m_pImperium;
	}
}


void CItem::Load(OnlineText *Text, char *pFile)
{
	pOnlineText = Text;

	FILE *fp = BindJXFile::GetInstance()->GetFile( pFile );
	
	CItemHeader Item;

	while(!feof(fp))
	{
		fread(&Item, sizeof(CItemHeader), 1, fp);		

		Item.pMakeItem		= new MakeItem;
		fread(Item.pMakeItem, sizeof(MakeItem), 1, fp);
		
		Item.m_pGuardian	= new Guardian;
		fread(Item.m_pGuardian, sizeof(Guardian), 1, fp);

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

void CItem::AddItem(CItemHeader &Item)
{
	_Items.push_back(Item);	
	efficientAddOrUpdata(_Item, Item.m_uiID, _Items.end()-1);
}



BOOL CItem::LoadImperium( char *pItemFile, vector<CItemHeader> *v )
{
	FILE				*fp;
	OnlineCommonParser	ocp;

	// ������ ����
	fp = BindJXFile::GetInstance()->GetFile(pItemFile);
	if(fp == NULL)		return FALSE;	

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pItemFile );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pItemFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item = {0,};
		Item.m_pImperium = new Imperium;
	
		// �ڷ� �ޱ� ����
		ocp.GetValue( (int&)Item.m_uiID,								iLineNo, iColNo );		// ������ �ڵ�
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// �̸�(������)
		ocp.GetValue( (char&)Item.m_pImperium->m_siID,					iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_uiKind,					iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siHp,					iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siSupply,				iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siPopulation,			iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siPrice,				iLineNo, iColNo );

//		ocp.GetValue( (char&)Item.m_pGuardian->m_siKind,				iLineNo, iColNo );		// ��ȣ�� ����
//		ocp.GetValue( (char&)Item.m_pGuardian->m_siStartCondition,		iLineNo, iColNo );		// �߻� ����
//		ocp.GetValue( (char&)Item.m_pGuardian->m_siSkillNum,			iLineNo, iColNo );		// ��� ����
/*
		for(int i=0; i<ON_GUARDIAN_RECALL_NUM; i++)
		{
			ocp.GetValue( (char&)Item.m_pGuardian->m_siRecallNum[i],		iLineNo, iColNo );		// ��ȯ ������
			ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// ��ȯ ���� ����
			Item.m_pGuardian->m_uiRecallKind[i] = GetID( pszTemp );
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNHitrate[i],			iLineNo, iColNo );		// ���� Ȯ��(�Ϲݸ�)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siBHitrate[i],			iLineNo, iColNo );		// ���� Ȯ��(���Ѹ�)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siSBHitrate[i],		iLineNo, iColNo );		// ���� Ȯ��(������[���ڰ���])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNoHaltSBHitrate[i],	iLineNo, iColNo );		// ���� Ȯ��(������[���ںҰ�])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siAliveTime[i],		iLineNo, iColNo );		// �Ҹ�����
		}
*/
		ocp.GetValue( Item.m_siItemName,								iLineNo, iColNo );		// �̸� �ڵ�
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// �׷�
		Item.m_siGroupCode = GetGroup( pszTemp , &Item.m_siSpecial );
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// ���� ����(������)
		ocp.GetValue( Item.m_siModule,									iLineNo, iColNo );		// ���� �ڵ�
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// ����(������)
		ocp.GetValue( Item.m_siItemInfo,								iLineNo, iColNo );		// ���� �ڵ�
		ocp.GetValue( (__int64&)Item.m_uiValue,							iLineNo, iColNo );		// ����
		ocp.GetValue( (char&)Item.m_uiWeight,							iLineNo, iColNo );		// ����
		ocp.GetValue( siTemp,											iLineNo, iColNo );		// �ִ� ���� ����(������)
		ocp.GetValue( Item.m_szSpriteFile,		15,						iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSpriteFont,								iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,						iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSm_SpriteFont,							iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( Item.m_siReqStr,									iLineNo, iColNo );		// �� �䱸ġ
		ocp.GetValue( Item.m_siReqDex,									iLineNo, iColNo );		// ��÷�� �䱸ġ
		ocp.GetValue( Item.m_siReqLv,									iLineNo, iColNo );		// ���� �䱸ġ
		ocp.GetValue( (char&)Item.m_uiCostChange,						iLineNo, iColNo );		// CostChange
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// Sound
		Item.m_siSound = GetSoundID( Item.m_siGroupCode, pszTemp );
		ocp.GetValue( (char&)Item.m_uiTradeGrade,						iLineNo, iColNo );		// ���� ���� �ſ���

		// robypark 2004/11/10 14:29
		// ������ �����ǿ� ��õ� ĳ���ͷ� ���� �ɼ� �б�
		ocp.GetValue( (char &)Item.m_pImperium->m_scTransformOption,	iLineNo, iColNo );

		/*
		// robypark 2005/1/24 17:44
		ocp.GetValue( (char&)Item.m_pImperium->m_scTransformLevelLimit,	iLineNo, iColNo );		// �ش� ������ �������� �����ϱ� ���� ���� ����
		ocp.GetValue( Item.m_pImperium->m_siTransformCost,				iLineNo, iColNo );		// �ش� ������ �������� �����ϱ� ���� �ݾ�

		// robypark 2005/1/25 17:39
		ocp.GetValue( Item.m_pImperium->m_siArmyKindDescTextID,			iLineNo, iColNo );		// �ش� ������ ���� ���� �ؽ�Ʈ ID

		// robypark 2005/1/27 11:1
		ocp.GetValue( Item.m_pImperium->m_szAnimationResourceID_Blue, 63, 	iLineNo, iColNo );	// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
		ocp.GetValue( Item.m_pImperium->m_szAnimationResourceID_Red,  63,	iLineNo, iColNo );	// �ش� ������ ���� �̹��� ����. ���� ������ ��
		*/

		// ���� ���� �ʱ�ȭ
		Item.m_Attribute = 0;
		Item.m_Attribute |= ON_ITEM_USE_POSSIBLE;
		Item.m_siStrength = 0;		// ��
		Item.m_siDex = 0;			// ��ø
		Item.m_siVitality = 0;		// ����
		Item.m_siInt = 0;			// ����
		Item.m_siHpGain = 0;		// Hp �̵�
		Item.m_siHpMax = 0;			// Hp Max
		Item.m_siMpGain = 0;		// Mp �̵�
		Item.m_siMpMax = 0;			// Mp Max

		Item.m_siHealth = 0;		// �ǰ�
		Item.m_siHealthMax = 0;		// �ǰ� �ִ�ġ
		Item.m_siSpecialParam = 0;	// ��
		Item.m_siAttack = 0;		// ���ݷ�
		Item.m_siDefense = 0;		// ����( AC )
		Item.m_siDamageRegist = 0;	// Ÿ������
		Item.m_siMagicRegist = 0;	// ��������
		Item.m_siMedic = 0;			// Encorg.Medic

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pItemFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )		break;
	}

	fclose( fp );
	ocp.Release();

	AddContainer( v );



	return TRUE;
}
//-----------------------------------------------------------------------------
// Name: LoadGuardian()
// Code: All (2004-07-07)
// Desc: ������ �̹����� TEXT �� �ε���.
//		��ȣ�� ���� �ļ�
//-----------------------------------------------------------------------------
BOOL CItem::LoadGuardian( char *pItemFile, vector<CItemHeader> *v )
{
	FILE				*fp;
	OnlineCommonParser	ocp;

	// ������ ����
	fp = BindJXFile::GetInstance()->GetFile(pItemFile);
	if(fp == NULL)		return FALSE;	

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pItemFile );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pItemFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item = {0,};
		Item.m_pGuardian = new Guardian;
		
		// �ڷ� �ޱ� ����
		ocp.GetValue( (int&)Item.m_uiID,								iLineNo, iColNo );		// ������ �ڵ�
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// �̸�(������)
		ocp.GetValue( (char&)Item.m_pGuardian->m_siKind,				iLineNo, iColNo );		// ��ȣ�� ����
		ocp.GetValue( (char&)Item.m_pGuardian->m_siStartCondition,		iLineNo, iColNo );		// �߻� ����
		ocp.GetValue( (char&)Item.m_pGuardian->m_siSkillNum,			iLineNo, iColNo );		// ��� ����

		for(int i=0; i<ON_GUARDIAN_RECALL_NUM; i++)
		{
			ocp.GetValue( (char&)Item.m_pGuardian->m_siRecallNum[i],		iLineNo, iColNo );		// ��ȯ ������
			ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// ��ȯ ���� ����
			Item.m_pGuardian->m_uiRecallKind[i] = GetID( pszTemp );
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNHitrate[i],			iLineNo, iColNo );		// ���� Ȯ��(�Ϲݸ�)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siBHitrate[i],			iLineNo, iColNo );		// ���� Ȯ��(���Ѹ�)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siSBHitrate[i],		iLineNo, iColNo );		// ���� Ȯ��(������[���ڰ���])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNoHaltSBHitrate[i],	iLineNo, iColNo );		// ���� Ȯ��(������[���ںҰ�])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siAliveTime[i],		iLineNo, iColNo );		// �Ҹ�����
		}

		ocp.GetValue( Item.m_siItemName,								iLineNo, iColNo );		// �̸� �ڵ�
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// �׷�
		Item.m_siGroupCode = GetGroup( pszTemp , &Item.m_siSpecial );
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// ���� ����(������)
		ocp.GetValue( Item.m_siModule,									iLineNo, iColNo );		// ���� �ڵ�
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// ����(������)
		ocp.GetValue( Item.m_siItemInfo,								iLineNo, iColNo );		// ���� �ڵ�
		ocp.GetValue( (__int64&)Item.m_uiValue,							iLineNo, iColNo );		// ����
		ocp.GetValue( (char&)Item.m_uiWeight,							iLineNo, iColNo );		// ����
		ocp.GetValue( siTemp,											iLineNo, iColNo );		// �ִ� ���� ����(������)
		ocp.GetValue( Item.m_szSpriteFile,		15,						iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSpriteFont,								iLineNo, iColNo );		// �κ��丮 ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,						iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ �̸�
		ocp.GetValue( Item.m_siSm_SpriteFont,							iLineNo, iColNo );		// �ʵ� ������ ��������Ʈ ��Ʈ ��ȣ
		ocp.GetValue( Item.m_siReqStr,									iLineNo, iColNo );		// �� �䱸ġ
		ocp.GetValue( Item.m_siReqDex,									iLineNo, iColNo );		// ��÷�� �䱸ġ
		ocp.GetValue( Item.m_siReqLv,									iLineNo, iColNo );		// ���� �䱸ġ
		ocp.GetValue( (char&)Item.m_uiCostChange,						iLineNo, iColNo );		// CostChange
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// Sound
		Item.m_siSound = GetSoundID( Item.m_siGroupCode, pszTemp );
		ocp.GetValue( (char&)Item.m_uiTradeGrade,						iLineNo, iColNo );		// ���� ���� �ſ���
		// actdoll (2005/02/01 11:32) : �ð��� ������ ���� �߰��۾�
		ocp.GetValue( Item.m_bTimeLimit,					iLineNo, iColNo	);		// �ð����� �ֳ� ����
		ocp.GetValue( Item.m_siDay,							iLineNo, iColNo );		// ���� �ϱ�.

		// ���� ���� �ʱ�ȭ
		Item.m_Attribute = 0;
		Item.m_Attribute |= ON_ITEM_ATTACH_BODY_POSSIBLE;	// ���� ����
		Item.m_siStrength = 0;		// ��
		Item.m_siDex = 0;			// ��ø
		Item.m_siVitality = 0;		// ����
		Item.m_siInt = 0;			// ����
		Item.m_siHpGain = 0;		// Hp �̵�
		Item.m_siHpMax = 0;			// Hp Max
		Item.m_siMpGain = 0;		// Mp �̵�
		Item.m_siMpMax = 0;			// Mp Max

		Item.m_siHealth = 0;		// �ǰ�
		Item.m_siHealthMax = 0;		// �ǰ� �ִ�ġ
		Item.m_siSpecialParam = 0;	// ��
		Item.m_siAttack = 0;		// ���ݷ�
		Item.m_siDefense = 0;		// ����( AC )
		Item.m_siDamageRegist = 0;	// Ÿ������
		Item.m_siMagicRegist = 0;	// ��������
		Item.m_siMedic = 0;			// Encorg.Medic

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pItemFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )		break;
	}

	fclose( fp );
	ocp.Release();

	AddContainer( v );
	
	return TRUE;
}

UI16 CItem::GetID(CHAR *pToken)
{
	UI16	uiVal;

	uiVal		=	MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}

BOOL CItem::IsTimeLimitItem(UI32 uiItemID)
{
	CItemHeader		*pItem;

	if( uiItemID == 0 )		return FALSE;

	pItem =	GetItem( uiItemID );

	if( pItem != NULL )
		return pItem->m_bTimeLimit;

	return FALSE;
}

SI16 CItem::GetItemUsingDay(UI32 uiItemID)
{
	CItemHeader		*pItem;

	if( uiItemID == 0 )		return 0;

	pItem =	GetItem( uiItemID );

	if( pItem != NULL )
		return pItem->m_siDay;

	return 0;
	
}