#include <GSL.h>
#include "VillageAttackInfo.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자. 초기화를 수행한다.
//	수정 일자	:	2002-05-27 오후 2:08:16 - 양정모	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
clVillageAttackInfo::clVillageAttackInfo()
{
	// 각 공격 패턴을 정의한다.

	// 물 공격	
	stVAInfo[VILLAGE_ATTACK_TYPE_WATER].siAttackPower	=	10;

	// 돌 공격	
	stVAInfo[VILLAGE_ATTACK_TYPE_STONE].siAttackPower	=	20;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공격력을 얻어온다.
//	수정 일자	:	2002-05-27 오후 2:17:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	clVillageAttackInfo::GetAttackPower(SI32 siAttackID)
{
	// 공격력을 얻어온다.
	return stVAInfo[siAttackID].siAttackPower;
}