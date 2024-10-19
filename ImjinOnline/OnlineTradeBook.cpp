/********************************************************************************************
	파일명	:	OnlineTradeBook.cpp
	작성자	:	정 진 욱
	담당자	:	이 준 석
	작성일	:	2001.11.08
	수정일  :	2002.02.22
	내  용  :	장부
********************************************************************************************/
#include <GSL.h>


#include	<Main.h>
#include    <Mouse.h>
#include	<clGame.h>

#include	"OnlineWorld.h"
#include	<OnlineText.h>
#include	"OnlineTradeBook.h"
#include	"OnlineMegaText.h"
#include	"OnlineVillageSave.h"
#include	"OnlineVillage-Parser.h"
#include	"OnlineHelp.h"
#include	"OnlineProductStructure.h"
#include	"OnlineFont.h"
#include	"OnlineClient.h"
#include	"OnlineEconomy.h"
#include	"OnlineItem.h"
#include	"OnlineResource.h"
#include	"OnlineFieldChat.h"
#include	"OnlineMiniMap.h"
#include	"OnlineCharStatus.h"
#include	"OnlineFieldArea.h"
#include	"OnlineVillageManager.h"
#include	"OnlineFollower.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineTrade.h"
#include	"OnlineBooth.h"
#include	"OnlineMsgBox.h"
#include	"OnlineSkillParser.h"
#include	"OnlineMessenger.h"
#include	"OnlineQuest-Parser.h"
#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include	"_SkillDefine.h"
#include	"onlinevillagestructmanager.h"
#include    "OnlineETC.h"
//#include	"OnlineBasicTip.h"					// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
//#include	"OnlineBasicQuest.h"				// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
#include	"OnlineParty.h"

#include	"OnlineGovernData.h"

#include	"..\EnumData\Enum_KI_Character.h"		// KindInfo - Character

#include	"OnlineNumberControl.h"


extern _InputDevice				IpD;
extern _clGame* pGame;

#define ON_BOOK_TOTAL_LINE			15
#define ON_BOOK_INVESTMENT_LINE     5		// 투자한 마을 리스트가 리스트박스에서 보여지는 최대 개수
#define ON_BOOK_INVESTMENT_LIST_LINE 6		// 투자한 마을의 투자자 리스트가 리스트박스에서 보여지는 최대 개수
#define ON_BOOK_ACCOUNT_LINE		9
#define ON_BOOK_ITEM_LINE			9
#define ON_BOOK_ALL_ITEM_LINE		10
#define ON_BOOK_SCROLL_BAR_TOP		260

#define ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT	3	// 점령한 마을 목록 리스트 박스에서 화면에 보여지는 최대 개수


//---------------------------------------------------------
// 공성전 대방 인터페이스 처리
#define ON_BOOK_SIEGEWARFARE_INIT				0
#define ON_BOOK_SIEGEWARFARE_POLL				1
#define ON_BOOK_SIEGEWARFARE_WAIT				2
#define ON_BOOK_SIEGEWARFARE_EXIT				3
#define ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY		4
#define ON_BOOK_SIEGEWARFARE_GIVE_A_COMPETENCY	5


//---------------------------------------------------------
// 친구목록
#define ON_BOOK_FRIEND_INIT						0
#define ON_BOOK_FRIEND_POLL						1
#define ON_BOOK_FRIEND_ADD						2
#define ON_BOOK_FRIEND_REFUSAL					3
#define ON_BOOK_FRIEND_REFUSAL_CANCEL			4
#define ON_BOOK_FRIEND_DELETE					5
//#define ON_BOOK_FRIEND_DELETE_ALL				6
#define ON_BOOK_FRIEND_MESSENGER				6


//----------------------------------------------------------
// Quest스킬
#define ON_BOOK_QUEST_INIT						0
#define ON_BOOK_QUEST_PLAYINGQUEST				1
#define ON_BOOK_QUEST_COMPLETEDQUEST			2

#define	ON_BOOK_GUILD_INIT						0
#define ON_BOOK_GUILD_POLL						1
#define ON_BOOK_GUILD_EXIT						2

#define ON_BOOK_PARTY_INIT						0
#define ON_BOOK_PARTY_POLL						1
#define ON_BOOK_PARTY_EXIT						2

#define PARTY_CHECK_MSG		1
#define PARTY_MSG			2
#define PARTY_ACTION		3
#define PARTY_WAIT			4
#define PARTY_NO_MSG		5
#define PARTY_DISS			6

POINT m_ptCharNamePos[6] = 
{
	{54,101},{54,132},{54,163},
	{54,194},{54,225},{54,256}
};


OnlineTradeBook::OnlineTradeBook()
{
	m_Investment.m_pAuditVillageInfo = NULL;
}

OnlineTradeBook::~OnlineTradeBook()
{
	Free();
}

VOID	OnlineTradeBook::Initialize()
{
	SetAction(FALSE);
//	m_bScrollLeft	=	FALSE;

	m_pMyOnlineWorld->pMyData->m_GoodFriendList.DeleteAllFriend();
	m_pFriendList->m_bFlag = FALSE;

	m_pOnGuildMemberDAI			=	NULL;
	m_pOnGuildMemberDHS			=	NULL;
	m_pOnGuildMemberHNS			=	NULL;
	m_pOnGuildMemberHND			=	NULL;


	m_siLoginGuildMember		=   0;
	m_siNowGuildMember			=   0;
	m_siSHNExceptMember			=   0;
	
	m_siDAICount				=	0;
	m_siDHSCount				=	0;
	m_siHNSCount				=	0;
	m_siHNDCount				=	0;

	m_siGuildScroll2Count		=	0;
	m_siGuildScroll1Count		=	0;

	m_Investment.m_InvestmentCount = 0;

	if(m_Investment.m_pAuditVillageInfo)
	{
		delete m_Investment.m_pAuditVillageInfo;
		m_Investment.m_pAuditVillageInfo = NULL;
	}

	if(m_pOnInvestmentInfo)
	{
		delete[] m_pOnInvestmentInfo;
		m_pOnInvestmentInfo = NULL;
	}

	if (m_BookDataSiegeWarfare.m_pMemberList)
	{
		delete []m_BookDataSiegeWarfare.m_pMemberList;
		m_BookDataSiegeWarfare.m_pMemberList = NULL;
	}

	// 상단원 목록 초기화
	ZeroMemory(m_BookDataSiegeWarfare.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	// 점령한 마을 정보(목록 및 투자%) 초기화
	if (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList)
	{
		delete []m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList;

		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList = NULL;
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount = 0;
	}

	m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll = 0;												// 스크롤
	m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos = 0;	

//	m_pFriendList->m_pFriendEditBox->Initialize();
	// 대방이라면 공성전 병력, 보급품 관련 인터페이스 메뉴 버튼 활성화
	if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
	{
		m_btnBookSiegeWarfare.SetAction(TRUE);
	}
	else
	{
		m_btnBookSiegeWarfare.SetAction(FALSE);
	}
}

//______________________________________________________________________________________________________________
// 메인 폴
VOID	OnlineTradeBook::Init( cltOnlineWorld *pMyOnlineWorld )
{
	m_pMyOnlineWorld	=	pMyOnlineWorld;


	clGrp.LoadXspr("Online\\Gamespr\\Book\\Book_2.Spr",m_BookMyMoneySpr);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_2_SCRBAR_1.Spr",m_MoneySmallScrollBaseSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_2_SCRBAR_2.Spr",m_MoneyLargeScrollBaseSpr);

	clGrp.LoadXspr("Online\\Gamespr\\Book\\Book_2_2.Spr",m_BookSiegeWarfare);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\BOOK_BUTTON_MINI.Spr", m_BookDataSiegeWarfare.m_sprButtonRecruitArmy);

	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_1.Spr",m_BookFriendSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_2X.Spr",m_BookGuildSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_3.Spr",m_BookPartySpr);

	clGrp.LoadXspr("Online\\GameSpr\\Book\\book_3_tab.Spr",m_BookTab);


	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_handle.Spr",m_BookScrollBarSpr);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_Up.Spr",m_BookScrollBttnUpSpr);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_Down.Spr",m_BookScrollBttnDnSpr);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\main_window_button.Spr",m_BookExit);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\Book_Button_small.Spr",m_BtnPartyBaseSpr);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\BOOK_3_3_NAMEBOX.Spr",m_NameBaseSpr);
	

	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_button_small.Spr",m_FriendBttnSpr);

	clGrp.LoadXspr("Online\\Gamespr\\Book\\fulldownmenu.Spr",m_BttnFriendMenu);
	
	clGrp.LoadXspr("Online\\GameSpr\\CheckButton.Spr", m_CheckSpr);
	clGrp.LoadXspr("Online\\GameSpr\\QuestButton.Spr", m_QuestBSpr);	

	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_1_CUT_ICON.Spr",m_FirendRefusalSpr);

	
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_2_SCRBAR_1.Spr",m_GuildSmallScrollBaseSpr);

	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_2_SCRBAR_2.Spr",m_GuildLargeScrollBaseSpr);

	clGrp.LoadXspr("Online\\GameSpr\\Book\\small_window_button.spr", m_sprTown_button_medium);

	// 각각의 구조체를 초기화 한다.
	ZeroMemory( &m_Total, sizeof( TOTALMONEY ) );
	m_QuestDataStatus = ON_BOOK_QUEST_INIT;
	ZeroMemory( &m_QuestData, sizeof( SPlayerQuest ) );
	m_QuestBankBookCount = 0;

	pMsgBox = new OnlineMsgBox;
	pMsgBox->Init(pMyOnlineWorld);

	m_Total.m_siX				=	95;
	m_Total.m_siY				=	45;

	SetRect(&m_rtMyMoney, m_Total.m_siX, m_Total.m_siY, 
		m_Total.m_siX + m_BookMyMoneySpr.Header.Xsize, m_Total.m_siY + m_BookMyMoneySpr.Header.Ysize);

	SetRect(&m_rtVillageScroll, m_Total.m_siX + 221, m_Total.m_siY + 175, 
		m_Total.m_siX + 231,m_Total.m_siY + 232);

	SetRect(&m_rtInvestmentScroll,m_Total.m_siX + 221 , m_Total.m_siY + 271,
		m_Total.m_siX + 231 , m_Total.m_siY + 361);
			  
	m_siX						=	0;//800 - m_pBook.Header.Xsize;
	m_siY						=	0;
	m_siNoFieldHandle			=	-1;
	m_siScroll					=	0;
//	m_siPageStatus				=	1;
	m_bMouseDown				=	FALSE;
	m_bActive					=	FALSE;
	m_bScroll					=	FALSE;
	m_siStatus					=	ON_BOOK_TOTAL_MONEY;
	m_pOnlineCharStatus			=	new OnlineCharStatus;	
	m_Investment.m_pRect		=   new RECT[ ON_BOOK_INVESTMENT_LINE ]; //유저투자정보 
	m_Investment.m_pListRect	=   new RECT[ ON_BOOK_INVESTMENT_LIST_LINE ]; //투자정보 리스트.
	m_pOnInvestmentInfo			=	NULL; 
	m_pOnGuildMemberInfo		=   NULL;

	m_pOnGuildMemberDAI			=	NULL;
	m_pOnGuildMemberDHS			=	NULL;
	m_pOnGuildMemberHNS			=	NULL;
	m_pOnGuildMemberHND			=	NULL;


	m_siLoginGuildMember		=   0;
	m_siNowGuildMember			=   0;
	m_siSHNExceptMember			=   0;

	m_siDAICount				=	0;
	m_siDHSCount				=	0;
	m_siHNSCount				=	0;
	m_siHNDCount				=	0;

	m_siGuildScroll2Count		=	0;
	m_siGuildScroll1Count		=	0;


	SI32	i;

	// 투자한 마을 리스트 박스 클릭 위치 설정
	for(i = 0; i < ON_BOOK_INVESTMENT_LINE; i++)
	{
		SetRect(&m_Investment.m_pRect[i], m_Total.m_siX + 18, m_Total.m_siY + 170 + ( i * 14 ), m_Total.m_siX + 18 + 200, m_Total.m_siY + 170 + ( i * 14 ) + 14 );
	}
	// 투자자 리스트 박스 클릭 위치 설정
	for(i = 0 ; i < ON_BOOK_INVESTMENT_LIST_LINE; i++)
	{
		SetRect(&m_Investment.m_pListRect[i],m_siX + 20, m_siY + 268 + ( i * 19 ), m_siX + 20 + 200, m_siY + 268 + ( i * 19 ) + 19 );
	}
			
	SetRect( &m_rtBook, m_Total.m_siX, m_Total.m_siY, m_Total.m_siX + m_BookMyMoneySpr.Header.Xsize, m_Total.m_siY + m_BookMyMoneySpr.Header.Ysize );
	
	m_Total.m_MyMoneyStatus			=	ON_BOOK_TOTAL_MONEY_INIT;	
	m_Total.m_bFlag					=	FALSE;	
	m_Investment.m_InvestmentStatus =   ON_BOOK_INVESTMENT_INIT;
	m_siScrollPos					=	136;
	m_Total.m_uiTotalUser			=	0;
	m_Total.m_uiRanking				=	0;
	m_Total.m_uiProperty			=	0;
	m_Total.m_uiBankMoney			=	0;
	m_Total.m_siLoanMoney			=	0;

	m_Investment.m_siScrollPos		=	0;

	
	m_BttnMoneyScrollUp.Create(m_Total.m_siX + 221, m_Total.m_siY + 166,m_BookScrollBttnUpSpr.Header.Xsize,m_BookScrollBttnUpSpr.Header.Ysize,"",BUTTON_PUT_LEFT, TRUE );
	m_BttnMoneyScrollDn.Create(m_Total.m_siX + 221, m_Total.m_siY + 232,m_BookScrollBttnDnSpr.Header.Xsize,m_BookScrollBttnDnSpr.Header.Ysize,"",BUTTON_PUT_LEFT, TRUE );
	
	m_BttnInvestScrollUp.Create(m_Total.m_siX + 221,m_Total.m_siY +	262,m_BookScrollBttnUpSpr.Header.Xsize,m_BookScrollBttnUpSpr.Header.Ysize,"",BUTTON_PUT_LEFT, TRUE );
	m_BttnInvestScrollDn.Create(m_Total.m_siX + 221,m_Total.m_siY +	361,m_BookScrollBttnDnSpr.Header.Xsize,m_BookScrollBttnDnSpr.Header.Ysize,"",BUTTON_PUT_LEFT, TRUE );

	m_BttnMoneyScrollBar.Create(m_Total.m_siX + 221, m_Total.m_siY + 151, m_BookScrollBarSpr.Header.Xsize,m_BookScrollBarSpr.Header.Ysize,"",BUTTON_PUT_LEFT, TRUE );
	m_BttnInvestScrollBar.Create(m_Total.m_siX + 221,m_Total.m_siY + 271,m_BookScrollBarSpr.Header.Xsize,m_BookScrollBarSpr.Header.Ysize,"",BUTTON_PUT_LEFT, TRUE );

	m_BttnMoneyExit.Create(m_Total.m_siX + 85, m_Total.m_siY + 394 , m_BookExit.Header.Xsize, m_BookExit.Header.Ysize + 2,m_pMyOnlineWorld->pOnlineText->Get(8000492),BUTTON_PUT_LEFT, TRUE );

	// 보유재산, 대방 공성 병력, 보급품 인터페이스 탭 버튼
	m_btnBookESTATE.Create(		 m_Total.m_siX + 36, m_Total.m_siY + 31, 72,24,m_pMyOnlineWorld->pOnlineText->Get(8000552),BUTTON_PUT_LEFT,TRUE);
	m_btnBookSiegeWarfare.Create(m_Total.m_siX + 130, m_Total.m_siY + 31, 72,24,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_SIEGEWARFARE),BUTTON_PUT_LEFT, FALSE);

	/////////////////////////**** 친구목록 ****////////////////////////////////
	m_pFriendList = new FRIENDLIST;

	m_pFriendList->m_siX = 95;
	m_pFriendList->m_siY = 45;

	ZeroMemory(m_pFriendList->m_szFriendName, ON_ID_LENGTH+1);
	m_pFriendList->m_bFlag			=	FALSE;
	m_pFriendList->m_FriendStatus	=	ON_BOOK_FRIEND_INIT;
	m_pFriendList->m_pRect			=	new RECT;
	SetRect( m_pFriendList->m_pRect, m_pFriendList->m_siX + 36, m_pFriendList->m_siY + 361, m_pFriendList->m_siX + 139, m_pFriendList->m_siY + 380);

	

	m_Guild.m_siX		 = 95;
	m_Guild.m_siY		 = 45;

	m_Party.m_siX		 = 95;
	m_Party.m_siY		 = 45;

	m_BttnGuildScrollUp1.Create(m_Party.m_siX + 221,m_Party.m_siY + 179,m_BookScrollBttnUpSpr.Header.Xsize,m_BookScrollBttnUpSpr.Header.Ysize,
		"",BUTTON_PUT_LEFT, TRUE);
	m_BttnGuildScrollDn1.Create(m_Party.m_siX + 221,m_Party.m_siY + 265,m_BookScrollBttnDnSpr.Header.Xsize,m_BookScrollBttnDnSpr.Header.Ysize,
		"",BUTTON_PUT_LEFT, TRUE);

	m_BttnGuildScrollUp2.Create(m_Party.m_siX + 221,m_Party.m_siY + 294,m_BookScrollBttnUpSpr.Header.Xsize,m_BookScrollBttnUpSpr.Header.Ysize,
		"",BUTTON_PUT_LEFT, TRUE);
	m_BttnGuildScrollDn2.Create(m_Party.m_siX + 221,m_Party.m_siY + 380,m_BookScrollBttnDnSpr.Header.Xsize,m_BookScrollBttnDnSpr.Header.Ysize,
		"",BUTTON_PUT_LEFT, TRUE);

	m_BttnGuildScrollUp3.Create(m_Party.m_siX + 221,m_Party.m_siY + 294,m_BookScrollBttnUpSpr.Header.Xsize,m_BookScrollBttnUpSpr.Header.Ysize,
		"",BUTTON_PUT_LEFT, TRUE);
	m_BttnGuildScrollDn3.Create(m_Party.m_siX + 221,m_Party.m_siY + 380,m_BookScrollBttnDnSpr.Header.Xsize,m_BookScrollBttnDnSpr.Header.Ysize,
		"",BUTTON_PUT_LEFT, TRUE);

	m_BPartyOutBasebttn.Create(m_Party.m_siX + 90,m_Party.m_siY + 304,m_BtnPartyBaseSpr.Header.Xsize,
		m_BtnPartyBaseSpr.Header.Ysize,m_pMyOnlineWorld->pOnlineText->Get(8000541),BUTTON_PUT_LEFT, TRUE);

	m_BookFriendTab.Create(m_pFriendList->m_siX + 10, m_pFriendList->m_siY + 31, 72,24,m_pMyOnlineWorld->pOnlineText->Get(8000542),BUTTON_PUT_LEFT,TRUE);
	m_BookGuildTab.Create(m_pFriendList->m_siX + 83, m_pFriendList->m_siY + 31, 72,24,m_pMyOnlineWorld->pOnlineText->Get(8000543),BUTTON_PUT_LEFT,TRUE);
	m_BookPartyTab.Create(m_pFriendList->m_siX + 156, m_pFriendList->m_siY + 31, 72,24,m_pMyOnlineWorld->pOnlineText->Get(8000544),BUTTON_PUT_LEFT,TRUE);

	m_BttnAdd.Create( m_pFriendList->m_siX + 141, m_pFriendList->m_siY + 362,m_FriendBttnSpr.Header.Xsize,m_FriendBttnSpr.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(8000545), BUTTON_PUT_LEFT, TRUE );
	m_BttnMessenger.Create(m_pFriendList->m_siX, m_pFriendList->m_siY,m_BttnFriendMenu.Header.Xsize,m_BttnFriendMenu.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(8000546), BUTTON_PUT_LEFT, TRUE );
	m_BttnRefusalOk.Create(m_pFriendList->m_siX, m_pFriendList->m_siY,m_BttnFriendMenu.Header.Xsize,m_BttnFriendMenu.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(8000547), BUTTON_PUT_LEFT, TRUE );
	m_BttnRefusalCancel.Create(m_pFriendList->m_siX, m_pFriendList->m_siY,m_BttnFriendMenu.Header.Xsize,m_BttnFriendMenu.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(8000548), BUTTON_PUT_LEFT, TRUE );
	m_BttnDelete.Create(m_pFriendList->m_siX, m_pFriendList->m_siY,m_BttnFriendMenu.Header.Xsize,m_BttnFriendMenu.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(6300003), BUTTON_PUT_LEFT, TRUE );
	
//	m_pFriendList->m_pFriendEditBox = new OnlineEditControl;
//	m_pFriendList->m_pFriendEditBox->CreateEditBox(1000, 1000, 80, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_ID_LENGTH );
//	m_pFriendList->m_pFriendEditBox->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
//	SendMessage( m_pFriendList->m_pFriendEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)(ON_ID_LENGTH), 0 );

	OnlineEditControlEx	*pOecEx;
	if( m_pFriendList->m_hOecFriend )	Handler_OnlineEditControlEx::KillControl( m_pFriendList->m_hOecFriend );
	RECT	rcRect  = { m_pFriendList->m_siX + 36, m_pFriendList->m_siY + 361, m_pFriendList->m_siX + 139, m_pFriendList->m_siY + 380};
	m_pFriendList->m_hOecFriend	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, ON_ID_LENGTH);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_pFriendList->m_hOecFriend );
	pOecEx->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(216, 201, 183) );

	for(i = 0; i < MAX_FRIEND_NUM; i++)
		m_pFriendList->m_bCheck[i] = FALSE;

	//////////////////// 공성전 대방 인터페이스 관련 ////////////////////////////////////////////
	m_BookDataSiegeWarfare.m_siX		 = 95;
	m_BookDataSiegeWarfare.m_siY		 = 45;

	SetRect(&m_BookDataSiegeWarfare.m_rtOccupiedVillageListBox, 14, 256, 14 + 95 + 12, 256 + 105);
	SetRect(&m_BookDataSiegeWarfare.m_rtGuildMemberListBox, 123, 256, 123 + 95, 256 + 105);
	OffsetRect(&m_BookDataSiegeWarfare.m_rtOccupiedVillageListBox, m_BookDataSiegeWarfare.m_siX, m_BookDataSiegeWarfare.m_siY);
	OffsetRect(&m_BookDataSiegeWarfare.m_rtGuildMemberListBox, m_BookDataSiegeWarfare.m_siX, m_BookDataSiegeWarfare.m_siY);

	// 스크롤 바 초기화
	m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll = 0;												// 스크롤
	SetRect(&m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar, 112, 262, 112 + 9, 262 + 90);		// 스크롤 사각영역
	OffsetRect(&m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar, m_BookDataSiegeWarfare.m_siX, m_BookDataSiegeWarfare.m_siY);
	m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos = 0;	

	// 점령한 마을 목록 스크롤 바 버튼 생성
	m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollUp.Create(m_BookDataSiegeWarfare.m_siX + 112, m_BookDataSiegeWarfare.m_siY + 253,
									m_BookScrollBttnUpSpr.Header.Xsize,m_BookScrollBttnUpSpr.Header.Ysize,
									"",BUTTON_PUT_LEFT, TRUE);
	m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollDown.Create(m_BookDataSiegeWarfare.m_siX + 112, m_BookDataSiegeWarfare.m_siY + 352,
									m_BookScrollBttnDnSpr.Header.Xsize,m_BookScrollBttnDnSpr.Header.Ysize,
									"",BUTTON_PUT_LEFT, TRUE);

	// 스크롤 바 이미지 로드
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_2_2_SCRBAR.Spr", m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_sprScrollBar);

	/*
	// 모집 버튼 생성
	// 검병 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 65, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY),BUTTON_PUT_LEFT, TRUE);

	// 창병 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 89, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY),BUTTON_PUT_LEFT, TRUE);

	// 궁병 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 113, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY), BUTTON_PUT_LEFT, TRUE);

	// 기마병 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 137, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY),BUTTON_PUT_LEFT, TRUE);

	// 충차 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 161, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY),BUTTON_PUT_LEFT, TRUE);

	// 발석거 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 185, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY),BUTTON_PUT_LEFT, TRUE);

	// 보급수레 모집
	m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.Create(m_BookDataSiegeWarfare.m_siX + 182,m_BookDataSiegeWarfare.m_siY + 209, m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Xsize,m_BookDataSiegeWarfare.m_sprButtonRecruitArmy.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_RECRUIT_ARMY),BUTTON_PUT_LEFT, TRUE);
	*/

	// 권한 주기 버튼
	m_BookDataSiegeWarfare.m_btnGiveACompetency.Create(m_BookDataSiegeWarfare.m_siX + 161,m_BookDataSiegeWarfare.m_siY + 367, m_sprTown_button_medium.Header.Xsize,m_sprTown_button_medium.Header.Ysize,
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BUTTON_GIVE_A_COMPETENCY),BUTTON_PUT_LEFT, TRUE);
}

//______________________________________________________________________________________________________________
// 메인 폴
VOID	OnlineTradeBook::Poll()
{
	if( m_bActive == FALSE )	
	{
		return;
	}

	CheckButton();

	if( m_bActive == FALSE )	
	{
		return;
	}

	CHAR	*pDesc = NULL;

	m_pMyOnlineWorld->pIOnlineMouse->GetPos( &m_ptMouse );	

	switch( m_siStatus )
	{
		case ON_BOOK_TOTAL_MONEY:		ActionMyMoney();					break;
		case ON_BOOK_STATUS:			m_pOnlineCharStatus->Action();		break;	
		case ON_BOOK_FRIEND:			ActionFriend();						break;
		case ON_BOOK_GUILD:				ActionGuild();						break;
		case ON_BOOK_PARTY:				ActionParty();						break;
		case ON_BOOK_SIEGEWARFARE:		ActionSiegeWarfare();				break;
	}
	

	if( IpD.LeftPressSwitch )					m_bMouseDown	=	TRUE;
	else										m_bMouseDown	=	FALSE;

}


//______________________________________________________________________________________________________________
// 출력
VOID	OnlineTradeBook::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	if( m_bActive == FALSE )				return;	

	HDC				hDC;

	if( clGrp.LockSurface( pSurface ) == TRUE ) 
	{		
		m_BttnMoneyExit.Put(&m_BookExit,0,2,1,BUTTON_PUT_NOMOVE );

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
		
		pSurface->ReleaseDC( hDC );
	}

	switch( m_siStatus )
	{
		case ON_BOOK_TOTAL_MONEY:		DrawMyMoney( pSurface );					break;
		case ON_BOOK_FRIEND:			DrawFriend(pSurface);						break;
		case ON_BOOK_GUILD:				DrawGuild(pSurface);						break;
		case ON_BOOK_PARTY:				DrawParty(pSurface);						break;
		case ON_BOOK_STATUS:			m_pOnlineCharStatus->Draw(pSurface);		break;		
		case ON_BOOK_SIEGEWARFARE:		DrawSiegeWarfare( pSurface );				break;
	}
}


//______________________________________________________________________________________________________________
// 출력 
VOID	OnlineTradeBook::Free()
{
	clGrp.FreeXspr(m_BookMyMoneySpr);
	clGrp.FreeXspr(m_BookSiegeWarfare);
	clGrp.FreeXspr(m_MoneySmallScrollBaseSpr);
	clGrp.FreeXspr(m_MoneyLargeScrollBaseSpr);
	clGrp.FreeXspr(m_BookDataSiegeWarfare.m_sprButtonRecruitArmy);
	clGrp.FreeXspr(m_BookFriendSpr);
	clGrp.FreeXspr(m_BookGuildSpr);
	clGrp.FreeXspr(m_BookPartySpr);
	clGrp.FreeXspr(m_BookTab);
	clGrp.FreeXspr(m_BookScrollBarSpr);
	clGrp.FreeXspr(m_BookScrollBttnUpSpr);
	clGrp.FreeXspr(m_BookScrollBttnDnSpr);
	clGrp.FreeXspr(m_BookExit);
	clGrp.FreeXspr(m_BtnPartyBaseSpr);
	clGrp.FreeXspr(m_NameBaseSpr);
	clGrp.FreeXspr(m_FriendBttnSpr);
	clGrp.FreeXspr(m_BttnFriendMenu);
	clGrp.FreeXspr(m_CheckSpr);
	clGrp.FreeXspr(m_QuestBSpr);	
	clGrp.FreeXspr(m_FirendRefusalSpr);
	clGrp.FreeXspr(m_GuildSmallScrollBaseSpr);
	clGrp.FreeXspr(m_GuildLargeScrollBaseSpr);
	clGrp.FreeXspr(m_sprTown_button_medium);
	clGrp.FreeXspr(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_sprScrollBar);

	if (m_BookDataSiegeWarfare.m_pMemberList)
	{
		delete []m_BookDataSiegeWarfare.m_pMemberList;
		m_BookDataSiegeWarfare.m_pMemberList = NULL;
	}

	// 점령한 마을 정보(목록 및 투자%) 초기화
	if (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList)
	{
		delete []m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList;

		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList = NULL;
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount = 0;
	}

	if(m_pFriendList)	
	{
/*		if( m_pFriendList->m_pFriendEditBox )
		{
			m_pFriendList->m_pFriendEditBox->Free();
			delete m_pFriendList->m_pFriendEditBox;
			m_pFriendList->m_pFriendEditBox = NULL;
		}
*/
		Handler_OnlineEditControlEx::KillControl( m_pFriendList->m_hOecFriend);
		
		if( m_pFriendList->m_pRect )
		{
			delete [] m_pFriendList->m_pRect;
			m_pFriendList->m_pRect	=	NULL;
		}
		
		delete m_pFriendList;
		m_pFriendList = NULL;
	}

	if( m_pOnlineCharStatus )
	{
		m_pOnlineCharStatus->Free();
		delete m_pOnlineCharStatus;
		m_pOnlineCharStatus = NULL;
	}

	if( m_Investment.m_pRect)
	{
		delete [] m_Investment.m_pRect;
		m_Investment.m_pRect = NULL;
	}	
	if(m_Investment.m_pListRect)
	{
		delete [] m_Investment.m_pListRect;
		m_Investment.m_pListRect = NULL;
	}

	if(m_CheckSpr.Image)  {clGrp.FreeXspr(m_CheckSpr);  m_CheckSpr.Image  = NULL;}

	if(pMsgBox)
	{		
		pMsgBox->Initialize();
		delete pMsgBox;
		pMsgBox = NULL;
	}

	if(m_QuestBSpr.Image)
	{
		clGrp.FreeXspr(m_QuestBSpr);
		m_QuestBSpr.Image = NULL;
	}
	
	if(m_pOnInvestmentInfo)
	{
		delete[] m_pOnInvestmentInfo;
		m_pOnInvestmentInfo = NULL;
	}
	
	if(m_Investment.m_pAuditVillageInfo)
	{
		delete m_Investment.m_pAuditVillageInfo;
		m_Investment.m_pAuditVillageInfo = NULL;
	}

	if(m_pOnGuildMemberInfo)
	{
		delete m_pOnGuildMemberInfo;
		m_pOnGuildMemberInfo = NULL;
	}



	if(m_pOnGuildMemberDAI)
	{
		delete[] m_pOnGuildMemberDAI;
		m_pOnGuildMemberDAI	= NULL;
	}

	if(m_pOnGuildMemberDHS)
	{
		delete[] m_pOnGuildMemberDHS;
		m_pOnGuildMemberDHS = NULL;
	}

	if(m_pOnGuildMemberHNS)
	{
		delete[] m_pOnGuildMemberHNS;
		m_pOnGuildMemberHNS = NULL;
	}

	if(m_pOnGuildMemberHND)
	{
		delete[] m_pOnGuildMemberHND;
		m_pOnGuildMemberHND = NULL;
	}

	if(m_pOnGuildMemberInfo)
	{
		delete[] m_pOnGuildMemberInfo;
		m_pOnGuildMemberInfo = NULL;
	}

	// actdoll (2004/06/29 16:41) : 누수발견! 해제해준다.
	if( m_Total.m_pAuditOwnInfo )
	{
		delete m_Total.m_pAuditOwnInfo;
		m_Total.m_pAuditOwnInfo = NULL;
	}
}




//______________________________________________________________________________________________________________
// 활성화 한다.
VOID	OnlineTradeBook::SetAction( BOOL bFlag, SI32 siFlag )
{
	if( m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )		return;

	m_bActive = bFlag;

	if(bFlag)
	{
		SetRect(&m_rtBook, m_Total.m_siX, m_Total.m_siY, m_Total.m_siX + m_BookMyMoneySpr.Header.Xsize, m_Total.m_siY + m_BookMyMoneySpr.Header.Ysize);

		if( m_siNoFieldHandle != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
		m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);

//		m_pOnlineCharStatus->SetX(m_siX);
		m_pOnlineCharStatus->SetActive(TRUE);

		if( m_pMyOnlineWorld->pOnlineMiniMap->IsActive() && m_pMyOnlineWorld->pOnlineMiniMap->ISFullMode() == TRUE)			
			m_pMyOnlineWorld->pOnlineMiniMap->SetActive( FALSE );

		m_siStatus = siFlag;
	}
	else
	{
		// 서버에서 메세지를 받을 경우 플래그는 참
		m_Total.m_bFlag					=	FALSE;
//		m_Property.m_bFlag				=	FALSE;

		// 각 해당 버튼의 상태
		m_Total.m_MyMoneyStatus			=	ON_BOOK_TOTAL_MONEY_INIT;	
		m_pFriendList->m_FriendStatus	=	ON_BOOK_FRIEND_INIT;
		m_QuestDataStatus               =   ON_BOOK_QUEST_INIT;
		m_Investment.m_InvestmentStatus	=   ON_BOOK_INVESTMENT_INIT;
		m_Guild.m_siStatus				=	ON_BOOK_GUILD_INIT;

		if( m_siNoFieldHandle != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
		m_siNoFieldHandle = -1;

		m_pOnlineCharStatus->SetActive( FALSE );

		if(!m_pMyOnlineWorld->pOnlineMessenger->IsAction())
			m_pMyOnlineWorld->SetShortKey( FALSE );
		
//		m_siScrollX = GetTickCount();
//		m_bScrollRight = FALSE;
//		m_bScrollLeft = TRUE;

//		m_pFriendList->m_bIsCheckAll = FALSE;
		for(SI08 i = 0; i < MAX_FRIEND_NUM; i++)
			m_pFriendList->m_bCheck[i] = FALSE;
		
		if(m_pOnInvestmentInfo)
		{
			delete[] m_pOnInvestmentInfo;
			m_pOnInvestmentInfo = NULL;
		}
		
/*		if(m_Investment.m_pstCapital)
		{
			delete [] m_Investment.m_pstCapital;
			m_Investment.m_pstCapital = NULL;
		}
*/
	}
}


//______________________________________________________________________________________________________________
// 현재 활성화 인가?
BOOL	OnlineTradeBook::IsAction()
{
	return m_bActive;
}

//______________________________________________________________________________________________________________
// 버튼을 체크한다.
VOID	OnlineTradeBook::CheckButton()
{
	if(m_siStatus == ON_BOOK_INVESTMENT && m_Investment.m_InvestmentStatus != ON_BOOK_INVESTMENT_POLL)	return;


	if((m_siStatus == ON_BOOK_FRIEND) || (m_siStatus ==	ON_BOOK_GUILD) || (m_siStatus == ON_BOOK_PARTY))
	{
		if( m_BookFriendTab.Process(m_bMouseDown) == TRUE)
		{
			m_siStatus				=	ON_BOOK_FRIEND;
			m_pFriendList->m_FriendStatus			=   ON_BOOK_FRIEND_INIT;
			
		}
		else if(m_BookGuildTab.Process(m_bMouseDown) == TRUE)
		{
			m_siStatus				=	ON_BOOK_GUILD;
			m_Guild.m_siStatus		=	ON_BOOK_GUILD_INIT;
		}
		else if(m_BookPartyTab.Process(m_bMouseDown) == TRUE)
		{
			m_siStatus				=	ON_BOOK_PARTY;
			m_Party.m_siStatus		=	ON_BOOK_PARTY_INIT;
		}
	}
	// 공성전 대방(상단 병력, 보급품 관련) 인터페이스 추가
	else if ((ON_BOOK_TOTAL_MONEY == m_siStatus) || (ON_BOOK_SIEGEWARFARE == m_siStatus))
	{
		if (m_btnBookESTATE.Process(m_bMouseDown))
		{
			m_siStatus				=	ON_BOOK_TOTAL_MONEY;
			m_Total.m_MyMoneyStatus	=	ON_BOOK_TOTAL_MONEY_INIT;
		}
		else if (m_btnBookSiegeWarfare.Process(m_bMouseDown))
		{
			if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			{
				m_siStatus				=	ON_BOOK_SIEGEWARFARE;
				m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_INIT;
			}
		}
	}
	
	if ((m_BttnMoneyExit.Process( m_bMouseDown ) == TRUE ) && (ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY != m_BookDataSiegeWarfare.m_siStatus))
	{
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );		
		m_siNoFieldHandle = -1;

		if(!m_pMyOnlineWorld->pOnlineMessenger->IsAction())
			m_pMyOnlineWorld->SetShortKey( FALSE );
		IpD.LeftPressSwitch = FALSE;
		SetAction( FALSE );
	}
}


//______________________________________________________________________________________________________________
// 나의 총재산 액숀~
VOID	OnlineTradeBook::ActionMyMoney()
{
	POINT		pt;
	SI16		siItemID;
	pt.x	=	IpD.Mouse_X;
	pt.y	=	IpD.Mouse_Y;

	switch( m_Total.m_MyMoneyStatus )
	{
	case ON_BOOK_TOTAL_MONEY_INIT:
		{
			if( m_Total.m_bFlag == FALSE )
			{	
				m_Total.m_bFlag	=	TRUE;
			}
			SetRect( &m_rtScrollBar, m_siX + 222, m_siY + 105, m_siX + 222, m_siY + 330 );

			m_pMyOnlineWorld->pOnlineClient->SendAuditOwnListMsg();

			if( m_Total.m_pAuditOwnInfo )
			{
				delete m_Total.m_pAuditOwnInfo;
				m_Total.m_pAuditOwnInfo	=	NULL;
			}

			m_Total.m_MyMoneyStatus	=	ON_BOOK_TOTAL_MONEY_POLL;
			m_siScroll				=	0;
			m_siScrollPos			=	0;
			m_Total.m_siItemNum		=	0;
			m_bScroll				=	FALSE;
			
			m_pMyOnlineWorld->pOnlineClient->SendReqAuditVillageInfo();

			if(m_pOnInvestmentInfo)
			{
				delete[] m_pOnInvestmentInfo;
				m_pOnInvestmentInfo = NULL;
			}

			// 대방이라면 공성전 병력, 보급품 관련 인터페이스 메뉴 버튼 활성화
			if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			{
				m_btnBookSiegeWarfare.SetAction(TRUE);
			}
			else
			{
				m_btnBookSiegeWarfare.SetAction(FALSE);
			}
		}
		break;
	case ON_BOOK_TOTAL_MONEY_RESET:
		{
			SI32			i, ix;			
			MyItemData		MyItemData;
			m_Total.m_siItemNum		=	0;

			ZeroMemory( m_Total.m_Item, sizeof( TOTALITEM ) * ( ON_MYDATA_MAX_ITEM_NUM * ON_MAX_FOLLOWER_NUM ) );

			for( i=0 ; i<ON_MAX_FOLLOWER_NUM; i++ ) 
			{
				if( m_pMyOnlineWorld->pMyData->IsValidFollower( i ) == TRUE )
				{
					for( ix=0 ; ix<ON_MYDATA_MAX_ITEM_NUM ; ix++ )
					{
						if( m_pMyOnlineWorld->pMyData->GetItem(i, ix, &MyItemData) == TRUE )
						{
							siItemID = ItemSearch(m_Total.m_siItemNum,MyItemData.uiID);
							if(siItemID == -1)
							{
								m_Total.m_Item[m_Total.m_siItemNum].m_siItemNum	=	MyItemData.uiQuantity;  //아이템의 갯수.
								m_Total.m_Item[m_Total.m_siItemNum].m_uiItemID	=	MyItemData.uiID;        //아이디.
								m_Total.m_siItemNum++;                                                      //아이템의 총합.
							}
							else
							{
								m_Total.m_Item[siItemID].m_siItemNum += MyItemData.uiQuantity;
							}
						}
					}
				}
			}
			m_Total.m_MyMoneyStatus	=	ON_BOOK_TOTAL_MONEY_POLL;
		}
		break;

	case ON_BOOK_TOTAL_MONEY_POLL:
		{
			// 대방이라면 공성전 병력, 보급품 관련 인터페이스 메뉴 버튼 활성화
			if (m_pMyOnlineWorld->pMyData->GetGuildClass() == m_pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			{
				m_btnBookSiegeWarfare.SetAction(TRUE);
			}
			else
			{
				m_btnBookSiegeWarfare.SetAction(FALSE);
			}

			if( m_bMouseDown )
			{
				SI16	siHeight;

				if(m_Investment.m_InvestmentCount > ON_BOOK_INVESTMENT_LINE)
				{
					
					//자신의 투자내역
					if(m_BttnMoneyScrollUp.Process(m_bMouseDown))
					{
						
						m_siScroll--;
						if(m_siScroll < 0)	m_siScroll++;
						else
						{
							siHeight		=	(SI16)((m_rtVillageScroll.bottom - m_rtVillageScroll.top) - 9);
							m_siScrollPos	=	siHeight * ((float)m_siScroll / (float)(m_Investment.m_InvestmentCount - ON_BOOK_INVESTMENT_LINE));
						}
						m_Investment.m_siOverMouse = m_siSelect - m_siScroll;
					}
					else if(m_BttnMoneyScrollDn.Process(m_bMouseDown))
					{
						m_siScroll++;
						if(m_siScroll > (m_Investment.m_InvestmentCount - ON_BOOK_INVESTMENT_LINE))	
							m_siScroll--;
						else
						{
							siHeight		=	(SI16)((m_rtVillageScroll.bottom - m_rtVillageScroll.top) - 9);
							m_siScrollPos	=	siHeight * ((float)m_siScroll / (float)(m_Investment.m_InvestmentCount - ON_BOOK_INVESTMENT_LINE));
						}
						m_Investment.m_siOverMouse = m_siSelect - m_siScroll;
					}
				}
				if(m_Investment.m_InvestmentUserCount > ON_BOOK_INVESTMENT_LIST_LINE/2)
				{
					//마을의 투자내역
					if(m_BttnInvestScrollUp.Process(m_bMouseDown))
					{
						m_Investment.m_siScroll--;
						if(m_Investment.m_siScroll < 0)	
							m_Investment.m_siScroll++;
						else
						{
							siHeight		=	(SI16)((m_rtInvestmentScroll.bottom - m_rtInvestmentScroll.top) - 9);
							m_Investment.m_siScrollPos = siHeight * ((float)m_Investment.m_siScroll / (float)(m_Investment.m_InvestmentUserCount - ON_BOOK_INVESTMENT_LIST_LINE/2));
						}
					}
					else if(m_BttnInvestScrollDn.Process(m_bMouseDown))
					{
						m_Investment.m_siScroll++;
						if(m_Investment.m_siScroll > (m_Investment.m_InvestmentUserCount - ON_BOOK_INVESTMENT_LIST_LINE/2))	
							m_Investment.m_siScroll--;
						else
						{
							siHeight		=	(SI16)((m_rtInvestmentScroll.bottom - m_rtInvestmentScroll.top) - 9);
							m_Investment.m_siScrollPos	= siHeight * ((float)m_Investment.m_siScroll / (float)(m_Investment.m_InvestmentUserCount - ON_BOOK_INVESTMENT_LIST_LINE/2));
						}
					}
				}
			}
			//유저의 투자마을 리스트를 선택했을때.
			if( m_bMouseDown )
			{
				for(int i=0 ; i< min(ON_BOOK_INVESTMENT_LINE, m_Investment.m_InvestmentCount); i++ )
				{
					if( PtInRect( &m_Investment.m_pRect[i], m_ptMouse ) == TRUE )
					{
						m_siSelect =  i + m_siScroll;
						m_Investment.m_siOverMouse = m_siSelect - m_siScroll;
						m_Total.m_MyMoneyStatus = ON_BOOK_TOTAL_MONEY_SELECT;
						m_pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_GetList(m_Investment.m_pAuditVillageInfo[m_siSelect].uiVillageCode);
						m_Investment.m_siScroll = 0;
						m_Investment.m_siScrollPos = 0;
						break;
					}
				}
			}
		}
		break;
	case ON_BOOK_TOTAL_MONEY_SELECT:

		break;

	case ON_BOOK_TOTAL_MONEY_EXIT:
		if(m_pOnInvestmentInfo)
		{
			delete[] m_pOnInvestmentInfo;
			m_pOnInvestmentInfo = NULL;
		}
		break;
	}
}

SI16	OnlineTradeBook::ItemSearch(SI16 total,UI16 id)
{
	SI16 i = 0;
	for(i = 0 ; i < total ; i++)
	{
		if(m_Total.m_Item[i].m_uiItemID == id)
			return i;
	}

	return -1;
	
}
//______________________________________________________________________________________________________________
// 나의 총재산 드로우
VOID	OnlineTradeBook::DrawMyMoney( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC				hDC;
	CHAR			szTemp[512];
	CHAR			szTemp2[128];
	VillageHeader	*pHeader;
	char			szMoney[50];
	COLORREF		rgb;		
	MONEY			mnTemp;
	


	if( clGrp.LockSurface( pSurface ) == TRUE )
	{

		clGrp.PutSpriteT( m_Total.m_siX,m_Total.m_siY,m_BookMyMoneySpr.Header.Xsize,m_BookMyMoneySpr.Header.Ysize,m_BookMyMoneySpr.Image);
		clGrp.PutSpriteT( m_Total.m_siX + 217, m_Total.m_siY + 166, m_MoneySmallScrollBaseSpr.Header.Xsize, m_MoneySmallScrollBaseSpr.Header.Ysize,m_MoneySmallScrollBaseSpr.Image);
		clGrp.PutSpriteT( m_Total.m_siX + 217, m_Total.m_siY + 262, m_MoneyLargeScrollBaseSpr.Header.Xsize, m_MoneyLargeScrollBaseSpr.Header.Ysize,m_MoneyLargeScrollBaseSpr.Image);

		if(m_Investment.m_InvestmentCount > ON_BOOK_INVESTMENT_LINE)
			clGrp.PutSpriteT( m_Total.m_siX + 221, m_rtVillageScroll.top  + m_siScrollPos, m_BookScrollBarSpr.Header.Xsize, m_BookScrollBarSpr.Header.Ysize, m_BookScrollBarSpr.Image );

		if(ON_BOOK_INVESTMENT_LIST_LINE/2 < m_Investment.m_InvestmentUserCount)
			clGrp.PutSpriteT( m_Total.m_siX + 221, m_rtInvestmentScroll.top  + m_Investment.m_siScrollPos, m_BookScrollBarSpr.Header.Xsize, m_BookScrollBarSpr.Header.Ysize, m_BookScrollBarSpr.Image );

		m_BttnMoneyScrollUp.Put(&m_BookScrollBttnUpSpr,0,1,0,BUTTON_PUT_NOMOVE);
		m_BttnMoneyScrollDn.Put(&m_BookScrollBttnDnSpr,0,1,0,BUTTON_PUT_NOMOVE);

		m_BttnInvestScrollUp.Put(&m_BookScrollBttnUpSpr,0,1,0,BUTTON_PUT_NOMOVE);
		m_BttnInvestScrollDn.Put(&m_BookScrollBttnDnSpr,0,1,0,BUTTON_PUT_NOMOVE);


		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// robypark 2004/10/1 10:54
		// 파라미터 m_pMyOnlineWorld->pOnlineText->Get(8000552)로 수정. 친구목록 텍스트 인덱스 값을 보유재산 텍스트 인덱스 값으로 수정
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_Total.m_siX + 60, m_Total.m_siY + 8, 118, 12,m_pMyOnlineWorld->pOnlineText->Get(8000552), RGB( 216, 201, 183 ));

		m_BttnMoneyExit.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 총 재산
		MONEY totalmoney = m_pMyOnlineWorld->pMyData->GetMoney() + m_pMyOnlineWorld->pMyData->GetSaving();

		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// 현재 소지금
		MakeMoneyText(m_pMyOnlineWorld->pMyData->GetMoney(), szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 20, m_Total.m_siY + 61,60,16, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NOWMONEY), RGB( 216, 201, 183 ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 86, m_Total.m_siY + 61,133,16, szTemp, RGB( 216, 201, 183 ) );

		// 전장 저축 금액
		MakeMoneyText(m_pMyOnlineWorld->pMyData->GetSaving(), szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 20, m_Total.m_siY + 85,60,16, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANKMONEY), RGB( 216, 201, 183 ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 86, m_Total.m_siY + 85,133,16, szTemp, RGB( 216, 201, 183 ) );

		// 보유 생산 시설
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 20, m_Total.m_siY + 112,80,12, m_pMyOnlineWorld->pOnlineText->Get(8000556), RGB( 216, 201, 183 ) );

		// 보유 생산 시설 종류에 따른 출력
		if(m_Total.m_pAuditOwnInfo)
		{
//			m_Total.m_pAuditOwnInfo->uiVillageCode;
//			m_Total.m_pAuditOwnInfo->PlantID;

			switch(m_Total.m_pAuditOwnInfo->PlantID)
			{
			case ON_VIL_ST_KIND_FARM:
				sprintf(szTemp2, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FARM ));
				break;
			case ON_VIL_ST_KIND_PASTURE:
				sprintf(szTemp2,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PASTURE));
				break;
			case ON_VIL_ST_KIND_FACTORY:
				sprintf(szTemp2,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FACTORY ));
				break;
			case ON_VIL_ST_KIND_ARMOR:
				sprintf(szTemp2,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ARMOR ));
				break;
			case ON_VIL_ST_KIND_WEAPON:
				sprintf(szTemp2,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WEAPON ));
				break;
			}

			pHeader	=	m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_Total.m_pAuditOwnInfo->uiVillageCode);
			
			if(pHeader)
			{
				sprintf(szTemp,"%s - %s",m_pMyOnlineWorld->pOnlineText->Get( pHeader->siVillageName ),szTemp2);
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 18, m_Total.m_siY + 129,201,14 , szTemp, RGB( 216, 201, 183 ) );
			}
		}

		// 투자한 마을
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 20, m_Total.m_siY + 152,80,12, m_pMyOnlineWorld->pOnlineText->Get(8000553), RGB( 216, 201, 183 ) );

		// 투자자 정보
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 20, m_Total.m_siY + 248,80,12, m_pMyOnlineWorld->pOnlineText->Get(8000554), RGB( 216, 201, 183 ) );

		// 총 투자금
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 10, m_Total.m_siY + 370,75,14, m_pMyOnlineWorld->pOnlineText->Get(8000555), RGB( 216, 201, 183 ) );

		// 투자한 마을 리스트 출력
		for(int i = 0; i < min( ON_BOOK_INVESTMENT_LINE, m_Investment.m_InvestmentCount ); i++)
		{
			if( i == m_Investment.m_siOverMouse )				rgb = RGB( 255, 255, 255 );
			else												rgb = RGB( 216, 201, 183 );

			pHeader	=	m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_Investment.m_pAuditVillageInfo[i + m_siScroll].uiVillageCode);

			if( pHeader )
			{
				sprintf(szTemp,"%d",m_Investment.m_InvestmentCount);
				// 투자한 마을 수
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 105, m_Total.m_siY + 151,39,14, szTemp,RGB( 216, 201, 183 ));

				MakeMoneyText(m_Investment.m_pAuditVillageInfo[i + m_siScroll].mnInvestedMoney,szMoney);
				sprintf(szTemp,"%s%s",szMoney, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTINFO_MONEY));

				// 투자한 마을 이름 출력
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_Total.m_siX + 18, m_Total.m_siY + 170 + ( i * 14 ), m_pMyOnlineWorld->pOnlineText->Get( pHeader->siVillageName ), rgb );

				// 투자한 금액 출력
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC,m_Total.m_siX + 218, m_Total.m_siY + 170 + ( i * 14 ),szTemp,rgb);
			}

		}

		// 마을 투자자 리스트 보기 활성화인 경우		
		if(m_pOnInvestmentInfo)
		{
			//rgb = RGB( 255, 255, 255 );
			//pHeader	=	m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_Investment.m_pAuditVillageInfo[m_siSelect].uiVillageCode);
			//m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 105, m_Total.m_siY + 248 ,30,12, m_pMyOnlineWorld->pOnlineText->Get( pHeader->siVillageName ), RGB( 216, 201, 183 ));


			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			mnTemp = 0;
			for(int i = 0 ; i <  m_Investment.m_InvestmentUserCount; i++)
				mnTemp += m_pOnInvestmentInfo[i].m_InvestmentMoney;

			// 총 투자금액 출력
			MakeMoneyText(mnTemp,szMoney);
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 86 , m_Total.m_siY +370,141,14, szMoney, rgb );

			//////////////////////////////////////////////////////////////////////////////////////////////////////////

			// 마을에 투자한 사람들 목록 출력
			for(i = 0 ; i < min(ON_BOOK_INVESTMENT_LIST_LINE/2,m_Investment.m_InvestmentUserCount) ; i++)
			{
				rgb = RGB( 255, 255, 255 );

				// 투자자 이름 출력
				sprintf(szTemp,"%2d. %s",i + m_Investment.m_siScroll + 1,(char*)m_pOnInvestmentInfo[i + m_Investment.m_siScroll].m_InvestmentName);
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_Total.m_siX + 18, m_Total.m_siY + 268 + ( i * 30 ), szTemp, rgb );

				rgb = RGB( 216, 201, 183 );

				// 투자일 출력
				sprintf((char*)szTemp,"%4d.%2d.%2d", 
					(m_pOnInvestmentInfo[i + m_Investment.m_siScroll].m_InvestmentUpdateTime/1000000),
					((m_pOnInvestmentInfo[i + m_Investment.m_siScroll].m_InvestmentUpdateTime/10000)%100),
					((m_pOnInvestmentInfo[i + m_Investment.m_siScroll].m_InvestmentUpdateTime/100)%100));
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_Total.m_siX + 133, m_Total.m_siY + 268 + ( i * 30 ), szTemp, rgb );
				
				// 투자금 출력
				MakeMoneyText(m_pOnInvestmentInfo[i + m_Investment.m_siScroll].m_InvestmentMoney,szMoney);

				sprintf((char*)szTemp,"%s%s(%d%%)", szMoney, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTINFO_MONEY), (int)m_pOnInvestmentInfo[i + m_Investment.m_siScroll].m_InvestmentPercent);

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_Total.m_siX + 219, m_Total.m_siY + 268 + ( i * 30 ) + 14,szTemp,rgb);
			}
		}

		pSurface->ReleaseDC( hDC );
	}

	// 보유재산, 공성 탭 그리기
	DrawTab(pSurface);
}


VOID	OnlineTradeBook::ActionFriend()
{
//	if(m_bScrollRight)	return;

	SI16	i;
	SI16	temp;
	FILE	*fp;

	OnlineEditControlEx	*pOecEx;

	pOecEx	= Handler_OnlineEditControlEx::GetControl( m_pFriendList->m_hOecFriend );

	switch( m_pFriendList->m_FriendStatus )
	{
	case ON_BOOK_FRIEND_INIT:
		{
			sprintf( m_pFriendList->m_szPath, "Online\\Friend\\BadFriendList%d", g_stConnectServerInfo.m_WorldID);

			if( m_pFriendList->m_bFlag == FALSE )
			{
				// 친구목록을 텍스트로부터 읽는다.
				BOOL	bDelete = FALSE;
				fp = fopen( m_pFriendList->m_szPath, "rb" );
		
				if(fp)
				{
					SI08	siFriendNum;
					fread( &siFriendNum, sizeof(siFriendNum), 1, fp );
					for(i = 0; i < siFriendNum; i++)
					{
						CHAR	szFriendName[ON_ID_LENGTH+1];
						ZeroMemory(szFriendName, ON_ID_LENGTH+1);
						fread( szFriendName, ON_ID_LENGTH, 1, fp );
						if((strcmp(szFriendName, "지우지마") == 0) || (strcmp(szFriendName, "홍미금이") == 0))
						{
							bDelete = TRUE;
							break;
						}
						m_pMyOnlineWorld->pMyData->m_BadFriendList.AddFriend(m_pMyOnlineWorld->pMyData->GetMyCharName(), 
							szFriendName, FALSE);
						m_pMyOnlineWorld->pMyData->m_BadFriendList.SetDenial(szFriendName,TRUE);
					}
					fclose( fp );
				}

				if(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends() > 30 || bDelete)
				{
					DeleteFile(m_pFriendList->m_szPath);
					m_pMyOnlineWorld->pMyData->m_BadFriendList.DeleteAllFriend();
				}

				m_pFriendList->m_bFlag = TRUE;
			}

			ZeroMemory(m_pFriendList->m_szFriendName, sizeof(m_pFriendList->m_szFriendName));
	
			m_pFriendList->m_FriendStatus	= ON_BOOK_FRIEND_POLL;

		//	m_pFriendList->m_pFriendEditBox->EnableWindow( TRUE );
			
		//	if( m_siNoFieldHandle != -1 )		
		//		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );
			
		//	m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);
		}
		break;
	case ON_BOOK_FRIEND_POLL:
		{
			pMsgBox->Process();

			if(IpD.RightPressSwitch)
			{
				for(i = 0 ; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends() ; i++)
				{
					if(i < 15)
					{
						temp = i * 6;
						m_pFriendList->m_bCheck[i] = FALSE;	
						if( (m_ptMouse.x > m_pFriendList->m_siX + 25) && ( m_ptMouse.x < m_pFriendList->m_siX + 115)
							&& (m_ptMouse.y > m_pFriendList->m_siY + 84 + temp + (i * 12)) && (m_ptMouse.y < m_pFriendList->m_siY + 96 + temp + (i * 12)) )
						{
							m_pFriendList->m_bCheck[i] = TRUE;
							m_ptMenu.x = m_ptMouse.x;
							m_ptMenu.y = m_ptMouse.y;
						}
					}
					else
					{
						temp = (i - 15) * 6;
						if( (m_ptMouse.x > m_pFriendList->m_siX + 134) && ( m_ptMouse.x < m_pFriendList->m_siX + 224)
							&& (m_ptMouse.y > m_pFriendList->m_siY + 84 + temp + ((i - 15) * 12)) && (m_ptMouse.y < m_pFriendList->m_siY + 96 + temp + ( (i -15) * 12)) )
						{
							m_pFriendList->m_bCheck[i] = TRUE;
							m_ptMenu.x = m_ptMouse.x;
							m_ptMenu.y = m_ptMouse.y;
						}
						
					}
				}
				IpD.RightPressSwitch = FALSE;
			}
				

			if( PtInRect( m_pFriendList->m_pRect, m_ptMouse ) == TRUE && m_bMouseDown)
			{
//				m_pFriendList->m_pFriendEditBox->EnableWindow( TRUE );
//				m_pFriendList->m_pFriendEditBox->SetFocus();
				Handler_OnlineEditControlEx::SetFocus( m_pFriendList->m_hOecFriend);
				m_pMyOnlineWorld->SetShortKey( TRUE );
			}

			// 친구이름 텍스트창
			//추가.추가추가 나중에 테스트 후..-_-;
			//if(m_pFriendList->m_pFriendEditBox->IsEnableWindow())
			//	m_pFriendList->m_pFriendEditBox->GetString(m_pFriendList->m_szFriendName);
			
			BOOL bOk = FALSE;
			//if(m_pFriendList->m_pFriendEditBox->IsDoneMessage() == TRUE)
			if(pOecEx->IsPushEnter())
			{
				bOk = TRUE;
			//	m_pFriendList->m_pFriendEditBox->InitDoneMessage();
			}

			// 추가 버튼
			if(m_BttnAdd.Process(m_bMouseDown) == TRUE || bOk)
			{
				//m_pFriendList->m_pFriendEditBox->GetString(m_pFriendList->m_szFriendName);
				strcpy(m_pFriendList->m_szFriendName,pOecEx->GetString());

				if(m_pFriendList->m_szFriendName[0] != 0)
				{
					if(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends() > 29)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_OVER));
						m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
					}
					else
					{
						// 공백있나 검사
						// actdoll (2004/01/29 12:57) : 함수명칭 변경 [ CheckID() -> IsValidCharacterNameString() ]
						// actdoll (2004/06/18 16:29) : 운영팀 요청으로 친구 등록시 필터링을 안하게 처리
//						if( !m_pMyOnlineWorld->IsValidCharacterNameString( m_pFriendList->m_szFriendName ) )
//						{
//							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_ADDFRIEND), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATIDFAIL));
//							break;
//						}

						// 같은 이름이 없나 검사
						if(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(m_pFriendList->m_szFriendName) != NULL)
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_ADDFRIEND), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_ADDFRIEND_ISUSER));
							break;
						}
						m_pMyOnlineWorld->pOnlineClient->SendReqAddFriend(m_pFriendList->m_szFriendName);
						m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_ADD;
					}
					//m_pFriendList->m_pFriendEditBox->EnableWindow( FALSE );
					m_pMyOnlineWorld->SetShortKey( FALSE );					
				}
				pOecEx->Clear();
			}	

			if (m_BttnRefusalOk.Process(m_bMouseDown) == TRUE )
			{
				for(i = 0 ; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends() ; i++)
				{
					if(m_pFriendList->m_bCheck[i] == TRUE) 
					{
						strcpy(m_pFriendList->m_szFriendName,m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID);
						m_pFriendList->m_bCheck[i] = FALSE;
					}
				}
				// 수신거부 캐릭 텍스트에 저장.
				m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_REFUSAL;

				// 같은 이름이 없나 검사
				if(m_pMyOnlineWorld->pMyData->m_BadFriendList.GetFriend(m_pFriendList->m_szFriendName) != NULL)
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK_ISUSER));
					m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
				}
			}


			if( m_BttnRefusalCancel.Process(m_bMouseDown) == TRUE )
			{
				for(i = 0 ; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends() ; i++)
				{
					if(m_pFriendList->m_bCheck[i] == TRUE) 
					{
						strcpy(m_pFriendList->m_szFriendName,m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID);
						m_pFriendList->m_bCheck[i] = FALSE;
					}
				}
				//if(m_pFriendList->m_szFriendName[0] != 0)
				//{
					// 같은 이름이 있나 검사
				if(m_pMyOnlineWorld->pMyData->m_BadFriendList.GetFriend(m_pFriendList->m_szFriendName) != NULL)
					m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_REFUSAL_CANCEL;
				else
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK_CANCEL), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK_NOTFINDUSER1));
					m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
				}
				//}
			}



			if( m_BttnDelete.Process(m_bMouseDown) == TRUE )
			{
				UI16 count = 0;
				CHAR name[MAX_FRIEND_NUM * ON_ID_LENGTH];
				ZeroMemory(name, sizeof(name));
				for(UI16 i = 0; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends(); i++)
				{
					if(m_pFriendList->m_bCheck[i])
					{
						strncpy(&name[count*ON_ID_LENGTH], m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID, ON_ID_LENGTH);
						count++;
					}
				}
				if(count == m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends())
				{
					m_pMyOnlineWorld->pOnlineClient->SendReqDeleteAllFriend();
					//m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_DELETE_ALL;
				}
				else
				{
					m_pMyOnlineWorld->pOnlineClient->SendReqDeleteFriend(name, count);
					//m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_DELETE;
				}

			}

			if(m_BttnMessenger.Process(m_bMouseDown) == TRUE )
			{
				// 레벨, 신용등급 5이하는 귓말을 할수없다.
				if(m_pMyOnlineWorld->pMyData->GetTradeGrade() <= 5)
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8000518));
				else
					m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_MESSENGER;
			}
		}
		break;

	case ON_BOOK_FRIEND_ADD:
		{
		}
		break;

	case ON_BOOK_FRIEND_DELETE:
		{
		}
		break;
	case ON_BOOK_FRIEND_REFUSAL:
		{
			// 파일에 수신거부 이름 추가
			CreateDirectory( "Online\\Friend", NULL );

			fp = fopen( m_pFriendList->m_szPath, "r+b" );
			if(fp)
			{
				// 현재 수신거부자 수 기록
				SI08	siFriendNum = m_pMyOnlineWorld->pMyData->m_BadFriendList.GetNumFriends() + 1;
				fseek(fp, 0, SEEK_SET);
				fwrite( &siFriendNum, sizeof( SI08 ), 1, fp );
				// 수신거부 이름 기록
				fseek(fp, 0, SEEK_END);
				fwrite( m_pFriendList->m_szFriendName, ON_ID_LENGTH, 1, fp );
				
				fclose( fp );
			}
			else
			{
				fp = fopen( m_pFriendList->m_szPath, "wb" );
				// 현재 수신거부자 수 기록
				SI08	siFriendNum = m_pMyOnlineWorld->pMyData->m_BadFriendList.GetNumFriends() + 1;
				fwrite( &siFriendNum, sizeof( SI08 ), 1, fp );
				// 수신거부 이름 기록
				fwrite( m_pFriendList->m_szFriendName, ON_ID_LENGTH, 1, fp );
				
				fclose( fp );
			}
			
			// 리스트에 수신거부 이름추가
			m_pMyOnlineWorld->pMyData->m_BadFriendList.AddFriend(m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pFriendList->m_szFriendName, FALSE);
			m_pMyOnlineWorld->pMyData->m_BadFriendList.SetDenial(m_pFriendList->m_szFriendName,TRUE);

			//m_pFriendList->m_pFriendEditBox->SetString("");
			pOecEx->Clear();
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCKOK));
			m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
		}
		break;

	case ON_BOOK_FRIEND_REFUSAL_CANCEL:
		{
			// 수신거부 리스트에서 삭제
			if(m_pMyOnlineWorld->pMyData->m_BadFriendList.DeleteFriend(m_pFriendList->m_szFriendName))
			{
				m_pMyOnlineWorld->pMyData->m_BadFriendList.SetDenial(m_pFriendList->m_szFriendName,FALSE);
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK_RELEASE));
			}
			else	// 리스트에서 같은 이름을 못찾는다면
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CHATTING_SENDBLOCK_NOTFINDUSER));
				break;
			}

			// 파일에 다시 써준다.
			fp = fopen( m_pFriendList->m_szPath, "wb" );

			// 현재 친구 수, 이름 기록
			SI08	siFriendNum = m_pMyOnlineWorld->pMyData->m_BadFriendList.GetNumFriends();
			fwrite( &siFriendNum, sizeof( SI08 ), 1, fp );

			for(i = 0; i < m_pMyOnlineWorld->pMyData->m_BadFriendList.GetNumFriends(); i++)
			{
				fwrite( m_pMyOnlineWorld->pMyData->m_BadFriendList.GetFriend(i)->szID, ON_ID_LENGTH, 1, fp );
			}
			
			fclose( fp );

			// 체크버튼 초기화
//			m_pFriendList->m_bIsCheckAll = FALSE;
			for(i = 0; i < MAX_FRIEND_NUM; i++)
				m_pFriendList->m_bCheck[i] = FALSE;

			//m_pFriendList->m_pFriendEditBox->SetString("");
			pOecEx->Clear();
			m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
		}
		break;

	case ON_BOOK_FRIEND_MESSENGER:
		{
			for(i = 0; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends(); i++)
			{
				if(m_pFriendList->m_bCheck[i])
				{
					m_pMyOnlineWorld->pOnlineMessenger->SetAction(TRUE, m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID);
					m_pFriendList->m_bCheck[i] = FALSE;
					break;	
				}
			}
			m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
		}
		break;
	}
}

VOID	OnlineTradeBook::ActionParty()
{
	switch(m_Party.m_siStatus)	
	{
	case ON_BOOK_PARTY_INIT:
		break;
	case ON_BOOK_PARTY_POLL:
	{
		if(m_BPartyOutBasebttn.Process(m_bMouseDown))
		{
			m_pMyOnlineWorld->pOnlineClient->SendRequestQuitParty();
		}
	}
		break;
	case ON_BOOK_PARTY_EXIT:
		break;
	}
}

VOID	OnlineTradeBook::DrawParty( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hDC;
	SI16	i;
	SI16	j	=	0;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_Party.m_siX,m_Party.m_siY,m_BookPartySpr.Header.Xsize,m_BookPartySpr.Header.Ysize,m_BookPartySpr.Image);
	
		for(i = 0 ; i < 6 ; i++)
			clGrp.PutSpriteT(m_Party.m_siX + 47, m_Party.m_siY + 95 + (i * 31), m_NameBaseSpr.Header.Xsize, m_NameBaseSpr.Header.Ysize,m_NameBaseSpr.Image);

		m_BPartyOutBasebttn.Put( &m_BtnPartyBaseSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );


		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_Party.m_siX + 60,m_Party.m_siY + 8, 118, 12,m_pMyOnlineWorld->pOnlineText->Get(8000530), RGB( 216, 201, 183 ));

		m_BttnMoneyExit.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		m_BPartyOutBasebttn.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		if(m_pMyOnlineWorld->pOnlineParty->GetPartyNum() > 0 && m_pMyOnlineWorld->pOnlineParty->m_stPartyInfo != NULL)
		{
			for(i = 0 ; i < m_pMyOnlineWorld->pOnlineParty->GetPartyNum();i++)
			{
				if(i != m_pMyOnlineWorld->pOnlineParty->GetMyCharNum())
				{
					if(m_pMyOnlineWorld->pOnlineParty->m_stPartyInfo[i].bIsPartyLeader == TRUE)
					{
						 m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_Party.m_siX + m_ptCharNamePos[j].x ,m_Party.m_siY + m_ptCharNamePos[j].y + 1,
							130, 15,m_pMyOnlineWorld->pOnlineParty->m_stPartyInfo[i].Name,RGB(255,0, 0));
					}
					else
					{
						 m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_Party.m_siX + m_ptCharNamePos[j].x ,m_Party.m_siY + m_ptCharNamePos[j].y + 1,
							130, 15,m_pMyOnlineWorld->pOnlineParty->m_stPartyInfo[i].Name,RGB(216,201, 183));				
					}
					 j++;
				}
			}
		}


		pSurface->ReleaseDC( hDC );
	}
	DrawTab(pSurface);
}	

VOID	OnlineTradeBook::ActionGuild()
{
	switch(m_Guild.m_siStatus)	
	{
	case ON_BOOK_GUILD_INIT:
		m_pMyOnlineWorld->pOnlineClient->SendAuditGuildMemberList();
		m_Guild.m_siStatus = ON_BOOK_GUILD_POLL;
		break;
	case ON_BOOK_GUILD_POLL:
		{

			if(m_BttnGuildScrollUp1.Process(m_bMouseDown))
			{
				if(m_siGuildScroll1Count <= 0)
					m_siGuildScroll1Count = 0;
				if(m_siGuildScroll1Count > 0)
					m_siGuildScroll1Count--;
			}
			if(m_BttnGuildScrollDn1.Process(m_bMouseDown))
			{
				float  TempHNSCount = m_siHNSCount;
				if(m_siGuildScroll1Count < (SI16) (((TempHNSCount / 2) + 0.5) - 5))
				{
					m_siGuildScroll1Count++;
				}
			}


			if(m_BttnGuildScrollUp2.Process(m_bMouseDown))
			{
				if(m_siGuildScroll2Count <= 0)
					m_siGuildScroll2Count = 0;
				if(m_siGuildScroll2Count > 0)
					m_siGuildScroll2Count--;
			}
			if(m_BttnGuildScrollDn2.Process(m_bMouseDown))
			{
				float  TempHNDCount = m_siHNDCount;
				if(m_siGuildScroll2Count < (SI16) (((TempHNDCount / 2) + 0.5) - 5))
				{
					m_siGuildScroll2Count++;
				}
			}
		}
		break;
	case ON_BOOK_GUILD_EXIT:
		break;
	}
}

VOID	OnlineTradeBook::DrawGuild( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;
	CHAR	szTemp[80];
	SI16	i;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_Guild.m_siX,m_Guild.m_siY,m_BookGuildSpr.Header.Xsize,m_BookGuildSpr.Header.Ysize,m_BookGuildSpr.Image);

		//clGrp.PutSpriteT(m_Guild.m_siX + 217,m_Guild.m_siY + 172 ,m_GuildSmallScrollBaseSpr.Header.Xsize,m_GuildSmallScrollBaseSpr.Header.Ysize,m_GuildSmallScrollBaseSpr.Image);

		//clGrp.PutSpriteT(m_Guild.m_siX + 217,m_Guild.m_siY + 233 ,m_GuildSmallScrollBaseSpr.Header.Xsize,m_GuildSmallScrollBaseSpr.Header.Ysize,m_GuildSmallScrollBaseSpr.Image);

		clGrp.PutSpriteT(m_Guild.m_siX + 217,m_Guild.m_siY + 179 ,m_GuildLargeScrollBaseSpr.Header.Xsize,m_GuildLargeScrollBaseSpr.Header.Ysize,m_GuildLargeScrollBaseSpr.Image);

		clGrp.PutSpriteT(m_Guild.m_siX + 217,m_Guild.m_siY + 294 ,m_GuildLargeScrollBaseSpr.Header.Xsize,m_GuildLargeScrollBaseSpr.Header.Ysize,m_GuildLargeScrollBaseSpr.Image);

		m_BttnGuildScrollUp1.Put(&m_BookScrollBttnUpSpr,0,1,0,BUTTON_PUT_NOMOVE);
		m_BttnGuildScrollDn1.Put(&m_BookScrollBttnDnSpr,0,1,0,BUTTON_PUT_NOMOVE);

		m_BttnGuildScrollUp2.Put(&m_BookScrollBttnUpSpr,0,1,0,BUTTON_PUT_NOMOVE);
		m_BttnGuildScrollDn2.Put(&m_BookScrollBttnDnSpr,0,1,0,BUTTON_PUT_NOMOVE);

		m_BttnGuildScrollUp3.Put(&m_BookScrollBttnUpSpr,0,1,0,BUTTON_PUT_NOMOVE);
		m_BttnGuildScrollDn3.Put(&m_BookScrollBttnDnSpr,0,1,0,BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}
	if( pSurface->GetDC( &hdc ) == DD_OK )
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 60 ,m_Guild.m_siY + 8, 118, 12,m_pMyOnlineWorld->pOnlineText->Get(8000530), RGB( 216, 201, 183 ));
		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 62,60,16,
			m_pMyOnlineWorld->pOnlineText->Get(8000533),RGB(216,201, 183));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 85,60,12,
			m_pMyOnlineWorld->pOnlineText->Get(8000534),RGB(216,201, 183));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 109,60,16,
			m_pMyOnlineWorld->pOnlineText->Get(8000535),RGB(216,201, 183));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 132,60,16,
			m_pMyOnlineWorld->pOnlineText->Get(8000536),RGB(216,201, 183));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 165,60,12,
			m_pMyOnlineWorld->pOnlineText->Get(8000537),RGB(216,201, 183));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 280,60,12,
			m_pMyOnlineWorld->pOnlineText->Get(8000538),RGB(216,201, 183));

//		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 20, m_Guild.m_siY + 280,60,12,
//			"사환",RGB(216,201, 183));

		if(m_pOnGuildMemberInfo)
		{
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 86, m_Guild.m_siY + 62,133,16,
				m_pMyOnlineWorld->pMyData->GetGuildName(),RGB(216,201,183));

			sprintf(szTemp,"%s %d%s/%s %d%s",m_pMyOnlineWorld->pOnlineText->Get(8000224),m_siLoginGuildMember,m_pMyOnlineWorld->pOnlineText->Get(8000540),
				m_pMyOnlineWorld->pOnlineText->Get(8000539),m_siNowGuildMember,m_pMyOnlineWorld->pOnlineText->Get(8000540));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 86, m_Guild.m_siY + 85,133,12,
				szTemp,RGB(216,201,183));
			
			if(m_pOnGuildMemberDAI != NULL)
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 86, m_Guild.m_siY + 109,133,16,
					m_pOnGuildMemberDAI[0].szName,RGB(216,201,183));
			}

			if(m_pOnGuildMemberDHS != NULL)
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 86, m_Guild.m_siY + 132,133,16,
					m_pOnGuildMemberDHS[0].szName,RGB(216,201,183));
			}
	
			
			if(m_pOnGuildMemberHNS != NULL)
			{
				for(i = 0 ; i < 5 ;i++)
				{
					if(m_siHNSCount > (i * 2) + (m_siGuildScroll1Count *2))
					{
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 18, m_Guild.m_siY + 184 + (i * 19),90,12,
						m_pOnGuildMemberHNS[(i * 2) + (m_siGuildScroll1Count *2)].szName,RGB(216,201,183));
					}
					if(m_siHNSCount > (i * 2) + 1 + (m_siGuildScroll1Count *2))
					{
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 125, m_Guild.m_siY + 184 + (i * 19),90,12,
							m_pOnGuildMemberHNS[(i * 2) + 1 + (m_siGuildScroll1Count *2)].szName,RGB(216,201,183));
					}
				}
			}

			if(m_pOnGuildMemberHND != NULL)
			{
				for(i = 0 ; i < 5;i++)
				{
					if(m_siHNDCount > (i * 2) + (m_siGuildScroll2Count *2))
					{
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 18 , m_Guild.m_siY + 299 + (i * 19),90,12,
							m_pOnGuildMemberHND[(i * 2) + (m_siGuildScroll2Count *2)].szName,RGB(216,201,183));
					}

					if(m_siHNDCount > (i * 2) + 1 + (m_siGuildScroll2Count *2))
					{
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_Guild.m_siX + 125, m_Guild.m_siY + 299 + (i * 19),90,12,
							m_pOnGuildMemberHND[(i * 2)+ 1 + (m_siGuildScroll2Count *2)].szName,RGB(216,201,183));
					}
				}
			}
		}

		m_BttnMoneyExit.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hdc );
	}
	DrawTab(pSurface);
}	

VOID	OnlineTradeBook::DrawTab(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hdc;
	
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		if((m_siStatus == ON_BOOK_FRIEND) || (m_siStatus ==	ON_BOOK_GUILD) || (m_siStatus == ON_BOOK_PARTY))
		{
			m_BookFriendTab.Put(&m_BookTab, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BookGuildTab.Put(&m_BookTab, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BookPartyTab.Put(&m_BookTab, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}
		else if ((ON_BOOK_TOTAL_MONEY == m_siStatus) || (ON_BOOK_SIEGEWARFARE == m_siStatus))
		{
			m_btnBookESTATE.Put(&m_BookTab, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_btnBookSiegeWarfare.Put(&m_BookTab, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hdc ) == DD_OK )
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		if((m_siStatus == ON_BOOK_FRIEND) || (m_siStatus ==	ON_BOOK_GUILD) || (m_siStatus == ON_BOOK_PARTY))
		{
			m_BookFriendTab.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BookGuildTab.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BookPartyTab.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		else if ((ON_BOOK_TOTAL_MONEY == m_siStatus) || (ON_BOOK_SIEGEWARFARE == m_siStatus))
		{
			m_btnBookESTATE.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_btnBookSiegeWarfare.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}

		pSurface->ReleaseDC( hdc );
	}

}

VOID	OnlineTradeBook::DrawFriend( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;
	SI08	i;
	char	szTemp[64];
	SI16 temp = 0;
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl(m_pFriendList->m_hOecFriend);

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT(m_pFriendList->m_siX,m_pFriendList->m_siY,m_BookFriendSpr.Header.Xsize,m_BookFriendSpr.Header.Ysize,m_BookFriendSpr.Image);

		m_BttnAdd.Put(&m_FriendBttnSpr,0,1,1,BUTTON_PUT_NOMOVE);
		
		

		// 로그인 중인지 아닌지 표시
		for(i = 0; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends(); i++)
		{
			if( i < 15 )
			{
				temp = i * 6;
				if(m_pMyOnlineWorld->pMyData->m_BadFriendList.IsDenial(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID))
					clGrp.PutSpriteT(m_pFriendList->m_siX + 12 ,m_pFriendList->m_siY + 84 + temp + (i * 12) , m_FirendRefusalSpr.Header.Xsize,m_FirendRefusalSpr.Header.Ysize,m_FirendRefusalSpr.Image);
			}
			else
			{
				temp = (i - 15) * 6;
				if(m_pMyOnlineWorld->pMyData->m_BadFriendList.IsDenial(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID))
					clGrp.PutSpriteT(m_pFriendList->m_siX + 120 ,m_pFriendList->m_siY + 84 + temp + ((i  - 15)* 12) , m_FirendRefusalSpr.Header.Xsize,m_FirendRefusalSpr.Header.Ysize,m_FirendRefusalSpr.Image);
			}
		}

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hdc ) == DD_OK )
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_pFriendList->m_siX + 60 ,m_pFriendList->m_siY + 8, 118, 12,m_pMyOnlineWorld->pOnlineText->Get(8000530), RGB( 216, 201, 183 ));

		m_BttnMoneyExit.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_pFriendList->m_siX + 9,m_pFriendList->m_siY + 61,219,13.,m_pMyOnlineWorld->pOnlineText->Get(8000531), RGB( 216, 201, 183 ));
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
		for(i = 0; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends(); i++)
		{
			
			if( i < 15 )
			{
				temp = i * 6;
				if(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->bLogin)
				{
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pFriendList->m_siX + 25, m_pFriendList->m_siY + 84 + temp + (i * 12),m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID, RGB( 0, 255, 0 ));
				}
				else
				{
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pFriendList->m_siX + 25, m_pFriendList->m_siY + 84 + temp + (i * 12),m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID, RGB( 250, 250, 250 ));
				}
			}
			else
			{
				temp = (i - 15) * 6;
				if(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->bLogin)
				{
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pFriendList->m_siX + 134, m_pFriendList->m_siY + 84 + temp + ((i  - 15)* 12),m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID, RGB( 0, 255, 0 ));
				}
				else
				{
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, m_pFriendList->m_siX + 134, m_pFriendList->m_siY + 84 + temp + ((i  - 15)* 12), m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID, RGB( 250, 250, 250 ));
				}
			}
		}

		sprintf((char*)szTemp, m_pFriendList->m_szFriendName);
		//if( m_pFriendList->m_pFriendEditBox->IsFocus() == TRUE )
		if(Handler_OnlineEditControlEx::IsFocus(m_pFriendList->m_hOecFriend))
		{
			//m_pFriendList->m_pFriendEditBox->DrawText(hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), m_pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), RGB(216, 201, 183), m_pFriendList->m_siX + 38, m_pFriendList->m_siY + 366, NULL, TRUE);
			pOecEx->Draw(hdc);
		}
		else 
		{
			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_pFriendList->m_siX + 38, m_pFriendList->m_siY + 366, szTemp, RGB( 216, 201, 183 ));
		}

		m_BttnAdd.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hdc );
	}

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		for( i = 0; i < MAX_FRIEND_NUM; i++)
		{
			if(m_pFriendList->m_bCheck[i])
			{
				if(m_pMyOnlineWorld->pMyData->m_BadFriendList.IsDenial(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID))
				{
					m_BttnRefusalCancel.SetAction(TRUE);
					m_BttnRefusalOk.SetAction(FALSE);
					m_BttnMessenger.SetX(m_ptMenu.x);
					m_BttnMessenger.SetY(m_ptMenu.y);
					m_BttnMessenger.Put(&m_BttnFriendMenu,0,1,1,BUTTON_PUT_NOMOVE);
					m_BttnRefusalCancel.SetX(m_ptMenu.x);
					m_BttnRefusalCancel.SetY(m_ptMenu.y + m_BttnFriendMenu.Header.Ysize);
					m_BttnRefusalCancel.Put(&m_BttnFriendMenu,0,1,1,BUTTON_PUT_NOMOVE);
					m_BttnDelete.SetX(m_ptMenu.x);
					m_BttnDelete.SetY(m_ptMenu.y + (m_BttnFriendMenu.Header.Ysize * 2));
					m_BttnDelete.Put(&m_BttnFriendMenu,0,1,1,BUTTON_PUT_NOMOVE);
				}
				else
				{
					m_BttnRefusalCancel.SetAction(FALSE);
					m_BttnRefusalOk.SetAction(TRUE);
					m_BttnMessenger.SetX(m_ptMenu.x);
					m_BttnMessenger.SetY(m_ptMenu.y);
					m_BttnMessenger.Put(&m_BttnFriendMenu,0,1,1,BUTTON_PUT_NOMOVE);
					m_BttnRefusalOk.SetX(m_ptMenu.x);
					m_BttnRefusalOk.SetY(m_ptMenu.y + m_BttnFriendMenu.Header.Ysize);
					m_BttnRefusalOk.Put(&m_BttnFriendMenu,0,1,1,BUTTON_PUT_NOMOVE);
					m_BttnDelete.SetX(m_ptMenu.x);
					m_BttnDelete.SetY(m_ptMenu.y + (m_BttnFriendMenu.Header.Ysize * 2));
					m_BttnDelete.Put(&m_BttnFriendMenu,0,1,1,BUTTON_PUT_NOMOVE);
				}				
			}
		}
		clGrp.UnlockSurface( pSurface );
	}
	if( pSurface->GetDC( &hdc ) == DD_OK )
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		for( i = 0; i < MAX_FRIEND_NUM; i++)
		{
			if(m_pFriendList->m_bCheck[i])
			{
				if(m_pMyOnlineWorld->pMyData->m_BadFriendList.IsDenial(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID))
				{
					m_BttnMessenger.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_BttnRefusalCancel.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_BttnDelete.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				}
				else
				{
					m_BttnMessenger.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_BttnRefusalOk.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_BttnDelete.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				}
			}
		}

		pSurface->ReleaseDC( hdc );
	}

	DrawTab(pSurface);
	pMsgBox->Draw(pSurface);
}


VOID	OnlineTradeBook::BonusUp( BOOL bFlag )
{
	m_pOnlineCharStatus->RecvMsg_GrownUp( bFlag );
}

// 현재 어느 창이 열려 있는지 알아 온다.
SI16	OnlineTradeBook::GetStatus()
{
	return m_siStatus;
}

VOID	OnlineTradeBook::SetStatus( SI16 siFlag )
{

	SetRect(&m_rtBook, m_Total.m_siX, m_Total.m_siY, m_Total.m_siX + m_BookMyMoneySpr.Header.Xsize, m_Total.m_siY + m_BookMyMoneySpr.Header.Ysize);

	switch( siFlag )
	{
	case ON_BOOK_TOTAL_MONEY:
		if( m_siNoFieldHandle != -1 )		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );		
		m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);
		m_siStatus			=	ON_BOOK_TOTAL_MONEY;
		m_Total.m_MyMoneyStatus	=	ON_BOOK_TOTAL_MONEY_INIT;
		m_pOnlineCharStatus->SetActive( FALSE );
		break;

	case ON_BOOK_STATUS:
		if( m_siNoFieldHandle != -1 )		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );		
		m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);
		m_siStatus			=	ON_BOOK_STATUS;
		break;

	case ON_BOOK_FRIEND:
		if( m_siNoFieldHandle != -1 )		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );		
		m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);
		m_siStatus			=	ON_BOOK_FRIEND;
		m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_INIT;
		break;
	case ON_BOOK_QUEST:
		if( m_siNoFieldHandle != -1 )		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldHandle );		
		m_siNoFieldHandle	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_rtBook);
		m_siStatus			=	ON_BOOK_QUEST;
		break;

	}
	
}

VOID	OnlineTradeBook::SetSelect( UI08 uiSlot )
{
	m_pOnlineCharStatus->SetSelect( uiSlot );
}

BOOL	OnlineTradeBook::DeleteFriendName()
{
	for(int i = 0; i < m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends(); i++)
	{
		if(m_pFriendList->m_bCheck[i])
		{
			m_pMyOnlineWorld->pMyData->m_GoodFriendList.DeleteFriend(m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetFriend(i)->szID);

			for(int j = i+1; j < MAX_FRIEND_NUM; j++)
				m_pFriendList->m_bCheck[j-1] = m_pFriendList->m_bCheck[j];
			return TRUE;
		}
	}
	return FALSE;
}

VOID	OnlineTradeBook::KillScroll()
{
	m_bActive		= FALSE;
}

void OnlineTradeBook::AuditTotal(BYTE *pMsg)
{
	OnResAuditTotalMsg		*pOnResAuditTotalMsg = (OnResAuditTotalMsg*)pMsg;

	POINT		pt;
	pt.x	=	IpD.Mouse_X;
	pt.y	=	IpD.Mouse_Y;


	if( pOnResAuditTotalMsg )
	{
		if( pOnResAuditTotalMsg->dwReponse == ON_RET_AUDIT_OK )
		{
			OnResAuditTotalOkMsg	*pOnResAuditTotalOkMsg;

			pOnResAuditTotalOkMsg	=	(OnResAuditTotalOkMsg*)pOnResAuditTotalMsg;				
			m_Total.m_uiTotalUser	=	pOnResAuditTotalOkMsg->uiTotalUser;
			m_Total.m_uiProperty	=	pOnResAuditTotalOkMsg->siProperty;
			m_Total.m_uiBankMoney	=	pOnResAuditTotalOkMsg->siBankMoney;
			m_Total.m_siLoanMoney	=	pOnResAuditTotalOkMsg->siLoanMoney;
		}
		else	return;
	}
}

void OnlineTradeBook::AuditOwn(BYTE *pMsg)
{
	OnResAuditOwnListMsg *pOnResAuditOwnListMsg	=	(OnResAuditOwnListMsg*)pMsg;
	
	if(pOnResAuditOwnListMsg)
	{
		if( pOnResAuditOwnListMsg->dwResponse == ON_RET_AUDIT_OK )
		{	
			OnResAuditOwnListOkMsg		*pOnResAuditOwnListOkMsg;
			OnAuditOwnInfo				*pOnAuditOwnInfo;
			
			pOnResAuditOwnListOkMsg	=	(OnResAuditOwnListOkMsg*)pOnResAuditOwnListMsg;				
			pOnAuditOwnInfo			=	(OnAuditOwnInfo*)(pOnResAuditOwnListOkMsg + 1 );				
			
			if( m_Total.m_pAuditOwnInfo )
			{
				delete m_Total.m_pAuditOwnInfo;
				m_Total.m_pAuditOwnInfo = NULL;
			}
			m_Total.m_pAuditOwnInfo	=	new OnAuditOwnInfo;
			memcpy(m_Total.m_pAuditOwnInfo, pOnAuditOwnInfo, sizeof( OnAuditOwnInfo ));

			OnlineVillageParser *lpOnlineVillageParser = m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser();
			if( !lpOnlineVillageParser )		return;
			VillageHeader *lpVillageHeader = lpOnlineVillageParser->GetVillageHeaderCode(m_Total.m_pAuditOwnInfo->uiVillageCode);
			if( !lpVillageHeader )				return;

			switch( lpVillageHeader->siStructureKinds[8])
			{
			case ON_VIL_ST_KIND_FARM:
				m_Total.m_pAuditOwnInfo->PlantID = ON_VIL_ST_KIND_FARM;
				break;
			case ON_VIL_ST_KIND_PASTURE:
				m_Total.m_pAuditOwnInfo->PlantID = ON_VIL_ST_KIND_PASTURE;
				break;
			case ON_VIL_ST_KIND_FACTORY:
				m_Total.m_pAuditOwnInfo->PlantID = ON_VIL_ST_KIND_FACTORY;
				break;
			case ON_VIL_ST_KIND_ARMOR:
				m_Total.m_pAuditOwnInfo->PlantID = ON_VIL_ST_KIND_ARMOR;
				break;
			case ON_VIL_ST_KIND_WEAPON:
				m_Total.m_pAuditOwnInfo->PlantID = ON_VIL_ST_KIND_WEAPON;
				break;
			}

			// 내정보에 설정
			m_pMyOnlineWorld->pMyData->uiPropertyVillageCode	= m_Total.m_pAuditOwnInfo->uiVillageCode;
			m_pMyOnlineWorld->pMyData->uiPlantID				= m_Total.m_pAuditOwnInfo->PlantID;
		}
	}
}

void OnlineTradeBook::GuildInfo(BYTE *pMsg)
{

	SI16	i;

	SI16 siDAI = 0;
	SI16 siDHS = 0;
	SI16 siHNS = 0;
	SI16 siHND = 0;


	OnRespAuditGuildMemberInfoMsg* lpOnResAuditGuildMemberInfoMsg = (OnRespAuditGuildMemberInfoMsg *)pMsg;
		
	m_siSHNExceptMember  = lpOnResAuditGuildMemberInfoMsg->siSHNExceptMemberCount;     //사환이상.

	if(m_siSHNExceptMember > 0)
	{
		m_siLoginGuildMember = lpOnResAuditGuildMemberInfoMsg->siLoginedGuildMemberCount;  //로그인
		m_siNowGuildMember   = lpOnResAuditGuildMemberInfoMsg->siNowGuildMemberCount;	   //총원

		strAuditGuildMemberInfo *pAuditGuildMemberInfo = (strAuditGuildMemberInfo*)((BYTE*)lpOnResAuditGuildMemberInfoMsg+sizeof(OnRespAuditGuildMemberInfoMsg));

		if(m_pOnGuildMemberInfo)
		{
			delete[] m_pOnGuildMemberInfo;
			m_pOnGuildMemberInfo = NULL;
		}

		m_pOnGuildMemberInfo = new strAuditGuildMemberInfo[m_siSHNExceptMember];

		memcpy(m_pOnGuildMemberInfo,pAuditGuildMemberInfo,sizeof(strAuditGuildMemberInfo)*m_siSHNExceptMember);
	}
	else if(m_siSHNExceptMember == -2)
	{
		if(m_pOnGuildMemberInfo)
		{
			delete[] m_pOnGuildMemberInfo;
			m_pOnGuildMemberInfo = NULL;
		}
	}

	if((m_pOnGuildMemberInfo != NULL)&& (lpOnResAuditGuildMemberInfoMsg->siSHNExceptMemberCount > 0))
	{
		//각 상단의 대방 대행수 행수 행동대장의 수를 얻어온다.-_-;
		for(i = 0; i < m_siSHNExceptMember;i++)
		{
			//대방
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetDAICode())
			{
				m_siDAICount++;
			}
			//대행수
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetDHSCode())
			{
				m_siDHSCount++;
			}
			//행수
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetHNSCode())
			{
				m_siHNSCount++;
			}
			//행동대장.
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetHNDCode())
			{
				m_siHNDCount++;
			}
		}
		
		if(m_pOnGuildMemberDAI)
		{
			delete[] m_pOnGuildMemberDAI;
			m_pOnGuildMemberDAI	= NULL;
		}

		if(m_pOnGuildMemberDHS)
		{
			delete[] m_pOnGuildMemberDHS;
			m_pOnGuildMemberDHS = NULL;
		}

		if(m_pOnGuildMemberHNS)
		{
			delete[] m_pOnGuildMemberHNS;
			m_pOnGuildMemberHNS = NULL;
		}

		if(m_pOnGuildMemberHND)
		{
			delete[] m_pOnGuildMemberHND;
			m_pOnGuildMemberHND = NULL;
		}

	
		if(m_siDAICount > 0)
			m_pOnGuildMemberDAI = new strAuditGuildMemberInfo[m_siDAICount];
		if(m_siDHSCount > 0)
			m_pOnGuildMemberDHS = new strAuditGuildMemberInfo[m_siDHSCount];
		if(m_siHNSCount > 0)
			m_pOnGuildMemberHNS = new strAuditGuildMemberInfo[m_siHNSCount];
		if(m_siHNDCount > 0)
			m_pOnGuildMemberHND = new strAuditGuildMemberInfo[m_siHNDCount];

		for(i = 0; i < m_siSHNExceptMember;i++)
		{
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetDAICode())
			{
				memcpy(&m_pOnGuildMemberDAI[siDAI],&m_pOnGuildMemberInfo[i],sizeof(strAuditGuildMemberInfo));
				siDAI++;
			}
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetDHSCode())
			{
				memcpy(&m_pOnGuildMemberDHS[siDHS],&m_pOnGuildMemberInfo[i],sizeof(strAuditGuildMemberInfo));
				siDHS++;
			}
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetHNDCode())
			{
				memcpy(&m_pOnGuildMemberHND[siHND],&m_pOnGuildMemberInfo[i],sizeof(strAuditGuildMemberInfo));
				siHND++;
			}
			if(m_pOnGuildMemberInfo[i].siClassInGuild == m_pMyOnlineWorld->m_cltIGuildClass.GetHNSCode())
			{
				memcpy(&m_pOnGuildMemberHNS[siHNS],&m_pOnGuildMemberInfo[i],sizeof(strAuditGuildMemberInfo));
				siHNS++;
			}
		}
	}
}


void OnlineTradeBook::FriendAddOne(BYTE *pMsg)
{
	OnResAuditFriendAddOne*	lpOnResAuditFriendAddOne = (OnResAuditFriendAddOne*)pMsg;
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl(m_pFriendList->m_hOecFriend);

	switch(lpOnResAuditFriendAddOne->dwResponse)
	{
	case FRIEND_OK:
		// 리스트에 친구이름 추가
		m_pMyOnlineWorld->pMyData->m_GoodFriendList.AddFriend(m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pFriendList->m_szFriendName, lpOnResAuditFriendAddOne->bLogin);
		break;
	case FRIEND_ALREADY_EXIST:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_EXIST));
		break;
	case FRIEND_CANNOT_ADD_MYSELF:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_NOT_REGISTER_MYSELF));
		break;
	case FRIEND_INVALID_NAME:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_INVALID_ID));
		break;
	case FRIEND_FULL:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_ADD_EXCESS));
		break;
	default:
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_ADD_FAIL));
		break;
	}

	//m_pFriendList->m_pFriendEditBox->SetString("");
	//m_pFriendList->m_pFriendEditBox->EnableWindow(FALSE);
	pOecEx->Clear();
	m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;

}

void OnlineTradeBook::FriendRemove(BYTE *pMsg)
{
	OnResAuditFriendRemoveSome*	lpOnResAuditFriendRemoveSome = (OnResAuditFriendRemoveSome*)pMsg;
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl(m_pFriendList->m_hOecFriend);
	

	if(lpOnResAuditFriendRemoveSome->dwResponse == ON_RET_AUDIT_OK)
	{
		// 친구 리스트에서 삭제
		SI08	siSize = m_pMyOnlineWorld->pMyData->m_GoodFriendList.GetNumFriends();
		for(SI32 i = 0; i < siSize; i++)
		{
			if(!DeleteFriendName())		return;
		} 
		
		// 체크버튼 초기화
//		m_pFriendList->m_bIsCheckAll = FALSE;
		for(i = 0; i < MAX_FRIEND_NUM; i++)
			m_pFriendList->m_bCheck[i] = FALSE;
	}
	else if(lpOnResAuditFriendRemoveSome->dwResponse == ON_RET_AUDIT_NO)
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_DELETE_FAIL));

	pOecEx->Clear();
//	m_pFriendList->m_pFriendEditBox->SetString("");
	m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
}

void OnlineTradeBook::FriendRemoveAll(BYTE *pMsg)
{
	OnlineEditControlEx	*pOecEx;
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_pFriendList->m_hOecFriend );
	OnResAuditFriendRemoveAll*	lpOnResAuditFriendRemoveAll = (OnResAuditFriendRemoveAll*)pMsg;
	

	if(lpOnResAuditFriendRemoveAll->dwResponse == ON_RET_AUDIT_OK)
		m_pMyOnlineWorld->pMyData->m_GoodFriendList.DeleteAllFriend();
	else if(lpOnResAuditFriendRemoveAll->dwResponse == ON_RET_AUDIT_NO)
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIEND_DELETE_FAIL));

	for(SI32 i = 0; i < MAX_FRIEND_NUM; i++)
		m_pFriendList->m_bCheck[i] = FALSE;
//	m_pFriendList->m_pFriendEditBox->SetString("");
	pOecEx->Clear();
	m_pFriendList->m_FriendStatus = ON_BOOK_FRIEND_POLL;
}

void OnlineTradeBook::VillageList(BYTE *pMsg)
{
	OnResAuditVillageList *pOnResAuditVillageList = (OnResAuditVillageList*)pMsg;

	if(pOnResAuditVillageList)
	{
		if(pOnResAuditVillageList->dwResponse == ON_RET_AUDIT_OK)
		{
			OnResAuditVillageListOK		*pOnResAuditVillageOkList;
			OnAuditVillageInfo			*pOnAuditVillageInfo;

			pOnResAuditVillageOkList	= (OnResAuditVillageListOK*)pOnResAuditVillageList;	
			
			m_Investment.m_InvestmentCount = (SI16)pOnResAuditVillageOkList->siVillageCount;

			pOnAuditVillageInfo         =   (OnAuditVillageInfo*)((BYTE*)pOnResAuditVillageOkList+sizeof(OnResAuditVillageListOK));

			if(m_Investment.m_pAuditVillageInfo)
			{
				delete m_Investment.m_pAuditVillageInfo;
				m_Investment.m_pAuditVillageInfo = NULL;
			}

			if(m_Investment.m_InvestmentCount > 0)
			{
				m_Investment.m_pAuditVillageInfo = new OnAuditVillageInfo[m_Investment.m_InvestmentCount];
				memcpy(m_Investment.m_pAuditVillageInfo, pOnAuditVillageInfo,sizeof(OnAuditVillageInfo)*m_Investment.m_InvestmentCount);
			}

		}
	}
}

void OnlineTradeBook::InvestmentList(BYTE *pMsg)
{
	OnResGovernmentGetInvestmentList *lpOnResGovernmentGetInvestmentList = (OnResGovernmentGetInvestmentList*)pMsg;

	if(lpOnResGovernmentGetInvestmentList)
	{
		if(lpOnResGovernmentGetInvestmentList->dwResponse == ON_RET_OK)
		{
			m_Investment.m_InvestmentUserCount = lpOnResGovernmentGetInvestmentList->m_InvestmentListCount;
			m_Investment.m_siTempScroll = m_Investment.m_InvestmentUserCount *2;

			OnInvestmentInfo *lpOnInvestmentInfo = (OnInvestmentInfo*)((BYTE*)lpOnResGovernmentGetInvestmentList+sizeof(OnResGovernmentGetInvestmentList));

			if(m_pOnInvestmentInfo)
			{
				delete[] m_pOnInvestmentInfo;
				m_pOnInvestmentInfo = NULL;
			}

			m_pOnInvestmentInfo = new OnInvestmentInfo[m_Investment.m_InvestmentUserCount];
			memcpy(m_pOnInvestmentInfo,lpOnInvestmentInfo,sizeof(OnInvestmentInfo)*m_Investment.m_InvestmentUserCount);
		}
		m_Total.m_MyMoneyStatus = ON_BOOK_TOTAL_MONEY_POLL;
	}

}

BOOL	OnlineTradeBook::Quest_SetBankBookCount(MONEY Count)
{
	m_QuestBankBookCount = Count;
	return TRUE;
}

// robypark 2004/12/24 13:34
// 대방 공성전 관련 인터페이스 사용 버튼 상태 갱신
VOID	OnlineTradeBook::UpdateButtonSiegeWarfare()
{
	// 점령한 마을 폭탄투자 관련 처리 가능 시간인 경우
	if (m_pMyOnlineWorld->IsInTimeAllInInvestment())
	{
		// 권한주기 버튼 활성화
		m_BookDataSiegeWarfare.m_btnGiveACompetency.SetAction(TRUE);
	}
	else
	{
		// 권한주기 버튼 비활성화
		m_BookDataSiegeWarfare.m_btnGiveACompetency.SetAction(FALSE);
	}

	/*
	// 소집권 아이템 만들 수 있는 시간인가?
	if (m_pMyOnlineWorld->IsInTimeRecruitArmyItem())
	{
		// 차출 버튼 활성화
		// 검병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.SetAction(TRUE);

		// 창병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.SetAction(TRUE);

		// 궁병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.SetAction(TRUE);

		// 기마병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.SetAction(TRUE);

		// 충차 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.SetAction(TRUE);

		// 발석거 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.SetAction(TRUE);

		// 보급수레 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.SetAction(TRUE);
	}
	else
	{
		// 차출 버튼 비활성화
		// 검병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.SetAction(FALSE);

		// 창병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.SetAction(FALSE);

		// 궁병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.SetAction(FALSE);

		// 기마병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.SetAction(FALSE);

		// 충차 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.SetAction(FALSE);

		// 발석거 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.SetAction(FALSE);

		// 보급수레 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.SetAction(FALSE);
	}
	*/
}

// 대방 공성전 관련 인터페이스 처리
VOID	OnlineTradeBook::ActionSiegeWarfare()
{
	switch (m_BookDataSiegeWarfare.m_siStatus)
	{
		case ON_BOOK_SIEGEWARFARE_INIT:	//	0
			{
				// 현재 상단이 모집한 공성 유닛들 수를 얻어온다.
				ZeroMemory(m_BookDataSiegeWarfare.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);
				m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
				m_pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
				// 현재 상단이 보유하고 있는 보급품 수 얻기
				m_BookDataSiegeWarfare.m_uiGuildSupplyGoodsAmount = 0;
				//m_pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
				// 현재 상단이 점령한 마을 목록을 얻어온다.
				m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage = FALSE;
				m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount = 0;
				m_pMyOnlineWorld->pOnlineClient->SendRequest_guild_villageattack_result();

				// 현재 상단원들의 목록을 얻어온다.
				m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx = 1;
				m_BookDataSiegeWarfare.m_siCurPage = 1;

				m_pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_BookDataSiegeWarfare.m_siCurPage, 0);
				m_BookDataSiegeWarfare.m_bDownloadedGuildMember = FALSE;
				m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;

				m_BookDataSiegeWarfare.m_siRecruitArmyKind	= FALSE;	// 소집하려는 캐릭터 Online Kind

				// 차출, 권한주기 버튼 상태 갱신
				UpdateButtonSiegeWarfare();
			}
			break;

		case ON_BOOK_SIEGEWARFARE_POLL:	//	1
			{
				// 마우스 처리
				POINT ptMouse = {IpD.Mouse_X, IpD.Mouse_Y};

				// 차출, 권한주기 버튼 상태 갱신
				UpdateButtonSiegeWarfare();

				/*
				// 소집권 아이템 만들 수 있는 시간인가?
				if (m_pMyOnlineWorld->IsInTimeRecruitArmyItem())
				{
					// 검병 소집 버튼
					if (m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN] != 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_SWORDMAN, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 창병 소집 버튼
					else if (m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN] > 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_SPEARMAN, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 궁병 소집 버튼
					else if (m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN] > 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_ARCHERMAN, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 기마병 소집 버튼
					else if (m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN] > 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_HORSEMAN, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 충차 소집 버튼
					else if (m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER] > 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_SIEGE_TOWER, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 발석거 소집 버튼
					else if (m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT] > 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_CATAPULT, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 보급수레 소집 버튼
					else if (m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.Process(m_bMouseDown))
					{
						// 공성 유닛 리스트 다운로드 하였으며, 소집할 수 있는 유닛(부대)가 존재한다.
						if ((m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount) && (m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON] > 0))
						{
							// 공성전 소집권 아이템 정보 헤더 얻기
							CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_SUPPLY_WAGON, FALSE));

							if (pItemHeader)
							{
								// 소집하려는 캐릭터의 Imperium.txt에서 읽어들인 아이템 정보 인덱스 설정
								m_BookDataSiegeWarfare.m_siRecruitArmyKind = pItemHeader->m_pImperium->m_siID;
							}
							else
								break;

							// 계산기 설정
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON]);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.GetX() + 20, m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.GetY() + 5);
							m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY;

							// 계산기 인터페이스 사용중에는 마우스 클릭 막기
							m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 699);

							//m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = FALSE;
						}
					}
					// 점령한 마을 목록 처리
					else if (PtInRect(&m_BookDataSiegeWarfare.m_rtOccupiedVillageListBox, ptMouse))
					{
						if (FALSE == m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage)
							break;

						if (m_bMouseDown)
						{
							// 스크롤 바가 활성화되었다면
							if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount > ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT)
							{
								SI16 siHeight = 0;

								// 스크롤 바 버튼 처리
								if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollUp.Process(m_bMouseDown))
								{
									m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll--;

									if (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll < 0)	
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll++;
									else
									{
										siHeight		=	(SI16)((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.bottom - m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.top) - 9);
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos = siHeight * ((float)m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll / (float)(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount - ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT));
									}
								}
								else if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollDown.Process(m_bMouseDown))
								{
									m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll++;
									if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll > (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount - ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT))
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll--;
									else
									{
										siHeight		=	(SI16)((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.bottom - m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.top) - 9);
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos	= siHeight * ((float)m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll / (float)(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount - ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT));
									}
								}
							}
							// 점령한 마을 클릭 처리
							for(int i=0 ; i< min(ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT, m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount); i++ )
							{
								if (m_ptMouse.x >= m_BookDataSiegeWarfare.m_siX + 14 && m_ptMouse.x < m_BookDataSiegeWarfare.m_siX+14+95 && m_ptMouse.y >= (m_BookDataSiegeWarfare.m_siY+256 + i * 30) && m_ptMouse.y < (m_BookDataSiegeWarfare.m_siY+256 + (i + 1) * 30))
								{
									m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx = i + m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll;
									break;
								}
							}
						}
					}
					// 상단원 목록 처리
					else if (PtInRect(&m_BookDataSiegeWarfare.m_rtGuildMemberListBox, ptMouse))
					{
						if (m_bMouseDown)
						{
							// 사용자가 페이지 변경할 경우
							if(ActionMemberList() != -1)
							{
								// 새 페이지에 해당되는 상단원 목록 얻기
								m_pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_BookDataSiegeWarfare.m_siCurPage, 0);
								m_BookDataSiegeWarfare.m_bDownloadedGuildMember = FALSE;
							}
						}
					}
					// 권한 주기 버튼 처리
					else if (m_BookDataSiegeWarfare.m_btnGiveACompetency.Process(m_bMouseDown))
					{
						if (FALSE == m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage)
							break;

						// 점령한 마을 폭탄투자 관련 처리 가능 시간인 경우
						if (m_pMyOnlineWorld->IsInTimeAllInInvestment())
						{
							// 점령한 마을이 있다. 상단원이 존재
							if ((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount)
								&& (m_BookDataSiegeWarfare.m_uiMemberNum))
							{
								// 점령한 마을 목록에서 아이템 선택, 권한을 주려는 캐릭터 선택
								if ((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siVillageCode)
									&& (m_BookDataSiegeWarfare.m_pMemberList[m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx].siAccount))
								{
									VillageHeader *lpVillageHeader = m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siVillageCode);

									if (lpVillageHeader)
									{
										// %s 상단원에게 %s 마을의 투자권한(%3d%%)를 주시겠습니까?
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", 
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GIVE_A_COMPETENCY), 
															m_BookDataSiegeWarfare.m_pMemberList[m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx].szName, m_pMyOnlineWorld->pOnlineText->Get( lpVillageHeader->siVillageName ), 
															m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siPercent);

										m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_GIVE_A_COMPETENCY;
									}
								}
							}
						}
					}
				}
				// 차출 가능한 시간 이외
				else
				*/
				{
					// 점령한 마을 목록 처리
					if (PtInRect(&m_BookDataSiegeWarfare.m_rtOccupiedVillageListBox, ptMouse))
					{
						if (FALSE == m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage)
							break;

						if (m_bMouseDown)
						{
							// 스크롤 바가 활성화되었다면
							if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount > ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT)
							{
								SI16 siHeight = 0;

								// 스크롤 바 버튼 처리
								if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollUp.Process(m_bMouseDown))
								{
									m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll--;

									if (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll < 0)	
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll++;
									else
									{
										siHeight		=	(SI16)((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.bottom - m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.top) - 30);
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos = siHeight * ((float)m_Investment.m_siScroll / (float)(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount - ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT));
									}
								}
								else if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollDown.Process(m_bMouseDown))
								{
									m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll++;
									if(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll > (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount - ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT))
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll--;
									else
									{
										siHeight		=	(SI16)((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.bottom - m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.top) - 30);
										m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos	= siHeight * ((float)m_Investment.m_siScroll / (float)(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount - ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT));
									}
								}
							}
							// 점령한 마을 클릭 처리
							for(int i=0 ; i< min(ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT, m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount); i++ )
							{
								if (m_ptMouse.x >= m_BookDataSiegeWarfare.m_siX + 14 && m_ptMouse.x < m_BookDataSiegeWarfare.m_siX+14+95 && m_ptMouse.y >= (m_BookDataSiegeWarfare.m_siY+256 + i * 30) && m_ptMouse.y < (m_BookDataSiegeWarfare.m_siY+256 + (i + 1) * 30))
								{
									m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx = i + m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll;
									break;
								}
							}
						}
					}
					// 상단원 목록 처리
					else if (PtInRect(&m_BookDataSiegeWarfare.m_rtGuildMemberListBox, ptMouse))
					{
						if (m_bMouseDown)
						{
							// 사용자가 페이지 변경할 경우
							if(ActionMemberList() != -1)
							{
								// 새 페이지에 해당되는 상단원 목록 얻기
								m_pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_BookDataSiegeWarfare.m_siCurPage, 0);
								m_BookDataSiegeWarfare.m_bDownloadedGuildMember = FALSE;
							}
						}
					}
					// 권한 주기 버튼 처리
					else if (m_BookDataSiegeWarfare.m_btnGiveACompetency.Process(m_bMouseDown))
					{
						if (FALSE == m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage)
							break;

						// 점령한 마을 폭탄투자 관련 처리 가능 시간인 경우
						if (m_pMyOnlineWorld->IsInTimeAllInInvestment())
						{
							// 점령한 마을이 있다. 상단원이 존재
							if ((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount)
								&& (m_BookDataSiegeWarfare.m_uiMemberNum))
							{
								// 점령한 마을 목록에서 아이템 선택, 권한을 주려는 캐릭터 선택
								if ((m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siVillageCode)
									&& (m_BookDataSiegeWarfare.m_pMemberList[m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx].siAccount))
								{
									VillageHeader *lpVillageHeader = m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siVillageCode);

									if (lpVillageHeader)
									{
										// %s 상단원에게 %s 마을의 투자권한(%3d%%)를 주시겠습니까?
										m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", 
																	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GIVE_A_COMPETENCY), 
																	m_BookDataSiegeWarfare.m_pMemberList[m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx].szName,
																	m_pMyOnlineWorld->pOnlineText->Get( lpVillageHeader->siVillageName ), m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siPercent);

										m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_GIVE_A_COMPETENCY;
									}
								}
							}
						}
					}
				}
			}
			break;

		case ON_BOOK_SIEGEWARFARE_WAIT:	//	2
			// 응답을 기다린다.
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			break;
		case ON_BOOK_SIEGEWARFARE_RECRUIT_ARMY:	// 4
			{
				// 계산기에서 확인 버튼이 눌려져, 유효한 값이 있는 경우
				if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && m_pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// 소집권 아이템 만들 수 있는 시간인가?
					if (m_pMyOnlineWorld->IsInTimeRecruitArmyItem())
					{
						m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_WAIT;

						// 공성 병력 소집 아이템 생성 요청 메시지 전달
						m_pMyOnlineWorld->pOnlineClient->SendDecreaseGuildUnit(m_BookDataSiegeWarfare.m_siRecruitArmyKind, m_pMyOnlineWorld->pOnlineNumPad->siCount);
					}
					else
						m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;

					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();

					// 마우스 클릭 메시지가 인터페이스 이외로 가지 못하게 했던것 원상태로
					if (-1 != m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc)
						m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc );
				}
				else if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && m_pMyOnlineWorld->pOnlineNumPad->siCount == 0)
				{
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;

					// 마우스 클릭 메시지가 인터페이스 이외로 가지 못하게 했던것 원상태로
					if (-1 != m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc)
						m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_BookDataSiegeWarfare.m_siNoFieldHandle_Calc );
				}
			}
			break;

		// 공성전 마을 투자 권한주기 메시지 박스 처리하기
		case ON_BOOK_SIEGEWARFARE_GIVE_A_COMPETENCY: //	5
			{
				// 권한주기 확인
				if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
				{
					// 점령한 마을 폭탄투자 관련 처리 가능 시간인 경우
					if (m_pMyOnlineWorld->IsInTimeAllInInvestment())
					{
						m_pMyOnlineWorld->pOnlineClient->SendRequest_guild_choice_player(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx].siVillageCode, m_BookDataSiegeWarfare.m_pMemberList[m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx].szName);
						m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_WAIT;
					}
					else
						m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;
				}
				// 권한주기 취소
				else if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL)
				{
					m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;
				}
			}
			break;

		case ON_BOOK_SIEGEWARFARE_EXIT:	//	3
			break;
	}
}

#define DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X		20
#define DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y		66
#define DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH			60
#define DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT			15
#define DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP		24
#define DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X	86
#define DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y	66
#define DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH			134
#define DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT		15


// 대방 공성전 관련 인터페이스 그리기
VOID	OnlineTradeBook::DrawSiegeWarfare(LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC				hDC;
	CHAR			szTemp[512];
//	CHAR			szTemp2[128];
//	VillageHeader	*pHeader;
//	char			szMoney[50];
//	COLORREF		rgb;		
//	MONEY			mnTemp;

	// 이미지들 출력
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		// 대화상자 이미지 출력
		clGrp.PutSpriteT( m_Total.m_siX,m_Total.m_siY,m_BookSiegeWarfare.Header.Xsize,m_BookSiegeWarfare.Header.Ysize,m_BookSiegeWarfare.Image);

		// 버튼 이미지 출력
		/*
		// 검병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		// 창병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		// 궁병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		// 기마병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		// 충차 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		// 발석거 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		// 보급수레 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.Put(&m_BookDataSiegeWarfare.m_sprButtonRecruitArmy, 0, 0, 1, BUTTON_PUT_NOMOVE);
		*/
		// 권한주기
		m_BookDataSiegeWarfare.m_btnGiveACompetency.Put(&m_sprTown_button_medium, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 점령한 마을 목록 리스트 박스 스크롤 바가 필요한 경우만 그리기
		if (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount > ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT)
		{
			// 점령한 마을 목록 리스트 박스 스크롤 바 배경 그리기
			clGrp.PutSpriteT( m_Total.m_siX + 108, m_Total.m_siY + 253,
								m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_sprScrollBar.Header.Xsize,
								m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_sprScrollBar.Header.Ysize,
								m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_sprScrollBar.Image);

			// 점령한 마을 목록 리스트 박스의 스크롤 바 그리기
			clGrp.PutSpriteT( m_Total.m_siX + 112, 
							m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_rtScrollBar.top + m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos,
							m_BookScrollBarSpr.Header.Xsize, m_BookScrollBarSpr.Header.Ysize,
							m_BookScrollBarSpr.Image );

			// 점령한 마을 목록 리스트 박스 스크롤 버튼 그리기
			m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollUp.Put(&m_BookScrollBttnUpSpr,0,1,0,BUTTON_PUT_NOMOVE);
			m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_btnScrollDown.Put(&m_BookScrollBttnDnSpr,0,1,0,BUTTON_PUT_NOMOVE);
		}
		
		clGrp.UnlockSurface( pSurface );
	}

	// 텍스트 출력
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// 대화 상자 타이틀 출력(공성)
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_Total.m_siX + 60, m_Total.m_siY + 8, 118, 12,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOK_INTERFACE_SIEGEWARFARE), RGB( 216, 201, 183 ));

		// 닫기 버튼 텍스 출력
		m_BttnMoneyExit.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		/////////////////////
		// 검병
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y,
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT, 
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SWORDMAN), RGB( 216, 201, 183 ) );
		// 검병 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y,
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT,
															szTemp, RGB(250, 250, 250) );

		/////////////////////
		// 창병
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 1)),
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT,
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SPEARMAN), RGB( 216, 201, 183 ) );
		// 창병 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 1)), 
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT,
															szTemp, RGB(250, 250, 250) );

		/////////////////////
		// 궁병
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 2)),
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT,
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ARCHER), RGB( 216, 201, 183 ) );
		// 궁병 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 2)), 
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT,
															szTemp, RGB(250, 250, 250) );

		/////////////////////
		// 기마병
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 3)),
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT,
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HORSEMAN), RGB( 216, 201, 183 ) );
		// 기마병 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 3)), 
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT,
															szTemp, RGB(250, 250, 250) );

		/////////////////////
		// 충차
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 4)),
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT,
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SIEGETOWER), RGB( 216, 201, 183 ) );
		// 충차 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 4)), 
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT, 
															szTemp, RGB(250, 250, 250) );

		/////////////////////
		// 발석거
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 5)),
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT,
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CATAPULT), RGB( 216, 201, 183 ) );
		// 발석거 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 5)), 
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT, 
															szTemp, RGB(250, 250, 250) );

		/////////////////////
		// 보급수레
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 6)),
															DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_HEIGHT, 
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SUPPLYGOODS_WAGON), RGB( 216, 201, 183 ) );
		// 보급수레 부대수
		MakeMoneyText(m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON], szTemp);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_X, m_Total.m_siY + (DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_POSITION_Y + (DRAW_SIEGEWARFARE_ITEM_KIND_TEXT_VERT_GAP * 6)), 
															DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_WIDTH, DRAW_SIEGEWARFARE_ITEM_KIND_COUNT_HEIGHT, 
															szTemp, RGB(250, 250, 250) );

		// 상단 보급 품 보유수

		// 점령한 마을
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 20, m_Total.m_siY + 239, 
															80, 12, 
															m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OCCUPIED_VILLAGE_COUNT), RGB(216, 201, 183) );

		sprintf(szTemp, "%4d", m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount);	// 점령한 마을 수
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_Total.m_siX + 106, m_Total.m_siY + 239, 
															37, 12, 
															szTemp, RGB(216, 201, 183) );
		
		// 점령한 마을 목록
		if (m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage)
		{
			for (SI16 ssI = 0; ssI < min(ON_BOOK_OCCUPIED_VILLAGE_LISTBOX_COUNT, m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount); ssI++)
			{
				VillageHeader*		lpVillageHeader;
				DWORD dwTextColor = RGB(250, 250, 250);

				// 사용자가 선택한 아이템(마을)일 경우 글자색 변경
				if (ssI + m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll == m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx)
					dwTextColor = RGB(0, 250, 0);
		
				// 점령한 마을 정보 출력
				lpVillageHeader = m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[ssI + m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll].siVillageCode);
				if (lpVillageHeader)
				{
					// 점령한 마을 이름 출력
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox( hDC, m_Total.m_siX + 14, m_Total.m_siY + 256 + (ssI * 30), 
																		95, 12, NULL,
																		m_pMyOnlineWorld->pOnlineText->Get( lpVillageHeader->siVillageName ), dwTextColor,
																		DT_VCENTER);

					// 폭탄 투자 가능한 % 출력
					sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_COMPETENCY_PERCENT), m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList[ssI + m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll].siPercent);

					m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox( hDC, m_Total.m_siX + 14, m_Total.m_siY + 256 + (ssI * 30) + 15, 
																		95, 12, NULL,
																		szTemp, dwTextColor,
																		DT_RIGHT | DT_VCENTER);
				}
			}
		}
		
		// 상단원 목록
		if (m_BookDataSiegeWarfare.m_bDownloadedGuildMember)
		{
			DrawMemberList(hDC);
		}
		
		/*
		// 버튼 텍스트 출력
		// 검병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SwordMan.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		// 창병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SpearMan.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		// 궁병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_ArcherMan.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		// 기마병 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_HorseMan.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		// 충차 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SiegeTower.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		// 발석거 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_Catapult.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		// 보급수레 모집
		m_BookDataSiegeWarfare.m_btnRecruitArmy_SupplyWagon.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		*/
		// 권한주기
		m_BookDataSiegeWarfare.m_btnGiveACompetency.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hDC );
	}

	DrawTab(pSurface);
}

// 지금 상태 대화상자가 활성화 되었는지 검사
BOOL	OnlineTradeBook::IsActivityBook(SI08 siBookKind)
{
	if ((m_siStatus == siBookKind) && (TRUE == m_bActive))
		return TRUE;

	return FALSE;
}

// 상단원 목록 처리
SI16 OnlineTradeBook::ActionMemberList(void)
{
	SI16 siListCount;

	// 상단원 선택 처리
	SI16 siYCount, TempSelect;
	
	siListCount = m_BookDataSiegeWarfare.m_uiMemberNum % GUILD_LIST_MAX_LINE;

	for(siYCount = 0; siYCount < GUILD_LIST_MAX_LINE; siYCount++)
	{
		if(m_ptMouse.x >= m_BookDataSiegeWarfare.m_siX+123 && m_ptMouse.x < m_BookDataSiegeWarfare.m_siX+218 && m_ptMouse.y >= (m_BookDataSiegeWarfare.m_siY+256 + siYCount * 15) && m_ptMouse.y < (m_BookDataSiegeWarfare.m_siY+256 + (siYCount + 1) * 15))
		{
			TempSelect = siYCount;
			
			if(TempSelect >= siListCount)
				continue;

			m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx = TempSelect;
			break;
		}
	}

	// Page 선택
	return ActionPage();
}

// 상단원 페이지 번호간 간격
#define PAGENUMBER_WIDTH	18

// 상단원 목록 페이지 처리
SI16 OnlineTradeBook::ActionPage()
{
	SI16 StartPage, EndPage, Prior, Next, PageCount, RenderPosX;
	CheckRenderPage((SI16)m_BookDataSiegeWarfare.m_siMaxPage, (SI16)m_BookDataSiegeWarfare.m_siCurPage, StartPage, EndPage, Prior, Next, 4);

	POINT ptMouse = {IpD.Mouse_X, IpD.Mouse_Y};

	// Page 선택
	PageCount = EndPage - StartPage + 1;
	RenderPosX = m_BookDataSiegeWarfare.m_siX + 170 - (PageCount * PAGENUMBER_WIDTH) / 2;
	for(SI16 i = 0; i < PageCount; i++)
	{
		if(ptMouse.x >= (RenderPosX+i*PAGENUMBER_WIDTH) && ptMouse.x < (RenderPosX+(i+1)*PAGENUMBER_WIDTH) && ptMouse.y >= (m_BookDataSiegeWarfare.m_siY+346) && ptMouse.y <= (m_BookDataSiegeWarfare.m_siY+361))
		{
			if(m_BookDataSiegeWarfare.m_siCurPage != (StartPage+i))
			{
				m_BookDataSiegeWarfare.m_siCurPage = StartPage + i;
				return m_BookDataSiegeWarfare.m_siCurPage;
			}
		}
	}

	if(Prior)
	{
		RenderPosX -= PAGENUMBER_WIDTH - 3;
		if(ptMouse.x >= RenderPosX && ptMouse.x < (RenderPosX+PAGENUMBER_WIDTH) && ptMouse.y >= (m_BookDataSiegeWarfare.m_siY+346) && ptMouse.y <= (m_BookDataSiegeWarfare.m_siY+361))
		{
			if(m_BookDataSiegeWarfare.m_siCurPage != (StartPage-1))
			{
				m_BookDataSiegeWarfare.m_siCurPage = StartPage - 1;
				return m_BookDataSiegeWarfare.m_siCurPage;
			}
		}
	}
	if(Next)
	{
		// robypark 2005/1/25 11:45
		// 이전 페이지와 다음 페이지 표시가 있을 경우 위치 버그 수정
		if (Prior)
			RenderPosX += (PageCount + 1) * PAGENUMBER_WIDTH - 3;
		else
			RenderPosX += PageCount * PAGENUMBER_WIDTH + 2;
		if(ptMouse.x >= RenderPosX && ptMouse.x < (RenderPosX+PAGENUMBER_WIDTH) && ptMouse.y >= (m_BookDataSiegeWarfare.m_siY+346) && ptMouse.y <= (m_BookDataSiegeWarfare.m_siY+361))
		{
			if(m_BookDataSiegeWarfare.m_siCurPage != (EndPage+1))
			{
				m_BookDataSiegeWarfare.m_siCurPage = EndPage + 1;
				return m_BookDataSiegeWarfare.m_siCurPage;
			}
		}
	}
	return -1;
}

// 상단원 목록 페이지 번호 그리기
VOID OnlineTradeBook::DrawPage(HDC hdc)
{
	CHAR szTemp1[256];
	DWORD TextColor = RGB(250, 250, 250);

	SI16 StartPage, EndPage, Prior, Next, PageCount, RenderPosX;
	CheckRenderPage((SI16)m_BookDataSiegeWarfare.m_siMaxPage, (SI16)m_BookDataSiegeWarfare.m_siCurPage, StartPage, EndPage, Prior, Next, 4);

	PageCount = EndPage - StartPage + 1;
	RenderPosX = m_BookDataSiegeWarfare.m_siX + 170 - (PageCount * PAGENUMBER_WIDTH) / 2;
	for(SI16 i = 0; i < PageCount; i++)
	{
		SI16 index = StartPage + i;
		if(m_BookDataSiegeWarfare.m_siCurPage == index) 
			TextColor = RGB(0, 250, 0);
		else
			TextColor = RGB(250, 250, 250);

		sprintf((char*)szTemp1, "%2d", index);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, RenderPosX + i * PAGENUMBER_WIDTH, m_BookDataSiegeWarfare.m_siY + 346, PAGENUMBER_WIDTH, 12, (char*)szTemp1, TextColor);
	}

	if(Prior)
	{
		RenderPosX -= PAGENUMBER_WIDTH - 3;
//		sprintf((char*)szTemp1, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PREV));
		sprintf((char*)szTemp1, "<<");
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, RenderPosX, m_BookDataSiegeWarfare.m_siY + 346, PAGENUMBER_WIDTH, 12, (char*)szTemp1, RGB(250, 250, 250));
	}
	if(Next)
	{
		// robypark 2005/1/25 11:45
		// 이전 페이지와 다음 페이지 표시가 있을 경우 위치 버그 수정
		if (Prior)
			RenderPosX += (PageCount + 1) * PAGENUMBER_WIDTH - 3;
		else
			RenderPosX += PageCount * PAGENUMBER_WIDTH + 2;

//		sprintf((char*)szTemp1, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEXT));
		sprintf((char*)szTemp1, ">>");
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, RenderPosX, m_BookDataSiegeWarfare.m_siY + 346, PAGENUMBER_WIDTH, 12, (char*)szTemp1, RGB(250, 250, 250));
	}
}

// 상단원 목록 그리기
VOID OnlineTradeBook::DrawMemberList(HDC hdc)
{
	CHAR szTemp1[256], szTemp2[256];
	DWORD TextColor = RGB(250, 250, 250);

	// 상단원리스트 출력
	for(SI16 i = 0; i < PAGE_LIST_MAX_LINE; i++)
	{
		SI16 index = i + (m_BookDataSiegeWarfare.m_siCurPage - 1) * PAGE_LIST_MAX_LINE;
			
		if(i >= m_BookDataSiegeWarfare.m_uiMemberNum || index < 0)
			continue;
		if(i == m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx)
			TextColor = RGB(0, 250, 0);
		else
			TextColor = RGB(250, 250, 250);
		/*	
		// 번호
		sprintf((char*)szTemp1,  "%3d", index + 1);
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_BookDataSiegeWarfare.m_siX+123, m_BookDataSiegeWarfare.m_siY+256 + (i * 15), (char*)szTemp1, TextColor);
		*/	
		// 아이디
		ZeroMemory(szTemp1, sizeof(szTemp1));
		strncpy((char*)szTemp1, (char*)m_BookDataSiegeWarfare.m_pMemberList[i].szName, ON_ID_LENGTH);
		sprintf((char*)szTemp2,  "%3s", szTemp1);
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_BookDataSiegeWarfare.m_siX+123, m_BookDataSiegeWarfare.m_siY+256 + (i * 15), (char*)szTemp2, TextColor);

		/*
		// 직급
		sprintf((char*)szTemp1,  "%5s", m_pMyOnlineWorld->pOnlineText->Get(m_pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_GuildData.m_pMemberList[i].siClassInGuild)));
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+165, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
	
		// 레벨
		sprintf((char*)szTemp1,  "%4d", m_GuildData.m_pMemberList[i].siLevel);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+260, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
			
		// 재산현황
		ZeroMemory(szTemp1, sizeof(szTemp1));
		MakeMoneyText(m_GuildData.m_pMemberList[i].mnPropery, (char*)szTemp1);
		sprintf((char*)szTemp2,  m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), szTemp1);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+360, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);
			
		// 신용등급
		sprintf((char*)szTemp1,  "%5d", m_GuildData.m_pMemberList[i].uiTradeCredit);
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+436, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
		*/
	}

	// Page 번호
	DrawPage(hdc);
}

// 상단원 목록 요청 응답 처리
UI16	OnlineTradeBook::SetGuildMemberList(BYTE *pMsg)
{
	OnResGuildMembersListMsg *lpMembersListMsg = (OnResGuildMembersListMsg*)pMsg;

	if(lpMembersListMsg->dwResponse == ON_RET_OK)
	{
		BYTE *lpTempBuffer = (BYTE*)lpMembersListMsg + sizeof(OnResGuildMembersListMsg);

		if(m_BookDataSiegeWarfare.m_pMemberList) 
		{
			delete[] m_BookDataSiegeWarfare.m_pMemberList;
			m_BookDataSiegeWarfare.m_pMemberList = NULL;
		}
		
		m_BookDataSiegeWarfare.m_pMemberList = new strGuildMemberInfo[lpMembersListMsg->uiGuildMemebersNum];
		
		m_BookDataSiegeWarfare.m_uiMemberNum = lpMembersListMsg->uiGuildMemebersNum;
		m_BookDataSiegeWarfare.m_siMaxPage = lpMembersListMsg->dwTotalMemberListPageNum;
		
		memcpy(m_BookDataSiegeWarfare.m_pMemberList, lpTempBuffer, sizeof(strGuildMemberInfo) * lpMembersListMsg->uiGuildMemebersNum);
		/*
		for(UI16 i = 0; i < lpMembersListMsg->uiGuildMemebersNum; i++)
		{
			const TradeHeader* pHeader = m_pMyOnlineWorld->pOnlineTradeParser->GetHeader(m_BookDataSiegeWarfare.m_pMemberList[i].uiTradeCredit);
			m_BookDataSiegeWarfare.m_pMemberList[i].uiTradeCredit = (pHeader == NULL) ? 0 : pHeader->uiTradeGrade;
		}
		*/

		m_BookDataSiegeWarfare.m_siSelectedGuildMemberIdx = 0;

		m_BookDataSiegeWarfare.m_bDownloadedGuildMember = TRUE;
	}
	else
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_IMPOMATIONFAIL));

	return 0;
}

// robypark 2004/11/16 15:36
// 공성전 상단 공성 유닛 리스트 요청 응답
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 OnlineTradeBook::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;

	ZeroMemory(m_BookDataSiegeWarfare.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	// 공성 검병 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN];
	// 공성 창병 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN];
	// 공성 궁병 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN];
	// 공성 기마병 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN];
	// 공성 충차 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER];
	// 공성 발석거 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT];
	// 공성 보급수레 부대수 설정
	m_BookDataSiegeWarfare.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON];

	m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = TRUE;

	return 0;
}

// 상단에 있는 공성전 유닛 부대를 소집권 아이템으로 변환한 결과
UI16 OnlineTradeBook::OnDecreaseGuildUnit(BYTE* pMsg)
{
	OnResDecreaseGuildUnit*			pOnResDecreaseGuildUnit;

	pOnResDecreaseGuildUnit			=	(OnResDecreaseGuildUnit*)pMsg;

	m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;

	if (ON_RET_OK == pOnResDecreaseGuildUnit->dwResponse)
	{
		// 공성전 소집권 아이템 정보 헤더 얻기
//		CItemHeader *pItemHeader = m_pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(pOnResDecreaseGuildUnit->siKind);

//		if (pItemHeader)
		{
			// 상단에 남은 공성전 유닛 부대수 수정
			m_BookDataSiegeWarfare.m_siGuildCount[pOnResDecreaseGuildUnit->siKind] = pOnResDecreaseGuildUnit->siTotalCount;
		}

		// 사용자 인벤토리에 아이템 추가
		MyItemData SiegeWarfareUnitItem;

		// 소집권 아이템 정보 설정
		SiegeWarfareUnitItem.siPosInInventory = pOnResDecreaseGuildUnit->siInventoryPos;
		SiegeWarfareUnitItem.uiID			  = pOnResDecreaseGuildUnit->siItemID;
		SiegeWarfareUnitItem.uiQuantity		  = pOnResDecreaseGuildUnit->siQuantity;

		// 인벤토리에 삽입
		m_pMyOnlineWorld->pMyData->AddItem(pOnResDecreaseGuildUnit->siMercenarySlot, &SiegeWarfareUnitItem);
		m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
	}
	else
	{
		// 소집권 아이템 생성 실패
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_CREATE_ITEM_ABOUT_RECRUIT_ARMY));
	}

	m_BookDataSiegeWarfare.m_bDownloadedGuildUnitCount = TRUE;

	return 0;
}

// 공성전 상단 보급품 개수 얻기
// ON_RESPONSE_LIST_GUILD_SUPPLY
UI16 OnlineTradeBook::OnResponseListGuildSupply(BYTE *pMsg)
{
	OnResGetGuildSupplyList *pOnResGetGuildSupplyList = (OnResGetGuildSupplyList *)pMsg;

	m_BookDataSiegeWarfare.m_uiGuildSupplyGoodsAmount = pOnResGetGuildSupplyList->siGuildSupply;

	return 0;
}

// robypark 2004/12/2 10:56
// 공성전 결과 점령한 마을 리스트 및 투자 % 응답
// ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT
UI16 OnlineTradeBook::OnResponseGuildVillageattackResult(BYTE *pMsg)
{
	OnResVillageAttackResult *pOnResVillageAttackResult = (OnResVillageAttackResult *)pMsg;

	// 이전 데이터 삭제
	if (m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList)
	{
		delete []m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList;

		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList = NULL;
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount = 0;
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScroll = 0;
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siScrollPos = 0;
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx = 0;
	}

	m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage = FALSE;

	// 리스트 요청 응답 결과가 정상적으로 처리
	if (ON_RET_OK == pOnResVillageAttackResult->dwResponse)
	{
		if (0 >= pOnResVillageAttackResult->siVillageCount)
			return 0;

		m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage = TRUE;

		// 점령한 마을 개수 설정
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount = pOnResVillageAttackResult->siVillageCount;

		// 점령한 마을 정보 변수 메모리 할당
		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList = new OnGuildResult[m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount];

		ZeroMemory(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList, sizeof(OnGuildResult) * m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siOccupiedVillageCount);

		// 점령한 마을 정보 복사
		memcpy(m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_pOccupiedVillageList, (BYTE*)pOnResVillageAttackResult + sizeof(OnResVillageAttackResult), sizeof(OnGuildResult) * pOnResVillageAttackResult->siVillageCount);

		m_BookDataSiegeWarfare.m_sBook_Siegewarfare_Occupied_Village.m_siSelectedItemIdx = 0;
	}

	return 0;
}

// robypark 2004/12/2 13:45
// 공성전 종료 후에 마을에 대한 폭투 권한을 특정 캐릭터에 주기 요청
// ON_RESPONSE_GUILD_CHOICE_PLAYER
UI16 OnlineTradeBook::OnResponseGuildChoicePlayer(BYTE *pMsg)
{
	OnResGuildChoicePlayer	*pOnResGuildChoicePlayer	= (OnResGuildChoicePlayer *)pMsg;

	m_BookDataSiegeWarfare.m_siStatus = ON_BOOK_SIEGEWARFARE_POLL;

	if (ON_RET_OK == pOnResGuildChoicePlayer->dwResponse)
	{
		VillageHeader *lpVillageHeader = m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResGuildChoicePlayer->siVillageCode);
		
		if (lpVillageHeader)
		{
			// 대방이 다른 사람에게 투자권한을 준 경우
			if (0 != strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(), pOnResGuildChoicePlayer->szUserName))
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
													m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HAVE_A_COMPETENCY), 
													pOnResGuildChoicePlayer->szUserName,
													m_pMyOnlineWorld->pOnlineText->Get( lpVillageHeader->siVillageName ));
			}
			else
			{
				// 대방이 자기 자신에게 투자권한을 준 경우
				// %s 마을에 대한 투자권을 위임 받았습니다. 투자권은 %d 시에 사라지니, 그 전에 투자권을 행사 하십시오.
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
													m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MSGBOX_RECEIVE_COMPETENCY), 
													m_pMyOnlineWorld->pOnlineText->Get( lpVillageHeader->siVillageName ),
													m_pMyOnlineWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_AllInInvestment_End);
			}
		}

		// 점령한 마을 새로 요청
		m_BookDataSiegeWarfare.m_bDownloadedOccuiedVillage = FALSE;
		m_pMyOnlineWorld->pOnlineClient->SendRequest_guild_villageattack_result();
	}
	else
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_GIVE_A_COMPETENCY));

	return 0;
}
