#ifndef _GAME2_H
#define _GAME2_H


// ���� ���� ����
#define GM_ERROR      0    // ���� �������� �� �𸣰��� 
#define GM_STAGE      1    // �������� ������ 
#define GM_SOLO       2    // ȥ���ϱ� 
#define GM_MULTI      3    // ������ �ϱ� 

void			InitGame1();
void			InitGame2();
// ����� ������ ������ �ʱ�ȭ�Ѵ�.
BOOL 			InitUserMapGame();
// �¶��� ���� ������ �ʱ�ȭ�Ѵ�.
BOOL 			InitOnlineBattleGame();

BOOL 			GameDraw1();
BOOL 			GameDraw2();
BOOL 			GameDraw3();
BOOL 			GameDraw4();
void 			DrawSmallMapImjin();
void 			DrawSmallMapMyth();
BOOL 			DrawPannel();
void 			SmallMapToggleOperation();
// ���� ������ ��ƾ�� ����ִ� �Լ� 
// ���ϰ�: 0 -> normal
SHORT 			GameProcess();

// ������ �ӵ��� �����ϰ� �����. 
BOOL 			GameDelay();

BOOL 			GameWinCondition();
// ������ �����ϴµ� �ʿ��� ���� 
void 			EndGame();
SHORT 			GameAction();
// ���콺 ��ư�� ���� �ڵ� ����� ó���Ѵ�. 
BOOL			OperateAutoOrder();
void			ScrollOperation();
void			SystemKeyCheck();
void			GroupInterface();
_NewID			GetUnderMouseID();
// �巡�װ� ����Ǿ������� ó���̴�.
void			DragOperation();
// Ű���� ó���� ������ ��ƾ 
BOOL			GameOverHead();

VOID			InitGameSetChar(SI32 &userpointindex);
// �ϱ��� ������ ��� ������� �ɴ´�.
VOID			InitGameSetGeneral(SI32 &userpointindex);
VOID			InitGameSetCharOnline(SI32 &userpointindex);;

// ���� ������ �׻� ��ȿ�ѱ� ?
BOOL			IsSmallMapValid();

// �¶��� ������ ��ĥ ������ ������ �����Ѵ�. 
BOOL FindProperOnlineBattleMap(SI32 playercount, SI32 maptype, char* mapname);

// ��ũ�� ��� ���θ� �����ϴ� �Լ�. 
void SetScrollProcSwitch(BOOL mode);

// �𰢿� ���ؼ� ����ġ �ս��� ġ���.
void SendBattleMessage_LoseForRetreat();

#endif
