#ifndef _GAME2_H
#define _GAME2_H


// 현재 게임 상태
#define GM_ERROR      0    // 무슨 상태인지 잘 모르겠음 
#define GM_STAGE      1    // 스테이지 게임중 
#define GM_SOLO       2    // 혼자하기 
#define GM_MULTI      3    // 여럿이 하기 

void			InitGame1();
void			InitGame2();
// 사용자 지도로 게임을 초기화한다.
BOOL 			InitUserMapGame();
// 온라인 전투 게임을 초기화한다.
BOOL 			InitOnlineBattleGame();

BOOL 			GameDraw1();
BOOL 			GameDraw2();
BOOL 			GameDraw3();
BOOL 			GameDraw4();
void 			DrawSmallMapImjin();
void 			DrawSmallMapMyth();
BOOL 			DrawPannel();
void 			SmallMapToggleOperation();
// 실제 게임의 루틴이 들어있는 함수 
// 리턴값: 0 -> normal
SHORT 			GameProcess();

// 게임의 속도를 일정하게 만든다. 
BOOL 			GameDelay();

BOOL 			GameWinCondition();
// 게임을 종료하는데 필요한 설정 
void 			EndGame();
SHORT 			GameAction();
// 마우스 버튼에 의한 자동 명령을 처리한다. 
BOOL			OperateAutoOrder();
void			ScrollOperation();
void			SystemKeyCheck();
void			GroupInterface();
_NewID			GetUnderMouseID();
// 드래그가 종료되었을때의 처리이다.
void			DragOperation();
// 키보드 처리를 포함한 루틴 
BOOL			GameOverHead();

VOID			InitGameSetChar(SI32 &userpointindex);
// 일기토 게임일 경우 장수들을 심는다.
VOID			InitGameSetGeneral(SI32 &userpointindex);
VOID			InitGameSetCharOnline(SI32 &userpointindex);;

// 작은 지도가 항상 유효한기 ?
BOOL			IsSmallMapValid();

// 온라인 전투를 펼칠 적당한 지도를 선택한다. 
BOOL FindProperOnlineBattleMap(SI32 playercount, SI32 maptype, char* mapname);

// 스크롤 허용 여부를 결정하는 함수. 
void SetScrollProcSwitch(BOOL mode);

// 퇴각에 의해서 경험치 손실을 치룬다.
void SendBattleMessage_LoseForRetreat();

#endif
