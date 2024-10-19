#ifndef	_ONLINE_GOVERNMENT_MGR_HEADER
#define _ONLINE_GOVERNMENT_MGR_HEADER

#include "OnlineVillageBaseBarrack.h"
#include "OnlineGovernData.h"


enum
{   
	ON_GOVERN_MAIN_STATUS_ENTER_WAIT = 0,
	ON_GOVERN_MAIN_STATUS_VILLAGEINFO,
	ON_GOVERN_MAIN_STATUS_INVESTMENT,
	ON_GOVERN_MAIN_STATUS_GUILD,
	ON_GOVERN_MAIN_STATUS_EXIT_WAIT,
	ON_GOVERN_MAIN_STATUS_EXIT
};



class	OnlineGovernMgr : public OnlineVillageBaseStructure
{
private:
	VillageInfo		m_VillageData;
	InvestmentInfo	m_InvestData;
	GuildInfo		m_GuildData;

	UI08			m_uiMainStatus, m_uiReturnStatus;
	BOOL			m_bMouseDown;
	_Button			m_BVillageInfo, m_BInvest, m_BGuild, m_BExit;
	XSPR			m_TabImage, m_ExitImage;
	SI16			m_siX, m_siY;

public:
	OnlineGovernMgr();
	virtual	~OnlineGovernMgr();
		
	virtual	VOID	Init(UI16 VillageCode, cltOnlineWorld* pOnlineWorld, 
						VillageStructManager* pVillageStructManager);
	virtual	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);
	virtual BOOL	Action()	{ return FALSE; };

	VOID	DrawMgr(LPDIRECTDRAWSURFACE7 pSurface);
	UI16	GovernEnter(BYTE *pMsg);
	UI16	GovernLeave(BYTE *pMsg);

	// VillageInfo
	VOID	InitVillageInfo();
	VOID	FreeVillageInfo();
	VOID	InitVillageInfo_RecruitArmy();		// ���¸��� �������̽� �ʱ�ȭ
	VOID	FreeVillageInfo_RecruitArmy();		// ���¸��� �������̽� �Ҵ� �޸� �ʱ�ȭ
	VOID	InitVillageInfo_ProduceSupplyGoods();	// ����ǰ ���� �������̽� �ʱ�ȭ
	VOID	FreeVillageInfo_ProduceSupplyGoods();	// ����ǰ ���� �������̽� �Ҵ� �޸� �ʱ�ȭ
	UI08	PollVillageInfo(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawVillageInfo(LPDIRECTDRAWSURFACE7 pSurface);

	// robypark 2004/11/15 15:27
	UI08	ActionToDefenceMenu(void);			// �����ϱ�
	UI08	ActionRecruitArmy(void);			// ���¸���
	UI08	ActionProduceSupplyGoods(void);		// ����ǰ ����
	UI08	ActionDeclareOccupation(void);		// ���ɼ���

	// ��û ���� �ǿ��� ���Ǵ� ��ư �����ϱ�, ���¸���, ����ǰ ����, ���ɼ��� ��ư ���� ����(Ȱ��, ��Ȱ��)
	void	UpdateVillageInfoButton(void);

	// robypark 2004/12/27 15:42
	// ��û ���� �� - ���� �������� ���� ��ư Ȱ��ȭ ���� �����ϱ�
	void	UpdateVillageInfo_BtnRecruitArmy(void);

	// ���¸��� �������̽� �׸���
	VOID	DrawRecruitArmy(LPDIRECTDRAWSURFACE7 pSurface);

	UI16	GovernVillageInfo(BYTE *pMsg);
	UI16	GovernDescript(BYTE *pMsg);
	UI16	GovernVillageDefenceInfo(BYTE *pMsg);
	UI16	GovernVillageDefenceSet(BYTE *pMsg);

	// Investment
	VOID	InitInvestment();
	VOID	FreeInvestment();
	UI08	PollInvestment(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawInvestment(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawInvestorList(HDC hdc);
	VOID	ActionInvestScroll();

	UI16	GovernInvestProfitInfo(BYTE *pMsg);
	UI16	InvestmentList(BYTE *pMsg);
	UI16	GovernInvestMylist(BYTE *pMsg);
	UI16	GovernInvestLeave(BYTE *pMsg);
	UI16	GovernInvestSet(BYTE *pMsg);
	UI16	GovernInvestGetout(BYTE *pMsg);

	// robypark 2004/12/2 14:15
	// ������ ���� �Ŀ� ���� ��ź ����(���� ����)�� ���������� ���� ����
	// ON_RESPONSE_VILLAGE_INVESTMENT_POWER
	UI16	OnResponseVillageInvestmentPower(BYTE *pMsg);

	// Guild
	VOID	InitGuild();
	VOID	FreeGuild();
	UI08	PollGuild(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawGuild(LPDIRECTDRAWSURFACE7 pSurface);

	UI08	ActionOpen();					// ��ܼ���
	UI08	ActionWaitingList();			// ����� ����Ʈ
	UI08	ActionFlag();					// ��߼���
	UI08	ActionJoinLeave();				// ����/Ż��
	UI08	ActionClose();					// �����ü
	UI08	ActionMemberList();				// ��ܿ��Ӹ�
	UI08	ActionDiffGuild();				// �ٸ����
	UI08	ActionOfferPeace();				// ȭģ�����ϱ�

	VOID	DrawFlag(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawDiffGuildList(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawDiffGuildList(HDC hdc);
	VOID	DrawVillageList(HDC hdc);
	VOID	DrawMemberList(UI08 uiType, HDC hdc);
	VOID	DrawPage(HDC hdc);				// Page ��ȣ ���

	VOID	InitButtonStatus();				// ��� sub poll ���·� ���� �� ��ư�� ��Ȱ�� �����ش�.
	VOID	UpdateButtonStatus();			// ��ư�� Ȱ��/��Ȱ�� ���¸� �������ش�.
	VOID	ActionFlagScroll();				// ��� ��ũ��
	UI16	ActionGuildScroll(bool &bIsPressedBtnOfferPeace);		// ��ܸ���Ʈ ��ũ��
	SI16	ActionPage();					// ��������Ʈ Page ó��
	SI16	ActionMemberList(UI08 uiType);	// �����, ��ܿ�����Ʈ Page ó��

	UI16	GovernGuildCreateCost(BYTE *pMsg);
	UI16	GovernGuildCreate(BYTE *pMsg);
	UI16	GovernGuildDelete(BYTE *pMsg);
	UI16	GovernGuildJoin(BYTE *pMsg);
	UI16	GovernGuildFlagList(BYTE *pMsg);
	UI16	ResChangeGuildFlag(BYTE *pMsg);
	UI16	GovernGuildFlagSet(BYTE *pMsg);
	UI16	GovernGuildList(BYTE *pMsg);
	UI16	GovernGuildSetRelation(BYTE *pMsg);
	UI16	GovernGuildDetailInfo(BYTE *pMsg);
	UI16	GovernGuildWaitingMemberList(BYTE *pMsg);
	UI16	GovernGuildMemberList(BYTE *pMsg);
	UI16	GovernGuildWaitingMemberJoin(BYTE *pMsg);
	UI16	GovernGuildPromotion(BYTE *pMsg);
	UI16	GovernGuildDemotion(BYTE *pMsg);
	UI16	GovernGuildDismiss(BYTE *pMsg);

	// robypark 2004/11/16 15:36
	// ������ ��� ���� ���� ����Ʈ ��û ����
	// ON_RESPONSE_LIST_GUILD_UNIT
	UI16	OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	// ��û���� ����̳� ����� ���� ������ ��û�� ���� �޽��� ó��
	UI16	OnIncreaseGuildUnit(BYTE* pMsg);

	// ������ ��� ����ǰ ���� ���
	// ON_RESPONSE_LIST_GUILD_SUPPLY
	UI16	OnResponseListGuildSupply(BYTE *pMsg);

	// ������ ���� ���� ���� ��û �޽��� ����
	UI16	OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg);

	// ������ ��� ����ǰ ���� ��û ����
	// ON_RESPONSE_INCREASE_GUILD_SUPPLY
	UI16	OnResponseIncreaseGuildSupply(BYTE *pMsg);

	// ������ ���� ���� �� ���
	// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
	UI16	OnResponseGetVillageDefenceSoldier(BYTE *pMsg);

	// robypark 2004/11/25 16:4
	// ȭģ���� �޽��� ���� ����
	// ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER
	UI16 OnResponseGuildFrieldlyMessageOffer(BYTE *pMsg);
};


#endif