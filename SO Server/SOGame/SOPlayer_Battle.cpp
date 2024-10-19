#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ��û�Ѵ�.
//	���� ����	:	2002-02-21 ���� 9:35:20 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::RecvBattle()
{
	OnReqBattleMsg			*pReqBattleMsg;

	// ������ ��û�Ѵ�.
	pReqBattleMsg	=	(OnReqBattleMsg*)cMsg;	

	if(PickBattleWithEnemy(pReqBattleMsg->uiCharID) == FALSE)
	{
		// ������ �Ŵµ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_BATTLE, SO_SFM_ONLYME, LPVOID(ON_RET_BATTLE_NO));		
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �ٽ� �¶������� ���ư���.
//	���� ����	:	2002-05-14 ���� 2:06:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReturnOnline()
{
	UI16							uiCharFItemInfo[2];			// 0���� ĳ���� ��, 1���� ������ ��,
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
		// ����� ��ġ�� ������ �����ϰ� ���� ��´�.		
		if( !IsEvent() )	ChangeSatiety(-IFollower.GetUseSatietyWarAfter());

		// �̺�Ʈ�߿��� ������ ���� �� ���ΰ��� �뺴�� ü�°� ����, ������ ���� Ǯ�� ä������.
		if( IsEvent() )
		{
			IFollower.SetFullLifeOfAllChar();
			IFollower.SetFullManaOfAllChar();
			ChangeSatiety( ON_MAX_HEALTH );		// ������ �ִ�
		}

		// �켱 �ʿ��� ���� ��ǥ�� ã�´�. 
		// ���� ��ġ�� �������� �ʿ� ���� ��ǥ�� ���´�.
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

			// ĳ������ ��ġ�� �����Ѵ�.
			siX				=	SI16(ptPlayerPos.x);
			siY				=	SI16(ptPlayerPos.y);
		}

		if(bSearchMap == TRUE)
		{
			// �ʿ� ĳ���� ID�� �����Ѵ�.		
			if(SetIDOnMap( 3 ) == FALSE)
				writeInfoToFile("SetIDOnMap Failed.txt", "5");
//				pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "5");
			
			// ������ ĳ���Ϳ� �������� ������ ���´�.
			uiCharFItemInfo[0]	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_FIELD);
			uiCharFItemInfo[1]	=	pMyWorld->clISOMap.GetNearSectorItemList(uiMapID, siX, siY, FieldItemInfo);		


			// �켱 ������ �� ������ ĳ���Ϳ� �������� ������ �����ش�.
			SendFieldMsg(ON_RESPONSE_RETURNONLINE, SO_SFM_ONLYME, 
							LPVOID(ON_RET_RETURNONLINE_FIELD),
							LPVOID(uiCharFItemInfo), 
							LPVOID(uiCharUniIDs), 
							LPVOID(FieldItemInfo));
		}				
	}
}
