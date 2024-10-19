#ifndef	_SCCOMMON_VILLAGE_ATTACK_HEADER
#define	_SCCOMMON_VILLAGE_ATTACK_HEADER

#include <windows.h>
#include <directives.h>

enum	{VILLAGE_ATTACK_TYPE_WATER, VILLAGE_ATTACK_TYPE_STONE, VILLAGE_ATTACK_TYPE_NUM};

// 마을이 플레이어를 공격하는 것에 대한 정보
struct	strVillageAttackInfo
{
	SI32	siAttackPower;
};

class	clVillageAttackInfo
{	
public:
	strVillageAttackInfo	stVAInfo[VILLAGE_ATTACK_TYPE_NUM];

public:
	clVillageAttackInfo();		// 생성자. 초기화를 수행한다.	

	SI32	GetAttackPower(SI32 siAttackID);		// 공격력을 얻어온다.
};

#endif
