#pragma once

#pragma warning ( disable : 4786 )

#include <iostream>
#include <map>
#include <string>

using namespace std;


class cltOnlineWorld;
class OnlineMsgProc
{
public:
	typedef UI16	( OnlineMsgProc::*MsgProcessFnc )(BYTE *pMsg);
	typedef map< DWORD, MsgProcessFnc, less< DWORD > >	MsgContainer;	// 함수 포인터를 저장할 컨테이너
	

public:
	OnlineMsgProc( cltOnlineWorld *pWorld );
	~OnlineMsgProc()							{ removeAll(); }


	MsgProcessFnc	GetFnc( DWORD dw )									// 함수를 찾는다.
	{
		pair< MsgContainer::iterator, MsgContainer::iterator > Find = _Msg.equal_range( dw );

		if(Find.first->first == dw)
		{
			return Find.first->second;
		}

		return NULL;
	}
	

private:	
	void			Init_System_Fnc();
	void			Init_Field_Fnc();
	void			Init_Village_Fnc();	


	void			Add( DWORD key, MsgProcessFnc Fnc );				// 함수를 추가
	void			AddLogIn( DWORD key, MsgProcessFnc Fnc );
	void			removeAll();


private:
	cltOnlineWorld	*_pWorld;	
	MsgContainer	_Msg;


	// 시스템
private:
	UI16	OnCheckGameGuard(BYTE *pMsg);
	UI16	OnResCheckGameGuard(BYTE *pMsg);
	UI16	OnResLogOut(BYTE *pMsg);									// 로그아웃
	UI16	ChangedTime(BYTE *pMsg);									// 시간 변경
	UI16	OnTargetMe(BYTE *pMsg);										// 몬스터가 날 공격 목표로 잡음
	UI16	OnReleaseMe(BYTE *pMsg);									// 몬스터 공격 목표를 잡은것을 취소
	UI16	OnChangeMoney(BYTE *pMsg);									// 돈이 바낌	
	UI16	OnOfficaialAnnouncement(BYTE *pMsg);						// 공지 사항
	UI16	OnTextMessageg(BYTE *pMsg);									//
	UI16	OnResWarp(BYTE *pMsg);										// 워프
	UI16	OnResBonusUp(BYTE *pMsg);									// 보너스 업
	UI16	OnHungry(BYTE *pMsg);										// 배고픔	
	UI16	ChangeWeather(BYTE *pMsg);
	UI16	OnResItemReceived(BYTE *pMsg);								// 구입한 아이템 개수를 반환 
	UI16	RunProgramList(BYTE *pMsg);									// 실행 중인 프로그램	
	UI16	MakeThunder(BYTE *pMsg);									// 번개	
	UI16	SelectWorld(BYTE *pMsg);
	UI16	NewChar(BYTE *pMsg);	
	UI16	CheckVersion(BYTE *pMsg);	
	UI16	Notice(BYTE *pMsg);
	UI16	WorldInfo(BYTE *pMsg);
	UI16	RecvPWData(BYTE *pMsg);
	UI16	ResLogin(BYTE *pMsg);
	UI16	CharsInfo(BYTE *pMsg);	
	UI16	ReturnOnline(BYTE *pMsg);
	UI16	DelChar(BYTE *pMsg);
	UI16	LeaveVillage(BYTE *pMsg);
	UI16	InvestmentList(BYTE *pMsg);
	UI16	SetAttackMode(BYTE *pMsg);									// 전투 모드
	UI16	ResRank(BYTE *pMsg);

	// robypark 2004/12/6 17:11
	// 공성전이 종료되었다. 모든 소집권 아이템을 인벤토리에서 삭제하며 또한 원래 주인공 유닛으로 돌려논다.
	// ON_CHANGED_WAR_END
	UI16	OnResChangedWarEnd(BYTE *pMsg);

	// robypark 2004/12/7 12:59
	// 공성유닛이 죽어서 생긴 패널티를 원래대로 돌린다.
	// ON_CHANGED_PLAYER_DEAD_PENALTY
	UI16 OnResChangedPlayerDeadPenalty(BYTE *pMsg);

	// 마을 안에서 처리할 함수들
private:	
	UI16	ResCure(BYTE *pMsg);										// 의원에서 한명만 치료
	UI16	ResCureAll(BYTE *pMsg);										// 의원에서 전원 치료
	UI16	GetCureList(BYTE *pMsg);									// 의원에서 치료리스트

	UI16	BoardShip(BYTE *pMsg);										// 배가 출발
	UI16	GetOutShip(BYTE *pMsg);										// 배에서 내린다.	
	UI16	ResChangeGuildFlag(BYTE *pMsg);								// 상단 깃발을 변경	
	UI16	OnResChangedVillageDefence(BYTE *pMsg);						// 마을 방어력 변경

	UI16	BankWithdraw(BYTE *pMsg);									// 전장출금
	UI16	BankSave(BYTE *pMsg);										// 전장입금
	UI16	BankAccountList(BYTE *pMsg);								// 통장수	

	UI16	BarrackBuySolderList(BYTE *pMsg);							// 훈련소 고용리스트
	UI16	BarrackBuySolder(BYTE *pMsg);								// 훈련소 고용응답
	UI16	BarrackDismissSolderList(BYTE *pMsg);						// 훈련소 해고리스트
	UI16	BarrackDismissSolder(BYTE *pMsg);							// 훈련소 해고응답

	UI16	GBarrackBuySolderList(BYTE *pMsg);							// 장수양성소 고용리스트
	UI16	GBarrackBuySolder(BYTE *pMsg);								// 장수양성소 고용응답
	UI16	GBarrackDismissSolderList(BYTE *pMsg);						// 장수양성소 해고리스트
	UI16	GBarrackDismissSolder(BYTE *pMsg);							// 장수양성소 해고응답
	UI16	GBarrackChangjobSolderList(BYTE *pMsg);						// 장수양성소 전직리스트
	UI16	GBarrackChangjobSolder(BYTE *pMsg);							// 장수양성소 전직응답

	UI16	GBarrackBuyGeneralExList(BYTE *pMsg);						// 2차 전직 장수양성소 고용리스트
	UI16	GBarrackBuyGeneralEx(BYTE *pMsg);							// 2차 전직 장수양성소 고용응답
	UI16	GBarrackDismissGeneralExList(BYTE *pMsg);					// 2차 전직 장수양성소 해고리스트
	UI16	GBarrackDismissGeneralEx(BYTE *pMsg);						// 2차 전직 장수양성소 해고응답
	UI16	GBarrackChangeGeneralExList(BYTE *pMsg);					// 2차 전직 장수양성소 전직 리스트
	UI16	GBarrackChangeGeneralEx(BYTE *pMsg);						// 2차 전직 장수양성소 전직 응답

	UI16	MBarrackBuyMonsterList(BYTE *pMsg);							// 몬스터고용소 고용리스트
	UI16	MBarrackBuyMonster(BYTE *pMsg);								// 몬스터고용소 고용
	UI16	MBarrackDismissMonsterList(BYTE *pMsg);						// 몬스터고용소 해고리스트
	UI16	MBarrackDismissMonster(BYTE *pMsg);							// 몬스터고용소 해고

	UI16	InnItemList(BYTE *pMsg);									// 장수양성소 해고응답
	UI16	InnTakeoutItem(BYTE *pMsg);									// 장수양성소 전직리스트
	UI16	InnSaveItem(BYTE *pMsg);									// 장수양성소 전직응답

	UI16	WharfEnter(BYTE *pMsg);										// 항구 들어감
	UI16	WharfBuyTicket(BYTE *pMsg);									// 항구 티켓구입 응답
	UI16	WharfExit(BYTE *pMsg);										// 항구 나가기
	UI16	WharfWaitroomEnter(BYTE *pMsg);								// 항구 대기실 들어감
	UI16	WharfWaitroomExit(BYTE *pMsg);								// 항구 대기실 나가기
	UI16	WharfTicketList(BYTE *pMsg);								// 항구 티켓 리스트	갱신

	UI16	MarketChangeIteminfo(BYTE *pMsg);							// 시전 아이템 변경
	UI16	MarketEnter(BYTE *pMsg);									// 시전 들어감
	UI16	MarketItemList(BYTE *pMsg);									// 시전 아이템 리스트
	UI16	MarketSellItem(BYTE *pMsg);									// 시전 아이템 팔기
	UI16	MarketBuyItem(BYTE *pMsg);									// 시전 아이템 사기
	UI16	MarketLeave(BYTE *pMsg);									// 시전 나가기
	UI16	MarketMoveItem(BYTE *pMsg);									// 시전 아이템 이동

	UI16	MakeFarmInfo(BYTE *pMsg);									// 생산시설 리스트
	UI16	MakeFarmBuy(BYTE *pMsg);									// 생산시설 구입

	UI16	GovernEnter(BYTE *pMsg);									// 관청 들어감
	UI16	GovernLeave(BYTE *pMsg);									// 관청 나가기
	UI16	GovernInvestSet(BYTE *pMsg);								// 관청(마을투자) 투자
	UI16	GovernInvestGetout(BYTE *pMsg);								// 관청(마을투자) 인출
	UI16	GovernInvestMylist(BYTE *pMsg);								// 관청(마을투자) 내가 투자한 마을리스트
	UI16	GovernInvestLeave(BYTE *pMsg);
	UI16	GovernInvestProfitInfo(BYTE *pMsg);							// 관청(마을투자) 마을 수익금
	UI16	GovernDescript(BYTE *pMsg);									// 관청(마을안내문) 마을 안내문
	UI16	GovernVillageInfo(BYTE *pMsg);								// 관청(마을정보) 마을 상세정보
	UI16	GovernVillageDefenceInfo(BYTE *pMsg);						// 관청(마을정보) 방어력 투자정보
	UI16	GovernVillageDefenceSet(BYTE *pMsg);						// 관청(마을정보) 방어력 투자향상

	UI16	GovernGuildMsg(BYTE *pMsg);									// 관청(상단) 
	UI16	GovernGuildList(BYTE *pMsg);								// 관청(상단) 모든 상단 리스트
	UI16	GovernGuildSetRelation(BYTE *pMsg);							// 관청(상단) 상단간 적대 설정
	UI16	GovernGuildDetailInfo(BYTE *pMsg);							// 관청(상단) 상단상세정보(마을리스트)
	UI16	GovernGuildJoin(BYTE *pMsg);								// 관청(상단) 상단 가입 신청 및 취소
	UI16	GovernGuildCreateCost(BYTE *pMsg);							// 관청(상단) 상단 개설비용
	UI16	GovernGuildCreate(BYTE *pMsg);								// 관청(상단) 상단 개설
	UI16	GovernGuildDelete(BYTE *pMsg);								// 관청(상단) 상단 해체
	UI16	GovernGuildWaitingMemberList(BYTE *pMsg);					// 관청(상단) 대기자 명단
	UI16	GovernGuildWaitingMemberJoin(BYTE *pMsg);					// 관청(상단) 대기자 가입 및 취소
	UI16	GovernGuildMemberList(BYTE *pMsg);							// 관청(상단) 상단원 리스트
	UI16	GovernGuildDismiss(BYTE *pMsg);								// 관청(상단) 상단원 해고
	UI16	GovernGuildPromotion(BYTE *pMsg);							// 관청(상단) 상단원 직급 임명
	UI16	GovernGuildDemotion(BYTE *pMsg);							// 관청(상단) 상단원 직급 해임
	UI16	GovernGuildFlagList(BYTE *pMsg);							// 관청(상단) 깃발 리스트
	UI16	GovernGuildFlagSet(BYTE *pMsg);								// 관청(상단) 깃발 선택

	UI16	FarmWorkCompleted(BYTE *pMsg);								// 생산시설 현재 작업이 완료되었다
	UI16	FarmSlotInfo(BYTE *pMsg);									// 생산시설 슬롯(건물) 정보
	UI16	FarmSlotExpand(BYTE *pMsg);									// 생산시설 슬롯 확장
	UI16	FarmExit(BYTE *pMsg);										// 생산시설 나가기
	UI16	FarmStructBuild(BYTE *pMsg);								// 생산시설 건물 건설
	UI16	FarmStructDestroy(BYTE *pMsg);								// 생산시설 건물 철거
	UI16	FarmStructUpgrade(BYTE *pMsg);								// 생산시설 건물 업그레이드
	UI16	FarmStructExit(BYTE *pMsg);									// 생산시설 건물 나가기
	UI16	FarmStorageInfo(BYTE *pMsg);								// 생산시설 창고 정보
	UI16	FarmStorageItemInput(BYTE *pMsg);							// 생산시설 창고 아이템 넣기
	UI16	FarmStorageItemOutput(BYTE *pMsg);							// 생산시설 창고 아이템 가져오기
	UI16	FarmProductInfo(BYTE *pMsg);								// 생산시설 아이템 생산 정보
	UI16	FarmProduct(BYTE *pMsg);									// 생산시설 아이템 생산하기
	UI16	FarmProductCancel(BYTE *pMsg);								// 생산시설 아이템 생산 취소
	UI16	FarmProductPayChange(BYTE *pMsg);							// 생산시설 아이템 생산 임금 변경
	UI16	FarmWorkStart(BYTE *pMsg);									// 생산시설 일하기 시작(일할수있는지 체크)
	UI16	FarmWorkResult(BYTE *pMsg);									// 생산시설 일한후 돈받기

	
	// 필드
private:
	UI16	NewInfoOnMove(BYTE *pMsg);									// 이동중에 추가나 삭제할 캐릭터 정보.
	UI16	OnDelChar(BYTE *pMsg);										// 캐릭터 제거
	UI16	OnAddChar(BYTE *pMsg);										// 캐릭터 추가
	UI16	OnResPosotion(BYTE *pMsg);									// 다른 캐릭터의 위치를 받는다.	
	UI16	OnResEnterVillage(BYTE *pMsg);								// 마을로 들어 간다.
	UI16	OnResMoveMap(BYTE *pMsg);									// 포탈로 이동	
	UI16	OnResPlayerInfo(BYTE *pMsg);								// 플레이어 정보를 받는다.	
	UI16	OnResBattle(BYTE *pMsg);									// 전투 시작	
	UI16	OnResBattlePlay(BYTE *pMsg);								//	
	UI16	OnChangeChlothes(BYTE *pMsg);								// 의복을 바꺼 입는다.
	UI16	OnResReadDescrit(BYTE *pMsg);								// 마을 안내 문을 읽는다.	
	UI16	OnResVillageNation(BYTE *pMsg);								// 마을의 국적을 얻어 온다.	
	UI16	OnResImoticon(BYTE *pMsg);									// 이모티콘	

	UI16	OnResParty(BYTE *pMsg);										// 파티요청
	UI16	OnResAksParty(BYTE *pMsg);									// 상대방에게 파티를 요청한것을 알려 준다.
	UI16	OnResAcceptParty(BYTE *pMsg);								// 파티 요청에 대한 응답.허락,취소.
	UI16	OnResQuitParty(BYTE *pMsg);									// 파티를 떠났다.
	UI16	OnResPartyCancel(BYTE *pMsg);
	UI16	OnResLeaveParty(BYTE *pMsg);								// 파티에서 나간다.
	UI16	OnResPartyDeleted(BYTE *pMsg);								// 파티 해체.
	UI16	OnResCancelRequest(BYTE *pMsg);								// 파티 취소에 대한 응답 메세지.
//	UI16    OnResPartyLeader(BYTE *pMsg);								// 파티장이 된다.
//	UI16	OnResPartyMemberChange(BYTE *pMsg);							// 파티를 나갔을때 주변사람들에게 보내는 메세지.


	UI16	OnResAcceptTrade(BYTE *pMsg);								// 거래 허락
	UI16	OnResTrade(BYTE *pMsg);										// 거래 요청
	UI16	OnResAksTrade(BYTE *pMsg);									// 상대방에게 거래 요청한것을 알려 준다.	
	UI16	OnResCancelTrade(BYTE *pMsg);								// 거래 취소
	UI16	OnResFinishTrade(BYTE *pMsg);								// 거래 버튼 누름	
	UI16	OnResDecideTrade(BYTE *pMsg);								// 거래 하는것을 수락	
	UI16	OnCompleteTrade(BYTE *pMsg);								// 거래 성공	
	UI16	OnChangeTrade(BYTE *pMsg);									// 거래 물품 변경	
	UI16	OnResAttackVillage(BYTE *pMsg);								// 마을 공격	
	UI16	OnResCharInfoMinimap(BYTE *pMsg);							// 미니맵에서 캐릭터 정보를 얻어 온다.	
	UI16	OnGuildMessage(BYTE *pMsg);									// 상단 메세지	
	UI16	OnResQuestSelectNpc(BYTE *pMsg);							// 퀘스트 메세지	
	UI16	OnResQuestAccept(BYTE *pMsg);								// 퀘스트 메세지 수락 응답	
	UI16	OnResQuestCamcel(BYTE *pMsg);								// 퀘스트 취소	
	UI16	OnResQuestGetPlay(BYTE *pMsg);								// 진행중인 퀘스트
	UI16	OnResQuestGetPlayData(BYTE *pMsg);							// 진행중인 퀘스트 데이타
	UI16	OnResQuestClear(BYTE *pMsg);								// 퀘스트 초기화
	UI16	OnChangeBoothItem(BYTE *pMsg);								// 좌판에서 아이템이 변경되었다.	
	UI16	OnResChangeBoothItem(BYTE *pMsg);							// 좌판에서 아이템 변경함.	
	UI16	OnResCharDetailInfo(BYTE *pMsg);							// 캐릭터의 상세 정보	
	UI16	OnChangeFieldItem(BYTE *pMsg);								// 맵에 있는 아이템이 변경
	UI16	OnResProbeArea(BYTE *pMsg);									// 	
	UI16	NotifyResult(BYTE *pMsg);									// 경매인에게 결과 알림	
	UI16	NotifyAdvertise(BYTE *pMsg);								// 경매 광고	
	UI16	OnResPersonal(BYTE *pMsg);									// 	
	UI16	OnVmercenaryChange(BYTE *pMsg);								// 따라 다니는 용병 교체	
	UI16	OnVmercenaryChangeMe(BYTE *pMsg);							// 따라 다니는 용병 교체 본인	
	UI16	OnChangeCharCode(BYTE *pMsg);								// 주변의 사람들의 코드가 변경	
	UI16	OnImperiumUse(BYTE *pMsg);
	UI16	AuditOwn(BYTE *pMsg);										// 장부 부동산보유 정보(응답)	
	UI16	GuildInfo(BYTE *pMsg);
	UI16	MyVillageAttacked(BYTE *pMsg);								// 나의 마을이 공격 당하고 있다
	UI16	FriendList(BYTE *pMsg);										// 친구 리스트
	UI16	FriendLogOut(BYTE *pMsg);									// 친구 리스트에 있는 친구가 로그 아웃	
	UI16	VillageInfo(BYTE *pMsg);									// 마을 정보	
	UI16	SellArticle(BYTE *pMsg);									// 경매(용병,아이템) 요청에 응답 한다	
	UI16	Bidding(BYTE *pMsg);										// 입찰에 대한 요청	
	UI16	ReportReMainTime(BYTE *pMsg);								// 	
	UI16	ReportBidding(BYTE *pMsg);									// 입찰 성공		
	UI16	ReportSuccessful(BYTE *pMsg);								// 착찰
	UI16	ReportFail(BYTE *pMsg);										// 아무것두 안삼	
	UI16    ReportNew(BYTE *pMsg);										// 경매가 새로 열림
	UI16	Enter(BYTE *pMsg);	
	UI16	OnResOpen(BYTE *pMsg);										// 좌판 오픈
	UI16	OnResBuyItem(BYTE *pMsg);									// 좌판에서 아이템 구입
	UI16	OnResClose(BYTE *pMsg);										// 좌판 닫어
	UI16	OnResBuyFollower(BYTE *pMsg);								// 좌판 용병 구입
	UI16	OnResChangeItem(BYTE *pMsg);								// 좌판 아이템 변경
	UI16	OnResSetName(BYTE *pMsg);									// 용병 이름 변경	
	UI16	WearItem(BYTE *pMsg);	
	UI16	StripItem(BYTE *pMsg);
	UI16	MoveItem(BYTE *pMsg);
	UI16	ThrowItem(BYTE *pMsg);
	UI16	PickupItem(BYTE *pMsg);
	UI16	SpendItem(BYTE *pMsg);
	UI16	AddFollower(BYTE *pMsg);
	UI16	RefineItem(BYTE *pMsg);
	UI16	ChangeTradeGoods(BYTE *pMsg);
	UI16	AuditTotal(BYTE *pMsg);	
	UI16	FriendAddOne(BYTE *pMsg);
	UI16	FriendRemove(BYTE *pMsg);
	UI16	FriendRemoveAll(BYTE *pMsg);
	UI16	VillageList(BYTE *pMsg);
//	UI16	StatiStics(BYTE *pMsg);	
//	UI16	OnDokdoQuestCompletedUser( BYTE *pMsg );					// actdoll (2004/03/30 11:12) : = 이벤트 = 독도 퀘스트 완료자 정보
	UI16	OnBroadCast_ReloadWearItem(BYTE *pMsg);						// actdoll (2004/07/19 14:07) : = 시간제 아이템 = 장착한 아이템 정보 갱신
	UI16	OnBroadCast_CurrentServerTime(BYTE *pMsg);					// robypark 2004/9/14 19:31 서버 시간을 전달받아 이벤트 퀘스트를 비활성화해 준다.

	UI16	FieldAttack_Ready(BYTE* pMsg);
	UI16	FieldAttack_Action(BYTE* pMsg);
	UI16	FieldAttack_Dead(BYTE* pMsg);

	UI16	LongVillageAttack_Ready(BYTE* pMsg);
	UI16	LongVillageAttack_Action(BYTE* pMsg);

	// 수성하기 인터페이스로 들어가기 요청에 대한 응답 처리
	// ON_RESPONSE_WAR_VILLAGE_DEFENCE
	UI16	OnResponseEnterVillageDefenceInterface(BYTE *pMsg);

	/*
	// 수성하기 인터페이스에서 마을로 들어가기 요청에 대한 응답 처리
	// ON_RESPONSE_WAR_VILLAGE_IN
	UI16	OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg);
	*/
	// 수성하기 인터페이스에 나온다. 필드로
	// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
	UI16	OnResponseLeaveVillageDefenceInterface(BYTE *pMsg);

	// robypark 2005/1/28 18:44
	// 공성전 유닛으로 변신 요청 응답
	// ON_RESPONSE_CHANGE_WAR
	UI16	OnResChangeWar(BYTE *pMsg);

	// robypark 2005/1/29 14:25
	// 공성 중에 보급수레가 자신의 마나회복(10%) 요청 응답
	// ON_RESPONSE_CHARGE_MP
	UI16	OnResponseChargeMP(BYTE *pMsg);

	// 수성하기 인터페이스에서 공격 속성 등의 초기화 정보 얻기
	// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK
	UI16	OnResponseVullageAttackTypeCheck(BYTE *pMsg);

	UI16	VillagePlayerOilAttack(BYTE* pMsg);					// 공성전 마을 반격 - 기름 공격 설정
	UI16	VillagePlayerStoneAttack(BYTE* pMsg);				// 공성전 마을 반격 - 돌 공격 설정
	UI16	OnResponseVillageDefence(BYTE *pMsg);				// 공성전 마을 반격 - 마을 방어력 향상
	// 공성전 마을 반격이 끝났다.
	// ON_CHANGED_VILLAGE_ATTACK_TYPE
	UI16	OnBroadcastChangedVillageAttackType(BYTE *pMsg );

	UI16	OnIncreaseGuildUnit(BYTE* pMsg);
	UI16	OnDecreaseGuildUnit(BYTE* pMsg);

	// robypark 2004/11/9 17:27
	// 공성전 보급 수레로 부터 보급품 받기 메시지 처리
	// ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT
	UI16 OnResponseSupplyGoodsFromSupplyWagon( BYTE *pMsg );

	// robypark 2004/11/9 18:46
	// 공성전 보급 수레가 마을에서 보급품 받기
	// ON_RESPONSE_DECREASE_GUILD_SUPPLY
	UI16 OnResponseSupplyGoodsFromVillage( BYTE *pMsg );

	// robypark 2004/11/16 15:36
	// 공성전 상단 공성 유닛 리스트 요청 응답
	// ON_RESPONSE_LIST_GUILD_UNIT
	UI16 OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	// 공성전 마을 수비병 증가 요청 메시지 응답
	UI16 OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg);

	// 공성전 상단 보급품 개수 얻기
	// ON_RESPONSE_LIST_GUILD_SUPPLY
	UI16 OnResponseListGuildSupply(BYTE *pMsg);

	// 공성전 상단 보급품 생산 요청 응답
	// ON_RESPONSE_INCREASE_GUILD_SUPPLY
	UI16 OnResponseIncreaseGuildSupply(BYTE *pMsg);

	// 공성전 마을 수비병 수 얻기
	// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
	UI16 OnResponseGetVillageDefenceSoldier(BYTE *pMsg);

	// robypark 2004/11/25 16:4
	// 화친제의 메시지 보낸 응답
	// ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER
	UI16 OnResponseGuildFrieldlyMessageOffer(BYTE *pMsg);

	// robypark 2004/11/26 12:47
	// 화친제의 수락 응답
	// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT
	UI16 OnResponseGuildFriendlyMessage_Accept(BYTE *pMsg);

	// robypark 2004/11/26 12:47
	// 화친제의 거절 응답
	// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL
	UI16 OnResponseGuildFriendlyMessage_Cancel(BYTE *pMsg);

	// robypark 2004/12/2 10:56
	// 공성전 결과 점령한 마을 리스트 및 투자 % 응답
	// ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT
	UI16 OnResponseGuildVillageattackResult(BYTE *pMsg);

	// robypark 2004/12/2 13:45
	// 공성전 종료 후에 마을에 대한 폭투 권한을 특정 캐릭터에 주기 요청
	// ON_RESPONSE_GUILD_CHOICE_PLAYER
	UI16 OnResponseGuildChoicePlayer(BYTE *pMsg);

	// robypark 2004/12/2 14:15
	// 공성전 종료 후에 마을 폭탄 투가(권한 여부)가 가능한지에 대한 응답
	// ON_RESPONSE_VILLAGE_INVESTMENT_POWER
	UI16 OnResponseVillageInvestmentPower(BYTE *pMsg);

	// robypark 2005/1/5 17:12
	// 공성전 유닛인 상태에서 일반 유닛으로 돌아가겠다고한 요청에 대한 응답
	// ON_RESPONSE_NORMAL_CHAR
	UI16 OnResponseNormalCharacter(BYTE *pMsg);

	// robypark 2005/2/15 17:43
	// 공성전이 화친제의에 의해서 사라졌다
	// ON_RESPONSE_NOT_WAR
	UI16 OnResponseNotWar(BYTE *pMsg);

	// robypark 2005/2/16 2:52
	// 공성전 시간이 되어서 사용자가 공성이 있다고 알려줌
	// ON_RESPONE_WAR_TIME
	UI16 OnResponseWarTime(BYTE *pMsg);
};