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

extern	_MainData			GD;									// ������ �ֵ����� 
extern	clock_t				CurrentClock;						// �ð� ���� 
extern	_Cheat 				Cheat;
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	_Config 			Config;								// ȯ�� ���� 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	SHORT 				MapEditMode;						// ���� ���� ����ΰ�?
extern	BOOL				GameDropSwitch;

// �ð� ���ѿ�����  �¸� ���� 
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


// �¸��ߴ��� �й� �ߴ��� ���θ� �˷��ش�. 
SHORT WinCondition()
{

    // ���� ���� ��忡���� �¸� �й谡 ����.
	if(MapEditMode==TRUE)return 0;

	int status = VictoryNDefeatDelay();
	if(status)
	{
		// �޴��� �� �ִٸ� �޴��� ���ش�. 
		SetExitMenu(); 

		return status;
	}

	return WinConditionOnlineBattleMode();
}



// �¶��� ���������� �¸� ���� 
SHORT WinConditionOnlineBattleMode()
{
	// �¸� ������ �Ź� üũ�ϴ� ���� �ƴϴ�. 
	if(GD.Frame <= 0 && (GD.Frame % 200))return 0;

	// ���� ����̸� �¸��� �й赵 �ƴϴ�.
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
			// �������� �������� ��찡 ������ ������ �����Ѵ�. 
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

	// ���� �������� ������ �¸� 
	if(enemyswitch == FALSE)
	{
		if(GD.VictoryDelay==0)
		{
			GD.VictoryDelay = CurrentClock;
		}
	}

	// �Ʊ��� �������� ������ �й� 
	if(ourswitch == FALSE)
	{
		if(GD.DefeatDelay==0)
			GD.DefeatDelay = CurrentClock;
	}

	// ������ ����� �¸� 
	if(GameDropSwitch == TRUE
	&& GD.VictoryDelay == 0 
	&& GD.DefeatDelay == 0)
	     GD.VictoryDelay = CurrentClock;

		return 0;
}
