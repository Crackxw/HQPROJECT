#ifndef _FINDPATHSTARAIGHT_H
#define _FINDPATHSTARAIGHT_H


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
