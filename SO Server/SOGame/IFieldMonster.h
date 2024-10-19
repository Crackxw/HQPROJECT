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
	SI32	FindIndex(SI32 siID);		// ID�� ����Ͽ� INDEX�� ã�´�.
public:			
	IFieldMonster();					// ������.
	~IFieldMonster();					// �Ҹ���.
	
	VOID	Init(IMonster *pIMonster, ISOMap *pIMap);	// Init.
	VOID	Action();										// �׼�.
	VOID	DeadMonster(SI32 siID);							// ���Ͱ� �׾���.	
	VOID	AddFollowerInfo(UI08 uiCharID, _ImportantParameta *pImpotantParameter);		// ĳ������ ������ �߰����ش�.	
	VOID	StartBattle(SI32 siID);		// ������ �����Ѵ�.
	VOID	EndBattle(SI32 siID);		// ������ �����Ѵ�.
	BOOL	GetInfo(SI32 siID, CHAR *pszBuffer);			// ������ ���´�.
};

#endif
