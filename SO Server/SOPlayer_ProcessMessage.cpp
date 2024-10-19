#include "SOMain.h"
#include "SOWorld.h"
#include "Skill.h"
#include "SkillMgr.h"
#include "SOPlayer.h"
#include "OnlineGameMsg.h"
#include "Game.h"
#include "Element.h"
#include "Battle.h"
#include "BattleMgr.h"
#include "XPlayer.h"
#include "BlackPig.h"

#include "AuctionList.h"
#include "AuctionMarket.h"
#include "AuctionMarketProc.h"

#include "Smith.h"

#include "..\\XCommon\\JXPacket.h"
#include "..\\JWCommon\\JWSocket.h"
#include "..\\JWCommon\\JWRecvBuffer.h"
#include "..\\JWCommon\\JWSendBuffer.h"
#include "../SCCommon/GSCProtocol.h"		// 2003.07.8 일 추가함 , 정성운
#include "../SCCommon/IOnlineFollower.h"	// 2003.07.8 일 추가함 , 정성운
#include "../party/IPartyMgr.h"

#include <assert.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 모든 메세지를 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOPlayer::ProcessMessage()
{
	OnMsg		*pMsg;
	BOOL		bProcessMessage = FALSE;
	DWORD		dwCmd;


	CBlackPig	encr;

	// 플레이어가 로그인된 상태일때
	if(uiState == ON_STATE_LOGIN)
	{

		if( GetTickCount() - m_lastTick > 150000 ) LogOut();

		CheckGameGuard();

		pMsg = (OnMsg *)m_pXPlayer->GetSocket()->GetRecvBuffer()->GetFirstPacket();

		if( pMsg == NULL ) return;

		siMsgLength = *(UI16 *)pMsg - 3;
	
		if( siMsgLength < 4 || siMsgLength > 30000 ) {

			m_pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();

			m_pXPlayer->m_pSOPlayer->LogOut();

			return;
		}

		dwCmd = *(DWORD *)( (unsigned char *)pMsg + 3 );

		encr.SetKey( m_pXPlayer->m_cKey1, m_pXPlayer->m_cKey2 );

		encr.SetAutoKey( TRUE );

		if( encr.Decode( IN (unsigned char *)pMsg, OUT (unsigned char *)pmsg, m_pXPlayer->m_dwRoundKey)
			== FALSE ) {
			
			m_pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();

			m_pXPlayer->m_pSOPlayer->LogOut();

			return;
		}

		encr.GetKey( (unsigned char &)m_pXPlayer->m_cKey1, (unsigned char &)m_pXPlayer->m_cKey2 );

/*
		if( ( dwCmd >= 10050 && dwCmd <= 10300 ) ||
			dwCmd == ON_REQUEST_FARM_WORK ) {
			// 전투 패킷일 경우에만 암호화 해제
			// 작업량 패킷도 암호화 해제 

			encr.SetKey( m_pXPlayer->m_cKey1, m_pXPlayer->m_cKey2 );

			if( encr.Decode( IN (unsigned char *)pMsg, OUT (unsigned char *)pmsg )
				== FALSE ) {
			
				m_pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();

				m_pXPlayer->m_pSOPlayer->LogOut();

				return;
			}
		} else {

			memcpy( pmsg,  (char *)pMsg, siMsgLength + 3 );			
		}
*/
		m_pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();

		pMsg	=	(OnMsg*)cMsg;

		m_lastTick = GetTickCount();

		// 플레이어가 필드에 있을때
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE )
		//if(siStatus	 & ON_PLAYER_STATUS_INFIELD)
		{				
			bProcessMessage	= ProcessMessageInField();
			
			if(bProcessMessage == FALSE)
				bProcessMessage = ProcessPartyMessage();
		}

		// 플레이어가 마을에 있을때
		//if(stAccountInfo.pVillage)
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE )
		{
			if(bProcessMessage == FALSE)
				bProcessMessage		=	ProcessMessageInVillage();
		}

		// 플레이어가 전투중일때
		//if(siStatus==ON_PLAYER_STATUS_INBATTLE)
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INBATTLE ) == TRUE )
		{				
			bProcessMessage		=	ProcessMessageInBattle();
		}
		
		if(bProcessMessage == FALSE)
		{
			if(ProcessMessageCommonly() == FALSE)
			{
				// 여기에서도 버퍼를 메시지를 처리하지 않았다면
				// 메시지를 그냥 지운다.
			}
		}
	}

}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 필드에 관련된 메세지를 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::ProcessMessageInField()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{	
	// 경매소 관련 처리 	
	case AUCTION::REQUEST_SELL_ITEM:		
		m_pAuctionMarketProc->recvRequestSellItem(cMsg);		
		break;

	case AUCTION::REQUEST_SELL_FOLLOWER:	
		m_pAuctionMarketProc->recvRequestSellFollower(cMsg);		
		break;

	case AUCTION::REQUEST_BIDDING:
		m_pAuctionMarketProc->recvRequestBidding(cMsg);		
		break;

	case AUCTION::REQUEST_ENTER:
		m_pAuctionMarketProc->recvRequestEnter(cMsg); 
		break;

	case AUCTION::REQUEST_LEAVE:
		m_pAuctionMarketProc->recvRequestLeave(cMsg);
		break;

	case ON_REQUEST_AUDIT_GUILD_INFO:
		RecvGetAuditGuildInfo();
		break;

	// 자원 탐사 요청 
	case BURIED::REQUEST_PROBE_AREA:
		recvProbeArea();
		break;

	// 이동한다.
	case ON_REQUEST_POSITION:
		RecvPosition();
		break;

	case ON_REQUEST_CHAR_DETAILINFO:
		RecvCharDetailInfo();
		break;

	// 로그아웃한다.
	case ON_REQUEST_LOGOUT:
		LogOut();
		break;		

	// 특정 플레이어의 정보를 요청한다.
	case ON_REQUEST_PLAYER_INFO:
		RecvPlayerInfo();
		break;		

	// 마을에 들어간다.
	case ON_REQUEST_ENTER_VILLAGE:
		RecvEnterVillage();
		break;		
	
	// 전투를 요청한다.
	case ON_REQUEST_BATTLE:
		RecvBattle();
		break;

	// 아이템을 버린다.
	case ON_REQUEST_THROWITEM:
		RecvThrowItem();
		break;

	// 아이템을 줍는다.
	case ON_REQUEST_PICKUPITEM:
		RecvPickUpItem();
		break;

	case ON_REQUEST_MOVEMAP:
		RecvMoveMap();
		break;

	case ON_REQUEST_TRADE:
		if( IsEvent() )	break;
		RecvAskTrade();
		break;

	case ON_REQUEST_ACCEPT_TRADE:
		RecvAcceptTrade();
		break;

	case ON_REQUEST_CANCEL_TRADE:
		RecvCancelTrade();
		break;

	case ON_REQUEST_FINISH_CHOOSE_TRADEITEM:
		RecvFinishChooseTradeItem();
		break;

	case ON_REQUEST_DECIDE_TRADE:
		RecvDecideTrade();
		break;

	case ON_REQUEST_CHANGE_TRADE_GOODS:
		RecvChangeTradeGoods();
		break;
	
	case ON_REQUEST_COMPLETE_TRADE_BOOK:
		RecvCompleteTradeBook();
		break;	

	case ON_REQUEST_IMOTICON:
		RecvImoticon();
		break;

	case ON_REQUEST_WARP:
		RecvWarp();
		break;

/*	// 저자 거리에서의 메세지
	case ON_REQUEST_CHARGEBOOTH:
		RecvChargeBooth();
		break;
	case ON_REQUEST_LEAVEBOOTH:
		RecvLeaveBooth();
		break;
	case ON_REQUEST_DISPLAYBOOTH:
		RecvDisplayBooth();
		break;
	case ON_REQUEST_SETBOOTHITEM:
		RecvSetBoothItem();
		break;
	case ON_REQUEST_CANCELBOOTHITEM:
		RecvCancelBoothItem();
		break;
	case ON_REQUEST_GETBOOTHINFO:
		RecvBoothInfo();
		break;*/
		
	case ON_REQUEST_ATTACK_VILLAGE:
		RecvAttackVillage();
		break;

	//마을의 공격 형태를 설정할수 있는 상태인지 알아온다.
	case ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK:
//		RecvVillageAttackTypeCheck();
		break;

	// 성의 공격 형태를 설정한다.(기름)
	case ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK:
//		RecvVillagePlayerOilAttack();
		break;
	// 성의 공격 형태를 설정한다. (돌)
	case ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK:
//		RecvVillagePlayerStoneAttack();
		break;
	// 성의 공격 형태를 설정한다.(마을 수비병)
	case ON_REQUEST_VILLAGE_DEFENCE:
		RecvPlayerVillageDefenceUp();
		break;
	//공성 관련 플레이어->성 추가 부분.
	case ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY:
		RecvLongDistanceAttackVillageReady();		
		break;
	case ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION:
		RecvLongDistanceAttackVillageAction();
		break;

	//보급수레에서 보급품을 얻어온다.
	case ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT:
		RecvHeal();
//		RecvGetSupplyAtGuildUnit();
		break;

	//공성 관련 필드 전투 추가 부분.
	case ON_REQUEST_FIELDATTACK_READY:
		RecvFieldAttackReady();
		break;
	case ON_REQUEST_FIELDATTACK_ACTION:
		RecvFieldAttackAction();
		break;
	case ON_REQUEST_FIELDATTACK_DEAD:
		RecvFieldAttackDead();
		break;
	
	case ON_REQUEST_INCREASE_GUILD_UNIT:
		RecvIncreaseGuildUnit();
		break;
	case ON_REQUEST_DECREASE_GUILD_UNIT:
		RecvDecreaseGuildUnit();		
		break;
	case ON_REQUEST_LIST_GUILD_UNIT:
		RecvGetGuildUnitList();	
		break;

	case ON_REQUEST_INCREASE_GUILD_SUPPLY:
//		RecvIncreaseGuildSupply();
		break;
	case ON_REQUEST_DECREASE_GUILD_SUPPLY:
//		RecvDecreaseGuildSupply();	
		break;
	case ON_REQUEST_LIST_GUILD_SUPPLY:
//		RecvGetGuildSupplyList();
		break;

	case ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER:
//		RecvIncreaseVillageDefenceSoldier();
		break;
	case ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER:
//		RecvGetVillageDefenceSoldier();
		break;

	case ON_REQUEST_GUILD_VILLAGEATTACK_RESULT:
		RecvVillageAttackResult();
		break;
	case ON_REQUEST_GUILD_CHOICE_PLAYER:
		RecvChoicePlayer();
		break;
	case ON_REQUEST_GUILD_MEMBERS_LIST:
		RecvReqGuildMembersList();
		break;
	case ON_REQUEST_WAR_VILLAGE_DEFENCE:
		RecvReqWarVillageDefence();
		break;
	case ON_REQUEST_VILLAGE_DEFENCE_STOP:
		RecvReqWarVillageDefenceStop();
//	case ON_REQUEST_WAR_VILLAGE_IN:
//		RecvReqWarVillageIn();
		break;
	case ON_REQUEST_WAR_WARP:	
		RecvWarWarp();
		break;
	case ON_REQUEST_NORMAL_CHAR:
		RecvNormalChar();
		break;
	//공성유닛으로 변신을 요청한다.
	case ON_REQUEST_CHANGE_WAR:
		RecvChangeUnit();
		break;
	case ON_REQUEST_CHARGE_MP:
		RecvChargeMP();
		break;
    //전투 포기를 요청하였다
	case ON_REQUEST_GIVEUP:
		RecvGiveUp();
		break;

	case ON_REQUEST_PM_OPEN:
		RecvPMOpen();
		break;
	case ON_REQUEST_PM_CLOSE:
		RecvPMClose();
		break;
	case ON_REQUEST_PM_BUYITEM:
		RecvPMBuyItem();
		break;
	case ON_REQUEST_PM_ITEMINFO:
		RecvPMReqItemInfo();
		break;
	case ON_REQUEST_PM_CHANGEITEM:
		RecvPMChangeItem();
		break;

	case ON_REQUEST_QUEST_SELECTNPC:
		RecvReqQuestSelectNPC();
		break;
	case ON_REQUEST_QUEST_ACCEPTQUEST:
		RecvReqQuestAcceptQuest();
		break;
	case ON_REQUEST_QUEST_CANCELQUEST:
		RecvReqQuestCancelQuest();
		break;
	case ON_REQUEST_QUEST_GETPLAYQUEST:
		RecvReqQuestGetPlayQuest();
		break;		
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST:
		// 특정 마을의 투자자 리스트를 요청 받았다. (원래는 관청에서만 처리하던 메시지였는데 장부에서 같은 메시지를 사용하기 때문에 field에도 뺐다.)
		RecvGovernmentInvestmentGetList();
		break;		

	case ON_REQUEST_MACRO_USING_USER_INFO : 		
		RecvMacroUsingUserInfo();
		break;
	

	// 알수 없는 명령이 들어왔을때.
	default:
		bProcessMsg	=	FALSE;		
		break;
	}

	return bProcessMsg;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을에 관련된 메세지를 처리한다.
//	수정 일자	:	2002-02-27 오후 2:44:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::ProcessMessageInVillage()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{	
	// 주막 아이템 리스트 및 가져오기

	case ON_REQUEST_ITEMLIST:
		RecvItemList();
		break;

	case ON_REQUEST_TAKEOUTITEM:
		RecvTakeOutItem();
		break;

	case ON_REQUEST_SAVEITEM:
		RecvSaveItemIntoItemBank();
		break;




	// 상점에서 아이템들의 리스트를 요구한다.
	case ON_REQUEST_LIST_ITEM:
		RecvListItem();
		break;
		
	// 아이템을 산다.
	case ON_REQUEST_BUY_ITEM:
		RecvBuyItem();
		break;
		
	// 아이템을 판다.
	case ON_REQUEST_SELL_ITEM:
		RecvSellItem();
		break;
					
	// 마을에서 떠난다.
	case ON_REQUEST_LEAVE_VILLAGE:
		RecvLeaveVillage();
		break;
		
/*
	case ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM:
		RecvJoinInnChatRoom();
		break;
	case ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM:
		RecvLeaveInnChatRoom();
		break;
*/

	case ON_REQUEST_ENTER_MARKET:
		RecvEnterMarket();
		break;
	case ON_REQUEST_LEAVE_MARKET:
		RecvLeaveMarket();
		break;
	
	// 농장
	case ON_REQUEST_FARM_INFO:
		RecvFarmInfo();
		break;
	case ON_REQUEST_FARM_BUY:
		RecvFarmBuy();
		break;
	case ON_REQUEST_FARM_SLOTINFO:
		RecvFarmSlotInfo();
		break;
	case ON_REQUEST_FARM_BUILDSTRUCT:
		RecvFarmBuildStruct();
		break;
	case ON_REQUEST_FARM_EXPANDSLOT:
		RecvFarmExpandSlot();
		break;
	case ON_REQUEST_FARM_DESTROYSTRUCT:
		RecvFarmDestroyStruct();
		break;
	case ON_REQUEST_FARM_SETEQUIPMENT:
		RecvFarmSetEquipment();
		break;
	case ON_REQUEST_WRITE_DESCRIPT:
		RecvWriteDescript();
		break;
	
	// 농장에 새로 추가된 메세지.
	case ON_REQUEST_FARM_ITEMPRODUCT:
		RecvFarmItemProduct();
		break;
	case ON_REQUEST_PLANT_CANCEL_PLAN:
		RecvPlantCancelPlan();
		break;
	case ON_REQUEST_PLANT_CHANGE_PAY:
		RecvPlantChangPay();
		break;
	case ON_REQUEST_FARM_UPGRADESLOT:
		RecvFarmUpgradeSlot();
		break;
	case ON_REQUEST_FARM_WORKSTART:
		RecvFarmWorkStart();
		break;
	case ON_REQUEST_FARM_WORK:
		RecvFarmWork();
		break;

	case ON_REQUEST_FARM_WORK_HACK:
		RecvFarmWorkHack();
		break;

	case ON_REQUEST_STORAGE_INPUTITEM:
		RecvStorageInputItem();
		break;
	case ON_REQUEST_STORAGE_OUTPUTITEM:
		RecvStorageOutputItem();
		break;
	case ON_REQUEST_STORAGE_INFO:
		RecvStorageInfo();
		break;
	case ON_REQUEST_BANK_OPENSAVINGACCOUNT:
		RecvOpenSavingAccount();
		break;
	case ON_REQUEST_BANK_BANKBOOK_LIST:
		RecvBankbookList();
		break;		
	case ON_REQUEST_BANK_INFO:
		RecvBankInfo();
		break;
	case ON_REQUEST_BANK_DEPOSIT:
		RecvBankDeposit();
		break;
	case ON_REQUEST_BANK_DRAWING:
		RecvBankDrawing();
		break;
	case ON_REQUEST_BANK_COLLATERAL:
		RecvBankCollateral();
		break;
	case ON_REQUEST_BANK_COLLATERALLIST:
		RecvBankCollateralList();
		break;
	case ON_REQUEST_BANK_TAKECOLLATERAL:
		RecvBankTakeCollateral();
		break;
/*
	case ON_REQUEST_INN_CREATECHATROOM:
		// 주점에서 채팅방을 만든다.
		RecvCreateChatRoomInInn();
		break;	
	case ON_REQUEST_ENTER_INN:
		RecvEnterInn();
		break;
	case ON_REQUEST_LEAVE_INN:
		RecvLeaveInn();
		break;
	case ON_REQUEST_ENTER_VWR:
		RecvEnterWaitRoom();
		break;	
	case ON_REQUEST_ENTER_NOTICEBOARD:
		// 게시판에 들어간다.
		RecvEnterNoticeBoard();
		break;
	case ON_REQUEST_REGIST_NOTICE:
		// 게시판에 글을 등록시킨다.
		RecvRegistNotice();
		break;
	case ON_REQUEST_DELETE_NOTICE:
		// 게시판의 글을 삭제한다.
		RecvDeleteNotice();
		break;
	case ON_REQUEST_NOTICEBOARD_NOTICELIST:
		// 게시판의 글을 얻어온다.
		RecvNoticesList();
		break;
	case ON_REQUEST_VIEW_NOTICE:
		// 게시판의 본문을 얻어온다.
		RecvViewNotice();
		break;
	case ON_REQUEST_MODIFY_NOTICE:
		// 게시판의 본문을 수정한다.
		RecvModifyNotice();
		break;
*/
	case ON_REQUEST_INCREASE_GUILD_UNIT:
		RecvIncreaseGuildUnit();
		break;
	case ON_REQUEST_DECREASE_GUILD_UNIT:
		RecvDecreaseGuildUnit();		
		break;
	case ON_REQUEST_LIST_GUILD_UNIT:
		RecvGetGuildUnitList();	
		break;

	case ON_REQUEST_INCREASE_GUILD_SUPPLY:
//		RecvIncreaseGuildSupply();
		break;
	case ON_REQUEST_DECREASE_GUILD_SUPPLY:
//		RecvDecreaseGuildSupply();	
		break;
	case ON_REQUEST_LIST_GUILD_SUPPLY:
//		RecvGetGuildSupplyList();
		break;
	case ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER:
//		RecvIncreaseVillageDefenceSoldier();
		break;
	case ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER:
//		RecvGetVillageDefenceSoldier();
		break;


	case ON_REQUEST_BARRACK_GETSOLDERLIST:
		// 용병 List를 요청한다.
		RecvBarrackGetSolderList();
		break;
	case ON_REQUEST_BARRACK_BUYSOLDER:
		// 용병을 고용한다.
		RecvBarrackBuySolder();
		break;
	case ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST:
		// 해고할 용병 List를 요청한다.
		RecvBarrackGetDismissalSolderList();
		break;
	case ON_REQUEST_BARRACK_DISMISSALSOLDER:
		// 용병을 해고한다.
		RecvBarrackDismissalSolder();
		break;
	case ON_REQUEST_GBARRACK_GETSOLDERLIST:
		// 장수용병 List를 요청한다.
		RecvGBarrackGetSolderList();
		break;
	case ON_REQUEST_GBARRACK_BUYSOLDER:
		// 장수용병을 고용한다.
		RecvGBarrackBuySolder();
		break;
	case ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST:
		// 전직할 장수용병 List를 요청한다.
		RecvGBarrackGetChangeJobSolderList();
		break;
	case ON_REQUEST_GBARRACK_CHANGEJOBSOLDER:
		// 장수용병을 전직한다.
		RecvGBarrackChangeJobSolder();
		break;
	case ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST:
		// 해고할 장수용병 List를 요청한다.
		RecvGBarrackGetDismissalSolderList();
		break;
	case ON_REQUEST_GBARRACK_DISMISSALSOLDER:
		// 장수용병을 해고한다.
		RecvGBarrackDismissalSolder();
		break;
	///////////////////////////////////////////////////////////////////////////////////
	case ON_REQUEST_GBARRACK_GET_GENERAL_LIST:
		// 2차 장수 List 요청.
		RecvGBarrackGetGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_BUY_GENERAL:
		//2차 장수 고용
		RecvGBarrackBuyGeneralEx();
		break;
	case ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST:
		//2차 장수 전직 List 요청 
		RecvGBarrackGetChangeGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_CHANGE_GENERAL:
		//2차 장수 전직
		RecvGBarrackChangeGeneralEx();
		break;
	case ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST:
		//2차 장수 해고 List 요청
		RecvGBarrackGetDismissalGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_DISMISSAL_GENERAL:
		//2차 장수 해고 
		RecvGBarrackDismissalGeneralEx();
		break;
	////////////////////////////////////////////////////////////////////////////////////
	case ON_REQUEST_MBARRACK_GETMONSTERLIST:
		// 몬스터 List를 요청한다.
		RecvMBarrackGetMonsterList();
		break;
	case ON_REQUEST_MBARRACK_BUYMONSTER:
		// 몬스터를 고용한다.
		RecvMBarrackBuyMonster();
		break;
	case ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST:
		// 해고할 몬스터 List를 요청한다.
		RecvMBarrackGetDismissMonsterList();
		break;
	case ON_REQUEST_MBARRACK_DISMISSMONSTER:
		// 몬스터를 해고한다.
		RecvMBarrackDismissMonster();
		break;
	case ON_REQUEST_HOSPITAL_GETCURELIST:
		// 용병 치료List를 요청한다.
		RecvHospitalGetCureList();
		break;
	case ON_REQUEST_HOSPITAL_CURESOLDER:
		// 용병을 치료한다.
		RecvHospitalCureSolder();
		break;
	case ON_REQUEST_HOSPITAL_CUREALLSOLDER:
		// 모든 용병을 치료한다.
		RecvHospitalCureAllSolder();
		break;
	case ON_REQUEST_WHARF_ENTER:
		// 부두에 들어간다.
		RecvWharfEnter();
		break;
	case ON_REQUEST_WHARF_BUYTICKET:
		// 부두에서 표를 산다.
		RecvWharfBuyTicket();
		break;
	case ON_REQUEST_WHARF_EXIT:
		// 부두에서 나간다.
		RecvWharfExit();
		break;
	case ON_REQUEST_WHARFWAITROOM_ENTER:
		// 부두대기실에 들어간다.
		RecvWharfWaitRoomEnter();
		break;
	case ON_REQUEST_WHARFWAITROOM_EXIT:
		// 부두대기실에서 나간다.
		RecvWharfWaitRoomExit();
		break;
	case ON_REQUEST_GOVERNMENT_MAIN_ENTER:
		// 관청 들어가기
		RecvGovernmentEnter();
		break;
	case ON_REQUEST_GOVERNMENT_MAIN_LEAVE:
		// 관청 나가기
		RecvGovernmentLeave();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER:
		// 관청, 투자 들어가기
		RecvGovernmentInvestmentEnter();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE:
		// 관청, 투자 나가기
		RecvGovernmentInvestmentLeave();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST:
		// 관청, 투자 List얻기
		RecvGovernmentInvestmentGetList();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT:
		// 관청, 투자 하기
		RecvGovernmentInvestmentSetInvestment();
		break;
	case ON_REQUEST_GOVERNMENT_VILLAGEINFO:
		// 관청, 마을정보 얻기
		RecvGovernmentVillageInfo();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT:
		// 관청, 투자한돈 가지고 나가기
		RecvGovernmentInvestmentGetOut();
		break;
	case ON_REQUEST_PROFIT_STATISTICS:
		RecvProfitStatistics();
		break;
	case ON_REQUEST_GUILD_CREATE_COST:
		// 상단 개설 비용 정보 요청
		RecvReqGuildCreateCost();
		break;
	case ON_REQUEST_CREATE_GUILD:
		// 상단 개설 요청
		RecvCreateGuild();
		break;
	case ON_REQUEST_DELETE_GUILD:
		// 상단 삭제 요청
		RecvDeleteGuild();
		break;
	case ON_REQUEST_JOIN_LEAVE_GUILD:
		RecvJoinLeaveGuild();
		break;
	case ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST:
		RecvReqGuildWaitingJoinMembersList();
		break;
	case ON_REQUEST_JOINGUILD_OKNO:
		RecvJoinGuildOkNo();
		break;
	case ON_REQUEST_GUILD_DETAIL_INFO:
		RecvGuildDetailInfo();
		break;
	case ON_REQUEST_GUILD_MEMBERS_LIST:
		RecvReqGuildMembersList();
		break;
	case ON_REQUEST_GUILD_DISMISSAL:
		RecvGuildDismissal();
		break;
	case ON_REQUEST_GUILD_PROMOTION:
		RecvReqGuildPromotion();
		break;
	case ON_REQUEST_GUILD_DEMOTION:
		RecvReqGuildDemotion();
		break;
	case ON_REQUEST_GUILD_LIST:				//상단의 리스트를 받아온다.
		RecvGuildList();
		break;	
	case ON_REQUEST_GUILD_SET_RELATION:		//상단의 적대관계설정.
		RecvGuildSetRelation();
		break;	
	case ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER:	//화친제의 메세지를 보낸다.
		RecvFriendlyMessageOffer();
		break;
	case ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT:	//화친제의를 받아들인다.
		RecvFriendlyMessageAccept();
		break;
	case ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL:	//화친제의를 씹는다.
		RecvFreidnlyMessageCancel();
		break;

	case ON_REQUEST_GUILD_VILLAGEATTACK_RESULT:
		RecvVillageAttackResult();
		break;
	case ON_REQUEST_GUILD_CHOICE_PLAYER:
		RecvChoicePlayer();
		break;
	case ON_REQUEST_VILLAGE_INVESTMENT_POWER:
		RecvInvestmentPower();
		break;
	
	case ON_REQUEST_CHANGE_GUILD_FLAG:
		RecvGuildChangeFlag();
		break;	
	case ON_REQUEST_USED_FLAG_LIST:
		RecvReqUsedFlagList();
		break;	
	case ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT:
		RecvReqMaxVillageDefenceInvestment();
		break;
	case ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT:
		RecvReqSetVillageDefenceInvestment();
		break;
	case ON_REQUEST_QUEST_SELECTNPC:
		RecvReqQuestSelectNPC();
		break;
	case ON_REQUEST_QUEST_ACCEPTQUEST:
		RecvReqQuestAcceptQuest();
		break;
	case ON_REQUEST_QUEST_CANCELQUEST:
		RecvReqQuestCancelQuest();
		break;
	case ON_REQUEST_QUEST_GETPLAYQUEST:
		RecvReqQuestGetPlayQuest();
		break;

	case ON_REQUEST_ENTER_STRUCTURE:
		RecvEnterStructure();
		break;

	case ON_REQUEST_LEAVE_STRUCTURE:
		RecvLeaveStructure();
		break;

	case ON_REQUEST_FARM_WORKINFO:		
		RecvEnterFactory();
		break;

	case ON_REQUEST_LEAVE_FACTORY:
		RecvLeaveFactory();
		break;

	case ON_REQUEST_BANK_SAVE:
		RecvBankSave();
		break;
	case ON_REQUEST_BANK_WITHDRAW:
		RecvBankWithdraw();
		break; 

	case ON_REQUEST_MACRO_USING_USER_INFO :		
		RecvMacroUsingUserInfo();
		break;


	default:
		// 알수 없는 명령이 들어왔을때.'		
		bProcessMsg = FALSE;
		break;
	}

	return bProcessMsg;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투중의 메세지를 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::ProcessMessageInBattle()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg	=	TRUE;

	pMsg	=	(OnMsg*)cMsg;
	 
	switch(pMsg->dwMsgType)
	{
	case ON_REQUEST_RETURNONLINE:
		{	
			if( IsEvent() )
			{
				OnReqReturnOnlineMsg	*pReqReturnOnlineMsg = ( OnReqReturnOnlineMsg* )pMsg;
				CBattle					*pBattle;
				SI32					i;
				BaseChar				*pBaseChar;
				SOPlayer				*pTargetPlayer;
				SI32					siScore;
				DWORD					dwWinDBAccount, dwLoseDBAccount;
				
				if( ( pBattle =	GetBattle() ) != NULL )
				{
				
					// 적을 얻어온다.
					// 최대 전투에 참여 가능한 사람은 7명이다.
					for( i = 0; i < 7; i++ )
					{
						if( ( pBaseChar = pBattle->GetPlayer( i ) ) == NULL )	continue;						

						if( ISPLAYER( pBaseChar->uiAccount ) == NULL )			continue;

						pTargetPlayer = ( SOPlayer* )pBaseChar;

						if( this == pTargetPlayer )								continue;

						if( pMyWorld->m_ExpoRankingSystem.IsInvalidBattle( stAccountInfo.siDBAccount,
																			pTargetPlayer->stAccountInfo.siDBAccount ) == FALSE )	continue;						


						if( pReqReturnOnlineMsg->dwBattleResult	== TRUE )
						{
							dwWinDBAccount		=	stAccountInfo.siDBAccount;
							dwLoseDBAccount		=	pTargetPlayer->stAccountInfo.siDBAccount;
						}
						else
						{
							dwWinDBAccount		=	pTargetPlayer->stAccountInfo.siDBAccount;
							dwLoseDBAccount		=	stAccountInfo.siDBAccount;
						}
						

						siScore = pMyWorld->m_ExpoRankingSystem.CalcScore( dwWinDBAccount, dwLoseDBAccount );	

						// DB에 저장한다.
						DBIKEvent_ExpoBattleResult	ExpoBattleResult;
						
						ExpoBattleResult.siWinDBAccount		=	dwWinDBAccount;
						ExpoBattleResult.uiWinDBSlot		=	1;

						ExpoBattleResult.siLoseDBAccount	=	dwLoseDBAccount;
						ExpoBattleResult.uiLoseDBSlot		=	1;

						ExpoBattleResult.siScore			=	siScore;

						pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_EVENT_EXPOBATTLERESULT, 
															LPVOID( &ExpoBattleResult ), 
															sizeof( ExpoBattleResult ) );

								
						break;
					}					
				}				
			}

			removeSelfInBattle();			
			RecvReturnOnline();
		}
		break;

	case ON_REQUEST_MACRO_USING_USER_INFO :		
		RecvMacroUsingUserInfo();
		break;

	// 알수 없는 명령이 들어왔을때.
	default:
		{				
			if(pMsg->dwMsgType >= 10050 && pMsg->dwMsgType <= 10300 && siMsgLength <= 128)
			{
				if(GetBattle() != NULL)
				{
					GetBattle()->PushData(cMsg, siMsgLength);
				}
			}			
			else
			{
				bProcessMsg	=	FALSE;
			}
		}
		break;
	}

	return bProcessMsg;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	공통적인 메시지를 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::ProcessMessageCommonly()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{

	case ON_REQUEST_MOVEITEM: 				
		RecvShareItem();
		break;
	case ON_REQUEST_WEARITEM:
		if( IsEvent() )	break;
		RecvWearItem();
		break;
	case ON_REQUEST_STRIPITEM:
		if( IsEvent() )	break;
		RecvStripItem();
		break;
	case ON_REQUEST_READ_DESCRIPT:
		RecvReadDescript();
		break;
	case ON_REQUEST_VILLAGE_NATION:
		RecvVillageNation();
		break;
	case ON_REQUEST_SPENDITEM:
		RecvSpendItem();
		break;
//	case ON_REQUEST_DELETE_TIMERITEM:			
//		RecvDeleteTimerItem();
//		break;
	// 장부 관련 프로토콜
	case ON_REQUEST_AUDIT_ACCOUNT:
		RecvAuditAccountList();
		break;
	case ON_REQUEST_AUDIT_OWN:
		RecvAuditOwnList();
		break;
	case ON_REQUEST_AUDIT_VILLAGE_LIST:
		RecvAuditVillageList();
		break;		
	case ON_REQUEST_AUDIT_VILLAGE_INFO:
		RecvAuditVillageInfo();
		break;
	case ON_REQUEST_AUDIT_FRIEND_LIST:
		RecvAuditFriendList();
		break;
	case ON_REQUEST_AUDIT_FRIEND_SET_DENIAL:
		RecvAuditFriendSetDenial();
		break;
	case ON_REQUEST_AUDIT_FRIEND_ADD_ONE:
		RecvAuditFriendAddOne();
		break;
	case ON_REQUEST_AUDIT_FRIEND_REMOVE_SOME:
		RecvAuditFriendRemoveSome();
		break;
	case ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL:
		RecvAuditFriendRemoveAll();
		break;

	case ON_REQUEST_AUDIT_PROFIT_STATISTICS:
		RecvAuditProfitStatistics();
		break;
	case ON_REQUEST_AUDIT_STATUS:
		RecvAuditStatus();
		break;
	case ON_REQUEST_AUDIT_SETNAME:
		RecvAuditSetName();
		break;
	case ON_COMMAND:
		RecvCommand();
		break;	
	case ON_REQUEST_INC_ABILITY_BYBONUS:
		RecvIncAbilityByBonus();
		break;
	case ON_HEARTBEAT:
		RecvHeartBeat();
		break;
	case ON_REQUEST_PI_INMINIMAP:
		RecvReqPIInMinimap();
		break;
	case ON_REQUEST_VILLAGE_INFO:
		RecvRequestVillageInfo();
		break;
	case ON_REQUEST_BANK_BANKBOOK_LISTCOUNT:
		RecvBankbookListCount();
		break;		
	case ON_REQUEST_VMERCENARY_DELETE:
		{			
			if( siVMercenarySlot == 0 )		break;		// 따라다니는 용병이 없는 상태

			RecvVMercenaryChange(0);

			DBIKVMercenaryHide		VMercenaryHide;

			VMercenaryHide.siDBAccount = stAccountInfo.siDBAccount;
			VMercenaryHide.uiDBSlot    = stAccountInfo.uiSlot;
			pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_VMERCENARYDELETE, LPVOID(&VMercenaryHide), sizeof(DBIKVMercenaryHide));
		}
		break;	
	case ON_REQUEST_PERSONAL_INFO:
		RecvRequestPersonalInfo();
		break;
	// 아이템 제련
	case ON_REQUEST_REFINE_ITEM:
		RecvRefineItem();

		break;
	case ON_RESPONSE_RUN_PROGRAM_LIST:
		RecvRunProgramList();
		break;
	// 전투모드 / 평화모드 설정
	case ON_REQUEST_SET_ATTACK_MODE:
		RecvSetAttackMode();
		break;		

	// 로그아웃한다.
	case ON_REQUEST_LOGOUT:
		LogOut();
		break;		
	case ON_REQUEST_MACRO_USING_USER_INFO : 
		RecvMacroUsingUserInfo();
		break;

	case ON_REQUEST_CHECK_GAMEGUARD :
		RecvRequestCheckGameGuard();
		break;

	case ON_USER_INFO_GAMEGUARD :
		RecvUserInfoGameGuard();
		break;

	default:
		bProcessMsg	=	FALSE;
		break;
	}

	return bProcessMsg;
}

/*
ON_REQUEST_PARTY				// 파티를 요청한다.
ON_RESPONSE_PARTY				// 파티를 요청한다.(응답)(서버)

ON_RECEIVE_ASK_PARTY			// 해당 클라이언트에게 다른 클라이언트가 파티를 요청했다는 것을 알려준다.

ON_REQUEST_ACCEPT_PARTY			// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다. 동의하느냐 마느냐.
ON_RESPONSE_ACCEPT_PARTY		// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다.(응답) 동의 결과에 따라.
ON_REQUEST_LEAVE_PARTY			// 클아이언트가 파티를 떠난다.
*/

/************************************************************************/
/*                                                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/

#ifdef _VERIFY_PARTY_PACKET
#define OUTPUTDEBUGSTRING(x) OutputDebugString(x)
#else
#define OUTPUTDEBUGSTRING(x)
#endif

inline void TRACE_LOG(char *szFormat, ...)
{
#ifdef _VERIFY_PARTY_PACKET
#pragma message(" _VERIFY_PARTY_PACKET :: TRACE_LOG Function compiled\n")

	char szBuffer[1024];
    char *pArguments;
	
	pArguments=(char*)&szFormat+sizeof(szFormat);
	vsprintf(szBuffer, szFormat, pArguments);

	OUTPUTDEBUGSTRING( szBuffer );

#endif
}

VOID SOPlayer::RecvRequestParty()
{	
	OnReqAskPartyMsg	*pMsg;
	SOPlayer			*pTargetPlayer;	
	BOOL				bRes	=	FALSE;

	pMsg	=	(OnReqAskPartyMsg*)cMsg;

	if( ( pTargetPlayer = pMyWorld->GetPlayer( pMsg->UserID ) ) == NULL )						goto	END;

	// 내가 파티장이 아니고 일반 파티원일 경우에는 다른 사람에게 파티 신청을 요청할 수 없다.
	if( ( IsPartyLeader() == FALSE ) && ( IsPartyMember() == TRUE ) )							goto	END;

	// 내가 상대방에게 파티를 요청할 수 있는 상태가 아니라면 실패한다.
	if( pTargetPlayer->CanRequestJoinParty() == FALSE )											goto	END;	

	// 상대방이 파티 요청을 받을 수 있는 상태가 아니라면 실패한다.
	if( pTargetPlayer->CanReceiveRequestJoinParty() == FALSE )									goto	END;	
	
	RequestJoinParty( pTargetPlayer->uiAccount );
	pTargetPlayer->ReceiveRequestJoinParty( uiAccount );

	bRes	=	TRUE;

END:
	
	if ( bRes == TRUE )
	{
		// 파티 가입 신청자(자기 자신)에게 보내는 메세지.
		SendFieldMsg( ON_RESPONSE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

		// 파티 가입 응답자 에게 보내는 메세지 
		pTargetPlayer->SendFieldMsg( ON_RECEIVE_ASK_PARTY, SO_SFM_ONLYME, LPVOID( uiAccount ), LPVOID( szID ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO) );
	}

}



/************************************************************************/
// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다. 동의하느냐 마느냐.
// 타겟 클라이언트 -> 서버로 온 메세지 

/************************************************************************/
VOID SOPlayer::RecvRequestAcceptParty()
{
	OnResAskPartyMsg	*pMsg;

	pMsg	=	( OnResAskPartyMsg* )cMsg;

	switch( pMsg->dwResponse )
	{
	case ON_RET_OK:
		
		// 파티에 참여하는데 동의하였다.
		AcceptRequestingJoinParty();

		break;

	case ON_RET_NO:

		// 파티에 참여하는데 거부하였다.
		RejectRequestingJoinParty();

		break;
	}
}


/************************************************************************/
/*                                                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/
VOID	SOPlayer::RecvRequestLeaveParty()
{
	BOOL		bRes	=	FALSE;	

	if( LeaveParty() == FALSE )																	goto	END;

	bRes	=	TRUE;
	
END:
	
	if( bRes == TRUE )
	{
		// 파티에서 나가는데 성공했다.
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( szID ) );		
	}	
	else
	{
		// 파티에서 나가는데 실패했다.
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );		
	}

}

/*
ON_REQUEST_PARTY				// 파티를 요청한다.
ON_RESPONSE_PARTY				// 파티를 요청한다.(응답)(서버)

ON_RECEIVE_ASK_PARTY			// 해당 클라이언트에게 다른 클라이언트가 파티를 요청했다는 것을 알려준다.

ON_REQUEST_ACCEPT_PARTY			// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다. 동의하느냐 마느냐.
ON_RESPONSE_ACCEPT_PARTY		// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다.(응답) 동의 결과에 따라.
ON_REQUEST_LEAVE_PARTY			// 클아이언트가 파티를 떠난다.
*/

BOOL	SOPlayer::ProcessPartyMessage(VOID)
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{
		case ON_REQUEST_PARTY:
		{	
			// 파티 요청 메세지
			RecvRequestParty();
		}break;

		case ON_REQUEST_CANCEL_REQUESTING_PARTY:
			RecvCancelRequestParty();
			break;	

		case ON_REQUEST_ACCEPT_PARTY:
		{
			RecvRequestAcceptParty();
		}break;

		case ON_REQUEST_LEAVE_PARTY:
		{
			RecvRequestLeaveParty();
		}break;

		default:
			bProcessMsg= FALSE;
	}

	return bProcessMsg;
}

VOID	SOPlayer::RecvCancelRequestParty()
{
	BOOL	bRes	=	FALSE;

	// 파티 요청을 취소한다.
	if( CancelRequestingParty() == FALSE )						goto	END;

	bRes	=	TRUE;

END:
	if( bRes == TRUE )
	{
		SendFieldMsg( ON_RESPONSE_CANCEL_REQUESTING_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_CANCEL_REQUESTING_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// GameGuard관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _GAMEGUARD
#include "CSAuth.h"
#endif

#define GAMEGUARD_CHECK_TIME (1000 * 60 * 5 )
#define GAMEGUARD_CHECK_WAITING_TIME (1000 * 10 * 60)
// #define 

VOID SOPlayer::CheckGameGuard(VOID)
{
// ---- 윤영대 2005 02 22
#ifdef _GAMEGUARD
	DWORD dwNowTick;
#endif // _GAMEGUARD
// ----

	if(m_pCSAuth == NULL) return;

#ifdef _GAMEGUARD
	dwNowTick = GetTickCount();

	if(m_bCheckPass == TRUE)
	{
		if( dwNowTick - m_lastGameGuardCheckTime > GAMEGUARD_CHECK_TIME )
		{
			if( ( pMyWorld->m_bGameGuard == TRUE ) ) // && ( IsAdminPlayer() == false ) )
			{
				m_lastGameGuardCheckTime = dwNowTick;
				SendFieldMsg( ON_CHECK_GAMEGUARD, SO_SFM_ONLYME, (LPVOID)m_pCSAuth->GetAuthDword());
				m_bCheckPass = FALSE;
			}
		}
	}
	else
	{
		if((dwNowTick - m_lastGameGuardCheckTime ) > GAMEGUARD_CHECK_WAITING_TIME)
		{
			SendFieldMsg(ON_RESPONSE_CHECK_GAMEGUARD, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
			m_bCheckPass = TRUE;
			LogOut();
			return;
		}
	}
#endif
}

VOID SOPlayer::RecvRequestCheckGameGuard(VOID)
{
	if(m_pCSAuth == NULL) return;

#ifdef _GAMEGUARD
	OnCheckGameGuardMsg *pCheckGameGuard;

	pCheckGameGuard	= ( OnCheckGameGuardMsg* )cMsg;

	if((m_pCSAuth->CheckAuthDword(pCheckGameGuard->dwGameGuardKey) == TRUE) ||
		(pCheckGameGuard->dwGameGuardKey == GAMEGUARD_PASSKEY))
	{
		SendFieldMsg(ON_RESPONSE_CHECK_GAMEGUARD, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
		m_bCheckPass = TRUE;
		return;
	}
#endif
	m_bCheckPass = FALSE;
}

VOID SOPlayer::RecvUserInfoGameGuard(VOID)
{
	OnReqGameGuardUserInfo *pOnReqGameGuardUserInfo;

	pOnReqGameGuardUserInfo	= ( OnReqGameGuardUserInfo* )cMsg;
	pOnReqGameGuardUserInfo->szMemo[sizeof(pOnReqGameGuardUserInfo->szMemo)-1] = '\0';

	if(uiState == ON_STATE_LOGIN)
	{
		writeInfoToFile("GameGuard-ID.txt", "[Account ID] : %s, Character Name : %s, IP : %s Memo : %s",
			stAccountInfo.id, logininfo->charBaseInfo.name, stAccountInfo.IP, pOnReqGameGuardUserInfo->szMemo);
	}
	else
	{
		writeInfoToFile("GameGuard-IP.txt", "IP : %s Memo : %s",
			stAccountInfo.IP, pOnReqGameGuardUserInfo->szMemo);
	}
}


/*
VOID SOPlayer::CheckGameGuard(VOID)
{
#ifdef _GAMEGUARD
	if(m_pCSAuth == NULL) return;

	if( GetTickCount() - m_lastGameGuardCheckTime > GAMEGUARD_CHECK_TIME )
	{
		if( ( pMyWorld->m_bGameGuard == TRUE ) ) // && ( IsAdminPlayer() == false ) )
		{
			// 파티에서 나가는데 성공했다.
			m_lastGameGuardCheckTime = 0;
			SendFieldMsg( ON_CHECK_GAMEGUARD, SO_SFM_ONLYME, (LPVOID)m_pCSAuth->GetAuthDword());
				// LPVOID( ON_RET_OK ), LPVOID( szID ) );		
		}
	}
#endif
}
*/