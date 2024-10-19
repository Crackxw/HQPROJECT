#ifndef _STAGE_H
#define _STAGE_H  1 

#define VICTORY_DELAY     4000
#define VICTORY_HALFDELAY 3000
#define DEFEAT_DELAY      4000
#define DEFEAT_HALFDELAY  3000

int VictoryNDefeatDelay();

// 스테이지 게임을 진행하기 전에 초기화한다. 
void InitStage();

// 게임에 들어가기 전에 초기화해주는 함수 
BOOL InitStageGame();

// 승리했는지 패배 했는지 여부를 알려준다. 
SHORT WinCondition();
// 온라인 전투에서의 승리 조건 
SHORT WinConditionOnlineBattleMode();



#endif