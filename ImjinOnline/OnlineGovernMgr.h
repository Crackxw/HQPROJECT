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
	VOID	InitVillageInfo_RecruitArmy();		// 병력모집 인터페이스 초기화
	VOID	FreeVillageInfo_RecruitArmy();		// 병력모집 인터페이스 할당 메모리 초기화
	VOID	InitVillageInfo_ProduceSupplyGoods();	// 보급품 생산 인터페이스 초기화
	VOID	FreeVillageInfo_ProduceSupplyGoods();	// 보급품 생산 인터페이스 할당 메모리 초기화
	UI08	PollVillageInfo(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawVillageInfo(LPDIRECTDRAWSURFACE7 pSurface);

	// robypark 2004/11/15 15:27
	UI08	ActionToDefenceMenu(void);			// 수성하기
	UI08	ActionRecruitArmy(void);			// 병력모집
	UI08	ActionProduceSupplyGoods(void);		// 보급품 생산
	UI08	ActionDeclareOccupation(void);		// 점령선포

	// 관청 정보 탭에서 사용되는 버튼 수성하기, 병력모집, 보급품 생산, 점령선포 버튼 상태 설정(활성, 비활성)
	void	UpdateVillageInfoButton(void);

	// robypark 2004/12/27 15:42
	// 관청 정보 탭 - 병력 모집에서 모집 버튼 활성화 여부 설정하기
	void	UpdateVillageInfo_BtnRecruitArmy(void);

	// 병력모집 인터페이스 그리기
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
	// 공성전 종료 후에 마을 폭탄 투가(권한 여부)가 가능한지에 대한 응답
	// ON_RESPONSE_VILLAGE_INVESTMENT_POWER
	UI16	OnResponseVillageInvestmentPower(BYTE *pMsg);

	// Guild
	VOID	InitGuild();
	VOID	FreeGuild();
	UI08	PollGuild(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawGuild(LPDIRECTDRAWSURFACE7 pSurface);

	UI08	ActionOpen();					// 상단설립
	UI08	ActionWaitingList();			// 대기자 리스트
	UI08	ActionFlag();					// 깃발선택
	UI08	ActionJoinLeave();				// 가입/탈퇴
	UI08	ActionClose();					// 상단해체
	UI08	ActionMemberList();				// 상단원임명
	UI08	ActionDiffGuild();				// 다른상단
	UI08	ActionOfferPeace();				// 화친제의하기

	VOID	DrawFlag(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawDiffGuildList(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	DrawDiffGuildList(HDC hdc);
	VOID	DrawVillageList(HDC hdc);
	VOID	DrawMemberList(UI08 uiType, HDC hdc);
	VOID	DrawPage(HDC hdc);				// Page 번호 출력

	VOID	InitButtonStatus();				// 상단 sub poll 상태로 들어갔을 때 버튼을 비활성 시켜준다.
	VOID	UpdateButtonStatus();			// 버튼의 활성/비활성 상태를 갱신해준다.
	VOID	ActionFlagScroll();				// 깃발 스크롤
	UI16	ActionGuildScroll(bool &bIsPressedBtnOfferPeace);		// 상단리스트 스크롤
	SI16	ActionPage();					// 마을리스트 Page 처리
	SI16	ActionMemberList(UI08 uiType);	// 대기자, 상단원리스트 Page 처리

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
	// 공성전 상단 공성 유닛 리스트 요청 응답
	// ON_RESPONSE_LIST_GUILD_UNIT
	UI16	OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	// 관청에서 대방이나 행수가 병력 모집을 요청한 응답 메시지 처리
	UI16	OnIncreaseGuildUnit(BYTE* pMsg);

	// 공성전 상단 보급품 개수 얻기
	// ON_RESPONSE_LIST_GUILD_SUPPLY
	UI16	OnResponseListGuildSupply(BYTE *pMsg);

	// 공성전 마을 수비병 증가 요청 메시지 응답
	UI16	OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg);

	// 공성전 상단 보급품 생산 요청 응답
	// ON_RESPONSE_INCREASE_GUILD_SUPPLY
	UI16	OnResponseIncreaseGuildSupply(BYTE *pMsg);

	// 공성전 마을 수비병 수 얻기
	// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
	UI16	OnResponseGetVillageDefenceSoldier(BYTE *pMsg);

	// robypark 2004/11/25 16:4
	// 화친제의 메시지 보낸 응답
	// ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER
	UI16 OnResponseGuildFrieldlyMessageOffer(BYTE *pMsg);
};


#endif