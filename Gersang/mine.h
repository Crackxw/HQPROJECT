#ifndef _MINE_H
#define _MINE_H

// 적의 지뢰를 제거할 떄의 처리. 
BOOL DetectEnemyMine(SI32 x, SI32 y);

// 적의 지뢰를 밟았을때의 처리. 
BOOL StepEnemyMine(SI32 x, SI32 y, SI32 playernumber);

// x, y, 에 지뢰가 있는가?
BOOL IsMine(SI32 x, SI32 y);

// x, y, 에 적의 지뢰가 있는가?
BOOL IsEnemyMine(SI32 x, SI32 y, SI32 playernumber);

//  지뢰 폭발을 처리하는 루틴 
BOOL MineExplode(SHORT minepnumber, SHORT myid,SHORT x, SHORT y, BOOL validswitch);


#endif