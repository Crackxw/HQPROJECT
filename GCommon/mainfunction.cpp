//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>
#include <checkdupgame.h>
#include <clGame.h>
#include "..\Gersang\ymemoryfree.h"
#include "..\Gersang\directx.h"

#include <Mouse.h>
#include <findpath5.h>
#include <mainfunction.h>
#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <item.h>
#include "..\Gersang\music.h"
#include <SoundManager.h>
#include <chat.h>
#include "..\Gersang\mapedit.h"
#include <mymath.h>
#include "..\Gersang\myio.h"
#include "..\Gersang\mapedit.h"
#include "..\Gersang\resource.h"
#include "..\Gersang\gamemenu.h"
#include "..\Gersang\gmenu.h"
#include "..\Gersang\loadgameinitmenu.h"
#include "..\Gersang\Yprg.h"
#include "..\Gersang\Winlose.h"
#include "..\Gersang\CalcAttack.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <OnlineWorld.h>
#include "OnLineItem.h"
#include <etc.h>

#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "charanimation.h"
#include <kindinfo.h>
#include <zmouse.h>
#include <myfile.h>
//#include <cdmanage.h>
#include <capture.h>
#include <charinterface.h>
#include <MapInterface.h>
#include <Map.h>
#include <bullet.h>
#include <smoke.h>
#include <Game2.h>
#include <ParametaBox.h>
#include <MyNet-Game.h>
#include "..\Gersang\text.h"
#include <preg.h>
#include	"clNumberRender.h"
#include	"clItemInfoWindow.h"
#include <Mynet.h>
#include "DebugTool.h"
#include "stringcpy.h"

// actdoll (2004/05/04 16:33) : 새 에디트 컨트롤러 세팅
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

//#include "..\\SCCommon\\keyhook.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

//--> Test Code (sagolboss)
// actdoll (2004/10/01 11:04) : 회장님 소스 막음
//#include "..\GersangUI\UIGSMgr.h"
//#include "..\GersangUI\UIGSSurfaceBufferMgr.h"
//#include "..\CommonInterface\MouseHandlerMgr.h"
//<--


//extern HQFileManager	HQFile;
extern _clCapture		Capture;							// 화면 캡춰 관련 클래스 


extern SI32  WindowModeSwitch;
extern	_Text 				Text;								// 텍스트 
extern char					SavePath[];								// 현재 게임이 설치된 경로 
extern clock_t				StartClock, PrevClock;
extern clock_t				MonitorMacroClock1, MonitorMacroClock2;

extern	_clGame* pGame;
extern	UI32					GameScreenMode;
extern _InputDevice				IpD;
extern	SHORT					GameStatus;							// 현재의 게임 상황을 알리는 변수 
extern  _Chat 					Chat;
extern	SHORT					MessageMode;						// 메시지가 누구에게 가는가?
extern	SHORT 					GameMenuShowSwitch;					// 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	_MainData				GD;									// 게임의 주데이터 
extern	clock_t					CurrentClock;						// 시간 관련 
extern	SHORT 					MapEditMode;						// 지도 편집 모드인가?
extern	cltMapInterface			clifMap;							// 맵관련 인터페이스 
extern	_Map 					Map;
extern	int						CDStopSwitch;						// CD음악을 정지 시킨다. 
extern	int						bActive;							// 현재 액티브인지 아닌지 알려주는 변수  
extern	BOOL					g_bMainInit;						// 현재 초기화 중인 경우 무조건 액티브가 되도록
extern	BOOL					GameDropSwitch ;


extern	SI32					UserPlayerNumber;					// 사용자 관련 
extern	cltCharInterface		clifChar;							// 캐릭터 관련 인터페이스 
extern	XSPR 					PannelSpr;
extern	BOOL 					DPLauncherSwitch;					// dp launcher에 의해 작동되었는가
extern	_FileVary 				FV;
extern	_MyNetGame				MyNet;
extern	_FindPathGame		   *FindPathGame;
extern	char 					CDPath[];

extern	CCharFileLoadManager	g_FileLoadManager;				// 조합형 캐릭터
extern	CItem					*g_pItem;						// 아이템 클래스

//------------------------------------
// Direct X 용 변수들 
//------------------------------------
extern  LPDIRECTDRAWSURFACE7	lpPannelSurface;					// 판넬용 Surface


extern	int						EnterKeyPressDoneSwitch;
extern	DWORD					CaptureClock;						// 게임을 캡춰한 시간. 
extern	DWORD					LastDrawClock;
extern	BOOL					ScrollProcSwitch;
extern	SHORT					GameStatusBuffer;

extern  SHORT                   GameStatus;
extern  BOOL                    GameDraw4();

int     OriginalScreenXsize, OriginalScreenYsize; 
SI32 g_siAdType = 0;	

/////////////////////////////////////
// 사운드 관련 
////////////////////////////////////
SoundManager *g_pSoundManager = NULL;

static void DetermineShowCursor(DWORD nHittest);


// 불필요한 파일을 삭제한다.
void DeleteUnusableData()
{
	// 쓸데 없는 파일이 남아 있는 상태가 아니라야 한다.
	DeleteData();

//	HQFile.FileDeCode( "Data.hq", "Data" );

	CHAR	szFileNameBuffer[1024];

	// 게임이 설치된 경로와 저장된 경로를 얻어 온다. 
	// actdoll (2004/07/14 14:05) : 미쳐버릴 상황 발생!!!
	//		재수없게도 Carom 3D와 같은 키값의 이름이 동일한 내용과 
	//		겹쳐 문제가 발생할 수 있으므로 수정해서 오린다.
//	GetPath(SavePath);
//	g_siAdType = GetAdType();
	GetCurrentDirectory( MAX_PATH, SavePath );
	strcat( SavePath, "\\" );
	g_siAdType = 0;

	GetFileNamePath("ErrorCode.txt", SavePath, szFileNameBuffer);
	DeleteFile(szFileNameBuffer);
}

// 생성자 
_clGame::_clGame()
{
	pOnlineWorld = NULL;
	pBattle = NULL;

	pNumberRender   = NULL;
	pItemInfoWindow = NULL;
	m_pEffectDataMgr= NULL;

	LanguageCode = ELANGAGECODE_KOREAN;
}

// 소멸자. 
_clGame::~_clGame()
{
	if(pBattle)
	{
		delete pBattle;
		pBattle = NULL;
	}

	if(pNumberRender)
	{
		delete pNumberRender;
		pNumberRender = NULL;
	}

	if(pItemInfoWindow)
	{
		delete pItemInfoWindow;
		pItemInfoWindow = NULL;
	}	

	if(m_pEffectDataMgr)
	{
		delete m_pEffectDataMgr;
		m_pEffectDataMgr = NULL;
	}	
// actdoll (2004/10/01 11:04) : 회장님 소스 막음
//	delete GetSurfaceBufferMgr();
//	delete GetUIGSMgr();
//	delete GetMouseHandlerMgr();

	DestroyWindow(Hwnd);
}


BOOL _clGame::CreateGameWindow(HINSTANCE hInstance, int nCmdShow)
{
	CHAR szName[128], szTitle[128];
	if(pGame->LanguageCode == ELANGAGECODE_JAPAN)
	{
		strcpy(szName, "Kyosyoden");
		strcpy(szTitle, "Kyosyoden");
	}
	else
	{
		strcpy(szName, "Gersang");
		strcpy(szTitle, "Gersang");
	}
	
	HWND hwnd;
	WNDCLASS wc;

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=WindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wc.hCursor=LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = NULL;//(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName=szName;
	wc.lpszClassName=CLASSNAME;
	RegisterClass(&wc);

	OriginalScreenXsize=GetSystemMetrics(SM_CXSCREEN);
	OriginalScreenYsize=GetSystemMetrics(SM_CYSCREEN);


	if(WindowModeSwitch == FALSE)
	{
		hwnd=CreateWindowEx(
        WS_EX_TOPMOST,
        CLASSNAME,
        szTitle,
        WS_POPUP|WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		// 원래 해상도를 저장한다. 
		800, 
		600, 
		NULL, 
		NULL,
		hInstance,
		NULL);
	}
	else
	{
		DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
                DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
                DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
                DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );

		hwnd=CreateWindowEx(
        WS_EX_APPWINDOW,
        CLASSNAME,
        szTitle,
        (WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)),
		CW_USEDEFAULT, CW_USEDEFAULT,
		800 + dwFrameWidth * 2 , 
		600 + dwFrameHeight * 2 + dwMenuHeight + dwCaptionHeight, 
		NULL, 
		NULL,
		hInstance,
		NULL);
	}


	if(hwnd==NULL)
		return FALSE;

	// 전역 핸들에 값을 넘긴다. 
	Hwnd=hwnd;
	hInst=hInstance;

	// robypark 2004/12/21 18:33
	// 윈도 디버그 모드일 때에는 중복 실행이 가능하도록 수정
#ifdef	_DEBUG
	if( !WindowModeSwitch )
	{
#endif//_DEBUG
		// 중복된 게임이 있는지 확인한다. 
		if(CheckDupGame(szName, szTitle) == TRUE)
		{
			SetForegroundWindow(FindWindowEx(NULL, NULL, szName, szTitle));
			CloseDupGameHandle();

			return FALSE;
		}
#ifdef	_DEBUG
	}
#endif//_DEBUG

	// 메세지 훗킹을 시작한다.(Mouse move, mouse left button down)

	DWORD dwThreadID = GetCurrentThreadId();
	char szBuf[256];
	sprintf(szBuf, "APP Thread ID:%d\n", dwThreadID);
	OutputDebugString(szBuf);

//	installhook(Hwnd);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

BOOL _clGame::CreateGameDirectDraw()
{
    ////////////////////////////////////////////
    //  DirectDraw object 만들기
    ////////////////////////////////////////////    

	SI32 flag;
	
//#ifdef _WINDOWED
//	
//	WindowModeSwitch = TRUE;
//#endif

	if(WindowModeSwitch == TRUE)
	{
		flag = DDSCL_NORMAL;
	}
	else
	{
		flag = DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN;
	}

		if(clGrp.CreateDirectDraw(Hwnd, GameScreenMode, flag) == TRUE )
		{
			if (WindowModeSwitch) {
				LoadPalette();

				clGrp.SetAllPalette();
			}



				// off screen을 생성한다. 
				CreateSurface();
			
		}
		else
		{
			MessageBox(Hwnd, "Display Error", "Error", MB_OK);
			finiObjects();
			DestroyWindow( Hwnd);
			return FALSE;
			
		}

		//////////////////////////////////////////////////////////
		// 팔레트 화일을 불러서 Palette에 저장한다.
		//////////////////////////////////////////////////////////

		if (!WindowModeSwitch) {
	 		LoadPalette();

			clGrp.SetAllPalette();
		}

		// 16비트 팔레트에 값을 넣는다. 
		clGrp.SetColorTable();
		
		
		return TRUE;
}

//--------------------------------------------------------------------------
// Name: doInit()
// Desc: 윈도우 생성 
//--------------------------------------------------------------------------
BOOL _clGame::Init(HINSTANCE hInstance, int nCmdShow)
{
	if(CreateGameWindow(hInstance, nCmdShow) == FALSE)return FALSE;

	// 게임 상세 버전을 레지스트리에 기록한다.
//	SetServerVersion();


	// 불필요한 파일을 삭제한다.
	DeleteUnusableData();


	CoInitialize(NULL);

	// 다이렉트 드로우를 생성한다.
	if(CreateGameDirectDraw() == FALSE)return FALSE;

	// Config 화일을 불러온다. 
	if(Config.Load()==FALSE)
	{
		Config.Init();       // Config 파일이 없을경우 Default값으로 설정한다.
	}

	// 사운드 초기화
	InitSoundManager();

	// timeGetTime()의 단위를 설정한다.
	timeBeginPeriod(1);

	// 클리핑 영역을 화면 전체로 설정한다. 
	clGrp.SetClipFullScreen();

	// 게임이 시작된 시간을 구한다. 
	StartClock=timeGetTime();

	MonitorMacroClock1 = StartClock;
	MonitorMacroClock2 = StartClock;

	ShowCursor(FALSE);

#ifdef _OGG_PLAY
	new CBgmMgr;
	if( g_pSoundManager && g_pSoundManager->pEAXSound)
	{
		GetBgmMgr()->Init(g_pSoundManager->pEAXSound->GetDirectSound(), 32 * 1024);
		GetBgmMgr()->SetVolume(g_pSoundManager->GetStreamSoundVolume());
	}
#endif
	
//--> Test Code (sagolboss)
/*
	new CMouseHandlerMgr;
	new CUIGSMgr;
	new CUIGSSurfaceBufferMgr;
*/
//<--

	return TRUE;

}

UINT _clGame::AlterationWndMsg(UINT uMsg)
{
	if(//uMsg == WM_MOUSEMOVE			||
	   uMsg == WM_LBUTTONDOWN		||
	   uMsg	== WM_LBUTTONUP)
//	   uMsg == WM_KEYDOWN			||
//	   uMsg == WM_USER_RETURN)
	   {
 			uMsg += 0x1000; 
	   }

	return uMsg;
}

UINT _clGame::OriginallyWndMsg(UINT uMsg)
{
	if(//uMsg == (WM_MOUSEMOVE+0x1000)		||
	   uMsg == (WM_LBUTTONDOWN+0x1000)		||
	   uMsg	== (WM_LBUTTONUP+0x1000))
//	   uMsg == (WM_KEYDOWN+0x1000)			||
//	   uMsg == (WM_USER_RETURN+0x1000))
	   {
			uMsg -= 0x1000;
	   }

	return uMsg;
}

void _clGame::Poll(MSG *pmsg)
{
	while( 1 )
    {
		if(pGame)
		{
			if(pOnlineWorld)
				pOnlineWorld->SendHeartBeat();
		}

        if (PeekMessage( pmsg, NULL, 0, 0, PM_NOREMOVE))
        {
			if(pGame)
				MonitorMacro();

            if (!GetMessage( pmsg, NULL, 0, 0))
            { 
                break;		// 종료
            }
			// TRUE이면 다른 쓰레드에서 넘어온 메시지이다.
			if( InSendMessage() == TRUE )
			{
				break;
			}

			if(pmsg->message == WM_PASTE)	break;

			if(pmsg->wParam == VK_RETURN)
			{
				if(pGame->pOnlineWorld)
				{
					if(pGame->pOnlineWorld->pOnlineVillage == NULL && pGame->LanguageCode == ELANGAGECODE_KOREAN)
					{
//						if(pmsg->message == WM_KEYDOWN)	
//							pmsg->message = WM_USER_RETURN;
//						else if(pmsg->message == WM_SYSKEYDOWN)		
//							pmsg->message = WM_USER_SYSRETURN;
						// actdoll (2004/05/05 17:57) : 유저리턴은 안쓴다.
						if( pmsg->message == WM_SYSKEYDOWN )	pmsg->message = WM_USER_SYSRETURN;
					}
				}
			}

			// 특정 메세지 아이디를 변경한다.
	//		pmsg->message = pGame->AlterationWndMsg(pmsg->message);

            TranslateMessage(pmsg); 
            DispatchMessage(pmsg);
        }
        else
		{
			MyNet.ReceiveMessages();
			
			if(bPause==FALSE)
			{
				if(bActive==FALSE 
					&& GameStatus != GS_GAME 
					&& GameStatus != GS_GAMEMENU
					&& GameStatus != GS_INITWIN
					&& GameStatus != GS_INITLOSE
					&& GameStatus != GS_ONLINEWORLD)
				{
					continue;
				}
				
				
				PrevClock		= CurrentClock;
				CurrentClock	= timeGetTime();
				
				// 윈도우가 활성화 되어 있을때에만 효과음을 낸다. 
				if(bActive == TRUE)
				{
					// 효과음을 연주한다.
					if(pGame)
					{
						if(pGame->m_pEffectDataMgr)
							pGame->m_pEffectDataMgr->Pop();
					}
					
					// fade in효과를 처리한다. 
					FadeIn();
				}
				switch(GameStatus)
				{
					
				case GS_GAME:					GsGame();						break;	// 전투 중 - 루프
					
				case GS_INITQUIT:				GsInitQuit(); 					break;	// 필드에서 나가기 시 초기화 - 1회 (이때 GS_INITFADEOUT, GS_FADEOUT을 통과함)
					
				case GS_QUIT:					GsQuit();						break;	// 필드에서 나가기 시 - 루프 (완전 종료)
					
//				case GS_INITLOGO:				GsInitLogo();					break;
					
//				case GS_LOGO:					GsLogo();						break;
					
//				case GS_INITHQLOGO:				GsInitHQLogo();					break;
					
//				case GS_HQLOGO: 				GsHQLogo();	        			break;
					
//				case GS_INITSAMSUNGLOGO:		GsInitSamsungLogo();			break;
					
//				case GS_SAMSUNGLOGO:			GsSamsungLogo();				break;
					
//				case GS_INITSTAFF:				GsInitStaff();					break;
					
//				case GS_STAFF:					GsStaff();						break;
					
				case GS_INITINITMENU:			GsInitQuit();					break;
					
				case GS_INITMENU:				GsInitMenu();					break;
					
//				case GS_INITOPENING:			GsInitOpening();				break;
					
//				case GS_OPENING:				GsOpening();					break;
					
//				case GS_INITENDING:				GsInitEnding();					break;
					
//				case GS_ENDING:					GsEnding();						break;
					
					// 온라인상에서의 전투. (초기화)
				case GS_INITONLINEBATTLEGAME:	GsInitOnlineBattleGame();		break;	// 전투 전 초기화 - 1회
					
				case GS_INITRESTART:			GsInitRestart();				break;
					
				case GS_RESTART:				GsRestart();					break;
					
				case GS_INITWIN:				GsInitWin();					break;	// 전투 후 결과 초기화 - 승리 시 해당 - 1회
					
				case GS_WIN:					GsWin();						break;	// 전투 후 결과 화면 - 승리 시 해당 - 루프
					
				case GS_INITLOSE:				GsInitLose();					break;	// 전투 후 결과 초기화 - 패배 시 해당 - 1회
					
				case GS_LOSE:					GsLose();						break;	// 전투 후 결과 화면 - 패배 시 해당 - 루프
					
					// 게임 메뉴를 초기화한다. 
				case GS_INITGAMEMENU:			GsInitGameMenu();				break;	// 전투 중 옵션 화면 초기화 - 1회
					
					// 게임 메뉴를 처리한다. 
				case GS_GAMEMENU:				GsGameMenu();					break;	// 전투 중 옵션 화면 - 루프
					
				case GS_INITFADEOUT:			GsInitFadeOut();				break;	// 모든 페이드 아웃 장면 - 1회 초기화
					
				case GS_FADEOUT:				GsFadeOut();					break;	// 모든 페이드 아웃 장면 - 루프
					
					
					//  임진록 온라인 관련
				case GS_INITONLINEWORLD:		GsInitOnlineWorld();			break;	// 게임 초기화 - 게임 실행시 가장 먼저 진입 - 1회 초기화
					
				case GS_ONLINEWORLD:			GsOnlineWorld();				break;	// 메인 폴 - 메인화면, 필드 등에서의 모든 행동 - 루프

				case GS_INITBATTLE:				GameStatus = GS_INITONLINEWORLD;break;
					
				}	
			} //if(bPause==FALSE)
		}
		
	}
	
			
}

// 사운드 초기화
void _clGame::InitSoundManager()
{
	if( g_pSoundManager != NULL )	return;
	
	g_pSoundManager = new SoundManager(Hwnd, "EffectSound.txt", FALSE, FALSE, 20, 1); // Main Window Handle, 파일명, EAX, 3D, Static 슬롯개수, Stream 슬롯개수

	if( g_pSoundManager == NULL )
	{
		clGrp.Error("Notice", Text.NotProperSoundCardMessage.Get());
	}
	else
	{
		// 사운드 볼륨을 설정한다. 
	    Config.SetEffectSoundVolume( Config.m_siEffectSoundVolume);
	    Config.SetBackgroundMusicVolume( Config.m_siBackgroundMusicVolume);

		// 사운드의 좌우볼륨증폭, 상하볼륨증폭, 팬
		g_pSoundManager->SetEmulate3DValue(1, 1, 3);
	}
}

// 사운드 해제
void _clGame::FreeSound()
{
#ifdef _OGG_PLAY
	if( GetBgmMgr())
		delete GetBgmMgr();
#endif

	// 게임이 배경 음악을 해제 한다.
	FreeMenuBackgroundMusic();

	if( g_pSoundManager )
	{
		delete g_pSoundManager;
		g_pSoundManager = NULL;
	}
}

//--------------------------------------------------------------------------
// Name: finiObjects()
// Desc: 프로그램을 종료할때 처리 하는 루틴 
//--------------------------------------------------------------------------
void _clGame::finiObjects(void)
{
	int i;


	// 
	timeEndPeriod(1);

	if(FindPathGame)
	{
	   delete FindPathGame;
	   FindPathGame = NULL;
    }	

	g_pItem	=	NULL;

	FreeFnt();

	// 게임에 사용되는 모든 버튼들을 해제한다. 
	FreeAllButtons();

	// 정모가 할당한 메모리 해제 
	AllMemoryFree();
	
	// Config 파일을 저장한다.
	Config.Save();

	// Number Render
	if(pNumberRender)
		pNumberRender->Release();

	// Item Info Window
	if(pItemInfoWindow)
		pItemInfoWindow->Release();
	
	// 사운드 해제
	FreeSound();

	// 게임을 종료시키게 설정 한다.
	EndGame();

	// 서피스를 모두 해제한다. 
	CustomSurfaceRelease();
	clGrp.ReleaseDirectDraw();

	// 텍스트 해제 
	TextFree();

	// 윈도우 시스템 컬러를 복구한다. 
	int colorarray[25];
	COLORREF rgb[25];
	for(i=0;i<25;i++)
	{
		colorarray[i]=i;
		rgb[i]=GetSysColor(i);
	}
	SetSysColors(25, colorarray, rgb);

	CoUninitialize();

//	HQFile.Free();
}

// 전투 정보를 초기화한다.
BOOL _clGame::InitBattle()
{
	pBattle = new _clBattle;
	if(pBattle == NULL)return FALSE;
	
	if(pBattle->Init() == FALSE)return FALSE;
	
	return TRUE;
}


// 기타 변수들을 초기화한다. 
BOOL _clGame::InitEtc()
{
	// Number Render 생성
	pNumberRender = new clNumberRender;
	if(pNumberRender == NULL) return FALSE;
	pNumberRender->Create();
	
	pItemInfoWindow = new clItemInfoWindow;
	if(pItemInfoWindow == NULL) return FALSE;
	pItemInfoWindow->Create(this);

	m_pEffectDataMgr = new EffectDataMgr;

	return TRUE;
}

_clBattle::_clBattle()
{
	
}

_clBattle::~_clBattle()
{
	DeleteGameFont();
}

BOOL _clBattle::Init()
{
	HelpFont			= NULL;
	GetExpFont= NULL;
	TitleFont= NULL;
	StaffFont= NULL;
	SetGameFont= NULL;
	CharInfoFont= NULL;
	PlayerNameFont= NULL;
	BriefingFont= NULL;
	SaveInfoFont= NULL;
	ChatFont= NULL;
	VersionInfoFont= NULL;
	VersionInfoPlusFont= NULL;
	NormalEditFont= NULL;
	NormalEditPlusFont= NULL;
	ChatEditFont= NULL;
	WinLoseFont= NULL;
	IDFont= NULL;
	GameItemFont= NULL;

	// 게임 버튼을 초기화한다. 
	ZeroMemory(GameButton, sizeof(_GameButton) * MAX_GAME_BUTTON);

	LastUpdateCharInfo		= 0;
	UpdateCharInfoDataIndex = 0;


	// 게임에 사용되는 모든 문자열을 초기화한다. (Credits 포함)
	TextInit();
	
	

	// spr 이미지 파일 정보를 불러온다.
	LoadImgFileList();

	// 게임에 사용되는 화일 변수들을 초기화한다. 
	FV.Init();

	// 멥에디터에서 사용하는 File Open Box변수들의 초기화 
	PopMapFileInitialize(pGame->Hwnd);

	// 모든 메뉴 아이템을 초기화 한다. (생성버튼의 단축아이콘 및 메시지, 명령번호 지정)
    ItemInit();				
	
	// 각 명령에 대한 특성치들을 설정한다.
	OrderInit();
	
	// 모든 캐릭터에대한 특성치 설정 
	if(InitKindInfo() == FALSE)return FALSE;
//	InitKindInfo();

	// 공격 속성의 속성을 설정한다. 
	InitAttackAtb();

	g_FileLoadManager.LoadInfo();

	g_pItem	= NULL;

	// 모든 공격에대한 특성치 부여 
	InitBulletKindInfo();
	// 특수 효과에 대한 특성치 부여 
	InitSmokeKindInfo();
    // 수학변수 값들을 계산한다. (sqrt값과 sine값)
	InitMath();
	
	// Dplay용 데이터를 초기화한다. 
	MyNet.Init();

	// 온라인에서 사용할 경험치를 데이터 화일에서 읽어온다. 
	LoadExpData(g_fTestVersion);

	FindPathGame = new _FindPathGame(MAX_MAP_XSIZE, MAX_MAP_YSIZE);
	if(FindPathGame)
		FindPathGame->SetFindPathSize(MAX_MAP_XSIZE, MAX_MAP_YSIZE);


	// 폰트를 만든다. 
	CreateGameFont();
	
	// Color Table 을 설정한다.
	InitColorTable();
	// 언덕 Tile Table을 초기화 한다.
	
	clifMap.InitTile();

    // 버튼의 위치 및 크기를 초기화 하여 설정한다.
	OnceInitButton();

	// 문자 입력용 에디트 콘트롤을 만든다.
	if(NormalEditFont == NULL || ChatEditFont == NULL)
	{
		clGrp.Error("Invalid Font", "1249574");
	}
	else
	{
		// ===============================================================================
//		m_EditControl.CreateEditBox(1000, 1000, 700, 16, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 55 );
		// ===============================================================================
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
		RECT	rcDest = { 233, 582, 533, 600 };
		m_hOecChat	= Handler_OnlineEditControlEx::CreateControl( 0, &rcDest, 55 );
		// ===============================================================================
	}
	
	// 게임용 스프라이트를 불러온다.
	LoadFnt();

	// 게임에 사용되는 버튼들을 불러온다.(양정모)
	LoadAllButtons();
	
	InitFogImage();
	
	// 기본 마우스로 변경한다.
	SetMouse(MOUSE_NORMAL);
	

	return TRUE;
}

//  게임에 사용될 폰트를 만든다.
void _clBattle::CreateGameFont()
{
	SI32 size, thick;
	SI32 sicharset	= 0;
	char nation[128]="";
	char font[128];
	

	switch(pGame->LanguageCode)
	{
	case ELANGAGECODE_KOREAN:
		sicharset	  = HANGEUL_CHARSET;
		strcpy(nation, "KOREA");
		break;
	case ELANGAGECODE_TAIWAN:
		sicharset	  = CHINESEBIG5_CHARSET;
		strcpy(nation, "TAIWAN");
		break;
	case ELANGAGECODE_JAPAN:
		sicharset	  = SHIFTJIS_CHARSET;
		strcpy(nation, "JAPAN");
		break;
	case ELANGAGECODE_HONGKONG:
		sicharset	  = CHINESEBIG5_CHARSET;
		strcpy(nation, "HONGKONG");
		break;
	case ELANGAGECODE_CHINA:
		 sicharset	  = DEFAULT_CHARSET;
		 strcpy(nation, "CHINA");
		break;
	case ELANGAGECODE_INDONESIA:
		sicharset	  = DEFAULT_CHARSET;
		strcpy(nation, "INDONESIA");
		break;
	default:
		clGrp.Error("fd!239744", "fd3873:%d", pGame->LanguageCode);
		break;
	}

	size = GetTextFormat(nation,	"TitleFont",			"Size");
	thick= GetTextFormat(nation,	"TitleFont",			"Thick");
	GetTextFormatFont	(nation,	"TitleFont",			font );
	TitleFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(TitleFont == NULL) clGrp.Error("Error", "TitleFont Create Fail");
	TitleFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(TitleFont_Underline == NULL) clGrp.Error("Error", "TitleFont Create Fail");
	
	size = GetTextFormat(nation,	"StaffFont",			"Size");
	thick= GetTextFormat(nation,	"StaffFont",			"Thick");
	GetTextFormatFont	(nation,	"StaffFont",			font );
	StaffFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(StaffFont == NULL) clGrp.Error("Error", "StaffFont Create Fail");
	StaffFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(StaffFont_Underline == NULL) clGrp.Error("Error", "StaffFont Create Fail");
	
	size = GetTextFormat(nation,	"HelpFont",				"Size");
	thick= GetTextFormat(nation,	"HelpFont",				"Thick");
	GetTextFormatFont	(nation,	"HelpFont",				font );
	HelpFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(HelpFont == NULL) clGrp.Error("Error", "HelpFont Create Fail");
	HelpFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(HelpFont_Underline == NULL) clGrp.Error("Error", "HelpFont Create Fail");

	size = GetTextFormat(nation,	"GetExpFont",				"Size");
	thick= GetTextFormat(nation,	"GetExpFont",				"Thick");
	GetTextFormatFont	(nation,	"GetExpFont",				font );
	GetExpFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(GetExpFont == NULL) clGrp.Error("Error", "GetExpFont Create Fail");
	GetExpFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(GetExpFont_Underline == NULL) clGrp.Error("Error", "GetExpFont Create Fail");


	size = GetTextFormat(nation,	"BriefingFont",			"Size");
	thick= GetTextFormat(nation,	"BriefingFont",			"Thick");
	GetTextFormatFont	(nation,	"BriefingFont",			font );
	BriefingFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(BriefingFont == NULL) clGrp.Error("Error", "BriefingFont Create Fail");
	BriefingFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(BriefingFont_Underline == NULL) clGrp.Error("Error", "BriefingFont Create Fail");

	size = GetTextFormat(nation,	"SaveInfoFont",			"Size");
	thick= GetTextFormat(nation,	"SaveInfoFont",			"Thick");
	GetTextFormatFont	(nation,	"SaveInfoFont",			font );
	SaveInfoFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(SaveInfoFont == NULL) clGrp.Error("Error", "SaveInfoFont Create Fail");
	SaveInfoFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(SaveInfoFont_Underline == NULL) clGrp.Error("Error", "SaveInfoFont Create Fail");

	size = GetTextFormat(nation,	"VersionInfoFont",		"Size");
	thick= GetTextFormat(nation,	"VersionInfoFont",		"Thick");
	GetTextFormatFont	(nation,	"VersionInfoFont",		font );
	VersionInfoFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(VersionInfoFont == NULL) clGrp.Error("Error", "VersionInfoFont Create Fail");
	VersionInfoFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(VersionInfoFont_Underline == NULL) clGrp.Error("Error", "VersionInfoFont Create Fail");

	size = GetTextFormat(nation,	"VersionInfoPlusFont",	"Size");
	thick= GetTextFormat(nation,	"VersionInfoPlusFont",	"Thick");
	GetTextFormatFont	(nation,	"VersionInfoPlusFont",	font );
	VersionInfoPlusFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(VersionInfoPlusFont == NULL) clGrp.Error("Error", "VersionInfoPlusFont Create Fail");
	VersionInfoPlusFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(VersionInfoPlusFont_Underline == NULL) clGrp.Error("Error", "VersionInfoPlusFont Create Fail");

	size = GetTextFormat(nation,	"NormalEditFont",		"Size");
	thick= GetTextFormat(nation,	"NormalEditFont",		"Thick");
	GetTextFormatFont	(nation,	"NormalEditFont",		font );
	NormalEditFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(NormalEditFont == NULL) clGrp.Error("Error", "NormalEditFont Create Fail");
	NormalEditFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(NormalEditFont_Underline == NULL) clGrp.Error("Error", "NormalEditFont Create Fail");

	size = GetTextFormat(nation,	"NormalEditPlusFont",	"Size");
	thick= GetTextFormat(nation,	"NormalEditPlusFont",	"Thick");
	GetTextFormatFont	(nation,	"NormalEditPlusFont",	font );
	NormalEditPlusFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(NormalEditPlusFont == NULL) clGrp.Error("Error", "NormalEditPlusFont Create Fail");
	NormalEditPlusFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(NormalEditPlusFont_Underline == NULL) clGrp.Error("Error", "NormalEditPlusFont Create Fail");

	size = GetTextFormat(nation,	"ChatEditFont",			"Size");
	thick= GetTextFormat(nation,	"ChatEditFont",			"Thick");
	GetTextFormatFont	(nation,	"ChatEditFont",			font );
	ChatEditFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(ChatEditFont == NULL) clGrp.Error("Error", "ChatEditFont Create Fail");
	ChatEditFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(ChatEditFont_Underline == NULL) clGrp.Error("Error", "ChatEditFont Create Fail");

	size = GetTextFormat(nation,	"WinLoseFont",			"Size");
	thick= GetTextFormat(nation,	"WinLoseFont",			"Thick");
	GetTextFormatFont	(nation,	"WinLoseFont",			font );
	WinLoseFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(WinLoseFont == NULL) clGrp.Error("Error", "WinLoseFont Create Fail");
	WinLoseFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(WinLoseFont_Underline == NULL) clGrp.Error("Error", "WinLoseFont Create Fail");

	size = GetTextFormat(nation,	"IDFont",				"Size");
	thick= GetTextFormat(nation,	"IDFont",				"Thick");
	GetTextFormatFont	(nation,	"IDFont",				font );
	IDFont = CreateFont(size, 0, 0, 0, thick, FALSE, FALSE, 0, sicharset, 0, 0, 0, 0, font);
	if(IDFont == NULL) clGrp.Error("Error", "IDFont Create Fail");
	IDFont_Underline = CreateFont(size, 0, 0, 0, thick, FALSE, TRUE, 0, sicharset, 0, 0, 0, 0, font);
	if(IDFont_Underline == NULL) clGrp.Error("Error", "IDFont Create Fail");
}

// 게임에 사용된 폰트를 해제한다. 
void _clBattle::DeleteGameFont()
{
	DeleteObject(TitleFont);
	DeleteObject(StaffFont);
	DeleteObject(HelpFont);
	DeleteObject(GetExpFont);
	DeleteObject(BriefingFont);
	DeleteObject(SaveInfoFont);
	DeleteObject(VersionInfoFont);
	DeleteObject(VersionInfoPlusFont);
	DeleteObject(NormalEditFont);
	DeleteObject(NormalEditPlusFont);
	DeleteObject(ChatEditFont);
	DeleteObject(WinLoseFont);
	DeleteObject(IDFont);

	DeleteObject(TitleFont_Underline);
	DeleteObject(StaffFont_Underline);
	DeleteObject(HelpFont_Underline);
	DeleteObject(GetExpFont_Underline);
	DeleteObject(BriefingFont_Underline);
	DeleteObject(SaveInfoFont_Underline);
	DeleteObject(VersionInfoFont_Underline);
	DeleteObject(VersionInfoPlusFont_Underline);
	DeleteObject(NormalEditFont_Underline);
	DeleteObject(NormalEditPlusFont_Underline);
	DeleteObject(ChatEditFont_Underline);
	DeleteObject(WinLoseFont_Underline);
	DeleteObject(IDFont_Underline);
}



//--------------------------------------------------------------------------
// Name: WindowProc()
// Desc: 윈도우 Procedure
//-------------------------------------------------------------------------- 
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	static int FirstSwitch=TRUE;
	SHORT oldkeystatus=0;
	
	// actdoll (2004/05/04 17:22) : = 에디트 컨트롤 = 메시지 파서를 동작시킨다.
	if( Handler_OnlineEditControlEx::Recv_KeyMsg( hWnd, message, wParam, lParam ) )		return 0;

	switch(message)
	{
//	// actdoll (2004/01/20 10:29) : IME관련 추가사항
//	case WM_IME_STARTCOMPOSITION:
//		{
//			// Add by OrionYOung
//			// 텍스트 입력 창이 아닌곳에서 한글/칸지 모드 강제 종료.
//			HIMC		hImc = NULL;
//			hImc = ImmGetContext(hWnd);
//			if (hImc!=NULL)
//			{
//				ImmSetOpenStatus(hImc,FALSE);
//				ImmReleaseContext(hWnd, hImc);
//			}
//			
//		}
//		break;

		// 클라이언트 영역이 아닐때에는 커서가 보인당~~~
	case WM_SETCURSOR:
		{
			DetermineShowCursor(LOWORD(lParam));
			
		}

		break;
	// 에디트 컨트롤에서 F10키 땜시 막아버림!
/*	case WM_MENUSELECT:
	case WM_INITMENU:
		{
			return 0;
		}
		break;
	case WM_ENTERIDLE:
		{
			if (wParam == MSGF_MENU)  return 0;

		}
		break;*/
	
	

	case WM_TIMER:
		break;

	case WM_CHAR:

		return 0;
	case WM_MOUSEWHEEL:
		{
			WORD zDelta;
			zDelta = HIWORD(wParam);

			if (zDelta > 0) {
				IpD.WheelUp = TRUE;
				IpD.WheelDown = FALSE;
			}
			else if (zDelta < 0)
			{
				IpD.WheelUp = FALSE;
				IpD.WheelDown = TRUE;

			}
			
			
		}
		return 0;
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pMinMax = (MINMAXINFO*) lParam;
			
			DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
			DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
			DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
			DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );
			
			pMinMax->ptMinTrackSize.x = 800  + dwFrameWidth * 2;
			pMinMax->ptMinTrackSize.y = 600 + dwFrameHeight * 2 + 
			/*	dwMenuHeight +*/ dwCaptionHeight;
			
			pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
			pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
		}
		return 0;


	case WM_MOUSEMOVE:
		if (WindowModeSwitch)
		{
			
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);		

			IpD.Mouse_X = point.x;
			IpD.Mouse_Y = point.y;


		}
		else {
	
			IpD.Mouse_X=LOWORD(lParam);
			IpD.Mouse_Y=HIWORD(lParam);
		}
//--> Test Code (sagolboss)
		/*
		if( GetMouseHandlerMgr()->OnMouseEvent(WM_MOUSEMOVE, IpD.Mouse_X, IpD.Mouse_Y))
			return 0;
		*/
//<--
		GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, 0, TRUE);
		
		// actdoll (2004/01/15 22:33) : 디버그 입력
//		g_DbgDisp.Proc_GetMousePos( LOWORD(lParam), HIWORD(lParam) );
		return 0;

	case WM_LBUTTONDOWN:
//--> Test Code (sagolboss)
		/*
		if( GetMouseHandlerMgr()->OnMouseEvent(WM_LBUTTONDOWN, IpD.Mouse_X, IpD.Mouse_Y))
			return 0;
		*/
//<--
		GD.KB.PushKeyBuffer(TRUE, FALSE, IpD.Mouse_X, IpD.Mouse_Y, 0, FALSE);
		IpD.LeftOneSwich++;
		IpD.LeftPressSwitch = TRUE;
		IpD.LeftPressDoneSwitch = FALSE;

		// actdoll (2004/01/15 22:33) : 디버그 입력
//		g_DbgDisp.Proc_GetMousePush( message );

		return 0;

	case WM_LBUTTONUP:
//--> Test Code (sagolboss)
		/*
		GetMouseHandlerMgr()->OnMouseEvent(WM_LBUTTONUP, IpD.Mouse_X, IpD.Mouse_Y);
			//return 0;
		*/
//<--
		GD.KB.PushKeyBuffer(FALSE, FALSE, IpD.Mouse_X, IpD.Mouse_Y, 0, FALSE);
		IpD.LeftOneSwich = 0;
		IpD.LeftPressSwitch = FALSE;
		IpD.LeftPressDoneSwitch = TRUE;

		// 더블 클릭이 계속 반복되는 것을 막기 위해.. 
		if(IpD.IsDoubleClick()== FALSE)
		{
			IpD.PrevLeftUp2Clock = IpD.PrevLeftUp1Clock;
			IpD.PrevLeftUp1Clock = CurrentClock;
		}
		else
		{
			IpD.PrevLeftUp2Clock = 0;
			IpD.PrevLeftUp1Clock = CurrentClock;
		}
		// actdoll (2004/01/15 22:33) : 디버그 입력
//		g_DbgDisp.Proc_GetMousePush( message );
		return 0;

	case WM_RBUTTONDOWN:
//--> Test Code (sagolboss)
		/*
		if( GetMouseHandlerMgr()->OnMouseEvent(WM_RBUTTONDOWN, IpD.Mouse_X, IpD.Mouse_Y))
			return 0;
		*/
//<--
		GD.KB.PushKeyBuffer(FALSE, TRUE, IpD.Mouse_X, IpD.Mouse_Y, 0, FALSE);
		IpD.RightPressSwitch=TRUE;
		// actdoll (2004/01/15 22:33) : 디버그 입력
//		g_DbgDisp.Proc_GetMousePush( message );

		return 0;

	case WM_RBUTTONUP:
//--> Test Code (sagolboss)
		/*
		if( GetMouseHandlerMgr()->OnMouseEvent(WM_RBUTTONUP, IpD.Mouse_X, IpD.Mouse_Y))
			return 0;
		*/
//<--

		GD.KB.PushKeyBuffer(FALSE, FALSE, IpD.Mouse_X, IpD.Mouse_Y, 0, FALSE);
		IpD.RightPressSwitch=FALSE;
		IpD.RightPressDoneSwitch=TRUE;
		// actdoll (2004/01/15 22:33) : 디버그 입력
//		g_DbgDisp.Proc_GetMousePush( message );

		return 0;

	case WM_SYSKEYUP:
		GD.KB.SetAKey(wParam, FALSE);
		
		switch(wParam)
		{
		case VK_F10:
			return 0;
		}
		break;

	case WM_KEYUP:
	    GD.KB.SetAKey(wParam, FALSE);
		if(wParam == VK_RETURN)		
		{
			if(pGame->pOnlineWorld)
				pGame->pOnlineWorld->bReturnKeyDown = FALSE;
		}

		return 0;

	case WM_USER_SYSRETURN:
		{			
			switch(wParam)
			{
			case VK_F10:
				oldkeystatus=GD.KB.GetAKey(wParam);
				GD.KB.SetAKey(wParam, TRUE);
				break;
			}
			
			// 키보드 입력을 기억한다.
			// 이전의 키 상태가 FALSE일때에만 (누르고 있는 상태는 한번의 입력만 받는다.)
			if(oldkeystatus==FALSE)
				GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, wParam, FALSE);
			
			return 0;
		}
	case WM_SYSKEYDOWN:		
		switch(wParam)
		{
			case VK_F10:
				oldkeystatus=GD.KB.GetAKey(wParam);
				GD.KB.SetAKey(wParam, TRUE);
				break;
		}

		// 키보드 입력을 기억한다.
		// 이전의 키 상태가 FALSE일때에만 (누르고 있는 상태는 한번의 입력만 받는다.)
		if(oldkeystatus==FALSE)
		   GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, wParam, FALSE);

		return 0;

	case WM_USER_RETURN:
		{			
//			// 스크립트가 연주중이지 않을 때에만...
//			EnterKeyPressDoneSwitch=TRUE;
//			if(pGame->pOnlineWorld)
//				pGame->pOnlineWorld->bReturnKeyDown = TRUE;
//			
//			// 엔터키가 입력되었을때 
//			if(wParam=='\n' || wParam=='\r')
//			{				
//				if(GameStatus==GS_GAME)
//				{
//					if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
//					{
//						// native 언어 모드
//						ChangeLanguage(TRUE);
//						
//						pGame->pBattle->m_EditControl.SetFocus();
//						pGame->pBattle->m_EditControl.SetString("");
//						
//						// 누구에게 보내는 메시지인지가 결정된다. 
//						if(GetAsyncKeyState(VK_CONTROL)<-1)
//						{
//							MessageMode=MESSAGEMODE_ALLY;
//						}
//						else
//						{
//							MessageMode=MESSAGEMODE_ALL;
//						}
//					}
//				}
//			}
		}
		return 0;
    case WM_KEYDOWN:		
		switch(wParam)
		{
#ifdef	_DEBUG
			// 디버그 모드일때만 F10버튼을 누르면 /InfoShow명령어가 토글로 가동된다.
		case 0xc0:
			pGame->pOnlineWorld->m_InfoFlag = !pGame->pOnlineWorld->m_InfoFlag;
			break;
			
#endif//_DEBUG
		case VK_RETURN:
			{				
				// 스크립트가 연주중이지 않을 때에만...
				EnterKeyPressDoneSwitch=TRUE;
				if(pGame->pOnlineWorld)
					pGame->pOnlineWorld->bReturnKeyDown = TRUE;
				
				// 엔터키가 입력되었을때 
				if(wParam=='\n' || wParam=='\r')
				{				
					if(GameStatus==GS_GAME)
					{
						// ===============================================================================
//						if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
//						{
//							pGame->pBattle->m_EditControl.SetFocus();
//							pGame->pBattle->m_EditControl.SetString("");
//							
//							// 누구에게 보내는 메시지인지가 결정된다. 
//							if(GetAsyncKeyState(VK_CONTROL)<-1)
//							{
//								MessageMode=MESSAGEMODE_ALLY;
//							}
//							else
//							{
//								MessageMode=MESSAGEMODE_ALL;
//							}
//
//							// native 언어 모드
//							ChangeLanguage(TRUE);
//						}
						// ===============================================================================
						// ===============================================================================
						// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
						// 안켜진 상태에서 포커스가 왔을 경우 입력을 받는 것을 개시한다.
						if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
						{
							OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat );
							pOecEx->Clear();
							Handler_OnlineEditControlEx::SetFocus( pGame->pBattle->m_hOecChat );
							
							// 누구에게 보내는 메시지인지가 결정된다. 
							if(GetAsyncKeyState(VK_CONTROL)<-1)
							{
								MessageMode=MESSAGEMODE_ALLY;
							}
							else
							{
								MessageMode=MESSAGEMODE_ALL;
							}
						}
						// ===============================================================================
					}
				}
			}
			return 0;

		case VK_TAB:
			if(GameStatus==GS_GAME)
			{
				SmallMapToggleOperation();
			}
			break;

		case VK_ESCAPE:
			if(GameMenuShowSwitch == TRUE)
			{
			    SetGotoPreviousMenu();
				GD.m_bBlackCheck = TRUE;
			}
	        else if(GameStatus == GS_GAME)
			{
				DWriteStringToFile("나가기 버튼을 눌렀을때");
				// 기본 마우스로 변경한다.
				SetMouse(MOUSE_NORMAL);

				// 게임 메뉴를 보여 주도록 한다. 
			    GameMenuShowSwitch = TRUE;
				GD.m_bBlackCheck = FALSE;
			}
			return 0;
			break;

		case VK_SPACE:
			if(GameStatus==GS_GAME)
			{
				// 지도 편집기에서는 언덕이 깨진곳으로 화면이 이동한다.
				if(MapEditMode==TRUE)
				{
					for(SHORT i=0;i < clifMap.GetYsize();i++)
						for(SHORT j=0;j < clifMap.GetXsize();j++)
						{
							if((clifMap.GetNewBlkMode(j, i) || Map.GetCastleMode(j, i) )&& clifMap.GetLevelInfo(j, i) == HILL_SLOPE)
							{
								GD.SetScreenMove(j, i);
								break;
							}
						}

				}
				// 글자를 입력중인 것이 아닐때 & 스크립트 연주중이지 않을때.  
				// ===============================================================================
//				else if(pGame->pBattle->m_EditControl.IsFocus() ==FALSE)
				// ===============================================================================
				// ===============================================================================
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				else if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
				// ===============================================================================
				{
				    if(GD.BattleFieldX>=0 && GD.BattleFieldY>=0)
					{
			            GD.SetScreenMove(GD.BattleFieldX, GD.BattleFieldY);
					}
				}
			}
			return 0;
			break;
		case VK_HOME:
			if(GameStatus==GS_GAME)
			   GD.SetScreenMove(GD.HomeX, GD.HomeY);
			return 0;

		case VK_F11:
			// 현재 게임중인 사용자들의 정보를 보여준다. 
			GD.PlayerMessageDelay=MAX_MESSAGE_DELAY;
			return 0;
			break;

         // 화면 캡춰기능 
		case VK_F12:
			if(abs(CurrentClock  - CaptureClock ) > 1500)
			{
				CaptureClock = CurrentClock;

				// 화면을 캡춰한다. 
				Capture.ScreenCapture();
			}

			return 0;
			break;

		default:
			oldkeystatus=GD.KB.GetAKey(wParam);
		    GD.KB.SetAKey(wParam, TRUE);
			break;
		}

		// 키보드 입력을 기억한다.
		// 이전의 키 상태가 FALSE일때에만 (누르고 있는 상태는 한번의 입력만 받는다.)
		if(oldkeystatus==FALSE)
		   GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, wParam, FALSE);

		return 0;

///////////////////////////////////////////////////////////

	case WM_CREATE:
		return 0;
    case WM_ACTIVATEAPP:
        //bActive = (BOOL)wParam && GetForegroundWindow() == hWnd && !IsIconic(hWnd);
		bActive = (BOOL)wParam;// && GetForegroundWindow() == hWnd;
		// 초기화 중인 경우에는 무조건 액티브 되도록
		if( g_bMainInit ) bActive = TRUE;	

		if(bActive==TRUE)
		{
			pGame->bPause=FALSE;
			
			if(FirstSwitch==FALSE)
			{
			   Restore();

			   // 포커스를 어디로 설정하느냐를 결정한다. 
			   if(pGame->pBattle)
			   {

					// ===============================================================================
//					pGame->pBattle->m_EditControl.SetString("");
					// ===============================================================================
					// ===============================================================================
					// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
					if( Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat ) )
					{
						Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat )->Clear();
					}
					// ===============================================================================
			   }

			   ::SetFocus(pGame->Hwnd);

			}
		}
/*		else
		{
			if(pGame->pOnlineWorld)
				pGame->pOnlineWorld->SetMainPollStatus(ON_MAINPOLL_FORCED_LOGOUT);
		}
*/
		FirstSwitch=FALSE;
		return 0;

	case WM_PAINT:
		{
			HDC				hDC;
			PAINTSTRUCT		ps;
			
			hDC = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		/*
		if (WindowModeSwitch) {

			if (GameStatus == GS_GAME)
			{
				GameDraw4();
				
			}
			else {
				if (pGame) { 
					if (pGame->pOnlineWorld) {
						
						
						pGame->pOnlineWorld->UpdateScreen(GETSCREENSURFACE(pGame->pOnlineWorld),FALSE,TRUE);
					}
				}
			}
		}
		*/

		break;

	case WM_PALETTECHANGED:
		if((HWND)wParam==hWnd)
			break;

	case WM_QUERYNEWPALETTE:
		
		if(clGrp.SetPalette() == DDERR_SURFACELOST)
		{
			clGrp.Restore();
		}

		break;

	case WM_DESTROY:
	

		if(pGame)
			pGame->finiObjects();

		// 화일 맵핑 핸들을 해제한다. 
		CloseDupGameHandle();

		// 훗킹을 제거한다.
	//	removehook();

		PostQuitMessage(0);
		return 0;
	}

	// 특정 메세지 아이디을 원래대로 돌린다.
//	message = pGame->OriginallyWndMsg(message);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//--------------------------------------------------------------------------
// Name: InitQuit()
// Desc: 종료를 위한 준비를 한다. 
//--------------------------------------------------------------------------
BOOL InitQuit()
{
	return TRUE;
}


//--------------------------------------------------------------------------
// Name: Restore()
// Desc: 미니마이즈된 상태에서 다시 복구할때 사용되는 함수 
//--------------------------------------------------------------------------
BOOL Restore()
{
	 clGrp.Restore();

     return FALSE;
}

static draw1switch = FALSE;
static draw2switch = FALSE;
static draw3switch = FALSE;
static draw4switch = FALSE;

void GsGame()
{
	SI32 i;
	static int lastdrawframe;
	// actdoll (2004/10/01 10:56) : 게임에서 마지막 메시지 출력을 위한 루틴
	static BOOL		bLastThrough = false;
	static SHORT	shPrevGameStatus	= GS_GAME;

	if(pGame->pOnlineWorld)
	{
		UI16 uiResult = pGame->pOnlineWorld->pOnlineClient->DataProc();
		pGame->pOnlineWorld->pOnlineClientChatting->DataProc();
		if(uiResult)    pGame->pOnlineWorld->ReceiveMessageProc(uiResult);

		// 전투 메세지가 이다면... 
		SI32 siSize;
		if(PopClientBattleMsg(pGame->pOnlineWorld->m_TempBuffer, &siSize))
		{
			if( !pGame->pOnlineWorld->pOnlineClient->SendBattleMsg(pGame->pOnlineWorld->m_TempBuffer, siSize) )
			{
				SetMessage("Sending Failure");
			}
		}
	}

//	g_FileLoadManager.CheckXSprCompleteQueue();
	g_FileLoadManager.CheckXSprFreeCompleteQueue();

	// actdoll (2004/09/17 17:20) : 
	//	이 부분은 막바지에 게임 승패 상태가 결정되었을 경우 한번 더 찍어주기 위한 루틴이다.
	if( !bLastThrough )	GameProcess();

	if( GameStatus == GS_INITWIN || GameStatus == GS_INITLOSE )	
	{
		bLastThrough		= true;
		shPrevGameStatus	= GameStatus;
		GameStatus			= GS_GAME;
		draw1switch			= FALSE;
		draw2switch			= FALSE;
		draw3switch			= FALSE;
		draw4switch			= FALSE;
	}

	
	// 미니마이즈된 상태에서는 찍어줘서는 안된다....
	if(bActive == FALSE)return ;

	// 게임 초반의 장면은 그려주지 않는다. 
	if(GD.Frame < 3 )return ;

	UI32 diff = abs(CurrentClock - LastDrawClock);

	if(draw1switch == FALSE)
	{ 
		if(GameStatus == GS_GAME)
		{
			GameDraw1();
			draw1switch = TRUE;
		}
	}
	else if(draw2switch == FALSE)
	{
		if(GameStatus == GS_GAME)
		{
			SHORT id;


			SetScrollProcSwitch(FALSE);

			for(i = 0; i < GD.CharOrderIndex; i++ )
			{
				id = GD.CharOrder[i];
				clifChar.SetDrawFrame(id, 0);
			}
			
			GameDraw2();
			draw2switch = TRUE;

		}
	}
	else if(draw3switch == FALSE)
	{
		if(GameStatus == GS_GAME)
		{
			GameDraw3();
			draw3switch = TRUE;
		}
	}
	else if(draw4switch == FALSE)
	{
		if(GameStatus == GS_GAME)
		{
			GameDraw4();
			draw4switch = TRUE;
		}
	}
	// 멀티 테스트일 때는 화면 드로우할 때 프레임을 건너뛰지 않는다.
	else if(diff >= (UI32)(max(50, pGame->Config.GetRealGameSpeedClock())))
	{
		draw1switch = FALSE;
		draw2switch = FALSE;
		draw3switch = FALSE;
		draw4switch = FALSE;
		LastDrawClock = CurrentClock;
	}

	// actdoll (2004/10/01 11:00) : 
	//	루프 한바퀴를 몽짱 돌았을 경우
	if( bLastThrough && !draw1switch && !draw2switch && !draw3switch && !draw4switch )
	{
		EndGame();
		bLastThrough		= false;
		GameStatus			= shPrevGameStatus;
		shPrevGameStatus	= GS_GAME;
	}
}



void GsInitQuit()
{
	InitQuit();
	
	GameStatusBuffer=GS_QUIT;
	GameStatus=GS_INITFADEOUT;

}

void GsQuit()
{
	if( pGame )	
	{
		delete pGame ;
		pGame = NULL;
	}

//	FreeIMEDll();
}


/*
void GsInitLogo()
{
	InitLogo();
	GameStatus=GS_LOGO;
}
*/
/*
void GsLogo()
{
	SHORT status;

	status=Logo();
	if(status!=0)
	{
	   GameStatus=GS_INITINITMENU;
	}
}
*/
/*
void GsInitHQLogo()
{
	InitHQLogo();
	GameStatus=GS_HQLOGO;
}
*/

/*
void GsHQLogo()
{
	SHORT status ;

	status=HQLogo();
	if(status!=0)
	{
		
		if(status==1)
		{
		// 임진록2의 경우
		#ifdef _COMPANYLOGO
			GameStatus	=	GS_INITSAMSUNGLOGO;
		#else
			GameStatus	=	GS_INITINITMENU;
		#endif

		}
		else
			GameStatus = GS_INITINITMENU;
	}
	
}
*/
/*
void GsInitSamsungLogo()
{
	
	InitSamsungLogo();
	GameStatus=GS_SAMSUNGLOGO;
}
*/
/*
void GsSamsungLogo()
{
	SHORT status = SamsungLogo();

	if(status!=0)
	{

		// 로고 동영상을 보여준다.
		if(status==1)
			GameStatus=GS_INITLOGO;
		else
			GameStatus=GS_INITINITMENU;
		

	}
}
*/

void GsInitStaff()
{
//	InitStaff();
//	GameStatus=GS_STAFF;
}

void GsStaff()
{
/*
	if(Staff()!=0)
	{
		GameStatusBuffer=GS_INITINITMENU;
		GameStatus=GS_INITFADEOUT;
	}
*/
}

void GsInitMenu()
{

	SHORT status=InitMenu();
	if(status)
	{
		GameStatusBuffer=status;
		GameStatus=GS_INITFADEOUT;
	}
}

void GsInitStageGame()
{
	
	GameStatus = GS_GAME;
}


void GsInitOnlineBattleGame()
{
	// 게임에 들어가기 전에 초기화한다. 

	// actdoll (2004/11/05 16:16) : IME부분
	//	만약 포커스가 맞춰진 에디트 박스가 존재할 경우 전부 클리어시킨다.
	//	이렇게 하면 IME 관련된 모든 리소스가 정리된다.
	Handler_OnlineEditControlEx::ClearCurrentFocus();


	if(MapEditMode == TRUE)
	{
		// 전투 클래스를 생성한다.
		pGame->InitBattle();
		
		// 기타 정보를 초기화한다. 
		pGame->InitEtc();
	}

	InitGame1();

	InitOnlineBattleGame();

	InitGame2();
	GameStatus=GS_GAME;


}

void GsInitRestart()
{
	GameStatus=GS_RESTART;
}

void GsRestart()
{
	
}

void GsInitWin()
{
	InitWin();
	GameStatus=GS_WIN;
}

void GsWin()
{
	SHORT status=Win();
	if(status)
	{
		GameStatusBuffer=status;
		GameStatus=GS_INITFADEOUT;
	}
}

void GsInitLose()
{
	InitLose();
	GameStatus=GS_LOSE;
}

void GsLose()
{
	SHORT status=Lose();
	if(status)
	{
		GameStatusBuffer=status;
		GameStatus=GS_INITFADEOUT;
	}
}

void GsInitBattle()
{
	if(pGame)
	{
		// 기타 정보를 초기화한다. 
		pGame->InitEtc();

		// 전투 클래스를 생성한다.
		pGame->InitBattle();

		GameStatus = GS_INITONLINEWORLD;
	}
}


void GsInitGameMenu()
{
	SetMouse(MOUSE_NORMAL);
	
	InitGameMenu();
	GameStatus=GS_GAMEMENU;


}

void GsGameMenu()
{
	
	
   	GsGame();
   	
	SHORT status = GameMenu();
	
	if(GameStatus == GS_GAMEMENU)
	{
					   
		if(status)
		{
			
			// 게임 메뉴를 없앤다. 
			GameMenuShowSwitch = FALSE;
			
			switch(status)
			{
			case GS_GAME:
				GameStatus=status;
				break;
				
				// 게임을 다시한다. 
			case GS_INITRESTART:
				GameStatusBuffer=status;
				GameStatus=GS_INITFADEOUT;
				EndGame();
				break;
				
			case GS_INITINITMENU:
				OperateGameMenuInitInitMenu();
				break;
			}
		}
	}
}

void GsInitFadeOut()
{
	InitFadeOut();
	GameStatus=GS_FADEOUT;
}

void GsFadeOut()
{
	if(FadeOut()==TRUE)
	{
		GameStatus=GameStatusBuffer;
	}
}





VOID	GsInitOnlineWorld()
{
	pGame->InitOnlineWorld();
	GameStatus	 = GS_ONLINEWORLD;
}

VOID	GsOnlineWorld()
{
	UI16	uiGameStatus;

	uiGameStatus	=	pGame->OnlineWorld();

	if(uiGameStatus)	
		GameStatus	=	SI16(uiGameStatus);	
}



// 게임메뉴에서 종료하기를 눌렀을 때 처리 함수. 
BOOL OperateGameMenuInitQuit()
{
	GameStatusBuffer = GS_INITQUIT;
	GameStatus       = GS_INITFADEOUT;

	return TRUE;			
}

// 게임메뉴에서 초기메뉴를 눌렀을 때 처리 함수. 
BOOL OperateGameMenuInitInitMenu()
{
	GameStatusBuffer = GS_ONLINEWORLD;
	GameStatus=GS_INITFADEOUT;
	// 게임을 초기화한다. 
	EndGame();
	
	return TRUE;
}

// 커서를 보여야할지 말아야 할지 경정한다.
void DetermineShowCursor(DWORD nHittest)
{
	static showcursor = 0;

	if (WindowModeSwitch)
	{
		switch(nHittest)
		{
			case HTCLIENT:
				if (showcursor) {
					while(ShowCursor(FALSE) >= 0);
					showcursor = 0;
				}
				break;
			default:
				if (!showcursor) {
					while(ShowCursor(TRUE) < 0);
					showcursor = 1;
				}
				break;

		}
	}
	

}
