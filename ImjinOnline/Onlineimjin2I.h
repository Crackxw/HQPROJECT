#ifndef	_ONLINE_IMJIN2I_H
#define _ONLINE_IMJIN2I_H

#include "Defines.h"
#include <parametabox.h>
#include <OnlineMyWeapons.h>

// 개별적인 플레이어들에 대한 정보. 
struct	OnImjinBattlePlayerInfo
{
	CHAR	szPlayerName[ON_MAX_FOLLOWER_NUM][ON_ID_LENGTH + 1];	// 캐릭터 이름.
	SI32	siPlayersUniqueID;										// 캐릭터 ID.	
	SI32	GuildCode;												// 상단 코드 	
	BOOL	bPlayer;												// 일반 사용자인가? 컴퓨터 몬스터인가?
	UI16	uiKind[ON_MAX_FOLLOWER_NUM];							// 캐릭터들.

	_ImportantParameta Iparameta[ON_MAX_FOLLOWER_NUM];				// 각 캐릭터들의 중요 파라메타 정보. 

	MyWeapons	MW[ON_MAX_FOLLOWER_NUM];							// 각 캐릭터들의 인벤토리 상황 정보. 
};

// 게임서버가 클라이언트로 통보하는 메시지. 
struct	OnlineBattleInfo
{
	SI32						BattleMode;						// 어떤 전투 타입인가 ?
	CHAR						Reserved[9];
	SI32						siBattleServerChannel;			// 클라이언트에서 접속할 전투 서버의 채널 번호.
	SI32						siMyCharUniqueID;				// 나의 고유 ID.	
	UI16						uiRandomSeed;					// 랜덤 Seed.
	MONEY						UserMoney;

	DWORD						PlayerNumber;
	BOOL						IsSingleMode;

	OnImjinBattlePlayerInfo		OIBPI[8];						// 임진록에서 전투하려는 캐릭터의 정보.

};

class	OnlineImjin2I
{
public:
	OnlineBattleInfo	BattleInfo;

public:
	OnlineImjin2I();							// 생성자.
};


// 임진록 2 클라이언트와 통신을 하기 위한 변수이다.
extern	OnlineImjin2I					GOnlineImjin2I;
#endif
