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

// 시스템 메시지 함수 초기화
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
	// 공성전이 종료되었다. 모든 소집권 아이템을 인벤토리에서 삭제하며 또한 원래 주인공 유닛으로 돌려논다.
	Add( ON_CHANGED_WAR_END,	OnResChangedWarEnd );

	// robypark 2004/12/7 12:59
	// 공성유닛이 죽어서 생긴 패널티를 원래대로 돌린다.
	Add( ON_CHANGED_PLAYER_DEAD_PENALTY, OnResChangedPlayerDeadPenalty );

	// robypark 2005/2/15 17:43
	// 공성전이 화친제의에 의해서 사라졌다
	Add( ON_RESPONSE_NOT_WAR, OnResponseNotWar );

	// robypark 2005/2/16 2:52
	// 공성전 시간이 되어서 사용자가 공성이 있다고 알려줌
	Add( ON_RESPONE_WAR_TIME, OnResponseWarTime );
}

// 필드 메세지 함수 초기화
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
//	Add( ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO, OnDokdoQuestCompletedUser );// actdoll (2004/03/30 11:12) : = 이벤트 = 독도 퀘스트 완료자 정보
	Add( ON_RELOAD_WEAR_ITEM, OnBroadCast_ReloadWearItem );	// actdoll (2004/07/19 14:07) : = 시간제 아이템 = 장착한 아이템 정보 갱신
	Add( ON_RESOPNSE_REAL_TIME, OnBroadCast_CurrentServerTime);	// robypark 2004/9/14 19:33 서버 시간을 전달받아 이벤트 퀘스트를 비활성화해 준다.

	Add( ON_RESPONSE_FIELDATTACK_READY,FieldAttack_Ready);
	Add( ON_RESPONSE_FIELDATTACK_ACTION,FieldAttack_Action);
	Add( ON_RESPONSE_FIELDATTACK_DEAD,FieldAttack_Dead);

	
	Add( ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY,LongVillageAttack_Ready);
	Add( ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION,LongVillageAttack_Action);

	// 수성하기 인터페이스로 들어가기 요청에 대한 응답 처리
	// ON_RESPONSE_WAR_VILLAGE_DEFENCE
	Add( ON_RESPONSE_WAR_VILLAGE_DEFENCE, OnResponseEnterVillageDefenceInterface );

	/*
	// 수성하기 인터페이스에서 마을로 들어가기 요청에 대한 응답 처리
	Add ( ON_RESPONSE_WAR_VILLAGE_IN, OnResponseEnterVillageFromVillageDefenceInterface );
	*/

	// 수성하기 인터페이스에 나온다. 필드로
	Add ( ON_RESPONSE_VILLAGE_DEFFNCE_STOP, OnResponseLeaveVillageDefenceInterface );

	// robypark 2005/1/28 18:44
	// 공성전 유닛으로 변신 요청 응답
	Add ( ON_RESPONSE_CHANGE_WAR, OnResChangeWar );

	// robypark 2005/1/29 14:25
	// 공성 중에 보급수레가 자신의 마나회복(10%) 요청 응답
	Add ( ON_RESPONSE_CHARGE_MP, OnResponseChargeMP );

	// 수성하기 인터페이스에서 공격 속성 등의 초기화 정보 얻기
	Add (ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK, OnResponseVullageAttackTypeCheck );

	Add( ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK,VillagePlayerOilAttack);			// 공성전 마을 반격 - 기름 공격 설정
	Add( ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK,VillagePlayerStoneAttack);		// 공성전 마을 반격 - 돌 공격 설정
	Add( ON_RESPONSE_VILLAGE_DEFENCE, OnResponseVillageDefence);				// 공성전 마을 반격 - 마을 방어력 향상
	Add( ON_CHANGED_VILLAGE_ATTACK_TYPE, OnBroadcastChangedVillageAttackType );			// 공성전 마을 반격이 끝났다.

	Add( ON_RESPONSE_INCREASE_GUILD_UNIT,OnIncreaseGuildUnit);
	Add( ON_RESPONSE_DECREASE_GUILD_UNIT,OnDecreaseGuildUnit);

//	Add( ON_RESPONSE_INCREASE_GUILD_SUPPLY,OnIncreaseGuildUnit);
//	Add( ON_RESPONSE_DECREASE_GUILD_SUPPLY,OnDecreaseGuildUnit);
//	Add( ON_REQUEST_LIST_GUILD_SUPPLY,);

	// robypark 2004/11/9 17:25
	// 공성전 보급 수레로 부터 보급품 받기
	Add( ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT, OnResponseSupplyGoodsFromSupplyWagon );

	// robypark 2004/11/9 18:17
	// 공성전 보급수레가 마을로 부터 보급품 받기
	Add( ON_RESPONSE_DECREASE_GUILD_SUPPLY, OnResponseSupplyGoodsFromVillage );

	// robypark 2004/11/16 15:34
	// 공성전 상단 공성 유닛 리스트 얻기
	Add( ON_RESPONSE_LIST_GUILD_UNIT, OnResponseSiegeWarfareUnitList );

	// robypark 2004/11/25 16:4
	// 화친제의 메시지 보낸 응답
	Add( ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, OnResponseGuildFrieldlyMessageOffer );

	// robypark 2004/11/26 12:47
	// 화친제의 수락 응답
	Add ( ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT, OnResponseGuildFriendlyMessage_Accept );

	// robypark 2004/11/26 12:47
	// 화친제의 거절 응답
	Add ( ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL, OnResponseGuildFriendlyMessage_Cancel );

	// robypark 2004/12/2 10:56
	// 공성전 결과 점령한 마을 리스트 및 투자 % 응답
	Add ( ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT, OnResponseGuildVillageattackResult );

	// robypark 2004/12/2 13:45
	// 공성전 종료 후에 마을에 대한 폭투 권한을 특정 캐릭터에 주기 요청
	Add ( ON_RESPONSE_GUILD_CHOICE_PLAYER, OnResponseGuildChoicePlayer );

	// robypark 2005/1/5 17:12
	// 공성전 유닛인 상태에서 일반 유닛으로 돌아가겠다고한 요청에 대한 응답
	Add ( ON_RESPONSE_NORMAL_CHAR, OnResponseNormalCharacter );
}

// 미을 메세지 함수 초기화
void OnlineMsgProc::Init_Village_Fnc()
{
	// 의원
	Add( ON_RESPONSE_HOSPITAL_CURESOLDER, ResCure );
	Add( ON_RESPONSE_HOSPITAL_CUREALLSOLDER, ResCureAll );
	Add( ON_RESPONSE_HOSPITAL_GETCURELIST, GetCureList );

	// 항구
	Add( ON_RESPONSE_WHARF_ENTER, WharfEnter);
	Add( ON_RESPONSE_WHARF_BUYTICKET, WharfBuyTicket );
	Add( ON_RESPONSE_WHARF_EXIT, WharfExit );
	Add( ON_RESPONSE_WHARFWAITROOM_ENTER, WharfWaitroomEnter );
	Add( ON_RESPONSE_WHARFWAITROOM_EXIT, WharfWaitroomExit );
	Add( ON_RESPONSE_WHARF_CHANGETICKETLIST, WharfTicketList );
	Add( ON_BOARDSHIP, BoardShip );
	Add( ON_GETOUTSHIP, GetOutShip );

	// 관청
	Add( ON_RESPONSE_CHANGE_GUILD_FLAG, ResChangeGuildFlag );	
	Add( ON_CHANGED_VILLAGE_DEFENCE, OnResChangedVillageDefence );

	// 전장
	Add( ON_RESPONSE_BANK_WITHDRAW, BankWithdraw );	
	Add( ON_RESPONSE_BANK_SAVE, BankSave );
	Add( ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT, BankAccountList );	

	// 훈련소
	Add( ON_RESPONSE_BARRACK_GETSOLDERLIST, BarrackBuySolderList );
	Add( ON_RESPONSE_BARRACK_BUYSOLDER, BarrackBuySolder );
	Add( ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, BarrackDismissSolderList );
	Add( ON_RESPONSE_BARRACK_DISMISSALSOLDER, BarrackDismissSolder );

	// 장수고용소
	Add( ON_RESPONSE_GBARRACK_GETSOLDERLIST, GBarrackBuySolderList );
	Add( ON_RESPONSE_GBARRACK_BUYSOLDER, GBarrackBuySolder );
	Add( ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, GBarrackDismissSolderList );
	Add( ON_RESPONSE_GBARRACK_DISMISSALSOLDER, GBarrackDismissSolder );
	Add( ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, GBarrackChangjobSolderList );
	Add( ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, GBarrackChangjobSolder );

	// 2차 전직 장수 고용소
	Add( ON_RESPONSE_GBARRACK_GET_GENERAL_LIST, GBarrackBuyGeneralExList );
	Add( ON_RESPONSE_GBARRACK_BUY_GENERAL,GBarrackBuyGeneralEx );
	Add( ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, GBarrackDismissGeneralExList );
	Add( ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, GBarrackDismissGeneralEx );
	Add( ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST, GBarrackChangeGeneralExList );
	Add( ON_RESPONSE_GBARRACK_CHANGE_GENERAL, GBarrackChangeGeneralEx );

	// 몬스터고용소
	Add( ON_RESPONSE_MBARRACK_GETMONSTERLIST, MBarrackBuyMonsterList );
	Add( ON_RESPONSE_MBARRACK_BUYMONSTER, MBarrackBuyMonster );
	Add( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, MBarrackDismissMonsterList );
	Add( ON_RESPONSE_MBARRACK_DISMISSMONSTER, MBarrackDismissMonster );

	// 주막
	Add( ON_RESPONSE_ITEMLIST, InnItemList );
	Add( ON_RESPONSE_TAKEOUTITEM, InnTakeoutItem );
	Add( ON_RESPONSE_SAVEITEM, InnSaveItem );

	// 시전
	Add( ON_RESPONSE_CHANGED_ITEMINFO, MarketChangeIteminfo);
	Add( ON_RESPONSE_ENTER_MARKET, MarketEnter );
	Add( ON_RESPONSE_LIST_ITEM, MarketItemList );
	Add( ON_RESPONSE_SELL_ITEM, MarketSellItem );
	Add( ON_RESPONSE_BUY_ITEM, MarketBuyItem );
	Add( ON_RESPONSE_LEAVE_MARKET, MarketLeave );
//	Add( ON_RESPONSE_MOVEITEM, MarketMoveItem );

	// 관청
	Add( ON_RESPONSE_GOVERNMENT_MAIN_ENTER, GovernEnter);
	Add( ON_RESPONSE_GOVERNMENT_MAIN_LEAVE, GovernLeave );

	// 관청(마을투자)
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT, GovernInvestSet );	// 투자
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT, GovernInvestGetout );		// 인출
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER, GovernInvestMylist );			// 내가 투자한 마을리스트
	Add( ON_RESPONSE_PROFIT_STATISTICS, GovernInvestProfitInfo );				// 마을수익금
	Add( ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE, GovernInvestLeave );

	// robypark 2004/12/2 14:15
	Add( ON_RESPONSE_VILLAGE_INVESTMENT_POWER, OnResponseVillageInvestmentPower );	// 공성전 종료 후에 마을 폭탄 투가(권한 여부)가 가능한지에 대한 응답

	// 관청(마을안내문)
	Add( ON_RESPONSE_WRITE_DESCRIPT, GovernDescript );							// 마을 안내문

	// 관청(마을정보)
	Add( ON_RESPONSE_GOVERNMENT_VILLAGEINFO, GovernVillageInfo );				// 마을상세정보
	Add( ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT, GovernVillageDefenceInfo);	// 방어력 투자 정보
	Add( ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT, GovernVillageDefenceSet );	// 방어력 투자 향상

	// 공성전 마을 수비병 증가 요청 메시지 응답
	Add ( ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER, OnResponseIncreaseVillageDefenceSoldier );

	// 공성전 상단 보급품 개수 얻기
	Add ( ON_RESPONSE_LIST_GUILD_SUPPLY, OnResponseListGuildSupply );

	// 공성전 마을 수비병 수 얻기
	Add ( ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER, OnResponseGetVillageDefenceSoldier );

	// 공성전 상단 보급품 생산 요청 응답
	Add ( ON_RESPONSE_INCREASE_GUILD_SUPPLY, OnResponseIncreaseGuildSupply );

	// 관청(상단)
//	Add( ON_GUILD_MSG, GovernGuildMsg );

	Add( ON_RESPONSE_GUILD_LIST, GovernGuildList );					// 모든 상단 리스트
	Add( ON_RESPONSE_GUILD_SET_RELATION, GovernGuildSetRelation );	// 상단간 적대 설정
	Add( ON_RESPONSE_GUILD_DETAIL_INFO, GovernGuildDetailInfo );	// 상단상세정보(마을리스트)
	Add( ON_RESPONSE_JOIN_LEAVE_GUILD, GovernGuildJoin );			// 상단 가입 신청 및 취소

	Add( ON_RESPONSE_GUILD_CREATE_COST, GovernGuildCreateCost );	// 상단 개설비용
	Add( ON_RESPONSE_CREATE_GUILD, GovernGuildCreate );				// 상단 개설
	Add( ON_RESPONSE_DELETE_GUILD, GovernGuildDelete );				// 상단 해체

	Add( ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST, GovernGuildWaitingMemberList);	// 대기자 명단
	Add( ON_RESPONSE_JOINGUILD_OKNO, GovernGuildWaitingMemberJoin );					// 대기자 가입 및 취소

	Add( ON_RESPONSE_GUILD_MEMBERS_LIST, GovernGuildMemberList );	// 상단원 리스트
	Add( ON_RESPONSE_GUILD_DISMISSAL, GovernGuildDismiss );			// 상단원 해고
	Add( ON_RESPONSE_GUILD_PROMOTION, GovernGuildPromotion );		// 상단원 직급 임명
	Add( ON_RESPONSE_GUILD_DEMOTION, GovernGuildDemotion );			// 상단원 직급 해임

	Add( ON_RESPONSE_USED_FLAG_LIST, GovernGuildFlagList );			// 깃발 리스트
	Add( ON_RESPONSE_CHANGE_GUILD_FLAG1, GovernGuildFlagSet );		// 깃발 선택


	// 생산시설 리스트
	Add( ON_RESPONSE_FARM_INFO, MakeFarmInfo );
	Add( ON_RESPONSE_FARM_BUY, MakeFarmBuy );

	// 생산시설
	Add( ON_FARMWORK_COMPLETED, FarmWorkCompleted );				// 현재 작업이 완료되었다
	Add( ON_RESPONSE_FARM_SLOTINFO, FarmSlotInfo );					// 슬롯(건물) 정보
	Add( ON_RESPONSE_FARM_EXPANDSLOT, FarmSlotExpand );				// 슬롯 확장
	Add( ON_RESPONSE_LEAVE_FACTORY, FarmExit );						// 생산시설 나가기

	Add( ON_RESPONSE_FARM_BUILDSTRUCT, FarmStructBuild );			// 건물 건설
	Add( ON_RESPONSE_FARM_DESTROYSTRUCT, FarmStructDestroy );		// 건물 철거
	Add( ON_RESPONSE_FARM_UPGRADESLOT, FarmStructUpgrade );			// 건물 업그레이드
	Add( ON_RESPONSE_LEAVE_STRUCTURE, FarmStructExit );				// 건물 나가기

	Add( ON_RESPONSE_STORAGE_INFO, FarmStorageInfo );				// 창고 정보
	Add( ON_RESPONSE_STORAGE_INPUTITEM, FarmStorageItemInput );		// 창고 아이템 넣기
	Add( ON_RESPONSE_STORAGE_OUTPUTITEM, FarmStorageItemOutput );	// 창고 아이템 가져오기

	Add( ON_RESPONSE_FARM_WORKINFO, FarmProductInfo );				// 아이템 생산 정보
	Add( ON_RESPONSE_FARM_ITEMPRODUCT, FarmProduct );				// 아이템 생산하기
	Add( ON_RESPONSE_PLANT_CANCEL_PLAN, FarmProductCancel );		// 아이템 생산 취소
	Add( ON_RESPONSE_PLANT_CHANGE_PAY, FarmProductPayChange );		// 아이템 생산 임금 변경

	Add( ON_RESPONSE_FARM_WORKSTART, FarmWorkStart );				// 일하기 시작(일할수있는지 체크)
	Add( ON_RESPONSE_FARM_WORK, FarmWorkResult );					// 일한후 돈받기
}

void OnlineMsgProc::Add( DWORD key, MsgProcessFnc Fnc )
{		
	efficientAddOrUpdata( _Msg, key, Fnc );
}

void OnlineMsgProc::removeAll()
{
	_Msg.clear();
}
