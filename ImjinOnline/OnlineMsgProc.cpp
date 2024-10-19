#include <GSL.h>

#include "OnlineGlobal.h"
#include "OnlineMsgProc.h"
#include "Utility.h"

OnlineMsgProc::OnlineMsgProc( cltOnlineWorld *pWorld )
{
	_pWorld	= pWorld; 

	Init_System_Fnc();
	Init_Field_Fnc(); 
	Init_Village_Fnc();
}

// �ý��� �޽��� �Լ� �ʱ�ȭ
void OnlineMsgProc::Init_System_Fnc()
{
	Add( ON_CHECK_GAMEGUARD, OnCheckGameGuard );
	Add( ON_RESPONSE_CHECK_GAMEGUARD, OnResCheckGameGuard );
	Add( ON_RESPONSE_LOGOUT, OnResLogOut );
	Add( ON_CHANGED_TIME, ChangedTime );
	Add( ON_TARGETME, OnTargetMe );
	Add( ON_RELEASEME, OnReleaseMe );
	Add( ON_CHANGED_MONEY, OnChangeMoney );
	Add( ON_OFFICIAL_ANNOUNCEMENT, OnOfficaialAnnouncement );
	Add( ON_TEXT_MSG, OnTextMessageg );	
	Add( ON_RESPONSE_WARP, OnResWarp );
	Add( ON_RESPONSE_INC_ABILITY_BYBONUS, OnResBonusUp );
	Add( ON_HUNGRY, OnHungry );
	Add( ON_RESPONSE_ITEMRECEIVED, OnResItemReceived );
	Add( ON_CHANGE_WEATHER, ChangeWeather );
	Add( ON_REQUEST_RUN_PROGRAM_LIST, RunProgramList );
	Add( ON_MAKE_THUNDER, MakeThunder );		
	Add( ON_RESPONSE_RETURNONLINE, ReturnOnline );	
	Add( ON_RESPONSE_LEAVE_VILLAGE, LeaveVillage );
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST, InvestmentList );
	Add( ON_EVENT_USERRANKINGS, ResRank );

	// robypark 2004/12/6 17:11
	// �������� ����Ǿ���. ��� ������ �������� �κ��丮���� �����ϸ� ���� ���� ���ΰ� �������� �������.
	Add( ON_CHANGED_WAR_END,	OnResChangedWarEnd );

	// robypark 2004/12/7 12:59
	// ���������� �׾ ���� �г�Ƽ�� ������� ������.
	Add( ON_CHANGED_PLAYER_DEAD_PENALTY, OnResChangedPlayerDeadPenalty );

	// robypark 2005/2/15 17:43
	// �������� ȭģ���ǿ� ���ؼ� �������
	Add( ON_RESPONSE_NOT_WAR, OnResponseNotWar );

	// robypark 2005/2/16 2:52
	// ������ �ð��� �Ǿ ����ڰ� ������ �ִٰ� �˷���
	Add( ON_RESPONE_WAR_TIME, OnResponseWarTime );
}

// �ʵ� �޼��� �Լ� �ʱ�ȭ
void OnlineMsgProc::Init_Field_Fnc()
{
	Add( ON_NEWINFOONMOVE, NewInfoOnMove );
	Add( ON_DELCHAR, OnDelChar );
	Add( ON_ADDCHAR, OnAddChar );	
	Add( ON_RESPONSE_ENTER_VILLAGE, OnResEnterVillage );	
	Add( ON_RESPONSE_MOVEMAP, OnResMoveMap );
	Add( ON_RESPONSE_PLAYER_INFO, OnResPlayerInfo );
	Add( ON_RESPONSE_BATTLE, OnResBattle );
	Add( ON_RESPONSE_BATTLEPLAYER, OnResBattlePlay );
	Add( ON_CHANGE_CLOTHES, OnChangeChlothes );
	Add( ON_RESPONSE_READ_DESCRIPT, OnResReadDescrit );	
	Add( ON_RESPONSE_VILLAGE_NATION, OnResVillageNation );
	Add( ON_RESPONSE_IMOTICON, OnResImoticon );

	Add( ON_RESPONSE_PARTY,	OnResParty);
	Add( ON_RECEIVE_ASK_PARTY,OnResAksParty);
	Add( ON_RESPONSE_ACCEPT_PARTY,OnResAcceptParty);
//	Add( ON_RESPONSE_PARTY_LIST,OnResQuitParty);
	Add( ON_CANCEL_REQUESTED_PARTY,OnResPartyCancel);
	Add( ON_RESPONSE_LEAVE_PARTY,OnResLeaveParty);
	Add( ON_PARTY_DELETED,OnResPartyDeleted);
//	Add( ON_PARTY_BE_PARTY_LEADER,OnResPartyLeader);
	Add( ON_RESPONSE_CANCEL_REQUESTING_PARTY,OnResCancelRequest);
//	Add( ON_PARTY_MEMBER_CHANGED,OnResPartyMemberChange);


	Add( ON_RESPONSE_ACCEPT_TRADE, OnResAcceptTrade );
	Add( ON_RESPONSE_TRADE, OnResTrade );
	Add( ON_RECEIVE_ASK_TRADE, OnResAksTrade );
	Add( ON_RESPONSE_CANCEL_TRADE, OnResCancelTrade );	
	Add( ON_RESPONSE_FINISH_CHOOSE_TRADEITEM, OnResFinishTrade );
	Add( ON_RESPONSE_DECIDE_TRADE, OnResDecideTrade );	
	Add( ON_COMPLETE_TRADE, OnCompleteTrade );
	Add( ON_CHANGE_TRADE_GOODS, OnChangeTrade );	
	Add( ON_RESPONSE_ATTACK_VILLAGE, OnResAttackVillage );	
	Add( ON_RESPONSE_POSITION, OnResPosotion );	
	Add( ON_RESPONSE_PI_INMINIMAP, OnResCharInfoMinimap );
	Add( ON_GUILD_MSG, OnGuildMessage );
	Add( ON_RESPONSE_QUEST_SELECTNPC, OnResQuestSelectNpc );
	Add( ON_RESPONSE_QUEST_ACCEPTQUEST, OnResQuestAccept );
	Add( ON_RESPONSE_QUEST_CANCELQUEST, OnResQuestCamcel );	
	Add( ON_RESPONSE_QUEST_GETPLAYQUEST, OnResQuestGetPlay );	
	Add( ON_RESPONSE_QUEST_GETPLAYQUESTDATA, OnResQuestGetPlayData );
	Add( ON_RESPONSE_QUEST_ALLCLEARQUEST, OnResQuestClear );
	Add( ON_PM_CHANGE_BOOTH_ITEM, OnChangeBoothItem );
	Add( ON_RESPONSE_PM_ITEMINFO, OnResChangeBoothItem );
	Add( ON_RESPONSE_CHAR_DETAILINFO, OnResCharDetailInfo );
	Add( ON_CHANGE_FIELDITEM, OnChangeFieldItem );
	Add( BURIED::RESPONSE_PROBE_AREA, OnResProbeArea );	
	Add( ON_RESPONSE_PERSONAL_INFO, OnResPersonal );
	Add( ON_RESPONSE_VMERCENARY_CHANGE, OnVmercenaryChange );	
	Add( ON_RESPONSE_VMERCENARY_CHANGEMAIN, OnVmercenaryChangeMe );	
	Add( ON_CHANGE_CHARACTERCODE, OnChangeCharCode );	
	Add( ON_IMPERIUM_USE,OnImperiumUse);
	Add( ON_RESPONSE_AUDIT_OWN, AuditOwn );
	Add( ON_RESPONSE_AUDIT_GUILD_INFO,GuildInfo);
	Add( ON_MYVILLAGE_ATTACKED, MyVillageAttacked );
	Add( ON_RESPONSE_AUDIT_FRIEND_LIST, FriendList );
	Add( ON_FRIEND_LOGGEDINOUT, FriendLogOut );
	Add( ON_RESPONSE_VILLAGE_INFO, VillageInfo );
	Add( AUCTION::NOTIFY_RESULT, NotifyResult );	
	Add( AUCTION::NOTIFY_ADVERTISE, NotifyAdvertise );	
	Add( AUCTION::RESPONSE_SELL_ARTICLE, SellArticle );
	Add( AUCTION::RESPONSE_BIDDING, Bidding );
	Add( AUCTION::REPORT_REMAIN_TIME, ReportReMainTime );
	Add( AUCTION::REPORT_BIDDING, ReportBidding );
	Add( AUCTION::REPORT_SUCCESSFUL_BID, ReportSuccessful );
	Add( AUCTION::REPORT_FAIL_BID, ReportFail );
	Add( AUCTION::REPORT_NEW_INFO, ReportNew );
	Add( AUCTION::RESPONSE_ENTER, Enter );
	Add( ON_RESPONSE_PM_OPEN, OnResOpen );
	Add( ON_RESPONSE_PM_BUYITEM, OnResBuyItem );
	Add( ON_RESPONSE_PM_CLOSE, OnResClose );	
	Add( ON_RESPONSE_PM_BUYFOLLOWER, OnResBuyFollower );	
	Add( ON_RESPONSE_PM_CHANGEITEM, OnResChangeItem );	
	Add( ON_RESPONSE_AUDIT_SETNAME, OnResSetName );
	Add( ON_RESPONSE_WEARITEM, WearItem );
	Add( ON_RESPONSE_STRIPITEM, StripItem );
	Add( ON_RESPONSE_MOVEITEM, MoveItem );
	Add( ON_RESPONSE_THROWITEM, ThrowItem );
	Add( ON_RESPONSE_PICKUPITEM, PickupItem );
	Add( ON_RESPONSE_SPENDITEM, SpendItem );
	Add( ON_RESPONSE_REFINE_ITEM, RefineItem );	
	Add( ON_RESPONSE_CHANGE_TRADE_GOODS, ChangeTradeGoods );
	Add( ON_RESPONSE_AUDIT_TOTAL, ChangeTradeGoods );
	Add( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, FriendAddOne );
	Add( ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME, FriendRemove );	
	Add( ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL, FriendRemoveAll );		
	Add( ON_RESPONSE_AUDIT_VILLAGE_LIST, VillageList );
//	Add( ON_RESPONSE_AUDIT_PROFIT_STATISTICS, StatiStics );		
	Add( ON_ADD_FOLLOWER,AddFollower);
	Add( ON_RESPONSE_SET_ATTACK_MODE, SetAttackMode );
//	Add( ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO, OnDokdoQuestCompletedUser );// actdoll (2004/03/30 11:12) : = �̺�Ʈ = ���� ����Ʈ �Ϸ��� ����
	Add( ON_RELOAD_WEAR_ITEM, OnBroadCast_ReloadWearItem );	// actdoll (2004/07/19 14:07) : = �ð��� ������ = ������ ������ ���� ����
	Add( ON_RESOPNSE_REAL_TIME, OnBroadCast_CurrentServerTime);	// robypark 2004/9/14 19:33 ���� �ð��� ���޹޾� �̺�Ʈ ����Ʈ�� ��Ȱ��ȭ�� �ش�.

	Add( ON_RESPONSE_FIELDATTACK_READY,FieldAttack_Ready);
	Add( ON_RESPONSE_FIELDATTACK_ACTION,FieldAttack_Action);
	Add( ON_RESPONSE_FIELDATTACK_DEAD,FieldAttack_Dead);

	
	Add( ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY,LongVillageAttack_Ready);
	Add( ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION,LongVillageAttack_Action);

	// �����ϱ� �������̽��� ���� ��û�� ���� ���� ó��
	// ON_RESPONSE_WAR_VILLAGE_DEFENCE
	Add( ON_RESPONSE_WAR_VILLAGE_DEFENCE, OnResponseEnterVillageDefenceInterface );

	/*
	// �����ϱ� �������̽����� ������ ���� ��û�� ���� ���� ó��
	Add ( ON_RESPONSE_WAR_VILLAGE_IN, OnResponseEnterVillageFromVillageDefenceInterface );
	*/

	// �����ϱ� �������̽��� ���´�. �ʵ��
	Add ( ON_RESPONSE_VILLAGE_DEFFNCE_STOP, OnResponseLeaveVillageDefenceInterface );

	// robypark 2005/1/28 18:44
	// ������ �������� ���� ��û ����
	Add ( ON_RESPONSE_CHANGE_WAR, OnResChangeWar );

	// robypark 2005/1/29 14:25
	// ���� �߿� ���޼����� �ڽ��� ����ȸ��(10%) ��û ����
	Add ( ON_RESPONSE_CHARGE_MP, OnResponseChargeMP );

	// �����ϱ� �������̽����� ���� �Ӽ� ���� �ʱ�ȭ ���� ���
	Add (ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK, OnResponseVullageAttackTypeCheck );

	Add( ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK,VillagePlayerOilAttack);			// ������ ���� �ݰ� - �⸧ ���� ����
	Add( ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK,VillagePlayerStoneAttack);		// ������ ���� �ݰ� - �� ���� ����
	Add( ON_RESPONSE_VILLAGE_DEFENCE, OnResponseVillageDefence);				// ������ ���� �ݰ� - ���� ���� ���
	Add( ON_CHANGED_VILLAGE_ATTACK_TYPE, OnBroadcastChangedVillageAttackType );			// ������ ���� �ݰ��� ������.

	Add( ON_RESPONSE_INCREASE_GUILD_UNIT,OnIncreaseGuildUnit);
	Add( ON_RESPONSE_DECREASE_GUILD_UNIT,OnDecreaseGuildUnit);

//	Add( ON_RESPONSE_INCREASE_GUILD_SUPPLY,OnIncreaseGuildUnit);
//	Add( ON_RESPONSE_DECREASE_GUILD_SUPPLY,OnDecreaseGuildUnit);
//	Add( ON_REQUEST_LIST_GUILD_SUPPLY,);

	// robypark 2004/11/9 17:25
	// ������ ���� ������ ���� ����ǰ �ޱ�
	Add( ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT, OnResponseSupplyGoodsFromSupplyWagon );

	// robypark 2004/11/9 18:17
	// ������ ���޼����� ������ ���� ����ǰ �ޱ�
	Add( ON_RESPONSE_DECREASE_GUILD_SUPPLY, OnResponseSupplyGoodsFromVillage );

	// robypark 2004/11/16 15:34
	// ������ ��� ���� ���� ����Ʈ ���
	Add( ON_RESPONSE_LIST_GUILD_UNIT, OnResponseSiegeWarfareUnitList );

	// robypark 2004/11/25 16:4
	// ȭģ���� �޽��� ���� ����
	Add( ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, OnResponseGuildFrieldlyMessageOffer );

	// robypark 2004/11/26 12:47
	// ȭģ���� ���� ����
	Add ( ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT, OnResponseGuildFriendlyMessage_Accept );

	// robypark 2004/11/26 12:47
	// ȭģ���� ���� ����
	Add ( ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL, OnResponseGuildFriendlyMessage_Cancel );

	// robypark 2004/12/2 10:56
	// ������ ��� ������ ���� ����Ʈ �� ���� % ����
	Add ( ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT, OnResponseGuildVillageattackResult );

	// robypark 2004/12/2 13:45
	// ������ ���� �Ŀ� ������ ���� ���� ������ Ư�� ĳ���Ϳ� �ֱ� ��û
	Add ( ON_RESPONSE_GUILD_CHOICE_PLAYER, OnResponseGuildChoicePlayer );

	// robypark 2005/1/5 17:12
	// ������ ������ ���¿��� �Ϲ� �������� ���ư��ڴٰ��� ��û�� ���� ����
	Add ( ON_RESPONSE_NORMAL_CHAR, OnResponseNormalCharacter );
}

// ���� �޼��� �Լ� �ʱ�ȭ
void OnlineMsgProc::Init_Village_Fnc()
{
	// �ǿ�
	Add( ON_RESPONSE_HOSPITAL_CURESOLDER, ResCure );
	Add( ON_RESPONSE_HOSPITAL_CUREALLSOLDER, ResCureAll );
	Add( ON_RESPONSE_HOSPITAL_GETCURELIST, GetCureList );

	// �ױ�
	Add( ON_RESPONSE_WHARF_ENTER, WharfEnter);
	Add( ON_RESPONSE_WHARF_BUYTICKET, WharfBuyTicket );
	Add( ON_RESPONSE_WHARF_EXIT, WharfExit );
	Add( ON_RESPONSE_WHARFWAITROOM_ENTER, WharfWaitroomEnter );
	Add( ON_RESPONSE_WHARFWAITROOM_EXIT, WharfWaitroomExit );
	Add( ON_RESPONSE_WHARF_CHANGETICKETLIST, WharfTicketList );
	Add( ON_BOARDSHIP, BoardShip );
	Add( ON_GETOUTSHIP, GetOutShip );

	// ��û
	Add( ON_RESPONSE_CHANGE_GUILD_FLAG, ResChangeGuildFlag );	
	Add( ON_CHANGED_VILLAGE_DEFENCE, OnResChangedVillageDefence );

	// ����
	Add( ON_RESPONSE_BANK_WITHDRAW, BankWithdraw );	
	Add( ON_RESPONSE_BANK_SAVE, BankSave );
	Add( ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT, BankAccountList );	

	// �Ʒü�
	Add( ON_RESPONSE_BARRACK_GETSOLDERLIST, BarrackBuySolderList );
	Add( ON_RESPONSE_BARRACK_BUYSOLDER, BarrackBuySolder );
	Add( ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, BarrackDismissSolderList );
	Add( ON_RESPONSE_BARRACK_DISMISSALSOLDER, BarrackDismissSolder );

	// �������
	Add( ON_RESPONSE_GBARRACK_GETSOLDERLIST, GBarrackBuySolderList );
	Add( ON_RESPONSE_GBARRACK_BUYSOLDER, GBarrackBuySolder );
	Add( ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, GBarrackDismissSolderList );
	Add( ON_RESPONSE_GBARRACK_DISMISSALSOLDER, GBarrackDismissSolder );
	Add( ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, GBarrackChangjobSolderList );
	Add( ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, GBarrackChangjobSolder );

	// 2�� ���� ��� ����
	Add( ON_RESPONSE_GBARRACK_GET_GENERAL_LIST, GBarrackBuyGeneralExList );
	Add( ON_RESPONSE_GBARRACK_BUY_GENERAL,GBarrackBuyGeneralEx );
	Add( ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, GBarrackDismissGeneralExList );
	Add( ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, GBarrackDismissGeneralEx );
	Add( ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST, GBarrackChangeGeneralExList );
	Add( ON_RESPONSE_GBARRACK_CHANGE_GENERAL, GBarrackChangeGeneralEx );

	// ���Ͱ���
	Add( ON_RESPONSE_MBARRACK_GETMONSTERLIST, MBarrackBuyMonsterList );
	Add( ON_RESPONSE_MBARRACK_BUYMONSTER, MBarrackBuyMonster );
	Add( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, MBarrackDismissMonsterList );
	Add( ON_RESPONSE_MBARRACK_DISMISSMONSTER, MBarrackDismissMonster );

	// �ָ�
	Add( ON_RESPONSE_ITEMLIST, InnItemList );
	Add( ON_RESPONSE_TAKEOUTITEM, InnTakeoutItem );
	Add( ON_RESPONSE_SAVEITEM, InnSaveItem );

	// ����
	Add( ON_RESPONSE_CHANGED_ITEMINFO, MarketChangeIteminfo);
	Add( ON_RESPONSE_ENTER_MARKET, MarketEnter );
	Add( ON_RESPONSE_LIST_ITEM, MarketItemList );
	Add( ON_RESPONSE_SELL_ITEM, MarketSellItem );
	Add( ON_RESPONSE_BUY_ITEM, MarketBuyItem );
	Add( ON_RESPONSE_LEAVE_MARKET, MarketLeave );
//	Add( ON_RESPONSE_MOVEITEM, MarketMoveItem );

	// ��û
	Add( ON_RESPONSE_GOVERNMENT_MAIN_ENTER, GovernEnter);
	Add( ON_RESPONSE_GOVERNMENT_MAIN_LEAVE, GovernLeave );

	// ��û(��������)
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, GovernInvestSet );	// ����
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT, GovernInvestGetout );		// ����
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER, GovernInvestMylist );			// ���� ������ ��������Ʈ
	Add( ON_RESPONSE_PROFIT_STATISTICS, GovernInvestProfitInfo );				// �������ͱ�
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE, GovernInvestLeave );

	// robypark 2004/12/2 14:15
	Add( ON_RESPONSE_VILLAGE_INVESTMENT_POWER, OnResponseVillageInvestmentPower );	// ������ ���� �Ŀ� ���� ��ź ����(���� ����)�� ���������� ���� ����

	// ��û(�����ȳ���)
	Add( ON_RESPONSE_WRITE_DESCRIPT, GovernDescript );							// ���� �ȳ���

	// ��û(��������)
	Add( ON_RESPONSE_GOVERNMENT_VILLAGEINFO, GovernVillageInfo );				// ����������
	Add( ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT, GovernVillageDefenceInfo);	// ���� ���� ����
	Add( ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, GovernVillageDefenceSet );	// ���� ���� ���

	// ������ ���� ���� ���� ��û �޽��� ����
	Add ( ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER, OnResponseIncreaseVillageDefenceSoldier );

	// ������ ��� ����ǰ ���� ���
	Add ( ON_RESPONSE_LIST_GUILD_SUPPLY, OnResponseListGuildSupply );

	// ������ ���� ���� �� ���
	Add ( ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER, OnResponseGetVillageDefenceSoldier );

	// ������ ��� ����ǰ ���� ��û ����
	Add ( ON_RESPONSE_INCREASE_GUILD_SUPPLY, OnResponseIncreaseGuildSupply );

	// ��û(���)
//	Add( ON_GUILD_MSG, GovernGuildMsg );

	Add( ON_RESPONSE_GUILD_LIST, GovernGuildList );					// ��� ��� ����Ʈ
	Add( ON_RESPONSE_GUILD_SET_RELATION, GovernGuildSetRelation );	// ��ܰ� ���� ����
	Add( ON_RESPONSE_GUILD_DETAIL_INFO, GovernGuildDetailInfo );	// ��ܻ�����(��������Ʈ)
	Add( ON_RESPONSE_JOIN_LEAVE_GUILD, GovernGuildJoin );			// ��� ���� ��û �� ���

	Add( ON_RESPONSE_GUILD_CREATE_COST, GovernGuildCreateCost );	// ��� �������
	Add( ON_RESPONSE_CREATE_GUILD, GovernGuildCreate );				// ��� ����
	Add( ON_RESPONSE_DELETE_GUILD, GovernGuildDelete );				// ��� ��ü

	Add( ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, GovernGuildWaitingMemberList);	// ����� ���
	Add( ON_RESPONSE_JOINGUILD_OKNO, GovernGuildWaitingMemberJoin );					// ����� ���� �� ���

	Add( ON_RESPONSE_GUILD_MEMBERS_LIST, GovernGuildMemberList );	// ��ܿ� ����Ʈ
	Add( ON_RESPONSE_GUILD_DISMISSAL, GovernGuildDismiss );			// ��ܿ� �ذ�
	Add( ON_RESPONSE_GUILD_PROMOTION, GovernGuildPromotion );		// ��ܿ� ���� �Ӹ�
	Add( ON_RESPONSE_GUILD_DEMOTION, GovernGuildDemotion );			// ��ܿ� ���� ����

	Add( ON_RESPONSE_USED_FLAG_LIST, GovernGuildFlagList );			// ��� ����Ʈ
	Add( ON_RESPONSE_CHANGE_GUILD_FLAG1, GovernGuildFlagSet );		// ��� ����


	// ����ü� ����Ʈ
	Add( ON_RESPONSE_FARM_INFO, MakeFarmInfo );
	Add( ON_RESPONSE_FARM_BUY, MakeFarmBuy );

	// ����ü�
	Add( ON_FARMWORK_COMPLETED, FarmWorkCompleted );				// ���� �۾��� �Ϸ�Ǿ���
	Add( ON_RESPONSE_FARM_SLOTINFO, FarmSlotInfo );					// ����(�ǹ�) ����
	Add( ON_RESPONSE_FARM_EXPANDSLOT, FarmSlotExpand );				// ���� Ȯ��
	Add( ON_RESPONSE_LEAVE_FACTORY, FarmExit );						// ����ü� ������

	Add( ON_RESPONSE_FARM_BUILDSTRUCT, FarmStructBuild );			// �ǹ� �Ǽ�
	Add( ON_RESPONSE_FARM_DESTROYSTRUCT, FarmStructDestroy );		// �ǹ� ö��
	Add( ON_RESPONSE_FARM_UPGRADESLOT, FarmStructUpgrade );			// �ǹ� ���׷��̵�
	Add( ON_RESPONSE_LEAVE_STRUCTURE, FarmStructExit );				// �ǹ� ������

	Add( ON_RESPONSE_STORAGE_INFO, FarmStorageInfo );				// â�� ����
	Add( ON_RESPONSE_STORAGE_INPUTITEM, FarmStorageItemInput );		// â�� ������ �ֱ�
	Add( ON_RESPONSE_STORAGE_OUTPUTITEM, FarmStorageItemOutput );	// â�� ������ ��������

	Add( ON_RESPONSE_FARM_WORKINFO, FarmProductInfo );				// ������ ���� ����
	Add( ON_RESPONSE_FARM_ITEMPRODUCT, FarmProduct );				// ������ �����ϱ�
	Add( ON_RESPONSE_PLANT_CANCEL_PLAN, FarmProductCancel );		// ������ ���� ���
	Add( ON_RESPONSE_PLANT_CHANGE_PAY, FarmProductPayChange );		// ������ ���� �ӱ� ����

	Add( ON_RESPONSE_FARM_WORKSTART, FarmWorkStart );				// ���ϱ� ����(���Ҽ��ִ��� üũ)
	Add( ON_RESPONSE_FARM_WORK, FarmWorkResult );					// ������ ���ޱ�
}

void OnlineMsgProc::Add( DWORD key, MsgProcessFnc Fnc )
{		
	efficientAddOrUpdata( _Msg, key, Fnc );
}

void OnlineMsgProc::removeAll()
{
	_Msg.clear();
}
