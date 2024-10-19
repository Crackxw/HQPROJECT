#include <GSL.h>

#include <main.h>

#include <findpath5.h>
#include "maindata.h"
#include <etc.h>
#include "calcattack.h"


#include <Magic.h>

#include <map.h>
#include <smoke.h>
#include <CharOrg.h>

extern	_Char				CR[];								// ĳ������ ������ 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Map 				Map;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];


// id�� �������� ������ �� �ִ°�?
// id��ü�� �������� ������ Ȯ���Ѵ�. 
BOOL CanLightningGlobal(_NewID id)
{
	// ���� ĳ���ʹ� ���� ������ �� ����.
	if(IsSee(id) == FALSE)return FALSE;

	// ����ü�� ���� ������ �� ����.
	if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;

	return TRUE;
}


// �ڽ��� ������ �����Ѱ�?
BOOL _Char::CanLightning()
{
	// �������� ���� �־�� �Ѵ�. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_LIGHTNING)return FALSE;

	return TRUE;
}

BOOL _Char::CanLightning(_NewID id)
{

	if(CanLightningGlobal(id)==FALSE)return FALSE;


	if(CanLightning()==FALSE)return FALSE;


	return TRUE;
}

BOOL _Char::SetLightning(_NewID id)
{

	if(CanLightning(id)==FALSE)return FALSE;

	WorkStep=WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::LightningOperation(_NewID id)
{

	SHORT i, j;

	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return FALSE;}
	m_cltCharStatus.SetWorkDelayor(0);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()== 0)
	{
		Step = 0;
		m_cltCharDrawInfo.SetAniStep(Step);
	}
	else
	{
	   ++Step %= KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   m_cltCharDrawInfo.SetAniStep(Step);
	   m_cltCharDrawInfo.SetUpdateAnimationSwitch(TRUE);
	}

	
	 if(Step == KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siInstant)
	 {


		 // ������ ����ų �� ���ٸ�, 
	     if(CanLightning(id) == FALSE)
		 {    
		     m_scBusySwitch=FALSE;
	         OrderStop(m_NewID, BY_COMPUTER);
	         return FALSE;
		 }


	     // �������� �ٿ��ش�. 
		 ReduceMana(MAGICPOWER_LIGHTNING);

		 // ���ٴڿ� ���� �ٿ��ش�. 
		// Map.PutFire(CR[id.ID].X, CR[id.ID].Y, GD.Random()%2+1, GROUNDFIRE_LIFETIME);


		 // ������ ���� ���ݷ��� �޸��ش�.
		 SHORT level =  m_ParmeterBox.IP.GetLevel();
		 GD.Rain.MakeThunder(m_NewID, GetPlayerNumber(), id, CR[id.ID].m_ssX, CR[id.ID].m_ssY, CalcDamage(SPECIAL_LIGHTNING, m_ParmeterBox.CalcApower(), m_scMagicApplyRate, level));

		 // ������ ������ ���� �ֺ��� ���⵵ �����. 
			 for(i = -1;i <= 1;i++)
				 for(j = -1;j <= 1;j++)
				 {
					 if(level < 20 && (j!=0 || i!=0))continue;

					 if(Map.IsInMapArea(CR[id.ID].m_ssX+j, CR[id.ID].m_ssY+i)==TRUE)
					 {
				          SHORT smokeid=FindEmptySmoke();
				          if(smokeid)
						  {
					          SHORT centerx=MapCenterX[CR[id.ID].m_ssX+j][CR[id.ID].m_ssY+i];
					          SHORT centery=MapCenterY[CR[id.ID].m_ssX+j][CR[id.ID].m_ssY+i];
				  

				              switch(GD.Random()%3)
							  { 	
				                case 0:
		                           SetSmoke(KIND_SMOKE_LIGHTEXP1, smokeid, centerx,centery-20, AREA_LAND);
					               break;
				               case 1:
					               SetSmoke(KIND_SMOKE_LIGHTEXP2, smokeid, centerx,centery-20, AREA_LAND);
					               break;
				               case 2:
					               SetSmoke(KIND_SMOKE_LIGHTEXP3, smokeid, centerx,centery-20, AREA_LAND);
					               break;
							  }  

						  }
					 }
				 }


	     OrderStop(m_NewID, BY_COMPUTER);
	 }

	 if(Step == 0)
	 {
	   m_scBusySwitch=FALSE;
	   return TRUE;
	 }
	 else return FALSE;

}

BOOL _Char::Lightning()
{
	SHORT status;
	SHORT length=0;
	_NewID targetid=Para2;
	SHORT healrange = GetAttackRange() + 2;
	
   switch(WorkStep)
   {
	  case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 

		 // ������ ����ų �� ���ٸ�, �ߴ��Ѵ�.
		 if(CanLightning(targetid)==FALSE)
		 {   
			 m_scBusySwitch=FALSE;
		     OrderStop(m_NewID, BY_COMPUTER);
		     return FALSE;
		 }

		 // �����Ÿ� �ȿ� ������,
		 if(IsInRange(targetid.ID, healrange) == TRUE )
		 { 
			 WorkStep = WS_HEAL;
		 }
		 else // ���� ��Ÿ� �ۿ� ������, 
		 {  
			  WorkStep = WS_GOTO_TARGET;

		      // ���� �ִ� ������ �̵��Ѵ�. 
			  SetMove(CR[targetid.ID].GetX(), CR[targetid.ID].GetY());
		 }
		break;


	case WS_GOTO_TARGET:

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
			   if(CanLightning(targetid)==FALSE)
			   {  
				   OrderStop(m_NewID, BY_COMPUTER);
				   return FALSE;
			   }
			   if(IsInRange(AttackID.ID, healrange))
			   {
				  WorkStep=WS_HEAL;
				  Step=0;
			   }

			   // �������� �����ϰų� �߰� ������ �����ϸ�, 
		       if(status>0)
			   {
			      WorkStep=WS_DECIDE_LENGTH_TO_DEST;
				   
			      AttackGiveUpDelayor++;
		          if(AttackGiveUpDelayor>AttackGiveUpDelay)
				     OrderStop(m_NewID, BY_COMPUTER);
			   }

		   }

		break;

	case WS_HEAL:

		 // ������ ģ��.
		 LightningOperation(targetid);

	     break;
	}
	
	return FALSE;
}



