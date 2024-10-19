//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>

#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
#include <etc.h>
#include <CharOrg.h>

#include <map.h>
#include <mapinterface.h>
#include <SPrg.h>
#include <findpath5.h>


extern	_Map			Map;
extern	_FindPathGame  *FindPathGame;
extern	_MainData		GD;
// 지도의 각 좌표마다의 중심값, 
extern	int				MapCenterX[][MAX_MAP_YSIZE];
extern	int				MapCenterY[][MAX_MAP_YSIZE];
extern	cltMapInterface clifMap;          // 맵관련 인터페이스 
extern _KindInfo KI[];

// 캐릭터를 이동하게 만든다. 
BOOL cltCharOrg::SetMove(SHORT dx, SHORT dy)
{
	if(dx<0)dx=0;
	if(dx>=Map.MH.MapXsize)dx=Map.MH.MapXsize-1;
    if(dy<0)dy=0;
 	if(dy>=Map.MH.MapYsize)dy=Map.MH.MapYsize-1;

	DX=dx; DY=dy;

	MX=m_ssX; MY=m_ssY;
	MoveBlockDelay=0;
	MoveBlockCount=0;
	
	MoveBlockX=-1;
	MoveBlockY=-1;

   // 자리를 이동하면 자리 사수를 해제한다. 
   HoldSwitch = FALSE;

	return TRUE;
}



int cltCharOrg::Move(SHORT moveblockcount, SHORT range)
{
	if(IsAtb(ATB_FLY)==TRUE)
	{
		return MoveFly(moveblockcount, range);
	}
	else
	{
		return MoveNormal(moveblockcount, range);
	}
}

// 이동 루틴 
// 리턴값: 1 -> 목적지 도착 
int cltCharOrg::MoveNormal(SHORT moveblockcount, SHORT range)
{
	int x=m_ssX, y=m_ssY;
	int direction;
	int rx=RX, ry=RY;
	SI32 aniindex = 0;
	SHORT moveswitch=FALSE;
	

	// 이동전 조치가 완료되면 이동, 
	// 아니면 조치 
	if(IsPreMoveOk()==FALSE)
	{
		return MOVEINFO_MOVING;
	}
	
	// 이동중 걸려서 지연이 생기면, 
	if(MoveBlockDelay>0)
	{
		// 충돌이 해소 되었다면, 
		if(MoveBlockX>=0 && IsColl(MoveBlockX, MoveBlockY, Block|BLOCK_MOVE)==FALSE && Map.IsColl(MoveBlockX, MoveBlockY, BLOCK_ICEWALL)==FALSE)
		{
			MoveBlockX=-1;
			MoveBlockY=-1;
			
			MoveBlockDelay=0;
		}
		// 계속 충돌된 상태로 있다면, 
		else
		{
			MoveBlockDelay--;
			
			// 기다릴대로 기다린 후에는 다른 길로 가는 방법을 모색한다. 
			if(MoveBlockDelay==0 && MovePercent==0)
			{
				FindAnotherPathSwitch=TRUE;
			}
			
			// 애니메이션이 완료된 것이 아니라면, 
			if(Step!=0)
			{
				if(LowMoveSwitch==TRUE)
				{
					SetAnimation(ANI_LOWMOVE);
					DoAnimationDelay(TRUE);
				}
				else
				{
					if(SwimSwitch == TRUE)
					{
						SetAnimation(ANI_SWIM);
						DoAnimationDelay(TRUE);
					}
					else
					{
						
						SetAnimation(ANI_MOVE);
						DoAnimationDelay(TRUE);
					}
				}
			}
			
			return MOVEINFO_MOVING;
		}
	}
	
	
	// 중간지점에 도착한 것이라면 
	if(MX==m_ssX && MY==m_ssY && MovePercent==0)
	{
		// 임시 버퍼에 충돌하는 데이터를 저장한다. 
		SHORT temp_block=Block;
		
		// 다른 경로를 찾게 만드는 설정이 되어 있고 '이동 유닛 충돌'이 되어 있지 않다면, 
		if(FindAnotherPathSwitch==TRUE && (Block&BLOCK_MOVE)==0)
		{
			// 다른 경로를 찾도록 만드는 것을 해제한다. 
			FindAnotherPathSwitch=FALSE;
			
			// 충돌속성에 이동 유닛 충돌까지 포함한다. 
			Block+=BLOCK_MOVE;
		}
		
		SHORT returnval = FindPathGame->FindPath(m_ssX, m_ssY, DX, DY, MX, MY, range, m_NewID.ID);
		
		// 원래의 충돌 속성을 복구한다. 
		Block=temp_block;
	
		// 너무 많은 유닛이 움직여서 추가로 움직일 수 없다. 
		if(returnval==-1)
		{
			return MOVEINFO_MOVING;
		}
		
		if(MX==m_ssX && MY==m_ssY)
		{
			MoveBlockDelay=5;
			MoveBlockCount++;
			if(MoveBlockCount>=2)
			{
				return MOVEINFO_MOVEDONE;
			}
			
			else return MOVEINFO_MOVING;
		}
		else 
		{
			MoveDelayor=0;
			MoveBlockDelay=0;
			MoveBlockCount=0;
		}
	}
	
	// 이동 방향을 초기화한다. 
	direction=0;
	
	// 이동할 위치를 찾는다. 
	NX=m_ssX, NY=m_ssY;
	if(m_ssX<MX)
	{
		NX++;
		direction|=EAST;
	}
	else if(m_ssX>MX)
	{
		NX--;
		direction|=WEST;
	}
	if(m_ssY<MY)
	{
		NY++;
		direction|=SOUTH;
	}
	else if(m_ssY>MY)
	{
		NY--;
		direction|=NORTH;
	}

	// 실제 이동하기 전에 걸리면, 
	// 잠시 대기했다가 다시 이동 시킨다. 
	// 여러번 걸리면 멈춘다. 

	if(IsColl(NX, NY, Block|BLOCK_MOVE) == TRUE || Map.IsColl(NX, NY, BLOCK_ICEWALL))
	{
		MoveBlockX=NX;
		MoveBlockY=NY;
		
		NX=m_ssX;
		NY=m_ssY;
		
		// 오래 기다리면 이동 완료 
		MoveBlockDelay=5;
		MoveBlockCount++;

		if(MoveBlockCount>=moveblockcount)
		{
			SetMove(DX, DY);
		}
		
		return MOVEINFO_MOVING;
	}
	
	MoveBlockCount=0;
	
	// 방향을 전환한다. 
	if(direction)
	{
		if(ChangeDirection(direction)==FALSE)return MOVEINFO_MOVING;
	}
	
	SHORT movespeed;
	
	// 언덕을 오르는 중이면 속도를 낮춘다. 
	// 얕은   물에서도      속도를 낮춘다. 
	if(ClimbSwitch==TRUE || Map.GetShallowMap(m_ssX, m_ssY)==TRUE)
	{
		movespeed=MoveSpeed-MoveSpeed/3;
	}
	// 포복중이면 속도를 낮춘다. 
	else if(LowMoveSwitch==TRUE)
	{
		movespeed=MoveSpeed-MoveSpeed/2;
	}
	// 수영중이면 속도를 낮춘다. 
	else if(SwimSwitch==TRUE)
	{
		movespeed=MoveSpeed-MoveSpeed/2;
	}
	else
		movespeed=MoveSpeed;
	
	if(MovePercent<0)
	{
		MovePercent+=movespeed;
		if(MovePercent>0)MovePercent=0;
	}
	else
		MovePercent+=movespeed;
	
	if(MovePercent>=50)
	{
		MovePercent=MovePercent-100;
		
		// 이동의 기준을 만든다.
		MOX=m_ssX;
		MOY=m_ssY;
		MDX=NX;
		MDY=NY;
		
		m_ssX=NX;
		m_ssY=NY;
		
		// 실제 이동했음을 알려주는 변수 
		moveswitch=TRUE;
		
		// 지도 위치 x,y에서 이루어지는 조치를 취한다. 
		OperateMapPosition(m_ssX, m_ssY);
	}
	// 좌표 전환이 있은지 얼마 동안은, 
	else if(MovePercent<=0)
	{
	}
	else
	{
		// 이동의 기준을 만든다.
		MOX=m_ssX;
		MOY=m_ssY;
		MDX=NX;
		MDY=NY;
	}
	
	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. 
	SHORT centerx, centery;
	Map.GetCenterXYByMapCenterXY(MOX, MOY, centerx, centery);
	
	// 이동할 위치의 중심을 찾는다. 
	SHORT nx_centerx, ny_centery;
	Map.GetCenterXYByMapCenterXY(MDX, MDY, nx_centerx, ny_centery);
	
	//지금 좌표와 다음 좌표간의 백분율 비 변위를 구한다.
	if(MovePercent>=0)
		GetMoveRatio(centerx, centery, nx_centerx, ny_centery, MovePercent, RX, RY);
	else
	{
		GetMoveRatio(nx_centerx, ny_centery, centerx, centery, abs(MovePercent), RX, RY);
	}
	
	// 경사지인지 아닌지 확인한다. 
	if(Map.GetLevelInfo(m_ssX,m_ssY)==HILL_SLOPE)
	{
		ClimbSwitch=TRUE;
	}
	else ClimbSwitch=FALSE;  
	
    // 포복이 가능한 캐릭터이면
    if(IsAtb(ATB_LOWMOVE))
    {
		if(MovePercent==0 )
		{
			if( BeAttackedSwitch==TRUE && IsFriendToByID(m_NewID.ID, AttackMeID.ID) == FALSE
				&& Map.GetShallowMap(m_ssX, m_ssY) == FALSE)
			{
				LowMoveSwitch=TRUE;
			}
			else
				LowMoveSwitch=FALSE;
		}
    }
	else
		LowMoveSwitch=FALSE;
	
	
	
	// 걸려있지 않거나 첫번째 프레임이 아니면,
	if(LowMoveSwitch==TRUE)
	{
		SetAnimation(ANI_LOWMOVE);
		DoAnimationDelay(TRUE);
	}
	else
	{
		if(SwimSwitch == TRUE)
		{
			SetAnimation(ANI_SWIM);
			DoAnimationDelay(TRUE);
		}
		else
		{
			SetAnimation(ANI_MOVE);
			DoAnimationDelay(TRUE);
		}
	}
	
	
	// 중간좌표까지 이상없이 도착
	if(MovePercent==0)
	{
		// 최종 목적지 도착 
		if(DX==m_ssX && DY==m_ssY)
		{
			return MOVEINFO_MOVEDONE;
		}
		// 중간 목적지 도착 
		if(MX==m_ssX && MY==m_ssY)
		{
			return MOVEINFO_HALFDONE;
		}
	}
	
	// 실제로 이동을 했음을 알려준다. 
	if(moveswitch==TRUE)
	{
		return MOVEINFO_REALMOVE;
	}
	
	return MOVEINFO_MOVING;

}

// 이동 루틴 
// 리턴값: 1 -> 목적지 도착 
int cltCharOrg::MoveFly(SHORT moveblockcount, SHORT range)
{
   int x=m_ssX, y=m_ssY;
   int direction;
   int rx=RX, ry=RY, NX=m_ssX, NY=m_ssY;
   SHORT realnx=0, realny=0;
   SHORT realdx=0, realdy=0;
   SHORT i,j,k;

   // 이동전 조치가 완료되면 이동, 
   // 아니면 조치 
   if(IsPreMoveOk()==FALSE)
   {
	   return 0;
   }

   // 목적지의 도트좌표를 구한다. 
   SHORT movespeed=MoveSpeed+MoveSpeedVary;
   realdx=MapCenterX[DX][DY];
   realdy=MapCenterY[DX][DY];


   // 직선으로 가는 경로를 찾는다. 

   // 이동 방향을 초기화한다. 
   direction=DecideDirectionChar16(m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy);

   // 목적지의 방향과 현재 방향이 일치하면 그냥 목적지로 간다. 
   SHORT gomode=FALSE;
   if(TempDirection==direction)
   {
      gomode=TRUE;
   }
   else
   {
       gomode=FALSE;
   }

   // 방향이 일치하지 않고 
   // 목적지와 현재 좌표가 가까우면 진행하지 않고 방향만 바꾼다.
   if( TempDirection!=direction
   &&  (abs(m_cltCharStatus.GetCenterX()-realdx)<40 && abs(m_cltCharStatus.GetCenterY()-realdy)<40) )
   {
	   realnx=m_cltCharStatus.GetCenterX();
	   realny=m_cltCharStatus.GetCenterY();
   }
   else
   {
      FindPathStraight(m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy, TempDirection, movespeed, gomode, realnx, realny);
   }

   MovePercent=0;

   SetAnimation(ANI_MOVE);
   DoAnimationDelay(TRUE);
   
   SHORT oldx=m_ssX;
   SHORT oldy=m_ssY;


   // X, Y좌표를 구한다.
   clifMap.GetRealTileByRealDot(realnx, realny, &m_ssX, &m_ssY);

   RX=realnx-MapCenterX[m_ssX][m_ssY];
   RY=realny-MapCenterY[m_ssX][m_ssY];

   // 방향을 전환한다. 
   if(ChangeDirection(direction)==FALSE)return 0;

   // 목적지에 도착했다. 
   if(DX==m_ssX && DY==m_ssY)
   {
	   //만약 목적지에 다른 항공기가 있다면 인근의 다른 지역으로 자리를 옮긴다. 
	   if(clifMap.GetSkyMap(m_ssX, m_ssY))
	   {
		   // 인근의 빈자리를 찾아서 간다. (현재 위치에서 가장 가까운 곳으로)
		   for(i = 0;i < 5;i++)
			   for(j = -i;j <= i;j++)
				   for(k = -i;k <= i;k++)
				   {
					   
					   if(abs(j)!=i && abs(k)!=i)continue;
					   
					   if(Map.IsInMapArea(m_ssX+k, m_ssY+j)==FALSE)continue;
					   
					   if(clifMap.GetSkyMap(m_ssX+k, m_ssY+j) == 0)
					   {
						   DX=m_ssX+k;
						   DY=m_ssY+j;
						   SetMove(DX, DY);
						   goto CLTCHARORG_MOVE_EXIT;
					   }
				   }
				   
CLTCHARORG_MOVE_EXIT:
				   return 0;
				   
	   }
	   else
		   return 1;
   }

   return 0;

}


void cltCharOrg::CharMove()
{

	// 목적지에 도착 
	if(Move(BASIC_MOVE_BLOCK_COUNT) == 1)
	{
		OrderStop(m_NewID, BY_COMPUTER);
	}

	if(MovePercent==0)
        m_scBusySwitch=FALSE;
	else 
	 	m_scBusySwitch=TRUE;
}

BOOL cltCharOrg::MoveDone()
{
	if(MovePercent)return FALSE;
	return TRUE;
}

// 지하로 이동하는 것 처리.
SI32 cltCharOrg::UnderGroundMove()
{
	switch(WorkStep)
	{
	case WS_INITINGROUND:
		m_scBusySwitch = TRUE;
		SetAnimation(ANI_INGROUND);
		m_cltCharDrawInfo.SetAniStep(0);
		WorkStep	= WS_INGROUND;
		break;

	case WS_INGROUND:
		SetAnimation(ANI_INGROUND);
		DoAnimationDelay(TRUE, ANI_ATB_ONCE);

		if(m_cltCharDrawInfo.GetAniStep() == (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[InGroundCharAnimationIndex].GetFrameNumber() - 1))
		{
			WorkStep = WS_INITOUTGROUND;

			// 위치가 이동된다. 
			m_ssX	= MX;
			m_ssY	= MY;
		}
		break;

	case WS_INITOUTGROUND:
		SetAnimation(ANI_OUTGROUND);
		m_cltCharDrawInfo.SetAniStep(0);
		WorkStep	= WS_OUTGROUND;
		break;

	case WS_OUTGROUND:
		SetAnimation(ANI_OUTGROUND);
		DoAnimationDelay(TRUE, ANI_ATB_ONCE);

		if(m_cltCharDrawInfo.GetAniStep() == (KI[m_cltCharStatus.GetKind()].m_clCharAnimation[OutGroundCharAnimationIndex].GetFrameNumber() - 1))
		{
			m_scBusySwitch				= FALSE;
		}
		break;

	}

	return MOVEINFO_MOVING;
}


BOOL cltCharOrg::IsPreMoveOk()
{
	return TRUE;
}

// 이동할 준비가 되었는가?
BOOL cltCharOrg::IsPreMoveOkSAMC()
{
	   return TRUE;
}

BOOL cltCharOrg::IsPreMoveOkFireTankK()
{
	// 사격자세가 풀렸으면, 
	if(ReadyToFireStep == 0)return TRUE;

	return FALSE;
}


