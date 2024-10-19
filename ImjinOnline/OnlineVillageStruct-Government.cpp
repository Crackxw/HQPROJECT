/************************************************************************************************
	파일명 : OnlineVillageStruct-Government.cpp
	담당자 : 이윤석

	마을 관청 처리
************************************************************************************************/
#include <GSL.h>

#include <Main.h>
#include <Mouse.h>
#include <clGame.h>

#include "charanimation.h"
#include <kindinfo.h>
#include <myfile.h>
#include <etc.h>


#include "..\\Gersang\\Music.h"
#include "OnlineWorld.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineVillage.h"
#include "OnlineVillageStruct-Government.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "OnlineNumberControl.h"
#include "OnlineETC.h"
#include "OnlineFont.h"
#include "OnlinePannel.h"
#include "OnlineResource.h"
#include "OnlineMsgBox.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "MakePalette.h"
#include "OnlineCommonStruct.h"
#include "Timer.h"
#include "OnlineTradeParser.h"
#include "OnlineKeyboard.h"
#include "OnlineVillageManager.h"
#include <Common.h>
#include "OnlineMessenger.h"
#include "OnlineMercenary.h"

extern	_clGame* pGame;
extern _InputDevice				IpD;




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OnlineVillageStructureGovernment::OnlineVillageStructureGovernment()
{
	// Base
	m_NowWork   = 0;
	m_NextWork  = 0;
	m_fWaitFlag = 0;

	m_YetEditHWND         = NULL;
	m_YetEditHWNDEnable   = 0;
	m_YetEditLockShortKey = 0;

	// Image
	ZeroMemory(&m_Image_MainInterface_Face,        sizeof(XSPR));	

	ZeroMemory(&m_Image_InvestmentInterface_Back,                   sizeof(XSPR));
	ZeroMemory(&m_Image_DescriptInterface_Back,                     sizeof(XSPR));
	ZeroMemory(&m_Image_GuildInterface_Back,                        sizeof(XSPR));
	ZeroMemory(&m_Image_VillageInfoInterface_Back,                  sizeof(XSPR));
	ZeroMemory(&m_Image_VillageInfoDefenceInvestmentInterface_Back, sizeof(XSPR));	

	ZeroMemory(&m_Image_Button_Guild_DB,           sizeof(XSPR));	
	ZeroMemory(&m_Image_Button_All_MM,             sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_EM,             sizeof(XSPR));
	ZeroMemory(&m_Image_Button_All_B,              sizeof(XSPR));
	ZeroMemory(&m_Image_Scroll_All_M,              sizeof(XSPR));


	m_Image_Scroll_All_SBU = NULL;
	m_Image_Scroll_All_SBD = NULL;
	m_Image_Scroll_All_SBBU = NULL;
	m_Image_Scroll_All_SBBD = NULL;
	
	// Investment Data
	ZeroMemory(m_InvestmentData, sizeof(m_InvestmentData));
	m_NowSelectInvestment       = 0;
	m_NowSettingInvestmentMoney = 0;
	m_stCapital       = NULL;
	m_stCapitalCount  = 0;
	m_stCapitalSelect = 0;
	m_stCapitalScroll = 0;
	m_InvestmentAfterMoney = 0;

	// 안내문
	m_pDescriptEditBox = NULL;

	ZeroMemory(m_VillageDescript_Descript, sizeof(m_VillageDescript_Descript));
	ZeroMemory(m_VillageInfo_Descript, sizeof(m_VillageInfo_Descript));
	m_VillageInfo_DescriptScroll = 0;
	m_VillageInfo_DescriptLine   = 0;

	m_VillageInfo_InOutPlayerCount = 0;
	ZeroMemory(m_VillageInfo_StructList, sizeof(m_VillageInfo_StructList));
	m_VillageInfo_RectHRGN    = NULL;
	m_VillageInfo_RectBigHRGN = NULL;

	// Scroll
	m_StartTickCount          = 0;
	m_NowScrollInvestmentType = 0;
	m_NowScroll               = 0;

	m_Guild_NowState = 0;
	m_Guild_NowMessageState = 0;

	m_Guild_GuildName = NULL;

	m_GuildJoinWaitPlayerListCount = 0;
	m_GuildJoinWaitPlayerList      = 0;
	m_GuildJoinWaitPlayerList_PageMaxCount = 0;
	m_GuildJoinWaitPlayerList_PageNowCount = 0;

	m_GuildGuildPlayerListCount = 0;
	m_GuildGuildPlayerList      = 0;
	m_GuildGuildPlayerList_PageMaxCount = 0;
	m_GuildGuildPlayerList_PageNowCount = 0;

	m_GuildDiffGuildListCount = 0;
	m_GuildDiffGuildList      = 0;

	m_GuildVillageListCount			= 0;
	m_GuildVillageList_PageMaxCount	= 0;
	m_GuildVillageList_PageNowCount	= 1;

	m_GuildFlag_MaxCount = 0;
	m_GuildFlag_Index    = 0;
	m_GuildCreateCost	 = 0;
	m_RectHRGN    = NULL;
	m_RectBigHRGN = NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OnlineVillageStructureGovernment::~OnlineVillageStructureGovernment()
{
	Free();
}

BOOL	OnlineVillageStructureGovernment::Action()
{
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID OnlineVillageStructureGovernment::Init(UI16 VillageCode, cltOnlineWorld* pOnlineWorld, VillageStructManager* pVillageStructManager)
{
	SI32 TempCount;

	Free();

	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	// Load Image
	clGrp.LoadXspr("Online\\VillageBack\\GOVERNMENT_INVESTMENT.Spr",    m_Image_InvestmentInterface_Back);
	clGrp.LoadXspr("Online\\VillageBack\\GOVERNMENT_VDES.Spr",          m_Image_DescriptInterface_Back);
	clGrp.LoadXspr("Online\\VillageBack\\GOVERNMENT_Guild.Spr",         m_Image_GuildInterface_Back);
	clGrp.LoadXspr("Online\\VillageBack\\GOVERNMENT_BACK.Spr",          m_Image_VillageInfoInterface_Back);
	clGrp.LoadXspr("Online\\VillageBack\\Barrack_AllImage_CheckInterface.Spr", m_Image_VillageInfoDefenceInvestmentInterface_Back);

	clGrp.LoadXspr("Online\\VillageBack\\GuildDB.Spr",           m_Image_Button_Guild_DB);
	clGrp.LoadXspr("Online\\VillageBack\\Farm_MainButton.Spr",	 m_Image_Button_All_MM);
	clGrp.LoadXspr("Online\\VillageBack\\AllImage_ButtonEM.Spr", m_Image_Button_All_EM);
	clGrp.LoadXspr("Online\\VillageBack\\BANK_AUCTIONEXIT.Spr",  m_Image_Button_All_B);
	clGrp.LoadXspr("Online\\VillageBack\\AllImage_ScrollM.Spr",  m_Image_Scroll_All_M);

	m_Image_Scroll_All_SBU  = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SCROLLV_UP);
	m_Image_Scroll_All_SBD  = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SCROLLV_DOWN);

	m_Image_Scroll_All_SBBU = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_UPBIG);
	m_Image_Scroll_All_SBBD = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_DOWNBIG);

	// Button
	m_Button_Main_Investment.Create(296,      82,  m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_VillageDescript.Create(395, 82,  m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_VILIMPOMATION), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_VillageInfo.Create(490,     82,  m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_VILLAGEINFO), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_Guild.Create(587,           82,  m_Image_Button_All_MM.Header.Xsize, m_Image_Button_All_MM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATION), BUTTON_PUT_LEFT, TRUE);
	m_Button_Main_Exit.Create(677,            467, m_Image_Button_All_B.Header.Xsize,  m_Image_Button_All_B.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_EXIT), BUTTON_PUT_LEFT, TRUE);

	m_Button_Investment_SetInvestment.Create(688, 425,    m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT),        BUTTON_PUT_LEFT, TRUE);
	m_Button_Investment_InvestmentGetOut.Create(610, 425, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT), BUTTON_PUT_LEFT, TRUE);

	m_Button_Investment_InvestmentGetOut_OK.Create(355, 335,     m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT),   BUTTON_PUT_LEFT, TRUE);
	m_Button_Investment_InvestmentGetOut_Cancel.Create(450, 335, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTBUTTON_EXIT), BUTTON_PUT_LEFT, TRUE);

	//m_Button_Investment_InvestmentTypeScrollUp.Create(492, 81,    m_Image_Scroll_All_SBU->Header.Xsize, m_Image_Scroll_All_SBU->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	//m_Button_Investment_InvestmentTypeScrollDown.Create(492, 217, m_Image_Scroll_All_SBD->Header.Xsize, m_Image_Scroll_All_SBD->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_Button_Investment_InvestmentDataScrollUp.Create(749, 104,    m_Image_Scroll_All_SBU->Header.Xsize, m_Image_Scroll_All_SBU->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_Investment_InvestmentDataScrollDown.Create(749, 196, m_Image_Scroll_All_SBD->Header.Xsize, m_Image_Scroll_All_SBD->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_Button_Investment_VillageProfitYearScrollUp.Create(692, 345,    m_Image_Scroll_All_SBBU->Header.Xsize, m_Image_Scroll_All_SBBU->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_Investment_VillageProfitYearScrollDown.Create(708, 345,  m_Image_Scroll_All_SBBD->Header.Xsize, m_Image_Scroll_All_SBBD->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_Button_Investment_VillageProfitVYearScrollUp.Create(589, 236,    m_Image_Scroll_All_SBU->Header.Xsize, m_Image_Scroll_All_SBBU->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_Investment_VillageProfitVYearScrollDown.Create(589, 355,  m_Image_Scroll_All_SBD->Header.Xsize, m_Image_Scroll_All_SBBD->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	
	m_Button_Descropt_SetDescropt.Create(688, 425,  m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OK1),   BUTTON_PUT_LEFT, TRUE);

	m_Button_VillageInfo_InvestmentDefence.Create(688, 425,       m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_DEFUP), BUTTON_PUT_LEFT, TRUE);
	m_Button_VillageInfo_InvestmentDefenceOK.Create(355, 335,     m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT), BUTTON_PUT_LEFT, TRUE);
	m_Button_VillageInfo_InvestmentDefenceCancel.Create(450, 335, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTBUTTON_EXIT),    BUTTON_PUT_LEFT, TRUE);

	m_Button_VillageInfo_DescriptScrollUp.Create(749, 104,   m_Image_Scroll_All_SBU->Header.Xsize, m_Image_Scroll_All_SBU->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_VillageInfo_DescriptScrollDown.Create(749, 196, m_Image_Scroll_All_SBD->Header.Xsize, m_Image_Scroll_All_SBD->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	m_Button_Guild_MakeGuild.Create(614, 111,                 m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONBIL), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_DeleteGuild.Create(683, 111,               m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONDES), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_JoinGuild.Create(614, 133,                m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_JOIN),   BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_DismissalGuild.Create(683, 133,           m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_MANAGE),    BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_FlagGuild.Create(683, 155,                m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FLAGCHOICE),      BUTTON_PUT_LEFT, TRUE);

	m_Button_Guild_DiffGuild.Create(614, 177,                m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_OTHERTRADEASSOCIATION), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_PlayerJoinDismissalGuild.Create(683, 177, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONYN), BUTTON_PUT_LEFT, TRUE);

	m_Button_Guild_OK.Create(613, 425,          m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_Cancel.Create(688, 425,      m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTBUTTON_EXIT), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_JoinOK.Create(538, 425,      m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONY), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_JoinCancel.Create(613, 425,  m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONN), BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_VillageList.Create(613, 425, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_LIST),   BUTTON_PUT_LEFT, TRUE);

	m_Button_Guild_DHS_Appointment.Create(388, 425, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_DHS_APPOINT),   BUTTON_PUT_LEFT, FALSE);
	m_Button_Guild_HND_Appointment.Create(463, 425, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_HND_APPOINT),   BUTTON_PUT_LEFT, FALSE);
	m_Button_Guild_Class_Dissmissal.Create(538, 425, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_CLASS_DISSMISSAL),   BUTTON_PUT_LEFT, FALSE);
	m_Button_Guild_DismissalOK.Create(613, 425, m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE),   BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_JoinExit.Create(688, 425,    m_Image_Button_All_EM.Header.Xsize, m_Image_Button_All_EM.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTBUTTON_EXIT), BUTTON_PUT_LEFT, TRUE);

	m_Button_Guild_InvestmentTypeScrollUp.Create(749, 204,   m_Image_Scroll_All_SBU->Header.Xsize, m_Image_Scroll_All_SBU->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_Button_Guild_InvestmentTypeScrollDown.Create(749, 415, m_Image_Scroll_All_SBD->Header.Xsize, m_Image_Scroll_All_SBD->Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	// 자신이 최고 투자자 이면 마을 안내문 버튼을 Enable 한다.
	m_Button_Main_VillageDescript.SetAction(FALSE);
	if( (strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0) ||
		pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5)
	{
		m_Button_Main_VillageDescript.SetAction(TRUE);
	}

	// Investment Info
	for(TempCount = 0; TempCount < MAX_INVESTMENT_TYPE; TempCount++)
	{
		m_InvestmentData[TempCount].m_MyInvestment = -1;
	}

	// Initialize
	m_NowWork         = GOVERNMENT_NOWWORK_INVEST_START;
	m_NextWork        = GOVERNMENT_NOWWORK_INVEST_START;

	m_NowScroll            = 0;
	m_NowSelectInvestment  = -1;
	m_StartTickCount       = 0;
	m_TempPos              = 0;
	m_InvestmentAfterMoney = 0;

	siKind	          = ON_VIL_ST_KIND_GOVERNMENT;
	
	pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Main_Enter();

	m_VillageInfo_RectHRGN    = CreateRectRgn(396, 106, 747, 199);
	m_VillageInfo_RectBigHRGN = CreateRectRgn(0, 0, 799, 599);

	// 현재 마을 시설을 검사한다.
	sprintf((char*)m_VillageInfo_StructList, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_VILINSTITUTIOON));
	pMyOnlineWorld->pOnlineVillage->MakeVillageInStructor((char*)&m_VillageInfo_StructList[strlen((char*)m_VillageInfo_StructList)]);

	m_Guild_NowState = 0;
	m_Guild_NowMessageState = 0;

	m_GuildJoinWaitPlayerListCount = 0;
	m_GuildJoinWaitPlayerList      = 0;
	m_GuildJoinWaitPlayerList_PageMaxCount = 0;
	m_GuildJoinWaitPlayerList_PageNowCount = 0;

	m_GuildGuildPlayerListCount = 0;
	m_GuildGuildPlayerList      = 0;
	m_GuildGuildPlayerList_PageMaxCount = 0;
	m_GuildGuildPlayerList_PageNowCount = 0;

	m_GuildDiffGuildListCount = 0;
	m_GuildDiffGuildList      = 0;

	m_GuildVillageListCount			= 0;
	m_GuildVillageList_PageMaxCount	= 0;
	m_GuildVillageList_PageNowCount	= 1;

	m_GuildFlag_MaxCount = 0;
	m_GuildFlag_Index    = 0;
	m_RectHRGN    = CreateRectRgn(527, 110, 743,196 );
	m_RectBigHRGN = CreateRectRgn(0, 0, 799, 599);

	// 마을안내문 영역설정
	SetRect(&m_rtDescriptRect, 280, 205, 750, 420);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해제.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID OnlineVillageStructureGovernment::Free()
{
	// Base
	m_NowWork   = 0;
	m_NextWork  = 0;
	m_fWaitFlag = 0;
	m_TempPos   = 0;

	m_YetEditHWND         = NULL;
	m_YetEditHWNDEnable   = 0;
	m_YetEditLockShortKey = 0;

	// Image
	if(m_Image_MainInterface_Face.Image)        {clGrp.FreeXspr(m_Image_MainInterface_Face);		ZeroMemory(&m_Image_MainInterface_Face,        sizeof(XSPR));}
	
	if(m_Image_InvestmentInterface_Back.Image)                   {clGrp.FreeXspr(m_Image_InvestmentInterface_Back);	ZeroMemory(&m_Image_InvestmentInterface_Back,    sizeof(XSPR));}
	if(m_Image_DescriptInterface_Back.Image)                     {clGrp.FreeXspr(m_Image_DescriptInterface_Back);		              ZeroMemory(&m_Image_DescriptInterface_Back,                     sizeof(XSPR));}
	if(m_Image_GuildInterface_Back.Image)                        {clGrp.FreeXspr(m_Image_GuildInterface_Back);			              ZeroMemory(&m_Image_GuildInterface_Back,                        sizeof(XSPR));}
	if(m_Image_VillageInfoInterface_Back.Image)                  {clGrp.FreeXspr(m_Image_VillageInfoInterface_Back);	              ZeroMemory(&m_Image_VillageInfoInterface_Back,                  sizeof(XSPR));}
	if(m_Image_VillageInfoDefenceInvestmentInterface_Back.Image) {clGrp.FreeXspr(m_Image_VillageInfoDefenceInvestmentInterface_Back); ZeroMemory(&m_Image_VillageInfoDefenceInvestmentInterface_Back, sizeof(XSPR));}

	if(m_Image_Button_Guild_DB.Image)           {clGrp.FreeXspr(m_Image_Button_Guild_DB);			ZeroMemory(&m_Image_Button_Guild_DB,           sizeof(XSPR));}
	if(m_Image_Button_All_MM.Image)             {clGrp.FreeXspr(m_Image_Button_All_MM);				ZeroMemory(&m_Image_Button_All_MM,             sizeof(XSPR));}
	if(m_Image_Button_All_EM.Image)             {clGrp.FreeXspr(m_Image_Button_All_EM);				ZeroMemory(&m_Image_Button_All_EM,             sizeof(XSPR));}
	if(m_Image_Button_All_B.Image)              {clGrp.FreeXspr(m_Image_Button_All_B);				ZeroMemory(&m_Image_Button_All_B,              sizeof(XSPR));}
	if(m_Image_Scroll_All_M.Image)              {clGrp.FreeXspr(m_Image_Scroll_All_M);				ZeroMemory(&m_Image_Scroll_All_M,              sizeof(XSPR));}

	m_Image_Scroll_All_SBBU = NULL;
	m_Image_Scroll_All_SBBD = NULL;

	// Investment Data
	ZeroMemory(m_InvestmentData, sizeof(m_InvestmentData));
	m_NowSelectInvestment       = 0;
	m_NowSettingInvestmentMoney = 0;
	if(m_stCapital)
	{
		delete[] m_stCapital;
		m_stCapital      = NULL;
	}
	m_stCapitalCount       = 0;
	m_stCapitalSelect      = 0;
	m_stCapitalScroll      = 0;
	m_InvestmentAfterMoney = 0;

	// 안내문
	if(m_pDescriptEditBox)
	{
		m_pDescriptEditBox->Free();
		delete m_pDescriptEditBox;
		m_pDescriptEditBox = NULL;
	}

	ZeroMemory(m_VillageInfo_Descript, sizeof(m_VillageInfo_Descript));
	m_VillageInfo_DescriptScroll = 0;
	m_VillageInfo_DescriptLine   = 0;

	m_VillageInfo_InOutPlayerCount = 0;
	ZeroMemory(m_VillageInfo_StructList, sizeof(m_VillageInfo_StructList));
	if(m_VillageInfo_RectHRGN)
	{
		DeleteObject(m_VillageInfo_RectHRGN);
		m_VillageInfo_RectHRGN    = NULL;
	}
	if(m_VillageInfo_RectBigHRGN)
	{
		DeleteObject(m_VillageInfo_RectBigHRGN);
		m_VillageInfo_RectBigHRGN = NULL;
	}

	// Scroll
	m_StartTickCount          = 0;
	m_NowScrollInvestmentType = 0;
	m_NowScroll               = 0;

	m_Guild_NowState = 0;
	m_Guild_NowMessageState = 0;
	if(m_Guild_GuildName)
	{
		m_Guild_GuildName->Free();
		delete m_Guild_GuildName;
		m_Guild_GuildName = NULL;
	}
	m_GuildJoinWaitPlayerList_PageMaxCount = 0;
	m_GuildJoinWaitPlayerList_PageNowCount = 0;

	m_GuildJoinWaitPlayerListCount = 0;
	if(m_GuildJoinWaitPlayerList)
	{
		delete []m_GuildJoinWaitPlayerList;
		m_GuildJoinWaitPlayerList      = 0;
	}

	m_GuildGuildPlayerListCount = 0;
	if(m_GuildGuildPlayerList)
	{
		delete[] m_GuildGuildPlayerList;
		m_GuildGuildPlayerList      = 0;
	}
	m_GuildGuildPlayerList_PageMaxCount = 0;
	m_GuildGuildPlayerList_PageNowCount = 0;

	if(m_GuildVillageList)
	{
		delete[] m_GuildVillageList;
		m_GuildVillageList      = 0;
	}
	m_GuildVillageListCount				= 0;
	m_GuildVillageList_PageMaxCount		= 0;
	m_GuildVillageList_PageNowCount		= 1;

	m_GuildDiffGuildListCount = 0;
	if(m_GuildDiffGuildList)
	{
		delete[] m_GuildDiffGuildList;
		m_GuildDiffGuildList      = 0;
	}

	m_GuildFlag_MaxCount = 0;
	if(m_GuildFlag_Index)
	{
		delete[] m_GuildFlag_Index;
		m_GuildFlag_Index = 0;
	}
	if(m_RectHRGN)    DeleteObject(m_RectHRGN);
	if(m_RectBigHRGN) DeleteObject(m_RectBigHRGN);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	액션.	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	float				TempSize;
	SI32				x, y;
	SI32				TempCount;
	SI32				TempCount1;
	BOOL				fLiveMouse;
	BOOL				TempbMouseDown;
	SI16				TempRenderStartPage;
	SI16				TempRenderEndPage;
	SI16				TempRenderPrior;
	SI16				TempRenderNext;
	SI16				TempPageCount;
	SI16				TempPageData[12];
	SI16				TempRenderPosX;
	SI16				TempRenderPosY;

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);								// 마우스 좌표를 구한다.

	TempbMouseDown = bMouseDown;
	fLiveMouse     = TRUE;
	if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE)
	{
		TempbMouseDown = FALSE;
		fLiveMouse     = FALSE;
	}

	m_Button_Main_Investment.SetAction(FALSE);
	m_Button_Main_VillageDescript.SetAction(FALSE);
	m_Button_Main_Guild.SetAction(FALSE);
	m_Button_Main_VillageInfo.SetAction(FALSE);
	m_Button_Main_Exit.SetAction(FALSE);

	m_Button_VillageInfo_InvestmentDefence.SetAction(FALSE);

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	switch(m_NowWork)
	{
		case GOVERNMENT_NOWWORK_INVEST_START:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			break;

		case GOVERNMENT_NOWWORK_INVEST_STARTPOOL:
			Draw_Back(pSurface, GetTickCount() - m_StartTickCount);
			Draw_Investment(pSurface, GetTickCount() - m_StartTickCount);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);

			// 일정 시간이 지나면 Pool 로 이동
			if(m_StartTickCount < (GetTickCount() - 450))
			{
				m_NowWork        = GOVERNMENT_NOWWORK_INVEST_POOL;
				m_StartTickCount = GetTickCount();

				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Enter(0);
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetVillageProfitStatistics();
				m_fWaitFlag                 = GOVERNMENT_WAITFLAG_GETLIST;
				m_NowSelectInvestment       = 0;
				m_NowSettingInvestmentMoney = 0;

				m_NowScroll = 0;
				m_TempPos   = -600;

				if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA ||
					pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
						PushEffect(EFFECT_GOV00, 0, 0);
				else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
						PushEffect(EFFECT_GOV00T, 0, 0);
				else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
						PushEffect(EFFECT_GOV00J, 0, 0);

			}
			break;

		case GOVERNMENT_NOWWORK_INVEST_POOL:
			if(m_fWaitFlag == GOVERNMENT_WAITFLAG_INVESTMENTRESULT)
			{
				// 투자 결과를 기다리고 있으면
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			}
			else if(m_fWaitFlag == GOVERNMENT_WAITFLAG_GETLIST)
			{
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			}
			else
			{
				if(pMyOnlineWorld->pOnlineMsgBox->IsActive() || pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					m_Button_Main_VillageInfo.SetAction(FALSE);
					m_Button_Main_Guild.SetAction(FALSE);
					m_Button_Main_Exit.SetAction(FALSE);
					m_Button_Investment_SetInvestment.SetAction(FALSE);
					m_Button_Investment_InvestmentGetOut.SetAction(FALSE);
				}
				else
				{
					if((strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
						|| pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5) 
						m_Button_Main_VillageDescript.SetAction(TRUE);
					m_Button_Main_Guild.SetAction(TRUE);
					m_Button_Main_VillageInfo.SetAction(TRUE);
					m_Button_Main_Exit.SetAction(TRUE);

					// 각 Button에 대한 Help
					if(m_Button_Main_Investment.IsInMouse())
					{
						char			TempBufferText[256];
						sprintf((char*)TempBufferText, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENT), pMyOnlineWorld->pOnlineVillage->GetVillageName());
						pMyOnlineWorld->pOnlineHelp->SetText((char*)TempBufferText, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					}
					if(m_Button_Main_VillageDescript.IsInMouse())        pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEDESCRIPT),      (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					if(m_Button_Main_Guild.IsInMouse())                  pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_GUILD),                (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					if(m_Button_Main_VillageInfo.IsInMouse())		     pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEINFO),          (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					if(m_Button_Investment_SetInvestment.IsInMouse())    pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENTHELP),       (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					if(m_Button_Investment_InvestmentGetOut.IsInMouse())
					{
						char			TempBufferText[256];

						sprintf((char*)TempBufferText, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENTGETOUTHELP), m_InvestmentData[m_NowSelectInvestment].m_GetOutTex);
						pMyOnlineWorld->pOnlineHelp->SetText((char*)TempBufferText, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					}

					// 현재 List를 선택중이면
					// 다른 메뉴 처리
					if(m_Button_Main_VillageDescript.Process(TempbMouseDown))
					{
//						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();

						m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_POOL;
						m_StartTickCount = GetTickCount();

						// Edit Box 생성
						m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
						m_YetEditHWND         = GetFocus();
						m_YetEditHWNDEnable   = IsWindowEnabled(m_YetEditHWND);

						pMyOnlineWorld->SetShortKey(TRUE);
						if( m_pDescriptEditBox )
						{
							m_pDescriptEditBox->Free();
							delete m_pDescriptEditBox;
						}
						m_pDescriptEditBox = new OnlineEditControl;
						m_pDescriptEditBox->CreateEditBox(1000, 1000, 230, 100, pGame->Hwnd, pGame->hInst, FALSE, FALSE, VILLAGE_DESCRIPT_NOTE, FALSE, TRUE);
//						SendMessage(m_pDescriptEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)VILLAGE_DESCRIPT_NOTE, 0);
						m_pDescriptEditBox->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
						m_pDescriptEditBox->EnableWindow(TRUE);
						m_pDescriptEditBox->SetString("");
						m_pDescriptEditBox->SetFocus();
						m_pDescriptEditBox->SetKeyLock('\'', TRUE);
						m_pDescriptEditBox->SetKeyLock('\"', TRUE);
						m_pDescriptEditBox->SetKeyLock('\\', TRUE);
					}

					if(m_Button_Main_Guild.Process(TempbMouseDown))
					{
//						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();

						m_NowWork        = GOVERNMENT_NOWWORK_GUILD_POOL;
						m_StartTickCount = GetTickCount();
					
						m_Guild_NowState = 0;
						m_Guild_NowMessageState = 0;
					}

					if(m_Button_Main_VillageInfo.Process(TempbMouseDown))
					{
//						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();

						m_NowWork        = GOVERNMENT_NOWWORK_VILLLAGEINFO_POOL;
						m_StartTickCount = GetTickCount();

						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_VillageInfo();
					}

					// 나가기 처리
					if(m_Button_Main_Exit.Process(TempbMouseDown) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
					{
						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();

						m_NowWork        = GOVERNMENT_NOWWORK_INVEST_END;
					}

					// 현재 수익 통계
					if(m_Button_Investment_VillageProfitYearScrollUp.Process(TempbMouseDown))
					{
						m_stCapitalSelect--;
						m_stCapitalScroll = 0;
					}
					if(m_Button_Investment_VillageProfitYearScrollDown.Process(TempbMouseDown))
					{
						m_stCapitalSelect++;
						m_stCapitalScroll = 0;
					}
					if(m_Button_Investment_VillageProfitVYearScrollUp.Process(TempbMouseDown))
					{
						m_stCapitalScroll--;
						if(m_stCapitalScroll < 0)
						{
							m_stCapitalScroll = 0;
						}
						
					}
					if(m_Button_Investment_VillageProfitVYearScrollDown.Process(TempbMouseDown))
					{
						m_stCapitalScroll++;
						if(m_stCapitalScroll >= 3)
						{
							m_stCapitalScroll = 2;
						}
					}

					if(m_stCapitalCount > 0)
					{
						if(TempbMouseDown)
						{
							x = ptMouse.x;
							y = ptMouse.y;

							if((x >= 580) && (x < 607) && (y >= 244) && (y < 355))
							{								
								if(y < 244) y = 244;
								if(y > 343) y = 343;
								y -= 244;

								TempSize    = 49.0f;
								m_stCapitalScroll = (SI32)((float)y / TempSize);
							}
						}

						if(m_stCapitalSelect < 0)
						{
							m_stCapitalSelect = 0;
							m_stCapitalScroll = 0;
						}
						if(m_stCapitalSelect >= m_stCapitalCount)
						{
							m_stCapitalSelect = m_stCapitalCount - 1;
							m_stCapitalScroll = 0;
						}

						if(m_stCapitalScroll < 0)
						{
							m_stCapitalScroll = 0;
						}
						if(m_stCapitalScroll >= 3)
						{
							m_stCapitalScroll = 2;
						}
					}
					else
					{
						m_stCapitalSelect = 0;
						m_stCapitalScroll = 0;
					}

					// 투자 List 처리
					if(TempbMouseDown)
					{
						x = ptMouse.x;
						y = ptMouse.y;

						for(TempCount = 0; TempCount < MAX_INVESTMENT_TYPE; TempCount++)
						{
							if(m_NowSelectInvestment != TempCount)
							{
								if((x >= 374) && (x < 489) && (y >= (85 + (TempCount * 20))) && (y < (105 + (TempCount * 20))))
								{
									pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Enter(TempCount);
									m_fWaitFlag = GOVERNMENT_WAITFLAG_GETLIST;
									break;
								}
							}
						}
					}

					m_Button_Investment_SetInvestment.SetAction(TRUE);
					/*
					if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
					{
						// 만약 마을의 길드가 자신의 길드가 아니면
						if(pMyOnlineWorld->pMyData->GetGuildName())
						{
							if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) != 0)
							{
								m_Button_Investment_SetInvestment.SetAction(FALSE);
							}
							else if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
							{
								m_Button_Investment_SetInvestment.SetAction(FALSE);
							}
						}
						else
						{
							m_Button_Investment_SetInvestment.SetAction(FALSE);
						}
					}
					*/

					// 투자금 인출
					m_Button_Investment_InvestmentGetOut.SetAction(FALSE);
					if(m_InvestmentData[m_NowSelectInvestment].m_GetOutFlag)
					{
						if(m_InvestmentData[m_NowSelectInvestment].m_MyInvestment != -1)
						{
							m_Button_Investment_InvestmentGetOut.SetAction(TRUE);
						}
					}

					if(m_InvestmentData[m_NowSelectInvestment].m_fData)
					{
						// 투자자 List Scroll 처리
						if(m_NowScroll <  0)                                                       m_NowScroll = 0;
						if(m_NowScroll >= m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment) m_NowScroll = 0;
						if(TempbMouseDown)
						{
							if(m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment > 2)
							{
								x = ptMouse.x;
								y = ptMouse.y;

								if((x >= 740) && (x < 766) && (y >= 112) && (y < 192))
								{								
									if(y < 112) y = 112;
									if(y > 192) y = 192;
									y -= 112;

									TempSize    = (float)81 / (float)(m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment - 1);
									m_NowScroll = (SI32)((float)y / TempSize);
								}
							}
							else
							{
								m_NowScroll = 0;
							}
						}

						if(m_Button_Investment_InvestmentDataScrollUp.Process(TempbMouseDown))
						{
							m_NowScroll--;
							if(m_NowScroll < 0) m_NowScroll = 0;
						}

						if(m_Button_Investment_InvestmentDataScrollDown.Process(TempbMouseDown))
						{
							m_NowScroll++;
							if(m_NowScroll > (m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment - 2)) m_NowScroll = (m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment - 2);
							if(m_NowScroll < 0) m_NowScroll = 0;
						}

						// 투자 처리
						if(m_Button_Investment_SetInvestment.Process(TempbMouseDown))
						{
							MONEY		TempMaxMoney;			//내가 투자할수 있는 돈(최고)
							MONEY		TempMyMoney;			//현재 나의 돈
							//MONEY		TempBestMoney;			//최고투자자 금액
							int			investtime;				//현재시간
							SOnlineTime		Time;
							//현재시간 int형으로 변환
							Time = pMyOnlineWorld->pTimer->GetNowTime();
							investtime = MakeTimeInt(Time);

							// 최대 투자액 검사
							TempMaxMoney = pMyOnlineWorld->pMyData->GetMoney();
							TempMyMoney  = 0;
							if(((m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney * m_InvestmentData[m_NowSelectInvestment].m_MaxPercent) / 100) < TempMaxMoney)
							{
								TempMaxMoney = (m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney * m_InvestmentData[m_NowSelectInvestment].m_MaxPercent) / 100;
							}

							if(m_InvestmentData[m_NowSelectInvestment].m_MyInvestment == -1)
							{
								// 한달에 한번만 투자 할수있다 현재시간과 투자할수있는 시간 비교
								if(m_InvestmentData[m_NowSelectInvestment].m_MyInvestmentCanTime <= investtime)
								{
									m_NowWork   = GOVERNMENT_NOWWORK_INVEST_SETINVESTMENT;
									pMyOnlineWorld->pOnlineVillage->fFlip = FALSE;
									
									pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment(
										TempMaxMoney
										, ((m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney * m_InvestmentData[m_NowSelectInvestment].m_MinPercent) / 100)
										, m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney, 0);
									pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
									pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShowCenter();
								}
								else
								{
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENTONEMONTH));
								}
							}
							else
							{
								if(m_InvestmentData[m_NowSelectInvestment].m_MyInvestmentCanTime <= investtime)
								{
									m_NowWork   = GOVERNMENT_NOWWORK_INVEST_SETINVESTMENT;
									pMyOnlineWorld->pOnlineVillage->fFlip = FALSE;
									
									pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment(
										TempMaxMoney
										, 0
										, m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney, m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_InvestmentData[m_NowSelectInvestment].m_MyInvestment].m_InvestmentMoney);

									pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
									pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShowCenter();
								}
								else
								{
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENTONEMONTH));
								}
							}
							if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA ||
								pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
									PushEffect(EFFECT_GOV10, 0, 0);
							else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
									PushEffect(EFFECT_GOV10T, 0, 0);
							else if(pMyOnlineWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
									PushEffect(EFFECT_GOV10J, 0, 0);
							return FALSE;
						}

						// 투자금 인출
						if(m_Button_Investment_InvestmentGetOut.Process(TempbMouseDown))
						{
							m_NowWork = GOVERNMENT_NOWWORK_INVEST_INVESTMENTGETOUT;
							pMyOnlineWorld->pOnlineVillage->fFlip = FALSE;
							pMyOnlineWorld->SetShortKey(TRUE);

							m_InvestmentGetOutMoney = 0;

							m_pDescriptEditBox = new OnlineEditControl;
							m_pDescriptEditBox->CreateEditBox(1000, 1000, 150, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, VILLAGE_DESCRIPT_NOTE, TRUE);
//							SendMessage(m_pDescriptEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)VILLAGE_DESCRIPT_NOTE, 0);
							m_pDescriptEditBox->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
							m_pDescriptEditBox->EnableWindow(TRUE);
							m_pDescriptEditBox->SetString("");
							m_pDescriptEditBox->SetFocus();
							m_pDescriptEditBox->SetKeyLock('\'', TRUE);
							m_pDescriptEditBox->SetKeyLock('\"', TRUE);
							m_pDescriptEditBox->SetKeyLock('\\', TRUE);

							m_pDescriptEditBox->SetString("0");

							return FALSE;
						}
					}
				}
			}

			if(m_TempPos != 0)
			{
				if(((SI32)m_TempPos + (SI32)(GetTickCount() - m_StartTickCount)) >= 0)
				{
					m_TempPos = 0;
				}
			}

			Draw_Back(pSurface, 0);
			if(m_TempPos != 0) Draw_Investment(pSurface, (SI32)(GetTickCount() - m_StartTickCount) + (SI32)m_TempPos);
			else               Draw_Investment(pSurface, 0);

			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_INVEST_SETINVESTMENT:
			m_Button_Investment_SetInvestment.SetAction(FALSE);
			m_Button_Investment_InvestmentGetOut.SetAction(FALSE);

			if((pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess()) && (pMyOnlineWorld->pOnlineNumPad->siCount != 0))
			{
				BOOL		fSend;

				fSend = TRUE;
				if(m_InvestmentData[m_NowSelectInvestment].m_MyInvestment == -1)
				{
					// 5% 이하는 취급안함
					if((((m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney * m_InvestmentData[m_NowSelectInvestment].m_MinPercent) / 100) > pMyOnlineWorld->pOnlineNumPad->siCount) || (((m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney * m_InvestmentData[m_NowSelectInvestment].m_MaxPercent) / 100) < pMyOnlineWorld->pOnlineNumPad->siCount))
					{
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FIRSTINVESTMENT), m_InvestmentData[m_NowSelectInvestment].m_MinPercent, m_InvestmentData[m_NowSelectInvestment].m_MaxPercent);

						fSend = FALSE;
					}
				}
				if(pMyOnlineWorld->pMyData->GetMoney() < pMyOnlineWorld->pOnlineNumPad->siCount)
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WARP_MONEY));

					fSend = FALSE;
				}

				// Send Message
				if(fSend)
				{
					m_InvestmentAfterMoney = pMyOnlineWorld->pMyData->GetMoney() - pMyOnlineWorld->pOnlineNumPad->siCount;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_SetInvestment(m_NowSelectInvestment, pMyOnlineWorld->pOnlineNumPad->siCount);

					m_NowWork   = GOVERNMENT_NOWWORK_INVEST_POOL;
					m_fWaitFlag = GOVERNMENT_WAITFLAG_INVESTMENTRESULT;
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
				}
				else
				{
					m_NowWork   = GOVERNMENT_NOWWORK_INVEST_POOL;
					m_fWaitFlag = 0;
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
				}
			}
			else if((pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess()) && (pMyOnlineWorld->pOnlineNumPad->siCount == 0))
			{
				m_NowWork   = GOVERNMENT_NOWWORK_INVEST_POOL;
				m_fWaitFlag = 0;
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->InitializeGovernmentInvestment();
			}

			Draw_Back(pSurface, 0);
			Draw_Investment(pSurface, 0);
			pMyOnlineWorld->pOnlineNumPad->pNumberControl->Draw(pSurface);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_INVEST_INVESTMENTGETOUT:
			{
				char		TempBuffer[VILLAGE_DESCRIPT_NOTE + 1];
				MONEY		TempMax;

				m_pDescriptEditBox->GetString((char*)TempBuffer);
				if(TempBuffer[0] == NULL)
				{
					m_InvestmentGetOutMoney = 0;
					m_pDescriptEditBox->SetString("0");
				}
				else
				{
					sscanf((char*)TempBuffer, "%I64d", &m_InvestmentGetOutMoney);
				}

				TempMax = m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_InvestmentData[m_NowSelectInvestment].m_MyInvestment].m_InvestmentMoney;

				// 지정 가능한 숫자인지 검사
				if(m_InvestmentGetOutMoney > TempMax)
				{
					m_InvestmentGetOutMoney = TempMax;

					sprintf((char*)TempBuffer, "%I64d", m_InvestmentGetOutMoney);
					m_pDescriptEditBox->SetString((char*)TempBuffer);
				}

				// OK Check
				m_Button_Investment_InvestmentGetOut_OK.SetAction(FALSE);
				if(m_InvestmentGetOutMoney >= 10)
				{
					m_Button_Investment_InvestmentGetOut_OK.SetAction(TRUE);
				}

				if(m_Button_Investment_InvestmentGetOut_OK.Process(TempbMouseDown))
				{
					if(m_InvestmentGetOutMoney >= 10)
					{
						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_InvestmentGetOut(m_InvestmentGetOutMoney);

						pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
						SetFocus(m_YetEditHWND);
						EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

						m_NowWork = GOVERNMENT_NOWWORK_INVEST_INVESTMENTGETOUT_RESULTWAIT;
						pMyOnlineWorld->pOnlineVillage->fFlip = FALSE;

						delete m_pDescriptEditBox;
						m_pDescriptEditBox = NULL;
						pMyOnlineWorld->SetShortKey(FALSE);
						return FALSE;
					}
				}

				// Cancel Check
				if(m_Button_Investment_InvestmentGetOut_Cancel.Process(TempbMouseDown))
				{
					m_NowWork                             = GOVERNMENT_NOWWORK_INVEST_POOL;
					pMyOnlineWorld->pOnlineVillage->fFlip = FALSE;

					delete m_pDescriptEditBox;
					m_pDescriptEditBox = NULL;
					pMyOnlineWorld->SetShortKey(FALSE);

					return FALSE;
				}
			}

			Draw_Back(pSurface, 0);
			Draw_Investment(pSurface, 0);
			Draw_InvestmentGetOut(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_INVEST_INVESTMENTGETOUT_RESULTWAIT:
			{
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
				
				Draw_Back(pSurface, 0);
				Draw_Investment(pSurface, 0);
				Draw_InvestmentGetOut(pSurface, 0);
				pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			}
			break;

		case GOVERNMENT_NOWWORK_INVEST_END:
			{
				// Server에서 확인 Message검사
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
				
				Draw_Back(pSurface, 0);
				Draw_Investment(pSurface, 0);
				pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			}
			break;

		case GOVERNMENT_NOWWORK_INVEST_ENDPOOL:
			Draw_Back(pSurface, GetTickCount() - m_StartTickCount);
			Draw_Investment(pSurface, GetTickCount() - m_StartTickCount);

			// 일정 시간이 지나면 Pool 로 이동
			if(m_StartTickCount < (GetTickCount() - 450))
			{
				Free();
				return TRUE;
			}
			break;

		case GOVERNMENT_NOWWORK_DESCRIPT_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive())
			{
				m_Button_Main_Investment.SetAction(FALSE);
				m_Button_Main_Guild.SetAction(FALSE);
				m_Button_Main_VillageInfo.SetAction(FALSE);
				m_Button_Main_Exit.SetAction(FALSE);
				m_Button_Descropt_SetDescropt.SetAction(FALSE);
			}
			else
			{
				m_Button_Main_Investment.SetAction(TRUE);
				m_Button_Main_Guild.SetAction(TRUE);
				m_Button_Main_VillageInfo.SetAction(TRUE);
				m_Button_Main_Exit.SetAction(TRUE);

				if(m_pDescriptEditBox)
				{
					if(!pMyOnlineWorld->GetShortKey())
						pMyOnlineWorld->SetShortKey(TRUE);
					m_Button_Descropt_SetDescropt.SetAction(TRUE);
				}

				// 각 Button에 대한 Help
				if(m_Button_Main_Investment.IsInMouse())
				{
					char			TempBufferText[256];
					sprintf((char*)TempBufferText, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENT), pMyOnlineWorld->pOnlineVillage->GetVillageName());
					pMyOnlineWorld->pOnlineHelp->SetText((char*)TempBufferText, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				}
				if(m_Button_Main_VillageDescript.IsInMouse()) pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEDESCRIPT), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				if(m_Button_Main_Guild.IsInMouse())           pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_GUILD),           (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				if(m_Button_Main_VillageInfo.IsInMouse())     pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEINFO),     (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);

				// 다른 메뉴 처리
				if(m_Button_Main_Investment.Process(TempbMouseDown))
				{
					if(m_pDescriptEditBox)
					{
						m_pDescriptEditBox->Free();
						delete m_pDescriptEditBox;
						m_pDescriptEditBox = NULL;
					}

					// Focus 지정
					pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
					SetFocus(m_YetEditHWND);
					EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Enter(0);
					m_fWaitFlag                 = GOVERNMENT_WAITFLAG_GETLIST;
					m_NowSelectInvestment       = 0;
					m_NowSettingInvestmentMoney = 0;

					m_NowScroll = 0;

					m_NowWork        = GOVERNMENT_NOWWORK_INVEST_POOL;
					m_StartTickCount = GetTickCount();
				}

				if(m_Button_Main_Guild.Process(TempbMouseDown))
				{
					if(m_pDescriptEditBox)
					{
						m_pDescriptEditBox->Free();
						delete m_pDescriptEditBox;
						m_pDescriptEditBox = NULL;
					}

					// Focus 지정
					pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
					SetFocus(m_YetEditHWND);
					EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

					m_NowWork        = GOVERNMENT_NOWWORK_GUILD_POOL;
					m_StartTickCount = GetTickCount();
					
					m_Guild_NowState = 0;
					m_Guild_NowMessageState = 0;
				}

				if(m_Button_Main_VillageInfo.Process(TempbMouseDown))
				{
					if(m_pDescriptEditBox)
					{
						m_pDescriptEditBox->Free();
						delete m_pDescriptEditBox;
						m_pDescriptEditBox = NULL;
					}

					// Focus 지정
					pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
					SetFocus(m_YetEditHWND);
					EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

					m_NowWork        = GOVERNMENT_NOWWORK_VILLLAGEINFO_POOL;
					m_StartTickCount = GetTickCount();

					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_VillageInfo();
				}

				// 나가기 처리
				if(m_Button_Main_Exit.Process(TempbMouseDown) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					if(m_pDescriptEditBox)
					{
						m_pDescriptEditBox->Free();
						delete m_pDescriptEditBox;
						m_pDescriptEditBox = NULL;
					}

					// Focus 지정
					pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
					SetFocus(m_YetEditHWND);
					EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();

					m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_END;
				}

				if((PtInRect( &m_rtDescriptRect, ptMouse ) == TRUE) && TempbMouseDown)
				{
					if(!m_pDescriptEditBox->IsFocus())
						m_pDescriptEditBox->SetFocus();
				}

				// 마을안내문을 저장한다.
				if(m_pDescriptEditBox)
				{
					ZeroMemory((char*)m_VillageDescript_Descript, sizeof(m_VillageDescript_Descript));
					m_pDescriptEditBox->GetString((char*)m_VillageDescript_Descript);	
				}

				if(m_Button_Descropt_SetDescropt.Process(TempbMouseDown))
				{
					if(CheckLeadByte(m_VillageDescript_Descript))
					{
						m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_RESULTWAIT;
						
						pMyOnlineWorld->pOnlineClient->SendRequestVillageDescript_Write((char*)m_VillageDescript_Descript);
						m_pDescriptEditBox->SetString("");
						m_pDescriptEditBox->EnableWindow(FALSE);
					}
					else
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000526));
				}
			}

			Draw_Back(pSurface, 0);
			Draw_Descript(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_DESCRIPT_RESULTWAIT:
			{
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
				
				Draw_Back(pSurface, 0);
				Draw_Descript(pSurface, 0);
				pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			}
			break;

		case GOVERNMENT_NOWWORK_DESCRIPT_END:
			Draw_Back(pSurface, 0);
			Draw_Descript(pSurface, 0);

			m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_ENDPOOL;
			m_StartTickCount = GetTickCount();
			break;

		case GOVERNMENT_NOWWORK_DESCRIPT_ENDPOOL:
			Draw_Back(pSurface, GetTickCount() - m_StartTickCount);
			Draw_Descript(pSurface, GetTickCount() - m_StartTickCount);

			// 일정 시간이 지나면 Pool 로 이동
			if(m_StartTickCount < (GetTickCount() - 450))
			{
				Free();
				return TRUE;
			}
			break;

		case GOVERNMENT_NOWWORK_VILLLAGEINFO_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() || pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
			{
				m_Button_Main_Investment.SetAction(FALSE);
				m_Button_Main_VillageDescript.SetAction(FALSE);
				m_Button_Main_Guild.SetAction(FALSE);
				m_Button_Main_Exit.SetAction(FALSE);
				m_Button_VillageInfo_InvestmentDefence.SetAction(FALSE);
			}
			else
			{
				m_Button_Main_Investment.SetAction(TRUE);
				if(strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0) 
					m_Button_Main_VillageDescript.SetAction(TRUE);
				m_Button_Main_Guild.SetAction(TRUE);
				m_Button_Main_Exit.SetAction(TRUE);
				
				// 각 Button에 대한 Help
				if(m_Button_Main_Investment.IsInMouse())
				{
					char			TempBufferText[256];
					sprintf((char*)TempBufferText, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENT), pMyOnlineWorld->pOnlineVillage->GetVillageName());
					pMyOnlineWorld->pOnlineHelp->SetText((char*)TempBufferText, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				}
				if(m_Button_Main_VillageDescript.IsInMouse()) pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEDESCRIPT), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				if(m_Button_Main_Guild.IsInMouse())           pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_GUILD),           (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				if(m_Button_Main_VillageInfo.IsInMouse())     pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEINFO),     (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				
				// 다른 메뉴 처리
				if(m_Button_Main_Investment.Process(TempbMouseDown))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Enter(0);
					m_fWaitFlag                 = GOVERNMENT_WAITFLAG_GETLIST;
					m_NowSelectInvestment       = 0;
					m_NowSettingInvestmentMoney = 0;
					
					m_NowScroll = 0;
					
					m_NowWork        = GOVERNMENT_NOWWORK_INVEST_POOL;
					m_StartTickCount = GetTickCount();
				}
				
				if(m_Button_Main_Guild.Process(TempbMouseDown))
				{
					m_NowWork        = GOVERNMENT_NOWWORK_GUILD_POOL;
					m_StartTickCount = GetTickCount();
					
					m_Guild_NowState = 0;
					m_Guild_NowMessageState = 0;
				}
				
				if(m_Button_Main_VillageDescript.Process(TempbMouseDown))
				{
					m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_POOL;
					m_StartTickCount = GetTickCount();
					
					// Edit Box 생성
					m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
					m_YetEditHWND         = GetFocus();
					m_YetEditHWNDEnable   = IsWindowEnabled(m_YetEditHWND);
					
					pMyOnlineWorld->SetShortKey(TRUE);
					if( m_pDescriptEditBox )
					{
						m_pDescriptEditBox->Free();
						delete m_pDescriptEditBox;
					}
					m_pDescriptEditBox = new OnlineEditControl;
					m_pDescriptEditBox->CreateEditBox(1000, 1000, 230, 100, pGame->Hwnd, pGame->hInst, FALSE, FALSE, VILLAGE_DESCRIPT_NOTE, FALSE, TRUE);
//					SendMessage(m_pDescriptEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)VILLAGE_DESCRIPT_NOTE, 0);
					m_pDescriptEditBox->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					m_pDescriptEditBox->EnableWindow(TRUE);
					m_pDescriptEditBox->SetString("");
					m_pDescriptEditBox->SetFocus();
					m_pDescriptEditBox->SetKeyLock('\'', TRUE);
					m_pDescriptEditBox->SetKeyLock('\"', TRUE);
					m_pDescriptEditBox->SetKeyLock('\\', TRUE);
				}
				
				// Village Descript Scroll
				if(m_VillageInfo_DescriptLine > 6)
				{
					if(TempbMouseDown)
					{
						x = ptMouse.x;
						y = ptMouse.y;

						if((x >= 740) && (x < 766) && (y >= 112) && (y < 192))
						{								
							if(y < 112) y = 112;
							if(y > 192) y = 192;
							y -= 112;

							TempSize                     = (float)75 / (float)((m_VillageInfo_DescriptLine - 6) - 1);
							m_VillageInfo_DescriptScroll = (SI32)((float)y / TempSize);
						}
					}

					if(m_Button_VillageInfo_DescriptScrollUp.Process(TempbMouseDown))
					{
						m_VillageInfo_DescriptScroll--;
					}
					if(m_Button_VillageInfo_DescriptScrollDown.Process(TempbMouseDown))
					{
						if(m_VillageInfo_DescriptLine > 6)
						{
							m_VillageInfo_DescriptScroll++;
						}
					}
					
					if(m_VillageInfo_DescriptScroll < 0) m_VillageInfo_DescriptScroll = 0;
					if(m_VillageInfo_DescriptScroll >= (m_VillageInfo_DescriptLine - 6)) m_VillageInfo_DescriptScroll = (m_VillageInfo_DescriptLine - 6) - 1;
				}
				else
				{
					m_VillageInfo_DescriptScroll = 0;
				}

				// 마을 최고투자자 이거나 직급이 대행수이면 마을방어를 할수있다.
				if(strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
				{
					m_Button_VillageInfo_InvestmentDefence.SetAction(TRUE);
				}

				// 같은 길드의 대행수라면 방어력 투자가능
				if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
				{
					if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
					{
						if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0)
						{
							m_Button_VillageInfo_InvestmentDefence.SetAction(TRUE);
						}
					}
				}

				//if(m_VillageInfo_NowDefence >= m_VillageInfo_MaxDefence) m_Button_VillageInfo_InvestmentDefence.SetAction(FALSE);

				if(m_Button_VillageInfo_InvestmentDefence.Process(TempbMouseDown))
				{
					// 마을 투자로
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetMaxVillageDefenceInvestment();
					m_NowWork        = GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_WAIT;
				}
				
				// 나가기 처리
				if(m_Button_Main_Exit.Process(TempbMouseDown) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();
					
					m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_END;
				}
			}

			Draw_Back(pSurface, 0);
			Draw_VillageInfo(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_WAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_Back(pSurface, 0);
			Draw_VillageInfo(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_POOL:
			{
				char TempBuffer[VILLAGE_DESCRIPT_NOTE+1];

				// 지정 가능한 숫자인지 검사
				m_VillageInfo_UpDefence = 0;

				ZeroMemory(TempBuffer, sizeof(TempBuffer));
				m_pDescriptEditBox->GetString((char*)TempBuffer);
				sscanf((char*)TempBuffer, "%I64d", &m_VillageInfo_UpDefence);

				if(m_VillageInfo_UpDefence < 0)
				{
					m_VillageInfo_UpDefence = 0;

					sprintf((char*)TempBuffer, "%I64d", m_VillageInfo_UpDefence);
					m_pDescriptEditBox->SetString((char*)TempBuffer);
				}
				if((m_VillageInfo_UpDefence + m_VillageInfo_NowDefence) > m_VillageInfo_MaxDefence)
				{
					m_VillageInfo_UpDefence = m_VillageInfo_MaxDefence - m_VillageInfo_NowDefence;

					sprintf((char*)TempBuffer, "%I64d", m_VillageInfo_UpDefence);
					m_pDescriptEditBox->SetString((char*)TempBuffer);
				}
				if((m_VillageInfo_UpDefence * m_VillageInfo_DefencePerMoney) > m_VillageInfo_MaxMoney)
				{
					m_VillageInfo_UpDefence = (m_VillageInfo_MaxMoney / m_VillageInfo_DefencePerMoney);

					sprintf((char*)TempBuffer, "%I64d", m_VillageInfo_UpDefence);
					m_pDescriptEditBox->SetString((char*)TempBuffer);
				}

				// OK Check
				if(m_Button_VillageInfo_InvestmentDefenceOK.Process(TempbMouseDown))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_SetVillageDefenceInvestment(m_VillageInfo_UpDefence * m_VillageInfo_DefencePerMoney);

					pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
					SetFocus(m_YetEditHWND);
					EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

					m_NowWork = GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_RESULTWAIT;
				}

				// Cancel Check
				if(m_Button_VillageInfo_InvestmentDefenceCancel.Process(TempbMouseDown))
				{
					pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
					SetFocus(m_YetEditHWND);
					EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

					m_NowWork = GOVERNMENT_NOWWORK_VILLLAGEINFO_POOL;
				}
			}

			Draw_Back(pSurface, 0);
			Draw_VillageInfo(pSurface, 0);
			Draw_VillageInfo_SetDefenceInvestment(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_VILLLAGEINFO_SETDEFENCE_RESULTWAIT:
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

			Draw_Back(pSurface, 0);
			Draw_VillageInfo(pSurface, 0);
			Draw_VillageInfo_SetDefenceInvestment(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_VILLLAGEINFO_END:
			Draw_Back(pSurface, 0);
			Draw_VillageInfo(pSurface, 0);

			m_NowWork        = GOVERNMENT_NOWWORK_VILLLAGEINFO_ENDPOOL;
			m_StartTickCount = GetTickCount();
			break;

		case GOVERNMENT_NOWWORK_VILLLAGEINFO_ENDPOOL:
			Draw_Back(pSurface, GetTickCount() - m_StartTickCount);
			Draw_VillageInfo(pSurface, GetTickCount() - m_StartTickCount);

			// 일정 시간이 지나면 Pool 로 이동
			if(m_StartTickCount < (GetTickCount() - 450))
			{
				Free();
				return TRUE;
			}
			break;

		case GOVERNMENT_NOWWORK_GUILD_POOL:
			if(pMyOnlineWorld->pOnlineMsgBox->IsActive() || pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
			{
				m_Button_Main_Investment.SetAction(FALSE);
				m_Button_Main_VillageDescript.SetAction(FALSE);
				m_Button_Main_VillageInfo.SetAction(FALSE);
				m_Button_Main_Exit.SetAction(FALSE);

				m_Button_Guild_MakeGuild.SetAction(FALSE);
				m_Button_Guild_DeleteGuild.SetAction(FALSE);
				m_Button_Guild_DiffGuild.SetAction(FALSE);
				m_Button_Guild_FlagGuild.SetAction(FALSE);
				m_Button_Guild_JoinGuild.SetAction(FALSE);
				m_Button_Guild_DismissalGuild.SetAction(FALSE);
				m_Button_Guild_PlayerJoinDismissalGuild.SetAction(FALSE);
			}
			else
			{
				if(m_Guild_NowMessageState == 0)
				{
					m_Button_Main_Investment.SetAction(TRUE);
					if(strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0) 
						m_Button_Main_VillageDescript.SetAction(TRUE);
					m_Button_Main_VillageInfo.SetAction(TRUE);
					m_Button_Main_Exit.SetAction(TRUE);

					// 각 Button에 대한 Help
					if(m_Button_Main_Investment.IsInMouse())
					{
						char			TempBufferText[256];
						sprintf((char*)TempBufferText, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENT), pMyOnlineWorld->pOnlineVillage->GetVillageName());
						pMyOnlineWorld->pOnlineHelp->SetText((char*)TempBufferText, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					}
					if(m_Button_Main_VillageDescript.IsInMouse()) pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEDESCRIPT), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					if(m_Button_Main_Guild.IsInMouse())           pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_GUILD),           (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
					if(m_Button_Main_VillageInfo.IsInMouse())     pMyOnlineWorld->pOnlineHelp->SetText(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_VILLAGEINFO),     (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE);
				}

				// 다른 메뉴 처리
				if(m_Button_Main_Investment.Process(TempbMouseDown))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Enter(0);
					m_fWaitFlag                 = GOVERNMENT_WAITFLAG_GETLIST;
					m_NowSelectInvestment       = 0;
					m_NowSettingInvestmentMoney = 0;

					m_NowScroll = 0;

					m_NowWork        = GOVERNMENT_NOWWORK_INVEST_POOL;
					m_StartTickCount = GetTickCount();
				}

				if(m_Button_Main_VillageInfo.Process(TempbMouseDown))
				{
					m_NowWork        = GOVERNMENT_NOWWORK_VILLLAGEINFO_POOL;
					m_StartTickCount = GetTickCount();

					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_VillageInfo();
				}

				if(m_Button_Main_VillageDescript.Process(TempbMouseDown))
				{
					m_NowWork        = GOVERNMENT_NOWWORK_DESCRIPT_POOL;
					m_StartTickCount = GetTickCount();

					// Edit Box 생성
					m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
					m_YetEditHWND         = GetFocus();
					m_YetEditHWNDEnable   = IsWindowEnabled(m_YetEditHWND);

					pMyOnlineWorld->SetShortKey(TRUE);
					if( m_pDescriptEditBox )
					{
						m_pDescriptEditBox->Free();
						delete m_pDescriptEditBox;
					}
					m_pDescriptEditBox = new OnlineEditControl;
					m_pDescriptEditBox->CreateEditBox(1000, 1000, 230, 100, pGame->Hwnd, pGame->hInst, FALSE, FALSE, VILLAGE_DESCRIPT_NOTE, FALSE, TRUE);
//					SendMessage(m_pDescriptEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)VILLAGE_DESCRIPT_NOTE, 0);
					m_pDescriptEditBox->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					m_pDescriptEditBox->EnableWindow(TRUE);
					m_pDescriptEditBox->SetString("");
					m_pDescriptEditBox->SetFocus();
					m_pDescriptEditBox->SetKeyLock('\'', TRUE);
					m_pDescriptEditBox->SetKeyLock('\"', TRUE);
					m_pDescriptEditBox->SetKeyLock('\\', TRUE);
				}

				// 나가기 처리
				if(m_Button_Main_Exit.Process(TempbMouseDown) || LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Invenstment_Leave();

					m_NowWork        = GOVERNMENT_NOWWORK_GUILD_END;
				}

				m_Button_Guild_MakeGuild.SetAction(FALSE);
				m_Button_Guild_DeleteGuild.SetAction(FALSE);
				m_Button_Guild_DiffGuild.SetAction(FALSE);
				m_Button_Guild_FlagGuild.SetAction(FALSE);
				m_Button_Guild_JoinGuild.SetAction(FALSE);
				m_Button_Guild_DismissalGuild.SetAction(FALSE);
				m_Button_Guild_PlayerJoinDismissalGuild.SetAction(FALSE);

				m_Button_Guild_OK.SetAction(FALSE);
				m_Button_Guild_Cancel.SetAction(FALSE);
				m_Button_Guild_JoinOK.SetAction(FALSE);
				m_Button_Guild_JoinCancel.SetAction(FALSE);
				m_Button_Guild_JoinExit.SetAction(FALSE);
				m_Button_Guild_DismissalOK.SetAction(FALSE);
				m_Button_Guild_VillageList.SetAction(FALSE);
				m_Button_Guild_InvestmentTypeScrollUp.SetAction(FALSE);
				m_Button_Guild_InvestmentTypeScrollDown.SetAction(FALSE);

				switch(m_Guild_NowState)
				{
					case ON_GOVERNMENT_GUILD_INIT:			// Data를 기다리는중
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));

						m_Guild_NowState = ON_GOVERNMENT_GUILD_POLL;
						m_Guild_NowMessageState = 0;
						break;

					case ON_GOVERNMENT_GUILD_POLL:			// None
						m_Button_Guild_DiffGuild.SetAction(TRUE);
						if(pMyOnlineWorld->pMyData->GetGuildName())
						{
							// 현재 길드에 가입돼어 있을때
							if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
							{
								// 현재 마을에 길드가 있을때
								if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0)
								{
									// 이 마을의 길드가 자신이 속한 길드일때
									if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
									{
										m_Button_Guild_JoinGuild.SetAction(TRUE);
										m_Button_Guild_DeleteGuild.SetAction(TRUE);
										m_Button_Guild_FlagGuild.SetAction(TRUE);
										m_Button_Guild_DismissalGuild.SetAction(TRUE);
									}
									if(pMyOnlineWorld->pMyData->GetGuildClass() != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
									{
										//m_Button_Guild_DismissalGuild.SetAction(TRUE);
									}
									m_Button_Guild_PlayerJoinDismissalGuild.SetAction(TRUE);
								}
								else
								{
									// 이 마을의 길드는 자신이 속한 길드가 아닐때(모든 버튼 무시)
								}
							}
							else
							{								
								// 현재 마을에 길드가 없을때(모든 버튼 무시)
							}
						}
						else
						{
							// 현재 길드에 미가입돼어 있을때
							if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
							{
								// 현재 마을에 길드가 있을때
								m_Button_Guild_PlayerJoinDismissalGuild.SetAction(TRUE);
							}
							else
							{								
								// 현재 마을에 길드가 없을때
								if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.m_MaxInvestmentID, pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
								{
									m_Button_Guild_MakeGuild.SetAction(TRUE);
								}
							}
						}

						if(m_Button_Guild_MakeGuild.Process(TempbMouseDown))
						{
							m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
							m_YetEditHWND         = GetFocus();
							m_YetEditHWNDEnable   = IsWindowEnabled(m_YetEditHWND);

							pMyOnlineWorld->SetShortKey(TRUE);
							if( m_Guild_GuildName )
							{
								m_Guild_GuildName->Free();
								delete m_Guild_GuildName;
							}
							m_Guild_GuildName = new OnlineEditControl;
							m_Guild_GuildName->CreateEditBox(1000, 1000, 230, 100, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_GUILDNAME_LENGTH, FALSE, TRUE);
//							SendMessage(m_Guild_GuildName->GethWnd(), EM_LIMITTEXT, (WPARAM)ON_GUILDNAME_LENGTH, 0);
							m_Guild_GuildName->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
							m_Guild_GuildName->EnableWindow(TRUE);
							m_Guild_GuildName->SetString("");
							m_Guild_GuildName->SetFocus();
							m_Guild_GuildName->SetKeyLock('\'', TRUE);
							m_Guild_GuildName->SetKeyLock('\"', TRUE);
							m_Guild_GuildName->SetKeyLock(0x0D, TRUE);
							m_Guild_GuildName->SetKeyLock(0x0A, TRUE);
							m_Guild_GuildName->SetKeyLock('\t', TRUE);
							m_Guild_GuildName->SetKeyLock('\\', TRUE);
							m_Guild_GuildName->SetKeyLock('%', TRUE);

							// 상단개설 비용을 요청한다.
							pMyOnlineWorld->pOnlineClient->SendRequestGuildCreateCost();

							m_Guild_NowState = ON_GOVERNMENT_GUILD_OPEN;
							m_Guild_NowMessageState = 0;
						}

						if(m_Button_Guild_DeleteGuild.Process(TempbMouseDown))
						{
							m_Guild_NowState = ON_GOVERNMENT_GUILD_DELETE;
							m_Guild_NowMessageState = 0;
						}

						if(m_Button_Guild_DiffGuild.Process(TempbMouseDown))
						{
							pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDiffGuild();

							m_Guild_NowState = ON_GOVERNMENT_GUILD_LIST;
							m_Guild_NowMessageState = 1;
							m_GuildScrollBar = 0;
							m_GuildScrollSelect = -1;
						}

						if(m_Button_Guild_FlagGuild.Process(TempbMouseDown))
						{
							m_Guild_NowState        = ON_GOVERNMENT_GUILD_SELECT_FLAG;
							m_Guild_NowMessageState = 1;
							m_GuildScrollBar        = 0;
							m_GuildScrollSelect     = -1;
						}

						if(m_Button_Guild_JoinGuild.Process(TempbMouseDown))
						{
							pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(1);

							m_Guild_NowState = ON_GOVERNMENT_GUILD_JOIN_LIST;
							m_Guild_NowMessageState = 1;
							m_GuildScrollBar = 0;
							m_GuildScrollSelect = -1;
						}

						if(m_Button_Guild_DismissalGuild.Process(TempbMouseDown))
						{
							pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(1, 0);

							m_Guild_NowState = ON_GOVERNMENT_GUILD_DISSMISSAL_LIST;
							m_Guild_NowMessageState = 1;
							m_GuildScrollBar = 0;
							m_GuildScrollSelect = -1;
						}

						if(m_Button_Guild_PlayerJoinDismissalGuild.Process(TempbMouseDown))
						{
							if(pMyOnlineWorld->pMyData->GetGuildName()) m_Guild_NowState = ON_GOVERNMENT_GUILD_LEAVE;
							else                                        m_Guild_NowState = ON_GOVERNMENT_GUILD_JOIN;
							m_Guild_NowMessageState = 0;
						}
						break;

					case ON_GOVERNMENT_GUILD_OPEN:			// 상단 개설
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 상단개설비용 응답
								break;
							case 1:		// 입력
								{
									m_Button_Guild_OK.SetAction(TRUE);
									m_Button_Guild_Cancel.SetAction(TRUE);
									if(m_Button_Guild_OK.Process(TempbMouseDown))
									{
										// 현재 상태에서 상단을 만들수 있는지 검사(Level)
										// Skill(상단개설가능여부)
										if((pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) >= GUILDMAKE_MINLEVEL) || (pMyOnlineWorld->pMyData->GetTradeGrade() >= GUILDMAKE_MINCREDIT))
										{
											CHAR TempBuffer[ON_GUILDNAME_LENGTH+1];
											ZeroMemory((char*)TempBuffer, sizeof(TempBuffer));
											if(pMyOnlineWorld->pMyData->GetMoney() >= m_GuildCreateCost)
											{
												m_Guild_GuildName->GetString((char*)TempBuffer);
												pMyOnlineWorld->pOnlineClient->SendRequestGovernment_MakeGuild((char*)TempBuffer);
												
												m_Guild_NowMessageState = 2;
											}
											else
											{
												sprintf(TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_OPEN_COST), m_GuildCreateCost);
												pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), (char*)TempBuffer);
												m_GuildCreateCost = 0;
												m_Guild_NowMessageState = 3;
											}
										}
										else
										{
											pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONAGREE), GUILDMAKE_MINLEVEL, GUILDMAKE_MINCREDIT);
											m_Guild_NowMessageState = 3;
										}
									}
									if(m_Button_Guild_Cancel.Process(TempbMouseDown))
									{
										if(m_Guild_GuildName)
										{
											delete m_Guild_GuildName;
											m_Guild_GuildName = NULL;
										}
										m_GuildCreateCost = 0;
										
										// Focus 지정
										pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
										SetFocus(m_YetEditHWND);
										EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);
										
										m_Guild_NowState = 1;
										m_Guild_NowMessageState = 0;
									}
								}
								break;

							case 2:		// 응답대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 3:		// 응답출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									if(m_Guild_GuildName)
									{
										delete m_Guild_GuildName;
										m_Guild_GuildName = NULL;
									}
									
									// Focus 지정
									pMyOnlineWorld->SetShortKey(m_YetEditLockShortKey);
									SetFocus(m_YetEditHWND);
									EnableWindow(m_YetEditHWND, m_YetEditHWNDEnable);

									m_Guild_NowState = 1;
									m_Guild_NowMessageState = 0;
								}
								break;
						}
						break;

					case ON_GOVERNMENT_GUILD_DELETE:			// 상단해체
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								m_Button_Guild_OK.SetAction(TRUE);
								m_Button_Guild_Cancel.SetAction(TRUE);

								if(m_Button_Guild_OK.Process(TempbMouseDown))
								{
									pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DeleteGuild();

									m_Guild_NowMessageState = 1;
								}
								if(m_Button_Guild_Cancel.Process(TempbMouseDown))
								{
									m_Guild_NowState = 1;
								}
								break;

							case 1:		// 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowState = 1;
									m_Guild_NowMessageState = 0;
								}
								break;
						}
						break;

					case ON_GOVERNMENT_GUILD_JOIN_LIST:			// 상단원 가입
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								if(m_GuildScrollSelect != -1)
								{
									m_Button_Guild_JoinOK.SetAction(TRUE);
									m_Button_Guild_JoinCancel.SetAction(TRUE);
								}
								m_Button_Guild_InvestmentTypeScrollUp.SetAction(TRUE);
								m_Button_Guild_InvestmentTypeScrollDown.SetAction(TRUE);
								m_Button_Guild_JoinExit.SetAction(TRUE);

								if(m_GuildScrollSelect >= 0 && m_GuildScrollSelect < 5)
								{
									char		TempBuffer[256];

									ZeroMemory(TempBuffer, 256);
									strncpy((char*)TempBuffer, m_GuildJoinWaitPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
									if(TempBuffer[0] != 0)
									{
										if(m_Button_Guild_JoinOK.Process(TempbMouseDown))
										{
											pMyOnlineWorld->pOnlineClient->SendRequestGovernment_JoinGuildPlayer((char*)TempBuffer);
											
											m_GuildJoinWaitPlayerTempFlag = 0;
											m_Guild_NowMessageState = 2;
										}
										if(m_Button_Guild_JoinCancel.Process(TempbMouseDown))
										{
											pMyOnlineWorld->pOnlineClient->SendRequestGovernment_JoinCancelGuildPlayer((char*)TempBuffer);
											
											m_GuildJoinWaitPlayerTempFlag = 1;
											m_Guild_NowMessageState = 2;
										}
									}
								}
								if(m_Button_Guild_JoinExit.Process(TempbMouseDown))
								{
									m_Guild_NowState = 1;

									m_GuildJoinWaitPlayerListCount = 0;
									if(m_GuildJoinWaitPlayerList)
									{
										delete[] m_GuildJoinWaitPlayerList;
										m_GuildJoinWaitPlayerList = NULL;
									}
									m_GuildJoinWaitPlayerList_PageMaxCount = 0;
									m_GuildJoinWaitPlayerList_PageNowCount = 0;
								}

								// List 선택
								if(TempbMouseDown)
								{
									for(TempCount = 0; TempCount < 5; TempCount++)
									{
										TempCount1 = TempCount + m_GuildScrollBar;
										if((TempCount1 >= 0) && (TempCount1 < m_GuildJoinWaitPlayerListCount))
										{
											if((ptMouse.x >= 302) && (ptMouse.x < 747) && (ptMouse.y >= (250 + (TempCount * 20))) && (ptMouse.y < (270 + (TempCount * 20))))
											{
												m_GuildScrollSelect = TempCount1;
												break;
											}
										}
									}

									// Scroll Bar
									if(m_GuildJoinWaitPlayerListCount > 5)
									{
										if((ptMouse.x >= 739) && (ptMouse.x < 763) && (ptMouse.y >= 205) && (ptMouse.y < 422))
										{
											x = ptMouse.x;
											y = ptMouse.y;

											if(y < 212) y = 212;
											if(y > 404) y = 404;

											y -= 212;

											TempSize         = 192.0f / (float)(m_GuildJoinWaitPlayerListCount - 4);
											m_GuildScrollBar = (SI32)((float)y / TempSize);
										}
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}

								// Page 처리
								CheckRenderPage((SI16)m_GuildJoinWaitPlayerList_PageMaxCount, (SI16)m_GuildJoinWaitPlayerList_PageNowCount
									, TempRenderStartPage, TempRenderEndPage, TempRenderPrior, TempRenderNext);

								ZeroMemory(TempPageData, sizeof(SI16) * 12);
								TempPageCount = 0;

								if(TempRenderPrior)
								{
									TempPageData[TempPageCount] = -2;
									TempPageCount++;
								}
								else
								{
									TempPageData[TempPageCount] = -4;
									TempPageCount++;
								}
								for(TempCount = 0; TempCount < ((TempRenderEndPage - TempRenderStartPage) + 1); TempCount++)
								{
									TempPageData[TempPageCount] = TempCount + TempRenderStartPage;
									TempPageCount++;
								}
								if(TempRenderNext)
								{
									TempPageData[TempPageCount] = -1;
									TempPageCount++;
								}
								else
								{
									TempPageData[TempPageCount] = -3;
									TempPageCount++;
								}

								// Page 출력
								TempRenderPosX = 501 - ((TempPageCount * 30) / 2);
								TempRenderPosY = 379;
								if(pMyOnlineWorld->fLeftUpSwitch)
								{
									for(TempCount = 0; TempCount < TempPageCount; TempCount++)
									{
										if((ptMouse.x >= (TempRenderPosX + (TempCount * 30))) && (ptMouse.x < (TempRenderPosX + ((TempCount + 1) * 30))) && (ptMouse.y >= 379) && (ptMouse.y < 395))
										{
											switch(TempPageData[TempCount])
											{
												case -1:
													pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(TempRenderEndPage + 1);
													m_Guild_NowMessageState = 1;
													m_GuildScrollBar = 0;
													m_GuildScrollSelect = 0;
													break;

												case -2:
													pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(TempRenderStartPage - 1);
													m_Guild_NowMessageState = 1;
													m_GuildScrollBar = 0;
													m_GuildScrollSelect = 0;
													break;

												case -3:
												case -4:
												case 0:
													break;

												default:
													if(m_GuildJoinWaitPlayerList_PageNowCount != TempPageData[TempCount])
													{
														pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(TempPageData[TempCount]);
														m_Guild_NowMessageState = 1;
														m_GuildScrollBar = 0;
														m_GuildScrollSelect = 0;
													}
													break;
											}
										}
									}
								}

								if(m_GuildScrollSelect != -1)
								{
									if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN))&&
										!(pMyOnlineWorld->pOnlineMessenger->IsAction()&&!pMyOnlineWorld->pOnlineMessenger->IsMini()))
									{
										char		TempBuffer[256];
										
										ZeroMemory(TempBuffer, 256);
										strncpy((char*)TempBuffer, m_GuildJoinWaitPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_JoinGuildPlayer((char*)TempBuffer);
										
										m_GuildJoinWaitPlayerTempFlag = 0;
										m_Guild_NowMessageState = 2;
									}
								}
								
								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RIGHT)))
								{
									SI16 NextPage = m_GuildJoinWaitPlayerList_PageNowCount + 1;
									if(NextPage <= TempRenderEndPage)
									{
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(NextPage);
										m_Guild_NowMessageState = 1;
										m_GuildScrollBar = 0;
										m_GuildScrollSelect = 0;
									}
								}
								
								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_LEFT)))
								{
									SI16 PrevPage = m_GuildJoinWaitPlayerList_PageNowCount - 1;
									if(PrevPage >= TempRenderStartPage)
									{
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(PrevPage);
										m_Guild_NowMessageState = 1;
										m_GuildScrollBar = 0;
										m_GuildScrollSelect = 0;
									}
								}
								
								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_UP)))
								{
									m_GuildScrollSelect--;
									if(m_GuildScrollSelect < 0)		m_GuildScrollSelect = 0;
								}
								
								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_DOWN)))
								{
									m_GuildScrollSelect++;
									if(m_GuildScrollSelect > m_GuildJoinWaitPlayerListCount - 1)	m_GuildScrollSelect = m_GuildJoinWaitPlayerListCount - 1;
								}

								if(m_Button_Guild_InvestmentTypeScrollUp.Process(TempbMouseDown))
								{
									m_GuildScrollBar--;
									if(m_GuildScrollBar < 0) m_GuildScrollBar = 0;
								}
								if(m_Button_Guild_InvestmentTypeScrollDown.Process(TempbMouseDown))
								{
									if(m_GuildJoinWaitPlayerListCount > 5)
									{
										m_GuildScrollBar++;
										if(m_GuildScrollBar > (m_GuildJoinWaitPlayerListCount - 4)) m_GuildScrollBar = (m_GuildJoinWaitPlayerListCount - 4);
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}
								break;

							case 1:		// 정보 받기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 3:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowMessageState = 0;
								}
								break;
						}
						break;

					case ON_GOVERNMENT_GUILD_DISSMISSAL_LIST:			// 상단원 해고
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								m_Button_Guild_DHS_Appointment.SetAction(FALSE);
								m_Button_Guild_HND_Appointment.SetAction(FALSE);
								m_Button_Guild_Class_Dissmissal.SetAction(FALSE);
								if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
								{
									if(m_GuildScrollSelect != -1)
									{
										// 대행수임명
										if(m_GuildGuildPlayerList[m_GuildScrollSelect].siClassInGuild != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
											m_Button_Guild_DHS_Appointment.SetAction(TRUE);

										// 행동대장임명 버튼활성화(사환일 경우 임명가능)
										if(m_GuildGuildPlayerList[m_GuildScrollSelect].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN))
											m_Button_Guild_HND_Appointment.SetAction(TRUE);

										// 직급해임 버튼활성화(대행수이거나 행동대장일 경우 해임가능)
										if(m_GuildGuildPlayerList[m_GuildScrollSelect].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS) || m_GuildGuildPlayerList[m_GuildScrollSelect].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND))
											m_Button_Guild_Class_Dissmissal.SetAction(TRUE);

										if(m_GuildGuildPlayerList[m_GuildScrollSelect].siClassInGuild != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
											m_Button_Guild_DismissalOK.SetAction(TRUE);
									}
								}
								//m_Button_Guild_InvestmentTypeScrollUp.SetAction(TRUE);
								//m_Button_Guild_InvestmentTypeScrollDown.SetAction(TRUE);
								m_Button_Guild_JoinExit.SetAction(TRUE);

								if(m_GuildScrollSelect != -1)
								{
									// 대행수 임명
									if(m_Button_Guild_DHS_Appointment.Process(TempbMouseDown))
									{
										BYTE		TempBuffer[256];
										ZeroMemory(TempBuffer, 256);
										strncpy((char*)TempBuffer, (char*)m_GuildGuildPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_AppointClass((char*)TempBuffer, pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS));

										m_Guild_NowMessageState = 5;
									}
									// 행동대장 임명
									else if(m_Button_Guild_HND_Appointment.Process(TempbMouseDown))
									{
										BYTE		TempBuffer[256];
										ZeroMemory(TempBuffer, 256);
										strncpy((char*)TempBuffer, (char*)m_GuildGuildPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_AppointClass((char*)TempBuffer, pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND));

										m_Guild_NowMessageState = 5;
									}
									// 대행수, 행동대장 직급해임
									else if(m_Button_Guild_Class_Dissmissal.Process(TempbMouseDown))
									{
										BYTE		TempBuffer[256];
										ZeroMemory(TempBuffer, 256);
										strncpy((char*)TempBuffer, (char*)m_GuildGuildPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DismissClass((char*)TempBuffer);

										m_Guild_NowMessageState = 6;
									}
									// 상단원 해고
									else if(m_Button_Guild_DismissalOK.Process(TempbMouseDown))
									{
										if(m_GuildGuildPlayerList[m_GuildScrollSelect].siClassInGuild != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
										{
											BYTE		TempBuffer[256];
											ZeroMemory(TempBuffer, 256);
											strncpy((char*)TempBuffer, (char*)m_GuildGuildPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
											pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DismissalGuildPlayer((char*)TempBuffer);
											
											m_Guild_NowMessageState = 2;
										}
									}
								}
								if(m_Button_Guild_JoinExit.Process(TempbMouseDown))
								{
									m_Guild_NowState = 1;

									m_GuildGuildPlayerListCount = 0;
									if(m_GuildGuildPlayerList)
									{
										delete[] m_GuildGuildPlayerList;
										m_GuildGuildPlayerList = NULL;
									}
									m_GuildGuildPlayerList_PageMaxCount = 0;
									m_GuildGuildPlayerList_PageNowCount = 0;
								}

								// List 선택
								if(TempbMouseDown)
								{
									for(TempCount = 0; TempCount < 5; TempCount++)
									{
										TempCount1 = TempCount + m_GuildScrollBar;
										if((TempCount1 >= 0) && (TempCount1 < m_GuildGuildPlayerListCount))
										{
											if((ptMouse.x >= 302) && (ptMouse.x < 747) && (ptMouse.y >= (250 + (TempCount * 20))) && (ptMouse.y < (270 + (TempCount * 20))))
											{
												if(strncmp((char*)m_GuildGuildPlayerList[TempCount1].szName, (char*)pMyOnlineWorld->pMyData->GetMyCharName(), ON_ID_LENGTH) != 0)
												{
													m_GuildScrollSelect = TempCount1;
												}
												break;
											}
										}
									}

									// Scroll Bar
									if(m_GuildGuildPlayerListCount > 5)
									{
										if((ptMouse.x >= 739) && (ptMouse.x < 763) && (ptMouse.y >= 205) && (ptMouse.y < 422))
										{
											x = ptMouse.x;
											y = ptMouse.y;

											if(y < 212) y = 212;
											if(y > 404) y = 404;

											y -= 212;

											TempSize         = 192.0f / (float)(m_GuildGuildPlayerListCount - 4);
											m_GuildScrollBar = (SI32)((float)y / TempSize);
										}
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}

								// Page 처리
								CheckRenderPage((SI16)m_GuildGuildPlayerList_PageMaxCount, (SI16)m_GuildGuildPlayerList_PageNowCount
									, TempRenderStartPage, TempRenderEndPage, TempRenderPrior, TempRenderNext);

								ZeroMemory(TempPageData, sizeof(SI16) * 12);
								TempPageCount = 0;

								if(TempRenderPrior)
								{
									TempPageData[TempPageCount] = -2;
									TempPageCount++;
								}
								else
								{
									TempPageData[TempPageCount] = -4;
									TempPageCount++;
								}
								for(TempCount = 0; TempCount < ((TempRenderEndPage - TempRenderStartPage) + 1); TempCount++)
								{
									TempPageData[TempPageCount] = TempCount + TempRenderStartPage;
									TempPageCount++;
								}
								if(TempRenderNext)
								{
									TempPageData[TempPageCount] = -1;
									TempPageCount++;
								}
								else
								{
									TempPageData[TempPageCount] = -3;
									TempPageCount++;
								}

								// Page 선택
								TempRenderPosX = 501 - ((TempPageCount * 30) / 2);
								TempRenderPosY = 379;
								if(pMyOnlineWorld->fLeftUpSwitch)
								{
									for(TempCount = 0; TempCount < TempPageCount; TempCount++)
									{
										if((ptMouse.x >= (TempRenderPosX + (TempCount * 30))) && (ptMouse.x < (TempRenderPosX + ((TempCount + 1) * 30))) && (ptMouse.y >= 379) && (ptMouse.y < 395))
										{
											switch(TempPageData[TempCount])
											{
												case -1:
													pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(TempRenderEndPage + 1, 0);
													m_Guild_NowMessageState = 1;
													m_GuildScrollBar = 0;
													m_GuildScrollSelect = 0;
													break;

												case -2:
													pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(TempRenderStartPage - 1, 0);
													m_Guild_NowMessageState = 1;
													m_GuildScrollBar = 0;
													m_GuildScrollSelect = 0;
													break;

												case -3:
												case -4:
												case 0:
													break;

												default:
													if(m_GuildGuildPlayerList_PageNowCount != TempPageData[TempCount])
													{
														pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(TempPageData[TempCount], 0);
														m_Guild_NowMessageState = 1;
														m_GuildScrollBar = 0;
														m_GuildScrollSelect = 0;
													}
													break;
											}
										}
									}
								}

								if(m_GuildScrollSelect != -1)
								{
									if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN))&&!(pMyOnlineWorld->pOnlineMessenger->IsAction()&&!pMyOnlineWorld->pOnlineMessenger->IsMini()))
									{
										BYTE		TempBuffer[256];

										ZeroMemory(TempBuffer, 256);
										strncpy((char*)TempBuffer, (char*)m_GuildGuildPlayerList[m_GuildScrollSelect].szName, ON_ID_LENGTH);
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DismissalGuildPlayer((char*)TempBuffer);

										m_Guild_NowMessageState = 2;
									}
								}

								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RIGHT)))
								{
									SI16 NextPage = m_GuildGuildPlayerList_PageNowCount + 1;
									if(NextPage <= TempRenderEndPage)
									{
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(NextPage, 0);
										m_Guild_NowMessageState = 1;
										m_GuildScrollBar = 0;
										m_GuildScrollSelect = 0;
									}
								}
								
								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_LEFT)))
								{
									SI16 PrevPage = m_GuildGuildPlayerList_PageNowCount - 1;
									if(PrevPage >= TempRenderStartPage)
									{
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(PrevPage, 0);
										m_Guild_NowMessageState = 1;
										m_GuildScrollBar = 0;
										m_GuildScrollSelect = 0;
									}
								}

								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_UP)))
								{
									m_GuildScrollSelect--;
									if(m_GuildScrollSelect < 0)		m_GuildScrollSelect = 0;
								}
								
								if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_DOWN)))
								{
									m_GuildScrollSelect++;
									if(m_GuildScrollSelect > m_GuildGuildPlayerListCount - 1)	m_GuildScrollSelect = m_GuildGuildPlayerListCount - 1;
								}

								if(m_Button_Guild_InvestmentTypeScrollUp.Process(TempbMouseDown))
								{
									m_GuildScrollBar--;
									if(m_GuildScrollBar < 0) m_GuildScrollBar = 0;
								}
								if(m_Button_Guild_InvestmentTypeScrollDown.Process(TempbMouseDown))
								{
									if(m_GuildGuildPlayerListCount > 5)
									{
										m_GuildScrollBar++;
										if(m_GuildScrollBar > (m_GuildGuildPlayerListCount - 4)) m_GuildScrollBar = (m_GuildGuildPlayerListCount - 4);
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}
								break;

							case 1:		// 정보 받기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 3:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowMessageState = 0;
								}
								break;

							case 4:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowState        = 1;
									m_Guild_NowMessageState = 0;
								}
								break;
							case 5:		// 대행수, 행동대장 직급임명 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;
							case 6:		// // 대행수, 행동대장 직급해임 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;
						}
						break;

					case ON_GOVERNMENT_GUILD_LIST:			// 다른 상단
						BYTE	TempBuffer[256];
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								m_Button_Guild_InvestmentTypeScrollUp.SetAction(TRUE);
								m_Button_Guild_InvestmentTypeScrollDown.SetAction(TRUE);
								m_Button_Guild_Cancel.SetAction(TRUE);

								if(m_Button_Guild_Cancel.Process(TempbMouseDown))
								{
									m_Guild_NowState = 1;

									m_GuildDiffGuildListCount = 0;
									if(m_GuildDiffGuildList)
									{
										delete[] m_GuildDiffGuildList;
										m_GuildDiffGuildList = NULL;
									}
								}

								// List 선택
								if(TempbMouseDown)
								{
									for(TempCount = 0; TempCount < 8; TempCount++)
									{
										TempCount1 = TempCount + m_GuildScrollBar;
										if((TempCount1 >= 0) && (TempCount1 < m_GuildDiffGuildListCount))
										{
											if((ptMouse.x >= 406) && (ptMouse.x < 446) && (ptMouse.y >= (245 + (TempCount * 20))) && (ptMouse.y < (265 + (TempCount * 20))))
											{
												// 현재 대방인지 검사
												if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
												{
													if(pMyOnlineWorld->pMyData->GetGuildName())
													{
														// 현재 마을에 길드가 있을때
														char		TempBufferGuildName[128];

														ZeroMemory(TempBufferGuildName, 128);
														strncpy((char*)TempBufferGuildName, pMyOnlineWorld->pMyData->GetGuildName(), ON_GUILDNAME_LENGTH);
														if(strncmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, (char*)TempBufferGuildName, ON_GUILDNAME_LENGTH) == 0)
														{
															// 이 마을의 길드가 자신이 속한 길드일때
															if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
															{
																// 선택한길드가 자기길드인지 검사
																if(strncmp((char*)m_GuildDiffGuildList[TempCount1].szGuildID, (char*)TempBufferGuildName, ON_GUILDNAME_LENGTH) != 0)
																{
																	// 관계를 할것인지 묻는다.
																	ZeroMemory(TempBuffer, 256);
																	strncpy((char*)TempBuffer, (char*)m_GuildDiffGuildList[TempCount1].szGuildID, ON_GUILDNAME_LENGTH);
																	switch(m_GuildDiffGuildList[TempCount1].siRelation)
																	{
																		case ON_RET_GLD_ENEMY:
																			m_GuildDiffGuild_Setting = ON_RET_GLD_NEUTRAL;
																			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONHOSOFF), (char*)TempBuffer);
																			m_Guild_NowMessageState  = 4;
																			m_GuildScrollSelect      = TempCount1;
																			break;

																		case ON_RET_GLD_NEUTRAL:
																			m_GuildDiffGuild_Setting = ON_RET_GLD_ENEMY;
																			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONHOSON), (char*)TempBuffer);
																			m_Guild_NowMessageState  = 4;
																			m_GuildScrollSelect      = TempCount1;
																			break;

																		case ON_RET_GLD_ALLY:
																			m_GuildDiffGuild_Setting = ON_RET_GLD_NEUTRAL;
																			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONALLOFF), (char*)TempBuffer);
																			m_Guild_NowMessageState  = 4;
																			m_GuildScrollSelect      = TempCount1;
																			break;

																		default:
																			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONINFOERROR), (char*)TempBuffer);
																			m_GuildScrollSelect     = TempCount1;
																			m_Guild_NowMessageState = 3;
																			break;
																	}
																}
															}
														}
													}
												}

												// 선택
												m_GuildScrollSelect = TempCount1;
												break;
											}
											if((ptMouse.x >= 300) && (ptMouse.x < 730) && (ptMouse.y >= (245 + (TempCount * 20))) && (ptMouse.y < (265 + (TempCount * 20))))
											{
												m_GuildScrollSelect = TempCount1;
												break;
											}
										}
									}

									// Scroll Bar
									if(m_GuildDiffGuildListCount > 8)
									{
										if((ptMouse.x >= 739) && (ptMouse.x < 763) && (ptMouse.y >= 212) && (ptMouse.y <= 404))
										{
											x = ptMouse.x;
											y = ptMouse.y;

											if(y < 212) y = 212;
											if(y > 404) y = 404;

											y -= 212;

											TempSize         = 191.0f / (float)(m_GuildDiffGuildListCount - 7);
											m_GuildScrollBar = (SI32)((float)y / TempSize);
										}
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}
								
								// 상단 마을리스트
								if(m_GuildScrollSelect >= 0)
									m_Button_Guild_VillageList.SetAction(TRUE);
								
								if(m_Button_Guild_VillageList.Process(TempbMouseDown))
								{
									if(m_GuildScrollSelect >= 0 && m_GuildScrollSelect < m_GuildDiffGuildListCount)
									{
										// 서버에 상단마을리스트 정보를 요청
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetGuildVillageList((char*)m_GuildDiffGuildList[m_GuildScrollSelect].szGuildID);
										m_Button_Guild_VillageList.SetAction(FALSE);
										m_Guild_NowMessageState = 5;
									}
									else
									{
										pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_NO_SELECT_GUILD));
									}
								}


								if(m_Button_Guild_InvestmentTypeScrollUp.Process(TempbMouseDown))
								{
									m_GuildScrollBar--;
									if(m_GuildScrollBar < 0) m_GuildScrollBar = 0;
								}
								if(m_Button_Guild_InvestmentTypeScrollDown.Process(TempbMouseDown))
								{
									if(m_GuildDiffGuildListCount > 8)
									{
										m_GuildScrollBar++;
										if(m_GuildScrollBar > (m_GuildDiffGuildListCount - 7)) m_GuildScrollBar = (m_GuildDiffGuildListCount - 7);
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}
								break;

							case 1:		// 정보 받기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 3:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowMessageState = 0;
								}
								break;

							case 4:		// 적대관계를 맺을건지 묻는부분
								switch(pMyOnlineWorld->m_siMsgBoxStatus)
								{
									case ON_MSGBOX_RETURN_OK:
										pMyOnlineWorld->pOnlineClient->SendRequestGovernment_SetRelation((char*)m_GuildDiffGuildList[m_GuildScrollSelect].szGuildID, m_GuildDiffGuild_Setting);
										m_Guild_NowMessageState = 2;
										break;

									case ON_MSGBOX_RETURN_CANCEL:
										m_Guild_NowMessageState = 0;
										break;
								}
								break;
							case 5:		// 상단마을리스트 정보를 받는부분
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;
						}
						break;
					case ON_GOVERNMENT_GUILD_JOIN:			// 상단 가입
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								m_Button_Guild_OK.SetAction(TRUE);
								m_Button_Guild_Cancel.SetAction(TRUE);

								if(m_Button_Guild_OK.Process(TempbMouseDown))
								{
									pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GuildJoinLeave();

									m_Guild_NowMessageState = 1;
								}
								if(m_Button_Guild_Cancel.Process(TempbMouseDown))
								{
									m_Guild_NowState = 1;
								}
								break;

							case 1:		// 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowState = 1;
									m_Guild_NowMessageState = 0;
								}
								break;
						}
						break;

					case ON_GOVERNMENT_GUILD_LEAVE:			// 상단 탈퇴
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								m_Button_Guild_OK.SetAction(TRUE);
								m_Button_Guild_Cancel.SetAction(TRUE);

								if(m_Button_Guild_OK.Process(TempbMouseDown))
								{
									pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GuildJoinLeave();

									m_Guild_NowMessageState = 1;
								}
								if(m_Button_Guild_Cancel.Process(TempbMouseDown))
								{
									m_Guild_NowState = 1;
								}
								break;

							case 1:		// 응답 대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowState = 1;
									m_Guild_NowMessageState = 0;
								}
								break;
						}
						break;

					case ON_GOVERNMENT_GUILD_SELECT_FLAG:			// 깃발 선택
						switch(m_Guild_NowMessageState)
						{
							case 0:		// 입력
								m_Button_Guild_OK.SetAction(FALSE);
								m_Button_Guild_Cancel.SetAction(TRUE);
								//m_Button_Guild_InvestmentTypeScrollUp.SetAction(TRUE);
								//m_Button_Guild_InvestmentTypeScrollDown.SetAction(TRUE);

								/* Skill(기발지정) 불가능할 경우
								{
									m_Guild_NowState        = 1;
									m_Guild_NowMessageState = 0;
								}*/
								
								// 버튼 처리
								if(m_GuildScrollSelect != -1)
								{
									m_Button_Guild_OK.SetAction(TRUE);
									if(m_Button_Guild_OK.Process(TempbMouseDown))
									{
										pMyOnlineWorld->pOnlineClient->SendChangeGuildFlag(m_GuildFlag_Index[m_GuildScrollSelect]);
										m_Guild_NowMessageState = 2;
									}
								}
								if(m_Button_Guild_Cancel.Process(TempbMouseDown))
								{
									m_Guild_NowState        = 1;
									m_Guild_NowMessageState = 0;
								}

								// 스크롤 처리
								if(TempbMouseDown)
								{
									if(((m_GuildFlag_MaxCount + 5) / 6) > 1)
									{
										if(m_Button_Guild_InvestmentTypeScrollUp.Process(TempbMouseDown))
										{
											m_GuildScrollBar--;
											if(m_GuildScrollBar < 0) m_GuildScrollBar = 0;
										}

										if(m_Button_Guild_InvestmentTypeScrollDown.Process(TempbMouseDown))
										{
											m_GuildScrollBar++;
											if(m_GuildScrollBar > (((m_GuildFlag_MaxCount + 5) / 6) - 1)) m_GuildScrollBar = (((m_GuildFlag_MaxCount + 5) / 6) - 1);
										}

										if((ptMouse.x >= 739) && (ptMouse.x < 763) && (ptMouse.y >= 205) && (ptMouse.y < 422))
										{
											x = ptMouse.x;
											y = ptMouse.y;

											if(y < 212) y = 212;
											if(y > 404) y = 404;

											y -= 212;

											TempSize         = 192.0f / (float)(((m_GuildFlag_MaxCount + 5) / 6) - 1);
											m_GuildScrollBar = (SI32)((float)y / TempSize);
										}
									}
									else
									{
										m_GuildScrollBar = 0;
									}
								}

								// 깃발 선택 처리
								{
									SI32		TempSelect;
									SI32		TempXCount;
									SI32		TempYCount;
									
									if(TempbMouseDown)
									{
										// Check List
										TempSelect = -1;
										for(TempYCount = 0; TempYCount < 1; TempYCount++)
										{
											for(TempXCount = 0; TempXCount < 6; TempXCount++)
											{
												if((ptMouse.x >= (308 + (TempXCount * 70))) && (ptMouse.x < (308 + ((TempXCount + 1) * 70))) && (ptMouse.y >= (244 + (TempYCount * 133))) && (ptMouse.y < (244 + ((TempYCount + 1) * 133))))
												{
													TempSelect = ((TempYCount + m_GuildScrollBar) * 6) + TempXCount;
													
													break;
												}
											}
										}

										if(TempSelect != -1)
										{
											if(TempSelect < m_GuildFlag_MaxCount)
											{
												if((m_GuildFlag_Index[TempSelect] & 0x8000) == 0)
												{
													m_GuildScrollSelect = TempSelect;
												
												}
											}
										}
									}
									if(IpD.IsDoubleClick() == TRUE)
									{
										// Check List
										TempSelect = -1;
										for(TempYCount = 0; TempYCount < 1; TempYCount++)
										{
											for(TempXCount = 0; TempXCount < 6; TempXCount++)
											{
												if((ptMouse.x >= (308 + (TempXCount * 70))) && (ptMouse.x < (308 + ((TempXCount + 1) * 70))) && (ptMouse.y >= (244 + (TempYCount * 133))) && (ptMouse.y < (244 + ((TempYCount + 1) * 133))))
												{
													TempSelect = ((TempYCount + m_GuildScrollBar) * 6) + TempXCount;
													
													break;
												}
											}
										}

										if(TempSelect != -1)
										{
											if(TempSelect < m_GuildFlag_MaxCount)
											{
												if((m_GuildFlag_Index[TempSelect] & 0x8000) == 0)
												{
													m_GuildScrollSelect = TempSelect;

													pMyOnlineWorld->pOnlineClient->SendChangeGuildFlag(m_GuildFlag_Index[m_GuildScrollSelect]);
													m_Guild_NowMessageState = 2;
													
												}
											}
										}
									}
								}
								break;

							case 1:		// 응답 대기
								{
									pMyOnlineWorld->pOnlineClient->SendUsedFlag();
									m_Guild_NowMessageState = 5;
								}
								break;

							case 5:
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 2:		// 응답대기
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
								break;

							case 3:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowState = 1;
									m_Guild_NowMessageState = 0;
								}
								break;

							case 4:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowMessageState = 0;
								}
								break;

							case 6:		// 응답 출력
								if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
								{
									m_Guild_NowState = 1;
									m_Guild_NowMessageState = 0;
								}
								break;
						}
						break;
					case ON_GOVERNMENT_GUILD_VILLAGE_LIST:			// 상단 마을리스트
						{
							m_Button_Guild_JoinExit.SetAction(TRUE);
							if(m_Button_Guild_JoinExit.Process(TempbMouseDown))
							{
								m_GuildVillageList_PageNowCount	= 1;
								m_Guild_NowState = ON_GOVERNMENT_GUILD_LIST;
								m_Guild_NowMessageState = 0;

								// 마을리스트 해제
								if(m_GuildVillageList)
								{
									delete[] m_GuildVillageList;
									m_GuildVillageList      = 0;
								}
							}
							
							// List 선택
							if(TempbMouseDown)
							{
								for(TempCount = 0; TempCount < 5; TempCount++)
								{
									TempCount1 = TempCount + (m_GuildVillageList_PageNowCount - 1) * 5;
									if((TempCount1 >= 0) && (TempCount1 < m_GuildVillageListCount))
									{
										if((ptMouse.x >= 302) && (ptMouse.x < 747) && (ptMouse.y >= (250 + (TempCount * 20))) && (ptMouse.y < (270 + (TempCount * 20))))
										{
											m_GuildScrollSelect = TempCount;
											break;
										}
									}
								}
							}
							
							// Page 처리
							CheckRenderPage((SI16)m_GuildVillageList_PageMaxCount, (SI16)m_GuildVillageList_PageNowCount
								, TempRenderStartPage, TempRenderEndPage, TempRenderPrior, TempRenderNext);
							
							ZeroMemory(TempPageData, sizeof(SI16) * 12);
							TempPageCount = 0;
							
							if(TempRenderPrior)
							{
								TempPageData[TempPageCount] = -2;
								TempPageCount++;
							}
							else
							{
								TempPageData[TempPageCount] = -4;
								TempPageCount++;
							}
							for(TempCount = 0; TempCount < ((TempRenderEndPage - TempRenderStartPage) + 1); TempCount++)
							{
								TempPageData[TempPageCount] = TempCount + TempRenderStartPage;
								TempPageCount++;
							}
							if(TempRenderNext)
							{
								TempPageData[TempPageCount] = -1;
								TempPageCount++;
							}
							else
							{
								TempPageData[TempPageCount] = -3;
								TempPageCount++;
							}
							
							// Page 선택
							TempRenderPosX = 501 - ((TempPageCount * 30) / 2);
							TempRenderPosY = 379;
							if(pMyOnlineWorld->fLeftUpSwitch)
							{
								for(TempCount = 0; TempCount < TempPageCount; TempCount++)
								{
									if((ptMouse.x >= (TempRenderPosX + (TempCount * 30))) && (ptMouse.x < (TempRenderPosX + ((TempCount + 1) * 30))) && (ptMouse.y >= 379) && (ptMouse.y < 395))
									{
										switch(TempPageData[TempCount])
										{
										case -3:	case -4:	break;
										case -1:		// Next
											m_GuildVillageList_PageNowCount = TempRenderStartPage + 10;
											m_GuildScrollSelect = 0;
											break;
										case -2:		// Prev
											m_GuildVillageList_PageNowCount = TempRenderStartPage - 1;
											m_GuildScrollSelect = 0;
											break;
										default:
											m_GuildVillageList_PageNowCount = TempPageData[TempCount];
											m_GuildScrollSelect = 0;
											break;
										}
									}		
								}
							}

							if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RIGHT)))
							{
								SI16 NextPage = m_GuildVillageList_PageNowCount + 1;
								if(NextPage <= TempRenderEndPage)
								{
									m_GuildVillageList_PageNowCount = NextPage;
									m_GuildScrollSelect = 0;
								}
							}
							
							if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_LEFT)))
							{
								SI16 PrevPage = m_GuildVillageList_PageNowCount - 1;
								if(PrevPage >= TempRenderStartPage)
								{
									m_GuildVillageList_PageNowCount = PrevPage;
									m_GuildScrollSelect = 0;
								}
							}
						}
						break;
				}
			}

			Draw_Back(pSurface, 0);
			Draw_Guild(pSurface, 0);
			pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
			break;

		case GOVERNMENT_NOWWORK_GUILD_END:
			Draw_Back(pSurface, 0);
			Draw_Guild(pSurface, 0);

			m_NowWork        = GOVERNMENT_NOWWORK_GUILD_ENDPOOL;
			m_StartTickCount = GetTickCount();
			break;

		case GOVERNMENT_NOWWORK_GUILD_ENDPOOL:
			Draw_Back(pSurface, GetTickCount() - m_StartTickCount);
			Draw_Guild(pSurface, GetTickCount() - m_StartTickCount);

			// 일정 시간이 지나면 Pool 로 이동
			if(m_StartTickCount < (GetTickCount() - 450))
			{
				Free();
				return TRUE;
			}
			break;
	}

	// 용병
	pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	// 장부
	if(pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
		pMyOnlineWorld->pOnlineTradeBook->Poll();
		pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 미니맵
	if(pMyOnlineWorld->pOnlineMiniMap->IsActive())
	{
		pMyOnlineWorld->pOnlineMiniMap->Pool();
		pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// Pannel을 출력한다.
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_INVENTORY | ONLINEPANNEL_DISABLEFLAG_CHAT | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON);

	if(IpD.LeftPressSwitch) bMouseDown = TRUE;
	else				    bMouseDown = FALSE;

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Back
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_Back(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	HDC					hDC;
	SI16				StartPos;
	float				TempFloat;
	BYTE				fFlag;				// 0 : Normal 출력, 1 : 닫혀진 스크롤, 2 : 떨어지는 스크롤

	// 시작 위치 검사
	if(TickCount > 450) TickCount = 450;
	StartPos = 0;
	fFlag    = 0;
	switch(m_NowWork)
	{
		case GOVERNMENT_NOWWORK_INVEST_STARTPOOL:
			TempFloat = (float)sin((90.0f - ((float)(TickCount) / 5.0f)) * 0.017444f);
			StartPos  = -(int)(TempFloat * TempFloat * 588.0f);
			fFlag     = 1;
			break;

		case GOVERNMENT_NOWWORK_DESCRIPT_ENDPOOL:
		case GOVERNMENT_NOWWORK_GUILD_ENDPOOL:
		case GOVERNMENT_NOWWORK_VILLLAGEINFO_ENDPOOL:
		case GOVERNMENT_NOWWORK_INVEST_ENDPOOL:
			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
			StartPos  = -((int)(TempFloat * TempFloat * 588.0f));
			fFlag     = 0;
			break;

		default:
			if((GetTickCount() - m_StartTickCount) < 600)
			{
				fFlag = 2;
			}
			break;
	}

	m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       - StartPos);

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 나가기 버튼 출력
		m_Button_Main_Exit.Put(&m_Image_Button_All_B, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{	
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);

		// 관청 Name 출력
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 573 - StartPos, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		m_Button_Main_Exit.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}

	m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       + StartPos);

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Investment
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_Investment(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	float				TempSize;
	SI32				TempCount;
	SI32				TempCount2;
	HDC					hDC;
	SI16				StartPos;
	float				TempFloat;
	BYTE				fFlag;				// 0 : Normal 출력, 1 : 닫혀진 스크롤, 2 : 떨어지는 스크롤
	BYTE				TempBuffer[256];
	BYTE				TempBuffer1[256];
	BYTE				TempFlag;
	MONEY				TempAddMoney;
	int					UpdateTime;          //투자한 시점	
	// 시작 위치 검사
	if(TickCount > 450) TickCount = 450;
	StartPos = 0;
	fFlag    = 0;
	if(TickCount < 0)
	{
		if(TickCount == -600)
		{
			TempFloat = 0;
			fFlag     = 1;
		}
		else
		{
			TempFloat = (float)sin((90.0f - ((float)(600 + TickCount) / 5.0f)) * 0.017444f);
			fFlag     = 2;
		}
	}

	switch(m_NowWork)
	{
		case GOVERNMENT_NOWWORK_INVEST_STARTPOOL:
			TempFloat = (float)sin((90.0f - ((float)(TickCount) / 5.0f)) * 0.017444f);
			StartPos  = -(int)(TempFloat * TempFloat * 592.0f);

			fFlag = 1;
			break;

		case GOVERNMENT_NOWWORK_INVEST_ENDPOOL:
			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			break;
	}

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() - StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() - StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() - StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() - StartPos);
	m_Button_Investment_SetInvestment.SetX(m_Button_Investment_SetInvestment.GetX() - StartPos);
	m_Button_Investment_InvestmentGetOut.SetX(m_Button_Investment_InvestmentGetOut.GetX() - StartPos);
	m_Button_Investment_InvestmentTypeScrollUp.SetX(m_Button_Investment_InvestmentTypeScrollUp.GetX()     - StartPos);
	m_Button_Investment_InvestmentTypeScrollDown.SetX(m_Button_Investment_InvestmentTypeScrollDown.GetX() - StartPos);
	m_Button_Investment_InvestmentDataScrollUp.SetX(m_Button_Investment_InvestmentDataScrollUp.GetX()     - StartPos);
	m_Button_Investment_InvestmentDataScrollDown.SetX(m_Button_Investment_InvestmentDataScrollDown.GetX() - StartPos);
	m_Button_Investment_VillageProfitYearScrollUp.SetX(m_Button_Investment_VillageProfitYearScrollUp.GetX()       - StartPos);
	m_Button_Investment_VillageProfitYearScrollDown.SetX(m_Button_Investment_VillageProfitYearScrollDown.GetX()   - StartPos);
	m_Button_Investment_VillageProfitVYearScrollUp.SetX(m_Button_Investment_VillageProfitVYearScrollUp.GetX()     - StartPos);
	m_Button_Investment_VillageProfitVYearScrollDown.SetX(m_Button_Investment_VillageProfitVYearScrollDown.GetX() - StartPos);
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 스크롤 출력
		clGrp.PutSpriteT(230 - StartPos, 0, m_Image_InvestmentInterface_Back.Header.Xsize, m_Image_InvestmentInterface_Back.Header.Ysize, m_Image_InvestmentInterface_Back.Image);
		
		// actdoll (2004/01/06 17:08) : 다른 방법으로 NPC 초상그리기
//		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GOV, pMyOnlineWorld->pOnlineVillage->GetNation() );

		
		//clGrp.PutSpriteCT(269 - StartPos, 112, m_Image_MainInterface_Face.Header.Xsize, m_Image_MainInterface_Face.Header.Ysize, m_Image_MainInterface_Face.Image);
		
		// 투자 List Scroll 출력
		clGrp.PutSpriteT(511 - StartPos, 107, m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
		
		// 투자자 List Scroll 출력
		if(m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment > 2)
		{
			TempSize    = 80.0f / (float)(m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment - 1);
			clGrp.PutSpriteT(749 - StartPos, 112 + (SI32)((float)m_NowScroll * TempSize), m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
		}
		else
		{
			clGrp.PutSpriteT(749 - StartPos, 112, m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
		}
		
		// 투자 버튼 출력
		//m_Button_Investment_InvestmentTypeScrollUp.Put(m_Image_Scroll_All_SBU,   0, 2, 1, BUTTON_PUT_NOMOVE);
		//m_Button_Investment_InvestmentTypeScrollDown.Put(m_Image_Scroll_All_SBD, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		m_Button_Investment_InvestmentDataScrollUp.Put(m_Image_Scroll_All_SBU,   0, 1, 2, BUTTON_PUT_NOMOVE);
		m_Button_Investment_InvestmentDataScrollDown.Put(m_Image_Scroll_All_SBD, 0, 1, 2, BUTTON_PUT_NOMOVE);
		
		clGrp.PutSpriteT(589 - StartPos, 244 + m_stCapitalScroll * 49, m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
		
		m_Button_Investment_VillageProfitYearScrollUp.Put(m_Image_Scroll_All_SBBU,   0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Investment_VillageProfitYearScrollDown.Put(m_Image_Scroll_All_SBBD, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		m_Button_Investment_VillageProfitVYearScrollUp.Put(m_Image_Scroll_All_SBU,   0, 1, 2, BUTTON_PUT_NOMOVE);
		m_Button_Investment_VillageProfitVYearScrollDown.Put(m_Image_Scroll_All_SBD, 0, 1, 2, BUTTON_PUT_NOMOVE);
		
		// 나가기 버튼 출력
		m_Button_Investment_SetInvestment.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Investment_InvestmentGetOut.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Investment.SetAction(TRUE);
		m_Button_Main_Investment.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Investment.SetAction(FALSE);
		m_Button_Main_VillageDescript.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Guild.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageInfo.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);
		
		m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX() - StartPos);
		m_Button_Main_Exit.Put(&m_Image_Button_All_B, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 500 - StartPos, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));
		m_Button_Main_Exit.Put(hDC);
		m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX() + StartPos);
		
		switch(fFlag)
		{
			case 0:
				TempFlag = TRUE;
				if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
				{
					// 만약 마을의 길드가 자신의 길드가 아니면
					TempFlag = FALSE;
				}

				if(TempFlag)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 400 - StartPos, 110, 101, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_NONEGUILDINVESTMENTHELP));
				}
				else
				{
					sprintf((char*)TempBuffer1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_ISGUILDINVESTMENTHELP), pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID);
					pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 400 - StartPos, 110, 101, (char*)TempBuffer1);
				}

				if(m_InvestmentData[m_NowSelectInvestment].m_fData)
				{
					// 투자자 List 출력
					SelectObject(hDC, m_RectHRGN);
					for(TempCount = 0; TempCount < 3; TempCount++)
					{						
						if(((m_NowScroll + TempCount) >= 0) && ((m_NowScroll + TempCount) < m_InvestmentData[m_NowSelectInvestment].m_MaxInvestment))
						{
							sprintf((char*)TempBuffer, "%2d. %s", m_NowScroll + TempCount + 1, m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_NowScroll + TempCount].m_InvestmentName);
							pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 527 - StartPos, 110 + (TempCount * 36), (char*)TempBuffer, RGB(200, 200, 200));

							if(m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_NowScroll + TempCount].m_InvestmentType != 2)
							{
								UpdateTime = m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_NowScroll + TempCount].m_InvestmentUpdateTime;
								switch(pGame->LanguageCode)
								{
									case ELANGAGECODE_KOREAN:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 1000000), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100);
										break;

									case ELANGAGECODE_ENGLISH:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100, (UpdateTime / 1000000));
										break;

									case ELANGAGECODE_CHINA:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 1000000), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100);
										break;

									case ELANGAGECODE_JAPAN:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 1000000), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100);
										break;

									case ELANGAGECODE_TAIWAN:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 1000000), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100);
										break;

									case ELANGAGECODE_THAILAND:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100, (UpdateTime / 1000000));
										break;

									case ELANGAGECODE_EVENT01:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 1000000), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100);
										break;

									case ELANGAGECODE_HONGKONG:
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARMONTHDAY), (UpdateTime / 1000000), (UpdateTime / 10000) % 100, (UpdateTime / 100) % 100);
										break;
								}
								pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 645 - StartPos, 110 + (TempCount * 36), (char*)TempBuffer, RGB(128, 128, 128));
							}

							MakeMoneyText(m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_NowScroll + TempCount].m_InvestmentMoney, (char*)TempBuffer1);
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_POSSETION), (SI32)m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_NowScroll + TempCount].m_InvestmentPercent, TempBuffer1);
							pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 547 - StartPos, 110 + 16 + (TempCount * 36), (char*)TempBuffer, RGB(128, 128, 128));
						}
					}
					SelectObject(hDC, m_RectBigHRGN);
					// 현재 투자에 대한 정보 출력
					MakeMoneyText(m_InvestmentData[m_NowSelectInvestment].m_NowInvestmentMoney, (char*)TempBuffer1);
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_VILINVESTMENTMONEY), TempBuffer1);
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 312 - StartPos, 248, (char*)TempBuffer, RGB(200, 200, 200));

					// 현재 투자된 금액 출력
					if(m_stCapitalSelect < m_stCapitalCount)
					{
						TempAddMoney = 0;
						for(TempCount = 0; TempCount < 7; TempCount++)
						{
							TempAddMoney += m_stCapital[m_stCapitalSelect].Profit[TempCount];
						}

						for(TempCount = 0; TempCount < 5; TempCount++)
						{
							TempCount2 = TempCount + m_stCapitalScroll;
							if(m_stCapital[m_stCapitalSelect].Profit[TempCount2])
							{
								switch(TempCount2)
								{
									case 0:			// 교역수익(시전)
 										MakeMoneyText(m_stCapital[m_stCapitalSelect].TradeProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEINCOME), TempBuffer1);
										break;

									case 1:			// 전장수익(은행)
										MakeMoneyText(m_stCapital[m_stCapitalSelect].BankProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_SESSTIONINCOME), TempBuffer1);
										break;

									case 2:			// 용병수익(훈련소)
										MakeMoneyText(m_stCapital[m_stCapitalSelect].EmployProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_SOLIDERINCOME), TempBuffer1);
										break;

									case 3:			// 치료수익(의원)
										MakeMoneyText(m_stCapital[m_stCapitalSelect].CureProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_HEALINCOME), TempBuffer1);
										break;

									case 4:			// 부두수익(항구)
										MakeMoneyText(m_stCapital[m_stCapitalSelect].TicketProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_HARBORINCOME), TempBuffer1);
										break;

									case 5:			// 주막수익(주막)
										MakeMoneyText(m_stCapital[m_stCapitalSelect].InnProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BARINCOME), TempBuffer1);
										break;

									case 6:			// 생산시설수익(농장,공장,목장)
										MakeMoneyText(m_stCapital[m_stCapitalSelect].PlantProfit, (char*)TempBuffer1);
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_PRODUCTIONINCOME), TempBuffer1);
										break;
								}
							}
							else
							{
								switch(TempCount2)
								{
									case 0:			// 교역수익(시전)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEINCOMENO));
										break;

									case 1:			// 전장수익(은행)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_SESSTIONINCOMENO));
										break;

									case 2:			// 용병수익(훈련소)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_SOLIDERINCOMENO));
										break;

									case 3:			// 치료수익(의원)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_HEALINCOMENO));
										break;

									case 4:			// 부두수익(항구)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_HARBORINCOMENO));
										break;

									case 5:			// 주막수익(주막)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BARINCOMENO));
										break;

									case 6:			// 생산시설수익(농장,공장,목장)
										sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_PRODUCTIONINCOMENO));
										break;
								}
							}

							pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 312 - StartPos, 270 + (TempCount * 18), (char*)TempBuffer, RGB(200, 200, 200));
						}

						MakeMoneyText(TempAddMoney, (char*)TempBuffer1);
						sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_ONEYEARINCOME), TempBuffer1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 312 - StartPos, 370, (char*)TempBuffer, RGB(200, 200, 200));

						sprintf((char*)TempBuffer,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEAR), m_stCapital[m_stCapitalSelect].Year);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 640 - StartPos, 347, (char*)TempBuffer, RGB(200, 200, 200));						
					}
					else
					{
						sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_ONEYEARINCOME1));
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 277 - StartPos, 380, (char*)TempBuffer, RGB(200, 200, 200));
					}

					// 투자한 금액 출력
					if(m_InvestmentData[m_NowSelectInvestment].m_MyInvestment == -1)
					{
						sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENEMONEY));
					}
					else
					{
						MakeMoneyText(m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_InvestmentData[m_NowSelectInvestment].m_MyInvestment].m_InvestmentMoney, (char*)TempBuffer1);
						sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_VILINVESTMENTMONEY), TempBuffer1);
					}
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 289 - StartPos, 405, (char*)TempBuffer, RGB(200, 200, 200));
				}

				// 투자 버튼 Text 출력
				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Investment_SetInvestment.Put(hDC);
				m_Button_Investment_InvestmentGetOut.Put(hDC);
				m_Button_Investment_InvestmentTypeScrollUp.Put(hDC);
				m_Button_Investment_InvestmentTypeScrollDown.Put(hDC);
				//m_Button_Investment_InvestmentDataScrollUp.Put(hDC);
				//m_Button_Investment_InvestmentDataScrollDown.Put(hDC);

				m_Button_Investment_VillageProfitYearScrollUp.Put(hDC);
				m_Button_Investment_VillageProfitYearScrollDown.Put(hDC);
				m_Button_Investment_VillageProfitVYearScrollUp.Put(hDC);
				m_Button_Investment_VillageProfitVYearScrollDown.Put(hDC);
				break;
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		m_Button_Main_Investment.Put(hDC);
		m_Button_Main_VillageDescript.Put(hDC);
		m_Button_Main_Guild.Put(hDC);
		m_Button_Main_VillageInfo.Put(hDC);
		

		pSurface->ReleaseDC(hDC);
	}

	m_Button_Investment_VillageProfitYearScrollUp.SetX(m_Button_Investment_VillageProfitYearScrollUp.GetX()       + StartPos);
	m_Button_Investment_VillageProfitYearScrollDown.SetX(m_Button_Investment_VillageProfitYearScrollDown.GetX()   + StartPos);
	m_Button_Investment_VillageProfitVYearScrollUp.SetX(m_Button_Investment_VillageProfitVYearScrollUp.GetX()     + StartPos);
	m_Button_Investment_VillageProfitVYearScrollDown.SetX(m_Button_Investment_VillageProfitVYearScrollDown.GetX() + StartPos);

	m_Button_Investment_SetInvestment.SetX(m_Button_Investment_SetInvestment.GetX() + StartPos);
	m_Button_Investment_InvestmentGetOut.SetX(m_Button_Investment_InvestmentGetOut.GetX() + StartPos);
	m_Button_Investment_InvestmentTypeScrollUp.SetX(m_Button_Investment_InvestmentTypeScrollUp.GetX()     + StartPos);
	m_Button_Investment_InvestmentTypeScrollDown.SetX(m_Button_Investment_InvestmentTypeScrollDown.GetX() + StartPos);
	m_Button_Investment_InvestmentDataScrollUp.SetX(m_Button_Investment_InvestmentDataScrollUp.GetX()     + StartPos);
	m_Button_Investment_InvestmentDataScrollDown.SetX(m_Button_Investment_InvestmentDataScrollDown.GetX() + StartPos);

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() + StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() + StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() + StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() + StartPos);

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Investment GetOut
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_InvestmentGetOut(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	HDC					hDC;
	BYTE				TempBuffer[256];
	BYTE				TempBuffer1[256];
	BYTE				TempBuffer2[256];
	MONEY				TempMoney;
	int					Len;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 바탕 출력
		clGrp.PutSpriteT(244, 206, m_Image_VillageInfoDefenceInvestmentInterface_Back.Header.Xsize, m_Image_VillageInfoDefenceInvestmentInterface_Back.Header.Ysize, m_Image_VillageInfoDefenceInvestmentInterface_Back.Image);

		// 버튼 출력
		m_Button_Investment_InvestmentGetOut_OK.Put(&m_Image_Button_All_EM,     0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Investment_InvestmentGetOut_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);

		// 인출 헬프
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_STRUCTHELP_GOVERNMENT_INVESTMENTGETOUTHELP), m_InvestmentData[m_NowSelectInvestment].m_GetOutTex);
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 258, 218, 268, (char*)TempBuffer);

		// 인출금액
		TempMoney = (m_InvestmentGetOutMoney * (100 - m_InvestmentData[m_NowSelectInvestment].m_GetOutTex)) / 100;
		MakeMoneyText(TempMoney, (char*)TempBuffer1);
		while(strlen((char*)TempBuffer1) < 12)
		{
			Len = strlen((char*)TempBuffer1);
			TempBuffer1[Len]     = ' ';
			TempBuffer1[Len + 1] = NULL;
		}

		sprintf((char*)TempBuffer, "%s%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_TOTAL), TempBuffer1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 267, 277, (char*)TempBuffer, RGB(200, 200, 200));

		// 세금
		TempMoney = (m_InvestmentGetOutMoney * m_InvestmentData[m_NowSelectInvestment].m_GetOutTex) / 100;
		MakeMoneyText(TempMoney, (char*)TempBuffer1);
		while(strlen((char*)TempBuffer1) < 12)
		{
			Len = strlen((char*)TempBuffer1);
			TempBuffer1[Len]     = ' ';
			TempBuffer1[Len + 1] = NULL;
		}
		sprintf((char*)TempBuffer, "%s%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_TEX), TempBuffer1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 267, 292, (char*)TempBuffer, RGB(200, 200, 200));

		// 총 인출금액
		MakeMoneyText(m_InvestmentData[m_NowSelectInvestment].m_InvestmentList[m_InvestmentData[m_NowSelectInvestment].m_MyInvestment].m_InvestmentMoney, (char*)TempBuffer);
		sprintf((char*)TempBuffer2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_MAX), TempBuffer);
		MakeMoneyText(m_InvestmentGetOutMoney, (char*)TempBuffer1);

		if(((GetTickCount() / 300) % 2) == 1)
		{
			SI32		Len;

			Len = strlen((char*)TempBuffer1);
			TempBuffer1[Len]     = '_';
			TempBuffer1[Len + 1] = NULL;
		}
		while(strlen((char*)TempBuffer1) < 12)
		{
			Len = strlen((char*)TempBuffer1);
			TempBuffer1[Len]     = ' ';
			TempBuffer1[Len + 1] = NULL;
		}

		sprintf((char*)TempBuffer, "%s%s%s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT_GETOUT_MONEY), TempBuffer1, TempBuffer2);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 267, 307, (char*)TempBuffer, RGB(200, 200, 200));

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		m_Button_Investment_InvestmentGetOut_OK.Put(hDC);
		m_Button_Investment_InvestmentGetOut_Cancel.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Descript
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_Descript(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	
	HDC					hDC;
	SI16				StartPos;
	float				TempFloat;

	// 시작 위치 검사
	if(TickCount > 450) TickCount = 450;
	StartPos = 0;

	switch(m_NowWork)
	{
		case GOVERNMENT_NOWWORK_DESCRIPT_ENDPOOL:
			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			break;
	}

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() - StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() - StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() - StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() - StartPos);
	m_Button_Descropt_SetDescropt.SetX(m_Button_Descropt_SetDescropt.GetX() - StartPos);
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 스크롤 출력
		clGrp.PutSpriteT(230 - StartPos, 0, m_Image_DescriptInterface_Back.Header.Xsize, m_Image_DescriptInterface_Back.Header.Ysize, m_Image_DescriptInterface_Back.Image);

		// actdoll (2004/01/06 17:08) : 다른 방법으로 NPC 초상그리기
//		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GOV, pMyOnlineWorld->pOnlineVillage->GetNation() );


		m_Button_Descropt_SetDescropt.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 나가기 버튼 출력
		m_Button_Main_Investment.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageDescript.SetAction(TRUE);
		m_Button_Main_VillageDescript.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageDescript.SetAction(FALSE);
		m_Button_Main_Guild.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageInfo.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);

				m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       - StartPos);
				m_Button_Main_Exit.Put(&m_Image_Button_All_B, 0, 2, 1, BUTTON_PUT_NOMOVE);
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 500 - StartPos, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));
				m_Button_Main_Exit.Put(hDC);
				m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       + StartPos);


		// 관청의 말
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 418 - StartPos, 110, 320, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BESTINVESTMENTONLY));

		// 내가 쓴 안내문
		if(m_pDescriptEditBox)
		{
			m_pDescriptEditBox->DrawTextAsBox(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), RGB(255, 255, 255), 285 - StartPos, 252, 400, NULL, TRUE);
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		m_Button_Descropt_SetDescropt.Put(hDC);

		m_Button_Main_Investment.Put(hDC);
		m_Button_Main_VillageDescript.Put(hDC);
		m_Button_Main_Guild.Put(hDC);
		m_Button_Main_VillageInfo.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}

	m_Button_Descropt_SetDescropt.SetX(m_Button_Descropt_SetDescropt.GetX() + StartPos);
	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() + StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() + StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() + StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() + StartPos);

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Village Info
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_VillageInfo(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	
	HDC					hDC;
	SI16				StartPos;
	float				TempFloat;
	BYTE				TempBuffer[256];
	float				TempSize;

	// 시작 위치 검사
	if(TickCount > 450) TickCount = 450;
	StartPos = 0;

	switch(m_NowWork)
	{
		case GOVERNMENT_NOWWORK_VILLLAGEINFO_ENDPOOL:
			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			break;
	}

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() - StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() - StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() - StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() - StartPos);
	m_Button_VillageInfo_InvestmentDefence.SetX(m_Button_VillageInfo_InvestmentDefence.GetX() - StartPos);
	m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX() - StartPos);
	m_Button_VillageInfo_DescriptScrollUp.SetX(m_Button_VillageInfo_DescriptScrollUp.GetX() - StartPos);
	m_Button_VillageInfo_DescriptScrollDown.SetX(m_Button_VillageInfo_DescriptScrollDown.GetX() - StartPos);
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 스크롤 출력
		clGrp.PutSpriteT(230 - StartPos, 0, m_Image_VillageInfoInterface_Back.Header.Xsize, m_Image_VillageInfoInterface_Back.Header.Ysize, m_Image_VillageInfoInterface_Back.Image);

		// actdoll (2004/01/06 17:08) : 다른 방법으로 NPC 초상그리기
//		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GOV, pMyOnlineWorld->pOnlineVillage->GetNation() );


		// Scroll
		if(m_VillageInfo_DescriptLine > 6)
		{
			TempSize = 75.0f / (float)((m_VillageInfo_DescriptLine - 6) - 1);
			clGrp.PutSpriteT(749 - StartPos, 112 + (SI32)((float)m_VillageInfo_DescriptScroll * TempSize), m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
		}
		else
		{
			clGrp.PutSpriteT(749 - StartPos, 112, m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
		}
		
		// 나가기 버튼 출력
		m_Button_Main_Investment.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageDescript.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Guild.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageInfo.SetAction(TRUE);
		m_Button_Main_VillageInfo.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageInfo.SetAction(FALSE);
		m_Button_Main_Exit.Put(&m_Image_Button_All_B, 0, 2, 1, BUTTON_PUT_NOMOVE);

		m_Button_VillageInfo_InvestmentDefence.Put(&m_Image_Button_All_EM,   0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_VillageInfo_DescriptScrollUp.Put(m_Image_Scroll_All_SBU,   0, 1, 0, BUTTON_PUT_NOMOVE);
		m_Button_VillageInfo_DescriptScrollDown.Put(m_Image_Scroll_All_SBD, 0, 1, 0, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);

		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 500 - StartPos, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));
		m_Button_Main_Exit.Put(hDC);

		// 마을 정보
		if(m_VillageInfo_InOutPlayerCount)
		{
			SelectObject(hDC, m_VillageInfo_RectHRGN);
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 418 - StartPos, 110 - (m_VillageInfo_DescriptScroll * 15), 300, (char*)m_VillageInfo_Descript);
			SelectObject(hDC, m_VillageInfo_RectBigHRGN);

			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_PEOPLE), pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 285 - StartPos, 251, (char*)TempBuffer, RGB(200, 200, 200));
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_YEARPEOPLE), m_VillageInfo_InOutPlayerCount);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 285 - StartPos, 281, (char*)TempBuffer, RGB(200, 200, 200));
			sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_DEFENCE), m_VillageInfo_NowDefence, m_VillageInfo_MaxDefence);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 285 - StartPos, 311, (char*)TempBuffer, RGB(200, 200, 200));
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 285 - StartPos, 341, 480, (char*)m_VillageInfo_StructList, RGB(200, 200, 200));
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		m_Button_Main_Investment.Put(hDC);
		m_Button_Main_VillageDescript.Put(hDC);
		m_Button_Main_Guild.Put(hDC);
		m_Button_Main_VillageInfo.Put(hDC);

		m_Button_VillageInfo_InvestmentDefence.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() + StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() + StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() + StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() + StartPos);
	m_Button_VillageInfo_InvestmentDefence.SetX(m_Button_VillageInfo_InvestmentDefence.GetX() + StartPos);
	m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       + StartPos);
	m_Button_VillageInfo_DescriptScrollUp.SetX(m_Button_VillageInfo_DescriptScrollUp.GetX() + StartPos);
	m_Button_VillageInfo_DescriptScrollDown.SetX(m_Button_VillageInfo_DescriptScrollDown.GetX() + StartPos);

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Village Info Set Defence Investment
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_VillageInfo_SetDefenceInvestment(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	HDC					hDC;
	BYTE				TempBuffer[256];
	BYTE				TempBuffer1[256];
	BYTE				TempBuffer2[256];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 바탕 출력
		clGrp.PutSpriteT(244, 206, m_Image_VillageInfoDefenceInvestmentInterface_Back.Header.Xsize, m_Image_VillageInfoDefenceInvestmentInterface_Back.Header.Ysize, m_Image_VillageInfoDefenceInvestmentInterface_Back.Image);

		// 버튼 출력
		m_Button_VillageInfo_InvestmentDefenceOK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_VillageInfo_InvestmentDefenceCancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);

		m_Button_Main_Exit.Put(&m_Image_Button_All_B, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 500, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));
		m_Button_Main_Exit.Put(hDC);

		// 마을 정보
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_VILINVESTMENTIMPORTANT));
		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, 258, 218, 268, (char*)TempBuffer);

		MakeMoneyText(m_VillageInfo_NowDefence, (char*)TempBuffer1);
		MakeMoneyText(m_VillageInfo_MaxDefence, (char*)TempBuffer2);
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_NOWDEFENCE), TempBuffer1, TempBuffer2);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 267, 277, (char*)TempBuffer, RGB(200, 200, 200));

		MakeMoneyText(m_VillageInfo_UpDefence, (char*)TempBuffer1);
		sprintf((char*)TempBuffer,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_UPDEFENCE), TempBuffer1);
		if(((GetTickCount() / 300) % 2) == 1)
		{
			SI32		Len;

			Len = strlen((char*)TempBuffer);
			TempBuffer[Len]     = '_';
			TempBuffer[Len + 1] = NULL;
		}
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 267, 292, (char*)TempBuffer, RGB(200, 200, 200));

		if(((m_VillageInfo_MaxDefence - m_VillageInfo_NowDefence) * m_VillageInfo_DefencePerMoney) > m_VillageInfo_MaxMoney)
		{
			MakeMoneyText(m_VillageInfo_MaxMoney / m_VillageInfo_DefencePerMoney, (char*)TempBuffer1);
		}
		else
		{
			MakeMoneyText(m_VillageInfo_MaxDefence - m_VillageInfo_NowDefence, (char*)TempBuffer1);
		}
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BESTOFBEST), TempBuffer1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 413, 292, (char*)TempBuffer, RGB(200, 200, 200));
		
		MakeMoneyText(m_VillageInfo_UpDefence * m_VillageInfo_DefencePerMoney, (char*)TempBuffer1);
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESMONEY), TempBuffer1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 267, 307, (char*)TempBuffer, RGB(200, 200, 200));

		MakeMoneyText(m_VillageInfo_MaxMoney, (char*)TempBuffer1);
		sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_BESTOFBEST), TempBuffer1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 413, 307, (char*)TempBuffer, RGB(200, 200, 200));

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		m_Button_VillageInfo_InvestmentDefenceOK.Put(hDC);
		m_Button_VillageInfo_InvestmentDefenceCancel.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Draw Guild
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::Draw_Guild(LPDIRECTDRAWSURFACE7 pSurface, int TickCount)
{
	HDC					hDC;
	SI16				StartPos;
	float				TempFloat;
	SI32				TempCount;
	SI32				TempCount1;
	BYTE				TempBuffer[256];
	BYTE				TempBuffer1[256];
	BYTE				TempBuffer2[256];
	float				TempSize;
	DWORD				TempColor;
	XSPR*				lpXSPR;
	SI16				TempRenderStartPage;
	SI16				TempRenderEndPage;
	SI16				TempRenderPrior;
	SI16				TempRenderNext;
	SI16				TempPageCount;
	SI16				TempPageData[12];
	SI16				TempRenderPosX;
	SI16				TempRenderPosY;

	// 시작 위치 검사
	if(TickCount > 450) TickCount = 450;
	StartPos = 0;

	switch(m_NowWork)
	{
		case GOVERNMENT_NOWWORK_GUILD_ENDPOOL:
			TempFloat = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
			StartPos  = -((int)(TempFloat * TempFloat * 592.0f));
			break;
	}

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() - StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() - StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() - StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() - StartPos);

	m_Button_Guild_MakeGuild.SetX(m_Button_Guild_MakeGuild.GetX() - StartPos);
	m_Button_Guild_DeleteGuild.SetX(m_Button_Guild_DeleteGuild.GetX() - StartPos);
	m_Button_Guild_DiffGuild.SetX(m_Button_Guild_DiffGuild.GetX() - StartPos);
	m_Button_Guild_FlagGuild.SetX(m_Button_Guild_FlagGuild.GetX() - StartPos);
	m_Button_Guild_JoinGuild.SetX(m_Button_Guild_JoinGuild.GetX() - StartPos);
	m_Button_Guild_DismissalGuild.SetX(m_Button_Guild_DismissalGuild.GetX() - StartPos);
	m_Button_Guild_PlayerJoinDismissalGuild.SetX(m_Button_Guild_PlayerJoinDismissalGuild.GetX() - StartPos);
	m_Button_Guild_OK.SetX(m_Button_Guild_OK.GetX() - StartPos);
	m_Button_Guild_Cancel.SetX(m_Button_Guild_Cancel.GetX() - StartPos);
	m_Button_Guild_JoinOK.SetX(m_Button_Guild_JoinOK.GetX() - StartPos);
	m_Button_Guild_JoinCancel.SetX(m_Button_Guild_JoinCancel.GetX() - StartPos);
	m_Button_Guild_JoinExit.SetX(m_Button_Guild_JoinExit.GetX() - StartPos);
	m_Button_Guild_DismissalOK.SetX(m_Button_Guild_DismissalOK.GetX() - StartPos);
	m_Button_Guild_VillageList.SetX(m_Button_Guild_VillageList.GetX() - StartPos);
	m_Button_Guild_DHS_Appointment.SetX(m_Button_Guild_DHS_Appointment.GetX() - StartPos);
	m_Button_Guild_HND_Appointment.SetX(m_Button_Guild_HND_Appointment.GetX() - StartPos);
	m_Button_Guild_Class_Dissmissal.SetX(m_Button_Guild_Class_Dissmissal.GetX() - StartPos);


	m_Button_Guild_InvestmentTypeScrollDown.SetX(m_Button_Guild_InvestmentTypeScrollDown.GetX() - StartPos);
	m_Button_Guild_InvestmentTypeScrollUp.SetX(m_Button_Guild_InvestmentTypeScrollUp.GetX() - StartPos);


	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// 스크롤 출력
		clGrp.PutSpriteT(230 - StartPos, 0, m_Image_GuildInterface_Back.Header.Xsize,  m_Image_GuildInterface_Back.Header.Ysize,  m_Image_GuildInterface_Back.Image);

		// actdoll (2004/01/06 17:08) : 다른 방법으로 NPC 초상그리기
//		pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GOV, pMyOnlineWorld->pOnlineVillage->GetNation() );

		m_Button_Guild_MakeGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Guild_DeleteGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Guild_DiffGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Guild_FlagGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Guild_JoinGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Guild_DismissalGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Guild_PlayerJoinDismissalGuild.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		switch(m_Guild_NowState)
		{
			case ON_GOVERNMENT_GUILD_INIT:			// Data를 기다리는중
				break;

			case ON_GOVERNMENT_GUILD_POLL:			// None
				break;

			case ON_GOVERNMENT_GUILD_OPEN:			// 상단 개설
				m_Button_Guild_OK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_GOVERNMENT_GUILD_DELETE:		// 상단해체
				m_Button_Guild_OK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_GOVERNMENT_GUILD_JOIN_LIST:		// 상단원 가입
				// 스크롤 출력
				if(m_GuildJoinWaitPlayerListCount > 5)
				{
					TempSize    = 130.0f / (float)(m_GuildJoinWaitPlayerListCount - 4);
					clGrp.PutSpriteT(755 - StartPos, 265 + (SI32)((float)m_GuildScrollBar * TempSize), m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
				}
				else
				{
					//clGrp.PutSpriteCT(755 - StartPos, 265,                          m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
				}

				// 버튼 출력
				m_Button_Guild_JoinOK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_JoinCancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_JoinExit.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);


				//m_Button_Guild_InvestmentTypeScrollUp.Put(m_Image_Scroll_All_SBU, 0, 2, 1, BUTTON_PUT_NOMOVE);
				//m_Button_Guild_InvestmentTypeScrollDown.Put(m_Image_Scroll_All_SBD, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_GOVERNMENT_GUILD_DISSMISSAL_LIST:	// 상단원 해고
				// 스크롤 출력
				if(m_GuildGuildPlayerListCount > 5)
				{
					TempSize    = 130.0f / (float)(m_GuildGuildPlayerListCount - 4);
					clGrp.PutSpriteT(755 - StartPos, 265 + (SI32)((float)m_GuildScrollBar * TempSize), m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
				}
				else
				{
					//clGrp.PutSpriteCT(755 - StartPos, 265,                          m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
				}

				// 버튼 출력
				m_Button_Guild_DHS_Appointment.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_HND_Appointment.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_Class_Dissmissal.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_DismissalOK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_JoinExit.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				//m_Button_Guild_InvestmentTypeScrollUp.Put(m_Image_Scroll_All_SBU, 0, 2, 1, BUTTON_PUT_NOMOVE);
				//m_Button_Guild_InvestmentTypeScrollDown.Put(m_Image_Scroll_All_SBD, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_GOVERNMENT_GUILD_LIST:				// 다른 상단
				// 스크롤 출력
				if(m_GuildDiffGuildListCount > 8)
				{
					TempSize    = 191.0f / (float)(m_GuildDiffGuildListCount - 7);
					clGrp.PutSpriteT(749 - StartPos, 212 + (SI32)((float)m_GuildScrollBar * TempSize), m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
				}
				else
				{
					clGrp.PutSpriteT(749 - StartPos, 212,                          m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
				}

				for(TempCount = 0; TempCount < 8; TempCount++)
				{
					TempCount1 = TempCount + m_GuildScrollBar;
					if(TempCount1 < m_GuildDiffGuildListCount)
					{
						// 길드 깃발
						lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagM(m_GuildDiffGuildList[TempCount1].siFlag);
						if(lpXSPR)
						{
							if(lpXSPR->Image)
							{
								clGrp.PutSpriteT(404 - StartPos, 245 + (TempCount * 20), lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[0]]);
							}
						}

						// 내 상태 표시
						switch(m_GuildDiffGuildList[TempCount1].siRelation)
						{
							case ON_RET_GLD_ALLY:		// 동맹
								clGrp.PutSpriteT(426 - StartPos, 245 + (TempCount * 20), m_Image_Button_Guild_DB.Header.Xsize, m_Image_Button_Guild_DB.Header.Ysize, &m_Image_Button_Guild_DB.Image[m_Image_Button_Guild_DB.Header.Start[2]]);
								break;

							case ON_RET_GLD_ENEMY:		// 적
								clGrp.PutSpriteT(426 - StartPos, 245 + (TempCount * 20), m_Image_Button_Guild_DB.Header.Xsize, m_Image_Button_Guild_DB.Header.Ysize, &m_Image_Button_Guild_DB.Image[m_Image_Button_Guild_DB.Header.Start[1]]);
								break;

							case ON_RET_GLD_NEUTRAL:	// 중립
							default:
								clGrp.PutSpriteT(426 - StartPos, 245 + (TempCount * 20), m_Image_Button_Guild_DB.Header.Xsize, m_Image_Button_Guild_DB.Header.Ysize, &m_Image_Button_Guild_DB.Image[m_Image_Button_Guild_DB.Header.Start[0]]);
								break;
						}

						// 상대 길드 표시
						switch(m_GuildDiffGuildList[TempCount1].siRelationOfOpponent)
						{
							case ON_RET_GLD_ALLY:		// 동맹
								clGrp.PutSpriteT(446 - StartPos, 245 + (TempCount * 20), m_Image_Button_Guild_DB.Header.Xsize, m_Image_Button_Guild_DB.Header.Ysize, &m_Image_Button_Guild_DB.Image[m_Image_Button_Guild_DB.Header.Start[2]]);
								break;

							case ON_RET_GLD_ENEMY:		// 적
								clGrp.PutSpriteT(446 - StartPos, 245 + (TempCount * 20), m_Image_Button_Guild_DB.Header.Xsize, m_Image_Button_Guild_DB.Header.Ysize, &m_Image_Button_Guild_DB.Image[m_Image_Button_Guild_DB.Header.Start[1]]);
								break;

							case ON_RET_GLD_NEUTRAL:	// 중립
							default:
								clGrp.PutSpriteT(446 - StartPos, 245 + (TempCount * 20), m_Image_Button_Guild_DB.Header.Xsize, m_Image_Button_Guild_DB.Header.Ysize, &m_Image_Button_Guild_DB.Image[m_Image_Button_Guild_DB.Header.Start[0]]);
								break;
						}
					}
				}

				m_Button_Guild_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_VillageList.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

				m_Button_Guild_InvestmentTypeScrollUp.Put(m_Image_Scroll_All_SBU, 0, 1, 2, BUTTON_PUT_NOMOVE);
				m_Button_Guild_InvestmentTypeScrollDown.Put(m_Image_Scroll_All_SBD, 0, 1, 2, BUTTON_PUT_NOMOVE);
				
				break;

			case ON_GOVERNMENT_GUILD_JOIN:			// 상단 가입
				m_Button_Guild_OK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_GOVERNMENT_GUILD_LEAVE:			// 상단 탈퇴
				m_Button_Guild_OK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_GOVERNMENT_GUILD_SELECT_FLAG:	// 깃발 선택
				m_Button_Guild_OK.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_Button_Guild_Cancel.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);

				{
					SI32		TempXCount;
					SI32		TempYCount;
					SI32		TempCount;
					XSPR*		lpXSPR;

					// Scroll
					if(((m_GuildFlag_MaxCount + 5) / 6) > 1)
					{
						TempSize = 192.0f / (float)(((m_GuildFlag_MaxCount + 5) / 6) - 1);
						clGrp.PutSpriteT(749 - StartPos, 212 + (SI32)((float)m_GuildScrollBar * TempSize), m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
					}
					else
					{
						clGrp.PutSpriteT(749 - StartPos, 212,                               m_Image_Scroll_All_M.Header.Xsize, m_Image_Scroll_All_M.Header.Ysize, m_Image_Scroll_All_M.Image);
					}

					for(TempYCount = 0; TempYCount < 1; TempYCount++)
					{
						for(TempXCount = 0; TempXCount < 6; TempXCount++)
						{
							TempCount = ((TempYCount + m_GuildScrollBar) * 6) + TempXCount;
							if(TempCount < m_GuildFlag_MaxCount)
							{
								lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagV(m_GuildFlag_Index[TempCount] & 0x0fff);

								if(lpXSPR)
								{
									if((m_GuildFlag_Index[TempCount] & 0x8000) == 0)
									{
										// Enable
										if(TempCount == m_GuildScrollSelect)
										{
											clGrp.PutSpriteT(308 + (TempXCount * 70) - StartPos, 244 + (TempYCount * 133)
												, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[(GetTickCount() / 120) % lpXSPR->Header.font_number]]);
										}
										else
										{
											clGrp.PutSpriteT(308 + (TempXCount * 70) - StartPos, 244 + (TempYCount * 133)
												, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[0]]);
										}
									}
									else
									{
										// Disable
										if(pMyOnlineWorld->pPaletteManager->GetPalette(pMyOnlineWorld->PaletteDataWhiteRed))
										{
											clGrp.PutSpriteT(308 + (TempXCount * 70) - StartPos, 244 + (TempYCount * 133)
												, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[0], pMyOnlineWorld->pPaletteManager->GetPalette(pMyOnlineWorld->PaletteDataWhiteRed));
										}
										else
										{
											clGrp.PutSpriteT(308 + (TempXCount * 70) - StartPos, 244 + (TempYCount * 133)
												, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[0]);
										}
									}
								}
							}
						}
					}
				}
				break;
			case ON_GOVERNMENT_GUILD_VILLAGE_LIST:	// 상단 마을리스트
				m_Button_Guild_JoinExit.Put(&m_Image_Button_All_EM, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;
		}

		// 나가기 버튼 출력
		m_Button_Main_Investment.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_VillageDescript.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_Button_Main_Guild.SetAction(TRUE);
		m_Button_Main_Guild.Put(&m_Image_Button_All_MM, 2, 2, 2, BUTTON_PUT_NOMOVE);
		m_Button_Main_Guild.SetAction(FALSE);
		m_Button_Main_VillageInfo.Put(&m_Image_Button_All_MM, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}	

	// Menu 버튼 Text 출력
	if(pSurface->GetDC(&hDC) == DD_OK)
	{
		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		SetBkMode(hDC, TRANSPARENT);

				m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       - StartPos);
				m_Button_Main_Exit.Put(&m_Image_Button_All_B, 0, 2, 1, BUTTON_PUT_NOMOVE);
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 500 - StartPos, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));
				m_Button_Main_Exit.Put(hDC);
				m_Button_Main_Exit.SetX(m_Button_Main_Exit.GetX()                       + StartPos);


		switch(m_Guild_NowState)
		{
			case ON_GOVERNMENT_GUILD_INIT:		// Data를 기다리는중
				break;

			case ON_GOVERNMENT_GUILD_POLL:		// None
				break;
				
			case ON_GOVERNMENT_GUILD_OPEN:		// 상단 개설
				if(m_GuildCreateCost)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 300 - StartPos, 250, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED),  RGB(200, 200, 200));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 320 - StartPos, 270, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED1), RGB(200, 200, 200));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 320 - StartPos, 290, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED2), RGB(200, 200, 200));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 320 - StartPos, 310, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED3), RGB(200, 200, 200));
					
					if(m_Guild_GuildName)
					{
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 332 - StartPos, 368, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_NAME), RGB(200, 200, 200));
						sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(2287), m_GuildCreateCost);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 310 - StartPos, 335, (char*)TempBuffer, RGB(200, 200, 200));
						
						m_Guild_GuildName->DrawText(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), RGB(200, 200, 200), 378 - StartPos, 368, NULL, TRUE);
					}
				}

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_OK.Put(hDC);
				m_Button_Guild_Cancel.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_DELETE:		// 상단해체
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_REALFIRE), pMyOnlineWorld->pMyData->GetGuildName());
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 359 - StartPos, 300, (char*)TempBuffer, RGB(200, 200, 200));

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_OK.Put(hDC);
				m_Button_Guild_Cancel.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_JOIN_LIST:		// 상단원 가입
				// 리스트 출력
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_THEOTHERS));
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 300 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));
				for(TempCount = 0; TempCount < 5; TempCount++)
				{
					TempCount1 = TempCount + m_GuildScrollBar;
					if((TempCount1 < m_GuildJoinWaitPlayerListCount) && (TempCount1 >= 0))
					{
						if(TempCount1 == m_GuildScrollSelect) TempColor = RGB(0, 255, 0);
						else                                  TempColor = RGB(200, 200, 200);

						// 번호
						sprintf((char*)TempBuffer,  "%3d", TempCount1 + 1 + ((m_GuildJoinWaitPlayerList_PageNowCount - 1) * 5));
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 302 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 아이디
						ZeroMemory(TempBuffer1, 256);
						strncpy((char*)TempBuffer1, (char*)m_GuildJoinWaitPlayerList[TempCount1].szName, ON_ID_LENGTH);
						sprintf((char*)TempBuffer,  "%3s", TempBuffer1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 338 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 레벨
						ZeroMemory(TempBuffer1, 256);
						sprintf((char*)TempBuffer,  "%4d", m_GuildJoinWaitPlayerList[TempCount1].siLevel);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 413 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 재산현황(순위)
						ZeroMemory(TempBuffer2, 256);
						MakeMoneyText(m_GuildJoinWaitPlayerList[TempCount1].mnPropery, (char*)TempBuffer2);
						sprintf((char*)TempBuffer,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), TempBuffer2);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 474 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 무역신용도
						ZeroMemory(TempBuffer1, 256);
						strncpy((char*)TempBuffer1, (char*)m_GuildJoinWaitPlayerList[TempCount1].szName, ON_ID_LENGTH);
						sprintf((char*)TempBuffer,  "%5d", m_GuildJoinWaitPlayerList[TempCount1].uiTradeCredit);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 595 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);
					}
				}

				// Page 처리
				CheckRenderPage((SI16)m_GuildJoinWaitPlayerList_PageMaxCount, (SI16)m_GuildJoinWaitPlayerList_PageNowCount
					, TempRenderStartPage, TempRenderEndPage, TempRenderPrior, TempRenderNext);

				ZeroMemory(TempPageData, sizeof(SI16) * 12);
				TempPageCount = 0;

				if(TempRenderPrior)
				{
					TempPageData[TempPageCount] = -2;
					TempPageCount++;
				}
				else
				{
					TempPageData[TempPageCount] = -4;
					TempPageCount++;
				}
				for(TempCount = 0; TempCount < ((TempRenderEndPage - TempRenderStartPage) + 1); TempCount++)
				{
					TempPageData[TempPageCount] = TempCount + TempRenderStartPage;
					TempPageCount++;
				}
				if(TempRenderNext)
				{
					TempPageData[TempPageCount] = -1;
					TempPageCount++;
				}
				else
				{
					TempPageData[TempPageCount] = -3;
					TempPageCount++;
				}

				// Page 출력
				TempRenderPosX = 501 - ((TempPageCount * 30) / 2);
				TempRenderPosY = 379;
				for(TempCount = 0; TempCount < TempPageCount; TempCount++)
				{
					TempColor = RGB(200, 200, 200);
					switch(TempPageData[TempCount])
					{
						case -3:
							TempColor = RGB(128, 128, 128);
						case -1:
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEXT));
							break;

						case -4:
							TempColor = RGB(128, 128, 128);
						case -2:
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PREV));
							break;

						case 0:
							break;

						default:
							if(m_GuildJoinWaitPlayerList_PageNowCount == TempPageData[TempCount]) TempColor = RGB(0, 255, 0);
							sprintf((char*)TempBuffer, "[%d]", TempPageData[TempCount]);
							break;
					}

					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC
						, TempRenderPosX + (TempCount * 30) - StartPos, TempRenderPosY, 30, 16
						, (char*)TempBuffer, TempColor);
				}

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_JoinOK.Put(hDC);
				m_Button_Guild_JoinCancel.Put(hDC);
				m_Button_Guild_JoinExit.Put(hDC);
				m_Button_Guild_InvestmentTypeScrollUp.Put(hDC);
				m_Button_Guild_InvestmentTypeScrollDown.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_DISSMISSAL_LIST:	// 상단원 해고
				// 리스트 출력
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_THEOTHERS1));
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 300 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));
				for(TempCount = 0; TempCount < 5; TempCount++)
				{
					TempCount1 = TempCount + m_GuildScrollBar;
					if(TempCount1 < m_GuildGuildPlayerListCount)
					{
						if(TempCount1 == m_GuildScrollSelect) TempColor = RGB(0, 255, 0);
						else                                  TempColor = RGB(200, 200, 200);

						// 번호
						sprintf((char*)TempBuffer,  "%3d", TempCount1 + 1 + ((m_GuildGuildPlayerList_PageNowCount - 1) * 5));
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 302 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 아이디
						ZeroMemory(TempBuffer1, 256);
						strncpy((char*)TempBuffer1, (char*)m_GuildGuildPlayerList[TempCount1].szName, ON_ID_LENGTH);
						sprintf((char*)TempBuffer,  "%3s", TempBuffer1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 338 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 직급
						sprintf((char*)TempBuffer,  "%5s", GetGradeInGuild(m_GuildGuildPlayerList[TempCount1].siClassInGuild));
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 398 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 레벨
						sprintf((char*)TempBuffer,  "%3d", m_GuildGuildPlayerList[TempCount1].siLevel);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 496 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 재산현황
						ZeroMemory(TempBuffer2, 256);
						MakeMoneyText(m_GuildGuildPlayerList[TempCount1].mnPropery, (char*)TempBuffer2);
						sprintf((char*)TempBuffer,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), TempBuffer2);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 538 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 무역신용도
						sprintf((char*)TempBuffer,  "%5d", m_GuildGuildPlayerList[TempCount1].uiTradeCredit);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 663 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);
					}
				}

				// Page 처리
				CheckRenderPage((SI16)m_GuildGuildPlayerList_PageMaxCount, (SI16)m_GuildGuildPlayerList_PageNowCount
					, TempRenderStartPage, TempRenderEndPage, TempRenderPrior, TempRenderNext);

				ZeroMemory(TempPageData, sizeof(SI16) * 12);
				TempPageCount = 0;

				if(TempRenderPrior)
				{
					TempPageData[TempPageCount] = -2;
					TempPageCount++;
				}
				else
				{
					TempPageData[TempPageCount] = -4;
					TempPageCount++;
				}
				for(TempCount = 0; TempCount < ((TempRenderEndPage - TempRenderStartPage) + 1); TempCount++)
				{
					TempPageData[TempPageCount] = TempCount + TempRenderStartPage;
					TempPageCount++;
				}
				if(TempRenderNext)
				{
					TempPageData[TempPageCount] = -1;
					TempPageCount++;
				}
				else
				{
					TempPageData[TempPageCount] = -3;
					TempPageCount++;
				}

				// Page 출력
				TempRenderPosX = 501 - ((TempPageCount * 30) / 2);
				TempRenderPosY = 379;
				for(TempCount = 0; TempCount < TempPageCount; TempCount++)
				{
					TempColor = RGB(200, 200, 200);
					switch(TempPageData[TempCount])
					{
						case -3:
							TempColor = RGB(128, 128, 128);
						case -1:
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEXT));
							break;

						case -4:
							TempColor = RGB(128, 128, 128);
						case -2:
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PREV));
							break;

						case 0:
							break;

						default:
							if(m_GuildGuildPlayerList_PageNowCount == TempPageData[TempCount]) TempColor = RGB(0, 255, 0);
							sprintf((char*)TempBuffer, "[%d]", TempPageData[TempCount]);
							break;
					}

					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC
						, TempRenderPosX + (TempCount * 30) - StartPos, TempRenderPosY, 30, 16
						, (char*)TempBuffer, TempColor);
				}

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_DHS_Appointment.Put(hDC);
				m_Button_Guild_HND_Appointment.Put(hDC);
				m_Button_Guild_Class_Dissmissal.Put(hDC);
				m_Button_Guild_DismissalOK.Put(hDC);
				m_Button_Guild_JoinExit.Put(hDC);
				m_Button_Guild_InvestmentTypeScrollUp.Put(hDC);
				m_Button_Guild_InvestmentTypeScrollDown.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_LIST:			// 다른 상단
				// 리스트 출력
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_THEOTHERS2));
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 329 - StartPos, 227, (char*)TempBuffer, RGB(200, 200, 200));
				for(TempCount = 0; TempCount < 8; TempCount++)
				{
					TempCount1 = TempCount + m_GuildScrollBar;
					if(TempCount1 < m_GuildDiffGuildListCount)
					{
						if(TempCount1 == m_GuildScrollSelect) TempColor = RGB(0, 255, 0);
						else                                  TempColor = RGB(200, 200, 200);

						sprintf((char*)TempBuffer,  "%3d.", TempCount1 + 1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 304  - StartPos, 247 + (TempCount * 20), (char*)TempBuffer, TempColor);

						ZeroMemory(TempBuffer1, 256);
						strncpy((char*)TempBuffer1, (char*)m_GuildDiffGuildList[TempCount1].szGuildID, ON_GUILDNAME_LENGTH);
						sprintf((char*)TempBuffer,  "%s", TempBuffer1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 329  - StartPos, 247 + (TempCount * 20), (char*)TempBuffer, TempColor);

						ZeroMemory(TempBuffer1, 256);
						strncpy((char*)TempBuffer1, (char*)m_GuildDiffGuildList[TempCount1].szDAIID, ON_ID_LENGTH);
						sprintf((char*)TempBuffer,  "%s", TempBuffer1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 509  - StartPos, 247 + (TempCount * 20), (char*)TempBuffer, TempColor);

						sprintf((char*)TempBuffer,  "%d", m_GuildDiffGuildList[TempCount1].uiOwnVillageNum);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 616  - StartPos, 247 + (TempCount * 20), (char*)TempBuffer, TempColor);

						sprintf((char*)TempBuffer,  "%d/%d", m_GuildDiffGuildList[TempCount1].uiGuildMemberNum, m_GuildDiffGuildList[TempCount1].uiMaxGuildMembersNum);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 680  - StartPos, 247 + (TempCount * 20), (char*)TempBuffer, TempColor);
					}
				}

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_VillageList.Put(hDC);
				m_Button_Guild_Cancel.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_JOIN:			// 상단 가입
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONJOIN), pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID);
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 359 - StartPos, 300, (char*)TempBuffer, RGB(200, 200, 200));

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_OK.Put(hDC);
				m_Button_Guild_Cancel.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_LEAVE:			// 상단 탈퇴
				if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
				{
					sprintf((char*)TempBuffer,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONJOINCEN), pMyOnlineWorld->pMyData->GetGuildName());
				}
				else
				{
					sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONJOINOUT), pMyOnlineWorld->pMyData->GetGuildName());
				}
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 359 - StartPos, 300, (char*)TempBuffer, RGB(200, 200, 200));

				SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_Button_Guild_OK.Put(hDC);
				m_Button_Guild_Cancel.Put(hDC);
				break;

			case ON_GOVERNMENT_GUILD_SELECT_FLAG:	// 깃발 선택
				m_Button_Guild_OK.Put(hDC);
				m_Button_Guild_Cancel.Put(hDC);
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 315 , 390, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_CHANGEFLAGNEEDMONEY), RGB(200, 200, 200));
				break;
			case ON_GOVERNMENT_GUILD_VILLAGE_LIST:
				// 제목(번호, 마을이름, 최고투자자, 최고투자액, 마을방어력)
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_NUMBER));
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 300 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_NAME));
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 338 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_OWNERNAME));
				pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 440 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_INVESTORMONEY));
				pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, 618 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));
				sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_DEFENCE));
				pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, 710 - StartPos, 225, (char*)TempBuffer, RGB(200, 200, 200));

				// 마을리스트 출력
				for(TempCount = 0; TempCount < 5; TempCount++)
				{
					TempCount1 = TempCount + (m_GuildVillageList_PageNowCount - 1) * 5;
					if((TempCount1 < m_GuildVillageListCount) && (TempCount1 >= 0))
					{
						if(TempCount == m_GuildScrollSelect) TempColor = RGB(0, 255, 0);
						else                                  TempColor = RGB(200, 200, 200);

						// 번호
						sprintf((char*)TempBuffer,  "%3d", TempCount1 + 1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 298 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 마을이름
						ZeroMemory(TempBuffer1, 256);
						VillageHeader	*pVillageHeader = pMyOnlineWorld->pOnlineVillageManager->GetVillageByCode(m_GuildVillageList[TempCount1].uiVillageCode);
						strcpy((char*)TempBuffer1, (char*)pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));
						sprintf((char*)TempBuffer,  "%3s", TempBuffer1);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 338 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 최고투자자
						sprintf((char*)TempBuffer,  "%3s", m_GuildVillageList[TempCount1].szBestInvestorName);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 440 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 최고투자액
						ZeroMemory(TempBuffer2, 256);
						MakeMoneyText(m_GuildVillageList[TempCount1].mnBestInvestorMoney, (char*)TempBuffer2);
						sprintf((char*)TempBuffer,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), TempBuffer2);
						pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, 615 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);

						// 마을방어도
						sprintf((char*)TempBuffer,  "%I64d", m_GuildVillageList[TempCount1].vdVillageDefencePower);
						pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, 705 - StartPos, 250 + (TempCount * 20), (char*)TempBuffer, TempColor);
					}
				}

				// Page 처리
				CheckRenderPage((SI16)m_GuildVillageList_PageMaxCount, (SI16)m_GuildVillageList_PageNowCount
					, TempRenderStartPage, TempRenderEndPage, TempRenderPrior, TempRenderNext);

				ZeroMemory(TempPageData, sizeof(SI16) * 12);
				TempPageCount = 0;

				if(TempRenderPrior)
				{
					TempPageData[TempPageCount] = -2;
					TempPageCount++;
				}
				else
				{
					TempPageData[TempPageCount] = -4;
					TempPageCount++;
				}
				for(TempCount = 0; TempCount < ((TempRenderEndPage - TempRenderStartPage) + 1); TempCount++)
				{
					TempPageData[TempPageCount] = TempCount + TempRenderStartPage;
					TempPageCount++;
				}
				if(TempRenderNext)
				{
					TempPageData[TempPageCount] = -1;
					TempPageCount++;
				}
				else
				{
					TempPageData[TempPageCount] = -3;
					TempPageCount++;
				}

				// Page 출력
				TempRenderPosX = 501 - ((TempPageCount * 30) / 2);
				TempRenderPosY = 379;
				for(TempCount = 0; TempCount < TempPageCount; TempCount++)
				{
					TempColor = RGB(200, 200, 200);
					switch(TempPageData[TempCount])
					{
						case -3:
							TempColor = RGB(128, 128, 128);
						case -1:
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEXT));
							break;

						case -4:
							TempColor = RGB(128, 128, 128);
						case -2:
							sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PREV));
							break;

						case 0:
							break;

						default:
							if(m_GuildVillageList_PageNowCount == TempPageData[TempCount]) TempColor = RGB(0, 255, 0);
							sprintf((char*)TempBuffer, "[%d]", TempPageData[TempCount]);
							break;
					}

					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC
						, TempRenderPosX + (TempCount * 30) - StartPos, TempRenderPosY, 30, 16
						, (char*)TempBuffer, TempColor);
				}
				m_Button_Guild_JoinExit.Put(hDC);
				break;
		}

		SelectObject(hDC, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		m_Button_Guild_MakeGuild.Put(hDC);
		m_Button_Guild_DeleteGuild.Put(hDC);
		m_Button_Guild_DiffGuild.Put(hDC);
		m_Button_Guild_FlagGuild.Put(hDC);
		m_Button_Guild_JoinGuild.Put(hDC);
		m_Button_Guild_DismissalGuild.Put(hDC);
		m_Button_Guild_PlayerJoinDismissalGuild.Put(hDC);

		m_Button_Main_Investment.Put(hDC);
		m_Button_Main_VillageDescript.Put(hDC);
		m_Button_Main_Guild.Put(hDC);
		m_Button_Main_VillageInfo.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}

	m_Button_Guild_MakeGuild.SetX(m_Button_Guild_MakeGuild.GetX() + StartPos);
	m_Button_Guild_DeleteGuild.SetX(m_Button_Guild_DeleteGuild.GetX() + StartPos);
	m_Button_Guild_DiffGuild.SetX(m_Button_Guild_DiffGuild.GetX() + StartPos);
	m_Button_Guild_FlagGuild.SetX(m_Button_Guild_FlagGuild.GetX() + StartPos);
	m_Button_Guild_JoinGuild.SetX(m_Button_Guild_JoinGuild.GetX() + StartPos);
	m_Button_Guild_DismissalGuild.SetX(m_Button_Guild_DismissalGuild.GetX() + StartPos);
	m_Button_Guild_PlayerJoinDismissalGuild.SetX(m_Button_Guild_PlayerJoinDismissalGuild.GetX() + StartPos);
	m_Button_Guild_OK.SetX(m_Button_Guild_OK.GetX() + StartPos);
	m_Button_Guild_Cancel.SetX(m_Button_Guild_Cancel.GetX() + StartPos);
	m_Button_Guild_JoinOK.SetX(m_Button_Guild_JoinOK.GetX() + StartPos);
	m_Button_Guild_JoinCancel.SetX(m_Button_Guild_JoinCancel.GetX() + StartPos);
	m_Button_Guild_JoinExit.SetX(m_Button_Guild_JoinExit.GetX() + StartPos);
	m_Button_Guild_DismissalOK.SetX(m_Button_Guild_DismissalOK.GetX() + StartPos);

	m_Button_Main_Investment.SetX(m_Button_Main_Investment.GetX() + StartPos);
	m_Button_Main_VillageDescript.SetX(m_Button_Main_VillageDescript.GetX() + StartPos);
	m_Button_Main_Guild.SetX(m_Button_Main_Guild.GetX() + StartPos);
	m_Button_Main_VillageInfo.SetX(m_Button_Main_VillageInfo.GetX() + StartPos);

	m_Button_Guild_InvestmentTypeScrollDown.SetX(m_Button_Guild_InvestmentTypeScrollDown.GetX() +StartPos);
	m_Button_Guild_InvestmentTypeScrollUp.SetX(m_Button_Guild_InvestmentTypeScrollUp.GetX() +StartPos);

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Find My ID
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16 OnlineVillageStructureGovernment::FindInvestmentList(OnInvestmentInfo* lpOnInvestmentInfo, SI16 InvestmentCount, char* lpMyID)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < InvestmentCount; TempCount++)
	{
		if(lpOnInvestmentInfo[TempCount].m_InvestmentType == 0)
		{
			if(strcmp((char*)lpOnInvestmentInfo[TempCount].m_InvestmentName, (char*)lpMyID) == 0)
			{
				return TempCount;
			}
		}
	}

	return -1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	직급 이름을 얻어옴
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
char* OnlineVillageStructureGovernment::GetGradeInGuild(SI32 Code)
{
	return pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(Code));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	지원자인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL OnlineVillageStructureGovernment::CheckGuildJWJ(SI32 Code)
{
	if(Code == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)) return TRUE;

	return FALSE;
}

UI16 OnlineVillageStructureGovernment::InvestmentList(BYTE *pMsg)
{
	OnResGovernmentGetInvestmentList*		lpOnResGovernmentGetInvestmentList;
	OnInvestmentInfo*						lpOnInvestmentInfo;

	lpOnResGovernmentGetInvestmentList = (OnResGovernmentGetInvestmentList*)pMsg;

	if(lpOnResGovernmentGetInvestmentList->dwResponse == ON_RET_OK)
	{
		lpOnInvestmentInfo                 = (OnInvestmentInfo*)((BYTE*)lpOnResGovernmentGetInvestmentList + sizeof(OnResGovernmentGetInvestmentList));
		
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_VillageGainingsMoney = lpOnResGovernmentGetInvestmentList->m_GainingsMoney;
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_MaxInvestment        = lpOnResGovernmentGetInvestmentList->m_InvestmentListCount;
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_MyInvestmentCanTime  = lpOnResGovernmentGetInvestmentList->m_MyInvestmentCanTime;
		
		CopyMemory(&m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_InvestmentList[0], lpOnInvestmentInfo, sizeof(OnInvestmentInfo) * lpOnResGovernmentGetInvestmentList->m_InvestmentListCount);
		
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_MyInvestment    = FindInvestmentList(lpOnInvestmentInfo, lpOnResGovernmentGetInvestmentList->m_InvestmentListCount, pMyOnlineWorld->pMyData->GetMyCharacters(pMyOnlineWorld->pMyData->GetSelectMyCharacterSlot())->name);
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_MyGainingsMoney = (MONEY)((double)m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_VillageGainingsMoney * m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_InvestmentList[m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_MyInvestment].m_InvestmentPercent) / 100;
		
		// 최대 투자액 검사
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_NowInvestmentMoney = 0;
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_BestInvestment     = -1;
		for(UI16 TempCount = 0; TempCount < lpOnResGovernmentGetInvestmentList->m_InvestmentListCount; TempCount++)
		{
			m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_NowInvestmentMoney += m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_InvestmentList[TempCount].m_InvestmentMoney;
			
			if(strcmp((char*)lpOnResGovernmentGetInvestmentList->m_MaxInvestmentID, (char*)m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_InvestmentList[TempCount].m_InvestmentName) == 0)
			{
				m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_BestInvestment = TempCount;
			}
		}
		
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_GetOutFlag = lpOnResGovernmentGetInvestmentList->m_fGetOut;
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_GetOutTex  = lpOnResGovernmentGetInvestmentList->m_GetOutTex;
		
		m_InvestmentData[lpOnResGovernmentGetInvestmentList->m_Type].m_fData = TRUE;
		
		// 마을의 최고 투자자 아이디 변경
		strcpy((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.m_MaxInvestmentID, (char*)lpOnResGovernmentGetInvestmentList->m_MaxInvestmentID);
		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
		{
			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
		}
	}
	
	if(m_fWaitFlag == GOVERNMENT_WAITFLAG_GETLIST) m_fWaitFlag = 0;
	return 0;
}