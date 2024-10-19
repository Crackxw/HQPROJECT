#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "maindata.h"
#include <colortable.h>
#include <etc.h>
#include <item.h>
#include <map.h>
#include <charinterface.h>
#include <charorg.h>
#include <mynet-game.h>
#include <volcano.h>
#include "DebugTool.h"
#include "BindJXFile.h"
#include "..\Gersang\cheat.h"

extern	_clGame* pGame;
extern	_MainData			GD;									// 게임의 주데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Map 				Map;
extern	_Cheat 				Cheat;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_Item 				Item[];
extern	_Order 				Order[];
extern	clock_t				CurrentClock;						// 시간 관련 
extern	char 				SavePath[];
extern	_MyNetGame			MyNet;
extern  _Char   CR[];       // 캐릭터의 포인터 
extern	SI32				BlackStatus, DarkStatus;
extern	SHORT 		MapEditMode;

void _MainData::Init()
{
	int i, j, k;

	ZeroMemory(this, sizeof(_MainData));

	// 시간 초기화 
	Time.Init();

	// 지진의 초기화 
	EarthQuake.Init();

	for(i=0;i<MAX_BULLET_NUMBER;i++)
	{
		BulletInfo[i]=0;
	}
	for(i=0;i<MAX_SMOKE_NUMBER;i++)
	{
		SmokeInfo[i]=0;
	}

	Frame=0;
	GameFrame=0;
	RandomCount=0;
	RandomSeed=1;
    Rand=OldRand=0;

	MoveCharNumber=0;
	MoveCharID=0;
	m_bBlackCheck = TRUE;

	MoveMark.Init();


	// 작은 지도 관련 
	SmapModeChangedSwitch=FALSE;
	SmapModeFullSwitch=TRUE;

	ObjectNeedSwitch=FALSE;
	SelectedNumber=0;
	SelectedOrder=0;

	SelectedTeam=-1;
	PressedTeam=-1;

	for(i=0;i<MAX_MENU_NUMBER;i++)
	{
      Menu[i]=0;
	  MenuReserve[i]=0;
	}

	for(i=0;i<MAX_MULTI_PLAYER;i++)
		for(j=0;j<MAX_ORDER_QUE;j++)
		{
	      ZeroMemory(&OrderQue[i][j], sizeof(_OrderData));
		}

	ObjectBufferIndex=0;

	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
	    SelectedID[i].Init();
        IDPara1[i]=0; // 각 아이디 값의 파라메타 
	    IDPara2[i]=0; // 각 아이디 값의 파라메타 
	}

	// 캐릭터 정보를 초기화한다. 
	for(i=0;i<MAX_CHAR_NUMBER;i++)
	{
		CharInfo[i]=0;
	}

	BlackCheckStartID=0;

	for( i=0; i<MAX_MULTI_PLAYER; i++ )
		for( j=0; j<MAX_MAP_YSIZE; j++ )
			for( k=0; k<MAX_MAP_XSIZE; k++ )
			{
				BlackMap[i][k][j] = FOG2_BLACK_STEP;
			}

		for( j=0; j<MAX_MAP_YSIZE; j++ )
			for( k=0; k<MAX_MAP_XSIZE; k++ )
			{
				BlackMap2[k][j] = 0;
				BlackMap2ChangedSwitch[k][j] = TRUE;
			}

		  

	DarkLayor=0;

	Rain.Init();


	BattleFieldX=-1;
	BattleFieldY=-1;

	for(i = 0; i<MAX_MULTI_PLAYER;i++)
	{
		Player[i].Init(i);
	}

	bAutoMagicEnableSwitch = TRUE;

}


// 완전히 어두운 포그속에 있는가? 
BOOL _MainData::IsInBlackFog(SHORT x, SHORT y)
{
	if(BlackMap[UserPlayerNumber][x][y] == FOG2_BLACK_STEP)return TRUE;

	return FALSE;
}

// 중간 포그나 그보다 어두운 곳에 있는가?
BOOL _MainData::IsInMidFog(SHORT x, SHORT y)
{
	if(BlackMap[UserPlayerNumber][x][y] >= FOG2_DARK_STEP)return TRUE;

	return FALSE;
}


BOOL _MainData::SetBlackMap2(SI32 tilex, SI32 tiley, SI32 pos, SI32 data)
{
	if( Map.IsInMapArea(tilex, tiley) == FALSE )	return FALSE; 


	if(pos == 0)
	{
		BlackMap2[tilex][tiley] = (BlackMap2[tilex][tiley] & 0x0fff);
		BlackMap2[tilex][tiley] |= (data << 12);
	}
	else if(pos == 1 )
	{
		BlackMap2[tilex][tiley] = (BlackMap2[tilex][tiley] & 0xf0ff);
		BlackMap2[tilex][tiley] |= (data << 8);
	}
	else if(pos == 2 )
	{
		BlackMap2[tilex][tiley] = (BlackMap2[tilex][tiley] & 0xff0f);
		BlackMap2[tilex][tiley] |= (data << 4);
	}
	else if(pos == 3 )
	{
		BlackMap2[tilex][tiley] = (BlackMap2[tilex][tiley] & 0xfff0);
		BlackMap2[tilex][tiley] |= data;
	}

	return TRUE;
}

SHORT _MainData::GetBlackMap2(SHORT x, SHORT y)
{

	SI32 i;
	SI32 max_data =0;
	SI32 data = 0;

	if(Map.IsInMapArea(x, y)==FALSE)return 0;

	for(i=0;i<4;i++)
	{
		data =GetBlackMap2(x, y, i);
		if(data > max_data)
		{
			max_data= data;
		}
	}

	return max_data;
}

UI16 _MainData::GetBlackMap2(SI32 tilex, SI32 tiley, SI32 pos)
{
	UI16 data;
	
	if(Map.IsInMapArea(tilex, tiley) == FALSE)return 0;

	if(pos == 0)
	{
		data = ((BlackMap2[tilex][tiley] & 0xf000) >> 12);
	}
	else if(pos ==1 )
	{
		data = ((BlackMap2[tilex][tiley] & 0x0f00) >> 8);
	}
	else if(pos ==2 )
	{
		data = ((BlackMap2[tilex][tiley] & 0x00f0) >> 4);
	}
	else if(pos ==3 )
	{
		data = (BlackMap2[tilex][tiley] & 0x000f);
	}

	return data;
}


BOOL _MainData::IsCompleteBlackBlackMap2(SI32 tilex, SI32 tiley)
{
	if( BlackStatus  == BlackMap2[tilex][tiley] )  return TRUE;

	return FALSE;
}

BOOL _MainData::IsCompleteWhiteBlackMap2(SI32 tilex, SI32 tiley)
{
	if( BlackMap2[tilex][tiley] == 0 )  return TRUE;

	return FALSE;
}

BOOL _MainData::IsDarkFogBlackMap2(SI32 tilex, SI32 tiley)
{
	if( DarkStatus == BlackMap2[tilex][tiley] )  return TRUE;

	return FALSE;
}

void _MainData::DeleteAirCharID(_NewID id)
{

	SHORT i;

	if(AirCharNumber<=0)return ;

	for(i=0;i<AirCharNumber;i++)
	{
		if(AirCharID[i].IsSame(id))
		{
			AirCharID[i]=AirCharID[AirCharNumber-1];
			AirCharNumber--;
			return;
		}
	}


}

void _MainData::DeleteLandCharID(_NewID id)
{

	SHORT i;

	if(LandCharNumber<=0)return ;
//	if(AirCharNumber<=0)return ;

	for(i=0;i<LandCharNumber;i++)
	{
		if(LandCharID[i].IsSame(id))
		{
//			LandCharID[i]=LandCharID[AirCharNumber-1];
			LandCharID[i]=LandCharID[LandCharNumber-1];
			LandCharNumber--;
			return;
		}
	}


}


BOOL _MainData::PushObject(SHORT x, SHORT y, _NewID id)
{
  ObjectBuffer[ObjectBufferIndex].X=x;
  ObjectBuffer[ObjectBufferIndex].Y=y;
  ObjectBuffer[ObjectBufferIndex].ID=id;  // 캐릭터도 저장한다. 

   if(ObjectBufferIndex<MAX_MULTI_ORDER_NUMBER)
      ObjectBufferIndex++;	

   return TRUE;
}


BOOL _MainData::IncreaseCharNumber(SHORT pnumber, SHORT kind)
{
	if(pnumber>=MAX_MULTI_PLAYER)
	{
		return FALSE;
	}
	if(kind>=KIND_ON_MAX)
	{
		return FALSE;
	}

	CharNumber[pnumber][kind]++;


	return TRUE;
}

BOOL _MainData::DecreaseCharNumber(SHORT pnumber, SHORT kind)
{
	if(pnumber>=MAX_MULTI_PLAYER)
	{
		return FALSE;
	}
	if(kind>=KIND_ON_MAX)
	{
		return FALSE;
	}

	CharNumber[pnumber][kind]--;

	return TRUE;
}

// 시야를 처리한다.
void _MainData::BlackCheck(SHORT mode,BOOL bPKFieldMap)
{
	SHORT id;
	if(Cheat.CheatSeeSwitch == TRUE && MapEditMode)
		return;

	SHORT i, j, k;
	SHORT blackcheckswitch=FALSE;

    SHORT xsize = Map.GetXsize();
    SHORT ysize = Map.GetYsize();

	// mode가 TRUE일때만 기존의 것을 지운다. 
	if(mode == TRUE)
	{
		for(k = 0; k < MAX_MULTI_PLAYER;k++)
		{
			if(GD.Player[k].PI.Country == COUNTRY_NEUTRAL) continue;
			
			for(i = 0; i < ysize; i++)
			{
				for(j = 0; j < xsize; j++)
				{
					// 이미 밝혀졌던 곳.
					if(bPKFieldMap)
					{
						if(BlackMap[k][j][i] < FOG2_DARK_STEP)
						{
							BlackMap[k][j][i] = FOG2_DARK_STEP;
						}
					}
					else
						BlackMap[k][j][i] = 0;
				}
			}
		}
	}
	if(bPKFieldMap)
	{
		for(i = 0; i < ysize; i++)
		{
			for(j = 0; j < xsize; j++)
			{
				if(Map.GetFireMap(j, i))
				{
					GlobalBlackCheck(j, i, 2, TRUE);
				}    
			}
		}
		VolcanoBlackCheck();
		if((GD.InitInitMenuCount == 0) && (GD.InitQuitCount == 0) && (m_bBlackCheck == TRUE))
		{
			for(i = 0; i < CharOrderIndex; i++)
			{
				id = CharOrder[i];
					
				if(IsAlive(id))
				{
					CR[id].BlackCheck();
				}
			}
		}
	}
    Map.SetMapDrawSwitch(TRUE);

}


SHORT _MainData::GetMenu(SHORT index, int unique)
{
	char buffer[FILE_NAME_SIZE];

	if(index<0)
	{
		clGrp.FlipToGDISurface();
		wsprintf(buffer, "FDsgfdjew8jds: %d", unique);
		MessageBox(pGame->Hwnd, buffer, "fdsf93", MB_OK);
		return 0;
	}
	if(index>=MAX_MENU_NUMBER)
	{
		clGrp.FlipToGDISurface();
		wsprintf(buffer, "FDsgfdjew8jdsZ: %d", unique);
		MessageBox(pGame->Hwnd, buffer, "fdsf93", MB_OK);
		return 0;
	}

	return Menu[index];
}

void _MainData::SetMenu(SHORT index, SHORT menu, SHORT reservenumber, SHORT mode, SHORT menupercent)
{
	if(index<0)
	{
		clGrp.FlipToGDISurface();
		MessageBox(pGame->Hwnd, "fdsfejew8jds", "fdsf93", MB_OK);
		return ;
	}
	if(index>=MAX_MENU_NUMBER)
	{
		clGrp.FlipToGDISurface();
		MessageBox(pGame->Hwnd, "fdfdsdsfejew8jds", "fdsf93", MB_OK);
		return ;
	}

	Menu[index]=menu;
	MenuReserve[index]=reservenumber;
	MenuMode[index]=mode;
}

  
void _MainData::PushOrderBuffer(_OrderData order)
{
	if(OrderBufferIndex >= MAX_ORDER_BUFFER_NUMBER)return ;

/*	// 이전 명령과 같은 명령이면 무시한다. 
	if(OrderBufferIndex > 0)
	{
		if(order == OrderBuffer[OrderBufferIndex - 1])return ;
	}
*/
	OrderBuffer[OrderBufferIndex] = order;
	OrderBufferIndex++;

}

void _MainData::PopOrderBuffer()
{
	int i;

	if(OrderBufferIndex<=0)return ;

	for(i=1;i<OrderBufferIndex;i++)
	{
		OrderBuffer[i-1]=OrderBuffer[i];
	}

	OrderBufferIndex--;

}



void _MainData::SetBattleField(SHORT x, SHORT y)
{
	BattleFieldX=x;
	BattleFieldY=y;
}

void _MainData::SetScreenMove(int x, int y)
{
	ScreenMoveSwitch=TRUE;
	ScreenMoveX=x;
	ScreenMoveY=y;

}

// 전투 결과를 저장한다. 
BOOL _MainData::SaveResult(SHORT winswitch)
{
	FILE* fp;
	char buffer[FILE_NAME_SIZE];
	char playername[128];


	// 저장할 경로와 이름을 결정한다. 
	GetFileNamePath("result.dat", SavePath, buffer); 	

	fp=BindJXFile::GetInstance()->GetFile( buffer );
	if(fp==NULL)
	{
		return FALSE;
	}

	// 사용자의 이름을 얻어온다. 
	strncpy(playername, MyNet.PlayersName.Name[UserPlayerNumber], 128-1);

	// 이름을 저장한다.
	fwrite(playername, 128, 1, fp);

	// 승리 패배 여부를 저장한다. 
	fwrite(&winswitch, sizeof(SHORT), 1, fp);
	
	fclose(fp);

	return TRUE;
}




VOID _MainData::SetAutoMagicEnableSwitch(BOOL mode) 
{ 
	bAutoMagicEnableSwitch = mode; 
	
	for(int i =0;i <MAX_MULTI_PLAYER;i++)
	{
		if(Player[i].PI.Host != BY_COMPUTER)
		{
			// 자동 마법을 구사하는지 여부를 알려준다. 
			if(bAutoMagicEnableSwitch  == TRUE)
			{
				Player[i].AutoMagicSwitch=TRUE;
			}
			else
				Player[i].AutoMagicSwitch=FALSE;
		}
	}
}





// 선택된 캐릭터의 플레이어 번호를 돌려준다. (선택된 캐릭터중 첫번째 캐릭터의 플레이어 번호다. )
SI32 _MainData::FindSelectedCharPlayer()
{
	
	SI32 i;

	for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	{
		// 선택된 캐릭터를 넣는다. 
		if(GD.SelectedID[i].IsEmpty() == FALSE)
		{
			return clifChar.GetPlayerNumber( GD.SelectedID[i].ID );
		}
	}

	return -1;
}


// pnumber1가 pnumber2에게 친절한가 ?
BOOL _MainData::IsFriendTo(SI32 pnumber1, SI32 pnumber2)
{
	return OnlineAllyNumSwitch[pnumber1][pnumber2];
}


