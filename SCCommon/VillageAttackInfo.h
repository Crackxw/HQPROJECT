#ifndef	_SCCOMMON_VILLAGE_ATTACK_HEADER
#define	_SCCOMMON_VILLAGE_ATTACK_HEADER

#include <windows.h>
#include <directives.h>

enum	{VILLAGE_ATTACK_TYPE_WATER, VILLAGE_ATTACK_TYPE_STONE, VILLAGE_ATTACK_TYPE_NUM};

// ������ �÷��̾ �����ϴ� �Ϳ� ���� ����
struct	strVillageAttackInfo
{
	SI32	siAttackPower;
};

class	clVillageAttackInfo
{	
public:
	strVillageAttackInfo	stVAInfo[VILLAGE_ATTACK_TYPE_NUM];

public:
	clVillageAttackInfo();		// ������. �ʱ�ȭ�� �����Ѵ�.	

	SI32	GetAttackPower(SI32 siAttackID);		// ���ݷ��� ���´�.
};

#endif
