#include "SOMain.h"
#include "IFieldMonster.h"
#include "ISOMonster.h"
#include "OnlineMonsterParser.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
IFieldMonster::IFieldMonster()
{
	pFieldMonster		=	NULL;
	siFieldMonsterNum	=	0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
IFieldMonster::~IFieldMonster()
{
	if(pFieldMonster)
	{
		delete [] pFieldMonster;
		pFieldMonster	=	NULL;
	}	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Init.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IFieldMonster::Init(IMonster *pIMonster, ISOMap *pIMap)
{
	OnlineMonsterParser			Parser;
	MonsterPatternHeader		*pMonsterPatternHeader;
	SI32						i, j;
	stFieldMonsterInfo			FieldMonsterInfo[MAX_FIELD_MONSTER_KIND_PER_GROUP];

	m_pIMonster		=	pIMonster;

	if(Parser.Init("Field Monster.txt") == TRUE)
	{		
		siFieldMonsterNum	=	Parser.GetTotalPattern();				

		pFieldMonster		=	new cltFieldMonster[siFieldMonsterNum+1];		

		// Static ������ �ʱ�ȭ ��Ų��.

		cltFieldMonster::InitStaticVariable(pIMonster, pIMap);		

		for(i = 1; i < siFieldMonsterNum + 1; i++)
		{				
			pMonsterPatternHeader	=	Parser.GetPatternHeader(i);
	
			for(j = 0; j < MAX_FIELD_MONSTER_KIND_PER_GROUP; j++)
			{
				FieldMonsterInfo[j].uiKind	=	pMonsterPatternHeader->uiMonsterID[j];
				FieldMonsterInfo[j].uiRate	=	pMonsterPatternHeader->siRate[j];
				FieldMonsterInfo[j].siLevel	=	pMonsterPatternHeader->uiMonsterLv[j];
			}

			pFieldMonster[i].Init(i, 
									pMonsterPatternHeader->siMapIndex,
									pMonsterPatternHeader->siX, 
									pMonsterPatternHeader->siY, 
									pMonsterPatternHeader->siRadius, 
									pMonsterPatternHeader->siMaxKind,									
									pMonsterPatternHeader->siGroupMinQuantity,
									pMonsterPatternHeader->siGroupMaxQuantity,
									pMonsterPatternHeader->siMaxQuantity,
									pMonsterPatternHeader->siMinQuantity, 
									FieldMonsterInfo);		

		}		
	}
	else
	{
		printf("Field Monster Parser Failed !\n");
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�׼�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IFieldMonster::Action()
{
	SI32	i;

	/*
	{
		m_pIMonster->CheckInvalidChar( "Start IFieldMonster::Action()" );
	}
	*/

	for( i = 1; i < siFieldMonsterNum + 1; i++ )
	{
		pFieldMonster[ i ].Action();
	}

	/*
	{
		CHAR	szFieldMonsterAction[ 1024 ];

		ZeroMemory( szFieldMonsterAction, sizeof( szFieldMonsterAction ) );
		sprintf( szFieldMonsterAction, "End IFieldMonster::Action() [ %d ]", siFieldMonsterNum );
		m_pIMonster->CheckInvalidChar( szFieldMonsterAction );
	}
	*/

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���Ͱ� �׾���.	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IFieldMonster::DeadMonster(SI32 siID)
{
	SI32	siIndex;		

	siIndex	=	FindIndex(siID);

	if(siIndex != 0)
	{
		pFieldMonster[siIndex].DeadMonster();
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ID�� ����Ͽ� INDEX�� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IFieldMonster::FindIndex(SI32 siID)
{
	SI32	i;

	for(i = 1; i < siFieldMonsterNum + 1; i++)
		if(pFieldMonster[i].GetID() == siID)
			return i;

	return 0;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-02-27 ���� 1:34:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IFieldMonster::StartBattle(SI32 siID)
{
	SI32	siIndex;		

	siIndex	=	FindIndex(siID);

	if(siIndex != 0)
	{
		pFieldMonster[siIndex].StartBattle();
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-02-27 ���� 1:34:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IFieldMonster::EndBattle(SI32 siID)
{
	SI32	siIndex;		

	siIndex	=	FindIndex(siID);

	if(siIndex != 0)
	{
		pFieldMonster[siIndex].EndBattle();
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���´�.
//	���� ����	:	2002-06-17 ���� 11:07:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IFieldMonster::GetInfo(SI32 siID, CHAR *pszBuffer)
{
	SI32	siIndex;		

	siIndex	=	FindIndex(siID);

	if(siIndex != 0)
	{
		pFieldMonster[siIndex].GetInfo(pszBuffer);
		return	TRUE;
	}
	else
		return	FALSE;
}