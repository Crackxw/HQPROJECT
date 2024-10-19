//--------------------------------------------------------------------
//  ��� : ���°�
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
// ������ �� ��ǥ������ �߽ɰ�, 
extern	int				MapCenterX[][MAX_MAP_YSIZE];
extern	int				MapCenterY[][MAX_MAP_YSIZE];
extern	cltMapInterface clifMap;          // �ʰ��� �������̽� 
extern _KindInfo KI[];

// ĳ���͸� �̵��ϰ� �����. 
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

   // �ڸ��� �̵��ϸ� �ڸ� ����� �����Ѵ�. 
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

// �̵� ��ƾ 
// ���ϰ�: 1 -> ������ ���� 
int cltCharOrg::MoveNormal(SHORT moveblockcount, SHORT range)
{
	int x=m_ssX, y=m_ssY;
	int direction;
	int rx=RX, ry=RY;
	SI32 aniindex = 0;
	SHORT moveswitch=FALSE;
	

	// �̵��� ��ġ�� �Ϸ�Ǹ� �̵�, 
	// �ƴϸ� ��ġ 
	if(IsPreMoveOk()==FALSE)
	{
		return MOVEINFO_MOVING;
	}
	
	// �̵��� �ɷ��� ������ �����, 
	if(MoveBlockDelay>0)
	{
		// �浹�� �ؼ� �Ǿ��ٸ�, 
		if(MoveBlockX>=0 && IsColl(MoveBlockX, MoveBlockY, Block|BLOCK_MOVE)==FALSE && Map.IsColl(MoveBlockX, MoveBlockY, BLOCK_ICEWALL)==FALSE)
		{
			MoveBlockX=-1;
			MoveBlockY=-1;
			
			MoveBlockDelay=0;
		}
		// ��� �浹�� ���·� �ִٸ�, 
		else
		{
			MoveBlockDelay--;
			
			// ��ٸ���� ��ٸ� �Ŀ��� �ٸ� ��� ���� ����� ����Ѵ�. 
			if(MoveBlockDelay==0 && MovePercent==0)
			{
				FindAnotherPathSwitch=TRUE;
			}
			
			// �ִϸ��̼��� �Ϸ�� ���� �ƴ϶��, 
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
	
	
	// �߰������� ������ ���̶�� 
	if(MX==m_ssX && MY==m_ssY && MovePercent==0)
	{
		// �ӽ� ���ۿ� �浹�ϴ� �����͸� �����Ѵ�. 
		SHORT temp_block=Block;
		
		// �ٸ� ��θ� ã�� ����� ������ �Ǿ� �ְ� '�̵� ���� �浹'�� �Ǿ� ���� �ʴٸ�, 
		if(FindAnotherPathSwitch==TRUE && (Block&BLOCK_MOVE)==0)
		{
			// �ٸ� ��θ� ã���� ����� ���� �����Ѵ�. 
			FindAnotherPathSwitch=FALSE;
			
			// �浹�Ӽ��� �̵� ���� �浹���� �����Ѵ�. 
			Block+=BLOCK_MOVE;
		}
		
		SHORT returnval = FindPathGame->FindPath(m_ssX, m_ssY, DX, DY, MX, MY, range, m_NewID.ID);
		
		// ������ �浹 �Ӽ��� �����Ѵ�. 
		Block=temp_block;
	
		// �ʹ� ���� ������ �������� �߰��� ������ �� ����. 
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
	
	// �̵� ������ �ʱ�ȭ�Ѵ�. 
	direction=0;
	
	// �̵��� ��ġ�� ã�´�. 
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

	// ���� �̵��ϱ� ���� �ɸ���, 
	// ��� ����ߴٰ� �ٽ� �̵� ��Ų��. 
	// ������ �ɸ��� �����. 

	if(IsColl(NX, NY, Block|BLOCK_MOVE) == TRUE || Map.IsColl(NX, NY, BLOCK_ICEWALL))
	{
		MoveBlockX=NX;
		MoveBlockY=NY;
		
		NX=m_ssX;
		NY=m_ssY;
		
		// ���� ��ٸ��� �̵� �Ϸ� 
		MoveBlockDelay=5;
		MoveBlockCount++;

		if(MoveBlockCount>=moveblockcount)
		{
			SetMove(DX, DY);
		}
		
		return MOVEINFO_MOVING;
	}
	
	MoveBlockCount=0;
	
	// ������ ��ȯ�Ѵ�. 
	if(direction)
	{
		if(ChangeDirection(direction)==FALSE)return MOVEINFO_MOVING;
	}
	
	SHORT movespeed;
	
	// ����� ������ ���̸� �ӵ��� �����. 
	// ����   ��������      �ӵ��� �����. 
	if(ClimbSwitch==TRUE || Map.GetShallowMap(m_ssX, m_ssY)==TRUE)
	{
		movespeed=MoveSpeed-MoveSpeed/3;
	}
	// �������̸� �ӵ��� �����. 
	else if(LowMoveSwitch==TRUE)
	{
		movespeed=MoveSpeed-MoveSpeed/2;
	}
	// �������̸� �ӵ��� �����. 
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
		
		// �̵��� ������ �����.
		MOX=m_ssX;
		MOY=m_ssY;
		MDX=NX;
		MDY=NY;
		
		m_ssX=NX;
		m_ssY=NY;
		
		// ���� �̵������� �˷��ִ� ���� 
		moveswitch=TRUE;
		
		// ���� ��ġ x,y���� �̷������ ��ġ�� ���Ѵ�. 
		OperateMapPosition(m_ssX, m_ssY);
	}
	// ��ǥ ��ȯ�� ������ �� ������, 
	else if(MovePercent<=0)
	{
	}
	else
	{
		// �̵��� ������ �����.
		MOX=m_ssX;
		MOY=m_ssY;
		MDX=NX;
		MDY=NY;
	}
	
	// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. 
	SHORT centerx, centery;
	Map.GetCenterXYByMapCenterXY(MOX, MOY, centerx, centery);
	
	// �̵��� ��ġ�� �߽��� ã�´�. 
	SHORT nx_centerx, ny_centery;
	Map.GetCenterXYByMapCenterXY(MDX, MDY, nx_centerx, ny_centery);
	
	//���� ��ǥ�� ���� ��ǥ���� ����� �� ������ ���Ѵ�.
	if(MovePercent>=0)
		GetMoveRatio(centerx, centery, nx_centerx, ny_centery, MovePercent, RX, RY);
	else
	{
		GetMoveRatio(nx_centerx, ny_centery, centerx, centery, abs(MovePercent), RX, RY);
	}
	
	// ��������� �ƴ��� Ȯ���Ѵ�. 
	if(Map.GetLevelInfo(m_ssX,m_ssY)==HILL_SLOPE)
	{
		ClimbSwitch=TRUE;
	}
	else ClimbSwitch=FALSE;  
	
    // ������ ������ ĳ�����̸�
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
	
	
	
	// �ɷ����� �ʰų� ù��° �������� �ƴϸ�,
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
	
	
	// �߰���ǥ���� �̻���� ����
	if(MovePercent==0)
	{
		// ���� ������ ���� 
		if(DX==m_ssX && DY==m_ssY)
		{
			return MOVEINFO_MOVEDONE;
		}
		// �߰� ������ ���� 
		if(MX==m_ssX && MY==m_ssY)
		{
			return MOVEINFO_HALFDONE;
		}
	}
	
	// ������ �̵��� ������ �˷��ش�. 
	if(moveswitch==TRUE)
	{
		return MOVEINFO_REALMOVE;
	}
	
	return MOVEINFO_MOVING;

}

// �̵� ��ƾ 
// ���ϰ�: 1 -> ������ ���� 
int cltCharOrg::MoveFly(SHORT moveblockcount, SHORT range)
{
   int x=m_ssX, y=m_ssY;
   int direction;
   int rx=RX, ry=RY, NX=m_ssX, NY=m_ssY;
   SHORT realnx=0, realny=0;
   SHORT realdx=0, realdy=0;
   SHORT i,j,k;

   // �̵��� ��ġ�� �Ϸ�Ǹ� �̵�, 
   // �ƴϸ� ��ġ 
   if(IsPreMoveOk()==FALSE)
   {
	   return 0;
   }

   // �������� ��Ʈ��ǥ�� ���Ѵ�. 
   SHORT movespeed=MoveSpeed+MoveSpeedVary;
   realdx=MapCenterX[DX][DY];
   realdy=MapCenterY[DX][DY];


   // �������� ���� ��θ� ã�´�. 

   // �̵� ������ �ʱ�ȭ�Ѵ�. 
   direction=DecideDirectionChar16(m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), realdx, realdy);

   // �������� ����� ���� ������ ��ġ�ϸ� �׳� �������� ����. 
   SHORT gomode=FALSE;
   if(TempDirection==direction)
   {
      gomode=TRUE;
   }
   else
   {
       gomode=FALSE;
   }

   // ������ ��ġ���� �ʰ� 
   // �������� ���� ��ǥ�� ������ �������� �ʰ� ���⸸ �ٲ۴�.
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


   // X, Y��ǥ�� ���Ѵ�.
   clifMap.GetRealTileByRealDot(realnx, realny, &m_ssX, &m_ssY);

   RX=realnx-MapCenterX[m_ssX][m_ssY];
   RY=realny-MapCenterY[m_ssX][m_ssY];

   // ������ ��ȯ�Ѵ�. 
   if(ChangeDirection(direction)==FALSE)return 0;

   // �������� �����ߴ�. 
   if(DX==m_ssX && DY==m_ssY)
   {
	   //���� �������� �ٸ� �װ��Ⱑ �ִٸ� �α��� �ٸ� �������� �ڸ��� �ű��. 
	   if(clifMap.GetSkyMap(m_ssX, m_ssY))
	   {
		   // �α��� ���ڸ��� ã�Ƽ� ����. (���� ��ġ���� ���� ����� ������)
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

	// �������� ���� 
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

// ���Ϸ� �̵��ϴ� �� ó��.
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

			// ��ġ�� �̵��ȴ�. 
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

// �̵��� �غ� �Ǿ��°�?
BOOL cltCharOrg::IsPreMoveOkSAMC()
{
	   return TRUE;
}

BOOL cltCharOrg::IsPreMoveOkFireTankK()
{
	// ����ڼ��� Ǯ������, 
	if(ReadyToFireStep == 0)return TRUE;

	return FALSE;
}


