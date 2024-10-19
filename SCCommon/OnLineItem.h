/************************************************************************************
파일명	:	OnLineItem.h
작성자	:	정 진 욱
담당자  :	이 상 원
작성일  :	2001.04.20
수정일  :	2002.02.16
************************************************************************************/

#ifndef	_ONLINE_ITEM_H
#define _ONLINE_ITEM_H

#pragma warning ( disable : 4786 )

#define ON_STRING_ITEM_NAME							32
#define ON_STRING_ITEM_INFO							256
#define ON_STRING_ITEM_FILE							50

//아이템 속성
#define	ON_ITEM_NONE								0x00000000
#define	ON_ITEM_ATTACH_BODY_POSSIBLE				0x00000002
#define	ON_ITEM_USE_POSSIBLE						0x00000004
#define	ON_ITEM_M_KOREA								0x00000008
#define	ON_ITEM_F_KOREA								0x00000010
#define	ON_ITEM_M_CHINA								0x00000020
#define	ON_ITEM_F_CHINA								0x00000040
#define	ON_ITEM_M_JAPAN								0x00000080
#define	ON_ITEM_F_JAPAN								0x00000100
#define ON_ITEM_LIGHT								0x00000200
#define ON_ITEM_MEDIUM								0x00000400
#define ON_ITEM_HEAVY								0x00000800
#define ON_ITEM_NO_TOOL								0x00001000
#define ON_ITEM_NO_MATERIAL							0x00002000
#define ON_ITEM_NO_MAKE								0x00004000
#define ON_ITEM_REVIVE								0x00008000
#define	ON_ITEM_M_TAIWAN							0x00010000
#define	ON_ITEM_F_TAIWAN							0x00020000
#define ON_ITEM_UPGRADE_POSSIBLE					0x00040000




#define ON_ITEM_ARMOR_CLOTH									1
#define ON_ITEM_ARMOR_METAL									2
#define ON_ITEM_ARMOR_RARE									3
#define ON_ITEM_ARMOR_WOODEN								4
#define ON_ITEM_BELT_CLOTH									5
#define ON_ITEM_BELT_METAL									6
#define ON_ITEM_BELT_RARE									7
#define ON_ITEM_GLOVE_CLOTH									8
#define ON_ITEM_GLOVE_METAL									9
#define ON_ITEM_GLOVE_RARE									10
#define ON_ITEM_GLOVE_WOODEN								11
#define ON_ITEM_HELMET_CLOTH								12				
#define ON_ITEM_HELMET_METAL								13
#define ON_ITEM_HELMET_RARE									14
#define ON_ITEM_HELMET_WOODEN								15
#define ON_ITEM_HELMET_HAT									16
#define ON_ITEM_SHOES_CLOTH									17
#define ON_ITEM_SHOES_ETC									18
#define ON_ITEM_SHOES_METAL									19
#define ON_ITEM_SHOES_WOODEN								20
#define ON_ITEM_WEAPON_DART									21
#define ON_ITEM_WEAPON_GUN									22
#define ON_ITEM_WEAPON_ARROW								23
#define ON_ITEM_WEAPON_RING									24
#define ON_ITEM_WEAPON_SWORD								25
#define ON_ITEM_WEAPON_WOODEN								26	
#define ON_ITEM_ETC											27

#define ON_FOOD_VEGETABLE									31
#define ON_FOOD_TEA											32
#define ON_FOOD_SPECIAL									    33
#define ON_FOOD_NUDDLE										34
#define ON_FOOD_MEAT										35
#define ON_FOOD_FRUITS										36
#define ON_FOOD_ETC											37



#define ON_ITEM_SWORD								0x00000001
#define ON_ITEM_ARMOR								0x00000002
#define ON_ITEM_ANIMAL								0x00000004
#define ON_ITEM_DRESS								0x00000008
#define ON_ITEM_ELEMENT								0x00000010
#define ON_ITEM_FOOD								0x00000020
#define ON_ITEM_ACCESSARY							0x00000040
#define ON_ITEM_LIQUOR								0x00000080
#define ON_ITEM_TOOL								0x00000100
#define ON_ITEM_RING								0x00000200
#define ON_ITEM_HELMAT								0x00000400
#define ON_ITEM_GLOVES								0x00000800
#define ON_ITEM_BELT								0x00001000
#define ON_ITEM_BOOTS								0x00002000
#define ON_ITEM_LRING								0x00004000
#define ON_ITEM_RRING								0x00008000
#define ON_ITEM_TRADE								0x00010000
#define ON_ITEM_TOALL								0x00020000          // 용병 전원에 적용
#define ON_ITEM_SPECIAL							    0x00040000          // 스탯 초기화
#define ON_ITEM_ALL_RECOVER							0x00080000			// 용병 전원에 적용 & %로 모든 수치를 체워 준다.
#define ON_ITEM_GUARDIAN							0x00100000

#define ON_ITEM_SPECIAL_INITIALIZE                  1
#define ON_ITEM_SPECIAL_SHOWMERCENARY               2
#define ON_ITEM_SPECIAL_CHANGESEX                   3
#define ON_ITEM_SPECIAL_CHANGENATION                4
#define ON_ITEM_SPECIAL_CREDITUP                    5
#define ON_ITEM_SPECIAL_SEALINGBEAD					6
#define ON_ITEM_SPECIAL_MAGICSTONERED				7
#define ON_ITEM_SPECIAL_MAGICSTONEBLUE				8
#define ON_ITEM_SPECIAL_MAGICSTONEBLACK				9
#define ON_ITEM_SPECIAL_TACTICS						10
#define ON_ITEM_SPECIAL_DONKEY						11
#define ON_ITEM_SPECIAL_IMPERIUM					12


#define	ON_ITEM_MISSLE_TYPE_NONE					0
#define	ON_ITEM_MISSLE_TYPE_FREE					1
#define	ON_ITEM_MISSLE_TYPE_BULLET					2
#define	ON_ITEM_MISSLE_TYPE_DART					3
#define	ON_ITEM_MISSLE_TYPE_SWORD					4
#define	ON_ITEM_MISSLE_TYPE_SEPEAR					5
#define	ON_ITEM_MISSLE_TYPE_AXE						6
#define	ON_ITEM_MISSLE_TYPE_ARROW					7
#define	ON_ITEM_MISSLE_TYPE_FAN						8
#define ON_ITEM_MISSLE_TYPE_CRAW					9
#define ON_ITEM_MISSLE_TYPE_BUDDIST					10
#define ON_ITEM_MISSLE_TYPE_DOUBLESTICK				11
#define ON_ITEM_MISSLE_TYPE_SCYTHE					12
#define ON_ITEM_MISSLE_TYPE_SPINSTONE				13
#define ON_ITEM_MISSLE_TYPE_BEADS					14
#define ON_ITEM_MISSLE_TYPE_EMPTY					15
#define ON_ITEM_MISSLE_TYPE_FIREGUN					16
#define	ON_ITEM_MISSLE_TYPE_DSWORD					17			// 쌍검
#define ON_ITEM_MISSLE_TYPE_STICK					18			// 봉
#define ON_ITEM_MISSLE_TYPE_PEARL					19			// 구슬



enum {ON_ITEM_MAGICATTACK_NONE,ON_ITEM_MAGICATTACK_KILLSHOT,ON_ITEM_MAGICATTACK_SEISMIC,ON_ITEM_MAGICATTACK_HIDDEN,ON_ITEM_MAGICATTACK_VITALSTEEL,ON_ITEM_MAGICATTACK_MOONAXE,
      ON_ITEM_MAGICATTACK_FIREBLOW,ON_ITEM_MAGICATTACK_ASTRAFIRE,ON_ITEM_MAGICATTACK_FIREARROW,ON_ITEM_MAGICATTACK_WEDGEATTACK,ON_ITEM_MAGICATTACK_BOLT, ON_ITEM_MAGICATTACK_ASTRALFIRE,
	  ON_ITEM_MAGICATTACK_HOLD,ON_ITEM_MAGICATTACK_TORNADO,ON_ITEM_MAGICATTACK_MAGICARROW,ON_ITEM_MAGICATTACK_RUNNINGFIRE,ON_ITEM_MAGICATTACK_ENERGYPA, 
		ON_ITEM_MAGICATTACK_LONG_SPEAR,ON_ITEM_MAGICATTACK_DOUBLE_SWORD,ON_ITEM_MAGICATTACK_ICE_BARRIER,ON_ITEM_MAGIC_FLOWER_NEEDLE_RAIN,
		ON_ITEM_MAGIC_SOUL_CHANGE,ON_ITEM_MAGIC_STONE_MAGIC,ON_ITEM_MAGIC_GOD_SPEAR,ON_ITEM_MAGICATTACK_MONKEYSTICK};


#define ON_ITEM_MAX_ADDMAKE							5
#define ON_ITEM_MAX_TOOL							3

#define ON_ITEM_TRADEBOOK_ID						279					// 아이템 구조체에 있는 장부 아디


#include	<windows.h>
#include	"GSCDefine.h"
#include	"Directives.h"
#include	"OnlineText.h"


#include	<iostream>
#include	<vector>
#include	<map>

using namespace std;


struct MakeItem
{	
	UI32			m_uiItemID[ ON_ITEM_MAX_ADDMAKE ];												// 새로운 아이템을 만들때 들어 가는 아이템 아이디
	SI16			m_siItemNum[ ON_ITEM_MAX_ADDMAKE ];												// 새로운 아이템을 만들때 들어 가는 아이템 수
	UI32			m_uiToolID[ ON_ITEM_MAX_TOOL ];													// 아이템을 만들때 들어가는 아이템 툴 아이디
	UI32			m_siLabor;																		// 노동력
	SI32			m_siProduction;																	// 생산량
	SI32			m_siMaxProduction;																// 최대 생산량
};

struct WeaponHeader
{
	SI16			siMinDam;																		// 최소 공격력
	SI16			siMaxDam;																		// 최대 공격력
	SI16			siAttackRange;																	// 공격 범위
	SI16			siWeaponType;																	// 무기 타입	
	SI16			siAttackAtb;																	// 공격 속성(예: 단검은 ATTACKATB_SWORD로 1)
	UI08			uiMagic;																		// 마법 공격 타입
	SI16			siMagicDesc;																	// 마법 설명
	SI16			siMagicAttack;																	// 마법 공격력
	UI08			uiGrade;																		// 고급무기 +등급
};

#define ON_GUARDIAN_RECALL_NUM		2

struct Guardian
{
	SI08	m_siKind;									// 수호부 종류
	SI08	m_siStartCondition;							// 발생조건(1 : 공격할때, 2 : 즉시)
	SI08	m_siSkillNum;								// 기술개수

	SI08	m_siRecallNum[ON_GUARDIAN_RECALL_NUM];		// 소환 마리수
	UI16	m_uiRecallKind[ON_GUARDIAN_RECALL_NUM];		// 소환캐릭 종류
	SI08	m_siNHitrate[ON_GUARDIAN_RECALL_NUM];		// 적중확률(일반)
	SI08	m_siBHitrate[ON_GUARDIAN_RECALL_NUM];		// 적중확률(보스)
	SI08	m_siSBHitrate[ON_GUARDIAN_RECALL_NUM];		// 적중확률(왕보스)
	SI08    m_siNoHaltSBHitrate[ON_GUARDIAN_RECALL_NUM];// 적중확률(노포박 보스)
	SI08	m_siAliveTime[ON_GUARDIAN_RECALL_NUM];		// 소멸조건(0 : 즉시, 1000 : 무한)
};

// robypark 2004/11/10 14:14
// 공성전 소집권 아이템 사용하여 변신하는 캐릭터에 대한 옵션 상수 선언
#define ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE				1	// 명시된 캐릭터로 변신 가능
#define ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL		2	// 명시된 캐릭터로 된 대장 유닛으로 변신 불가능(대방 및 행수 사용 불가능)
#define ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE			4	// 이 공성전 유닛은 마을 수성인터페이스를 이용할 수 있다.

struct Imperium
{	
	SI08	m_siID;										// 소집권 종류.
	UI16	m_uiKind;									// 변하는 캐릭터 아이디.
	SI16	m_siHp;										// 기본으로 주어지는 캐릭터 체력(max와 동일).
	SI16	m_siSupply;									// 기본으로 주어지는 보급량(max와 동일).
	SI16	m_siPopulation;								// 소집권 구매시 사용되는 인구수.
	SI16	m_siPrice;									// 인구수 1명당 소집권 만드는데 필요한 비용.
														// 실제 소집권 가격은 m_siPopulation * m_siPrice 이다.
	SI08	m_scTransformOption;						// robypark 2004/11/10 14:13 - 해당 소집권에 명시된 캐릭터로 변신 가능에 대한 옵션으로 OR(|) 연산된 값이다.
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE(1)				// 명시된 캐릭터로 변신 가능
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL(2)	// 명시된 캐릭터로 된 대장 유닛으로 변신 불가능(대방 및 행수 사용 불가능)
														// ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE(4)		// 이 공성전 유닛은 마을 수성인터페이스를 이용할 수 있다.
/*
	// robypark 2005/1/24 17:44
	SI08	m_scTransformLevelLimit;					// 해당 공성전 유닛으로 변신하기 위한 레벨 제한
	SI32	m_siTransformCost;							// 해당 공성전 유닛으로 변신하기 위한 금액

	// robypark 2005/1/25 17:39
	SI32	m_siArmyKindDescTextID;						// 해당 공성전 유닛 설명 텍스트 ID

	// robypark 2005/1/27 11:1
	char	m_szAnimationResourceID_Blue[64];			// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	char	m_szAnimationResourceID_Red[64];			// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때
*/
};

//아이템 해더 
struct CItemHeader
{
	UI32			m_uiID;					// 아이템 ID	
	UI08			m_uiWeight;				// 아이템의 무게
	MONEY			m_uiValue;				// 아이템의 가격	
	SI32			m_Attribute;			// 아이템 속성	
	SI16			m_siAttack;				// 아이템 공격력
	SI16			m_siDefense;			// 아이템의 방어력
	SI16			m_siStrength;			// 힘
	SI16			m_siDex;				// 민첩
	SI16			m_siVitality;			// 생명
	SI16			m_siInt;				// 지력
	SI16			m_siHpMax;
	SI16			m_siHpGain;
	SI16			m_siMpMax;
	SI16			m_siMpGain;
	SI16			m_siHealth;				// 건강
	SI16			m_siHealthMax;			// 사용 되지 않음
	SI16			m_siSpecialParam;		// 명성	
	SI32			m_siItemName;			// 아이템 이름
	SI32			m_siItemInfo;			// 아이템 설명
	SI16			m_siModule;				// 아이템 무게 단위
	SI32			m_siGroupCode;			// 그룹 코드 (Text)	
	SI16			m_siDamageRegist;		// 타격 저항
	SI16			m_siMagicRegist;		// 마법 저항
	SI16			m_siNation;				// 아이템이 생산 되는 국가
	SI16			m_siDrawID;				// 그림의 ID
	SI16			m_siReqStr;				// 착용 가능한 힘
	SI16			m_siReqDex;				// 착용 가능한 민첩
	SI16			m_siReqLv;				// 착용 가능한 레벨
	SI32			m_siDBPrice;			// 데이타베이스에서 얻어오는 값.
	SI32			m_siStdEvp;				// 표준편차
	UI08			m_uiCostChange;			// 
	SI16			m_siSound;				// 무기 소리
	UI08			m_uiTradeGrade;			// 필요 신용 등급
	//SI16			m_siEncorgAtt;			// 용병 공격력 상승
	//SI16			m_siEncorgDep;			// 용병 방어력 상승
	SI16			m_siMedic;
	BOOL			m_bTimeLimit;			// 시간제한 1 시간없음 0
	SI16			m_siDay;				// 몇일일까.

	SI32            m_siSpecial;			// 아이템의 특수효과~


	SI16			m_siSpriteFile;			// 아이템 파일
	SI16			m_siSm_SpriteFile;		// 아이템 파일 작은 사이즈
	SI16			m_siSpriteFont;			// 아이템 이미지 폰트
	SI16			m_siSm_SpriteFont;		// 아이템 이지지 폰트 작은 사이즈

	CHAR			m_szSpriteFile[16];		// 아이템 파일 이름
	CHAR			m_szSm_SpriteFile[16];	// 아이템 파일 이름(작은거)

	WeaponHeader	*pWeaponHeader;			// 다른 아이템에 대한 포인터
	MakeItem		*pMakeItem;				// 만들때 사용 하는 아이템 정보
	Guardian		*m_pGuardian;			// 수호부
	Imperium		*m_pImperium;			// 소집권.
};


struct SearchByID
{
	UI32			m_uiKey;																		// 키 값
	CItemHeader		*m_pItemHeader;																	// 아이템 헤더의 포인터
};


class CItem
{
public:	
	vector<UI32>		m_IDBuffer;

protected:
	//몇개의 아이템이 저장 돼어 있는가?		
	
	
	CItemHeader*		m_pItemHead;	
	SearchByID			*m_pSearchByID;																			// 아이템을 찾기 위한 구조체

	OnlineText			*pOnlineText;
	SI16				m_siWeaponNum, m_siArmorNum, m_siBeltNum, m_siBootsNum, m_siGlovesNum, m_siHelmatNum, m_siRing, m_siDress,m_siGuardian;

	vector<CItemHeader>	_Weapon;
	vector<CItemHeader>	_Items;

	map<UI32, CItemHeader*, less<UI32> > _Item;

public:	

	CItem();
	virtual ~CItem();	

	virtual	void		Load(OnlineText *Text, char *pFile);
	void				AddItem(CItemHeader &Item);
	
	virtual BOOL		ItemLoad(	OnlineText *ponlinetext, char *pWeaponFile, char *pArmorFile, char *pAccessaryFile, 
									char *pAnimal, char *pDress, char *pElement, char *pFood, char *pLiquor,
									char *pMedicine, char *pTool, char *pTrade, char *pGuardian,char* pImperium);

	void				ReMoveAll();																			// 모든 아이템을 리스트에서 지운다.
	SI32				GetCount()					{ return _Item.size(); }									// 총 아이템 수
	VOID				SetFile(SI32 siFile, CHAR *pFileName, SI16 siXsize, SI16 siYsize, BOOL bSm);			// 리소스에 로드 되어 있는 아이템의 파일의 인덱스를 셋팅 해준다.
	VOID				GetSpriteFileName( SI32 siIndex, CHAR *pFileName, CHAR *SmallFileName );				// 헤더에서 아이템의 이름을 리턴 받는다.
	UI08				GetWearLoc( UI32 uiID );																// 아이템이 착용할수 있는 위치를 리턴함둥	
	VOID				GetStdEvp();																			// 아이템의 표준편차를 구한다.
	CItemHeader*		GetItem( UINT nID );																	// 아이템을 얻는다.
	CItemHeader*		GetItem( CHAR *pszItemName );															// 아이템을 얻는다.
	CItemHeader*		GetItemInfo( UI32 uiItemID, SI16 &siEquip );
	CItemHeader*		GetItemHeader( UI32 index );															// 저장된 순서(index)에 의해 아이템을 얻는다.
	BOOL				IsUpgradable( UI32 uiItemID );															// 업그레이드 가능한 무기인가?
	SI16				GetSoundID(SI16 siGroupCode,char *pToken);
	BOOL				IsTimeLimitItem(UI32 uiItemID);
	SI16				GetItemUsingDay(UI32 uiItemID);


	SI16				GetWeaponNum()				{ return _Weapon.size();	}
	SI16				GetArmorNum()				{ return m_siArmorNum;		}	
	SI16				GetBootsNum()				{ return m_siBootsNum;		}
	SI16				GetGlovesNum()				{ return m_siGlovesNum;		}
	SI16				GetBeltNum()				{ return m_siBeltNum;		}
	SI16				GetHelmatNum()				{ return m_siHelmatNum;		}
	SI16				GetRingNum()				{ return m_siRing;			}
	SI16				GetDressNum()				{ return m_siDress;			}
	SI16				GetGuardianNum()			{ return m_siGuardian;		}

protected:
	virtual BOOL		LoadItem( char *pItemFile, vector<CItemHeader> *v );			// 무기를 제외한 아이템을 로드함
	virtual BOOL		LoadWeapon( char *pWeaponFile );								// 무기 아이템을 로드함
	virtual BOOL		LoadGuardian( char *pItemFile, vector<CItemHeader> *v );		// 수호부.
	virtual BOOL		LoadImperium( char *pItemFile, vector<CItemHeader> *v );		// 소집권 아이템.
	UI16				GetID( char *pToken );											// 소환몬스터 아이디
	SI32				GetGroup( char *pToken , SI32 *psiSpecial);						// 아이템의 종류를 리턴함
	SI32				GetNation( char *pToken );										// 착용가능한 나라를 리턴
	SI16				GetGroup( WeaponHeader *Weapon, char *pToken );					// 아이템의 종류를 리턴함

	void				AddContainer( vector<CItemHeader> *v );
	void				Remove( vector<CItemHeader> *v );

public:
	//------------------------------------------------------------
	// robypark 2004/11/9 21:54
	//함수명 : GetItemHeaderFromImperium
	//파라미터: SI32 siKindOnlineId: 서버에서 사용되는 온라인 캐릭터 ID
	//설  명 : 공성전을 위한 함수로 변신되는 캐릭터(Online) ID를
	//		   이용하여 변신 아이템정보 헤더를 찾는다.
	//------------------------------------------------------------
	CItemHeader* GetItemHeaderFromImperium( SI32 siKindOnlineID );

	//------------------------------------------------------------
	// robypark 2004/12/13 13:39
	//함수명 : GetItemHeaderFromImperiumByIndex
	//파라미터: SI32 siImperiumIndex: 소집권 아이템의 인덱스
	//설  명 : 공성전을 위한 함수로 변신되는 소집권 아이템의 인덱스
	//			를 이용하여 아이템 정보 헤더를 찾는다.
	//------------------------------------------------------------
	CItemHeader* GetItemHeaderFromImperiumByIndex( SI32 siImperiumIndex );
};

#endif