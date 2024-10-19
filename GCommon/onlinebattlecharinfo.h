#ifndef _ONLINEBATTLECHARINFO_H
#define _ONLINEBATTLECHARINFO_H

#define MAX_ONLINE_CHAR_NUMBER_PER_PLAYER		10    // 플레이어당 최대 캐릭터 갯수. 

// 개별 캐릭터의 정보. 
class _OnlineBattleCharInfo {
public:
	UI08 CharKind;	// 캐릭터의 종류.
	UI08 Reserved[3];
};

// 각 플레이어의 정보. 
class _OnlineBattlePlayerInfo{
public:
	SI32 CharNumber;	// 플레이어 소속의 캐릭터 갯수. 
	_OnlineBattleCharInfo * pOBCI[MAX_ONLINE_CHAR_NUMBER_PER_PLAYER];
};

#endif

