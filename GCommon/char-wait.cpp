//--------------------------------------------------------------------
//  담당 : 김태곤
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
extern	cltMapInterface clifMap;          // 맵관련 인터페이스 
extern	cltCharInterface clifChar; 
extern	_Char			CR[];
extern	_Map			Map;

BOOL _Char::OperateWait()
{
	SI32 aniindex = 0;

	int i = 0, j = 0, k = 0;
	SHORT id = 0;

	// 화면상에 유효한 경우에만 작동한다. 
	if(m_scSeeSwitch==FALSE)return FALSE;

	// 정지시 가만히 있는 캐릭터는 조치를 취하지 않는다.
	// 동작에 관계없이 일정한 애니를 갖는 유닛은 안된다.
	if(IsAtb(ATB_ABSANIWAIT) == FALSE)
	{
		
		// 수영 중 정지시에도 계속 수영중인 애니메이션으로
		if(SwimSwitch == TRUE && KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_SWIM))
		{
			SetAnimation(ANI_SWIM_WAIT);
			DoAnimationDelay(TRUE);
		}
		else
		{
			// 정지시 애니메이션을 보여준다. 
			m_cltCharDrawInfo.SetLastAniFrame(GD.Frame);
			
			//  애니메이션 타입을 변경한다. 
			if(NextWaitType)
			{
				WaitType = NextWaitType;
				NextWaitType = 0;
			}

			// 애니메이션이 마지막 프레임에 도착했다면, 
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
					// 정지 애니메이션을 다양하게 설정한다.
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


	// 예약된 명령이 있으면 처리한다.
	if(ReserveOrderIndex)
	{
		PopReserveOrder();
		return TRUE;
	}

	// 지도 편집 모드가 작동중이면, 리턴한다. 
	if(MapEditMode==TRUE)return FALSE;

	// 자동 주변 치료 기능 

//	if((GD.Rand + NewID.ID) % 100==0)
  // {   
	if(HealAI()==TRUE)
		return TRUE;
//	}

	// 이전 작업에 대한 처리 루틴 
	OldOperation();
	
	// 이따금씩만 아래 루틴을 수행한다. 
	if(GD.Frame % 20)return FALSE;

	/*
	// 지뢰를 매설할 수 있는 경우 주변에 지뢰를 매설한다. 
	if( CanMine()==TRUE && GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER )
	{
		// 온라인일 때는 좀더 시간 간격을 두어서 지뢰를 매설한다.
		if( GD.Random() % 3 == 0)
		{
			
			SI32 x, y;

			// 주변 영역을 스캔하여 지뢰를 심을 곳이 있는지 확인한다. 
			if(SearchForMineSpot(&x, &y) == TRUE)
			{
				OrderMine(NewID, x, y, BY_COMPUTER);
				return TRUE;
			}
		}
	}
	*/
	

	// 자신의 자리에 불이 붙으면 옆자리로 피한다.
	if(HoldSwitch == FALSE && Map.GetFireMap(m_ssX, m_ssY) )
	{
		int six, siy;

		if(FindNearFireNo(&six, &siy) == TRUE)
		{
			OrderMove(m_NewID, six, siy, BY_COMPUTER);
			return TRUE;
		}
	}



	// 온라인 게임에서 몬스터는 항상 적을 찾아서 움직여야 한다. 
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


