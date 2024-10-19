#include <Main.h>

#ifdef _IMJIN2
	#include "..\Gersang\maindata.h"
	#include <etc.h>
#endif

#include <map.h>
#ifdef _IMJIN2
	#include "..\Gersang\char.h"
#endif

extern _Char CR[];
extern _MainData GD;

BOOL _Player::SetFireWarning(SI32 six, SI32 siy)
{
	FirePos.x = six;
	FirePos.y = siy;


	return TRUE;
}

BOOL _Player::DecideCheckFire()
{
	SI32	id;

	switch( PlayerStep )
	{
	case PS_CHECK_FIREBUFFER:
		if( FirePos.x >= 0 )
		{
			ExtinguishPos = FirePos;
			PlayerStep = PS_FIND_NEARFIREMAN;
		}
		else
		{
			ComWait();
		}
		break;
	case PS_FIND_NEARFIREMAN:
		id = FindNearFireman();
		if( IsAlive(id) )
		{
			OrderExtinguish(CR[id].NewID, ExtinguishPos.x, ExtinguishPos.y, BY_COMPUTER); 
		}

		ComWait();
		break;	
	}

	return TRUE;
}

SI32 _Player::FindNearFireman()
{
	SI32 length, length_score, random_score, score;
	SI32 max_score = 0;
	SI32 max_score_id = 0;
	SI32 id;
	SI32 i;

	for( i=0; i<CharNumber; i++ )
	{
		id = CharID[i].ID;
		
		if( IsSee(id)											// 살아 있어야 한다. 
			&& CR[id].GetPlayerNumber() == PI.PlayerNumber      // 같은 플레이어야 한다.
			&& CR[id].IsAtb2(ATB2_EXTINGUISH) == TRUE			// 불을 끌 수 있는 유닛이어야 한다.
			&& CR[id].CurrentOrder != ORDER_EXTINGUISH			// 현재 불을 끄고 있지 않아야 한다.
			&& CR[id].CanWork() )                               // 작업 가능해야 한다. 
		{
			// 목적지까지의 거리를 구한다. (50%)
			// 거리가 가까울수록 좋다. 
			length = max( abs(CR[id].GetX()-ExtinguishPos.x), abs(CR[id].GetY()-ExtinguishPos.y) );
			
			// 거리에 의한 점수 
			// 가까울수록 점수가 높다.
			length_score = max(0, 100-(length*100/MAX_MAP_XSIZE)) * 50 / 100;
			
			// 랜덤 값 (50%)
            random_score = GD.Random() % 50;

			// 총 점수를 구한다. 
			score = length_score + random_score;

			if( score > max_score )
			{
				max_score = score;
				max_score_id = id;
			}
		}
	} //for

	return max_score_id;
}
