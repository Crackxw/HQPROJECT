#ifndef _GAME_H
#define _GAME_H

// ���� ���� ����
#define GM_ERROR      0    // ���� �������� �� �𸣰��� 
#define GM_STAGE      1    // �������� ������ 
#define GM_SOLO       2    // ȥ���ϱ� 
#define GM_MULTI      3    // ������ �ϱ� 

// ����� ������ ������ �ʱ�ȭ�Ѵ�.
BOOL InitUserMapGame();

void InitGame1();
void InitGame2();

// ������ �����ϴµ� �ʿ��� ���� 
void EndGame();

// Ű���� ó���� ������ ��ƾ 
BOOL GameOverHead();
SHORT GameAction();   // ���� ó�� 
SHORT GameDraw();   // �׸��� ���� �κ� 

// ���� ������ ��ƾ�� ����ִ� �Լ� 
SHORT GameProcess();

// ���콺 ��ư�� ���� �ڵ� ����� ó���Ѵ�. 
BOOL OperateAutoOrder();
void InputOperate(DWORD wparam);

void ScrollOperation();

DrawPannel();

#endif