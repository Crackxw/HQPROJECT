#include "SOMain.h"
#include "SOWorld.h"

#include "SOMonster.h"
#include "SORand.h"


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
Monster::Monster()
{
	siX					=	0;
	siY					=	0;

	uiKind				=	0;
	uiAccount			=	0;	
	uiGuildID			=	0;
	siClassInGuild		=	0;


	// ���� �ʱ�ȭ 
	//----------------------------------------------------------------------------------------
	
	// ���¸� ��� �Ѵ� 
	CState					state;
	state.setState( NORMAL_MOVING );
	state.addTransitionState( SEEN_WEAK_ENEMY, FOLLOW_MOVING );
	state.addTransitionState( SEEN_STRONG_ENEMY, AVOID_MOVING );

	addState(state);
	//----------------------------------------------------------------------------------------
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
Monster::~Monster()
{	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ
//	���� ����	:	2002-06-29 ���� 10:23:11 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	Monster::Init()
{
	// �ʱ�ȭ�� �Ѵ�.
	BaseChar::Init();

	siFieldMonsterID	=	0;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-06-20 ���� 1:34:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	Monster::Init(SI32 sifieldmonsterid, UI16 uiaccount, UI16 uimapid, SI16 six, SI16 siy, LPRECT prcactivityarea, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{	
	if( ( ISMONSTER( uiaccount ) == FALSE ) || ( uiRepresentKind == 0 ) ) 
	{

//		pMyWorld->clServer->WriteInfo( "..\\MonsterInitError.txt", "Account : [ %u ], Kind : [ %u ] ",  uiaccount, uiRepresentKind );
		writeInfoToFile( "MonsterInitError.txt", "Account : [ %u ], Kind : [ %u ] ",  uiaccount, uiRepresentKind );		
	}


	// �ʱ�ȭ 
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

	// �⺻ ���� 
	CHAR	*pszMonsterName	=	pMyWorld->clSOCharKI.GetNameByString( uiRepresentKind );

	BaseChar::Init( uiaccount, uimapid, six, siy, uiRepresentKind, pszMonsterName );	
	

	// ������ �⺻ ������ �ʱ�ȭ ���ش�.
	SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );
	//siStatus			=	ON_PLAYER_STATUS_INFIELD;
	siFieldMonsterID	=	sifieldmonsterid;			
	
	// Ȱ�� ������ �����Ѵ�.	
	prcActivityArea		=	prcactivityarea;				
	
	// Ŭ���̾�Ʈ�� �̵� �ӵ��� ���������� �̵� ���� �ð����� ��ȯ�Ѵ�.
	uiMoveSpeed			=	pMyWorld->clSOCharKI.GetMoveSpeed(uiRepresentKind);
	m_actionDelay		=	SO_CONVERSION_CHARMOVEPSEED(uiMoveSpeed);			
	m_prevActionTime	=   0;	

	// ������ �ɷ�ġ�� �����Ѵ�.
	IFollower.Init();
 
	for(i = 0; i < ON_MAX_CHAR_PER_PLAYER; i++)
	{
		uiMonsterKind	=	uiMonsterKinds[i];

		if(uiMonsterKind != 0)
		{			
			// ��ȿ�� �����̴�.
			ZeroMemory(&FollowerInfo, sizeof(FollowerInfo));
			
			FollowerInfo.uiKind		=	uiMonsterKind;														// ������ ID.			

			strncpy(FollowerInfo.Name, pMyWorld->clSOCharKI.GetNameByString(uiMonsterKind), ON_ID_LENGTH);	// ������ �̸�.
		
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

			// ���ϵ��� �ʱ�ȭ �Ѵ�.
			IFollower.InitFollower(i, &FollowerInfo, lpCharHead->siNation, &pMyWorld->clItem);

			// ü�°� �������� �ִ밪���� �����Ѵ�.
			pFollower							=	IFollower.GetFollower(i);
			pFollower->ParametaBox.IP.SetLife(pFollower->ParametaBox.GetMaxLife());
			pFollower->ParametaBox.IP.SetMana(pFollower->ParametaBox.GetMaxMana(), 8);			

			// ���Ͱ� ������ ����߸��� �������� �����Ѵ�.
			memcpy(siDropItem, pMyWorld->clSOCharKI.GetDropItem(FollowerInfo.uiKind), sizeof(UI16) * 4);
			memcpy( siDropPercent, pMyWorld->clSOCharKI.GetDropPercent( FollowerInfo.uiKind ), sizeof( SI32 ) * 4 );

			// 0. �������� ���� Ȯ���� �������� �κ��� �߰��Ѵ� 
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
	
	// ���͸� �ʿ� �����Ѵ�.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����.
//	���� ����	:	2002-05-11 ���� 3:08:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	Monster::Free(BOOL IsDeadInBattle)
{
	// ���Ͱ� �׾��ٸ� ���� �ڱ� �ʵ忡�� �ڽ��� �׾��ٴ°��� �˷��ش�.	
	if((siFieldMonsterID != 0) && (IsDeadInBattle == FALSE))
		pMyWorld->clFieldMonster.DeadMonster(siFieldMonsterID);

	// ���� �ʵ忡 �ְ�,
	// ���� �ʿ� �ڱⰡ �ɾ��� �ִٸ� �ʿ��� ID�� ���� �Ѵ�.
	//if(siStatus == ON_PLAYER_STATUS_INFIELD)
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE )
	{			
		// �ʿ��� ���� �����Ѵ�.
		DelIDOnMap( 14 );
	}	

	Init();	
}	



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ ID�� ��ȿ���� ����.(������ ������� �ʴ���)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	Monster::IsValidCharID(UI08 uicharID)
{
	if(uicharID >= 0 && uicharID < ON_MAX_CHAR_PER_PLAYER)
		return TRUE;
	else
		return FALSE;
}


/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���������� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	Monster::GetfFight(void)
{
	if(siStatus == ON_PLAYER_STATUS_INBATTLE) return TRUE;
	return FALSE;
}
*/


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ĳ������ ������ ���´�.
//	���� ����	:	2002-08-15 ���� 12:58:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	Monster::GetInfo(OnCharMonsterInfo *pMonsterInfo)
{	
	BaseChar::GetInfo((OnCharBaseInfo*)pMonsterInfo);	
	
	// ������ �߰� ������ ���´�.
	pMonsterInfo->siFightingPower = GetFightingPower();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	Monster::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	UI16	uiPrevMapID;
	SI32	siPrevX, siPrevY;

	// �̵��Ϸ��� ���� ��ȿ���� ������ FALSE�� �����Ѵ�.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	return FALSE;				

	// ������ ���� �� ��츦 ����ؼ� ������ �� ID�� �����Ѵ�.
	uiPrevMapID		=	uiMapID;	
	siPrevX			=	siX;
	siPrevY			=	siY;

	if(BaseChar::Warp(uimapid, six, siy) == TRUE)
	{		
		return TRUE;
	}
	else
	{
		// ���ο� �������� �����ϴµ� �����ߴٸ�
		// ���� �ڸ��� �ٽ� �����Ѵ�.
		BaseChar::Warp(uiPrevMapID, siPrevX, siPrevY);
		return FALSE;
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �غ��Ѵ�.
//	���� ����	:	2002-02-27 ���� 1:22:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	Monster::PrepareBattle()
{
	// �ݵ�� �����߿� �־�� �Ѵ�.
	if(GetBattle() == NULL)
	{		
		char	szbuffer[1024];
				
		sprintf(szbuffer, "���Ͱ� ������ ������ ���� PrepareBattle [%u]\n", uiAccount);		
		OutputDebugString(szbuffer);		
		return;
	}

	// clFieldMonster���� ���� ���̶�� ���� �˷��ش�.
	pMyWorld->clFieldMonster.StartBattle(siFieldMonsterID);
		
	// ������ �غ��Ѵ�.
	BaseChar::PrepareBattle();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����ߴ�.
//	���� ����	:	2002-05-10 ���� 3:18:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	Monster::EndBattle()
{
	POINT		ptPos;

	// �ݵ�� �����߿� �־�� �Ѵ�.
	if(GetBattle() == NULL)
	{		
		char	szbuffer[1024];
				
		sprintf(szbuffer, "���Ͱ� ������ ������ ���� EndBattle [%u]\n", uiAccount);		
		OutputDebugString(szbuffer);		
		return;
	}
	

	if( IFollower.GetFollowerNum() == 0)
	{
		Free(TRUE);
	}
	else
	{
		// clFieldMonster���� ���� ���� �ߴٴ� ���� �˷��ش�.
		pMyWorld->clFieldMonster.EndBattle(siFieldMonsterID);
		
		// �ٽ� �ʵ忡 �����ϱ� ���� ���� ��ġ�� �������� �ڽ��� ID�� ������ ��ġ�� ã�´�
		//----------------------------------------------------------------------------------------------
		ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, uiMoveableTileInfo);
		
		if(ptPos.x != -1 && ptPos.y != -1)
		{
			siX	=	SI16(ptPos.x);
			siY	=	SI16(ptPos.y);
			
			// �ʿ��ٰ� �ڱ��� ���̵� �����Ѵ�.		
			if(SetIDOnMap( 15 ) == FALSE)
				writeInfoToFile("SetIDOnMap Failed.txt", "4");
//				pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "4");
		}
		else
			writeInfoToFile("SearchMap Failed.txt", "1");
//			pMyWorld->clServer->WriteInfo("..\\SearchMap Failed.txt", "1");
		
		// ������ ���� �Ѵ� 
		BaseChar::EndBattle();	


		// ���� ���·� ������ 
		normalState();
	}	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���Ͱ� �Ҽ��� �ִ� �ʵ��� ID�� ���´�.
//	���� ����	:	2002-06-17 ���� 9:47:32 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	Monster::GetFieldID()
{
	return	siFieldMonsterID;
}






//-----------------------------------------------------------------------------------------------------------------------
//
//										���� ���� ó�� ���� �κ� 
//
//-----------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���Ͱ� ���� �ʵ����� ��ġ�ϰ� �ִ°� 
//-----------------------------------------------------------------------------------------------------------------------
bool	Monster::isMonsterInField()
{
	return CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );	
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �־��� ��ġ�� �ڽ��� Ȱ�� ���� �ȿ� �ִ��� �˻� �Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
bool	Monster::isPosInMyArea(int x, int y)
{		
	POINT pos;	
	pos.x = x;	pos.y = y;
				
	// Ȱ�� ������ NULL�� ��� ���ѵ� ������ ���� 
	if( prcActivityArea == NULL )			return	TRUE;
	if( PtInRect(prcActivityArea, pos) )	return	TRUE;

	return FALSE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ��ǥ�� ���� ���� �Ѿ� ���� ��ġ�� ��´�
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

		// ���� ���� ��� ��� �ִ� ���� ã�� �̵��Ѵ� 
		if( pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) 
			nextPos = getEmptyNextPos();
	}
	// ����� ���� ��쿡�� ������ �̵��� ������ 
	else 
		nextPos = getRandomNextPos();

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ��ǥ�� ���� ���� ���Ͽ� ���� ��ġ�� ��´� 
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

		// ���� ���� ��� ��� �ִ� ���� ã�� �̵��Ѵ� 
		if( pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) 
			nextPos = getEmptyNextPos();
	}
	// ����� ���� ��쿡�� ������ �̵��� ������ 
	else 
		nextPos = getRandomNextPos();

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��ó�� ��� �ִ� ��ġ�� ��´� 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getEmptyNextPos()
{		
	// 50% Ȯ���� ���� ������ �ٲ۴� 
	int 	 entryPoint = rand() % 100 < 50 ? 7 : 0;
	int		 i = entryPoint;
	
	while( TRUE )
	{		
		// ������ �����Ѵ� 
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
								

		// �̵��� ��ǥ�� ���Ѵ� 
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

		// ������ ���� ���̶�� ���� �Ѵ� 
		if( !pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) return nextPos;		
		
		if( entryPoint == 0 ) if( ++i > 7 ) break;
		if( entryPoint == 7 ) if( --i < 0 ) break;
	}

	// ��� �ִ� ���� ã�� ���ߴٸ� ���� ��ġ�� �����Ѵ� 
	POINT curPos;
	curPos.x = siX;
	curPos.y = siY;
	
	return curPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ϰ� ������ �̵��� ��ġ�� ��´� 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getRandomNextPos()
{
	// �����ϰ� �̵��� ������ ���Ѵ� 	
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
							
	// �̵��� ��ǥ�� ���Ѵ� 
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
//	Desc : ������ ���� �ִ��� ���� �Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
SOPlayer* Monster::checkEnemyToArround()
{	
	WORD		 nearPlayerID;

	// ���� 3ĭ�� �˻� 
	if( pMyWorld->clISOMap.GetCharUniID(uiMapID, 
									    siX - 3, siY - 3, siX + 3, siY + 3, 
									    &nearPlayerID, 1, ON_MAP_CHARID_PLAYER) >= 1 )
	{		
		SOPlayer *enemy = pMyWorld->pPlayerOnAccount[nearPlayerID];
			
		if( enemy )
		{
			// ���� ����������϶��� �����Ѵ� 
			// �ڽ��� �����ȿ� �ִ� ���� ��쿡�� ��ȿ�ϴ� 
			if( enemy->GetBattle() && isPosInMyArea( enemy->siX, enemy->siY ) ) return enemy;
		}
	}

	return NULL;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ൿ ���� �ð��� �˻� �Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
bool Monster::checkActionDelay()
{
	if( (pMyWorld->dwtimeGetTime - m_prevActionTime) > m_actionDelay )
	{
		// �ൿ �ؾ� �� �ð��̴� 
		m_prevActionTime = pMyWorld->dwtimeGetTime;
		return TRUE;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ���·� ������ 
//-----------------------------------------------------------------------------------------------------------------------
void	Monster::normalState()
{	
	m_targetEnemy = NULL;	
	setCurState( NORMAL_MOVING );
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������� �� ���� �����Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Attack()
{							
	if(m_targetEnemy)
	{
		// ������ ���� ��� 
		if(max( abs(siX - m_targetEnemy->siX), abs(siY - m_targetEnemy->siY)) <= 1)
		{	
			// 20%�� Ȯ���� 
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
//	Desc : ������ ������ ��ġ�� �̵��Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Move(POINT movePos)
{
	// ���ο� ��ǥ�� �̵� ������ ���� �ϰ�� 
	if( isPosInMyArea( movePos.x , movePos.y ) )
	{
		pMyWorld->clISOMap.MoveChar(this, movePos.x, movePos.y,FALSE, uiMoveableTileInfo);
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ������ ��� 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::lostEnemy()
{				
	if(m_targetEnemy)
	{
		// ȭ�鿡 ����ǥ ǥ�ø� ���� �Ѵ� 
		m_targetEnemy->SendFieldMsg(ON_RELEASEME, SO_SFM_ONLYME, LPVOID(uiAccount));			
		
		// ���� ���·� ������ 
		normalState();		
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� �߰� ���� ��� 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::seenEnemy(SOPlayer* enemy)
{	
	if( enemy )
	{
		m_targetEnemy = enemy;		

		// �߰��� ����� �������� ���ǰ� �̿� ���߾� �����Ѵ� 
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
//	Desc : ���� ���� ���� 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Action()
{		
	if( GetBattle()			!= NULL  )		return;
	if( isMonsterInField()  == FALSE )		return;
	if( checkActionDelay()  == FALSE )		return;


	switch( getCurState() )
	{
		// �Ϲ� �̵� 
		case NORMAL_MOVING:
			{
				// �̵� 
				POINT nextPos = getRandomNextPos();								
				Move( nextPos );
				
				// ������ �˻��Ѵ� 
				SOPlayer* enemy;
				if( enemy = checkEnemyToArround() ) 
				{														
					seenEnemy(enemy);																			
				}						
			}
			break;

		// ���� �̵�  
		case FOLLOW_MOVING:
			{	
				SOPlayer* enemy = checkEnemyToArround();
				
				// ������ ���� ���ų� ������ �����ϴ� ���� �ƴ� ��� 
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

		// ȸ�� �̵� 
		case AVOID_MOVING:
			{
				// �ϴ� ����ġ�� 
				POINT nextPos = getAvoidNextPos();
				Move( nextPos );					

				// ���� �ϰ� ������ ���ɴ� 
				if( rand() % 100 < 30 )
				{
					SOPlayer* enemy = checkEnemyToArround();

					// ������ ���� ���ų� ������ ���� �ƴ� ��� 
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