/************************************************************************************
���ϸ�	:	OnLineItem.h
�ۼ���	:	�� �� ��
�����  :	�� �� ��
�ۼ���  :	2001.04.20
������  :	2002.02.16
************************************************************************************/

#ifndef	_ONLINE_ITEM_H
#define _ONLINE_ITEM_H

#pragma warning ( disable : 4786 )

#define ON_STRING_ITEM_NAME							32
#define ON_STRING_ITEM_INFO							256
#define ON_STRING_ITEM_FILE							50

//������ �Ӽ�
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
#define ON_ITEM_TOALL								0x00020000          // �뺴 ������ ����
#define ON_ITEM_SPECIAL							    0x00040000          // ���� �ʱ�ȭ
#define ON_ITEM_ALL_RECOVER							0x00080000			// �뺴 ������ ���� & %�� ��� ��ġ�� ü�� �ش�.
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
#define	ON_ITEM_MISSLE_TYPE_DSWORD					17			// �ְ�
#define ON_ITEM_MISSLE_TYPE_STICK					18			// ��
#define ON_ITEM_MISSLE_TYPE_PEARL					19			// ����



enum {ON_ITEM_MAGICATTACK_NONE,ON_ITEM_MAGICATTACK_KILLSHOT,ON_ITEM_MAGICATTACK_SEISMIC,ON_ITEM_MAGICATTACK_HIDDEN,ON_ITEM_MAGICATTACK_VITALSTEEL,ON_ITEM_MAGICATTACK_MOONAXE,
      ON_ITEM_MAGICATTACK_FIREBLOW,ON_ITEM_MAGICATTACK_ASTRAFIRE,ON_ITEM_MAGICATTACK_FIREARROW,ON_ITEM_MAGICATTACK_WEDGEATTACK,ON_ITEM_MAGICATTACK_BOLT, ON_ITEM_MAGICATTACK_ASTRALFIRE,
	  ON_ITEM_MAGICATTACK_HOLD,ON_ITEM_MAGICATTACK_TORNADO,ON_ITEM_MAGICATTACK_MAGICARROW,ON_ITEM_MAGICATTACK_RUNNINGFIRE,ON_ITEM_MAGICATTACK_ENERGYPA, 
		ON_ITEM_MAGICATTACK_LONG_SPEAR,ON_ITEM_MAGICATTACK_DOUBLE_SWORD,ON_ITEM_MAGICATTACK_ICE_BARRIER,ON_ITEM_MAGIC_FLOWER_NEEDLE_RAIN,
		ON_ITEM_MAGIC_SOUL_CHANGE,ON_ITEM_MAGIC_STONE_MAGIC,ON_ITEM_MAGIC_GOD_SPEAR,ON_ITEM_MAGICATTACK_MONKEYSTICK};


#define ON_ITEM_MAX_ADDMAKE							5
#define ON_ITEM_MAX_TOOL							3

#define ON_ITEM_TRADEBOOK_ID						279					// ������ ����ü�� �ִ� ��� �Ƶ�


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
	UI32			m_uiItemID[ ON_ITEM_MAX_ADDMAKE ];												// ���ο� �������� ���鶧 ��� ���� ������ ���̵�
	SI16			m_siItemNum[ ON_ITEM_MAX_ADDMAKE ];												// ���ο� �������� ���鶧 ��� ���� ������ ��
	UI32			m_uiToolID[ ON_ITEM_MAX_TOOL ];													// �������� ���鶧 ���� ������ �� ���̵�
	UI32			m_siLabor;																		// �뵿��
	SI32			m_siProduction;																	// ���귮
	SI32			m_siMaxProduction;																// �ִ� ���귮
};

struct WeaponHeader
{
	SI16			siMinDam;																		// �ּ� ���ݷ�
	SI16			siMaxDam;																		// �ִ� ���ݷ�
	SI16			siAttackRange;																	// ���� ����
	SI16			siWeaponType;																	// ���� Ÿ��	
	SI16			siAttackAtb;																	// ���� �Ӽ�(��: �ܰ��� ATTACKATB_SWORD�� 1)
	UI08			uiMagic;																		// ���� ���� Ÿ��
	SI16			siMagicDesc;																	// ���� ����
	SI16			siMagicAttack;																	// ���� ���ݷ�
	UI08			uiGrade;																		// ��޹��� +���
};

#define ON_GUARDIAN_RECALL_NUM		2

struct Guardian
{
	SI08	m_siKind;									// ��ȣ�� ����
	SI08	m_siStartCondition;							// �߻�����(1 : �����Ҷ�, 2 : ���)
	SI08	m_siSkillNum;								// �������

	SI08	m_siRecallNum[ON_GUARDIAN_RECALL_NUM];		// ��ȯ ������
	UI16	m_uiRecallKind[ON_GUARDIAN_RECALL_NUM];		// ��ȯĳ�� ����
	SI08	m_siNHitrate[ON_GUARDIAN_RECALL_NUM];		// ����Ȯ��(�Ϲ�)
	SI08	m_siBHitrate[ON_GUARDIAN_RECALL_NUM];		// ����Ȯ��(����)
	SI08	m_siSBHitrate[ON_GUARDIAN_RECALL_NUM];		// ����Ȯ��(�պ���)
	SI08    m_siNoHaltSBHitrate[ON_GUARDIAN_RECALL_NUM];// ����Ȯ��(������ ����)
	SI08	m_siAliveTime[ON_GUARDIAN_RECALL_NUM];		// �Ҹ�����(0 : ���, 1000 : ����)
};

// robypark 2004/11/10 14:14
// ������ ������ ������ ����Ͽ� �����ϴ� ĳ���Ϳ� ���� �ɼ� ��� ����
#define ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE				1	// ��õ� ĳ���ͷ� ���� ����
#define ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL		2	// ��õ� ĳ���ͷ� �� ���� �������� ���� �Ұ���(��� �� ��� ��� �Ұ���)
#define ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE			4	// �� ������ ������ ���� �����������̽��� �̿��� �� �ִ�.

struct Imperium
{	
	SI08	m_siID;										// ������ ����.
	UI16	m_uiKind;									// ���ϴ� ĳ���� ���̵�.
	SI16	m_siHp;										// �⺻���� �־����� ĳ���� ü��(max�� ����).
	SI16	m_siSupply;									// �⺻���� �־����� ���޷�(max�� ����).
	SI16	m_siPopulation;								// ������ ���Ž� ���Ǵ� �α���.
	SI16	m_siPrice;									// �α��� 1��� ������ ����µ� �ʿ��� ���.
														// ���� ������ ������ m_siPopulation * m_siPrice �̴�.
	SI08	m_scTransformOption;						// robypark 2004/11/10 14:13 - �ش� �����ǿ� ��õ� ĳ���ͷ� ���� ���ɿ� ���� �ɼ����� OR(|) ����� ���̴�.
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_ENABLE(1)				// ��õ� ĳ���ͷ� ���� ����
														// ON_ITEM_SIEGEWARFARE_TRANSFORM_DISABLE_GENERAL(2)	// ��õ� ĳ���ͷ� �� ���� �������� ���� �Ұ���(��� �� ��� ��� �Ұ���)
														// ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE(4)		// �� ������ ������ ���� �����������̽��� �̿��� �� �ִ�.
/*
	// robypark 2005/1/24 17:44
	SI08	m_scTransformLevelLimit;					// �ش� ������ �������� �����ϱ� ���� ���� ����
	SI32	m_siTransformCost;							// �ش� ������ �������� �����ϱ� ���� �ݾ�

	// robypark 2005/1/25 17:39
	SI32	m_siArmyKindDescTextID;						// �ش� ������ ���� ���� �ؽ�Ʈ ID

	// robypark 2005/1/27 11:1
	char	m_szAnimationResourceID_Blue[64];			// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	char	m_szAnimationResourceID_Red[64];			// �ش� ������ ���� �̹��� ����. ���� ������ ��
*/
};

//������ �ش� 
struct CItemHeader
{
	UI32			m_uiID;					// ������ ID	
	UI08			m_uiWeight;				// �������� ����
	MONEY			m_uiValue;				// �������� ����	
	SI32			m_Attribute;			// ������ �Ӽ�	
	SI16			m_siAttack;				// ������ ���ݷ�
	SI16			m_siDefense;			// �������� ����
	SI16			m_siStrength;			// ��
	SI16			m_siDex;				// ��ø
	SI16			m_siVitality;			// ����
	SI16			m_siInt;				// ����
	SI16			m_siHpMax;
	SI16			m_siHpGain;
	SI16			m_siMpMax;
	SI16			m_siMpGain;
	SI16			m_siHealth;				// �ǰ�
	SI16			m_siHealthMax;			// ��� ���� ����
	SI16			m_siSpecialParam;		// ��	
	SI32			m_siItemName;			// ������ �̸�
	SI32			m_siItemInfo;			// ������ ����
	SI16			m_siModule;				// ������ ���� ����
	SI32			m_siGroupCode;			// �׷� �ڵ� (Text)	
	SI16			m_siDamageRegist;		// Ÿ�� ����
	SI16			m_siMagicRegist;		// ���� ����
	SI16			m_siNation;				// �������� ���� �Ǵ� ����
	SI16			m_siDrawID;				// �׸��� ID
	SI16			m_siReqStr;				// ���� ������ ��
	SI16			m_siReqDex;				// ���� ������ ��ø
	SI16			m_siReqLv;				// ���� ������ ����
	SI32			m_siDBPrice;			// ����Ÿ���̽����� ������ ��.
	SI32			m_siStdEvp;				// ǥ������
	UI08			m_uiCostChange;			// 
	SI16			m_siSound;				// ���� �Ҹ�
	UI08			m_uiTradeGrade;			// �ʿ� �ſ� ���
	//SI16			m_siEncorgAtt;			// �뺴 ���ݷ� ���
	//SI16			m_siEncorgDep;			// �뺴 ���� ���
	SI16			m_siMedic;
	BOOL			m_bTimeLimit;			// �ð����� 1 �ð����� 0
	SI16			m_siDay;				// �����ϱ�.

	SI32            m_siSpecial;			// �������� Ư��ȿ��~


	SI16			m_siSpriteFile;			// ������ ����
	SI16			m_siSm_SpriteFile;		// ������ ���� ���� ������
	SI16			m_siSpriteFont;			// ������ �̹��� ��Ʈ
	SI16			m_siSm_SpriteFont;		// ������ ������ ��Ʈ ���� ������

	CHAR			m_szSpriteFile[16];		// ������ ���� �̸�
	CHAR			m_szSm_SpriteFile[16];	// ������ ���� �̸�(������)

	WeaponHeader	*pWeaponHeader;			// �ٸ� �����ۿ� ���� ������
	MakeItem		*pMakeItem;				// ���鶧 ��� �ϴ� ������ ����
	Guardian		*m_pGuardian;			// ��ȣ��
	Imperium		*m_pImperium;			// ������.
};


struct SearchByID
{
	UI32			m_uiKey;																		// Ű ��
	CItemHeader		*m_pItemHeader;																	// ������ ����� ������
};


class CItem
{
public:	
	vector<UI32>		m_IDBuffer;

protected:
	//��� �������� ���� �ž� �ִ°�?		
	
	
	CItemHeader*		m_pItemHead;	
	SearchByID			*m_pSearchByID;																			// �������� ã�� ���� ����ü

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

	void				ReMoveAll();																			// ��� �������� ����Ʈ���� �����.
	SI32				GetCount()					{ return _Item.size(); }									// �� ������ ��
	VOID				SetFile(SI32 siFile, CHAR *pFileName, SI16 siXsize, SI16 siYsize, BOOL bSm);			// ���ҽ��� �ε� �Ǿ� �ִ� �������� ������ �ε����� ���� ���ش�.
	VOID				GetSpriteFileName( SI32 siIndex, CHAR *pFileName, CHAR *SmallFileName );				// ������� �������� �̸��� ���� �޴´�.
	UI08				GetWearLoc( UI32 uiID );																// �������� �����Ҽ� �ִ� ��ġ�� �����Ե�	
	VOID				GetStdEvp();																			// �������� ǥ�������� ���Ѵ�.
	CItemHeader*		GetItem( UINT nID );																	// �������� ��´�.
	CItemHeader*		GetItem( CHAR *pszItemName );															// �������� ��´�.
	CItemHeader*		GetItemInfo( UI32 uiItemID, SI16 &siEquip );
	CItemHeader*		GetItemHeader( UI32 index );															// ����� ����(index)�� ���� �������� ��´�.
	BOOL				IsUpgradable( UI32 uiItemID );															// ���׷��̵� ������ �����ΰ�?
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
	virtual BOOL		LoadItem( char *pItemFile, vector<CItemHeader> *v );			// ���⸦ ������ �������� �ε���
	virtual BOOL		LoadWeapon( char *pWeaponFile );								// ���� �������� �ε���
	virtual BOOL		LoadGuardian( char *pItemFile, vector<CItemHeader> *v );		// ��ȣ��.
	virtual BOOL		LoadImperium( char *pItemFile, vector<CItemHeader> *v );		// ������ ������.
	UI16				GetID( char *pToken );											// ��ȯ���� ���̵�
	SI32				GetGroup( char *pToken , SI32 *psiSpecial);						// �������� ������ ������
	SI32				GetNation( char *pToken );										// ���밡���� ���� ����
	SI16				GetGroup( WeaponHeader *Weapon, char *pToken );					// �������� ������ ������

	void				AddContainer( vector<CItemHeader> *v );
	void				Remove( vector<CItemHeader> *v );

public:
	//------------------------------------------------------------
	// robypark 2004/11/9 21:54
	//�Լ��� : GetItemHeaderFromImperium
	//�Ķ����: SI32 siKindOnlineId: �������� ���Ǵ� �¶��� ĳ���� ID
	//��  �� : �������� ���� �Լ��� ���ŵǴ� ĳ����(Online) ID��
	//		   �̿��Ͽ� ���� ���������� ����� ã�´�.
	//------------------------------------------------------------
	CItemHeader* GetItemHeaderFromImperium( SI32 siKindOnlineID );

	//------------------------------------------------------------
	// robypark 2004/12/13 13:39
	//�Լ��� : GetItemHeaderFromImperiumByIndex
	//�Ķ����: SI32 siImperiumIndex: ������ �������� �ε���
	//��  �� : �������� ���� �Լ��� ���ŵǴ� ������ �������� �ε���
	//			�� �̿��Ͽ� ������ ���� ����� ã�´�.
	//------------------------------------------------------------
	CItemHeader* GetItemHeaderFromImperiumByIndex( SI32 siImperiumIndex );
};

#endif