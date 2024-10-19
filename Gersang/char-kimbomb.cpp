#include <main.h>

#include <MapInterface.h>
#include <Magic.h>
#include <KindInfo.h>
#include <CharOrg.h>

extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_NewID 				ZeroID;


BOOL _Char::CanKimBomb()
{

	// 마법력이 부족하면 안된다. 
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
		
/*		// 재고를 줄여준다. 
		// 마법력을 줄여준다. 
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
	
		// 마법에 의해서 레벨이 오르지 않도록 ZeroID를 넣는다. 
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

