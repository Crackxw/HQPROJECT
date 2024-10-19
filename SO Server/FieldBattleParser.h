#ifndef	FIELD_BATTLE_PARSER
#define FIELD_BATTLE_PARSER

#include <windows.h>
#include "../SCCommon/GSCDefine.h"

#define ON_VILLAGE_ATTACK_OIL	1
#define ON_VILLAGE_ATTACK_STONE	2

#define FIELD_BATTLE_SWORD		0		// �˺�
#define FIELD_BATTLE_SPEAR		1		// â��
#define FIELD_BATTLE_HORSE		2		// �⸶��
#define FIELD_BATTLE_ARROW		3		// �ú�
#define FIELD_BATTLE_DEFENSE	4		// ����
#define FIELD_BATTLE_RAMPART	5		// ����
#define FIELD_BATTLE_SIEGE		6		// �߼���
#define FIELD_BATTLE_SUPPLY		7		// ���޼���.
#define FIELD_BATTLE_CASTLE		8       // ����.

#define FIELD_BATTLE_ATTACK_UNIT_NUM	8
#define FIELD_BATTLE_DEFENSE_UNIT_NUM	9

struct	strFieldBattlePercent
{
	short	siPercent;
};

struct	strFieldBattleUnit
{
	short				siBaseDamage;	//������ ����Ҽ� �ִ� �⺻������.
	unsigned short		uiKind;			//������ KindID
	BOOL				bBattleUnit;	//������ �����Ҽ� �ִ� ���������ΰ� ���� �����ΰ�.
	unsigned short		uiItemID;		//�� �� ������ ����� ���� ItemID
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
	short		siAttackPercent;			//�߰��� ����.
	short		siDefencePercent;			//�߰��� ���.
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
