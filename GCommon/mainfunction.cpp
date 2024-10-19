//--------------------------------------------------------------------
//  ��� : ���°�
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

// actdoll (2004/05/04 16:33) : �� ����Ʈ ��Ʈ�ѷ� ����
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"

//#include "..\\SCCommon\\keyhook.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

//--> Test Code (sagolboss)
// actdoll (2004/10/01 11:04) : ȸ��� �ҽ� ����
//#include "..\GersangUI\UIGSMgr.h"
//#include "..\GersangUI\UIGSSurfaceBufferMgr.h"
//#include "..\CommonInterface\MouseHandlerMgr.h"
//<--


//extern HQFileManager	HQFile;
extern _clCapture		Capture;							// ȭ�� ĸ�� ���� Ŭ���� 


extern SI32  WindowModeSwitch;
extern	_Text 				Text;								// �ؽ�Ʈ 
extern char					SavePath[];								// ���� ������ ��ġ�� ��� 
extern clock_t				StartClock, PrevClock;
extern clock_t				MonitorMacroClock1, MonitorMacroClock2;

extern	_clGame* pGame;
extern	UI32					GameScreenMode;
extern _InputDevice				IpD;
extern	SHORT					GameStatus;							// ������ ���� ��Ȳ�� �˸��� ���� 
extern  _Chat 					Chat;
extern	SHORT					MessageMode;						// �޽����� �������� ���°�?
extern	SHORT 					GameMenuShowSwitch;					// �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	_MainData				GD;									// ������ �ֵ����� 
extern	clock_t					CurrentClock;						// �ð� ���� 
extern	SHORT 					MapEditMode;						// ���� ���� ����ΰ�?
extern	cltMapInterface			clifMap;							// �ʰ��� �������̽� 
extern	_Map 					Map;
extern	int						CDStopSwitch;						// CD������ ���� ��Ų��. 
extern	int						bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
extern	BOOL					g_bMainInit;						// ���� �ʱ�ȭ ���� ��� ������ ��Ƽ�갡 �ǵ���
extern	BOOL					GameDropSwitch ;


extern	SI32					UserPlayerNumber;					// ����� ���� 
extern	cltCharInterface		clifChar;							// ĳ���� ���� �������̽� 
extern	XSPR 					PannelSpr;
extern	BOOL 					DPLauncherSwitch;					// dp launcher�� ���� �۵��Ǿ��°�
extern	_FileVary 				FV;
extern	_MyNetGame				MyNet;
extern	_FindPathGame		   *FindPathGame;
extern	char 					CDPath[];

extern	CCharFileLoadManager	g_FileLoadManager;				// ������ ĳ����
extern	CItem					*g_pItem;						// ������ Ŭ����

//------------------------------------
// Direct X �� ������ 
//------------------------------------
extern  LPDIRECTDRAWSURFACE7	lpPannelSurface;					// �ǳڿ� Surface


extern	int						EnterKeyPressDoneSwitch;
extern	DWORD					CaptureClock;						// ������ ĸ���� �ð�. 
extern	DWORD					LastDrawClock;
extern	BOOL					ScrollProcSwitch;
extern	SHORT					GameStatusBuffer;

extern  SHORT                   GameStatus;
extern  BOOL                    GameDraw4();

int     OriginalScreenXsize, OriginalScreenYsize; 
SI32 g_siAdType = 0;	

/////////////////////////////////////
// ���� ���� 
////////////////////////////////////
SoundManager *g_pSoundManager = NULL;

static void DetermineShowCursor(DWORD nHittest);


// ���ʿ��� ������ �����Ѵ�.
void DeleteUnusableData()
{
	// ���� ���� ������ ���� �ִ� ���°� �ƴ϶�� �Ѵ�.
	DeleteData();

//	HQFile.FileDeCode( "Data.hq", "Data" );

	CHAR	szFileNameBuffer[1024];

	// ������ ��ġ�� ��ο� ����� ��θ� ��� �´�. 
	// actdoll (2004/07/14 14:05) : ���Ĺ��� ��Ȳ �߻�!!!
	//		������Ե� Carom 3D�� ���� Ű���� �̸��� ������ ����� 
	//		���� ������ �߻��� �� �����Ƿ� �����ؼ� ������.
//	GetPath(SavePath);
//	g_siAdType = GetAdType();
	GetCurrentDirectory( MAX_PATH, SavePath );
	strcat( SavePath, "\\" );
	g_siAdType = 0;

	GetFileNamePath("ErrorCode.txt", SavePath, szFileNameBuffer);
	DeleteFile(szFileNameBuffer);
}

// ������ 
_clGame::_clGame()
{
	pOnlineWorld = NULL;
	pBattle = NULL;

	pNumberRender   = NULL;
	pItemInfoWindow = NULL;
	m_pEffectDataMgr= NULL;

	LanguageCode = ELANGAGECODE_KOREAN;
}

// �Ҹ���. 
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
// actdoll (2004/10/01 11:04) : ȸ��� �ҽ� ����
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
		// ���� �ػ󵵸� �����Ѵ�. 
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

	// ���� �ڵ鿡 ���� �ѱ��. 
	Hwnd=hwnd;
	hInst=hInstance;

	// robypark 2004/12/21 18:33
	// ���� ����� ����� ������ �ߺ� ������ �����ϵ��� ����
#ifdef	_DEBUG
	if( !WindowModeSwitch )
	{
#endif//_DEBUG
		// �ߺ��� ������ �ִ��� Ȯ���Ѵ�. 
		if(CheckDupGame(szName, szTitle) == TRUE)
		{
			SetForegroundWindow(FindWindowEx(NULL, NULL, szName, szTitle));
			CloseDupGameHandle();

			return FALSE;
		}
#ifdef	_DEBUG
	}
#endif//_DEBUG

	// �޼��� ��ŷ�� �����Ѵ�.(Mouse move, mouse left button down)

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
    //  DirectDraw object �����
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



				// off screen�� �����Ѵ�. 
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
		// �ȷ�Ʈ ȭ���� �ҷ��� Palette�� �����Ѵ�.
		//////////////////////////////////////////////////////////

		if (!WindowModeSwitch) {
	 		LoadPalette();

			clGrp.SetAllPalette();
		}

		// 16��Ʈ �ȷ�Ʈ�� ���� �ִ´�. 
		clGrp.SetColorTable();
		
		
		return TRUE;
}

//--------------------------------------------------------------------------
// Name: doInit()
// Desc: ������ ���� 
//--------------------------------------------------------------------------
BOOL _clGame::Init(HINSTANCE hInstance, int nCmdShow)
{
	if(CreateGameWindow(hInstance, nCmdShow) == FALSE)return FALSE;

	// ���� �� ������ ������Ʈ���� ����Ѵ�.
//	SetServerVersion();


	// ���ʿ��� ������ �����Ѵ�.
	DeleteUnusableData();


	CoInitialize(NULL);

	// ���̷�Ʈ ��ο츦 �����Ѵ�.
	if(CreateGameDirectDraw() == FALSE)return FALSE;

	// Config ȭ���� �ҷ��´�. 
	if(Config.Load()==FALSE)
	{
		Config.Init();       // Config ������ ������� Default������ �����Ѵ�.
	}

	// ���� �ʱ�ȭ
	InitSoundManager();

	// timeGetTime()�� ������ �����Ѵ�.
	timeBeginPeriod(1);

	// Ŭ���� ������ ȭ�� ��ü�� �����Ѵ�. 
	clGrp.SetClipFullScreen();

	// ������ ���۵� �ð��� ���Ѵ�. 
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
                break;		// ����
            }
			// TRUE�̸� �ٸ� �����忡�� �Ѿ�� �޽����̴�.
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
						// actdoll (2004/05/05 17:57) : ���������� �Ⱦ���.
						if( pmsg->message == WM_SYSKEYDOWN )	pmsg->message = WM_USER_SYSRETURN;
					}
				}
			}

			// Ư�� �޼��� ���̵� �����Ѵ�.
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
				
				// �����찡 Ȱ��ȭ �Ǿ� ���������� ȿ������ ����. 
				if(bActive == TRUE)
				{
					// ȿ������ �����Ѵ�.
					if(pGame)
					{
						if(pGame->m_pEffectDataMgr)
							pGame->m_pEffectDataMgr->Pop();
					}
					
					// fade inȿ���� ó���Ѵ�. 
					FadeIn();
				}
				switch(GameStatus)
				{
					
				case GS_GAME:					GsGame();						break;	// ���� �� - ����
					
				case GS_INITQUIT:				GsInitQuit(); 					break;	// �ʵ忡�� ������ �� �ʱ�ȭ - 1ȸ (�̶� GS_INITFADEOUT, GS_FADEOUT�� �����)
					
				case GS_QUIT:					GsQuit();						break;	// �ʵ忡�� ������ �� - ���� (���� ����)
					
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
					
					// �¶��λ󿡼��� ����. (�ʱ�ȭ)
				case GS_INITONLINEBATTLEGAME:	GsInitOnlineBattleGame();		break;	// ���� �� �ʱ�ȭ - 1ȸ
					
				case GS_INITRESTART:			GsInitRestart();				break;
					
				case GS_RESTART:				GsRestart();					break;
					
				case GS_INITWIN:				GsInitWin();					break;	// ���� �� ��� �ʱ�ȭ - �¸� �� �ش� - 1ȸ
					
				case GS_WIN:					GsWin();						break;	// ���� �� ��� ȭ�� - �¸� �� �ش� - ����
					
				case GS_INITLOSE:				GsInitLose();					break;	// ���� �� ��� �ʱ�ȭ - �й� �� �ش� - 1ȸ
					
				case GS_LOSE:					GsLose();						break;	// ���� �� ��� ȭ�� - �й� �� �ش� - ����
					
					// ���� �޴��� �ʱ�ȭ�Ѵ�. 
				case GS_INITGAMEMENU:			GsInitGameMenu();				break;	// ���� �� �ɼ� ȭ�� �ʱ�ȭ - 1ȸ
					
					// ���� �޴��� ó���Ѵ�. 
				case GS_GAMEMENU:				GsGameMenu();					break;	// ���� �� �ɼ� ȭ�� - ����
					
				case GS_INITFADEOUT:			GsInitFadeOut();				break;	// ��� ���̵� �ƿ� ��� - 1ȸ �ʱ�ȭ
					
				case GS_FADEOUT:				GsFadeOut();					break;	// ��� ���̵� �ƿ� ��� - ����
					
					
					//  ������ �¶��� ����
				case GS_INITONLINEWORLD:		GsInitOnlineWorld();			break;	// ���� �ʱ�ȭ - ���� ����� ���� ���� ���� - 1ȸ �ʱ�ȭ
					
				case GS_ONLINEWORLD:			GsOnlineWorld();				break;	// ���� �� - ����ȭ��, �ʵ� ����� ��� �ൿ - ����

				case GS_INITBATTLE:				GameStatus = GS_INITONLINEWORLD;break;
					
				}	
			} //if(bPause==FALSE)
		}
		
	}
	
			
}

// ���� �ʱ�ȭ
void _clGame::InitSoundManager()
{
	if( g_pSoundManager != NULL )	return;
	
	g_pSoundManager = new SoundManager(Hwnd, "EffectSound.txt", FALSE, FALSE, 20, 1); // Main Window Handle, ���ϸ�, EAX, 3D, Static ���԰���, Stream ���԰���

	if( g_pSoundManager == NULL )
	{
		clGrp.Error("Notice", Text.NotProperSoundCardMessage.Get());
	}
	else
	{
		// ���� ������ �����Ѵ�. 
	    Config.SetEffectSoundVolume( Config.m_siEffectSoundVolume);
	    Config.SetBackgroundMusicVolume( Config.m_siBackgroundMusicVolume);

		// ������ �¿캼������, ���Ϻ�������, ��
		g_pSoundManager->SetEmulate3DValue(1, 1, 3);
	}
}

// ���� ����
void _clGame::FreeSound()
{
#ifdef _OGG_PLAY
	if( GetBgmMgr())
		delete GetBgmMgr();
#endif

	// ������ ��� ������ ���� �Ѵ�.
	FreeMenuBackgroundMusic();

	if( g_pSoundManager )
	{
		delete g_pSoundManager;
		g_pSoundManager = NULL;
	}
}

//--------------------------------------------------------------------------
// Name: finiObjects()
// Desc: ���α׷��� �����Ҷ� ó�� �ϴ� ��ƾ 
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

	// ���ӿ� ���Ǵ� ��� ��ư���� �����Ѵ�. 
	FreeAllButtons();

	// ���� �Ҵ��� �޸� ���� 
	AllMemoryFree();
	
	// Config ������ �����Ѵ�.
	Config.Save();

	// Number Render
	if(pNumberRender)
		pNumberRender->Release();

	// Item Info Window
	if(pItemInfoWindow)
		pItemInfoWindow->Release();
	
	// ���� ����
	FreeSound();

	// ������ �����Ű�� ���� �Ѵ�.
	EndGame();

	// ���ǽ��� ��� �����Ѵ�. 
	CustomSurfaceRelease();
	clGrp.ReleaseDirectDraw();

	// �ؽ�Ʈ ���� 
	TextFree();

	// ������ �ý��� �÷��� �����Ѵ�. 
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

// ���� ������ �ʱ�ȭ�Ѵ�.
BOOL _clGame::InitBattle()
{
	pBattle = new _clBattle;
	if(pBattle == NULL)return FALSE;
	
	if(pBattle->Init() == FALSE)return FALSE;
	
	return TRUE;
}


// ��Ÿ �������� �ʱ�ȭ�Ѵ�. 
BOOL _clGame::InitEtc()
{
	// Number Render ����
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

	// ���� ��ư�� �ʱ�ȭ�Ѵ�. 
	ZeroMemory(GameButton, sizeof(_GameButton) * MAX_GAME_BUTTON);

	LastUpdateCharInfo		= 0;
	UpdateCharInfoDataIndex = 0;


	// ���ӿ� ���Ǵ� ��� ���ڿ��� �ʱ�ȭ�Ѵ�. (Credits ����)
	TextInit();
	
	

	// spr �̹��� ���� ������ �ҷ��´�.
	LoadImgFileList();

	// ���ӿ� ���Ǵ� ȭ�� �������� �ʱ�ȭ�Ѵ�. 
	FV.Init();

	// �㿡���Ϳ��� ����ϴ� File Open Box�������� �ʱ�ȭ 
	PopMapFileInitialize(pGame->Hwnd);

	// ��� �޴� �������� �ʱ�ȭ �Ѵ�. (������ư�� ��������� �� �޽���, ��ɹ�ȣ ����)
    ItemInit();				
	
	// �� ��ɿ� ���� Ư��ġ���� �����Ѵ�.
	OrderInit();
	
	// ��� ĳ���Ϳ����� Ư��ġ ���� 
	if(InitKindInfo() == FALSE)return FALSE;
//	InitKindInfo();

	// ���� �Ӽ��� �Ӽ��� �����Ѵ�. 
	InitAttackAtb();

	g_FileLoadManager.LoadInfo();

	g_pItem	= NULL;

	// ��� ���ݿ����� Ư��ġ �ο� 
	InitBulletKindInfo();
	// Ư�� ȿ���� ���� Ư��ġ �ο� 
	InitSmokeKindInfo();
    // ���к��� ������ ����Ѵ�. (sqrt���� sine��)
	InitMath();
	
	// Dplay�� �����͸� �ʱ�ȭ�Ѵ�. 
	MyNet.Init();

	// �¶��ο��� ����� ����ġ�� ������ ȭ�Ͽ��� �о�´�. 
	LoadExpData(g_fTestVersion);

	FindPathGame = new _FindPathGame(MAX_MAP_XSIZE, MAX_MAP_YSIZE);
	if(FindPathGame)
		FindPathGame->SetFindPathSize(MAX_MAP_XSIZE, MAX_MAP_YSIZE);


	// ��Ʈ�� �����. 
	CreateGameFont();
	
	// Color Table �� �����Ѵ�.
	InitColorTable();
	// ��� Tile Table�� �ʱ�ȭ �Ѵ�.
	
	clifMap.InitTile();

    // ��ư�� ��ġ �� ũ�⸦ �ʱ�ȭ �Ͽ� �����Ѵ�.
	OnceInitButton();

	// ���� �Է¿� ����Ʈ ��Ʈ���� �����.
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
		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
		RECT	rcDest = { 233, 582, 533, 600 };
		m_hOecChat	= Handler_OnlineEditControlEx::CreateControl( 0, &rcDest, 55 );
		// ===============================================================================
	}
	
	// ���ӿ� ��������Ʈ�� �ҷ��´�.
	LoadFnt();

	// ���ӿ� ���Ǵ� ��ư���� �ҷ��´�.(������)
	LoadAllButtons();
	
	InitFogImage();
	
	// �⺻ ���콺�� �����Ѵ�.
	SetMouse(MOUSE_NORMAL);
	

	return TRUE;
}

//  ���ӿ� ���� ��Ʈ�� �����.
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

// ���ӿ� ���� ��Ʈ�� �����Ѵ�. 
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
// Desc: ������ Procedure
//-------------------------------------------------------------------------- 
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	static int FirstSwitch=TRUE;
	SHORT oldkeystatus=0;
	
	// actdoll (2004/05/04 17:22) : = ����Ʈ ��Ʈ�� = �޽��� �ļ��� ���۽�Ų��.
	if( Handler_OnlineEditControlEx::Recv_KeyMsg( hWnd, message, wParam, lParam ) )		return 0;

	switch(message)
	{
//	// actdoll (2004/01/20 10:29) : IME���� �߰�����
//	case WM_IME_STARTCOMPOSITION:
//		{
//			// Add by OrionYOung
//			// �ؽ�Ʈ �Է� â�� �ƴѰ����� �ѱ�/ĭ�� ��� ���� ����.
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

		// Ŭ���̾�Ʈ ������ �ƴҶ����� Ŀ���� ���δ�~~~
	case WM_SETCURSOR:
		{
			DetermineShowCursor(LOWORD(lParam));
			
		}

		break;
	// ����Ʈ ��Ʈ�ѿ��� F10Ű ���� ���ƹ���!
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
		
		// actdoll (2004/01/15 22:33) : ����� �Է�
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

		// actdoll (2004/01/15 22:33) : ����� �Է�
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

		// ���� Ŭ���� ��� �ݺ��Ǵ� ���� ���� ����.. 
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
		// actdoll (2004/01/15 22:33) : ����� �Է�
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
		// actdoll (2004/01/15 22:33) : ����� �Է�
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
		// actdoll (2004/01/15 22:33) : ����� �Է�
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
			
			// Ű���� �Է��� ����Ѵ�.
			// ������ Ű ���°� FALSE�϶����� (������ �ִ� ���´� �ѹ��� �Է¸� �޴´�.)
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

		// Ű���� �Է��� ����Ѵ�.
		// ������ Ű ���°� FALSE�϶����� (������ �ִ� ���´� �ѹ��� �Է¸� �޴´�.)
		if(oldkeystatus==FALSE)
		   GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, wParam, FALSE);

		return 0;

	case WM_USER_RETURN:
		{			
//			// ��ũ��Ʈ�� ���������� ���� ������...
//			EnterKeyPressDoneSwitch=TRUE;
//			if(pGame->pOnlineWorld)
//				pGame->pOnlineWorld->bReturnKeyDown = TRUE;
//			
//			// ����Ű�� �ԷµǾ����� 
//			if(wParam=='\n' || wParam=='\r')
//			{				
//				if(GameStatus==GS_GAME)
//				{
//					if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
//					{
//						// native ��� ���
//						ChangeLanguage(TRUE);
//						
//						pGame->pBattle->m_EditControl.SetFocus();
//						pGame->pBattle->m_EditControl.SetString("");
//						
//						// �������� ������ �޽��������� �����ȴ�. 
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
			// ����� ����϶��� F10��ư�� ������ /InfoShow��ɾ ��۷� �����ȴ�.
		case 0xc0:
			pGame->pOnlineWorld->m_InfoFlag = !pGame->pOnlineWorld->m_InfoFlag;
			break;
			
#endif//_DEBUG
		case VK_RETURN:
			{				
				// ��ũ��Ʈ�� ���������� ���� ������...
				EnterKeyPressDoneSwitch=TRUE;
				if(pGame->pOnlineWorld)
					pGame->pOnlineWorld->bReturnKeyDown = TRUE;
				
				// ����Ű�� �ԷµǾ����� 
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
//							// �������� ������ �޽��������� �����ȴ�. 
//							if(GetAsyncKeyState(VK_CONTROL)<-1)
//							{
//								MessageMode=MESSAGEMODE_ALLY;
//							}
//							else
//							{
//								MessageMode=MESSAGEMODE_ALL;
//							}
//
//							// native ��� ���
//							ChangeLanguage(TRUE);
//						}
						// ===============================================================================
						// ===============================================================================
						// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
						// ������ ���¿��� ��Ŀ���� ���� ��� �Է��� �޴� ���� �����Ѵ�.
						if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
						{
							OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( pGame->pBattle->m_hOecChat );
							pOecEx->Clear();
							Handler_OnlineEditControlEx::SetFocus( pGame->pBattle->m_hOecChat );
							
							// �������� ������ �޽��������� �����ȴ�. 
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
				DWriteStringToFile("������ ��ư�� ��������");
				// �⺻ ���콺�� �����Ѵ�.
				SetMouse(MOUSE_NORMAL);

				// ���� �޴��� ���� �ֵ��� �Ѵ�. 
			    GameMenuShowSwitch = TRUE;
				GD.m_bBlackCheck = FALSE;
			}
			return 0;
			break;

		case VK_SPACE:
			if(GameStatus==GS_GAME)
			{
				// ���� �����⿡���� ����� ���������� ȭ���� �̵��Ѵ�.
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
				// ���ڸ� �Է����� ���� �ƴҶ� & ��ũ��Ʈ ���������� ������.  
				// ===============================================================================
//				else if(pGame->pBattle->m_EditControl.IsFocus() ==FALSE)
				// ===============================================================================
				// ===============================================================================
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
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
			// ���� �������� ����ڵ��� ������ �����ش�. 
			GD.PlayerMessageDelay=MAX_MESSAGE_DELAY;
			return 0;
			break;

         // ȭ�� ĸ���� 
		case VK_F12:
			if(abs(CurrentClock  - CaptureClock ) > 1500)
			{
				CaptureClock = CurrentClock;

				// ȭ���� ĸ���Ѵ�. 
				Capture.ScreenCapture();
			}

			return 0;
			break;

		default:
			oldkeystatus=GD.KB.GetAKey(wParam);
		    GD.KB.SetAKey(wParam, TRUE);
			break;
		}

		// Ű���� �Է��� ����Ѵ�.
		// ������ Ű ���°� FALSE�϶����� (������ �ִ� ���´� �ѹ��� �Է¸� �޴´�.)
		if(oldkeystatus==FALSE)
		   GD.KB.PushKeyBuffer(-1, -1, IpD.Mouse_X, IpD.Mouse_Y, wParam, FALSE);

		return 0;

///////////////////////////////////////////////////////////

	case WM_CREATE:
		return 0;
    case WM_ACTIVATEAPP:
        //bActive = (BOOL)wParam && GetForegroundWindow() == hWnd && !IsIconic(hWnd);
		bActive = (BOOL)wParam;// && GetForegroundWindow() == hWnd;
		// �ʱ�ȭ ���� ��쿡�� ������ ��Ƽ�� �ǵ���
		if( g_bMainInit ) bActive = TRUE;	

		if(bActive==TRUE)
		{
			pGame->bPause=FALSE;
			
			if(FirstSwitch==FALSE)
			{
			   Restore();

			   // ��Ŀ���� ���� �����ϴ��ĸ� �����Ѵ�. 
			   if(pGame->pBattle)
			   {

					// ===============================================================================
//					pGame->pBattle->m_EditControl.SetString("");
					// ===============================================================================
					// ===============================================================================
					// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
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

		// ȭ�� ���� �ڵ��� �����Ѵ�. 
		CloseDupGameHandle();

		// ��ŷ�� �����Ѵ�.
	//	removehook();

		PostQuitMessage(0);
		return 0;
	}

	// Ư�� �޼��� ���̵��� ������� ������.
//	message = pGame->OriginallyWndMsg(message);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//--------------------------------------------------------------------------
// Name: InitQuit()
// Desc: ���Ḧ ���� �غ� �Ѵ�. 
//--------------------------------------------------------------------------
BOOL InitQuit()
{
	return TRUE;
}


//--------------------------------------------------------------------------
// Name: Restore()
// Desc: �̴ϸ������ ���¿��� �ٽ� �����Ҷ� ���Ǵ� �Լ� 
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
	// actdoll (2004/10/01 10:56) : ���ӿ��� ������ �޽��� ����� ���� ��ƾ
	static BOOL		bLastThrough = false;
	static SHORT	shPrevGameStatus	= GS_GAME;

	if(pGame->pOnlineWorld)
	{
		UI16 uiResult = pGame->pOnlineWorld->pOnlineClient->DataProc();
		pGame->pOnlineWorld->pOnlineClientChatting->DataProc();
		if(uiResult)    pGame->pOnlineWorld->ReceiveMessageProc(uiResult);

		// ���� �޼����� �̴ٸ�... 
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
	//	�� �κ��� �������� ���� ���� ���°� �����Ǿ��� ��� �ѹ� �� ����ֱ� ���� ��ƾ�̴�.
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

	
	// �̴ϸ������ ���¿����� ����༭�� �ȵȴ�....
	if(bActive == FALSE)return ;

	// ���� �ʹ��� ����� �׷����� �ʴ´�. 
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
	// ��Ƽ �׽�Ʈ�� ���� ȭ�� ��ο��� �� �������� �ǳʶ��� �ʴ´�.
	else if(diff >= (UI32)(max(50, pGame->Config.GetRealGameSpeedClock())))
	{
		draw1switch = FALSE;
		draw2switch = FALSE;
		draw3switch = FALSE;
		draw4switch = FALSE;
		LastDrawClock = CurrentClock;
	}

	// actdoll (2004/10/01 11:00) : 
	//	���� �ѹ����� ��¯ ������ ���
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
		// ������2�� ���
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

		// �ΰ� �������� �����ش�.
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
	// ���ӿ� ���� ���� �ʱ�ȭ�Ѵ�. 

	// actdoll (2004/11/05 16:16) : IME�κ�
	//	���� ��Ŀ���� ������ ����Ʈ �ڽ��� ������ ��� ���� Ŭ�����Ų��.
	//	�̷��� �ϸ� IME ���õ� ��� ���ҽ��� �����ȴ�.
	Handler_OnlineEditControlEx::ClearCurrentFocus();


	if(MapEditMode == TRUE)
	{
		// ���� Ŭ������ �����Ѵ�.
		pGame->InitBattle();
		
		// ��Ÿ ������ �ʱ�ȭ�Ѵ�. 
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
		// ��Ÿ ������ �ʱ�ȭ�Ѵ�. 
		pGame->InitEtc();

		// ���� Ŭ������ �����Ѵ�.
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
			
			// ���� �޴��� ���ش�. 
			GameMenuShowSwitch = FALSE;
			
			switch(status)
			{
			case GS_GAME:
				GameStatus=status;
				break;
				
				// ������ �ٽ��Ѵ�. 
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



// ���Ӹ޴����� �����ϱ⸦ ������ �� ó�� �Լ�. 
BOOL OperateGameMenuInitQuit()
{
	GameStatusBuffer = GS_INITQUIT;
	GameStatus       = GS_INITFADEOUT;

	return TRUE;			
}

// ���Ӹ޴����� �ʱ�޴��� ������ �� ó�� �Լ�. 
BOOL OperateGameMenuInitInitMenu()
{
	GameStatusBuffer = GS_ONLINEWORLD;
	GameStatus=GS_INITFADEOUT;
	// ������ �ʱ�ȭ�Ѵ�. 
	EndGame();
	
	return TRUE;
}

// Ŀ���� ���������� ���ƾ� ���� �����Ѵ�.
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
