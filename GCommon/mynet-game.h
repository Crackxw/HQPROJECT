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
	// ������ �ٰŷ� ���� ��� �������� �����Ѵ�. 
	void DecideTransVary(SI32 latency);

};

#endif
