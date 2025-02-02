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
#include "../SCCommon/GSCProtocol.h"		// 2003.07.8 橾 蹺陛л , 薑撩遴
#include "../SCCommon/IOnlineFollower.h"	// 2003.07.8 橾 蹺陛л , 薑撩遴
#include "../party/IPartyMgr.h"

#include <assert.h>

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 賅萇 詭撮雖蒂 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOPlayer::ProcessMessage()
{
	OnMsg		*pMsg;
	BOOL		bProcessMessage = FALSE;
	DWORD		dwCmd;


	CBlackPig	encr;

	// Ы溯檜橫陛 煎斜檣脹 鼻鷓橾陽
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
			// 瞪癱 ぬ韃橾 唳辦縑虜 懍���� п薯
			// 濛機榆 ぬ韃紫 懍���� п薯 

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

		// Ы溯檜橫陛 в萄縑 氈擊陽
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE )
		//if(siStatus	 & ON_PLAYER_STATUS_INFIELD)
		{				
			bProcessMessage	= ProcessMessageInField();
			
			if(bProcessMessage == FALSE)
				bProcessMessage = ProcessPartyMessage();
		}

		// Ы溯檜橫陛 葆擊縑 氈擊陽
		//if(stAccountInfo.pVillage)
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INVILLAGE ) == TRUE )
		{
			if(bProcessMessage == FALSE)
				bProcessMessage		=	ProcessMessageInVillage();
		}

		// Ы溯檜橫陛 瞪癱醞橾陽
		//if(siStatus==ON_PLAYER_STATUS_INBATTLE)
		if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INBATTLE ) == TRUE )
		{				
			bProcessMessage		=	ProcessMessageInBattle();
		}
		
		if(bProcessMessage == FALSE)
		{
			if(ProcessMessageCommonly() == FALSE)
			{
				// 罹晦縑憮紫 幗ぷ蒂 詭衛雖蒂 籀葬ж雖 彊懊棻賊
				// 詭衛雖蒂 斜傖 雖遴棻.
			}
		}
	}

}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// в萄縑 婦溼脹 詭撮雖蒂 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::ProcessMessageInField()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{	
	// 唳衙模 婦溼 籀葬 	
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

	// 濠錳 鬚餌 蹂羶 
	case BURIED::REQUEST_PROBE_AREA:
		recvProbeArea();
		break;

	// 檜翕и棻.
	case ON_REQUEST_POSITION:
		RecvPosition();
		break;

	case ON_REQUEST_CHAR_DETAILINFO:
		RecvCharDetailInfo();
		break;

	// 煎斜嬴醒и棻.
	case ON_REQUEST_LOGOUT:
		LogOut();
		break;		

	// か薑 Ы溯檜橫曖 薑爾蒂 蹂羶и棻.
	case ON_REQUEST_PLAYER_INFO:
		RecvPlayerInfo();
		break;		

	// 葆擊縑 菟橫除棻.
	case ON_REQUEST_ENTER_VILLAGE:
		RecvEnterVillage();
		break;		
	
	// 瞪癱蒂 蹂羶и棻.
	case ON_REQUEST_BATTLE:
		RecvBattle();
		break;

	// 嬴檜蠱擊 幗萼棻.
	case ON_REQUEST_THROWITEM:
		RecvThrowItem();
		break;

	// 嬴檜蠱擊 鄹朝棻.
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

/*	// 盪濠 剪葬縑憮曖 詭撮雖
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

	//葆擊曖 奢問 ⑽鷓蒂 撲薑й熱 氈朝 鼻鷓檣雖 憲嬴螞棻.
	case ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK:
//		RecvVillageAttackTypeCheck();
		break;

	// 撩曖 奢問 ⑽鷓蒂 撲薑и棻.(晦葷)
	case ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK:
//		RecvVillagePlayerOilAttack();
		break;
	// 撩曖 奢問 ⑽鷓蒂 撲薑и棻. (給)
	case ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK:
//		RecvVillagePlayerStoneAttack();
		break;
	// 撩曖 奢問 ⑽鷓蒂 撲薑и棻.(葆擊 熱綠煽)
	case ON_REQUEST_VILLAGE_DEFENCE:
		RecvPlayerVillageDefenceUp();
		break;
	//奢撩 婦溼 Ы溯檜橫->撩 蹺陛 睡碟.
	case ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY:
		RecvLongDistanceAttackVillageReady();		
		break;
	case ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION:
		RecvLongDistanceAttackVillageAction();
		break;

	//爾晝熱溯縑憮 爾晝ヶ擊 橢橫螞棻.
	case ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT:
		RecvHeal();
//		RecvGetSupplyAtGuildUnit();
		break;

	//奢撩 婦溼 в萄 瞪癱 蹺陛 睡碟.
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
	//奢撩嶸棉戲煎 滲褐擊 蹂羶и棻.
	case ON_REQUEST_CHANGE_WAR:
		RecvChangeUnit();
		break;
	case ON_REQUEST_CHARGE_MP:
		RecvChargeMP();
		break;
    //瞪癱 ん晦蒂 蹂羶ж艘棻
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
		// か薑 葆擊曖 癱濠濠 葬蝶お蒂 蹂羶 嫡懊棻. (錳楚朝 婦羶縑憮虜 籀葬ж湍 詭衛雖艘朝等 濰睡縑憮 偽擎 詭衛雖蒂 餌辨ж晦 陽僥縑 field縑紫 趕棻.)
		RecvGovernmentInvestmentGetList();
		break;		

	case ON_REQUEST_MACRO_USING_USER_INFO : 		
		RecvMacroUsingUserInfo();
		break;
	

	// 憲熱 橈朝 貲滄檜 菟橫諮擊陽.
	default:
		bProcessMsg	=	FALSE;		
		break;
	}

	return bProcessMsg;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊縑 婦溼脹 詭撮雖蒂 籀葬и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 2:44:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::ProcessMessageInVillage()
{
	OnMsg		*pMsg;
	BOOL		bProcessMsg = TRUE;

	pMsg	=	(OnMsg*)cMsg;

	switch(pMsg->dwMsgType)
	{	
	// 輿虞 嬴檜蠱 葬蝶お 塽 陛螳螃晦

	case ON_REQUEST_ITEMLIST:
		RecvItemList();
		break;

	case ON_REQUEST_TAKEOUTITEM:
		RecvTakeOutItem();
		break;

	case ON_REQUEST_SAVEITEM:
		RecvSaveItemIntoItemBank();
		break;




	// 鼻薄縑憮 嬴檜蠱菟曖 葬蝶お蒂 蹂掘и棻.
	case ON_REQUEST_LIST_ITEM:
		RecvListItem();
		break;
		
	// 嬴檜蠱擊 骯棻.
	case ON_REQUEST_BUY_ITEM:
		RecvBuyItem();
		break;
		
	// 嬴檜蠱擊 っ棻.
	case ON_REQUEST_SELL_ITEM:
		RecvSellItem();
		break;
					
	// 葆擊縑憮 集陪棻.
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
	
	// 堯濰
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
	
	// 堯濰縑 億煎 蹺陛脹 詭撮雖.
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
		// 輿薄縑憮 瓣た寞擊 虜萇棻.
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
		// 啪衛っ縑 菟橫除棻.
		RecvEnterNoticeBoard();
		break;
	case ON_REQUEST_REGIST_NOTICE:
		// 啪衛っ縑 旋擊 蛔煙衛鑑棻.
		RecvRegistNotice();
		break;
	case ON_REQUEST_DELETE_NOTICE:
		// 啪衛っ曖 旋擊 餉薯и棻.
		RecvDeleteNotice();
		break;
	case ON_REQUEST_NOTICEBOARD_NOTICELIST:
		// 啪衛っ曖 旋擊 橢橫螞棻.
		RecvNoticesList();
		break;
	case ON_REQUEST_VIEW_NOTICE:
		// 啪衛っ曖 獄僥擊 橢橫螞棻.
		RecvViewNotice();
		break;
	case ON_REQUEST_MODIFY_NOTICE:
		// 啪衛っ曖 獄僥擊 熱薑и棻.
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
		// 辨煽 List蒂 蹂羶и棻.
		RecvBarrackGetSolderList();
		break;
	case ON_REQUEST_BARRACK_BUYSOLDER:
		// 辨煽擊 堅辨и棻.
		RecvBarrackBuySolder();
		break;
	case ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST:
		// п堅й 辨煽 List蒂 蹂羶и棻.
		RecvBarrackGetDismissalSolderList();
		break;
	case ON_REQUEST_BARRACK_DISMISSALSOLDER:
		// 辨煽擊 п堅и棻.
		RecvBarrackDismissalSolder();
		break;
	case ON_REQUEST_GBARRACK_GETSOLDERLIST:
		// 濰熱辨煽 List蒂 蹂羶и棻.
		RecvGBarrackGetSolderList();
		break;
	case ON_REQUEST_GBARRACK_BUYSOLDER:
		// 濰熱辨煽擊 堅辨и棻.
		RecvGBarrackBuySolder();
		break;
	case ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST:
		// 瞪霜й 濰熱辨煽 List蒂 蹂羶и棻.
		RecvGBarrackGetChangeJobSolderList();
		break;
	case ON_REQUEST_GBARRACK_CHANGEJOBSOLDER:
		// 濰熱辨煽擊 瞪霜и棻.
		RecvGBarrackChangeJobSolder();
		break;
	case ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST:
		// п堅й 濰熱辨煽 List蒂 蹂羶и棻.
		RecvGBarrackGetDismissalSolderList();
		break;
	case ON_REQUEST_GBARRACK_DISMISSALSOLDER:
		// 濰熱辨煽擊 п堅и棻.
		RecvGBarrackDismissalSolder();
		break;
	///////////////////////////////////////////////////////////////////////////////////
	case ON_REQUEST_GBARRACK_GET_GENERAL_LIST:
		// 2離 濰熱 List 蹂羶.
		RecvGBarrackGetGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_BUY_GENERAL:
		//2離 濰熱 堅辨
		RecvGBarrackBuyGeneralEx();
		break;
	case ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST:
		//2離 濰熱 瞪霜 List 蹂羶 
		RecvGBarrackGetChangeGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_CHANGE_GENERAL:
		//2離 濰熱 瞪霜
		RecvGBarrackChangeGeneralEx();
		break;
	case ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST:
		//2離 濰熱 п堅 List 蹂羶
		RecvGBarrackGetDismissalGeneralExList();
		break;
	case ON_REQUEST_GBARRACK_DISMISSAL_GENERAL:
		//2離 濰熱 п堅 
		RecvGBarrackDismissalGeneralEx();
		break;
	////////////////////////////////////////////////////////////////////////////////////
	case ON_REQUEST_MBARRACK_GETMONSTERLIST:
		// 跨蝶攪 List蒂 蹂羶и棻.
		RecvMBarrackGetMonsterList();
		break;
	case ON_REQUEST_MBARRACK_BUYMONSTER:
		// 跨蝶攪蒂 堅辨и棻.
		RecvMBarrackBuyMonster();
		break;
	case ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST:
		// п堅й 跨蝶攪 List蒂 蹂羶и棻.
		RecvMBarrackGetDismissMonsterList();
		break;
	case ON_REQUEST_MBARRACK_DISMISSMONSTER:
		// 跨蝶攪蒂 п堅и棻.
		RecvMBarrackDismissMonster();
		break;
	case ON_REQUEST_HOSPITAL_GETCURELIST:
		// 辨煽 纂猿List蒂 蹂羶и棻.
		RecvHospitalGetCureList();
		break;
	case ON_REQUEST_HOSPITAL_CURESOLDER:
		// 辨煽擊 纂猿и棻.
		RecvHospitalCureSolder();
		break;
	case ON_REQUEST_HOSPITAL_CUREALLSOLDER:
		// 賅萇 辨煽擊 纂猿и棻.
		RecvHospitalCureAllSolder();
		break;
	case ON_REQUEST_WHARF_ENTER:
		// 睡舒縑 菟橫除棻.
		RecvWharfEnter();
		break;
	case ON_REQUEST_WHARF_BUYTICKET:
		// 睡舒縑憮 ル蒂 骯棻.
		RecvWharfBuyTicket();
		break;
	case ON_REQUEST_WHARF_EXIT:
		// 睡舒縑憮 釭除棻.
		RecvWharfExit();
		break;
	case ON_REQUEST_WHARFWAITROOM_ENTER:
		// 睡舒渠晦褒縑 菟橫除棻.
		RecvWharfWaitRoomEnter();
		break;
	case ON_REQUEST_WHARFWAITROOM_EXIT:
		// 睡舒渠晦褒縑憮 釭除棻.
		RecvWharfWaitRoomExit();
		break;
	case ON_REQUEST_GOVERNMENT_MAIN_ENTER:
		// 婦羶 菟橫陛晦
		RecvGovernmentEnter();
		break;
	case ON_REQUEST_GOVERNMENT_MAIN_LEAVE:
		// 婦羶 釭陛晦
		RecvGovernmentLeave();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER:
		// 婦羶, 癱濠 菟橫陛晦
		RecvGovernmentInvestmentEnter();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE:
		// 婦羶, 癱濠 釭陛晦
		RecvGovernmentInvestmentLeave();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST:
		// 婦羶, 癱濠 List橢晦
		RecvGovernmentInvestmentGetList();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT:
		// 婦羶, 癱濠 ж晦
		RecvGovernmentInvestmentSetInvestment();
		break;
	case ON_REQUEST_GOVERNMENT_VILLAGEINFO:
		// 婦羶, 葆擊薑爾 橢晦
		RecvGovernmentVillageInfo();
		break;
	case ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT:
		// 婦羶, 癱濠и絲 陛雖堅 釭陛晦
		RecvGovernmentInvestmentGetOut();
		break;
	case ON_REQUEST_PROFIT_STATISTICS:
		RecvProfitStatistics();
		break;
	case ON_REQUEST_GUILD_CREATE_COST:
		// 鼻欽 偃撲 綠辨 薑爾 蹂羶
		RecvReqGuildCreateCost();
		break;
	case ON_REQUEST_CREATE_GUILD:
		// 鼻欽 偃撲 蹂羶
		RecvCreateGuild();
		break;
	case ON_REQUEST_DELETE_GUILD:
		// 鼻欽 餉薯 蹂羶
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
	case ON_REQUEST_GUILD_LIST:				//鼻欽曖 葬蝶お蒂 嫡嬴螞棻.
		RecvGuildList();
		break;	
	case ON_REQUEST_GUILD_SET_RELATION:		//鼻欽曖 瞳渠婦啗撲薑.
		RecvGuildSetRelation();
		break;	
	case ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER:	//�俸�薯曖 詭撮雖蒂 爾魚棻.
		RecvFriendlyMessageOffer();
		break;
	case ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT:	//�俸�薯曖蒂 嫡嬴菟檣棻.
		RecvFriendlyMessageAccept();
		break;
	case ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL:	//�俸�薯曖蒂 壅朝棻.
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
		// 憲熱 橈朝 貲滄檜 菟橫諮擊陽.'		
		bProcessMsg = FALSE;
		break;
	}

	return bProcessMsg;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 瞪癱醞曖 詭撮雖蒂 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
				
					// 瞳擊 橢橫螞棻.
					// 譆渠 瞪癱縑 霤罹 陛棟и 餌塋擎 7貲檜棻.
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

						// DB縑 盪濰и棻.
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

	// 憲熱 橈朝 貲滄檜 菟橫諮擊陽.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	奢鱔瞳檣 詭衛雖蒂 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	// 濰睡 婦溼 Щ煎饜屬
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
			if( siVMercenarySlot == 0 )		break;		// 評塭棻棲朝 辨煽檜 橈朝 鼻鷓

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
	// 嬴檜蠱 薯溼
	case ON_REQUEST_REFINE_ITEM:
		RecvRefineItem();

		break;
	case ON_RESPONSE_RUN_PROGRAM_LIST:
		RecvRunProgramList();
		break;
	// 瞪癱賅萄 / ゎ�飛藒� 撲薑
	case ON_REQUEST_SET_ATTACK_MODE:
		RecvSetAttackMode();
		break;		

	// 煎斜嬴醒и棻.
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
ON_REQUEST_PARTY				// だじ蒂 蹂羶и棻.
ON_RESPONSE_PARTY				// だじ蒂 蹂羶и棻.(擬港)(憮幗)

ON_RECEIVE_ASK_PARTY			// п渡 贗塭檜樹お縑啪 棻艇 贗塭檜樹お陛 だじ蒂 蹂羶ц棻朝 匙擊 憲溥遽棻.

ON_REQUEST_ACCEPT_PARTY			// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻. 翕曖ж替傍 葆替傍.
ON_RESPONSE_ACCEPT_PARTY		// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻.(擬港) 翕曖 唸婁縑 評塭.
ON_REQUEST_LEAVE_PARTY			// 贗嬴檜樹お陛 だじ蒂 集陪棻.
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

	// 頂陛 だじ濰檜 嬴棲堅 橾奩 だじ錳橾 唳辦縑朝 棻艇 餌塋縑啪 だじ 褐羶擊 蹂羶й 熱 橈棻.
	if( ( IsPartyLeader() == FALSE ) && ( IsPartyMember() == TRUE ) )							goto	END;

	// 頂陛 鼻渠寞縑啪 だじ蒂 蹂羶й 熱 氈朝 鼻鷓陛 嬴棲塭賊 褒ぬи棻.
	if( pTargetPlayer->CanRequestJoinParty() == FALSE )											goto	END;	

	// 鼻渠寞檜 だじ 蹂羶擊 嫡擊 熱 氈朝 鼻鷓陛 嬴棲塭賊 褒ぬи棻.
	if( pTargetPlayer->CanReceiveRequestJoinParty() == FALSE )									goto	END;	
	
	RequestJoinParty( pTargetPlayer->uiAccount );
	pTargetPlayer->ReceiveRequestJoinParty( uiAccount );

	bRes	=	TRUE;

END:
	
	if ( bRes == TRUE )
	{
		// だじ 陛殮 褐羶濠(濠晦 濠褐)縑啪 爾頂朝 詭撮雖.
		SendFieldMsg( ON_RESPONSE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );

		// だじ 陛殮 擬港濠 縑啪 爾頂朝 詭撮雖 
		pTargetPlayer->SendFieldMsg( ON_RECEIVE_ASK_PARTY, SO_SFM_ONLYME, LPVOID( uiAccount ), LPVOID( szID ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO) );
	}

}



/************************************************************************/
// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻. 翕曖ж替傍 葆替傍.
// 顫啃 贗塭檜樹お -> 憮幗煎 螞 詭撮雖 

/************************************************************************/
VOID SOPlayer::RecvRequestAcceptParty()
{
	OnResAskPartyMsg	*pMsg;

	pMsg	=	( OnResAskPartyMsg* )cMsg;

	switch( pMsg->dwResponse )
	{
	case ON_RET_OK:
		
		// だじ縑 霤罹ж朝等 翕曖ж艘棻.
		AcceptRequestingJoinParty();

		break;

	case ON_RET_NO:

		// だじ縑 霤罹ж朝等 剪睡ж艘棻.
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
		// だじ縑憮 釭陛朝等 撩奢ц棻.
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( szID ) );		
	}	
	else
	{
		// だじ縑憮 釭陛朝等 褒ぬц棻.
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );		
	}

}

/*
ON_REQUEST_PARTY				// だじ蒂 蹂羶и棻.
ON_RESPONSE_PARTY				// だじ蒂 蹂羶и棻.(擬港)(憮幗)

ON_RECEIVE_ASK_PARTY			// п渡 贗塭檜樹お縑啪 棻艇 贗塭檜樹お陛 だじ蒂 蹂羶ц棻朝 匙擊 憲溥遽棻.

ON_REQUEST_ACCEPT_PARTY			// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻. 翕曖ж替傍 葆替傍.
ON_RESPONSE_ACCEPT_PARTY		// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻.(擬港) 翕曖 唸婁縑 評塭.
ON_REQUEST_LEAVE_PARTY			// 贗嬴檜樹お陛 だじ蒂 集陪棻.
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
			// だじ 蹂羶 詭撮雖
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

	// だじ 蹂羶擊 鏃模и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// GameGuard婦溼
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#ifdef _GAMEGUARD
#include "CSAuth.h"
#endif

#define GAMEGUARD_CHECK_TIME (1000 * 60 * 5 )
#define GAMEGUARD_CHECK_WAITING_TIME (1000 * 10 * 60)
// #define 

VOID SOPlayer::CheckGameGuard(VOID)
{
// ---- 彌艙渠 2005 02 22
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
			// だじ縑憮 釭陛朝等 撩奢ц棻.
			m_lastGameGuardCheckTime = 0;
			SendFieldMsg( ON_CHECK_GAMEGUARD, SO_SFM_ONLYME, (LPVOID)m_pCSAuth->GetAuthDword());
				// LPVOID( ON_RET_OK ), LPVOID( szID ) );		
		}
	}
#endif
}
*/