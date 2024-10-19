#include <GSL.h>

#include <main.h>

#include <setchar.h>
#include "maindata.h"
#include <etc.h>
#include <config.h>
#include "cheat.h"
#include "stage.h"
//#include <stage-func.h>


#include <charinterface.h>
#include "gamemenu.h"
#include "text.h"
#include <GameButton.h>
#include <volcano.h>
#include <cltString.h>
#include <myfile.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	clock_t				CurrentClock;						// 시간 관련 
extern	_Cheat 				Cheat;
extern	_Text 				Text;								// 텍스트 
extern	_Config 			Config;								// 환경 변수 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	SHORT 				MapEditMode;						// 지도 편집 모드인가?
extern	BOOL				GameDropSwitch;

// 시간 제한에서의  승리 조건 
SHORT WinConditionTimeLimit();

int VictoryNDefeatDelay()
{
	if(GD.VictoryDelay)
	{
		if(abs(CurrentClock - GD.VictoryDelay) > VICTORY_DELAY)return 1;
	}

	if(GD.DefeatDelay)
	{
		if(abs(CurrentClock - GD.DefeatDelay) > DEFEAT_DELAY)return -1;
	}

	return 0;
}


// 승리했는지 패배 했는지 여부를 알려준다. 
SHORT WinCondition()
{

    // 지도 편집 모드에서는 승리 패배가 없다.
	if(MapEditMode==TRUE)return 0;

	int status = VictoryNDefeatDelay();
	if(status)
	{
		// 메뉴가 떠 있다면 메뉴를 없앤다. 
		SetExitMenu(); 

		return status;
	}

	return WinConditionOnlineBattleMode();
}



// 온라인 전투에서의 승리 조건 
SHORT WinConditionOnlineBattleMode()
{
	// 승리 조건은 매번 체크하는 것이 아니다. 
	if(GD.Frame <= 0 && (GD.Frame % 200))return 0;

	// 관전 모드이면 승리도 패배도 아니다.
	if(GD.Player[UserPlayerNumber].PI.Country == COUNTRY_NEUTRAL)return 0;

	SI32			i;
	SI32			id;
	BOOL enemyswitch = FALSE;
	BOOL ourswitch   = FALSE;

	for(i = 0;i < GD.CharOrderIndex;i++)
	{
		id = GD.CharOrder[i];
		if(IsAlive(id))
		{
			// 피차간에 적대적인 경우가 있으면 적으로 간주한다. 
			if(IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(id))==FALSE
			|| IsFriendToByPlayerNumber(clifChar.GetPlayerNumber(id), UserPlayerNumber)==FALSE)
			{
				enemyswitch = TRUE;
			}
			else
			{
				if(clifChar.GetPlayerNumber(id) == UserPlayerNumber)
				{
					ourswitch = TRUE;
				}
			}
		}
	}

	// 적이 존재하지 않으면 승리 
	if(enemyswitch == FALSE)
	{
		if(GD.VictoryDelay==0)
		{
			GD.VictoryDelay = CurrentClock;
		}
	}

	// 아군이 존재하지 않으면 패배 
	if(ourswitch == FALSE)
	{
		if(GD.DefeatDelay==0)
			GD.DefeatDelay = CurrentClock;
	}

	// 접속이 끊기면 승리 
	if(GameDropSwitch == TRUE
	&& GD.VictoryDelay == 0 
	&& GD.DefeatDelay == 0)
	     GD.VictoryDelay = CurrentClock;

		return 0;
}
