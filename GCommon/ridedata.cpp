//--------------------------------------------------------------------
//  ��� : ���°�
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
extern	cltCharInterface	clifChar;         // ĳ���� ���� �������̽� 


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

	// ȭ�鿡 ���̴� ���¿��� �¿� �� �ִ�. 
	if(CR[id.ID].m_scSeeSwitch == FALSE)return FALSE;


	// �̹� ž���ϰ� �ִ� ���ֵ� �¿� �� ����. 
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

	// ��� ������ ���� ���� ���̶��, 
	if(number==-1)
	{
		start=0;
		end=MAX_CREW_NUMBER-1;
	}
	// Ư�� ���ָ� ���������� ���� ���� ���̸�, 
	else
	{
		start=number;
		end=number;
	}

	// ���ڸ��� ã�Ƽ� ĳ���͸� �����´�. 
    for(i=start;i<=end;i++)
	{
		// ���ڸ��� üũ���� �ʴ´�. 
	   if(Crew[i].IsEmpty())continue;

	   // ��ȿ�� �¹������� Ȯ���Ѵ�. 
	   id=Crew[i].ID;
	   if(GD.CharInfo[id]==0)
	   {
	  	   Crew[i].Init();
		   continue;
	   } 
		
	   // �¹����� ���� �� �ڸ��� �ִ��� Ȯ���Ѵ�. 
	   if(CR [ID.ID].FindEmptyArea(x, y, CR[id].m_scIDXsize, CR[id].m_scIDYsize)==TRUE)
	   {

		   // ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
           if(GD.IsInBlackFog(CR[ID.ID].m_ssX, CR[ID.ID].m_ssY)==FALSE)
              PushEffect(EFFECT_RIDEOUT, clifChar.GetX(ID.ID), clifChar.GetY(ID.ID));

		   // ���� �¹����� �ʱ�ȭ �Ѵ�. 
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

	// ž�� �ο��� �������Ѵ�. 
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

// ���� ���� ž�� ������ ���� ���Ѵ�. 
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

// Ư�� ���� ������ ���� ���Ѵ�. 
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

// �¹������� ��ġ�� �������ش�. 
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

// Ư�� �������� �̵��Ͽ� ������ �����Ѵ�.
void _RideData::MoveRideOut()
{
	SHORT status;
	SHORT destx=LOWORD(CR[ID.ID].Para1);
	SHORT desty=HIWORD(CR[ID.ID].Para1);

	switch(CR[ID.ID].WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:
		
		// ��ǥ�������� �̵���Ų��. 
		CR[ID.ID].SetMove(destx, desty);
		CR[ID.ID].WorkStep=WS_RIDEOUT;
		break;


	case WS_RIDEOUT:

		status=CR[ID.ID].Move(BASIC_MOVE_BLOCK_COUNT);

	   if(CR[ID.ID].MovePercent==0)
          CR[ID.ID].m_scBusySwitch=FALSE;
	   else 
	 	  CR[ID.ID].m_scBusySwitch=TRUE;

	   // �������� �����ϸ� 
	   if(status==1)
	   {
		   // ������ �����Ѵ�. 
		   RideOut(-1);
		   OrderStop(ID, BY_COMPUTER);
	   }
		break;
	default:
		break;
	}
}


// Ư�� �������� �̵��Ͽ� ������ �����ϰ� Ư�� ĳ���͸� �����Ѵ�.
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
		
		// ��ǥ�������� �̵���Ų��. 
		CR[ID.ID].SetMove(destx, desty);
		CR[ID.ID].WorkStep=WS_RIDEOUT;
		break;


	case WS_RIDEOUT:
		status=CR[ID.ID].Move(BASIC_MOVE_BLOCK_COUNT);

	   if(CR[ID.ID].MovePercent==0)
          CR[ID.ID].m_scBusySwitch=FALSE;
	   else 
	 	  CR[ID.ID].m_scBusySwitch=TRUE;

	   // �������� �����ϸ� 
	   if(status==1)
	   {
		   // ������ �����ϱ� ���� ž���ο����� ���̵� ���Ѵ�. 
		   for(i=0;i<MAX_CREW_NUMBER;i++)
		   {
			   temp_id[i]=0;
		   }

		   for(i=0;i<MAX_CREW_NUMBER;i++)
		   {
			   temp_id[i]=Crew[i].ID;
		   }

		   // ������ �����Ѵ�. 
		   RideOut(-1);
		   OrderStop(ID, BY_COMPUTER);

		   // ������ ���µ��� ������ ���Ѵ�. 
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


