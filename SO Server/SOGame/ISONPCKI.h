#ifndef ISONPC_H
#define ISONPC_H

#include <Directives.h>
#include "SONPCKI.h"

class	INPCKI
{
private:
	SI32		siTotalNPCKINum;
	NPCKI		*pNPCKI;	

private:
	SI16	FindIndex(UI16 uiKind);				// Kind를 사용하여 Index를 얻어온다.
public:	
	INPCKI();		// 생성자.
	~INPCKI();		// 소멸자.

	BOOL	Init();	// 초기화.
	VOID	Free();	// 해제.

	SI32	GetTotalMonsterNum();				// 전체 NPC 수를 얻어온다.	
	UI16	GetID(UI16 uiIndex);				// NPC의 ID를 얻어온다.
	UI16	GetMoveSpeed(UI16 uiKind);			// 캐릭터의 이동 속도를 얻어온다.
};

#endif
