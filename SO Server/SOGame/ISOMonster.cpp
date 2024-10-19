#include "SOMain.h"
#include "SOWorld.h"
#include "ISOMonster.h"

#define	MONSTERACCOUNTTOINDEX(x)	(x - MONSTER_START_ACCOUNT)


//----------------------------------------------------------------------------------------------------------------
//	����	:	������.
//----------------------------------------------------------------------------------------------------------------
IMonster::IMonster()
{	
	uiLastNPCAccount		=	MONSTER_START_ACCOUNT;	
}	



//----------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���
//----------------------------------------------------------------------------------------------------------------
IMonster::~IMonster()
{
	Free();
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Init(SOWorld *pWorld)
{	
	pMyWorld		=	pWorld;
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	���͸� �����Ѵ�.
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::CreateMonster(SI32 siFieldMonsterID, UI16 uiMapID, LPRECT prcActivityArea, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{
	UI16	uiAccount;
	SI32	siX, siY;	
	POINT	ptSearchedPos;
	
	
	if((uiAccount =	FindAccount()) != 0)
	{
		// �����ϰ� �������� �ϳ� ���Ѵ�.		
		siX				=	rand() % (prcActivityArea->right - prcActivityArea->left) + (prcActivityArea->left);
		siY				=	rand() % (prcActivityArea->bottom - prcActivityArea->top) + (prcActivityArea->top);

		// ���ʹ� ������ �����Ѵ�.
		ptSearchedPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);

		// �ڸ��� ���������� ã�Ҵ��� �˻��Ѵ�.
		if(ptSearchedPos.x != -1 && ptSearchedPos.y != -1)
		{	
			// ĳ���͸� ���� �ڸ��� ã�Ҵ�.
			Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Init(siFieldMonsterID, uiAccount, uiMapID, ptSearchedPos.x, ptSearchedPos.y, prcActivityArea, uiRepresentKind, uiMonsterKinds, siMonsterLevel);			
			
			return TRUE;			
		}					
	}
	
	printf("create monster failed : [%d] [%u]\n", siFieldMonsterID, uiRepresentKind);
	return FALSE;
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	���͸� �����Ѵ�.
//	���� ����	:	2002-06-03 ���� 8:17:09 - ������
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::CreateMonster(UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{
	UI16	uiAccount;	
	POINT	ptSearchedPos;

	// 1000�� ���� ��ġ�� �����ϰ� ã�� �Ŀ� ������ ���� ó�� �Ѵ�.
	if((uiAccount =	FindAccount()) != 0)
	{
		// ���ʹ� ������ �����Ѵ�.
		ptSearchedPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);

		// �ڸ��� ���������� ã�Ҵ��� �˻��Ѵ�.
		if(ptSearchedPos.x != -1 && ptSearchedPos.y != -1)
		{	
			Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Init(0, uiAccount, uiMapID, ptSearchedPos.x, ptSearchedPos.y, NULL, uiRepresentKind, uiMonsterKinds, siMonsterLevel);				
			return TRUE;				
		}
	}	
	
	return FALSE;
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	����.
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Free()
{	
	SI32		i;

	for(i = 0; i < MAX_MONSTER_NUM; i++)
	{
		// �����Ǿ� �ִ� ĳ���͸� ���ش�.
		if(Monsters[i].IsValid() == TRUE)
		{
			Monsters[i].Free(FALSE);
		}
	}

	uiLastNPCAccount		=	0;	
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	Ư�� ���͸� �����Ѵ�.
//	���� ����	:	2002-05-11 ���� 3:08:05 - ������
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Free(UI16 uiAccount, BOOL IsDeadInBattle)
{
	if(IsValidID(uiAccount) == TRUE)		
	{
		// ��ü ���͸� �ϳ����� ��Ų��.
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Free(IsDeadInBattle);		
	}
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	Action()
//	���� ����	:	2002-06-03 ���� 5:53:11 - ������
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Action(UI16 uiAccount)
{	
	if(IsValidID(uiAccount) == TRUE)		
	{
		// �ش� ������ Action�� �����ش�.
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Action();
	}
	else
	{
		// ���� ���ͼ��� �ȵǴ� �κ��̴�.
		printf("Invalid Monster Action : [%u]\n", uiAccount);			
	}
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	����ִ� ������ ã�´�. 
//----------------------------------------------------------------------------------------------------------------
UI16	IMonster::FindAccount()
{
	do
	{
		uiLastNPCAccount++;
		if(uiLastNPCAccount < MONSTER_START_ACCOUNT)
			uiLastNPCAccount	=	MONSTER_START_ACCOUNT;

		if(uiLastNPCAccount > (MONSTER_START_ACCOUNT + MAX_MONSTER_NUM))
			uiLastNPCAccount	=	MONSTER_START_ACCOUNT;		

		if(Monsters[MONSTERACCOUNTTOINDEX(uiLastNPCAccount)].IsValid() == FALSE)
			return uiLastNPCAccount;
	}while(TRUE);

	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ���´�.	
//	���� ����	:	2002-08-15 ���� 12:51:21 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IMonster::GetInfo(UI16 uiAccount, OnCharMonsterInfo *pMonsterInfo)
{
	if(IsValidID(uiAccount) == TRUE)		
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetInfo(pMonsterInfo);
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	��ȿ�� ID �ΰ�?
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::IsValidID(UI16 uiAccount)
{
	if(ISMONSTER(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].IsValid();	
	else
		return FALSE;
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	ĳ���͸� ���δ�.(ĳ���� ��ü��)
//	���� ����	:	2002-05-11 ���� 3:05:11 - ������
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::DeadChar(UI16 uiAccount, BOOL IsDeadInBattle)
{
	Free(uiAccount, IsDeadInBattle);	
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	ĳ������ ���¸� ���´�.
//----------------------------------------------------------------------------------------------------------------
SI16	IMonster::GetStatus(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetStatus();			
	else
		return 0;
}



//----------------------------------------------------------------------------------------------------------------
//	����	:	������ �������� ����
//----------------------------------------------------------------------------------------------------------------
SI32	IMonster::GetFightPower(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetFightingPower();				
	else
		return 0;	
}	



//----------------------------------------------------------------------------------------------------------------
//	����	:	���͸� ���� ��Ų��.
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::Warp(UI16 uiAccount, UI16 uiMapID, SI32 siX, SI32 siY)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Warp(uiMapID, siX, siY);				
	else
		return 0;	
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	ĳ������ ������ ���´�.
//	���� ����	:	2002-03-05 ���� 5:21:03 - ������
//----------------------------------------------------------------------------------------------------------------
UI16	IMonster::GetKind(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetKind();				
	else
		return 0;		
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	������ ��ġ�� �˾ƿ´�.	
//	���� ����	:	2002-04-15 ���� 12:48:45 - ������
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::GetPos(UI16 uiAccount, UI16 *puiMapIndex, LPPOINT pPoint)
{
	if(IsValidID(uiAccount) == TRUE)
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetPos(puiMapIndex, pPoint);		
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	���͸� ���´�.
//	���� ����	:	2002-05-10 ���� 3:49:16 - ������
//----------------------------------------------------------------------------------------------------------------
Monster*	IMonster::GetMonster(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)	
		return	&Monsters[MONSTERACCOUNTTOINDEX(uiAccount)];	
	else
		return 0;
}



//----------------------------------------------------------------------------------------------------------------
//	����		:	���Ͱ� �Ҽ��� �ִ� �ʵ��� ID�� ���´�.
//	���� ����	:	2002-06-17 ���� 9:49:11 - ������
//----------------------------------------------------------------------------------------------------------------
SI32	IMonster::GetFieldID(UI16 uiAccount)
{	
	if(IsValidID(uiAccount) == TRUE)	
		return	Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetFieldID();		
	else
		return 0;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�̵� �����ÿ� �̵� �޽����� ������ �÷��̾�鿡�� ���´°� �˾ƿ´�.
//	���� ����	:	2002-07-26 ���� 6:12:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	IMonster::DidSendMoveInfo(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)	
		return	Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].DidSendMoveInfo();		
	else
		return 0;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IMonster::CheckInvalidChar( CHAR *pszParam )
{
	SI32	i;
	static	BOOL	bInvalidChar = FALSE;
	
	if( bInvalidChar == TRUE )				return FALSE;

	for( i = 0; i < MAX_MONSTER_NUM; i++ )
	{
		if( Monsters[ i ].IsValid() == TRUE )
		{
			if( Monsters[ i ].GetKind() == 0 )
			{
//				pMyWorld->clServer->WriteInfo( "..\\CheckInvalidChar.txt", "Param : [ %s ] [ FID : %d ] [ Account : %d ] [ Array : %d ]", 
				writeInfoToFile( "CheckInvalidChar.txt", "Param : [ %s ] [ FID : %d ] [ Account : %d ] [ Array : %d ]", 
					pszParam, 
							Monsters[ i ].GetFieldID(),
							Monsters[ i ].uiAccount,
							i);

				bInvalidChar	=	TRUE;
			}
		}
	}

	return	TRUE;
}



DWORD	IMonster::GetTotalMonsterNum()
{
	SI32	i;
	DWORD	dwTotalMonsterNum = 0;
	
	for( i = 0; i < MAX_MONSTER_NUM; i++ )
	{
		if( Monsters[ i ].IsValid() == TRUE )	dwTotalMonsterNum++;
	}

	return	dwTotalMonsterNum;
}
