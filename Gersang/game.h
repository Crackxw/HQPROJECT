#ifndef _GAME_H
#define _GAME_H

// 현재 게임 상태
#define GM_ERROR      0    // 무슨 상태인지 잘 모르겠음 
#define GM_STAGE      1    // 스테이지 게임중 
#define GM_SOLO       2    // 혼자하기 
#define GM_MULTI      3    // 여럿이 하기 

// 사용자 지도로 게임을 초기화한다.
BOOL InitUserMapGame();

void InitGame1();
void InitGame2();

// 게임을 종료하는데 필요한 설정 
void EndGame();

// 키보드 처리를 포함한 루틴 
BOOL GameOverHead();
SHORT GameAction();   // 각종 처리 
SHORT GameDraw();   // 그리기 관련 부분 

// 실제 게임의 루틴이 들어있는 함수 
SHORT GameProcess();

// 마우스 버튼에 의한 자동 명령을 처리한다. 
BOOL OperateAutoOrder();
void InputOperate(DWORD wparam);

void ScrollOperation();

DrawPannel();

#endif