#include "SOMain.h"
#include "SOWorld.h"
#include "ISOMonster.h"

#define	MONSTERACCOUNTTOINDEX(x)	(x - MONSTER_START_ACCOUNT)


//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
IMonster::IMonster()
{	
	uiLastNPCAccount		=	MONSTER_START_ACCOUNT;	
}	



//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자
//----------------------------------------------------------------------------------------------------------------
IMonster::~IMonster()
{
	Free();
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화.
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Init(SOWorld *pWorld)
{	
	pMyWorld		=	pWorld;
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	몬스터를 생성한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::CreateMonster(SI32 siFieldMonsterID, UI16 uiMapID, LPRECT prcActivityArea, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{
	UI16	uiAccount;
	SI32	siX, siY;	
	POINT	ptSearchedPos;
	
	
	if((uiAccount =	FindAccount()) != 0)
	{
		// 랜덤하게 기준점을 하나 정한다.		
		siX				=	rand() % (prcActivityArea->right - prcActivityArea->left) + (prcActivityArea->left);
		siY				=	rand() % (prcActivityArea->bottom - prcActivityArea->top) + (prcActivityArea->top);

		// 몬스터는 땅에만 존재한다.
		ptSearchedPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);

		// 자리를 성공적으로 찾았는지 검사한다.
		if(ptSearchedPos.x != -1 && ptSearchedPos.y != -1)
		{	
			// 캐릭터를 놓을 자리를 찾았다.
			Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Init(siFieldMonsterID, uiAccount, uiMapID, ptSearchedPos.x, ptSearchedPos.y, prcActivityArea, uiRepresentKind, uiMonsterKinds, siMonsterLevel);			
			
			return TRUE;			
		}					
	}
	
	printf("create monster failed : [%d] [%u]\n", siFieldMonsterID, uiRepresentKind);
	return FALSE;
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	몬스터를 생성한다.
//	수정 일자	:	2002-06-03 오후 8:17:09 - 양정모
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::CreateMonster(UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{
	UI16	uiAccount;	
	POINT	ptSearchedPos;

	// 1000번 정도 위치를 랜덤하게 찾은 후에 없으면 실패 처리 한다.
	if((uiAccount =	FindAccount()) != 0)
	{
		// 몬스터는 땅에만 존재한다.
		ptSearchedPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);

		// 자리를 성공적으로 찾았는지 검사한다.
		if(ptSearchedPos.x != -1 && ptSearchedPos.y != -1)
		{	
			Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Init(0, uiAccount, uiMapID, ptSearchedPos.x, ptSearchedPos.y, NULL, uiRepresentKind, uiMonsterKinds, siMonsterLevel);				
			return TRUE;				
		}
	}	
	
	return FALSE;
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제.
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Free()
{	
	SI32		i;

	for(i = 0; i < MAX_MONSTER_NUM; i++)
	{
		// 생성되어 있는 캐릭터를 없앤다.
		if(Monsters[i].IsValid() == TRUE)
		{
			Monsters[i].Free(FALSE);
		}
	}

	uiLastNPCAccount		=	0;	
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	특정 몬스터를 해제한다.
//	수정 일자	:	2002-05-11 오후 3:08:05 - 양정모
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Free(UI16 uiAccount, BOOL IsDeadInBattle)
{
	if(IsValidID(uiAccount) == TRUE)		
	{
		// 전체 몬스터를 하나감소 시킨다.
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Free(IsDeadInBattle);		
	}
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	Action()
//	수정 일자	:	2002-06-03 오후 5:53:11 - 양정모
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::Action(UI16 uiAccount)
{	
	if(IsValidID(uiAccount) == TRUE)		
	{
		// 해당 몬스터의 Action을 취해준다.
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Action();
	}
	else
	{
		// 절대 들어와서는 안되는 부분이다.
		printf("Invalid Monster Action : [%u]\n", uiAccount);			
	}
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	비어있는 계정을 찾는다. 
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	몬스터의 정보를 얻어온다.	
//	수정 일자	:	2002-08-15 오후 12:51:21 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IMonster::GetInfo(UI16 uiAccount, OnCharMonsterInfo *pMonsterInfo)
{
	if(IsValidID(uiAccount) == TRUE)		
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetInfo(pMonsterInfo);
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	유효한 ID 인가?
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::IsValidID(UI16 uiAccount)
{
	if(ISMONSTER(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].IsValid();	
	else
		return FALSE;
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 죽인다.(캐릭터 자체를)
//	수정 일자	:	2002-05-11 오후 3:05:11 - 양정모
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::DeadChar(UI16 uiAccount, BOOL IsDeadInBattle)
{
	Free(uiAccount, IsDeadInBattle);	
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 상태를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	IMonster::GetStatus(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetStatus();			
	else
		return 0;
}



//----------------------------------------------------------------------------------------------------------------
//	설명	:	몬스터의 전투력을 얻어옴
//----------------------------------------------------------------------------------------------------------------
SI32	IMonster::GetFightPower(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetFightingPower();				
	else
		return 0;	
}	



//----------------------------------------------------------------------------------------------------------------
//	설명	:	몬스터를 워프 시킨다.
//----------------------------------------------------------------------------------------------------------------
BOOL	IMonster::Warp(UI16 uiAccount, UI16 uiMapID, SI32 siX, SI32 siY)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].Warp(uiMapID, siX, siY);				
	else
		return 0;	
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	캐릭터의 종류를 얻어온다.
//	수정 일자	:	2002-03-05 오후 5:21:03 - 양정모
//----------------------------------------------------------------------------------------------------------------
UI16	IMonster::GetKind(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)
		return Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetKind();				
	else
		return 0;		
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	몬스터의 위치를 알아온다.	
//	수정 일자	:	2002-04-15 오후 12:48:45 - 양정모
//----------------------------------------------------------------------------------------------------------------
VOID	IMonster::GetPos(UI16 uiAccount, UI16 *puiMapIndex, LPPOINT pPoint)
{
	if(IsValidID(uiAccount) == TRUE)
		Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetPos(puiMapIndex, pPoint);		
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	몬스터를 얻어온다.
//	수정 일자	:	2002-05-10 오후 3:49:16 - 양정모
//----------------------------------------------------------------------------------------------------------------
Monster*	IMonster::GetMonster(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)	
		return	&Monsters[MONSTERACCOUNTTOINDEX(uiAccount)];	
	else
		return 0;
}



//----------------------------------------------------------------------------------------------------------------
//	설명		:	몬스터가 소속해 있는 필드의 ID를 얻어온다.
//	수정 일자	:	2002-06-17 오전 9:49:11 - 양정모
//----------------------------------------------------------------------------------------------------------------
SI32	IMonster::GetFieldID(UI16 uiAccount)
{	
	if(IsValidID(uiAccount) == TRUE)	
		return	Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].GetFieldID();		
	else
		return 0;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이동 했을시에 이동 메시지를 주위에 플레이어들에게 보냈는가 알아온다.
//	수정 일자	:	2002-07-26 오후 6:12:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	IMonster::DidSendMoveInfo(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == TRUE)	
		return	Monsters[MONSTERACCOUNTTOINDEX(uiAccount)].DidSendMoveInfo();		
	else
		return 0;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
