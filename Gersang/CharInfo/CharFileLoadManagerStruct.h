//=============================================================================
//	FileName	:	CharFileLoadManagerStruct.h
//	Discription	:	������ ĳ���� ���� ������ ������ ����ü��
//	History		:	2001/10/23(�ڿ���) - Create
//					2002/01/07(�ڿ���) - �������� ���� ����
//=============================================================================

#ifndef _CHARFILELOADMANAGERSTRUCT_H_
#define _CHARFILELOADMANAGERSTRUCT_H_

//-----------------------------------------------------------------------------
// �ִ� ��������Ʈ ����
//-----------------------------------------------------------------------------
#define MAX_XSPR				500			// ���ӿ����� 300������ �ϴ� ����Ѵ�.

//-----------------------------------------------------------------------------
// ACTION
// ����� ���ΰ��� ĳ������ Ŭ������ �����ϹǷ� ���� Action ������ Set�� �����ؾ��Ѵ�.
//-----------------------------------------------------------------------------

// ���ΰ��� �ൿ����  
#define HERO_ACT_A1				0			// ����
#define HERO_ACT_N1				1			// ���ڸ�
#define HERO_ACT_WL				2			// �ȱ�
#define HERO_ACT_DT				3			// �ױ�

#define MAX_HEROACTIONS			4	


//-----------------------------------------------------------------------------
// WEAPONCLASS
//-----------------------------------------------------------------------------
// ���⿡ ���� �ൿ�� �� �ִ� ���� CLASS
#define HERO_CLASS_1HS			0			// �Ѽ� ����
#define HERO_CLASS_2HS			1			// �μ� ����
#define HERO_CLASS_2HT			2			//???
#define HERO_CLASS_THW			3			//???
#define HERO_CLASS_BOW			4			// Ȱ
#define HERO_CLASS_HTH			5			// �Ǽ�
#define HERO_CLASS_RIF			6			// ��
#define HERO_CLASS_SP1			7			// �⺻ ����
#define HERO_CLASS_SP2			8			// Ư�� ����
#define HERO_CLASS_SP3			9			// Ư�� ����
#define HERO_CLASS_SP4			10			// Ư�� ����
#define HERO_CLASS_SP5			11			// Ư�� ����

#define MAX_HEROWEAPONCLASS		12			// ���Ⱑ �ൿ�Ҽ��ִ� ����

//-----------------------------------------------------------------------------
// BODYPART
//-----------------------------------------------------------------------------
// �ϳ��� ĳ���͸� �����ϱ� ���� PART
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

#define MAX_HEROBODYPART		13			// �ϳ��� ĳ���͸� �����ϱ� ���� PART0

//=================================
// ĳ������ Animation Speed
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
#define MAX_CFGHEADER			9		// Config ���� ��� �κ��� �� ���� ��

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
#define MAX_HERO_KIND_NUM		6		// �� ������ ���ΰ� ĳ���� ���� ��

#define MAX_HEROID_NUM			100		//mmm �Ѳ����� �����ϴ� �ִ� ���ΰ� ������ ĳ���� ��

//============================================================================
// EXEL INFORMATION
//============================================================================

#define MAX_LINESTR				3072	// Excel ������ ������ ���ڰ���

//------------
// info
#define MAX_ITEMINFO			100



//-----------------------------------------------------------------------------
// .cof ���Ͽ� ���� ����
//-----------------------------------------------------------------------------
struct SCharHeroConfig
{
	SI32	siTotalPiece[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];
	SI32	siDirections[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];
	SI32	siTotalFrame[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_ATTACKSPEEDTYPE];
	SI32	siFontCount[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];

	SI32	siCenterX[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];
	SI32	siCenterY[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS];

	char	cNation[MAX_NAMECODESTR];																// �����ڵ� "K"
	char	cSex[MAX_NAMECODESTR];																	// �����ڵ� "MA"
	SI32	siSex;																					// �����ڵ�(1: ����, 2: ����, 3: ����)

	//---------------------------------
	// PART
	char	cPartCode[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART][MAX_PARTCODESTR];	// .cof ������ "LG	A11HSU	0 0 0 0" ���ο��� "A11HSU"

	SI32	siOffsetX[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];
	SI32	siOffsetY[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];
	SI32	siRender[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];						// �������ΰ�

	// DRAW
	SI08	siPartDrawSort[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_BODYDIR][MAX_HEROBODYPART];	// �� BODY PART�� ��� ���� 
	SI08	siPartFrame[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_ATTACKSPEEDTYPE][MAX_BODYFRAME];	// BODY�� ANIMATION
	//[MAX_ATTACKSPEEDTYPE]
};





//-------------------------------------------------------------------------------------------------
// �ΰ� ���� ����ü
//-------------------------------------------------------------------------------------------------
struct SCharHumanInfo
{
	char cName[30];										// �ΰ� �̸� 
	SI32 siID;	
	char cLeftArm[MAX_NAMECODESTR];						// LEFT Arm CODE
	char cRightArm[MAX_NAMECODESTR];					// RIGHT Arm CODE
	char cLeg[MAX_NAMECODESTR];							// Leg CODE
	char cSP1[MAX_NAMECODESTR];							// Sp1 CODE
};


//-------------------------------------------------------------------------------------------------
// ���� ���� ����ü
//-------------------------------------------------------------------------------------------------
struct SCharArmorInfo
{
	char cName[30];										// ARMOR �̸� 
	SI32 siID;	
	char cLeftShoulder[MAX_NAMECODESTR];				// LEFT SHOULDER CODE
	char cRightShoulder[MAX_NAMECODESTR];				// RIGHT SHOULDER CODE
	char cTorso[MAX_NAMECODESTR];						// TORSO CODE
	BOOL bArmor;										// �����ΰ�?
};


//-------------------------------------------------------------------------------------------------
// ���� ���� ����ü
//-------------------------------------------------------------------------------------------------
struct SCharWeaponInfo
{
	char cName[30];										// WEAPON �̸� 
	SI32 siID;	
	char cWClass[MAX_NAMECODESTR];						// WEAPON CLASS
	char cLeftWeapon[MAX_NAMECODESTR];					// Left Weapon Code
	char cRightWeapon[MAX_NAMECODESTR];					// Right Weapon Code
};


//-------------------------------------------------------------------------------------------------
// ��� ���� ����ü
//-------------------------------------------------------------------------------------------------
struct SCharHelmetInfo
{
	char cName[30];										// HELMET �̸� 
	SI32 siID;	
	char cHelmet[MAX_NAMECODESTR];						// HELMET CODE
};



//-------------------------------------------------------------------------------------------------
// SPR�� �����ϱ� ���� ���
//-------------------------------------------------------------------------------------------------
struct SCharHeroCurrEquip
{
	BOOL	bInUse;										// ����ü ��� ������

	//---------------------------------
	// �� HERO�� ���� ���� 
	SI32	siHumanInfoID;
	SI32	siWeaponInfoID;
	SI32	siArmorInfoID;
	SI32	siHelmetInfoID;

	BOOL	bPart[MAX_HEROACTIONS][MAX_HEROWEAPONCLASS][MAX_HEROBODYPART];	// SCharHeroConfig�� cPartCode ���� �ִٸ� bPart == TRUE

	// ������ ĳ������ ���ۿ� ���� ���� ID
	char	cFileName[MAX_HEROACTIONS][MAX_HEROBODYPART][MAX_FULLPATHNAME];	// Ex) "TR\GNTRLITA11HSU.SPR"
	BOOL	bChanged[MAX_HEROACTIONS][MAX_HEROBODYPART];
	SI32	siFileID[MAX_HEROACTIONS][MAX_HEROBODYPART];						// Spr �迭 �ε��� ��ȣ. SCharHeroShareSpr�� siShareFileID[X] ���� ����.
	SI32	siShareIndex[MAX_HEROACTIONS][MAX_HEROBODYPART];					// ���߿� ������ �޸𸮿��� �������Ѿ� �ϴ��� �˾ƺ��� ���� �����Ѵ�. ���� �ּ������� X ���� ����.

};

struct SCharHeroShareSpr
{
	// �ش� ���Ͽ� ���� ���� ID
	char cShareFileName[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM][MAX_FULLPATHNAME];
	SI32 siShareFileID[MAX_HEROACTIONS*MAX_HEROBODYPART*MAX_HEROID_NUM];	// Spr �迭 �ε��� ��ȣ
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
	// CONFIG (COF������ ������ �о �����Ѵ�)
	//-------------------------------------------
	SCharHeroConfig				m_HeroConfig[MAX_HERO_KIND_NUM];					// ���ΰ��� COF������ ��� �о� �����Ѵ�.

	//-------------------------------------------
	// ���� ����
	//-------------------------------------------
	SCharHumanInfo				m_HumanInfo[MAX_ITEMINFO];
	SCharWeaponInfo				m_WeaponInfo[MAX_ITEMINFO];
	SCharArmorInfo				m_ArmorInfo[MAX_ITEMINFO];
	SCharHelmetInfo				m_HelmetInfo[MAX_ITEMINFO];

	//-------------------------------------------
	// SPR�� �����ϱ����� ���
	//-------------------------------------------
	SCharHeroCurrEquip			m_HeroCurrEquip[MAX_CHAR_NUMBER];					// ���� �� HERO�� ITEM ���¸� �����Ѵ�.
	SCharHeroCurrEquip			m_HeroPrevEquip[MAX_CHAR_NUMBER];					// �������� �� HERO�� ITEM ���¸� �����Ѵ�.

	SCharHeroShareSpr			m_HeroShareSpr;										// �� HERO���� SPR�� ������ �� �ֵ��� ������ �����Ѵ�.

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
	// HERO�� �׸����ϵ��� �о���δ�.
	BOOL						LoadHeroFiles(SI32 siKindID, SCharHeroCurrEquip *pPrevEquip, SCharHeroCurrEquip *pCurrEquip);
	BOOL						SetHeroConfig(SI32 siKindID, SCharHeroCurrEquip *pCurrEquip);
	
	// ����Ʈ ���� ������ �����Ѵ�.
	BOOL						SetDefaultItemInfo(CHAR *pDefaultBody);
	// �ΰ� ������ ���Ϸκ��� �о���δ�.
	BOOL						LoadHumanInfo(char *pFileName);
	// ���� ������ ���Ϸκ��� �о���δ�.
	BOOL						LoadArmorInfo(char *pFileName);
	// ���� ������ ���Ϸκ��� �о���δ�.
	BOOL						LoadWeaponInfo(char *pFileName);
	// ��� ������ ���Ϸκ��� �о���δ�.
	BOOL						LoadHelmetInfo(char *pFileName);

	SI32						GetHeroClassCode(SCharHeroCurrEquip *pEquip);

	// ĳ���Ϳ��� ������ ���� ������ �ɴ´�.
	BOOL						SetChange(SI32 siEquip, SI32 siEquipInfoID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody);
	// ĳ���Ϳ��� ��� ������ ���� ������ �ɴ´�.
	BOOL						SetAllChange(SI32 siArmorID, SI32 siHelmetID, SI32 siWeaponID, SI32 siKindID, SI32 siID, CHAR *pDefaultBody);

};

#endif


