#ifndef	_ONLINE_IMJIN2I_H
#define _ONLINE_IMJIN2I_H

#include "Defines.h"
#include <parametabox.h>
#include <OnlineMyWeapons.h>

// �������� �÷��̾�鿡 ���� ����. 
struct	OnImjinBattlePlayerInfo
{
	CHAR	szPlayerName[ON_MAX_FOLLOWER_NUM][ON_ID_LENGTH + 1];	// ĳ���� �̸�.
	SI32	siPlayersUniqueID;										// ĳ���� ID.	
	SI32	GuildCode;												// ��� �ڵ� 	
	BOOL	bPlayer;												// �Ϲ� ������ΰ�? ��ǻ�� �����ΰ�?
	UI16	uiKind[ON_MAX_FOLLOWER_NUM];							// ĳ���͵�.

	_ImportantParameta Iparameta[ON_MAX_FOLLOWER_NUM];				// �� ĳ���͵��� �߿� �Ķ��Ÿ ����. 

	MyWeapons	MW[ON_MAX_FOLLOWER_NUM];							// �� ĳ���͵��� �κ��丮 ��Ȳ ����. 
};

// ���Ӽ����� Ŭ���̾�Ʈ�� �뺸�ϴ� �޽���. 
struct	OnlineBattleInfo
{
	SI32						BattleMode;						// � ���� Ÿ���ΰ� ?
	CHAR						Reserved[9];
	SI32						siBattleServerChannel;			// Ŭ���̾�Ʈ���� ������ ���� ������ ä�� ��ȣ.
	SI32						siMyCharUniqueID;				// ���� ���� ID.	
	UI16						uiRandomSeed;					// ���� Seed.
	MONEY						UserMoney;

	DWORD						PlayerNumber;
	BOOL						IsSingleMode;

	OnImjinBattlePlayerInfo		OIBPI[8];						// �����Ͽ��� �����Ϸ��� ĳ������ ����.

};

class	OnlineImjin2I
{
public:
	OnlineBattleInfo	BattleInfo;

public:
	OnlineImjin2I();							// ������.
};


// ������ 2 Ŭ���̾�Ʈ�� ����� �ϱ� ���� �����̴�.
extern	OnlineImjin2I					GOnlineImjin2I;
#endif
