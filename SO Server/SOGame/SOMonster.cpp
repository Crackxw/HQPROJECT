#include "SOMain.h"
#include "SOWorld.h"

#include "SOMonster.h"
#include "SORand.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Monster::Monster()
{
	siX					=	0;
	siY					=	0;

	uiKind				=	0;
	uiAccount			=	0;	
	uiGuildID			=	0;
	siClassInGuild		=	0;


	// 상태 초기화 
	//----------------------------------------------------------------------------------------
	
	// 상태를 등록 한다 
	CState					state;
	state.setState( NORMAL_MOVING );
	state.addTransitionState( SEEN_WEAK_ENEMY, FOLLOW_MOVING );
	state.addTransitionState( SEEN_STRONG_ENEMY, AVOID_MOVING );

	addState(state);
	//----------------------------------------------------------------------------------------
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Monster::~Monster()
{	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화
//	수정 일자	:	2002-06-29 오전 10:23:11 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	Monster::Init()
{
	// 초기화를 한다.
	BaseChar::Init();

	siFieldMonsterID	=	0;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-06-20 오후 1:34:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	Monster::Init(SI32 sifieldmonsterid, UI16 uiaccount, UI16 uimapid, SI16 six, SI16 siy, LPRECT prcactivityarea, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{	
	if( ( ISMONSTER( uiaccount ) == FALSE ) || ( uiRepresentKind == 0 ) ) 
	{

//		pMyWorld->clServer->WriteInfo( "..\\MonsterInitError.txt", "Account : [ %u ], Kind : [ %u ] ",  uiaccount, uiRepresentKind );
		writeInfoToFile( "MonsterInitError.txt", "Account : [ %u ], Kind : [ %u ] ",  uiaccount, uiRepresentKind );		
	}


	// 초기화 
	normalState();

	SI32					i, j;
	UI16					uiMoveSpeed;
	OnFollowerInfo			FollowerInfo;
	UI16					uiMonsterKind;	
	_ImportantParameta		Parameter;		
	CharHead*				lpCharHead;	
	OnlineFollower			*pFollower;
	SI16					siDropItem[4];
	SI32					siDropPercent[4];

	// 기본 적인 
	CHAR	*pszMonsterName	=	pMyWorld->clSOCharKI.GetNameByString( uiRepresentKind );

	BaseChar::Init( uiaccount, uimapid, six, siy, uiRepresentKind, pszMonsterName );	
	

	// 몬스터의 기본 변수를 초기화 해준다.
	SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );
	//siStatus			=	ON_PLAYER_STATUS_INFIELD;
	siFieldMonsterID	=	sifieldmonsterid;			
	
	// 활동 영역을 저장한다.	
	prcActivityArea		=	prcactivityarea;				
	
	// 클라이언트의 이동 속도를 서버에서의 이동 지연 시간으로 변환한다.
	uiMoveSpeed			=	pMyWorld->clSOCharKI.GetMoveSpeed(uiRepresentKind);
	m_actionDelay		=	SO_CONVERSION_CHARMOVEPSEED(uiMoveSpeed);			
	m_prevActionTime	=   0;	

	// 몬스터의 능력치를 설정한다.
	IFollower.Init();
 
	for(i = 0; i < ON_MAX_CHAR_PER_PLAYER; i++)
	{
		uiMonsterKind	=	uiMonsterKinds[i];

		if(uiMonsterKind != 0)
		{			
			// 유효한 몬스터이다.
			ZeroMemory(&FollowerInfo, sizeof(FollowerInfo));
			
			FollowerInfo.uiKind		=	uiMonsterKind;														// 몬스터의 ID.			

			strncpy(FollowerInfo.Name, pMyWorld->clSOCharKI.GetNameByString(uiMonsterKind), ON_ID_LENGTH);	// 몬스터의 이름.
		
			memcpy(&Parameter, pMyWorld->clSOCharKI.GetParameta(uiMonsterKind), sizeof(_ImportantParameta));
			lpCharHead = pMyWorld->GetCharHeader(FollowerInfo.uiKind);	
			
			Parameter.SetCurExp(GetRequiredExpForLevel(siMonsterLevel[i]));
			Parameter.SetLevel(siMonsterLevel[i]);

			Parameter.SetStr(Parameter.GetStr() + (siMonsterLevel[i] -1 ) );
			Parameter.SetDex(Parameter.GetDex() + (siMonsterLevel[i] -1 ) );
			Parameter.SetVit(Parameter.GetVit() + (siMonsterLevel[i] -1 ) );
			Parameter.SetInt(Parameter.GetInt() + (siMonsterLevel[i] -1 ) );		

			FollowerInfo.siExp					=	Parameter.GetCurExp();
			FollowerInfo.siLevel				=	Parameter.GetLevel();
			FollowerInfo.siStr					=	Parameter.GetStr();
			FollowerInfo.siDex					=	Parameter.GetDex();
			FollowerInfo.siVit					=	Parameter.GetVit();
			FollowerInfo.siInt					=	Parameter.GetInt();							
			FollowerInfo.siLife					=	Parameter.GetLife();
			FollowerInfo.siMana					=	Parameter.GetMana();
			FollowerInfo.siBonus				=	Parameter.GetBonus();
			FollowerInfo.siAC					=	Parameter.GetAC();
			FollowerInfo.siDamageRegist			=	Parameter.DamageResist;
			FollowerInfo.siMagicRegist			=	Parameter.MagicResist;
			
			FollowerInfo.siLiftConstant			=	lpCharHead->siCon;			

			FollowerInfo.siEquipMinDamage		=	Parameter.EquipMinDamage;
			FollowerInfo.siEquipMaxDamage		=	Parameter.EquipMaxDamage;

			// 부하들을 초기화 한다.
			IFollower.InitFollower(i, &FollowerInfo, lpCharHead->siNation, &pMyWorld->clItem);

			// 체력과 마법력을 최대값으로 설정한다.
			pFollower							=	IFollower.GetFollower(i);
			pFollower->ParametaBox.IP.SetLife(pFollower->ParametaBox.GetMaxLife());
			pFollower->ParametaBox.IP.SetMana(pFollower->ParametaBox.GetMaxMana(), 8);			

			// 몬스터가 죽을때 떨어뜨리는 아이템을 설정한다.
			memcpy(siDropItem, pMyWorld->clSOCharKI.GetDropItem(FollowerInfo.uiKind), sizeof(UI16) * 4);
			memcpy( siDropPercent, pMyWorld->clSOCharKI.GetDropPercent( FollowerInfo.uiKind ), sizeof( SI32 ) * 4 );

			// 0. 아이템을 가질 확률로 아이템을 인벤에 추가한다 
			//-----------------------------------------------------------------------------------------------
			for(j = 0; j < 3; j++)
			{
				if( ( lrand() % 100000000L) <= siDropPercent[j] )
				{		
					AddItem(i, siDropItem[j] , 1);
				}
			}		
		}
	}
	
	// 몬스터를 맵에 설정한다.
	if(SetIDOnMap( 13 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "3");	
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "3");	

	if( ( uiAccount != uiaccount ) || ( uiKind != uiRepresentKind ) || ( uiKind == 0 ) ) 
	{
//		pMyWorld->clServer->WriteInfo( "..\\MonsterInitError.txt", "Account : [ %u, %u ], Kind : [ %u, %u ], Name : [ %d, %s ]",  
		writeInfoToFile( "MonsterInitError.txt", "Account : [ %u, %u ], Kind : [ %u, %u ], Name : [ %d, %s ]",  
						uiAccount, uiaccount, uiKind, uiRepresentKind, 
						( pszMonsterName != NULL ) ? strlen( pszMonsterName ) : 0, 
						( pszMonsterName != NULL ) ? pszMonsterName : "Null Point");	
	}

}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해제.
//	수정 일자	:	2002-05-11 오후 3:08:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	Monster::Free(BOOL IsDeadInBattle)
{
	// 몬스터가 죽었다면 현재 자기 필드에게 자신이 죽었다는것을 알려준다.	
	if((siFieldMonsterID != 0) && (IsDeadInBattle == FALSE))
		pMyWorld->clFieldMonster.DeadMonster(siFieldMonsterID);

	// 현재 필드에 있고,
	// 현재 맵에 자기가 심어져 있다면 맵에서 ID를 삭제 한다.
	//if(siStatus == ON_PLAYER_STATUS_INFIELD)
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE )
	{			
		// 맵에서 나를 삭제한다.
		DelIDOnMap( 14 );
	}	

	Init();	
}	



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 ID가 유효한지 본다.(범위를 벗어나지는 않는지)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	Monster::IsValidCharID(UI08 uicharID)
{
	if(uicharID >= 0 && uicharID < ON_MAX_CHAR_PER_PLAYER)
		return TRUE;
	else
		return FALSE;
}


/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 전투중인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	Monster::GetfFight(void)
{
	if(siStatus == ON_PLAYER_STATUS_INBATTLE) return TRUE;
	return FALSE;
}
*/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	몬스터 캐릭터의 정보를 얻어온다.
//	수정 일자	:	2002-08-15 오후 12:58:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	Monster::GetInfo(OnCharMonsterInfo *pMonsterInfo)
{	
	BaseChar::GetInfo((OnCharBaseInfo*)pMonsterInfo);	
	
	// 몬스터의 추가 정보를 얻어온다.
	pMonsterInfo->siFightingPower = GetFightingPower();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	Monster::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	UI16	uiPrevMapID;
	SI32	siPrevX, siPrevY;

	// 이동하려는 맵이 유효하지 않으면 FALSE를 리턴한다.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	return FALSE;				

	// 워프가 실패 할 경우를 대비해서 현재의 맵 ID를 저장한다.
	uiPrevMapID		=	uiMapID;	
	siPrevX			=	siX;
	siPrevY			=	siY;

	if(BaseChar::Warp(uimapid, six, siy) == TRUE)
	{		
		return TRUE;
	}
	else
	{
		// 새로운 지점으로 워프하는데 실패했다면
		// 원래 자리로 다시 워프한다.
		BaseChar::Warp(uiPrevMapID, siPrevX, siPrevY);
		return FALSE;
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투를 준비한다.
//	수정 일자	:	2002-02-27 오후 1:22:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	Monster::PrepareBattle()
{
	// 반드시 전투중에 있어야 한다.
	if(GetBattle() == NULL)
	{		
		char	szbuffer[1024];
				
		sprintf(szbuffer, "몬스터가 전투에 있지도 않음 PrepareBattle [%u]\n", uiAccount);		
		OutputDebugString(szbuffer);		
		return;
	}

	// clFieldMonster에게 전투 중이라는 것을 알려준다.
	pMyWorld->clFieldMonster.StartBattle(siFieldMonsterID);
		
	// 전투를 준비한다.
	BaseChar::PrepareBattle();
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투를 종료했다.
//	수정 일자	:	2002-05-10 오후 3:18:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	Monster::EndBattle()
{
	POINT		ptPos;

	// 반드시 전투중에 있어야 한다.
	if(GetBattle() == NULL)
	{		
		char	szbuffer[1024];
				
		sprintf(szbuffer, "몬스터가 전투에 있지도 않음 EndBattle [%u]\n", uiAccount);		
		OutputDebugString(szbuffer);		
		return;
	}
	

	if( IFollower.GetFollowerNum() == 0)
	{
		Free(TRUE);
	}
	else
	{
		// clFieldMonster에게 전투 종료 했다는 것을 알려준다.
		pMyWorld->clFieldMonster.EndBattle(siFieldMonsterID);
		
		// 다시 필드에 설정하기 위해 현재 위치를 기준으로 자신의 ID를 설정할 위치를 찾는다
		//----------------------------------------------------------------------------------------------
		ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, uiMoveableTileInfo);
		
		if(ptPos.x != -1 && ptPos.y != -1)
		{
			siX	=	SI16(ptPos.x);
			siY	=	SI16(ptPos.y);
			
			// 맵에다가 자기의 아이디를 설정한다.		
			if(SetIDOnMap( 15 ) == FALSE)
				writeInfoToFile("SetIDOnMap Failed.txt", "4");
//				pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "4");
		}
		else
			writeInfoToFile("SearchMap Failed.txt", "1");
//			pMyWorld->clServer->WriteInfo("..\\SearchMap Failed.txt", "1");
		
		// 전투를 종료 한다 
		BaseChar::EndBattle();	


		// 평상시 상태로 돌린다 
		normalState();
	}	
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	몬스터가 소속해 있는 필드의 ID를 얻어온다.
//	수정 일자	:	2002-06-17 오전 9:47:32 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	Monster::GetFieldID()
{
	return	siFieldMonsterID;
}






//-----------------------------------------------------------------------------------------------------------------------
//
//										몬스터 상태 처리 관련 부분 
//
//-----------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 몬스터가 현재 필드위에 위치하고 있는가 
//-----------------------------------------------------------------------------------------------------------------------
bool	Monster::isMonsterInField()
{
	return CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );	
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 주어진 위치가 자신의 활동 영역 안에 있는지 검사 한다 
//-----------------------------------------------------------------------------------------------------------------------
bool	Monster::isPosInMyArea(int x, int y)
{		
	POINT pos;	
	pos.x = x;	pos.y = y;
				
	// 활동 영역이 NULL인 경우 제한된 영역이 없음 
	if( prcActivityArea == NULL )			return	TRUE;
	if( PtInRect(prcActivityArea, pos) )	return	TRUE;

	return FALSE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 목표로 삼은 적을 쫓아 다음 위치를 얻는다
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getTargetNextPos()
{
	POINT nextPos;
	
	if( m_targetEnemy )
	{				
		nextPos.x = siX;
		nextPos.y = siY;

		if( m_targetEnemy->siX > siX )			nextPos.x  = siX + 1;
		else									nextPos.x  = siX - 1;

		if( m_targetEnemy->siY > siY )			nextPos.y  = siY + 1;
		else									nextPos.y  = siY - 1;		

		if( nextPos.x < 0 ) nextPos.x = 0;
		if( nextPos.y < 0 ) nextPos.y = 0;

		// 막힌 곳일 경우 비어 있는 곳을 찾아 이동한다 
		if( pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) 
			nextPos = getEmptyNextPos();
	}
	// 대상이 없을 경우에는 랜덤한 이동을 따른다 
	else 
		nextPos = getRandomNextPos();

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 목표로 삼은 적을 피하여 다음 위치를 얻는다 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getAvoidNextPos()
{
	POINT nextPos;
	
	if( m_targetEnemy )
	{				
		nextPos.x = siX;
		nextPos.y = siY;

		if( m_targetEnemy->siX > siX )			nextPos.x  = siX - 1;
		else									nextPos.x  = siX + 1;

		if( m_targetEnemy->siY > siY )			nextPos.y  = siY - 1;
		else									nextPos.y  = siY + 1;		

		if( nextPos.x < 0 ) nextPos.x = 0;
		if( nextPos.y < 0 ) nextPos.y = 0;

		// 막힌 곳일 경우 비어 있는 곳을 찾아 이동한다 
		if( pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) 
			nextPos = getEmptyNextPos();
	}
	// 대상이 없을 경우에는 랜덤한 이동을 따른다 
	else 
		nextPos = getRandomNextPos();

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 근처에 비어 있는 위치를 얻는다 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getEmptyNextPos()
{		
	// 50% 확률로 시작 지점을 바꾼다 
	int 	 entryPoint = rand() % 100 < 50 ? 7 : 0;
	int		 i = entryPoint;
	
	while( TRUE )
	{		
		// 방향을 선택한다 
		int	direction;
		switch(i)				
		{							
			case 0: direction		=	ON_EAST;				break;	
			case 1:	direction		=	ON_EAST | ON_SOUTH;		break;			
			case 2:	direction		=	ON_SOUTH;				break;
			case 3:	direction		=	ON_WEST | ON_SOUTH;		break;
			case 4:	direction		=	ON_WEST;				break;
			case 5:	direction		=	ON_WEST | ON_NORTH;		break;
			case 6:	direction		=	ON_NORTH;				break;								
			case 7:	direction		=	ON_EAST | ON_NORTH;		break;
		}
								

		// 이동할 좌표를 구한다 
		POINT  nextPos;		
		switch(direction)
		{
			case ON_WEST:		
				nextPos.x	=	siX - 1;			
				nextPos.y	=	siY;
				break;
			case ON_EAST:				
				nextPos.x	=	siX + 1;			
				nextPos.y	=	siY;
				break;
			case ON_NORTH:					
				nextPos.x	=	siX;			
				nextPos.y	=	siY - 1;
				break;
			case ON_SOUTH:
				nextPos.x	=	siX;
				nextPos.y	=	siY + 1;
				break;
			case ON_WEST | ON_NORTH:
				nextPos.x	=	siX - 1;			
				nextPos.y	=	siY - 1;
				break;
			case ON_WEST | ON_SOUTH:					
				nextPos.x	=	siX - 1;			
				nextPos.y	=	siY + 1;
				break;
			case ON_EAST | ON_NORTH:					
				nextPos.x	=	siX + 1;			
				nextPos.y	=	siY + 1;
				break;
			case ON_EAST | ON_SOUTH:					
				nextPos.x	=	siX + 1;			
				nextPos.y	=	siY - 1;
				break;
		}	

		if( nextPos.x < 0 ) nextPos.x = 0;
		if( nextPos.y < 0 ) nextPos.y = 0;

		// 막히지 않은 곳이라면 중지 한다 
		if( !pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) return nextPos;		
		
		if( entryPoint == 0 ) if( ++i > 7 ) break;
		if( entryPoint == 7 ) if( --i < 0 ) break;
	}

	// 비어 있는 곳을 찾지 못했다면 현재 위치를 리턴한다 
	POINT curPos;
	curPos.x = siX;
	curPos.y = siY;
	
	return curPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 랜덤하게 다음에 이동할 위치를 얻는다 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getRandomNextPos()
{
	// 랜덤하게 이동할 방향을 정한다 	
	int	direction;
	switch(rand() % 8)				
	{					
		case 0: direction		=	ON_EAST;				break;	
		case 1:	direction		=	ON_EAST | ON_SOUTH;		break;			
		case 2:	direction		=	ON_SOUTH;				break;
		case 3:	direction		=	ON_WEST | ON_SOUTH;		break;
		case 4:	direction		=	ON_WEST;				break;
		case 5:	direction		=	ON_WEST | ON_NORTH;		break;
		case 6:	direction		=	ON_NORTH;				break;								
		case 7:	direction		=	ON_EAST | ON_NORTH;		break;
	}
							
	// 이동할 좌표를 구한다 
	POINT nextPos;	
	switch(direction)
	{
		case ON_WEST:		
			nextPos.x	=	siX - 1;			
			nextPos.y	=	siY;
			break;
		case ON_EAST:				
			nextPos.x	=	siX + 1;			
			nextPos.y	=	siY;
			break;
		case ON_NORTH:					
			nextPos.x	=	siX;			
			nextPos.y	=	siY - 1;
			break;
		case ON_SOUTH:
			nextPos.x	=	siX;
			nextPos.y	=	siY + 1;
			break;
		case ON_WEST | ON_NORTH:
			nextPos.x	=	siX - 1;			
			nextPos.y	=	siY - 1;
			break;
		case ON_WEST | ON_SOUTH:					
			nextPos.x	=	siX - 1;			
			nextPos.y	=	siY + 1;
			break;
		case ON_EAST | ON_NORTH:					
			nextPos.x	=	siX + 1;			
			nextPos.y	=	siY + 1;
			break;
		case ON_EAST | ON_SOUTH:					
			nextPos.x	=	siX + 1;			
			nextPos.y	=	siY - 1;
			break;
	}
		
	if( nextPos.x < 0 ) nextPos.x = 0;	
	if( nextPos.y < 0 ) nextPos.y = 0;

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 주위에 적이 있는지 감지 한다 
//-----------------------------------------------------------------------------------------------------------------------
SOPlayer* Monster::checkEnemyToArround()
{	
	WORD		 nearPlayerID;

	// 주위 3칸을 검색 
	if( pMyWorld->clISOMap.GetCharUniID(uiMapID, 
									    siX - 3, siY - 3, siX + 3, siY + 3, 
									    &nearPlayerID, 1, ON_MAP_CHARID_PLAYER) >= 1 )
	{		
		SOPlayer *enemy = pMyWorld->pPlayerOnAccount[nearPlayerID];
			
		if( enemy )
		{
			// 적이 전투대기중일때만 공격한다 
			// 자신의 영역안에 있는 적인 경우에만 유효하다 
			if( enemy->GetBattle() && isPosInMyArea( enemy->siX, enemy->siY ) ) return enemy;
		}
	}

	return NULL;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 행동 지연 시간을 검사 한다 
//-----------------------------------------------------------------------------------------------------------------------
bool Monster::checkActionDelay()
{
	if( (pMyWorld->dwtimeGetTime - m_prevActionTime) > m_actionDelay )
	{
		// 행동 해야 할 시간이다 
		m_prevActionTime = pMyWorld->dwtimeGetTime;
		return TRUE;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 평상시 상태로 돌린다 
//-----------------------------------------------------------------------------------------------------------------------
void	Monster::normalState()
{	
	m_targetEnemy = NULL;	
	setCurState( NORMAL_MOVING );
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 대상으로 한 적을 공격한다 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Attack()
{							
	if(m_targetEnemy)
	{
		// 근접해 있을 경우 
		if(max( abs(siX - m_targetEnemy->siX), abs(siY - m_targetEnemy->siY)) <= 1)
		{	
			// 20%의 확률로 
			if( rand() % 100 <= 20 )
			{
				if( PickBattleWithEnemy(m_targetEnemy->uiAccount) )
				{					
					m_targetEnemy->SendFieldMsg(ON_RELEASEME, SO_SFM_ONLYME, LPVOID(uiAccount));
				}
			}
		}
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 실제로 몬스터의 위치를 이동한다 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Move(POINT movePos)
{
	// 새로운 좌표가 이동 가능한 영역 일경우 
	if( isPosInMyArea( movePos.x , movePos.y ) )
	{
		pMyWorld->clISOMap.MoveChar(this, movePos.x, movePos.y,FALSE, uiMoveableTileInfo);
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 적을 놓쳤을 경우 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::lostEnemy()
{				
	if(m_targetEnemy)
	{
		// 화면에 느낌표 표시를 해제 한다 
		m_targetEnemy->SendFieldMsg(ON_RELEASEME, SO_SFM_ONLYME, LPVOID(uiAccount));			
		
		// 평상시 상태로 돌린다 
		normalState();		
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 적을 발견 했을 경우 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::seenEnemy(SOPlayer* enemy)
{	
	if( enemy )
	{
		m_targetEnemy = enemy;		

		// 발견한 상대의 전투력을 살피고 이에 맞추어 대응한다 
		if( enemy->GetFightingPower() < GetFightingPower() * 2 )
		{			
			m_targetEnemy->SendFieldMsg(ON_TARGETME, SO_SFM_ONLYME, LPVOID(uiAccount));																		
			transitionState( SEEN_WEAK_ENEMY );		
		}
		else 
		{
			transitionState( SEEN_WEAK_ENEMY );		
		}
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 몬스터 메인 루프 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Action()
{		
	if( GetBattle()			!= NULL  )		return;
	if( isMonsterInField()  == FALSE )		return;
	if( checkActionDelay()  == FALSE )		return;


	switch( getCurState() )
	{
		// 일반 이동 
		case NORMAL_MOVING:
			{
				// 이동 
				POINT nextPos = getRandomNextPos();								
				Move( nextPos );
				
				// 주위를 검사한다 
				SOPlayer* enemy;
				if( enemy = checkEnemyToArround() ) 
				{														
					seenEnemy(enemy);																			
				}						
			}
			break;

		// 추적 이동  
		case FOLLOW_MOVING:
			{	
				SOPlayer* enemy = checkEnemyToArround();
				
				// 주위에 적이 없거나 이전에 추적하던 적이 아닐 경우 
				if( !enemy || enemy != m_targetEnemy )
				{					
					lostEnemy();
				}				
				else					
				{						
					POINT nextPos = getTargetNextPos();
					Move( nextPos );					

					Attack();
				}
			}
			break;		

		// 회피 이동 
		case AVOID_MOVING:
			{
				// 일단 도망치고 
				POINT nextPos = getAvoidNextPos();
				Move( nextPos );					

				// 랜덤 하게 주위를 살핀다 
				if( rand() % 100 < 30 )
				{
					SOPlayer* enemy = checkEnemyToArround();

					// 주위에 적이 없거나 이전의 적이 아닐 경우 
					if( !enemy || enemy != m_targetEnemy )
					{					
						lostEnemy();
					}				
				}
			}
			break;
					
		default:
			break;
	}

}