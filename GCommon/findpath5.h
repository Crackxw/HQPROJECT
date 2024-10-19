#ifndef _FINDPATH5_H
#define _FINDPATH5_H

#define BASIC_MOVE_BLOCK_COUNT   4

#include <pathfind.h>

class _FindPathGame : public _FindPath{
public:

	SHORT MoveID;
	SHORT MaxNodes;
	SI32 MinX, MaxX;
	SI32 MinY, MaxY;

	_FindPathGame(int maxxsize, int maxysize);
	BOOL IsColl(int x, int y);
	int FindPath(SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT& mx, SHORT& my, SHORT range, SHORT id);
};

// ���� ��θ� ���Ѵ�. 
// �Ķ��Ÿ 
// x, y   : ������ġ 
// dx, dy : ������ ��ġ 
// direction: ���� ���� (16���� )
//                     SOUTH, SSW, SOUTH|WEST, WWS, WEST, WWN, NORTH|WEST, NNW, 
//		               NORTH, NNE, NORTH|EAST, EEN, EAST, EES, SOUTH|EAST, SSE 
// range: �̵� �Ÿ� 
// gomode : �������� �����ΰ����� (TRUE ->  �������� ����. 
//                                 FASLSE-> ���� ���⿡ ���� �̵��Ѵ�. )
// nx, ny : �̵��ؾ��� ��ġ

// ���ϰ�:  �̵������ϸ�   1 
//          �̵��Ұ����ϸ� 2
 
SHORT FindPathStraight(SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT direction, SHORT range, SHORT gomode, SHORT& nx, SHORT &ny);



#endif