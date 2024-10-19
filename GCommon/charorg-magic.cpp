//--------------------------------------------------------------------
//  ��� : ���°�
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
extern	_Char				CR[];								// ĳ������ ������ 
extern	_NewID 				ZeroID;
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];


#define SPECIALINFOTYPE_RANGE		1			// ��ȿ�Ÿ�. 
#define SPECIALINFOTYPE_MAGICPOWER	2			// �ʿ� ������. 
#define SPECIALINFOTYPE_ATB3		3			// �ʿ� �Ӽ�.  
#define SPECIALINFOTYPE_ATB4		4			// Atb4

// �Ѱ��� Ư�� ���ݿ� ���� ������ �� ������ �Ǵ°�?
#define SPECIALINFO_DATA_FIELD_NUMBER 5

static SI32 SpecialInfo[]=
{
	// ��� ����,		��ȿ�Ÿ�	������						�ʿ�Ӽ�1		�ʿ�Ӽ�2 
	SPECIAL_MAGIC5J,		0,		MAGICPOWER_MAGIC5J,			ATB3_MAGIC5J,		0,		// ����������.
	SPECIAL_MAGIC8J,		5,		MAGICPOWER_MAGIC8J,			0,					0,		// ȯ����
	SPECIAL_MAGIC4C,		10,		MAGICPOWER_MAGIC4C,			ATB3_MAGIC4,		0,		// ������� 
	SPECIAL_MAGIC6C,		10,		MAGICPOWER_MAGIC6C,			ATB3_MAGIC6C,		0,		// ���ڼ� 
	SPECIAL_ICE,			10,		MAGICPOWER_ICE,				ATB3_ICE,			0,		// �̻��ǻ�� Ư������.
	SPECIAL_AMBUSH,			0,		0,							0,					0,		// �ź� 
	SPECIAL_DETECTMINE,		0, 		MAGICPOWER_DETECTMINE,		0,					0,		// ����Ž�� 
	SPECIAL_EXTINGUISH,		0,		MAGICPOWER_EXTINGUISH,		ATB3_EXTINGUISH,	0,		// ��ȭ��
	SPECIAL_KIMBOMB,		0,		MAGICPOWER_KIMBOMB,			ATB3_KIMBOMB,		0,		// ���ڰ� 
	SPECIAL_GROUNDEXP,		10,		MAGICPOWER_GROUNDEXP,		ATB3_GROUNDEXP,		0,		// ������ 
	SPECIAL_TORNADO,		10,		MAGICPOWER_TORNADO,			ATB3_TORNADO,		0,		// ǳ���. 
	SPECIAL_MAGIC4T,		10,		MAGICPOWER_MAGIC4T,			ATB3_MAGIC4T,		0,		// ���ɺм�� => ��������
	SPECIAL_MAGIC2T,		10,		MAGICPOWER_MAGIC2T,			ATB3_POISONBULLET,	0,		// ������. 
	/////////////////////////////////////////////////////////////////////////////////
	SPECIAL_MAGICTEST,		10,		MAGICPOWER_MAGICTEST,		ATB3_MAGICTEST,		0,		//���� �׽�Ʈ.
	/////////////////////////////////////////////////////////////////////////////////
	SPECIAL_MINE,			0,		MAGICPOWER_MINE		,		ATB3_MINE,			0,		// ����.
	SPECIAL_YELLOWSAND,		10,		MAGICPOWER_YELLOWSAND,		ATB3_YELLOWSAND,	0,		// Ȳ��.
	SPECIAL_SOUL,			0,		MAGICPOWER_SOUL,			ATB3_SOUL,			0,		// �հ��� �߼��� ȥ.
	SPECIAL_GHOST,			10,		MAGICPOWER_GHOST,			ATB3_GHOST,			0,		// �丣������ �ͽ�ź.
	SPECIAL_DARKFIRE,		10,		MAGICPOWER_DARKFIRE,		ATB3_DARKFIRE,		0,		// ����ȭ����ǳ.
	SPECIAL_IVORY,			10,		MAGICPOWER_IVORY,			0,					ATB4_IVORY,				//�������.
	SPECIAL_BLUEDRAGON,		0,		MAGICPOWER_BLUEDRAGON,		0,					ATB4_BLUEDRAGON,		//���û�浵.
	SPECIAL_RUNNINGFIRE,	10,		MAGICPOWER_RUNNINGFIRE,		0,					ATB4_RUNNINGFIRE,		//������ź.
	SPECIAL_ENERGYPA,		10,		MAGICPOWER_ENERGYPA,		0,					ATB4_ENERGYPA,			//��ǳ.
	SPECIAL_SNOWATTACK,		10,		MAGICPOWER_SNOWATTACK,		0,					ATB4_SNOWATTACK,		//��������.
	SPECIAL_FIREATTACK,		10,		MAGICPOWER_FIREATTACK,		0,					ATB4_FIREATTACK,		//ȭ������.
	SPECIAL_FIREENERGYPA,	10,		MAGICPOWER_FIREENERGYPA,	0,					ATB4_FIREENERGYPA,		//��ȭ��ǳ(���Ȳ���).
	SPECIAL_DOUBLE_SWORD,	0,		MAGICPOWER_DOUBLE_SWORD,	0,					ATB4_DOUBLE_SWORD,		//�ְ˳���.
	SPECIAL_LONG_SPEAR,		10,		MAGICPOWER_LONG_SPEAR,		0,					ATB4_LONG_SPEAR,		//��â��ȯ.
	SPECIAL_ICE_BARRIER,	18,		MAGICPOWER_ICE_BARRIER,		0,					ATB4_ICE_BARRIER,		//������.
	SPECIAL_SOUL_CHANGE,	10,		MAGICPOWER_SOUL_CHANGE,		0,					ATB4_SOUL_CHANGE,		//���Ǽ�.
	SPECIAL_FLOWER_NEEDLE_RAIN,10,	MAGICPOWER_FLOWER_NEEDLE_RAIN,0,				ATB4_FLOWER_NEEDLE_RAIN,//��õȭ��
	SPECIAL_STONE_MAGIC,	10,		MAGICPOWER_STONE_MAGIC,		0,					ATB4_STONE_MAGIC,		//������
	SPECIAL_GOD_SPEAR,		5,		MAGICPOWER_GOD_SPEAR,		0,					ATB4_GOD_SPEAR,			//��â�뵵
	SPECIAL_CLOUD_WIND,		10,		MAGICPOWER_CLOUD_WIND,		0,					ATB4_CLOUD_WIND,		//ǳ���
	SPECIAL_MANABURN,		10,		MAGICPOWER_MANABURN,		0,					ATB4_MANABURN,			//������.

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:48) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	// ��� ����,					��ȿ�Ÿ�		�ʿ� ������						�ʿ�Ӽ�1		�ʿ�Ӽ�2 
	SPECIAL_FIRE_ROUND_DANCE,			0,		MAGICPOWER_FIRE_ROUND_DANCE,			0,		ATB4_FIRE_ROUND_DANCE,			// ����������(����-�ź���, �̼��� 2�� ���� ���)
	SPECIAL_NOVA,						0,		MAGICPOWER_NOVA,						0,		ATB4_NOVA,						// ����(����-������, ���� 2�� ���� ���)
	SPECIAL_FURY_OF_VOLCANO,		    8,		MAGICPOWER_FURY_OF_VOLCANO,				0,		ATB4_FURY_OF_VOLCANO,			// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
	SPECIAL_POISONING_HEAVENS,			0,		MAGICPOWER_POISONING_HEAVENS,			0,		ATB4_POISONING_HEAVENS,			// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
	SPECIAL_BOMB_OF_HEAVEN,			   12,		MAGICPOWER_BOMB_OF_HEAVEN,				0,		ATB4_BOMB_OF_HEAVEN,			// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
	SPECIAL_SIEGE_MODE,					0,		MAGICPOWER_SIEGE_MODE,					0,		ATB4_SIEGE_MODE,				// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
	SPECIAL_ONSLAUGHT_FIREBALL,		   10,		MAGICPOWER_ONSLAUGHT_FIREBALL,			0,		ATB4_ONSLAUGHT_FIREBALL,		// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
	SPECIAL_FIRERAIN_OF_GOD,			0,		MAGICPOWER_FIRERAIN_OF_GOD,				0,		ATB4_FIRERAIN_OF_GOD,			// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
	// ��� ����,					��ȿ�Ÿ�		�ʿ� ������						�ʿ�Ӽ�1		�ʿ�Ӽ�2 
	-1
};


// Ư�� ����� ������ ��´�. 
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

	// ȭ��� ��ȿ�� ��쿡�� �۵��Ѵ�. 
	if(m_scSeeSwitch==FALSE)return FALSE;


	SetAnimation(ANI_AMBUSH);
	DoAnimationDelay(TRUE);


	/*
	Animation = ANI_AMBUSH;

	// ������ �ִϸ��̼� 
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
// Description	:	����Ž��
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

		
		// ������ ���ؼ� ������ ������ �ʵ��� ZeroID�� �ִ´�. 
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

	// Ư�� ��� ���� ���� .
	SI32 range	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_RANGE);
	SI32 magic	= GetSpecialInfo(siSpecial, SPECIALINFOTYPE_MAGICPOWER);

	// ��ǥ�� ��� ĳ����  
	targetid = Para2;

	// ��ǥ ��ġ. 
	x = LOWORD(Para1);
	y = HIWORD(Para1);


	if( siSpecial )
	{
		length=0;
		
		switch(WorkStep)
		{
		case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 
			
			if(CanSpecial(siSpecial, targetid)==FALSE)
			{   
				m_scBusySwitch=FALSE;
				OrderStop(m_NewID, BY_COMPUTER);
				return FALSE;
			}
			
			// �����Ÿ� �ȿ� ������,
			if(max(abs(x - m_ssX), abs(y - m_ssY) ) < range )
			{ 
				//Direction = DecideDirection8(m_ssX, m_ssY, x, y);
				WorkStep= WS_ATTACK;
			}
			else // ���� ��Ÿ� �ۿ� ������, 
			{  
				WorkStep=WS_GOTO_TARGET;
				
				// ���� �ִ� ������ �̵��Ѵ�. 
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
				
				// �������� �����ϰų� �߰� ������ �����ϸ�, 
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
		
		
		// �������� �ٿ��ش�. 
		if( m_ParmeterBox.IP.GetMana() >= magic )
		{
			// �������� �ٿ��ش�. 
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
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
		{
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		//temp
		::SetMagicIvory(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_IVORY, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);	//min(100, m_ParmeterBox.GetHitRate(CR[id.ID].m_ParmeterBox.GetDR()) * 2)

	break;

	case SPECIAL_BLUEDRAGON:
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	//������ź.
	case SPECIAL_RUNNINGFIRE:
		bulletid = FindEmptyBullet();
		if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
		{
			  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		SetBullet(KIND_BULLET_RUNNINGFIRE, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, AttackID, CalcDamage(SPECIAL_RUNNINGFIRE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
	break;
	//��ǳ
	case SPECIAL_ENERGYPA:
		bulletid = FindEmptyBullet();
		SetBullet(KIND_BULLET_ENERGYPA, bulletid, m_ssX, m_ssY, x, y, m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), MapCenterX[x][y], MapCenterY[x][y], m_NewID, ZeroID, CalcDamage(SPECIAL_ENERGYPA, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);
		if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
		{
			  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		::SetMagicEnergyPa(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_ENERGYPA, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);

	break;
	//��ȭ��ǳ(���Ȳ���).
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
	case SPECIAL_DOUBLE_SWORD:					// �ְ˳���
		if( GD.IsInBlackFog(m_ssX, m_ssY) == FALSE )
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		::SetMagicDoubleSword(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_DOUBLE_SWORD, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
		break;
	case SPECIAL_ICE_BARRIER:					// ������
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicIceBarrier(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_ICE_BARRIER, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_FLOWER_NEEDLE_RAIN:			// ��õȭ��
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicFlowerNeedleRain(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_FLOWER_NEEDLE_RAIN, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_SOUL_CHANGE:					// ���Ǽ�
		bulletid = FindEmptyBullet();
		if(GD.IsInBlackFog(m_ssX,m_ssY) == FALSE)
		{
			  PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		::SetMagicSoulChange(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_SOUL_CHANGE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
		break;
	case SPECIAL_STONE_MAGIC:					// ������
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicStoneMagic(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_STONE_MAGIC, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_GOD_SPEAR:						// ��â�뵵
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicGodSpear(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_GOD_SPEAR, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	case SPECIAL_CLOUD_WIND:					// ǳ���
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagicCloudWind(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_CLOUD_WIND, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;

	//----------------------------------------------------------------------------
	// robypark (2004/05/17 15:48) : �߰� ��� - ��� 2�� ���� Ư�� ���
	//----------------------------------------------------------------------------

	// ����������(����-�ź���, �̼��� 2�� ���� ���)
	case SPECIAL_FIRE_ROUND_DANCE:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_FIRE_ROUND_DANCE(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_FIRE_ROUND_DANCE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;
	
	// ����(����-������, ���� 2�� ���� ���)
	case SPECIAL_NOVA:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			PushEffect(EFFECT_THUNDER, m_ssX, m_ssY);

			// ������ �Ӹ� ���� ���� ����ġ�� smoke ����
			SI32 smokeid = FindEmptySmoke();
			if(smokeid)
				SetSmoke(KIND_SMOKE_LIGHTNING, smokeid, MapCenterX[m_ssX][m_ssY], MapCenterY[m_ssX][m_ssY] - 70, AREA_AIR);

			// 16�������� �Ѿ� ����
			CreateBullet_SpecialNOVA(m_ssX, m_ssY, m_NewID);
		}
		break;

	// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
	case SPECIAL_FURY_OF_VOLCANO:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_FURY_OF_VOLCANO(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, m_ParmeterBox.CalcApower(), 100, id);
			 PushEffect(EFFECT_MAGIC4C, x, y);
		}
		break;

	// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
	case SPECIAL_POISONING_HEAVENS:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			::SetMagic_POISONING_HEAVENS(m_ssX, m_ssY, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_POISONING_HEAVENS, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), 100, id);
			 PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
		}
		break;

	// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
	case SPECIAL_BOMB_OF_HEAVEN:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			// ��ǥ�� �������� ĳ���Ͱ� �ٶ󺸵��� �Ѵ�.
			ChangeDirection(DecideDirection8(m_ssX, m_ssY, x, y));

//			::SetMagic_BOMB_OF_HEAVEN(x, y, m_NewID, GetPlayerNumber(), AREA_LAND, CalcDamage(SPECIAL_BOMB_OF_HEAVEN, m_ParmeterBox.CalcApower(), MagicApplyRate), 100, id);
			PushEffect(EFFECT_MAGIC4C, m_ssX, m_ssY);
			SI32 siBulletID = FindEmptyBullet();
			// �Ѿ� ����
			SetBullet(KIND_BULLET_BOMB_OF_HEAVEN, siBulletID, m_ssX, m_ssY, x, y, \
							m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY() - 40, \
							MapCenterX[x][y], MapCenterY[x][y],\
							m_NewID, ZeroID, \
							CalcDamage(SPECIAL_BOMB_OF_HEAVEN, m_ParmeterBox.CalcApower(), m_scMagicApplyRate),
							m_ParmeterBox.GetHitRate(0), TargetDirection, AttackArea);

			// ź�� ���� ��õ���� ���� ����� ���ȴٴ� ���� ���
			// ��, �̹��� �׸��� �� ������ ���, ���� ���� ó���Ѵٴ� �ǹ�
			Bullet_CurrentOrder(siBulletID, ORDER_BULLET_EVENT_IMPACT);
		}
		break;

	// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
	case SPECIAL_SIEGE_MODE:
		break;
	
	// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
	case SPECIAL_ONSLAUGHT_FIREBALL:
		if(Map.IsInMapArea(x, y) == TRUE && GD.IsInBlackFog(m_ssX, m_ssY) == FALSE)
		{
			// ��ǥ�� �������� ĳ���Ͱ� �ٶ󺸵��� �Ѵ�.
			ChangeDirection(DecideDirection8(m_ssX, m_ssY, x, y));

			// �Ͱ�ȭ���� �Ѿ� ����
			CreateBullet_ONSLAUGHT_FIREBALL(m_ssX, m_ssY, x, y, m_NewID);
		}
		break;
		
	// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
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
// Description	:	�ʻ�� ����
BOOL cltCharOrg::Special(SI32 siAttackAtb)
{
	SHORT	status;
	SHORT	length;
	_NewID	targetid;
	SHORT	x, y;

	// Ư�� ��� ���� ���� .
	SI32 range	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_RANGE);
	SI32 magic	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_MAGICPOWER);

	// ��ǥ�� ��� ĳ����  
	targetid = Para2;

	// ��ǥ ��ġ. 
	x = LOWORD(Para1);
	y = HIWORD(Para1);


	switch( siAttackAtb )
	{

		// �ź�
	case SPECIAL_AMBUSH:
		return Ambush();
		
		// ����Ž��
	case SPECIAL_DETECTMINE:
		DetectMine();
		break;
		
	default:
		length=0;
		
		switch(WorkStep)
		{
		case WS_DECIDE_LENGTH_TO_DEST:    // ��ǥ������ �Ÿ��� �����Ѵ�. 
			
			if(CanSpecial(siAttackAtb, targetid)==FALSE)
			{   
				m_scBusySwitch=FALSE;
				OrderStop(m_NewID, BY_COMPUTER);
				return FALSE;
			}
			
			// �����Ÿ� �ȿ� ������,
			if(max(abs(x - m_ssX), abs(y - m_ssY) ) < range )
			{ 
				//Direction = DecideDirection8(m_ssX, m_ssY, x, y);
				WorkStep=WS_HEAL;
			}
			else // ���� ��Ÿ� �ۿ� ������, 
			{  
				WorkStep=WS_GOTO_TARGET;
				
				// ���� �ִ� ������ �̵��Ѵ�. 
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
				
				// �������� �����ϰų� �߰� ������ �����ϸ�, 
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


	// ������ ������ �ʿ��� ���̶��, 
	if(magic)
	{
		// �������� ���� �־�� �Ѵ�. 
		if(m_ParmeterBox.IP.GetMana() < magic)return FALSE;
	}

	// ������ �ʿ��� �Ӽ��� �ִ� ���̶��, 
	if(atb4)
	{
		// �Ӽ��� �ִ��� Ȯ���Ѵ�.
		if(IsAtb4(atb4) == FALSE)return FALSE;
	}

	switch(siSpecial)
	{
	// ������
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

	// ��ȿ�Ÿ��� �ִ� ����� ���� ��� �������� �Ÿ��� ����ؾ� �Ѵ�.
	if(range)
	{
		if(CanSpecialAtb4(siSpecial, id) == FALSE)return FALSE;
		WorkStep = WS_DECIDE_LENGTH_TO_DEST;
		AttackGiveUpDelayor = 0;
	}
	// ��ȿ�Ÿ��� ���� ����� �ٷ� �� �ڸ����� �����Ѵ�.
	else
	{
		if( CanSpecialAtb4(siSpecial, id) == FALSE )	return FALSE;
		WorkStep = WS_ATTACK;
		AttackGiveUpDelayor = 0;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Description	:	������ �� �� �ִ°�
BOOL cltCharOrg::CanSpecial(SI32 siAttackAtb, _NewID id, SI32 *pReturnVal)
{
	SI32 magic	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_MAGICPOWER);
	SI32 atb3	= GetSpecialInfo(siAttackAtb, SPECIALINFOTYPE_ATB3);


	// ������ ������ �ʿ��� ���̶��, 
	if(magic)
	{
		// �������� ���� �־�� �Ѵ�. 
		if(m_ParmeterBox.IP.GetMana() < magic)return FALSE;
	}

	// ������ �ʿ��� �Ӽ��� �ִ� ���̶��, 
	if(atb3)
	{
		// �Ӽ��� �ִ��� Ȯ���Ѵ�.
		if(IsAtb3(atb3) == FALSE)return FALSE;
	}

	
	switch( siAttackAtb )
	{

	// �������
	case SPECIAL_MAGIC4C:						
		if( IsSee(id) == FALSE )				return FALSE;
		if( CR[id.ID].IsAtb(ATB_FLY) )			return FALSE;
		break;

		// ǳ���. 
	case SPECIAL_TORNADO:
	case SPECIAL_YELLOWSAND:
		// ���� ĳ���ʹ� ������ �� ����. 
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
		
		
		// �������� �ٿ��ش�. 
		if( m_ParmeterBox.IP.GetMana() >= magic ) 
		{
			// �������� �ٿ��ش�. 
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

			  // ��ȿ�� ���� ������ �־�� �Ѵ�. 
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

// ����� �������븦 ã�´�. 
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

			  // ��ȿ�� ���� ������ �־�� �Ѵ�. 
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

	// ��ȿ�Ÿ��� �ִ� ����� ���� ��� �������� �Ÿ��� ����ؾ� �Ѵ�.
	if(range)
	{
		if(CanSpecial(siAttackAtb, id) == FALSE)return FALSE;
		WorkStep = WS_DECIDE_LENGTH_TO_DEST;
		AttackGiveUpDelayor = 0;
	}
	// ��ȿ�Ÿ��� ���� ����� �ٷ� �� �ڸ����� �����Ѵ�.
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
			if(Map.IsInMapArea(x, y) == TRUE)		// m_ssX,m_ssY(���ΰ���ġ) x,y(��ǥ��ġ)
			{
				SetBullet(KIND_BULLET_DARKFIRE, bulletid, m_ssX, m_ssY, x, y, 
					m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), 
					MapCenterX[x][y], MapCenterY[x][y], 
					m_NewID, ZeroID, CalcDamage(SPECIAL_DARKFIRE, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0), 0, AttackArea);

				//����ȭ����ǳ ����.
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
			
			// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
			if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
				PushEffect(EFFECT_MAGIC6C, m_ssX, m_ssY);
			
		}
		break;

	case SPECIAL_MAGIC8J:
/*		// ���� ������ �˾Ƴ���. 
		SI32 sikind = CR[id.ID].m_cltCharStatus.GetKind();
		
		// ���ڸ��� ã�´�. 
		BOOL emptyswitch = FALSE;
		
		// ���� ĳ�����̸�, 
		if(KI[sikind].IsKindAtb(KINDATB_WATER))
		{				
			// ������ �ڸ��� ã�Ҵ�. 
			if(FindEmptyAreaWater(tempX, tempY, KI[sikind].IDXsize, KI[sikind].IDYsize)==TRUE)
				emptyswitch=TRUE;
		}
		// ���� ĳ�����̸�, 
		else if(KI[sikind].IsKindAtb(KINDATB_FLY))
		{
			// ������ �ڸ��� ã�Ҵ�. 
			if(FindEmptyAreaAir(tempX, tempY, 1, 1)==TRUE)
				emptyswitch=TRUE;
		}
		// ���� ĳ�����̸�, 
		else
		{
			// ������ �ڸ��� ã�Ҵ�. 
			if(FindEmptyArea(tempX, tempY, KI[sikind].IDXsize, KI[sikind].IDYsize, FALSE) == TRUE)
				emptyswitch=TRUE;
		}
		
		if( emptyswitch == TRUE )
		{
			// ������ ������ ���̵� ã�´�. 
			SI32 char_id = FindEmptyID(GetPlayerNumber());
			
			if(char_id)
			{
				// ĳ���͸� �����Ѵ�. 
				SetChar(sikind, char_id, tempX, tempY, SOUTH, 100, GetPlayerNumber());
				CR[char_id].SetHealDraw(TRUE);
				CR[char_id].CharDrawInfo.SetTransparentSwitch(TRUE);
				CR[char_id].m_siTimerDelay = 10;
			}
			
			PushEffect(EFFECT_MAGIC8J, m_ssX, m_ssY);
			CR[id.ID].SetStealExpDraw(TRUE);
			
			// �������� ���δ�. 
			m_ParmeterBox.IP.SetMana(m_ParmeterBox.IP.GetMana() - MAGICPOWER_MAGIC8J);
			if( m_ParmeterBox.IP.GetMana() < 0 )	m_ParmeterBox.IP.SetMana(0);
			
		}
*/		
		break;
	case SPECIAL_MAGIC4C:						//�������.

		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
	//���� �׽�Ʈ 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case SPECIAL_MAGICTEST:
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
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
		// ������ ��ο� ���� ���� �ʾƾ� �Ҹ��� ����. 
		if(GD.IsInBlackFog(m_ssX, m_ssY)==FALSE)
			PushEffect(EFFECT_EXTINGUISH, m_ssX, m_ssY); 

		SetMagicExtinguish(m_ssX, m_ssY);
		break;

	case SPECIAL_MINE:
		break;

	case SPECIAL_TORNADO:
		 // ������ Ÿ���� �ش�. 
		 level = m_ParmeterBox.IP.GetLevel();

		 // ǳ����� ������ ���� �����Ѵ�. 
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
		// ������ Ÿ���� �ش�. 
		 level = m_ParmeterBox.IP.GetLevel();

		 // Ȳ���� ������ ���� �����Ѵ�. 
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
// robypark (2004/05/18 16:53) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
// �Լ���: CreateBullet_SpecialNOVA
// �Ķ����:
//			SI32 siX[in]: ����� ����ϴ� ĳ���� Ƽ�� ��ġ(X)
//			SI32 siY[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(Y)
//			_NewID OwnerID[in]: ����� ����ϴ� ĳ���� ID
// ����: ����(����-������, ���� 2�� ����): 16�������� ������ �߻��Ѵ�.
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

	// N, S, W, E �� ���� �Ѿ� ����
	for (siI = 0; siI < 4; siI++)
	{
		siTargetX = 0;
		siTargetY = 0;

		siBulletID = FindEmptyBullet();

		// �Ѿ��� �߻�� ��ġ�� ���⿡ ���� ����
		CreateTarget_byLinear(siX, siY, siDelta_X[siI], siDelta_Y[siI], 0, siTargetX, siTargetY);

		// �Ѿ��� ��ǥ ��ġ�� ĳ���� ��ġ�� �����ϴٸ� �������� �ʴ´�.
		if ((siX == siTargetX) && (siY == siTargetY))
			continue;

		// �Ѿ��� ��ǥ ��ġ�� �� ���� �ۿ� �����Ǿ��ٸ� �������� �ʴ´�. (X ��ǥ)
		if ((siTargetX < 0) || (siTargetX >= siTileCntX))
			continue;

		// �Ѿ��� ��ǥ ��ġ�� �� ���� �ۿ� �����Ǿ��ٸ� �������� �ʴ´�. (Y ��ǥ)
		if ((siTargetY < 0) || (siTargetY >= siTileCntY))
			continue;

		// �Ѿ� ����
		SetBullet(KIND_BULLET_THUNDER_BOLT, siBulletID, siX, siY, siTargetX, siTargetY, \
				m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
				MapCenterX[siTargetX][siTargetY], MapCenterY[siTargetX][siTargetY],\
				OwnerID, ZeroID, \
				siDamage, siHitRate,\
				0, AttackArea);

		// ���� ���� �Ѿ��̶�� ���� ���
		Bullet_CurrentOrder(siBulletID, ORDER_BULLET_STRAIGHT);

		// �Ѿ� �߻翡 ������ �α� ���� �۾�
		Bullet_SetLuanchDelay(siBulletID, 3);
		Bullet_DrawSwitch(siBulletID, FALSE);
	}

	// NW, NNW, NNE, NE, EEN, EES, SE, SSE, SSW, SW, WWS, WWN �������� 12�� �Ѿ� ����
	for (siI = 0; siI < 6; siI++)
	{
		{
			for (SI32 siJ = 0; siJ < 2; siJ++)
			{
				siTargetX = 0;
				siTargetY = 0;

				siBulletID = FindEmptyBullet();
	
				// �Ѿ��� �߻�� ��ġ�� ���⿡ ���� ����
				CreateTarget_byLinear(siX, siY, 1, siDelta_Y[siJ], m[siI], siTargetX, siTargetY);

				// �Ѿ��� ��ǥ ��ġ�� ĳ���� ��ġ�� �����ϴٸ� �������� �ʴ´�.
				if ((siX == siTargetX) && (siY == siTargetY))
					continue;

				// �Ѿ��� ��ǥ ��ġ�� �� ���� �ۿ� �����Ǿ��ٸ� �������� �ʴ´�. (X ��ǥ)
				if ((siTargetX < 0) || (siTargetX >= siTileCntX))
					continue;

				// �Ѿ��� ��ǥ ��ġ�� �� ���� �ۿ� �����Ǿ��ٸ� �������� �ʴ´�. (Y ��ǥ)
				if ((siTargetY < 0) || (siTargetY >= siTileCntY))
					continue;

				// �Ѿ� ����
				SetBullet(KIND_BULLET_THUNDER_BOLT, siBulletID, siX, siY, siTargetX, siTargetY, \
						m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
						MapCenterX[siTargetX][siTargetY], MapCenterY[siTargetX][siTargetY],\
						OwnerID, ZeroID, \
						siDamage, siHitRate,\
						0, AttackArea);

				// ���� ���� �Ѿ��̶�� ���� ���
				Bullet_CurrentOrder(siBulletID, ORDER_BULLET_STRAIGHT);

				// �Ѿ� �߻翡 ������ �α� ���� �۾�
				Bullet_SetLuanchDelay(siBulletID, 3);
				Bullet_DrawSwitch(siBulletID, FALSE);
			}
		}
	}
}


//----------------------------------------------------------------------------
// robypark (2004/05/19 15:05) : �߰� ��� - ��� 2�� ���� Ư�� ��� ����,
//								 �Ͱ�ȭ���ĸ� ���� �Լ�
//----------------------------------------------------------------------------
// �Լ���: GetDirection_ONSLAUGHT_FIREBALL
// Parameter
//			SI32 siX[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(X)
//			SI32 siY[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(Y)
//			SI32 siTargetX[in]: ��ǥ Ÿ�� ��ġ(X)
//			SI32 siTargetY[in]: ��ǥ Ÿ�� ��ġ(Y)
//			_NewID OwnerID[in]: ����� ����ϴ� ĳ���� ID
// ����: �Ͱ�ȭ���ĸ� �߻��Ѵ�.
void cltCharOrg::CreateBullet_ONSLAUGHT_FIREBALL(SI32 siX, SI32 siY, SI32 siTargetX, SI32 siTargetY, _NewID OwnerID)
{
	SI32	siBulletID	= 0;

	// �Ѿ��� ��ǥ ��ġ�� ĳ���� ��ġ�� �����ϴٸ� �������� �ʴ´�.
	if ((siX == siTargetX) && (siY == siTargetY))
		return;

	siBulletID = FindEmptyBullet();

	// �Ѿ��� �߻�� ��ġ�� ���⿡ ���� ����
	CreateTarget_byLinear(siX, siY, siTargetX, siTargetY, siTargetX, siTargetY);

	// �Ѿ��� ��ǥ ��ġ�� ĳ���� ��ġ�� �����ϴٸ� �������� �ʴ´�.
	if ((siX == siTargetX) && (siY == siTargetY))
		return;

	// �Ѿ��� ��ǥ ��ġ�� �� ���� �ۿ� �����Ǿ��ٸ� �������� �ʴ´�. (X ��ǥ)
	if ((siTargetX < 0) || (siTargetX >= Map.GetXsize()))
		return;

	// �Ѿ��� ��ǥ ��ġ�� �� ���� �ۿ� �����Ǿ��ٸ� �������� �ʴ´�. (Y ��ǥ)
	if ((siTargetY < 0) || (siTargetY >= Map.GetYsize()))
		return;

	// �Ѿ� ����
	SetBullet(KIND_BULLET_ONSLAUGHT_FIREBALL, siBulletID, siX, siY, siTargetX, siTargetY, \
			m_cltCharStatus.GetCenterX(), m_cltCharStatus.GetCenterY(), \
			MapCenterX[siTargetX][siTargetY], MapCenterY[siTargetX][siTargetY],\
			OwnerID, ZeroID, \
			CalcDamage(SPECIAL_ONSLAUGHT_FIREBALL, m_ParmeterBox.CalcApower(), m_scMagicApplyRate), m_ParmeterBox.GetHitRate(0),\
			0, AttackArea);

	// ����(Piercing) �Ѿ��̶�� ���� ���
	Bullet_CurrentOrder(siBulletID, ORDER_BULLET_PIERCE);
}

// robypark 2004-5-29 13:5 ���� ����
// ĳ������ ��ǥ�� ��ǥ���� ��ǥ�� �Է¹޾� �� ���� �̷�� ������ ���� �� ���а��� �������� ���ؼ� �����ش�.
// SI32 x1[in]: �߻��Ϸ��� ĳ���� ��ǥ(X)
// SI32 y1[in]: �߻��Ϸ��� ĳ���� ��ǥ(Y)
// SI32 x2[in]: ��ǥ�� ��ǥ(X)
// SI32 y2[in]: ��ǥ�� ��ǥ(Y)
// SI32 &siTargetX[out]: �� ��ǥ�� �̷�� ������ �� �� ���а� ������(X)
// SI32 &siTargetY[out]: �� ��ǥ�� �̷�� ������ �� �� ���а� ������(Y)
void cltCharOrg::CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 x2, SI32 y2, SI32 &siTargetX, SI32 &siTargetY)
{
	float m = 0.0f;				// ���� m
	float b = 0.0f;				// y = mx + b���� b�� y����
	float fIntercept_X = 0.0f;	// x ����
	float fIntercept_Y = 0.0f;	// y ����
	SI32 siDelta_X, siDelta_Y;
	SI32 siTileCntX		= 0;	// �� Ÿ�� ����(X)
	SI32 siTileCntY		= 0;	// �� Ÿ�� ����(Y)

	// ���� ũ�� ���
	siTileCntX = Map.GetXsize();
	siTileCntY = Map.GetYsize();

	siDelta_X = x2 - x1;
	siDelta_Y = y2 - y1;

	m = (float)siDelta_Y / (float)siDelta_X;	// ���� ���ϱ�
	b = y2 - (m * x2);							// y = mx + b���� b�� ���ϱ�
	fIntercept_X = ((-1) * b) / m;				// x���� ���ϱ�
	fIntercept_Y = b;							// y���� ���ϱ�

	// x = a�� ����[ax + by + c = 0  ( a �� 0 , b �� 0 )]
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
	// y = b�� ����
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
	// 1��и�� 3��и��� ������ ����
	else if (m > 0)
	{
		// 3��и��� ���ϴ� ����
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
		// 1��и��� ���ϴ� ����
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
	// 2��и�� 4��и��� ������ ����
	else
	{
		// 2��и��� ���ϴ� ����
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
		// 4��и��� ���ϴ� ����
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
// ĳ������ ��ǥ�� ��ǥ���� ��ǥ�� �Է¹޾� �� ���� �̷�� ������ ���� �� ���а��� �������� ���ؼ� �����ش�.
// SI32 x1[in]: �߻��Ϸ��� ĳ���� ��ǥ(X)
// SI32 y1[in]: �߻��Ϸ��� ĳ���� ��ǥ(Y)
// SI32 siDelta_X[in]: �߻��ϴ� ĳ���� ��ǥ�� ��ǥ���� ��ġ ����(X)
// SI32 siDelta_Y[in]: �߻��ϴ� ĳ���� ��ǥ�� ��ǥ���� ��ġ ����(Y)
// float m[in]: ���� m
// SI32 x2[in]: ��ǥ�� ��ǥ(X)
// SI32 y2[in]: ��ǥ�� ��ǥ(Y)
// SI32 &siTargetX[out]: ��(x1, y1)�� ���� m�� �̷�� ������ �� �� ���а� ������(X)
// SI32 &siTargetY[out]: ��(x1, y1)�� ���� m�� �̷�� ������ �� �� ���а� ������(Y)
void cltCharOrg::CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 siDelta_X, SI32 siDelta_Y, float m, SI32 &siTargetX, SI32 &siTargetY)
{
	float b = 0.0f;
	float fIntercept_X = 0.0f;	// x ����
	float fIntercept_Y = 0.0f;	// y ����
	SI32 siTileCntX		= 0;	// �� Ÿ�� ����(X)
	SI32 siTileCntY		= 0;	// �� Ÿ�� ����(Y)

	// ���� ũ�� ���
	siTileCntX = Map.GetXsize();
	siTileCntY = Map.GetYsize();

	b = y1 - (m * x1);				// y = mx + b���� b�� ���ϱ�
	fIntercept_X = ((-1) * b) / m;	// x ���� ���ϱ�
	fIntercept_Y = b;				// y ���� ���ϱ�

	// x = a�� ����[ax + by + c = 0  ( a �� 0 , b �� 0 )]
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
	// y = b�� ����
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
	// 1��и�� 3��и��� ������ ����
	else if (m > 0)
	{
		// 3��и��� ���ϴ� ����
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
		// 1��и��� ���ϴ� ����
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
	// 2��и�� 4��и��� ������ ����
	else
	{
		// 2��и��� ���ϴ� ����
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
		// 4��и��� ���ϴ� ����
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
