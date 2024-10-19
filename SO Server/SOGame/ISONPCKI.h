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
	SI16	FindIndex(UI16 uiKind);				// Kind�� ����Ͽ� Index�� ���´�.
public:	
	INPCKI();		// ������.
	~INPCKI();		// �Ҹ���.

	BOOL	Init();	// �ʱ�ȭ.
	VOID	Free();	// ����.

	SI32	GetTotalMonsterNum();				// ��ü NPC ���� ���´�.	
	UI16	GetID(UI16 uiIndex);				// NPC�� ID�� ���´�.
	UI16	GetMoveSpeed(UI16 uiKind);			// ĳ������ �̵� �ӵ��� ���´�.
};

#endif
