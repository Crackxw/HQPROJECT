#ifndef _FINDPATHSTARAIGHT_H
#define _FINDPATHSTARAIGHT_H


// 직선 경로를 구한다. 
// 파라메타 
// x, y   : 시작위치 
// dx, dy : 목적지 위치 
// direction: 나의 방향 (16방위 )
//                     SOUTH, SSW, SOUTH|WEST, WWS, WEST, WWN, NORTH|WEST, NNW, 
//		               NORTH, NNE, NORTH|EAST, EEN, EAST, EES, SOUTH|EAST, SSE 
// range: 이동 거리 
// gomode : 목적지로 갈것인가여부 (TRUE ->  목적지로 간다. 
//                                 FASLSE-> 현재 방향에 따라 이동한다. )
// nx, ny : 이동해야할 위치

// 리턴값:  이동가능하면   1 
//          이동불가능하면 2
 
SHORT FindPathStraight(SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT direction, SHORT range, SHORT gomode, SHORT& nx, SHORT &ny);


#endif
