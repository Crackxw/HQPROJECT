#ifndef _STAGE_H
#define _STAGE_H  1 

#define VICTORY_DELAY     4000
#define VICTORY_HALFDELAY 3000
#define DEFEAT_DELAY      4000
#define DEFEAT_HALFDELAY  3000

int VictoryNDefeatDelay();

// �������� ������ �����ϱ� ���� �ʱ�ȭ�Ѵ�. 
void InitStage();

// ���ӿ� ���� ���� �ʱ�ȭ���ִ� �Լ� 
BOOL InitStageGame();

// �¸��ߴ��� �й� �ߴ��� ���θ� �˷��ش�. 
SHORT WinCondition();
// �¶��� ���������� �¸� ���� 
SHORT WinConditionOnlineBattleMode();



#endif