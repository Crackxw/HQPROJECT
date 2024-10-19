#ifndef _SPRG_H
#define _SPRG_H

BOOL Circle(int nStep, int x, int y, int *StepCnt, POINT *pt);
BOOL Circle2(int nStep, int x, int y, int ratio, POINT *pt);
BOOL BLine(int nStep, int x1, int y1, int x2, int y2, POINT *pt);
int WindEffect(int SDirect, int WDirect);

#endif