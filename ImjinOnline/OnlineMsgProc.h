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
	typedef map< DWORD, MsgProcessFnc, less< DWORD > >	MsgContainer;	// �Լ� �����͸� ������ �����̳�
	

public:
	OnlineMsgProc( cltOnlineWorld *pWorld );
	~OnlineMsgProc()							{ removeAll(); }


	MsgProcessFnc	GetFnc( DWORD dw )									// �Լ��� ã�´�.
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


	void			Add( DWORD key, MsgProcessFnc Fnc );				// �Լ��� �߰�
	void			AddLogIn( DWORD key, MsgProcessFnc Fnc );
	void			removeAll();


private:
	cltOnlineWorld	*_pWorld;	
	MsgContainer	_Msg;


	// �ý���
private:
	UI16	OnCheckGameGuard(BYTE *pMsg);
	UI16	OnResCheckGameGuard(BYTE *pMsg);
	UI16	OnResLogOut(BYTE *pMsg);									// �α׾ƿ�
	UI16	ChangedTime(BYTE *pMsg);									// �ð� ����
	UI16	OnTargetMe(BYTE *pMsg);										// ���Ͱ� �� ���� ��ǥ�� ����
	UI16	OnReleaseMe(BYTE *pMsg);									// ���� ���� ��ǥ�� �������� ���
	UI16	OnChangeMoney(BYTE *pMsg);									// ���� �ٳ�	
	UI16	OnOfficaialAnnouncement(BYTE *pMsg);						// ���� ����
	UI16	OnTextMessageg(BYTE *pMsg);									//
	UI16	OnResWarp(BYTE *pMsg);										// ����
	UI16	OnResBonusUp(BYTE *pMsg);									// ���ʽ� ��
	UI16	OnHungry(BYTE *pMsg);										// �����	
	UI16	ChangeWeather(BYTE *pMsg);
	UI16	OnResItemReceived(BYTE *pMsg);								// ������ ������ ������ ��ȯ 
	UI16	RunProgramList(BYTE *pMsg);									// ���� ���� ���α׷�	
	UI16	MakeThunder(BYTE *pMsg);									// ����	
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
	UI16	SetAttackMode(BYTE *pMsg);									// ���� ���
	UI16	ResRank(BYTE *pMsg);

	// robypark 2004/12/6 17:11
	// �������� ����Ǿ���. ��� ������ �������� �κ��丮���� �����ϸ� ���� ���� ���ΰ� �������� �������.
	// ON_CHANGED_WAR_END
	UI16	OnResChangedWarEnd(BYTE *pMsg);

	// robypark 2004/12/7 12:59
	// ���������� �׾ ���� �г�Ƽ�� ������� ������.
	// ON_CHANGED_PLAYER_DEAD_PENALTY
	UI16 OnResChangedPlayerDeadPenalty(BYTE *pMsg);

	// ���� �ȿ��� ó���� �Լ���
private:	
	UI16	ResCure(BYTE *pMsg);										// �ǿ����� �Ѹ� ġ��
	UI16	ResCureAll(BYTE *pMsg);										// �ǿ����� ���� ġ��
	UI16	GetCureList(BYTE *pMsg);									// �ǿ����� ġ�Ḯ��Ʈ

	UI16	BoardShip(BYTE *pMsg);										// �谡 ���
	UI16	GetOutShip(BYTE *pMsg);										// �迡�� ������.	
	UI16	ResChangeGuildFlag(BYTE *pMsg);								// ��� ����� ����	
	UI16	OnResChangedVillageDefence(BYTE *pMsg);						// ���� ���� ����

	UI16	BankWithdraw(BYTE *pMsg);									// �������
	UI16	BankSave(BYTE *pMsg);										// �����Ա�
	UI16	BankAccountList(BYTE *pMsg);								// �����	

	UI16	BarrackBuySolderList(BYTE *pMsg);							// �Ʒü� ��븮��Ʈ
	UI16	BarrackBuySolder(BYTE *pMsg);								// �Ʒü� �������
	UI16	BarrackDismissSolderList(BYTE *pMsg);						// �Ʒü� �ذ���Ʈ
	UI16	BarrackDismissSolder(BYTE *pMsg);							// �Ʒü� �ذ�����

	UI16	GBarrackBuySolderList(BYTE *pMsg);							// ����缺�� ��븮��Ʈ
	UI16	GBarrackBuySolder(BYTE *pMsg);								// ����缺�� �������
	UI16	GBarrackDismissSolderList(BYTE *pMsg);						// ����缺�� �ذ���Ʈ
	UI16	GBarrackDismissSolder(BYTE *pMsg);							// ����缺�� �ذ�����
	UI16	GBarrackChangjobSolderList(BYTE *pMsg);						// ����缺�� ��������Ʈ
	UI16	GBarrackChangjobSolder(BYTE *pMsg);							// ����缺�� ��������

	UI16	GBarrackBuyGeneralExList(BYTE *pMsg);						// 2�� ���� ����缺�� ��븮��Ʈ
	UI16	GBarrackBuyGeneralEx(BYTE *pMsg);							// 2�� ���� ����缺�� �������
	UI16	GBarrackDismissGeneralExList(BYTE *pMsg);					// 2�� ���� ����缺�� �ذ���Ʈ
	UI16	GBarrackDismissGeneralEx(BYTE *pMsg);						// 2�� ���� ����缺�� �ذ�����
	UI16	GBarrackChangeGeneralExList(BYTE *pMsg);					// 2�� ���� ����缺�� ���� ����Ʈ
	UI16	GBarrackChangeGeneralEx(BYTE *pMsg);						// 2�� ���� ����缺�� ���� ����

	UI16	MBarrackBuyMonsterList(BYTE *pMsg);							// ���Ͱ��� ��븮��Ʈ
	UI16	MBarrackBuyMonster(BYTE *pMsg);								// ���Ͱ��� ���
	UI16	MBarrackDismissMonsterList(BYTE *pMsg);						// ���Ͱ��� �ذ���Ʈ
	UI16	MBarrackDismissMonster(BYTE *pMsg);							// ���Ͱ��� �ذ�

	UI16	InnItemList(BYTE *pMsg);									// ����缺�� �ذ�����
	UI16	InnTakeoutItem(BYTE *pMsg);									// ����缺�� ��������Ʈ
	UI16	InnSaveItem(BYTE *pMsg);									// ����缺�� ��������

	UI16	WharfEnter(BYTE *pMsg);										// �ױ� ��
	UI16	WharfBuyTicket(BYTE *pMsg);									// �ױ� Ƽ�ϱ��� ����
	UI16	WharfExit(BYTE *pMsg);										// �ױ� ������
	UI16	WharfWaitroomEnter(BYTE *pMsg);								// �ױ� ���� ��
	UI16	WharfWaitroomExit(BYTE *pMsg);								// �ױ� ���� ������
	UI16	WharfTicketList(BYTE *pMsg);								// �ױ� Ƽ�� ����Ʈ	����

	UI16	MarketChangeIteminfo(BYTE *pMsg);							// ���� ������ ����
	UI16	MarketEnter(BYTE *pMsg);									// ���� ��
	UI16	MarketItemList(BYTE *pMsg);									// ���� ������ ����Ʈ
	UI16	MarketSellItem(BYTE *pMsg);									// ���� ������ �ȱ�
	UI16	MarketBuyItem(BYTE *pMsg);									// ���� ������ ���
	UI16	MarketLeave(BYTE *pMsg);									// ���� ������
	UI16	MarketMoveItem(BYTE *pMsg);									// ���� ������ �̵�

	UI16	MakeFarmInfo(BYTE *pMsg);									// ����ü� ����Ʈ
	UI16	MakeFarmBuy(BYTE *pMsg);									// ����ü� ����

	UI16	GovernEnter(BYTE *pMsg);									// ��û ��
	UI16	GovernLeave(BYTE *pMsg);									// ��û ������
	UI16	GovernInvestSet(BYTE *pMsg);								// ��û(��������) ����
	UI16	GovernInvestGetout(BYTE *pMsg);								// ��û(��������) ����
	UI16	GovernInvestMylist(BYTE *pMsg);								// ��û(��������) ���� ������ ��������Ʈ
	UI16	GovernInvestLeave(BYTE *pMsg);
	UI16	GovernInvestProfitInfo(BYTE *pMsg);							// ��û(��������) ���� ���ͱ�
	UI16	GovernDescript(BYTE *pMsg);									// ��û(�����ȳ���) ���� �ȳ���
	UI16	GovernVillageInfo(BYTE *pMsg);								// ��û(��������) ���� ������
	UI16	GovernVillageDefenceInfo(BYTE *pMsg);						// ��û(��������) ���� ��������
	UI16	GovernVillageDefenceSet(BYTE *pMsg);						// ��û(��������) ���� �������

	UI16	GovernGuildMsg(BYTE *pMsg);									// ��û(���) 
	UI16	GovernGuildList(BYTE *pMsg);								// ��û(���) ��� ��� ����Ʈ
	UI16	GovernGuildSetRelation(BYTE *pMsg);							// ��û(���) ��ܰ� ���� ����
	UI16	GovernGuildDetailInfo(BYTE *pMsg);							// ��û(���) ��ܻ�����(��������Ʈ)
	UI16	GovernGuildJoin(BYTE *pMsg);								// ��û(���) ��� ���� ��û �� ���
	UI16	GovernGuildCreateCost(BYTE *pMsg);							// ��û(���) ��� �������
	UI16	GovernGuildCreate(BYTE *pMsg);								// ��û(���) ��� ����
	UI16	GovernGuildDelete(BYTE *pMsg);								// ��û(���) ��� ��ü
	UI16	GovernGuildWaitingMemberList(BYTE *pMsg);					// ��û(���) ����� ���
	UI16	GovernGuildWaitingMemberJoin(BYTE *pMsg);					// ��û(���) ����� ���� �� ���
	UI16	GovernGuildMemberList(BYTE *pMsg);							// ��û(���) ��ܿ� ����Ʈ
	UI16	GovernGuildDismiss(BYTE *pMsg);								// ��û(���) ��ܿ� �ذ�
	UI16	GovernGuildPromotion(BYTE *pMsg);							// ��û(���) ��ܿ� ���� �Ӹ�
	UI16	GovernGuildDemotion(BYTE *pMsg);							// ��û(���) ��ܿ� ���� ����
	UI16	GovernGuildFlagList(BYTE *pMsg);							// ��û(���) ��� ����Ʈ
	UI16	GovernGuildFlagSet(BYTE *pMsg);								// ��û(���) ��� ����

	UI16	FarmWorkCompleted(BYTE *pMsg);								// ����ü� ���� �۾��� �Ϸ�Ǿ���
	UI16	FarmSlotInfo(BYTE *pMsg);									// ����ü� ����(�ǹ�) ����
	UI16	FarmSlotExpand(BYTE *pMsg);									// ����ü� ���� Ȯ��
	UI16	FarmExit(BYTE *pMsg);										// ����ü� ������
	UI16	FarmStructBuild(BYTE *pMsg);								// ����ü� �ǹ� �Ǽ�
	UI16	FarmStructDestroy(BYTE *pMsg);								// ����ü� �ǹ� ö��
	UI16	FarmStructUpgrade(BYTE *pMsg);								// ����ü� �ǹ� ���׷��̵�
	UI16	FarmStructExit(BYTE *pMsg);									// ����ü� �ǹ� ������
	UI16	FarmStorageInfo(BYTE *pMsg);								// ����ü� â�� ����
	UI16	FarmStorageItemInput(BYTE *pMsg);							// ����ü� â�� ������ �ֱ�
	UI16	FarmStorageItemOutput(BYTE *pMsg);							// ����ü� â�� ������ ��������
	UI16	FarmProductInfo(BYTE *pMsg);								// ����ü� ������ ���� ����
	UI16	FarmProduct(BYTE *pMsg);									// ����ü� ������ �����ϱ�
	UI16	FarmProductCancel(BYTE *pMsg);								// ����ü� ������ ���� ���
	UI16	FarmProductPayChange(BYTE *pMsg);							// ����ü� ������ ���� �ӱ� ����
	UI16	FarmWorkStart(BYTE *pMsg);									// ����ü� ���ϱ� ����(���Ҽ��ִ��� üũ)
	UI16	FarmWorkResult(BYTE *pMsg);									// ����ü� ������ ���ޱ�

	
	// �ʵ�
private:
	UI16	NewInfoOnMove(BYTE *pMsg);									// �̵��߿� �߰��� ������ ĳ���� ����.
	UI16	OnDelChar(BYTE *pMsg);										// ĳ���� ����
	UI16	OnAddChar(BYTE *pMsg);										// ĳ���� �߰�
	UI16	OnResPosotion(BYTE *pMsg);									// �ٸ� ĳ������ ��ġ�� �޴´�.	
	UI16	OnResEnterVillage(BYTE *pMsg);								// ������ ��� ����.
	UI16	OnResMoveMap(BYTE *pMsg);									// ��Ż�� �̵�	
	UI16	OnResPlayerInfo(BYTE *pMsg);								// �÷��̾� ������ �޴´�.	
	UI16	OnResBattle(BYTE *pMsg);									// ���� ����	
	UI16	OnResBattlePlay(BYTE *pMsg);								//	
	UI16	OnChangeChlothes(BYTE *pMsg);								// �Ǻ��� �ٲ� �Դ´�.
	UI16	OnResReadDescrit(BYTE *pMsg);								// ���� �ȳ� ���� �д´�.	
	UI16	OnResVillageNation(BYTE *pMsg);								// ������ ������ ��� �´�.	
	UI16	OnResImoticon(BYTE *pMsg);									// �̸�Ƽ��	

	UI16	OnResParty(BYTE *pMsg);										// ��Ƽ��û
	UI16	OnResAksParty(BYTE *pMsg);									// ���濡�� ��Ƽ�� ��û�Ѱ��� �˷� �ش�.
	UI16	OnResAcceptParty(BYTE *pMsg);								// ��Ƽ ��û�� ���� ����.���,���.
	UI16	OnResQuitParty(BYTE *pMsg);									// ��Ƽ�� ������.
	UI16	OnResPartyCancel(BYTE *pMsg);
	UI16	OnResLeaveParty(BYTE *pMsg);								// ��Ƽ���� ������.
	UI16	OnResPartyDeleted(BYTE *pMsg);								// ��Ƽ ��ü.
	UI16	OnResCancelRequest(BYTE *pMsg);								// ��Ƽ ��ҿ� ���� ���� �޼���.
//	UI16    OnResPartyLeader(BYTE *pMsg);								// ��Ƽ���� �ȴ�.
//	UI16	OnResPartyMemberChange(BYTE *pMsg);							// ��Ƽ�� �������� �ֺ�����鿡�� ������ �޼���.


	UI16	OnResAcceptTrade(BYTE *pMsg);								// �ŷ� ���
	UI16	OnResTrade(BYTE *pMsg);										// �ŷ� ��û
	UI16	OnResAksTrade(BYTE *pMsg);									// ���濡�� �ŷ� ��û�Ѱ��� �˷� �ش�.	
	UI16	OnResCancelTrade(BYTE *pMsg);								// �ŷ� ���
	UI16	OnResFinishTrade(BYTE *pMsg);								// �ŷ� ��ư ����	
	UI16	OnResDecideTrade(BYTE *pMsg);								// �ŷ� �ϴ°��� ����	
	UI16	OnCompleteTrade(BYTE *pMsg);								// �ŷ� ����	
	UI16	OnChangeTrade(BYTE *pMsg);									// �ŷ� ��ǰ ����	
	UI16	OnResAttackVillage(BYTE *pMsg);								// ���� ����	
	UI16	OnResCharInfoMinimap(BYTE *pMsg);							// �̴ϸʿ��� ĳ���� ������ ��� �´�.	
	UI16	OnGuildMessage(BYTE *pMsg);									// ��� �޼���	
	UI16	OnResQuestSelectNpc(BYTE *pMsg);							// ����Ʈ �޼���	
	UI16	OnResQuestAccept(BYTE *pMsg);								// ����Ʈ �޼��� ���� ����	
	UI16	OnResQuestCamcel(BYTE *pMsg);								// ����Ʈ ���	
	UI16	OnResQuestGetPlay(BYTE *pMsg);								// �������� ����Ʈ
	UI16	OnResQuestGetPlayData(BYTE *pMsg);							// �������� ����Ʈ ����Ÿ
	UI16	OnResQuestClear(BYTE *pMsg);								// ����Ʈ �ʱ�ȭ
	UI16	OnChangeBoothItem(BYTE *pMsg);								// ���ǿ��� �������� ����Ǿ���.	
	UI16	OnResChangeBoothItem(BYTE *pMsg);							// ���ǿ��� ������ ������.	
	UI16	OnResCharDetailInfo(BYTE *pMsg);							// ĳ������ �� ����	
	UI16	OnChangeFieldItem(BYTE *pMsg);								// �ʿ� �ִ� �������� ����
	UI16	OnResProbeArea(BYTE *pMsg);									// 	
	UI16	NotifyResult(BYTE *pMsg);									// ����ο��� ��� �˸�	
	UI16	NotifyAdvertise(BYTE *pMsg);								// ��� ����	
	UI16	OnResPersonal(BYTE *pMsg);									// 	
	UI16	OnVmercenaryChange(BYTE *pMsg);								// ���� �ٴϴ� �뺴 ��ü	
	UI16	OnVmercenaryChangeMe(BYTE *pMsg);							// ���� �ٴϴ� �뺴 ��ü ����	
	UI16	OnChangeCharCode(BYTE *pMsg);								// �ֺ��� ������� �ڵ尡 ����	
	UI16	OnImperiumUse(BYTE *pMsg);
	UI16	AuditOwn(BYTE *pMsg);										// ��� �ε��꺸�� ����(����)	
	UI16	GuildInfo(BYTE *pMsg);
	UI16	MyVillageAttacked(BYTE *pMsg);								// ���� ������ ���� ���ϰ� �ִ�
	UI16	FriendList(BYTE *pMsg);										// ģ�� ����Ʈ
	UI16	FriendLogOut(BYTE *pMsg);									// ģ�� ����Ʈ�� �ִ� ģ���� �α� �ƿ�	
	UI16	VillageInfo(BYTE *pMsg);									// ���� ����	
	UI16	SellArticle(BYTE *pMsg);									// ���(�뺴,������) ��û�� ���� �Ѵ�	
	UI16	Bidding(BYTE *pMsg);										// ������ ���� ��û	
	UI16	ReportReMainTime(BYTE *pMsg);								// 	
	UI16	ReportBidding(BYTE *pMsg);									// ���� ����		
	UI16	ReportSuccessful(BYTE *pMsg);								// ����
	UI16	ReportFail(BYTE *pMsg);										// �ƹ��͵� �Ȼ�	
	UI16    ReportNew(BYTE *pMsg);										// ��Ű� ���� ����
	UI16	Enter(BYTE *pMsg);	
	UI16	OnResOpen(BYTE *pMsg);										// ���� ����
	UI16	OnResBuyItem(BYTE *pMsg);									// ���ǿ��� ������ ����
	UI16	OnResClose(BYTE *pMsg);										// ���� �ݾ�
	UI16	OnResBuyFollower(BYTE *pMsg);								// ���� �뺴 ����
	UI16	OnResChangeItem(BYTE *pMsg);								// ���� ������ ����
	UI16	OnResSetName(BYTE *pMsg);									// �뺴 �̸� ����	
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
//	UI16	OnDokdoQuestCompletedUser( BYTE *pMsg );					// actdoll (2004/03/30 11:12) : = �̺�Ʈ = ���� ����Ʈ �Ϸ��� ����
	UI16	OnBroadCast_ReloadWearItem(BYTE *pMsg);						// actdoll (2004/07/19 14:07) : = �ð��� ������ = ������ ������ ���� ����
	UI16	OnBroadCast_CurrentServerTime(BYTE *pMsg);					// robypark 2004/9/14 19:31 ���� �ð��� ���޹޾� �̺�Ʈ ����Ʈ�� ��Ȱ��ȭ�� �ش�.

	UI16	FieldAttack_Ready(BYTE* pMsg);
	UI16	FieldAttack_Action(BYTE* pMsg);
	UI16	FieldAttack_Dead(BYTE* pMsg);

	UI16	LongVillageAttack_Ready(BYTE* pMsg);
	UI16	LongVillageAttack_Action(BYTE* pMsg);

	// �����ϱ� �������̽��� ���� ��û�� ���� ���� ó��
	// ON_RESPONSE_WAR_VILLAGE_DEFENCE
	UI16	OnResponseEnterVillageDefenceInterface(BYTE *pMsg);

	/*
	// �����ϱ� �������̽����� ������ ���� ��û�� ���� ���� ó��
	// ON_RESPONSE_WAR_VILLAGE_IN
	UI16	OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg);
	*/
	// �����ϱ� �������̽��� ���´�. �ʵ��
	// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
	UI16	OnResponseLeaveVillageDefenceInterface(BYTE *pMsg);

	// robypark 2005/1/28 18:44
	// ������ �������� ���� ��û ����
	// ON_RESPONSE_CHANGE_WAR
	UI16	OnResChangeWar(BYTE *pMsg);

	// robypark 2005/1/29 14:25
	// ���� �߿� ���޼����� �ڽ��� ����ȸ��(10%) ��û ����
	// ON_RESPONSE_CHARGE_MP
	UI16	OnResponseChargeMP(BYTE *pMsg);

	// �����ϱ� �������̽����� ���� �Ӽ� ���� �ʱ�ȭ ���� ���
	// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK
	UI16	OnResponseVullageAttackTypeCheck(BYTE *pMsg);

	UI16	VillagePlayerOilAttack(BYTE* pMsg);					// ������ ���� �ݰ� - �⸧ ���� ����
	UI16	VillagePlayerStoneAttack(BYTE* pMsg);				// ������ ���� �ݰ� - �� ���� ����
	UI16	OnResponseVillageDefence(BYTE *pMsg);				// ������ ���� �ݰ� - ���� ���� ���
	// ������ ���� �ݰ��� ������.
	// ON_CHANGED_VILLAGE_ATTACK_TYPE
	UI16	OnBroadcastChangedVillageAttackType(BYTE *pMsg );

	UI16	OnIncreaseGuildUnit(BYTE* pMsg);
	UI16	OnDecreaseGuildUnit(BYTE* pMsg);

	// robypark 2004/11/9 17:27
	// ������ ���� ������ ���� ����ǰ �ޱ� �޽��� ó��
	// ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT
	UI16 OnResponseSupplyGoodsFromSupplyWagon( BYTE *pMsg );

	// robypark 2004/11/9 18:46
	// ������ ���� ������ �������� ����ǰ �ޱ�
	// ON_RESPONSE_DECREASE_GUILD_SUPPLY
	UI16 OnResponseSupplyGoodsFromVillage( BYTE *pMsg );

	// robypark 2004/11/16 15:36
	// ������ ��� ���� ���� ����Ʈ ��û ����
	// ON_RESPONSE_LIST_GUILD_UNIT
	UI16 OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	// ������ ���� ���� ���� ��û �޽��� ����
	UI16 OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg);

	// ������ ��� ����ǰ ���� ���
	// ON_RESPONSE_LIST_GUILD_SUPPLY
	UI16 OnResponseListGuildSupply(BYTE *pMsg);

	// ������ ��� ����ǰ ���� ��û ����
	// ON_RESPONSE_INCREASE_GUILD_SUPPLY
	UI16 OnResponseIncreaseGuildSupply(BYTE *pMsg);

	// ������ ���� ���� �� ���
	// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
	UI16 OnResponseGetVillageDefenceSoldier(BYTE *pMsg);

	// robypark 2004/11/25 16:4
	// ȭģ���� �޽��� ���� ����
	// ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER
	UI16 OnResponseGuildFrieldlyMessageOffer(BYTE *pMsg);

	// robypark 2004/11/26 12:47
	// ȭģ���� ���� ����
	// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT
	UI16 OnResponseGuildFriendlyMessage_Accept(BYTE *pMsg);

	// robypark 2004/11/26 12:47
	// ȭģ���� ���� ����
	// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL
	UI16 OnResponseGuildFriendlyMessage_Cancel(BYTE *pMsg);

	// robypark 2004/12/2 10:56
	// ������ ��� ������ ���� ����Ʈ �� ���� % ����
	// ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT
	UI16 OnResponseGuildVillageattackResult(BYTE *pMsg);

	// robypark 2004/12/2 13:45
	// ������ ���� �Ŀ� ������ ���� ���� ������ Ư�� ĳ���Ϳ� �ֱ� ��û
	// ON_RESPONSE_GUILD_CHOICE_PLAYER
	UI16 OnResponseGuildChoicePlayer(BYTE *pMsg);

	// robypark 2004/12/2 14:15
	// ������ ���� �Ŀ� ���� ��ź ����(���� ����)�� ���������� ���� ����
	// ON_RESPONSE_VILLAGE_INVESTMENT_POWER
	UI16 OnResponseVillageInvestmentPower(BYTE *pMsg);

	// robypark 2005/1/5 17:12
	// ������ ������ ���¿��� �Ϲ� �������� ���ư��ڴٰ��� ��û�� ���� ����
	// ON_RESPONSE_NORMAL_CHAR
	UI16 OnResponseNormalCharacter(BYTE *pMsg);

	// robypark 2005/2/15 17:43
	// �������� ȭģ���ǿ� ���ؼ� �������
	// ON_RESPONSE_NOT_WAR
	UI16 OnResponseNotWar(BYTE *pMsg);

	// robypark 2005/2/16 2:52
	// ������ �ð��� �Ǿ ����ڰ� ������ �ִٰ� �˷���
	// ON_RESPONE_WAR_TIME
	UI16 OnResponseWarTime(BYTE *pMsg);
};