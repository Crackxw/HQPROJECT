//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	������ �¶��� [�Ż�] - ���ι�
//	File Name		:	Main.cpp
//	Birth Date		:	2004. 01. 01.
//	Creator			:	�� �� ��
//	Editer			:	���°�, ������, ������, �����, ���ؼ�, ������, 
//						ȫ����, ������, �̹μ�, ������, �ڻ��, �輺��,
//						������, ������, �ֺ���, ������
//	Copyright		:	Joyon Entertainments. Co. 2000. 2005.
//	Comments		: 
//		�Ż��� ���ι��Դϴ�.
//
//===================================================================================================

//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
/********************************************************************
*
*  1998.8.22~ 2000.1.6
*  ������2 
*
*********************************************************************/
// Initialize the COM interface identifiers
//#define WIN32_LEAN_AND_MEAN
//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: �⺻ ��� ����
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

// actdoll (2004/10/19 11:45) : ���Ӱ��� �Լ� ����
#include "GGuard.h"

//-----------------------------------------------------------------------------
// actdoll (2004/03/11 13:58) : ũ��Ƽ�� ���׸� ������� ����׿� ���� ���� Ŭ����
//-----------------------------------------------------------------------------
#include "..\\SCCommon\\JODebug.h"
#include <GSMemory.h>

#include <resource.h>

// ���� ������ ���. 
//#define _MAPEDIT	1

//--------------------------------------------------------------------------
// �ʱ�ȭ ������ ������� ��Ʈ��
//--------------------------------------------------------------------------
const	char*	DF_ERROR_MEMORY_NOT_ENOUGH			= "�޸𸮰� ������� �ʽ��ϴ�!\n\n Memory is not enough! Program will be shutdown.";
const	char*	DF_ERROR_CREATE_STANDARD_NTN		= "Error Code [80000001]\nCannot check International Information Setting(IIS)!\n\n Program will run by basic informations of OS.\n We recommand to reinstall when you catch some problems.";
const	char*	DF_ERROR_CANNOT_FIND_HQ				= "Error Code [80000002]\nCRITICAL ERROR!\n\n  CANNOT FIND CORE DATA!\n  PLEASE REINSTALL.";
const	char*	DF_ERROR_WARNING_FOR_CRACK			= "Error Code [80000003]\nCRITICAL ERROR!\n\n  CORE DATA AUTHORIZATION FAILED!\n  PLEASE REINSTALL.";
const	char*	DF_ERROR_DXINIT_IS_FAILED			= "�ý��� �ʱ�ȭ �� ������ �߻��߽��ϴ�!\n\n System Initialize is Failed!";
const	char*	DF_ERROR_CANNOT_EXEC_LAUNCHER		= "�� ���������� �ٷ� �۵���ų �� �����ϴ�!\n%s ������ �ִ� run.exe�� ������ �ֽʽÿ�.\n���� run.exe�� �����ߴµ� �� ������ ��ٸ� ���İ� ������Ʈ�� ����̹Ƿ� �ٽ� �ѹ� �õ��� �ֽʽÿ�.\n\n You cannot run this file directly!\n Please execute the \"run.exe\" file in %s folder.\n Also you can see this when launcher was updated. Then, please try again.";
const	char*	DF_ERROR_UPDATE_VERSION_SERVER		= "����� ������ �����ϱ� ���� ������ �ٽ� �ѹ� ������ �ֽʽÿ�.\n\n Please try again to run the game for applying new updates.";
const	char*	DF_ERROR_CANNOT_FIND_ISI			= "������������Ʈ ������ ã�� �� ���ų� ������ �ֽ��ϴ�! ���α׷��� �ٽ� ��ġ�� �ּ���.\n\n International Information List(IIL) is incorrect! Please reinstall.";
const	char*	DF_ERROR_CANNOT_FIND_NATION_LIST	= "������ ���������� ��ġ�ϴ� �����Ͱ� �����ϴ�. ���α׷��� �ٽ� ��ġ�� �ּ���.\n\n International Information List(IIL) is Mismatched with International Information Setting(IIS)!\n Please reinstall.";

//--------------------------------------------------------------------------
// ����ȭ�� ���̼��� �ؽ�Ʈ
//--------------------------------------------------------------------------
const	char*	DF_TEXT_COPYRIGHT_WITH_GAMANIA	= "Copyright 2001-2005. Gamania & Joyon Entertainment. All rights reserved.";		// �ѱ�/�Ϻ�/�븸/�߱�/ȫ���
const	char*	DF_TEXT_COPYRIGHT_WITH_BOLEHCOM	= "Copyright 2004-2005. BOLEH.COM & Joyon Entertainment. All rights reserved.";		// �ε��׽þƿ�

//--------------------------------------------------------------------------
// �� ������ �ν��ڵ� - �� ������ ��ȭ��ȣ �ڵ� - ���� ���۵� ������� �ۼ�
//--------------------------------------------------------------------------
const	int		DF_CODE_NATION_IDENTITY_KOREA		= 82;		// �ѱ�
const	int		DF_CODE_NATION_IDENTITY_TAIWAN		= 886;		// �븸
const	int		DF_CODE_NATION_IDENTITY_JAPAN		= 81;		// �Ϻ�
const	int		DF_CODE_NATION_IDENTITY_HONGKONG	= 852;		// ȫ��
const	int		DF_CODE_NATION_IDENTITY_CHINA		= 86;		// �߱�
const	int		DF_CODE_NATION_IDENTITY_INDONESIA	= 62;		// �ε��׽þ�

//--------------------------------------------------------------------------
// �� �� �ڵ�������
//--------------------------------------------------------------------------
const	int		DF_CODE_CODEPAGE_ENGLISH			= CP_ACP;			// ����(�⺻)
const	int		DF_CODE_CODEPAGE_KOREAN				= 949;				// �ѱ��� 
const	int		DF_CODE_CODEPAGE_JAPANESE			= 932;				// �Ϻ���
const	int		DF_CODE_CODEPAGE_CHINESE_SIMP		= 936;				// �߱���(�߱�, �̰�����)
const	int		DF_CODE_CODEPAGE_CHINESE_TRAD		= 950;				// �߱���(�븸, ȫ��)

//--------------------------------------------------------------------------
// �� ������ HQ ������
//--------------------------------------------------------------------------
char*	const	DF_TEXT_SERVICE_NATION_LIST[ELANGAGECODE_MAX]	= { "None", "Korea", "Taiwan", "Japan", "Hongkong", "China", "Indonesia" };

//--------------------------------------------------------------------------
// ����׿� ���
//--------------------------------------------------------------------------
char			DF_PATH_DEBUG_OUTSIDE_HQTEXT[MAX_PATH]	= {0,};	// (����׿�) �ܺ� HQ Text �������

//------------------------------------
// �׷��ȿ� ���õ� Class ���� 
//------------------------------------
extern cltGrp                clGrp;               // �׷��ȿ� ���õ� Class ���� 

//--------------------------------------------------------------------------
// ���� ������ 
//--------------------------------------------------------------------------
_clGame *pGame;


XSPR TileSpr[MAX_MAP_TILE_SPR_NUMBER];						// ���� ��Ʈ�� Ÿ�� 
XSPR TitleSpr;												// Ÿ��Ʋ ��� �׸��� ���� ����  
XSPR PannelSpr;

//------------------------------------
// ��Ʈ��ũ ����  
//------------------------------------

OnlineBattleInfo OBInfo;

int     MultiPlayDelayTime		= DEFAULT_MULTI_PLAY_DELAY_TIME;      // MultiPlay Delay�⺻ �ð� 
DWORD	PrevMultiClock;
DWORD	MessageSendFrame		= 0;  // ����̳� NO_MESSAGE�� ���� ������ 
BOOL	GameDropSwitch			= FALSE;
_MyNetGame MyNet;



//------------------------------------
// ȭ����� 
//------------------------------------
// actdoll (2005/01/10 23:08) : ���� ��� ���� ����
//	���� �÷��� ������ �̿��Ͽ� ���� ��带 �����Ѵ�.
//#ifdef _WINDOWED
//SI32  WindowModeSwitch = TRUE;
//#else
//SI32  WindowModeSwitch = FALSE;
//#endif
extern int WindowModeSwitch;
SI32				WindowModeSwitch	= FALSE;

_ScreenPoint        SP;									// ȭ�鿡 ���� ���� ���� 
int					bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
BOOL				g_bMainInit;						// ���� �ʱ�ȭ ���� ��� ������ ��Ƽ�갡 �ǵ���
_clCapture			Capture;							// ȭ�� ĸ�� ���� Ŭ���� 

UI32  GameScreenMode = MODE_GRAPHICS800_600;

DWORD LastDrawClock;

//------------------------------------
// ���� ���� ������ 
//------------------------------------
SI32	UserPlayerNumber;
SHORT	GameLeftPressSwitch = FALSE;
SHORT	GameRightPressSwitch= FALSE;
SHORT	GameMouseX=10;
SHORT	GameMouseY=10;
SHORT	GameRealX;
SHORT	GameRealY;
DWORD	GameWParam;

DWORD CaptureClock ;      // ������ ĸ���� �ð�. 

// ���� ���� ���� 
SHORT PointNumber;
SHORT PointArray[200];

//------------------------------------
// ���� ���� ������ ���� ������ 
//------------------------------------
_Item    Item[MAX_ITEM_NUMBER];
_Order   Order[MAX_ORDER_NUMBER];
_MapEdit MapEdit;

_Help					Help;
_Chat					Chat;                           // �Է°���
_Cheat					Cheat;
_FindPathGame		   *FindPathGame;

//------------------------------------
// ���� ���۸�� 
//------------------------------------
#ifdef _MAPEDIT
SHORT					MapEditMode          = TRUE;				// ���� ���� ���(TRUE : ���� ������, FALSE : ����)
#else
SHORT					MapEditMode          = FALSE;				// ���� ���� ���(TRUE : ���� ������, FALSE : ����)
#endif

//------------------------------------
// ���� ���� ���� 
//------------------------------------
#ifdef _MAPEDIT
SHORT					GameStatus				= GS_INITONLINEBATTLEGAME; // ������ ���� ��Ȳ�� �˸��� ���� 
#else
SHORT					GameStatus				= GS_INITONLINEWORLD; // ������ ���� ��Ȳ�� �˸��� ���� 
#endif

SHORT					GameStatusBuffer;
SHORT					GamePauseSwitch			= FALSE;          // ������ ��� �ߴܵǾ��°�?
SHORT					GameMenuShowSwitch		= FALSE;          // �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
SHORT					MessageMode				= MESSAGEMODE_ALL;// �޽����� �������� ���°�?


// ���ӿ��� ���Ǵ� ���� 
RECT					ScreenRect;									// ������ ǥ�õǴ� ���� 
_Text					Text;										// �ؽ�Ʈ Ŭ���� ���� 
_FileVary				FV;											// ���Ǵ� ������ ������ �����Ѵ�. 
_KindInfo				KI[KIND_ON_MAX];					// ĳ���� ������ ������ 
CCharFileLoadManager	g_FileLoadManager;							// ĳ���� ������
CItem					*g_pItem = NULL;							// ������ Ŭ����


_NewID					ZeroID;


//------------------------------------
// ��� ���庯�� 
//------------------------------------
char					SavePath[256];								// ���� ������ ��ġ�� ��� 
char					CurrentDirectory[256];

//------------------------------------
// �޴� ���� 
//------------------------------------
int   MenuMode;											// �޴��� ���¸� ���� 
SHORT MenuOfMouse;										// ���콺 Ŀ���� ��ġ�ϰ� �ִ� �޴��� ��ȣ 
SHORT CrewMenuOfMouse;

BOOL IsMosueInControlSwitch;							// ���콺�� ���� ��Ʈ������ ��ġ�ϰ� �ִ�.
BOOL bGameAllyWindowSwitch;


//------------------------------------
// ����� ���� Ű����� ���콺 
//------------------------------------
int    MouseMode;										// ���콺�� ó�� ���¸� ���� 
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
// ����� ���� ���� ���� ������ 
//------------------------------------
int   MapDragSwitch        = FALSE;
int   MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
SHORT MapX, MapY;                    // ���콺�� ��ġ�� small ��ǥ 
SHORT MouseHarvestX, MouseHarvestY;
SHORT UnderMouseCropType;
SHORT UnderMouseCropMount;

//------------------------------------
// ���ӿ� ���Ǵ� �ð躯�� 
//------------------------------------
clock_t StartClock, CurrentClock, PrevClock;
clock_t ClickClock;

clock_t MonitorMacroClock1, MonitorMacroClock2;		// ��ũ�� �˻� Process ���� �ֱ�

struct tm       *pNewTime;

//------------------------------------
// ���ӿ� ���Ǵ� Block������ ��Ÿ����
//------------------------------------
SHORT BasicBlock=BLOCK_CHAR|BLOCK_DEEPWATER|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_RUIN|BLOCK_ICEWALL;
SHORT WaterBlock=BLOCK_CHAR|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_LAND|BLOCK_SHALLOWWATER|BLOCK_ICEWALL;


int   FlyVibrateData[MAX_FLY_VIBRATE]={0, 1, 2, 3, 4, 3, 2, 1};

//=====================================================================
// �����ϱ� ���� ���� 
//=====================================================================

// ������ �� ��ǥ������ �߽ɰ�, 
int MapCenterX[MAX_MAP_XSIZE][MAX_MAP_YSIZE];  
int MapCenterY[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
// ���� ��ǥ�� ������ �⺻ ��ǥ�� ���Ѵ�. 
unsigned char MapByRealX[(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];
unsigned char MapByRealY[(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];

SI32 MaxMapXsize = 180;
SI32 MaxMapYsize = 180;

_Map    Map;                       // ������ ������  
_YSMap  SMap;                      // ���� ������ ������   
_Char   CR[MAX_CHAR_NUMBER];       // ĳ������ ������ 
cltCharInterface clifChar;         // ĳ���� ���� �������̽� 
cltMapInterface clifMap;           // ��     ���� �������̽� 

_MainData GD;                      // ������ �ֵ����� 


//HQFileManager	HQFile;
char			LogInID[128]		= {0,};

char			g_AutoID[256];
char			g_AutoIDPW[256];
SI16			g_AutoServerCode = -1;


//----------------------------------------------------------------------------
// actdoll (2004/10/05 9:10) : Ŭ���̾�Ʈ �õ� �� �۵� �÷���
//----------------------------------------------------------------------------
BOOL			g_fTestVersion			= FALSE;		// �׼����ΰ� �ƴѰ�?
BOOL			g_bSkipIntro			= FALSE;		// ��Ʈ�� ��ŵ����?

// Ŀ�ǵ� ���� �ɼ�
const	DWORD	DF_FLAG_CMDLINE_ACCESS	= 0x10000000;	// Ŀ�ǵ� ���� ��� : ���� (�� ���� ���� ��� Ŭ���̾�Ʈ�� �������� �ʴ´�.)
const	DWORD	DF_FLAG_CMDLINE_IP		= 0x00000001;	// Ŀ�ǵ� ���� ��� : ����Ʈ ���� IP ���� (��ġ����)
const	DWORD	DF_FLAG_CMDLINE_NT		= 0x00000002;	// Ŀ�ǵ� ���� ��� : ���� ������ ���� (��ġ����)
const	DWORD	DF_FLAG_CMDLINE_ST		= 0x00000004;	// Ŀ�ǵ� ���� ��� : ���� Ÿ�԰� ���� (��ġ����)
const	DWORD	DF_FLAG_CMDLINE_RA		= 0x00000008;	// Ŀ�ǵ� ���� ��� : Ŭ���̾�Ʈ�� �����ϰ� ��ó�� �ٽ��ѹ� ������Ѷ� (��ġ����)

const	DWORD	DF_FLAG_CMDLINE_SKIPGG	= 0x00001000;	// Ŀ�ǵ� ���� ��� : ���Ӱ��� ��ŵ
//----------------------------------------------------------------------------


char			REG_PATH[256] = "";

//=====================================================================



//--------------------------------------------------------------------------
// Main.cpp �� Methods
//--------------------------------------------------------------------------
extern HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

// actdoll (2004/01/16 0:18) : HQ üũ�� ��ġ �� GCS ����
const	unsigned __int64	DF_VALUE_CS		= 0x2F360C5A2964A4E7;	// üũ�� �񱳰�			(Debug�� �ߴ� HQ���� �˻� ���� ���� ����� �����Ű�� �˴ϴ�.)
const	UI32				DF_VALUE_CSF	= 0x1c437f64;			// üũ�� �÷���			(�� ���� �÷����Դϴ�. �� ���� �ٲ������� DF_VALUE_CS�� ��ü�� �ٲ�ϴ�.)
const	DWORD				DF_VALUE_ATI	= 0x80d36cfa;			// ��� �ɼ� �÷����� �����ڵ�(��� �� ���̽��� �� ���� �����)
const	char*				DF_VALUE_AAC	= "ilovegs";			// ��ڰ� �Է��ϴ� �����ڵ� �÷��װ�

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
// Desc: ������ ���ι�. ��� ���α׷��� ���⼭ ���۵ȴ�!
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// actdoll (2004/03/12 2:45) : 
	//	�Ż� Ŭ���̾�Ʈ�� � �������� ���̴� ������ üũ
	DWORD	dwRetOrder;

	GSMemoryInit(GS_DEBUG_WINDOWS);
	GSMemorySetLevel(2);
	XDebugInit(XDOT_DEBUG);
	
	// ������Ʈ ���� Ȯ�� �� ���� >> ���� ����, ���� ����, GCS ����, ����� ���� �ľ� ��
	if( !Init_CheckUpdates( hInstance ) )		return 0;

	// Ŀ�ǵ� ���� Ȯ��
	dwRetOrder	= Init_CheckAdOpt( lpCmdLine, nCmdShow );
	dwRetOrder	= Init_CheckCmdLines( lpCmdLine, nCmdShow, dwRetOrder );

	
	// Ŭ���̾�Ʈ �⺻ ���� Ȯ�� >> HQ�ε�, �系/�׼�/����/�����ڵ� �Ǻ�, ���Ŀ� ���� ���� �ɼ� �Ǻ�
	if( !Init_CheckBasics( hInstance, dwRetOrder ) )	
	{	
		BindJXFile::GetInstance()->destroy();
		return 0;
	}

	// ���� ���� >> _clGame ���� �� �ʱ�ȭ
	if( !Init_Engines( hInstance, nCmdShow, dwRetOrder ) )
	{
		BindJXFile::GetInstance()->destroy();
		return 0;
	}

	// ====================		!!!���� ����!!!		==================== //
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
	// ====================		!!!���� ����!!!		==================== //

	// ���ҽ� ����
	Kill();
	
	GSShowMemoryStatus();
//	showMemStats();

	return msg.wParam;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckUpdates()
// Code: actdoll (2004-03-12)
// Desc: ���α׷� ���� �� ������ ������Ʈ �ؾ� �� �������� Ȯ���Ѵ�.
//-----------------------------------------------------------------------------
BOOL	Init_CheckUpdates( HINSTANCE hInstance )
{
	// ���� �⺻���� �ʱ�ȭ
	ZeroMemory( &g_stConnectServerInfo, sizeof(g_stConnectServerInfo) );

	//----------------------------------------------------------------------------
	//	1�ܰ� - ���� ����
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
	//	2�ܰ� - ���� ����
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
	//	3�ܰ� - HQ ������ üũ �� �ε�
	//----------------------------------------------------------------------------
	if( !DataWork( hInstance ) )
	{
		MessageBox( NULL, DF_ERROR_WARNING_FOR_CRACK, "Gersang", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	4�ܰ� - ���� ����, ���� Ÿ���� Ȯ��
	//----------------------------------------------------------------------------
	// ���� ������ Ȯ���Ѵ�.
	int		iNation = 0, iType = 0;
	if( !GetNTN( "Online\\ntn.con", iNation, iType ) )
	{
		// ���� ���� ���Ͽ� ������ ����ٸ� ���ο� ���� ������ ����� �޽��� �ڽ� ����.
		CreateNTN( "Online\\ntn.con", iNation, iType );
		ShowErrorMsgBox( 10001001 );
//		MessageBox( NULL, DF_ERROR_CREATE_STANDARD_NTN, "ERROR(64dfacc4)", MB_OK );
	}


	//----------------------------------------------------------------------------
	//	5�ܰ� - �⺻ ������ ���� üũ
	//----------------------------------------------------------------------------
	// ������ �ִ��� �������� Ȯ���Ѵ�
	if( !SetLocaleInfo( iNation, iType ) )
	{
		BindJXFile::GetInstance()->destroy();
		ShowErrorMsgBox( 10001002 );
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_NATION_LIST, "039addeb", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	6�ܰ� - GCS ����(�̰� GCS�� ���Ե� �ѱ� �׼��� �ȴ�)
	//----------------------------------------------------------------------------
	SYSTEMTIME		sysTime;
	char			pszCrashText[1024], pszCrashTime[1024];
	GetSystemTime( &sysTime );
	sprintf( pszCrashText, "!!CriticalCrash!!_V%d", ON_CLINETVERSION );
	sprintf( pszCrashTime, "StartTime- %d/%d/%d %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	InstallCrashHandler( pszCrashText, pszCrashTime, "GERSANG", CRASH_CLIENT );				// ũ���� �ڵ鷯 ��ġ
	SetNationCode( DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );	// ũ���� ���� �ڵ� ��ġ


	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckAdOpt()
// Code: actdoll (2004-03-12)
// Desc: ���� �ɼ��� üũ�Ѵ�.
//		return		- ���� �����̶�� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
DWORD	Init_CheckAdOpt( LPSTR lpCmdLine, int nCmdShow )
{
	// ���ؽ��� �����Ͽ� ���� ���� ���������� �ƴ��� Ȯ��
	HANDLE	hMutex	= CreateMutex( NULL, TRUE, "gs041216cat" );
	DWORD	dwRet	= GetLastError();
	ReleaseMutex( hMutex );
	CloseHandle( hMutex );

	// �� ��Ȳ�̶�� �������� �۵� ���°� �ƴϴ�.
	if( !hMutex || dwRet != ERROR_ALREADY_EXISTS )		return 0;

	// �ϴ� ������� ���� ������ �ɷ����� ����
	BOOL	bFound = false;
	char	*pRealData, *pszToken, pszData[2048] = {0,}, pszAAC[128] = {0,};

	strcpy( pszData, lpCmdLine );
	pszToken = strtok( pszData, "- \t\n\r" );
	while( pszToken )
	{
		// ��� �÷��� �����͸� ã�Ƴ���.
		if( ( pRealData = strstr( pszToken, "AO:" ) ) != NULL )
		{
			bFound	= true;
			pRealData	+= 3;	// ���� ������ �ʹ����� �̵�
			break;
		}
		pszToken	= strtok( NULL, "- \t\n\r" );
	}

	// �÷��׷� ã�� ���ߴٸ� ���Ϸ� ã�ƺ���.
	if( !bFound )
	{
		char	pszPath[MAX_PATH];
		GetWindowsDirectory( pszPath, MAX_PATH );
		strcat( pszPath, "\\Ffgsdeac.cks" );
		
		// ���� ��ü�� ���ٸ� ������
		FILE	*fp = fopen( pszPath, "rb" );
		if( !fp )	return 0;

		// �ϴ� ũ�� �ް�
		fseek( fp, 0, SEEK_END );
		DWORD	dwLen = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		// ������ ���ۿ� ����
		fread( pszData, dwLen, sizeof(BYTE), fp );
		pRealData	= pszData;

		fclose( fp );
	}

struct ST_ADFLAG
{
	UINT	m_uiNTN;				// ��� ���� NTN
	
	BOOL	m_bSkipLauncher;		// ���� ��ŵ ����
	BOOL	m_bSkipIntroScreen;		// ��Ʈ�� ��ŵ ����
	BOOL	m_bSkipGameGuard;		// ���Ӱ��� ��ŵ ����
	BOOL	m_bRunAsWindowMode;		// �������� ���� ����

	char	m_szVersionServerIP[64];		// �������� IP ��Ʈ���� ����ִ� ������
	char	m_szFrontServerIP[64];			// ����Ʈ���� IP ��Ʈ���� ����ִ� ������
	char	m_szPathCustomHQ[MAX_PATH];		// ����Ʈ���� IP ��Ʈ���� ����ִ� ������
}	stAdmin = {0,};

	// �˻� ����.
	UINT	uiRet = GetAdDecoData( (BYTE*)pRealData, strlen( pRealData ), (DWORD&)stAdmin.m_uiNTN, 
									stAdmin.m_bSkipGameGuard, stAdmin.m_bSkipLauncher, stAdmin.m_bSkipIntroScreen, stAdmin.m_bRunAsWindowMode,
									pszAAC, stAdmin.m_szVersionServerIP, stAdmin.m_szFrontServerIP, stAdmin.m_szPathCustomHQ );
	if( stricmp( pszAAC, DF_VALUE_AAC ) != 0 || uiRet == 0 )		
	{
		ZeroMemory( &stAdmin, sizeof(stAdmin) );
		return 0;
	}

	// �����̴�. ���� Ȯ��
	dwRet	= 0;
	if( strlen(stAdmin.m_szFrontServerIP) > 1 )	{ dwRet |= DF_FLAG_CMDLINE_IP;	strcpy( g_stConnectServerInfo.m_FrontServerIP, stAdmin.m_szFrontServerIP ); }	// ����Ʈ���� IP����
	if( strlen(stAdmin.m_szPathCustomHQ) > 1 )	{ strcpy( DF_PATH_DEBUG_OUTSIDE_HQTEXT, stAdmin.m_szPathCustomHQ ); }											// ���� HQ ���
	if( stAdmin.m_uiNTN != 0 )					{ dwRet	|= DF_FLAG_CMDLINE_NT | DF_FLAG_CMDLINE_ST;	g_stConnectServerInfo.m_iServiceNation = stAdmin.m_uiNTN; g_stConnectServerInfo.m_iServiceType = 1; }	// NTN ����
	if( stAdmin.m_bSkipGameGuard )				{ dwRet	|= DF_FLAG_CMDLINE_SKIPGG; }	// ���Ӱ��� ��ŵ����
	if( stAdmin.m_bSkipLauncher )				{ dwRet	|= DF_FLAG_CMDLINE_ACCESS; }	// ���� ��ŵ ����
	g_bSkipIntro		= stAdmin.m_bSkipIntroScreen;									// ��Ʈ�� ��ŵ ����
	WindowModeSwitch	= stAdmin.m_bRunAsWindowMode;									// ������� ��������

	// Ȯ�λ��
	ZeroMemory( &stAdmin, sizeof(stAdmin) );
	MessageBox( NULL, "Administrator Option was initialized!", "Gersang", MB_OK|MB_ICONINFORMATION );
	// actdoll (2005/01/05 18:26) : ���������� ���⼭ ��� Ȯ���� ������ ��� ũ���� ������ �����Ѵ�.
	SetAppType( CRASH_CLIENT_ADMIN );

	return dwRet;
}


//-----------------------------------------------------------------------------
// Name: Init_CheckCmdLines()
// Code: actdoll (2004-03-12)
// Desc: Ŀ�ǵ� ������ ������ �о�´�.
//		return		- ������ ���´ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
DWORD	Init_CheckCmdLines( LPSTR lpCmdLine, int nCmdShow, DWORD dwOrder )
{
	char	*pTemp, *pOrder, pszCmdLine[2048] = {0,};


	//----------------------------------------------------------------------------
	//	1�ܰ� - Ŭ���̾�Ʈ ���� ���� �÷��� Ȯ��
	//----------------------------------------------------------------------------
	strcpy( pszCmdLine, lpCmdLine );
	pOrder = strtok( pszCmdLine, "- \t\n\r" );
	while( pOrder )
	{
		// �ؿܲ� ����ؼ� �̹��� ������Ų��.
		if( stricmp( pOrder, "LF" ) == 0 )
		{
			dwOrder |= DF_FLAG_CMDLINE_ACCESS;
			break;
		}
		// ��ó���� �����Ǿ���.	(Launcher Start)
//			if( stricmp( pOrder, "LS" ) == 0 )
//			{
//				dwOrder |= DF_FLAG_CMDLINE_ACCESS;
//				break;
//			}
//			// �������� �����Ǿ���.	(Manual Start)
//			if( ( pTemp = strstr( pOrder, "MS:" ) ) != NULL )
//			{
//				pTemp += 3;
//				if( strlen( pTemp ) )	// ��ġ��
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


	// ���� ����� ���� �÷��װ� ���ٸ� �ϴ� ����
	if( !(dwOrder & DF_FLAG_CMDLINE_ACCESS) )		return 0;


	//----------------------------------------------------------------------------
	//	2�ܰ� - ��Ÿ ��ɵ� Ȯ��
	//----------------------------------------------------------------------------
	strcpy( pszCmdLine, lpCmdLine );
	pOrder = strtok( pszCmdLine, "- \t\n\r" );
	while( pOrder )
	{
		// ����Ʈ ���� IP.		(frontserver IP)
		if( !( dwOrder & DF_FLAG_CMDLINE_IP ) &&	( pTemp = strstr( pOrder, "IP:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// IP �ּ� ����
			{
				strcpy( g_stConnectServerInfo.m_FrontServerIP, pTemp );
				dwOrder |= DF_FLAG_CMDLINE_IP;
			}
		}
		// ���� ����.			(Nation Type)
		else if( !( dwOrder & DF_FLAG_CMDLINE_NT ) &&	( pTemp = strstr( pOrder, "NT:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// �������� ��ġ ����
			{
				g_stConnectServerInfo.m_iServiceNation	= atoi( pTemp );
				dwOrder |= DF_FLAG_CMDLINE_NT;
			}
		}
		// ���� ����.			(Service Type)
		else if( !( dwOrder & DF_FLAG_CMDLINE_ST ) &&	( pTemp = strstr( pOrder, "ST:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// �������� ��ġ ����
			{
				g_stConnectServerInfo.m_iServiceType	= atoi( pTemp );
				dwOrder |= DF_FLAG_CMDLINE_ST;
			}
		}
		// ��⵿ ��û.			(Retry Again)
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
// Desc: �ʱ⿡ �ε��Ǿ� �� �⺻ �������� ���캻��.
//		return		- ������ ���´ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	Init_CheckBasics( HINSTANCE hInstance, DWORD dwOrder )
{
	//----------------------------------------------------------------------------
	//	1�ܰ� - �⺻ ������ ���� ��˻�(������ ServiceNation, Type, FrontServerIP�� ����)
	//----------------------------------------------------------------------------
	// ��� ��ɵ ���� �۾��� �������� �� �����Ƿ� �ٽ� �ѹ� Ȯ���Ѵ�.
//	if( !(dwOrder & DF_FLAG_CMDLINE_NT) )	g_stConnectServerInfo.m_iServiceNation	= iNation;
//	if( !(dwOrder & DF_FLAG_CMDLINE_ST) )	g_stConnectServerInfo.m_iServiceType	= iType;
	if( !SetLocaleInfo( g_stConnectServerInfo.m_iServiceNation, g_stConnectServerInfo.m_iServiceType, g_stConnectServerInfo.m_FrontServerIP ) )
	{
		ShowErrorMsgBox( 10001002 );
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_NATION_LIST, "039addeb", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	2�ܰ� - Ŭ���̾�Ʈ ���� ���� Ȯ��
	//----------------------------------------------------------------------------
	// ���� ���� ���� �Ǵ�
	if( !(dwOrder & DF_FLAG_CMDLINE_ACCESS) )
	{
		char	pszPath[MAX_PATH];//pszTemp[1024], 
		GetCurrentDirectory( MAX_PATH, pszPath );
		ShowErrorMsgBox( 10001003, pszPath );
//		sprintf( pszTemp, DF_ERROR_CANNOT_EXEC_LAUNCHER, pszPath, pszPath );
//		MessageBox( NULL, pszTemp, "Error!", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
	// �簡�� ���� �Ǵ�
	if( dwOrder & DF_FLAG_CMDLINE_RA )
	{
		ShowErrorMsgBox( 10001004 );
//		MessageBox( NULL, DF_ERROR_UPDATE_VERSION_SERVER, "Notice!", MB_OK | MB_ICONWARNING );
		return FALSE;
	}


//	//----------------------------------------------------------------------------
//	//	3�ܰ� - ���õ� ����/����Ÿ�Կ� ���� ���� ���� �����ͺ��̽� ���� ���ϴ� ������ ����
//	//----------------------------------------------------------------------------
//	char	pszDummyIP[128]; 
//	
//	// ���� ��� �÷��׿� ���� �̹� ������ IP�� �ִٸ� �ϴ� ���
//	if( dwOrder & DF_FLAG_CMDLINE_IP )	strcpy( pszDummyIP, g_stConnectServerInfo.m_FrontServerIP );
//	
//	// HQ���� ���ϴ� ���������� ���� Ÿ���� �������� IP �� ���񽺻� �������� ��´�.
//	// �ܺ� HQ������ ���� ���
//	sprintf( pszNation, "%s\\ServiceListInfo.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
//	if( !ReadIPEx( pszNation, true, dwOrder & DF_FLAG_CMDLINE_NT, 
//					g_stConnectServerInfo.m_iServiceNation, g_stConnectServerInfo.m_iServiceType, &g_stConnectServerInfo ) )
//	{
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_ISI, "039addeb", MB_OK );
//		BindJXFile::GetInstance()->destroy();
//		return FALSE;
//	}
//	// �ش� ��� �÷��װ� ����ִٸ� �̺κи� ����
//	if( dwOrder & DF_FLAG_CMDLINE_IP )	strcpy( g_stConnectServerInfo.m_FrontServerIP, pszDummyIP );
//
//	//----------------------------------------------------------------------------
//	//	3�ܰ� - ����ó�� �÷��� ����
//	//----------------------------------------------------------------------------
//	sprintf( pszNation, "Nation:%d, Type:%d", iNation, iType );								// ũ���� ���� �ڵ� ����(����, Ÿ��, ���ν��࿩��)
//	SetNationCode( pszNation );																// ũ���� ���� �ڵ� ��ġ

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Init_Engines()
// Code: actdoll (2004-03-12)
// Desc: ���� ���� �� ���� ���� ������ �ε��Ѵ�.
//		return		- ������ ���´ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	Init_Engines( HINSTANCE hInstance, int nCmdShow, DWORD dwOrder )
{
	// �ʱ�ȭ ������ �ִ�. 
	g_bMainInit = TRUE;

	//----------------------------------------------------------------------------
	//	1�ܰ� - ���� ���� ���� [���� ���� ȭ�� ����] (���߿� �� �������� ���񽺵� ��� �� �κ��� �� ������ ����.)
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
	//	2�ܰ� - �Ż� ����Ŭ���� ����
	//----------------------------------------------------------------------------

	// ���� Ŭ���� ���� �� ���� ����
	pGame =	new _clGame;
	if(pGame == NULL)	
	{
		// actdoll (2004/03/11 11:39) : ���� �߻� �� ������ ���Ͻ� ���� �ڵ� �����ش�.
		ShowErrorMsgBox( 10001006 );
//		MessageBox( NULL, DF_ERROR_MEMORY_NOT_ENOUGH, "ERROR(5bdf448c)", MB_OK );
		return FALSE;
	}

	
	//----------------------------------------------------------------------------
	//	3�ܰ� - ���� ���� �� ���� �⺻ ���� ����
	//----------------------------------------------------------------------------

	pGame->m_iNation		= g_stConnectServerInfo.m_iServiceNation;
	pGame->m_iServiceType	= g_stConnectServerInfo.m_iServiceType;
	pGame->LanguageCode		= (ELanguageCode)g_stConnectServerInfo.m_iServiceLanguage;
	pGame->m_pszHQNation	= DF_TEXT_SERVICE_NATION_LIST[pGame->LanguageCode];
	pGame->m_uiCodePage		= g_stConnectServerInfo.m_iServiceCP;

	
	//----------------------------------------------------------------------------
	//	4�ܰ� - ���� �� DirectX ����
	//----------------------------------------------------------------------------

	if(!pGame->Init(hInstance, nCmdShow))
	{
		ShowErrorMsgBox( 10001007 );
//		MessageBox( NULL, DF_ERROR_DXINIT_IS_FAILED, "ERROR(8e756bc4)", MB_OK );
		delete pGame;
		return FALSE;
	}
	SetHwndToGameGuard( pGame->Hwnd );	// actdoll (2004/10/19 11:44) : ���Ӱ��忡 ������ ���� �ڵ��� �̺κп� ����
	
	// �ʱ�ȭ�� ����Ǿ���. 
	g_bMainInit = FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Kill()
// Code: actdoll (2004-03-12)
// Desc: ���ο��� �ε��� ��� �����͵��� ����
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
	//	���Ӱ��� ���� 
	ReleaseGameGuard();
}



//-----------------------------------------------------------------------------
// Name: FCS()
// Code: actdoll (2003-12-30)
// Desc: ���� üũ���� ó���ϴ� �Լ���.
//		������ üũ���� ������ ����̴�. byte, word, dword ������ ������ ��ü, ġȯ, �ջ� � ����
//		ũ��ŷ�� ö���� ������ �� �ִ� ����̴�. �ٸ� �Լ� �Һ�ð��� �� ���.
//		(CPU 2.8���� ���15~17ms)
//
//		pFileName		- üũ���� �ϰ� ���� ���ϸ�
//		bAdminOption	- �����ڿ� �����Ͱ� ���� ��
//		return			- üũ�� �����(64bit)
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

	// ���� ����
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

		length		= ( uiFileSize >> 2 ) << 1;			// ���� ����� ���̸� ���Ѵ�.
		remainder	= ( uiFileSize - ( length << 1 ) );	// ������ ����Ʈ�� ����س��.

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
// Desc: HQ���ϵ��� �ε��� ���õ� �۾��� ó���ϴ� �Լ���.
//		return		- true��� HQ�� ���������� Ȯ��, false��� HQ�۾� ����
//-----------------------------------------------------------------------------
BOOL	DataWork( HINSTANCE hInstance )
{
	// �� ������ HQ���� �˻�
	char	*pszHQFile = ".\\hq";

	// actdoll (2004/01/15 23:55) : �߿��� hq������ ������ ��ŷ���� ���ϵ��� üũ���� �ɾ���.
	//		Debug ��忡���� ǥ�õ� ��ġ�� DF_VALUE_CS ���� �����Ͽ� �������ϸ� �Ǹ� �׳� ������ �� ������
	//		Release ��忡���� ��ġ�� ���� ���� ��� �׳� ����������.
	//		���� �ű��ϰԵ�(?) �� hq������ üũ�� �� �ٸ� �Ż� ���α׷��� ���ư� �� hq ������ ���� ����
	//		���߽����� ���� �ʴ´�. �Ƹ��� ���� File ���� API�� ���� Ȥ�� Ư���� �� �ϴ�.

	// actdoll (2004/05/27 18:54) : V.12250���� ��� ������ ������� hq�� �̸��� �����Ѵ�.
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
		ShowDlg( hInstance, "ũ��ŷ ������ �G�� ����", pTemp );
#else
		return FALSE;
#endif
	}

	// �� �κ��� �Ϲ� ��ڰ� �׽�Ʈ�� ���� ���� �ɼ��� ������ ��� ����� �ø��� Ű�� �����Ѵ�.
//#ifdef _DEBUG
//	sprintf( pTemp, "[ Administrator Authorization Code ]\n\n\n%I64d", FCS( pszHQFile, true ) );
//	MessageBox( NULL, pTemp, "AAC Notice - Only Debug", MB_OK );
//	ShowDlg( hInstance, "������ ���� �ڵ� (AAC)", (char*)DF_VALUE_AAC );
//#endif

	return ( BindJXFile::GetInstance()->openJX( pszHQFile ) );
}

//-----------------------------------------------------------------------------
// Name: GetNTN()
// Code: actdoll (2004-03-12)
// Desc: ���� ���� �� ���� ���� ������ üũ�Ѵ�.
//		pszFileName	- ntn���ϸ�
//		iNation		- ���� ���� �ڵ�, ������� -1 ����
//		iType		- ���� Ÿ��, ������� -1 ����
//		return		- true��� ���������� �����͸� Ȯ��, false��� ������ ���ų� ���� ���� ����
//-----------------------------------------------------------------------------
BOOL	GetNTN( char *pszFileName, int &iNation, int &iType )
{
	FILE	*pFile;

	iNation = iType = -1;

	pFile	= fopen( pszFileName, "rb" );
	if( !pFile )	return FALSE;

	// ������ ���� ������ ���ɴ�.
	DWORD	dwNTNVersion, dwNation, dwType;
	
	// ���� ���� �б�
	fread( &dwNTNVersion, sizeof(DWORD), 1, pFile );
	if( dwNTNVersion == ON_NTNFILE_VERSION )
	{
		fread( &dwNation,	sizeof(DWORD), 1, pFile );			// ���� ���� ��ȣ
		fread( &dwType,		sizeof(DWORD), 1, pFile );			// ���� Ÿ��
	}
	else
	{
		dwNation = dwType = 0xffffffff;
	}
	fclose( pFile );

	// ���� - ���� ��ȣ
	switch( dwNation )
	{
		case DF_CODE_NATION_IDENTITY_KOREA			:	// �ѱ�
		case DF_CODE_NATION_IDENTITY_JAPAN			:	// �Ϻ�
		case DF_CODE_NATION_IDENTITY_CHINA			:	// �߱�
		case DF_CODE_NATION_IDENTITY_TAIWAN			:	// �븸
		case DF_CODE_NATION_IDENTITY_HONGKONG		:	// ȫ��
		case DF_CODE_NATION_IDENTITY_INDONESIA		:	// �ε��׽þ�
				iNation		= (int)dwNation;
		default			:
			break;
	}

	// ���� - Ÿ��
	iType	= (int)dwType;
	if( dwNation == DF_CODE_NATION_IDENTITY_KOREA && iType == 0 )	g_fTestVersion	= true;		// �ѱ��ǿ� Ÿ���� 0�� ��� ���� �׼�
	else															g_fTestVersion	= false;	// �ƴ� ��� �׼��� �ƴϴ�.

	if( iNation == -1 || iType == -1 )		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: CreateNTN()
// Code: actdoll (2004-03-12)
// Desc: ���� ���� �� ���� ���� ������ �����Ѵ�.
//		pszFileName	- ntn���ϸ�
//		iVersion	- ����� ����
//		iNation		- ���� ���� �ڵ�
//		iType		- ���� Ÿ��
//		return		- true��� ���������� �����͸� Ȯ��, false��� ������ ���ų� ���� ���� ����
//-----------------------------------------------------------------------------
BOOL	CreateNTN( char *pszFileName, int &iNation, int &iType )
{
	FILE	*pFile;
	char	pszTemp[12] = {0,};

	// Ÿ���� �����ش�.
	if( iType == -1 )	iType = 1;

	// ���� �ý����� ���� ������ ������ �� ���ٸ� OS�⺻ ������ �����ϰ� �����̴�.
	if( iNation == -1 )
	{
		GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTCOUNTRY, pszTemp, sizeof(pszTemp) );
		iNation	= atoi( pszTemp );
		return FALSE;
	}

	// ����� Ȯ�εƴٸ� ���� ���� ����
	pFile	= fopen( pszFileName, "wb" );
	if( !pFile )	return FALSE;

	fwrite( &ON_NTNFILE_VERSION,	sizeof(DWORD), 1, pFile );			// NTN ���� ����
	fwrite( &iNation,				sizeof(DWORD), 1, pFile );			// ���� ���� ��ȣ
	fwrite( &iType,					sizeof(DWORD), 1, pFile );			// ���� Ÿ��

	fclose( pFile );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetLocaleInfo()
// Code: actdoll (2004-03-12)
// Desc: ���� �������� �޾Ƽ� ������ ������ ������ �����ش�.
//		iServiceNation	- ���� ����� ����(��ȭ)��ȣ
//		return			- true��� ���������� �����͸� Ȯ��, false��� ������ ���� �ʴ� ����(��ȭ)��ȣ�̴�.
//-----------------------------------------------------------------------------
BOOL	SetLocaleInfo( int iServiceNation, int iServiceType, char *pszCustomFrontServerIP )
{
	// �������� �ƴ϶�� ����
	if( iServiceNation > 0 )		g_stConnectServerInfo.m_iServiceNation	= iServiceNation;
	if( iServiceType >= 0 )			g_stConnectServerInfo.m_iServiceType	= iServiceType;

	// �������� �������� ���� ��� �ڵ带 �������ش�.
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

	// �־��� ���� ���������� �������� ������ �����͸� �̾Ƴ���.
	FILE*	fp;
	OnlineCommonParser	ocp;
	char	pszFileName[MAX_PATH];

	sprintf( pszFileName, "%s\\ServiceListInfo.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );
	if( !fp )	return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "ServiceListInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	BOOL	bRet	= false;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		char	pszDesc[48], pszTemp[128];
		SI32	siIndex, siNation, siType;
		ocp.GetValue( siIndex,			iLineNo, iColNo );		// ��ȣ
		ocp.GetValue( pszDesc,	47,		iLineNo, iColNo );		// ����
		ocp.GetValue( siNation,			iLineNo, iColNo );		// ���� ���� ��ȣ
		ocp.GetValue( siType,			iLineNo, iColNo );		// ���� Ÿ�� ��ȣ

		// �ϴ� ������� ã�� ������ ��ġ���� ������ ����
		if( siNation != iServiceNation || siType != iServiceType )	continue;
		bRet	= true;

		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ���� ���� IP
		// ���� ���⼭ �缳 IP�� �ִٸ� �� ������ ����
		if( pszCustomFrontServerIP )	
		{
			strcpy( g_stConnectServerInfo.m_FrontServerIP, pszCustomFrontServerIP );
			ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ����Ʈ ���� IP
		}
		else
		{
			ocp.GetValue( g_stConnectServerInfo.m_FrontServerIP,			127,	iLineNo, iColNo );		// ����Ʈ ���� IP
		}
		ocp.GetValue( g_stConnectServerInfo.m_ServiceName,				127,	iLineNo, iColNo );		// ���� ��ü��
		ocp.GetValue( g_stConnectServerInfo.m_ServiceURL,				511,	iLineNo, iColNo );		// ���� ��ü URL
		ocp.GetValue( g_stConnectServerInfo.m_ServiceCreateAccountURL,	511,	iLineNo, iColNo );		// ��� ������ URL
		ocp.GetValue( g_stConnectServerInfo.m_ServiceShopName,			127,	iLineNo, iColNo );		// ���θ� ��
		ocp.GetValue( g_stConnectServerInfo.m_ServiceShopURL,			511,	iLineNo, iColNo );		// ���θ� ��ü URL
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ��Ÿ
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ��Ÿ URL

		break;
	}

	fclose( fp );
	ocp.Release();

	return bRet;
}

//-----------------------------------------------------------------------------
// Name: ReadIPEx()
// Code: actdoll (2004-06-01)
// Desc: ������ ���� �����ͺ��̽� ������ �м��ϰ� �ش� ������ ������ ����.
//		���� ���� �系�׼������� ���� �ִٸ� ������ ù°�ٸ� �д´�.
//		lpFileName				- ServiceInfo.cfg ���ϸ�
//		bUseHQ					- HQ���� ������ ���� ��� true, �Ϲ� ������ ���� ��� false
//		bJustReadFirstLine		- �׽�Ʈ�� ���Ǵ� �÷���. true �� ��� �ش� ���������� ù��° ���ΰ��� �����ͷ� �ѱ��. 
//								false�� ��� iNation, iType�� ��ġ�ϴ� �����͸� ã�� �����Ѵ�.
//		iNation					- ���� ���� �ڵ�
//		iType					- ���� Ÿ��
//		lpSConnectServerInfo	- ���ǿ� �´� ���� ������ �߰����� �� ����� ����
//		return					- ����� ã�Ƴ´ٸ� true, ������ �ִٸ� false
//-----------------------------------------------------------------------------
BOOL	ReadIPEx( char* lpFileName, BOOL bUseHQ, BOOL bJustReadFirstLine, int iNation, int iType, SConnectServerInfo* lpSConnectServerInfo )
{
	FILE*	fp;
	OnlineCommonParser	ocp;

	if( !lpFileName || !lpSConnectServerInfo )	return FALSE;

	// HQ���� ������ ����.
	if( bUseHQ )
	{
		fp = BindJXFile::GetInstance()->GetFile( lpFileName );
		if( !fp )	return FALSE;
	}
	// �Ϲ� ������ ����.
	else
	{
		fp = fopen( lpFileName, "rb" );
		if( !fp )	return FALSE;
	}

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "ServiceInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		char	pszDesc[48], pszTemp[128];
		SI32	siIndex, siServiceNation, siServiceType;
		ocp.GetValue( siIndex,			iLineNo, iColNo );		// ��ȣ
		ocp.GetValue( pszDesc,	47,		iLineNo, iColNo );		// ����
		ocp.GetValue( siServiceNation,	iLineNo, iColNo );		// ���� ���� ��ȣ
		ocp.GetValue( siServiceType,	iLineNo, iColNo );		// ���� Ÿ�� ��ȣ

		//----------------------------------------------------------------------------
		// �ϴ� ������� ã�� ������ ��ġ���� ������ ����, ù���� ���� �����϶�� ���� �ƴϸ� ��� ��ȯ
		if( !bJustReadFirstLine )
		{
			if( iNation != siServiceNation || iType != siServiceType )	continue;
		}
		// �Ѿ�Դٸ� ã�� ���̴�.
		lpSConnectServerInfo->m_iServiceNation		= siServiceNation;
		lpSConnectServerInfo->m_iServiceType		= siServiceType;
		//----------------------------------------------------------------------------

		ocp.GetValue( lpSConnectServerInfo->m_FrontServerIP,			127,	iLineNo, iColNo );		// ���� ���� IP
		ocp.GetValue( lpSConnectServerInfo->m_FrontServerIP,			127,	iLineNo, iColNo );		// ����Ʈ ���� IP
		ocp.GetValue( lpSConnectServerInfo->m_ServiceName,				127,	iLineNo, iColNo );		// ���� ��ü��
		ocp.GetValue( lpSConnectServerInfo->m_ServiceURL,				511,	iLineNo, iColNo );		// ���� ��ü URL
		ocp.GetValue( lpSConnectServerInfo->m_ServiceCreateAccountURL,	511,	iLineNo, iColNo );		// ��� ������ URL
		ocp.GetValue( lpSConnectServerInfo->m_ServiceShopName,			127,	iLineNo, iColNo );		// ���θ� ��
		ocp.GetValue( lpSConnectServerInfo->m_ServiceShopURL,			511,	iLineNo, iColNo );		// ���θ� ��ü URL
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ��Ÿ
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ��Ÿ URL
		
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
// Desc: ������ ������ ��� ���̾�α׸� ǥ���Ѵ�.
//		hInstance	- �� Ŭ���̾�Ʈ�� �ν��Ͻ� �ڵ�
//		pszTitle	- �޽��� Ÿ��Ʋ
//		pszText		- ���� ������ ��� �޽���
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
// Desc: ��� ��ɾ� �����͸� �޾� �ص��Ͽ� �����Ѵ�.
//		pDest				- ���ڵ� ���� ���� ���� ������
//		uiDestBuffSize		- pDest ���� ũ��
//		dwNTNValue			- NTN �ڵ尪
//		bSkipGameGuard		- ���Ӱ��� ���� ���ϸ� true, �����̶�� false
//		bSkipLauncher		- ���ľ��� ����ȴٸ� true, ���� �ݵ�� ��ġ�� �Ѵٸ� false
//		bSkipIntro			- ��Ʈ�� ���� �ε��Ѵٸ� true, �ƴ϶�� false
//		pszAACCode			- AAC�ڵ尪
//		pszVersionServerIP	- ������ ���� ���� IP
//		pszFrontServerIP	- ������ ����Ʈ ���� IP
//		pszPathCustomHQ		- ������ �ܺ� HQText�� ����ִ� ������
//		return				- ���������� �۾��ߴٸ� pDest�� ����ִ� �������� byte��, ������ �ִٸ� 0
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

	// �� Ǯ��
	char		pszData[3];
	for( UINT i = 0, uiLen = 0; i < uiSrcBuffSize; i += 2, uiLen++ )
	{
		memcpy( pszData, pSrc + i, 2 );
		pszData[2] = 0;
		*( pbtDummy + uiLen ) = (BYTE)strtol( pszData, NULL, 16 );
	}

	// ���ڵ� �۾� ����
	DWORD	dwChkSum[8]	=
	{	
		0x8ac7d236,		0x135f47e6,		0xfcad6318,		0xa39e17bc,
		0xd5390148,		0x6e3f4d9a,		0x7efa143c,		0x957bd340,
	};

	// üũ�� �����Ͽ� �����Ѵ�.
	DWORD	*dwPointer	= (DWORD*)pbtDummy;
	for( i = 0; i < uiLen >> 2; i++ )
	{
		*dwPointer	^=	dwChkSum[i % 8];
		dwPointer++;
	}

	// Ȯ�λ��
	ZeroMemory( dwChkSum, sizeof(DWORD)*8 );

	uiLen		= 0;
	// NTN ����(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	dwNTNValue					= (DWORD)wTemp;
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// AAC�ڵ� ���� ����(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// AAC�ڵ� ���� ����(?B)
	memcpy( pszAACCode, pPointer, wTemp );
	pszAACCode[wTemp]			= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// ���Ӱ��� ��ŵ? ����(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipGameGuard				= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// �������� IP ���� ����(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// �������� IP ���� ����(?B)
	memcpy( pszVersionServerIP, pPointer, wTemp );
	pszVersionServerIP[wTemp]	= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;
	
	// ���� ��ŵ? ����(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipLauncher				= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// ����Ʈ���� IP ���� ����(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// ����Ʈ���� IP ���� ����(?B)
	memcpy( pszFrontServerIP, pPointer, wTemp );
	pszFrontServerIP[wTemp]		= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// ��Ʈ�� ��ŵ? ����(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipIntro					= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// HQ�ؽ�Ʈ ���� ����(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// HQ�ؽ�Ʈ IP ���� ����(?B)
	memcpy( pszPathCustomHQ, pPointer, wTemp );
	pszPathCustomHQ[wTemp]		= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// ������� �㰡? ����(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bRunAsWindowMode			= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// �÷��� ������ ����(4B)
	memcpy( &dwTemp, pPointer, sizeof(DWORD) );
	pPointer	+= sizeof(DWORD);
	uiLen		+= sizeof(DWORD);
	if( dwTemp != DF_VALUE_ATI )		// �÷��� �������� ���� �ʴٸ� �ױ⸮!
	{
		uiLen	= 0;
	}

	// �۾��� ������ �� ���� ����
	ZeroMemory( pbtDummy, 4096 );
	ZeroMemory( pszData, 3 );

	return uiLen;
}

//-----------------------------------------------------------------------------
// Name: ShowErrorMsgBox()
// Code: actdoll (2004-03-12)
// Desc: �� �������� ������ �� ��Ʈ���� ���� �޽����ڽ� ���·� ������ش�.
//		iErrorCode		- �����ڵ�
//-----------------------------------------------------------------------------
void	ShowErrorMsgBox( int iErrorCode, ... )
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	BOOL				bFind = false;
	char				pszFullErrorMsg[2048] = {0,}, pszDummy[1024] = {0,};

	// �ϴ� ���� �����ڵ� ���ۿ� �ڵ尪�� �־���´�.
	sprintf( pszFullErrorMsg, "Error Code - %08X\n\n", iErrorCode);

	// ������ �����Ѵٸ� HQ���� �ش� ���� �� ���� ������ ����.
	if( g_stConnectServerInfo.m_iServiceLanguage > ELANGAGECODE_NONE && g_stConnectServerInfo.m_iServiceLanguage < ELANGAGECODE_MAX )
	{
		char	pszFileName[MAX_PATH];
		sprintf( pszFileName, "%s\\ErrorText.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
		if( fp = BindJXFile::GetInstance()->GetFile( pszFileName ) )
		{
			// �Ľ��� �����Ѵ�.
			int		iRet, iErrLine;
			BOOL	bLess;
			if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) > 0 )
			{
				// �ڷ� �˻�
				while( !bFind )
				{
					if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

					SI32	iLineNo, iColNo;

					// �ڷḦ ���������� �޴´�.
					int		iCode;
					char	pszErrorText[1024], pszBuffer[1024];

					ocp.GetValue( iCode,				iLineNo, iColNo );		// ��ȣ
					ocp.GetValue( pszBuffer,	1023,	iLineNo, iColNo );		// ����

					// �ϴ� 1�� �����޽����� ������ ������ ���´�. �����޽����� ������ �ȵ� ��츦 ���ؼ���.
					if( iCode != iErrorCode )
					{
						if( iCode != 1 )		continue;
					}
					// �׷��� ���� ��� ���� �޽����� �����Ѵ�.
					else
					{
						sprintf( pszFullErrorMsg, "Error Code - %08X\n\n", iErrorCode);
						bFind	= true;
					}

					// ��±��� ����
					ConvertSpecialChar( pszBuffer );
					// �����޽����� �����Ѵ�.
					va_list		marker;
					va_start( marker, iErrorCode );
					vsprintf( pszErrorText, pszBuffer, marker );
					va_end( marker );     

					// �޽����� ������ ���´�.
					strcat( pszFullErrorMsg, pszErrorText );

					// �� ���� ����
					strcat( pszFullErrorMsg, "\n\n\n" );
				}
				ocp.Release();
			}
			fclose( fp );
		}
	}

	// ������ ���� ������ ���� ������ ����.
	if( fp = BindJXFile::GetInstance()->GetFile( "ErrorText_Eng.dat" ) )
	{
		// �Ľ��� �����Ѵ�.
		int		iRet, iErrLine;
		BOOL	bLess;
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "ErrorText_Eng Error : Cannot Init! [ Ret-%d | Line-%d | Less-%d ]", iRet, iErrLine, bLess );
		}
		else
		{
			// �ڷ� �˻�
			while(1)
			{
				if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����
				
				SI32	iLineNo, iColNo;
				
				// �ڷḦ ���������� �޴´�.
				int		iCode;
				char	pszErrorText[1024], pszBuffer[1024];
				
				ocp.GetValue( iCode,				iLineNo, iColNo );		// ��ȣ
				ocp.GetValue( pszBuffer,	1023,	iLineNo, iColNo );		// ����
				
				// �����͸� ã�� ���ߴٸ� �� �������� �˻��Ѵ�.
				if( iCode != iErrorCode )		continue;
				bFind	= true;

				// ��±��� ����
				ConvertSpecialChar( pszBuffer );
				// �����޽����� �����Ѵ�.
				va_list		marker;
				va_start( marker, iErrorCode );
				vsprintf( pszErrorText, pszBuffer, marker );
				va_end( marker );     

				// �޽����� ������ ���´�.
				strcat( pszFullErrorMsg, pszErrorText );
				break;
			}
			ocp.Release();
		}
		fclose( fp );
	}

	// ���� �ش� �ε����� �������� �޽����� �ϳ��� �߰����� ���ߴٸ�
	if( !bFind )
	{
		sprintf( pszFullErrorMsg, "ErrorCode [%08X]\nUnknown Error Message Type!", iErrorCode );
	}

	MessageBox( ( pGame ? pGame->Hwnd : NULL ), pszFullErrorMsg, "Gersang", MB_OK|MB_ICONWARNING|MB_TOPMOST );
}

//-----------------------------------------------------------------------------
// Name: ConvertSpecialChar()
// Code: actdoll (2004-03-12)
// Desc: OnlineText Ŭ������ �ִ� ��ɰ� ������ ���̴�. 
//		�޽��� ó���� ���Ǹ� ���� �ϴ� ���⿡�� ������ ����.
//		iErrorCode		- �����ڵ�
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
