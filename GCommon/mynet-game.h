#ifndef _MYNETGAME_H
#define _MYNETGAME_H

#include <MyNet.h>

class _MyNetGame : public _MyNet{
public:

	DWORD PingDelay[MAX_MULTI_PLAYER];
	DWORD PingDelayCount[MAX_MULTI_PLAYER];
	DWORD PingAccumulated[MAX_MULTI_PLAYER];

	void Init();
	int GetGameLatency();
	// 지연을 근거로 각종 통신 변수들을 설정한다. 
	void DecideTransVary(SI32 latency);

};

#endif
