#ifndef	_ONLINE_GOVERNMENT_DATA_HEADER
#define _ONLINE_GOVERNMENT_DATA_HEADER

#define	VILLAGE_DESCRIPT_NOTE			254		// 마을 안내문 길이
#define	INVESTOR_MAX_LINE				5		// 투자자 목록 최대라인

#define	GUILDMAKE_MINLEVEL				30		// 상단을 만들시 최소 필요 Level
#define	GUILDMAKE_MINCREDIT				20		// 상단을 만들시 최소 필요 신용도

#define GUILD_FLAG_COST					100000	// 상단깃발 지정 요금
#define	GUILD_FLAG_MIN_NEED_MEMBER		60		// 상단깃발 지정에 필요한 최소 상단원수
#define GUILD_FLAG_XCOUNT				6		// 상단깃발 보이는 X개수 
#define GUILD_FLAG_YCOUNT				1		// 상단깃발 보이는 Y개수 

#define	GUILD_LIST_MAX_LINE				8		// 상단리스트 최대라인
#define	PAGE_LIST_MAX_LINE				5		// 마을, 상단원 리스트 최대라인

#define OFFERPEACE_MESSAGE_LENGTH		80		// 화친제의 메시지 길이(NULL문자 포함)

enum
{   
	ON_SUB_VILLAGEINFO_INIT = 0,
	ON_SUB_VILLAGEINFO_POLL,
	ON_SUB_VILLAGEINFO_SETDEFENCE,
	ON_SUB_VILLAGEINFO_WAIT,
	ON_SUB_VILLAGEINFO_EXIT,

	// robypark 2004/11/15 14:52
	// 공성전 관청 마을 정보 탭 내 메뉴 추가
	ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU,			// 수성하기
	ON_SUB_VILLAGEINFO_RECRUIT_ARMY,			// 병력 모집
	ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS,		// 보급품 생산
	ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION		// 점령 선포
};

// 공성전 관청 인터페이 중에 수성, 병력 모집등의 서브 메뉴 처리를 위한
enum
{   
	ON_SUB_VILLAGEINFO_SUB_INIT = 0,
	ON_SUB_VILLAGEINFO_SUB_POLL,
	ON_SUB_VILLAGEINFO_SUB_WAIT,
	ON_SUB_VILLAGEINFO_SUB_EXIT,
	ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY,		// 필드 전투 가능한 공성전 유닛 모집
	ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY,	// 마을 주둔군 모집
	ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS		// 마을 보급품 생산
};

enum
{   
	ON_SUB_INVEST_INIT = 0,
	ON_SUB_INVEST_POLL,
	ON_SUB_INVEST_WAIT,
	ON_SUB_INVEST_SETINVEST,
	ON_SUB_INVEST_GETOUT,
	ON_SUB_INVEST_EXIT,
	ON_SUB_INVEST_SETINVEST_ALLIN,	// 폭탄 투자 계산기 인터페이스 활성화된 경우, 서버로 폭탄 투자 메시지 전달
};

enum
{   
	ON_SUB_GUILD_INIT = 0,
	ON_SUB_GUILD_POLL,
	ON_SUB_GUILD_WAIT,
	ON_SUB_GUILD_OPEN,
	ON_SUB_GUILD_WAITING_LIST,
	ON_SUB_GUILD_FLAG,
	ON_SUB_GUILD_JOIN_LEAVE,
	ON_SUB_GUILD_CLOSE,
	ON_SUB_GUILD_MEMBER_LIST,
	ON_SUB_GUILD_DIFF_GUILD,
	ON_SUB_GUILD_EXIT
};

enum
{   
	ON_SUB_SUB_GUILD_INIT = 0,
	ON_SUB_SUB_GUILD_POLL,
	ON_SUB_SUB_GUILD_RELATION,
	ON_SUB_SUB_GUILD_VILLAGELIST,
	ON_SUB_SUB_GUILD_WAIT,
	ON_SUB_SUB_GUILD_WAIT2,
	ON_SUB_SUB_GUILD_EXIT,
	ON_SUB_SUB_GUILD_OFFERPEACE,			// 화친제의하기 메뉴 처리
	ON_SUB_SUB_GUILD_SHOW_OFFEREDPEACE,		// 화친제의한 상대방 메시지 보기 처리
};

// robypark 2004/11/25 11:10
// 화친제의 관련 인터페이스 처리
enum
{
	ON_SUB_SUB_GUILD_OFFERPEACE_INIT = 0,	// 초기화
	ON_SUB_SUB_GUILD_OFFERPEACE_POLL,		// 메인 처리
	ON_SUB_SUB_GUILD_OFFERPEACE_WAIT,		// 대기
	ON_SUB_SUB_GUILD_OFFERPEACE_EXIT,		// 종료
};

// 관청에서 공성전 병력모집할 때 모집하려는 유닛 정보 구조체
struct SRECRUIT_ArmyInformation_InGovern
{
	SI32			m_siArmyKind;				// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스
//	SI32			m_siArmyUnitCount;			// 모집하려는 유닛 부대 수
	SI32			m_siArmyUnitCost;			// 모집하려는 한 부대의 한 사람당 비용. 즉 검병 한 부대는 100이므로 한 부대의 비용은 인구수와 곱한 값이다.
	SI32			m_siArmyUnitSupplyGoods;	// 모집하려는 한 부대의 보급품 비용
	SI32			m_siArmyUnitPopulation;		// 모집하려는 한 부대의 인구수
};

// 관청에서 공성전 병력모집할 때 인터페이스 관련 구조체
struct SRECRUIT_Army_InGovern
{
	// 병력모집 인터페이스 배경 이미지
	XSPR				m_MainImage;

	// 병력모집 버튼 이미지
	XSPR				m_sprBtnRecruit;

	// 병력 모집할 때 사용되는 병과별 모집 버튼
	_Button				m_btnSiegeWarfare_Swordman;		// 공성전 검병 모집 버튼
	_Button				m_btnSiegeWarfare_Spearman;		// 공성전 창병 모집 버튼
	_Button				m_btnSiegeWarfare_Horseman;		// 공성전 기마병 모집 버튼
	_Button				m_btnSiegeWarfare_Archerman;	// 공성전 궁병 모집 버튼
	_Button				m_btnSiegeWarfare_Siege_tower;	// 공성전 충차 모집 버튼
	_Button				m_btnSiegeWarfare_Catapult;		// 공성전 발석거 모집 버튼
	_Button				m_btnSiegeWarfare_Supply_wagon;	// 공성전 보급수레 모집 버튼
	_Button				m_btnSiegeWarfare_Garrison;		// 공성전 수비병(주둔군) 모집 버튼

	// 관청에서 공성전 병력모집할 때 모집하려는 유닛 정보 구조체
	SRECRUIT_ArmyInformation_InGovern	m_sArmyInformation_InGovern;

	SI32				m_siGuildCount[ON_MAX_GUILD_UNIT];	// 수비병을 제외한 상단이 모집한 공성전 유닛들 리스트.
	UI32				m_siRecruitablePopulation;			// 모집 가능한 인구수
	BOOL				m_bDownloadedUnitCount;				// 공성전 유닛 리스트를 다운로드하였다.

	SI32				m_siDefenceSoldier;					// 수비병 수
	BOOL				m_bDownloadedDefenceSoldier;		// 공성전 수비병 수를 다운로드하였다.
};

// 마을 보급품 생산 인터페이스 구조체
struct SProduceSupplyGoods
{
	// 보급품 생산 인터페이스 배경 이미지
	XSPR				m_MainImage;

	// 보급품 생산 버튼 이미지
	XSPR				m_sprMiniButton;

	_Button				m_btnProduceSupplyGoods;			// 보급품 생산 버튼
	SI16				m_siCostSupplyGoods;				// 보급품 하나의 생산비용 125냥

	SProduceSupplyGoods()
	{
		m_siCostSupplyGoods = 125;
	}
};

// 마을정보 관련 구조체
struct VillageInfo
{
	UI08					m_uiStatus, m_uiSubStatus;
	XSPR					m_MainImage, m_DescOkImage, m_ButImage;
	_Button					m_BDescOk, m_BDefence, m_btnBack/*뒤로, 상위 메뉴로 돌아가기*/;

	// robypark 2004/11/15 13:50
	// 변경되는 공성전을 위한 관청 -마을 정보 - 추가 버튼
	_Button					m_btnToDefenceMenu;				// 수성하기 버튼
	_Button					m_btnRecruitArmy;				// 병력모집 버튼
	_Button					m_btnProduceSupplyGoods;		// 보급품 생산 버튼
	_Button					m_btnDeclareOccupation;			// 점령선포 버튼

	// 병력 모집 정보 구조체 변수
	SRECRUIT_Army_InGovern	m_sRECRUIT_Army_InGovern;

	// 마을 보급품 생산 인터페이스 구조체 변수
	SProduceSupplyGoods		m_sProduceSupplyGoods;

	UI32					m_uiPopulation;					// 인구
	SI32					m_siInOutPlayer;				// 연간 마을출입자 수
	VDEF					m_CurDefPower, m_MaxDefPower;	// 방어력
	MONEY					m_MoneyPerDef, m_MaxMoney;		// 방어력 1당 Money, 최대방어력복구자금
	CHAR					m_szStruct[256];				// 내부시설

	//===========================================================================================
	// actdoll (2004/05/04 16:33) : [OECEX]
//	OnlineEditControl*		m_pEditBox;
//	RECT					m_rcDescRect;					// 마을안내문 영역
	HOEC					m_hOecNotice;					// 마을 안내문

	SI32					m_siSupplyGoodsAmount;				// 상단 보급품 수

	VillageInfo()
	{
		m_uiPopulation = m_siInOutPlayer = m_CurDefPower = m_MaxDefPower = 0;
		ZeroMemory(m_szStruct, sizeof(m_szStruct));
//		SetRect(&m_rcDescRect, 0, 0, 0, 0);
	}
};

// 투자 관련 구조체
struct InvestmentInfo
{
	UI08				m_uiStatus;
	XSPR				m_MainImage;

	SI08				m_siTalk;								// 관청대사 종류
	MONEY				m_TotInvestMoney;						// 총 투자금
	MONEY				m_TotCapital;							// 총 수익

	XSPR				m_InvestButImage;						// 투자버튼 이미지
	_Button				m_BGetOut, m_BInvest;					// 투자금 인출, 투자
	_Button				m_BYearUp, m_BYearDown;					// 수익년도

	// 스크롤관련
	SI16				m_siScroll, m_siScrollPos;
	RECT				m_rtScroll;								// 스크롤 영역
	XSPR				m_ScrBackImage, m_ScrBarImage;			// 스크롤바 이미지
	XSPR				m_UpImage, m_DownImage;					// 업다운 버튼 이미지
	_Button				m_BScrollUp, m_BScrollDown;				// 스크롤 버튼


	// 투자자 목록, 투자
	OnInvestmentInfo	*m_pInvestList;							// 투자자 목록
	SI08				m_siInvestNum;							// 투자자 수
	MONEY				m_MyInvestMoney;						// 내가 투자한 금액
	SI32				m_siMyInvestCanTime;					// 내가 투자할 수 있는 날짜
	SI32				m_siMyInvestMaxPersent;					// 내가 투자할 수 있는 최대 퍼센트
	SI32				m_siMyInvestMinPersent;					// 내가 투자할 수 있는 최소 퍼센트
	MONEY				m_InvestAfterMoney;						// 투자 후 소지금
	MONEY				m_InvestAfterMoney_InBank;				// 투자 후 은행 저금액

	// 투자금 인출
	BYTE				m_bGetOut;								// 투자금 회수 Flag
	BYTE				m_GetOutTexPersent;						// 투자금 회수시 세금 Percent

	// 마을 수익 현황
	stCapital			*m_pCapitalList;						// 시설수익 리스트
	SI08				m_siCapitalNum;							// 시설수익 리스트 개수(년도별)
	SI08				m_siCurCapital;							// 현재 선택한 시설수익

	SI16				m_siAllInInvestmentPercent;				// 사용자가 들어와 있는 마을에 폭탄 투자 가능한 % 및 가능 여부

	InvestmentInfo()
	{
		m_siTalk = m_TotInvestMoney = m_TotCapital  = m_siScroll = m_siScrollPos = m_MyInvestMoney = m_siMyInvestCanTime
		= m_siMyInvestMaxPersent = m_siMyInvestMinPersent = m_bGetOut = m_GetOutTexPersent = m_siCapitalNum = m_siCurCapital = 0;
		m_pInvestList = NULL;
		m_pCapitalList = NULL;
		m_siAllInInvestmentPercent = 0;	
	}
};

struct SInterfaceOfferPeace
{
	HOEC				m_hOecMessage;	// 화친제의 메시지 OnlineEditControl
	_Button				m_btnSendMessage;	// 화친제의 메시지 전달
	_Button				m_btnCancel;		// 화친제의 메시지 작성 취소

	RECT				m_rtEditBox;	// 화친제의 메시지 박스의 박스 크기

	SInterfaceOfferPeace()
	{
		m_hOecMessage = NULL;
	}
};

// 상단관련 구조체들
struct GuildInfo
{
	UI08				m_uiStatus, m_uiSubStatus, m_uiSubStatus_OfferPeace/*화친제의 메뉴 처리 관련*/;
	XSPR				m_MainImage, m_ButImage;
	_Button				m_BOpen, m_BMemberList, m_BFlag, m_BJoinLeave, m_BClose, m_BAppoint, m_BDiffGuild;
	_Button				m_BOk, m_BCancel, m_BBack, m_BDHS, m_BHND;

	SI16				m_siCurPage, m_siMaxPage;

	// 스크롤 관련
	SI16				m_siScroll, m_siScrollPos;
	RECT				m_rtScroll;								// 스크롤 영역
	XSPR				m_ScrBackImage, m_ScrBarImage;			// 스크롤바 이미지
	XSPR				m_UpImage, m_DownImage;					// 업다운 버튼 이미지
	_Button				m_BScrollUp, m_BScrollDown;				// 스크롤 버튼
	SI16				m_siSelect;

	// 상단설립
	MONEY				m_GuildOpenMoney;
	//OnlineEditControl*	m_pEditBox;
	HOEC					m_hOecEditBox;

	// 대기자 리스트
	strCandidateInfo	*m_pWaitingList;
	UI16				m_uiWaitingNum;

	// 상단원 리스트
	strGuildMemberInfo	*m_pMemberList;
	UI16				m_uiMemberNum;

	// 깃발지정
	SI32				m_siFlagIndex[MAX_ONLINE_FLAG_FILE];
	SI16				m_siFlagCount;

	// 다른상단
	strGuildInfo		*m_pGuildList;							// 다른상단 리스트
	UI32				m_uiGuildNum;							// 다른상단 갯수
	XSPR				m_RelationImage;						// 적대이미지
	UI16				m_uiRelation;							// 일반, 적대, 동맹 관계설정

	// robypark 2004/11/19 17:32
	_Button				m_btnOfferPeace;						// 화친제의 버튼
	SInterfaceOfferPeace	m_sInterfaceOfferPeace;				// 화친제의 메뉴 관련 처리
	XSPR				m_sprOfferedPeace;						// 화친제의를 받은 경우 표시되는 아이콘 이미지

	// 마을리스트
	OnGuildVillageInfo	*m_pVillageList;						// 마을리스트
	UI16				m_uiVillageNum;

	GuildInfo()
	{
		m_siCurPage = m_siMaxPage = m_GuildOpenMoney = m_siFlagCount = m_siScroll = m_siScrollPos = 
		m_siSelect = m_uiGuildNum = m_uiVillageNum = m_uiWaitingNum = m_uiMemberNum = 0;
		SetRect(&m_rtScroll, 0, 0, 0, 0);
		m_pGuildList = NULL;
		m_pVillageList = NULL;
		m_pWaitingList = NULL;
		m_pMemberList = NULL;
	}
};

#endif