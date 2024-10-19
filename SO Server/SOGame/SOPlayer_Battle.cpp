#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투를 요청한다.
//	수정 일자	:	2002-02-21 오전 9:35:20 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::RecvBattle()
{
	OnReqBattleMsg			*pReqBattleMsg;

	// 전투를 요청한다.
	pReqBattleMsg	=	(OnReqBattleMsg*)cMsg;	

	if(PickBattleWithEnemy(pReqBattleMsg->uiCharID) == FALSE)
	{
		// 전투를 거는데 실패했다.
		SendFieldMsg(ON_RESPONSE_BATTLE, SO_SFM_ONLYME, LPVOID(ON_RET_BATTLE_NO));		
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투를 끝내고 다시 온라인으로 돌아간다.
//	수정 일자	:	2002-05-14 오전 2:06:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReturnOnline()
{
	UI16							uiCharFItemInfo[2];			// 0번은 캐릭터 수, 1번은 아이템 수,
	UI16							uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	OnlineReceiveFieldItemInfo		FieldItemInfo[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	POINT							ptPlayerPos;
	BOOL							bSearchMap;			

	if( IFollower.GetFollowerParameterLife(0) == 0 && !IsEvent() )
	{
		SendDeadPlayerToVillageHospital();		
	}
	else
	{
		// 배고픔 수치를 전투를 종료하고 나서 깎는다.		
		if( !IsEvent() )	ChangeSatiety(-IFollower.GetUseSatietyWarAfter());

		// 이벤트중에는 전투가 끝난 후 주인공과 용병의 체력과 마법, 포만감 등이 풀로 채워진다.
		if( IsEvent() )
		{
			IFollower.SetFullLifeOfAllChar();
			IFollower.SetFullManaOfAllChar();
			ChangeSatiety( ON_MAX_HEALTH );		// 포만감 최대
		}

		// 우선 맵에서 나올 좌표를 찾는다. 
		// 현재 위치를 기준으로 맵에 심을 좌표를 얻어온다.
		ptPlayerPos		=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, uiMoveableTileInfo);		

		if((ptPlayerPos.x == -1) && (ptPlayerPos.y == -1))
		{
			bSearchMap = FALSE;
//			pMyWorld->clServer->WriteInfo("..\\__SearchMap Failed.txt", "[uiMapID : %2d]  [siX : %2d]  [siY : %2d]  [uiMoveableTileInfo : %d]", uiMapID, siX, siY, uiMoveableTileInfo );
			writeInfoToFile("__SearchMap Failed.txt", "[uiMapID : %2d]  [siX : %2d]  [siY : %2d]  [uiMoveableTileInfo : %d]", uiMapID, siX, siY, uiMoveableTileInfo );
		}
		else
		{
			bSearchMap	=	TRUE;

			// 캐릭터의 위치를 수정한다.
			siX				=	SI16(ptPlayerPos.x);
			siY				=	SI16(ptPlayerPos.y);
		}

		if(bSearchMap == TRUE)
		{
			// 맵에 캐릭터 ID를 설정한다.		
			if(SetIDOnMap( 3 ) == FALSE)
				writeInfoToFile("SetIDOnMap Failed.txt", "5");
//				pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "5");
			
			// 주위에 캐릭터와 아이템의 정보를 얻어온다.
			uiCharFItemInfo[0]	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
			uiCharFItemInfo[1]	=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);		


			// 우선 나에게 내 주위의 캐릭터와 아이템의 정보를 보내준다.
			SendFieldMsg(ON_RESPONSE_RETURNONLINE, SO_SFM_ONLYME, 
							LPVOID(ON_RET_RETURNONLINE_FIELD),
							LPVOID(uiCharFItemInfo), 
							LPVOID(uiCharUniIDs), 
							LPVOID(FieldItemInfo));
		}				
	}
}
