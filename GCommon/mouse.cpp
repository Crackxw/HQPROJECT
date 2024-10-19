//--------------------------------------------------------------------
//  ��� : ���°�
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

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;
extern	SHORT 				GameMenuShowSwitch;					// �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	int 				MouseMode;							// ���콺�� ó�� ���¸� ���� 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_NewID 				UnderMouseID;
extern	int 				MenuMode;							// �޴��� ���¸� ���� 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameMouseX, GameMouseY;
extern	SHORT 				MouseHarvestX, MouseHarvestY;
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	int 				ControlKeyPressSwitch;
extern	_InputDevice		IpD;
extern	_Char				CR[];								// ĳ������ ������ 

extern	SHORT				AKey[];
extern clock_t				CurrentClock;

extern _clGame *pGame;

extern int  WindowModeSwitch;
_InputDevice IpD;

SHORT	OldDrawMouseX, OldDrawMouseY;
SHORT	DrawMouseX, DrawMouseY;

SHORT	MouseHotSpotX, MouseHotSpotY;

SHORT	MouseType;       // ���콺�� � ������ ���ΰ�?
SHORT	MouseFont;
SHORT	DrawMouseFont;   // ������ �׷����� ���콺 Ŀ�� 
SHORT	MouseStep=1;

UI08	MouseBuffer[60*60];

BOOL   _InputDevice::IsDoubleClick()
{

	// ���� Ŭ������ Ȯ���Ѵ�.
	// ������ Ŭ������ �ð� ������ üũ�Ѵ�.
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

// ���콺�� �׷��ش�. 
void DrawMouse()
{
	DrawMouse(clGrp.lpDDSBack);
}



// ���콺�� �׷��ش�. 
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


		// ���콺 �ڸ��� �̹����� �����´�. 
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


			// �⺻�� ���콺�� ��� ���콺 ��ư�� ���� ���̶��, 
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

// ���콺�� �׷��ش�. 
void DrawMousePrimary()
{
	// ������ ����� ��� ������� �ʴ´�.
	if (WindowModeSwitch) return;

	int mousexsize=GetFileXsize(FILE_GAME_MOUSE);
	int mouseysize=GetFileYsize(FILE_GAME_MOUSE);
	int hotspotx=MouseHotSpotX;
	int hotspoty=MouseHotSpotY;
	int clipx1, clipy1, clipx2, clipy2;

	// ���콺�� �������� �ʾ����� �ٽ� �׸��� �ʴ´�. 
	if(DrawMouseX==IpD.Mouse_X-hotspotx && DrawMouseY==IpD.Mouse_Y-hotspoty)return ; 
	

	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipFullScreen();

	// Already Lock Error ������ ���Ƴ�
	/*
	RECT orgrect;
	if(clGrp.LockSurface(SURFACE_PRIMARY)==TRUE)
	{
		// ���콺 �ڸ��� �̹����� �����´�. 
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


		// ���콺 �ڸ��� �̹����� �����´�. 
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

// mapx, mapy �ֺ��� �ִ� ��� ã�� ���� ����� ��� ��ġ�� harvestx, harvesty�� �ִ´�.
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

// �����󿡼� ���콺�� ��ġ�� �������� üũ�Ѵ�. 
BOOL GetMouseMode(SHORT mapx, SHORT mapy)
{
	int x=0, y=0;
	SHORT isinblackswitch=FALSE;


	// �޴��� �ҷ��� ���¿����� ���콺�� ���¸� �������� �ʴ´�. 
	if(GameMenuShowSwitch==TRUE)
	{
		return FALSE;
	}

	// ���콺 ��带 �ʱ�ȭ�Ѵ�. 
	MouseMode=0;

	// ��ȿ�� �������� �ִ°�?
	if(Map.IsInMapArea(mapx, mapy)==FALSE)return FALSE;

	// ���콺�� �ִ� ���� ������ �����̶��, 
	if(GD.BlackMap[UserPlayerNumber][mapx][mapy] == FOG2_BLACK_STEP)
	{
		isinblackswitch=TRUE;
	}
	else isinblackswitch=FALSE;


	if(IsSee(UnderMouseID.ID) && isinblackswitch==FALSE )
	{

	   // ������ ���̸� , 
	   if((MenuMode&MENUMODE_ATTACK)
	   && IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==FALSE
	   && clifChar.GetArea(UnderMouseID.ID)==AREA_LAND)
	   {
		   MouseMode|=MOUSEMODE_ATTACK;

		   // ���콺 �׸��� �����Ѵ�. 
		   SetMouse(MOUSE_ATTACK);
	   }
	   // ������ ���̸�, 
	   else if((MenuMode&MENUMODE_ATTACKAIR)
	   && IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==FALSE
	   && clifChar.GetArea(UnderMouseID.ID)==AREA_AIR)
	   {
		   MouseMode|=MOUSEMODE_ATTACK;

		   // ���콺 �׸��� �����Ѵ�. 
		   SetMouse(MOUSE_ATTACK);
	   }
	   // ���� ���� �λ���� �Ʊ��̶��, 
	   else if(MenuMode&MENUMODE_HEAL
	   && clifChar.CallCanHealGlobal(UnderMouseID)==TRUE
	   && IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==TRUE
	   && ShiftKeyPressSwitch==FALSE)
	   {
		   MouseMode|=MOUSEMODE_HEAL;
		   // ���콺 �׸��� �����Ѵ�. 
		   SetMouse(MOUSE_HEAL);
	   }
	   else if(MenuMode&MENUMODE_MANA
		&& clifChar.CallCanManaGlobal(UnderMouseID)==TRUE
		&& IsFriendToByPlayerNumber(UserPlayerNumber, clifChar.GetPlayerNumber(UnderMouseID.ID))==TRUE
		&& ShiftKeyPressSwitch==FALSE)
	   {
			MouseMode|=MOUSEMODE_MANA;
			//���� 
			SetMouse(MOUSE_HEAL);
	   }
	   else 
	   {
		   SetMouse(MOUSE_CHAR);
		   
		   // �ڵ���� ������ �����ϴ�.
		   MouseMode|=MOUSEMODE_CHAR;

		   // 2��ư ����̰� �޴��� �̵��� �ִٸ�, 
           if(pGame->Config.m_uiInterfaceMode==INTERFACEMODE_2BUTTON && (MenuMode&MENUMODE_MOVE) )
		   { 
			   // �ڵ���� �̵��� ���ɤ���. 
		       MouseMode|=MOUSEMODE_MOVE;  
		   }
	   }
	}
	else
	{
		// �޴��� ��ȭ�� �ְ� ���� ������ ���̶��, 
		if((MenuMode&MENUMODE_EXTINGUISH) && Map.GetFireMap(mapx, mapy)
		&& isinblackswitch==FALSE)
		{
		    SetMouse(MOUSE_EXTINGUISH);
            MouseMode|=MOUSEMODE_EXTINGUISH;  // ��ȭ ���� 
		}
		// �޴��� ��Ȯ�� �ְ�(����)
		else if((MenuMode&MENUMODE_HARVESTTREE) && MouseModeHarvest(mapx, mapy, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE
		&& (clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_TREE || clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_BAMBOO)
		&& isinblackswitch==FALSE)
		{
		    SetMouse(MOUSE_HARVEST);
            MouseMode|=MOUSEMODE_HARVEST;  // ��Ȯ���� 
		}
		// �޴��� ��Ȯ�� �ְ�(�)
		else if((MenuMode&MENUMODE_HARVESTCROP) && MouseModeHarvest(mapx, mapy, GameMouseX, GameMouseY, MouseHarvestX, MouseHarvestY)==TRUE
		&& ( clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_RICE || clifMap.GetCropTypeMap(MouseHarvestX, MouseHarvestY)==CROP_TYPE_POTATO)
		&& isinblackswitch==FALSE)
		{
		    SetMouse(MOUSE_HARVEST);
            MouseMode|=MOUSEMODE_HARVEST;  // ��Ȯ���� 
		}
		// �޴��� �̵��� �ְ�, 
		else if((MenuMode&MENUMODE_MOVE))
		{
		    MouseMode|=MOUSEMODE_MOVE;  // ���콺�� �ִ� ������ �� �����̴�. 
		}
	}

	return TRUE;
}

// ���� �̿��� �������� ���콺�� �������� üũ�Ѵ�. 


// �巡�� �� �����ȿ��� ĳ������ ���̵�� ������ ���ؼ� �����Ѵ�. 
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
	
	// ���� Ŭ���� ���� ���� ĳ���� �����̶��, 
	// ������ ���õ� ĳ���Ͱ� ��ȿ�ϸ�, 
	if(IsSee(center_id) && clifChar.GetPlayerNumber(center_id)==UserPlayerNumber)
	{
		// ������ ĳ������ ������, 
		kind=clifChar.GetKind(center_id);
		
		InputSelectChar(center_id, solo_id, temp_id, index);
	}
	
	for(i=0;i<GD.CharOrderIndex;i++)
	{
		
		id = GD.CharOrder[i];

		if(IsSee(id) && CR[id].m_cltCharDrawInfo.GetDrawSwitch() == TRUE
		&& GD.IsInBlackFog(clifChar.GetX(id), clifChar.GetY(id)) == FALSE)
		{
			// ĳ�������� �ִ°�?
			if( clifChar.IsAbsPtInObj(id, min_x, min_y, max_x, max_y)==TRUE)
			{
				
				BOOL select_switch = TRUE;
				
				// ����Ŭ���� ���� �����̰� ���� ������ �ƴϸ�, 
				// �������� �ʴ´�.
				// �׷��� ����� ��쿡�� �ٸ� ����� ������ �����Ѵ�. 
				if(kind)
				{
					// ���� Ŭ���� ���ְ� ������ ���ٸ�, 
					if(clifChar.GetKind(id) == kind)
					{
						
					}
					else
					{
						select_switch = FALSE;
					}
				}
				
				// alt Ű�� �������� ���� ĳ���Ͷ�� �������� �ʴ´�.
				if(IpD.AltKeyPressSwitch==TRUE &&  clifChar.IsAtb(id, ATB_ATTACK)==FALSE)
				{
					select_switch = FALSE;
				}

				// control Ű�� ���Ȱ� ���� �����̸� �������� �ʴ´�.
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

	// ���� �����ϴ� ���� �ƴϸ� ������ ���� �Ұ� 
	if(clifChar.GetPlayerNumber(id) != UserPlayerNumber)
	{
	   solo_id=id;
	}
	//�ڵ� ����ġ�� �����Ǿ� ������ ���� �Ұ�
	else if(clifChar.GetAutoSwitch(id) == TRUE)
	{
		solo_id=id;
	}
	// ���ð����� ���ڸ� ���� 
	else if(index<MAX_SELECTED_CHAR_NUMBER)
	{

	   // ���� ���� �ִ��� ���θ� �˷��ִ� ���� 
	   int alreadyswitch=FALSE;
	   // ���� ���̵��� �ִ��� Ȯ���� �Ŀ� �ִ´�. 
       for(k=0;k<index;k++)
	   {
		   // ���� ���� �ִ�. 
		   if(temp_id[k].IsSame(clifChar.GetID(id)) )alreadyswitch=TRUE;
	   }
	   // ���� ���� ���� ������ �ִ´�. 
	   if(alreadyswitch==FALSE)
	   {
	       temp_id[index]=clifChar.GetID(id); index++;
	   }
	}
	return TRUE;
}
