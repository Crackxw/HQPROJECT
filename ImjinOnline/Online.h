#ifndef	_ONLINE_H
#define _ONLINE_H

#define	ON_SCREEN_XSIZE				800
#define	ON_SCREEN_YSIZE				600
  
#define	ONLINE_START_LATITUDE		45				// ���� ����.
#define	ONLINE_START_LONGITUDE		108				// ���� �浵.
#define	ONLINE_END_LATITUDE			22				// ���� ����.
#define	ONLINE_END_LONGITUDE		145				// ���� �浵.
#define	ONLINE_LATITUDE_HEIGHT		(abs(ONLINE_END_LATITUDE - ONLINE_START_LATITUDE))			// ���ӿ��� ����ϴ� ���� ����
#define	ONLINE_LONGITUDE_WIDTH		(abs(ONLINE_END_LONGITUDE - ONLINE_START_LONGITUDE))			// ���ӿ��� ����ϴ� �浵 ����

// CLIENT �ƴϸ� SERVER���� �Ѵ�.
#define	ONLINE_CLIENT

// �� �÷��̾�� ���� �� �ִ� �ִ� ĳ���� ��
//#define	ONLINE_MAX_CHARACTER_PER_PLAYER		3

//#define	ONLINE_DEBUG				// �¶��� ����� ����̴�.

#define	ON_STRUCT_IN_VILLAGE_POS_FIXED
 
class	cltOnlineWorld;

// extern	HQDebug  GHQDebugTool;

#define	ON_PERCENT(x)		(x)
#endif
