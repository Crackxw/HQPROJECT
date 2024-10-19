////////////////////////////////////////////////////////////////////
////  공격 관련 
#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include <setchar.h>
#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "..\Gersang\calcattack.h"
#include "music.h"
#include <CharOrg.h>

#include <Map.h>
#include <MapInterface.h>
#include <ysmap.h>
#include <Magic.h>
#include <bullet.h>
#include <smoke.h>
#include <MyNet.h>
#include <MyNet-Game.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

extern _clGame* pGame;

extern	_MyNetGame			MyNet;
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_NewID 				ZeroID;
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_YSMap 				SMap;								// 작은 지도용 데이터 

extern SI32 AttackAtbInfo[ATTACKATB_MAX];

// robypark 2004/6/14 15:51
extern	CItem					*g_pItem;

// robypark 2004/10/18 12:17
// 두 가지 속성을가진 캐릭터 공격 속성 결정 관련 상수값
#define PRIORITY_ATTACK_TYPE_CYCLE				0		// 기존 처럼 번갈어 가며
#define PRIORITY_ATTACK_TYPE_CLOSE_RANGE		1		// 근거리 우선
#define PRIORITY_ATTACK_TYPE_LONG_RANGE			2		// 장거리 우선

// 공격 효과음을 얻어온다. 
SI32 cltCharOrg::GetAttackEffect() const
{
	switch(m_siCurrentAttack)
	{
	case 0: return KI[m_cltCharStatus.GetKind()].m_siAttackEffect1; break;
	default: return KI[m_cltCharStatus.GetKind()].m_siAttackEffect2; break;
	}
	return 0;
}

// 공격 범위를 얻어온다. 
SI32 cltCharOrg::GetAttackRange()const
{
	if(m_cltCharStatus.GetKind() == KIND_ON_AGILETIGER)
		return m_siAttackRange2;
	switch(m_siCurrentAttack )
	{
	case 0: return m_siAttackRange1; break;
	default: return m_siAttackRange2; break;
	}
	return 0;
}

// 공격 범위를 설정한다.
void cltCharOrg::SetAttackRange(SI32 range)
{
	switch(m_siCurrentAttack)
	{
	case 0:  m_siAttackRange1 = range; break;
	default: m_siAttackRange2 = range; break;
	}
	
}

// 공격 속성을 얻어온다. 
SI32 cltCharOrg::GetAttackAtb()const 
{
	switch(m_siCurrentAttack )
	{
	case 0: return m_siAttackAtb1; break;
	default: return m_siAttackAtb2; break;
	}
	return 0;
}

// 공격 속성을 설정한다.
void cltCharOrg::SetAttackAtb(SI32 atb)
{
	switch(m_siCurrentAttack)
	{
	case 0:  m_siAttackAtb1 = atb; break;
	default: m_siAttackAtb2 = atb; break;
	}
}

////////////////////////////////////////////////////////////////////
// attack_id 적을 공격하게 만드는 함수 
BOOL _Char::SetAttack(_NewID id)
{
	// 공격 목표가 이미 죽었거나 적이 아니면 공격하지 않는다. 
	if(IsAlive(id.ID)==FALSE)return FALSE;

	// 자기 자신은 공격할 수 없다. 
	if(id.IsSame(m_NewID))return FALSE;

	AttackID = id;

	// 공중 캐릭터인가 아니면 지상 캐릭터인가?
    AttackArea = CR[AttackID.ID].m_cltCharStatus.GetArea();

	AttackGiveUpDelayor	  =	0;
    AttackMode=ATTACK_MODE_CHAR;

    return TRUE;
}

// 지상을 공격하게 만드는 함수 
BOOL _Char::SetAttackGround(SI32 six, SI32 siy)
{

	AttackID = ZeroID;

	// 공중 캐릭터인가 아니면 지상 캐릭터인가?
    AttackArea=AREA_LAND;

	AttackGiveUpDelayor	  =	0;
    AttackMode=ATTACK_MODE_GROUND;

    return TRUE;
}



// fire직전에 확인 
BOOL _Char::ReadyToFire()
{
	// 캐릭터의 방향이 목표를 향하고 있는지 확인한다. 
	if(ChangeDirection(TargetDirection)==FALSE)
		return FALSE;

	return TRUE;
	
	return FALSE;
}


// 본격적인 전투에 들어가기 전에 처리해야 할 것을 처리한다. 
BOOL _Char::PreAttack()
{
	return TRUE;
}


BOOL _Char::Escape()
{
	// 사수 모드가 설정되어 있고 명령을 내린 주체가 컴이면, 
	if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
	{
		// 이동하지 않는다.
		m_scBusySwitch = FALSE;
		return TRUE;
	}
	// 적을 공격하기 위해 거리를 띄우러 이동한다.
	else
	{
		WorkStep=WS_GOTO_OPPOSITE;
		
		SHORT dir=DecideDirection8(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, m_ssX, m_ssY);
		SHORT tx=m_ssX, ty=m_ssY;
		
		if(dir&NORTH)
		{
			ty=max(0, m_ssY-3);
		}
		if(dir&SOUTH)
		{
			ty=min(clifMap.GetYsize() - 1, m_ssY+3);
		}
		if(dir&EAST)
		{
			tx=min(clifMap.GetXsize() - 1, m_ssX+3);
		}
		if(dir&WEST)
		{
			tx=max(0, m_ssX-3);
		}
		
		SetMove(tx, ty);
	}
	return FALSE;
}

BOOL _Char::TryAttack()
{
	Step=0;
	CR[AttackID.ID].GetHitPoint(HitPointX, HitPointY);
			 
	// 목표의 방향을 구한다. 
	TargetDirection=DecideDirection8(m_ssX, m_ssY, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
	
	
	
	// 다른 준비가 되었는지 확인한다. 
	if( ReadyToFire() == FALSE )
		return TRUE;
	 
	
	// 상황에 따라 필살기를 쓰기도 한다.
	if( IsStrongAttackAvailable() == TRUE && m_CloudWindFrame <= 0)
	{
		// 닌자인 경우 적의 건물을 점령한다. 
		if( IsAtb(ATB_CAPTURE) == TRUE )
		{
			OrderCapture(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_PLAYER);
		}
		// 무녀의 경우 적을 현혹한다.
		else if( IsAtb(ATB_BETRAY) == TRUE )
		{
			OrderBetray(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
		}
		// 명 승병인 경우 
		else if( IsAtb2(ATB2_STEALEXP) == TRUE )
		{
			
			if(OldOrder.Order==ORDER_WAIT)
				SetOldOrder(ORDER_ATTACK, HostMode,  FALSE, m_ssAttackX|(m_ssAttackY<<16), AttackID);
			
			OrderStealExp(m_NewID, BY_COMPUTER);
		}
		// 번개를, 
		else if( IsAtb3(ATB3_LIGHTNING) == TRUE )
		{
			OrderLightning(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
		}
		// actdoll (2004/03/10 10:29) : 
		//	이 부분은 일단 IsStrongAttackAvailable() 함수 내로 옮긴다.
		//	한 몹 유닛이 두가지 이상의 기술을 쓸 수 있게 되면서 로직의 중복으로 인하여 연옥술과 포박술이 정상적으로 
		//	작동을 하지 못하게 된다. 따라서 일단 같은 위치에서 작동될 수 있도록 작업을 해줬다.
		//	추후 반드시 수정되야 할 부분중에 하나다.
		// 연옥술을 
//		else if( IsAtb3(ATB3_GROUNDEXP) == TRUE )
//		{
//			OrderGroundExp(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
//		}
		// 포박술을 
//		else if( IsAtb3(ATB3_MAGIC6C) == TRUE )
//		{
//			OrderMagic6c(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
//		}
		// 이사의 사념 특수공격.
		else if( IsAtb3(ATB3_ICE) == TRUE)
		{
			OrderIce(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), m_NewID, BY_COMPUTER);
		}
		else if( IsAtb4(ATB4_LONG_SPEAR) == TRUE)
		{
			OrderLongSpear(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
		}

		
		return TRUE;
	}
	else
	{
		WorkStep=WS_FIRE;
		
		AttackGiveUpDelayor=0;
		
		m_ssAttackX=CR[AttackID.ID].GetX();
		m_ssAttackY=CR[AttackID.ID].GetY();
	}

	return FALSE;
}


BOOL _Char::TryAttackGround()
{

	SI32 six = LOWORD(Para1);
	SI32 siy = HIWORD(Para1);
	Step=0;
	
	if(Map.IsInMapArea(six, siy) == TRUE)
	{
		HitPointX = MapCenterX[six][siy];
		HitPointY = MapCenterY[six][siy];
	}
	else
	{
		// 공격을 중단한다.
		return FALSE;
	}

			 
	// 목표의 방향을 구한다. 
	TargetDirection=DecideDirection8(m_ssX, m_ssY, six, siy);
	
	
	// 다른 준비가 되었는지 확인한다. 
	if(ReadyToFire()==FALSE)return TRUE;
			 
	WorkStep=WS_FIRE;
	
	AttackGiveUpDelayor=0;
	
	m_ssAttackX = six;
	m_ssAttackY = siy;

	return TRUE;
}


// 캐릭터 전투 
BOOL _Char::Attack()
{
	SHORT status;
	SHORT direction=0;
	SI32 six = LOWORD(Para1);
	SI32 siy = HIWORD(Para1);
	
	if(AttackMode == ATTACK_MODE_CHAR)
	{
		// 공격 대상이 유효하고 연합군이면, 
		if(IsSee(AttackID) && IsFriendToByPlayerNumber(GetPlayerNumber(), CR[AttackID.ID].GetPlayerNumber())==TRUE
			&& m_scBusySwitch == FALSE)
		{
			
			if(abs(CR[AttackID.ID].m_cltCharStatus.GetChangedPlayerFrame()-GD.Frame) < 20  // 플레이어가 변경된 지 얼마 안된 것이라면,    
				|| HostMode==BY_COMPUTER                                // 명령의 주체가 컴이거나
				|| GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER)        // 게임의 주체가 컴이면, 
			{
				return FALSE;
			}
		}
	}

	switch(WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
		
		// 유닛에 대한 공격이라면, 
		if(AttackMode == ATTACK_MODE_CHAR)
		{
			
			// 적을 공격할 수 없는 상태라면 중단한다.
			if(CanAttack(AttackID)==FALSE)
			{   
				
				m_scBusySwitch=FALSE;
				
				// 주변을 수색한다. 
				SearchForwardSwitch = TRUE;
				
				return FALSE;
			}
			
			// robypark 2004/10/18 12:19
			// 두 가지 공격 속성이 있고, 번갈어 가며 공격하는 인공지능 값이 아닐 경우
			if ((PRIORITY_ATTACK_TYPE_CYCLE != m_ssPriority_Attack_Type) && (m_siAttackAtb2))
			{
				// 두 번째 공격 속성이 더 원거리 공격을 한다.
				if (m_siAttackRange1 < m_siAttackRange2)
				{
					// 근거리 공격 우선 속성일 경우 근거리 공격을 우선한다.
					if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_CLOSE_RANGE)
					{
						m_siCurrentAttack = 0;

						// 적이 사정거리 밖에 있으면,
//						if(FALSE == IsInRange(AttackID.ID, GetAttackRange()))
//							m_siCurrentAttack = 1;
					}
					// 원거리 공격 우선 속성일 경우 원거리 공격을 우선한다.
					else if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_LONG_RANGE)
						m_siCurrentAttack = 1;
				}
				// 첫 번째 공격 속성이 더 원거리 공격을 한다.
				else
				{
					// 근거리 공격 우선 속성일 경우 근거리 공격을 우선한다.
					if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_CLOSE_RANGE)
					{
						m_siCurrentAttack = 1;

						// 적이 사정거리 밖에 있으면,
//						if(FALSE == IsInRange(AttackID.ID, GetAttackRange()))
//							m_siCurrentAttack = 0;
					}
					// 원거리 공격 우선 속성일 경우 원거리 공격을 우선한다.
					else if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_LONG_RANGE)
						m_siCurrentAttack = 0;
				}
			}
	
			// 적이 사정거리 안에 있으면,
			if(IsInRange(AttackID.ID, GetAttackRange()))
			{ 
				
				// robypark 2004-5-24, 16:06 최소거리 공격 가능이 가능한가
				// 최소 거리에서 공격 불가능한 캐릭터인 경우에는 
				if ((AttackAtbInfo[GetAttackAtb()] & ATTACKATB_ATB_MINRANGE)
						&& (IsInMinRange(AttackID.ID, KI[m_cltCharStatus.GetKind()]\
											.GetMinAttackRange(m_siCurrentAttack)) == TRUE))
//				if(IsAtb(ATB_MINRANGE)==TRUE && IsInMinRange(AttackID.ID, KI[m_cltCharStatus.GetKind()].GetMinAttackRange(CurrentAttack))==TRUE)
//				if ((AttackAtbInfo[GetAttackAtb()] & ATTACKATB_ATB_MINRANGE) \
//				&& (IsInMinRange(AttackID.ID, 1) == TRUE))
				{			
					// 공격 가능하고 이동 가능 하다면, 
					if(CanAttack(AttackID))
					{
						
						// 사수 모드가 설정되어 있고 명령을 내린 주체가 컴이면, 
						if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
						{
							// 이동하지 않는다.
							m_scBusySwitch=FALSE;
							return FALSE;
						}
						// 적을 공격하기 위해 거리를 띄우러 이동한다.
						else
						{
							// robypark 2004/10/18 12:24
							// 원거리 우선 공격 속성을가진 경우라면, 근거리 공격으로 전환하도록 한다.
							// 적과의 거리가 원거리 공격이 불가능한 경우
							if ((PRIORITY_ATTACK_TYPE_LONG_RANGE == m_ssPriority_Attack_Type) && (m_siAttackAtb2))
							{
								// 근거리 공격으로 전환된다.
								if (m_siAttackRange1 < m_siAttackRange2)
									m_siCurrentAttack = 0;
								else
									m_siCurrentAttack = 1;

								// 공격 시도
								if (IsInRange(AttackID.ID, GetAttackRange()))
								{
									if (TryAttack() == FALSE) 
									{
										return TRUE;
									}
								}
							}

							if(Escape() == TRUE)
							{
								// 이동하지 않는다. 
								return FALSE;
							}
						}
					}
					else
					{
						m_scBusySwitch=FALSE;
						return FALSE;
					}
					
				}
				else
				{
					if(TryAttack()==TRUE)
					{
						return TRUE;
					}
				}				  
			}
			else // 적이 사거리 밖에 있으면, 
			{  
				
				// 또다른 공격 속성이 있다면 
				if(m_siAttackAtb2 && m_siCurrentAttack == 0)
				{
					m_siCurrentAttack = 1;
					
					if(CanAttack(AttackID) == FALSE)
					{
						m_siCurrentAttack = 0;
					}
				}
				else if(m_siAttackAtb1 && m_siCurrentAttack == 1)
				{
					m_siCurrentAttack = 0;
					
					if(CanAttack(AttackID) == FALSE)
					{
						m_siCurrentAttack = 1;
					}
				}

				// 공격가능하고 이동 가능 하다면, 
				if(CanAttack(AttackID) )
				{
					WorkStep=WS_GOTO_TARGET;
					
					
					// 사수 모드가 설정되어 있고 명령을 내린 주체가 컴이면, 
					if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
					{
						// 이동시키지 않는다. 
						m_scBusySwitch=FALSE;
						return FALSE;
					}
					// 적이 있는 곳으로 이동한다. 
					else
					{
						// 폭격이면 대상보다 위에 가야 한다. 
						if(GetAttackAtb()==ATTACKATB_BOMB)
						{
							SetMove(CR[AttackID.ID].GetX()-2, CR[AttackID.ID].GetY()-2);
						}
						else
						{
							SetMove(CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY());
						}
					}
				}
				else
				{
					m_scBusySwitch=FALSE;
					
					// 주변을 수색한다. 
					SearchForwardSwitch = TRUE;
					
					return FALSE;
				}
			}
		  }
		  else
		  {
			  if(CanAttackGround(six, siy) == FALSE)
			  {   
				  
				  m_scBusySwitch=FALSE;
				  return FALSE;
			  }
			  
			  // 사거리 안에 있으면, 
			  if(max(abs(m_ssX-six), abs(m_ssY-siy)) < GetAttackRange())
			  {
				  if(TryAttackGround() == FALSE)
				  {
					  // 공격을 중단한다. 
					  return FALSE;
				  }
			  }
			  // 사거리 밖에 있으면, 
			  else
			  {
				  // 공격가능하고 이동 가능 하다면, 
				  if(CanAttackGround(six, siy))
				  {
					  WorkStep=WS_GOTO_TARGET;
					  
					  // 사수 모드가 설정되어 있고 명령을 내린 주체가 컴이면, 
					  if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
					  {
						  // 이동시키지 않는다. 
						  m_scBusySwitch=FALSE;
						  return FALSE;
					  }
					  // 적이 있는 곳으로 이동한다. 
					  else
					  {
						  // 폭격이면 대상보다 위에 가야 한다. 
						  if(GetAttackAtb() == ATTACKATB_BOMB)
						  {
							  SetMove(six-2, siy-2);
						  }
						  else
						  {
							  SetMove(six, siy);
						  }
					  }
				  }
				  else
				  {
					  m_scBusySwitch=FALSE;
					  return FALSE;
				  }
			  }
		  }
		  if(HealAI()==TRUE)
			  return TRUE;
		  
		  break;
		  
	case WS_GOTO_TARGET:
		status=Move(BASIC_MOVE_BLOCK_COUNT, GetAttackRange());
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}
		
		if(m_scBusySwitch==FALSE)
		{
			
			if(AttackMode == ATTACK_MODE_CHAR)
			{
				// 적을 공격할 수 없는 처지라면, 
				if(CanAttack(AttackID)==FALSE)
				{  
					// 주변을 수색한다. 
					SearchForwardSwitch = TRUE;
					
					return FALSE;
				}
				if(IsInRange(AttackID.ID, GetAttackRange()))
				{
					WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				}
			}
			else
			{
				// 적을 공격할 수 없는 처지라면, 
				if(CanAttackGround(six, siy)==FALSE)
				{  
					return FALSE;
				}
				
				if(max(abs(m_ssX-six), abs(m_ssY-siy)) < GetAttackRange())
				{
					WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				}
			}
			
			// 목적지에 도착하거나 중간 지점에 도착하면, 
			if(status>0)
			{
				WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				
				AttackGiveUpDelayor++;
				if(AttackGiveUpDelayor>AttackGiveUpDelay)
				{
					return FALSE;
				}
			}
			
		}
		if(HealAI()==TRUE)
			return TRUE;
		
		break;
		
	case WS_GOTO_OPPOSITE:
		status=Move(BASIC_MOVE_BLOCK_COUNT);
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}
		
		if(m_scBusySwitch==FALSE)
		{
			
			if(AttackMode == ATTACK_MODE_CHAR)
			{
				// 적을 공격할 수 없는 처지라면, 
				if(CanAttack(AttackID)==FALSE)
				{  
					// 주변을 수색한다. 
					SearchForwardSwitch = TRUE;
					
					return FALSE;
				}
				if(IsInRange(AttackID.ID, GetAttackRange()))
				{
					if(IsInMinRange(AttackID.ID, 1)==FALSE)
						WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				}
			}
			else
			{
				// 적을 공격할 수 없는 처지라면, 
				if(CanAttackGround(six, siy)==FALSE)
				{  
					return FALSE;
				}
				if(max(abs(m_ssX-six), abs(m_ssY-siy)) <  GetAttackRange())
				{
					if(max(abs(m_ssX-six), abs(m_ssY-siy)) >  GetAttackRange())
						WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				}
			}
			
			// 목적지에 도착하거나 중간 지점에 도착하면, 
			if(status>0)
			{
				WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				
				AttackGiveUpDelayor++;
				if(AttackGiveUpDelayor>AttackGiveUpDelay)
				{
					return FALSE;
				}
			}
			
		}
		if(HealAI()==TRUE)
			return TRUE;
		break;
		
	case WS_FIRE:
		m_scBusySwitch=TRUE;
		
		if(Fire())
		{
			WorkStep=WS_DECIDE_LENGTH_TO_DEST;
			m_scBusySwitch=FALSE;
		}
		break;
	}
	//	if(HealAI()==TRUE)
	//		return TRUE;
	
	return TRUE;
}

BOOL _Char::Fire()
{
    int id=0; 
	int x=0, y=0;
	SI32 attackinstantstart, attackinstantend, attackinstantdiff;
	SI32 apower		= 0;
	SI32 hitrate	= 0;

	if(m_CloudWindFrame > 0)
		return TRUE;

    // 포복이 가능한 캐릭터이면, 
    if(IsAtb(ATB_LOWATTACK))
	{
		if(Step == 0)
		{
	      // 적으로부터 공격을 받는 상황이면, 
	      if(BeAttackedSwitch==TRUE && IsFriendToByID(m_NewID.ID, AttackMeID.ID) == FALSE
		  && clifMap.GetShallowMap(m_ssX/2, m_ssY/2) == FALSE)
		  {
	         LowMoveSwitch=TRUE;
		  }
	      else
		     LowMoveSwitch=FALSE;
		}
	}
	else
		LowMoveSwitch=FALSE;


	if(FireRepeatDelay > FireRepeatDelayor)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}

	if(GetTotalAttackDelay() > m_siAttackDelayor)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}

	// 포복 공격 
	if(LowMoveSwitch == TRUE)
	{

		SetAnimation(ANI_LOWATTACK);

		if(DoAnimationDelay(TRUE) == FALSE)return FALSE;

		attackinstantstart = GetAniInstant();

	}
	else
	{
		if(IsAtb(ATB_ABSANIWAIT) == FALSE)
		{
			if(m_siCurrentAttack == 0)
				SetAnimation(ANI_ATTACK);
			else
				SetAnimation(ANI_ATTACK2);

#ifdef _SKILL_
			// robypark 2004/6/14 15:45
			// 거북차, 지진차, 불랑기포차가 무기를 장착했을 경우 공격2 애니메이션을 사용한다.
			if ((m_cltCharStatus.GetKind() == KIND_ON_TURTLE_VEHICLE) || // 조선-거북차
				(m_cltCharStatus.GetKind() == KIND_ON_QUAKE_VEHICLE) || // 일본-지진차
				(m_cltCharStatus.GetKind() == KIND_ON_FARANGI_VEHICLE)) // 중국-불랑기포차
			{
				// 무기를 장착하였다.
				CItemHeader		*pItemHeader;
				pItemHeader = g_pItem->GetItem(WeaponID);
				if (pItemHeader)
					SetAnimation(ANI_ATTACK2);
			}
#endif
			if(DoAnimationDelay(TRUE) == FALSE)return FALSE;
		

			attackinstantstart = GetAniInstant();
		}
		else
		{
			attackinstantstart = 0;
		}
	}

	
	if(IsAlive(AttackID) == TRUE)
	{
		hitrate = m_ParmeterBox.GetHitRate( CR[AttackID.ID].m_ParmeterBox.GetDR() );
	}
	else
	{
		hitrate = m_ParmeterBox.GetHitRate( 0 );
	}

	if(GetAttackAtb() == ATTACKATB_ELECTRON)
	{
		attackinstantdiff = 1;
		attackinstantend =  KI[m_cltCharStatus.GetKind()].m_clCharAnimation[m_cltCharDrawInfo.GetAniVary()].GetFrameNumber() - 1;
	}
	else if(GetAttackAtb() == ATTACKATB_MACHINEGUN)
	{
		attackinstantdiff = 2;
		attackinstantend =  KI[m_cltCharStatus.GetKind()].m_clCharAnimation[m_cltCharDrawInfo.GetAniVary()].GetFrameNumber() - 1;
	}
	else
	{
		attackinstantdiff = 1;
		attackinstantend = attackinstantstart;
	}

	// robypark 2004/7/12 11:1
	// 2차 장수 무기 착용에 따른 처리 추가
	// 캐릭터 타입얻기
	SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));

	if(attackinstantstart <= m_cltCharDrawInfo.GetAniStep() && m_cltCharDrawInfo.GetAniStep() <= attackinstantend
	&& ((m_cltCharDrawInfo.GetAniStep()-attackinstantstart)%attackinstantdiff == 0))
	{
		// 쐐기지르기 속성이 있다면, 
		// robypark 2004/7/12 11:1
		// 2차 장수 무기 착용에 따른 처리 추가
		// 2차 장수는 무기에 있는 쐐기지르기 기술을 사용할 수 없다.
		if((IsAtb3(ATB3_MAGIC8K) == TRUE) && (3 != siCharType))
		{
			if(ReduceMana(MAGICPOWER_MAGIC8K) == TRUE)
			{
				SI32 id=FindEmptyBullet();
				if(id)
				{
					SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
					SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
					
					apower = CalcDamage(SPECIAL_MAGIC8K, m_ParmeterBox.CalcApower(), m_scMagicApplyRate);
					SetBullet(KIND_BULLET_MAGIC8K, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
					
					// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
					SetBattleFieldNEventMark();
					
					// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
					if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
						PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
				}
			}
		}
		// 신창노도 속성이 있다면(필살기)
		// robypark 2004/7/12 11:1
		// 2차 장수 무기 착용에 따른 처리 추가
		// 2차 장수는 무기에 있는 신창노도 기술을 사용할 수 없다.
		if ((IsAtb4(ATB4_GOD_SPEAR) == TRUE) && (3 != siCharType))
		{
			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_GOD_SPEAR)
			{
				if(GD.Random() % 7 == 0)
				{
					SI32 BulletID=FindEmptyBullet();
					if(BulletID)
					{
						apower = CalcDamage(SPECIAL_GOD_SPEAR, m_ParmeterBox.CalcApower(), m_scMagicApplyRate);
						SHORT x = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
						SHORT y = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
						
						ReduceMana(MAGICPOWER_GOD_SPEAR);
						SetBullet(KIND_BULLET_GODSPEAR, BulletID, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
						CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_GODSPEAR, 1, AttackArea, apower, AttackID, m_ssAttackX, m_ssAttackY, 100);
					}

					SI32 smokeid = FindEmptySmoke();
					if(smokeid)
						SetSmoke(KIND_SMOKE_SPECIAL, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AttackArea);
				}
			}
		}
		
		switch(GetAttackAtb())
		{
			
		case ATTACKATB_SWORD:			FireOperation_Sword(attackinstantstart, hitrate);		break;	// 검 공격 
		case ATTACKATB_DUEOKSINI:		FireOperation_Dueoksini(attackinstantstart, hitrate);	break;	// 장승 공격 
		case ATTACKATB_TOAD:			FireOperation_Toad(attackinstantstart, hitrate);		break;	// 두꺼비 공격 
		case ATTACKATB_CANNONJ:			FireOperation_CannonJ(attackinstantstart, hitrate);		break;	// 파쇄차공격이면, 
		case ATTACKATB_ELEPHANT:		FireOperation_Elephant(attackinstantstart, hitrate);	break;  // 코끼리 
		case ATTACKATB_BIGSWORD:		FireOperation_BigSword(attackinstantstart, hitrate);	break;  // 큰 검 공격  
		case ATTACKATB_MACHINEGUN:  
		case ATTACKATB_GUN:
		case ATTACKATB_STRONGGUN:		FireOperation_Gun(attackinstantstart, hitrate);			break;	// 총공격 (일반총, 강력한 총)
		case ATTACKATB_ELECTRON:		FireOperation_Electron(attackinstantstart, hitrate);	break;	// 전기공격
		case ATTACKATB_GATPHAPOISON:	FireOperation_GatphaPoison(attackinstantstart, hitrate);break;  // 갓파의 독. 
		case ATTACKATB_MAGIC:			FireOperation_Magic2(attackinstantstart, hitrate);		break;	
		case ATTACKATB_MAGIC2:			FireOperation_Magic2(attackinstantstart, hitrate);		break;	// 조선 승병 공격 
		case ATTACKATB_MAGIC3:			FireOperation_Magic3(attackinstantstart, hitrate);		break;	// 일본 무녀 공격  
		case ATTACKATB_MAGICBALL:		FireOperation_MagicBall(attackinstantstart, hitrate);	break;	// 심유경마법공격 
		case ATTACKATB_JUSTICEK:		FireOperation_Justicek(attackinstantstart, hitrate);	break;	// 조선 유생공격
		case ATTACKATB_ARROW:
		case ATTACKATB_AIRATTACKUPARROW:
		case ATTACKATB_FIREARROW:		FireOperation_Arrow(attackinstantstart, hitrate);		break;	// 일반 화살 공격 || 비전연구 후 화살 공격 || 불화살 공격. 
		case ATTACKATB_NEEDLE:			FireOperation_Needle(attackinstantstart, hitrate);		break;	// 침
		case ATTACKATB_SNOWWOMAN:		FireOperation_Snowman(attackinstantstart, hitrate);		break;	// 설녀의 마법 공격. 
		case ATTACKATB_FURHEADDEMON:	FireOperation_FurHeadDemon(attackinstantstart, hitrate);break;	// 털가면 -  얼음 
		case ATTACKATB_DART:			FireOperation_Dart(attackinstantstart, hitrate);		break;	// 닌자의 수리검
		case ATTACKATB_AIRMISSILE:		FireOperation_AirMissile(attackinstantstart, hitrate);	break;  // // 미사일 
		case ATTACKATB_SAMMISSILE:		FireOperation_SamMissile(attackinstantstart, hitrate);	break;	// 천궁 미사일 
		case ATTACKATB_CANNON:			FireOperation_Cannon(attackinstantstart, hitrate);		break;	// 대포 
		case ATTACKATB_TOWERCANNON:		FireOperation_TowerCannon(attackinstantstart, hitrate);	break;	// 화포 망루. 
		case ATTACKATB_HEAVYGUN:		FireOperation_HeavyGun(attackinstantstart, hitrate);	break;	// 기관포 
		case ATTACKATB_SHIPCANNON:		FireOperation_ShipCannon(attackinstantstart, hitrate);	break;	// 함포 
		case ATTACKATB_THROWBOMB:		FireOperation_ThrowBomb(attackinstantstart, hitrate);	break;	// 던지는 폭탄 
		case ATTACKATB_CANNON2C:		FireOperation_Cannon2C(attackinstantstart, hitrate);	break;	// 발석거  
		case ATTACKATB_BOMB:			FireOperation_Bomb(attackinstantstart, hitrate);		break;	// 폭탄
		case ATTACKATB_FIRECAR:			FireOperation_FireCar(attackinstantstart, hitrate);		break;	// 화차. 
		case ATTACKATB_FIRE:			FireOperation_Fire(attackinstantstart, hitrate);		break;	// 화염 
		case ATTACKATB_FIREUSINGMANA:	FireOperation_FireUsingMana(attackinstantstart, hitrate);break;	// 화염 (마법사용)
		case ATTACKATB_GOLDDRAGONELECTRON:FireOperation_GoldDragonElectron(attackinstantstart, hitrate);		break;	// 화염 
		case ATTACKATB_ONGHWA:			FireOperation_Onghwa(attackinstantstart, hitrate);		break;	// 온천 원숭이 해골. 
		case ATTACKATB_GHOSTCAT:		FireOperation_GhostCat(attackinstantstart, hitrate);	break;	// 귀묘 음파 공격. 
		case ATTACKATB_DOLL1:			FireOperation_Doll1(attackinstantstart, hitrate);		break;	// 무명 인형. 
		case ATTACKATB_DOLL2:			FireOperation_Doll2(attackinstantstart, hitrate);		break;	// 하카다 인형. 
		case ATTACKATB_KUKET:			FireOperation_Kuket(attackinstantstart, hitrate);		break;	// 쿠켓 인형.
		case ATTACKATB_CATDOLL:			FireOperation_CatDoll(attackinstantstart, hitrate);		break;	// 고양이 인형.
		case ATTACKATB_OLDFOXDOLL:		FireOperation_OldFoxDoll(attackinstantstart, hitrate);	break;	// 구미호 인형.
		case ATTACKATB_RACCOONDOLL:		FireOperation_RaccoonDoll(attackinstantstart, hitrate);	break;	// 너구리 인형.
		case ATTACKATB_NINJADOLL:		FireOperation_NinjaDoll(attackinstantstart, hitrate);	break;	// 너구리 인형.
		case ATTACKATB_SNOWWOMANDOOL:	FireOperation_SnowWomanDoll(attackinstantstart, hitrate);	break;	// 설녀 인형.
		case ATTACKATB_YANGGUIBIDOLL:	FireOperation_YangGuiBiDoll(attackinstantstart, hitrate);	break;	// 양귀비 인형.
		case ATTACKATB_JIJANGDOLL:		FireOperation_JijangDoll(attackinstantstart, hitrate);	break;	// 지장보살 인형.
		case ATTACKATB_SNAKEWOMANDOLL:	FireOperation_SnakeWomanDoll(attackinstantstart, hitrate);	break;	// 사녀 인형.
		case ATTACKATB_SNOW:			FireOperation_Snow(attackinstantstart, hitrate);		break;	// 하카다 인형.
		case ATTACKATB_BUTTERFLY:		FireOperation_Butterfly(attackinstantstart, hitrate);	break;	// 독나방 
		case ATTACKATB_WIDOW:			FireOperation_Widow(attackinstantstart, hitrate);		break;	// 거미요괴 
		case ATTACKATB_WAVE:			FireOperation_Wave(attackinstantstart, hitrate);		break;	// 바다거북이 파동공격. 
		case ATTACKATB_KIMBOMB:			FireOperation_KimBomb(attackinstantstart, hitrate);		break;	// 폭뢰격 
		case ATTACKATB_LEECH:			FireOperation_Leech(attackinstantstart, hitrate);		break;	// 거머리 체력 빼앗기. 
		case ATTACKATB_SEAHORSE:		FireOperation_SeaHorse(attackinstantstart, hitrate);	break;	// 해마 물뿜기 . 
		case ATTACKATB_POISONARROW:		FireOperation_PoisonArrow(attackinstantstart, hitrate);	break;	// 독침 
		case ATTACKATB_THROWSPEAR:		FireOperation_ThrowSpear(attackinstantstart, hitrate);	break;	// 던지는 창. 
		case ATTACKATB_CACTUS:			FireOperation_Cactus(attackinstantstart, hitrate);		break;
		case ATTACKATB_SCORPION:		FireOperation_Scorpion(attackinstantstart, hitrate);	break;
		case ATTACKATB_CLAW:			FireOperation_Claw(attackinstantstart, hitrate);		break;
		case ATTACKATB_MAGICIAN:		FireOperation_Magician(attackinstantstart, hitrate);	break;	// 요술사. 
		case ATTACKATB_BIGTREE:			FireOperation_BigTree(attackinstantstart, hitrate);		break;	// 거대수 특수 공격.
		case ATTACKATB_VOLCANOFLAME:	FireOperation_VolcanoFlame(attackinstantstart, hitrate);break;	// 화산불꽃
		case ATTACKATB_EVIL:			FireOperation_Evil(attackinstantstart, hitrate);		break;	// 이사의 사념 장풍.
		case ATTACKATB_AUTA:			FireOperation_Auta(attackinstantstart, hitrate);		break;	// 아우타 공격.
		case ATTACKATB_ICE:				FireOperation_Ice(attackinstantstart, hitrate);			break;  // 이사의 사념 특수기술.
		case ATTACKATB_MAGIC4T:			FireOperation_Magic4T(attackinstantstart, hitrate);		break;  // 징키스칸혼 특수기술.
		case ATTACKATB_JAPANGHOST:		FireOperation_JapanGhost(attackinstantstart, hitrate);	break;	// 일본귀신 우산살 공격
		case ATTACKATB_SUB1:			FireOperation_Sub1(attackinstantstart, hitrate);		break;	// 진성여왕 화살공격 1
		case ATTACKATB_SUB2:			FireOperation_Sub2(attackinstantstart, hitrate);		break;	// 진성여왕 화살공격 2
		case ATTACKATB_PAPERDOLL:		FireOperation_PaperDoll(attackinstantstart, hitrate);	break;	// 음양사 종이인형
		case ATTACKATB_NEEDLEATTACK:	FireOperation_NeedleAttack(attackinstantstart, hitrate);break;	// 의술사 침공격
		case ATTACKATB_CLOUDWIND:		FireOperation_CloudWind(attackinstantstart, hitrate);	break;	// 염력사 풍모술
		case ATTACKATB_SOULCHANGE:		FireOperation_Seimei(attackinstantstart, hitrate);		break;	// 세이메이 기본공격
		case ATTACKATB_MAGICCATTACK:	FireOperation_MagicCAttack(attackinstantstart, hitrate);break;	// 도술사 기본공격
		case ATTACKATB_SUNBIYOUN:		FireOperation_Sunbiyoun(attackinstantstart, hitrate);	break;	// 순비연 기본공격	
		
		// actdoll (2004/03/06 5:02) : 유명계/진시황릉 추가
		case ATTACKATB_PYTHON:			FireOperation_Python		( attackinstantstart, hitrate );	break;	// 이무기 원거리 마법공격	
		case ATTACKATB_JINEMPEROR:		FireOperation_JinEmperor	( attackinstantstart, hitrate );	break;	// 진시황의혼 원거리 마법공격

		// linus : 수호부 관련
//		case ATTACKATB_RABBIT:			FireOperation_Rabbit(attackinstantstart, hitrate);				break;	// 수호부(토끼) 기본공격		
		case ATTACKATB_GUARDIAN_SNAKE:	FireOperation_GuardianSnake ( attackinstantstart, hitrate );	break;	// 수호부(뱀) 기본공격.

		// damduk : 수호부 관련
		case ATTACKATB_GUARDIAN_PIG:	FireOperation_GuardianPig(attackinstantstart, hitrate);			break;	// 수호부(돼지) 기본공격.

		// robypark (2004/5/19 17:31) : 장수 2차 전직 관련 추가
		// 조선-거북차, 이순신 2차 전직
		case ATTACKATB_TURTLE_VEHICLE:
			FireOperation_TURTLE_VEHICLE(attackinstantstart, hitrate);
			break;

		// 조선-뇌전차, 허준 2차 전직
		case ATTACKATB_THUNDER_VEHICLE:
			FireOperation_THUNDER_VEHICLE(attackinstantstart, hitrate);
			break;

		// 일본-지진차, 와키자카 2차 전직
		case ATTACKATB_QUAKE_VEHICLE:
			FireOperation_QUAKE_VEHICLE(attackinstantstart, hitrate);
			break;

		// 일본-흑룡차, 세이메이 2차 전직
		case ATTACKATB_BLACK_DRAGON_VEHICLE:
			FireOperation_BLACK_DRAGON_VEHICLE(attackinstantstart, hitrate);
			break;

		// 중국-발석거, 순비연<=이령 2차 전직
		case ATTACKATB_CATAPULT:
			FireOperation_CATAPULT(attackinstantstart, hitrate);
			break;

		// 중국-불랑기포차, 이령<=순비연 2차 전직
		case ATTACKATB_FARANGI_VEHICLE:
			FireOperation_FARANGI_VEHICLE(attackinstantstart, hitrate);
			break;

		// 대만-화룡차, 손유창 2차 전직
		case ATTACKATB_FIRE_DRAGON_VEHICLE:
			FireOperation_FIRE_DRAGON_VEHICLE(attackinstantstart, hitrate);
			break;

		// 대만-봉황비조, 장선화 2차 전직
		case ATTACKATB_CHINESEPHOENIX:
			FireOperation_CHINESEPHOENIX(attackinstantstart, hitrate);
			break;

		// robypark (2004/6/4 13:6)
		// 앙천대소 공격, 불랑기포차, 126
		case ATTACKATB_ROAR_OF_LAUGHTER:
			FireOperation_ROAR_OF_LAUGHTER(attackinstantstart, hitrate);
			break;

		// robypark 2004/7/14 16:47
		// 에비스 인형 공격
		case ATTACKATB_DOLL_EVIS:
			FireOperation_DOLL_EVIS(attackinstantstart, hitrate);
			break;

		// robypark 2005/2/2 17:27
		// 달마 인형 공격
		case ATTACKATB_DOLL_DHARMA:
			FireOperation_DOLL_DHARMA(attackinstantstart, hitrate);
			break;

		default:
			{
				//char buffer[FILE_NAME_SIZE];
				//wsprintf(buffer, "kind:%s attackatb:%d", KI[m_cltCharStatus.GetKind()].Name, GetAttackAtb());
				//SetMessage(buffer);
			}
			break;
			
		}
	}
	else
	{
		ElectronSwitch = FALSE;
	}
	
	if(m_cltCharDrawInfo.GetAniStep() == 0)
	{
		m_siAttackDelayor	=	0;
		m_scBusySwitch		=	FALSE;

		// 또다른 공격 속성이 있다면 
		if(m_siAttackAtb2 && m_siCurrentAttack == 0)
		{
			m_siCurrentAttack = 1;
			
			if(CanAttack(AttackID) == FALSE)
			{
				m_siCurrentAttack = 0;
			}
		}
		else if(m_siAttackAtb1 && m_siCurrentAttack == 1)
		{
			m_siCurrentAttack = 0;
			
			if(CanAttack(AttackID) == FALSE)
			{
				m_siCurrentAttack = 1;
			}
		}
	
		return TRUE;
	}
	else return FALSE;
	
}


BOOL _Char::IsStrongAttackAvailable()
{
	SI32 sirand;
	SI32 data = 0;
	int returnval;
	
	// 분신이면 특수효과를 쓸 수 없다.
	if(m_siTimerDelay)
		return FALSE;
	

	if((GD.Random()%2) == 0)
	{
		if(abs(GD.Frame - LastStrongAttackFrame ) < 100) return FALSE;
		
		sirand = GD.Random() % 15 ;
		
		// 공중탄을 쓴다. 
		if(sirand == 1)
		{
/*			// 공격 목표가 항공기라면, 
			if(CR[AttackID.ID].Area == AREA_AIR)
			{
				OrderAirMagic(m_NewID, BY_COMPUTER);
				return TRUE;
			}
			*/
		}
		// 만월도끼춤을 가한다.(컴인 경우에만..)
		else if( sirand == 2 && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE) )
		{

			if( CanSpecial(SPECIAL_MAGIC5J, ZeroID) == TRUE 
				&& CR[AttackID.ID].m_cltCharStatus.GetArea() == AREA_LAND )
			{

				OrderMagic5j(m_NewID, BY_COMPUTER);
				LastStrongAttackFrame = GD.Frame;
				return TRUE;
			}
		}
		// 지뢰를 매설한다.
/*
		else if( sirand == 3)
		{
			SI32 x, y;

			if( CanMine() == TRUE && GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER  
				&& CR[AttackID.ID].m_cltCharStatus.GetArea() == AREA_LAND
				&& SearchForMineSpot(&x, &y) == TRUE)
			{
				OrderMine(m_NewID, x, y, BY_COMPUTER);
				LastStrongAttackFrame = GD.Frame;
				return TRUE;
			}
		}
*/
		// 보호막을 친다. 
		else if(sirand == 4 )
		{
			// 체력이 떨어지고 방어막이 설치되어 있지 않아야 한다. 
			if(GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER  
				&& ShieldDelay == 0
				&& CanShield(m_NewID) == TRUE)
			{
				OrderShield(m_NewID, m_NewID, BY_COMPUTER);
			}
		}

		// 번개 치기
		if(sirand == 5 && IsAtb3(ATB3_LIGHTNING) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(CanLightning(AttackID)==TRUE)
			{
				return TRUE;
			}

			return FALSE;
		}

		// 기공신포. (컴플레이어야 한다.)
		if( (sirand >= 6 && sirand <= 10) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(CanSpecial(SPECIAL_MAGIC4C, AttackID) == TRUE)
			{
				OrderMagic4c(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;

		}

		// 풍백술
		if((sirand >= 11 && sirand <= 14) && IsAtb3(ATB3_TORNADO) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			
			// 공격목표에 지진을 일으킬 수 있으면, 
			if(CanSpecial(SPECIAL_TORNADO,  AttackID) == TRUE)
			{
				OrderTornado(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}

	}
	else
	{

		data = GD.Random() % 30;
 
		// 치료 . 
		if(data == 0 && IsAtb2(ATB2_HEAL) && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]))
		{
			HealAI();
			return FALSE;
		}

		// 현혹. 
		if(data == 1 && IsAtb(ATB_BETRAY))
		{
			if(CanBetray(AttackID, &returnval)==TRUE)
			{
				return TRUE;
			}
			return FALSE;

		}

		// 적장의 경험치 빼앗기. 
		if(data == 2 && IsAtb2(ATB2_STEALEXP))
		{
			if(CanStealExp()==TRUE)
			{
				return TRUE;
			}
			return FALSE;
		}

		// 적의 체력 빼앗기 
		if(data == 3 && IsAtb3(ATB3_ABSORBLIFE) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE) )
		{
			if(CanAbsorb(AttackID) == TRUE)
			{
				OrderAbsorb(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}

		if(data == 4 && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]))
		{
			if(CanSpecial(SPECIAL_MAGICTEST,AttackID) == TRUE)
			{
				OrderMagicTest(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
			}
		}

		// 현혹술
		if(data == 5 && IsAtb2(ATB2_BETRAY))
		{
			SI32 returnval = 0;

			// 공격목표가 현혹할 수 있는 것이라면, 
			if(CanBetray(AttackID, &returnval)==TRUE)
			{
				OrderBetray(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}

		// 분신술
		// 분신은 분신술을 구사할 수 없다. (HostID.IsEmpty() == TRUE : 분신이 아니다.)
		if( data >= 6  && data <= 8 && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()])
			&& HostID.IsEmpty() == TRUE)
		{
			if(IsAtb3(ATB3_CLON))
			{
				if(CanClon(ZeroID)==TRUE)
				{
					OrderClon(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}
			else if(IsAtb3(ATB3_ENEMYCLON))
			{
				if(CanClon(AttackID) == TRUE)
				{
					OrderEnemyClon(m_NewID, AttackID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}
			else if(IsAtb3(ATB3_SEAHORSECLON))
			{
				if(CanClon(AttackID) == TRUE)
				{
					OrderClon(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}
		}
		//요술사의 아귀,식귀 소환.
		//아마쿠사에 의해 소환된 요술사는 아귀,식귀를 소환할수 있다.
		if( data == 9 && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]))
		{
			if(IsAtb3(ATB3_CLON2))
			{
				if(CanClon(AttackID) == TRUE)
				{
					OrderClon(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}
		}
		
		//소환술
		if( (data >= 10 && data <= 12) && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) 
		&& HostID.IsEmpty() == TRUE)
		{
			//공통 소환
			if(CanReCallDamduk())
			{
				OrderReCallDamduk(m_NewID, BY_COMPUTER);	
				return TRUE;
			}
			//팬더 소환
			else if(IsAtb3(ATB3_MAGIC_RECALL_PANDA))	
			{
				if(CanReCall()==TRUE)
				{
					OrderReCallPanda(m_NewID, BY_COMPUTER);	
					return TRUE;
				}
				return FALSE;
			}
			//황룡 소환.
			else if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY))
			{
				if(CanReCall()==TRUE)
				{
					OrderReCallMonkey(m_NewID, BY_COMPUTER);	
					return TRUE;
				}
				return FALSE;					
			}
			//아마쿠사의 요술사 천구 소환.
			else if(IsAtb3(ATB3_MAGIC_RECALL_AMAKUSA))
			{
				if(CanReCall()==TRUE)
				{
					OrderReCallAmakusa(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}			
			//비호의 졸개호랑이 소환
			else if(IsAtb3(ATB3_MAGIC_RECALL_TIGER))
			{
				if(CanReCall()== TRUE)
				{
					OrderReCallTiger(m_NewID,	BY_COMPUTER);
				}	
				return FALSE;
			}
		}



		//황사.
		if( data == 13 && IsAtb3(ATB3_YELLOWSAND) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE) )
		{
			if(CanSpecial(SPECIAL_YELLOWSAND,  AttackID) == TRUE)
			{
				OrderYellowSand(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}

		
		// 지진술
		if(data == 14)
		{
			if( data == 14 && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE) )
			{

				// 자동 마법이 설정되어 있지도 않고 사용자이면, 
				if(GD.Player[GetPlayerNumber()].AutoMagicSwitch==FALSE
				&& ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()])==FALSE)return FALSE;

				SHORT length;		

				// 공격목표에 지진을 일으킬 수 있으면, 
				if(CanEarthQuake(AttackID)==TRUE)
				{
					// 거리가 너무 가까우면 지진을 일으키지 않는다.
					length=max(abs(m_ssX-CR[AttackID.ID].m_ssX), abs(m_ssY-CR[AttackID.ID].m_ssY));
					//if(length>=3)
					{
						OrderEarthQuake(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
					}
					
					return TRUE;
				}
				return FALSE;
			}
		}

		// 공간 이동술
		if( data == 15 && IsAtb2(ATB2_WARP) )
		{
			// 자동 마법이 설정되어 있지도 않고 사용자이면, 
			if(GD.Player[GetPlayerNumber()].AutoMagicSwitch==FALSE
			&& ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()])==FALSE)return FALSE;
			
			if(GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER)
			{
		
				// 체력이 많이 떨어진 어려운 지경에, 
				if(CanWarp() == TRUE
				&& m_ParmeterBox.IP.GetLife() < m_ParmeterBox.GetMaxLife())
				{
					OrderWarp(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}
		}
		// 현혹술
		if( data == 16 && IsAtb2(ATB2_BETRAY2) )
		{
			SI32 returnval = 0;

			if(CanBetray(AttackID, &returnval)==TRUE)
			{
				if(OldOrder.Order==ORDER_WAIT)
				   SetOldOrder(ORDER_ATTACK, HostMode,  FALSE, m_ssAttackX|(m_ssAttackY<<16), AttackID);

				OrderBetray(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);

				return TRUE;
			}

			return FALSE;
		}
		// 연옥술
		if( data == 17 &&  (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(IsAtb3(ATB3_GROUNDEXP))
			{
				if(CanSpecial(SPECIAL_GROUNDEXP, ZeroID) == TRUE)	
				{
				// actdoll (2004/03/10 4:55) : 
				//	이 부분은 상위의 함수에서 빼왔다. 하위 명령에 대한 부분과 상위 명령에 대한 부분의 처리 중복으로 인하여
				//	병합적으로 처리된 몬스터 유닛의 기술이 여러개가 다양하게 발사되지 않고 하나만 발생되는 버그가 생겼다.
				//	따라서 우선 급한대로 병합 기술을 가진 유닛의 기술들의 우선순위를 평등하게 놓기 위해 이 곳으로 추출했다.
				//	추후 이 부분을 전명 수정해야 할 것으로 생각된다.
					OrderGroundExp(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
					return TRUE;
				}
			}

			return FALSE;
		}
		//폭뢰격
		if( data == 18 &&  (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(CanSpecial(SPECIAL_KIMBOMB,  AttackID) == TRUE)
			{
				OrderKimBomb(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}
		// 암흑화염폭풍
		if( data == 19 &&  (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(IsAtb3(ATB3_DARKFIRE))
			{
				if(CanSpecial(SPECIAL_DARKFIRE, ZeroID) == TRUE)	return TRUE;
			}

			return FALSE;
		}
		// 포박술
		if( data == 20 && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(IsSee(AttackID) && CanSpecial(SPECIAL_MAGIC6C, ZeroID) == TRUE 
			&& CR[AttackID.ID].IsAtb(ATB_FLY) == FALSE
			&& CR[AttackID.ID].m_siHaltDelay == 0)
			{
				LastStrongAttackFrame = GD.Frame;
				// actdoll (2004/03/10 4:55) : 
				//	이 부분은 상위의 함수에서 빼왔다. 하위 명령에 대한 부분과 상위 명령에 대한 부분의 처리 중복으로 인하여
				//	병합적으로 처리된 몬스터 유닛의 기술이 여러개가 다양하게 발사되지 않고 하나만 발생되는 버그가 생겼다.
				//	따라서 우선 급한대로 병합 기술을 가진 유닛의 기술들의 우선순위를 평등하게 놓기 위해 이 곳으로 추출했다.
				//	추후 이 부분을 전명 수정해야 할 것으로 생각된다.
				if( IsAtb3(ATB3_MAGIC6C) == TRUE )	
				{
					OrderMagic6c(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
					return TRUE;
				}
			}
			return FALSE;
		}
		// 빙석술
		if( data == 21 && (ISMONSTER( MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if( CanSpecialAtb4( SPECIAL_ICE_BARRIER, AttackID ) )
			{
				OrderIceBarrier(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
		}
		// 석회술
		if( ( data >= 22 && data <= 23 ) && (ISMONSTER( MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if( CanSpecialAtb4( SPECIAL_STONE_MAGIC, AttackID ) )
			{
				OrderStoneMagic(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
		}
		// 마나번 
		if( ( data >= 24 && data <= 27) &&  (ISMONSTER( MyNet.PlayersdpId.dpId[GetPlayerNumber()] ) || m_bGuardianChar == TRUE))
		{
			if(CanSpecialAtb4( SPECIAL_MANABURN, AttackID ) )
			{
				OrderManaburn(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
			
		}
	}
	
	return FALSE;

}



// 공격의 최종 조율된 위치를 알려준다.
void _Char::DecideAttackPoint(int port, int& x, int&y)
{
	x=m_cltCharStatus.GetCenterX();
	y=m_cltCharStatus.GetCenterY();
}


// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
void _Char::SetBattleFieldNEventMark()
{

   // 공격 받는 유닛의 위치로 전투 위치 설정 
   if(CR[AttackID.ID].GetPlayerNumber() == UserPlayerNumber)
   {

	 if(clifMap.IsInScreen(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY) == FALSE)
	 {
	    GD.SetBattleField(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
        SMap.SetEventMark(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
	 }

   }
   // 공격하는유닛이 아군이면 전투 위치로 설정 
   else if(GetPlayerNumber() == UserPlayerNumber)
   {

	 if(clifMap.IsInScreen(m_ssX, m_ssY) == FALSE)
	 {
	    GD.SetBattleField(m_ssX, m_ssY);
	    SMap.SetEventMark(m_ssX, m_ssY);
	 }

   }
}


// 공격에 대한 정보를 저장한다. 다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
void _Char::SetFireInfo(_NewID id)
{
	FireID     = id;
	FireSwitch = TRUE;
	FireDelay  = MAX_FIRE_DELAY;
}

void _Char::FireOperation_Sword(SI32 attackinstant, SI32 hitrate)
{
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		if(GetAttackEffect())
		{
#ifndef _USE_HASHCODE_EFFECTSOUND			
			PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
			HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
		}
		else
		{
			if( IsAtb(ATB_CAPTURE) == TRUE )
			{
				PushEffect(EFFECT_NINJAJ, m_ssX, m_ssY);
			}
			// 늑대낭인이면, 
			if(m_cltCharStatus.GetKind() == KIND_ON_HORSESWORDJ
				|| m_cltCharStatus.GetKind() == KIND_ON_HORSESWORDUPJ)
			{
				PushEffect(EFFECT_HORSESWORDJATTACK, m_ssX, m_ssY);
			}
			else
			{
				PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
			}
		}
	}
	
	// 전투에 따라 이벤트 마크와 전투 지역 설정 등을 담당한다. 			 
	SetBattleFieldNEventMark();

	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_SWORD, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
}

void _Char::FireOperation_GuardianSnake(SI32 attackinstant, SI32 hitrate)
{
	SetFireInfo(AttackID);

	// 전투에 따라 이벤트 마크와 전투 지역 설정 등을 담당한다. 			 
	SetBattleFieldNEventMark();

	SI16 siMana = CR[AttackID.ID].m_ParmeterBox.IP.GetMana();

	if(siMana > 300)
	{
		CR[AttackID.ID].ReduceMana(300);  //마나 감소.
	}
	else
	{
		CR[AttackID.ID].ReduceMana(siMana);
	}

	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CLAW, 0, AttackArea, 0, AttackID, m_ssAttackX, m_ssAttackY, 100);
}

void _Char::FireOperation_Claw(SI32 attackinstant, SI32 hitrate)
{
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		if(GetAttackEffect())
		{
#ifndef _USE_HASHCODE_EFFECTSOUND
			PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
			HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
		}
		else
		{
			PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
		}
	}
	
	// 전투에 따라 이벤트 마크와 전투 지역 설정 등을 담당한다. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CLAW, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Dueoksini(SI32 attackinstant, SI32 hitrate)
{
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		if(GetAttackEffect())
		{
#ifndef _USE_HASHCODE_EFFECTSOUND
			PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
			HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
		}
	}
	
	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_DUEOKSINI, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Toad(SI32 attackinstant, SI32 hitrate)
{
	
	if(IsAlive(AttackID) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_TOADPOISON)
		{
			// 치료력을 줄여준다. 
			ReduceMana(MAGICPOWER_TOADPOISON);
			
			CR[AttackID.ID].m_cltPoisonEffect.Set(TRUE, 500, m_ParmeterBox.CalcApower() / 2);
		}
	}
	
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
	}
	
	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_TOAD, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_CannonJ(SI32 attackinstant, SI32 hitrate)
{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		{
			PushEffect(EFFECT_CANNONJ, m_ssX, m_ssY);
		}
		
		// 유효한 지도 영역 안에 있어야 한다.
		if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
		{
			SI32 smokeid = FindEmptySmoke();
			if(smokeid)
			{
				SetSmoke(KIND_SMOKE_DUST, smokeid, MapCenterX[m_ssAttackX][m_ssAttackY], MapCenterY[m_ssAttackX][m_ssAttackY], AttackArea);
			}
		}
		else
		{
			clGrp.Error("cannonj", "139663");
		}
		
		// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
		SetBattleFieldNEventMark();
		
		CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CANNONJ, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Elephant(SI32 attackinstant, SI32 hitrate)
{
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		PushEffect(EFFECT_CANNONJ, m_ssX, m_ssY);
	}
	
	
	if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
	{
		SI32 smokeid = FindEmptySmoke();
		if(smokeid)
		{
			SetSmoke(KIND_SMOKE_DUST, smokeid, MapCenterX[m_ssAttackX][m_ssAttackY], MapCenterY[m_ssAttackX][m_ssAttackY], AttackArea);
		}
	}
	else
	{
		clGrp.Error("elephant", "139663");
		
	}
	
	
	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CANNONJ, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Sub1(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_QUEENSUB1, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Sub2(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_QUEENSUB2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_BigSword(SI32 attackinstant, SI32 hitrate)
{
	
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		if(GetAttackEffect())
		{
#ifndef _USE_HASHCODE_EFFECTSOUND
			PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
			HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
		}
		else
		{
			PushEffect(EFFECT_BIGSWORD, m_ssX, m_ssY);
		}
	}
	
	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
	SetBattleFieldNEventMark();
	
	
	/*			// 용 비늘검이 있는 경우 용소환 가능. (기존의 용이 없어야 한다. )
	// 마법력이 기준 이상이 되어야 한다. 
	else if(HaveItem(GOODS_SWORD7) && IsAlive(Slave2ID) == FALSE
	&& m_ParmeterBox.GetMana() >= MAGICPOWER_DRAGON)
	{
	SI16 emptyx, emptyy;
	if(FindEmptyAreaAir(emptyx, emptyy, 1, 1)==TRUE)
	{
	
	  id = GD.Player[GetPlayerNumber()].FindPlayerEmptyID(KIND_ATTACKERC, FALSE);
	  if(id)
	  {
	  // 캐릭터를 생산한다. 
	  SetChar(KIND_ATTACKERC, id, emptyx, emptyy, Direction, 100, GetPlayerNumber());
	  //CR[id].TransparentSwitch = TRUE;
	  CR[id].m_siTimerDelay = 10;
	  CR[id].HostID = m_NewID;
	  
		Slave2ID = CR[id].m_NewID;
		
		  // 마법력을 줄인다. 
		  m_ParmeterBox.SetMana(m_ParmeterBox.GetMana() - MAGICPOWER_DRAGON);
		  
			}
			}
			}
	*/


	{
		CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_SWORD, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	}
	
	
}

void _Char::FireOperation_Gun(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 smokeid1;
	
	if(IsSee(AttackID.ID))
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		SI32 apower = m_ParmeterBox.CalcApower();
		
		// 운이 좋으면 필살기로 작동한다.
		if(IsAtb3(ATB3_SPECIALATTACK) == TRUE)
		{
			if(GD.Random() % 12 == 0)
			{
				apower = CalcDamage(SPECIAL_SPECIALATTACK, apower, m_scMagicApplyRate);
				
				SI32 smokeid = FindEmptySmoke();
				if(smokeid)
				{
					hitrate += 20;
					if(hitrate>100)	hitrate = 100;
					SetSmoke(KIND_SMOKE_SPECIAL, smokeid,m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AttackArea);
				}
			}
		}

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(FireRepeatNumber, attackstartx, attackstarty);

			// 연발 사격은 연기를 발생시킨다. 
			if(GetAttackAtb() == ATTACKATB_MACHINEGUN)
			{
				SI32 smokeid = FindEmptySmoke();
				if(smokeid)
				{
					SetSmoke(KIND_SMOKE_MAGIC8K_EXP, smokeid, x+ 16 - GD.Random() % 32, y+ 16 - GD.Random() % 32, AttackArea);
				}
			}
			
			if( IsAtb2(ATB2_GUNSMOKE) == TRUE )
			{
				// 총알 발사 연기 
				smokeid1=FindEmptySmoke();
				if(smokeid1)
				{
					SetSmoke(KIND_SMOKE_GUNSMOKE, smokeid1, attackstartx, attackstarty, AttackArea);
				}
			}

			// 귀신탄
			if(IsAtb3(ATB3_GHOST) == TRUE)
			{
				SetBullet(KIND_BULLET_GHOST, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			}
			// 철갑탄 
			else if(IsAttackAtb(ATTACKATB_STRONGGUN)==TRUE)
			{
				SetBullet(KIND_BULLET_STRONGGUN, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			}
			else
			{
				SetBullet(KIND_BULLET_GUN, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			}
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				if(GetAttackEffect())
				{
#ifndef _USE_HASHCODE_EFFECTSOUND
					PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
					HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
				}
				else
				{
					PushEffect(EFFECT_GUN, m_ssX, m_ssY);
				}
			}
		}
		
		// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
		SetBattleFieldNEventMark();
	}
}



void _Char::FireOperation_Electron(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
		
		if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
		{
			// 공격에 대한 정보를 저장한다. 
			//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
			SetFireInfo(AttackID);
			
			id=FindEmptyBullet();
			if(id)
			{
				SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
				SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
				
				// 빠지직을 만든다. 
				ElectronSwitch = TRUE;
				ElectronInfo1  = m_cltCharStatus.GetCenterX();
				ElectronInfo2  = m_cltCharStatus.GetCenterY();
				ElectronInfo3  = x;
				ElectronInfo4  = y;
				
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_GENERAL4KATTACK, m_ssX, m_ssY);
				
				
				if(m_cltCharDrawInfo.GetAniStep() == attackinstant)
				{
					
					
					// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
					SetBattleFieldNEventMark();
					
					CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_ELECTRON, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
					
					// 감전된 모습을 보여주게 한다. 
					CR[AttackID.ID].m_cltElectronShock.SetElectronDraw(TRUE);
					
					// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
					if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
						PushEffect(EFFECT_ELECTRIC, m_ssX, m_ssY);
					
				}
				
			}
		}
	
}


void _Char::FireOperation_GatphaPoison(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if( IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE )
	{
		// 공격에 대한 정보를 저장한다. 
		// 다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_GATPHAPOISON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
	
}

void _Char::FireOperation_Magic2(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_BUDAFIRE2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Magician(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_MAGICIANBULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Magic3(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if( IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE )
	{				
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_BUDAFIRE3, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
	
}

void _Char::FireOperation_Justicek(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		
			// 마법력이 충분할 때는 적을 마비시키는 능력을 발휘할 수 있다. 
			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_JUSTICEK)
			{
				 // 마법력을 줄여준다. 
				ReduceMana(MAGICPOWER_JUSTICEK);

				SetBullet(KIND_BULLET_JUSTICEK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			else
			{
				SetBullet(KIND_BULLET_BUDAFIRE2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FIRE_JUSTICEK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_MagicBall(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(m_ParmeterBox.CalcApower() > 70)
			{
				
				SetBullet(KIND_BULLET_MAGICBALL2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			else
			{
				SetBullet(KIND_BULLET_MAGICBALL1, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			
			
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
			
		}
	}
	
}

void _Char::FireOperation_Arrow(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		// 운이 좋으면 필살기로 작동한다.
		if(IsAtb3(ATB3_SPECIALATTACK) == TRUE)
		{
			if(GD.Random() % 12 == 0)
			{
				apower = CalcDamage(SPECIAL_SPECIALATTACK, apower, m_scMagicApplyRate);
				
				SI32 smokeid = FindEmptySmoke();
				if(smokeid)
				{
					hitrate += 20;
					if(hitrate>100)	hitrate = 100;
					SetSmoke(KIND_SMOKE_SPECIAL, smokeid,m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AttackArea);
				}
			}
		}

		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAtb3(ATB3_FIREARROW))
			{
				apower = CalcDamage(SPECIAL_FIREARROW, apower, m_scMagicApplyRate);
				SetBullet(KIND_BULLET_FIREARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
			else if(IsAtb3(ATB3_FIREARROW2) && ReduceMana(MAGICPOWER_FIREARROW2) == TRUE)
			{
				apower = CalcDamage(SPECIAL_FIREARROW2, apower, m_scMagicApplyRate);
				SetBullet(KIND_BULLET_BURNARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
			else
			{
				SetBullet(KIND_BULLET_ARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
}

void _Char::FireOperation_Needle(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_NEEDLE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
		}
	}
	
}

void _Char::FireOperation_Snowman(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetSnowMagic(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), hitrate, AttackID);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
	
}


void _Char::FireOperation_FurHeadDemon(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_FURHEADDEMON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				if(GetAttackEffect())
				{
#ifndef _USE_HASHCODE_EFFECTSOUND
					PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
					HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
				}
			}
		}
	}
}


void _Char::FireOperation_Dart(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[CR[AttackID.ID].m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_DART, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
	
}

void _Char::FireOperation_AirMissile(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		if(id)
		{
			SHORT x = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_AIRMISSILE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, 
				m_cltCharStatus.GetCenterX()+8-GD.Random()%16, 
				m_cltCharStatus.GetCenterY()+8-GD.Random()%16, 
				x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower() * 3 / 2  , hitrate, 0, AttackArea);
			
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SAMC, m_ssX, m_ssY);
			
		}
	}
	
	
}


void _Char::FireOperation_SamMissile(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_SAMMISSILE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, 
				m_cltCharStatus.GetCenterX()+8-GD.Random()%16, 
				m_cltCharStatus.GetCenterY()+8-GD.Random()%16, 
				x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			if( IsAtb2(ATB2_SAMC) )
			{
				if(FireRepeatNumber==1)
				{
					// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
					if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
						PushEffect(EFFECT_SAMC, m_ssX, m_ssY);
				}
			}
			else
			{
				
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_SAM, m_ssX, m_ssY);
			}
		}
	}
	
	
	
}

void _Char::FireOperation_Cannon(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_CANNON, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_CANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
	
}

void _Char::FireOperation_TowerCannon(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_CANNON, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			{
				SetBullet(KIND_BULLET_CANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
	
}

void _Char::FireOperation_HeavyGun(SI32 attackinstant, SI32 hitrate)
{
	SHORT length;
	SHORT realdx, realdy;
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SMALLCANNON, m_ssX, m_ssY);
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()% max(1, (length/2));
			realdy=y-(length/4)+GD.Random()% max(1, (length/2));
			
			
			SetBullet(KIND_BULLET_HEAVYGUN, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
	
}

void _Char::FireOperation_ShipCannon(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(FireRepeatNumber==1)
			{
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_SHIPCANNONDOUBLE, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_SHIPCANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
}

void _Char::FireOperation_ThrowBomb(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(FireRepeatNumber==1)
			{
				
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_SHIPCANNONDOUBLE, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			
			SetBullet(KIND_BULLET_THROWBOMB, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
	
}

void _Char::FireOperation_Cannon2C(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	FireRepeatNumber++;
	if(FireRepeatNumber>=MaxFireRepeatNumber)
	{
		FireRepeatNumber=0;
		FireRepeatDelayor=0;
	}
	
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	id=FindEmptyBullet();
	if(id)
	{
		if(FireRepeatNumber==0)
		{
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				// 대장군포의 효과음. 
				if( IsAtb2(ATB2_CANNON2KFIRE) )
				{
					PushEffect(EFFECT_CANNON2KFIRE, m_ssX, m_ssY);							
				}
				else
				{
					PushEffect(EFFECT_CANNON2CFIRE, m_ssX, m_ssY);
				}
			}
		}
		
		SetBullet(KIND_BULLET_SHIPCANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), HitPointX, HitPointY, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
		
		// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
		SetBattleFieldNEventMark();
		
	}
	
}

void _Char::FireOperation_Bomb(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 주변을 보이게 만든다.
		if(FireRepeatNumber==0)
		{
			// 주변을 보이게 만든다.
		}
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(FireRepeatNumber==1)
			{
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_BOMB, m_ssX, m_ssY);
			}
			
			SHORT startx=m_cltCharStatus.GetCenterX()+10-GD.Random()%20;
			
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_BOMB, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, 
				startx, m_cltCharStatus.GetCenterY(), 
				startx, m_cltCharStatus.GetCenterY()+40+5-GD.Random()%10, 
				m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
		}
	}
	
	
	
}

void _Char::FireOperation_FireCar(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		if(FireRepeatNumber==0)
		{
		}
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/12)+GD.Random()%max(1, (length/6));
			realdy=y-(length/12)+GD.Random()%max(1, (length/6));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_FIRETANKARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, 
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, TargetDirection, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_TANK, m_ssX, m_ssY);
			
			
		}
	}
	
	
}


void _Char::FireOperation_Fire(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_FIRE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FLAME, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_FireUsingMana(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);

		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_FIRE)
		{
			// 마법력을 줄여준다. 
			ReduceMana(MAGICPOWER_FIRE);
			
			id=FindEmptyBullet();
			if(id)
			{
				SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
				SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
				
				length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
				
				realdx=x-(length/4)+GD.Random()%max(1, (length/2));
				realdy=y-(length/4)+GD.Random()%max(1, (length/2));
				
				// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
				SetBattleFieldNEventMark();
				
				int attackstartx, attackstarty;
				DecideAttackPoint(0, attackstartx, attackstarty);
				
				SetBullet(KIND_BULLET_FIRE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
				
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_FLAME, m_ssX, m_ssY);
			}
		}
	}

}

void _Char::FireOperation_GoldDragonElectron(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_GOLDDRAGONELECTRON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_GOLDDRAGONFIRE, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_Onghwa(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_ONGHWA, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_ONGHWAATTACK, m_ssX, m_ssY);
		}
	}
	
}


void _Char::FireOperation_GhostCat(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_GHOSTCAT, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Kuket(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_KUKET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}	
}

void _Char::FireOperation_CatDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}		
			SetBullet(KIND_BULLET_CATDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_OldFoxDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_OLDFOXDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_RaccoonDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_RACCOONDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_SnowWomanDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_SNOWWOMANDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_YangGuiBiDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;				
			}
			SetBullet(KIND_BULLET_YANGGUIBIDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_JijangDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;				
			}
			SetBullet(KIND_BULLET_JIJANGDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_SnakeWomanDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;				
			}
			SetBullet(KIND_BULLET_SNAKEWOMANDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}


void _Char::FireOperation_NinjaDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;				
			}
			SetBullet(KIND_BULLET_NINJADOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Snow(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_SNOW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Doll1(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_DOLL1, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
	
}


void _Char::FireOperation_Doll2(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;
				
			}
			
			SetBullet(KIND_BULLET_DOLL2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
	
}

void _Char::FireOperation_Butterfly(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			apower = m_ParmeterBox.CalcApower() ;
			
			SetBullet(KIND_BULLET_BUTTERFLY, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Widow(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 apower = 0;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{

			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_WIDOW)
			{
				// 마법력을 줄여준다. 
				ReduceMana(MAGICPOWER_WIDOW);
				
				SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
				SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
				
				apower = m_ParmeterBox.CalcApower() ;
				
				SetBullet(KIND_BULLET_WIDOW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				
				// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
				SetBattleFieldNEventMark();
				
				// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
		}
	}
	
}


void _Char::FireOperation_Wave(SI32 attackinstant, SI32 hitrate)
{

		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_WAVE)
		{
			// 치료력을 줄여준다. 
			ReduceMana(MAGICPOWER_WAVE);
		}
		else
		{
			return ;
		}

		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		{
			PushEffect(EFFECT_TUTTLEWAVE, m_ssX, m_ssY);
		}
		
		// 유효한 지도 영역 안에 있어야 한다.
		if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
		{
			SI32 smokeid = FindEmptySmoke();
			if(smokeid)
			{
				SetSmoke(KIND_SMOKE_WAVEEFFECT, smokeid, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AttackArea);
			}
		}
		else
		{
			clGrp.Error("cannonj", "139663");
		}
		
		// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
		SetBattleFieldNEventMark();
		
		CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_WAVE, 3, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Magic4T(SI32 attackinstant, SI32 hitrate)
{
	if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_MAGIC4T)
	{
		ReduceMana(MAGICPOWER_MAGIC4T);
	}
	else 
		return;

	SetFireInfo(AttackID);

	if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
	{
		SpecialAct(SPECIAL_MAGIC4T,AttackID, m_ssAttackX, m_ssAttackY );
	}
	else
	{
		clGrp.Error("cannonj", "139663");
	}
	
	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
	SetBattleFieldNEventMark();

}

void _Char::FireOperation_Ice(SI32 attackinstant, SI32 hitrate)
{
	if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_ICE)
	{
		ReduceMana(MAGICPOWER_ICE);
	}
	else 
		return;

	SetFireInfo(AttackID);

	if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
	{
		// 마법에 의해서 레벨이 오르지 않도록 ZeroID를 넣는다. 
		SetMagicIce(m_ssX, m_ssY, ZeroID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), m_ParmeterBox.GetHitRate(0), ZeroID);
	}
	else
	{
		clGrp.Error("cannonj", "139663");
	}
	
	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
	SetBattleFieldNEventMark();

}

void _Char::FireOperation_KimBomb(SI32 attackinstant, SI32 hitrate)
{

		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_KIMBOMB)
		{
			// 치료력을 줄여준다. 
			ReduceMana(MAGICPOWER_KIMBOMB);
		}
		else
		{
			return ;
		}

		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		{
			//PushEffect(EFFECT_CANNONJ, X, Y);
		}
		
		// 유효한 지도 영역 안에 있어야 한다.
		if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
		{
			// 마법에 의해서 레벨이 오르지 않도록 ZeroID를 넣는다. 
			SetKimBomb(m_ssX,m_ssY,ZeroID,GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), m_ParmeterBox.GetHitRate(0), ZeroID);
		}
		
		// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
		SetBattleFieldNEventMark();

}

void _Char::FireOperation_Leech(SI32 attackinstant, SI32 hitrate)
{
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
	}
	
	// 전투에 따라 이벤트 마크와 전투 지역 설정 등을 담당한다. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_LEECH, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}


void _Char::FireOperation_SeaHorse(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_SEAHORSE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_PoisonArrow(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower();
		
		// 운이 좋으면 필살기로 작동한다.
		if(IsAtb3(ATB3_SPECIALATTACK) == TRUE)
		{
			if(GD.Random() % 12 == 0)
			{
				apower = CalcDamage(SPECIAL_SPECIALATTACK, apower, m_scMagicApplyRate);
				
				SI32 smokeid = FindEmptySmoke();
				if(smokeid)
				{
					hitrate += 20;
					if(hitrate>100)	hitrate = 100;
					SetSmoke(KIND_SMOKE_SPECIAL, smokeid,m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), AttackArea);
				}
			}
		}

		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_POISONARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
		}
	}
}


void _Char::FireOperation_ThrowSpear(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_THROWSPEAR, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
		}
	}
}



void _Char::FireOperation_Cactus(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_CACTUSBULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
		}
	}
	
}

void _Char::FireOperation_Scorpion(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_SCORPIONBULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
		}
	}
	
}

//이사의 사념 공격.
void _Char::FireOperation_Evil(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		
			SetBullet(KIND_BULLET_EVIL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FIRE_JUSTICEK, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_VolcanoFlame(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);

			
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_VOLCANOFLAME, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FLAME, m_ssX, m_ssY);
		}
	}
	
}

void _Char::FireOperation_BigTree(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_BIGTREE)
			{
				// 치료력을 줄여준다. 
				ReduceMana(MAGICPOWER_BIGTREE);
			}
			else
			{
				return ;
			}

			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBigTree(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower()/4, hitrate, AttackID);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

void _Char::FireOperation_Auta(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_AUTA, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_JapanGhost(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	SI32 length;
	SHORT realdx, realdy;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_JAPANGHOST, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_PaperDoll(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_PAPERDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_NeedleAttack(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_NEEDLEATTACK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_CloudWind(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	SI32 apower = m_ParmeterBox.CalcApower();
	// 랜덤하게 풍모술이 발휘된다.
	if(IsAtb4(ATB4_CLOUD_WIND) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_CLOUD_WIND)
		{		
			if(GD.Random() % 3 == 0 && CR[AttackID.ID].m_CloudWindFrame==0)
			{
				SHORT x = CR[AttackID.ID].GetX();
				SHORT y = CR[AttackID.ID].GetY();
				
				ReduceMana(MAGICPOWER_CLOUD_WIND);
				SpecialActAtb4(SPECIAL_CLOUD_WIND, AttackID, x, y);
			}
		}
	}

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_CLOUDWIND, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_Seimei(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// robypark 2004/9/13 13:30
			// 세이메이 공격의 원래의도인 원거리 공격이 되도록 수정
			SetBullet(KIND_BULLET_SOULCHANGE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 20, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_MagicCAttack(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_MAGICCATTACK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

/*void _Char::FireOperation_Rabbit(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_RABBIT, id, X, Y, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(X, Y)==FALSE)
				PushEffect(EFFECT_BUDA, X, Y);
		}
	}
}*/

void _Char::FireOperation_Sunbiyoun(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_SUNBIYOUNATTACK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

// 이무기 원거리 마법공격
void _Char::FireOperation_Python(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		
			SetBullet(KIND_BULLET_PYTHON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FIRE_JUSTICEK, m_ssX, m_ssY);
		}
	}

}

// 진시황의혼 원거리 마법공격
void _Char::FireOperation_JinEmperor(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		
			SetBullet(KIND_BULLET_JINEMPEROR, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FIRE_JUSTICEK, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_GuardianPig(SI32 attackinstant, SI32 hitrate)
{
	// 공격에 대한 정보를 저장한다. 
	//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
	SetFireInfo(AttackID);
	
	// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		if(GetAttackEffect())
		{
#ifndef _USE_HASHCODE_EFFECTSOUND
			PushEffect(GetAttackEffect(), m_ssX, m_ssY);
#else
			HashCodePushEffect(GetAttackEffect(), m_ssX, m_ssY);
#endif
		}
		else
		{
			PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
		}
	}
	
	// 전투에 따라 이벤트 마크와 전투 지역 설정 등을 담당한다. 		
	SetBattleFieldNEventMark();

	// 삼키는 중에는 다시 공격을 할 수 없다.
	// 삼키는 중에는 다시 공격을 할 수 없다.
	if(!m_bPigAttack)
	{
		m_dwPigAttackDelay = timeGetTime();
		m_bPigAttack = TRUE;
		m_cltCharDrawInfo.SetAnimation(ANI_WAIT0);
		OrderStop(m_NewID, BY_COMPUTER);
		
		CR[AttackID.ID].m_dwPigAttackDelay = timeGetTime();
		CR[AttackID.ID].m_bPigDefence = TRUE;				// Action 불능으로 만든다.
		CR[AttackID.ID].m_cltCharDrawInfo.SetDrawSwitch(FALSE);	// Draw   불능으로 만든다.
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 조선-거북차, 이순신 2차 전직
void _Char::FireOperation_TURTLE_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();

			SetBullet(KIND_BULLET_TURTLE_FIREBALL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_FIRE_KU102AE1, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 조선-뇌전차, 허준 2차 전직
void _Char::FireOperation_THUNDER_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_THUNDER_BOLT, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_FIRE_KU103AE2, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 일본-지진차, 와키자카 2차 전직
void _Char::FireOperation_QUAKE_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SHORT smokeid;

	// 적에게 타격을 준다. 
	SHORT level = m_ParmeterBox.IP.GetLevel();

	SI32 apower = CalcDamage(SPECIAL_EARTHQUAKE, m_ParmeterBox.CalcApower(), \
						m_scMagicApplyRate, KI[m_cltCharStatus.GetKind()].m_siKindAtb); 

//	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_EARTHQUAKE, 2, AREA_LAND, 
	// robypark 2004/6/11 10:32
	// 물리속성 데미지를 주도록 수정
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_QUAKE_VEHICLE, 2, AREA_LAND, 
		 apower, AttackID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, 
		 hitrate);
//		 m_ParmeterBox.GetHitRate(0));

	// 지진 웨이브 이펙트를 연출한다.
	smokeid = FindEmptySmoke();
	if(smokeid)
	{
		// 지진차 공격효과음
		PushEffect(EFFECT_FIRE_JU104AE1, CR[m_NewID.ID].m_ssX, CR[m_NewID.ID].m_ssY);

		// 화면 흔들림을 만든다. 
	    GD.EarthQuake.SetEarthQuake();

		// 바닥용 Smoke으로 보여줘여 함으로 마지막 인수로 TRUE를 넘겨준다.
		SetSmoke(KIND_SMOKE_EARTHQUAKE_WAVE, smokeid, CR[AttackID.ID].m_cltCharStatus.GetCenterX(), CR[AttackID.ID].m_cltCharStatus.GetCenterY(), AREA_LAND, -1, TRUE);

		// 땅울림 효과음
		PushEffect(EFFECT_EXP_JU104AE2, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
	}

     OrderStop(m_NewID, BY_COMPUTER);
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 일본-흑룡차, 세이메이 2차 전직
void _Char::FireOperation_BLACK_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_POISON_ICE_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_FIRE_JU105AE1, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 중국-발석거, 순비연<=이령 2차 전직
void _Char::FireOperation_CATAPULT(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();

			SetBullet(KIND_BULLET_CATAPULT_FIRE_STONE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 40, \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_FIRE_CU106AE1, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 중국-불랑기포차, 이령<=순비연 2차 전직
void _Char::FireOperation_FARANGI_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
		SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();

		if (id)
		{
			// 배의 대포 발사(포물선이 아니라 직선으로 수정됨)
			SetBullet(KIND_BULLET_FARANGI_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				TargetX, TargetY, m_NewID, AttackID, apower * 0.34, hitrate, \
				0, AttackArea);

			// 총알 발사에 시차를 두기 위한 작업
			Bullet_SetLuanchDelay(id, 1);
			Bullet_DrawSwitch(id, FALSE);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
//			PushEffect(EFFECT_CANNON, X, Y);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}

		id = FindEmptyBullet();
		
		if (id)
		{
			// 배의 대포 발사(포물선이 아니라 직선으로 수정됨)
			SetBullet(KIND_BULLET_FARANGI_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX() + 15, m_cltCharStatus.GetCenterY(), \
				TargetX + 15, TargetY, m_NewID, AttackID, apower * 0.33, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
//			PushEffect(EFFECT_CANNON, X, Y);
			PushEffect(EFFECT_FIRE_CU107AE1, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	
		id = FindEmptyBullet();

		if (id)
		{
			// 배의 대포 발사(포물선이 아니라 직선으로 수정됨)
			SetBullet(KIND_BULLET_FARANGI_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX() - 15, m_cltCharStatus.GetCenterY(), \
				TargetX - 15, TargetY, m_NewID, AttackID, apower * 0.33, hitrate, \
				0, AttackArea);

			// 총알 발사에 시차를 두기 위한 작업
			Bullet_SetLuanchDelay(id, 2);
			Bullet_DrawSwitch(id, FALSE);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
//			PushEffect(EFFECT_CANNON, X, Y);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 대만-화룡차, 손유창 2차 전직
void _Char::FireOperation_FIRE_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK2);

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// 화룡출수 발사
			SetBullet(KIND_BULLET_FIRE_DRAGON_MISSILE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 40, \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_FIRE_TU108AE2, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
// 대만-봉황비조, 장선화 2차 전직
void _Char::FireOperation_CHINESEPHOENIX(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_CHINESEPHOENIX_FEATHER, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_FIRE_TU109AE1, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

// robypark (2004/6/4 13:6)
// 앙천대소 공격, 불랑기포차, 126
// ATTACKATB_ROAR_OF_LAUGHTER
void _Char::FireOperation_ROAR_OF_LAUGHTER(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	// robypark 2004/11/11 11:20
	// 앙천대소 공격일 때 마나소모하도록 수정
	if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_SIEGE_MODE_FIRE_MISSILE)
	{
		ReduceMana(MAGICPOWER_SIEGE_MODE_FIRE_MISSILE);
	}
	else 
	{
//		m_scBusySwitch=FALSE;

		// 마나가 없을 경우 앙천대소=>불랑기포차로 자동 변신
//		OrderSIEGE_MODE_UNDO(m_NewID, m_ssX, m_ssY, ZeroID, BY_COMPUTER);

		return;
	}
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{		
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = CalcDamage(SPECIAL_SIEGE_MODE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate);

		// robypark 2004/11/2 11:28
		// 앙천대소 명중률 + Alpha(50) 추가
		hitrate += 50;
		if (hitrate > 100)
			hitrate = 100;

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_ROAR_OF_LAUGHTER_BOMB, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 30, \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			PushEffect(EFFECT_SAMC, m_ssX, m_ssY);

			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
		}
	}
}

// robypark 2004/7/14 16:42
// 에비스 인형 공격
// ATTACKATB_DOLL_EVIS
void _Char::FireOperation_DOLL_EVIS(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;				
			}
			SetBullet(KIND_BULLET_DOLL_EVIS, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

// robypark 2005/2/2 17:27
// 달마 인형 공격
// ATTACKATB_DOLL_DHARMA
void _Char::FireOperation_DOLL_DHARMA(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// 공격에 대한 정보를 저장한다. 
		//다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
		SetFireInfo(AttackID);	
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			if(IsAlive(AttackID) == TRUE 
				&& KI[ CR[AttackID.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))
			{
				apower = m_ParmeterBox.CalcApower() * 2;
			}
			else
			{
				apower = m_ParmeterBox.CalcApower() ;				
			}
			SetBullet(KIND_BULLET_DOLL_DHARMA, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
			SetBattleFieldNEventMark();
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}
