//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

#include <player.h>
#include <CharOrg.h>
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include "..\Gersang\Message.h"

#include <myfile.h>




extern	_KindInfo					KI[];							// 캐릭터 종류별 데이터 
extern	_FileVary 				FV;

extern	CCharFileLoadManager	g_FileLoadManager;				// 조합형 캐릭터

void _Char::DecideFontNormal()
{
	SI32 file = 0, font = 0 ;
	SI32 anistep = 0;
	anistep = m_cltCharDrawInfo.GetAniStep();

	switch(m_cltCharDrawInfo.GetAnimation())
	{
	case ANI_DYING:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[DyingCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break;
	case ANI_READYTOFIRE: 			KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ReadyToFireCharAnimationIndex].DecideFontAni(Direction,	anistep,	&file, &font, &m_bReverseSwitch);		break;
	case ANI_SWIM:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[SwimCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_SWIM_WAIT:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[SwimWaitCharAnimationIndex].DecideFontAni(Direction,		anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_AMBUSH:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AmbushCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_MAGIC:     			KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MagicCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_MAGIC2:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Magic2CharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_HEAL:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[HealCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_MANA:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[ManaCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_LOWATTACK:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[LowAttackCharAnimationIndex].DecideFontAni(Direction,		anistep,	&file, &font, &m_bReverseSwitch);		break; 
	case ANI_ATTACK:				DecideFontAttack(&file, &font, &m_bReverseSwitch);		break;	
	case ANI_ATTACK2:				DecideFontAttack2(&file, &font, &m_bReverseSwitch);	break;	
	case ANI_LOWMOVE:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[LowMoveCharAnimationIndex].DecideFontAni(Direction,		anistep,	&file, &font, &m_bReverseSwitch);			break;
	case ANI_MOVE:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MoveCharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);				break;
	case ANI_WAIT0:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Wait0CharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);				break; // 현재 대기 상태일때에만 
	case ANI_WAIT1:					KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Wait1CharAnimationIndex].DecideFontAni(Direction,			anistep,	&file, &font, &m_bReverseSwitch);				break; // 현재 대기 상태일때에만 
	case ANI_INGROUND:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[InGroundCharAnimationIndex].DecideFontAni(Direction,		anistep,	&file, &font, &m_bReverseSwitch);			break; // 독지네 땅속으로 들어가는 애니메이션. 
	case ANI_OUTGROUND:				KI[m_cltCharStatus.GetKind()].m_clCharAnimation[OutGroundCharAnimationIndex].DecideFontAni(Direction,		anistep,	&file, &font, &m_bReverseSwitch);			break; // 독지네 땅속에서 나오는 애니메이션. 

	case ANI_MAKEPORTAL:			KI[m_cltCharStatus.GetKind()].m_clCharAnimation[MakePortalCharAnimationIndex].DecideFontAni(Direction,		anistep,	&file, &font, &m_bReverseSwitch);			break; 	

    default:
		font=Step;
		clGrp.Error("fdsa3r3", "[kind:%d %s][order:%d][ani:%d]", m_cltCharStatus.GetKind(), KI[m_cltCharStatus.GetKind()].m_pszName, CurrentOrder, m_cltCharDrawInfo.GetAnimation());
	    break;
	} 

	m_cltCharDrawInfo.SetFile(file);
	m_cltCharDrawInfo.SetFont(font);


}

// 찍어야 할 그림을 선정한다. 
void _Char::DecideFont()
{

	if(m_cltCharDrawInfo.GetUpdateAnimationSwitch()==FALSE)
		return ;

	m_cltCharDrawInfo.SetUpdateAnimationSwitch(FALSE);

	if( KI[m_cltCharStatus.GetKind()].IsKindAtb(KINDATB_HERO) )
	{
		DecideFontHero();
	}
	else
	{
		DecideFontNormal();
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 공격시의 애니메이션 처리 
void _Char::DecideFontAttack(SI32 * pfile, SI32* pfont, BOOL* preverseswitch)
{

	// 공격 프레임이 따로 없는 것이라면, 
	if( KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].GetFrameNumber() == 0 )
	{
		KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Wait0CharAnimationIndex].DecideFontAni(Direction, m_cltCharDrawInfo.GetAniStep(), pfile, pfont, preverseswitch);
	}
	else
	{
		KI[m_cltCharStatus.GetKind()].m_clCharAnimation[AttackCharAnimationIndex].DecideFontAni(Direction, m_cltCharDrawInfo.GetAniStep(), pfile, pfont, preverseswitch);	 
	}


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 공격시의 애니메이션 처리 
void _Char::DecideFontAttack2(SI32 * pfile, SI32* pfont, BOOL* preverseswitch)
{
	// 공격 프레임이 따로 없는 것이라면, 
	if( KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Attack2CharAnimationIndex].GetFrameNumber() == 0 )
	{
		KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Wait0CharAnimationIndex].DecideFontAni(Direction, m_cltCharDrawInfo.GetAniStep(), pfile, pfont, preverseswitch);
	}
	else
	{
		KI[m_cltCharStatus.GetKind()].m_clCharAnimation[Attack2CharAnimationIndex].DecideFontAni(Direction, m_cltCharDrawInfo.GetAniStep(), pfile, pfont, preverseswitch);	 
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 주인공 조합형 캐릭터의 화면에 보여줄 폰트 결정
void _Char::DecideFontHero()
{
	SI32 siFontCount;

	// 한 방향당 총 폰트수를 얻어온다.
	siFontCount		= g_FileLoadManager.m_HeroConfig[KI[m_cltCharStatus.GetKind()].m_ssHeroID].siFontCount[AniAction][CharBaseWClass];

	if( AniAction == HERO_ACT_DT )
	{
		DirectionNo = 0;
		m_bReverseSwitch = FALSE;
	}
	else
	{
		switch( Direction )
		{
		case EAST|SOUTH:	DirectionNo = 0; 	m_bReverseSwitch = FALSE;	break;
		case SOUTH:			DirectionNo = 1; 	m_bReverseSwitch = FALSE;	break;
		case SOUTH|WEST:	DirectionNo = 2; 	m_bReverseSwitch = FALSE;	break;
		case WEST:			DirectionNo = 3; 	m_bReverseSwitch = FALSE;	break;
		case NORTH|WEST:	DirectionNo = 4; 	m_bReverseSwitch = FALSE;	break;
		case NORTH:			DirectionNo = 3;  	m_bReverseSwitch = TRUE;	break;
		case NORTH|EAST:	DirectionNo = 2;   	m_bReverseSwitch = TRUE;	break;
		case EAST:			DirectionNo = 1;   	m_bReverseSwitch = TRUE;	break;
		default:			break;
		}
	}
	
	// 그 방향의 첫 폰트 번호
	AniStartFont = siFontCount * DirectionNo;


	// 이번에 찍힐 Animation을 설정한다.
	AniStep_Hero = m_cltCharDrawInfo.GetAniStep();//???
}


