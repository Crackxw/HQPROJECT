#ifndef _MINE_H
#define _MINE_H

// ���� ���ڸ� ������ ���� ó��. 
BOOL DetectEnemyMine(SI32 x, SI32 y);

// ���� ���ڸ� ��������� ó��. 
BOOL StepEnemyMine(SI32 x, SI32 y, SI32 playernumber);

// x, y, �� ���ڰ� �ִ°�?
BOOL IsMine(SI32 x, SI32 y);

// x, y, �� ���� ���ڰ� �ִ°�?
BOOL IsEnemyMine(SI32 x, SI32 y, SI32 playernumber);

//  ���� ������ ó���ϴ� ��ƾ 
BOOL MineExplode(SHORT minepnumber, SHORT myid,SHORT x, SHORT y, BOOL validswitch);


#endif