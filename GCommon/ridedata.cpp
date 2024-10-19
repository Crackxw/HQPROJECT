//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>
#include "..\Gersang\maindata.h"
#include <etc.h>
#include "..\Gersang\music.h"
#include <CharOrg.h>
#include <charinterface.h>
#include <findpath5.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

extern	_Char				CR[];
extern	_MainData			GD;
extern	cltCharInterface	clifChar;         // 캐릭터 관련 인터페이스 


void _RideData::Init(_NewID id)
{

	ZeroMemory(this, sizeof(_RideData));

	ID=id;
	MaxCrewNumber=0;
	CrewNumber=0;
}



BOOL _RideData::RideOn(_NewID id)
{
	SHORT i;

	// 화면에 보이는 상태여야 태울 수 있다. 
	if(CR[id.ID].m_scSeeSwitch == FALSE)return FALSE;


	// 이미 탑승하고 있는 유닛도 태울 수 없다. 
	for(i = 0;i<GetMaxCrewNumber();i++)
	{
		if(id.IsSame(Crew[i]))
		{
			return FALSE;
		}
	}

	if(CrewNumber>=MaxCrewNumber)return FALSE;

	for(i=0;i<MaxCrewNumber;i++)
	{
		if(Crew[i].IsEmpty())
		{
			Crew[i]=id;
			CrewNumber++;
		    return TRUE;
		}
	}
	return FALSE;
}


void _RideData::RideOut(SHORT number)
{
	SHORT i;
	SHORT id;
	SHORT x, y;
	SHORT start, end;

	if(CrewNumber<=0)return ;

	// 모든 유닛을 내려 놓는 것이라면, 
	if(number==-1)
	{
		start=0;
		end=MAX_CREW_NUMBER-1;
	}
	// 특정 유닛만 선택적으로 내려 놓는 것이면, 
	else
	{
		start=number;
		end=number;
	}

	// 빈자리를 찾아서 캐릭터를 내놓는다. 
    for(i=start;i<=end;i++)
	{
		// 빈자리는 체크하지 않는다. 
	   if(Crew[i].IsEmpty())continue;

	   // 유효한 승무원인지 확인한다. 
	   id=Crew[i].ID;
	   if(GD.CharInfo[id]==0)
	   {
	  	   Crew[i].Init();
		   continue;
	   } 
		
	   // 승무원을 내릴 빈 자리가 있는지 확인한다. 
	   if(CR [ID.ID].FindEmptyArea(x, y, CR[id].m_scIDXsize, CR[id].m_scIDYsize)==TRUE)
	   {

		   // 완전히 어두운 곳에 있지 않아야 소리가 난다. 
           if(GD.IsInBlackFog(CR[ID.ID].m_ssX, CR[ID.ID].m_ssY)==FALSE)
              PushEffect(EFFECT_RIDEOUT, clifChar.GetX(ID.ID), clifChar.GetY(ID.ID));

		   // 내린 승무원은 초기화 한다. 
			Crew[i].Init();
			CrewNumber--;
			CR[id].m_ssX=x;
			CR[id].m_ssY=y;
			CR[id].SetSeeSwitch(TRUE);
			CR[id].SetID();
            CR[id].OperateMapPosition(x, y);
			
			OrderStop(CR[id].m_NewID, BY_COMPUTER);
		}
	}

	// 탑승 인원을 재정비한다. 
	CrewNumber=0;
	for(i=0;i<MAX_CREW_NUMBER;i++)
	{
		if(Crew[i].IsEmpty())continue;

		id=Crew[i].ID;
		if(GD.CharInfo[id]==0)
		{
			Crew[i].Init();
			continue;
		}

		CrewNumber++;
	}

}

BOOL _RideData::CanRide()
{
	if(CrewNumber<MaxCrewNumber)return TRUE;

	return FALSE;
}


SHORT _RideData::GetCrewNumber()
{
	return CrewNumber;
}

// 총을 가진 탑승 유닛의 수를 구한다. 
SHORT _RideData::GetGunUnitCrewNumber()
{
	SHORT count =0;


	int i;
	SHORT id;

	for(i=0;i<MAX_CREW_NUMBER;i++)
	{
		if(Crew[i].IsEmpty())continue;
		
		id=Crew[i].ID;
		if( GD.CharInfo[id]!=0 
		&& (CR[id].IsAttackAtb(ATTACKATB_GUN) || CR[id].IsAttackAtb(ATTACKATB_STRONGGUN)) )
		{
			count++;
		}
	}


	return count;
}

// 특정 종류 유닛의 수를 구한다. 
SHORT _RideData::GetUnitNumberByKind(SI32 siKind)
{
	int i;
	SHORT id;
	SHORT count =0;

	for(i=0;i<MAX_CREW_NUMBER;i++)
	{
		if(Crew[i].IsEmpty())continue;
		
		id=Crew[i].ID;
		if( GD.CharInfo[id]!=0 	&& CR[id].m_cltCharStatus.GetKind() == siKind)
		{
			count++;
		}
	}

	return count;
}

SHORT _RideData::GetMaxCrewNumber()
{
	return MaxCrewNumber;
}

// 승무원들의 위치를 보정해준다. 
void _RideData::SetCrewPosition(SHORT x, SHORT y)
{
	SHORT i;
	SHORT id;

	for(i=0;i<MAX_CREW_NUMBER;i++)
	{
		if(Crew[i].IsEmpty())continue;

		id=Crew[i].ID;
		if(GD.CharInfo[id]!=0)
		{
			CR[id].m_ssX=x;
			CR[id].m_ssY=y;
		}
	}

}

// 특정 지점으로 이동하여 병력을 투하한다.
void _RideData::MoveRideOut()
{
	SHORT status;
	SHORT destx=LOWORD(CR[ID.ID].Para1);
	SHORT desty=HIWORD(CR[ID.ID].Para1);

	switch(CR[ID.ID].WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:
		
		// 목표지점으로 이동시킨다. 
		CR[ID.ID].SetMove(destx, desty);
		CR[ID.ID].WorkStep=WS_RIDEOUT;
		break;


	case WS_RIDEOUT:

		status=CR[ID.ID].Move(BASIC_MOVE_BLOCK_COUNT);

	   if(CR[ID.ID].MovePercent==0)
          CR[ID.ID].m_scBusySwitch=FALSE;
	   else 
	 	  CR[ID.ID].m_scBusySwitch=TRUE;

	   // 목적지에 도착하면 
	   if(status==1)
	   {
		   // 병력을 투하한다. 
		   RideOut(-1);
		   OrderStop(ID, BY_COMPUTER);
	   }
		break;
	default:
		break;
	}
}


// 특정 지점으로 이동하여 병력을 투하하고 특정 캐릭터를 공격한다.
void _RideData::MoveRideOutAttack()
{
	SHORT i;
	SHORT status;
	SHORT temp_id[MAX_CREW_NUMBER];
	SHORT destx=LOWORD(CR[ID.ID].Para1);
	SHORT desty=HIWORD(CR[ID.ID].Para1);
	_NewID attackid=CR[ID.ID].Para2;

	switch(CR[ID.ID].WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:
		
		// 목표지점으로 이동시킨다. 
		CR[ID.ID].SetMove(destx, desty);
		CR[ID.ID].WorkStep=WS_RIDEOUT;
		break;


	case WS_RIDEOUT:
		status=CR[ID.ID].Move(BASIC_MOVE_BLOCK_COUNT);

	   if(CR[ID.ID].MovePercent==0)
          CR[ID.ID].m_scBusySwitch=FALSE;
	   else 
	 	  CR[ID.ID].m_scBusySwitch=TRUE;

	   // 목적지에 도착하면 
	   if(status==1)
	   {
		   // 병력을 투하하기 전에 탑승인원들의 아이디를 구한다. 
		   for(i=0;i<MAX_CREW_NUMBER;i++)
		   {
			   temp_id[i]=0;
		   }

		   for(i=0;i<MAX_CREW_NUMBER;i++)
		   {
			   temp_id[i]=Crew[i].ID;
		   }

		   // 병력을 투하한다. 
		   RideOut(-1);
		   OrderStop(ID, BY_COMPUTER);

		   // 투하한 병력들은 공격을 명한다. 
		   if(IsSee(attackid))
		   {
		       for(i=0;i<MAX_CREW_NUMBER;i++)
			   {
			       if(IsSee(temp_id[i]))
				   {
				      OrderMove(CR[ temp_id[i] ].m_NewID, CR[attackid.ID].m_ssX, CR[attackid.ID].m_ssY, BY_COMPUTER);
				   }
			   }
		   }

	   }
		break;
	default:
		break;
	}
}


