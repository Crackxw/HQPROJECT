//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"
#include <findpath5.h>
#include <etc.h>
#include "..\Gersang\music.h"
#include <CharOrg.h>
#include <Magic.h>
#include <Map.h>
#include <bullet.h>
#include <Smoke.h>
#include <setchar.h>
#include "calcattack.h"

extern	_MainData			GD;
extern	_KindInfo			KI[];
extern	_Map				Map;
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	_NewID 				ZeroID;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];


#define SPECIALINFOTYPE_RANGE		1			// 유효거리. 
#define SPECIALINFOTYPE_MAGICPOWER	2			// 필요 마법력. 
#define SPECIALINFOTYPE_ATB3		3			// 필요 속성.  
#define SPECIALINFOTYPE_ATB4		4			// Atb4

// 한가지 특수 공격에 대한 정보가 몇 가지나 되는가?
#define SPECIALINFO_DATA_FIELD_NUMBER 5

static SI32 SpecialInfo[]=
{
	// 기술 종류,		유효거리	마법력						필요속성1		필요속성2 
	SPECIAL_MAGIC5J,		0,		MAGICPOWER_MAGIC5J,			ATB3_MAGIC5J,		0,		// 만월도끼춤.
	SPECIAL_MAGIC8J,		5,		MAGICPOWER_MAGIC8J,			0,					0,		// 환영술
	SPECIAL_MAGIC4C,		10,		MAGICPOWER_MAGIC4C,			ATB3_MAGIC4,		0,		// 기공신포 
	SPECIAL_MAGIC6C,		10,		MAGICPOWER_MAGIC6C,			ATB3_MAGIC6C,		0,		// 포박술 
	SPECIAL_ICE,			10,		MAGICPOWER_ICE,				ATB3_ICE,			0,		// 이사의사념 특수공격.
	SPECIAL_AMBUSH,			0,		0,							0,					0,		// 매복 
	SPECIAL_DETECTMINE,		0, 		MAGICPOWER_DETECTMINE,		0,					0,		// 지뢰탐지 
	SPECIAL_EXTINGUISH,		0,		MAGICPOWER_EXTINGUISH,		ATB3_EXTINGUISH,	0,		// 소화술
	SPECIAL_KIMBOMB,		0,		MAGICPOWER_KIMBOMB,			ATB3_KIMBOMB,		0,		// 폭뢰격 
	SPECIAL_GROUNDEXP,		10,		MAGICPOWER_GROUNDEXP,		ATB3_GROUNDEXP,		0,		// 연옥술 
	SPECIAL_TORNADO,		10,		MAGICPOWER_TORNADO,			ATB3_TORNADO,		0,		// 풍백술. 
	SPECIAL_MAGIC4T,		10,		MAGICPOWER_MAGIC4T,			ATB3_MAGIC4T,		0,		// 원심분쇄격 => 흑룡대차륜
	SPECIAL_MAGIC2T,		10,		MAGICPOWER_MAGIC2T,			ATB3_POISONBULLET,	0,		// 독무술. 
	/////////////////////////////////////////////////////////////////////////////////
	SPECIAL_MAGICTEST,		10,		MAGICPOWER_MAGICTEST,		ATB3_MAGICTEST,		0,		//마법 테스트.
	/////////////////////////////////////////////////////////////////////////////////
	SPECIAL_MINE,			0,		MAGICPOWER_MINE		,		ATB3_MINE,			0,		// 지뢰.
	SPECIAL_YELLOWSAND,		10,		MAGICPOWER_YELLOWSAND,		ATB3_YELLOWSAND,	0,		// 황사.
	SPECIAL_SOUL,			0,		MAGICPOWER_SOUL,			ATB3_SOUL,			0,		// 왕거한 야수의 혼.
	SPECIAL_GHOST,			10,		MAGICPOWER_GHOST,			ATB3_GHOST,			0,		// 페르난데스 귀신탄.
	SPECIAL_DARKFIRE,		10,		MAGICPOWER_DARKFIRE,		ATB3_DARKFIRE,		0,		// 암흑화염폭풍.
	SPECIAL_IVORY,			10,		MAGICPOWER_IVORY,			0,					ATB4_IVORY,				//상아폭격.
	SPECIAL_BLUEDRAGON,		0,		MAGICPOWER_BLUEDRAGON,		0,					ATB4_BLUEDRAGON,		//비격청룡도.
	SPECIAL_RUNNINGFIRE,	10,		MAGICPOWER_RUNNINGFIRE,		0,					ATB4_RUNNINGFIRE,		//연발포탄.
	SPECIAL_ENERGYPA,		10,		MAGICPOWER_ENERGYPA,		0,					ATB4_ENERGYPA,			//장풍.
	SPECIAL_SNOWATTACK,		10,		MAGICPOWER_SNOWATTACK,		0,					ATB4_SNOWATTACK,		//설공신포.
	SPECIAL_FIREATTACK,		10,		MAGICPOWER_FIREATTACK,		0,					ATB4_FIREATTACK,		//화공신포.
	SPECIAL_FIREENERGYPA,	10,		MAGICPOWER_FIREENERGYPA,	0,					ATB4_FIREENERGYPA,		//열화장풍(비격황룡권).
	SPECIAL_DOUBLE_SWORD,	0,		MAGICPOWER_DOUBLE_SWORD,	0,					ATB4_DOUBLE_SWORD,		//쌍검난무.
	SPECIAL_LONG_SPEAR,		10,		MAGICPOWER_LONG_SPEAR,		0,					ATB4_LONG_SPEAR,		//장창소환.
	SPECIAL_ICE_BARRIER,	18,		MAGICPOWER_ICE_BARRIER,		0,					ATB4_ICE_BARRIER,		//빙석술.
	SPECIAL_SOUL_CHANGE,	10,		MAGICPOWER_SOUL_CHANGE,		0,					ATB4_SOUL_CHANGE,		//빙의술.
	SPECIAL_FLOWER_NEEDLE_RAIN,10,	MAGICPOWER_FLOWER_NEEDLE_RAIN,0,				ATB4_FLOWER_NEEDLE_RAIN,//만천화우
	SPECIAL_STONE_MAGIC,	10,		MAGICPOWER_STONE_MAGIC,		0,					ATB4_STONE_MAGIC,		//석괴술
	SPECIAL_GOD_SPEAR,		5,		MAGICPOWER_GOD_SPEAR,		0,					ATB4_GOD_SPEAR,			//신창노도
	SPECIAL_CLOUD_WIND,		10,		MAGICPOWER_CLOUD_WIND,		0,					ATB4_CLOUD_WIND,		//풍모술
	SPECIAL_MANABURN,		10,		MAGICPOWER_MANABURN,		0,					ATB4_MANABURN,			//마나번.

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:48) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	// 기술 종류,					유효거리		필요 마법력						필요속성1		필요속성2 
	SPECIAL_FIRE_ROUND_DANCE,			0,		MAGICPOWER_FIRE_ROUND_DANCE,			0,		ATB4_FIRE_ROUND_DANCE,			// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	SPECIAL_NOVA,						0,		MAGICPOWER_NOVA,						0,		ATB4_NOVA,						// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	SPECIAL_FURY_OF_VOLCANO,		    8,		MAGICPOWER_FURY_OF_VOLCANO,				0,		ATB4_FURY_OF_VOLCANO,			// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	SPECIAL_POISONING_HEAVENS,			0,		MAGICPOWER_POISONING_HEAVENS,			0,		ATB4_POISONING_HEAVENS,			// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	SPECIAL_BOMB_OF_HEAVEN,			   12,		MAGICPOWER_BOMB_OF_HEAVEN,				0,		ATB4_BOMB_OF_HEAVEN,			// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
	SPECIAL_SIEGE_MODE,					0,		MAGICPOWER_SIEGE_MODE,					0,		ATB4_SIEGE_MODE,				// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
	SPECIAL_ONSLAUGHT_FIREBALL,		   10,		MAGICPOWER_ONSLAUGHT_FIREBALL,			0,		ATB4_ONSLAUGHT_FIREBALL,		// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	SPECIAL_FIRERAIN_OF_GOD,			0,		MAGICPOWER_FIRERAIN_OF_GOD,				0,		ATB4_FIRERAIN_OF_GOD,			// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
	// 기술 종류,					유효거리		필요 마법력						필요속성1		필요속성2 
	-1
};


// 특수 기술의 정보를 얻는다. 
SI32 cltCharOrg::GetSpecialInfo(SI32 siAttackAtb, SI32 infotype)
{
	SI32 index = 0;
	SI32 data;
	while(1)
	{
		data = SpecialInfo[index * SPECIALINFO_DATA_FIELD_NUMBER];

		if(data == -1)
		{
			clGrp.Error("fsdf93j3", "fsdf397hjf:%d", siAttackAtb);
			return -1;
		}

		if(data == siAttackAtb)
		{
			switch(infotype)
			{
			case SPECIALINFOTYPE_RANGE:
				return SpecialInfo[index * SPECIALINFO_DATA_FIELD_NUMBER + 1];
				break;
			case SPECIALINFOTYPE_MAGICPOWER:
				return SpecialInfo[index * SPECIALINFO_DATA_FIELD_NUMBER + 2];
				break;
			case SPECIALINFOTYPE_ATB3:
				return SpecialInfo[index * SPECIALINFO_DATA_FIELD_NUMBER + 3];
				break;

			case SPECIALINFOTYPE_ATB4:
				return SpecialInfo[index * SPECIALINFO_DATA_FIELD_NUMBER + 4];

			default:
				clGrp.Error("fds89yfd", "4975dfjdf:%d", infotype);
				break;
			}
		}

		index++;
	}
}

BOOL cltCharOrg::Ambush()
{

	int i = 0, j = 0, k = 0;
	SHORT id = 0;

	if(AmbushDelay)
	{
		AmbushDelay --;
		AmbushSwitch = FALSE;
	}
	else
		AmbushSwitch = TRUE;

	// 화면상에 유효한 경우에만 작동한다. 
	if(m_scSeeSwitch==FALSE)return FALSE;


	SetAnimation(ANI_AMBUSH);
	DoAnimationDelay(TRUE);


	/*
	Animation = ANI_AMBUSH;

	// 움직임 애니메이션 
	WorkDelayor++;
	if(WorkDelayor >= KI[m_cltCharStatus.GetKind()].clCharAnimation[AmbushCharAnimationIndex].siDelay)
	{
		WorkDelayor = 0;
		Step++;
		Step%= KI[m_cltCharStatus.GetKind()].clCharAnimation[AmbushCharAnimationIndex].GetFrameNumber();
		
		AniStep=Step;
		UpdateAnimationSwitch=TRUE;
	}
	*/

	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Description	:	지뢰탐지
void cltCharOrg::DetectMine()
{
	m_cltCharDrawInfo.SetAnimation(ANI_ATTACK);

	m_cltCharStatus.SetWorkDelayor(m_cltCharStatus.GetWorkDelayor() + 1);
	if(m_cltCharStatus.GetWorkDelayor() < KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].siDelay){ return ;}
	m_cltCharStatus.SetWorkDelayor(0);

	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber()==0)
	{
		Step=0;
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
		if( m_ParmeterBox.IP.GetMana() >= MAGICPOWER_DETECTMINE )
		{
			m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_DETECTMINE);
		}
		else
		{
			OrderStop(m_NewID, BY_COMPUTER);
			return ;
		}

		
		// 마법에 의해서 레벨이 오르지 않도록 ZeroID를 넣는다. 
		SetMagicDetectMine(m_ssX, m_ssY, GetPlayerNumber());
			
	}

	 if(Step==0)
	 {
	   m_scBusySwitch=FALSE;
	   OrderStop(m_NewID, BY_COMPUTER);
	   return ;
	 }
	 else return ;
}

BOOL cltCharOrg::SpecialAtb4(SI32 siSpecial)
{
	SHORT	status;
	SHORT	length;
	_NewID	targetid;
	SHORT	x, y;

	// 특수 기술 구현 범위 .
	SI32 range	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_RANGE);
	SI32 magic	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_MAGICPOWER);

	// 목표로 삼는 캐릭터  
	targetid = Para2;

	// 목표 위치. 
	x = LOWORD(Para1);
	y = HIWORD(Para1);


	if( siSpecial )
	{
		length=0;
		
		switch(WorkStep)
		{
		case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
			
			if(CanSpecial(siSpecial, targetid)==FALSE)
			{   
				m_scBusySwitch=FALSE;
				OrderStop(m_NewID, BY_COMPUTER);
				return FALSE;
			}
			
			// 사정거리 안에 있으면,
			if(max(abs(x - m_ssX), abs(y - m_ssY) ) < range )
			{ 
				//Direction = DecideDirection8(m_ssX, m_ssY, x, y);
				WorkStep= WS_ATTACK;
			}
			else // 적이 사거리 밖에 있으면, 
			{  
				WorkStep=WS_GOTO_TARGET;
				
				// 적이 있는 곳으로 이동한다. 
				SetMove(x, y);
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
				if(CanSpecial(siSpecial, targetid)==FALSE)
				{  
					OrderStop(m_NewID, BY_COMPUTER);
					return FALSE;
				}
				if(max(abs(x - m_ssX), abs(y - m_ssY) ) < range )
				{
					WorkStep= WS_ATTACK;
					Step=0;
				}
				
				// 목적지에 도착하거나 중간 지점에 도착하면, 
				if(status>0)
				{
					WorkStep = WS_DECIDE_LENGTH_TO_DEST;
					
					AttackGiveUpDelayor++;
					if(AttackGiveUpDelayor>AttackGiveUpDelay)
						OrderStop(m_NewID, BY_COMPUTER);
				}
				
			}
			
			break;
			
		case WS_ATTACK:
			SpecialOperationAtb4(siSpecial, targetid, x, y);
			break;
		}
		
		return FALSE;
		
	}
	return TRUE;
}

BOOL cltCharOrg::SpecialOperationAtb4(SI32 siSpecial, _NewID id, SI32 x, SI32 y)
{
	SI32 attackinstant;

	SI32 range	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_RANGE);
	SI32 magic	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_MAGICPOWER);


	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber())
	{
		SetAnimation(ANI_MAGIC);
	}
	else
	{
		SetAnimation(ANI_ATTACK);
	}

	
	if(DoAnimationDelay(TRUE) == FALSE)return FALSE;
	
	attackinstant = GetAniInstant();

	
	if(m_cltCharDrawInfo.GetAniStep() == attackinstant)
	{

		if(CanSpecialAtb4(siSpecial, id) == FALSE)
		{    
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		
		// 마법력을 줄여준다. 
		if( m_ParmeterBox.IP.GetMana() >= magic )
		{
			// 마법력을 줄여준다. 
			ReduceMana(magic);
		}
		else
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		SpecialActAtb4(siSpecial, id, x, y);
		
		OrderStop(m_NewID, BY_COMPUTER);
	}
	
	if(Step==0)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}
	else return FALSE;
	
	return FALSE;
}

SI32 cltCharOrg::SpecialActAtb4(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y)
{
	SI32 bulletid;
//	SI32 smokeid;
//	SI32 level;

	switch(siAttackAtb)
	{
	case SPECIAL_IVORY:
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
		{
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		//temp
		::SetMagicIvory(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_IVORY, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);	//min(100, m_ParmeterBox.GetHitRate(CR[id.ID].m_ParmeterBox.GetDR()) * 2)

	break;

	case SPECIAL_BLUEDRAGON:
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
		{
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		//temp
		::SetMagicBlueDragon(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_BLUEDRAGON, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);

	break;

	case SPECIAL_FIREATTACK:
		bulletid = FindEmptyBullet();
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
		{
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		//temp
		SetBullet(KIND_BULLET_FIREATTACK, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, AttackID, CalcDamage(SPECIAL_FIREATTACK, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
	break;
	//연발폭탄.
	case SPECIAL_RUNNINGFIRE:
		bulletid = FindEmptyBullet();
		if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
		{
			  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		SetBullet(KIND_BULLET_RUNNINGFIRE, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, AttackID, CalcDamage(SPECIAL_RUNNINGFIRE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
	break;
	//장풍
	case SPECIAL_ENERGYPA:
		bulletid = FindEmptyBullet();
		SetBullet(KIND_BULLET_ENERGYPA, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, ZeroID, CalcDamage(SPECIAL_ENERGYPA, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
		if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
		{
			  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		::SetMagicEnergyPa(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_ENERGYPA, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);

	break;
	//열화장풍(비격황룡권).
	case SPECIAL_FIREENERGYPA:
		{
			if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
			{
				  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
			}
			
			bulletid = FindEmptyBullet();
			SetBullet(KIND_BULLET_FIREENERGYPA, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, AttackID, CalcDamage(SPECIAL_FIREENERGYPA, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
		}
	break;
	case SPECIAL_MANABURN:
		::SetMagicManaBurn(m_ssX,m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_MANABURN, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
		break;
	case SPECIAL_DOUBLE_SWORD:					// 쌍검난무
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		::SetMagicDoubleSword(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_DOUBLE_SWORD, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
		break;
	case SPECIAL_ICE_BARRIER:					// 빙석술
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicIceBarrier(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_ICE_BARRIER, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_FLOWER_NEEDLE_RAIN:			// 만천화우
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicFlowerNeedleRain(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_FLOWER_NEEDLE_RAIN, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_SOUL_CHANGE:					// 빙의술
		bulletid = FindEmptyBullet();
		if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
		{
			  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		::SetMagicSoulChange(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_SOUL_CHANGE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
		break;
	case SPECIAL_STONE_MAGIC:					// 석괴술
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicStoneMagic(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_STONE_MAGIC, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_GOD_SPEAR:						// 신창노도
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicGodSpear(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_GOD_SPEAR, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_CLOUD_WIND:					// 풍모술
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicCloudWind(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_CLOUD_WIND, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;

	//----------------------------------------------------------------------------
	// robypark (2004/05/17 15:48) : 추가 기술 - 장수 2차 전직 특수 기술
	//----------------------------------------------------------------------------

	// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	case SPECIAL_FIRE_ROUND_DANCE:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_FIRE_ROUND_DANCE(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_FIRE_ROUND_DANCE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	
	// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	case SPECIAL_NOVA:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			PushEffect(EFFECT_THUNDER, m_ssX, m_ssY);

			// 뇌전차 머리 위로 번개 내리치는 smoke 생성
			SI32 smokeid = FindEmptySmoke();
			if(smokeid)
				SetSmoke(KIND_SMOKE_LIGHTNING, smokeid, MapCenterX[m_ssX][m_ssY], MapCenterY[m_ssX][m_ssY] - 70, AREA_AIR);

			// 16방향으로 총알 생성
			CreateBullet_SpecialNOVA(m_ssX, m_ssY, m_NewID);
		}
		break;

	// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	case SPECIAL_FURY_OF_VOLCANO:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_FURY_OF_VOLCANO(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), 100, id);
			 PushEffect(EFFECT_MAGIC4C, x, y);
		}
		break;

	// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	case SPECIAL_POISONING_HEAVENS:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_POISONING_HEAVENS(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_POISONING_HEAVENS, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;

	// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
	case SPECIAL_BOMB_OF_HEAVEN:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			// 목표의 방향으로 캐릭터가 바라보도록 한다.
			ChangeDirection(DecideDirection8(m_ssX, m_ssY, x, y));

//			::SetMagic_BOMB_OF_HEAVEN(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_BOMB_OF_HEAVEN, m_ParmeterBox.CalcApower(), MagicApplyRate), 100, id);
			PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
			SI32 siBulletID = FindEmptyBullet();
			// 총알 생성
			SetBullet(KIND_BULLET_BOMB_OF_HEAVEN, siBulletID, m_ssX, m_ssY, x, y, \
							m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 40, \
							MapCenterX[x][y], MapCenterY[x][y],\
							m_NewID, ZeroID, \
							CalcDamage(SPECIAL_BOMB_OF_HEAVEN, m_ParmeterBox.CalcApower(), m_scMagicApplyRate),
							m_ParmeterBox.GetHitRate(0), TargetDirection, AttackArea);

			// 탄착 이후 승천포의 원래 기술이 사용된다는 겻을 명시
			// 즉, 이미지 그리기 및 데미지 계산, 등의 일을 처리한다는 의미
			Bullet_CurrentOrder(siBulletID, ORDER_BULLET_EVENT_IMPACT);
		}
		break;

	// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
	case SPECIAL_SIEGE_MODE:
		break;
	
	// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	case SPECIAL_ONSLAUGHT_FIREBALL:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			// 목표의 방향으로 캐릭터가 바라보도록 한다.
			ChangeDirection(DecideDirection8(m_ssX, m_ssY, x, y));

			// 맹격화룡파 총알 생성
			CreateBullet_ONSLAUGHT_FIREBALL(m_ssX, m_ssY, x, y, m_NewID);
		}
		break;
		
	// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
	case SPECIAL_FIRERAIN_OF_GOD:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_FIRERAIN_OF_GOD(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_FIRERAIN_OF_GOD, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Description	:	필살기 적용
BOOL cltCharOrg::Special(SI32 siAttackAtb)
{
	SHORT	status;
	SHORT	length;
	_NewID	targetid;
	SHORT	x, y;

	// 특수 기술 구현 범위 .
	SI32 range	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_RANGE);
	SI32 magic	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_MAGICPOWER);

	// 목표로 삼는 캐릭터  
	targetid = Para2;

	// 목표 위치. 
	x = LOWORD(Para1);
	y = HIWORD(Para1);


	switch( siAttackAtb )
	{

		// 매복
	case SPECIAL_AMBUSH:
		return Ambush();
		
		// 지뢰탐지
	case SPECIAL_DETECTMINE:
		DetectMine();
		break;
		
	default:
		length=0;
		
		switch(WorkStep)
		{
		case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
			
			if(CanSpecial(siAttackAtb, targetid)==FALSE)
			{   
				m_scBusySwitch=FALSE;
				OrderStop(m_NewID, BY_COMPUTER);
				return FALSE;
			}
			
			// 사정거리 안에 있으면,
			if(max(abs(x - m_ssX), abs(y - m_ssY) ) < range )
			{ 
				//Direction = DecideDirection8(m_ssX, m_ssY, x, y);
				WorkStep=WS_HEAL;
			}
			else // 적이 사거리 밖에 있으면, 
			{  
				WorkStep=WS_GOTO_TARGET;
				
				// 적이 있는 곳으로 이동한다. 
				SetMove(x, y);
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
				if(CanSpecial(siAttackAtb, targetid)==FALSE)
				{  
					OrderStop(m_NewID, BY_COMPUTER);
					return FALSE;
				}
				if(max(abs(x - m_ssX), abs(y - m_ssY) ) < range )
				{
					WorkStep=WS_HEAL;
					Step=0;
				}
				
				// 목적지에 도착하거나 중간 지점에 도착하면, 
				if(status>0)
				{
					WorkStep = WS_DECIDE_LENGTH_TO_DEST;
					
					AttackGiveUpDelayor++;
					if(AttackGiveUpDelayor>AttackGiveUpDelay)
						OrderStop(m_NewID, BY_COMPUTER);
				}
				
			}
			
			break;
			
		case WS_HEAL:
			SpecialOperation(siAttackAtb, targetid, x, y);
			break;
		}
		
		return FALSE;
		
	}
	return TRUE;
}


BOOL cltCharOrg::CanSpecialAtb4(SI32 siSpecial, _NewID id)
{
	SI32 magic	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_MAGICPOWER);
	SI32 atb4	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_ATB4);


	// 구현에 마법이 필요한 것이라면, 
	if(magic)
	{
		// 마법력이 남아 있어야 한다. 
		if(m_ParmeterBox.IP.GetMana() < magic)return FALSE;
	}

	// 구현에 필요한 속성이 있는 것이라면, 
	if(atb4)
	{
		// 속성이 있는지 확인한다.
		if(IsAtb4(atb4) == FALSE)return FALSE;
	}

	switch(siSpecial)
	{
	// 석괴술
	case SPECIAL_STONE_MAGIC:						
		if( IsSee(id) == FALSE )				return FALSE;
		if( CR[id.ID].IsAtb(ATB_FLY) )			return FALSE;
		break;
	case SPECIAL_SOUL_CHANGE:
		if(IsSee(id) == FALSE)											   return FALSE;
		if(KI[ CR[id.ID].m_cltCharStatus.GetKind() ].IsKindAtb(KINDATB_GENERAL))return FALSE;
		if(IsFriendToByPlayerNumber(GetPlayerNumber(), CR[id.ID].m_cltCharStatus.GetPlayerNumber())==TRUE)return FALSE;
		break;
	}

	return TRUE;
}

BOOL cltCharOrg::SetSpecialAtb4(SI32 siSpecial, _NewID id)
{
	SI32 range	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_RANGE);

	// 유효거리가 있는 기술은 먼저 대상 지역과의 거리를 계산해야 한다.
	if(range)
	{
		if(CanSpecialAtb4(siSpecial, id) == FALSE)return FALSE;
		WorkStep = WS_DECIDE_LENGTH_TO_DEST;
		AttackGiveUpDelayor = 0;
	}
	// 유효거리가 없는 기술은 바로 그 자리에서 구현한다.
	else
	{
		if( CanSpecialAtb4(siSpecial, id) == FALSE )	return FALSE;
		WorkStep = WS_ATTACK;
		AttackGiveUpDelayor = 0;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Description	:	마법을 쓸 수 있는가
BOOL cltCharOrg::CanSpecial(SI32 siAttackAtb, _NewID id, SI32 *pReturnVal)
{
	SI32 magic	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_MAGICPOWER);
	SI32 atb3	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_ATB3);


	// 구현에 마법이 필요한 것이라면, 
	if(magic)
	{
		// 마법력이 남아 있어야 한다. 
		if(m_ParmeterBox.IP.GetMana() < magic)return FALSE;
	}

	// 구현에 필요한 속성이 있는 것이라면, 
	if(atb3)
	{
		// 속성이 있는지 확인한다.
		if(IsAtb3(atb3) == FALSE)return FALSE;
	}

	
	switch( siAttackAtb )
	{

	// 기공신포
	case SPECIAL_MAGIC4C:						
		if( IsSee(id) == FALSE )				return FALSE;
		if( CR[id.ID].IsAtb(ATB_FLY) )			return FALSE;
		break;

		// 풍백술. 
	case SPECIAL_TORNADO:
	case SPECIAL_YELLOWSAND:
		// 비행 캐릭터는 공격할 수 없다. 
		if(CR[id.ID].IsAtb(ATB_FLY))return FALSE;
		break;

	}

	return TRUE;

}




///////////////////////////////////////////////////////////////////////////////
// Description	:	
BOOL cltCharOrg::SpecialOperation(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y)
{
	
	SI32 attackinstant;

	SI32 range	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_RANGE);
	SI32 magic	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_MAGICPOWER);

	if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Magic2CharAnimationIndex].GetFrameNumber())
	{
		SetAnimation(ANI_MAGIC2);
	}
	else if(KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].GetFrameNumber())
	{
		SetAnimation(ANI_MAGIC);
	}
	else
	{
		SetAnimation(ANI_ATTACK);
	}

	
	if(DoAnimationDelay(TRUE) == FALSE)return FALSE;
	
	attackinstant = GetAniInstant();

	
	if(m_cltCharDrawInfo.GetAniStep() == attackinstant)
	{

		if(CanSpecial(siAttackAtb, id) == FALSE)
		{    
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		
		// 마법력을 줄여준다. 
		if( m_ParmeterBox.IP.GetMana() >= magic ) 
		{
			// 마법력을 줄여준다. 
			ReduceMana(magic);
		}
		else
		{
			m_scBusySwitch=FALSE;
			OrderStop(m_NewID, BY_COMPUTER);
			return FALSE;
		}
		
		SpecialAct(siAttackAtb, id, x, y);
		
		OrderStop(m_NewID, BY_COMPUTER);
	}
	
	if(Step==0)
	{
		m_scBusySwitch=FALSE;
		return TRUE;
	}
	else return FALSE;
	
	return FALSE;
}




BOOL cltCharOrg::FindNearFire(int* six, int* siy)
{
	SHORT i, j;
	SHORT range;

	for(range=1;range<=8;range++)
	{
	   for(i=-range;i<=range;i++)
	      for(j=-range;j<=range;j++)
		  {
			  if(i==-range || i==range || j==-range || j==range){}
			  else continue;

			  // 유효한 지도 영역에 있어야 한다. 
			  if(Map.IsInMapArea(m_ssX+j, m_ssY+i)==FALSE)continue;
 
			  if(Map.GetFireMap(m_ssX+j, m_ssY+i))
			  {
				  *six=m_ssX+j;
				  *siy=m_ssY+i;
				  return TRUE;
			  }
		  }
	  }

	 return FALSE;
}

// 가까운 안전지대를 찾는다. 
BOOL cltCharOrg::FindNearFireNo(int* six, int* siy)
{
	SHORT i, j;
	SHORT range;

	for(range=1;range<=8;range++)
	{
	   for(i=-range;i<=range;i++)
	      for(j=-range;j<=range;j++)
		  {
			  if(i==-range || i==range || j==-range || j==range){}
			  else continue;

			  // 유효한 지도 영역에 있어야 한다. 
			  if(Map.IsInMapArea(m_ssX+j, m_ssY+i)==FALSE)continue;
 
			  if(Map.GetFireMap(m_ssX+j,m_ssY+i) == 0)
			  {
				  *six=m_ssX+j;
				  *siy=m_ssY+i;
				  return TRUE;
			  }
		  }
	  }

	 return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Description	:	
BOOL cltCharOrg::SetSpecial(SI32 siAttackAtb, _NewID id, SI32 *pReturnVal)
{
	SI32 range	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_RANGE);

	// 유효거리가 있는 기술은 먼저 대상 지역과의 거리를 계산해야 한다.
	if(range)
	{
		if(CanSpecial(siAttackAtb, id) == FALSE)return FALSE;
		WorkStep = WS_DECIDE_LENGTH_TO_DEST;
		AttackGiveUpDelayor = 0;
	}
	// 유효거리가 없는 기술은 바로 그 자리에서 구현한다.
	else
	{
		if( CanSpecial(siAttackAtb, id) == FALSE )	return FALSE;
		WorkStep = WS_HEAL;
		AttackGiveUpDelayor = 0;
	}

	return TRUE;
}


SI32 cltCharOrg::SpecialAct(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y)
{
	SI32 bulletid;
	SI32 smokeid;
	SI32 level;

	switch(siAttackAtb)
	{
	case SPECIAL_GROUNDEXP:
		bulletid = FindEmptyBullet();
		if(bulletid)
		{
			if(Map.IsInMapArea(x, y) == TRUE)
			{
				SetBullet(KIND_BULLET_GROUNDEXP, bulletid, m_ssX, m_ssY, x, y, 
					m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
					MapCenterX[x][y], MapCenterY[x][y], 
					m_NewID, ZeroID, CalcDamage(SPECIAL_GROUNDEXP, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
				

				PushEffect(EFFECT_ATTACK_GENERAL_K8_EXT2, m_ssX, m_ssY); 

				// hojae_append
				Bullet_DrawSwitch(bulletid, FALSE);
				Bullet_CurrentOrder(bulletid, ORDER_BULLET_WAITEXP);
				
			}
			else
			{
				clGrp.Error("fsd7232", "x:%d  y:%d", x, y);
			}
		}
		break;
	case SPECIAL_DARKFIRE:
		bulletid = FindEmptyBullet();
		if(bulletid)
		{
			if(Map.IsInMapArea(x, y) == TRUE)		// m_ssX,m_ssY(주인공위치) x,y(목표위치)
			{
				SetBullet(KIND_BULLET_DARKFIRE, bulletid, m_ssX, m_ssY, x, y, 
					m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
					MapCenterX[x][y], MapCenterY[x][y], 
					m_NewID, ZeroID, CalcDamage(SPECIAL_DARKFIRE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);

				//암흑화염폭풍 사운드.
				PushEffect(EFFECT_ATTACK_GENERAL_T3_EXT, m_ssX, m_ssY); 
				
				// hojae_append
				Bullet_DrawSwitch(bulletid, FALSE);
				Bullet_CurrentOrder(bulletid, ORDER_BULLET_DARKFIRE);
				
			}
			else
			{
				clGrp.Error("fsd7232", "x:%d  y:%d", x, y);
			}
		}
		break;
	case SPECIAL_MAGIC2T:
		::SetMagic2T(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower()/4, 70, id);
		
		break;

	case SPECIAL_MAGIC6C:
		bulletid = FindEmptyBullet();
		if( bulletid )
		{
			SetBullet(KIND_BULLET_MAGIC6C, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, ZeroID, CalcDamage(SPECIAL_MAGIC6C, m_ParmeterBox.CalcApower(), m_scMagicApplyRate, m_ParmeterBox.IP.GetLevel()), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
			
			// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_MAGIC6C, m_ssX, m_ssY);
			
		}
		break;

	case SPECIAL_MAGIC8J:
/*		// 적의 종류를 알아낸다. 
		SI32 sikind = CR[id.ID].m_cltCharStatus.GetKind();
		
		// 빈자리를 찾는다. 
		BOOL emptyswitch = FALSE;
		
		// 수상 캐릭터이면, 
		if(KI[sikind].IsKindAtb(KINDATB_WATER))
		{				
			// 생산할 자리를 찾았다. 
			if(FindEmptyAreaWater(tempX, tempY, KI[sikind].IDXsize, KI[sikind].IDYsize)==TRUE)
				emptyswitch=TRUE;
		}
		// 공중 캐릭터이면, 
		else if(KI[sikind].IsKindAtb(KINDATB_FLY))
		{
			// 생산할 자리를 찾았다. 
			if(FindEmptyAreaAir(tempX, tempY, 1, 1)==TRUE)
				emptyswitch=TRUE;
		}
		// 육상 캐릭터이면, 
		else
		{
			// 생산할 자리를 찾았다. 
			if(FindEmptyArea(tempX, tempY, KI[sikind].IDXsize, KI[sikind].IDYsize, FALSE) == TRUE)
				emptyswitch=TRUE;
		}
		
		if( emptyswitch == TRUE )
		{
			// 유닛을 복사할 아이디를 찾는다. 
			SI32 char_id = FindEmptyID(GetPlayerNumber());
			
			if(char_id)
			{
				// 캐릭터를 생산한다. 
				SetChar(sikind, char_id, tempX, tempY, SOUTH, 100, GetPlayerNumber());
				CR[char_id].SetHealDraw(TRUE);
				CR[char_id].CharDrawInfo.SetTransparentSwitch(TRUE);
				CR[char_id].m_siTimerDelay = 10;
			}
			
			PushEffect(EFFECT_MAGIC8J, m_ssX, m_ssY);
			CR[id.ID].SetStealExpDraw(TRUE);
			
			// 마법력을 줄인다. 
			m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_MAGIC8J);
			if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);
			
		}
*/		
		break;
	case SPECIAL_MAGIC4C:						//기공신포.

		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
		{
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		
		::SetMagic4c(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_MAGIC4C, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);	//min(100, m_ParmeterBox.GetHitRate(CR[id.ID].m_ParmeterBox.GetDR()) * 2)
		break;
	case SPECIAL_ICE:
		{
			::SetMagicIce(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_MAGIC4C, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
		}
		break;
	//마법 테스트 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case SPECIAL_MAGICTEST:
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
		{
			PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}

		::SetMagicTest(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, 10000, 100, id);
		break;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	case SPECIAL_KIMBOMB:
		SetKimBomb(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_KIMBOMB, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), ZeroID);
		break;

	case SPECIAL_MAGIC5J:
		SetMagic5j(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_MAGIC5J, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), ZeroID);
		break;

	case SPECIAL_EXTINGUISH:
		// 완전히 어두운 곳에 있지 않아야 소리가 난다. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			PushEffect(EFFECT_EXTINGUISH, m_ssX, m_ssY); 

		SetMagicExtinguish(m_ssX, m_ssY);
		break;

	case SPECIAL_MINE:
		break;

	case SPECIAL_TORNADO:
		 // 적에게 타격을 준다. 
		 level = m_ParmeterBox.IP.GetLevel();

		 // 풍백술을 일으켜 적을 공격한다. 
		 smokeid = FindEmptySmoke();
		 if(smokeid)
		 {
			 PushEffect(EFFECT_TORNADO, CR[id.ID].GetX(), CR[id.ID].GetY() );

			 SI32 tempx = m_cltCharStatus.GetCenterX();
			 SI32 tempy = m_cltCharStatus.GetCenterY();
			 SetSmoke(KIND_SMOKE_TORNADO, smokeid, tempx, tempy, AREA_LAND);

			 // hojae_repair
			 Smoke_PlayerNumber(smokeid, GetPlayerNumber());
			 Smoke_Apower(smokeid, CalcDamage(SPECIAL_TORNADO, m_ParmeterBox.CalcApower(), m_scMagicApplyRate) );
			 Smoke_AttackID(smokeid, id);
			 Smoke_AttackerID(smokeid, m_NewID);
		 }
		 break;
		 ////////////////////////////////
		 ////////////////////////////////
		 ////////////////////////////////
		 ////////////////////////////////
		 ////////////////////////////////
		 ////////////////////////////////
	case SPECIAL_YELLOWSAND:
		// 적에게 타격을 준다. 
		 level = m_ParmeterBox.IP.GetLevel();

		 // 황사을 일으켜 적을 공격한다. 
		 smokeid = FindEmptySmoke();
		 if(smokeid)
		 {
			 PushEffect(EFFECT_TORNADO, CR[id.ID].GetX(), CR[id.ID].GetY() );

			 SI32 tempx = m_cltCharStatus.GetCenterX();
			 SI32 tempy = m_cltCharStatus.GetCenterY();
			 SetSmoke(KIND_SMOKE_SMALL_TORNADO, smokeid, tempx, tempy  , AREA_LAND);

			 // hojae_repair
			 Smoke_PlayerNumber(smokeid, GetPlayerNumber());
			 Smoke_Apower(smokeid, CalcDamage(SPECIAL_YELLOWSAND, m_ParmeterBox.CalcApower(), m_scMagicApplyRate) );
			 Smoke_AttackID(smokeid, id);
			 Smoke_AttackerID(smokeid, m_NewID);
		 }
		 break;

	case SPECIAL_MAGIC4T:

		 smokeid = FindEmptySmoke();
		 if(smokeid)
		 {
			 PushEffect(EFFECT_TORNADO, CR[id.ID].GetX(), CR[id.ID].GetY() );

			 SI32 tempx = m_cltCharStatus.GetCenterX();
			 SI32 tempy = m_cltCharStatus.GetCenterY();
			 SetSmoke(KIND_SMOKE_MAGIC4T, smokeid, tempx, tempy, AREA_LAND);

			 // hojae_repair
			 Smoke_PlayerNumber(smokeid, GetPlayerNumber());
			 Smoke_Apower(smokeid, CalcDamage(SPECIAL_MAGIC4T, m_ParmeterBox.CalcApower(), m_scMagicApplyRate) );
			 Smoke_AttackID(smokeid, id);
			 Smoke_AttackerID(smokeid, m_NewID);
		 }
		 break;

	default:
		clGrp.Error("fdsf933j", "fsdf93h2e2:%d", siAttackAtb);
		break;
		
	}

	return 0;
}

//----------------------------------------------------------------------------
// robypark (2004/05/18 16:53) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
// 함수명: CreateBullet_SpecialNOVA
// 파라미터:
//			SI32 siX[in]: 기술을 사용하는 캐릭터 티일 위치(X)
//			SI32 siY[in]: 기술을 사용하는 캐릭터 타일 위치(Y)
//			_NewID OwnerID[in]: 기술을 사용하는 캐릭터 ID
// 설명: 굉뇌(조선-뇌전차, 허준 2차 전직): 16방향으로 번개를 발사한다.
void cltCharOrg::CreateBullet_SpecialNOVA(SI32 siX, SI32 siY, _NewID OwnerID)
{
	SI32 siBulletID		 = 0;
	SI32 siI			 = 0;
	SI32 siDamage		 = CalcDamage(SPECIAL_NOVA, m_ParmeterBox.CalcApower(), m_scMagicApplyRate);
	SI32 siHitRate		 = m_ParmeterBox.GetHitRate(0);
	SI32 siTargetX		 = 0;
	SI32 siTargetY		 = 0;
	SI32 siTileCntX		 = Map.GetXsize();
	SI32 siTileCntY		 = Map.GetYsize();
	SI32 siDelta_X[4]	 = {0, 0, -1, 1};
	SI32 siDelta_Y[4]	 = {-1, 1, 0, 0};
//	float m[14]			 = {0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f,
//						   -0.25f, -0.5f, -0.75f, -1.0f, -1.25f, -1.5f, -1.75};
	float m[6]			 = {1.0f, 0.5f, 1.25f, -1.0f, -0.5f, -1.5f};

	// N, S, W, E 네 방향 총알 생성
	for (siI = 0; siI < 4; siI++)
	{
		siTargetX = 0;
		siTargetY = 0;

		siBulletID = FindEmptyBullet();

		// 총알이 발사될 위치를 방향에 따라 생성
		CreateTarget_byLinear(siX, siY, siDelta_X[siI], siDelta_Y[siI], 0, siTargetX, siTargetY);

		// 총알의 목표 위치가 캐릭터 위치와 동일하다면 생성하지 않는다.
		if ((siX == siTargetX) && (siY == siTargetY))
			continue;

		// 총알의 목표 위치가 맵 범위 밖에 생성되었다면 생성하지 않는다. (X 좌표)
		if ((siTargetX < 0) || (siTargetX >= siTileCntX))
			continue;

		// 총알의 목표 위치가 맵 범위 밖에 생성되었다면 생성하지 않는다. (Y 좌표)
		if ((siTargetY < 0) || (siTargetY >= siTileCntY))
			continue;

		// 총알 생성
		SetBullet(KIND_BULLET_THUNDER_BOLT, siBulletID, siX, siY, siTargetX, siTargetY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				MapCenterX[siTargetX][siTargetY], MapCenterY[siTargetX][siTargetY],\
				OwnerID, ZeroID, \
				siDamage, siHitRate,\
				0, AttackArea);

		// 직선 진행 총알이라는 것을 명시
		Bullet_CurrentOrder(siBulletID, ORDER_BULLET_STRAIGHT);

		// 총알 발사에 시차를 두기 위한 작업
		Bullet_SetLuanchDelay(siBulletID, 3);
		Bullet_DrawSwitch(siBulletID, FALSE);
	}

	// NW, NNW, NNE, NE, EEN, EES, SE, SSE, SSW, SW, WWS, WWN 방향으로 12개 총알 생성
	for (siI = 0; siI < 6; siI++)
	{
		{
			for (SI32 siJ = 0; siJ < 2; siJ++)
			{
				siTargetX = 0;
				siTargetY = 0;

				siBulletID = FindEmptyBullet();
	
				// 총알이 발사될 위치를 방향에 따라 생성
				CreateTarget_byLinear(siX, siY, 1, siDelta_Y[siJ], m[siI], siTargetX, siTargetY);

				// 총알의 목표 위치가 캐릭터 위치와 동일하다면 생성하지 않는다.
				if ((siX == siTargetX) && (siY == siTargetY))
					continue;

				// 총알의 목표 위치가 맵 범위 밖에 생성되었다면 생성하지 않는다. (X 좌표)
				if ((siTargetX < 0) || (siTargetX >= siTileCntX))
					continue;

				// 총알의 목표 위치가 맵 범위 밖에 생성되었다면 생성하지 않는다. (Y 좌표)
				if ((siTargetY < 0) || (siTargetY >= siTileCntY))
					continue;

				// 총알 생성
				SetBullet(KIND_BULLET_THUNDER_BOLT, siBulletID, siX, siY, siTargetX, siTargetY, \
						m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
						MapCenterX[siTargetX][siTargetY], MapCenterY[siTargetX][siTargetY],\
						OwnerID, ZeroID, \
						siDamage, siHitRate,\
						0, AttackArea);

				// 직선 진행 총알이라는 것을 명시
				Bullet_CurrentOrder(siBulletID, ORDER_BULLET_STRAIGHT);

				// 총알 발사에 시차를 두기 위한 작업
				Bullet_SetLuanchDelay(siBulletID, 3);
				Bullet_DrawSwitch(siBulletID, FALSE);
			}
		}
	}
}


//----------------------------------------------------------------------------
// robypark (2004/05/19 15:05) : 추가 기술 - 장수 2차 전직 특수 기술 관련,
//								 맹격화룡파를 위한 함수
//----------------------------------------------------------------------------
// 함수명: GetDirection_ONSLAUGHT_FIREBALL
// Parameter
//			SI32 siX[in]: 기술을 사용하는 캐릭터 타일 위치(X)
//			SI32 siY[in]: 기술을 사용하는 캐릭터 타일 위치(Y)
//			SI32 siTargetX[in]: 목표 타일 위치(X)
//			SI32 siTargetY[in]: 목표 타일 위치(Y)
//			_NewID OwnerID[in]: 기술을 사용하는 캐릭터 ID
// 설명: 맹격화룡파를 발사한다.
void cltCharOrg::CreateBullet_ONSLAUGHT_FIREBALL(SI32 siX, SI32 siY, SI32 siTargetX, SI32 siTargetY, _NewID OwnerID)
{
	SI32	siBulletID	= 0;

	// 총알의 목표 위치가 캐릭터 위치와 동일하다면 생성하지 않는다.
	if ((siX == siTargetX) && (siY == siTargetY))
		return;

	siBulletID = FindEmptyBullet();

	// 총알이 발사될 위치를 방향에 따라 생성
	CreateTarget_byLinear(siX, siY, siTargetX, siTargetY, siTargetX, siTargetY);

	// 총알의 목표 위치가 캐릭터 위치와 동일하다면 생성하지 않는다.
	if ((siX == siTargetX) && (siY == siTargetY))
		return;

	// 총알의 목표 위치가 맵 범위 밖에 생성되었다면 생성하지 않는다. (X 좌표)
	if ((siTargetX < 0) || (siTargetX >= Map.GetXsize()))
		return;

	// 총알의 목표 위치가 맵 범위 밖에 생성되었다면 생성하지 않는다. (Y 좌표)
	if ((siTargetY < 0) || (siTargetY >= Map.GetYsize()))
		return;

	// 총알 생성
	SetBullet(KIND_BULLET_ONSLAUGHT_FIREBALL, siBulletID, siX, siY, siTargetX, siTargetY, \
			m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
			MapCenterX[siTargetX][siTargetY], MapCenterY[siTargetX][siTargetY],\
			OwnerID, ZeroID, \
			CalcDamage(SPECIAL_ONSLAUGHT_FIREBALL, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0),\
			0, AttackArea);

	// 관통(Piercing) 총알이라는 것을 명시
	Bullet_CurrentOrder(siBulletID, ORDER_BULLET_PIERCE);
}

// robypark 2004-5-29 13:5 최종 수정
// 캐릭터의 좌표와 목표물의 좌표를 입력받아 두 점이 이루는 직선과 맵의 끝 선분과의 교차점을 구해서 돌려준다.
// SI32 x1[in]: 발사하려는 캐릭터 좌표(X)
// SI32 y1[in]: 발사하려는 캐릭터 좌표(Y)
// SI32 x2[in]: 목표의 좌표(X)
// SI32 y2[in]: 목표의 좌표(Y)
// SI32 &siTargetX[out]: 두 좌표가 이루는 직선과 맵 끝 선분과 교차점(X)
// SI32 &siTargetY[out]: 두 좌표가 이루는 직선과 맵 끝 선분과 교차점(Y)
void cltCharOrg::CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 x2, SI32 y2, SI32 &siTargetX, SI32 &siTargetY)
{
	float m = 0.0f;				// 기울기 m
	float b = 0.0f;				// y = mx + b에서 b로 y절편
	float fIntercept_X = 0.0f;	// x 절편
	float fIntercept_Y = 0.0f;	// y 절편
	SI32 siDelta_X, siDelta_Y;
	SI32 siTileCntX		= 0;	// 맵 타일 개수(X)
	SI32 siTileCntY		= 0;	// 맵 타일 개수(Y)

	// 맵의 크기 얻기
	siTileCntX = Map.GetXsize();
	siTileCntY = Map.GetYsize();

	siDelta_X = x2 - x1;
	siDelta_Y = y2 - y1;

	m = (float)siDelta_Y / (float)siDelta_X;	// 기울기 구하기
	b = y2 - (m * x2);							// y = mx + b에서 b값 구하기
	fIntercept_X = ((-1) * b) / m;				// x절편 구하기
	fIntercept_Y = b;							// y절편 구하기

	// x = a인 직선[ax + by + c = 0  ( a ≠ 0 , b ≠ 0 )]
	if (siDelta_X == 0)
	{
		if (y1 > y2)
		{
			siTargetX = x1;
			siTargetY = 0;
		}
		else
		{
			siTargetX = x1;
			siTargetY = siTileCntY - 1;
		}
	}
	// y = b인 직선
	else if (siDelta_Y == 0)
	{
		if (x1 > x2)
		{
			siTargetX = 0;
			siTargetY = y1;
		}
		else
		{
			siTargetX = siTileCntX - 1;
			siTargetY = y1;
		}
	}
	// 1사분면과 3사분면을 지나는 직선
	else if (m > 0)
	{
		// 3사분면을 향하는 직선
		if (y1 > y2)
		{
			if (b < 0)
			{
				siTargetX = fIntercept_X;
				siTargetY = 0;
			}
			else
			{
				siTargetX = 0;
				siTargetY = b;
			}
		}
		// 1사분면을 향하는 직선
		else
		{
			siTargetX = ((siTileCntY - 1) - b) / m;
			siTargetY = siTileCntY - 1;

			if (siTargetX > (siTileCntX - 1))
			{
				siTargetX = siTileCntX - 1;
				siTargetY = ((siTileCntX - 1) * m) + b;
			}
		}
	}
	// 2사분면과 4사분면을 지나는 직선
	else
	{
		// 2사분면을 향하는 직선
		if (y1 < y2)
		{
			if (b < (siTileCntY - 1))
			{
				siTargetX = 0;
				siTargetY = b;
			}
			else
			{
				siTargetX = ((siTileCntY - 1) - b) / m;
				siTargetY = siTileCntY - 1;
			}
		}
		// 4사분면을 향하는 직선
		else
		{
			if (fIntercept_X < (siTileCntX - 1))
			{
				siTargetX = fIntercept_X;
				siTargetY = 0;
			}
			else
			{
				siTargetX = siTileCntX - 1;
				siTargetY = ((siTileCntX - 1) * m) + b;
			}
		}
	}
}

// robypark 2004-5-29 13:15
// 캐릭터의 좌표와 목표물의 좌표를 입력받아 두 점이 이루는 직선과 맵의 끝 선분과의 교차점을 구해서 돌려준다.
// SI32 x1[in]: 발사하려는 캐릭터 좌표(X)
// SI32 y1[in]: 발사하려는 캐릭터 좌표(Y)
// SI32 siDelta_X[in]: 발사하는 캐릭터 좌표와 목표물의 위치 차이(X)
// SI32 siDelta_Y[in]: 발사하는 캐릭터 좌표와 목표물의 위치 차이(Y)
// float m[in]: 기울기 m
// SI32 x2[in]: 목표의 좌표(X)
// SI32 y2[in]: 목표의 좌표(Y)
// SI32 &siTargetX[out]: 점(x1, y1)과 기울기 m이 이루는 직선과 맵 끝 선분과 교차점(X)
// SI32 &siTargetY[out]: 점(x1, y1)과 기울기 m이 이루는 직선과 맵 끝 선분과 교차점(Y)
void cltCharOrg::CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 siDelta_X, SI32 siDelta_Y, float m, SI32 &siTargetX, SI32 &siTargetY)
{
	float b = 0.0f;
	float fIntercept_X = 0.0f;	// x 절편
	float fIntercept_Y = 0.0f;	// y 절편
	SI32 siTileCntX		= 0;	// 맵 타일 개수(X)
	SI32 siTileCntY		= 0;	// 맵 타일 개수(Y)

	// 맵의 크기 얻기
	siTileCntX = Map.GetXsize();
	siTileCntY = Map.GetYsize();

	b = y1 - (m * x1);				// y = mx + b에서 b값 구하기
	fIntercept_X = ((-1) * b) / m;	// x 절편 구하기
	fIntercept_Y = b;				// y 절편 구하기

	// x = a인 직선[ax + by + c = 0  ( a ≠ 0 , b ≠ 0 )]
	if (siDelta_X == 0)
	{
		if (siDelta_Y < 0)
		{
			siTargetX = x1;
			siTargetY = 0;
		}
		else
		{
			siTargetX = x1;
			siTargetY = siTileCntY - 1;
		}
	}
	// y = b인 직선
	else if (siDelta_Y == 0)
	{
		if (siDelta_X < 0)
		{
			siTargetX = 0;
			siTargetY = y1;
		}
		else
		{
			siTargetX = siTileCntX - 1;
			siTargetY = y1;
		}
	}
	// 1사분면과 3사분면을 지나는 직선
	else if (m > 0)
	{
		// 3사분면을 향하는 직선
		if (siDelta_Y < 0)
		{
			if (b < 0)
			{
				siTargetX = fIntercept_X;
				siTargetY = 0;
			}
			else
			{
				siTargetX = 0;
				siTargetY = b;
			}
		}
		// 1사분면을 향하는 직선
		else
		{
			siTargetX = ((siTileCntY - 1) - b) / m;
			siTargetY = siTileCntY - 1;

			if (siTargetX > (siTileCntX - 1))
			{
				siTargetX = siTileCntX - 1;
				siTargetY = ((siTileCntX - 1) * m) + b;
			}
		}
	}
	// 2사분면과 4사분면을 지나는 직선
	else
	{
		// 2사분면을 향하는 직선
		if (siDelta_Y > 0)
		{
			if (b < (siTileCntY - 1))
			{
				siTargetX = 0;
				siTargetY = b;
			}
			else
			{
				siTargetX = ((siTileCntY - 1) - b) / m;
				siTargetY = siTileCntY - 1;
			}
		}
		// 4사분면을 향하는 직선
		else
		{
			if (fIntercept_X < (siTileCntX - 1))
			{
				siTargetX = fIntercept_X;
				siTargetY = 0;
			}
			else
			{
				siTargetX = siTileCntX - 1;
				siTargetY = ((siTileCntX - 1) * m) + b;
			}
		}
	}
} 	
