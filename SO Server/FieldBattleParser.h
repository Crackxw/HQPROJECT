#ifndef	FIELD_BATTLE_PARSER
#define FIELD_BATTLE_PARSER

#include <windows.h>
#include "../SCCommon/GSCDefine.h"

#define ON_VILLAGE_ATTACK_OIL	1
#define ON_VILLAGE_ATTACK_STONE	2

#define FIELD_BATTLE_SWORD		0		// 검병
#define FIELD_BATTLE_SPEAR		1		// 창병
#define FIELD_BATTLE_HORSE		2		// 기마병
#define FIELD_BATTLE_ARROW		3		// 궁병
#define FIELD_BATTLE_DEFENSE	4		// 수비병
#define FIELD_BATTLE_RAMPART	5		// 충차
#define FIELD_BATTLE_SIEGE		6		// 발석거
#define FIELD_BATTLE_SUPPLY		7		// 보급수레.
#define FIELD_BATTLE_CASTLE		8       // 성벽.

#define FIELD_BATTLE_ATTACK_UNIT_NUM	8
#define FIELD_BATTLE_DEFENSE_UNIT_NUM	9

struct	strFieldBattlePercent
{
	short	siPercent;
};

struct	strFieldBattleUnit
{
	short				siBaseDamage;	//유닛이 사용할수 있는 기본데미지.
	unsigned short		uiKind;			//유닛의 KindID
	BOOL				bBattleUnit;	//유닛이 조정할수 있는 공성유닛인가 수비병 유닛인가.
	unsigned short		uiItemID;		//이 이 유닛을 만들기 위한 ItemID
	strFieldBattlePercent stFBPer[FIELD_BATTLE_DEFENSE_UNIT_NUM];
};


class CFieldBattleParser
{
public:
	strFieldBattleUnit	m_stFBUnit[FIELD_BATTLE_ATTACK_UNIT_NUM];

	CFieldBattleParser();
	~CFieldBattleParser();

	void Init();
	void Load(char* szFileName);
	short GetCalcDamage(short siAttackUnit,short siDefenseUnit,short siAttackType);
	//short GetCalcDamageByVillage(short siDefenceSoldier,short siDefenseUnit,short siAttackType);
	BOOL  IsFieldAttackUnit(unsigned short uiKind);
	short GetUnitKind(unsigned short uiAUnitKind);
	unsigned short GetItemID(short slotID);
	short GetCalcDamageByDefenceSoldier(short siDefenceSoldier,short siDefenseUnit,short siAttackType);
};

#define ATTACK_POWER_DAMAGE_NUM			7

struct strAttackPowerDamage
{
	int			siMinAttackPower;
	int			siMaxAttackPower;
	short		siAttackPercent;			//견고한 공격.
	short		siDefencePercent;			//견고한 방어.
};

class CAttackPowerParser
{
public:
	strAttackPowerDamage	m_stAttackPowerDamage[ATTACK_POWER_DAMAGE_NUM];

	CAttackPowerParser();
	~CAttackPowerParser();

	void	Load(char* szFileName);
	short	GetAttackPercent(int siAttackPower);
	short	GetDefencePercent(int siAttackPower);
};

#endif
