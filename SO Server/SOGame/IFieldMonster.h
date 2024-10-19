#ifndef	_IFIELD_MONSTER_HEADER
#define _IFIELD_MONSTER_HEADER

#include <windows.h>
#include "ISOMonster.h"
#include "IOnlineMap.h"
#include "FieldMonster.h"

class	IFieldMonster
{
private:
	cltFieldMonster		*pFieldMonster;
	SI32				siFieldMonsterNum;

	IMonster			*m_pIMonster;

private:
	SI32	FindIndex(SI32 siID);		// ID를 사용하여 INDEX를 찾는다.
public:			
	IFieldMonster();					// 생성자.
	~IFieldMonster();					// 소멸자.
	
	VOID	Init(IMonster *pIMonster, ISOMap *pIMap);	// Init.
	VOID	Action();										// 액션.
	VOID	DeadMonster(SI32 siID);							// 몬스터가 죽었다.	
	VOID	AddFollowerInfo(UI08 uiCharID, _ImportantParameta *pImpotantParameter);		// 캐릭터의 정보를 추가해준다.	
	VOID	StartBattle(SI32 siID);		// 전투를 시작한다.
	VOID	EndBattle(SI32 siID);		// 전투를 종료한다.
	BOOL	GetInfo(SI32 siID, CHAR *pszBuffer);			// 정보를 얻어온다.
};

#endif
