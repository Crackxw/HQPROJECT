#ifndef	_GBSERVER_HEADER
#define	_GBSERVER_HEADER
/*
// ID 부여 (총 65535개의 ID가 할당 되어 있다.)
// 서버에서의 플레이어 관련. (1 ~ 32767)
#define	MAX_PLAYER_NUM	32768
#define	PLAYER_START_ACCOUNT		1		

// 서버에서의 배 관련.(32768 ~ 33767) 
#define	MAX_SHIP_NUM		1000
#define	SHIP_START_ACCOUNT		32768

// 서버에서의 몬스터 관련. (33768 ~ 65535)
#define	MAX_MONSTER_NUM			31768
#define	MONSTER_START_ACCOUNT	33768

// Account를 이용하여 Kind를 얻어온다.
#define	ISPLAYER(x)				(((x >= PLAYER_START_ACCOUNT) && x <= (PLAYER_START_ACCOUNT + MAX_PLAYER_ACCOUNT_NUM - 1)) ? TRUE : FALSE)
#define	ISHIP(x)				(((x >= SHIP_START_ACCOUNT) && x <= (SHIP_START_ACCOUNT + MAX_SHIP_ACCOUNT_NUM - 1)) ? TRUE : FALSE)
#define	ISMONSTER(x)			(((x >= MONSTER_START_ACCOUNT) && x <= (MONSTER_START_ACCOUNT + MAX_MONSTER_ACCOUNT_NUM - 1)) ? TRUE : FALSE)
*/
#define _IMJINONLINESERVER


#endif
