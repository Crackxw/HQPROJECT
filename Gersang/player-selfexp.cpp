#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
	#include <etc.h>
#endif

#include "char.h"

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Char				CR[];								// 캐릭터의 포인터 


void _Player::ComSelfExp()
{

	SHORT i;
	SHORT length;
	SHORT min_length;
	SHORT min_id;
	SHORT satid;
	SHORT id;


	switch(PlayerStep)
	{
	// 공격시킬 위성을 찾는다. 
	case PS_DECIDESAT:

		// 위성이 존재하지 않으면, 
		if(GD.CharNumber[PI.PlayerNumber][KIND_SATJ]==0)
		{
			ComWait();
			return ;
		}


		ListIndex=0;
		for(i=0;i<CharNumber;i++)
		{
			id=CharID[i].ID;

			if(GD.CharInfo[id] && CR[id].CanSelfExp()==TRUE && CR[id].CurrentOrder==ORDER_WAIT)
			{
				PushList(id);
			}
		}


		// 공격시킬 위성을 찾았다. 
		if(ListIndex>0)
		{
			Para[0]=(SHORT)List[GD.Random()%ListIndex];
			PlayerStep=PS_DECIDEATTACKAREA;
		}
		else
		{
			ComWait();
		}
		break;

	case PS_DECIDEATTACKAREA:
		satid=Para[0];

		// 위성이 유효하지 않으면, 
		if(IsSee(satid)==FALSE || CR[satid].CanSelfExp()==FALSE)
		{
			ComWait();
			return ;
		}

		// 가까운 곳에 있는 적 캐릭터를 찾아 낸다. 
		min_length=10000;
		min_id=0;
		for(i=0;i<GD.CharOrderIndex;i++)
		{

			id=GD.CharOrder[i];

			// 건물 캐릭터만 찾는다. 
			if(IsSee(id) && IsFriendByPlayerNumber(PI.PlayerNumber, CR[id].PlayerNumber)==FALSE
			&& CR[id].IsAtb(ATB_BUILDING) )
			{

				length=max( abs(CR[satid].GetX()- CR[id].GetX()), 
					        abs(CR[satid].GetY()- CR[id].GetY()) );

				if(length<min_length)
				{
					min_length=length;
					min_id=id;
				}
			}
		}

		if(min_id)
		{
			OrderSelfExp(CR[satid].NewID, CR[min_id].GetX(), CR[min_id].GetY(), BY_COMPUTER);
		}

		ComWait();
		break;
	default:
		break;
	}
}