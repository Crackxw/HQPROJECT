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

	RECT				rcActivityArea;						// 몬스터들의 활동 영역.
private:
	UI16				GetWillCreateMonsters(UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);	// 만들 몬스터의 Kind를 얻어온다.
	BOOL				CreateMonster();																									// 몬스터를 생성한다.(한마리)
public:	
	SI32				siCurrentFieldMonsterNum;					// 현재 필드에 있는 몬스터 수.
	cltFieldMonster();		// 생성자.
	
	VOID	Init(SI32 siid, UI16 uimapid, SI32 six, SI32 siy, SI32 siradius, SI32 simaxmonsterkindpergroup, SI32 sigroupminquantity, SI32 sigroupmaxquantity, SI32 simaxmonsternum, SI32 siminmonsternum, stFieldMonsterInfo pFieldMonsterInfo[MAX_FIELD_MONSTER_KIND_PER_GROUP]);	// 초기화.
	VOID	Action();			// 액션.
	VOID	DeadMonster();		// 몬스터가 죽었다.
	SI32	GetID();			// ID를 얻어온다.

	VOID	StartBattle();				// 전투를 시작한다.
	VOID	EndBattle();				// 전투를 종료한다.
	VOID	GetInfo(CHAR *pszBuffer);	// 정보를 얻어온다.
	
	static	VOID	InitStaticVariable(IMonster *pimonster, ISOMap *pimap);		// 정적 변수를 초기화 한다.	
	inline SI32 GetMinMonsterNum() {return siMinMonsterNum;};
	inline IMonster *GetMonsterClass(){return pIMonster;};

};

#endif
