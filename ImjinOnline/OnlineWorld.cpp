#include <GSL.h>
#include	<Mouse.h>
#include	<clGame.h>
#include "MainFunction.h"

#include "..\Gersang\maindata.h"
#include "..\Gersang\music.h"
#include <SoundManager.h>
#include "..\Gersang\directx.h"

#include	"OnlineVersion.h"
#include	"OnlineWorld.h"
#include	"OnlineImjin2I.h"
#include	"HqFileManager.h"
#include	"OnlineETC.h"


#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include	<OnlineHelp.h>
#include	<OnlineConfig.h>
#include	<OnlineMegaText.h>
#include	<OnlineNumberControl.h>
#include	<OnlineTradeBook.h>
#include	<OnlineVillageSave.h>
#include	<OnlineProductStructure.h>
#include	<OnlineTrade.h>
#include	<OnlineBooth.h>
#include	<OnlineTip.h>
//#include	<OnlineBasicTip.h>			// actdoll (2004/02/16 16:11) : »ç¿ëÇÏ´Â °÷ÀÌ ¾øÀ¸¸ç ÀüºÎ È®ÀÎÇß½À´Ï´Ù.
//#include	<OnlineBasicQuest.h>		// actdoll (2004/02/16 16:11) : »ç¿ëÇÏ´Â °÷ÀÌ ¾øÀ¸¸ç ÀüºÎ È®ÀÎÇß½À´Ï´Ù.
#include	<OnlineParty.h>
#include	<OnlineFollower.h>
#include	<IProfit.h>
#include	<OnlineDungeonParser.h>
#include	<OnlineTicket-Parser.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS ¿ë µ¥ÀÌÅÍ Ãß°¡
#include	<OnlinePannel.h>
#include	<OnlineNoticeList.h>
#include	<OnlineKeyboard.h>
#include	<OnlineClient.h>
#include	<OnlineMyData.h>
#include	<OnlineVillage.h>
#include	<OnlineText.h>
#include	<OnlineResource.h>
#include	<OnlineMercenary-Parser.h>
#include	<OnlineFieldChat.h>
#include	<OnlineMiniMap.h>
#include	<OnlineMsgBox.h>
#include	<OnlineMap.h>
#include	<OnlineListControl.h>
#include	<OnlineInventory.h>
#include	<OnlineFieldArea.h>
#include	"MakePalette.h"
#include	"OnlineImoticon.h"
#include	"OnlineVillageManager.h"
#include	"OnlineCharActionCode.h"
#include	"OnlineCharUI.h"
#include	"OnlineVillageBaseStructure.h"
#include	"OnlineVillage-Parser.h"
#include	"clNumberRender.h"
#include	"OnlineTradeParser.h"
#include	"OnlineSkillParser.h"
#include	"OnlineGeneral-Parser.h"
#include	"OnlineMessenger.h"
#include	"OnlineQuest-Parser.h"
#include	"OnlineQuestPlay.h"
#include	"clItemInfoWindow.h"
#include	"slanderfilter.h"
#include	"OnlineSetting.h"
#include	"BuriedItemParser.h"
#include    "OnlineFieldAuction.h"
#include    "OnlineCharStatus.h"
//#include    "OnlineBanner.h"			// actdoll (2004/02/25 16:11) : »ç¿ëÇÏ´Â °÷ÀÌ ¾øÀ¸¸ç ÀüºÎ È®ÀÎÇß½À´Ï´Ù.
#include	"OnlineSmith.h"
#include	"OnlineSmithParser.h"
#include	"OnlineLevelMark-Parser.h"
#include	"OnlineMsgProc.h"
#include    "ddraw.h"
#include    "OnlineMFGS.h"
#include	"..\\gersang\\charinfo\\CharFileLoadManager.h"
#include	"MemMgr.h"
#include	<myfile.h>
#include	"bindJxFile.h"
#include	<DebugTool.h>
#include	"OnlineNPCTalk.h"
#include	"OnlineChatMacro.h"
#include	"OnlineQuestInterface.h"
#include	"OnlineMercenary.h"
#include	"OnlinePortrait.h"		// actdoll (031229)
#include	"stringcpy.h"
#include    "..\\SCCommon\\MacroCheckProcess.h"
#include	"OnlineMapSound.h"

#include	"..\EnumData\Enum_AttackAtb.h"			// AttackAtb
#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME ÄÁÆ®·Ñ·¯ ¼¼ÆÃ

#include "../SCCommon/OnlineCommonParser.h"	// Using Online Common Parser

#include	"FieldHeroInfoMgr.h"

// °ø¼º À¯´Ö °ü¸®ÀÚ
#include	"SiegeUnitTableMgr.h"

#include "SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"


//ÁØ¼®ÀÌÇü Ãß°¡ºÎºÐ 
/*
#include	"JSWnd.h"
#include	"JSWndMgr.h"
#include	"JSTestWnd.h"
*/
//#include	"JSWnd.h"
//#include	"JSWndMgr.h"
//#include	"JSTestWnd.h"
#include	"tlhelp32.h"
#include	"GGuard.h"

// sagolboss Ãß°¡ºÎºÐ ->
#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif
// <- Ãß°¡ ³¡ 

//#define	CPF		33				// ÇÑ ÇÁ·¹ÀÓ´ç Å¬·°¼ö´Â 40ÀÌ´Ù.
//#define	CPF		1			// ÇÑ ÇÁ·¹ÀÓ´ç Å¬·°¼ö´Â 40ÀÌ´Ù.

extern _clGame* pGame;
extern	_MainData				GD;									// °ÔÀÓÀÇ ÁÖµ¥ÀÌÅÍ 
extern _InputDevice				IpD;
extern	SoundManager			*g_pSoundManager;
extern	CCharFileLoadManager	g_FileLoadManager;					// Á¶ÇÕÇü Ä³¸¯ÅÍ
extern CItem					*g_pItem;
extern char						LogInID[128];

extern clock_t					CurrentClock, MonitorMacroClock1, MonitorMacroClock2;

extern int WindowModeSwitch;
extern int bActive;

LPDIRECTDRAWSURFACE7              lpCaptureSurface = NULL;

extern	_KindInfo					KI[];								// Ä³¸¯ÅÍ Á¾·ùº° µ¥ÀÌÅÍ 

// ÀÓÁø·Ï 2 Å¬¶óÀÌ¾ðÆ®¿Í Åë½ÅÀ» ÇÏ±â À§ÇÑ º¯¼öÀÌ´Ù.
OnlineImjin2I					GOnlineImjin2I;

// µð¹ö±×¸¦ À§ÇÑ Àü¿ªº¯¼ö.
// HQDebug  GHQDebugTool;

// actdoll (2004/08/27 0:34) : ÀÔ·Â±â »óÅÂ º¸´Â µð¹ö±× Ã¢(_DEBUG½Ã¸¸ °¡µ¿. ¾ø¾Ù·Á¸é ÁÖ¼®´Þ°Í)
//#define _SHOW_INPUTSTATUS_

// robypark 2005/1/7 18:41
// ¼öÄ¡(10%´ÜÀ§·Î)¸¦ °ÔÀÌÁö ÇüÅÂ·Î ±×·ÁÁØ´Ù.
// °ø¼ºÀü À¯´Ö Ã¼·Â, º¸±Þ °ÔÀÌÁö¿¡¼­ »ç¿ë
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¿Â¶óÀÎ ¿ùµå ÃÊ±â ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
BOOL	_clGame::InitOnlineWorld()
{	
	DeleteFile("ErrorCode.txt");
	DeleteFile("ObjectBug.txt");

	clGrp.SetClipArea(0, 0, ON_SCREEN_XSIZE - 1, ON_SCREEN_YSIZE - 1);

	pOnlineWorld	=	new cltOnlineWorld;		
	
	if(pOnlineWorld)
		pOnlineWorld->Init();

	if(pOnlineWorld)
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¿Â¶óÀÎ ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
UI16	_clGame::OnlineWorld()
{
	UI16	uiNextStatus = 0;

	uiNextStatus	=	pOnlineWorld->Poll();

	if(uiNextStatus)	
	{
		// GS_INITONLINEBATTLEGAMEÀº ÀÓÁø·Ï2 ÀüÅõ·Î µé¾î°¡±â À§ÇÔÀÌ¹Ç·Î
		// ÀÌ¶§ ¸ðµç ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØ¼­´Â ¾ÈµÈ´Ù.
		if(uiNextStatus != GS_INITONLINEBATTLEGAME)
			FreeOnlineWorld();	
	}
	
	return uiNextStatus;		
}

//----------------------------------------------------------------------------------------------------------------	
//	¼³¸í	:	¿Â¶óÀÎ ÇØÁ¦ ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
VOID	_clGame::FreeOnlineWorld()
{
	if(pOnlineWorld)
	{
		delete pOnlineWorld;
		pOnlineWorld = NULL;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÀÓÁø·Ï ¿Â¶óÀÎ ¿ùµå ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
cltOnlineWorld::cltOnlineWorld()
{
	PaletteWhiteColor           =   RGB(255, 255, 255);
	fYetLight					=	0.0f;

	bNotice						=	FALSE;

	m_GMFlag                    =   ON_USERGRADE_NORMAL;
	m_InfoFlag                  =   TRUE;
	m_ShowMessagBoxCharInfoFlag =   FALSE;
//	m_LanguageMode              =   FALSE;
	m_fChattingRoom             =   FALSE;
	bPay						=	FALSE;
	bTempFlag					=	FALSE;
	m_bForcedLogout				=	FALSE;
	bPeace						=	TRUE;
	bFirstLogIn					=	TRUE;
	m_VillageAttackDelay        =   0;
	m_ImageFreeTimer			=	0;

	m_GameTickCount             = 0;
	m_WaitTickCount				= 0;
	m_GameTime		            = 0;
	m_dwGameGuardCheckTick		= 0;

	m_HackGameTickCount         = 0;
	m_HackGameTime				= 0;
	m_TickTimeDiff				= 0;

	m_LastNowUserConnectMessageTickCount        = 0;
	m_LastNowUserConnectMessageNowUser          = 0;
	m_LastNowUserConnectMessageErrorOutPutCount = 0;

	m_iMinPlaySoundTick		  = 0;

	dwMapTileDrawTime         = 0;
	siMapTileDrawTimeAddTime  = 0;
	siMapTileDrawTimeAddCount = 0;

	uiTotalFrame				=	0;
	dwCurrentFrameStartTime		=	0;
	dwCurrentFrameAddTime		=	0;
	dwCurrentFrameSkippingCount	=	0;
	siGamePollStatus			=	0;	
	siMainPollStatus			=	0;
	BackgroundMusic				=	0;
	siTalkDealy					=	0;
	m_siNoField					=	-1;
	fLeftUpSwitch				=	FALSE;
	fRightUpSwitch				=	FALSE;
	fLeftOneSwitch				=	FALSE;
	fVillageRightOneSwitch		=	FALSE;
	fVillageLeftOneSwitch		=	FALSE;
	fRightOneSwitch				=	FALSE;
	fYetLeftPressSwitch			=	FALSE;
	fYetRightPressSwitch		=	FALSE;
	ShortKey					=	FALSE;
	bServerSelect				=	FALSE;
	pPaletteManager             =   NULL;
	m_GameOption                =   NULL;
	pTicketParser               =   NULL;
	pGeneralParser              =   NULL;
	pGeneralParserEx			=   NULL;
	pQuestParser				=	NULL;
	pLevelMarkParser			=	NULL;
	pTimer						=	NULL;
	fFirstGetTime               =   FALSE;
	pOnlineMercenaryParser      =   NULL;
	pOnlineEconomy				=	NULL;
	pIOnlineChar				=	NULL;
	pIOnlineObjectKI			=	NULL;
	pIOnlineObject				=	NULL;
	pOnlineMap					=	NULL;
	pOnlineVillage				=	NULL;
	pOnlineVillageDescriptList  =   NULL;
	pOnlineNoticeList           =   NULL;
	pOnlineFieldEffect          =   NULL;
	pclFindPath					=	NULL;
	pMyData						=	NULL;
	pOnlineImoticon             =   NULL;
	pOnlineClient				=	NULL;
	pOnlineClientChatting		=	NULL;
	pOnlineResource				=	NULL;
	pOnlineCharUI				=	NULL;
	pOnlineInventory			=	NULL;
	pOnlineKeyboard				=	NULL;
	pOnlineText					=	NULL;
	pConfig						=	NULL;
	pOnlineMegaText				=	NULL;
//	pOnlineCharAni				=	NULL;
	pOnlineCharActionCode		=	NULL;
	pIOnlineMouse				=	NULL;
	pOnlineMousePoint           =   NULL;
	pOnlineFieldArea			=	NULL;
	pOnlineVillageManager		=	NULL;
	pOnlinePannel				=	NULL;
	pOnlineMiniMap				=	NULL;
	pOnlineHelp					=	NULL;	
	pMousePointInfo				=	NULL;
	pOnlineNumPad				=	NULL;
	pOnlineVillageSave			=	NULL;
	pOnlineMsgBoxPay			=	NULL;
	pOnlineMsgBoxUsePortal      =   NULL;
	pOnlineMsgBoxError          =   NULL;
	pOnlineLoginMsgBox			=   NULL;
	pTempSearch					=	NULL;
	pOnlineBooth				=	NULL;
	pOnlineBench				=	NULL;
	pOnlineTip					=	NULL;
	pOnlineQuestPlay			=	NULL;
//	pOnlineBasicTip				=   NULL;
//	pOnlineBasicQuest			=   NULL;
	pOnlineParty				=	NULL;
	pOnlineMessenger			=	NULL;
	pOnlineDungeon				=	NULL;
	pOnlineSetting				=	NULL;
	pIProfit					=	NULL;
	pOnlineTradeParser			=	NULL;
	pOnlineSkillParser			=	NULL;
	siTempSearchNum				=	0;
	pOnlineVillageInfoData      =   NULL;
//	pOnlineBanner				=	NULL;
	pMemoryProtected            =   NULL;
	pOnlineCharParser			=	NULL;

	pOnlineReception            =   NULL;
	pOnlineFieldAuction         =   NULL;
	pOnlineSmith				=   NULL;
	pOnlineSmithParser			=   NULL;
	pOnlineMemMgr				=   NULL;
//	m_pMacroProcess				=	NULL;

	// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤ º¯¼ö
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	// robypark 2005/1/14 17:11
	// ÁÖÀÎ°ø Ä³¸¯ÅÍµéÀÇ Á¤º¸ °ü¸®ÀÚ ÃÊ±âÈ­
	m_pFieldHeroInfoMgr = NULL;

	// robypark 2005/1/31 12:55
	// °ø¼º À¯´Ö °ü¸®ÀÚ
	m_pSiegeUnitTableMgr = NULL;

	// ¹öÁ¯À» ¸¸µç´Ù.
	siOnlineMajorVersion		=	ON_CV_MAJOR;
	siOnlineMinorVersion		=	ON_CV_MINOR;

	bExitOnlineGame				=	FALSE;
	bFindTrade					=	FALSE;

	ZeroMemory( m_TempBuffer, sizeof( m_TempBuffer ) );
	wsprintf(szOnlineVersion, "Ver %d.%03d", siOnlineMajorVersion, siOnlineMinorVersion / 10);

	ZeroMemory(m_MemoUser, 32);

	bCharMove		= TRUE;

//	m_bEventDokdoQuestPlay	= FALSE;	// actdoll (2004/04/02 15:28) : = ÀÌº¥Æ® = µ¶µµ Äù½ºÆ®¸¦ ÁøÇàÁßÀÎ°¡?

	// ÁÖ¸· µµÂøÇÑ ¾ÆÀÌÅÛ°ü·Ã
	m_bReceivedItem			= FALSE;
	m_siAniNoField			= -1;
	m_bItemDraging			= FALSE;
	m_siReceivedItemCount	= 0;
	m_siInnStatus			= -1;


	m_CharPosition_X = 0;
	m_CharPosition_Y = 0;

	m_siFieldHp		 = 100;

	m_bGuildOn = TRUE;
	m_bMiniGameOn = FALSE;

	bReturnKeyDown = FALSE;
	m_bFollowerSelLock = FALSE;

	m_siNationCode = ON_COUNTRY_KOREA;

	////////////////////////
	pOnlineLogInMgr				= NULL;
	pOnlineLoadingData			= NULL;
	m_ReceivedItemAniSpr.Image  = NULL;
//	m_Spr_Icon_DokdoQuest.Image	= NULL;	// actdoll (2004/04/02 15:28) : = ÀÌº¥Æ® = µ¶µµ Äù½ºÆ®¿ë ÀÌ¹ÌÁö ¾ÆÀÌÄÜ

//	m_IME_DLLHandle = NULL;
	
	// actdoll (2004/03/30 11:02) : = ÀÌº¥Æ® = µ¶µµÄù½ºÆ® ÃÊ±âÈ­
//	m_uiDokdoQuestCompleteUserCount	= 0;

	// °ø¼ºÀü À¯´ÖÀÏ °æ¿ì °ø¼ºÀü À¯´ÖÀÇ »ç¿ëÀÚ ÀÌ¸§, Ã¼·Â, º¸±ÞÇ°, µîÀ» º¼ ¼ö ÀÖ´ÂÁö¸¦ ³ªÅ¸³»´Â ÇÃ·¡±×
	m_scIsShowInformationAboutSiegeWarfareUnit = 0;

	m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_NONE;
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÀÓÁø·Ï ¿Â¶óÀÎ ¿ùµå ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
cltOnlineWorld::~cltOnlineWorld()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÀÓÁø·Ï ¿Â¶óÀÎ ¿ùµå ÃÊ±âÈ­.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::Initialize()
{
	PaletteWhiteColor           =   RGB(255, 255, 255);
	fYetLight					=	0.0f;

	m_GMFlag                    =   ON_USERGRADE_NORMAL;
	m_InfoFlag                  =   TRUE;
	m_ShowMessagBoxCharInfoFlag =   FALSE;
//	m_LanguageMode              =   FALSE;
	m_fChattingRoom             =   FALSE;
	bPay						=	FALSE;
	bTempFlag					=	FALSE;
	m_bForcedLogout				=	FALSE;
	bPeace						=	TRUE;
	bFirstLogIn					=	FALSE;
	m_VillageAttackDelay        =   0;

	m_GameTickCount             = 0;
	m_WaitTickCount				= 0;
	m_GameTime		            = 0;
	m_dwGameGuardCheckTick		= 0;

	m_HackGameTickCount         = 0;
	m_HackGameTime				= 0;
	m_TickTimeDiff				= 0;

	m_LastNowUserConnectMessageTickCount        = 0;
	m_LastNowUserConnectMessageNowUser          = 0;
	m_LastNowUserConnectMessageErrorOutPutCount = 0;

	dwMapTileDrawTime         = 0;
	siMapTileDrawTimeAddTime  = 0;
	siMapTileDrawTimeAddCount = 0;

	m_siFieldHp				  = 100;

	uiTotalFrame				=	0;
	dwCurrentFrameStartTime		=	0;
	dwCurrentFrameAddTime		=	0;
	dwCurrentFrameSkippingCount	=	0;
	siGamePollStatus			=	0;	
	siMainPollStatus			=	ON_MAINPOLL_LOGINMGR;
	BackgroundMusic				=	0;
	siTalkDealy					=	0;
	m_siNoField					=	-1;
	fLeftUpSwitch				=	FALSE;
	fRightUpSwitch				=	FALSE;
	fLeftOneSwitch				=	FALSE;
	fVillageRightOneSwitch		=	FALSE;
	fVillageLeftOneSwitch		=	FALSE;
	fRightOneSwitch				=	FALSE;
	fYetLeftPressSwitch			=	FALSE;
	fYetRightPressSwitch		=	FALSE;
	ShortKey					=	FALSE;
	bServerSelect				=	FALSE;
	siTempSearchNum				=	0;

	// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤ º¯¼ö
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	// ¹öÁ¯À» ¸¸µç´Ù.
	siOnlineMajorVersion		=	ON_CV_MAJOR;
	siOnlineMinorVersion		=	ON_CV_MINOR;

	bExitOnlineGame				=	FALSE;
	bFindTrade					=	FALSE;

	ZeroMemory( m_TempBuffer, sizeof( m_TempBuffer ) );
	wsprintf(szOnlineVersion, "Ver %d.%03d", siOnlineMajorVersion, siOnlineMinorVersion / 10);

	ZeroMemory(m_MemoUser, 32);

	bCharMove		= TRUE;

	// actdoll (2004/04/02 15:28) : = ÀÌº¥Æ® = µ¶µµ Äù½ºÆ® °ü·Ã
//	m_bEventDokdoQuestPlay	= FALSE;	
//	if(m_Spr_Icon_DokdoQuest.Image)  {clGrp.FreeXspr(m_Spr_Icon_DokdoQuest);  m_Spr_Icon_DokdoQuest.Image  = NULL;}

	// ÁÖ¸· µµÂøÇÑ ¾ÆÀÌÅÛ °ü·Ã
	m_bReceivedItem			= FALSE;
	m_bItemDraging			= FALSE;
	m_siReceivedItemCount	= 0;
	m_siInnStatus			= -1;
	if(m_ReceivedItemAniSpr.Image)  {clGrp.FreeXspr(m_ReceivedItemAniSpr);  m_ReceivedItemAniSpr.Image  = NULL;}
		

	pOnlineCharUI->Init(this);

#ifdef _OGG_PLAY
	// actdoll (2004/11/25 13:35) : °¨¸¶´Ï¾Æ Á¦ÆÒÀÇ ¹®Á¦·Î ´çºÐ°£ º¸·ù
//	if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_JAPAN)
//		GetBgmMgr()->Play(OGGBGM_FILENAME_GERSANGTITLE_JAPAN, TRUE);
//	else
		GetBgmMgr()->Play(OGGBGM_FILENAME_GERSANGTITLE, TRUE);
#else
	if(g_pSoundManager)
	{
		g_pSoundManager->StopAllSound();
		
		BackgroundMusic	=	g_pSoundManager->AddPlayID( "music\\GersangTitle.YAV", FALSE, TRUE, FALSE );
		g_pSoundManager->PlaySound( BackgroundMusic );	
	}
#endif

	m_CharPosition_X = 0;
	m_CharPosition_Y = 0;

	m_siUpdateCounter = 0;
	m_siUpdateMethod = 0;

	m_bGuildOn = TRUE;
	m_bMiniGameOn = FALSE;
	m_bFollowerSelLock = FALSE;

	m_siNationCode = ON_COUNTRY_KOREA;

	// robypark 2004/10/26 10:55
	// ÀüÅõ¸ðµåÀÎ »óÅÂ·Î ·Î±×¾Æ¿ô ÈÄ¿¡ ÀçÁ¢¼ÓÇÏ¿©µµ È­¸é ÀÎÅÍÆäÀÌ½º¿¡ Ç¥½ÃµÇ´Â ÅØ½ºÆ®°¡
	// ÆòÈ­¸ðµå·Î ÃÊ±âÈ­ µÇ¾úÀ½¿¡µµ ºÒ±¸ÇÏ°í ÀüÅõ¸ðµå·Î Ç¥½ÃµÇ´Â ¹ö±× ¼öÁ¤.
	// ¿ª½Ã, °Å·¡Â÷´Ü ÅØ½ºÆ® ¿ª½Ã ¹®Á¦ ¼öÁ¤
	pOnlinePannel->SetButtonText_PeaceMode(!bPeace);
	pOnlinePannel->SetButtonText_TradeMode(!pOnlineTrade->m_bPermitTrade);

	// °ø¼ºÀü À¯´ÖÀÏ °æ¿ì °ø¼ºÀü À¯´ÖÀÇ »ç¿ëÀÚ ÀÌ¸§, Ã¼·Â, º¸±ÞÇ°, µîÀ» º¼ ¼ö ÀÖ´ÂÁö¸¦ ³ªÅ¸³»´Â ÇÃ·¡±×
	m_scIsShowInformationAboutSiegeWarfareUnit = 0;

	m_sRecoveryMana.m_dwLeftTime	 = m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime;
	m_sRecoveryMana.m_bSendedRequest = FALSE;
}

BOOL	cltOnlineWorld::Init()
{
	SI32	TempType = 1;
	SI32	i;
	SI32	siMapXsize, siMapYsize;	
	UI08	uiPalette[768], uiImjinPalette[768];
	CHAR	szSpriteFileName[1024];
	CHAR	szSmallFileName[1024];
	SI32	siFile;
	XSPR	*pTempSpr;
	SI32	TempColorErrorVal, TempColorErrorVal1;
	SI16	TempCount;

	// actdoll (2004/11/15 18:45) : Ãß°¡
	if( !g_bSkipIntro )		IntroPoll();

	//----------------------------------------------------------------------------
	//	ÇÁ·ÐÆ® ·ÎµùÈ­¸é ÀÌ¹ÌÁö ÁØºñ
	//----------------------------------------------------------------------------

	pOnlineLoadingData = new OnlineLoadingData;
	ZeroMemory( pOnlineLoadingData, sizeof(pOnlineLoadingData) );

	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
	SetGamePalette(uiPalette);

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_bg.Spr", pOnlineLoadingData->BackImage );	
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_bg_over.Spr", pOnlineLoadingData->BackOverImage );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_play_grade.Spr", pOnlineLoadingData->PlayGradeImage );
	if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Gamania.Spr", pOnlineLoadingData->m_Spr_CompanyLogo );	// ÇÑ±¹/Áß±¹/ÀÏº»/´ë¸¸/È«Äá¿ë
	else														clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Bolehcom.Spr", pOnlineLoadingData->m_Spr_CompanyLogo );	// ÀÎµµ³×½Ã¾Æ¿ë

	// actdoll (2004/12/20 19:13) : ·Î°í ¼öÁ¤
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Gersang_Logo_Ani.Spr", pOnlineLoadingData->AniLogoImage );

	m_LanguageCode = pGame->LanguageCode; 

	//----------------------------------------------------------------------------
	//	ÇÁ·ÐÆ® ·ÎµùÈ­¸é ÀÌ¹ÌÁö ÁØºñ ¿Ï·á
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 1´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' ÁÂÃø ¼¼·Îº¯ 1È¹		: START
	//----------------------------------------------------------------------------
	// ¿Â¶óÀÎ¿¡¼­ »ç¿ëÇÏ´Â ÆùÆ®¸¦ »ý¼ºÇÑ´Ù.
	pOnlineFonts	=	new	OnlineFonts;
	if( !pOnlineFonts->Create() )
	{
		clGrp.Error("OnlineFonts Init Error", "");
		delete pOnlineLoadingData;
		pOnlineLoadingData	= NULL;
		return FALSE;
	}

	// ¿Â¶óÀÎ¿¡¼­ Ãâ·Â¿ëÀ¸·Î »ç¿ëÇÏ´Â ÅØ½ºÆ® ¿£ÁøÀ» Çü¼ºÇÑ´Ù.
	pOnlineText				=	new OnlineText;
	if(pOnlineText->Init( (CHAR*)pGame->m_pszHQNation, (CHAR*)( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) ) == FALSE)
	{
		clGrp.Error("OnlineText Init Error", "");
		delete pOnlineText;
		pOnlineText	= NULL;
		return FALSE;
	}
	
	// actdoll (2004/05/04 16:33) : [OECEX]
	// ¿¡µðÆ® ÇÚµé·¯ÀÇ ±âº» ¼¼ÆÃ
	Handler_OnlineEditControlEx::Initialize( pGame->Hwnd );
	Handler_OnlineEditControlEx::SetCandidateFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	// actdoll (2004/11/03 17:10) : IME ÄÁÆ®·Ñ·¯ °ü·Ã
	OnlineInputStatusControl::Init( this );


	pOnlineLoadingData->uiFrame = 0;
	ScrollLoading(0);

	m_bEvent = FALSE;

	pOnlineMsgProc	= new OnlineMsgProc( this );

	m_GameOption = new OnlineGameOption;
	m_GameOption->ReadText("Online\\GameOption.txt");

	pMemoryProtected = new CMemoryProtected;
	pMemoryProtected->Create();

	GsInitBattle();
	//----------------------------------------------------------------------------
	//	·Îµù 1´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' ÁÂÃø ¼¼·Îº¯ 1È¹		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 2´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' »ó´Ü °¡·Îº¯ 1È¹		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(10);

	// ÀÓÁø·Ï ¿Â¶óÀÎ¿¡¼­ ¾²ÀÏ ¸®¼Ò½ºµîÀ» ·ÎµåÇÑ´Ù.	
	pOnlineResource		=	new	OnlineResource;	
	if(pOnlineResource->Init() == FALSE) 
		clGrp.Error("FKJE8567", "Online Resource Init Failed");	
	//----------------------------------------------------------------------------
	//	·Îµù 2´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' »ó´Ü °¡·Îº¯ 1È¹		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 3´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' Áß¾Ó º¯ 2È¹			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(20);

	pOnlineMap	=	new OnlineMap;	
	// ¸ÊÀ» ºÒ·¯¿Â´Ù.
	// ½ÇÁúÀûÀ¸·Î ¸ÊÀ» ¸Þ¸ð¸®¿¡ ·ÎµåÇÏÁö ¾Ê´Â´Ù. ´Ù¸¸ ÇÊ¿äÇÒ¶§ ·ÎµåÇÒ ¼ö ÀÖµµ·Ï È­ÀÏ ÀÌ¸§¸¸ ÀúÀåÇÑ´Ù.	
	if(pOnlineMap->Create(this) == FALSE)
	{
		clGrp.Error("Map Load Error.", "");						
	}		

	m_pOnlineMapSound = new OnlineMapSound();
	ASSERT(m_pOnlineMapSound);
	{
		// ±¹°¡º° À½¾Ç ¼³Á¤ °ü·Ã. 
		// 
		SM_POINT koreaArea[31] = {
			{ 1467, 710 },	{ 1329, 831 },	{ 1189, 1080 },	{ 1012, 1046 },	{ 917,  504 },	
			{ 935, 467 },	{ 943, 439 },	{ 976, 415 },	{ 989, 411 },	{ 1032, 351 },
			{ 1069, 345 },	{ 1072, 325 },	{ 1069, 312  },	{ 1080, 303 },	{ 1143, 329 },
			{ 1118, 323  },	{ 1160, 333  },	{ 1155, 310 },	{ 1162, 305 },	{ 1197, 318 },
			{ 1213, 309 },	{ 1224, 310 },	{ 1237, 303 },	{ 1231, 290 },	{ 1241, 283 },
			{ 1253, 287 },	{ 1277, 304 },	{ 1285, 309 },  { 1290, 320 },  { 1300, 337 },
			{ 1322, 348 },
		};
		SM_POINT koreaAreaPoint = {1142, 637};

		SM_POINT japanArea[7] = {
			{ 1942, 1141 },	{ 1244, 1099 },	{ 1322, 853 },	{ 1457, 719 },	{ 1585, 469 },
			{ 1579, 25 },	{ 1972, 27 }, 
		};
		SM_POINT japanAreaPoint = {1765, 743};

		SM_POINT taiwanArea[6] = {
			{ 548, 1702 },	{ 666, 1471 },	{ 978, 1104 },	{ 1355,1523 },	{ 1229,1961 },
			{ 449, 1950},
		};
		SM_POINT taiwanAreaPoint = {810, 1575};

		m_pOnlineMapSound->Create(pOnlineMap->m_siXsize, pOnlineMap->m_siYsize, 5, SMA_CHINA);
		m_pOnlineMapSound->AddSoundArea(31, koreaArea, koreaAreaPoint, SMA_KOREA);
		m_pOnlineMapSound->AddSoundArea(7, japanArea, japanAreaPoint, SMA_JAPAN);
		m_pOnlineMapSound->AddSoundArea(6, taiwanArea, taiwanAreaPoint, SMA_TAIWAN);
//		soundMap.TestDraw(pDC);
	}

	// ¿Â¶óÀÎ¿¡¼­ »ç¿ëÇÏ´Â ¸ðµç ¸¶¿ì½º¸¦ °ü¸®ÇÏ´Â °´Ã¼¸¦ »ý¼ºÇÑ´Ù.
	pIOnlineMouse	=	new	IOnlineMouse;
	pIOnlineMouse->Init();

	pOnlineMousePoint = new OnlineMousePoint;
	pOnlineMousePoint->Init(pOnlineResource, pOnlineMap);

	pMousePointInfo			=	new OnlineMousePointInfo;
	pMousePointInfo->Init();

	ShowSystemCursor(FALSE);
	pIOnlineMouse->SetMouse(ON_MOUSE_BASIC, TRUE);
	pIOnlineMouse->ShowMouse(TRUE);
	pIOnlineMouse->SetPos(400, 300);

	m_GMFlag                    = ON_USERGRADE_NORMAL;
	m_InfoFlag                  = TRUE;
	m_ShowMessagBoxCharInfoFlag = FALSE;
//	m_LanguageMode              = FALSE;
	m_VillageAttackDelay        =   0;
	
#ifdef _OGG_PLAY
	// actdoll (2004/11/25 13:35) : °¨¸¶´Ï¾Æ Á¦ÆÒÀÇ ¹®Á¦·Î ´çºÐ°£ º¸·ù
//	if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_JAPAN)
//		GetBgmMgr()->Play(OGGBGM_FILENAME_GERSANGTITLE_JAPAN, TRUE);
//	else
	GetBgmMgr()->Play(OGGBGM_FILENAME_GERSANGTITLE, TRUE);
#else
	if(g_pSoundManager)
	{
		g_pSoundManager->StopAllSound();
		
		BackgroundMusic	=	g_pSoundManager->AddPlayID( "music\\GersangTitle.YAV", FALSE, TRUE, FALSE );
		g_pSoundManager->PlaySound( BackgroundMusic );	
	}
#endif
	
	GD.Init();					// °ÔÀÓ µ¥ÀÌÅ¸ ÃÊ±âÈ­	
	//----------------------------------------------------------------------------
	//	·Îµù 3´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' Áß¾Ó º¯ 2È¹			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 4´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' ÇÏ´Ü º¯ 1È¹ ±ÛÀÚ ¿Ï¼º	: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(30);

	/*********************************************************************************************************************
									·Îµù ½ºÇÁ¶óÀÌÆ®¸¦ ·ÎµåÇÏ°í ÆÈ·¹Æ®¸¦ ·Îµå ÇÑ´Ù.
	*********************************************************************************************************************/	
	// ¿Â¶óÀÎ¿¡¼­ »ç¿ëÇÏ´Â ÆùÆ®¸¦ »ý¼ºÇÑ´Ù.
//	pOnlineFonts	=	new	OnlineFonts;
//	pOnlineFonts->Create();

	// ¿Â¶óÀÎ¿¡¼­ Ãâ·Â¿ëÀ¸·Î »ç¿ëÇÏ´Â ÅØ½ºÆ® ¿£ÁøÀ» Çü¼ºÇÑ´Ù.
//	pOnlineText				=	new OnlineText;
//	if(pOnlineText->Init( (CHAR*)pGame->m_pszHQNation ) == FALSE)
//		clGrp.Error("OnlineText Init Error", "");	

	m_cltIGuildClass.Init("Online\\Village\\grade.txt");

	/*********************************************************************************************************************
									Palette Manager¸¦ ¼³Á¤ÇÑ´Ù.
	*********************************************************************************************************************/
	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiImjinPalette);

	pPaletteManager = new OnlinePaletteManager;
	pPaletteManager->Init(uiImjinPalette, 128);
	PaletteDataRed			= pPaletteManager->MakePalette(RGB(255, 0, 0));
	PaletteDataWhiteRed		= pPaletteManager->MakePalette(RGB(255, 128, 128));
	PaletteDataWhiteBlue	= pPaletteManager->MakePalette(RGB(70, 255, 80));
	PaletteDataYellow		= pPaletteManager->MakePalette(RGB(240, 240, 0));
	PaletteDataGreen		= pPaletteManager->MakePalette(RGB(0, 0, 255));
	PaletteDataBlack		= pPaletteManager->MakePalette(RGB(0, 0, 0));

	// Find White Color
	PaletteWhiteColorIndex = 255;
	TempColorErrorVal      = 100000000;
	for(TempCount = 1; TempCount < 254; TempCount++)
	{
		TempColorErrorVal1 =  (abs(63 - (SI32)uiImjinPalette[(TempCount * 3)]))
							+ (abs(63 - (SI32)uiImjinPalette[(TempCount * 3) + 1]))
							+ (abs(63 - (SI32)uiImjinPalette[(TempCount * 3) + 2]));

		if(TempColorErrorVal1 < TempColorErrorVal)
		{
			PaletteWhiteColorIndex = (UI08)TempCount;
			TempColorErrorVal      = TempColorErrorVal1;
		}
	}

	ZeroMemory(PaletteNight, 768 * MAX_NIGHT);
	CopyMemory(PaletteNight[0], uiImjinPalette, 768);
	clGrp.LoadOnlyPaletteFile("pal\\night1.pal", PaletteNight[1]);
	for(TempCount = 0; TempCount < 256; TempCount++)
	{
		PaletteNight[1][(TempCount * 3)    ] = ((BYTE)((float)PaletteNight[1][(TempCount * 3)    ] * 0.95f));
		PaletteNight[1][(TempCount * 3) + 1] = ((BYTE)((float)PaletteNight[1][(TempCount * 3) + 1] * 0.95f));
		PaletteNight[1][(TempCount * 3) + 2] = ((BYTE)((float)PaletteNight[1][(TempCount * 3) + 2] * 1.0f));
	}
	clGrp.LoadOnlyPaletteFile("pal\\night2.pal", PaletteNight[2]);
	for(TempCount = 0; TempCount < 256; TempCount++)
	{
		PaletteNight[2][(TempCount * 3)    ] = ((BYTE)((float)PaletteNight[2][(TempCount * 3)    ] * 0.90f));
		PaletteNight[2][(TempCount * 3) + 1] = ((BYTE)((float)PaletteNight[2][(TempCount * 3) + 1] * 0.90f));
		PaletteNight[2][(TempCount * 3) + 2] = ((BYTE)((float)PaletteNight[2][(TempCount * 3) + 2] * 0.95f));
	}
	clGrp.LoadOnlyPaletteFile("pal\\night3.pal", PaletteNight[3]);
	for(TempCount = 0; TempCount < 256; TempCount++)
	{
		PaletteNight[3][(TempCount * 3)    ] = ((BYTE)((float)PaletteNight[3][(TempCount * 3)    ] * 0.85f));
		PaletteNight[3][(TempCount * 3) + 1] = ((BYTE)((float)PaletteNight[3][(TempCount * 3) + 1] * 0.85f));
		PaletteNight[3][(TempCount * 3) + 2] = ((BYTE)((float)PaletteNight[3][(TempCount * 3) + 2] * 0.90f));
	}
	clGrp.LoadOnlyPaletteFile("pal\\night4.pal", PaletteNight[4]);
	for(TempCount = 0; TempCount < 256; TempCount++)
	{
		PaletteNight[4][(TempCount * 3)    ] = ((BYTE)((float)PaletteNight[4][(TempCount * 3)    ] * 0.80f));
		PaletteNight[4][(TempCount * 3) + 1] = ((BYTE)((float)PaletteNight[4][(TempCount * 3) + 1] * 0.80f));
		PaletteNight[4][(TempCount * 3) + 2] = ((BYTE)((float)PaletteNight[4][(TempCount * 3) + 2] * 0.85f));
	}

	// actdoll (2004/02/23 15:55) : 
	//	´Ù¸¥ ¹æ½ÄÀ¸·Î ÇØ´ç Á¤º¸¸¦ ·ÎµùÇÑ´Ù.
	m_SlanderFilter.LoadReferenceWord( (CHAR*)pGame->m_pszHQNation, ( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) );
	//----------------------------------------------------------------------------
	//	·Îµù 4´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ËÝ' ÇÏ´Ü º¯ 1È¹ ±ÛÀÚ ¿Ï¼º	: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 5´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' »ó´Ü Á¡ 1È¹			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(40);

	/*********************************************************************************************************************
									TicketParser¸¦ ¼³Á¤ÇÑ´Ù.
	*********************************************************************************************************************/
	pTicketParser               =   new OnlineTicketParser;
	pTicketParser->ReadText("Online\\Village\\TicketInfo.txt");

	/*********************************************************************************************************************
									GeneralParser¸¦ ¼³Á¤ÇÑ´Ù.
	*********************************************************************************************************************/
	pGeneralParser               =   new OnlineGeneralParser;
	pGeneralParser->ReadText("Online\\CharInfo\\General_list.txt");

	/*********************************************************************************************************************
									GeneralParserEx¸¦ ¼³Á¤ÇÑ´Ù.(2Â÷ ÀüÁ÷ ¿ëº´)
	*********************************************************************************************************************/
	pGeneralParserEx               =   new OnlineGeneralParser;
	pGeneralParserEx->ReadText("Online\\CharInfo\\GeneralListEx.txt");

	/*********************************************************************************************************************
									QuestParser¸¦ ¼³Á¤ÇÑ´Ù.
	*********************************************************************************************************************/
	pQuestParser				=	new OnlineQuestParser;
	// robypark 2004/9/9 17:53
	// ÀÌº¥Æ®¿ë Äù½ºÆ® °ü·Ã ÀÎÀÚ Ãß°¡
	char szHQNation[128];
	sprintf(szHQNation, "%s", (char*)pGame->m_pszHQNation);
	pQuestParser->ReadText("Online\\Quest\\quest.txt", szHQNation, 2004, 9, 13);

	/*********************************************************************************************************************
									LevelMark Parser¸¦ ¼³Á¤ÇÑ´Ù.
	*********************************************************************************************************************/
	pLevelMarkParser				=	new OnlineLevelMarkParser;
	pLevelMarkParser->Create("Online\\CharInfo\\LevelMark.txt");

	/*********************************************************************************************************************
									Timer¸¦ ¼³Á¤ÇÑ´Ù.
	*********************************************************************************************************************/
	SOnlineTime		Time;
	pTimer = new COnlineTimer;
	Time.Year  = 1;
	Time.Month = 1;
	Time.Day   = 1; 
	Time.Hour  = 1;
	pTimer->Create(Time);
	m_GameTickCount = 0;
	fFirstGetTime   = FALSE;

	pCharFileLoadManager	=	&g_FileLoadManager;

	pOnlineMercenaryParser = new OnlineMercenaryParser;
	pOnlineMercenaryParser->ReadText("Online\\Village\\mercenary.txt");

	/*********************************************************************************************************************
									ÅëÀå¿¡ °üÇÑ ¼³Á¤
	*********************************************************************************************************************/	
	pOnlineEconomy = new OnlineEconomy;
	pOnlineEconomy->Init("Online\\Village\\Economy.txt");

	// ´ÙÀÌ·ºÆ® µå·Î¿ì °´Ã¼¸¦ »ý¼ºÇÑ´Ù. ¹Ýµå½Ã ¸®¼Ò½º¸¦ ·ÎµåÇÑ ÈÄ È£ÃâÇØ ÁÖ¾î¾ß ÇÑ´Ù.
	if(clGrp.CreateOffScreen(pOnlineDirectDraw, ON_SCREEN_XSIZE, ON_SCREEN_YSIZE, TRUE) == FALSE)
	{
		clGrp.Error("FKJE8567", "Online DirectDraw Create Failed");	
		return FALSE;
	}
	
	// ¸Ê Á¾·ù¸¦ ÃÊ±âÈ­ÇÑ´Ù.

//	DrawTextInLoading("¸¶À» °ü¸®ÀÚ »ý¼º Áß");
	// ¸¶À» °ü¸®ÀÚ¸¦ »ý¼ºÇÑ´Ù.
	pOnlineVillageManager	=	new OnlineVillageManager;

	if(!pOnlineVillageManager->Init("Online\\Village\\VillageInfo.txt", "Online\\Village\\Structure Placement.txt", this))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_VILLAGE_MANAGER_INIT_ERROR), "");						
	}		
	m_fChattingRoom             =   FALSE;

	// ¸¶¿ï ¼³¸í List »ý¼º
//	DrawTextInLoading("¸¶À» ¾È³»¹® °ü¸®ÀÚ »ý¼º Áß");
	pOnlineVillageDescriptList = new OnlineVillageDescriptList;
	pOnlineVillageDescriptList->Init(2, 16);

	// °øÁö»çÇ× List
	pOnlineNoticeList = new OnlineNoticeList;
	pOnlineNoticeList->Init(this, 10, 5, 233, 582, 288, 4);

	// ÇÊµå»ó Effect
	pOnlineFieldEffect = new OnlineFieldEffect;
	pOnlineFieldEffect->Init(this, 64);

	// ½ºÅ³
	pOnlineSkillParser = new OnlineSkillParser;
	pOnlineSkillParser->Init();
	//----------------------------------------------------------------------------
	//	·Îµù 5´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' »ó´Ü Á¡ 1È¹			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 6´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' »ó´Ü °¡·Îº¯ 1È¹		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(50);

	// ¿ÀºêÁ§Æ® ÀÎÅÍÆäÀÌ½º¸¦ »ý¼ºÇÑ´Ù.

	pIOnlineObject	=	new	IOnlineObject;

	// ¿ÀºêÁ§Æ®À» ÃÊ±âÈ­ ÇÑ´Ù.
	if(!pIOnlineObject->Init())
	{
		clGrp.Error("FKJE8567", pOnlineText->Get(ON_TEXT_OBJECT_INIT_ERROR));
	}	

	// Å°º¸µå ÀÔ·ÂÀ» Ã³¸®ÇÏ´Â Å¬·¡½º¸¦ »ý¼ºÇÑ´Ù.
	pOnlineKeyboard	=	new	OnlineKeyboard;
	pOnlineKeyboard->Init();

	// Ä³¸¯ÅÍÀÇ ¹®ÀÚ¿­ ¾×¼Ç ÄÚµå¸¦ ¼ýÀÚ ¾×¼Ç ÄÚµå·Î º¯È¯ÇØ ÁÖ´Â °´Ã¼¸¦ »ý¼ºÇÑ´Ù.
	// Ä³¸¯ÅÍÀÇ ¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ ÃÊ±âÈ­ ÇÏ±âÀü¿¡ ÇØÁÖ¾î¾ß ÇÑ´Ù.
	pOnlineCharActionCode	=	new OnlineCharActionCode;
	
	// Ä³¸¯ÅÍÀÇ ¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ ÃÊ±âÈ­ ÇÑ´Ù. ¹Ýµå½Ã Ä³¸¯ÅÍ¸¦ ÃÊ±âÈ­ ÇÏ±â ÀÌÀü¿¡ ÇØÁÖ¾î¾ß ÇÑ´Ù.	
//	pOnlineCharAni	=	new OnlineCharAni;

	//clGrp.Error("", "pOnlineCharAni->Init 1");
/*	if(!pOnlineCharAni->Init(this))
	{
		clGrp.Error("Ä³¸¯ÅÍ ¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ ÃÊ±âÈ­ ÇÏ´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù.", "");
	}*/
	//clGrp.Error("", "pOnlineCharAni->Init 2");

	// ¹è Á¾·ù¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
//	clGrp.Error("", "pIOnlineShipKI	=	new IOnlineShipKI 1");
	pIOnlineShipKI	=	new IOnlineShipKI;
//	clGrp.Error("", "pIOnlineShipKI	=	new IOnlineShipKI 2");

//	clGrp.Error("", "pIOnlineShipKI->Init() 1");
	if(!pIOnlineShipKI->Init())
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_SHIP_KIND_INIT_ERROR), "");
	}
//	clGrp.Error("", "pIOnlineShipKI->Init() 2");

	// Ä³¸¯ÅÍ Á¾·ù¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
	pIOnlineCharKI	=	new	IOnlineCharKI;

	if(!pIOnlineCharKI->Init(this))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_CHAR_KIND_INIT_ERROR), "");
	}
	//----------------------------------------------------------------------------
	//	·Îµù 6´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' »ó´Ü °¡·Îº¯ 1È¹		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 7´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' »óÁß´Ü Á¡ 2È¹			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(60);

//	DrawTextInLoading("¿ÀºêÁ§Æ® Á¾·ù ÃÊ±âÈ­, Ä³¸¯ÅÍ »ý¼º, ¸¶À» »ý¼º Áß");
	// ¿ÀºêÁ§Æ® Á¾·ù¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
	pIOnlineObjectKI	=	new IOnlineObjectKI;

	if(!pIOnlineObjectKI->Init(pOnlineResource, pOnlineText))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_OBJECT_KIND_INIT_ERROR), "");
	}	

	// Ä³¸¯ÅÍ ÀÎÅÍÆäÀÌ½º¸¦ »ý¼ºÇÑ´Ù.
	pIOnlineChar	=	new	IOnlineChar;

	// Ä³¸¯ÅÍ¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
	if(pIOnlineChar->Init(this) == FALSE)
	{
		clGrp.Error("FKJE8567", pOnlineText->Get(ON_TEXT_CHAR_INIT_ERROR));
	}

	// °¥¯±â ÀÎ°øÁö´ÉÀ» »ý¼ºÇÑ´Ù.
	pclFindPath	=	new cltOnlineFindPath(ON_MAP_XSIZE, ON_MAP_YSIZE, pOnlineMap);
	
	pOnlineMap->GetSize(&siMapXsize, &siMapYsize);
	pclFindPath->SetFindPathSize(siMapXsize, siMapYsize);		
	//----------------------------------------------------------------------------
	//	·Îµù 7´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' »óÁß´Ü Á¡ 2È¹			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 8´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' ÁÂÃø ¼¼·Îº¯ 1È¹		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(70);

	// ³ªÀÇ µ¥ÀÌÅ¸¸¦ »ý¼ºÇÑ´Ù.
	pMyData		=	new	MyData;
	pMyData->Init(this);

	// ³» Ä³¸¯ÅÍ¸¦ Á¶ÀÛÇÏ±â À§ÇÑ °´Ã¼¸¦ »ý¼ºÇÑ´Ù.	
	pOnlineCharUI	=	new	OnlineCharUI;
	pOnlineCharUI->Init(this);	
	
	dwCurrentFrameStartTime		=	0;
	dwCurrentFrameAddTime		=	0;
	dwCurrentFrameSkippingCount	=	0;
	siMainPollStatus			=	ON_MAINPOLL_LOGINMGR;	

	// ÀÌ¸ðÆ¼†ö Ã³¸®
	pOnlineImoticon = new OnlineImoticon;
	pOnlineImoticon->Init(this);

	// ÀÎº¥Åä¸®¸¦ »ý¼ºÇÑ´Ù. ¿©±â¼­´Â »ý¼º¸¸ ÇØÁÖ°í ÃÊ±âÈ­´Â ³ªÀÇ µ¥ÀÌÅ¸¸¦ ¹ÞÀº ÀÌÈÄ¿¡ ÇØÁØ´Ù.
	pOnlineInventory		=	new	OnlineInventory;
	pOnlineTradeBook		=	new OnlineTradeBook;
	pOnlineTradeBook->Init( this );

	// ¿Â¶óÀÎ¿¡¼­ »ç¿ëÇÏ´Â ¸ðµç ¾ÆÀÌÅÛÀ» ·ÎµåÇÑ´Ù.		
	pItemsInfo				=	new CItem;		

	pItemsInfo->ItemLoad(	pOnlineText, 
							"Online\\Item\\weapon.txt", "Online\\Item\\armor.txt", "Online\\Item\\accessary.txt", "Online\\Item\\Animal.txt", 
							"Online\\Item\\Dress.txt", "Online\\Item\\Element.txt", "Online\\Item\\Food.txt", "Online\\Item\\Liquor.txt", 
							"Online\\Item\\Medicine.txt", "Online\\Item\\Tool.txt", "Online\\Item\\Trade.txt", "Online\\Item\\Specific.txt" ,
							"Online\\Item\\Imperium.txt");

	g_pItem	=	pItemsInfo;

	for( i=0 ; i<(SI32)pItemsInfo->GetCount(); ++i)											// ¸®¼Ò½º·Î º¯È¯
	{
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );
				
		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			if( pOnlineResource->GetItemFileIndex( szSpriteFileName ) == -1 )					// Å« ±×¸²À» ·ÎµåÇÑ´Ù.( ±×¸²ÀÌ ·Îµå°¡ µÅ¾î ÀÖÁö ¾Ê´Ù¸é ·ÎµåÇÔ)			
				pOnlineResource->AddItemSpr( szSpriteFileName );									

			if( pOnlineResource->GetItemFileIndex( szSmallFileName ) == -1 )					// ÀÛÀº ±×¸²À» ·ÎµåÇÑ´Ù.			
				pOnlineResource->AddItemSpr( szSmallFileName );			
		}		

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// robypark 2004/11/24 16:55
	// °ø¼ºÀü ½Ã°£ µîÀÇ ¿É¼Ç ÆÄÀÏ HQ¿¡¼­ ÀÐ±â
	LoadSOptionSiegeWarfare(szHQNation, pGame->m_iServiceType, "Online\\OptionSiegeWarfare.txt");

	// robypark 2005/1/14 17:11
	// ÁÖÀÎ°ø Ä³¸¯ÅÍµéÀÇ Á¤º¸ °ü¸®ÀÚ »ý¼º ¹× ·Îµå
	m_pFieldHeroInfoMgr = new CFieldHeroInfoMgr;
	m_pFieldHeroInfoMgr->LoadFieldHeroInfo();

	// robypark 2005/1/31 12:55
	// °ø¼º À¯´Ö °ü¸®ÀÚ
	m_pSiegeUnitTableMgr = new CSiegeUnitTableMgr;
	m_pSiegeUnitTableMgr->LoadSiegeUnitTable();			// °ø¼º À¯´Ö ±âº» Å×ÀÌºí
	m_pSiegeUnitTableMgr->LoadSiegeUnitTableCalcHP();	// ÀüÅõ·Â¿¡ µû¸¥ Ã¼·Â °è»ê Å×ÀÌºí
	m_pSiegeUnitTableMgr->LoadSiegeUnitTableCalcMP();	// ÀüÅõ·Â¿¡ µû¸¥ ¸¶·Â °è»ê Å×ÀÌºí

	//----------------------------------------------------------------------------
	//	·Îµù 8´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' ÁÂÃø ¼¼·Îº¯ 1È¹		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 9´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' Áß¾Ó º¯ 2È¹			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(80);

	for( i=0; i<(SI32)pItemsInfo->GetCount() ; ++i)												// ´Ù½¬ ÇÑ¹ø ¾ÆÀÌÅÛ ±¸Á¶Ã¼¿¡ ¸®¼Ò½º ÀÎµ¦½º¸¦ ³Ö¾î ÁØ´Ù.
	{		
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );					// ¾ÆÀÌÅÛ ÆÄÀÏÀ» ¹Þ´Â´Ù.

		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			// Å« ±×¸²
			siFile		=	pOnlineResource->GetItemFileIndex( szSpriteFileName );			// ¸®¼Ò½º¿¡¼­ ÆÄÀÏ ÀÌ¸§³Ö¾î ÀÎµ¦½º¸¦ ¹Þ´Â´Ù.
			pTempSpr	=	pOnlineResource->GetxsprItem( siFile );

	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSpriteFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, FALSE );
			else					clGrp.Error("", pOnlineText->Get(ON_TEXT_ITEM_SEARCH_ERROR), szSpriteFileName );

			// ÀÛÀº ±×¸²
			siFile		=	pOnlineResource->GetItemFileIndex( szSmallFileName );				// ¸®¼Ò½º¿¡¼­ ÆÄÀÏ ÀÌ¸§³Ö¾î ÀÎµ¦½º¸¦ ¹Þ´Â´Ù.
			pTempSpr	=	pOnlineResource->GetxsprItem( siFile );
			
	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSmallFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, TRUE );
			else					clGrp.Error("", pOnlineText->Get(ON_TEXT_ITEM_SEARCH_ERROR), szSmallFileName );
		}

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// Config ÆÄÀÏ¿ë
	pConfig					=	new OnlineConfig;
	pConfig->OpenConfig();
	if(LogInID[0]) pConfig->SetConfig( LogInID );

	// Mega Text¿ë	
	pOnlineMegaText			=	new	OnlineMegaText;	
	pOnlineMegaText->Initialize(pOnlineResource, pOnlineFonts);	

	pOnlineBuilding			=	new OnlineBuilding;
	if( pOnlineBuilding->Init() == FALSE )
		clGrp.Error("", pOnlineText->Get(ON_TEXT_STRUCT_INIT_ERROR) );

	// ÇÊµå Áö¿ª °ü¸® °´Ã¼.
	pOnlineFieldArea		=	new OnlineFieldArea;
	pOnlineFieldArea->Init();	

	// ÆÇ³ÚÀ» ÃÊ±âÈ­ ÇÑ´Ù.	
	pOnlinePannel			=	new OnlinePannel;
	pOnlinePannel->Init(this);	

	pIProfit				=	new cltIProfit;
	pIProfit->Init( "Online\\Village\\Profit.txt" );

	// ¹Ì´Ï¸ÊÀ» ·ÎµåÇÔ
	pOnlineMiniMap			=	new OnlineMiniMap;
	pOnlineMiniMap->Init(this);

	pOnlineHelp				=	new OnlineHelp;
	pOnlineHelp->Init(this);
	
	pOnlineClient	=	new	OnlineClient;
	pOnlineClient->InitZero();	
	pOnlineClient->InitSocket();					// ÇÑ¹ø¸¸ ½ÇÇà

	// RequestPlayerInfo º¯¼ö¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
	RPI.Init(pOnlineClient);

	pOnlineNumPad					=	new OnlineNumPad;
	pOnlineNumPad->pNumberControl	=	new OnlineNumberControl;
	pOnlineNumPad->pNumberControl->Init( &pOnlineNumPad->siCount, this); 
	pOnlineNumPad->fDisable = 0;

	pOnlineMsgBox					=	new OnlineMsgBox; 
	pOnlineMsgBox->Init( this );

	pOnlineMsgBoxPay				=	new OnlineMsgBox;
	pOnlineMsgBoxPay->Init( this );

	pOnlineMsgBoxUsePortal			=	new OnlineMsgBox;
	pOnlineMsgBoxUsePortal->Init( this );

	pOnlineMsgBoxError			    =	new OnlineMsgBox;
	pOnlineMsgBoxError->Init( this );

	pOnlineLoginMsgBox              =   new OnlineMsgBox;
	pOnlineLoginMsgBox->Init( this ,TRUE);
	//----------------------------------------------------------------------------
	//	·Îµù 9´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' Áß¾Ó º¯ 2È¹			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 10´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' ¾ÈÂÊ º¯ 5È¹ ±ÛÀÚ ¿Ï¼º	: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(90);

	pOnlineTrade					=	new OnlineTrade;
	pOnlineTrade->Init( this );

	pOnlineBooth					=	new OnlineBooth;
	pOnlineBooth->Init( this );

	pOnlineBench					=	new OnlineBench[ 128 ];
	ZeroMemory( pOnlineBench, sizeof( OnlineBench ) * 128 );

	pOnlineTip						=	new	OnlineTip;
	pOnlineTip->Init( this );

	pOnlineQuestPlay				=	new OnlineQuestPlay;
	pOnlineQuestPlay->Init(this);

//	pOnlineBasicTip					=	new OnlineBasicTip;		// actdoll (2004/02/16 16:11) : »ç¿ëÇÏ´Â °÷ÀÌ ¾øÀ¸¸ç ÀüºÎ È®ÀÎÇß½À´Ï´Ù.
//	pOnlineBasicTip->Init(this);

//	pOnlineBasicQuest				=	new OnlineBasicQuest;	// actdoll (2004/02/16 16:11) : »ç¿ëÇÏ´Â °÷ÀÌ ¾øÀ¸¸ç ÀüºÎ È®ÀÎÇß½À´Ï´Ù.
//	pOnlineBasicQuest->Init(this);

	pOnlineParty					=   new OnlineParty;
	pOnlineParty->Init(this);

	pOnlineMessenger				=	new	OnlineMessenger;
	pOnlineMessenger->Init( this );

	pOnlineDungeon					=	new OnlineDungeonParser;
	pOnlineDungeon->Init( pOnlineText );

	pOnlineSetting					=	new OnlineSetting;
	pOnlineSetting->Init( this );


	m_pChatMacro					=	new OnlineChatMacro(this);
	m_pQuestInterface				=	new OnlineQuestInterface(this);

	pOnlineClientChatting	=	new	HQClientSocket;
	pOnlineClientChatting->Create(8192, 8192);	

	pOnlineTradeParser = new OnlineTradeParser;
	
	pCBuriedItemParser = new CBuriedItemParser;
	pCBuriedItemParser->readFile("Online\\Item\\ItemforField.txt");

//	pOnlineBanner = new OnlineBanner;
//	pOnlineBanner->Init(this);

	pOnlineSmithParser = new OnlineSmithParser;
	pOnlineSmithParser->ReadText("Online\\Item\\Weapon_process.txt");

	m_pNpcTalk		= new OnlineNPCTalk(this);

	// actdoll (031229) : ÃÊ»ó Å¬·¡½º¸¦ ÃÊ±âÈ­ÇÑ´Ù. ÀÏ´Ü Àá°¡³õ°Ú´Ù.
	m_pPortrait		= new OnlinePortrait;
	
	m_pMercenary	= new OnlineMercenary( this, m_pPortrait );

//	pOnlineMemMgr = new MemMgr(10, 500);

	fLeftUpSwitch				=	FALSE;
	fRightUpSwitch				=	FALSE;
	fLeftOneSwitch				=	FALSE;
	fRightOneSwitch				=	FALSE;
	fVillageLeftOneSwitch		=	FALSE;
	fVillageRightOneSwitch		=	FALSE;
	fYetLeftPressSwitch			=	FALSE;
	fYetRightPressSwitch		=	FALSE;

	fYetLight					=	0.0f;

	ZeroMemory(m_MemoUser, 32);

	bCharMove                                   = TRUE;
	m_LastNowUserConnectMessageTickCount        = 0;
	m_LastNowUserConnectMessageNowUser          = 0;
	m_LastNowUserConnectMessageErrorOutPutCount = 0;

	pMyData->GetFollowersInterface()->InitStaticVariable(pItemsInfo);

	pOnlineVillageInfoData = new OnlineVillageInfoData;
	ZeroMemory(pOnlineVillageInfoData, sizeof(OnlineVillageInfoData));

	pOnlineFieldAuction = new OnlineFieldAuction;
	pOnlineFieldAuction->Init(this);

	pOnlineReception    = new OnlineReception;
	pOnlineReception->Init(this);

	pOnlineSmith    = new OnlineSmith;
	pOnlineSmith->Init(this);
	
	//////////////////////////////////////////////////
	// ¸ÅÅ©·Î ÇÁ·Î¼¼¼­
//	m_pMacroProcess = new CMacroCheckProcess(static_cast<COnlineClientCallback*>(pOnlineClient));
//	m_pMacroProcess->StartThread();

//ÁØ¼®ÀÌÇü Ãß°¡ºÎºÐ 
//	pJSWndMgr = new JSWndMgr(this, 1);
//	pJSTestWnd = new JSTestWnd;

	
	pGame->ChatInit();
	//----------------------------------------------------------------------------
	//	·Îµù 10´Ü°è	- "ËÝßÂ" ±ÛÀÚ Áß 'ßÂ' ¾ÈÂÊ º¯ 5È¹ ±ÛÀÚ ¿Ï¼º	: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	·Îµù 11´Ü°è	- "ËÝßÂ" ±ÛÀÚ								: START
	//----------------------------------------------------------------------------
	// Loading11
	ScrollLoading(100);
	
	//µðÆúÆ® Ä³¸¯ÅÍ ÃÊ±âÈ­.
	pCharFileLoadManager->SetDefaultChange(1, 1, 59,  1, 1,	"KD01");
	pCharFileLoadManager->SetDefaultChange(1, 1, 59,  2, 2, "KD01");
	pCharFileLoadManager->SetDefaultChange(2, 2, 59,  3, 3, "JD01");
	pCharFileLoadManager->SetDefaultChange(2, 2, 60,  4, 4, "JD01");
	pCharFileLoadManager->SetDefaultChange(3, 3, 59,  5, 5,	"CD01");
	pCharFileLoadManager->SetDefaultChange(3, 3, 59,  6, 6, "CD01");
	pCharFileLoadManager->SetDefaultChange(48, 25, 59,  7, 7, "TD01");
	pCharFileLoadManager->SetDefaultChange(48, 25, 59,  8, 8, "TD01");

	

	SetForegroundWindow(pGame->Hwnd);

	bPeace = TRUE;
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÀÓÁø·Ï ¿Â¶óÀÎ ¿ùµå.
//	¸®ÅÏ	:	´ÙÀ½ GameStatus °ª.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::Poll()
{
	UI16	uiTempMainPollStatus;
	UI16	uiNextMenu	=	0;

	//============================================================================================
	// ¸¶¿ì½º À§Ä¡Ã³¸®
	POINT		TempPoint;
	GetCursorPos(&TempPoint);
	if (WindowModeSwitch)
	{
		ScreenToClient(pGame->Hwnd,&TempPoint);
		if (bActive) {}
	}
	IpD.Mouse_X = TempPoint.x;
	IpD.Mouse_Y = TempPoint.y;

	GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, 0, TRUE);

	// ¸¶¿ì½º¸¦ ¼¼ÆÃÇÑ´Ù.
	if ((siGamePollStatus != ON_GAMEPOLL_FIELD) && (siGamePollStatus != ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE))
		pIOnlineMouse->Action();
	pIOnlineMouse->SetPos(IpD.Mouse_X, IpD.Mouse_Y);

	// ¸¶¿ì½º Up Check
	fLeftUpSwitch  = FALSE;
	fRightUpSwitch = FALSE;

	if((IpD.LeftPressSwitch  == FALSE) && (fYetLeftPressSwitch  != IpD.LeftPressSwitch))  {fLeftUpSwitch  = TRUE; 	fLeftMemoryUpSwitch  = TRUE;}
	if((IpD.RightPressSwitch == FALSE) && (fYetRightPressSwitch != IpD.RightPressSwitch)) {fRightUpSwitch = TRUE;	fRightMemoryUpSwitch = TRUE;}

	fYetLeftPressSwitch  = IpD.LeftPressSwitch;
	fYetRightPressSwitch = IpD.RightPressSwitch;

	SendHeartBeat();

//	pCharFileLoadManager->CheckXSprCompleteQueue();
	pCharFileLoadManager->CheckXSprFreeCompleteQueue();

	if(siMainPollStatus != ON_MAINPOLL_GAME) fYetLight = 0.0f;

	switch(siMainPollStatus)
	{	
	case ON_MAINPOLL_LOGINMGR:
		uiTempMainPollStatus	=	LogInMgrPoll();
		if(uiTempMainPollStatus)
			siMainPollStatus	=	uiTempMainPollStatus;
		break;
	case ON_MAINPOLL_FORCED_LOGOUT:
		{
			pOnlineClient->Close();
			pOnlineClientChatting->Close();
			clGrp.lpDDSBack->BltFast(0, 0, GETSCREENSURFACE(this), NULL, DDBLTFAST_WAIT);
			
			uiNextMenu = GS_INITQUIT;		
		}
		break;
	case ON_MAINPOLL_INITGAME:				
		InitGamePoll();		
		siMainPollStatus		=	ON_MAINPOLL_GAME;
		break;		
	case ON_MAINPOLL_GAME:		
		uiTempMainPollStatus	=	GamePoll();
		if(uiTempMainPollStatus)		
			siMainPollStatus	=	uiTempMainPollStatus;				
		break;	
	case ON_MAINPOLL_INITRECEIVEGOOUTVILLAGE:
		InitReceiveGoOutVillage();
		siMainPollStatus		=	ON_MAINPOLL_RECEIVEGOOUTVILLAGE;
		break;
	case ON_MAINPOLL_RECEIVEGOOUTVILLAGE:
		uiTempMainPollStatus	=	ReceiveGoOutVillage();

		if(uiTempMainPollStatus)
		{
			siMainPollStatus	=	uiTempMainPollStatus;
		}
		break;
	case ON_MAINPOLL_INITGOTOIMJINGAME:
		// ÀÓÁø·Ï ¿Â¶óÀÎ¿¡¼­ »ç¿ëÇÏ´Â ÀÏºÎ ÀÚ¿øµéÀ» ÇØÁ¦ÇÑ´Ù.
		// ¸ðµç ÀÚ¿øÀ» ÇØÁ¦ÇÏ¸é ¾ÈµÈ´Ù.		
		FreeForGotoImjinGame();		

		m_siUpdateCounter = 0;
		m_siUpdateMethod = 0;

		siMainPollStatus	=	ON_MAINPOLL_GOTOIMJINGAME;		
		break;
	case ON_MAINPOLL_GOTOIMJINGAME:		
		siMainPollStatus		=	ON_MAINPOLL_INITRETURNONLINE;
		uiNextMenu				=	GS_INITONLINEBATTLEGAME;
		break;
	case ON_MAINPOLL_INITRETURNONLINE:		
		InitReturnOnline();
		::SetFocus(pGame->Hwnd);
		siMainPollStatus	=	ON_MAINPOLL_RETURNONLINE;
		break;
	case ON_MAINPOLL_RETURNONLINE:		
		uiTempMainPollStatus	=	ReturnOnline();
		if(uiTempMainPollStatus != 0)	siMainPollStatus = uiTempMainPollStatus;
		break;	
	case ON_MAINPOLL_ENTERBATTLEEFFECT:
		m_siUpdateCounter++;



		if (m_siUpdateCounter) UpdateScreen(GETSCREENSURFACE(this));
		

		switch (m_siUpdateMethod)
		{
		case 1:
			if (m_siUpdateCounter >= 20)
			{
				m_siUpdateCounter = 0;
				m_siUpdateMethod = 0;
				siMainPollStatus = ON_MAINPOLL_INITGOTOIMJINGAME;
			}
			break;
		case 2:
			{
				DDBLTFX ddbltfx;
				int left = m_siUpdateCounter * 16;
				int right = 800 - m_siUpdateCounter * 16;
				int top = m_siUpdateCounter * 12;
				int bottom = 600 -m_siUpdateCounter * 12;

				ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
				ddbltfx.dwSize      = sizeof(ddbltfx);
				ddbltfx.dwFillColor = 0;
		
			
				RECT blackRect;
				
				 blackRect.left = 0;
				blackRect.right = 800;
				blackRect.top = 0;
				blackRect.bottom = top;

				
				
				 GETSCREENSURFACE(this)->Blt( &blackRect, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

				 blackRect.left = 0;
				blackRect.right = left;
				blackRect.top = top;
				blackRect.bottom = bottom;

				
				
				 GETSCREENSURFACE(this)->Blt( &blackRect, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

				 blackRect.left = right;
				blackRect.right = 800;
				blackRect.top = top;
				blackRect.bottom = bottom;

				
				
				 GETSCREENSURFACE(this)->Blt( &blackRect, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

				 blackRect.left = 0;
				blackRect.right = 800;
				blackRect.top = bottom;
				blackRect.bottom = 600;

				
				
				 GETSCREENSURFACE(this)->Blt( &blackRect, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

				
				
				
					if (m_siUpdateCounter >= 25)
					{
						m_siUpdateCounter = 0;
						m_siUpdateMethod = 0;
						siMainPollStatus = ON_MAINPOLL_INITGOTOIMJINGAME;
					}
			}
			break;
		default:
			m_siUpdateCounter = 0;
			m_siUpdateMethod = 0;
			siMainPollStatus = ON_MAINPOLL_INITGOTOIMJINGAME;
			break;
		}
		break;
	}		

	YetMousePosition.x = IpD.Mouse_X;
	YetMousePosition.y = IpD.Mouse_Y;

	return uiNextMenu;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¸ÞÀÎ Poll() ÃÊ±âÈ­
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitGamePoll()
{		
	RECT	rcMap;

	siGamePollStatus		=	ON_GAMEPOLL_FIELD;	

/*	UI08	uiPalette[768];	
	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
	SetGamePalette(uiPalette);*/

	// Å¬¸®ÇÎÀ» ¼³Á¤ÇÑ´Ù.(¸ÊÀ» ±âÁØÀ¸·Î Å¬¸®ÇÎÀ» ¼³Á¤ÇÑ´Ù.)
	rcMap	=	pOnlineMap->GetScreenRect();
	clGrp.SetClipArea(rcMap.left, rcMap.top, rcMap.right, rcMap.bottom);

	// Å°º¸µå ¹öÆÛ¸¦ Ã»¼ÒÇØÁØ´Ù.
	pOnlineKeyboard->Init();

	RPI.Init();

	if(pOnlineVillageSave)
	{
		delete pOnlineVillageSave;
	}
	pOnlineVillageSave = new OnlineVillageSave;
	pOnlineVillageSave->Init( this );

	FindTrade();

	pOnlineMap->m_NPCParser.ReadText("Online\\Map\\MapNPC.txt");
	ZeroMemory(pOnlineVillageInfoData, sizeof(OnlineVillageInfoData));

	if(pOnlineMiniMap)
	{
		pOnlineMiniMap->MakeTargetList();
		pOnlineMiniMap->SetActive(TRUE);
	}

	pOnlineMousePoint->Clear();

	m_LastNowUserConnectMessageTickCount        = 0;
	m_LastNowUserConnectMessageNowUser          = 0;
	m_LastNowUserConnectMessageErrorOutPutCount = 0;

	// ¾ÆÀÌÅÛ µµÂø Ç¥½ÃÀÌ¹ÌÁö ·Îµù
	clGrp.LoadXspr( "Online\\GameSpr\\Inn_ReceivedItem.Spr", m_ReceivedItemAniSpr );

	// actdoll (2004/04/02 15:30) :	= ÀÌº¥Æ® = µ¶µµ Äù½ºÆ® ¾ÆÀÌÄÜ ·Îµù
//	clGrp.LoadXspr( "Online\\GameSpr\\Icon_Taekuk.SPR", m_Spr_Icon_DokdoQuest );

	pConfig->OpenConfig();

	if(!m_pMercenary->IsAction())
		m_pMercenary->OnButtonPush();

	pOnlineFieldAuction->OnGameStart();

	::SetFocus(pGame->Hwnd);		
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¸ÞÀÎ Poll()
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::GamePoll()
{
	UI16		uiClientRet;	
	UI16		uiNextMenu = 0;
	SI32		siGotoPortalID;
	char		TempBuffer1[1024];
	char		TempBuffer2[1024];

	static DWORD TempCheckTickCount = GetTickCount();
 
	if(TempCheckTickCount > GetTickCount())
	{
		TempCheckTickCount = GetTickCount();
	}
	if((GetTickCount() - TempCheckTickCount) > 10000)
	{
		TempCheckTickCount = GetTickCount();
	}	



//	pCharFileLoadManager->CheckXSprCompleteQueue();
//	pCharFileLoadManager->CheckXSprFreeCompleteQueue();

	//============================================================================================
	// ÀÏ¹Ý MSG Ã³¸®
	while(1)
	{
		uiClientRet	 =	pOnlineClient->DataProc();										// µ¥ÀÌÅ¸ÀÇ ÇÁ·Î½ÃÀú.

		if(pOnlineClient->ConnectOK == FALSE && !m_WaitTickCount && !m_bForcedLogout)
		{
			pOnlineCharUI->SetAction(FALSE);
			pOnlineMsgBoxError->SetMsg(ON_MSGBOX_TYPE_OK, "Disconnected", pOnlineText->Get(ON_TEXT_DISCONNECTED));
			m_bForcedLogout = TRUE;
		}		

		if( pOnlineClient->m_pMsgBuffer.empty() )	break;

		uiClientRet = pOnlineClient->GetMsgIndex();

		if( uiClientRet )
		{
			uiNextMenu = ReceiveMessageProc(uiClientRet);
			if(uiNextMenu != 0) break;
		}
	}

	pOnlineClientChatting->DataProc();


	//============================================================================================
	// ¸¶À»·Î ÀÌµ¿
	if(pMyData->IsGotoVillage() && (pMyData->IsWaitGotoVillageResponse() == FALSE) && siGamePollStatus == ON_GAMEPOLL_FIELD )
	{
		// ¼­¹ö¿¡°Ô ¸¶À»·Î µé¾î°£´Ù°í ¿äÃ»ÇÑ´Ù.		
		pOnlineClient->SendGotoVillage(pMyData->GetGotoVillage());
		pMyData->SetWaitGotoVillageResponse(TRUE);			
	}


	//============================================================================================
	// Game Server°¡ ÀÌ»ó¾ø´ÂÁö °Ë»ç
	if(m_GMFlag != ON_USERGRADE_NORMAL)
	{
		CheckNowConnectUserMessage();
	}

	//============================================================================================
	// Numpad
	if(pOnlineNumPad->pNumberControl)
	{
		if(pOnlineNumPad->pNumberControl->IsShow() == FALSE)
		{
			pOnlineNumPad->fDisable++;
		}
		else
		{
			pOnlineNumPad->fDisable = 0;
		}
	}

	//============================================================================================
	// Portal·Î ÀÌµ¿
	if(pOnlineMsgBoxUsePortal->IsActive())
	{
		SI16			TempX, TempY;

		switch(pOnlineMsgBoxUsePortal->Process())
		{
			case ON_MSGBOX_RETURN_OK:
				// µ·ÀÌ µÅ´ÂÁö °Ë»ç

				if(pMyData->GetMoney() < pOnlineMap->PortalParser.GetPortal(UsePortalType)->i64PortalMoney)
				{
					pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_WARP_TITLE), pOnlineText->Get(ON_TEXT_WARP_MONEY));
					UsePortalType = 0;

					pMyData->SetGotoPortal(FALSE, 0);
					pMyData->SetWaitGotoPortalResponse(FALSE);		
					pOnlineCharUI->SetAction(TRUE);

					pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
					pOnlineCharUI->SetMove(TempX, TempY);
				}
				else
				{
					// ·¹º§ÀÌ³ª ½Å¿ëµµ°¡ µÅ´ÂÁö °Ë»ç

					if((pOnlineMap->PortalParser.GetPortal(UsePortalType)->siNeedLV > pMyData->GetFollowerParameterLevel(0)) && (
						pOnlineMap->PortalParser.GetPortal(UsePortalType)->siNeedCredit > pMyData->GetTradeGrade()))
					{						
						pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_WARP_TITLE), pOnlineText->Get(ON_TEXT_WARP_LVCREDIT), pOnlineMap->PortalParser.GetPortal(UsePortalType)->siNeedLV, pOnlineMap->PortalParser.GetPortal(UsePortalType)->siNeedCredit);
						UsePortalType = 0;

						SI16		TempX, TempY;

						pMyData->SetGotoPortal(FALSE, 0);
						pMyData->SetWaitGotoPortalResponse(FALSE);		
						pOnlineCharUI->SetAction(TRUE);

						pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
						pOnlineCharUI->SetMove(TempX, TempY);
					}
					else if(pOnlineBooth->IsActionIn())
					{
						UsePortalType = 0;

						SI16		TempX, TempY;

						pMyData->SetGotoPortal(FALSE, 0);
						pMyData->SetWaitGotoPortalResponse(FALSE);		
						pOnlineCharUI->SetAction(TRUE);

						pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
						pOnlineCharUI->SetMove(TempX, TempY);
					}
					else
					{
						// ÇöÀç ¼±ÅÃÇÑ PortalÀÇ ID ¸¦ °Ë»ö

						SEffectData		TempEffect;

						// Warp
						NextMapIndex = UsePortalType;
						pOnlineClient->SendGotoPortal(UsePortalType);

						pMyData->SetWaitGotoPortalResponse(TRUE);

						// Effect
						pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
						TempEffect.m_Type                                = EFFECTTYPE_WARPEFFECT;
						TempEffect.m_EffectData_WarpEffect.m_Frame        = 0;
						TempEffect.m_EffectData_WarpEffect.m_XPos         = TempX;
						TempEffect.m_EffectData_WarpEffect.m_YPos         = TempY;
						pOnlineFieldEffect->AddEffect(TempEffect);
					}
				}
				break;

			case ON_MSGBOX_RETURN_CANCEL:
				SI16		TempX, TempY;

				pMyData->SetGotoPortal(FALSE, 0);
				pMyData->SetWaitGotoPortalResponse(FALSE);		
				pOnlineCharUI->SetAction(TRUE);

				pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
				pOnlineCharUI->SetMove(TempX, TempY);
				break;
		}
	}
	else
	{
		UsePortalType = 0;
		if(pMyData->IsGotoPortal() && (pMyData->IsWaitGotoPortalResponse() == FALSE))
		{
			// ¼­¹ö¿¡°Ô Portal·Î µé¾î°£´Ù°í ¿äÃ»ÇÑ´Ù.						
			siGotoPortalID	= pMyData->GetGotoPortal();			// µé¾î°¥ PortalÀÇ ÀÌ¸§À» ¾ò¾î¿Â´Ù.
			
			// ÇöÀç PortalÀ» µé¾î°¥¼ö ÀÖ´Â Áö °Ë»ç
			if((pMyData->GetFollowerParameterBattlePower() < pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siReqBattlePower) && (g_fTestVersion == FALSE))
			{
				SI16		TempX, TempY;

				pMyData->SetGotoPortal(FALSE, 0);
				pMyData->SetWaitGotoPortalResponse(FALSE);		
				pOnlineCharUI->SetAction(TRUE);

				sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PORTALMSG_LOWLEVEL), pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siReqBattlePower);
				pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), TempBuffer1);

				pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
				pOnlineCharUI->SetMove(TempX, TempY);
			}
			else if((pMyData->GetFollowerParameterLevel(0) > pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siReqMaxLv) && (pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siReqMaxLv > 0))
			{
				SI16		TempX, TempY;

				pMyData->SetGotoPortal(FALSE, 0);
				pMyData->SetWaitGotoPortalResponse(FALSE);		
				pOnlineCharUI->SetAction(TRUE);

				sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PORTALMSG_HIGHLEVEL), pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siReqMaxLv);
				pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), TempBuffer1);

				pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
				pOnlineCharUI->SetMove(TempX, TempY);
			}
			else
			{
				if(pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->i64PortalMoney != 0)
				{
					UsePortalType = siGotoPortalID;

					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_WARP_TEXT), pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->i64PortalMoney);
					sprintf((char*)TempBuffer2, "%s\n%s"
						, pOnlineText->Get(pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siPortalDescriptionCode), TempBuffer1);
					pOnlineMsgBoxUsePortal->SetMsg( ON_MSGBOX_TYPE_OKCANCEL, pOnlineText->Get(ON_TEXT_WARP_TITLE), (char*)TempBuffer2);
				}
				else if((pMyData->GetFollowerParameterLevel(0) < pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siNeedLV) && (pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siNeedLV > 0))
				{
					SI16		TempX, TempY;

					pMyData->SetGotoPortal(FALSE, 0);
					pMyData->SetWaitGotoPortalResponse(FALSE);		
					pOnlineCharUI->SetAction(TRUE);

					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PORTALMSG_PKFIELD_LOWLEVEL), pOnlineMap->PortalParser.GetPortal(siGotoPortalID)->siNeedLV);
					pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), TempBuffer1);

					pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
					pOnlineCharUI->SetMove(TempX, TempY);
				}
				else if(pOnlineBooth->IsActionIn())
				{
					SI16		TempX, TempY;

					pMyData->SetGotoPortal(FALSE, 0);
					pMyData->SetWaitGotoPortalResponse(FALSE);		
					pOnlineCharUI->SetAction(TRUE);

					pIOnlineChar->GetDPos(pMyData->GetMyCharUniqueID(), &TempX, &TempY);
					pOnlineCharUI->SetMove(TempX, TempY);
				}
				else 
				{
					// ÇöÀç ¼±ÅÃÇÑ PortalÀÇ ID ¸¦ °Ë»ö

					NextMapIndex = siGotoPortalID;
					pOnlineClient->SendGotoPortal(siGotoPortalID);

					pMyData->SetWaitGotoPortalResponse(TRUE);
				}
			}
		}
	}
	
	//============================================================================================
	// ÇÊµå·Î ÀÌµ¿
//	if(pMyData->IsGotoField() == TRUE)
//	{
//		GotoField();	
//		pMyData->SetGotoField(FALSE);	
//	}	

	//============================================================================================
	// ½Ã°£¿¡ µû¸¥ ¹ã³· Ã³¸®
	float					TempLight;
	SOnlineTime				NowTime;
	static SOnlineTime		YetNowTime;

	TempLight  = 1.0f;
	NowTime    = pTimer->GetNowTime();

	if(pOnlineVillage != NULL)
	{
		if(pOnlineVillage->GetStatus() != ON_VILLAGE_STATUS_IN_STRUCTURE) 
		{
			// ÇöÀç À§Ä¡°¡ °Ç¹°¾ÈÀÌ ¾Æ´Ï¸é ½Ã°£¿¡ µû¶ó ¹à°ÔÇÔ
			if((NowTime.Hour <= 5) || (NowTime.Hour >= 22))
			{
				TempLight = 0.6f;
			}
			else if(NowTime.Hour < 9)
			{
				TempLight = 0.6f + ((float)(NowTime.Hour - 5) * 0.1f);
			}
			else if(NowTime.Hour > 18)
			{
				TempLight = 0.6f + ((float)(22 - NowTime.Hour) * 0.1f);
			}
		}
	}
	else
	{
		// ÇöÀç À§Ä¡°¡ °Ç¹°¾ÈÀÌ ¾Æ´Ï¸é ½Ã°£¿¡ µû¶ó ¹à°ÔÇÔ
		if((NowTime.Hour <= 5) || (NowTime.Hour >= 22))
		{
			TempLight = 0.6f;
		}
		else if(NowTime.Hour < 9)
		{
			TempLight = 0.6f + ((float)(NowTime.Hour - 5) * 0.1f);
		}
		else if(NowTime.Hour > 18)
		{
			TempLight = 0.6f + ((float)(22 - NowTime.Hour) * 0.1f);
		}
	}

	if(YetNowTime.Hour != NowTime.Hour)
	{
		if(NowTime.Hour == 6)
		{
			PushEffect(EFFECT_DAY, 0, 0);
		}
		else if((NowTime.Hour <= 5) || (NowTime.Hour >= 20))
		{
			PushEffect(EFFECT_NIGHT, 0, 0);
		}		
	}
	if(fYetLight != TempLight)
	{
		SetGamePalette(PaletteNight[10 - (SI16)(TempLight * 10.0f)]);
		PaletteWhiteColor = RGB(PaletteNight[10 - (SI16)(TempLight * 10.0f)][PaletteWhiteColorIndex * 3] * 4, PaletteNight[10 - (SI16)(TempLight * 10.0f)][(PaletteWhiteColorIndex * 3) + 1] * 4, PaletteNight[10 - (SI16)(TempLight * 10.0f)][(PaletteWhiteColorIndex * 3) + 2] * 4);
	}

	YetNowTime = NowTime;
	fYetLight  = TempLight;	

	//============================================================================================
	// ¸¶À» °ø°Ý½Ã »ý±â´Â Delay Ã³¸®
	if(m_VillageAttackDelay)
	{
		m_VillageAttackDelay--;
	}
 
	//============================================================================================
	// ¸¶¿ì½º One Click
	// One Click Check
	if(IpD.LeftPressSwitch)
	{
		fVillageLeftOneSwitch++;
	}
	else
	{
		fVillageLeftOneSwitch = FALSE;
	}

	if(IpD.RightPressSwitch)
	{
		fVillageRightOneSwitch++;
	}
	else
	{
		fVillageRightOneSwitch = FALSE;
	}

	//============================================================================================
	// Prepare
	if(uiNextMenu == 0)
	{
		switch(siGamePollStatus)
		{
			case ON_GAMEPOLL_FIELD:			
				return FieldPoll(0);		

			case ON_GAMEPOLL_VILLAGE:
				dwCurrentFrameAddTime   = 0;
				dwCurrentFrameStartTime = 0;
				return VillagePoll(0);	

			// ¼ö¼º ÀÎÅÍÆäÀÌ½º Poll
			case ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE:
				return VillageDefenceInterfacePoll(0);
		}	
	}
	else
	{
		return uiNextMenu;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	Ä³¸¯ÅÍ°¡ ÇÊµå»ó¿¡ ÀÖÀ»¶§ È£ÃâÇÏ´Â ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::FieldPoll(UI16 uiClientRet)
{
	UI16			uiNextMenu	=	0;		
//	CHAR			szMyCharSpeechBuffer[1024];
	CHAR			TempBuffer[1024];
//	CHAR			TempBuffer1[1024];
	DWORD			dwPrevFrameStartTime, dwPrevFrameTimeRequire;	
	SI32			siX, siY;					
	SI16			siTX, siTY;					
	POINT			ptMouse;
	BOOL			fRender;
	SI16			siHeroX, siHeroY;
	static SI32		CheckObjectZone = 0;

	if((GetTickCount() % 50) == 0)
		pIOnlineMouse->Action();

	if(dwCurrentFrameStartTime == 0) dwCurrentFrameStartTime = timeGetTime();

	dwPrevFrameStartTime			=	dwCurrentFrameStartTime;	

	// ´ÙÀ½ ÇÁ·¹ÀÓÀ» ÁøÇàÇÒ ¼ö ÀÖ´Â »óÈ²ÀÌ µÉ¶§±îÁö ´ë±âÇÑ´Ù.
	dwPrevFrameStartTime			-=	dwCurrentFrameAddTime;
	dwCurrentFrameAddTime			=	0;
	dwPrevFrameTimeRequire			=	timeGetTime() - dwPrevFrameStartTime;

	// ÃÊ´ç ÇÁ·¹ÀÓ¼ö¸¦ ¸ÂÃá´Ù.
	if(dwPrevFrameTimeRequire < CPF)
	{
		Sleep(1);
		return uiNextMenu;
	}
	else
	{
		dwCurrentFrameAddTime	=	dwPrevFrameTimeRequire - CPF;
	}

	// ¸¸¾à ÄÄÇ»ÅÍ°¡ µþ¸®¸é Render¸¦ ÇÏÁö ¾Ê´Â´Ù.
	fRender = TRUE;
	if(dwCurrentFrameAddTime > 1000)
	{
		dwCurrentFrameAddTime = 0;
	}
	if(dwCurrentFrameAddTime > CPF)
	{
		fRender = FALSE;
		dwCurrentFrameSkippingCount++;
	}
	if(fRender == TRUE)
	{
		dwCurrentFrameSkippingCount = 0;
	}
	if(dwCurrentFrameSkippingCount > 3)
	{
		fRender = TRUE;
		dwCurrentFrameSkippingCount = 0;
	}

	m_siMsgBoxStatus = pOnlineMsgBox->Process();

	/////////////////////////////////////////////////////
	if(m_WaitTickCount)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", pOnlineText->Get(ON_TEXT_WAIT));

		if((GetTickCount() - m_WaitTickCount) >= 3000)
		{
			// °×ÀÓÆú Á¢¼Ó½Ã ¼³Á¤Çß´ø°Í ÇØÁ¦ÇÏ±â
			pOnlineSetting->GameInit();

			// ·Î±×ÀÎ È­¸éÀ¸·Î
			SetMainPollStatus(ON_MAINPOLL_LOGINMGR);
			SetLogOut(0, ON_MAINPOLL_LOGINMGR);
			m_WaitTickCount = 0;
			return uiNextMenu;
		}
	}
	////////////////////////////////////////////////////
	// ÇÁ·¹ÀÓÀ» Áõ°¡ ½ÃÅ²´Ù.
	uiTotalFrame++;


	siFPS	=	1000 / (timeGetTime() - dwPrevFrameStartTime);

	// °ø¼ºÀü À¯´Ö
	if (TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()))
	{
		// °ø°Ý Áö¿¬ ½Ã°£ ÀÚµ¿ °»½Å
		pIOnlineChar->SetSiegeWarfareUnit_PastAttackDelay(pMyData->GetMyCharUniqueID(), dwPrevFrameTimeRequire);

		/*
		// °ø¼ºÀü º¸±Þ ¼ö·¹ÀÎ °æ¿ì ÀÚµ¿ ¼Ò¸ðµÇÁö ¾Ê´Â´Ù.
		if (KI[pIOnlineChar->GetImjinKind( pMyData->GetMyCharUniqueID() )].IsAttribute1(ATB_PATIENT))
		{
			// º¸±ÞÇ° ÀÚµ¿ ¼Ò¸ð ³²Àº ½Ã°£ Ã³¸®
			if (m_siRestTimeAutoSpendSupplyGoods > dwPrevFrameTimeRequire)
				m_siRestTimeAutoSpendSupplyGoods -= dwPrevFrameTimeRequire;
			else
			{
				// reset rest time
				m_siRestTimeAutoSpendSupplyGoods = m_SOptionSiegeWarfare.siSIEGEWARFARE_WasteSupplygoodsTime;

				// spend a supply goods
				pIOnlineChar->WasteSiegeWarfareSupplyGoods(pMyData->GetMyCharUniqueID());
				pMyData->SetCurrentSupplyGoodsAmount(pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(pMyData->GetMyCharUniqueID()));
			}
		}
		*/
	}

	// ÇöÀç ½Ã°£À» ¾ò¾î¿Â´Ù.
	dwCurrentFrameStartTime		=	timeGetTime();		

	// À§¿¡¼­ FrameSkipping ¶§¹®¿¡ Up Check¸¦ ¹«½ÃÇÏ°í Áö³ª°¥ °æ¿ì°¡ ÀÖ±â¶§¹®¿¡ ¿©±â¼­¸¸ ´Ù½Ã °Ë»ç

	if(fLeftMemoryUpSwitch)
	{
		fLeftUpSwitch       = TRUE;
		fLeftMemoryUpSwitch = FALSE;
	}
	if(fRightMemoryUpSwitch)
	{
		fRightUpSwitch       = TRUE;
		fRightMemoryUpSwitch = FALSE;
	}

	// One Click Check
	if(IpD.LeftPressSwitch)
	{
		fLeftOneSwitch++;
	}
	else
	{
		fLeftOneSwitch = FALSE;
	}

	if(IpD.RightPressSwitch)
	{
		fRightOneSwitch++;
	}
	else
	{
		fRightOneSwitch = FALSE;
	}

	DWriteStringToFile("½ÃÀÛ 2");

	// ¿À¹ö Çìµå	
	pIOnlineMouse->Action();
	pOnlineMousePoint->Action();

	// Å°º¸µåÀÇ ÀÔ·Â»óÅÂ¸¦ ¾ò¾î¿Â´Ù.
	pOnlineKeyboard->Action();

	//ÁØ¼®ÀÌÇü Ãß°¡ºÎºÐ 
	// ¸ðµç À©µµ °ü¸®ÀÚ
//	pJSWndMgr->Action();
	
	ptMouse.x	=	IpD.Mouse_X;
	ptMouse.y	=	IpD.Mouse_Y;

	pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

	// ¸¶À» Á¤º¸ Ã³¸®
	if(pOnlineMap->m_MapInfo.uiVillageCode != pOnlineVillageInfoData->m_uiVillageCode)
	{
		pOnlineVillageInfoData->m_siFrame = 0;
		if(pOnlineMap->m_MapInfo.uiVillageCode != pOnlineVillageInfoData->m_siVillageSeriesWait)
		{
			pOnlineClient->SendRequestVillageInfo(pOnlineMap->m_MapInfo.uiVillageCode);
			pOnlineVillageInfoData->m_siVillageSeriesWait = pOnlineMap->m_MapInfo.uiVillageCode;
		}
	}
	else
	{
		pOnlineVillageInfoData->m_siFrame++;
		if(pOnlineVillageInfoData->m_siFrame > 16)
		{
			pOnlineVillageInfoData->m_siFrame = 16;
		}
	}

	if(pOnlineVillage == NULL)
	{
		// ÇöÀç Áö¿ªÀ» Ã¼Å©ÇÏ¿© ±× Áö¿ªÀÇ ¹è°æÀ½¾ÇÀ» ¼³Á¤ÇÑ´Ù.
		if(pOnlineMap->m_MapIndex == 0)
		{
			SetBackGroundMusic(CheckHeroZone());
		}

		if( !pIOnlineChar->IsMoving(pMyData->GetMyCharUniqueID()) )
		{
			siTalkDealy++;

			if( siTalkDealy >= 6000 )
			{
				// robypark 2005/1/14 17:27
				// ÁÖÀÎ°ø °ü·Ã µ¥ÀÌÅÍ °ü¸®ÀÚ ¾ò±â
				CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

				if (pFieldHeroInfo)
				{
					// ÇÑ °÷¿¡ ¿À·¡ ÀÖÀ» °æ¿ì Áö·çÇÏ´Ù°í ´ë»ç ÇÃ·¹ÀÌ
					if (pFieldHeroInfo->GetSndBoring())
						HashCodePushEffect(pFieldHeroInfo->GetSndBoring(), 0, 0);
				}

				siTalkDealy = 0;
			}
		}
		else
			siTalkDealy = 0;

		// ÇöÀç ÀÖ´Â À§Ä¡°¡ Portal Zone ÀÌ¸é
		if((pMyData->IsGotoPortal() && (pMyData->IsWaitGotoPortalResponse() == FALSE)) == FALSE)
		{
			PortalHeader*	lpPortalHeader;
			SI16			siPortalCode;		

			siPortalCode   = -1;
			lpPortalHeader = pOnlineMap->PortalParser.GetPortal(siHeroX, siHeroY, pOnlineMap->m_MapIndex);
			if(lpPortalHeader)
			{
				siPortalCode = lpPortalHeader->siPortalCode;

				if(lpPortalHeader->siPortalType == 1)
				{
					if(siPortalCode != -1)
					{
						if((pMyData->GetFollowerParameterBattlePower() < lpPortalHeader->siReqBattlePower) && (g_fTestVersion == FALSE))
						{
							sprintf((char*)TempBuffer, pOnlineText->Get(ON_TEXT_PORTALMSG_LOWLEVEL), lpPortalHeader->siReqBattlePower);
							pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), TempBuffer);
						}
						else if((pMyData->GetFollowerParameterLevel(0) > lpPortalHeader->siReqMaxLv) && (lpPortalHeader->siReqMaxLv > 0))
						{
							sprintf((char*)TempBuffer, pOnlineText->Get(ON_TEXT_PORTALMSG_HIGHLEVEL), lpPortalHeader->siReqMaxLv);
							pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), TempBuffer);
						}
						else
						{
							// ¸¶À»¿¡ ±ÙÁ¢ÇØ ÀÖ´Ù. ¸¶À» ¾ÈÀ¸·Î µé¾î°£´Ù.										
							pMyData->SetGotoPortal(TRUE, siPortalCode);

							// Ä³¸¯ÅÍ¿¡°Ô Á¤Áö ¸í·ÉÀ» ³»·ÁÁØ´Ù.
							pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

							// ´õ ÀÌ»óÀÇ »ç¿ëÀÚ ÀÔ·ÂÀ» ¹ÞÁö ¾Ê´Â´Ù.
							pOnlineCharUI->SetAction(FALSE);
						}
					}
				}
			}
		}
	}
	
	// ÀÌ¸ðÆ¼ÄÜÀ» Ã³¸®ÇÑ´Ù.
	pOnlineImoticon->Pool(fLeftUpSwitch);

	if(pOnlineVillage == NULL)
	{
		// ¸¶¿ì½º ÀÖ´Â À§Ä¡¿¡ Àû Ä³¸¯ÅÍ°¡ ÀÖÀ¸¸é ¸¶¿ì½º¸¦ º¯°æÇÑ´Ù.
		// ´Ü, ÇÊµå ¿µ¿ªÀÌ¿©¾ß ÇÑ´Ù.	
		pOnlineHelp->Initialize();

		pOnlineMsgBoxPay->Process();
		if(pOnlineMsgBoxError->Process() == ON_MSGBOX_RETURN_OK)
		{
			pOnlineClientChatting->Close();
			return ON_MAINPOLL_FORCED_LOGOUT;
		}
	}
	
	DWriteStringToFile("½ÃÀÛ 3");

	// robyprk 2004/11/5 18:5
	// »ç¿ëÀÚ ÀÚ½ÅÀÌ °ø¼ºÀü À¯´ÖÀÎÁö °Ë»ç
	BOOL bIsSiegeWarfareUnit = pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID());

	// º¯½Å ¹öÆ° ºñÈ°¼ºÈ­
	pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);

	// robypark 2005/1/17 11:0
	// ÇÇ³­¹ÎÀ¸·Î º¯½Å °ü·Ã Ã³¸®
	if (IsInTimeSiegeWarfare())	// °ø¼º½Ã°£ÀÌ¶ó¸é
	{
		// robypark 2005/1/29 13:32
		// ÃÊº¸ÀÚ ¼ö·ÃÀåÀ¸·Î ¿öÇÁ µÉ ¼ö ÀÖ´Ù´Â °æ°í ¸Þ½ÃÁö ¼³Á¤
		if (m_sWarningMsgInSiegeWarfare.m_dwLeftTime < dwPrevFrameTimeRequire)
		{
			HDC hdc;
			char szMsg[1024] = {0, };

			// °ø¼ºÁß ÁÖÀÇ»çÇ× ¸Þ½ÃÁö Ãâ·Â
			// HDC¸¦ ÀÌ¿ëÇÏ¿© Âï¾î¾ß ÇÒ Á¤º¸¸¦ ¼³Á¤
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				sprintf(szMsg, pOnlineText->Get(8100118), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);

				SetMessage4(szMsg, hdc, -1, 0, RGB(0, 255, 0));

				GETSCREENSURFACE(this)->ReleaseDC(hdc);
			}

			// Ã¤ÆÃ Ã¢¿¡µµ Ãß°¡
			pOnlinePannel->AddChat((char*)szMsg, ON_INN_COLOR_GREEN, CHATTINGMODE_NORMAL);

			m_sWarningMsgInSiegeWarfare.m_dwLeftTime = WARNINGMSGINSIEGEWARFARE_DELAY;
		}
		else
			m_sWarningMsgInSiegeWarfare.m_dwLeftTime -= dwPrevFrameTimeRequire;

		// robypark 2005/1/29 14:6
		// °ø¼ºÀü º¸±Þ¼ö·¹ÀÎ°¡?
		if (KI[pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID())].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// º¸±Þ¼ö·¹(¸Þµñ)ÀÏ °æ¿ì 10ÃÊ¸¶´Ù ¸¶³ª°¡ 10%¾¿ È¸º¹µÈ´Ù.
			if (FALSE == m_sRecoveryMana.m_bSendedRequest)
			{
				if (m_sRecoveryMana.m_dwLeftTime < dwPrevFrameTimeRequire)
				{
					m_sRecoveryMana.m_dwLeftTime	 = m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime;
					m_sRecoveryMana.m_bSendedRequest = TRUE;

					// ¼­¹ö¿¡ ¸¶³ª ÀÚ¿¬Áõ°¡ ¿äÃ»
					pOnlineClient->SendReuqestChargeMP();
				}
				else
					m_sRecoveryMana.m_dwLeftTime -= dwPrevFrameTimeRequire;
			}
		}


		// ÁÖÀÎ°ø ¸ðµå¿´´Ù¸é,
		if (SIEGE_TRANSFORM_MODE_HERO == m_scSiegeTransformMode)
		{
			// °ø¼ºÇÏ´Â ³¯ÀÎ °æ¿ìÀÇ »ç¿ëÀÚ¸¸
			if (pMyData->IsSiegeWarfareDay())
			{
				// »ç¿ëÀÚ¸¦ Á¦¿ÜÇÑ ¸ðµç ÁÖÀÎ°ø Ä³¸¯ÅÍ¸¦ ÇÇ³­¹ÎÀ¸·Î º¯½Å½ÃÅ²´Ù.
				pIOnlineChar->AllHeroCharacterTransformTo();

				// ÇÇ³­¹Î ¸ðµå ¼³Á¤
				m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_REFUGEE;
			}
		}

		// °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¸ç
		if (FALSE == bIsSiegeWarfareUnit)
		{
			// º¯½Å °¡´ÉÇÑ°¡?
			if ((NULL != pMyData->GetGuildName())
				&& (pMyData->CanTransformSiegeWarfareUnit())
				&& (pMyData->IsSiegeWarfareDay())
				&& (FALSE == pOnlinePannel->GetSiegeWarfareTransformUIWnd()->IsSendedMsg()))
			{
				// º» ¸ÊÀÌ¶ó¸é
				if (0 == pOnlineMap->m_MapIndex)
				{
					// º¯½Å ¹öÆ° È°¼ºÈ­
					pOnlinePannel->SetShowTransformSiegeUnitBtn(TRUE);
				}
				else
				{
					// º¯½ÅÀ» À§ÇØ ¿­·ÁÁø Ã¢ÀÌ ÀÖ´Ù¸é ´Ý±â
					pOnlinePannel->ShowWindow_SiegeWarfareTransformUIWnd(FALSE);
				}

				// ¹Ì´Ï ¸ÊÀÌ È°¼ºÈ­?
				if (pOnlineMiniMap->IsActive())
				{
					// ¹Ì´Ï¸Ê »óÅÂ°¡ ÀüÃ¼È­¸éÀÏ °æ¿ì?
					if (pOnlineMiniMap->ISFullMode())
					{
						// º¯½Å ¹öÆ° ºñÈ°¼ºÈ­
						pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);
					}
				}
			}
		}
	}
	else
	{
		// ÇÇ³­¹Î ¸ðµå¿´´Ù¸é
		if (SIEGE_TRANSFORM_MODE_REFUGEE == m_scSiegeTransformMode)
		{
			// »ç¿ëÀÚ¸¦ Á¦¿ÜÇÑ ¸ðµç ÇÇ³­¹Î Ä³¸¯ÅÍ¸¦ ÁÖÀÎ°ø Ä³¸¯ÅÍ·Î º¯½Å½ÃÅ²´Ù.
			pIOnlineChar->AllHeroCharacterTransformTo();

			// ÁÖÀÎ°ø ¸ðµå ¼³Á¤
			m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_HERO;
		}

		// º¯½ÅÀ» À§ÇØ ¿­·ÁÁø Ã¢ÀÌ ÀÖ´Ù¸é ´Ý±â
		pOnlinePannel->ShowWindow_SiegeWarfareTransformUIWnd(FALSE);
	}

	// ¸¶¿ì½º Ä¿¼­ ¾Æ·¡ ¿ÀºêÁ§Æ® Ã³¸®(Ç³¼± µµ¿ò¸»µîÀ» À§ÇÑ)
	if (pOnlineVillage == NULL)
	{
		// robypark 2005/1/26 18:33
		ProcessFieldMouseMove(bIsSiegeWarfareUnit, ptMouse);
	}

	// °ø¼ºÀü ¸¶À»°ø°Ý
	if(pOnlineVillage == NULL)
	{
		if(SIEGEWARFARE_ATTACKMODE_VILLAGE == m_sSiegeWarfare_AttackMode.scAttackMode)
		{
			UI16		ObjectCode;

			// °ø¼º ½Ã°£ÀÎÁö ºñ±³
			if(IsInTimeSiegeWarfare())
			{
				// Object ¸¦ Ã£´Â´Ù.
				ObjectCode = pIOnlineObject->GetVillageObject(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);
				
				if(ObjectCode != 0)
				{
					if(GetVillageWarDayGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))	// °ø¼ºÇÏ´Â ¸¶À»ÀÌ´Ù.
					{
						BOOL bRangeAttacker = FALSE;		// ¿ø°Å¸®, ±Ù°Å¸® °ø¼ºÀü À¯´Ö ±¸ºÐ ÇÃ·¡±×
						if (TRUE == SiegeWarfare_CanAttackVillage(pMyData->GetMyCharUniqueID(), ObjectCode, bRangeAttacker))
						{
							// ¸¶À»ÀÌ¸é °ø°Ý
							pIOnlineObject->GetPos(ObjectCode, &siTX, &siTY);

							// ±Ù°Å¸® °ø¼ºÀü À¯´ÖÀÏ °æ¿ì
							if (FALSE == bRangeAttacker)
							{
								pOnlineCharUI->SetShortDistanceAttackVillage(siTX, siTY);
							}
							// ¿ø°Å¸® °ø¼ºÀü À¯´ÖÀÏ °æ¿ì
							else
							{
								pOnlineCharUI->SetLongDistanceAttackVillage(siTX, siTY);
							}
						}
					}
					else
					{
						// Àû´ë ¸¶À»ÀÏ °æ¿ì
						if (GetVillageEnemyGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))
						{
							// Àû´ë ¸¶À»ÀÌÁö¸¸ ¿À´Ã ÀÌ ¸¶À»ÀÇ »ó´Ü°ú °ø¼ºÀüÀÌ ¾ø´Ù.
							char szErrorMsg[512];
							SVillageData*		lpSVillageData;
							VillageHeader*		lpVillageHeader;

							lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);
							lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);

							if ((lpSVillageData) && (lpVillageHeader))
							{
								// »ó´ÜÀÌ ÀÖ´Â ¸¶À»ÀÎ °æ¿ì
								if (NULL != lpSVillageData->m_VillageGuildName[0])
								{
									sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
												lpSVillageData->m_VillageGuildName, 
												pOnlineText->Get(lpVillageHeader->siVillageName ));
								}
								// »ó´ÜÀÌ ¾ø´Â ¸¶À»ÀÎ °æ¿ì
								else
								{
									sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY),
												pOnlineText->Get(lpVillageHeader->siVillageName ));
								}

								pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);
							}
						}
						else
						{
							// Àû´ë ¸¶À»ÀÌ ¾Æ´Ò°æ¿ì
							pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
						}

						// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤ º¯¼ö
						m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
						m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
					}
				}
			}
			else
			{
				char szErrorMsg[256];
				sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_VILLAGE_ATTACK_TIME), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);
				pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), szErrorMsg);

				// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
				m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
				m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			}
		}
	}

	// °ø¼ºÀü Ä³¸¯ÅÍ°£ ÀüÅõ
	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if(SIEGEWARFARE_ATTACKMODE_CHARACTER == m_sSiegeWarfare_AttackMode.scAttackMode)	// Ä³¸¯ÅÍ°ø°Ý ¸ðµå
			{
				if(bCharMove == TRUE)
				{
					// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ °ø¼ºÀü À¯´ÖÀÎÁö °Ë»ç
					if (TRUE == bIsSiegeWarfareUnit)
					{
						// °ø¼º À¯´Ö °£¿¡ ÀüÅõÀÌ´Ù. °ø°ÝÀÌ °¡´ÉÇÑÁö °Ë»ç
						if (TRUE == SiegeWarfare_CanAttackUnit(pMyData->GetMyCharUniqueID(), m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID))
						{
							pOnlineCharUI->SetFieldAttack(m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID);
						}				
					}	
				}	// if(bCharMove == TRUE)
			}	// if (Ä³¸¯ÅÍ °ø°Ý ¸ðµå)
		}	// if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
	}


	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if (fRightOneSwitch == 1)
			{
				// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
				m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
				m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

				if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					// robypark 2005/1/26 18:22
					// ÇÊµå¿¡ ÀÖÀ» ¶§ ¸¶¿ì½º Ä¿¼­ ¿À¸¥ÂÊ ¹öÆ° Å¬¸¯ Ã³¸®
					ProcessFieldMouseRightBtnClick(bIsSiegeWarfareUnit);
				}
			}

			if(IpD.LeftPressSwitch)
			{		
				// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ¸¦ ÀÌµ¿½ÃÅ²´Ù.
				// ÇöÀç ¸¶¿ì½º°¡ °¡¸®Å°´Â °ÍÀ» ±âÁØÀ¸·Î ÀÌµ¿, °ø°Ý, ¸¶À»¿¡ µé¾î°£´Ù, ¾ÆÀÌÅÛÀ» ÁÝ´Â´Ù µîÀÇ ¸í·ÉÀ» ³»¸°´Ù.
				if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					// robypark 2005/1/26 18:22
					// ÇÊµå¿¡ ÀÖÀ» ¶§ ¸¶¿ì½º Ä¿¼­ ¿ÞÂÊ ¹öÆ° Å¬¸¯ Ã³¸®
					ProcessFieldMouseLeftBtnClick(bIsSiegeWarfareUnit, siX, siY);
				}
			}
		}
	}

	pOnlineCharUI->Poll();

	// ´ÜÃàÅ°¸¦ Ã³¸®ÇÑ´Ù.
	FieldShortCutKey();	
 
	if(pOnlineVillage == NULL)
	{
//		m_pNpcTalk->Action();

		m_pQuestInterface->Action();

		m_pChatMacro->Action();

		pOnlineParty->Action();

		// ÆÇ³Ú
		pOnlinePannel->Poll();

		// ÀÎº¥Åä¸®.
		pOnlineInventory->SetFlagVillage(FALSE);
		pOnlineInventory->Action();

		// °Å·¡Ã¢
		pOnlineTrade->Action();

		// ÀåºÎ
		pOnlineTradeBook->Poll();


		// ÁÂÆÇ
		pOnlineBooth->ActionBench();

		// ÆÁ
		pOnlineTip->Action();

		// Quest Play
		pOnlineQuestPlay->Poll();

//		//ÃÊº¸ÀÚ µµ¿ò¸».
//		pOnlineBasicTip->Action();

//		pOnlineBasicQuest->Action();

		

		// °æ¸Å 
		pOnlineFieldAuction->Process();

		// Á¢¼ö
		pOnlineReception->Process();

		// ´ëÀåÀåÀÌ
		pOnlineSmith->Poll();

//		pJSTestWnd->Action();

		// ¾ÆÀÌÅÛ È¹µæ Ã¢ °ü·Ã Çàµ¿À» ÇÑ´Ù. 
		pGame->pItemInfoWindow->Prepare();

		// ¸Þ½ÅÀú
		pOnlineMessenger->Action();

		m_pMercenary->Action();

		if( bFindTrade )
			FindTradeBook();

		// ¹Ì´Ï¸Ê
		pOnlineMiniMap->Pool(FALSE);

		// ³» Ä³¸¯ÅÍÀÇ ¸í·ÉÀ» Ã³¸®ÇÑ´Ù.(ÀÌµ¿, °ø°Ý, ¸¶À» µé¾î°¨, ¾ÆÀÌÅÛ ÁÝ±â)
		ProcessMyCharOrder();
	}
		
	// º¸³¾ ¸Þ½ÃÁö°¡ ÀÖÀ¸¸é º¸³»ÁØ´Ù.
	RPI.Send();

	// Resource °ü¸®
//	if( timeGetTime() - m_ImageFreeTimer > 5000 )
	{
		pOnlineResource->Prepare();
//		FreeOldImageByTime();

//		m_ImageFreeTimer = timeGetTime();
	}

	// ¿À¹öÇìµå End
	// ¿ÀºêÁ§Æ® ¾×¼Ç.	
	pIOnlineObject->Action();
	

	// Ä³¸¯ÅÍ ¾×¼Ç.		
	pIOnlineChar->Action();		

	// µû¶ó´Ù´Ï´Â ¿ëº´µé Ã³¸®
	pIOnlineChar->ActionByVMercenary();

	pOnlineMap->Action();

	// robypark 2004/10/25 13:28
	// °ø¼ºÀü ÀüÅõ °ü·Ã È¿°ú Ã³¸®
	m_SiegeWarfare_BulletMgr.Action();
	m_SiegeWarfare_SmokeMgr.Action();

	//Äù½ºÆ® ¾ÆÀÌÄÜÀ» Ãâ·ÂÇÑ´Ù.
//	if(pOnlineTradeBook->IsAction())
//		pOnlineBasicQuest->SetAction(FALSE);
//	else 
//		pOnlineBasicQuest->SetAction(TRUE);
		


	CheckObjectZone++;
	if(CheckObjectZone > 15)
	{
		SI16		TempX;
		SI16		TempY;

		TempX = siHeroX;
		TempY = siHeroY;
		pIOnlineChar->GetPos(pOnlineMap->GetCurrentFocusCharID(), &TempX, &TempY);
		pIOnlineObject->CheckZone((TempX - (ON_MAP_VIEW_XSIZE_HALF * 2)), (TempY - (ON_MAP_VIEW_YSIZE_HALF * 2)), (TempX + (ON_MAP_VIEW_XSIZE_HALF * 2)), (TempY + (ON_MAP_VIEW_YSIZE_HALF * 2)));
		CheckObjectZone = 0;
	}
	// È­¸é¿¡ ±×¸°´Ù.	

	pOnlineFieldEffect->Pool();

	
	DWriteStringToFile("±×¸®±â 1");
	if(fRender)
	{
		FieldDraw();	
	}
	DWriteStringToFile("±×¸®±â 2");
	

	if( bExitOnlineGame )
	{
		siMainPollStatus = ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}

	if(pOnlineVillage == NULL)
	{
		// È¯°æ¼³Á¤
		pOnlineSetting->Action();
	}

	DWriteStringToFile("³¡");

	return uiNextMenu;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÇÊµå¿¡¼­ È­¸é Ãâ·ÂºÎºÐÀ» Ã³¸®ÇÑ´Ù.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FieldDraw()
{
	HDC		hdc;
	HFONT	hOldFont;
	SI32	siX, siY;
	CHAR	szBuffer[1024];
	CHAR	szBuffer2[1024];
	CHAR	szChatBuffer[1024];	
	SI16	siHeroX, siHeroY;
	DWORD	TempTickCount;
	BOOL	fFieldRender;
	static DWORD	YetTempTickCount = 0;

	TempTickCount = GetTickCount();

	fFieldRender = TRUE;
	if(pOnlineVillage)
	{
		if(pOnlineVillage->CheckFieldRender() == FALSE)
		{
			fFieldRender = FALSE;
		}
	}

	if(fFieldRender)
	{
		// Å¸ÀÏ, Ä³¸¯ÅÍ, ¿ÀºêÁ§Æ®¸¦ ±×¸°´Ù.	
		pOnlineMap->Draw(GETSCREENSURFACE(this));	
		
		// robypark 2004/10/25 13:27
		// °ø¼ºÀü ÀüÅõ È¿°ú ±×¸®±â
		m_SiegeWarfare_BulletMgr.Draw();
		m_SiegeWarfare_SmokeMgr.Draw();

		pOnlineFieldEffect->Render(GETSCREENSURFACE(this));

		pOnlineMousePoint->Draw();

		pIOnlineChar->AllDrawSpeech(GETSCREENSURFACE(this));

		// NPC ¸»ÇÏ±â
		m_pNpcTalk->Action();
		m_pNpcTalk->Draw(GETSCREENSURFACE(this));

		// Macro
		m_pChatMacro->Draw(GETSCREENSURFACE(this));
		
		// robypark 2004/11/13 16:37
		// °ø¼ºÀü À¯´Ö Á¤º¸ Ã¢µéÀ» ¸ðµÎ Ç¥½ÃÇÏ´Â °æ¿ì Ã³¸®
		if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
			{
				// HDC¸¦ ÀÌ¿ëÇÏ¿© Âï¾î¾ß ÇÒ Ä³¸¯ÅÍ¿¡ ´ëÇÑ Á¤º¸¸¦ Âï¾îÁØ´Ù.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// ¸ðµç °ø¼ºÀü À¯´ÖÀÇ Ä³¸¯ÅÍ¸í ±×¸®±â
					pIOnlineChar->AllDrawName(hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);

					if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
					{
						// ¸ðµç °ø¼ºÀü À¯´Ö Ã¼·Â ¹× º¸±Þ °ÔÀÌÁö ±×¸®±â
						pIOnlineChar->AllDrawSiegeWarfareUnit_StatusBar();

						// ¸ðµç °ø¼º À¯´Ö »ó´Ü ±ê¹ß ±×¸®±â
						pIOnlineChar->AllDrawSiegeWarfareUnit_GuildFlag();

						clGrp.UnlockSurface(GETSCREENSURFACE(this));
					}
				}	// if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)

		// ÇöÀç ¸¶¿ì½º°¡ Ä³¸¯ÅÍ¸¦ °¡¸£Å°°í ÀÖÀ¸¸é
		if(m_SelectCharacterIndex != -1)
		{
			// °ø¼ºÀü À¯´ÖÀÏ °æ¿ì Ã¼·Â °ÔÀÌÁö Ãâ·ÂÇÏ±â
			if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
			{
				// HDC¸¦ ÀÌ¿ëÇÏ¿© Âï¾î¾ß ÇÒ Ä³¸¯ÅÍ¿¡ ´ëÇÑ Á¤º¸¸¦ Âï¾îÁØ´Ù.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// °ø¼ºÀü À¯´Ö Ä³¸¯ÅÍ¸í ±×¸®±â
					pIOnlineChar->DrawName(m_SelectCharacterIndex, hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);
				}

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// ¸ðµç °ø¼ºÀü À¯´Ö Ã¼·Â ¹× º¸±Þ °ÔÀÌÁö ±×¸®±â
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(m_SelectCharacterIndex);

					// ¸ðµç °ø¼º À¯´Ö »ó´Ü ±ê¹ß ±×¸®±â
					pIOnlineChar->DrawGuildFlag(m_SelectCharacterIndex);

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}
		}

		// HDC¸¦ ÀÌ¿ëÇÏ¿© Âï¾î¾ß ÇÒ Ä³¸¯ÅÍ¿¡ ´ëÇÑ Á¤º¸¸¦ Âï¾îÁØ´Ù.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);

			hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
			SetTextColor(hdc, RGB(0, 0, 0));
			
			pIOnlineChar->DrawName(pMyData->GetMyCharUniqueID(), hdc);		

			SelectObject(hdc, hOldFont);
			GETSCREENSURFACE(this)->ReleaseDC(hdc);

			// robypark 2004/11/12 21:28
			// °ø¼ºÀü À¯´Ö Ã¼·Â ¹× º¸±Þ °ÔÀÌÁö ±×¸®±â
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// °ø¼ºÀü À¯´Ö Ã¼·Â ¹× º¸±Þ °ÔÀÌÁö ±×¸®±â
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(pMyData->GetMyCharUniqueID());

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}
		}	
		
		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// »ó´Ü ±ê¹ßÀ» Âï´Â´Ù.
			pIOnlineChar->DrawGuildFlag(pMyData->GetMyCharUniqueID());

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	
	}

	// ÇÊµå¿¡¼­ °Ç¹° ÀÛ¾÷À» ÇÏ°í ÀÖÀ»¶§
	if(pOnlineMap->m_MapInfo.uiVillageCode)
	{
		if(pOnlineMap->m_MapInfo.uiVillageCode == pOnlineVillageInfoData->m_uiVillageCode)
		{
			float		TempFloat;
			SI16		SignStartPos;
			XSPR		*lpBoard;

			SignStartPos = 0;
			if(pOnlineVillageInfoData->m_siFrame < 16)
			{
				TempFloat    = (float)sin(((float)pOnlineVillageInfoData->m_siFrame / 5.625f) * 0.017444f);
				SignStartPos = -222 + (int)(TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * 222.0f);
			}

			lpBoard = pOnlineResource->GetxsprGame(ON_GAMESPR_DOWNTOWN_BOARDER);
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// ¸¶À» °£ÆÇ
				clGrp.PutSpriteT(-1 + SignStartPos, 0, lpBoard->Header.Xsize, lpBoard->Header.Ysize, lpBoard->Image);

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

			// ¸¶À» ÀÌ¸§À» Ãâ·ÂÇØÁØ´Ù.
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				::SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAT));
				::SetBkMode(hdc, TRANSPARENT);
				::SetTextColor(hdc, RGB(240, 240, 240));

				strcpy(szBuffer, pOnlineText->Get(pOnlineVillageInfoData->m_lpVillageHeader->siVillageName));
				pOnlineMegaText->DrawTextCenter(hdc, 23 + SignStartPos, 12, 120, 17, szBuffer);		

				MakeMoneyText(pOnlineVillageInfoData->m_VillageInfo.uiPopulation, (char*)szBuffer);
				sprintf((char*)szBuffer2, pOnlineText->Get(ON_TEXT_VILINFO_PEOPLE), szBuffer);

				pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 63, szBuffer2, RGB(10, 10, 10));		
				pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 62, szBuffer2, RGB(210, 210, 210));		

				if(pOnlineVillageInfoData->m_VillageInfo.m_MaxInvestmentID[0] == NULL) sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_MAXINVEST_NONE));
				else                                                                   sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_MAXINVEST_NAME), pOnlineVillageInfoData->m_VillageInfo.m_MaxInvestmentID);
				pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 83, szBuffer, RGB(10, 10, 10));
				pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 82, szBuffer, RGB(210, 210, 210));

				if(pOnlineVillageInfoData->m_VillageInfo.szGuildID[0] == NULL) sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_GUILD_NONE));
				else                                                           sprintf((char*)szBuffer, pOnlineText->Get(ON_TEXT_VILINFO_GUILD_NAME), pOnlineVillageInfoData->m_VillageInfo.szGuildID);
				pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 103, szBuffer, RGB(10, 10, 10));
				pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 102, szBuffer, RGB(210, 210, 210));

				GETSCREENSURFACE(this)->ReleaseDC(hdc);
			}
		}
	}

/*	if(pOnlineVillage)
	{
		if(pOnlineVillage->PollField() == FALSE)
		{
			pOnlineVillage->FreeMapStruct();
			delete pOnlineVillage;
			pOnlineVillage = NULL;
		}
	}
	else
*/
	{
		// ÀÏ¹Ý »óÅÂÀÏ¶§
		// ¹Ì´Ï¸Ê Ã¢ÀÌ È°¼ºÈ­ µÇ¾î ÀÖ´Ù¸é Ãâ·Â~~
		if(pOnlineMiniMap->IsActive())
			pOnlineMiniMap->Draw(GETSCREENSURFACE(this));

		pOnlineParty->Draw(GETSCREENSURFACE(this));

		pOnlineTradeBook->Draw( GETSCREENSURFACE(this) );

		// °æ¸ÅÁ¢¼öÃ¢ 
		pOnlineReception->Draw(GETSCREENSURFACE(this));

		// ´ëÀåÀåÀÌ
		pOnlineSmith->Draw(GETSCREENSURFACE(this));

		// robypark 2005/2/2 16:18
		// °ø¼º À¯´Ö ÀÎÅÍÆäÀÌ½º È°¼ºÈ­½Ã
		if ((pOnlinePannel->GetSiegeWarfareUnitUIHandle()->IsVisibleWindow())
			|| (pOnlinePannel->GetSiegeWarfareUnitUIHandle()->IsMinimizeWindow()))
		{
			pOnlinePannel->SetFlagVillage(FALSE, ONLINEPANNEL_DISABLEFLAG_CHARINFO/*¿ëº´Ã¢*/);
		}
		else
			pOnlinePannel->SetFlagVillage(FALSE, NULL);

		// ÆÇ³ÚÀ» ±×¸°´Ù.
//		pOnlinePannel->SetFlagVillage(FALSE, NULL);
		pOnlinePannel->Draw(GETSCREENSURFACE(this));

		// °Å·¡Ã¢ÀÌ È°¼ºÈ­ µÇ¾î ÀÖµû¸é
		if( pOnlineTrade->IsActive() )
		{	
			// ³» ÀÎº¥Åä¸®¿¡¼­ ¾ÆÀÌÅÛÀÌ µå·¡±× ÁßÀÌ¶ó¸é °Å·¡Ã¢À» ¸ÕÀú Âï¾î ÁÖ°í ÀÎº¥Åä¸®¸¦ Âï¾î ÁØ´Ù.
			if(pOnlineInventory->GetDragItemInfo() != -1 )
			{
				pOnlineTrade->Draw(GETSCREENSURFACE(this));
				m_pMercenary->Draw(GETSCREENSURFACE(this));
				if(pOnlineInventory->IsActive())	pOnlineInventory->Draw(GETSCREENSURFACE(this));
			}

			else if( pOnlineTrade->GetList()->GetDragSlot() != -1 )
			{
				m_pMercenary->Draw(GETSCREENSURFACE(this));
				if(pOnlineInventory->IsActive())	pOnlineInventory->Draw(GETSCREENSURFACE(this));
				pOnlineTrade->Draw(GETSCREENSURFACE(this));
			}
			
			else
			{
				pOnlineTrade->Draw(GETSCREENSURFACE(this));
				m_pMercenary->Draw(GETSCREENSURFACE(this));
				if(pOnlineInventory->IsActive())	pOnlineInventory->Draw(GETSCREENSURFACE(this));				
			}
		}
		else
		{
			// ¿ëº´ °ü·Ã Ãâ·Â			
			m_pMercenary->Draw(GETSCREENSURFACE(this));

			// ÁÂÆÇ
			if(pOnlineBooth->IsActionIn())
			{
				pOnlineBooth->Draw(GETSCREENSURFACE(this));	
				if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));
				pOnlineBooth->pOnlineMsgBoxBench->Draw(GETSCREENSURFACE(this));
			}
			else if(pOnlineBooth->IsActionOut())
			{
				if(pOnlineBooth->GetList()->GetDragSlot() != -1)
				{
					if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));
					pOnlineBooth->Draw(GETSCREENSURFACE(this));
				}
				else
				{
					pOnlineBooth->Draw(GETSCREENSURFACE(this));
					if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));
				}
				pOnlineBooth->pOnlineMsgBoxBench->Draw(GETSCREENSURFACE(this));
			}
			else
				if(pOnlineInventory->IsActive())		pOnlineInventory->Draw(GETSCREENSURFACE(this));

			pOnlineTrade->Draw(GETSCREENSURFACE(this));
		}

		// ÀÌ¸ðÆ¼ÄÜÀ» Ãâ·ÂÇÑ´Ù.
		pOnlineImoticon->Draw(GETSCREENSURFACE(this));	

		// È­¸éÀ¸·Î ¿Å±ä´Ù.
		// Á¤º¸¸¦ È­¸é¿¡ Âï¾îÁØ´Ù.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SI32 siScreenDotX, siScreenDotY;

			pOnlineMap->TransformMapXYToScreenDotCenterXY(2, 2, siScreenDotX, siScreenDotY);
			pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		

			if((m_GMFlag != ON_USERGRADE_NORMAL) && (m_InfoFlag == FALSE))
			{
				if(pIOnlineChar->GetShow(pMyData->GetMyCharUniqueID()))
				{
					pIOnlineChar->GetPos(pOnlineMap->GetCurrentFocusCharID(), &siHeroX, &siHeroY);
					sprintf((char*)szChatBuffer, "Player Pos(Tile) : %d, %d | Mouse Position(Pixel) : %d, %d", siHeroX, siHeroY, IpD.Mouse_X, IpD.Mouse_Y );
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 11, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 10, szChatBuffer, PaletteWhiteColor);			

					sprintf((char*)szChatBuffer, "Now Time : %dY %dM %dD %dH %dM", m_RealTime.uiYear, m_RealTime.uiMonth, m_RealTime.uiDay, m_RealTime.uiHour, m_RealTime.uiMinute);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 31, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 30, szChatBuffer, PaletteWhiteColor);
					
					
					sprintf((char*)szChatBuffer, "Now FPS : %d, Render Count : %d, Left Message Count : %d, Recv Data = %d K", YetTempTickCount == 0 ? 1000 : 1000 / YetTempTickCount, YetTempTickCount, pOnlineClient->m_pMsgBuffer.size(), pOnlineClient->m_RecvDataSize / 1024);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 51, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 50, szChatBuffer, PaletteWhiteColor);			

					// ÇöÀç Tile Buffer »ç¿ë·ü
					SI32		TempTempCount;
					SI32		TempTempBaseCount;

					TempTempBaseCount = 0;
					for(TempTempCount = 0; TempTempCount < MAX_ONLINE_TILE_BASEDATAFILE; TempTempCount++)
					{
						if(pOnlineResource->ImageBaseTileData[TempTempCount].m_TileIndex != -1) TempTempBaseCount++;
					}
					sprintf((char*)szChatBuffer, "Now Tile Buffer P, Base : %d/%d", TempTempBaseCount, MAX_ONLINE_TILE_BASEDATAFILE);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 71, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 70, szChatBuffer, PaletteWhiteColor);			


					TempTempBaseCount = 0;
					for(TempTempCount = 0; TempTempCount < MAX_ONLINE_OBJECT_BASEDATAFILE; TempTempCount++)
					{
						if(pOnlineResource->ImageBaseObjectData[TempTempCount].m_TileIndex != -1) TempTempBaseCount++;
					}
					sprintf((char*)szChatBuffer, "Now Object Buffer P, Base : %d/%d", TempTempBaseCount, MAX_ONLINE_OBJECT_BASEDATAFILE);
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					pOnlineMegaText->DrawText( hdc, 11, 91, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 90, szChatBuffer, PaletteWhiteColor);			

					// ÇöÀç ¼±ÅÃÇÑ °³Ã¼ Ãâ·Â
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					switch(pMousePointInfo->GetPointKind())
					{
					case ON_MOUSE_POINT_CHAR:
						// ¼±ÅÃÇÑ Ä³¸¯ÅÍ Ç¥½Ã
						if(pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam())))
						{
							// PlayerÀÎÁö

							char*		lpGuild = pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam());

							if(lpGuild)
							{
								if(lpGuild[0] == NULL)
								{
									lpGuild = NULL;
								}
							}

							if(lpGuild != NULL)
							{
								XSPR*		lpXSPR;

								// ±æµå¿øÀÏ °æ¿ì

								lpXSPR = pOnlineResource->GetxsprFlagM(pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								if(lpXSPR)
								{
									if(lpXSPR->Image == NULL)
									{
										lpXSPR = NULL;
									}
								}

								if(lpXSPR)
								{
									// ±æµå ±ê¹ßÀÌ ÀÖ´Â°æ¿ì

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d, Flag : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								}
								else
								{
									// ±æµå ±ê¹ßÀÌ ¾ø´Â°æ¿ì

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()));
								}
							}
							else
							{
								// ±æµå¿øÀÌ ¾Æ´Ò°æ¿ì

								sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\"", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()));
							}
						}
						else
						{
							// MonsterÀÎÁö

							sprintf((char*)szChatBuffer, "Now Select : Monster, UNID : %d", (UI32)pMousePointInfo->GetParam());
						}
						pOnlineMegaText->DrawText( hdc, 11, 111, szChatBuffer, PaletteDataBlack);			
						pOnlineMegaText->DrawText( hdc, 10, 110, szChatBuffer, PaletteWhiteColor);			
						break;				
					case ON_MOUSE_POINT_OBJECT:
						sprintf((char*)szChatBuffer, "Now Select : Object, ID : %d", pMousePointInfo->GetParam());
						break;
					case ON_MOUSE_POINT_ITEM:
						UI32		uiItemID;

						uiItemID		=	pMousePointInfo->GetParam();				

						sprintf((char*)szChatBuffer, "Now Select : Item, ID : %d", uiItemID);
						break;
					case ON_MOUSE_POINT_MILEPOST:
						sprintf((char*)szChatBuffer, "Now Select : MilePost");
						break;
					case ON_MOUSE_POINT_INTERFACE:
						sprintf((char*)szChatBuffer, "Now Select : Interface");
						break;

					case ON_MOUSE_POINT_NPC:
						sprintf((char*)szChatBuffer, "Now Select : NPC");
						break;			

					default:
						sprintf((char*)szChatBuffer, "Now Select : Field, x : %d, y : %d", siX, siY);
						break;
					}
					pOnlineMegaText->DrawText( hdc, 11, 111, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 110, szChatBuffer, PaletteWhiteColor);			

					MapStoreHeader*			lpMapStoreHeader;
					SI16					MPointFlag;

					// ¿øÇÏ´Â ÀÚÆÇÀÇ Data¸¦ ¾ò¾î¿Â´Ù.
					MPointFlag = 0;
					lpMapStoreHeader = pOnlineMap->GetMapStoreParser(siX, siY, &MPointFlag);
					if(lpMapStoreHeader)
					{
						sprintf((char*)szChatBuffer, "Booth : Field, x : %d, y : %d, Pos : %d", siX, siY, MPointFlag);
						pOnlineMegaText->DrawText( hdc, 11, 131, szChatBuffer, PaletteDataBlack);			
						pOnlineMegaText->DrawText( hdc, 10, 130, szChatBuffer, PaletteWhiteColor);			
					}

					// PK Flag
					sprintf((char*)szChatBuffer, "PK Flag : %s", CheckPKFieldMap() ? "TRUE" : "FALSE");
					pOnlineMegaText->DrawText( hdc, 11, 151, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 150, szChatBuffer, PaletteWhiteColor);

					// Tile Draw Time
					sprintf((char*)szChatBuffer, "Tile Draw Time : %I64d", dwMapTileDrawTime);
					pOnlineMegaText->DrawText( hdc, 11, 171, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 170, szChatBuffer, PaletteWhiteColor);

					char tempbuf[1024];
					sprintf( tempbuf, "Work(%3d) WorkComplete(%3d) Free(%3d) FreeComplete(%3d)",
					pCharFileLoadManager->pOnlineLoadXSprProc->GetWorkQueueLength(),
					pCharFileLoadManager->pOnlineLoadXSprProc->GetCompleteQueueLength(),
					pCharFileLoadManager->pOnlineLoadXSprProc->GetFreeListLength(),
					pCharFileLoadManager->pOnlineLoadXSprProc->GetFreeQueueLength() );

					pOnlineMegaText->DrawText( hdc, 11, 191, szChatBuffer, PaletteDataBlack);			
					pOnlineMegaText->DrawText( hdc, 10, 190, tempbuf, PaletteWhiteColor );
				}
			}

			GETSCREENSURFACE(this)->ReleaseDC(hdc);
		}	

		ShowMessage(100, 35, GETSCREENSURFACE(this));

		// °æ¸ÅÃ¢ 
		pOnlineFieldAuction->Draw(GETSCREENSURFACE(this));

		// ÇÑ±¹¾î ¹öÁ¯ÀÏ¶§¸¸ Ç¥±âµÈ´Ù.
//		if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//		{
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// actdoll (2004/04/02 15:28) : = ÀÌº¥Æ® = µ¶µµ Äù½ºÆ® ÁøÇà ÁßÀÎ°¡ Ç¥½Ã
//				if( m_bReceivedItem || m_bEventDokdoQuestPlay )
//				{
//					DWORD	dwTemp = timeGetTime();
//					
//					if( ( dwTemp - m_dwDelay ) > 500 )
//					{
//						m_bReceivdeItemShow = !m_bReceivdeItemShow;
//						m_dwDelay	=	dwTemp;
//					}
//					if(m_bReceivdeItemShow)
//					{
//						if( m_bReceivedItem )			clGrp.PutSpriteT(746, 465, m_ReceivedItemAniSpr.Header.Xsize, m_ReceivedItemAniSpr.Header.Ysize, m_ReceivedItemAniSpr.Image );
//						if( m_bEventDokdoQuestPlay )	clGrp.PutSpriteT(700, 465, m_Spr_Icon_DokdoQuest.Header.Xsize, m_Spr_Icon_DokdoQuest.Header.Ysize, m_Spr_Icon_DokdoQuest.Image );
//					}
//				}

				// ¹ÞÀº¾ÆÀÌÅÛ Ç¥½Ã
				if( m_bReceivedItem )
				{
					DWORD	dwTemp = timeGetTime();
					
					if( ( dwTemp - m_dwDelay ) > 500 )
					{
						m_bReceivdeItemShow = !m_bReceivdeItemShow;
						m_dwDelay	=	dwTemp;
					}
					if(m_bReceivdeItemShow)		clGrp.PutSpriteT(746, 465, m_ReceivedItemAniSpr.Header.Xsize, m_ReceivedItemAniSpr.Header.Ysize, m_ReceivedItemAniSpr.Image );
				}
				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}
//		}

		POINT pt;
		pt.x	= IpD.Mouse_X;
		pt.y	= IpD.Mouse_Y;

		// ¹ÞÀº¾ÆÀÌÅÛ °øÁö
		if(m_bReceivedItem)
		{
			if(PtInRect( &m_rtReceivedItem, pt ) == TRUE)
			{
				pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_ITEM_ARRIVED_DESCRIPT),IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
			}
		}

		// ÇÑ±¹¾î ¹öÁ¯ÀÏ¶§¸¸ Ç¥±âµÈ´Ù.
//		if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//		{
//			// actdoll (2004/04/02 15:28) : = ÀÌº¥Æ® = µ¶µµ Äù½ºÆ® ÁøÇà ÁßÀÎ°¡ °øÁö
//			if(m_bEventDokdoQuestPlay)
//			{
//				if(PtInRect( &m_rtIconDokdoQuest, pt ) == TRUE)
//				{
//					pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_EVENT_PLAYING_DOKDO_QUEST),IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE );
//				}
//			}
//		}



		

		// µµ¿ò¸» Ãâ·Â
		SI16 siDrawedHelpPosX = 0;
		SI16 siDrawedHelpPosY = 0;
		int helpx = pOnlineHelp->Show(GETSCREENSURFACE(this), &siDrawedHelpPosX, &siDrawedHelpPosY);

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			XSPR*		lpXSPR;

			// ÇöÀç ¸¶¿ì½º°¡ Ä³¸¯ÅÍ¸¦ °¡¸£Å°°í ÀÖÀ¸¸é
			if(m_SelectCharacterIndex != -1)
			{
				lpXSPR = pOnlineResource->GetxsprFlagM(pIOnlineChar->GetGuildFlag(m_SelectCharacterIndex));
				if(lpXSPR)
				{
					if(lpXSPR->Image)
					{
						clGrp.PutSpriteT(m_SelectCharacterNameDrawX + 5 + helpx, m_SelectCharacterNameDrawY + 7, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[(GetTickCount() / 120) % lpXSPR->Header.font_number]]);
					}
				}

				/*
				// robypark 2005/1/8 12:26
				// °ø¼ºÀü À¯´ÖÀÏ °æ¿ì Ã¼·Â, º¸±ÞÇ° °ÔÀÌÁö Ãâ·ÂÇÏ±â
				if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
				{
					// °ø¼ºÀü À¯´Ö Ã¼·Â °ÔÀÌÁö¸¸ Ãâ·Â
					XSPR	*psprGaugeHealth = NULL;

					// »ç¿ëÀÚ¿Í °°Àº »ó´ÜÀÎ°¡?
					if (IsMemberOfSameGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
					}
					// Àû´ë »ó´Ü¿øÀÎ°¡?
					else if (pIOnlineChar->GetIsHostilityGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
					}
					else	// ±× ¹ÛÀÇ °æ¿ì ¾È±×¸°´Ù.
						psprGaugeHealth = NULL;

					if (psprGaugeHealth)
					{
						// ±×¸®±â
						DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
												siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
												m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct + 1,
												psprGaugeHealth);
					}

					// °ø¼ºÀü À¯´Ö º¸±Þ°ÔÀÌÁö¸¦ Ãâ·ÂÇØ¾ß ÇÏ´Â °æ¿ì¶ó¸é
					if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods)
					{
						// °ø¼ºÀü À¯´Ö º¸±ÞÇ° °ÔÀÌÁö Ãâ·Â
						XSPR	*psprGaugeSupplyGoodsAmount = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

						if (psprGaugeSupplyGoodsAmount)
						{
							// ±×¸®±â
							DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
													siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY + 3 + 1,
													m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeSupplyGoodsPct + 1,
													psprGaugeSupplyGoodsAmount);
						}
					}
				}
				*/
			}

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	

		// ¸Þ¼¼Áö ¹Ú½º Ãâ·Â
		pOnlineMsgBox->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxUsePortal->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxError->Draw(GETSCREENSURFACE(this));

		// TIP Ãâ·Â
		pOnlineTip->Draw(GETSCREENSURFACE(this));

		// Quest Play
		pOnlineQuestPlay->Draw(GETSCREENSURFACE(this));

		// Quest
		m_pQuestInterface->Draw(GETSCREENSURFACE(this));

//		//ÃÊº¸ÀÚ µµ¿ò¸»
//		pOnlineBasicTip->Draw(GETSCREENSURFACE(this));

		//ÃÊº¸ÀÚ Äù½ºÆ®.
//		pOnlineBasicQuest->Draw(GETSCREENSURFACE(this));
		
		
		// ¾ÆÀÌÅÛ È¹µæ Ã¢À» º¸¿©ÁØ´Ù. 
		pGame->pItemInfoWindow->Render(GETSCREENSURFACE(this));

		// ¸Þ½ÅÀú Ãâ·Â
		pOnlineMessenger->Draw(GETSCREENSURFACE(this));

		// ¼ýÀÚÆÇ µå·Î¿ì
		pOnlineNumPad->pNumberControl->Draw( GETSCREENSURFACE(this) );

		// È¯°æ¼³Á¤ µå·Î¿ì
		pOnlineSetting->Draw(GETSCREENSURFACE(this));

		//ÁØ¼®ÀÌÇü Ãß°¡ºÎºÐ 
		// ¸ðµç À©µµµéÀ» µå·Î¿ì
//		pJSWndMgr->Draw(GETSCREENSURFACE(this));

		pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(this));

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// ¸¶¿ì½º¸¦ Âï´Â´Ù.
			pIOnlineMouse->Draw();			

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}
		
		// for debug



		

		UpdateScreen(GETSCREENSURFACE(this));
	}

	YetTempTickCount = GetTickCount() - TempTickCount;
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	Ä³¸¯ÅÍ°¡ ¸¶À»»ó¿¡ ÀÖÀ»¶§ È£ÃâÇÏ´Â ÇÔ¼ö.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::VillagePoll(UI16 uiClientRet)
{
	m_siMsgBoxStatus = pOnlineMsgBox->Process();

	// ´ÜÃàÅ°¸¦ Ã³¸®ÇÑ´Ù.
	FieldShortCutKey();	
	pOnlineMousePoint->Clear();

	pOnlineMsgBoxPay->Process();
	if(pOnlineMsgBoxError->Process() == ON_MSGBOX_RETURN_OK && m_bForcedLogout)
	{
		pOnlineClientChatting->Close();
		return ON_MAINPOLL_FORCED_LOGOUT;
	}

	if(pOnlineVillage->Poll(uiClientRet) == TRUE)
	{
		pOnlineMiniMap->MakeTargetList();		
		return	ON_MAINPOLL_INITRECEIVEGOOUTVILLAGE;
	}
	else
	{
//		GotoField();
		return	0;
	}	
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÇØÁ¦.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::Free()
{

	fYetLight					=	0.0f;
	m_GameTickCount             =   0;

	bCharMove = TRUE;

	// ¸¶¿ì½º¸¦ ¾Èº¸ÀÌ°Ô ÇØÁØ´Ù.
	ShowSystemCursor(FALSE);		

	// Ä³¸¯ÅÍ ÀÎÅÍÆäÀÌ½º¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pIOnlineChar)
	{
		pIOnlineChar->Free();

		delete pIOnlineChar;
		pIOnlineChar = NULL;
	}

	delete m_pMercenary;
	delete m_pChatMacro;
	delete m_pQuestInterface;

	// actdoll (031229)	: ÃÊ»ó µ¥ÀÌÅÍ ÇØÁ¦
	delete m_pPortrait;
	delete m_pNpcTalk;	

	g_FileLoadManager.Free();	// Á¶ÇÕÇü Ä³¸¯ÅÍ Free()	

	m_cltIGuildClass.Free();	

	// actdoll (2004/11/03 17:19) : IMEÄÁÆ®·Ñ·¯ ÇØÁ¦
	OnlineInputStatusControl::Free();

	if(m_ReceivedItemAniSpr.Image)  {clGrp.FreeXspr(m_ReceivedItemAniSpr);  m_ReceivedItemAniSpr.Image  = NULL;}
//	if(m_Spr_Icon_DokdoQuest.Image)  {clGrp.FreeXspr(m_Spr_Icon_DokdoQuest);  m_Spr_Icon_DokdoQuest.Image  = NULL;}

	if(pOnlineLoadingData)
	{
		// actdoll (2004/06/29 12:57) : ´©¼ö Ã³¸®
		if( pOnlineLoadingData->BackImage.Image )		clGrp.FreeXspr( pOnlineLoadingData->BackImage );
		if( pOnlineLoadingData->BackOverImage.Image )	clGrp.FreeXspr( pOnlineLoadingData->BackOverImage );
		if( pOnlineLoadingData->AniLogoImage.Image )	clGrp.FreeXspr( pOnlineLoadingData->AniLogoImage );
		if( pOnlineLoadingData->PlayGradeImage.Image )	clGrp.FreeXspr( pOnlineLoadingData->PlayGradeImage );
		if( pOnlineLoadingData->m_Spr_CompanyLogo.Image )	clGrp.FreeXspr( pOnlineLoadingData->m_Spr_CompanyLogo );

		delete pOnlineLoadingData;
		pOnlineLoadingData = NULL;
	}
	
	if( pOnlineMsgProc )
	{
		delete pOnlineMsgProc;
		pOnlineMsgProc = NULL;
	}
	if(pMemoryProtected)
	{
		delete pMemoryProtected;
		pMemoryProtected = NULL;
	}

	// Palette Manager ÇØÁ¦
	if(pPaletteManager)
	{
		pPaletteManager->Free();

		delete pPaletteManager;
		pPaletteManager = NULL;
	}

	if( pOnlineTradeBook )
	{
		pOnlineTradeBook->Free();
		delete pOnlineTradeBook;
		pOnlineTradeBook			=	NULL;
	}

	// TicketParser
	if(pTicketParser)
	{
		pTicketParser->Free();
		delete pTicketParser;
		pTicketParser = NULL;
	}

	// GeneralParser
	if(pGeneralParser)
	{
		pGeneralParser->Free();
		delete pGeneralParser;
		pGeneralParser = NULL;
	}

	if(pGeneralParserEx)
	{
		pGeneralParserEx->Free();
		delete pGeneralParserEx;
		pGeneralParserEx = NULL;
	}

	// Quest
	if(pQuestParser)
	{
		pQuestParser->Free();
		delete pQuestParser;
		pQuestParser = NULL;
	}

	// Level Mark Parser
	if(pLevelMarkParser)
	{
		pLevelMarkParser->Release();
		delete pLevelMarkParser;
		pLevelMarkParser = NULL;
	}

	if( pOnlineTrade )
	{
		if( pOnlineTrade->IsActive() )
			pOnlineTrade->SetActive( FALSE );

		delete pOnlineTrade;
		pOnlineTrade				=	NULL;
	}

	// Timer ÇØÁ¦
	if(pTimer)
	{
		pTimer->Release();
		delete pTimer;
		pTimer = NULL;
	}
	fFirstGetTime = FALSE;
	if(pOnlineMercenaryParser)
	{
		pOnlineMercenaryParser->Free();
		delete pOnlineMercenaryParser;
		pOnlineMercenaryParser = NULL;
	}
	if(pOnlineEconomy)
	{
		pOnlineEconomy->Free();
		delete pOnlineEconomy;
		pOnlineEconomy = NULL;
	}
	// Ä³¸¯ÅÍÀÇ Á¾·ù ÀÎÅÍÆäÀÌ½º¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pIOnlineCharKI)
	{
		pIOnlineCharKI->Free();

		delete pIOnlineCharKI;
		pIOnlineCharKI =	 NULL;
	}

	// ¿ÀºêÁ§Æ® Á¾·ùÀÇ ÀÎÅÍÆäÀÌ½º¸¦  ÇØÁ¦ÇÑ´Ù.
	if(pIOnlineObjectKI)
	{
		pIOnlineObjectKI->Free();

		delete pIOnlineObjectKI;
		pIOnlineObjectKI = NULL;
	}

	// ¸¶À» °´Ã¼¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pOnlineVillage)
	{
		delete pOnlineVillage;
		pOnlineVillage = NULL;
	}

	// ¸¶À» ¼³¸í List ÇØÁ¦
	if(pOnlineVillageDescriptList)
	{
		delete pOnlineVillageDescriptList;
		pOnlineVillageDescriptList = NULL;
	}

	// °øÁö»çÇ× List ÇØÁ¦
	if(pOnlineNoticeList)
	{
		delete pOnlineNoticeList;
		pOnlineNoticeList = NULL;
	}

	// ÇÊµå»ó Effect
	if(pOnlineFieldEffect)
	{
		delete pOnlineFieldEffect;
		pOnlineFieldEffect = NULL;
	}

	// ±æÃ£±â ÀÎ°øÁö´ÉÀ» »èÁ¦ÇÑ´Ù.
	if(pclFindPath)
	{
		delete pclFindPath;
		pclFindPath = NULL;
	}
		
	// ÀÌ¸ðÆ¼ÄÜ
	if(pOnlineImoticon)
	{
		pOnlineImoticon->Free();

		delete pOnlineImoticon;
		pOnlineImoticon = NULL;
	}

	// ³ªÀÇ µ¥ÀÌÅ¸¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pMyData)
	{	
		pMyData->Free();

		delete pMyData;
		pMyData = NULL;
	}

	// robypark 2005/1/14 17:11
	// ÁÖÀÎ°ø Ä³¸¯ÅÍµéÀÇ Á¤º¸ °ü¸®ÀÚ ÃÊ±âÈ­
	if (m_pFieldHeroInfoMgr != NULL)
	{
		delete m_pFieldHeroInfoMgr;
		m_pFieldHeroInfoMgr = NULL;
	}

	// robypark 2005/1/31 12:55
	// °ø¼º À¯´Ö °ü¸®ÀÚ
	if (NULL != m_pSiegeUnitTableMgr)
	{
		delete m_pSiegeUnitTableMgr;
		m_pSiegeUnitTableMgr = NULL;
	}

	// ³×Æ®¿öÅ© ¿¬°áÀ» ÇØÁ¦ÇÑ´Ù.
	if(pOnlineClient)
	{		
		delete pOnlineClient;
		pOnlineClient = NULL;
	}
	if(pOnlineClientChatting)
	{		
		delete pOnlineClientChatting;
		pOnlineClientChatting = NULL;
	}
//	if(m_pMacroProcess != NULL)
//	{
//		delete m_pMacroProcess;
//		m_pMacroProcess = NULL;
//	}

	WSACleanup();

	if(pOnlineResource)
	{
		pOnlineResource->Free();
		delete pOnlineResource;
		pOnlineResource = NULL;
	}

	if(m_pOnlineMapSound)
	{
		delete m_pOnlineMapSound;
		m_pOnlineMapSound = NULL;
	}

	// ¸Ê°ü¸®ÀÚ¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pOnlineMap)
	{
		pOnlineMap->Free();

		delete pOnlineMap;
		pOnlineMap	=	NULL;
	}		

	if(pOnlineCharUI)
	{	
		pOnlineCharUI->Free();		

		delete pOnlineCharUI;
		pOnlineCharUI	=	NULL;
	}

	if(pOnlineInventory)
	{
		pOnlineInventory->Free();

		delete pOnlineInventory;
		pOnlineInventory	=	NULL;
	}

	if(pOnlineKeyboard)
	{
		delete pOnlineKeyboard;

		pOnlineKeyboard		=	NULL;
	}
	
	if(pOnlineFonts)
	{
		pOnlineFonts->Free();

		delete pOnlineFonts;
		pOnlineFonts		=	NULL;
	}

	if(pOnlineText)
	{
		delete pOnlineText;
		pOnlineText	=	NULL;
	}	

	if(pOnlineDirectDraw)
	{
		pOnlineDirectDraw->Release();
		pOnlineDirectDraw = NULL;
	}

	if(pConfig)
	{
		// Config ÆÄÀÏÀ» ÀúÀåÇÔ.
		pConfig->SaveConfig();
		delete pConfig;
		pConfig		=	NULL;
	}

	if(pOnlineMegaText)
	{
		pOnlineMegaText->ReMoveAll();

		delete pOnlineMegaText;
		pOnlineMegaText	=	NULL;
	}
	
/*	if(pOnlineCharAni)
	{
		pOnlineCharAni->Free();

		delete pOnlineCharAni;
		pOnlineCharAni	=	NULL;
	}	*/
	
	if(pOnlineCharActionCode)
	{
		delete pOnlineCharActionCode;
		pOnlineCharActionCode	=	NULL;
	}

	
	if(pIOnlineMouse)
	{
		pIOnlineMouse->Free();

		delete pIOnlineMouse;
		pIOnlineMouse			=	NULL;
	}	
	if(pOnlineMousePoint)
	{
		pOnlineMousePoint->Free();
		delete pOnlineMousePoint;
		pOnlineMousePoint = NULL;
	}

	if(pOnlineFieldArea)
	{
		pOnlineFieldArea->Free();

		delete pOnlineFieldArea;
		pOnlineFieldArea		=	NULL;
	}	

	if(pOnlineVillageManager)
	{
		pOnlineVillageManager->Free();

		delete pOnlineVillageManager;
		pOnlineVillageManager		=	NULL;
	}
	
	if(pOnlinePannel)
	{
		pOnlinePannel->Free();

		delete pOnlinePannel;
		pOnlinePannel				=	NULL;
	}	

	if(pOnlineMiniMap)
	{
		pOnlineMiniMap->Free();
		delete pOnlineMiniMap;
		pOnlineMiniMap				=	NULL;
	}

	if(pOnlineHelp)
	{
		pOnlineHelp->FreeHelp();
		delete pOnlineHelp;
		pOnlineHelp					=	NULL;
	}

	if(pMousePointInfo)
	{
		delete	pMousePointInfo;
		pMousePointInfo				=	NULL;
	}

	if(pOnlineNumPad)
	{
		pOnlineNumPad->pNumberControl->Free();
		delete pOnlineNumPad->pNumberControl;
		delete pOnlineNumPad;

		pOnlineNumPad				=	NULL;
	}

	if( pOnlineMsgBox )
	{		
		delete pOnlineMsgBox;
		pOnlineMsgBox				=	NULL;		
	}

	if( pOnlineMsgBoxPay )
	{
		delete pOnlineMsgBoxPay;
		pOnlineMsgBoxPay			=	NULL;
	}

	if( pOnlineMsgBoxUsePortal )
	{
		delete pOnlineMsgBoxUsePortal;
		pOnlineMsgBoxUsePortal		=	NULL;
	}

	if( pOnlineMsgBoxError )
	{
		delete pOnlineMsgBoxError;
		pOnlineMsgBoxError			=	NULL;
	}

	if( pOnlineLoginMsgBox )
	{
		delete pOnlineLoginMsgBox;
		pOnlineLoginMsgBox			=	NULL;
	}

		
	if( pOnlineVillageSave )
	{
		pOnlineVillageSave->Free();
		delete pOnlineVillageSave;
		pOnlineVillageSave			=	NULL;
	}
	
	
	if( pOnlineBuilding )
	{
		delete pOnlineBuilding;
		pOnlineBuilding				=	NULL;
	}

	if( pItemsInfo )
	{		
		delete pItemsInfo;
		pItemsInfo					=	NULL;
	}

	if( pOnlineBooth )
	{
		delete pOnlineBooth;
		pOnlineBooth				=	NULL;
	}

	if( pOnlineBench )
	{
		delete [] pOnlineBench;
		pOnlineBench				=	NULL;
	}

	// ¿ÀºêÁ§Æ®ÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pIOnlineObject)
	{
		pIOnlineObject->Free();

		delete pIOnlineObject;
		pIOnlineObject		=	NULL;
	}

	if(pIOnlineShipKI)
	{
		pIOnlineShipKI->Free();
		
		delete pIOnlineShipKI;
		pIOnlineShipKI	=	NULL;
	}

	if( pOnlineTip )
	{
		pOnlineTip->Free();
		delete pOnlineTip;
		pOnlineTip	=	NULL;
	}

	if( pOnlineQuestPlay )
	{
		pOnlineQuestPlay->Free();
		delete pOnlineQuestPlay;
		pOnlineQuestPlay	=	NULL;
	}

/*	if( pOnlineBasicTip)
	{
		pOnlineBasicTip->Free();
		delete pOnlineBasicTip;
		pOnlineBasicTip	=	NULL;
	}
*/
/*	if(pOnlineBasicQuest)
	{
		pOnlineBasicQuest->Free();
		delete pOnlineBasicQuest;
		pOnlineBasicQuest = NULL;
	}*/

	if(pOnlineParty)
	{
		pOnlineParty->Free();
		delete pOnlineParty;
		pOnlineParty = NULL;
	}	 

	if( pOnlineMessenger )
	{
		pOnlineMessenger->Free();
		delete pOnlineMessenger;
		pOnlineMessenger =	NULL;
	}

	if( pOnlineDungeon )
	{
		pOnlineDungeon->Free();
		delete pOnlineDungeon;
		pOnlineDungeon	=	NULL;
	}

	if( pOnlineSetting )
	{
		pOnlineSetting->Free();
		delete pOnlineSetting;
		pOnlineSetting	=	NULL;
	}

	if( pIProfit )
	{
		pIProfit->Free();
		delete pIProfit;
		pIProfit = NULL;
	}

	if(g_pSoundManager)
	{
		g_pSoundManager->StopAllSound();
		g_pSoundManager->DelPlayID( BackgroundMusic );
	}

	if( pOnlineTradeParser )
	{
		delete pOnlineTradeParser;
		pOnlineTradeParser = NULL;
	}

	if( pOnlineSkillParser )
	{
		delete pOnlineSkillParser;
		pOnlineSkillParser = NULL;
	}

	if(pOnlineVillageInfoData)
	{
		delete pOnlineVillageInfoData;
		pOnlineVillageInfoData = NULL;
	}

	if( pCBuriedItemParser )
	{		
		delete pCBuriedItemParser;
		pCBuriedItemParser = NULL;
	}

	if (pOnlineReception)
	{
		delete pOnlineReception;
		pOnlineReception = NULL;
	}

	if (pOnlineFieldAuction)
	{
		delete pOnlineFieldAuction;
		pOnlineFieldAuction = NULL;
	}

	if (pOnlineSmith)
	{
		delete pOnlineSmith;
		pOnlineSmith = NULL;
	}

	if (pOnlineSmithParser)
	{
		delete pOnlineSmithParser;
		pOnlineSmithParser = NULL;
	}
	//ÁØ¼®ÀÌÇü Ãß°¡ºÎºÐ 
	
/*	if(pJSWndMgr)
	{
		delete pJSWndMgr;
		pJSWndMgr = NULL;
	}
*/	
	
/*	if(pJSTestWnd)
	{
		delete pJSTestWnd;
		pJSTestWnd = NULL;
	}
*/
/*	if(pOnlineMemMgr)
	{
		delete pOnlineMemMgr;
		pOnlineMemMgr = NULL;
	}
*/
//	if (pOnlineBanner)
//	{
//		delete pOnlineBanner;
//		pOnlineBanner = NULL;
//	}

	if (pOnlineCharParser)
	{
		pOnlineCharParser->Free();
		delete pOnlineCharParser;
		pOnlineCharParser = NULL;
	}

//	if(m_LogInInfo_Data)
//	{
//		delete m_LogInInfo_Data;
//		m_LogInInfo_Data = NULL;
//	}

	// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤ º¯¼ö
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	DeleteFile( "temp.spr" );

	clGrp.FlipToGDISurface();
	
//	MFGS2_Free();

	if(m_GameOption)
	{
		delete m_GameOption;
		m_GameOption = NULL;
	}

//	FreeLibrary(m_IME_DLLHandle);

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¹æÇâÀ» ±¸ÇÑ´Ù.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	cltOnlineWorld::GetDirection8(SI32 siX1, SI32 siY1, SI32 siX2, SI32 siY2)
{
	SI32	siDirection = 0;
	
	if(siX1 < siX2)		
	{	
		siDirection	|=	ON_EAST;		
	}
	else if(siX1 > siX2)	
	{
		siDirection	|=	ON_WEST;				
	}

	if(siY1 < siY2)		
	{
		siDirection	|=	ON_SOUTH;						
	}
	else if(siY1 > siY2)	
	{
		siDirection	|=	ON_NORTH;								
	}

	return siDirection;

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÇÁ·¹ÀÓ¼ö¸¦ ±¸ÇØ¿Â´Ù.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	cltOnlineWorld::GetTotalFrame()
{
	return uiTotalFrame;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¹é ¼­ÇÇ½º¸¦ ÇÁ¶óÀÌ¸Ó¸® ¼­ÇÇ½º¿¡ °»½Å ½ÃÅ²´Ù.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::UpdateScreen(LPDIRECTDRAWSURFACE7 pSurface, BOOL bSourceColorKey,BOOL bForceRedraw)
{
	DWORD					dwTrans;
	LPDIRECTDRAWCLIPPER		pClipper;
	HRESULT hResult =0;

	if(pSurface == NULL)	pSurface	=	clGrp.lpDDSBack;

	// actdoll (2004/11/03 17:28) : IME ÄÁÆ®·Ñ·¯ °ü·Ã...
	//	¾ÈÁÁÀº ¹æ¹ýÀÌÁö¸¸ ¹èÄ¡°¡ Áß±¸³­¹æÀÌ¶ó Ãâ·ÂÄÚµå¸¦ ¿©±â·Î ÀâÀ» ¼ö ¹Û¿¡ ¾ø´Ù.
	OnlineInputStatusControl::Draw( pSurface );

#ifdef	_DEBUG
#ifdef	_SHOW_INPUTSTATUS_
	// actdoll (2004/08/09 14:52) : [ÀÓ½Ã] IME ½ºÅ×ÀÌÅÍ½º ¾ò¾î¿À±â
	HDC		hDC;
	HRESULT hr = pSurface->GetDC(&hDC);
	if( hr == DD_OK )
	{
		SetBkMode( hDC, OPAQUE );
		HFONT	hOldFont = (HFONT)SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_ITEM));
		HIMC	hImc = ImmGetContext( pGame->Hwnd );
		if( hImc )
		{
			char	pszTemp[512];
			DWORD	dwRet1, dwRet2;
			BOOL	bOpenStatus = ImmGetOpenStatus( hImc );
			ImmGetConversionStatus( hImc, &dwRet1, &dwRet2 );
			
			pOnlineMegaText->DrawText( hDC, 11, 11, "[IME STATUS]", PaletteDataBlack);			
			pOnlineMegaText->DrawText( hDC, 10, 10, "[IME STATUS]", PaletteWhiteColor);

			sprintf( pszTemp, "OpenStatus : %d", bOpenStatus );
			pOnlineMegaText->DrawText( hDC, 11, 31, pszTemp, PaletteDataBlack);			
			pOnlineMegaText->DrawText( hDC, 10, 30, pszTemp, PaletteWhiteColor);

			sprintf( pszTemp, "CM_NATIVE | CM_KATAKANA | CM_FULLSHAPE | CM_ROMAN | CM_CHARCODE | CM_HANJACONVERT | CM_SOFTKBD | CM_NOCONVERSION | CM_EUDC | CM_SYMBOL | CM_FIXED |" );
			pOnlineMegaText->DrawText( hDC, 11, 51, pszTemp, PaletteDataBlack);			
			pOnlineMegaText->DrawText( hDC, 10, 50, pszTemp, PaletteWhiteColor);

			sprintf( pszTemp, "    %d    |     %d      |      %d      |    %d    |      %d     |        %d       |     %d     |        %d       |    %d   |     %d    |     %d   |",
								dwRet1&IME_CMODE_NATIVE,
								dwRet1&IME_CMODE_KATAKANA, 
								dwRet1&IME_CMODE_FULLSHAPE, 
								dwRet1&IME_CMODE_ROMAN, 
								dwRet1&IME_CMODE_CHARCODE, 
								dwRet1&IME_CMODE_HANJACONVERT, 
								dwRet1&IME_CMODE_SOFTKBD, 
								dwRet1&IME_CMODE_NOCONVERSION, 
								dwRet1&IME_CMODE_EUDC, 
								dwRet1&IME_CMODE_SYMBOL, 
								dwRet1&IME_CMODE_FIXED );
			pOnlineMegaText->DrawText( hDC, 11, 71, pszTemp, PaletteDataBlack);			
			pOnlineMegaText->DrawText( hDC, 10, 70, pszTemp, PaletteWhiteColor);

			sprintf( pszTemp, "SM_PLAURALCLAUSE | SM_SINGLECONVERT | SM_AUTOMATIC | SM_PHRASEPREDICT | SM_CONVERSATION" );
			pOnlineMegaText->DrawText( hDC, 11, 91, pszTemp, PaletteDataBlack);
			pOnlineMegaText->DrawText( hDC, 10, 90, pszTemp, PaletteWhiteColor);

			sprintf( pszTemp, "       %d        |        %d        |      %d      |        %d        |       %d      |",
								dwRet2&IME_SMODE_PLAURALCLAUSE,
								dwRet2&IME_SMODE_SINGLECONVERT, 
								dwRet2&IME_SMODE_AUTOMATIC, 
								dwRet2&IME_SMODE_PHRASEPREDICT, 
								dwRet2&IME_SMODE_CONVERSATION );
			pOnlineMegaText->DrawText( hDC, 11, 111, pszTemp, PaletteDataBlack);			
			pOnlineMegaText->DrawText( hDC, 10, 110, pszTemp, PaletteWhiteColor);

			ImmReleaseContext( pGame->Hwnd, hImc );
		}
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, hOldFont );
		pSurface->ReleaseDC( hDC );
	}
#endif// _SHOW_INPUTSTATUS_
#endif//_DEBUG

	//clGrp.lpDD7->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);	<<-- Á¤¸ð°¡ »èÁ¦...2003/06/13 23:11

	/*
	if(pSurface == clGrp.lpDDSBack)
	{
		clGrp.lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	}
	else
	*/
	
	if(clGrp.lpDDSPrimary->GetClipper(&pClipper) == DDERR_NOCLIPPERATTACHED)
	{
		// Å¬¸®ÆÛ°¡ ºÎÂøÀÌ µÇ¾îÀÖÁö ¾Ê´Ù.		
		dwTrans	=	DDBLT_WAIT;
		
		if(bSourceColorKey == TRUE)			dwTrans	|=	DDBLT_KEYSRC;
		
		clGrp.lpDDSPrimary->Blt(NULL, pSurface, NULL, dwTrans, NULL);
		/*
		if(clGrp.BltFast(SURFACE_PRIMARY, 0, 0, pSurface, NULL, dwTrans) != DD_OK)
		clGrp.Error("BltFast ¿¡·¯", "");
		*/
		clGrp.Error("BltFast Error", "");
	}
	else
	{	
		// Á¤¸ð°¡ »èÁ¦... 2003/06/13 23:18
		/*GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );
		*/
		
		dwTrans	=	DDBLT_WAIT;
		
		if(bSourceColorKey == TRUE)			dwTrans	|=	DDBLT_KEYSRC;
		
        if (!WindowModeSwitch) {
			// Á¤¸ð°¡ »èÁ¦... 2003/06/13 23:18
			/*RECT rc;
			GetWindowRect(pGame->Hwnd, &rc);*/
			hResult = clGrp.lpDDSPrimary->Blt( NULL, pSurface, NULL, dwTrans, NULL);
	
			// ¼­ÇÇ½º°¡ ·Î½ºÆ®µÈ °ÍÀÌ¸é, 
			if(hResult == DDERR_SURFACELOST)
				clGrp.Restore();
		}
		else 
		{				
			GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
			ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
			ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );

			HDC destHdc;
			HDC hdc;
			
			if (pSurface->GetDC(&hdc) == DD_OK) {
				clGrp.lpDDSConvertSurface->GetDC(&destHdc);
				BitBlt(destHdc, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize(), hdc, 0, 0, SRCCOPY);
				//GdiFlush();
				pSurface->ReleaseDC(hdc);
				clGrp.lpDDSConvertSurface->ReleaseDC(destHdc);
				
				clGrp.m_rcWindow.bottom = clGrp.m_rcWindow.top + 600;
				clGrp.m_rcWindow.right = clGrp.m_rcWindow.left + 800;
				
				switch(m_siUpdateMethod)
				{
				case 1:
					{

						RECT srcrect;
						srcrect.top = m_siUpdateCounter * 9;
						srcrect.bottom = 600 - m_siUpdateCounter * 9;
						srcrect.left = m_siUpdateCounter * 12;
						srcrect.right = 800 - m_siUpdateCounter * 12;
						clGrp.lpDDSPrimary->Blt( &clGrp.m_rcWindow, clGrp.lpDDSConvertSurface,
						&srcrect, DDBLT_WAIT , NULL );
					}
					break;
				default:
					clGrp.lpDDSPrimary->Blt( &clGrp.m_rcWindow, clGrp.lpDDSConvertSurface,
						NULL, DDBLT_WAIT , NULL );
					break;
				}

				lpCaptureSurface = pSurface;
			}
		}
	}
}	


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	Å¸ÀÏ ÁÂÇ¥¸¦ »ç¿ëÇÏ¿© °æµµ¿Í À§µµ¸¦ ¾ò¾î¿Â´Ù.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::GetLatitudeLongitude(SI32 siX, SI32 siY, SI32 *psiLongitudeDegree, SI32 *psiLongitudeMinute, SI32 *psiLatitudeDegree, SI32 *psiLatitudeMinute)
{
	SI32	siLongitude;
	SI32	siLatitude;

	siLongitude		=	ONLINE_START_LONGITUDE + (ONLINE_LONGITUDE_WIDTH * siX) / pOnlineMap->m_siXsize;
	siLatitude		=	ONLINE_START_LATITUDE - (ONLINE_LATITUDE_HEIGHT * siY) / pOnlineMap->m_siYsize;		

	*psiLongitudeDegree			=	siLongitude	/ 60;
	*psiLongitudeMinute			=	abs(siLongitude % 60);

	*psiLatitudeDegree			=	siLatitude / 60;	
	*psiLatitudeMinute			=	abs(siLatitude % 60);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¹®ÀÚ¿­·Î ¹æÇâÀ» °¡Áö°í ¼ýÀÚ·Î µÇ¾î ÀÖ´Â ¹æÇâÀ» ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	cltOnlineWorld::GetDirection(CHAR *pszDirection)
{
	SI32	siDirection =	0;

	if(strstr(pszDirection, "EAST") != 0)	siDirection |= ON_EAST;
	if(strstr(pszDirection, "WEST") != 0)	siDirection |= ON_WEST;
	if(strstr(pszDirection, "NORTH") != 0)	siDirection |= ON_NORTH;
	if(strstr(pszDirection, "SOUTH") != 0)	siDirection |= ON_SOUTH;

	return siDirection;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	½Ã½ºÅÛ ¸¶¿ì½º¸¦ º¸ÀÌ°Ô ÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::ShowSystemCursor(BOOL bShow)
{
	SI32 i = 0;

	if(bShow == TRUE)		
		while(i = ShowCursor(TRUE) < 0);
	else					
		while(i = ShowCursor(FALSE) >= 0);
}
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	·Î±×¾Æ¿ôÀ» ÇÏ°í ´ÙÀ½À¸·Î ÀÌµ¿ÇÒ ¸Þ´º¸¦ ¼³Á¤ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::SetLogOut(SI32 siNextMenu, SI32 siNextPoll, BOOL bGoGameServer)
{
	siNextMenuAfterLogOut		=	siNextMenu;
	siNextPollAfterLogOut		=	siNextPoll;
	bWillGoGameServer			=	bGoGameServer;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀÓÁø·Ï2 Game À¸·Î °¡±â À§ÇÏ¿© ÇØÁ¦ÇØ¾ß ÇÒ °ÍµîÀ» ÇØÁ¦ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::FreeForGotoImjinGame()
{/*
	// ¸®¼Ò½º¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pOnlineResource)
	{
		pOnlineResource->Free();

		delete pOnlineResource;
		pOnlineResource	=	NULL;
	}

	// ¸ÊÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pOnlineMap)	
	{
		pOnlineMap->Free();

		delete pOnlineMap;
		pOnlineMap	=	NULL;
	}	
*/
	// Ä³¸¯ÅÍÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pIOnlineChar)
	{
//		pIOnlineChar->AllFree();
//		delete pIOnlineChar;
//		pIOnlineChar		=	NULL;
	}

	// ¿ÀºêÁ§Æ®ÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇÑ´Ù.
	if(pIOnlineObject)
	{
		pIOnlineObject->Free();

		delete pIOnlineObject;
		pIOnlineObject		=	NULL;
	}

	// ¹è°æ À½¾ÇÀ» ÇØÁ¦ÇÑ´Ù.
	if(g_pSoundManager)
		g_pSoundManager->StopSound( BackgroundMusic );
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI16	cltOnlineWorld::DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// ¿É¼ÇÀÌ ÀÖ´Ù¸é, 
	if(option)
	{
		if(option != ' ')
		{
			switch(option)
			{
				case '/':   font = 10;  break;
				case '+':	font = 11;	break;
				case ',':	font = 12;	break;
				case '(':	font = 13;	break;
				case ')':	font = 14;	break;
				default:    font = 0;   SetMessage("No Operator."); break;
			}

			clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]]);
		}

		startx += number_xsize;
	}
	else
	{
		itoa(number, string, 10);

	    for(i = 0; i < lstrlen(string); i++)
		{
			if(string[i] != ' ')
			{
				switch(string[i])
				{
					case '/':   font = 10;						break;
					case '+':	font = 11;						break;
					case ',':	font = 12;						break;
					case '(':	font = 13;						break;
					case ')':	font = 14;						break;
					default:	font = startfont+string[i]-'0';	break;
				}

				clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]]);
			}
	  
			startx += number_xsize;
		}
	}

	return startx;
}
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber )
{
	SI16	i;
	SI16	siFont;	
	SI16	Add;

	if(pNumber == NULL)
	{
		siFont = 0;
		clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[ pXspr->Header.Start[siFont]]);
	}
	else
	{
		for(i = 0; i < (SI16)strlen(pNumber); i++)
		{
			Add = pXspr->Header.Xsize;
			if(pNumber[i] != ' ')
			{
				switch(pNumber[i])
				{
					case '/':   siFont = 10;				break;
					case '+':	siFont = 11;				break;
					case '-':	siFont = 12;				break;
					case '%':	siFont = 13;				break;
					case '.':	siFont = 14;	Add /= 2;	break;
					case '(':	siFont = 15;				break;
					case ')':	siFont = 16;				break;
					default:	siFont = pNumber[i] - '0';	break;
				}

				clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
			}
			startx += Add;
		}
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI16	cltOnlineWorld::DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// ¿É¼ÇÀÌ ÀÖ´Ù¸é, 
	if(option)
	{
		if(option != ' ')
		{
			switch(option)
			{
				case '/':   font = 10;								break;
				case '+':	font = 11;								break;
				case ',':	font = 12;								break;
				case '(':	font = 13;								break;
				case ')':	font = 14;								break;
				default:    font = 0;   SetMessage("No Operator."); break;
			}

			clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]]);
		}

		startx += number_xsize;
	}
	else
	{
		itoa(number, string, 10);

		if(pPaletteManager->GetPalette(PaletteDataRed))
		{
			for(i = 0; i < lstrlen(string); i++)
			{
				if(string[i] != ' ')
				{
					switch(string[i])
					{
						case '/':	font = 10;						break;
						case '+':	font = 11;						break;
						case ',':	font = 12;						break;
						case '(':	font = 13;						break;
						case ')':	font = 14;						break;
						default:	font = startfont+string[i]-'0';	break;
					}

					clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]], pPaletteManager->GetPalette(PaletteDataRed));
				}

				startx += number_xsize;
			}
		}
		else
		{
			for(i = 0; i < lstrlen(string); i++)
			{
				if(string[i] != ' ')
				{
					switch(string[i])
					{
						case '/':	font = 10;						break;
						case '+':	font = 11;						break;
						case ',':	font = 12;						break;
						case '(':	font = 13;						break;
						case ')':	font = 14;						break;
						default:	font = startfont+string[i]-'0';	break;
					}

					clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]]);
				}

				startx += number_xsize;
			}
		}
	}

	return startx;
}
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber )
{
	SI16	i;
	SI16	siFont;	

	if(pNumber == NULL)
	{
		siFont = 0;
		if(pPaletteManager->GetPalette(PaletteDataRed))
		{
			clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]], pPaletteManager->GetPalette(PaletteDataRed));
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
		}
	}
	else
	{
		if(pPaletteManager->GetPalette(PaletteDataRed))
		{
			for(i = 0; i < (SI16)strlen(pNumber); i++)
			{
				if(pNumber[i] != ' ')
				{
					switch(pNumber[i])
					{
						case '/':	siFont = 10;				break;
						case '+':	siFont = 11;				break;
						case ',':	siFont = 12;				break;
						case '(':	siFont = 13;				break;
						case ')':	siFont = 14;				break;
						default:	siFont = pNumber[i] - '0';	break;
					}

					clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]], pPaletteManager->GetPalette(PaletteDataRed));
				}
				startx += pXspr->Header.Xsize;
			}
		}
		else
		{
			for(i = 0; i < (SI16)strlen(pNumber); i++)
			{
				if(pNumber[i] != ' ')
				{
					switch(pNumber[i])
					{
						case '/':	siFont = 10;				break;
						case '+':	siFont = 11;				break;
						case ',':	siFont = 12;				break;
						case '(':	siFont = 13;				break;
						case ')':	siFont = 14;				break;
						default:	siFont = pNumber[i] - '0';	break;
					}

					clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
				}
				startx += pXspr->Header.Xsize;
			}
		}
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI16	cltOnlineWorld::DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// ¿É¼ÇÀÌ ÀÖ´Ù¸é, 
	if(option)
	{
		if(option != ' ')
		{
			switch(option)
			{
				case '/':   font = 10;								break;
				case '+':	font = 11;								break;
				case ',':	font = 12;								break;
				case '(':	font = 13;								break;
				case ')':	font = 14;								break;
				default:    font = 0;   SetMessage("No Operator."); break;
			}

			clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]]);
		}

		startx += number_xsize;
	}
	else
	{
		itoa(number, string, 10);

		if(pPaletteManager->GetPalette(PaletteDataGreen))
		{
			for(i = 0; i < lstrlen(string); i++)
			{
				if(string[i] != ' ')
				{
					switch(string[i])
					{
						case '/':	font = 10;						break;
						case '+':	font = 11;						break;
						case ',':	font = 12;						break;
						case '(':	font = 13;						break;
						case ')':	font = 14;						break;
						default:	font = startfont+string[i]-'0';	break;
					}

					clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]], pPaletteManager->GetPalette(PaletteDataGreen));
				}

				startx += number_xsize;
			}
		}
		else
		{
			for(i = 0; i < lstrlen(string); i++)
			{
				if(string[i] != ' ')
				{
					switch(string[i])
					{
						case '/':	font = 10;						break;
						case '+':	font = 11;						break;
						case ',':	font = 12;						break;
						case '(':	font = 13;						break;
						case ')':	font = 14;						break;
						default:	font = startfont+string[i]-'0';	break;
					}

					clGrp.PutSpriteT(startx, starty, number_xsize, number_ysize, &pXspr->Image[pXspr->Header.Start[font]]);
				}

				startx += number_xsize;
			}
		}
	}

	return startx;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber )
{
	SI16	i;
	SI16	siFont;	

	if(pNumber == NULL)
	{
		siFont = 0;
		if(pPaletteManager->GetPalette(PaletteDataGreen))
		{
			clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]], pPaletteManager->GetPalette(PaletteDataGreen));
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
		}
	}
	else
	{
		if(pPaletteManager->GetPalette(PaletteDataGreen))
		{
			for(i = 0; i < (SI16)strlen(pNumber); i++)
			{
				if(pNumber[i] != ' ')
				{
					switch(pNumber[i])
					{
						case '/':	siFont = 10;				break;
						case '+':	siFont = 11;				break;
						case ',':	siFont = 12;				break;
						case '(':	siFont = 13;				break;
						case ')':	siFont = 14;				break;
						default:	siFont = pNumber[i] - '0';	break;
					}

					clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]], pPaletteManager->GetPalette(PaletteDataGreen));
				}
				startx += pXspr->Header.Xsize;
			}
		}
		else
		{
			for(i = 0; i < (SI16)strlen(pNumber); i++)
			{
				if(pNumber[i] != ' ')
				{
					switch(pNumber[i])
					{
						case '/':	siFont = 10;				break;
						case '+':	siFont = 11;				break;
						case ',':	siFont = 12;				break;
						case '(':	siFont = 13;				break;
						case ')':	siFont = 14;				break;
						default:	siFont = pNumber[i] - '0';	break;
					}

					clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
				}
				startx += pXspr->Header.Xsize;
			}
		}
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼ýÀÚ¸¦ Ãâ·ÂÇØÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::DrawNumberBlue(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber )
{
	SI16	i;
	SI16	siFont;	

	if(pNumber == NULL)
	{
		siFont = 0;
		if(pPaletteManager->GetPalette(PaletteDataWhiteBlue))
		{
			clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]], pPaletteManager->GetPalette(PaletteDataWhiteBlue));
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
		}
	}
	else
	{
		if(pPaletteManager->GetPalette(PaletteDataWhiteBlue))
		{
			for(i = 0; i < (SI16)strlen(pNumber); i++)
			{
				if(pNumber[i] != ' ')
				{
					switch(pNumber[i])
					{
						case '/':	siFont = 10;				break;
						case '+':	siFont = 11;				break;
						case ',':	siFont = 12;				break;
						case '(':	siFont = 13;				break;
						case ')':	siFont = 14;				break;
						default:	siFont = pNumber[i] - '0';	break;
					}

					clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]], pPaletteManager->GetPalette(PaletteDataWhiteBlue));
				}
				startx += pXspr->Header.Xsize;
			}
		}
		else
		{
			for(i = 0; i < (SI16)strlen(pNumber); i++)
			{
				if(pNumber[i] != ' ')
				{
					switch(pNumber[i])
					{
						case '/':	siFont = 10;				break;
						case '+':	siFont = 11;				break;
						case ',':	siFont = 12;				break;
						case '(':	siFont = 13;				break;
						case ')':	siFont = 14;				break;
						default:	siFont = pNumber[i] - '0';	break;
					}

					clGrp.PutSpriteT(startx, starty, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[pXspr->Header.Start[siFont]]);
				}
				startx += pXspr->Header.Xsize;
			}
		}
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇöÀç ¿Â¶óÀÎ °ÔÀÓÁßÀÎ°¡?
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	_clGame::IsOnlineWorld()
{	
	if(pOnlineWorld != NULL)
		return TRUE;
	else
		return FALSE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¹öÁ¯À» È­¸é¿¡ Âï¾îÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::DrawVersion(HDC hdc, SI32 siX, SI32 siY)
{
	SetTextColor(hdc, PaletteWhiteColor);
	TextOut(hdc, siX, siY, szOnlineVersion, strlen(szOnlineVersion));
}
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	°ÔÀÓ Á¾·á

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::ExitOnline()
{
	bExitOnlineGame = TRUE;
}
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇöÀç ¹è°æÀ½¾ÇÀÇ ¾ÆÀÌµð¸¦ ¸®ÅÏ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
UI16	cltOnlineWorld::GetSoundID()
{
	return BackgroundMusic;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	·ÎµùÁß¿¡ ÅØ½ºÆ®¸¦ Ãâ·ÂÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::DrawTextInLoading(CHAR *pszString)
{	
	HDC		hdc;
	SI32	siX, siY;
	SIZE	szText;		

	if(clGrp.lpDDSPrimary->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(200, 200, 200));		

		GetTextExtentPoint(hdc, pszString, strlen(pszString), &szText);
	
		siX		=	0;
		siY		=	clGrp.GetScreenYsize() - szText.cy;

		TextOut(hdc, siX, siY, pszString, strlen(pszString));
		
		clGrp.lpDDSPrimary->ReleaseDC(hdc);
	}
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÆòÈ­ ¸ðµå, ÀüÅõ ¸ðµå º¯È¯
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::SetPeace(BOOL bFlag)		// ÆòÈ­ ¸ðµå -> ÀüÅõ ¸ðµå
{	
	bPeace = bFlag;
	
	ToggleAttackMode(pMyData->GetMyCharUniqueID(), !bPeace); 

	pOnlineClient->SendAttackMode(!bPeace);
}						

VOID	cltOnlineWorld::ToggleAttackMode(UI16 uiAccount, BOOL bAttackMode)
{
	UI16	Equip[WEAR_ITEM_NUM];
	
	// ÀåÂø Á¤º¸¸¦ ¾ò¾î ¿Â´Ù.
	pIOnlineChar->GetEquip(uiAccount, Equip);	
	pIOnlineChar->SetAttackMode(uiAccount, bAttackMode);
	pIOnlineChar->SetEquipItem(uiAccount, Equip);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	·ÎµùÁß¿¡ ÇÁ·Î±×·¡½º¹Ù¸¦ Ãâ·ÂÇÑ´Ù.			hojae_append 2002.01.03
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID cltOnlineWorld::ScrollLoading( UI32 uiLoadPercent )
{
	if(clGrp.LockSurface(clGrp.lpDDSBack) == TRUE)
	{
		clGrp.PutSpriteT(0, 0, pOnlineLoadingData->BackImage.Header.Xsize, pOnlineLoadingData->BackImage.Header.Ysize, pOnlineLoadingData->BackImage.Image);
		clGrp.PutSpriteT(0, 22, pOnlineLoadingData->BackOverImage.Header.Xsize, pOnlineLoadingData->BackOverImage.Header.Ysize, pOnlineLoadingData->BackOverImage.Image);
		clGrp.PutSpriteT(553, 22, pOnlineLoadingData->AniLogoImage.Header.Xsize , pOnlineLoadingData->AniLogoImage.Header.Ysize, &pOnlineLoadingData->AniLogoImage.Image[pOnlineLoadingData->AniLogoImage.Header.Start[pOnlineLoadingData->uiFrame]]);
//		clGrp.PutSpriteT(522, 19, pOnlineLoadingData->AniLogoImage.Header.Xsize , pOnlineLoadingData->AniLogoImage.Header.Ysize, &pOnlineLoadingData->AniLogoImage.Image[pOnlineLoadingData->AniLogoImage.Header.Start[pOnlineLoadingData->uiFrame]]);

		// actdoll (2004/09/01 13:36) : È¸»ç ¹× ÀÌ¿ë°¡´É ¿¬·É¹üÀ§¿¡ ´ëÇÑ ·Î°í
		clGrp.PutSpriteT( 186, 550, pOnlineLoadingData->m_Spr_CompanyLogo.Header.Xsize, pOnlineLoadingData->m_Spr_CompanyLogo.Header.Ysize, pOnlineLoadingData->m_Spr_CompanyLogo.Image );
		if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_KOREA )	clGrp.PutSpriteT( 10, 10, pOnlineLoadingData->PlayGradeImage.Header.Xsize, pOnlineLoadingData->PlayGradeImage.Header.Ysize, pOnlineLoadingData->PlayGradeImage.Image );

		clGrp.UnlockSurface( clGrp.lpDDSBack );		
	}

	HDC hDC;
	HRESULT hr = clGrp.lpDDSBack->GetDC(&hDC);

	if( hr == DD_OK )
	{	
		SetBkMode(hDC, TRANSPARENT);
		if(pOnlineText)
		{
			char temp[256];
			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_LOGIN));
			sprintf( temp, "Loading(%d%%)", uiLoadPercent );
			for(int i=0; i<pOnlineLoadingData->uiFrame; i++)
				strcat(temp, ".");
			
			pOnlineMegaText->DrawTextCenter( hDC, 548, 304, 164, 20, temp, RGB(10, 10, 10));
			pOnlineMegaText->DrawTextCenter( hDC, 547, 303, 164, 20, temp, RGB(250, 250, 250));

			// actdoll (2004/09/02 4:23) : ¶óÀÌ¼±½º ÅØ½ºÆ®
			char	*pszLicense;
			if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_GAMANIA;
			else														pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_BOLEHCOM;
			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_ITEM));
			pOnlineMegaText->DrawText( hDC, 286, 564, pszLicense, RGB(250, 250, 250) );


			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_CHAT));
			pOnlineMegaText->DrawText(hDC, 290, 579, pOnlineText->Get(ON_TEXT_LOADING_NOTICE), RGB(250, 250, 250));
			// actdoll (2004/03/03 19:02) : µð¹ö±× ¸ðµå¿¡¼­´Â µð¹ö±× ¹öÁ¯ÀÓÀ» Ç¥±âÇÑ´Ù.
#ifdef	_DEBUG
			sprintf( temp,"Debuged Mode %d.%04d", ON_CLINETVERSION / 10000, ON_CLINETVERSION % 10000 );
			pOnlineMegaText->DrawText(hDC, 600, 579, temp , RGB(250, 250, 250));
#else//	no _DEBUG
			sprintf( temp,"Ver %d.%04d", ON_CLINETVERSION / 10000, ON_CLINETVERSION % 10000 );
			pOnlineMegaText->DrawText(hDC, 730, 579, temp , RGB(250, 250, 250));
#endif//_DEBUG
			
		}
		clGrp.lpDDSBack->ReleaseDC(hDC);
	}

	if(WindowModeSwitch)	UpdateScreen(clGrp.lpDDSBack,FALSE);
    else					UpdateScreen(clGrp.lpDDSBack);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀåºÎ¿¡¼­ ÃÖ°í ÅõÀÚ »óÇ°À» ±¸ÇØ ÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::FindTrade()
{
	bFindTrade	=	TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀåºÎ°¡ ÃÖ°í ÅõÀÚ »óÇ°À» Ã£´Â ÁßÀÎ°¡?
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	cltOnlineWorld::IsFindNow()
{
	return bFindTrade;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	´ÜÃàÅ°
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::SetShortKey( BOOL bFlag )
{
	if(bFlag)
		ShortKey	=	bFlag;
	else if(!pOnlineMessenger->IsAction() || pOnlineMessenger->IsMini())
		ShortKey	=	bFlag;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÀåºÎ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltOnlineWorld::FindTradeBook()
{
	// ÀåºÎ °Ë»ö

	pTempSearch	=	pOnlineVillageSave->SortSearch( siTempSearchNum );	

	if( pTempSearch )
	{		
		pOnlineVillageSave->SearchSave( pTempSearch, siTempSearchNum );

		pTempSearch			=	NULL;
		siTempSearchNum		=	0;
		bFindTrade			=	FALSE;			
	}

	else if( siTempSearchNum == -2 )
	{
		pTempSearch			=	NULL;
		siTempSearchNum		=	0;
		bFindTrade			=	FALSE;
	}
}

BOOL	cltOnlineWorld::GetShortKey()
{
	return ShortKey;
}

BOOL	cltOnlineWorld::SendHeartBeat(BOOL first)
{
	// Å¬¶óÀÌ¾ðÆ® ÀÚÃ¼ ½ºÇÇµåÇÙ Ã¼Å©
	static bool s_bSpeedHackUser = FALSE;
	static int frame = 0;

	if(GetTickCount() - m_dwGameGuardCheckTick > (1000 * 5))
	{
		m_dwGameGuardCheckTick = GetTickCount();		
		if(CheckGameGuard() == FALSE) SendMessage(pGame->Hwnd, WM_CLOSE, 0, 0);
	}

	if( ++frame > 150 )
	{
		frame = 0;
		
		if(!s_bSpeedHackUser && abs(timeGetTime() - GetTickCount() - m_TickTimeDiff) >= 10000)
		{
			s_bSpeedHackUser = TRUE;
			// TypeÀÌ 0ÀÌ¸é ¼­¹ö¿¡¼­ ½ºÇÇµåÇÙ Ã³¸®¸¦ ÇÔ.
			if(pOnlineClient) pOnlineClient->SendGameTickCount(0, 0);
			return TRUE;
		}
	}

	// 30ÃÊ¸¶´Ù Server¿¡°Ô TickCount ¸Þ¼¼Áö
	if(m_GameTickCount)
	{
		if(first)
		{
			if(pOnlineClient) 
			{
				m_GameTickCount = GetTickCount();
				pOnlineClient->SendGameTickCount(m_GameTickCount, 1);
			}
		}
		else
		{
			DWORD	TickDiff = GetTickCount() - m_GameTickCount;
			SI16	siResult1 = (SI16)(TickDiff / SERVERINFO_TICKCOUNT);
			if( siResult1 >= 1)
			{
				// ½ºÇÙÀ» ½èÀ» °æ¿ì siResult1 °ªÀÌ 3¸¸ÀÌ ³Ñ¾î°¡´Â °æ¿ìµµ ÀÖ´Ù. ¼­¹ö ºÎÇÏ¸¦ ÁÙÀÌ±â À§ÇØ ÃÖ´ë 10¹ø ±îÁö¸¸...
				for(int i = 0; i < min(siResult1, 10); i++)
				{
					if(pOnlineClient) pOnlineClient->SendGameTickCount(m_GameTickCount + SERVERINFO_TICKCOUNT * (i+1), 1);
				}
				m_GameTickCount += SERVERINFO_TICKCOUNT * siResult1;
			}
		}
	}
	
	// 30ÃÊ¸¶´Ù Server¿¡°Ô Time ¸Þ¼¼Áö
	if(m_GameTime)
	{
		if(first)
		{
			if(pOnlineClient) 
			{
				m_GameTime = timeGetTime();
				m_TickTimeDiff = timeGetTime() - GetTickCount();
				pOnlineClient->SendGameTickCount(m_GameTime, 2);
			}
		}
		else
		{
			DWORD	TimeDiff = timeGetTime() - m_GameTime;
			SI16	siResult2 = (SI16)(TimeDiff / SERVERINFO_TICKCOUNT);
			if( siResult2 >= 1)
			{
				for(int i = 0; i < min(siResult2, 10); i++)
				{
					if(pOnlineClient) pOnlineClient->SendGameTickCount(m_GameTime + SERVERINFO_TICKCOUNT * (i+1), 2);
				}
				m_GameTime += SERVERINFO_TICKCOUNT * siResult2;
			}
		}
	}

	return TRUE;
}

SI32	cltOnlineWorld::GetVillageStateCode(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;
	VillageHeader*		lpVillageHeader;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return lpSVillageData->m_VillageStateCode;
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	// List¿¡ ¾øÀ¸¸é ±âº»ÀûÀÎ ±¹°¡ ÄÚµå¸¦ ÀÌ¿ëÇÑ´Ù.
	lpVillageHeader = pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(VillageCode);
	if(lpVillageHeader)
	{
		return (SI16)lpVillageHeader->uiNation;
	}

	// ¿¡·¯³ª¸é ¹«Á¶°Ç ÄÚ¸®¾Æ~~~!!!
	return ON_COUNTRY_NONE;
}

SI32	cltOnlineWorld::GetVillageGuildFlag(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			if(lpSVillageData->m_VillageGuildName[0])
			{
				return lpSVillageData->m_VillageGuildFlag;
			}
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return -1;
}

float	cltOnlineWorld::GetVillageDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return (float)((double)lpSVillageData->m_vdChangedDefence / (double)lpSVillageData->m_vdChangedMaxDefence);
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0.0f;
}

BOOL	cltOnlineWorld::GetVillageEnemyGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ÀÚ½ÅÀÇ »ó´ÜÀÌ ¾øÀ¸¸é ¹«Á¶°Ç FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_ENEMY) return TRUE;
				else                                                      return FALSE;
			}
		}
		else
		{
			// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

// robypark 2004/12/22 11:50
// ¿À´Ã °ø¼ºÇÏ´Â ¸¶À»ÀÎÁö °Ë»ç
BOOL	cltOnlineWorld::GetVillageWarDayGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ÀÚ½ÅÀÇ »ó´ÜÀÌ ¾øÀ¸¸é ¹«Á¶°Ç FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				// ÀÌ ¸¶À»°ú °ø¼ºÀ» ÇÏ´Â°¡?
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_WAR)
					return TRUE;

				else 
					return FALSE;
			}
		}
		else
		{
			// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

// robypark 2004/10/28 17:29
// ¸¶À»ÀÌ Ä³¸¯ÅÍ ¼Ò¼Ó »ó´ÜÀÇ ¸¶À»ÀÎÁö °Ë»ç
// [IN] UI16 VillageCode: ¸¶À» ÄÚµå
BOOL	cltOnlineWorld::GetVillageOurGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ÀÚ½ÅÀÇ »ó´ÜÀÌ ¾øÀ¸¸é ¹«Á¶°Ç FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				// ÀÚ½ÅÀÌ ¼Ò¼ÓÇÑ »ó´Ü ¼ÒÀ¯ÀÇ ¸¶À»ÀÎÁö °Ë»ç
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_MINE)
					return TRUE;
				else 
					return FALSE;
			}
		}
		else
		{
			// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

VDEF	cltOnlineWorld::GetVillageDefenceNowDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return lpSVillageData->m_vdChangedDefence;
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

VDEF	cltOnlineWorld::GetVillageDefenceMaxDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return lpSVillageData->m_vdChangedMaxDefence;
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// ¸¶À»¿¡ ÁÖµÐÇÑ ÇöÀç ¼öºñº´ ºÎ´ë ¼ö ¾ò±â
SI32 cltOnlineWorld::GetVillageCurrentGarrisonCount(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return lpSVillageData->m_siCurrentGarrison;
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// ¸¶À»¿¡ ÁÖµÐÇÑ ÃÖ´ë ¼öºñº´ ºÎ´ë ¼ö ¾ò±â
SI32 cltOnlineWorld::GetVillageMaximumGarrisonCount(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return lpSVillageData->m_siMaximumGarrison;
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// ¸¶À»ÀÇ ¹Ý°Ý °ø°Ý ÇüÅÂ ¾ò±â. ÀÏ¹Ý, µ¹, ±â¸§
SI16 cltOnlineWorld::GetvillageCounterAttackType(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ÇöÀç ±¹°¡ ÄÚµå°¡ List¿¡ µé¾îÀÖ´ÂÁö °Ë»ç

	lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			return lpSVillageData->m_ssCounterAttackType;
		}
	}
	else
	{
		// ¾øÀ¸¸é Server¿¡ Data ¿äÃ»
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

void cltOnlineWorld::CheckNowConnectUser(SI32 MaxUser, SI32 NowUser)
{
	if(m_LastNowUserConnectMessageTickCount)
	{
		SI32		TempCheckUser;
		
		if(m_LastNowUserConnectMessageNowUser < 500)
		{
			TempCheckUser = 50;
		}
		else
		{
			TempCheckUser = ((m_LastNowUserConnectMessageNowUser * 10) / 100);
		}

		if((m_LastNowUserConnectMessageNowUser - TempCheckUser) > NowUser)
		{
			ErrorGameServer();
		}
	}

	m_LastNowUserConnectMessageNowUser   = NowUser;
	m_LastNowUserConnectMessageTickCount = GetTickCount();
}

void cltOnlineWorld::CheckNowConnectUserMessage(void)
{
	// ÀÏÁ¤½Ã°£µ¿¾È ¸Þ¼¼Áö°¡ ¿ÀÁö ¾Ê¾ÒÀ¸¸é ¿¡·¯ Ãâ·Â
	if(m_LastNowUserConnectMessageTickCount)
	{
		if((GetTickCount() - m_LastNowUserConnectMessageTickCount) > 90000)
		{
			if(m_LastNowUserConnectMessageNowUser > 50)
			{
				ErrorGameServer();
				m_LastNowUserConnectMessageTickCount = GetTickCount();
			}
		}
	}

	if(m_LastNowUserConnectMessageErrorOutPutCount)
	{
		m_LastNowUserConnectMessageErrorOutPutCount--;

		if((m_LastNowUserConnectMessageErrorOutPutCount % 10) == 0)
		{
			PushEffect(EFFECT_MANAGERWARNING, 0, 0);	
		}
	}
}

void cltOnlineWorld::ErrorGameServer(void)
{
	//m_LastNowUserConnectMessageErrorOutPutCount = 10000;
	//PushEffect(EFFECT_MANAGERWARNING, 0, 0);	
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Get War Field Map
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32 cltOnlineWorld::GetWarFieldMap(void)
{
	if(pOnlineMap)
		return pOnlineMap->GetWarFieldMap();
	else
		return FALSE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇöÀç FieldMapÀÌ PK°¡ °¡´ÉÇÑÁö °Ë»ç

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL cltOnlineWorld::CheckPKFieldMap(void)
{
	if(pOnlineMap)
		return pOnlineMap->CheckPKFieldMap();
	else
		return FALSE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶À»¾ÈÀÎÁö Flag
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL cltOnlineWorld::IsVillage(void)
{
	return siGamePollStatus == ON_GAMEPOLL_VILLAGE ? TRUE : FALSE;
}

/*MONEY	GetOnlineUserMoney()
{
	if( pOnlineWorld )
		return pOnlineWorld->pMyData->GetMoney();

	return 0;
}*/

BOOL _clGame::OnlineSendHeartBeat()
{

	if( pOnlineWorld )
	{
		pOnlineWorld->SendHeartBeat();
		pOnlineWorld->pOnlineClient->DataProc();
	}

	return FALSE;
}


// ¾ÆÀÌÅÛ ÀÌ¸§À» ¾ò¾î ¿Â´Ù.
CHAR* _clGame::GetItemName( UI16 uiItemID )
{
	if( pOnlineWorld )
	{
		CItemHeader	*pItem = pOnlineWorld->pItemsInfo->GetItem( uiItemID );
		if( pItem )
			return pOnlineWorld->pOnlineText->Get( pItem->m_siItemName );
	}

	return NULL;
}


// ¿Â¶óÀÎ »ó´Ü ¸Þ½ÃÁö°ü·Ã Å¬·¡½ºÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò¾î¿Â´Ù.
OnlineNoticeList* _clGame::GetOnlineNoticeListPointer()
{
	if( pOnlineWorld )
	{
		return pOnlineWorld->pOnlineNoticeList;
	}

	return NULL;
}

void _clGame::ChattingPacketProc(char *pPacket)
{
	if( pOnlineWorld )
	{
		HQPacket packet;

		// ÆÐÅ¶ ÀÐ±â

		packet.RecvPacket(pPacket);

		pOnlineWorld->ReceiveMessageProcChatting(&packet);
	}
}

VOID	cltOnlineWorld::PlayFoodEatEffectSound(SI16 siSound )
{
	switch(siSound)
	{
		case ON_FOOD_VEGETABLE:	PushEffect(EFFECT_FOOD_VEGETABLE,0, 0); break;
		case ON_FOOD_TEA:		PushEffect(EFFECT_FOOD_TEA,0,0);		break;
		case ON_FOOD_SPECIAL:	PushEffect(EFFECT_FOOD_SPECIAL,0,0);	break;
		case ON_FOOD_NUDDLE:	PushEffect(EFFECT_FOOD_NUDDLE,0,0);		break;
		case ON_FOOD_MEAT:		PushEffect(EFFECT_FOOD_MEAT,0,0);		break;
		case ON_FOOD_FRUITS:	PushEffect(EFFECT_FOOD_FRUITS,0,0);		break;
		case ON_FOOD_ETC:		PushEffect(EFFECT_FOOD_ETC,0,0);		break;

	}
}


VOID	cltOnlineWorld::PlayItemEffectSound( SI16 siGroupCode,SI16 siSound )
{
	switch( siSound )
	{
	case ON_ITEM_ARMOR_CLOTH:	PushEffect(EFFECT_ARMOR_CLOTH,0, 0); break;
	case ON_ITEM_ARMOR_METAL:	PushEffect(EFFECT_ARMOR_METAL,0, 0); break;
	case ON_ITEM_ARMOR_RARE:	PushEffect(EFFECT_ARMOR_RARE,0, 0);  break;
	case ON_ITEM_ARMOR_WOODEN:	PushEffect(EFFECT_ARMOR_WOODEN,0, 0);break;

	case ON_ITEM_BELT_CLOTH:	PushEffect(EFFECT_BELT_CLOTH,0, 0);  break;
	case ON_ITEM_BELT_METAL:	PushEffect(EFFECT_BELT_METAL,0, 0);  break;
	case ON_ITEM_BELT_RARE:		PushEffect(EFFECT_BELT_RARE,0, 0);   break;	

	case ON_ITEM_GLOVE_CLOTH:	PushEffect(EFFECT_GLOVE_CLOTH,0, 0);	break;
	case ON_ITEM_GLOVE_METAL:	PushEffect(EFFECT_GLOVE_METAL,0, 0);	break;
	case ON_ITEM_GLOVE_RARE:	PushEffect(EFFECT_GLOVE_RARE,0, 0);	break;
	case ON_ITEM_GLOVE_WOODEN:	PushEffect(EFFECT_GLOVE_WOODEN,0, 0);break;

	case ON_ITEM_HELMET_CLOTH:	PushEffect(EFFECT_HELMET_CLOTH,0, 0);break;
	case ON_ITEM_HELMET_METAL:	PushEffect(EFFECT_HELMET_METAL,0, 0);break;
	case ON_ITEM_HELMET_RARE:	PushEffect(EFFECT_HELMET_RARE,0, 0); break;
	case ON_ITEM_HELMET_WOODEN:	PushEffect(EFFECT_HELMET_WOODEN,0, 0);break;
	case ON_ITEM_HELMET_HAT:	PushEffect(EFFECT_HELMET_HAT,0, 0);break;

	case ON_ITEM_SHOES_CLOTH:	PushEffect(EFFECT_SHOES_CLOTH,0, 0);break;
	case ON_ITEM_SHOES_ETC:		PushEffect(EFFECT_SHOES_ETC,0, 0);break;
	case ON_ITEM_SHOES_METAL:	PushEffect(EFFECT_SHOES_METAL,0, 0);break;
	case ON_ITEM_SHOES_WOODEN:	PushEffect(EFFECT_SHOES_WOODEN,0, 0);break;
	
	case ON_ITEM_WEAPON_DART:	PushEffect(EFFECT_WEAPON_DART,0, 0);break;
	case ON_ITEM_WEAPON_GUN:	PushEffect(EFFECT_WEAPON_GUN,0, 0);break;
	case ON_ITEM_WEAPON_ARROW:	PushEffect(EFFECT_WEAPON_ARROW,0, 0);break;
	case ON_ITEM_WEAPON_RING:	PushEffect(EFFECT_WEAPON_RING,0, 0);break;
	case ON_ITEM_WEAPON_SWORD:	PushEffect(EFFECT_WEAPON_SWORD,0, 0);break;
	case ON_ITEM_WEAPON_WOODEN:	PushEffect(EFFECT_WEAPON_WOODEN,0, 0);break;
	case ON_ITEM_ETC:			PushEffect(EFFECT_ETC,0, 0);break;

		/*
	case 1:		PushEffect( EFFECT_USEITEM_FRUIT, 0, 0 );		break;
	case 2:		PushEffect( EFFECT_USEITEM_POTION, 0, 0 );		break;
	case 3:		PushEffect( EFFECT_USEITEM_SWORD, 0, 0 );		break;
	case 4:		PushEffect( EFFECT_USEITEM_HELMET, 0, 0 );		break;
	case 5:		PushEffect( EFFECT_USEITEM_MEET, 0, 0 );		break;
	case 6:		PushEffect( EFFECT_USEITEM_CLOTH, 0, 0 );		break;
	case 7:		PushEffect( EFFECT_USEITEM_ARMOUR, 0, 0 );		break;
	case 8:		PushEffect( EFFECT_USEITEM_WEAPON, 0, 0 );		break;
	default:	PushEffect( EFFECT_ITEMEQUIP0, 0, 0 );			break;
		*/
	}
}

char* cltOnlineWorld::ReadText(char* filename)
{
	FILE*    fp = NULL;

	ZeroMemory(szURLBuffer,sizeof(szURLBuffer));
	

	if( ( fp = BindJXFile::GetInstance()->GetFile(filename) ) == NULL )	return NULL;

	fgets(szURLBuffer,1024,fp);

	fclose(fp);

	return szURLBuffer;
}

VOID cltOnlineWorld::GotoField()
{
	if(pOnlineVillage)
	{
		pOnlineVillage->FreeMapStruct();
		delete pOnlineVillage;
		pOnlineVillage = NULL;
	}

	// Ä³¸¯ÅÍ ¸í·É ¹öÆÛ¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
	pOnlineCharUI->Init();

	// Ä³¸¯ÅÍ¸¦ ´ë±â »óÅÂ·Î µÐ´Ù		
	if(pMyData->GetMyCharUniqueID() != 0)
	{
		pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);		
	}
	else
	{
		// ³ªÀÇ Ä³¸¯ÅÍ°¡ ¾ø´Ù.
		pOnlineCharUI->SetAction(FALSE);				// UI Å¬·¡½º¸¦ disable ½ÃÅ²´Ù.
	}

	// ÇÊµå·Î ÀÌµ¿ÇØ¾ß ÇÑ´Ù.
	siGamePollStatus	=	ON_GAMEPOLL_FIELD;
}


VOID cltOnlineWorld::GotoVillage()
{
	
	pOnlineClient->SendGotoVillage(pMyData->GetGotoVillage());	
	pMyData->SetWaitGotoVillageResponse(TRUE);
}

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd,LPARAM lParam)
{
	char Cap[255];

	GetWindowText(hwnd, Cap, 255);

	if(Cap[0] != 0)
	{
		strcpy(pGame->pOnlineWorld->m_szProgramList[pGame->pOnlineWorld->m_uiProgramNum], Cap);
		pGame->pOnlineWorld->m_uiProgramNum++;
	}
	return TRUE;
}

SI08 cltOnlineWorld::CheckHeroZone(void)
{
	SM_ATTRIB smAttrib;
	SM_POINT smPos;
	SI16 siHeroX, siHeroY;
	SI08 siNationCode = ON_COUNTRY_NONE;


	if(!pIOnlineChar)		return siNationCode;
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

	smPos.x = siHeroX;
	smPos.y = siHeroY;

	smAttrib = m_pOnlineMapSound->GetAttrib(smPos);

	switch(smAttrib & 0xF0)
	{
		case SMA_KOREA :
			siNationCode = ON_COUNTRY_KOREA;
			break;
		case SMA_JAPAN :
			siNationCode = ON_COUNTRY_JAPAN;
			break;
		case SMA_TAIWAN	:
			siNationCode = ON_COUNTRY_TAIWAN;
			break;
		case SMA_CHINA :
			siNationCode = ON_COUNTRY_CHINA;
			break;
//		default :
	}

	return(siNationCode);
}

#if 0
	SI16 siHeroX, siHeroY;
	SI32 siFuncA=0, siFuncB=0, siFuncC=0;
	SI08 siNationCode = ON_COUNTRY_KOREA;
	if(!pIOnlineChar)		return siNationCode;
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
	SI32 siX = (SI32)siHeroX;
	SI32 siY = (SI32)siHeroY;

	if(siX >= 0 && siX <= 300)
	{
		siFuncA = siY - 1260;
		if(siFuncA >= 0)	siNationCode = ON_COUNTRY_TAIWAN;
		else				siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 455)
	{
		siFuncA = 156*siY + 43*siX - 209460;
		if(siFuncA >= 0)	siNationCode = ON_COUNTRY_TAIWAN;
		else				siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 555)
	{
		siFuncA = 10*siY - 3*siX - 10805;
		if(siFuncA >= 0)	siNationCode = ON_COUNTRY_TAIWAN;
		else				siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 573)
	{
		siFuncA = 18*siY - 95*siX + 30279;
		if(siFuncA >= 0)	siNationCode = ON_COUNTRY_TAIWAN;
		else				siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 672)
	{
		siFuncA = 99*siY - 97*siX - 77277;
		if(siFuncA >= 0)	siNationCode = ON_COUNTRY_TAIWAN;
		else				siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 900)
	{
		siFuncA = 100*siY + 113*siX - 219836;
		if(siFuncA >= 0)	siNationCode = ON_COUNTRY_TAIWAN;
		else				siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siHeroX <= 935)
	{
		siFuncA = 100*siY + 113*siX - 219836;
		siFuncB = 9*siY - 68*siX + 56196;
		siFuncC = 35*siY + 83*siX - 94160;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_TAIWAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_CHINA;
		else if(siFuncC >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 972)
	{
		siFuncA = 100*siY + 113*siX - 219836;
		siFuncB = 9*siY - 68*siX + 56196;
		siFuncC = 72*siY + 85*siX - 113531;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_TAIWAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_CHINA;
		else if(siFuncC >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 1079)
	{
		siFuncA = 262*siY - 75*siX - 215300;
		siFuncB = 72*siY + 85*siX - 113531;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_TAIWAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 1157)
	{
		siFuncA = 262*siY - 75*siX - 215300;
		siFuncB = 39*siY - 2*siX - 9659;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_TAIWAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 1234)
	{
		siFuncA = 262*siY - 75*siX - 215300;
		siFuncB = 27*siY - siX - 7132;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_TAIWAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 1319)
	{
		siFuncA = 11*siY + 26*siX - 43928;
		siFuncB = 27*siY - siX - 7132;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_JAPAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 1412)
	{
		siFuncA = 11*siY + 26*siX - 43928;
		siFuncB = 181*siY - 135*siX + 121412;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_JAPAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 1500)
	{
		siFuncA = 178*siY + 519*siX - 849596;
		siFuncB = 181*siY - 135*siX + 121412;
		if(siFuncA >= 0)		siNationCode = ON_COUNTRY_JAPAN;
		else if(siFuncB >= 0)	siNationCode = ON_COUNTRY_KOREA;
		else					siNationCode = ON_COUNTRY_CHINA;
	}
	else if(siX <= 2000)
	{
		siNationCode = ON_COUNTRY_JAPAN;
	}

	return siNationCode;
}
#endif

VOID cltOnlineWorld::SetBackGroundMusic(SI08 siTempNation, BOOL bForce)
{
	// robypark 2004/12/20 16:12
	// °ø¼ºÀü Áß¿¡ µé·ÁÁö´Â ¹è°æÀ½¾Ç Ãß°¡
	// °ø¼ºÀü ½Ã°£ÀÌ¸ç, 
	if (IsInTimeSiegeWarfare())
	{
		// °ø¼ºÀü À¯´ÖÀÌ°Å³ª, ¼ö¼º ÀÎÅÍÆäÀÌ½º »ó¿¡ ÀÖ´Ù¸é
		if (TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()) 
			|| (TRUE == pMyData->IsInVillageDefenceInterface()))
		{
			// ÇöÀç ¹è°æÀ½¾ÇÀÌ ¿¬ÁÖ ÁßÀÌ¸ç,
			if (TRUE == GetBgmMgr()->IsPlaying())
			{
				// ÇöÀç ¿¬ÁÖ ÁßÀÎ ¹è°æÀ½¾ÇÀÌ °ø¼ºÀü ¹è°æÀ½¾ÇÀÌ ¾Æ´Ï¶ó¸é
				if (OGGBGM_FILENAME_SIEGEBATTLEMUSIC != GetBgmMgr()->GetCurrentBgmIndex())
				{
					//clGrp.Error("¹è°æ À½¾Ç ¿¬ÁÖ", "¹è°æÀ½¾Ç ¿¬ÁÖ Áß ÀÔ´Ï´Ù. °ø¼ºÀü ¹è°æÀ½¾Ç ¿¬ÁÖÇÕ´Ï´Ù.");
					// °ø¼º ¹è°æ À½¾Ç ¿¬ÁÖ
					GetBgmMgr()->Play(OGGBGM_FILENAME_SIEGEBATTLEMUSIC, TRUE);
				}
			}
			else	// ¿¬ÁÖ ÁßÀÌ ¾Æ´Ï¶ó¸é, 
			{
				//clGrp.Error("¹è°æ À½¾Ç ¿¬ÁÖ", "°ø¼ºÀü ¹è°æÀ½¾Ç ¿¬ÁÖÇÕ´Ï´Ù.");

				// °ø¼º ¹è°æ À½¾Ç ¿¬ÁÖ
				GetBgmMgr()->Play(OGGBGM_FILENAME_SIEGEBATTLEMUSIC, TRUE);
			}

			// °ø¼ºÀü ¹è°æÀ½¾Ç ¿¬ÁÖ Áß¿¡´Â ±¹°¡º° ¹è°æÀ½¾ÇÀ» ¿¬ÁÖÇÏÁö ¾Ê´Â´Ù.
			return;
		}
	}

	if(siTempNation==ON_COUNTRY_NONE) return;

	// °ø¼ºÀü ¹è°æÀ½¾Ç ¿¬ÁÖ°¡ ÇÊ¿ä¾ø´Â °æ¿ì ¿ø·¡ °Å»ó ¹è°æÀ½¾Ç ¿¬ÁÖ¸¦ ÇÏµµ·Ï ÇÑ´Ù.
	// ÇöÀç ¹è°æÀ½¾ÇÀÌ ¿¬ÁÖ ÁßÀÌ¸ç,
	if (TRUE == GetBgmMgr()->IsPlaying())
	{
		// ÇöÀç ¿¬ÁÖ ÁßÀÎ ¹è°æÀ½¾ÇÀÌ °ø¼ºÀü ¹è°æÀ½¾ÇÀÌ¶ó¸é
		if (OGGBGM_FILENAME_SIEGEBATTLEMUSIC == GetBgmMgr()->GetCurrentBgmIndex())
		{
			// °­Á¦·Î ¹è°æÀ½¾ÇÀ» ¹Ù²Ùµµ·Ï ÇÑ´Ù.
			bForce = true;
		}
	}

	if(m_siNationCode != siTempNation || bForce)
	{
		if((bForce == false ) && ( GetTickCount() - m_iMinPlaySoundTick < 10000 )) return;

		m_siNationCode = siTempNation;
		
#ifdef _OGG_PLAY
		switch(m_siNationCode)
		{
			case ON_COUNTRY_KOREA:	
				GetBgmMgr()->Play(OGGBGM_FILENAME_KOREAFIELD, TRUE);
				GetBgmMgr()->SetNextBgm(OGGBGM_FILENAME_KOREAFIELD);
				break;
			case ON_COUNTRY_JAPAN:	
				GetBgmMgr()->Play(OGGBGM_FILENAME_JAPANFIELD, TRUE);
				GetBgmMgr()->SetNextBgm(OGGBGM_FILENAME_JAPANFIELD);
				break;
			case ON_COUNTRY_CHINA:	
				GetBgmMgr()->Play(OGGBGM_FILENAME_CHINAFIELD, TRUE);
				GetBgmMgr()->SetNextBgm(OGGBGM_FILENAME_CHINAFIELD);
				break;
			case ON_COUNTRY_TAIWAN:	
				GetBgmMgr()->Play(OGGBGM_FILENAME_TAIWANFIELD, TRUE);
				GetBgmMgr()->SetNextBgm(OGGBGM_FILENAME_TAIWANFIELD);
				break;
			default:
				GetBgmMgr()->Play(OGGBGM_FILENAME_NUNOCHON_KYUNGMAE, TRUE);
				GetBgmMgr()->SetNextBgm(OGGBGM_FILENAME_NUNOCHON_KYUNGMAE);
				break;

		}
#else
		if(g_pSoundManager)
		{
			g_pSoundManager->StopAllSound();
			g_pSoundManager->DelPlayID(BackgroundMusic);
			switch(m_siNationCode)
			{
			case ON_COUNTRY_KOREA:	BackgroundMusic	= g_pSoundManager->AddPlayID("music\\KoreaField.YAV", FALSE, TRUE, FALSE);	break;
			case ON_COUNTRY_JAPAN:	BackgroundMusic	= g_pSoundManager->AddPlayID("music\\JapanField.YAV", FALSE, TRUE, FALSE);	break;
			case ON_COUNTRY_CHINA:	BackgroundMusic	= g_pSoundManager->AddPlayID("music\\ChinaField.YAV", FALSE, TRUE, FALSE);	break;
			case ON_COUNTRY_TAIWAN:	BackgroundMusic	= g_pSoundManager->AddPlayID("music\\Music1.YAV", FALSE, TRUE, FALSE);		break;
			default:				BackgroundMusic	= g_pSoundManager->AddPlayID("music\\Music1.YAV", FALSE, TRUE, FALSE);		break;
			}
			g_pSoundManager->PlaySound( BackgroundMusic );
		}
#endif
	}
	else
	{
		m_iMinPlaySoundTick = GetTickCount();
	}
}

BOOL cltOnlineWorld::IsRunningProcess(char *szPorocessName)
{
	// ½ÇÇàÁßÀÎ ÇÁ·Î±×·¥ ¸ñ·ÏÀ» ±¸ÇÑ´Ù.
	HANDLE hSnap;
	PROCESSENTRY32 pe;
	
	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnap == (HANDLE)-1)		return FALSE;
	
	pe.dwSize=sizeof(PROCESSENTRY32);
	if (Process32First(hSnap,&pe)) 
	{
		do 
		{
			if(strcmp(szPorocessName, pe.szExeFile)==0)
			{
				CloseHandle(hSnap);
				return TRUE;
			}
		} 
		while (Process32Next(hSnap,&pe));
	}
	CloseHandle(hSnap);
	return FALSE;
}

VOID _clGame::MonitorMacro()
{
	SI08 siResult = 0;

	if(abs(CurrentClock - MonitorMacroClock1) > 30000)
	{
		MonitorMacroClock1 = CurrentClock;
//		siResult = GetDLGitemTextA();
	}

/*	if(siResult)
	{
		pOnlineWorld->pOnlineMsgBox->SetAction(FALSE);
		pOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "¸¶ÀÛ ¸ÅÅ©·Î °ËÃâ", "siResult : %d", siResult);
	}*/
}

BOOL	cltOnlineWorld::IsNight()
{
	SOnlineTime				NowTime;
	
	// ÇöÀç ½Ã°£À» ¾ò¾î¿Â´Ù.
	NowTime    =		pTimer->GetNowTime();

	if( ( NowTime.Hour >= 6 ) && ( NowTime.Hour < 20 ) )		return	FALSE;
	else														return	TRUE;

}

BOOL	cltOnlineWorld::WeaponWearCheck(UI16 uiCharID, SI16 siWeaponType)
{
	switch(siWeaponType)
	{	
		case ON_ITEM_MISSLE_TYPE_FAN:
		{
			if(uiCharID == 9027)  return TRUE;
			else				  return FALSE;
		}
		break;
		case ON_ITEM_MISSLE_TYPE_DOUBLESTICK:
		{
			if(uiCharID == 323)   return TRUE;
			else				  return FALSE;	
		}
		break;
		case ON_ITEM_MISSLE_TYPE_SPINSTONE:
		{
			if( uiCharID == 67 ) return TRUE;
			else				 return FALSE;
		}
		break;
		case ON_ITEM_MISSLE_TYPE_SCYTHE:
		{
			if( uiCharID == 8771 )	return TRUE;
			else					return FALSE;
		}
		break;
		case ON_ITEM_MISSLE_TYPE_BEADS:
		case ON_ITEM_MISSLE_TYPE_BUDDIST:
		case ON_ITEM_MISSLE_TYPE_FIREGUN:
		case ON_ITEM_MISSLE_TYPE_STICK:
		case ON_ITEM_MISSLE_TYPE_DSWORD:
		case ON_ITEM_MISSLE_TYPE_PEARL:
			return FALSE;
		break;
	}	
	return TRUE;
}

// robypark 2004/10/27 13:59
// °ø¼ºÀü À¯´ÖÀÌ Àû »ó´Ü °ø¼ºÀü À¯´Ö¿¡ ´ëÇÑ °ø°ÝÀÌ °¡´ÉÇÑÁö °Ë»ç ÇÔ¼ö
// UI16 uiAttackerUniqueID: °ø°ÝÀÚÀÇ UniqueID
// UI16 uiTargetUniqueID: ÇÇ°ÝÀÚÀÇ UniqueID
BOOL cltOnlineWorld::SiegeWarfare_CanAttackUnit(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID)
{
	// Á¸ÀçÇÏ´Â ¸ñÇ¥ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == pIOnlineChar->IsExistUniqueID(uiTargetUniqueID))
	{
		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// °ø¼º ½Ã°£ÀÎÁö ºñ±³
	if(FALSE == IsInTimeSiegeWarfare())
	{
		char szErrorMsg[256];

		sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_SIEGEWARFARE_TIME_IS), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END); 
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, szErrorMsg);

		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

		return FALSE;
	}

	// º» ¸ÊÀÌ ¾Æ´Ñ °æ¿ì ºÒ°¡´É
	if (pOnlineMap->m_MapIndex != 0)
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));

		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}
/*
	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ »ì¾ÆÀÖ´Â°¡?
	if (0 >= pIOnlineChar->GetSiegeWarfare_Hp(uiAttackerUniqueID))
	{
		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}

	// ÇÇ°ÝÀÚ Ä³¸¯ÅÍ°¡ »ì¾ÆÀÖ´Â°¡?
	if (0 >= pIOnlineChar->GetSiegeWarfare_Hp(uiTargetUniqueID))
	{
		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â°¡?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
	{
		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}
*/
	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ °ø¼ºÀü À¯´Ö °ø°ÝÀÌ °¡´ÉÇÑ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "°ø¼ºÀü À¯´ÖÀ» °ø°ÝÇÏÁö ¸øÇÏ´Â À¯´ÖÀÌ´Ù.");

		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	/*
	// ÇÇ°ÝÀÚ Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â°¡?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
	{
		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}
	*/

	// ÇÇ°ÝÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¶ó¸ç °ø°Ý ºÒ°¡´É
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "°ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï´Ù.");

		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// Àû´ë »ó´ÜÀÎÁö °Ë»ç
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// °ø°ÝÀÚ À§Ä¡
	SI16 siPosition_AttakerX, siPosition_AttakerY;
	// ÇÇ°ÝÀÚ À§Ä¡
	SI16 siPosition_TargetX, siPosition_TargetY;

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¿ø°Å¸® À¯´ÖÀÏ °æ¿ì, Áï ±Ù°Å¸® °ø°ÝÀÌ ºÒ°¡´ÉÇÑ °æ¿ì °Ë»ç
	// ¿ø°Å¸® À¯´Ö ÆÇ´ÜÀº Ä³¸¯ÅÍ ¼Ó¼º¿¡ ATB_MINRANGE°¡ ÀÖÀ» °æ¿ì ¿ø°Å¸®·Î ÆÇ´ÜÇÑ´Ù.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// °ø°ÝÀÚ ¹× ÇÇ°ÝÀÚ Ä³¸¯ÅÍ À§Ä¡ ¾ò±â
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

		// ÇÇ°ÝÀÚ Ä³¸¯ÅÍ¿Í °Å¸®°¡ ÃÖ¼Ò°Å¸® ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
		if (ManContactvillage(siPosition_TargetX, siPosition_TargetY, 1, 1, siPosition_AttakerX, siPosition_AttakerY, KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
//			pIOnlineChar->SetSpeech(uiAttackerUniqueID, "Àû°úÀÇ °Å¸®°¡ °¡±î¿ö¼­ °ø°ÝÀÌ ºÒ°¡´ÉÇÏ´Ù.");

			// °ø°Ý ¸ðµå ¼³Á¤
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
	}
	else
	{
		/*
		// ±Ù°Å¸® ÀÚµ¿ °ø°Ý È½¼ö Á¦ÇÑ
		if (m_sSiegeWarfare_AttackMode.siAutoAttackTimes >= 3)
		{
			// °ø°Ý ¸ðµå ¼³Á¤
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
		*/
	}


	/*
	// º¸±Þ °ÔÀÌÁö°¡ 0ÀÏ °æ¿ì °ø°Ý ºÒ°¡´É
	if (0 >= pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiAttackerUniqueID))
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_SPEECH_CANNOT_ATTACK_RANOUT_SUPPLYGOODS));

		// °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;

		return FALSE;
	}
	*/

	// °ø°Ý Áö¿¬½Ã°£ÀÌ °æ°úµÇ¾ú´ÂÁö °Ë»ç
	if (pIOnlineChar->GetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "°ø°Ý Áö¿¬½Ã°£ÀÌ °æ°úÇÏÁö ¾Ê¾Ò´Ù.");

		return FALSE;
	}

	// °ø°Ý Áö¿¬½Ã°£ÀÌ °æ°úÇÏ¿´À¸¹Ç·Î, °ø°ÝÇÏ´Â ÇöÀç ½Ã°£À» ¼³Á¤
	pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID, KI[uiKindInfoIdx_Attacker].GetAttackDelay(0));

	// °ø°Ý °¡´É
	return TRUE;
}

// robypark 2005/1/8 13:16
// °ø¼ºÀü À¯´ÖÀÌ °ø°ÝÀÌ °¡´ÉÇÑÁö °Ë»ç ÇÔ¼ö
// UI16 uiTargetUniqueID: ÇÇ°ÝÀÚÀÇ UniqueID
BOOL	cltOnlineWorld::SiegeWarfare_CheckCanAttackUnit(UI16 uiTargetUniqueID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ °ø¼ºÀü À¯´Ö °ø°ÝÀÌ °¡´ÉÇÑ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
		return FALSE;
	}
/*
	// ÇÇ°ÝÀÚ Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â°¡?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
		return FALSE;
*/

	// ÇÇ°ÝÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¶ó¸ç °ø°Ý ºÒ°¡´É
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
		return FALSE;
	}

	// Àû´ë »ó´ÜÀÎÁö °Ë»ç
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		return FALSE;
	}

	// °ø°Ý °¡´É
	return TRUE;
}

// robypark 2005/1/12 11:3
// °ø¼ºÀü À¯´ÖÀÌ °ø°ÝÀÌ °¡´ÉÇÑÁö °Ë»çÇÏ¿© ¸¶¿ì½º Ä¿¼­ ¼³Á¤À» À§ÇÑ ¸¶¿ì½º Å¸ÀÔ ¾ò±â
// UI16 uiTargetUniqueID: ÇÇ°ÝÀÚÀÇ UniqueID
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanAttackUnit(UI16 uiTargetUniqueID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

/*
	// ÇÇ°ÝÀÚ Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â°¡?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;
*/
	// »ç¿ëÀÚ Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â°¡?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	BOOL bRangeAttacker = FALSE;	// ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¿ø°Å¸® À¯´ÖÀÏ °æ¿ì, Áï ±Ù°Å¸® °ø°ÝÀÌ ºÒ°¡´ÉÇÑ °æ¿ì °Ë»ç
	// ¿ø°Å¸® À¯´Ö ÆÇ´ÜÀº Ä³¸¯ÅÍ ¼Ó¼º¿¡ ATB_MINRANGE°¡ ÀÖÀ» °æ¿ì ¿ø°Å¸®·Î ÆÇ´ÜÇÑ´Ù.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		bRangeAttacker = TRUE;	// ¿ø°Å¸® °ø°Ý À¯´Ö
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ °ø¼ºÀü À¯´Ö °ø°ÝÀÌ °¡´ÉÇÑ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;
	}

	/*
	// ÇÇ°ÝÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¶ó¸ç °ø°Ý ºÒ°¡´É
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;

		// °ø°ÝÀÚ°¡ ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ¿ø°Å¸® °ø°Ý ºÒ°¡´É
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// ±Ù°Å¸® °ø°Ý ºÒ°¡´É
	}
	*/

	// Àû´ë »ó´ÜÀÎÁö °Ë»ç
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;
		// °ø°ÝÀÚ°¡ ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ¿ø°Å¸® °ø°Ý ºÒ°¡´É
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// ±Ù°Å¸® °ø°Ý ºÒ°¡´É
	}

	// °ø°ÝÀÚ°¡ ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
	if (bRangeAttacker)
	{
		// °ø°ÝÀÚ À§Ä¡
		SI16 siPosition_AttakerX, siPosition_AttakerY;
		// ÇÇ°ÝÀÚ À§Ä¡
		SI16 siPosition_TargetX, siPosition_TargetY;

		// °ø°ÝÀÚ ¹× ÇÇ°ÝÀÚ Ä³¸¯ÅÍ À§Ä¡ ¾ò±â
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

		// ÇÇ°ÝÀÚ Ä³¸¯ÅÍ¿Í °Å¸®°¡ ÃÖ¼Ò°Å¸® ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
		if (ManContactvillage(siPosition_TargetX, siPosition_TargetY, 1, 1, siPosition_AttakerX, siPosition_AttakerY, KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ¿ø°Å¸® °ø°Ý ºÒ°¡´É
		}

		return ON_MOUSE_BATTLE_SUB_LONGRANGE;	// ¿ø°Å¸® °ø°Ý °¡´É
	}

	return ON_MOUSE_BATTLE_SUB_SHORTRANGE;	// ±Ù°Å¸® °ø°Ý °¡´É
}

// robypark 2004/10/28 16:21
// °ø¼ºÀü À¯´ÖÀÌ °ø°ÝÀÌ ¸¶À»À» °ø°Ý °¡´ÉÇÑÁö °Ë»ç
// [IN]  UI16 uiAttackerUniqueID: °ø°ÝÀÚÀÇ UniqueID
// [IN]  UI32 siObjectID		: °ø°Ý ´ë»ó ¿ÀºêÁ§Æ®(¸¶À») ID
// [OUT] BOOL &bRangeAttacker	: °ø°ÝÀÚ°¡ ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
BOOL cltOnlineWorld::SiegeWarfare_CanAttackVillage(UI16 uiAttackerUniqueID, SI32 siObjectID, BOOL &bRangeAttacker)
{
	// º» ¸ÊÀÌ ¾Æ´Ñ °æ¿ì ºÒ°¡´É
	if (pOnlineMap->m_MapIndex != 0)
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));
		// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// °ø°ÝÀÚ À§Ä¡
	SI16 siPosition_AttakerX, siPosition_AttakerY;

	// ¸¶À» À§Ä¡, Å©±â
	SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

	// °ø°ÝÀÚ´Â ±Ù°Å¸® °ø°Ý À¯´ÖÀÌ´Ù.
	bRangeAttacker = FALSE;

	// °ø°ÝÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¶ó¸ç °ø°Ý ºÒ°¡´É
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "°ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï´Ù.");
		// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¸¶À» °ø°ÝÀÌ °¡´ÉÇÑ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "¸¶À» °ø°ÝÀÌ ºÒ°¡´ÉÇÑ °ø¼ºÀü À¯´ÖÀÌ´Ù.");
		// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¿ø°Å¸® À¯´ÖÀÏ °æ¿ì, Áï ±Ù°Å¸® °ø°ÝÀÌ ºÒ°¡´ÉÇÑ °æ¿ì °Ë»ç
	// ¿ø°Å¸® À¯´Ö ÆÇ´ÜÀº Ä³¸¯ÅÍ ¼Ó¼º¿¡ ATB_MINRANGE°¡ ÀÖÀ» °æ¿ì ¿ø°Å¸®·Î ÆÇ´ÜÇÑ´Ù.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// °ø°ÝÀÚ´Â ¿ø°Å¸® °ø°Ý À¯´ÖÀÌ´Ù.
		bRangeAttacker = TRUE;

		// °ø°ÝÀÚ ¹× ÇÇ°ÝÀÚ Ä³¸¯ÅÍ À§Ä¡ ¾ò±â
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		
		// ¿ÀºêÁ§Æ®ÀÇ º» À§Ä¡¸¦ ±¸ÇÑ´Ù.
		pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

		// ¿ÀºêÁ§Æ®ÀÇ »çÀÌÁî¸¦ ±¸ÇÑ´Ù.
		pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

		// ¸¶À»°ú °Å¸®°¡ ÃÖ¼Ò°Å¸® ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
		if(ManContactvillage(siVillageX - siVillageXsize / 2, siVillageY - siVillageYsize / 2, // position village
								siVillageXsize, siVillageYsize, // range
								siPosition_AttakerX, siPosition_AttakerY,	// position attacker
								KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
//			pIOnlineChar->SetSpeech(uiAttackerUniqueID, "¸¶À» °ø°ÝÀ» ÇÏ±â¿¡´Â °Å¸®°¡ ³Ê¹« °¡±õ´Ù.");
			// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}

	}
	else
	{
		/*
		// ±ÙÁ¢ °ø°Ý À¯´Ö ÀÚµ¿ °ø°Ý È½¼ö Á¦ÇÑ
		if (m_sSiegeWarfare_AttackMode.siAutoAttackTimes >= 3)
		{
			// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
		*/
	}

	// °ø¼ºÇÏ´Â ³¯ÀÇ ¸¶À»ÀÎ°¡?
	if (FALSE == GetVillageWarDayGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))
	{
		// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	/*
	// º¸±Þ °ÔÀÌÁö°¡ 0ÀÏ °æ¿ì °ø°Ý ºÒ°¡´É
	if (0 >= pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiAttackerUniqueID))
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_SPEECH_CANNOT_ATTACK_RANOUT_SUPPLYGOODS));
		// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		return FALSE;
	}
	*/

	// °ø°Ý Áö¿¬½Ã°£ÀÌ °æ°úµÇ¾ú´ÂÁö °Ë»ç
	if (pIOnlineChar->GetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "°ø°Ý Áö¿¬½Ã°£ÀÌ °æ°úÇÏÁö ¾Ê¾Ò´Ù.");
		return FALSE;
	}

	// °ø°Ý Áö¿¬½Ã°£ÀÌ °æ°úÇÏ¿´À¸¹Ç·Î, °ø°ÝÇÏ´Â ÇöÀç ½Ã°£À» ¼³Á¤
	pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID, KI[uiKindInfoIdx_Attacker].GetAttackDelay(0));

	// °ø°Ý °¡´É
	return TRUE;
}

// robypark 2005/1/8 13:16
// °ø¼ºÀü À¯´ÖÀÌ °ø°ÝÀÌ ¸¶À»À» °ø°Ý °¡´ÉÇÑÁö °Ë»ç
BOOL	cltOnlineWorld::SiegeWarfare_CheckCanAttackVillage(void)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// °ø°ÝÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¶ó¸ç °ø°Ý ºÒ°¡´É
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
		return FALSE;
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¸¶À» °ø°ÝÀÌ °¡´ÉÇÑ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
		return FALSE;
	}

	// °ø°Ý °¡´É
	return TRUE;
}

// robypark 2005/1/12 11:3
// °ø¼ºÀü À¯´ÖÀÌ °ø°ÝÀÌ ¸¶À»À» °ø°Ý °¡´ÉÇÑÁö °Ë»çÇÏ¿© ¸¶¿ì½º Ä¿¼­ ¼³Á¤À» À§ÇÑ ¸¶¿ì½º Å¸ÀÔ ¾ò±â
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanAttackVillage(SI32 siObjectID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// °ø°ÝÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¶ó¸ç °ø°Ý ºÒ°¡´É
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;	// °ø°Ý ºÒ°¡´É
	}

	// »ç¿ëÀÚ Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â°¡?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;	// °ø°Ý ºÒ°¡´É
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// °ø°ÝÀÚ´Â ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
	BOOL bRangeAttacker = FALSE;

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¿ø°Å¸® À¯´ÖÀÏ °æ¿ì, Áï ±Ù°Å¸® °ø°ÝÀÌ ºÒ°¡´ÉÇÑ °æ¿ì °Ë»ç
	// ¿ø°Å¸® À¯´Ö ÆÇ´ÜÀº Ä³¸¯ÅÍ ¼Ó¼º¿¡ ATB_MINRANGE°¡ ÀÖÀ» °æ¿ì ¿ø°Å¸®·Î ÆÇ´ÜÇÑ´Ù.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// °ø°ÝÀÚ´Â ¿ø°Å¸® °ø°Ý À¯´ÖÀÌ´Ù.
		bRangeAttacker = TRUE;
	}

	// °ø°ÝÀÚ Ä³¸¯ÅÍ°¡ ¸¶À» °ø°ÝÀÌ °¡´ÉÇÑ Ä³¸¯ÅÍÀÎ°¡?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
		// °ø°ÝÀÚ°¡ ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ¿ø°Å¸® °ø°Ý ºÒ°¡´É
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// ±Ù°Å¸® °ø°Ý ºÒ°¡´É
	}

	// °ø°ÝÀÚ°¡ ¿ø°Å¸® °ø°Ý À¯´ÖÀÎ°¡?
	if (bRangeAttacker)
	{
		// °ø°ÝÀÚ À§Ä¡
		SI16 siPosition_AttakerX, siPosition_AttakerY;

		// ¸¶À» À§Ä¡, Å©±â
		SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

		// °ø°ÝÀÚ ¹× ÇÇ°ÝÀÚ Ä³¸¯ÅÍ À§Ä¡ ¾ò±â
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		
		// ¿ÀºêÁ§Æ®ÀÇ º» À§Ä¡¸¦ ±¸ÇÑ´Ù.
		pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

		// ¿ÀºêÁ§Æ®ÀÇ »çÀÌÁî¸¦ ±¸ÇÑ´Ù.
		pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

		// ¸¶À»°ú °Å¸®°¡ ÃÖ¼Ò°Å¸® ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
		if(ManContactvillage(siVillageX - siVillageXsize / 2, siVillageY - siVillageYsize / 2, // position village
								siVillageXsize, siVillageYsize, // range
								siPosition_AttakerX, siPosition_AttakerY,	// position attacker
								KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ¿ø°Å¸® °ø°Ý ºÒ°¡´É
		}

		return ON_MOUSE_BATTLE_SUB_LONGRANGE;	// ¿ø°Å¸® °ø°Ý °¡´É
	}

	return ON_MOUSE_BATTLE_SUB_SHORTRANGE;	// ±Ù°Å¸® °ø°Ý °¡´É
}

// robypark 2004/11/1 16:20
// ÀÚ½ÅÀÇ ÁÖº¯¿¡ ´ëÀå À¯´ÖÀÌ ÀÖ´ÂÁö °Ë»ç
// return value : °ø¼ºÀü À¯´ÖÀÇ °ø°Ý Å¸ÀÔ.
//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷)¿¡ ¿µÇâÀ» ¹Þ´Â´Ù.
//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - ´ëÀå °ø°ÝÀÌ´Ù.
//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - ÀÏ¹Ý ÇüÅÂÀÇ °ø°ÝÀ» ÇÑ´Ù.
//				-1									   - À¯È¿ÇÏÁö ¾ÊÀº °ø°ÝÀÌ´Ù.
SI16 cltOnlineWorld::SiegeWarfare_IsInAreaGeneralEffect(void)
{
	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ Unique ID ¾ò±â
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ñ °æ¿ì
	SI32 siKindImjinIdx_Attacker = pIOnlineChar->GetImjinKind(uiMyCharacterUniqueID);
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiMyCharacterUniqueID))
		return -1;

	// °ø¼ºÀü º¸±Þ ¼ö·¹ÀÏ °æ¿ì
	if (KI[siKindImjinIdx_Attacker].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		return -1;

	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ ÁÂÇ¥
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// ´ëÀåÀº ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷) Àû¿ë¾ÈµÊ
	if(pIOnlineChar->GetCaptain(uiMyCharacterUniqueID))
	{
		// °ø°Ý Å¸ÀÔ ¼³Á¤
		// ´ëÀåÀÇ °ø°ÝÀÌ´Ù.
		return ON_ATTACK_TYPE_CAPTAIN;
	}

	// ÀÚ½ÅÀÇ À§Ä¡ Ã£±â
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// °Ë»ö ¹üÀ§
	SI16 ssTileX, ssTileY;

	// 5 X 5 Å¸ÀÏÀÌ³»¿¡ ´ëÀåÀÌ ÀÖ´ÂÁö Ã£±â
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY++)
		{
			// °Ë»öÇÒ Å¸ÀÏ °è»ê
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// ¸Ê¿¡ ¼³Á¤µÈ Ä³¸¯ÅÍ ID Ã£±â
			UI16 uiCharacterID = pOnlineMap->GetCharID(siTargetX, siTargetY);

			// ÇØ´ç À§Ä¡¿¡ Ä³¸¯ÅÍ°¡ Á¸Àç
			if (0 != uiCharacterID)
			{
				// Ã£Àº Ä³¸¯ÅÍÀÇ Unique ID ¾ò±â
				UI16 uiCharacterUniqueID = pIOnlineChar->GetUniqueID(uiCharacterID);

				// °ø¼ºÀü À¯´ÖÀÎÁö °Ë»ç
				if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiCharacterUniqueID)) && (uiMyCharacterUniqueID != uiCharacterUniqueID))
				{
					// ´ë¹æÀÌ°Å³ª Çà¼ö Ä³¸¯ÅÍÀÎÁö °Ë»ç. Áï, ´ëÀå À¯´ÖÀÎÁö °Ë»ç, ¼ö¼º ÀÎÅÍÆäÀÌ½º »ó¿¡ ÀÖÁö ¾Ê´Ù.
					if ((pIOnlineChar->GetCaptain(uiCharacterUniqueID)) && (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiCharacterUniqueID)))
					{
						// °°Àº »ó´ÜÀÎÁö °Ë»ç
						if (IsMemberOfSameGuild(uiCharacterUniqueID))
						{
							// °ø°Ý Å¸ÀÔ ¼³Á¤
							// ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷) ¹üÀ§ ¾È¿¡ Á¸ÀçÇÑ´Ù.	
							return ON_ATTACK_TYPE_NEARCAPTAIN;
						}
					}
				}
			}	// if (0 != uiCharacterID)
		}	// for (ssTileY = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY++)
	}	// for (ssTileX = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX++)

	// ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷) Àû¿ë ¹ÞÁö ¾Ê´Â´Ù.
	return ON_ATTACK_TYPE_NORMAL;
}

// robypark 2004/11/8 16:23
// Æ¯Á¤ Ä³¸¯ÅÍÀÇ ÁÖº¯¿¡ ´ëÀå À¯´ÖÀÌ ÀÖ´ÂÁö °Ë»ç
// [IN] UI16 uiUniqueID: ºñ±³ÇÏ·Á´Â Ä³¸¯ÅÍ Unique ID
// return value : °ø¼ºÀü À¯´ÖÀÇ °ø°Ý Å¸ÀÔ.
//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷)¿¡ ¿µÇâÀ» ¹Þ´Â´Ù.
//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - ´ëÀå °ø°ÝÀÌ´Ù.
//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - ÀÏ¹Ý ÇüÅÂÀÇ °ø°ÝÀ» ÇÑ´Ù.
//				-1									   - À¯È¿ÇÏÁö ¾ÊÀº °ø°ÝÀÌ´Ù.
SI16 cltOnlineWorld::SiegeWarfare_IsInAreaGeneralEffect(UI16 uiUniqueID)
{
	// °ø¼º ½Ã°£ÀÎÁö ºñ±³
	if(FALSE == IsInTimeSiegeWarfare())
	{
		return -1;
	}

	// º» ¸ÊÀÌ ¾Æ´Ñ °æ¿ì ºÒ°¡´É
	if (pOnlineMap->m_MapIndex != 0)
		return -1;

	// Ä³¸¯ÅÍ°¡ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ñ °æ¿ì
	SI32 siKindImjinIdx_Attacker = pIOnlineChar->GetImjinKind(uiUniqueID);
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID))
		return -1;

	// °ø¼ºÀü º¸±Þ ¼ö·¹ÀÏ °æ¿ì
	if (KI[siKindImjinIdx_Attacker].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		return -1;

	// Ä³¸¯ÅÍ ÁÂÇ¥
	SI16 siCharacterPositionX, siCharacterPositionY;

	// ´ëÀåÀº ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷) Àû¿ë¾ÈµÊ
	if(pIOnlineChar->GetCaptain(uiUniqueID))
	{
		// °ø°Ý Å¸ÀÔ ¼³Á¤
		// ´ëÀåÀÇ °ø°ÝÀÌ´Ù.
		return ON_ATTACK_TYPE_CAPTAIN;
	}

	// ÀÚ½ÅÀÇ À§Ä¡ Ã£±â
	pIOnlineChar->GetPos(uiUniqueID, &siCharacterPositionX, &siCharacterPositionY);

	// °Ë»ö ¹üÀ§
	SI16 ssTileX, ssTileY;

	// 5 X 5 Å¸ÀÏÀÌ³»¿¡ ´ëÀåÀÌ ÀÖ´ÂÁö Ã£±â
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY++)
		{
			// °Ë»öÇÒ Å¸ÀÏ °è»ê
			SI16 siTargetX = siCharacterPositionX + ssTileX;
			SI16 siTargetY = siCharacterPositionY + ssTileY;

			// ¸Ê¿¡ ¼³Á¤µÈ Ä³¸¯ÅÍ ID Ã£±â
			UI16 uiCharacterID = pOnlineMap->GetCharID(siTargetX, siTargetY);

			// ÇØ´ç À§Ä¡¿¡ Ä³¸¯ÅÍ°¡ Á¸Àç
			if (0 != uiCharacterID)
			{
				// Ã£Àº Ä³¸¯ÅÍÀÇ Unique ID ¾ò±â
				UI16 uiCharacterUniqueID = pIOnlineChar->GetUniqueID(uiCharacterID);

				// °ø¼ºÀü À¯´ÖÀÎÁö °Ë»ç
				if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiCharacterUniqueID)) && (uiUniqueID != uiCharacterUniqueID))
				{
					// ´ë¹æÀÌ°Å³ª Çà¼ö Ä³¸¯ÅÍÀÎÁö °Ë»ç. Áï, ´ëÀå À¯´ÖÀÎÁö °Ë»ç, ¼ö¼º ÀÎÅÍÆäÀÌ½º »ó¿¡ ÀÖÁö ¾Ê´Ù.
					if ((pIOnlineChar->GetCaptain(uiCharacterUniqueID)) && (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiCharacterUniqueID)))
					{
						// °°Àº »ó´ÜÀÎÁö °Ë»ç
						if (IsMemberOfSameGuild(uiUniqueID, uiCharacterUniqueID))
						{
							// °ø°Ý Å¸ÀÔ ¼³Á¤
							// ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷) ¹üÀ§ ¾È¿¡ Á¸ÀçÇÑ´Ù.	
							return ON_ATTACK_TYPE_NEARCAPTAIN;
						}
					}	// if ((pIOnlineChar->GetGuildInGrade(uiCharacterUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) || (pIOnlineChar->GetGuildInGrade(uiCharacterUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)))
				}	// if (TRUE == KI[TransKindOnlineToImjin(pIOnlineChar->GetKind(uiCharacterUniqueID))].IsKindAtb(KINDATB_SIEGE_WARFARE_UNIT))
			}	// if (0 != uiCharacterID)
		}	// for (ssTileY = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY++)
	}	// for (ssTileX = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX++)

	// ´ëÀåÈ¿°ú(°ø°Ý·Â ¾÷) Àû¿ë ¹ÞÁö ¾Ê´Â´Ù.
	return ON_ATTACK_TYPE_NORMAL;
}

// robypark 2004/11/1 16:22
// °ø¼ºÀü À¯´ÖÀ¸·Î º¯½ÅÇÏ°íÀÚ ÇÒ ¶§ ÀÚ½Å ÁÖº¯¿¡ Àû´ë»ó´Ü ¸¶À»ÀÇ ¹üÀ§ ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
// Return Value: TRUE  - Àû »ó´Ü ¸¶À» ¹üÀ§ ¾È¿¡ Ä³¸¯ÅÍ°¡ Á¸ÀçÇÏ°Å³ª º» ¸ÊÀÌ ¾Æ´Ï´Ù.
//				 FALSE - º¯½Å °¡´ÉÇÑ °÷¿¡ Ä³¸¯ÅÍ°¡ Á¸ÀçÇÑ´Ù.
BOOL cltOnlineWorld::SiegeWarfare_IsInAreaDisabledTransform(void)
{
	// °ø¼º ½Ã°£ÀÎÁö ºñ±³
	if(FALSE == IsInTimeSiegeWarfare())
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_SIEGEWARFARE_TIME_IS), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END); 
		return TRUE;
	}

	// ÀüÅõ ´ë±âÁß
	if(pMyData->IsWaitBattleResponse())
		return TRUE;

	// ¸¶À»¿¡ µé¾î°¡´Â ÁßÀÌ°Å³ª, ¸¶À» µé¾î°¡±â À§ÇÑ ´ë±â ÁßÀÌ¶ó¸é
	if ((pMyData->IsGotoVillage()) && (pMyData->IsWaitGotoVillageResponse() == TRUE))
		return TRUE;

	// º» ¸ÊÀÌ ¾Æ´Ñ °æ¿ì º¯½Å ºÒ°¡´É
	if (pOnlineMap->m_MapIndex != 0)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));
		return TRUE;
	}

	// Ä³¸¯ÅÍ°¡ ¸¶À» ¾È¿¡ ÀÖ´Â °æ¿ì
	if (pOnlineVillage != NULL)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_VILLAGE));
		return TRUE;
	}

	// Ä³¸¯ÅÍ°¡ ¼ö¼º ÀÎÅÍÆäÀÌ½º ¾È¿¡ ÀÖ´Â °æ¿ì
	if (pMyData->IsInVillageDefenceInterface())
		return TRUE;

	// »ó´ÜÀÌ ¾ø´Â °æ¿ì
	if (NULL == pMyData->GetGuildName())
		return TRUE;

	// ÀÌ¹Ì °ø¼ºÀü À¯´ÖÀÏ °æ¿ì °ø¼º À¯´ÖÀ¸·Î ÀçÂ÷ º¯½ÅÇÒ ¼ö ¾ø´Ù.
	if (NULL != m_pSiegeUnitTableMgr->GetSiegeUnitTable(pMyData->GetMyCharKind()))
		return TRUE;

	// ÀÚ½ÅÀÌ ÁÖº¯ÀÇ Àû´ë»ó´Ü ¸¶À»ÀÇ ¹üÀ§ ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
	if (TRUE == SiegeWarfare_IsInAreaHostileVillage())
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "",
								pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_NEAR_HOSTILITY_VILLAGE));

		// º¯½Å ºÒ°¡´É
		return TRUE;
	}

	// º¯½Å °¡´É
	return FALSE;
}

// robypark 2005/1/13 21:39
// ÀÚ½Å ÁÖº¯¿¡ Àû´ë»ó´Ü ¸¶À»ÀÇ ¹üÀ§ ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
// Return Value: TRUE  - Àû »ó´Ü ¸¶À» ¹üÀ§ ¾È¿¡ Ä³¸¯ÅÍ°¡ Á¸Àç.
//				 FALSE - ±ÙÃ³¿¡ Àû´ë »ó´ÜÀÌ ¾ø´Â °÷¿¡ Ä³¸¯ÅÍ°¡ Á¸ÀçÇÑ´Ù.
BOOL	cltOnlineWorld::SiegeWarfare_IsInAreaHostileVillage(void)
{
	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ Unique ID ¾ò±â
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ À§Ä¡
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// ÀÚ½ÅÀÇ À§Ä¡ Ã£±â
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// °Ë»ö ¹üÀ§
	SI16 ssTileX, ssTileY;

	// 21 X 21 Å¸ÀÏÀÌ³»¿¡ Àû »ó´Ü ¼Ò¼Ó ¸¶À»ÀÌ ÀÖ´ÂÁö Ã£±â
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY++)
		{
			// °Ë»öÇÒ Å¸ÀÏ °è»ê
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// ÇØ´ç ÁÂÇ¥ÀÇ ¸Ê¿¡ ÀÖ´Â ¿ÀºêÁ§Æ®ID ¾ò±â
			SI16 siObjectID = pOnlineMap->GetObjectID(siTargetX, siTargetY);

			// À¯È¿ÇÑ°¡?
			if (siObjectID != 0)
			{
				// ¿ÀºêÁ§Æ®°¡ ¸¶À»ÀÎÁö ¾Ë¾Æº»´Ù.
				if (pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					// ¸¶À» ÄÚµå¸¦ ¾ò¾î¿Â´Ù.
					UI16 uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

					// À¯È¿ÇÑ ¸¶À» ÄÚµåÀÎÁö?
					if (0 != uiVillageCode)
					{
						// Àû »ó´Ü(°ø¼ºÇÏ´Â ³¯) ¸¶À»ÀÎÁö?
						if (GetVillageWarDayGuild(uiVillageCode))
						{
							// Àû »ó´Ü ¸¶À» ¹üÀ§¾È¿¡ ÀÖ´Ù.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// Àû´ë »ó´Ü ¸¶À» ¹üÀ§ ¹Û
	return FALSE;
}

// robypark 2005/1/13 21:39
// ÀÚ½ÅÀÌ ÁÖº¯ÀÇ ¾Æ±º»ó´Ü ¸¶À»ÀÇ ¹üÀ§ ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
// Return Value: TRUE  - ¾Æ±º »ó´Ü ¸¶À» ¹üÀ§ ¾È¿¡ Ä³¸¯ÅÍ°¡ Á¸Àç.
//				 FALSE - ±ÙÃ³¿¡ ¾Æ±º »ó´ÜÀÌ ¾ø´Â °÷¿¡ Ä³¸¯ÅÍ°¡ Á¸ÀçÇÑ´Ù.
BOOL	cltOnlineWorld::SiegeWarfare_IsInAreaOurVillage(void)
{
	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ Unique ID ¾ò±â
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// ÀÚ½ÅÀÇ Ä³¸¯ÅÍ À§Ä¡
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// ÀÚ½ÅÀÇ À§Ä¡ Ã£±â
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// °Ë»ö ¹üÀ§
	SI16 ssTileX, ssTileY;

	// 21 X 21 Å¸ÀÏÀÌ³»¿¡ ¾Æ±º »ó´Ü ¼Ò¼Ó ¸¶À»ÀÌ ÀÖ´ÂÁö Ã£±â
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY++)
		{
			// °Ë»öÇÒ Å¸ÀÏ °è»ê
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// ÇØ´ç ÁÂÇ¥ÀÇ ¸Ê¿¡ ÀÖ´Â ¿ÀºêÁ§Æ®ID ¾ò±â
			SI16 siObjectID = pOnlineMap->GetObjectID(siTargetX, siTargetY);

			// À¯È¿ÇÑ°¡?
			if (siObjectID != 0)
			{
				// ¿ÀºêÁ§Æ®°¡ ¸¶À»ÀÎÁö ¾Ë¾Æº»´Ù.
				if (pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					// ¸¶À» ÄÚµå¸¦ ¾ò¾î¿Â´Ù.
					UI16 uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

					// À¯È¿ÇÑ ¸¶À» ÄÚµåÀÎÁö?
					if (0 != uiVillageCode)
					{
						// ¾Æ±º »ó´Ü ¸¶À»ÀÎÁö?
						if (GetVillageOurGuild(uiVillageCode))
						{
							// ¾Æ±º »ó´Ü ¸¶À» ¹üÀ§¾È¿¡ ÀÖ´Ù.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// ¾Æ±º »ó´Ü ¸¶À» ¹üÀ§ ¹Û.
	return FALSE;
}

// robypark 2005/1/13 21:45
// °ø¼º¿¡ ¹æÇØµÇ´Â ÀÏ¹Ý À¯ÀúµéÀ» °¢±¹ ÃÊº¸ÀÚ ¼ö·ÃÀåÀ¸·Î ¿öÇÁ½ÃÅ³ ¼ö ÀÖ´ÂÁö °Ë»ç
BOOL	cltOnlineWorld::SiegeWarfare_CheckAbledWarfRefugee(UI16 uiDestUniqueID)
{
	// °ø¼ºÀü ½Ã°£ÀÌ¶ó¸é
	if (IsInTimeSiegeWarfare())
	{
		if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()))		// »ç¿ëÀÚ°¡ °ø¼ºÀü À¯´ÖÀÌ¶ó¸é
			&& ( FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiDestUniqueID) )			// ´ë»óÀÌ °ø¼ºÀü À¯´ÖÀÌ ¾Æ´Ï¸ç,
			&& ( TRUE == pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind(uiDestUniqueID) ) )	// ´ë»óÀÌ ÇÃ·¹ÀÌ¾îÀÌ¸ç,
			&& ( FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiDestUniqueID) ) 	// ´ë»óÀÌ ¼ö¼º ÀÎÅÍÆäÀÌ½º¿¡ ÀÖ´Â Ä³¸¯ÅÍ°¡ ¾Æ´Ï¸ç,
			&& ( FALSE == pIOnlineChar->GetBattleFlag(uiDestUniqueID) )	)				// ´ë»óÀÌ ÀüÅõ ´ë±âÁßÀÎ Ä³¸¯ÅÍ°¡ ¾Æ´Ï¸ç,
		{
			// »ç¿ëÀÚ »ó´Ü³» Á÷±ÞÀÌ ´ë¹æ, ´ëÇà¼ö, Çà¼öÀÎ °æ¿ì
			if ((pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				|| (pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
				|| (pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)))
			{
				// °°Àº »ó´Ü¿øÀÌ ¾Æ´Ï¶ó¸é
				if (FALSE == IsMemberOfSameGuild(uiDestUniqueID))
				{
					// Àû´ë »ó´Ü¿øÀÌ ¾Æ´Ï¶ó¸é,
					//if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiDestUniqueID))
					{
						if ((TRUE == SiegeWarfare_IsInAreaOurVillage())				// »ç¿ëÀÚ°¡ ¾Æ±º »ó´Ü ¸¶À» ±ÙÃ³ÀÌ°Å³ª,
							|| (TRUE == SiegeWarfare_IsInAreaHostileVillage()))		// Àû´ë »ó´Ü¸¶À» ±ÙÃ³¶ó¸é
						{
							// ¿öÇÁ ½ÃÅ³ ¼ö ÀÖ´Ù.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// ¿öÇÁ ½ÃÅ³ ¼ö ¾ø´Ù.
	return FALSE;
}

// robypark 2004/11/2 15:0
// °ø¼ºÀü À¯´Ö °ø°Ý ¼Ó¼º¿¡ µû¶ó È¿°ú »ç¿ëÇÏ±â
void	cltOnlineWorld::SiegeWarfare_CreateAttackEffectCharacter(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID, bool bNeedToSendActionMsg/* = false*/)
{
	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	SI32 siEmptySmokeID = 0;

	// °ø°ÝÀÚ À§Ä¡
	SI16 siPosition_AttakerX, siPosition_AttakerY;
	// ÇÇ°ÝÀÚ À§Ä¡
	SI16 siPosition_TargetX, siPosition_TargetY;

	// °ø°ÝÀÚ ¹× ÇÇ°ÝÀÚ Ä³¸¯ÅÍ À§Ä¡ ¾ò±â
	pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
	pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

	// Ä³¸¯ÅÍ °ø°Ý ¼Ó¼º¿¡ µû¸¥ È¿°ú ¿¬Ãâ
	switch (KI[uiKindInfoIdx_Attacker].GetAttackAtb(0))
	{
		// °ø¼º °Ëº´, ±â¸¶º´ °ø°Ý
		case ATTACKATB_SWORD:
			siEmptySmokeID = m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_BLADEHIT"));

			// robypark 2005/1/14 18:52
			// ¸ñÇ¥ Ä³¸¯ÅÍ Áß½É¿¡ ±×·ÁÁöµµ·Ï ¼³Á¤
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SWORD, 0, 0);
			break;

		// °ø¼º Ã¢º´ °ø°Ý
		case ATTACKATB_LONG_SPEAR:
			siEmptySmokeID = m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_SPEARHIT"));

			// robypark 2005/1/14 18:52
			// ¸ñÇ¥ Ä³¸¯ÅÍ Áß½É¿¡ ±×·ÁÁöµµ·Ï ¼³Á¤
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SPEAR2, 0, 0);
			break;

		// °ø¼º ±Ãº´ °ø°Ý
		case ATTACKATB_ARROW:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_ARROW, siPosition_AttakerX, siPosition_AttakerY, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));
			
			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_ARROW, 0, 0);
			break;

		// °ø¼º ÃæÂ÷ °ø°Ý
		case ATTACKATB_UPGRADETOWERCANNON:
			// °ø°Ý È÷Æ® È¿°ú ÀÌ¹ÌÁö´Â Ãâ·ÂµÇÁö ¾ÊÀ¸³ª È¿°úÀ½¸¸ ¿¬ÁÖÇÑ´Ù.
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_WALLBREAKERHIT"), true);

			// robypark 2005/1/14 18:52
			// ¸ñÇ¥ Ä³¸¯ÅÍ Áß½É¿¡ ±×·ÁÁöµµ·Ï ¼³Á¤
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_JU104AE1, 0, 0);
			break;

		// °ø¼º ¹ß¼®°Å °ø°Ý
		case ATTACKATB_CATAPULT:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_STONE, siPosition_AttakerX - 1, siPosition_AttakerY - 1, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_CU106AE1, 0, 0);
			break;
	}
}

// robypark 2004/11/3 14:5
// °ø¼ºÀü À¯´Ö °ø°Ý ¼Ó¼º¿¡ µû¶ó È¿°ú »ç¿ëÇÏ±â
// ¸ñÇ¥¹°ÀÇ À§Ä¡¿¡ »ý¼º
// Effect Áß¿¡ SmokeÀÏ °æ¿ì ¹«Á¶°Ç ±Ù°Å¸® °ø°ÝÀ¸·Î ÆÇ´ÜÇÏ¸ç ¼­¹ö¿¡ Action¸Þ½ÃÁö¸¦ Àü¼ÛÇÏ±â ¾Ê´Â´Ù.
// ¹Ý¸é¿¡ BulletÀÏ °æ¿ì ¹«Á¶°Ç ¿ø°Å¸® ¸¶À» °ø°ÝÀ¸·Î ÆÇ´ÜÇÏ¸ç ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION¸Þ½ÃÁö¸¦ ¼­¹ö·Î Àü¼ÛÇÑ´Ù.
// ÁÖÀÇ ¼­¹ö¿¡ ¸Þ½ÃÁö Àü¼ÛÀº ÀÚ½Å°ú °ü·ÃµÈ °æ¿ì¿¡¸¸ ÇØ´çµÈ´Ù.
void cltOnlineWorld::SiegeWarfare_CreateAttackEffectVillage(UI16 uiAttackerUniqueID, SI16 ssTargetX, SI16 ssTargetY, UI16 uiVillageCode/* = 0*/)
{
	// °ø°ÝÀÚ Ä³¸¯ÅÍ Á¾·ù Á¤º¸ ÀÎµ¦½º
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// °ø°ÝÀÚ À§Ä¡
	SI16 siPosition_AttakerX, siPosition_AttakerY;

	// °ø°ÝÀÚ Ä³¸¯ÅÍ À§Ä¡ ¾ò±â
	pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);

	// Ä³¸¯ÅÍ °ø°Ý ¼Ó¼º¿¡ µû¸¥ È¿°ú ¿¬Ãâ
	switch (KI[uiKindInfoIdx_Attacker].GetAttackAtb(0))
	{
		// °ø¼º °Ëº´, ±â¸¶º´ °ø°Ý
		case ATTACKATB_SWORD:
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_BLADEHIT"));

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SWORD, 0, 0);
			break;

		// °ø¼º Ã¢º´ °ø°Ý
		case ATTACKATB_LONG_SPEAR:
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_SPEARHIT"));

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SPEAR2, 0, 0);
			break;

		// °ø¼º ±Ãº´ °ø°Ý
		case ATTACKATB_ARROW:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_ARROW, siPosition_AttakerX, siPosition_AttakerY, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));
			
			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_ARROW, 0, 0);
			break;

		// °ø¼º ÃæÂ÷ °ø°Ý
		case ATTACKATB_UPGRADETOWERCANNON:
			// °ø°Ý È÷Æ® È¿°ú ÀÌ¹ÌÁö´Â Ãâ·ÂµÇÁö ¾ÊÀ¸³ª È¿°úÀ½¸¸ ¿¬ÁÖÇÑ´Ù.
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_WALLBREAKERHIT"), true);

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_JU104AE1, 0, 0);
			break;

		// °ø¼º ¹ß¼®°Å °ø°Ý
		case ATTACKATB_CATAPULT:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_STONE, siPosition_AttakerX - 1, siPosition_AttakerY - 1, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));

			// È¿°úÀ½ ¿¬ÁÖ
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_CU106AE1, 0, 0);
			break;
	}
}

// robypark 2004/11/8 15:38
// °ø¼ºÀü À¯´ÖÀÎÁö¸¦ °Ë»çÇÑ´Ù.
BOOL	cltOnlineWorld::IsSiegeWarfareUnit(SI32 siImjinKindIdx)
{
	// À¯È¿ÇÑ Ä³¸¯ÅÍ ÀÎµ¦½º ÀÎÁö °Ë»ç
	if (KIND_ON_INVALID == siImjinKindIdx)
		return FALSE;

	// Ä³¸¯ÅÍ ÀÎµ¦½º ¹üÀ§ ¾ÈÀÎÁö °Ë»ç
	if (KIND_ON_MAX <= siImjinKindIdx)
		return FALSE;

	// °ø¼ºÀü À¯´ÖÀÎÁö °Ë»ç
	if (TRUE == KI[siImjinKindIdx].IsKindAtb(KINDATB_SIEGE_WARFARE_UNIT))
		return TRUE;

	return FALSE;
}

// robypark 2004/11/24 16:52
// °ø¼ºÀü ½Ã°£ µîÀÇ ¿É¼Ç ÆÄÀÏ HQ¿¡¼­ ÀÐ±â
BOOL cltOnlineWorld::LoadSOptionSiegeWarfare(char *pszServiceNation, SI16 siFindServiceType, char *pszFileName)
{
	FILE*	fp;
	OnlineCommonParser	ocp;

	// HQ¿¡¼­ ÆÄÀÏÀ» ¿¬´Ù.
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : Ãâ·ÂµÉ ¿¡·¯ÆÄÀÏ¸í ÀûÀç
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "LoadSOptionSiegeWarfare Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// ÀÚ·á °Ë»ö
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// µ¥ÀÌÅÍ ¶óÀÎ ÃßÃâ

		SI32	iLineNo, iColNo;

		// ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î ¹Þ´Â´Ù.
		SI32	siServiceType;

		char	pszNation[32];

		ocp.GetValue( pszNation,								31,		iLineNo, iColNo );		// Service Nation
		ocp.GetValue( siServiceType,									iLineNo, iColNo );		// Service Type
		// Ã£À¸·Á´Â ¼­ºñ½º ±¹°¡ÀÎÁö ºñ±³
		if (stricmp(pszNation, pszServiceNation) == 0)
		{
			// Ã£À¸·Á´Â ¼­ºñ½º Å¸ÀÔ(ÇÁ·ÐÆ® ¼­¹ö) ºñ±³	¿¹] ´ëÇÑ¹Î±¹ ÇÏ·æÀº 0, Á¤¼·Àº 1
			if (siServiceType == siFindServiceType)
			{
				// °ø¼º ¿äÀÏ ÀÐ±â
				for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
				{
					ocp.GetValue( (char &)m_SOptionSiegeWarfare.scSiegeDay[i],	iLineNo, iColNo );		// °ø¼ºÇÏ´Â ¿äÀÏ
				}

				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN,					iLineNo, iColNo );		// °ø¼º ½ÃÀÛ ½Ã°£
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END,					iLineNo, iColNo );		// °ø¼º Á¾·á ½Ã°£

				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_Begin,	iLineNo, iColNo );		// ÆøÅº ÅõÀÚ ±ÇÇÑ °ü·Ã Ã³¸® ½ÃÀÛ½Ã°£
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End,	iLineNo, iColNo );		// ÆøÅº ÅõÀÚ ±ÇÇÑ °ü·Ã Ã³¸® Á¾·á½Ã°£

				// ´ëÀå È¿°ú ¹üÀ§
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range,	iLineNo, iColNo );
				// °ø¼ºÀü À¯´ÖÀ¸·Î º¯½ÅÇÒ ¼ö ¾ø´Â Àû´ë »ó´Ü ¸¶À»°úÀÇ °Å¸®
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range, 	iLineNo, iColNo );
				// º¸±Þ ¼ö·¹ ¸¶³ª 10% ÀÚµ¿ Ã¤¿ì±â Äð Å¸ÀÓ
				ocp.GetValue( m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime,		iLineNo, iColNo );

				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_Cost,		iLineNo, iColNo );	// ¸¶À» ¹Ý°Ý ¹æ¾î·Â Çâ»ó ºñ¿ë
				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_Amount,	iLineNo, iColNo );	// ¸¶À» ¹Ý°Ý ¹æ¾î·Â Çâ»óµÇ´Â ¼öÄ¡
				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_CoolTime,	iLineNo, iColNo );	// ¸¶À» ¹Ý°Ý ¹æ¾î·Â Çâ»ó Äð Å¸ÀÓ(ÃÊ)

				// °ø¼ºÀü ÁØºñ¸¦ À§ÇÏ¿© ´ë¹æ, ´ëÇà¼ö, Çà¼ö°¡ »ó´Ü ¼Ò¼Ó ¸¶À»¿¡¼­ º´·Â¸ðÁý ºÒ°¡´É ½Ã°£
				m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_Begin	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_End	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END;		// °ø¼º ³¡³ª°í ÇÑ ½Ã°£ ´õ ¸ðÁý ºÒ°¡´É

				// »ç¿ëÀÚµéÀÌ »ó´Ü °¡ÀÔÀÌ ºÒ°¡´ÉÇÑ ½Ã°£
				m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_Begin		= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_End			= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End;

				// »ç¿ëÀÚµéÀÌ »ó´Ü¿¡¼­ Å»Åð°¡ ºÒ°¡´ÉÇÑ ½Ã°£(´ë¹æÀÌ »ó´Ü¿ø ÇØ°íµµ ºÒ°¡´ÉÇÏ´Ù)
				m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_Begin	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_End		= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End;
				break;
			}
		}

		// È®ÀÎ »ç»ì - ÀÌ°ÍÀÌ Áö±Ý ¸¶Áö¸· ¶óÀÎÂ°¸¦ Ã³¸®ÇÏ°í ÀÖ´Ù¸é ºê·¹ÀÌÅ©
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// robypark 2004/12/1 10:11
// »ç¿ëÀÚÀÇ ¿ëº´(ÁÖÀÎ°ø Æ÷ÇÔ) ÀÎº¥Åä¸®¿¡ °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
BOOL cltOnlineWorld::IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(void)
{
	UI08		uiFollowerSlot = 0;
	UI16		i = 0;

	// »ç¿ëÀÚÀÇ ¸ðµç ¿ëº´ ½½·ÔÀ» °Ë»çÇÑ´Ù.
	for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
	{
		// ÇØ´ç ½½·ÔÀÇ ¿ëº´ÀÇ ÀÎº¥Åä¸®¿¡ °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
		if (IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(uiFollowerSlot))
			return TRUE;	// ÀÎº¥Åä¸®¿¡ °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÑ´Ù.
	}

	// °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ »ç¿ëÀÚ ÀÎº¥Åä¸®¿¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
	return FALSE;
}

// robypark 2005/1/8 16:26
// ÇØ´ç ½½·ÔÀÇ ¿ëº´ÀÇ ÀÎº¥Åä¸®¿¡ °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
BOOL cltOnlineWorld::IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(UI08	uiFollowerSlot)
{
	UI16		i = 0;
	MyItemData	tempMyItemData;
	CItemHeader *pItem = NULL;

	// À¯È¿ÇÑ ¿ëº´ ½½·ÔÀÌ¶ó¸é
	if (pMyData->IsValidFollower(uiFollowerSlot))
	{
		// ÀÎº¥Åä¸®¿¡ ÀÖ´Â ¾ÆÀÌÅÛµéÀ» °Ë»ç
		for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
		{
			// ÀÎº¥Åä¸®¿¡¼­ ¾ÆÀÌÅÛ ¾ò±â
			if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
			{
				// À¯È¿ÇÑ ¾ÆÀÌÅÛÀÌ¶ó¸é
				if(tempMyItemData.siPosInInventory != -1)
				{
					// ¾ÆÀÌÅÛ Á¤º¸ Çì´õ ¾ò±â
					pItem = pItemsInfo->GetItem(tempMyItemData.uiID);
					if (pItem)
					{
						// °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ¶ó¸é
						if (pItem->m_pImperium)
						{
							// ÀÎº¥Åä¸®¿¡ °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÑ´Ù.
							return TRUE;
						}	// if (pItem->m_pImperium)
					}	// if (pItem)
				}	// if(tempMyItemData.siPosInInventory != -1)
			}	// if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
		}	// for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	}	// if (pMyData->IsValidFollower(uiFollowerSlot))

	// °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ ÇØ´ç ½½·ÔÀÇ ¿ëº´ ÀÎº¥Åä¸®¿¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
	return FALSE;
}

// robypark 2004/12/2 17:31
// ÁÖÀÎ°ø ¹× ¿ëº´ ÀÎº¥Åä¸®¿¡ ÀÖ´Â ¸ðµç °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀ» »èÁ¦ÇÑ´Ù.
void cltOnlineWorld::DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit(void)
{
	UI08		uiFollowerSlot = 0;
	UI16		i = 0;
	MyItemData	tempMyItemData;
	CItemHeader *pItem = NULL;

	// »ç¿ëÀÚÀÇ ¸ðµç ¿ëº´ ½½·ÔÀ» °Ë»çÇÑ´Ù.
	for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
	{
		// À¯È¿ÇÑ ¿ëº´ ½½·ÔÀÌ¶ó¸é
		if (pMyData->IsValidFollower(uiFollowerSlot))
		{
			// ÀÎº¥Åä¸®¿¡ ÀÖ´Â ¾ÆÀÌÅÛµéÀ» °Ë»ç
			for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
			{
				// ÀÎº¥Åä¸®¿¡¼­ ¾ÆÀÌÅÛ ¾ò±â
				if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
				{
					// À¯È¿ÇÑ ¾ÆÀÌÅÛÀÌ¶ó¸é
					if(tempMyItemData.siPosInInventory != -1)
					{
						// ¾ÆÀÌÅÛ Á¤º¸ Çì´õ ¾ò±â
						pItem = pItemsInfo->GetItem(tempMyItemData.uiID);
						if (pItem)
						{
							// °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ¶ó¸é
							if (pItem->m_pImperium)
							{
								// ÀÎº¥Åä¸®¿¡ °ø¼ºÀü ¼ÒÁý±Ç ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÑ´Ù. ¾ÆÀÌÅÛÀ» »èÁ¦ÇØ¶ó
								pMyData->DelItem(uiFollowerSlot, &tempMyItemData);
							}	// if (pItem->m_pImperium)
						}	// if (pItem)
					}	// if(tempMyItemData.siPosInInventory != -1)
				}	// if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
			}	// for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
		}	// if (pMyData->IsValidFollower(uiFollowerSlot))
	}	// for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
}

// robypark 2004/11/13 12:24
// °ø¼ºÀü À¯´ÖÀÏ °æ¿ì °ø¼ºÀü À¯´ÖÀÇ »ç¿ëÀÚ ÀÌ¸§, Ã¼·Â, º¸±ÞÇ°, µîÀ» º¼ ¼ö ÀÖ´ÂÁö¸¦ ³ªÅ¸³»´Â ÇÃ·¡±× ¼³Á¤
void	cltOnlineWorld::SetIsShowInformationAboutSiegeWarfareUnit(SI08 scIsShowInformationAboutSiegeWarfareUnit)
{
	m_scIsShowInformationAboutSiegeWarfareUnit = scIsShowInformationAboutSiegeWarfareUnit;
}

// robypark 2005/1/8 11:18
// º¸±ÞÇ° ÀÚµ¿ ¼Ò¸ð ³²Àº Áö¿¬½Ã°£ ¾ò±â
SI32	cltOnlineWorld::GetRestTimeAutoSpendSupplyGoods(void)
{
	return m_siRestTimeAutoSpendSupplyGoods;
}

// robypark 2005/1/8 11:18
// º¸±ÞÇ° ÀÚµ¿ ¼Ò¸ð ³²Àº Áö¿¬½Ã°£ ¼³Á¤
void	cltOnlineWorld::SetRestTimeAutoSpendSupplyGoods(SI32 siRestTimeAutoSpendSupplyGoods)
{
	m_siRestTimeAutoSpendSupplyGoods = siRestTimeAutoSpendSupplyGoods;
}

// robypark 2005/1/12 19:30
// °ø¼ºÀü °ø°Ý ¸ðµå ¼³Á¤ ÃÊ±âÈ­
void	cltOnlineWorld::CancelSiegeWarfare_AttackMode(void)
{
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
}

// H2-S-05.02.04 //
// À¯Àú ¾îÄ«¿îÆ®·Î Ä³¸¯ÅÍ ÀÌ¸§ ¾ò´Â´Ù.
char*	cltOnlineWorld::OnlineCharName(UI16 nAccount)
{
	return pIOnlineChar->GetCharName(nAccount);
}