/**************************************************************************************************
	だ橾貲 : OnlinePannel.cpp
	氬渡濠 : 薑霞遵

	ж欽 っ喧 檣攪む檜蝶
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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	m_pSiegeWarfareUnitUI			= NULL;			// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶
	m_pSiegeWarfareTransformUIWnd	= NULL;			// 奢撩瞪 嶸棉戲煎 滲褐 孺紫
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlinePannel::~OnlinePannel()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	營薑綠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 瓣た 鐘お煤曖 晦獄高擊 陝 措陛滌 樹橫羹啗煎 蜃醮場朝棻.
	OnlineEditControlEx	*pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
//	if( pOecEx )		pOecEx->GetIMEBase()->SetInputStatus( IME_CMODE_NATIVE );


	TempYetMoney		= 0xffffffffffffffff;
	TempYetBattlePower	= 0xffffffff;
	TempYetCredit		= 0xffffffff;

	// ぬ割 瞪羹
	m_pt_Pannel.x			=	0;
	m_pt_Pannel.y			=	ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize;
	ChangeChatBasePos( (m_pSMS)?m_pSMS->GetCurrentYSize():0 );

	SetWhisperList(FALSE);
	m_siWhisperSel = 0;
	m_WhisperList.clear();

	// robypark 2005/1/27 22:6
	// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶
	if ((m_pSiegeWarfareUnitUI->IsVisibleWindow())
		|| (m_pSiegeWarfareUnitUI->IsMinimizeWindow()))
	{
		m_pSiegeWarfareUnitUI->SiegeWarfareUnitUI_DestroyWindow();
	}

	// 奢撩瞪 嶸棉戲煎 滲褐 孺紫
	if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
	{
		m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);
		m_pSiegeWarfareTransformUIWnd->SetSendedMsg(FALSE);

	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
 
	// 奩萄衛 葬模蝶陛 蟾晦�� 腎橫 氈橫撿 и棻.
	if(m_pMyOnlineWorld->pOnlineResource == NULL)
		clGrp.Error("OnlinePannel::Init", "m_pMyOnlineWorld->pOnlineResource == NULL");

	// 檜嘐雖 煎萄
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
	// 奢撩瞪 嶸棉 鼻鷓 薑爾 爾晦 幗が 蝶Щ塭檜お 煎萄
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_UNIT_INFO_BASE.Spr",					m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis);	// 幗が 纔舒葬
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_B.Spr",							m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton);	// 幗が 檜嘐雖
	
	m_siPannelNoField[0] = pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize, 200, ON_SCREEN_YSIZE);
	m_siPannelNoField[1] = pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(200, ON_SCREEN_YSIZE - 25, 600, ON_SCREEN_YSIZE);
	m_siPannelNoField[2] = pOnlineWorld->pOnlineFieldArea->SetNoFieldArea(600, ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize, ON_SCREEN_XSIZE, ON_SCREEN_YSIZE);

	// 奢撩瞪 嶸棉檣 鼻鷓縑憮 賅萇 奢撩瞪 嶸棉爾晦 幗が 檣攪む檜蝶 No Field Area Index
	m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle = -1;

	// SMS 鐘お煤 儅撩(и措虜 陛棟)
	if( g_stConnectServerInfo.m_iServiceNation == DF_CODE_NATION_IDENTITY_KOREA ) 
	{
		m_pSMS = new OnlineSMS;
		m_pSMS->Init(pOnlineWorld);
	}

	// 瓣た 鐘お煤 儅撩
	m_pChatControl = new OnlineFieldChat;
	m_pChatControl->Init(pOnlineWorld, ((m_pSMS)?m_pSMS->GetCurrentYSize():0) );
	m_pChatControl->SetDistY( ((m_pSMS)?m_pSMS->GetCurrentYSize():0) );

	// ぬ割 瞪羹
	m_pt_Pannel.x			=	0;
	m_pt_Pannel.y			=	ON_SCREEN_YSIZE - m_SprImg_Pannel.Header.Ysize;
	// ぬ割 瓣た 殮溘 塭檣
	m_pt_PannelChatLine.x	=	201;
	m_pt_PannelChatLine.y	=	ON_SCREEN_YSIZE - m_SprImg_PannelChatLine.Header.Ysize - ((m_pSMS)?m_pSMS->GetCurrentYSize():0);
	
	// 幗が擊 儅撩 и棻.
	m_Btn_ShowWhisperList.Create	(206,578 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_ShowWhisperList.Header.Xsize,	m_SprBtn_ShowWhisperList.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000512),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_ChangeChatWindow.Create	(575,578 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_ChangeChatWindow.Header.Xsize,	m_SprBtn_ChangeChatWindow.Header.Ysize,	NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_ShowImoticon.Create		(524,580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_ShowImoticon.Header.Xsize,		m_SprBtn_ShowImoticon.Header.Ysize,		NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroPlayPause.Create		(539,580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_MacroPlayPause.Header.Xsize,		m_SprBtn_MacroPlayPause.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000513),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroSetting.Create		(554,580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), m_SprBtn_MacroSetting.Header.Xsize,		m_SprBtn_MacroSetting.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000514),	BUTTON_PUT_LEFT, TRUE );

	m_Btn_Inventory.Create	(619,519, m_SprBtn_Inventory.Header.Xsize,	m_SprBtn_Inventory.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000515),							BUTTON_PUT_LEFT, TRUE );	// 檣漸饜葬.
	m_Btn_CharStatus.Create	(644,519, m_SprBtn_CharStatus.Header.Xsize,	m_SprBtn_CharStatus.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_STATUS),				BUTTON_PUT_LEFT, TRUE );	// 議葛攪 鼻鷓.
	m_Btn_Account.Create	(669,519, m_SprBtn_Account.Header.Xsize,	m_SprBtn_Account.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_MYMONEY),				BUTTON_PUT_LEFT, TRUE );	// 濰睡
	m_Btn_Mercenary.Create	(694,519, m_SprBtn_Mercenary.Header.Xsize,	m_SprBtn_Mercenary.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SOLIDER),					BUTTON_PUT_LEFT, TRUE );	// 辨煽
	m_Btn_Quest.Create		(719,519, m_SprBtn_Quest.Header.Xsize,		m_SprBtn_Quest.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000516),							BUTTON_PUT_LEFT, TRUE );	// 紫遺蜓.
	m_Btn_Friends.Create	(744,519, m_SprBtn_Friends.Header.Xsize,	m_SprBtn_Friends.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FRIENDLISTINFO_FRIENDLIST),	BUTTON_PUT_LEFT, TRUE );	// 耀掘跡煙.
	m_Btn_Option.Create		(769,519, m_SprBtn_Option.Header.Xsize,		m_SprBtn_Option.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000517), BUTTON_PUT_LEFT, TRUE );	// 撢た

	m_Btn_AggressiveMode.Create	(650,554, m_SprBtn_Common.Header.Xsize, m_SprBtn_Common.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PEACEMODE), BUTTON_PUT_LEFT, TRUE );
	m_Btn_TradePermission.Create(704,554, m_SprBtn_Common.Header.Xsize, m_SprBtn_Common.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PERMIT_TRADE), BUTTON_PUT_LEFT, TRUE );

	/*
	// robypark 2004/11/13 14:3
	// 奢撩瞪 鼻鷓 薑爾 爾檜晦 幗が
	m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.Create(710 + 3, 447 + 3, 
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Xsize,
				m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton.Header.Ysize,
				m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SHOW_SIEGEWARFAREUNIT_INFORMATION),
				BUTTON_PUT_LEFT, TRUE );

	// robypark 2005/1/5 17:18
	// 奢撩瞪 嶸棉縑憮 => 橾奩 嶸棉戲煎 滲褐 蹂羶 幗が
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

	// 瓣た辨 縑蛤お 鐘お煤擊 儅撩
	OnlineEditControlEx	*pOecEx;
	if( m_hOecChat )	Handler_OnlineEditControlEx::KillControl( m_hOecChat );
	RECT	rcRect = { 233, 580 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0), 233+288, 580+15 - ((m_pSMS)?m_pSMS->GetCurrentYSize():0) };
	m_hOecChat		= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, MAX_CHAT_BUFFER);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
	pOecEx->SetFont( m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(255, 255, 255) );
	m_siFieldBlock_ChatLine	= pOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_PannelChatLine.x, m_pt_PannelChatLine.y, m_pt_PannelChatLine.x + m_SprImg_PannelChatLine.Header.Xsize, m_pt_PannelChatLine.y + m_SprImg_PannelChatLine.Header.Ysize);

	// 奢雖餌о 嬪纂 夥紱堅
	m_pMyOnlineWorld->pOnlineNoticeList->SetPosition( rcRect.left, rcRect.top + 1, rcRect.right - rcRect.left, 4 );			// 奢雖餌о 嬪纂 夥紱堅

	// robypark 2005/1/27 22:6
	// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶
	m_pSiegeWarfareUnitUI	= NULL;
	m_pSiegeWarfareUnitUI = new CSiegeWarfareUnitUI;
	m_pSiegeWarfareUnitUI->Init(pOnlineWorld);

	// 奢撩瞪 嶸棉戲煎 滲褐 孺紫
	m_pSiegeWarfareTransformUIWnd = NULL;
	m_pSiegeWarfareTransformUIWnd = new CSiegeWarfareTransformUIWnd;
	m_pSiegeWarfareTransformUIWnd->Init(pOnlineWorld);
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯 л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	// 奢撩瞪 嶸棉 鼻鷓 薑爾 爾晦 幗が 蝶Щ塭檜お п薯
	clGrp.FreeXspr(m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis);		// 幗が 纔舒葬
	clGrp.FreeXspr(m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton);	// 幗が 檜嘐雖

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
	// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶
	if (m_pSiegeWarfareUnitUI)
	{
		delete m_pSiegeWarfareUnitUI;
		m_pSiegeWarfareUnitUI	= NULL;
	}

	// 奢撩瞪 嶸棉戲煎 滲褐 孺紫
	if (m_pSiegeWarfareTransformUIWnd)
	{
		delete m_pSiegeWarfareTransformUIWnd;
		m_pSiegeWarfareTransformUIWnd = NULL;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ィ葭 л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
			// 瓣た婦溼(в萄 鼻)
			if(m_pMyOnlineWorld->pOnlineVillage == NULL)
			{
				// в萄縑憮 葬欐 酈陛 揚溜堅 瓣た ん醴蝶陛 橈朝 鼻鷓橾 唳辦
				if ((m_pMyOnlineWorld->bReturnKeyDown)	// 葬欐酈陛 揚溜堅
					&& (!Handler_OnlineEditControlEx::IsFocus(m_hOecChat))	// 瓣た 殮溘璽縑 ん醴蝶陛 橈堅
					&& ((m_pMyOnlineWorld->GetGamePollStatus() == ON_GAMEPOLL_FIELD)	// в萄檜剪釭
						|| (m_pMyOnlineWorld->GetGamePollStatus() == ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE)))	// 熱撩 檣攪む檜蝶 鼻橾 唳辦
				{
					m_pMyOnlineWorld->bReturnKeyDown = FALSE;		// 葬欐 樓撩 薯剪

					// 敏蜓 �側瘓� 鼻鷓橾陽
					if(m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						// 蹴模 鼻鷓塭賊
						if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
						{
							// お溯檜萄 磁檜釭 檜楛 薹縑 ん醴蝶陛 蜃醮螳 氈雖 彊棻賊 瓣た塭檣縑 ん醴蝶蒂 蜃醮塭
							if(!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->GetHOECFriend()))
								Handler_OnlineEditControlEx::SetFocus( m_hOecChat);
						}
					}
					// 謝っ檜 �側瘓音Ц� 氈堅 嬴檜蠱擊 厥朝 醞橾陽
					else if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn() && m_pMyOnlineWorld->pOnlineBooth->GetStatus() == ON_BENCH_IN_INPUT_ITEM_INFO)
					{
						// 謝っ 殮溘璽縑 ん醴蝶蒂 蜃醮塭
						m_pMyOnlineWorld->pOnlineBooth->SetFocusEditBox();
					}
					// 濰睡曖 耀掘跡煙縑 ん醴蝶陛 橈堅 議葛攪 鼻鷓璽曖 檜葷 掖羹 璽縑 ん醴蝶陛 橈棻賊
					else if(!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->GetHOECFriend()) &&
					!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->m_pOnlineCharStatus->GetHOECRename()))
					{
						// 瓣た縑 ん醴蝶蒂 遽棻.
						Handler_OnlineEditControlEx::SetFocus( m_hOecChat);
						pOecEx->Clear();
					}
				}	// if(m_pMyOnlineWorld->bReturnKeyDown && !Handler_OnlineEditControlEx::IsFocus(m_hOecChat) && m_pMyOnlineWorld->GetGamePollStatus() == ON_GAMEPOLL_FIELD)
			}	// if(m_pMyOnlineWorld->pOnlineVillage == NULL)
			// 瓣た婦溼(葆擊 鼻)
			else
			{
				// 葬欐 酈陛 揚溜堅 瓣た ん醴蝶陛 橈朝 鼻鷓橾 唳辦
				if(m_pMyOnlineWorld->bReturnKeyDown && !Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
				{
					m_pMyOnlineWorld->bReturnKeyDown = FALSE;			// 葬欐 樓撩 薯剪
					if(m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						;
					}
					//friend list 蹺陛.蹺陛蹺陛
					else if(!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->m_pOnlineCharStatus->GetHOECRename()) &&
						!Handler_OnlineEditControlEx::IsFocus(m_pMyOnlineWorld->pOnlineTradeBook->GetHOECFriend()))
					{
						//Handler_OnlineEditControlEx::ClearCurrentFocus( pGame->Hwnd );
						Handler_OnlineEditControlEx::SetFocus( m_hOecChat );
						pOecEx->Clear();
					}
				}
			}
				
			// 殮溘醞檣 縑覃 夢蝶縑憮 縛攪酈蒂 蟻棻.
			if( pOecEx->IsPushEnter() )
			{
				if( Handler_OnlineEditControlEx::IsFocus( m_hOecChat ) )
				{
					// 釭曖 議葛攪陛 蜓擊 ц棻		
					strcpy( szMyCharSpeechBuffer, pOecEx->GetString() );
					
					// 蝶お葭 薑爾陛 薑�旁狫� 彊戲賊 瓣た 寰銑葡
					if( !CheckLeadByte( szMyCharSpeechBuffer ) )		
					{
						pOecEx->Clear();
						Handler_OnlineEditControlEx::ClearCurrentFocus();
						break;
					}

					// 葆擊寰縑憮 �分� 謝っ賅萄縑憮
					if( m_pMyOnlineWorld->pOnlineVillage || m_pMyOnlineWorld->pOnlineBooth->IsActionIn() )
					{
						if( GetChattingMode() != CHATTINGMODE_GUILD )	// 望萄 礙檜 嬴棲塭賊
						{
							// 擅薹縑 敏樓蜓 頂辨檜 橈棻賊 瓣た 寰銑葡
							if( strncmp( szMyCharSpeechBuffer, "/to ", 4 ) != 0 )
							{
								pOecEx->Clear();
								Handler_OnlineEditControlEx::ClearCurrentFocus();
								break;
							}
						}
					}

					// 溯漣, 褐辨蛔晝 5檜ж朝 敏蜓擊 й熱橈棻.
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

					// 望萄 瓣た橾 唳辦
					if( GetChattingMode() == CHATTINGMODE_GUILD )
					{
						// 遴艙濠 貲滄檣雖 �挫恉炾�
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

					// 餌辨濠陛 熱撩 檣攪む檜蝶縑 氈擊 陽 warp 貲滄戲煎 濠褐檜釭 棻艇 餌辨濠蒂
					// 錶Щ 衛鑒 熱 橈棻.
					// 遴艙濠 貲滄檣雖 �挫恉炾�
					if( Check_ManagerOrder((char*)szMyCharSpeechBuffer ) == TRUE )
					{
						// 錶Щ 貲滄橫檜貊
						if( strnicmp( szMyCharSpeechBuffer, "/w ", 3 ) == 0 )
						{
							// 爾頂溥朝 議葛攪 UniqueID 橢晦
							CHAR			cSep[] = " ,";
							CHAR			*pToken;
							CHAR			szTempSpeechBuffer[1024];
							UI16			uiPlayerID;

							strcpy(szTempSpeechBuffer, szMyCharSpeechBuffer);

							// 貲滄橫蒂 橢橫螞棻.
							pToken = strtok(szTempSpeechBuffer, cSep);

							// 錶Щ衛酈溥朝 議葛攪 uiPlayerID蒂 橢橫螞棻.
							if((pToken	= strtok(NULL, cSep)) != NULL)
							{
								uiPlayerID	=	atoi(pToken);

								// 熱撩 檣攪む檜蝶縑 氈朝 議葛攪塭賊
								if (TRUE == m_pMyOnlineWorld->pIOnlineChar->IsInVillageDefenceInterface(uiPlayerID))
								{
									pOecEx->Clear();
									Handler_OnlineEditControlEx::ClearCurrentFocus();
									break;
								}
							}
						}
					}

					// 遴艙濠 貲滄橾 唳辦
					if( Check_ManagerOrder( (char*)szMyCharSpeechBuffer ) )
					{
						// 遴艙濠 貲滄橫 撮た
						Process_ManagerOrder( (char*)szMyCharSpeechBuffer );
						
						// 縑蛤お 夢蝶蒂 梟荻�� и棻.
						pOecEx->Clear();
					}
					// 嬴棲塭賊
					else
					{
						if( szMyCharSpeechBuffer[0] != NULL )
						{
							// 踹撲 в攪 羹觼
							m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(szMyCharSpeechBuffer);
							
							// в萄塭賊
							if( m_pMyOnlineWorld->pOnlineVillage == NULL )
							{
								// 頂 議葛攪 檜葷擊 嫡堅
								CHAR *pCharName	= m_pMyOnlineWorld->pMyData->GetMyCharName();
								if( !strcmp( pCharName, m_pMyOnlineWorld->pOnlineText->Get( 8000495 ) ) )
									m_pMyOnlineWorld->pIOnlineChar->SetSpeech( m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), szMyCharSpeechBuffer, TRUE );
								else
									m_pMyOnlineWorld->pIOnlineChar->SetSpeech( m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), szMyCharSpeechBuffer );	
								
								// 剪楚醞橾陽 
								if( m_pMyOnlineWorld->pOnlineTrade->IsActive() )
									AddChatID( m_pMyOnlineWorld->pMyData->GetMyCharName(), szMyCharSpeechBuffer, CHATTINGMODE_NORMAL );
								else if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
									m_pChatControl->AddChat(0, szMyCharSpeechBuffer, CHATTINGMODE_NORMAL);
							}

							// 憮幗縑啪 釭曖 蜓擊 爾頂遽棻.
							if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
								m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendChatting(szMyCharSpeechBuffer);
							
							// 縑蛤お 夢蝶蒂 梟荻�� и棻.
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

			// .擊 鎰戲賊 檜瞪敏蜓 鼻渠蒂 撮たп遽棻.
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
			
			// ん虜馬檜 0檜堅, 頂陛 瓣た醞檜 嬴棍陽朝
			if((m_pMyOnlineWorld->pMyData->GetHealth() == 0) && (m_pMyOnlineWorld->pIOnlineChar->CheckSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()) == FALSE)
				&& !m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
			{
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HUNGRY2));
				m_pChatControl->AddChat(0, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HUNGRY2), CHATTINGMODE_NORMAL);
			}
			
			// 葆擊 夤橾 唳辦(в萄)
			if(m_bIsInVillage == FALSE )
			{
				if (m_pMyOnlineWorld->pOnlineFieldAuction->IsEntered()) return;
				
				if( m_pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK && m_pMyOnlineWorld->m_bForcedLogout)			
					m_pMyOnlineWorld->ExitOnline();

				if( m_pChatControl )	m_pChatControl->Poll();

				// 瓣た璽 餌檜鍔 夥紱朝 幗が 援蒂陽
				if( m_Btn_ChangeChatWindow.Process( m_bMouseDown ) == TRUE )
				{
					SetChatOpen();
				}

				// 瓣た璽 贗葛 衛 ん醴蝶
				if( pOecEx->IsPointInRect( pt ) && m_bMouseDown )
					Handler_OnlineEditControlEx::SetFocus( m_hOecChat );

				// 詭撮雖 紫雜憲葡 贗葛
				if(PtInRect(&m_rt_Messenger, pt) && m_pMyOnlineWorld->fLeftOneSwitch == 1)
				{
					/** @author 梯撩遽 (sagolboss)
						@date   2004-08-09 螃�� 1:55
						幗斜葬んお曖 熱薑蹂羶縑 評塭 褐辨蛔晝 溯漣 6檜ж朝 餌辨ж雖 跤и棻朝 詭撮雖 夢蝶蒂 嗥鄴棲棻
					*/
		
					if(m_pMyOnlineWorld->pMyData->GetTradeGrade() <= 5)
					{
						///--> 蹺陛脹 囀萄 衛濛 (sagolboss)
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8000693));
						pOecEx->Clear();
						Handler_OnlineEditControlEx::ClearCurrentFocus();
						///<-- 蹺陛 囀萄 部
					}
					else
					{
						if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
							m_pMyOnlineWorld->pOnlineMessenger->SetMini(FALSE);
						else
							m_pMyOnlineWorld->pOnlineMessenger->SetMini(TRUE);
					}
				}

				// SMS 轎溘 塽 餌檜鍔 夥紱朝 幗が 援蒂陽
				if(m_pSMS)			
				{
					m_pSMS->Action();
					if( m_pSMS->IsModeJustChanged() )	// 寞旎瞪縑 SMS曖 ⑽鷓陛 夥船歷棻賊
					{
						ChangeChatBasePos( m_pSMS->GetCurrentYSize() );													// 瓣た璽 嬪纂 夥紱堅
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
						if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			//耀掘 跡煙
						{
							if( m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_FRIEND )
								m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
							else
								m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_FRIEND );
						}					
						else										
							m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_FRIEND );
					}
					else if( m_Btn_Account.Process( m_bMouseDown ) == TRUE )			//癱濠旎
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
						if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			//蝶囌
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
					if( m_Btn_TradePermission.Process( m_bMouseDown ) == TRUE )					// 剪楚 ъ辨
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
					// 雖援晦 熱薑 ///////////////////////////////////////////////////////////////////////////////////////////////
					else if(m_Btn_MacroPlayPause.Process(m_bMouseDown))					m_pMyOnlineWorld->m_pChatMacro->OnRunningToggle();
					else if(m_Btn_MacroSetting.Process(m_bMouseDown))					m_pMyOnlineWorld->m_pChatMacro->OnButtonPush();
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////

					else if( m_Btn_Quest.Process( m_bMouseDown ) == TRUE )					// そ
					{
						m_pMyOnlineWorld->m_pQuestInterface->OnButtonPush();
					}
					else if( m_Btn_Mercenary.Process( m_bMouseDown ) == TRUE )				//
					{
						m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
					}
					else if( m_Btn_Option.Process( m_bMouseDown ) == TRUE )				// �秣瞍麥�
					{
						// 啪歜縑憮 釭除棻.
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
					// 奢撩瞪 嶸棉 鼻鷓 爾晦 幗が 籀葬
					else if ((m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT) &&
						(m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.Process( m_bMouseDown ) == TRUE)
						&& (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
						&& (FALSE == m_bIsInVillage))
					{
						SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
						// ⑷營 奢撩 嶸棉 鼻鷓 薑爾 爾檜晦 塭賊
						if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
						{
							// ⑷營 奢撩 嶸棉 鼻鷓 薑爾 寰爾檜晦煎 撲薑
							scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
						}
						else
						{
							// ⑷營 奢撩 嶸棉 鼻鷓 薑爾 爾檜晦煎 撲薑
							scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
						}

						m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
					}
					// robypark 2005/1/5 17:34
					// 奢撩瞪 嶸棉 => 橾奩 嶸棉 滲褐 蹂羶 幗が 籀葬
					else if ((m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT) &&
						(m_sShowInformationAboutSiegeWarfareUnit.m_Btn_RequestReturnNormalCharacter.Process( m_bMouseDown ) == TRUE)
						&& (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
						&& (FALSE == m_bIsInVillage))
					{
						// 滲褐 蹂羶 詭衛雖 爾頂晦
						m_pMyOnlineWorld->pOnlineClient->SendRequestNormalCharacter(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID());
					}
					*/

					// 奢撩 嶸棉 滲褐 孺紫 籀葬
					if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
					{
						SetShowTransformSiegeUnitBtn(FALSE);

						// 辨煽璽檜 翮溥氈棻賊 殘嬴遽棻.
						if (m_pMyOnlineWorld->m_pMercenary->IsAction())
						{
							m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
						}

						if (FALSE == m_pSiegeWarfareTransformUIWnd->Action())
						{
							m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);

							// 餌辨濠 殮溘 棻衛 �側瘓�
							m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
						}
					}

					// 奢撩 嶸棉 檣攪む檜蝶 籀葬
					if ((m_pSiegeWarfareUnitUI->IsVisibleWindow())
						|| (m_pSiegeWarfareUnitUI->IsMinimizeWindow()))
					{
						// 辨煽璽檜 翮溥氈棻賊 殘嬴遽棻.
						if (m_pMyOnlineWorld->m_pMercenary->IsAction())
						{
							m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
						}

						if (FALSE == m_pSiegeWarfareUnitUI->Action())
						{
							m_pSiegeWarfareUnitUI->ShowWindow(SW_HIDE);
						}
					}

					// 奢撩瞪 嶸棉 嶸棉 滲褐 幗が 籀葬
					if ((m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.Process( m_bMouseDown ) == TRUE)
						&& (FALSE == m_bIsInVillage))
					{
						// 滲褐 孺紫 �側瘓�
						if (FALSE == m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
						{
							if ((m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive())				// 剪楚 蹂羶 醞檜剪釭
									|| (m_pMyOnlineWorld->pOnlineTrade->IsActive())					// 剪楚 醞檜剪釭
									|| (m_pMyOnlineWorld->pMyData->IsWaitBattleResponse())			// 瞪癱 渠晦醞橾 陽 剪楚虞擠
									|| (m_pMyOnlineWorld->pMyData->IsGotoVillage()) 				// 葆擊縑 菟橫陛朝 醞檜剪釭
									|| (m_pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse())		// 葆擊 菟橫陛晦 嬪и 渠晦 醞檜塭賊
									|| (m_pMyOnlineWorld->pIOnlineChar->IsMoving(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))	// 檜翕 醞檜塭賊
									|| (m_pMyOnlineWorld->pOnlineMsgBoxUsePortal->IsActive())		// ん驍擊 檜辨ж溥朝 唳辦
									|| (m_pMyOnlineWorld->pOnlineQuestPlay->IsAction()))			// NPC縑啪 蠡蝶お蒂 嫡朝 醞檜剪釭
							{
								// 滲褐й 熱 橈棻
							}
							else
							{
								// 滲褐 孺紫 �側瘓� 撩奢 罹睡
								if (m_pSiegeWarfareTransformUIWnd->ShowWindow(TRUE))
								{
									SetShowTransformSiegeUnitBtn(FALSE);

									// 翮溥霞 璽菟 殘晦
									// 檣漸饜葬陛 翮溥 氈戲賊 殘嬴遽棻.
									if(m_pMyOnlineWorld->pOnlineInventory->IsActive())
										m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

									// 濰睡陛 翮溥 氈戲賊 殘嬴遽棻.
									if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
										m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

									// 嘐棲裘紫 殘朝棻!!
									if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive())
										m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);

									// 衛蝶蠱 詭景 殘晦
									if(m_pMyOnlineWorld->pOnlineSetting->IsAction())
										m_pMyOnlineWorld->pOnlineSetting->SetAction(FALSE);

									// 辨煽璽檜 翮溥氈棻賊 殘嬴遽棻.
									if (m_pMyOnlineWorld->m_pMercenary->IsAction())
									{
										m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
									}

									// 餌辨濠 殮溘 虞晦
									m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);
								}
							}
						}
					}
//				}
			}	// if(m_bIsInVillage == FALSE )
			// 葆擊 寰橾 唳辦
			else
			{
				// 葆擊 寰橾 唳辦
				if (m_pMyOnlineWorld->pOnlineVillage)
				{
					SetShowTransformSiegeUnitBtn(FALSE);

					// 奢撩 嶸棉 滲褐 孺紫 籀葬
					if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
					{
						m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);
					}
				}

				// 奢撩 嶸棉 檣攪む檜蝶 籀葬
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

				// 瓣た璽 餌檜鍔 夥紱朝 幗が 援蒂陽
				if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHAT) == 0)
				{
					if( m_Btn_ChangeChatWindow.Process(m_bMouseDown) == TRUE )
					{
						SetChatOpen();
					}
				}

				// 瓣た璽 贗葛 衛 ん醴蝶
				if( pOecEx->IsPointInRect( pt ) && m_bMouseDown )
					if(!m_pMyOnlineWorld->pOnlineBooth->IsActionIn())
						Handler_OnlineEditControlEx::SetFocus( m_hOecChat);

				// 詭撮雖 紫雜憲葡 贗葛
				if(PtInRect(&m_rt_Messenger, pt) && m_pMyOnlineWorld->fVillageLeftOneSwitch == 1)
				{
					if(m_pMyOnlineWorld->pOnlineMessenger->IsMini())
						m_pMyOnlineWorld->pOnlineMessenger->SetMini(FALSE);
					else
						m_pMyOnlineWorld->pOnlineMessenger->SetMini(TRUE);
				}
/*
				// robypark 2004/12/10 20:25
				// 奢撩瞪 嶸棉 鼻鷓 爾晦 幗が 籀葬
				if ((FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()) &&
						(m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.Process( m_bMouseDown ) == TRUE))
				{
					SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
					// ⑷營 奢撩 嶸棉 鼻鷓 薑爾 爾檜晦 塭賊
					if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
					{
						// ⑷營 奢撩 嶸棉 鼻鷓 薑爾 寰爾檜晦煎 撲薑
						scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}
					else
					{
						// ⑷營 奢撩 嶸棉 鼻鷓 薑爾 爾檜晦煎 撲薑
						scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
					}

					m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
				}
*/
				// SMS 轎溘 塽 餌檜鍔 夥紱朝 幗が 援蒂陽
				if(m_pSMS)			
				{
					m_pSMS->Action();
					if( m_pSMS->IsModeJustChanged() )	// 寞旎瞪縑 SMS曖 ⑽鷓陛 夥船歷棻賊
					{
						ChangeChatBasePos( m_pSMS->GetCurrentYSize() );													// 瓣た璽 嬪纂 夥紱堅
					}
				}
/*				// 詭撮雖 紫雜憲葡 贗葛
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
						if( m_Btn_Friends.Process( m_bMouseDown ) == TRUE )				// 耀掘跡煙 薑爾
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
						if( m_Btn_Account.Process( m_bMouseDown ) == TRUE )				// 嘐棲裘
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
							// 啪歜縑憮 釭除棻.
							m_pMyOnlineWorld->ExitOnline();
						}
					}
//				}

			}	// else <== if(m_bIsInVillage == FALSE )
			
			// 葆辦蝶 蕾斬衛 檣攪む檜蝶 撲貲
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
				// 奢撩瞪 嶸棉 鼻鷓 爾晦 幗が Ё摹紫遺蜓 籀葬
				SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
				if ((TRUE == m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.IsInMouse())
					&& (FALSE == m_bIsInVillage))
				{
					// 奢撩 嶸棉檜剪釭 熱撩 檣攪む檜蝶 鼻檜塭賊
					if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT) || (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()))
					{
						sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOL_TIP_SHOW_SIEGEWARFAREUNIT_INFO));
						m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
					}
				}

				// robypark 2005/1/5 17:36
				// 奢撩瞪 嶸棉 => 橾奩 嶸棉 滲褐 蹂羶 幗が Ё摹紫遺蜓 籀葬
				if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					&& (TRUE == m_sShowInformationAboutSiegeWarfareUnit.m_Btn_RequestReturnNormalCharacter.IsInMouse())
					&& (FALSE == m_bIsInVillage))
				{
					sprintf(szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_CANCEL_TRANSFORM));
					m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
				}
				*/
				// 奢撩瞪 嶸棉 滲褐 蹂羶 Ё摹紫遺蜓 籀葬
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	萄煎辦 л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 奢雖餌о List
	if(!Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
	{
		m_pMyOnlineWorld->pOnlineNoticeList->Poll(pSurface);
	}

	//============================================================================================
	// 瓣た璽
	if( m_pChatControl )	m_pChatControl->Draw(pSurface);
	if( m_pSMS )			m_pSMS->Draw(pSurface);

	clGrp.GetClipArea(siClipLeft, siClipTop, siClipRight, siClipBottom);
	clGrp.SetClipFullScreen();

	if(clGrp.LockSurface(pSurface) == TRUE)
	{	
		// Main ж欽檣攪む檜蝶
		clGrp.PutSpriteT(m_pt_Pannel.x,			m_pt_Pannel.y,			m_SprImg_Pannel.Header.Xsize,			m_SprImg_Pannel.Header.Ysize,			m_SprImg_Pannel.Image);
		clGrp.PutSpriteT(m_pt_PannelChatLine.x,	m_pt_PannelChatLine.y,	m_SprImg_PannelChatLine.Header.Xsize,	m_SprImg_PannelChatLine.Header.Ysize,	m_SprImg_PannelChatLine.Image);

		// 1:1渠�倥� 詭衛雖 紫雜 憲葡(億煎螞 詭衛雖陛 氈堅 葆辦蝶陛 嬪縑 橈戲賊 梭緇剪葡)
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

		// 雖援晦 熱薑 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(m_pMyOnlineWorld->m_pChatMacro->IsRunning())		m_Btn_MacroPlayPause.Put(&m_SprBtn_MacroPlayPause,2,2,3,BUTTON_PUT_NOMOVE);
		else											m_Btn_MacroPlayPause.Put(&m_SprBtn_MacroPlayPause,0,0,1,BUTTON_PUT_NOMOVE);
		
		m_Btn_MacroSetting.Put(&m_SprBtn_MacroSetting,0,0,1,BUTTON_PUT_NOMOVE);		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		SI16		siHPBarWidth;

		// 寡堅Ь 熱纂蒂掘и棻.
		siHPBarWidth = ((m_pMyOnlineWorld->pMyData->GetHealth() * m_SprGauge_Hungry.Header.Xsize) / ON_MAX_HEALTH);

		RECT rtClip;
		clGrp.GetClipRect(&rtClip);

		clGrp.SetClipArea(80, 523, 80+siHPBarWidth,  523+m_SprGauge_Hungry.Header.Ysize);
		clGrp.PutSpriteCT(80, 523 , m_SprGauge_Hungry.Header.Xsize, m_SprGauge_Hungry.Header.Ysize, m_SprGauge_Hungry.Image);
		clGrp.SetClipArea(rtClip.left, rtClip.top, rtClip.right, rtClip.bottom);


		// 敏蜓鼻渠 跡煙
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
		// 奢撩瞪 嶸棉 鼻鷓 薑爾 爾晦 幗が 斜葬晦
		// 餌辨濠陛 奢撩瞪 嶸棉檜堅
		SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
		if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT) && (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
			&& (FALSE == m_bIsInVillage))
		{
			// 奢撩瞪 嶸棉 鼻鷓 爾檜晦
			// 幗が 纔舒葬 斜葬晦
			clGrp.PutSpriteT(710, 447, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize,
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Image);

			// 奢撩瞪 嶸棉縑憮 => 橾奩 嶸棉戲煎 給嬴陛晦
			// 幗が 纔舒葬 斜葬晦
			clGrp.PutSpriteT(710, 477, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize,
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Image);

			// 葆辦蝶 醴憮陛 幗が嬪縑 氈棻賊,
			if (m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.IsInMouse())
			{
				m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.PutSpriteButton(
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetX(), 
																m_sShowInformationAboutSiegeWarfareUnit.m_Btn_ShowInformationAboutSiegeWarfareUnit.GetY(), 
																&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton, 
																1, FALSE, FALSE);
			}
			// ⑷營 撲薑檜 鼻鷓 爾檜晦 塭賊
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

			// 奢撩瞪 嶸棉 => 橾奩 嶸棉 給嬴陛晦 幗が 斜葬晦
			m_sShowInformationAboutSiegeWarfareUnit.
					m_Btn_RequestReturnNormalCharacter.Put(
							&m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitButton,
							0, 2, 1, BUTTON_PUT_NOMOVE );
		}
		*/

		if (m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.IsVisibleWindow())
		{
			// 奢撩瞪 嶸棉嶸棉戲煎 滲褐
			// 幗が 纔舒葬 斜葬晦
			clGrp.PutSpriteT(710, 477, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize,
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize, 
								m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Image);

			// 幗が
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

		// 衛除
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

		// 鼻欽ル衛
		SI32 siGuildInGrade = m_pMyOnlineWorld->pMyData->GetGuildClass();
		if(m_pMyOnlineWorld->pMyData->GetGuildName())
		{
			sprintf((char*)szNumericString, "[%s][%s]", m_pMyOnlineWorld->pMyData->GetGuildName(), m_pMyOnlineWorld->pOnlineText->Get(m_pMyOnlineWorld->m_cltIGuildClass.GetClassName(siGuildInGrade)));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,54, 539,132, 14, szNumericString, RGB(250, 250, 250));
		}

		//溯漣 ル衛
		sprintf(szNumericString,"%d",m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,12,535,26,13,szNumericString,RGB(250, 250, 250));

		// 奢問溘5
		sprintf(szNumericString, "%d", MaxWeight);		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 39, 559,48,14, szNumericString, RGB(250, 250, 250));

		// 褐辨蛔晝
		sprintf(szNumericString, "%d", m_pMyOnlineWorld->pMyData->GetTradeGrade());
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 127, 559,59,14, szNumericString, RGB(250, 250, 250));

		// 絲
		MakeMoneyText(m_pMyOnlineWorld->pMyData->GetMoney(), (char*)szNumericString);
	  	m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,39, 580,147,14, szNumericString, RGB(250, 250, 250));


		
		// 瓣た旋 鎰橫輿晦
		if(Handler_OnlineEditControlEx::IsFocus(m_hOecChat))
		{
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
			pOecEx->Draw( hdc );
		}
		else
		{
			// 奢雖餌о List 轎溘
			SelectObject( hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
			m_pMyOnlineWorld->pOnlineNoticeList->Draw(hdc);
		}

		// 敏蜓鼻渠 跡煙
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

		// actdoll (2004/03/31 13:26) : = 檜漸お = 絮紫蠡蝶お 識婪 璋濠 頂羲 瞳擠
		// 絮紫 檜漸お朝 и措縑憮虜 --v
//		if(pGame->LanguageCode == ELANGAGECODE_KOREAN)
//		{
//			char	pszString1[128], pszString2[128];
//			strcpy	( pszString1, "[絮紫雖酈晦 檜漸お 霞ч 醞]");
//			sprintf	( pszString2, "⑷營 %d貲檜 絮紫縑 鷓斛晦蒂 笮懊蝗棲棻.", m_pMyOnlineWorld->GetDokdoQuestCompleteUserCount() );
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 101, 6, pszString1, RGB( 0, 0, 0 ) );	
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 100, 5, pszString1, RGB( 255, 128, 0 ) );	
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 101, 21, pszString2, RGB( 0, 0, 0 ) );	
//			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 100, 20, pszString2, RGB( 255, 255, 128 ) );	
//		}

		/*
		// robypark 2004/11/13 13:58
		// 奢撩瞪 嶸棉 鼻鷓 薑爾 爾晦 幗が 臢蝶お 斜葬晦
		// 餌辨濠陛 奢撩瞪 嶸棉檜堅
		SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
		if ((scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT) && (FALSE == m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
			&& (FALSE == m_bIsInVillage))
		{
			// 賅萇 奢撩瞪 嶸棉 鼻鷓 爾晦 幗が
			m_sShowInformationAboutSiegeWarfareUnit.
				m_Btn_ShowInformationAboutSiegeWarfareUnit.
					Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));																

			// 奢撩瞪 嶸棉 => 橾奩 嶸棉 滲褐 蹂羶 幗が
			m_sShowInformationAboutSiegeWarfareUnit.
				m_Btn_RequestReturnNormalCharacter.
					Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));																
		}
		*/
		if (m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.IsVisibleWindow())
		{
			// 奢撩瞪 嶸棉 滲褐 蹂羶 幗が
			m_sShowInformationAboutSiegeWarfareUnit.
				m_btnRequestTrasnformUI.
					Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		
		pSurface->ReleaseDC(hdc);
	}

	// robypark 2005/1/27 22:6
	// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶 斜葬晦
	m_pSiegeWarfareUnitUI->Draw(pSurface);

	// 奢撩瞪 嶸棉戲煎 滲褐 孺紫 斜葬晦
	m_pSiegeWarfareTransformUIWnd->Draw(pSurface);


	// 璋濠菟擊 鎰橫遽棻.
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		SEffectData			TempEffect;
		
		// 絲 滲и剪
		if(TempYetMoney != 0xffffffffffffffff)
		{
			if(TempYetMoney != m_pMyOnlineWorld->pMyData->GetMoney())
			{
				// 滲�倆� 熱 轎溘
				TempEffect.m_Type                                = EFFECTTYPE_CHANGENUMBER;
				TempEffect.m_EffectData_ChangeNum.m_Frame        = 0;
				TempEffect.m_EffectData_ChangeNum.m_XPos         = 93;
				TempEffect.m_EffectData_ChangeNum.m_YPos         = 579;
				TempEffect.m_EffectData_ChangeNum.m_ChangeNumber = m_pMyOnlineWorld->pMyData->GetMoney() - TempYetMoney;
				m_pMyOnlineWorld->pOnlineFieldEffect->AddEffect(TempEffect);
			}
		}
		TempYetMoney = m_pMyOnlineWorld->pMyData->GetMoney();
	
		// 褐辨紫 滲и剪
		if(TempYetCredit != 0xffffffff)
		{
			if((UI32)TempYetCredit != m_pMyOnlineWorld->pMyData->GetTradeCredit())
			{
				// 滲�倆� 熱 轎溘
				TempEffect.m_Type                                = EFFECTTYPE_CHANGENUMBER;
				TempEffect.m_EffectData_ChangeNum.m_Frame        = 0;
				TempEffect.m_EffectData_ChangeNum.m_XPos         = 145;
				TempEffect.m_EffectData_ChangeNum.m_YPos         = 560;
				TempEffect.m_EffectData_ChangeNum.m_ChangeNumber = (__int64)(m_pMyOnlineWorld->pMyData->GetTradeCredit() - TempYetCredit);
				m_pMyOnlineWorld->pOnlineFieldEffect->AddEffect(TempEffect);
			}
		}
		TempYetCredit = m_pMyOnlineWorld->pMyData->GetTradeCredit();

		// 奢問溘 滲и剪
		if(TempYetBattlePower != 0xffffffff)
		{
			if(TempYetBattlePower != MaxWeight)
			{
				// 滲�倆� 熱 轎溘
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊寰縑 氈朝雖 Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		// 奢撩瞪 嶸棉檜塭賊 瞪癱賅萄, ゎ�飛藒� 幗が擊 餌辨 熱 橈棻. 傳и 濠翕戲煎 瞪癱賅萄陛 脹棻.
		if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			// ゎ�飛藒撊騤�,
			if (m_pMyOnlineWorld->IsPeace())
			{
				// 瞪癱賅萄煎 撲薑
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

		// 辨煽璽 幗が 撲薑
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

		// 奢撩瞪 嶸棉檜塭賊 瞪癱賅萄, ゎ�飛藒� 幗が擊 餌辨 熱 橈棻. 傳и 濠翕戲煎 瞪癱賅萄陛 脹棻.
		if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			// ゎ�飛藒撊騤�,
			if (m_pMyOnlineWorld->IsPeace())
			{
				// 瞪癱賅萄煎 撲薑
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

		// 辨煽璽 幗が 撲薑
		if((m_siDisableFlag & ONLINEPANNEL_DISABLEFLAG_CHARINFO))
			m_Btn_Mercenary.SetAction(FALSE);
		else
			m_Btn_Mercenary.SetAction(TRUE);

		m_Btn_ChangeChatWindow.SetAction(TRUE);
		m_Btn_MacroPlayPause.SetAction(TRUE);
		m_Btn_MacroSetting.SetAction(TRUE);
	}
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//瞪癱 ,ゎ�� 賅萄 掘碟
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//剪楚 ,離欽  賅萄 掘碟
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

// robypark 2004/10/26 11:5 ゎ�飛藒� 幗が 臢蝶お虜 撲薑ж晦
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

// robypark 2004/10/26 11:5 剪楚賅萄 幗が 臢蝶お虜 撲薑ж晦
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 嬪纂
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlinePannel::SetVillagePos(SI32 VilXPos, SI32 VilYPos)
{
	VillagePositionX = VilXPos;
	VillagePositionY = VilYPos;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	璽擊 翮熱 氈朝雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 葆擊檣雖 Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlinePannel::GetfIsVillage(void)
{
	return m_bIsInVillage;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 嬪纂 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlinePannel::GetVillagePosition(SI32& xPos, SI32& yPos)
{
	xPos = VillagePositionX;
	yPos = VillagePositionY;
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瓣た婦溼л熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlinePannel::ChangeChattingMode(UI08 ChattinMode)
{
	// 偃檣 剪楚醞縑朝 '橾奩 瓣た'戲煎 夤縑 腎雖 彊朝棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦葬濠辨 貲滄檣雖 匐餌

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlinePannel::Check_ManagerOrder(char* lpChatting)
{
	if(lpChatting == NULL) return FALSE;

	if(lpChatting[0] == '/') return TRUE;
	
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	婦葬濠辨 貲滄 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlinePannel::Process_ManagerOrder(char* lpChatting)
{
	char*	lpStr;
	char	TempOrder[256];
	char*	lpStr1;
	char*	lpStr2;

	if(Check_ManagerOrder(lpChatting))
	{
		//====================================================================================		
		// 貲滄橫 匐餌

		lpStr = &lpChatting[1];
		ZeroMemory(TempOrder, 256);
		sscanf(lpStr, "%s", TempOrder);

		//====================================================================================		
		// 貲滄橫蒂 渠僥濠煎 熱薑
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
			// 檜睡碟擎 敏樓蜓
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

			// 擅縑 /to 僥濠翮擊 餉薯и棻.
			TempStart = 0;
			for(TempCount = 0; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] == '/')
				{
					TempStart = TempCount + 3;
					break;
				}
			}

			// ID 蒂 匐儀и棻.
			for(TempCount = TempStart; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] != ' ')
				{
					sscanf((char*)&TempSrcBuffer[TempCount], "%s", TempBuffer);
					TempStart = TempCount + strlen((char*)TempBuffer);
					break;
				}
			}

			// 遴艙濠и纔 敏樓蜓 旎雖
			if(!strcmp(TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(8000495)))
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OPERATOR_WARNING));
				return FALSE;
			}

			// 棻擠縑 螃朝 僥濰擎 賅舒 Chatting Message
			strcpy((char*)TempBuffer1, (char*)&TempSrcBuffer[TempStart]);

			if(TempBuffer[0] && TempBuffer1[0])
			{
				// 瓣た 睡碟 擅縑 ' '睡碟擊 匐餌ж罹 餉薯
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
					// 踹撲 в攪
					m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank((char*)TempBuffer2);

					// 憮幗煎 敏蜓擊 爾魚棻.
					m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendWhisper((char*)TempBuffer, (char*)TempBuffer2);

					// 鼻欽薑爾蹂羶
					m_pMyOnlineWorld->pOnlineClient->SendRequestGuildInfo((char*)TempBuffer);

					// 詭褐盪蒂 嗥遴棻.
					m_pMyOnlineWorld->pOnlineMessenger->SetAction(TRUE, TempBuffer);
					if(!m_pMyOnlineWorld->pOnlineMessenger->IsAction())
					{
						m_pMyOnlineWorld->pOnlineMessenger->AddChat(0, TempBuffer2);
					}
					
					// Chatting縑 陴晦啪 л
					//pOnlineFieldChat->AddChat(0, TempBuffer2);

					ZeroMemory(m_pMyOnlineWorld->m_MemoUser, 32);
					strncpy((char*)m_pMyOnlineWorld->m_MemoUser, (char*)TempBuffer, 30);
				}
			}
		}
		else if(strncmp(lpChatting, "/GUILD ", 6) == 0)
		{
			// 踹撲 в攪
			m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(lpChatting);

			//====================================================================================		
			// 貲滄橫 瞪歎
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
		}
		else if(strncmp(lpChatting, "/PARTYCHAT ", 10) == 0)
		{
			// 踹撲 в攪
			m_pMyOnlineWorld->m_SlanderFilter.ReplaceStringIgnoreBlank(lpChatting);

			//====================================================================================		
			// 貲滄橫 瞪歎
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
		}
		else if(strncmp(lpChatting, "/EXCLUDE ", 8) == 0)
		{
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
			
			// 擅縑 /EXCLUDE 僥濠翮擊 餉薯и棻.
			int		TempStart = 0, TempCount, TempSize;
			char	TempSrcBuffer[256], TempBuffer[256];
			ZeroMemory(TempBuffer,  256);
			strcpy((char*)TempSrcBuffer, lpChatting);
			TempSize = strlen((char*)TempSrcBuffer);

			// 擅縑 /EXCLUDE 僥濠翮擊 餉薯и棻.
			for(TempCount = 0; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] == '/')
				{
					TempStart = TempCount + 9;
					break;
				}
			}

			// ID 蒂 匐儀и棻.
			for(TempCount = TempStart; TempCount < TempSize; TempCount++)
			{
				if(TempSrcBuffer[TempCount] != ' ')
				{
					sscanf((char*)&TempSrcBuffer[TempCount], "%s", TempBuffer);
					TempStart = TempCount + strlen((char*)TempBuffer);
					break;
				}
			}

			// BadFriend 葬蝶お縑 ID蒂 晦煙и棻.
			m_pMyOnlineWorld->pMyData->m_BadFriendList.AddFriend(m_pMyOnlineWorld->pMyData->GetMyCharName(), TempBuffer, FALSE);
		}
		else
		{
			//====================================================================================		
			// 貲滄橫 瞪歎
			m_pMyOnlineWorld->pOnlineClient->SendGMMessage(&lpChatting[1]);
		}

		return TRUE;
	}                   

	return FALSE;
}

VOID	OnlinePannel::AddWhisperList(char* lpname)
{
	// 偽擎 檜葷檜 氈釭 匐儀�醴� 蹺陛и棻
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
	// ぬ割 瓣た 殮溘 塭檣
	m_pt_PannelChatLine.y	=	ON_SCREEN_YSIZE - m_SprImg_PannelChatLine.Header.Ysize - siAddBottomLen;
	if( m_siFieldBlock_ChatLine != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siFieldBlock_ChatLine );
	m_siFieldBlock_ChatLine = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_pt_PannelChatLine.x, m_pt_PannelChatLine.y, m_pt_PannelChatLine.x + m_SprImg_PannelChatLine.Header.Xsize, m_pt_PannelChatLine.y + m_SprImg_PannelChatLine.Header.Ysize);
	
	// 幗が 嬪纂 掖薑
	m_Btn_ShowWhisperList.Create	(206,578 - siAddBottomLen, m_SprBtn_ShowWhisperList.Header.Xsize,	m_SprBtn_ShowWhisperList.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000512),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_ChangeChatWindow.Create	(575,578 - siAddBottomLen, m_SprBtn_ChangeChatWindow.Header.Xsize,	m_SprBtn_ChangeChatWindow.Header.Ysize,	NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_ShowImoticon.Create		(524,580 - siAddBottomLen, m_SprBtn_ShowImoticon.Header.Xsize,		m_SprBtn_ShowImoticon.Header.Ysize,		NULL,											BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroPlayPause.Create		(539,580 - siAddBottomLen, m_SprBtn_MacroPlayPause.Header.Xsize,	m_SprBtn_MacroPlayPause.Header.Ysize,	m_pMyOnlineWorld->pOnlineText->Get(8000513),	BUTTON_PUT_LEFT, TRUE );
	m_Btn_MacroSetting.Create		(554,580 - siAddBottomLen, m_SprBtn_MacroSetting.Header.Xsize,		m_SprBtn_MacroSetting.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(8000514),	BUTTON_PUT_LEFT, TRUE );

	// 瓣た 殮溘 塭檣 掖薑
	OnlineEditControlEx	*pOecEx;
	pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecChat );
	pOecEx->SetRectOfDraw( 233, 580 - siAddBottomLen, 233+288, 580+15 - siAddBottomLen );
	

	// 奢雖餌о 嬪纂 掖薑
	RECT	rcRect;
	pOecEx->GetRectOfDraw( rcRect );
	m_pMyOnlineWorld->pOnlineNoticeList->SetPosition( rcRect.left, rcRect.top + 1, rcRect.right - rcRect.left, 4 );

	// 瓣た 頂羲璽 掖薑
	m_pChatControl->SetDistY( siAddBottomLen );

}

//-----------------------------------------------------------------------------
// Name: AddSMSMessage()
// Code: actdoll (2004-06-14)
// Desc: SMS縑 轎溘й 臢蝶お蒂 盪濰и棻.
//-----------------------------------------------------------------------------
BOOL	OnlinePannel::AddSMSMessage( char *pszRecvTime, char *pszTelNumber, char *pszMessage )
{
	return ( ( m_pSMS ) ? m_pSMS->PushSMSData( pszRecvTime, pszTelNumber, pszMessage ) : FALSE );
}

// robypark 2005/1/28 14:3
// 奢撩瞪 嶸棉 滲褐 幗が �側瘓� 撲薑
void	OnlinePannel::SetShowTransformSiegeUnitBtn(BOOL bShow)
{
	m_sShowInformationAboutSiegeWarfareUnit.m_btnRequestTrasnformUI.ShowWindow(bShow);

	if (bShow)
	{
		// 檜瞪縑 撲薑ж雖 彊懊棻賊
		if (-1 == m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle)
		{
			// No Field Area 撲薑
			m_sShowInformationAboutSiegeWarfareUnit.
				m_siNoFieldHandle
					= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea
							(710 - 3, 477 - 3, 
								710 - 3 + m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Xsize + 3, 
								477 - 3 + m_sShowInformationAboutSiegeWarfareUnit.m_SprBtn_ShowInformationAboutSiegeWarfareUnitBasis.Header.Ysize + 3);
		}
	}
	else	// No Field Area п薯
	{
		// 檜瞪縑 撲薑ц棻賊
		if (-1 != m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle)
		{
			// No Field Area п薯
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle);
			m_sShowInformationAboutSiegeWarfareUnit.m_siNoFieldHandle = -1;
		}
	}
}

// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶 �側瘓� 撲薑
void	OnlinePannel::ShowWindow_SiegeWarfareUnitUI(DWORD dwShowWindow)
{
	// 嶸�褲� 等檜攪檣陛?
	if (NULL == m_pSiegeWarfareUnitUI)
		return;

	m_pSiegeWarfareUnitUI->ShowWindow(dwShowWindow);
}

// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶 孺紫 儅撩
void	OnlinePannel::SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow)
{
	// 嶸�褲� 等檜攪檣陛?
	if (NULL == m_pSiegeWarfareUnitUI)
		return;

	m_pSiegeWarfareUnitUI->SiegeWarfareUnitUI_CreateWindow(dwShowWindow);
}

// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶 孺紫 だ惚
void	OnlinePannel::SiegeWarfareUnitUI_DestroyWindow(void)
{
	// 嶸�褲� 等檜攪檣陛?
	if (NULL == m_pSiegeWarfareUnitUI)
		return;

	m_pSiegeWarfareUnitUI->SiegeWarfareUnitUI_DestroyWindow();
}

// 奢撩瞪 嶸棉戲煎 滲褐 孺紫 �側瘓� 撲薑
void	OnlinePannel::ShowWindow_SiegeWarfareTransformUIWnd(BOOL bShow)
{
	// 嶸�褲� 等檜攪檣陛?
	if (NULL == m_pSiegeWarfareTransformUIWnd)
		return;

	// 檜嘐 �側瘓音� 鼻鷓塭賊
	if (m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
	{
		// �側瘓音� 璽擊 殘晦塭賊
		if (FALSE == bShow)
		{
			// close
			m_pSiegeWarfareTransformUIWnd->ShowWindow(FALSE);
		}
	}
	// 綠�側瘓� 脹 鼻鷓塭賊
	else
	{
		// 綠�側瘓音� 璽擊 翮晦塭賊
		if (TRUE == bShow)
		{
			// open
			m_pSiegeWarfareTransformUIWnd->ShowWindow(TRUE);
		}
	}
}

// robypark 2005/1/28 15:51
// 奢撩瞪 鼻欽 奢撩 嶸棉 葬蝶お 蹂羶 擬港
// ON_RESPONSE_LIST_GUILD_UNIT
UI16	OnlinePannel::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	// 嶸�褲狫� 彊擎 等檜攪
	if (NULL == m_pSiegeWarfareTransformUIWnd)
		return 0;

	// 綠�側瘓音Ц�棻賊
	if (FALSE == m_pSiegeWarfareTransformUIWnd->IsVisibleWindow())
		return 0;

	// 詭衛雖 籀葬
	return m_pSiegeWarfareTransformUIWnd->OnResponseSiegeWarfareUnitList(pMsg);
}

// robypark 2005/1/27 22:6
// 奢撩瞪 嶸棉橾 唳辦 檣攪む檜蝶 橢晦
CSiegeWarfareUnitUI	*OnlinePannel::GetSiegeWarfareUnitUIHandle(void)
{
	return m_pSiegeWarfareUnitUI;
}

// 奢撩瞪 嶸棉戲煎 滲褐 孺紫
CSiegeWarfareTransformUIWnd	*OnlinePannel::GetSiegeWarfareTransformUIWnd(void)
{
	return m_pSiegeWarfareTransformUIWnd;
}
