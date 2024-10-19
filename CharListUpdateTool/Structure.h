

#ifndef __STRUCTURE_H_INCLUDED__
#define __STRUCTURE_H_INCLUDED__

#include "..\\XCommon\\jxdef.h"


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


#define ON_ATTRI_PLAYER				1
#define ON_ATTRI_MONSTER			2
#define ON_ATTRI_MERCENARY 			4


struct _ImportantParameta
{
	SI32	siSeed;
	SI32	siCheckSum;
	SI32	siCurExp;
	SI32	siLevel;
	SI16	siStr;
	SI16	siDex;
	SI16	siVit;
	SI16	siInt;
	SI16	siAC;
	SI16	siBonus;
	SI32	siLife;
	SI32	siMana;
	SI16	DamageResist;
	SI16	MagicResist;

	SI16	EquipMinDamage;
	SI16	EquipMaxDamage;
};

struct	CharHead
{
	char				szCharName[40];				// �ɸ��� �̸�
	UI16				uiID;						// �ɸ��� ���̵�
	UI32				uiCharName;					// �ɸ��� �̸�
	SI16				siAttri;
	UI16				uiMyExp;					// ��� ���� ����
	SI16				siNation;					// ����
	SI16				siMoveSpeed;				// �̵� �ӵ�
	SI32				siManaMax;	
	SI32				siCost;
	SI16				siLv;
	SI16				siCon;
	SI16				siNeedLv;					// �뺴�� ��� �ִ� ��
	UI08				uiWeaponType;				// ��� �Ҽ� �ִ� ����
	SI16				siNeedCredit;				// �뺴�� �����Ҽ� �ִ� �ſ뵵
	SI16				siItemDrop[4];				// ���� ���� ������
	SI32				siDropPercent[4];			// ���� ���� Ȯ��
	SI32				siDescText;
	SI16				siGeneral;					// �Ϲݿ뺴(0), ���(1), ����(2)
	SI08				siMonsterType;				// �Ϲݸ���(1), ������(2)

	_ImportantParameta	Parameta;	

	const bool CharHead::operator < (const CharHead &lhs) const { return lhs.uiID < this->uiID ? true : false; };
	const bool CharHead::operator ==(const CharHead &lhs) const { return lhs.uiID == this->uiID ? true : false; };
};


typedef struct _DBINFO
{
	char szDBName[32];
	char szDBConnString[256];
}DBINFO;

#endif