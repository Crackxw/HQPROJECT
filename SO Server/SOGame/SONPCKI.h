#ifndef SONPC_H
#define SONPC_H

#include <windows.h>
#include <Directives.h>

// NPC 클래스이다.(몬스터)
class	NPCKI
{
private:
	UI16	uiID;			// NPC의 ID이다.	
	SI16	siNation;		// 국가이다.
	SI16	siMoveSpeed;	// NPC의 이동 속도이다.
public:
	NPCKI();				// 생성자.
	~NPCKI();				// 소멸자.

	VOID	Init(UI16 uiID, SI16 siNation, SI16 siMoveSpeed);			// 초기화.
	VOID	Free();														// 해제
	UI16	GetID();													// ID를 얻어온다.
	UI16	GetMoveSpeed();												// 이동 속도를 얻어온다.
};

#endif
