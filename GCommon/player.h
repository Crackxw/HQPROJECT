#ifndef _PLAYER_H
#define _PLAYER_H

#include <team.h>

// 선택가능한 국가의 종류 
#define COUNTRY_NO        0      // 등장하지 않음 
#define COUNTRY_KOREA     1      // 조선 (천신에서는 고구려, 고려)
#define COUNTRY_JAPAN     2      // 일본 (천신에서는 백제)
#define COUNTRY_CHINA     3      // 명 (천신에서는 신라)
#define COUNTRY_TAIWAN    4      // 대만
#define COUNTRY_RANDOM    5      // 랜덤국 
#define COUNTRY_NEUTRAL   6      // 중립국



class _PlayerInfo
{
public:

	unsigned char Country;
	unsigned char PlayerNumber;
	unsigned char Host;
	unsigned char MyBit;
	unsigned char EnemyBit;
	unsigned char Reserved;


	void Init(SHORT pnumber);
	// 동맹을 맺고 해제한다. 
    void SetFriendSwitch(SHORT pnumber, SHORT mode);


};


class _Player {
public:
	_PlayerInfo PI;
	_Team Team;

	// 자동 마법 구사 스위치 
	SHORT AutoMagicSwitch;

	void Init(SI32 pnumber);
	void PushFriend(SI32 player);
	void PopFriend(SI32 player);
	BOOL IsFriend(SI32 player);


};

#endif