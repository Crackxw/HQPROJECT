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
		
		if( IsSee(id)											// ��� �־�� �Ѵ�. 
			&& CR[id].GetPlayerNumber() == PI.PlayerNumber      // ���� �÷��̾�� �Ѵ�.
			&& CR[id].IsAtb2(ATB2_EXTINGUISH) == TRUE			// ���� �� �� �ִ� �����̾�� �Ѵ�.
			&& CR[id].CurrentOrder != ORDER_EXTINGUISH			// ���� ���� ���� ���� �ʾƾ� �Ѵ�.
			&& CR[id].CanWork() )                               // �۾� �����ؾ� �Ѵ�. 
		{
			// ������������ �Ÿ��� ���Ѵ�. (50%)
			// �Ÿ��� �������� ����. 
			length = max( abs(CR[id].GetX()-ExtinguishPos.x), abs(CR[id].GetY()-ExtinguishPos.y) );
			
			// �Ÿ��� ���� ���� 
			// �������� ������ ����.
			length_score = max(0, 100-(length*100/MAX_MAP_XSIZE)) * 50 / 100;
			
			// ���� �� (50%)
            random_score = GD.Random() % 50;

			// �� ������ ���Ѵ�. 
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
