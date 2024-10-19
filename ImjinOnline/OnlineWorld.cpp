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
//#include	<OnlineBasicTip.h>			// actdoll (2004/02/16 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
//#include	<OnlineBasicQuest.h>		// actdoll (2004/02/16 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
#include	<OnlineParty.h>
#include	<OnlineFollower.h>
#include	<IProfit.h>
#include	<OnlineDungeonParser.h>
#include	<OnlineTicket-Parser.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
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
//#include    "OnlineBanner.h"			// actdoll (2004/02/25 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
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
#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME ��Ʈ�ѷ� ����

#include "../SCCommon/OnlineCommonParser.h"	// Using Online Common Parser

#include	"FieldHeroInfoMgr.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

#include "SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"


//�ؼ����� �߰��κ� 
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

// sagolboss �߰��κ� ->
#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif
// <- �߰� �� 

//#define	CPF		33				// �� �����Ӵ� Ŭ������ 40�̴�.
//#define	CPF		1			// �� �����Ӵ� Ŭ������ 40�̴�.

extern _clGame* pGame;
extern	_MainData				GD;									// ������ �ֵ����� 
extern _InputDevice				IpD;
extern	SoundManager			*g_pSoundManager;
extern	CCharFileLoadManager	g_FileLoadManager;					// ������ ĳ����
extern CItem					*g_pItem;
extern char						LogInID[128];

extern clock_t					CurrentClock, MonitorMacroClock1, MonitorMacroClock2;

extern int WindowModeSwitch;
extern int bActive;

LPDIRECTDRAWSURFACE7              lpCaptureSurface = NULL;

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 

// ������ 2 Ŭ���̾�Ʈ�� ����� �ϱ� ���� �����̴�.
OnlineImjin2I					GOnlineImjin2I;

// ����׸� ���� ��������.
// HQDebug  GHQDebugTool;

// actdoll (2004/08/27 0:34) : �Է±� ���� ���� ����� â(_DEBUG�ø� ����. ���ٷ��� �ּ��ް�)
//#define _SHOW_INPUTSTATUS_

// robypark 2005/1/7 18:41
// ��ġ(10%������)�� ������ ���·� �׷��ش�.
// ������ ���� ü��, ���� ���������� ���
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

//----------------------------------------------------------------------------------------------------------------
//	����	:	�¶��� ���� �ʱ� �Լ�.
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
//	����	:	�¶��� �Լ�.
//----------------------------------------------------------------------------------------------------------------
UI16	_clGame::OnlineWorld()
{
	UI16	uiNextStatus = 0;

	uiNextStatus	=	pOnlineWorld->Poll();

	if(uiNextStatus)	
	{
		// GS_INITONLINEBATTLEGAME�� ������2 ������ ���� �����̹Ƿ�
		// �̶� ��� �޸𸮸� �����ؼ��� �ȵȴ�.
		if(uiNextStatus != GS_INITONLINEBATTLEGAME)
			FreeOnlineWorld();	
	}
	
	return uiNextStatus;		
}

//----------------------------------------------------------------------------------------------------------------	
//	����	:	�¶��� ���� �Լ�.
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
//	����	:	������ �¶��� ���� �Լ�.
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

	// ������ ���� ��� ���� ����
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	// robypark 2005/1/14 17:11
	// ���ΰ� ĳ���͵��� ���� ������ �ʱ�ȭ
	m_pFieldHeroInfoMgr = NULL;

	// robypark 2005/1/31 12:55
	// ���� ���� ������
	m_pSiegeUnitTableMgr = NULL;

	// ������ �����.
	siOnlineMajorVersion		=	ON_CV_MAJOR;
	siOnlineMinorVersion		=	ON_CV_MINOR;

	bExitOnlineGame				=	FALSE;
	bFindTrade					=	FALSE;

	ZeroMemory( m_TempBuffer, sizeof( m_TempBuffer ) );
	wsprintf(szOnlineVersion, "Ver %d.%03d", siOnlineMajorVersion, siOnlineMinorVersion / 10);

	ZeroMemory(m_MemoUser, 32);

	bCharMove		= TRUE;

//	m_bEventDokdoQuestPlay	= FALSE;	// actdoll (2004/04/02 15:28) : = �̺�Ʈ = ���� ����Ʈ�� �������ΰ�?

	// �ָ� ������ �����۰���
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
//	m_Spr_Icon_DokdoQuest.Image	= NULL;	// actdoll (2004/04/02 15:28) : = �̺�Ʈ = ���� ����Ʈ�� �̹��� ������

//	m_IME_DLLHandle = NULL;
	
	// actdoll (2004/03/30 11:02) : = �̺�Ʈ = ��������Ʈ �ʱ�ȭ
//	m_uiDokdoQuestCompleteUserCount	= 0;

	// ������ ������ ��� ������ ������ ����� �̸�, ü��, ����ǰ, ���� �� �� �ִ����� ��Ÿ���� �÷���
	m_scIsShowInformationAboutSiegeWarfareUnit = 0;

	m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_NONE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ �¶��� ���� �Լ�.
//----------------------------------------------------------------------------------------------------------------
cltOnlineWorld::~cltOnlineWorld()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ �¶��� ���� �ʱ�ȭ.
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

	// ������ ���� ��� ���� ����
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

	// ������ �����.
	siOnlineMajorVersion		=	ON_CV_MAJOR;
	siOnlineMinorVersion		=	ON_CV_MINOR;

	bExitOnlineGame				=	FALSE;
	bFindTrade					=	FALSE;

	ZeroMemory( m_TempBuffer, sizeof( m_TempBuffer ) );
	wsprintf(szOnlineVersion, "Ver %d.%03d", siOnlineMajorVersion, siOnlineMinorVersion / 10);

	ZeroMemory(m_MemoUser, 32);

	bCharMove		= TRUE;

	// actdoll (2004/04/02 15:28) : = �̺�Ʈ = ���� ����Ʈ ����
//	m_bEventDokdoQuestPlay	= FALSE;	
//	if(m_Spr_Icon_DokdoQuest.Image)  {clGrp.FreeXspr(m_Spr_Icon_DokdoQuest);  m_Spr_Icon_DokdoQuest.Image  = NULL;}

	// �ָ� ������ ������ ����
	m_bReceivedItem			= FALSE;
	m_bItemDraging			= FALSE;
	m_siReceivedItemCount	= 0;
	m_siInnStatus			= -1;
	if(m_ReceivedItemAniSpr.Image)  {clGrp.FreeXspr(m_ReceivedItemAniSpr);  m_ReceivedItemAniSpr.Image  = NULL;}
		

	pOnlineCharUI->Init(this);

#ifdef _OGG_PLAY
	// actdoll (2004/11/25 13:35) : �����Ͼ� ������ ������ ��а� ����
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
	// ��������� ���·� �α׾ƿ� �Ŀ� �������Ͽ��� ȭ�� �������̽��� ǥ�õǴ� �ؽ�Ʈ��
	// ��ȭ���� �ʱ�ȭ �Ǿ������� �ұ��ϰ� �������� ǥ�õǴ� ���� ����.
	// ����, �ŷ����� �ؽ�Ʈ ���� ���� ����
	pOnlinePannel->SetButtonText_PeaceMode(!bPeace);
	pOnlinePannel->SetButtonText_TradeMode(!pOnlineTrade->m_bPermitTrade);

	// ������ ������ ��� ������ ������ ����� �̸�, ü��, ����ǰ, ���� �� �� �ִ����� ��Ÿ���� �÷���
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

	// actdoll (2004/11/15 18:45) : �߰�
	if( !g_bSkipIntro )		IntroPoll();

	//----------------------------------------------------------------------------
	//	����Ʈ �ε�ȭ�� �̹��� �غ�
	//----------------------------------------------------------------------------

	pOnlineLoadingData = new OnlineLoadingData;
	ZeroMemory( pOnlineLoadingData, sizeof(pOnlineLoadingData) );

	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
	SetGamePalette(uiPalette);

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_bg.Spr", pOnlineLoadingData->BackImage );	
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_bg_over.Spr", pOnlineLoadingData->BackOverImage );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_play_grade.Spr", pOnlineLoadingData->PlayGradeImage );
	if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Gamania.Spr", pOnlineLoadingData->m_Spr_CompanyLogo );	// �ѱ�/�߱�/�Ϻ�/�븸/ȫ���
	else														clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Bolehcom.Spr", pOnlineLoadingData->m_Spr_CompanyLogo );	// �ε��׽þƿ�

	// actdoll (2004/12/20 19:13) : �ΰ� ����
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Gersang_Logo_Ani.Spr", pOnlineLoadingData->AniLogoImage );

	m_LanguageCode = pGame->LanguageCode; 

	//----------------------------------------------------------------------------
	//	����Ʈ �ε�ȭ�� �̹��� �غ� �Ϸ�
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 1�ܰ�	- "����" ���� �� '��' ���� ���κ� 1ȹ		: START
	//----------------------------------------------------------------------------
	// �¶��ο��� ����ϴ� ��Ʈ�� �����Ѵ�.
	pOnlineFonts	=	new	OnlineFonts;
	if( !pOnlineFonts->Create() )
	{
		clGrp.Error("OnlineFonts Init Error", "");
		delete pOnlineLoadingData;
		pOnlineLoadingData	= NULL;
		return FALSE;
	}

	// �¶��ο��� ��¿����� ����ϴ� �ؽ�Ʈ ������ �����Ѵ�.
	pOnlineText				=	new OnlineText;
	if(pOnlineText->Init( (CHAR*)pGame->m_pszHQNation, (CHAR*)( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) ) == FALSE)
	{
		clGrp.Error("OnlineText Init Error", "");
		delete pOnlineText;
		pOnlineText	= NULL;
		return FALSE;
	}
	
	// actdoll (2004/05/04 16:33) : [OECEX]
	// ����Ʈ �ڵ鷯�� �⺻ ����
	Handler_OnlineEditControlEx::Initialize( pGame->Hwnd );
	Handler_OnlineEditControlEx::SetCandidateFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	// actdoll (2004/11/03 17:10) : IME ��Ʈ�ѷ� ����
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
	//	�ε� 1�ܰ�	- "����" ���� �� '��' ���� ���κ� 1ȹ		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 2�ܰ�	- "����" ���� �� '��' ��� ���κ� 1ȹ		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(10);

	// ������ �¶��ο��� ���� ���ҽ����� �ε��Ѵ�.	
	pOnlineResource		=	new	OnlineResource;	
	if(pOnlineResource->Init() == FALSE) 
		clGrp.Error("FKJE8567", "Online Resource Init Failed");	
	//----------------------------------------------------------------------------
	//	�ε� 2�ܰ�	- "����" ���� �� '��' ��� ���κ� 1ȹ		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 3�ܰ�	- "����" ���� �� '��' �߾� �� 2ȹ			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(20);

	pOnlineMap	=	new OnlineMap;	
	// ���� �ҷ��´�.
	// ���������� ���� �޸𸮿� �ε����� �ʴ´�. �ٸ� �ʿ��Ҷ� �ε��� �� �ֵ��� ȭ�� �̸��� �����Ѵ�.	
	if(pOnlineMap->Create(this) == FALSE)
	{
		clGrp.Error("Map Load Error.", "");						
	}		

	m_pOnlineMapSound = new OnlineMapSound();
	ASSERT(m_pOnlineMapSound);
	{
		// ������ ���� ���� ����. 
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

	// �¶��ο��� ����ϴ� ��� ���콺�� �����ϴ� ��ü�� �����Ѵ�.
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
	// actdoll (2004/11/25 13:35) : �����Ͼ� ������ ������ ��а� ����
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
	
	GD.Init();					// ���� ����Ÿ �ʱ�ȭ	
	//----------------------------------------------------------------------------
	//	�ε� 3�ܰ�	- "����" ���� �� '��' �߾� �� 2ȹ			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 4�ܰ�	- "����" ���� �� '��' �ϴ� �� 1ȹ ���� �ϼ�	: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(30);

	/*********************************************************************************************************************
									�ε� ��������Ʈ�� �ε��ϰ� �ȷ�Ʈ�� �ε� �Ѵ�.
	*********************************************************************************************************************/	
	// �¶��ο��� ����ϴ� ��Ʈ�� �����Ѵ�.
//	pOnlineFonts	=	new	OnlineFonts;
//	pOnlineFonts->Create();

	// �¶��ο��� ��¿����� ����ϴ� �ؽ�Ʈ ������ �����Ѵ�.
//	pOnlineText				=	new OnlineText;
//	if(pOnlineText->Init( (CHAR*)pGame->m_pszHQNation ) == FALSE)
//		clGrp.Error("OnlineText Init Error", "");	

	m_cltIGuildClass.Init("Online\\Village\\grade.txt");

	/*********************************************************************************************************************
									Palette Manager�� �����Ѵ�.
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
	//	�ٸ� ������� �ش� ������ �ε��Ѵ�.
	m_SlanderFilter.LoadReferenceWord( (CHAR*)pGame->m_pszHQNation, ( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] ? DF_PATH_DEBUG_OUTSIDE_HQTEXT : NULL ) );
	//----------------------------------------------------------------------------
	//	�ε� 4�ܰ�	- "����" ���� �� '��' �ϴ� �� 1ȹ ���� �ϼ�	: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 5�ܰ�	- "����" ���� �� '��' ��� �� 1ȹ			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(40);

	/*********************************************************************************************************************
									TicketParser�� �����Ѵ�.
	*********************************************************************************************************************/
	pTicketParser               =   new OnlineTicketParser;
	pTicketParser->ReadText("Online\\Village\\TicketInfo.txt");

	/*********************************************************************************************************************
									GeneralParser�� �����Ѵ�.
	*********************************************************************************************************************/
	pGeneralParser               =   new OnlineGeneralParser;
	pGeneralParser->ReadText("Online\\CharInfo\\General_list.txt");

	/*********************************************************************************************************************
									GeneralParserEx�� �����Ѵ�.(2�� ���� �뺴)
	*********************************************************************************************************************/
	pGeneralParserEx               =   new OnlineGeneralParser;
	pGeneralParserEx->ReadText("Online\\CharInfo\\GeneralListEx.txt");

	/*********************************************************************************************************************
									QuestParser�� �����Ѵ�.
	*********************************************************************************************************************/
	pQuestParser				=	new OnlineQuestParser;
	// robypark 2004/9/9 17:53
	// �̺�Ʈ�� ����Ʈ ���� ���� �߰�
	char szHQNation[128];
	sprintf(szHQNation, "%s", (char*)pGame->m_pszHQNation);
	pQuestParser->ReadText("Online\\Quest\\quest.txt", szHQNation, 2004, 9, 13);

	/*********************************************************************************************************************
									LevelMark Parser�� �����Ѵ�.
	*********************************************************************************************************************/
	pLevelMarkParser				=	new OnlineLevelMarkParser;
	pLevelMarkParser->Create("Online\\CharInfo\\LevelMark.txt");

	/*********************************************************************************************************************
									Timer�� �����Ѵ�.
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
									���忡 ���� ����
	*********************************************************************************************************************/	
	pOnlineEconomy = new OnlineEconomy;
	pOnlineEconomy->Init("Online\\Village\\Economy.txt");

	// ���̷�Ʈ ��ο� ��ü�� �����Ѵ�. �ݵ�� ���ҽ��� �ε��� �� ȣ���� �־�� �Ѵ�.
	if(clGrp.CreateOffScreen(pOnlineDirectDraw, ON_SCREEN_XSIZE, ON_SCREEN_YSIZE, TRUE) == FALSE)
	{
		clGrp.Error("FKJE8567", "Online DirectDraw Create Failed");	
		return FALSE;
	}
	
	// �� ������ �ʱ�ȭ�Ѵ�.

//	DrawTextInLoading("���� ������ ���� ��");
	// ���� �����ڸ� �����Ѵ�.
	pOnlineVillageManager	=	new OnlineVillageManager;

	if(!pOnlineVillageManager->Init("Online\\Village\\VillageInfo.txt", "Online\\Village\\Structure Placement.txt", this))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_VILLAGE_MANAGER_INIT_ERROR), "");						
	}		
	m_fChattingRoom             =   FALSE;

	// ���� ���� List ����
//	DrawTextInLoading("���� �ȳ��� ������ ���� ��");
	pOnlineVillageDescriptList = new OnlineVillageDescriptList;
	pOnlineVillageDescriptList->Init(2, 16);

	// �������� List
	pOnlineNoticeList = new OnlineNoticeList;
	pOnlineNoticeList->Init(this, 10, 5, 233, 582, 288, 4);

	// �ʵ�� Effect
	pOnlineFieldEffect = new OnlineFieldEffect;
	pOnlineFieldEffect->Init(this, 64);

	// ��ų
	pOnlineSkillParser = new OnlineSkillParser;
	pOnlineSkillParser->Init();
	//----------------------------------------------------------------------------
	//	�ε� 5�ܰ�	- "����" ���� �� '��' ��� �� 1ȹ			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 6�ܰ�	- "����" ���� �� '��' ��� ���κ� 1ȹ		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(50);

	// ������Ʈ �������̽��� �����Ѵ�.

	pIOnlineObject	=	new	IOnlineObject;

	// ������Ʈ�� �ʱ�ȭ �Ѵ�.
	if(!pIOnlineObject->Init())
	{
		clGrp.Error("FKJE8567", pOnlineText->Get(ON_TEXT_OBJECT_INIT_ERROR));
	}	

	// Ű���� �Է��� ó���ϴ� Ŭ������ �����Ѵ�.
	pOnlineKeyboard	=	new	OnlineKeyboard;
	pOnlineKeyboard->Init();

	// ĳ������ ���ڿ� �׼� �ڵ带 ���� �׼� �ڵ�� ��ȯ�� �ִ� ��ü�� �����Ѵ�.
	// ĳ������ �ִϸ��̼� ������ �ʱ�ȭ �ϱ����� ���־�� �Ѵ�.
	pOnlineCharActionCode	=	new OnlineCharActionCode;
	
	// ĳ������ �ִϸ��̼� ������ �ʱ�ȭ �Ѵ�. �ݵ�� ĳ���͸� �ʱ�ȭ �ϱ� ������ ���־�� �Ѵ�.	
//	pOnlineCharAni	=	new OnlineCharAni;

	//clGrp.Error("", "pOnlineCharAni->Init 1");
/*	if(!pOnlineCharAni->Init(this))
	{
		clGrp.Error("ĳ���� �ִϸ��̼� ������ �ʱ�ȭ �ϴµ� �����߽��ϴ�.", "");
	}*/
	//clGrp.Error("", "pOnlineCharAni->Init 2");

	// �� ������ �ʱ�ȭ �Ѵ�.
//	clGrp.Error("", "pIOnlineShipKI	=	new IOnlineShipKI 1");
	pIOnlineShipKI	=	new IOnlineShipKI;
//	clGrp.Error("", "pIOnlineShipKI	=	new IOnlineShipKI 2");

//	clGrp.Error("", "pIOnlineShipKI->Init() 1");
	if(!pIOnlineShipKI->Init())
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_SHIP_KIND_INIT_ERROR), "");
	}
//	clGrp.Error("", "pIOnlineShipKI->Init() 2");

	// ĳ���� ������ �ʱ�ȭ �Ѵ�.
	pIOnlineCharKI	=	new	IOnlineCharKI;

	if(!pIOnlineCharKI->Init(this))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_CHAR_KIND_INIT_ERROR), "");
	}
	//----------------------------------------------------------------------------
	//	�ε� 6�ܰ�	- "����" ���� �� '��' ��� ���κ� 1ȹ		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 7�ܰ�	- "����" ���� �� '��' ���ߴ� �� 2ȹ			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(60);

//	DrawTextInLoading("������Ʈ ���� �ʱ�ȭ, ĳ���� ����, ���� ���� ��");
	// ������Ʈ ������ �ʱ�ȭ �Ѵ�.
	pIOnlineObjectKI	=	new IOnlineObjectKI;

	if(!pIOnlineObjectKI->Init(pOnlineResource, pOnlineText))
	{
		clGrp.Error(pOnlineText->Get(ON_TEXT_OBJECT_KIND_INIT_ERROR), "");
	}	

	// ĳ���� �������̽��� �����Ѵ�.
	pIOnlineChar	=	new	IOnlineChar;

	// ĳ���͸� �ʱ�ȭ �Ѵ�.
	if(pIOnlineChar->Init(this) == FALSE)
	{
		clGrp.Error("FKJE8567", pOnlineText->Get(ON_TEXT_CHAR_INIT_ERROR));
	}

	// ������ �ΰ������� �����Ѵ�.
	pclFindPath	=	new cltOnlineFindPath(ON_MAP_XSIZE, ON_MAP_YSIZE, pOnlineMap);
	
	pOnlineMap->GetSize(&siMapXsize, &siMapYsize);
	pclFindPath->SetFindPathSize(siMapXsize, siMapYsize);		
	//----------------------------------------------------------------------------
	//	�ε� 7�ܰ�	- "����" ���� �� '��' ���ߴ� �� 2ȹ			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 8�ܰ�	- "����" ���� �� '��' ���� ���κ� 1ȹ		: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(70);

	// ���� ����Ÿ�� �����Ѵ�.
	pMyData		=	new	MyData;
	pMyData->Init(this);

	// �� ĳ���͸� �����ϱ� ���� ��ü�� �����Ѵ�.	
	pOnlineCharUI	=	new	OnlineCharUI;
	pOnlineCharUI->Init(this);	
	
	dwCurrentFrameStartTime		=	0;
	dwCurrentFrameAddTime		=	0;
	dwCurrentFrameSkippingCount	=	0;
	siMainPollStatus			=	ON_MAINPOLL_LOGINMGR;	

	// �̸�Ƽ�� ó��
	pOnlineImoticon = new OnlineImoticon;
	pOnlineImoticon->Init(this);

	// �κ��丮�� �����Ѵ�. ���⼭�� ������ ���ְ� �ʱ�ȭ�� ���� ����Ÿ�� ���� ���Ŀ� ���ش�.
	pOnlineInventory		=	new	OnlineInventory;
	pOnlineTradeBook		=	new OnlineTradeBook;
	pOnlineTradeBook->Init( this );

	// �¶��ο��� ����ϴ� ��� �������� �ε��Ѵ�.		
	pItemsInfo				=	new CItem;		

	pItemsInfo->ItemLoad(	pOnlineText, 
							"Online\\Item\\weapon.txt", "Online\\Item\\armor.txt", "Online\\Item\\accessary.txt", "Online\\Item\\Animal.txt", 
							"Online\\Item\\Dress.txt", "Online\\Item\\Element.txt", "Online\\Item\\Food.txt", "Online\\Item\\Liquor.txt", 
							"Online\\Item\\Medicine.txt", "Online\\Item\\Tool.txt", "Online\\Item\\Trade.txt", "Online\\Item\\Specific.txt" ,
							"Online\\Item\\Imperium.txt");

	g_pItem	=	pItemsInfo;

	for( i=0 ; i<(SI32)pItemsInfo->GetCount(); ++i)											// ���ҽ��� ��ȯ
	{
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );
				
		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			if( pOnlineResource->GetItemFileIndex( szSpriteFileName ) == -1 )					// ū �׸��� �ε��Ѵ�.( �׸��� �ε尡 �ž� ���� �ʴٸ� �ε���)			
				pOnlineResource->AddItemSpr( szSpriteFileName );									

			if( pOnlineResource->GetItemFileIndex( szSmallFileName ) == -1 )					// ���� �׸��� �ε��Ѵ�.			
				pOnlineResource->AddItemSpr( szSmallFileName );			
		}		

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// robypark 2004/11/24 16:55
	// ������ �ð� ���� �ɼ� ���� HQ���� �б�
	LoadSOptionSiegeWarfare(szHQNation, pGame->m_iServiceType, "Online\\OptionSiegeWarfare.txt");

	// robypark 2005/1/14 17:11
	// ���ΰ� ĳ���͵��� ���� ������ ���� �� �ε�
	m_pFieldHeroInfoMgr = new CFieldHeroInfoMgr;
	m_pFieldHeroInfoMgr->LoadFieldHeroInfo();

	// robypark 2005/1/31 12:55
	// ���� ���� ������
	m_pSiegeUnitTableMgr = new CSiegeUnitTableMgr;
	m_pSiegeUnitTableMgr->LoadSiegeUnitTable();			// ���� ���� �⺻ ���̺�
	m_pSiegeUnitTableMgr->LoadSiegeUnitTableCalcHP();	// �����¿� ���� ü�� ��� ���̺�
	m_pSiegeUnitTableMgr->LoadSiegeUnitTableCalcMP();	// �����¿� ���� ���� ��� ���̺�

	//----------------------------------------------------------------------------
	//	�ε� 8�ܰ�	- "����" ���� �� '��' ���� ���κ� 1ȹ		: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 9�ܰ�	- "����" ���� �� '��' �߾� �� 2ȹ			: START
	//----------------------------------------------------------------------------
	pOnlineLoadingData->uiFrame++;
	ScrollLoading(80);

	for( i=0; i<(SI32)pItemsInfo->GetCount() ; ++i)												// �ٽ� �ѹ� ������ ����ü�� ���ҽ� �ε����� �־� �ش�.
	{		
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );					// ������ ������ �޴´�.

		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			// ū �׸�
			siFile		=	pOnlineResource->GetItemFileIndex( szSpriteFileName );			// ���ҽ����� ���� �̸��־� �ε����� �޴´�.
			pTempSpr	=	pOnlineResource->GetxsprItem( siFile );

	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSpriteFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, FALSE );
			else					clGrp.Error("", pOnlineText->Get(ON_TEXT_ITEM_SEARCH_ERROR), szSpriteFileName );

			// ���� �׸�
			siFile		=	pOnlineResource->GetItemFileIndex( szSmallFileName );				// ���ҽ����� ���� �̸��־� �ε����� �޴´�.
			pTempSpr	=	pOnlineResource->GetxsprItem( siFile );
			
	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSmallFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, TRUE );
			else					clGrp.Error("", pOnlineText->Get(ON_TEXT_ITEM_SEARCH_ERROR), szSmallFileName );
		}

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// Config ���Ͽ�
	pConfig					=	new OnlineConfig;
	pConfig->OpenConfig();
	if(LogInID[0]) pConfig->SetConfig( LogInID );

	// Mega Text��	
	pOnlineMegaText			=	new	OnlineMegaText;	
	pOnlineMegaText->Initialize(pOnlineResource, pOnlineFonts);	

	pOnlineBuilding			=	new OnlineBuilding;
	if( pOnlineBuilding->Init() == FALSE )
		clGrp.Error("", pOnlineText->Get(ON_TEXT_STRUCT_INIT_ERROR) );

	// �ʵ� ���� ���� ��ü.
	pOnlineFieldArea		=	new OnlineFieldArea;
	pOnlineFieldArea->Init();	

	// �ǳ��� �ʱ�ȭ �Ѵ�.	
	pOnlinePannel			=	new OnlinePannel;
	pOnlinePannel->Init(this);	

	pIProfit				=	new cltIProfit;
	pIProfit->Init( "Online\\Village\\Profit.txt" );

	// �̴ϸ��� �ε���
	pOnlineMiniMap			=	new OnlineMiniMap;
	pOnlineMiniMap->Init(this);

	pOnlineHelp				=	new OnlineHelp;
	pOnlineHelp->Init(this);
	
	pOnlineClient	=	new	OnlineClient;
	pOnlineClient->InitZero();	
	pOnlineClient->InitSocket();					// �ѹ��� ����

	// RequestPlayerInfo ������ �ʱ�ȭ �Ѵ�.
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
	//	�ε� 9�ܰ�	- "����" ���� �� '��' �߾� �� 2ȹ			: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 10�ܰ�	- "����" ���� �� '��' ���� �� 5ȹ ���� �ϼ�	: START
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

//	pOnlineBasicTip					=	new OnlineBasicTip;		// actdoll (2004/02/16 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
//	pOnlineBasicTip->Init(this);

//	pOnlineBasicQuest				=	new OnlineBasicQuest;	// actdoll (2004/02/16 16:11) : ����ϴ� ���� ������ ���� Ȯ���߽��ϴ�.
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

	// actdoll (031229) : �ʻ� Ŭ������ �ʱ�ȭ�Ѵ�. �ϴ� �ᰡ���ڴ�.
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
	// ��ũ�� ���μ���
//	m_pMacroProcess = new CMacroCheckProcess(static_cast<COnlineClientCallback*>(pOnlineClient));
//	m_pMacroProcess->StartThread();

//�ؼ����� �߰��κ� 
//	pJSWndMgr = new JSWndMgr(this, 1);
//	pJSTestWnd = new JSTestWnd;

	
	pGame->ChatInit();
	//----------------------------------------------------------------------------
	//	�ε� 10�ܰ�	- "����" ���� �� '��' ���� �� 5ȹ ���� �ϼ�	: END
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	�ε� 11�ܰ�	- "����" ����								: START
	//----------------------------------------------------------------------------
	// Loading11
	ScrollLoading(100);
	
	//����Ʈ ĳ���� �ʱ�ȭ.
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
//	����	:	������ �¶��� ����.
//	����	:	���� GameStatus ��.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::Poll()
{
	UI16	uiTempMainPollStatus;
	UI16	uiNextMenu	=	0;

	//============================================================================================
	// ���콺 ��ġó��
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

	// ���콺�� �����Ѵ�.
	if ((siGamePollStatus != ON_GAMEPOLL_FIELD) && (siGamePollStatus != ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE))
		pIOnlineMouse->Action();
	pIOnlineMouse->SetPos(IpD.Mouse_X, IpD.Mouse_Y);

	// ���콺 Up Check
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
		// ������ �¶��ο��� ����ϴ� �Ϻ� �ڿ����� �����Ѵ�.
		// ��� �ڿ��� �����ϸ� �ȵȴ�.		
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
//	����	:	���� Poll() �ʱ�ȭ
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitGamePoll()
{		
	RECT	rcMap;

	siGamePollStatus		=	ON_GAMEPOLL_FIELD;	

/*	UI08	uiPalette[768];	
	clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
	SetGamePalette(uiPalette);*/

	// Ŭ������ �����Ѵ�.(���� �������� Ŭ������ �����Ѵ�.)
	rcMap	=	pOnlineMap->GetScreenRect();
	clGrp.SetClipArea(rcMap.left, rcMap.top, rcMap.right, rcMap.bottom);

	// Ű���� ���۸� û�����ش�.
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

	// ������ ���� ǥ���̹��� �ε�
	clGrp.LoadXspr( "Online\\GameSpr\\Inn_ReceivedItem.Spr", m_ReceivedItemAniSpr );

	// actdoll (2004/04/02 15:30) :	= �̺�Ʈ = ���� ����Ʈ ������ �ε�
//	clGrp.LoadXspr( "Online\\GameSpr\\Icon_Taekuk.SPR", m_Spr_Icon_DokdoQuest );

	pConfig->OpenConfig();

	if(!m_pMercenary->IsAction())
		m_pMercenary->OnButtonPush();

	pOnlineFieldAuction->OnGameStart();

	::SetFocus(pGame->Hwnd);		
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� Poll()
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
	// �Ϲ� MSG ó��
	while(1)
	{
		uiClientRet	 =	pOnlineClient->DataProc();										// ����Ÿ�� ���ν���.

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
	// ������ �̵�
	if(pMyData->IsGotoVillage() && (pMyData->IsWaitGotoVillageResponse() == FALSE) && siGamePollStatus == ON_GAMEPOLL_FIELD )
	{
		// �������� ������ ���ٰ� ��û�Ѵ�.		
		pOnlineClient->SendGotoVillage(pMyData->GetGotoVillage());
		pMyData->SetWaitGotoVillageResponse(TRUE);			
	}


	//============================================================================================
	// Game Server�� �̻������ �˻�
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
	// Portal�� �̵�
	if(pOnlineMsgBoxUsePortal->IsActive())
	{
		SI16			TempX, TempY;

		switch(pOnlineMsgBoxUsePortal->Process())
		{
			case ON_MSGBOX_RETURN_OK:
				// ���� �Ŵ��� �˻�

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
					// �����̳� �ſ뵵�� �Ŵ��� �˻�

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
						// ���� ������ Portal�� ID �� �˻�

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
			// �������� Portal�� ���ٰ� ��û�Ѵ�.						
			siGotoPortalID	= pMyData->GetGotoPortal();			// �� Portal�� �̸��� ���´�.
			
			// ���� Portal�� ���� �ִ� �� �˻�
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
					// ���� ������ Portal�� ID �� �˻�

					NextMapIndex = siGotoPortalID;
					pOnlineClient->SendGotoPortal(siGotoPortalID);

					pMyData->SetWaitGotoPortalResponse(TRUE);
				}
			}
		}
	}
	
	//============================================================================================
	// �ʵ�� �̵�
//	if(pMyData->IsGotoField() == TRUE)
//	{
//		GotoField();	
//		pMyData->SetGotoField(FALSE);	
//	}	

	//============================================================================================
	// �ð��� ���� �㳷 ó��
	float					TempLight;
	SOnlineTime				NowTime;
	static SOnlineTime		YetNowTime;

	TempLight  = 1.0f;
	NowTime    = pTimer->GetNowTime();

	if(pOnlineVillage != NULL)
	{
		if(pOnlineVillage->GetStatus() != ON_VILLAGE_STATUS_IN_STRUCTURE) 
		{
			// ���� ��ġ�� �ǹ����� �ƴϸ� �ð��� ���� �����
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
		// ���� ��ġ�� �ǹ����� �ƴϸ� �ð��� ���� �����
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
	// ���� ���ݽ� ����� Delay ó��
	if(m_VillageAttackDelay)
	{
		m_VillageAttackDelay--;
	}
 
	//============================================================================================
	// ���콺 One Click
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

			// ���� �������̽� Poll
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
//	����	:	ĳ���Ͱ� �ʵ�� ������ ȣ���ϴ� �Լ�.
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

	// ���� �������� ������ �� �ִ� ��Ȳ�� �ɶ����� ����Ѵ�.
	dwPrevFrameStartTime			-=	dwCurrentFrameAddTime;
	dwCurrentFrameAddTime			=	0;
	dwPrevFrameTimeRequire			=	timeGetTime() - dwPrevFrameStartTime;

	// �ʴ� �����Ӽ��� �����.
	if(dwPrevFrameTimeRequire < CPF)
	{
		Sleep(1);
		return uiNextMenu;
	}
	else
	{
		dwCurrentFrameAddTime	=	dwPrevFrameTimeRequire - CPF;
	}

	// ���� ��ǻ�Ͱ� ������ Render�� ���� �ʴ´�.
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
			// ������ ���ӽ� �����ߴ��� �����ϱ�
			pOnlineSetting->GameInit();

			// �α��� ȭ������
			SetMainPollStatus(ON_MAINPOLL_LOGINMGR);
			SetLogOut(0, ON_MAINPOLL_LOGINMGR);
			m_WaitTickCount = 0;
			return uiNextMenu;
		}
	}
	////////////////////////////////////////////////////
	// �������� ���� ��Ų��.
	uiTotalFrame++;


	siFPS	=	1000 / (timeGetTime() - dwPrevFrameStartTime);

	// ������ ����
	if (TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()))
	{
		// ���� ���� �ð� �ڵ� ����
		pIOnlineChar->SetSiegeWarfareUnit_PastAttackDelay(pMyData->GetMyCharUniqueID(), dwPrevFrameTimeRequire);

		/*
		// ������ ���� ������ ��� �ڵ� �Ҹ���� �ʴ´�.
		if (KI[pIOnlineChar->GetImjinKind( pMyData->GetMyCharUniqueID() )].IsAttribute1(ATB_PATIENT))
		{
			// ����ǰ �ڵ� �Ҹ� ���� �ð� ó��
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

	// ���� �ð��� ���´�.
	dwCurrentFrameStartTime		=	timeGetTime();		

	// ������ FrameSkipping ������ Up Check�� �����ϰ� ������ ��찡 �ֱ⶧���� ���⼭�� �ٽ� �˻�

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

	DWriteStringToFile("���� 2");

	// ���� ���	
	pIOnlineMouse->Action();
	pOnlineMousePoint->Action();

	// Ű������ �Է»��¸� ���´�.
	pOnlineKeyboard->Action();

	//�ؼ����� �߰��κ� 
	// ��� ���� ������
//	pJSWndMgr->Action();
	
	ptMouse.x	=	IpD.Mouse_X;
	ptMouse.y	=	IpD.Mouse_Y;

	pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);		
	pIOnlineChar->GetPos(pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

	// ���� ���� ó��
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
		// ���� ������ üũ�Ͽ� �� ������ ��������� �����Ѵ�.
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
				// ���ΰ� ���� ������ ������ ���
				CFieldHeroInfo *pFieldHeroInfo = m_pFieldHeroInfoMgr->GetFieldHeroInfo(pMyData->GetMyCharKind());

				if (pFieldHeroInfo)
				{
					// �� ���� ���� ���� ��� �����ϴٰ� ��� �÷���
					if (pFieldHeroInfo->GetSndBoring())
						HashCodePushEffect(pFieldHeroInfo->GetSndBoring(), 0, 0);
				}

				siTalkDealy = 0;
			}
		}
		else
			siTalkDealy = 0;

		// ���� �ִ� ��ġ�� Portal Zone �̸�
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
							// ������ ������ �ִ�. ���� ������ ����.										
							pMyData->SetGotoPortal(TRUE, siPortalCode);

							// ĳ���Ϳ��� ���� ����� �����ش�.
							pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);

							// �� �̻��� ����� �Է��� ���� �ʴ´�.
							pOnlineCharUI->SetAction(FALSE);
						}
					}
				}
			}
		}
	}
	
	// �̸�Ƽ���� ó���Ѵ�.
	pOnlineImoticon->Pool(fLeftUpSwitch);

	if(pOnlineVillage == NULL)
	{
		// ���콺 �ִ� ��ġ�� �� ĳ���Ͱ� ������ ���콺�� �����Ѵ�.
		// ��, �ʵ� �����̿��� �Ѵ�.	
		pOnlineHelp->Initialize();

		pOnlineMsgBoxPay->Process();
		if(pOnlineMsgBoxError->Process() == ON_MSGBOX_RETURN_OK)
		{
			pOnlineClientChatting->Close();
			return ON_MAINPOLL_FORCED_LOGOUT;
		}
	}
	
	DWriteStringToFile("���� 3");

	// robyprk 2004/11/5 18:5
	// ����� �ڽ��� ������ �������� �˻�
	BOOL bIsSiegeWarfareUnit = pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID());

	// ���� ��ư ��Ȱ��ȭ
	pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);

	// robypark 2005/1/17 11:0
	// �ǳ������� ���� ���� ó��
	if (IsInTimeSiegeWarfare())	// �����ð��̶��
	{
		// robypark 2005/1/29 13:32
		// �ʺ��� ���������� ���� �� �� �ִٴ� ��� �޽��� ����
		if (m_sWarningMsgInSiegeWarfare.m_dwLeftTime < dwPrevFrameTimeRequire)
		{
			HDC hdc;
			char szMsg[1024] = {0, };

			// ������ ���ǻ��� �޽��� ���
			// HDC�� �̿��Ͽ� ���� �� ������ ����
			if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			{
				sprintf(szMsg, pOnlineText->Get(8100118), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);

				SetMessage4(szMsg, hdc, -1, 0, RGB(0, 255, 0));

				GETSCREENSURFACE(this)->ReleaseDC(hdc);
			}

			// ä�� â���� �߰�
			pOnlinePannel->AddChat((char*)szMsg, ON_INN_COLOR_GREEN, CHATTINGMODE_NORMAL);

			m_sWarningMsgInSiegeWarfare.m_dwLeftTime = WARNINGMSGINSIEGEWARFARE_DELAY;
		}
		else
			m_sWarningMsgInSiegeWarfare.m_dwLeftTime -= dwPrevFrameTimeRequire;

		// robypark 2005/1/29 14:6
		// ������ ���޼����ΰ�?
		if (KI[pIOnlineChar->GetImjinKind(pMyData->GetMyCharUniqueID())].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// ���޼���(�޵�)�� ��� 10�ʸ��� ������ 10%�� ȸ���ȴ�.
			if (FALSE == m_sRecoveryMana.m_bSendedRequest)
			{
				if (m_sRecoveryMana.m_dwLeftTime < dwPrevFrameTimeRequire)
				{
					m_sRecoveryMana.m_dwLeftTime	 = m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime;
					m_sRecoveryMana.m_bSendedRequest = TRUE;

					// ������ ���� �ڿ����� ��û
					pOnlineClient->SendReuqestChargeMP();
				}
				else
					m_sRecoveryMana.m_dwLeftTime -= dwPrevFrameTimeRequire;
			}
		}


		// ���ΰ� ��忴�ٸ�,
		if (SIEGE_TRANSFORM_MODE_HERO == m_scSiegeTransformMode)
		{
			// �����ϴ� ���� ����� ����ڸ�
			if (pMyData->IsSiegeWarfareDay())
			{
				// ����ڸ� ������ ��� ���ΰ� ĳ���͸� �ǳ������� ���Ž�Ų��.
				pIOnlineChar->AllHeroCharacterTransformTo();

				// �ǳ��� ��� ����
				m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_REFUGEE;
			}
		}

		// ������ ������ �ƴϸ�
		if (FALSE == bIsSiegeWarfareUnit)
		{
			// ���� �����Ѱ�?
			if ((NULL != pMyData->GetGuildName())
				&& (pMyData->CanTransformSiegeWarfareUnit())
				&& (pMyData->IsSiegeWarfareDay())
				&& (FALSE == pOnlinePannel->GetSiegeWarfareTransformUIWnd()->IsSendedMsg()))
			{
				// �� ���̶��
				if (0 == pOnlineMap->m_MapIndex)
				{
					// ���� ��ư Ȱ��ȭ
					pOnlinePannel->SetShowTransformSiegeUnitBtn(TRUE);
				}
				else
				{
					// ������ ���� ������ â�� �ִٸ� �ݱ�
					pOnlinePannel->ShowWindow_SiegeWarfareTransformUIWnd(FALSE);
				}

				// �̴� ���� Ȱ��ȭ?
				if (pOnlineMiniMap->IsActive())
				{
					// �̴ϸ� ���°� ��üȭ���� ���?
					if (pOnlineMiniMap->ISFullMode())
					{
						// ���� ��ư ��Ȱ��ȭ
						pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);
					}
				}
			}
		}
	}
	else
	{
		// �ǳ��� ��忴�ٸ�
		if (SIEGE_TRANSFORM_MODE_REFUGEE == m_scSiegeTransformMode)
		{
			// ����ڸ� ������ ��� �ǳ��� ĳ���͸� ���ΰ� ĳ���ͷ� ���Ž�Ų��.
			pIOnlineChar->AllHeroCharacterTransformTo();

			// ���ΰ� ��� ����
			m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_HERO;
		}

		// ������ ���� ������ â�� �ִٸ� �ݱ�
		pOnlinePannel->ShowWindow_SiegeWarfareTransformUIWnd(FALSE);
	}

	// ���콺 Ŀ�� �Ʒ� ������Ʈ ó��(ǳ�� ���򸻵��� ����)
	if (pOnlineVillage == NULL)
	{
		// robypark 2005/1/26 18:33
		ProcessFieldMouseMove(bIsSiegeWarfareUnit, ptMouse);
	}

	// ������ ��������
	if(pOnlineVillage == NULL)
	{
		if(SIEGEWARFARE_ATTACKMODE_VILLAGE == m_sSiegeWarfare_AttackMode.scAttackMode)
		{
			UI16		ObjectCode;

			// ���� �ð����� ��
			if(IsInTimeSiegeWarfare())
			{
				// Object �� ã�´�.
				ObjectCode = pIOnlineObject->GetVillageObject(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);
				
				if(ObjectCode != 0)
				{
					if(GetVillageWarDayGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))	// �����ϴ� �����̴�.
					{
						BOOL bRangeAttacker = FALSE;		// ���Ÿ�, �ٰŸ� ������ ���� ���� �÷���
						if (TRUE == SiegeWarfare_CanAttackVillage(pMyData->GetMyCharUniqueID(), ObjectCode, bRangeAttacker))
						{
							// �����̸� ����
							pIOnlineObject->GetPos(ObjectCode, &siTX, &siTY);

							// �ٰŸ� ������ ������ ���
							if (FALSE == bRangeAttacker)
							{
								pOnlineCharUI->SetShortDistanceAttackVillage(siTX, siTY);
							}
							// ���Ÿ� ������ ������ ���
							else
							{
								pOnlineCharUI->SetLongDistanceAttackVillage(siTX, siTY);
							}
						}
					}
					else
					{
						// ���� ������ ���
						if (GetVillageEnemyGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))
						{
							// ���� ���������� ���� �� ������ ��ܰ� �������� ����.
							char szErrorMsg[512];
							SVillageData*		lpSVillageData;
							VillageHeader*		lpVillageHeader;

							lpSVillageData	= pOnlineVillageDescriptList->GetVillageDataData(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);
							lpVillageHeader	= pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode);

							if ((lpSVillageData) && (lpVillageHeader))
							{
								// ����� �ִ� ������ ���
								if (NULL != lpSVillageData->m_VillageGuildName[0])
								{
									sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_CANNOT_ATTACK_VILLAGE_WRONG_DAY_GUILD),
												lpSVillageData->m_VillageGuildName, 
												pOnlineText->Get(lpVillageHeader->siVillageName ));
								}
								// ����� ���� ������ ���
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
							// ���� ������ �ƴҰ��
							pIOnlineChar->SetSpeech(pMyData->GetMyCharUniqueID(), pOnlineText->Get(ON_TEXT_ERROR_NOTATTACKVILLAGE));
						}

						// ������ ���� ��� ���� ����
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

				// ������ ���� ��� ����
				m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
				m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			}
		}
	}

	// ������ ĳ���Ͱ� ����
	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if(SIEGEWARFARE_ATTACKMODE_CHARACTER == m_sSiegeWarfare_AttackMode.scAttackMode)	// ĳ���Ͱ��� ���
			{
				if(bCharMove == TRUE)
				{
					// ������ ĳ���Ͱ� ������ �������� �˻�
					if (TRUE == bIsSiegeWarfareUnit)
					{
						// ���� ���� ���� �����̴�. ������ �������� �˻�
						if (TRUE == SiegeWarfare_CanAttackUnit(pMyData->GetMyCharUniqueID(), m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID))
						{
							pOnlineCharUI->SetFieldAttack(m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID);
						}				
					}	
				}	// if(bCharMove == TRUE)
			}	// if (ĳ���� ���� ���)
		}	// if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
	}


	if(pOnlineVillage == NULL)
	{
		if(pMyData->GetMyCharUniqueID() == pOnlineMap->m_uiCurrentFocusCharID)
		{
			if (fRightOneSwitch == 1)
			{
				// ������ ���� ��� ����
				m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
				m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

				if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					// robypark 2005/1/26 18:22
					// �ʵ忡 ���� �� ���콺 Ŀ�� ������ ��ư Ŭ�� ó��
					ProcessFieldMouseRightBtnClick(bIsSiegeWarfareUnit);
				}
			}

			if(IpD.LeftPressSwitch)
			{		
				// �ڽ��� ĳ���͸� �̵���Ų��.
				// ���� ���콺�� ����Ű�� ���� �������� �̵�, ����, ������ ����, �������� �ݴ´� ���� ����� ������.
				if(pOnlineFieldArea->IsFieldArea(IpD.Mouse_X, IpD.Mouse_Y) == TRUE )
				{
					// robypark 2005/1/26 18:22
					// �ʵ忡 ���� �� ���콺 Ŀ�� ���� ��ư Ŭ�� ó��
					ProcessFieldMouseLeftBtnClick(bIsSiegeWarfareUnit, siX, siY);
				}
			}
		}
	}

	pOnlineCharUI->Poll();

	// ����Ű�� ó���Ѵ�.
	FieldShortCutKey();	
 
	if(pOnlineVillage == NULL)
	{
//		m_pNpcTalk->Action();

		m_pQuestInterface->Action();

		m_pChatMacro->Action();

		pOnlineParty->Action();

		// �ǳ�
		pOnlinePannel->Poll();

		// �κ��丮.
		pOnlineInventory->SetFlagVillage(FALSE);
		pOnlineInventory->Action();

		// �ŷ�â
		pOnlineTrade->Action();

		// ���
		pOnlineTradeBook->Poll();


		// ����
		pOnlineBooth->ActionBench();

		// ��
		pOnlineTip->Action();

		// Quest Play
		pOnlineQuestPlay->Poll();

//		//�ʺ��� ����.
//		pOnlineBasicTip->Action();

//		pOnlineBasicQuest->Action();

		

		// ��� 
		pOnlineFieldAuction->Process();

		// ����
		pOnlineReception->Process();

		// ��������
		pOnlineSmith->Poll();

//		pJSTestWnd->Action();

		// ������ ȹ�� â ���� �ൿ�� �Ѵ�. 
		pGame->pItemInfoWindow->Prepare();

		// �޽���
		pOnlineMessenger->Action();

		m_pMercenary->Action();

		if( bFindTrade )
			FindTradeBook();

		// �̴ϸ�
		pOnlineMiniMap->Pool(FALSE);

		// �� ĳ������ ����� ó���Ѵ�.(�̵�, ����, ���� ��, ������ �ݱ�)
		ProcessMyCharOrder();
	}
		
	// ���� �޽����� ������ �����ش�.
	RPI.Send();

	// Resource ����
//	if( timeGetTime() - m_ImageFreeTimer > 5000 )
	{
		pOnlineResource->Prepare();
//		FreeOldImageByTime();

//		m_ImageFreeTimer = timeGetTime();
	}

	// ������� End
	// ������Ʈ �׼�.	
	pIOnlineObject->Action();
	

	// ĳ���� �׼�.		
	pIOnlineChar->Action();		

	// ����ٴϴ� �뺴�� ó��
	pIOnlineChar->ActionByVMercenary();

	pOnlineMap->Action();

	// robypark 2004/10/25 13:28
	// ������ ���� ���� ȿ�� ó��
	m_SiegeWarfare_BulletMgr.Action();
	m_SiegeWarfare_SmokeMgr.Action();

	//����Ʈ �������� ����Ѵ�.
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
	// ȭ�鿡 �׸���.	

	pOnlineFieldEffect->Pool();

	
	DWriteStringToFile("�׸��� 1");
	if(fRender)
	{
		FieldDraw();	
	}
	DWriteStringToFile("�׸��� 2");
	

	if( bExitOnlineGame )
	{
		siMainPollStatus = ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}

	if(pOnlineVillage == NULL)
	{
		// ȯ�漳��
		pOnlineSetting->Action();
	}

	DWriteStringToFile("��");

	return uiNextMenu;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʵ忡�� ȭ�� ��ºκ��� ó���Ѵ�.
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
		// Ÿ��, ĳ����, ������Ʈ�� �׸���.	
		pOnlineMap->Draw(GETSCREENSURFACE(this));	
		
		// robypark 2004/10/25 13:27
		// ������ ���� ȿ�� �׸���
		m_SiegeWarfare_BulletMgr.Draw();
		m_SiegeWarfare_SmokeMgr.Draw();

		pOnlineFieldEffect->Render(GETSCREENSURFACE(this));

		pOnlineMousePoint->Draw();

		pIOnlineChar->AllDrawSpeech(GETSCREENSURFACE(this));

		// NPC ���ϱ�
		m_pNpcTalk->Action();
		m_pNpcTalk->Draw(GETSCREENSURFACE(this));

		// Macro
		m_pChatMacro->Draw(GETSCREENSURFACE(this));
		
		// robypark 2004/11/13 16:37
		// ������ ���� ���� â���� ��� ǥ���ϴ� ��� ó��
		if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
		{
			if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
			{
				// HDC�� �̿��Ͽ� ���� �� ĳ���Ϳ� ���� ������ ����ش�.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// ��� ������ ������ ĳ���͸� �׸���
					pIOnlineChar->AllDrawName(hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);

					if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
					{
						// ��� ������ ���� ü�� �� ���� ������ �׸���
						pIOnlineChar->AllDrawSiegeWarfareUnit_StatusBar();

						// ��� ���� ���� ��� ��� �׸���
						pIOnlineChar->AllDrawSiegeWarfareUnit_GuildFlag();

						clGrp.UnlockSurface(GETSCREENSURFACE(this));
					}
				}	// if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
			}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		}	// if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)

		// ���� ���콺�� ĳ���͸� ����Ű�� ������
		if(m_SelectCharacterIndex != -1)
		{
			// ������ ������ ��� ü�� ������ ����ϱ�
			if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
			{
				// HDC�� �̿��Ͽ� ���� �� ĳ���Ϳ� ���� ������ ����ش�.
				if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
				{
					SetBkMode(hdc, TRANSPARENT);

					hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
					SetTextColor(hdc, RGB(0, 0, 0));
			
					// ������ ���� ĳ���͸� �׸���
					pIOnlineChar->DrawName(m_SelectCharacterIndex, hdc);
	
					SelectObject(hdc, hOldFont);
					GETSCREENSURFACE(this)->ReleaseDC(hdc);
				}

				if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
				{
					// ��� ������ ���� ü�� �� ���� ������ �׸���
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(m_SelectCharacterIndex);

					// ��� ���� ���� ��� ��� �׸���
					pIOnlineChar->DrawGuildFlag(m_SelectCharacterIndex);

					clGrp.UnlockSurface(GETSCREENSURFACE(this));
				}
			}
		}

		// HDC�� �̿��Ͽ� ���� �� ĳ���Ϳ� ���� ������ ����ش�.
		if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);

			hOldFont		=	(HFONT)SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAR2));		
			SetTextColor(hdc, RGB(0, 0, 0));
			
			pIOnlineChar->DrawName(pMyData->GetMyCharUniqueID(), hdc);		

			SelectObject(hdc, hOldFont);
			GETSCREENSURFACE(this)->ReleaseDC(hdc);

			// robypark 2004/11/12 21:28
			// ������ ���� ü�� �� ���� ������ �׸���
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// ������ ���� ü�� �� ���� ������ �׸���
				if (m_scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					pIOnlineChar->DrawSiegeWarfareUnit_StatusBar(pMyData->GetMyCharUniqueID());

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}
		}	
		
		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// ��� ����� ��´�.
			pIOnlineChar->DrawGuildFlag(pMyData->GetMyCharUniqueID());

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}	
	}

	// �ʵ忡�� �ǹ� �۾��� �ϰ� ������
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
				// ���� ����
				clGrp.PutSpriteT(-1 + SignStartPos, 0, lpBoard->Header.Xsize, lpBoard->Header.Ysize, lpBoard->Image);

				clGrp.UnlockSurface(GETSCREENSURFACE(this));
			}

			// ���� �̸��� ������ش�.
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
		// �Ϲ� �����϶�
		// �̴ϸ� â�� Ȱ��ȭ �Ǿ� �ִٸ� ���~~
		if(pOnlineMiniMap->IsActive())
			pOnlineMiniMap->Draw(GETSCREENSURFACE(this));

		pOnlineParty->Draw(GETSCREENSURFACE(this));

		pOnlineTradeBook->Draw( GETSCREENSURFACE(this) );

		// �������â 
		pOnlineReception->Draw(GETSCREENSURFACE(this));

		// ��������
		pOnlineSmith->Draw(GETSCREENSURFACE(this));

		// robypark 2005/2/2 16:18
		// ���� ���� �������̽� Ȱ��ȭ��
		if ((pOnlinePannel->GetSiegeWarfareUnitUIHandle()->IsVisibleWindow())
			|| (pOnlinePannel->GetSiegeWarfareUnitUIHandle()->IsMinimizeWindow()))
		{
			pOnlinePannel->SetFlagVillage(FALSE, ONLINEPANNEL_DISABLEFLAG_CHARINFO/*�뺴â*/);
		}
		else
			pOnlinePannel->SetFlagVillage(FALSE, NULL);

		// �ǳ��� �׸���.
//		pOnlinePannel->SetFlagVillage(FALSE, NULL);
		pOnlinePannel->Draw(GETSCREENSURFACE(this));

		// �ŷ�â�� Ȱ��ȭ �Ǿ� �ֵ���
		if( pOnlineTrade->IsActive() )
		{	
			// �� �κ��丮���� �������� �巡�� ���̶�� �ŷ�â�� ���� ��� �ְ� �κ��丮�� ��� �ش�.
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
			// �뺴 ���� ���			
			m_pMercenary->Draw(GETSCREENSURFACE(this));

			// ����
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

		// �̸�Ƽ���� ����Ѵ�.
		pOnlineImoticon->Draw(GETSCREENSURFACE(this));	

		// ȭ������ �ű��.
		// ������ ȭ�鿡 ����ش�.
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

					// ���� Tile Buffer ����
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

					// ���� ������ ��ü ���
					SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_CHAT ) );
					switch(pMousePointInfo->GetPointKind())
					{
					case ON_MOUSE_POINT_CHAR:
						// ������ ĳ���� ǥ��
						if(pIOnlineCharKI->IsPlayer((UI16)pIOnlineChar->GetKind((UI16)pMousePointInfo->GetParam())))
						{
							// Player����

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

								// ������ ���

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
									// ��� ����� �ִ°��

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d, Flag : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildFlag((UI16)pMousePointInfo->GetParam()));
								}
								else
								{
									// ��� ����� ���°��

									sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\", Guild : \"%s\", Class : %d"
										, (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildName((UI16)pMousePointInfo->GetParam())
										, pIOnlineChar->GetGuildInGrade((UI16)pMousePointInfo->GetParam()));
								}
							}
							else
							{
								// ������ �ƴҰ��

								sprintf((char*)szChatBuffer, "Now Select : Character, UNID : %d, ID : \"%s\"", (UI16)pMousePointInfo->GetParam(), pIOnlineChar->GetCharName((UI16)pMousePointInfo->GetParam()));
							}
						}
						else
						{
							// Monster����

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

					// ���ϴ� ������ Data�� ���´�.
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

		// ���â 
		pOnlineFieldAuction->Draw(GETSCREENSURFACE(this));

		// �ѱ��� �����϶��� ǥ��ȴ�.
//		if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//		{
			if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
			{
				// actdoll (2004/04/02 15:28) : = �̺�Ʈ = ���� ����Ʈ ���� ���ΰ� ǥ��
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

				// ���������� ǥ��
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

		// ���������� ����
		if(m_bReceivedItem)
		{
			if(PtInRect( &m_rtReceivedItem, pt ) == TRUE)
			{
				pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_ITEM_ARRIVED_DESCRIPT),IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
			}
		}

		// �ѱ��� �����϶��� ǥ��ȴ�.
//		if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//		{
//			// actdoll (2004/04/02 15:28) : = �̺�Ʈ = ���� ����Ʈ ���� ���ΰ� ����
//			if(m_bEventDokdoQuestPlay)
//			{
//				if(PtInRect( &m_rtIconDokdoQuest, pt ) == TRUE)
//				{
//					pOnlineHelp->SetText(pOnlineText->Get(ON_TEXT_EVENT_PLAYING_DOKDO_QUEST),IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT, ON_HELP_WHITE, TRUE );
//				}
//			}
//		}



		

		// ���� ���
		SI16 siDrawedHelpPosX = 0;
		SI16 siDrawedHelpPosY = 0;
		int helpx = pOnlineHelp->Show(GETSCREENSURFACE(this), &siDrawedHelpPosX, &siDrawedHelpPosY);

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			XSPR*		lpXSPR;

			// ���� ���콺�� ĳ���͸� ����Ű�� ������
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
				// ������ ������ ��� ü��, ����ǰ ������ ����ϱ�
				if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeHealth)
				{
					// ������ ���� ü�� �������� ���
					XSPR	*psprGaugeHealth = NULL;

					// ����ڿ� ���� ����ΰ�?
					if (IsMemberOfSameGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
					}
					// ���� ��ܿ��ΰ�?
					else if (pIOnlineChar->GetIsHostilityGuild(m_SelectCharacterIndex))
					{
						psprGaugeHealth = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
					}
					else	// �� ���� ��� �ȱ׸���.
						psprGaugeHealth = NULL;

					if (psprGaugeHealth)
					{
						// �׸���
						DrawGaugeCharacter(siDrawedHelpPosX + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosX, 
												siDrawedHelpPosY + m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPosY,
												m_sSiegeWarfareUnitGaugeInfo.siDrawGaugeHealthPct + 1,
												psprGaugeHealth);
					}

					// ������ ���� ���ް������� ����ؾ� �ϴ� �����
					if (m_sSiegeWarfareUnitGaugeInfo.bDrawGaugeSupplyGoods)
					{
						// ������ ���� ����ǰ ������ ���
						XSPR	*psprGaugeSupplyGoodsAmount = pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

						if (psprGaugeSupplyGoodsAmount)
						{
							// �׸���
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

		// �޼��� �ڽ� ���
		pOnlineMsgBox->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxUsePortal->Draw(GETSCREENSURFACE(this));
		pOnlineMsgBoxError->Draw(GETSCREENSURFACE(this));

		// TIP ���
		pOnlineTip->Draw(GETSCREENSURFACE(this));

		// Quest Play
		pOnlineQuestPlay->Draw(GETSCREENSURFACE(this));

		// Quest
		m_pQuestInterface->Draw(GETSCREENSURFACE(this));

//		//�ʺ��� ����
//		pOnlineBasicTip->Draw(GETSCREENSURFACE(this));

		//�ʺ��� ����Ʈ.
//		pOnlineBasicQuest->Draw(GETSCREENSURFACE(this));
		
		
		// ������ ȹ�� â�� �����ش�. 
		pGame->pItemInfoWindow->Render(GETSCREENSURFACE(this));

		// �޽��� ���
		pOnlineMessenger->Draw(GETSCREENSURFACE(this));

		// ������ ��ο�
		pOnlineNumPad->pNumberControl->Draw( GETSCREENSURFACE(this) );

		// ȯ�漳�� ��ο�
		pOnlineSetting->Draw(GETSCREENSURFACE(this));

		//�ؼ����� �߰��κ� 
		// ��� �������� ��ο�
//		pJSWndMgr->Draw(GETSCREENSURFACE(this));

		pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(this));

		if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
		{
			// ���콺�� ��´�.
			pIOnlineMouse->Draw();			

			clGrp.UnlockSurface(GETSCREENSURFACE(this));
		}
		
		// for debug



		

		UpdateScreen(GETSCREENSURFACE(this));
	}

	YetTempTickCount = GetTickCount() - TempTickCount;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	ĳ���Ͱ� ������ ������ ȣ���ϴ� �Լ�.
//----------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::VillagePoll(UI16 uiClientRet)
{
	m_siMsgBoxStatus = pOnlineMsgBox->Process();

	// ����Ű�� ó���Ѵ�.
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
//	����	:	����.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::Free()
{

	fYetLight					=	0.0f;
	m_GameTickCount             =   0;

	bCharMove = TRUE;

	// ���콺�� �Ⱥ��̰� ���ش�.
	ShowSystemCursor(FALSE);		

	// ĳ���� �������̽��� �����Ѵ�.
	if(pIOnlineChar)
	{
		pIOnlineChar->Free();

		delete pIOnlineChar;
		pIOnlineChar = NULL;
	}

	delete m_pMercenary;
	delete m_pChatMacro;
	delete m_pQuestInterface;

	// actdoll (031229)	: �ʻ� ������ ����
	delete m_pPortrait;
	delete m_pNpcTalk;	

	g_FileLoadManager.Free();	// ������ ĳ���� Free()	

	m_cltIGuildClass.Free();	

	// actdoll (2004/11/03 17:19) : IME��Ʈ�ѷ� ����
	OnlineInputStatusControl::Free();

	if(m_ReceivedItemAniSpr.Image)  {clGrp.FreeXspr(m_ReceivedItemAniSpr);  m_ReceivedItemAniSpr.Image  = NULL;}
//	if(m_Spr_Icon_DokdoQuest.Image)  {clGrp.FreeXspr(m_Spr_Icon_DokdoQuest);  m_Spr_Icon_DokdoQuest.Image  = NULL;}

	if(pOnlineLoadingData)
	{
		// actdoll (2004/06/29 12:57) : ���� ó��
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

	// Palette Manager ����
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

	// Timer ����
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
	// ĳ������ ���� �������̽��� �����Ѵ�.
	if(pIOnlineCharKI)
	{
		pIOnlineCharKI->Free();

		delete pIOnlineCharKI;
		pIOnlineCharKI =	 NULL;
	}

	// ������Ʈ ������ �������̽���  �����Ѵ�.
	if(pIOnlineObjectKI)
	{
		pIOnlineObjectKI->Free();

		delete pIOnlineObjectKI;
		pIOnlineObjectKI = NULL;
	}

	// ���� ��ü�� �����Ѵ�.
	if(pOnlineVillage)
	{
		delete pOnlineVillage;
		pOnlineVillage = NULL;
	}

	// ���� ���� List ����
	if(pOnlineVillageDescriptList)
	{
		delete pOnlineVillageDescriptList;
		pOnlineVillageDescriptList = NULL;
	}

	// �������� List ����
	if(pOnlineNoticeList)
	{
		delete pOnlineNoticeList;
		pOnlineNoticeList = NULL;
	}

	// �ʵ�� Effect
	if(pOnlineFieldEffect)
	{
		delete pOnlineFieldEffect;
		pOnlineFieldEffect = NULL;
	}

	// ��ã�� �ΰ������� �����Ѵ�.
	if(pclFindPath)
	{
		delete pclFindPath;
		pclFindPath = NULL;
	}
		
	// �̸�Ƽ��
	if(pOnlineImoticon)
	{
		pOnlineImoticon->Free();

		delete pOnlineImoticon;
		pOnlineImoticon = NULL;
	}

	// ���� ����Ÿ�� �����Ѵ�.
	if(pMyData)
	{	
		pMyData->Free();

		delete pMyData;
		pMyData = NULL;
	}

	// robypark 2005/1/14 17:11
	// ���ΰ� ĳ���͵��� ���� ������ �ʱ�ȭ
	if (m_pFieldHeroInfoMgr != NULL)
	{
		delete m_pFieldHeroInfoMgr;
		m_pFieldHeroInfoMgr = NULL;
	}

	// robypark 2005/1/31 12:55
	// ���� ���� ������
	if (NULL != m_pSiegeUnitTableMgr)
	{
		delete m_pSiegeUnitTableMgr;
		m_pSiegeUnitTableMgr = NULL;
	}

	// ��Ʈ��ũ ������ �����Ѵ�.
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

	// �ʰ����ڸ� �����Ѵ�.
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
		// Config ������ ������.
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

	// ������Ʈ�� �޸𸮸� �����Ѵ�.
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
	//�ؼ����� �߰��κ� 
	
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

	// ������ ���� ��� ���� ����
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
//	����	:	������ ���Ѵ�.
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
//	����	:	�����Ӽ��� ���ؿ´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	cltOnlineWorld::GetTotalFrame()
{
	return uiTotalFrame;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�� ���ǽ��� �����̸Ӹ� ���ǽ��� ���� ��Ų��.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::UpdateScreen(LPDIRECTDRAWSURFACE7 pSurface, BOOL bSourceColorKey,BOOL bForceRedraw)
{
	DWORD					dwTrans;
	LPDIRECTDRAWCLIPPER		pClipper;
	HRESULT hResult =0;

	if(pSurface == NULL)	pSurface	=	clGrp.lpDDSBack;

	// actdoll (2004/11/03 17:28) : IME ��Ʈ�ѷ� ����...
	//	������ ��������� ��ġ�� �߱������̶� ����ڵ带 ����� ���� �� �ۿ� ����.
	OnlineInputStatusControl::Draw( pSurface );

#ifdef	_DEBUG
#ifdef	_SHOW_INPUTSTATUS_
	// actdoll (2004/08/09 14:52) : [�ӽ�] IME �������ͽ� ������
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

	//clGrp.lpDD7->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);	<<-- ���� ����...2003/06/13 23:11

	/*
	if(pSurface == clGrp.lpDDSBack)
	{
		clGrp.lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	}
	else
	*/
	
	if(clGrp.lpDDSPrimary->GetClipper(&pClipper) == DDERR_NOCLIPPERATTACHED)
	{
		// Ŭ���۰� ������ �Ǿ����� �ʴ�.		
		dwTrans	=	DDBLT_WAIT;
		
		if(bSourceColorKey == TRUE)			dwTrans	|=	DDBLT_KEYSRC;
		
		clGrp.lpDDSPrimary->Blt(NULL, pSurface, NULL, dwTrans, NULL);
		/*
		if(clGrp.BltFast(SURFACE_PRIMARY, 0, 0, pSurface, NULL, dwTrans) != DD_OK)
		clGrp.Error("BltFast ����", "");
		*/
		clGrp.Error("BltFast Error", "");
	}
	else
	{	
		// ���� ����... 2003/06/13 23:18
		/*GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );
		*/
		
		dwTrans	=	DDBLT_WAIT;
		
		if(bSourceColorKey == TRUE)			dwTrans	|=	DDBLT_KEYSRC;
		
        if (!WindowModeSwitch) {
			// ���� ����... 2003/06/13 23:18
			/*RECT rc;
			GetWindowRect(pGame->Hwnd, &rc);*/
			hResult = clGrp.lpDDSPrimary->Blt( NULL, pSurface, NULL, dwTrans, NULL);
	
			// ���ǽ��� �ν�Ʈ�� ���̸�, 
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
//	����	:	Ÿ�� ��ǥ�� ����Ͽ� �浵�� ������ ���´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڿ��� ������ ������ ���ڷ� �Ǿ� �ִ� ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltOnlineWorld::GetDirection(CHAR *pszDirection)
{
	SI32	siDirection =	0;

	if(strstr(pszDirection, "EAST") != 0)	siDirection |= ON_EAST;
	if(strstr(pszDirection, "WEST") != 0)	siDirection |= ON_WEST;
	if(strstr(pszDirection, "NORTH") != 0)	siDirection |= ON_NORTH;
	if(strstr(pszDirection, "SOUTH") != 0)	siDirection |= ON_SOUTH;

	return siDirection;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ý��� ���콺�� ���̰� ���ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::ShowSystemCursor(BOOL bShow)
{
	SI32 i = 0;

	if(bShow == TRUE)		
		while(i = ShowCursor(TRUE) < 0);
	else					
		while(i = ShowCursor(FALSE) >= 0);
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�α׾ƿ��� �ϰ� �������� �̵��� �޴��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::SetLogOut(SI32 siNextMenu, SI32 siNextPoll, BOOL bGoGameServer)
{
	siNextMenuAfterLogOut		=	siNextMenu;
	siNextPollAfterLogOut		=	siNextPoll;
	bWillGoGameServer			=	bGoGameServer;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������2 Game ���� ���� ���Ͽ� �����ؾ� �� �͵��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::FreeForGotoImjinGame()
{/*
	// ���ҽ��� �����Ѵ�.
	if(pOnlineResource)
	{
		pOnlineResource->Free();

		delete pOnlineResource;
		pOnlineResource	=	NULL;
	}

	// ���� �޸𸮸� �����Ѵ�.
	if(pOnlineMap)	
	{
		pOnlineMap->Free();

		delete pOnlineMap;
		pOnlineMap	=	NULL;
	}	
*/
	// ĳ������ �޸𸮸� �����Ѵ�.
	if(pIOnlineChar)
	{
//		pIOnlineChar->AllFree();
//		delete pIOnlineChar;
//		pIOnlineChar		=	NULL;
	}

	// ������Ʈ�� �޸𸮸� �����Ѵ�.
	if(pIOnlineObject)
	{
		pIOnlineObject->Free();

		delete pIOnlineObject;
		pIOnlineObject		=	NULL;
	}

	// ��� ������ �����Ѵ�.
	if(g_pSoundManager)
		g_pSoundManager->StopSound( BackgroundMusic );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	cltOnlineWorld::DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// �ɼ��� �ִٸ�, 
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	cltOnlineWorld::DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// �ɼ��� �ִٸ�, 
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	cltOnlineWorld::DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option)
{
	SHORT i;
	char string[20];
	SHORT font;
	SHORT number_xsize	=	pXspr->Header.Xsize;
	SHORT number_ysize	=	pXspr->Header.Ysize;

	// �ɼ��� �ִٸ�, 
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ڸ� ������ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �¶��� �������ΰ�?
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	_clGame::IsOnlineWorld()
{	
	if(pOnlineWorld != NULL)
		return TRUE;
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ȭ�鿡 ����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::DrawVersion(HDC hdc, SI32 siX, SI32 siY)
{
	SetTextColor(hdc, PaletteWhiteColor);
	TextOut(hdc, siX, siY, szOnlineVersion, strlen(szOnlineVersion));
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::ExitOnline()
{
	bExitOnlineGame = TRUE;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ��������� ���̵� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	cltOnlineWorld::GetSoundID()
{
	return BackgroundMusic;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ε��߿� �ؽ�Ʈ�� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȭ ���, ���� ��� ��ȯ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::SetPeace(BOOL bFlag)		// ��ȭ ��� -> ���� ���
{	
	bPeace = bFlag;
	
	ToggleAttackMode(pMyData->GetMyCharUniqueID(), !bPeace); 

	pOnlineClient->SendAttackMode(!bPeace);
}						

VOID	cltOnlineWorld::ToggleAttackMode(UI16 uiAccount, BOOL bAttackMode)
{
	UI16	Equip[WEAR_ITEM_NUM];
	
	// ���� ������ ��� �´�.
	pIOnlineChar->GetEquip(uiAccount, Equip);	
	pIOnlineChar->SetAttackMode(uiAccount, bAttackMode);
	pIOnlineChar->SetEquipItem(uiAccount, Equip);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ε��߿� ���α׷����ٸ� ����Ѵ�.			hojae_append 2002.01.03
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID cltOnlineWorld::ScrollLoading( UI32 uiLoadPercent )
{
	if(clGrp.LockSurface(clGrp.lpDDSBack) == TRUE)
	{
		clGrp.PutSpriteT(0, 0, pOnlineLoadingData->BackImage.Header.Xsize, pOnlineLoadingData->BackImage.Header.Ysize, pOnlineLoadingData->BackImage.Image);
		clGrp.PutSpriteT(0, 22, pOnlineLoadingData->BackOverImage.Header.Xsize, pOnlineLoadingData->BackOverImage.Header.Ysize, pOnlineLoadingData->BackOverImage.Image);
		clGrp.PutSpriteT(553, 22, pOnlineLoadingData->AniLogoImage.Header.Xsize , pOnlineLoadingData->AniLogoImage.Header.Ysize, &pOnlineLoadingData->AniLogoImage.Image[pOnlineLoadingData->AniLogoImage.Header.Start[pOnlineLoadingData->uiFrame]]);
//		clGrp.PutSpriteT(522, 19, pOnlineLoadingData->AniLogoImage.Header.Xsize , pOnlineLoadingData->AniLogoImage.Header.Ysize, &pOnlineLoadingData->AniLogoImage.Image[pOnlineLoadingData->AniLogoImage.Header.Start[pOnlineLoadingData->uiFrame]]);

		// actdoll (2004/09/01 13:36) : ȸ�� �� �̿밡�� ���ɹ����� ���� �ΰ�
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

			// actdoll (2004/09/02 4:23) : ���̼��� �ؽ�Ʈ
			char	*pszLicense;
			if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_GAMANIA;
			else														pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_BOLEHCOM;
			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_ITEM));
			pOnlineMegaText->DrawText( hDC, 286, 564, pszLicense, RGB(250, 250, 250) );


			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_CHAT));
			pOnlineMegaText->DrawText(hDC, 290, 579, pOnlineText->Get(ON_TEXT_LOADING_NOTICE), RGB(250, 250, 250));
			// actdoll (2004/03/03 19:02) : ����� ��忡���� ����� �������� ǥ���Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ο��� �ְ� ���� ��ǰ�� ���� �ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::FindTrade()
{
	bFindTrade	=	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ΰ� �ְ� ���� ��ǰ�� ã�� ���ΰ�?
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineWorld::IsFindNow()
{
	return bFindTrade;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ű
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::SetShortKey( BOOL bFlag )
{
	if(bFlag)
		ShortKey	=	bFlag;
	else if(!pOnlineMessenger->IsAction() || pOnlineMessenger->IsMini())
		ShortKey	=	bFlag;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::FindTradeBook()
{
	// ��� �˻�

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
	// Ŭ���̾�Ʈ ��ü ���ǵ��� üũ
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
			// Type�� 0�̸� �������� ���ǵ��� ó���� ��.
			if(pOnlineClient) pOnlineClient->SendGameTickCount(0, 0);
			return TRUE;
		}
	}

	// 30�ʸ��� Server���� TickCount �޼���
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
				// ������ ���� ��� siResult1 ���� 3���� �Ѿ�� ��쵵 �ִ�. ���� ���ϸ� ���̱� ���� �ִ� 10�� ������...
				for(int i = 0; i < min(siResult1, 10); i++)
				{
					if(pOnlineClient) pOnlineClient->SendGameTickCount(m_GameTickCount + SERVERINFO_TICKCOUNT * (i+1), 1);
				}
				m_GameTickCount += SERVERINFO_TICKCOUNT * siResult1;
			}
		}
	}
	
	// 30�ʸ��� Server���� Time �޼���
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

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	// List�� ������ �⺻���� ���� �ڵ带 �̿��Ѵ�.
	lpVillageHeader = pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(VillageCode);
	if(lpVillageHeader)
	{
		return (SI16)lpVillageHeader->uiNation;
	}

	// �������� ������ �ڸ���~~~!!!
	return ON_COUNTRY_NONE;
}

SI32	cltOnlineWorld::GetVillageGuildFlag(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return -1;
}

float	cltOnlineWorld::GetVillageDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0.0f;
}

BOOL	cltOnlineWorld::GetVillageEnemyGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� �ڽ��� ����� ������ ������ FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// ���� ���� �ڵ尡 List�� ����ִ��� �˻�
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
			// ������ Server�� Data ��û
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

// robypark 2004/12/22 11:50
// ���� �����ϴ� �������� �˻�
BOOL	cltOnlineWorld::GetVillageWarDayGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� �ڽ��� ����� ������ ������ FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// ���� ���� �ڵ尡 List�� ����ִ��� �˻�
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				// �� ������ ������ �ϴ°�?
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_WAR)
					return TRUE;

				else 
					return FALSE;
			}
		}
		else
		{
			// ������ Server�� Data ��û
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

// robypark 2004/10/28 17:29
// ������ ĳ���� �Ҽ� ����� �������� �˻�
// [IN] UI16 VillageCode: ���� �ڵ�
BOOL	cltOnlineWorld::GetVillageOurGuild(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� �ڽ��� ����� ������ ������ FALSE
	if((pMyData->GetGuildName() != NULL) && (pMyData->GetGuildClass() != m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ)))
	{
		// ���� ���� �ڵ尡 List�� ����ִ��� �˻�
		lpSVillageData = pOnlineVillageDescriptList->GetVillageDataData(VillageCode);
		if(lpSVillageData)
		{
			if(lpSVillageData->m_fWait == FALSE)
			{
				// �ڽ��� �Ҽ��� ��� ������ �������� �˻�
				if(lpSVillageData->m_dwGuildRelation == ON_RET_GLD_MINE)
					return TRUE;
				else 
					return FALSE;
			}
		}
		else
		{
			// ������ Server�� Data ��û
			pOnlineClient->SendRequestVillageData(VillageCode);
			pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
		}
	}

	return FALSE;
}

VDEF	cltOnlineWorld::GetVillageDefenceNowDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

VDEF	cltOnlineWorld::GetVillageDefenceMaxDefence(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// ������ �ֵ��� ���� ���� �δ� �� ���
SI32 cltOnlineWorld::GetVillageCurrentGarrisonCount(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// ������ �ֵ��� �ִ� ���� �δ� �� ���
SI32 cltOnlineWorld::GetVillageMaximumGarrisonCount(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
		pOnlineClient->SendRequestVillageData(VillageCode);
		pOnlineVillageDescriptList->AddVillageDataData(VillageCode);
	}

	return 0;
}

// ������ �ݰ� ���� ���� ���. �Ϲ�, ��, �⸧
SI16 cltOnlineWorld::GetvillageCounterAttackType(UI16 VillageCode)
{
	SVillageData*		lpSVillageData;

	// ���� ���� �ڵ尡 List�� ����ִ��� �˻�

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
		// ������ Server�� Data ��û
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
	// �����ð����� �޼����� ���� �ʾ����� ���� ���
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get War Field Map
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 cltOnlineWorld::GetWarFieldMap(void)
{
	if(pOnlineMap)
		return pOnlineMap->GetWarFieldMap();
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� FieldMap�� PK�� �������� �˻�

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL cltOnlineWorld::CheckPKFieldMap(void)
{
	if(pOnlineMap)
		return pOnlineMap->CheckPKFieldMap();
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���������� Flag
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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


// ������ �̸��� ��� �´�.
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


// �¶��� ��� �޽������� Ŭ������ �����͸� ���´�.
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

		// ��Ŷ �б�

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

	// ĳ���� ��� ���۸� �ʱ�ȭ �Ѵ�.
	pOnlineCharUI->Init();

	// ĳ���͸� ��� ���·� �д�		
	if(pMyData->GetMyCharUniqueID() != 0)
	{
		pIOnlineChar->AddOrder(pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);		
	}
	else
	{
		// ���� ĳ���Ͱ� ����.
		pOnlineCharUI->SetAction(FALSE);				// UI Ŭ������ disable ��Ų��.
	}

	// �ʵ�� �̵��ؾ� �Ѵ�.
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
	// ������ �߿� ������� ������� �߰�
	// ������ �ð��̸�, 
	if (IsInTimeSiegeWarfare())
	{
		// ������ �����̰ų�, ���� �������̽� �� �ִٸ�
		if (TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()) 
			|| (TRUE == pMyData->IsInVillageDefenceInterface()))
		{
			// ���� ��������� ���� ���̸�,
			if (TRUE == GetBgmMgr()->IsPlaying())
			{
				// ���� ���� ���� ��������� ������ ��������� �ƴ϶��
				if (OGGBGM_FILENAME_SIEGEBATTLEMUSIC != GetBgmMgr()->GetCurrentBgmIndex())
				{
					//clGrp.Error("��� ���� ����", "������� ���� �� �Դϴ�. ������ ������� �����մϴ�.");
					// ���� ��� ���� ����
					GetBgmMgr()->Play(OGGBGM_FILENAME_SIEGEBATTLEMUSIC, TRUE);
				}
			}
			else	// ���� ���� �ƴ϶��, 
			{
				//clGrp.Error("��� ���� ����", "������ ������� �����մϴ�.");

				// ���� ��� ���� ����
				GetBgmMgr()->Play(OGGBGM_FILENAME_SIEGEBATTLEMUSIC, TRUE);
			}

			// ������ ������� ���� �߿��� ������ ��������� �������� �ʴ´�.
			return;
		}
	}

	if(siTempNation==ON_COUNTRY_NONE) return;

	// ������ ������� ���ְ� �ʿ���� ��� ���� �Ż� ������� ���ָ� �ϵ��� �Ѵ�.
	// ���� ��������� ���� ���̸�,
	if (TRUE == GetBgmMgr()->IsPlaying())
	{
		// ���� ���� ���� ��������� ������ ��������̶��
		if (OGGBGM_FILENAME_SIEGEBATTLEMUSIC == GetBgmMgr()->GetCurrentBgmIndex())
		{
			// ������ ��������� �ٲٵ��� �Ѵ�.
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
	// �������� ���α׷� ����� ���Ѵ�.
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
		pOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "���� ��ũ�� ����", "siResult : %d", siResult);
	}*/
}

BOOL	cltOnlineWorld::IsNight()
{
	SOnlineTime				NowTime;
	
	// ���� �ð��� ���´�.
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
// ������ ������ �� ��� ������ ���ֿ� ���� ������ �������� �˻� �Լ�
// UI16 uiAttackerUniqueID: �������� UniqueID
// UI16 uiTargetUniqueID: �ǰ����� UniqueID
BOOL cltOnlineWorld::SiegeWarfare_CanAttackUnit(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID)
{
	// �����ϴ� ��ǥ ĳ�����ΰ�?
	if (FALSE == pIOnlineChar->IsExistUniqueID(uiTargetUniqueID))
	{
		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// ���� �ð����� ��
	if(FALSE == IsInTimeSiegeWarfare())
	{
		char szErrorMsg[256];

		sprintf(szErrorMsg, pOnlineText->Get(ON_TEXT_SIEGEWARFARE_TIME_IS), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END); 
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, szErrorMsg);

		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;

		return FALSE;
	}

	// �� ���� �ƴ� ��� �Ұ���
	if (pOnlineMap->m_MapIndex != 0)
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));

		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}
/*
	// ������ ĳ���Ͱ� ����ִ°�?
	if (0 >= pIOnlineChar->GetSiegeWarfare_Hp(uiAttackerUniqueID))
	{
		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}

	// �ǰ��� ĳ���Ͱ� ����ִ°�?
	if (0 >= pIOnlineChar->GetSiegeWarfare_Hp(uiTargetUniqueID))
	{
		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}

	// ������ ĳ���Ͱ� ���� �������̽��� �ִ°�?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
	{
		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}
*/
	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// ������ ĳ���Ͱ� ������ ���� ������ ������ ĳ�����ΰ�?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "������ ������ �������� ���ϴ� �����̴�.");

		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	/*
	// �ǰ��� ĳ���Ͱ� ���� �������̽��� �ִ°�?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
	{
		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		return FALSE;
	}
	*/

	// �ǰ��ڰ� ������ ������ �ƴ϶�� ���� �Ұ���
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "������ ������ �ƴϴ�.");

		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// ���� ������� �˻�
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// ������ ��ġ
	SI16 siPosition_AttakerX, siPosition_AttakerY;
	// �ǰ��� ��ġ
	SI16 siPosition_TargetX, siPosition_TargetY;

	// ������ ĳ���Ͱ� ���Ÿ� ������ ���, �� �ٰŸ� ������ �Ұ����� ��� �˻�
	// ���Ÿ� ���� �Ǵ��� ĳ���� �Ӽ��� ATB_MINRANGE�� ���� ��� ���Ÿ��� �Ǵ��Ѵ�.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// ������ �� �ǰ��� ĳ���� ��ġ ���
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

		// �ǰ��� ĳ���Ϳ� �Ÿ��� �ּҰŸ� �ȿ� �ִ��� �˻�
		if (ManContactvillage(siPosition_TargetX, siPosition_TargetY, 1, 1, siPosition_AttakerX, siPosition_AttakerY, KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
//			pIOnlineChar->SetSpeech(uiAttackerUniqueID, "������ �Ÿ��� ������� ������ �Ұ����ϴ�.");

			// ���� ��� ����
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
	}
	else
	{
		/*
		// �ٰŸ� �ڵ� ���� Ƚ�� ����
		if (m_sSiegeWarfare_AttackMode.siAutoAttackTimes >= 3)
		{
			// ���� ��� ����
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
		*/
	}


	/*
	// ���� �������� 0�� ��� ���� �Ұ���
	if (0 >= pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiAttackerUniqueID))
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_SPEECH_CANNOT_ATTACK_RANOUT_SUPPLYGOODS));

		// ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiCharacterAttackUniqueID = 0;

		return FALSE;
	}
	*/

	// ���� �����ð��� ����Ǿ����� �˻�
	if (pIOnlineChar->GetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "���� �����ð��� ������� �ʾҴ�.");

		return FALSE;
	}

	// ���� �����ð��� ����Ͽ����Ƿ�, �����ϴ� ���� �ð��� ����
	pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID, KI[uiKindInfoIdx_Attacker].GetAttackDelay(0));

	// ���� ����
	return TRUE;
}

// robypark 2005/1/8 13:16
// ������ ������ ������ �������� �˻� �Լ�
// UI16 uiTargetUniqueID: �ǰ����� UniqueID
BOOL	cltOnlineWorld::SiegeWarfare_CheckCanAttackUnit(UI16 uiTargetUniqueID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// ������ ĳ���Ͱ� ������ ���� ������ ������ ĳ�����ΰ�?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
		return FALSE;
	}
/*
	// �ǰ��� ĳ���Ͱ� ���� �������̽��� �ִ°�?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
		return FALSE;
*/

	// �ǰ��ڰ� ������ ������ �ƴ϶�� ���� �Ұ���
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
		return FALSE;
	}

	// ���� ������� �˻�
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		return FALSE;
	}

	// ���� ����
	return TRUE;
}

// robypark 2005/1/12 11:3
// ������ ������ ������ �������� �˻��Ͽ� ���콺 Ŀ�� ������ ���� ���콺 Ÿ�� ���
// UI16 uiTargetUniqueID: �ǰ����� UniqueID
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanAttackUnit(UI16 uiTargetUniqueID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

/*
	// �ǰ��� ĳ���Ͱ� ���� �������̽��� �ִ°�?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiTargetUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;
*/
	// ����� ĳ���Ͱ� ���� �������̽��� �ִ°�?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
		return ON_MOUSE_BATTLE_SUB_NONE;

	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	BOOL bRangeAttacker = FALSE;	// ���Ÿ� ���� �����ΰ�?

	// ������ ĳ���Ͱ� ���Ÿ� ������ ���, �� �ٰŸ� ������ �Ұ����� ��� �˻�
	// ���Ÿ� ���� �Ǵ��� ĳ���� �Ӽ��� ATB_MINRANGE�� ���� ��� ���Ÿ��� �Ǵ��Ѵ�.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		bRangeAttacker = TRUE;	// ���Ÿ� ���� ����
	}

	// ������ ĳ���Ͱ� ������ ���� ������ ������ ĳ�����ΰ�?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;
	}

	/*
	// �ǰ��ڰ� ������ ������ �ƴ϶�� ���� �Ұ���
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiTargetUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;

		// �����ڰ� ���Ÿ� ���� �����ΰ�?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ���Ÿ� ���� �Ұ���
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// �ٰŸ� ���� �Ұ���
	}
	*/

	// ���� ������� �˻�
	if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiTargetUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;
		// �����ڰ� ���Ÿ� ���� �����ΰ�?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ���Ÿ� ���� �Ұ���
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// �ٰŸ� ���� �Ұ���
	}

	// �����ڰ� ���Ÿ� ���� �����ΰ�?
	if (bRangeAttacker)
	{
		// ������ ��ġ
		SI16 siPosition_AttakerX, siPosition_AttakerY;
		// �ǰ��� ��ġ
		SI16 siPosition_TargetX, siPosition_TargetY;

		// ������ �� �ǰ��� ĳ���� ��ġ ���
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

		// �ǰ��� ĳ���Ϳ� �Ÿ��� �ּҰŸ� �ȿ� �ִ��� �˻�
		if (ManContactvillage(siPosition_TargetX, siPosition_TargetY, 1, 1, siPosition_AttakerX, siPosition_AttakerY, KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ���Ÿ� ���� �Ұ���
		}

		return ON_MOUSE_BATTLE_SUB_LONGRANGE;	// ���Ÿ� ���� ����
	}

	return ON_MOUSE_BATTLE_SUB_SHORTRANGE;	// �ٰŸ� ���� ����
}

// robypark 2004/10/28 16:21
// ������ ������ ������ ������ ���� �������� �˻�
// [IN]  UI16 uiAttackerUniqueID: �������� UniqueID
// [IN]  UI32 siObjectID		: ���� ��� ������Ʈ(����) ID
// [OUT] BOOL &bRangeAttacker	: �����ڰ� ���Ÿ� ���� �����ΰ�?
BOOL cltOnlineWorld::SiegeWarfare_CanAttackVillage(UI16 uiAttackerUniqueID, SI32 siObjectID, BOOL &bRangeAttacker)
{
	// �� ���� �ƴ� ��� �Ұ���
	if (pOnlineMap->m_MapIndex != 0)
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));
		// ������ ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// ������ ��ġ
	SI16 siPosition_AttakerX, siPosition_AttakerY;

	// ���� ��ġ, ũ��
	SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

	// �����ڴ� �ٰŸ� ���� �����̴�.
	bRangeAttacker = FALSE;

	// �����ڰ� ������ ������ �ƴ϶�� ���� �Ұ���
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "������ ������ �ƴϴ�.");
		// ������ ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// ������ ĳ���Ͱ� ���� ������ ������ ĳ�����ΰ�?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "���� ������ �Ұ����� ������ �����̴�.");
		// ������ ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	// ������ ĳ���Ͱ� ���Ÿ� ������ ���, �� �ٰŸ� ������ �Ұ����� ��� �˻�
	// ���Ÿ� ���� �Ǵ��� ĳ���� �Ӽ��� ATB_MINRANGE�� ���� ��� ���Ÿ��� �Ǵ��Ѵ�.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// �����ڴ� ���Ÿ� ���� �����̴�.
		bRangeAttacker = TRUE;

		// ������ �� �ǰ��� ĳ���� ��ġ ���
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		
		// ������Ʈ�� �� ��ġ�� ���Ѵ�.
		pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

		// ������Ʈ�� ����� ���Ѵ�.
		pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

		// ������ �Ÿ��� �ּҰŸ� �ȿ� �ִ��� �˻�
		if(ManContactvillage(siVillageX - siVillageXsize / 2, siVillageY - siVillageYsize / 2, // position village
								siVillageXsize, siVillageYsize, // range
								siPosition_AttakerX, siPosition_AttakerY,	// position attacker
								KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
//			pIOnlineChar->SetSpeech(uiAttackerUniqueID, "���� ������ �ϱ⿡�� �Ÿ��� �ʹ� ������.");
			// ������ ���� ��� ����
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}

	}
	else
	{
		/*
		// ���� ���� ���� �ڵ� ���� Ƚ�� ����
		if (m_sSiegeWarfare_AttackMode.siAutoAttackTimes >= 3)
		{
			// ������ ���� ��� ����
			m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
			m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
			m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
			return FALSE;
		}
		*/
	}

	// �����ϴ� ���� �����ΰ�?
	if (FALSE == GetVillageWarDayGuild(m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode))
	{
		// ������ ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
		return FALSE;
	}

	/*
	// ���� �������� 0�� ��� ���� �Ұ���
	if (0 >= pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiAttackerUniqueID))
	{
		pIOnlineChar->SetSpeech(uiAttackerUniqueID, pOnlineText->Get(ON_TEXT_SPEECH_CANNOT_ATTACK_RANOUT_SUPPLYGOODS));
		// ������ ���� ��� ����
		m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
		m_sSiegeWarfare_AttackMode.uiVillageAttackVillageCode = 0;
		return FALSE;
	}
	*/

	// ���� �����ð��� ����Ǿ����� �˻�
	if (pIOnlineChar->GetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID))
	{
//		pIOnlineChar->SetSpeech(uiAttackerUniqueID, "���� �����ð��� ������� �ʾҴ�.");
		return FALSE;
	}

	// ���� �����ð��� ����Ͽ����Ƿ�, �����ϴ� ���� �ð��� ����
	pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(uiAttackerUniqueID, KI[uiKindInfoIdx_Attacker].GetAttackDelay(0));

	// ���� ����
	return TRUE;
}

// robypark 2005/1/8 13:16
// ������ ������ ������ ������ ���� �������� �˻�
BOOL	cltOnlineWorld::SiegeWarfare_CheckCanAttackVillage(void)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// �����ڰ� ������ ������ �ƴ϶�� ���� �Ұ���
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
		return FALSE;
	}

	// ������ ĳ���Ͱ� ���� ������ ������ ĳ�����ΰ�?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
		return FALSE;
	}

	// ���� ����
	return TRUE;
}

// robypark 2005/1/12 11:3
// ������ ������ ������ ������ ���� �������� �˻��Ͽ� ���콺 Ŀ�� ������ ���� ���콺 Ÿ�� ���
SI16	cltOnlineWorld::SiegeWarfare_CheckCursorCanAttackVillage(SI32 siObjectID)
{
	UI16 uiAttackerUniqueID  = pMyData->GetMyCharUniqueID();

	// �����ڰ� ������ ������ �ƴ϶�� ���� �Ұ���
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiAttackerUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;	// ���� �Ұ���
	}

	// ����� ĳ���Ͱ� ���� �������̽��� �ִ°�?
	if (TRUE == pIOnlineChar->IsInVillageDefenceInterface(uiAttackerUniqueID))
	{
		return ON_MOUSE_BATTLE_SUB_NONE;	// ���� �Ұ���
	}

	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// �����ڴ� ���Ÿ� ���� �����ΰ�?
	BOOL bRangeAttacker = FALSE;

	// ������ ĳ���Ͱ� ���Ÿ� ������ ���, �� �ٰŸ� ������ �Ұ����� ��� �˻�
	// ���Ÿ� ���� �Ǵ��� ĳ���� �Ӽ��� ATB_MINRANGE�� ���� ��� ���Ÿ��� �Ǵ��Ѵ�.
	if (TRUE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_MINRANGE))
	{
		// �����ڴ� ���Ÿ� ���� �����̴�.
		bRangeAttacker = TRUE;
	}

	// ������ ĳ���Ͱ� ���� ������ ������ ĳ�����ΰ�?
	if (FALSE == KI[uiKindInfoIdx_Attacker].IsAttribute1(ATB_ATTACK_VILLAGE))
	{
		// �����ڰ� ���Ÿ� ���� �����ΰ�?
		if (bRangeAttacker)
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ���Ÿ� ���� �Ұ���
		else
			return ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE;	// �ٰŸ� ���� �Ұ���
	}

	// �����ڰ� ���Ÿ� ���� �����ΰ�?
	if (bRangeAttacker)
	{
		// ������ ��ġ
		SI16 siPosition_AttakerX, siPosition_AttakerY;

		// ���� ��ġ, ũ��
		SI16		siVillageX, siVillageY,	siVillageXsize, siVillageYsize;	

		// ������ �� �ǰ��� ĳ���� ��ġ ���
		pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
		
		// ������Ʈ�� �� ��ġ�� ���Ѵ�.
		pIOnlineObject->GetPos(siObjectID, &siVillageX, &siVillageY);

		// ������Ʈ�� ����� ���Ѵ�.
		pIOnlineObject->GetSize(siObjectID, &siVillageXsize, &siVillageYsize);					

		// ������ �Ÿ��� �ּҰŸ� �ȿ� �ִ��� �˻�
		if(ManContactvillage(siVillageX - siVillageXsize / 2, siVillageY - siVillageYsize / 2, // position village
								siVillageXsize, siVillageYsize, // range
								siPosition_AttakerX, siPosition_AttakerY,	// position attacker
								KI[uiKindInfoIdx_Attacker].GetMinAttackRange(0)) == TRUE)
		{
			return ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE;	// ���Ÿ� ���� �Ұ���
		}

		return ON_MOUSE_BATTLE_SUB_LONGRANGE;	// ���Ÿ� ���� ����
	}

	return ON_MOUSE_BATTLE_SUB_SHORTRANGE;	// �ٰŸ� ���� ����
}

// robypark 2004/11/1 16:20
// �ڽ��� �ֺ��� ���� ������ �ִ��� �˻�
// return value : ������ ������ ���� Ÿ��.
//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - ����ȿ��(���ݷ� ��)�� ������ �޴´�.
//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - ���� �����̴�.
//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - �Ϲ� ������ ������ �Ѵ�.
//				-1									   - ��ȿ���� ���� �����̴�.
SI16 cltOnlineWorld::SiegeWarfare_IsInAreaGeneralEffect(void)
{
	// �ڽ��� ĳ���� Unique ID ���
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// �ڽ��� ĳ���Ͱ� ������ ������ �ƴ� ���
	SI32 siKindImjinIdx_Attacker = pIOnlineChar->GetImjinKind(uiMyCharacterUniqueID);
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiMyCharacterUniqueID))
		return -1;

	// ������ ���� ������ ���
	if (KI[siKindImjinIdx_Attacker].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		return -1;

	// �ڽ��� ĳ���� ��ǥ
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// ������ ����ȿ��(���ݷ� ��) ����ȵ�
	if(pIOnlineChar->GetCaptain(uiMyCharacterUniqueID))
	{
		// ���� Ÿ�� ����
		// ������ �����̴�.
		return ON_ATTACK_TYPE_CAPTAIN;
	}

	// �ڽ��� ��ġ ã��
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// �˻� ����
	SI16 ssTileX, ssTileY;

	// 5 X 5 Ÿ���̳��� ������ �ִ��� ã��
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY++)
		{
			// �˻��� Ÿ�� ���
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// �ʿ� ������ ĳ���� ID ã��
			UI16 uiCharacterID = pOnlineMap->GetCharID(siTargetX, siTargetY);

			// �ش� ��ġ�� ĳ���Ͱ� ����
			if (0 != uiCharacterID)
			{
				// ã�� ĳ������ Unique ID ���
				UI16 uiCharacterUniqueID = pIOnlineChar->GetUniqueID(uiCharacterID);

				// ������ �������� �˻�
				if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiCharacterUniqueID)) && (uiMyCharacterUniqueID != uiCharacterUniqueID))
				{
					// ����̰ų� ��� ĳ�������� �˻�. ��, ���� �������� �˻�, ���� �������̽� �� ���� �ʴ�.
					if ((pIOnlineChar->GetCaptain(uiCharacterUniqueID)) && (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiCharacterUniqueID)))
					{
						// ���� ������� �˻�
						if (IsMemberOfSameGuild(uiCharacterUniqueID))
						{
							// ���� Ÿ�� ����
							// ����ȿ��(���ݷ� ��) ���� �ȿ� �����Ѵ�.	
							return ON_ATTACK_TYPE_NEARCAPTAIN;
						}
					}
				}
			}	// if (0 != uiCharacterID)
		}	// for (ssTileY = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY++)
	}	// for (ssTileX = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX++)

	// ����ȿ��(���ݷ� ��) ���� ���� �ʴ´�.
	return ON_ATTACK_TYPE_NORMAL;
}

// robypark 2004/11/8 16:23
// Ư�� ĳ������ �ֺ��� ���� ������ �ִ��� �˻�
// [IN] UI16 uiUniqueID: ���Ϸ��� ĳ���� Unique ID
// return value : ������ ������ ���� Ÿ��.
//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - ����ȿ��(���ݷ� ��)�� ������ �޴´�.
//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - ���� �����̴�.
//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - �Ϲ� ������ ������ �Ѵ�.
//				-1									   - ��ȿ���� ���� �����̴�.
SI16 cltOnlineWorld::SiegeWarfare_IsInAreaGeneralEffect(UI16 uiUniqueID)
{
	// ���� �ð����� ��
	if(FALSE == IsInTimeSiegeWarfare())
	{
		return -1;
	}

	// �� ���� �ƴ� ��� �Ұ���
	if (pOnlineMap->m_MapIndex != 0)
		return -1;

	// ĳ���Ͱ� ������ ������ �ƴ� ���
	SI32 siKindImjinIdx_Attacker = pIOnlineChar->GetImjinKind(uiUniqueID);
	if (FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID))
		return -1;

	// ������ ���� ������ ���
	if (KI[siKindImjinIdx_Attacker].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		return -1;

	// ĳ���� ��ǥ
	SI16 siCharacterPositionX, siCharacterPositionY;

	// ������ ����ȿ��(���ݷ� ��) ����ȵ�
	if(pIOnlineChar->GetCaptain(uiUniqueID))
	{
		// ���� Ÿ�� ����
		// ������ �����̴�.
		return ON_ATTACK_TYPE_CAPTAIN;
	}

	// �ڽ��� ��ġ ã��
	pIOnlineChar->GetPos(uiUniqueID, &siCharacterPositionX, &siCharacterPositionY);

	// �˻� ����
	SI16 ssTileX, ssTileY;

	// 5 X 5 Ÿ���̳��� ������ �ִ��� ã��
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range; ssTileY++)
		{
			// �˻��� Ÿ�� ���
			SI16 siTargetX = siCharacterPositionX + ssTileX;
			SI16 siTargetY = siCharacterPositionY + ssTileY;

			// �ʿ� ������ ĳ���� ID ã��
			UI16 uiCharacterID = pOnlineMap->GetCharID(siTargetX, siTargetY);

			// �ش� ��ġ�� ĳ���Ͱ� ����
			if (0 != uiCharacterID)
			{
				// ã�� ĳ������ Unique ID ���
				UI16 uiCharacterUniqueID = pIOnlineChar->GetUniqueID(uiCharacterID);

				// ������ �������� �˻�
				if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(uiCharacterUniqueID)) && (uiUniqueID != uiCharacterUniqueID))
				{
					// ����̰ų� ��� ĳ�������� �˻�. ��, ���� �������� �˻�, ���� �������̽� �� ���� �ʴ�.
					if ((pIOnlineChar->GetCaptain(uiCharacterUniqueID)) && (FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiCharacterUniqueID)))
					{
						// ���� ������� �˻�
						if (IsMemberOfSameGuild(uiUniqueID, uiCharacterUniqueID))
						{
							// ���� Ÿ�� ����
							// ����ȿ��(���ݷ� ��) ���� �ȿ� �����Ѵ�.	
							return ON_ATTACK_TYPE_NEARCAPTAIN;
						}
					}	// if ((pIOnlineChar->GetGuildInGrade(uiCharacterUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) || (pIOnlineChar->GetGuildInGrade(uiCharacterUniqueID) == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)))
				}	// if (TRUE == KI[TransKindOnlineToImjin(pIOnlineChar->GetKind(uiCharacterUniqueID))].IsKindAtb(KINDATB_SIEGE_WARFARE_UNIT))
			}	// if (0 != uiCharacterID)
		}	// for (ssTileY = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileY++)
	}	// for (ssTileX = -SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX <= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE; ssTileX++)

	// ����ȿ��(���ݷ� ��) ���� ���� �ʴ´�.
	return ON_ATTACK_TYPE_NORMAL;
}

// robypark 2004/11/1 16:22
// ������ �������� �����ϰ��� �� �� �ڽ� �ֺ��� ������ ������ ���� �ȿ� �ִ��� �˻�
// Return Value: TRUE  - �� ��� ���� ���� �ȿ� ĳ���Ͱ� �����ϰų� �� ���� �ƴϴ�.
//				 FALSE - ���� ������ ���� ĳ���Ͱ� �����Ѵ�.
BOOL cltOnlineWorld::SiegeWarfare_IsInAreaDisabledTransform(void)
{
	// ���� �ð����� ��
	if(FALSE == IsInTimeSiegeWarfare())
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_SIEGEWARFARE_TIME_IS), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END); 
		return TRUE;
	}

	// ���� �����
	if(pMyData->IsWaitBattleResponse())
		return TRUE;

	// ������ ���� ���̰ų�, ���� ���� ���� ��� ���̶��
	if ((pMyData->IsGotoVillage()) && (pMyData->IsWaitGotoVillageResponse() == TRUE))
		return TRUE;

	// �� ���� �ƴ� ��� ���� �Ұ���
	if (pOnlineMap->m_MapIndex != 0)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_DUNGEON));
		return TRUE;
	}

	// ĳ���Ͱ� ���� �ȿ� �ִ� ���
	if (pOnlineVillage != NULL)
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_IN_VILLAGE));
		return TRUE;
	}

	// ĳ���Ͱ� ���� �������̽� �ȿ� �ִ� ���
	if (pMyData->IsInVillageDefenceInterface())
		return TRUE;

	// ����� ���� ���
	if (NULL == pMyData->GetGuildName())
		return TRUE;

	// �̹� ������ ������ ��� ���� �������� ���� ������ �� ����.
	if (NULL != m_pSiegeUnitTableMgr->GetSiegeUnitTable(pMyData->GetMyCharKind()))
		return TRUE;

	// �ڽ��� �ֺ��� ������ ������ ���� �ȿ� �ִ��� �˻�
	if (TRUE == SiegeWarfare_IsInAreaHostileVillage())
	{
		pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "",
								pOnlineText->Get(ON_TEXT_CANNOT_TRANSFORM_NEAR_HOSTILITY_VILLAGE));

		// ���� �Ұ���
		return TRUE;
	}

	// ���� ����
	return FALSE;
}

// robypark 2005/1/13 21:39
// �ڽ� �ֺ��� ������ ������ ���� �ȿ� �ִ��� �˻�
// Return Value: TRUE  - �� ��� ���� ���� �ȿ� ĳ���Ͱ� ����.
//				 FALSE - ��ó�� ���� ����� ���� ���� ĳ���Ͱ� �����Ѵ�.
BOOL	cltOnlineWorld::SiegeWarfare_IsInAreaHostileVillage(void)
{
	// �ڽ��� ĳ���� Unique ID ���
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// �ڽ��� ĳ���� ��ġ
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// �ڽ��� ��ġ ã��
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// �˻� ����
	SI16 ssTileX, ssTileY;

	// 21 X 21 Ÿ���̳��� �� ��� �Ҽ� ������ �ִ��� ã��
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY++)
		{
			// �˻��� Ÿ�� ���
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// �ش� ��ǥ�� �ʿ� �ִ� ������ƮID ���
			SI16 siObjectID = pOnlineMap->GetObjectID(siTargetX, siTargetY);

			// ��ȿ�Ѱ�?
			if (siObjectID != 0)
			{
				// ������Ʈ�� �������� �˾ƺ���.
				if (pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					// ���� �ڵ带 ���´�.
					UI16 uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

					// ��ȿ�� ���� �ڵ�����?
					if (0 != uiVillageCode)
					{
						// �� ���(�����ϴ� ��) ��������?
						if (GetVillageWarDayGuild(uiVillageCode))
						{
							// �� ��� ���� �����ȿ� �ִ�.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// ���� ��� ���� ���� ��
	return FALSE;
}

// robypark 2005/1/13 21:39
// �ڽ��� �ֺ��� �Ʊ���� ������ ���� �ȿ� �ִ��� �˻�
// Return Value: TRUE  - �Ʊ� ��� ���� ���� �ȿ� ĳ���Ͱ� ����.
//				 FALSE - ��ó�� �Ʊ� ����� ���� ���� ĳ���Ͱ� �����Ѵ�.
BOOL	cltOnlineWorld::SiegeWarfare_IsInAreaOurVillage(void)
{
	// �ڽ��� ĳ���� Unique ID ���
	UI16 uiMyCharacterUniqueID = pMyData->GetMyCharUniqueID();

	// �ڽ��� ĳ���� ��ġ
	SI16 siMyCharacterPositionX, siMyCharacterPositionY;

	// �ڽ��� ��ġ ã��
	pIOnlineChar->GetPos(uiMyCharacterUniqueID, &siMyCharacterPositionX, &siMyCharacterPositionY);

	// �˻� ����
	SI16 ssTileX, ssTileY;

	// 21 X 21 Ÿ���̳��� �Ʊ� ��� �Ҽ� ������ �ִ��� ã��
	for (ssTileX = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileX++)
	{
		for (ssTileY = -m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY <= m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range; ssTileY++)
		{
			// �˻��� Ÿ�� ���
			SI16 siTargetX = siMyCharacterPositionX + ssTileX;
			SI16 siTargetY = siMyCharacterPositionY + ssTileY;

			// �ش� ��ǥ�� �ʿ� �ִ� ������ƮID ���
			SI16 siObjectID = pOnlineMap->GetObjectID(siTargetX, siTargetY);

			// ��ȿ�Ѱ�?
			if (siObjectID != 0)
			{
				// ������Ʈ�� �������� �˾ƺ���.
				if (pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
				{
					// ���� �ڵ带 ���´�.
					UI16 uiVillageCode = pIOnlineObject->GetVillageCode(siObjectID);

					// ��ȿ�� ���� �ڵ�����?
					if (0 != uiVillageCode)
					{
						// �Ʊ� ��� ��������?
						if (GetVillageOurGuild(uiVillageCode))
						{
							// �Ʊ� ��� ���� �����ȿ� �ִ�.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// �Ʊ� ��� ���� ���� ��.
	return FALSE;
}

// robypark 2005/1/13 21:45
// ������ ���صǴ� �Ϲ� �������� ���� �ʺ��� ���������� ������ų �� �ִ��� �˻�
BOOL	cltOnlineWorld::SiegeWarfare_CheckAbledWarfRefugee(UI16 uiDestUniqueID)
{
	// ������ �ð��̶��
	if (IsInTimeSiegeWarfare())
	{
		if ((TRUE == pIOnlineChar->IsSiegeWarfareUnit(pMyData->GetMyCharUniqueID()))		// ����ڰ� ������ �����̶��
			&& ( FALSE == pIOnlineChar->IsSiegeWarfareUnit(uiDestUniqueID) )			// ����� ������ ������ �ƴϸ�,
			&& ( TRUE == pIOnlineCharKI->IsPlayer( (UI16)pIOnlineChar->GetKind(uiDestUniqueID) ) )	// ����� �÷��̾��̸�,
			&& ( FALSE == pIOnlineChar->IsInVillageDefenceInterface(uiDestUniqueID) ) 	// ����� ���� �������̽��� �ִ� ĳ���Ͱ� �ƴϸ�,
			&& ( FALSE == pIOnlineChar->GetBattleFlag(uiDestUniqueID) )	)				// ����� ���� ������� ĳ���Ͱ� �ƴϸ�,
		{
			// ����� ��ܳ� ������ ���, �����, ����� ���
			if ((pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				|| (pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
				|| (pMyData->GetGuildClass() == m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)))
			{
				// ���� ��ܿ��� �ƴ϶��
				if (FALSE == IsMemberOfSameGuild(uiDestUniqueID))
				{
					// ���� ��ܿ��� �ƴ϶��,
					//if (FALSE == pIOnlineChar->GetIsHostilityGuild(uiDestUniqueID))
					{
						if ((TRUE == SiegeWarfare_IsInAreaOurVillage())				// ����ڰ� �Ʊ� ��� ���� ��ó�̰ų�,
							|| (TRUE == SiegeWarfare_IsInAreaHostileVillage()))		// ���� ��ܸ��� ��ó���
						{
							// ���� ��ų �� �ִ�.
							return TRUE;
						}
					}
				}
			}
		}
	}

	// ���� ��ų �� ����.
	return FALSE;
}

// robypark 2004/11/2 15:0
// ������ ���� ���� �Ӽ��� ���� ȿ�� ����ϱ�
void	cltOnlineWorld::SiegeWarfare_CreateAttackEffectCharacter(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID, bool bNeedToSendActionMsg/* = false*/)
{
	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	SI32 siEmptySmokeID = 0;

	// ������ ��ġ
	SI16 siPosition_AttakerX, siPosition_AttakerY;
	// �ǰ��� ��ġ
	SI16 siPosition_TargetX, siPosition_TargetY;

	// ������ �� �ǰ��� ĳ���� ��ġ ���
	pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);
	pIOnlineChar->GetPos(uiTargetUniqueID,	 &siPosition_TargetX, &siPosition_TargetY);

	// ĳ���� ���� �Ӽ��� ���� ȿ�� ����
	switch (KI[uiKindInfoIdx_Attacker].GetAttackAtb(0))
	{
		// ���� �˺�, �⸶�� ����
		case ATTACKATB_SWORD:
			siEmptySmokeID = m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_BLADEHIT"));

			// robypark 2005/1/14 18:52
			// ��ǥ ĳ���� �߽ɿ� �׷������� ����
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SWORD, 0, 0);
			break;

		// ���� â�� ����
		case ATTACKATB_LONG_SPEAR:
			siEmptySmokeID = m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_SPEARHIT"));

			// robypark 2005/1/14 18:52
			// ��ǥ ĳ���� �߽ɿ� �׷������� ����
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SPEAR2, 0, 0);
			break;

		// ���� �ú� ����
		case ATTACKATB_ARROW:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_ARROW, siPosition_AttakerX, siPosition_AttakerY, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));
			
			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_ARROW, 0, 0);
			break;

		// ���� ���� ����
		case ATTACKATB_UPGRADETOWERCANNON:
			// ���� ��Ʈ ȿ�� �̹����� ��µ��� ������ ȿ������ �����Ѵ�.
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_WALLBREAKERHIT"), true);

			// robypark 2005/1/14 18:52
			// ��ǥ ĳ���� �߽ɿ� �׷������� ����
			if (siEmptySmokeID)
				m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_SetDrawInCharacterCenterPos(siEmptySmokeID, uiTargetUniqueID);

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_JU104AE1, 0, 0);
			break;

		// ���� �߼��� ����
		case ATTACKATB_CATAPULT:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_STONE, siPosition_AttakerX - 1, siPosition_AttakerY - 1, siPosition_TargetX, siPosition_TargetY, (bNeedToSendActionMsg) ? uiTargetUniqueID : 0, TARGET_TYPE_CHARACTER, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_CU106AE1, 0, 0);
			break;
	}
}

// robypark 2004/11/3 14:5
// ������ ���� ���� �Ӽ��� ���� ȿ�� ����ϱ�
// ��ǥ���� ��ġ�� ����
// Effect �߿� Smoke�� ��� ������ �ٰŸ� �������� �Ǵ��ϸ� ������ Action�޽����� �����ϱ� �ʴ´�.
// �ݸ鿡 Bullet�� ��� ������ ���Ÿ� ���� �������� �Ǵ��ϸ� ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION�޽����� ������ �����Ѵ�.
// ���� ������ �޽��� ������ �ڽŰ� ���õ� ��쿡�� �ش�ȴ�.
void cltOnlineWorld::SiegeWarfare_CreateAttackEffectVillage(UI16 uiAttackerUniqueID, SI16 ssTargetX, SI16 ssTargetY, UI16 uiVillageCode/* = 0*/)
{
	// ������ ĳ���� ���� ���� �ε���
	UI16 uiKindInfoIdx_Attacker = pIOnlineChar->GetImjinKind(uiAttackerUniqueID);

	// ������ ��ġ
	SI16 siPosition_AttakerX, siPosition_AttakerY;

	// ������ ĳ���� ��ġ ���
	pIOnlineChar->GetPos(uiAttackerUniqueID, &siPosition_AttakerX, &siPosition_AttakerY);

	// ĳ���� ���� �Ӽ��� ���� ȿ�� ����
	switch (KI[uiKindInfoIdx_Attacker].GetAttackAtb(0))
	{
		// ���� �˺�, �⸶�� ����
		case ATTACKATB_SWORD:
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_BLADEHIT"));

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SWORD, 0, 0);
			break;

		// ���� â�� ����
		case ATTACKATB_LONG_SPEAR:
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_SPEARHIT"));

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_SPEAR2, 0, 0);
			break;

		// ���� �ú� ����
		case ATTACKATB_ARROW:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_ARROW, siPosition_AttakerX, siPosition_AttakerY, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));
			
			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_ARROW, 0, 0);
			break;

		// ���� ���� ����
		case ATTACKATB_UPGRADETOWERCANNON:
			// ���� ��Ʈ ȿ�� �̹����� ��µ��� ������ ȿ������ �����Ѵ�.
			m_SiegeWarfare_SmokeMgr.SetSiegeWarfare_Smoke(KIND_SMOKE_SIEGEWARFARE_IN_FIGHT, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK), GetEffectDataMgr()->GetMyHashTable()->Find("EFFECT_WALLBREAKERHIT"), true);

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_JU104AE1, 0, 0);
			break;

		// ���� �߼��� ����
		case ATTACKATB_CATAPULT:
			m_SiegeWarfare_BulletMgr.SetSiegeWarfare_Bullet(KIND_BULLET_SIEGEWARFARE_STONE, siPosition_AttakerX - 1, siPosition_AttakerY - 1, ssTargetX, ssTargetY, uiVillageCode, TARGET_TYPE_VILLAGE, KI[uiKindInfoIdx_Attacker].GetCharAniamtionInstant(ANI_ATTACK));

			// ȿ���� ����
			if (0 != KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0))
				HashCodePushEffect(KI[uiKindInfoIdx_Attacker].GetWeaponAttackEffect(0), 0, 0);
			else
				PushEffect(EFFECT_FIRE_CU106AE1, 0, 0);
			break;
	}
}

// robypark 2004/11/8 15:38
// ������ ���������� �˻��Ѵ�.
BOOL	cltOnlineWorld::IsSiegeWarfareUnit(SI32 siImjinKindIdx)
{
	// ��ȿ�� ĳ���� �ε��� ���� �˻�
	if (KIND_ON_INVALID == siImjinKindIdx)
		return FALSE;

	// ĳ���� �ε��� ���� ������ �˻�
	if (KIND_ON_MAX <= siImjinKindIdx)
		return FALSE;

	// ������ �������� �˻�
	if (TRUE == KI[siImjinKindIdx].IsKindAtb(KINDATB_SIEGE_WARFARE_UNIT))
		return TRUE;

	return FALSE;
}

// robypark 2004/11/24 16:52
// ������ �ð� ���� �ɼ� ���� HQ���� �б�
BOOL cltOnlineWorld::LoadSOptionSiegeWarfare(char *pszServiceNation, SI16 siFindServiceType, char *pszFileName)
{
	FILE*	fp;
	OnlineCommonParser	ocp;

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "LoadSOptionSiegeWarfare Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		SI32	siServiceType;

		char	pszNation[32];

		ocp.GetValue( pszNation,								31,		iLineNo, iColNo );		// Service Nation
		ocp.GetValue( siServiceType,									iLineNo, iColNo );		// Service Type
		// ã������ ���� �������� ��
		if (stricmp(pszNation, pszServiceNation) == 0)
		{
			// ã������ ���� Ÿ��(����Ʈ ����) ��	��] ���ѹα� �Ϸ��� 0, ������ 1
			if (siServiceType == siFindServiceType)
			{
				// ���� ���� �б�
				for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
				{
					ocp.GetValue( (char &)m_SOptionSiegeWarfare.scSiegeDay[i],	iLineNo, iColNo );		// �����ϴ� ����
				}

				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN,					iLineNo, iColNo );		// ���� ���� �ð�
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END,					iLineNo, iColNo );		// ���� ���� �ð�

				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_Begin,	iLineNo, iColNo );		// ��ź ���� ���� ���� ó�� ���۽ð�
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End,	iLineNo, iColNo );		// ��ź ���� ���� ���� ó�� ����ð�

				// ���� ȿ�� ����
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaGeneralEffect_Range,	iLineNo, iColNo );
				// ������ �������� ������ �� ���� ���� ��� �������� �Ÿ�
				ocp.GetValue( (short &)m_SOptionSiegeWarfare.siSIEGEWARFARE_IsinAreaHostileVillage_Range, 	iLineNo, iColNo );
				// ���� ���� ���� 10% �ڵ� ä��� �� Ÿ��
				ocp.GetValue( m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime,		iLineNo, iColNo );

				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_Cost,		iLineNo, iColNo );	// ���� �ݰ� ���� ��� ���
				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_Amount,	iLineNo, iColNo );	// ���� �ݰ� ���� ���Ǵ� ��ġ
				ocp.GetValue( m_SOptionSiegeWarfare.siVillageCounterAttack_DefenceUp_CoolTime,	iLineNo, iColNo );	// ���� �ݰ� ���� ��� �� Ÿ��(��)

				// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� �Ұ��� �ð�
				m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_Begin	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siSiegeWarfareTime_RecruitArmy_End	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END;		// ���� ������ �� �ð� �� ���� �Ұ���

				// ����ڵ��� ��� ������ �Ұ����� �ð�
				m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_Begin		= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siGuildDisabledTimeToJoin_End			= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End;

				// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð�(����� ��ܿ� �ذ� �Ұ����ϴ�)
				m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_Begin	= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN;
				m_SOptionSiegeWarfare.siGuildDisabledTimeToWithdraw_End		= m_SOptionSiegeWarfare.siSIEGEWARFARETIME_AllInInvestment_End;
				break;
			}
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// robypark 2004/12/1 10:11
// ������� �뺴(���ΰ� ����) �κ��丮�� ������ ������ �������� �ִ��� �˻��Ѵ�.
BOOL cltOnlineWorld::IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(void)
{
	UI08		uiFollowerSlot = 0;
	UI16		i = 0;

	// ������� ��� �뺴 ������ �˻��Ѵ�.
	for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
	{
		// �ش� ������ �뺴�� �κ��丮�� ������ ������ �������� �ִ��� �˻��Ѵ�.
		if (IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(uiFollowerSlot))
			return TRUE;	// �κ��丮�� ������ ������ �������� �����Ѵ�.
	}

	// ������ ������ �������� ����� �κ��丮�� �������� �ʴ´�.
	return FALSE;
}

// robypark 2005/1/8 16:26
// �ش� ������ �뺴�� �κ��丮�� ������ ������ �������� �ִ��� �˻��Ѵ�.
BOOL cltOnlineWorld::IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(UI08	uiFollowerSlot)
{
	UI16		i = 0;
	MyItemData	tempMyItemData;
	CItemHeader *pItem = NULL;

	// ��ȿ�� �뺴 �����̶��
	if (pMyData->IsValidFollower(uiFollowerSlot))
	{
		// �κ��丮�� �ִ� �����۵��� �˻�
		for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
		{
			// �κ��丮���� ������ ���
			if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
			{
				// ��ȿ�� �������̶��
				if(tempMyItemData.siPosInInventory != -1)
				{
					// ������ ���� ��� ���
					pItem = pItemsInfo->GetItem(tempMyItemData.uiID);
					if (pItem)
					{
						// ������ ������ �������̶��
						if (pItem->m_pImperium)
						{
							// �κ��丮�� ������ ������ �������� �����Ѵ�.
							return TRUE;
						}	// if (pItem->m_pImperium)
					}	// if (pItem)
				}	// if(tempMyItemData.siPosInInventory != -1)
			}	// if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
		}	// for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	}	// if (pMyData->IsValidFollower(uiFollowerSlot))

	// ������ ������ �������� �ش� ������ �뺴 �κ��丮�� �������� �ʴ´�.
	return FALSE;
}

// robypark 2004/12/2 17:31
// ���ΰ� �� �뺴 �κ��丮�� �ִ� ��� ������ ������ �������� �����Ѵ�.
void cltOnlineWorld::DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit(void)
{
	UI08		uiFollowerSlot = 0;
	UI16		i = 0;
	MyItemData	tempMyItemData;
	CItemHeader *pItem = NULL;

	// ������� ��� �뺴 ������ �˻��Ѵ�.
	for (uiFollowerSlot = 0; uiFollowerSlot < ON_MAX_FOLLOWER_NUM; uiFollowerSlot++)
	{
		// ��ȿ�� �뺴 �����̶��
		if (pMyData->IsValidFollower(uiFollowerSlot))
		{
			// �κ��丮�� �ִ� �����۵��� �˻�
			for (i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)		
			{
				// �κ��丮���� ������ ���
				if (pMyData->GetItem(uiFollowerSlot, i, &tempMyItemData))
				{
					// ��ȿ�� �������̶��
					if(tempMyItemData.siPosInInventory != -1)
					{
						// ������ ���� ��� ���
						pItem = pItemsInfo->GetItem(tempMyItemData.uiID);
						if (pItem)
						{
							// ������ ������ �������̶��
							if (pItem->m_pImperium)
							{
								// �κ��丮�� ������ ������ �������� �����Ѵ�. �������� �����ض�
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
// ������ ������ ��� ������ ������ ����� �̸�, ü��, ����ǰ, ���� �� �� �ִ����� ��Ÿ���� �÷��� ����
void	cltOnlineWorld::SetIsShowInformationAboutSiegeWarfareUnit(SI08 scIsShowInformationAboutSiegeWarfareUnit)
{
	m_scIsShowInformationAboutSiegeWarfareUnit = scIsShowInformationAboutSiegeWarfareUnit;
}

// robypark 2005/1/8 11:18
// ����ǰ �ڵ� �Ҹ� ���� �����ð� ���
SI32	cltOnlineWorld::GetRestTimeAutoSpendSupplyGoods(void)
{
	return m_siRestTimeAutoSpendSupplyGoods;
}

// robypark 2005/1/8 11:18
// ����ǰ �ڵ� �Ҹ� ���� �����ð� ����
void	cltOnlineWorld::SetRestTimeAutoSpendSupplyGoods(SI32 siRestTimeAutoSpendSupplyGoods)
{
	m_siRestTimeAutoSpendSupplyGoods = siRestTimeAutoSpendSupplyGoods;
}

// robypark 2005/1/12 19:30
// ������ ���� ��� ���� �ʱ�ȭ
void	cltOnlineWorld::CancelSiegeWarfare_AttackMode(void)
{
	m_sSiegeWarfare_AttackMode.scAttackMode = SIEGEWARFARE_ATTACKMODE_NONE;
	m_sSiegeWarfare_AttackMode.siAutoAttackTimes = 0;
}

// H2-S-05.02.04 //
// ���� ��ī��Ʈ�� ĳ���� �̸� ��´�.
char*	cltOnlineWorld::OnlineCharName(UI16 nAccount)
{
	return pIOnlineChar->GetCharName(nAccount);
}