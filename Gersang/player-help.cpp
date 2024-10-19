#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
	#include <etc.h>
#endif

#include <map.h>
#include "char.h"

extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;


void _Player::ComHelp()
{
	SHORT i;
	SHORT id;
	SHORT length;
	SHORT max_score, total_score, random_score, attack_score, length_score;
	SHORT maxid;
	SHORT attackid=0;

	switch(PlayerStep)
	{
	// 도움을 필요로 하는 캐릭터를 찾는다. 
	case PS_DECIDE_NEED_HELP_CHAR:

		HelpAttackTargetID.Init();
		max_score=0;


		for(i=0;i<CharNumber;i++)
		{
			id=CharID[i].ID;

			// 도움이 필요한 캐릭터를 찾았다. 
			attackid=0;
			if(IsSee(id))
			{
				if(CR[id].BeAttackedSwitch==TRUE 
			    &&  IsSee(CR[id].AttackMeID.ID) 
			    && IsFriendByPlayerNumber(PI.PlayerNumber, CR[ CR[id].AttackMeID.ID ].PlayerNumber)==FALSE)
				{

					attackid=CR[id].AttackMeID.ID;
				}
				else if(CR[id].FireSwitch==TRUE 
			    &&  IsSee(CR[id].FireID.ID) 
			    && IsFriendByPlayerNumber(PI.PlayerNumber, CR[ CR[id].FireID.ID ].PlayerNumber)==FALSE)
				{
					attackid=CR[id].FireID.ID;
				}
			}

			// 도움이 필요한 캐릭터를 찾았다. 
			if(attackid)
			{
				// 아군을 괴롭히는 캐릭터를 찾는다. 
				
				// 공격받는 캐릭터가 반격 능력이 있는지 확인한다. 
				if( CR[id].CanAttack(CR[attackid].NewID) )
				{
					 // 반격 능력이 있다면 10
					 attack_score=10;
				}
				// 반격 능력이 없다면 
				else
				{
					attack_score=30;
				}


				// 아군 기지와의 거리를 구한다. 
				// 거리가 가까우면 점수가 높다. (40%)
				length=FindLengthToNearestBase(CR[id].X, CR[id].Y);
				
				if(length<0)length_score=0;
				else
				  length_score=(MAX_MAP_XSIZE-length)*40/100;


				// 랜덤 접수 30
				random_score=GD.Random()%30;

				total_score=attack_score+length_score+random_score;

				if(total_score>max_score)
				{
				   max_score=total_score;
				   HelpAttackTargetID=CR[attackid].NewID;
				}
			}
		}

		// 도와야할 캐릭터가 있다면, 
		if(HelpAttackTargetID.IsEmpty()==FALSE)
		{
	       PlayerStep=PS_DECIDE_REINFORCEMENT;
		}
		else
		{
			ComWait();
			return;
		}

		break;

	case PS_DECIDE_REINFORCEMENT:

		if(IsSee(HelpAttackTargetID.ID)==FALSE)
		{
			ComWait();
			return ;
		}

		// 지원군을 파견한다.
		maxid=0;
		max_score=0;
		for(i=0;i<CharNumber;i++)
		{
			id=CharID[i].ID;

			if(IsSee(id) && CR[id].CanAttack(HelpAttackTargetID)
			&& CR[id].CurrentOrder!=ORDER_ATTACK)
			{
				// 거리가 가까워야 한다. 
				length=max(abs(CR[id].GetX()-CR[HelpAttackTargetID.ID].GetX()),
					       abs(CR[id].GetY()-CR[HelpAttackTargetID.ID].GetY())  );

				// 거리 점수 60점 
				if(length>13)continue;
				length_score=(MAX_MAP_XSIZE-length)*60/100;


				// 랜덤 점수 40점
				random_score=GD.Random()%40;

				total_score=length_score+random_score;

				if(total_score>max_score)
				{
					max_score=total_score;
					maxid=id;
				}
			}
		}
			

		if(maxid)
		{
			OrderMove(CR[maxid].NewID, CR[HelpAttackTargetID.ID].GetX(), 
				               CR[HelpAttackTargetID.ID].GetY(), BY_COMPUTER);
		}

		ComWait();
		break;

	}

}


