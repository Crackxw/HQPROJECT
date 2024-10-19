#ifndef	_ONLINE_H
#define _ONLINE_H

#define	ON_SCREEN_XSIZE				800
#define	ON_SCREEN_YSIZE				600
  
#define	ONLINE_START_LATITUDE		45				// 시작 위도.
#define	ONLINE_START_LONGITUDE		108				// 시작 경도.
#define	ONLINE_END_LATITUDE			22				// 시작 위도.
#define	ONLINE_END_LONGITUDE		145				// 시작 경도.
#define	ONLINE_LATITUDE_HEIGHT		(abs(ONLINE_END_LATITUDE - ONLINE_START_LATITUDE))			// 게임에서 사용하는 위도 높이
#define	ONLINE_LONGITUDE_WIDTH		(abs(ONLINE_END_LONGITUDE - ONLINE_START_LONGITUDE))			// 게임에서 사용하는 경도 넓이

// CLIENT 아니면 SERVER여야 한다.
#define	ONLINE_CLIENT

// 한 플레이어당 가질 수 있는 최대 캐릭터 수
//#define	ONLINE_MAX_CHARACTER_PER_PLAYER		3

//#define	ONLINE_DEBUG				// 온라인 디버그 모드이다.

#define	ON_STRUCT_IN_VILLAGE_POS_FIXED
 
class	cltOnlineWorld;

// extern	HQDebug  GHQDebugTool;

#define	ON_PERCENT(x)		(x)
#endif
