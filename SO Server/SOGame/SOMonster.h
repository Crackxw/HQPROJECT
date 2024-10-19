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

// 몬스터 클래스.
class	Monster : public BaseChar, public CStateMgr
{

private:	
	SI32		siFieldMonsterID;
	LPRECT		prcActivityArea;		

public:
	Monster();								// 생성자.
	~Monster();								// 소멸자.

	// 초기화
	VOID	Init();							
	VOID	Init(SI32 sifieldmonsterid, UI16 uiaccount, UI16 uimapid, SI16 six, SI16 siy, LPRECT prcactivityarea, UI16 uiRepresentKind, 
				 UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);		

	VOID	Free(BOOL IsDeadInBattle);								// 해제.	
	BOOL	DeadChar(UI08 uiCharID, BOOL IsDeadInBattle);			// 몬스터가 가지고 있는 캐릭터를 없앤다.
	BOOL	IsValidCharID(UI08 uicharID);							// 캐릭터의 ID가 유효한지 본다.(범위를 벗어나지는 않는지)
	VOID	GetInfo(OnCharMonsterInfo *pMonsterInfo);				// 몬스터 캐릭터의 정보를 얻어온다.
	
	VOID	Action();												// 매 루프마다 한 몬스터가 실행해야 하는 함수 

//	BOOL	GetfFight(void);										// 현재 전투중인지 검사
	BOOL	Warp(UI16 uiMapID, SI32 siX, SI32 siY);					// 워프를 한다.
																												
	
	SI32	GetFieldID();											// 몬스터가 소속해 있는 필드의 ID를 얻어온다.

	VOID	PrepareBattle();										// 전투를 준비 한다 
	VOID	EndBattle();											// 전투를 종료 한다 


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
