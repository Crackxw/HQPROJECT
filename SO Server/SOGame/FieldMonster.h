#ifndef	_FIELD_MONSTER_HEADER
#define _FIELD_MONSTER_HEADER

#define	MAX_FIELD_MONSTER_KIND_PER_GROUP	7

#include <Directives.h>
#include <Windows.h>
#include "OnlineCGSHeader.h"

class	IMonster;
class	ISOMap;

struct	stFieldMonsterInfo
{
	UI16	uiKind;
	UI16	uiRate;
	SI32	siLevel;
};

class	cltFieldMonster
{
private:
	static				IMonster		*pIMonster;
	static				ISOMap			*pIMap;

	SI32				siID;
	UI16				uiMapID;
	SI32				siBaseX, siBaseY;
	SI32				siRadius;
	SI32				siMaxMonsterKindPerGroup;	
	SI32				siMaxMonsterKindNum;
	SI32				siMinMonsterNumPerGroup;
	SI32				siMaxMonsterNumPerGroup;
	SI32				siMaxMonsterNum;
	SI32				siMinMonsterNum;	
	DWORD				dwPrevResurrectTime;	

	stFieldMonsterInfo	FieldMonsterInfo[MAX_FIELD_MONSTER_KIND_PER_GROUP];
	UI08				uiSourceMonsterKindIndexTable[MAX_FIELD_MONSTER_KIND_PER_GROUP];	

	RECT				rcActivityArea;						// ���͵��� Ȱ�� ����.
private:
	UI16				GetWillCreateMonsters(UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);	// ���� ������ Kind�� ���´�.
	BOOL				CreateMonster();																									// ���͸� �����Ѵ�.(�Ѹ���)
public:	
	SI32				siCurrentFieldMonsterNum;					// ���� �ʵ忡 �ִ� ���� ��.
	cltFieldMonster();		// ������.
	
	VOID	Init(SI32 siid, UI16 uimapid, SI32 six, SI32 siy, SI32 siradius, SI32 simaxmonsterkindpergroup, SI32 sigroupminquantity, SI32 sigroupmaxquantity, SI32 simaxmonsternum, SI32 siminmonsternum, stFieldMonsterInfo pFieldMonsterInfo[MAX_FIELD_MONSTER_KIND_PER_GROUP]);	// �ʱ�ȭ.
	VOID	Action();			// �׼�.
	VOID	DeadMonster();		// ���Ͱ� �׾���.
	SI32	GetID();			// ID�� ���´�.

	VOID	StartBattle();				// ������ �����Ѵ�.
	VOID	EndBattle();				// ������ �����Ѵ�.
	VOID	GetInfo(CHAR *pszBuffer);	// ������ ���´�.
	
	static	VOID	InitStaticVariable(IMonster *pimonster, ISOMap *pimap);		// ���� ������ �ʱ�ȭ �Ѵ�.	
	inline SI32 GetMinMonsterNum() {return siMinMonsterNum;};
	inline IMonster *GetMonsterClass(){return pIMonster;};

};

#endif
