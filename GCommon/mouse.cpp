//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <mouse.h>
#include <clGame.h>

#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <CharOrg.h>

#include "charanimation.h"
#include <kindinfo.h>
#include <etc.h>
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <map.h>
#include <mapInterface.h>
#include <charinterface.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;
extern	SHORT 				GameMenuShowSwitch;					// 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	int 				MouseMode;							// 마우스가 처한 상태를 저장 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_NewID 				UnderMouseID;
extern	int 				MenuMode;							// 메뉴의 상태를 저장 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				MouseHarvestX, MouseHarvestY;
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	int 				ControlKeyPressSwitch;
extern	_InputDevice		IpD;
extern	_Char				CR[];								// 캐릭터의 포인터 

extern	SHORT				AKey[];
extern clock_t				CurrentClock;

extern _clGame *pGame;

extern int  WindowModeSwitch;
_InputDevice IpD;

SHORT	OldDrawMouseX, OldDrawMouseY;
SHORT	DrawMouseX, DrawMouseY;

SHORT	MouseHotSpotX, MouseHotSpotY;

SHORT	MouseType;       // 마우스가 어떤 유형의 것인가?
SHORT	MouseFont;
SHORT	DrawMouseFont;   // 실제로 그려지는 마우스 커서 
SHORT	MouseStep=1;

UI08	MouseBuffer[60*60];

BOOL   _InputDevice::IsDoubleClick()
{

	// 더블 클릭인지 확인한다.
	// 이전의 클릭과의 시간 간격을 체크한다.
	if(abs(PrevLeftUp1Clock - PrevLeftUp2Clock) < 300  
	&& abs(CurrentClock - PrevLeftUp1Clock ) < 300)
	{
		return TRUE;
	}

	return FALSE;
}


void SetMouse(SHORT mousemode)
{
	switch(mousemode)
	{
	case MOUSE_RIDE:
		MouseType=mousemode;
		MouseFont=69;
		MouseStep=4;
		MouseHotSpotX=9;
		MouseHotSpotY=26;
		break;

	case MOUSE_MAGIC:
		MouseType=mousemode;
		MouseFont=100;
		MouseStep=0;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_ATTACK:
		MouseType=mousemode;
		MouseFont=60;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;

		break;
	case MOUSE_NORMAL:
		MouseType=mousemode;
		MouseFont=80;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;

		break;

	case MOUSE_RESULT:
		MouseType=mousemode;
		MouseFont=183;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

/*	case MOUSE_RESULT:
		MouseType=mousemode;
		MouseFont=126;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;
*/


	case MOUSE_INITMENU:
		MouseType=mousemode;
		MouseFont=180;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

/*	case MOUSE_INITMENU:
		MouseType=mousemode;
		MouseFont=120;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;
*/

	case MOUSE_CHOOSEMISSION:
		MouseType=mousemode;
		MouseFont=186;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_SETMULTI:
		MouseType=mousemode;
		MouseFont=123;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_SETGAME:
		MouseType=mousemode;
		MouseFont=189;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_PROVIDER:
		MouseType=mousemode;
		MouseFont=192;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_SESSION:
		MouseType=mousemode;
		MouseFont=195;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;


	case MOUSE_CHAR:
		MouseType=mousemode;
		MouseFont=0;
		MouseStep=4;
		MouseHotSpotX=15;
		MouseHotSpotY=15;
		break;

	case MOUSE_HEAL:
		MouseType=mousemode;
		MouseFont=64;
		MouseStep=4;
		MouseHotSpotX=16;
		MouseHotSpotY=31;
		break;

	case MOUSE_HARVEST:
		MouseType=mousemode;
		MouseFont=10;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=15;
		break;

	case MOUSE_EXTINGUISH:
		MouseType=mousemode;
		MouseFont=84;
		MouseStep=14;
		MouseHotSpotX=15;
		MouseHotSpotY=15;
		break;

	case MOUSE_SCROLL_SOUTH:
		MouseType=mousemode;
		MouseFont=20;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_SW:
		MouseType=mousemode;
		MouseFont=21;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_WEST:
		MouseType=mousemode;
		MouseFont=22;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_NW:
		MouseType=mousemode;
		MouseFont=23;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NORTH:
		MouseType=mousemode;
		MouseFont=24;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NE:
		MouseType=mousemode;
		MouseFont=25;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_EAST:
		MouseType=mousemode;
		MouseFont=26;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_SE:
		MouseType=mousemode;
		MouseFont=27;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=31;
		break;

	case MOUSE_SCROLL_SOUTH_X:
		MouseType=mousemode;
		MouseFont=28;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_SW_X:
		MouseType=mousemode;
		MouseFont=29;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_WEST_X:
		MouseType=mousemode;
		MouseFont=30;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_NW_X:
		MouseType=mousemode;
		MouseFont=31;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NORTH_X:
		MouseType=mousemode;
		MouseFont=32;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NE_X:
		MouseType=mousemode;
		MouseFont=33;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_EAST_X:
		MouseType=mousemode;
		MouseFont=34;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_SE_X:
		MouseType=mousemode;
		MouseFont=35;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=31;
		break;

	}
}


void DrawMouseBuffer()
{
	int clipx1, clipy1, clipx2, clipy2;

	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipFullScreen();

	clGrp.PutImageC(OldDrawMouseX, OldDrawMouseY, 
	          GetFileXsize(FILE_GAME_MOUSE),
		      GetFileYsize(FILE_GAME_MOUSE),
		      MouseBuffer);

	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
}

// 마우스를 그려준다. 
void DrawMouse()
{
	DrawMouse(clGrp.lpDDSBack);
}



// 마우스를 그려준다. 
void DrawMouse(LPDIRECTDRAWSURFACE7 surface)
{

	int mousexsize = GetFileXsize(FILE_GAME_MOUSE);
	int mouseysize = GetFileYsize(FILE_GAME_MOUSE);
	int hotspotx=MouseHotSpotX;
	int hotspoty=MouseHotSpotY;
	int clipx1, clipy1, clipx2, clipy2;
	RECT orgrect;

	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipFullScreen();

	if(clGrp.LockSurface(surface)==TRUE)
	{

		DrawMouseX=IpD.Mouse_X-hotspotx;
		DrawMouseY=IpD.Mouse_Y-hotspoty;

		orgrect.left=OldDrawMouseX;
		orgrect.right=OldDrawMouseX+mousexsize;
		orgrect.top=OldDrawMouseY;
		orgrect.bottom=OldDrawMouseY+mouseysize;

		orgrect.left=DrawMouseX;
		orgrect.right=DrawMouseX+mousexsize;
		orgrect.top=DrawMouseY;
		orgrect.bottom=DrawMouseY+mouseysize;

		OldDrawMouseX=DrawMouseX;
		OldDrawMouseY=DrawMouseY;


		// 마우스 자리의 이미지를 가져온다. 
		clGrp.GetImageC(DrawMouseX, DrawMouseY, 
			GetFileXsize(FILE_GAME_MOUSE), 
			GetFileYsize(FILE_GAME_MOUSE),
			MouseBuffer);

		if(MouseStep<=0)
		{
			DrawMouseFont=MouseFont;
		}
		else
		{
		    DrawMouseFont=MouseFont+(GD.Frame/3)%MouseStep;


			// 기본형 마우스의 경우 마우스 버튼이 눌린 것이라면, 
			if(MouseType==MOUSE_NORMAL
			|| MouseType == MOUSE_INITMENU
			|| MouseType == MOUSE_SETMULTI
			|| MouseType == MOUSE_SETGAME
			|| MouseType == MOUSE_PROVIDER
			|| MouseType == MOUSE_SESSION
			|| MouseType == MOUSE_RESULT
			|| MouseType == MOUSE_CHOOSEMISSION)
			{
				if(IpD.LeftPressSwitch==TRUE)
				{
					DrawMouseFont=MouseFont+1;
				}
			}
		}

		Draw_PutSpriteT(DrawMouseX, DrawMouseY, FILE_GAME_MOUSE, DrawMouseFont);

		clGrp.UnlockSurface(surface);
	}
	
	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
}

// 마우스를 그려준다. 
void DrawMousePrimary()
{
	// 윈도우 모드인 경우 적용되지 않는다.
	if (WindowModeSwitch) return;

	int mousexsize=GetFileXsize(FILE_GAME_MOUSE);
	int mouseysize=GetFileYsize(FILE_GAME_MOUSE);
	int hotspotx=MouseHotSpotX;
	int hotspoty=MouseHotSpotY;
	int clipx1, clipy1, clipx2, clipy2;

	// 마우스가 움직이지 않았으면 다시 그리지 않는다. 
	if(DrawMouseX==IpD.Mouse_X-hotspotx && DrawMouseY==IpD.Mouse_Y-hotspoty)return ; 
	

	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipFullScreen();

	// Already Lock Error 때문에 막아놈
	/*
	RECT orgrect;
	if(clGrp.LockSurface(SURFACE_PRIMARY)==TRUE)
	{
		// 마우스 자리의 이미지를 가져온다. 
		clGrp.PutImageC(DrawMouseX, DrawMouseY, 
			GetFileXsize(FILE_GAME_MOUSE), 
			GetFileYsize(FILE_GAME_MOUSE),
			MouseBuffer);

		DrawMouseX = IpD.Mouse_X - hotspotx;
		DrawMouseY = IpD.Mouse_Y - hotspoty;

		orgrect.left   = OldDrawMouseX;
		orgrect.right  = OldDrawMouseX + mousexsize;
		orgrect.top    = OldDrawMouseY;
		orgrect.bottom = OldDrawMouseY + mouseysize;

		orgrect.left   = DrawMouseX;
		orgrect.right  = DrawMouseX + mousexsize;
		orgrect.top    = DrawMouseY;
		orgrect.bottom = DrawMouseY + mouseysize;

		OldDrawMouseX = DrawMouseX;
		OldDrawMouseY = DrawMouseY;


		// 마우스 자리의 이미지를 가져온다. 
		clGrp.GetImageC(DrawMouseX, DrawMouseY, 
			GetFileXsize(FILE_GAME_MOUSE), 
			GetFileYsize(FILE_GAME_MOUSE),
			MouseBuffer);

		Draw_PutSpriteT(DrawMouseX, DrawMouseY, FILE_GAME_MOUSE, DrawMouseFont);

		clGrp.UnlockSurface(SURFACE_PRIMARY);
	}
	*/

	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);

}

// mapx, mapy 주변에 있는 곡물을 찾아 가장 가까운 곡물의 위치를 harvestx, harvesty에 넣는다.
BOOL MouseModeHarvest(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT& harvestx, SHORT& harvesty)
{

	SHORT i, j;
	SHORT length;
	SHORT min_length=10000;

	for(i=-2;i<=2;i++)
		for(j=-2;j<=2;j++)
		{
			if(Map.IsInCrop(mapx+j, mapy+i, mousex, mousey, length)==TRUE)
			{
				if(length<min_length)
				{
					min_length=length;
				   harvestx=mapx+j;
				   harvesty=mapy+i;
				}
			}
		}


		if(min_length<10000)
		   return TRUE;

		return FALSE;
}

// 지도상에서 마우스의 위치와 움직임을 체크한다. 
BOOL GetMouseMode(SHORT mapx, SHORT mapy)
{
	int x=0, y=0;
	SHORT isinblackswitch=FALSE;


	// 메뉴가 불러진 상태에서는 마우스의 상태를 변경하지 않는다. 
	if(GameMenuShowSwitch==TRUE)
	{
		return FALSE;
	}

	// 마우스 모드를 초기화한다. 
	MouseMode=0;

	// 유효한 영역내에 있는가?
	if(Map.IsInMapArea(mapx, mapy)==FALSE)return FALSE;

	// 마우스가 있는 곳이 미정찰 지역이라면, 
	if(GD.BlackMap[UserPlayerNumber][mapx][mapy] == FOG2_BLACK_STEP)
	{
		isinblackswitch=TRUE;
	}
	else isinblackswitch=FALSE;


	if(IsSee(UnderMouseID.ID) && isinblackswitch==FALSE )
	{

	   // 지상의 적이면 , 
	   if((MenuMode&MENUMODE_ATTACK)
	   && IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==FALSE
	   && clifChar.GetArea(UnderMouseID.ID)==AREA_LAND)
	   {
		   MouseMode|=MOUSEMODE_ATTACK;

		   // 마우스 그림을 설정한다. 
		   SetMouse(MOUSE_ATTACK);
	   }
	   // 공중의 적이면, 
	   else if((MenuMode&MENUMODE_ATTACKAIR)
	   && IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==FALSE
	   && clifChar.GetArea(UnderMouseID.ID)==AREA_AIR)
	   {
		   MouseMode|=MOUSEMODE_ATTACK;

		   // 마우스 그림을 설정한다. 
		   SetMouse(MOUSE_ATTACK);
	   }
	   // 밑의 것이 부상당한 아군이라면, 
	   else if(MenuMode&MENUMODE_HEAL
	   && clifChar.CallCanHealGlobal(UnderMouseID)==TRUE
	   && IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==TRUE
	   && ShiftKeyPressSwitch==FALSE)
	   {
		   MouseMode|=MOUSEMODE_HEAL;
		   // 마우스 그림을 설정한다. 
		   SetMouse(MOUSE_HEAL);
	   }
	   else if(MenuMode&MENUMODE_MANA
		&& clifChar.CallCanManaGlobal(UnderMouseID)==TRUE
		&& IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==TRUE
		&& ShiftKeyPressSwitch==FALSE)
	   {
			MouseMode|=MOUSEMODE_MANA;
			//마나 
			SetMouse(MOUSE_HEAL);
	   }
	   else 
	   {
		   SetMouse(MOUSE_CHAR);
		   
		   // 자동명령 선택이 가능하다.
		   MouseMode|=MOUSEMODE_CHAR;

		   // 2버튼 방식이고 메뉴에 이동이 있다면, 
           if(pGame->Config.m_uiInterfaceMode==INTERFACEMODE_2BUTTON && (MenuMode&MENUMODE_MOVE) )
		   { 
			   // 자동명령 이동도 가능ㅎ다. 
		       MouseMode|=MOUSEMODE_MOVE;  
		   }
	   }
	}
	else
	{
		// 메뉴에 소화가 있고 불을 선택한 것이라면, 
		if((MenuMode&MENUMODE_EXTINGUISH) && Map.GetFireMap(mapx, mapy)
		&& isinblackswitch==FALSE)
		{
		    SetMouse(MOUSE_EXTINGUISH);
            MouseMode|=MOUSEMODE_EXTINGUISH;  // 소화 가능 
		}
		// 메뉴에 수확이 있고(목재)
		else if((MenuMode&MENUMODE_HARVESTTREE) && MouseModeHarvest(mapx, mapy, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE
		&& (clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_TREE || clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_BAMBOO)
		&& isinblackswitch==FALSE)
		{
		    SetMouse(MOUSE_HARVEST);
            MouseMode|=MOUSEMODE_HARVEST;  // 수확가능 
		}
		// 메뉴에 수확이 있고(곡물)
		else if((MenuMode&MENUMODE_HARVESTCROP) && MouseModeHarvest(mapx, mapy, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE
		&& ( clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_RICE || clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_POTATO)
		&& isinblackswitch==FALSE)
		{
		    SetMouse(MOUSE_HARVEST);
            MouseMode|=MOUSEMODE_HARVEST;  // 수확가능 
		}
		// 메뉴에 이동이 있고, 
		else if((MenuMode&MENUMODE_MOVE))
		{
		    MouseMode|=MOUSEMODE_MOVE;  // 마우스가 있는 지역은 빈 지역이다. 
		}
	}

	return TRUE;
}

// 지도 이외의 지역에서 마우스의 움직임을 체크한다. 


// 드래깅 된 범위안에서 캐릭터의 아이디와 갯수를 구해서 리턴한다. 
void SelectCharInDrag(_NewID* temp_id, SHORT& index, SHORT dragstartx, SHORT dragstarty, SHORT dragendx, SHORT dragendy, SHORT center_id)
{
	SHORT i;
	SHORT solo_id=0;
	SHORT multi_id=0;
	SHORT kind=0;
	SHORT id ;
	
	SHORT min_x=min(dragstartx, dragendx);
	SHORT max_x=max(dragstartx, dragendx);
	SHORT min_y=min(dragstarty, dragendy);
	SHORT max_y=max(dragstarty, dragendy);
	
	// 더블 클릭에 의한 동일 캐릭터 선택이라면, 
	// 기존의 선택된 캐릭터가 유효하면, 
	if(IsSee(center_id) && clifChar.GetPlayerNumber(center_id)==UserPlayerNumber)
	{
		// 수색할 캐릭터의 종류는, 
		kind=clifChar.GetKind(center_id);
		
		InputSelectChar(center_id, solo_id, temp_id, index);
	}
	
	for(i=0;i<GD.CharOrderIndex;i++)
	{
		
		id = GD.CharOrder[i];

		if(IsSee(id) && CR[id].m_cltCharDrawInfo.GetDrawSwitch() == TRUE
		&& GD.IsInBlackFog(clifChar.GetX(id), clifChar.GetY(id)) == FALSE)
		{
			// 캐릭터위에 있는가?
			if( clifChar.IsAbsPtInObj(id, min_x, min_y, max_x, max_y)==TRUE)
			{
				
				BOOL select_switch = TRUE;
				
				// 더블클릭에 의한 선택이고 같은 종류가 아니면, 
				// 선택하지 않는다.
				// 그러나 장수인 경우에는 다른 장수가 있을때 선택한다. 
				if(kind)
				{
					// 더블 클릭된 유닛과 종류가 같다면, 
					if(clifChar.GetKind(id) == kind)
					{
						
					}
					else
					{
						select_switch = FALSE;
					}
				}
				
				// alt 키가 눌려졌고 비무장 캐릭터라면 선택하지 않는다.
				if(IpD.AltKeyPressSwitch==TRUE &&  clifChar.IsAtb(id, ATB_ATTACK)==FALSE)
				{
					select_switch = FALSE;
				}

				// control 키가 눌렸고 공중 유닛이면 선택하지 않는다.
				else if(ControlKeyPressSwitch==TRUE && clifChar.GetArea(id)==AREA_AIR)
				{
					select_switch = FALSE;
				}


				if(select_switch == TRUE)
					InputSelectChar(id, solo_id, temp_id, index);
			}
		}
	}
	
	
	if(index==0 && solo_id)
	{
		temp_id[0]=clifChar.GetID(solo_id);
		index=1;
	}
	
}

BOOL InputSelectChar(SHORT id, SHORT &solo_id, _NewID* temp_id, SHORT& index)
{

	int k;
		   

	if(IsSee(id)==FALSE)return FALSE;
	if(CR[id].m_cltCharDrawInfo.GetDrawSwitch() == FALSE)return FALSE;

	// 내가 조정하는 군이 아니면 여러개 선택 불가 
	if(clifChar.GetPlayerNumber(id) != UserPlayerNumber)
	{
	   solo_id=id;
	}
	//자동 스위치가 설정되어 있으면 선택 불가
	else if(clifChar.GetAutoSwitch(id) == TRUE)
	{
		solo_id=id;
	}
	// 선택가능한 숫자만 선택 
	else if(index<MAX_SELECTED_CHAR_NUMBER)
	{

	   // 같은 값이 있는지 여부를 알려주는 변수 
	   int alreadyswitch=FALSE;
	   // 같은 아이디값이 있는지 확인한 후에 넣는다. 
       for(k=0;k<index;k++)
	   {
		   // 같은 값이 있다. 
		   if(temp_id[k].IsSame(clifChar.GetID(id)) )alreadyswitch=TRUE;
	   }
	   // 같은 값이 없을 때에만 넣는다. 
	   if(alreadyswitch==FALSE)
	   {
	       temp_id[index]=clifChar.GetID(id); index++;
	   }
	}
	return TRUE;
}
