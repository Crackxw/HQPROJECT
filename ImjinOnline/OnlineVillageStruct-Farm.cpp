
/*
********************************************************************************************************
	파일명 : OnlineVillageStruct-Farm.cpp
	작성자 : 정진욱
	담당자 : 정진욱
	작성일 : 2001.11.20
	수정일 : 2002.02.04
	내  용 : 생산 건물 내부 ( 농장이 될수 있고 공장두 될수 있따 )
			 현재는 농장전용...
*********************************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include <SoundManager.h>
#include <clGame.h>

#include "OnlineWorld.h"
#include "OnlineListControl.h"
#include "OnlineVillageBaseStructure.h"
#include "OnlineBaseLB.h"
#include "OnlineVillageStruct-Farm.h"
#include "OnlineProductStructure.h"
#include "OnlineMegaText.h"
#include "OnlineETC.h"
#include "OnlineHelp.h"
#include "OnlineNumberControl.h"
#include "OnlineTradeBook.h" 
#include "OnlineVillageStruct-Make.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineMydata.h"
#include "OnlineVillage.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include "OnlineMiniMap.h"
#include "OnlineMsgBox.h"
#include "OnlineInventory.h"
#include "MiniGame1.h"
#include "OnlineMercenary.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

extern _InputDevice				IpD;
extern	_clGame* pGame;
extern	SoundManager	   *g_pSoundManager;

#define ON_BUTTON_CHECK_SAMPLE_VALUE	1
#define ON_FARM_TAX_RATE				10

#define ON_FARM_SHOW_BUILD_STRUCT		0
#define ON_FARM_SHOW_BUILD_CONTROL		2
#define ON_FARM_SHOW_ITEM_MAKE			1




// 스크롤 용
#define ON_FARM_PREV_NONE				0
#define ON_FARM_PREV_EXIT				1
#define ON_FARM_PREV_MAIN				2
#define ON_FARM_PREV_BUILD				3
#define ON_FARM_PREV_WORK				4


// 메인
#define ON_FARM_SCROLL_LEFT				1
#define ON_FARM_SCROLL_RIGHT			2
#define ON_FARM_INIT					4
#define ON_FARM_POLL					5
#define ON_FARM_EXIT					6
#define ON_FARM_SCROLL_EXITRIGHT		7
#define ON_FARM_SCROLL_EXITLEFT			8
#define ON_FARM_BUILD_STRUCT			9
#define ON_FARM_SCROLL_RIGHT_INIT		10
#define ON_FARM_WAIT					11
#define ON_FARM_ITEM_MAKE				14
#define ON_FARM_STOREHOUSE				15
#define ON_FARM_WORKER					16
#define ON_FARM_EXTENSION_WAIT			17
#define ON_FARM_DESTROY_WAIT			18
#define ON_FARM_UPGRADE_WAIT			19
#define ON_FARM_UPGRADE_INPUT			20
#define ON_FARM_DESTROY					21
#define ON_FARM_EXIT_WAIT				22




// 건물 건설
#define ON_FARM_BUILD_STRUCT_INIT		1
#define ON_FARM_BUILD_STRUCT_POLL		2
#define	ON_FARM_BUILD_STRUCT_EXIT		3
#define	ON_FARM_BUILD_STRUCT_WAIT		4
#define	ON_FARM_BUILD_STRUCT_INPUT		5

#define ON_FARM_STRUCT_MAKE				1
#define ON_FARM_STRUCT_ITEM_MAKE		2


// 건물 관리
#define ON_FARM_BUILD_CONTROL_INIT		1
#define ON_FARM_BUILD_CONTROL_POLL		2
#define	ON_FARM_BUILD_CONTROL_EXIT		3
#define ON_FARM_BUILD_CONTROL_WAIT		4


// 창고
#define ON_FARM_BUILD_STOREHOUSE_INIT		1
#define ON_FARM_BUILD_STOREHOUSE_POLL		2
#define ON_FARM_BUILD_STOREHOUSE_EXIT		3
#define ON_FARM_BUILD_STOREHOUSE_INPUT		4
#define ON_FARM_BUILD_STOREHOUSE_WAIT		5
#define ON_FARM_BUILD_STOREHOUSE_DESTROY	6
#define ON_FARM_BUILD_STOREHOUSE_DESTROY_OK	7



// 아이템 생산
#define ON_FARM_ITEM_MAKE_INIT			1
#define ON_FARM_ITEM_MAKE_POLL			2
#define ON_FARM_ITEM_MAKE_EXIT			3
#define ON_FARM_ITEM_MAKE_WAIT			4
#define ON_FARM_ITEM_MAKE_INPUT			5


// 일하기
#define ON_FARM_WORK_INIT				1
#define ON_FARM_WORK_POLL				2
#define ON_FARM_WORK_PLAY_MINIGAME		3
#define ON_FARM_WORK_EXIT				4
#define ON_FARM_WORK_WAIT				5
#define ON_FARM_WORK_TOOL				6
#define ON_FARM_WORK_CHANG_PAY_WAIT		7
#define ON_FARM_WORK_CHANG_PAY_INPUT	8
#define ON_FARM_WORK_CANCEL				9
#define ON_FARM_WORK_STOP			   10

/////////////////////////////////////


OnlineVillageStructFarm::OnlineVillageStructFarm() : OnlineVillageBaseStructure()
{
	m_pMyVillage		=	NULL;
	m_pMyOnlineWorld	=	NULL;
	m_pTempDragList		=	NULL;
	m_NpcList			=	NULL;
	m_pItemID			=	NULL;
	m_pTitleName		=	NULL;
	m_bUpGrade			=	FALSE;
	m_bMouseDown		=	FALSE;
	m_bOpen				=	FALSE;
	m_bExtension		=	FALSE;
	bNowScroll			=	FALSE;
	m_bInFactory		=	FALSE;
	m_bWorkCompleted	=	FALSE;
	m_bExitOk			=	FALSE;
	m_siStatus			=	0;	
	m_siSelect			=	-1;
	m_siScrollX			=	0;
	m_siRight			=	0;
	m_siShowFont		=	-1;	
	m_siTempConunt		=	-1;
	m_siMouseStatus		=	0;
	m_siYetPos			=	0;
	m_siTalk			=	0;
	m_siMiniGameType	=	0;
	m_siMiniGameScore	=	0;
	m_siBuildIndex		=   0;
//	m_siMsgResult		=	-1;

	ZeroMemory( &m_siXpos, sizeof( SI16 ) * ON_MAX_FACTORY_NUM );
	ZeroMemory( &m_siYpos, sizeof( SI16 ) * ON_MAX_FACTORY_NUM );
	ZeroMemory( m_szTalk, sizeof( m_szTalk ) );

	m_CheckInterface.m_uiKind = 0;
}


OnlineVillageStructFarm::~OnlineVillageStructFarm()
{
	Free();
}

//////////////////////////////////////
// 초기화
VOID	OnlineVillageStructFarm::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{
	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	m_pMyOnlineWorld	=	pOnlineWorld;
	m_pMyVillage	 	=	pVillageStructManager;	
	m_siStatus			=	ON_FARM_INIT;
	m_siPrevStatus		=	ON_FARM_PREV_NONE;
	m_siBuildStatus		=	ON_FARM_BUILD_CONTROL_INIT;
	m_pMakeList			=	m_pMyOnlineWorld->pOnlineVillage->GetMakeList();
	m_bNoBuilding		=	FALSE;
	m_bOpen				=	FALSE;
	m_bMiniGame			=	FALSE;
	m_bRecvOk			=	FALSE;
	m_bExitOk			=	FALSE;

	//TOWN_PRODUCTION_BUILDING.Spr

	clGrp.LoadXspr("Online\\VillageBack\\Farm_Back.Spr", BackImage );									// 뒷 배경
	clGrp.LoadXspr("Online\\VillageBack\\Farm_Title.Spr", m_TitleSpr );
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_MainButtonSpr );						// 아무것두 건설하지 않았을때의 바닥	
	clGrp.LoadXspr("Online\\GameSpr\\village\\TOWN_PRODUCTION_BUILDING.Spr", m_InterfaceSpr );
	clGrp.LoadXspr("Online\\GameSpr\\village\\TOWN_PRODUCTION_ITEM.Spr",m_ItemInterfaceSpr);
//	clGrp.LoadXspr("Online\\VillageBack\\VillageBorder.Spr", m_BoardSpr );
	clGrp.LoadXspr("Online\\GameSpr\\Book\\main_window_button.Spr", m_BackSpr );
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\Main_Button2.Spr", m_OkSpr );
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_PRODUCTION_WAREHOUSE.Spr", m_NpcSpr );
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW.Spr", m_CheckSpr );	//메세지 창
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW_LINE.Spr",m_CheckLineSpr);
	clGrp.LoadXspr("Online\\VillageBack\\ItemPannel.Spr", m_ItemPannelSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_PRODUCTION_ITEM_SCRBAR.Spr", m_ItemScrollBase);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_PRODUCTION_BUTTON_BASE.Spr",m_ButtonBaseSpr);
	clGrp.LoadXspr("Online\\GameSpr\\inventory\\Sub_Button.Spr",m_ButtonCheckBttnSpr);
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\TOWN_EXIT_BASE.Spr", m_ExitBackImage);
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\TOWN_EXIT_button.Spr", m_ExitImage);
	clGrp.LoadXspr( "Online\\Gamespr\\Book\\book_scr_Up.Spr",m_ScrollUpImage);
	clGrp.LoadXspr( "Online\\Gamespr\\Book\\book_scr_Down.Spr",m_ScrollDnImage);


	m_pButton3Spr = m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_EXITBUTTON );

	m_siX						=	134;//134800 - 354 - m_NpcSpr.Header.Xsize;
	m_siY						=	45;//45'0;

	// 어느 나라 인지 알아 온다.
	switch( m_pMyOnlineWorld->pOnlineVillage->GetNation() )
	{
	case ON_COUNTRY_JAPAN:
		clGrp.LoadXspr("Online\\VillageBack\\Farm_Building_JAN.Spr", m_BuidingSpr );
		break;

	case ON_COUNTRY_CHINA:
		clGrp.LoadXspr("Online\\VillageBack\\Farm_Building.Spr", m_BuidingSpr );
		break;

	case ON_COUNTRY_TAIWAN:
		clGrp.LoadXspr("Online\\VillageBack\\Farm_Building.Spr", m_BuidingSpr );
		break;

	case ON_COUNTRY_KOREA:
	default:
		clGrp.LoadXspr("Online\\VillageBack\\Farm_Building.Spr", m_BuidingSpr );
		break;
	}
	

	//시설 관리, 물품 생산, 시설 철거, 시설 향상 버튼
	m_BMain02.Create( m_siX + 314, m_siY + 47, m_MainButtonSpr.Header.Xsize, m_MainButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_STRCT_PROVE), BUTTON_PUT_LEFT, TRUE );
	m_BMain03.Create( m_siX + 422, m_siY + 47, m_MainButtonSpr.Header.Xsize, m_MainButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_STRCT_DESTROY), BUTTON_PUT_LEFT, TRUE );

	// 나가기, 돌아가기, 오케이, 작업 중지, 임금변경 버튼
	m_BExit.Create(691, 474, m_ExitImage.Header.Xsize, m_ExitImage.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );	
	m_BBack.Create( m_siX + 232,m_siY + 394, m_BackSpr.Header.Xsize, m_BackSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BACK ), BUTTON_PUT_LEFT, FALSE );

	m_BModify.Create( m_siX + 106, m_siY + 364, m_MainButtonSpr.Header.Xsize, m_MainButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAYCHANGE), BUTTON_PUT_LEFT, FALSE );
	m_BStop.Create( m_siX + 227, m_siY + 364, m_MainButtonSpr.Header.Xsize, m_MainButtonSpr.Header.Ysize,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKSTOP), BUTTON_PUT_LEFT, FALSE );
	m_BOk.Create( m_siX + 348, m_siY + 364, m_MainButtonSpr.Header.Xsize, m_MainButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_BUILD ), BUTTON_PUT_LEFT, FALSE );

	// 창고에서 쓸 버튼 ( 제거 버튼을 만들어 준다. )
	m_BDestroy.Create( m_siX + 134 - 41, m_siY + 362, m_OkSpr.Header.Xsize,
					   m_OkSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WHEREHOUSEDESTORY), BUTTON_PUT_LEFT, FALSE );
	m_BSubExit.Create(m_siX + 139 - 40, m_siY + 394, m_BackSpr.Header.Xsize,  m_BackSpr.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);
	
	m_BScrUp.Create(  m_siX + 344, m_siY + 215, m_ScrollUpImage.Header.Xsize,m_ScrollUpImage.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE);
	m_BScrDn.Create(  m_siX + 344, m_siY + 346, m_ScrollDnImage.Header.Xsize,m_ScrollDnImage.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE);

	m_BScr2Up.Create(  m_siX + 493, m_siY + 215, m_ScrollUpImage.Header.Xsize,m_ScrollUpImage.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE);
	m_BScr2Dn.Create(  m_siX + 493, m_siY + 346, m_ScrollDnImage.Header.Xsize,m_ScrollDnImage.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE);

	m_BScrBuildUp.Create( m_siX + 488 , m_siY + 143,m_ScrollUpImage.Header.Xsize,m_ScrollUpImage.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE);
	m_BScrBuildDn.Create( m_siX + 488 , m_siY + 325,m_ScrollDnImage.Header.Xsize,m_ScrollDnImage.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE);

	// 빌딩 구조체를 초기화 한다.
	for( SI32 i=0 ; i<ON_MAX_FACTORY_NUM; i++ )
	{
		m_Building[i].m_bBuilding		=	FALSE;
		m_Building[i].m_bSelect			=	FALSE;
		m_Building[i].m_siBuilingName	=	-1;		
		m_Building[i].m_pWorkInfo		=	NULL;
		m_Building[i].m_uiPay			=	0;
		m_Building[i].m_uiWork			=	0;
	}


	// 여기 시설에서 건설 할수 있는 것들을 알아 보고 버튼을 생성을 한다.
	//XSPR	*pScrollBar	=	m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_SCROLLV );
	XSPR	*pScrollBar =   m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_QUEST_SCR_HANDLE );
	m_SelectBuild.Create( m_siX + 368, m_siY + 151, 120, 174, m_siX + 368, m_siY + 151, 120, 180, 150, 18, NULL, pScrollBar, m_siX + 488 + 5, m_siY + 152,m_siY + 325);
	m_SelectControl.Create( m_siX + 368, m_siY + 223, 120, 124, m_siX + 368, m_siY + 223, 120, 135, 150, 18, NULL, pScrollBar, m_siX + 493 + 5, m_siY + 224,m_siY + 347,TRUE);
	m_SelectWorkMaterial.Create( m_siX + 368, m_siY + 151, 120, 174, m_siX + 368, m_siY + 151, 120, 135, 120, 18, NULL, NULL, m_siX + 493 + 5, m_siY + 224,m_siY + 347);
	m_SelectItemWorkMaterial.Create(m_siX + 153, m_siY + 223, 176, 124, m_siX + 153, m_siY + 223, 176, 124, 150, 18, NULL, pScrollBar, m_siX + 344 + 5,m_siY + 224,m_siY + 347,TRUE);
	m_SelectMakeMaterial.Create( m_siX + 153, m_siY + 223, 176, 124, m_siX + 153, m_siY + 223, 176, 124, 150, 18, NULL, pScrollBar, m_siX + 344 + 5,m_siY + 224,m_siY + 347,TRUE);
	ZeroMemory( m_szBuildCode, sizeof( m_szBuildCode ) );

	// 우선 무슨 건물인가 알아 보고....
	switch( pVillageStructManager->GetKind() )
	{		
		case ON_VIL_ST_KIND_FARM:		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_FARM );		break;		// 농장
		case ON_VIL_ST_KIND_WEAPON:		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_WEAPON );		break;		// 무기장
		case ON_VIL_ST_KIND_ARMOR:		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_ARMOR );		break;		// 갑주장
		case ON_VIL_ST_KIND_PASTURE:	strcpy( m_szKind, ON_VIL_ST_KIND_CODE_PASTURE );	break;		// 목장
		case ON_VIL_ST_KIND_FACTORY:	strcpy( m_szKind, ON_VIL_ST_KIND_CODE_FACTORY );	break;		// 공장
	}

	stPlants	*pPlants = m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );
	
	// 그 건물에서 지을수 있는 정보를 얻어 온다.
	if( strcmp( pPlants->Code, m_szKind ) == 0 )
		memcpy( m_szBuildCode, pPlants->FCode, sizeof( m_szBuildCode ) );	


	m_NpcList	=	new OnlineListControl(pOnlineWorld);
	m_NpcList->Init( 60, 60, 5, 2, m_siX + 15 - 40, m_siY + 222, 0 );
	m_NpcList->Clear();

//	m_CheckInterface.m_pEdit01	=	new OnlineEditControl;
//	m_CheckInterface.m_pEdit02	=	new OnlineEditControl;

//	m_CheckInterface.m_pEdit01->CreateEditBox( 1000, 1000, 100, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 4, TRUE );
//	m_CheckInterface.m_pEdit02->CreateEditBox( 1000, 1000, 100, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 4, TRUE );

//	SendMessage( m_CheckInterface.m_pEdit01->GethWnd(), EM_LIMITTEXT, (WPARAM)5, 0 );
//	SendMessage( m_CheckInterface.m_pEdit02->GethWnd(), EM_LIMITTEXT, (WPARAM)5, 0 );

//	m_CheckInterface.m_pEdit01->SetNextWindow( m_CheckInterface.m_pEdit02->GethWnd() );
//	m_CheckInterface.m_pEdit02->SetNextWindow( m_CheckInterface.m_pEdit01->GethWnd() );
//	m_CheckInterface.m_pEdit01->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
//	m_CheckInterface.m_pEdit02->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );


	OnlineEditControlEx	*pOecEx;
	if( m_CheckInterface.m_hOecEdit01 )	Handler_OnlineEditControlEx::KillControl( m_CheckInterface.m_hOecEdit01 );
	RECT	rcRect					= { 262 + 25, 212 + 30, 262 + 70, 212 + 45};
	m_CheckInterface.m_hOecEdit01	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect, 5,30000);
	pOecEx							= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
	pOecEx->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(255, 255, 255) );

	if( m_CheckInterface.m_hOecEdit02 )	Handler_OnlineEditControlEx::KillControl( m_CheckInterface.m_hOecEdit02 );
	RECT	rcRect2					= { 262 + 25, 212 + 50, 262 + 70, 212 + 65};
	m_CheckInterface.m_hOecEdit02	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect2, 5,30000);
	pOecEx							= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
	pOecEx->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(255, 255, 255) );

	m_CheckInterface.m_bActive	=	FALSE;

	Handler_OnlineEditControlEx::SetNextHandle(m_CheckInterface.m_hOecEdit01,m_CheckInterface.m_hOecEdit02);
	Handler_OnlineEditControlEx::SetNextHandle(m_CheckInterface.m_hOecEdit02,m_CheckInterface.m_hOecEdit01);



	m_CheckInterface.m_BOk.Create( (( ON_SCREEN_XSIZE - m_CheckSpr.Header.Xsize ) / 2) + 30, (( ON_SCREEN_YSIZE - m_CheckSpr.Header.Ysize ) / 2) + 148, m_ButtonCheckBttnSpr.Header.Xsize, m_ButtonCheckBttnSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_OK ), BUTTON_PUT_LEFT, FALSE );
	m_CheckInterface.m_BCancel.Create( (( ON_SCREEN_XSIZE - m_CheckSpr.Header.Xsize ) / 2) + 164, (( ON_SCREEN_YSIZE - m_CheckSpr.Header.Ysize ) / 2) + 148, m_ButtonCheckBttnSpr.Header.Xsize, m_ButtonCheckBttnSpr.Header.Ysize,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_CENCLE), BUTTON_PUT_LEFT, FALSE );

	ZeroMemory( m_szTalk, sizeof( m_szTalk ) );

	m_pTitleName	=	m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_ADMIN );

	SetRect(&m_rtNpcRect, m_siX + 15 - 40, m_siY + 222, m_siX + 315 - 40, m_siY + 342);
	SetRect(&m_rtPlayerRect, m_siX + m_NpcSpr.Header.Xsize - 15 , m_siY + 222, m_siX + m_NpcSpr.Header.Xsize + 300 , m_siY + 342);
}

BOOL	OnlineVillageStructFarm::Action()
{
	SI16	i;

	m_pMyOnlineWorld->pOnlineInventory->Action();
	m_pMyOnlineWorld->pOnlineMsgBoxError->Process();

	switch( m_siStatus )
	{
	case ON_FARM_BUILD_STRUCT:		BuildAction();					break;					// 건물을 건설을 한다.
	case ON_FARM_ITEM_MAKE:			ItemMakeAction();				break;					// 건물에서 아이템 생산
	case ON_FARM_STOREHOUSE:		StoreAction();					break;					// 창고	
	case ON_FARM_WORKER:			WorkAction();					break;					// 일꾼

	case ON_FARM_POLL: 
		// 단지에서 나가기
		if(m_BExit.Process( m_bMouseDown ) == TRUE || LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
		{
			m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveStructure();
			m_siStatus = ON_FARM_EXIT_WAIT;
			break;
		}

		// 슬롯 확장 
		if( m_bExtension )
		{
			if( m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK )
			{
				m_bExtension	=	FALSE;

				MONEY MyMoney = m_pMyOnlineWorld->pMyData->GetMoney();
				if(MyMoney >= 10000)
				{
					m_siStatus	=	ON_FARM_EXTENSION_WAIT;
					m_pMyOnlineWorld->pOnlineClient->SendRequesExtension(m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList());
				}
				else
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_LENDPLUS), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_EXTENTION_SLOT));
				}
			}
			else if( m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL )
			{
				m_bExtension	=	FALSE;
			}
			break;
		}

		bNowScroll = FALSE;

		for( i=0 ; i < ON_MAX_FACTORY_NUM; i++ )
		{
			if( m_Building[i].m_pBuilding.Process( m_bMouseDown ) == TRUE )
			{
				m_siSelect			=	i;
				// 건물이 있을경우
				if( m_Building[i].m_bBuilding == TRUE )
				{
					if( m_pMakeList->GetMaster() )			// 주인이다 
					{	
						// 만약 창고를 선택을 해따면...
						if( strcmp( m_Building[ m_siSelect ].m_szKind, "STG" ) == 0 )
						{	
							// 창고가 건설 되어 있따.
							if( m_Building[ m_siSelect ].m_uiPay == 0 )
							{
								m_siStatus		=	ON_FARM_STOREHOUSE;
								m_siNextStatus	=	0;
								m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_INIT;								

								// 서버로 아이템을 요청한다.
								m_pMyOnlineWorld->pOnlineClient->SendRequestStorageInfo( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
							}
							// 만약 창고가 건설 중이라면...
							else
							{
								m_Building[i].m_bSelect	=	TRUE;
								m_siStatus				=	ON_FARM_SCROLL_RIGHT_INIT;
								m_siNextStatus			=	ON_FARM_WORKER;
								m_siScrollX				=	0;
								m_siPrevStatus			=	ON_FARM_PREV_WORK;
								m_siShowFont			=	ON_FARM_SHOW_ITEM_MAKE;
								m_siBuildStatus			=	ON_FARM_WORK_INIT;
								m_pTitleName			=	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING);

							}
							break;
						}
						else
						{
							// 이 건물에 일이 있나 보고 일이 이따면 일을 할 수 있또록 설정을 한다.
							if( m_Building[i].m_uiPay == 0 )
							{
								m_siStatus				=	ON_FARM_SCROLL_RIGHT_INIT;
								m_Building[i].m_bSelect	=	TRUE;
								m_siNextStatus			=	ON_FARM_ITEM_MAKE;
								m_siScrollX				=	0;
								m_siPrevStatus			=	ON_FARM_PREV_MAIN;
								m_siBuildStatus			=	ON_FARM_ITEM_MAKE_INIT;
								
								// 이 건물이 현재 생산 중인가 알아 본다.
								break;
							}
							// 일이 이따
							else
							{
								m_Building[i].m_bSelect	=	TRUE;
								m_siStatus				=	ON_FARM_SCROLL_RIGHT_INIT;
								m_siNextStatus			=	ON_FARM_WORKER;
								m_siScrollX				=	0;
								m_siPrevStatus			=	ON_FARM_PREV_WORK;
								m_siShowFont			=	ON_FARM_SHOW_ITEM_MAKE;
								m_siBuildStatus			=	ON_FARM_WORK_INIT;
								m_pTitleName			=	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING);

							}
						}
					}
					else									// 일하러 온 사람
					{
						if( m_Building[i].m_uiPay != 0 )	
						{
							m_Building[i].m_bSelect	=	TRUE;
							m_siStatus				=	ON_FARM_SCROLL_RIGHT_INIT;
							m_siNextStatus			=	ON_FARM_WORKER;
							m_siScrollX				=	0;
							m_siPrevStatus			=	ON_FARM_PREV_WORK;
							m_siShowFont			=	ON_FARM_SHOW_ITEM_MAKE;
							m_siBuildStatus			=	ON_FARM_WORK_INIT;
							m_pTitleName			=	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING);
						}
					}
				}
				// 건물이 없다
				else
				{
					if( m_pMakeList->GetMaster() )
					{
						// 시설 확장
						// Skill(부지확장, 1단계 건물건설)
						if( i >= m_pMakeList->GetSlotNum() )
						{
							m_bExtension	=	TRUE;
							m_pMyOnlineWorld->pOnlineVillage->m_siMsgResult	=	-1;
							m_siSelect		=	i;

							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OKCANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_LENDPLUS), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NEWLEND) );
						}
						else
						{
							m_siStatus				=	ON_FARM_SCROLL_RIGHT_INIT;
							m_siNextStatus			=	ON_FARM_BUILD_STRUCT;
							m_siBuildStatus			=	ON_FARM_BUILD_STRUCT_INIT;

							m_siSelect				=	i;
							m_siBuildSelect			=	-1;
							m_Building[i].m_bSelect	=	TRUE;							

							m_siShowFont			=	ON_FARM_SHOW_BUILD_STRUCT;							
						}
						break;
					}
				}
			}			
		}


		for( i=0 ; i<ON_MAX_FACTORY_NUM; i++ )
		{
			if( m_Building[i].m_pBuilding.IsInMouse() )
			{
				if(m_Building[i].m_uiPay > 0 )
				{
					char desc[256];
					sprintf(desc, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_PAY),m_Building[i].m_uiPay);
				
					m_pMyOnlineWorld->pOnlineHelp->SetText( m_Building[i].m_pBuilding.GetMenuName(), desc,IpD.Mouse_X, IpD.Mouse_Y);
				}
				else
				{
					if(i >= m_pMakeList->GetSlotNum())
					{
						m_pMyOnlineWorld->pOnlineHelp->SetText(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_LENDEXTENTION), IpD.Mouse_X, IpD.Mouse_Y, (ON_HELP_LEFT | ON_HELP_UP), ON_HELP_WHITE);
					}
					else
					{
						m_pMyOnlineWorld->pOnlineHelp->SetText(m_Building[i].m_pBuilding.GetMenuName(), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NOOPERATION), IpD.Mouse_X, IpD.Mouse_Y);
					}
				}
			}
		}
		break;

	case ON_FARM_EXTENSION_WAIT:
		break;

	case ON_FARM_DESTROY:
		if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK) // 오케이 버튼 눌렀을 때
		{
			m_pMyOnlineWorld->pOnlineClient->SendRequesDestroyStrcut( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
			m_siStatus  =  ON_FARM_DESTROY_WAIT;
		}
		
		else if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL )
		{
			m_siStatus	=	ON_FARM_POLL;
		}
		break;

	case ON_FARM_DESTROY_WAIT:
		// FarmStructDestroy()
		break;
		
	case ON_FARM_UPGRADE_INPUT:
		{
			SI16	siTemp	= CheckAction();		
			m_siPay			= _atoi64( m_CheckInterface.m_szEdit02 );

			// OK
			if( siTemp == 1 )
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequseUpgradeSlot( (UI08)m_siSelect, (SI16)m_siPay );
				m_siStatus	=	ON_FARM_UPGRADE_WAIT;				
			}
			// NO
			else if( siTemp == 0 )
			{
				m_siStatus							 = ON_FARM_POLL;
				m_Building[ m_siSelect ].m_bNowBuild = TRUE;				
			}			
		}
		break;

	case ON_FARM_UPGRADE_WAIT:
		break;

	case ON_FARM_INIT:
		m_siStatus	=	ON_FARM_SCROLL_LEFT;
		m_bOpen		=	FALSE;
		break;

	case ON_FARM_SCROLL_LEFT:
		if( m_siPrevStatus ==  ON_FARM_PREV_MAIN )
		{
			m_siStatus	=	ON_FARM_POLL;
			bNowScroll	= FALSE;
			m_siTempX	= 0;
			m_siTempY	= 0;
			m_siTempZ	= 0;

			return FALSE;
		}
		else if( m_siPrevStatus == ON_FARM_PREV_NONE )
		{
			m_siStatus	=	ON_FARM_POLL;				
		}
		else if( m_siPrevStatus == ON_FARM_PREV_EXIT )
		{
			m_siStatus	=	ON_FARM_POLL;
		}

		else if( m_siPrevStatus == ON_FARM_PREV_BUILD )
		{
			m_siStatus		=	ON_FARM_SCROLL_RIGHT_INIT;
			m_siNextStatus	=	ON_FARM_BUILD_STRUCT;
		}

		else if( m_siPrevStatus == ON_FARM_PREV_WORK )				// 일하러 온사람이다..
			m_siStatus	=	ON_FARM_POLL;
//		}		

		m_siTempX = 0;//-( m_BoardSpr.Header.Xsize - (SI16)( GetTickCount() - m_siScrollX ) );
		m_siTempY = 0;
		m_siTempZ = 0;
//		bNowScroll = TRUE;
		return FALSE;

	case ON_FARM_SCROLL_RIGHT_INIT:
//		m_siRight	=	GetTickCount();
		m_siStatus	=	ON_FARM_SCROLL_RIGHT;
//		bNowScroll	=	TRUE;

		//m_BBack.SetX( 675 );
		//m_BOk.SetX( 690 );

		m_siTempX = 0;
		m_siTempY = 0;
		m_siTempZ = 0;

		return FALSE;

	case ON_FARM_SCROLL_RIGHT:		
	{
//		if( m_siRight < (GetTickCount() - 450) )
//		{
			SI16 siXpos	=	ON_SCREEN_XSIZE - m_InterfaceSpr.Header.Xsize;

//			m_BMain01.SetX( siXpos + 85);
//			m_BMain02.SetX( siXpos + 185);
//			m_BMain03.SetX( siXpos + 285);

			m_siStatus	=	m_siNextStatus;
			m_dwStart	=	GetTickCount();
			m_bOpen		=	TRUE;
			bNowScroll	=	FALSE;

			//m_BBack.SetX( 675 );
			//m_BOk.SetX( 690 );

			if(m_siStatus == ON_FARM_ITEM_MAKE || m_siStatus == ON_FARM_WORKER)
				m_pMyOnlineWorld->pOnlineClient->SendRequestWorkInfo( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
			break;
//		}		

		return FALSE;
	}
	case ON_FARM_SCROLL_EXITRIGHT:
	{
		if( m_siNextStatus == ON_FARM_DESTROY_WAIT )			m_siStatus	=	m_siNextStatus;
		else if( m_siNextStatus == ON_FARM_UPGRADE_INPUT )		m_siStatus	=	ON_FARM_UPGRADE_INPUT;
		else if( m_siNextStatus == ON_FARM_SCROLL_EXITLEFT )	m_siStatus	=	ON_FARM_SCROLL_EXITLEFT;
		else if( m_siNextStatus != ON_FARM_POLL )				m_siStatus	=	ON_FARM_SCROLL_RIGHT_INIT;
		else													m_siStatus	=	ON_FARM_POLL;
		
		m_bOpen	= FALSE;
		bNowScroll = FALSE;			
		return FALSE;
	}
	case ON_FARM_SCROLL_EXITLEFT:
		m_siStatus	=	ON_FARM_EXIT;
		return FALSE;

	case ON_FARM_EXIT_WAIT:
		break;
	case ON_FARM_EXIT:
		Free();
		return TRUE;
	}


	if( IpD.LeftPressSwitch )			m_bMouseDown = TRUE;
	else								m_bMouseDown = FALSE;

	return FALSE;
}

///////////////////////////////////////
// 버튼을 생성 함
VOID		OnlineVillageStructFarm::CreateFarm(OnResFarmSlotInfoMsg *pOnResFarmSlotInfoMsg)
{
	SI16	i, ix, iy, siBuidingNum;
	SI16	siIndex;
	SI16	siFont;

	stFacility				*pFacility;

	OnFarmSlotList			*pOnFarmSlotList;
	
	pOnFarmSlotList			=	(OnFarmSlotList*)( pOnResFarmSlotInfoMsg + 1 );	
	siBuidingNum			=	pOnResFarmSlotInfoMsg->siFactoryNum;

	m_pMakeList->SetSlotNum((UI08)pOnResFarmSlotInfoMsg->siMaxConstructibleFactoryNum);

	if( siBuidingNum != 0 )
	{
		for( i=0 ; i<siBuidingNum; i++ )
		{
			siIndex									=	pOnFarmSlotList[i].SlotIndex; //몇번 째 땅인가. 
			m_Building[ siIndex ].m_uiSlotLevel		=	pOnFarmSlotList[i].uiLevel;	 		
			m_Building[ siIndex ].m_uiPay			=	pOnFarmSlotList[i].siPay;
			m_Building[ siIndex ].m_uiWork			=	pOnFarmSlotList[i].uiLabor;

			// pOnFarmSlotList[i].StructCode 이것이 널이 인지 체크
			if( pOnFarmSlotList[i].StructCode[0] != 0 )	//땅에 건물이 있으면
			{
				m_Building[ siIndex ].m_bBuilding =	TRUE;	//있음으로 하고,
				strcpy( m_Building[ siIndex ].m_szKind, pOnFarmSlotList[i].StructCode );	//건물 종류를 저장
			}
		}
	}
	else
	{
		m_bNoBuilding	=	TRUE;
	}


	SI16	siXpos[ ON_MAX_FACTORY_NUM ]   = { 78 , 173, 358, 546, 467, 265 };
	SI16	siYpos[ ON_MAX_FACTORY_NUM ]   = { 389, 280, 234, 281, 389, 426 };
	SI16	siXBuild[ ON_MAX_FACTORY_NUM ] = { 108, 195, 386, 575, 487, 297 };
	SI16	siYBuild[ ON_MAX_FACTORY_NUM ] = { 340, 224, 185, 221, 337, 370 };
	XSPR	*pButtonSpr;
	UI08	*pTemp;

	
	for( i=0 ; i<ON_MAX_FACTORY_NUM; i++ ) //땅 여섯개 중에서,
	{
		m_pCheckButton[i]			=	NULL;
		m_pBuildButton[i]			=	NULL;
		m_Building[i].m_pWorkInfo	=	NULL;		
		m_Building[i].m_bNowBuild	=	FALSE;


		// 건물이 있뜨면
		if( m_Building[i].m_bBuilding )
		{
			//건물 이미지를 로드하고
			m_pBuildButton[i]			=	new UI08 [ m_BuidingSpr.Header.Xsize * m_BuidingSpr.Header.Ysize ];
			pButtonSpr					=	&m_BuidingSpr;
			pTemp						=	m_pBuildButton[i];
			pFacility					=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[i].m_szKind );

			if( pFacility != NULL )	 
			{
				siFont						=	pFacility->siSpriteFont;
				m_Building[i].m_siAniFrame	=	11;
			}
			else	//창고가 아니라면 건물을 지우고 없는 걸로 한다. 
			{
				delete [] m_pBuildButton[i];
				m_pBuildButton[i]			=	NULL;
				m_pCheckButton[i]			=	new UI08 [ m_TitleSpr.Header.Xsize * m_TitleSpr.Header.Ysize ];
				pButtonSpr					=	&m_TitleSpr;
				pTemp						=	m_pCheckButton[i];
				siFont						=	i;
				m_Building[i].m_siAniFrame	=	0;
				m_Building[i].m_bBuilding	=	FALSE;
			}
		}	
		else //건물이 없으면
		{
			m_pCheckButton[i]			=	new UI08 [ m_TitleSpr.Header.Xsize * m_TitleSpr.Header.Ysize ];
			pButtonSpr					=	&m_TitleSpr;
			pTemp						=	m_pCheckButton[i];
			siFont						=	i;
			m_Building[i].m_siAniFrame	=	0;
		}


		if( clGrp.LockSurface( pTemp, pButtonSpr->Header.Xsize, pButtonSpr->Header.Ysize ) == TRUE )
		{
			clGrp.PutImageT( 0, 0, pButtonSpr->Header.Xsize, pButtonSpr->Header.Ysize, 
							&pButtonSpr->Image[ pButtonSpr->Header.Start[ siFont ] ] );

			for( iy = 0 ; iy<pButtonSpr->Header.Ysize ; iy++ )
				for( ix = 0; ix<pButtonSpr->Header.Xsize ; ix++)
				{
					if( pTemp[ iy * pButtonSpr->Header.Xsize + ix ] !=  TRANSCOLOR )
						pTemp[ iy * pButtonSpr->Header.Xsize + ix ]  =	 ON_BUTTON_CHECK_SAMPLE_VALUE;
				}
		}
		

		if( m_Building[i].m_bBuilding == FALSE )				// 건물이 없을때 땅을 그려준다. 
		{

			m_siXpos[i] = siXpos[i];
			m_siYpos[i] = siYpos[i];
			m_Building[i].m_pBuilding.Create( siXpos[i], siYpos[i], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize, 
											m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_VOID ), 
											pTemp, ON_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_LEFT );
		}
		else	// 건물이 있을때 건물을 그려준다. 
		{
			pFacility	= m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[i].m_szKind );
			if( pFacility )
			{
				m_siXpos[i] = siXBuild[i];
				m_siYpos[i] = siYBuild[i];
				m_Building[i].m_pBuilding.Create( siXBuild[i], siYBuild[i], m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ), pTemp, ON_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_LEFT );
			}
		}
	}
}
//////////////////////////////////////
// 날린다.
VOID	OnlineVillageStructFarm::Free()
{
	SI16		i;

	for( i=0 ; i<ON_MAX_FACTORY_NUM; i++ )
	{
		if( m_pCheckButton[i] )			delete [] m_pCheckButton[i];
		if( m_pBuildButton[i] )			delete [] m_pBuildButton[i];			
		if( m_Building[i].m_pWorkInfo)	delete m_Building[i].m_pWorkInfo;

		m_pBuildButton[i] = NULL;
		m_pCheckButton[i] = NULL;
		m_Building[i].m_pWorkInfo = NULL;
	}

	clGrp.FreeXspr( BackImage );
	clGrp.FreeXspr( m_TitleSpr );
	clGrp.FreeXspr( m_MainButtonSpr );
	clGrp.FreeXspr( m_BuidingSpr );
	clGrp.FreeXspr( m_InterfaceSpr );
//	clGrp.FreeXspr( m_BoardSpr );
	clGrp.FreeXspr( m_BackSpr );
	clGrp.FreeXspr( m_OkSpr );
	clGrp.FreeXspr( m_NpcSpr );
	clGrp.FreeXspr( m_CheckSpr );
	clGrp.FreeXspr( m_ItemPannelSpr );
	clGrp.FreeXspr(m_ItemInterfaceSpr);
	clGrp.FreeXspr(m_ItemScrollBase);
	clGrp.FreeXspr(m_ButtonBaseSpr);
	clGrp.FreeXspr(m_CheckLineSpr);
	clGrp.FreeXspr(m_ButtonCheckBttnSpr);
	clGrp.FreeXspr(m_ExitImage);
	clGrp.FreeXspr(m_ExitBackImage);
	clGrp.FreeXspr(m_ScrollUpImage);
	clGrp.FreeXspr(m_ScrollDnImage);

	m_SelectBuild.DelAllSell();
	m_SelectControl.DelAllSell();
	m_SelectMakeMaterial.DelAllSell();
	m_SelectWorkMaterial.DelAllSell();
	m_SelectItemWorkMaterial.DelAllSell();

	if(m_NpcList)
	{
		delete m_NpcList;
		m_NpcList =	NULL;
	}
	
/*	if( m_CheckInterface.m_pEdit01 )
	{
		m_CheckInterface.m_pEdit01->Free();
		delete m_CheckInterface.m_pEdit01;
		m_CheckInterface.m_pEdit01	=	NULL;
	}

	if( m_CheckInterface.m_pEdit02 )
	{
		m_CheckInterface.m_pEdit02->Free();
		delete m_CheckInterface.m_pEdit02;
		m_CheckInterface.m_pEdit02	=	NULL;
	}*/

	Handler_OnlineEditControlEx::KillControl( m_CheckInterface.m_hOecEdit01);
	Handler_OnlineEditControlEx::KillControl( m_CheckInterface.m_hOecEdit02);

	if( m_pItemID )
	{
		delete [] m_pItemID;
		m_pItemID = NULL;
	}	
}

/////////////////////////////////////
// 메인 폴
BOOL	OnlineVillageStructFarm::Poll(LPDIRECTDRAWSURFACE7 pSurface) 
{
	HDC		hDC;
	SI16	i;
	SI32	siBackImageFont;

	if( bNowScroll )
	{
		ScrollDraw( pSurface, m_siTempX, m_siTempY, m_siTempZ );
		return FALSE;
	}

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		stFacility		*pFacility;
		SI16			siTempFont;

		if( pMyOnlineWorld->IsNight() == TRUE )
		{
			// 현재 밤이라면
			siBackImageFont		=	1;				
			
		}
		else
		{
			// 현재 낮이라면
			siBackImageFont		=	0;					
		}

		// 해당 폰트의 그림이 있는지 최종 검사한다.
		if( siBackImageFont >= BackImage.Header.font_number )	siBackImageFont	=	0;


		clGrp.PutSpriteT( 0, 0, BackImage.Header.Xsize, BackImage.Header.Ysize, &BackImage.Image[ BackImage.Header.Start[ siBackImageFont ] ] );		

		for( i=0 ; i<ON_MAX_FACTORY_NUM; i++ )
		{	
			if( m_Building[i].m_bBuilding == FALSE )
			{
				if( m_Building[i].m_bSelect || m_Building[i].m_pBuilding.IsInMouse() == TRUE )
					m_Building[i].m_pBuilding.Put( &m_TitleSpr, i, i, i, BUTTON_PUT_NOMOVE );
			}
			else
			{
				pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[i].m_szKind );

				if( pFacility )
				{
					if( stricmp( pFacility->Code, "STG" ) == 0 )
					{
						siTempFont	=	pFacility->siSpriteFont;
					}
					else
					{
						if( m_Building[i].m_uiSlotLevel != 0 )	siTempFont	=	pFacility->siSpriteFont + m_Building[i].m_uiSlotLevel - 1;
						else									siTempFont	=	pFacility->siSpriteFont;
					}

					m_Building[i].m_pBuilding.Put( &m_BuidingSpr, siTempFont, siTempFont, siTempFont, BUTTON_PUT_NOMOVE );
				}
			}
		}

		clGrp.PutSpriteT(686, 469, m_ExitBackImage.Header.Xsize, m_ExitBackImage.Header.Ysize, m_ExitBackImage.Image);
		m_BExit.Put(&m_ExitImage, 0, 0, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode(hDC, TRANSPARENT );
		::SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_NUMBERC));
		m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC( hDC );
	}


	switch( m_siStatus )
	{
	case ON_FARM_BUILD_STRUCT:
		pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE, TRUE);
		BuildDraw(pSurface);		
		break;
	case ON_FARM_ITEM_MAKE:
		pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE, TRUE);
		ItemMakeDraw(pSurface);	
		break;
	case ON_FARM_STOREHOUSE:
		pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE, TRUE);
		StorehouseDraw(pSurface);	
		break;
	case ON_FARM_WORKER:
		pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE, TRUE);
		WorkDraw(pSurface);		
		break;
	default:
		pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, FALSE, TRUE);
		break;
	}

	if( m_bMiniGame )				return FALSE;

	// 장부
	if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
//		m_pMyOnlineWorld->pOnlineTradeBook->Poll();
//		m_pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	}

	// 용병
	if(m_pMyOnlineWorld->m_pMercenary->IsAction())
		m_pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));


	// 인벤토리
	if(m_pMyOnlineWorld->pOnlineInventory->IsActive())
	{
		if( m_siStatus == ON_FARM_STOREHOUSE )
		{
			POINT point;

			point.x = IpD.Mouse_X;
			point.y = IpD.Mouse_Y;

			if( m_NpcList->GetDragSlot() != -1 )
			{	
				m_pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
				m_NpcList->Draw( GETSCREENSURFACE(m_pMyOnlineWorld) );
			}
			else if( m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1 )
			{
				m_NpcList->Draw( GETSCREENSURFACE(m_pMyOnlineWorld) );
				m_pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
			}
			else
				m_pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
		}
		else
			m_pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	}

	// Pannel을 출력한다.
	if(m_siStatus != ON_FARM_STOREHOUSE)
		m_pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_INVENTORY | ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_MINIMAP | ONLINEPANNEL_DISABLEFLAG_CHAT | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON | ONLINEPANNEL_DISABLEFLAG_PARTY | ONLINEPANNEL_DISABLEFLAG_FRIEND | ONLINEPANNEL_DISABLEFLAG_STATUS | ONLINEPANNEL_DISABLEFLAG_TRADEBOOK);
	else
		m_pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_MINIMAP | ONLINEPANNEL_DISABLEFLAG_CHAT | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON | ONLINEPANNEL_DISABLEFLAG_PARTY | ONLINEPANNEL_DISABLEFLAG_FRIEND | ONLINEPANNEL_DISABLEFLAG_STATUS | ONLINEPANNEL_DISABLEFLAG_TRADEBOOK);

	CheckDraw( GETSCREENSURFACE(m_pMyOnlineWorld) );

	m_pMyOnlineWorld->pOnlineMsgBoxError->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	
	return FALSE;
}

///////////////////////////////////////////////////
// 스크롤
VOID	OnlineVillageStructFarm::ScrollDraw(LPDIRECTDRAWSURFACE7 pSurface, SI16 x, SI16 y, SI16 z)
{
	HDC				hDC;
	SI16			siXpos, siYpos, i, siTempFont;
	stFacility		*pFacility;
	CHAR			szTemp[100];

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT( 0, 0, BackImage.Header.Xsize, BackImage.Header.Ysize, BackImage.Image );		

		for( i=0 ; i<ON_MAX_FACTORY_NUM; i++ )
		{
			if( m_Building[i].m_bBuilding )
			{
				pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[i].m_szKind );

				if( pFacility )
				{
					if( strcmp( pFacility->Code, "STG" ) != 0 )	
					{
						if( m_Building[i].m_uiSlotLevel != 0 )	siTempFont	=	pFacility->siSpriteFont +
																				m_Building[i].m_uiSlotLevel - 1;
						else									siTempFont	=	pFacility->siSpriteFont;
					}
					else										siTempFont	=	pFacility->siSpriteFont;

					m_Building[i].m_pBuilding.Put( &m_BuidingSpr, siTempFont, siTempFont, siTempFont, BUTTON_PUT_NOMOVE );
				}
			}
		}

		if( m_siStatus == ON_FARM_SCROLL_RIGHT )
		{
			siXpos	=	ON_SCREEN_XSIZE - z;
			siYpos	=	0;
			
			clGrp.PutSpriteT( siXpos + 15, 0, m_InterfaceSpr.Header.Xsize, m_InterfaceSpr.Header.Ysize, &m_InterfaceSpr.Image[ m_InterfaceSpr.Header.Start[m_siShowFont]]);

			if( m_pMakeList->GetMaster() )
			{
				m_BMain03.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
				m_BMain02.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			}

			m_BBack.Put( &m_BackSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}

		else if( m_siStatus == ON_FARM_SCROLL_EXITRIGHT )
		{
			siXpos	=	ON_SCREEN_XSIZE - m_InterfaceSpr.Header.Xsize - z;
			siYpos	=	0;
			
			clGrp.PutSpriteT( siXpos + 15, 0, m_InterfaceSpr.Header.Xsize, m_InterfaceSpr.Header.Ysize, 
								&m_InterfaceSpr.Image[ m_InterfaceSpr.Header.Start[m_siShowFont]] );



			if( m_pMakeList->GetMaster() )
			{
				m_BMain03.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
				m_BMain02.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			}
			m_BOk.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BBack.Put( &m_BackSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}
		
		clGrp.PutSpriteT(686, 469, m_ExitBackImage.Header.Xsize, m_ExitBackImage.Header.Ysize, m_ExitBackImage.Image);
		m_BExit.Put(&m_ExitImage, 0, 0, 1, BUTTON_PUT_NOMOVE );
		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		VillageHeader	*pTempVillageHeader	=	m_pMyOnlineWorld->pOnlineVillage->GetVillageHeader();

		SI16	Tempxpos	=	x == 0 ? 0 : x - 1;

		sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_POS ), m_pMyOnlineWorld->pOnlineText->Get( pTempVillageHeader->siVillageName ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, Tempxpos + 29, 60, szTemp, RGB( 10, 10, 10 ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, Tempxpos + 28, 59, szTemp, RGB( 150, 150, 150 ) );


		sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_MASTER_NAME ), m_pMakeList->GetMakeList()->m_szID );
		m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, Tempxpos + 29, 80, szTemp, RGB( 10, 10, 10 ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, Tempxpos + 28, 79, szTemp, RGB( 150, 150, 150 ) );

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, Tempxpos + 25, 13, 117, 15, m_pMakeList->GetMakeList()->m_szFarmName, RGB( 10, 10, 10 ) );
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, Tempxpos + 24, 12, 117, 15, m_pMakeList->GetMakeList()->m_szFarmName, RGB( 210, 210, 210 ) );


		if( m_siStatus == ON_FARM_SCROLL_EXITRIGHT )
		{
			if( m_pMakeList->GetMaster() )
			{
				m_BMain03.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_BMain02.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}

			m_BBack.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BOk.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		else if( m_siStatus == ON_FARM_SCROLL_RIGHT )
		{
			if( m_pMakeList->GetMaster() )
			{
				m_BMain03.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_BMain02.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}
		}

		::SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_NUMBERC));
		m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC( hDC );
	}

	// 장부
	if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
		m_pMyOnlineWorld->pOnlineTradeBook->Poll();
		m_pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	}

	// 미니맵
	if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive())
	{
		m_pMyOnlineWorld->pOnlineMiniMap->Pool();
		m_pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	}

	// 용병
	if(m_pMyOnlineWorld->m_pMercenary->IsAction())
	{
		m_pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	}

	if(m_pMyOnlineWorld->pOnlineInventory->IsActive())
	{
		m_pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(m_pMyOnlineWorld));
	}

	// Pannel을 출력한다.
	m_pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_INVENTORY | ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_MINIMAP | 
													ONLINEPANNEL_DISABLEFLAG_CHAT | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON | ONLINEPANNEL_DISABLEFLAG_FRIEND | ONLINEPANNEL_DISABLEFLAG_PARTY | ONLINEPANNEL_DISABLEFLAG_STATUS | ONLINEPANNEL_DISABLEFLAG_TRADEBOOK);
}

////////////////////////////////////////////////
// 건설 액션
VOID	OnlineVillageStructFarm::BuildAction()
{
	SI16			i;
	SI32			siTempSelect;
	stFacility		*pFacility;
	stPlants		*pPlants;
	CHAR			szTemp[256];
	SI16			siTemp;

	switch( m_siBuildStatus )
	{ 
	case ON_FARM_BUILD_STRUCT_INIT:
		// 건물을 알았으면 해당하는 건물의 정보를 얻는다.
		pPlants		=	m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );
		
		// 그 건물에서 지을수 있는 정보를 얻어 온다.
		memcpy( m_szBuildCode, pPlants->FCode, sizeof( m_szBuildCode ) );

		// 리스트 컨트롤을 초기화
		m_SelectBuild.DelAllSell();

		// 건물에서 지을수 있는 정보를 얻었다면 리스트에 건물 이름을 너어 준다.
		for( i=0; i<pPlants->siBuildNum; i++ )
		{
			pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_szBuildCode[i] );
			sprintf( szTemp, "%d. %s", i+1, m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ) );
			m_SelectBuild.AddSell( (LPVOID)i, szTemp );
		}	

		m_BBack.SetAction( TRUE );
		//m_BOk.SetX(685);
		//m_BOk.SetY(425);
		m_BOk.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_BUILD ) );

		m_siBuildSelect		=	-1;
		m_siBuildStatus		=	ON_FARM_BUILD_STRUCT_POLL;

//		m_BMain01.SetAction( FALSE );
		m_BMain02.SetAction( FALSE );
		m_BMain03.SetAction( FALSE );
		m_BExit.SetAction( FALSE );
		break;

	case ON_FARM_BUILD_STRUCT_POLL:

		m_SelectBuild.Action(IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &siTempSelect, FALSE );
		if(m_BScrBuildUp.Process(m_bMouseDown))
		{
			m_SelectBuild.PrevSell();
		}
		else if(m_BScrBuildDn.Process(m_bMouseDown))
		{
			m_SelectBuild.NextSell();
		}
		if( m_BBack.Process( m_bMouseDown ) == TRUE && m_pMyOnlineWorld->pOnlineInventory->IsActive() == FALSE )
		{
			if(m_bInFactory)
				m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();
			m_BBack.SetAction( FALSE );
			m_siBuildStatus		=	ON_FARM_BUILD_STRUCT_EXIT;
			break;
		}

		if( LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )
		{
			if(m_bInFactory)
				m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();
			m_BBack.SetAction( FALSE );
			m_siBuildStatus		=	ON_FARM_BUILD_STRUCT_EXIT;
			break;
		}

/*		if( LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN) ) )
		{
			if( m_siBuildSelect != -1 && m_siSelect != -1 )
			{
				SetShowCheckBox();
				m_siBuildStatus	= ON_FARM_BUILD_STRUCT_INPUT;				
			}
		}
*/
		if( m_BOk.Process( m_bMouseDown ) == TRUE )
		{
			if( m_siBuildSelect != -1 && m_siSelect != -1 )
			{
				SetShowCheckBox();
				m_siBuildStatus	= ON_FARM_BUILD_STRUCT_INPUT;				
			}
		}		
	
		// 선택 되어 있는것이 없다면 건설 버튼을 비활성화
		if( m_siBuildSelect == -1 )			m_BOk.SetAction( FALSE );

		// 무슨 건물을 선택을 했는가?
		m_SelectBuild.Action( IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &siTempSelect );

		if( siTempSelect != -1 )
		{
			m_siBuildSelect	= siTempSelect;

			// 만약 건물이 전혀 없다면 창고를 지어야 한다.
			if( m_bNoBuilding )
			{
				m_siTalk		=	ON_TEXT_MAKE_TALK01;
				pFacility		=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_szBuildCode[siTempSelect] );
				m_dwDelayTime	=	timeGetTime();

				if( stricmp( pFacility->Code, "STG" ) == 0 )	m_BOk.SetAction( TRUE );
				else
				{
					m_BOk.SetAction( FALSE );
				}
			}			
			else
			{
				m_BOk.SetAction( TRUE );
				m_dwDelayTime	= timeGetTime();

				if( stricmp( m_szBuildCode[siTempSelect], "STG" ) == 0 )		m_siTalk = ON_TEXT_MAKE_TALK01;
				else if( stricmp( m_szBuildCode[siTempSelect], "FRU" ) == 0 )	m_siTalk = ON_TEXT_MAKE_TALK03;
				else if( stricmp( m_szBuildCode[siTempSelect], "MIL" ) == 0 )	m_siTalk = ON_TEXT_MAKE_TALK04;
				else if( stricmp( m_szBuildCode[siTempSelect], "LND" ) == 0 )	m_siTalk = ON_TEXT_MAKE_TALK02;
			}
		}
		break;

	case ON_FARM_BUILD_STRUCT_INPUT:
		m_BOk.SetAction( FALSE );
		siTemp			=	CheckAction();		
		m_siPay			=	_atoi64( m_CheckInterface.m_szEdit02 );

		// 오케이
		if( siTemp == 1 )
		{
			pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_szBuildCode[ m_siBuildSelect ] );
			if(pFacility->UpLabor[0] * (m_siPay + ON_FARM_TAX_RATE) <= m_pMyOnlineWorld->pMyData->GetMoney())
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequestBuildStruct( (SI16)m_siPay, m_siSelect, m_szBuildCode[ m_siBuildSelect ] ); 
				m_Building[ m_siSelect ].m_uiWork	=	pFacility->UpLabor[0];
				m_siBuildStatus = ON_FARM_BUILD_STRUCT_WAIT;
			}
			else
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_NOMONEY) );
				m_siBuildStatus = ON_FARM_BUILD_STRUCT_POLL;
			}
		}
		else if( siTemp == 0 )
		{
			m_siBuildStatus = ON_FARM_BUILD_STRUCT_POLL;
		}
		break;

	case ON_FARM_BUILD_STRUCT_WAIT:	
		// FarmStructBuild()
		break;

	case ON_FARM_BUILD_STRUCT_EXIT:
		{
			if(m_bExitOk || !m_bInFactory)
			{
				m_siStatus		=	ON_FARM_SCROLL_EXITRIGHT;		
				m_siNextStatus	=	ON_FARM_POLL;
				m_siBuildStatus	=	ON_FARM_BUILD_STRUCT_INIT;
//				m_siRight		=	GetTickCount();
				pPlants			=	m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );
				m_siBuildSelect	=	-1;
				m_siTalk		=	0;
				
				for( i=0 ; i<ON_MAX_FACTORY_NUM; i++ )		m_Building[i].m_bSelect = FALSE;
				m_SelectBuild.DelAllSell();
				m_BBack.SetAction( FALSE );
				m_BExit.SetAction( TRUE );
				
//				m_BMain01.SetAction( TRUE );
				m_BMain03.SetAction( TRUE );
				m_BMain02.SetAction( TRUE );
				m_bInFactory	= FALSE;
				m_bExitOk		= FALSE;
			}
		}
		break;
	}
}

////////////////////////////////////////////////
// 건설 드로우
VOID	OnlineVillageStructFarm::BuildDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	SI16			siTemp;
	HDC				hDC;
	stFacility		*pFacility;	

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		
	 	siTemp = ON_SCREEN_XSIZE - m_InterfaceSpr.Header.Xsize;

		clGrp.PutSpriteT(m_siX, m_siY, m_InterfaceSpr.Header.Xsize, m_InterfaceSpr.Header.Ysize, &m_InterfaceSpr.Image[ m_InterfaceSpr.Header.Start[ON_FARM_SHOW_BUILD_STRUCT]] );
		
		// actdoll (2004/01/06 17:08) : 다른 방법으로 NPC 초상그리기
		m_pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 45, NPC_KIND_FARM, m_pMyOnlineWorld->pOnlineVillage->GetNation() );
		
		// 선택한 건물이 있다면 해당 건물을 출력을 해준다.
		if( m_siBuildSelect > -1 )
		{
			pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_szBuildCode[m_siBuildSelect] );
			clGrp.PutSpriteT( m_siX + 59 +  (246 - m_BuidingSpr.Header.Xsize ) / 2, m_siY + 165 + (( 146 - m_BuidingSpr.Header.Ysize ) /2 ) , m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize, &m_BuidingSpr.Image[ m_BuidingSpr.Header.Start[ pFacility->siSpriteFont ] ] );
		}

		m_BScrBuildUp.Put( &m_ScrollUpImage, 0, 2, 1, BUTTON_PUT_NOMOVE ); 
		m_BScrBuildDn.Put( &m_ScrollDnImage, 0, 2, 1, BUTTON_PUT_NOMOVE ); 
		
//		m_BMain01.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
//		m_BMain02.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
//		m_BMain03.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );	

		m_BOk.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BBack.Put( &m_BackSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_BBack.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BOk.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		DWORD	dwTemp	=	timeGetTime();

		// 일정 시간이 지나면 멘트를 지워 준다.
		if( ( ( dwTemp - m_dwDelayTime ) / 4000 ) > 10 )	
			m_siTalk = 0;

		// 상인의 멘트
		if( m_siTalk )
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2( hDC, m_siX + 109, m_siY + 70, 402, m_pMyOnlineWorld->pOnlineText->Get( m_siTalk ) );					

		pSurface->ReleaseDC( hDC );
	}

	m_SelectBuild.Draw( pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ), RGB(150, 150, 150), RGB(128, 128, 128), TRUE, RGB(0, 255, 0), RGB(200, 200, 200));
}

////////////////////////////////////////////////
// 창고 액숀
VOID	OnlineVillageStructFarm::StoreAction()
{
	SI16				siIndex;
	POINT				pt;
	OnlineListControl	*pTempList;
	CItemHeader			*pItem;

	switch( m_siBuildStatus )
	{
	case ON_FARM_BUILD_STOREHOUSE_INIT:
		// FarmStorageInfo()
		break;

	case ON_FARM_BUILD_STOREHOUSE_POLL:
		if( m_BSubExit.Process( m_bMouseDown ) == TRUE || m_pMyOnlineWorld->pOnlineInventory->IsActive() == FALSE )
		{
			if(!m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
			{
				if(m_bInFactory)
					m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();
				m_BBack.SetAction( FALSE );
				m_siBuildStatus	=	ON_FARM_ITEM_MAKE_EXIT;
				break;
			}
		}

		else if( m_BDestroy.Process(m_bMouseDown) == TRUE ) // 제거 버튼을 누르면 확인 메시지박스를 띄운다.
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OKCANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHEREHOUSE),  m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_RWHEREHOUSEDESTORY));
			m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_DESTROY;
			break;
		}

		if( LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )
		{
			if(m_bInFactory)
				m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();
			m_BBack.SetAction( FALSE );
			m_siBuildStatus		=	ON_FARM_ITEM_MAKE_EXIT;
			break;
		}

		// 창고에서 인벤토리는 항상 활동중이다.
		if( m_pMyOnlineWorld->pOnlineInventory->IsActive() == FALSE )	
			m_pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );

		m_pMyOnlineWorld->pOnlineInventory->Action();

		pt.x	=	IpD.Mouse_X;
		pt.y	=	IpD.Mouse_Y;		
		siIndex	=	m_NpcList->GetSelectSlot();

		pItem = NULL;
		if( siIndex != -1 )
		{
			if(m_NpcList->GetSlotInfo(siIndex))
				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_NpcList->GetSlotInfo(siIndex)->uiItemID );

			if( pItem )	m_pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo *>(m_NpcList->GetSlotInfo(siIndex)), (SI16)pt.x, (SI16)pt.y, ON_HELP_DOWN, FALSE, FALSE);
		}

		// 아이템을 드래그 업 한다.
		if( m_siMouseStatus == 0 && IpD.LeftPressSwitch)
		{
			pTempList	=	m_pMyOnlineWorld->pOnlineInventory->GetLCInventory();
			
			if( pTempList->DragUp() )
			{
				m_siMouseStatus = 1;
				m_pTempDragList = pTempList;
			}
			else if(m_NpcList->DragUp())		
			{
				m_siMouseStatus = 2;
				m_pTempDragList = m_NpcList;
			}

			if(m_pMyOnlineWorld->pOnlineInventory->IsWearDrag())
			{
				pTempList->StopDrag();
				m_siMouseStatus = 0;
			}

			m_dwDelayTime = timeGetTime();
		}

		// 아이템 삽입 & 드래그 다운
		else if(m_siMouseStatus == 1 || m_siMouseStatus == 2)
		{
			DWORD	dwTemp	=	timeGetTime();

			pTempList		=	m_pMyOnlineWorld->pOnlineInventory->GetLCInventory();						// 인벤토리의 리스트 컨트롤 받는다.

			// 일정 시간을 기달린다.
			if( ( dwTemp - m_dwDelayTime ) >= 250 && IpD.LeftPressSwitch)
			{
				// 창고로 아이템을 삽입
				if(PtInRect( &m_rtNpcRect, pt ) && (m_siMouseStatus == 1))
				{
					m_siMouseStatus = 0;

					SI16 siSlot = m_pTempDragList->GetDragSlot();
					if(m_pTempDragList->GetSlotInfo(siSlot))
					{
						m_uiItemID		= m_pTempDragList->GetSlotInfo(siSlot)->uiItemID;
						
						if( m_uiItemID )
						{
							// 시작 //////////////////////////////////////
							// robypark 2004/11/26 12:3
							// 공성전 소집권 아이템은 창고에 저장할 수 없다.
							CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(m_uiItemID);

							if (pItemHeader)
							{
								// 소집권 아이템인지 검사
								if (NULL != pItemHeader->m_pImperium)
								{
									m_siBuildStatus = ON_FARM_BUILD_STOREHOUSE_POLL;
									m_pTempDragList->StopDrag();
									m_siSelectLB = 0;

									break;
								}
							}
							// 끝 //////////////////////////////////////

							m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_INPUT;
							m_siYetPos		=	siSlot;
							m_siSelectLB	=	(SI16)m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( pTempList->GetSlotInfo(pTempList->GetDragSlot())->ItemCount );
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( IpD.Mouse_X, IpD.Mouse_Y );
							
							m_pMyOnlineWorld->m_pMercenary->NoAction( TRUE );
						}
					}

					break;
				}
				else if(PtInRect( &m_rtNpcRect, pt ) && (m_siMouseStatus == 2))
				{
					m_siMouseStatus = 0;
					m_NpcList->StopDrag();
					IpD.LeftPressSwitch = 0;
					break;
				}

				// 내 인벤토리에 삽입을 한다.
				else if(PtInRect( &m_rtPlayerRect, pt ) && (m_siMouseStatus == 2))				
				{
					m_siMouseStatus = 0;

					SI16 siSlot = m_NpcList->GetDragSlot();
					if(!m_NpcList->GetSlotInfo(siSlot))		break;

					m_uiItemID		= 0;
					m_uiItemID		= m_NpcList->GetSlotInfo(siSlot)->uiItemID;

					if( m_uiItemID )
					{
						// 무게를 알아 보고 남은 무게 만큼 아이템을 제한 한다.
						m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_INPUT;

						// 무게를 알아 보고 남은 무게 만큼 아이템을 제한 한다.
						m_siSelectLB = (SI16)m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
						UI32 uiTempQuantity = min(m_NpcList->GetSlotInfo(siSlot)->ItemCount, m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake((UI08)m_siSelectLB, m_uiItemID));						
						if(uiTempQuantity > 0)
						{
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(uiTempQuantity);
							m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(IpD.Mouse_X, IpD.Mouse_Y);
						}
						else
						{
							m_siSelectLB = 0;
							m_pTempDragList->StopDrag();
							m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_POLL;
						}
						m_pMyOnlineWorld->m_pMercenary->NoAction( TRUE );
					}					
					break;
				}
			}
		}

		break;

	case ON_FARM_BUILD_STOREHOUSE_DESTROY:
		{
			// 사용자가 오케이 눌름
			if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK )
			{	
				m_pMyOnlineWorld->pOnlineClient->SendRequesDestroyStrcut( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
				m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_DESTROY_OK;
			}

			else if( m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL )
			{
				m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_POLL;

			}
		}
		break;
		
	case ON_FARM_BUILD_STOREHOUSE_DESTROY_OK:		
		{
			// FarmStructDestroy()
			if(m_bExitOk || !m_bInFactory)
			{
				SI16	siXpos[ ON_MAX_FACTORY_NUM ]   = { 78, 173, 358, 546, 467, 265 };
				SI16	siYpos[ ON_MAX_FACTORY_NUM ]   = { 389, 280, 234, 281, 389, 426 };
				SI16	iy, ix;
				
				m_Building[ m_siSelect ].m_bBuilding		=	FALSE;
				m_Building[ m_siSelect ].m_siBuilingName	=	0;
				m_Building[ m_siSelect ].m_siAniFrame		=	0;
				m_siXpos[ m_siSelect ]						=	siXpos[ m_siSelect ];
				m_siYpos[ m_siSelect ]						=	siYpos[ m_siSelect ];
				
				
				if( m_pBuildButton[m_siSelect] )	delete [] m_pBuildButton[m_siSelect];
				if( m_pCheckButton[m_siSelect] )	delete [] m_pCheckButton[m_siSelect];
				
				
				m_pBuildButton[m_siSelect] = NULL;
				m_pCheckButton[m_siSelect] = NULL;
				m_pCheckButton[m_siSelect] = new UI08[ m_TitleSpr.Header.Xsize * m_TitleSpr.Header.Ysize ];
				
				
				if( clGrp.LockSurface( m_pCheckButton[m_siSelect], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize ) )
				{
					clGrp.PutImageT( 0, 0, m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize, &m_TitleSpr.Image[ m_TitleSpr.Header.Start[ m_siSelect ] ] );
					
					for( iy = 0 ; iy<m_TitleSpr.Header.Ysize ; iy++ )
						for( ix = 0; ix<m_TitleSpr.Header.Xsize ; ix++)
						{
							if( m_pCheckButton[m_siSelect][ iy * m_TitleSpr.Header.Xsize + ix ] !=  TRANSCOLOR )
								m_pCheckButton[m_siSelect][ iy * m_TitleSpr.Header.Xsize + ix ]  =  ON_BUTTON_CHECK_SAMPLE_VALUE;
						}
						
						m_Building[ m_siSelect ].m_pBuilding.Create( siXpos[ m_siSelect ], siYpos[ m_siSelect ], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize,
							m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_VOID ), m_pCheckButton[m_siSelect], ON_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_LEFT );
				}
				m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_EXIT;
				
				// 건물에 들어갔음 설정
				m_bInFactory = FALSE;
				m_bExitOk = FALSE;	

				m_BExit.SetAction( TRUE );
			}
		}
		break;

	case ON_FARM_BUILD_STOREHOUSE_WAIT:
		// FarmStorageItemInput()
		break;

	case ON_FARM_BUILD_STOREHOUSE_INPUT:
		if( m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && m_pTempDragList )
		{
			m_pMyOnlineWorld->m_pMercenary->NoAction( FALSE );

			if( m_pMyOnlineWorld->pOnlineNumPad->siCount == 0 )	
			{
				m_siBuildStatus = ON_FARM_BUILD_STOREHOUSE_POLL;
				m_siMouseStatus	= 0;
				m_pTempDragList->StopDrag();
				break;
			}

			UI08  uiPlantID			=	0;
			m_uiQuantity			=	(UI32)m_pMyOnlineWorld->pOnlineNumPad->siCount;
			m_siBuildStatus			=	ON_FARM_BUILD_STOREHOUSE_WAIT;
			m_siMouseStatus			=	0;

	
			// 아이템을 넣을 수 가 있다면
			// 지금 선택 되어 있는 캐릭터 슬롯을 알아 낸다.
			if( strcmp( m_szKind, ON_VIL_ST_KIND_CODE_FARM ) == 0 )		uiPlantID	=	ON_PLANT_FARM;

			// 창고로 아이템 삽입
			if( m_pTempDragList == m_pMyOnlineWorld->pOnlineInventory->GetLCInventory() )
			{
				m_siPos = m_NpcList->GetEmptySlot(m_uiItemID);
				
				// 서버로 보낸다.
				if( m_siPos != -1 )
				{	
					m_pMyOnlineWorld->pOnlineClient->SendRequestStorageInPut( uiPlantID, (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect, m_uiQuantity,  (UI08)m_siSelectLB, m_siYetPos, (UI16)m_uiItemID );
				}
				else
				{
					m_siBuildStatus = ON_FARM_BUILD_STOREHOUSE_POLL;
					m_pTempDragList->StopDrag();
					m_siSelectLB = 0;
				}
			}

			// 내 인벤토리로 아이템 삽입
			else if( m_pTempDragList ==  m_NpcList)
			{
				m_siPos = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(m_uiItemID);

				// 서버로 보낸다.
				if( m_siPos != -1 )
				{
					m_pMyOnlineWorld->pOnlineClient->SendRequestOutPut( uiPlantID, (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect, (UI08)m_siSelectLB, (UI16)m_uiItemID, m_uiQuantity, m_siPos );
				}
				else
				{
					m_siBuildStatus = ON_FARM_BUILD_STOREHOUSE_POLL;
					m_pTempDragList->StopDrag();
					m_siSelectLB = 0;
				}
			}
		}
		break;
		
	case ON_FARM_BUILD_STOREHOUSE_EXIT:
		{
			if(m_bExitOk || !m_bInFactory)
			{
				m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
				m_pMyOnlineWorld->pOnlineInventory->SetInStruct( FALSE );
				
				m_BExit.SetAction( TRUE );
				m_BBack.SetAction( FALSE );
				
				m_siStatus		= ON_FARM_POLL;
				m_siSelect		= -1;
				m_siBuildSelect	= -1;
				m_bInFactory	= FALSE;
				m_bExitOk		= FALSE;
			}
		}
		break;
	}
}

////////////////////////////////////////////////
// 창고 드로우
VOID	OnlineVillageStructFarm::StorehouseDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC			hDC;

	if( m_siBuildStatus == ON_FARM_BUILD_STOREHOUSE_INIT )	return;

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT( m_siX - 40, m_siY, m_NpcSpr.Header.Xsize, m_NpcSpr.Header.Ysize, m_NpcSpr.Image );

		clGrp.PutSpriteT( m_siX + 10 +  (246 - m_BuidingSpr.Header.Xsize ) / 2, m_siY + 50 + (( 146 - m_BuidingSpr.Header.Ysize ) /2 ) , 
			m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize, &m_BuidingSpr.Image[ m_BuidingSpr.Header.Start[ 9 ] ] );

		m_BDestroy.Put( &m_OkSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
		m_BSubExit.Put( &m_BackSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );		

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_BDestroy.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BSubExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hDC );
	}
	
	m_NpcList->Draw(pSurface);
}

////////////////////////////////////////////////
// 아이템 만들기 액션
VOID	OnlineVillageStructFarm::ItemMakeAction()
{
	SI16			siTemp;
	SI32			TempSelect, TempSelect1;
	stFacility		*pFacility;	
	CItemHeader		*pItem;
	BOOL			bChangeSelect = FALSE;

	switch( m_siBuildStatus )
	{
	case ON_FARM_ITEM_MAKE_INIT:
		// FarmProductInfo()
		break;

	case ON_FARM_ITEM_MAKE_POLL:
		if(m_Building[ m_siSelect ].m_uiSlotLevel == 3)		m_BMain02.SetAction(FALSE);

		if(m_BScrUp.Process(m_bMouseDown) == TRUE)
		{
			m_SelectMakeMaterial.PrevSell();
		}
		if(m_BScrDn.Process(m_bMouseDown) == TRUE)
		{
			m_SelectMakeMaterial.NextSell();
		}
		if(m_BScr2Up.Process(m_bMouseDown) == TRUE)
		{
			m_SelectControl.PrevSell();
		}
		if(m_BScr2Dn.Process(m_bMouseDown) == TRUE)
		{
			m_SelectControl.NextSell();
		}
		// 돌아가기
		if(m_BBack.Process( m_bMouseDown ) == TRUE || LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
		{
			if(m_bInFactory)
				m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();
			m_BBack.SetAction( FALSE );
			m_siBuildStatus	=	ON_FARM_ITEM_MAKE_EXIT;
			break;
		}

		/*
		//물품생산 클릭시
		else if( m_BMain01.Process( m_bMouseDown ) == TRUE ) 
		{
			if( m_siStatus != ON_FARM_ITEM_MAKE && m_siSelect != -1 ) //물품생산 버튼이 두번 눌리면 그대로 로딩. 메시지 주고 받지 않고.
			{
				stFacility	*pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
				
				if( pFacility )
				{
					if( strcmp( pFacility->Code, "STG" ) != 0 )
					{				
						m_siStatus		=	m_bOpen == TRUE ? ON_FARM_SCROLL_EXITRIGHT : ON_FARM_SCROLL_RIGHT_INIT;
						m_siNextStatus	=	ON_FARM_ITEM_MAKE;
						m_siBuildStatus	=	ON_FARM_ITEM_MAKE_INIT;
						m_siShowFont	=	ON_FARM_SHOW_ITEM_MAKE;
//						m_siRight		=	GetTickCount();
						m_bMouseDown	=	FALSE;
						m_pTitleName	=	m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_ITEMMAKE );
						
						// 이 건물이 현재 생산 중인가 알아 본다.
//						m_pMyOnlineWorld->pOnlineClient->SendRequestWorkInfo( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
					}
				}
			}
			break;
		}
		*/
		
		//시설 철거 
		else if( m_BMain03.Process( m_bMouseDown ) == TRUE )
		{ 
			// 건물 철거
			m_bMouseDown	=	FALSE;
			if( m_siSelect != -1 )
			{
				if( m_siStatus != ON_FARM_POLL )
				{
					m_siStatus		=	ON_FARM_SCROLL_EXITRIGHT;
					m_siNextStatus	=	ON_FARM_DESTROY_WAIT;
				}
				
				//확인/취소 메시지 박스를 띄운다. 
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_RWHEREHOUSEDESTORY));
				m_siStatus	=	ON_FARM_DESTROY;
			}
			break;
		}
		
		//시설 향상
		else if( m_BMain02.Process( m_bMouseDown ) == TRUE )
		{
			// 슬롯 업글
			// Skill(2,3단계 건물건설)
			if( m_siSelect != -1 )
			{
				if( m_siStatus != ON_FARM_POLL )
				{
					// Slot Grade 확인
					if(m_Building[ m_siSelect ].m_uiSlotLevel > 2)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NOTSLOTUPGRADE));
						m_siStatus							 = ON_FARM_POLL;
					}
					else
					{
						m_siStatus		=	ON_FARM_SCROLL_EXITRIGHT;
						m_siNextStatus	=	ON_FARM_UPGRADE_INPUT;
						m_bMouseDown	=	FALSE;
						m_pTitleName	=	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTUPGRADE);				
						SetShowCheckBox();		
					}
				}			
			}
			break;
		}

		if( m_BMain03.IsInMouse() )
		{
			CHAR		szTemp[ 512 ];
			stFacility	*pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
			
			ZeroMemory( szTemp, sizeof( szTemp ) );		
			
			if( pFacility )		
			{
				sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_SLOT_DESTORY ), m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ) );
				m_pMyOnlineWorld->pOnlineHelp->SetText( m_BMain03.GetMenuName(), szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_DOWN, ON_HELP_WHITE );
			}
			break;
		}
		else if( m_BMain02.IsInMouse() )
		{
			CHAR		szTemp[ 512 ];
			stFacility	*pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
			
			ZeroMemory( szTemp, sizeof( szTemp ) );		
			
			if( pFacility )
			{
				sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_SLOT_EXTENSION ), m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ) );
				m_pMyOnlineWorld->pOnlineHelp->SetText( m_BMain02.GetMenuName(), szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_DOWN, ON_HELP_WHITE );
			}
			break;
		}

		// 생산 버튼를 누르면...
		if( m_BOk.Process( m_bMouseDown ) == TRUE )
		{
			if( m_siBuildSelect == -1 )		break;
			SetShowCheckBox();
			m_siBuildStatus	=	ON_FARM_ITEM_MAKE_INPUT;

			// 생산 버튼을 누르면 위에 있는 버튼 4개를 모두 비활성화 한다.
			// 만약 누를 수 가 이따면 문제가 생김~
			m_BOk.SetAction( FALSE );
//			m_BMain01.SetAction( FALSE );
			m_BMain03.SetAction( FALSE );
			m_BMain02.SetAction( FALSE );
		}

		m_SelectControl.Action( IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &TempSelect );
		m_SelectMakeMaterial.Action( IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &TempSelect1, FALSE );

		// TempSelect가 -1이 아니라면 변화 생긴 거이므로 
		if( TempSelect != -1 )							
		{
			if(m_siBuildSelect != TempSelect)		
			{
				m_SelectMakeMaterial.DelAllSell();
				bChangeSelect = TRUE;
			}
			m_siBuildSelect	= TempSelect;
		}

		// 만약 선택한것이 있뜸 버튼을 활성화 한다.
		if( m_siBuildSelect > -1 )		m_BOk.SetAction( TRUE );

		if(!m_Building[ m_siSelect ].m_pWorkInfo && bChangeSelect)
		{
			if( (m_siBuildSelect > -1) && m_pItemID )
			{
				pItem			=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_pItemID[ m_siBuildSelect ] );

				if( !pItem )		break;

				if( pItem->m_Attribute & ON_ITEM_NO_MAKE )
				{
					m_SelectMakeMaterial.AddSell((LPVOID)0, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NOPRODUCTIONITEM));
				}
				else
				{
					if( pItem->m_Attribute & ON_ITEM_NO_MATERIAL )
						m_SelectMakeMaterial.AddSell((LPVOID)0, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WANTMATERIAL));
					else	
					{							
						SI16	siTempLine = 0, ix;
						CHAR	szTemp[ 512 ];
						
						sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK01 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), pItem->pMakeItem->m_siProduction );
						m_SelectMakeMaterial.AddSell((LPVOID)siTempLine, szTemp);
						siTempLine++;
						
						if( pItem->pMakeItem )
						{
							CItemHeader *pTempItem;
							
							for( ix=0 ; ix<ON_ITEM_MAX_ADDMAKE; ix++ )
							{
								if( pItem->pMakeItem->m_uiItemID[ix] != 0 )
								{
									pTempItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( pItem->pMakeItem->m_uiItemID[ix] );
									sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK02 ), m_pMyOnlineWorld->pOnlineText->Get( pTempItem->m_siItemName ), pItem->pMakeItem->m_siItemNum[ix] );
									m_SelectMakeMaterial.AddSell((LPVOID)siTempLine, szTemp);
									siTempLine++;
								}
							}
							
							for( ix=0 ; ix<ON_ITEM_MAX_TOOL; ix++ )
							{
								if( pItem->pMakeItem->m_uiToolID[ix] != 0 )
								{
									pTempItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( pItem->pMakeItem->m_uiToolID[ix] );
									sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK05 ), m_pMyOnlineWorld->pOnlineText->Get( pTempItem->m_siItemName ) );
									m_SelectMakeMaterial.AddSell((LPVOID)siTempLine, szTemp);
									siTempLine++;
								}
							}

							sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK03 ), pItem->pMakeItem->m_siLabor );
							m_SelectMakeMaterial.AddSell((LPVOID)siTempLine, szTemp);
							siTempLine++;
							
							sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK04 ) );
							m_SelectMakeMaterial.AddSell((LPVOID)siTempLine, szTemp);
							siTempLine++;
						}
					}
				}
			}
		}
		break;

	case ON_FARM_ITEM_MAKE_WAIT:
		// FarmProduct()
		break;

	case ON_FARM_ITEM_MAKE_INPUT:
		// 생산하는 아이템 수량과 임금을 성정을 한다.
		siTemp			=	CheckAction();
		m_siQuantity	=	atoi( m_CheckInterface.m_szEdit01 );
		m_siPay			=	_atoi64( m_CheckInterface.m_szEdit02 );

		if( siTemp == 1 )
		{
			Initialize();
//			m_BMain01.SetAction( TRUE );
			m_BMain02.SetAction( TRUE );
			m_BMain03.SetAction( TRUE );

			pFacility			=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
			CItemHeader	*pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_pItemID[ m_siBuildSelect ] );

			SI16 siRest = m_siQuantity % pItem->pMakeItem->m_siProduction;

			if(!siRest)
			{
				if(pItem->pMakeItem->m_siLabor * (m_siPay + ON_FARM_TAX_RATE) * m_siQuantity / pItem->pMakeItem->m_siProduction <= m_pMyOnlineWorld->pMyData->GetMoney())
				{
					//m_BModify.SetX(533);	//m_BStop.SetX(615);		//m_BOk.SetX(690);
					//m_BModify.SetY(425);	//m_BStop.SetY(425);		//m_BOk.SetY(425);
					
					m_siBuildStatus	=	ON_FARM_ITEM_MAKE_WAIT;
					m_pMyOnlineWorld->pOnlineClient->SendRequestMakeItem( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect, m_pItemID[ m_siBuildSelect ], (UI32)m_siQuantity, m_siPay );
				}
				else
				{
					m_BOk.SetAction( TRUE );
					m_BExit.SetAction( TRUE );
					
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_NOMONEY) );				
					m_siBuildStatus	=	ON_FARM_ITEM_MAKE_POLL;	
				}
			}
			else
			{
				m_BOk.SetAction( TRUE );
				m_BExit.SetAction( TRUE );
				
				CHAR Temp[200];
				sprintf(Temp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_INPUT_AGAIN), pItem->pMakeItem->m_siProduction);
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", Temp );				
				m_siBuildStatus	=	ON_FARM_ITEM_MAKE_POLL;	
			}
		}
		else if( siTemp == 0 )
		{
			Initialize();
//			m_BMain01.SetAction( TRUE );
			m_BMain02.SetAction( TRUE );
			m_BMain03.SetAction( TRUE );
			m_BOk.SetAction( TRUE );
			m_BExit.SetAction( TRUE );

			m_siBuildStatus	=	ON_FARM_ITEM_MAKE_POLL;				
		}

		break;
		
	case ON_FARM_ITEM_MAKE_EXIT:	
		{
			if(m_bExitOk || !m_bInFactory)
			{
				m_BOk.SetAction( FALSE );		
				m_BBack.SetAction( FALSE );
				m_SelectControl.DelAllSell();
				m_SelectMakeMaterial.DelAllSell();
				m_BOk.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_BUILD ) );
				delete m_Building[ m_siSelect ].m_pWorkInfo;		
				
				m_Building[ m_siSelect ].m_pWorkInfo	=	NULL;		
//				m_siRight								=	GetTickCount();
				m_siStatus								=	ON_FARM_SCROLL_EXITRIGHT;
				m_siNextStatus							=	ON_FARM_POLL;
				m_siBuildSelect							=	-1;			
				m_siBuildStatus							=	ON_FARM_ITEM_MAKE_INIT;

//				m_BMain01.SetAction( TRUE );
				m_BMain03.SetAction( TRUE );
				m_BMain02.SetAction( TRUE );
				m_BExit.SetAction( TRUE );
				m_bInFactory	= FALSE;
				m_bExitOk		= FALSE;
			}
		}
		break;
	}
}

////////////////////////////////////////////////
// 아이템 만들기 드로우
VOID	OnlineVillageStructFarm::ItemMakeDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			siTemp, siTempFont;
	CHAR			szTemp[256];	
	CItemHeader		*pItem;
	stFacility		*pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );

	
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		siTemp		=	ON_SCREEN_XSIZE - m_InterfaceSpr.Header.Xsize;

		if( stricmp( m_Building[ m_siSelect ].m_szKind, "STG" ) != 0 )
		{
			if( m_Building[ m_siSelect ].m_uiSlotLevel != 0 )
			{
				if( m_Building[ m_siSelect ].m_uiSlotLevel != 0 )
					siTempFont	=	pFacility->siSpriteFont + m_Building[ m_siSelect ].m_uiSlotLevel - 1;
				else 
					siTempFont	=	pFacility->siSpriteFont + m_Building[ m_siSelect ].m_uiSlotLevel;
			}
			else
				siTempFont	=	pFacility->siSpriteFont + m_Building[ m_siSelect ].m_uiSlotLevel;
		}
		else
			siTempFont	=	pFacility->siSpriteFont;

		clGrp.PutSpriteT( m_siX, m_siY, m_ItemInterfaceSpr.Header.Xsize, m_ItemInterfaceSpr.Header.Ysize, m_ItemInterfaceSpr.Image );
		clGrp.PutSpriteT( m_siX + 30 + ( ( 246 - m_BuidingSpr.Header.Xsize ) / 2 ), m_siY + 47 + ( ( 146 - m_BuidingSpr.Header.Ysize ) / 2 ), m_BuidingSpr.Header.Xsize, 
						  m_BuidingSpr.Header.Ysize, &m_BuidingSpr.Image[ m_BuidingSpr.Header.Start[ siTempFont ] ] );
		clGrp.PutSpriteT(m_siX + 489,m_siY + 215,m_ItemScrollBase.Header.Xsize,m_ItemScrollBase.Header.Ysize,m_ItemScrollBase.Image);
		
		if(m_pItemID && (m_SelectControl.GetCurrentPosition() != -1))
		{
			pItem		=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_pItemID[m_SelectControl.GetCurrentPosition()]);

			if(pItem)
			{
				SI32 siItemIndex =	m_pMyOnlineWorld->pOnlineResource->GetItemFileIndex(pItem->m_szSpriteFile);
				XSPR *pItemSpr	 =  m_pMyOnlineWorld->pOnlineResource->GetxsprItem( siItemIndex );

				clGrp.PutSpriteT( m_siX + 68, m_siY + 243, pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[ pItemSpr->Header.Start[ pItem->m_siSpriteFont ] ] );
			}
		}	
	
//		m_BMain01.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		
		m_BMain02.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BMain03.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );

		clGrp.PutSpriteT(m_siX + 342,m_siY + 358,m_ButtonBaseSpr.Header.Xsize,m_ButtonBaseSpr.Header.Ysize,m_ButtonBaseSpr.Image);

		m_BScrUp.Put( &m_ScrollUpImage, 0, 2, 1, BUTTON_PUT_NOMOVE ); 
		m_BScrDn.Put( &m_ScrollDnImage, 0, 2, 1, BUTTON_PUT_NOMOVE );

		m_BScr2Up.Put( &m_ScrollUpImage, 0, 2, 1, BUTTON_PUT_NOMOVE ); 
		m_BScr2Dn.Put( &m_ScrollDnImage, 0, 2, 1, BUTTON_PUT_NOMOVE );

		m_BBack.Put( &m_BackSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BOk.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		if(m_pItemID && (m_SelectControl.GetCurrentPosition() != -1))
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 55, m_siY + 319,86,17, m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), RGB( 255, 255, 255 ) );
		

		if( m_Building[ m_siSelect ].m_pWorkInfo )
		{	
			if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex != ON_PLANT_SLOTUPGRADE_CODE )
			{
				pItem		=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex );

				SI16	siTempLine = 0, ix;

				if( pItem )
				{
					sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK01 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 540, 295, szTemp, RGB( 150, 150, 150 ) );
					siTempLine++;

					if( pItem->pMakeItem )
					{
						CItemHeader *pTempItem;

						for( ix=0 ; ix<ON_ITEM_MAX_ADDMAKE; ix++ )
						{
							if( pItem->pMakeItem->m_uiItemID[ix] != 0 )
							{
								pTempItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( pItem->pMakeItem->m_uiItemID[ix] );

								sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK02 ), m_pMyOnlineWorld->pOnlineText->Get( pTempItem->m_siItemName ), pItem->pMakeItem->m_siItemNum[ix] );
								m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 540, 295 + ( siTempLine * 15 ), szTemp, RGB( 150, 150, 150 ) );
								siTempLine++;
							}
						}

						for( ix=0 ; ix<ON_ITEM_MAX_TOOL; ix++ )
						{
							if( pItem->pMakeItem->m_uiToolID[ix] != 0 )
							{
								pTempItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( pItem->pMakeItem->m_uiToolID[ix] );

								sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK05 ), m_pMyOnlineWorld->pOnlineText->Get( pTempItem->m_siItemName ) );
								m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 540, 295 + ( siTempLine * 15 ), szTemp, RGB( 150, 150, 150 ) );
								siTempLine++;
							}
						}
					}

					sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK03 ), pItem->pMakeItem->m_siLabor );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 540, 295 + ( siTempLine * 15 ), szTemp, RGB( 150, 150, 150 ) );
					siTempLine++;


					sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK04 ) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 540, 295 + ( siTempLine * 15 ), szTemp, RGB( 150, 150, 150 ) );
					siTempLine++;
				}
			}

			if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 265, 295, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_STRCT_PROVEING), RGB( 150, 150, 150 ) );
				sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY), m_Building[ m_siSelect ].m_uiPay );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 265, 335, szTemp, RGB( 150, 150, 150 ) );
			}
		}

		m_BMain02.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BMain03.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		m_BOk.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BBack.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hDC );
	}

	if( m_siShowFont == ON_FARM_SHOW_BUILD_CONTROL )
	{
		//아이템 설명.
		m_SelectMakeMaterial.Draw( pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ), 
			RGB(150, 150, 150), RGB(150, 150, 150), TRUE, RGB(150, 150, 150), RGB(150, 150, 150));

		//아이템.
		m_SelectControl.Draw( pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ), 
			RGB(150, 150, 150), RGB(96, 96, 96), TRUE, RGB(0, 255, 0), RGB(200, 200, 200));
	}
}

////////////////////////////////////////////////
// 
VOID	OnlineVillageStructFarm::WorkAction()
{
	stFacility	*pFacility;	
	SI32		TempSelect;
	SI16		siTempLine = 0;

	switch( m_siBuildStatus )
	{
	case ON_FARM_WORK_INIT:
		// FarmProductInfo()
		break;

	case ON_FARM_WORK_POLL:
		m_SelectWorkMaterial.Action( IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &TempSelect, FALSE );
		m_SelectItemWorkMaterial.Action( IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &TempSelect, FALSE );

		if(m_siBuildIndex == ON_FARM_STRUCT_MAKE)
		{
			if(m_BScrUp.Process(m_bMouseDown) == TRUE)
			{
				m_SelectWorkMaterial.PrevSell();
			}
			else if(m_BScrDn.Process(m_bMouseDown) == TRUE)
			{
				m_SelectWorkMaterial.NextSell();
			}
		}
		else
		{
			if(m_BScrUp.Process(m_bMouseDown) == TRUE)
			{
				m_SelectItemWorkMaterial.PrevSell();
			}
			else if(m_BScrDn.Process(m_bMouseDown) == TRUE)
			{
				m_SelectItemWorkMaterial.NextSell();
			}
		}
		if( m_BOk.Process( m_bMouseDown ) == TRUE )
		{
			if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
				break;
			m_siBuildStatus	=	ON_FARM_WORK_TOOL;			
			m_BExit.SetAction( FALSE );
			m_BOk.SetAction( FALSE );
			m_BBack.SetAction( FALSE );
			m_BStop.SetAction( FALSE );
			m_BModify.SetAction( FALSE );
			

			m_pMyOnlineWorld->pOnlineClient->SendRequseFarmWorkStartMsg( (UI08)m_siSelect );
		}

		else if( m_BBack.Process( m_bMouseDown ) == TRUE || LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )
		{
			if(m_bInFactory)
				m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();

			m_BModify.SetAction( FALSE );
			m_BStop.SetAction( FALSE );
			m_BOk.SetAction( FALSE );
			m_BBack.SetAction( FALSE );

			m_siBuildStatus	=	ON_FARM_WORK_EXIT;
		}

		else if( m_BOk.IsInMouse() )
		{
			CHAR szTempBuffer[512];
			
			if( m_Building[ m_siSelect ].m_pWorkInfo )
			{
				ZeroMemory( szTempBuffer, sizeof( szTempBuffer ) );
				pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
				
				if( pFacility )
				{
					if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
						sprintf( szTempBuffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK ), m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ) );
					else
					{
						CItemHeader	*pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex);
						if( pItem )	sprintf( szTempBuffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ) );
					}
					
					m_pMyOnlineWorld->pOnlineHelp->SetText( m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING), szTempBuffer, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_DOWN, ON_HELP_WHITE );
				}
			}
		}

		// 주인만 가능함
		if( m_pMakeList->GetMaster() )
		{
			// 임금 변경
			if(m_BModify.Process(m_bMouseDown) == TRUE)
			{
				if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
					break;
				m_siBuildStatus = ON_FARM_WORK_CHANG_PAY_INPUT;
				SetShowCheckBox(1);
			}

			else if(m_BModify.IsInMouse())
			{
				if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
					break;
				CHAR szTempBuffer[512];

				ZeroMemory( szTempBuffer, sizeof( szTempBuffer ) );
				sprintf( szTempBuffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_CHANG_PAY ), m_Building[ m_siSelect ].m_uiPay );
				m_pMyOnlineWorld->pOnlineHelp->SetText( m_BModify.GetMenuName(), szTempBuffer, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_DOWN, ON_HELP_WHITE );			
			}

			else if(m_BStop.Process(m_bMouseDown) == TRUE)
			{

				if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
					break;
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OKCANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHEREHOUSE),  m_pMyOnlineWorld->pOnlineText->Get(8000527));
				m_siBuildStatus = ON_FARM_WORK_STOP;
				
/*				// 생산 취소 메세지를 보내 준다.
				m_BModify.SetAction(FALSE);
				m_BStop.SetAction(FALSE);
				m_BOk.SetAction(FALSE);
				m_pMyOnlineWorld->pOnlineClient->SendRequseCancel( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
				m_siBuildStatus	=	ON_FARM_WORK_CANCEL;
*/
			}
			else if(m_BStop.IsInMouse() )
			{
				if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
					break;

				CHAR szTempBuffer[512];

				ZeroMemory( szTempBuffer, sizeof( szTempBuffer ) );

				pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );

				if( (pFacility) && (m_Building[ m_siSelect ].m_pWorkInfo) )
				{
					if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
					{
						sprintf( szTempBuffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_CANCEL ), m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ), m_Building[ m_siSelect ].m_uiPay * m_Building[ m_siSelect ].m_uiWork );
					}
					else
					{
						CItemHeader	*pItem;
						pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex);
						
						if( pItem )
							sprintf( szTempBuffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_CANCEL ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), m_Building[ m_siSelect ].m_uiPay * m_Building[ m_siSelect ].m_uiWork );
					}

					m_pMyOnlineWorld->pOnlineHelp->SetText( m_BStop.GetMenuName(), szTempBuffer, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_DOWN, ON_HELP_WHITE );
				}
			}
		}



		{
			stFacility	*pFacility;
			CItemHeader	*pItem;
			CHAR		szTemp01Buffer[ 512 ], szTemp02Buffer[ 512 ];		

			pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );			

			ZeroMemory( m_szTalk, sizeof( m_szTalk ) );
			ZeroMemory( szTemp02Buffer, sizeof( szTemp02Buffer ) );
			ZeroMemory( szTemp01Buffer, sizeof( szTemp01Buffer ) );

			if( m_Building[ m_siSelect ].m_pWorkInfo )
			{
				if( ON_PLANT_SLOTUPGRADE_CODE != m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex )
				{
					pItem		=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex);

					if( pItem )
					{
						sprintf( szTemp01Buffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_MAKE01 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ) );
						sprintf( szTemp02Buffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_MAKE02 ), m_Building[ m_siSelect ].m_uiWork, m_Building[ m_siSelect ].m_uiPay );
						sprintf( m_szTalk, "%s %s", szTemp01Buffer, szTemp02Buffer );
					}
				}
				else
				{
					sprintf( szTemp01Buffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_MAKE01 ), m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ) );
					sprintf( szTemp02Buffer, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_MAKE02 ), m_Building[ m_siSelect ].m_uiWork, m_Building[ m_siSelect ].m_uiPay );
					sprintf( m_szTalk, "%s %s", szTemp01Buffer, szTemp02Buffer );				
				}
			}
		}
		break;
	case ON_FARM_WORK_STOP:
		{
			if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK )
			{
				m_BModify.SetAction(FALSE);
				m_BStop.SetAction(FALSE);
				m_BOk.SetAction(FALSE);
				m_pMyOnlineWorld->pOnlineClient->SendRequseCancel( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect );
				m_siBuildStatus	=	ON_FARM_WORK_CANCEL;
				m_siBuildStatus = ON_FARM_WORK_POLL;

			}
			else if(m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL )
			{
				m_siBuildStatus = ON_FARM_WORK_POLL;
			}
		}
		break;
	case ON_FARM_WORK_CANCEL:
		// FarmProductCancel()
		break;

	case ON_FARM_WORK_CHANG_PAY_INPUT:
		{
			m_BModify.SetAction(FALSE);
			m_BStop.SetAction(FALSE);
			m_BOk.SetAction(FALSE);
			SI16	siTemp = CheckAction();
			OnlineEditControlEx	*pOecEx;

			if( siTemp == 1 )
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequseChangePay( m_pMakeList->GetPlantsKind(), (UI08)m_pMakeList->GetSelectList(), (UI08)m_siSelect, (SI16)(m_siPay = _atoi64( m_CheckInterface.m_szEdit02 )) );

				m_BModify.SetAction(TRUE);
				m_BStop.SetAction(TRUE);
				m_BOk.SetAction(TRUE);
				m_siBuildStatus = ON_FARM_WORK_CHANG_PAY_WAIT;
				//m_CheckInterface.m_pEdit02->SetString( "" );			
				pOecEx			= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
				pOecEx->Clear();
			}
			else if( siTemp == 0 )
			{
				m_BModify.SetAction(TRUE);
				m_BStop.SetAction(TRUE);
				m_BOk.SetAction(TRUE);
				m_siBuildStatus = ON_FARM_WORK_POLL;				
			}
		}
		break;


	case ON_FARM_WORK_CHANG_PAY_WAIT:
		// FarmProductPayChange()
		break;

	case ON_FARM_WORK_TOOL:
		// FarmWorkStart()
		break;

	case ON_FARM_WORK_PLAY_MINIGAME:
		if( MiniGame(m_siMiniGameType) == FALSE || LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
		{
			ApplyHack();

			m_siBuildStatus	=	ON_FARM_WORK_WAIT;

			// robypark 2004/12/30 12:47
			// 미니게임 마작 종료할 때 ESC키로 종료한 경우 단축키 사용기능을 비활성화된 상태로 종료된다.
			// 다시 단축키 사용이 가능하도록 설정한다.
			if (pMyOnlineWorld->GetShortKey())
				pMyOnlineWorld->SetShortKey( FALSE );	

			// 서버로 노동량을 보내 준다.
			pFacility =	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );

			if(m_pMyOnlineWorld->pOnlineClient->SendRequestWork( (UI08)m_siSelect, GetMiniGameType(), GetMiniGameWork(), GetMiniGameLevel())==FALSE)
			{
				m_siBuildStatus	= ON_FARM_WORK_POLL;
			}

			EndMiniGame(m_siMiniGameType);
			m_bMiniGame		=	FALSE;
			m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
			
			// 배경음을 켠다.
#ifdef _OGG_PLAY
			GetBgmMgr()->PlayNextBgm(TRUE);
#else
			g_pSoundManager->PlaySound( m_pMyOnlineWorld->pOnlineVillage->BackgroundMusic );
#endif
			m_pMyOnlineWorld->m_bMiniGameOn = FALSE;
		}
		else
			m_pMyOnlineWorld->m_bMiniGameOn = TRUE;
		break;

	case ON_FARM_WORK_WAIT:	
		// FarmWorkResult()
		break;

	case ON_FARM_WORK_EXIT:
		{
			if(m_bExitOk || !m_bInFactory)
			{
				m_BOk.SetAction( FALSE );
				m_BStop.SetAction( FALSE );
				m_BModify.SetAction( FALSE );
				m_BOk.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_BUILD ) );
				delete m_Building[ m_siSelect ].m_pWorkInfo;
				
				m_Building[ m_siSelect ].m_pWorkInfo	=	NULL;
//				m_siRight								=	GetTickCount();
				m_siStatus								=	ON_FARM_SCROLL_EXITRIGHT;
				m_siNextStatus							=	ON_FARM_POLL;
				m_siBuildSelect							=	-1;
				
//				m_BMain01.SetAction( TRUE );
				m_BMain03.SetAction( TRUE );
				m_BMain02.SetAction( TRUE );
				m_BExit.SetAction( TRUE );
				m_bInFactory	= FALSE;
				m_bExitOk		= FALSE;
			}
		}
		break;
	}
}

////////////////////////////////////////////////
// 
VOID	OnlineVillageStructFarm::WorkDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	SI16			siTemp, siTempFont;
	stFacility		*pFacility;
	CItemHeader		*pItem;


	if( m_siBuildStatus == ON_FARM_WORK_PLAY_MINIGAME )		
	{
		DrawMiniGame( m_siMiniGameType, pSurface );
		return;
	}

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{		
		siTemp		=	ON_SCREEN_XSIZE - m_InterfaceSpr.Header.Xsize;
		pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );

		if( stricmp( m_Building[ m_siSelect ].m_szKind, "STG" ) != 0 )
		{
			if(m_Building[ m_siSelect ].m_uiSlotLevel != 0 )
			{
				if( m_Building[ m_siSelect ].m_uiSlotLevel != 0 )
					siTempFont	=	pFacility->siSpriteFont + m_Building[ m_siSelect ].m_uiSlotLevel - 1;
				else 
					siTempFont	=	pFacility->siSpriteFont + m_Building[ m_siSelect ].m_uiSlotLevel;
			}
			else 
				siTempFont	=	pFacility->siSpriteFont + m_Building[ m_siSelect ].m_uiSlotLevel;
		}
		else
			siTempFont	=	pFacility->siSpriteFont;

		//시설 향상 중이면.
		if(m_siBuildIndex == ON_FARM_STRUCT_MAKE)
		{
			clGrp.PutSpriteT( m_siX, m_siY, m_InterfaceSpr.Header.Xsize, m_InterfaceSpr.Header.Ysize,	m_InterfaceSpr.Image);
			clGrp.PutSpriteT( m_siX + 59 +  (246 - m_BuidingSpr.Header.Xsize ) / 2, m_siY + 165 + (( 146 - m_BuidingSpr.Header.Ysize ) /2 ), m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize, &m_BuidingSpr.Image[ m_BuidingSpr.Header.Start[ siTempFont ] ] );
			m_pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 45, NPC_KIND_FARM, m_pMyOnlineWorld->pOnlineVillage->GetNation() );
		}
		else //if(m_siBuildIndex == ON_FARM_STRUCT_ITEM_MAKE)
		{
			clGrp.PutSpriteT( m_siX,m_siY,m_ItemInterfaceSpr.Header.Xsize,m_ItemInterfaceSpr.Header.Ysize,m_ItemInterfaceSpr.Image);
			clGrp.PutSpriteT( m_siX + 30 +  (246 - m_BuidingSpr.Header.Xsize ) / 2, m_siY + 47 + (( 146 - m_BuidingSpr.Header.Ysize ) /2 ), m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize, 
				&m_BuidingSpr.Image[ m_BuidingSpr.Header.Start[ siTempFont ] ] );

			m_BScrUp.Put( &m_ScrollUpImage, 0, 2, 1, BUTTON_PUT_NOMOVE ); 
			m_BScrDn.Put( &m_ScrollDnImage, 0, 2, 1, BUTTON_PUT_NOMOVE ); 

		}
//		else
		// 주인이라면 임금 조정 버튼과 생산 취소 버튼을 찍어 준다.
		if( m_pMakeList->GetMaster() )
		{
			clGrp.PutSpriteT(m_siX + 100,m_siY + 358,m_ButtonBaseSpr.Header.Xsize,m_ButtonBaseSpr.Header.Ysize,m_ButtonBaseSpr.Image);
			clGrp.PutSpriteT(m_siX + 221,m_siY + 358,m_ButtonBaseSpr.Header.Xsize,m_ButtonBaseSpr.Header.Ysize,m_ButtonBaseSpr.Image);
			m_BStop.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE ); 
			m_BModify.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
//			m_BMain01.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
//			m_BMain03.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
//			m_BMain02.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		}

		clGrp.PutSpriteT(m_siX + 342,m_siY + 358,m_ButtonBaseSpr.Header.Xsize,m_ButtonBaseSpr.Header.Ysize,m_ButtonBaseSpr.Image);

		m_BBack.Put( &m_BackSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BOk.Put( &m_MainButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );

		

		if(m_siBuildIndex != ON_FARM_STRUCT_MAKE)
		{
			if( m_Building[ m_siSelect ].m_pWorkInfo )
			{
				if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex != ON_PLANT_SLOTUPGRADE_CODE )
				{
					pItem		=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex);
					
					if( pItem )
					{
						SI32 siItemIndex =	m_pMyOnlineWorld->pOnlineResource->GetItemFileIndex( pItem->m_szSpriteFile );
						XSPR *pItemSpr	 =  m_pMyOnlineWorld->pOnlineResource->GetxsprItem( siItemIndex );

	//					clGrp.PutSpriteT( 640, 288, m_ItemPannelSpr.Header.Xsize, m_ItemPannelSpr.Header.Ysize, m_ItemPannelSpr.Image );
						clGrp.PutSpriteT( m_siX + 68, m_siY + 243, pItemSpr->Header.Xsize, pItemSpr->Header.Ysize, &pItemSpr->Image[ pItemSpr->Header.Start[ pItem->m_siSpriteFont ] ] );
					}
				}
			}
		}

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		if(m_siBuildIndex == ON_FARM_STRUCT_MAKE)
		{
			if( m_szTalk[0] != 0 )	m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2( hDC, m_siX + 109, m_siY + 70, 402, m_szTalk );
		}
		else if(m_siBuildIndex == ON_FARM_STRUCT_ITEM_MAKE)
		{
			if( m_szTalk[0] != 0 )	m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2( hDC, m_siX + 305, m_siY + 86, 198, m_szTalk );
		}

		if(m_siBuildIndex != ON_FARM_STRUCT_MAKE)
		{
			if( m_Building[ m_siSelect ].m_pWorkInfo )
			{
				if( m_Building[ m_siSelect ].m_uiPay == 0 )
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 310, 290, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NOPLAY), RGB( 240, 20, 20 ) );
				else
				{	
					// 시설 향상중이다.                                                                                                                                 
					if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
					{
					}

					// 아이템 생산중이다.
					else
					{
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 55, m_siY + 319, 86, 16, m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), RGB( 255, 255, 255 ) );
					}
				}
			}
			else if(m_siBuildStatus != ON_FARM_WORK_INIT)
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, 310, 300, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NOPLAY), RGB( 240, 20, 20 ) );
		}

		if( m_pMakeList->GetMaster() )
		{
			m_BStop.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BModify.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}

		m_BOk.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BBack.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC( hDC );
	}

	if(m_siBuildIndex == ON_FARM_STRUCT_MAKE)
	{
		m_SelectWorkMaterial.Draw( pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ), 
			RGB(150, 150, 150), RGB(150, 150, 150), TRUE, RGB(150, 150, 150), RGB(150, 150, 150));
	}
	else
	{
		m_SelectItemWorkMaterial.Draw(pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ), 
			RGB(150, 150, 150), RGB(150, 150, 150), TRUE, RGB(150, 150, 150), RGB(150, 150, 150));
	}
}


////////////////////////////////////////////////
// 
VOID	OnlineVillageStructFarm::SetShowCheckBox(UI08 uiKind)
{
	m_CheckInterface.m_siX		=	( ON_SCREEN_XSIZE - m_CheckSpr.Header.Xsize ) / 2; 
	m_CheckInterface.m_siY		=	( ON_SCREEN_YSIZE - m_CheckSpr.Header.Ysize ) / 2;
	m_CheckInterface.m_bActive	=	TRUE;
	m_CheckInterface.m_uiKind	=	uiKind;

	if( m_CheckInterface.m_siX + m_CheckSpr.Header.Xsize > ON_SCREEN_XSIZE )			m_CheckInterface.m_siX	=	ON_SCREEN_XSIZE - m_CheckSpr.Header.Xsize - 1;
	if( m_CheckInterface.m_siY + m_CheckSpr.Header.Ysize > ON_SCREEN_YSIZE )			m_CheckInterface.m_siY	=	ON_SCREEN_YSIZE - m_CheckSpr.Header.Ysize - 1;

	//m_CheckInterface.m_pEdit01->SetString("");
	//m_CheckInterface.m_pEdit02->SetString("");

	OnlineEditControlEx	*pOecEx;
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
	pOecEx->Clear();
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
	pOecEx->Clear();

	//	m_CheckInterface.m_pEdit01->EnableWindow( TRUE );
	//	m_CheckInterface.m_pEdit02->EnableWindow( TRUE );
	//	m_CheckInterface.m_pEdit01->SetFocus();

	Handler_OnlineEditControlEx::SetFocus( m_CheckInterface.m_hOecEdit01 );

	m_CheckInterface.m_BOk.SetAction( TRUE );
	m_CheckInterface.m_BCancel.SetAction( TRUE );

	m_BExit.SetAction( FALSE );
	m_BBack.SetAction( FALSE );

	ZeroMemory( m_CheckInterface.m_szEdit01, sizeof(m_CheckInterface.m_szEdit01) );
	ZeroMemory( m_CheckInterface.m_szEdit02, sizeof(m_CheckInterface.m_szEdit02) );


}

////////////////////////////////////////////////
//
VOID	OnlineVillageStructFarm::Initialize()
{
	m_BExit.SetAction( TRUE );
	m_BBack.SetAction( TRUE );

//	m_BMain01.SetAction( TRUE );
	m_BMain03.SetAction( TRUE );
	m_BMain02.SetAction( TRUE );

	m_CheckInterface.m_BOk.SetAction( FALSE );
	m_CheckInterface.m_BCancel.SetAction( FALSE );

//	m_CheckInterface.m_pEdit01->SetString( "" );
//	m_CheckInterface.m_pEdit02->SetString( "" );
	OnlineEditControlEx	*pOecEx;
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
	pOecEx->Clear();
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
	pOecEx->Clear();

	m_CheckInterface.m_bActive	=	FALSE;
}

////////////////////////////////////////////////
// 어러 가지 선택을 할수 가 있따 
SI16	OnlineVillageStructFarm::CheckAction()
{
	m_pMyOnlineWorld->m_bFollowerSelLock = TRUE;
	//m_CheckInterface.m_pEdit01->GetString( m_CheckInterface.m_szEdit01);
	//m_CheckInterface.m_pEdit02->GetString( m_CheckInterface.m_szEdit02);
	OnlineEditControlEx	*pOecEx;
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
	strcpy(m_CheckInterface.m_szEdit01,pOecEx->GetString());
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
	strcpy(m_CheckInterface.m_szEdit02,pOecEx->GetString());

	SI32  siTempQuantity	= atoi( m_CheckInterface.m_szEdit01 );
	MONEY siTempPay			= _atoi64( m_CheckInterface.m_szEdit02 );

	if(siTempQuantity >= 1000)		
	{
		pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
		pOecEx->SetString("1000");
		strcpy(m_CheckInterface.m_szEdit01,pOecEx->GetString());
		//m_CheckInterface.m_pEdit01->SetString("1000");
		//m_CheckInterface.m_pEdit01->GetString( m_CheckInterface.m_szEdit01);
	}

	if(siTempPay >= 30000)		
	{
		pOecEx = Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
		pOecEx->SetString("30000");
		strcpy(m_CheckInterface.m_szEdit02,pOecEx->GetString());
		//m_CheckInterface.m_pEdit02->SetString("30000");
		//m_CheckInterface.m_pEdit02->GetString( m_CheckInterface.m_szEdit02);
	}


	pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
	if( m_CheckInterface.m_BOk.Process( m_bMouseDown ) == TRUE || 
		pOecEx->IsPushEnter() == TRUE)//LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN)))
	{
		m_pMyOnlineWorld->m_bFollowerSelLock = FALSE;
		SI16	siTemp1, siTemp2;

		siTemp1	=	atoi( m_CheckInterface.m_szEdit01 );
		siTemp2 =	atoi( m_CheckInterface.m_szEdit02 );

		//m_CheckInterface.m_pEdit02->InitDoneMessage();

		if( m_CheckInterface.m_uiKind == 0 )
		{
			if( siTemp1 && siTemp2 )
			{
				m_CheckInterface.m_bActive	=	FALSE;
				m_BExit.SetAction( TRUE );
				m_BBack.SetAction( TRUE );
				return 1;
			}
		}
		else
		{
			if( siTemp2 )
			{
				m_CheckInterface.m_bActive	=	FALSE;
				m_BExit.SetAction( TRUE );
				m_BBack.SetAction( TRUE );
				return 1;
			}
		}
	}

	if( m_CheckInterface.m_BCancel.Process( m_bMouseDown ) == TRUE )
	{
		m_pMyOnlineWorld->m_bFollowerSelLock = TRUE;
		m_CheckInterface.m_bActive	=	FALSE;
		ZeroMemory( m_CheckInterface.m_szEdit01, sizeof(m_CheckInterface.m_szEdit01) );
		ZeroMemory( m_CheckInterface.m_szEdit02, sizeof(m_CheckInterface.m_szEdit02) );
		m_BBack.SetAction( TRUE );
		m_BOk.SetAction( TRUE );
		m_BExit.SetAction( TRUE );

		return 0;
	}

	// 마우스 클릭으로 포커스 설정하기
	POINT pt;
	pt.x	= IpD.Mouse_X;
	pt.y	= IpD.Mouse_Y;
	RECT	rcEdit1, rcEdit2;
	SetRect(&rcEdit1, m_CheckInterface.m_siX + 60, m_CheckInterface.m_siY + 30, m_CheckInterface.m_siX + 180, m_CheckInterface.m_siY + 50);
	SetRect(&rcEdit2, m_CheckInterface.m_siX + 60, m_CheckInterface.m_siY + 50, m_CheckInterface.m_siX + 180, m_CheckInterface.m_siY + 70);

	//if(PtInRect( &rcEdit1, pt ) == TRUE && m_bMouseDown)
		//m_CheckInterface.m_pEdit01->SetFocus();

	//if(PtInRect( &rcEdit2, pt ) == TRUE && m_bMouseDown)
	//	m_CheckInterface.m_pEdit02->SetFocus();

	if(PtInRect( &rcEdit1, pt ) == TRUE && m_bMouseDown)
		Handler_OnlineEditControlEx::SetFocus( m_CheckInterface.m_hOecEdit01 );
	if(PtInRect( &rcEdit2, pt ) == TRUE && m_bMouseDown)
		Handler_OnlineEditControlEx::SetFocus( m_CheckInterface.m_hOecEdit02 );


	return -1;
}


////////////////////////////////////////////////
// 

VOID	OnlineVillageStructFarm::CheckDraw2(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC				hDC;
	CHAR			szTemp01[256], szTemp02[256], szTemp03[256];
	stFacility		*pFacility;
	CItemHeader*	pItem;
	OnlineEditControlEx	*pOecEx;



	if( clGrp.LockSurface( pSurface ) == TRUE )
	{

		clGrp.PutSpriteT( m_CheckInterface.m_siX, m_CheckInterface.m_siY, m_CheckLineSpr.Header.Xsize, m_CheckLineSpr.Header.Ysize, m_CheckLineSpr.Image );

		clGrp.PutSpriteJin( m_CheckInterface.m_siX, m_CheckInterface.m_siY, m_CheckSpr.Header.Xsize, m_CheckSpr.Header.Ysize, m_CheckSpr.Image );

		m_CheckInterface.m_BOk.Put( &m_ButtonCheckBttnSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_CheckInterface.m_BCancel.Put( &m_ButtonCheckBttnSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_CheckInterface.m_BOk.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_CheckInterface.m_BCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pFacility		=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_szBuildCode[ m_siBuildSelect ] );

		if( m_siStatus == ON_FARM_BUILD_STRUCT )
		{
			// 공장 생성 관련 메시지 박스
			sprintf( szTemp01, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKFORCE), pFacility->UpLabor[ 0 ] );
//			sprintf( szTemp02,"%s",m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1));							// actdoll (2004/02/12 13:11) : 이거 추후 반드시 수정할 것

//			m_CheckInterface.m_pEdit02->SetFocus();
			Handler_OnlineEditControlEx::SetFocus( m_CheckInterface.m_hOecEdit02 );
			
//			if( rand()%100 > 60 ) strcat( szTemp02, "_" );

			// 작업량 표시
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 30, szTemp01, RGB( 255, 255, 255 ) );

			// 임금 표시
			// actdoll (2004/02/12 14:09) : 기존 텍스트 출력 방식을 에딧박스 출력 방식으로 교체
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, szTemp02, RGB( 255, 255, 255 ) );
//			m_CheckInterface.m_pEdit02->DrawText( hDC, NULL, NULL, RGB(255, 255, 255), m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), true );
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC,m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1),RGB( 255, 255, 255 ));
/*			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
			pOecEx->SetCursorShow(false);
			pOecEx->SetRectOfDraw(m_CheckInterface.m_siX + 80,m_CheckInterface.m_siY + 50,m_CheckInterface.m_siX + 150,m_CheckInterface.m_siY + 65);
			pOecEx->Draw(hDC);
*/
			// 임금 표시
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC,m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1),RGB( 255, 255, 255 ));

			// robypark 2004/8/26 16:46
			// 임금 표시 3자리마다 comma 찍기
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
			MakeMoneyText(atoi(pOecEx->GetString()), szTemp03);
			
			// 임금 표시 부분에 캐럿찍기
			if (rand() % 100 < 90)
				strcat( szTemp03, "_" );

			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 80, m_CheckInterface.m_siY + 50, szTemp03, RGB( 255, 255, 255 ));

			// 세금 표시
			sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_TAX), pFacility->UpLabor[ 0 ] * ON_FARM_TAX_RATE);
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 70, szTemp02, RGB( 255, 255, 255 ) );

			sprintf( szTemp01, "%d", pFacility->UpLabor[ 0 ] );
			int iTemp = atoi(szTemp01);
			//m_CheckInterface.m_pEdit01->SetString( szTemp01 );
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
			//pOecEx->SetString(szTemp01);
			pOecEx->SetValue(iTemp);

			// 설치 비용 표시
			sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_BUILDCOST), pFacility->UpLabor[ 0 ] * (atoi(m_CheckInterface.m_szEdit02) + ON_FARM_TAX_RATE));
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 90, szTemp02, RGB( 255, 255, 255 ) );
		}
	
		else if( m_siStatus == ON_FARM_UPGRADE_INPUT )
		{
			pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );

			if( pFacility )
			{
				sprintf( szTemp01, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKFORCE), pFacility->UpLabor[ m_Building[ m_siSelect ].m_uiSlotLevel] );
//				sprintf( szTemp02, "%s%s",m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1));		// actdoll (2004/02/12 13:11) : 이거 추후 반드시 수정할 것

				//m_CheckInterface.m_pEdit02->SetFocus();
				Handler_OnlineEditControlEx::SetFocus( m_CheckInterface.m_hOecEdit02 );
//				if( rand()%100 > 60 ) strcat( szTemp02, "_" );
				
				// 작업량 표시
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 30, szTemp01, RGB( 255, 255, 255 ) );

				// 임금 표시
				// actdoll (2004/02/12 14:09) : 기존 텍스트 출력 방식을 에딧박스 출력 방식으로 교체
//				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, szTemp02, RGB( 255, 255, 255 ) );
				//m_CheckInterface.m_pEdit02->DrawText( hDC, NULL, NULL, RGB(255, 255, 255), m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), true );

/*
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), RGB(255,255,255 ));
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
				pOecEx->SetRectOfDraw(m_CheckInterface.m_siX + 80,m_CheckInterface.m_siY + 50,m_CheckInterface.m_siX + 150,m_CheckInterface.m_siY + 65);
				pOecEx->SetCursorShow(false);
				pOecEx->Draw(hDC);
*/
				// robypark 2004/8/28 21:46
				// 포커스가 맞춰진 에디트 윈도에 캐럿이 찍히도록 수정
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
				MakeMoneyText(atoi(pOecEx->GetString()), szTemp03);
			
				// 임금 표시 부분에 캐럿찍기
				if (rand() % 100 < 90)
					strcat( szTemp03, "_" );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), RGB(255,255,255 ));
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 80, m_CheckInterface.m_siY + 50, szTemp03, RGB( 255, 255, 255 ) );

				sprintf( szTemp01, "%d", pFacility->UpLabor[ m_Building[ m_siSelect ].m_uiSlotLevel] );
				int iTemp = atoi(szTemp01);
				//m_CheckInterface.m_pEdit01->SetString( szTemp01 );
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
				//pOecEx->SetString(szTemp01);
				pOecEx->SetValue(iTemp);
				
				// 세금
				sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_TAX), pFacility->UpLabor[ m_Building[ m_siSelect ].m_uiSlotLevel] * ON_FARM_TAX_RATE);
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 70, szTemp02, RGB( 255, 255, 255 ) );

				sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_BUILDCOST), pFacility->UpLabor[ m_Building[ m_siSelect ].m_uiSlotLevel] * (atoi(m_CheckInterface.m_szEdit02) + ON_FARM_TAX_RATE) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 90, szTemp02, RGB( 255, 255, 255 ) );
			}
		}
		else
		{
			pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );

//			sprintf( szTemp01, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRODUCTIONCOST), m_CheckInterface.m_szEdit01 );
//			sprintf( szTemp02, "%s%s",m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), m_CheckInterface.m_szEdit02 );		

			// actdoll (2004/02/12 14:09) : 기존 텍스트 출력 방식을 에딧박스 출력 방식으로 교체
			BOOL	bIsFocus01	= false;
			BOOL	bIsFocus02	= false;
//			if( m_CheckInterface.m_pEdit01->IsFocus() )				if( rand()%100 > 60 ) strcat( szTemp01, "_" );
//			if( m_CheckInterface.m_pEdit02->IsFocus() )				if( rand()%100 > 60 ) strcat( szTemp02, "_" );

			//if( m_CheckInterface.m_pEdit01->IsFocus() )				
			if(Handler_OnlineEditControlEx::IsFocus(m_CheckInterface.m_hOecEdit01))
				bIsFocus01	= true;
			//if( m_CheckInterface.m_pEdit02->IsFocus() )
			if(Handler_OnlineEditControlEx::IsFocus(m_CheckInterface.m_hOecEdit02))
				bIsFocus02	= true;


			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 30, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRODUCTIONCOST), RGB( 255, 255, 255 ) );
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), RGB( 255, 255, 255 ) );
//			m_CheckInterface.m_pEdit01->DrawText( hDC, NULL, NULL, RGB(255, 255, 255), m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 30, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRODUCTIONCOST), bIsFocus01 );
//			m_CheckInterface.m_pEdit02->DrawText( hDC, NULL, NULL, RGB(255, 255, 255), m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY1), bIsFocus02 );
/*
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
			pOecEx->SetRectOfDraw(m_CheckInterface.m_siX + 80,m_CheckInterface.m_siY + 30,m_CheckInterface.m_siX + 150,m_CheckInterface.m_siY + 45);
			pOecEx->SetCursorShow(false);
			pOecEx->Draw(hDC);
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
			pOecEx->SetRectOfDraw(m_CheckInterface.m_siX + 80,m_CheckInterface.m_siY + 50,m_CheckInterface.m_siX + 150,m_CheckInterface.m_siY + 65);
			pOecEx->SetCursorShow(false);
			pOecEx->Draw(hDC);
*/
			// robypark 2004/8/28 16:10
			// 포커스가 맞춰진 에디트 윈도에 캐럿이 찍히도록 수정
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit01 );
			MakeMoneyText(atoi(pOecEx->GetString()), szTemp03);
			
			// 생산량 표시 부분에 캐럿찍기
			if ((bIsFocus01) && (rand() % 100 < 90))
				strcat( szTemp03, "_" );

			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 80, m_CheckInterface.m_siY + 30, szTemp03, RGB( 255, 255, 255 ) );
			
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
			MakeMoneyText(atoi(pOecEx->GetString()), szTemp03);
			
			// 임금 표시 부분에 캐럿찍기
			if ((bIsFocus02) && (rand() % 100 < 90))
				strcat( szTemp03, "_" );

			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 80, m_CheckInterface.m_siY + 50, szTemp03, RGB( 255, 255, 255 ) );
			
			if( pFacility )
			{
				UI16	uiTempItemID = 0, uiTempSkip=0;

				for( SI16 ix=0; ix<ON_FARM_LEV3_PRODUCT; ix++ )
				{
					if( uiTempSkip == m_siBuildSelect )
					{
						pItem = m_pMyOnlineWorld->pItemsInfo->GetItem( pFacility->uiPr[m_pMyOnlineWorld->pOnlineVillage->GetNation()-1][ ix ] );
						if( pItem != NULL )	break;
					}

					uiTempSkip = pFacility->uiPr[m_pMyOnlineWorld->pOnlineVillage->GetNation()-1][ix] != 0 ? uiTempSkip += 1 : uiTempSkip;
				}
			}

			MONEY	siTempTotalMoney  = _atoi64( m_CheckInterface.m_szEdit02 );
			MONEY	siTempTotalMoney2 = _atoi64( m_CheckInterface.m_szEdit01 );

			if( siTempTotalMoney2 == 0 )
			{
				// 세금
				sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_TAX), 0);
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 70, szTemp02, RGB( 255, 255, 255 ) );

				sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_OPERATIONCOST));
			}
			else 
			{
				// 세금
				sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_TAX), pItem->pMakeItem->m_siLabor * ON_FARM_TAX_RATE * ( siTempTotalMoney2 / pItem->pMakeItem->m_siProduction ));
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 70, szTemp02, RGB( 255, 255, 255 ) );
				
				sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_OPERATIONCOST1), (siTempTotalMoney + ON_FARM_TAX_RATE) * ( siTempTotalMoney2 / pItem->pMakeItem->m_siProduction ) * pItem->pMakeItem->m_siLabor );
			}

			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 90, szTemp02, RGB( 255, 255, 255 ) );
		}

		pSurface->ReleaseDC( hDC );
	}
}

VOID	OnlineVillageStructFarm::CheckDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( !m_CheckInterface.m_bActive )		return; 

	if( m_CheckInterface.m_uiKind == 0 )	CheckDraw2( pSurface );
	else									ModifyDraw( pSurface );


}

// 변경되는 임금 메시지 박스 그리기
VOID	OnlineVillageStructFarm::ModifyDraw(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC hDC;
	OnlineEditControlEx	*pOecEx;

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT( m_CheckInterface.m_siX, m_CheckInterface.m_siY, m_CheckLineSpr.Header.Xsize, m_CheckLineSpr.Header.Ysize, m_CheckLineSpr.Image );

		clGrp.PutSpriteJin( m_CheckInterface.m_siX, m_CheckInterface.m_siY, m_CheckSpr.Header.Xsize, m_CheckSpr.Header.Ysize, m_CheckSpr.Image );

		m_CheckInterface.m_BOk.Put( &m_ButtonCheckBttnSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_CheckInterface.m_BCancel.Put( &m_ButtonCheckBttnSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		stFacility		*pFacility;
		CHAR			szTemp02[512];
		pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );


		m_CheckInterface.m_BOk.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_CheckInterface.m_BCancel.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));


		//m_CheckInterface.m_pEdit02->SetFocus();
		Handler_OnlineEditControlEx::SetFocus( m_CheckInterface.m_hOecEdit02 );


		if( pFacility )
		{			
			sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGEPAYBEFORE), m_Building[ m_siSelect ].m_uiPay );
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 30, szTemp02, RGB(255,255,255 ) );

//			sprintf( szTemp02, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGEPAYTARGET), m_CheckInterface.m_szEdit02 );

			// actdoll (2004/02/12 14:09) : 기존 텍스트 출력 방식을 에딧박스 출력 방식으로 교체
//			if( rand()%100 > 60 ) strcat( szTemp02, "_" );
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, szTemp02, RGB(255,255,255 ) );						
			//m_CheckInterface.m_pEdit02->DrawText( hDC, NULL, NULL, RGB(255, 255, 255), m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGEPAYTARGET), true );
		/*
			pOecEx = Handler_OnlineEditControlEx::GetControl(m_CheckInterface.m_hOecEdit02);
			pOecEx->SetCursorShow(false);
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGEPAYTARGET), RGB(255,255,255 ) );
			pOecEx->SetRectOfDraw(m_CheckInterface.m_siX + 100,m_CheckInterface.m_siY + 50,m_CheckInterface.m_siX + 150,m_CheckInterface.m_siY + 65);
			pOecEx->Draw(hDC);
		*/
			// robypark 2004/8/26 16:46
			// 변경될 임금 표시 3자리마다 comma 찍기
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_CheckInterface.m_hOecEdit02 );
			MakeMoneyText(atoi(pOecEx->GetString()), szTemp02);
			
			// 변경될 임금 표시 부분에 캐럿찍기
			if (rand() % 100 < 90)
				strcat( szTemp02, "_" );

			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 25, m_CheckInterface.m_siY + 50, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGEPAYTARGET), RGB(255,255,255 ) );
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_CheckInterface.m_siX + 100, m_CheckInterface.m_siY + 50, szTemp02, RGB( 255, 255, 255 ));
		}

		pSurface->ReleaseDC( hDC );
	}

}

UI16 OnlineVillageStructFarm::FarmWorkCompleted(BYTE *pMsg)
{
	OnMsg *pOnCompletedWork = (OnMsg*)pMsg;

	m_bInFactory = FALSE;
	m_bWorkCompleted = TRUE;
	m_pMyOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_JOP_COMPLETE));
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmSlotInfo(BYTE *pMsg)
{
	OnResFarmSlotInfoMsg *pOnResFarmSlotInfoMsg = (OnResFarmSlotInfoMsg*)pMsg;
	
	if(pOnResFarmSlotInfoMsg->siFactoryNum != -1)
	{
		m_siStatus	=	ON_FARM_INIT;
		m_siPrevStatus = ON_FARM_PREV_NONE;
		bNowScroll	=	FALSE;
		CreateFarm(pOnResFarmSlotInfoMsg);
	}
	else
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_NO_GET_STINFO));
		m_siStatus		=	ON_FARM_SCROLL_EXITLEFT;
		m_siNextStatus	=	ON_FARM_EXIT;
		m_siPrevStatus	=	ON_FARM_PREV_NONE;
		m_siScrollX		=	GetTickCount();
	}
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmSlotExpand(BYTE *pMsg)
{
	OnResFarmExpandSlotMsg *pOnResFarmExpandSlotMsg = (OnResFarmExpandSlotMsg*)pMsg;

	if( pOnResFarmExpandSlotMsg->dwResponse == ON_RET_OK )
	{
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() - ON_PLANT_EXPANDSLOT_COST );		// 나의 돈을 다수 설정
		m_pMakeList->SetSlotNum( m_pMakeList->GetSlotNum() + 1 );														// 슬롯 수를 조정
	}
	
	m_siStatus	=	ON_FARM_POLL;
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmExit(BYTE *pMsg)
{
	OnResLeaveFactoryMsg *pOnResLeaveFactoryMsg = (OnResLeaveFactoryMsg*)pMsg;

	if( pOnResLeaveFactoryMsg->dwResponse == ON_RET_OK )
	{
		m_bExitOk = TRUE;
	}
	else
	{
		m_BBack.SetAction( TRUE );
		
		// BuildAction()
		if(m_siStatus == ON_FARM_BUILD_STRUCT)		m_siBuildStatus = ON_FARM_BUILD_STRUCT_POLL;
		// StoreAction()
		else if(m_siStatus == ON_FARM_STOREHOUSE)	m_siBuildStatus = ON_FARM_BUILD_STOREHOUSE_POLL;
		// ItemMakeAction()
		else if(m_siStatus == ON_FARM_ITEM_MAKE)	m_siBuildStatus = ON_FARM_ITEM_MAKE_POLL;
		// WorkAction()
		else if(m_siStatus == ON_FARM_WORKER)		m_siBuildStatus = ON_FARM_WORK_POLL;
	}

	return 0;
}

UI16 OnlineVillageStructFarm::FarmStructBuild(BYTE *pMsg)
{
	stFacility	*pFacility;
	OnResFarmBuildStructMsg	*pOnResFarmBuildStructMsg = (OnResFarmBuildStructMsg*)pMsg;
	
	if(m_bInFactory)
		m_pMyOnlineWorld->pOnlineClient->SendRequestLeaveFactory();
	
	if( pOnResFarmBuildStructMsg->dwResponse == ON_RET_OK )
	{
		SI16	ix, iy;
		SI16	siXBuild[ ON_MAX_FACTORY_NUM ]	=	{ 108, 195, 386, 575, 487, 297 };
		SI16	siYBuild[ ON_MAX_FACTORY_NUM ]	=	{ 340, 224, 185, 221, 337, 370 };
		
		pFacility									=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_szBuildCode[ m_siBuildSelect ] );
		m_Building[ m_siSelect ].m_bBuilding		=	TRUE;				
		m_Building[ m_siSelect ].m_siBuilingName	=	pFacility->siTextCode;
		m_Building[ m_siSelect ].m_siAniFrame		=	0;
		m_Building[ m_siSelect ].m_uiSlotLevel		=	0;
		m_Building[ m_siSelect ].m_uiPay			=	m_siPay;
		m_siXpos[ m_siSelect ]						=	siXBuild[ m_siSelect ];
		m_siYpos[ m_siSelect ]						=	siYBuild[ m_siSelect ];
		m_bNoBuilding								=	FALSE;												// 빌딩을 한개를 지었으니 무조건 건물이 한개는 있따.
		m_dwStart									=	timeGetTime();										// 애니 매이션 프래임을 위한 뻘짓
		m_siBuildStatus								=	ON_FARM_BUILD_STRUCT_EXIT;
		
		if( m_pBuildButton[m_siSelect] == NULL )	m_pBuildButton[m_siSelect]	=	new UI08[ m_BuidingSpr.Header.Xsize * m_BuidingSpr.Header.Ysize ];
		
		if( clGrp.LockSurface( m_pBuildButton[m_siSelect], m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize ) )
		{
			clGrp.PutImageT( 0, 0, m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize, &m_BuidingSpr.Image[ m_BuidingSpr.Header.Start[ pFacility->siSpriteFont ] ] );
			
			for( iy = 0 ; iy<m_BuidingSpr.Header.Ysize ; iy++ )
				for( ix = 0; ix<m_BuidingSpr.Header.Xsize ; ix++)
				{
					if( m_pBuildButton[m_siSelect][ iy * m_BuidingSpr.Header.Xsize + ix ] !=  TRANSCOLOR )
						m_pBuildButton[m_siSelect][ iy * m_BuidingSpr.Header.Xsize + ix ]  =  ON_BUTTON_CHECK_SAMPLE_VALUE;
				}
				
				m_Building[ m_siSelect ].m_pBuilding.Create( m_siXpos[ m_siSelect ], m_siYpos[ m_siSelect ], m_BuidingSpr.Header.Xsize, m_BuidingSpr.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get( pFacility->siTextCode ), m_pBuildButton[m_siSelect], ON_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_LEFT );
		}
		
		
		// 건설이 성공하면 내돈을 건물값 만큼 빼준다.
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() - ( pFacility->UpLabor[ 0 ] * (m_siPay + ON_FARM_TAX_RATE) ) );
		strcpy( m_Building[ m_siSelect ].m_szKind, m_szBuildCode[ m_siBuildSelect ] );
	}
	
	else if( pOnResFarmBuildStructMsg->dwResponse == ON_RET_FARM_MONEY )
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WARP_MONEY) );
		m_siBuildStatus	=	ON_FARM_BUILD_STRUCT_EXIT;
	}
	
	else if ( pOnResFarmBuildStructMsg->dwResponse == ON_RET_NO )
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_SERVERSTRUCTNO) );
		m_siBuildStatus	=	ON_FARM_BUILD_STRUCT_EXIT;
		m_siStatus		=	ON_FARM_POLL;
	}
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmStructDestroy(BYTE *pMsg)
{
	OnResFarmDestroyStructMsg *pOnResFarmDestroyStructMsg = (OnResFarmDestroyStructMsg*)pMsg;

	if( pOnResFarmDestroyStructMsg->dwResponse == ON_RET_OK )
	{
		m_bExitOk = TRUE;
		if(m_siStatus == ON_FARM_DESTROY_WAIT)	
		{
			SI16	siXpos[ ON_MAX_FACTORY_NUM ]   = { 78, 173, 358, 546, 467, 265 };
			SI16	siYpos[ ON_MAX_FACTORY_NUM ]   = { 389, 280, 234, 281, 389, 426 };
			SI16	iy, ix;
			m_bInFactory = FALSE;
			m_bExitOk = FALSE;
			m_BExit.SetAction(TRUE);
			
			m_Building[ m_siSelect ].m_bBuilding		=	FALSE;
			m_Building[ m_siSelect ].m_siBuilingName	=	0;
			m_Building[ m_siSelect ].m_siAniFrame		=	0;
			m_siXpos[ m_siSelect ]						=	siXpos[ m_siSelect ];
			m_siYpos[ m_siSelect ]						=	siYpos[ m_siSelect ];
			
			if( m_pBuildButton[m_siSelect] )	delete [] m_pBuildButton[m_siSelect];
			if( m_pCheckButton[m_siSelect] )	delete [] m_pCheckButton[m_siSelect];
			
			m_pBuildButton[m_siSelect] = NULL;
			m_pCheckButton[m_siSelect] = NULL;
			m_pCheckButton[m_siSelect] = new UI08[ m_TitleSpr.Header.Xsize * m_TitleSpr.Header.Ysize ];
			
			if( clGrp.LockSurface( m_pCheckButton[m_siSelect], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize ) )
			{
				clGrp.PutImageT( 0, 0, m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize, &m_TitleSpr.Image[ m_TitleSpr.Header.Start[ m_siSelect ] ] );
				
				for( iy = 0 ; iy<m_TitleSpr.Header.Ysize ; iy++ )
					for( ix = 0; ix<m_TitleSpr.Header.Xsize ; ix++)
					{
						if( m_pCheckButton[m_siSelect][ iy * m_TitleSpr.Header.Xsize + ix ] !=  TRANSCOLOR )
							m_pCheckButton[m_siSelect][ iy * m_TitleSpr.Header.Xsize + ix ]  =  ON_BUTTON_CHECK_SAMPLE_VALUE;
					}
					
					m_Building[ m_siSelect ].m_pBuilding.Create( siXpos[ m_siSelect ], siYpos[ m_siSelect ], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize,
						m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_VOID ), m_pCheckButton[m_siSelect], ON_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_LEFT );
			}
			m_siStatus = ON_FARM_POLL;
		}
	}
	else
	{
		if(m_siStatus == ON_FARM_DESTROY_WAIT)		// Action()
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_SERVERDESTORYNO));
			m_siBuildStatus = ON_FARM_POLL;
		}
		else if(m_siStatus == ON_FARM_STOREHOUSE)	// StoreAction()
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WHEREHOUSE), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_REMOVEFAIL));
			m_siBuildStatus = ON_FARM_BUILD_STOREHOUSE_POLL;
		}
	}

	return 0;
}

UI16 OnlineVillageStructFarm::FarmStructUpgrade(BYTE *pMsg)
{
	OnResFarmUpgradeSlotMsg	*pOnResFarmUpgradeSlotMsg = (OnResFarmUpgradeSlotMsg*)pMsg;
	
	m_siStatus = ON_FARM_POLL;
	if( pOnResFarmUpgradeSlotMsg->dwResponse == ON_RET_OK )
	{
		stFacility		*pFacility = m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );				
		
		// 해당 건물에 임금을 설정을 해준다.
		m_Building[ m_siSelect ].m_uiPay  = m_siPay;
		m_Building[ m_siSelect ].m_uiWork = pFacility->UpLabor[ m_Building[ m_siSelect ].m_uiSlotLevel ];
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() - ( (m_siPay + ON_FARM_TAX_RATE) * pFacility->UpLabor[ m_Building[ m_siSelect ].m_uiSlotLevel] ) );
	}
	else if( pOnResFarmUpgradeSlotMsg->dwResponse == ON_RET_FARM_MONEY )
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WARP_MONEY));
	else
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_UPGRADE_FAIL));
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmStructExit(BYTE *pMsg)
{
	OnResLeaveStructureMsg *pOnResLeaveStructureMsg = (OnResLeaveStructureMsg*)pMsg;
	
	if(pOnResLeaveStructureMsg->dwResponse == ON_RET_OK)
	{
		m_dwStart		=	GetTickCount();		
		m_siRight		=	0;
		m_bMouseDown	=	FALSE;
		
		m_siStatus		=	ON_FARM_SCROLL_EXITLEFT;
		m_siNextStatus	=	ON_FARM_EXIT;
		m_siPrevStatus	=	ON_FARM_PREV_NONE;
		m_siScrollX		=	GetTickCount();
		
		for(UI16 i=0 ; i<ON_MAX_FACTORY_NUM; i++ )		
			m_Building[i].m_bSelect = FALSE;
		
		m_SelectBuild.DelAllSell();
		m_SelectControl.DelAllSell();
		m_SelectWorkMaterial.DelAllSell();
		m_SelectItemWorkMaterial.DelAllSell();

		m_BBack.SetAction( FALSE );
		m_BOk.SetAction( FALSE );
	}
	else
	{
		m_siStatus = ON_FARM_POLL;
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FARM_FAIL_EXIT ));
	}

	return 0;
}

UI16 OnlineVillageStructFarm::FarmStorageInfo(BYTE *pMsg)
{
	OnResStorageInfoMsg	*pOnResStorageInfoMsg = (OnResStorageInfoMsg*)pMsg;
	
	if( pOnResStorageInfoMsg->dwResponse == ON_RET_OK )
	{
		OnResStorageInfoOkMsg		*pOnResStorageInfoOkMsg;
		SI32						siItemNum;				
		STORAGE_ITEM				*pItem;
		
		pOnResStorageInfoOkMsg	=	(OnResStorageInfoOkMsg*)pOnResStorageInfoMsg;
		siItemNum				=	pOnResStorageInfoOkMsg->uiNum;
		
		pOnResStorageInfoOkMsg++;
		pItem					=	(STORAGE_ITEM*)pOnResStorageInfoOkMsg;
		
		m_NpcList->Clear();
		m_pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );
		m_pMyOnlineWorld->pOnlineInventory->SetInStruct( ON_STRUCT_IN );
		
		// 창고에 있는 아이템을 받는다.( 서버에서 )
		for( UI16 i=0 ; i < siItemNum; i++ )
		{
			// 아이템 넣어 준다.
			ItemInfo TempItemInfo;
			TempItemInfo.uiItemID = pItem->uiItemCode;
			TempItemInfo.ItemCount = pItem->uiQuantity;
			SI16 siSlot = m_NpcList->GetEmptySlot(pItem->uiItemCode);
			
			m_NpcList->AddItem(TempItemInfo, siSlot);
			pItem++;
		}
		
		// 버튼을 활성, 비활성화 한다.
		m_BDestroy.SetAction( TRUE );
		m_BExit.SetAction( FALSE );
		m_BBack.SetAction( TRUE );
		
		// 건물에 들어갔음 설정
		m_bInFactory = TRUE;
		
		m_siBuildStatus	= ON_FARM_BUILD_STOREHOUSE_POLL;			
	}
	else
	{
		m_siStatus = ON_FARM_POLL;
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_NO_GET_SHINFO));
	}
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmStorageItemInput(BYTE *pMsg)
{
	OnResStorageInputItemMsg *pOnResStorageInputItemMsg = (OnResStorageInputItemMsg*)pMsg;
	
	if( pOnResStorageInputItemMsg->dwResponse == ON_RET_OK )
	{
		m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();
		
		MyItemData	ItemData;
		ItemData.siPosInInventory	=	m_siYetPos;
		ItemData.uiID				=	m_uiItemID;
		ItemData.uiQuantity			=	m_uiQuantity;
		
		// 창고의 아템을 갯수 만틈 오려 준다.
		ItemInfo TempItemInfo;
		TempItemInfo.uiItemID	= m_uiItemID;
		TempItemInfo.ItemCount	= m_uiQuantity;
		SI16 siSlot				= m_NpcList->GetEmptySlot(m_uiItemID);
		if(m_NpcList->GetSlotInfo(siSlot))
			TempItemInfo.ItemCount	+= m_NpcList->GetSlotInfo(siSlot)->ItemCount;
		m_NpcList->AddItem(TempItemInfo, siSlot);
		
		
		m_pMyOnlineWorld->pMyData->DelItem( (UI08)m_siSelectLB, &ItemData );					// 마이 데이타에서 아이템을 지우거
		m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();									// 인벤토리 아이템을 다쉬 넣어 준다.
		
		m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_POLL;
		m_uiItemID		=	0;
		m_siSelectLB	=	0;
		m_uiQuantity	=	0;
		m_siPos			=	-1;
	}
	else
	{
		m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();
		
		m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_POLL;
		m_uiItemID		=	0;
		m_siSelectLB	=	0;
		m_uiQuantity	=	0;
		m_siPos			=	-1;
	}
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmStorageItemOutput(BYTE *pMsg)
{
	OnResStorageOutputItemMsg *pOnResStorageOutputItemMsg = (OnResStorageOutputItemMsg*)pMsg;

	if( pOnResStorageOutputItemMsg->dwResponse == ON_RET_OK )
	{
		MyItemData	ItemData;
		ItemData.siPosInInventory	=	m_siPos;
		ItemData.uiID				=	m_uiItemID;
		ItemData.uiQuantity			=	m_uiQuantity;
		
		m_pMyOnlineWorld->pMyData->AddItem((UI08)m_siSelectLB, &ItemData );						// 마이 데이타에서 아이템을 너어준다.
		
		m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();									// 인벤토리 아이템을 다쉬 넣어 준
		
		// 창고에 있는 아템을 빼준다.
		SI16	siIndex	=	m_NpcList->GetDragSlot();
		m_NpcList->DeleteSlot(siIndex, m_uiQuantity);
		
		m_NpcList->StopDrag();
		m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_POLL;
		m_uiItemID		=	0;
		m_siSelectLB	=	0;
		m_uiQuantity	=	0;
		m_siPos			=	-1;
	}
	else
	{
		m_NpcList->StopDrag();
		
		m_siBuildStatus	=	ON_FARM_BUILD_STOREHOUSE_POLL;
		m_uiItemID		=	0;
		m_siSelectLB	=	0;
		m_uiQuantity	=	0;
		m_siPos			=	-1;
	}

	return 0;
}

UI16 OnlineVillageStructFarm::FarmProductInfo(BYTE *pMsg)
{
	stFacility *pFacility;
	CItemHeader	*pItem;
	SI16 ix, i, siTempLine=0;
	CHAR szTemp[512];

	if(m_siStatus == ON_FARM_ITEM_MAKE)
	{
		OnResFarmWorkInfoMsg *pOnResFarmWorkInfoMsg = (OnResFarmWorkInfoMsg*)pMsg;
		
		if( pOnResFarmWorkInfoMsg->dwResponse == ON_RET_OK )
		{
			if( m_Building[ m_siSelect ].m_pWorkInfo ) delete m_Building[ m_siSelect ].m_pWorkInfo;
			
			m_Building[ m_siSelect ].m_pWorkInfo	=	NULL;
			m_siBuildSelect							=	-1;
			m_uiItemID								=	0;
			m_siSelectLB							=	0;
			m_siPos									=	-1;
			m_bRecvOk								=	TRUE;
			m_siBuildStatus							=	ON_FARM_ITEM_MAKE_POLL;		
			
			m_BBack.SetAction( TRUE );
			m_SelectControl.DelAllSell();		
			
			OnResFarmWorkInfoOkMsg		*pOnResFarmWorkInfoOkMsg;
			
			pOnResFarmWorkInfoOkMsg	=	(OnResFarmWorkInfoOkMsg*)pOnResFarmWorkInfoMsg;
			m_siShowFont			=	ON_FARM_SHOW_ITEM_MAKE;
			
			// 생산 중인 아이템을 서버로 부터 받은 다음 각 해당 구조체에 넣어 준다.
			m_Building[ m_siSelect ].m_pWorkInfo					=	new BuildWorkInfo;
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex		=	pOnResFarmWorkInfoOkMsg->uiItem;
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiNum			=	pOnResFarmWorkInfoOkMsg->uiNum;
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiFinalNum		=	pOnResFarmWorkInfoOkMsg->uiFinalNum;
			m_Building[ m_siSelect ].m_uiPay						=	pOnResFarmWorkInfoOkMsg->siPay;
			
			// 만약 건물을 짓고 있는 중이라면 
			if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
			{
//				m_BMain01.SetAction( FALSE );
				m_BMain03.SetAction( FALSE );
				m_BMain02.SetAction( FALSE );
			}
			
			//m_BOk.SetX(702);
			m_BOk.SetAction( FALSE );
			
			// 건물에 들어갔음 설정
			m_bInFactory = TRUE;
		}
		else
		{
			// 생산 가능한 모든 아이템의 수를 알아 본다.				
			SI16	siTempCount = 0, siTempCount1 = 0;
			m_siShowFont		=	ON_FARM_SHOW_BUILD_CONTROL;
			pFacility			=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
			m_siBuildStatus		=	ON_FARM_ITEM_MAKE_POLL;
			
			// 일단 생산이 불가능하게 말들어 준다
			m_SelectControl.DelAllSell();
			m_BOk.SetAction( FALSE );
			m_BMain03.SetAction( TRUE );
			
			if( !pFacility )	return 0;
			if( m_pItemID )		delete [] m_pItemID;				
			
			// 슬롯의 렙에 따라 생산 가능한 아이템 수가 틀리다.
			switch( m_Building[ m_siSelect ].m_uiSlotLevel )
			{
			case 1:		siTempCount	=	ON_FARM_LEV1_PRODUCT;										break;
			case 2:		siTempCount	=	ON_FARM_LEV2_PRODUCT;										break;
			case 3:		siTempCount	=	ON_FARM_LEV3_PRODUCT;		m_BMain02.SetAction(FALSE);		break;
			}
			
			m_pItemID	=	new UI16[ siTempCount ];
			
			for( i=0 ; i<siTempCount; i++ )  
			{
				pItem	=		m_pMyOnlineWorld->pItemsInfo->GetItem( pFacility->uiPr[m_pMyOnlineWorld->pOnlineVillage->GetNation()-1][i] );
				if( pItem )	
				{
					m_SelectControl.AddSell( (LPVOID)siTempCount1, m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ) );
					m_pItemID[ siTempCount1 ] = pFacility->uiPr[m_pMyOnlineWorld->pOnlineVillage->GetNation()-1][i];
					siTempCount1++;
				}
			}
		}	
		
		// 모든 과정이 성공적으로 끝이 나면.. 버튼을 활성화 한다.
		//m_BOk.SetX(688);
		//m_BOk.SetY(425);
		m_BExit.SetAction( FALSE );
		m_BBack.SetAction( TRUE );
		m_BOk.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRODUCTION));
	}
	else if(m_siStatus == ON_FARM_WORKER)
	{
		OnResFarmWorkInfoMsg *pOnResFarmWorkInfoMsg = (OnResFarmWorkInfoMsg*)pMsg;
		
		m_BExit.SetAction( FALSE );
		if( pOnResFarmWorkInfoMsg->dwResponse == ON_RET_OK )
		{
			if( m_Building[ m_siSelect ].m_pWorkInfo )	delete m_Building[ m_siSelect ].m_pWorkInfo;				
			m_Building[ m_siSelect ].m_pWorkInfo = NULL;
			
			m_BOk.SetAction( FALSE );
			m_BStop.SetAction( FALSE );
			m_BModify.SetAction ( FALSE );
			
//			m_BMain01.SetAction( FALSE );
			m_BMain03.SetAction( FALSE );
			m_BMain02.SetAction( FALSE );			
			
			OnResFarmWorkInfoOkMsg *pOnResFarmWorkInfoOkMsg = (OnResFarmWorkInfoOkMsg*)pOnResFarmWorkInfoMsg;;
			m_siBuildStatus	= ON_FARM_WORK_POLL;
			
			pFacility	=	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
			
			if( m_Building[ m_siSelect ].m_pWorkInfo )	delete m_Building[ m_siSelect ].m_pWorkInfo;
			m_Building[ m_siSelect ].m_pWorkInfo					=	new BuildWorkInfo;
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex		=	pOnResFarmWorkInfoOkMsg->uiItem;
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiNum			=	pOnResFarmWorkInfoOkMsg->uiNum;
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiFinalNum		=	pOnResFarmWorkInfoOkMsg->uiFinalNum;
			m_Building[ m_siSelect ].m_uiPay						=	pOnResFarmWorkInfoOkMsg->siPay;
			
			// 시설 향상중이다.
			if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
			{
				m_siMiniGameType	=	MINIGAME_TYPE_1;
				m_siBuildIndex		=   ON_FARM_STRUCT_MAKE;
				
				m_SelectWorkMaterial.DelAllSell();
				
				m_SelectWorkMaterial.AddSell((LPVOID)siTempLine, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_BULDING));
				siTempLine++;
				
				sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_TARGETBUILD), m_Building[ m_siSelect ].m_pWorkInfo->m_uiFinalNum );
				m_SelectWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
				siTempLine++;
				
				sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAY), m_Building[ m_siSelect ].m_uiPay );
				m_SelectWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
				siTempLine++;
			}
			// 아이템 생산중이다.
			else
			{
				if( strcmp( "FAR", m_szKind ) == 0 )
					m_siMiniGameType	=	MINIGAME_TYPE_3;
				else
					m_siMiniGameType	=	MINIGAME_TYPE_2;

				m_siBuildIndex			=	ON_FARM_STRUCT_ITEM_MAKE;
				
				CItemHeader	*pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex);	
				if( pItem )
				{
					m_SelectItemWorkMaterial.DelAllSell();
					
					sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK01 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), pItem->pMakeItem->m_siProduction );
					m_SelectItemWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
					siTempLine++;
					
					if( pItem->pMakeItem )
					{
						CItemHeader *pTempItem;
						
						for( ix=0 ; ix<ON_ITEM_MAX_ADDMAKE; ix++ )
						{
							if( pItem->pMakeItem->m_uiItemID[ix] != 0 )
							{
								pTempItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( pItem->pMakeItem->m_uiItemID[ix] );
								
								sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK02 ), m_pMyOnlineWorld->pOnlineText->Get( pTempItem->m_siItemName ), pItem->pMakeItem->m_siItemNum[ix] );
								m_SelectItemWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
								siTempLine++;
							}
						}
						
						for( ix=0 ; ix<ON_ITEM_MAX_TOOL; ix++ )
						{
							if( pItem->pMakeItem->m_uiToolID[ix] != 0 )
							{
								pTempItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( pItem->pMakeItem->m_uiToolID[ix] );
								
								sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK05 ), m_pMyOnlineWorld->pOnlineText->Get( pTempItem->m_siItemName ) );
								m_SelectItemWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
								siTempLine++;
							}
						}
					}
					
					sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK03 ), pItem->pMakeItem->m_siLabor );
					m_SelectItemWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
					siTempLine++;
					
					sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WORK04 ) );
					m_SelectItemWorkMaterial.AddSell((LPVOID)siTempLine, szTemp);
					siTempLine++;
				}
			}
			
			// 주인일 경우 임금 변경, 작업 중지, 메인 버튼을 누를수가 있따
			if( m_pMakeList->GetMaster() )
			{
				// 임금 변경 버튼
				m_BModify.SetAction (TRUE);
				// 작업 중지 버튼
				m_BStop.SetAction(TRUE);
			}
			
			// 일하기 버튼
			m_BOk.SetAction( TRUE );
			
			m_BOk.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING));
			m_BStop.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKSTOP));
			m_BModify.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAYCHANGE) );
			m_BBack.SetAction( TRUE );
			
			// 건물에 들어갔음 설정
			m_bInFactory = TRUE;
		}
		else
		{
			if( m_Building[ m_siSelect ].m_pWorkInfo )	delete m_Building[ m_siSelect ].m_pWorkInfo;				
			m_Building[ m_siSelect ].m_pWorkInfo =	NULL;
			m_siBuildStatus						 =	ON_FARM_WORK_POLL;
			
			m_BOk.SetAction( FALSE );
			m_BStop.SetAction( FALSE );
			m_BModify.SetAction ( FALSE );
			
//			m_BMain01.SetAction( FALSE );
			m_BMain03.SetAction( FALSE );
			m_BMain02.SetAction( FALSE );
			
			m_BOk.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING) );
			m_BStop.SetMenuName( m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKSTOP) );
			m_BModify.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PAYCHANGE));
			m_BBack.SetAction( TRUE );
		}
	}
	return 0;
}

UI16 OnlineVillageStructFarm::FarmProduct(BYTE *pMsg)
{
	stFacility		*pFacility;
	CItemHeader		*pItem;

	OnResFarmItemProductMsg *pOnResFarmItemProductMsg = (OnResFarmItemProductMsg*)pMsg;
	
	m_siBuildStatus		=	ON_FARM_ITEM_MAKE_POLL;			
	
	// 서버로 응답 메세지가 오면 4개의 메인 버튼과 어케이 버튼을 활성화 한다.
	m_BOk.SetAction( TRUE );
//	m_BMain01.SetAction( TRUE );
	m_BMain02.SetAction( TRUE );
	m_BMain03.SetAction( TRUE );
	
	if( pOnResFarmItemProductMsg->dwResponse == ON_RET_OK )
	{
		m_SelectControl.DelAllSell();
		
		pFacility =	m_pMyOnlineWorld->pOnlineBuilding->GetFacility( m_Building[ m_siSelect ].m_szKind );
		
		pItem = m_pMyOnlineWorld->pItemsInfo->GetItem( m_pItemID[ m_siBuildSelect ] );				
		
		if(m_Building[ m_siSelect ].m_pWorkInfo )	delete m_Building[ m_siSelect ].m_pWorkInfo;
		m_Building[ m_siSelect ].m_pWorkInfo				=	new BuildWorkInfo;
		m_Building[ m_siSelect ].m_pWorkInfo->m_uiFinalNum	=	m_siQuantity;				
		m_Building[ m_siSelect ].m_pWorkInfo->m_uiNum		=	0;
		m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex	=	m_pItemID[m_siBuildSelect];
		m_Building[ m_siSelect ].m_uiPay					=	m_siPay;	
		m_Building[ m_siSelect ].m_uiWork					=	pItem == NULL ? 0 : ((pItem->pMakeItem->m_siLabor * m_siQuantity)/pItem->pMakeItem->m_siProduction);
		
		
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() - (MONEY)( m_Building[ m_siSelect ].m_uiWork * (m_siPay + ON_FARM_TAX_RATE) ) );
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_ITEMPRODUCTION) );
		
		// 아이템 생산이 성공을 하면 뒷 보드를 아이템 생산용 그림으로 바꺼 주고
		// 아이템 생산을 생산을 할 수 있도록 m_siStatus를 생산으로 변경을 한다.
		m_siShowFont		= ON_FARM_SHOW_ITEM_MAKE;
		m_siStatus			= ON_FARM_WORKER;
		m_siMiniGameType	= 2;
		
//		m_BMain01.SetAction( FALSE );
		m_BMain02.SetAction( FALSE );
		m_BMain03.SetAction( FALSE );
		
		m_BModify.SetAction( TRUE );
		m_BStop.SetAction( TRUE );
		m_BOk.SetAction( TRUE );
		m_BOk.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WORKING));
		
		// 공장 들어감
		m_bInFactory = TRUE;
	}
	// 서버에서 아이템 생산 실패 에러 메세지들...
	else if( pOnResFarmItemProductMsg->dwResponse == ON_RET_NO )						// 실패
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_ITEMPRODUCTIONFAIL) );
	
	else if( pOnResFarmItemProductMsg->dwResponse == ON_RET_FARM_MATERIAL )				// 재료가 부족
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_MATERIALNO) );
	
	else if( pOnResFarmItemProductMsg->dwResponse == ON_RET_FARM_TOOL )					// 도구가 없다
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_WHEREHOUSEITEMNO) );
	
	else if( pOnResFarmItemProductMsg->dwResponse == ON_RET_FARM_MONEY )				// 돈이 없다
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_NOMONEY) );
	
	else if( pOnResFarmItemProductMsg->dwResponse == ON_RET_FARM_NOTOWNER )				// 농장 주인이 아니다 
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_ONLYMASTER) );			
	
	else if( pOnResFarmItemProductMsg->dwResponse == ON_RET_FARM_ITEMERROR )			//	그런 아이템은 생산할 수 없다.
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_NOPRODUCTIONITEM));
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmProductCancel(BYTE *pMsg)
{
	OnResPlantCancelPlanMsg	*pOnResPlantCancelPlanMsg = (OnResPlantCancelPlanMsg*)pMsg;

	if( pOnResPlantCancelPlanMsg->dwResponse == ON_RET_OK )
	{
		m_bInFactory = FALSE;
		
		m_siBuildStatus	=	ON_FARM_WORK_EXIT;
		
		// 나의 돈을 올려 준다.
		m_pMyOnlineWorld->pMyData->SetMoney( pOnResPlantCancelPlanMsg->ChangedMoney );
		
		m_Building[ m_siSelect ].m_uiPay	= 0;
		m_Building[ m_siSelect ].m_uiWork	= 0;			
		
		// 만약 건물을 건설중이고, 슬롯 레벨이 1인 상태에서 작업 중지를 시키면 건물을 제거 한다.
		if( (m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE) && (m_Building[ m_siSelect ].m_uiSlotLevel == 0) )
		{
			SI16	siXpos[ ON_MAX_FACTORY_NUM ]   = { 78, 173, 358, 546, 467, 265 };
			SI16	siYpos[ ON_MAX_FACTORY_NUM ]   = { 389, 280, 234, 281, 389, 426 };
			SI16	iy, ix;
			
			m_Building[ m_siSelect ].m_bBuilding		=	FALSE;
			m_Building[ m_siSelect ].m_siBuilingName	=	0;
			m_Building[ m_siSelect ].m_siAniFrame		=	0;
			m_siXpos[ m_siSelect ]						=	siXpos[ m_siSelect ];
			m_siYpos[ m_siSelect ]						=	siYpos[ m_siSelect ];
			
			
			if( m_pBuildButton[m_siSelect] )	delete [] m_pBuildButton[m_siSelect];
			if( m_pCheckButton[m_siSelect] )	delete [] m_pCheckButton[m_siSelect];
			
			
			m_pBuildButton[m_siSelect] = NULL;
			m_pCheckButton[m_siSelect] = NULL;
			m_pCheckButton[m_siSelect] = new UI08[ m_TitleSpr.Header.Xsize * m_TitleSpr.Header.Ysize ];
			
			
			if( clGrp.LockSurface( m_pCheckButton[m_siSelect], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize ) )
			{
				clGrp.PutImageT( 0, 0, m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize, &m_TitleSpr.Image[ m_TitleSpr.Header.Start[ m_siSelect ] ] );
				
				for( iy = 0 ; iy<m_TitleSpr.Header.Ysize ; iy++ )
					for( ix = 0; ix<m_TitleSpr.Header.Xsize ; ix++)
					{
						if( m_pCheckButton[m_siSelect][ iy * m_TitleSpr.Header.Xsize + ix ] !=  TRANSCOLOR )
							m_pCheckButton[m_siSelect][ iy * m_TitleSpr.Header.Xsize + ix ]  =  ON_BUTTON_CHECK_SAMPLE_VALUE;
					}
					
					m_Building[ m_siSelect ].m_pBuilding.Create( siXpos[ m_siSelect ], siYpos[ m_siSelect ], m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize,
						m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_VOID ), m_pCheckButton[m_siSelect], ON_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_LEFT );
			}
		}
	}
	else
	{
		m_siBuildStatus	=	ON_FARM_WORK_POLL;
	}
	return 0;
}

UI16 OnlineVillageStructFarm::FarmProductPayChange(BYTE *pMsg)
{
	OnResPlantChangePayMsg	*pOnResPlantChangePayMsg = (OnResPlantChangePayMsg*)pMsg;

	if( pOnResPlantChangePayMsg->dwResponse == ON_RET_OK )
	{
		m_pMyOnlineWorld->pMyData->SetMoney( pOnResPlantChangePayMsg->ChangedMoney );
		m_Building[ m_siSelect ].m_uiPay = m_siPay;
		m_siBuildStatus					 = ON_FARM_WORK_POLL;
		
	}
	else
	{
		m_siBuildStatus = ON_FARM_WORK_POLL;
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_BOARD_DELFAIL), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_CHANGEPAYFAIL) );
	}
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmWorkStart(BYTE *pMsg)
{
	OnResFarmWorkStartMsg *pOnResFarmWorkStartMsg = (OnResFarmWorkStartMsg*)pMsg;

	if( pOnResFarmWorkStartMsg->dwResponse == ON_RET_OK )
	{
#ifdef _OGG_PLAY
		// actdoll (2004/12/01 15:20) : 
		//	현재 재생되고 있는 BGM을 미리 기억해 놓고 미니게임을 나갈 때 그놈을 연주한다.
		GetBgmMgr()->SetNextBgm( GetBgmMgr()->GetCurrentBgmIndex() );
		GetBgmMgr()->Close();
#else
		// 배경음을 끈다.
		g_pSoundManager->StopSound( m_pMyOnlineWorld->pOnlineVillage->BackgroundMusic );
#endif
		
		m_siBuildStatus	=	ON_FARM_WORK_PLAY_MINIGAME;
		m_bMiniGame		=	TRUE;
		
		if(m_pMyOnlineWorld->m_pMercenary->IsAction())
			m_pMyOnlineWorld->m_pMercenary->OnButtonPush();

		m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( FALSE );
		InitMiniGame( m_siMiniGameType, m_pMyOnlineWorld );
	}
	else
	{
		m_siBuildStatus	=	ON_FARM_WORK_POLL;
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_TOOLNO), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_TOOLNOWORKNO) );
		m_BExit.SetAction( TRUE );
		m_BOk.SetAction( TRUE );
		m_BBack.SetAction( TRUE );
		m_BStop.SetAction( TRUE );
		m_BModify.SetAction( TRUE );
	}
	
	return 0;
}

UI16 OnlineVillageStructFarm::FarmWorkResult(BYTE *pMsg)
{
	OnResFarmWorkMsg *pOnResFarmWorkMsg = (OnResFarmWorkMsg*)pMsg;
	
	if(pOnResFarmWorkMsg->dwResponse == ON_RET_OK)
	{
		m_siBuildStatus						=	ON_FARM_WORK_POLL;
		
		m_Building[ m_siSelect ].m_uiWork	=	pOnResFarmWorkMsg->uiSlotRemainLabor;
		m_BBack.SetAction( TRUE );
		
		m_pMyOnlineWorld->pMyData->SetMoney( m_pMyOnlineWorld->pMyData->GetMoney() + pOnResFarmWorkMsg->siPay );
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_GIVEPAY), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_GIVEPAY1), pOnResFarmWorkMsg->siPay );
		
		
		// 남은 작업량이 0이면 작업끝
		if( m_Building[ m_siSelect ].m_uiWork == 0 )
		{
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiFinalNum	= 0;
			m_Building[ m_siSelect ].m_uiPay  = 0;
			
			if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
				m_Building[ m_siSelect ].m_uiSlotLevel++;
			
			m_bInFactory = FALSE;
			m_siBuildStatus	= ON_FARM_WORK_EXIT;					
		}
	}
	else
	{
		if(m_bWorkCompleted)
		{
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FARM_FINISH_WORK));
			m_Building[ m_siSelect ].m_pWorkInfo->m_uiFinalNum	= 0;
			m_Building[ m_siSelect ].m_uiPay  = 0;
			
			if( m_Building[ m_siSelect ].m_pWorkInfo->m_uiItemIndex == ON_PLANT_SLOTUPGRADE_CODE )
				m_Building[ m_siSelect ].m_uiSlotLevel++;
			
			m_bInFactory = FALSE;
			m_bWorkCompleted = FALSE;
			
			m_siBuildStatus	= ON_FARM_WORK_EXIT;
		}
		else if(m_siMiniGameScore > 0 && m_siMiniGameScore < 4000)
		{
			m_siBuildStatus	= ON_FARM_WORK_POLL;
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WORK_FAIL) );
		}
		else
			m_siBuildStatus	= ON_FARM_WORK_POLL;
	}
	
	m_BExit.SetAction( TRUE );
	m_BOk.SetAction( TRUE );
	m_BBack.SetAction( TRUE );
	m_BStop.SetAction( TRUE );
	m_BModify.SetAction( TRUE );
	
	return 0;
}

