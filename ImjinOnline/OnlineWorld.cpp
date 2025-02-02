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
//#include	<OnlineBasicTip.h>			// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
//#include	<OnlineBasicQuest.h>		// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
#include	<OnlineParty.h>
#include	<OnlineFollower.h>
#include	<IProfit.h>
#include	<OnlineDungeonParser.h>
#include	<OnlineTicket-Parser.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
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
//#include    "OnlineBanner.h"			// actdoll (2004/02/25 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
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
#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME 컨트롤러 세팅

#include "../SCCommon/OnlineCommonParser.h"	// Using Online Common Parser

#include	"FieldHeroInfoMgr.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

#include "SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"


//준석이형 추가부분 
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

// sagolboss 추가부분 ->
#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif
// <- 추가 끝 

//#define	CPF		33				// 한 프레임당 클럭수는 40이다.
//#define	CPF		1			// 한 프레임당 클럭수는 40이다.

extern _clGame* pGame;
extern	_MainData				GD;									// 게임의 주데이터 
extern _InputDevice				IpD;
extern	SoundManager			*g_pSoundManager;
extern	CCharFileLoadManager	g_FileLoadManager;					// 조합형 캐릭터
extern CItem					*g_pItem;
extern char						LogInID[128];

extern clock_t					CurrentClock, MonitorMacroClock1, MonitorMacroClock2;

extern int WindowModeSwitch;
extern int bActive;

LPDIRECTDRAWSURFACE7              lpCaptureSurface = NULL;

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 

// 임진록 2 클라이언트와 통신을 하기 위한 변수이다.
OnlineImjin2I					GOnlineImjin2I;

// 디버그를 위한 전역변수.
// HQDebug  GHQDebugTool;

// actdoll (2004/08/27 0:34) : 입력기 상태 보는 디버그 창(_DEBUG시만 가동. 없앨려면 주석달것)
//#define _SHOW_INPUTSTATUS_

// robypark 2005/1/7 18:41
// 수치(10%단위로)를 게이지 형태로 그려준다.
// 공성전 유닛 체력, 보급 게이지에서 사용
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

//----------------------------------------------------------------------------------------------------------------
//	설명	:	온라인 월드 초기 함수.
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
//	설명	:	온라인 함수.
//----------------------------------------------------------------------------------------------------------------
UI16	_clGame::OnlineWorld()
{
	UI16	uiNextStatus = 0;

	uiNextStatus	=	pOnlineWorld->Poll();

	if(uiNextStatus)	
	{
		// GS_INITONLINEBATTLEGAME은 임진록2 전투로 들어가기 위함이므로
		// 이때 모든 메모리를 해제해서는 안된다.
		if(uiNextStatus != GS_INITONLINEBATTLEGAME)
			FreeOnlineWorld();	
	}
	
	return uiNextStatus;		
}

//----------------------------------------------------------------------------------------------------------------	
//	설명	:	온라인 해제 함수.
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
//	설명	:	임진록 온라인 월드 함수.
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

	// 공성전 공격 모드 설정 변수
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	// robypark 2005/1/14 17:11
	// 주인공 캐릭터들의 정보 관리자 초기화
	m_pFieldHeroInfoMgr = NULL;

	// robypark 2005/1/31 12:55
	// 공성 유닛 관리자
	m_pSiegeUnitTableMgr = NULL;

	// 버젼을 만든다.
	siOnlineMajorVersion		=	ON_CV_MAJOR;
	siOnlineMinorVersion		=	ON_CV_MINOR;

	bExitOnlineGame				=	FALSE;
	bFindTrade					=	FALSE;

	ZeroMemory( m_TempBuffer, sizeof( m_TempBuffer ) );
	wsprintf(szOnlineVersion, "Ver %d.%03d", siOnlineMajorVersion, siOnlineMinorVersion / 10);

	ZeroMemory(m_MemoUser, 32);

	bCharMove		= TRUE;

//	m_bEventDokdoQuestPlay	= FALSE;	// actdoll (2004/04/02 15:28) : = 이벤트 = 독도 퀘스트를 진행중인가?

	// 주막 도착한 아이템관련
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
//	m_Spr_Icon_DokdoQuest.Image	= NULL;	// actdoll (2004/04/02 15:28) : = 이벤트 = 독도 퀘스트용 이미지 아이콘

//	m_IME_DLLHandle = NULL;
	
	// actdoll (2004/03/30 11:02) : = 이벤트 = 독도퀘스트 초기화
//	m_uiDokdoQuestCompleteUserCount	= 0;

	// 공성전 유닛일 경우 공성전 유닛의 사용자 이름, 체력, 보급품, 등을 볼 수 있는지를 나타내는 플래그
	m_scIsShowInformationAboutSiegeWarfareUnit = 0;

	m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_NONE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	임진록 온라인 월드 함수.
//----------------------------------------------------------------------------------------------------------------
cltOnlineWorld::~cltOnlineWorld()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	임진록 온라인 월드 초기화.
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

	// 공성전 공격 모드 설정 변수
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	// 버젼을 만든다.
	siOnlineMajorVersion		=	ON_CV_MAJOR;
	siOnlineMinorVersion		=	ON_CV_MINOR;

	bExitOnlineGame				=	FALSE;
	bFindTrade					=	FALSE;

	ZeroMemory( m_TempBuffer, sizeof( m_TempBuffer ) );
	wsprintf(szOnlineVersion, "Ver %d.%03d", siOnlineMajorVersion, siOnlineMinorVersion / 10);

	ZeroMemory(m_MemoUser, 32);

	bCharMove		= TRUE;

	// actdoll (2004/04/02 15:28) : = 이벤트 = 독도 퀘스트 관련
//	m_bEventDokdoQuestPlay	= FALSE;	
//	if(m_Spr_Icon_DokdoQuest.Image)  {clGrp.FreeXspr(m_Spr_Icon_DokdoQuest);  m_Spr_Icon_DokdoQuest.Image  = NULL;}

	// 주막 도착한 아이템 관련
	m_bReceivedItem			= FALSE;
	m_bItemDraging			= FALSE;
	m_siReceivedItemCount	= 0;
	m_siInnStatus			= -1;
	if(m_ReceivedItemAniSpr.Image)  {clGrp.FreeXspr(m_ReceivedItemAniSpr);  m_ReceivedItemAniSpr.Image  = NULL;}
		

	pOnlineCharUI->Init(this);

#ifdef _OGG_PLAY
	// actdoll (2004/11/25 13:35) : 감마니아 제팬의 문제로 당분간 보류
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
	// 전투모드인 상태로 로그아웃 후에 재접속하여도 화면 인터페이스에 표시되는 텍스트가
	// 평화모드로 초기화 되었음에도 불구하고 전투모드로 표시되는 버그 수정.
	// 역시, 거래차단 텍스트 역시 문제 수정
	pOnlinePannel->SetButtonText_PeaceMode(!bPeace);
	pOnlinePannel->SetButtonText_TradeMode(!pOnlineTrade->m_bPermitTrade);

	// 공성전 유닛일 경우 공성전 유닛의 사용자 이름, 체력, 보급품, 등을 볼 수 있는지를 나타내는 플래그
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

	// actdoll (2004/11/15 18:45) : 추가
	if( !g_bSkipIntro )		IntroPoll();

	//----------------------------------------------------------------------------
	//	프론트 로딩화면 이미지 준비
	//----------------------------------------------------------------------------

	pOnlineLoadingData = new OnlineLoadingData;
	ZeroMemory( pOnlineLoadingData, sizeof(pOnlineLoadingData) );

	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
	SetGamePalette(uiPalette);

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_bg.Spr", pOnlineLoadingData->BackImage );	
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_bg_over.Spr", pOnlineLoadingData->BackOverImage );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_play_grade.Spr", pOnlineLoadingData->PlayGradeImage );
	if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Gamania.Spr", pOnlineLoadingData->m_Spr_CompanyLogo );	// 한국/중국/일본/대만/홍콩용
	else														clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Bolehcom.Spr", pOnlineLoadingData->m_Spr_CompanyLogo );	// 인도네시아용

	// actdoll (2004/12/20 19:13) : 로고 수정
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Gersang_Logo_Ani.Spr", pOnlineLoadingData->AniLogoImage );

	m_LanguageCode = pGame->LanguageCode; 

	//----------------------------------------------------------------------------
	//	프론트 로딩화면 이미지 준비 완료
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 1단계	- "巨商" 글자 중 '巨' 좌측 세로변 1획		: START
	//----------------------------------------------------------------------------
	// 온라인에서 사용하는 폰트를 생성한다.
	pOnlineFonts	=	new	OnlineFonts;
	if( !pOnlineFonts->Create() )
	{
		clGrp.Error("OnlineFonts Init Error", "");
		delete pOnlineLoadingData;
		pOnlineLoadingData	= NULL;
		return FALSE;
	}

	// 온라인에서 출력용으로 사용하는 텍스트 엔진을 형성한다.
	pOnlineText				=	new OnlineText;
	if(pOnlineText->Init( (CHAR*)pGame->m_pszHQNation, (CHAR*)( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) ) == FALSE)
	{
		clGrp.Error("OnlineText Init Error", "");
		delete pOnlineText;
		pOnlineText	= NULL;
		return FALSE;
	}
	
	// actdoll (2004/05/04 16:33) : [OECEX]
	// 에디트 핸들러의 기본 세팅
	Handler_OnlineEditControlEx::Initialize( pGame->Hwnd );
	Handler_OnlineEditControlEx::SetCandidateFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	// actdoll (2004/11/03 17:10) : IME 컨트롤러 관련
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
	//	로딩 1단계	- "巨商" 글자 중 '巨' 좌측 세로변 1획		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 2단계	- "巨商" 글자 중 '巨' 상단 가로변 1획		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(10);

	// 임진록 온라인에서 쓰일 리소스등을 로드한다.	
	pOnlineResource		=	new	OnlineResource;	
	if(pOnlineResource->Init() == FALSE) 
		clGrp.Error("FKJE8567", "Online Resource Init Failed");	
	//----------------------------------------------------------------------------
	//	로딩 2단계	- "巨商" 글자 중 '巨' 상단 가로변 1획		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 3단계	- "巨商" 글자 중 '巨' 중앙 변 2획			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(20);

	pOnlineMap	=	new OnlineMap;	
	// 맵을 불러온다.
	// 실질적으로 맵을 메모리에 로드하지 않는다. 다만 필요할때 로드할 수 있도록 화일 이름만 저장한다.	
	if(pOnlineMap->Create(this) == FALSE)
	{
		clGrp.Error("Map Load Error.", "");						
	}		

	m_pOnlineMapSound = new OnlineMapSound();
	ASSERT(m_pOnlineMapSound);
	{
		// 국가별 음악 설정 관련. 
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

	// 온라인에서 사용하는 모든 마우스를 관리하는 객체를 생성한다.
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
	// actdoll (2004/11/25 13:35) : 감마니아 제팬의 문제로 당분간 보류
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
	
	GD.Init();					// 게임 데이타 초기화	
	//----------------------------------------------------------------------------
	//	로딩 3단계	- "巨商" 글자 중 '巨' 중앙 변 2획			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 4단계	- "巨商" 글자 중 '巨' 하단 변 1획 글자 완성	: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(30);

	/*********************************************************************************************************************
									로딩 스프라이트를 로드하고 팔레트를 로드 한다.
	*********************************************************************************************************************/	
	// 온라인에서 사용하는 폰트를 생성한다.
//	pOnlineFonts	=	new	OnlineFonts;
//	pOnlineFonts->Create();

	// 온라인에서 출력용으로 사용하는 텍스트 엔진을 형성한다.
//	pOnlineText				=	new OnlineText;
//	if(pOnlineText->Init( (CHAR*)pGame->m_pszHQNation ) == FALSE)
//		clGrp.Error("OnlineText Init Error", "");	

	m_cltIGuildClass.Init("Online\\Village\\grade.txt");

	/*********************************************************************************************************************
									Palette Manager를 설정한다.
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
	//	다른 방식으로 해당 정보를 로딩한다.
	m_SlanderFilter.LoadReferenceWord( (CHAR*)pGame->m_pszHQNation, ( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) );
	//----------------------------------------------------------------------------
	//	로딩 4단계	- "巨商" 글자 중 '巨' 하단 변 1획 글자 완성	: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 5단계	- "巨商" 글자 중 '商' 상단 점 1획			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(40);

	/*********************************************************************************************************************
									TicketParser를 설정한다.
	*********************************************************************************************************************/
	pTicketParser               =   new OnlineTicketParser;
	pTicketParser->ReadText("Online\\Village\\TicketInfo.txt");

	/*********************************************************************************************************************
									GeneralParser를 설정한다.
	*********************************************************************************************************************/
	pGeneralParser               =   new OnlineGeneralParser;
	pGeneralParser->ReadText("Online\\CharInfo\\General_list.txt");

	/*********************************************************************************************************************
									GeneralParserEx를 설정한다.(2차 전직 용병)
	*********************************************************************************************************************/
	pGeneralParserEx               =   new OnlineGeneralParser;
	pGeneralParserEx->ReadText("Online\\CharInfo\\GeneralListEx.txt");

	/*********************************************************************************************************************
									QuestParser를 설정한다.
	*********************************************************************************************************************/
	pQuestParser				=	new OnlineQuestParser;
	// robypark 2004/9/9 17:53
	// 이벤트용 퀘스트 관련 인자 추가
	char szHQNation[128];
	sprintf(szHQNation, "%s", (char*)pGame->m_pszHQNation);
	pQuestParser->ReadText("Online\\Quest\\quest.txt", szHQNation, 2004, 9, 13);

	/*********************************************************************************************************************
									LevelMark Parser를 설정한다.
	*********************************************************************************************************************/
	pLevelMarkParser				=	new OnlineLevelMarkParser;
	pLevelMarkParser->Create("Online\\CharInfo\\LevelMark.txt");

	/*********************************************************************************************************************
									Timer를 설정한다.
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
									통장에 관한 설정
	*********************************************************************************************************************/	
	pOnlineEconomy = new OnlineEconomy;
	pOnlineEconomy->Init("Online\\Village\\Economy.txt");

	// 다이렉트 드로우 객체를 생성한다. 반드시 리소스를 로드한 후 호출해 주어야 한다.
	if(clGrp.CreateOffScreen(pOnlineDirectDraw, ON_SCREEN_XSIZE, ON_SCREEN_YSIZE, TRUE) == FALSE)
	{
		clGrp.Error("FKJE8567", "Online DirectDraw Create Failed");	
		return FALSE;
	}
	
	// 맵 종류를 초기화한다.

//	DrawTextInLoading("마을 관리자 생성 중");
	// 마을 관리자를 생성한다.
	pOnlineVillageManager	=	new OnlineVillageManager;

	if(!pOnlineVillageManager->Init("Online\\Village\\VillageInfo.txt", "Online\\Village\\Structure Placement.txt", this))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_VILLAGE_MANAGER_INIT_ERROR), "");						
	}		
	m_fChattingRoom             =   FALSE;

	// 마울 설명 List 생성
//	DrawTextInLoading("마을 안내문 관리자 생성 중");
	pOnlineVillageDescriptList = new OnlineVillageDescriptList;
	pOnlineVillageDescriptList->Init(2, 16);

	// 공지사항 List
	pOnlineNoticeList = new OnlineNoticeList;
	pOnlineNoticeList->Init(this, 10, 5, 233, 582, 288, 4);

	// 필드상 Effect
	pOnlineFieldEffect = new OnlineFieldEffect;
	pOnlineFieldEffect->Init(this, 64);

	// 스킬
	pOnlineSkillParser = new OnlineSkillParser;
	pOnlineSkillParser->Init();
	//----------------------------------------------------------------------------
	//	로딩 5단계	- "巨商" 글자 중 '商' 상단 점 1획			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 6단계	- "巨商" 글자 중 '商' 상단 가로변 1획		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(50);

	// 오브젝트 인터페이스를 생성한다.

	pIOnlineObject	=	new	IOnlineObject;

	// 오브젝트을 초기화 한다.
	if(!pIOnlineObject->Init())
	{
		clGrp.Error("FKJE8567", pOnlineText->Get(ON_TEXT_OBJECT_INIT_ERROR));
	}	

	// 키보드 입력을 처리하는 클래스를 생성한다.
	pOnlineKeyboard	=	new	OnlineKeyboard;
	pOnlineKeyboard->Init();

	// 캐릭터의 문자열 액션 코드를 숫자 액션 코드로 변환해 주는 객체를 생성한다.
	// 캐릭터의 애니메이션 정보를 초기화 하기전에 해주어야 한다.
	pOnlineCharActionCode	=	new OnlineCharActionCode;
	
	// 캐릭터의 애니메이션 정보를 초기화 한다. 반드시 캐릭터를 초기화 하기 이전에 해주어야 한다.	
//	pOnlineCharAni	=	new OnlineCharAni;

	//clGrp.Error("", "pOnlineCharAni->Init 1");
/*	if(!pOnlineCharAni->Init(this))
	{
		clGrp.Error("캐릭터 애니메이션 정보를 초기화 하는데 실패했습니다.", "");
	}*/
	//clGrp.Error("", "pOnlineCharAni->Init 2");

	// 배 종류를 초기화 한다.
//	clGrp.Error("", "pIOnlineShipKI	=	new IOnlineShipKI 1");
	pIOnlineShipKI	=	new IOnlineShipKI;
//	clGrp.Error("", "pIOnlineShipKI	=	new IOnlineShipKI 2");

//	clGrp.Error("", "pIOnlineShipKI->Init() 1");
	if(!pIOnlineShipKI->Init())
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_SHIP_KIND_INIT_ERROR), "");
	}
//	clGrp.Error("", "pIOnlineShipKI->Init() 2");

	// 캐릭터 종류를 초기화 한다.
	pIOnlineCharKI	=	new	IOnlineCharKI;

	if(!pIOnlineCharKI->Init(this))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_CHAR_KIND_INIT_ERROR), "");
	}
	//----------------------------------------------------------------------------
	//	로딩 6단계	- "巨商" 글자 중 '商' 상단 가로변 1획		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 7단계	- "巨商" 글자 중 '商' 상중단 점 2획			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(60);

//	DrawTextInLoading("오브젝트 종류 초기화, 캐릭터 생성, 마을 생성 중");
	// 오브젝트 종류를 초기화 한다.
	pIOnlineObjectKI	=	new IOnlineObjectKI;

	if(!pIOnlineObjectKI->Init(pOnlineResource, pOnlineText))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_OBJECT_KIND_INIT_ERROR), "");
	}	

	// 캐릭터 인터페이스를 생성한다.
	pIOnlineChar	=	new	IOnlineChar;

	// 캐릭터를 초기화 한다.
	if(pIOnlineChar->Init(this) == FALSE)
	{
		clGrp.Error("FKJE8567", pOnlineText->Get(ON_TEXT_CHAR_INIT_ERROR));
	}

	// 갈칮기 인공지능을 생성한다.
	pclFindPath	=	new cltOnlineFindPath(ON_MAP_XSIZE, ON_MAP_YSIZE, pOnlineMap);
	
	pOnlineMap->GetSize(&siMapXsize, &siMapYsize);
	pclFindPath->SetFindPathSize(siMapXsize, siMapYsize);		
	//----------------------------------------------------------------------------
	//	로딩 7단계	- "巨商" 글자 중 '商' 상중단 점 2획			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 8단계	- "巨商" 글자 중 '商' 좌측 세로변 1획		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(70);

	// 나의 데이타를 생성한다.
	pMyData		=	new	MyData;
	pMyData->Init(this);

	// 내 캐릭터를 조작하기 위한 객체를 생성한다.	
	pOnlineCharUI	=	new	OnlineCharUI;
	pOnlineCharUI->Init(this);	
	
	dwCurrentFrameStartTime		=	0;
	dwCurrentFrameAddTime		=	0;
	dwCurrentFrameSkippingCount	=	0;
	siMainPollStatus			=	ON_MAINPOLL_LOGINMGR;	

	// 이모티놐 처리
	pOnlineImoticon = new OnlineImoticon;
	pOnlineImoticon->Init(this);

	// 인벤토리를 생성한다. 여기서는 생성만 해주고 초기화는 나의 데이타를 받은 이후에 해준다.
	pOnlineInventory		=	new	OnlineInventory;
	pOnlineTradeBook		=	new OnlineTradeBook;
	pOnlineTradeBook->Init( this );

	// 온라인에서 사용하는 모든 아이템을 로드한다.		
	pItemsInfo				=	new CItem;		

	pItemsInfo->ItemLoad(	pOnlineText, 
							"Online\\Item\\weapon.txt", "Online\\Item\\armor.txt", "Online\\Item\\accessary.txt", "Online\\Item\\Animal.txt", 
							"Online\\Item\\Dress.txt", "Online\\Item\\Element.txt", "Online\\Item\\Food.txt", "Online\\Item\\Liquor.txt", 
							"Online\\Item\\Medicine.txt", "Online\\Item\\Tool.txt", "Online\\Item\\Trade.txt", "Online\\Item\\Specific.txt" ,
							"Online\\Item\\Imperium.txt");

	g_pItem	=	pItemsInfo;

	for( i=0 ; i<(SI32)pItemsInfo->GetCount(); ++i)											// 리소스로 변환
	{
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );
				
		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			if( pOnlineResource->GetItemFileIndex( szSpriteFileName ) == -1 )					// 큰 그림을 로드한다.( 그림이 로드가 돼어 있지 않다면 로드함)			
				pOnlineResource->AddItemSpr( szSpriteFileName );									

			if( pOnlineResource->GetItemFileIndex( szSmallFileName ) == -1 )					// 작은 그림을 로드한다.			
				pOnlineResource->AddItemSpr( szSmallFileName );			
		}		

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// robypark 2004/11/24 16:55
	// 공성전 시간 등의 옵션 파일 HQ에서 읽기
	LoadSOptionSiegeWarfare(szHQNation, pGame->m_iServiceType, "Online\\OptionSiegeWarfare.txt");

	// robypark 2005/1/14 17:11
	// 주인공 캐릭터들의 정보 관리자 생성 및 로드
	m_pFieldHeroInfoMgr = new CFieldHeroInfoMgr;
	m_pFieldHeroInfoMgr->LoadFieldHeroInfo();

	// robypark 2005/1/31 12:55
	// 공성 유닛 관리자
	m_pSiegeUnitTableMgr = new CSiegeUnitTableMgr;
	m_pSiegeUnitTableMgr->LoadSiegeUnitTable();			// 공성 유닛 기본 테이블
	m_pSiegeUnitTableMgr->LoadSiegeUnitTableCalcHP();	// 전투력에 따른 체력 계산 테이블
	m_pSiegeUnitTableMgr->LoadSiegeUnitTableCalcMP();	// 전투력에 따른 마력 계산 테이블

	//----------------------------------------------------------------------------
	//	로딩 8단계	- "巨商" 글자 중 '商' 좌측 세로변 1획		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 9단계	- "巨商" 글자 중 '商' 중앙 변 2획			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(80);

	for( i=0; i<(SI32)pItemsInfo->GetCount() ; ++i)												// 다쉬 한번 아이템 구조체에 리소스 인덱스를 넣어 준다.
	{		
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );					// 아이템 파일을 받는다.

		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			// 큰 그림
			siFile		=	pOnlineResource->GetItemFileIndex( szSpriteFileName );			// 리소스에서 파일 이름넣어 인덱스를 받는다.
			pTempSpr	=	pOnlineResource->GetxsprItem( siFile );

	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSpriteFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, FALSE );
			else					clGrp.Error("", pOnlineText->Get(ON_TEXT_ITEM_SEARCH_ERROR), szSpriteFileName );

			// 작은 그림
			siFile		=	pOnlineResource->GetItemFileIndex( szSmallFileName );				// 리소스에서 파일 이름넣어 인덱스를 받는다.
			pTempSpr	=	pOnlineResource->GetxsprItem( siFile );
			
	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSmallFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, TRUE );
			else					clGrp.Error("", pOnlineText->Get(ON_TEXT_ITEM_SEARCH_ERROR), szSmallFileName );
		}

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// Config 파일용
	pConfig					=	new OnlineConfig;
	pConfig->OpenConfig();
	if(LogInID[0]) pConfig->SetConfig( LogInID );

	// Mega Text용	
	pOnlineMegaText			=	new	OnlineMegaText;	
	pOnlineMegaText->Initialize(pOnlineResource, pOnlineFonts);	

	pOnlineBuilding			=	new OnlineBuilding;
	if( pOnlineBuilding->Init() == FALSE )
		clGrp.Error("", pOnlineText->Get(ON_TEXT_STRUCT_INIT_ERROR) );

	// 필드 지역 관리 객체.
	pOnlineFieldArea		=	new OnlineFieldArea;
	pOnlineFieldArea->Init();	

	// 판넬을 초기화 한다.	
	pOnlinePannel			=	new OnlinePannel;
	pOnlinePannel->Init(this);	

	pIProfit				=	new cltIProfit;
	pIProfit->Init( "Online\\Village\\Profit.txt" );

	// 미니맵을 로드함
	pOnlineMiniMap			=	new OnlineMiniMap;
	pOnlineMiniMap->Init(this);

	pOnlineHelp				=	new OnlineHelp;
	pOnlineHelp->Init(this);
	
	pOnlineClient	=	new	OnlineClient;
	pOnlineClient->InitZero();	
	pOnlineClient->InitSocket();					// 한번만 실행

	// RequestPlayerInfo 변수를 초기화 한다.
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
	//	로딩 9단계	- "巨商" 글자 중 '商' 중앙 변 2획			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 10단계	- "巨商" 글자 중 '商' 안쪽 변 5획 글자 완성	: START
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

//	pOnlineBasicTip					=	new OnlineBasicTip;		// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
//	pOnlineBasicTip->Init(this);

//	pOnlineBasicQuest				=	new OnlineBasicQuest;	// actdoll (2004/02/16 16:11) : 사용하는 곳이 없으며 전부 확인했습니다.
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

	// actdoll (031229) : 초상 클래스를 초기화한다. 일단 잠가놓겠다.
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
	// 매크로 프로세서
//	m_pMacroProcess = new CMacroCheckProcess(static_cast<COnlineClientCallback*>(pOnlineClient));
//	m_pMacroProcess->StartThread();

//준석이형 추가부분 
//	pJSWndMgr = new JSWndMgr(this, 1);
//	pJSTestWnd = new JSTestWnd;

	
	pGame->ChatInit();
	//----------------------------------------------------------------------------
	//	로딩 10단계	- "巨商" 글자 중 '商' 안쪽 변 5획 글자 완성	: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	로딩 11단계	- "巨商" 글자								: START
	//----------------------------------------------------------------------------
	// Loading11
	ScrollLoading(100);
	
	//디폴트 캐릭터 초기화.
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
//	설명	:	임진록 온라인 월드.
//	리턴	:	다음 GameStatus 값.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::Poll()
{
	UI16	uiTempMainPollStatus;
	UI16	uiNextMenu	=	0;

	//============================================================================================
	// 마우스 위치처리
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

	// 마우스를 세팅한다.
	if ((siGamePollStatus != ON_GAMEPOLL_FIELD) && (siGamePollStatus != ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE))
		pIOnlineMouse->Action();
	pIOnlineMouse->SetPos(IpD.Mouse_X, IpD.Mouse_Y);

	// 마우스 Up Check
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
		// 임진록 온라인에서 사용하는 일부 자원들을 해제한다.
		// 모든 자원을 해제하면 안된다.		
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
//	설명	:	메인 Poll() 초기화
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitGamePoll()
{		
	RECT	rcMap;

	siGamePollStatus		=	ON_GAMEPOLL_FIELD;	

/*	UI08	uiPalette[768];	
	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
	SetGamePalette(uiPalette);*/

	// 클리핑을 설정한다.(맵을 기준으로 클리핑을 설정한다.)
	rcMap	=	pOnlineMap->GetScreenRect();
	clGrp.SetClipArea(rcMap.left, rcMap.top, rcMap.right, rcMap.bottom);

	// 키보드 버퍼를 청소해준다.
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

	// 아이템 도착 표시이미지 로딩
	clGrp.LoadXspr( "Online\\GameSpr\\Inn_ReceivedItem.Spr", m_ReceivedItemAniSpr );

	// actdoll (2004/04/02 15:30) :	= 이벤트 = 독도 퀘스트 아이콘 로딩
//	clGrp.LoadXspr( "Online\\GameSpr\\Icon_Taekuk.SPR", m_Spr_Icon_DokdoQuest );

	pConfig->OpenConfig();

	if(!m_pMercenary->IsAction())
		m_pMercenary->OnButtonPush();

	pOnlineFieldAuction->OnGameStart();

	::SetFocus(pGame->Hwnd);		
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	메인 Poll()
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
	// 일반 MSG 처리
	while(1)
	{
		uiClientRet	 =	pOnlineClient->DataProc();										// 데이타의 프로시저.

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
	// 마을로 이동
	if(pMyData->IsGotoVillage() && (pMyData->IsWaitGotoVillageResponse() == FALSE) && siGamePollStatus == ON_GAMEPOLL_FIELD )
	{
		// 서버에게 마을로 들어간다고 요청한다.		
		pOnlineClient->SendGotoVillage(pMyData->GetGotoVillage());
		pMyData->SetWaitGotoVillageResponse(TRUE);			
	}


	//============================================================================================
	// Game Server가 이상없는지 검사
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
	// Portal로 이동
	if(pOnlineMsgBoxUsePortal->IsActive())
	{
		SI16			TempX, TempY;

		switch(pOnlineMsgBoxUsePortal->Process())
		{
			case ON_MSGBOX_RETURN_OK:
				// 돈이 돼는지 검사

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
					// 레벨이나 신용도가 돼는지 검사

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
						// 현재 선택한 Portal의 ID 를 검색

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
			// 서버에게 Portal로 들어간다고 요청한다.						
			siGotoPortalID	= pMyData->GetGotoPortal();			// 들어갈 Portal의 이름을 얻어온다.
			
			// 현재 Portal을 들어갈수 있는 지 검사
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
					// 현재 선택한 Portal의 ID 를 검색

					NextMapIndex = siGotoPortalID;
					pOnlineClient->SendGotoPortal(siGotoPortalID);

					pMyData->SetWaitGotoPortalResponse(TRUE);
				}
			}
		}
	}
	
	//============================================================================================
	// 필드로 이동
//	if(pMyData->IsGotoField() == TRUE)
//	{
//		GotoField();	
//		pMyData->SetGotoField(FALSE);	
//	}	

	//============================================================================================
	// 시간에 따른 밤낮 처리
	float					TempLight;
	SOnlineTime				NowTime;
	static SOnlineTime		YetNowTime;

	TempLight  = 1.0f;
	NowTime    = pTimer->GetNowTime();

	if(pOnlineVillage != NULL)
	{
		if(pOnlineVillage->GetStatus() != ON_VILLAGE_STATUS_IN_STRUCTURE) 
		{
			// 현재 위치가 건물안이 아니면 시간에 따라 밝게함
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
		// 현재 위치가 건물안이 아니면 시간에 따라 밝게함
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
	// 마을 공격시 생기는 Delay 처리
	if(m_VillageAttackDelay)
	{
		m_VillageAttackDelay--;
	}
 
	//============================================================================================
	// 마우스 One Click
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

			// 수성 인터페이스 Poll
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
//	설명	:	캐릭터가 필드상에 있을때 호출하는 함수.
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

	// 다음 프레임을 진행할 수 있는 상황이 될때까지 대기한다.
	dwPrevFrameStartTime			-=	dwCurrentFrameAddTime;
	dwCurrentFrameAddTime			=	0;
	dwPrevFrameTimeRequire			=	timeGetTime() - dwPrevFrameStartTime;

	// 초당 프레임수를 맞춘다.
	if(dwPrevFrameTimeRequire < CPF)
	{
		Sleep(1);
		return uiNextMenu;
	}
	else
	{
		dwCurrentFrameAddTime	=	dwPrevFrameTimeRequire - CPF;
	}

	// 만약 컴퓨터가 딸리면 Render를 하지 않는다.
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
			// 겜임폴 접속시 설정했던것 해제하기
			pOnlineSetting->GameInit();

			// 로그인 화면으로
			SetMainPollStatus(ON_MAINPOLL_LOGINMGR);
			SetLogOut(0, ON_MAINPOLL_LOGINMGR);
			m_WaitTickCount = 0;
			return uiNextMenu;
		}
	}
	////////////////////////////////////////////////////
	// 프레임을 증가 시킨다.
	uiTotalFrame++;


	siFPS	=	1000 / (timeGetTime() - dwPrevFrameStartTime);

	// 공성전 유닛
	if (TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()))
	{
		// 공격 지연 시간 자동 갱신
		pIOnlineChar->SetSiegeWarfareUnit_PastAttackDelay(pMyData->GetMyCharUniqueID(), dwPrevFrameTimeRequire);

		/*
		// 공성전 보급 수레인 경우 자동 소모되지 않는다.
		if (KI[pIOnlineChar->GetImjinKind( pMyData->GetMyCharUniqueID() )].IsAttribute1(ATB_PATIENT))
		{
			// 보급품 자동 소모 남은 시간 처리
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

	// 현재 시간을 얻어온다.
	dwCurrentFrameStartTime		=	timeGetTime();		

	// 위에서 FrameSkipping 때문에 Up Check를 무시하고 지나갈 경우가 있기때문에 여기서만 다시 검사

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

	DWriteStringToFile("시작 2");

	// 오버 헤드	
	pIOnlineMouse->Action();
	pOnlineMousePoint->Action();

	// 키보드의 입력상태를 얻어온다.
	pOnlineKeyboard->Action();

	//준석이형 추가부분 
	// 모든 윈도 관리자
//	pJSWndMgr->Action();
	
	ptMouse.x	=	IpD.Mouse_X;
	ptMouse.y	=	IpD.Mouse_Y;

	pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

	// 마을 정보 처리
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
		// 현재 지역을 체크하여 그 지역의 배경음악을 설정한다.
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
				// 주인공 관련 데이터 관리자 얻기
				CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

				if (pFieldHeroInfo)
				{
					// 한 곳에 오래 있을 경우 지루하다고 대사 플레이
					if (pFieldHeroInfo->GetSndBoring())
						HashCodePushEffect(pFieldHeroInfo->GetSndBoring(), 0, 0);
				}

				siTalkDealy = 0;
			}
		}
		else
			siTalkDealy = 0;

		// 현재 있는 위치가 Portal Zone 이면
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
							// 마을에 근접해 있다. 마을 안으로 들어간다.										
							pMyData->SetGotoPortal(TRUE, siPortalCode);

							// 캐릭터에게 정지 명령을 내려준다.
							pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

							// 더 이상의 사용자 입력을 받지 않는다.
							pOnlineCharUI->SetAction(FALSE);
						}
					}
				}
			}
		}
	}
	
	// 이모티콘을 처리한다.
	pOnlineImoticon->Pool(fLeftUpSwitch);

	if(pOnlineVillage == NULL)
	{
		// 마우스 있는 위치에 적 캐릭터가 있으면 마우스를 변경한다.
		// 단, 필드 영역이여야 한다.	
		pOnlineHelp->Initialize();

		pOnlineMsgBoxPay->Process();
		if(pOnlineMsgBoxError->Process() == ON_MSGBOX_RETURN_OK)
		{
			pOnlineClientChatting->Close();
			return ON_MAINPOLL_FORCED_LOGOUT;
		}
	}
	
	DWriteStringToFile("시작 3");

	// robyprk 2004/11/5 18:5
	// 사용자 자신이 공성전 유닛인지 검사
	BOOL bIsSiegeWarfareUnit = pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID());

	// 변신 버튼 비활성화
	pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);

	// robypark 2005/1/17 11:0
	// 피난민으로 변신 관련 처리
	if (IsInTimeSiegeWarfare())	// 공성시간이라면
	{
		// robypark 2005/1/29 13:32
		// 초보자 수련장으로 워프 될 수 있다는 경고 메시지 설정
		if (m_sWarningMsgInSiegeWarfare.m_dwLeftTime < dwPrevFrameTimeRequire)
		{
			HDC hdc;
			char szMsg[1024] = {0, };

			// 공성중 주의사항 메시지 출력
			// HDC를 이용하여 찍어야 할 정보를 설정
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				sprintf(szMsg, pOnlineText->Get(8100118), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);

				SetMessage4(szMsg, hdc, -1, 0, RGB(0, 255, 0));

				GETSCREENSURFACE(this)->ReleaseDC(hdc);
			}

			// 채팅 창에도 추가
			pOnlinePannel->AddChat((char*)szMsg, ON_INN_COLOR_GREEN, CHATTINGMODE_NORMAL);

			m_sWarningMsgInSiegeWarfare.m_dwLeftTime = WARNINGMSGINSIEGEWARFARE_DELAY;
		}
		else
			m_sWarningMsgInSiegeWarfare.m_dwLeftTime -= dwPrevFrameTimeRequire;

		// robypark 2005/1/29 14:6
		// 공성전 보급수레인가?
		if (KI[pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID())].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// 보급수레(메딕)일 경우 10초마다 마나가 10%씩 회복된다.
			if (FALSE == m_sRecoveryMana.m_bSendedRequest)
			{
				if (m_sRecoveryMana.m_dwLeftTime < dwPrevFrameTimeRequire)
				{
					m_sRecoveryMana.m_dwLeftTime	 = m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime;
					m_sRecoveryMana.m_bSendedRequest = TRUE;

					// 서버에 마나 자연증가 요청
					pOnlineClient->SendReuqestChargeMP();
				}
				else
					m_sRecoveryMana.m_dwLeftTime -= dwPrevFrameTimeRequire;
			}
		}


		// 주인공 모드였다면,
		if (SIEGE_TRANSFORM_MODE_HERO == m_scSiegeTransformMode)
		{
			// 공성하는 날인 경우의 사용자만
			if (pMyData->IsSiegeWarfareDay())
			{
				// 사용자를 제외한 모든 주인공 캐릭터를 피난민으로 변신시킨다.
				pIOnlineChar->AllHeroCharacterTransformTo();

				// 피난민 모드 설정
				m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_REFUGEE;
			}
		}

		// 공성전 유닛이 아니며
		if (FALSE == bIsSiegeWarfareUnit)
		{
			// 변신 가능한가?
			if ((NULL != pMyData->GetGuildName())
				&& (pMyData->CanTransformSiegeWarfareUnit())
				&& (pMyData->IsSiegeWarfareDay())
				&& (FALSE == pOnlinePannel->GetSiegeWarfareTransformUIWnd()->IsSendedMsg()))
			{
				// 본 맵이라면
				if (0 == pOnlineMap->m_MapIndex)
				{
					// 변신 버튼 활성화
					pOnlinePannel->SetShowTransformSiegeUnitBtn(TRUE);
				}
				else
				{
					// 변신을 위해 열려진 창이 있다면 닫기
					pOnlinePannel->ShowWindow_SiegeWarfareTransformUIWnd(FALSE);
				}

				// 미니 맵이 활성화?
				if (pOnlineMiniMap->IsActive())
				{
					// 미니맵 상태가 전체화면일 경우?
					if (pOnlineMiniMap->ISFullMode())
					{
						// 변신 버튼 비활성화
						pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);
					}
				}
			}
		}
	}
	else
	{
		// 피난민 모드였다면
		if (SIEGE_TRANSFORM_MODE_REFUGEE == m_scSiegeTransformMode)
		{
			// 사용자를 제외한 모든 피난민 캐릭터를 주인공 캐릭터로 변신시킨다.
			pIOnlineChar->AllHeroCharacterTransformTo();

			// 주인공 모드 설정
			m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_HERO;
		}

		// 변신을 위해 열려진 창이 있다면 닫기
		pOnlinePannel->ShowWindow_SiegeWarfareTransformUIWnd(FALSE);
	}

	// 마우스 커서 아래 오브젝트 처리(풍선 도움말등을 위한)
	if (pOnlineVillage == NULL)
	{
		// robypark 2005/1/26 18:33
		ProcessFieldMouseMove(bIsSiegeWarfareUnit, ptMouse);
	}

	// 공성전 마을공격
	if(pOnlineVillage == NULL)
	{
		if(SIEGEWARFARE_ATTACKMODE_VILLAGE == m_sSiegeWarfare_AttackMode.scAttackMode)
		{
			UI16		ObjectCode;

			// 공성 시간인지 비교
			if(IsInTimeSiegeWarfare())
			{
				// Object 를 찾는다.
				ObjectCode = pIOnlineObject->GetVillageObject(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);
				
				if(ObjectCode != 0)
				{
					if(GetVillageWarDayGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))	// 공성하는 마을이다.
					{
						BOOL bRangeAttacker = FALSE;		// 원거리, 근거리 공성전 유닛 구분 플래그
						if (TRUE == SiegeWarfare_CanAttackVillage(pMyData->GetMyCharUniqueID(), ObjectCode, bRangeAttacker))
						{
							// 마을이면 공격
							pIOnlineObject->GetPos(ObjectCode, &siTX, &siTY);

							// 근거리 공성전 유닛일 경우
							if (FALSE == bRangeAttacker)
							{
								pOnlineCharUI->SetShortDistanceAttackVillage(siTX, siTY);
							}
							// 원거리 공성전 유닛일 경우
							else
							{
								pOnlineCharUI->SetLongDistanceAttackVillage(siTX, siTY);
							}
						}
					}
					else
					{
						// 적대 마을일 경우
						if (GetVillageEnemyGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))
						{
							// 적대 마을이지만 오늘 이 마을의 상단과 공성전이 없다.
							char szErrorMsg[512];
							SVillageData*		lpSVillageData;
							VillageHeader*		lpVillageHeader;

							lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);
							lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);

							if ((lpSVillageData) && (lpVillageHeader))
							{
								// 상단이 있는 마을인 경우
								if (NULL != lpSVillageData->m_VillageGuildName[0])
								{
									sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
												lpSVillageData->m_VillageGuildName, 
												pOnlineText->Get(lpVillageHeader->siVillageName ));
								}
								// 상단이 없는 마을인 경우
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
							// 적대 마을이 아닐경우
							pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
						}

						// 공성전 공격 모드 설정 변수
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

				// 공성전 공격 모드 설정
				m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
				m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			}
		}
	}

	// 공성전 캐릭터간 전투
	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if(SIEGEWARFARE_ATTACKMODE_CHARACTER == m_sSiegeWarfare_AttackMode.scAttackMode)	// 캐릭터공격 모드
			{
				if(bCharMove == TRUE)
				{
					// 공격자 캐릭터가 공성전 유닛인지 검사
					if (TRUE == bIsSiegeWarfareUnit)
					{
						// 공성 유닛 간에 전투이다. 공격이 가능한지 검사
						if (TRUE == SiegeWarfare_CanAttackUnit(pMyData->GetMyCharUniqueID(), m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID))
						{
							pOnlineCharUI->SetFieldAttack(m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID);
						}				
					}	
				}	// if(bCharMove == TRUE)
			}	// if (캐릭터 공격 모드)
		}	// if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
	}


	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if (fRightOneSwitch == 1)
			{
				// 공성전 공격 모드 설정
				m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
				m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

				if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					// robypark 2005/1/26 18:22
					// 필드에 있을 때 마우스 커서 오른쪽 버튼 클릭 처리
					ProcessFieldMouseRightBtnClick(bIsSiegeWarfareUnit);
				}
			}

			if(IpD.LeftPressSwitch)
			{		
				// 자신의 캐릭터를 이동시킨다.
				// 현재 마우스가 가리키는 것을 기준으로 이동, 공격, 마을에 들어간다, 아이템을 줍는다 등의 명령을 내린다.
				if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					// robypark 2005/1/26 18:22
					// 필드에 있을 때 마우스 커서 왼쪽 버튼 클릭 처리
					ProcessFieldMouseLeftBtnClick(bIsSiegeWarfareUnit, siX, siY);
				}
			}
		}
	}

	pOnlineCharUI->Poll();

	// 단축키를 처리한다.
	FieldShortCutKey();	
 
	if(pOnlineVillage == NULL)
	{
//		m_pNpcTalk->Action();

		m_pQuestInterface->Action();

		m_pChatMacro->Action();

		pOnlineParty->Action();

		// 판넬
		pOnlinePannel->Poll();

		// 인벤토리.
		pOnlineInventory->SetFlagVillage(FALSE);
		pOnlineInventory->Action();

		// 거래창
		pOnlineTrade->Action();

		// 장부
		pOnlineTradeBook->Poll();


		// 좌판
		pOnlineBooth->ActionBench();

		// 팁
		pOnlineTip->Action();

		// Quest Play
		pOnlineQuestPlay->Poll();

//		//초보자 도움말.
//		pOnlineBasicTip->Action();

//		pOnlineBasicQuest->Action();

		

		// 경매 
		pOnlineFieldAuction->Process();

		// 접수
		pOnlineReception->Process();

		// 대장장이
		pOnlineSmith->Poll();

//		pJSTestWnd->Action();

		// 아이템 획득 창 관련 행동을 한다. 
		pGame->pItemInfoWindow->Prepare();

		// 메신저
		pOnlineMessenger->Action();

		m_pMercenary->Action();

		if( bFindTrade )
			FindTradeBook();

		// 미니맵
		pOnlineMiniMap->Pool(FALSE);

		// 내 캐릭터의 명령을 처리한다.(이동, 공격, 마을 들어감, 아이템 줍기)
		ProcessMyCharOrder();
	}
		
	// 보낼 메시지가 있으면 보내준다.
	RPI.Send();

	// Resource 관리
//	if( timeGetTime() - m_ImageFreeTimer > 5000 )
	{
		pOnlineResource->Prepare();
//		FreeOldImageByTime();

//		m_ImageFreeTimer = timeGetTime();
	}

	// 오버헤드 End
	// 오브젝트 액션.	
	pIOnlineObject->Action();
	

	// 캐릭터 액션.		
	pIOnlineChar->Action();		

	// 따라다니는 용병들 처리
	pIOnlineChar->ActionByVMercenary();

	pOnlineMap->Action();

	// robypark 2004/10/25 13:28
	// 공성전 전투 관련 효과 처리
	m_SiegeWarfare_BulletMgr.Action();
	m_SiegeWarfare_SmokeMgr.Action();

	//퀘스트 아이콘을 출력한다.
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
	// 화면에 그린다.	

	pOnlineFieldEffect->Pool();

	
	DWriteStringToFile("그리기 1");
	if(fRender)
	{
		FieldDraw();	
	}
	DWriteStringToFile("그리기 2");
	

	if( bExitOnlineGame )
	{
		siMainPollStatus = ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}

	if(pOnlineVillage == NULL)
	{
		// 환경설정
		pOnlineSetting->Action();
	}

	DWriteStringToFile("끝");

	return uiNextMenu;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	필드에서 화면 출력부분을 처리한다.
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
		// 타일, 캐릭터, 오브젝트를 그린다.	
		pOnlineMap->Draw(GETSCREENSURFACE(this));	
		
		// robypark 2004/10/25 13:27
		// 공성전 전투 효과 그리기
		m_SiegeWarfare_BulletMgr.Draw();
		m_SiegeWarfare_SmokeMgr.Draw();

		pOnlineFieldEffect->Render(GETSCREENSURFACE(this));

		pOnlineMousePoint->Draw();

		pIOnlineChar->AllDrawSpeech(GETSCREENSURFACE(this));

		// NPC 말하기
		m_pNpcTalk->Action();
		m_pNpcTalk->Draw(GETSCREENSURFACE(this));

		// Macro
		m_pChatMacro->Draw(GETSCREENSURFACE(this));
		
		// robypark 2004/11/13 16:37
		// 공성전 유닛 정보 창들을 모두 표시하는 경우 처리
		if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
			{
				// HDC를 이용하여 찍어야 할 캐릭터에 대한 정보를 찍어준다.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// 모든 공성전 유닛의 캐릭터명 그리기
					pIOnlineChar->AllDrawName(hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);

					if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
					{
						// 모든 공성전 유닛 체력 및 보급 게이지 그리기
						pIOnlineChar->AllDrawSiegeWarfareUnit_StatusBar();

						// 모든 공성 유닛 상단 깃발 그리기
						pIOnlineChar->AllDrawSiegeWarfareUnit_GuildFlag();

						clGrp.UnlockSurface(GETSCREENSURFACE(this));
					}
				}	// if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)

		// 현재 마우스가 캐릭터를 가르키고 있으면
		if(m_SelectCharacterIndex != -1)
		{
			// 공성전 유닛일 경우 체력 게이지 출력하기
			if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
			{
				// HDC를 이용하여 찍어야 할 캐릭터에 대한 정보를 찍어준다.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// 공성전 유닛 캐릭터명 그리기
					pIOnlineChar->DrawName(m_SelectCharacterIndex, hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);
				}

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// 모든 공성전 유닛 체력 및 보급 게이지 그리기
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(m_SelectCharacterIndex);

					// 모든 공성 유닛 상단 깃발 그리기
					pIOnlineChar->DrawGuildFlag(m_SelectCharacterIndex);

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}
		}

		// HDC를 이용하여 찍어야 할 캐릭터에 대한 정보를 찍어준다.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);

			hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
			SetTextColor(hdc, RGB(0, 0, 0));
			
			pIOnlineChar->DrawName(pMyData->GetMyCharUniqueID(), hdc);		

			SelectObject(hdc, hOldFont);
			GETSCREENSURFACE(this)->ReleaseDC(hdc);

			// robypark 2004/11/12 21:28
			// 공성전 유닛 체력 및 보급 게이지 그리기
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// 공성전 유닛 체력 및 보급 게이지 그리기
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(pMyData->GetMyCharUniqueID());

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}
		}	
		
		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// 상단 깃발을 찍는다.
			pIOnlineChar->DrawGuildFlag(pMyData->GetMyCharUniqueID());

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	
	}

	// 필드에서 건물 작업을 하고 있을때
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
				// 마을 간판
				clGrp.PutSpriteT(-1 + SignStartPos, 0, lpBoard->Header.Xsize, lpBoard->Header.Ysize, lpBoard->Image);

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

			// 마을 이름을 출력해준다.
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
		// 일반 상태일때
		// 미니맵 창이 활성화 되어 있다면 출력~~
		if(pOnlineMiniMap->IsActive())
			pOnlineMiniMap->Draw(GETSCREENSURFACE(this));

		pOnlineParty->Draw(GETSCREENSURFACE(this));

		pOnlineTradeBook->Draw( GETSCREENSURFACE(this) );

		// 경매접수창 
		pOnlineReception->Draw(GETSCREENSURFACE(this));

		// 대장장이
		pOnlineSmith->Draw(GETSCREENSURFACE(this));

		// robypark 2005/2/2 16:18
		// 공성 유닛 인터페이스 활성화시
		if ((pOnlinePannel->GetSiegeWarfareUnitUIHandle()->IsVisibleWindow())
			|| (pOnlinePannel->GetSiegeWarfareUnitUIHandle()->IsMinimizeWindow()))
		{
			pOnlinePannel->SetFlagVillage(FALSE, ONLINEPANNEL_DISABLEFLAG_CHARINFO/*용병창*/);
		}
		else
			pOnlinePannel->SetFlagVillage(FALSE, NULL);

		// 판넬을 그린다.
//		pOnlinePannel->SetFlagVillage(FALSE, NULL);
		pOnlinePannel->Draw(GETSCREENSURFACE(this));

		// 거래창이 활성화 되어 있따면
		if( pOnlineTrade->IsActive() )
		{	
			// 내 인벤토리에서 아이템이 드래그 중이라면 거래창을 먼저 찍어 주고 인벤토리를 찍어 준다.
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
			// 용병 관련 출력			
			m_pMercenary->Draw(GETSCREENSURFACE(this));

			// 좌판
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

		// 이모티콘을 출력한다.
		pOnlineImoticon->Draw(GETSCREENSURFACE(this));	

		// 화면으로 옮긴다.
		// 정보를 화면에 찍어준다.
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

					// 현재 Tile Buffer 사용률
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

					// 현재 선택한 개체 출력
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					switch(pMousePointInfo->GetPointKind())
					{
					case ON_MOUSE_POINT_CHAR:
						// 선택한 캐릭터 표시
						if(pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam())))
						{
							// Player인지

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

								// 길드원일 경우

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
									// 길드 깃발이 있는경우

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d, Flag : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								}
								else
								{
									// 길드 깃발이 없는경우

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()));
								}
							}
							else
							{
								// 길드원이 아닐경우

								sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\"", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()));
							}
						}
						else
						{
							// Monster인지

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

					// 원하는 자판의 Data를 얻어온다.
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

		// 경매창 
		pOnlineFieldAuction->Draw(GETSCREENSURFACE(this));

		// 한국어 버젼일때만 표기된다.
//		if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//		{
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// actdoll (2004/04/02 15:28) : = 이벤트 = 독도 퀘스트 진행 중인가 표시
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

				// 받은아이템 표시
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

		// 받은아이템 공지
		if(m_bReceivedItem)
		{
			if(PtInRect( &m_rtReceivedItem, pt ) == TRUE)
			{
				pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_ITEM_ARRIVED_DESCRIPT),IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
			}
		}

		// 한국어 버젼일때만 표기된다.
//		if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//		{
//			// actdoll (2004/04/02 15:28) : = 이벤트 = 독도 퀘스트 진행 중인가 공지
//			if(m_bEventDokdoQuestPlay)
//			{
//				if(PtInRect( &m_rtIconDokdoQuest, pt ) == TRUE)
//				{
//					pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_EVENT_PLAYING_DOKDO_QUEST),IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE );
//				}
//			}
//		}



		

		// 도움말 출력
		SI16 siDrawedHelpPosX = 0;
		SI16 siDrawedHelpPosY = 0;
		int helpx = pOnlineHelp->Show(GETSCREENSURFACE(this), &siDrawedHelpPosX, &siDrawedHelpPosY);

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			XSPR*		lpXSPR;

			// 현재 마우스가 캐릭터를 가르키고 있으면
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
				// 공성전 유닛일 경우 체력, 보급품 게이지 출력하기
				if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
				{
					// 공성전 유닛 체력 게이지만 출력
					XSPR	*psprGaugeHealth = NULL;

					// 사용자와 같은 상단인가?
					if (IsMemberOfSameGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
					}
					// 적대 상단원인가?
					else if (pIOnlineChar->GetIsHostilityGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
					}
					else	// 그 밖의 경우 안그린다.
						psprGaugeHealth = NULL;

					if (psprGaugeHealth)
					{
						// 그리기
						DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
												siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
												m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct + 1,
												psprGaugeHealth);
					}

					// 공성전 유닛 보급게이지를 출력해야 하는 경우라면
					if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods)
					{
						// 공성전 유닛 보급품 게이지 출력
						XSPR	*psprGaugeSupplyGoodsAmount = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

						if (psprGaugeSupplyGoodsAmount)
						{
							// 그리기
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

		// 메세지 박스 출력
		pOnlineMsgBox->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxUsePortal->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxError->Draw(GETSCREENSURFACE(this));

		// TIP 출력
		pOnlineTip->Draw(GETSCREENSURFACE(this));

		// Quest Play
		pOnlineQuestPlay->Draw(GETSCREENSURFACE(this));

		// Quest
		m_pQuestInterface->Draw(GETSCREENSURFACE(this));

//		//초보자 도움말
//		pOnlineBasicTip->Draw(GETSCREENSURFACE(this));

		//초보자 퀘스트.
//		pOnlineBasicQuest->Draw(GETSCREENSURFACE(this));
		
		
		// 아이템 획득 창을 보여준다. 
		pGame->pItemInfoWindow->Render(GETSCREENSURFACE(this));

		// 메신저 출력
		pOnlineMessenger->Draw(GETSCREENSURFACE(this));

		// 숫자판 드로우
		pOnlineNumPad->pNumberControl->Draw( GETSCREENSURFACE(this) );

		// 환경설정 드로우
		pOnlineSetting->Draw(GETSCREENSURFACE(this));

		//준석이형 추가부분 
		// 모든 윈도들을 드로우
//		pJSWndMgr->Draw(GETSCREENSURFACE(this));

		pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(this));

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// 마우스를 찍는다.
			pIOnlineMouse->Draw();			

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}
		
		// for debug



		

		UpdateScreen(GETSCREENSURFACE(this));
	}

	YetTempTickCount = GetTickCount() - TempTickCount;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터가 마을상에 있을때 호출하는 함수.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::VillagePoll(UI16 uiClientRet)
{
	m_siMsgBoxStatus = pOnlineMsgBox->Process();

	// 단축키를 처리한다.
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
//	설명	:	해제.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::Free()
{

	fYetLight					=	0.0f;
	m_GameTickCount             =   0;

	bCharMove = TRUE;

	// 마우스를 안보이게 해준다.
	ShowSystemCursor(FALSE);		

	// 캐릭터 인터페이스를 해제한다.
	if(pIOnlineChar)
	{
		pIOnlineChar->Free();

		delete pIOnlineChar;
		pIOnlineChar = NULL;
	}

	delete m_pMercenary;
	delete m_pChatMacro;
	delete m_pQuestInterface;

	// actdoll (031229)	: 초상 데이터 해제
	delete m_pPortrait;
	delete m_pNpcTalk;	

	g_FileLoadManager.Free();	// 조합형 캐릭터 Free()	

	m_cltIGuildClass.Free();	

	// actdoll (2004/11/03 17:19) : IME컨트롤러 해제
	OnlineInputStatusControl::Free();

	if(m_ReceivedItemAniSpr.Image)  {clGrp.FreeXspr(m_ReceivedItemAniSpr);  m_ReceivedItemAniSpr.Image  = NULL;}
//	if(m_Spr_Icon_DokdoQuest.Image)  {clGrp.FreeXspr(m_Spr_Icon_DokdoQuest);  m_Spr_Icon_DokdoQuest.Image  = NULL;}

	if(pOnlineLoadingData)
	{
		// actdoll (2004/06/29 12:57) : 누수 처리
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

	// Palette Manager 해제
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

	// Timer 해제
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
	// 캐릭터의 종류 인터페이스를 해제한다.
	if(pIOnlineCharKI)
	{
		pIOnlineCharKI->Free();

		delete pIOnlineCharKI;
		pIOnlineCharKI =	 NULL;
	}

	// 오브젝트 종류의 인터페이스를  해제한다.
	if(pIOnlineObjectKI)
	{
		pIOnlineObjectKI->Free();

		delete pIOnlineObjectKI;
		pIOnlineObjectKI = NULL;
	}

	// 마을 객체를 해제한다.
	if(pOnlineVillage)
	{
		delete pOnlineVillage;
		pOnlineVillage = NULL;
	}

	// 마을 설명 List 해제
	if(pOnlineVillageDescriptList)
	{
		delete pOnlineVillageDescriptList;
		pOnlineVillageDescriptList = NULL;
	}

	// 공지사항 List 해제
	if(pOnlineNoticeList)
	{
		delete pOnlineNoticeList;
		pOnlineNoticeList = NULL;
	}

	// 필드상 Effect
	if(pOnlineFieldEffect)
	{
		delete pOnlineFieldEffect;
		pOnlineFieldEffect = NULL;
	}

	// 길찾기 인공지능을 삭제한다.
	if(pclFindPath)
	{
		delete pclFindPath;
		pclFindPath = NULL;
	}
		
	// 이모티콘
	if(pOnlineImoticon)
	{
		pOnlineImoticon->Free();

		delete pOnlineImoticon;
		pOnlineImoticon = NULL;
	}

	// 나의 데이타를 해제한다.
	if(pMyData)
	{	
		pMyData->Free();

		delete pMyData;
		pMyData = NULL;
	}

	// robypark 2005/1/14 17:11
	// 주인공 캐릭터들의 정보 관리자 초기화
	if (m_pFieldHeroInfoMgr != NULL)
	{
		delete m_pFieldHeroInfoMgr;
		m_pFieldHeroInfoMgr = NULL;
	}

	// robypark 2005/1/31 12:55
	// 공성 유닛 관리자
	if (NULL != m_pSiegeUnitTableMgr)
	{
		delete m_pSiegeUnitTableMgr;
		m_pSiegeUnitTableMgr = NULL;
	}

	// 네트워크 연결을 해제한다.
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

	// 맵관리자를 해제한다.
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
		// Config 파일을 저장함.
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

	// 오브젝트의 메모리를 해제한다.
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
	//준석이형 추가부분 
	
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

	// 공성전 공격 모드 설정 변수
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
//	설명	:	방향을 구한다.
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
//	설명	:	프레임수를 구해온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	cltOnlineWorld::GetTotalFrame()
{
	return uiTotalFrame;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	백 서피스를 프라이머리 서피스에 갱신 시킨다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::UpdateScreen(LPDIRECTDRAWSURFACE7 pSurface, BOOL bSourceColorKey,BOOL bForceRedraw)
{
	DWORD					dwTrans;
	LPDIRECTDRAWCLIPPER		pClipper;
	HRESULT hResult =0;

	if(pSurface == NULL)	pSurface	=	clGrp.lpDDSBack;

	// actdoll (2004/11/03 17:28) : IME 컨트롤러 관련...
	//	안좋은 방법이지만 배치가 중구난방이라 출력코드를 여기로 잡을 수 밖에 없다.
	OnlineInputStatusControl::Draw( pSurface );

#ifdef	_DEBUG
#ifdef	_SHOW_INPUTSTATUS_
	// actdoll (2004/08/09 14:52) : [임시] IME 스테이터스 얻어오기
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

	//clGrp.lpDD7->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);	<<-- 정모가 삭제...2003/06/13 23:11

	/*
	if(pSurface == clGrp.lpDDSBack)
	{
		clGrp.lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	}
	else
	*/
	
	if(clGrp.lpDDSPrimary->GetClipper(&pClipper) == DDERR_NOCLIPPERATTACHED)
	{
		// 클리퍼가 부착이 되어있지 않다.		
		dwTrans	=	DDBLT_WAIT;
		
		if(bSourceColorKey == TRUE)			dwTrans	|=	DDBLT_KEYSRC;
		
		clGrp.lpDDSPrimary->Blt(NULL, pSurface, NULL, dwTrans, NULL);
		/*
		if(clGrp.BltFast(SURFACE_PRIMARY, 0, 0, pSurface, NULL, dwTrans) != DD_OK)
		clGrp.Error("BltFast 에러", "");
		*/
		clGrp.Error("BltFast Error", "");
	}
	else
	{	
		// 정모가 삭제... 2003/06/13 23:18
		/*GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );
		*/
		
		dwTrans	=	DDBLT_WAIT;
		
		if(bSourceColorKey == TRUE)			dwTrans	|=	DDBLT_KEYSRC;
		
        if (!WindowModeSwitch) {
			// 정모가 삭제... 2003/06/13 23:18
			/*RECT rc;
			GetWindowRect(pGame->Hwnd, &rc);*/
			hResult = clGrp.lpDDSPrimary->Blt( NULL, pSurface, NULL, dwTrans, NULL);
	
			// 서피스가 로스트된 것이면, 
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
//	설명	:	타일 좌표를 사용하여 경도와 위도를 얻어온다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	문자열로 방향을 가지고 숫자로 되어 있는 방향을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltOnlineWorld::GetDirection(CHAR *pszDirection)
{
	SI32	siDirection =	0;

	if(strstr(pszDirection, "EAST") != 0)	siDirection |= ON_EAST;
	if(strstr(pszDirection, "WEST") != 0)	siDirection |= ON_WEST;
	if(strstr(pszDirection, "NORTH") != 0)	siDirection |= ON_NORTH;
	if(strstr(pszDirection, "SOUTH") != 0)	siDirection |= ON_SOUTH;

	return siDirection;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	시스템 마우스를 보이게 해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::ShowSystemCursor(BOOL bShow)
{
	SI32 i = 0;

	if(bShow == TRUE)		
		while(i = ShowCursor(TRUE) < 0);
	else					
		while(i = ShowCursor(FALSE) >= 0);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	로그아웃을 하고 다음으로 이동할 메뉴를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::SetLogOut(SI32 siNextMenu, SI32 siNextPoll, BOOL bGoGameServer)
{
	siNextMenuAfterLogOut		=	siNextMenu;
	siNextPollAfterLogOut		=	siNextPoll;
	bWillGoGameServer			=	bGoGameServer;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	임진록2 Game 으로 가기 위하여 해제해야 할 것등을 해제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::FreeForGotoImjinGame()
{/*
	// 리소스를 해제한다.
	if(pOnlineResource)
	{
		pOnlineResource->Free();

		delete pOnlineResource;
		pOnlineResource	=	NULL;
	}

	// 맵의 메모리를 해제한다.
	if(pOnlineMap)	
	{
		pOnlineMap->Free();

		delete pOnlineMap;
		pOnlineMap	=	NULL;
	}	
*/
	// 캐릭터의 메모리를 해제한다.
	if(pIOnlineChar)
	{
//		pIOnlineChar->AllFree();
//		delete pIOnlineChar;
//		pIOnlineChar		=	NULL;
	}

	// 오브젝트의 메모리를 해제한다.
	if(pIOnlineObject)
	{
		pIOnlineObject->Free();

		delete pIOnlineObject;
		pIOnlineObject		=	NULL;
	}

	// 배경 음악을 해제한다.
	if(g_pSoundManager)
		g_pSoundManager->StopSound( BackgroundMusic );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	cltOnlineWorld::DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// 옵션이 있다면, 
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	cltOnlineWorld::DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// 옵션이 있다면, 
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	cltOnlineWorld::DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// 옵션이 있다면, 
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	숫자를 출력해준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 온라인 게임중인가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	_clGame::IsOnlineWorld()
{	
	if(pOnlineWorld != NULL)
		return TRUE;
	else
		return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	버젼을 화면에 찍어준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::DrawVersion(HDC hdc, SI32 siX, SI32 siY)
{
	SetTextColor(hdc, PaletteWhiteColor);
	TextOut(hdc, siX, siY, szOnlineVersion, strlen(szOnlineVersion));
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게임 종료

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::ExitOnline()
{
	bExitOnlineGame = TRUE;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 배경음악의 아이디를 리턴
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	cltOnlineWorld::GetSoundID()
{
	return BackgroundMusic;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	로딩중에 텍스트를 출력한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	평화 모드, 전투 모드 변환
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::SetPeace(BOOL bFlag)		// 평화 모드 -> 전투 모드
{	
	bPeace = bFlag;
	
	ToggleAttackMode(pMyData->GetMyCharUniqueID(), !bPeace); 

	pOnlineClient->SendAttackMode(!bPeace);
}						

VOID	cltOnlineWorld::ToggleAttackMode(UI16 uiAccount, BOOL bAttackMode)
{
	UI16	Equip[WEAR_ITEM_NUM];
	
	// 장착 정보를 얻어 온다.
	pIOnlineChar->GetEquip(uiAccount, Equip);	
	pIOnlineChar->SetAttackMode(uiAccount, bAttackMode);
	pIOnlineChar->SetEquipItem(uiAccount, Equip);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	로딩중에 프로그래스바를 출력한다.			hojae_append 2002.01.03
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID cltOnlineWorld::ScrollLoading( UI32 uiLoadPercent )
{
	if(clGrp.LockSurface(clGrp.lpDDSBack) == TRUE)
	{
		clGrp.PutSpriteT(0, 0, pOnlineLoadingData->BackImage.Header.Xsize, pOnlineLoadingData->BackImage.Header.Ysize, pOnlineLoadingData->BackImage.Image);
		clGrp.PutSpriteT(0, 22, pOnlineLoadingData->BackOverImage.Header.Xsize, pOnlineLoadingData->BackOverImage.Header.Ysize, pOnlineLoadingData->BackOverImage.Image);
		clGrp.PutSpriteT(553, 22, pOnlineLoadingData->AniLogoImage.Header.Xsize , pOnlineLoadingData->AniLogoImage.Header.Ysize, &pOnlineLoadingData->AniLogoImage.Image[pOnlineLoadingData->AniLogoImage.Header.Start[pOnlineLoadingData->uiFrame]]);
//		clGrp.PutSpriteT(522, 19, pOnlineLoadingData->AniLogoImage.Header.Xsize , pOnlineLoadingData->AniLogoImage.Header.Ysize, &pOnlineLoadingData->AniLogoImage.Image[pOnlineLoadingData->AniLogoImage.Header.Start[pOnlineLoadingData->uiFrame]]);

		// actdoll (2004/09/01 13:36) : 회사 및 이용가능 연령범위에 대한 로고
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

			// actdoll (2004/09/02 4:23) : 라이선스 텍스트
			char	*pszLicense;
			if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_GAMANIA;
			else														pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_BOLEHCOM;
			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_ITEM));
			pOnlineMegaText->DrawText( hDC, 286, 564, pszLicense, RGB(250, 250, 250) );


			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_CHAT));
			pOnlineMegaText->DrawText(hDC, 290, 579, pOnlineText->Get(ON_TEXT_LOADING_NOTICE), RGB(250, 250, 250));
			// actdoll (2004/03/03 19:02) : 디버그 모드에서는 디버그 버젼임을 표기한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부에서 최고 투자 상품을 구해 준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::FindTrade()
{
	bFindTrade	=	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부가 최고 투자 상품을 찾는 중인가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineWorld::IsFindNow()
{
	return bFindTrade;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	단축키
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::SetShortKey( BOOL bFlag )
{
	if(bFlag)
		ShortKey	=	bFlag;
	else if(!pOnlineMessenger->IsAction() || pOnlineMessenger->IsMini())
		ShortKey	=	bFlag;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	장부
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::FindTradeBook()
{
	// 장부 검색

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
	// 클라이언트 자체 스피드핵 체크
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
			// Type이 0이면 서버에서 스피드핵 처리를 함.
			if(pOnlineClient) pOnlineClient->SendGameTickCount(0, 0);
			return TRUE;
		}
	}

	// 30초마다 Server에게 TickCount 메세지
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
				// 스핵을 썼을 경우 siResult1 값이 3만이 넘어가는 경우도 있다. 서버 부하를 줄이기 위해 최대 10번 까지만...
				for(int i = 0; i < min(siResult1, 10); i++)
				{
					if(pOnlineClient) pOnlineClient->SendGameTickCount(m_GameTickCount + SERVERINFO_TICKCOUNT * (i+1), 1);
				}
				m_GameTickCount += SERVERINFO_TICKCOUNT * siResult1;
			}
		}
	}
	
	// 30초마다 Server에게 Time 메세지
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

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	// List에 없으면 기본적인 국가 코드를 이용한다.
	lpVillageHeader = pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(VillageCode);
	if(lpVillageHeader)
	{
		return (SI16)lpVillageHeader->uiNation;
	}

	// 에러나면 무조건 코리아~~~!!!
	return ON_COUNTRY_NONE;
}

SI32	cltOnlineWorld::GetVillageGuildFlag(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return -1;
}

float	cltOnlineWorld::GetVillageDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0.0f;
}

BOOL	cltOnlineWorld::GetVillageEnemyGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 자신의 상단이 없으면 무조건 FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// 현재 국가 코드가 List에 들어있는지 검사
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
			// 없으면 Server에 Data 요청
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

// robypark 2004/12/22 11:50
// 오늘 공성하는 마을인지 검사
BOOL	cltOnlineWorld::GetVillageWarDayGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 자신의 상단이 없으면 무조건 FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// 현재 국가 코드가 List에 들어있는지 검사
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				// 이 마을과 공성을 하는가?
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_WAR)
					return TRUE;

				else 
					return FALSE;
			}
		}
		else
		{
			// 없으면 Server에 Data 요청
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

// robypark 2004/10/28 17:29
// 마을이 캐릭터 소속 상단의 마을인지 검사
// [IN] UI16 VillageCode: 마을 코드
BOOL	cltOnlineWorld::GetVillageOurGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 자신의 상단이 없으면 무조건 FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// 현재 국가 코드가 List에 들어있는지 검사
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				// 자신이 소속한 상단 소유의 마을인지 검사
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_MINE)
					return TRUE;
				else 
					return FALSE;
			}
		}
		else
		{
			// 없으면 Server에 Data 요청
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

VDEF	cltOnlineWorld::GetVillageDefenceNowDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

VDEF	cltOnlineWorld::GetVillageDefenceMaxDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// 마을에 주둔한 현재 수비병 부대 수 얻기
SI32 cltOnlineWorld::GetVillageCurrentGarrisonCount(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// 마을에 주둔한 최대 수비병 부대 수 얻기
SI32 cltOnlineWorld::GetVillageMaximumGarrisonCount(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// 마을의 반격 공격 형태 얻기. 일반, 돌, 기름
SI16 cltOnlineWorld::GetvillageCounterAttackType(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// 현재 국가 코드가 List에 들어있는지 검사

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
		// 없으면 Server에 Data 요청
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
	// 일정시간동안 메세지가 오지 않았으면 에러 출력
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get War Field Map
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32 cltOnlineWorld::GetWarFieldMap(void)
{
	if(pOnlineMap)
		return pOnlineMap->GetWarFieldMap();
	else
		return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 FieldMap이 PK가 가능한지 검사

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL cltOnlineWorld::CheckPKFieldMap(void)
{
	if(pOnlineMap)
		return pOnlineMap->CheckPKFieldMap();
	else
		return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을안인지 Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


// 아이템 이름을 얻어 온다.
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


// 온라인 상단 메시지관련 클래스의 포인터를 얻어온다.
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

		// 패킷 읽기

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

	// 캐릭터 명령 버퍼를 초기화 한다.
	pOnlineCharUI->Init();

	// 캐릭터를 대기 상태로 둔다		
	if(pMyData->GetMyCharUniqueID() != 0)
	{
		pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);		
	}
	else
	{
		// 나의 캐릭터가 없다.
		pOnlineCharUI->SetAction(FALSE);				// UI 클래스를 disable 시킨다.
	}

	// 필드로 이동해야 한다.
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
	// 공성전 중에 들려지는 배경음악 추가
	// 공성전 시간이며, 
	if (IsInTimeSiegeWarfare())
	{
		// 공성전 유닛이거나, 수성 인터페이스 상에 있다면
		if (TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()) 
			|| (TRUE == pMyData->IsInVillageDefenceInterface()))
		{
			// 현재 배경음악이 연주 중이며,
			if (TRUE == GetBgmMgr()->IsPlaying())
			{
				// 현재 연주 중인 배경음악이 공성전 배경음악이 아니라면
				if (OGGBGM_FILENAME_SIEGEBATTLEMUSIC != GetBgmMgr()->GetCurrentBgmIndex())
				{
					//clGrp.Error("배경 음악 연주", "배경음악 연주 중 입니다. 공성전 배경음악 연주합니다.");
					// 공성 배경 음악 연주
					GetBgmMgr()->Play(OGGBGM_FILENAME_SIEGEBATTLEMUSIC, TRUE);
				}
			}
			else	// 연주 중이 아니라면, 
			{
				//clGrp.Error("배경 음악 연주", "공성전 배경음악 연주합니다.");

				// 공성 배경 음악 연주
				GetBgmMgr()->Play(OGGBGM_FILENAME_SIEGEBATTLEMUSIC, TRUE);
			}

			// 공성전 배경음악 연주 중에는 국가별 배경음악을 연주하지 않는다.
			return;
		}
	}

	if(siTempNation==ON_COUNTRY_NONE) return;

	// 공성전 배경음악 연주가 필요없는 경우 원래 거상 배경음악 연주를 하도록 한다.
	// 현재 배경음악이 연주 중이며,
	if (TRUE == GetBgmMgr()->IsPlaying())
	{
		// 현재 연주 중인 배경음악이 공성전 배경음악이라면
		if (OGGBGM_FILENAME_SIEGEBATTLEMUSIC == GetBgmMgr()->GetCurrentBgmIndex())
		{
			// 강제로 배경음악을 바꾸도록 한다.
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
	// 실행중인 프로그램 목록을 구한다.
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
		pOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "마작 매크로 검출", "siResult : %d", siResult);
	}*/
}

BOOL	cltOnlineWorld::IsNight()
{
	SOnlineTime				NowTime;
	
	// 현재 시간을 얻어온다.
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
// 공성전 유닛이 적 상단 공성전 유닛에 대한 공격이 가능한지 검사 함수
// UI16 uiAttackerUniqueID: 공격자의 UniqueID
// UI16 uiTargetUniqueID: 피격자의 UniqueID
BOOL cltOnlineWorld::SiegeWarfare_CanAttackUnit(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID)
{
	// 존재하는 목표 캐릭터인가?
	if (FALSE == pIOnlineChar->IsExistUniqueID(uiTargetUniqueID))
	{
		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// 공성 시간인지 비교
	if(FALSE == IsInTimeSiegeWarfare())
	{
		char szErrorMsg[256];

		sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_SIEGEWARFARE_TIME_IS), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END); 
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, szErrorMsg);

		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

		return FALSE;
	}

	// 본 맵이 아닌 경우 불가능
	if (pOnlineMap->m_MapIndex != 0)
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));

		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}
/*
	// 공격자 캐릭터가 살아있는가?
	if (0 >= pIOnlineChar->GetSiegeWarfare_Hp(uiAttackerUniqueID))
	{
		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}

	// 피격자 캐릭터가 살아있는가?
	if (0 >= pIOnlineChar->GetSiegeWarfare_Hp(uiTargetUniqueID))
	{
		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}

	// 공격자 캐릭터가 수성 인터페이스에 있는가?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
	{
		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}
*/
	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// 공격자 캐릭터가 공성전 유닛 공격이 가능한 캐릭터인가?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "공성전 유닛을 공격하지 못하는 유닛이다.");

		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	/*
	// 피격자 캐릭터가 수성 인터페이스에 있는가?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
	{
		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}
	*/

	// 피격자가 공성전 유닛이 아니라며 공격 불가능
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "공성전 유닛이 아니다.");

		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// 적대 상단인지 검사
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// 공격자 위치
	SI16 siPosition_AttakerX, siPosition_AttakerY;
	// 피격자 위치
	SI16 siPosition_TargetX, siPosition_TargetY;

	// 공격자 캐릭터가 원거리 유닛일 경우, 즉 근거리 공격이 불가능한 경우 검사
	// 원거리 유닛 판단은 캐릭터 속성에 ATB_MINRANGE가 있을 경우 원거리로 판단한다.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// 공격자 및 피격자 캐릭터 위치 얻기
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

		// 피격자 캐릭터와 거리가 최소거리 안에 있는지 검사
		if (ManContactvillage(siPosition_TargetX, siPosition_TargetY, 1, 1, siPosition_AttakerX, siPosition_AttakerY, KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
//			pIOnlineChar->SetSpeech(uiAttackerUniqueID, "적과의 거리가 가까워서 공격이 불가능하다.");

			// 공격 모드 설정
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
	}
	else
	{
		/*
		// 근거리 자동 공격 횟수 제한
		if (m_sSiegeWarfare_AttackMode.siAutoAttackTimes >= 3)
		{
			// 공격 모드 설정
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
		*/
	}


	/*
	// 보급 게이지가 0일 경우 공격 불가능
	if (0 >= pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiAttackerUniqueID))
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_SPEECH_CANNOT_ATTACK_RANOUT_SUPPLYGOODS));

		// 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;

		return FALSE;
	}
	*/

	// 공격 지연시간이 경과되었는지 검사
	if (pIOnlineChar->GetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "공격 지연시간이 경과하지 않았다.");

		return FALSE;
	}

	// 공격 지연시간이 경과하였으므로, 공격하는 현재 시간을 설정
	pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID, KI[uiKindInfoIdx_Attacker].GetAttackDelay(0));

	// 공격 가능
	return TRUE;
}

// robypark 2005/1/8 13:16
// 공성전 유닛이 공격이 가능한지 검사 함수
// UI16 uiTargetUniqueID: 피격자의 UniqueID
BOOL	cltOnlineWorld::SiegeWarfare_CheckCanAttackUnit(UI16 uiTargetUniqueID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// 공격자 캐릭터가 공성전 유닛 공격이 가능한 캐릭터인가?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
		return FALSE;
	}
/*
	// 피격자 캐릭터가 수성 인터페이스에 있는가?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
		return FALSE;
*/

	// 피격자가 공성전 유닛이 아니라며 공격 불가능
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
		return FALSE;
	}

	// 적대 상단인지 검사
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		return FALSE;
	}

	// 공격 가능
	return TRUE;
}

// robypark 2005/1/12 11:3
// 공성전 유닛이 공격이 가능한지 검사하여 마우스 커서 설정을 위한 마우스 타입 얻기
// UI16 uiTargetUniqueID: 피격자의 UniqueID
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanAttackUnit(UI16 uiTargetUniqueID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

/*
	// 피격자 캐릭터가 수성 인터페이스에 있는가?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;
*/
	// 사용자 캐릭터가 수성 인터페이스에 있는가?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	BOOL bRangeAttacker = FALSE;	// 원거리 공격 유닛인가?

	// 공격자 캐릭터가 원거리 유닛일 경우, 즉 근거리 공격이 불가능한 경우 검사
	// 원거리 유닛 판단은 캐릭터 속성에 ATB_MINRANGE가 있을 경우 원거리로 판단한다.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		bRangeAttacker = TRUE;	// 원거리 공격 유닛
	}

	// 공격자 캐릭터가 공성전 유닛 공격이 가능한 캐릭터인가?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;
	}

	/*
	// 피격자가 공성전 유닛이 아니라며 공격 불가능
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;

		// 공격자가 원거리 공격 유닛인가?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// 원거리 공격 불가능
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// 근거리 공격 불가능
	}
	*/

	// 적대 상단인지 검사
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;
		// 공격자가 원거리 공격 유닛인가?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// 원거리 공격 불가능
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// 근거리 공격 불가능
	}

	// 공격자가 원거리 공격 유닛인가?
	if (bRangeAttacker)
	{
		// 공격자 위치
		SI16 siPosition_AttakerX, siPosition_AttakerY;
		// 피격자 위치
		SI16 siPosition_TargetX, siPosition_TargetY;

		// 공격자 및 피격자 캐릭터 위치 얻기
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

		// 피격자 캐릭터와 거리가 최소거리 안에 있는지 검사
		if (ManContactvillage(siPosition_TargetX, siPosition_TargetY, 1, 1, siPosition_AttakerX, siPosition_AttakerY, KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// 원거리 공격 불가능
		}

		return ON_MOUSE_BATTLE_SUB_LONGRANGE;	// 원거리 공격 가능
	}

	return ON_MOUSE_BATTLE_SUB_SHORTRANGE;	// 근거리 공격 가능
}

// robypark 2004/10/28 16:21
// 공성전 유닛이 공격이 마을을 공격 가능한지 검사
// [IN]  UI16 uiAttackerUniqueID: 공격자의 UniqueID
// [IN]  UI32 siObjectID		: 공격 대상 오브젝트(마을) ID
// [OUT] BOOL &bRangeAttacker	: 공격자가 원거리 공격 유닛인가?
BOOL cltOnlineWorld::SiegeWarfare_CanAttackVillage(UI16 uiAttackerUniqueID, SI32 siObjectID, BOOL &bRangeAttacker)
{
	// 본 맵이 아닌 경우 불가능
	if (pOnlineMap->m_MapIndex != 0)
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));
		// 공성전 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// 공격자 위치
	SI16 siPosition_AttakerX, siPosition_AttakerY;

	// 마을 위치, 크기
	SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

	// 공격자는 근거리 공격 유닛이다.
	bRangeAttacker = FALSE;

	// 공격자가 공성전 유닛이 아니라며 공격 불가능
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "공성전 유닛이 아니다.");
		// 공성전 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// 공격자 캐릭터가 마을 공격이 가능한 캐릭터인가?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "마을 공격이 불가능한 공성전 유닛이다.");
		// 공성전 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// 공격자 캐릭터가 원거리 유닛일 경우, 즉 근거리 공격이 불가능한 경우 검사
	// 원거리 유닛 판단은 캐릭터 속성에 ATB_MINRANGE가 있을 경우 원거리로 판단한다.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// 공격자는 원거리 공격 유닛이다.
		bRangeAttacker = TRUE;

		// 공격자 및 피격자 캐릭터 위치 얻기
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		
		// 오브젝트의 본 위치를 구한다.
		pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

		// 오브젝트의 사이즈를 구한다.
		pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

		// 마을과 거리가 최소거리 안에 있는지 검사
		if(ManContactvillage(siVillageX - siVillageXsize / 2, siVillageY - siVillageYsize / 2, // position village
								siVillageXsize, siVillageYsize, // range
								siPosition_AttakerX, siPosition_AttakerY,	// position attacker
								KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
//			pIOnlineChar->SetSpeech(uiAttackerUniqueID, "마을 공격을 하기에는 거리가 너무 가깝다.");
			// 공성전 공격 모드 설정
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}

	}
	else
	{
		/*
		// 근접 공격 유닛 자동 공격 횟수 제한
		if (m_sSiegeWarfare_AttackMode.siAutoAttackTimes >= 3)
		{
			// 공성전 공격 모드 설정
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
		*/
	}

	// 공성하는 날의 마을인가?
	if (FALSE == GetVillageWarDayGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))
	{
		// 공성전 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	/*
	// 보급 게이지가 0일 경우 공격 불가능
	if (0 >= pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiAttackerUniqueID))
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_SPEECH_CANNOT_ATTACK_RANOUT_SUPPLYGOODS));
		// 공성전 공격 모드 설정
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		return FALSE;
	}
	*/

	// 공격 지연시간이 경과되었는지 검사
	if (pIOnlineChar->GetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "공격 지연시간이 경과하지 않았다.");
		return FALSE;
	}

	// 공격 지연시간이 경과하였으므로, 공격하는 현재 시간을 설정
	pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID, KI[uiKindInfoIdx_Attacker].GetAttackDelay(0));

	// 공격 가능
	return TRUE;
}

// robypark 2005/1/8 13:16
// 공성전 유닛이 공격이 마을을 공격 가능한지 검사
BOOL	cltOnlineWorld::SiegeWarfare_CheckCanAttackVillage(void)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// 공격자가 공성전 유닛이 아니라며 공격 불가능
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
		return FALSE;
	}

	// 공격자 캐릭터가 마을 공격이 가능한 캐릭터인가?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
		return FALSE;
	}

	// 공격 가능
	return TRUE;
}

// robypark 2005/1/12 11:3
// 공성전 유닛이 공격이 마을을 공격 가능한지 검사하여 마우스 커서 설정을 위한 마우스 타입 얻기
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanAttackVillage(SI32 siObjectID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// 공격자가 공성전 유닛이 아니라며 공격 불가능
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;	// 공격 불가능
	}

	// 사용자 캐릭터가 수성 인터페이스에 있는가?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;	// 공격 불가능
	}

	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// 공격자는 원거리 공격 유닛인가?
	BOOL bRangeAttacker = FALSE;

	// 공격자 캐릭터가 원거리 유닛일 경우, 즉 근거리 공격이 불가능한 경우 검사
	// 원거리 유닛 판단은 캐릭터 속성에 ATB_MINRANGE가 있을 경우 원거리로 판단한다.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// 공격자는 원거리 공격 유닛이다.
		bRangeAttacker = TRUE;
	}

	// 공격자 캐릭터가 마을 공격이 가능한 캐릭터인가?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
		// 공격자가 원거리 공격 유닛인가?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// 원거리 공격 불가능
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// 근거리 공격 불가능
	}

	// 공격자가 원거리 공격 유닛인가?
	if (bRangeAttacker)
	{
		// 공격자 위치
		SI16 siPosition_AttakerX, siPosition_AttakerY;

		// 마을 위치, 크기
		SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

		// 공격자 및 피격자 캐릭터 위치 얻기
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		
		// 오브젝트의 본 위치를 구한다.
		pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

		// 오브젝트의 사이즈를 구한다.
		pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

		// 마을과 거리가 최소거리 안에 있는지 검사
		if(ManContactvillage(siVillageX - siVillageXsize / 2, siVillageY - siVillageYsize / 2, // position village
								siVillageXsize, siVillageYsize, // range
								siPosition_AttakerX, siPosition_AttakerY,	// position attacker
								KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// 원거리 공격 불가능
		}

		return ON_MOUSE_BATTLE_SUB_LONGRANGE;	// 원거리 공격 가능
	}

	return ON_MOUSE_BATTLE_SUB_SHORTRANGE;	// 근거리 공격 가능
}

// robypark 2004/11/1 16:20
// 자신의 주변에 대장 유닛이 있는지 검사
// return value : 공성전 유닛의 공격 타입.
//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - 대장효과(공격력 업)에 영향을 받는다.
//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - 대장 공격이다.
//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - 일반 형태의 공격을 한다.
//				-1									   - 유효하지 않은 공격이다.
SI16 cltOnlineWorld::SiegeWarfare_IsInAreaGeneralEffect(void)
{
	// 자신의 캐릭터 Unique ID 얻기
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// 자신의 캐릭터가 공성전 유닛이 아닌 경우
	SI32 siKindImjinIdx_Attacker = pIOnlineChar->GetImjinKind(uiMyCharacterUniqueID);
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiMyCharacterUniqueID))
		return -1;

	// 공성전 보급 수레일 경우
	if (KI[siKindImjinIdx_Attacker].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		return -1;

	// 자신의 캐릭터 좌표
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// 대장은 대장효과(공격력 업) 적용안됨
	if(pIOnlineChar->GetCaptain(uiMyCharacterUniqueID))
	{
		// 공격 타입 설정
		// 대장의 공격이다.
		return ON_ATTACK_TYPE_CAPTAIN;
	}

	// 자신의 위치 찾기
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// 검색 범위
	SI16 ssTileX, ssTileY;

	// 5 X 5 타일이내에 대장이 있는지 찾기
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY++)
		{
			// 검색할 타일 계산
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// 맵에 설정된 캐릭터 ID 찾기
			UI16 uiCharacterID = pOnlineMap->GetCharID(siTargetX, siTargetY);

			// 해당 위치에 캐릭터가 존재
			if (0 != uiCharacterID)
			{
				// 찾은 캐릭터의 Unique ID 얻기
				UI16 uiCharacterUniqueID = pIOnlineChar->GetUniqueID(uiCharacterID);

				// 공성전 유닛인지 검사
				if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiCharacterUniqueID)) && (uiMyCharacterUniqueID != uiCharacterUniqueID))
				{
					// 대방이거나 행수 캐릭터인지 검사. 즉, 대장 유닛인지 검사, 수성 인터페이스 상에 있지 않다.
					if ((pIOnlineChar->GetCaptain(uiCharacterUniqueID)) && (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiCharacterUniqueID)))
					{
						// 같은 상단인지 검사
						if (IsMemberOfSameGuild(uiCharacterUniqueID))
						{
							// 공격 타입 설정
							// 대장효과(공격력 업) 범위 안에 존재한다.	
							return ON_ATTACK_TYPE_NEARCAPTAIN;
						}
					}
				}
			}	// if (0 != uiCharacterID)
		}	// for (ssTileY = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY++)
	}	// for (ssTileX = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX++)

	// 대장효과(공격력 업) 적용 받지 않는다.
	return ON_ATTACK_TYPE_NORMAL;
}

// robypark 2004/11/8 16:23
// 특정 캐릭터의 주변에 대장 유닛이 있는지 검사
// [IN] UI16 uiUniqueID: 비교하려는 캐릭터 Unique ID
// return value : 공성전 유닛의 공격 타입.
//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - 대장효과(공격력 업)에 영향을 받는다.
//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - 대장 공격이다.
//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - 일반 형태의 공격을 한다.
//				-1									   - 유효하지 않은 공격이다.
SI16 cltOnlineWorld::SiegeWarfare_IsInAreaGeneralEffect(UI16 uiUniqueID)
{
	// 공성 시간인지 비교
	if(FALSE == IsInTimeSiegeWarfare())
	{
		return -1;
	}

	// 본 맵이 아닌 경우 불가능
	if (pOnlineMap->m_MapIndex != 0)
		return -1;

	// 캐릭터가 공성전 유닛이 아닌 경우
	SI32 siKindImjinIdx_Attacker = pIOnlineChar->GetImjinKind(uiUniqueID);
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID))
		return -1;

	// 공성전 보급 수레일 경우
	if (KI[siKindImjinIdx_Attacker].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		return -1;

	// 캐릭터 좌표
	SI16 siCharacterPositionX, siCharacterPositionY;

	// 대장은 대장효과(공격력 업) 적용안됨
	if(pIOnlineChar->GetCaptain(uiUniqueID))
	{
		// 공격 타입 설정
		// 대장의 공격이다.
		return ON_ATTACK_TYPE_CAPTAIN;
	}

	// 자신의 위치 찾기
	pIOnlineChar->GetPos(uiUniqueID, &siCharacterPositionX, &siCharacterPositionY);

	// 검색 범위
	SI16 ssTileX, ssTileY;

	// 5 X 5 타일이내에 대장이 있는지 찾기
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY++)
		{
			// 검색할 타일 계산
			SI16 siTargetX = siCharacterPositionX + ssTileX;
			SI16 siTargetY = siCharacterPositionY + ssTileY;

			// 맵에 설정된 캐릭터 ID 찾기
			UI16 uiCharacterID = pOnlineMap->GetCharID(siTargetX, siTargetY);

			// 해당 위치에 캐릭터가 존재
			if (0 != uiCharacterID)
			{
				// 찾은 캐릭터의 Unique ID 얻기
				UI16 uiCharacterUniqueID = pIOnlineChar->GetUniqueID(uiCharacterID);

				// 공성전 유닛인지 검사
				if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiCharacterUniqueID)) && (uiUniqueID != uiCharacterUniqueID))
				{
					// 대방이거나 행수 캐릭터인지 검사. 즉, 대장 유닛인지 검사, 수성 인터페이스 상에 있지 않다.
					if ((pIOnlineChar->GetCaptain(uiCharacterUniqueID)) && (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiCharacterUniqueID)))
					{
						// 같은 상단인지 검사
						if (IsMemberOfSameGuild(uiUniqueID, uiCharacterUniqueID))
						{
							// 공격 타입 설정
							// 대장효과(공격력 업) 범위 안에 존재한다.	
							return ON_ATTACK_TYPE_NEARCAPTAIN;
						}
					}	// if ((pIOnlineChar->GetGuildInGrade(uiCharacterUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) || (pIOnlineChar->GetGuildInGrade(uiCharacterUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)))
				}	// if (TRUE == KI[TransKindOnlineToImjin(pIOnlineChar->GetKind(uiCharacterUniqueID))].IsKindAtb(KINDATB_SIEGE_WARFARE_UNIT))
			}	// if (0 != uiCharacterID)
		}	// for (ssTileY = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY++)
	}	// for (ssTileX = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX++)

	// 대장효과(공격력 업) 적용 받지 않는다.
	return ON_ATTACK_TYPE_NORMAL;
}

// robypark 2004/11/1 16:22
// 공성전 유닛으로 변신하고자 할 때 자신 주변에 적대상단 마을의 범위 안에 있는지 검사
// Return Value: TRUE  - 적 상단 마을 범위 안에 캐릭터가 존재하거나 본 맵이 아니다.
//				 FALSE - 변신 가능한 곳에 캐릭터가 존재한다.
BOOL cltOnlineWorld::SiegeWarfare_IsInAreaDisabledTransform(void)
{
	// 공성 시간인지 비교
	if(FALSE == IsInTimeSiegeWarfare())
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_SIEGEWARFARE_TIME_IS), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END); 
		return TRUE;
	}

	// 전투 대기중
	if(pMyData->IsWaitBattleResponse())
		return TRUE;

	// 마을에 들어가는 중이거나, 마을 들어가기 위한 대기 중이라면
	if ((pMyData->IsGotoVillage()) && (pMyData->IsWaitGotoVillageResponse() == TRUE))
		return TRUE;

	// 본 맵이 아닌 경우 변신 불가능
	if (pOnlineMap->m_MapIndex != 0)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));
		return TRUE;
	}

	// 캐릭터가 마을 안에 있는 경우
	if (pOnlineVillage != NULL)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_VILLAGE));
		return TRUE;
	}

	// 캐릭터가 수성 인터페이스 안에 있는 경우
	if (pMyData->IsInVillageDefenceInterface())
		return TRUE;

	// 상단이 없는 경우
	if (NULL == pMyData->GetGuildName())
		return TRUE;

	// 이미 공성전 유닛일 경우 공성 유닛으로 재차 변신할 수 없다.
	if (NULL != m_pSiegeUnitTableMgr->GetSiegeUnitTable(pMyData->GetMyCharKind()))
		return TRUE;

	// 자신이 주변의 적대상단 마을의 범위 안에 있는지 검사
	if (TRUE == SiegeWarfare_IsInAreaHostileVillage())
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "",
								pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_NEAR_HOSTILITY_VILLAGE));

		// 변신 불가능
		return TRUE;
	}

	// 변신 가능
	return FALSE;
}

// robypark 2005/1/13 21:39
// 자신 주변에 적대상단 마을의 범위 안에 있는지 검사
// Return Value: TRUE  - 적 상단 마을 범위 안에 캐릭터가 존재.
//				 FALSE - 근처에 적대 상단이 없는 곳에 캐릭터가 존재한다.
BOOL	cltOnlineWorld::SiegeWarfare_IsInAreaHostileVillage(void)
{
	// 자신의 캐릭터 Unique ID 얻기
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// 자신의 캐릭터 위치
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// 자신의 위치 찾기
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// 검색 범위
	SI16 ssTileX, ssTileY;

	// 21 X 21 타일이내에 적 상단 소속 마을이 있는지 찾기
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY++)
		{
			// 검색할 타일 계산
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// 해당 좌표의 맵에 있는 오브젝트ID 얻기
			SI16 siObjectID = pOnlineMap->GetObjectID(siTargetX, siTargetY);

			// 유효한가?
			if (siObjectID != 0)
			{
				// 오브젝트가 마을인지 알아본다.
				if (pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					// 마을 코드를 얻어온다.
					UI16 uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

					// 유효한 마을 코드인지?
					if (0 != uiVillageCode)
					{
						// 적 상단(공성하는 날) 마을인지?
						if (GetVillageWarDayGuild(uiVillageCode))
						{
							// 적 상단 마을 범위안에 있다.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// 적대 상단 마을 범위 밖
	return FALSE;
}

// robypark 2005/1/13 21:39
// 자신이 주변의 아군상단 마을의 범위 안에 있는지 검사
// Return Value: TRUE  - 아군 상단 마을 범위 안에 캐릭터가 존재.
//				 FALSE - 근처에 아군 상단이 없는 곳에 캐릭터가 존재한다.
BOOL	cltOnlineWorld::SiegeWarfare_IsInAreaOurVillage(void)
{
	// 자신의 캐릭터 Unique ID 얻기
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// 자신의 캐릭터 위치
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// 자신의 위치 찾기
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// 검색 범위
	SI16 ssTileX, ssTileY;

	// 21 X 21 타일이내에 아군 상단 소속 마을이 있는지 찾기
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY++)
		{
			// 검색할 타일 계산
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// 해당 좌표의 맵에 있는 오브젝트ID 얻기
			SI16 siObjectID = pOnlineMap->GetObjectID(siTargetX, siTargetY);

			// 유효한가?
			if (siObjectID != 0)
			{
				// 오브젝트가 마을인지 알아본다.
				if (pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					// 마을 코드를 얻어온다.
					UI16 uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

					// 유효한 마을 코드인지?
					if (0 != uiVillageCode)
					{
						// 아군 상단 마을인지?
						if (GetVillageOurGuild(uiVillageCode))
						{
							// 아군 상단 마을 범위안에 있다.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// 아군 상단 마을 범위 밖.
	return FALSE;
}

// robypark 2005/1/13 21:45
// 공성에 방해되는 일반 유저들을 각국 초보자 수련장으로 워프시킬 수 있는지 검사
BOOL	cltOnlineWorld::SiegeWarfare_CheckAbledWarfRefugee(UI16 uiDestUniqueID)
{
	// 공성전 시간이라면
	if (IsInTimeSiegeWarfare())
	{
		if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()))		// 사용자가 공성전 유닛이라면
			&& ( FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiDestUniqueID) )			// 대상이 공성전 유닛이 아니며,
			&& ( TRUE == pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind(uiDestUniqueID) ) )	// 대상이 플레이어이며,
			&& ( FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiDestUniqueID) ) 	// 대상이 수성 인터페이스에 있는 캐릭터가 아니며,
			&& ( FALSE == pIOnlineChar->GetBattleFlag(uiDestUniqueID) )	)				// 대상이 전투 대기중인 캐릭터가 아니며,
		{
			// 사용자 상단내 직급이 대방, 대행수, 행수인 경우
			if ((pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				|| (pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
				|| (pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)))
			{
				// 같은 상단원이 아니라면
				if (FALSE == IsMemberOfSameGuild(uiDestUniqueID))
				{
					// 적대 상단원이 아니라면,
					//if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiDestUniqueID))
					{
						if ((TRUE == SiegeWarfare_IsInAreaOurVillage())				// 사용자가 아군 상단 마을 근처이거나,
							|| (TRUE == SiegeWarfare_IsInAreaHostileVillage()))		// 적대 상단마을 근처라면
						{
							// 워프 시킬 수 있다.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// 워프 시킬 수 없다.
	return FALSE;
}

// robypark 2004/11/2 15:0
// 공성전 유닛 공격 속성에 따라 효과 사용하기
void	cltOnlineWorld::SiegeWarfare_CreateAttackEffectCharacter(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID, bool bNeedToSendActionMsg/* = false*/)
{
	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	SI32 siEmptySmokeID = 0;

	// 공격자 위치
	SI16 siPosition_AttakerX, siPosition_AttakerY;
	// 피격자 위치
	SI16 siPosition_TargetX, siPosition_TargetY;

	// 공격자 및 피격자 캐릭터 위치 얻기
	pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
	pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

	// 캐릭터 공격 속성에 따른 효과 연출
	switch (KI[uiKindInfoIdx_Attacker].GetAttackAtb(0))
	{
		// 공성 검병, 기마병 공격
		case ATTACKATB_SWORD:
			siEmptySmokeID = m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_BLADEHIT"));

			// robypark 2005/1/14 18:52
			// 목표 캐릭터 중심에 그려지도록 설정
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SWORD, 0, 0);
			break;

		// 공성 창병 공격
		case ATTACKATB_LONG_SPEAR:
			siEmptySmokeID = m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_SPEARHIT"));

			// robypark 2005/1/14 18:52
			// 목표 캐릭터 중심에 그려지도록 설정
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SPEAR2, 0, 0);
			break;

		// 공성 궁병 공격
		case ATTACKATB_ARROW:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_ARROW, siPosition_AttakerX, siPosition_AttakerY, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));
			
			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_ARROW, 0, 0);
			break;

		// 공성 충차 공격
		case ATTACKATB_UPGRADETOWERCANNON:
			// 공격 히트 효과 이미지는 출력되지 않으나 효과음만 연주한다.
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_WALLBREAKERHIT"), true);

			// robypark 2005/1/14 18:52
			// 목표 캐릭터 중심에 그려지도록 설정
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_JU104AE1, 0, 0);
			break;

		// 공성 발석거 공격
		case ATTACKATB_CATAPULT:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_STONE, siPosition_AttakerX - 1, siPosition_AttakerY - 1, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_CU106AE1, 0, 0);
			break;
	}
}

// robypark 2004/11/3 14:5
// 공성전 유닛 공격 속성에 따라 효과 사용하기
// 목표물의 위치에 생성
// Effect 중에 Smoke일 경우 무조건 근거리 공격으로 판단하며 서버에 Action메시지를 전송하기 않는다.
// 반면에 Bullet일 경우 무조건 원거리 마을 공격으로 판단하며 ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION메시지를 서버로 전송한다.
// 주의 서버에 메시지 전송은 자신과 관련된 경우에만 해당된다.
void cltOnlineWorld::SiegeWarfare_CreateAttackEffectVillage(UI16 uiAttackerUniqueID, SI16 ssTargetX, SI16 ssTargetY, UI16 uiVillageCode/* = 0*/)
{
	// 공격자 캐릭터 종류 정보 인덱스
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// 공격자 위치
	SI16 siPosition_AttakerX, siPosition_AttakerY;

	// 공격자 캐릭터 위치 얻기
	pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);

	// 캐릭터 공격 속성에 따른 효과 연출
	switch (KI[uiKindInfoIdx_Attacker].GetAttackAtb(0))
	{
		// 공성 검병, 기마병 공격
		case ATTACKATB_SWORD:
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_BLADEHIT"));

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SWORD, 0, 0);
			break;

		// 공성 창병 공격
		case ATTACKATB_LONG_SPEAR:
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_SPEARHIT"));

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SPEAR2, 0, 0);
			break;

		// 공성 궁병 공격
		case ATTACKATB_ARROW:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_ARROW, siPosition_AttakerX, siPosition_AttakerY, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));
			
			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_ARROW, 0, 0);
			break;

		// 공성 충차 공격
		case ATTACKATB_UPGRADETOWERCANNON:
			// 공격 히트 효과 이미지는 출력되지 않으나 효과음만 연주한다.
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_WALLBREAKERHIT"), true);

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_JU104AE1, 0, 0);
			break;

		// 공성 발석거 공격
		case ATTACKATB_CATAPULT:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_STONE, siPosition_AttakerX - 1, siPosition_AttakerY - 1, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));

			// 효과음 연주
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_CU106AE1, 0, 0);
			break;
	}
}

// robypark 2004/11/8 15:38
// 공성전 유닛인지를 검사한다.
BOOL	cltOnlineWorld::IsSiegeWarfareUnit(SI32 siImjinKindIdx)
{
	// 유효한 캐릭터 인덱스 인지 검사
	if (KIND_ON_INVALID == siImjinKindIdx)
		return FALSE;

	// 캐릭터 인덱스 범위 안인지 검사
	if (KIND_ON_MAX <= siImjinKindIdx)
		return FALSE;

	// 공성전 유닛인지 검사
	if (TRUE == KI[siImjinKindIdx].IsKindAtb(KINDATB_SIEGE_WARFARE_UNIT))
		return TRUE;

	return FALSE;
}

// robypark 2004/11/24 16:52
// 공성전 시간 등의 옵션 파일 HQ에서 읽기
BOOL cltOnlineWorld::LoadSOptionSiegeWarfare(char *pszServiceNation, SI16 siFindServiceType, char *pszFileName)
{
	FILE*	fp;
	OnlineCommonParser	ocp;

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "LoadSOptionSiegeWarfare Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		SI32	siServiceType;

		char	pszNation[32];

		ocp.GetValue( pszNation,								31,		iLineNo, iColNo );		// Service Nation
		ocp.GetValue( siServiceType,									iLineNo, iColNo );		// Service Type
		// 찾으려는 서비스 국가인지 비교
		if (stricmp(pszNation, pszServiceNation) == 0)
		{
			// 찾으려는 서비스 타입(프론트 서버) 비교	예] 대한민국 하룡은 0, 정섭은 1
			if (siServiceType == siFindServiceType)
			{
				// 공성 요일 읽기
				for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
				{
					ocp.GetValue( (char &)m_SOptionSiegeWarfare.scSiegeDay[i],	iLineNo, iColNo );		// 공성하는 요일
				}

				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN,					iLineNo, iColNo );		// 공성 시작 시간
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END,					iLineNo, iColNo );		// 공성 종료 시간

				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_Begin,	iLineNo, iColNo );		// 폭탄 투자 권한 관련 처리 시작시간
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End,	iLineNo, iColNo );		// 폭탄 투자 권한 관련 처리 종료시간

				// 대장 효과 범위
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range,	iLineNo, iColNo );
				// 공성전 유닛으로 변신할 수 없는 적대 상단 마을과의 거리
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range, 	iLineNo, iColNo );
				// 보급 수레 마나 10% 자동 채우기 쿨 타임
				ocp.GetValue( m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime,		iLineNo, iColNo );

				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_Cost,		iLineNo, iColNo );	// 마을 반격 방어력 향상 비용
				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_Amount,	iLineNo, iColNo );	// 마을 반격 방어력 향상되는 수치
				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_CoolTime,	iLineNo, iColNo );	// 마을 반격 방어력 향상 쿨 타임(초)

				// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 불가능 시간
				m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_Begin	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_End	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END;		// 공성 끝나고 한 시간 더 모집 불가능

				// 사용자들이 상단 가입이 불가능한 시간
				m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_Begin		= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_End			= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End;

				// 사용자들이 상단에서 탈퇴가 불가능한 시간(대방이 상단원 해고도 불가능하다)
				m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_Begin	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_End		= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End;
				break;
			}
		}

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// robypark 2004/12/1 10:11
// 사용자의 용병(주인공 포함) 인벤토리에 공성전 소집권 아이템이 있는지 검사한다.
BOOL cltOnlineWorld::IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(void)
{
	UI08		uiFollowerSlot = 0;
	UI16		i = 0;

	// 사용자의 모든 용병 슬롯을 검사한다.
	for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
	{
		// 해당 슬롯의 용병의 인벤토리에 공성전 소집권 아이템이 있는지 검사한다.
		if (IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(uiFollowerSlot))
			return TRUE;	// 인벤토리에 공성전 소집권 아이템이 존재한다.
	}

	// 공성전 소집권 아이템이 사용자 인벤토리에 존재하지 않는다.
	return FALSE;
}

// robypark 2005/1/8 16:26
// 해당 슬롯의 용병의 인벤토리에 공성전 소집권 아이템이 있는지 검사한다.
BOOL cltOnlineWorld::IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(UI08	uiFollowerSlot)
{
	UI16		i = 0;
	MyItemData	tempMyItemData;
	CItemHeader *pItem = NULL;

	// 유효한 용병 슬롯이라면
	if (pMyData->IsValidFollower(uiFollowerSlot))
	{
		// 인벤토리에 있는 아이템들을 검사
		for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
		{
			// 인벤토리에서 아이템 얻기
			if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
			{
				// 유효한 아이템이라면
				if(tempMyItemData.siPosInInventory != -1)
				{
					// 아이템 정보 헤더 얻기
					pItem = pItemsInfo->GetItem(tempMyItemData.uiID);
					if (pItem)
					{
						// 공성전 소집권 아이템이라면
						if (pItem->m_pImperium)
						{
							// 인벤토리에 공성전 소집권 아이템이 존재한다.
							return TRUE;
						}	// if (pItem->m_pImperium)
					}	// if (pItem)
				}	// if(tempMyItemData.siPosInInventory != -1)
			}	// if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
		}	// for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	}	// if (pMyData->IsValidFollower(uiFollowerSlot))

	// 공성전 소집권 아이템이 해당 슬롯의 용병 인벤토리에 존재하지 않는다.
	return FALSE;
}

// robypark 2004/12/2 17:31
// 주인공 및 용병 인벤토리에 있는 모든 공성전 소집권 아이템을 삭제한다.
void cltOnlineWorld::DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit(void)
{
	UI08		uiFollowerSlot = 0;
	UI16		i = 0;
	MyItemData	tempMyItemData;
	CItemHeader *pItem = NULL;

	// 사용자의 모든 용병 슬롯을 검사한다.
	for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
	{
		// 유효한 용병 슬롯이라면
		if (pMyData->IsValidFollower(uiFollowerSlot))
		{
			// 인벤토리에 있는 아이템들을 검사
			for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
			{
				// 인벤토리에서 아이템 얻기
				if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
				{
					// 유효한 아이템이라면
					if(tempMyItemData.siPosInInventory != -1)
					{
						// 아이템 정보 헤더 얻기
						pItem = pItemsInfo->GetItem(tempMyItemData.uiID);
						if (pItem)
						{
							// 공성전 소집권 아이템이라면
							if (pItem->m_pImperium)
							{
								// 인벤토리에 공성전 소집권 아이템이 존재한다. 아이템을 삭제해라
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
// 공성전 유닛일 경우 공성전 유닛의 사용자 이름, 체력, 보급품, 등을 볼 수 있는지를 나타내는 플래그 설정
void	cltOnlineWorld::SetIsShowInformationAboutSiegeWarfareUnit(SI08 scIsShowInformationAboutSiegeWarfareUnit)
{
	m_scIsShowInformationAboutSiegeWarfareUnit = scIsShowInformationAboutSiegeWarfareUnit;
}

// robypark 2005/1/8 11:18
// 보급품 자동 소모 남은 지연시간 얻기
SI32	cltOnlineWorld::GetRestTimeAutoSpendSupplyGoods(void)
{
	return m_siRestTimeAutoSpendSupplyGoods;
}

// robypark 2005/1/8 11:18
// 보급품 자동 소모 남은 지연시간 설정
void	cltOnlineWorld::SetRestTimeAutoSpendSupplyGoods(SI32 siRestTimeAutoSpendSupplyGoods)
{
	m_siRestTimeAutoSpendSupplyGoods = siRestTimeAutoSpendSupplyGoods;
}

// robypark 2005/1/12 19:30
// 공성전 공격 모드 설정 초기화
void	cltOnlineWorld::CancelSiegeWarfare_AttackMode(void)
{
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
}

// H2-S-05.02.04 //
// 유저 어카운트로 캐릭터 이름 얻는다.
char*	cltOnlineWorld::OnlineCharName(UI16 nAccount)
{
	return pIOnlineChar->GetCharName(nAccount);
}