//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>

#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
#include <etc.h>
#include <CharOrg.h>

#include <Map.h>
#include <mapinterface.h>
#include <Charinterface.h>

extern	_KindInfo		KI[];
extern	_MainData		GD;
extern	SHORT			MapEditMode;
extern	cltMapInterface clifMap;          // �ʰ��� �������̽� 
extern	cltCharInterface clifChar; 
extern	_Char			CR[];
extern	_Map			Map;

BOOL _Char::OperateWait()
{
	SI32 aniindex = 0;

	int i = 0, j = 0, k = 0;
	SHORT id = 0;

	// ȭ��� ��ȿ�� ��쿡�� �۵��Ѵ�. 
	if(m_scSeeSwitch==FALSE)return FALSE;

	// ������ ������ �ִ� ĳ���ʹ� ��ġ�� ������ �ʴ´�.
	// ���ۿ� ������� ������ �ִϸ� ���� ������ �ȵȴ�.
	if(IsAtb(ATB_ABSANIWAIT) == FALSE)
	{
		
		// ���� �� �����ÿ��� ��� �������� �ִϸ��̼�����
		if(SwimSwitch == TRUE && KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_SWIM))
		{
			SetAnimation(ANI_SWIM_WAIT);
			DoAnimationDelay(TRUE);
		}
		else
		{
			// ������ �ִϸ��̼��� �����ش�. 
			m_cltCharDrawInfo.SetLastAniFrame(GD.Frame);
			
			//  �ִϸ��̼� Ÿ���� �����Ѵ�. 
			if(NextWaitType)
			{
				WaitType = NextWaitType;
				NextWaitType = 0;
			}

			// �ִϸ��̼��� ������ �����ӿ� �����ߴٸ�, 
			SI32 lastframe;
			if(WaitType == ANI_WAIT0)
			{
				lastframe = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ Wait0CharAnimationIndex ].GetFrameNumber() ;
			}
			else if(WaitType == ANI_WAIT1)
			{
				lastframe = KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ Wait1CharAnimationIndex ].GetFrameNumber() ;
			}
			else
			{
				clGrp.Error("fd9261", "WaitType:%d", WaitType );
			}


			if(m_cltCharDrawInfo.GetAniStep() == 0 )
			{
				if(WaitAnimationChangedSwitch == FALSE)
				{
					WaitAnimationChangedSwitch = TRUE;
					// ���� �ִϸ��̼��� �پ��ϰ� �����Ѵ�.
					NextWaitType = ANI_WAIT0;
					
					if((GD.Rand + m_NewID.ID) % 2)
					{
						if(Wait1CharAnimationIndex)
						{
							NextWaitType	= ANI_WAIT1;
						}
					}
				}
			}
			else
			{
				WaitAnimationChangedSwitch = FALSE;
			}

			SetAnimation(WaitType);
			DoAnimationDelay(TRUE);
			
		}
	}


	// ����� ����� ������ ó���Ѵ�.
	if(ReserveOrderIndex)
	{
		PopReserveOrder();
		return TRUE;
	}

	// ���� ���� ��尡 �۵����̸�, �����Ѵ�. 
	if(MapEditMode==TRUE)return FALSE;

	// �ڵ� �ֺ� ġ�� ��� 

//	if((GD.Rand + NewID.ID) % 100==0)
  // {   
	if(HealAI()==TRUE)
		return TRUE;
//	}

	// ���� �۾��� ���� ó�� ��ƾ 
	OldOperation();
	
	// �̵��ݾ��� �Ʒ� ��ƾ�� �����Ѵ�. 
	if(GD.Frame % 20)return FALSE;

	/*
	// ���ڸ� �ż��� �� �ִ� ��� �ֺ��� ���ڸ� �ż��Ѵ�. 
	if( CanMine()==TRUE && GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER )
	{
		// �¶����� ���� ���� �ð� ������ �ξ ���ڸ� �ż��Ѵ�.
		if( GD.Random() % 3 == 0)
		{
			
			SI32 x, y;

			// �ֺ� ������ ��ĵ�Ͽ� ���ڸ� ���� ���� �ִ��� Ȯ���Ѵ�. 
			if(SearchForMineSpot(&x, &y) == TRUE)
			{
				OrderMine(NewID, x, y, BY_COMPUTER);
				return TRUE;
			}
		}
	}
	*/
	

	// �ڽ��� �ڸ��� ���� ������ ���ڸ��� ���Ѵ�.
	if(HoldSwitch == FALSE && Map.GetFireMap(m_ssX, m_ssY) )
	{
		int six, siy;

		if(FindNearFireNo(&six, &siy) == TRUE)
		{
			OrderMove(m_NewID, six, siy, BY_COMPUTER);
			return TRUE;
		}
	}



	// �¶��� ���ӿ��� ���ʹ� �׻� ���� ã�Ƽ� �������� �Ѵ�. 
	if(GD.Player[GetPlayerNumber()].PI.Host != BY_PLAYER || m_bGuardianChar == TRUE)
	{

		if(GD.Random() % 5)return FALSE;

		SI32 id;
		SI32 list[100];
		SI32 listindex = 0;
		for(i = 0;i<GD.CharOrderIndex;i++)
		{
			id = GD.CharOrder[i];
			
			if(IsAlive(id) && IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id].GetPlayerNumber()) == FALSE)
			{
				if(listindex < 100)
				{
					list[listindex] = id;
					listindex ++;
				}
			}
		}

		if(listindex)
		{
			id = list[GD.Random() % listindex];

			OrderMove(m_NewID, CR[id].GetX(), CR[id].GetY(), BY_COMPUTER);
		}
	}
	return TRUE;
}


