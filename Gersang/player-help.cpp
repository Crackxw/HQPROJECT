#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
	#include <etc.h>
#endif

#include <map.h>
#include "char.h"

extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 
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
	// ������ �ʿ�� �ϴ� ĳ���͸� ã�´�. 
	case PS_DECIDE_NEED_HELP_CHAR:

		HelpAttackTargetID.Init();
		max_score=0;


		for(i=0;i<CharNumber;i++)
		{
			id=CharID[i].ID;

			// ������ �ʿ��� ĳ���͸� ã�Ҵ�. 
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

			// ������ �ʿ��� ĳ���͸� ã�Ҵ�. 
			if(attackid)
			{
				// �Ʊ��� �������� ĳ���͸� ã�´�. 
				
				// ���ݹ޴� ĳ���Ͱ� �ݰ� �ɷ��� �ִ��� Ȯ���Ѵ�. 
				if( CR[id].CanAttack(CR[attackid].NewID) )
				{
					 // �ݰ� �ɷ��� �ִٸ� 10
					 attack_score=10;
				}
				// �ݰ� �ɷ��� ���ٸ� 
				else
				{
					attack_score=30;
				}


				// �Ʊ� �������� �Ÿ��� ���Ѵ�. 
				// �Ÿ��� ������ ������ ����. (40%)
				length=FindLengthToNearestBase(CR[id].X, CR[id].Y);
				
				if(length<0)length_score=0;
				else
				  length_score=(MAX_MAP_XSIZE-length)*40/100;


				// ���� ���� 30
				random_score=GD.Random()%30;

				total_score=attack_score+length_score+random_score;

				if(total_score>max_score)
				{
				   max_score=total_score;
				   HelpAttackTargetID=CR[attackid].NewID;
				}
			}
		}

		// ���;��� ĳ���Ͱ� �ִٸ�, 
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

		// �������� �İ��Ѵ�.
		maxid=0;
		max_score=0;
		for(i=0;i<CharNumber;i++)
		{
			id=CharID[i].ID;

			if(IsSee(id) && CR[id].CanAttack(HelpAttackTargetID)
			&& CR[id].CurrentOrder!=ORDER_ATTACK)
			{
				// �Ÿ��� ������� �Ѵ�. 
				length=max(abs(CR[id].GetX()-CR[HelpAttackTargetID.ID].GetX()),
					       abs(CR[id].GetY()-CR[HelpAttackTargetID.ID].GetY())  );

				// �Ÿ� ���� 60�� 
				if(length>13)continue;
				length_score=(MAX_MAP_XSIZE-length)*60/100;


				// ���� ���� 40��
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


