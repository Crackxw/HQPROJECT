/********************************************************************************************
	파일명	:	OnlineTradeBook.h
	작성자	:	정 진 욱
	담당자	:	이 준 석
	작성일	:	2001.11.08
	수정일  :	2001.11.26
********************************************************************************************/


#ifndef	__TRADEBOOK_H
#define	__TRADEBOOK_H


//---------------------------------------------------------
// 메인 
#define ON_BOOK_TOTAL_MONEY				0	// 보유재산
#define ON_BOOK_FRIEND					1	// 친구목록
#define ON_BOOK_STATUS					3
//#define ON_BOOK_PROPERTY				4
#define ON_BOOK_QUEST					5
#define ON_BOOK_INVESTMENT				6	//투자금
#define	ON_BOOK_GUILD					7	// 상단
#define ON_BOOK_PARTY					8	// 파티
#define ON_BOOK_SIEGEWARFARE			9	// 대방의 공성전 인터페이스


//투자금 정보.
#define ON_BOOK_INVESTMENT_INIT					0
#define ON_BOOK_INVESTMENT_LIST					1
#define ON_BOOK_INVESTMENT_SELECT				2
#define ON_BOOK_INVESTMENT_POLL					3
#define ON_BOOK_INVESTMENT_EXIT					4

// 나의 총재산
#define ON_BOOK_TOTAL_MONEY_INIT		0
#define ON_BOOK_TOTAL_MONEY_POLL		1
#define ON_BOOK_TOTAL_MONEY_EXIT		2
#define ON_BOOK_TOTAL_MONEY_RESET		3
#define ON_BOOK_TOTAL_MONEY_SELECT		4


struct SAVEFILE_HEADER;
struct SAVEFILE_SEARCH;
struct SAVEFILE_SORT;
struct OnAccountNumber;

#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include <vector>
#include <list>
using namespace std;


#define ON_BOOK_MAX_ACCOUNTLIST		20


struct TOTALITEM
{
	UI16							m_uiItemID;
	MONEY							m_siItemNum;
	UI16							m_uiItemCount;
};


struct TOTALMONEY																						// TAB 1
{	
	TOTALITEM						m_Item[ON_MYDATA_MAX_ITEM_NUM * ON_MAX_FOLLOWER_NUM];
	OnAuditOwnInfo					*m_pAuditOwnInfo;
	SI16							m_siItemNum;
	SI16							m_MyMoneyStatus;													// 총재산에서의 상태 변수
	UI32							m_uiTotalUser;														// 전체 등록 사용자수
	UI32							m_uiRanking;														// 자신의 랭킹
	MONEY							m_uiProperty;														// 총 자산.	
	MONEY							m_uiBankMoney;														// 전장 잔고
	MONEY							m_siLoanMoney;														// 전장 대출
	BOOL							m_bFlag;															// 서버로 부터 데이타를 한번만 받는다.
	SI16							m_siX,m_siY;
};

// 공성전 대방 인터페이스 점령한 마을 목록 관련 정보 구조체
struct SBOOK_SIEGEWARFARE_OCCUPIED_VILLAGE
{
	// 점령한 마을
	OnGuildResult		*m_pOccupiedVillageList;					// 이 상단이 점령한 마을 리스트 및 폭투 %
	SI16				m_siOccupiedVillageCount;					// 이 상단이 점령한 마을 개수

	SI16				m_siScroll;									// 스크롤
	RECT				m_rtScrollBar;								// 스크롤 사각영역
	SI16				m_siScrollPos;	
	SI16				m_siSelectedItemIdx;

	// 스크롤 바 버튼
	_Button				m_btnScrollUp;
	_Button				m_btnScrollDown;

	// 스크롤 바 스크롤 이미지
	XSPR				m_sprScrollBar;

	SBOOK_SIEGEWARFARE_OCCUPIED_VILLAGE()
	{
		m_pOccupiedVillageList = NULL;
		m_siOccupiedVillageCount = 0;

		m_siScroll = 0;												// 스크롤
		SetRect(&m_rtScrollBar, 112, 262, 112 + 9, 262 + 90);		// 스크롤 사각영역
		m_siScrollPos = 0;	
	}
};

// 공성전 대방 인터페이스 관련 구조체
struct SBOOK_SIEGEWARFARE														// TAB 2
{	
	SI16				m_siStatus;									// 상태 변수
	BOOL				m_bFlag;									// 서버로 부터 데이타를 한번만 받는다.
	SI16				m_siX,m_siY;

	// 공성 유닛 리스트
	SI32				m_siGuildCount[ON_MAX_GUILD_UNIT];			// 상단 내에 존재하는 공성 유닛 수. 수비병 제외
	BOOL				m_bDownloadedGuildUnitCount;

	MONEY				m_uiGuildSupplyGoodsAmount;					// 상단 보유 보급품 개수
	BOOL				m_bDownloadedGuildSupplyGoodsAmount;

	// 점령한 마을 목록
	SBOOK_SIEGEWARFARE_OCCUPIED_VILLAGE		m_sBook_Siegewarfare_Occupied_Village;
	BOOL				m_bDownloadedOccuiedVillage;

	// 상단원 목록 보기 페이지
	SI16				m_siCurPage, m_siMaxPage;

	// 상단원 리스트
	strGuildMemberInfo	*m_pMemberList;
	UI16				m_uiMemberNum;
	SI16				m_siSelectedGuildMemberIdx;
	BOOL				m_bDownloadedGuildMember;

	RECT				m_rtOccupiedVillageListBox;		// 점령된 마을 리스트 박스 크기
	RECT				m_rtGuildMemberListBox;			// 상단원 목록 리스트 박스 크기

	XSPR				m_sprButtonRecruitArmy;

	// 소집하려는 공성전 유닛 설정
	SI32				m_siRecruitArmyKind;		// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스
	SI32				m_siNoFieldHandle_Calc;		// 계산기 인터페이 No Field Area
	// 병력 모집 버튼
	_Button				m_btnRecruitArmy_SwordMan;		// 공성 검병
	_Button				m_btnRecruitArmy_SpearMan;		// 공성 창병
	_Button				m_btnRecruitArmy_ArcherMan;		// 공성 궁병
	_Button				m_btnRecruitArmy_HorseMan;		// 공성 기마병
	_Button				m_btnRecruitArmy_SiegeTower;	// 공성 충차
	_Button				m_btnRecruitArmy_Catapult;		// 공성 발석거
	_Button				m_btnRecruitArmy_SupplyWagon;	// 공성 보급 수레

	_Button				m_btnGiveACompetency;			// 권한 주기 버튼

	SBOOK_SIEGEWARFARE()
	{
		m_siStatus = m_bFlag = m_siX = m_siY = m_uiGuildSupplyGoodsAmount = m_siCurPage = m_siMaxPage = m_uiMemberNum = 0;

		m_bDownloadedGuildMember = m_bDownloadedGuildSupplyGoodsAmount = m_bDownloadedGuildUnitCount = m_bDownloadedOccuiedVillage = FALSE;

		ZeroMemory(m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

		m_pMemberList = NULL;

		SetRect(&m_rtOccupiedVillageListBox, 14, 256, 14 + 95, 256 + 105);
		SetRect(&m_rtGuildMemberListBox, 123, 256, 123 + 95, 256 + 105);
	}
};

struct GUILD						
{
	SI16							m_siStatus;
	SI16							m_siX,m_siY;
};

struct PARTY
{
	SI16							m_siStatus;
	SI16							m_siX,m_siY;
};
/*
struct PROPERTY																							// TAB 3
{ 
	OnAuditOwnInfo					*m_pAuditOwnInfo;
	SI16							m_PropertyStatus;													// 부동산의 상태 변수
	SI16							m_siOwnNum;															// 부동산 수
	SI16							m_siOverMouse;
	RECT							*m_pRect;
	BOOL							m_bFlag;															// 서버로 부터 데이타를 한번만 받느다.
};
*/

struct INVESTMENT
{
	OnAuditVillageInfo*				m_pAuditVillageInfo;
	SI16							m_InvestmentStatus;
	SI16							m_InvestmentCount;
	SI16							m_InvestmentUserCount;
	SI16							m_siOverMouse;
	RECT*							m_pRect;
	SI16							m_siScroll;
	RECT*							m_pListRect;
	RECT							m_rtScrollBar;
	SI16							m_siScrollPos;
	SI16							m_siTempScroll;
	MONEY							m_uMoney;
};

struct VILLAGEINFO																						// TAB 4
{
	SAVEFILE_HEADER					*m_pHeader;															// 구조체 헤더
	SAVEFILE_SEARCH					*m_pSearchHeader;	
	SAVEFILE_SORT					*m_pSort;	
	RECT							*m_pRect, *m_pSearchRect;											// 마을 이름 (버튼용)	
	SI16							m_VillageStatus;													// 마을 정보의 상태 변수
	SI16							m_VillageTotal;														// 마을 파일의 갯수	
	SI16							m_siScroll;															// 스크롤
	RECT							m_rtScrollBar;														// 스크롤 사각영역
	SI16							m_siScrollPos;	
	SI16							m_siSelect;															// 선택한 아이템 인덱스
	RECT							*m_prtItem;															// 아이템 정보창
	SI16							m_siSearchNum;														// 총 검색된 수	
	UI32							m_uiSearchID;														// 현재 검색 중인 아이템의 아이디
	SI16							m_siVillageOver;
	SI16							m_siItemOver;														// 마우스가 버튼들 위에 있을가?
	SI16							m_siSelectOver, m_siSelectSearch;
};

struct FRIENDLIST																						// TAB 1
{
	SI16							m_FriendStatus;														// 친구목록의 상태 변수
	BOOL							m_bFlag;															// 서버로 부터 데이타를 한번만 받느다.
	_Button							m_BAdd, m_BRefusal, m_BRefusalCancel, m_BDelete;//, m_BMessenger;		// 추가, 수신거부, 거부취소, 친구삭제, 1:1대화
//	_Button							m_BCheck[MAX_FRIEND_NUM]; //m_BCheckAll;								// 친구선택 체크버튼 
//	BOOL							m_bIsCheckAll;
	BOOL							m_bCheck[MAX_FRIEND_NUM];
//	OnlineEditControl				*m_pFriendEditBox;
	HOEC							m_hOecFriend;
	
	CHAR							m_szFriendName[ON_ID_LENGTH+1];										// 에디트 박스의 텍스트
	RECT							*m_pRect;															// 에디트 박스 영역
	CHAR							m_szPath[256];														// 친구목록 경로
	XSPR							m_LoginSpr;
	SI16							m_siX,m_siY;
};


class OnlineCharStatus;
 

class OnlineTradeBook
{
public:
	OnlineCharStatus				*m_pOnlineCharStatus;
	FRIENDLIST						*m_pFriendList;														// tab1의 구조체( 친구목록)

private:
	cltOnlineWorld					*m_pMyOnlineWorld;
	
	///////////////////////////////////////////////////////////////////////////////
	XSPR							m_BookMyMoneySpr;
	XSPR							m_BookSiegeWarfare;		// 공성전 대방 인터페이스 메인 이미지
	XSPR							m_MoneySmallScrollBaseSpr,m_MoneyLargeScrollBaseSpr;
	XSPR							m_BookScrollBarSpr,m_BookScrollBttnUpSpr,m_BookScrollBttnDnSpr;
	XSPR							m_BookExit;
	XSPR							m_BtnPartyBaseSpr,m_NameBaseSpr;
	XSPR							m_BookFriendSpr,m_BookGuildSpr,m_BookPartySpr;
	XSPR							m_BookTab;
	XSPR							m_FirendRefusalSpr;
	XSPR							m_GuildSmallScrollBaseSpr,m_GuildLargeScrollBaseSpr;

	XSPR							m_FriendBttnSpr;

	XSPR							m_BttnFriendMenu;

	XSPR							m_sprTown_button_medium;

	RECT							m_rtMyMoney;
	RECT							m_rtVillageScroll,m_rtInvestmentScroll;

	_Button							m_BttnMoneyScrollBar,m_BttnMoneyScrollUp,m_BttnMoneyScrollDn;
	_Button							m_BttnInvestScrollBar,m_BttnInvestScrollUp,m_BttnInvestScrollDn;
	_Button							m_BttnMoneyExit;
	_Button							m_BookFriendTab,m_BookGuildTab,m_BookPartyTab;

	// robypark 2004/11/15 19:7
	_Button							m_btnBookESTATE;		// 보유재산 보기 탭 버튼
	_Button							m_btnBookSiegeWarfare;	// 대방 공성전 병력, 보급 관련 정보 보기 탭 버튼

	_Button							m_BttnAdd, m_BttnRefusalOk,m_BttnDelete,m_BttnMessenger;
	_Button							m_BttnRefusalCancel;
	

	_Button							m_BttnGuildScrollUp1,m_BttnGuildScrollUp2,m_BttnGuildScrollUp3;
	_Button							m_BttnGuildScrollDn1,m_BttnGuildScrollDn2,m_BttnGuildScrollDn3;
	_Button							m_BttnGuildBar1,m_BttnGuildBar2,m_BttnGuildBar3;

	POINT							m_ptMenu;
	///////////////////////////////////////////////////////////////////////////////
	
	XSPR							m_CheckSpr, m_QuestBSpr;

	
	SI16							m_siX, m_siY;														// 기준 좌표
	SI16							m_siXsize;
	SI16							m_siStatus;															// 현재 장부 상태
	BOOL							m_bMouseDown;														// 마우스 상태
	BOOL							m_bActive;				
	BOOL							m_bScroll;
	POINT							m_ptMouse;															// 마우스 좌표
	RECT							m_rtBook;	
//	SI16							m_siPageStatus;														// 장부 몇번째 페이지인가


	TOTALMONEY						m_Total;															// Tab1의 구조체( 자산 )
//	PROPERTY						m_Property;															// Tab3의 구조체( 부동산 정보 )
//	VILLAGEINFO						m_VillageInfo;														// Tab4의 구조체( 마을 정보 )
	SI16							m_QuestDataStatus;													// Quest Data 상태
	SPlayerQuest					m_QuestData;														// Quest Data
	MONEY							m_QuestBankBookCount;												// Quest용 Data, 은해 계좌수
	INVESTMENT						m_Investment;
	GUILD							m_Guild;
	PARTY							m_Party;
	SBOOK_SIEGEWARFARE				m_BookDataSiegeWarfare;												// 대방 공성전 인터페이스


	SI16							m_siNoFieldHandle;													// 노 필드 지역 설정	
	RECT							m_rtScrollBar;														// 스크롤바의 사각 영역
	SI16							m_siScrollPos;														// 스크롤바의 위치
	SI16							m_siScroll;															// 스크롤
	SI16							m_siSelect;															// 현재 선택한 위치

	BOOL							m_bScrollRight;
	BOOL							m_bScrollLeft;
	BOOL							m_bScrollNow1;	
	UI32							m_siScrollX;

	OnlineMsgBox					*pMsgBox;

	SI16							m_siQuestScroll;													// Quest Scroll
//	OnAuditVillageInfo*				pOnAuditVillageInfo;
	OnInvestmentInfo*				m_pOnInvestmentInfo;
	strAuditGuildMemberInfo*		m_pOnGuildMemberInfo;



	SI32							m_siLoginGuildMember;
	SI32							m_siNowGuildMember;
	SI32							m_siSHNExceptMember;

	SI32							m_siDAICount;
	SI32							m_siDHSCount;
	SI32							m_siHNSCount;
	SI32							m_siHNDCount;
	
	strAuditGuildMemberInfo*		m_pOnGuildMemberDAI;
	strAuditGuildMemberInfo*		m_pOnGuildMemberDHS;
	strAuditGuildMemberInfo*		m_pOnGuildMemberHNS;
	strAuditGuildMemberInfo*		m_pOnGuildMemberHND;

	SI32							m_siGuildScroll1Count;
	SI32							m_siGuildScroll2Count;
	

public:
	OnlineTradeBook();
	~OnlineTradeBook();

	VOID							Init( cltOnlineWorld *pMyOnlineWorld );								// 초기화
	VOID							Initialize();														// 로그인으로 돌아갈때의 초기화
	VOID							Poll();																// 메인 폴
	VOID							Draw( LPDIRECTDRAWSURFACE7 pSurface );								// 출력
	VOID							Free();																// 날림

	BOOL							IsAction();															// 현재 활성화 인가?
	VOID							SetAction( BOOL bFlag, SI32 siFlag = 0 );							// 활성화 한다.
	VOID							BonusUp( BOOL bFlag );												// 보너스를 올린다.
	SI16							GetStatus();														// 현재 어느 창이 열려 있는지 알아 온다.
	VOID							SetStatus( SI16 siFlag );											// 어느 창을 열것인가 결정
	VOID							SetSelect( UI08 uiSlot );
	SI16							GetWidth() { return m_siXsize; };
	VOID							KillScroll();
	VOID							GetBookMoney(SI16 t) { m_Total.m_MyMoneyStatus = t ;};
	INVESTMENT*						GetInvestmentInfo()	{ return &m_Investment; };
	TOTALMONEY*						GetTotalMoney()		{ return &m_Total;};

	_Button							m_BPartyOutBasebttn;

	// 지금 상태 대화상자가 활성화 되었는지 검사
	BOOL							IsActivityBook(SI08 siBookKind);

	// 상단원 목록 요청 응답 처리
	UI16							SetGuildMemberList(BYTE *pMsg);

	// robypark 2004/11/16 15:36
	// 공성전 상단 공성 유닛 리스트 요청 응답
	// ON_RESPONSE_LIST_GUILD_UNIT
	UI16							OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	// 상단에 있는 공성전 유닛 부대를 소집권 아이템으로 변환한 결과
	UI16							OnDecreaseGuildUnit(BYTE* pMsg);

	// 공성전 상단 보급품 개수 얻기
	// ON_RESPONSE_LIST_GUILD_SUPPLY
	UI16							OnResponseListGuildSupply(BYTE *pMsg);

	// robypark 2004/12/2 10:56
	// 공성전 결과 점령한 마을 리스트 및 투자 % 응답
	// ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT
	UI16							OnResponseGuildVillageattackResult(BYTE *pMsg);

	// robypark 2004/12/2 13:45
	// 공성전 종료 후에 마을에 대한 폭투 권한을 특정 캐릭터에 주기 요청
	// ON_RESPONSE_GUILD_CHOICE_PLAYER
	UI16							OnResponseGuildChoicePlayer(BYTE *pMsg);

private:

	VOID							CheckButton();														// 버튼을 체크한다.	


	VOID							ActionMyMoney();													// 나의 총재산 액숀~
	VOID							ActionFriend();														// 친구목록
	VOID							ActionGuild();
	VOID							ActionParty();
	VOID							ActionSiegeWarfare();	// 대방 공성전 관련 인터페이스 처리

	// robypark 2004/12/24 13:34
	VOID							UpdateButtonSiegeWarfare();	// 대방 공성전 관련 인터페이스 사용 버튼 상태 갱신

	VOID							DrawMyMoney( LPDIRECTDRAWSURFACE7 pSurface );						// 나의 총재산 드로우
	VOID							DrawFriend( LPDIRECTDRAWSURFACE7 pSurface );						// 친구목록 드로우
	VOID							DrawGuild(	LPDIRECTDRAWSURFACE7 pSurface );
	VOID							DrawParty(	LPDIRECTDRAWSURFACE7 pSurface );
	VOID							DrawSiegeWarfare(LPDIRECTDRAWSURFACE7 pSurface );	// 대방 공성전 관련 인터페이스 그리기
	BOOL							DeleteFriendName();
	SI16							ItemSearch(SI16 total,UI16 id);

	// 대방 인터페이스
	// 상단원 목록 처리
	SI16							ActionMemberList(void);
	// 상단원 목록 페이지 처리
	SI16							ActionPage();
	// 상단원 목록 그리기
	VOID							DrawMemberList(HDC hdc);
	// 상단원 목록 페이지 번호 그리기
	VOID							DrawPage(HDC hdc);

public:
	BOOL							Quest_SetBankBookCount(MONEY Count);
	MONEY							Quest_GetBankBookCount( void )	{ return m_QuestBankBookCount; }		// actdoll (2004/01/14 17:02) : 추가

	void							AuditTotal(BYTE *pMsg);
	void							FriendAddOne(BYTE *pMsg);
	void							FriendRemove(BYTE *pMsg);
	void							FriendRemoveAll(BYTE *pMsg);
	void							AuditOwn(BYTE *pMsg);
	void							VillageList(BYTE *pMsg);
	void							InvestmentList(BYTE *pMsg);
	void							GuildInfo(BYTE *pMsg);

	VOID							DrawTab( LPDIRECTDRAWSURFACE7	pSurface);
	HOEC							GetHOECFriend( void )	{ return m_pFriendList->m_hOecFriend; }

	
//	void							StatiStics(BYTE *pMsg);	
};


#endif
