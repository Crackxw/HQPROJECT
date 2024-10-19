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
#include "../SCCommon/GSCProtocol.h"		// 2003.07.8 �� �߰��� , ������
#include "../SCCommon/IOnlineFollower.h"	// 2003.07.8 �� �߰��� , ������
#include "../party/IPartyMgr.h"

#include <assert.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��� �޼����� ó���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::ProcessMessage()
{
	OnMsg		*pMsg;
	BOOL		bProcessMessage = FALSE;
	DWORD		dwCmd;


	CBlackPig	encr;

	// �÷��̾ �α��ε� �����϶�
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
			// ���� ��Ŷ�� ��쿡�� ��ȣȭ ����
			// �۾��� ��Ŷ�� ��ȣȭ ���� 

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

		// �÷��̾ �ʵ忡 ������
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE )
		//if(siStatus	 & ON_PLAYER_STATUS_INFIELD)
		{				
			bProcessMessage	= ProcessMessageInField();
			
			if(bProcessMessage == FALSE)
				bProcessMessage = ProcessPartyMessage();
		}

		// �÷��̾ ������ ������
		//if(stAccountInfo.pVillage)
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE )
		{
			if(bProcessMessage == FALSE)
				bProcessMessage		=	ProcessMessageInVillage();
		}

		// �÷��̾ �������϶�
		//if(siStatus==ON_PLAYER_STATUS_INBATTLE)
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INBATTLE ) == TRUE )
		{				
			bProcessMessage		=	ProcessMessageInBattle();
		}
		
		if(bProcessMessage == FALSE)
		{
			if(ProcessMessageCommonly() == FALSE)
			{
				// ���⿡���� ���۸� �޽����� ó������ �ʾҴٸ�
				// �޽����� �׳� �����.
			}
		}
	}

}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �ʵ忡 ���õ� �޼����� ó���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::ProcessMessageInField()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{	
	// ��ż� ���� ó�� 	
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

	// �ڿ� Ž�� ��û 
	case BURIED::REQUEST_PROBE_AREA:
		recvProbeArea();
		break;

	// �̵��Ѵ�.
	case ON_REQUEST_POSITION:
		RecvPosition();
		break;

	case ON_REQUEST_CHAR_DETAILINFO:
		RecvCharDetailInfo();
		break;

	// �α׾ƿ��Ѵ�.
	case ON_REQUEST_LOGOUT:
		LogOut();
		break;		

	// Ư�� �÷��̾��� ������ ��û�Ѵ�.
	case ON_REQUEST_PLAYER_INFO:
		RecvPlayerInfo();
		break;		

	// ������ ����.
	case ON_REQUEST_ENTER_VILLAGE:
		RecvEnterVillage();
		break;		
	
	// ������ ��û�Ѵ�.
	case ON_REQUEST_BATTLE:
		RecvBattle();
		break;

	// �������� ������.
	case ON_REQUEST_THROWITEM:
		RecvThrowItem();
		break;

	// �������� �ݴ´�.
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

/*	// ���� �Ÿ������� �޼���
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

	//������ ���� ���¸� �����Ҽ� �ִ� �������� �˾ƿ´�.
	case ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK:
//		RecvVillageAttackTypeCheck();
		break;

	// ���� ���� ���¸� �����Ѵ�.(�⸧)
	case ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK:
//		RecvVillagePlayerOilAttack();
		break;
	// ���� ���� ���¸� �����Ѵ�. (��)
	case ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK:
//		RecvVillagePlayerStoneAttack();
		break;
	// ���� ���� ���¸� �����Ѵ�.(���� ����)
	case ON_REQUEST_VILLAGE_DEFENCE:
		RecvPlayerVillageDefenceUp();
		break;
	//���� ���� �÷��̾�->�� �߰� �κ�.
	case ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY:
		RecvLongDistanceAttackVillageReady();		
		break;
	case ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION:
		RecvLongDistanceAttackVillageAction();
		break;

	//���޼������� ����ǰ�� ���´�.
	case ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT:
		RecvHeal();
//		RecvGetSupplyAtGuildUnit();
		break;

	//���� ���� �ʵ� ���� �߰� �κ�.
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
	//������������ ������ ��û�Ѵ�.
	case ON_REQUEST_CHANGE_WAR:
		RecvChangeUnit();
		break;
	case ON_REQUEST_CHARGE_MP:
		RecvChargeMP();
		break;
    //���� ���⸦ ��û�Ͽ���
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
		// Ư�� ������ ������ ����Ʈ�� ��û �޾Ҵ�. (������ ��û������ ó���ϴ� �޽������µ� ��ο��� ���� �޽����� ����ϱ� ������ field���� ����.)
		RecvGovernmentInvestmentGetList();
		break;		

	case ON_REQUEST_MACRO_USING_USER_INFO : 		
		RecvMacroUsingUserInfo();
		break;
	

	// �˼� ���� ����� ��������.
	default:
		bProcessMsg	=	FALSE;		
		break;
	}

	return bProcessMsg;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������ ���õ� �޼����� ó���Ѵ�.
//	���� ����	:	2002-02-27 ���� 2:44:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::ProcessMessageInVillage()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{	
	// �ָ� ������ ����Ʈ �� ��������

	case ON_REQUEST_ITEMLIST:
		RecvItemList();
		break;

	case ON_REQUEST_TAKEOUTITEM:
		RecvTakeOutItem();
		break;

	case ON_REQUEST_SAVEITEM:
		RecvSaveItemIntoItemBank();
		break;




	// �������� �����۵��� ����Ʈ�� �䱸�Ѵ�.
	case ON_REQUEST_LIST_ITEM:
		RecvListItem();
		break;
		
	// �������� ���.
	case ON_REQUEST_BUY_ITEM:
		RecvBuyItem();
		break;
		
	// �������� �Ǵ�.
	case ON_REQUEST_SELL_ITEM:
		RecvSellItem();
		break;
					
	// �������� ������.
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
	
	// ����
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
	
	// ���忡 ���� �߰��� �޼���.
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
		// �������� ä�ù��� �����.
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
		// �Խ��ǿ� ����.
		RecvEnterNoticeBoard();
		break;
	case ON_REQUEST_REGIST_NOTICE:
		// �Խ��ǿ� ���� ��Ͻ�Ų��.
		RecvRegistNotice();
		break;
	case ON_REQUEST_DELETE_NOTICE:
		// �Խ����� ���� �����Ѵ�.
		RecvDeleteNotice();
		break;
	case ON_REQUEST_NOTICEBOARD_NOTICELIST:
		// �Խ����� ���� ���´�.
		RecvNoticesList();
		break;
	case ON_REQUEST_VIEW_NOTICE:
		// �Խ����� ������ ���´�.
		RecvViewNotice();
		break;
	case ON_REQUEST_MODIFY_NOTICE:
		// �Խ����� ������ �����Ѵ�.
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
		// �뺴 List�� ��û�Ѵ�.
		RecvBarrackGetSolderList();
		break;
	case ON_REQUEST_BARRACK_BUYSOLDER:
		// �뺴�� ����Ѵ�.
		RecvBarrackBuySolder();
		break;
	case ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST:
		// �ذ��� �뺴 List�� ��û�Ѵ�.
		RecvBarrackGetDismissalSolderList();
		break;
	case ON_REQUEST_BARRACK_DISMISSALSOLDER:
		// �뺴�� �ذ��Ѵ�.
		RecvBarrackDismissalSolder();
		break;
	case ON_REQUEST_GBARRACK_GETSOLDERLIST:
		// ����뺴 List�� ��û�Ѵ�.
		RecvGBarrackGetSolderList();
		break;
	case ON_REQUEST_GBARRACK_BUYSOLDER:
		// ����뺴�� ����Ѵ�.
		RecvGBarrackBuySolder();
		break;
	case ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST:
		// ������ ����뺴 List�� ��û�Ѵ�.
		RecvGBarrackGetChangeJobSolderList();
		break;
	case ON_REQUEST_GBARRACK_CHANGEJOBSOLDER:
		// ����뺴�� �����Ѵ�.
		RecvGBarrackChangeJobSolder();
		break;
	case ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST:
		// �ذ��� ����뺴 List�� ��û�Ѵ�.
		RecvGBarrackGetDismissalSolderList();
		break;
	case ON_REQUEST_GBARRACK_DISMISSALSOLDER:
		// ����뺴�� �ذ��Ѵ�.
		RecvGBarrackDismissalSolder();
		break;
	///////////////////////////////////////////////////////////////////////////////////
	case ON_REQUEST_GBARRACK_GET_GENERAL_LIST:
		// 2�� ��� List ��û.
		RecvGBarrackGetGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_BUY_GENERAL:
		//2�� ��� ���
		RecvGBarrackBuyGeneralEx();
		break;
	case ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST:
		//2�� ��� ���� List ��û 
		RecvGBarrackGetChangeGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_CHANGE_GENERAL:
		//2�� ��� ����
		RecvGBarrackChangeGeneralEx();
		break;
	case ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST:
		//2�� ��� �ذ� List ��û
		RecvGBarrackGetDismissalGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_DISMISSAL_GENERAL:
		//2�� ��� �ذ� 
		RecvGBarrackDismissalGeneralEx();
		break;
	////////////////////////////////////////////////////////////////////////////////////
	case ON_REQUEST_MBARRACK_GETMONSTERLIST:
		// ���� List�� ��û�Ѵ�.
		RecvMBarrackGetMonsterList();
		break;
	case ON_REQUEST_MBARRACK_BUYMONSTER:
		// ���͸� ����Ѵ�.
		RecvMBarrackBuyMonster();
		break;
	case ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST:
		// �ذ��� ���� List�� ��û�Ѵ�.
		RecvMBarrackGetDismissMonsterList();
		break;
	case ON_REQUEST_MBARRACK_DISMISSMONSTER:
		// ���͸� �ذ��Ѵ�.
		RecvMBarrackDismissMonster();
		break;
	case ON_REQUEST_HOSPITAL_GETCURELIST:
		// �뺴 ġ��List�� ��û�Ѵ�.
		RecvHospitalGetCureList();
		break;
	case ON_REQUEST_HOSPITAL_CURESOLDER:
		// �뺴�� ġ���Ѵ�.
		RecvHospitalCureSolder();
		break;
	case ON_REQUEST_HOSPITAL_CUREALLSOLDER:
		// ��� �뺴�� ġ���Ѵ�.
		RecvHospitalCureAllSolder();
		break;
	case ON_REQUEST_WHARF_ENTER:
		// �εο� ����.
		RecvWharfEnter();
		break;
	case ON_REQUEST_WHARF_BUYTICKET:
		// �εο��� ǥ�� ���.
		RecvWharfBuyTicket();
		break;
	case ON_REQUEST_WHARF_EXIT:
		// �εο��� ������.
		RecvWharfExit();
		break;
	case ON_REQUEST_WHARFWAITROOM_ENTER:
		// �εδ��ǿ� ����.
		RecvWharfWaitRoomEnter();
		break;
	case ON_REQUEST_WHARFWAITROOM_EXIT:
		// �εδ��ǿ��� ������.
		RecvWharfWaitRoomExit();
		break;
	case ON_REQUEST_GOVERNMENT_MAIN_ENTER:
		// ��û ����
		RecvGovernmentEnter();
		break;
	case ON_REQUEST_GOVERNMENT_MAIN_LEAVE:
		// ��û ������
		RecvGovernmentLeave();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER:
		// ��û, ���� ����
		RecvGovernmentInvestmentEnter();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE:
		// ��û, ���� ������
		RecvGovernmentInvestmentLeave();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST:
		// ��û, ���� List���
		RecvGovernmentInvestmentGetList();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT:
		// ��û, ���� �ϱ�
		RecvGovernmentInvestmentSetInvestment();
		break;
	case ON_REQUEST_GOVERNMENT_VILLAGEINFO:
		// ��û, �������� ���
		RecvGovernmentVillageInfo();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT:
		// ��û, �����ѵ� ������ ������
		RecvGovernmentInvestmentGetOut();
		break;
	case ON_REQUEST_PROFIT_STATISTICS:
		RecvProfitStatistics();
		break;
	case ON_REQUEST_GUILD_CREATE_COST:
		// ��� ���� ��� ���� ��û
		RecvReqGuildCreateCost();
		break;
	case ON_REQUEST_CREATE_GUILD:
		// ��� ���� ��û
		RecvCreateGuild();
		break;
	case ON_REQUEST_DELETE_GUILD:
		// ��� ���� ��û
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
	case ON_REQUEST_GUILD_LIST:				//����� ����Ʈ�� �޾ƿ´�.
		RecvGuildList();
		break;	
	case ON_REQUEST_GUILD_SET_RELATION:		//����� ������輳��.
		RecvGuildSetRelation();
		break;	
	case ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER:	//ȭģ���� �޼����� ������.
		RecvFriendlyMessageOffer();
		break;
	case ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT:	//ȭģ���Ǹ� �޾Ƶ��δ�.
		RecvFriendlyMessageAccept();
		break;
	case ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL:	//ȭģ���Ǹ� �ô´�.
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
		// �˼� ���� ����� ��������.'		
		bProcessMsg = FALSE;
		break;
	}

	return bProcessMsg;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �������� �޼����� ó���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
				
					// ���� ���´�.
					// �ִ� ������ ���� ������ ����� 7���̴�.
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

						// DB�� �����Ѵ�.
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

	// �˼� ���� ����� ��������.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� �޽����� ó���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	// ��� ���� ��������
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
			if( siVMercenarySlot == 0 )		break;		// ����ٴϴ� �뺴�� ���� ����

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
	// ������ ����
	case ON_REQUEST_REFINE_ITEM:
		RecvRefineItem();

		break;
	case ON_RESPONSE_RUN_PROGRAM_LIST:
		RecvRunProgramList();
		break;
	// ������� / ��ȭ��� ����
	case ON_REQUEST_SET_ATTACK_MODE:
		RecvSetAttackMode();
		break;		

	// �α׾ƿ��Ѵ�.
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
ON_REQUEST_PARTY				// ��Ƽ�� ��û�Ѵ�.
ON_RESPONSE_PARTY				// ��Ƽ�� ��û�Ѵ�.(����)(����)

ON_RECEIVE_ASK_PARTY			// �ش� Ŭ���̾�Ʈ���� �ٸ� Ŭ���̾�Ʈ�� ��Ƽ�� ��û�ߴٴ� ���� �˷��ش�.

ON_REQUEST_ACCEPT_PARTY			// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�. �����ϴ��� ������.
ON_RESPONSE_ACCEPT_PARTY		// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�.(����) ���� ����� ����.
ON_REQUEST_LEAVE_PARTY			// Ŭ���̾�Ʈ�� ��Ƽ�� ������.
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

	// ���� ��Ƽ���� �ƴϰ� �Ϲ� ��Ƽ���� ��쿡�� �ٸ� ������� ��Ƽ ��û�� ��û�� �� ����.
	if( ( IsPartyLeader() == FALSE ) && ( IsPartyMember() == TRUE ) )							goto	END;

	// ���� ���濡�� ��Ƽ�� ��û�� �� �ִ� ���°� �ƴ϶�� �����Ѵ�.
	if( pTargetPlayer->CanRequestJoinParty() == FALSE )											goto	END;	

	// ������ ��Ƽ ��û�� ���� �� �ִ� ���°� �ƴ϶�� �����Ѵ�.
	if( pTargetPlayer->CanReceiveRequestJoinParty() == FALSE )									goto	END;	
	
	RequestJoinParty( pTargetPlayer->uiAccount );
	pTargetPlayer->ReceiveRequestJoinParty( uiAccount );

	bRes	=	TRUE;

END:
	
	if ( bRes == TRUE )
	{
		// ��Ƽ ���� ��û��(�ڱ� �ڽ�)���� ������ �޼���.
		SendFieldMsg( ON_RESPONSE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

		// ��Ƽ ���� ������ ���� ������ �޼��� 
		pTargetPlayer->SendFieldMsg( ON_RECEIVE_ASK_PARTY, SO_SFM_ONLYME, LPVOID( uiAccount ), LPVOID( szID ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO) );
	}

}



/************************************************************************/
// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�. �����ϴ��� ������.
// Ÿ�� Ŭ���̾�Ʈ -> ������ �� �޼��� 

/************************************************************************/
VOID SOPlayer::RecvRequestAcceptParty()
{
	OnResAskPartyMsg	*pMsg;

	pMsg	=	( OnResAskPartyMsg* )cMsg;

	switch( pMsg->dwResponse )
	{
	case ON_RET_OK:
		
		// ��Ƽ�� �����ϴµ� �����Ͽ���.
		AcceptRequestingJoinParty();

		break;

	case ON_RET_NO:

		// ��Ƽ�� �����ϴµ� �ź��Ͽ���.
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
		// ��Ƽ���� �����µ� �����ߴ�.
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( szID ) );		
	}	
	else
	{
		// ��Ƽ���� �����µ� �����ߴ�.
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );		
	}

}

/*
ON_REQUEST_PARTY				// ��Ƽ�� ��û�Ѵ�.
ON_RESPONSE_PARTY				// ��Ƽ�� ��û�Ѵ�.(����)(����)

ON_RECEIVE_ASK_PARTY			// �ش� Ŭ���̾�Ʈ���� �ٸ� Ŭ���̾�Ʈ�� ��Ƽ�� ��û�ߴٴ� ���� �˷��ش�.

ON_REQUEST_ACCEPT_PARTY			// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�. �����ϴ��� ������.
ON_RESPONSE_ACCEPT_PARTY		// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�.(����) ���� ����� ����.
ON_REQUEST_LEAVE_PARTY			// Ŭ���̾�Ʈ�� ��Ƽ�� ������.
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
			// ��Ƽ ��û �޼���
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

	// ��Ƽ ��û�� ����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// GameGuard����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
#ifdef _GAMEGUARD
#include "CSAuth.h"
#endif

#define GAMEGUARD_CHECK_TIME (1000 * 60 * 5 )
#define GAMEGUARD_CHECK_WAITING_TIME (1000 * 10 * 60)
// #define 

VOID SOPlayer::CheckGameGuard(VOID)
{
// ---- ������ 2005 02 22
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
			// ��Ƽ���� �����µ� �����ߴ�.
			m_lastGameGuardCheckTime = 0;
			SendFieldMsg( ON_CHECK_GAMEGUARD, SO_SFM_ONLYME, (LPVOID)m_pCSAuth->GetAuthDword());
				// LPVOID( ON_RET_OK ), LPVOID( szID ) );		
		}
	}
#endif
}
*/