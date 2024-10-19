/**************************************************************************************************
	파일명 : OnlinePannel.cpp
	담당자 : 정진욱

	하단 판넨 인터페이스
**************************************************************************************************/

#include <GSL.h>

#include <mouse.h>
#include <clGame.h>


#include "OnlineWorld.h"
#include "OnlineTradeBook.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineETC.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include "OnlineMyData.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include "OnlineFieldChat.h"
#include "OnlineMiniMap.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "OnlineImoticon.h"
#include "OnlineTradeParser.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlineSetting.h"
#include "OnlineTrade.h"
#include "OnlineTip.h"
#include "OnlineFieldAuction.h"
#include "OnlineKeyboard.h"
#include "OnlineMessenger.h"
#include "OnlineCharStatus.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineNoticeList.h"
#include "OnlineVillage.h"
#include "OnlineParty.h"
#include <Common.h>
#include "OnlineChatMacro.h"
#include "OnlineQuestInterface.h"
#include "OnlineKeyboard.h"
#include "OnlineMercenary.h"

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
#include "..\GersangUI\UIGSInfoHeader.h"
#include "..\GersangUI\UIGSPreCompileHeader.h"
#include "..\GersangUI\UIGSMgr.h"
#include "..\GersangUI\UIGSSurfaceBufferMgr.h"
//<-- 
#endif

// robypark 2005/1/27 22:6
#include "SiegeWarfareUnitUI.h"
#include "SiegeWarfareTransformUIWnd.h"

#include	"OnlineQuestPlay.h"

#include <OnlineCharUI.h>

extern	_clGame* pGame;
extern _InputDevice				IpD;

#define GAUGE_WIDTH			284
#define MAX_CHAT_BUFFER		50



enum
{   
	ON_PANNEL_POLL					= 0,
	ON_PANNEL_TOGLEMODE_CHANGE
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OnlinePannel::OnlinePannel()
{
	m_pMyOnlineWorld			= NULL;
	m_pChatControl				= NULL;
	m_pSMS						= NULL;

	m_siDisableFlag             = NULL;
	m_pChatControl			= NULL;

	TempYetMoney		= 0xffffffffffffffff;
	TempYetBattlePower	= 0xffffffff;
	TempYetCredit		= 0xffffffff;
	m_bHomeKey			= FALSE;

	// robypark 2005/1/27 22:6
	m_pSiegeWarfareUnitUI			= NULL;			// 공성전 유닛일 경우 인터페이스
	m_pSiegeWarfareTransformUIWnd	= NULL;			// 공성전 유닛으로 변신 윈도
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OnlinePannel::~OnlinePannel()
{
	Free();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	재정비
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::Initialize()
{
	m_siPollStatus			=	ON_PANNEL_POLL;
	m_bShowMiniMap			=	FALSE;
	m_bMouseDown			=	FALSE;
	m_bIsInVillage			=	FALSE;
	m_bMessageShow			=	FALSE;
	m_siDisableFlag         =	NULL;
	m_bHomeKey				=	FALSE;

//	if( m_pChatControl )	m_pChatControl->Initialize();
	if( m_pSMS )			m_pSMS->Initialize();

	// 채팅 컨트롤의 기본값을 각 국가별 언어체계로 맞춰놓는다.
	OnlineEditControlEx	*pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
//	if( pOecEx )		pOecEx->GetIMEBase()->SetInputStatus( IME_CMODE_NATIVE );


	TempYetMoney		= 0xffffffffffffffff;
	TempYetBattlePower	= 0xffffffff;
	TempYetCredit		= 0xffffffff;

	// 패널 전체
	m_pt_Pannel.x			=	0;
	m_pt_Pannel.y			=	ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize;
	ChangeChatBasePos( (m_pSMS)?m_pSMS->GetCurrentYSize():0 );

	SetWhisperList(FALSE);
	m_siWhisperSel = 0;
	m_WhisperList.clear();

	// robypark 2005/1/27 22:6
	// 공성전 유닛일 경우 인터페이스
	if ((m_pSiegeWarfareUnitUI->IsVisibleWindow())
		|| (m_pSiegeWarfareUnitUI->IsMinimizeWindow()))
	{
		m_pSiegeWarfareUnitUI->SiegeWarfareUnitUI_DestroyWindow();
	}

	// 공성전 유닛으로 변신 윈도
	if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
	{
		m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);
		m_pSiegeWarfareTransformUIWnd->SetSendedMsg(FALSE);

	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::Init(cltOnlineWorld *pOnlineWorld)
{
	m_pMyOnlineWorld		=	pOnlineWorld;
	m_siPollStatus			=	ON_PANNEL_POLL;
	m_bShowMiniMap			=	FALSE;
	m_bMouseDown			=	FALSE;
	m_bIsInVillage			=	FALSE;
	m_bMessageShow			=	FALSE;
	m_siDisableFlag         =	NULL;
	m_bWhisperMode			=	FALSE;
	m_siWhisperSel			=	0;
 
	// 반드시 리소스가 초기화 되어 있어야 한다.
	if(m_pMyOnlineWorld->pOnlineResource == NULL)
		clGrp.Error("OnlinePannel::Init", "m_pMyOnlineWorld->pOnlineResource == NULL");

	// 이미지 로드
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_panel.spr",					m_SprImg_Pannel);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_Chat_panel.spr",				m_SprImg_PannelChatLine);

	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_a.Spr",				m_SprBtn_Inventory);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_b.Spr",				m_SprBtn_CharStatus);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_c.Spr",				m_SprBtn_Account);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_d.Spr",				m_SprBtn_Mercenary);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_e.Spr",				m_SprBtn_Quest);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_f.Spr",				m_SprBtn_Friends);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_menu_icon_g.Spr",				m_SprBtn_Option);

	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_mid_button.Spr",				m_SprBtn_Common);

	clGrp.LoadXspr("Online\\GameSpr\\Panel\\MAIN_HUNGRY_GAGE.Spr",				m_SprGauge_Hungry);

	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_chat_sidebutton_list.Spr",		m_SprBtn_ShowWhisperList);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_chat_sidebutton_size.Spr",		m_SprBtn_ChangeChatWindow);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_chat_sbutton_emoticon.Spr",	m_SprBtn_ShowImoticon);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_chat_sbutton_macroplay.Spr",	m_SprBtn_MacroPlayPause);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_chat_sbutton_macroset.Spr",	m_SprBtn_MacroSetting);
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\main_chat_list.Spr", 				m_SprBtn_WhisperDestID);

	clGrp.LoadXspr("Online\\GameSpr\\Pannel_Messenger.Spr",						m_SprIcon_Messenger);

	// robypark 2004/11/13 13:58
	// 공성전 유닛 상태 정보 보기 버튼 스프라이트 로드
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_UNIT_INFO_BASE.Spr",					m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis);	// 버튼 테두리
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_B.Spr",							m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton);	// 버튼 이미지
	
	m_siPannelNoField[0] = pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize, 200, ON_SCREEN_YSIZE);
	m_siPannelNoField[1] = pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(200, ON_SCREEN_YSIZE - 25, 600, ON_SCREEN_YSIZE);
	m_siPannelNoField[2] = pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(600, ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize, ON_SCREEN_XSIZE, ON_SCREEN_YSIZE);

	// 공성전 유닛인 상태에서 모든 공성전 유닛보기 버튼 인터페이스 No Field Area Index
	m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle = -1;

	// SMS 컨트롤 생성(한국만 가능)
	if( g_stConnectServerInfo.m_iServiceNation == DF_CODE_NATION_IDENTITY_KOREA ) 
	{
		m_pSMS = new OnlineSMS;
		m_pSMS->Init(pOnlineWorld);
	}

	// 채팅 컨트롤 생성
	m_pChatControl = new OnlineFieldChat;
	m_pChatControl->Init(pOnlineWorld, ((m_pSMS)?m_pSMS->GetCurrentYSize():0) );
	m_pChatControl->SetDistY( ((m_pSMS)?m_pSMS->GetCurrentYSize():0) );

	// 패널 전체
	m_pt_Pannel.x			=	0;
	m_pt_Pannel.y			=	ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize;
	// 패널 채팅 입력 라인
	m_pt_PannelChatLine.x	=	201;
	m_pt_PannelChatLine.y	=	ON_SCREEN_YSIZE - m_SprImg_PannelChatLine.Header.Ysize - ((m_pSMS)?m_pSMS->GetCurrentYSize():0);
	
	// 버튼을 생성 한다.
	m_Btn_ShowWhisperList.Create	(206,578 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_ShowWhisperList.Header.Xsize,	m_SprBtn_ShowWhisperList.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000512),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_ChangeChatWindow.Create	(575,578 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_ChangeChatWindow.Header.Xsize,	m_SprBtn_ChangeChatWindow.Header.Ysize,	NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_ShowImoticon.Create		(524,580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_ShowImoticon.Header.Xsize,		m_SprBtn_ShowImoticon.Header.Ysize,		NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroPlayPause.Create		(539,580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_MacroPlayPause.Header.Xsize,		m_SprBtn_MacroPlayPause.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000513),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroSetting.Create		(554,580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_MacroSetting.Header.Xsize,		m_SprBtn_MacroSetting.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000514),	BUTTON_PUT_LEFT, TRUE );

	m_Btn_Inventory.Create	(619,519, m_SprBtn_Inventory.Header.Xsize,	m_SprBtn_Inventory.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000515),							BUTTON_PUT_LEFT, TRUE );	// 인벤토리.
	m_Btn_CharStatus.Create	(644,519, m_SprBtn_CharStatus.Header.Xsize,	m_SprBtn_CharStatus.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_STATUS),				BUTTON_PUT_LEFT, TRUE );	// 캐릭터 상태.
	m_Btn_Account.Create	(669,519, m_SprBtn_Account.Header.Xsize,	m_SprBtn_Account.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_MYMONEY),				BUTTON_PUT_LEFT, TRUE );	// 장부
	m_Btn_Mercenary.Create	(694,519, m_SprBtn_Mercenary.Header.Xsize,	m_SprBtn_Mercenary.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SOLIDER),					BUTTON_PUT_LEFT, TRUE );	// 용병
	m_Btn_Quest.Create		(719,519, m_SprBtn_Quest.Header.Xsize,		m_SprBtn_Quest.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000516),							BUTTON_PUT_LEFT, TRUE );	// 도움말.
	m_Btn_Friends.Create	(744,519, m_SprBtn_Friends.Header.Xsize,	m_SprBtn_Friends.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIENDLISTINFO_FRIENDLIST),	BUTTON_PUT_LEFT, TRUE );	// 친구목록.
	m_Btn_Option.Create		(769,519, m_SprBtn_Option.Header.Xsize,		m_SprBtn_Option.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000517), BUTTON_PUT_LEFT, TRUE );	// 셋팅

	m_Btn_AggressiveMode.Create	(650,554, m_SprBtn_Common.Header.Xsize, m_SprBtn_Common.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PEACEMODE), BUTTON_PUT_LEFT, TRUE );
	m_Btn_TradePermission.Create(704,554, m_SprBtn_Common.Header.Xsize, m_SprBtn_Common.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PERMIT_TRADE), BUTTON_PUT_LEFT, TRUE );

	/*
	// robypark 2004/11/13 14:3
	// 공성전 상태 정보 보이기 버튼
	m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.Create(710 + 3, 447 + 3, 
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Xsize,
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Ysize,
				m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SHOW_SIEGEWARFAREUNIT_INFORMATION),
				BUTTON_PUT_LEFT, TRUE );

	// robypark 2005/1/5 17:18
	// 공성전 유닛에서 => 일반 유닛으로 변신 요청 버튼
	m_sShowInformationAboutSiegeWarfareUnit.m_Btn_RequestReturnNormalCharacter.Create(710 + 3, 477 + 3,
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Xsize,
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Ysize,
				m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANCEL_TRANSFORM),
				BUTTON_PUT_LEFT, TRUE );
	*/
	m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.Create(710 + 3, 477 + 3,
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Xsize,
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Ysize,
				m_pMyOnlineWorld->pOnlineText->Get(8100107),
				BUTTON_PUT_LEFT, TRUE );

	SetRect( &m_rt_Gauge_Hungry,	80,		523,				80 + m_SprGauge_Hungry.Header.Xsize,	523 + m_SprGauge_Hungry.Header.Ysize );
	SetRect( &m_rt_MyMoney,			39,		580,				39 + 147,								580 + 14 );
	SetRect( &m_rt_MyGrade,			127,	559,				127 + 59,								559 + 14 );
	SetRect( &m_rt_MyAttackPower,   39,		559,				39 + 48,								559 + 14 );
	SetRect( &m_rt_Date,			637,	581,				637 + 134,								581 + 14 );
	SetRect( &m_rt_Messenger,		170,	m_pt_Pannel.y + 32, 170 + m_SprIcon_Messenger.Header.Xsize,	m_pt_Pannel.y + 30 + m_SprIcon_Messenger.Header.Ysize);	
//	SetRect( &m_rt_ChatEdit,		233,	579,				233+288,								579 + 15 );

	// 채팅용 에디트 컨트롤을 생성
	OnlineEditControlEx	*pOecEx;
	if( m_hOecChat )	Handler_OnlineEditControlEx::KillControl( m_hOecChat );
	RECT	rcRect = { 233, 580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), 233+288, 580+15 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0) };
	m_hOecChat		= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, MAX_CHAT_BUFFER);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
	pOecEx->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(255, 255, 255) );
	m_siFieldBlock_ChatLine	= pOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_PannelChatLine.x, m_pt_PannelChatLine.y, m_pt_PannelChatLine.x + m_SprImg_PannelChatLine.Header.Xsize, m_pt_PannelChatLine.y + m_SprImg_PannelChatLine.Header.Ysize);

	// 공지사항 위치 바꾸고
	m_pMyOnlineWorld->pOnlineNoticeList->SetPosition( rcRect.left, rcRect.top + 1, rcRect.right - rcRect.left, 4 );			// 공지사항 위치 바꾸고

	// robypark 2005/1/27 22:6
	// 공성전 유닛일 경우 인터페이스
	m_pSiegeWarfareUnitUI	= NULL;
	m_pSiegeWarfareUnitUI = new CSiegeWarfareUnitUI;
	m_pSiegeWarfareUnitUI->Init(pOnlineWorld);

	// 공성전 유닛으로 변신 윈도
	m_pSiegeWarfareTransformUIWnd = NULL;
	m_pSiegeWarfareTransformUIWnd = new CSiegeWarfareTransformUIWnd;
	m_pSiegeWarfareTransformUIWnd->Init(pOnlineWorld);
}	

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해제 함수.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::Free()
{
	Handler_OnlineEditControlEx::KillControl( m_hOecChat);

	clGrp.FreeXspr(m_SprImg_Pannel);
	clGrp.FreeXspr(m_SprImg_PannelChatLine);
	clGrp.FreeXspr(m_SprGauge_Hungry);
	clGrp.FreeXspr(m_SprBtn_ShowWhisperList);
	clGrp.FreeXspr(m_SprBtn_ChangeChatWindow);
	clGrp.FreeXspr(m_SprBtn_ShowImoticon);
	clGrp.FreeXspr(m_SprBtn_MacroPlayPause);
	clGrp.FreeXspr(m_SprBtn_MacroSetting);
	clGrp.FreeXspr(m_SprBtn_Inventory);
	clGrp.FreeXspr(m_SprBtn_CharStatus);
	clGrp.FreeXspr(m_SprBtn_Account);
	clGrp.FreeXspr(m_SprBtn_Mercenary);
	clGrp.FreeXspr(m_SprBtn_Quest);
	clGrp.FreeXspr(m_SprBtn_Friends);
	clGrp.FreeXspr(m_SprBtn_Option);
	clGrp.FreeXspr(m_SprBtn_Common);
	clGrp.FreeXspr(m_SprBtn_WhisperDestID);
	clGrp.FreeXspr(m_SprIcon_Messenger);

	// robypark 2004/11/13 13:58
	// 공성전 유닛 상태 정보 보기 버튼 스프라이트 해제
	clGrp.FreeXspr(m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis);		// 버튼 테두리
	clGrp.FreeXspr(m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton);	// 버튼 이미지

	if(m_pChatControl)
	{
		m_pChatControl->Free();
		delete m_pChatControl;
		m_pChatControl = NULL;
	}

	if( m_pSMS )
	{
		m_pSMS->Free();
		delete m_pSMS;
		m_pSMS = NULL;
	}

	// robypark 2005/1/27 22:6
	// 공성전 유닛일 경우 인터페이스
	if (m_pSiegeWarfareUnitUI)
	{
		delete m_pSiegeWarfareUnitUI;
		m_pSiegeWarfareUnitUI	= NULL;
	}

	// 공성전 유닛으로 변신 윈도
	if (m_pSiegeWarfareTransformUIWnd)
	{
		delete m_pSiegeWarfareTransformUIWnd;
		m_pSiegeWarfareTransformUIWnd = NULL;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	폴링 함수.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::Poll()
{
	POINT pt;
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;
	CHAR	szTemp[1024], szMyCharSpeechBuffer[1024];
	ZeroMemory( szTemp,  sizeof( szTemp ) );
	ZeroMemory( szMyCharSpeechBuffer,  sizeof( szMyCharSpeechBuffer ) );
	vector<WhisperData>::iterator it;

	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl(m_hOecChat);

	switch(m_siPollStatus)
	{
	case ON_PANNEL_POLL:
		{
			// 채팅관련(필드 상)
			if(m_pMyOnlineWorld->pOnlineVillage == NULL)
			{
				// 필드에서 리턴 키가 눌렸고 채팅 포커스가 없는 상태일 경우
				if ((m_pMyOnlineWorld->bReturnKeyDown)	// 리턴키가 눌렸고
					&& (!Handler_OnlineEditControlEx::IsFocus(m_hOecChat))	// 채팅 입력창에 포커스가 없고
					&& ((m_pMyOnlineWorld->GetGamePollStatus() == ON_GAMEPOLL_FIELD)	// 필드이거나
						|| (m_pMyOnlineWorld->GetGamePollStatus() == ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE)))	// 수성 인터페이스 상일 경우
				{
					m_pMyOnlineWorld->bReturnKeyDown = FALSE;		// 리턴 속성 제거

					// 귓말 활성화 상태일때
					if(m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						// 축소 상태라면
						if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
						{
							// 트레이드 북이나 이런 쪽에 포커스가 맞춰져 있지 않다면 채팅라인에 포커스를 맞춰라
							if(!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->GetHOECFriend()))
								Handler_OnlineEditControlEx::SetFocus( m_hOecChat);
						}
					}
					// 좌판이 활성화되어 있고 아이템을 넣는 중일때
					else if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn() && m_pMyOnlineWorld->pOnlineBooth->GetStatus() == ON_BENCH_IN_INPUT_ITEM_INFO)
					{
						// 좌판 입력창에 포커스를 맞춰라
						m_pMyOnlineWorld->pOnlineBooth->SetFocusEditBox();
					}
					// 장부의 친구목록에 포커스가 없고 캐릭터 상태창의 이름 교체 창에 포커스가 없다면
					else if(!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->GetHOECFriend()) &&
					!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->m_pOnlineCharStatus->GetHOECRename()))
					{
						// 채팅에 포커스를 준다.
						Handler_OnlineEditControlEx::SetFocus( m_hOecChat);
						pOecEx->Clear();
					}
				}	// if(m_pMyOnlineWorld->bReturnKeyDown && !Handler_OnlineEditControlEx::IsFocus(m_hOecChat) && m_pMyOnlineWorld->GetGamePollStatus() == ON_GAMEPOLL_FIELD)
			}	// if(m_pMyOnlineWorld->pOnlineVillage == NULL)
			// 채팅관련(마을 상)
			else
			{
				// 리턴 키가 눌렸고 채팅 포커스가 없는 상태일 경우
				if(m_pMyOnlineWorld->bReturnKeyDown && !Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
				{
					m_pMyOnlineWorld->bReturnKeyDown = FALSE;			// 리턴 속성 제거
					if(m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						;
					}
					//friend list 추가.추가추가
					else if(!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->m_pOnlineCharStatus->GetHOECRename()) &&
						!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->GetHOECFriend()))
					{
						//Handler_OnlineEditControlEx::ClearCurrentFocus( pGame->Hwnd );
						Handler_OnlineEditControlEx::SetFocus( m_hOecChat );
						pOecEx->Clear();
					}
				}
			}
				
			// 입력중인 에딧 박스에서 엔터키를 쳤다.
			if( pOecEx->IsPushEnter() )
			{
				if( Handler_OnlineEditControlEx::IsFocus( m_hOecChat ) )
				{
					// 나의 캐릭터가 말을 했다		
					strcpy( szMyCharSpeechBuffer, pOecEx->GetString() );
					
					// 스트링 정보가 정확하지 않으면 채팅 안뿌림
					if( !CheckLeadByte( szMyCharSpeechBuffer ) )		
					{
						pOecEx->Clear();
						Handler_OnlineEditControlEx::ClearCurrentFocus();
						break;
					}

					// 마을안에서 혹은 좌판모드에서
					if( m_pMyOnlineWorld->pOnlineVillage || m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )
					{
						if( GetChattingMode() != CHATTINGMODE_GUILD )	// 길드 챗이 아니라면
						{
							// 앞쪽에 귓속말 내용이 없다면 채팅 안뿌림
							if( strncmp( szMyCharSpeechBuffer, "/to ", 4 ) != 0 )
							{
								pOecEx->Clear();
								Handler_OnlineEditControlEx::ClearCurrentFocus();
								break;
							}
						}
					}

					// 레벨, 신용등급 5이하는 귓말을 할수없다.
					if( m_pMyOnlineWorld->pMyData->GetTradeGrade() <= 5 )
					{
						if( strnicmp( szMyCharSpeechBuffer, "/to ", 4 ) == 0 )
						{
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get( 8000518 ) );
							pOecEx->Clear();
							Handler_OnlineEditControlEx::ClearCurrentFocus();
							break;
						}
					}

					// 길드 채팅일 경우
					if( GetChattingMode() == CHATTINGMODE_GUILD )
					{
						// 운영자 명령인지 확인하고
						if( Check_ManagerOrder((char*)szMyCharSpeechBuffer ) == FALSE )
						{
							if( szMyCharSpeechBuffer[0] != NULL )
							{
								sprintf( (char*)szTemp, "/GUILD %s", szMyCharSpeechBuffer );
								strcpy( (char*)szMyCharSpeechBuffer, (char*)szTemp );
								ZeroMemory( szTemp, sizeof(szTemp) );
							}
						}
					}

					// 사용자가 수성 인터페이스에 있을 때 warp 명령으로 자신이나 다른 사용자를
					// 워프 시킬 수 없다.
					// 운영자 명령인지 확인하고
					if( Check_ManagerOrder((char*)szMyCharSpeechBuffer ) == TRUE )
					{
						// 워프 명령어이며
						if( strnicmp( szMyCharSpeechBuffer, "/w ", 3 ) == 0 )
						{
							// 보내려는 캐릭터 UniqueID 얻기
							CHAR			cSep[] = " ,";
							CHAR			*pToken;
							CHAR			szTempSpeechBuffer[1024];
							UI16			uiPlayerID;

							strcpy(szTempSpeechBuffer, szMyCharSpeechBuffer);

							// 명령어를 얻어온다.
							pToken = strtok(szTempSpeechBuffer, cSep);

							// 워프시키려는 캐릭터 uiPlayerID를 얻어온다.
							if((pToken	= strtok(NULL, cSep)) != NULL)
							{
								uiPlayerID	=	atoi(pToken);

								// 수성 인터페이스에 있는 캐릭터라면
								if (TRUE == m_pMyOnlineWorld->pIOnlineChar->IsInVillageDefenceInterface(uiPlayerID))
								{
									pOecEx->Clear();
									Handler_OnlineEditControlEx::ClearCurrentFocus();
									break;
								}
							}
						}
					}

					// 운영자 명령일 경우
					if( Check_ManagerOrder( (char*)szMyCharSpeechBuffer ) )
					{
						// 운영자 명령어 세팅
						Process_ManagerOrder( (char*)szMyCharSpeechBuffer );
						
						// 에디트 박스를 깨끗히 한다.
						pOecEx->Clear();
					}
					// 아니라면
					else
					{
						if( szMyCharSpeechBuffer[0] != NULL )
						{
							// 욕설 필터 체크
							m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(szMyCharSpeechBuffer);
							
							// 필드라면
							if( m_pMyOnlineWorld->pOnlineVillage == NULL )
							{
								// 내 캐릭터 이름을 받고
								CHAR *pCharName	= m_pMyOnlineWorld->pMyData->GetMyCharName();
								if( !strcmp( pCharName, m_pMyOnlineWorld->pOnlineText->Get( 8000495 ) ) )
									m_pMyOnlineWorld->pIOnlineChar->SetSpeech( m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), szMyCharSpeechBuffer, TRUE );
								else
									m_pMyOnlineWorld->pIOnlineChar->SetSpeech( m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), szMyCharSpeechBuffer );	
								
								// 거래중일때 
								if( m_pMyOnlineWorld->pOnlineTrade->IsActive() )
									AddChatID( m_pMyOnlineWorld->pMyData->GetMyCharName(), szMyCharSpeechBuffer, CHATTINGMODE_NORMAL );
								else if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
									m_pChatControl->AddChat(0, szMyCharSpeechBuffer, CHATTINGMODE_NORMAL);
							}

							// 서버에게 나의 말을 보내준다.
							if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
								m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendChatting(szMyCharSpeechBuffer);
							
							// 에디트 박스를 깨끗히 한다.
							pOecEx->Clear();
						}
						/*
						else
						{
							pOecEx->Clear();
							Handler_OnlineEditControlEx::ClearCurrentFocus();
						}
						*/
					}	// if( Handler_OnlineEditControlEx::IsFocus( m_hOecChat ) )
					
					pOecEx->Clear();
					Handler_OnlineEditControlEx::ClearCurrentFocus();

				}// if(Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
			}// if(pOecEx->IsPushEnter())

			// .을 찍으면 이전귓말 상대를 세팅해준다.
//			strcpy(szMyCharSpeechBuffer,pOecEx->GetString());
//
//			if(szMyCharSpeechBuffer[0] == '.')
//			{
//				SetWhisperList(TRUE);
//			}

			if(m_bWhisperMode)
			{
				if(m_bMouseDown)
				{
					int i;
					for(it = m_WhisperList.begin(), i=0; it != m_WhisperList.end(); it++, i++)
					{
						if(pt.x > it->siX && pt.x < (it->siX+89) && pt.y > it->siY && pt.y < (it->siY+16))
						{
							if(m_WhisperList.size() > m_siWhisperSel)
							{
								m_WhisperList[m_siWhisperSel].bSelect = FALSE;
								it->bSelect = TRUE;
								m_siWhisperSel = i;
								IpD.LeftPressSwitch = FALSE;
								SetWhisperList(FALSE);
							}
						}
					}
				}	// if(m_bMouseDown)

				if(LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN)))
					SetWhisperList(FALSE);

				if(LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_DOWN)))
				{
					if(m_WhisperList.size() > m_siWhisperSel)
						m_WhisperList[m_siWhisperSel].bSelect = FALSE;
					
					m_siWhisperSel--;
					if(m_siWhisperSel < 0)	m_siWhisperSel = 0;
					
					if(m_WhisperList.size() > m_siWhisperSel)
						m_WhisperList[m_siWhisperSel].bSelect = TRUE;
				}
				else if(LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_UP)))
				{
					if(m_WhisperList.size() > m_siWhisperSel)
						m_WhisperList[m_siWhisperSel].bSelect = FALSE;
					
					m_siWhisperSel++;
					if(m_siWhisperSel >= m_WhisperList.size())	m_siWhisperSel--;
					
					if(m_WhisperList.size() > m_siWhisperSel)
						m_WhisperList[m_siWhisperSel].bSelect = TRUE;
				}
			}	// if(m_bWhisperMode)
			
			// 포만감이 0이고, 내가 채팅중이 아닐때는
			if((m_pMyOnlineWorld->pMyData->GetHealth() == 0) && (m_pMyOnlineWorld->pIOnlineChar->CheckSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()) == FALSE)
				&& !m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
			{
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HUNGRY2));
				m_pChatControl->AddChat(0, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HUNGRY2), CHATTINGMODE_NORMAL);
			}
			
			// 마을 밖일 경우(필드)
			if(m_bIsInVillage == FALSE )
			{
				if (m_pMyOnlineWorld->pOnlineFieldAuction->IsEntered()) return;
				
				if( m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK && m_pMyOnlineWorld->m_bForcedLogout)			
					m_pMyOnlineWorld->ExitOnline();

				if( m_pChatControl )	m_pChatControl->Poll();

				// 채팅창 사이즈 바꾸는 버튼 누를때
				if( m_Btn_ChangeChatWindow.Process( m_bMouseDown ) == TRUE )
				{
					SetChatOpen();
				}

				// 채팅창 클릭 시 포커스
				if( pOecEx->IsPointInRect( pt ) && m_bMouseDown )
					Handler_OnlineEditControlEx::SetFocus( m_hOecChat );

				// 메세지 도착알림 클릭
				if(PtInRect(&m_rt_Messenger, pt) && m_pMyOnlineWorld->fLeftOneSwitch == 1)
				{
					/** @author 김성준 (sagolboss)
						@date   2004-08-09 오후 1:55
						버그리포트의 수정요청에 따라 신용등급 레벨 6이하는 사용하지 못한다는 메세지 박스를 띄웁니다
					*/
		
					if(m_pMyOnlineWorld->pMyData->GetTradeGrade() <= 5)
					{
						///--> 추가된 코드 시작 (sagolboss)
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8000693));
						pOecEx->Clear();
						Handler_OnlineEditControlEx::ClearCurrentFocus();
						///<-- 추가 코드 끝
					}
					else
					{
						if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
							m_pMyOnlineWorld->pOnlineMessenger->SetMini(FALSE);
						else
							m_pMyOnlineWorld->pOnlineMessenger->SetMini(TRUE);
					}
				}

				// SMS 출력 및 사이즈 바꾸는 버튼 누를때
				if(m_pSMS)			
				{
					m_pSMS->Action();
					if( m_pSMS->IsModeJustChanged() )	// 방금전에 SMS의 형태가 바뀌었다면
					{
						ChangeChatBasePos( m_pSMS->GetCurrentYSize() );													// 채팅창 위치 바꾸고
					}
				}


				/*
				else if( BTogleMode.Process( m_bMouseDown ) == TRUE )
				{
					BTogleMode.SetAction(FALSE);
					StartTickCount = GetTickCount();
					m_siPollStatus = ON_PANNEL_TOGLEMODE_CHANGE;
					if(!bTogleMode)
					{
						BFollower.SetAction(FALSE);
						BMiniMap.SetAction(FALSE);
						BInventoy.SetAction(FALSE);
						BTradeBook.SetAction(FALSE);
					}
					else
					{
						BTradeOnOff.SetAction(FALSE);
						BAttack.SetAction(FALSE);
						BImoticon.SetAction(FALSE);
						BParty.SetAction(FALSE);
						BSetting.SetAction(FALSE);
					}
					bTogling = TRUE;
				}*/

//				if(!bTogleMode)
//				{
					if( m_Btn_Friends.Process( m_bMouseDown ) == TRUE )
					{
						if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			//친구 목록
						{
							if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_FRIEND )
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
							else
								m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_FRIEND );
						}					
						else										
							m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_FRIEND );
					}
					else if( m_Btn_Account.Process( m_bMouseDown ) == TRUE )			//투자금
					{
						if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			
						{
							if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_TOTAL_MONEY )
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
							else
							{
								m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_TOTAL_MONEY );
							}
						}					
						else										
							m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_TOTAL_MONEY );
					}	
					else if( m_Btn_Inventory.Process( m_bMouseDown ) == TRUE && !m_pMyOnlineWorld->pOnlineTrade->IsActive())			// Inventory
					{
						if(m_pMyOnlineWorld->pOnlineInventory->IsActive())			m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
						else														m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);		
					}
					else if( m_Btn_CharStatus.Process( m_bMouseDown ) == TRUE )
					{
						if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			//스탯
						{
							if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_STATUS )
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
							else
								m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_STATUS );
						}					
						else										
							m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_STATUS );
					}
//				}
//				else
//				{
					if( m_Btn_TradePermission.Process( m_bMouseDown ) == TRUE )					// 거래 허용
					{
						if(m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade)			
						{
							TradeChange(TRUE);
						}
						else
						{
							TradeChange(FALSE);
						}
					}
					else if( m_Btn_AggressiveMode.Process( m_bMouseDown ) == TRUE )
					{
						if( m_pMyOnlineWorld->IsPeace() )
						{
							ModeChange(TRUE);
						}
						else
						{
							ModeChange(FALSE);
						}
					}
					else if( m_Btn_ShowImoticon.Process( m_bMouseDown ) == TRUE )				// Imoticon
					{
						if(m_pMyOnlineWorld->pOnlineImoticon->IsActive())				m_pMyOnlineWorld->pOnlineImoticon->SetActive(FALSE);
						else														m_pMyOnlineWorld->pOnlineImoticon->SetActive(TRUE);
					}
					else if( m_Btn_ShowWhisperList.Process(m_bMouseDown) == TRUE)
					{
						SetWhisperList(!m_bWhisperMode);
						if(m_WhisperList.size() > m_siWhisperSel)
							m_WhisperList[m_siWhisperSel].bSelect = TRUE;
					}
#ifdef _USE_GSUI
					else if( m_Btn_ShowWhisperList.Process(m_bMouseDown) == TRUE)
					{
						GetUIGSMgr()->GetMultiListBox(UIWND_ID_MULTILISTBOX_CHATABILITY)->ShowWindow(true);
					}
#endif
					// 지누기 수정 ///////////////////////////////////////////////////////////////////////////////////////////////
					else if(m_Btn_MacroPlayPause.Process(m_bMouseDown))					m_pMyOnlineWorld->m_pChatMacro->OnRunningToggle();
					else if(m_Btn_MacroSetting.Process(m_bMouseDown))					m_pMyOnlineWorld->m_pChatMacro->OnButtonPush();
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////

					else if( m_Btn_Quest.Process( m_bMouseDown ) == TRUE )					// 팁
					{
						m_pMyOnlineWorld->m_pQuestInterface->OnButtonPush();
					}
					else if( m_Btn_Mercenary.Process( m_bMouseDown ) == TRUE )				//
					{
						m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
					}
					else if( m_Btn_Option.Process( m_bMouseDown ) == TRUE )				// 환경설정
					{
						// 게임에서 나간다.
						if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn() && m_pMyOnlineWorld->pOnlineBooth->GetStatusOut() != ON_BENCH_OUT_INVENTORY_POLL
							&& !m_pMyOnlineWorld->pOnlineTrade->IsActive())
						{
							if(!m_pMyOnlineWorld->pOnlineSetting->IsAction())
								m_pMyOnlineWorld->pOnlineSetting->SetAction(TRUE);
							else
								m_pMyOnlineWorld->pOnlineSetting->SetAction(FALSE);
						}
					}
					/*
					// robypark 2004/11/13 14:37
					// 공성전 유닛 상태 보기 버튼 처리
					else if ((m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT) &&
						(m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.Process( m_bMouseDown ) == TRUE)
						&& (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
						&& (FALSE == m_bIsInVillage))
					{
						SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
						// 현재 공성 유닛 상태 정보 보이기 라면
						if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
						{
							// 현재 공성 유닛 상태 정보 안보이기로 설정
							scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
						}
						else
						{
							// 현재 공성 유닛 상태 정보 보이기로 설정
							scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
						}

						m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
					}
					// robypark 2005/1/5 17:34
					// 공성전 유닛 => 일반 유닛 변신 요청 버튼 처리
					else if ((m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT) &&
						(m_sShowInformationAboutSiegeWarfareUnit.m_Btn_RequestReturnNormalCharacter.Process( m_bMouseDown ) == TRUE)
						&& (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
						&& (FALSE == m_bIsInVillage))
					{
						// 변신 요청 메시지 보내기
						m_pMyOnlineWorld->pOnlineClient->SendRequestNormalCharacter(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID());
					}
					*/

					// 공성 유닛 변신 윈도 처리
					if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
					{
						SetShowTransformSiegeUnitBtn(FALSE);

						// 용병창이 열려있다면 닫아준다.
						if (m_pMyOnlineWorld->m_pMercenary->IsAction())
						{
							m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
						}

						if (FALSE == m_pSiegeWarfareTransformUIWnd->Action())
						{
							m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);

							// 사용자 입력 다시 활성화
							m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
						}
					}

					// 공성 유닛 인터페이스 처리
					if ((m_pSiegeWarfareUnitUI->IsVisibleWindow())
						|| (m_pSiegeWarfareUnitUI->IsMinimizeWindow()))
					{
						// 용병창이 열려있다면 닫아준다.
						if (m_pMyOnlineWorld->m_pMercenary->IsAction())
						{
							m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
						}

						if (FALSE == m_pSiegeWarfareUnitUI->Action())
						{
							m_pSiegeWarfareUnitUI->ShowWindow(SW_HIDE);
						}
					}

					// 공성전 유닛 유닛 변신 버튼 처리
					if ((m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.Process( m_bMouseDown ) == TRUE)
						&& (FALSE == m_bIsInVillage))
					{
						// 변신 윈도 활성화
						if (FALSE == m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
						{
							if ((m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive())				// 거래 요청 중이거나
									|| (m_pMyOnlineWorld->pOnlineTrade->IsActive())					// 거래 중이거나
									|| (m_pMyOnlineWorld->pMyData->IsWaitBattleResponse())			// 전투 대기중일 때 거래막음
									|| (m_pMyOnlineWorld->pMyData->IsGotoVillage()) 				// 마을에 들어가는 중이거나
									|| (m_pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse())		// 마을 들어가기 위한 대기 중이라면
									|| (m_pMyOnlineWorld->pIOnlineChar->IsMoving(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))	// 이동 중이라면
									|| (m_pMyOnlineWorld->pOnlineMsgBoxUsePortal->IsActive())		// 포탈을 이용하려는 경우
									|| (m_pMyOnlineWorld->pOnlineQuestPlay->IsAction()))			// NPC에게 퀘스트를 받는 중이거나
							{
								// 변신할 수 없다
							}
							else
							{
								// 변신 윈도 활성화 성공 여부
								if (m_pSiegeWarfareTransformUIWnd->ShowWindow(TRUE))
								{
									SetShowTransformSiegeUnitBtn(FALSE);

									// 열려진 창들 닫기
									// 인벤토리가 열려 있으면 닫아준다.
									if(m_pMyOnlineWorld->pOnlineInventory->IsActive())
										m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

									// 장부가 열려 있으면 닫아준다.
									if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
										m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

									// 미니맵도 닫는다!!
									if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive())
										m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);

									// 시스템 메뉴 닫기
									if(m_pMyOnlineWorld->pOnlineSetting->IsAction())
										m_pMyOnlineWorld->pOnlineSetting->SetAction(FALSE);

									// 용병창이 열려있다면 닫아준다.
									if (m_pMyOnlineWorld->m_pMercenary->IsAction())
									{
										m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
									}

									// 사용자 입력 막기
									m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);
								}
							}
						}
					}
//				}
			}	// if(m_bIsInVillage == FALSE )
			// 마을 안일 경우
			else
			{
				// 마을 안일 경우
				if (m_pMyOnlineWorld->pOnlineVillage)
				{
					SetShowTransformSiegeUnitBtn(FALSE);

					// 공성 유닛 변신 윈도 처리
					if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
					{
						m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);
					}
				}

				// 공성 유닛 인터페이스 처리
				if ((m_pSiegeWarfareUnitUI->IsVisibleWindow())
					|| (m_pSiegeWarfareUnitUI->IsMinimizeWindow()))
				{
					if (FALSE == m_pSiegeWarfareUnitUI->Action())
					{
						m_pSiegeWarfareUnitUI->ShowWindow(SW_HIDE);
					}
				}

				if( m_pChatControl )
					m_pChatControl->Poll();

				// 채팅창 사이즈 바꾸는 버튼 누를때
				if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHAT) == 0)
				{
					if( m_Btn_ChangeChatWindow.Process(m_bMouseDown) == TRUE )
					{
						SetChatOpen();
					}
				}

				// 채팅창 클릭 시 포커스
				if( pOecEx->IsPointInRect( pt ) && m_bMouseDown )
					if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
						Handler_OnlineEditControlEx::SetFocus( m_hOecChat);

				// 메세지 도착알림 클릭
				if(PtInRect(&m_rt_Messenger, pt) && m_pMyOnlineWorld->fVillageLeftOneSwitch == 1)
				{
					if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
						m_pMyOnlineWorld->pOnlineMessenger->SetMini(FALSE);
					else
						m_pMyOnlineWorld->pOnlineMessenger->SetMini(TRUE);
				}
/*
				// robypark 2004/12/10 20:25
				// 공성전 유닛 상태 보기 버튼 처리
				if ((FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()) &&
						(m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.Process( m_bMouseDown ) == TRUE))
				{
					SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
					// 현재 공성 유닛 상태 정보 보이기 라면
					if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
					{
						// 현재 공성 유닛 상태 정보 안보이기로 설정
						scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}
					else
					{
						// 현재 공성 유닛 상태 정보 보이기로 설정
						scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}

					m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
				}
*/
				// SMS 출력 및 사이즈 바꾸는 버튼 누를때
				if(m_pSMS)			
				{
					m_pSMS->Action();
					if( m_pSMS->IsModeJustChanged() )	// 방금전에 SMS의 형태가 바뀌었다면
					{
						ChangeChatBasePos( m_pSMS->GetCurrentYSize() );													// 채팅창 위치 바꾸고
					}
				}
/*				// 메세지 도착알림 클릭
				BOOL	bMessengerShortKey = FALSE;
				if(m_pMyOnlineWorld->pOnlineVillage)
				{
					if(m_pMyOnlineWorld->pOnlineVillage->m_siPollStatus != ON_VILLAGE_STATUS_IN_STRUCTURE)
						bMessengerShortKey = TRUE;
				}
				else
					bMessengerShortKey = TRUE;

				if(PtInRect(&m_rt_Messenger, pt) && m_pMyOnlineWorld->fVillageLeftOneSwitch == 1 && bMessengerShortKey)
				{
					if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
						m_pMyOnlineWorld->pOnlineMessenger->SetMini(FALSE);
					else
						m_pMyOnlineWorld->pOnlineMessenger->SetMini(TRUE);
				}*/

//				if(!bTogleMode)
//				{
//					if()
//					{
//					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_FRIEND/*ONLINEPANNEL_DISABLEFLAG_CHARINFO*/) == 0)
					{
						if( m_Btn_Friends.Process( m_bMouseDown ) == TRUE )				// 친구목록 정보
						{
							if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			
							{
								if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_FRIEND )
									m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
								else
									m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_FRIEND );
							}					
							else										
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_FRIEND );
						}
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_MINIMAP) == 0)
					{
						if( m_Btn_Account.Process( m_bMouseDown ) == TRUE )				// 미니맵
						{
							if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			
							{
								if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_TOTAL_MONEY )
									m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
								else
									m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_TOTAL_MONEY );
							}					
							else										
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_TOTAL_MONEY );
						}	
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_INVENTORY) == 0)
					{
						if( m_Btn_Inventory.Process( m_bMouseDown ) == TRUE )				// Inventory
						{
							if(m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
							else													m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);		
						}
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_TRADEBOOK) == 0)
					{
						if( m_Btn_CharStatus.Process( m_bMouseDown ) == TRUE )
						{
							if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			
							{
								if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_STATUS )
									m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
								else
									m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_STATUS );
							}					
							else										
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_STATUS );
						}
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHARINFO) == 0)
					{
						if( m_Btn_Mercenary.Process( m_bMouseDown ) == TRUE )
							m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
					}

//				}
//				else
//				{
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_PERMITTRADE) == 0)		
					{
						if( m_Btn_TradePermission.Process( m_bMouseDown ) == TRUE )
						{
							if(m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade)			
							{
								m_Btn_TradePermission.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BLOCK_TRADE));
								m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade = FALSE;
							}
							else
							{
								m_Btn_TradePermission.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PERMIT_TRADE));
								m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade = TRUE;
							}
						}
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_ATTACK) == 0)
					{
						if( m_Btn_AggressiveMode.Process( m_bMouseDown ) == TRUE )
						{
							if( m_pMyOnlineWorld->IsPeace() )
							{
								m_Btn_AggressiveMode.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BATTLEMODE));
								m_pMyOnlineWorld->SetPeace( FALSE );
							}
							else
							{
								m_Btn_AggressiveMode.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PEACEMODE));
								m_pMyOnlineWorld->SetPeace( TRUE );
							}
						}
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_IMOTICON) == 0)
					{
						if( m_Btn_ShowImoticon.Process( m_bMouseDown ) == TRUE )				// Imoticon
						{
							if(m_pMyOnlineWorld->pOnlineImoticon->IsActive())			m_pMyOnlineWorld->pOnlineImoticon->SetActive(FALSE);
							else													m_pMyOnlineWorld->pOnlineImoticon->SetActive(TRUE);		
						}
					}
					//ONLINEPANNEL_DISABLEFLAG_TIP
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_TIP) == 0)
					{
						if( m_Btn_Quest.Process( m_bMouseDown ) == TRUE )					
						{
							if(m_pMyOnlineWorld->pOnlineTip->IsAction())					m_pMyOnlineWorld->pOnlineTip->SetAction(FALSE);
							else														m_pMyOnlineWorld->pOnlineTip->SetAction(TRUE);
							/*
							if(m_pMyOnlineWorld->pOnlineParty->GetPartyCharBaseInfo() == TRUE)
							{
								m_pMyOnlineWorld->pOnlineParty->SetPartyCharInfo(FALSE);
							}
							else if(m_pMyOnlineWorld->pOnlineParty->GetPartyCharBaseInfo() == FALSE)
							{
								m_pMyOnlineWorld->pOnlineParty->SetPartyCharInfo(TRUE);
							}
							*/
						}	
					}
					if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_OPTION) == 0)
					{
						if( m_Btn_Option.Process( m_bMouseDown ) == TRUE )
						{
							// 게임에서 나간다.
							m_pMyOnlineWorld->ExitOnline();
						}
					}
//				}

			}	// else <== if(m_bIsInVillage == FALSE )
			
			// 마우스 접근시 인터페이스 설명
			if( m_pMyOnlineWorld->pOnlineHelp->IsActive() == FALSE )
			{
				if( m_Btn_Friends.IsInMouse() == TRUE )						sprintf( szTemp, "%s(E)", m_Btn_Friends.GetMenuName() );
				else if( m_Btn_Account.IsInMouse() == TRUE )					sprintf( szTemp, "%s(W)", m_Btn_Account.GetMenuName() );
				else if( m_Btn_Inventory.IsInMouse() == TRUE )				sprintf( szTemp, "%s(I)", m_Btn_Inventory.GetMenuName() );
				else if( m_Btn_CharStatus.IsInMouse() == TRUE )				sprintf( szTemp, "%s(S)", m_Btn_CharStatus.GetMenuName() );
				if( m_Btn_TradePermission.IsInMouse() == TRUE )
				{
					if(m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade)	sprintf( szTemp, "%s(X)", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PERMIT_TRADE));
					else												sprintf( szTemp, "%s(X)", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BLOCK_TRADE));
				}
				else if( m_Btn_AggressiveMode.IsInMouse() == TRUE )		
				{
					if( m_pMyOnlineWorld->IsPeace() )						sprintf( szTemp, "%s(A)", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PEACEMODE) );
					else												sprintf( szTemp, "%s(A)", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BATTLEMODE));
				}
				else if( m_Btn_ShowImoticon.IsInMouse() == TRUE )				sprintf( szTemp, "%s", m_Btn_ShowImoticon.GetMenuName() );
				else if( m_Btn_Quest.IsInMouse() == TRUE )					sprintf( szTemp, "%s(Q)", m_Btn_Quest.GetMenuName() );
				else if( m_Btn_Mercenary.IsInMouse() == TRUE )					sprintf( szTemp, "%s(F)", m_Btn_Mercenary.GetMenuName() );
				else if( m_Btn_Option.IsInMouse() == TRUE )					sprintf( szTemp, "%s(ESC)", m_Btn_Option.GetMenuName() );
				else if( m_Btn_ShowWhisperList.IsInMouse() == TRUE)				sprintf( szTemp, "%s", m_Btn_ShowWhisperList.GetMenuName() ); 
				else if( m_Btn_MacroPlayPause.IsInMouse() == TRUE)				sprintf( szTemp, "%s", m_Btn_MacroPlayPause.GetMenuName() ); 
				else if( m_Btn_MacroSetting.IsInMouse() == TRUE)				sprintf( szTemp, "%s", m_Btn_MacroSetting.GetMenuName() ); 

				if( m_Btn_ChangeChatWindow.IsInMouse() == TRUE )							sprintf( szTemp, "%s", m_Btn_ChangeChatWindow.GetMenuName() );
				else if( PtInRect( &m_rt_Gauge_Hungry, pt ) )						sprintf( szTemp, "%s : %d/%d", m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_HEALTH ), m_pMyOnlineWorld->pMyData->GetHealth(), ON_MAX_HEALTH);
				else if(PtInRect(&m_rt_MyMoney, pt))							sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MYMONEY) );
				else if(PtInRect(&m_rt_MyGrade, pt))							sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CREDITGRADE));
				else if(PtInRect(&m_rt_MyAttackPower, pt))							sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_ATTACK) );
				else if(PtInRect(&m_rt_Date, pt))							sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_BOARD_DATE) );
				else if(PtInRect(&m_rt_Messenger, pt))
				{
					if(m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						if(m_pMyOnlineWorld->pOnlineMessenger->IsNewMessage())sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEW_MESSGAE_ARRIVED));
						else												sprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MESSENGER_DESC));
					}
				}

				if( szTemp[0] != 0 )
					m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );

				/*
				// robypark 2004/11/13 14:37
				// 공성전 유닛 상태 보기 버튼 풍선도움말 처리
				SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
				if ((TRUE == m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.IsInMouse())
					&& (FALSE == m_bIsInVillage))
				{
					// 공성 유닛이거나 수성 인터페이스 상이라면
					if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT) || (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()))
					{
						sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOL_TIP_SHOW_SIEGEWARFAREUNIT_INFO));
						m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
					}
				}

				// robypark 2005/1/5 17:36
				// 공성전 유닛 => 일반 유닛 변신 요청 버튼 풍선도움말 처리
				if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					&& (TRUE == m_sShowInformationAboutSiegeWarfareUnit.m_Btn_RequestReturnNormalCharacter.IsInMouse())
					&& (FALSE == m_bIsInVillage))
				{
					sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_CANCEL_TRANSFORM));
					m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
				}
				*/
				// 공성전 유닛 변신 요청 풍선도움말 처리
				if ((TRUE == m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.IsInMouse())
					&& (FALSE == m_bIsInVillage))
				{
					sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(8100111));
					m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
				}
			}	// if( m_pMyOnlineWorld->pOnlineHelp->IsActive() == FALSE )
		}
		break;
	case ON_PANNEL_TOGLEMODE_CHANGE:
		{
			SI16	StartPos = 0;
			m_siPollStatus	= ON_PANNEL_POLL;

			m_Btn_Friends.SetAction(TRUE);
			m_Btn_Account.SetAction(TRUE);
			m_Btn_Inventory.SetAction(TRUE);
			m_Btn_CharStatus.SetAction(TRUE);

			m_Btn_TradePermission.SetAction(TRUE);
			m_Btn_AggressiveMode.SetAction(TRUE);
			m_Btn_ShowImoticon.SetAction(TRUE);
			m_Btn_Quest.SetAction(TRUE);
			m_Btn_Option.SetAction(TRUE);
			m_Btn_Mercenary.SetAction(TRUE);
			m_Btn_ShowWhisperList.SetAction(TRUE);
			m_Btn_MacroPlayPause.SetAction(TRUE);
			m_Btn_MacroSetting.SetAction(TRUE);
		}
		break;
	}
	
	//============================================================================================
	// Banner
//	m_pMyOnlineWorld->pOnlineBanner->Poll();

	if( IpD.LeftPressSwitch )					m_bMouseDown	=	TRUE;
	else										m_bMouseDown	=	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	드로우 함수.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
#ifdef _USE_GSUI
//--> Test Code (sagolboss)
	GetSurfaceBufferMgr()->ClearSurface();
	GetUIGSMgr()->Draw();
	GetSurfaceBufferMgr()->FlipSurface();
//<--
#endif

	HDC		hdc;	
	SI32	siClipLeft, siClipTop, siClipRight, siClipBottom;
	CHAR	szNumericString[128];			
	SI32	i, MaxWeight;
//	SI16	siCurHealth, siHPBarWidth, siFont;
	vector<WhisperData>::iterator it;
	OnlineEditControlEx	*pOecEx;

	POINT	pt;
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	MaxWeight  = 0;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(m_pMyOnlineWorld->pMyData->IsValidFollower(i))
		{
			MaxWeight += m_pMyOnlineWorld->pMyData->GetFollowerParameterBattlePower(i);
		}
	}

	// 공지사항 List
	if(!Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
	{
		m_pMyOnlineWorld->pOnlineNoticeList->Poll(pSurface);
	}

	//============================================================================================
	// 채팅창
	if( m_pChatControl )	m_pChatControl->Draw(pSurface);
	if( m_pSMS )			m_pSMS->Draw(pSurface);

	clGrp.GetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
	clGrp.SetClipFullScreen();

	if(clGrp.LockSurface(pSurface) == TRUE)
	{	
		// Main 하단인터페이스
		clGrp.PutSpriteT(m_pt_Pannel.x,			m_pt_Pannel.y,			m_SprImg_Pannel.Header.Xsize,			m_SprImg_Pannel.Header.Ysize,			m_SprImg_Pannel.Image);
		clGrp.PutSpriteT(m_pt_PannelChatLine.x,	m_pt_PannelChatLine.y,	m_SprImg_PannelChatLine.Header.Xsize,	m_SprImg_PannelChatLine.Header.Ysize,	m_SprImg_PannelChatLine.Image);

		// 1:1대화창 메시지 도착 알림(새로온 메시지가 있고 마우스가 위에 없으면 깜빡거림)
		if(m_pMyOnlineWorld->pOnlineMessenger->IsAction())
		{
			if(m_pMyOnlineWorld->pOnlineMessenger->IsNewMessage() && !PtInRect(&m_rt_Messenger, pt))
			{
				DWORD	dwTemp = timeGetTime();
				
				if( ( dwTemp - m_dwDistDelay ) > 500 )
				{
					m_bMessageShow = !m_bMessageShow;
					m_dwDistDelay	=	dwTemp;
				}
				if(m_bMessageShow)
					clGrp.PutSpriteT(170, m_pt_Pannel.y + 30, m_SprIcon_Messenger.Header.Xsize, m_SprIcon_Messenger.Header.Ysize, m_SprIcon_Messenger.Image);
			}
			else
				clGrp.PutSpriteT(172, m_pt_Pannel.y + 30, m_SprIcon_Messenger.Header.Xsize, m_SprIcon_Messenger.Header.Ysize, m_SprIcon_Messenger.Image);
		}

		if( m_pMyOnlineWorld->pOnlineTradeBook->IsAction() && m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_FRIEND)		
			m_Btn_Friends.Put( &m_SprBtn_Friends, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														
			m_Btn_Friends.Put( &m_SprBtn_Friends, 0, 2, 1, BUTTON_PUT_NOMOVE );

		if( m_pMyOnlineWorld->pOnlineTradeBook->IsAction() == TRUE && m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_TOTAL_MONEY)		
			m_Btn_Account.Put( &m_SprBtn_Account, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														
			m_Btn_Account.Put( &m_SprBtn_Account, 0, 2, 1, BUTTON_PUT_NOMOVE );		

		if( m_pMyOnlineWorld->pOnlineTradeBook->IsAction() == TRUE && m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_STATUS)	
			m_Btn_CharStatus.Put( &m_SprBtn_CharStatus, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														
			m_Btn_CharStatus.Put( &m_SprBtn_CharStatus, 0, 2, 1, BUTTON_PUT_NOMOVE );

		if(m_pMyOnlineWorld->pOnlineInventory->IsActive() == TRUE)	m_Btn_Inventory.Put( &m_SprBtn_Inventory, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														m_Btn_Inventory.Put( &m_SprBtn_Inventory, 0, 2, 1, BUTTON_PUT_NOMOVE );
		if( m_pMyOnlineWorld->pOnlineImoticon->IsActive() == TRUE )	m_Btn_ShowImoticon.Put( &m_SprBtn_ShowImoticon, 1, 0, 1, BUTTON_PUT_NOMOVE );
		else														m_Btn_ShowImoticon.Put( &m_SprBtn_ShowImoticon, 0, 0, 1, BUTTON_PUT_NOMOVE );
		if(m_pMyOnlineWorld->pOnlineTip->IsAction() == TRUE)			m_Btn_Quest.Put( &m_SprBtn_Quest, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														m_Btn_Quest.Put( &m_SprBtn_Quest, 0, 2, 1, BUTTON_PUT_NOMOVE );
		if(m_pMyOnlineWorld->pOnlineSetting->IsAction() == TRUE )		m_Btn_Option.Put( &m_SprBtn_Option, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														m_Btn_Option.Put( &m_SprBtn_Option, 0, 2, 1, BUTTON_PUT_NOMOVE );
		if(m_pMyOnlineWorld->m_pMercenary->IsAction() == TRUE )		m_Btn_Mercenary.Put( &m_SprBtn_Mercenary, 1, 2, 1, BUTTON_PUT_NOMOVE );
		else														m_Btn_Mercenary.Put( &m_SprBtn_Mercenary, 0, 2, 1, BUTTON_PUT_NOMOVE );
		
		m_Btn_ShowWhisperList.Put( &m_SprBtn_ShowWhisperList, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_Btn_ChangeChatWindow.Put( &m_SprBtn_ChangeChatWindow, 0, 2, 1, BUTTON_PUT_NOMOVE );

		// 지누기 수정 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(m_pMyOnlineWorld->m_pChatMacro->IsRunning())		m_Btn_MacroPlayPause.Put(&m_SprBtn_MacroPlayPause,2,2,3,BUTTON_PUT_NOMOVE);
		else											m_Btn_MacroPlayPause.Put(&m_SprBtn_MacroPlayPause,0,0,1,BUTTON_PUT_NOMOVE);
		
		m_Btn_MacroSetting.Put(&m_SprBtn_MacroSetting,0,0,1,BUTTON_PUT_NOMOVE);		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		SI16		siHPBarWidth;

		// 배고픔 수치를구한다.
		siHPBarWidth = ((m_pMyOnlineWorld->pMyData->GetHealth() * m_SprGauge_Hungry.Header.Xsize) / ON_MAX_HEALTH);

		RECT rtClip;
		clGrp.GetClipRect(&rtClip);

		clGrp.SetClipArea(80, 523, 80+siHPBarWidth,  523+m_SprGauge_Hungry.Header.Ysize);
		clGrp.PutSpriteCT(80, 523 , m_SprGauge_Hungry.Header.Xsize, m_SprGauge_Hungry.Header.Ysize, m_SprGauge_Hungry.Image);
		clGrp.SetClipArea(rtClip.left, rtClip.top, rtClip.right, rtClip.bottom);


		// 귓말상대 목록
		if(m_bWhisperMode)
		{
			for(i = 0; i < WHISPER_LIST_COUNT; i++)
			{
				if(i == m_siWhisperSel)
					clGrp.PutSpriteT(206, 562 - i*16, m_SprBtn_WhisperDestID.Header.Xsize, m_SprBtn_WhisperDestID.Header.Ysize, 
						&m_SprBtn_WhisperDestID.Image[m_SprBtn_WhisperDestID.Header.Start[1]]);
				else
					clGrp.PutSpriteT(206, 562 - i*16, m_SprBtn_WhisperDestID.Header.Xsize, m_SprBtn_WhisperDestID.Header.Ysize, 
						&m_SprBtn_WhisperDestID.Image[m_SprBtn_WhisperDestID.Header.Start[0]]);
			}
		}

		/*
		// robypark 2004/11/13 13:58
		// 공성전 유닛 상태 정보 보기 버튼 그리기
		// 사용자가 공성전 유닛이고
		SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
		if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT) && (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
			&& (FALSE == m_bIsInVillage))
		{
			// 공성전 유닛 상태 보이기
			// 버튼 테두리 그리기
			clGrp.PutSpriteT(710, 447, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize,
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Image);

			// 공성전 유닛에서 => 일반 유닛으로 돌아가기
			// 버튼 테두리 그리기
			clGrp.PutSpriteT(710, 477, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize,
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Image);

			// 마우스 커서가 버튼위에 있다면,
			if (m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.IsInMouse())
			{
				m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.PutSpriteButton(
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetX(), 
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetY(), 
																&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton, 
																1, FALSE, FALSE);
			}
			// 현재 설정이 상태 보이기 라면
			else if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
			{
				m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.PutSpriteButton(
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetX(), 
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetY(), 
																&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton, 
																2, FALSE, FALSE);
			}
			else
			{
				m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.PutSpriteButton(
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetX(), 
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetY(), 
																&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton, 
																0, FALSE, FALSE);
			}

			// 공성전 유닛 => 일반 유닛 돌아가기 버튼 그리기
			m_sShowInformationAboutSiegeWarfareUnit.
					m_Btn_RequestReturnNormalCharacter.Put(
							&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton,
							0, 2, 1, BUTTON_PUT_NOMOVE );
		}
		*/

		if (m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.IsVisibleWindow())
		{
			// 공성전 유닛유닛으로 변신
			// 버튼 테두리 그리기
			clGrp.PutSpriteT(710, 477, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize,
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Image);

			// 버튼
			m_sShowInformationAboutSiegeWarfareUnit.
				m_btnRequestTrasnformUI.Put(
							&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton,
							0, 2, 1, BUTTON_PUT_NOMOVE );
		}

		clGrp.UnlockSurface(pSurface);
	}

	if( pSurface->GetDC(&hdc) == DD_OK)
	{
		SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT) );				
		SetBkMode(hdc, TRANSPARENT);

		// 시간
		SOnlineTime		Time;
		Time = m_pMyOnlineWorld->pTimer->GetNowTime();
		switch(pGame->LanguageCode)
		{
			case ELANGAGECODE_KOREAN:	case ELANGAGECODE_TAIWAN:	case ELANGAGECODE_JAPAN:	case ELANGAGECODE_HONGKONG:	case ELANGAGECODE_CHINA:
				wsprintf(szNumericString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TIMETEXT), Time.Year, Time.Month, Time.Day, Time.Hour);
				break;
			case ELANGAGECODE_INDONESIA:
				wsprintf(szNumericString, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TIMETEXT), Time.Day, Time.Month, Time.Year, Time.Hour);
				break;
		}

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 637,581,134,14, szNumericString, RGB(250, 250, 250));			

		// 상단표시
		SI32 siGuildInGrade = m_pMyOnlineWorld->pMyData->GetGuildClass();
		if(m_pMyOnlineWorld->pMyData->GetGuildName())
		{
			sprintf((char*)szNumericString, "[%s][%s]", m_pMyOnlineWorld->pMyData->GetGuildName(), m_pMyOnlineWorld->pOnlineText->Get(m_pMyOnlineWorld->m_cltIGuildClass.GetClassName(siGuildInGrade)));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,54, 539,132, 14, szNumericString, RGB(250, 250, 250));
		}

		//레벨 표시
		sprintf(szNumericString,"%d",m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,12,535,26,13,szNumericString,RGB(250, 250, 250));

		// 공격력5
		sprintf(szNumericString, "%d", MaxWeight);		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 39, 559,48,14, szNumericString, RGB(250, 250, 250));

		// 신용등급
		sprintf(szNumericString, "%d", m_pMyOnlineWorld->pMyData->GetTradeGrade());
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 127, 559,59,14, szNumericString, RGB(250, 250, 250));

		// 돈
		MakeMoneyText(m_pMyOnlineWorld->pMyData->GetMoney(), (char*)szNumericString);
	  	m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,39, 580,147,14, szNumericString, RGB(250, 250, 250));


		
		// 채팅글 찍어주기
		if(Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
		{
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
			pOecEx->Draw( hdc );
		}
		else
		{
			// 공지사항 List 출력
			SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
			m_pMyOnlineWorld->pOnlineNoticeList->Draw(hdc);
		}

		// 귓말상대 목록
		if(m_bWhisperMode)
		{
			for(it = m_WhisperList.begin(); it != m_WhisperList.end(); it++)
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, it->siX, it->siY, 89, 16, it->szName, RGB(255,255,255));
			}
		}

		m_Btn_TradePermission.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_Btn_AggressiveMode.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
//		}

		// actdoll (2004/03/31 13:26) : = 이벤트 = 독도퀘스트 총괄 숫자 내역 적음
		// 독도 이벤트는 한국에서만 --v
//		if(pGame->LanguageCode == ELANGAGECODE_KOREAN)
//		{
//			char	pszString1[128], pszString2[128];
//			strcpy	( pszString1, "[독도지키기 이벤트 진행 중]");
//			sprintf	( pszString2, "현재 %d명이 독도에 태극기를 꽂았습니다.", m_pMyOnlineWorld->GetDokdoQuestCompleteUserCount() );
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 101, 6, pszString1, RGB( 0, 0, 0 ) );	
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 100, 5, pszString1, RGB( 255, 128, 0 ) );	
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 101, 21, pszString2, RGB( 0, 0, 0 ) );	
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 100, 20, pszString2, RGB( 255, 255, 128 ) );	
//		}

		/*
		// robypark 2004/11/13 13:58
		// 공성전 유닛 상태 정보 보기 버튼 텍스트 그리기
		// 사용자가 공성전 유닛이고
		SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
		if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT) && (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
			&& (FALSE == m_bIsInVillage))
		{
			// 모든 공성전 유닛 상태 보기 버튼
			m_sShowInformationAboutSiegeWarfareUnit.
				m_Btn_ShowInformationAboutSiegeWarfareUnit.
					Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));																

			// 공성전 유닛 => 일반 유닛 변신 요청 버튼
			m_sShowInformationAboutSiegeWarfareUnit.
				m_Btn_RequestReturnNormalCharacter.
					Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));																
		}
		*/
		if (m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.IsVisibleWindow())
		{
			// 공성전 유닛 변신 요청 버튼
			m_sShowInformationAboutSiegeWarfareUnit.
				m_btnRequestTrasnformUI.
					Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		
		pSurface->ReleaseDC(hdc);
	}

	// robypark 2005/1/27 22:6
	// 공성전 유닛일 경우 인터페이스 그리기
	m_pSiegeWarfareUnitUI->Draw(pSurface);

	// 공성전 유닛으로 변신 윈도 그리기
	m_pSiegeWarfareTransformUIWnd->Draw(pSurface);


	// 숫자들을 찍어준다.
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		SEffectData			TempEffect;
		
		// 돈 변한거
		if(TempYetMoney != 0xffffffffffffffff)
		{
			if(TempYetMoney != m_pMyOnlineWorld->pMyData->GetMoney())
			{
				// 변화한 수 출력
				TempEffect.m_Type                                = EFFECTTYPE_CHANGENUMBER;
				TempEffect.m_EffectData_ChangeNum.m_Frame        = 0;
				TempEffect.m_EffectData_ChangeNum.m_XPos         = 93;
				TempEffect.m_EffectData_ChangeNum.m_YPos         = 579;
				TempEffect.m_EffectData_ChangeNum.m_ChangeNumber = m_pMyOnlineWorld->pMyData->GetMoney() - TempYetMoney;
				m_pMyOnlineWorld->pOnlineFieldEffect->AddEffect(TempEffect);
			}
		}
		TempYetMoney = m_pMyOnlineWorld->pMyData->GetMoney();
	
		// 신용도 변한거
		if(TempYetCredit != 0xffffffff)
		{
			if((UI32)TempYetCredit != m_pMyOnlineWorld->pMyData->GetTradeCredit())
			{
				// 변화한 수 출력
				TempEffect.m_Type                                = EFFECTTYPE_CHANGENUMBER;
				TempEffect.m_EffectData_ChangeNum.m_Frame        = 0;
				TempEffect.m_EffectData_ChangeNum.m_XPos         = 145;
				TempEffect.m_EffectData_ChangeNum.m_YPos         = 560;
				TempEffect.m_EffectData_ChangeNum.m_ChangeNumber = (__int64)(m_pMyOnlineWorld->pMyData->GetTradeCredit() - TempYetCredit);
				m_pMyOnlineWorld->pOnlineFieldEffect->AddEffect(TempEffect);
			}
		}
		TempYetCredit = m_pMyOnlineWorld->pMyData->GetTradeCredit();

		// 공격력 변한거
		if(TempYetBattlePower != 0xffffffff)
		{
			if(TempYetBattlePower != MaxWeight)
			{
				// 변화한 수 출력
				TempEffect.m_Type                                = EFFECTTYPE_CHANGENUMBER;
				TempEffect.m_EffectData_ChangeNum.m_Frame        = 0;
				TempEffect.m_EffectData_ChangeNum.m_XPos         = 55;
				TempEffect.m_EffectData_ChangeNum.m_YPos         = 560;
				TempEffect.m_EffectData_ChangeNum.m_ChangeNumber = (__int64)(MaxWeight - TempYetBattlePower);
				m_pMyOnlineWorld->pOnlineFieldEffect->AddEffect(TempEffect);
			}
		}
		TempYetBattlePower = MaxWeight;

		clGrp.UnlockSurface(pSurface);
	}	
	
	clGrp.SetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);	

	//============================================================================================
	// Banner
//	m_pMyOnlineWorld->pOnlineBanner->Draw(pSurface);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을안에 있는지 Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::SetFlagVillage(BOOL bIsInVillage, UI32 Disable)
{
	m_bIsInVillage  = bIsInVillage;
	m_siDisableFlag = Disable;

	if(m_bIsInVillage)
	{
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_TRADEBOOK)
		{
			if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive()) m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);
			m_Btn_Account.SetAction(FALSE);
		}
		else
			m_Btn_Account.SetAction(TRUE);

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_INVENTORY)
		{
			if(m_pMyOnlineWorld->pOnlineInventory->IsActive()) m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
			m_Btn_Inventory.SetAction(FALSE);
		}
		else if(m_pMyOnlineWorld->pOnlineTrade->IsActive())
			m_Btn_Inventory.SetAction(FALSE);
		else
			m_Btn_Inventory.SetAction(TRUE);

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_TRADEBOOK)
		{
			if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction()) m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
			m_Btn_CharStatus.SetAction(FALSE);
		}
		else
			m_Btn_CharStatus.SetAction(TRUE);

		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_PERMITTRADE)	m_Btn_TradePermission.SetAction(FALSE);
		else													m_Btn_TradePermission.SetAction(TRUE);

		// 공성전 유닛이라면 전투모드, 평화모드 버튼을 사용 수 없다. 또한 자동으로 전투모드가 된다.
		if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			// 평화모드라면,
			if (m_pMyOnlineWorld->IsPeace())
			{
				// 전투모드로 설정
				ModeChange(TRUE);
			}

			if (0 == (m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_ATTACK))
				m_siDisableFlag += ONLINEPANNEL_DISABLEFLAG_ATTACK;

			m_Btn_AggressiveMode.SetAction(FALSE);
		}
		else
		{
			if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_ATTACK)
				m_Btn_AggressiveMode.SetAction(FALSE);
			else	
				m_Btn_AggressiveMode.SetAction(TRUE);
		}

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_IMOTICON)		m_Btn_ShowImoticon.SetAction(FALSE);
		else													m_Btn_ShowImoticon.SetAction(TRUE);

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_TIP)		m_Btn_Quest.SetAction(FALSE);
		else													m_Btn_Quest.SetAction(TRUE);

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_OPTION)		m_Btn_Option.SetAction(FALSE);
		else													m_Btn_Option.SetAction(TRUE);

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_FRIEND)		m_Btn_Friends.SetAction(FALSE);
		else													m_Btn_Friends.SetAction(TRUE);
	
		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_STATUS)		m_Btn_CharStatus.SetAction(FALSE);
		else													m_Btn_CharStatus.SetAction(TRUE);

		// 용병창 버튼 설정
		if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHARINFO))
			m_Btn_Mercenary.SetAction(FALSE);
		else
			m_Btn_Mercenary.SetAction(TRUE);

		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////

		if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHAT)
			m_Btn_ChangeChatWindow.SetAction(FALSE);
		else
			m_Btn_ChangeChatWindow.SetAction(TRUE);
	}
	else
	{
		m_Btn_Friends.SetAction(TRUE);
		m_Btn_Account.SetAction(TRUE);
		m_Btn_Inventory.SetAction(TRUE);
		m_Btn_CharStatus.SetAction(TRUE);

		m_Btn_TradePermission.SetAction(TRUE);

		// 공성전 유닛이라면 전투모드, 평화모드 버튼을 사용 수 없다. 또한 자동으로 전투모드가 된다.
		if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			// 평화모드라면,
			if (m_pMyOnlineWorld->IsPeace())
			{
				// 전투모드로 설정
				ModeChange(TRUE);
			}

			if (0 == (m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_ATTACK))
				m_siDisableFlag += ONLINEPANNEL_DISABLEFLAG_ATTACK;

			m_Btn_AggressiveMode.SetAction(FALSE);
		}
		else
		{
			if(m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_ATTACK)
				m_Btn_AggressiveMode.SetAction(FALSE);
			else	
				m_Btn_AggressiveMode.SetAction(TRUE);
		}

		m_Btn_ShowImoticon.SetAction(TRUE);
		m_Btn_Quest.SetAction(TRUE);
		m_Btn_Option.SetAction(TRUE);

		// 용병창 버튼 설정
		if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHARINFO))
			m_Btn_Mercenary.SetAction(FALSE);
		else
			m_Btn_Mercenary.SetAction(TRUE);

		m_Btn_ChangeChatWindow.SetAction(TRUE);
		m_Btn_MacroPlayPause.SetAction(TRUE);
		m_Btn_MacroSetting.SetAction(TRUE);
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//전투 ,평화 모드 구분
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID OnlinePannel::ModeChange(BOOL bFlag)
{
	if(bFlag)
	{
		m_Btn_AggressiveMode.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BATTLEMODE));
		m_pMyOnlineWorld->SetPeace( FALSE );	
	}
	else
	{
		m_Btn_AggressiveMode.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PEACEMODE));
		m_pMyOnlineWorld->SetPeace( TRUE );
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//거래 ,차단  모드 구분
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID OnlinePannel::TradeChange(BOOL bFlag)
{
	if(bFlag)
	{
		m_Btn_TradePermission.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BLOCK_TRADE));
		m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade = FALSE;

	}
	else
	{
		m_Btn_TradePermission.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PERMIT_TRADE));
		m_pMyOnlineWorld->pOnlineTrade->m_bPermitTrade = TRUE;
	}
}

// robypark 2004/10/26 11:5 평화모드 버튼 텍스트만 설정하기
VOID OnlinePannel::SetButtonText_PeaceMode(BOOL bPeace)
{
	if(bPeace)
	{
		m_Btn_AggressiveMode.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BATTLEMODE));
	}
	else
	{
		m_Btn_AggressiveMode.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PEACEMODE));
	}
}

// robypark 2004/10/26 11:5 거래모드 버튼 텍스트만 설정하기
VOID OnlinePannel::SetButtonText_TradeMode(BOOL bTrade)
{
	if(bTrade)
	{
		m_Btn_TradePermission.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BLOCK_TRADE));

	}
	else
	{
		m_Btn_TradePermission.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PERMIT_TRADE));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을의 위치
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlinePannel::SetVillagePos(SI32 VilXPos, SI32 VilYPos)
{
	VillagePositionX = VilXPos;
	VillagePositionY = VilYPos;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	창을 열수 있는지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlinePannel::GetFlagOptionWindow(UI32 WindowType)
{
	if(m_bIsInVillage)
	{
		if(m_siDisableFlag & WindowType)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 마을인지 Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlinePannel::GetfIsVillage(void)
{
	return m_bIsInVillage;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 위치 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlinePannel::GetVillagePosition(SI32& xPos, SI32& yPos)
{
	xPos = VillagePositionX;
	yPos = VillagePositionY;
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	채팅관련함수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlinePannel::ChangeChattingMode(UI08 ChattinMode)
{
	// 개인 거래중에는 '일반 채팅'으로 밖에 되지 않는다.
	if( ( m_pMyOnlineWorld->pOnlineTrade->IsActive() == TRUE ) && ( ChattinMode != CHATTINGMODE_NORMAL ) )				return	FALSE;

	m_pChatControl->SetMode(ChattinMode);
	
	return TRUE;
}

UI08	OnlinePannel::GetChattingMode()
{
	return m_pChatControl->GetMode();
}

VOID	OnlinePannel::SetChatOpen(SI08 siHeight)
{
	if(siHeight == 1)
	{
		if(m_pChatControl->GetHeightStatus() == CHAT_HEIGHT_CLOSE)
			m_pChatControl->SetHeightStatus(CHAT_HEIGHT_MEDIUM);
		else if(m_pChatControl->GetHeightStatus() == CHAT_HEIGHT_MEDIUM)
			m_pChatControl->SetHeightStatus(CHAT_HEIGHT_HIGH);
		else if(m_pChatControl->GetHeightStatus() == CHAT_HEIGHT_HIGH)
			m_pChatControl->SetHeightStatus(CHAT_HEIGHT_CLOSE);
	}
	else if(siHeight == 2)
	{
		if(m_pChatControl->GetHeightStatus() == CHAT_HEIGHT_CLOSE)
			m_pChatControl->SetHeightStatus(CHAT_HEIGHT_MEDIUM);
		else if(m_pChatControl->GetHeightStatus() == CHAT_HEIGHT_MEDIUM)
			m_pChatControl->SetHeightStatus(CHAT_HEIGHT_HIGH);
	}
	else
		m_pChatControl->SetHeightStatus(CHAT_HEIGHT_CLOSE);
}

BOOL	OnlinePannel::AddChat(UI32 uiID, CHAR *pStr, UI08 uiChatMode)
{
	return m_pChatControl->AddChat(uiID, pStr, uiChatMode);
}

BOOL	OnlinePannel::AddChat(CHAR *pStr, SI32 siContentColor, UI08 uiChatMode)
{
	return m_pChatControl->AddChat(pStr, siContentColor, uiChatMode);
}

BOOL	OnlinePannel::AddChatID(CHAR *pID, CHAR *pStr, UI08 uiChatMode)
{
	return m_pChatControl->AddChatID(pID, pStr, uiChatMode);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관리자용 명령인지 검사

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlinePannel::Check_ManagerOrder(char* lpChatting)
{
	if(lpChatting == NULL) return FALSE;

	if(lpChatting[0] == '/') return TRUE;
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	관리자용 명령 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlinePannel::Process_ManagerOrder(char* lpChatting)
{
	char*	lpStr;
	char	TempOrder[256];
	char*	lpStr1;
	char*	lpStr2;

	if(Check_ManagerOrder(lpChatting))
	{
		//====================================================================================		
		// 명령어 검사

		lpStr = &lpChatting[1];
		ZeroMemory(TempOrder, 256);
		sscanf(lpStr, "%s", TempOrder);

		//====================================================================================		
		// 명령어를 대문자로 수정
		lpStr1 = (char*)TempOrder;
		lpStr2 = (char*)&lpChatting[1];
		while((*lpStr1) != NULL)
		{
			*lpStr2 = (char)toupper(*lpStr2);

			lpStr1++;
			lpStr2++;
		}

		if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/HEROHIDE") == 0))
		{
			m_pMyOnlineWorld->pIOnlineChar->Show(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), FALSE);
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/HEROSHOW") == 0))
		{
			m_pMyOnlineWorld->pIOnlineChar->Show(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), TRUE);
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/INFOHIDE") == 0))
		{
			m_pMyOnlineWorld->m_InfoFlag = TRUE;
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/INFOSHOW") == 0))
		{
			m_pMyOnlineWorld->m_InfoFlag = FALSE;
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/WHISPER on") == 0))
		{
			m_pMyOnlineWorld->pOnlineMessenger->m_bWhisperOn = TRUE;
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/WHISPER off") == 0))
		{
			m_pMyOnlineWorld->pOnlineMessenger->m_bWhisperOn = FALSE;
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/GUILD on") == 0))
		{
			m_pMyOnlineWorld->m_bGuildOn = TRUE;
		}
		else if((m_pMyOnlineWorld->m_GMFlag != ON_USERGRADE_NORMAL) && (strcmp(lpChatting, "/GUILD off") == 0))
		{
			m_pMyOnlineWorld->m_bGuildOn = FALSE;
		}
		else if(strncmp(lpChatting, "/TO ", 4) == 0)
		{
			// 이부분은 귓속말
			char		TempSrcBuffer[256];
			char		TempBuffer[256];
			char		TempBuffer1[256];
			char		TempBuffer2[256];
			int			TempCount;
			int			TempSize;
			int			TempStart;

			strcpy((char*)TempSrcBuffer, lpChatting);
			ZeroMemory(TempBuffer,  256);
			ZeroMemory(TempBuffer1, 256);
			ZeroMemory(TempBuffer2, 256);
			TempSize = strlen((char*)TempSrcBuffer);

			// 앞에 /to 문자열을 삭제한다.
			TempStart = 0;
			for(TempCount = 0; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] == '/')
				{
					TempStart = TempCount + 3;
					break;
				}
			}

			// ID 를 검색한다.
			for(TempCount = TempStart; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] != ' ')
				{
					sscanf((char*)&TempSrcBuffer[TempCount], "%s", TempBuffer);
					TempStart = TempCount + strlen((char*)TempBuffer);
					break;
				}
			}

			// 운영자한테 귓속말 금지
			if(!strcmp(TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(8000495)))
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OPERATOR_WARNING));
				return FALSE;
			}

			// 다음에 오는 문장은 모두 Chatting Message
			strcpy((char*)TempBuffer1, (char*)&TempSrcBuffer[TempStart]);

			if(TempBuffer[0] && TempBuffer1[0])
			{
				// 채팅 부분 앞에 ' '부분을 검사하여 삭제
				for(TempCount = 0; TempCount < (int)strlen((char*)TempBuffer1); TempCount++)
				{
					if(TempBuffer1[TempCount] != ' ')
					{
						strcpy((char*)TempBuffer2, (char*)&TempBuffer1[TempCount]);
						break;
					}
				}

				if(TempBuffer2[0])
				{
					// 욕설 필터
					m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank((char*)TempBuffer2);

					// 서버로 귓말을 보낸다.
					m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendWhisper((char*)TempBuffer, (char*)TempBuffer2);

					// 상단정보요청
					m_pMyOnlineWorld->pOnlineClient->SendRequestGuildInfo((char*)TempBuffer);

					// 메신저를 띄운다.
					m_pMyOnlineWorld->pOnlineMessenger->SetAction(TRUE, TempBuffer);
					if(!m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						m_pMyOnlineWorld->pOnlineMessenger->AddChat(0, TempBuffer2);
					}
					
					// Chatting에 남기게 함
					//pOnlineFieldChat->AddChat(0, TempBuffer2);

					ZeroMemory(m_pMyOnlineWorld->m_MemoUser, 32);
					strncpy((char*)m_pMyOnlineWorld->m_MemoUser, (char*)TempBuffer, 30);
				}
			}
		}
		else if(strncmp(lpChatting, "/GUILD ", 6) == 0)
		{
			// 욕설 필터
			m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(lpChatting);

			//====================================================================================		
			// 명령어 전송
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
		}
		else if(strncmp(lpChatting, "/PARTYCHAT ", 10) == 0)
		{
			// 욕설 필터
			m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(lpChatting);

			//====================================================================================		
			// 명령어 전송
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
		}
		else if(strncmp(lpChatting, "/EXCLUDE ", 8) == 0)
		{
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
			
			// 앞에 /EXCLUDE 문자열을 삭제한다.
			int		TempStart = 0, TempCount, TempSize;
			char	TempSrcBuffer[256], TempBuffer[256];
			ZeroMemory(TempBuffer,  256);
			strcpy((char*)TempSrcBuffer, lpChatting);
			TempSize = strlen((char*)TempSrcBuffer);

			// 앞에 /EXCLUDE 문자열을 삭제한다.
			for(TempCount = 0; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] == '/')
				{
					TempStart = TempCount + 9;
					break;
				}
			}

			// ID 를 검색한다.
			for(TempCount = TempStart; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] != ' ')
				{
					sscanf((char*)&TempSrcBuffer[TempCount], "%s", TempBuffer);
					TempStart = TempCount + strlen((char*)TempBuffer);
					break;
				}
			}

			// BadFriend 리스트에 ID를 기록한다.
			m_pMyOnlineWorld->pMyData->m_BadFriendList.AddFriend(m_pMyOnlineWorld->pMyData->GetMyCharName(), TempBuffer, FALSE);
		}
		else
		{
			//====================================================================================		
			// 명령어 전송
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
		}

		return TRUE;
	}                   

	return FALSE;
}

VOID	OnlinePannel::AddWhisperList(char* lpname)
{
	// 같은 이름이 있나 검색후에 추가한다
	vector<WhisperData>::iterator it;
	for(it = m_WhisperList.begin(); it != m_WhisperList.end(); it++)
	{
		if(strcmp(it->szName, lpname) == 0)
			return;
	}

	if(m_WhisperList.size() == WHISPER_LIST_COUNT)
	{
		for(it = m_WhisperList.begin(); it != m_WhisperList.end(); it++)
		{
			it->siY += 16;
		}
		m_WhisperList.erase(m_WhisperList.begin());
	}

	WhisperData AddData;
	strncpy(AddData.szName, lpname, ON_ID_LENGTH);
	AddData.siX = 206;
	AddData.siY = 562 - m_WhisperList.size() * 16;

	m_WhisperList.push_back(AddData);
}

VOID	OnlinePannel::SetWhisperList(BOOL bAction)
{
	OnlineEditControlEx	*pOecEx;

	if(m_bWhisperMode == bAction)	return;

	m_bWhisperMode = bAction;

	if(bAction)
	{
		m_siChatNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(206, 418, 295, 578);
	}
	else
	{
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siChatNoField);
		m_siChatNoField = -1;

		if(m_WhisperList.size() > m_siWhisperSel)
		{
			CHAR szWhisperChat[128];
			sprintf(szWhisperChat, "/to %s ", m_WhisperList[m_siWhisperSel].szName);
			pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
			pOecEx->SetString(szWhisperChat);
			Handler_OnlineEditControlEx::SetFocus(m_hOecChat);
			
		}
	}
}

VOID	OnlinePannel::ChangeChatBasePos( SI32 siAddBottomLen )
{
	// 패널 채팅 입력 라인
	m_pt_PannelChatLine.y	=	ON_SCREEN_YSIZE - m_SprImg_PannelChatLine.Header.Ysize - siAddBottomLen;
	if( m_siFieldBlock_ChatLine != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siFieldBlock_ChatLine );
	m_siFieldBlock_ChatLine = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_PannelChatLine.x, m_pt_PannelChatLine.y, m_pt_PannelChatLine.x + m_SprImg_PannelChatLine.Header.Xsize, m_pt_PannelChatLine.y + m_SprImg_PannelChatLine.Header.Ysize);
	
	// 버튼 위치 교정
	m_Btn_ShowWhisperList.Create	(206,578 - siAddBottomLen, m_SprBtn_ShowWhisperList.Header.Xsize,	m_SprBtn_ShowWhisperList.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000512),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_ChangeChatWindow.Create	(575,578 - siAddBottomLen, m_SprBtn_ChangeChatWindow.Header.Xsize,	m_SprBtn_ChangeChatWindow.Header.Ysize,	NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_ShowImoticon.Create		(524,580 - siAddBottomLen, m_SprBtn_ShowImoticon.Header.Xsize,		m_SprBtn_ShowImoticon.Header.Ysize,		NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroPlayPause.Create		(539,580 - siAddBottomLen, m_SprBtn_MacroPlayPause.Header.Xsize,	m_SprBtn_MacroPlayPause.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000513),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroSetting.Create		(554,580 - siAddBottomLen, m_SprBtn_MacroSetting.Header.Xsize,		m_SprBtn_MacroSetting.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000514),	BUTTON_PUT_LEFT, TRUE );

	// 채팅 입력 라인 교정
	OnlineEditControlEx	*pOecEx;
	pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
	pOecEx->SetRectOfDraw( 233, 580 - siAddBottomLen, 233+288, 580+15 - siAddBottomLen );
	

	// 공지사항 위치 교정
	RECT	rcRect;
	pOecEx->GetRectOfDraw( rcRect );
	m_pMyOnlineWorld->pOnlineNoticeList->SetPosition( rcRect.left, rcRect.top + 1, rcRect.right - rcRect.left, 4 );

	// 채팅 내역창 교정
	m_pChatControl->SetDistY( siAddBottomLen );

}

//-----------------------------------------------------------------------------
// Name: AddSMSMessage()
// Code: actdoll (2004-06-14)
// Desc: SMS에 출력할 텍스트를 저장한다.
//-----------------------------------------------------------------------------
BOOL	OnlinePannel::AddSMSMessage( char *pszRecvTime, char *pszTelNumber, char *pszMessage )
{
	return ( ( m_pSMS ) ? m_pSMS->PushSMSData( pszRecvTime, pszTelNumber, pszMessage ) : FALSE );
}

// robypark 2005/1/28 14:3
// 공성전 유닛 변신 버튼 활성화 설정
void	OnlinePannel::SetShowTransformSiegeUnitBtn(BOOL bShow)
{
	m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.ShowWindow(bShow);

	if (bShow)
	{
		// 이전에 설정하지 않았다면
		if (-1 == m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle)
		{
			// No Field Area 설정
			m_sShowInformationAboutSiegeWarfareUnit.
				m_siNoFieldHandle
					= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea
							(710 - 3, 477 - 3, 
								710 - 3 + m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize + 3, 
								477 - 3 + m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize + 3);
		}
	}
	else	// No Field Area 해제
	{
		// 이전에 설정했다면
		if (-1 != m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle);
			m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle = -1;
		}
	}
}

// 공성전 유닛일 경우 인터페이스 활성화 설정
void	OnlinePannel::ShowWindow_SiegeWarfareUnitUI(DWORD dwShowWindow)
{
	// 유효한 데이터인가?
	if (NULL == m_pSiegeWarfareUnitUI)
		return;

	m_pSiegeWarfareUnitUI->ShowWindow(dwShowWindow);
}

// 공성전 유닛일 경우 인터페이스 윈도 생성
void	OnlinePannel::SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow)
{
	// 유효한 데이터인가?
	if (NULL == m_pSiegeWarfareUnitUI)
		return;

	m_pSiegeWarfareUnitUI->SiegeWarfareUnitUI_CreateWindow(dwShowWindow);
}

// 공성전 유닛일 경우 인터페이스 윈도 파괴
void	OnlinePannel::SiegeWarfareUnitUI_DestroyWindow(void)
{
	// 유효한 데이터인가?
	if (NULL == m_pSiegeWarfareUnitUI)
		return;

	m_pSiegeWarfareUnitUI->SiegeWarfareUnitUI_DestroyWindow();
}

// 공성전 유닛으로 변신 윈도 활성화 설정
void	OnlinePannel::ShowWindow_SiegeWarfareTransformUIWnd(BOOL bShow)
{
	// 유효한 데이터인가?
	if (NULL == m_pSiegeWarfareTransformUIWnd)
		return;

	// 이미 활성화된 상태라면
	if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
	{
		// 활성화된 창을 닫기라면
		if (FALSE == bShow)
		{
			// close
			m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);
		}
	}
	// 비활성환 된 상태라면
	else
	{
		// 비활성화된 창을 열기라면
		if (TRUE == bShow)
		{
			// open
			m_pSiegeWarfareTransformUIWnd->ShowWindow(TRUE);
		}
	}
}

// robypark 2005/1/28 15:51
// 공성전 상단 공성 유닛 리스트 요청 응답
// ON_RESPONSE_LIST_GUILD_UNIT
UI16	OnlinePannel::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	// 유효하지 않은 데이터
	if (NULL == m_pSiegeWarfareTransformUIWnd)
		return 0;

	// 비활성화되었다면
	if (FALSE == m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
		return 0;

	// 메시지 처리
	return m_pSiegeWarfareTransformUIWnd->OnResponseSiegeWarfareUnitList(pMsg);
}

// robypark 2005/1/27 22:6
// 공성전 유닛일 경우 인터페이스 얻기
CSiegeWarfareUnitUI	*OnlinePannel::GetSiegeWarfareUnitUIHandle(void)
{
	return m_pSiegeWarfareUnitUI;
}

// 공성전 유닛으로 변신 윈도
CSiegeWarfareTransformUIWnd	*OnlinePannel::GetSiegeWarfareTransformUIWnd(void)
{
	return m_pSiegeWarfareTransformUIWnd;
}
