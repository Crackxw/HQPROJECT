//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>
#include <myfile.h>
#include "..\Gersang\maindata.h"
#include <CharOrg.h>
#include "..\Gersang\yprg.h"
#include "..\Gersang\text.h"
#include <setchar.h>
#include <etc.h>
#include <mynet.h>

extern	_MainData		GD;
extern	_KindInfo		KI[];
extern	PLAYERSNAME		PlayersName;					// �÷��̾��� �̸��� �����Ѵ�. 
extern	_Char			CR[];
extern	int				GeneralList[];					// ������ ����� ����Ʈ 
extern	SI32			UserPlayerNumber;


DWORD CheaterMessageClock ;
int GlobalKindInfoCheckIndex = 0;



void _PlayerInfo::Init(SHORT pnumber)
{
	Country      = COUNTRY_NO;
	PlayerNumber = (unsigned char)pnumber;
	Host         = BY_COMPUTER;

	MyBit=(1<<pnumber);
}

void _Player::Init(SI32 pnumber)
{
	PI.Init(pnumber);
}

