//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	임진록 온라인 [거상] - 아이템 데이터베이스 모듈
//	File Name		:	OnlineItem.cpp
//	Birth Date		:	2001. 04. 20.
//	Creator			:	정 진 욱, 이 상 원
//	Editer			:	조 현 준
//	Copyright		:	Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		아이템과 관련된 처리를 총괄하는 클래스입니다. 서버와 클라이언트 양쪽에서 사용합니다.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: 
// Code: All (2004-07-07)
// Desc: 헤더 및 정의 선언
//-----------------------------------------------------------------------------
#include <GSL.h>
#include	<search.h>
#include	<io.h>
#include	"OnLineItem.h"
#include	"bindJXFile.h"
#include	"utility.h"
#include	"OnlineCommonParser.h"		// actdoll (2004/07/07 17:54) : 공용 파서 시스템



//-----------------------------------------------------------------------------
// Name: CItem()
// Code: All (2004-07-07)
// Desc: 생성자
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
// Desc: 소멸자
//-----------------------------------------------------------------------------
CItem::~CItem()
{
	ReMoveAll();	
	m_IDBuffer.clear();
}

//-----------------------------------------------------------------------------
// Name: ReMoveAll()
// Code: All (2004-07-07)
// Desc: 메모리에서 모든 아이템를 날린다.
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
// Desc: 아이템 이미지와 TEXT 를 로드함.
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
//함수명 : GetItem
//설  명 : ID를 받어 ID와 동일한 아이템을 리턴을 해준다
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
//함수명 : GetItem
//설  명 : 아이템 이름을 받어 ID와 동일한 아이템을 리턴을 해준다
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
//함수명 : GetItemHeader
//설  명 : 모든 아이템을 모아놓은 pItemsInfo에 저장된 순서(index)에 의해 아이템을 리턴해준다
//------------------------------------------------------------
CItemHeader*	CItem::GetItemHeader( UI32 index )
{
	if( index == -1 ) return NULL;
	map<UI32, CItemHeader*, less<UI32> >::iterator i = _Item.begin();
	for( int ix=0; ix<index; ++ix, ++i );
	return i->second;
}

//------------------------------------------------------------
//함수명 : SetItemFile
//설  명 : 리소스에 로드 되어 있는 아이템의 파일의 인덱스를 셋팅 해준다.
//------------------------------------------------------------
VOID CItem::SetFile(SI32 siFile, CHAR *pFileName, SI16 siXsize, SI16 siYsize, BOOL bSm)
{
	if( bSm == FALSE )
	{
		// 모든 아이템 헤더를 검색해서 같은 파일명이 있다면 인자로 받은 리소스의 인덱스 번호를 넣어 준다.
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
//함수명 : GetItemHeaderFromImperium
//파라미터: SI32 siKindOnlineId: 서버에서 사용되는 온라인 캐릭터 ID
//설  명 : 공성전을 위한 함수로 변신되는 캐릭터(Online) ID를
//		   이용하여 변신 아이템정보 헤더를 찾는다.
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
//함수명 : GetItemHeaderFromImperiumByIndex
//파라미터: SI32 siImperiumIndex: 소집권 아이템의 인덱스
//설  명 : 공성전을 위한 함수로 변신되는 소집권 아이템의 인덱스
//			를 이용하여 아이템 정보 헤더를 찾는다.
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
//함수명 : GetSpriteFileName
//설  명 : 헤더에서 아이템의 이름을 리턴 받는다.
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
//함수명 : LoadItem
//설  명 : 무기를 제외한 아이템을 로드함
//------------------------------------------------------------
BOOL	CItem::LoadItem( char *pItemFile, vector<CItemHeader> *v )
{
	FILE				*fp;
	SI16				i;
	OnlineCommonParser	ocp;

	// 파일 읽고
	fp = BindJXFile::GetInstance()->GetFile( pItemFile );
	if( fp == NULL )		return FALSE;	

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pItemFile );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "LoadItem Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pItemFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item	= {0,};
		Item.pMakeItem		= new MakeItem;
		Item.m_Attribute	= 0;

		ocp.GetValue( (int&)Item.m_uiID,					iLineNo, iColNo );		// 아이템 코드
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 아이템 아이디(사용안함)
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 아이템 이름(사용안함)
		ocp.GetValue( Item.m_siItemName,					iLineNo, iColNo );		// 아이템 이름 코드
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 아이템 종류
		Item.m_siGroupCode	=	GetGroup( pszTemp , &Item.m_siSpecial);
		ocp.GetValue( Item.m_siDrawID,						iLineNo, iColNo );		// 그림의 ID(?)
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 단위(사용안함)
		ocp.GetValue( Item.m_siModule,						iLineNo, iColNo );		// 단위 코드
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 아이템 설명(사용안함)
		ocp.GetValue( Item.m_siItemInfo,					iLineNo, iColNo );		// 아이템 설명 코드
		ocp.GetValue( (__int64&)Item.m_uiValue,				iLineNo, iColNo );		// 가격
		ocp.GetValue( (char&)Item.m_uiWeight,				iLineNo, iColNo );		// 무게
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// 최대소지가능갯수(사용안함)
		ocp.GetValue( Item.m_szSpriteFile,		15,			iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSpriteFont,					iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 폰트 번호
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,			iLineNo, iColNo );		// 필드 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSm_SpriteFont,				iLineNo, iColNo );		// 필드 아이템 스프라이트 폰트 번호
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// 착용 가능한가 불가한가?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_ATTACH_BODY_POSSIBLE;
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// 사용 가능한가 불가한가?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_USE_POSSIBLE;
		ocp.GetValue( Item.m_siStrength,					iLineNo, iColNo );		// 힘 증가량
		ocp.GetValue( Item.m_siDex,							iLineNo, iColNo );		// 민첩성 증가량
		ocp.GetValue( Item.m_siVitality,					iLineNo, iColNo );		// 생명 증가량
		ocp.GetValue( Item.m_siInt,							iLineNo, iColNo );		// 지능 증가량
		ocp.GetValue( Item.m_siHpGain,						iLineNo, iColNo );		// HP 이득
		ocp.GetValue( Item.m_siHpMax,						iLineNo, iColNo );		// HP 최대량
		ocp.GetValue( Item.m_siMpGain,						iLineNo, iColNo );		// MP 이득
		ocp.GetValue( Item.m_siMpMax,						iLineNo, iColNo );		// MP 최대량
		ocp.GetValue( Item.m_siHealth,						iLineNo, iColNo );		// 건강
		ocp.GetValue( Item.m_siHealthMax,					iLineNo, iColNo );		// 건강 최대량
		ocp.GetValue( Item.m_siSpecialParam,				iLineNo, iColNo );		// 명성
		ocp.GetValue( Item.m_siAttack,						iLineNo, iColNo );		// 공격력
		ocp.GetValue( Item.m_siDefense,						iLineNo, iColNo );		// 방어력
		ocp.GetValue( Item.m_siDamageRegist,				iLineNo, iColNo );		// 타격저항
		ocp.GetValue( Item.m_siMagicRegist,					iLineNo, iColNo );		// 마법저항
		ocp.GetValue( Item.m_siNation,						iLineNo, iColNo );		// 생산국가

		for( i=0 ; i<ON_ITEM_MAX_ADDMAKE; i++ )
		{
			ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 필요 자원 명칭(사용안함)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiItemID[i],	iLineNo, iColNo );		// 필요 자원 ID
			ocp.GetValue( Item.pMakeItem->m_siItemNum[i],		iLineNo, iColNo );		// 필요 자원의 갯수
		}
		for( i=0 ; i<ON_ITEM_MAX_TOOL ; i++ )
		{
			ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 필요 도구 명칭(사용안함)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiToolID[i],	iLineNo, iColNo );		// 필요 도구 ID
		}
		ocp.GetValue( (int&)Item.pMakeItem->m_siLabor,		iLineNo, iColNo );		// 노동력
		ocp.GetValue( Item.pMakeItem->m_siProduction,		iLineNo, iColNo );		// 생산량
		ocp.GetValue( Item.pMakeItem->m_siMaxProduction,	iLineNo, iColNo );		// 최대생산량
		ocp.GetValue( siTemp,								iLineNo, iColNo );		// 농장코드(사용안함)

//		ocp.GetValue( Item.m_siReqStr,						iLineNo, iColNo );		// 힘 요구치
//		ocp.GetValue( Item.m_siReqDex,						iLineNo, iColNo );		// 민첩성 요구치
//		ocp.GetValue( Item.m_siReqLv,						iLineNo, iColNo );		// 레벨 요구치
		ocp.GetValue( Item.m_siReqStr,						iLineNo, iColNo );		// 힘 요구치
		ocp.GetValue( Item.m_siReqDex,						iLineNo, iColNo );		// 민첩성 요구치
		ocp.GetValue( Item.m_siReqLv,						iLineNo, iColNo );		// 레벨 요구치
		ocp.GetValue( (char&)Item.m_uiCostChange,			iLineNo, iColNo );		// Cost Change
		ocp.GetValue( pszTemp,					255,		iLineNo, iColNo );		// 효과음
		Item.m_siSound						=	GetSoundID( Item.m_siGroupCode, pszTemp );
		ocp.GetValue( (char&)Item.m_uiTradeGrade,			iLineNo, iColNo );		// 필요 신용 등급
//		ocp.GetValue( Item.m_siMedic,						iLineNo, iColNo );		// ???
		ocp.GetValue( Item.m_bTimeLimit,					iLineNo, iColNo	);		// 시간제한 있나 없나
		ocp.GetValue( Item.m_siDay,							iLineNo, iColNo );		// 몇일 일까.

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "LoadItem Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pItemFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )		break;
	}

	ocp.Release();
	fclose( fp );
	AddContainer( v );
	
	return TRUE;
}


//------------------------------------------------------------
//함수명 : LoadWeapon
//설  명 : 무기 아이템을 로드함
//------------------------------------------------------------
BOOL	CItem::LoadWeapon( char *pWeaponFile )
{
	FILE				*fp;
	SI16				i;
	OnlineCommonParser	ocp;

	// 파일 읽고
	fp	=	BindJXFile::GetInstance()->GetFile( pWeaponFile );
	if( fp == NULL )		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pWeaponFile );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Weapon Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pWeaponFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item	= {0,};
		Item.pWeaponHeader	= new WeaponHeader;
		Item.pMakeItem		= new MakeItem;
		Item.m_Attribute	= ON_ITEM_SWORD;

		ocp.GetValue( (int&)Item.m_uiID,						iLineNo, iColNo );		// 아이템 코드
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 아이템 아이디(사용안함)
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 아이템 이름(사용안함)
		ocp.GetValue( Item.m_siItemName,						iLineNo, iColNo );		// 아이템 이름 코드
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 아이템 종류
		Item.m_siGroupCode	=	GetGroup( Item.pWeaponHeader, pszTemp );
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 아이템 종류 코드
		Item.m_Attribute	|=	GetNation( pszTemp );
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 단위(사용안함)
		ocp.GetValue( Item.m_siModule,							iLineNo, iColNo );		// 단위 코드
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 아이템 설명(사용안함)
		ocp.GetValue( Item.m_siItemInfo,						iLineNo, iColNo );		// 아이템 설명 코드
		ocp.GetValue( (__int64&)Item.m_uiValue,					iLineNo, iColNo );		// 가격
		ocp.GetValue( (char&)Item.m_uiWeight,					iLineNo, iColNo );		// 무게
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// 최대소지가능갯수(사용안함)
		ocp.GetValue( Item.m_szSpriteFile,		15,				iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSpriteFont,						iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 폰트 번호
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,				iLineNo, iColNo );		// 필드 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSm_SpriteFont,					iLineNo, iColNo );		// 필드 아이템 스프라이트 폰트 번호
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// 착용 가능한가 불가한가?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_ATTACH_BODY_POSSIBLE;
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// 사용 가능한가 불가한가?
		if( siTemp != 0 )	Item.m_Attribute	|=	ON_ITEM_USE_POSSIBLE;

		ocp.GetValue( Item.pWeaponHeader->siMinDam,				iLineNo, iColNo );		// 무기의 최소 대미지
		ocp.GetValue( Item.pWeaponHeader->siMaxDam,				iLineNo, iColNo );		// 무기의 최대 대미지
		ocp.GetValue( Item.m_siDrawID,							iLineNo, iColNo );		// 해당 그림의 ID
		ocp.GetValue( Item.pWeaponHeader->siAttackRange,		iLineNo, iColNo );		// 공격 범위
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 무기 타입
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
		ocp.GetValue( Item.m_siReqStr,							iLineNo, iColNo );		// 힘 요구치
		ocp.GetValue( Item.m_siReqDex,							iLineNo, iColNo );		// 민첩성 요구치
		ocp.GetValue( Item.m_siReqLv,							iLineNo, iColNo );		// 레벨 요구치
		ocp.GetValue( pszTemp,					255,			iLineNo, iColNo );		// 효과음
		Item.m_siSound						=	GetSoundID( Item.m_siGroupCode, pszTemp );
		
		ocp.GetValue( Item.m_siStrength,						iLineNo, iColNo );		// 힘 증가량
		ocp.GetValue( Item.m_siDex,								iLineNo, iColNo );		// 민첩성 증가량
		ocp.GetValue( Item.m_siVitality,						iLineNo, iColNo );		// 생명 증가량
		ocp.GetValue( Item.m_siInt,								iLineNo, iColNo );		// 지능 증가량
		ocp.GetValue( Item.m_siHpGain,							iLineNo, iColNo );		// HP 이득
		ocp.GetValue( Item.m_siHpMax,							iLineNo, iColNo );		// HP 최대량
		ocp.GetValue( Item.m_siMpGain,							iLineNo, iColNo );		// MP 이득
		ocp.GetValue( Item.m_siMpMax,							iLineNo, iColNo );		// MP 최대량
		ocp.GetValue( Item.m_siHealth,							iLineNo, iColNo );		// 건강
		ocp.GetValue( Item.m_siHealthMax,						iLineNo, iColNo );		// 건강 최대량
		ocp.GetValue( (char&)Item.pWeaponHeader->uiGrade,		iLineNo, iColNo );		// 고급무기 +등급
		ocp.GetValue( Item.m_siNation,							iLineNo, iColNo );		// 생산국가

		for( i=0 ; i<ON_ITEM_MAX_ADDMAKE; i++ )
		{
			ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// 필요 자원 명칭(사용안함)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiItemID[i],		iLineNo, iColNo );		// 필요 자원 ID
			ocp.GetValue( Item.pMakeItem->m_siItemNum[i],			iLineNo, iColNo );		// 필요 자원의 갯수
		}
		for( i=0 ; i<ON_ITEM_MAX_TOOL ; i++ )
		{
			ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// 필요 도구 명칭(사용안함)
			ocp.GetValue( (int&)Item.pMakeItem->m_uiToolID[i],		iLineNo, iColNo );		// 필요 도구 ID
		}
		ocp.GetValue( (int&)Item.pMakeItem->m_siLabor,			iLineNo, iColNo );		// 노동력
		ocp.GetValue( Item.pMakeItem->m_siProduction,			iLineNo, iColNo );		// 생산량
		ocp.GetValue( Item.pMakeItem->m_siMaxProduction,		iLineNo, iColNo );		// 최대생산량
		ocp.GetValue( siTemp,									iLineNo, iColNo );		// 농장코드(사용안함)
		ocp.GetValue( (char&)Item.m_uiCostChange,				iLineNo, iColNo );		// Cost Change
		if( Item.pMakeItem->m_uiItemID[0] == 0 )
			Item.m_Attribute |= ON_ITEM_NO_MATERIAL;
		if( Item.pMakeItem->m_uiToolID[0] == 0 )
			Item.m_Attribute |= ON_ITEM_NO_TOOL;
		ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// 마법 타입
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
		ocp.GetValue( pszTemp,						255,		iLineNo, iColNo );		// 마법 타입 설명(사용안함)
		ocp.GetValue( Item.pWeaponHeader->siMagicDesc,			iLineNo, iColNo );		// 마법 타입 설명 코드
		ocp.GetValue( Item.pWeaponHeader->siMagicAttack,		iLineNo, iColNo );		// 마법 공격력
		ocp.GetValue( (char&)Item.m_uiTradeGrade,				iLineNo, iColNo );		// 필요 신용 등급
		// actdoll (2005/02/01 11:32) : 시간제 아이템 관련 추가작업
		ocp.GetValue( Item.m_bTimeLimit,					iLineNo, iColNo	);		// 시간제한 있나 없나
		ocp.GetValue( Item.m_siDay,							iLineNo, iColNo );		// 몇일 일까.

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Weapon Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pWeaponFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		_Weapon.push_back( Item );
		m_IDBuffer.push_back( Item.m_uiID );

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )		break;
	}

	ocp.Release();
	fclose( fp );
	AddContainer( &_Weapon );

	return TRUE;
}


//------------------------------------------------------------
//함수명 : GetGroup
//설  명 : 아이템의 종류를 리턴함
//------------------------------------------------------------
SI32	CItem::GetGroup( char *pToken ,SI32 *pSpecial)
{
	SI16	siLen	=	strlen( pToken );
	SI32	siRet	=	0;


	switch( pToken[0] )
	{
	case 'W':	siRet	|=	ON_ITEM_SWORD;							break;								// 무기 
	case 'Y':	siRet	|=	ON_ITEM_ACCESSARY;						break;								// 악세사리
	case 'T':	siRet	|=	ON_ITEM_TOOL;							break;								// 공구
	case 'F':	siRet	|=	ON_ITEM_FOOD;							break;								// 먹는거
	case 'E':	siRet	|=	ON_ITEM_ELEMENT;						break;								// 아이템 제작시 원료
	case 'C':	siRet	|=	ON_ITEM_DRESS;		m_siDress++;		break;								// 의류
	case 'A':	siRet	|=	ON_ITEM_ARMOR;		m_siArmorNum++;		break;								// 갑옷
	case 'O':	siRet	|=	ON_ITEM_ANIMAL;							break;								// 동물
	case 'B':	siRet	|=	ON_ITEM_BELT;		m_siBeltNum++;		break;								// 벨트
	case 'S':	siRet	|=	ON_ITEM_BOOTS;		m_siBootsNum++;		break;								// 부츠
	case 'R':	siRet	|=	ON_ITEM_RING;		m_siRing++;			break;								// 반지
	case 'H':	siRet	|=	ON_ITEM_HELMAT;		m_siHelmatNum++;	break;								// 하이바
	case 'G':	siRet	|=	ON_ITEM_GLOVES;		m_siGlovesNum++;	break;								// 장갑
	case 'U':	siRet	|=  ON_ITEM_GUARDIAN;	m_siGuardian++;		break;								// 수호부.
	case 'L':	siRet	|=	ON_ITEM_REVIVE;							break;								// 살아나는 아이템
	case 'Z':	siRet	|=	ON_ITEM_TRADE;							break;								// 장부
    case 'I':	siRet	|=	ON_ITEM_SPECIAL;						break;								// 스탯 초기화	

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
				*pSpecial = ON_ITEM_SPECIAL_SEALINGBEAD;		// 봉인의 구슬
				break;
			case 'A':
				*pSpecial = ON_ITEM_SPECIAL_MAGICSTONERED;		// 붉은 마석
				break;
			case 'B':
				*pSpecial = ON_ITEM_SPECIAL_MAGICSTONEBLUE;		// 푸른 마석
				break;
			case 'D':
				*pSpecial = ON_ITEM_SPECIAL_MAGICSTONEBLACK;	// 검은 마석
				break;
			case 'T':
				*pSpecial = ON_ITEM_SPECIAL_TACTICS;			// 손자 병법서
				break;
			case 'K':
				*pSpecial = ON_ITEM_SPECIAL_DONKEY;				// 당나귀
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
//함수명 : GetWearLoc
//설  명 : 아이템이 착용할수 있는 위치를 리턴함둥 ( Kind )
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
//함수명 : GetNation
//설  명 : 착용가능한 나라를 리턴
//------------------------------------------------------------
SI32	CItem::GetNation( char *pToken )
{
	SI16	i;	
	SI32	siRet = 0;

	if( atoi( pToken ) == 0 )	return 0;

	for( i=0 ; i<3 ; i++ )
	{
		if( pToken[i] != 'X' )					// 남자 
		{
			switch( pToken[i] )
			{
			case 'K':	siRet	|=	ON_ITEM_M_KOREA;				break;
			case 'C':	siRet	|=	ON_ITEM_M_CHINA;				break;
			case 'J':	siRet	|=	ON_ITEM_M_JAPAN;				break;
			case 'T':	siRet	|=	ON_ITEM_M_TAIWAN;				break;
			}
		}
		if( pToken[i+1] != 'X' )				// 여자
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
//함수명 : GetNation
//설  명 : 아이템 아이디를 받아 착용할 그림의 아이디로 변환 한다.
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
// 설    명 : 아이템 아이디를 받아 업그레이드 가능한 아이템인지 검사한다.
// 수정일자 : 2003-3-31 18:15
// 작 성 자 : 백상현
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
//함수명 : GetGroup
//설  명 : 아이템의 종류를 리턴함
//------------------------------------------------------------
SI16	CItem::GetGroup( WeaponHeader *Weapon, char *pToken )
{
	SI16	siLen	=	strlen( pToken );
	SI32	siRet	=	0;
	char	szTemp[4];


	strcpy( szTemp, pToken + 1 );

	switch( pToken[0] )
	{
	case 'W':	siRet	|=	ON_ITEM_SWORD;		break;								// 무기 
	case 'Y':	siRet	|=	ON_ITEM_ACCESSARY;	break;								// 악세사리
	case 'T':	siRet	|=	ON_ITEM_TOOL;		break;								// 공구
	case 'F':	siRet	|=	ON_ITEM_FOOD;		break;								// 먹는거
	case 'E':	siRet	|=	ON_ITEM_ELEMENT;	break;								// 아이템 제작시 원료
	case 'C':	siRet	|=	ON_ITEM_DRESS;		break;								// 의류
	case 'A':	siRet	|=	ON_ITEM_DRESS;		break;								// 갑옷
	case 'O':	siRet	|=	ON_ITEM_ANIMAL;		break;								// 동물
	case 'B':	siRet	|=	ON_ITEM_BELT;		break;								// 벨트
	case 'S':	siRet	|=	ON_ITEM_BOOTS;		break;								// 부츠
	case 'R':	siRet	|=	ON_ITEM_RING;		break;								// 반지
	case 'H':	siRet	|=	ON_ITEM_HELMAT;		break;								// 하이바
	case 'G':	siRet	|=	ON_ITEM_GLOVES;		break;								// 장갑
	case 'U':	siRet	|=  ON_ITEM_GUARDIAN;	break;								// 수호부
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

	// 파일을 열고
	fp = BindJXFile::GetInstance()->GetFile(pItemFile);
	if(fp == NULL)		return FALSE;	

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pItemFile );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pItemFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item = {0,};
		Item.m_pImperium = new Imperium;
	
		// 자료 받기 개시
		ocp.GetValue( (int&)Item.m_uiID,								iLineNo, iColNo );		// 아이템 코드
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 이름(사용안함)
		ocp.GetValue( (char&)Item.m_pImperium->m_siID,					iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_uiKind,					iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siHp,					iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siSupply,				iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siPopulation,			iLineNo, iColNo );
		ocp.GetValue( (int&)Item.m_pImperium->m_siPrice,				iLineNo, iColNo );

//		ocp.GetValue( (char&)Item.m_pGuardian->m_siKind,				iLineNo, iColNo );		// 수호부 종류
//		ocp.GetValue( (char&)Item.m_pGuardian->m_siStartCondition,		iLineNo, iColNo );		// 발생 조건
//		ocp.GetValue( (char&)Item.m_pGuardian->m_siSkillNum,			iLineNo, iColNo );		// 기술 갯수
/*
		for(int i=0; i<ON_GUARDIAN_RECALL_NUM; i++)
		{
			ocp.GetValue( (char&)Item.m_pGuardian->m_siRecallNum[i],		iLineNo, iColNo );		// 소환 마리수
			ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 소환 몬스터 종류
			Item.m_pGuardian->m_uiRecallKind[i] = GetID( pszTemp );
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNHitrate[i],			iLineNo, iColNo );		// 적중 확률(일반몹)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siBHitrate[i],			iLineNo, iColNo );		// 적중 확률(강한몹)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siSBHitrate[i],		iLineNo, iColNo );		// 적중 확률(보스몹[포박가능])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNoHaltSBHitrate[i],	iLineNo, iColNo );		// 적중 확률(보스몹[포박불가])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siAliveTime[i],		iLineNo, iColNo );		// 소멸조건
		}
*/
		ocp.GetValue( Item.m_siItemName,								iLineNo, iColNo );		// 이름 코드
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 그룹
		Item.m_siGroupCode = GetGroup( pszTemp , &Item.m_siSpecial );
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 유닛 단위(사용안함)
		ocp.GetValue( Item.m_siModule,									iLineNo, iColNo );		// 유닛 코드
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 설명문(사용안함)
		ocp.GetValue( Item.m_siItemInfo,								iLineNo, iColNo );		// 설명 코드
		ocp.GetValue( (__int64&)Item.m_uiValue,							iLineNo, iColNo );		// 가격
		ocp.GetValue( (char&)Item.m_uiWeight,							iLineNo, iColNo );		// 무게
		ocp.GetValue( siTemp,											iLineNo, iColNo );		// 최대 소지 갯수(사용안함)
		ocp.GetValue( Item.m_szSpriteFile,		15,						iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSpriteFont,								iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 폰트 번호
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,						iLineNo, iColNo );		// 필드 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSm_SpriteFont,							iLineNo, iColNo );		// 필드 아이템 스프라이트 폰트 번호
		ocp.GetValue( Item.m_siReqStr,									iLineNo, iColNo );		// 힘 요구치
		ocp.GetValue( Item.m_siReqDex,									iLineNo, iColNo );		// 민첨성 요구치
		ocp.GetValue( Item.m_siReqLv,									iLineNo, iColNo );		// 레벨 요구치
		ocp.GetValue( (char&)Item.m_uiCostChange,						iLineNo, iColNo );		// CostChange
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// Sound
		Item.m_siSound = GetSoundID( Item.m_siGroupCode, pszTemp );
		ocp.GetValue( (char&)Item.m_uiTradeGrade,						iLineNo, iColNo );		// 구입 가능 신용등급

		// robypark 2004/11/10 14:29
		// 공성전 소집권에 명시된 캐릭터로 변신 옵션 읽기
		ocp.GetValue( (char &)Item.m_pImperium->m_scTransformOption,	iLineNo, iColNo );

		/*
		// robypark 2005/1/24 17:44
		ocp.GetValue( (char&)Item.m_pImperium->m_scTransformLevelLimit,	iLineNo, iColNo );		// 해당 공성전 유닛으로 변신하기 위한 레벨 제한
		ocp.GetValue( Item.m_pImperium->m_siTransformCost,				iLineNo, iColNo );		// 해당 공성전 유닛으로 변신하기 위한 금액

		// robypark 2005/1/25 17:39
		ocp.GetValue( Item.m_pImperium->m_siArmyKindDescTextID,			iLineNo, iColNo );		// 해당 공성전 유닛 설명 텍스트 ID

		// robypark 2005/1/27 11:1
		ocp.GetValue( Item.m_pImperium->m_szAnimationResourceID_Blue, 63, 	iLineNo, iColNo );	// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
		ocp.GetValue( Item.m_pImperium->m_szAnimationResourceID_Red,  63,	iLineNo, iColNo );	// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때
		*/

		// 빠진 값들 초기화
		Item.m_Attribute = 0;
		Item.m_Attribute |= ON_ITEM_USE_POSSIBLE;
		Item.m_siStrength = 0;		// 힘
		Item.m_siDex = 0;			// 민첩
		Item.m_siVitality = 0;		// 생명
		Item.m_siInt = 0;			// 지능
		Item.m_siHpGain = 0;		// Hp 이득
		Item.m_siHpMax = 0;			// Hp Max
		Item.m_siMpGain = 0;		// Mp 이득
		Item.m_siMpMax = 0;			// Mp Max

		Item.m_siHealth = 0;		// 건강
		Item.m_siHealthMax = 0;		// 건강 최대치
		Item.m_siSpecialParam = 0;	// 명성
		Item.m_siAttack = 0;		// 공격력
		Item.m_siDefense = 0;		// 방어력( AC )
		Item.m_siDamageRegist = 0;	// 타격저항
		Item.m_siMagicRegist = 0;	// 마법저항
		Item.m_siMedic = 0;			// Encorg.Medic

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pItemFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
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
// Desc: 아이템 이미지와 TEXT 를 로드함.
//		수호부 관련 파서
//-----------------------------------------------------------------------------
BOOL CItem::LoadGuardian( char *pItemFile, vector<CItemHeader> *v )
{
	FILE				*fp;
	OnlineCommonParser	ocp;

	// 파일을 열고
	fp = BindJXFile::GetInstance()->GetFile(pItemFile);
	if(fp == NULL)		return FALSE;	

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pItemFile );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pItemFile, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		SI32	siTemp;
		char	pszTemp[256];
		CItemHeader	Item = {0,};
		Item.m_pGuardian = new Guardian;
		
		// 자료 받기 개시
		ocp.GetValue( (int&)Item.m_uiID,								iLineNo, iColNo );		// 아이템 코드
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 이름(사용안함)
		ocp.GetValue( (char&)Item.m_pGuardian->m_siKind,				iLineNo, iColNo );		// 수호부 종류
		ocp.GetValue( (char&)Item.m_pGuardian->m_siStartCondition,		iLineNo, iColNo );		// 발생 조건
		ocp.GetValue( (char&)Item.m_pGuardian->m_siSkillNum,			iLineNo, iColNo );		// 기술 갯수

		for(int i=0; i<ON_GUARDIAN_RECALL_NUM; i++)
		{
			ocp.GetValue( (char&)Item.m_pGuardian->m_siRecallNum[i],		iLineNo, iColNo );		// 소환 마리수
			ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 소환 몬스터 종류
			Item.m_pGuardian->m_uiRecallKind[i] = GetID( pszTemp );
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNHitrate[i],			iLineNo, iColNo );		// 적중 확률(일반몹)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siBHitrate[i],			iLineNo, iColNo );		// 적중 확률(강한몹)
			ocp.GetValue( (char&)Item.m_pGuardian->m_siSBHitrate[i],		iLineNo, iColNo );		// 적중 확률(보스몹[포박가능])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siNoHaltSBHitrate[i],	iLineNo, iColNo );		// 적중 확률(보스몹[포박불가])
			ocp.GetValue( (char&)Item.m_pGuardian->m_siAliveTime[i],		iLineNo, iColNo );		// 소멸조건
		}

		ocp.GetValue( Item.m_siItemName,								iLineNo, iColNo );		// 이름 코드
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 그룹
		Item.m_siGroupCode = GetGroup( pszTemp , &Item.m_siSpecial );
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 유닛 단위(사용안함)
		ocp.GetValue( Item.m_siModule,									iLineNo, iColNo );		// 유닛 코드
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// 설명문(사용안함)
		ocp.GetValue( Item.m_siItemInfo,								iLineNo, iColNo );		// 설명 코드
		ocp.GetValue( (__int64&)Item.m_uiValue,							iLineNo, iColNo );		// 가격
		ocp.GetValue( (char&)Item.m_uiWeight,							iLineNo, iColNo );		// 무게
		ocp.GetValue( siTemp,											iLineNo, iColNo );		// 최대 소지 갯수(사용안함)
		ocp.GetValue( Item.m_szSpriteFile,		15,						iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSpriteFont,								iLineNo, iColNo );		// 인벤토리 아이템 스프라이트 폰트 번호
		ocp.GetValue( Item.m_szSm_SpriteFile,	15,						iLineNo, iColNo );		// 필드 아이템 스프라이트 이름
		ocp.GetValue( Item.m_siSm_SpriteFont,							iLineNo, iColNo );		// 필드 아이템 스프라이트 폰트 번호
		ocp.GetValue( Item.m_siReqStr,									iLineNo, iColNo );		// 힘 요구치
		ocp.GetValue( Item.m_siReqDex,									iLineNo, iColNo );		// 민첨성 요구치
		ocp.GetValue( Item.m_siReqLv,									iLineNo, iColNo );		// 레벨 요구치
		ocp.GetValue( (char&)Item.m_uiCostChange,						iLineNo, iColNo );		// CostChange
		ocp.GetValue( pszTemp,					255,					iLineNo, iColNo );		// Sound
		Item.m_siSound = GetSoundID( Item.m_siGroupCode, pszTemp );
		ocp.GetValue( (char&)Item.m_uiTradeGrade,						iLineNo, iColNo );		// 구입 가능 신용등급
		// actdoll (2005/02/01 11:32) : 시간제 아이템 관련 추가작업
		ocp.GetValue( Item.m_bTimeLimit,					iLineNo, iColNo	);		// 시간제한 있나 없나
		ocp.GetValue( Item.m_siDay,							iLineNo, iColNo );		// 몇일 일까.

		// 빠진 값들 초기화
		Item.m_Attribute = 0;
		Item.m_Attribute |= ON_ITEM_ATTACH_BODY_POSSIBLE;	// 착용 가능
		Item.m_siStrength = 0;		// 힘
		Item.m_siDex = 0;			// 민첩
		Item.m_siVitality = 0;		// 생명
		Item.m_siInt = 0;			// 지능
		Item.m_siHpGain = 0;		// Hp 이득
		Item.m_siHpMax = 0;			// Hp Max
		Item.m_siMpGain = 0;		// Mp 이득
		Item.m_siMpMax = 0;			// Mp Max

		Item.m_siHealth = 0;		// 건강
		Item.m_siHealthMax = 0;		// 건강 최대치
		Item.m_siSpecialParam = 0;	// 명성
		Item.m_siAttack = 0;		// 공격력
		Item.m_siDefense = 0;		// 방어력( AC )
		Item.m_siDamageRegist = 0;	// 타격저항
		Item.m_siMagicRegist = 0;	// 마법저항
		Item.m_siMedic = 0;			// Encorg.Medic

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pItemFile, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		v->push_back( Item );		
		m_IDBuffer.push_back( Item.m_uiID );

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
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