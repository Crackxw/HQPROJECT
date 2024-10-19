#ifndef _CROP_H
#define _CROP_H

// kind의 곡물이 수확할 가치가 있는 것인가?
BOOL IsCropHarvest(SHORT id, SHORT kind);
// 가까운 곳의 곡물을 찾는다. 
BOOL FindNearCrop(SHORT x, SHORT y, SHORT &dx, SHORT &dy, SHORT croptype);

#endif