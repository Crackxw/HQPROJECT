#ifndef _PLAYER_H
#define _PLAYER_H

#include <team.h>

// ���ð����� ������ ���� 
#define COUNTRY_NO        0      // �������� ���� 
#define COUNTRY_KOREA     1      // ���� (õ�ſ����� ����, ���)
#define COUNTRY_JAPAN     2      // �Ϻ� (õ�ſ����� ����)
#define COUNTRY_CHINA     3      // �� (õ�ſ����� �Ŷ�)
#define COUNTRY_TAIWAN    4      // �븸
#define COUNTRY_RANDOM    5      // ������ 
#define COUNTRY_NEUTRAL   6      // �߸���



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
	// ������ �ΰ� �����Ѵ�. 
    void SetFriendSwitch(SHORT pnumber, SHORT mode);


};


class _Player {
public:
	_PlayerInfo PI;
	_Team Team;

	// �ڵ� ���� ���� ����ġ 
	SHORT AutoMagicSwitch;

	void Init(SI32 pnumber);
	void PushFriend(SI32 player);
	void PopFriend(SI32 player);
	BOOL IsFriend(SI32 player);


};

#endif