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
	UI16			FindAccount();				// 계정을 찾는다.


public:	
	IMonster();									// 생성자
	~IMonster();								// 소멸자

	// 초기화
	VOID	Init(SOWorld *pWorld);												

	// 몬스터를 생성한다
	BOOL	CreateMonster(SI32 siFieldMonsterID, UI16 uiMapID, LPRECT prcActivityArea, UI16 uiRepresentKind, 
						  UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);		
	BOOL	CreateMonster(UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiRepresentKind, 
						  UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER]);	
	
	
	VOID	Free();															 // 해제																						
	VOID	Free(UI16 uiAccount, BOOL IsDeadInBattle);						 // 특정 몬스터를 해제한다.
		
	VOID	Action(UI16 uiAccount);																	
		
	VOID	GetInfo(UI16 uiAccount, OnCharMonsterInfo *pMonsterInfo);		// 몬스터의 정보를 얻어온다.								
	BOOL	IsValidID(UI16 uiAccount);										 // 유효한 ID 인가?
	VOID	DeadChar(UI16 uiAccount, BOOL IsDeadInBattle);					 // 캐릭터를 죽인다.(캐릭터 자체를)	
	SI16	GetStatus(UI16 uiAccount);										 // 캐릭터의 상태를 얻어온다.
	
	SI32	GetFightPower(UI16 uiAccount);									 // 몬스터의 전투력을 얻어옴
	BOOL	Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY);		 // 몬스터를 워프 시킨다.

	UI16	GetKind(UI16 uiAccount);										 // 몬스터의 종류를 얻어온다.
	VOID	GetPos(UI16 uiAccount, UI16 *puiMapIndex, LPPOINT pPoint);		 // 몬스터의 위치를 알아온다.	
	SI32	GetFieldID(UI16 uiAccount);										 // 몬스터가 소속해 있는 필드의 ID를 얻어온다.
	UI08	DidSendMoveInfo(UI16 uiAccount);								 // 이동 했을시에 이동 메시지를 주위에 플레이어들에게 보냈는가 알아온다.

	BOOL	CheckInvalidChar( CHAR *pszParam );

	Monster*	GetMonster(UI16 uiAccount);									 // 몬스터를 얻어온다.

	DWORD	GetTotalMonsterNum();											// 살아있는 전체 몬스터의 수를 얻어온다.
};

#endif
