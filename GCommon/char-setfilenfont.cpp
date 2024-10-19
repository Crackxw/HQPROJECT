//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>

#include "..\Gersang\rain.h"
#include <CharOrg.h>
#include "..\Gersang\CharInfo\CharFileLoadManager.h"

#include <map.h>
#include <MyFile.h>

extern	_KindInfo					KI[];							// 캐릭터 종류별 데이터 
extern	CCharFileLoadManager	g_FileLoadManager;					// 조합형 캐릭터



void cltCharOrg::SetFileNFont()
{
	SI32 i;

	SI32 realimagexsize, realimageysize;

	for(i = 1;i < MAX_ANIMATION_PER_UNIT;i++)
	{
	
		switch(KI[m_cltCharStatus.GetKind()].m_siCharAnimationInfo[i])
		{
		case 0:
			break;

		case ANI_MOVE:
			MoveCharAnimationIndex = i;
			break;
		case ANI_WAIT0:
			Wait0CharAnimationIndex = i;
			break;
		case ANI_WAIT1:
			Wait1CharAnimationIndex = i;
			break;
		case ANI_LOWMOVE:
			LowMoveCharAnimationIndex = i;
			break;
		case ANI_DYING:
			DyingCharAnimationIndex = i;
			break;
		case ANI_ATTACK:
			AttackCharAnimationIndex = i;
			break;

		case ANI_ATTACK2:
			Attack2CharAnimationIndex = i;
			break;

		case ANI_SWIM:
			SwimCharAnimationIndex = i;
			break;
		case ANI_SWIM_WAIT:
			SwimWaitCharAnimationIndex = i;
			break;
			
		case ANI_AMBUSH:
			AmbushCharAnimationIndex = i;
			break;
			
		case ANI_MAGIC:
			MagicCharAnimationIndex = i;
			break;
			
		case ANI_HEAL:
			HealCharAnimationIndex = i;
			break;
		case ANI_MANA:
			ManaCharAnimationIndex = i;
			break;
			
		case ANI_LOWATTACK:
			LowAttackCharAnimationIndex = i;
			break;

		case ANI_READYTOFIRE:
			ReadyToFireCharAnimationIndex = i;
			break;

		case ANI_SATJETC:
			SatJEtcCharAnimationIndex = i;
			break;

		case ANI_MAGICETC:
			MagicEtcCharAnimationIndex = i;
			break;

		case ANI_FIREHOUSEKETC:
			FireHouseKEtcCharAnimationIndex = i;
			break;

		case ANI_ETC:
			EtcCharAnimationIndex = i;
			break;
		
		case ANI_SHIPYARDKETC:
			ShipyardKEtcCharAnimationIndex = i;
			break;

		case ANI_TOWERCETC:
			TowerCEtcCharAnimationIndex = i;
			break;

		case ANI_ATTACKETC:
			AttackEtcCharAnimationIndex = i;
			break;

		case ANI_DYINGETC:
			DyingEtcCharAnimationIndex = i;
			break;
	
		case ANI_GENERAL4K_MAGICETC:
			General4kMagicEtcCharAnimationIndex = i;
			break;
	
		case ANI_GENERAL4J_MAGICETC:
			General4jMagicEtcCharAnimationIndex = i;
			break;

		case ANI_INVALID:
			InvalidCharAnimationIndex = i;
			break;

		case ANI_MAKEPORTAL:
			MakePortalCharAnimationIndex = i;
			break;

		case ANI_INGROUND:
			InGroundCharAnimationIndex = i;
			break;

		case ANI_OUTGROUND:
			OutGroundCharAnimationIndex = i;
			break;
		case ANI_MAGIC2:
			Magic2CharAnimationIndex = i;
			break;

		default:
			clGrp.Error("fds9282", "[%d]", KI[m_cltCharStatus.GetKind()].m_siCharAnimationInfo[i])  ;
			break;
		}
	}

	m_cltCharDrawInfo.SetRealImageXPercent(KI[m_cltCharStatus.GetKind()].m_ssXsizePercent);
	m_cltCharDrawInfo.SetRealImageYPercent(KI[m_cltCharStatus.GetKind()].m_ssYsizePercent);


	realimagexsize	= ImageXsize * m_cltCharDrawInfo.GetRealImageXPercent()/100;
	realimageysize	= ImageYsize * m_cltCharDrawInfo.GetRealImageYPercent()/100;
	
	RECT rect;
	rect.left	= (ImageXsize-realimagexsize)/2;
	rect.right	= rect.left+realimagexsize;
	
	rect.bottom	= ImageYsize - (KI[m_cltCharStatus.GetKind()].m_ssBottomIndent);
	rect.top		= rect.bottom-realimageysize;

	m_cltCharDrawInfo.SetRealImageRect(rect);
}