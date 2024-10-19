#include <main.h>

#include <MapInterface.h>
#include <Magic.h>
#include <KindInfo.h>
#include <CharOrg.h>

extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_NewID 				ZeroID;


BOOL _Char::CanKimBomb()
{

	// �������� �����ϸ� �ȵȴ�. 
	if(m_ParmeterBox.IP.GetMana() < MAGICPOWER_KIMBOMB)return FALSE;


	return TRUE;
}

void _Char::KimBomb()
{
	Animation=ANI_ATTACK;

	WorkDelayor++;
	if(WorkDelayor < KI[Kind].clCharAnimation[AttackCharAnimationIndex].siDelay){ return ;}
	WorkDelayor=0;

	if(KI[Kind].clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()==0)
	{
		Step=0;
		AniStep=Step;
	}
	else
	{
	   ++Step %= KI[Kind].clCharAnimation[AttackCharAnimationIndex].GetFrameNumber();
	   AniStep=Step;
	   UpdateAnimationSwitch=TRUE;
	}

	
	if(Step == KI[Kind].clCharAnimation[AttackCharAnimationIndex].siInstant)
	{
		
/*		// ��� �ٿ��ش�. 
		// �������� �ٿ��ش�. 
		if(GD.Player[PlayerNumber].Goods[GOODS_AIRMAGIC]>0)
		{
			GD.Player[PlayerNumber].Goods[GOODS_AIRMAGIC]--;
		}
		else
		{
			OrderStop(NewID, BY_COMPUTER);
			return ;
		}
*/
		

		if(m_ParmeterBox.IP.GetMana() >= MAGICPOWER_KIMBOMB)
		{
			m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana()-MAGICPOWER_KIMBOMB);
		}
		else
		{
			OrderStop(NewID, BY_COMPUTER);
			return ;
		}
	
		// ������ ���ؼ� ������ ������ �ʵ��� ZeroID�� �ִ´�. 
		SetKimBomb(X, Y, ZeroID, PlayerNumber, AREA_LAND, GetApower(), m_ParmeterBox.GetHitRate(0), ZeroID);
			
	}

	 if(Step==0)
	 {
	   BusySwitch=FALSE;
	   OrderStop(NewID, BY_COMPUTER);
	   return ;
	 }
	 else
	 {
		 return ;
	 }
}

