#ifndef	ISO_MONSTER_HEADER
#define ISO_MONSTER_HEADER

#include "SOMap.h"
#include "SOMonster.h"
#include "GSCDefine.h"



class	IMonster
{
private:
	SOWorld			*pMyWorld;

	Monster			Monsters[MAX_MONSTER_NUM];
	UI32			uiLastNPCAccount;

private:
	UI16			FindAccount();				// ������ ã�´�.


public:	
	IMonster();									// ������
	~IMonster();								// �Ҹ���

	// �ʱ�ȭ
	VOID	Init(SOWorld *pWorld);												

	// ���͸� �����Ѵ�
	BOOL	CreateMonster(SI32 siFieldMonsterID, UI16 uiMapID, LPRECT prcActivityArea, UI16 uiRepresentKind, 
						  UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);		
	BOOL	CreateMonster(UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiRepresentKind, 
						  UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);	
	
	
	VOID	Free();															 // ����																						
	VOID	Free(UI16 uiAccount, BOOL IsDeadInBattle);						 // Ư�� ���͸� �����Ѵ�.
		
	VOID	Action(UI16 uiAccount);																	
		
	VOID	GetInfo(UI16 uiAccount, OnCharMonsterInfo *pMonsterInfo);		// ������ ������ ���´�.								
	BOOL	IsValidID(UI16 uiAccount);										 // ��ȿ�� ID �ΰ�?
	VOID	DeadChar(UI16 uiAccount, BOOL IsDeadInBattle);					 // ĳ���͸� ���δ�.(ĳ���� ��ü��)	
	SI16	GetStatus(UI16 uiAccount);										 // ĳ������ ���¸� ���´�.
	
	SI32	GetFightPower(UI16 uiAccount);									 // ������ �������� ����
	BOOL	Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY);		 // ���͸� ���� ��Ų��.

	UI16	GetKind(UI16 uiAccount);										 // ������ ������ ���´�.
	VOID	GetPos(UI16 uiAccount, UI16 *puiMapIndex, LPPOINT pPoint);		 // ������ ��ġ�� �˾ƿ´�.	
	SI32	GetFieldID(UI16 uiAccount);										 // ���Ͱ� �Ҽ��� �ִ� �ʵ��� ID�� ���´�.
	UI08	DidSendMoveInfo(UI16 uiAccount);								 // �̵� �����ÿ� �̵� �޽����� ������ �÷��̾�鿡�� ���´°� �˾ƿ´�.

	BOOL	CheckInvalidChar( CHAR *pszParam );

	Monster*	GetMonster(UI16 uiAccount);									 // ���͸� ���´�.

	DWORD	GetTotalMonsterNum();											// ����ִ� ��ü ������ ���� ���´�.
};

#endif
