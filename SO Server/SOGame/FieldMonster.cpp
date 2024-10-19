#include "SOMain.h"
#include "FieldMonster.h"
#include "OnlineCGSHeader.h"
#include "ISOMonster.h"
#include "IOnlineMap.h"

IMonster	*cltFieldMonster::pIMonster		=	NULL;
ISOMap		*cltFieldMonster::pIMap			=	NULL;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltFieldMonster::cltFieldMonster()
{
	siID						=	0;
	uiMapID						=	0;
	siBaseX						=	0;
	siBaseY						=	0;
	siRadius					=	0;
	siMaxMonsterKindPerGroup	=	0;
	siMaxMonsterNum				=	0;
	siMinMonsterNum				=	0;
	dwPrevResurrectTime			=	0;
	
	ZeroMemory(FieldMonsterInfo, sizeof(FieldMonsterInfo));
	ZeroMemory(&rcActivityArea, sizeof(rcActivityArea));
}	

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//	���� ����	:	2002-02-27 ���� 1:07:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::Init(SI32 siid, UI16 uimapid, SI32 six, SI32 siy, SI32 siradius, SI32 simaxmonsterkindpergroup, SI32 sigroupminquantity, SI32 sigroupmaxquantity, SI32 simaxmonsternum, SI32 siminmonsternum, stFieldMonsterInfo pFieldMonsterInfo[MAX_FIELD_MONSTER_KIND_PER_GROUP])
{	
	SI32						i;
	SI32						siMonsterCounter = 0;		
	SIZE						szMapSize;

	siID						=	siid;
	uiMapID						=	uimapid;
	siBaseX						=	six;
	siBaseY						=	siy;
	siRadius					=	siradius;
	siMaxMonsterKindPerGroup	=	simaxmonsterkindpergroup;
	siMaxMonsterNum				=	simaxmonsternum;
	siMinMonsterNum				=	siminmonsternum;	
	siMaxMonsterNumPerGroup		=	sigroupmaxquantity;
	siMinMonsterNumPerGroup		=	sigroupminquantity;

	szMapSize					=	pIMap->GetMapSize(uiMapID);

	rcActivityArea.left			=	max(siBaseX - siRadius, 0);
	rcActivityArea.top			=	max(siBaseY - siRadius, 0);
	rcActivityArea.right		=	min(siBaseX + siRadius, szMapSize.cx - 1);
	rcActivityArea.bottom		=	min(siBaseY + siRadius, szMapSize.cy - 1);
	
	memcpy(FieldMonsterInfo, pFieldMonsterInfo, sizeof(FieldMonsterInfo));

	// Total Rate�� ���Ѵ�.
	siMaxMonsterKindNum		=	0;	
	for(i = 0; i < MAX_FIELD_MONSTER_KIND_PER_GROUP; i++)
	{
		if(FieldMonsterInfo[i].uiKind != 0)
		{
			uiSourceMonsterKindIndexTable[siMaxMonsterKindNum]	=	i;
			siMaxMonsterKindNum++;
		}
	}

	// ���� siMaxMonsterNum��ŭ ������ �ɴ´�.	
	siCurrentFieldMonsterNum	=	0;
	for(i = 0; i < siMaxMonsterNum; i++)
	{	
		// ���͸� �����Ѵ�.
		CreateMonster();			
	}
	
	// ������ ���͸� ��Ȱ��Ų �ð��� ����Ѵ�.
	dwPrevResurrectTime	=	timeGetTime();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ Kind�� ���´�.
//	���� ����	:	2002-12-15 ���� 12:51:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	cltFieldMonster::GetWillCreateMonsters( UI16 uiMonsterKinds[ ON_MAX_CHAR_PER_PLAYER ], SI32 siMonsterLevel[ ON_MAX_CHAR_PER_PLAYER ] )
{	
	stFieldMonsterInfo		*pstMonsterInfo;
	UI16					uiRandomlyIndex;
	SI32					siMonsterKindPerGroup, siMonsterKindNum;
	UI08					uiMonsterKindIndexTable[ MAX_FIELD_MONSTER_KIND_PER_GROUP ];
	UI08					uiSelectedMonsterKindIndex[ MAX_FIELD_MONSTER_KIND_PER_GROUP ];
	SI32					i, j;
	SI32					siMonsterCounter, siTempMonsterNum;
	UI16					uiRepresentKind, siRepresentKindNum;
	SI32					siMonsterNumPerGroup;
	SI32					siTotalRate;     

	//pIMonster->CheckInvalidChar( "Start cltFieldMonster::GetWillCreateMonsters()" );

	ZeroMemory( uiMonsterKinds, sizeof( UI16 ) * ON_MAX_CHAR_PER_PLAYER );
	ZeroMemory( siMonsterLevel, sizeof( SI32 ) * ON_MAX_CHAR_PER_PLAYER );	

	if( siMaxMonsterKindPerGroup == 0 || siMaxMonsterKindNum == 0 )
	{
		printf("siMaxMonsterKindPerGroup or(and) siMaxMonsterKindNum Equal Zero [%d, [%d\n", siMaxMonsterKindPerGroup, siMaxMonsterKindNum);
		return 0;
	}

	if( siMaxMonsterKindPerGroup > siMaxMonsterKindNum )
	{
		printf("siMaxMonsterKindPerGroup greater than siMaxMonsterKindNum, [%d] > [%d]\n", siMaxMonsterKindPerGroup, siMaxMonsterKindNum);
		return 0;
	}
	
	// siMaxMonsterKindPerGroup�� ���͸� ������ ���� �ȴ�.
	siMonsterKindPerGroup		=	siMaxMonsterKindPerGroup;

	// MonsterKind �� ����ִ� Table�� �����Ѵ�.	
	siMonsterKindNum			=	siMaxMonsterKindNum;
	memcpy( uiMonsterKindIndexTable, uiSourceMonsterKindIndexTable, siMaxMonsterKindNum );	

	siTotalRate					=	0;		
	siMonsterNumPerGroup		=	rand() % ( siMaxMonsterNumPerGroup - siMinMonsterNumPerGroup + 1 ) + siMinMonsterNumPerGroup;		// �� ���� �׷��� ������ ������ ���� ���Ѵ�.			
	siMonsterNumPerGroup		= siMonsterNumPerGroup < 1 ? siMinMonsterNumPerGroup : siMonsterNumPerGroup;

	// �켱 siMonsterKindPerGroup ��ŭ �����ϰ� ������ Kind�� �����Ѵ�.
	for( i = 0; i < siMonsterKindPerGroup; i++ )
	{
		// �����ϰ� �ϳ��� �ε����� �����س���.
		uiRandomlyIndex		=	rand() % siMonsterKindNum;

		pstMonsterInfo					=	&FieldMonsterInfo[ uiMonsterKindIndexTable[ uiRandomlyIndex ] ];		
		uiSelectedMonsterKindIndex[ i ]	=	uiMonsterKindIndexTable[ uiRandomlyIndex ];

		siTotalRate						+=	pstMonsterInfo->uiRate;		

		// ���� Table�� �ٽ� �����Ѵ�.		
		memmove( &uiMonsterKindIndexTable[ uiRandomlyIndex ], &uiMonsterKindIndexTable[ uiRandomlyIndex + 1 ], ( siMonsterKindNum - ( uiRandomlyIndex + 1 ) ) );		

		// siMonsterKindNum�� �ϳ� ���ҽ�Ų��.
		siMonsterKindNum--;
	}	
	
	uiRepresentKind				=	0;
	siRepresentKindNum			=	0;
	siMonsterCounter			=	0;	
	for(i = 0; i < siMonsterKindPerGroup; i++)
	{			
		pstMonsterInfo		=	&FieldMonsterInfo[uiSelectedMonsterKindIndex[i]];

		// �ּ� �Ѹ��� �̻��� ���� �־�� �Ѵ�.		
		siTempMonsterNum	=	max((siMonsterNumPerGroup * pstMonsterInfo->uiRate) / siTotalRate, 1);
		
		for(j = 0; j < siTempMonsterNum; j++)
		{		
			uiMonsterKinds[j + siMonsterCounter]	=	pstMonsterInfo->uiKind;
			siMonsterLevel[j + siMonsterCounter]	=	pstMonsterInfo->siLevel;
		}
	
		siMonsterCounter	+=	siTempMonsterNum;
	
		if(siRepresentKindNum < siTempMonsterNum)
		{
			uiRepresentKind		=	pstMonsterInfo->uiKind;
			siRepresentKindNum	=	siTempMonsterNum;
		}	
	}	
	
	//pIMonster->CheckInvalidChar( "End cltFieldMonster::GetWillCreateMonsters()" );

	return uiRepresentKind;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�׼�.
//	���� ����	:	2002-02-27 ���� 1:07:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::Action()
{

	if(siCurrentFieldMonsterNum < siMinMonsterNum)
	{
		// ���� ���� ���� �ּ� ���� ������ ���� ������ �ٽ� �����Ų��.		
		CreateMonster();

		// ���͸� ����� �ð��� ����Ѵ�.
		dwPrevResurrectTime	=	timeGetTime();
	}
	else
	{
		if(siCurrentFieldMonsterNum < siMaxMonsterNum)
		{
			// ���� �ִ� ���� ���� �۰� ���� �ð��� ��� �ߴٸ� ���͸� �Ѹ��� �����Ų��.
			if((timeGetTime() - dwPrevResurrectTime) > 1000)
			{
				// ���͸� �Ѹ��� �����Ѵ�.				
				CreateMonster();

				// ���͸� ����� �ð��� ����Ѵ�.
				dwPrevResurrectTime	=	timeGetTime();
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���͸� �����Ѵ�.(�Ѹ���)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltFieldMonster::CreateMonster()
{
	UI16		uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER];	
	SI32		siMonsterLevel[ON_MAX_CHAR_PER_PLAYER];	
	UI16		uiRepresentKind;

	// �����ؾ� �ϴ� ������ ������ ���Ѵ�.
	uiRepresentKind	=	GetWillCreateMonsters(uiMonsterKinds, siMonsterLevel);

	if(uiRepresentKind != 0)
	{
		// ���͸� �����Ѵ�.
		// ���͸� �����ϴ� ������ �˷��ش�.
		if(pIMonster->CreateMonster(siID, uiMapID, &rcActivityArea, uiRepresentKind, uiMonsterKinds, siMonsterLevel) == FALSE)
		{
			// ���ϰ��� FALSE��� �� �̻��� ���͸� ������ �� ���ٴ� ���̴�.
			return FALSE;
		}
		else
		{
			siCurrentFieldMonsterNum++;				// ���� �ʵ忡 �ִ� ������ ���� ���� ��Ų��.
			return TRUE;
		}	
	}
	else
	{
		return FALSE;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���Ͱ� �׾���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::DeadMonster()
{	
	siCurrentFieldMonsterNum	=	max(siCurrentFieldMonsterNum - 1, 0);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ID�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltFieldMonster::GetID()
{
	return siID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-06-03 ���� 4:10:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::InitStaticVariable(IMonster *pimonster, ISOMap *pimap)
{	
	pIMonster	=	pimonster;
	pIMap		=	pimap;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-02-27 ���� 1:07:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::StartBattle()
{
	// ���� �ʵ忡 �ִ� ������ ���� ���� ��Ų��.
	siCurrentFieldMonsterNum--;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-02-27 ���� 1:29:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::EndBattle()
{
	// ���� �ʵ忡 �ִ� ������ ���� ���� ��Ų��.
	siCurrentFieldMonsterNum++;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���´�.
//	���� ����	:	2002-06-17 ���� 11:08:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltFieldMonster::GetInfo(CHAR *pszBuffer)
{
	SI32	i;
	SI32	siRealMonsterNum;
	UI16	uiAccount;
	UI16	uiFirstMonsterAccount = 0;	

	siRealMonsterNum	=	0;
	for(i = 0; i < MAX_MONSTER_NUM; i++)
	{
		uiAccount	=	MONSTER_START_ACCOUNT + i;

		if(pIMonster->IsValidID(uiAccount) == TRUE)
		{
			if(pIMonster->GetFieldID(uiAccount) == siID)
			{
				if(uiFirstMonsterAccount == 0)
				{
					//if(pIMonster->GetStatus(uiAccount) == ON_PLAYER_STATUS_INFIELD)
					if( CHECKSTATUS( pIMonster->GetStatus(uiAccount), ON_PLAYER_STATUS_INFIELD ) == TRUE )
						uiFirstMonsterAccount	=	uiAccount;
				}

				siRealMonsterNum++;
			}
		}
	}
		
	sprintf(pszBuffer, "FID : [%d] MAX : [%u], MIN : [%u], CUR : [%u], REAL  : [%u], Account : [%u]", 
						siID, siMaxMonsterNum, siMinMonsterNum, siCurrentFieldMonsterNum, siRealMonsterNum, uiFirstMonsterAccount);
}

