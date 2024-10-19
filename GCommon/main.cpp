//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	ÀÓÁø·Ï ¿Â¶óÀÎ [°Å»ó] - ¸ÞÀÎ¹®
//	File Name		:	Main.cpp
//	Birth Date		:	2004. 01. 01.
//	Creator			:	°Å »ó ÆÀ
//	Editer			:	±èÅÂ°ï, ¾çÁ¤¸ð, ÀÌÀ±¼®, ¹é»óÇö, ÀÌÁØ¼®, Á¤Áø¿í, 
//						È«¼ºÈÆ, Á¶ÇöÁØ, ÀÌ¹Î¼ö, ÀÓÁØÇõ, ¹Ú»ó¿ø, ±è¼ºÁØ,
//						±èÀ¯¹Î, ±ÇÇõÈÆ, ÃÖº´ÁÖ, À±¿µ´ë
//	Copyright		:	Joyon Entertainments. Co. 2000. 2005.
//	Comments		: 
//		°Å»óÀÇ ¸ÞÀÎ¹®ÀÔ´Ï´Ù.
//
//===================================================================================================

//--------------------------------------------------------------------
//  ´ã´ç : ±èÅÂ°ï
//--------------------------------------------------------------------
/********************************************************************
*
*  1998.8.22~ 2000.1.6
*  ÀÓÁø·Ï2 
*
*********************************************************************/
// Initialize the COM interface identifiers
//#define WIN32_LEAN_AND_MEAN
//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: ±âº» Çì´õ ¼±¾ð
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

// actdoll (2004/10/19 11:45) : °ÔÀÓ°¡µå ÇÔ¼ö °ü·Ã
#include "GGuard.h"

//-----------------------------------------------------------------------------
// actdoll (2004/03/11 13:58) : Å©¸®Æ¼ÄÃ ¹ö±×¸¦ Àâ±âÀ§ÇÑ µð¹ö±×¿ë ÆÄÀÏ »ý¼º Å¬·¡½º
//-----------------------------------------------------------------------------
#include "..\\SCCommon\\JODebug.h"
#include <GSMemory.h>

#include <resource.h>

// Áöµµ ÆíÁý±â ¸ðµå. 
//#define _MAPEDIT	1

//--------------------------------------------------------------------------
// ÃÊ±âÈ­ ¿¡·¯½Ã Áý¾î³ÖÀ» ½ºÆ®¸µ
//--------------------------------------------------------------------------
const	char*	DF_ERROR_MEMORY_NOT_ENOUGH			= "¸Þ¸ð¸®°¡ ÃæºÐÇÏÁö ¾Ê½À´Ï´Ù!\n\n Memory is not enough! Program will be shutdown.";
const	char*	DF_ERROR_CREATE_STANDARD_NTN		= "Error Code [80000001]\nCannot check International Information Setting(IIS)!\n\n Program will run by basic informations of OS.\n We recommand to reinstall when you catch some problems.";
const	char*	DF_ERROR_CANNOT_FIND_HQ				= "Error Code [80000002]\nCRITICAL ERROR!\n\n  CANNOT FIND CORE DATA!\n  PLEASE REINSTALL.";
const	char*	DF_ERROR_WARNING_FOR_CRACK			= "Error Code [80000003]\nCRITICAL ERROR!\n\n  CORE DATA AUTHORIZATION FAILED!\n  PLEASE REINSTALL.";
const	char*	DF_ERROR_DXINIT_IS_FAILED			= "½Ã½ºÅÛ ÃÊ±âÈ­ Áß ¿À·ù°¡ ¹ß»ýÇß½À´Ï´Ù!\n\n System Initialize is Failed!";
const	char*	DF_ERROR_CANNOT_EXEC_LAUNCHER		= "ÀÌ ½ÇÇàÆÄÀÏÀº ¹Ù·Î ÀÛµ¿½ÃÅ³ ¼ö ¾ø½À´Ï´Ù!\n%s Æú´õ¿¡ ÀÖ´Â run.exe¸¦ ½ÇÇàÇØ ÁÖ½Ê½Ã¿À.\n¸¸¾à run.exe¸¦ ½ÇÇàÇß´Âµ¥ ÀÌ ¿¡·¯°¡ ¶á´Ù¸é ·±ÃÄ°¡ ¾÷µ¥ÀÌÆ®µÈ °æ¿ìÀÌ¹Ç·Î ´Ù½Ã ÇÑ¹ø ½ÃµµÇØ ÁÖ½Ê½Ã¿À.\n\n You cannot run this file directly!\n Please execute the \"run.exe\" file in %s folder.\n Also you can see this when launcher was updated. Then, please try again.";
const	char*	DF_ERROR_UPDATE_VERSION_SERVER		= "º¯°æµÈ »çÇ×À» Àû¿ëÇÏ±â À§ÇØ °ÔÀÓÀ» ´Ù½Ã ÇÑ¹ø ½ÇÇàÇØ ÁÖ½Ê½Ã¿À.\n\n Please try again to run the game for applying new updates.";
const	char*	DF_ERROR_CANNOT_FIND_ISI			= "±¹°¡Á¤º¸¸®½ºÆ® ÆÄÀÏÀ» Ã£À» ¼ö ¾ø°Å³ª ¹®Á¦°¡ ÀÖ½À´Ï´Ù! ÇÁ·Î±×·¥À» ´Ù½Ã ¼³Ä¡ÇØ ÁÖ¼¼¿ä.\n\n International Information List(IIL) is incorrect! Please reinstall.";
const	char*	DF_ERROR_CANNOT_FIND_NATION_LIST	= "¼³Á¤µÈ ±¹°¡Á¤º¸¿Í ÀÏÄ¡ÇÏ´Â µ¥ÀÌÅÍ°¡ ¾ø½À´Ï´Ù. ÇÁ·Î±×·¥À» ´Ù½Ã ¼³Ä¡ÇØ ÁÖ¼¼¿ä.\n\n International Information List(IIL) is Mismatched with International Information Setting(IIS)!\n Please reinstall.";

//--------------------------------------------------------------------------
// ¸ÞÀÎÈ­¸é ¶óÀÌ¼¾½º ÅØ½ºÆ®
//--------------------------------------------------------------------------
const	char*	DF_TEXT_COPYRIGHT_WITH_GAMANIA	= "Copyright 2001-2005. Gamania & Joyon Entertainment. All rights reserved.";		// ÇÑ±¹/ÀÏº»/´ë¸¸/Áß±¹/È«Äá¿ë
const	char*	DF_TEXT_COPYRIGHT_WITH_BOLEHCOM	= "Copyright 2004-2005. BOLEH.COM & Joyon Entertainment. All rights reserved.";		// ÀÎµµ³×½Ã¾Æ¿ë

//--------------------------------------------------------------------------
// °¢ ±¹°¡º° ÀÎ½ÄÄÚµå - °¢ ±¹°¡º° ÀüÈ­¹øÈ£ ÄÚµå - ¼­ºñ½º ½ÃÀÛµÈ ¼ø¼­´ë·Î ÀÛ¼º
//--------------------------------------------------------------------------
const	int		DF_CODE_NATION_IDENTITY_KOREA		= 82;		// ÇÑ±¹
const	int		DF_CODE_NATION_IDENTITY_TAIWAN		= 886;		// ´ë¸¸
const	int		DF_CODE_NATION_IDENTITY_JAPAN		= 81;		// ÀÏº»
const	int		DF_CODE_NATION_IDENTITY_HONGKONG	= 852;		// È«Äá
const	int		DF_CODE_NATION_IDENTITY_CHINA		= 86;		// Áß±¹
const	int		DF_CODE_NATION_IDENTITY_INDONESIA	= 62;		// ÀÎµµ³×½Ã¾Æ

//--------------------------------------------------------------------------
// °¢ ¾ð¾îº° ÄÚµåÆäÀÌÁö
//--------------------------------------------------------------------------
const	int		DF_CODE_CODEPAGE_ENGLISH			= CP_ACP;			// ¿µ¹®(±âº»)
const	int		DF_CODE_CODEPAGE_KOREAN				= 949;				// ÇÑ±¹¾î 
const	int		DF_CODE_CODEPAGE_JAPANESE			= 932;				// ÀÏº»¾î
const	int		DF_CODE_CODEPAGE_CHINESE_SIMP		= 936;				// Áß±¹¾î(Áß±¹, ½Ì°¡Æ÷¸£)
const	int		DF_CODE_CODEPAGE_CHINESE_TRAD		= 950;				// Áß±¹¾î(´ë¸¸, È«Äá)

//--------------------------------------------------------------------------
// °¢ ±¹°¡º° HQ Æú´õ¸í
//--------------------------------------------------------------------------
char*	const	DF_TEXT_SERVICE_NATION_LIST[ELANGAGECODE_MAX]	= { "None", "Korea", "Taiwan", "Japan", "Hongkong", "China", "Indonesia" };

//--------------------------------------------------------------------------
// µð¹ö±×¿ë °æ·Î
//--------------------------------------------------------------------------
char			DF_PATH_DEBUG_OUTSIDE_HQTEXT[MAX_PATH]	= {0,};	// (µð¹ö±×¿ë) ¿ÜºÎ HQ Text ÂüÁ¶°æ·Î

//------------------------------------
// ±×·¡ÇÈ¿¡ °ü·ÃµÈ Class ¼±¾ð 
//------------------------------------
extern cltGrp                clGrp;               // ±×·¡ÇÈ¿¡ °ü·ÃµÈ Class ¼±¾ð 

//--------------------------------------------------------------------------
// Àü¿ª º¯¼öµé 
//--------------------------------------------------------------------------
_clGame *pGame;


XSPR TileSpr[MAX_MAP_TILE_SPR_NUMBER];						// Áöµµ ÆùÆ®¿ë Å¸ÀÏ 
XSPR TitleSpr;												// Å¸ÀÌÆ² ¹è°æ ±×¸²¿¡ »ç¿ëµÉ Æ÷¸Ë  
XSPR PannelSpr;

//------------------------------------
// ³×Æ®¿öÅ© ¼±¾ð  
//------------------------------------

OnlineBattleInfo OBInfo;

int     MultiPlayDelayTime		= DEFAULT_MULTI_PLAY_DELAY_TIME;      // MultiPlay Delay±âº» ½Ã°£ 
DWORD	PrevMultiClock;
DWORD	MessageSendFrame		= 0;  // ¸í·ÉÀÌ³ª NO_MESSAGE¸¦ º¸³½ ÇÁ·¹ÀÓ 
BOOL	GameDropSwitch			= FALSE;
_MyNetGame MyNet;



//------------------------------------
// È­¸é°ü·Ã 
//------------------------------------
// actdoll (2005/01/10 23:08) : À©µµ ¸ðµå º¯°æ ¿©ºÎ
//	ÀÌÁ¦ ÇÃ·¡±× ÆÄÀÏÀ» ÀÌ¿ëÇÏ¿© À©µµ ¸ðµå¸¦ º¯°æÇÑ´Ù.
//#ifdef _WINDOWED
//SI32  WindowModeSwitch = TRUE;
//#else
//SI32  WindowModeSwitch = FALSE;
//#endif
extern int WindowModeSwitch;
SI32				WindowModeSwitch	= FALSE;

_ScreenPoint        SP;									// È­¸é¿¡ »ç¿ëµÉ ¿©·¯ Á¤º¸ 
int					bActive;							// ÇöÀç ¾×Æ¼ºêÀÎÁö ¾Æ´ÑÁö ¾Ë·ÁÁÖ´Â º¯¼ö  
BOOL				g_bMainInit;						// ÇöÀç ÃÊ±âÈ­ ÁßÀÎ °æ¿ì ¹«Á¶°Ç ¾×Æ¼ºê°¡ µÇµµ·Ï
_clCapture			Capture;							// È­¸é Ä¸Ãç °ü·Ã Å¬·¡½º 

UI32  GameScreenMode = MODE_GRAPHICS800_600;

DWORD LastDrawClock;

//------------------------------------
// °ÔÀÓ °ü·Ã º¯¼öµé 
//------------------------------------
SI32	UserPlayerNumber;
SHORT	GameLeftPressSwitch = FALSE;
SHORT	GameRightPressSwitch= FALSE;
SHORT	GameMouseX=10;
SHORT	GameMouseY=10;
SHORT	GameRealX;
SHORT	GameRealY;
DWORD	GameWParam;

DWORD CaptureClock ;      // °ÔÀÓÀ» Ä¸ÃçÇÑ ½Ã°£. 

// Àü±â °ø°Ý °ü·Ã 
SHORT PointNumber;
SHORT PointArray[200];

//------------------------------------
// °ÔÀÓ ³»ºÎ Á¤º¸¸¦ À§ÇÑ º¯¼öµé 
//------------------------------------
_Item    Item[MAX_ITEM_NUMBER];
_Order   Order[MAX_ORDER_NUMBER];
_MapEdit MapEdit;

_Help					Help;
_Chat					Chat;                           // ÀÔ·Â°ü·Ã
_Cheat					Cheat;
_FindPathGame		   *FindPathGame;

//------------------------------------
// °ÔÀÓ ½ÃÀÛ¸ðµå 
//------------------------------------
#ifdef _MAPEDIT
SHORT					MapEditMode          = TRUE;				// Áöµµ ÆíÁý ¸ðµå(TRUE : Áöµµ ÆíÁý±â, FALSE : °ÔÀÓ)
#else
SHORT					MapEditMode          = FALSE;				// Áöµµ ÆíÁý ¸ðµå(TRUE : Áöµµ ÆíÁý±â, FALSE : °ÔÀÓ)
#endif

//------------------------------------
// °ÔÀÓ ³»ºÎ º¯¼ö 
//------------------------------------
#ifdef _MAPEDIT
SHORT					GameStatus				= GS_INITONLINEBATTLEGAME; // ÇöÀçÀÇ °ÔÀÓ »óÈ²À» ¾Ë¸®´Â º¯¼ö 
#else
SHORT					GameStatus				= GS_INITONLINEWORLD; // ÇöÀçÀÇ °ÔÀÓ »óÈ²À» ¾Ë¸®´Â º¯¼ö 
#endif

SHORT					GameStatusBuffer;
SHORT					GamePauseSwitch			= FALSE;          // °ÔÀÓÀÌ Àá½Ã Áß´ÜµÇ¾ú´Â°¡?
SHORT					GameMenuShowSwitch		= FALSE;          // °ÔÀÓÁß¿¡ ¸Þ´º¸¦ º¸¿©ÁÖ´ÂÁö ¿©ºÎ¸¦ ¾Ë·ÁÁØ´Ù. 
SHORT					MessageMode				= MESSAGEMODE_ALL;// ¸Þ½ÃÁö°¡ ´©±¸¿¡°Ô °¡´Â°¡?


// °ÔÀÓ¿¡¼­ »ç¿ëµÇ´Â º¯¼ö 
RECT					ScreenRect;									// Áöµµ°¡ Ç¥½ÃµÇ´Â ¿µ¿ª 
_Text					Text;										// ÅØ½ºÆ® Å¬·¡½º ¼±¾ð 
_FileVary				FV;											// »ç¿ëµÇ´Â ÆÄÀÏÀÇ º¯¼ö¸¦ ÀúÀåÇÑ´Ù. 
_KindInfo				KI[KIND_ON_MAX];					// Ä³¸¯ÅÍ Á¾·ùº° µ¥ÀÌÅÍ 
CCharFileLoadManager	g_FileLoadManager;							// Ä³¸¯ÅÍ µ¥ÀÌÅÍ
CItem					*g_pItem = NULL;							// ¾ÆÀÌÅÛ Å¬·¡½º


_NewID					ZeroID;


//------------------------------------
// °æ·Î ÀúÀåº¯¼ö 
//------------------------------------
char					SavePath[256];								// ÇöÀç °ÔÀÓÀÌ ¼³Ä¡µÈ °æ·Î 
char					CurrentDirectory[256];

//------------------------------------
// ¸Þ´º °ü·Ã 
//------------------------------------
int   MenuMode;											// ¸Þ´ºÀÇ »óÅÂ¸¦ ÀúÀå 
SHORT MenuOfMouse;										// ¸¶¿ì½º Ä¿¼­°¡ À§Ä¡ÇÏ°í ÀÖ´Â ¸Þ´ºÀÇ ¹øÈ£ 
SHORT CrewMenuOfMouse;

BOOL IsMosueInControlSwitch;							// ¸¶¿ì½º°¡ °ÔÀÓ ÄÜÆ®·ÑÀ§¿¡ À§Ä¡ÇÏ°í ÀÖ´Ù.
BOOL bGameAllyWindowSwitch;


//------------------------------------
// »ç¿ëÀÚ °ü·Ã Å°º¸µå¿Í ¸¶¿ì½º 
//------------------------------------
int    MouseMode;										// ¸¶¿ì½º°¡ Ã³ÇÑ »óÅÂ¸¦ ÀúÀå 
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
// »ç¿ëÀÚ °ü·Ã ¸ÊÀ» À§ÇÑ º¯¼öµé 
//------------------------------------
int   MapDragSwitch        = FALSE;
int   MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
SHORT MapX, MapY;                    // ¸¶¿ì½º°¡ À§Ä¡ÇÑ small ÁÂÇ¥ 
SHORT MouseHarvestX, MouseHarvestY;
SHORT UnderMouseCropType;
SHORT UnderMouseCropMount;

//------------------------------------
// °ÔÀÓ¿¡ »ç¿ëµÇ´Â ½Ã°èº¯¼ö 
//------------------------------------
clock_t StartClock, CurrentClock, PrevClock;
clock_t ClickClock;

clock_t MonitorMacroClock1, MonitorMacroClock2;		// ¸ÅÅ©·Î °Ë»ç Process ½ÇÇà ÁÖ±â

struct tm       *pNewTime;

//------------------------------------
// °ÔÀÓ¿¡ »ç¿ëµÇ´Â Blockº¯¼ö¿Í ±âÅ¸º¯¼ö
//------------------------------------
SHORT BasicBlock=BLOCK_CHAR|BLOCK_DEEPWATER|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_RUIN|BLOCK_ICEWALL;
SHORT WaterBlock=BLOCK_CHAR|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_LAND|BLOCK_SHALLOWWATER|BLOCK_ICEWALL;


int   FlyVibrateData[MAX_FLY_VIBRATE]={0, 1, 2, 3, 4, 3, 2, 1};

//=====================================================================
// ÀúÀåÇÏ±â °ü·Ã Á¤º¸ 
//=====================================================================

// ÁöµµÀÇ °¢ ÁÂÇ¥¸¶´ÙÀÇ Áß½É°ª, 
int MapCenterX[MAX_MAP_XSIZE][MAX_MAP_YSIZE];  
int MapCenterY[MAX_MAP_XSIZE][MAX_MAP_YSIZE];
// Àý´ë ÁÂÇ¥¸¦ °¡Áö°í ±âº» ÁÂÇ¥¸¦ ±¸ÇÑ´Ù. 
unsigned char MapByRealX[(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];
unsigned char MapByRealY[(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];

SI32 MaxMapXsize = 180;
SI32 MaxMapYsize = 180;

_Map    Map;                       // Áöµµ¿ë µ¥ÀÌÅÍ  
_YSMap  SMap;                      // ÀÛÀº Áöµµ¿ë µ¥ÀÌÅÍ   
_Char   CR[MAX_CHAR_NUMBER];       // Ä³¸¯ÅÍÀÇ Æ÷ÀÎÅÍ 
cltCharInterface clifChar;         // Ä³¸¯ÅÍ °ü·Ã ÀÎÅÍÆäÀÌ½º 
cltMapInterface clifMap;           // ¸Ê     °ü·Ã ÀÎÅÍÆäÀÌ½º 

_MainData GD;                      // °ÔÀÓÀÇ ÁÖµ¥ÀÌÅÍ 


//HQFileManager	HQFile;
char			LogInID[128]		= {0,};

char			g_AutoID[256];
char			g_AutoIDPW[256];
SI16			g_AutoServerCode = -1;


//----------------------------------------------------------------------------
// actdoll (2004/10/05 9:10) : Å¬¶óÀÌ¾ðÆ® ½Ãµ¿ ¹× ÀÛµ¿ ÇÃ·¡±×
//----------------------------------------------------------------------------
BOOL			g_fTestVersion			= FALSE;		// Å×¼·¿ëÀÎ°¡ ¾Æ´Ñ°¡?
BOOL			g_bSkipIntro			= FALSE;		// ÀÎÆ®·Î ½ºÅµ¿©ºÎ?

// Ä¿¸Çµå ¶óÀÎ ¿É¼Ç
const	DWORD	DF_FLAG_CMDLINE_ACCESS	= 0x10000000;	// Ä¿¸Çµå ¶óÀÎ ¸í·É : °¡µ¿ (ÀÌ °ªÀÌ ¾øÀ» °æ¿ì Å¬¶óÀÌ¾ðÆ®´Â °¡µ¿µÇÁö ¾Ê´Â´Ù.)
const	DWORD	DF_FLAG_CMDLINE_IP		= 0x00000001;	// Ä¿¸Çµå ¶óÀÎ ¸í·É : ÇÁ·ÐÆ® ¼­¹ö IP ¼³Á¤ (¼öÄ¡ÀÖÀ½)
const	DWORD	DF_FLAG_CMDLINE_NT		= 0x00000002;	// Ä¿¸Çµå ¶óÀÎ ¸í·É : ±¹°¡ Á¤º¸°ª ¼³Á¤ (¼öÄ¡ÀÖÀ½)
const	DWORD	DF_FLAG_CMDLINE_ST		= 0x00000004;	// Ä¿¸Çµå ¶óÀÎ ¸í·É : ¼­ºñ½º Å¸ÀÔ°ª ¼³Á¤ (¼öÄ¡ÀÖÀ½)
const	DWORD	DF_FLAG_CMDLINE_RA		= 0x00000008;	// Ä¿¸Çµå ¶óÀÎ ¸í·É : Å¬¶óÀÌ¾ðÆ®¸¦ Á¾·áÇÏ°í ·±Ã³¸¦ ´Ù½ÃÇÑ¹ø ½ÇÇà½ÃÄÑ¶ó (¼öÄ¡¾øÀ½)

const	DWORD	DF_FLAG_CMDLINE_SKIPGG	= 0x00001000;	// Ä¿¸Çµå ¶óÀÎ ¸í·É : °ÔÀÓ°¡µå ½ºÅµ
//----------------------------------------------------------------------------


char			REG_PATH[256] = "";

//=====================================================================



//--------------------------------------------------------------------------
// Main.cpp ÀÇ Methods
//--------------------------------------------------------------------------
extern HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

// actdoll (2004/01/16 0:18) : HQ Ã¼Å©¼¶ ÀåÄ¡ ¹× GCS ¼³Á¤
const	unsigned __int64	DF_VALUE_CS		= 0x2F360C5A2964A4E7;	// Ã¼Å©¼¶ ºñ±³°ª			(Debug½Ã ¶ß´Â HQÆÄÀÏ °Ë»ê ¿À·ù °ªÀ» ¿©±â´Ù Àû¿ë½ÃÅ°¸é µË´Ï´Ù.)
const	UI32				DF_VALUE_CSF	= 0x1c437f64;			// Ã¼Å©¼¶ ÇÃ·¡±×			(ÀÌ °ªÀº ÇÃ·¡±×ÀÔ´Ï´Ù. ÀÌ °ªÀÌ ¹Ù²î¾î¹ö¸®¸é DF_VALUE_CS°ª ÀÚÃ¼°¡ ¹Ù²ò´Ï´Ù.)
const	DWORD				DF_VALUE_ATI	= 0x80d36cfa;			// ¿î¿µÀÚ ¿É¼Ç ÇÃ·¡±×ÀÇ ÀÎÁõÄÚµå(¿î¿µÀÚ Åø º£ÀÌ½º¿¡ ÀÌ °ªÀÌ Àû¿ëµÊ)
const	char*				DF_VALUE_AAC	= "ilovegs";			// ¿î¿µÀÚ°¡ ÀÔ·ÂÇÏ´Â ÀÎÁõÄÚµå ÇÃ·¡±×°ª

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
// Desc: À©µµ¿ì ¸ÞÀÎ¹®. ¸ðµç ÇÁ·Î±×·¥Àº ¿©±â¼­ ½ÃÀÛµÈ´Ù!
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// actdoll (2004/03/12 2:45) : 
	//	°Å»ó Å¬¶óÀÌ¾ðÆ®°¡ ¾î¶² ¸ñÀûÀ¸·Î ¾²ÀÌ´Â ³ðÀÎÁö Ã¼Å©
	DWORD	dwRetOrder;

	GSMemoryInit(GS_DEBUG_WINDOWS);
	GSMemorySetLevel(2);
	XDebugInit(XDOT_DEBUG);
	
	// ¾÷µ¥ÀÌÆ® ³»¿ë È®ÀÎ ¹× º¯°æ >> ¹öÁ¯ °»½Å, ·±ÃÄ °»½Å, GCS °¡µ¿, µð¹ö±× Á¤º¸ ÆÄ¾Ç µî
	if( !Init_CheckUpdates( hInstance ) )		return 0;

	// Ä¿¸Çµå ¶óÀÎ È®ÀÎ
	dwRetOrder	= Init_CheckAdOpt( lpCmdLine, nCmdShow );
	dwRetOrder	= Init_CheckCmdLines( lpCmdLine, nCmdShow, dwRetOrder );

	
	// Å¬¶óÀÌ¾ðÆ® ±âº» Á¤º¸ È®ÀÎ >> HQ·Îµå, »ç³»/Å×¼·/Á¤¼·/±¹°¡ÄÚµå ÆÇº°, ·±ÃÄ¿¡ ÀÇÇÑ °¡µ¿ ¿É¼Ç ÆÇº°
	if( !Init_CheckBasics( hInstance, dwRetOrder ) )	
	{	
		BindJXFile::GetInstance()->destroy();
		return 0;
	}

	// ¿£Áø °¡µ¿ >> _clGame °¡µ¿ ¹× ÃÊ±âÈ­
	if( !Init_Engines( hInstance, nCmdShow, dwRetOrder ) )
	{
		BindJXFile::GetInstance()->destroy();
		return 0;
	}

	// ====================		!!!¸ÞÀÎ ·çÇÁ!!!		==================== //
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
	// ====================		!!!¸ÞÀÎ ·çÇÁ!!!		==================== //

	// ¸®¼Ò½º ÇØÁ¦
	Kill();
	
	GSShowMemoryStatus();
//	showMemStats();

	return msg.wParam;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckUpdates()
// Code: actdoll (2004-03-12)
// Desc: ÇÁ·Î±×·¥ ½ÃÀÛ Àü »õ·ÎÀÌ ¾÷µ¥ÀÌÆ® ÇØ¾ß ÇÒ Á¤º¸µéÀ» È®ÀÎÇÑ´Ù.
//-----------------------------------------------------------------------------
BOOL	Init_CheckUpdates( HINSTANCE hInstance )
{
	// ¼­ºñ½º ±âº»Á¤º¸ ÃÊ±âÈ­
	ZeroMemory( &g_stConnectServerInfo, sizeof(g_stConnectServerInfo) );

	//----------------------------------------------------------------------------
	//	1´Ü°è - ¹öÁ¯ °»½Å
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
	//	2´Ü°è - ·±ÃÄ °»½Å
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
	//	3´Ü°è - HQ µ¥ÀÌÅÍ Ã¼Å© ¹× ·Îµå
	//----------------------------------------------------------------------------
	if( !DataWork( hInstance ) )
	{
		MessageBox( NULL, DF_ERROR_WARNING_FOR_CRACK, "Gersang", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	4´Ü°è - ±¹°¡ Á¤º¸, ¼­ºñ½º Å¸ÀÔÀ» È®ÀÎ
	//----------------------------------------------------------------------------
	// ±¹Àû Á¤º¸¸¦ È®ÀÎÇÑ´Ù.
	int		iNation = 0, iType = 0;
	if( !GetNTN( "Online\\ntn.con", iNation, iType ) )
	{
		// ±¹°¡ Á¤º¸ ÆÄÀÏ¿¡ ¹®Á¦°¡ »ý°å´Ù¸é »õ·Î¿î Á¤º¸ ÆÄÀÏÀ» ¸¸µé°í ¸Þ½ÃÁö ¹Ú½º ¶ç¿î´Ù.
		CreateNTN( "Online\\ntn.con", iNation, iType );
		ShowErrorMsgBox( 10001001 );
//		MessageBox( NULL, DF_ERROR_CREATE_STANDARD_NTN, "ERROR(64dfacc4)", MB_OK );
	}


	//----------------------------------------------------------------------------
	//	5´Ü°è - ±âº» ·ÎÄÉÀÏ Á¤º¸ Ã¼Å©
	//----------------------------------------------------------------------------
	// ¹®Á¦°¡ ÀÖ´ÂÁö ¾ø´ÂÁö¸¦ È®ÀÎÇÑ´Ù
	if( !SetLocaleInfo( iNation, iType ) )
	{
		BindJXFile::GetInstance()->destroy();
		ShowErrorMsgBox( 10001002 );
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_NATION_LIST, "039addeb", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	6´Ü°è - GCS °¡µ¿(ÀÌ°Ç GCS°¡ Æ÷ÇÔµÈ ÇÑ±¹ Å×¼·¸¸ µÈ´Ù)
	//----------------------------------------------------------------------------
	SYSTEMTIME		sysTime;
	char			pszCrashText[1024], pszCrashTime[1024];
	GetSystemTime( &sysTime );
	sprintf( pszCrashText, "!!CriticalCrash!!_V%d", ON_CLINETVERSION );
	sprintf( pszCrashTime, "StartTime- %d/%d/%d %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	InstallCrashHandler( pszCrashText, pszCrashTime, "GERSANG", CRASH_CLIENT );				// Å©·¡½¬ ÇÚµé·¯ ¼³Ä¡
	SetNationCode( DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );	// Å©·¡½¬ ±¹°¡ ÄÚµå ¼³Ä¡


	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Init_CheckAdOpt()
// Code: actdoll (2004-03-12)
// Desc: ¾îµå¹Î ¿É¼ÇÀ» Ã¼Å©ÇÑ´Ù.
//		return		- °¡µ¿ ½ÃÀÛÀÌ¶ó¸é true, ¾Æ´Ï¶ó¸é false
//-----------------------------------------------------------------------------
DWORD	Init_CheckAdOpt( LPSTR lpCmdLine, int nCmdShow )
{
	// ¹ÂÅØ½º¸¦ »ý¼ºÇÏ¿© ¾îµå¹Î ÅøÀÌ °¡µ¿ÁßÀÎÁö ¾Æ´ÑÁö È®ÀÎ
	HANDLE	hMutex	= CreateMutex( NULL, TRUE, "gs041216cat" );
	DWORD	dwRet	= GetLastError();
	ReleaseMutex( hMutex );
	CloseHandle( hMutex );

	// ÀÌ »óÈ²ÀÌ¶ó¸é Á¤»óÀûÀÎ ÀÛµ¿ »óÅÂ°¡ ¾Æ´Ï´Ù.
	if( !hMutex || dwRet != ERROR_ALREADY_EXISTS )		return 0;

	// ÀÏ´Ü ¸í·ÉÁÙÀ» ÅëÇÑ µ¥ÀÌÅÍ °É·¯³»±â °³½Ã
	BOOL	bFound = false;
	char	*pRealData, *pszToken, pszData[2048] = {0,}, pszAAC[128] = {0,};

	strcpy( pszData, lpCmdLine );
	pszToken = strtok( pszData, "- \t\n\r" );
	while( pszToken )
	{
		// ¿î¿µÀÚ ÇÃ·¡±× µ¥ÀÌÅÍ¸¦ Ã£¾Æ³½´Ù.
		if( ( pRealData = strstr( pszToken, "AO:" ) ) != NULL )
		{
			bFound	= true;
			pRealData	+= 3;	// ½ÇÁ¦ µ¥ÀÌÅÍ ÃÊ¹ÝÀ¸·Î ÀÌµ¿
			break;
		}
		pszToken	= strtok( NULL, "- \t\n\r" );
	}

	// ÇÃ·¡±×·Î Ã£Áö ¸øÇß´Ù¸é ÆÄÀÏ·Î Ã£¾Æº»´Ù.
	if( !bFound )
	{
		char	pszPath[MAX_PATH];
		GetWindowsDirectory( pszPath, MAX_PATH );
		strcat( pszPath, "\\Ffgsdeac.cks" );
		
		// ÆÄÀÏ ÀÚÃ¼°¡ ¾ø´Ù¸é ¿¡·¯´Ù
		FILE	*fp = fopen( pszPath, "rb" );
		if( !fp )	return 0;

		// ÀÏ´Ü Å©±â ¹Þ°í
		fseek( fp, 0, SEEK_END );
		DWORD	dwLen = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		// ÆÄÀÏÀ» ¹öÆÛ¿¡ ÀûÀç
		fread( pszData, dwLen, sizeof(BYTE), fp );
		pRealData	= pszData;

		fclose( fp );
	}

struct ST_ADFLAG
{
	UINT	m_uiNTN;				// ¿î¿µÀÚ ÁöÁ¤ NTN
	
	BOOL	m_bSkipLauncher;		// ·±ÃÄ ½ºÅµ ¿©ºÎ
	BOOL	m_bSkipIntroScreen;		// ÀÎÆ®·Î ½ºÅµ ¿©ºÎ
	BOOL	m_bSkipGameGuard;		// °ÔÀÓ°¡µå ½ºÅµ ¿©ºÎ
	BOOL	m_bRunAsWindowMode;		// À©µµ¸ðµå·Î °¡µ¿ ¿©ºÎ

	char	m_szVersionServerIP[64];		// ¹öÀü¼­¹ö IP ½ºÆ®¸µÀÌ µé¾îÀÖ´Â Æ÷ÀÎÅÍ
	char	m_szFrontServerIP[64];			// ÇÁ·ÐÆ®¼­¹ö IP ½ºÆ®¸µÀÌ µé¾îÀÖ´Â Æ÷ÀÎÅÍ
	char	m_szPathCustomHQ[MAX_PATH];		// ÇÁ·ÐÆ®¼­¹ö IP ½ºÆ®¸µÀÌ µé¾îÀÖ´Â Æ÷ÀÎÅÍ
}	stAdmin = {0,};

	// °Ë»ê µé¾î°£´Ù.
	UINT	uiRet = GetAdDecoData( (BYTE*)pRealData, strlen( pRealData ), (DWORD&)stAdmin.m_uiNTN, 
									stAdmin.m_bSkipGameGuard, stAdmin.m_bSkipLauncher, stAdmin.m_bSkipIntroScreen, stAdmin.m_bRunAsWindowMode,
									pszAAC, stAdmin.m_szVersionServerIP, stAdmin.m_szFrontServerIP, stAdmin.m_szPathCustomHQ );
	if( stricmp( pszAAC, DF_VALUE_AAC ) != 0 || uiRet == 0 )		
	{
		ZeroMemory( &stAdmin, sizeof(stAdmin) );
		return 0;
	}

	// Á¤»óÀÌ´Ù. ¼¼ÆÃ È®ÀÎ
	dwRet	= 0;
	if( strlen(stAdmin.m_szFrontServerIP) > 1 )	{ dwRet |= DF_FLAG_CMDLINE_IP;	strcpy( g_stConnectServerInfo.m_FrontServerIP, stAdmin.m_szFrontServerIP ); }	// ÇÁ·ÐÆ®¼­¹ö IPº¯Á¶
	if( strlen(stAdmin.m_szPathCustomHQ) > 1 )	{ strcpy( DF_PATH_DEBUG_OUTSIDE_HQTEXT, stAdmin.m_szPathCustomHQ ); }											// ÀÓÀÇ HQ °æ·Î
	if( stAdmin.m_uiNTN != 0 )					{ dwRet	|= DF_FLAG_CMDLINE_NT | DF_FLAG_CMDLINE_ST;	g_stConnectServerInfo.m_iServiceNation = stAdmin.m_uiNTN; g_stConnectServerInfo.m_iServiceType = 1; }	// NTN ¼¼ÆÃ
	if( stAdmin.m_bSkipGameGuard )				{ dwRet	|= DF_FLAG_CMDLINE_SKIPGG; }	// °ÔÀÓ°¡µå ½ºÅµ¿©ºÎ
	if( stAdmin.m_bSkipLauncher )				{ dwRet	|= DF_FLAG_CMDLINE_ACCESS; }	// ·±ÃÄ ½ºÅµ ¿©ºÎ
	g_bSkipIntro		= stAdmin.m_bSkipIntroScreen;									// ÀÎÆ®·Î ½ºÅµ ¿©ºÎ
	WindowModeSwitch	= stAdmin.m_bRunAsWindowMode;									// À©µµ¸ðµå °¡µ¿¿©ºÎ

	// È®ÀÎ»ç»ì
	ZeroMemory( &stAdmin, sizeof(stAdmin) );
	MessageBox( NULL, "Administrator Option was initialized!", "Gersang", MB_OK|MB_ICONINFORMATION );
	// actdoll (2005/01/05 18:26) : ¿¹¿ÜÀûÀ¸·Î ¿©±â¼­ ¿î¿µÀÚ È®ÀÎÀÌ ³¡³µÀ» °æ¿ì Å©·¡½¬ Á¤º¸¸¦ °»½ÅÇÑ´Ù.
	SetAppType( CRASH_CLIENT_ADMIN );

	return dwRet;
}


//-----------------------------------------------------------------------------
// Name: Init_CheckCmdLines()
// Code: actdoll (2004-03-12)
// Desc: Ä¿¸Çµå ¶óÀÎÀÇ Á¤º¸¸¦ ÀÐ¾î¿Â´Ù.
//		return		- ¹«»çÈ÷ ³¡³Â´Ù¸é true, ¾Æ´Ï¶ó¸é false
//-----------------------------------------------------------------------------
DWORD	Init_CheckCmdLines( LPSTR lpCmdLine, int nCmdShow, DWORD dwOrder )
{
	char	*pTemp, *pOrder, pszCmdLine[2048] = {0,};


	//----------------------------------------------------------------------------
	//	1´Ü°è - Å¬¶óÀÌ¾ðÆ® ½ÇÇà °¡´É ÇÃ·¡±× È®ÀÎ
	//----------------------------------------------------------------------------
	strcpy( pszCmdLine, lpCmdLine );
	pOrder = strtok( pszCmdLine, "- \t\n\r" );
	while( pOrder )
	{
		// ÇØ¿Ü²¨ ´ëºñÇØ¼­ ÀÌ¹ø¸¸ °¡µ¿½ÃÅ²´Ù.
		if( stricmp( pOrder, "LF" ) == 0 )
		{
			dwOrder |= DF_FLAG_CMDLINE_ACCESS;
			break;
		}
		// ·±Ã³¿¡¼­ °¡µ¿µÇ¾ú´Ù.	(Launcher Start)
//			if( stricmp( pOrder, "LS" ) == 0 )
//			{
//				dwOrder |= DF_FLAG_CMDLINE_ACCESS;
//				break;
//			}
//			// ¼öµ¿À¸·Î °¡µ¿µÇ¾ú´Ù.	(Manual Start)
//			if( ( pTemp = strstr( pOrder, "MS:" ) ) != NULL )
//			{
//				pTemp += 3;
//				if( strlen( pTemp ) )	// ¼öÄ¡ºñ±³
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


	// ¸¸¾à ¸í·ÉÁÙ °¡µ¿ ÇÃ·¡±×°¡ ¾ø´Ù¸é ÀÏ´Ü ½ÇÆÐ
	if( !(dwOrder & DF_FLAG_CMDLINE_ACCESS) )		return 0;


	//----------------------------------------------------------------------------
	//	2´Ü°è - ±âÅ¸ ¸í·Éµé È®ÀÎ
	//----------------------------------------------------------------------------
	strcpy( pszCmdLine, lpCmdLine );
	pOrder = strtok( pszCmdLine, "- \t\n\r" );
	while( pOrder )
	{
		// ÇÁ·ÐÆ® ¼­¹ö IP.		(frontserver IP)
		if( !( dwOrder & DF_FLAG_CMDLINE_IP ) &&	( pTemp = strstr( pOrder, "IP:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// IP ÁÖ¼Ò º¹»ç
			{
				strcpy( g_stConnectServerInfo.m_FrontServerIP, pTemp );
				dwOrder |= DF_FLAG_CMDLINE_IP;
			}
		}
		// ±¹°¡ Á¤º¸.			(Nation Type)
		else if( !( dwOrder & DF_FLAG_CMDLINE_NT ) &&	( pTemp = strstr( pOrder, "NT:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// ±¹°¡Á¤º¸ ¼öÄ¡ º¹»ç
			{
				g_stConnectServerInfo.m_iServiceNation	= atoi( pTemp );
				dwOrder |= DF_FLAG_CMDLINE_NT;
			}
		}
		// ¼­ºñ½º Á¤º¸.			(Service Type)
		else if( !( dwOrder & DF_FLAG_CMDLINE_ST ) &&	( pTemp = strstr( pOrder, "ST:" ) ) != NULL )
		{
			pTemp += 3;
			if( strlen( pTemp ) )	// ±¹°¡Á¤º¸ ¼öÄ¡ º¹»ç
			{
				g_stConnectServerInfo.m_iServiceType	= atoi( pTemp );
				dwOrder |= DF_FLAG_CMDLINE_ST;
			}
		}
		// Àç±âµ¿ ¿äÃ».			(Retry Again)
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
// Desc: ÃÊ±â¿¡ ·ÎµùµÇ¾ß ÇÒ ±âº» Á¤º¸µéÀ» »ìÆìº»´Ù.
//		return		- ¹«»çÈ÷ ³¡³Â´Ù¸é true, ¾Æ´Ï¶ó¸é false
//-----------------------------------------------------------------------------
BOOL	Init_CheckBasics( HINSTANCE hInstance, DWORD dwOrder )
{
	//----------------------------------------------------------------------------
	//	1´Ü°è - ±âº» ·ÎÄÉÀÏ Á¤º¸ Àç°Ë»ö(¼öÁ¤µÈ ServiceNation, Type, FrontServerIP¸¦ Àû¿ë)
	//----------------------------------------------------------------------------
	// ¿î¿µÀÚ ¸í·Éµî¿¡ ÀÇÇØ ÀÛ¾÷ÀÌ ¼öÁ¤µÆÀ» ¼ö ÀÖÀ¸¹Ç·Î ´Ù½Ã ÇÑ¹ø È®ÀÎÇÑ´Ù.
//	if( !(dwOrder & DF_FLAG_CMDLINE_NT) )	g_stConnectServerInfo.m_iServiceNation	= iNation;
//	if( !(dwOrder & DF_FLAG_CMDLINE_ST) )	g_stConnectServerInfo.m_iServiceType	= iType;
	if( !SetLocaleInfo( g_stConnectServerInfo.m_iServiceNation, g_stConnectServerInfo.m_iServiceType, g_stConnectServerInfo.m_FrontServerIP ) )
	{
		ShowErrorMsgBox( 10001002 );
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_NATION_LIST, "039addeb", MB_OK );
		return FALSE;
	}


	//----------------------------------------------------------------------------
	//	2´Ü°è - Å¬¶óÀÌ¾ðÆ® °¡µ¿ °¡´É È®ÀÎ
	//----------------------------------------------------------------------------
	// ½ÇÇà °¡´É À¯¹« ÆÇ´Ü
	if( !(dwOrder & DF_FLAG_CMDLINE_ACCESS) )
	{
		char	pszPath[MAX_PATH];//pszTemp[1024], 
		GetCurrentDirectory( MAX_PATH, pszPath );
		ShowErrorMsgBox( 10001003, pszPath );
//		sprintf( pszTemp, DF_ERROR_CANNOT_EXEC_LAUNCHER, pszPath, pszPath );
//		MessageBox( NULL, pszTemp, "Error!", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
	// Àç°¡µ¿ À¯¹« ÆÇ´Ü
	if( dwOrder & DF_FLAG_CMDLINE_RA )
	{
		ShowErrorMsgBox( 10001004 );
//		MessageBox( NULL, DF_ERROR_UPDATE_VERSION_SERVER, "Notice!", MB_OK | MB_ICONWARNING );
		return FALSE;
	}


//	//----------------------------------------------------------------------------
//	//	3´Ü°è - ¼¼ÆÃµÈ ±¹°¡/¼­ºñ½ºÅ¸ÀÔ¿¡ µû¶ó ¼­ºñ½º Á¤º¸ µ¥ÀÌÅÍº£ÀÌ½º ¿¡¼­ ¿øÇÏ´Â ³»¿ëÀ» ÃßÃâ
//	//----------------------------------------------------------------------------
//	char	pszDummyIP[128]; 
//	
//	// ¸¸¾à ¸í·É ÇÃ·¡±×¿¡ ÀÇÇØ ÀÌ¹Ì ¼³Á¤µÈ IP°¡ ÀÖ´Ù¸é ÀÏ´Ü ¹é¾÷
//	if( dwOrder & DF_FLAG_CMDLINE_IP )	strcpy( pszDummyIP, g_stConnectServerInfo.m_FrontServerIP );
//	
//	// HQ¿¡¼­ ¿øÇÏ´Â ±¹°¡Á¤º¸¿Í ¼­ºñ½º Å¸ÀÔÀ» ±âÁØÀ¸·Î IP ¹× ¼­ºñ½º»ç Á¤º¸µîÀ» ¾ò´Â´Ù.
//	// ¿ÜºÎ HQ¼³Á¤ÀÌ ¾øÀ» °æ¿ì
//	sprintf( pszNation, "%s\\ServiceListInfo.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
//	if( !ReadIPEx( pszNation, true, dwOrder & DF_FLAG_CMDLINE_NT, 
//					g_stConnectServerInfo.m_iServiceNation, g_stConnectServerInfo.m_iServiceType, &g_stConnectServerInfo ) )
//	{
//		MessageBox( NULL, DF_ERROR_CANNOT_FIND_ISI, "039addeb", MB_OK );
//		BindJXFile::GetInstance()->destroy();
//		return FALSE;
//	}
//	// ÇØ´ç ¸í·É ÇÃ·¡±×°¡ »ì¾ÆÀÖ´Ù¸é ÀÌºÎºÐ¸¸ º¯°æ
//	if( dwOrder & DF_FLAG_CMDLINE_IP )	strcpy( g_stConnectServerInfo.m_FrontServerIP, pszDummyIP );
//
//	//----------------------------------------------------------------------------
//	//	3´Ü°è - ¿¹¿ÜÃ³¸® ÇÃ·¡±× ¼³Á¤
//	//----------------------------------------------------------------------------
//	sprintf( pszNation, "Nation:%d, Type:%d", iNation, iType );								// Å©·¡½¬ ±¹°¡ ÄÚµå ¼¼ÆÃ(±¹°¡, Å¸ÀÔ, ³»ºÎ½ÇÇà¿©ºÎ)
//	SetNationCode( pszNation );																// Å©·¡½¬ ±¹°¡ ÄÚµå ¼³Ä¡

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Init_Engines()
// Code: actdoll (2004-03-12)
// Desc: ±¹°¡ Á¤º¸ ¹× ¼­¹ö Á¤º¸ ÆÄÀÏÀ» ·ÎµùÇÑ´Ù.
//		return		- ¹«»çÈ÷ ³¡³Â´Ù¸é true, ¾Æ´Ï¶ó¸é false
//-----------------------------------------------------------------------------
BOOL	Init_Engines( HINSTANCE hInstance, int nCmdShow, DWORD dwOrder )
{
	// ÃÊ±âÈ­ °úÁ¤¿¡ ÀÖ´Ù. 
	g_bMainInit = TRUE;

	//----------------------------------------------------------------------------
	//	1´Ü°è - °ÔÀÓ °¡µå ¼¼ÆÃ [°ÔÀÓ °¡µå È­¸é µîÀå] (³ªÁß¿¡ Àü ±¹°¡¿¡¼­ ¼­ºñ½ºµÉ °æ¿ì ÀÌ ºÎºÐÀº ´õ ¾ÕÀ¸·Î »«´Ù.)
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
	//	2´Ü°è - °Å»ó °ÔÀÓÅ¬·¡½º ±¸µ¿
	//----------------------------------------------------------------------------

	// °ÔÀÓ Å¬·¡½º »ý¼º ¹× Á¤º¸ ¼¼ÆÃ
	pGame =	new _clGame;
	if(pGame == NULL)	
	{
		// actdoll (2004/03/11 11:39) : ¿¡·¯ ¹ß»ý ÈÄ ¸ðÁ¶¸® ¸®ÅÏ½Ã ¿¡·¯ ÄÚµå º¸¿©ÁØ´Ù.
		ShowErrorMsgBox( 10001006 );
//		MessageBox( NULL, DF_ERROR_MEMORY_NOT_ENOUGH, "ERROR(5bdf448c)", MB_OK );
		return FALSE;
	}

	
	//----------------------------------------------------------------------------
	//	3´Ü°è - ¼­ºñ½º ±¸¼º ¹× Á¢¼Ó ±âº» Á¤º¸ ¼¼ÆÃ
	//----------------------------------------------------------------------------

	pGame->m_iNation		= g_stConnectServerInfo.m_iServiceNation;
	pGame->m_iServiceType	= g_stConnectServerInfo.m_iServiceType;
	pGame->LanguageCode		= (ELanguageCode)g_stConnectServerInfo.m_iServiceLanguage;
	pGame->m_pszHQNation	= DF_TEXT_SERVICE_NATION_LIST[pGame->LanguageCode];
	pGame->m_uiCodePage		= g_stConnectServerInfo.m_iServiceCP;

	
	//----------------------------------------------------------------------------
	//	4´Ü°è - À©µµ ¹× DirectX °¡µ¿
	//----------------------------------------------------------------------------

	if(!pGame->Init(hInstance, nCmdShow))
	{
		ShowErrorMsgBox( 10001007 );
//		MessageBox( NULL, DF_ERROR_DXINIT_IS_FAILED, "ERROR(8e756bc4)", MB_OK );
		delete pGame;
		return FALSE;
	}
	SetHwndToGameGuard( pGame->Hwnd );	// actdoll (2004/10/19 11:44) : °ÔÀÓ°¡µå¿¡ Àû¿ëÇÒ À©µµ ÇÚµéÀ» ÀÌºÎºÐ¿¡ ¼¼ÆÃ
	
	// ÃÊ±âÈ­°¡ Á¾·áµÇ¾ú´Ù. 
	g_bMainInit = FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Kill()
// Code: actdoll (2004-03-12)
// Desc: ¸ÞÀÎ¿¡¼­ ·ÎµùµÈ ¸ðµç µ¥ÀÌÅÍµéÀ» ÇØÁ¦
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
	//	°ÔÀÓ°¡µå ÇØÁ¦ 
	ReleaseGameGuard();
}



//-----------------------------------------------------------------------------
// Name: FCS()
// Code: actdoll (2003-12-30)
// Desc: ÆÄÀÏ Ã¼Å©¼¶À» Ã³¸®ÇÏ´Â ÇÔ¼ö´Ù.
//		±âÁ¸ÀÇ Ã¼Å©¼¶À» º¸°­ÇÑ ¹æ¹ýÀÌ´Ù. byte, word, dword ´ÜÀ§ÀÇ µ¥ÀÌÅÍ ±³Ã¼, Ä¡È¯, ÇÕ»ê µî¿¡ ´ëÇÑ
//		Å©·¡Å·À» Ã¶ÀúÈ÷ Â÷´ÜÇÒ ¼ö ÀÖ´Â ¹æ¹ýÀÌ´Ù. ´Ù¸¸ ÇÔ¼ö ¼Òºñ½Ã°£ÀÌ Á» µç´Ù.
//		(CPU 2.8¿¡¼­ Æò±Õ15~17ms)
//
//		pFileName		- Ã¼Å©¼¶À» ÇÏ°í ½ÍÀº ÆÄÀÏ¸í
//		bAdminOption	- °ü¸®ÀÚ¿ë µ¥ÀÌÅÍ°ª ÃßÃâ ½Ã
//		return			- Ã¼Å©¼¶ °á°ú°ª(64bit)
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

	// ÆÄÀÏ ¿ÀÇÂ
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

		length		= ( uiFileSize >> 2 ) << 1;			// ½ÇÁ¦ °è»êÇÒ ±æÀÌ¸¦ ±¸ÇÑ´Ù.
		remainder	= ( uiFileSize - ( length << 1 ) );	// ³ª¸ÓÁö ¹ÙÀÌÆ®¸¦ °è»êÇØ³í´Ù.

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
// Desc: HQÆÄÀÏµéÀÇ ·Îµù°ú °ü·ÃµÈ ÀÛ¾÷À» Ã³¸®ÇÏ´Â ÇÔ¼ö´Ù.
//		return		- true¶ó¸é HQ¸¦ Á¤»óÀûÀ¸·Î È®ÀÎ, false¶ó¸é HQÀÛ¾÷ ½ÇÆÐ
//-----------------------------------------------------------------------------
BOOL	DataWork( HINSTANCE hInstance )
{
	// °¢ ±¹°¡º° HQÆÄÀÏ °Ë»ö
	char	*pszHQFile = ".\\hq";

	// actdoll (2004/01/15 23:55) : Áß¿äÇÑ hqµ¥ÀÌÅÍ ÆÄÀÏÀ» ÇØÅ·ÇÏÁö ¸øÇÏµµ·Ï Ã¼Å©¼¶À» °É¾î³í´Ù.
	//		Debug ¸ðµå¿¡¼­´Â Ç¥½ÃµÈ ¼öÄ¡¸¦ DF_VALUE_CS °ª¿¡ ´ëÀÔÇÏ¿© ÄÄÆÄÀÏÇÏ¸é µÇ¸ç ±×³É ÁøÇàÇÒ ¼ö ÀÖÀ¸³ª
	//		Release ¸ðµå¿¡¼­´Â ¼öÄ¡°¡ ¸ÂÁö ¾ÊÀ» °æ¿ì ±×³É ³ª°¡¹ö¸°´Ù.
	//		¶ÇÇÑ ½Å±âÇÏ°Ôµµ(?) ÀÌ hqÆÄÀÏÀ» Ã¼Å©ÇÑ ÈÄ ´Ù¸¥ °Å»ó ÇÁ·Î±×·¥ÀÌ µ¹¾Æ°¥ ¶§ hq ÆÄÀÏÀ» ÀÐÁö ¸øÇØ
	//		ÀÌÁß½ÇÇàÀÌ µÇÁö ¾Ê´Â´Ù. ¾Æ¸¶µµ À©µµ File °ü·Ã APIÀÇ ¹ö±× È¤Àº Æ¯¼ºÀÎ µí ÇÏ´Ù.

	// actdoll (2004/05/27 18:54) : V.12250ºÎÅÍ ¸ðµç ¹öÁ¯¿¡ »ó°ü¾øÀÌ hq·Î ÀÌ¸§À» ÅëÇÕÇÑ´Ù.
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
		ShowDlg( hInstance, "Å©·¡Å· ¼¼ÀÌÆÛ ®G¼¶ ¿¡·Ö", pTemp );
#else
		return FALSE;
#endif
	}

	// ÀÌ ºÎºÐÀº ÀÏ¹Ý ¿î¿µÀÚ°¡ Å×½ºÆ®¸¦ À§ÇÑ ¾îµå¹Î ¿É¼ÇÀ» ¼³Á¤ÇÒ °æ¿ì »ç¿ëÇÒ ½Ã¸®¾ó Å°¸¦ ¸®ÅÏÇÑ´Ù.
//#ifdef _DEBUG
//	sprintf( pTemp, "[ Administrator Authorization Code ]\n\n\n%I64d", FCS( pszHQFile, true ) );
//	MessageBox( NULL, pTemp, "AAC Notice - Only Debug", MB_OK );
//	ShowDlg( hInstance, "°ü¸®ÀÚ ÀÎÁõ ÄÚµå (AAC)", (char*)DF_VALUE_AAC );
//#endif

	return ( BindJXFile::GetInstance()->openJX( pszHQFile ) );
}

//-----------------------------------------------------------------------------
// Name: GetNTN()
// Code: actdoll (2004-03-12)
// Desc: ±¹°¡ Á¤º¸ ¹× ¼­¹ö Á¤º¸ ÆÄÀÏÀ» Ã¼Å©ÇÑ´Ù.
//		pszFileName	- ntnÆÄÀÏ¸í
//		iNation		- ¼­ºñ½º ±¹°¡ ÄÚµå, ¿¡·¯¶ó¸é -1 ¸®ÅÏ
//		iType		- ¼­ºñ½º Å¸ÀÔ, ¿¡·¯¶ó¸é -1 ¸®ÅÏ
//		return		- true¶ó¸é Á¤»óÀûÀ¸·Î µ¥ÀÌÅÍ¸¦ È®ÀÎ, false¶ó¸é ÆÄÀÏÀÌ ¾ø°Å³ª Á¤º¸ ÃßÃâ ½ÇÆÐ
//-----------------------------------------------------------------------------
BOOL	GetNTN( char *pszFileName, int &iNation, int &iType )
{
	FILE	*pFile;

	iNation = iType = -1;

	pFile	= fopen( pszFileName, "rb" );
	if( !pFile )	return FALSE;

	// ÆÄÀÏÀ» ¿­¾î Á¤º¸¸¦ »ìÇÉ´Ù.
	DWORD	dwNTNVersion, dwNation, dwType;
	
	// ÆÄÀÏ ¹öÁ¯ ÀÐ±â
	fread( &dwNTNVersion, sizeof(DWORD), 1, pFile );
	if( dwNTNVersion == ON_NTNFILE_VERSION )
	{
		fread( &dwNation,	sizeof(DWORD), 1, pFile );			// ¼­ºñ½º ±¹°¡ ¹øÈ£
		fread( &dwType,		sizeof(DWORD), 1, pFile );			// ¼­ºñ½º Å¸ÀÔ
	}
	else
	{
		dwNation = dwType = 0xffffffff;
	}
	fclose( pFile );

	// °ËÁõ - ±¹°¡ ¹øÈ£
	switch( dwNation )
	{
		case DF_CODE_NATION_IDENTITY_KOREA			:	// ÇÑ±¹
		case DF_CODE_NATION_IDENTITY_JAPAN			:	// ÀÏº»
		case DF_CODE_NATION_IDENTITY_CHINA			:	// Áß±¹
		case DF_CODE_NATION_IDENTITY_TAIWAN			:	// ´ë¸¸
		case DF_CODE_NATION_IDENTITY_HONGKONG		:	// È«Äá
		case DF_CODE_NATION_IDENTITY_INDONESIA		:	// ÀÎµµ³×½Ã¾Æ
				iNation		= (int)dwNation;
		default			:
			break;
	}

	// °ËÁõ - Å¸ÀÔ
	iType	= (int)dwType;
	if( dwNation == DF_CODE_NATION_IDENTITY_KOREA && iType == 0 )	g_fTestVersion	= true;		// ÇÑ±¹ÆÇ¿¡ Å¸ÀÔÀÌ 0ÀÏ °æ¿ì ±¹³» Å×¼·
	else															g_fTestVersion	= false;	// ¾Æ´Ò °æ¿ì Å×¼·ÀÌ ¾Æ´Ï´Ù.

	if( iNation == -1 || iType == -1 )		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: CreateNTN()
// Code: actdoll (2004-03-12)
// Desc: ±¹°¡ Á¤º¸ ¹× ¼­¹ö Á¤º¸ ÆÄÀÏÀ» ¼³Á¤ÇÑ´Ù.
//		pszFileName	- ntnÆÄÀÏ¸í
//		iVersion	- ÀúÀåµÈ ¹öÁ¯
//		iNation		- ¼­ºñ½º ±¹°¡ ÄÚµå
//		iType		- ¼­ºñ½º Å¸ÀÔ
//		return		- true¶ó¸é Á¤»óÀûÀ¸·Î µ¥ÀÌÅÍ¸¦ È®ÀÎ, false¶ó¸é ÆÄÀÏÀÌ ¾ø°Å³ª Á¤º¸ ÃßÃâ ½ÇÆÐ
//-----------------------------------------------------------------------------
BOOL	CreateNTN( char *pszFileName, int &iNation, int &iType )
{
	FILE	*pFile;
	char	pszTemp[12] = {0,};

	// Å¸ÀÔÀ» ¸ÂÃçÁØ´Ù.
	if( iType == -1 )	iType = 1;

	// ÇöÀç ½Ã½ºÅÛÀÇ ±¹°¡ Á¤º¸¸¦ ÃßÃâÇÒ ¼ö ¾ø´Ù¸é OS±âº» Á¤º¸·Î °¡µ¿ÇÏ°í ¸®ÅÏÀÌ´Ù.
	if( iNation == -1 )
	{
		GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTCOUNTRY, pszTemp, sizeof(pszTemp) );
		iNation	= atoi( pszTemp );
		return FALSE;
	}

	// Á¦´ë·Î È®ÀÎµÆ´Ù¸é ¼¼ÆÃ ÆÄÀÏ ÀúÀå
	pFile	= fopen( pszFileName, "wb" );
	if( !pFile )	return FALSE;

	fwrite( &ON_NTNFILE_VERSION,	sizeof(DWORD), 1, pFile );			// NTN ÆÄÀÏ ¹öÁ¯
	fwrite( &iNation,				sizeof(DWORD), 1, pFile );			// ¼­ºñ½º ±¹°¡ ¹øÈ£
	fwrite( &iType,					sizeof(DWORD), 1, pFile );			// ¼­ºñ½º Å¸ÀÔ

	fclose( pFile );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetLocaleInfo()
// Code: actdoll (2004-03-12)
// Desc: ±¹°¡ Á¤º¸°ªÀ» ¹Þ¾Æ¼­ °ÔÀÓÀÇ ·ÎÄÉÀÏ °ªµéÀ» ¸ÂÃçÁØ´Ù.
//		iServiceNation	- ¼­ºñ½º ´ë»óÀÇ ±¹°¡(ÀüÈ­)¹øÈ£
//		return			- true¶ó¸é Á¤»óÀûÀ¸·Î µ¥ÀÌÅÍ¸¦ È®ÀÎ, false¶ó¸é Áö¿øÀ» ÇÏÁö ¾Ê´Â ±¹°¡(ÀüÈ­)¹øÈ£ÀÌ´Ù.
//-----------------------------------------------------------------------------
BOOL	SetLocaleInfo( int iServiceNation, int iServiceType, char *pszCustomFrontServerIP )
{
	// ¿¡·¯°ªÀÌ ¾Æ´Ï¶ó¸é ¼¼ÆÃ
	if( iServiceNation > 0 )		g_stConnectServerInfo.m_iServiceNation	= iServiceNation;
	if( iServiceType >= 0 )			g_stConnectServerInfo.m_iServiceType	= iServiceType;

	// ±¹°¡Á¤º¸ ¼³Á¤°ª¿¡ µû¶ó ¾ð¾î ÄÚµå¸¦ ¼³Á¤ÇØÁØ´Ù.
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

	// ÁÖ¾îÁø ¼­ºñ½º Áö¿ªÁ¤º¸¸¦ ±âÁØÀ¸·Î ³ª¸ÓÁö µ¥ÀÌÅÍ¸¦ »Ì¾Æ³½´Ù.
	FILE*	fp;
	OnlineCommonParser	ocp;
	char	pszFileName[MAX_PATH];

	sprintf( pszFileName, "%s\\ServiceListInfo.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
	fp = BindJXFile::GetInstance()->GetFile( pszFileName );
	if( !fp )	return FALSE;

	// actdoll (2004/08/23 14:10) : Ãâ·ÂµÉ ¿¡·¯ÆÄÀÏ¸í ÀûÀç
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", pszFileName );
	ocp.SetErrFileName( pszErrName );
	
	// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "ServiceListInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// ÀÚ·á °Ë»ö
	BOOL	bRet	= false;
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// µ¥ÀÌÅÍ ¶óÀÎ ÃßÃâ

		SI32	iLineNo, iColNo;

		// ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î ¹Þ´Â´Ù.
		char	pszDesc[48], pszTemp[128];
		SI32	siIndex, siNation, siType;
		ocp.GetValue( siIndex,			iLineNo, iColNo );		// ¹øÈ£
		ocp.GetValue( pszDesc,	47,		iLineNo, iColNo );		// ¼³¸í¹®
		ocp.GetValue( siNation,			iLineNo, iColNo );		// ¼­ºñ½º ±¹°¡ ¹øÈ£
		ocp.GetValue( siType,			iLineNo, iColNo );		// ¼­ºñ½º Å¸ÀÔ ¹øÈ£

		// ÀÏ´Ü ¿©±â±îÁö Ã£°í Á¤º¸°¡ ÀÏÄ¡ÇÏÁö ¾ÊÀ¸¸é ¸®ÅÏ
		if( siNation != iServiceNation || siType != iServiceType )	continue;
		bRet	= true;

		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ¹öÁ¯ ¼­¹ö IP
		// ¸¸¾à ¿©±â¼­ »ç¼³ IP°¡ ÀÖ´Ù¸é ±× °ªÀ¸·Î º¯°æ
		if( pszCustomFrontServerIP )	
		{
			strcpy( g_stConnectServerInfo.m_FrontServerIP, pszCustomFrontServerIP );
			ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ÇÁ·ÐÆ® ¼­¹ö IP
		}
		else
		{
			ocp.GetValue( g_stConnectServerInfo.m_FrontServerIP,			127,	iLineNo, iColNo );		// ÇÁ·ÐÆ® ¼­¹ö IP
		}
		ocp.GetValue( g_stConnectServerInfo.m_ServiceName,				127,	iLineNo, iColNo );		// ¼­ºñ½º ¾÷Ã¼¸í
		ocp.GetValue( g_stConnectServerInfo.m_ServiceURL,				511,	iLineNo, iColNo );		// ¼­ºñ½º ¾÷Ã¼ URL
		ocp.GetValue( g_stConnectServerInfo.m_ServiceCreateAccountURL,	511,	iLineNo, iColNo );		// µî·Ï ÆäÀÌÁö URL
		ocp.GetValue( g_stConnectServerInfo.m_ServiceShopName,			127,	iLineNo, iColNo );		// ¼îÇÎ¸ô ¸í
		ocp.GetValue( g_stConnectServerInfo.m_ServiceShopURL,			511,	iLineNo, iColNo );		// ¼îÇÎ¸ô ¾÷Ã¼ URL
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ±âÅ¸
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ±âÅ¸ URL

		break;
	}

	fclose( fp );
	ocp.Release();

	return bRet;
}

//-----------------------------------------------------------------------------
// Name: ReadIPEx()
// Code: actdoll (2004-06-01)
// Desc: ±¹°¡º° Á¤º¸ µ¥ÀÌÅÍº£ÀÌ½º ÆÄÀÏÀ» ºÐ¼®ÇÏ°í ÇØ´ç Á¤º¸¸¦ ÃßÃâÇØ ³½´Ù.
//		¸¸¾à ÇöÀç »ç³»Å×¼·¿ëÀ¸·Î µ¹°í ÀÖ´Ù¸é ¹«Á¶°Ç Ã¹Â°ÁÙ¸¸ ÀÐ´Â´Ù.
//		lpFileName				- ServiceInfo.cfg ÆÄÀÏ¸í
//		bUseHQ					- HQ³»ÀÇ ÆÄÀÏÀ» ÀÐÀ» °æ¿ì true, ÀÏ¹Ý ÆÄÀÏÀ» ÀÐÀ» °æ¿ì false
//		bJustReadFirstLine		- Å×½ºÆ®½Ã »ç¿ëµÇ´Â ÇÃ·¡±×. true ÀÏ °æ¿ì ÇØ´ç Á¤º¸ÆÄÀÏÀÇ Ã¹¹øÂ° ¶óÀÎ°ªÀ» µ¥ÀÌÅÍ·Î ³Ñ±ä´Ù. 
//								falseÀÏ °æ¿ì iNation, iType°ú ÀÏÄ¡ÇÏ´Â µ¥ÀÌÅÍ¸¦ Ã£¾Æ ¸®ÅÏÇÑ´Ù.
//		iNation					- ¼­ºñ½º ±¹°¡ ÄÚµå
//		iType					- ¼­ºñ½º Å¸ÀÔ
//		lpSConnectServerInfo	- Á¶°Ç¿¡ ¸Â´Â ±¹°¡ Á¤º¸¸¦ ¹ß°ßÇßÀ» ¶§ ÀúÀåµÉ ¹öÆÛ
//		return					- Á¦´ë·Î Ã£¾Æ³Â´Ù¸é true, ¹®Á¦°¡ ÀÖ´Ù¸é false
//-----------------------------------------------------------------------------
BOOL	ReadIPEx( char* lpFileName, BOOL bUseHQ, BOOL bJustReadFirstLine, int iNation, int iType, SConnectServerInfo* lpSConnectServerInfo )
{
	FILE*	fp;
	OnlineCommonParser	ocp;

	if( !lpFileName || !lpSConnectServerInfo )	return FALSE;

	// HQ¿¡¼­ ÆÄÀÏÀ» ¿¬´Ù.
	if( bUseHQ )
	{
		fp = BindJXFile::GetInstance()->GetFile( lpFileName );
		if( !fp )	return FALSE;
	}
	// ÀÏ¹Ý ÆÄÀÏÀ» ¿¬´Ù.
	else
	{
		fp = fopen( lpFileName, "rb" );
		if( !fp )	return FALSE;
	}

	// actdoll (2004/08/23 14:10) : Ãâ·ÂµÉ ¿¡·¯ÆÄÀÏ¸í ÀûÀç
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "ServiceInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// ÀÚ·á °Ë»ö
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	break;		// µ¥ÀÌÅÍ ¶óÀÎ ÃßÃâ

		SI32	iLineNo, iColNo;

		// ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î ¹Þ´Â´Ù.
		char	pszDesc[48], pszTemp[128];
		SI32	siIndex, siServiceNation, siServiceType;
		ocp.GetValue( siIndex,			iLineNo, iColNo );		// ¹øÈ£
		ocp.GetValue( pszDesc,	47,		iLineNo, iColNo );		// ¼³¸í¹®
		ocp.GetValue( siServiceNation,	iLineNo, iColNo );		// ¼­ºñ½º ±¹°¡ ¹øÈ£
		ocp.GetValue( siServiceType,	iLineNo, iColNo );		// ¼­ºñ½º Å¸ÀÔ ¹øÈ£

		//----------------------------------------------------------------------------
		// ÀÏ´Ü ¿©±â±îÁö Ã£°í Á¤º¸°¡ ÀÏÄ¡ÇÏÁö ¾ÊÀ¸¸é ¸®ÅÏ, Ã¹¶óÀÎ °ªÀ» ÃßÃâÇÏ¶ó´Â °ÍÀÌ ¾Æ´Ï¸é °è¼Ó ¼øÈ¯
		if( !bJustReadFirstLine )
		{
			if( iNation != siServiceNation || iType != siServiceType )	continue;
		}
		// ³Ñ¾î¿Ô´Ù¸é Ã£Àº °ÍÀÌ´Ù.
		lpSConnectServerInfo->m_iServiceNation		= siServiceNation;
		lpSConnectServerInfo->m_iServiceType		= siServiceType;
		//----------------------------------------------------------------------------

		ocp.GetValue( lpSConnectServerInfo->m_FrontServerIP,			127,	iLineNo, iColNo );		// ¹öÁ¯ ¼­¹ö IP
		ocp.GetValue( lpSConnectServerInfo->m_FrontServerIP,			127,	iLineNo, iColNo );		// ÇÁ·ÐÆ® ¼­¹ö IP
		ocp.GetValue( lpSConnectServerInfo->m_ServiceName,				127,	iLineNo, iColNo );		// ¼­ºñ½º ¾÷Ã¼¸í
		ocp.GetValue( lpSConnectServerInfo->m_ServiceURL,				511,	iLineNo, iColNo );		// ¼­ºñ½º ¾÷Ã¼ URL
		ocp.GetValue( lpSConnectServerInfo->m_ServiceCreateAccountURL,	511,	iLineNo, iColNo );		// µî·Ï ÆäÀÌÁö URL
		ocp.GetValue( lpSConnectServerInfo->m_ServiceShopName,			127,	iLineNo, iColNo );		// ¼îÇÎ¸ô ¸í
		ocp.GetValue( lpSConnectServerInfo->m_ServiceShopURL,			511,	iLineNo, iColNo );		// ¼îÇÎ¸ô ¾÷Ã¼ URL
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ±âÅ¸
		ocp.GetValue( pszTemp,											127,	iLineNo, iColNo );		// ±âÅ¸ URL
		
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
// Desc: °£´ÜÇÑ º¹»çÇü Ãâ·Â ´ÙÀÌ¾ó·Î±×¸¦ Ç¥½ÃÇÑ´Ù.
//		hInstance	- ÀÌ Å¬¶óÀÌ¾ðÆ®ÀÇ ÀÎ½ºÅÏ½º ÇÚµé
//		pszTitle	- ¸Þ½ÃÁö Å¸ÀÌÆ²
//		pszText		- º¹»ç °¡´ÉÇÑ Ãâ·Â ¸Þ½ÃÁö
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
// Desc: ¿î¿µÀÚ ¸í·É¾î µ¥ÀÌÅÍ¸¦ ¹Þ¾Æ ÇØµ¶ÇÏ¿© ¸®ÅÏÇÑ´Ù.
//		pDest				- ÀÎÄÚµù °ªÀ» ¹ÞÀ» ¹öÆÛ Æ÷ÀÎÅÍ
//		uiDestBuffSize		- pDest ¹öÆÛ Å©±â
//		dwNTNValue			- NTN ÄÚµå°ª
//		bSkipGameGuard		- °ÔÀÓ°¡µå ½ÇÇà ¾ÈÇÏ¸é true, ½ÇÇàÀÌ¶ó¸é false
//		bSkipLauncher		- ·±ÃÄ¾øÀÌ ½ÇÇàµÈ´Ù¸é true, ·±ÃÄ ¹Ýµå½Ã °ÅÄ¡°Ô ÇÑ´Ù¸é false
//		bSkipIntro			- ÀÎÆ®·Î ¾øÀÌ ·ÎµùÇÑ´Ù¸é true, ¾Æ´Ï¶ó¸é false
//		pszAACCode			- AACÄÚµå°ª
//		pszVersionServerIP	- ¼³Á¤ÇÒ ¹öÀü ¼­¹ö IP
//		pszFrontServerIP	- ¼³Á¤ÇÒ ÇÁ·ÐÆ® ¼­¹ö IP
//		pszPathCustomHQ		- ¼³Á¤ÇÒ ¿ÜºÎ HQText°¡ µé¾îÀÖ´Â Æú´õ¸í
//		return				- Á¤»óÀûÀ¸·Î ÀÛ¾÷Çß´Ù¸é pDest¿¡ µé¾îÀÖ´Â µ¥ÀÌÅÍÀÇ byte°ª, ¹®Á¦°¡ ÀÖ´Ù¸é 0
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

	// °ª Ç®±â
	char		pszData[3];
	for( UINT i = 0, uiLen = 0; i < uiSrcBuffSize; i += 2, uiLen++ )
	{
		memcpy( pszData, pSrc + i, 2 );
		pszData[2] = 0;
		*( pbtDummy + uiLen ) = (BYTE)strtol( pszData, NULL, 16 );
	}

	// ÀÎÄÚµù ÀÛ¾÷ °³½Ã
	DWORD	dwChkSum[8]	=
	{	
		0x8ac7d236,		0x135f47e6,		0xfcad6318,		0xa39e17bc,
		0xd5390148,		0x6e3f4d9a,		0x7efa143c,		0x957bd340,
	};

	// Ã¼Å©¼¶ ´ëÀÔÇÏ¿© ÇØÁ¦ÇÑ´Ù.
	DWORD	*dwPointer	= (DWORD*)pbtDummy;
	for( i = 0; i < uiLen >> 2; i++ )
	{
		*dwPointer	^=	dwChkSum[i % 8];
		dwPointer++;
	}

	// È®ÀÎ»ç»ì
	ZeroMemory( dwChkSum, sizeof(DWORD)*8 );

	uiLen		= 0;
	// NTN ÀûÀç(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	dwNTNValue					= (DWORD)wTemp;
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// AACÄÚµå ±æÀÌ ÀûÀç(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// AACÄÚµå ³»¿ë ÀûÀç(?B)
	memcpy( pszAACCode, pPointer, wTemp );
	pszAACCode[wTemp]			= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// °ÔÀÓ°¡µå ½ºÅµ? ÀûÀç(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipGameGuard				= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// ¹öÀü¼­¹ö IP ±æÀÌ ÀûÀç(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// ¹öÀü¼­¹ö IP ³»¿ë ÀûÀç(?B)
	memcpy( pszVersionServerIP, pPointer, wTemp );
	pszVersionServerIP[wTemp]	= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;
	
	// ·±ÃÄ ½ºÅµ? ÀûÀç(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipLauncher				= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// ÇÁ·ÐÆ®¼­¹ö IP ±æÀÌ ÀûÀç(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// ÇÁ·ÐÆ®¼­¹ö IP ³»¿ë ÀûÀç(?B)
	memcpy( pszFrontServerIP, pPointer, wTemp );
	pszFrontServerIP[wTemp]		= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// ÀÎÆ®·Î ½ºÅµ? ÀûÀç(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bSkipIntro					= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// HQÅØ½ºÆ® ±æÀÌ ÀûÀç(2B)
	memcpy( &wTemp, pPointer, sizeof(WORD) );
	pPointer					+= sizeof(WORD);
	uiLen						+= sizeof(WORD);

	// HQÅØ½ºÆ® IP ³»¿ë ÀûÀç(?B)
	memcpy( pszPathCustomHQ, pPointer, wTemp );
	pszPathCustomHQ[wTemp]		= 0;
	pPointer					+= wTemp;
	uiLen						+= wTemp;

	// À©µµ¸ðµå Çã°¡? ÀûÀç(1B)
	memcpy( &btTemp, pPointer, sizeof(BYTE) );
	bRunAsWindowMode			= (BOOL)btTemp;
	pPointer					+= sizeof(BYTE);
	uiLen						+= sizeof(BYTE);

	// ÇÃ·¡±× ÀÎÁõ°ª ÀûÀç(4B)
	memcpy( &dwTemp, pPointer, sizeof(DWORD) );
	pPointer	+= sizeof(DWORD);
	uiLen		+= sizeof(DWORD);
	if( dwTemp != DF_VALUE_ATI )		// ÇÃ·¡±× ÀÎÁõ°ªÀÌ ¸ÂÁö ¾Ê´Ù¸é ´×±â¸®!
	{
		uiLen	= 0;
	}

	// ÀÛ¾÷¿¡ ¾²¿´´ø °ª ÀüºÎ ÇØÁ¦
	ZeroMemory( pbtDummy, 4096 );
	ZeroMemory( pszData, 3 );

	return uiLen;
}

//-----------------------------------------------------------------------------
// Name: ShowErrorMsgBox()
// Code: actdoll (2004-03-12)
// Desc: °¢ ±¹°¡º°·Î ¹®Á¦°¡ µÈ ½ºÆ®¸µÀ» À©µµ ¸Þ½ÃÁö¹Ú½º ÇüÅÂ·Î Ãâ·ÂÇØÁØ´Ù.
//		iErrorCode		- ¿¡·¯ÄÚµå
//-----------------------------------------------------------------------------
void	ShowErrorMsgBox( int iErrorCode, ... )
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	BOOL				bFind = false;
	char				pszFullErrorMsg[2048] = {0,}, pszDummy[1024] = {0,};

	// ÀÏ´Ü ¸ÕÀú ¿¡·¯ÄÚµå ¹öÆÛ¿¡ ÄÚµå°ªÀ» ³Ö¾î³õ´Â´Ù.
	sprintf( pszFullErrorMsg, "Error Code - %08X\n\n", iErrorCode);

	// ±¹ÀûÀÌ Á¸ÀçÇÑ´Ù¸é HQ¿¡¼­ ÇØ´ç ¾ð¾î·Î µÈ ¿¡·¯ ÆÄÀÏÀ» ¿¬´Ù.
	if( g_stConnectServerInfo.m_iServiceLanguage > ELANGAGECODE_NONE && g_stConnectServerInfo.m_iServiceLanguage < ELANGAGECODE_MAX )
	{
		char	pszFileName[MAX_PATH];
		sprintf( pszFileName, "%s\\ErrorText.dat", DF_TEXT_SERVICE_NATION_LIST[g_stConnectServerInfo.m_iServiceLanguage] );
		if( fp = BindJXFile::GetInstance()->GetFile( pszFileName ) )
		{
			// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
			int		iRet, iErrLine;
			BOOL	bLess;
			if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) > 0 )
			{
				// ÀÚ·á °Ë»ö
				while( !bFind )
				{
					if( ocp.ParseDataLine() == 0 )	break;		// µ¥ÀÌÅÍ ¶óÀÎ ÃßÃâ

					SI32	iLineNo, iColNo;

					// ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î ¹Þ´Â´Ù.
					int		iCode;
					char	pszErrorText[1024], pszBuffer[1024];

					ocp.GetValue( iCode,				iLineNo, iColNo );		// ¹øÈ£
					ocp.GetValue( pszBuffer,	1023,	iLineNo, iColNo );		// ¼³¸í¹®

					// ÀÏ´Ü 1¹ø ¿¡·¯¸Þ½ÃÁö´Â ¹«Á¶°Ç ÀúÀåÇØ ³õ´Â´Ù. ¿¡·¯¸Þ½ÃÁö°¡ ¹ø¿ªÀÌ ¾ÈµÈ °æ¿ì¸¦ À§ÇØ¼­´Ù.
					if( iCode != iErrorCode )
					{
						if( iCode != 1 )		continue;
					}
					// ±×·¸Áö ¾ÊÀ» °æ¿ì ¿¡·¯ ¸Þ½ÃÁö¸¦ ÀçÆíÇÑ´Ù.
					else
					{
						sprintf( pszFullErrorMsg, "Error Code - %08X\n\n", iErrorCode);
						bFind	= true;
					}

					// Ãâ·Â±¸¹® ¼öÁ¤
					ConvertSpecialChar( pszBuffer );
					// ¿¡·¯¸Þ½ÃÁö¸¦ °»½ÅÇÑ´Ù.
					va_list		marker;
					va_start( marker, iErrorCode );
					vsprintf( pszErrorText, pszBuffer, marker );
					va_end( marker );     

					// ¸Þ½ÃÁö¸¦ º¹»çÇØ ³õ´Â´Ù.
					strcat( pszFullErrorMsg, pszErrorText );

					// ÇÑ ÁÙÀ» ¶¾´Ù
					strcat( pszFullErrorMsg, "\n\n\n" );
				}
				ocp.Release();
			}
			fclose( fp );
		}
	}

	// ´ÙÀ½Àº ¿µ¾î ±âÁØÀÇ ¿¡·¯ ÆÄÀÏÀ» ¿¬´Ù.
	if( fp = BindJXFile::GetInstance()->GetFile( "ErrorText_Eng.dat" ) )
	{
		// ÆÄ½ÌÀ» ½ÃÀÛÇÑ´Ù.
		int		iRet, iErrLine;
		BOOL	bLess;
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "ErrorText_Eng Error : Cannot Init! [ Ret-%d | Line-%d | Less-%d ]", iRet, iErrLine, bLess );
		}
		else
		{
			// ÀÚ·á °Ë»ö
			while(1)
			{
				if( ocp.ParseDataLine() == 0 )	break;		// µ¥ÀÌÅÍ ¶óÀÎ ÃßÃâ
				
				SI32	iLineNo, iColNo;
				
				// ÀÚ·á¸¦ ¼øÂ÷ÀûÀ¸·Î ¹Þ´Â´Ù.
				int		iCode;
				char	pszErrorText[1024], pszBuffer[1024];
				
				ocp.GetValue( iCode,				iLineNo, iColNo );		// ¹øÈ£
				ocp.GetValue( pszBuffer,	1023,	iLineNo, iColNo );		// ¼³¸í¹®
				
				// µ¥ÀÌÅÍ¸¦ Ã£Áö ¸øÇß´Ù¸é ±× ´ÙÀ½ÁÙÀ» °Ë»öÇÑ´Ù.
				if( iCode != iErrorCode )		continue;
				bFind	= true;

				// Ãâ·Â±¸¹® ¼öÁ¤
				ConvertSpecialChar( pszBuffer );
				// ¿¡·¯¸Þ½ÃÁö¸¦ °»½ÅÇÑ´Ù.
				va_list		marker;
				va_start( marker, iErrorCode );
				vsprintf( pszErrorText, pszBuffer, marker );
				va_end( marker );     

				// ¸Þ½ÃÁö¸¦ º¹»çÇØ ³õ´Â´Ù.
				strcat( pszFullErrorMsg, pszErrorText );
				break;
			}
			ocp.Release();
		}
		fclose( fp );
	}

	// ¸¸¾à ÇØ´ç ÀÎµ¦½º¸¦ ±âÁØÀ¸·Î ¸Þ½ÃÁö¸¦ ÇÏ³ªµµ ¹ß°ßÇÏÁö ¸øÇß´Ù¸é
	if( !bFind )
	{
		sprintf( pszFullErrorMsg, "ErrorCode [%08X]\nUnknown Error Message Type!", iErrorCode );
	}

	MessageBox( ( pGame ? pGame->Hwnd : NULL ), pszFullErrorMsg, "Gersang", MB_OK|MB_ICONWARNING|MB_TOPMOST );
}

//-----------------------------------------------------------------------------
// Name: ConvertSpecialChar()
// Code: actdoll (2004-03-12)
// Desc: OnlineText Å¬·¡½º¿¡ ÀÖ´Â ±â´É°ú µ¿ÀÏÇÑ ³ðÀÌ´Ù. 
//		¸Þ½ÃÁö Ã³¸®ÀÇ ÆíÀÇ¸¦ À§ÇØ ÀÏ´Ü ¿©±â¿¡¼­ µ¿¿øÇØ ¾´´Ù.
//		iErrorCode		- ¿¡·¯ÄÚµå
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
