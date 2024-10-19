//#ifndef _ROCKET_NEW_TILE_MAP

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <Mouse.h>
#include <MainFunction.h>

#include <clGame.h>
#include "clItemInfoWindow.h"

#include <setchar.h>
#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <etc.h>
#include <item.h>
#include "..\Gersang\crop.h"
#include "..\Gersang\screenpoint.h"
#include "..\Gersang\music.h"
#include <SoundManager.h>
#include <chat.h>

#include "..\Gersang\mapedit.h"
#include "..\Gersang\stage.h"
#include "../SCCommon/OnlineBattleMsg.h"
#include "DebugTool.h"

#include <map.h>
#include <volcano.h>


#include <OnlineImjin2i.h>

#include <OnlineWorld.h>
#include <OnlineNoticeList.h>

#include <OnlineBattleMsg.h>
#include "..\Gersang\directx.h"
#include "..\Gersang\text.h"
#include "..\Gersang\help.h"
#include "..\Gersang\yprg.h"
#include <CharOrg.h>

#include <yPutGetResource.h>

#include <ysmap.h>

#include <map.h>
#include <mapinterface.h>
#include <charinterface.h>
#include <magic.h>
#include <GameButton.h>
#include <bullet.h>
#include <smoke.h>
#include <Game2.h>
#include <onlineImjin2i.h>
#include <MyNet-Game.h>
#include <myfile.h>
#include "bindJxFile.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME 컨트롤러 세팅

// OnlineBattleInfo의 정보가 유효한지 알려준다. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo);

extern _clGame *pGame;


extern	SHORT 				GamePauseSwitch;					// 게임이 잠시 중단되었는가?
extern  _Chat 				Chat;
extern	RECT				ScreenRect;							// 지도가 표시되는 영역 
extern	_ScreenPoint 		SP;									// 화면에 사용될 여러 정보 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	_YSMap 				SMap;								// 작은 지도용 데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	XSPR 				PannelSpr;
extern	SHORT 				MapEditMode;						// 지도 편집 모드인가?
extern	_MapEdit 			MapEdit;
extern	SHORT 				GameMenuShowSwitch;					// 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	DWORD				PrevMultiClock;
extern	DWORD				MessageSendFrame;					// 명령이나 NO_MESSAGE를 보낸 프레임 
extern	BOOL				GameDropSwitch ;
extern	int 				DragSwitch;
extern	int 				MapDragSwitch;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern	DWORD 				GameRecordSwitch;
extern	clock_t				StartClock, CurrentClock;						// 시간 관련 
extern	_MyNetGame			MyNet;
extern	SI32				MaxMapXsize;
extern	SI32				MaxMapYsize;
extern	char 				SavePath[];
extern	UI08 				ScreenMapBuffer[];					// 화면에 찍을 지도를 임시로 저장하는 곳 
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				MapX, MapY;							// 마우스가 위치한 small 좌표 
extern	int 				DragStartX, DragStartY, DragEndX, DragEndY;
extern	_FileVary 			FV;
extern	_Help 				Help;
extern	SHORT				GameStatus;							// 현재의 게임 상황을 알리는 변수 
extern	SHORT				GameStatusBuffer;
extern	int					bActive;							// 현재 액티브인지 아닌지 알려주는 변수  
extern _InputDevice			IpD;
extern	int					CDStopSwitch;						// CD음악을 정지 시킨다. 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	int 				MouseMode;							// 마우스가 처한 상태를 저장 
// 키보드 관련
extern	int 				ControlKeyPressSwitch;
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameRealX, GameRealY;
extern	SHORT 				UnderMouseCropType;
extern	SHORT 				UnderMouseCropMount;
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	int 				MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
extern	_NewID 				UnderMouseID;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	DWORD 				GameWParam;
extern	int 				DragDoneSwitch;
extern	int 				MenuMode;							// 메뉴의 상태를 저장 
extern	BOOL 				IsMosueInControlSwitch;				// 마우스가 게임 콘트롤위에 위치하고 있다.
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_Order 				Order[];
extern	_Text 				Text;								// 텍스트 
extern	SHORT 				MouseHarvestX, MouseHarvestY;
extern	SHORT 				MenuOfMouse;						// 마우스 커서가 위치하고 있는 메뉴의 번호 
extern	_NewID 				ZeroID;
extern  char 				UserSelectMapFileName[];			// 사용자가 선택한 지도 이름을 저장할 공간   
extern	int   				MultiPlayDelayTime;
extern  BOOL				bGameAllyWindowSwitch;

extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;			// 캐릭터 그림용
extern LPDIRECTDRAWSURFACE7 lpPannelSurface;		// 판넬용 면 

extern BOOL IsMouseInPannelSwitch  ;
extern BOOL	IsAllyWindowSwitch;

extern	_OnlineBattleMsgQue OBMQue;


extern OnlineBattleInfo OBInfo;
// 최대 프레임 이격 
extern SHORT DPMaxFrameDiff;

extern clock_t FrameDelay ;   // 프레임당 클럭
extern int FrameClockDelay;

extern SoundManager	*g_pSoundManager;

static DWORD PrevFrameClock;
static DWORD PrevGameWinConditionFrame;
static BOOL IsMouseInPannelSwitch   = FALSE;
static BOOL IsAllyWindowSwitch      = FALSE;
static BOOL ScrollProcSwitch;

int ObjectTempSelectedSwitch=FALSE;
int Scroll;
SHORT ScrollDoneSwitch=FALSE;  // 스크롤이 이루어졌는가?
SHORT DoubleClickedID;  // 더블 클릭으로 선택된 아이디 

BOOL IsMouseInMapSwitch      = FALSE;
BOOL IsMouseInBigMapSwitch   = FALSE;
BOOL IsMouseInSmallMapSwitch = FALSE;

static SI32 BattleMusic	= 0;
OnlineNoticeList *pOnlineNoticeList;

clock_t FrameDelay = 0;   // 프레임당 클럭

static BOOL GameQuitSwitch = FALSE;

extern int WindowModeSwitch;
extern HDC g_hBattleScreenDC;

#define MAX_SET_CHAR_POINT_NUMBER   21
POINT SetCharPoint[MAX_SET_CHAR_POINT_NUMBER]=
{
	{0,0},		{0,2},	{-2,0}, {0,-2}, {2,0}, 
	{-2,-2},	{2,-2}, {-2,2}, {2,2},  {0, -4},
	{4,0},		{0,4},	{-4,0}, {2,-4}, {-2,-4},
	{2,4},		{-2,4}, {-4,2}, {-4,-2},{4,2},
	{4,-2},
};

#define SCROLL_RANGE         6

void InitGame1()
{

	// 팔레트를 설정한다. 
	SetGamePalette(clGrp.Palette);
	InitDarkTable(clGrp.Palette);
	SetMouse(MOUSE_NORMAL);

	

	if(pGame == NULL)
	{
		clGrp.Error("Error", "fdei82hsdas");
	}
	else if(pGame->pBattle == NULL)
	{
		clGrp.Error("Error", "fdei82hsdas11");
	}
	else
	{
		pGame->pBattle->InitGameButton();
	}


	// 이전에 게임이 중단되어 있었다면 속개한다. 
	GamePauseSwitch=FALSE;
	bGameAllyWindowSwitch = FALSE;

	// 이전에 텍스트 입력 모드가 설정되어 있었다면 해제한다. 
	Chat.SetInputModeSwitch(FALSE, 0);


	// 사용자의 서피스를 제거한다. 
	CustomSurfaceRelease();
	
	// 임의의 서피스를 다시 만든다. 
	CreateSurface();


	// 화면의 영역을 설정한다. 
	ScreenRect.left=0;
	ScreenRect.right=CLIENT_SCREEN_XSIZE-1;
	ScreenRect.top=0;
	ScreenRect.bottom=CLIENT_SCREEN_YSIZE-1;

	clGrp.SetClipArea((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, (SHORT)ScreenRect.right, (SHORT)ScreenRect.bottom);

	SP.Init();

}



void InitGame2()
{
	SHORT i, j, k;


	// 작은 지도를 초기화 한다.
	SMap.Init(Map.GetXsize(), Map.GetYsize());
	
	
	Map.MakeSmallMapBase();

	
	if(GD.GameMode == GAME_MODE_ONLINEBATTLE)
	{
		//작은 지도를 항상 유효화 시킨다. 
		GD.SetSmallMapValidSwitch(TRUE);

		// 자동 마법 사용 불가. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// 모든 업그레이드 완료한다.
		GD.SetUpgradeAllSwitch(TRUE);

		// 온라인 게임의 스피드로 설정한다. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// 캐릭터들의 시야를 직선으로 처리힌다. 
		GD.SetStraightSightMode(TRUE);

	}
	if(GD.GameMode == GAME_MODE_HERO)
	{
		//작은 지도를 항상 유효화 시킨다. 
		GD.SetSmallMapValidSwitch(TRUE);

		// 자동 마법 사용 불가. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// 모든 업그레이드 완료한다.
		GD.SetUpgradeAllSwitch(TRUE);

		// 온라인 게임의 스피드로 설정한다. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// 캐릭터들의 시야를 직선으로 처리힌다. 
		GD.SetStraightSightMode(TRUE);

	}


	LoadPannel(GD.Player[UserPlayerNumber].PI.Country);


	// 판넬을 surface로 옮긴다.
	if(clGrp.LockSurface(lpPannelSurface)==TRUE)
	{
		clGrp.PutImageT(0, 0, 
		          PannelSpr.Header.Xsize, 
			      PannelSpr.Header.Ysize,
		          PannelSpr.Image);

		clGrp.UnlockSurface(lpPannelSurface);
	}

	if(MapEditMode==TRUE)
	{
		MapEdit.Init();
	}


	// 설정된 값에 따라 기본적인 속도를 얻어온다.
	FrameDelay = pGame->Config.GetRealGameSpeedClock();

	// 키버퍼를 초기화하여 이전의 입력을 무시한다. 
	GD.KB.InitKeyBuffer();

	// 게임 메뉴를 보이지 않도록 만든다. 
	GameMenuShowSwitch=FALSE;

	InitFrameClock();

	InitGetResource();

	PrevGameWinConditionFrame = 10000;
	PrevMultiClock   = 0;
	MessageSendFrame = 0;  // 명령이나 NO_MESSAGE를 보낸 프레임 

	GameDropSwitch = FALSE;


	// 지도 드래깅을 초기화한다. 
	DragSwitch = FALSE;
	MapDragSwitch = FALSE;
	GameLeftPressSwitch = FALSE;
	GameRightPressSwitch = FALSE;

	// 영어 모드
	ChangeLanguage(FALSE);


	if(MapEditMode == TRUE)
	{
		for(i=0;i<Map.GetYsize();i++)
			for(j=0;j<Map.GetXsize();j++)
			{
				GD.BlackMap[UserPlayerNumber][j][i]=0;
			}
	}
	else
	{
		// 관전 모드이면 모든 화면을 다 본다. 
		for(k=0;k<MAX_MULTI_PLAYER;k++)
		{
			if(GD.Player[k].PI.Country == COUNTRY_NEUTRAL)
			{
				for(i=0;i<Map.GetYsize();i++)
					for(j=0;j<Map.GetXsize();j++)
					{
						GD.BlackMap[k][j][i]=0;
					}
			}
		}
	}

	if(GD.Player[UserPlayerNumber].PI.Country == COUNTRY_NEUTRAL)
	{
		//작은 지도를 항상 유효화 시킨다. 
		GD.SetSmallMapValidSwitch(TRUE);
	}		
	


	// 빠른 게임 전개 모드이면 모든 업그레이드를 다 완료시킨다. 
#ifdef _QUICKMODE
	GD.SetUpgradeAllSwitch(TRUE);
#endif

	PrevMultiClock = CurrentClock;

}


// 게임에 들어가기 전에 초기화해주는 함수 
//BOOL InitNoStageGame()
//{
//	SI32 userpointindex = 0;
//
//	// 게임의 주 데이터를 초기화한다. 
//	GD.Init();
//	InitVolcano();
//
//	// 멀티세팅에서 값이 나오면, 
//   GD.RandomSeed = 1;
//
//	// 게임 방식을 정한다. 
//	GD.GameMode = GAME_MODE_NORMAL;
//
//	Map.InitMap(100, 100, FALSE);
//
//	
//	Map.SetMapDrawSwitch(TRUE);
//
//	// 지도용 화일을 불러온다. 
//	Map.MH.MapType = MAP_TYPE_NORMAL;
//	LoadMapTile(Map.MH.MapType);
//	
//	// 화면을 시작위치로 옮긴다. 
//	Map.VirtualMapX=0;
//	Map.VirtualMapY=0;
//	
//	GD.HomeX=Map.VirtualMapX;
//	GD.HomeY=Map.VirtualMapY;
//
//	return TRUE;
//}


BOOL InitOnlineBattleGame()
{

	SHORT i, j;
	char buffer[FILE_NAME_SIZE];
	SI32 userpointindex;

	MyNet.Init();

	// 게임의 주 데이터를 초기화한다. 
	GD.Init();
	InitVolcano();



/////////////////////////////////////////////////////////////////////////////////////

	// 내 컴의 아이디. 
	OBInfo = GOnlineImjin2I.BattleInfo;
	
	
	UI32 size = 0;
	UI32 startclock = timeGetTime();
	
	
	// 지연을 근거로 각종 통신 변수들을 설정한다. 
	MyNet.DecideTransVary(300);
	
	// 전투에 필요한 정보를 얻어온다.
	
	// 잘못된 정보가 없는지 확인한다. 
	if(MapEditMode == FALSE)
	{
		CheckOnlineBattleInfo(&OBInfo);
	}
	
	// 플레이어의 구체적인 정보를 얻어온다. 
	
	
	
	for( i = 0; i < MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.dpId[i] = OBInfo.OIBPI[i].siPlayersUniqueID;
		
		// 사용자, 혹은 몬스터인경우. 
		if(OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			// 사람인 경우에만 이름이 있다.
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				
				strncpy(MyNet.PlayersName.Name[i], OBInfo.OIBPI[i].szPlayerName[0], ON_ID_LENGTH);
				
				// 널문자를 넣어준다. 
				MyNet.PlayersName.Name[i][ON_ID_LENGTH] = NULL;
				
			}
			else
				strcpy(MyNet.PlayersName.Name[i], "MONSTER");
		}
	}

	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.Player[i].Init(i);

		// 유효한 플레이이어인 경우 , 
		if(MyNet.PlayersdpId.dpId[i]) 
		{
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				MyNet.PlayersdpId.Player[i].Host	= BY_PLAYER;
				MyNet.PlayersdpId.Player[i].Country = COUNTRY_KOREA; 				
			}
			else if(ISMONSTER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				MyNet.PlayersdpId.Player[i].Host	= BY_COMPUTER;
				MyNet.PlayersdpId.Player[i].Country = COUNTRY_KOREA;   
			}
			else if(ISSHIP(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
			}
			
			// 자기 자신은 자신에게 동맹이다. 
			GD.OnlineAllyNumSwitch[i][i] = TRUE;
		}
	}

	// 서버는 항상 1이다. 
	MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1]				= 1;
	MyNet.PlayersdpId.Player[MAX_MULTI_PLAYER-1].Country	= COUNTRY_NEUTRAL;
	MyNet.HostPlayerDpId = MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1];;


	MyNet.PlayersdpId.GameMode= GAME_MODE_NORMAL;

	// 사용자의 번호를 구한다. 
	for(i = 0;i < MAX_MULTI_PLAYER;i++)
	{
		if(GOnlineImjin2I.BattleInfo.siMyCharUniqueID == OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			UserPlayerNumber = i;
		}
	}
	if(MapEditMode)
		UserPlayerNumber = 0;
	
	MyNet.PlayersdpId.Player[UserPlayerNumber].Host = BY_PLAYER;


/////////////////////////////////////////////////////////////////////////////////////

	// 랜덤 시드를 얻어온다.
	if(MapEditMode == TRUE)
	{
		GD.RandomSeed		= 1;
	}
	else
	{
		GD.RandomSeed		= OBInfo.uiRandomSeed;
	}

	if(GD.RandomSeed	== 0)
	{
		clGrp.Error("3209873", "3210847");
		GD.RandomSeed = 1;
	}


   //////////////////////////////
   // NPC간은 동맹으로 설정한다. ///
   //////////////////////////////
   
   SI32 mon_player = 0;
	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		// 유효한 플레이이어인 경우 , 
		if(MyNet.PlayersdpId.dpId[i]) 
		{
			// NPC간은 동맹을 설정한다. 
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == FALSE)
			{
				for( j = 0; j < MAX_MULTI_PLAYER; j++ )
				{
					if(ISPLAYER(MyNet.PlayersdpId.dpId[j]) == FALSE)
					{
						GD.OnlineAllyNumSwitch[i][j] = TRUE;
						GD.OnlineAllyNumSwitch[j][i] = TRUE;
					}
				}
			}
		}
	}

	// 플레이어 간에는  동맹을 맺는다. 
	// PK가 허용되지 않는 필드에서만..(허용되는 필드는 아무하고도 동맹하지 않는다.)
	if(pGame->pOnlineWorld && pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
	{
		if(MyNet.PlayersdpId.dpId[0] && MyNet.PlayersdpId.dpId[1])
		{
			// 플레이어간에 전투를 시작했을때(PK)
			if(ISPLAYER(MyNet.PlayersdpId.dpId[1]) == TRUE)
			{
				for(i=0; i<(MAX_MULTI_PLAYER-1); i++)
				{
					if(!MyNet.PlayersdpId.dpId[i])				continue;
					if(!ISPLAYER(MyNet.PlayersdpId.dpId[i]))	continue;
					for(j=0; j<(MAX_MULTI_PLAYER-1); j++)
					{
						if(!MyNet.PlayersdpId.dpId[j])			continue;
						if(!ISPLAYER(MyNet.PlayersdpId.dpId[j]))continue;

						BOOL iHostility = pGame->pOnlineWorld->pIOnlineChar->GetIsHostilityGuild(MyNet.PlayersdpId.dpId[i]);
						BOOL jHostility = pGame->pOnlineWorld->pIOnlineChar->GetIsHostilityGuild(MyNet.PlayersdpId.dpId[j]);
						if((iHostility&&jHostility)||(!iHostility&&!jHostility))
							GD.OnlineAllyNumSwitch[i][j] = TRUE;
					}
				}
			}
			// 몹과의 전투
			else
			{
				for(i=0; i<(MAX_MULTI_PLAYER-1); i++)
				{
					if(!MyNet.PlayersdpId.dpId[i])				continue;
					if(!ISPLAYER(MyNet.PlayersdpId.dpId[i]))	continue;
					for(j=0; j<(MAX_MULTI_PLAYER-1); j++)
					{
						if(!MyNet.PlayersdpId.dpId[j])			continue;
						if(!ISPLAYER(MyNet.PlayersdpId.dpId[j]))continue;

						GD.OnlineAllyNumSwitch[i][j] = TRUE;
					}
				}
			}
		}
	}

	// 게임 방식을 정한다. 
	GD.GameMode = GAME_MODE_ONLINEBATTLE;
	
	char mapname[128]="";
	
	// 지형의 타입을 얻어온다. 
	if(MapEditMode == TRUE)
	{
		Map.MH.MapType = MAP_TYPE_NORMAL;
		
		Map.InitMap(100, 100, FALSE);
		
	}
	else
	{
		
		// 게임에 등장하는 플레이어가 몇 명인지 확인한다. 
		SHORT playercount=0;
		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			// 등장하지 않는 국가이면, 
			if(MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NO
				&& MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NEUTRAL)
			{
				playercount++;
			}
		}
		
		// 플레이어 숫자가 비정상적이면 알린다. 
		if(playercount < 2)
		{
			clGrp.Error("f9837521", "[%d]", playercount);
		}
		
		SI32 maptype = pGame->pOnlineWorld->GetWarFieldMap();
		if(FindProperOnlineBattleMap(playercount, maptype, mapname) == TRUE)
		{
			
			// 지도를 초기화한다.
			//				strcpy(buffer, SavePath);
			strcpy(buffer, "Online\\BattleMap\\");
			strcat(buffer, mapname);
			
			Map.Load(buffer);
		}
		else
		{
			clGrp.Error("125308", "no map [%d]", playercount);
		}
		
		// 확장 기지를 설정한다.
		Map.ExpansionBaseNumber=0;
		for(i = 0;i < MAX_MULTI_PLAYER;i++)
		{
			if(Map.clMapPlayerInfo[i].StartPoint.x >= 0 && Map.clMapPlayerInfo[i].StartPoint.y >= 0)
			{
				Map.PushExpansionBasePoint((SHORT)Map.clMapPlayerInfo[i].StartPoint.x, (SHORT)Map.clMapPlayerInfo[i].StartPoint.y);
			}
		}
		
	}

	//시작하는 위치를 랜덤하게 바꾼다.
	Map.RandomStartPos();
	
	// 지도용 화일을 불러온다. 
	LoadMapTile(Map.MH.MapType);
	Map.SetMapDrawSwitch(TRUE);

    // 불러오거나 임의로 설정된 지도를 최종적으로 확정한다. 
    Map.MakeLastMap();

	   // 세팅에서 정해진 설정을 넘겨준다. 
	   for(i=0;i<MAX_MULTI_PLAYER;i++)
	   {
		   SHORT pnumber=MyNet.PlayersdpId.Player[i].PlayerNumber;
		   GD.Player[pnumber].PI=MyNet.PlayersdpId.Player[i];
	   }
	   
	   
	   // EnemyBit를 설정한다. 
	   for(i=0;i<MAX_MULTI_PLAYER;i++)
	   {
		   GD.Player[i].PI.MyBit=(1<<GD.Player[i].PI.PlayerNumber);
		   
		   for(j=0;j<MAX_MULTI_PLAYER;j++)
		   {
			   if(i==j)continue;
			   
			   if(GD.IsFriendTo(i, j) == FALSE)
			   {
				   GD.Player[i].PI.EnemyBit|=(1<<j);
			   }
		   }
	   }

	   InitGameSetCharOnline(userpointindex);


	 // 중립국이면, 
	 if(GD.Player[UserPlayerNumber].PI.Country==COUNTRY_NEUTRAL)
	 {
		 // 화면을 시작위치로 옮긴다. 
		 Map.VirtualMapX=0;
		 Map.VirtualMapY=0;
		 
		 GD.HomeX=Map.VirtualMapX;
		 GD.HomeY=Map.VirtualMapY;
	 }
	 else
	 {
		 
		 // 화면을 시작위치로 옮긴다. 
		 if(userpointindex >=0 )
		 {
			 Map.VirtualMapX=Map.clMapPlayerInfo[userpointindex].StartPoint.x;
			 Map.VirtualMapY=Map.clMapPlayerInfo[userpointindex].StartPoint.y;
		 }
		 else
		 {
			 Map.VirtualMapX = 0;
			 Map.VirtualMapY = 0;
		 }

		 
		 GD.HomeX=Map.VirtualMapX;
		 GD.HomeY=Map.VirtualMapY;
	 }

	 // 팀정보를 불러서 업데이트 한다. 
	 pGame->Config.GetTeamInfo();

#ifdef _OGG_PLAY
	 GetBgmMgr()->Play(OGGBGM_FILENAME_WARTITLE, TRUE);
#else
	 // 전투용 음악을 연주한다.
	 if(g_pSoundManager)
	 {
		 g_pSoundManager->StopAllSound();
		 
		 BattleMusic	=	g_pSoundManager->AddPlayID( "music\\Music3.YAV", FALSE, TRUE, FALSE );
		 g_pSoundManager->PlaySound( BattleMusic );	
	 }
#endif

 	// 상단 메시지를 처리할 수 있는 클래스의 포이터를 얻어온다. 
	pOnlineNoticeList = pGame->GetOnlineNoticeListPointer();

	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->SetPosition(233, 582, 335,4);
	}

	// 일찍 퇴각하면 경험치를 잃는다는 경고를 알려준다.
	{
		char buffer[128];

		if(pGame->pOnlineWorld)
		{
			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
				wsprintf(buffer, Text.RetreatWarningInBattleArea.Get(), RETREAT_PERMIT_CLOCK/1000);
			else
				wsprintf(buffer, Text.RetreatWarningInPeaceArea.Get());
			
			SetMessage(buffer);
		}
	}

	Chat.Initialize();

	// ORDER_QUIT 명령은 한번만 실행되어야 하기 때문에 게임초기에 다음의 스위치를 초기화한다. 
	GameQuitSwitch = FALSE;

	// 아이템 획득 정보창을 없앤다. 
	pGame->pItemInfoWindow->ClearAllItemInfoWindow();



	return TRUE;
}

void _clGame::ChatInit()
{
	Chat.Init();
}




BOOL GameDraw1()
{

	//  이동마크의행동 
	GD.MoveMark.Action();

	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

       // 지도를 옮긴다. 
	   clGrp.PutImage32(0, 0, CLIENT_SCREEN_XSIZE, CLIENT_SCREEN_YSIZE, ScreenMapBuffer);
	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	return TRUE;

}


BOOL GameDraw2()
{
	HDC		hdc;
	
	SI32 i = 0;
	
	
	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{
		
		// 캐릭터를 포함한 객체들을 그린다. 
		Map.DrawObj();
		
		
		// 지도 편집 상태이면 플레이어 시작 위치도 그려준다. 
		if(MapEditMode==TRUE)
		{
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				// 유효한 플레이어 위치라면, 
				if(Map.clMapPlayerInfo[i].StartPoint.x>=0 && Map.clMapPlayerInfo[i].StartPoint.y>=0)
					Map.DrawPlayerStartPoint((SHORT)Map.clMapPlayerInfo[i].StartPoint.x, (SHORT)Map.clMapPlayerInfo[i].StartPoint.y, i);
			}
		}
		clGrp.UnlockSurface(lpScreenBuffer);
	}

	if(lpScreenBuffer->GetDC(&hdc) == DD_OK)
	{
		DrawGetResource(hdc);
		
		lpScreenBuffer->ReleaseDC(hdc);
	}
	

	
	return TRUE;
}

BOOL GameDraw3()
{
	SI32 i=0, j=0;
	HRESULT hResult = 0;
	SI32	siFile = 0;
	HDC hdc;


	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{
	   
	   // 지도 편집 모드이면, 
	   if(MapEditMode==TRUE)
	   {
		   if(MapEdit.TerrainMode==MAPEDIT_MODE_UNIT && Map.IsPtInMap(GameMouseX, GameMouseY)==TRUE)
			   Map.DrawBuilding(MapEdit.SelectedKind, MapX, MapY, TRUE, 0);
		   
		   SHORT centerx, centery;
		   if(Map.GetRefXY(MapX, MapY, centerx, centery)==TRUE)
		   {
			   Map.DrawMousePos(centerx, centery, MapX, MapY, 0, FALSE);
		   }

	   }

	   // 여여문의 지뢰 매설에는 마우스 위치를 알려주는 것이 같이 다닌다.
	   if(GD.SelectedOrder==ORDER_MINE)
	   {
		   SHORT centerx, centery;
		   
		   // 지도 편집 모드에서 마우스 위치를 표시한다. 
		   //if(Map.GetCenterXYByMapCenterXY(MapX, MapY, centerx, centery)==TRUE)
		   if(Map.GetRefXY(MapX, MapY, centerx, centery)==TRUE)
		   {
	          Map.DrawMousePos(centerx, centery, MapX, MapY, 0, TRUE);
		   }

	   }

	   // 비와 불을 그린다.
	   GD.Rain.Draw();


	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	

    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

		if(MapDragSwitch==TRUE)
		{
		   Map.DrawDragBox(DragStartX, DragStartY, DragEndX, DragEndY);
		}

	   // 이동 마크 찍기 
       GD.MoveMark.DrawMoveMark();

 
	   // 로고 그리기 
	   Draw_PutSpriteLightT(clGrp.GetScreenXsize()-GetFileXsize(FV.OnlineLogoFile) - 10, 
		   ScreenRect.bottom-GetFileYsize(FV.OnlineLogoFile)-70, 
		   FV.OnlineLogoFile,0);	// hojae_append
	   
	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	   // 아이템 획득 창을 보여준다. 
	 pGame->pItemInfoWindow->Render(lpScreenBuffer);



	// 판넬에 작은 지도를 그린다. 
	DrawSmallMapImjin();
	
	//채팅창 그리기.
	pGame->m_ChatList.Process(lpScreenBuffer, 1);

	// 판넬을  그리기 
	DrawPannel();

	// 입력중인 문장을 표시해준다. 
	Chat.DrawInputText();

	// 작은지도에서 화면영역을 나타내는 작은 사각형을 그린다. 
	if(IsSmallMapValid())
	{
	    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
	       SMap.DrawBox(Map.VirtualMapX, Map.VirtualMapY);
	       clGrp.UnlockSurface(lpScreenBuffer);
		}
	}

	// 메뉴를 그려준다. 
	DrawMenu(lpScreenBuffer);

	// 메시지가 있으면 표시한다. 
	ShowMessage(10, 70, lpScreenBuffer);

	// 남은 시간을 표시한다. 
	if(GetPlayTime() == TRUE)
	{
		if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
			siFile = FILE_ON_BASE_TIMER;
			Draw_PutSpriteLightT(12,8,siFile,0);
			clGrp.UnlockSurface(lpScreenBuffer);
		}
	}
	ShowPlayTime(38, 38, lpScreenBuffer);

	// 상단 메시지 처리. 
	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->Poll(lpScreenBuffer);
	}
	// ===============================================================================
//	if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
	// ===============================================================================
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	// ===============================================================================
	{
		// 상단 메시지를 출력한다. 
		if(lpScreenBuffer->GetDC(&hdc)==DD_OK)
		{
			
			if(pOnlineNoticeList)
			{
				SelectObject(hdc, pGame->pBattle->HelpFont);
				SetBkMode(hdc, TRANSPARENT);
				
				pOnlineNoticeList->Draw(hdc);
			}
			
			lpScreenBuffer->ReleaseDC(hdc);
		}
	}
	
	// 마우스 밑의 곡물 정보를 표시한다. 
	ShowCropInfo(lpScreenBuffer);

	// 지도 편집 모드에서는 정보를 표시한다. 
	if(MapEditMode==TRUE)
	{
 		MapEdit.MapEdit_DrawInfo();
    }

	return 0;
}


BOOL GameDraw4()
{
	static RGBQUAD prgb[256];
	HRESULT hResult =0;

	// 도움말 정보를 보여준다. 
	Help.Draw();

    //////// lpDDSBack를 lpDDSPrimary로 복사한다.
	DrawMouse(lpScreenBuffer);

	
	GD.EarthQuake.Action();

	RECT rect;
	rect.left   = 0;
	rect.right  = clGrp.GetScreenXsize();
	rect.top    = GD.EarthQuake.GetOffsetY();
	rect.bottom = clGrp.GetScreenYsize();

	// actdoll (2004/11/03 17:28) : IME 컨트롤러 관련...
	//	안좋은 방법이지만 배치가 중구난방이라 출력코드를 여기로 잡을 수 밖에 없다.
	OnlineInputStatusControl::Draw( lpScreenBuffer );

	if (WindowModeSwitch)
	{

		GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );
			
		for(int i=0; i<256; i++ )
		{
			prgb[i].rgbRed		= clGrp.PalEntries[i].peRed;//Palette[i*3] << 2;
			prgb[i].rgbGreen	= clGrp.PalEntries[i].peGreen;//Palette[i*3+1] << 2;
			prgb[i].rgbBlue		= clGrp.PalEntries[i].peBlue;//Palette[i*3+2] << 2;
			prgb[i].rgbReserved	= 0;
		}

		SetDIBColorTable(g_hBattleScreenDC,0,255,(RGBQUAD *)prgb);		

		pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);
		
	}
	else 
	{
		
		if(clGrp.lpClipper)
		{
			hResult = clGrp.lpDDSPrimary->Blt(NULL, lpScreenBuffer, &rect, DDBLT_WAIT, NULL);
		}
		else
		{
			hResult = clGrp.BltFast(SURFACE_PRIMARY, 0, 0, lpScreenBuffer, &rect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
		}
	}


	// 서피스가 로스트된 것이면, 
	if(hResult == DDERR_SURFACELOST)
	{
		clGrp.Restore();
	}


	if (!WindowModeSwitch) {
		if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
			DrawMouseBuffer();
			clGrp.UnlockSurface(lpScreenBuffer);
		}
	}

	return TRUE;
}



BOOL IsSmallMapValid()
{

//	if(Cheat.CheatSeeSwitch==TRUE 
//	|| GD.GetSmallMapValidSwitch() )return TRUE;
	
	return TRUE;
}


void DrawSmallMapImjin()
{
	// 봉화대가 있으면 작은 지도를 그려주고, 
	if(IsSmallMapValid())
	{
		SHORT drawswitch=FALSE;
		
		// 작은 지도가 표시안되는 상태에서 보이면,
		if(GD.SmallMapDrawSwitch==FALSE)
		{
			// 효과음을 낸다. 
			if(GD.SmallMapDrawSwitch==FALSE)
			   PushEffect(EFFECT_MAP1, 0, 0);

			drawswitch=TRUE;
		}
		// 보이는 상태에서는 30프레임마다..
		else
		{
			if(abs(GD.Frame - GD.DrawSmallMapFrame) >= 30)
			{
				GD.DrawSmallMapFrame = GD.Frame;
				drawswitch=TRUE;
			}
		}

		GD.SmallMapDrawSwitch=TRUE;

		// 작은지도의 모두가 변경되었다면, 
		if(GD.SmapModeChangedSwitch==TRUE)
		{
			GD.SmapModeChangedSwitch=FALSE;

			drawswitch=TRUE;
		}

		if(drawswitch==TRUE)
		{
		    if(clGrp.LockSurface(lpPannelSurface)==TRUE)
			{
				// 지형까지 보이는 모드이면, 지형도 그려준다. 
	            SMap.Draw(GD.SmapModeFullSwitch);

 	            SMap.DrawChar();

	            clGrp.UnlockSurface(lpPannelSurface);
			}
		}
	}

	// 봉화대가 없으면 작은 지도를 없앤다.
	if(IsSmallMapValid() == FALSE && GD.SmallMapDrawSwitch==TRUE)
	{
		// 효과음을 낸다. 
		if(GD.SmallMapDrawSwitch==TRUE)
		   PushEffect(EFFECT_MAP2, 0, 0);

		GD.SmallMapDrawSwitch=FALSE;

		// 판넬을 surface로 옮긴다.
	    if(clGrp.LockSurface(lpPannelSurface)==TRUE)
		{
			
	         clGrp.PutImageT(0, 0, 
		          PannelSpr.Header.Xsize, 
			      PannelSpr.Header.Ysize,
		          PannelSpr.Image);

	        clGrp.UnlockSurface(lpPannelSurface);
		}
	}

}

BOOL DrawPannel()
{
	HRESULT hResult;

	RECT _rect;
	

	if(WindowModeSwitch)
	{
		_rect.left = 0;
		_rect.right = PannelSpr.Header.Xsize;
		_rect.top = 0;
		_rect.bottom = PannelSpr.Header.Ysize;
	

		lpScreenBuffer->BltFast(0, clGrp.GetScreenYsize()-PannelSpr.Header.Ysize, lpPannelSurface, &_rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
//		if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
//		{
//			clGrp.PutSprite8T(0, clGrp.GetScreenYsize()-PannelSpr.Header.Ysize, 
//				PannelSpr.Header.Xsize, 
//				PannelSpr.Header.Ysize,
//				PannelSpr.Image);
//			
//			clGrp.UnlockSurface(lpScreenBuffer);
//		}
	}
	else
	{

		_rect.left = 0;
		_rect.right = clGrp.GetScreenXsize();
		_rect.top = 0;
		_rect.bottom = PannelSpr.Header.Ysize;

		hResult = lpScreenBuffer->BltFast(0, clGrp.GetScreenYsize()-PannelSpr.Header.Ysize, lpPannelSurface, &_rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);

		// 서피스가 로스트된 것이면, 
		if(hResult == DDERR_SURFACELOST)
		{
			clGrp.Restore();
		}

	}


	return TRUE;
	
}


void SmallMapToggleOperation()
{
	if(GD.SmallMapDrawSwitch)
	{
		GD.SmapModeChangedSwitch=TRUE;
		GD.SmapModeFullSwitch=(GD.SmapModeFullSwitch ? FALSE:TRUE);
	}
}


// 실제 게임의 루틴이 들어있는 함수 
// 리턴값: 0 -> normal
SHORT GameProcess()
{
	// 게임이 살아 있음을 일정시간마다 게임 서버에 보내준다. (일정시간등의 체크는 함수내에서 처리한다.)
	pGame->OnlineSendHeartBeat();

	// 게임중 채팅 입력을 관장한다.
	InputOperateInGame();
	
	
	if(GD.Frame==0)StartClock = CurrentClock;
	

	// 게임 메뉴를 보여준다. 
	if(GameMenuShowSwitch==TRUE)
	{
		// 게임에서 게임메뉴를 부를 때에만,
		if(GameStatus==GS_GAME)
		{
			// 게임 메뉴에서는 마우스를 기본 마우스로 사용한다.
			SetMouse(MOUSE_NORMAL);
			
			// 팔레트를 초기화해준다. 
			SetGamePalette(clGrp.Palette);
			
			GameStatus=GS_INITGAMEMENU;

			// 현재 프라이머리 화면을 lpscreenbuffer로 옮긴다. 
			if(lpScreenBuffer)
			{
				if (WindowModeSwitch)
				{
					pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);				
				}
				else
				{
					lpScreenBuffer->BltFast(0, 0, clGrp.lpDDSPrimary, NULL, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);

				}

				if (!WindowModeSwitch) {
					// 마우스를 지운다. 
					if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
					{
						DrawMouseBuffer();
						clGrp.UnlockSurface(lpScreenBuffer);
					}
				}

			}

			return 0;
		}
	}
	
	if(GameOverHead()==TRUE)return 0;


	if(GamePauseSwitch==FALSE)
	{
		// 화면이동관련 
		if(ScrollProcSwitch == FALSE)
		{
			ScrollOperation();
			SetScrollProcSwitch(TRUE);
		}
		
		// 치트키로 된 명령을 부여한다. 
//		Cheat.CheatOrder();
	}
	// 지도 타일을 그린다. 
	Map.DrawMap();

	if(GamePauseSwitch==FALSE)
	{
		// 저장된 명령을 처리한다. 
		PopOrder();
	}

	// 게임을 지연시킨다. 
	if(GameDelay() == FALSE)return 0;



	// 초기메뉴로 돌아가는 카운트 다운. 
	if(GD.InitInitMenuCount)
	{
		GD.InitInitMenuCount--;

		if(GD.InitInitMenuCount == 0)
		{
			OperateGameMenuInitInitMenu();
			return 0;
		}

	}

	// 게임을 종료하는 카운트 다운. 
	if(GD.InitQuitCount)
	{
		GD.InitQuitCount--;

		if(GD.InitQuitCount == 0)
		{
			OperateGameMenuInitQuit();
			return 0;
		}
	}


	GD.Frame++;

	// 매 1분당 전투 서버로 하트비트 메시지를 보낸다. 
	if(GD.Frame && (GD.Frame % (60000/DEFAULT_MULTI_PLAY_DELAY_TIME) == 0))
	{
		OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(MyNet.PlayersdpId.dpId[UserPlayerNumber ]);
	}

	if(GD.Frame >= 20)
	{
		if(GD.Frame == 20)SetStartFrameClock(CurrentClock);
		else
		{
			// 경과된 시간을 기록한다. 
			PushFrameClock(GD.Frame, CurrentClock);
		}
	}

	GameAction();


	// 윈도우가 활성화 되어 있는 경우에만 마우스를 그린다. 
	if(bActive == TRUE)
	{
		POINT point;
		GetCursorPos(&point);
		if (WindowModeSwitch)
		{
			ScreenToClient(pGame->Hwnd,&point);
//			BOOL bChange = TRUE;
//			if (point.x > 800)
//			{
//				bChange = TRUE;
//				point.x = 800;
//			}
//			if (point.x < 0)
//			{
//				bChange = TRUE;
//				point.x = 0;
//			}
//			if (point.y < 0)
//			{
//				bChange = TRUE;
//				point.y = 0;
//			}
//			if (point.y > 600)
//			{
//				bChange = TRUE;
//				point.y = 600;
//			}
//
//			POINT temppt = point;
//
//		    ClientToScreen(pGame->Hwnd,&temppt);

		//	if (bChange) SetCursorPos(temppt.x,temppt.y);


		}
		
		IpD.Mouse_X=(SHORT)point.x;
		IpD.Mouse_Y=(SHORT)point.y;
		
		
		DrawMousePrimary();
	}

    GD.GameFrame++;
	
	// 플레이 시간을 측정한다. 
	if(abs(GD.TempAccumulatedPlayClock - CurrentClock) >= 1000)
	{
		GD.TempAccumulatedPlayClock = CurrentClock;
		GD.AccumulatedPlaySecond ++;
	}

	
	return 0;
}


// 게임의 속도를 일정하게 만든다. 
BOOL GameDelay()
{

	DWORD basicdelay;
	
	// 설정된 값에 따라 기본적인 속도를 얻어온다.
	basicdelay = pGame->Config.GetRealGameSpeedClock();

	FrameDelay = basicdelay + FrameClockDelay;
	FrameClockDelay = 0;


	clock_t tempclock = CurrentClock;

	// 빠르게 도착했다면, 
	if( abs(CurrentClock - PrevFrameClock) < FrameDelay)
	{
		// 지연시켜야 할 시간 ,
		clock_t delayclock = min( max( FrameDelay - (CurrentClock - PrevFrameClock), 0), FrameDelay);
		
		int _clock = CurrentClock;
		int index=0;
		while((_clock - CurrentClock) < delayclock)
		{
			return FALSE;
		}

		tempclock = _clock;
	}
	else
	{
		clock_t overclock = min( ((CurrentClock - PrevFrameClock) - FrameDelay) , FrameDelay);
		
		// 원래 설정되었어야 할 시간 (지연이 없었을 경우의 시각)
		tempclock = (CurrentClock - overclock);

	}


	PrevFrameClock = tempclock;

	return TRUE;
}


BOOL GameWinCondition()
{
	char	buffer[128];
	// 프레임이 달라져야만  체크한다. 
	if(GD.Frame == PrevGameWinConditionFrame)return FALSE;
	PrevGameWinConditionFrame = GD.Frame;

	SHORT status = WinCondition();


	//이기거나 지면 게임을 빠져 나가야 한다. 
	if((GD.VictoryDelay && (abs(CurrentClock - GD.VictoryDelay) > VICTORY_HALFDELAY))
	|| (GD.DefeatDelay  && (abs(CurrentClock - GD.DefeatDelay)  > DEFEAT_HALFDELAY)))
	{
		if(GameQuitSwitch == FALSE)
		{
			GameQuitSwitch = TRUE;
			MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);
		}
	}

	


	// actdoll (2004/09/16 15:06) : 2004 전주엑스포 이벤트용
	if(status == 1)
	{
		int siTemp = 0;
		if((strcmp(MyNet.PlayersName.Name[1],"MONSTER") != 0) && (strcmp(MyNet.PlayersName.Name[2],"") == 0)
			&& (strcmp(MyNet.PlayersName.Name[3],"") == 0) && (strcmp(MyNet.PlayersName.Name[4],"") == 0)
			&& (strcmp(MyNet.PlayersName.Name[5],"") == 0) && (strcmp(MyNet.PlayersName.Name[6],"") == 0)
			&& (strcmp(MyNet.PlayersName.Name[7],"") == 0))
		{
			ZeroMemory(buffer,128);
			wsprintf(buffer,Text.Msg_About_Winner_Of_Battle.Get(),MyNet.PlayersName.Name[UserPlayerNumber]);
			SetMessage(buffer);

			if(UserPlayerNumber == 0) siTemp = 1;

			ZeroMemory(buffer,128);
			wsprintf(buffer,Text.Msg_About_Loser_Of_Battle.Get(),MyNet.PlayersName.Name[siTemp]);
			SetMessage(buffer);

//			ShowMessage(10,70,lpScreenBuffer);
		}
	}
	else if(status == -1)
	{
		int siTemp = 0;
		if((strcmp(MyNet.PlayersName.Name[1],"MONSTER") != 0) && (strcmp(MyNet.PlayersName.Name[2],"") == 0)
			&& (strcmp(MyNet.PlayersName.Name[3],"") == 0) && (strcmp(MyNet.PlayersName.Name[4],"") == 0)
			&& (strcmp(MyNet.PlayersName.Name[5],"") == 0) && (strcmp(MyNet.PlayersName.Name[6],"") == 0)
			&& (strcmp(MyNet.PlayersName.Name[7],"") == 0))
		{
			if(UserPlayerNumber == 0) siTemp = 1;

			ZeroMemory(buffer,128);
			wsprintf(buffer,Text.Msg_About_Winner_Of_Battle.Get(),MyNet.PlayersName.Name[siTemp]);
			SetMessage(buffer);

			ZeroMemory(buffer,128);
			wsprintf(buffer,Text.Msg_About_Loser_Of_Battle.Get(),MyNet.PlayersName.Name[UserPlayerNumber]);
			SetMessage(buffer);

//			ShowMessage(10,70,lpScreenBuffer);
		}
	}
			
	// 승리했다. 
	if(status == 1)
	{
		//wsprintf(buffer, "전투에서 승리하였습니다.");
//		SetMessage("전투에서 승리하였습니다.");
		// 승리한것임을 기억한다. 
		GD.WinSwitch = TRUE;

		GameStatus   = GS_INITWIN;

//    	EndGame(); 
//		pGame->pOnlineWorld->pOnlineClient->SendReturnOnline(TRUE);
		return TRUE;
	}
	// 패배 했다. 
	else if(status == -1)
	{
		//wsprintf(buffer, "졌네..불쌍..");
//		SetMessage("전투에서 패배하였습니다.");
		GD.WinSwitch = FALSE;
		GameStatus = GS_INITLOSE;

//		EndGame();
//		pGame->pOnlineWorld->pOnlineClient->SendReturnOnline(FALSE);
		return TRUE;
	}

	return FALSE;
}




//--------------------------------------------------------------------------
// Name: EndGame()
// Desc: 게임을 종료하는데 필요한 설정 
//--------------------------------------------------------------------------
void EndGame()
{
	// actdoll (2004/11/05 15:58) : OECEX
	//	만약 에디트 박스의 포커스가 맞춰져 있는 상태에서 게임이 끝나면 포커스를 없앤다.
	Handler_OnlineEditControlEx::ClearCurrentFocus();

	if (WindowModeSwitch)
	{
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.bottom = 600;
		rect.right = 800;
		HRESULT hr =	clGrp.lpDDSBack->BltFast(0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
		
	}


#ifdef _OGG_PLAY
#else
	if(g_pSoundManager)
	{
		g_pSoundManager->StopAllSound();

		if(BattleMusic)
		{
			g_pSoundManager->DelPlayID( BattleMusic );
			BattleMusic = 0;
		}
	}
#endif

	// 사용자 번호를 초기화한다. 
	UserPlayerNumber=0;

	Map.FreeMap();
	FreeMapTile();

	// 마우스 커서를 기본형으로 교체한다.
	SetMouse( MOUSE_NORMAL );


	// 팔레트를 게임 팔레트로 초기화한다. 
	SetGamePalette(clGrp.Palette);

	// 캐릭터를 모두 없앤다. 
	DeleteChar();

	// 총알을 모두 없앤다. 
	DeleteBullet();
	// 연기를  모두 없앤다. 
	DeleteSmoke();
	
	// 마법등을 모두 없앤다. 
	DeleteMagic();

	// 게임 중에 로딩된 이미지 파일의 메모리를 해제한다.
	FreeImgFile();



	if(pGame)
	{
		if(pGame->pBattle)
			pGame->pBattle->InitGameButton();
	}
	
	// 전체 화면을 클리핑 영역으로 설정한다.
	clGrp.SetClipFullScreen();
}

SHORT GameAction()
{
	SHORT i;
	GD.OldRand=GD.Rand;
    GD.Rand = GD.Random();
	SHORT id = 0;


	// 게임 데이터의 해킹 가능성을 확인한다. 
	if(GD.Frame % 10 == 0)
	{
		// kindinfo등의 해킹 여부를 검사한다. 
		if(CheckKindInfo() == FALSE)
		{
			// 서버에 해킹 우려가 있는 사용자라는 사실을 알린다. 
			// 게임을 종료시킨다. 
			GameStatus = GS_INITQUIT;
			
			// 해킹으로 인해 게임이 종료됨을 알린다. 
			//clGrp.Error("Game Quit", "There is possibility of Hacking. Your account will be blocked. Please contact us.");
		}
	}


	if(GamePauseSwitch==FALSE)
	{
	    // 임시로 
	    GD.MoveCharNumber=0;

		// 아이템 획득 창 관련 행동을 한다. 
		pGame->pItemInfoWindow->Prepare();

		// 지도 편집 모드가 아니어야 한다. 
	    if(MapEditMode==FALSE)
		{

			// 시간을 처리한다. 
			GD.Time.PassTime();


			// 비의 행동 
			GD.Rain.Action();

			VolcanoAction();
		}

		// 캐릭터의 행동 
		GD.SyncSignal  = 0;
		int charindex;
	    for(charindex = 0;charindex < GD.CharOrderIndex;charindex++)
		{
			id = GD.CharOrder[charindex] ;
			if(GD.CharInfo[ id ])
			{
				// 동기 신호를 얻어온다. 
				GD.SyncSignal = GD.SyncSignal + id + clifChar.GetX(id) + clifChar.GetY(id) + clifChar.GetCurrentOrder(id) 
					+ clifChar.GetWorkStep(id) + clifChar.GetDX(id) + clifChar.GetDY(id) + clifChar.GetPara1(id) 
					+ clifChar.GetDpower(id) + clifChar.GetLife(id) + clifChar.GetMana(id);


				DWriteStringToFile(" id = %d , life =  %d",id,clifChar.GetLife(id));
				if(clifChar.Action(id)==FALSE)
				{
					DWriteStringToFile("죽었다.id = %d",id);
					DeleteChar(id);
				}
			}
		}

		{

			// 전방 탐색을 실시한다.
			for(charindex = 0;charindex < GD.CharOrderIndex;charindex++)
			{
				id = GD.CharOrder[charindex] ;
				if(GD.CharInfo[ id ])
				{
					if(CR[id].SearchForwardSwitch == TRUE)
					{
						CR[id].SearchForward();
						CR[id].SearchForwardSwitch = FALSE;
					}
				}
			}
		}

		// 마법들의 행동. 
		MagicAction();

		// 총알들의 행동 
	    for(i=0;i<MAX_BULLET_NUMBER;i++)
		{
		   if(GD.BulletInfo[i])
		   {
			   //if(Bullet[i].Action()==FALSE)
			   if(Bullet_Action(i)==FALSE)	// hojae_repair
			   {
			 	  DeleteBullet(i);
			   }
		   }
		}

		// 연기들의 행동  
	    for(i=0;i<MAX_SMOKE_NUMBER;i++)
		{
		    if(GD.SmokeInfo[i])
			{
			    //if(Smoke[i].Action()==FALSE)
				if(Smoke_Action(i)==FALSE)	// hojae_repair
				{
				   DeleteSmoke(i);
				}
			}
		}

		// 게임 시간을 진행한다.
		PassPlayTime();

		// 캐릭터들의  안개 처리 
		// 기존의 밝은 지역을 어둡게 만들고 새로운 지역을 밝게 만든다. 
		// 포그.
		if(pGame->pOnlineWorld)
		{
			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,FALSE);
				// 기존의 밝은 지역은 놔두고 새로운 지역만 밝힌다. 
				else
					GD.BlackCheck(FALSE,FALSE);
			}
			else
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,TRUE);
				// 기존의 밝은 지역은 놔두고 새로운 지역만 밝힌다. 
				else
					GD.BlackCheck(FALSE,TRUE);
				
			}
		}
   }
   
   // 스페이스 바를 눌러 화면 이동 시킨다. 
   if(GD.ScreenMoveSwitch==TRUE)
   {
       Map.VirtualMapX=GD.ScreenMoveX;
       Map.VirtualMapY=GD.ScreenMoveY;
       Map.DrawForceSwitch=TRUE;
	   
	   GD.ScreenMoveSwitch=FALSE;
   }
   
	return 0;
}


// 마우스 버튼에 의한 자동 명령을 처리한다. 
BOOL OperateAutoOrder()
{

	// 현재 자동 명령이 아닌 방식으로 선택된 명령이 있다면 자동 명령은 무시한다. 
	if(GD.SelectedOrder!=0 && GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
	{
	   return FALSE;
	}

	// 메뉴에 수확이 있고 마우스가 곡물 위에 있으면, 
	if(MouseMode&MOUSEMODE_HARVEST)
    {
		GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_HARVEST;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 이동이 있고 마우스가 빈자리에 있으면, 
    else if(MouseMode&MOUSEMODE_MOVE)
    {
 	    GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_MOVE;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 공격이 있고 마우스가 적 위에 있으면, 
    else if(MouseMode&MOUSEMODE_ATTACK)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_ATTACK;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 탑승이 있으면
    else if(MouseMode&MOUSEMODE_RIDE)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_RIDE;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 치료가 있고 
    else if(MouseMode&MOUSEMODE_HEAL)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_HEAL;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
	else if(MouseMode&MOUSEMODE_MANA)
	{
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_MANA;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
	}
    // 메뉴에 깃발을 본영에 넣는 것이 있고. 
    else if(MouseMode&MOUSEMODE_GOINTOHQ)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_GOINTOHQ;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
	// 메뉴에 소화가 있고 마우스가 불 위에 있으면, 
	else if(MouseMode&MOUSEMODE_EXTINGUISH)
    {
		GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_EXTINGUISH;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
	else
	{
		return FALSE;
	}

	return TRUE;
}


void ScrollOperation()
{
	// 스크롤 정보를 초기화한다. 
	Scroll=0;


	

	//스킵하는 프레임에서만 스크롤을 한다. 
	// 윈도우 모드일때는 범위를 넓게 잡는다.
	if (WindowModeSwitch) {
		if(DragSwitch==FALSE )
		{
			if(abs(GameMouseX) <= SCROLL_RANGE )               Scroll|=WEST;
			if(abs(GameMouseX - (clGrp.GetScreenXsize()-1)) <= SCROLL_RANGE ) Scroll|=EAST;
			if(abs(GameMouseY) <= SCROLL_RANGE)                Scroll|=NORTH;
			if(abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE ) Scroll|=SOUTH;
			
			// 키보드에 의한 스크롤을 정의한다. 
			if(GetAsyncKeyState(VK_UP)   < -1)	   Scroll |= NORTH;
			if(GetAsyncKeyState(VK_DOWN) < -1)	   Scroll |= SOUTH;
			if(GetAsyncKeyState(VK_LEFT) < -1)     Scroll |= WEST;
			if(GetAsyncKeyState(VK_RIGHT)< -1)	   Scroll |= EAST;
		}
	}
	else
	{
		if(DragSwitch==FALSE )
		{
			if(GameMouseX==0)              Scroll|=WEST;
			if(GameMouseX==(clGrp.GetScreenXsize()-1))Scroll|=EAST;
			if(GameMouseY==0)              Scroll|=NORTH;
			if(GameMouseY==(clGrp.GetScreenYsize()-1))Scroll|=SOUTH;
			
			// 키보드에 의한 스크롤을 정의한다. 
			if(GetAsyncKeyState(VK_UP)   < -1)	   Scroll |= NORTH;
			if(GetAsyncKeyState(VK_DOWN) < -1)	   Scroll |= SOUTH;
			if(GetAsyncKeyState(VK_LEFT) < -1)     Scroll |= WEST;
			if(GetAsyncKeyState(VK_RIGHT)< -1)	   Scroll |= EAST;
		}
		
		
	}


	// 스크롤 중이면 지도를 그린다. 
	if(Scroll)
	{
		Map.SetMapDrawSwitch(TRUE);
	}

	// 스크롤 정보를 임시로 저장한다. 
	SHORT tempvirtualmapx=Map.VirtualMapX;
	SHORT tempvirtualmapy=Map.VirtualMapY;

	// 북쪽 스크롤 
	if(Scroll&NORTH)
	{
		if(Map.DecreaseVirtualMapY(1)==TRUE)
		{

	 	   if(Map.DecreaseVirtualMapX(1)==FALSE)
		   {
			   if(Map.DecreaseVirtualMapY(1)==FALSE)
				   Map.IncreaseVirtualMapY(1);
		   }
		}
		else
		{
			Map.DecreaseVirtualMapX(2);
		}
	}
	// 남쪽 스크롤 
	else if(Scroll&SOUTH)
	{
		if(Map.IncreaseVirtualMapY(1)==TRUE)
		{

			if(Map.IncreaseVirtualMapX(1)==FALSE)
			{
				if(Map.IncreaseVirtualMapY(1)==FALSE)
					Map.DecreaseVirtualMapY(1);

			}
		}
		else
		{
		   Map.IncreaseVirtualMapX(2);

		}
	}

	// 서쪽 스크롤 
	if(Scroll&WEST)
	{
	   if(Map.DecreaseVirtualMapX(1)==TRUE)
	   {
		  if(Map.IncreaseVirtualMapY(1)==FALSE)
		  {
			if(Map.DecreaseVirtualMapX(1)==FALSE)
				Map.IncreaseVirtualMapX(1);
		  }
	   }
	   else
	   {
	      Map.IncreaseVirtualMapY(2);
	   }
	}
	// 동쪽 스크롤 
    else if(Scroll&EAST)
	{
	    if(Map.IncreaseVirtualMapX(1)==TRUE)
		{
		   if(Map.DecreaseVirtualMapY(1)==FALSE)
		   {
			  if(Map.IncreaseVirtualMapX(1)==FALSE)
			  {
   				Map.DecreaseVirtualMapX(1);
			  }
		   }
		}
	    else
		{
	       Map.DecreaseVirtualMapY(2);
		}
	}


	if(Map.VirtualMapX==tempvirtualmapx+1 && Map.VirtualMapY==tempvirtualmapy-3)
	{
		Map.VirtualMapX=tempvirtualmapx;
		Map.VirtualMapY=tempvirtualmapy-2;
	}
	else if(Map.VirtualMapX==tempvirtualmapx-3 && Map.VirtualMapY==tempvirtualmapy+1)
	{
	   Map.VirtualMapX=tempvirtualmapx-2;
	   Map.VirtualMapY=tempvirtualmapy;
	}
	else if(Map.VirtualMapX==tempvirtualmapx+3 && Map.VirtualMapY==tempvirtualmapy-1)
	{
	   Map.VirtualMapX=tempvirtualmapx+2;
	   Map.VirtualMapY=tempvirtualmapy;
	}
	else if(Map.VirtualMapX==tempvirtualmapx-1 && Map.VirtualMapY==tempvirtualmapy+3)
	{
	   Map.VirtualMapX=tempvirtualmapx;
	   Map.VirtualMapY=tempvirtualmapy+2;
	}


	// 실제로 스크롤이 이루어 졌는지 안이루어 졌는지 확인한다. 
	if(tempvirtualmapx!=Map.VirtualMapX || tempvirtualmapy!=Map.VirtualMapY)
	{
		   ScrollDoneSwitch=TRUE;
	}
	else
	{
		ScrollDoneSwitch=FALSE;
	}

}


void SystemKeyCheck()
{
	if(GetAsyncKeyState(VK_CONTROL) < -1)
	{
		ControlKeyPressSwitch = TRUE;
	}
    else ControlKeyPressSwitch = FALSE;

	if(GetAsyncKeyState(VK_SHIFT) < -1)
	{
		ShiftKeyPressSwitch = TRUE;
	}
    else ShiftKeyPressSwitch = FALSE;

	//alt키 눌림 여부 확인        
	if(GetAsyncKeyState(VK_MENU) < -1)
	{
		 IpD.AltKeyPressSwitch=TRUE;
	}
	else
	{
	     IpD.AltKeyPressSwitch=FALSE;
	}
}


void GroupInterface()
{
	
    if(GD.SelectedNumber>=1)
	{
		if(ControlKeyPressSwitch==TRUE)
		{
			// 1
			if(GD.KB.GetAKey('1'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(1);
				//GD.Team.AddTeam(1,UserPlayerNumber);
				pGame->Config.SetTeamInfo(1);
			}
			// 2
			else if(GD.KB.GetAKey('2'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(2);
				//GD.Team.AddTeam(2,UserPlayerNumber);
				pGame->Config.SetTeamInfo(2);
			}
			// 3
			else if(GD.KB.GetAKey('3'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(3);
				//GD.Team.AddTeam(3,UserPlayerNumber);
				pGame->Config.SetTeamInfo(3);
			}
			// 4
			else if(GD.KB.GetAKey('4'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(4);
				//GD.Team.AddTeam(4,UserPlayerNumber);
				pGame->Config.SetTeamInfo(4);
			}
			// 5
			else if(GD.KB.GetAKey('5'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(5);
				//GD.Team.AddTeam(5,UserPlayerNumber);
				pGame->Config.SetTeamInfo(5);
			}
			// 6
			else if(GD.KB.GetAKey('6'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(6);
				//GD.Team.AddTeam(6,UserPlayerNumber);
				pGame->Config.SetTeamInfo(6);
			}
			// 7
			else if(GD.KB.GetAKey('7'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(7);
				//GD.Team.AddTeam(7,UserPlayerNumber);
				pGame->Config.SetTeamInfo(7);
			}
			// 8
			else if(GD.KB.GetAKey('8'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(8);
				//GD.Team.AddTeam(8,UserPlayerNumber);
				pGame->Config.SetTeamInfo(8);
			}
			// 9
			else if(GD.KB.GetAKey('9'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(9);
				//GD.Team.AddTeam(9,UserPlayerNumber);
				pGame->Config.SetTeamInfo(9);
			}
			// 0
			else if(GD.KB.GetAKey('0'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(0);
				//GD.Team.AddTeam(10,UserPlayerNumber);
				pGame->Config.SetTeamInfo(0);
			}
		}
	}
	
	// 그룹 선택 
	if(GD.KB.GetAKey('1'))
	{
		if(GD.PressedTeam!=1)
		{
			if(GD.SelectedTeam!=1)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(1);
				//GD.Team.SelectTeam(1,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(1);
				//GD.Team.GotoTeam(1,UserPlayerNumber);
		}
		GD.PressedTeam=1;
	}
	else if(GD.KB.GetAKey('2'))
	{
		if(GD.PressedTeam!=2)
		{
			if(GD.SelectedTeam!=2)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(2);
				//GD.Team.SelectTeam(2,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(2);
				//GD.Team.GotoTeam(2,UserPlayerNumber);
		}
		GD.PressedTeam=2;
	}
	else if(GD.KB.GetAKey('3'))
	{
		if(GD.PressedTeam!=3)
		{
			if(GD.SelectedTeam!=3)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(3);
				//GD.Team.SelectTeam(3,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(3);
				//GD.Team.GotoTeam(3,UserPlayerNumber);
		}
		GD.PressedTeam=3;
	}
	else if(GD.KB.GetAKey('4')) 
	{
		if(GD.PressedTeam!=4)
		{
			if(GD.SelectedTeam!=4)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(4);
				//GD.Team.SelectTeam(4,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(4);
				//GD.Team.GotoTeam(4,UserPlayerNumber);
		}
		GD.PressedTeam=4;
	}
	else if(GD.KB.GetAKey('5'))
	{
		if(GD.PressedTeam!=5)
		{
			if(GD.SelectedTeam!=5)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(5);
				//GD.Team.SelectTeam(5,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(5);
				//GD.Team.GotoTeam(5,UserPlayerNumber);
		}
		GD.PressedTeam=5;
	}
	else if(GD.KB.GetAKey('6'))
	{
		if(GD.PressedTeam!=6)
		{
			if(GD.SelectedTeam!=6)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(6);
				//GD.Team.SelectTeam(6,UserPlayerNumber);
				
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(6);
				//GD.Team.GotoTeam(6,UserPlayerNumber);
		}
		GD.PressedTeam=6;
	}
	else if(GD.KB.GetAKey('7'))
	{
		if(GD.PressedTeam!=7)
		{
			if(GD.SelectedTeam!=7)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(7);
				//GD.Team.SelectTeam(7,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(7);
				//GD.Team.GotoTeam(7,UserPlayerNumber);
		}
		GD.PressedTeam=7;
	}
	else if(GD.KB.GetAKey('8'))
	{
		if(GD.PressedTeam!=8)
		{
			if(GD.SelectedTeam!=8)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(8);
				//GD.Team.SelectTeam(8,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(8);
				//GD.Team.GotoTeam(8,UserPlayerNumber);
		}
		GD.PressedTeam=8;
	}
	else if(GD.KB.GetAKey('9'))
	{
		if(GD.PressedTeam!=9)
		{
			if(GD.SelectedTeam!=9)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(9);
				//GD.Team.SelectTeam(9,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(9);
				//GD.Team.GotoTeam(9,UserPlayerNumber);
		}
		GD.PressedTeam=9;
	}
	else if(GD.KB.GetAKey('0'))
	{
		if(GD.PressedTeam!=0)
		{
			if(GD.SelectedTeam!=0)
				GD.Player[UserPlayerNumber].Team.SelectTeam(0);
				//GD.Team.SelectTeam(0,UserPlayerNumber);
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(0);
				//GD.Team.GotoTeam(0,UserPlayerNumber);
		}
		GD.PressedTeam=0;
	}
	else
	{
		GD.PressedTeam=-1;
	}
}


_NewID GetUnderMouseID()
{
	SHORT id;
	SHORT airid;
	SHORT i, j;
	SHORT length=0;
	SHORT min_length=10000;
	SHORT x, y;


	_NewID newid;

	newid.Init();

	// 마우스가 지도위에 있는 것이 아니라면 , 
	if(Map.IsInMapArea(MapX, MapY) == FALSE) return newid;


	// 마우스 밑에 있는 캐릭터를 구한다. 
	if(GD.BlackMap[UserPlayerNumber][MapX][MapY] != FOG2_BLACK_STEP)
	{
			
		// 공중 캐릭터를 먼저 조사한다. 
		id         = 0;
		length     = 0;
		min_length = 10000;

		for(i = 0;i < GD.AirCharNumber;i++)
		{
			airid = GD.AirCharID[i].ID;

			if(IsSee(airid) == TRUE)
			{
				x=clifChar.GetX(airid);
				y=clifChar.GetY(airid);
					
				if(CR[airid].m_cltCharDrawInfo.GetDrawSwitch() == TRUE 
					&& clifChar.IsAbsPtInObj(airid, GameRealX, GameRealY, length) == TRUE)
				{
					if(length < min_length)
					{
						min_length = length;
						id         = airid;
						newid      = clifChar.GetID(id);
					}
				}
			}
		}

		// 공중에서 캐릭터를 찾으면 리턴한다. 
		if(newid.IsEmpty() == FALSE) return newid;


		// 지상에서 캐릭터를 찾는다. 
		length     = 0;
		min_length = 10000;
		
		
		for(SHORT range = 0 ; range < 3 ; range++)
		{
			for(i=MapY - range;i <= MapY + range; i++)
				for(j=MapX - range;j <= MapX + range; j++)
				{
					if(i==(MapY-range) || i== (MapY+range) || j==(MapX-range) || j==(MapX+range)){}
					else continue;
					
					// 지도의 범위내에 존재하는지 검사한다. 
					if(Map.IsInMapArea(j, i) == FALSE) continue;
					
					// 완전히 어두운 곳에서만 선택 불가 
					if(GD.BlackMap[UserPlayerNumber][j][i] == FOG2_BLACK_STEP)continue;
					
					id = Map.GetIDMap(j, i);
					
					if(IsSee(id) && CR[id].m_cltCharDrawInfo.GetDrawSwitch() == TRUE)
					{ 
						
						if(clifChar.IsAbsPtInObj(id, GameRealX, GameRealY, length)==TRUE)
						{
							if(length < min_length)
							{
								min_length = length;
								newid      = clifChar.GetID(id);
							}
						}
					}
				}

		}
			
		// 최종적으로 마우스 밑에 캐릭터가 없는 상태라면, 
		if(newid.IsEmpty())
		{
			SHORT harvestx, harvesty;
			
			if(MouseModeHarvest(MapX, MapY, GameMouseX, GameMouseY, harvestx, harvesty)==TRUE)
			{
				UnderMouseCropType  = clifMap.GetCropTypeMap(harvestx, harvesty);
				UnderMouseCropMount = Map.GetCropMountMap(harvestx, harvesty);
			}
			
		}
	}

	return newid;
}


// 드래그가 종료되었을때의 처리이다.
void DragOperation()
{
	SI32 i;
	SHORT id;
    SHORT realx1, realy1, realx2, realy2;

	// 선택된 유닛을 저장할 임시 버퍼를 만들고 초기화한다.
	_NewID temp_id[MAX_SELECTED_CHAR_NUMBER];
	SHORT index=0;
	SHORT newselectedid[MAX_SELECTED_CHAR_NUMBER];
	SHORT newselectednumber = 0;
	
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		temp_id[i].Init();
		newselectedid[i] = 0;
	}
	
	// 드래깅을 초기화한다. 
    MapDragSwitch = FALSE;

	
	// 선택 영역의 시작과 끝을 정렬한다. 
	int min_x = min(MapDragStartX, MapX);
	int max_x = max(MapDragStartX, MapX);
	int min_y = min(MapDragStartY, MapY);
	int max_y = max(MapDragStartY, MapY);
	
	MapDragStartX = min_x;
	MapDragStartY = min_y;
	MapDragEndX   = max_x;
	MapDragEndY   = max_y;

	
	// 마우스를 한곳에서 클릭했을때 
	if(abs(MapDragStartX-MapDragEndX)<=1 &&  abs(MapDragStartY-MapDragEndY)<=1)
	{
		
		// 1버튼 인터페이스이면 자동 명령 루틴을 수행하고 FALSE를 리턴하면 캐릭터 선택으로 설정된다. 
		// 2버튼 인터페이스이면 캐릭터 선택으로 설정된다. 
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
		{
		}
		else
		{
			// 마우스 밑에 캐릭터가 있다면, 
			if(UnderMouseID.IsEmpty() == FALSE)
			{
				// shift키가 눌린 상태이고 
				// 같은 편을 선택한 것이면, 
				if(ShiftKeyPressSwitch  == TRUE 
				&& clifChar.GetPlayerNumber(UnderMouseID.ID) == UserPlayerNumber)
				{
					// 기존에 선택되었던 캐릭터를 버퍼에 넣는다. 
					
							index = 0;

					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// 선택된 캐릭터를 넣는다. 
						if(GD.SelectedID[i].IsEmpty() == FALSE)
						{
							temp_id[index] = GD.SelectedID[i];
							index++;
						}
					}

					
					// 마우스 아래의 캐릭터가 이미 선택되어 있는 캐릭터가 아닌지 확인한다. 
					SHORT alreadyselectedswitch=FALSE;
					SHORT alreadyselectedindex=0;
					BOOL invalidswitch = FALSE;
					
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// 이미 선택된 유닛이 적군이거나 혼자선택 전용 유닛이면, 
						if(clifChar.GetPlayerNumber(temp_id[i].ID) != UserPlayerNumber)
						{
							invalidswitch = TRUE;
						}

						if(UnderMouseID.IsSame(temp_id[i]) == TRUE)
						{
							alreadyselectedswitch = TRUE;
							alreadyselectedindex  = i;
						}
					}
					
					// 새로이 선택된 캐릭터가 기존에 선택된 캐릭터라면, 
					if(alreadyselectedswitch == TRUE)
					{
						// 선택된 리스트에서 제거한다. 
						clifChar.SetSelect(temp_id[alreadyselectedindex].ID, FALSE, FALSE);
						temp_id[alreadyselectedindex].Init();

					}
					// 새로이 선택된 캐릭터가 선택되어있지 않은 캐릭터라면, 
					else
					{
						// 이미 선택된 캐릭터가 있는 상황에서 건물등과 같이 한개만 선택 가능한 유닛이면, 
						// 신규 선택을 하지 않는다.
						if(invalidswitch == TRUE)
						{
						}
						// 여러개가 선택되어도 무방한 캐릭터인 경우에는 추가한다. 
						else
						{
							// 새로 선택된 캐릭터의 수를 초기화한다. 
							newselectednumber = 0;

							// 빈자리에 새로 추가해 넣는다. 
							for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
							{
								// 빈자리에는 한번만 넣는다.
								if(temp_id[i].IsEmpty() && newselectednumber == 0)
								{
									temp_id[i] = UnderMouseID;
									clifChar.SetSelect(UnderMouseID.ID, TRUE, FALSE);
									
									// 신규로 선택된 유닛을 저장한다.
									newselectedid[index] = UnderMouseID.ID;
									newselectednumber++;
									
									index++;
								}
							}
						}
						
						
					}
					
				}
				// 쉬프트등이 눌리지 않는 그밖의 일반적인 선택의 경우 
				else
				{
					temp_id[0] = UnderMouseID;
					index      = 1;
					
					
					// 신규로 선택된 유닛을 저장한다.
					newselectedid[0] = UnderMouseID.ID;
					newselectednumber++;
					
					
					if(abs(CurrentClock-clifChar.GetSelectedClock(temp_id[0].ID))<DOUBLE_CLICK_DELAY)
					{
						DoubleClickedID = temp_id[0].ID;
					}
					
				}
			}
			
		}
	}
	// 마우스로 2칸 이상의 범위에서 실질적인 드래깅을 했을때 
	else  
	{
		
		Map.TransFromScreenToRealXY(DragStartX, DragStartY, realx1, realy1);
		Map.TransFromScreenToRealXY(DragEndX,   DragEndY,   realx2, realy2);
		
		// shift키가 눌린 상태라면, 
		SHORT newselectstartindex = 0;
		
		if(ShiftKeyPressSwitch  == TRUE)
		{
			
			// 기존에 선택된 캐릭터를 임시 버퍼에 넣는다. 
			index = 0;
			BOOL invalidswitch = FALSE;
			
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				if(GD.SelectedID[i].IsEmpty() == FALSE)
				{
					temp_id[index] = GD.SelectedID[i];
					index++;
					newselectstartindex = index;

					// 이미 선택된 유닛이 적군이거나 혼자선택 전용 유닛이면, 
					if(clifChar.GetPlayerNumber(GD.SelectedID[i].ID) != UserPlayerNumber)
					{
						invalidswitch = TRUE;
					}

				}
			}
			
			if(invalidswitch == FALSE)
			{
				// 범위 안에 있는 캐릭터를 구해서 임시 버퍼에 추가한다.
				SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
				
				
				// 신규로 선택된 유닛을 저장한다.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
			}
		}
		else
		{
			// 범위 안에 있는 캐릭터를 구해서 임시 버퍼를 채운다.
			SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
			
			// 신규로 선택된 유닛을 저장한다.
			for(i = 0;i < index;i++)
			{
				newselectedid[i] = temp_id[i].ID;
				newselectednumber++;
			}
		}
	}
	
	
	// 선택된 유닛이 있다면, 
	if(index > 0)
	{
		
		// 선택한 캐릭터가 하나이고 
		// 짧은 시간에 연속적으로 선택된 것이라면, 
		// 화면 안에 있는 같은 종류의 캐릭터들을 구한다. 
		if(DoubleClickedID)
		{
			
			// 더블 클릭된 유닛이 아군이면 추가로 선택한다.
			if(clifChar.GetPlayerNumber(DoubleClickedID) == UserPlayerNumber)
			{
				
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, realx1, realy1);
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.right,(SHORT)ScreenRect.bottom,   realx2, realy2);
				
				SHORT newselectstartindex = index;

				//  화면의 범위 안에 있는 캐릭터를 임시버퍼에 넣는다.
				SelectCharInDrag(temp_id, index, realx1, realy1, 
					realx2, realy2, DoubleClickedID);
				
				
				// 신규로 선택된 유닛을 저장한다.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
				
				DoubleClickedID = 0;
				
			}
		}
		
		// 선택된 유닛의 효과음을 낸다. 
		SHORT effectnumber=0;
		for(i=0;i<newselectednumber;i++)
		{
			id=newselectedid[(i+GD.Rand)%newselectednumber];
			// 선택된 캐릭터중 1개까지만 효과음을 낸다. 
			
			if(effectnumber==0 && KI[ clifChar.GetKind(id) ].m_siSelectEffect)
			{
				clifChar.SelectOperation(id);
				effectnumber++;
			}
		}
		
		//기존에 선택된 캐릭터를 취소한다. 
		Unselect();
		ClearMenu();
		
		// 유닛을 선택한다.
		for(i=0;i<index;i++)
		{
			id=temp_id[i].ID;
			clifChar.SetSelect(id, TRUE, FALSE);
		}
		
		
	}
	// 선택된 유닛이 없다면, 
	else
	{
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_2BUTTON)
		{
			//기존에 선택된 캐릭터를 취소한다. 
			Unselect();
			ClearMenu();
		}
	}
}



// 키보드 처리를 포함한 루틴 
BOOL GameOverHead()
{
	int i;
	SHORT id;
	DWORD wparam;
	SHORT mapx, mapy;

	SHORT leftpressswitch=FALSE, rightpressswitch=FALSE;
	SHORT mousex, mousey;
	char moveswitch;




	// 임시로 사용한다. 
	if(MapEditMode == TRUE)
	{
		MapEdit.Action();
	}

	
	// 승리 조건을 체크한다. 
	// 승리 했거나 패배 했으면 리턴한다. 
	if(GameWinCondition() == TRUE)
	{
		return TRUE;
	}
	
	// 게임이 중단된 상태면 리턴한다.
	if(GamePauseSwitch==TRUE)return FALSE;


	//////////////////////////////////////////////////////////
	// control, alt 등의 특수키의 상태를 확인한다. 
	//////////////////////////////////////////////////////////
	


    // 키보드 선택을 통한 그룹 설정 및 선택 

	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	{
		SystemKeyCheck();

		GroupInterface();
	}

	// 사용자의 마우스 , 키보드 등의 입력 상태를 확인한다. 
	GameWParam=0;

	if(GD.KB.PopKeyBuffer(leftpressswitch, rightpressswitch, mousex, mousey, wparam, moveswitch) == TRUE)
	{

		GameMouseX = mousex;
		GameMouseY = mousey;
    	GameWParam = wparam;
		
		// 왼쪽 마우스 버튼 눌림 
		if(leftpressswitch >= 0)
		{

			// 왼쪽 버튼이 처음 눌린것이라면, 
	        if(leftpressswitch == TRUE)
			{
		      DragSwitch=TRUE;
	   	      DragStartX=GameMouseX;
		      DragEndX=DragStartX;
		      DragStartY=GameMouseY;
		      DragEndY=DragStartY;
			}
	        // 왼쪽 버튼이 눌린 상태에서 풀린 것이라면, 
	        if(GameLeftPressSwitch==TRUE && leftpressswitch==FALSE)
			{
		        if(DragSwitch==TRUE)
				{
			       DragDoneSwitch=TRUE;
				}
		        DragSwitch=FALSE;
			}

			GameLeftPressSwitch=leftpressswitch;
	        GameRightPressSwitch=rightpressswitch;
	   }
		else
		{
		    // 마우스가 이동되었는가?
			if(moveswitch==TRUE)
			{
		       if(DragSwitch==TRUE)
			   {
			      DragEndX=GameMouseX;
			      DragEndY=GameMouseY;
			   }
			}
		}


	}
	
	

////////////////// 초기화 //////////////////////////

	// 마우스가 위치한 small 좌표를 초기화한다. 
	MapX = -1; 
	MapY = -1;
	
	UnderMouseID.Init();
	UnderMouseCropType  = 0;
	UnderMouseCropMount = 0;
	MouseMode           = 0;
	MenuMode            = 0;
	

	// 윈도우모드일땐 범위를 넓게 잡는다.
	if (WindowModeSwitch)
	{
		// 스크롤용 마우스를 설정한다. 
		if(abs(GameMouseX) <= SCROLL_RANGE)
		{
			if(abs(GameMouseY) <= SCROLL_RANGE)
			{
				if(ScrollDoneSwitch==TRUE)
					SetMouse(MOUSE_SCROLL_NW);
				else
					SetMouse(MOUSE_SCROLL_NW_X);
			}
			else if(GameMouseY==(clGrp.GetScreenYsize()-1))
			{
				SetMouse(MOUSE_SCROLL_SW);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_WEST);
			}
		}
		else if(abs(GameMouseX - (clGrp.GetScreenXsize()-1)) <= SCROLL_RANGE )
		{
			if(abs(GameMouseY) <= SCROLL_RANGE)
			{
				SetMouse(MOUSE_SCROLL_NE);
			}
			else if( abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE )
			{
				SetMouse(MOUSE_SCROLL_SE);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_EAST);
			}
		}
		else if(abs(GameMouseY) <= SCROLL_RANGE)
		{
			SetMouse(MOUSE_SCROLL_NORTH);
		}
		else if(abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE )
		{
			SetMouse(MOUSE_SCROLL_SOUTH);
		}
		// 마우스를 기본형으로 설정한다. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
		
	}
	else {
		// 스크롤용 마우스를 설정한다. 
		if(GameMouseX==0)
		{
			if(GameMouseY==0)
			{
				if(ScrollDoneSwitch==TRUE)
					SetMouse(MOUSE_SCROLL_NW);
				else
					SetMouse(MOUSE_SCROLL_NW_X);
			}
			else if(GameMouseY==(clGrp.GetScreenYsize()-1))
			{
				SetMouse(MOUSE_SCROLL_SW);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_WEST);
			}
		}
		else if(GameMouseX==(clGrp.GetScreenXsize()-1))
		{
			if(GameMouseY==0)
			{
				SetMouse(MOUSE_SCROLL_NE);
			}
			else if(GameMouseY==(clGrp.GetScreenYsize()-1))
			{
				SetMouse(MOUSE_SCROLL_SE);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_EAST);
			}
		}
		else if(GameMouseY==0)
		{
			SetMouse(MOUSE_SCROLL_NORTH);
		}
		else if(GameMouseY==(clGrp.GetScreenYsize()-1))
		{
			SetMouse(MOUSE_SCROLL_SOUTH);
		}
		// 마우스를 기본형으로 설정한다. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
	}



   // 판넬위에 마우스가 없다는 것을 초기화한다. 
   IsMouseInPannelSwitch   = FALSE;
   IsAllyWindowSwitch      = FALSE;
   IsMouseInMapSwitch      = FALSE;
   IsMouseInBigMapSwitch   = FALSE;
   IsMouseInSmallMapSwitch = FALSE;

/////////////////////////////////////////////////////

	// 작은 지도위에 있다면, 
	if(IsSmallMapValid() && SMap.IsPtInMap(GameMouseX, GameMouseY, mapx, mapy) == TRUE)
	{
	   
		// 유효한 지도 영역이어야 한다. 
		if(Map.IsInMapArea(mapx, mapy) == TRUE)
		{
			// 마우스가 지도 상에 있음을 확인한다. 
			IsMouseInMapSwitch       = TRUE;

			// 마우스가 작은 지도위에 있음을 확인한다. 
			IsMouseInSmallMapSwitch = TRUE;
			
			// 마우스가 위치한 지역을 얻는다. (기본좌표)
			MapX = mapx;
			MapY = mapy;
		}

	}
	// 마우스가 판넬위에 있다면, 
	else if(IsPtInPannel(GameMouseX, GameMouseY) == TRUE)
	{
		IsMouseInPannelSwitch = TRUE;
	}
	else if(IsPtInAllyWindow(GameMouseX, GameMouseY) == TRUE)
	{
		IsAllyWindowSwitch    = TRUE;
	}
	// 마우스가 큰 지도 위에 있을때 
	// 그리고 마우스가 다른 콘트롤위에 존재해서는 안된다. 
	else if(Map.IsPtInMap(GameMouseX, GameMouseY) == TRUE
	&& IsMosueInControlSwitch == FALSE)
	{
		// 마우스가 지도 상에 있음을 확인한다. 
		IsMouseInMapSwitch     = TRUE;

		// 마우스가 큰 지도위에 있음을 확인한다. 
		IsMouseInBigMapSwitch = TRUE;


		// 화면 좌표를 절대 좌표로 변환한다.
		Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, GameRealX, GameRealY);


		// 절대좌표를 근거로 해서 기본 좌표를 얻어온다.
		if(MapEditMode==TRUE)
		{
			SHORT small_map_center_x, small_map_center_y;
		    Map.DecideMapXYByXYLow(GameRealX, GameRealY, MapX, MapY, small_map_center_x, small_map_center_y);
		}
		else
		{
			clifMap.GetRealTileByRealDot(GameRealX, GameRealY, &MapX, &MapY);

			// 쿼터뷰 지도라서 무효한 테두리 영역이 선택된것이면 지도 위에 선택되어 있지 않은 것으로 한다. 
			if(MapX==0 && MapY==0)
			{
				// 원점과의 거리가 일정 범위를 벗어나면서 0,0좌표를 갖는다면 무효하다. 
				DWORD length=max(abs(MapCenterX[0][0]-GameRealX), abs(MapCenterY[0][0]-GameRealY));
				if(length>30)
				{
					MapX = -1;
					MapY = -1;

					// 마우스가 지도 상에 있음을 확인한다. 
		            IsMouseInMapSwitch=FALSE;
		            // 마우스가 큰 지도위에 있음을 확인한다. 
		            IsMouseInBigMapSwitch=FALSE;

				}
			}
		}

		// 마우스 아래의 캐릭터를 선택한다. 
		UnderMouseID = GetUnderMouseID();

	}

    OperateMenu();

	// 대상물이 필요하지 않을 때    
	if(GD.ObjectNeedSwitch == FALSE)
	{
		// 마우스의 위치와 지형과의 관계를 바탕으로 마우스가 처한 상황을 얻어온다.
		// 마우스가 지도 위에 있을 때에만 ....
		if(IsMouseInMapSwitch == TRUE)
		   GetMouseMode(MapX, MapY);
	
		// 2버튼 인터페이스 모드가 설정되어 있고 오른쪽 버튼이 눌리면, 자동 명령을 실행한다. 
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch == TRUE)
		{
           OperateAutoOrder();
		}

		// 드래깅이 완료되면, 
	   if(MapDragSwitch == TRUE && DragDoneSwitch == TRUE)
	   {
		   DragOperation();
	   }

	   if(DragSwitch == TRUE)
	   {
		   if(Map.IsInMapArea(MapX, MapY) == TRUE)
		   {
			   if(MapDragSwitch == FALSE)
			   {
				   if(IsMouseInBigMapSwitch == TRUE)
				   {
					   MapDragSwitch = TRUE;
					   MapDragStartX = MapX;
					   MapDragStartY = MapY;
					   MapDragEndX   = MapDragStartX;
					   MapDragEndY   = MapDragStartY;
				   }
			   }
		   }
	   }


	   if(MapDragSwitch == FALSE)
	   {
		   if(GameLeftPressSwitch == TRUE)
		   {
		      if(IsMouseInSmallMapSwitch == TRUE)
			  {

				  // 1버튼 인터페이스 방식이고 자동명령을 실행한다면, 
				  if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
				  {
				  }
				  // 화면이동으로 사용된다. 
				  else if(abs(Map.VirtualMapX-MapX)>5 || abs(Map.VirtualMapY-MapY)>5)
				  {
					  if(Map.IsInMapArea(MapX, MapY) == TRUE)
					  {
						  Map.VirtualMapX     = MapX;
						  Map.VirtualMapY     = MapY;
						  Map.DrawForceSwitch = TRUE;
					  }
				  }
			  }
			  
		   }
	   }

	   DragDoneSwitch = FALSE;
	}

	if(GD.ObjectNeedSwitch == TRUE)   // object 가 필요하다. 
	{
		SHORT buildkind = 0;

		// 선택된 명령에 따라 마우스를 설정한다. 
		if(GD.SelectedOrder == ORDER_MOVERIDEOUT
		|| GD.SelectedOrder == ORDER_TRAINSPOT
		|| GD.SelectedOrder == ORDER_CAPTUREBUILD)
		{
			SetMouse(MOUSE_RIDE);
		}
		else if(GD.SelectedOrder==ORDER_ATTACK
		||		GD.SelectedOrder==ORDER_ATTACKGROUND
		||      GD.SelectedOrder==ORDER_SELFEXP)
		{
			SetMouse(MOUSE_ATTACK);
		}
		else if(GD.SelectedOrder==ORDER_HEAL)
		{
			SetMouse(MOUSE_HEAL);
		}
		else if(GD.SelectedOrder == ORDER_MANA)
		{
			//마나
			SetMouse(MOUSE_HEAL);
		}
		else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_MAGIC) == TRUE)
		{
			SetMouse(MOUSE_MAGIC);
		}
			
		// 지뢰 매설이면 
		if(GD.SelectedOrder==ORDER_MINE)
		{
//			if(Map.IsInMapArea(MapX, MapY)==TRUE)
//			{
//				// 빈땅이어야 하고 지뢰가 이미 매설된 곳이면 안된다. 
//			   if(Map.IsEmpty(MapX, MapY)==TRUE && Map.GetMineMap(MapX, MapY) == -1)
//				    GD.CanBuildSwitch=TRUE;
//			   else GD.CanBuildSwitch=FALSE;
//			}
//			else 
//			   GD.CanBuildSwitch=FALSE;

			int a = 0;
		}

		// 유효한 지도 영영에서 마우스 왼쪽 버튼이 눌리면 임시 목적지가 선정된다.
		if(MapX!=-1 && MapY!=-1 && (GameLeftPressSwitch==TRUE))
		{
			ObjectTempSelectedSwitch = TRUE;
		}
		// 유효하지 않은 곳에서는 임시 목적지가 선정되지 않는다. 
		if(MapX==-1 && MapY==-1)
			ObjectTempSelectedSwitch=FALSE;

		// 자동 명령에 의해 선택된 목적지가 아니면, 
		if(GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
		{
			// 2버튼 방식이고, 오른쪽 버튼이 눌린 경우 
			if(pGame->Config.m_uiInterfaceMode==INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch==TRUE)
			{

				// 목적물을 필요로 하지 않는 상태로 만든다. 
	            GD.ObjectNeedSwitch=FALSE;

				// 선택된 명령을 취소한다. 
	            GD.SelectedOrder=0;
				// 명령 선택 옵션을 초기화한다. 
			    GD.SelectedOrderOption=0;
			}

		}

		// 임시 목적지가 선정되면, 
		if(ObjectTempSelectedSwitch==TRUE)
		{

			// 마우스 왼쪽 버튼이 떨어지면, 
            if(GameLeftPressSwitch==FALSE)
			{

				// 임시 목적지는 취소한다.
			   ObjectTempSelectedSwitch=FALSE;

		       switch(GD.ObjectNeedType)
			   {
			      case OBJECT1:
				  // 지뢰매설이고  지을 수 없으면obj를 받아들이지 않는다.  
					  if(GD.SelectedOrder 
					  && GD.SelectedOrder==ORDER_MINE)
					  {
						  SetMessage2(Text.CannotMakeThereMessage.Get(), UserPlayerNumber, EFFECT_CANNOTMAKETHERE);
					  }
					  // 수확이고 
					  else if(GD.SelectedOrder  && GD.SelectedOrder==ORDER_HARVEST)
					  {
						  if(MouseModeHarvest(MapX, MapY, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE)
						  {
							  // obj를 선택한다.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MouseHarvestX, MouseHarvestY, UnderMouseID);
						  }
						  else
						  {
							  // obj를 선택한다.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MapX, MapY, UnderMouseID);

						  }
					  }
					  else
					  {
					     // obj를 선택한다.  
			             GD.ObjectNeedSwitch=FALSE;
				         GD.PushObject(MapX, MapY, UnderMouseID);
					  }
				     break;
			      default:
				     break;
			   }
			}
		}
	}

	// 오른쪽  마우스 버튼이 눌렸을때의 처리 
	if(IpD.RightPressDoneSwitch==TRUE)
	{
		IpD.RightPressDoneSwitch=FALSE;

		// 마우스가 메뉴 버튼위에 있지 않아야 된다. 
		if(MenuOfMouse<0)
		{

			// 1버튼 인터페이스가 설정되어 있으면 오른쪽 버튼은 선택 취소로 작동한다. 
		    if(pGame->Config.m_uiInterfaceMode==INTERFACEMODE_1BUTTON)
			{

             Unselect();
             ClearMenu();

	         GD.ObjectNeedSwitch=FALSE;

	         GD.SelectedOrder=0;
			 GD.SelectedOrderOption=0;
			}

		}
	}


	if(GD.SelectedOrder)
	{

		switch(GD.ObjectNeedType)
		{
		case NOOBJECT:

			if(Order[GD.SelectedOrder].IsAtb(ORDERATB_UPDATEMENU)==TRUE)
			{
				// 선택된 캐릭터의 메뉴를 업데이트한다. 
				for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
				{
					if(IsSee(GD.SelectedID[i].ID))
					{
						clifChar.UpdateMenu(GD.SelectedID[i].ID, GD.SelectedOrder);
					}
				}
			}
			else
			{
				DWORD para1;

				// 오른쪽 버튼 선택으로 입력받은 명령은, 
				if(GD.SelectedOrderOption&SELECTEDORDEROPTION_RIGHTPRESS)
				{
					para1=TRUE;
				}
				else
					para1=FALSE;


			    if(ControlKeyPressSwitch==TRUE)
   		            MakeOrder(UserPlayerNumber, GD.SelectedOrder, para1, ZeroID, FALSE);
			    else 
				    MakeOrder(UserPlayerNumber, GD.SelectedOrder, para1, ZeroID, TRUE);
			}

			GD.SelectedOrder=0;
			GD.SelectedOrderOption=0;
			GD.ObjectBufferIndex=0;
			break;

		case OBJECT1:

			// 이미 대상이 선정되어 있다면, 
			if(GD.ObjectNeedSwitch==FALSE)
			{

				// 선택된 명령이 이동일때 이동 마크를 표시하게 하고 이동 효과음을 낸다. 
				if( GD.SelectedOrder == ORDER_MOVE
				||  GD.SelectedOrder == ORDER_PATROL
				|| (GD.SelectedOrder == ORDER_ATTACK && GD.ObjectBuffer[0].ID.IsEmpty()) )	
				{
					// 선택된 유닛이 이동 명령을 받을때 효과음을 내게 만든다. 
					int effectnumber = 0;

					for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
					{
						id = GD.SelectedID[(i + GD.Rand) % MAX_SELECTED_CHAR_NUMBER].ID;

						if(IsSee(id) == TRUE)
						{
							if(effectnumber == 0)
							{
								if(KI[ clifChar.GetKind(id) ].m_siMoveEffect)
								{
#ifndef _USE_HASHCODE_EFFECTSOUND									
							        PushEffect(KI[ clifChar.GetKind(id) ].m_siMoveEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#else
							        HashCodePushEffect(KI[ clifChar.GetKind(id) ].m_siMoveEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#endif

							         effectnumber++;
								}
							}
						}
					}

					SHORT realx, realy;
					Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, realx, realy);
					GD.MoveMark.SetMoveMarkReal(realx, realy);

				}
				// 선택된 효과음이 공격일때 공격 효과음을 낸다.
				else if(GD.SelectedOrder==ORDER_ATTACK )
				{
					int effectnumber=0;
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						id=GD.SelectedID[(i+GD.Rand)%MAX_SELECTED_CHAR_NUMBER].ID;

						if(IsSee(id)==TRUE && clifChar.CanAttack(id, GD.ObjectBuffer[0].ID)==TRUE)
						{
							if(effectnumber==0)
							{
 								if(KI[ clifChar.GetKind(id) ].m_siAttackEffect)
								{
#ifndef _USE_HASHCODE_EFFECTSOUND
							        PushEffect(KI[ clifChar.GetKind(id) ].m_siAttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#else
							        HashCodePushEffect(KI[ clifChar.GetKind(id) ].m_siAttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#endif
							         effectnumber++;
								}
							}
						}
					}
				}
				// 선택된 효과음이 수확일떄 
			    else if(GD.SelectedOrder==ORDER_HARVEST)
				{
					// 선택된 유닛이 공격이나 수확 명령을 받을때 효과음을 내게 만든다. 
					int effectnumber=0;

					// 대상지의 곡물 종류를 확인한다. 
					SHORT crop_type = clifMap.GetCropTypeMap(GD.ObjectBuffer[0].X, GD.ObjectBuffer[0].Y);

					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						id=GD.SelectedID[(i+GD.Rand)%MAX_SELECTED_CHAR_NUMBER].ID;


						if(IsSee(id)==TRUE && IsCropHarvest(id, crop_type)==TRUE)
						{
							if(effectnumber == 0)
							{
								if(KI[ clifChar.GetKind(id) ].m_siAttackEffect)
								{
#ifndef _USE_HASHCODE_EFFECTSOUND
							        PushEffect(KI[ clifChar.GetKind(id) ].m_siAttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#else
							        HashCodePushEffect(KI[ clifChar.GetKind(id) ].m_siAttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#endif
							         effectnumber++;
								}
							}
						}
					}
				}
				// 집결지 설정을 표시한다. 
				else if(GD.SelectedOrder == ORDER_TRAINSPOT)
				{
					if(GD.ObjectBuffer[0].ID.IsEmpty())
					{
					   SHORT realx, realy;
					   Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, realx, realy);
					   GD.MoveMark.SetMoveMarkReal(realx, realy);

					}
				}
				// 건설 명령이면 
				// 지뢰 매설 명령이면, 
				else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_BUILD)
				|| GD.SelectedOrder==ORDER_MINE)
				{
					PushEffect(EFFECT_DECIDEBUILD, 0, 0);
				}


				// 선택된 캐릭터가 있으면, 
				if(GD.ObjectBuffer[0].ID.IsEmpty()==FALSE)
				{
					if(IsAlive(GD.ObjectBuffer[0].ID.ID)==TRUE)
					{
						// obj로 선택된 캐릭터의 newselectedswitch를 설정한다.  
						clifChar.SetNewSelectedSwitch(GD.ObjectBuffer[0].ID.ID, TRUE);
					}
				}

				if(ControlKeyPressSwitch == TRUE)
				    MakeOrder(UserPlayerNumber, GD.SelectedOrder, GD.ObjectBuffer[0].X | (GD.ObjectBuffer[0].Y << 16), GD.ObjectBuffer[0].ID, FALSE);
				else
					MakeOrder(UserPlayerNumber, GD.SelectedOrder, GD.ObjectBuffer[0].X | (GD.ObjectBuffer[0].Y << 16), GD.ObjectBuffer[0].ID, TRUE);


				GD.SelectedOrder         = 0;
				GD.SelectedOrderOption   = 0;

				GD.ObjectBufferIndex=0;
			}

			break;
		}
	}

	return FALSE;
}


void InitGameSetCharOnline(SI32 &userpointindex)
{
#ifdef _ONLINE
	SI32 i, j, id;
	SI32 x, y;
	SI32 maxoff;
	SI32 kind;
	SI32 index = 0;


  	// 플레이어 시작 위치에 적절한 캐릭터를 배치한다.
	SHORT posindex=0;

	for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{
		maxoff = 1;
		
		// 유효하지 않은 플레이어는 없앤다. 
 		if(OBInfo.OIBPI[i].siPlayersUniqueID == 0)continue;
		
		index = 0;

		for(j = 0;j < MAX_SET_CHAR_POINT_NUMBER; j++)
		{
			if(index >= ON_MAX_FOLLOWER_NUM)continue;
			
			if(OBInfo.OIBPI[i].uiKind[index] == 0)
			{
				index++;
				continue;
			}
			else
				kind = TransKindOnlineToImjin(OBInfo.OIBPI[i].uiKind[index]);

			//체력이 없는 캐릭터는 만들지 않는다. 
			if(OBInfo.OIBPI[i].Iparameta[index].GetLife() > 0)
			{
				id = FindEmptyID(index);
				
				if(id)
				{
					x = Map.clMapPlayerInfo[posindex].StartPoint.x + SetCharPoint[j].x; 
					y = Map.clMapPlayerInfo[posindex].StartPoint.y + SetCharPoint[j].y;

					if( Map.IsInMapArea(x, y) == TRUE && Map.IsEmpty(x, y))
					{
						// CR[id]에 캐릭터를 등록한다.
						SetChar(kind, id, x, y, SOUTH, 100, i, &OBInfo.OIBPI[i].Iparameta[index], &OBInfo.OIBPI[i].MW[index], index);

						OBInfo.OIBPI[i].Iparameta->GetLife();
						OBInfo.OIBPI[i].Iparameta->GetMana();


						
						// 사람 플레이어이면 전송 받은 이름을 사용한다. 
					 	if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
						{
							
							if(strcmp(OBInfo.OIBPI[i].szPlayerName[index], "") != 0)
							{
								CR[id].SetName(OBInfo.OIBPI[i].szPlayerName[index], ON_ID_LENGTH);
							}
							else
							{
								clGrp.Error("NO name", "kind:%d", OBInfo.OIBPI[i].uiKind[index]);
							}
						}
						// NPC이면 이미 저장된 이름을 사용한다. 
						else
						{
							CR[id].SetName(KI[kind].m_pszName, ON_ID_LENGTH);
						}

						index++;
					}
				}
				else
				{
					clGrp.Error("SetChar Error", "Can't Find EmptyID!");
					index++;
				}
			}
			else
				index++;
		}
			
		if(i == UserPlayerNumber)	userpointindex = posindex;
		posindex++;
	}
#endif
}


BOOL FindProperOnlineBattleMap(SI32 playercount, SI32 maptype, char* mapname)
{
	FILE	*fp;

	CHAR	buffer[1024];
	SI32    xsize, ysize;
	SI32	type;
	CHAR	delimiter[] = "`\t\n";
	CHAR	*pToken = NULL;
	INT		index;
	SI32 mapplayercount;
	char name[128];
	char *mapnamelist[100];
	SI32 i;

	if(maptype == 2 || maptype == 3 || maptype == 4 || maptype == 5 || maptype == 6 || maptype == 7)
	{
	}
	else
	{
		maptype = 1;
	}

	// savepath를 고려하여 화일이름을 보정한다. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "OnlineBattleMap.dat" );

	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// 첫번째 6줄은 무시한다. 
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);

	index = 0;

	while( fgets(buffer, 1024, fp) != NULL)
	{ 
		sscanf(buffer, "%s %d %d %d %d", name, &xsize, &ysize, &mapplayercount, &type);

		if(playercount <= mapplayercount && mapplayercount <= playercount + 3
		&& maptype == type)
		{
			if(index < 100)
			{
				mapnamelist[index] = new char[128];
				strcpy(mapnamelist[index], name);
				index++;
			}
		}
	}

	fclose(fp);

	if(index)
	{
		
		SI32 data = GD.Random() % index;
		strcpy(mapname, mapnamelist[data]);
		// 할당한 메모리를 해제한다. 
		for(i=0;i<index;i++)
		{
			delete[] mapnamelist[i];
		}

		return TRUE;
	}

	return FALSE;

}



void SetScrollProcSwitch(BOOL mode)
{
	ScrollProcSwitch = mode;
}


// OnlineBattleInfo의 정보가 유효한지 알려준다. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo)
{
	SI32 i, j;
	SI32 playerswitch = FALSE;
	SI32 playercount  = 0;

	for(i=0; i<MAX_MULTI_PLAYER; i++)
	{
		// 사용자가 없는 경우, 
		// 사람인 경우에만 이름이 있다는 것을 근거로 체크.
		if(pobinfo->OIBPI[i].szPlayerName[0])
		{
			playerswitch = TRUE;
		}
		// 전투에 참여하는 사용자의 
		if(pobinfo->OIBPI[i].siPlayersUniqueID)
		{
			playercount++;
		}
 
		for(j=0;j<ON_MAX_FOLLOWER_NUM;j++)
		{ 
			// 보너스 점수가 마이너스인 경우, 
			if(pobinfo->OIBPI[i].Iparameta[j].GetBonus()  < 0)
			{
				clGrp.Error("Minus Bonus", "Bonus:%d", pobinfo->OIBPI[i].Iparameta[j].GetBonus());
				pobinfo->OIBPI[i].Iparameta[j].SetBonus(0);
				return FALSE;
			}
		}
	}


	// 전투에 임하는 사용자가 없는 경우라면, 
	if(playerswitch == FALSE)
	{
		clGrp.Error("No Human Player", "[%s][%s][%s][%s][%s][%s][%s]", 
			pobinfo->OIBPI[0].szPlayerName,
			pobinfo->OIBPI[1].szPlayerName,
			pobinfo->OIBPI[2].szPlayerName,
			pobinfo->OIBPI[3].szPlayerName,
			pobinfo->OIBPI[4].szPlayerName,
			pobinfo->OIBPI[5].szPlayerName,
			pobinfo->OIBPI[6].szPlayerName);
		
		return FALSE;
	}
	
	// 전투에 참여한 플레이어 수가 2보다 적으면, 
	if(playercount < 2)
	{
		clGrp.Error("Not Enough Players", "PlayerCount:%d", playercount);
		return FALSE;
		
	}
	// 전투에 참여한 플레이어 수가 7보다 많으면, 
	if(playercount > 7)
	{
		clGrp.Error("Too Many Players", "PlayerCount:%d", playercount);
		return FALSE;
	}
	
	return TRUE;
}

// 퇴각에 의해서 경험치 손실을 치룬다.
void SendBattleMessage_LoseForRetreat()
{

	// 해킹 우려가 없을때에만 전송한다. 
	{
		// 주인공 캐릭터의 레벨을 얻어온다. 
		SI32 level = min(100, OBInfo.OIBPI[UserPlayerNumber].Iparameta[0].GetLevel());
		
		SI32 exp = min(0, -GetExpReduceForDying(level)/2);

		if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
		{
			for(SI16 id = 0 ; id < 200; id++)
			{
				if(CR[id].GetPlayerNumber() == UserPlayerNumber)
				{
					if(IsAlive(id))
					{
						if((TransKindImjinToOnline(CR[id].m_cltCharStatus.GetKind()) & 255) == 'C')
						{
							if(CR[id].m_ParmeterBox.IP.CheckCheckSum() == TRUE)
							{
								OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[UserPlayerNumber ], 0, exp);

								char buffer[128];
								wsprintf(buffer, Text.ExpLost.Get(), -exp);
								SetMessage(buffer);
							}
						}
					}
				}
			}
		}
		
		
	}
//	else
//	{
//		SetMessage("해킹의 가능성이 있는 데이터입니다.");
//		m_ParmeterBox.IP.SetLife(0);
//	}
	
}

/*
#else

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <Mouse.h>
#include <MainFunction.h>

#include <clGame.h>
#include "clItemInfoWindow.h"

#include <setchar.h>
#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <etc.h>
#include <item.h>
#include "..\Gersang\crop.h"
#include "..\Gersang\screenpoint.h"
#include "..\Gersang\music.h"
#include <SoundManager.h>
#include <chat.h>

#include "..\Gersang\mapedit.h"
#include "..\Gersang\stage.h"
#include "../SCCommon/OnlineBattleMsg.h"
#include "DebugTool.h"

#include <map.h>
#include <volcano.h>


#include <OnlineImjin2i.h>

#include <OnlineWorld.h>
#include <OnlineNoticeList.h>

#include <OnlineBattleMsg.h>
#include "..\Gersang\directx.h"
#include "..\Gersang\text.h"
#include "..\Gersang\help.h"
#include "..\Gersang\yprg.h"
#include <CharOrg.h>

#include <yPutGetResource.h>

#include <ysmap.h>

#include <map.h>
#include <mapinterface.h>
#include <charinterface.h>
#include <magic.h>
#include <GameButton.h>
#include <bullet.h>
#include <smoke.h>
#include <Game2.h>
#include <onlineImjin2i.h>
#include <MyNet-Game.h>
#include <myfile.h>
#include "bindJxFile.h"


// OnlineBattleInfo의 정보가 유효한지 알려준다. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo);

extern _clGame *pGame;


extern	SHORT 				GamePauseSwitch;					// 게임이 잠시 중단되었는가?
extern  _Chat 				Chat;
extern	RECT				ScreenRect;							// 지도가 표시되는 영역 
extern	_ScreenPoint 		SP;									// 화면에 사용될 여러 정보 
extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	_YSMap 				SMap;								// 작은 지도용 데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	XSPR 				PannelSpr;
extern	SHORT 				MapEditMode;						// 지도 편집 모드인가?
extern	_MapEdit 			MapEdit;
extern	SHORT 				GameMenuShowSwitch;					// 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	DWORD				PrevMultiClock;
extern	DWORD				MessageSendFrame;					// 명령이나 NO_MESSAGE를 보낸 프레임 
extern	BOOL				GameDropSwitch ;
extern	int 				DragSwitch;
extern	int 				MapDragSwitch;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern	DWORD 				GameRecordSwitch;
extern	clock_t				StartClock, CurrentClock;						// 시간 관련 
extern	_MyNetGame			MyNet;
extern	SI32				MaxMapXsize;
extern	SI32				MaxMapYsize;
extern	char 				SavePath[];
extern	UI08 				ScreenMapBuffer[];					// 화면에 찍을 지도를 임시로 저장하는 곳 
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				MapX, MapY;							// 마우스가 위치한 small 좌표 
extern	int 				DragStartX, DragStartY, DragEndX, DragEndY;
extern	_FileVary 			FV;
extern	_Help 				Help;
extern	SHORT				GameStatus;							// 현재의 게임 상황을 알리는 변수 
extern	SHORT				GameStatusBuffer;
extern	int					bActive;							// 현재 액티브인지 아닌지 알려주는 변수  
extern _InputDevice			IpD;
extern	int					CDStopSwitch;						// CD음악을 정지 시킨다. 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	int 				MouseMode;							// 마우스가 처한 상태를 저장 
// 키보드 관련
extern	int 				ControlKeyPressSwitch;
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameRealX, GameRealY;
extern	SHORT 				UnderMouseCropType;
extern	SHORT 				UnderMouseCropMount;
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	int 				MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
extern	_NewID 				UnderMouseID;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	DWORD 				GameWParam;
extern	int 				DragDoneSwitch;
extern	int 				MenuMode;							// 메뉴의 상태를 저장 
extern	BOOL 				IsMosueInControlSwitch;				// 마우스가 게임 콘트롤위에 위치하고 있다.
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_Order 				Order[];
extern	_Text 				Text;								// 텍스트 
extern	SHORT 				MouseHarvestX, MouseHarvestY;
extern	SHORT 				MenuOfMouse;						// 마우스 커서가 위치하고 있는 메뉴의 번호 
extern	_NewID 				ZeroID;
extern  char 				UserSelectMapFileName[];			// 사용자가 선택한 지도 이름을 저장할 공간   
extern	int   				MultiPlayDelayTime;
extern  BOOL				bGameAllyWindowSwitch;

extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;			// 캐릭터 그림용
extern LPDIRECTDRAWSURFACE7 lpPannelSurface;		// 판넬용 면 

extern BOOL IsMouseInPannelSwitch  ;
extern BOOL	IsAllyWindowSwitch;

extern	_OnlineBattleMsgQue OBMQue;


extern OnlineBattleInfo OBInfo;
// 최대 프레임 이격 
extern SHORT DPMaxFrameDiff;

extern clock_t FrameDelay ;   // 프레임당 클럭
extern int FrameClockDelay;

extern SoundManager	*g_pSoundManager;

static DWORD PrevFrameClock;
static DWORD PrevGameWinConditionFrame;
static BOOL IsMouseInPannelSwitch   = FALSE;
static BOOL IsAllyWindowSwitch      = FALSE;
static BOOL ScrollProcSwitch;

int ObjectTempSelectedSwitch=FALSE;
int Scroll;
SHORT ScrollDoneSwitch=FALSE;  // 스크롤이 이루어졌는가?
SHORT DoubleClickedID;  // 더블 클릭으로 선택된 아이디 

BOOL IsMouseInMapSwitch      = FALSE;
BOOL IsMouseInBigMapSwitch   = FALSE;
BOOL IsMouseInSmallMapSwitch = FALSE;

static SI32 BattleMusic	= 0;
OnlineNoticeList *pOnlineNoticeList;

clock_t FrameDelay = 0;   // 프레임당 클럭

static BOOL GameQuitSwitch = FALSE;

extern int WindowModeSwitch;
extern HDC g_hBattleScreenDC;

#define MAX_SET_CHAR_POINT_NUMBER   21
POINT SetCharPoint[MAX_SET_CHAR_POINT_NUMBER]=
{
	{0,0},		{0,2},	{-2,0}, {0,-2}, {2,0}, 
	{-2,-2},	{2,-2}, {-2,2}, {2,2},  {0, -4},
	{4,0},		{0,4},	{-4,0}, {2,-4}, {-2,-4},
	{2,4},		{-2,4}, {-4,2}, {-4,-2},{4,2},
	{4,-2},
};

#define SCROLL_RANGE         6

void InitGame1()
{

	// 팔레트를 설정한다. 
	SetGamePalette(clGrp.Palette);
	InitDarkTable(clGrp.Palette);
	SetMouse(MOUSE_NORMAL);

	

	if(pGame == NULL)
	{
		clGrp.Error("Error", "fdei82hsdas");
	}
	else if(pGame->pBattle == NULL)
	{
		clGrp.Error("Error", "fdei82hsdas11");
	}
	else
	{
		pGame->pBattle->InitGameButton();
	}


	// 이전에 게임이 중단되어 있었다면 속개한다. 
	GamePauseSwitch=FALSE;
	bGameAllyWindowSwitch = FALSE;

	// 이전에 텍스트 입력 모드가 설정되어 있었다면 해제한다. 
	Chat.SetInputModeSwitch(FALSE, 0);


	// 사용자의 서피스를 제거한다. 
	CustomSurfaceRelease();
	
	// 임의의 서피스를 다시 만든다. 
	CreateSurface();


	// 화면의 영역을 설정한다. 
	ScreenRect.left=0;
	ScreenRect.right=CLIENT_SCREEN_XSIZE-1;
	ScreenRect.top=0;
	ScreenRect.bottom=CLIENT_SCREEN_YSIZE-1;

	clGrp.SetClipArea((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, (SHORT)ScreenRect.right, (SHORT)ScreenRect.bottom);

	SP.Init();

}



void InitGame2()
{
	SHORT i, j, k;


	// 작은 지도를 초기화 한다.
	SMap.Init(Map.GetXsize(), Map.GetYsize());
	
	
	Map.MakeSmallMapBase();

	
	if(GD.GameMode == GAME_MODE_ONLINEBATTLE)
	{
		//작은 지도를 항상 유효화 시킨다. 
		GD.SetSmallMapValidSwitch(TRUE);

		// 자동 마법 사용 불가. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// 모든 업그레이드 완료한다.
		GD.SetUpgradeAllSwitch(TRUE);

		// 온라인 게임의 스피드로 설정한다. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// 캐릭터들의 시야를 직선으로 처리힌다. 
		GD.SetStraightSightMode(TRUE);

	}
	if(GD.GameMode == GAME_MODE_HERO)
	{
		//작은 지도를 항상 유효화 시킨다. 
		GD.SetSmallMapValidSwitch(TRUE);

		// 자동 마법 사용 불가. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// 모든 업그레이드 완료한다.
		GD.SetUpgradeAllSwitch(TRUE);

		// 온라인 게임의 스피드로 설정한다. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// 캐릭터들의 시야를 직선으로 처리힌다. 
		GD.SetStraightSightMode(TRUE);

	}


	LoadPannel(GD.Player[UserPlayerNumber].PI.Country);


	// 판넬을 surface로 옮긴다.
	if(clGrp.LockSurface(lpPannelSurface)==TRUE)
	{
		clGrp.PutImageT(0, 0, 
		          PannelSpr.Header.Xsize, 
			      PannelSpr.Header.Ysize,
		          PannelSpr.Image);

		clGrp.UnlockSurface(lpPannelSurface);
	}

	if(MapEditMode==TRUE)
	{
		MapEdit.Init();
	}


	// 설정된 값에 따라 기본적인 속도를 얻어온다.
	FrameDelay = pGame->Config.GetRealGameSpeedClock();

	// 키버퍼를 초기화하여 이전의 입력을 무시한다. 
	GD.KB.InitKeyBuffer();

	// 게임 메뉴를 보이지 않도록 만든다. 
	GameMenuShowSwitch=FALSE;

	InitFrameClock();

	InitGetResource();

	PrevGameWinConditionFrame = 10000;
	PrevMultiClock   = 0;
	MessageSendFrame = 0;  // 명령이나 NO_MESSAGE를 보낸 프레임 

	GameDropSwitch = FALSE;


	// 지도 드래깅을 초기화한다. 
	DragSwitch = FALSE;
	MapDragSwitch = FALSE;
	GameLeftPressSwitch = FALSE;
	GameRightPressSwitch = FALSE;

	// 영어 모드
	ChangeLanguage(FALSE);


	if(MapEditMode == TRUE)
	{
		for(i=0;i<Map.GetYsize();i++)
			for(j=0;j<Map.GetXsize();j++)
			{
				GD.BlackMap[UserPlayerNumber][j][i]=0;
			}
	}
	else
	{
		// 관전 모드이면 모든 화면을 다 본다. 
		for(k=0;k<MAX_MULTI_PLAYER;k++)
		{
			if(GD.Player[k].PI.Country == COUNTRY_NEUTRAL)
			{
				for(i=0;i<Map.GetYsize();i++)
					for(j=0;j<Map.GetXsize();j++)
					{
						GD.BlackMap[k][j][i]=0;
					}
			}
		}
	}

	if(GD.Player[UserPlayerNumber].PI.Country == COUNTRY_NEUTRAL)
	{
		//작은 지도를 항상 유효화 시킨다. 
		GD.SetSmallMapValidSwitch(TRUE);
	}		
	


	// 빠른 게임 전개 모드이면 모든 업그레이드를 다 완료시킨다. 
#ifdef _QUICKMODE
	GD.SetUpgradeAllSwitch(TRUE);
#endif

	PrevMultiClock = CurrentClock;

}


// 게임에 들어가기 전에 초기화해주는 함수 
//BOOL InitNoStageGame()
//{
//	SI32 userpointindex = 0;
//
//	// 게임의 주 데이터를 초기화한다. 
//	GD.Init();
//	InitVolcano();
//
//	// 멀티세팅에서 값이 나오면, 
//   GD.RandomSeed = 1;
//
//	// 게임 방식을 정한다. 
//	GD.GameMode = GAME_MODE_NORMAL;
//
//	Map.InitMap(100, 100, FALSE);
//
//	
//	Map.SetMapDrawSwitch(TRUE);
//
//	// 지도용 화일을 불러온다. 
//	Map.MH.MapType = MAP_TYPE_NORMAL;
//	LoadMapTile(Map.MH.MapType);
//	
//	// 화면을 시작위치로 옮긴다. 
//	Map.VirtualMapX=0;
//	Map.VirtualMapY=0;
//	
//	GD.HomeX=Map.VirtualMapX;
//	GD.HomeY=Map.VirtualMapY;
//
//	return TRUE;
//}

BOOL InitOnlineBattleGame()
{

	SHORT i, j;
	char buffer[FILE_NAME_SIZE];
	SI32 userpointindex;

	MyNet.Init();

	// 게임의 주 데이터를 초기화한다. 
	GD.Init();
	InitVolcano();



/////////////////////////////////////////////////////////////////////////////////////

	// 내 컴의 아이디. 
	OBInfo = GOnlineImjin2I.BattleInfo;
	
	
	UI32 size = 0;
	UI32 startclock = timeGetTime();
	
	
	// 지연을 근거로 각종 통신 변수들을 설정한다. 
	MyNet.DecideTransVary(300);
	
	// 전투에 필요한 정보를 얻어온다.
	
	// 잘못된 정보가 없는지 확인한다. 
	if(MapEditMode == FALSE)
	{
		CheckOnlineBattleInfo(&OBInfo);
	}
	
	// 플레이어의 구체적인 정보를 얻어온다. 
	
	
	
	for( i = 0; i < MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.dpId[i] = OBInfo.OIBPI[i].siPlayersUniqueID;
		
		// 사용자, 혹은 몬스터인경우. 
		if(OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			// 사람인 경우에만 이름이 있다.
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				
				strncpy(MyNet.PlayersName.Name[i], OBInfo.OIBPI[i].szPlayerName[0], ON_ID_LENGTH);
				
				// 널문자를 넣어준다. 
				MyNet.PlayersName.Name[i][ON_ID_LENGTH] = NULL;
				
			}
			else
				strcpy(MyNet.PlayersName.Name[i], "MONSTER");
		}
	}

	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.Player[i].Init(i);

		// 유효한 플레이이어인 경우 , 
		if(MyNet.PlayersdpId.dpId[i]) 
		{
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				MyNet.PlayersdpId.Player[i].Host	= BY_PLAYER;
				MyNet.PlayersdpId.Player[i].Country = COUNTRY_KOREA; 				
			}
			else if(ISMONSTER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				MyNet.PlayersdpId.Player[i].Host	= BY_COMPUTER;
				MyNet.PlayersdpId.Player[i].Country = COUNTRY_KOREA;   
			}
			else if(ISSHIP(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
			}
			
			// 자기 자신은 자신에게 동맹이다. 
			GD.OnlineAllyNumSwitch[i][i] = TRUE;
		}
	}

	// 서버는 항상 1이다. 
	MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1]				= 1;
	MyNet.PlayersdpId.Player[MAX_MULTI_PLAYER-1].Country	= COUNTRY_NEUTRAL;
	MyNet.HostPlayerDpId = MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1];;


	MyNet.PlayersdpId.GameMode= GAME_MODE_NORMAL;

	// 사용자의 번호를 구한다. 
	for(i = 0;i < MAX_MULTI_PLAYER;i++)
	{
		if(GOnlineImjin2I.BattleInfo.siMyCharUniqueID == OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			UserPlayerNumber = i;
		}
	}
	if(MapEditMode)
		UserPlayerNumber = 0;
	
	MyNet.PlayersdpId.Player[UserPlayerNumber].Host = BY_PLAYER;


/////////////////////////////////////////////////////////////////////////////////////

	// 랜덤 시드를 얻어온다.
	if(MapEditMode == TRUE)
	{
		GD.RandomSeed		= 1;
	}
	else
	{
		GD.RandomSeed		= OBInfo.uiRandomSeed;
	}

	if(GD.RandomSeed	== 0)
	{
		clGrp.Error("3209873", "3210847");
		GD.RandomSeed = 1;
	}


   //////////////////////////////
   // NPC간은 동맹으로 설정한다. ///
   //////////////////////////////
   
   SI32 mon_player = 0;
	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		// 유효한 플레이이어인 경우 , 
		if(MyNet.PlayersdpId.dpId[i]) 
		{
			// NPC간은 동맹을 설정한다. 
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == FALSE)
			{
				for( j = 0; j < MAX_MULTI_PLAYER; j++ )
				{
					if(ISPLAYER(MyNet.PlayersdpId.dpId[j]) == FALSE)
					{
						GD.OnlineAllyNumSwitch[i][j] = TRUE;
						GD.OnlineAllyNumSwitch[j][i] = TRUE;
					}
				}
			}
		}
	}

	// 플레이어 간에는  동맹을 맺는다. 
	// PK가 허용되지 않는 필드에서만..(허용되는 필드는 아무하고도 동맹하지 않는다.)
	if(pGame->pOnlineWorld && pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
	{
		if(MyNet.PlayersdpId.dpId[0] && MyNet.PlayersdpId.dpId[1])
		{
			// 플레이어간에 전투를 시작했을때(PK)
			if(ISPLAYER(MyNet.PlayersdpId.dpId[1]) == TRUE)
			{
				for(i=0; i<(MAX_MULTI_PLAYER-1); i++)
				{
					if(!MyNet.PlayersdpId.dpId[i])				continue;
					if(!ISPLAYER(MyNet.PlayersdpId.dpId[i]))	continue;
					for(j=0; j<(MAX_MULTI_PLAYER-1); j++)
					{
						if(!MyNet.PlayersdpId.dpId[j])			continue;
						if(!ISPLAYER(MyNet.PlayersdpId.dpId[j]))continue;

						BOOL iHostility = pGame->pOnlineWorld->pIOnlineChar->GetIsHostilityGuild(MyNet.PlayersdpId.dpId[i]);
						BOOL jHostility = pGame->pOnlineWorld->pIOnlineChar->GetIsHostilityGuild(MyNet.PlayersdpId.dpId[j]);
						if((iHostility&&jHostility)||(!iHostility&&!jHostility))
							GD.OnlineAllyNumSwitch[i][j] = TRUE;
					}
				}
			}
			// 몹과의 전투
			else
			{
				for(i=0; i<(MAX_MULTI_PLAYER-1); i++)
				{
					if(!MyNet.PlayersdpId.dpId[i])				continue;
					if(!ISPLAYER(MyNet.PlayersdpId.dpId[i]))	continue;
					for(j=0; j<(MAX_MULTI_PLAYER-1); j++)
					{
						if(!MyNet.PlayersdpId.dpId[j])			continue;
						if(!ISPLAYER(MyNet.PlayersdpId.dpId[j]))continue;

						GD.OnlineAllyNumSwitch[i][j] = TRUE;
					}
				}
			}
		}
	}

	// 게임 방식을 정한다. 
	GD.GameMode = GAME_MODE_ONLINEBATTLE;
	
	char mapname[128]="";
	
	// 지형의 타입을 얻어온다. 
	if(MapEditMode == TRUE)
	{
		Map.MH.MapType = MAP_TYPE_NORMAL;
		
		Map.InitMap(100, 100, FALSE);
		
	}
	else
	{
		
		// 게임에 등장하는 플레이어가 몇 명인지 확인한다. 
		SHORT playercount=0;
		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			// 등장하지 않는 국가이면, 
			if(MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NO
				&& MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NEUTRAL)
			{
				playercount++;
			}
		}
		
		// 플레이어 숫자가 비정상적이면 알린다. 
		if(playercount < 2)
		{
			clGrp.Error("f9837521", "[%d]", playercount);
		}
		
		SI32 maptype = pGame->pOnlineWorld->GetWarFieldMap();
		if(FindProperOnlineBattleMap(playercount, maptype, mapname) == TRUE)
		{
			
			// 지도를 초기화한다.
			//				strcpy(buffer, SavePath);
			strcpy(buffer, "Online\\BattleMap\\");
			strcat(buffer, mapname);
			
			Map.Load(buffer);
		}
		else
		{
			clGrp.Error("125308", "no map [%d]", playercount);
		}
		
		// 확장 기지를 설정한다.
		Map.ExpansionBaseNumber=0;
		for(i = 0;i < MAX_MULTI_PLAYER;i++)
		{
			if(Map.clMapPlayerInfo[i].StartPoint.x >= 0 && Map.clMapPlayerInfo[i].StartPoint.y >= 0)
			{
				Map.PushExpansionBasePoint((SHORT)Map.clMapPlayerInfo[i].StartPoint.x, (SHORT)Map.clMapPlayerInfo[i].StartPoint.y);
			}
		}
		
	}

	//시작하는 위치를 랜덤하게 바꾼다.
	Map.RandomStartPos();
	
	// 지도용 화일을 불러온다. 
	LoadMapTile(Map.MH.MapType);
	Map.SetMapDrawSwitch(TRUE);

    // 불러오거나 임의로 설정된 지도를 최종적으로 확정한다. 
    Map.MakeLastMap();

	   // 세팅에서 정해진 설정을 넘겨준다. 
	   for(i=0;i<MAX_MULTI_PLAYER;i++)
	   {
		   SHORT pnumber=MyNet.PlayersdpId.Player[i].PlayerNumber;
		   GD.Player[pnumber].PI=MyNet.PlayersdpId.Player[i];
	   }
	   
	   
	   // EnemyBit를 설정한다. 
	   for(i=0;i<MAX_MULTI_PLAYER;i++)
	   {
		   GD.Player[i].PI.MyBit=(1<<GD.Player[i].PI.PlayerNumber);
		   
		   for(j=0;j<MAX_MULTI_PLAYER;j++)
		   {
			   if(i==j)continue;
			   
			   if(GD.IsFriendTo(i, j) == FALSE)
			   {
				   GD.Player[i].PI.EnemyBit|=(1<<j);
			   }
		   }
	   }

	   InitGameSetCharOnline(userpointindex);


	 // 중립국이면, 
	 if(GD.Player[UserPlayerNumber].PI.Country==COUNTRY_NEUTRAL)
	 {
		 // 화면을 시작위치로 옮긴다. 
		 Map.VirtualMapX=0;
		 Map.VirtualMapY=0;
		 
		 GD.HomeX=Map.VirtualMapX;
		 GD.HomeY=Map.VirtualMapY;
	 }
	 else
	 {
		 
		 // 화면을 시작위치로 옮긴다. 
		 if(userpointindex >=0 )
		 {
			 Map.VirtualMapX=Map.clMapPlayerInfo[userpointindex].StartPoint.x;
			 Map.VirtualMapY=Map.clMapPlayerInfo[userpointindex].StartPoint.y;
		 }
		 else
		 {
			 Map.VirtualMapX = 0;
			 Map.VirtualMapY = 0;
		 }

		 
		 GD.HomeX=Map.VirtualMapX;
		 GD.HomeY=Map.VirtualMapY;
	 }

	 // 팀정보를 불러서 업데이트 한다. 
	 pGame->Config.GetTeamInfo();

#ifdef _OGG_PLAY
	 GetBgmMgr()->Play(OGGBGM_FILENAME_WARTITLE, TRUE);
#else
	 // 전투용 음악을 연주한다.
	 if(g_pSoundManager)
	 {
		 g_pSoundManager->StopAllSound();
		 
		 BattleMusic	=	g_pSoundManager->AddPlayID( "music\\Music3.YAV", FALSE, TRUE, FALSE );
		 g_pSoundManager->PlaySound( BattleMusic );	
	 }
#endif
	 
 	// 상단 메시지를 처리할 수 있는 클래스의 포이터를 얻어온다. 
	pOnlineNoticeList = pGame->GetOnlineNoticeListPointer();

	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->SetPosition(233, 582, 335,4);
	}

	// 일찍 퇴각하면 경험치를 잃는다는 경고를 알려준다.
	{
		char buffer[128];

		if(pGame->pOnlineWorld)
		{
			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
				wsprintf(buffer, Text.RetreatWarningInBattleArea.Get(), RETREAT_PERMIT_CLOCK/1000);
			else
				wsprintf(buffer, Text.RetreatWarningInPeaceArea.Get());
			
			SetMessage(buffer);
		}
	}

	Chat.Initialize();

	// ORDER_QUIT 명령은 한번만 실행되어야 하기 때문에 게임초기에 다음의 스위치를 초기화한다. 
	GameQuitSwitch = FALSE;

	// 아이템 획득 정보창을 없앤다. 
	pGame->pItemInfoWindow->ClearAllItemInfoWindow();



	return TRUE;
}

void _clGame::ChatInit()
{
	Chat.Init();
}




BOOL GameDraw1()
{

	//  이동마크의행동 
	GD.MoveMark.Action();

	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

       // 지도를 옮긴다. 
	   clGrp.PutImage32(0, 0, CLIENT_SCREEN_XSIZE, CLIENT_SCREEN_YSIZE, ScreenMapBuffer);
	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	return TRUE;

}


BOOL GameDraw2()
{
	HDC		hdc;
	
	SI32 i = 0;
	
	
	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{
		
		// 캐릭터를 포함한 객체들을 그린다. 
		Map.DrawObj();
		
		
		// 지도 편집 상태이면 플레이어 시작 위치도 그려준다. 
		if(MapEditMode==TRUE)
		{
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				// 유효한 플레이어 위치라면, 
				if(Map.clMapPlayerInfo[i].StartPoint.x>=0 && Map.clMapPlayerInfo[i].StartPoint.y>=0)
					Map.DrawPlayerStartPoint((SHORT)Map.clMapPlayerInfo[i].StartPoint.x, (SHORT)Map.clMapPlayerInfo[i].StartPoint.y, i);
			}
		}
		clGrp.UnlockSurface(lpScreenBuffer);
	}

	if(lpScreenBuffer->GetDC(&hdc) == DD_OK)
	{
		DrawGetResource(hdc);
		
		lpScreenBuffer->ReleaseDC(hdc);
	}
	

	
	return TRUE;
}

BOOL GameDraw3()
{
	SI32 i=0, j=0;
	HRESULT hResult = 0;
	SI32	siFile = 0;
	HDC hdc;


	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{
	   
	   // 지도 편집 모드이면, 
	   if(MapEditMode==TRUE)
	   {
		   if(MapEdit.TerrainMode==MAPEDIT_MODE_UNIT && Map.IsPtInMap(GameMouseX, GameMouseY)==TRUE)
			   Map.DrawBuilding(MapEdit.SelectedKind, MapX, MapY, TRUE, 0);
		   
		   SHORT centerx, centery;
		   if(Map.GetRefXY(MapX, MapY, centerx, centery)==TRUE)
		   {
			   Map.DrawMousePos(centerx, centery, MapX, MapY, 0, FALSE);
		   }

	   }

	   // 여여문의 지뢰 매설에는 마우스 위치를 알려주는 것이 같이 다닌다.
	   if(GD.SelectedOrder==ORDER_MINE)
	   {
		   SHORT centerx, centery;
		   
		   // 지도 편집 모드에서 마우스 위치를 표시한다. 
		   //if(Map.GetCenterXYByMapCenterXY(MapX, MapY, centerx, centery)==TRUE)
		   if(Map.GetRefXY(MapX, MapY, centerx, centery)==TRUE)
		   {
	          Map.DrawMousePos(centerx, centery, MapX, MapY, 0, TRUE);
		   }

	   }

	   // 비와 불을 그린다.
	   GD.Rain.Draw();


	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	

    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

		if(MapDragSwitch==TRUE)
		{
		   Map.DrawDragBox(DragStartX, DragStartY, DragEndX, DragEndY);
		}

	   // 이동 마크 찍기 
       GD.MoveMark.DrawMoveMark();

 
	   // 로고 그리기 
	   Draw_PutSpriteLightT(clGrp.GetScreenXsize()-GetFileXsize(FV.OnlineLogoFile) - 10, 
		   ScreenRect.bottom-GetFileYsize(FV.OnlineLogoFile)-70, 
		   FV.OnlineLogoFile,0);	// hojae_append
	   
	   clGrp.UnlockSurface(lpScreenBuffer);
	}



	   // 아이템 획득 창을 보여준다. 
	 pGame->pItemInfoWindow->Render(lpScreenBuffer);



	// 판넬에 작은 지도를 그린다. 
	DrawSmallMapImjin();
	
	
if( MapEditMode == FALSE)
{
	//채팅창 그리기.
	pGame->m_ChatList.Process(lpScreenBuffer, 1);
}
	// 판넬을  그리기 
	DrawPannel();

	// 입력중인 문장을 표시해준다. 
	Chat.DrawInputText();


	// 작은지도에서 화면영역을 나타내는 작은 사각형을 그린다. 
	if(IsSmallMapValid())
	{
	    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
	       SMap.DrawBox(Map.VirtualMapX, Map.VirtualMapY);
	       clGrp.UnlockSurface(lpScreenBuffer);
		}
	}

	// 메뉴를 그려준다. 
	DrawMenu(lpScreenBuffer);

	// 메시지가 있으면 표시한다. 
	ShowMessage(10, 70, lpScreenBuffer);

	// 남은 시간을 표시한다. 
	if(GetPlayTime() == TRUE)
	{
		if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
			siFile = FILE_ON_BASE_TIMER;
			Draw_PutSpriteLightT(12,8,siFile,0);
			clGrp.UnlockSurface(lpScreenBuffer);
		}
	}
	ShowPlayTime(38, 38, lpScreenBuffer);

	// 상단 메시지 처리. 
	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->Poll(lpScreenBuffer);
	}
	// ===============================================================================
//	if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
	// ===============================================================================
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	// ===============================================================================
	{
		// 상단 메시지를 출력한다. 
		if(lpScreenBuffer->GetDC(&hdc)==DD_OK)
		{
			
			if(pOnlineNoticeList)
			{
				SelectObject(hdc, pGame->pBattle->HelpFont);
				SetBkMode(hdc, TRANSPARENT);
				
				pOnlineNoticeList->Draw(hdc);
			}
			
			lpScreenBuffer->ReleaseDC(hdc);
		}
	}
	
	// 마우스 밑의 곡물 정보를 표시한다. 
	ShowCropInfo(lpScreenBuffer);

	// 지도 편집 모드에서는 정보를 표시한다. 
	if(MapEditMode==TRUE)
	{
 		MapEdit.MapEdit_DrawInfo();
    }

	return 0;
}


BOOL GameDraw4()
{
	static RGBQUAD prgb[256];
	HRESULT hResult =0;

	// 도움말 정보를 보여준다. 
	Help.Draw();



    //////// lpDDSBack를 lpDDSPrimary로 복사한다.
	DrawMouse(lpScreenBuffer);

	
	GD.EarthQuake.Action();

	RECT rect;
	rect.left   = 0;
	rect.right  = clGrp.GetScreenXsize();
	rect.top    = GD.EarthQuake.IndexY;
	rect.bottom = clGrp.GetScreenYsize();

	if (WindowModeSwitch)
	{

		GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
		ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );
			
		for(int i=0; i<256; i++ )
		{
			prgb[i].rgbRed		= clGrp.PalEntries[i].peRed;//Palette[i*3] << 2;
			prgb[i].rgbGreen	= clGrp.PalEntries[i].peGreen;//Palette[i*3+1] << 2;
			prgb[i].rgbBlue		= clGrp.PalEntries[i].peBlue;//Palette[i*3+2] << 2;
			prgb[i].rgbReserved	= 0;
		}

		SetDIBColorTable(g_hBattleScreenDC,0,255,(RGBQUAD *)prgb);		

		if( pGame->pOnlineWorld )
			pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);
		else
		{
			//여기는 맵 에디터 모드일때 들어온다
			GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
			ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
			ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );

			HDC destHdc;
			HDC hdc;
			
			if (lpScreenBuffer->GetDC(&hdc) == DD_OK) {
				clGrp.lpDDSConvertSurface->GetDC(&destHdc);
				BitBlt(destHdc, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize(), hdc, 0, 0, SRCCOPY);
				//GdiFlush();
				lpScreenBuffer->ReleaseDC(hdc);
				clGrp.lpDDSConvertSurface->ReleaseDC(destHdc);
				
				clGrp.m_rcWindow.bottom = clGrp.m_rcWindow.top + 600;
				clGrp.m_rcWindow.right = clGrp.m_rcWindow.left + 800;
				
				clGrp.lpDDSPrimary->Blt( &clGrp.m_rcWindow, clGrp.lpDDSConvertSurface,
					NULL, DDBLT_WAIT , NULL );
			}
		}
		
	}
	else 
	{
		
		if(clGrp.lpClipper)
		{
			hResult = clGrp.lpDDSPrimary->Blt(NULL, lpScreenBuffer, &rect, DDBLT_WAIT, NULL);
		}
		else
		{
			hResult = clGrp.BltFast(SURFACE_PRIMARY, 0, 0, lpScreenBuffer, &rect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
		}
	}


	// 서피스가 로스트된 것이면, 
	if(hResult == DDERR_SURFACELOST)
	{
		clGrp.Restore();
	}


	if (!WindowModeSwitch) {
		if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
			DrawMouseBuffer();
			clGrp.UnlockSurface(lpScreenBuffer);
		}
	}


	return TRUE;
}



BOOL IsSmallMapValid()
{

//	if(Cheat.CheatSeeSwitch==TRUE 
//	|| GD.GetSmallMapValidSwitch() )return TRUE;
	
	return TRUE;
}


void DrawSmallMapImjin()
{
	// 봉화대가 있으면 작은 지도를 그려주고, 
	if(IsSmallMapValid())
	{
		SHORT drawswitch=FALSE;
		
		// 작은 지도가 표시안되는 상태에서 보이면,
		if(GD.SmallMapDrawSwitch==FALSE)
		{
			// 효과음을 낸다. 
			if(GD.SmallMapDrawSwitch==FALSE)
			   PushEffect(EFFECT_MAP1, 0, 0);

			drawswitch=TRUE;
		}
		// 보이는 상태에서는 30프레임마다..
		else
		{
			if(abs(GD.Frame - GD.DrawSmallMapFrame) >= 30)
			{
				GD.DrawSmallMapFrame = GD.Frame;
				drawswitch=TRUE;
			}
		}

		GD.SmallMapDrawSwitch=TRUE;

		// 작은지도의 모두가 변경되었다면, 
		if(GD.SmapModeChangedSwitch==TRUE)
		{
			GD.SmapModeChangedSwitch=FALSE;

			drawswitch=TRUE;
		}

		if(drawswitch==TRUE)
		{
		    if(clGrp.LockSurface(lpPannelSurface)==TRUE)
			{
				// 지형까지 보이는 모드이면, 지형도 그려준다. 
	            SMap.Draw(GD.SmapModeFullSwitch);

 	            SMap.DrawChar();

	            clGrp.UnlockSurface(lpPannelSurface);
			}
		}
	}

	// 봉화대가 없으면 작은 지도를 없앤다.
	if(IsSmallMapValid() == FALSE && GD.SmallMapDrawSwitch==TRUE)
	{
		// 효과음을 낸다. 
		if(GD.SmallMapDrawSwitch==TRUE)
		   PushEffect(EFFECT_MAP2, 0, 0);

		GD.SmallMapDrawSwitch=FALSE;

		// 판넬을 surface로 옮긴다.
	    if(clGrp.LockSurface(lpPannelSurface)==TRUE)
		{
			
	         clGrp.PutImageT(0, 0, 
		          PannelSpr.Header.Xsize, 
			      PannelSpr.Header.Ysize,
		          PannelSpr.Image);

	        clGrp.UnlockSurface(lpPannelSurface);
		}
	}

}

BOOL DrawPannel()
{
	HRESULT hResult;

	RECT _rect;
	

	if(WindowModeSwitch)
	{
		_rect.left = 0;
		_rect.right = PannelSpr.Header.Xsize;
		_rect.top = 0;
		_rect.bottom = PannelSpr.Header.Ysize;
	

		lpScreenBuffer->BltFast(0, clGrp.GetScreenYsize()-PannelSpr.Header.Ysize, lpPannelSurface, &_rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
//		if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
//		{
//			clGrp.PutSprite8T(0, clGrp.GetScreenYsize()-PannelSpr.Header.Ysize, 
//				PannelSpr.Header.Xsize, 
//				PannelSpr.Header.Ysize,
//				PannelSpr.Image);
//			
//			clGrp.UnlockSurface(lpScreenBuffer);
//		}
	}
	else
	{

		_rect.left = 0;
		_rect.right = clGrp.GetScreenXsize();
		_rect.top = 0;
		_rect.bottom = PannelSpr.Header.Ysize;

		hResult = lpScreenBuffer->BltFast(0, clGrp.GetScreenYsize()-PannelSpr.Header.Ysize, lpPannelSurface, &_rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);

		// 서피스가 로스트된 것이면, 
		if(hResult == DDERR_SURFACELOST)
		{
			clGrp.Restore();
		}

	}


	return TRUE;
	
}


void SmallMapToggleOperation()
{
	if(GD.SmallMapDrawSwitch)
	{
		GD.SmapModeChangedSwitch=TRUE;
		GD.SmapModeFullSwitch=(GD.SmapModeFullSwitch ? FALSE:TRUE);
	}
}


// 실제 게임의 루틴이 들어있는 함수 
// 리턴값: 0 -> normal
SHORT GameProcess()
{
	// 게임이 살아 있음을 일정시간마다 게임 서버에 보내준다. (일정시간등의 체크는 함수내에서 처리한다.)
	pGame->OnlineSendHeartBeat();

	// 게임중 채팅 입력을 관장한다.
	InputOperateInGame();
	
	
	if(GD.Frame==0)StartClock = CurrentClock;
	

	// 게임 메뉴를 보여준다. 
	if(GameMenuShowSwitch==TRUE)
	{
		// 게임에서 게임메뉴를 부를 때에만,
		if(GameStatus==GS_GAME)
		{
			// 게임 메뉴에서는 마우스를 기본 마우스로 사용한다.
			SetMouse(MOUSE_NORMAL);
			
			// 팔레트를 초기화해준다. 
			SetGamePalette(clGrp.Palette);
			
			GameStatus=GS_INITGAMEMENU;

			// 현재 프라이머리 화면을 lpscreenbuffer로 옮긴다. 
			if(lpScreenBuffer)
			{
				if (WindowModeSwitch)
				{
					if( pGame->pOnlineWorld )
						pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);				
					else
					{
						//여기는 맵 에디터 모드일때 들어온다
						GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
						ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
						ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );
						
						HDC destHdc;
						HDC hdc;
						
						if (lpScreenBuffer->GetDC(&hdc) == DD_OK) {
							clGrp.lpDDSConvertSurface->GetDC(&destHdc);
							BitBlt(destHdc, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize(), hdc, 0, 0, SRCCOPY);
							//GdiFlush();
							lpScreenBuffer->ReleaseDC(hdc);
							clGrp.lpDDSConvertSurface->ReleaseDC(destHdc);
							
							clGrp.m_rcWindow.bottom = clGrp.m_rcWindow.top + 600;
							clGrp.m_rcWindow.right = clGrp.m_rcWindow.left + 800;
							
							clGrp.lpDDSPrimary->Blt( &clGrp.m_rcWindow, clGrp.lpDDSConvertSurface,
								NULL, DDBLT_WAIT , NULL );
						}

					}
				}
				else
				{
					lpScreenBuffer->BltFast(0, 0, clGrp.lpDDSPrimary, NULL, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);

				}

				if (!WindowModeSwitch) {
					// 마우스를 지운다. 
					if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
					{
						DrawMouseBuffer();
						clGrp.UnlockSurface(lpScreenBuffer);
					}
				}

			}

			return 0;
		}
	}
	
	if(GameOverHead()==TRUE)return 0;


	if(GamePauseSwitch==FALSE)
	{
		// 화면이동관련 
		if(ScrollProcSwitch == FALSE)
		{
			ScrollOperation();
			SetScrollProcSwitch(TRUE);
		}
		
		// 치트키로 된 명령을 부여한다. 
//		Cheat.CheatOrder();
	}
	// 지도 타일을 그린다. 
	Map.DrawMap();

	if(GamePauseSwitch==FALSE)
	{
		// 저장된 명령을 처리한다. 
		PopOrder();
	}

	// 게임을 지연시킨다. 
	if(GameDelay() == FALSE)return 0;



	// 초기메뉴로 돌아가는 카운트 다운. 
	if(GD.InitInitMenuCount)
	{
		GD.InitInitMenuCount--;

		if(GD.InitInitMenuCount == 0)
		{
			OperateGameMenuInitInitMenu();
			return 0;
		}

	}

	// 게임을 종료하는 카운트 다운. 
	if(GD.InitQuitCount)
	{
		GD.InitQuitCount--;

		if(GD.InitQuitCount == 0)
		{
			OperateGameMenuInitQuit();
			return 0;
		}
	}


	GD.Frame++;

	// 매 1분당 전투 서버로 하트비트 메시지를 보낸다. 
	if(GD.Frame && (GD.Frame % (60000/DEFAULT_MULTI_PLAY_DELAY_TIME) == 0))
	{
		OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(MyNet.PlayersdpId.dpId[UserPlayerNumber ]);
	}

	if(GD.Frame >= 20)
	{
		if(GD.Frame == 20)SetStartFrameClock(CurrentClock);
		else
		{
			// 경과된 시간을 기록한다. 
			PushFrameClock(GD.Frame, CurrentClock);
		}
	}

	GameAction();


	// 윈도우가 활성화 되어 있는 경우에만 마우스를 그린다. 
	if(bActive == TRUE)
	{
		POINT point;
		GetCursorPos(&point);
		if (WindowModeSwitch)
		{
			ScreenToClient(pGame->Hwnd,&point);
//			BOOL bChange = TRUE;
//			if (point.x > 800)
//			{
//				bChange = TRUE;
//				point.x = 800;
//			}
//			if (point.x < 0)
//			{
//				bChange = TRUE;
//				point.x = 0;
//			}
//			if (point.y < 0)
//			{
//				bChange = TRUE;
//				point.y = 0;
//			}
//			if (point.y > 600)
//			{
//				bChange = TRUE;
//				point.y = 600;
//			}
//
//			POINT temppt = point;
//
//		    ClientToScreen(pGame->Hwnd,&temppt);

		//	if (bChange) SetCursorPos(temppt.x,temppt.y);


		}
		
		IpD.Mouse_X=(SHORT)point.x;
		IpD.Mouse_Y=(SHORT)point.y;
		
		
		DrawMousePrimary();
	}

    GD.GameFrame++;
	
	// 플레이 시간을 측정한다. 
	if(abs(GD.TempAccumulatedPlayClock - CurrentClock) >= 1000)
	{
		GD.TempAccumulatedPlayClock = CurrentClock;
		GD.AccumulatedPlaySecond ++;
	}

	
	return 0;
}


// 게임의 속도를 일정하게 만든다. 
BOOL GameDelay()
{

	DWORD basicdelay;
	
	// 설정된 값에 따라 기본적인 속도를 얻어온다.
	basicdelay = pGame->Config.GetRealGameSpeedClock();

	FrameDelay = basicdelay + FrameClockDelay;
	FrameClockDelay = 0;


	clock_t tempclock = CurrentClock;

	// 빠르게 도착했다면, 
	if( abs(CurrentClock - PrevFrameClock) < FrameDelay)
	{
		// 지연시켜야 할 시간 ,
		clock_t delayclock = min( max( FrameDelay - (CurrentClock - PrevFrameClock), 0), FrameDelay);
		
		int _clock = CurrentClock;
		int index=0;
		while((_clock - CurrentClock) < delayclock)
		{
			return FALSE;
		}

		tempclock = _clock;
	}
	else
	{
		clock_t overclock = min( ((CurrentClock - PrevFrameClock) - FrameDelay) , FrameDelay);
		
		// 원래 설정되었어야 할 시간 (지연이 없었을 경우의 시각)
		tempclock = (CurrentClock - overclock);

	}


	PrevFrameClock = tempclock;

	return TRUE;
}


BOOL GameWinCondition()
{

	// 프레임이 달라져야만  체크한다. 
	if(GD.Frame == PrevGameWinConditionFrame)return FALSE;
	PrevGameWinConditionFrame = GD.Frame;

	SHORT status = WinCondition();


	//이기거나 지면 게임을 빠져 나가야 한다. 
	if((GD.VictoryDelay && (abs(CurrentClock - GD.VictoryDelay) > VICTORY_HALFDELAY))
	|| (GD.DefeatDelay  && (abs(CurrentClock - GD.DefeatDelay)  > DEFEAT_HALFDELAY)))
	{
		if(GameQuitSwitch == FALSE)
		{
			GameQuitSwitch = TRUE;
			MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);
		}
	}

	// 승리했다. 
	if(status == 1)
	{
		// 승리한것임을 기억한다. 
		GD.WinSwitch = TRUE;

		GameStatus   = GS_INITWIN;

    	EndGame();

//		pGame->pOnlineWorld->pOnlineClient->SendReturnOnline(TRUE);
		return TRUE;
	}
	// 패배 했다. 
	else if(status == -1)
	{
		GD.WinSwitch = FALSE;
		GameStatus = GS_INITLOSE;

		EndGame();
//		pGame->pOnlineWorld->pOnlineClient->SendReturnOnline(FALSE);
		return TRUE;
	}

	return FALSE;
}




//--------------------------------------------------------------------------
// Name: EndGame()
// Desc: 게임을 종료하는데 필요한 설정 
//--------------------------------------------------------------------------
void EndGame()
{
	if (WindowModeSwitch)
	{
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.bottom = 600;
		rect.right = 800;
		HRESULT hr =	clGrp.lpDDSBack->BltFast(0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
		
	}

#ifdef _OGG_PLAY
#else
	if(g_pSoundManager)
	{
		g_pSoundManager->StopAllSound();

		if(BattleMusic)
		{
			g_pSoundManager->DelPlayID( BattleMusic );
			BattleMusic = 0;
		}
	}
#endif

	// 사용자 번호를 초기화한다. 
	UserPlayerNumber=0;

	Map.FreeMap();
	FreeMapTile();

	// 마우스 커서를 기본형으로 교체한다.
	SetMouse( MOUSE_NORMAL );


	// 팔레트를 게임 팔레트로 초기화한다. 
	SetGamePalette(clGrp.Palette);

	// 캐릭터를 모두 없앤다. 
	DeleteChar();

	// 총알을 모두 없앤다. 
	DeleteBullet();
	// 연기를  모두 없앤다. 
	DeleteSmoke();
	
	// 마법등을 모두 없앤다. 
	DeleteMagic();

	// 게임 중에 로딩된 이미지 파일의 메모리를 해제한다.
	FreeImgFile();



	if(pGame)
	{
		if(pGame->pBattle)
			pGame->pBattle->InitGameButton();
	}
	
	// 전체 화면을 클리핑 영역으로 설정한다.
	clGrp.SetClipFullScreen();
}

SHORT GameAction()
{
	SHORT i;
	GD.OldRand=GD.Rand;
    GD.Rand = GD.Random();
	SHORT id = 0;


	// 게임 데이터의 해킹 가능성을 확인한다. 
	if(GD.Frame % 10 == 0)
	{
		// kindinfo등의 해킹 여부를 검사한다. 
		if(CheckKindInfo() == FALSE)
		{
			// 서버에 해킹 우려가 있는 사용자라는 사실을 알린다. 
			// 게임을 종료시킨다. 
			GameStatus = GS_INITQUIT;
			
			// 해킹으로 인해 게임이 종료됨을 알린다. 
			//clGrp.Error("Game Quit", "There is possibility of Hacking. Your account will be blocked. Please contact us.");
		}
	}


	if(GamePauseSwitch==FALSE)
	{
	    // 임시로 
	    GD.MoveCharNumber=0;

		// 아이템 획득 창 관련 행동을 한다. 
		pGame->pItemInfoWindow->Prepare();

		// 지도 편집 모드가 아니어야 한다. 
	    if(MapEditMode==FALSE)
		{

			// 시간을 처리한다. 
			GD.Time.PassTime();


			// 비의 행동 
			GD.Rain.Action();

			VolcanoAction();
		}

		// 캐릭터의 행동 
		GD.SyncSignal  = 0;
		int charindex;
	    for(charindex = 0;charindex < GD.CharOrderIndex;charindex++)
		{
			id = GD.CharOrder[charindex] ;
			if(GD.CharInfo[ id ])
			{
				// 동기 신호를 얻어온다. 
				GD.SyncSignal = GD.SyncSignal + id + clifChar.GetX(id) + clifChar.GetY(id) + clifChar.GetCurrentOrder(id) 
					+ clifChar.GetWorkStep(id) + clifChar.GetDX(id) + clifChar.GetDY(id) + clifChar.GetPara1(id) 
					+ clifChar.GetDpower(id) + clifChar.GetLife(id) + clifChar.GetMana(id);


				DWriteStringToFile(" id = %d , life =  %d",id,clifChar.GetLife(id));
				if(clifChar.Action(id)==FALSE)
				{
					DWriteStringToFile("죽었다.id = %d",id);
					DeleteChar(id);
				}
			}
		}

		{

			// 전방 탐색을 실시한다.
			for(charindex = 0;charindex < GD.CharOrderIndex;charindex++)
			{
				id = GD.CharOrder[charindex] ;
				if(GD.CharInfo[ id ])
				{
					if(CR[id].SearchForwardSwitch == TRUE)
					{
						CR[id].SearchForward();
						CR[id].SearchForwardSwitch = FALSE;
					}
				}
			}
		}

		// 마법들의 행동. 
		MagicAction();

		// 총알들의 행동 
	    for(i=0;i<MAX_BULLET_NUMBER;i++)
		{
		   if(GD.BulletInfo[i])
		   {
			   //if(Bullet[i].Action()==FALSE)
			   if(Bullet_Action(i)==FALSE)	// hojae_repair
			   {
			 	  DeleteBullet(i);
			   }
		   }
		}

		// 연기들의 행동  
	    for(i=0;i<MAX_SMOKE_NUMBER;i++)
		{
		    if(GD.SmokeInfo[i])
			{
			    //if(Smoke[i].Action()==FALSE)
				if(Smoke_Action(i)==FALSE)	// hojae_repair
				{
				   DeleteSmoke(i);
				}
			}
		}

		// 게임 시간을 진행한다.
		PassPlayTime();

		// 캐릭터들의  안개 처리 
		// 기존의 밝은 지역을 어둡게 만들고 새로운 지역을 밝게 만든다. 
		// 포그.
		if(pGame->pOnlineWorld)
		{
			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,FALSE);
				// 기존의 밝은 지역은 놔두고 새로운 지역만 밝힌다. 
				else
					GD.BlackCheck(FALSE,FALSE);
			}
			else
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,TRUE);
				// 기존의 밝은 지역은 놔두고 새로운 지역만 밝힌다. 
				else
					GD.BlackCheck(FALSE,TRUE);
				
			}
		}
   }
   
   // 스페이스 바를 눌러 화면 이동 시킨다. 
   if(GD.ScreenMoveSwitch==TRUE)
   {
       Map.VirtualMapX=GD.ScreenMoveX;
       Map.VirtualMapY=GD.ScreenMoveY;
       Map.DrawForceSwitch=TRUE;
	   
	   GD.ScreenMoveSwitch=FALSE;
   }
   
	return 0;
}


// 마우스 버튼에 의한 자동 명령을 처리한다. 
BOOL OperateAutoOrder()
{

	// 현재 자동 명령이 아닌 방식으로 선택된 명령이 있다면 자동 명령은 무시한다. 
	if(GD.SelectedOrder!=0 && GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
	{
	   return FALSE;
	}

	// 메뉴에 수확이 있고 마우스가 곡물 위에 있으면, 
	if(MouseMode&MOUSEMODE_HARVEST)
    {
		GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_HARVEST;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 이동이 있고 마우스가 빈자리에 있으면, 
    else if(MouseMode&MOUSEMODE_MOVE)
    {
 	    GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_MOVE;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 공격이 있고 마우스가 적 위에 있으면, 
    else if(MouseMode&MOUSEMODE_ATTACK)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_ATTACK;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 탑승이 있으면
    else if(MouseMode&MOUSEMODE_RIDE)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_RIDE;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // 메뉴에 치료가 있고 
    else if(MouseMode&MOUSEMODE_HEAL)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_HEAL;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
	else if(MouseMode&MOUSEMODE_MANA)
	{
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_MANA;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
	}
    // 메뉴에 깃발을 본영에 넣는 것이 있고. 
    else if(MouseMode&MOUSEMODE_GOINTOHQ)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_GOINTOHQ;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
	// 메뉴에 소화가 있고 마우스가 불 위에 있으면, 
	else if(MouseMode&MOUSEMODE_EXTINGUISH)
    {
		GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_EXTINGUISH;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
	else
	{
		return FALSE;
	}

	return TRUE;
}


void ScrollOperation()
{
	// 스크롤 정보를 초기화한다. 
	Scroll=0;


	

	//스킵하는 프레임에서만 스크롤을 한다. 
	// 윈도우 모드일때는 범위를 넓게 잡는다.
	if (WindowModeSwitch) {
		if(DragSwitch==FALSE )
		{
			if(abs(GameMouseX) <= SCROLL_RANGE )               Scroll|=WEST;
			if(abs(GameMouseX - (clGrp.GetScreenXsize()-1)) <= SCROLL_RANGE ) Scroll|=EAST;
			if(abs(GameMouseY) <= SCROLL_RANGE)                Scroll|=NORTH;
			if(abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE ) Scroll|=SOUTH;
			
			// 키보드에 의한 스크롤을 정의한다. 
			if(GetAsyncKeyState(VK_UP)   < -1)	   Scroll |= NORTH;
			if(GetAsyncKeyState(VK_DOWN) < -1)	   Scroll |= SOUTH;
			if(GetAsyncKeyState(VK_LEFT) < -1)     Scroll |= WEST;
			if(GetAsyncKeyState(VK_RIGHT)< -1)	   Scroll |= EAST;
		}
	}
	else
	{
		if(DragSwitch==FALSE )
		{
			if(GameMouseX==0)              Scroll|=WEST;
			if(GameMouseX==(clGrp.GetScreenXsize()-1))Scroll|=EAST;
			if(GameMouseY==0)              Scroll|=NORTH;
			if(GameMouseY==(clGrp.GetScreenYsize()-1))Scroll|=SOUTH;
			
			// 키보드에 의한 스크롤을 정의한다. 
			if(GetAsyncKeyState(VK_UP)   < -1)	   Scroll |= NORTH;
			if(GetAsyncKeyState(VK_DOWN) < -1)	   Scroll |= SOUTH;
			if(GetAsyncKeyState(VK_LEFT) < -1)     Scroll |= WEST;
			if(GetAsyncKeyState(VK_RIGHT)< -1)	   Scroll |= EAST;
		}
		
		
	}


	// 스크롤 중이면 지도를 그린다. 
	if(Scroll)
	{
		Map.SetMapDrawSwitch(TRUE);
	}

	// 스크롤 정보를 임시로 저장한다. 
	SHORT tempvirtualmapx=Map.VirtualMapX;
	SHORT tempvirtualmapy=Map.VirtualMapY;

	// 북쪽 스크롤 
	if(Scroll&NORTH)
	{
		if(Map.DecreaseVirtualMapY(1)==TRUE)
		{

	 	   if(Map.DecreaseVirtualMapX(1)==FALSE)
		   {
			   if(Map.DecreaseVirtualMapY(1)==FALSE)
				   Map.IncreaseVirtualMapY(1);
		   }
		}
		else
		{
			Map.DecreaseVirtualMapX(2);
		}
	}
	// 남쪽 스크롤 
	else if(Scroll&SOUTH)
	{
		if(Map.IncreaseVirtualMapY(1)==TRUE)
		{

			if(Map.IncreaseVirtualMapX(1)==FALSE)
			{
				if(Map.IncreaseVirtualMapY(1)==FALSE)
					Map.DecreaseVirtualMapY(1);

			}
		}
		else
		{
		   Map.IncreaseVirtualMapX(2);

		}
	}

	// 서쪽 스크롤 
	if(Scroll&WEST)
	{
	   if(Map.DecreaseVirtualMapX(1)==TRUE)
	   {
		  if(Map.IncreaseVirtualMapY(1)==FALSE)
		  {
			if(Map.DecreaseVirtualMapX(1)==FALSE)
				Map.IncreaseVirtualMapX(1);
		  }
	   }
	   else
	   {
	      Map.IncreaseVirtualMapY(2);
	   }
	}
	// 동쪽 스크롤 
    else if(Scroll&EAST)
	{
	    if(Map.IncreaseVirtualMapX(1)==TRUE)
		{
		   if(Map.DecreaseVirtualMapY(1)==FALSE)
		   {
			  if(Map.IncreaseVirtualMapX(1)==FALSE)
			  {
   				Map.DecreaseVirtualMapX(1);
			  }
		   }
		}
	    else
		{
	       Map.DecreaseVirtualMapY(2);
		}
	}


	if(Map.VirtualMapX==tempvirtualmapx+1 && Map.VirtualMapY==tempvirtualmapy-3)
	{
		Map.VirtualMapX=tempvirtualmapx;
		Map.VirtualMapY=tempvirtualmapy-2;
	}
	else if(Map.VirtualMapX==tempvirtualmapx-3 && Map.VirtualMapY==tempvirtualmapy+1)
	{
	   Map.VirtualMapX=tempvirtualmapx-2;
	   Map.VirtualMapY=tempvirtualmapy;
	}
	else if(Map.VirtualMapX==tempvirtualmapx+3 && Map.VirtualMapY==tempvirtualmapy-1)
	{
	   Map.VirtualMapX=tempvirtualmapx+2;
	   Map.VirtualMapY=tempvirtualmapy;
	}
	else if(Map.VirtualMapX==tempvirtualmapx-1 && Map.VirtualMapY==tempvirtualmapy+3)
	{
	   Map.VirtualMapX=tempvirtualmapx;
	   Map.VirtualMapY=tempvirtualmapy+2;
	}


	// 실제로 스크롤이 이루어 졌는지 안이루어 졌는지 확인한다. 
	if(tempvirtualmapx!=Map.VirtualMapX || tempvirtualmapy!=Map.VirtualMapY)
	{
		   ScrollDoneSwitch=TRUE;
	}
	else
	{
		ScrollDoneSwitch=FALSE;
	}

}


void SystemKeyCheck()
{
	if(GetAsyncKeyState(VK_CONTROL) < -1)
	{
		ControlKeyPressSwitch = TRUE;
	}
    else ControlKeyPressSwitch = FALSE;

	if(GetAsyncKeyState(VK_SHIFT) < -1)
	{
		ShiftKeyPressSwitch = TRUE;
	}
    else ShiftKeyPressSwitch = FALSE;

	//alt키 눌림 여부 확인        
	if(GetAsyncKeyState(VK_MENU) < -1)
	{
		 IpD.AltKeyPressSwitch=TRUE;
	}
	else
	{
	     IpD.AltKeyPressSwitch=FALSE;
	}
}


void GroupInterface()
{
	
    if(GD.SelectedNumber>=1)
	{
		if(ControlKeyPressSwitch==TRUE)
		{
			// 1
			if(GD.KB.GetAKey('1'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(1);
				//GD.Team.AddTeam(1,UserPlayerNumber);
				pGame->Config.SetTeamInfo(1);
			}
			// 2
			else if(GD.KB.GetAKey('2'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(2);
				//GD.Team.AddTeam(2,UserPlayerNumber);
				pGame->Config.SetTeamInfo(2);
			}
			// 3
			else if(GD.KB.GetAKey('3'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(3);
				//GD.Team.AddTeam(3,UserPlayerNumber);
				pGame->Config.SetTeamInfo(3);
			}
			// 4
			else if(GD.KB.GetAKey('4'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(4);
				//GD.Team.AddTeam(4,UserPlayerNumber);
				pGame->Config.SetTeamInfo(4);
			}
			// 5
			else if(GD.KB.GetAKey('5'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(5);
				//GD.Team.AddTeam(5,UserPlayerNumber);
				pGame->Config.SetTeamInfo(5);
			}
			// 6
			else if(GD.KB.GetAKey('6'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(6);
				//GD.Team.AddTeam(6,UserPlayerNumber);
				pGame->Config.SetTeamInfo(6);
			}
			// 7
			else if(GD.KB.GetAKey('7'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(7);
				//GD.Team.AddTeam(7,UserPlayerNumber);
				pGame->Config.SetTeamInfo(7);
			}
			// 8
			else if(GD.KB.GetAKey('8'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(8);
				//GD.Team.AddTeam(8,UserPlayerNumber);
				pGame->Config.SetTeamInfo(8);
			}
			// 9
			else if(GD.KB.GetAKey('9'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(9);
				//GD.Team.AddTeam(9,UserPlayerNumber);
				pGame->Config.SetTeamInfo(9);
			}
			// 0
			else if(GD.KB.GetAKey('0'))
			{
				GD.Player[UserPlayerNumber].Team.AddTeam(0);
				//GD.Team.AddTeam(10,UserPlayerNumber);
				pGame->Config.SetTeamInfo(0);
			}
		}
	}
	
	// 그룹 선택 
	if(GD.KB.GetAKey('1'))
	{
		if(GD.PressedTeam!=1)
		{
			if(GD.SelectedTeam!=1)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(1);
				//GD.Team.SelectTeam(1,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(1);
				//GD.Team.GotoTeam(1,UserPlayerNumber);
		}
		GD.PressedTeam=1;
	}
	else if(GD.KB.GetAKey('2'))
	{
		if(GD.PressedTeam!=2)
		{
			if(GD.SelectedTeam!=2)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(2);
				//GD.Team.SelectTeam(2,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(2);
				//GD.Team.GotoTeam(2,UserPlayerNumber);
		}
		GD.PressedTeam=2;
	}
	else if(GD.KB.GetAKey('3'))
	{
		if(GD.PressedTeam!=3)
		{
			if(GD.SelectedTeam!=3)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(3);
				//GD.Team.SelectTeam(3,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(3);
				//GD.Team.GotoTeam(3,UserPlayerNumber);
		}
		GD.PressedTeam=3;
	}
	else if(GD.KB.GetAKey('4')) 
	{
		if(GD.PressedTeam!=4)
		{
			if(GD.SelectedTeam!=4)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(4);
				//GD.Team.SelectTeam(4,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(4);
				//GD.Team.GotoTeam(4,UserPlayerNumber);
		}
		GD.PressedTeam=4;
	}
	else if(GD.KB.GetAKey('5'))
	{
		if(GD.PressedTeam!=5)
		{
			if(GD.SelectedTeam!=5)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(5);
				//GD.Team.SelectTeam(5,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(5);
				//GD.Team.GotoTeam(5,UserPlayerNumber);
		}
		GD.PressedTeam=5;
	}
	else if(GD.KB.GetAKey('6'))
	{
		if(GD.PressedTeam!=6)
		{
			if(GD.SelectedTeam!=6)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(6);
				//GD.Team.SelectTeam(6,UserPlayerNumber);
				
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(6);
				//GD.Team.GotoTeam(6,UserPlayerNumber);
		}
		GD.PressedTeam=6;
	}
	else if(GD.KB.GetAKey('7'))
	{
		if(GD.PressedTeam!=7)
		{
			if(GD.SelectedTeam!=7)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(7);
				//GD.Team.SelectTeam(7,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(7);
				//GD.Team.GotoTeam(7,UserPlayerNumber);
		}
		GD.PressedTeam=7;
	}
	else if(GD.KB.GetAKey('8'))
	{
		if(GD.PressedTeam!=8)
		{
			if(GD.SelectedTeam!=8)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(8);
				//GD.Team.SelectTeam(8,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(8);
				//GD.Team.GotoTeam(8,UserPlayerNumber);
		}
		GD.PressedTeam=8;
	}
	else if(GD.KB.GetAKey('9'))
	{
		if(GD.PressedTeam!=9)
		{
			if(GD.SelectedTeam!=9)
			{
				GD.Player[UserPlayerNumber].Team.SelectTeam(9);
				//GD.Team.SelectTeam(9,UserPlayerNumber);
			}
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(9);
				//GD.Team.GotoTeam(9,UserPlayerNumber);
		}
		GD.PressedTeam=9;
	}
	else if(GD.KB.GetAKey('0'))
	{
		if(GD.PressedTeam!=0)
		{
			if(GD.SelectedTeam!=0)
				GD.Player[UserPlayerNumber].Team.SelectTeam(0);
				//GD.Team.SelectTeam(0,UserPlayerNumber);
			else 
				GD.Player[UserPlayerNumber].Team.GotoTeam(0);
				//GD.Team.GotoTeam(0,UserPlayerNumber);
		}
		GD.PressedTeam=0;
	}
	else
	{
		GD.PressedTeam=-1;
	}
}


_NewID GetUnderMouseID()
{
	SHORT id;
	SHORT airid;
	SHORT i, j;
	SHORT length=0;
	SHORT min_length=10000;
	SHORT x, y;


	_NewID newid;

	newid.Init();

	// 마우스가 지도위에 있는 것이 아니라면 , 
	if(Map.IsInMapArea(MapX, MapY) == FALSE) return newid;


	// 마우스 밑에 있는 캐릭터를 구한다. 
	if(GD.BlackMap[UserPlayerNumber][MapX][MapY] != FOG2_BLACK_STEP)
	{
			
		// 공중 캐릭터를 먼저 조사한다. 
		id         = 0;
		length     = 0;
		min_length = 10000;

		for(i = 0;i < GD.AirCharNumber;i++)
		{
			airid = GD.AirCharID[i].ID;

			if(IsSee(airid) == TRUE)
			{
				x=clifChar.GetX(airid);
				y=clifChar.GetY(airid);
					
				if(CR[airid].CharDrawInfo.GetDrawSwitch() == TRUE 
					&& clifChar.IsAbsPtInObj(airid, GameRealX, GameRealY, length) == TRUE)
				{
					if(length < min_length)
					{
						min_length = length;
						id         = airid;
						newid      = clifChar.GetID(id);
					}
				}
			}
		}

		// 공중에서 캐릭터를 찾으면 리턴한다. 
		if(newid.IsEmpty() == FALSE) return newid;


		// 지상에서 캐릭터를 찾는다. 
		length     = 0;
		min_length = 10000;
		
		
		for(SHORT range = 0 ; range < 3 ; range++)
		{
			for(i=MapY - range;i <= MapY + range; i++)
				for(j=MapX - range;j <= MapX + range; j++)
				{
					if(i==(MapY-range) || i== (MapY+range) || j==(MapX-range) || j==(MapX+range)){}
					else continue;
					
					// 지도의 범위내에 존재하는지 검사한다. 
					if(Map.IsInMapArea(j, i) == FALSE) continue;
					
					// 완전히 어두운 곳에서만 선택 불가 
					if(GD.BlackMap[UserPlayerNumber][j][i] == FOG2_BLACK_STEP)continue;
					
					id = Map.GetIDMap(j, i);
					
					if(IsSee(id) && CR[id].CharDrawInfo.GetDrawSwitch() == TRUE)
					{ 
						
						if(clifChar.IsAbsPtInObj(id, GameRealX, GameRealY, length)==TRUE)
						{
							if(length < min_length)
							{
								min_length = length;
								newid      = clifChar.GetID(id);
							}
						}
					}
				}

		}
			
		// 최종적으로 마우스 밑에 캐릭터가 없는 상태라면, 
		if(newid.IsEmpty())
		{
			SHORT harvestx, harvesty;
			
			if(MouseModeHarvest(MapX, MapY, GameMouseX, GameMouseY, harvestx, harvesty)==TRUE)
			{
				UnderMouseCropType  = clifMap.GetCropTypeMap(harvestx, harvesty);
				UnderMouseCropMount = Map.GetCropMountMap(harvestx, harvesty);
			}
			
		}
	}

	return newid;
}


// 드래그가 종료되었을때의 처리이다.
void DragOperation()
{
	SI32 i;
	SHORT id;
    SHORT realx1, realy1, realx2, realy2;

	// 선택된 유닛을 저장할 임시 버퍼를 만들고 초기화한다.
	_NewID temp_id[MAX_SELECTED_CHAR_NUMBER];
	SHORT index=0;
	SHORT newselectedid[MAX_SELECTED_CHAR_NUMBER];
	SHORT newselectednumber = 0;
	
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		temp_id[i].Init();
		newselectedid[i] = 0;
	}
	
	// 드래깅을 초기화한다. 
    MapDragSwitch = FALSE;

	
	// 선택 영역의 시작과 끝을 정렬한다. 
	int min_x = min(MapDragStartX, MapX);
	int max_x = max(MapDragStartX, MapX);
	int min_y = min(MapDragStartY, MapY);
	int max_y = max(MapDragStartY, MapY);
	
	MapDragStartX = min_x;
	MapDragStartY = min_y;
	MapDragEndX   = max_x;
	MapDragEndY   = max_y;

	
	// 마우스를 한곳에서 클릭했을때 
	if(abs(MapDragStartX-MapDragEndX)<=1 &&  abs(MapDragStartY-MapDragEndY)<=1)
	{
		
		// 1버튼 인터페이스이면 자동 명령 루틴을 수행하고 FALSE를 리턴하면 캐릭터 선택으로 설정된다. 
		// 2버튼 인터페이스이면 캐릭터 선택으로 설정된다. 
		if(pGame->Config.InterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
		{
		}
		else
		{
			// 마우스 밑에 캐릭터가 있다면, 
			if(UnderMouseID.IsEmpty() == FALSE)
			{
				// shift키가 눌린 상태이고 
				// 같은 편을 선택한 것이면, 
				if(ShiftKeyPressSwitch  == TRUE 
				&& clifChar.GetPlayerNumber(UnderMouseID.ID) == UserPlayerNumber)
				{
					// 기존에 선택되었던 캐릭터를 버퍼에 넣는다. 
					
							index = 0;

					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// 선택된 캐릭터를 넣는다. 
						if(GD.SelectedID[i].IsEmpty() == FALSE)
						{
							temp_id[index] = GD.SelectedID[i];
							index++;
						}
					}

					
					// 마우스 아래의 캐릭터가 이미 선택되어 있는 캐릭터가 아닌지 확인한다. 
					SHORT alreadyselectedswitch=FALSE;
					SHORT alreadyselectedindex=0;
					BOOL invalidswitch = FALSE;
					
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// 이미 선택된 유닛이 적군이거나 혼자선택 전용 유닛이면, 
						if(clifChar.GetPlayerNumber(temp_id[i].ID) != UserPlayerNumber)
						{
							invalidswitch = TRUE;
						}

						if(UnderMouseID.IsSame(temp_id[i]) == TRUE)
						{
							alreadyselectedswitch = TRUE;
							alreadyselectedindex  = i;
						}
					}
					
					// 새로이 선택된 캐릭터가 기존에 선택된 캐릭터라면, 
					if(alreadyselectedswitch == TRUE)
					{
						// 선택된 리스트에서 제거한다. 
						clifChar.SetSelect(temp_id[alreadyselectedindex].ID, FALSE, FALSE);
						temp_id[alreadyselectedindex].Init();

					}
					// 새로이 선택된 캐릭터가 선택되어있지 않은 캐릭터라면, 
					else
					{
						// 이미 선택된 캐릭터가 있는 상황에서 건물등과 같이 한개만 선택 가능한 유닛이면, 
						// 신규 선택을 하지 않는다.
						if(invalidswitch == TRUE)
						{
						}
						// 여러개가 선택되어도 무방한 캐릭터인 경우에는 추가한다. 
						else
						{
							// 새로 선택된 캐릭터의 수를 초기화한다. 
							newselectednumber = 0;

							// 빈자리에 새로 추가해 넣는다. 
							for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
							{
								// 빈자리에는 한번만 넣는다.
								if(temp_id[i].IsEmpty() && newselectednumber == 0)
								{
									temp_id[i] = UnderMouseID;
									clifChar.SetSelect(UnderMouseID.ID, TRUE, FALSE);
									
									// 신규로 선택된 유닛을 저장한다.
									newselectedid[index] = UnderMouseID.ID;
									newselectednumber++;
									
									index++;
								}
							}
						}
						
						
					}
					
				}
				// 쉬프트등이 눌리지 않는 그밖의 일반적인 선택의 경우 
				else
				{
					temp_id[0] = UnderMouseID;
					index      = 1;
					
					
					// 신규로 선택된 유닛을 저장한다.
					newselectedid[0] = UnderMouseID.ID;
					newselectednumber++;
					
					
					if(abs(CurrentClock-clifChar.GetSelectedClock(temp_id[0].ID))<DOUBLE_CLICK_DELAY)
					{
						DoubleClickedID = temp_id[0].ID;
					}
					
				}
			}
			
		}
	}
	// 마우스로 2칸 이상의 범위에서 실질적인 드래깅을 했을때 
	else  
	{
		
		Map.TransFromScreenToRealXY(DragStartX, DragStartY, realx1, realy1);
		Map.TransFromScreenToRealXY(DragEndX,   DragEndY,   realx2, realy2);
		
		// shift키가 눌린 상태라면, 
		SHORT newselectstartindex = 0;
		
		if(ShiftKeyPressSwitch  == TRUE)
		{
			
			// 기존에 선택된 캐릭터를 임시 버퍼에 넣는다. 
			index = 0;
			BOOL invalidswitch = FALSE;
			
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				if(GD.SelectedID[i].IsEmpty() == FALSE)
				{
					temp_id[index] = GD.SelectedID[i];
					index++;
					newselectstartindex = index;

					// 이미 선택된 유닛이 적군이거나 혼자선택 전용 유닛이면, 
					if(clifChar.GetPlayerNumber(GD.SelectedID[i].ID) != UserPlayerNumber)
					{
						invalidswitch = TRUE;
					}

				}
			}
			
			if(invalidswitch == FALSE)
			{
				// 범위 안에 있는 캐릭터를 구해서 임시 버퍼에 추가한다.
				SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
				
				
				// 신규로 선택된 유닛을 저장한다.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
			}
		}
		else
		{
			// 범위 안에 있는 캐릭터를 구해서 임시 버퍼를 채운다.
			SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
			
			// 신규로 선택된 유닛을 저장한다.
			for(i = 0;i < index;i++)
			{
				newselectedid[i] = temp_id[i].ID;
				newselectednumber++;
			}
		}
	}
	
	
	// 선택된 유닛이 있다면, 
	if(index > 0)
	{
		
		// 선택한 캐릭터가 하나이고 
		// 짧은 시간에 연속적으로 선택된 것이라면, 
		// 화면 안에 있는 같은 종류의 캐릭터들을 구한다. 
		if(DoubleClickedID)
		{
			
			// 더블 클릭된 유닛이 아군이면 추가로 선택한다.
			if(clifChar.GetPlayerNumber(DoubleClickedID) == UserPlayerNumber)
			{
				
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, realx1, realy1);
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.right,(SHORT)ScreenRect.bottom,   realx2, realy2);
				
				SHORT newselectstartindex = index;

				//  화면의 범위 안에 있는 캐릭터를 임시버퍼에 넣는다.
				SelectCharInDrag(temp_id, index, realx1, realy1, 
					realx2, realy2, DoubleClickedID);
				
				
				// 신규로 선택된 유닛을 저장한다.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
				
				DoubleClickedID = 0;
				
			}
		}
		
		// 선택된 유닛의 효과음을 낸다. 
		SHORT effectnumber=0;
		for(i=0;i<newselectednumber;i++)
		{
			id=newselectedid[(i+GD.Rand)%newselectednumber];
			// 선택된 캐릭터중 1개까지만 효과음을 낸다. 
			
			if(effectnumber==0 && KI[ clifChar.GetKind(id) ].SelectEffect)
			{
				clifChar.SelectOperation(id);
				effectnumber++;
			}
		}
		
		//기존에 선택된 캐릭터를 취소한다. 
		Unselect();
		ClearMenu();
		
		// 유닛을 선택한다.
		for(i=0;i<index;i++)
		{
			id=temp_id[i].ID;
			clifChar.SetSelect(id, TRUE, FALSE);
		}
		
		
	}
	// 선택된 유닛이 없다면, 
	else
	{
		if(pGame->Config.InterfaceMode == INTERFACEMODE_2BUTTON)
		{
			//기존에 선택된 캐릭터를 취소한다. 
			Unselect();
			ClearMenu();
		}
	}
}



// 키보드 처리를 포함한 루틴 
BOOL GameOverHead()
{
	int i;
	SHORT id;
	DWORD wparam;
	SHORT mapx, mapy;

	SHORT leftpressswitch=FALSE, rightpressswitch=FALSE;
	SHORT mousex, mousey;
	char moveswitch;




	// 임시로 사용한다. 
	if(MapEditMode == TRUE)
	{
		MapEdit.Action();
	}

	
	// 승리 조건을 체크한다. 
	// 승리 했거나 패배 했으면 리턴한다. 
	if(GameWinCondition() == TRUE)
	{
		return TRUE;
	}
	
	// 게임이 중단된 상태면 리턴한다.
	if(GamePauseSwitch==TRUE)return FALSE;


	//////////////////////////////////////////////////////////
	// control, alt 등의 특수키의 상태를 확인한다. 
	//////////////////////////////////////////////////////////
	


    // 키보드 선택을 통한 그룹 설정 및 선택 

	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	{
		SystemKeyCheck();

		GroupInterface();
	}

	// 사용자의 마우스 , 키보드 등의 입력 상태를 확인한다. 
	GameWParam=0;

	if(GD.KB.PopKeyBuffer(leftpressswitch, rightpressswitch, mousex, mousey, wparam, moveswitch) == TRUE)
	{

		GameMouseX = mousex;
		GameMouseY = mousey;
    	GameWParam = wparam;
		
		// 왼쪽 마우스 버튼 눌림 
		if(leftpressswitch >= 0)
		{

			// 왼쪽 버튼이 처음 눌린것이라면, 
	        if(leftpressswitch == TRUE)
			{
		      DragSwitch=TRUE;
	   	      DragStartX=GameMouseX;
		      DragEndX=DragStartX;
		      DragStartY=GameMouseY;
		      DragEndY=DragStartY;
			}
	        // 왼쪽 버튼이 눌린 상태에서 풀린 것이라면, 
	        if(GameLeftPressSwitch==TRUE && leftpressswitch==FALSE)
			{
		        if(DragSwitch==TRUE)
				{
			       DragDoneSwitch=TRUE;
				}
		        DragSwitch=FALSE;
			}

			GameLeftPressSwitch=leftpressswitch;
	        GameRightPressSwitch=rightpressswitch;
	   }
		else
		{
		    // 마우스가 이동되었는가?
			if(moveswitch==TRUE)
			{
		       if(DragSwitch==TRUE)
			   {
			      DragEndX=GameMouseX;
			      DragEndY=GameMouseY;
			   }
			}
		}


	}
	
	

////////////////// 초기화 //////////////////////////

	// 마우스가 위치한 small 좌표를 초기화한다. 
	MapX = -1; 
	MapY = -1;
	
	UnderMouseID.Init();
	UnderMouseCropType  = 0;
	UnderMouseCropMount = 0;
	MouseMode           = 0;
	MenuMode            = 0;
	

	// 윈도우모드일땐 범위를 넓게 잡는다.
	if (WindowModeSwitch)
	{
		// 스크롤용 마우스를 설정한다. 
		if(abs(GameMouseX) <= SCROLL_RANGE)
		{
			if(abs(GameMouseY) <= SCROLL_RANGE)
			{
				if(ScrollDoneSwitch==TRUE)
					SetMouse(MOUSE_SCROLL_NW);
				else
					SetMouse(MOUSE_SCROLL_NW_X);
			}
			else if(GameMouseY==(clGrp.GetScreenYsize()-1))
			{
				SetMouse(MOUSE_SCROLL_SW);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_WEST);
			}
		}
		else if(abs(GameMouseX - (clGrp.GetScreenXsize()-1)) <= SCROLL_RANGE )
		{
			if(abs(GameMouseY) <= SCROLL_RANGE)
			{
				SetMouse(MOUSE_SCROLL_NE);
			}
			else if( abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE )
			{
				SetMouse(MOUSE_SCROLL_SE);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_EAST);
			}
		}
		else if(abs(GameMouseY) <= SCROLL_RANGE)
		{
			SetMouse(MOUSE_SCROLL_NORTH);
		}
		else if(abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE )
		{
			SetMouse(MOUSE_SCROLL_SOUTH);
		}
		// 마우스를 기본형으로 설정한다. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
		
	}
	else {
		// 스크롤용 마우스를 설정한다. 
		if(GameMouseX==0)
		{
			if(GameMouseY==0)
			{
				if(ScrollDoneSwitch==TRUE)
					SetMouse(MOUSE_SCROLL_NW);
				else
					SetMouse(MOUSE_SCROLL_NW_X);
			}
			else if(GameMouseY==(clGrp.GetScreenYsize()-1))
			{
				SetMouse(MOUSE_SCROLL_SW);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_WEST);
			}
		}
		else if(GameMouseX==(clGrp.GetScreenXsize()-1))
		{
			if(GameMouseY==0)
			{
				SetMouse(MOUSE_SCROLL_NE);
			}
			else if(GameMouseY==(clGrp.GetScreenYsize()-1))
			{
				SetMouse(MOUSE_SCROLL_SE);
			}
			else
			{
				SetMouse(MOUSE_SCROLL_EAST);
			}
		}
		else if(GameMouseY==0)
		{
			SetMouse(MOUSE_SCROLL_NORTH);
		}
		else if(GameMouseY==(clGrp.GetScreenYsize()-1))
		{
			SetMouse(MOUSE_SCROLL_SOUTH);
		}
		// 마우스를 기본형으로 설정한다. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
	}



   // 판넬위에 마우스가 없다는 것을 초기화한다. 
   IsMouseInPannelSwitch   = FALSE;
   IsAllyWindowSwitch      = FALSE;
   IsMouseInMapSwitch      = FALSE;
   IsMouseInBigMapSwitch   = FALSE;
   IsMouseInSmallMapSwitch = FALSE;

/////////////////////////////////////////////////////

	// 작은 지도위에 있다면, 
	if(IsSmallMapValid() && SMap.IsPtInMap(GameMouseX, GameMouseY, mapx, mapy) == TRUE)
	{
	   
		// 유효한 지도 영역이어야 한다. 
		if(Map.IsInMapArea(mapx, mapy) == TRUE)
		{
			// 마우스가 지도 상에 있음을 확인한다. 
			IsMouseInMapSwitch       = TRUE;

			// 마우스가 작은 지도위에 있음을 확인한다. 
			IsMouseInSmallMapSwitch = TRUE;
			
			// 마우스가 위치한 지역을 얻는다. (기본좌표)
			MapX = mapx;
			MapY = mapy;
		}

	}
	// 마우스가 판넬위에 있다면, 
	else if(IsPtInPannel(GameMouseX, GameMouseY) == TRUE)
	{
		IsMouseInPannelSwitch = TRUE;
	}
	else if(IsPtInAllyWindow(GameMouseX, GameMouseY) == TRUE)
	{
		IsAllyWindowSwitch    = TRUE;
	}
	// 마우스가 큰 지도 위에 있을때 
	// 그리고 마우스가 다른 콘트롤위에 존재해서는 안된다. 
	else if(Map.IsPtInMap(GameMouseX, GameMouseY) == TRUE
	&& IsMosueInControlSwitch == FALSE)
	{
		// 마우스가 지도 상에 있음을 확인한다. 
		IsMouseInMapSwitch     = TRUE;

		// 마우스가 큰 지도위에 있음을 확인한다. 
		IsMouseInBigMapSwitch = TRUE;


		// 화면 좌표를 절대 좌표로 변환한다.
		Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, GameRealX, GameRealY);


		// 절대좌표를 근거로 해서 기본 좌표를 얻어온다.
		if(MapEditMode==TRUE)
		{
			SHORT small_map_center_x, small_map_center_y;
		    Map.DecideMapXYByXYLow(GameRealX, GameRealY, MapX, MapY, small_map_center_x, small_map_center_y);
		}
		else
		{
			clifMap.GetRealTileByRealDot(GameRealX, GameRealY, &MapX, &MapY);

			// 쿼터뷰 지도라서 무효한 테두리 영역이 선택된것이면 지도 위에 선택되어 있지 않은 것으로 한다. 
			if(MapX==0 && MapY==0)
			{
				// 원점과의 거리가 일정 범위를 벗어나면서 0,0좌표를 갖는다면 무효하다. 
				DWORD length=max(abs(MapCenterX[0][0]-GameRealX), abs(MapCenterY[0][0]-GameRealY));
				if(length>30)
				{
					MapX = -1;
					MapY = -1;

					// 마우스가 지도 상에 있음을 확인한다. 
		            IsMouseInMapSwitch=FALSE;
		            // 마우스가 큰 지도위에 있음을 확인한다. 
		            IsMouseInBigMapSwitch=FALSE;

				}
			}
		}

		// 마우스 아래의 캐릭터를 선택한다. 
		UnderMouseID = GetUnderMouseID();

	}

    OperateMenu();

	// 대상물이 필요하지 않을 때    
	if(GD.ObjectNeedSwitch == FALSE)
	{
		// 마우스의 위치와 지형과의 관계를 바탕으로 마우스가 처한 상황을 얻어온다.
		// 마우스가 지도 위에 있을 때에만 ....
		if(IsMouseInMapSwitch == TRUE)
		   GetMouseMode(MapX, MapY);
	
		// 2버튼 인터페이스 모드가 설정되어 있고 오른쪽 버튼이 눌리면, 자동 명령을 실행한다. 
		if(pGame->Config.InterfaceMode == INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch == TRUE)
		{
           OperateAutoOrder();
		}

		// 드래깅이 완료되면, 
	   if(MapDragSwitch == TRUE && DragDoneSwitch == TRUE)
	   {
		   DragOperation();
	   }

	   if(DragSwitch == TRUE)
	   {
		   if(Map.IsInMapArea(MapX, MapY) == TRUE)
		   {
			   if(MapDragSwitch == FALSE)
			   {
				   if(IsMouseInBigMapSwitch == TRUE)
				   {
					   MapDragSwitch = TRUE;
					   MapDragStartX = MapX;
					   MapDragStartY = MapY;
					   MapDragEndX   = MapDragStartX;
					   MapDragEndY   = MapDragStartY;
				   }
			   }
		   }
	   }


	   if(MapDragSwitch == FALSE)
	   {
		   if(GameLeftPressSwitch == TRUE)
		   {
		      if(IsMouseInSmallMapSwitch == TRUE)
			  {

				  // 1버튼 인터페이스 방식이고 자동명령을 실행한다면, 
				  if(pGame->Config.InterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
				  {
				  }
				  // 화면이동으로 사용된다. 
				  else if(abs(Map.VirtualMapX-MapX)>5 || abs(Map.VirtualMapY-MapY)>5)
				  {
					  if(Map.IsInMapArea(MapX, MapY) == TRUE)
					  {
						  Map.VirtualMapX     = MapX;
						  Map.VirtualMapY     = MapY;
						  Map.DrawForceSwitch = TRUE;
					  }
				  }
			  }
			  
		   }
	   }

	   DragDoneSwitch = FALSE;
	}

	if(GD.ObjectNeedSwitch == TRUE)   // object 가 필요하다. 
	{
		SHORT buildkind = 0;

		// 선택된 명령에 따라 마우스를 설정한다. 
		if(GD.SelectedOrder == ORDER_MOVERIDEOUT
		|| GD.SelectedOrder == ORDER_TRAINSPOT
		|| GD.SelectedOrder == ORDER_CAPTUREBUILD)
		{
			SetMouse(MOUSE_RIDE);
		}
		else if(GD.SelectedOrder==ORDER_ATTACK
		||		GD.SelectedOrder==ORDER_ATTACKGROUND
		||      GD.SelectedOrder==ORDER_SELFEXP)
		{
			SetMouse(MOUSE_ATTACK);
		}
		else if(GD.SelectedOrder==ORDER_HEAL)
		{
			SetMouse(MOUSE_HEAL);
		}
		else if(GD.SelectedOrder == ORDER_MANA)
		{
			//마나
			SetMouse(MOUSE_HEAL);
		}
		else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_MAGIC) == TRUE)
		{
			SetMouse(MOUSE_MAGIC);
		}
			
		// 지뢰 매설이면 
		if(GD.SelectedOrder==ORDER_MINE)
		{
//			if(Map.IsInMapArea(MapX, MapY)==TRUE)
//			{
//				// 빈땅이어야 하고 지뢰가 이미 매설된 곳이면 안된다. 
//			   if(Map.IsEmpty(MapX, MapY)==TRUE && Map.GetMineMap(MapX, MapY) == -1)
//				    GD.CanBuildSwitch=TRUE;
//			   else GD.CanBuildSwitch=FALSE;
//			}
//			else 
//			   GD.CanBuildSwitch=FALSE;
			
			int a = 0;
		}

		// 유효한 지도 영영에서 마우스 왼쪽 버튼이 눌리면 임시 목적지가 선정된다.
		if(MapX!=-1 && MapY!=-1 && (GameLeftPressSwitch==TRUE))
		{
			ObjectTempSelectedSwitch = TRUE;
		}
		// 유효하지 않은 곳에서는 임시 목적지가 선정되지 않는다. 
		if(MapX==-1 && MapY==-1)
			ObjectTempSelectedSwitch=FALSE;

		// 자동 명령에 의해 선택된 목적지가 아니면, 
		if(GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
		{
			// 2버튼 방식이고, 오른쪽 버튼이 눌린 경우 
			if(pGame->Config.InterfaceMode==INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch==TRUE)
			{

				// 목적물을 필요로 하지 않는 상태로 만든다. 
	            GD.ObjectNeedSwitch=FALSE;

				// 선택된 명령을 취소한다. 
	            GD.SelectedOrder=0;
				// 명령 선택 옵션을 초기화한다. 
			    GD.SelectedOrderOption=0;
			}

		}

		// 임시 목적지가 선정되면, 
		if(ObjectTempSelectedSwitch==TRUE)
		{

			// 마우스 왼쪽 버튼이 떨어지면, 
            if(GameLeftPressSwitch==FALSE)
			{

				// 임시 목적지는 취소한다.
			   ObjectTempSelectedSwitch=FALSE;

		       switch(GD.ObjectNeedType)
			   {
			      case OBJECT1:
				  // 지뢰매설이고  지을 수 없으면obj를 받아들이지 않는다.  
					  if(GD.SelectedOrder 
					  && GD.SelectedOrder==ORDER_MINE)
					  {
						  SetMessage2(Text.CannotMakeThereMessage.Get(), UserPlayerNumber, EFFECT_CANNOTMAKETHERE);
					  }
					  // 수확이고 
					  else if(GD.SelectedOrder  && GD.SelectedOrder==ORDER_HARVEST)
					  {
						  if(MouseModeHarvest(MapX, MapY, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE)
						  {
							  // obj를 선택한다.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MouseHarvestX, MouseHarvestY, UnderMouseID);
						  }
						  else
						  {
							  // obj를 선택한다.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MapX, MapY, UnderMouseID);

						  }
					  }
					  else
					  {
					     // obj를 선택한다.  
			             GD.ObjectNeedSwitch=FALSE;
				         GD.PushObject(MapX, MapY, UnderMouseID);
					  }
				     break;
			      default:
				     break;
			   }
			}
		}
	}

	// 오른쪽  마우스 버튼이 눌렸을때의 처리 
	if(IpD.RightPressDoneSwitch==TRUE)
	{
		IpD.RightPressDoneSwitch=FALSE;

		// 마우스가 메뉴 버튼위에 있지 않아야 된다. 
		if(MenuOfMouse<0)
		{

			// 1버튼 인터페이스가 설정되어 있으면 오른쪽 버튼은 선택 취소로 작동한다. 
		    if(pGame->Config.InterfaceMode==INTERFACEMODE_1BUTTON)
			{

             Unselect();
             ClearMenu();

	         GD.ObjectNeedSwitch=FALSE;

	         GD.SelectedOrder=0;
			 GD.SelectedOrderOption=0;
			}

		}
	}


	if(GD.SelectedOrder)
	{

		switch(GD.ObjectNeedType)
		{
		case NOOBJECT:

			if(Order[GD.SelectedOrder].IsAtb(ORDERATB_UPDATEMENU)==TRUE)
			{
				// 선택된 캐릭터의 메뉴를 업데이트한다. 
				for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
				{
					if(IsSee(GD.SelectedID[i].ID))
					{
						clifChar.UpdateMenu(GD.SelectedID[i].ID, GD.SelectedOrder);
					}
				}
			}
			else
			{
				DWORD para1;

				// 오른쪽 버튼 선택으로 입력받은 명령은, 
				if(GD.SelectedOrderOption&SELECTEDORDEROPTION_RIGHTPRESS)
				{
					para1=TRUE;
				}
				else
					para1=FALSE;


			    if(ControlKeyPressSwitch==TRUE)
   		            MakeOrder(UserPlayerNumber, GD.SelectedOrder, para1, ZeroID, FALSE);
			    else 
				    MakeOrder(UserPlayerNumber, GD.SelectedOrder, para1, ZeroID, TRUE);
			}

			GD.SelectedOrder=0;
			GD.SelectedOrderOption=0;
			GD.ObjectBufferIndex=0;
			break;

		case OBJECT1:

			// 이미 대상이 선정되어 있다면, 
			if(GD.ObjectNeedSwitch==FALSE)
			{

				// 선택된 명령이 이동일때 이동 마크를 표시하게 하고 이동 효과음을 낸다. 
				if( GD.SelectedOrder == ORDER_MOVE
				||  GD.SelectedOrder == ORDER_PATROL
				|| (GD.SelectedOrder == ORDER_ATTACK && GD.ObjectBuffer[0].ID.IsEmpty()) )	
				{
					// 선택된 유닛이 이동 명령을 받을때 효과음을 내게 만든다. 
					int effectnumber = 0;

					for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
					{
						id = GD.SelectedID[(i + GD.Rand) % MAX_SELECTED_CHAR_NUMBER].ID;

						if(IsSee(id) == TRUE)
						{
							if(effectnumber == 0)
							{
								if(KI[ clifChar.GetKind(id) ].MoveEffect)
								{
#ifndef _USE_HASHCODE_EFFECTSOUND
							        PushEffect(KI[ clifChar.GetKind(id) ].MoveEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#else
									HashCodePushEffect(KI[ clifChar.GetKind(id) ].MoveEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#endif
							         effectnumber++;
								}
							}
						}
					}

					SHORT realx, realy;
					Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, realx, realy);
					GD.MoveMark.SetMoveMarkReal(realx, realy);

				}
				// 선택된 효과음이 공격일때 공격 효과음을 낸다.
				else if(GD.SelectedOrder==ORDER_ATTACK )
				{
					int effectnumber=0;
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						id=GD.SelectedID[(i+GD.Rand)%MAX_SELECTED_CHAR_NUMBER].ID;

						if(IsSee(id)==TRUE && clifChar.CanAttack(id, GD.ObjectBuffer[0].ID)==TRUE)
						{
							if(effectnumber==0)
							{
 								if(KI[ clifChar.GetKind(id) ].AttackEffect)
								{
#ifndef _USE_HASHCODE_EFFECTSOUND
							        PushEffect(KI[ clifChar.GetKind(id) ].AttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#else
									HashCodePushEffect(KI[ clifChar.GetKind(id) ].AttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#endif
							         effectnumber++;
								}
							}
						}
					}
				}
				// 선택된 효과음이 수확일떄 
			    else if(GD.SelectedOrder==ORDER_HARVEST)
				{
					// 선택된 유닛이 공격이나 수확 명령을 받을때 효과음을 내게 만든다. 
					int effectnumber=0;

					// 대상지의 곡물 종류를 확인한다. 
					SHORT crop_type = clifMap.GetCropTypeMap(GD.ObjectBuffer[0].X, GD.ObjectBuffer[0].Y);

					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						id=GD.SelectedID[(i+GD.Rand)%MAX_SELECTED_CHAR_NUMBER].ID;


						if(IsSee(id)==TRUE && IsCropHarvest(id, crop_type)==TRUE)
						{
							if(effectnumber == 0)
							{
								if(KI[ clifChar.GetKind(id) ].AttackEffect)
								{
#ifndef _USE_HASHCODE_EFFECTSOUND
							        PushEffect(KI[ clifChar.GetKind(id) ].AttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#else
									HashCodePushEffect(KI[ clifChar.GetKind(id) ].AttackEffect,
										       clifChar.GetX(id), 
											   clifChar.GetY(id));
#endif
							         effectnumber++;
								}
							}
						}
					}
				}
				// 집결지 설정을 표시한다. 
				else if(GD.SelectedOrder == ORDER_TRAINSPOT)
				{
					if(GD.ObjectBuffer[0].ID.IsEmpty())
					{
					   SHORT realx, realy;
					   Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, realx, realy);
					   GD.MoveMark.SetMoveMarkReal(realx, realy);

					}
				}
				// 건설 명령이면 
				// 지뢰 매설 명령이면, 
				else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_BUILD)
				|| GD.SelectedOrder==ORDER_MINE)
				{
					PushEffect(EFFECT_DECIDEBUILD, 0, 0);
				}


				// 선택된 캐릭터가 있으면, 
				if(GD.ObjectBuffer[0].ID.IsEmpty()==FALSE)
				{
					if(IsAlive(GD.ObjectBuffer[0].ID.ID)==TRUE)
					{
						// obj로 선택된 캐릭터의 newselectedswitch를 설정한다.  
						clifChar.SetNewSelectedSwitch(GD.ObjectBuffer[0].ID.ID, TRUE);
					}
				}

				if(ControlKeyPressSwitch == TRUE)
				    MakeOrder(UserPlayerNumber, GD.SelectedOrder, GD.ObjectBuffer[0].X | (GD.ObjectBuffer[0].Y << 16), GD.ObjectBuffer[0].ID, FALSE);
				else
					MakeOrder(UserPlayerNumber, GD.SelectedOrder, GD.ObjectBuffer[0].X | (GD.ObjectBuffer[0].Y << 16), GD.ObjectBuffer[0].ID, TRUE);


				GD.SelectedOrder         = 0;
				GD.SelectedOrderOption   = 0;

				GD.ObjectBufferIndex=0;
			}

			break;
		}
	}

	return FALSE;
}


void InitGameSetCharOnline(SI32 &userpointindex)
{
#ifdef _ONLINE
	SI32 i, j, id;
	SI32 x, y;
	SI32 maxoff;
	SI32 kind;
	SI32 index = 0;


  	// 플레이어 시작 위치에 적절한 캐릭터를 배치한다.
	SHORT posindex=0;

	for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{
		maxoff = 1;
		
		// 유효하지 않은 플레이어는 없앤다. 
 		if(OBInfo.OIBPI[i].siPlayersUniqueID == 0)continue;
		
		index = 0;

		for(j = 0;j < MAX_SET_CHAR_POINT_NUMBER; j++)
		{
			if(index >= ON_MAX_FOLLOWER_NUM)continue;
			
			if(OBInfo.OIBPI[i].uiKind[index] == 0)
			{
				index++;
				continue;
			}
			else
				kind = TransKindOnlineToImjin(OBInfo.OIBPI[i].uiKind[index]);

			//체력이 없는 캐릭터는 만들지 않는다. 
			if(OBInfo.OIBPI[i].Iparameta[index].GetLife() > 0)
			{
				id = FindEmptyID(index);
				
				if(id)
				{
					x = Map.clMapPlayerInfo[posindex].StartPoint.x + SetCharPoint[j].x; 
					y = Map.clMapPlayerInfo[posindex].StartPoint.y + SetCharPoint[j].y;

					if( Map.IsInMapArea(x, y) == TRUE && Map.IsEmpty(x, y))
					{
						// CR[id]에 캐릭터를 등록한다.
						SetChar(kind, id, x, y, SOUTH, 100, i, &OBInfo.OIBPI[i].Iparameta[index], &OBInfo.OIBPI[i].MW[index], index);

						OBInfo.OIBPI[i].Iparameta->GetLife();
						OBInfo.OIBPI[i].Iparameta->GetMana();


						
						// 사람 플레이어이면 전송 받은 이름을 사용한다. 
					 	if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
						{
							
							if(strcmp(OBInfo.OIBPI[i].szPlayerName[index], "") != 0)
							{
								CR[id].SetName(OBInfo.OIBPI[i].szPlayerName[index], ON_ID_LENGTH);
							}
							else
							{
								clGrp.Error("NO name", "kind:%d", OBInfo.OIBPI[i].uiKind[index]);
							}
						}
						// NPC이면 이미 저장된 이름을 사용한다. 
						else
						{
							CR[id].SetName(KI[kind].Name, ON_ID_LENGTH);
						}

						index++;
					}
				}
				else
				{
					clGrp.Error("SetChar Error", "Can't Find EmptyID!");
					index++;
				}
			}
			else
				index++;
		}
			
		if(i == UserPlayerNumber)	userpointindex = posindex;
		posindex++;
	}
#endif
}


BOOL FindProperOnlineBattleMap(SI32 playercount, SI32 maptype, char* mapname)
{
	FILE	*fp;

	CHAR	buffer[1024];
	SI32    xsize, ysize;
	SI32	type;
	CHAR	delimiter[] = "`\t\n";
	CHAR	*pToken = NULL;
	INT		index;
	SI32 mapplayercount;
	char name[128];
	char *mapnamelist[100];
	SI32 i;

	if(maptype == 2 || maptype == 3 || maptype == 4 || maptype == 5 || maptype == 6 || maptype == 7)
	{
	}
	else
	{
		maptype = 1;
	}

	// savepath를 고려하여 화일이름을 보정한다. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "OnlineBattleMap.dat" );

	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// 첫번째 6줄은 무시한다. 
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);
	fgets(buffer, 1024, fp);

	index = 0;

	while( fgets(buffer, 1024, fp) != NULL)
	{ 
		sscanf(buffer, "%s %d %d %d %d", name, &xsize, &ysize, &mapplayercount, &type);

		if(playercount <= mapplayercount && mapplayercount <= playercount + 3
		&& maptype == type)
		{
			if(index < 100)
			{
				mapnamelist[index] = new char[128];
				strcpy(mapnamelist[index], name);
				index++;
			}
		}
	}

	fclose(fp);

	if(index)
	{
		
		SI32 data = GD.Random() % index;
		strcpy(mapname, mapnamelist[data]);
		// 할당한 메모리를 해제한다. 
		for(i=0;i<index;i++)
		{
			delete[] mapnamelist[i];
		}

		return TRUE;
	}

	return FALSE;

}



void SetScrollProcSwitch(BOOL mode)
{
	ScrollProcSwitch = mode;
}


// OnlineBattleInfo의 정보가 유효한지 알려준다. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo)
{
	SI32 i, j;
	SI32 playerswitch = FALSE;
	SI32 playercount  = 0;

	for(i=0; i<MAX_MULTI_PLAYER; i++)
	{
		// 사용자가 없는 경우, 
		// 사람인 경우에만 이름이 있다는 것을 근거로 체크.
		if(pobinfo->OIBPI[i].szPlayerName[0])
		{
			playerswitch = TRUE;
		}
		// 전투에 참여하는 사용자의 
		if(pobinfo->OIBPI[i].siPlayersUniqueID)
		{
			playercount++;
		}
 
		for(j=0;j<ON_MAX_FOLLOWER_NUM;j++)
		{ 
			// 보너스 점수가 마이너스인 경우, 
			if(pobinfo->OIBPI[i].Iparameta[j].GetBonus()  < 0)
			{
				clGrp.Error("Minus Bonus", "Bonus:%d", pobinfo->OIBPI[i].Iparameta[j].GetBonus());
				pobinfo->OIBPI[i].Iparameta[j].SetBonus(0);
				return FALSE;
			}
		}
	}


	// 전투에 임하는 사용자가 없는 경우라면, 
	if(playerswitch == FALSE)
	{
		clGrp.Error("No Human Player", "[%s][%s][%s][%s][%s][%s][%s]", 
			pobinfo->OIBPI[0].szPlayerName,
			pobinfo->OIBPI[1].szPlayerName,
			pobinfo->OIBPI[2].szPlayerName,
			pobinfo->OIBPI[3].szPlayerName,
			pobinfo->OIBPI[4].szPlayerName,
			pobinfo->OIBPI[5].szPlayerName,
			pobinfo->OIBPI[6].szPlayerName);
		
		return FALSE;
	}
	
	// 전투에 참여한 플레이어 수가 2보다 적으면, 
	if(playercount < 2)
	{
		clGrp.Error("Not Enough Players", "PlayerCount:%d", playercount);
		return FALSE;
		
	}
	// 전투에 참여한 플레이어 수가 7보다 많으면, 
	if(playercount > 7)
	{
		clGrp.Error("Too Many Players", "PlayerCount:%d", playercount);
		return FALSE;
	}
	
	return TRUE;
}

// 퇴각에 의해서 경험치 손실을 치룬다.
void SendBattleMessage_LoseForRetreat()
{

	// 해킹 우려가 없을때에만 전송한다. 
	{
		// 주인공 캐릭터의 레벨을 얻어온다. 
		SI32 level = min(100, OBInfo.OIBPI[UserPlayerNumber].Iparameta[0].GetLevel());
		
		SI32 exp = min(0, -GetExpReduceForDying(level)/2);

		if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
		{
			for(SI16 id = 0 ; id < 200; id++)
			{
				if(CR[id].GetPlayerNumber() == UserPlayerNumber)
				{
					if(IsAlive(id))
					{
						if((TransKindImjinToOnline(CR[id].CharStatus.GetKind()) & 255) == 'C')
						{
							if(CR[id].m_ParmeterBox.IP.CheckCheckSum() == TRUE)
							{
								OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(MyNet.PlayersdpId.dpId[UserPlayerNumber ], 0, exp);

								char buffer[128];
								wsprintf(buffer, Text.ExpLost.Get(), -exp);
								SetMessage(buffer);
							}
						}
					}
				}
			}
		}
		
		
	}
//	else
//	{
//		SetMessage("해킹의 가능성이 있는 데이터입니다.");
//		m_ParmeterBox.IP.SetLife(0);
//	}
	
}

#endif
*/