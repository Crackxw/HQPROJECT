//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <colortable.h>
#include "..\Gersang\maindata.h"
#include <etc.h>
#include "..\Gersang\screenpoint.h"
#include <chat.h>

#include "..\Gersang\mapedit.h"
#include "..\Gersang\help.h"
#include <CharOrg.h>
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <map.h>
#include <GameButton.h>


extern	_MainData			GD;
extern	_Char				CR[];
extern	_Map				Map;
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	XSPR 				PannelSpr;
extern	_NewID 				ZeroID;
extern	SHORT 				General2KBirthdaySwitch;			// 이순신 장군 탄신일 인가여부
extern	SHORT 				General4KBirthdaySwitch;			// 석가 탄신일인지 여부 
extern	SHORT 				IndependenceDaySwitch;				// 광복절인지 여부
extern	SHORT 				SoloPlaySwitch;						// 혼자 하기 모드인가 여럿이 하기 모드인가?
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_ScreenPoint 		SP;									// 화면에 사용될 여러 정보 

extern	unsigned char		SqrtVary[];							// 루트 값 
extern	DWORD				SineDegree[];						// 사인값 
// 판넬용 면 
extern	LPDIRECTDRAWSURFACE7	lpPannelSurface;
extern LPDIRECTDRAWSURFACE7		lpBriefingBuffer;
extern BOOL						bGameAllyWindowSwitch;

int NewCircle2[]= {2,   1, 2,                           2,   2, 1,                         100};
int NewCircle3[]= {3,   1, 2, 2,                        2,   2, 2, 1,                      100};
int NewCircle4[]= {4,   1, 2, 3, 3,                     3,   3, 3, 2, 1,                   100};
int NewCircle5[]= {5,   1, 2, 3, 4, 4,                  4,   4, 4, 3, 2, 1,                100};
int NewCircle6[]= {5,   2, 3, 4, 5, 5,                  5,   5, 5, 4, 3, 2,                100};
int NewCircle7[]= {6,   2, 4, 5, 6, 7, 7,               7,   7, 7, 6, 5, 4, 2,             100};
int NewCircle8[]= {7,   3, 5, 6, 7, 7, 8, 8,            8,   8, 8, 7, 7, 6, 5, 3,          100};
int NewCircle9[]= {8,   3, 5, 6, 7, 8, 8, 9, 9,         9,   9, 9, 8, 8, 7, 6, 5, 3,       100};  
int NewCircle10[]={9,   3, 5, 6, 7, 8, 8, 9, 9, 9,      9,   9, 9, 9, 8, 8, 7, 6, 5, 3,    100}; 
int NewCircle11[]={10,  3, 5, 6, 7, 8, 8, 9, 9, 10, 10, 9,  10,10, 9, 9, 8, 8, 7, 6, 5, 3, 100}; 

void BlackCheckSpecialGroupDark(int* temp, SI32 x, SI32 y, SI32 pnumber);
void BlackCheckSpecialGroupLandLight(int* temp, SI32 x, SI32 y, SI32 pnumber);
void BlackCheckSpecialGroupAirLight(int* temp, SI32 x, SI32 y, SI32 pnumber);
void BlackCheckAllGroupLight(int* temp, SI32 x, SI32 y, SI32 pnumber);
void BlackCheckAllGroupDark(int* temp, SI32 x, SI32 y, SI32 pnumber);

// 캐릭터가 살아 있는지 여부를 알려준다. 
BOOL IsAlive(int id)
{
	if(GD.CharInfo[id]==0)return FALSE;
	if(CR[id].m_ParmeterBox.IP.GetLife() <= 0)return FALSE;

	return TRUE;
}

// 캐릭터가 살아 있는지 여부를 알려준다. 
BOOL IsAlive(_NewID id)
{
	if(GD.CharInfo[id.ID]==0)return FALSE;
	if(CR[id.ID].m_ParmeterBox.IP.GetLife() <= 0)return FALSE;

	if(id.IsSame(CR[id.ID].m_NewID)==FALSE)return FALSE;
	return TRUE;
}

// 볼수 있는 캐릭터인지 여부를 알려준다. 
BOOL IsSee(int id)
{
	if(GD.CharInfo[id]==0)return FALSE;

	if(CR[id].m_ParmeterBox.IP.GetLife() <= 0)return FALSE;
   
    if(CR[id].m_scSeeSwitch==FALSE)
    {
	  return FALSE;
    }

   return TRUE;
}

// 볼수 있는 캐릭터인지 여부를 알려준다. 
BOOL IsSee(_NewID id)
{
	if(GD.CharInfo[id.ID]==0)return FALSE;

	if(CR[id.ID].m_ParmeterBox.IP.GetLife() <= 0)return FALSE;
   
    if(CR[id.ID].m_scSeeSwitch==FALSE)
    {
	  return FALSE;
    }

	if(id.IsSame(CR[id.ID].m_NewID)==FALSE)return FALSE;

   return TRUE;
}

// 선택된 모든 캐릭터를 해제한다. 
void Unselect()
{
	int i;

	

	GD.SelectedTeam=-1;

	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		if(GD.SelectedID[i].IsEmpty()==FALSE)
		{
			//if( IsAlive( GD.SelectedID[i])  )
			{
				CR[GD.SelectedID[i].ID].SetSelect(FALSE, FALSE);
			}
/*			else
			{
				Error("unselect", "id:%d", GD.SelectedID[i]);
			}
			*/
		}
	}

}

// 캐릭터들의 평균 위치를 구한다. 
BOOL GetCenterXY(_NewID* plist, SHORT charnumber, short & centerx, short& centery)
{
	SHORT i;
	SHORT number=0;
	centerx=0;
	centery=0;

	for(i=0;i<charnumber;i++)
	{
		if( IsSee(plist[i].ID) )
		{
			centerx+=CR[plist[i].ID].m_ssX;
			centery+=CR[plist[i].ID].m_ssY;
			number++;
		}
	}

	if(number<=0)return FALSE;

	centerx/=number;
	centery/=number;

	return TRUE;
}


// sx, sy와 dx,dy를 가지고 방향을 찾는다. (8방향)
SHORT DecideDirection8(DWORD sx, DWORD sy, DWORD dx, DWORD dy)
{
	 int direction=0;
	 int x_len=abs(sx-dx);
	 int y_len=abs(sy-dy);


	 if(sx==dx && sy==dy)
	 {
		return SOUTH;
	 }

	 if(x_len>y_len)
	 {
		if(dx>sx)direction|=EAST;
		else if(dx<sx)direction|=WEST;

		 if(y_len*3>x_len)
		 {
		   if(dy>sy)direction|=SOUTH;
		   else if(dy<sy)direction|=NORTH;
		 }

	 }
	 else if(x_len==y_len)
	 {

		if(dx>sx)direction|=EAST;
		else if(dx<sx)direction|=WEST;

		if(dy>sy)direction|=SOUTH;
		else if(dy<sy)direction|=NORTH;

	 }
	 else
	 {
		if(dy>sy)direction|=SOUTH;
		else if(dy<sy)direction|=NORTH;

		if(x_len*3>y_len)
		{
		   if(dx>sx)direction|=EAST;
		   else if(dx<sx)direction|=WEST;
		}
	 }

	return direction;

}


SHORT DecideDirection16(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	int delta_x, delta_y;
	int direction;
	int temp;

	if(x2>x1)delta_x=x2-x1;
	else delta_x=x1-x2;
	if(y2>y1)delta_y=y2-y1;
	else delta_y=y1-y2;

	if(delta_x*6<delta_y){temp=0;}  // x=0 graph
	else if(delta_x*2<delta_y){temp=1;}
	else if(delta_x<delta_y*2){temp=2;}
	else if(delta_x<delta_y*6){temp=3;}
	else {temp=4;}

	if(x1<x2){
		 if(y1>y2){
			if(temp==0)direction=8;
			else if(temp==1)direction=9;
			else if(temp==2)direction=10;
			else if(temp==3)direction=11;
			else if(temp==4)direction=12;
		 }
		 else{
			if(temp==0)direction=0;
			else if(temp==1)direction=15;
			else if(temp==2)direction=14;
			else if(temp==3)direction=13;
			else if(temp==4)direction=12;
		 }
	}
	else {
		if(y1>y2)
		{
			if(temp==0)direction=8;
			else if(temp==1)direction=7;
			else if(temp==2)direction=6;
			else if(temp==3)direction=5;
			else if(temp==4)direction=4;
		 }
		 else{
			if(temp==0)direction=0;
			else if(temp==1)direction=1;
			else if(temp==2)direction=2;
			else if(temp==3)direction=3;
			else if(temp==4)direction=4;
		 }
	}
	return direction;
}

SHORT DecideDirectionChar16(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

	int delta_x, delta_y;
	int direction;
	int temp;

	if(x2>x1)delta_x=x2-x1;
	else delta_x=x1-x2;
	if(y2>y1)delta_y=y2-y1;
	else delta_y=y1-y2;

	if(delta_x*6<delta_y){temp=0;}  // x=0 graph
	else if(delta_x*2<delta_y){temp=1;}
	else if(delta_x<delta_y*2){temp=2;}
	else if(delta_x<delta_y*6){temp=3;}
	else {temp=4;}

	if(x1<x2){
		 if(y1>y2){
			if(temp==0)direction=NORTH|WEST;
			else if(temp==1)direction=NNW;
			else if(temp==2)direction=NORTH;
			else if(temp==3)direction=NNE;
			else if(temp==4)direction=NORTH|EAST;
		 }
		 else{
			if(temp==0)direction=SOUTH|EAST;
			else if(temp==1)direction=EES;
			else if(temp==2)direction=EAST;
			else if(temp==3)direction=EEN;
			else if(temp==4)direction=NORTH|EAST;
		 }
	}
	else {
		if(y1>y2)
		{
			if(temp==0)direction=NORTH|WEST;
			else if(temp==1)direction=WWN;
			else if(temp==2)direction=WEST;
			else if(temp==3)direction=WWS;
			else if(temp==4)direction=SOUTH|WEST;
		 }
		 else{
			if(temp==0)direction=SOUTH|EAST;
			else if(temp==1)direction=SSE;
			else if(temp==2)direction=SOUTH;
			else if(temp==3)direction=SSW;
			else if(temp==4)direction=SOUTH|WEST;
		 }
	}
	return direction;
}


SHORT DecideDirection32(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	SHORT i;
	SHORT q=0;


	// 원점으로 보정해 준다. 
	x2=x2-x1;
	y2=y2-y1;
	x1=0;
	y1=0;


	SHORT absx2=abs(x2);
	SHORT absy2=abs(y2);


	// 어느 사분원에 존재하는가?
	if(x2>=x1)
	{
		if(y2<=y1)q=1;
		else q=2;
	}
	else 
	{
		if(y2<=y1)q=4;
		else q=3;
	}

	// 적당한 크기로 비례 조정한다. 
	if(absx2>absy2)
	{
	    if(absx2>SQRT_MAX_SIZE)
		{
		   x2=SQRT_MAX_SIZE;
		   y2=SQRT_MAX_SIZE*y2/absx2;
		}
	}
	else
	{
	    if(absy2>SQRT_MAX_SIZE)
		{
		   x2=SQRT_MAX_SIZE*x2/absy2;
		   y2=SQRT_MAX_SIZE;
		}
	}


	// sine 값을 구한다. 
	SHORT sqrtvary;
	DWORD vary=x2*x2+y2*y2;


	if(vary>=MAX_SQRT_RANGE)
	{
		sqrtvary=(SHORT)sqrt(vary);
		
		SetMessage("Message:Overflow in sqrt()");
	}
	else
	{
		sqrtvary=SqrtVary[vary];
	}

	SHORT sine= ((-x2)*1000) / sqrtvary;

	// 가장 가까운 값을 찾는다. 
	SHORT length;
	SHORT min_length=10000;
	SHORT min_dir=0;

	
	SHORT start=0, end=0;
	if(q==1)
	{
		start=16;
		end=24;
	}
	if(q==2)
	{
		start=24;
		end=31;
	}
	if(q==3)
	{
		start=0;
		end=8;

	}
	if(q==4)
	{
		start=8;
		end=16;
	}

	for(i=start;i<=end;i++)
	{
		length=abs(sine-SineDegree[i]);

		if(length<min_length)
		{
			min_length=length;
			min_dir=i;
		}
	}

	return min_dir;
}

// srcdir을 기준으로 destdir이 시계방향인가?
BOOL IsClockwiseDir16(SHORT srcdir, SHORT destdir)
{
	SHORT index=0;
	SHORT source_index=0;
	SHORT dir[]={SOUTH, SSW, SOUTH|WEST, WWS, WEST, WWN, NORTH|WEST, NNW, 
		         NORTH, NNE, NORTH|EAST, EEN, EAST, EES, SOUTH|EAST, SSE };

	// 소스의 방향을 찾는다. 
	index=0;
	while(1)
	{
		if(dir[index]==srcdir)
		{
			source_index=index;
			break;
		}
		index++;
	}

	// 시계 방향으로 찾는다. 
	index=0;
	while(1)
	{
		if(dir[(source_index+index)%16]==destdir)
		{
			break;
		}
		index++;
	}

	// 반시계 방향으로 회전 
	if(index>=8)
	{
        return FALSE;
	}
	else 
	{
		return TRUE;
	}

	return TRUE;
}

BOOL IsFriendToByPlayerNumber(SHORT player_number1, SHORT player_number2)
{

	if(IsValidPlayerNumber(player_number1) == FALSE) return FALSE;
	if(IsValidPlayerNumber(player_number2) == FALSE) return FALSE;

	if(GD.IsFriendTo(player_number1, player_number2) == TRUE)return TRUE;

    return FALSE;
}

BOOL IsFriendToByID(SHORT id1, SHORT id2)
{

	if(IsAlive(id1)==FALSE)return FALSE;
	if(IsAlive(id2)==FALSE)return FALSE;

    return IsFriendToByPlayerNumber(CR[id1].GetPlayerNumber(), CR[id2].GetPlayerNumber());

}

// 플레이어 번호가 유효한지 검사한다. 
BOOL IsValidPlayerNumber(SHORT pnumber)
{

	if(pnumber>=0 && pnumber<MAX_MULTI_PLAYER)return TRUE;

	return FALSE;
}

BOOL FindEmptyAreaGlobal(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize, SHORT dx, SHORT dy, SHORT dxsize, SHORT dysize)
{
   SHORT i, j;
   int length, min_length=10000;
   SHORT min_x, min_y;
   SHORT tempx, tempy;

	  for(i=-5;i<=5;i++)
		 for(j=-5;j<=5;j++)
		 {
			 tempx=dx+j;
			 tempy=dy+i;
				 // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			     if(Map.IsEmpty(tempx, tempy, xsize, ysize, dx, dy, dxsize, dysize)==TRUE)
				 {
			        length=(tempx-dx)*(tempx-dx)+(tempy-dy)*(tempy-dy);

			        if(length<min_length)
					{
				       min_x=tempx;
				       min_y=tempy;
				       min_length=length;
					}
				 }
		 }

		 // 빈자리가 없으면, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // 빈자리가 있으면, 
		 {
			 x=min_x; y=min_y;
			 return TRUE;
		 }

   return FALSE;
}

// 숫자를 써준다.
SHORT DrawNumber(SHORT startx, SHORT starty, SHORT file, SHORT startfont, SHORT number, SHORT option, BOOL clipmode)
{
	SHORT i;
	char string[20];
	SHORT font;


	SHORT number_xsize	=	GetFileXsize(file);
	SHORT number_ysize	=	GetFileYsize(file);

	int clipx1, clipy1, clipx2, clipy2;

	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);
	

	// 옵션이 있다면, 
	if(option)
	{
		switch(option)
		{
		case '+':	font=10;	break;
		case '\\':  font=12;    break; 
		case '%':	font=11;    break;
		case '%%':  font=11;    break;
		case '~':	font=15;    break;
		default:    font=0;     SetMessage("사용할 연산자가 없습니다.(There's no more operators!)"); break;
		}

		Draw_PutSpriteT(startx, starty, number_xsize, number_ysize, file, font);	// hojae_append

		startx+=number_xsize;
	}
	else
	{

		itoa(number, string, 10);

	    for(i=0;i<lstrlen(string);i++)
		{
	       font=startfont+string[i]-'0';

			Draw_PutSpriteT(startx, starty, number_xsize, number_ysize,file, font);	// hojae_append
	  
	       startx+=number_xsize;
		}
	}

	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);

	return startx;
}

void KillOperation(SHORT myplayernumber, _NewID attackid)
{

	// 유효하지 않은 플레이어 번호가 넘어오면 리턴. 
	if(IsValidPlayerNumber(myplayernumber) == FALSE)
	{
		return ;
	}
	
	// 사망한 적의 플레이어 번호 
	SHORT eplayer = CR[attackid.ID ].GetPlayerNumber();

}

// 2캐릭터 사이의 거리를 구한다.
SHORT GetLengthBetweenChar(SHORT id1, SHORT id2)
{

	if(IsAlive(id1)==FALSE || IsAlive(id2)==FALSE)return 0;

	SHORT x1=CR[id1].m_ssX;
	SHORT y1=CR[id1].m_ssY;
	SHORT xsize1=CR[id1].m_scIDXsize;
	SHORT ysize1=CR[id1].m_scIDYsize;

	SHORT x2=CR[id2].m_ssX;
	SHORT y2=CR[id2].m_ssY;
	SHORT xsize2=CR[id2].m_scIDXsize;
	SHORT ysize2=CR[id2].m_scIDYsize;

	SHORT basicx1, basicx2;
	SHORT basicy1, basicy2;
	

	if(x2>x1)
	{
		basicx1=x1;
		basicx2=x2-xsize2+1;
	}
	else
	{
		basicx1=x1-xsize1+1;
		basicx2=x2;
	}

	if(y2>y1)
	{
		basicy1=y1;
		basicy2=y2-ysize2+1;
	}
	else
	{
		basicy1=y1-ysize1+1;
		basicy2=y2;
	}

	return max( abs(basicx1-basicx2), abs(basicy1-basicy2));
}



// x, y의 좌표가 판넬위에 있는지 확인한다. 
BOOL IsPtInPannel(SHORT x, SHORT y)
{
//	unsigned char color;
	SHORT pannelx=x;
    SHORT pannely=PannelSpr.Header.Ysize-(clGrp.GetScreenYsize() - y);
	  

	// 범위를 벗어나 있으면, 
	if(pannelx<0 || pannelx>=PannelSpr.Header.Xsize
	|| pannely<0 || pannely>=PannelSpr.Header.Ysize)
	{
		return FALSE;
	}

	// 해당점의 색을 구한다. 
/*	if(clGrp.LockSurface(lpPannelSurface)==TRUE)
	{
		color=clGrp.GetPixel(pannelx, pannely);

		clGrp.UnlockSurface(lpPannelSurface);
	}
*/

	// 투명색이면 판넬이 아니다. 
//	if(color==TRANSCOLOR)return FALSE;

	return TRUE;
}

BOOL IsPtInAllyWindow(SHORT x,SHORT y)
{

	// 동맹창을 켠 상태일 경우
	if(bGameAllyWindowSwitch == FALSE)
	{
		
		if((x > 680) && (x < 790) && (y > 391) && (y < 499))
			return TRUE;
	}

	else if(bGameAllyWindowSwitch == TRUE)
	{
		if((x > 680) && (x < 790) && (y > 482) && (y <  499))
			return TRUE;
	}
	return FALSE;
}


// 캐릭터 간의 거리를 구한다. 
SHORT GetLegthBetweenChar(SHORT id1, SHORT id2)
{
	if(IsSee(id1)==FALSE)return 0;
	if(IsSee(id2)==FALSE)return 0;

	return max( abs(CR[id1].m_ssX-CR[id2].m_ssX),  abs(CR[id1].m_ssY-CR[id2].m_ssY));
	
}

// 지도의 포그를 밝혀준다. 
void GlobalBlackCheck(SI32 x, SI32 y, SHORT range, BOOL bMode, SI32 pnumber)
{
	int* temp;

	switch(range)
	{
	case 0:
	case 1:
	case 2:
		temp=NewCircle2;
		break;
	case 3:
		temp=NewCircle3;
		break;
	case 4:
		temp=NewCircle4;
		break;
	case 5:
		temp=NewCircle5;
		break;
	case 6:
		temp=NewCircle6;
		break;
	case 7:
		temp=NewCircle7;
		break;
	case 8:
		temp=NewCircle8;
		break;
	case 9:
		temp=NewCircle9;
		break;
	case 10:
		temp=NewCircle10;
		break;
	case 11:
		temp=NewCircle11;
		break;
	default:
		temp=NewCircle11;
		break;
	}

	if(pnumber ==-1)
	{
		if(bMode == TRUE)
		{
			BlackCheckAllGroupLight(temp, x, y, pnumber);
		}
		else
		{
			BlackCheckAllGroupDark(temp, x, y, pnumber);
		}
	}
	else
	{
		if(bMode==TRUE)
		{
			BlackCheckSpecialGroupLandLight(temp, x, y, pnumber);
		}
		else
		{
			BlackCheckSpecialGroupDark(temp, x, y, pnumber);
		}
	}

}

	
void BlackCheckSpecialGroupDark(int* temp, SI32 x, SI32 y, SI32 pnumber)
{
	int index = 0;
	int init=temp[index];
	int data, j;
	int temp_x, temp_y;
	index++;

	while(1)
	{
		data=temp[index];

		if(data==100)break;

		for(j=-data;j<=data;j++)
		{
			temp_x=x+j;
			temp_y=y+index-init-1;

			if(Map.IsInMapArea(temp_x, temp_y)==TRUE)
			{
				// 내가 호의적인 플레이어의 포그도 같이 밝힌다.
				for(SI32 temp_player=0;temp_player<MAX_MULTI_PLAYER;temp_player++)
				{
					if(IsFriendToByPlayerNumber(pnumber, temp_player)==TRUE)
					{
						GD.BlackMap[temp_player][temp_x][temp_y]	= FOG2_BLACK_STEP;
					}
				}
			}
		}

		index++;
	}
}


void BlackCheckSpecialGroupLandLight(int* temp, SI32 x, SI32 y, SI32 pnumber)
{
	int index = 0;
	int data, j;
	int temp_x, temp_y;

	int init=temp[index];

	index++;

	while(1)
	{
		data=temp[index];

		if(data==100)break;

		for(j=-data;j<=data;j++)
		{
			temp_x=x+j;
			temp_y=y+index-init-1;

			if(Map.IsInMapArea(temp_x, temp_y)==TRUE)
			{
				if(GD.LastLevel[x][y] + 2  >= GD.LastLevel[temp_x][temp_y] )
				{
					
					// 내가 호의적인 플레이어의 포그도 같이 밝힌다.
					for(SI32 temp_player=0;temp_player<MAX_MULTI_PLAYER;temp_player++)
					{
						if(IsFriendToByPlayerNumber(pnumber, temp_player)==TRUE)
						{
							GD.BlackMap[temp_player][temp_x][temp_y]	= 0;
						}
					}
				}
			}
			
		}

		index++;
	}

}

void BlackCheckSpecialGroupAirLight(int* temp, SI32 x, SI32 y, SI32 pnumber)
{
	int index = 0;
	int data, j;
	int temp_x, temp_y;

	int init=temp[index];
	index++;

	while(1)
	{
		data=temp[index];

		if(data==100)break;

		for(j=-data;j<=data;j++)
		{
			temp_x=x+j;
			temp_y=y+index-init-1;

			if(Map.IsInMapArea(temp_x, temp_y)==TRUE)
			{
				GD.BlackMap[pnumber][temp_x][temp_y]	= 0;
			}
			
		}

		index++;
	}

}

void BlackCheckAllGroupLight(int* temp, SI32 x, SI32 y, SI32 pnumber)
{
	int index = 0;
	int data, i, j;
	int temp_x, temp_y;

	int init=temp[index];
	index++;

	while(1)
	{
		data=temp[index];

		if(data==100)break;

		for(j=-data;j<=data;j++)
		{
			temp_x=x+j;
			temp_y=y+index-init-1;
			
			if(Map.IsInMapArea(temp_x, temp_y)==TRUE)
			{
				for(i=0;i<MAX_MULTI_PLAYER;i++)
				{
					GD.BlackMap[i][temp_x][temp_y]              = 0;
				}
			}
			
		}
		index++;
	}

}

void BlackCheckAllGroupDark(int* temp, SI32 x, SI32 y, SI32 pnumber)
{
	int index = 0;
	int data, i, j;
	int temp_x, temp_y;

	int init=temp[index];
	index++;

	while(1)
	{
		data=temp[index];

		if(data==100)break;

		for(j=-data;j<=data;j++)
		{
			temp_x=x+j;
			temp_y=y+index-init-1;

			if(Map.IsInMapArea(temp_x, temp_y)==TRUE)
			{
				for(i=0;i<MAX_MULTI_PLAYER;i++)
				{
					GD.BlackMap[i][temp_x][temp_y] = FOG2_BLACK_STEP;
				}
				
			}
			
		}
		index++;
	}

}

// path를 고려하여 화일 이름을 구한다. 
void GetFileNamePath(char* src_filename, char* path, char* dst_filename)
{

	char buffer[FILE_NAME_SIZE];

	wsprintf(buffer, "%s%s", path, src_filename);
	strcpy(dst_filename, buffer);
}




DWORD FrameClock[50][2];
DWORD StartFrameClock ;
int FrameClockDelay; 

void SetStartFrameClock(DWORD clock)
{
	StartFrameClock = clock;
}

void InitFrameClock()
{
	int i;

	for( i=0;i<50;i++)
	{
		FrameClock[i][0] =0;
		FrameClock[i][1] =0;
	}

}

void PushFrameClock(DWORD frame, DWORD clock)
{
	int i;


	for(i = 0 ;i < 50 - 1 ;i++)
	{
		FrameClock[i][0] = FrameClock[i + 1][0];
		FrameClock[i][1] = FrameClock[i + 1][1];
	}
	
	FrameClock[50 - 1][0] = frame;
	FrameClock[50 - 1][1] = clock - StartFrameClock;
}


// 원격지 컴퓨터와 비교하여 내 컴의 속도가 정상인지 확인한다. 
void CompareSyncSpeed(SHORT id, DWORD frame, DWORD clock)
{
	int i;
	DWORD my_time = 0 ;

	if(FrameClock[50 - 1][0]  == 0)return ;
	if(clock == 0)return ;

	// 내 기록중에 frame이 남아 있는지 확인한다. 
	for(i=0;i < 50;i++)
	{
          if(FrameClock[i][0] == frame)
		  {
			  my_time = FrameClock[i][1];
			  break;
		  }
	}
	if(my_time == 0)
	{
		return ;
	}

	// 내 컴이 다른 컴에 비해 느리면, 
	if(my_time > clock)
	{
		SetMessage("My com. is slow. I would better hurry.");
		FrameClockDelay = -1;
	}
	else if(my_time < clock)
	{
		SetMessage("My com. is fast. I would better slow.");
		FrameClockDelay = 1;
	}

}

DWORD GetFrameClock()
{
	return FrameClock[50 - 1][1];
}


// 텍스트를 특정 좌표를 중심으로 찍는다.(가운데 정렬)
void DrawTextInCenter(HDC hdc, SI32 centerx, SI32 centery, char* text)
{
	SIZE size;

	//텍스트의 길이를 구한다. 
	GetTextExtentPoint32(hdc, text, strlen(text), &size);		
	TextOut(hdc, centerx - size.cx/2, centery - size.cy/2, text, lstrlen(text));
}


// 텍스트를 주어진 RECT안에 출력한다. 
void DrawTextInRect(LPDIRECTDRAWSURFACE7 psurface, HFONT font, char* buffer, RECT rect)
{
	HDC		hdc;
	SIZE	size;

	// 남은 시간을 표시한다. 
	if(lpBriefingBuffer)
	{
		HRESULT hr;
		
		if( (hr = lpBriefingBuffer->GetDC(&hdc) ) ==DD_OK)
		{
			SelectObject(hdc, font);
			
			//텍스트의 길이를 구한다. 
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &size);		
			lpBriefingBuffer->ReleaseDC(hdc);
		}
		
		// 글자가 쓰여야 할 공간을 지운다. 
		if(clGrp.LockSurface(lpBriefingBuffer) == TRUE)
		{
			clGrp.FillBox(0, 0, size.cx, size.cy, TRANSCOLOR);
			clGrp.UnlockSurface(lpBriefingBuffer);
		}
		
		// 글자를 출력한다.
		if(lpBriefingBuffer->GetDC(&hdc)==DD_OK)
		{
			SelectObject(hdc, font);
			
			SetTextColor( hdc, NORMAL_TEXT_COLOR );
			SetBkMode(hdc, TRANSPARENT);
			
			TextOut(hdc, 0, 0, buffer, lstrlen(buffer));
			
			lpBriefingBuffer->ReleaseDC(hdc);
		}
		
		RECT srcrect  = {0, 0, size.cx, size.cy};
		psurface->Blt(&rect, lpBriefingBuffer, &srcrect, DDBLT_KEYSRC, NULL);
	}
	
	
}



// Box를 사용하여 수치를 바형태로 표현한다. 
SI32 DrawBoxBar(SI32 value, SI32 maxvalue, SI32 valueperbox, POINT start, SI32 xdotsize, BOOL bVillage)
{
	SI32 i, j;
	if(value > maxvalue) maxvalue = value;
	// 최대 체력치로 몇개의 박스를 그려야 하는가 ?
	SI32 maxbox = maxvalue / valueperbox;
	if(value % valueperbox)maxbox++;

	// 채워진 체력은 몇개의 박스로 표시되는가 ?
	SI32 fillbox = value / valueperbox;
	if(value % valueperbox)fillbox++;


	if(xdotsize < ENERGY_UNIT_SIZE)
		return 0;

	// 가로로 최대 몇개의 박스를 그릴 수 있는가 ?
	SI32 xnumber = xdotsize / ENERGY_UNIT_SIZE;
	SI32 ynumber = maxbox / xnumber;
	if(maxbox % xnumber)ynumber++;

	if(xnumber > maxbox)
	{
		xnumber = maxbox;
	}
	start.x = start.x;// + (xdotsize  - (xnumber * ENERGY_UNIT_SIZE))/2;

	for( i = 0;i<ynumber;i++)
	{
		for(j=0;j<xnumber;j++)
		{
			if(maxbox)
			{
				if(!bVillage)
				{
					clGrp.BoxC(	start.x + j*ENERGY_UNIT_SIZE, start.y + i * ENERGY_UNIT_SIZE, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE,
						start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE, clGrp.GetPalette16(17, 17, 17));
					
					if(fillbox)
					{
						clGrp.FillBoxC(	start.x + j*ENERGY_UNIT_SIZE+1,	start.y + i * ENERGY_UNIT_SIZE+1, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1,	
							start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1, clGrp.GetPalette16(43, 13, 1));
					}
					else
					{
						clGrp.FillBoxC(	start.x + j*ENERGY_UNIT_SIZE+1,	start.y + i * ENERGY_UNIT_SIZE+1, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1,	
							start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1, clGrp.GetPalette16(55, 55, 55));
					}
				}
				else
				{
					clGrp.BoxC(	start.x + j*ENERGY_UNIT_SIZE, start.y + i * ENERGY_UNIT_SIZE, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE,
						start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE, COLOR_BORDER_IN_BOX_BAR);
					
					if(fillbox)
					{
						clGrp.FillBoxC(	start.x + j*ENERGY_UNIT_SIZE+1,	start.y + i * ENERGY_UNIT_SIZE+1, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1,	
							start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1, COLOR_GREEN);
					}
				}
			}
			
			if(maxbox)
			{
				maxbox--;
			}
			if(fillbox)
			{
				fillbox--;
			}
		}
	}
				
	return ynumber*ENERGY_UNIT_SIZE;
}

// robypark 2004/10/28 14:20
// Box를 사용하여 수치를 바형태로 표현한다. 
// 위의 DrawBoxBar()와 동일하나 바 테두리 선 색과 채워지는 색을 전달 받아 사용한다.
// bVillage 값이 TRUE라면 빈 바에는 clrBarEmpty 칼라 값으로 채워진다.
SI32 DrawBoxBar(SI32 value, SI32 maxvalue, SI32 valueperbox, POINT start, SI32 xdotsize, SI08 clrBarEdge, SI08 clrBarFilled, BOOL bVillage/* = FALSE*/, SI08 clrBarEmpty/* = 0*/)
{
	SI32 i, j;
	if(value > maxvalue) maxvalue = value;
	// 최대 체력치로 몇개의 박스를 그려야 하는가 ?
	SI32 maxbox = maxvalue / valueperbox;
	if(value % valueperbox)maxbox++;

	// 채워진 체력은 몇개의 박스로 표시되는가 ?
	SI32 fillbox = value / valueperbox;
	if(value % valueperbox)fillbox++;


	if(xdotsize < ENERGY_UNIT_SIZE)
		return 0;

	// 가로로 최대 몇개의 박스를 그릴 수 있는가 ?
	SI32 xnumber = xdotsize / ENERGY_UNIT_SIZE;
	SI32 ynumber = maxbox / xnumber;
	if(maxbox % xnumber)ynumber++;

	if(xnumber > maxbox)
	{
		xnumber = maxbox;
	}
	start.x = start.x;// + (xdotsize  - (xnumber * ENERGY_UNIT_SIZE))/2;

	for( i = 0;i<ynumber;i++)
	{
		for(j=0;j<xnumber;j++)
		{
			if(maxbox)
			{
				// 바 테두리 그리기
				clGrp.BoxC(	start.x + j*ENERGY_UNIT_SIZE, start.y + i * ENERGY_UNIT_SIZE, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE,
					start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE, clrBarEdge);
					
				if (fillbox)
				{
					// 바에 색 채우기
					clGrp.FillBoxC(	start.x + j*ENERGY_UNIT_SIZE+1,	start.y + i * ENERGY_UNIT_SIZE+1, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1,	
						start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1, clrBarFilled);
				}
				else
				{
					// 바의 빈 칸에 특정 색을 사용하는 경우라면...
					if (FALSE == bVillage)
					{
						// 바의 빈칸에 특정 색 채우기
						clGrp.FillBoxC(	start.x + j*ENERGY_UNIT_SIZE+1,	start.y + i * ENERGY_UNIT_SIZE+1, start.x + j*ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1,	
							start.y + i * ENERGY_UNIT_SIZE + ENERGY_UNIT_SIZE-1, clrBarEmpty);
					}
				}
			}
			
			if(maxbox)
			{
				maxbox--;
			}
			if(fillbox)
			{
				fillbox--;
			}
		}
	}
				
	return ynumber*ENERGY_UNIT_SIZE;
}

// robypark 2004/10/28 15:3
// 수치를 게이지 형태로 그려준다.
void DrawGauge(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight, R32 fRate, UI08 uiClrGaugeFilled, UI08 uiClrGaugeEmpty)
{
	// 그려질 수 없는 크기라면
	if( siWidth < 1 || siHeight < 1 )
		return;

	// 비율을 백분위에 맞게 보정
	if( fRate <= 0.0f)
		fRate = 0.0f;

	if( fRate > 1.0f )
		fRate = 1.0f;			// 비율 넘어갈때는 그냥 고정시켜라

	// 그려져야할 수치 크기를 비율에 맞게 보정
	UI32	uiLength		= siWidth * fRate;

	// 게이지를 채운다.
	if( uiLength >= 1 )
		clGrp.FillBoxC(siX, siY, siX + uiLength - 1,	siY + siHeight - 1, uiClrGaugeFilled);

	// 채워지지 않은 게이지에 색 칠하기
	if (fRate < 1.0f)
		clGrp.FillBoxC(siX + uiLength, siY, siX + siWidth - 1, siY + siHeight - 1, uiClrGaugeEmpty);

	clGrp.BoxC(	siX - 1, siY - 1, siX + siWidth, siY + siHeight, COLOR_BORDER_IN_BOX_BAR);
}

// robypark 2005/1/7 18:41
// 수치(10%단위로)를 게이지 형태로 그려준다.
// 공성전 유닛 체력, 보급 게이지에서 사용
void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage)
{
	// 범위를 넘지 않도록 처리
	if (siRate10 > 10)
		siRate10 = 10;

	if (siRate10 < 0)
		siRate10 = 0;

	UI32 uiWidth = (psprImage->Header.Xsize * siRate10) / 10;

	// 그리기
	clGrp.PutSprite(siX, siY,
						psprImage->Header.Xsize,
						psprImage->Header.Ysize,
						0,
						0,
						uiWidth,
						psprImage->Header.Ysize,
						psprImage->Image);
}

// robypark 2005/1/7 18:41
// 수치를 게이지 형태로 그려준다.
// 마을 방어력, 수비병 게이지 출력에서 사용
void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage)
{
	// 범위를 넘지 않도록 처리
	if (siRate > 100)
		siRate = 100;

	if (siRate < 0)
		siRate = 0;

	UI32 uiWidth = (psprImage->Header.Xsize * siRate) / 100;

	// 그리기
	clGrp.PutSprite(siX, siY,
						psprImage->Header.Xsize,
						psprImage->Header.Ysize,
						0,
						0,
						uiWidth,
						psprImage->Header.Ysize,
						psprImage->Image);
}

// 사망에 의한 경험치 손실을 구한다.
SI32 GetExpReduceForDying(SI32 level)
{
	return level * min(10, level);
}


// NPC 의 초상을 그린다.
// npc_kind: NPC의 종류.
// x, y: 찍어야 할 시작 위치.
// psurface: 찍어야 할 서피스 
// lock_switch: 서피스를 락한 후에 그려야 하는지 그냥 그려야 하는지 여부.(TRUE : 락 한 후에 그린다.)
void DrawNPCProtrait(SI32 npc_kind, SI16 nation,SI32 x, SI32 y, LPDIRECTDRAWSURFACE7 psurface, BOOL lock_switch) 
{
	SI32 font = 0;

	if(lock_switch == TRUE)
	{
		if(clGrp.LockSurface(psurface)==FALSE)
			return ;
	}

	switch(nation)
	{
	case ON_COUNTRY_JAPAN:
		{
			switch(npc_kind)
			{
			case NPC_KIND_GOV:			font = 90;	break;
			case NPC_KIND_FARM:			font = 185;	break;
			case NPC_KIND_WEAPONSHOP:	font = 92;	break;
			case NPC_KIND_NORMALMARKET:	font = 91;	break;
			case NPC_KIND_CROPMARKET:	font = 99;	break;
			case NPC_KIND_DRUGMARKET:	font = 95;	break;
			case NPC_KIND_HARVOR:		font = 98;	break;
			case NPC_KIND_DOCTOR:		font = 94;	break;
			case NPC_KIND_BANK:			font = 93;	break;
			case NPC_KIND_BARRACK:		font = 96;	break;
			case NPC_KIND_GBARRACK:		font = 97;	break;
			case NPC_KIND_MBARRACK:		font = 183;	break;
			default:					clGrp.Error("No NPC Portrait", "[%d]", npc_kind);		return ;
			}
		}
		break;
	case ON_COUNTRY_TAIWAN:
		{
			switch(npc_kind)
			{
			case NPC_KIND_GOV:			font = 160;	break;
			case NPC_KIND_FARM:			font = 186;	break;
			case NPC_KIND_WEAPONSHOP:	font = 168;	break;
			case NPC_KIND_NORMALMARKET:	font = 161;	break;
			case NPC_KIND_CROPMARKET:	font = 163;	break;
			case NPC_KIND_DRUGMARKET:	font = 162;	break;
			case NPC_KIND_HARVOR:		font = 166;	break;
			case NPC_KIND_DOCTOR:		font = 165;	break;
			case NPC_KIND_BANK:			font = 164;	break;
			case NPC_KIND_BARRACK:		font = 167;	break;
			case NPC_KIND_GBARRACK:		font = 169;	break;
			case NPC_KIND_MBARRACK:		font = 170;	break;
			default:					clGrp.Error("No NPC Portrait", "[%d]", npc_kind);		return ;
			}
		}
		break;
	case ON_COUNTRY_CHINA:
		{
			switch(npc_kind)
			{
			case NPC_KIND_GOV:			font = 171;	break;
			case NPC_KIND_FARM:			font = 187;	break;
			case NPC_KIND_WEAPONSHOP:	font = 175;	break;
			case NPC_KIND_NORMALMARKET:	font = 172;	break;
			case NPC_KIND_CROPMARKET:	font = 174;	break;
			case NPC_KIND_DRUGMARKET:	font = 176;	break;
			case NPC_KIND_HARVOR:		font = 180;	break;
			case NPC_KIND_DOCTOR:		font = 180;	break;
			case NPC_KIND_BANK:			font = 178;	break;
			case NPC_KIND_BARRACK:		font = 177;	break;
			case NPC_KIND_GBARRACK:		font = 179;	break;
			case NPC_KIND_MBARRACK:		font = 181;	break;
			default:					clGrp.Error("No NPC Portrait", "[%d]", npc_kind);		return ;
			}
		}
		break;
	case ON_COUNTRY_KOREA:
	default:
		{
			switch(npc_kind)
			{
			case NPC_KIND_GOV:			font = 74;	break;
			case NPC_KIND_FARM:			font = 184;	break;
			case NPC_KIND_WEAPONSHOP:	font = 76;	break;
			case NPC_KIND_NORMALMARKET:	font = 75;	break;
			case NPC_KIND_CROPMARKET:	font = 78;	break;
			case NPC_KIND_DRUGMARKET:	font = 77;	break;
			case NPC_KIND_HARVOR:		font = 80;	break;
			case NPC_KIND_DOCTOR:		font = 79;	break;
			case NPC_KIND_BANK:			font = 83;	break;
			case NPC_KIND_BARRACK:		font = 82;	break;
			case NPC_KIND_GBARRACK:		font = 81;	break;
			case NPC_KIND_MBARRACK:		font = 182;	break;
			default:					clGrp.Error("No NPC Portrait", "[%d]", npc_kind);		return ;
			}
		}
		break;
	}

	Draw_PutSpriteT(x,  y, FILE_ON_PORTRAIT, font);

	if(lock_switch == TRUE)
	{
		clGrp.UnlockSurface(psurface);
	}

}