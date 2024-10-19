#ifndef SONPC_H
#define SONPC_H

#include <windows.h>
#include <Directives.h>

// NPC Ŭ�����̴�.(����)
class	NPCKI
{
private:
	UI16	uiID;			// NPC�� ID�̴�.	
	SI16	siNation;		// �����̴�.
	SI16	siMoveSpeed;	// NPC�� �̵� �ӵ��̴�.
public:
	NPCKI();				// ������.
	~NPCKI();				// �Ҹ���.

	VOID	Init(UI16 uiID, SI16 siNation, SI16 siMoveSpeed);			// �ʱ�ȭ.
	VOID	Free();														// ����
	UI16	GetID();													// ID�� ���´�.
	UI16	GetMoveSpeed();												// �̵� �ӵ��� ���´�.
};

#endif
