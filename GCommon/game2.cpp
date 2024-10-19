//#ifndef _ROCKET_NEW_TILE_MAP

//--------------------------------------------------------------------
//  ��� : ���°�
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

#include	<OnlineInputStatusControl.h>	// actdoll (2004/11/03 18:18) : IME ��Ʈ�ѷ� ����

// OnlineBattleInfo�� ������ ��ȿ���� �˷��ش�. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo);

extern _clGame *pGame;


extern	SHORT 				GamePauseSwitch;					// ������ ��� �ߴܵǾ��°�?
extern  _Chat 				Chat;
extern	RECT				ScreenRect;							// ������ ǥ�õǴ� ���� 
extern	_ScreenPoint 		SP;									// ȭ�鿡 ���� ���� ���� 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;
extern	_YSMap 				SMap;								// ���� ������ ������ 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	XSPR 				PannelSpr;
extern	SHORT 				MapEditMode;						// ���� ���� ����ΰ�?
extern	_MapEdit 			MapEdit;
extern	SHORT 				GameMenuShowSwitch;					// �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	DWORD				PrevMultiClock;
extern	DWORD				MessageSendFrame;					// ����̳� NO_MESSAGE�� ���� ������ 
extern	BOOL				GameDropSwitch ;
extern	int 				DragSwitch;
extern	int 				MapDragSwitch;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern	DWORD 				GameRecordSwitch;
extern	clock_t				StartClock, CurrentClock;						// �ð� ���� 
extern	_MyNetGame			MyNet;
extern	SI32				MaxMapXsize;
extern	SI32				MaxMapYsize;
extern	char 				SavePath[];
extern	UI08 				ScreenMapBuffer[];					// ȭ�鿡 ���� ������ �ӽ÷� �����ϴ� �� 
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				MapX, MapY;							// ���콺�� ��ġ�� small ��ǥ 
extern	int 				DragStartX, DragStartY, DragEndX, DragEndY;
extern	_FileVary 			FV;
extern	_Help 				Help;
extern	SHORT				GameStatus;							// ������ ���� ��Ȳ�� �˸��� ���� 
extern	SHORT				GameStatusBuffer;
extern	int					bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
extern _InputDevice			IpD;
extern	int					CDStopSwitch;						// CD������ ���� ��Ų��. 
extern	_Char				CR[];								// ĳ������ ������ 
extern	int 				MouseMode;							// ���콺�� ó�� ���¸� ���� 
// Ű���� ����
extern	int 				ControlKeyPressSwitch;
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameRealX, GameRealY;
extern	SHORT 				UnderMouseCropType;
extern	SHORT 				UnderMouseCropMount;
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	int 				MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
extern	_NewID 				UnderMouseID;
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	DWORD 				GameWParam;
extern	int 				DragDoneSwitch;
extern	int 				MenuMode;							// �޴��� ���¸� ���� 
extern	BOOL 				IsMosueInControlSwitch;				// ���콺�� ���� ��Ʈ������ ��ġ�ϰ� �ִ�.
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_Order 				Order[];
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	SHORT 				MouseHarvestX, MouseHarvestY;
extern	SHORT 				MenuOfMouse;						// ���콺 Ŀ���� ��ġ�ϰ� �ִ� �޴��� ��ȣ 
extern	_NewID 				ZeroID;
extern  char 				UserSelectMapFileName[];			// ����ڰ� ������ ���� �̸��� ������ ����   
extern	int   				MultiPlayDelayTime;
extern  BOOL				bGameAllyWindowSwitch;

extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;			// ĳ���� �׸���
extern LPDIRECTDRAWSURFACE7 lpPannelSurface;		// �ǳڿ� �� 

extern BOOL IsMouseInPannelSwitch  ;
extern BOOL	IsAllyWindowSwitch;

extern	_OnlineBattleMsgQue OBMQue;


extern OnlineBattleInfo OBInfo;
// �ִ� ������ �̰� 
extern SHORT DPMaxFrameDiff;

extern clock_t FrameDelay ;   // �����Ӵ� Ŭ��
extern int FrameClockDelay;

extern SoundManager	*g_pSoundManager;

static DWORD PrevFrameClock;
static DWORD PrevGameWinConditionFrame;
static BOOL IsMouseInPannelSwitch   = FALSE;
static BOOL IsAllyWindowSwitch      = FALSE;
static BOOL ScrollProcSwitch;

int ObjectTempSelectedSwitch=FALSE;
int Scroll;
SHORT ScrollDoneSwitch=FALSE;  // ��ũ���� �̷�����°�?
SHORT DoubleClickedID;  // ���� Ŭ������ ���õ� ���̵� 

BOOL IsMouseInMapSwitch      = FALSE;
BOOL IsMouseInBigMapSwitch   = FALSE;
BOOL IsMouseInSmallMapSwitch = FALSE;

static SI32 BattleMusic	= 0;
OnlineNoticeList *pOnlineNoticeList;

clock_t FrameDelay = 0;   // �����Ӵ� Ŭ��

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

	// �ȷ�Ʈ�� �����Ѵ�. 
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


	// ������ ������ �ߴܵǾ� �־��ٸ� �Ӱ��Ѵ�. 
	GamePauseSwitch=FALSE;
	bGameAllyWindowSwitch = FALSE;

	// ������ �ؽ�Ʈ �Է� ��尡 �����Ǿ� �־��ٸ� �����Ѵ�. 
	Chat.SetInputModeSwitch(FALSE, 0);


	// ������� ���ǽ��� �����Ѵ�. 
	CustomSurfaceRelease();
	
	// ������ ���ǽ��� �ٽ� �����. 
	CreateSurface();


	// ȭ���� ������ �����Ѵ�. 
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


	// ���� ������ �ʱ�ȭ �Ѵ�.
	SMap.Init(Map.GetXsize(), Map.GetYsize());
	
	
	Map.MakeSmallMapBase();

	
	if(GD.GameMode == GAME_MODE_ONLINEBATTLE)
	{
		//���� ������ �׻� ��ȿȭ ��Ų��. 
		GD.SetSmallMapValidSwitch(TRUE);

		// �ڵ� ���� ��� �Ұ�. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// ��� ���׷��̵� �Ϸ��Ѵ�.
		GD.SetUpgradeAllSwitch(TRUE);

		// �¶��� ������ ���ǵ�� �����Ѵ�. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// ĳ���͵��� �þ߸� �������� ó������. 
		GD.SetStraightSightMode(TRUE);

	}
	if(GD.GameMode == GAME_MODE_HERO)
	{
		//���� ������ �׻� ��ȿȭ ��Ų��. 
		GD.SetSmallMapValidSwitch(TRUE);

		// �ڵ� ���� ��� �Ұ�. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// ��� ���׷��̵� �Ϸ��Ѵ�.
		GD.SetUpgradeAllSwitch(TRUE);

		// �¶��� ������ ���ǵ�� �����Ѵ�. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// ĳ���͵��� �þ߸� �������� ó������. 
		GD.SetStraightSightMode(TRUE);

	}


	LoadPannel(GD.Player[UserPlayerNumber].PI.Country);


	// �ǳ��� surface�� �ű��.
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


	// ������ ���� ���� �⺻���� �ӵ��� ���´�.
	FrameDelay = pGame->Config.GetRealGameSpeedClock();

	// Ű���۸� �ʱ�ȭ�Ͽ� ������ �Է��� �����Ѵ�. 
	GD.KB.InitKeyBuffer();

	// ���� �޴��� ������ �ʵ��� �����. 
	GameMenuShowSwitch=FALSE;

	InitFrameClock();

	InitGetResource();

	PrevGameWinConditionFrame = 10000;
	PrevMultiClock   = 0;
	MessageSendFrame = 0;  // ����̳� NO_MESSAGE�� ���� ������ 

	GameDropSwitch = FALSE;


	// ���� �巡���� �ʱ�ȭ�Ѵ�. 
	DragSwitch = FALSE;
	MapDragSwitch = FALSE;
	GameLeftPressSwitch = FALSE;
	GameRightPressSwitch = FALSE;

	// ���� ���
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
		// ���� ����̸� ��� ȭ���� �� ����. 
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
		//���� ������ �׻� ��ȿȭ ��Ų��. 
		GD.SetSmallMapValidSwitch(TRUE);
	}		
	


	// ���� ���� ���� ����̸� ��� ���׷��̵带 �� �Ϸ��Ų��. 
#ifdef _QUICKMODE
	GD.SetUpgradeAllSwitch(TRUE);
#endif

	PrevMultiClock = CurrentClock;

}


// ���ӿ� ���� ���� �ʱ�ȭ���ִ� �Լ� 
//BOOL InitNoStageGame()
//{
//	SI32 userpointindex = 0;
//
//	// ������ �� �����͸� �ʱ�ȭ�Ѵ�. 
//	GD.Init();
//	InitVolcano();
//
//	// ��Ƽ���ÿ��� ���� ������, 
//   GD.RandomSeed = 1;
//
//	// ���� ����� ���Ѵ�. 
//	GD.GameMode = GAME_MODE_NORMAL;
//
//	Map.InitMap(100, 100, FALSE);
//
//	
//	Map.SetMapDrawSwitch(TRUE);
//
//	// ������ ȭ���� �ҷ��´�. 
//	Map.MH.MapType = MAP_TYPE_NORMAL;
//	LoadMapTile(Map.MH.MapType);
//	
//	// ȭ���� ������ġ�� �ű��. 
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

	// ������ �� �����͸� �ʱ�ȭ�Ѵ�. 
	GD.Init();
	InitVolcano();



/////////////////////////////////////////////////////////////////////////////////////

	// �� ���� ���̵�. 
	OBInfo = GOnlineImjin2I.BattleInfo;
	
	
	UI32 size = 0;
	UI32 startclock = timeGetTime();
	
	
	// ������ �ٰŷ� ���� ��� �������� �����Ѵ�. 
	MyNet.DecideTransVary(300);
	
	// ������ �ʿ��� ������ ���´�.
	
	// �߸��� ������ ������ Ȯ���Ѵ�. 
	if(MapEditMode == FALSE)
	{
		CheckOnlineBattleInfo(&OBInfo);
	}
	
	// �÷��̾��� ��ü���� ������ ���´�. 
	
	
	
	for( i = 0; i < MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.dpId[i] = OBInfo.OIBPI[i].siPlayersUniqueID;
		
		// �����, Ȥ�� �����ΰ��. 
		if(OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			// ����� ��쿡�� �̸��� �ִ�.
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				
				strncpy(MyNet.PlayersName.Name[i], OBInfo.OIBPI[i].szPlayerName[0], ON_ID_LENGTH);
				
				// �ι��ڸ� �־��ش�. 
				MyNet.PlayersName.Name[i][ON_ID_LENGTH] = NULL;
				
			}
			else
				strcpy(MyNet.PlayersName.Name[i], "MONSTER");
		}
	}

	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.Player[i].Init(i);

		// ��ȿ�� �÷����̾��� ��� , 
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
			
			// �ڱ� �ڽ��� �ڽſ��� �����̴�. 
			GD.OnlineAllyNumSwitch[i][i] = TRUE;
		}
	}

	// ������ �׻� 1�̴�. 
	MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1]				= 1;
	MyNet.PlayersdpId.Player[MAX_MULTI_PLAYER-1].Country	= COUNTRY_NEUTRAL;
	MyNet.HostPlayerDpId = MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1];;


	MyNet.PlayersdpId.GameMode= GAME_MODE_NORMAL;

	// ������� ��ȣ�� ���Ѵ�. 
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

	// ���� �õ带 ���´�.
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
   // NPC���� �������� �����Ѵ�. ///
   //////////////////////////////
   
   SI32 mon_player = 0;
	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		// ��ȿ�� �÷����̾��� ��� , 
		if(MyNet.PlayersdpId.dpId[i]) 
		{
			// NPC���� ������ �����Ѵ�. 
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

	// �÷��̾� ������  ������ �δ´�. 
	// PK�� ������ �ʴ� �ʵ忡����..(���Ǵ� �ʵ�� �ƹ��ϰ� �������� �ʴ´�.)
	if(pGame->pOnlineWorld && pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
	{
		if(MyNet.PlayersdpId.dpId[0] && MyNet.PlayersdpId.dpId[1])
		{
			// �÷��̾�� ������ ����������(PK)
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
			// ������ ����
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

	// ���� ����� ���Ѵ�. 
	GD.GameMode = GAME_MODE_ONLINEBATTLE;
	
	char mapname[128]="";
	
	// ������ Ÿ���� ���´�. 
	if(MapEditMode == TRUE)
	{
		Map.MH.MapType = MAP_TYPE_NORMAL;
		
		Map.InitMap(100, 100, FALSE);
		
	}
	else
	{
		
		// ���ӿ� �����ϴ� �÷��̾ �� ������ Ȯ���Ѵ�. 
		SHORT playercount=0;
		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			// �������� �ʴ� �����̸�, 
			if(MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NO
				&& MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NEUTRAL)
			{
				playercount++;
			}
		}
		
		// �÷��̾� ���ڰ� ���������̸� �˸���. 
		if(playercount < 2)
		{
			clGrp.Error("f9837521", "[%d]", playercount);
		}
		
		SI32 maptype = pGame->pOnlineWorld->GetWarFieldMap();
		if(FindProperOnlineBattleMap(playercount, maptype, mapname) == TRUE)
		{
			
			// ������ �ʱ�ȭ�Ѵ�.
			//				strcpy(buffer, SavePath);
			strcpy(buffer, "Online\\BattleMap\\");
			strcat(buffer, mapname);
			
			Map.Load(buffer);
		}
		else
		{
			clGrp.Error("125308", "no map [%d]", playercount);
		}
		
		// Ȯ�� ������ �����Ѵ�.
		Map.ExpansionBaseNumber=0;
		for(i = 0;i < MAX_MULTI_PLAYER;i++)
		{
			if(Map.clMapPlayerInfo[i].StartPoint.x >= 0 && Map.clMapPlayerInfo[i].StartPoint.y >= 0)
			{
				Map.PushExpansionBasePoint((SHORT)Map.clMapPlayerInfo[i].StartPoint.x, (SHORT)Map.clMapPlayerInfo[i].StartPoint.y);
			}
		}
		
	}

	//�����ϴ� ��ġ�� �����ϰ� �ٲ۴�.
	Map.RandomStartPos();
	
	// ������ ȭ���� �ҷ��´�. 
	LoadMapTile(Map.MH.MapType);
	Map.SetMapDrawSwitch(TRUE);

    // �ҷ����ų� ���Ƿ� ������ ������ ���������� Ȯ���Ѵ�. 
    Map.MakeLastMap();

	   // ���ÿ��� ������ ������ �Ѱ��ش�. 
	   for(i=0;i<MAX_MULTI_PLAYER;i++)
	   {
		   SHORT pnumber=MyNet.PlayersdpId.Player[i].PlayerNumber;
		   GD.Player[pnumber].PI=MyNet.PlayersdpId.Player[i];
	   }
	   
	   
	   // EnemyBit�� �����Ѵ�. 
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


	 // �߸����̸�, 
	 if(GD.Player[UserPlayerNumber].PI.Country==COUNTRY_NEUTRAL)
	 {
		 // ȭ���� ������ġ�� �ű��. 
		 Map.VirtualMapX=0;
		 Map.VirtualMapY=0;
		 
		 GD.HomeX=Map.VirtualMapX;
		 GD.HomeY=Map.VirtualMapY;
	 }
	 else
	 {
		 
		 // ȭ���� ������ġ�� �ű��. 
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

	 // �������� �ҷ��� ������Ʈ �Ѵ�. 
	 pGame->Config.GetTeamInfo();

#ifdef _OGG_PLAY
	 GetBgmMgr()->Play(OGGBGM_FILENAME_WARTITLE, TRUE);
#else
	 // ������ ������ �����Ѵ�.
	 if(g_pSoundManager)
	 {
		 g_pSoundManager->StopAllSound();
		 
		 BattleMusic	=	g_pSoundManager->AddPlayID( "music\\Music3.YAV", FALSE, TRUE, FALSE );
		 g_pSoundManager->PlaySound( BattleMusic );	
	 }
#endif

 	// ��� �޽����� ó���� �� �ִ� Ŭ������ �����͸� ���´�. 
	pOnlineNoticeList = pGame->GetOnlineNoticeListPointer();

	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->SetPosition(233, 582, 335,4);
	}

	// ���� ���ϸ� ����ġ�� �Ҵ´ٴ� ��� �˷��ش�.
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

	// ORDER_QUIT ����� �ѹ��� ����Ǿ�� �ϱ� ������ �����ʱ⿡ ������ ����ġ�� �ʱ�ȭ�Ѵ�. 
	GameQuitSwitch = FALSE;

	// ������ ȹ�� ����â�� ���ش�. 
	pGame->pItemInfoWindow->ClearAllItemInfoWindow();



	return TRUE;
}

void _clGame::ChatInit()
{
	Chat.Init();
}




BOOL GameDraw1()
{

	//  �̵���ũ���ൿ 
	GD.MoveMark.Action();

	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

       // ������ �ű��. 
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
		
		// ĳ���͸� ������ ��ü���� �׸���. 
		Map.DrawObj();
		
		
		// ���� ���� �����̸� �÷��̾� ���� ��ġ�� �׷��ش�. 
		if(MapEditMode==TRUE)
		{
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				// ��ȿ�� �÷��̾� ��ġ���, 
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
	   
	   // ���� ���� ����̸�, 
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

	   // �������� ���� �ż����� ���콺 ��ġ�� �˷��ִ� ���� ���� �ٴѴ�.
	   if(GD.SelectedOrder==ORDER_MINE)
	   {
		   SHORT centerx, centery;
		   
		   // ���� ���� ��忡�� ���콺 ��ġ�� ǥ���Ѵ�. 
		   //if(Map.GetCenterXYByMapCenterXY(MapX, MapY, centerx, centery)==TRUE)
		   if(Map.GetRefXY(MapX, MapY, centerx, centery)==TRUE)
		   {
	          Map.DrawMousePos(centerx, centery, MapX, MapY, 0, TRUE);
		   }

	   }

	   // ��� ���� �׸���.
	   GD.Rain.Draw();


	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	

    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

		if(MapDragSwitch==TRUE)
		{
		   Map.DrawDragBox(DragStartX, DragStartY, DragEndX, DragEndY);
		}

	   // �̵� ��ũ ��� 
       GD.MoveMark.DrawMoveMark();

 
	   // �ΰ� �׸��� 
	   Draw_PutSpriteLightT(clGrp.GetScreenXsize()-GetFileXsize(FV.OnlineLogoFile) - 10, 
		   ScreenRect.bottom-GetFileYsize(FV.OnlineLogoFile)-70, 
		   FV.OnlineLogoFile,0);	// hojae_append
	   
	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	   // ������ ȹ�� â�� �����ش�. 
	 pGame->pItemInfoWindow->Render(lpScreenBuffer);



	// �ǳڿ� ���� ������ �׸���. 
	DrawSmallMapImjin();
	
	//ä��â �׸���.
	pGame->m_ChatList.Process(lpScreenBuffer, 1);

	// �ǳ���  �׸��� 
	DrawPannel();

	// �Է����� ������ ǥ�����ش�. 
	Chat.DrawInputText();

	// ������������ ȭ�鿵���� ��Ÿ���� ���� �簢���� �׸���. 
	if(IsSmallMapValid())
	{
	    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
	       SMap.DrawBox(Map.VirtualMapX, Map.VirtualMapY);
	       clGrp.UnlockSurface(lpScreenBuffer);
		}
	}

	// �޴��� �׷��ش�. 
	DrawMenu(lpScreenBuffer);

	// �޽����� ������ ǥ���Ѵ�. 
	ShowMessage(10, 70, lpScreenBuffer);

	// ���� �ð��� ǥ���Ѵ�. 
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

	// ��� �޽��� ó��. 
	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->Poll(lpScreenBuffer);
	}
	// ===============================================================================
//	if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
	// ===============================================================================
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	// ===============================================================================
	{
		// ��� �޽����� ����Ѵ�. 
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
	
	// ���콺 ���� � ������ ǥ���Ѵ�. 
	ShowCropInfo(lpScreenBuffer);

	// ���� ���� ��忡���� ������ ǥ���Ѵ�. 
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

	// ���� ������ �����ش�. 
	Help.Draw();

    //////// lpDDSBack�� lpDDSPrimary�� �����Ѵ�.
	DrawMouse(lpScreenBuffer);

	
	GD.EarthQuake.Action();

	RECT rect;
	rect.left   = 0;
	rect.right  = clGrp.GetScreenXsize();
	rect.top    = GD.EarthQuake.GetOffsetY();
	rect.bottom = clGrp.GetScreenYsize();

	// actdoll (2004/11/03 17:28) : IME ��Ʈ�ѷ� ����...
	//	������ ��������� ��ġ�� �߱������̶� ����ڵ带 ����� ���� �� �ۿ� ����.
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


	// ���ǽ��� �ν�Ʈ�� ���̸�, 
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
	// ��ȭ�밡 ������ ���� ������ �׷��ְ�, 
	if(IsSmallMapValid())
	{
		SHORT drawswitch=FALSE;
		
		// ���� ������ ǥ�þȵǴ� ���¿��� ���̸�,
		if(GD.SmallMapDrawSwitch==FALSE)
		{
			// ȿ������ ����. 
			if(GD.SmallMapDrawSwitch==FALSE)
			   PushEffect(EFFECT_MAP1, 0, 0);

			drawswitch=TRUE;
		}
		// ���̴� ���¿����� 30�����Ӹ���..
		else
		{
			if(abs(GD.Frame - GD.DrawSmallMapFrame) >= 30)
			{
				GD.DrawSmallMapFrame = GD.Frame;
				drawswitch=TRUE;
			}
		}

		GD.SmallMapDrawSwitch=TRUE;

		// ���������� ��ΰ� ����Ǿ��ٸ�, 
		if(GD.SmapModeChangedSwitch==TRUE)
		{
			GD.SmapModeChangedSwitch=FALSE;

			drawswitch=TRUE;
		}

		if(drawswitch==TRUE)
		{
		    if(clGrp.LockSurface(lpPannelSurface)==TRUE)
			{
				// �������� ���̴� ����̸�, ������ �׷��ش�. 
	            SMap.Draw(GD.SmapModeFullSwitch);

 	            SMap.DrawChar();

	            clGrp.UnlockSurface(lpPannelSurface);
			}
		}
	}

	// ��ȭ�밡 ������ ���� ������ ���ش�.
	if(IsSmallMapValid() == FALSE && GD.SmallMapDrawSwitch==TRUE)
	{
		// ȿ������ ����. 
		if(GD.SmallMapDrawSwitch==TRUE)
		   PushEffect(EFFECT_MAP2, 0, 0);

		GD.SmallMapDrawSwitch=FALSE;

		// �ǳ��� surface�� �ű��.
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

		// ���ǽ��� �ν�Ʈ�� ���̸�, 
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


// ���� ������ ��ƾ�� ����ִ� �Լ� 
// ���ϰ�: 0 -> normal
SHORT GameProcess()
{
	// ������ ��� ������ �����ð����� ���� ������ �����ش�. (�����ð����� üũ�� �Լ������� ó���Ѵ�.)
	pGame->OnlineSendHeartBeat();

	// ������ ä�� �Է��� �����Ѵ�.
	InputOperateInGame();
	
	
	if(GD.Frame==0)StartClock = CurrentClock;
	

	// ���� �޴��� �����ش�. 
	if(GameMenuShowSwitch==TRUE)
	{
		// ���ӿ��� ���Ӹ޴��� �θ� ������,
		if(GameStatus==GS_GAME)
		{
			// ���� �޴������� ���콺�� �⺻ ���콺�� ����Ѵ�.
			SetMouse(MOUSE_NORMAL);
			
			// �ȷ�Ʈ�� �ʱ�ȭ���ش�. 
			SetGamePalette(clGrp.Palette);
			
			GameStatus=GS_INITGAMEMENU;

			// ���� �����̸Ӹ� ȭ���� lpscreenbuffer�� �ű��. 
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
					// ���콺�� �����. 
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
		// ȭ���̵����� 
		if(ScrollProcSwitch == FALSE)
		{
			ScrollOperation();
			SetScrollProcSwitch(TRUE);
		}
		
		// ġƮŰ�� �� ����� �ο��Ѵ�. 
//		Cheat.CheatOrder();
	}
	// ���� Ÿ���� �׸���. 
	Map.DrawMap();

	if(GamePauseSwitch==FALSE)
	{
		// ����� ����� ó���Ѵ�. 
		PopOrder();
	}

	// ������ ������Ų��. 
	if(GameDelay() == FALSE)return 0;



	// �ʱ�޴��� ���ư��� ī��Ʈ �ٿ�. 
	if(GD.InitInitMenuCount)
	{
		GD.InitInitMenuCount--;

		if(GD.InitInitMenuCount == 0)
		{
			OperateGameMenuInitInitMenu();
			return 0;
		}

	}

	// ������ �����ϴ� ī��Ʈ �ٿ�. 
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

	// �� 1�д� ���� ������ ��Ʈ��Ʈ �޽����� ������. 
	if(GD.Frame && (GD.Frame % (60000/DEFAULT_MULTI_PLAY_DELAY_TIME) == 0))
	{
		OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(MyNet.PlayersdpId.dpId[UserPlayerNumber ]);
	}

	if(GD.Frame >= 20)
	{
		if(GD.Frame == 20)SetStartFrameClock(CurrentClock);
		else
		{
			// ����� �ð��� ����Ѵ�. 
			PushFrameClock(GD.Frame, CurrentClock);
		}
	}

	GameAction();


	// �����찡 Ȱ��ȭ �Ǿ� �ִ� ��쿡�� ���콺�� �׸���. 
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
	
	// �÷��� �ð��� �����Ѵ�. 
	if(abs(GD.TempAccumulatedPlayClock - CurrentClock) >= 1000)
	{
		GD.TempAccumulatedPlayClock = CurrentClock;
		GD.AccumulatedPlaySecond ++;
	}

	
	return 0;
}


// ������ �ӵ��� �����ϰ� �����. 
BOOL GameDelay()
{

	DWORD basicdelay;
	
	// ������ ���� ���� �⺻���� �ӵ��� ���´�.
	basicdelay = pGame->Config.GetRealGameSpeedClock();

	FrameDelay = basicdelay + FrameClockDelay;
	FrameClockDelay = 0;


	clock_t tempclock = CurrentClock;

	// ������ �����ߴٸ�, 
	if( abs(CurrentClock - PrevFrameClock) < FrameDelay)
	{
		// �������Ѿ� �� �ð� ,
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
		
		// ���� �����Ǿ���� �� �ð� (������ ������ ����� �ð�)
		tempclock = (CurrentClock - overclock);

	}


	PrevFrameClock = tempclock;

	return TRUE;
}


BOOL GameWinCondition()
{
	char	buffer[128];
	// �������� �޶����߸�  üũ�Ѵ�. 
	if(GD.Frame == PrevGameWinConditionFrame)return FALSE;
	PrevGameWinConditionFrame = GD.Frame;

	SHORT status = WinCondition();


	//�̱�ų� ���� ������ ���� ������ �Ѵ�. 
	if((GD.VictoryDelay && (abs(CurrentClock - GD.VictoryDelay) > VICTORY_HALFDELAY))
	|| (GD.DefeatDelay  && (abs(CurrentClock - GD.DefeatDelay)  > DEFEAT_HALFDELAY)))
	{
		if(GameQuitSwitch == FALSE)
		{
			GameQuitSwitch = TRUE;
			MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);
		}
	}

	


	// actdoll (2004/09/16 15:06) : 2004 ���ֿ����� �̺�Ʈ��
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
			
	// �¸��ߴ�. 
	if(status == 1)
	{
		//wsprintf(buffer, "�������� �¸��Ͽ����ϴ�.");
//		SetMessage("�������� �¸��Ͽ����ϴ�.");
		// �¸��Ѱ����� ����Ѵ�. 
		GD.WinSwitch = TRUE;

		GameStatus   = GS_INITWIN;

//    	EndGame(); 
//		pGame->pOnlineWorld->pOnlineClient->SendReturnOnline(TRUE);
		return TRUE;
	}
	// �й� �ߴ�. 
	else if(status == -1)
	{
		//wsprintf(buffer, "����..�ҽ�..");
//		SetMessage("�������� �й��Ͽ����ϴ�.");
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
// Desc: ������ �����ϴµ� �ʿ��� ���� 
//--------------------------------------------------------------------------
void EndGame()
{
	// actdoll (2004/11/05 15:58) : OECEX
	//	���� ����Ʈ �ڽ��� ��Ŀ���� ������ �ִ� ���¿��� ������ ������ ��Ŀ���� ���ش�.
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

	// ����� ��ȣ�� �ʱ�ȭ�Ѵ�. 
	UserPlayerNumber=0;

	Map.FreeMap();
	FreeMapTile();

	// ���콺 Ŀ���� �⺻������ ��ü�Ѵ�.
	SetMouse( MOUSE_NORMAL );


	// �ȷ�Ʈ�� ���� �ȷ�Ʈ�� �ʱ�ȭ�Ѵ�. 
	SetGamePalette(clGrp.Palette);

	// ĳ���͸� ��� ���ش�. 
	DeleteChar();

	// �Ѿ��� ��� ���ش�. 
	DeleteBullet();
	// ���⸦  ��� ���ش�. 
	DeleteSmoke();
	
	// �������� ��� ���ش�. 
	DeleteMagic();

	// ���� �߿� �ε��� �̹��� ������ �޸𸮸� �����Ѵ�.
	FreeImgFile();



	if(pGame)
	{
		if(pGame->pBattle)
			pGame->pBattle->InitGameButton();
	}
	
	// ��ü ȭ���� Ŭ���� �������� �����Ѵ�.
	clGrp.SetClipFullScreen();
}

SHORT GameAction()
{
	SHORT i;
	GD.OldRand=GD.Rand;
    GD.Rand = GD.Random();
	SHORT id = 0;


	// ���� �������� ��ŷ ���ɼ��� Ȯ���Ѵ�. 
	if(GD.Frame % 10 == 0)
	{
		// kindinfo���� ��ŷ ���θ� �˻��Ѵ�. 
		if(CheckKindInfo() == FALSE)
		{
			// ������ ��ŷ ����� �ִ� ����ڶ�� ����� �˸���. 
			// ������ �����Ų��. 
			GameStatus = GS_INITQUIT;
			
			// ��ŷ���� ���� ������ ������� �˸���. 
			//clGrp.Error("Game Quit", "There is possibility of Hacking. Your account will be blocked. Please contact us.");
		}
	}


	if(GamePauseSwitch==FALSE)
	{
	    // �ӽ÷� 
	    GD.MoveCharNumber=0;

		// ������ ȹ�� â ���� �ൿ�� �Ѵ�. 
		pGame->pItemInfoWindow->Prepare();

		// ���� ���� ��尡 �ƴϾ�� �Ѵ�. 
	    if(MapEditMode==FALSE)
		{

			// �ð��� ó���Ѵ�. 
			GD.Time.PassTime();


			// ���� �ൿ 
			GD.Rain.Action();

			VolcanoAction();
		}

		// ĳ������ �ൿ 
		GD.SyncSignal  = 0;
		int charindex;
	    for(charindex = 0;charindex < GD.CharOrderIndex;charindex++)
		{
			id = GD.CharOrder[charindex] ;
			if(GD.CharInfo[ id ])
			{
				// ���� ��ȣ�� ���´�. 
				GD.SyncSignal = GD.SyncSignal + id + clifChar.GetX(id) + clifChar.GetY(id) + clifChar.GetCurrentOrder(id) 
					+ clifChar.GetWorkStep(id) + clifChar.GetDX(id) + clifChar.GetDY(id) + clifChar.GetPara1(id) 
					+ clifChar.GetDpower(id) + clifChar.GetLife(id) + clifChar.GetMana(id);


				DWriteStringToFile(" id = %d , life =  %d",id,clifChar.GetLife(id));
				if(clifChar.Action(id)==FALSE)
				{
					DWriteStringToFile("�׾���.id = %d",id);
					DeleteChar(id);
				}
			}
		}

		{

			// ���� Ž���� �ǽ��Ѵ�.
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

		// �������� �ൿ. 
		MagicAction();

		// �Ѿ˵��� �ൿ 
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

		// ������� �ൿ  
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

		// ���� �ð��� �����Ѵ�.
		PassPlayTime();

		// ĳ���͵���  �Ȱ� ó�� 
		// ������ ���� ������ ��Ӱ� ����� ���ο� ������ ��� �����. 
		// ����.
		if(pGame->pOnlineWorld)
		{
			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,FALSE);
				// ������ ���� ������ ���ΰ� ���ο� ������ ������. 
				else
					GD.BlackCheck(FALSE,FALSE);
			}
			else
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,TRUE);
				// ������ ���� ������ ���ΰ� ���ο� ������ ������. 
				else
					GD.BlackCheck(FALSE,TRUE);
				
			}
		}
   }
   
   // �����̽� �ٸ� ���� ȭ�� �̵� ��Ų��. 
   if(GD.ScreenMoveSwitch==TRUE)
   {
       Map.VirtualMapX=GD.ScreenMoveX;
       Map.VirtualMapY=GD.ScreenMoveY;
       Map.DrawForceSwitch=TRUE;
	   
	   GD.ScreenMoveSwitch=FALSE;
   }
   
	return 0;
}


// ���콺 ��ư�� ���� �ڵ� ����� ó���Ѵ�. 
BOOL OperateAutoOrder()
{

	// ���� �ڵ� ����� �ƴ� ������� ���õ� ����� �ִٸ� �ڵ� ����� �����Ѵ�. 
	if(GD.SelectedOrder!=0 && GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
	{
	   return FALSE;
	}

	// �޴��� ��Ȯ�� �ְ� ���콺�� � ���� ������, 
	if(MouseMode&MOUSEMODE_HARVEST)
    {
		GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_HARVEST;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� �̵��� �ְ� ���콺�� ���ڸ��� ������, 
    else if(MouseMode&MOUSEMODE_MOVE)
    {
 	    GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_MOVE;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� ������ �ְ� ���콺�� �� ���� ������, 
    else if(MouseMode&MOUSEMODE_ATTACK)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_ATTACK;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� ž���� ������
    else if(MouseMode&MOUSEMODE_RIDE)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_RIDE;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� ġ�ᰡ �ְ� 
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
    // �޴��� ����� ������ �ִ� ���� �ְ�. 
    else if(MouseMode&MOUSEMODE_GOINTOHQ)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_GOINTOHQ;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
	// �޴��� ��ȭ�� �ְ� ���콺�� �� ���� ������, 
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
	// ��ũ�� ������ �ʱ�ȭ�Ѵ�. 
	Scroll=0;


	

	//��ŵ�ϴ� �����ӿ����� ��ũ���� �Ѵ�. 
	// ������ ����϶��� ������ �а� ��´�.
	if (WindowModeSwitch) {
		if(DragSwitch==FALSE )
		{
			if(abs(GameMouseX) <= SCROLL_RANGE )               Scroll|=WEST;
			if(abs(GameMouseX - (clGrp.GetScreenXsize()-1)) <= SCROLL_RANGE ) Scroll|=EAST;
			if(abs(GameMouseY) <= SCROLL_RANGE)                Scroll|=NORTH;
			if(abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE ) Scroll|=SOUTH;
			
			// Ű���忡 ���� ��ũ���� �����Ѵ�. 
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
			
			// Ű���忡 ���� ��ũ���� �����Ѵ�. 
			if(GetAsyncKeyState(VK_UP)   < -1)	   Scroll |= NORTH;
			if(GetAsyncKeyState(VK_DOWN) < -1)	   Scroll |= SOUTH;
			if(GetAsyncKeyState(VK_LEFT) < -1)     Scroll |= WEST;
			if(GetAsyncKeyState(VK_RIGHT)< -1)	   Scroll |= EAST;
		}
		
		
	}


	// ��ũ�� ���̸� ������ �׸���. 
	if(Scroll)
	{
		Map.SetMapDrawSwitch(TRUE);
	}

	// ��ũ�� ������ �ӽ÷� �����Ѵ�. 
	SHORT tempvirtualmapx=Map.VirtualMapX;
	SHORT tempvirtualmapy=Map.VirtualMapY;

	// ���� ��ũ�� 
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
	// ���� ��ũ�� 
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

	// ���� ��ũ�� 
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
	// ���� ��ũ�� 
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


	// ������ ��ũ���� �̷�� ������ ���̷�� ������ Ȯ���Ѵ�. 
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

	//altŰ ���� ���� Ȯ��        
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
	
	// �׷� ���� 
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

	// ���콺�� �������� �ִ� ���� �ƴ϶�� , 
	if(Map.IsInMapArea(MapX, MapY) == FALSE) return newid;


	// ���콺 �ؿ� �ִ� ĳ���͸� ���Ѵ�. 
	if(GD.BlackMap[UserPlayerNumber][MapX][MapY] != FOG2_BLACK_STEP)
	{
			
		// ���� ĳ���͸� ���� �����Ѵ�. 
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

		// ���߿��� ĳ���͸� ã���� �����Ѵ�. 
		if(newid.IsEmpty() == FALSE) return newid;


		// ���󿡼� ĳ���͸� ã�´�. 
		length     = 0;
		min_length = 10000;
		
		
		for(SHORT range = 0 ; range < 3 ; range++)
		{
			for(i=MapY - range;i <= MapY + range; i++)
				for(j=MapX - range;j <= MapX + range; j++)
				{
					if(i==(MapY-range) || i== (MapY+range) || j==(MapX-range) || j==(MapX+range)){}
					else continue;
					
					// ������ �������� �����ϴ��� �˻��Ѵ�. 
					if(Map.IsInMapArea(j, i) == FALSE) continue;
					
					// ������ ��ο� �������� ���� �Ұ� 
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
			
		// ���������� ���콺 �ؿ� ĳ���Ͱ� ���� ���¶��, 
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


// �巡�װ� ����Ǿ������� ó���̴�.
void DragOperation()
{
	SI32 i;
	SHORT id;
    SHORT realx1, realy1, realx2, realy2;

	// ���õ� ������ ������ �ӽ� ���۸� ����� �ʱ�ȭ�Ѵ�.
	_NewID temp_id[MAX_SELECTED_CHAR_NUMBER];
	SHORT index=0;
	SHORT newselectedid[MAX_SELECTED_CHAR_NUMBER];
	SHORT newselectednumber = 0;
	
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		temp_id[i].Init();
		newselectedid[i] = 0;
	}
	
	// �巡���� �ʱ�ȭ�Ѵ�. 
    MapDragSwitch = FALSE;

	
	// ���� ������ ���۰� ���� �����Ѵ�. 
	int min_x = min(MapDragStartX, MapX);
	int max_x = max(MapDragStartX, MapX);
	int min_y = min(MapDragStartY, MapY);
	int max_y = max(MapDragStartY, MapY);
	
	MapDragStartX = min_x;
	MapDragStartY = min_y;
	MapDragEndX   = max_x;
	MapDragEndY   = max_y;

	
	// ���콺�� �Ѱ����� Ŭ�������� 
	if(abs(MapDragStartX-MapDragEndX)<=1 &&  abs(MapDragStartY-MapDragEndY)<=1)
	{
		
		// 1��ư �������̽��̸� �ڵ� ��� ��ƾ�� �����ϰ� FALSE�� �����ϸ� ĳ���� �������� �����ȴ�. 
		// 2��ư �������̽��̸� ĳ���� �������� �����ȴ�. 
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
		{
		}
		else
		{
			// ���콺 �ؿ� ĳ���Ͱ� �ִٸ�, 
			if(UnderMouseID.IsEmpty() == FALSE)
			{
				// shiftŰ�� ���� �����̰� 
				// ���� ���� ������ ���̸�, 
				if(ShiftKeyPressSwitch  == TRUE 
				&& clifChar.GetPlayerNumber(UnderMouseID.ID) == UserPlayerNumber)
				{
					// ������ ���õǾ��� ĳ���͸� ���ۿ� �ִ´�. 
					
							index = 0;

					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// ���õ� ĳ���͸� �ִ´�. 
						if(GD.SelectedID[i].IsEmpty() == FALSE)
						{
							temp_id[index] = GD.SelectedID[i];
							index++;
						}
					}

					
					// ���콺 �Ʒ��� ĳ���Ͱ� �̹� ���õǾ� �ִ� ĳ���Ͱ� �ƴ��� Ȯ���Ѵ�. 
					SHORT alreadyselectedswitch=FALSE;
					SHORT alreadyselectedindex=0;
					BOOL invalidswitch = FALSE;
					
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// �̹� ���õ� ������ �����̰ų� ȥ�ڼ��� ���� �����̸�, 
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
					
					// ������ ���õ� ĳ���Ͱ� ������ ���õ� ĳ���Ͷ��, 
					if(alreadyselectedswitch == TRUE)
					{
						// ���õ� ����Ʈ���� �����Ѵ�. 
						clifChar.SetSelect(temp_id[alreadyselectedindex].ID, FALSE, FALSE);
						temp_id[alreadyselectedindex].Init();

					}
					// ������ ���õ� ĳ���Ͱ� ���õǾ����� ���� ĳ���Ͷ��, 
					else
					{
						// �̹� ���õ� ĳ���Ͱ� �ִ� ��Ȳ���� �ǹ���� ���� �Ѱ��� ���� ������ �����̸�, 
						// �ű� ������ ���� �ʴ´�.
						if(invalidswitch == TRUE)
						{
						}
						// �������� ���õǾ ������ ĳ������ ��쿡�� �߰��Ѵ�. 
						else
						{
							// ���� ���õ� ĳ������ ���� �ʱ�ȭ�Ѵ�. 
							newselectednumber = 0;

							// ���ڸ��� ���� �߰��� �ִ´�. 
							for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
							{
								// ���ڸ����� �ѹ��� �ִ´�.
								if(temp_id[i].IsEmpty() && newselectednumber == 0)
								{
									temp_id[i] = UnderMouseID;
									clifChar.SetSelect(UnderMouseID.ID, TRUE, FALSE);
									
									// �űԷ� ���õ� ������ �����Ѵ�.
									newselectedid[index] = UnderMouseID.ID;
									newselectednumber++;
									
									index++;
								}
							}
						}
						
						
					}
					
				}
				// ����Ʈ���� ������ �ʴ� �׹��� �Ϲ����� ������ ��� 
				else
				{
					temp_id[0] = UnderMouseID;
					index      = 1;
					
					
					// �űԷ� ���õ� ������ �����Ѵ�.
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
	// ���콺�� 2ĭ �̻��� �������� �������� �巡���� ������ 
	else  
	{
		
		Map.TransFromScreenToRealXY(DragStartX, DragStartY, realx1, realy1);
		Map.TransFromScreenToRealXY(DragEndX,   DragEndY,   realx2, realy2);
		
		// shiftŰ�� ���� ���¶��, 
		SHORT newselectstartindex = 0;
		
		if(ShiftKeyPressSwitch  == TRUE)
		{
			
			// ������ ���õ� ĳ���͸� �ӽ� ���ۿ� �ִ´�. 
			index = 0;
			BOOL invalidswitch = FALSE;
			
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				if(GD.SelectedID[i].IsEmpty() == FALSE)
				{
					temp_id[index] = GD.SelectedID[i];
					index++;
					newselectstartindex = index;

					// �̹� ���õ� ������ �����̰ų� ȥ�ڼ��� ���� �����̸�, 
					if(clifChar.GetPlayerNumber(GD.SelectedID[i].ID) != UserPlayerNumber)
					{
						invalidswitch = TRUE;
					}

				}
			}
			
			if(invalidswitch == FALSE)
			{
				// ���� �ȿ� �ִ� ĳ���͸� ���ؼ� �ӽ� ���ۿ� �߰��Ѵ�.
				SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
				
				
				// �űԷ� ���õ� ������ �����Ѵ�.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
			}
		}
		else
		{
			// ���� �ȿ� �ִ� ĳ���͸� ���ؼ� �ӽ� ���۸� ä���.
			SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
			
			// �űԷ� ���õ� ������ �����Ѵ�.
			for(i = 0;i < index;i++)
			{
				newselectedid[i] = temp_id[i].ID;
				newselectednumber++;
			}
		}
	}
	
	
	// ���õ� ������ �ִٸ�, 
	if(index > 0)
	{
		
		// ������ ĳ���Ͱ� �ϳ��̰� 
		// ª�� �ð��� ���������� ���õ� ���̶��, 
		// ȭ�� �ȿ� �ִ� ���� ������ ĳ���͵��� ���Ѵ�. 
		if(DoubleClickedID)
		{
			
			// ���� Ŭ���� ������ �Ʊ��̸� �߰��� �����Ѵ�.
			if(clifChar.GetPlayerNumber(DoubleClickedID) == UserPlayerNumber)
			{
				
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, realx1, realy1);
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.right,(SHORT)ScreenRect.bottom,   realx2, realy2);
				
				SHORT newselectstartindex = index;

				//  ȭ���� ���� �ȿ� �ִ� ĳ���͸� �ӽù��ۿ� �ִ´�.
				SelectCharInDrag(temp_id, index, realx1, realy1, 
					realx2, realy2, DoubleClickedID);
				
				
				// �űԷ� ���õ� ������ �����Ѵ�.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
				
				DoubleClickedID = 0;
				
			}
		}
		
		// ���õ� ������ ȿ������ ����. 
		SHORT effectnumber=0;
		for(i=0;i<newselectednumber;i++)
		{
			id=newselectedid[(i+GD.Rand)%newselectednumber];
			// ���õ� ĳ������ 1�������� ȿ������ ����. 
			
			if(effectnumber==0 && KI[ clifChar.GetKind(id) ].m_siSelectEffect)
			{
				clifChar.SelectOperation(id);
				effectnumber++;
			}
		}
		
		//������ ���õ� ĳ���͸� ����Ѵ�. 
		Unselect();
		ClearMenu();
		
		// ������ �����Ѵ�.
		for(i=0;i<index;i++)
		{
			id=temp_id[i].ID;
			clifChar.SetSelect(id, TRUE, FALSE);
		}
		
		
	}
	// ���õ� ������ ���ٸ�, 
	else
	{
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_2BUTTON)
		{
			//������ ���õ� ĳ���͸� ����Ѵ�. 
			Unselect();
			ClearMenu();
		}
	}
}



// Ű���� ó���� ������ ��ƾ 
BOOL GameOverHead()
{
	int i;
	SHORT id;
	DWORD wparam;
	SHORT mapx, mapy;

	SHORT leftpressswitch=FALSE, rightpressswitch=FALSE;
	SHORT mousex, mousey;
	char moveswitch;




	// �ӽ÷� ����Ѵ�. 
	if(MapEditMode == TRUE)
	{
		MapEdit.Action();
	}

	
	// �¸� ������ üũ�Ѵ�. 
	// �¸� �߰ų� �й� ������ �����Ѵ�. 
	if(GameWinCondition() == TRUE)
	{
		return TRUE;
	}
	
	// ������ �ߴܵ� ���¸� �����Ѵ�.
	if(GamePauseSwitch==TRUE)return FALSE;


	//////////////////////////////////////////////////////////
	// control, alt ���� Ư��Ű�� ���¸� Ȯ���Ѵ�. 
	//////////////////////////////////////////////////////////
	


    // Ű���� ������ ���� �׷� ���� �� ���� 

	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	{
		SystemKeyCheck();

		GroupInterface();
	}

	// ������� ���콺 , Ű���� ���� �Է� ���¸� Ȯ���Ѵ�. 
	GameWParam=0;

	if(GD.KB.PopKeyBuffer(leftpressswitch, rightpressswitch, mousex, mousey, wparam, moveswitch) == TRUE)
	{

		GameMouseX = mousex;
		GameMouseY = mousey;
    	GameWParam = wparam;
		
		// ���� ���콺 ��ư ���� 
		if(leftpressswitch >= 0)
		{

			// ���� ��ư�� ó�� �������̶��, 
	        if(leftpressswitch == TRUE)
			{
		      DragSwitch=TRUE;
	   	      DragStartX=GameMouseX;
		      DragEndX=DragStartX;
		      DragStartY=GameMouseY;
		      DragEndY=DragStartY;
			}
	        // ���� ��ư�� ���� ���¿��� Ǯ�� ���̶��, 
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
		    // ���콺�� �̵��Ǿ��°�?
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
	
	

////////////////// �ʱ�ȭ //////////////////////////

	// ���콺�� ��ġ�� small ��ǥ�� �ʱ�ȭ�Ѵ�. 
	MapX = -1; 
	MapY = -1;
	
	UnderMouseID.Init();
	UnderMouseCropType  = 0;
	UnderMouseCropMount = 0;
	MouseMode           = 0;
	MenuMode            = 0;
	

	// ���������϶� ������ �а� ��´�.
	if (WindowModeSwitch)
	{
		// ��ũ�ѿ� ���콺�� �����Ѵ�. 
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
		// ���콺�� �⺻������ �����Ѵ�. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
		
	}
	else {
		// ��ũ�ѿ� ���콺�� �����Ѵ�. 
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
		// ���콺�� �⺻������ �����Ѵ�. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
	}



   // �ǳ����� ���콺�� ���ٴ� ���� �ʱ�ȭ�Ѵ�. 
   IsMouseInPannelSwitch   = FALSE;
   IsAllyWindowSwitch      = FALSE;
   IsMouseInMapSwitch      = FALSE;
   IsMouseInBigMapSwitch   = FALSE;
   IsMouseInSmallMapSwitch = FALSE;

/////////////////////////////////////////////////////

	// ���� �������� �ִٸ�, 
	if(IsSmallMapValid() && SMap.IsPtInMap(GameMouseX, GameMouseY, mapx, mapy) == TRUE)
	{
	   
		// ��ȿ�� ���� �����̾�� �Ѵ�. 
		if(Map.IsInMapArea(mapx, mapy) == TRUE)
		{
			// ���콺�� ���� �� ������ Ȯ���Ѵ�. 
			IsMouseInMapSwitch       = TRUE;

			// ���콺�� ���� �������� ������ Ȯ���Ѵ�. 
			IsMouseInSmallMapSwitch = TRUE;
			
			// ���콺�� ��ġ�� ������ ��´�. (�⺻��ǥ)
			MapX = mapx;
			MapY = mapy;
		}

	}
	// ���콺�� �ǳ����� �ִٸ�, 
	else if(IsPtInPannel(GameMouseX, GameMouseY) == TRUE)
	{
		IsMouseInPannelSwitch = TRUE;
	}
	else if(IsPtInAllyWindow(GameMouseX, GameMouseY) == TRUE)
	{
		IsAllyWindowSwitch    = TRUE;
	}
	// ���콺�� ū ���� ���� ������ 
	// �׸��� ���콺�� �ٸ� ��Ʈ������ �����ؼ��� �ȵȴ�. 
	else if(Map.IsPtInMap(GameMouseX, GameMouseY) == TRUE
	&& IsMosueInControlSwitch == FALSE)
	{
		// ���콺�� ���� �� ������ Ȯ���Ѵ�. 
		IsMouseInMapSwitch     = TRUE;

		// ���콺�� ū �������� ������ Ȯ���Ѵ�. 
		IsMouseInBigMapSwitch = TRUE;


		// ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
		Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, GameRealX, GameRealY);


		// ������ǥ�� �ٰŷ� �ؼ� �⺻ ��ǥ�� ���´�.
		if(MapEditMode==TRUE)
		{
			SHORT small_map_center_x, small_map_center_y;
		    Map.DecideMapXYByXYLow(GameRealX, GameRealY, MapX, MapY, small_map_center_x, small_map_center_y);
		}
		else
		{
			clifMap.GetRealTileByRealDot(GameRealX, GameRealY, &MapX, &MapY);

			// ���ͺ� ������ ��ȿ�� �׵θ� ������ ���õȰ��̸� ���� ���� ���õǾ� ���� ���� ������ �Ѵ�. 
			if(MapX==0 && MapY==0)
			{
				// �������� �Ÿ��� ���� ������ ����鼭 0,0��ǥ�� ���´ٸ� ��ȿ�ϴ�. 
				DWORD length=max(abs(MapCenterX[0][0]-GameRealX), abs(MapCenterY[0][0]-GameRealY));
				if(length>30)
				{
					MapX = -1;
					MapY = -1;

					// ���콺�� ���� �� ������ Ȯ���Ѵ�. 
		            IsMouseInMapSwitch=FALSE;
		            // ���콺�� ū �������� ������ Ȯ���Ѵ�. 
		            IsMouseInBigMapSwitch=FALSE;

				}
			}
		}

		// ���콺 �Ʒ��� ĳ���͸� �����Ѵ�. 
		UnderMouseID = GetUnderMouseID();

	}

    OperateMenu();

	// ����� �ʿ����� ���� ��    
	if(GD.ObjectNeedSwitch == FALSE)
	{
		// ���콺�� ��ġ�� �������� ���踦 �������� ���콺�� ó�� ��Ȳ�� ���´�.
		// ���콺�� ���� ���� ���� ������ ....
		if(IsMouseInMapSwitch == TRUE)
		   GetMouseMode(MapX, MapY);
	
		// 2��ư �������̽� ��尡 �����Ǿ� �ְ� ������ ��ư�� ������, �ڵ� ����� �����Ѵ�. 
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch == TRUE)
		{
           OperateAutoOrder();
		}

		// �巡���� �Ϸ�Ǹ�, 
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

				  // 1��ư �������̽� ����̰� �ڵ������ �����Ѵٸ�, 
				  if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
				  {
				  }
				  // ȭ���̵����� ���ȴ�. 
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

	if(GD.ObjectNeedSwitch == TRUE)   // object �� �ʿ��ϴ�. 
	{
		SHORT buildkind = 0;

		// ���õ� ��ɿ� ���� ���콺�� �����Ѵ�. 
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
			//����
			SetMouse(MOUSE_HEAL);
		}
		else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_MAGIC) == TRUE)
		{
			SetMouse(MOUSE_MAGIC);
		}
			
		// ���� �ż��̸� 
		if(GD.SelectedOrder==ORDER_MINE)
		{
//			if(Map.IsInMapArea(MapX, MapY)==TRUE)
//			{
//				// ���̾�� �ϰ� ���ڰ� �̹� �ż��� ���̸� �ȵȴ�. 
//			   if(Map.IsEmpty(MapX, MapY)==TRUE && Map.GetMineMap(MapX, MapY) == -1)
//				    GD.CanBuildSwitch=TRUE;
//			   else GD.CanBuildSwitch=FALSE;
//			}
//			else 
//			   GD.CanBuildSwitch=FALSE;

			int a = 0;
		}

		// ��ȿ�� ���� �������� ���콺 ���� ��ư�� ������ �ӽ� �������� �����ȴ�.
		if(MapX!=-1 && MapY!=-1 && (GameLeftPressSwitch==TRUE))
		{
			ObjectTempSelectedSwitch = TRUE;
		}
		// ��ȿ���� ���� �������� �ӽ� �������� �������� �ʴ´�. 
		if(MapX==-1 && MapY==-1)
			ObjectTempSelectedSwitch=FALSE;

		// �ڵ� ��ɿ� ���� ���õ� �������� �ƴϸ�, 
		if(GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
		{
			// 2��ư ����̰�, ������ ��ư�� ���� ��� 
			if(pGame->Config.m_uiInterfaceMode==INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch==TRUE)
			{

				// �������� �ʿ�� ���� �ʴ� ���·� �����. 
	            GD.ObjectNeedSwitch=FALSE;

				// ���õ� ����� ����Ѵ�. 
	            GD.SelectedOrder=0;
				// ��� ���� �ɼ��� �ʱ�ȭ�Ѵ�. 
			    GD.SelectedOrderOption=0;
			}

		}

		// �ӽ� �������� �����Ǹ�, 
		if(ObjectTempSelectedSwitch==TRUE)
		{

			// ���콺 ���� ��ư�� ��������, 
            if(GameLeftPressSwitch==FALSE)
			{

				// �ӽ� �������� ����Ѵ�.
			   ObjectTempSelectedSwitch=FALSE;

		       switch(GD.ObjectNeedType)
			   {
			      case OBJECT1:
				  // ���ڸż��̰�  ���� �� ������obj�� �޾Ƶ����� �ʴ´�.  
					  if(GD.SelectedOrder 
					  && GD.SelectedOrder==ORDER_MINE)
					  {
						  SetMessage2(Text.CannotMakeThereMessage.Get(), UserPlayerNumber, EFFECT_CANNOTMAKETHERE);
					  }
					  // ��Ȯ�̰� 
					  else if(GD.SelectedOrder  && GD.SelectedOrder==ORDER_HARVEST)
					  {
						  if(MouseModeHarvest(MapX, MapY, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE)
						  {
							  // obj�� �����Ѵ�.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MouseHarvestX, MouseHarvestY, UnderMouseID);
						  }
						  else
						  {
							  // obj�� �����Ѵ�.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MapX, MapY, UnderMouseID);

						  }
					  }
					  else
					  {
					     // obj�� �����Ѵ�.  
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

	// ������  ���콺 ��ư�� ���������� ó�� 
	if(IpD.RightPressDoneSwitch==TRUE)
	{
		IpD.RightPressDoneSwitch=FALSE;

		// ���콺�� �޴� ��ư���� ���� �ʾƾ� �ȴ�. 
		if(MenuOfMouse<0)
		{

			// 1��ư �������̽��� �����Ǿ� ������ ������ ��ư�� ���� ��ҷ� �۵��Ѵ�. 
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
				// ���õ� ĳ������ �޴��� ������Ʈ�Ѵ�. 
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

				// ������ ��ư �������� �Է¹��� �����, 
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

			// �̹� ����� �����Ǿ� �ִٸ�, 
			if(GD.ObjectNeedSwitch==FALSE)
			{

				// ���õ� ����� �̵��϶� �̵� ��ũ�� ǥ���ϰ� �ϰ� �̵� ȿ������ ����. 
				if( GD.SelectedOrder == ORDER_MOVE
				||  GD.SelectedOrder == ORDER_PATROL
				|| (GD.SelectedOrder == ORDER_ATTACK && GD.ObjectBuffer[0].ID.IsEmpty()) )	
				{
					// ���õ� ������ �̵� ����� ������ ȿ������ ���� �����. 
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
				// ���õ� ȿ������ �����϶� ���� ȿ������ ����.
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
				// ���õ� ȿ������ ��Ȯ�ϋ� 
			    else if(GD.SelectedOrder==ORDER_HARVEST)
				{
					// ���õ� ������ �����̳� ��Ȯ ����� ������ ȿ������ ���� �����. 
					int effectnumber=0;

					// ������� � ������ Ȯ���Ѵ�. 
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
				// ������ ������ ǥ���Ѵ�. 
				else if(GD.SelectedOrder == ORDER_TRAINSPOT)
				{
					if(GD.ObjectBuffer[0].ID.IsEmpty())
					{
					   SHORT realx, realy;
					   Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, realx, realy);
					   GD.MoveMark.SetMoveMarkReal(realx, realy);

					}
				}
				// �Ǽ� ����̸� 
				// ���� �ż� ����̸�, 
				else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_BUILD)
				|| GD.SelectedOrder==ORDER_MINE)
				{
					PushEffect(EFFECT_DECIDEBUILD, 0, 0);
				}


				// ���õ� ĳ���Ͱ� ������, 
				if(GD.ObjectBuffer[0].ID.IsEmpty()==FALSE)
				{
					if(IsAlive(GD.ObjectBuffer[0].ID.ID)==TRUE)
					{
						// obj�� ���õ� ĳ������ newselectedswitch�� �����Ѵ�.  
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


  	// �÷��̾� ���� ��ġ�� ������ ĳ���͸� ��ġ�Ѵ�.
	SHORT posindex=0;

	for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{
		maxoff = 1;
		
		// ��ȿ���� ���� �÷��̾�� ���ش�. 
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

			//ü���� ���� ĳ���ʹ� ������ �ʴ´�. 
			if(OBInfo.OIBPI[i].Iparameta[index].GetLife() > 0)
			{
				id = FindEmptyID(index);
				
				if(id)
				{
					x = Map.clMapPlayerInfo[posindex].StartPoint.x + SetCharPoint[j].x; 
					y = Map.clMapPlayerInfo[posindex].StartPoint.y + SetCharPoint[j].y;

					if( Map.IsInMapArea(x, y) == TRUE && Map.IsEmpty(x, y))
					{
						// CR[id]�� ĳ���͸� ����Ѵ�.
						SetChar(kind, id, x, y, SOUTH, 100, i, &OBInfo.OIBPI[i].Iparameta[index], &OBInfo.OIBPI[i].MW[index], index);

						OBInfo.OIBPI[i].Iparameta->GetLife();
						OBInfo.OIBPI[i].Iparameta->GetMana();


						
						// ��� �÷��̾��̸� ���� ���� �̸��� ����Ѵ�. 
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
						// NPC�̸� �̹� ����� �̸��� ����Ѵ�. 
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

	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "OnlineBattleMap.dat" );

	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// ù��° 6���� �����Ѵ�. 
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
		// �Ҵ��� �޸𸮸� �����Ѵ�. 
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


// OnlineBattleInfo�� ������ ��ȿ���� �˷��ش�. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo)
{
	SI32 i, j;
	SI32 playerswitch = FALSE;
	SI32 playercount  = 0;

	for(i=0; i<MAX_MULTI_PLAYER; i++)
	{
		// ����ڰ� ���� ���, 
		// ����� ��쿡�� �̸��� �ִٴ� ���� �ٰŷ� üũ.
		if(pobinfo->OIBPI[i].szPlayerName[0])
		{
			playerswitch = TRUE;
		}
		// ������ �����ϴ� ������� 
		if(pobinfo->OIBPI[i].siPlayersUniqueID)
		{
			playercount++;
		}
 
		for(j=0;j<ON_MAX_FOLLOWER_NUM;j++)
		{ 
			// ���ʽ� ������ ���̳ʽ��� ���, 
			if(pobinfo->OIBPI[i].Iparameta[j].GetBonus()  < 0)
			{
				clGrp.Error("Minus Bonus", "Bonus:%d", pobinfo->OIBPI[i].Iparameta[j].GetBonus());
				pobinfo->OIBPI[i].Iparameta[j].SetBonus(0);
				return FALSE;
			}
		}
	}


	// ������ ���ϴ� ����ڰ� ���� �����, 
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
	
	// ������ ������ �÷��̾� ���� 2���� ������, 
	if(playercount < 2)
	{
		clGrp.Error("Not Enough Players", "PlayerCount:%d", playercount);
		return FALSE;
		
	}
	// ������ ������ �÷��̾� ���� 7���� ������, 
	if(playercount > 7)
	{
		clGrp.Error("Too Many Players", "PlayerCount:%d", playercount);
		return FALSE;
	}
	
	return TRUE;
}

// �𰢿� ���ؼ� ����ġ �ս��� ġ���.
void SendBattleMessage_LoseForRetreat()
{

	// ��ŷ ����� ���������� �����Ѵ�. 
	{
		// ���ΰ� ĳ������ ������ ���´�. 
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
//		SetMessage("��ŷ�� ���ɼ��� �ִ� �������Դϴ�.");
//		m_ParmeterBox.IP.SetLife(0);
//	}
	
}

/*
#else

//--------------------------------------------------------------------
//  ��� : ���°�
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


// OnlineBattleInfo�� ������ ��ȿ���� �˷��ش�. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo);

extern _clGame *pGame;


extern	SHORT 				GamePauseSwitch;					// ������ ��� �ߴܵǾ��°�?
extern  _Chat 				Chat;
extern	RECT				ScreenRect;							// ������ ǥ�õǴ� ���� 
extern	_ScreenPoint 		SP;									// ȭ�鿡 ���� ���� ���� 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;
extern	_YSMap 				SMap;								// ���� ������ ������ 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	XSPR 				PannelSpr;
extern	SHORT 				MapEditMode;						// ���� ���� ����ΰ�?
extern	_MapEdit 			MapEdit;
extern	SHORT 				GameMenuShowSwitch;					// �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	DWORD				PrevMultiClock;
extern	DWORD				MessageSendFrame;					// ����̳� NO_MESSAGE�� ���� ������ 
extern	BOOL				GameDropSwitch ;
extern	int 				DragSwitch;
extern	int 				MapDragSwitch;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern	DWORD 				GameRecordSwitch;
extern	clock_t				StartClock, CurrentClock;						// �ð� ���� 
extern	_MyNetGame			MyNet;
extern	SI32				MaxMapXsize;
extern	SI32				MaxMapYsize;
extern	char 				SavePath[];
extern	UI08 				ScreenMapBuffer[];					// ȭ�鿡 ���� ������ �ӽ÷� �����ϴ� �� 
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				MapX, MapY;							// ���콺�� ��ġ�� small ��ǥ 
extern	int 				DragStartX, DragStartY, DragEndX, DragEndY;
extern	_FileVary 			FV;
extern	_Help 				Help;
extern	SHORT				GameStatus;							// ������ ���� ��Ȳ�� �˸��� ���� 
extern	SHORT				GameStatusBuffer;
extern	int					bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
extern _InputDevice			IpD;
extern	int					CDStopSwitch;						// CD������ ���� ��Ų��. 
extern	_Char				CR[];								// ĳ������ ������ 
extern	int 				MouseMode;							// ���콺�� ó�� ���¸� ���� 
// Ű���� ����
extern	int 				ControlKeyPressSwitch;
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameRealX, GameRealY;
extern	SHORT 				UnderMouseCropType;
extern	SHORT 				UnderMouseCropMount;
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	int 				MapDragStartX, MapDragStartY, MapDragEndX, MapDragEndY;
extern	_NewID 				UnderMouseID;
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	DWORD 				GameWParam;
extern	int 				DragDoneSwitch;
extern	int 				MenuMode;							// �޴��� ���¸� ���� 
extern	BOOL 				IsMosueInControlSwitch;				// ���콺�� ���� ��Ʈ������ ��ġ�ϰ� �ִ�.
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_Order 				Order[];
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	SHORT 				MouseHarvestX, MouseHarvestY;
extern	SHORT 				MenuOfMouse;						// ���콺 Ŀ���� ��ġ�ϰ� �ִ� �޴��� ��ȣ 
extern	_NewID 				ZeroID;
extern  char 				UserSelectMapFileName[];			// ����ڰ� ������ ���� �̸��� ������ ����   
extern	int   				MultiPlayDelayTime;
extern  BOOL				bGameAllyWindowSwitch;

extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;			// ĳ���� �׸���
extern LPDIRECTDRAWSURFACE7 lpPannelSurface;		// �ǳڿ� �� 

extern BOOL IsMouseInPannelSwitch  ;
extern BOOL	IsAllyWindowSwitch;

extern	_OnlineBattleMsgQue OBMQue;


extern OnlineBattleInfo OBInfo;
// �ִ� ������ �̰� 
extern SHORT DPMaxFrameDiff;

extern clock_t FrameDelay ;   // �����Ӵ� Ŭ��
extern int FrameClockDelay;

extern SoundManager	*g_pSoundManager;

static DWORD PrevFrameClock;
static DWORD PrevGameWinConditionFrame;
static BOOL IsMouseInPannelSwitch   = FALSE;
static BOOL IsAllyWindowSwitch      = FALSE;
static BOOL ScrollProcSwitch;

int ObjectTempSelectedSwitch=FALSE;
int Scroll;
SHORT ScrollDoneSwitch=FALSE;  // ��ũ���� �̷�����°�?
SHORT DoubleClickedID;  // ���� Ŭ������ ���õ� ���̵� 

BOOL IsMouseInMapSwitch      = FALSE;
BOOL IsMouseInBigMapSwitch   = FALSE;
BOOL IsMouseInSmallMapSwitch = FALSE;

static SI32 BattleMusic	= 0;
OnlineNoticeList *pOnlineNoticeList;

clock_t FrameDelay = 0;   // �����Ӵ� Ŭ��

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

	// �ȷ�Ʈ�� �����Ѵ�. 
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


	// ������ ������ �ߴܵǾ� �־��ٸ� �Ӱ��Ѵ�. 
	GamePauseSwitch=FALSE;
	bGameAllyWindowSwitch = FALSE;

	// ������ �ؽ�Ʈ �Է� ��尡 �����Ǿ� �־��ٸ� �����Ѵ�. 
	Chat.SetInputModeSwitch(FALSE, 0);


	// ������� ���ǽ��� �����Ѵ�. 
	CustomSurfaceRelease();
	
	// ������ ���ǽ��� �ٽ� �����. 
	CreateSurface();


	// ȭ���� ������ �����Ѵ�. 
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


	// ���� ������ �ʱ�ȭ �Ѵ�.
	SMap.Init(Map.GetXsize(), Map.GetYsize());
	
	
	Map.MakeSmallMapBase();

	
	if(GD.GameMode == GAME_MODE_ONLINEBATTLE)
	{
		//���� ������ �׻� ��ȿȭ ��Ų��. 
		GD.SetSmallMapValidSwitch(TRUE);

		// �ڵ� ���� ��� �Ұ�. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// ��� ���׷��̵� �Ϸ��Ѵ�.
		GD.SetUpgradeAllSwitch(TRUE);

		// �¶��� ������ ���ǵ�� �����Ѵ�. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// ĳ���͵��� �þ߸� �������� ó������. 
		GD.SetStraightSightMode(TRUE);

	}
	if(GD.GameMode == GAME_MODE_HERO)
	{
		//���� ������ �׻� ��ȿȭ ��Ų��. 
		GD.SetSmallMapValidSwitch(TRUE);

		// �ڵ� ���� ��� �Ұ�. 
		GD.SetAutoMagicEnableSwitch(TRUE);

		// ��� ���׷��̵� �Ϸ��Ѵ�.
		GD.SetUpgradeAllSwitch(TRUE);

		// �¶��� ������ ���ǵ�� �����Ѵ�. 
		GD.SetOnlineSpeedSwitch(TRUE);

		// ĳ���͵��� �þ߸� �������� ó������. 
		GD.SetStraightSightMode(TRUE);

	}


	LoadPannel(GD.Player[UserPlayerNumber].PI.Country);


	// �ǳ��� surface�� �ű��.
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


	// ������ ���� ���� �⺻���� �ӵ��� ���´�.
	FrameDelay = pGame->Config.GetRealGameSpeedClock();

	// Ű���۸� �ʱ�ȭ�Ͽ� ������ �Է��� �����Ѵ�. 
	GD.KB.InitKeyBuffer();

	// ���� �޴��� ������ �ʵ��� �����. 
	GameMenuShowSwitch=FALSE;

	InitFrameClock();

	InitGetResource();

	PrevGameWinConditionFrame = 10000;
	PrevMultiClock   = 0;
	MessageSendFrame = 0;  // ����̳� NO_MESSAGE�� ���� ������ 

	GameDropSwitch = FALSE;


	// ���� �巡���� �ʱ�ȭ�Ѵ�. 
	DragSwitch = FALSE;
	MapDragSwitch = FALSE;
	GameLeftPressSwitch = FALSE;
	GameRightPressSwitch = FALSE;

	// ���� ���
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
		// ���� ����̸� ��� ȭ���� �� ����. 
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
		//���� ������ �׻� ��ȿȭ ��Ų��. 
		GD.SetSmallMapValidSwitch(TRUE);
	}		
	


	// ���� ���� ���� ����̸� ��� ���׷��̵带 �� �Ϸ��Ų��. 
#ifdef _QUICKMODE
	GD.SetUpgradeAllSwitch(TRUE);
#endif

	PrevMultiClock = CurrentClock;

}


// ���ӿ� ���� ���� �ʱ�ȭ���ִ� �Լ� 
//BOOL InitNoStageGame()
//{
//	SI32 userpointindex = 0;
//
//	// ������ �� �����͸� �ʱ�ȭ�Ѵ�. 
//	GD.Init();
//	InitVolcano();
//
//	// ��Ƽ���ÿ��� ���� ������, 
//   GD.RandomSeed = 1;
//
//	// ���� ����� ���Ѵ�. 
//	GD.GameMode = GAME_MODE_NORMAL;
//
//	Map.InitMap(100, 100, FALSE);
//
//	
//	Map.SetMapDrawSwitch(TRUE);
//
//	// ������ ȭ���� �ҷ��´�. 
//	Map.MH.MapType = MAP_TYPE_NORMAL;
//	LoadMapTile(Map.MH.MapType);
//	
//	// ȭ���� ������ġ�� �ű��. 
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

	// ������ �� �����͸� �ʱ�ȭ�Ѵ�. 
	GD.Init();
	InitVolcano();



/////////////////////////////////////////////////////////////////////////////////////

	// �� ���� ���̵�. 
	OBInfo = GOnlineImjin2I.BattleInfo;
	
	
	UI32 size = 0;
	UI32 startclock = timeGetTime();
	
	
	// ������ �ٰŷ� ���� ��� �������� �����Ѵ�. 
	MyNet.DecideTransVary(300);
	
	// ������ �ʿ��� ������ ���´�.
	
	// �߸��� ������ ������ Ȯ���Ѵ�. 
	if(MapEditMode == FALSE)
	{
		CheckOnlineBattleInfo(&OBInfo);
	}
	
	// �÷��̾��� ��ü���� ������ ���´�. 
	
	
	
	for( i = 0; i < MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.dpId[i] = OBInfo.OIBPI[i].siPlayersUniqueID;
		
		// �����, Ȥ�� �����ΰ��. 
		if(OBInfo.OIBPI[i].siPlayersUniqueID)
		{
			// ����� ��쿡�� �̸��� �ִ�.
			if(ISPLAYER(MyNet.PlayersdpId.dpId[i]) == TRUE)
			{
				
				strncpy(MyNet.PlayersName.Name[i], OBInfo.OIBPI[i].szPlayerName[0], ON_ID_LENGTH);
				
				// �ι��ڸ� �־��ش�. 
				MyNet.PlayersName.Name[i][ON_ID_LENGTH] = NULL;
				
			}
			else
				strcpy(MyNet.PlayersName.Name[i], "MONSTER");
		}
	}

	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		MyNet.PlayersdpId.Player[i].Init(i);

		// ��ȿ�� �÷����̾��� ��� , 
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
			
			// �ڱ� �ڽ��� �ڽſ��� �����̴�. 
			GD.OnlineAllyNumSwitch[i][i] = TRUE;
		}
	}

	// ������ �׻� 1�̴�. 
	MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1]				= 1;
	MyNet.PlayersdpId.Player[MAX_MULTI_PLAYER-1].Country	= COUNTRY_NEUTRAL;
	MyNet.HostPlayerDpId = MyNet.PlayersdpId.dpId[MAX_MULTI_PLAYER-1];;


	MyNet.PlayersdpId.GameMode= GAME_MODE_NORMAL;

	// ������� ��ȣ�� ���Ѵ�. 
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

	// ���� �õ带 ���´�.
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
   // NPC���� �������� �����Ѵ�. ///
   //////////////////////////////
   
   SI32 mon_player = 0;
	for( i=0; i<MAX_MULTI_PLAYER; i++ )
	{
		// ��ȿ�� �÷����̾��� ��� , 
		if(MyNet.PlayersdpId.dpId[i]) 
		{
			// NPC���� ������ �����Ѵ�. 
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

	// �÷��̾� ������  ������ �δ´�. 
	// PK�� ������ �ʴ� �ʵ忡����..(���Ǵ� �ʵ�� �ƹ��ϰ� �������� �ʴ´�.)
	if(pGame->pOnlineWorld && pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
	{
		if(MyNet.PlayersdpId.dpId[0] && MyNet.PlayersdpId.dpId[1])
		{
			// �÷��̾�� ������ ����������(PK)
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
			// ������ ����
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

	// ���� ����� ���Ѵ�. 
	GD.GameMode = GAME_MODE_ONLINEBATTLE;
	
	char mapname[128]="";
	
	// ������ Ÿ���� ���´�. 
	if(MapEditMode == TRUE)
	{
		Map.MH.MapType = MAP_TYPE_NORMAL;
		
		Map.InitMap(100, 100, FALSE);
		
	}
	else
	{
		
		// ���ӿ� �����ϴ� �÷��̾ �� ������ Ȯ���Ѵ�. 
		SHORT playercount=0;
		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			// �������� �ʴ� �����̸�, 
			if(MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NO
				&& MyNet.PlayersdpId.Player[i].Country!=COUNTRY_NEUTRAL)
			{
				playercount++;
			}
		}
		
		// �÷��̾� ���ڰ� ���������̸� �˸���. 
		if(playercount < 2)
		{
			clGrp.Error("f9837521", "[%d]", playercount);
		}
		
		SI32 maptype = pGame->pOnlineWorld->GetWarFieldMap();
		if(FindProperOnlineBattleMap(playercount, maptype, mapname) == TRUE)
		{
			
			// ������ �ʱ�ȭ�Ѵ�.
			//				strcpy(buffer, SavePath);
			strcpy(buffer, "Online\\BattleMap\\");
			strcat(buffer, mapname);
			
			Map.Load(buffer);
		}
		else
		{
			clGrp.Error("125308", "no map [%d]", playercount);
		}
		
		// Ȯ�� ������ �����Ѵ�.
		Map.ExpansionBaseNumber=0;
		for(i = 0;i < MAX_MULTI_PLAYER;i++)
		{
			if(Map.clMapPlayerInfo[i].StartPoint.x >= 0 && Map.clMapPlayerInfo[i].StartPoint.y >= 0)
			{
				Map.PushExpansionBasePoint((SHORT)Map.clMapPlayerInfo[i].StartPoint.x, (SHORT)Map.clMapPlayerInfo[i].StartPoint.y);
			}
		}
		
	}

	//�����ϴ� ��ġ�� �����ϰ� �ٲ۴�.
	Map.RandomStartPos();
	
	// ������ ȭ���� �ҷ��´�. 
	LoadMapTile(Map.MH.MapType);
	Map.SetMapDrawSwitch(TRUE);

    // �ҷ����ų� ���Ƿ� ������ ������ ���������� Ȯ���Ѵ�. 
    Map.MakeLastMap();

	   // ���ÿ��� ������ ������ �Ѱ��ش�. 
	   for(i=0;i<MAX_MULTI_PLAYER;i++)
	   {
		   SHORT pnumber=MyNet.PlayersdpId.Player[i].PlayerNumber;
		   GD.Player[pnumber].PI=MyNet.PlayersdpId.Player[i];
	   }
	   
	   
	   // EnemyBit�� �����Ѵ�. 
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


	 // �߸����̸�, 
	 if(GD.Player[UserPlayerNumber].PI.Country==COUNTRY_NEUTRAL)
	 {
		 // ȭ���� ������ġ�� �ű��. 
		 Map.VirtualMapX=0;
		 Map.VirtualMapY=0;
		 
		 GD.HomeX=Map.VirtualMapX;
		 GD.HomeY=Map.VirtualMapY;
	 }
	 else
	 {
		 
		 // ȭ���� ������ġ�� �ű��. 
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

	 // �������� �ҷ��� ������Ʈ �Ѵ�. 
	 pGame->Config.GetTeamInfo();

#ifdef _OGG_PLAY
	 GetBgmMgr()->Play(OGGBGM_FILENAME_WARTITLE, TRUE);
#else
	 // ������ ������ �����Ѵ�.
	 if(g_pSoundManager)
	 {
		 g_pSoundManager->StopAllSound();
		 
		 BattleMusic	=	g_pSoundManager->AddPlayID( "music\\Music3.YAV", FALSE, TRUE, FALSE );
		 g_pSoundManager->PlaySound( BattleMusic );	
	 }
#endif
	 
 	// ��� �޽����� ó���� �� �ִ� Ŭ������ �����͸� ���´�. 
	pOnlineNoticeList = pGame->GetOnlineNoticeListPointer();

	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->SetPosition(233, 582, 335,4);
	}

	// ���� ���ϸ� ����ġ�� �Ҵ´ٴ� ��� �˷��ش�.
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

	// ORDER_QUIT ����� �ѹ��� ����Ǿ�� �ϱ� ������ �����ʱ⿡ ������ ����ġ�� �ʱ�ȭ�Ѵ�. 
	GameQuitSwitch = FALSE;

	// ������ ȹ�� ����â�� ���ش�. 
	pGame->pItemInfoWindow->ClearAllItemInfoWindow();



	return TRUE;
}

void _clGame::ChatInit()
{
	Chat.Init();
}




BOOL GameDraw1()
{

	//  �̵���ũ���ൿ 
	GD.MoveMark.Action();

	if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

       // ������ �ű��. 
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
		
		// ĳ���͸� ������ ��ü���� �׸���. 
		Map.DrawObj();
		
		
		// ���� ���� �����̸� �÷��̾� ���� ��ġ�� �׷��ش�. 
		if(MapEditMode==TRUE)
		{
			for(i=0;i<MAX_MULTI_PLAYER;i++)
			{
				// ��ȿ�� �÷��̾� ��ġ���, 
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
	   
	   // ���� ���� ����̸�, 
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

	   // �������� ���� �ż����� ���콺 ��ġ�� �˷��ִ� ���� ���� �ٴѴ�.
	   if(GD.SelectedOrder==ORDER_MINE)
	   {
		   SHORT centerx, centery;
		   
		   // ���� ���� ��忡�� ���콺 ��ġ�� ǥ���Ѵ�. 
		   //if(Map.GetCenterXYByMapCenterXY(MapX, MapY, centerx, centery)==TRUE)
		   if(Map.GetRefXY(MapX, MapY, centerx, centery)==TRUE)
		   {
	          Map.DrawMousePos(centerx, centery, MapX, MapY, 0, TRUE);
		   }

	   }

	   // ��� ���� �׸���.
	   GD.Rain.Draw();


	   clGrp.UnlockSurface(lpScreenBuffer);
	}

	

    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
	{

		if(MapDragSwitch==TRUE)
		{
		   Map.DrawDragBox(DragStartX, DragStartY, DragEndX, DragEndY);
		}

	   // �̵� ��ũ ��� 
       GD.MoveMark.DrawMoveMark();

 
	   // �ΰ� �׸��� 
	   Draw_PutSpriteLightT(clGrp.GetScreenXsize()-GetFileXsize(FV.OnlineLogoFile) - 10, 
		   ScreenRect.bottom-GetFileYsize(FV.OnlineLogoFile)-70, 
		   FV.OnlineLogoFile,0);	// hojae_append
	   
	   clGrp.UnlockSurface(lpScreenBuffer);
	}



	   // ������ ȹ�� â�� �����ش�. 
	 pGame->pItemInfoWindow->Render(lpScreenBuffer);



	// �ǳڿ� ���� ������ �׸���. 
	DrawSmallMapImjin();
	
	
if( MapEditMode == FALSE)
{
	//ä��â �׸���.
	pGame->m_ChatList.Process(lpScreenBuffer, 1);
}
	// �ǳ���  �׸��� 
	DrawPannel();

	// �Է����� ������ ǥ�����ش�. 
	Chat.DrawInputText();


	// ������������ ȭ�鿵���� ��Ÿ���� ���� �簢���� �׸���. 
	if(IsSmallMapValid())
	{
	    if(clGrp.LockSurface(lpScreenBuffer)==TRUE)
		{
	       SMap.DrawBox(Map.VirtualMapX, Map.VirtualMapY);
	       clGrp.UnlockSurface(lpScreenBuffer);
		}
	}

	// �޴��� �׷��ش�. 
	DrawMenu(lpScreenBuffer);

	// �޽����� ������ ǥ���Ѵ�. 
	ShowMessage(10, 70, lpScreenBuffer);

	// ���� �ð��� ǥ���Ѵ�. 
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

	// ��� �޽��� ó��. 
	if(pOnlineNoticeList)
	{
		pOnlineNoticeList->Poll(lpScreenBuffer);
	}
	// ===============================================================================
//	if(pGame->pBattle->m_EditControl.IsFocus() == FALSE)
	// ===============================================================================
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	// ===============================================================================
	{
		// ��� �޽����� ����Ѵ�. 
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
	
	// ���콺 ���� � ������ ǥ���Ѵ�. 
	ShowCropInfo(lpScreenBuffer);

	// ���� ���� ��忡���� ������ ǥ���Ѵ�. 
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

	// ���� ������ �����ش�. 
	Help.Draw();



    //////// lpDDSBack�� lpDDSPrimary�� �����Ѵ�.
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
			//����� �� ������ ����϶� ���´�
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


	// ���ǽ��� �ν�Ʈ�� ���̸�, 
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
	// ��ȭ�밡 ������ ���� ������ �׷��ְ�, 
	if(IsSmallMapValid())
	{
		SHORT drawswitch=FALSE;
		
		// ���� ������ ǥ�þȵǴ� ���¿��� ���̸�,
		if(GD.SmallMapDrawSwitch==FALSE)
		{
			// ȿ������ ����. 
			if(GD.SmallMapDrawSwitch==FALSE)
			   PushEffect(EFFECT_MAP1, 0, 0);

			drawswitch=TRUE;
		}
		// ���̴� ���¿����� 30�����Ӹ���..
		else
		{
			if(abs(GD.Frame - GD.DrawSmallMapFrame) >= 30)
			{
				GD.DrawSmallMapFrame = GD.Frame;
				drawswitch=TRUE;
			}
		}

		GD.SmallMapDrawSwitch=TRUE;

		// ���������� ��ΰ� ����Ǿ��ٸ�, 
		if(GD.SmapModeChangedSwitch==TRUE)
		{
			GD.SmapModeChangedSwitch=FALSE;

			drawswitch=TRUE;
		}

		if(drawswitch==TRUE)
		{
		    if(clGrp.LockSurface(lpPannelSurface)==TRUE)
			{
				// �������� ���̴� ����̸�, ������ �׷��ش�. 
	            SMap.Draw(GD.SmapModeFullSwitch);

 	            SMap.DrawChar();

	            clGrp.UnlockSurface(lpPannelSurface);
			}
		}
	}

	// ��ȭ�밡 ������ ���� ������ ���ش�.
	if(IsSmallMapValid() == FALSE && GD.SmallMapDrawSwitch==TRUE)
	{
		// ȿ������ ����. 
		if(GD.SmallMapDrawSwitch==TRUE)
		   PushEffect(EFFECT_MAP2, 0, 0);

		GD.SmallMapDrawSwitch=FALSE;

		// �ǳ��� surface�� �ű��.
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

		// ���ǽ��� �ν�Ʈ�� ���̸�, 
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


// ���� ������ ��ƾ�� ����ִ� �Լ� 
// ���ϰ�: 0 -> normal
SHORT GameProcess()
{
	// ������ ��� ������ �����ð����� ���� ������ �����ش�. (�����ð����� üũ�� �Լ������� ó���Ѵ�.)
	pGame->OnlineSendHeartBeat();

	// ������ ä�� �Է��� �����Ѵ�.
	InputOperateInGame();
	
	
	if(GD.Frame==0)StartClock = CurrentClock;
	

	// ���� �޴��� �����ش�. 
	if(GameMenuShowSwitch==TRUE)
	{
		// ���ӿ��� ���Ӹ޴��� �θ� ������,
		if(GameStatus==GS_GAME)
		{
			// ���� �޴������� ���콺�� �⺻ ���콺�� ����Ѵ�.
			SetMouse(MOUSE_NORMAL);
			
			// �ȷ�Ʈ�� �ʱ�ȭ���ش�. 
			SetGamePalette(clGrp.Palette);
			
			GameStatus=GS_INITGAMEMENU;

			// ���� �����̸Ӹ� ȭ���� lpscreenbuffer�� �ű��. 
			if(lpScreenBuffer)
			{
				if (WindowModeSwitch)
				{
					if( pGame->pOnlineWorld )
						pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);				
					else
					{
						//����� �� ������ ����϶� ���´�
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
					// ���콺�� �����. 
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
		// ȭ���̵����� 
		if(ScrollProcSwitch == FALSE)
		{
			ScrollOperation();
			SetScrollProcSwitch(TRUE);
		}
		
		// ġƮŰ�� �� ����� �ο��Ѵ�. 
//		Cheat.CheatOrder();
	}
	// ���� Ÿ���� �׸���. 
	Map.DrawMap();

	if(GamePauseSwitch==FALSE)
	{
		// ����� ����� ó���Ѵ�. 
		PopOrder();
	}

	// ������ ������Ų��. 
	if(GameDelay() == FALSE)return 0;



	// �ʱ�޴��� ���ư��� ī��Ʈ �ٿ�. 
	if(GD.InitInitMenuCount)
	{
		GD.InitInitMenuCount--;

		if(GD.InitInitMenuCount == 0)
		{
			OperateGameMenuInitInitMenu();
			return 0;
		}

	}

	// ������ �����ϴ� ī��Ʈ �ٿ�. 
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

	// �� 1�д� ���� ������ ��Ʈ��Ʈ �޽����� ������. 
	if(GD.Frame && (GD.Frame % (60000/DEFAULT_MULTI_PLAY_DELAY_TIME) == 0))
	{
		OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(MyNet.PlayersdpId.dpId[UserPlayerNumber ]);
	}

	if(GD.Frame >= 20)
	{
		if(GD.Frame == 20)SetStartFrameClock(CurrentClock);
		else
		{
			// ����� �ð��� ����Ѵ�. 
			PushFrameClock(GD.Frame, CurrentClock);
		}
	}

	GameAction();


	// �����찡 Ȱ��ȭ �Ǿ� �ִ� ��쿡�� ���콺�� �׸���. 
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
	
	// �÷��� �ð��� �����Ѵ�. 
	if(abs(GD.TempAccumulatedPlayClock - CurrentClock) >= 1000)
	{
		GD.TempAccumulatedPlayClock = CurrentClock;
		GD.AccumulatedPlaySecond ++;
	}

	
	return 0;
}


// ������ �ӵ��� �����ϰ� �����. 
BOOL GameDelay()
{

	DWORD basicdelay;
	
	// ������ ���� ���� �⺻���� �ӵ��� ���´�.
	basicdelay = pGame->Config.GetRealGameSpeedClock();

	FrameDelay = basicdelay + FrameClockDelay;
	FrameClockDelay = 0;


	clock_t tempclock = CurrentClock;

	// ������ �����ߴٸ�, 
	if( abs(CurrentClock - PrevFrameClock) < FrameDelay)
	{
		// �������Ѿ� �� �ð� ,
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
		
		// ���� �����Ǿ���� �� �ð� (������ ������ ����� �ð�)
		tempclock = (CurrentClock - overclock);

	}


	PrevFrameClock = tempclock;

	return TRUE;
}


BOOL GameWinCondition()
{

	// �������� �޶����߸�  üũ�Ѵ�. 
	if(GD.Frame == PrevGameWinConditionFrame)return FALSE;
	PrevGameWinConditionFrame = GD.Frame;

	SHORT status = WinCondition();


	//�̱�ų� ���� ������ ���� ������ �Ѵ�. 
	if((GD.VictoryDelay && (abs(CurrentClock - GD.VictoryDelay) > VICTORY_HALFDELAY))
	|| (GD.DefeatDelay  && (abs(CurrentClock - GD.DefeatDelay)  > DEFEAT_HALFDELAY)))
	{
		if(GameQuitSwitch == FALSE)
		{
			GameQuitSwitch = TRUE;
			MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);
		}
	}

	// �¸��ߴ�. 
	if(status == 1)
	{
		// �¸��Ѱ����� ����Ѵ�. 
		GD.WinSwitch = TRUE;

		GameStatus   = GS_INITWIN;

    	EndGame();

//		pGame->pOnlineWorld->pOnlineClient->SendReturnOnline(TRUE);
		return TRUE;
	}
	// �й� �ߴ�. 
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
// Desc: ������ �����ϴµ� �ʿ��� ���� 
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

	// ����� ��ȣ�� �ʱ�ȭ�Ѵ�. 
	UserPlayerNumber=0;

	Map.FreeMap();
	FreeMapTile();

	// ���콺 Ŀ���� �⺻������ ��ü�Ѵ�.
	SetMouse( MOUSE_NORMAL );


	// �ȷ�Ʈ�� ���� �ȷ�Ʈ�� �ʱ�ȭ�Ѵ�. 
	SetGamePalette(clGrp.Palette);

	// ĳ���͸� ��� ���ش�. 
	DeleteChar();

	// �Ѿ��� ��� ���ش�. 
	DeleteBullet();
	// ���⸦  ��� ���ش�. 
	DeleteSmoke();
	
	// �������� ��� ���ش�. 
	DeleteMagic();

	// ���� �߿� �ε��� �̹��� ������ �޸𸮸� �����Ѵ�.
	FreeImgFile();



	if(pGame)
	{
		if(pGame->pBattle)
			pGame->pBattle->InitGameButton();
	}
	
	// ��ü ȭ���� Ŭ���� �������� �����Ѵ�.
	clGrp.SetClipFullScreen();
}

SHORT GameAction()
{
	SHORT i;
	GD.OldRand=GD.Rand;
    GD.Rand = GD.Random();
	SHORT id = 0;


	// ���� �������� ��ŷ ���ɼ��� Ȯ���Ѵ�. 
	if(GD.Frame % 10 == 0)
	{
		// kindinfo���� ��ŷ ���θ� �˻��Ѵ�. 
		if(CheckKindInfo() == FALSE)
		{
			// ������ ��ŷ ����� �ִ� ����ڶ�� ����� �˸���. 
			// ������ �����Ų��. 
			GameStatus = GS_INITQUIT;
			
			// ��ŷ���� ���� ������ ������� �˸���. 
			//clGrp.Error("Game Quit", "There is possibility of Hacking. Your account will be blocked. Please contact us.");
		}
	}


	if(GamePauseSwitch==FALSE)
	{
	    // �ӽ÷� 
	    GD.MoveCharNumber=0;

		// ������ ȹ�� â ���� �ൿ�� �Ѵ�. 
		pGame->pItemInfoWindow->Prepare();

		// ���� ���� ��尡 �ƴϾ�� �Ѵ�. 
	    if(MapEditMode==FALSE)
		{

			// �ð��� ó���Ѵ�. 
			GD.Time.PassTime();


			// ���� �ൿ 
			GD.Rain.Action();

			VolcanoAction();
		}

		// ĳ������ �ൿ 
		GD.SyncSignal  = 0;
		int charindex;
	    for(charindex = 0;charindex < GD.CharOrderIndex;charindex++)
		{
			id = GD.CharOrder[charindex] ;
			if(GD.CharInfo[ id ])
			{
				// ���� ��ȣ�� ���´�. 
				GD.SyncSignal = GD.SyncSignal + id + clifChar.GetX(id) + clifChar.GetY(id) + clifChar.GetCurrentOrder(id) 
					+ clifChar.GetWorkStep(id) + clifChar.GetDX(id) + clifChar.GetDY(id) + clifChar.GetPara1(id) 
					+ clifChar.GetDpower(id) + clifChar.GetLife(id) + clifChar.GetMana(id);


				DWriteStringToFile(" id = %d , life =  %d",id,clifChar.GetLife(id));
				if(clifChar.Action(id)==FALSE)
				{
					DWriteStringToFile("�׾���.id = %d",id);
					DeleteChar(id);
				}
			}
		}

		{

			// ���� Ž���� �ǽ��Ѵ�.
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

		// �������� �ൿ. 
		MagicAction();

		// �Ѿ˵��� �ൿ 
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

		// ������� �ൿ  
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

		// ���� �ð��� �����Ѵ�.
		PassPlayTime();

		// ĳ���͵���  �Ȱ� ó�� 
		// ������ ���� ������ ��Ӱ� ����� ���ο� ������ ��� �����. 
		// ����.
		if(pGame->pOnlineWorld)
		{
			if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,FALSE);
				// ������ ���� ������ ���ΰ� ���ο� ������ ������. 
				else
					GD.BlackCheck(FALSE,FALSE);
			}
			else
			{
				if(GD.Frame % 6 == 0)
					GD.BlackCheck(TRUE,TRUE);
				// ������ ���� ������ ���ΰ� ���ο� ������ ������. 
				else
					GD.BlackCheck(FALSE,TRUE);
				
			}
		}
   }
   
   // �����̽� �ٸ� ���� ȭ�� �̵� ��Ų��. 
   if(GD.ScreenMoveSwitch==TRUE)
   {
       Map.VirtualMapX=GD.ScreenMoveX;
       Map.VirtualMapY=GD.ScreenMoveY;
       Map.DrawForceSwitch=TRUE;
	   
	   GD.ScreenMoveSwitch=FALSE;
   }
   
	return 0;
}


// ���콺 ��ư�� ���� �ڵ� ����� ó���Ѵ�. 
BOOL OperateAutoOrder()
{

	// ���� �ڵ� ����� �ƴ� ������� ���õ� ����� �ִٸ� �ڵ� ����� �����Ѵ�. 
	if(GD.SelectedOrder!=0 && GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
	{
	   return FALSE;
	}

	// �޴��� ��Ȯ�� �ְ� ���콺�� � ���� ������, 
	if(MouseMode&MOUSEMODE_HARVEST)
    {
		GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_HARVEST;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� �̵��� �ְ� ���콺�� ���ڸ��� ������, 
    else if(MouseMode&MOUSEMODE_MOVE)
    {
 	    GD.ObjectNeedSwitch=TRUE;
		ObjectTempSelectedSwitch=TRUE;
   		GD.SelectedOrder=ORDER_MOVE;
		GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
		GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� ������ �ְ� ���콺�� �� ���� ������, 
    else if(MouseMode&MOUSEMODE_ATTACK)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_ATTACK;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� ž���� ������
    else if(MouseMode&MOUSEMODE_RIDE)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_RIDE;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
    // �޴��� ġ�ᰡ �ְ� 
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
    // �޴��� ����� ������ �ִ� ���� �ְ�. 
    else if(MouseMode&MOUSEMODE_GOINTOHQ)
    {
	   GD.ObjectNeedSwitch=TRUE;
	   ObjectTempSelectedSwitch=TRUE;
	   GD.SelectedOrder=ORDER_GOINTOHQ;
	   GD.SelectedOrderOption=SELECTEDORDEROPTION_AUTO;
	   GD.ObjectNeedType=OBJECT1;
    }
	// �޴��� ��ȭ�� �ְ� ���콺�� �� ���� ������, 
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
	// ��ũ�� ������ �ʱ�ȭ�Ѵ�. 
	Scroll=0;


	

	//��ŵ�ϴ� �����ӿ����� ��ũ���� �Ѵ�. 
	// ������ ����϶��� ������ �а� ��´�.
	if (WindowModeSwitch) {
		if(DragSwitch==FALSE )
		{
			if(abs(GameMouseX) <= SCROLL_RANGE )               Scroll|=WEST;
			if(abs(GameMouseX - (clGrp.GetScreenXsize()-1)) <= SCROLL_RANGE ) Scroll|=EAST;
			if(abs(GameMouseY) <= SCROLL_RANGE)                Scroll|=NORTH;
			if(abs(GameMouseY - (clGrp.GetScreenYsize()-1)) <= SCROLL_RANGE ) Scroll|=SOUTH;
			
			// Ű���忡 ���� ��ũ���� �����Ѵ�. 
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
			
			// Ű���忡 ���� ��ũ���� �����Ѵ�. 
			if(GetAsyncKeyState(VK_UP)   < -1)	   Scroll |= NORTH;
			if(GetAsyncKeyState(VK_DOWN) < -1)	   Scroll |= SOUTH;
			if(GetAsyncKeyState(VK_LEFT) < -1)     Scroll |= WEST;
			if(GetAsyncKeyState(VK_RIGHT)< -1)	   Scroll |= EAST;
		}
		
		
	}


	// ��ũ�� ���̸� ������ �׸���. 
	if(Scroll)
	{
		Map.SetMapDrawSwitch(TRUE);
	}

	// ��ũ�� ������ �ӽ÷� �����Ѵ�. 
	SHORT tempvirtualmapx=Map.VirtualMapX;
	SHORT tempvirtualmapy=Map.VirtualMapY;

	// ���� ��ũ�� 
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
	// ���� ��ũ�� 
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

	// ���� ��ũ�� 
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
	// ���� ��ũ�� 
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


	// ������ ��ũ���� �̷�� ������ ���̷�� ������ Ȯ���Ѵ�. 
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

	//altŰ ���� ���� Ȯ��        
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
	
	// �׷� ���� 
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

	// ���콺�� �������� �ִ� ���� �ƴ϶�� , 
	if(Map.IsInMapArea(MapX, MapY) == FALSE) return newid;


	// ���콺 �ؿ� �ִ� ĳ���͸� ���Ѵ�. 
	if(GD.BlackMap[UserPlayerNumber][MapX][MapY] != FOG2_BLACK_STEP)
	{
			
		// ���� ĳ���͸� ���� �����Ѵ�. 
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

		// ���߿��� ĳ���͸� ã���� �����Ѵ�. 
		if(newid.IsEmpty() == FALSE) return newid;


		// ���󿡼� ĳ���͸� ã�´�. 
		length     = 0;
		min_length = 10000;
		
		
		for(SHORT range = 0 ; range < 3 ; range++)
		{
			for(i=MapY - range;i <= MapY + range; i++)
				for(j=MapX - range;j <= MapX + range; j++)
				{
					if(i==(MapY-range) || i== (MapY+range) || j==(MapX-range) || j==(MapX+range)){}
					else continue;
					
					// ������ �������� �����ϴ��� �˻��Ѵ�. 
					if(Map.IsInMapArea(j, i) == FALSE) continue;
					
					// ������ ��ο� �������� ���� �Ұ� 
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
			
		// ���������� ���콺 �ؿ� ĳ���Ͱ� ���� ���¶��, 
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


// �巡�װ� ����Ǿ������� ó���̴�.
void DragOperation()
{
	SI32 i;
	SHORT id;
    SHORT realx1, realy1, realx2, realy2;

	// ���õ� ������ ������ �ӽ� ���۸� ����� �ʱ�ȭ�Ѵ�.
	_NewID temp_id[MAX_SELECTED_CHAR_NUMBER];
	SHORT index=0;
	SHORT newselectedid[MAX_SELECTED_CHAR_NUMBER];
	SHORT newselectednumber = 0;
	
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		temp_id[i].Init();
		newselectedid[i] = 0;
	}
	
	// �巡���� �ʱ�ȭ�Ѵ�. 
    MapDragSwitch = FALSE;

	
	// ���� ������ ���۰� ���� �����Ѵ�. 
	int min_x = min(MapDragStartX, MapX);
	int max_x = max(MapDragStartX, MapX);
	int min_y = min(MapDragStartY, MapY);
	int max_y = max(MapDragStartY, MapY);
	
	MapDragStartX = min_x;
	MapDragStartY = min_y;
	MapDragEndX   = max_x;
	MapDragEndY   = max_y;

	
	// ���콺�� �Ѱ����� Ŭ�������� 
	if(abs(MapDragStartX-MapDragEndX)<=1 &&  abs(MapDragStartY-MapDragEndY)<=1)
	{
		
		// 1��ư �������̽��̸� �ڵ� ��� ��ƾ�� �����ϰ� FALSE�� �����ϸ� ĳ���� �������� �����ȴ�. 
		// 2��ư �������̽��̸� ĳ���� �������� �����ȴ�. 
		if(pGame->Config.InterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
		{
		}
		else
		{
			// ���콺 �ؿ� ĳ���Ͱ� �ִٸ�, 
			if(UnderMouseID.IsEmpty() == FALSE)
			{
				// shiftŰ�� ���� �����̰� 
				// ���� ���� ������ ���̸�, 
				if(ShiftKeyPressSwitch  == TRUE 
				&& clifChar.GetPlayerNumber(UnderMouseID.ID) == UserPlayerNumber)
				{
					// ������ ���õǾ��� ĳ���͸� ���ۿ� �ִ´�. 
					
							index = 0;

					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// ���õ� ĳ���͸� �ִ´�. 
						if(GD.SelectedID[i].IsEmpty() == FALSE)
						{
							temp_id[index] = GD.SelectedID[i];
							index++;
						}
					}

					
					// ���콺 �Ʒ��� ĳ���Ͱ� �̹� ���õǾ� �ִ� ĳ���Ͱ� �ƴ��� Ȯ���Ѵ�. 
					SHORT alreadyselectedswitch=FALSE;
					SHORT alreadyselectedindex=0;
					BOOL invalidswitch = FALSE;
					
					for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
					{
						// �̹� ���õ� ������ �����̰ų� ȥ�ڼ��� ���� �����̸�, 
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
					
					// ������ ���õ� ĳ���Ͱ� ������ ���õ� ĳ���Ͷ��, 
					if(alreadyselectedswitch == TRUE)
					{
						// ���õ� ����Ʈ���� �����Ѵ�. 
						clifChar.SetSelect(temp_id[alreadyselectedindex].ID, FALSE, FALSE);
						temp_id[alreadyselectedindex].Init();

					}
					// ������ ���õ� ĳ���Ͱ� ���õǾ����� ���� ĳ���Ͷ��, 
					else
					{
						// �̹� ���õ� ĳ���Ͱ� �ִ� ��Ȳ���� �ǹ���� ���� �Ѱ��� ���� ������ �����̸�, 
						// �ű� ������ ���� �ʴ´�.
						if(invalidswitch == TRUE)
						{
						}
						// �������� ���õǾ ������ ĳ������ ��쿡�� �߰��Ѵ�. 
						else
						{
							// ���� ���õ� ĳ������ ���� �ʱ�ȭ�Ѵ�. 
							newselectednumber = 0;

							// ���ڸ��� ���� �߰��� �ִ´�. 
							for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
							{
								// ���ڸ����� �ѹ��� �ִ´�.
								if(temp_id[i].IsEmpty() && newselectednumber == 0)
								{
									temp_id[i] = UnderMouseID;
									clifChar.SetSelect(UnderMouseID.ID, TRUE, FALSE);
									
									// �űԷ� ���õ� ������ �����Ѵ�.
									newselectedid[index] = UnderMouseID.ID;
									newselectednumber++;
									
									index++;
								}
							}
						}
						
						
					}
					
				}
				// ����Ʈ���� ������ �ʴ� �׹��� �Ϲ����� ������ ��� 
				else
				{
					temp_id[0] = UnderMouseID;
					index      = 1;
					
					
					// �űԷ� ���õ� ������ �����Ѵ�.
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
	// ���콺�� 2ĭ �̻��� �������� �������� �巡���� ������ 
	else  
	{
		
		Map.TransFromScreenToRealXY(DragStartX, DragStartY, realx1, realy1);
		Map.TransFromScreenToRealXY(DragEndX,   DragEndY,   realx2, realy2);
		
		// shiftŰ�� ���� ���¶��, 
		SHORT newselectstartindex = 0;
		
		if(ShiftKeyPressSwitch  == TRUE)
		{
			
			// ������ ���õ� ĳ���͸� �ӽ� ���ۿ� �ִ´�. 
			index = 0;
			BOOL invalidswitch = FALSE;
			
			for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
			{
				if(GD.SelectedID[i].IsEmpty() == FALSE)
				{
					temp_id[index] = GD.SelectedID[i];
					index++;
					newselectstartindex = index;

					// �̹� ���õ� ������ �����̰ų� ȥ�ڼ��� ���� �����̸�, 
					if(clifChar.GetPlayerNumber(GD.SelectedID[i].ID) != UserPlayerNumber)
					{
						invalidswitch = TRUE;
					}

				}
			}
			
			if(invalidswitch == FALSE)
			{
				// ���� �ȿ� �ִ� ĳ���͸� ���ؼ� �ӽ� ���ۿ� �߰��Ѵ�.
				SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
				
				
				// �űԷ� ���õ� ������ �����Ѵ�.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
			}
		}
		else
		{
			// ���� �ȿ� �ִ� ĳ���͸� ���ؼ� �ӽ� ���۸� ä���.
			SelectCharInDrag(temp_id, index, realx1, realy1, realx2, realy2, 0);
			
			// �űԷ� ���õ� ������ �����Ѵ�.
			for(i = 0;i < index;i++)
			{
				newselectedid[i] = temp_id[i].ID;
				newselectednumber++;
			}
		}
	}
	
	
	// ���õ� ������ �ִٸ�, 
	if(index > 0)
	{
		
		// ������ ĳ���Ͱ� �ϳ��̰� 
		// ª�� �ð��� ���������� ���õ� ���̶��, 
		// ȭ�� �ȿ� �ִ� ���� ������ ĳ���͵��� ���Ѵ�. 
		if(DoubleClickedID)
		{
			
			// ���� Ŭ���� ������ �Ʊ��̸� �߰��� �����Ѵ�.
			if(clifChar.GetPlayerNumber(DoubleClickedID) == UserPlayerNumber)
			{
				
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, realx1, realy1);
				Map.TransFromScreenToRealXY((SHORT)ScreenRect.right,(SHORT)ScreenRect.bottom,   realx2, realy2);
				
				SHORT newselectstartindex = index;

				//  ȭ���� ���� �ȿ� �ִ� ĳ���͸� �ӽù��ۿ� �ִ´�.
				SelectCharInDrag(temp_id, index, realx1, realy1, 
					realx2, realy2, DoubleClickedID);
				
				
				// �űԷ� ���õ� ������ �����Ѵ�.
				for(i = newselectstartindex;i < index;i++)
				{
					newselectedid[i-newselectstartindex] = temp_id[i].ID;
					newselectednumber++;
				}
				
				DoubleClickedID = 0;
				
			}
		}
		
		// ���õ� ������ ȿ������ ����. 
		SHORT effectnumber=0;
		for(i=0;i<newselectednumber;i++)
		{
			id=newselectedid[(i+GD.Rand)%newselectednumber];
			// ���õ� ĳ������ 1�������� ȿ������ ����. 
			
			if(effectnumber==0 && KI[ clifChar.GetKind(id) ].SelectEffect)
			{
				clifChar.SelectOperation(id);
				effectnumber++;
			}
		}
		
		//������ ���õ� ĳ���͸� ����Ѵ�. 
		Unselect();
		ClearMenu();
		
		// ������ �����Ѵ�.
		for(i=0;i<index;i++)
		{
			id=temp_id[i].ID;
			clifChar.SetSelect(id, TRUE, FALSE);
		}
		
		
	}
	// ���õ� ������ ���ٸ�, 
	else
	{
		if(pGame->Config.InterfaceMode == INTERFACEMODE_2BUTTON)
		{
			//������ ���õ� ĳ���͸� ����Ѵ�. 
			Unselect();
			ClearMenu();
		}
	}
}



// Ű���� ó���� ������ ��ƾ 
BOOL GameOverHead()
{
	int i;
	SHORT id;
	DWORD wparam;
	SHORT mapx, mapy;

	SHORT leftpressswitch=FALSE, rightpressswitch=FALSE;
	SHORT mousex, mousey;
	char moveswitch;




	// �ӽ÷� ����Ѵ�. 
	if(MapEditMode == TRUE)
	{
		MapEdit.Action();
	}

	
	// �¸� ������ üũ�Ѵ�. 
	// �¸� �߰ų� �й� ������ �����Ѵ�. 
	if(GameWinCondition() == TRUE)
	{
		return TRUE;
	}
	
	// ������ �ߴܵ� ���¸� �����Ѵ�.
	if(GamePauseSwitch==TRUE)return FALSE;


	//////////////////////////////////////////////////////////
	// control, alt ���� Ư��Ű�� ���¸� Ȯ���Ѵ�. 
	//////////////////////////////////////////////////////////
	


    // Ű���� ������ ���� �׷� ���� �� ���� 

	if( !Handler_OnlineEditControlEx::IsFocus( pGame->pBattle->m_hOecChat ) )
	{
		SystemKeyCheck();

		GroupInterface();
	}

	// ������� ���콺 , Ű���� ���� �Է� ���¸� Ȯ���Ѵ�. 
	GameWParam=0;

	if(GD.KB.PopKeyBuffer(leftpressswitch, rightpressswitch, mousex, mousey, wparam, moveswitch) == TRUE)
	{

		GameMouseX = mousex;
		GameMouseY = mousey;
    	GameWParam = wparam;
		
		// ���� ���콺 ��ư ���� 
		if(leftpressswitch >= 0)
		{

			// ���� ��ư�� ó�� �������̶��, 
	        if(leftpressswitch == TRUE)
			{
		      DragSwitch=TRUE;
	   	      DragStartX=GameMouseX;
		      DragEndX=DragStartX;
		      DragStartY=GameMouseY;
		      DragEndY=DragStartY;
			}
	        // ���� ��ư�� ���� ���¿��� Ǯ�� ���̶��, 
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
		    // ���콺�� �̵��Ǿ��°�?
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
	
	

////////////////// �ʱ�ȭ //////////////////////////

	// ���콺�� ��ġ�� small ��ǥ�� �ʱ�ȭ�Ѵ�. 
	MapX = -1; 
	MapY = -1;
	
	UnderMouseID.Init();
	UnderMouseCropType  = 0;
	UnderMouseCropMount = 0;
	MouseMode           = 0;
	MenuMode            = 0;
	

	// ���������϶� ������ �а� ��´�.
	if (WindowModeSwitch)
	{
		// ��ũ�ѿ� ���콺�� �����Ѵ�. 
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
		// ���콺�� �⺻������ �����Ѵ�. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
		
	}
	else {
		// ��ũ�ѿ� ���콺�� �����Ѵ�. 
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
		// ���콺�� �⺻������ �����Ѵ�. 
		else
		{
			SetMouse(MOUSE_NORMAL);
		}
	}



   // �ǳ����� ���콺�� ���ٴ� ���� �ʱ�ȭ�Ѵ�. 
   IsMouseInPannelSwitch   = FALSE;
   IsAllyWindowSwitch      = FALSE;
   IsMouseInMapSwitch      = FALSE;
   IsMouseInBigMapSwitch   = FALSE;
   IsMouseInSmallMapSwitch = FALSE;

/////////////////////////////////////////////////////

	// ���� �������� �ִٸ�, 
	if(IsSmallMapValid() && SMap.IsPtInMap(GameMouseX, GameMouseY, mapx, mapy) == TRUE)
	{
	   
		// ��ȿ�� ���� �����̾�� �Ѵ�. 
		if(Map.IsInMapArea(mapx, mapy) == TRUE)
		{
			// ���콺�� ���� �� ������ Ȯ���Ѵ�. 
			IsMouseInMapSwitch       = TRUE;

			// ���콺�� ���� �������� ������ Ȯ���Ѵ�. 
			IsMouseInSmallMapSwitch = TRUE;
			
			// ���콺�� ��ġ�� ������ ��´�. (�⺻��ǥ)
			MapX = mapx;
			MapY = mapy;
		}

	}
	// ���콺�� �ǳ����� �ִٸ�, 
	else if(IsPtInPannel(GameMouseX, GameMouseY) == TRUE)
	{
		IsMouseInPannelSwitch = TRUE;
	}
	else if(IsPtInAllyWindow(GameMouseX, GameMouseY) == TRUE)
	{
		IsAllyWindowSwitch    = TRUE;
	}
	// ���콺�� ū ���� ���� ������ 
	// �׸��� ���콺�� �ٸ� ��Ʈ������ �����ؼ��� �ȵȴ�. 
	else if(Map.IsPtInMap(GameMouseX, GameMouseY) == TRUE
	&& IsMosueInControlSwitch == FALSE)
	{
		// ���콺�� ���� �� ������ Ȯ���Ѵ�. 
		IsMouseInMapSwitch     = TRUE;

		// ���콺�� ū �������� ������ Ȯ���Ѵ�. 
		IsMouseInBigMapSwitch = TRUE;


		// ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
		Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, GameRealX, GameRealY);


		// ������ǥ�� �ٰŷ� �ؼ� �⺻ ��ǥ�� ���´�.
		if(MapEditMode==TRUE)
		{
			SHORT small_map_center_x, small_map_center_y;
		    Map.DecideMapXYByXYLow(GameRealX, GameRealY, MapX, MapY, small_map_center_x, small_map_center_y);
		}
		else
		{
			clifMap.GetRealTileByRealDot(GameRealX, GameRealY, &MapX, &MapY);

			// ���ͺ� ������ ��ȿ�� �׵θ� ������ ���õȰ��̸� ���� ���� ���õǾ� ���� ���� ������ �Ѵ�. 
			if(MapX==0 && MapY==0)
			{
				// �������� �Ÿ��� ���� ������ ����鼭 0,0��ǥ�� ���´ٸ� ��ȿ�ϴ�. 
				DWORD length=max(abs(MapCenterX[0][0]-GameRealX), abs(MapCenterY[0][0]-GameRealY));
				if(length>30)
				{
					MapX = -1;
					MapY = -1;

					// ���콺�� ���� �� ������ Ȯ���Ѵ�. 
		            IsMouseInMapSwitch=FALSE;
		            // ���콺�� ū �������� ������ Ȯ���Ѵ�. 
		            IsMouseInBigMapSwitch=FALSE;

				}
			}
		}

		// ���콺 �Ʒ��� ĳ���͸� �����Ѵ�. 
		UnderMouseID = GetUnderMouseID();

	}

    OperateMenu();

	// ����� �ʿ����� ���� ��    
	if(GD.ObjectNeedSwitch == FALSE)
	{
		// ���콺�� ��ġ�� �������� ���踦 �������� ���콺�� ó�� ��Ȳ�� ���´�.
		// ���콺�� ���� ���� ���� ������ ....
		if(IsMouseInMapSwitch == TRUE)
		   GetMouseMode(MapX, MapY);
	
		// 2��ư �������̽� ��尡 �����Ǿ� �ְ� ������ ��ư�� ������, �ڵ� ����� �����Ѵ�. 
		if(pGame->Config.InterfaceMode == INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch == TRUE)
		{
           OperateAutoOrder();
		}

		// �巡���� �Ϸ�Ǹ�, 
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

				  // 1��ư �������̽� ����̰� �ڵ������ �����Ѵٸ�, 
				  if(pGame->Config.InterfaceMode == INTERFACEMODE_1BUTTON && OperateAutoOrder() == TRUE)
				  {
				  }
				  // ȭ���̵����� ���ȴ�. 
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

	if(GD.ObjectNeedSwitch == TRUE)   // object �� �ʿ��ϴ�. 
	{
		SHORT buildkind = 0;

		// ���õ� ��ɿ� ���� ���콺�� �����Ѵ�. 
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
			//����
			SetMouse(MOUSE_HEAL);
		}
		else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_MAGIC) == TRUE)
		{
			SetMouse(MOUSE_MAGIC);
		}
			
		// ���� �ż��̸� 
		if(GD.SelectedOrder==ORDER_MINE)
		{
//			if(Map.IsInMapArea(MapX, MapY)==TRUE)
//			{
//				// ���̾�� �ϰ� ���ڰ� �̹� �ż��� ���̸� �ȵȴ�. 
//			   if(Map.IsEmpty(MapX, MapY)==TRUE && Map.GetMineMap(MapX, MapY) == -1)
//				    GD.CanBuildSwitch=TRUE;
//			   else GD.CanBuildSwitch=FALSE;
//			}
//			else 
//			   GD.CanBuildSwitch=FALSE;
			
			int a = 0;
		}

		// ��ȿ�� ���� �������� ���콺 ���� ��ư�� ������ �ӽ� �������� �����ȴ�.
		if(MapX!=-1 && MapY!=-1 && (GameLeftPressSwitch==TRUE))
		{
			ObjectTempSelectedSwitch = TRUE;
		}
		// ��ȿ���� ���� �������� �ӽ� �������� �������� �ʴ´�. 
		if(MapX==-1 && MapY==-1)
			ObjectTempSelectedSwitch=FALSE;

		// �ڵ� ��ɿ� ���� ���õ� �������� �ƴϸ�, 
		if(GD.SelectedOrderOption != SELECTEDORDEROPTION_AUTO)
		{
			// 2��ư ����̰�, ������ ��ư�� ���� ��� 
			if(pGame->Config.InterfaceMode==INTERFACEMODE_2BUTTON && IpD.RightPressDoneSwitch==TRUE)
			{

				// �������� �ʿ�� ���� �ʴ� ���·� �����. 
	            GD.ObjectNeedSwitch=FALSE;

				// ���õ� ����� ����Ѵ�. 
	            GD.SelectedOrder=0;
				// ��� ���� �ɼ��� �ʱ�ȭ�Ѵ�. 
			    GD.SelectedOrderOption=0;
			}

		}

		// �ӽ� �������� �����Ǹ�, 
		if(ObjectTempSelectedSwitch==TRUE)
		{

			// ���콺 ���� ��ư�� ��������, 
            if(GameLeftPressSwitch==FALSE)
			{

				// �ӽ� �������� ����Ѵ�.
			   ObjectTempSelectedSwitch=FALSE;

		       switch(GD.ObjectNeedType)
			   {
			      case OBJECT1:
				  // ���ڸż��̰�  ���� �� ������obj�� �޾Ƶ����� �ʴ´�.  
					  if(GD.SelectedOrder 
					  && GD.SelectedOrder==ORDER_MINE)
					  {
						  SetMessage2(Text.CannotMakeThereMessage.Get(), UserPlayerNumber, EFFECT_CANNOTMAKETHERE);
					  }
					  // ��Ȯ�̰� 
					  else if(GD.SelectedOrder  && GD.SelectedOrder==ORDER_HARVEST)
					  {
						  if(MouseModeHarvest(MapX, MapY, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE)
						  {
							  // obj�� �����Ѵ�.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MouseHarvestX, MouseHarvestY, UnderMouseID);
						  }
						  else
						  {
							  // obj�� �����Ѵ�.  
			                  GD.ObjectNeedSwitch=FALSE;
				              GD.PushObject(MapX, MapY, UnderMouseID);

						  }
					  }
					  else
					  {
					     // obj�� �����Ѵ�.  
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

	// ������  ���콺 ��ư�� ���������� ó�� 
	if(IpD.RightPressDoneSwitch==TRUE)
	{
		IpD.RightPressDoneSwitch=FALSE;

		// ���콺�� �޴� ��ư���� ���� �ʾƾ� �ȴ�. 
		if(MenuOfMouse<0)
		{

			// 1��ư �������̽��� �����Ǿ� ������ ������ ��ư�� ���� ��ҷ� �۵��Ѵ�. 
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
				// ���õ� ĳ������ �޴��� ������Ʈ�Ѵ�. 
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

				// ������ ��ư �������� �Է¹��� �����, 
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

			// �̹� ����� �����Ǿ� �ִٸ�, 
			if(GD.ObjectNeedSwitch==FALSE)
			{

				// ���õ� ����� �̵��϶� �̵� ��ũ�� ǥ���ϰ� �ϰ� �̵� ȿ������ ����. 
				if( GD.SelectedOrder == ORDER_MOVE
				||  GD.SelectedOrder == ORDER_PATROL
				|| (GD.SelectedOrder == ORDER_ATTACK && GD.ObjectBuffer[0].ID.IsEmpty()) )	
				{
					// ���õ� ������ �̵� ����� ������ ȿ������ ���� �����. 
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
				// ���õ� ȿ������ �����϶� ���� ȿ������ ����.
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
				// ���õ� ȿ������ ��Ȯ�ϋ� 
			    else if(GD.SelectedOrder==ORDER_HARVEST)
				{
					// ���õ� ������ �����̳� ��Ȯ ����� ������ ȿ������ ���� �����. 
					int effectnumber=0;

					// ������� � ������ Ȯ���Ѵ�. 
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
				// ������ ������ ǥ���Ѵ�. 
				else if(GD.SelectedOrder == ORDER_TRAINSPOT)
				{
					if(GD.ObjectBuffer[0].ID.IsEmpty())
					{
					   SHORT realx, realy;
					   Map.TransFromScreenToRealXY(GameMouseX, GameMouseY, realx, realy);
					   GD.MoveMark.SetMoveMarkReal(realx, realy);

					}
				}
				// �Ǽ� ����̸� 
				// ���� �ż� ����̸�, 
				else if(Order[GD.SelectedOrder].IsAtb(ORDERATB_BUILD)
				|| GD.SelectedOrder==ORDER_MINE)
				{
					PushEffect(EFFECT_DECIDEBUILD, 0, 0);
				}


				// ���õ� ĳ���Ͱ� ������, 
				if(GD.ObjectBuffer[0].ID.IsEmpty()==FALSE)
				{
					if(IsAlive(GD.ObjectBuffer[0].ID.ID)==TRUE)
					{
						// obj�� ���õ� ĳ������ newselectedswitch�� �����Ѵ�.  
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


  	// �÷��̾� ���� ��ġ�� ������ ĳ���͸� ��ġ�Ѵ�.
	SHORT posindex=0;

	for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{
		maxoff = 1;
		
		// ��ȿ���� ���� �÷��̾�� ���ش�. 
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

			//ü���� ���� ĳ���ʹ� ������ �ʴ´�. 
			if(OBInfo.OIBPI[i].Iparameta[index].GetLife() > 0)
			{
				id = FindEmptyID(index);
				
				if(id)
				{
					x = Map.clMapPlayerInfo[posindex].StartPoint.x + SetCharPoint[j].x; 
					y = Map.clMapPlayerInfo[posindex].StartPoint.y + SetCharPoint[j].y;

					if( Map.IsInMapArea(x, y) == TRUE && Map.IsEmpty(x, y))
					{
						// CR[id]�� ĳ���͸� ����Ѵ�.
						SetChar(kind, id, x, y, SOUTH, 100, i, &OBInfo.OIBPI[i].Iparameta[index], &OBInfo.OIBPI[i].MW[index], index);

						OBInfo.OIBPI[i].Iparameta->GetLife();
						OBInfo.OIBPI[i].Iparameta->GetMana();


						
						// ��� �÷��̾��̸� ���� ���� �̸��� ����Ѵ�. 
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
						// NPC�̸� �̹� ����� �̸��� ����Ѵ�. 
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

	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);

	fp = BindJXFile::GetInstance()->GetFile( "OnlineBattleMap.dat" );

	if ( fp == NULL )
	{
		clGrp.Error("File Open Error", buffer);
		return FALSE;
	}

	// ù��° 6���� �����Ѵ�. 
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
		// �Ҵ��� �޸𸮸� �����Ѵ�. 
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


// OnlineBattleInfo�� ������ ��ȿ���� �˷��ش�. 
BOOL CheckOnlineBattleInfo(OnlineBattleInfo* pobinfo)
{
	SI32 i, j;
	SI32 playerswitch = FALSE;
	SI32 playercount  = 0;

	for(i=0; i<MAX_MULTI_PLAYER; i++)
	{
		// ����ڰ� ���� ���, 
		// ����� ��쿡�� �̸��� �ִٴ� ���� �ٰŷ� üũ.
		if(pobinfo->OIBPI[i].szPlayerName[0])
		{
			playerswitch = TRUE;
		}
		// ������ �����ϴ� ������� 
		if(pobinfo->OIBPI[i].siPlayersUniqueID)
		{
			playercount++;
		}
 
		for(j=0;j<ON_MAX_FOLLOWER_NUM;j++)
		{ 
			// ���ʽ� ������ ���̳ʽ��� ���, 
			if(pobinfo->OIBPI[i].Iparameta[j].GetBonus()  < 0)
			{
				clGrp.Error("Minus Bonus", "Bonus:%d", pobinfo->OIBPI[i].Iparameta[j].GetBonus());
				pobinfo->OIBPI[i].Iparameta[j].SetBonus(0);
				return FALSE;
			}
		}
	}


	// ������ ���ϴ� ����ڰ� ���� �����, 
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
	
	// ������ ������ �÷��̾� ���� 2���� ������, 
	if(playercount < 2)
	{
		clGrp.Error("Not Enough Players", "PlayerCount:%d", playercount);
		return FALSE;
		
	}
	// ������ ������ �÷��̾� ���� 7���� ������, 
	if(playercount > 7)
	{
		clGrp.Error("Too Many Players", "PlayerCount:%d", playercount);
		return FALSE;
	}
	
	return TRUE;
}

// �𰢿� ���ؼ� ����ġ �ս��� ġ���.
void SendBattleMessage_LoseForRetreat()
{

	// ��ŷ ����� ���������� �����Ѵ�. 
	{
		// ���ΰ� ĳ������ ������ ���´�. 
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
//		SetMessage("��ŷ�� ���ɼ��� �ִ� �������Դϴ�.");
//		m_ParmeterBox.IP.SetLife(0);
//	}
	
}

#endif
*/