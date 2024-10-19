#ifndef	SO_MONSTER_HEADER
#define SO_MONSTER_HEADER

#include <Directives.h>
#include <Windows.h>
#include "SOBaseChar.h"
#include "OnlineCommonStruct.h"
#include "SOMap.h"
#include "OnlineCGSHeader.h"
#include "Parametabox.h"
#include "OnlineMyWeapons.h"
#include "State.h"
#include "StateMgr.h"

class   SOPlayer;

// ���� Ŭ����.
class	Monster : public BaseChar, public CStateMgr
{

private:	
	SI32		siFieldMonsterID;
	LPRECT		prcActivityArea;		

public:
	Monster();								// ������.
	~Monster();								// �Ҹ���.

	// �ʱ�ȭ
	VOID	Init();							
	VOID	Init(SI32 sifieldmonsterid, UI16 uiaccount, UI16 uimapid, SI16 six, SI16 siy, LPRECT prcactivityarea, UI16 uiRepresentKind, 
				 UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);		

	VOID	Free(BOOL IsDeadInBattle);								// ����.	
	BOOL	DeadChar(UI08 uiCharID, BOOL IsDeadInBattle);			// ���Ͱ� ������ �ִ� ĳ���͸� ���ش�.
	BOOL	IsValidCharID(UI08 uicharID);							// ĳ������ ID�� ��ȿ���� ����.(������ ������� �ʴ���)
	VOID	GetInfo(OnCharMonsterInfo *pMonsterInfo);				// ���� ĳ������ ������ ���´�.
	
	VOID	Action();												// �� �������� �� ���Ͱ� �����ؾ� �ϴ� �Լ� 

//	BOOL	GetfFight(void);										// ���� ���������� �˻�
	BOOL	Warp(UI16 uiMapID, SI32 siX, SI32 siY);					// ������ �Ѵ�.
																												
	
	SI32	GetFieldID();											// ���Ͱ� �Ҽ��� �ִ� �ʵ��� ID�� ���´�.

	VOID	PrepareBattle();										// ������ �غ� �Ѵ� 
	VOID	EndBattle();											// ������ ���� �Ѵ� 


private:
//-------------------------------------------------------------------------------------------------------------------
	DWORD				m_actionDelay;								 
	DWORD				m_prevActionTime;
	SOPlayer*			m_targetEnemy;
	
	void				normalState();

	void				Move(POINT movePos);
	void				Attack();
	
	SOPlayer*			checkEnemyToArround();
	bool				checkActionDelay();

	POINT			    getRandomNextPos();
	POINT				getTargetNextPos();
	POINT				getEmptyNextPos();
	POINT				getAvoidNextPos();

	bool				isMonsterInField();
	bool				isPosInMyArea(int x, int y);

	void				seenEnemy(SOPlayer* enemy);
	void				lostEnemy();

	
};

#endif
