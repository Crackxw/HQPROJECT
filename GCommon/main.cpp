//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	임진록 온라인 [거상] - 메인문
//	File Name		:	Main.cpp
//	Birth Date		:	2004. 01. 01.
//	Creator			:	거 상 팀
//	Editer			:	김태곤, 양정모, 이윤석, 백상현, 이준석, 정진욱, 
//						홍성훈, 조현준, 이민수, 임준혁, 박상원, 김성준,
//						김유민, 권혁훈, 최병주, 윤영대
//	Copyright		:	Joyon Entertainments. Co. 2000. 2005.
//	Comments		: 
//		거상의 메인문입니다.
//
//===================================================================================================

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
/********************************************************************
*
*  1998.8.22~ 2000.1.6
*  임진록2 
*
*********************************************************************/
// Initialize the COM interface identifiers
//#define WIN32_LEAN_AND_MEAN
//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: 기본 헤더 선언
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <clGame.h>
#include <findpath5.h>
#include "..\Gersang\maindata.h"
#include <item.h>
#include "..\Gersang\screenpoint.h"
#include <chat.h>
#include "..\Gersang\cheat.h"
#include "..\Gersang\mapedit.h"
#include "..\Gersang\help.h"
#include "..\Gersang\text.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <Capture.h>
#include "../SCCommon/ReadIP.h"
#include "../SCCommon/OnlineCommonParser.h"



#include <map.h>
#include <ysmap.h>
#include <mapinterface.h>
#include <charinterface.h>
#include <mynet-Game.h>
#include "bindJXFile.h"

#include "..\\ImjinOnline\\OnlineVersion.h"

// actdoll (2004/10/19 11:45) : 게임가드 함수 관련
#include "GGuard.h"

//-----------------------------------------------------------------------------
// actdoll (2004/03/11 13:58) : 크리티컬 버그를 잡기위한 디버그용 파일 생성 클래스
//-----------------------------------------------------------------------------
#include "..\\SCCommon\\JODebug.h"
#include <GSMemory.h>

#include <resource.h>

// 지도 편집기 모드. 
//#define _MAPEDIT	1

//--------------------------------------------------------------------------
// 초기화 에러시 집어넣을 스트링
//--------------------------------------------------------------------------
const	char*	DF_ERROR_MEMORY_NOT_ENOUGH			= "메모리가 충분하지 않습니다!\n\n Memory is not enough! Program will be shutdown.";
const	char*	DF_ERROR_CREATE_STANDARD_NTN		= "Error Code [80000001]\nCannot check International Information Setting(IIS)!\n\n Program will run by basic informations of OS.\n We recommand to reinstall when you catch some problems.";
const	char*	DF_ERROR_CANNOT_FIND_HQ				= "Error Code [80000002]\nCRITICAL ERROR!\n\n  CANNOT FIND CORE DATA!\n  PLEASE REINSTALL.";
const	char*	DF_ERROR_WARNING_FOR_CRACK			= "Error Code [80000003]\nCRITICAL ERROR!\n\n  CORE DATA AUTHORIZATION FAILED!\n  PLEASE REINSTALL.";
const	char*	DF_ERROR_DXINIT_IS_FAILED			= "시스템 초기화 중 오류가 발생했습니다!\n\n System Initialize is Failed!";
const	char*	DF_ERROR_CANNOT_EXEC_LAUNCHER		= "이 실행파일은 바로 작동시킬 수 없습니다!\n%s 폴더에 있는 run.exe를 실행해 주십시오.\n만약 run.exe를 실행했는데 이 에러가 뜬다면 런쳐가 업데이트된 경우이므로 다시 한번 시도해 주십시오.\n\n You cannot run this file directly!\n Please execute the \"run.exe\" file in %s folder.\n Also you can see this when launcher was updated. Then, please try again.";
const	char*	DF_ERROR_UPDATE_VERSION_SERVER		= "변경된 사항을 적용하기 위해 게임을 다시 한번 실행해 주십시오.\n\n Please try again to run the game for applying new updates.";
const	char*	DF_ERROR_CANNOT_FIND_ISI			= "국가정보리스트 파일을 찾을 수 없거나 문제가 있습니다! 프로그램을 다시 설치해 주세요.\n\n International Information List(IIL) is incorrect! Please reinstall.";
const	char*	DF_ERROR_CANNOT_FIND_NATION_LIST	= "설정된 국가정보와 일치하는 데이터가 없습니다. 프로그램을 다시 설치해 주세요.\n\n International Information List(IIL) is Mismatched with International Information Setting(IIS)!\n Please reinstall.";

//--------------------------------------------------------------------------
// 메인화면 라이센스 텍스트
//--------------------------------------------------------------------------
const	char*	DF_TEXT_COPYRIGHT_WITH_GAMANIA	= "Copyright 2001-2005. Gamania & Joyon Entertainment. All rights reserved.";		// 한국/일본/대만/중국/홍콩용
const	char*	DF_TEXT_COPYRIGHT_WITH_BOLEHCOM	= "Copyright 2004-2005. BOLEH.COM & Joyon Entertainment. All rights reserved.";		// 인도네시아용

//--------------------------------------------------------------------------
// 각 국가별 인식코드 - 각 국가별 전화번호 코드 - 서비스 시작된 순서대로 작성
//--------------------------------------------------------------------------
const	int		DF_CODE_NATION_IDENTITY_KOREA		= 82;		// 한국
const	int		DF_CODE_NATION_IDENTITY_TAIWAN		= 886;		// 대만
const	int		DF_CODE_NATION_IDENTITY_JAPAN		= 81;		// 일본
const	int		DF_CODE_NATION_IDENTITY_HONGKONG	= 852;		// 홍콩
const	int		DF_CODE_NATION_IDENTITY_CHINA		= 86;		// 중국
const	int		DF_CODE_NATION_IDENTITY_INDONESIA	= 62;		// 인도네시아

//--------------------------------------------------------------------------
// 각 언어별 코드페이지
//--------------------------------------------------------------------------
const	int		DF_CODE_CODEPAGE_ENGLISH			= CP_ACP;			// 영문(기본)
const	int		DF_CODE_CODEPAGE_KOREAN				= 949;				// 한국어 
const	int		DF_CODE_CODEPAGE_JAPANESE			= 932;				// 일본어
const	int		DF_CODE_CODEPAGE_CHINESE_SIMP		= 936;				// 중국어(중국, 싱가포르)
const	int		DF_CODE_CODEPAGE_CHINESE_TRAD		= 950;				// 중국어(대만, 홍콩)

//--------------------------------------------------------------------------
// 각 국가별 HQ 폴더명
//--------------------------------------------------------------------------
char*	const	DF_TEXT_SERVICE_NATION_LIST[ELANGAGECODE_MAX]	= { "None", "Korea", "Taiwan", "Japan", "Hongkong", "China", "Indonesia" };

//--------------------------------------------------------------------------
// 디버그용 경로
//--------------------------------------------------------------------------
char			DF_PATH_DEBUG_OUTSIDE_HQTEXT[MAX_PATH]	= {0,};	// (디버그용) 외부 HQ Text 참조경로

//------------------------------------
// 그래픽에 관련된 Class 선언 
//------------------------------------
extern cltGrp                clGrp;               // 그래픽에 관련된 Class 선언 

//--------------------------------------------------------------------------
// 전역 변수들 
//--------------------------------------------------------------------------
_clGame *pGame;


XSPR TileSpr[MAX_MAP_TILE_SPR_NUMBER];						// 지도 폰트용 타일 
XSPR TitleSpr;												// 타이틀 배경 그림에 사용될 포맷  
XSPR PannelSpr;

//------------------------------------
// 네트워크 선언  
//------------------------------------

OnlineBattleInfo OBInfo;

int     MultiPlayDelayTime		= DEFAULT_MULTI_PLAY_DELAY_TIME;      // MultiPlay Delay기본 시간 
DWORD	PrevMultiClock;
DWORD	MessageSendFrame		= 0;  // 명령이나 NO_MESSAGE를 보낸 프레임 
BOOL	GameDropSwitch			= FALSE;
_MyNetGame MyNet;



//------------------------------------
// 화면관련 
//------------------------------------
// actdoll (2005/01/10 23:08) : 윈도 모드 변경 여부
//	이제 플래그 파일을 이용하여 윈도 모드를 변경한다.
//#ifdef _WINDOWED
//SI32  WindowModeSwitch = TRUE;
//#else
//SI32  WindowModeSwitch = FALSE;
//#endif
extern int WindowModeSwitch;
SI32				WindowModeSwitch	= FALSE;

_ScreenPoint        SP;									// 화면에 사용될 여러 정보 
int					bActive;							// 현재 액티브인지 아닌지 알려주는 변수  
BOOL				g_bMainInit;						// 현재 초기화 중인 경우 무조건 액티브가 되도록
_clCapture			Capture;							// 화면 캡춰 관련 클래스 

UI32  GameScreenMode = MODE_GRAPHICS800_600;

DWORD LastDrawClock;

//------------------------------------
// 게임 관련 변수들 
//------------------------------------
SI32	UserPlayerNumber;
SHORT	GameLeftPressSwitch = FALSE;
SHORT	GameRightPressSwitch= FALSE;
SHORT	GameMouseX=10;
SHORT	GameMouseY=10;
SHORT	GameRealX;
SHORT	GameRealY;
DWORD	GameWParam;

DWORD CaptureClock ;      // 게임을 캡춰한 시간. 

// 전기 공격 관련 
SHORT PointNumber;
SHORT PointArray[200];

//------------------------------------
// 게임 내부 정보를 위한 변수들 
//------------------------------------
_Item    Item[MAX_ITEM_NUMBER];
_Order   Order[MAX_ORDER_NUMBER];
_MapEdit MapEdit;

_Help					Help;
_Chat					Chat;                           // 입력관련
_Cheat					Cheat;
_FindPathGame		   *FindPathGame;

//------------------------------------
// 게임 시작모드 
//------------------------------------
#ifdef _MAPEDIT
SHORT					MapEditMode          = TRUE;				// 지도 편집 모드(TRUE : 지도 편집기, FALSE : 게임)
#else
SHORT					MapEditMode          = FALSE;				// 지도 편집 모드(TRUE : 지도 편집기, FALSE : 게임)
#endif

//------------------------------------
// 게임 내부 변수 
//------------------------------------
#ifdef _MAPEDIT
SHORT					GameStatus				= GS_INITONLINEBATTLEGAME; // 현재의 게임 상황을 알리는 변수 
#else
SHORT					GameStatus				= GS_INITONLINEWORLD; // 현재의 게임 상황을 알리는 변수 
#endif

SHORT					GameStatusBuffer;
SHORT					GamePauseSwitch			= FALSE;          // 게임이 잠시 중단되었는가?
SHORT					GameMenuShowSwitch		= FALSE;          // 게임중에 메뉴를 보여주는지 여부를 알려준다. 
SHORT					MessageMode				= MESSAGEMODE_ALL;// 메시지가 누구에게 가는가?


// 게임에서 사용되는 변수 
RECT					ScreenRect;									// 지도가 표시되는 영역 
_Text					Text;										// 텍스트 클래스 선언 
_FileVary				FV;											// 사용되는 파일의 변수를 저장한다. 
_KindInfo				KI[KIND_ON_MAX];					// 캐릭터 종류별 데이터 
CCharFileLoadManager	g_FileLoadManager;							// 캐릭터 데이터
CItem					*g_pItem = NULL;							// 아이템 클래스


_NewID					ZeroID;


//------------------------------------
// 경로 저장변수 
//------------------------------------
char					SavePath[256];								// 현재 게임이 설치된 경로 
char					CurrentDirectory[256];

//------------------------------------
// 메뉴 관련 
//------------------------------------
int   MenuMode;											// 메뉴의 상태를 저장 
SHORT MenuOfMouse;										// 마우스 커서가 위치하고 있는 메뉴의 번호 
SHORT CrewMenuOfMouse;

BOOL IsMosueInControlSwitch;							// 마우스가 게임 콘트롤위에 위치하고 있다.
BOOL bGameAllyWindowSwitch;


//------------------------------------
// 사용자 관련 키보드와 마우스 
//------------------------------------
int    MouseMode;										// 마우스가 처한 상태를 저장 
_NewID UnderMouseID;



int ControlKeyPressSwitch  = FALSE;
int TabPressDoneSwitch     = FALSE;
int ShiftKeyPressSwitch    = FALSE;
int DownKeyPressDoneSwitch = FALSE;
int UpKeyPressDoneSwitch   = FALSE;
int EnterKeyPressDoneSwitch= FALSE;
int DragSwitch             = FALSE;
int DragDoneSwitch         = FALSE;
int DragStartX, DragStartY, DragEndX, DragEndY;

//------------------------------------
// 사용자 관련 맵을 위한 변수들 
//------------------------------------
int   MapDragSwitch        = FALSE;
int   MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
SHORT MapX, MapY;                    // 마우스가 위치한 small 좌표 
SHORT MouseHarvestX, MouseHarvestY;
SHORT UnderMouseCropType;
SHORT UnderMouseCropMount;

//------------------------------------
// 게임에 사용되는 시계변수 
//------------------------------------
clock_t StartClock, CurrentClock, PrevClock;
clock_t ClickClock;

clock_t MonitorMacroClock1, MonitorMacroClock2;		// 매크로 검사 Process 실행 주기

struct tm       *pNewTime;

//------------------------------------
// 게임에 사용되는 Block변수와 기타변수
//------------------------------------
SHORT BasicBlock=BLOCK_CHAR|BLOCK_DEEPWATER|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_RUIN|BLOCK_ICEWALL;
SHORT WaterBlock=BLOCK_CHAR|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_LAND|BLOCK_SHALLOWWATER|BLOCK_ICEWALL;


int   FlyVibrateData[MAX_FLY_VIBRATE]={0, 1, 2, 3, 4, 3, 2, 1};

//=====================================================================
// 저장하기 관련 정보 
//=====================================================================

// 지도의 각 좌표마다의 중심값, 
int MapCenterX[MAX_MAP_XSIZE][MAX_MAP_YSIZE];  
int MapCenterY[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
// 절대 좌표를 가지고 기본 좌표를 구한다. 
unsigned char MapByRealX[(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];
unsigned char MapByRealY[(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];

SI32 MaxMapXsize = 180;
SI32 MaxMapYsize = 180;

_Map    Map;                       // 지도용 데이터  
_YSMap  SMap;                      // 작은 지도용 데이터   
_Char   CR[MAX_CHAR_NUMBER];       // 캐릭터의 포인터 
cltCharInterface clifChar;         // 캐릭터 관련 인터페이스 
cltMapInterface clifMap;           // 맵     관련 인터페이스 

_MainData GD;                      // 게임의 주데이터 


//HQFileManager	HQFile;
char			LogInID[128]		= {0,};

char			g_AutoID[256];
char			g_AutoIDPW[256];
SI16			g_AutoServerCode = -1;


//----------------------------------------------------------------------------
// actdoll (2004/10/05 9:10) : 클라이언트 시동 및 작동 플래그
//----------------------------------------------------------------------------
BOOL			g_fTestVersion			= FALSE;		// 테섭용인가 아닌가?
BOOL			g_bSkipIntro			= FALSE;		// 인트로 스킵여부?

// 커맨드 라인 옵션
const	DWORD	DF_FLAG_CMDLINE_ACCESS	= 0x10000000;	// 커맨드 라인 명령 : 가동 (이 값이 없을 경우 클라이언트는 가동되지 않는다.)
const	DWORD	DF_FLAG_CMDLINE_IP		= 0x00000001;	// 커맨드 라인 명령 : 프론트 서버 IP 설정 (수치있음)
const	DWORD	DF_FLAG_CMDLINE_NT		= 0x00000002;	// 커맨드 라인 명령 : 국가 정보값 설정 (수치있음)
const	DWORD	DF_FLAG_CMDLINE_ST		= 0x00000004;	// 커맨드 라인 명령 : 서비스 타입값 설정 (수치있음)
const	DWORD	DF_FLAG_CMDLINE_RA		= 0x00000008;	// 커맨드 라인 명령 : 클라이언트를 종료하고 런처를 다시한번 실행시켜라 (수치없음)

const	DWORD	DF_FLAG_CMDLINE_SKIPGG	= 0x00001000;	// 커맨드 라인 명령 : 게임가드 스킵
//----------------------------------------------------------------------------


char			REG_PATH[256] = "";

//=====================================================================



//--------------------------------------------------------------------------
// Main.cpp 의 Methods
//--------------------------------------------------------------------------
extern HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

// actdoll (2004/01/16 0:18) : HQ 체크섬 장치 및 GCS 설정
const	unsigned __int64	DF_VALUE_CS		= 0x2F360C5A2964A4E7;	// 체크섬 비교값			(Debug시 뜨는 HQ파일 검산 오류 값을 여기다 적용시키면 됩니다.)
const	UI32				DF_VALUE_CSF	= 0x1c437f64;			// 체크섬 플래그			(이 값은 플래그입니다. 이 값이 바뀌어버리면 DF_VALUE_CS값 자체가 바뀝니다.)
const	DWORD				DF_VALUE_ATI	= 0x80d36cfa;			// 운영자 옵션 플래그의 인증코드(운영자 툴 베이스에 이 값이 적용됨)
const	char*				DF_VALUE_AAC	= "ilovegs";			// 운영자가 입력하는 인증코드 플래그값

BOOL	Init_CheckUpdates	( HINSTANCE hInstance );
DWORD	Init_CheckAdOpt		( LPSTR lpCmdLine, int nCmdShow );
DWORD	Init_CheckCmdLines	( LPSTR lpCmdLine, int nCmdShow, DWORD dwOrder );
BOOL	Init_CheckBasics	( HINSTANCE hInstance, DWORD dwOrder );
BOOL	Init_Engines		( HINSTANCE hInstance, int nCmdShow, DWORD dwOrder );
void	Kill				( void );

unsigned __int64	FCS					( char* pFileName, BOOL bAdminOption = false );
BOOL				DataWork			( HINSTANCE hInstance );
BOOL				GetNTN				( char *pszFileName, int &iNation, int &iType );
BOOL				CreateNTN			( char *pszFileName, int &iNation, int &iType );
BOOL				SetLocaleInfo		( int iServiceNation, int iServiceType, char *pszCustomFrontServerIP = NULL );
BOOL				ReadIPEx			( char* lpFileName, BOOL bUseHQ, BOOL bJustReadFirstLine, int iNation, int iType, SConnectServerInfo* lpSConnectServerInfo );
void				ShowDlg				( HINSTANCE hInstance, char *pszTitle, char *pszText );
BOOL	CALLBACK	DlgBoxProc			( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
UINT				GetAdDecoData		( BYTE* pSrc, UINT uiSrcBuffSize, DWORD& dwNTNValue, BOOL& bSkipGameGuard, BOOL& bSkipLauncher, BOOL& bSkipIntro, BOOL& bRunAsWindowMode, 
						LPSTR pszAACCode, LPSTR pszVersionServerIP, LPSTR pszFrontServerIP, LPSTR pszPathCustomHQ );
void				ConvertSpecialChar	(CHAR *pSpecialString);

char	g_pszDlgTitle[1024] = {0,};
char	g_pszDlgText[1024] = {0,};

//-----------------------------------------------------------------------------
// Name: WinMain()
// Code: All (2004-01-01)
// Desc: 윈도우 메인문. 모든 프로그램은 여기서 시작된다!
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// actdoll (2004/03/12 2:45) : 
	//	거상 클라이언트가 어떤 목적으로 쓰이는 놈인지 체크
	DWORD	dwRetOrder;

	GSMemoryInit(GS_DEBUG_WINDOWS);
	GSMemorySetLevel(2);
	XDebugInit(XDOT_DEBUG);
	
	// 업데이트 내용 확인 및 변경 >> 버젼 갱신, 런쳐 갱신, GCS 가동, 디버그 정보 파악 등
	if( !Init_CheckUpdates( hInstance ) )		return 0;

	// 커맨드 라인 확인
	dwRetOrder	= Init_CheckAdOpt( lpCmdLine, nCmdShow );
	dwRetOrder	= Init_CheckCmdLines( lpCmdLine, nCmdShow, dwRetOrder );

	
	// 클라이언트 기본 정보 확인 >> HQ로드, 사내/테섭/정섭/국가코드 판별, 런쳐에 의한 가동 옵션 판별
	if( !Init_CheckBasics( hInstance, dwRetOrder ) )	
	{	
		BindJXFile::GetInstance()->destroy();
		return 0;
	}

	// 엔진 가동 >> _clGame 가동 및 초기화
	if( !Init_Engines( hInstance, nCmdShow, dwRetOrder ) )
	{
		BindJXFile::GetInstance()->destroy();
		return 0;
	}

	// ====================		!!!메인 루프!!!		==================== //
	MSG			msg;
	try
	{
		pGame->Poll(&msg);
	}
	catch (const exception& e)
	{
		MessageBox( NULL, e.what(), "SYSTEM ERROR", MB_OK | MB_TOPMOST );
		if( pGame )
		{
			if( pGame->pOnlineWorld )	delete pGame->pOnlineWorld;
			pGame->finiObjects();			
			delete pGame;
			pGame = NULL;
		}
	}
	// ====================		!!!메인 루프!!!		==================== //

	// 리소스 해제
	Kill();
	
	GSShowMemoryStatus();
//	showMemStats();

	return msg.wParam;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckUpdates()
// Code: actdoll (2004-03-12)
// Desc: 프로그램 시작 전 새로이 업데이트 해야 할 정보들을 확인한다.
//-----------------------------------------------------------------------------
BOOL	Init_CheckUpdates( HINSTANCE hInstance )
{
	// 서비스 기본정보 초기화
	ZeroMemory( &g_stConnectServerInfo, sizeof(g_stConnectServerInfo) );

	//----------------------------------------------------------------------------
	//	1단계 - 버젼 갱신
	//----------------------------------------------------------------------------
	FILE	*fp;
	fp = fopen( "Online\\vsn.dat", "wb" );
	if( fp )
	{
		DWORD	dwVersion	= ON_CLINETVERSION;
		dwVersion ^= 0xffffffff;
		fwrite( &dwVersion, sizeof(DWORD), 1, fp );
		fclose( fp );
	}


	//----------------------------------------------------------------------------
	//	2단계 - 런쳐 갱신
	//----------------------------------------------------------------------------
	char old[] = "run.exe", old1[] = "run2.exe";   
	fp = fopen(old1, "rb");
	if(fp)
	{
		fclose(fp);
		CopyFile(old1, old, FALSE ); 
		DeleteFile(old1);
	}


	//----------------------------------------------------------------------------
	//	3단계 - HQ 데이터 체크 및 로드
	//----------------------------------------------------------------------------
	if( !DataWork( hInstance ) )
	{
		MessageBox( NULL, DF_ERROR_WARNING_FOR_CRACK, "Gersang", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	4단계 - 국가 정보, 서비스 타입을 확인
	//----------------------------------------------------------------------------
	// 국적 정보를 확인한다.
	int		iNation = 0, iType = 0;
	if( !GetNTN( "Online\\ntn.con", iNation, iType ) )
	{
		// 국가 정보 파일에 문제가 생겼다면 새로운 정보 파일을 만들고 메시지 박스 띄운다.
		CreateNTN( "Online\\ntn.con", iNation, iType );
		ShowErrorMsgBox( 10001001 );
//		MessageBox( NULL, DF_ERROR_CREATE_STANDARD_NTN, "ERROR(64dfacc4)", MB_OK );
	}


	//----------------------------------------------------------------------------
	//	5단계 - 기본 로케일 정보 체크
	//----------------------------------------------------------------------------
	// 문제가 있는지 없는지를 확인한다
	if( !SetLocaleInfo( iNation, iType ) )
	{
		BindJXFile::GetInstance()->destroy();
		ShowErrorMsgBox( 10001002 );
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_NATION_LIST, "039addeb", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	6단계 - GCS 가동(이건 GCS가 포함된 한국 테섭만 된다)
	//----------------------------------------------------------------------------
	SYSTEMTIME		sysTime;
	char			pszCrashText[1024], pszCrashTime[1024];
	GetSystemTime( &sysTime );
	sprintf( pszCrashText, "!!CriticalCrash!!_V%d", ON_CLINETVERSION );
	sprintf( pszCrashTime, "StartTime- %d/%d/%d %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	InstallCrashHandler( pszCrashText, pszCrashTime, "GERSANG", CRASH_CLIENT );				// 크래쉬 핸들러 설치
	SetNationCode( DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );	// 크래쉬 국가 코드 설치


	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckAdOpt()
// Code: actdoll (2004-03-12)
// Desc: 어드민 옵션을 체크한다.
//		return		- 가동 시작이라면 true, 아니라면 false
//-----------------------------------------------------------------------------
DWORD	Init_CheckAdOpt( LPSTR lpCmdLine, int nCmdShow )
{
	// 뮤텍스를 생성하여 어드민 툴이 가동중인지 아닌지 확인
	HANDLE	hMutex	= CreateMutex( NULL, TRUE, "gs041216cat" );
	DWORD	dwRet	= GetLastError();
	ReleaseMutex( hMutex );
	CloseHandle( hMutex );

	// 이 상황이라면 정상적인 작동 상태가 아니다.
	if( !hMutex || dwRet != ERROR_ALREADY_EXISTS )		return 0;

	// 일단 명령줄을 통한 데이터 걸러내기 개시
	BOOL	bFound = false;
	char	*pRealData, *pszToken, pszData[2048] = {0,}, pszAAC[128] = {0,};

	strcpy( pszData, lpCmdLine );
	pszToken = strtok( pszData, "- \t\n\r" );
	while( pszToken )
	{
		// 운영자 플래그 데이터를 찾아낸다.
		if( ( pRealData = strstr( pszToken, "AO:" ) ) != NULL )
		{
			bFound	= true;
			pRealData	+= 3;	// 실제 데이터 초반으로 이동
			break;
		}
		pszToken	= strtok( NULL, "- \t\n\r" );
	}

	// 플래그로 찾지 못했다면 파일로 찾아본다.
	if( !bFound )
	{
		char	pszPath[MAX_PATH];
		GetWindowsDirectory( pszPath, MAX_PATH );
		strcat( pszPath, "\\Ffgsdeac.cks" );
		
		// 파일 자체가 없다면 에러다
		FILE	*fp = fopen( pszPath, "rb" );
		if( !fp )	return 0;

		// 일단 크기 받고
		fseek( fp, 0, SEEK_END );
		DWORD	dwLen = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		// 파일을 버퍼에 적재
		fread( pszData, dwLen, sizeof(BYTE), fp );
		pRealData	= pszData;

		fclose( fp );
	}

struct ST_ADFLAG
{
	UINT	m_uiNTN;				// 운영자 지정 NTN
	
	BOOL	m_bSkipLauncher;		// 런쳐 스킵 여부
	BOOL	m_bSkipIntroScreen;		// 인트로 스킵 여부
	BOOL	m_bSkipGameGuard;		// 게임가드 스킵 여부
	BOOL	m_bRunAsWindowMode;		// 윈도모드로 가동 여부

	char	m_szVersionServerIP[64];		// 버전서버 IP 스트링이 들어있는 포인터
	char	m_szFrontServerIP[64];			// 프론트서버 IP 스트링이 들어있는 포인터
	char	m_szPathCustomHQ[MAX_PATH];		// 프론트서버 IP 스트링이 들어있는 포인터
}	stAdmin = {0,};

	// 검산 들어간다.
	UINT	uiRet = GetAdDecoData( (BYTE*)pRealData, strlen( pRealData ), (DWORD&)stAdmin.m_uiNTN, 
									stAdmin.m_bSkipGameGuard, stAdmin.m_bSkipLauncher, stAdmin.m_bSkipIntroScreen, stAdmin.m_bRunAsWindowMode,
									pszAAC, stAdmin.m_szVersionServerIP, stAdmin.m_szFrontServerIP, stAdmin.m_szPathCustomHQ );
	if( stricmp( pszAAC, DF_VALUE_AAC ) != 0 || uiRet == 0 )		
	{
		ZeroMemory( &stAdmin, sizeof(stAdmin) );
		return 0;
	}

	// 정상이다. 세팅 확인
	dwRet	= 0;
	if( strlen(stAdmin.m_szFrontServerIP) > 1 )	{ dwRet |= DF_FLAG_CMDLINE_IP;	strcpy( g_stConnectServerInfo.m_FrontServerIP, stAdmin.m_szFrontServerIP ); }	// 프론트서버 IP변조
	if( strlen(stAdmin.m_szPathCustomHQ) > 1 )	{ strcpy( DF_PATH_DEBUG_OUTSIDE_HQTEXT, stAdmin.m_szPathCustomHQ ); }											// 임의 HQ 경로
	if( stAdmin.m_uiNTN != 0 )					{ dwRet	|= DF_FLAG_CMDLINE_NT | DF_FLAG_CMDLINE_ST;	g_stConnectServerInfo.m_iServiceNation = stAdmin.m_uiNTN; g_stConnectServerInfo.m_iServiceType = 1; }	// NTN 세팅
	if( stAdmin.m_bSkipGameGuard )				{ dwRet	|= DF_FLAG_CMDLINE_SKIPGG; }	// 게임가드 스킵여부
	if( stAdmin.m_bSkipLauncher )				{ dwRet	|= DF_FLAG_CMDLINE_ACCESS; }	// 런쳐 스킵 여부
	g_bSkipIntro		= stAdmin.m_bSkipIntroScreen;									// 인트로 스킵 여부
	WindowModeSwitch	= stAdmin.m_bRunAsWindowMode;									// 윈도모드 가동여부

	// 확인사살
	ZeroMemory( &stAdmin, sizeof(stAdmin) );
	MessageBox( NULL, "Administrator Option was initialized!", "Gersang", MB_OK|MB_ICONINFORMATION );
	// actdoll (2005/01/05 18:26) : 예외적으로 여기서 운영자 확인이 끝났을 경우 크래쉬 정보를 갱신한다.
	SetAppType( CRASH_CLIENT_ADMIN );

	return dwRet;
}


//-----------------------------------------------------------------------------
// Name: Init_CheckCmdLines()
// Code: actdoll (2004-03-12)
// Desc: 커맨드 라인의 정보를 읽어온다.
//		return		- 무사히 끝냈다면 true, 아니라면 false
//-----------------------------------------------------------------------------
DWORD	Init_CheckCmdLines( LPSTR lpCmdLine, int nCmdShow, DWORD dwOrder )
{
	char	*pTemp, *pOrder, pszCmdLine[2048] = {0,};


	//----------------------------------------------------------------------------
	//	1단계 - 클라이언트 실행 가능 플래그 확인
	//----------------------------------------------------------------------------
	strcpy( pszCmdLine, lpCmdLine );
	pOrder = strtok( pszCmdLine, "- \t\n\r" );
	while( pOrder )
	{
		// 해외꺼 대비해서 이번만 가동시킨다.
		if( stricmp( pOrder, "LF" ) == 0 )
		{
			dwOrder |= DF_FLAG_CMDLINE_ACCESS;
			break;
		}
		// 런처에서 가동되었다.	(Launcher Start)
//			if( stricmp( pOrder, "LS" ) == 0 )
//			{
//				dwOrder |= DF_FLAG_CMDLINE_ACCESS;
//				break;
//			}
//			// 수동으로 가동되었다.	(Manual Start)
//			if( ( pTemp = strstr( pOrder, "MS:" ) ) != NULL )
//			{
//				pTemp += 3;
//				if( strlen( pTemp ) )	// 수치비교
//				{
//					if( stricmp( pTemp, DF_VALUE_AAC ) == 0 )
//					{
//						dwOrder |= DF_FLAG_CMDLINE_ACCESS;
//						break;
//					}
//				}
//			}

		pOrder = strtok( NULL, "- \t\n\r" );
	}


	// 만약 명령줄 가동 플래그가 없다면 일단 실패
	if( !(dwOrder & DF_FLAG_CMDLINE_ACCESS) )		return 0;


	//----------------------------------------------------------------------------
	//	2단계 - 기타 명령들 확인
	//----------------------------------------------------------------------------
	strcpy( pszCmdLine, lpCmdLine );
	pOrder = strtok( pszCmdLine, "- \t\n\r" );
	while( pOrder )
	{
		// 프론트 서버 IP.		(frontserver IP)
		if( !( dwOrder & DF_FLAG_CMDLINE_IP ) &&	( pTemp = strstr( pOrder, "IP:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// IP 주소 복사
			{
				strcpy( g_stConnectServerInfo.m_FrontServerIP, pTemp );
				dwOrder |= DF_FLAG_CMDLINE_IP;
			}
		}
		// 국가 정보.			(Nation Type)
		else if( !( dwOrder & DF_FLAG_CMDLINE_NT ) &&	( pTemp = strstr( pOrder, "NT:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// 국가정보 수치 복사
			{
				g_stConnectServerInfo.m_iServiceNation	= atoi( pTemp );
				dwOrder |= DF_FLAG_CMDLINE_NT;
			}
		}
		// 서비스 정보.			(Service Type)
		else if( !( dwOrder & DF_FLAG_CMDLINE_ST ) &&	( pTemp = strstr( pOrder, "ST:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// 국가정보 수치 복사
			{
				g_stConnectServerInfo.m_iServiceType	= atoi( pTemp );
				dwOrder |= DF_FLAG_CMDLINE_ST;
			}
		}
		// 재기동 요청.			(Retry Again)
		else if( !( dwOrder & DF_FLAG_CMDLINE_RA ) &&	stricmp( pOrder, "RA" ) == 0 )
		{
			dwOrder |= DF_FLAG_CMDLINE_RA;
		}

		pOrder = strtok( NULL, "- \t\n\r" );
	}

	return dwOrder;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckBasics()
// Code: actdoll (2004-03-12)
// Desc: 초기에 로딩되야 할 기본 정보들을 살펴본다.
//		return		- 무사히 끝냈다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	Init_CheckBasics( HINSTANCE hInstance, DWORD dwOrder )
{
	//----------------------------------------------------------------------------
	//	1단계 - 기본 로케일 정보 재검색(수정된 ServiceNation, Type, FrontServerIP를 적용)
	//----------------------------------------------------------------------------
	// 운영자 명령등에 의해 작업이 수정됐을 수 있으므로 다시 한번 확인한다.
//	if( !(dwOrder & DF_FLAG_CMDLINE_NT) )	g_stConnectServerInfo.m_iServiceNation	= iNation;
//	if( !(dwOrder & DF_FLAG_CMDLINE_ST) )	g_stConnectServerInfo.m_iServiceType	= iType;
	if( !SetLocaleInfo( g_stConnectServerInfo.m_iServiceNation, g_stConnectServerInfo.m_iServiceType, g_stConnectServerInfo.m_FrontServerIP ) )
	{
		ShowErrorMsgBox( 10001002 );
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_NATION_LIST, "039addeb", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	2단계 - 클라이언트 가동 가능 확인
	//----------------------------------------------------------------------------
	// 실행 가능 유무 판단
	if( !(dwOrder & DF_FLAG_CMDLINE_ACCESS) )
	{
		char	pszPath[MAX_PATH];//pszTemp[1024], 
		GetCurrentDirectory( MAX_PATH, pszPath );
		ShowErrorMsgBox( 10001003, pszPath );
//		sprintf( pszTemp, DF_ERROR_CANNOT_EXEC_LAUNCHER, pszPath, pszPath );
//		MessageBox( NULL, pszTemp, "Error!", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
	// 재가동 유무 판단
	if( dwOrder & DF_FLAG_CMDLINE_RA )
	{
		ShowErrorMsgBox( 10001004 );
//		MessageBox( NULL, DF_ERROR_UPDATE_VERSION_SERVER, "Notice!", MB_OK | MB_ICONWARNING );
		return FALSE;
	}


//	//----------------------------------------------------------------------------
//	//	3단계 - 세팅된 국가/서비스타입에 따라 서비스 정보 데이터베이스 에서 원하는 내용을 추출
//	//----------------------------------------------------------------------------
//	char	pszDummyIP[128]; 
//	
//	// 만약 명령 플래그에 의해 이미 설정된 IP가 있다면 일단 백업
//	if( dwOrder & DF_FLAG_CMDLINE_IP )	strcpy( pszDummyIP, g_stConnectServerInfo.m_FrontServerIP );
//	
//	// HQ에서 원하는 국가정보와 서비스 타입을 기준으로 IP 및 서비스사 정보등을 얻는다.
//	// 외부 HQ설정이 없을 경우
//	sprintf( pszNation, "%s\\ServiceListInfo.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
//	if( !ReadIPEx( pszNation, true, dwOrder & DF_FLAG_CMDLINE_NT, 
//					g_stConnectServerInfo.m_iServiceNation, g_stConnectServerInfo.m_iServiceType, &g_stConnectServerInfo ) )
//	{
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_ISI, "039addeb", MB_OK );
//		BindJXFile::GetInstance()->destroy();
//		return FALSE;
//	}
//	// 해당 명령 플래그가 살아있다면 이부분만 변경
//	if( dwOrder & DF_FLAG_CMDLINE_IP )	strcpy( g_stConnectServerInfo.m_FrontServerIP, pszDummyIP );
//
//	//----------------------------------------------------------------------------
//	//	3단계 - 예외처리 플래그 설정
//	//----------------------------------------------------------------------------
//	sprintf( pszNation, "Nation:%d, Type:%d", iNation, iType );								// 크래쉬 국가 코드 세팅(국가, 타입, 내부실행여부)
//	SetNationCode( pszNation );																// 크래쉬 국가 코드 설치

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Init_Engines()
// Code: actdoll (2004-03-12)
// Desc: 국가 정보 및 서버 정보 파일을 로딩한다.
//		return		- 무사히 끝냈다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	Init_Engines( HINSTANCE hInstance, int nCmdShow, DWORD dwOrder )
{
	// 초기화 과정에 있다. 
	g_bMainInit = TRUE;

	//----------------------------------------------------------------------------
	//	1단계 - 게임 가드 세팅 [게임 가드 화면 등장] (나중에 전 국가에서 서비스될 경우 이 부분은 더 앞으로 뺀다.)
	//----------------------------------------------------------------------------
	switch(g_stConnectServerInfo.m_iServiceNation)
	{
	case DF_CODE_NATION_IDENTITY_KOREA :
		if( !CreateGameGuard( !(dwOrder & DF_FLAG_CMDLINE_SKIPGG) ) )	{	ShowErrorMsgBox( 10001005 );	return( FALSE );	}
		break;
	default :
		if( !CreateGameGuard( false ) )									{	ShowErrorMsgBox( 10001005 );	return( FALSE );	}
		break;
	}


	//----------------------------------------------------------------------------
	//	2단계 - 거상 게임클래스 구동
	//----------------------------------------------------------------------------

	// 게임 클래스 생성 및 정보 세팅
	pGame =	new _clGame;
	if(pGame == NULL)	
	{
		// actdoll (2004/03/11 11:39) : 에러 발생 후 모조리 리턴시 에러 코드 보여준다.
		ShowErrorMsgBox( 10001006 );
//		MessageBox( NULL, DF_ERROR_MEMORY_NOT_ENOUGH, "ERROR(5bdf448c)", MB_OK );
		return FALSE;
	}

	
	//----------------------------------------------------------------------------
	//	3단계 - 서비스 구성 및 접속 기본 정보 세팅
	//----------------------------------------------------------------------------

	pGame->m_iNation		= g_stConnectServerInfo.m_iServiceNation;
	pGame->m_iServiceType	= g_stConnectServerInfo.m_iServiceType;
	pGame->LanguageCode		= (ELanguageCode)g_stConnectServerInfo.m_iServiceLanguage;
	pGame->m_pszHQNation	= DF_TEXT_SERVICE_NATION_LIST[pGame->LanguageCode];
	pGame->m_uiCodePage		= g_stConnectServerInfo.m_iServiceCP;

	
	//----------------------------------------------------------------------------
	//	4단계 - 윈도 및 DirectX 가동
	//----------------------------------------------------------------------------

	if(!pGame->Init(hInstance, nCmdShow))
	{
		ShowErrorMsgBox( 10001007 );
//		MessageBox( NULL, DF_ERROR_DXINIT_IS_FAILED, "ERROR(8e756bc4)", MB_OK );
		delete pGame;
		return FALSE;
	}
	SetHwndToGameGuard( pGame->Hwnd );	// actdoll (2004/10/19 11:44) : 게임가드에 적용할 윈도 핸들을 이부분에 세팅
	
	// 초기화가 종료되었다. 
	g_bMainInit = FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Kill()
// Code: actdoll (2004-03-12)
// Desc: 메인에서 로딩된 모든 데이터들을 해제
//-----------------------------------------------------------------------------
void	Kill()
{
	if( pGame )
	{
		delete pGame;
		pGame	= NULL;
	}

	BindJXFile::GetInstance()->destroy();

	// actdoll (2004/10/19 11:58) : 
	//	게임가드 해제 
	ReleaseGameGuard();
}



//-----------------------------------------------------------------------------
// Name: FCS()
// Code: actdoll (2003-12-30)
// Desc: 파일 체크섬을 처리하는 함수다.
//		기존의 체크섬을 보강한 방법이다. byte, word, dword 단위의 데이터 교체, 치환, 합산 등에 대한
//		크래킹을 철저히 차단할 수 있는 방법이다. 다만 함수 소비시간이 좀 든다.
//		(CPU 2.8에서 평균15~17ms)
//
//		pFileName		- 체크섬을 하고 싶은 파일명
//		bAdminOption	- 관리자용 데이터값 추출 시
//		return			- 체크섬 결과값(64bit)
//-----------------------------------------------------------------------------
unsigned __int64	FCS( char* pFileName, BOOL bAdminOption )
{
	if( !pFileName  )					return 0;

	BOOL	bWork		= false; 
	BYTE	*pbtFile	= NULL;
	FILE	*fp			= NULL;
	UI32	uiReadSize = 0, uiFileSize = 0;

	fp = fopen( pFileName, "rb" );
	if( !fp )							return 0;
	fseek( fp, 0, SEEK_END );
	uiFileSize = ftell( fp );
	if( uiFileSize != 0xffffffff && uiFileSize != 0 )
	{
		pbtFile = new BYTE[uiFileSize];
		fseek( fp, 0, SEEK_SET );
		if( fread( pbtFile, sizeof(BYTE), uiFileSize, fp ) )	bWork = true;
	}
	fclose( fp );

	// 파일 오픈
//	HANDLE	hFile = ::CreateFile( pFileName, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH | FILE_FLAG_SEQUENTIAL_SCAN, NULL );
//	if( hFile == INVALID_HANDLE_VALUE )	return 0;
//	UI32	uiReadSize, uiFileSize = GetFileSize( hFile, NULL );
//	if( uiFileSize != INVALID_FILE_SIZE && uiFileSize != 0 )
//	{
//		pbtFile	= new BYTE[uiFileSize] ;
//		if( ReadFile( hFile, pbtFile, uiFileSize, (LPDWORD)&uiReadSize, NULL ) )	bWork = true;
//	}
//	CloseHandle( hFile );

	UI32	uiCheckSumValue	= DF_VALUE_CSF;
	UI32	length, remainder;
	UI32	hi, lo;
	BYTE	*pbtData	= pbtFile;

	if( bWork )
	{

		length		= ( uiFileSize >> 2 ) << 1;			// 실제 계산할 길이를 구한다.
		remainder	= ( uiFileSize - ( length << 1 ) );	// 나머지 바이트를 계산해논다.

		hi	= ( uiCheckSumValue >> 16 );
		lo	= ( uiCheckSumValue << 16 ) >> 16;
		for( UI32 index = 0; index < length; index += 2 )
		{
			hi	+= (UI32)( (*pbtData++) + (uiFileSize--) );
			lo	+= (UI32)( (*pbtData++) + (uiFileSize--) ) << 16;
			hi	+= (UI32)( (*pbtData++) + (uiFileSize--) ) << 8;
			lo	+= (UI32)( (*pbtData++) + (uiFileSize--) ) << 8;

			hi	= (hi << 8) + (hi >> 24);
			lo	= (lo << 8) + (lo >> 24);
		}
		if( remainder >= 1 )	hi += (UI32)( pbtData[0] + (uiFileSize--) );
		if( remainder >= 2 )	lo += (UI32)( pbtData[1] + (uiFileSize--) ) << 16;
		if( remainder == 3 )	hi += (UI32)( pbtData[2] + (uiFileSize--) ) << 8;
		if( remainder )
		{
			hi	= (hi << 8) + (hi >> 24);
			lo	= (lo << 8) + (lo >> 24);
		}

	}

	delete [] pbtFile;

    return ((unsigned __int64)hi << 32 ) | lo;
}

//-----------------------------------------------------------------------------
// Name: DataWork()
// Code: actdoll (2004-03-12)
// Desc: HQ파일들의 로딩과 관련된 작업을 처리하는 함수다.
//		return		- true라면 HQ를 정상적으로 확인, false라면 HQ작업 실패
//-----------------------------------------------------------------------------
BOOL	DataWork( HINSTANCE hInstance )
{
	// 각 국가별 HQ파일 검색
	char	*pszHQFile = ".\\hq";

	// actdoll (2004/01/15 23:55) : 중요한 hq데이터 파일을 해킹하지 못하도록 체크섬을 걸어논다.
	//		Debug 모드에서는 표시된 수치를 DF_VALUE_CS 값에 대입하여 컴파일하면 되며 그냥 진행할 수 있으나
	//		Release 모드에서는 수치가 맞지 않을 경우 그냥 나가버린다.
	//		또한 신기하게도(?) 이 hq파일을 체크한 후 다른 거상 프로그램이 돌아갈 때 hq 파일을 읽지 못해
	//		이중실행이 되지 않는다. 아마도 윈도 File 관련 API의 버그 혹은 특성인 듯 하다.

	// actdoll (2004/05/27 18:54) : V.12250부터 모든 버젼에 상관없이 hq로 이름을 통합한다.
	FILE	*fp	= fopen( pszHQFile, "r" );
	if( !fp )
	{
		MessageBox( NULL, DF_ERROR_CANNOT_FIND_HQ, "Gersang", MB_OK );
		return FALSE;
	}
	fclose( fp );

	if( FCS( pszHQFile ) != DF_VALUE_CS )
	{	
#ifdef _DEBUG
		char	pTemp[256];
//		sprintf( pTemp, "File [ %s ] CheckSum ERROR !\n\n0x%016I64X", pszHQFile, FCS( pszHQFile ) );
//		MessageBox( NULL, pTemp, "Cracking Safer - Only Debug", MB_OK );
		sprintf( pTemp, "0x%016I64X", FCS( pszHQFile ) );
		ShowDlg( hInstance, "크래킹 세이퍼 췍섬 에롸", pTemp );
#else
		return FALSE;
#endif
	}

	// 이 부분은 일반 운영자가 테스트를 위한 어드민 옵션을 설정할 경우 사용할 시리얼 키를 리턴한다.
//#ifdef _DEBUG
//	sprintf( pTemp, "[ Administrator Authorization Code ]\n\n\n%I64d", FCS( pszHQFile, true ) );
//	MessageBox( NULL, pTemp, "AAC Notice - Only Debug", MB_OK );
//	ShowDlg( hInstance, "관리자 인증 코드 (AAC)", (char*)DF_VALUE_AAC );
//#endif

	return ( BindJXFile::GetInstance()->openJX( pszHQFile ) );
}

//-----------------------------------------------------------------------------
// Name: GetNTN()
// Code: actdoll (2004-03-12)
// Desc: 국가 정보 및 서버 정보 파일을 체크한다.
//		pszFileName	- ntn파일명
//		iNation		- 서비스 국가 코드, 에러라면 -1 리턴
//		iType		- 서비스 타입, 에러라면 -1 리턴
//		return		- true라면 정상적으로 데이터를 확인, false라면 파일이 없거나 정보 추출 실패
//-----------------------------------------------------------------------------
BOOL	GetNTN( char *pszFileName, int &iNation, int &iType )
{
	FILE	*pFile;

	iNation = iType = -1;

	pFile	= fopen( pszFileName, "rb" );
	if( !pFile )	return FALSE;

	// 파일을 열어 정보를 살핀다.
	DWORD	dwNTNVersion, dwNation, dwType;
	
	// 파일 버젼 읽기
	fread( &dwNTNVersion, sizeof(DWORD), 1, pFile );
	if( dwNTNVersion == ON_NTNFILE_VERSION )
	{
		fread( &dwNation,	sizeof(DWORD), 1, pFile );			// 서비스 국가 번호
		fread( &dwType,		sizeof(DWORD), 1, pFile );			// 서비스 타입
	}
	else
	{
		dwNation = dwType = 0xffffffff;
	}
	fclose( pFile );

	// 검증 - 국가 번호
	switch( dwNation )
	{
		case DF_CODE_NATION_IDENTITY_KOREA			:	// 한국
		case DF_CODE_NATION_IDENTITY_JAPAN			:	// 일본
		case DF_CODE_NATION_IDENTITY_CHINA			:	// 중국
		case DF_CODE_NATION_IDENTITY_TAIWAN			:	// 대만
		case DF_CODE_NATION_IDENTITY_HONGKONG		:	// 홍콩
		case DF_CODE_NATION_IDENTITY_INDONESIA		:	// 인도네시아
				iNation		= (int)dwNation;
		default			:
			break;
	}

	// 검증 - 타입
	iType	= (int)dwType;
	if( dwNation == DF_CODE_NATION_IDENTITY_KOREA && iType == 0 )	g_fTestVersion	= true;		// 한국판에 타입이 0일 경우 국내 테섭
	else															g_fTestVersion	= false;	// 아닐 경우 테섭이 아니다.

	if( iNation == -1 || iType == -1 )		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: CreateNTN()
// Code: actdoll (2004-03-12)
// Desc: 국가 정보 및 서버 정보 파일을 설정한다.
//		pszFileName	- ntn파일명
//		iVersion	- 저장된 버젼
//		iNation		- 서비스 국가 코드
//		iType		- 서비스 타입
//		return		- true라면 정상적으로 데이터를 확인, false라면 파일이 없거나 정보 추출 실패
//-----------------------------------------------------------------------------
BOOL	CreateNTN( char *pszFileName, int &iNation, int &iType )
{
	FILE	*pFile;
	char	pszTemp[12] = {0,};

	// 타입을 맞춰준다.
	if( iType == -1 )	iType = 1;

	// 현재 시스템의 국가 정보를 추출할 수 없다면 OS기본 정보로 가동하고 리턴이다.
	if( iNation == -1 )
	{
		GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTCOUNTRY, pszTemp, sizeof(pszTemp) );
		iNation	= atoi( pszTemp );
		return FALSE;
	}

	// 제대로 확인됐다면 세팅 파일 저장
	pFile	= fopen( pszFileName, "wb" );
	if( !pFile )	return FALSE;

	fwrite( &ON_NTNFILE_VERSION,	sizeof(DWORD), 1, pFile );			// NTN 파일 버젼
	fwrite( &iNation,				sizeof(DWORD), 1, pFile );			// 서비스 국가 번호
	fwrite( &iType,					sizeof(DWORD), 1, pFile );			// 서비스 타입

	fclose( pFile );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetLocaleInfo()
// Code: actdoll (2004-03-12)
// Desc: 국가 정보값을 받아서 게임의 로케일 값들을 맞춰준다.
//		iServiceNation	- 서비스 대상의 국가(전화)번호
//		return			- true라면 정상적으로 데이터를 확인, false라면 지원을 하지 않는 국가(전화)번호이다.
//-----------------------------------------------------------------------------
BOOL	SetLocaleInfo( int iServiceNation, int iServiceType, char *pszCustomFrontServerIP )
{
	// 에러값이 아니라면 세팅
	if( iServiceNation > 0 )		g_stConnectServerInfo.m_iServiceNation	= iServiceNation;
	if( iServiceType >= 0 )			g_stConnectServerInfo.m_iServiceType	= iServiceType;

	// 국가정보 설정값에 따라 언어 코드를 설정해준다.
	switch( g_stConnectServerInfo.m_iServiceNation )
	{
		case DF_CODE_NATION_IDENTITY_KOREA		:	
			g_stConnectServerInfo.m_iServiceLanguage = ELANGAGECODE_KOREAN;		g_stConnectServerInfo.m_iServiceCP = DF_CODE_CODEPAGE_KOREAN;	
			break;
		case DF_CODE_NATION_IDENTITY_TAIWAN		:	
			g_stConnectServerInfo.m_iServiceLanguage = ELANGAGECODE_TAIWAN;		g_stConnectServerInfo.m_iServiceCP = DF_CODE_CODEPAGE_CHINESE_TRAD;	
			break;
		case DF_CODE_NATION_IDENTITY_JAPAN		:	
			g_stConnectServerInfo.m_iServiceLanguage = ELANGAGECODE_JAPAN;		g_stConnectServerInfo.m_iServiceCP = DF_CODE_CODEPAGE_JAPANESE;	
			break;
		case DF_CODE_NATION_IDENTITY_HONGKONG	:	
			g_stConnectServerInfo.m_iServiceLanguage = ELANGAGECODE_HONGKONG;	g_stConnectServerInfo.m_iServiceCP = DF_CODE_CODEPAGE_CHINESE_TRAD;	
			break;
		case DF_CODE_NATION_IDENTITY_CHINA		:	
			g_stConnectServerInfo.m_iServiceLanguage = ELANGAGECODE_CHINA;		g_stConnectServerInfo.m_iServiceCP = DF_CODE_CODEPAGE_CHINESE_SIMP;	
			break;
		case DF_CODE_NATION_IDENTITY_INDONESIA	:	
			g_stConnectServerInfo.m_iServiceLanguage = ELANGAGECODE_INDONESIA;	g_stConnectServerInfo.m_iServiceCP = DF_CODE_CODEPAGE_ENGLISH;	
			break;
		default									:
			return FALSE;
	}

	// 주어진 서비스 지역정보를 기준으로 나머지 데이터를 뽑아낸다.
	FILE*	fp;
	OnlineCommonParser	ocp;
	char	pszFileName[MAX_PATH];

	sprintf( pszFileName, "%s\\ServiceListInfo.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );
	if( !fp )	return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "ServiceListInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	BOOL	bRet	= false;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		char	pszDesc[48], pszTemp[128];
		SI32	siIndex, siNation, siType;
		ocp.GetValue( siIndex,			iLineNo, iColNo );		// 번호
		ocp.GetValue( pszDesc,	47,		iLineNo, iColNo );		// 설명문
		ocp.GetValue( siNation,			iLineNo, iColNo );		// 서비스 국가 번호
		ocp.GetValue( siType,			iLineNo, iColNo );		// 서비스 타입 번호

		// 일단 여기까지 찾고 정보가 일치하지 않으면 리턴
		if( siNation != iServiceNation || siType != iServiceType )	continue;
		bRet	= true;

		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// 버젼 서버 IP
		// 만약 여기서 사설 IP가 있다면 그 값으로 변경
		if( pszCustomFrontServerIP )	
		{
			strcpy( g_stConnectServerInfo.m_FrontServerIP, pszCustomFrontServerIP );
			ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// 프론트 서버 IP
		}
		else
		{
			ocp.GetValue( g_stConnectServerInfo.m_FrontServerIP,			127,	iLineNo, iColNo );		// 프론트 서버 IP
		}
		ocp.GetValue( g_stConnectServerInfo.m_ServiceName,				127,	iLineNo, iColNo );		// 서비스 업체명
		ocp.GetValue( g_stConnectServerInfo.m_ServiceURL,				511,	iLineNo, iColNo );		// 서비스 업체 URL
		ocp.GetValue( g_stConnectServerInfo.m_ServiceCreateAccountURL,	511,	iLineNo, iColNo );		// 등록 페이지 URL
		ocp.GetValue( g_stConnectServerInfo.m_ServiceShopName,			127,	iLineNo, iColNo );		// 쇼핑몰 명
		ocp.GetValue( g_stConnectServerInfo.m_ServiceShopURL,			511,	iLineNo, iColNo );		// 쇼핑몰 업체 URL
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// 기타
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// 기타 URL

		break;
	}

	fclose( fp );
	ocp.Release();

	return bRet;
}

//-----------------------------------------------------------------------------
// Name: ReadIPEx()
// Code: actdoll (2004-06-01)
// Desc: 국가별 정보 데이터베이스 파일을 분석하고 해당 정보를 추출해 낸다.
//		만약 현재 사내테섭용으로 돌고 있다면 무조건 첫째줄만 읽는다.
//		lpFileName				- ServiceInfo.cfg 파일명
//		bUseHQ					- HQ내의 파일을 읽을 경우 true, 일반 파일을 읽을 경우 false
//		bJustReadFirstLine		- 테스트시 사용되는 플래그. true 일 경우 해당 정보파일의 첫번째 라인값을 데이터로 넘긴다. 
//								false일 경우 iNation, iType과 일치하는 데이터를 찾아 리턴한다.
//		iNation					- 서비스 국가 코드
//		iType					- 서비스 타입
//		lpSConnectServerInfo	- 조건에 맞는 국가 정보를 발견했을 때 저장될 버퍼
//		return					- 제대로 찾아냈다면 true, 문제가 있다면 false
//-----------------------------------------------------------------------------
BOOL	ReadIPEx( char* lpFileName, BOOL bUseHQ, BOOL bJustReadFirstLine, int iNation, int iType, SConnectServerInfo* lpSConnectServerInfo )
{
	FILE*	fp;
	OnlineCommonParser	ocp;

	if( !lpFileName || !lpSConnectServerInfo )	return FALSE;

	// HQ에서 파일을 연다.
	if( bUseHQ )
	{
		fp = BindJXFile::GetInstance()->GetFile( lpFileName );
		if( !fp )	return FALSE;
	}
	// 일반 파일을 연다.
	else
	{
		fp = fopen( lpFileName, "rb" );
		if( !fp )	return FALSE;
	}

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "ServiceInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		char	pszDesc[48], pszTemp[128];
		SI32	siIndex, siServiceNation, siServiceType;
		ocp.GetValue( siIndex,			iLineNo, iColNo );		// 번호
		ocp.GetValue( pszDesc,	47,		iLineNo, iColNo );		// 설명문
		ocp.GetValue( siServiceNation,	iLineNo, iColNo );		// 서비스 국가 번호
		ocp.GetValue( siServiceType,	iLineNo, iColNo );		// 서비스 타입 번호

		//----------------------------------------------------------------------------
		// 일단 여기까지 찾고 정보가 일치하지 않으면 리턴, 첫라인 값을 추출하라는 것이 아니면 계속 순환
		if( !bJustReadFirstLine )
		{
			if( iNation != siServiceNation || iType != siServiceType )	continue;
		}
		// 넘어왔다면 찾은 것이다.
		lpSConnectServerInfo->m_iServiceNation		= siServiceNation;
		lpSConnectServerInfo->m_iServiceType		= siServiceType;
		//----------------------------------------------------------------------------

		ocp.GetValue( lpSConnectServerInfo->m_FrontServerIP,			127,	iLineNo, iColNo );		// 버젼 서버 IP
		ocp.GetValue( lpSConnectServerInfo->m_FrontServerIP,			127,	iLineNo, iColNo );		// 프론트 서버 IP
		ocp.GetValue( lpSConnectServerInfo->m_ServiceName,				127,	iLineNo, iColNo );		// 서비스 업체명
		ocp.GetValue( lpSConnectServerInfo->m_ServiceURL,				511,	iLineNo, iColNo );		// 서비스 업체 URL
		ocp.GetValue( lpSConnectServerInfo->m_ServiceCreateAccountURL,	511,	iLineNo, iColNo );		// 등록 페이지 URL
		ocp.GetValue( lpSConnectServerInfo->m_ServiceShopName,			127,	iLineNo, iColNo );		// 쇼핑몰 명
		ocp.GetValue( lpSConnectServerInfo->m_ServiceShopURL,			511,	iLineNo, iColNo );		// 쇼핑몰 업체 URL
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// 기타
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// 기타 URL
		
		fclose( fp );
		ocp.Release();
		return TRUE;
	}

	fclose( fp );
	ocp.Release();
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: ShowDlg(), DlgBoxProc()
// Code: actdoll (2004-06-01)
// Desc: 간단한 복사형 출력 다이얼로그를 표시한다.
//		hInstance	- 이 클라이언트의 인스턴스 핸들
//		pszTitle	- 메시지 타이틀
//		pszText		- 복사 가능한 출력 메시지
//-----------------------------------------------------------------------------
void	ShowDlg( HINSTANCE hInstance, char *pszTitle, char *pszText )
{
	if( pszTitle )	strcpy( g_pszDlgTitle, pszTitle );
	if( pszText )	strcpy( g_pszDlgText, pszText );
	DialogBox( hInstance, MAKEINTRESOURCE(IDD_MSGBOX_EDITTYPE), NULL, (DLGPROC)DlgBoxProc );
	ZeroMemory( g_pszDlgTitle, sizeof(g_pszDlgTitle) );
	ZeroMemory( g_pszDlgText, sizeof(g_pszDlgText) );
}

BOOL	CALLBACK	DlgBoxProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_INITDIALOG:
			Beep( 1000, 100 );
			SetDlgItemText( hDlg, IDS_MESSAGE,	g_pszDlgTitle );
			SetDlgItemText( hDlg, IDE_RESULT,	g_pszDlgText );
			break;

		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDB_OK:
					EndDialog( hDlg, wParam );
					return TRUE;
			}
			break;

		case WM_CLOSE:
		case WM_DESTROY:
			EndDialog( hDlg, wParam );
			return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: GetAdDecoData()
// Code: actdoll (2004-12-16)
// Desc: 운영자 명령어 데이터를 받아 해독하여 리턴한다.
//		pDest				- 인코딩 값을 받을 버퍼 포인터
//		uiDestBuffSize		- pDest 버퍼 크기
//		dwNTNValue			- NTN 코드값
//		bSkipGameGuard		- 게임가드 실행 안하면 true, 실행이라면 false
//		bSkipLauncher		- 런쳐없이 실행된다면 true, 런쳐 반드시 거치게 한다면 false
//		bSkipIntro			- 인트로 없이 로딩한다면 true, 아니라면 false
//		pszAACCode			- AAC코드값
//		pszVersionServerIP	- 설정할 버전 서버 IP
//		pszFrontServerIP	- 설정할 프론트 서버 IP
//		pszPathCustomHQ		- 설정할 외부 HQText가 들어있는 폴더명
//		return				- 정상적으로 작업했다면 pDest에 들어있는 데이터의 byte값, 문제가 있다면 0
//-----------------------------------------------------------------------------
UINT	GetAdDecoData( BYTE* pSrc, UINT uiSrcBuffSize, DWORD& dwNTNValue, BOOL& bSkipGameGuard, BOOL& bSkipLauncher, BOOL& bSkipIntro, BOOL& bRunAsWindowMode, 
						LPSTR pszAACCode, LPSTR pszVersionServerIP, LPSTR pszFrontServerIP, LPSTR pszPathCustomHQ )
{
	if( !pSrc )			return 0;

	BYTE	btTemp;
	WORD	wTemp;
	DWORD	dwTemp;

	BYTE	pbtDummy[4096]	= {0,};
	BYTE	*pPointer		= pbtDummy;

	// 값 풀기
	char		pszData[3];
	for( UINT i = 0, uiLen = 0; i < uiSrcBuffSize; i += 2, uiLen++ )
	{
		memcpy( pszData, pSrc + i, 2 );
		pszData[2] = 0;
		*( pbtDummy + uiLen ) = (BYTE)strtol( pszData, NULL, 16 );
	}

	// 인코딩 작업 개시
	DWORD	dwChkSum[8]	=
	{	
		0x8ac7d236,		0x135f47e6,		0xfcad6318,		0xa39e17bc,
		0xd5390148,		0x6e3f4d9a,		0x7efa143c,		0x957bd340,
	};

	// 체크섬 대입하여 해제한다.
	DWORD	*dwPointer	= (DWORD*)pbtDummy;
	for( i = 0; i < uiLen >> 2; i++ )
	{
		*dwPointer	^=	dwChkSum[i % 8];
		dwPointer++;
	}

	// 확인사살
	ZeroMemory( dwChkSum, sizeof(DWORD)*8 );

	uiLen		= 0;
	// NTN 적재(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	dwNTNValue					= (DWORD)wTemp;
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// AAC코드 길이 적재(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// AAC코드 내용 적재(?B)
	memcpy( pszAACCode, pPointer, wTemp );
	pszAACCode[wTemp]			= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// 게임가드 스킵? 적재(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipGameGuard				= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// 버전서버 IP 길이 적재(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// 버전서버 IP 내용 적재(?B)
	memcpy( pszVersionServerIP, pPointer, wTemp );
	pszVersionServerIP[wTemp]	= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;
	
	// 런쳐 스킵? 적재(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipLauncher				= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// 프론트서버 IP 길이 적재(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// 프론트서버 IP 내용 적재(?B)
	memcpy( pszFrontServerIP, pPointer, wTemp );
	pszFrontServerIP[wTemp]		= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// 인트로 스킵? 적재(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipIntro					= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// HQ텍스트 길이 적재(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// HQ텍스트 IP 내용 적재(?B)
	memcpy( pszPathCustomHQ, pPointer, wTemp );
	pszPathCustomHQ[wTemp]		= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// 윈도모드 허가? 적재(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bRunAsWindowMode			= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// 플래그 인증값 적재(4B)
	memcpy( &dwTemp, pPointer, sizeof(DWORD) );
	pPointer	+= sizeof(DWORD);
	uiLen		+= sizeof(DWORD);
	if( dwTemp != DF_VALUE_ATI )		// 플래그 인증값이 맞지 않다면 닝기리!
	{
		uiLen	= 0;
	}

	// 작업에 쓰였던 값 전부 해제
	ZeroMemory( pbtDummy, 4096 );
	ZeroMemory( pszData, 3 );

	return uiLen;
}

//-----------------------------------------------------------------------------
// Name: ShowErrorMsgBox()
// Code: actdoll (2004-03-12)
// Desc: 각 국가별로 문제가 된 스트링을 윈도 메시지박스 형태로 출력해준다.
//		iErrorCode		- 에러코드
//-----------------------------------------------------------------------------
void	ShowErrorMsgBox( int iErrorCode, ... )
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	BOOL				bFind = false;
	char				pszFullErrorMsg[2048] = {0,}, pszDummy[1024] = {0,};

	// 일단 먼저 에러코드 버퍼에 코드값을 넣어놓는다.
	sprintf( pszFullErrorMsg, "Error Code - %08X\n\n", iErrorCode);

	// 국적이 존재한다면 HQ에서 해당 언어로 된 에러 파일을 연다.
	if( g_stConnectServerInfo.m_iServiceLanguage > ELANGAGECODE_NONE && g_stConnectServerInfo.m_iServiceLanguage < ELANGAGECODE_MAX )
	{
		char	pszFileName[MAX_PATH];
		sprintf( pszFileName, "%s\\ErrorText.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
		if( fp = BindJXFile::GetInstance()->GetFile( pszFileName ) )
		{
			// 파싱을 시작한다.
			int		iRet, iErrLine;
			BOOL	bLess;
			if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) > 0 )
			{
				// 자료 검색
				while( !bFind )
				{
					if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

					SI32	iLineNo, iColNo;

					// 자료를 순차적으로 받는다.
					int		iCode;
					char	pszErrorText[1024], pszBuffer[1024];

					ocp.GetValue( iCode,				iLineNo, iColNo );		// 번호
					ocp.GetValue( pszBuffer,	1023,	iLineNo, iColNo );		// 설명문

					// 일단 1번 에러메시지는 무조건 저장해 놓는다. 에러메시지가 번역이 안된 경우를 위해서다.
					if( iCode != iErrorCode )
					{
						if( iCode != 1 )		continue;
					}
					// 그렇지 않을 경우 에러 메시지를 재편한다.
					else
					{
						sprintf( pszFullErrorMsg, "Error Code - %08X\n\n", iErrorCode);
						bFind	= true;
					}

					// 출력구문 수정
					ConvertSpecialChar( pszBuffer );
					// 에러메시지를 갱신한다.
					va_list		marker;
					va_start( marker, iErrorCode );
					vsprintf( pszErrorText, pszBuffer, marker );
					va_end( marker );     

					// 메시지를 복사해 놓는다.
					strcat( pszFullErrorMsg, pszErrorText );

					// 한 줄을 뗀다
					strcat( pszFullErrorMsg, "\n\n\n" );
				}
				ocp.Release();
			}
			fclose( fp );
		}
	}

	// 다음은 영어 기준의 에러 파일을 연다.
	if( fp = BindJXFile::GetInstance()->GetFile( "ErrorText_Eng.dat" ) )
	{
		// 파싱을 시작한다.
		int		iRet, iErrLine;
		BOOL	bLess;
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "ErrorText_Eng Error : Cannot Init! [ Ret-%d | Line-%d | Less-%d ]", iRet, iErrLine, bLess );
		}
		else
		{
			// 자료 검색
			while(1)
			{
				if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출
				
				SI32	iLineNo, iColNo;
				
				// 자료를 순차적으로 받는다.
				int		iCode;
				char	pszErrorText[1024], pszBuffer[1024];
				
				ocp.GetValue( iCode,				iLineNo, iColNo );		// 번호
				ocp.GetValue( pszBuffer,	1023,	iLineNo, iColNo );		// 설명문
				
				// 데이터를 찾지 못했다면 그 다음줄을 검색한다.
				if( iCode != iErrorCode )		continue;
				bFind	= true;

				// 출력구문 수정
				ConvertSpecialChar( pszBuffer );
				// 에러메시지를 갱신한다.
				va_list		marker;
				va_start( marker, iErrorCode );
				vsprintf( pszErrorText, pszBuffer, marker );
				va_end( marker );     

				// 메시지를 복사해 놓는다.
				strcat( pszFullErrorMsg, pszErrorText );
				break;
			}
			ocp.Release();
		}
		fclose( fp );
	}

	// 만약 해당 인덱스를 기준으로 메시지를 하나도 발견하지 못했다면
	if( !bFind )
	{
		sprintf( pszFullErrorMsg, "ErrorCode [%08X]\nUnknown Error Message Type!", iErrorCode );
	}

	MessageBox( ( pGame ? pGame->Hwnd : NULL ), pszFullErrorMsg, "Gersang", MB_OK|MB_ICONWARNING|MB_TOPMOST );
}

//-----------------------------------------------------------------------------
// Name: ConvertSpecialChar()
// Code: actdoll (2004-03-12)
// Desc: OnlineText 클래스에 있는 기능과 동일한 놈이다. 
//		메시지 처리의 편의를 위해 일단 여기에서 동원해 쓴다.
//		iErrorCode		- 에러코드
//-----------------------------------------------------------------------------
void	ConvertSpecialChar(CHAR *pSpecialString)
{
	CHAR *oriString = pSpecialString;
	bool bSpecial;
	CHAR *pWriteString;

	pWriteString = pSpecialString;
	bSpecial = false;

	while( *pSpecialString != '\0' )
	{
		if( bSpecial == true )
		{
			switch(*pSpecialString)
			{
				case '0' :
					*pWriteString++ = '\0';
					return;
				case 'n' :
					*pWriteString++ = '\n';
					*pWriteString++ = '\r';
					break;
				case 't' :
					*pWriteString++ = '\t';
					break;
				default:
					break;
			}
			bSpecial = false;
		}
		else
		{
			if( *pSpecialString == '\\' )
			{
				bSpecial = true;
			}
			else
			{
				*pWriteString++ = *pSpecialString;
			}
		}
		pSpecialString++;
	}
	*pWriteString = '\0';
}
