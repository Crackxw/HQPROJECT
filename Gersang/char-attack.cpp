////////////////////////////////////////////////////////////////////
////  ���� ���� 
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
extern	_Char				CR[];								// ĳ������ ������ 
extern	_NewID 				ZeroID;
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_YSMap 				SMap;								// ���� ������ ������ 

extern SI32 AttackAtbInfo[ATTACKATB_MAX];

// robypark 2004/6/14 15:51
extern	CItem					*g_pItem;

// robypark 2004/10/18 12:17
// �� ���� �Ӽ������� ĳ���� ���� �Ӽ� ���� ���� �����
#define PRIORITY_ATTACK_TYPE_CYCLE				0		// ���� ó�� ������ ����
#define PRIORITY_ATTACK_TYPE_CLOSE_RANGE		1		// �ٰŸ� �켱
#define PRIORITY_ATTACK_TYPE_LONG_RANGE			2		// ��Ÿ� �켱

// ���� ȿ������ ���´�. 
SI32 cltCharOrg::GetAttackEffect() const
{
	switch(m_siCurrentAttack)
	{
	case 0: return KI[m_cltCharStatus.GetKind()].m_siAttackEffect1; break;
	default: return KI[m_cltCharStatus.GetKind()].m_siAttackEffect2; break;
	}
	return 0;
}

// ���� ������ ���´�. 
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

// ���� ������ �����Ѵ�.
void cltCharOrg::SetAttackRange(SI32 range)
{
	switch(m_siCurrentAttack)
	{
	case 0:  m_siAttackRange1 = range; break;
	default: m_siAttackRange2 = range; break;
	}
	
}

// ���� �Ӽ��� ���´�. 
SI32 cltCharOrg::GetAttackAtb()const 
{
	switch(m_siCurrentAttack )
	{
	case 0: return m_siAttackAtb1; break;
	default: return m_siAttackAtb2; break;
	}
	return 0;
}

// ���� �Ӽ��� �����Ѵ�.
void cltCharOrg::SetAttackAtb(SI32 atb)
{
	switch(m_siCurrentAttack)
	{
	case 0:  m_siAttackAtb1 = atb; break;
	default: m_siAttackAtb2 = atb; break;
	}
}

////////////////////////////////////////////////////////////////////
// attack_id ���� �����ϰ� ����� �Լ� 
BOOL _Char::SetAttack(_NewID id)
{
	// ���� ��ǥ�� �̹� �׾��ų� ���� �ƴϸ� �������� �ʴ´�. 
	if(IsAlive(id.ID)==FALSE)return FALSE;

	// �ڱ� �ڽ��� ������ �� ����. 
	if(id.IsSame(m_NewID))return FALSE;

	AttackID = id;

	// ���� ĳ�����ΰ� �ƴϸ� ���� ĳ�����ΰ�?
    AttackArea = CR[AttackID.ID].m_cltCharStatus.GetArea();

	AttackGiveUpDelayor	  =	0;
    AttackMode=ATTACK_MODE_CHAR;

    return TRUE;
}

// ������ �����ϰ� ����� �Լ� 
BOOL _Char::SetAttackGround(SI32 six, SI32 siy)
{

	AttackID = ZeroID;

	// ���� ĳ�����ΰ� �ƴϸ� ���� ĳ�����ΰ�?
    AttackArea=AREA_LAND;

	AttackGiveUpDelayor	  =	0;
    AttackMode=ATTACK_MODE_GROUND;

    return TRUE;
}



// fire������ Ȯ�� 
BOOL _Char::ReadyToFire()
{
	// ĳ������ ������ ��ǥ�� ���ϰ� �ִ��� Ȯ���Ѵ�. 
	if(ChangeDirection(TargetDirection)==FALSE)
		return FALSE;

	return TRUE;
	
	return FALSE;
}


// �������� ������ ���� ���� ó���ؾ� �� ���� ó���Ѵ�. 
BOOL _Char::PreAttack()
{
	return TRUE;
}


BOOL _Char::Escape()
{
	// ��� ��尡 �����Ǿ� �ְ� ����� ���� ��ü�� ���̸�, 
	if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
	{
		// �̵����� �ʴ´�.
		m_scBusySwitch = FALSE;
		return TRUE;
	}
	// ���� �����ϱ� ���� �Ÿ��� ��췯 �̵��Ѵ�.
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
			 
	// ��ǥ�� ������ ���Ѵ�. 
	TargetDirection=DecideDirection8(m_ssX, m_ssY, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
	
	
	
	// �ٸ� �غ� �Ǿ����� Ȯ���Ѵ�. 
	if( ReadyToFire() == FALSE )
		return TRUE;
	 
	
	// ��Ȳ�� ���� �ʻ�⸦ ���⵵ �Ѵ�.
	if( IsStrongAttackAvailable() == TRUE && m_CloudWindFrame <= 0)
	{
		// ������ ��� ���� �ǹ��� �����Ѵ�. 
		if( IsAtb(ATB_CAPTURE) == TRUE )
		{
			OrderCapture(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_PLAYER);
		}
		// ������ ��� ���� ��Ȥ�Ѵ�.
		else if( IsAtb(ATB_BETRAY) == TRUE )
		{
			OrderBetray(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
		}
		// �� �º��� ��� 
		else if( IsAtb2(ATB2_STEALEXP) == TRUE )
		{
			
			if(OldOrder.Order==ORDER_WAIT)
				SetOldOrder(ORDER_ATTACK, HostMode,  FALSE, m_ssAttackX|(m_ssAttackY<<16), AttackID);
			
			OrderStealExp(m_NewID, BY_COMPUTER);
		}
		// ������, 
		else if( IsAtb3(ATB3_LIGHTNING) == TRUE )
		{
			OrderLightning(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
		}
		// actdoll (2004/03/10 10:29) : 
		//	�� �κ��� �ϴ� IsStrongAttackAvailable() �Լ� ���� �ű��.
		//	�� �� ������ �ΰ��� �̻��� ����� �� �� �ְ� �Ǹ鼭 ������ �ߺ����� ���Ͽ� �������� ���ڼ��� ���������� 
		//	�۵��� ���� ���ϰ� �ȴ�. ���� �ϴ� ���� ��ġ���� �۵��� �� �ֵ��� �۾��� �����.
		//	���� �ݵ�� �����Ǿ� �� �κ��߿� �ϳ���.
		// �������� 
//		else if( IsAtb3(ATB3_GROUNDEXP) == TRUE )
//		{
//			OrderGroundExp(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
//		}
		// ���ڼ��� 
//		else if( IsAtb3(ATB3_MAGIC6C) == TRUE )
//		{
//			OrderMagic6c(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
//		}
		// �̻��� ��� Ư������.
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
		// ������ �ߴ��Ѵ�.
		return FALSE;
	}

			 
	// ��ǥ�� ������ ���Ѵ�. 
	TargetDirection=DecideDirection8(m_ssX, m_ssY, six, siy);
	
	
	// �ٸ� �غ� �Ǿ����� Ȯ���Ѵ�. 
	if(ReadyToFire()==FALSE)return TRUE;
			 
	WorkStep=WS_FIRE;
	
	AttackGiveUpDelayor=0;
	
	m_ssAttackX = six;
	m_ssAttackY = siy;

	return TRUE;
}


// ĳ���� ���� 
BOOL _Char::Attack()
{
	SHORT status;
	SHORT direction=0;
	SI32 six = LOWORD(Para1);
	SI32 siy = HIWORD(Para1);
	
	if(AttackMode == ATTACK_MODE_CHAR)
	{
		// ���� ����� ��ȿ�ϰ� ���ձ��̸�, 
		if(IsSee(AttackID) && IsFriendToByPlayerNumber(GetPlayerNumber(), CR[AttackID.ID].GetPlayerNumber())==TRUE
			&& m_scBusySwitch == FALSE)
		{
			
			if(abs(CR[AttackID.ID].m_cltCharStatus.GetChangedPlayerFrame()-GD.Frame) < 20  // �÷��̾ ����� �� �� �ȵ� ���̶��,    
				|| HostMode==BY_COMPUTER                                // ����� ��ü�� ���̰ų�
				|| GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER)        // ������ ��ü�� ���̸�, 
			{
				return FALSE;
			}
		}
	}

	switch(WorkStep)
	{
	case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 
		
		// ���ֿ� ���� �����̶��, 
		if(AttackMode == ATTACK_MODE_CHAR)
		{
			
			// ���� ������ �� ���� ���¶�� �ߴ��Ѵ�.
			if(CanAttack(AttackID)==FALSE)
			{   
				
				m_scBusySwitch=FALSE;
				
				// �ֺ��� �����Ѵ�. 
				SearchForwardSwitch = TRUE;
				
				return FALSE;
			}
			
			// robypark 2004/10/18 12:19
			// �� ���� ���� �Ӽ��� �ְ�, ������ ���� �����ϴ� �ΰ����� ���� �ƴ� ���
			if ((PRIORITY_ATTACK_TYPE_CYCLE != m_ssPriority_Attack_Type) && (m_siAttackAtb2))
			{
				// �� ��° ���� �Ӽ��� �� ���Ÿ� ������ �Ѵ�.
				if (m_siAttackRange1 < m_siAttackRange2)
				{
					// �ٰŸ� ���� �켱 �Ӽ��� ��� �ٰŸ� ������ �켱�Ѵ�.
					if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_CLOSE_RANGE)
					{
						m_siCurrentAttack = 0;

						// ���� �����Ÿ� �ۿ� ������,
//						if(FALSE == IsInRange(AttackID.ID, GetAttackRange()))
//							m_siCurrentAttack = 1;
					}
					// ���Ÿ� ���� �켱 �Ӽ��� ��� ���Ÿ� ������ �켱�Ѵ�.
					else if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_LONG_RANGE)
						m_siCurrentAttack = 1;
				}
				// ù ��° ���� �Ӽ��� �� ���Ÿ� ������ �Ѵ�.
				else
				{
					// �ٰŸ� ���� �켱 �Ӽ��� ��� �ٰŸ� ������ �켱�Ѵ�.
					if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_CLOSE_RANGE)
					{
						m_siCurrentAttack = 1;

						// ���� �����Ÿ� �ۿ� ������,
//						if(FALSE == IsInRange(AttackID.ID, GetAttackRange()))
//							m_siCurrentAttack = 0;
					}
					// ���Ÿ� ���� �켱 �Ӽ��� ��� ���Ÿ� ������ �켱�Ѵ�.
					else if (m_ssPriority_Attack_Type == PRIORITY_ATTACK_TYPE_LONG_RANGE)
						m_siCurrentAttack = 0;
				}
			}
	
			// ���� �����Ÿ� �ȿ� ������,
			if(IsInRange(AttackID.ID, GetAttackRange()))
			{ 
				
				// robypark 2004-5-24, 16:06 �ּҰŸ� ���� ������ �����Ѱ�
				// �ּ� �Ÿ����� ���� �Ұ����� ĳ������ ��쿡�� 
				if ((AttackAtbInfo[GetAttackAtb()] & ATTACKATB_ATB_MINRANGE)
						&& (IsInMinRange(AttackID.ID, KI[m_cltCharStatus.GetKind()]\
											.GetMinAttackRange(m_siCurrentAttack)) == TRUE))
//				if(IsAtb(ATB_MINRANGE)==TRUE && IsInMinRange(AttackID.ID, KI[m_cltCharStatus.GetKind()].GetMinAttackRange(CurrentAttack))==TRUE)
//				if ((AttackAtbInfo[GetAttackAtb()] & ATTACKATB_ATB_MINRANGE) \
//				&& (IsInMinRange(AttackID.ID, 1) == TRUE))
				{			
					// ���� �����ϰ� �̵� ���� �ϴٸ�, 
					if(CanAttack(AttackID))
					{
						
						// ��� ��尡 �����Ǿ� �ְ� ����� ���� ��ü�� ���̸�, 
						if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
						{
							// �̵����� �ʴ´�.
							m_scBusySwitch=FALSE;
							return FALSE;
						}
						// ���� �����ϱ� ���� �Ÿ��� ��췯 �̵��Ѵ�.
						else
						{
							// robypark 2004/10/18 12:24
							// ���Ÿ� �켱 ���� �Ӽ������� �����, �ٰŸ� �������� ��ȯ�ϵ��� �Ѵ�.
							// ������ �Ÿ��� ���Ÿ� ������ �Ұ����� ���
							if ((PRIORITY_ATTACK_TYPE_LONG_RANGE == m_ssPriority_Attack_Type) && (m_siAttackAtb2))
							{
								// �ٰŸ� �������� ��ȯ�ȴ�.
								if (m_siAttackRange1 < m_siAttackRange2)
									m_siCurrentAttack = 0;
								else
									m_siCurrentAttack = 1;

								// ���� �õ�
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
								// �̵����� �ʴ´�. 
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
			else // ���� ��Ÿ� �ۿ� ������, 
			{  
				
				// �Ǵٸ� ���� �Ӽ��� �ִٸ� 
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

				// ���ݰ����ϰ� �̵� ���� �ϴٸ�, 
				if(CanAttack(AttackID) )
				{
					WorkStep=WS_GOTO_TARGET;
					
					
					// ��� ��尡 �����Ǿ� �ְ� ����� ���� ��ü�� ���̸�, 
					if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
					{
						// �̵���Ű�� �ʴ´�. 
						m_scBusySwitch=FALSE;
						return FALSE;
					}
					// ���� �ִ� ������ �̵��Ѵ�. 
					else
					{
						// �����̸� ��󺸴� ���� ���� �Ѵ�. 
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
					
					// �ֺ��� �����Ѵ�. 
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
			  
			  // ��Ÿ� �ȿ� ������, 
			  if(max(abs(m_ssX-six), abs(m_ssY-siy)) < GetAttackRange())
			  {
				  if(TryAttackGround() == FALSE)
				  {
					  // ������ �ߴ��Ѵ�. 
					  return FALSE;
				  }
			  }
			  // ��Ÿ� �ۿ� ������, 
			  else
			  {
				  // ���ݰ����ϰ� �̵� ���� �ϴٸ�, 
				  if(CanAttackGround(six, siy))
				  {
					  WorkStep=WS_GOTO_TARGET;
					  
					  // ��� ��尡 �����Ǿ� �ְ� ����� ���� ��ü�� ���̸�, 
					  if(HoldSwitch==TRUE && HostMode==BY_COMPUTER)
					  {
						  // �̵���Ű�� �ʴ´�. 
						  m_scBusySwitch=FALSE;
						  return FALSE;
					  }
					  // ���� �ִ� ������ �̵��Ѵ�. 
					  else
					  {
						  // �����̸� ��󺸴� ���� ���� �Ѵ�. 
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
				// ���� ������ �� ���� ó�����, 
				if(CanAttack(AttackID)==FALSE)
				{  
					// �ֺ��� �����Ѵ�. 
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
				// ���� ������ �� ���� ó�����, 
				if(CanAttackGround(six, siy)==FALSE)
				{  
					return FALSE;
				}
				
				if(max(abs(m_ssX-six), abs(m_ssY-siy)) < GetAttackRange())
				{
					WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				}
			}
			
			// �������� �����ϰų� �߰� ������ �����ϸ�, 
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
				// ���� ������ �� ���� ó�����, 
				if(CanAttack(AttackID)==FALSE)
				{  
					// �ֺ��� �����Ѵ�. 
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
				// ���� ������ �� ���� ó�����, 
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
			
			// �������� �����ϰų� �߰� ������ �����ϸ�, 
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

    // ������ ������ ĳ�����̸�, 
    if(IsAtb(ATB_LOWATTACK))
	{
		if(Step == 0)
		{
	      // �����κ��� ������ �޴� ��Ȳ�̸�, 
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

	// ���� ���� 
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
			// �ź���, ������, �Ҷ��������� ���⸦ �������� ��� ����2 �ִϸ��̼��� ����Ѵ�.
			if ((m_cltCharStatus.GetKind() == KIND_ON_TURTLE_VEHICLE) || // ����-�ź���
				(m_cltCharStatus.GetKind() == KIND_ON_QUAKE_VEHICLE) || // �Ϻ�-������
				(m_cltCharStatus.GetKind() == KIND_ON_FARANGI_VEHICLE)) // �߱�-�Ҷ�������
			{
				// ���⸦ �����Ͽ���.
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
	// 2�� ��� ���� ���뿡 ���� ó�� �߰�
	// ĳ���� Ÿ�Ծ��
	SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(m_cltCharStatus.GetKind()));

	if(attackinstantstart <= m_cltCharDrawInfo.GetAniStep() && m_cltCharDrawInfo.GetAniStep() <= attackinstantend
	&& ((m_cltCharDrawInfo.GetAniStep()-attackinstantstart)%attackinstantdiff == 0))
	{
		// ���������� �Ӽ��� �ִٸ�, 
		// robypark 2004/7/12 11:1
		// 2�� ��� ���� ���뿡 ���� ó�� �߰�
		// 2�� ����� ���⿡ �ִ� ���������� ����� ����� �� ����.
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
					
					// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
					SetBattleFieldNEventMark();
					
					// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
					if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
						PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
				}
			}
		}
		// ��â�뵵 �Ӽ��� �ִٸ�(�ʻ��)
		// robypark 2004/7/12 11:1
		// 2�� ��� ���� ���뿡 ���� ó�� �߰�
		// 2�� ����� ���⿡ �ִ� ��â�뵵 ����� ����� �� ����.
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
			
		case ATTACKATB_SWORD:			FireOperation_Sword(attackinstantstart, hitrate);		break;	// �� ���� 
		case ATTACKATB_DUEOKSINI:		FireOperation_Dueoksini(attackinstantstart, hitrate);	break;	// ��� ���� 
		case ATTACKATB_TOAD:			FireOperation_Toad(attackinstantstart, hitrate);		break;	// �β��� ���� 
		case ATTACKATB_CANNONJ:			FireOperation_CannonJ(attackinstantstart, hitrate);		break;	// �ļ��������̸�, 
		case ATTACKATB_ELEPHANT:		FireOperation_Elephant(attackinstantstart, hitrate);	break;  // �ڳ��� 
		case ATTACKATB_BIGSWORD:		FireOperation_BigSword(attackinstantstart, hitrate);	break;  // ū �� ����  
		case ATTACKATB_MACHINEGUN:  
		case ATTACKATB_GUN:
		case ATTACKATB_STRONGGUN:		FireOperation_Gun(attackinstantstart, hitrate);			break;	// �Ѱ��� (�Ϲ���, ������ ��)
		case ATTACKATB_ELECTRON:		FireOperation_Electron(attackinstantstart, hitrate);	break;	// �������
		case ATTACKATB_GATPHAPOISON:	FireOperation_GatphaPoison(attackinstantstart, hitrate);break;  // ������ ��. 
		case ATTACKATB_MAGIC:			FireOperation_Magic2(attackinstantstart, hitrate);		break;	
		case ATTACKATB_MAGIC2:			FireOperation_Magic2(attackinstantstart, hitrate);		break;	// ���� �º� ���� 
		case ATTACKATB_MAGIC3:			FireOperation_Magic3(attackinstantstart, hitrate);		break;	// �Ϻ� ���� ����  
		case ATTACKATB_MAGICBALL:		FireOperation_MagicBall(attackinstantstart, hitrate);	break;	// �����渶������ 
		case ATTACKATB_JUSTICEK:		FireOperation_Justicek(attackinstantstart, hitrate);	break;	// ���� ��������
		case ATTACKATB_ARROW:
		case ATTACKATB_AIRATTACKUPARROW:
		case ATTACKATB_FIREARROW:		FireOperation_Arrow(attackinstantstart, hitrate);		break;	// �Ϲ� ȭ�� ���� || �������� �� ȭ�� ���� || ��ȭ�� ����. 
		case ATTACKATB_NEEDLE:			FireOperation_Needle(attackinstantstart, hitrate);		break;	// ħ
		case ATTACKATB_SNOWWOMAN:		FireOperation_Snowman(attackinstantstart, hitrate);		break;	// ������ ���� ����. 
		case ATTACKATB_FURHEADDEMON:	FireOperation_FurHeadDemon(attackinstantstart, hitrate);break;	// �а��� -  ���� 
		case ATTACKATB_DART:			FireOperation_Dart(attackinstantstart, hitrate);		break;	// ������ ������
		case ATTACKATB_AIRMISSILE:		FireOperation_AirMissile(attackinstantstart, hitrate);	break;  // // �̻��� 
		case ATTACKATB_SAMMISSILE:		FireOperation_SamMissile(attackinstantstart, hitrate);	break;	// õ�� �̻��� 
		case ATTACKATB_CANNON:			FireOperation_Cannon(attackinstantstart, hitrate);		break;	// ���� 
		case ATTACKATB_TOWERCANNON:		FireOperation_TowerCannon(attackinstantstart, hitrate);	break;	// ȭ�� ����. 
		case ATTACKATB_HEAVYGUN:		FireOperation_HeavyGun(attackinstantstart, hitrate);	break;	// ����� 
		case ATTACKATB_SHIPCANNON:		FireOperation_ShipCannon(attackinstantstart, hitrate);	break;	// ���� 
		case ATTACKATB_THROWBOMB:		FireOperation_ThrowBomb(attackinstantstart, hitrate);	break;	// ������ ��ź 
		case ATTACKATB_CANNON2C:		FireOperation_Cannon2C(attackinstantstart, hitrate);	break;	// �߼���  
		case ATTACKATB_BOMB:			FireOperation_Bomb(attackinstantstart, hitrate);		break;	// ��ź
		case ATTACKATB_FIRECAR:			FireOperation_FireCar(attackinstantstart, hitrate);		break;	// ȭ��. 
		case ATTACKATB_FIRE:			FireOperation_Fire(attackinstantstart, hitrate);		break;	// ȭ�� 
		case ATTACKATB_FIREUSINGMANA:	FireOperation_FireUsingMana(attackinstantstart, hitrate);break;	// ȭ�� (�������)
		case ATTACKATB_GOLDDRAGONELECTRON:FireOperation_GoldDragonElectron(attackinstantstart, hitrate);		break;	// ȭ�� 
		case ATTACKATB_ONGHWA:			FireOperation_Onghwa(attackinstantstart, hitrate);		break;	// ��õ ������ �ذ�. 
		case ATTACKATB_GHOSTCAT:		FireOperation_GhostCat(attackinstantstart, hitrate);	break;	// �͹� ���� ����. 
		case ATTACKATB_DOLL1:			FireOperation_Doll1(attackinstantstart, hitrate);		break;	// ���� ����. 
		case ATTACKATB_DOLL2:			FireOperation_Doll2(attackinstantstart, hitrate);		break;	// ��ī�� ����. 
		case ATTACKATB_KUKET:			FireOperation_Kuket(attackinstantstart, hitrate);		break;	// ���� ����.
		case ATTACKATB_CATDOLL:			FireOperation_CatDoll(attackinstantstart, hitrate);		break;	// ����� ����.
		case ATTACKATB_OLDFOXDOLL:		FireOperation_OldFoxDoll(attackinstantstart, hitrate);	break;	// ����ȣ ����.
		case ATTACKATB_RACCOONDOLL:		FireOperation_RaccoonDoll(attackinstantstart, hitrate);	break;	// �ʱ��� ����.
		case ATTACKATB_NINJADOLL:		FireOperation_NinjaDoll(attackinstantstart, hitrate);	break;	// �ʱ��� ����.
		case ATTACKATB_SNOWWOMANDOOL:	FireOperation_SnowWomanDoll(attackinstantstart, hitrate);	break;	// ���� ����.
		case ATTACKATB_YANGGUIBIDOLL:	FireOperation_YangGuiBiDoll(attackinstantstart, hitrate);	break;	// ��ͺ� ����.
		case ATTACKATB_JIJANGDOLL:		FireOperation_JijangDoll(attackinstantstart, hitrate);	break;	// ���庸�� ����.
		case ATTACKATB_SNAKEWOMANDOLL:	FireOperation_SnakeWomanDoll(attackinstantstart, hitrate);	break;	// ��� ����.
		case ATTACKATB_SNOW:			FireOperation_Snow(attackinstantstart, hitrate);		break;	// ��ī�� ����.
		case ATTACKATB_BUTTERFLY:		FireOperation_Butterfly(attackinstantstart, hitrate);	break;	// ������ 
		case ATTACKATB_WIDOW:			FireOperation_Widow(attackinstantstart, hitrate);		break;	// �Ź̿䱫 
		case ATTACKATB_WAVE:			FireOperation_Wave(attackinstantstart, hitrate);		break;	// �ٴٰź��� �ĵ�����. 
		case ATTACKATB_KIMBOMB:			FireOperation_KimBomb(attackinstantstart, hitrate);		break;	// ���ڰ� 
		case ATTACKATB_LEECH:			FireOperation_Leech(attackinstantstart, hitrate);		break;	// �ŸӸ� ü�� ���ѱ�. 
		case ATTACKATB_SEAHORSE:		FireOperation_SeaHorse(attackinstantstart, hitrate);	break;	// �ظ� ���ձ� . 
		case ATTACKATB_POISONARROW:		FireOperation_PoisonArrow(attackinstantstart, hitrate);	break;	// ��ħ 
		case ATTACKATB_THROWSPEAR:		FireOperation_ThrowSpear(attackinstantstart, hitrate);	break;	// ������ â. 
		case ATTACKATB_CACTUS:			FireOperation_Cactus(attackinstantstart, hitrate);		break;
		case ATTACKATB_SCORPION:		FireOperation_Scorpion(attackinstantstart, hitrate);	break;
		case ATTACKATB_CLAW:			FireOperation_Claw(attackinstantstart, hitrate);		break;
		case ATTACKATB_MAGICIAN:		FireOperation_Magician(attackinstantstart, hitrate);	break;	// �����. 
		case ATTACKATB_BIGTREE:			FireOperation_BigTree(attackinstantstart, hitrate);		break;	// �Ŵ�� Ư�� ����.
		case ATTACKATB_VOLCANOFLAME:	FireOperation_VolcanoFlame(attackinstantstart, hitrate);break;	// ȭ��Ҳ�
		case ATTACKATB_EVIL:			FireOperation_Evil(attackinstantstart, hitrate);		break;	// �̻��� ��� ��ǳ.
		case ATTACKATB_AUTA:			FireOperation_Auta(attackinstantstart, hitrate);		break;	// �ƿ�Ÿ ����.
		case ATTACKATB_ICE:				FireOperation_Ice(attackinstantstart, hitrate);			break;  // �̻��� ��� Ư�����.
		case ATTACKATB_MAGIC4T:			FireOperation_Magic4T(attackinstantstart, hitrate);		break;  // ¡Ű��ĭȥ Ư�����.
		case ATTACKATB_JAPANGHOST:		FireOperation_JapanGhost(attackinstantstart, hitrate);	break;	// �Ϻ��ͽ� ���� ����
		case ATTACKATB_SUB1:			FireOperation_Sub1(attackinstantstart, hitrate);		break;	// �������� ȭ����� 1
		case ATTACKATB_SUB2:			FireOperation_Sub2(attackinstantstart, hitrate);		break;	// �������� ȭ����� 2
		case ATTACKATB_PAPERDOLL:		FireOperation_PaperDoll(attackinstantstart, hitrate);	break;	// ����� ��������
		case ATTACKATB_NEEDLEATTACK:	FireOperation_NeedleAttack(attackinstantstart, hitrate);break;	// �Ǽ��� ħ����
		case ATTACKATB_CLOUDWIND:		FireOperation_CloudWind(attackinstantstart, hitrate);	break;	// ���»� ǳ���
		case ATTACKATB_SOULCHANGE:		FireOperation_Seimei(attackinstantstart, hitrate);		break;	// ���̸��� �⺻����
		case ATTACKATB_MAGICCATTACK:	FireOperation_MagicCAttack(attackinstantstart, hitrate);break;	// ������ �⺻����
		case ATTACKATB_SUNBIYOUN:		FireOperation_Sunbiyoun(attackinstantstart, hitrate);	break;	// ���� �⺻����	
		
		// actdoll (2004/03/06 5:02) : �����/����Ȳ�� �߰�
		case ATTACKATB_PYTHON:			FireOperation_Python		( attackinstantstart, hitrate );	break;	// �̹��� ���Ÿ� ��������	
		case ATTACKATB_JINEMPEROR:		FireOperation_JinEmperor	( attackinstantstart, hitrate );	break;	// ����Ȳ��ȥ ���Ÿ� ��������

		// linus : ��ȣ�� ����
//		case ATTACKATB_RABBIT:			FireOperation_Rabbit(attackinstantstart, hitrate);				break;	// ��ȣ��(�䳢) �⺻����		
		case ATTACKATB_GUARDIAN_SNAKE:	FireOperation_GuardianSnake ( attackinstantstart, hitrate );	break;	// ��ȣ��(��) �⺻����.

		// damduk : ��ȣ�� ����
		case ATTACKATB_GUARDIAN_PIG:	FireOperation_GuardianPig(attackinstantstart, hitrate);			break;	// ��ȣ��(����) �⺻����.

		// robypark (2004/5/19 17:31) : ��� 2�� ���� ���� �߰�
		// ����-�ź���, �̼��� 2�� ����
		case ATTACKATB_TURTLE_VEHICLE:
			FireOperation_TURTLE_VEHICLE(attackinstantstart, hitrate);
			break;

		// ����-������, ���� 2�� ����
		case ATTACKATB_THUNDER_VEHICLE:
			FireOperation_THUNDER_VEHICLE(attackinstantstart, hitrate);
			break;

		// �Ϻ�-������, ��Ű��ī 2�� ����
		case ATTACKATB_QUAKE_VEHICLE:
			FireOperation_QUAKE_VEHICLE(attackinstantstart, hitrate);
			break;

		// �Ϻ�-�����, ���̸��� 2�� ����
		case ATTACKATB_BLACK_DRAGON_VEHICLE:
			FireOperation_BLACK_DRAGON_VEHICLE(attackinstantstart, hitrate);
			break;

		// �߱�-�߼���, ����<=�̷� 2�� ����
		case ATTACKATB_CATAPULT:
			FireOperation_CATAPULT(attackinstantstart, hitrate);
			break;

		// �߱�-�Ҷ�������, �̷�<=���� 2�� ����
		case ATTACKATB_FARANGI_VEHICLE:
			FireOperation_FARANGI_VEHICLE(attackinstantstart, hitrate);
			break;

		// �븸-ȭ����, ����â 2�� ����
		case ATTACKATB_FIRE_DRAGON_VEHICLE:
			FireOperation_FIRE_DRAGON_VEHICLE(attackinstantstart, hitrate);
			break;

		// �븸-��Ȳ����, �弱ȭ 2�� ����
		case ATTACKATB_CHINESEPHOENIX:
			FireOperation_CHINESEPHOENIX(attackinstantstart, hitrate);
			break;

		// robypark (2004/6/4 13:6)
		// ��õ��� ����, �Ҷ�������, 126
		case ATTACKATB_ROAR_OF_LAUGHTER:
			FireOperation_ROAR_OF_LAUGHTER(attackinstantstart, hitrate);
			break;

		// robypark 2004/7/14 16:47
		// ���� ���� ����
		case ATTACKATB_DOLL_EVIS:
			FireOperation_DOLL_EVIS(attackinstantstart, hitrate);
			break;

		// robypark 2005/2/2 17:27
		// �޸� ���� ����
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

		// �Ǵٸ� ���� �Ӽ��� �ִٸ� 
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
	
	// �н��̸� Ư��ȿ���� �� �� ����.
	if(m_siTimerDelay)
		return FALSE;
	

	if((GD.Random()%2) == 0)
	{
		if(abs(GD.Frame - LastStrongAttackFrame ) < 100) return FALSE;
		
		sirand = GD.Random() % 15 ;
		
		// ����ź�� ����. 
		if(sirand == 1)
		{
/*			// ���� ��ǥ�� �װ�����, 
			if(CR[AttackID.ID].Area == AREA_AIR)
			{
				OrderAirMagic(m_NewID, BY_COMPUTER);
				return TRUE;
			}
			*/
		}
		// ������������ ���Ѵ�.(���� ��쿡��..)
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
		// ���ڸ� �ż��Ѵ�.
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
		// ��ȣ���� ģ��. 
		else if(sirand == 4 )
		{
			// ü���� �������� ���� ��ġ�Ǿ� ���� �ʾƾ� �Ѵ�. 
			if(GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER  
				&& ShieldDelay == 0
				&& CanShield(m_NewID) == TRUE)
			{
				OrderShield(m_NewID, m_NewID, BY_COMPUTER);
			}
		}

		// ���� ġ��
		if(sirand == 5 && IsAtb3(ATB3_LIGHTNING) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(CanLightning(AttackID)==TRUE)
			{
				return TRUE;
			}

			return FALSE;
		}

		// �������. (���÷��̾�� �Ѵ�.)
		if( (sirand >= 6 && sirand <= 10) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(CanSpecial(SPECIAL_MAGIC4C, AttackID) == TRUE)
			{
				OrderMagic4c(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;

		}

		// ǳ���
		if((sirand >= 11 && sirand <= 14) && IsAtb3(ATB3_TORNADO) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			
			// ���ݸ�ǥ�� ������ ����ų �� ������, 
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
 
		// ġ�� . 
		if(data == 0 && IsAtb2(ATB2_HEAL) && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]))
		{
			HealAI();
			return FALSE;
		}

		// ��Ȥ. 
		if(data == 1 && IsAtb(ATB_BETRAY))
		{
			if(CanBetray(AttackID, &returnval)==TRUE)
			{
				return TRUE;
			}
			return FALSE;

		}

		// ������ ����ġ ���ѱ�. 
		if(data == 2 && IsAtb2(ATB2_STEALEXP))
		{
			if(CanStealExp()==TRUE)
			{
				return TRUE;
			}
			return FALSE;
		}

		// ���� ü�� ���ѱ� 
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

		// ��Ȥ��
		if(data == 5 && IsAtb2(ATB2_BETRAY))
		{
			SI32 returnval = 0;

			// ���ݸ�ǥ�� ��Ȥ�� �� �ִ� ���̶��, 
			if(CanBetray(AttackID, &returnval)==TRUE)
			{
				OrderBetray(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}

		// �нż�
		// �н��� �нż��� ������ �� ����. (HostID.IsEmpty() == TRUE : �н��� �ƴϴ�.)
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
		//������� �Ʊ�,�ı� ��ȯ.
		//�Ƹ���翡 ���� ��ȯ�� ������ �Ʊ�,�ı͸� ��ȯ�Ҽ� �ִ�.
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
		
		//��ȯ��
		if( (data >= 10 && data <= 12) && ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) 
		&& HostID.IsEmpty() == TRUE)
		{
			//���� ��ȯ
			if(CanReCallDamduk())
			{
				OrderReCallDamduk(m_NewID, BY_COMPUTER);	
				return TRUE;
			}
			//�Ҵ� ��ȯ
			else if(IsAtb3(ATB3_MAGIC_RECALL_PANDA))	
			{
				if(CanReCall()==TRUE)
				{
					OrderReCallPanda(m_NewID, BY_COMPUTER);	
					return TRUE;
				}
				return FALSE;
			}
			//Ȳ�� ��ȯ.
			else if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY))
			{
				if(CanReCall()==TRUE)
				{
					OrderReCallMonkey(m_NewID, BY_COMPUTER);	
					return TRUE;
				}
				return FALSE;					
			}
			//�Ƹ������ ����� õ�� ��ȯ.
			else if(IsAtb3(ATB3_MAGIC_RECALL_AMAKUSA))
			{
				if(CanReCall()==TRUE)
				{
					OrderReCallAmakusa(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}			
			//��ȣ�� ����ȣ���� ��ȯ
			else if(IsAtb3(ATB3_MAGIC_RECALL_TIGER))
			{
				if(CanReCall()== TRUE)
				{
					OrderReCallTiger(m_NewID,	BY_COMPUTER);
				}	
				return FALSE;
			}
		}



		//Ȳ��.
		if( data == 13 && IsAtb3(ATB3_YELLOWSAND) && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE) )
		{
			if(CanSpecial(SPECIAL_YELLOWSAND,  AttackID) == TRUE)
			{
				OrderYellowSand(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}

		
		// ������
		if(data == 14)
		{
			if( data == 14 && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE) )
			{

				// �ڵ� ������ �����Ǿ� ������ �ʰ� ������̸�, 
				if(GD.Player[GetPlayerNumber()].AutoMagicSwitch==FALSE
				&& ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()])==FALSE)return FALSE;

				SHORT length;		

				// ���ݸ�ǥ�� ������ ����ų �� ������, 
				if(CanEarthQuake(AttackID)==TRUE)
				{
					// �Ÿ��� �ʹ� ������ ������ ����Ű�� �ʴ´�.
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

		// ���� �̵���
		if( data == 15 && IsAtb2(ATB2_WARP) )
		{
			// �ڵ� ������ �����Ǿ� ������ �ʰ� ������̸�, 
			if(GD.Player[GetPlayerNumber()].AutoMagicSwitch==FALSE
			&& ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()])==FALSE)return FALSE;
			
			if(GD.Player[GetPlayerNumber()].PI.Host==BY_COMPUTER)
			{
		
				// ü���� ���� ������ ����� ���濡, 
				if(CanWarp() == TRUE
				&& m_ParmeterBox.IP.GetLife() < m_ParmeterBox.GetMaxLife())
				{
					OrderWarp(m_NewID, BY_COMPUTER);
					return TRUE;
				}
				return FALSE;
			}
		}
		// ��Ȥ��
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
		// ������
		if( data == 17 &&  (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(IsAtb3(ATB3_GROUNDEXP))
			{
				if(CanSpecial(SPECIAL_GROUNDEXP, ZeroID) == TRUE)	
				{
				// actdoll (2004/03/10 4:55) : 
				//	�� �κ��� ������ �Լ����� ���Դ�. ���� ��ɿ� ���� �κа� ���� ��ɿ� ���� �κ��� ó�� �ߺ����� ���Ͽ�
				//	���������� ó���� ���� ������ ����� �������� �پ��ϰ� �߻���� �ʰ� �ϳ��� �߻��Ǵ� ���װ� �����.
				//	���� �켱 ���Ѵ�� ���� ����� ���� ������ ������� �켱������ ����ϰ� ���� ���� �� ������ �����ߴ�.
				//	���� �� �κ��� ���� �����ؾ� �� ������ �����ȴ�.
					OrderGroundExp(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
					return TRUE;
				}
			}

			return FALSE;
		}
		//���ڰ�
		if( data == 18 &&  (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(CanSpecial(SPECIAL_KIMBOMB,  AttackID) == TRUE)
			{
				OrderKimBomb(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
			return FALSE;
		}
		// ����ȭ����ǳ
		if( data == 19 &&  (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(IsAtb3(ATB3_DARKFIRE))
			{
				if(CanSpecial(SPECIAL_DARKFIRE, ZeroID) == TRUE)	return TRUE;
			}

			return FALSE;
		}
		// ���ڼ�
		if( data == 20 && (ISMONSTER(MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if(IsSee(AttackID) && CanSpecial(SPECIAL_MAGIC6C, ZeroID) == TRUE 
			&& CR[AttackID.ID].IsAtb(ATB_FLY) == FALSE
			&& CR[AttackID.ID].m_siHaltDelay == 0)
			{
				LastStrongAttackFrame = GD.Frame;
				// actdoll (2004/03/10 4:55) : 
				//	�� �κ��� ������ �Լ����� ���Դ�. ���� ��ɿ� ���� �κа� ���� ��ɿ� ���� �κ��� ó�� �ߺ����� ���Ͽ�
				//	���������� ó���� ���� ������ ����� �������� �پ��ϰ� �߻���� �ʰ� �ϳ��� �߻��Ǵ� ���װ� �����.
				//	���� �켱 ���Ѵ�� ���� ����� ���� ������ ������� �켱������ ����ϰ� ���� ���� �� ������ �����ߴ�.
				//	���� �� �κ��� ���� �����ؾ� �� ������ �����ȴ�.
				if( IsAtb3(ATB3_MAGIC6C) == TRUE )	
				{
					OrderMagic6c(m_NewID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), AttackID, BY_COMPUTER);
					return TRUE;
				}
			}
			return FALSE;
		}
		// ������
		if( data == 21 && (ISMONSTER( MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if( CanSpecialAtb4( SPECIAL_ICE_BARRIER, AttackID ) )
			{
				OrderIceBarrier(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
		}
		// ��ȸ��
		if( ( data >= 22 && data <= 23 ) && (ISMONSTER( MyNet.PlayersdpId.dpId[GetPlayerNumber()]) || m_bGuardianChar == TRUE))
		{
			if( CanSpecialAtb4( SPECIAL_STONE_MAGIC, AttackID ) )
			{
				OrderStoneMagic(m_NewID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, AttackID, BY_COMPUTER);
				return TRUE;
			}
		}
		// ������ 
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



// ������ ���� ������ ��ġ�� �˷��ش�.
void _Char::DecideAttackPoint(int port, int& x, int&y)
{
	x=m_cltCharStatus.GetCenterX();
	y=m_cltCharStatus.GetCenterY();
}


// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
void _Char::SetBattleFieldNEventMark()
{

   // ���� �޴� ������ ��ġ�� ���� ��ġ ���� 
   if(CR[AttackID.ID].GetPlayerNumber() == UserPlayerNumber)
   {

	 if(clifMap.IsInScreen(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY) == FALSE)
	 {
	    GD.SetBattleField(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
        SMap.SetEventMark(CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
	 }

   }
   // �����ϴ������� �Ʊ��̸� ���� ��ġ�� ���� 
   else if(GetPlayerNumber() == UserPlayerNumber)
   {

	 if(clifMap.IsInScreen(m_ssX, m_ssY) == FALSE)
	 {
	    GD.SetBattleField(m_ssX, m_ssY);
	    SMap.SetEventMark(m_ssX, m_ssY);
	 }

   }
}


// ���ݿ� ���� ������ �����Ѵ�. �ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
void _Char::SetFireInfo(_NewID id)
{
	FireID     = id;
	FireSwitch = TRUE;
	FireDelay  = MAX_FIRE_DELAY;
}

void _Char::FireOperation_Sword(SI32 attackinstant, SI32 hitrate)
{
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
			// ���볶���̸�, 
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
	
	// ������ ���� �̺�Ʈ ��ũ�� ���� ���� ���� ���� ����Ѵ�. 			 
	SetBattleFieldNEventMark();

	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_SWORD, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
}

void _Char::FireOperation_GuardianSnake(SI32 attackinstant, SI32 hitrate)
{
	SetFireInfo(AttackID);

	// ������ ���� �̺�Ʈ ��ũ�� ���� ���� ���� ���� ����Ѵ�. 			 
	SetBattleFieldNEventMark();

	SI16 siMana = CR[AttackID.ID].m_ParmeterBox.IP.GetMana();

	if(siMana > 300)
	{
		CR[AttackID.ID].ReduceMana(300);  //���� ����.
	}
	else
	{
		CR[AttackID.ID].ReduceMana(siMana);
	}

	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CLAW, 0, AttackArea, 0, AttackID, m_ssAttackX, m_ssAttackY, 100);
}

void _Char::FireOperation_Claw(SI32 attackinstant, SI32 hitrate)
{
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	
	// ������ ���� �̺�Ʈ ��ũ�� ���� ���� ���� ���� ����Ѵ�. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CLAW, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Dueoksini(SI32 attackinstant, SI32 hitrate)
{
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	
	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_DUEOKSINI, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Toad(SI32 attackinstant, SI32 hitrate)
{
	
	if(IsAlive(AttackID) == TRUE)
	{
		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_TOADPOISON)
		{
			// ġ����� �ٿ��ش�. 
			ReduceMana(MAGICPOWER_TOADPOISON);
			
			CR[AttackID.ID].m_cltPoisonEffect.Set(TRUE, 500, m_ParmeterBox.CalcApower() / 2);
		}
	}
	
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
	}
	
	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_TOAD, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_CannonJ(SI32 attackinstant, SI32 hitrate)
{
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		{
			PushEffect(EFFECT_CANNONJ, m_ssX, m_ssY);
		}
		
		// ��ȿ�� ���� ���� �ȿ� �־�� �Ѵ�.
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
		
		// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
		SetBattleFieldNEventMark();
		
		CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_CANNONJ, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}

void _Char::FireOperation_Elephant(SI32 attackinstant, SI32 hitrate)
{
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	
	
	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_QUEENSUB1, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_QUEENSUB2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SPEAR2, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_BigSword(SI32 attackinstant, SI32 hitrate)
{
	
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	
	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
	SetBattleFieldNEventMark();
	
	
	/*			// �� ��ð��� �ִ� ��� ���ȯ ����. (������ ���� ����� �Ѵ�. )
	// �������� ���� �̻��� �Ǿ�� �Ѵ�. 
	else if(HaveItem(GOODS_SWORD7) && IsAlive(Slave2ID) == FALSE
	&& m_ParmeterBox.GetMana() >= MAGICPOWER_DRAGON)
	{
	SI16 emptyx, emptyy;
	if(FindEmptyAreaAir(emptyx, emptyy, 1, 1)==TRUE)
	{
	
	  id = GD.Player[GetPlayerNumber()].FindPlayerEmptyID(KIND_ATTACKERC, FALSE);
	  if(id)
	  {
	  // ĳ���͸� �����Ѵ�. 
	  SetChar(KIND_ATTACKERC, id, emptyx, emptyy, Direction, 100, GetPlayerNumber());
	  //CR[id].TransparentSwitch = TRUE;
	  CR[id].m_siTimerDelay = 10;
	  CR[id].HostID = m_NewID;
	  
		Slave2ID = CR[id].m_NewID;
		
		  // �������� ���δ�. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		SI32 apower = m_ParmeterBox.CalcApower();
		
		// ���� ������ �ʻ��� �۵��Ѵ�.
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

			// ���� ����� ���⸦ �߻���Ų��. 
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
				// �Ѿ� �߻� ���� 
				smokeid1=FindEmptySmoke();
				if(smokeid1)
				{
					SetSmoke(KIND_SMOKE_GUNSMOKE, smokeid1, attackstartx, attackstarty, AttackArea);
				}
			}

			// �ͽ�ź
			if(IsAtb3(ATB3_GHOST) == TRUE)
			{
				SetBullet(KIND_BULLET_GHOST, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			}
			// ö��ź 
			else if(IsAttackAtb(ATTACKATB_STRONGGUN)==TRUE)
			{
				SetBullet(KIND_BULLET_STRONGGUN, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			}
			else
			{
				SetBullet(KIND_BULLET_GUN, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			}
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
		SetBattleFieldNEventMark();
	}
}



void _Char::FireOperation_Electron(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
		
		if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
		{
			// ���ݿ� ���� ������ �����Ѵ�. 
			//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
			SetFireInfo(AttackID);
			
			id=FindEmptyBullet();
			if(id)
			{
				SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
				SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
				
				// �������� �����. 
				ElectronSwitch = TRUE;
				ElectronInfo1  = m_cltCharStatus.GetCenterX();
				ElectronInfo2  = m_cltCharStatus.GetCenterY();
				ElectronInfo3  = x;
				ElectronInfo4  = y;
				
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_GENERAL4KATTACK, m_ssX, m_ssY);
				
				
				if(m_cltCharDrawInfo.GetAniStep() == attackinstant)
				{
					
					
					// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
					SetBattleFieldNEventMark();
					
					CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_ELECTRON, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
					
					// ������ ����� �����ְ� �Ѵ�. 
					CR[AttackID.ID].m_cltElectronShock.SetElectronDraw(TRUE);
					
					// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		// �ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_GATPHAPOISON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_BUDAFIRE2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_MAGICIANBULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_BUDAFIRE3, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		
			// �������� ����� ���� ���� �����Ű�� �ɷ��� ������ �� �ִ�. 
			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_JUSTICEK)
			{
				 // �������� �ٿ��ش�. 
				ReduceMana(MAGICPOWER_JUSTICEK);

				SetBullet(KIND_BULLET_JUSTICEK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			else
			{
				SetBullet(KIND_BULLET_BUDAFIRE2, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		// ���� ������ �ʻ��� �۵��Ѵ�.
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
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
			else if(IsAtb3(ATB3_FIREARROW2) && ReduceMana(MAGICPOWER_FIREARROW2) == TRUE)
			{
				apower = CalcDamage(SPECIAL_FIREARROW2, apower, m_scMagicApplyRate);
				SetBullet(KIND_BULLET_BURNARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
			else
			{
				SetBullet(KIND_BULLET_ARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
		}
	}
	
}

void _Char::FireOperation_Needle(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_NEEDLE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetSnowMagic(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), hitrate, AttackID);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
	
}


void _Char::FireOperation_FurHeadDemon(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_FURHEADDEMON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			if( IsAtb2(ATB2_SAMC) )
			{
				if(FireRepeatNumber==1)
				{
					// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
					if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
						PushEffect(EFFECT_SAMC, m_ssX, m_ssY);
				}
			}
			else
			{
				
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_CANNON, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_CANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_CANNON, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			{
				SetBullet(KIND_BULLET_CANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			}
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_SMALLCANNON, m_ssX, m_ssY);
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()% max(1, (length/2));
			realdy=y-(length/4)+GD.Random()% max(1, (length/2));
			
			
			SetBullet(KIND_BULLET_HEAVYGUN, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(FireRepeatNumber==1)
			{
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_SHIPCANNONDOUBLE, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_SHIPCANNON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(FireRepeatNumber==1)
			{
				
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_SHIPCANNONDOUBLE, m_ssX, m_ssY);
			}
			
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			
			SetBullet(KIND_BULLET_THROWBOMB, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
	
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	id=FindEmptyBullet();
	if(id)
	{
		if(FireRepeatNumber==0)
		{
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				// ���屺���� ȿ����. 
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
		
		// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
		SetBattleFieldNEventMark();
		
	}
	
}

void _Char::FireOperation_Bomb(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// �ֺ��� ���̰� �����.
		if(FireRepeatNumber==0)
		{
			// �ֺ��� ���̰� �����.
		}
		
		FireRepeatNumber++;
		if(FireRepeatNumber>=MaxFireRepeatNumber)
		{
			FireRepeatNumber=0;
			FireRepeatDelayor=0;
		}
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(FireRepeatNumber==1)
			{
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
				if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
					PushEffect(EFFECT_BOMB, m_ssX, m_ssY);
			}
			
			SHORT startx=m_cltCharStatus.GetCenterX()+10-GD.Random()%20;
			
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/12)+GD.Random()%max(1, (length/6));
			realdy=y-(length/12)+GD.Random()%max(1, (length/6));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_FIRETANKARROW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, 
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, TargetDirection, AttackArea);
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_FIRE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);

		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_FIRE)
		{
			// �������� �ٿ��ش�. 
			ReduceMana(MAGICPOWER_FIRE);
			
			id=FindEmptyBullet();
			if(id)
			{
				SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
				SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
				
				length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
				
				realdx=x-(length/4)+GD.Random()%max(1, (length/2));
				realdy=y-(length/4)+GD.Random()%max(1, (length/2));
				
				// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
				SetBattleFieldNEventMark();
				
				int attackstartx, attackstarty;
				DecideAttackPoint(0, attackstartx, attackstarty);
				
				SetBullet(KIND_BULLET_FIRE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
				
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_GOLDDRAGONELECTRON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_ONGHWA, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_GHOSTCAT, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			apower = m_ParmeterBox.CalcApower() ;
			
			SetBullet(KIND_BULLET_BUTTERFLY, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{

			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_WIDOW)
			{
				// �������� �ٿ��ش�. 
				ReduceMana(MAGICPOWER_WIDOW);
				
				SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
				SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
				
				apower = m_ParmeterBox.CalcApower() ;
				
				SetBullet(KIND_BULLET_WIDOW, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);
				
				// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
				SetBattleFieldNEventMark();
				
				// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
			// ġ����� �ٿ��ش�. 
			ReduceMana(MAGICPOWER_WAVE);
		}
		else
		{
			return ;
		}

		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		{
			PushEffect(EFFECT_TUTTLEWAVE, m_ssX, m_ssY);
		}
		
		// ��ȿ�� ���� ���� �ȿ� �־�� �Ѵ�.
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
		
		// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
	
	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		// ������ ���ؼ� ������ ������ �ʵ��� ZeroID�� �ִ´�. 
		SetMagicIce(m_ssX, m_ssY, ZeroID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), m_ParmeterBox.GetHitRate(0), ZeroID);
	}
	else
	{
		clGrp.Error("cannonj", "139663");
	}
	
	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
	SetBattleFieldNEventMark();

}

void _Char::FireOperation_KimBomb(SI32 attackinstant, SI32 hitrate)
{

		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_KIMBOMB)
		{
			// ġ����� �ٿ��ش�. 
			ReduceMana(MAGICPOWER_KIMBOMB);
		}
		else
		{
			return ;
		}

		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
		{
			//PushEffect(EFFECT_CANNONJ, X, Y);
		}
		
		// ��ȿ�� ���� ���� �ȿ� �־�� �Ѵ�.
		if(Map.IsInMapArea(m_ssAttackX, m_ssAttackY)==TRUE)
		{
			// ������ ���ؼ� ������ ������ �ʵ��� ZeroID�� �ִ´�. 
			SetKimBomb(m_ssX,m_ssY,ZeroID,GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), m_ParmeterBox.GetHitRate(0), ZeroID);
		}
		
		// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
		SetBattleFieldNEventMark();

}

void _Char::FireOperation_Leech(SI32 attackinstant, SI32 hitrate)
{
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
	if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
	{
		PushEffect(EFFECT_SWORD, m_ssX, m_ssY);
	}
	
	// ������ ���� �̺�Ʈ ��ũ�� ���� ���� ���� ���� ����Ѵ�. 			 
	SetBattleFieldNEventMark();
	
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_LEECH, 0, AttackArea, m_ParmeterBox.CalcApower(), AttackID, m_ssAttackX, m_ssAttackY, hitrate);
	
}


void _Char::FireOperation_SeaHorse(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_SEAHORSE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower();
		
		// ���� ������ �ʻ��� �۵��Ѵ�.
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

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_THROWSPEAR, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_CACTUSBULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		
		
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBullet(KIND_BULLET_SCORPIONBULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			{
				PushEffect(EFFECT_ARROW, m_ssX, m_ssY);
			}
		}
	}
	
}

//�̻��� ��� ����.
void _Char::FireOperation_Evil(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		
			SetBullet(KIND_BULLET_EVIL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);

			
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_VOLCANOFLAME, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_BIGTREE)
			{
				// ġ����� �ٿ��ش�. 
				ReduceMana(MAGICPOWER_BIGTREE);
			}
			else
			{
				return ;
			}

			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			SetBigTree(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower()/4, hitrate, AttackID);
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_AUTA, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);

		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			length=max(abs(m_cltCharStatus.GetCenterX()-x), abs(m_cltCharStatus.GetCenterY()-y));
			
			realdx=x-(length/4)+GD.Random()%max(1, (length/2));
			realdy=y-(length/4)+GD.Random()%max(1, (length/2));
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			int attackstartx, attackstarty;
			DecideAttackPoint(0, attackstartx, attackstarty);
			
			SetBullet(KIND_BULLET_JAPANGHOST, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, attackstartx, attackstarty, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);				 
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_PAPERDOLL, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_NEEDLEATTACK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

void _Char::FireOperation_CloudWind(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	SI32 apower = m_ParmeterBox.CalcApower();
	// �����ϰ� ǳ����� ���ֵȴ�.
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_CLOUDWIND, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, apower, hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// robypark 2004/9/13 13:30
			// ���̸��� ������ �����ǵ��� ���Ÿ� ������ �ǵ��� ����
			SetBullet(KIND_BULLET_SOULCHANGE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 20, x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_MAGICCATTACK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_RABBIT, id, X, Y, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			SetBullet(KIND_BULLET_SUNBIYOUNATTACK, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_BUDA, m_ssX, m_ssY);
		}
	}
}

// �̹��� ���Ÿ� ��������
void _Char::FireOperation_Python(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		
			SetBullet(KIND_BULLET_PYTHON, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FIRE_JUSTICEK, m_ssX, m_ssY);
		}
	}

}

// ����Ȳ��ȥ ���Ÿ� ��������
void _Char::FireOperation_JinEmperor(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id=FindEmptyBullet();
		if(id)
		{
			SHORT x=CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT y=CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		
			SetBullet(KIND_BULLET_JINEMPEROR, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), x, y, m_NewID, AttackID, m_ParmeterBox.CalcApower(), hitrate, 0, AttackArea);
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_FIRE_JUSTICEK, m_ssX, m_ssY);
		}
	}

}

void _Char::FireOperation_GuardianPig(SI32 attackinstant, SI32 hitrate)
{
	// ���ݿ� ���� ������ �����Ѵ�. 
	//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
	SetFireInfo(AttackID);
	
	// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	
	// ������ ���� �̺�Ʈ ��ũ�� ���� ���� ���� ���� ����Ѵ�. 		
	SetBattleFieldNEventMark();

	// ��Ű�� �߿��� �ٽ� ������ �� �� ����.
	// ��Ű�� �߿��� �ٽ� ������ �� �� ����.
	if(!m_bPigAttack)
	{
		m_dwPigAttackDelay = timeGetTime();
		m_bPigAttack = TRUE;
		m_cltCharDrawInfo.SetAnimation(ANI_WAIT0);
		OrderStop(m_NewID, BY_COMPUTER);
		
		CR[AttackID.ID].m_dwPigAttackDelay = timeGetTime();
		CR[AttackID.ID].m_bPigDefence = TRUE;				// Action �Ҵ����� �����.
		CR[AttackID.ID].m_cltCharDrawInfo.SetDrawSwitch(FALSE);	// Draw   �Ҵ����� �����.
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// ����-�ź���, �̼��� 2�� ����
void _Char::FireOperation_TURTLE_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_FIRE_KU102AE1, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// ����-������, ���� 2�� ����
void _Char::FireOperation_THUNDER_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_FIRE_KU103AE2, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// �Ϻ�-������, ��Ű��ī 2�� ����
void _Char::FireOperation_QUAKE_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SHORT smokeid;

	// ������ Ÿ���� �ش�. 
	SHORT level = m_ParmeterBox.IP.GetLevel();

	SI32 apower = CalcDamage(SPECIAL_EARTHQUAKE, m_ParmeterBox.CalcApower(), \
						m_scMagicApplyRate, KI[m_cltCharStatus.GetKind()].m_siKindAtb); 

//	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_EARTHQUAKE, 2, AREA_LAND, 
	// robypark 2004/6/11 10:32
	// �����Ӽ� �������� �ֵ��� ����
	CalculateHit(m_NewID, GetPlayerNumber(), ATTACKATB_QUAKE_VEHICLE, 2, AREA_LAND, 
		 apower, AttackID, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY, 
		 hitrate);
//		 m_ParmeterBox.GetHitRate(0));

	// ���� ���̺� ����Ʈ�� �����Ѵ�.
	smokeid = FindEmptySmoke();
	if(smokeid)
	{
		// ������ ����ȿ����
		PushEffect(EFFECT_FIRE_JU104AE1, CR[m_NewID.ID].m_ssX, CR[m_NewID.ID].m_ssY);

		// ȭ�� ��鸲�� �����. 
	    GD.EarthQuake.SetEarthQuake();

		// �ٴڿ� Smoke���� �����࿩ ������ ������ �μ��� TRUE�� �Ѱ��ش�.
		SetSmoke(KIND_SMOKE_EARTHQUAKE_WAVE, smokeid, CR[AttackID.ID].m_cltCharStatus.GetCenterX(), CR[AttackID.ID].m_cltCharStatus.GetCenterY(), AREA_LAND, -1, TRUE);

		// ���︲ ȿ����
		PushEffect(EFFECT_EXP_JU104AE2, CR[AttackID.ID].m_ssX, CR[AttackID.ID].m_ssY);
	}

     OrderStop(m_NewID, BY_COMPUTER);
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// �Ϻ�-�����, ���̸��� 2�� ����
void _Char::FireOperation_BLACK_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_FIRE_JU105AE1, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// �߱�-�߼���, ����<=�̷� 2�� ����
void _Char::FireOperation_CATAPULT(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_FIRE_CU106AE1, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// �߱�-�Ҷ�������, �̷�<=���� 2�� ����
void _Char::FireOperation_FARANGI_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
		SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();

		if (id)
		{
			// ���� ���� �߻�(�������� �ƴ϶� �������� ������)
			SetBullet(KIND_BULLET_FARANGI_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				TargetX, TargetY, m_NewID, AttackID, apower * 0.34, hitrate, \
				0, AttackArea);

			// �Ѿ� �߻翡 ������ �α� ���� �۾�
			Bullet_SetLuanchDelay(id, 1);
			Bullet_DrawSwitch(id, FALSE);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
//			PushEffect(EFFECT_CANNON, X, Y);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}

		id = FindEmptyBullet();
		
		if (id)
		{
			// ���� ���� �߻�(�������� �ƴ϶� �������� ������)
			SetBullet(KIND_BULLET_FARANGI_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX() + 15, m_cltCharStatus.GetCenterY(), \
				TargetX + 15, TargetY, m_NewID, AttackID, apower * 0.33, hitrate, \
				0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
//			PushEffect(EFFECT_CANNON, X, Y);
			PushEffect(EFFECT_FIRE_CU107AE1, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	
		id = FindEmptyBullet();

		if (id)
		{
			// ���� ���� �߻�(�������� �ƴ϶� �������� ������)
			SetBullet(KIND_BULLET_FARANGI_BULLET, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX() - 15, m_cltCharStatus.GetCenterY(), \
				TargetX - 15, TargetY, m_NewID, AttackID, apower * 0.33, hitrate, \
				0, AttackArea);

			// �Ѿ� �߻翡 ������ �α� ���� �۾�
			Bullet_SetLuanchDelay(id, 2);
			Bullet_DrawSwitch(id, FALSE);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
//			PushEffect(EFFECT_CANNON, X, Y);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// �븸-ȭ����, ����â 2�� ����
void _Char::FireOperation_FIRE_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK2);

	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = m_ParmeterBox.CalcApower(); 

		if (id)
		{
			SHORT TargetX = CR[AttackID.ID].m_cltCharStatus.GetCenterX();
			SHORT TargetY = CR[AttackID.ID].m_cltCharStatus.GetCenterY();
			
			// ȭ����� �߻�
			SetBullet(KIND_BULLET_FIRE_DRAGON_MISSILE, id, m_ssX, m_ssY, m_ssAttackX, m_ssAttackY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 40, \
				TargetX, TargetY, m_NewID, AttackID, apower, hitrate, \
				0, AttackArea);

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_FIRE_TU108AE2, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
// �븸-��Ȳ����, �弱ȭ 2�� ����
void _Char::FireOperation_CHINESEPHOENIX(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{
		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_FIRE_TU109AE1, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

// robypark (2004/6/4 13:6)
// ��õ��� ����, �Ҷ�������, 126
// ATTACKATB_ROAR_OF_LAUGHTER
void _Char::FireOperation_ROAR_OF_LAUGHTER(SI32 attackinstant, SI32 hitrate)
{
	SI32 id;

	// robypark 2004/11/11 11:20
	// ��õ��� ������ �� �����Ҹ��ϵ��� ����
	if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_SIEGE_MODE_FIRE_MISSILE)
	{
		ReduceMana(MAGICPOWER_SIEGE_MODE_FIRE_MISSILE);
	}
	else 
	{
//		m_scBusySwitch=FALSE;

		// ������ ���� ��� ��õ���=>�Ҷ��������� �ڵ� ����
//		OrderSIEGE_MODE_UNDO(m_NewID, m_ssX, m_ssY, ZeroID, BY_COMPUTER);

		return;
	}
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange()) == TRUE)
	{		
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
		SetFireInfo(AttackID);
		
		id = FindEmptyBullet();
		
		SI32 apower = CalcDamage(SPECIAL_SIEGE_MODE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate);

		// robypark 2004/11/2 11:28
		// ��õ��� ���߷� + Alpha(50) �߰�
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

			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			PushEffect(EFFECT_SAMC, m_ssX, m_ssY);

			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
		}
	}
}

// robypark 2004/7/14 16:42
// ���� ���� ����
// ATTACKATB_DOLL_EVIS
void _Char::FireOperation_DOLL_EVIS(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}

// robypark 2005/2/2 17:27
// �޸� ���� ����
// ATTACKATB_DOLL_DHARMA
void _Char::FireOperation_DOLL_DHARMA(SI32 attackinstant, SI32 hitrate)
{
	SI32 id; 
	SI32 apower = 0;
	
	if(IsSee(AttackID.ID) && IsInRange(AttackID.ID, GetAttackRange())==TRUE)
	{	
		// ���ݿ� ���� ������ �����Ѵ�. 
		//�ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
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
			
			// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
			SetBattleFieldNEventMark();
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_NINJA2JATTACK, m_ssX, m_ssY);
		}
	}
}
