//===================================================================================================================================================
//	FileName	:	CharFileLoadManager.cpp
//	Discription	:	캐릭터 파일 관리
//	History		:	2001/10/23(박연숙) - Create
//===================================================================================================================================================
#include <GSL.h>

#include "CharFileLoadManager.h"
#include "message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	CCharFileLoadManager()
// Description	:	Construction Function
CCharFileLoadManager::CCharFileLoadManager()
{
	//-------------------------------------------
	// ARRAY INIT
	ZeroMemory(m_bSetXSpr,			sizeof(m_bSetXSpr));
	ZeroMemory(m_bSetDefaultSpr,	sizeof(m_bSetDefaultSpr));
	ZeroMemory(m_XSpr,				sizeof(m_XSpr));
	ZeroMemory(m_DefaultSpr,		sizeof(m_DefaultSpr));

	//-------------------------------------------
	// HERO CONFIG (COF파일의 정보를 읽어서 설정한다)
	ZeroMemory(m_HeroConfig,		sizeof(m_HeroConfig));
				
	//-------------------------------------------
	// 장착 정보
	ZeroMemory(m_HumanInfo,			sizeof(m_HumanInfo));
	ZeroMemory(m_WeaponInfo,		sizeof(m_WeaponInfo));
	ZeroMemory(m_ArmorInfo,			sizeof(m_ArmorInfo));
	ZeroMemory(m_HelmetInfo,		sizeof(m_HelmetInfo));

	//-------------------------------------------
	// SPR을 공유하기위한 장소
	ZeroMemory(m_HeroCurrEquip,		sizeof(m_HeroCurrEquip));
	ZeroMemory(m_HeroPrevEquip,		sizeof(m_HeroPrevEquip));

	ZeroMemory(m_DefaultPrevEquip,  sizeof(m_DefaultPrevEquip));
	ZeroMemory(m_DefaultCurrEquip,  sizeof(m_DefaultCurrEquip));
	

	ZeroMemory(&m_HeroShareSpr,		sizeof(m_HeroShareSpr));
	ZeroMemory(&m_HeroDefaultSpr,	sizeof(m_HeroDefaultSpr));
	
	pOnlineLoadXSprProc = new cltLoadXsprProc;


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	~CCharFileLoadManager()
// Description	:	Destruction Function
CCharFileLoadManager::~CCharFileLoadManager()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			:	LoadInfo()
// Description	:	정보 파일들을 읽어들인다. 실패시 FALSE 리턴
BOOL CCharFileLoadManager::LoadInfo()
{
	
	LoadHeroConfigInfo(HERO_KM);
	LoadHeroConfigInfo(HERO_KW);
	LoadHeroConfigInfo(HERO_JM);
	LoadHeroConfigInfo(HERO_JW);
	LoadHeroConfigInfo(HERO_CM);
	LoadHeroConfigInfo(HERO_CW);
	LoadHeroConfigInfo(HERO_TM);
	LoadHeroConfigInfo(HERO_TW);
	


	//-------------------------------------------
	// HERO ITEM 파일을 읽어온다. 
	LoadHumanInfo("Online\\JohapChar\\Text\\Humen.txt");

	LoadArmorInfo("Online\\JohapChar\\Text\\Armor.txt");

	LoadHelmetInfo("Online\\JohapChar\\Text\\Helmet.txt");

	LoadWeaponInfo("Online\\JohapChar\\Text\\Weapon.txt");


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			: FindEmptySprite()
// Description	: Sprite를 읽어들일수 있는 빈 XSpr배열을 찾는다.
BOOL CCharFileLoadManager::FindEmptySprite(SI32 &siFileIndex)
{
	//-------------------------------------------
	// 비어있는 배열을 찾는다.
	for( SI32 siCount = 1; siCount < MAX_XSPR; siCount++ )
	{
		if( m_bSetXSpr[siCount] == FALSE )
		{
			siFileIndex = siCount;	return TRUE;
		}
	}

	return FALSE;
}

BOOL CCharFileLoadManager::FindDefaultHeroFile(SI32 &siFileIndex)
{
	//-------------------------------------------
	// 비어있는 배열을 찾는다.
	for( SI32 siCount = 1; siCount < MAX_XSPR; siCount++ )
	{
		if( m_bSetDefaultSpr[siCount] == FALSE )
		{
			siFileIndex = siCount;	return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name			: Free()
// Description	: 
void CCharFileLoadManager::Free()
{
	FreeHeroSpriteAll();

	pOnlineLoadXSprProc->Stop();

	while( !pOnlineLoadXSprProc->HasStopped() )		Sleep(10);

	if( pOnlineLoadXSprProc )
	{
		delete pOnlineLoadXSprProc;
		pOnlineLoadXSprProc = NULL;
	}
}


