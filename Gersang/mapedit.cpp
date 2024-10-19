#ifndef _ROCKET_NEW_TILE_MAP
#include <GSL.h>
#include <main.h>
#include <Mouse.h>
#include <clGame.h>


#include <Game2.h>
#include <setchar.h>
#include "maindata.h"
#include "..\Gersang\mapedit.h"
#include "..\Gersang\myio.h"


#include "resource.h"
#include <map.h>
#include <MapInterface.h>
#include <charinterface.h>
#include <ysmap.h>
#include "charanimation.h"
#include <kindinfo.h>

extern	_clGame					*pGame;
extern	char 				CurrentDirectory[];
extern	_Map 				Map;
extern	_YSMap 				SMap;								// 작은 지도용 데이터 
extern	SHORT 				MapEditMode;						// 지도 편집 모드인가?
extern	SHORT 				MapX, MapY;							// 마우스가 위치한 small 좌표 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_MapEdit 			MapEdit;
extern	_MainData			GD;									// 게임의 주데이터 
extern _InputDevice				IpD;

extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;						// 캐릭터 그림용


BOOL CALLBACK SelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


void _MapEdit::Init()
{

	PutSeaMode=TRUE;
	PutWaterMode=TRUE;
	PutNewBlkMode=TRUE;
	PutLevelLevel=MAX_LEVEL_STEP-1;

	PlayerNumber=0;

	Level=0;
	Tile=TILE_DEFAULT;
	TerrainMode=MAPEDIT_MODE_LEVEL;
	SelectedKind=KIND_ON_SWORDK;
	Ruin=CROP_TYPE_RUIN0;
}

void _MapEdit::Action()
{

	SHORT i, j, k;
	SHORT id;
	SHORT ableswitch;

	if(GetAsyncKeyState(VK_F1)<-1)
	{
		ShowCursor(TRUE);
		DialogBox(pGame->hInst, MAKEINTRESOURCE(IDD_SELECTTERRAIN), pGame->Hwnd, (DLGPROC)SelectTerrainDlgProc);
		ShowCursor(FALSE);
	}

   if(GetAsyncKeyState(VK_F2)<-1)
   {
	   ShowCursor(TRUE);
	   GetCurrentDirectory(512, CurrentDirectory);

	   if(PopMapFileSaveDlg(pGame->Hwnd, MapFileName, MapFileShortName))
	   {
			Map.Save(MapFileName);
	   }

	   SetCurrentDirectory(CurrentDirectory);

	   ShowCursor(FALSE);
   }

   if(GetAsyncKeyState(VK_F4)<-1)
   {
	   Map.MakeSmallMapBase();
   }

   if(GetAsyncKeyState(VK_F3)<-1)
   {
	   ShowCursor(TRUE);

	   GetCurrentDirectory(512, CurrentDirectory);

	   if(PopMapFileOpenDlg(pGame->Hwnd, MapFileName, MapFileShortName))
	   {
		     EndGame();

			// 지도를 불러온다 
			Map.Load(MapFileName);

			Map.MakeLastMap();

			// 지도용 타일을 해제한다.
			FreeMapTile();

			// 지도용 화일을 불러온다. 
			SetCurrentDirectory(CurrentDirectory);
	        LoadMapTile(Map.MH.MapType);

			// 작은 지도를 초기화 한다.
	        SMap.Init(Map.MH.MapXsize, Map.MH.MapYsize);
	    	Map.MakeSmallMapBase();


	        for(i=0;i<MAX_MAP_CHAR_NUMBER;i++)
			{
		        switch(Map.clMapUnitInfo[i].Kind)
				{
		          case 0:
			         break;
		          case KIND_PLAYERSTART:
			         break;

		          default:
					  if(Map.IsInMapArea(Map.clMapUnitInfo[i].X, Map.clMapUnitInfo[i].Y)==TRUE)
					  {
			            SetChar(Map.clMapUnitInfo[i].Kind, i, Map.clMapUnitInfo[i].X, Map.clMapUnitInfo[i].Y, SOUTH, 100, Map.clMapUnitInfo[i].Pnumber);
					  }
					  else
					  {
						  Map.clMapUnitInfo[i].Kind=0;
						  Map.clMapUnitInfo[i].X=-1;
						  Map.clMapUnitInfo[i].Y=-1;
						  Map.clMapUnitInfo[i].Pnumber=0;
					  }
			         break;
				}
			}

	   }

	   SetCurrentDirectory(CurrentDirectory);

	   ShowCursor(FALSE);
   }



   // 지도 편집 모드가 설정되어 있다면, 
   if(MapEditMode==TRUE)
   {
		if(IpD.LeftPressSwitch==TRUE)
		{

			switch(TerrainMode)
			{
                case MAPEDIT_MODE_LEVEL:
	                 MapEdit_PutLevel(MapX, MapY, Level, TRUE);
	                 break;     
                case MAPEDIT_MODE_SEA:
	                 MapEdit_PutSea(MapX, MapY, TRUE);
	                 break;
                case MAPEDIT_MODE_SHALLOW:
	                 Map.PutShallow(MapX, MapY, TRUE);
	                 break;
                case MAPEDIT_MODE_NEWBLK:
	                 MapEdit_PutNewBlk(MapX, MapY);
	                 break;
                case MAPEDIT_MODE_NEWBLKGATE:
					MapEdit_PutNewBlkGate(MapX/2, MapY/2);
	                 break;
                case MAPEDIT_MODE_CASTLE:
	                 MapEdit_PutCastle(MapX, MapY);
	                 break; 
                case MAPEDIT_MODE_CASTLE_GATE_LEFT:
	                 Map.PutCastleGate(MapX, MapY, 0, TRUE);
	                 break; 
                case MAPEDIT_MODE_CASTLE_GATE_RIGHT:
	                 Map.PutCastleGate(MapX, MapY, 1, TRUE);
	                 break; 
                case MAPEDIT_MODE_TILE:
	                 Map.PutTile(MapX, MapY, Tile);
	                 break; 
                case MAPEDIT_MODE_RICE:
	                 Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_RICE);
	                 break; 
                case MAPEDIT_MODE_POTATO:
	                 Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_POTATO);
	                 break; 

                case MAPEDIT_MODE_TREE:
	                 Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_TREE);
	                 break; 

                case MAPEDIT_MODE_BAMBOO:
	                 Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_BAMBOO);
	                 break; 

                case MAPEDIT_MODE_RUIN:
					 Map.PutRuin(MapX, MapY, Ruin,  1);
	                 break; 

				case MAPEDIT_MODE_SHIPPATROL:
					// 가까운 지점에 이미 있다면 넣을 수 없다. 
					ableswitch=TRUE;

					for(i=0;i<MAX_SHIP_PATROL_POINT_NUMBER;i++)
					{
						// 가까이에 이미 있다면, 
						if(Map.ShipPatrolPoint[i].x!=-1 && Map.ShipPatrolPoint[i].y!=-1 
						&& max( abs(Map.ShipPatrolPoint[i].x-MapX), 
						        abs(Map.ShipPatrolPoint[i].y-MapY))<2 )
						{
							ableswitch=FALSE;
						}
					}

					if(ableswitch==TRUE)
					{
					   PushShipPatrol(MapX, MapY);
					}
					break;

				case MAPEDIT_MODE_SHIPLANDING:
					// 가까운 지점에 이미 있다면 넣을 수 없다. 
					ableswitch=TRUE;

					for(i=0;i<MAX_SHIP_LANDING_POINT_NUMBER;i++)
					{
						// 가까이에 이미 있다면, 
						if(Map.ShipLandingPoint[i].x!=-1 && Map.ShipLandingPoint[i].y!=-1 
						&& max( abs(Map.ShipLandingPoint[i].x-MapX), 
						        abs(Map.ShipLandingPoint[i].y-MapY))<2)
						{
							ableswitch=FALSE;
						}
					}

					if(ableswitch==TRUE)
 					    PushShipLanding(MapX, MapY);
					break;

                case MAPEDIT_MODE_UNIT:
					// 플레이어 시작지점이면, 
					if(SelectedKind==KIND_PLAYERSTART)
					{
						Map.clMapPlayerInfo[PlayerNumber].StartPoint.x=MapX;
						Map.clMapPlayerInfo[PlayerNumber].StartPoint.y=MapY;
					}
					else
					{
						for(i=1;i<MAX_MAP_CHAR_NUMBER;i++)
						{
							if(Map.clMapUnitInfo[i].Kind == 0)
							{

								// 선택된 자리에 다른 캐릭터가 있어서는 안된다. 
								SHORT locateswitch=TRUE;
								for( j=0; j<KI[SelectedKind].m_ssIDYsize; j++ )
									for( k=0; k<KI[SelectedKind].m_ssIDXsize; k++ )
									{
									   // 뭔가가 있다면, 
									   if( (KI[SelectedKind].m_uiAtb&ATB_FLY) )
									   {
										   id = clifMap.GetSkyMap(MapX-KI[SelectedKind].m_ssIDXsize/2+k, 
											   MapY-KI[SelectedKind].m_ssIDYsize/2+j);
									   }
									   else
									   {
										   id = Map.GetIDMap(MapX-KI[SelectedKind].m_ssIDXsize/2+k, 
											   MapY-KI[SelectedKind].m_ssIDYsize/2+j);
									   }
									   
									   if( id )		locateswitch = FALSE;
								   }

								if(locateswitch==TRUE)
							   {

						           SetChar(SelectedKind, i, MapX, MapY, SOUTH, 100, PlayerNumber);
						           Map.clMapUnitInfo[i].Kind=SelectedKind;
						           Map.clMapUnitInfo[i].X=MapX;
						           Map.clMapUnitInfo[i].Y=MapY;
						           Map.clMapUnitInfo[i].Pnumber=PlayerNumber;

								   
							   }
							   break;
						   }
					   }
					}
		                 
		                 break; 
				}

			}
			else if(IpD.RightPressSwitch==TRUE)
			{
				switch(TerrainMode)
				{
                case MAPEDIT_MODE_SHALLOW:
	                 Map.PutShallow(MapX, MapY, FALSE);
	                 break;

                case MAPEDIT_MODE_NEWBLK:
	                 MapEdit_DeleteNewBlk(MapX, MapY);
	                 break;

                case MAPEDIT_MODE_CASTLE:
	                 MapEdit_DeleteCastle(MapX, MapY);
	                 break; 

                case MAPEDIT_MODE_CASTLE_GATE_LEFT:
	                 Map.PutCastleGate(MapX, MapY, 0, FALSE);
	                 break; 
                case MAPEDIT_MODE_CASTLE_GATE_RIGHT:
	                 Map.PutCastleGate(MapX, MapY, 0, FALSE);
	                 break; 

				case MAPEDIT_MODE_UNIT:
				// 플레이어 시작지점이면, 
				if(SelectedKind==KIND_PLAYERSTART)
				{
					Map.clMapPlayerInfo[PlayerNumber].StartPoint.x=-1;
					Map.clMapPlayerInfo[PlayerNumber].StartPoint.y=-1;
				}
				else
				{

				   // 공중 속성이라면
					if(KI[SelectedKind].m_uiAtb&ATB_FLY)
					{
						id = clifMap.GetSkyMap(MapX, MapY);
					}
					else
					{
						id=Map.GetIDMap(MapX, MapY);
					}

				   if(id)
				   {
					   clifChar.DeleteID(id);

					   DeleteChar(id);

					   Map.clMapUnitInfo[id].Kind=0;
					   Map.clMapUnitInfo[id].X=0;
					   Map.clMapUnitInfo[id].Y=0;
					   Map.clMapUnitInfo[id].Pnumber=0;
				   }
				}

					break;
	            case MAPEDIT_MODE_POTATO:
				case MAPEDIT_MODE_RICE:
				case MAPEDIT_MODE_TREE:
				case MAPEDIT_MODE_BAMBOO:
		            Map.PutCrop(MapX,   MapY, 0);
					Map.PutTree(MapX,   MapY, 0);
		            break; 

                case MAPEDIT_MODE_RUIN:
					Map.PutRuin(MapX, MapY, 0,  0);

				}

			}
		}
}


// 배순찰지점을 추가한다.
void _MapEdit::PushShipPatrol(SHORT x, SHORT y)
{

	SHORT i;

	for(i=0;i<MAX_SHIP_PATROL_POINT_NUMBER;i++)
	{
		// 비어있으면 넣는다. 
	    if(Map.ShipPatrolPoint[i].x<0 && Map.ShipPatrolPoint[i].y<0)
		{
			Map.ShipPatrolPoint[i].x=x;
			Map.ShipPatrolPoint[i].y=y;
			return ;
		}
	}
}

// 배 상륙지점을 추가한다. 
void _MapEdit::PushShipLanding(SHORT x, SHORT y)
{
	SHORT i;

	for(i=0;i<MAX_SHIP_LANDING_POINT_NUMBER;i++)
	{
		// 비어있으면 넣는다. 
	    if(Map.ShipLandingPoint[i].x<0 && Map.ShipLandingPoint[i].y<0)
		{
			Map.ShipLandingPoint[i].x=x;
			Map.ShipLandingPoint[i].y=y;
			return ;
		}
	}

}

void _MapEdit::MapEdit_PutLevel(SHORT x, SHORT y, SHORT level, SHORT pushmode)
{
	Map.DeleteLevel(x, y, level);
	
	// 얕은 물은 지운다. 
	if( Map.GetSeaMode(x, y) == FALSE )
	    Map.PutShallow(x, y, FALSE);

	Map.PutLevel(x, y, level, pushmode);

}


void _MapEdit::MapEdit_PutSea(SHORT x, SHORT y, SHORT mode)
{


	Map.PutSea(x, y, mode, TRUE);
}


void _MapEdit::MapEdit_PutWater(SHORT x, SHORT y)
{
	

}

void _MapEdit::MapEdit_PutNewBlk(SHORT x, SHORT y)
{
	
	Map.PutNewBlk(x, y, TRUE);

}

void _MapEdit::MapEdit_DeleteNewBlk(SHORT x, SHORT y)
{
	
	Map.PutNewBlk(x, y, FALSE);

}


void _MapEdit::MapEdit_PutCastle(SHORT x, SHORT y)
{
	
	Map.PutCastle(x, y, TRUE);

}

void _MapEdit::MapEdit_DeleteCastle(SHORT x, SHORT y)
{
	
	Map.PutCastle(x, y, FALSE);

}

void _MapEdit::MapEdit_PutNewBlkGate(SHORT x, SHORT y)
{

	Map.PutNewBlkGate(x, y);
}


// 지도 정보를 표시한다. 
void _MapEdit::MapEdit_DrawInfo()
{
	HDC hdc;
	SI32 i;
	char buffer[FILE_NAME_SIZE];

	if(lpScreenBuffer->GetDC(&hdc)==DD_OK)
	{
         
	     SelectObject(hdc, pGame->pBattle->HelpFont);
         SetBkColor( hdc, RGB( 0, 0, 0 ) );
         SetTextColor( hdc, RGB( 180, 180, 180 ) );
   	     SetBkMode(hdc, TRANSPARENT);

		 // 게임에 참여할 수 있는 사용자의 수가 몇인지 파악한다. 
		 SI32 playercount = 0;
		 for(i = 0;i < MAX_MULTI_PLAYER;i++)
		 {
			 if(Map.clMapPlayerInfo[i].StartPoint.x>=0 && Map.clMapPlayerInfo[i].StartPoint.y>=0)
			 {
				 playercount++;
			 }
		 }
		 
	     wsprintf(buffer, "Selected Unit:%s X:%d Y:%d Start Number:%d", KI[MapEdit.SelectedKind].m_pszName, MapX, MapY, playercount);

	     TextOut(hdc, 200, 430, buffer, lstrlen(buffer));

         lpScreenBuffer->ReleaseDC(hdc);
	   }
}



BOOL CALLBACK SelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	char buffer[FILE_NAME_SIZE];
	HWND hwnd;
	SHORT i;
	SHORT hindex=0;
	NMHDR * pnmhdr;
	static TV_ITEM tvi;
	static TV_INSERTSTRUCT tvis;
	static HTREEITEM nationhtv[3];
	
	static HTREEITEM koreahtv[2];
	static HTREEITEM korbuild[20];
	static HTREEITEM korchar[20];

	static HTREEITEM japanhtv[2];
	static HTREEITEM jpnbuild[20];
	static HTREEITEM jpnchar[20];

	static HTREEITEM chinahtv[2];
	static HTREEITEM chnbuild[20];
	static HTREEITEM chnchar[20];

	


	switch(iMsg)
	{
	case WM_INITDIALOG:

		// 지형 정보 
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 0, (LPARAM)"Level");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 1, (LPARAM)"Sea");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 2, (LPARAM)"Shallow");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 3, (LPARAM)"New Block");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 4, (LPARAM)"New Block Gate");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 5, (LPARAM)"Castle");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 6, (LPARAM)"Castle Gate(left)");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 7, (LPARAM)"Castle Gate(right)");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 8, (LPARAM)"Tile");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 9, (LPARAM)"Rice");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,10, (LPARAM)"Potato");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,11, (LPARAM)"Tree");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,12, (LPARAM)"Bamboo");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,13, (LPARAM)"Ruin");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,14, (LPARAM)"Ship Patrol");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,15, (LPARAM)"Ship Landing");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,16, (LPARAM)"Unit");

		// 디폴트 선택 
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_SETCURSEL, MapEdit.TerrainMode, 0);

		// 플레이어 번호 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 0, (LPARAM)"1");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 1, (LPARAM)"2");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 2, (LPARAM)"3");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 3, (LPARAM)"4");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 4, (LPARAM)"5");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 5, (LPARAM)"6");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 6, (LPARAM)"7");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 7, (LPARAM)"8");

		// 디폴트 선택 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_SETCURSEL, MapEdit.PlayerNumber, 0);

		// 국가 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_NO,     (LPARAM)"None");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_KOREA,  (LPARAM)"Korea");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_JAPAN,  (LPARAM)"Japan");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_CHINA,  (LPARAM)"China");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_NEUTRAL,(LPARAM)"Neutral");

		// 호스트 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST), CB_INSERTSTRING, BY_PLAYER,     (LPARAM)"Player");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST), CB_INSERTSTRING, BY_COMPUTER,  (LPARAM)"Computer");

		// 그룹
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 0,     (LPARAM)"1");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 1,     (LPARAM)"2");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 2,     (LPARAM)"3");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 3,     (LPARAM)"4");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 4,     (LPARAM)"5");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 5,     (LPARAM)"6");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 6,     (LPARAM)"7");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 7,     (LPARAM)"8");

		// 지형 지물 
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 0,     (LPARAM)"건물잔해1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 1,     (LPARAM)"건물잔해2");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 2,     (LPARAM)"발석거잔해");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 3,     (LPARAM)"장승");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 4,     (LPARAM)"무덤1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 5,     (LPARAM)"부러진 거목");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 6,     (LPARAM)"성황당");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 7,     (LPARAM)"우물");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 8,     (LPARAM)"거북 비석");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 9,     (LPARAM)"거석1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 10,     (LPARAM)"거석2");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 11,     (LPARAM)"화계사");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 12,     (LPARAM)"이정표");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 13,     (LPARAM)"조선깃발1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 14,     (LPARAM)"명깃발1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 15,    (LPARAM)"일본깃발1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 16,    (LPARAM)"삼성 이정표");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 17,    (LPARAM)"삼성 바닥");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 18,    (LPARAM)"삼성 전화기");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 19,    (LPARAM)"화산");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 20,    (LPARAM)"성곽망루");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 21,    (LPARAM)"무너진 성곽");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 22,    (LPARAM)"성문");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 23,    (LPARAM)"성곽 타일");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 24,    (LPARAM)"큰 성문");
		
		// 타일 타입(눈...)
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 0,    (LPARAM)"기본 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 1,    (LPARAM)"눈   타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 2,    (LPARAM)"갈색 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 3,    (LPARAM)"녹색 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 4,    (LPARAM)"척박지 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 5,    (LPARAM)"화산 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 6,    (LPARAM)"봄 타일");


		// 높이 정보 
		if(MapEdit.Level==0)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL0), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL0), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==1)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL1), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL1), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==2)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL2), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL2), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==3)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL3), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL3), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==4)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL4), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL4), BM_SETCHECK, 0, 0);

		// 타일 정보 
		if(MapEdit.Tile==TILE_DEFAULT)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_NORMAL), BM_SETCHECK, 1, 0);
		else 
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_NORMAL), BM_SETCHECK, 0, 0);

		if(MapEdit.Tile==TILE_GRASS)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_GREEN), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_GREEN), BM_SETCHECK, 0, 0);

		if(MapEdit.Tile==TILE_GREEN)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_RED), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_RED), BM_SETCHECK, 0, 0);

		if(MapEdit.Tile==TILE_CROPAREA)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_CROPAREA), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_CROPAREA), BM_SETCHECK, 0, 0);

		// 지도 정보 
        hwnd=GetDlgItem(hDlg, IDC_MAP_XSIZE);
		wsprintf(buffer, "%d", Map.MH.MapXsize);
		SetWindowText(hwnd, buffer);

        hwnd=GetDlgItem(hDlg, IDC_MAP_YSIZE);
		wsprintf(buffer, "%d", Map.MH.MapYsize);
		SetWindowText(hwnd, buffer);

		// 유니트 
		tvis.hInsertAfter=TVI_LAST;
	    tvi.mask=TVIF_TEXT|TVIF_PARAM; 

		// 플레이어 시작위치  
		tvi.pszText="Player Start";
		tvi.lParam=KIND_PLAYERSTART;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[0]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
	
		// 조선 
		tvi.pszText="Korea";
		tvi.lParam=0;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[0]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		// 조선 캐릭터 
		tvi.pszText="Character";
		tvi.lParam=0;
		tvis.hParent=nationhtv[0];
		tvis.item=tvi;
		koreahtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		hindex=0;
		for(i=0;i<KIND_ON_MAX;i++)
		{
			if(KI[i].m_ssNation==COUNTRY_KOREA && i <= 111) // 온라인 캐릭터 안나오게 임시로.
			{
		       tvi.pszText=KI[i].m_pszName;
			   tvi.lParam=i;
		       tvis.hParent=koreahtv[1];
		       tvis.item=tvi;
		       korchar[hindex]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
			   hindex++;
			}
		}


		// 일본 
		tvi.pszText="Japan";
		tvi.lParam=0;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		tvi.pszText="Character";
		tvi.lParam=0;
		tvis.hParent=nationhtv[1];
		tvis.item=tvi;
		japanhtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		hindex=0;
		for(i=0;i<KIND_ON_MAX;i++)
		{
			if(KI[i].m_ssNation==COUNTRY_JAPAN)
			{
		       tvi.pszText=KI[i].m_pszName;
			   tvi.lParam=i;
		       tvis.hParent=japanhtv[1];
		       tvis.item=tvi;
		       jpnchar[hindex]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
			   hindex++;
			}
		}

		// 중국 
		tvi.pszText="China";
		tvi.lParam=0;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[2]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		tvi.pszText="Character";
		tvi.lParam=0;
		tvis.hParent=nationhtv[2];
		tvis.item=tvi;
		chinahtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		hindex=0;
		for(i=0;i<KIND_ON_MAX;i++)
		{
			if(KI[i].m_ssNation==COUNTRY_CHINA )
			{
		       tvi.pszText=KI[i].m_pszName;
			   tvi.lParam=i;
		       tvis.hParent=chinahtv[1];
		       tvis.item=tvi;
		       chnchar[hindex]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
			   hindex++;
			}
		}


    	return TRUE;

	case WM_NOTIFY:

		pnmhdr=(LPNMHDR)lParam;

			if(pnmhdr->code==TVN_SELCHANGED)
			{
		 	    tvi.mask=TVIF_PARAM;
			    tvi.hItem=((LPNM_TREEVIEW)pnmhdr)->itemNew.hItem;

			    if(TreeView_GetItem(GetDlgItem(hDlg, IDC_UNIT), &tvi)==TRUE)
				{
			       // 선택된 캐릭터를 찾는다. 
			       for(i=1;i<KIND_ON_MAX;i++)
				   {
				       if(tvi.lParam==i)
					   { 
					       MapEdit.SelectedKind=i;
						   
						   // 선택된 캐릭터의 개수를 표시한다. 
						   hwnd=GetDlgItem(hDlg, IDC_UNIT_NUMBER);
		                   wsprintf(buffer, "%d", GD.CharNumber[MapEdit.PlayerNumber][i]);
						   SetWindowText(hwnd, buffer);

						   //선택된 캐릭터의 최대 개수를 표시한다. 
						   hwnd=GetDlgItem(hDlg, IDC_MAX_UNIT_NUMBER);
		                   wsprintf(buffer, "%d", Map.clMapPlayerInfo[MapEdit.PlayerNumber].MaxUnitNumber[i]);
		                   SetWindowText(hwnd, buffer);

						   // 게임에 등장하는지 여부를 알려준다. 
						   if(Map.clMapPlayerInfo[MapEdit.PlayerNumber].IsCharInGame[i]==TRUE)
						      SendMessage(GetDlgItem(hDlg, IDC_INGAME), BM_SETCHECK, 1, 0);
						   else
							  SendMessage(GetDlgItem(hDlg, IDC_INGAME), BM_SETCHECK, 0, 0);

					   }
				   }
				}
			}
		break;

	case WM_COMMAND:

		switch(LOWORD(wParam))
		{
		case IDOK:

			switch(SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_GETCURSEL, 0, 0))
			{
			case 0: MapEdit.TerrainMode=MAPEDIT_MODE_LEVEL; break;
			case 1: MapEdit.TerrainMode=MAPEDIT_MODE_SEA;   break;
			case 2: MapEdit.TerrainMode=MAPEDIT_MODE_SHALLOW;   break;
			case 3: MapEdit.TerrainMode=MAPEDIT_MODE_NEWBLK;break;
			case 4: MapEdit.TerrainMode=MAPEDIT_MODE_NEWBLKGATE; break;
			case 5: MapEdit.TerrainMode=MAPEDIT_MODE_CASTLE; break;
			case 6: MapEdit.TerrainMode=MAPEDIT_MODE_CASTLE_GATE_LEFT; break;
			case 7: MapEdit.TerrainMode=MAPEDIT_MODE_CASTLE_GATE_RIGHT; break;
			case 8: MapEdit.TerrainMode=MAPEDIT_MODE_TILE;  break;
			case 9: MapEdit.TerrainMode=MAPEDIT_MODE_RICE;  break;
			case 10: MapEdit.TerrainMode=MAPEDIT_MODE_POTATO;break;
			case 11: MapEdit.TerrainMode=MAPEDIT_MODE_TREE;  break;
			case 12: MapEdit.TerrainMode=MAPEDIT_MODE_BAMBOO;  break;
			case 13: MapEdit.TerrainMode=MAPEDIT_MODE_RUIN;  break;
			case 14:MapEdit.TerrainMode=MAPEDIT_MODE_SHIPPATROL;break;
			case 15:MapEdit.TerrainMode=MAPEDIT_MODE_SHIPLANDING;break;
			case 16:MapEdit.TerrainMode=MAPEDIT_MODE_UNIT;  break;
			}

			switch(SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_GETCURSEL, 0, 0))
			{
			case 0: MapEdit.PlayerNumber=0; break;
			case 1: MapEdit.PlayerNumber=1; break;
			case 2: MapEdit.PlayerNumber=2; break;
			case 3: MapEdit.PlayerNumber=3; break;
			case 4: MapEdit.PlayerNumber=4; break;
			case 5: MapEdit.PlayerNumber=5; break;
			case 6: MapEdit.PlayerNumber=6; break;
			case 7: MapEdit.PlayerNumber=7; break;
			}


			switch(SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_GETCURSEL, 0, 0))
			{
			case 0: MapEdit.Ruin=CROP_TYPE_RUIN0; break;
			case 1: MapEdit.Ruin=CROP_TYPE_RUIN1; break;
			case 2: MapEdit.Ruin=CROP_TYPE_RUIN2; break;
			case 3: MapEdit.Ruin=CROP_TYPE_DOLL1; break;
			case 4: MapEdit.Ruin=CROP_TYPE_TOMB1; break;
			case 5: MapEdit.Ruin=CROP_TYPE_BROKENTREE1; break;
			case 6: MapEdit.Ruin=CROP_TYPE_ODDTREE1; break;
			case 7: MapEdit.Ruin=CROP_TYPE_WELL1;  break;
			case 8: MapEdit.Ruin=CROP_TYPE_TUTTLE1;  break;
			case 9: MapEdit.Ruin=CROP_TYPE_STONE1;  break;
			case 10: MapEdit.Ruin=CROP_TYPE_STONE2;  break;
			case 11: MapEdit.Ruin=CROP_TYPE_TEMPLE;  break;
			case 12: MapEdit.Ruin=CROP_TYPE_SIGN1;  break;
			case 13: MapEdit.Ruin=CROP_TYPE_FLAGK1; break;
			case 14: MapEdit.Ruin=CROP_TYPE_FLAGC1; break;
			case 15:MapEdit.Ruin=CROP_TYPE_FLAGJ1; break;
            case 16: MapEdit.Ruin=CROP_TYPE_AD11;  break;
			case 17: MapEdit.Ruin=CROP_TYPE_AD12;  break;
			case 18: MapEdit.Ruin=CROP_TYPE_AD13;  break;
			case 19: MapEdit.Ruin=CROP_TYPE_VOLCANO; break;
			case 20: MapEdit.Ruin=CROP_TYPE_CASTLETOWER;  break;
			case 21: MapEdit.Ruin=CROP_TYPE_DESTROYEDCASTLE;  break;
			case 22: MapEdit.Ruin=CROP_TYPE_CASTLEGATE;  break;
			case 23: MapEdit.Ruin=CROP_TYPE_CASTLETILE;  break;
			case 24: MapEdit.Ruin=CROP_TYPE_CASTLELARGEGATE;  break;
			}

			// 높이 
			hwnd=GetDlgItem(hDlg, IDC_LEVEL0);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=0;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL1);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=1;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL2);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=2;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL3);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=3;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL4);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=4;
			}

			// 타일 
			hwnd=GetDlgItem(hDlg, IDC_TILE_NORMAL);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_DEFAULT;
			}
			hwnd=GetDlgItem(hDlg, IDC_TILE_GREEN);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_GRASS;
			}
			hwnd=GetDlgItem(hDlg, IDC_TILE_RED);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_GREEN;
			}
			hwnd=GetDlgItem(hDlg, IDC_TILE_CROPAREA);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_CROPAREA;
			}

			// 지도 정보
			hwnd=GetDlgItem(hDlg, IDC_NEWMAP);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				hwnd=GetDlgItem(hDlg, IDC_MAP_XSIZE);
			    GetWindowText(hwnd, buffer, 10);
			    SHORT xsize=atoi(buffer);

                hwnd=GetDlgItem(hDlg, IDC_MAP_YSIZE);
			    GetWindowText(hwnd, buffer, 10);
			    SHORT ysize=atoi(buffer);

				EndGame();

				// 지도를 초기화한다. 
				Map.InitMap(xsize, ysize, FALSE);
				Map.MakeLastMap();
				SMap.Init(xsize, ysize);
			}



		    EndDialog(hDlg, 0);
		   return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case IDC_MAX_UNIT_NUMBER:
			// 캐릭터의 최대 개수를 지정한다. 
			if(HIWORD(wParam)==EN_CHANGE)
			{
				hwnd=GetDlgItem(hDlg, IDC_MAX_UNIT_NUMBER);
			    GetWindowText(hwnd, buffer, 10);
			    SHORT maxnumber=atoi(buffer);

				Map.clMapPlayerInfo[MapEdit.PlayerNumber].MaxUnitNumber[MapEdit.SelectedKind]=maxnumber;

			}
			break;

       case IDC_INGAME:
		   //if(HIWORD(wParam)==CN_CHANGE)
		   {
			  hwnd=GetDlgItem(hDlg, IDC_INGAME);
			  if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			  {
				  Map.clMapPlayerInfo[MapEdit.PlayerNumber].IsCharInGame[MapEdit.SelectedKind]=TRUE;
			  }
			  else
				  Map.clMapPlayerInfo[MapEdit.PlayerNumber].IsCharInGame[MapEdit.SelectedKind]=FALSE;
		   }

		   break;

		case IDC_PLAYERNUMBER:
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				MapEdit.PlayerNumber=(SHORT)SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_GETCURSEL, 0, 0);


				// 새로운 플레이어에 맞는 설정으로 바꾼다.
		        SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_SETCURSEL, Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Country, 0);
				SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST),    CB_SETCURSEL, Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Host   , 0);


				// 자원 표시 
				hwnd=GetDlgItem(hDlg, IDC_CROP);
		        wsprintf(buffer, "%d", Map.clMapPlayerInfo[MapEdit.PlayerNumber].Money);
		        SetWindowText(hwnd, buffer);

				hwnd=GetDlgItem(hDlg, IDC_TREE);
		        wsprintf(buffer, "%d", Map.clMapPlayerInfo[MapEdit.PlayerNumber].Tree);
		        SetWindowText(hwnd, buffer);

			}
			break;

		case IDC_PLAYER_COUNTRY:
			// 국가가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Country = (UI08)SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_GETCURSEL, 0, 0);
			}
			break;

		case IDC_PLAYER_HOST:
			// 주체가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Host=(UI08)SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST), CB_GETCURSEL, 0, 0);
			}
			break;

		case IDC_CROP:
			hwnd=GetDlgItem(hDlg, IDC_CROP);
		    GetWindowText(hwnd, buffer, 10);
		    Map.clMapPlayerInfo[MapEdit.PlayerNumber].Money=atoi(buffer);
			break;

		case IDC_TREE:
			hwnd=GetDlgItem(hDlg, IDC_TREE);
		    GetWindowText(hwnd, buffer, 10);
		    Map.clMapPlayerInfo[MapEdit.PlayerNumber].Tree=atoi(buffer);
			break;

		case IDC_PLAYER_GROUP:
			// 주체가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{

			}
			break;

		case IDC_MAPTYPE:

			// 주체가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{

				Map.MH.MapType= (SHORT)SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_GETCURSEL, 0, 0);

				FreeMapTile();
				LoadMapTile(Map.MH.MapType);

			}
			break;


		}
		break;
	}
	return FALSE;
}



#else

#include <GSL.h>
#include <main.h>
#include <Mouse.h>
#include <clGame.h>


#include <Game2.h>
#include <setchar.h>
#include "maindata.h"
#include "..\Gersang\mapedit.h"
#include "..\Gersang\myio.h"


#include "resource.h"
#include <map.h>
#include <MapInterface.h>
#include <charinterface.h>
#include <ysmap.h>
#include "charanimation.h"
#include <kindinfo.h>

extern	_clGame					*pGame;
extern	char 				CurrentDirectory[];
extern	_Map 				Map;
extern	_YSMap 				SMap;								// 작은 지도용 데이터 
extern	SHORT 				MapEditMode;						// 지도 편집 모드인가?
extern	SHORT 				MapX, MapY;							// 마우스가 위치한 small 좌표 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_MapEdit 			MapEdit;
extern	_MainData			GD;									// 게임의 주데이터 
extern _InputDevice				IpD;
extern _FileVary				FV;

extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;						// 캐릭터 그림용


BOOL CALLBACK SelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


void _MapEdit::Init()
{

	PutSeaMode=TRUE;
	PutWaterMode=TRUE;
	PutNewBlkMode=TRUE;
	PutLevelLevel=MAX_LEVEL_STEP-1;

	PlayerNumber=0;

	Level=0;
	Tile=TILE_DEFAULT;
	TerrainMode=MAPEDIT_MODE_LEVEL;
	SelectedKind=KIND_ON_SWORDK;
	Ruin=CROP_TYPE_RUIN0;
		
	Tree = 0;	//나무 타입..
}

void _MapEdit::Action()
{

	SHORT i, j, k;
	SHORT id;
	SHORT ableswitch;

	if(GetAsyncKeyState(VK_F1)<-1)
	{
		ShowCursor(TRUE);
		DialogBox(pGame->hInst, MAKEINTRESOURCE(IDD_SELECTTERRAIN), pGame->Hwnd, (DLGPROC)SelectTerrainDlgProc);
		ShowCursor(FALSE);
	}

   if(GetAsyncKeyState(VK_F2)<-1)
   {
	   ShowCursor(TRUE);
	   GetCurrentDirectory(512, CurrentDirectory);

	   if(PopMapFileSaveDlg(pGame->Hwnd, MapFileName, MapFileShortName))
	   {
			Map.Save(MapFileName);
	   }

	   SetCurrentDirectory(CurrentDirectory);

	   ShowCursor(FALSE);
   }

   if(GetAsyncKeyState(VK_F4)<-1)
   {
	   Map.MakeSmallMapBase();
   }

   if(GetAsyncKeyState(VK_F3)<-1)
   {
	   ShowCursor(TRUE);

	   GetCurrentDirectory(512, CurrentDirectory);

	   if(PopMapFileOpenDlg(pGame->Hwnd, MapFileName, MapFileShortName))
	   {
		     EndGame();

			// 지도를 불러온다 
			Map.Load(MapFileName);

			Map.MakeLastMap();

			// 지도용 타일을 해제한다.
			FreeMapTile();

			// 지도용 화일을 불러온다. 
			SetCurrentDirectory(CurrentDirectory);
	        LoadMapTile(Map.MH.MapType);

			// 작은 지도를 초기화 한다.
	        SMap.Init(Map.MH.MapXsize, Map.MH.MapYsize);
	    	Map.MakeSmallMapBase();


	        for(i=0;i<MAX_MAP_CHAR_NUMBER;i++)
			{
		        switch(Map.clMapUnitInfo[i].Kind)
				{
		          case 0:
			         break;
		          case KIND_PLAYERSTART:
			         break;

		          default:
					  if(Map.IsInMapArea(Map.clMapUnitInfo[i].X, Map.clMapUnitInfo[i].Y)==TRUE)
					  {
			            SetChar(Map.clMapUnitInfo[i].Kind, i, Map.clMapUnitInfo[i].X, Map.clMapUnitInfo[i].Y, SOUTH, 100, Map.clMapUnitInfo[i].Pnumber);
					  }
					  else
					  {
						  Map.clMapUnitInfo[i].Kind=0;
						  Map.clMapUnitInfo[i].X=-1;
						  Map.clMapUnitInfo[i].Y=-1;
						  Map.clMapUnitInfo[i].Pnumber=0;
					  }
			         break;
				}
			}

	   }

	   SetCurrentDirectory(CurrentDirectory);

	   ShowCursor(FALSE);
   }



   // 지도 편집 모드가 설정되어 있다면, 
   if(MapEditMode==TRUE)
   {
		if(IpD.LeftPressSwitch==TRUE)
		{

			switch(TerrainMode)
			{
                case MAPEDIT_MODE_LEVEL:
					 MessageBox(NULL, "현 버젼의 대상에서는 레벨 에디트를 할수 없습니다.","",MB_OK);
	                 //MapEdit_PutLevel(MapX, MapY, Level, TRUE);
					 break;     
                case MAPEDIT_MODE_SEA:
	                 MapEdit_PutSea(MapX, MapY, TRUE);
	                 break;
                case MAPEDIT_MODE_SHALLOW:
					 MessageBox(NULL, "현 버젼의 대상에서는 얕은 물 에디트를 할수없습니다.","",MB_OK);	
	                 //Map.PutShallow(MapX, MapY, TRUE);

	                 break;
                case MAPEDIT_MODE_NEWBLK:
	                 MapEdit_PutNewBlk(MapX, MapY);
	                 break;
                case MAPEDIT_MODE_NEWBLKGATE:
					MapEdit_PutNewBlkGate(MapX/2, MapY/2);
	                 break;
                case MAPEDIT_MODE_CASTLE:
	                 MapEdit_PutCastle(MapX, MapY);
	                 break; 
                case MAPEDIT_MODE_CASTLE_GATE_LEFT:
	                 Map.PutCastleGate(MapX, MapY, 0, TRUE);
	                 break; 
                case MAPEDIT_MODE_CASTLE_GATE_RIGHT:
	                 Map.PutCastleGate(MapX, MapY, 1, TRUE);
	                 break; 
                case MAPEDIT_MODE_TILE:
					{
						Map.PutTile_Daesang( MapX, MapY , Tile);
					}
	                 break; 
                case MAPEDIT_MODE_RICE:
					Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_RICE, -1);
	                 break; 
                case MAPEDIT_MODE_POTATO:
   	                 Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_POTATO, -1);
	                 break; 

                case MAPEDIT_MODE_TREE:
					 Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_TREE, Tree);
	                 break; 

                case MAPEDIT_MODE_BAMBOO:
						Map.SetCropRandom(MapX, MapY, 0, 100, CROP_TYPE_BAMBOO, -1);
	                 break; 

                case MAPEDIT_MODE_RUIN:
					 Map.PutRuin(MapX, MapY, Ruin,  1);
	                 break; 

				case MAPEDIT_MODE_SHIPPATROL:
					// 가까운 지점에 이미 있다면 넣을 수 없다. 
					ableswitch=TRUE;

					for(i=0;i<MAX_SHIP_PATROL_POINT_NUMBER;i++)
					{
						// 가까이에 이미 있다면, 
						if(Map.ShipPatrolPoint[i].x!=-1 && Map.ShipPatrolPoint[i].y!=-1 
						&& max( abs(Map.ShipPatrolPoint[i].x-MapX), 
						        abs(Map.ShipPatrolPoint[i].y-MapY))<2 )
						{
							ableswitch=FALSE;
						}
					}

					if(ableswitch==TRUE)
					{
					   PushShipPatrol(MapX, MapY);
					}
					break;

				case MAPEDIT_MODE_SHIPLANDING:
					// 가까운 지점에 이미 있다면 넣을 수 없다. 
					ableswitch=TRUE;

					for(i=0;i<MAX_SHIP_LANDING_POINT_NUMBER;i++)
					{
						// 가까이에 이미 있다면, 
						if(Map.ShipLandingPoint[i].x!=-1 && Map.ShipLandingPoint[i].y!=-1 
						&& max( abs(Map.ShipLandingPoint[i].x-MapX), 
						        abs(Map.ShipLandingPoint[i].y-MapY))<2)
						{
							ableswitch=FALSE;
						}
					}

					if(ableswitch==TRUE)
 					    PushShipLanding(MapX, MapY);
					break;

                case MAPEDIT_MODE_UNIT:
					// 플레이어 시작지점이면, 
					if(SelectedKind==KIND_PLAYERSTART)
					{
						Map.clMapPlayerInfo[PlayerNumber].StartPoint.x=MapX;
						Map.clMapPlayerInfo[PlayerNumber].StartPoint.y=MapY;
					}
					else
					{
						for(i=1;i<MAX_MAP_CHAR_NUMBER;i++)
						{
							if(Map.clMapUnitInfo[i].Kind == 0)
							{

								// 선택된 자리에 다른 캐릭터가 있어서는 안된다. 
								SHORT locateswitch=TRUE;
								for( j=0; j<KI[SelectedKind].IDYsize; j++ )
									for( k=0; k<KI[SelectedKind].IDXsize; k++ )
									{
									   // 뭔가가 있다면, 
									   if( (KI[SelectedKind].m_uiAtb&ATB_FLY) )
									   {
										   id = clifMap.GetSkyMap(MapX-KI[SelectedKind].IDXsize/2+k, 
											   MapY-KI[SelectedKind].IDYsize/2+j);
									   }
									   else
									   {
										   id = Map.GetIDMap(MapX-KI[SelectedKind].IDXsize/2+k, 
											   MapY-KI[SelectedKind].IDYsize/2+j);
									   }
									   
									   if( id )		locateswitch = FALSE;
								   }

								if(locateswitch==TRUE)
							   {

						           SetChar(SelectedKind, i, MapX, MapY, SOUTH, 100, PlayerNumber);
						           Map.clMapUnitInfo[i].Kind=SelectedKind;
						           Map.clMapUnitInfo[i].X=MapX;
						           Map.clMapUnitInfo[i].Y=MapY;
						           Map.clMapUnitInfo[i].Pnumber=PlayerNumber;

								   
							   }
							   break;
						   }
					   }
					}
		                 
		                 break; 
				}

			}
			else if(IpD.RightPressSwitch==TRUE)
			{
				switch(TerrainMode)
				{
                case MAPEDIT_MODE_SHALLOW:
	                 Map.PutShallow(MapX, MapY, FALSE);
	                 break;

                case MAPEDIT_MODE_NEWBLK:
	                 MapEdit_DeleteNewBlk(MapX, MapY);
	                 break;

                case MAPEDIT_MODE_CASTLE:
	                 MapEdit_DeleteCastle(MapX, MapY);
	                 break; 

                case MAPEDIT_MODE_CASTLE_GATE_LEFT:
	                 Map.PutCastleGate(MapX, MapY, 0, FALSE);
	                 break; 
                case MAPEDIT_MODE_CASTLE_GATE_RIGHT:
	                 Map.PutCastleGate(MapX, MapY, 0, FALSE);
	                 break; 
										 
				case MAPEDIT_MODE_SEA:
						 MapEdit_PutSea(MapX, MapY, FALSE);
					 break;

				case MAPEDIT_MODE_UNIT:
				// 플레이어 시작지점이면, 
				if(SelectedKind==KIND_PLAYERSTART)
				{
					Map.clMapPlayerInfo[PlayerNumber].StartPoint.x=-1;
					Map.clMapPlayerInfo[PlayerNumber].StartPoint.y=-1;
				}
				else
				{

				   // 공중 속성이라면
					if(KI[SelectedKind].m_uiAtb&ATB_FLY)
					{
						id = clifMap.GetSkyMap(MapX, MapY);
					}
					else
					{
						id=Map.GetIDMap(MapX, MapY);
					}

				   if(id)
				   {
					   clifChar.DeleteID(id);

					   DeleteChar(id);

					   Map.clMapUnitInfo[id].Kind=0;
					   Map.clMapUnitInfo[id].X=0;
					   Map.clMapUnitInfo[id].Y=0;
					   Map.clMapUnitInfo[id].Pnumber=0;
				   }
				}

					break;
				case MAPEDIT_MODE_TREE:
					Map.PutCrop(MapX,   MapY, 0, Tree);
					Map.PutTree(MapX,   MapY, 0, Tree);
					break;
	            case MAPEDIT_MODE_POTATO:
				case MAPEDIT_MODE_RICE:				
				case MAPEDIT_MODE_BAMBOO:					
		            Map.PutCrop(MapX,   MapY, 0, -1);
					Map.PutTree(MapX,   MapY, 0, -1);
		            break; 

                case MAPEDIT_MODE_RUIN:
					Map.PutRuin(MapX, MapY, 0,  0);

				}

			}
		}
}


// 배순찰지점을 추가한다.
void _MapEdit::PushShipPatrol(SHORT x, SHORT y)
{

	SHORT i;

	for(i=0;i<MAX_SHIP_PATROL_POINT_NUMBER;i++)
	{
		// 비어있으면 넣는다. 
	    if(Map.ShipPatrolPoint[i].x<0 && Map.ShipPatrolPoint[i].y<0)
		{
			Map.ShipPatrolPoint[i].x=x;
			Map.ShipPatrolPoint[i].y=y;
			return ;
		}
	}
}

// 배 상륙지점을 추가한다. 
void _MapEdit::PushShipLanding(SHORT x, SHORT y)
{
	SHORT i;

	for(i=0;i<MAX_SHIP_LANDING_POINT_NUMBER;i++)
	{
		// 비어있으면 넣는다. 
	    if(Map.ShipLandingPoint[i].x<0 && Map.ShipLandingPoint[i].y<0)
		{
			Map.ShipLandingPoint[i].x=x;
			Map.ShipLandingPoint[i].y=y;
			return ;
		}
	}

}

void _MapEdit::MapEdit_PutLevel(SHORT x, SHORT y, SHORT level, SHORT pushmode)
{
	Map.DeleteLevel(x, y, level);
	
	// 얕은 물은 지운다. 
	if( Map.GetSeaMode(x, y) == FALSE )
	    Map.PutShallow(x, y, FALSE);

	Map.PutLevel(x, y, level, pushmode);

}


void _MapEdit::MapEdit_PutSea(SHORT x, SHORT y, SHORT mode)
{
	Map.PutSea_Daesang(x, y, mode);
}


void _MapEdit::MapEdit_PutWater(SHORT x, SHORT y)
{
	

}

void _MapEdit::MapEdit_PutNewBlk(SHORT x, SHORT y)
{
	
	Map.PutNewBlk(x, y, TRUE);

}

void _MapEdit::MapEdit_DeleteNewBlk(SHORT x, SHORT y)
{
	
	Map.PutNewBlk(x, y, FALSE);

}


void _MapEdit::MapEdit_PutCastle(SHORT x, SHORT y)
{
	
	Map.PutCastle(x, y, TRUE);

}

void _MapEdit::MapEdit_DeleteCastle(SHORT x, SHORT y)
{
	
	Map.PutCastle(x, y, FALSE);

}

void _MapEdit::MapEdit_PutNewBlkGate(SHORT x, SHORT y)
{

	Map.PutNewBlkGate(x, y);
}


// 지도 정보를 표시한다. 
void _MapEdit::MapEdit_DrawInfo()
{
	HDC hdc;
	SI32 i;
	char buffer[FILE_NAME_SIZE];

	if(lpScreenBuffer->GetDC(&hdc)==DD_OK)
	{
         
	     SelectObject(hdc, pGame->pBattle->HelpFont);
         SetBkColor( hdc, RGB( 0, 0, 0 ) );
         SetTextColor( hdc, RGB( 180, 180, 180 ) );
   	     SetBkMode(hdc, TRANSPARENT);

		 // 게임에 참여할 수 있는 사용자의 수가 몇인지 파악한다. 
		 SI32 playercount = 0;
		 for(i = 0;i < MAX_MULTI_PLAYER;i++)
		 {
			 if(Map.clMapPlayerInfo[i].StartPoint.x>=0 && Map.clMapPlayerInfo[i].StartPoint.y>=0)
			 {
				 playercount++;
			 }
		 }
		 
	     wsprintf(buffer, "Selected Unit:%s X:%d Y:%d Start Number:%d", KI[MapEdit.SelectedKind].Name, MapX, MapY, playercount);

	     TextOut(hdc, 200, 430, buffer, lstrlen(buffer));

         lpScreenBuffer->ReleaseDC(hdc);
	   }
}



BOOL CALLBACK SelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	char buffer[FILE_NAME_SIZE];
	HWND hwnd;
	SHORT i;
	SHORT hindex=0;
	NMHDR * pnmhdr;
	static TV_ITEM tvi;
	static TV_INSERTSTRUCT tvis;
	static HTREEITEM nationhtv[3];
	
	static HTREEITEM koreahtv[2];
	static HTREEITEM korbuild[20];
	static HTREEITEM korchar[20];

	static HTREEITEM japanhtv[2];
	static HTREEITEM jpnbuild[20];
	static HTREEITEM jpnchar[20];

	static HTREEITEM chinahtv[2];
	static HTREEITEM chnbuild[20];
	static HTREEITEM chnchar[20];

	


	switch(iMsg)
	{
	case WM_INITDIALOG:

		// 지형 정보 
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 0, (LPARAM)"Level");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 1, (LPARAM)"Sea");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 2, (LPARAM)"Shallow");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 3, (LPARAM)"New Block");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 4, (LPARAM)"New Block Gate");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 5, (LPARAM)"Castle");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 6, (LPARAM)"Castle Gate(left)");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 7, (LPARAM)"Castle Gate(right)");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 8, (LPARAM)"Tile");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING, 9, (LPARAM)"Rice");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,10, (LPARAM)"Potato");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,11, (LPARAM)"Tree");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,12, (LPARAM)"Bamboo");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,13, (LPARAM)"Ruin");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,14, (LPARAM)"Ship Patrol");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,15, (LPARAM)"Ship Landing");
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_INSERTSTRING,16, (LPARAM)"Unit");

		// 디폴트 선택 
		SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_SETCURSEL, MapEdit.TerrainMode, 0);

		// 플레이어 번호 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 0, (LPARAM)"1");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 1, (LPARAM)"2");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 2, (LPARAM)"3");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 3, (LPARAM)"4");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 4, (LPARAM)"5");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 5, (LPARAM)"6");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 6, (LPARAM)"7");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_INSERTSTRING, 7, (LPARAM)"8");

		// 디폴트 선택 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_SETCURSEL, MapEdit.PlayerNumber, 0);

		// 국가 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_NO,     (LPARAM)"None");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_KOREA,  (LPARAM)"Korea");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_JAPAN,  (LPARAM)"Japan");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_CHINA,  (LPARAM)"China");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_INSERTSTRING, COUNTRY_NEUTRAL,(LPARAM)"Neutral");

		// 호스트 
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST), CB_INSERTSTRING, BY_PLAYER,     (LPARAM)"Player");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST), CB_INSERTSTRING, BY_COMPUTER,  (LPARAM)"Computer");

		// 그룹
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 0,     (LPARAM)"1");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 1,     (LPARAM)"2");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 2,     (LPARAM)"3");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 3,     (LPARAM)"4");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 4,     (LPARAM)"5");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 5,     (LPARAM)"6");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 6,     (LPARAM)"7");
		SendMessage(GetDlgItem(hDlg, IDC_PLAYER_GROUP), CB_INSERTSTRING, 7,     (LPARAM)"8");

		// 지형 지물 
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 0,     (LPARAM)"건물잔해1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 1,     (LPARAM)"건물잔해2");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 2,     (LPARAM)"발석거잔해");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 3,     (LPARAM)"장승");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 4,     (LPARAM)"무덤1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 5,     (LPARAM)"부러진 거목");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 6,     (LPARAM)"성황당");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 7,     (LPARAM)"우물");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 8,     (LPARAM)"거북 비석");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 9,     (LPARAM)"거석1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 10,     (LPARAM)"거석2");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 11,     (LPARAM)"화계사");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 12,     (LPARAM)"이정표");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 13,     (LPARAM)"조선깃발1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 14,     (LPARAM)"명깃발1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 15,    (LPARAM)"일본깃발1");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 16,    (LPARAM)"삼성 이정표");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 17,    (LPARAM)"삼성 바닥");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 18,    (LPARAM)"삼성 전화기");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 19,    (LPARAM)"화산");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 20,    (LPARAM)"성곽망루");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 21,    (LPARAM)"무너진 성곽");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 22,    (LPARAM)"성문");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 23,    (LPARAM)"성곽 타일");
		SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_INSERTSTRING, 24,    (LPARAM)"큰 성문");

		// 나무 타입..
		{
		for( int kkk = 0; kkk < FV.m_siTreeFileNo; kkk++)
		{			
			SendMessage(GetDlgItem(hDlg, IDC_TREE_SELECT), CB_ADDSTRING, kkk,     (LPARAM)FV.m_TreeImgFile[kkk].FileName );
		}
		}

		
		// 타일 타입(눈...)
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 0,    (LPARAM)"기본 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 1,    (LPARAM)"눈   타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 2,    (LPARAM)"갈색 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 3,    (LPARAM)"녹색 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 4,    (LPARAM)"척박지 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 5,    (LPARAM)"화산 타일");
		SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_INSERTSTRING, 6,    (LPARAM)"봄 타일");

				//대상 전투맵 타일용 메뉴..
		//타일의 종류를 추가한다.
		//타일의 종류는 텍스트로 기록한다..
		//..하드코딩 -_-b


		// 높이 정보 
		if(MapEdit.Level==0)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL0), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL0), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==1)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL1), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL1), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==2)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL2), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL2), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==3)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL3), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL3), BM_SETCHECK, 0, 0);

		if(MapEdit.Level==4)
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL4), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_LEVEL4), BM_SETCHECK, 0, 0);

		// 타일 정보 
		if(MapEdit.Tile==TILE_DEFAULT)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_NORMAL), BM_SETCHECK, 1, 0);
		else 
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_NORMAL), BM_SETCHECK, 0, 0);

		if(MapEdit.Tile==TILE_GRASS)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_GREEN), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_GREEN), BM_SETCHECK, 0, 0);

		if(MapEdit.Tile==TILE_GREEN)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_RED), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_RED), BM_SETCHECK, 0, 0);

		if(MapEdit.Tile==TILE_CROPAREA)
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_CROPAREA), BM_SETCHECK, 1, 0);
		else
		   SendMessage(GetDlgItem(hDlg, IDC_TILE_CROPAREA), BM_SETCHECK, 0, 0);

		// 지도 정보 
        hwnd=GetDlgItem(hDlg, IDC_MAP_XSIZE);
		wsprintf(buffer, "%d", Map.MH.MapXsize);
		SetWindowText(hwnd, buffer);

        hwnd=GetDlgItem(hDlg, IDC_MAP_YSIZE);
		wsprintf(buffer, "%d", Map.MH.MapYsize);
		SetWindowText(hwnd, buffer);

		// 유니트 
		tvis.hInsertAfter=TVI_LAST;
	    tvi.mask=TVIF_TEXT|TVIF_PARAM; 

		// 플레이어 시작위치  
		tvi.pszText="Player Start";
		tvi.lParam=KIND_PLAYERSTART;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[0]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
	
		// 조선 
		tvi.pszText="Korea";
		tvi.lParam=0;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[0]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		// 조선 캐릭터 
		tvi.pszText="Character";
		tvi.lParam=0;
		tvis.hParent=nationhtv[0];
		tvis.item=tvi;
		koreahtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		hindex=0;
		for(i=0;i<KIND_ON_MAX;i++)
		{
			if(KI[i].Nation==COUNTRY_KOREA && i <= 111) // 온라인 캐릭터 안나오게 임시로.
			{
		       tvi.pszText=KI[i].Name;
			   tvi.lParam=i;
		       tvis.hParent=koreahtv[1];
		       tvis.item=tvi;
		       korchar[hindex]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
			   hindex++;
			}
		}


		// 일본 
		tvi.pszText="Japan";
		tvi.lParam=0;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		tvi.pszText="Character";
		tvi.lParam=0;
		tvis.hParent=nationhtv[1];
		tvis.item=tvi;
		japanhtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		hindex=0;
		for(i=0;i<KIND_ON_MAX;i++)
		{
			if(KI[i].Nation==COUNTRY_JAPAN)
			{
		       tvi.pszText=KI[i].Name;
			   tvi.lParam=i;
		       tvis.hParent=japanhtv[1];
		       tvis.item=tvi;
		       jpnchar[hindex]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
			   hindex++;
			}
		}

		// 중국 
		tvi.pszText="China";
		tvi.lParam=0;
		tvis.hParent=TVI_ROOT;
		tvis.item=tvi;
		nationhtv[2]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		tvi.pszText="Character";
		tvi.lParam=0;
		tvis.hParent=nationhtv[2];
		tvis.item=tvi;
		chinahtv[1]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);

		hindex=0;
		for(i=0;i<KIND_ON_MAX;i++)
		{
			if(KI[i].Nation==COUNTRY_CHINA )
			{
		       tvi.pszText=KI[i].Name;
			   tvi.lParam=i;
		       tvis.hParent=chinahtv[1];
		       tvis.item=tvi;
		       chnchar[hindex]=TreeView_InsertItem(GetDlgItem(hDlg, IDC_UNIT), &tvis);
			   hindex++;
			}
		}


    	return TRUE;

	case WM_NOTIFY:

		pnmhdr=(LPNMHDR)lParam;

			if(pnmhdr->code==TVN_SELCHANGED)
			{
		 	    tvi.mask=TVIF_PARAM;
			    tvi.hItem=((LPNM_TREEVIEW)pnmhdr)->itemNew.hItem;

			    if(TreeView_GetItem(GetDlgItem(hDlg, IDC_UNIT), &tvi)==TRUE)
				{
			       // 선택된 캐릭터를 찾는다. 
			       for(i=1;i<KIND_ON_MAX;i++)
				   {
				       if(tvi.lParam==i)
					   { 
					       MapEdit.SelectedKind=i;
						   
						   // 선택된 캐릭터의 개수를 표시한다. 
						   hwnd=GetDlgItem(hDlg, IDC_UNIT_NUMBER);
		                   wsprintf(buffer, "%d", GD.CharNumber[MapEdit.PlayerNumber][i]);
						   SetWindowText(hwnd, buffer);

						   //선택된 캐릭터의 최대 개수를 표시한다. 
						   hwnd=GetDlgItem(hDlg, IDC_MAX_UNIT_NUMBER);
		                   wsprintf(buffer, "%d", Map.clMapPlayerInfo[MapEdit.PlayerNumber].MaxUnitNumber[i]);
		                   SetWindowText(hwnd, buffer);

						   // 게임에 등장하는지 여부를 알려준다. 
						   if(Map.clMapPlayerInfo[MapEdit.PlayerNumber].IsCharInGame[i]==TRUE)
						      SendMessage(GetDlgItem(hDlg, IDC_INGAME), BM_SETCHECK, 1, 0);
						   else
							  SendMessage(GetDlgItem(hDlg, IDC_INGAME), BM_SETCHECK, 0, 0);

					   }
				   }
				}
			}
		break;

	case WM_COMMAND:

		switch(LOWORD(wParam))
		{
		case IDOK:

			switch(SendMessage(GetDlgItem(hDlg, IDC_TERRAIN), CB_GETCURSEL, 0, 0))
			{
			case 0: MapEdit.TerrainMode=MAPEDIT_MODE_LEVEL; break;
			case 1: MapEdit.TerrainMode=MAPEDIT_MODE_SEA;   break;
			case 2: MapEdit.TerrainMode=MAPEDIT_MODE_SHALLOW;   break;
			case 3: MapEdit.TerrainMode=MAPEDIT_MODE_NEWBLK;break;
			case 4: MapEdit.TerrainMode=MAPEDIT_MODE_NEWBLKGATE; break;
			case 5: MapEdit.TerrainMode=MAPEDIT_MODE_CASTLE; break;
			case 6: MapEdit.TerrainMode=MAPEDIT_MODE_CASTLE_GATE_LEFT; break;
			case 7: MapEdit.TerrainMode=MAPEDIT_MODE_CASTLE_GATE_RIGHT; break;
			case 8: MapEdit.TerrainMode=MAPEDIT_MODE_TILE;  break;
			case 9: MapEdit.TerrainMode=MAPEDIT_MODE_RICE;  break;
			case 10: MapEdit.TerrainMode=MAPEDIT_MODE_POTATO;break;
			case 11: MapEdit.TerrainMode=MAPEDIT_MODE_TREE;  break;
			case 12: MapEdit.TerrainMode=MAPEDIT_MODE_BAMBOO;  break;
			case 13: MapEdit.TerrainMode=MAPEDIT_MODE_RUIN;  break;
			case 14:MapEdit.TerrainMode=MAPEDIT_MODE_SHIPPATROL;break;
			case 15:MapEdit.TerrainMode=MAPEDIT_MODE_SHIPLANDING;break;
			case 16:MapEdit.TerrainMode=MAPEDIT_MODE_UNIT;  break;
			}

			switch(SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_GETCURSEL, 0, 0))
			{
			case 0: MapEdit.PlayerNumber=0; break;
			case 1: MapEdit.PlayerNumber=1; break;
			case 2: MapEdit.PlayerNumber=2; break;
			case 3: MapEdit.PlayerNumber=3; break;
			case 4: MapEdit.PlayerNumber=4; break;
			case 5: MapEdit.PlayerNumber=5; break;
			case 6: MapEdit.PlayerNumber=6; break;
			case 7: MapEdit.PlayerNumber=7; break;
			}


			switch(SendMessage(GetDlgItem(hDlg, IDC_RUIN), CB_GETCURSEL, 0, 0))
			{
			case 0: MapEdit.Ruin=CROP_TYPE_RUIN0; break;
			case 1: MapEdit.Ruin=CROP_TYPE_RUIN1; break;
			case 2: MapEdit.Ruin=CROP_TYPE_RUIN2; break;
			case 3: MapEdit.Ruin=CROP_TYPE_DOLL1; break;
			case 4: MapEdit.Ruin=CROP_TYPE_TOMB1; break;
			case 5: MapEdit.Ruin=CROP_TYPE_BROKENTREE1; break;
			case 6: MapEdit.Ruin=CROP_TYPE_ODDTREE1; break;
			case 7: MapEdit.Ruin=CROP_TYPE_WELL1;  break;
			case 8: MapEdit.Ruin=CROP_TYPE_TUTTLE1;  break;
			case 9: MapEdit.Ruin=CROP_TYPE_STONE1;  break;
			case 10: MapEdit.Ruin=CROP_TYPE_STONE2;  break;
			case 11: MapEdit.Ruin=CROP_TYPE_TEMPLE;  break;
			case 12: MapEdit.Ruin=CROP_TYPE_SIGN1;  break;
			case 13: MapEdit.Ruin=CROP_TYPE_FLAGK1; break;
			case 14: MapEdit.Ruin=CROP_TYPE_FLAGC1; break;
			case 15:MapEdit.Ruin=CROP_TYPE_FLAGJ1; break;
            case 16: MapEdit.Ruin=CROP_TYPE_AD11;  break;
			case 17: MapEdit.Ruin=CROP_TYPE_AD12;  break;
			case 18: MapEdit.Ruin=CROP_TYPE_AD13;  break;
			case 19: MapEdit.Ruin=CROP_TYPE_VOLCANO; break;
			case 20: MapEdit.Ruin=CROP_TYPE_CASTLETOWER;  break;
			case 21: MapEdit.Ruin=CROP_TYPE_DESTROYEDCASTLE;  break;
			case 22: MapEdit.Ruin=CROP_TYPE_CASTLEGATE;  break;
			case 23: MapEdit.Ruin=CROP_TYPE_CASTLETILE;  break;
			case 24: MapEdit.Ruin=CROP_TYPE_CASTLELARGEGATE;  break;
			}

			//Tree
			{
			int SelectedTree = SendMessage(GetDlgItem(hDlg, IDC_TREE_SELECT), CB_GETCURSEL, 0, 0);
			if( SelectedTree != -1)
			{
				MapEdit.Tree = SelectedTree;
				FV.SetNowFile(SelectedTree);
			}
			}

			// 높이 
			hwnd=GetDlgItem(hDlg, IDC_LEVEL0);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=0;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL1);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=1;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL2);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=2;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL3);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=3;
			}

			hwnd=GetDlgItem(hDlg, IDC_LEVEL4);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Level=4;
			}

			// 타일 
			hwnd=GetDlgItem(hDlg, IDC_TILE_NORMAL);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_DEFAULT;
			}
			hwnd=GetDlgItem(hDlg, IDC_TILE_GREEN);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_GRASS;
			}
			hwnd=GetDlgItem(hDlg, IDC_TILE_RED);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_GREEN;
			}
			hwnd=GetDlgItem(hDlg, IDC_TILE_CROPAREA);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				MapEdit.Tile=TILE_CROPAREA;
			}

			// 지도 정보
			hwnd=GetDlgItem(hDlg, IDC_NEWMAP);
			if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			{
				hwnd=GetDlgItem(hDlg, IDC_MAP_XSIZE);
			    GetWindowText(hwnd, buffer, 10);
			    SHORT xsize=atoi(buffer);

                hwnd=GetDlgItem(hDlg, IDC_MAP_YSIZE);
			    GetWindowText(hwnd, buffer, 10);
			    SHORT ysize=atoi(buffer);

				EndGame();
				
				LoadMapTile(Map.MH.MapType);

				// 지도를 초기화한다. 
				Map.InitMap(xsize, ysize, FALSE);
				Map.MakeLastMap();
				SMap.Init(xsize, ysize);
			}



		    EndDialog(hDlg, 0);
		   return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case IDC_MAX_UNIT_NUMBER:
			// 캐릭터의 최대 개수를 지정한다. 
			if(HIWORD(wParam)==EN_CHANGE)
			{
				hwnd=GetDlgItem(hDlg, IDC_MAX_UNIT_NUMBER);
			    GetWindowText(hwnd, buffer, 10);
			    SHORT maxnumber=atoi(buffer);

				Map.clMapPlayerInfo[MapEdit.PlayerNumber].MaxUnitNumber[MapEdit.SelectedKind]=maxnumber;

			}
			break;

       case IDC_INGAME:
		   //if(HIWORD(wParam)==CN_CHANGE)
		   {
			  hwnd=GetDlgItem(hDlg, IDC_INGAME);
			  if(SendMessage(hwnd, BM_GETCHECK, 0, 0))
			  {
				  Map.clMapPlayerInfo[MapEdit.PlayerNumber].IsCharInGame[MapEdit.SelectedKind]=TRUE;
			  }
			  else
				  Map.clMapPlayerInfo[MapEdit.PlayerNumber].IsCharInGame[MapEdit.SelectedKind]=FALSE;
		   }

		   break;

		case IDC_PLAYERNUMBER:
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				MapEdit.PlayerNumber=(SHORT)SendMessage(GetDlgItem(hDlg, IDC_PLAYERNUMBER), CB_GETCURSEL, 0, 0);


				// 새로운 플레이어에 맞는 설정으로 바꾼다.
		        SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_SETCURSEL, Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Country, 0);
				SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST),    CB_SETCURSEL, Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Host   , 0);


				// 자원 표시 
				hwnd=GetDlgItem(hDlg, IDC_CROP);
		        wsprintf(buffer, "%d", Map.clMapPlayerInfo[MapEdit.PlayerNumber].Money);
		        SetWindowText(hwnd, buffer);

				hwnd=GetDlgItem(hDlg, IDC_TREE);
		        wsprintf(buffer, "%d", Map.clMapPlayerInfo[MapEdit.PlayerNumber].Tree);
		        SetWindowText(hwnd, buffer);

			}
			break;

		case IDC_PLAYER_COUNTRY:
			// 국가가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Country = (UI08)SendMessage(GetDlgItem(hDlg, IDC_PLAYER_COUNTRY), CB_GETCURSEL, 0, 0);
			}
			break;

		case IDC_PLAYER_HOST:
			// 주체가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				Map.clMapPlayerInfo[MapEdit.PlayerNumber].PI.Host=(UI08)SendMessage(GetDlgItem(hDlg, IDC_PLAYER_HOST), CB_GETCURSEL, 0, 0);
			}
			break;

		case IDC_CROP:
			hwnd=GetDlgItem(hDlg, IDC_CROP);
		    GetWindowText(hwnd, buffer, 10);
		    Map.clMapPlayerInfo[MapEdit.PlayerNumber].Money=atoi(buffer);
			break;

		case IDC_TREE:
			hwnd=GetDlgItem(hDlg, IDC_TREE);
		    GetWindowText(hwnd, buffer, 10);
		    Map.clMapPlayerInfo[MapEdit.PlayerNumber].Tree=atoi(buffer);
			break;

		case IDC_PLAYER_GROUP:
			// 주체가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{

			}
			break;

		case IDC_MAPTYPE:

			// 주체가 변경되면, 
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{

				Map.MH.MapType= (SHORT)SendMessage(GetDlgItem(hDlg, IDC_MAPTYPE), CB_GETCURSEL, 0, 0);

				FreeMapTile();
				LoadMapTile(Map.MH.MapType);

			}
			break;


		}
		break;
	}
	return FALSE;
}
#endif