//=============================================================================
//	FileName	:	CharFileLoadManagerStruct.h
//	Discription	:	조합형 캐릭터 파일 관리에 관련한 구조체들
//	History		:	2001/10/23(박연숙) - Create
//					2002/01/07(박연숙) - 전반적인 구조 수정
//=============================================================================

#ifndef _CHARFILELOADMANAGERSTRUCT_H_
#define _CHARFILELOADMANAGERSTRUCT_H_

//-----------------------------------------------------------------------------
// 최대 스프라이트 갯수
//-----------------------------------------------------------------------------
#define MAX_XSPR				500			// 게임에서는 300정도만 일단 사용한다.

//-----------------------------------------------------------------------------
// ACTION
// 현재는 주인공과 캐릭터의 클래스가 동일하므로 같은 Action 순서와 Set를 구성해야한다.
//-----------------------------------------------------------------------------

// 주인공의 행동동작  
#define HERO_ACT_A1				0			// 공격
#define HERO_ACT_N1				1			// 제자리
#define HERO_ACT_WL				2			// 걷기
#define HERO_ACT_DT				3			// 죽기

#define MAX_HEROACTIONS			4	


//-----------------------------------------------------------------------------
// WEAPONCLASS
//-----------------------------------------------------------------------------
// 무기에 따라 행동할 수 있는 동작 CLASS
#define HERO_CLASS_1HS			0			// 한손 스윙
#define HERO_CLASS_2HS			1			// 두손 스윙
#define HERO_CLASS_2HT			2			//???
#define HERO_CLASS_THW			3			//???
#define HERO_CLASS_BOW			4			// 활
#define HERO_CLASS_HTH			5			// 맨손
#define HERO_CLASS_RIF			6			// 총
#define HERO_CLASS_SP1			7			// 기본 동작
#define HERO_CLASS_SP2			8			// 특수 동작
#define HERO_CLASS_SP3			9			// 특수 동작
#define HERO_CLASS_SP4			10			// 특수 동작
#define HERO_CLASS_SP5			11			// 특수 동작

#define MAX_HEROWEAPONCLASS		12			// 무기가 행동할수있는 동작

//-----------------------------------------------------------------------------
// BODYPART
//-----------------------------------------------------------------------------
// 하나의 캐릭터를 조합하기 위한 PART
#define HERO_BODY_LG			0
#define HERO_BODY_TR			1
#define HERO_BODY_HD			2
#define HERO_BODY_RA			3
#define HERO_BODY_LA			4
#define HERO_BODY_RH			5
#define HERO_BODY_LH			6
#define HERO_BODY_HE			7
#define HERO_BODY_RF			8
#define HERO_BODY_LF			9
#define HERO_BODY_S1			10
#define HERO_BODY_S2			11
#define HERO_BODY_S3			12

#define MAX_HEROBODYPART		13			// 하나의 캐릭터를 조합하기 위한 PART0

//=================================
// 캐릭터의 Animation Speed
//=================================
#define CHAR_SLOWSPEED			0
#define CHAR_NORMALSPEED		1
#define CHAR_FASTSPEED			2
#define CHAR_VERYFASTSPEED		3

#define MAX_ATTACKSPEEDTYPE		4
//=================================


//-----------------------------------------------------------------------------
// CONFIG
//-----------------------------------------------------------------------------
#define MAX_CFGHEADER			9		// Config 파일 헤더 부분의 총 라인 수

#define MAX_NAMECODESTR			5
#define MAX_PARTCODESTR			7

#define MAX_BODYDIR				16		
#define MAX_BODYFRAME			60

#define MAX_FULLPATHNAME		30


//-----------------------------------------------------------------------------
// MAX HERO IN MULTIPLAY
//-----------------------------------------------------------------------------
#define HERO_KM					1
#define HERO_JM					2
#define HERO_CM					3
#define HERO_KW					4
#define HERO_JW					5
#define HERO_CW					6
#define MAX_HERO_KIND_NUM		6		// 총 조합형 주인공 캐릭터 종류 수

#define MAX_HEROID_NUM			100		//mmm 한꺼번에 등장하는 최대 주인공 조합형 캐릭터 수

//============================================================================
// EXEL INFORMATION
//============================================================================

#define MAX_LINESTR				3072	// Excel 파일의 라인의 문자갯수

//------------
// info
#define MAX_ITEMINFO			100



//-----------------------------------------------------------------------------
// .cof 파일에 대한 정보
//-----------------------------------------------------------------------------
struct SCharHeroConfig
{
	SI32	siTotalPiece[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];
	SI32	siDirections[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];
	SI32	siTotalFrame[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_ATTACKSPEEDTYPE];
	SI32	siFontCount[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];

	SI32	siCenterX[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];
	SI32	siCenterY[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];

	char	cNation[MAX_NAMECODESTR];																// 국가코드 "K"
	char	cSex[MAX_NAMECODESTR];																	// 성별코드 "MA"
	SI32	siSex;																					// 성별코드(1: 남자, 2: 여자, 3: 몬스터)

	//---------------------------------
	// PART
	char	cPartCode[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART][MAX_PARTCODESTR];	// .cof 파일중 "LG	A11HSU	0 0 0 0" 라인에서 "A11HSU"

	SI32	siOffsetX[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];
	SI32	siOffsetY[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];
	SI32	siRender[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];						// 반투명인가

	// DRAW
	SI08	siPartDrawSort[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_BODYDIR][MAX_HEROBODYPART];	// 각 BODY PART의 찍는 순서 
	SI08	siPartFrame[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_ATTACKSPEEDTYPE][MAX_BODYFRAME];	// BODY의 ANIMATION
	//[MAX_ATTACKSPEEDTYPE]
};





//-------------------------------------------------------------------------------------------------
// 인간 정보 구조체
//-------------------------------------------------------------------------------------------------
struct SCharHumanInfo
{
	char cName[30];										// 인간 이름 
	SI32 siID;	
	char cLeftArm[MAX_NAMECODESTR];						// LEFT Arm CODE
	char cRightArm[MAX_NAMECODESTR];					// RIGHT Arm CODE
	char cLeg[MAX_NAMECODESTR];							// Leg CODE
	char cSP1[MAX_NAMECODESTR];							// Sp1 CODE
};


//-------------------------------------------------------------------------------------------------
// 갑옷 정보 구조체
//-------------------------------------------------------------------------------------------------
struct SCharArmorInfo
{
	char cName[30];										// ARMOR 이름 
	SI32 siID;	
	char cLeftShoulder[MAX_NAMECODESTR];				// LEFT SHOULDER CODE
	char cRightShoulder[MAX_NAMECODESTR];				// RIGHT SHOULDER CODE
	char cTorso[MAX_NAMECODESTR];						// TORSO CODE
	BOOL bArmor;										// 갑옷인가?
};


//-------------------------------------------------------------------------------------------------
// 무기 정보 구조체
//-------------------------------------------------------------------------------------------------
struct SCharWeaponInfo
{
	char cName[30];										// WEAPON 이름 
	SI32 siID;	
	char cWClass[MAX_NAMECODESTR];						// WEAPON CLASS
	char cLeftWeapon[MAX_NAMECODESTR];					// Left Weapon Code
	char cRightWeapon[MAX_NAMECODESTR];					// Right Weapon Code
};


//-------------------------------------------------------------------------------------------------
// 헬멧 정보 구조체
//-------------------------------------------------------------------------------------------------
struct SCharHelmetInfo
{
	char cName[30];										// HELMET 이름 
	SI32 siID;	
	char cHelmet[MAX_NAMECODESTR];						// HELMET CODE
};



//-------------------------------------------------------------------------------------------------
// SPR을 공유하기 위한 장소
//-------------------------------------------------------------------------------------------------
struct SCharHeroCurrEquip
{
	BOOL	bInUse;										// 구조체 사용 프래그

	//---------------------------------
	// 각 HERO의 장착 상태 
	SI32	siHumanInfoID;
	SI32	siWeaponInfoID;
	SI32	siArmorInfoID;
	SI32	siHelmetInfoID;

	BOOL	bPart[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];	// SCharHeroConfig의 cPartCode 값이 있다면 bPart == TRUE

	// 각각의 캐릭터의 동작에 대한 파일 ID
	char	cFileName[MAX_HEROACTIONS][MAX_HEROBODYPART][MAX_FULLPATHNAME];	// Ex) "TR\GNTRLITA11HSU.SPR"
	BOOL	bChanged[MAX_HEROACTIONS][MAX_HEROBODYPART];
	SI32	siFileID[MAX_HEROACTIONS][MAX_HEROBODYPART];						// Spr 배열 인덱스 번호. SCharHeroShareSpr의 siShareFileID[X] 값을 가짐.
	SI32	siShareIndex[MAX_HEROACTIONS][MAX_HEROBODYPART];					// 나중에 파일을 메모리에서 해제시켜야 하는지 알아보기 위해 설정한다. 윗줄 주석에서의 X 값을 가짐.

};

struct SCharHeroShareSpr
{
	// 해당 파일에 대한 파일 ID
	char cShareFileName[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM][MAX_FULLPATHNAME];
	SI32 siShareFileID[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];	// Spr 배열 인덱스 번호
	SI32 siShareFileUse[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];
};



class CCharFileLoadManager
{
private:
	BOOL						LoadHeroConfigInfo(SI32 siKindID);

	// HERO CODE
	SI32						GetHeroBodyCode(char * pBody);
	SI32						GetHeroActCode(char * pAct);
	SI32						GetHeroClassCode(char * pClass);

public:
	//-------------------------------------------
	// SPRITE 
	//-------------------------------------------
	BOOL						m_bSetXSpr[MAX_XSPR];
	XSPR						m_XSpr[MAX_XSPR];

	//-------------------------------------------
	// CONFIG (COF파일의 정보를 읽어서 설정한다)
	//-------------------------------------------
	SCharHeroConfig				m_HeroConfig[MAX_HERO_KIND_NUM];					// 주인공의 COF파일을 모두 읽어 설정한다.

	//-------------------------------------------
	// 장착 정보
	//-------------------------------------------
	SCharHumanInfo				m_HumanInfo[MAX_ITEMINFO];
	SCharWeaponInfo				m_WeaponInfo[MAX_ITEMINFO];
	SCharArmorInfo				m_ArmorInfo[MAX_ITEMINFO];
	SCharHelmetInfo				m_HelmetInfo[MAX_ITEMINFO];

	//-------------------------------------------
	// SPR을 공유하기위한 장소
	//-------------------------------------------
	SCharHeroCurrEquip			m_HeroCurrEquip[MAX_CHAR_NUMBER];					// 현재 각 HERO의 ITEM 상태를 설정한다.
	SCharHeroCurrEquip			m_HeroPrevEquip[MAX_CHAR_NUMBER];					// 변경전의 각 HERO의 ITEM 상태를 설정한다.

	SCharHeroShareSpr			m_HeroShareSpr;										// 각 HERO들이 SPR을 공유할 수 있도록 정보를 설정한다.

public:
	CCharFileLoadManager();
	~CCharFileLoadManager();

	void						FreeSprites(SI32 siSprKind);


	BOOL						LoadHeroSprite(char *pFileName, SI32 &siFileIndex);
	BOOL						FreeHeroSprite(SI32 siFileIndex);
	void						FreeHeroSpriteAll();
	void						Free();
	BOOL						FindEmptySprite(SI32 &siFileIndex);


	BOOL						LoadInfo();
	BOOL						MakeCurrHeroFileName(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip, CHAR *pDefaultBody);
	// HERO의 그림파일들을 읽어들인다.
	BOOL						LoadHeroFiles(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip);
	BOOL						SetHeroConfig(SI32 siKindID, SCharHeroCurrEquip *pCurrEquip);
	
	// 디폴트 장착 정보를 셋팅한다.
	BOOL						SetDefaultItemInfo(CHAR *pDefaultBody);
	// 인간 정보를 파일로부터 읽어들인다.
	BOOL						LoadHumanInfo(char *pFileName);
	// 갑옷 정보를 파일로부터 읽어들인다.
	BOOL						LoadArmorInfo(char *pFileName);
	// 무기 정보를 파일로부터 읽어들인다.
	BOOL						LoadWeaponInfo(char *pFileName);
	// 헬멧 정보를 파일로부터 읽어들인다.
	BOOL						LoadHelmetInfo(char *pFileName);

	SI32						GetHeroClassCode(SCharHeroCurrEquip *pEquip);

	// 캐릭터에게 아이템 장착 정보를 심는다.
	BOOL						SetChange(SI32 siEquip, SI32 siEquipInfoID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody);
	// 캐릭터에게 모든 아이템 장착 정보를 심는다.
	BOOL						SetAllChange(SI32 siArmorID, SI32 siHelmetID, SI32 siWeaponID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody);

};

#endif


