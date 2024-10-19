#ifndef _ROCKET_NEW_TILE_MAP

//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <CharOrg.h>
#include <Capture.h>
//#include <OnlineWorld.h>

#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <bullet.h>
#include <smoke.h>
#include <map.h>

extern _clCapture		Capture;							// ȭ�� ĸ�� ���� Ŭ���� 
extern	_Map 				Map;
extern	_MainData			GD;									// ������ �ֵ����� 
extern	unsigned char		DarkTable[][256];					// ��ο� �ܰ� 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	RECT				ScreenRect;							// ������ ǥ�õǴ� ���� 
extern	SHORT 				MapEditMode;						// ���� ���� ����ΰ�?
extern	_FileVary 			FV;
extern	XSPR 				TileSpr[];							// ���� ��Ʈ�� Ÿ�� 
extern	unsigned char		TeamDarkTable[][256];
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 

extern	unsigned char		TeamLightTable[];
extern	unsigned char		CTable2[];
extern	unsigned char		BlackTable[];
extern	SI32				BlackStatus, DarkStatus;

// ĳ���� �׸���
extern	LPDIRECTDRAWSURFACE7 lpScreenBuffer;

extern	SHORT				OldVirtualMapX, OldVirtualMapY;
extern	char				DrawMapPart[][MAX_DRAWMAP_SCREEN_YSIZE];
extern	unsigned char		FogType[];
extern	unsigned char		FogTable[];
extern	unsigned char		FogImage[][2][15][64*48];

extern	_Tile Tile[];

#ifdef _DEBUG
// ���ڸ� ���ش�.
extern SHORT DrawNumber(SHORT startx, SHORT starty, SHORT file, SHORT startfont, SHORT number, SHORT option, BOOL clipmode);

// robypark 2004/8/4 18:42
// ġƮŰ ���� �÷��� ���� ����
extern bool g_bShowGrid;		// �ʿ� GRID �׸���
extern bool g_bShowMapIdx;		// ���� �� ���� ĳ���Ͱ� ������ ��� ���� �ε��� ��ǥ�� �׷������� �� �������� ����

#endif


SI16 DrawMapScreen[MAX_MAP_XSIZE][MAX_MAP_YSIZE];

int fog[] = {
-1,	//0
0,	//1
1,	//2
4,	//3
3,	//4
7,	//5
13,  //6
11, //7
2,   //8
12,	//9
5,	//10
8,	//11
6,	//12
10,	//13
9,	//14
14	//15

};

// ȭ�鿡 ���� ������ �ӽ÷� �����ϴ� �� 
UI08 ScreenMapBuffer[CLIENT_SCREEN_XSIZE*CLIENT_SCREEN_YSIZE];

unsigned char* MapCaptureBuffer;

BOOL _Map::DrawMapEntire()
{
	SHORT i, j;
	int xsize, ysize;
	int centerx, centery;
	int x1, y1, x2, y2;
	SHORT id;

	xsize=MH.MapXsize*64;
	ysize=MH.MapYsize*32+200;

	MapCaptureBuffer=new unsigned char [xsize*ysize];
	if(MapCaptureBuffer==0)return FALSE;

	VirtualMapX=0;
	VirtualMapY=0;
	Map.DrawForceSwitch=TRUE;

   for(i=0;i<MH.MapYsize;i++)
       for(j=0;j<MH.MapXsize;j++)
	   {
	     GD.BlackMap2ChangedSwitch[j][i]=TRUE;
	   }


	clGrp.LockSurface(MapCaptureBuffer, xsize, ysize);

	clGrp.SetScreenXsize(xsize);
	clGrp.SetScreenYsize(ysize);

	clGrp.GetClipArea(x1, y1, x2, y2);
	clGrp.SetClipArea(0, 0, xsize-1, ysize-1);

	for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {
		   centerx=xsize/2+
				+j*MAP_TILE_XSIZE_HALF
				-i*MAP_TILE_XSIZE_HALF;

		   centery=200+
				+i*(MAP_TILE_YSIZE/2) 
				+j*(MAP_TILE_YSIZE/2);
		   
		   // ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
		   if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)
		   {
			   DrawHillBlack(centerx, centery, j, i);
		   }
		   else
		   {
			   
			   // ���� ������ 
			   if(GetSeaFile(j/2, i/2))
			   {
				   DrawSea(centerx, centery, j, i);
			   }
			   // ��ֹ��� ����� ������ 
			   else if(GetNewBlkFile(j/2, i/2))
			   {
				   DrawNewBlk(centerx, centery, j, i);
			   }
			   // ���� ������ 
			   else if(GetCastleFile(j/2, i/2))
			   {
				   DrawCastle(centerx, centery, j, i);
			   }
			   // �Ϲ� ����� ������ 
			   else
				   DrawHill(centerx, centery, j, i, FALSE);
		   }
		   
		   GD.BlackMap2ChangedSwitch[j][i]= FALSE;		   
	   }

   if( pShallowMap )
   {
	   // ���� ���� �׸���. 
	   for(i=0;i<MH.MapYsize;i++)
		   for(j=0;j<MH.MapXsize;j++)
		   {
			   centerx=xsize/2+
				   +j*MAP_TILE_XSIZE_HALF
				   -i*MAP_TILE_XSIZE_HALF;
			   
			   centery=200+
				   +i*(MAP_TILE_YSIZE/2) 
				   +j*(MAP_TILE_YSIZE/2);


			   // ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			   if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			   else
			   {
				   	SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

				   
				   if( pShallowMap[j + YOff[i]] && GD.Rain.WaterLevel<(MAX_WATER_LEVEL_STEP-1) )
				   {
					   DrawShallow(centerx, centery, j, i, DarkTable[darkstep]);
				   }
			   }
		   }
   }
			
	// ���ڸ� �׸���. 
   for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {

		   centerx=xsize/2+MapCenterX[j][i];
	       centery=200+MapCenterY[j][i];


	
			// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			else
			{

				SHORT darkstep=GD.GetBlackMap2(j, i);
				darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

		       // ��Ȯ�� ���� ����� �׷��ش�. 
			   if(GetCropTypeMap(j, i)==CROP_TYPE_TREE && GetCropMountMap(j, i)==0)
			   {
			      DrawCrop(centerx, centery, j, i, DarkTable[darkstep]);
			   }
				// ���ڸ� ����ش�. 
			   else if(GetCropTypeMap(j, i)==CROP_TYPE_POTATO)
			   {
			      DrawCrop(centerx, centery, j, i, DarkTable[darkstep]);
			   }

			}
	   }

   // ���ڸ� �׸���. 
   for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {
		   centerx=xsize/2+MapCenterX[j][i];
	       centery=200+MapCenterY[j][i];

		
			// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			else
			{
				SHORT darkstep=GD.GetBlackMap2(j, i);
				darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

				if( GetMineMap(j, i) >=0 
				&& GD.IsFriendTo(GetMineMap(j, i), UserPlayerNumber) == TRUE )
				{
					DrawMine(centerx, centery, j, i, DarkTable[darkstep]);
				}
			}
	   }
		   
   for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {
		   centerx=xsize/2+MapCenterX[j][i];
	       centery=200+MapCenterY[j][i];

			    
			// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			else
			{
				SHORT darkstep=GD.GetBlackMap2(j, i);
				darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

				// ĳ���͸� �׸���. 
				// ��ü�� �׸��� �ʴ´�.
				id=GetIDMap(j, i);
				if(GD.CharInfo[id] && CR[id].CurrentOrder!=ORDER_DEAD && CR[id].CurrentOrder!=ORDER_CARCASS)
				{
				   CR[id].DecideFont();
				   CR[id].CalculateDrawStartXYInMapCaptureMode();
				   CR[id].m_ssDrawStartX+=(SHORT)(xsize/2);
				   CR[id].m_ssDrawStartY+=200;
				   CR[id].m_cltCharDrawInfo.SetDrawFrame(-1);
				   CR[id].Draw();
				}

				// ��� �׸���. 
				if(GetCropTypeMap(j, i)==CROP_TYPE_RICE 
                || GetCropTypeMap(j, i)>=CROP_TYPE_RUIN0
				|| (GetCropTypeMap(j, i)==CROP_TYPE_TREE && GetCropMountMap(j, i)) )
				{
				   DrawCrop(centerx, centery, j, i, DarkTable[darkstep]);
				}
			}
	   }


   // ���� ĳ���͸� �׸���. 
   for(i=0;i<GD.AirCharNumber;i++)
   {
	 id=GD.AirCharID[i].ID;
	 if(GD.CharInfo[id])
	 {
		 CR[id].DecideFont();
		 CR[id].CalculateDrawStartXYInMapCaptureMode();
		 CR[id].m_ssDrawStartX+=(SHORT)(xsize/2);
		 CR[id].m_ssDrawStartY+=200;
		 CR[id].m_cltCharDrawInfo.SetDrawFrame(-1);
		 CR[id].Draw();
	 }
   }
 	

	Capture.MapCapture(MapCaptureBuffer, xsize, ysize);

	delete[] MapCaptureBuffer;

	// ȭ���� ������ �����Ѵ�. 
	ScreenRect.left=0;
	ScreenRect.right=CLIENT_SCREEN_XSIZE-1;
	ScreenRect.top=0;
	ScreenRect.bottom=CLIENT_SCREEN_YSIZE-1;

	clGrp.SetClipArea((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, (SHORT)ScreenRect.right, (SHORT)ScreenRect.bottom);

	clGrp.SetScreenXsize(640);
	clGrp.SetScreenYsize(480);

	return TRUE;
}


// ���� Ÿ���� �׸���. 
BOOL _Map::DrawMap()
{
 
	int i, j;
	SHORT centerx, centery;
	int clipx1, clipy1, clipx2, clipy2;
	SI32 data, temp;

	// ���� ���� ��尡 �����Ǿ� ������ ��ŵ���� �ʰ� ��� �׸���. 
	if(MapEditMode==TRUE)
	    DrawForceSwitch=TRUE;

	///////////////////////////////////////////////////////////////////////////////////
	// ���� ���α׷��� ������ ȭ�� ������ ������Ʈ�Ѵ�.
	for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
		for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				
				if(GD.BlackMap[UserPlayerNumber][j][i] == FOG2_BLACK_STEP)
				{
					if(GD.BlackMap2[j][i] != BlackStatus)
					{
						GD.BlackMap2[j][i] = BlackStatus;
					}
				}
				else if(GD.BlackMap[UserPlayerNumber][j][i] == FOG2_DARK_STEP)
				{
					if(GD.BlackMap2[j][i] != DarkStatus)
					{
						GD.BlackMap2[j][i] = DarkStatus;
					}
				}
				else 
				{
					if(GD.BlackMap2[j][i] != 0)
					{
						GD.BlackMap2[j][i] = 0;
					}
				}
			}
		}

		
		// ���� �帷�� �׸���. 
		for(int count=0;count<2;count++)
		{
			for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
				for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
				{
					if(IsInMapArea(j, i)==TRUE)
					{
						
						SI32 max = 0;
						// �ֺ��� ��Ȳ�� �м��Ѵ�. 
						for(int k =0;k<4;k++)
						{
							data = GD.GetBlackMap2(j, i, k);
							
							if(k==0)
							{
								temp = GD.GetBlackMap2(j-1, i-1, 3);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j,   i-1, 2);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j-1, i,	1);
								if(temp > data)data = temp;
							}
							if(k==1)
							{
								temp = GD.GetBlackMap2(j, i-1, 3);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j+1,   i-1, 2);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j+1, i,	0);
								if(temp > data)data = temp;
							}
							if(k==2)
							{
								temp = GD.GetBlackMap2(j-1, i, 3);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j-1,   i+1, 1);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j, i+1,	0);
								if(temp > data)data = temp;
							}
							
							if(k==3)
							{
								temp = GD.GetBlackMap2(j+1, i, 2);
								
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2( j,   i+1, 1);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j+1, i+1,	0);
								if(temp > data)data = temp;
							}
							
							if(data > max)
							{
								max = data;
							}
							if(data > GD.GetBlackMap2(j, i, k))
							{
								GD.SetBlackMap2(j, i, k, data);
							}
						}
						
						for(k=0;k<4;k++)
						{
							if(GD.GetBlackMap2( j, i, k) < (max-1))
							{
								GD.SetBlackMap2(j, i, k, max-1);
							}
						}
						
					}
				}
		}

	   count=0;
	   for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
		   for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
		   {
			   if(IsInMapArea(j, i)==TRUE)
			   {
				   if(GD.BlackMap2[j][i] != DrawMapScreen[j][i])
				   {
					   DrawMapScreen[j][i] = GD.BlackMap2[j][i];
					   GD.BlackMap2ChangedSwitch[j][i] = TRUE;
					   count++;
				   }
			   }
		   }
/////////////////////////////////////////////////////////////////////////////////////


	if(clGrp.LockSurface(ScreenMapBuffer, CLIENT_SCREEN_XSIZE, CLIENT_SCREEN_YSIZE)==FALSE)return FALSE;
	
	   clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	   clGrp.SetClipArea(0, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1);


	if(MapDrawSwitch==TRUE || DrawForceSwitch==TRUE )
	{

	   if(DrawForceSwitch==TRUE)
	   {
		   DrawForceSwitch=FALSE;

		   for(i=0;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, (UI08)0);
			   }
		   
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX+1 && VirtualMapY==OldVirtualMapY+1)
	   {

		   memmove(ScreenMapBuffer, &ScreenMapBuffer[CLIENT_SCREEN_XSIZE*MAP_TILE_YSIZE], 
			       CLIENT_SCREEN_XSIZE*(CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE)*sizeof(UI08));

		   for(i=MAX_DRAWMAP_SCREEN_YSIZE-1;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   
				   clGrp.FillBox32(0, CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE, 
					   CLIENT_SCREEN_XSIZE-1, 
					   CLIENT_SCREEN_YSIZE-1, (UI08)0);
			   }
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX-1 && VirtualMapY==OldVirtualMapY-1)
	   {
		   memmove(&ScreenMapBuffer[CLIENT_SCREEN_XSIZE*MAP_TILE_YSIZE], ScreenMapBuffer, 
			       CLIENT_SCREEN_XSIZE*(CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE)*sizeof(UI08));

		   for(i=0;i<1;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1,  MAP_TILE_YSIZE-1, 0);
			   }
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX-1 && VirtualMapY==OldVirtualMapY+1)
	   {
		   for(i=0;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[MAP_TILE_XSIZE+i*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		       for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
			   {
			     DrawMapPart[0][j]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(0, 0, MAP_TILE_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
			   }
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX+1 && VirtualMapY==OldVirtualMapY-1)
	   {
		   for(i=0;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[MAP_TILE_XSIZE+i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		       for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
			   {
			     DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE-1][j]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(CLIENT_SCREEN_XSIZE - MAP_TILE_XSIZE, 0, CLIENT_SCREEN_XSIZE - 1, CLIENT_SCREEN_YSIZE - 1, 0);
			   }
	   }

	   // ������ ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX && VirtualMapY==OldVirtualMapY+2)
	   {
		   for(i=MAP_TILE_YSIZE;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[(i-MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[0][j]=TRUE;
		   }
		   
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][MAX_DRAWMAP_SCREEN_YSIZE-1]=TRUE;
		   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(0, 0, MAP_TILE_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
				   clGrp.FillBox32(0, CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
			   }

	   }
	   // �ϼ��� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX-2 && VirtualMapY==OldVirtualMapY)
	   {
		   for(i=CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE-1;i>=0;i--)
		   {
			   memmove(&ScreenMapBuffer[(i+MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[0][j]=TRUE;
		   }
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][0]=TRUE;
		   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(0, 0, MAP_TILE_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, MAP_TILE_YSIZE-1, 0);
			   }
	   }
	   // �ϵ��� ��ũ�� 
	   else if((VirtualMapX==OldVirtualMapX && VirtualMapY==OldVirtualMapY-2))
	   {
           VirtualMapX=OldVirtualMapX;
		   for(i=CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE-1;i>=0;i--)
		   {
			   memmove(&ScreenMapBuffer[(i+MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE-1][j]=TRUE;
		   }
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][0]=TRUE;
		   }

		   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
		   {
			   
			   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, MAP_TILE_YSIZE-1, 0);
			   clGrp.FillBox32(CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
		   }
	   }
	   // ������ ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX+2 && VirtualMapY==OldVirtualMapY)
	   {
		   for(i=MAP_TILE_YSIZE;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[(i-MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE-1][j]=TRUE;
		   }
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][MAX_DRAWMAP_SCREEN_YSIZE-1]=TRUE;
		   }
		   
		   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
		   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
		   {
			   
			   clGrp.FillBox32(0, CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
			   clGrp.FillBox32(CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE, 0,CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
		   }
	   }
	   else if(VirtualMapX!=OldVirtualMapX || VirtualMapY!=OldVirtualMapY)
	   {

		   // ������ ������ ��� �׸���. 
		   for(i=0;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
			   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
				   DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, (UI08)0);
			   }

	   }
	   else 
	   {

		   for(i=0;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=FALSE;
			   }
	   }
 

		OldVirtualMapX=VirtualMapX;
		OldVirtualMapY=VirtualMapY;


	   for(i = VirtualMapY - MAX_SCREEN_HALF_SIZE;i < VirtualMapY + MAX_SCREEN_HALF_SIZE;i++)
	   {
		   for(j = VirtualMapX - MAX_SCREEN_HALF_SIZE;j < VirtualMapX + MAX_SCREEN_HALF_SIZE;j++)
		   {
			#ifdef _DEBUG
   				centerx = -VirtualMapX * MAP_TILE_XSIZE_HALF
							+ VirtualMapY * MAP_TILE_XSIZE_HALF
							+ j*MAP_TILE_XSIZE_HALF
							- i*MAP_TILE_XSIZE_HALF
							+ ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2;

				centery = -VirtualMapY*MAP_TILE_YSIZE_HALF
							- VirtualMapX*MAP_TILE_YSIZE_HALF
							+ ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
							+ i * MAP_TILE_YSIZE_HALF 
							+ j * MAP_TILE_YSIZE_HALF ;
			#endif

				// ���������� ����.
				if(IsInMapArea(j, i) == TRUE)
				{
				#ifndef _DEBUG
   					centerx = -VirtualMapX * MAP_TILE_XSIZE_HALF
								+ VirtualMapY * MAP_TILE_XSIZE_HALF
								+ j*MAP_TILE_XSIZE_HALF
								- i*MAP_TILE_XSIZE_HALF
								+ ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2;

					centery = -VirtualMapY*MAP_TILE_YSIZE_HALF
								- VirtualMapX*MAP_TILE_YSIZE_HALF
								+ ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
								+ i * MAP_TILE_YSIZE_HALF 
								+ j * MAP_TILE_YSIZE_HALF ;
				#endif

					// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
					if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)
					{
						DrawHillBlack(centerx, centery, j, i);
					}
					else
					{
						// ���� ������ 
						if(GetSeaFile(j/2, i/2))
						{
							DrawSea(centerx, centery, j, i);
						}
						// ��ֹ��� ����� ������ 
						else if(GetNewBlkFile(j/2, i/2))
						{
							DrawNewBlk(centerx, centery, j, i);
						}
						// ���� ������ 
						else if(GetCastleFile(j/2, i/2))
						{
							DrawCastle(centerx, centery, j, i);
						}
						// �Ϲ� ����� ������ 
						else
						{
							DrawHill(centerx, centery, j, i, FALSE);
						}

						// robypark 2004/7/26 19:16
						// �ʿ� GRID�� �׷��ش�.
						// �ӽ÷�  �߰��� �ڵ�� ���Ŀ� �簢������ ���� ���� �����Ͽ� ����Ͽ��� �Ѵ�.
					#ifdef _DEBUG
						int r = 0, g = 63, b = 63;
						int x0, x1, x2, x3, y0, y1, y2, y3;
						int nID = GetIDMap(j, i);

						// �ش� �ʿ� ĳ���Ͱ� ������ ��� �� ����
						if (0 != nID)
						{
							r = 63;
							g = 63;
							b = 0;
						}

						// Ÿ�� ��ġ�� �´� �簢�� ��ġ ����
						SHORT _centerx, _centery;
						SHORT _screenx, _screeny;
						_centerx=MapCenterX[j][i];
						_centery=MapCenterY[j][i];

						TransFromRealDotToScreenDot(_centerx, _centery, _screenx, _screeny);

						x0 = _screenx;
						y0 = _screeny - MAP_TILE_YSIZE_HALF;
	
						x1 = _screenx + MAP_TILE_XSIZE_HALF - 1;
						y1 = _screeny;
	
						x2 = _screenx;
						y2 = _screeny + MAP_TILE_YSIZE_HALF; - 1;
	
						x3 = _screenx - MAP_TILE_XSIZE_HALF;
						y3 = _screeny;

						// robypark 2004/8/4 18:59
						// �ɼǿ� �� GRID �׷��ֱⰡ �����Ǿ��� ��쿡�� �׸����� ����
						if (true == g_bShowGrid)
						{
							// ���� �Լ��� �̿��Ͽ� �簢�� �׸���
							clGrp.LineC(x0, y0, x3, y3, r, g, b);
							clGrp.LineC(x1, y1, x0, y0, r, g, b);
							clGrp.LineC(x2, y2, x1, y1, r, g, b);
							clGrp.LineC(x3, y3, x2, y2, r, g, b);
						}

						// robypark 2004/8/4 18:59
						// �ɼǿ� �� GRID �׷��ֱⰡ �����Ǿ��� ��쿡�� �׸����� ����
						// ���� Ÿ�Ͽ� ĳ���� ���̵� ������ ��� �� �ε��� �׸���
						if ((0 != nID) && (true == g_bShowMapIdx))
						{
							// �� ��ǥ�� �߽��� ��ġ�� ǥ���Ѵ�. 
							SHORT _centerx, _centery;
							SHORT _screenx, _screeny;

							_centerx=MapCenterX[j][i];
							_centery=MapCenterY[j][i];

						    TransFromRealDotToScreenDot(_centerx, _centery, _screenx, _screeny);

							// Draw Map Index
							DrawNumber(_screenx-1, _screeny-1, FILE_ON_LEVEL_NUM, 0, j, 0, TRUE);		// X
							DrawNumber(_screenx-1, _screeny-1+10, FILE_ON_LEVEL_NUM, 0, i, 0, TRUE);	// Y
						}
					#endif			
					}
			   
					GD.BlackMap2ChangedSwitch[j][i] = FALSE;
				}
			}
	   }  	
	}

	MapDrawSwitch=FALSE;
	DrawForceSwitch=FALSE;
    clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);

	return TRUE;
}


// ���� Ÿ���� ������ �� ��Ҹ� �׸���. 
BOOL _Map::DrawObj()
{

	SHORT i, j;
	SHORT id;
	SHORT centerx, centery;
	SI32 type;

	if( pShallowMap )
	{
		// ���� ���� �׸���. 
		for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
			for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
			{
				
				centerx=-VirtualMapX*MAP_TILE_XSIZE_HALF
					+VirtualMapY*MAP_TILE_XSIZE_HALF
					+ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2
					+j*MAP_TILE_XSIZE_HALF
					-i*MAP_TILE_XSIZE_HALF;
				
				centery=-VirtualMapY*MAP_TILE_YSIZE/2
					-VirtualMapX*MAP_TILE_YSIZE/2 
					+ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
					+i*(MAP_TILE_YSIZE/2) 
					+j*(MAP_TILE_YSIZE/2);
				
				if(IsInMapArea(j, i)==TRUE)
				{
					
					// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
					if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)continue;
					else
					{
						SHORT darkstep=GD.GetBlackMap2(j, i);
						darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
						
						if( pShallowMap[j + YOff[i]] && GD.Rain.WaterLevel<(MAX_WATER_LEVEL_STEP-1) )
						{
							DrawShallow(centerx, centery, j, i, DarkTable[darkstep]);
						}
					}
				}
				
			}
	}

	// ���ڸ� �׸���. 
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)continue;
				else
				{
					SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
					
					// ��Ȯ�� ���� ����� �׷��ش�. 
					if((GetCropTypeMap(j, i)==CROP_TYPE_TREE || GetCropTypeMap(j, i)==CROP_TYPE_BAMBOO)&& GetCropMountMap(j, i)==0)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
					// ���ڸ� ����ش�. 
					if(GetCropTypeMap(j, i)==CROP_TYPE_POTATO)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
					// �ٴڿ� �ٴ� �Ｚ����� ��´�. 
					else if(GetCropTypeMap(j, i) == CROP_TYPE_AD12)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
					// ȭ�굵 ��´�. 
					else if(GetCropTypeMap(j, i) == CROP_TYPE_VOLCANO)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
				}
			}
		}
	}
		
	// ���ڸ� �׸���. 
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			
			if(IsInMapArea(j, i)==TRUE)
			{
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)continue;
				else
				{
					
					SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
					
					if( GetMineMap(j, i) >= 0 
						&& GD.IsFriendTo(GetMineMap(j,i), UserPlayerNumber) == TRUE)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawMine(cx, cy, j, i, DarkTable[darkstep]);
					}
				}
			}
		}
	}
	
	// ��ü�� �׸���.
	for(i=0;i<GD.CharOrderIndex;i++)
	{
		
		id=GD.CharOrder[i];
		
		if(GD.CharInfo[id] && (CR[id].CurrentOrder==ORDER_DEAD || CR[id].CurrentOrder==ORDER_CARCASS))
		{
			CR[id].DecideFont();
			CR[id].CalculateDrawStartXY();
			CR[id].Draw();
		}
	}

	// ������ �׸���.
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			
			if(IsInMapArea(j, i)==TRUE)
			{
				
				
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteWhiteBlackMap2(j, i ) == TRUE
					&& GetTreasureMap(j,i))
				{
					
					SI32 sitype = ((GetTreasureMap(j, i) & 0xff00) >> 8);
					
					SHORT cx, cy;
					GetCenterXY(j, i, cx, cy);
					DrawTreasure(cx, cy, j, i, NULL, sitype);
				}
			}
		}
	}
					
	// robypark (2004-5-24, 16:51)
	// ���⸦ �׸���. 
	// �ٴ�(Floor) Smoke�� ��� �׷��ش�.
	for(i = 0; i < MAX_SMOKE_NUMBER; i++)
	{
		if (GD.SmokeInfo[i])
		{
			// �׷��� ��ǥ ����
			Smoke_CalculateDrawStartXY(i);

			// �ٴ�(Floor) Smoke�� ��� �׷��ش�.
			if (TRUE == g_Smoke[i].m_bFloorSmoke)
			{
				// �׸���
				Smoke_Draw(i);
			}
		}
	}

	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)	continue;
				else
				{
					SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

					// ĳ���͸� �׸���. 
					// ��ü�� �׸��� �ʴ´�.
					id=GetIDMap(j, i);

					if(GD.CharInfo[id] && CR[id].CurrentOrder!=ORDER_DEAD && CR[id].CurrentOrder!=ORDER_CARCASS)
					{
						CR[id].DecideFont();
						CR[id].CalculateDrawStartXY();
						CR[id].Draw();
					}

					// ������ ��Ĺ����� �׸���. 
					if( GetCropTypeMap(j, i)  == CROP_TYPE_RICE 
					||  GetCropTypeMap(j, i)  >= CROP_TYPE_RUIN0
					|| ((GetCropTypeMap(j, i)  == CROP_TYPE_TREE || GetCropTypeMap(j, i)  == CROP_TYPE_BAMBOO) && GetCropMountMap(j, i)) )
					{
						type = GetCropTypeMap(j, i);
						if(type  != CROP_TYPE_AD12 
						&& type != CROP_TYPE_VOLCANO)
						{
							SHORT cx, cy;
							GetCenterXYByMapCenterXY(j, i, cx, cy);
							DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
						}
					}

					if(GetFireMap(j, i))
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawFire(cx, cy, j, i, DarkTable[darkstep]);
					}
				}

				if(MapEditMode==TRUE)
				{
					SHORT _screenx, _screeny;
					
					if(IsEmpty(j, i)==FALSE)
					{
						
						GetCenterXY(j, i, _screenx, _screeny);
						clGrp.FillBoxC(_screenx - 1, _screeny - 1, _screenx + 1, _screeny + 1, (UI08)COLOR_GREEN);
						
					}
				}



#ifdef _SHOWMAPCOORDINATE 
				// �� ��ǥ�� �߽��� ��ġ�� ǥ���Ѵ�. 
				SHORT _centerx, _centery;
				SHORT _screenx, _screeny;
				SHORT type = GetHillType(j,i);
				{
					//if(CollMap[j][i] & BLOCK_RUIN)
					{
						if(IsInMapArea(j, i)==TRUE)
						{
							_centerx=MapCenterX[j][i];
							_centery=MapCenterY[j][i];

					        TransFromRealDotToScreenDot(_centerx, _centery, _screenx, _screeny);

							DrawNumber(_screenx-1, _screeny-1, FILE_ON_LEVEL_NUM, 0, j, 0, TRUE);
							DrawNumber(_screenx-1, _screeny-1+10, FILE_ON_LEVEL_NUM, 0, i, 0, TRUE);
//				           DrawNumber(_screenx-1, _screeny-1, FILE_TEAMNUMBER, 0, j, 0);
//					       DrawNumber(_screenx-1, _screeny-1+10, FILE_TEAMNUMBER, 0, i, 0);
						}
					}
				}
#endif
			}
		}
	}
	// ������� for
		
	// �Ѿ��� �׸���. 
	for(i=0;i<MAX_BULLET_NUMBER;i++)
	{
		// robypark (2004/6/12 12:27
		// smoke�� �׷����� ���� �׷����� �Ѿ˵鸸 �׸����� ����
		if ((GD.BulletInfo[i]) && (false == g_Bullet[i].m_bLayerAboveVolcano))
		{
			Bullet_CalculateDrawStartXY(i);
			Bullet_Draw(i);
		}
	}

	// robypark (2004-5-24, 16:51)
	// ���⸦ �׸���. 
	// �ٴ�(Floor) Smoke�� �ƴ� �͸� �׷��ش�.
	for(i = 0; i < MAX_SMOKE_NUMBER; i++)
	{
		if (GD.SmokeInfo[i])
		{
			// �ٴ�(Floor) Smoke�� �ƴ� �͸� �׸���.
			if (FALSE == g_Smoke[i].m_bFloorSmoke)
			{
				// �׷��� ��ǥ ����
//				Smoke_CalculateDrawStartXY(i);

				// �׸���
				Smoke_Draw(i);
			}
		}
	}

	// robypark (2004/6/12 12:27
	// �Ѿ��� �׸���. 
	for(i=0;i<MAX_BULLET_NUMBER;i++)
	{
		// robypark (2004/6/12 12:27
		// smoke�� �׷��� ���Ŀ� �׷����� �Ѿ˵鸸 �׸���
		if ((GD.BulletInfo[i]) && (true == g_Bullet[i].m_bLayerAboveVolcano))
		{
			Bullet_CalculateDrawStartXY(i);
			Bullet_Draw(i);
		}
	}

	// ���� ĳ���͸� �׸���. 
	for(i=0;i<GD.AirCharNumber;i++)
	{
		id=GD.AirCharID[i].ID;
		if(GD.CharInfo[id])
		{
			CR[id].DecideFont();
			CR[id].CalculateDrawStartXY();

			CR[id].Draw();
		}
	}


	// ������ �׸���. 
/*	for(i = 0;i < MAX_CLOUD_NUMBER;i++)
	{
		if(GD.Rain.CloudSwitch[i])
		{
			TransFromRealDotToScreenDot((SHORT)GD.Rain.CloudPos[i].x, (SHORT)GD.Rain.CloudPos[i].y, screenx, screeny); 

			sifont =  (i  +  GD.Frame/3) % 15;

			if(clGrp.IsInClipArea(screenx, screeny, 
				screenx + GetFileXsize(FILE_GAME_CLOUD) -1, 
				screeny + GetFileYsize(FILE_GAME_CLOUD) -1)==TRUE)	// hojae_repair
			{
				if(i%2)
					Draw_PutSpriteLightImjinT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append
				else
					Draw_PutSpriteLightImjinRT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append

			}
			else
			{
				if(i%2)
					Draw_PutSpriteLightImjinCT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append
				else
					Draw_PutSpriteLightImjinCRT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append

			}
		}
	}
*/

	return TRUE;
}


BOOL _Map::DrawDragBox(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	int min_x, max_x;
	int min_y, max_y;
	RECT rect;

	clGrp.GetClipRect(&rect);

	min_x=max(min(x1, x2), rect.left);
	max_x=min(max(x1, x2), rect.right);
	min_y=max(min(y1, y2), rect.top);
	max_y=min(max(y1, y2), rect.bottom);

	clGrp.BoxC(min_x, min_y, max_x, max_y, 0, 63, 0);

	return TRUE;
}

BOOL _Map::IsInCrop(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT &length)
{
	SHORT file=0;
	SHORT font=0;
	SHORT type=0;
	SHORT vary=0;
	SHORT x, y;

	// ��ľ��, 
	if(IsCrop(mapx, mapy)==FALSE)return FALSE;

	// ��Ȯ�� ���� ���, 
	if(GetCropMountMap(mapx, mapy)==0)return FALSE;

	GetCenterXYByMapCenterXY(mapx, mapy, x, y);

	DecideCropFileNFont(mapx, mapy, file, font);

	// hojae_repair 
	SHORT startx=x-GetFileXsize(file)/2;
	SHORT starty=y-GetFileYsize(file);

	// �����̸�, 
	if(GetCropTypeMap(mapx, mapy)==CROP_TYPE_TREE)
	{
//		starty=y-(GameSpr[file].Header.Ysize-8);
		starty=y-(GetFileYsize(file)-8);	// hojae_repair
	}
	else 
	{
//		starty=y-(GameSpr[file].Header.Ysize-16);
		starty=y-(GetFileYsize(file)-16);	// hojae_repair
	}

	/*
	if(mousex>=startx && mousex<startx+GameSpr[file].Header.Xsize 
	&& mousey>=starty && mousey<starty+GameSpr[file].Header.Ysize )
	{
		length=max(abs(mousex-(startx+GameSpr[file].Header.Xsize/2)), 
			       abs(mousex-(startx+GameSpr[file].Header.Ysize/2)));
	   return TRUE;
	}
	*/
	// hojae_repair
	if(mousex>=startx && mousex<startx+GetFileXsize(file) 
	&& mousey>=starty && mousey<starty+GetFileYsize(file) )
	{
		length=max(abs(mousex-(startx+GetFileXsize(file)/2)), 
			       abs(mousex-(startx+GetFileYsize(file)/2)));
	   return TRUE;
	}

	return FALSE;
}

// ��� ���ϰ� ��Ʈ ��ȣ�� �����Ѵ�. 
void _Map::DecideCropFileNFont(SHORT mapx, SHORT mapy, SHORT &file, SHORT &font)
{
	
	SHORT type=0;
	SHORT vary=0;
	SHORT maxmount=0;
	
	type=GetCropTypeMap(mapx, mapy);
	vary=GetCropVaryMap(mapx, mapy);
	
	
	switch(type)
	{
	case CROP_TYPE_RICE:
        file=FILE_CROP1;
		font=0+(vary)*10+((GD.Frame/5)%8);
		break;
		
	case CROP_TYPE_POTATO:
		file=FILE_CROP0;
		font=5+(vary)*10;
		break;
		
	case CROP_TYPE_TREE:
		file=FV.TreeFile;
		
		maxmount=MAX_TREE_MOUNT;
		if(GetCropMountMap(mapx, mapy)>=maxmount)
			font=vary*10+0;
		else if(GetCropMountMap(mapx, mapy)==0)
			font=vary*10+2;
		else font=vary*10+1;
		break;
		
	case CROP_TYPE_BAMBOO:
		file=FV.TreeFile;
		
		maxmount=MAX_BAMBOO_MOUNT;
		
		if(GetCropMountMap(mapx, mapy)>=maxmount)
			font = 80 + vary*10+0;
		else if(GetCropMountMap(mapx, mapy)==0)
			font = 80 + vary*10+2;
		else	font = 80 + vary*10+1;
		break;
		
		
	default:
		switch(type)
		{
		case CROP_TYPE_RUIN0:			file=FILE_RUIN0;				font=0;		break;
		case CROP_TYPE_RUIN1:			file=FILE_RUIN0;				font=1;		break;
		case CROP_TYPE_RUIN2:			file=FILE_RUIN0;				font=2;		break;
		case CROP_TYPE_TOMB1:			file=FILE_RUIN0;				font=3;		break;
		case CROP_TYPE_BROKENTREE1:		file=FILE_RUIN0;				font=4;		break;
		case CROP_TYPE_ODDTREE1:		file=FILE_RUIN0;				font=5;		break;
		case CROP_TYPE_DOLL1:			file=FILE_RUIN1; 				font=0;		break;
		case CROP_TYPE_WELL1:			file=FILE_RUIN1; 				font=1;		break;
		case CROP_TYPE_TUTTLE1:			file=FILE_RUIN0; 				font=8;		break;
		case CROP_TYPE_STONE1:			file=FILE_RUIN0; 				font=6;		break;
		case CROP_TYPE_STONE2:			file=FILE_RUIN3; 				font=0;		break;
		case CROP_TYPE_AD11:			file=FILE_RUIN3; 				font=1;		break;
		case CROP_TYPE_AD12:			file=FILE_RUIN3; 				font=2;		break;
		case CROP_TYPE_AD13:			file=FILE_RUIN3; 				font=3;		break;
		case CROP_TYPE_CASTLETOWER:		file=FILE_RUIN_CASTLETOWER; 	font=0;		break;
		case CROP_TYPE_DESTROYEDCASTLE:	file=FILE_RUIN_DESTROYEDCASTLE; font=0;		break;
		case CROP_TYPE_CASTLEGATE:		file=FILE_RUIN_CASTLEGATE;		font=0;		break;
		case CROP_TYPE_CASTLETILE:		file=FILE_RUIN_CASTLETILE; 		font=0;		break;
		case CROP_TYPE_CASTLELARGEGATE:	file=FILE_RUIN_CASTLELARGEGATE; font=0;		break;
		case CROP_TYPE_VOLCANO:			file=FILE_RUIN4; 				font=0+(GD.Frame/4)%4;		break;
		case CROP_TYPE_TEMPLE:			file=FILE_RUIN0; 				font=7;		break;
		case CROP_TYPE_SIGN1:			file=FILE_RUIN1; 				font=2;		break;
		case CROP_TYPE_FLAGK1:			file=FILE_RUIN2; 				font= 0+(GD.Frame/3)%10; break;
		case CROP_TYPE_FLAGC1:			file=FILE_RUIN2; 				font=10+(GD.Frame/3)%10; break;
		case CROP_TYPE_FLAGJ1:			file=FILE_RUIN2; 				font=20+(GD.Frame/3)%10; break;
			
		default:
			SetMessage("139021904");
			file=FILE_CROP0;
			font=0;
			break;
		}
		
		break;
	}
}

// ��� �׸���. 
void _Map::DrawCrop(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table)
{

	SHORT file=0;
	SHORT font=0;
	SHORT maxmount=0;
	
	if(IsCrop(mapx, mapy)==FALSE)return;


	DecideCropFileNFont(mapx, mapy, file, font);

//	SHORT startx=x-GameSpr[file].Header.Xsize/2;
	SHORT startx=x-GetFileXsize(file)/2;	// hojae_repair
	SHORT starty;

	// ������ �볪���̸�. 
	if(GetCropTypeMap(mapx, mapy)==CROP_TYPE_TREE
	|| GetCropTypeMap(mapx, mapy)==CROP_TYPE_BAMBOO)
	{
//		starty=y-(GameSpr[file].Header.Ysize-8);
		starty=y-(GetFileYsize(file)-8);	// hojae_repair
	}
	else
	{
//		starty=y-(GameSpr[file].Header.Ysize-16);
		starty=y-(GetFileYsize(file)-16);	// hojae_repair
	}


//	if(clGrp.IsInClipArea(startx, starty, startx+GameSpr[file].Header.Xsize-1, starty+GameSpr[file].Header.Ysize-1)==TRUE)
	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)	// hojae_repair
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
			/*
			clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
							&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
							*/
			
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_append
			/*
			clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize,GameSpr[file].Header.Ysize,
							&GameSpr[file].Image[GameSpr[file].Header.Start[font]], table);
							*/
		}
	}
	else
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_append
		}
	}

}

// ���ڸ� �׸���. 
void _Map::DrawMine(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table)
{

	SHORT file=FILE_GAME_CANNON0;
	SHORT font=30;
	SHORT maxmount=0;

	/*
	SHORT startx=x-GameSpr[file].Header.Xsize/2;
	SHORT starty=y-(GameSpr[file].Header.Ysize-8);
	*/
	// hojae_repair
	SHORT startx=x-GetFileXsize(file)/2;
	SHORT starty=y-(GetFileYsize(file)-8);

//	if(clGrp.IsInClipArea(startx, starty, startx+GameSpr[file].Header.Xsize-1, starty+GameSpr[file].Header.Ysize-1)==TRUE)
	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)	// hojae_repair
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
			/*
        clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize,  GameSpr[file].Header.Ysize,
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
						*/
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_append
			/*
        clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]], table);
						*/
		}
	}
	else
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_dd
		}
	}

}



SI32 _Map::GetFireLifeTime(SI32 x, SI32 y)
{
	// ���� ���� ������ ���Ѵ�. 
	return (GetFireMap(x, y) & 0x1f);
}

// ���� ���� ���� �׸���.
void _Map::DrawFire(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char* table)
{

	SHORT file=0;
	SHORT font=0;
	SHORT maxmount=0;
	
	if(GetFireMap(mapx, mapy) == 0)return;

	file  = FILE_GROUNDFIRE;
	font  = (GD.Frame / 3) % 29;


	SHORT startx = x - GetFileXsize(file) / 2;
	SHORT starty = y - GetFileYsize(file) + 26;

	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)	// hojae_repair
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);	// hojae_append
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);	// hojae_append
	}

}


void _Map::DrawTreasure(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table, SHORT type)
{
}



// x, y:2�����ǥ 
BOOL _Map::DrawMousePos(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table, SHORT mode)
{
	
	SHORT startx, starty;
	SHORT file, font;
	int sx1, sy1, sx2, sy2;

	startx = centerx - MAP_TILE_XSIZE_HALF + 1;

	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
 	    starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;

	if( pLevelFile )	file = pLevelFile[x+YOff[y]];
	if( pLevelFont )	font = pLevelFont[x+YOff[y]];

	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize*2)-1, starty+(TileSpr[file].Header.Ysize*3)-1,
		                sx1, sy1, sx2, sy2);

	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;

	// Ÿ���� �׸���. 
	SHORT xcount=TileSpr[file].Header.HRes/TileSpr[file].Header.Xsize;  // ���ٴ� Ÿ���� ���� 
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;

	unsigned char *ctable;

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{

		if(mode==TRUE)
		{
			ctable=TeamLightTable;
		}
		else
		{
			if((GD.Frame/3)%2==0)
			{
				ctable=TeamLightTable;
			}
			else
			{
				ctable=TeamDarkTable[0];
			}
		}

		clGrp.PutYTileDot(startx, starty, 
                    ysize,
	               &TileSpr[file].Image[TileSpr[file].Header.Start[font]], 
				   ctable);
	}

		return TRUE;

}


// ���� ��Ҹ� �׸��� �Լ��� 
// ����� �׸���. 
BOOL _Map::DrawHill(SHORT centerx, SHORT centery, SHORT x, SHORT y, SHORT mode)
{
	
	SHORT startx, starty;
	SHORT file, font;
	int sx1, sy1, sx2, sy2;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
		starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;
	
	
	
	if( pLevelFile )	file = pLevelFile[x+YOff[y]];
	if( pLevelFont )	font = pLevelFont[x+YOff[y]];

	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);

	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;

	
    SHORT k, l;
	SHORT drawswitch=FALSE;
	
	for(k=boxy1;k<=boxy2;k++)
		for(l=boxx1;l<=boxx2;l++)
		{
			
			if(DrawMapPart[l][k]==TRUE)
			{
				drawswitch=TRUE;
			}
		}

	 // �׸��� ��尡 �������̸�, 
    if(mode==FALSE)
    {
		if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
		else if(drawswitch==FALSE)return FALSE;
    }

	DrawTile(file, font, x, y, startx, starty);

	return TRUE;
}


void _Map::DrawTile(SI32 file, SI32 font, SI32 x, SI32 y, SI32 startx, SI32 starty)
{
	
	SI32 i;
	unsigned char *table = NULL;

	// Ÿ���� �׸���. 
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;

	// ��ο� ������ ���Ѵ�.

	// ������ ���� ���� ���� ó���� ���� ���� �ʴ´�. 
	if(GD.BlackMap2[x][y]==0)
	{
		table = NULL;
	}
	else
	{
		// ���� ��ο� ���� �������� �Ѵ�. 
		SI32 data = 0;
		SI32 max_data = 0;
		SI32 sum = 0;
		for(i=0;i<4;i++)
		{
			
			data = GD.GetBlackMap2(x, y, i);
			
			if(data > max_data)
			{
				max_data = data;
			}
		}
		
		for(i=0;i<4;i++)
		{
			data = GD.GetBlackMap2(x, y, i);
			if(data == max_data)
			{
				sum += (1<<i);
			}
		}
		if(max_data==0)return ;
		
		table = FogImage[GetHillType(x, y)][(max_data-1)][fog[sum]];
	}


	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{

		if(table == NULL)
		{
			clGrp.PutYTileM(startx, starty, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{ 
			clGrp.PutYTileLight3(startx, starty, xsize, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}

	}
	else
	{
		if(table == NULL)
		{
			clGrp.PutYTileCM(startx, starty, xsize, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{ 
			clGrp.PutYTileCLight3(startx, starty, xsize, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}

	}
}

// ���� ��Ҹ� �׸��� �Լ��� 
// ����� �׸���. 
BOOL _Map::DrawHillBlack(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{
	
	SHORT startx, starty;
	SHORT font;
	int sx1, sy1, sx2, sy2;
	SI32 file = 0;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
		starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;
	
	
	
	if( pLevelFile )	file = pLevelFile[x+YOff[y]];
	if( pLevelFont )	font = pLevelFont[x+YOff[y]];

	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);

	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;

	
    SHORT k, l;
	SHORT drawswitch=FALSE;
	
	for(k=boxy1;k<=boxy2;k++)
		for(l=boxx1;l<=boxx2;l++)
		{
			
			if(DrawMapPart[l][k]==TRUE)
			{
				drawswitch=TRUE;
			}
		}

	// ȭ���� ����Ǹ�, �׸���. 
	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
	else if(drawswitch==FALSE)return FALSE;


	// Ÿ���� �׸���. 
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;


     font=GetHillType(x, y);

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{

		clGrp.PutYTileM(startx, starty, ysize, &TileSpr[FileTileBlackTile].Image[TileSpr[FileTileBlackTile].Header.Start[font]]);
	}
	else
	{
		clGrp.PutYTileCM(startx, starty, xsize, ysize, &TileSpr[FileTileBlackTile].Image[TileSpr[FileTileBlackTile].Header.Start[font]]);
	}

		return TRUE;

}



// ���� ���� �׸���. 
// x, y:2�����ǥ 
BOOL _Map::DrawShallow(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table)
{
	if( pShallowFont == NULL ) return TRUE;

	SHORT startx, starty;
	SHORT file=FileTileShallow;
	SHORT font;
	
	
	startx=centerx-TileSpr[file].Header.Xsize/2;
	starty=centery-TileSpr[file].Header.Ysize/2-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;//-16;
	
	

	font = (pShallowFont[x + YOff[y]]%4)*4 + GD.Rain.WaterLevel;
	
	if(clGrp.IsInClipArea(startx, starty, startx+TileSpr[file].Header.Xsize-1, starty+TileSpr[file].Header.Ysize-1)==TRUE)
	{
		if(table == NULL)
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}
	}
	else
	{
		if(table == NULL)
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}
	}
	
	return TRUE;
}


// ���� �׸���. 
// x, y:2�����ǥ 
BOOL _Map::DrawSea(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{

	SHORT tx=x/2;
	SHORT ty=y/2;
	int sx1, sy1, sx2, sy2;
	
	SHORT startx, starty;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
		starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;
	
	
	SHORT file=GetSeaFile(tx, ty);
	SHORT font=GetSeaFont(tx, ty);
	
	font+=((y%2)*2);
	font+=(x%2);
	
	
	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		sx1, sy1, sx2, sy2);
	
	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;
	
    SHORT k, l;
	SHORT drawswitch=FALSE;
	
	for(k=boxy1;k<=boxy2;k++)
	{
		for(l=boxx1;l<=boxx2;l++)
		{
			
			if(DrawMapPart[l][k]==TRUE)
			{
				drawswitch=TRUE;
			}
		}
	}
		
	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE)
	{
	}
	else if(drawswitch==FALSE)
		return FALSE;
		

	DrawTile(file, font, x, y, startx, starty);
		
	return TRUE;
}

// x, y:2�����ǥ 
BOOL _Map::DrawNewBlk(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{

	SHORT tx=x/2;
	SHORT ty=y/2;
	int sx1, sy1, sx2, sy2;

	SHORT startx, starty;

	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty = centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
 	    starty = centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;

	
	SHORT file=GetNewBlkFile(tx, ty);
	SHORT font=GetNewBlkFont(tx, ty);
	
	font+=((y%2)*2);
	font+=(x%2);

	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);


	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;
	
    SHORT k, l;
	SHORT drawswitch=FALSE;

	for(k=boxy1;k<=boxy2;k++)
	{
	   for(l=boxx1;l<=boxx2;l++)
	   {	
		  if(DrawMapPart[l][k]==TRUE)
		  {
			  drawswitch=TRUE;
		  }
	   }
	}

	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE)
	{
	}
	else if(drawswitch==FALSE)
		return FALSE;

	DrawTile(file, font, x, y, startx, starty);

	return TRUE;
}


// �����׸���.
// x, y:2�����ǥ 
BOOL _Map::DrawCastle(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{
	SHORT tx = x / 2;
	SHORT ty = y / 2;
	int sx1, sy1, sx2, sy2;

	SHORT startx, starty;

	startx = centerx - MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y) == HILL_PLANE)  // ��� 
	{
		starty = centery - MAP_TILE_YSIZE_HALF - (GetLevel(x,y) - 1) * HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
 	    starty = centery - MAP_TILE_YSIZE_HALF - (GetLevel(x,y))     * HEIGHT_PER_LEVEL;

	
	SHORT file=GetCastleFile(tx, ty);
	SHORT font=GetCastleFont(tx, ty);
	
	font+=((y%2)*2);
	font+=(x%2);

	
	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);


	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;
	
    SHORT k, l;
	SHORT drawswitch=FALSE;

	for(k=boxy1;k<=boxy2;k++)
	{
	   for(l=boxx1;l<=boxx2;l++)
	   {
	
		  if(DrawMapPart[l][k]==TRUE)
		  {
			  drawswitch=TRUE;
		  }
	   }
	}

	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
	else if(drawswitch==FALSE)return FALSE;

	DrawTile(file, font, x, y, startx, starty);

   return TRUE;
}

void _Map::DrawBuilding(SHORT kind, SHORT buildx, SHORT buildy, SHORT canbuildswitch, SHORT direction)
{

	SHORT file, font;

	// �Ǽ��� �׸��� �׸���.
	if(kind==0)return ;

	SHORT drawstartx;
	SHORT drawstarty;


	SHORT xsize=KI[kind].m_ssIDXsize;
	SHORT ysize=KI[kind].m_ssIDYsize;
		
	SHORT centerx, centery;

	// ��ȿ�� �����̸� �׸��� �ʴ´�. 
	if(IsInMapArea(buildx-xsize/2+xsize-1, buildy-ysize/2+ysize-1) == FALSE)return ;


	file = KI[kind].m_clCharAnimation[1].siFile;
	font = KI[kind].m_clCharAnimation[1].siFont;

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	if( file )
	{
		if(Return_Image(file) == NULL)
			LoadImgFile(file);
	}

	

	GetCenterXYByMapCenterXY(buildx-xsize/2+xsize-1, buildy-ysize/2+ysize-1,  centerx, centery);

	// hojae_repair
	SHORT drawxsize=GetFileXsize(file);
 	SHORT drawysize=GetFileYsize(file);

	drawstartx=centerx-drawxsize/2;
	drawstarty=centery+KI[kind].m_ssBottomIndent-drawysize  + MAP_TILE_YSIZE_HALF;//(drawysize-GameSpr[ KI[kind].File ].Header.Ysize)+KI[kind].BottomIndent;


	//clGrp.BoxC(drawstartx, drawstarty, drawstartx+ drawxsize-1, drawstarty+drawysize-1, 100);

	// �ݴ�� ��´�. 
	if(direction==EAST)
	{
	   if(clGrp.IsInClipArea(drawstartx, drawstarty, drawstartx+drawxsize-1, drawstarty+drawysize-1)==TRUE)
	   {

		    if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
			{
				Draw_PutSpriteDotRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
			}
		    else
			{
				Draw_PutSpriteDotRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
			}
	   }
	   else
	   {
	 	   if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
		   {
			   Draw_PutSpriteDotCRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
		   }
		   else
		   {
			   Draw_PutSpriteDotCRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
		   }
	   }

	}
	else
	{
	   if(clGrp.IsInClipArea(drawstartx, drawstarty, drawstartx+drawxsize-1, drawstarty+drawysize-1)==TRUE)
	   {

		    if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
			{
				Draw_PutSpriteDotT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
			}
		    else
			{
				Draw_PutSpriteDotT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
			}
	   }
	   else
	   {
	 	   if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
		   {
			   Draw_PutSpriteDotCT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
		   }
		   else
		   {
			   Draw_PutSpriteDotCT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
		   }
	   }
	}
}

// �÷��̾� ���� ��ġ�� �׷��ش�. 
void _Map::DrawPlayerStartPoint(SHORT x, SHORT y, SHORT pnumber)
{

	SHORT drawstartx;
	SHORT drawstarty;

	SHORT file=GetValueOfGameSprNumID("FILE_PLAYERSTART");
	SHORT font=0;
		
	SHORT centerx, centery;

	GetCenterXYByMapCenterXY(x, y, centerx, centery);

	/*
	SHORT drawxsize=GameSpr[file].Header.Xsize;
 	SHORT drawysize=GameSpr[file].Header.Ysize;
	*/
	// hojae_repair
	SHORT drawxsize=GetFileXsize(file);
 	SHORT drawysize=GetFileYsize(file);

	drawstartx=centerx-drawxsize/2;
	drawstarty=centery-drawysize/2;

	Draw_PutSpriteT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
}





#else


//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <CharOrg.h>
#include <Capture.h>
//#include <OnlineWorld.h>

#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <bullet.h>
#include <smoke.h>
#include <map.h>

extern _clCapture		Capture;							// ȭ�� ĸ�� ���� Ŭ���� 
extern	_Map 				Map;
extern	_MainData			GD;									// ������ �ֵ����� 
extern	unsigned char		DarkTable[][256];					// ��ο� �ܰ� 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	RECT				ScreenRect;							// ������ ǥ�õǴ� ���� 
extern	SHORT 				MapEditMode;						// ���� ���� ����ΰ�?
extern	_FileVary 			FV;
extern	XSPR 				TileSpr[];							// ���� ��Ʈ�� Ÿ�� 
extern	unsigned char		TeamDarkTable[][256];
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 

extern	unsigned char		TeamLightTable[];
extern	unsigned char		CTable2[];
extern	unsigned char		BlackTable[];
extern	SI32				BlackStatus, DarkStatus;

// ĳ���� �׸���
extern	LPDIRECTDRAWSURFACE7 lpScreenBuffer;

extern	SHORT				OldVirtualMapX, OldVirtualMapY;
extern	char				DrawMapPart[][MAX_DRAWMAP_SCREEN_YSIZE];
extern	unsigned char		FogType[];
extern	unsigned char		FogTable[];
extern	unsigned char		FogImage[][2][15][64*48];

extern	_Tile Tile[];


SI16 DrawMapScreen[MAX_MAP_XSIZE][MAX_MAP_YSIZE];

int fog[] = {
-1,	//0
0,	//1
1,	//2
4,	//3
3,	//4
7,	//5
13,  //6
11, //7
2,   //8
12,	//9
5,	//10
8,	//11
6,	//12
10,	//13
9,	//14
14	//15

};

// ȭ�鿡 ���� ������ �ӽ÷� �����ϴ� �� 
UI08 ScreenMapBuffer[CLIENT_SCREEN_XSIZE*CLIENT_SCREEN_YSIZE];

unsigned char* MapCaptureBuffer;

BOOL _Map::DrawMapEntire()
{
	SHORT i, j;
	int xsize, ysize;
	int centerx, centery;
	int x1, y1, x2, y2;
	SHORT id;

	xsize=MH.MapXsize*64;
	ysize=MH.MapYsize*32+200;

	MapCaptureBuffer=new unsigned char [xsize*ysize];
	if(MapCaptureBuffer==0)return FALSE;

	VirtualMapX=0;
	VirtualMapY=0;
	Map.DrawForceSwitch=TRUE;

   for(i=0;i<MH.MapYsize;i++)
       for(j=0;j<MH.MapXsize;j++)
	   {
	     GD.BlackMap2ChangedSwitch[j][i]=TRUE;
	   }


	clGrp.LockSurface(MapCaptureBuffer, xsize, ysize);

	clGrp.SetScreenXsize(xsize);
	clGrp.SetScreenYsize(ysize);

	clGrp.GetClipArea(x1, y1, x2, y2);
	clGrp.SetClipArea(0, 0, xsize-1, ysize-1);

	for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {
		   centerx=xsize/2+
				+j*MAP_TILE_XSIZE_HALF
				-i*MAP_TILE_XSIZE_HALF;

		   centery=200+
				+i*(MAP_TILE_YSIZE/2) 
				+j*(MAP_TILE_YSIZE/2);
		   
		   // ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
		   if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)
		   {
			   DrawHillBlack(centerx, centery, j, i);
		   }
		   else
		   {
			   
			   // ���� ������ 			   
			   /*if(GetSeaFile(j/2, i/2))
			   {
				   DrawSea(centerx, centery, j, i);
			   }
			   */
			   if(GetSeaFile(j, i))
			   {
				   DrawSea(centerx, centery, j, i);
			   }
			   // ��ֹ��� ����� ������ 
			   else if(GetNewBlkFile(j/2, i/2))
			   {
				   DrawNewBlk(centerx, centery, j, i);
			   }
			   // ���� ������ 
			   else if(GetCastleFile(j/2, i/2))
			   {
				   DrawCastle(centerx, centery, j, i);
			   }
			   // �Ϲ� ����� ������ 
			   else
				   DrawHill(centerx, centery, j, i, FALSE);
		   }
		   
		   GD.BlackMap2ChangedSwitch[j][i]= FALSE;
		   
	   }

   if( pShallowMap )
   {
	   // ���� ���� �׸���. 
	   for(i=0;i<MH.MapYsize;i++)
		   for(j=0;j<MH.MapXsize;j++)
		   {
			   centerx=xsize/2+
				   +j*MAP_TILE_XSIZE_HALF
				   -i*MAP_TILE_XSIZE_HALF;
			   
			   centery=200+
				   +i*(MAP_TILE_YSIZE/2) 
				   +j*(MAP_TILE_YSIZE/2);


			   // ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			   if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			   else
			   {
				   	SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

				   
				   if( pShallowMap[j + YOff[i]] && GD.Rain.WaterLevel<(MAX_WATER_LEVEL_STEP-1) )
				   {
					   DrawShallow(centerx, centery, j, i, DarkTable[darkstep]);
				   }
			   }
		   }
   }
			
	// ���ڸ� �׸���. 
   for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {

		   centerx=xsize/2+MapCenterX[j][i];
	       centery=200+MapCenterY[j][i];


	
			// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			else
			{

				SHORT darkstep=GD.GetBlackMap2(j, i);
				darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

		       // ��Ȯ�� ���� ����� �׷��ش�. 
			   if(GetCropTypeMap(j, i)==CROP_TYPE_TREE && GetCropMountMap(j, i)==0)
			   {
			      DrawCrop(centerx, centery, j, i, DarkTable[darkstep]);
			   }
				// ���ڸ� ����ش�. 
			   else if(GetCropTypeMap(j, i)==CROP_TYPE_POTATO)
			   {
			      DrawCrop(centerx, centery, j, i, DarkTable[darkstep]);
			   }

			}
	   }

   // ���ڸ� �׸���. 
   for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {
		   centerx=xsize/2+MapCenterX[j][i];
	       centery=200+MapCenterY[j][i];

		
			// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			else
			{
				SHORT darkstep=GD.GetBlackMap2(j, i);
				darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

				if( GetMineMap(j, i) >=0 
				&& GD.IsFriendTo(GetMineMap(j, i), UserPlayerNumber) == TRUE )
				{
					DrawMine(centerx, centery, j, i, DarkTable[darkstep]);
				}
			}
	   }
		   
   for(i=0;i<MH.MapYsize;i++)
	   for(j=0;j<MH.MapXsize;j++)
	   {
		   centerx=xsize/2+MapCenterX[j][i];
	       centery=200+MapCenterY[j][i];

			    
			// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)continue;
			else
			{
				SHORT darkstep=GD.GetBlackMap2(j, i);
				darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

				// ĳ���͸� �׸���. 
				// ��ü�� �׸��� �ʴ´�.
				id=GetIDMap(j, i);
				if(GD.CharInfo[id] && CR[id].CurrentOrder!=ORDER_DEAD && CR[id].CurrentOrder!=ORDER_CARCASS)
				{
				   CR[id].DecideFont();
				   CR[id].CalculateDrawStartXYInMapCaptureMode();
				   CR[id].m_ssDrawStartX+=(SHORT)(xsize/2);
				   CR[id].m_ssDrawStartY+=200;
				   CR[id].m_cltCharDrawInfo.SetDrawFrame(-1);
				   CR[id].Draw();
				}

				// ��� �׸���. 
				if(GetCropTypeMap(j, i)==CROP_TYPE_RICE 
                || GetCropTypeMap(j, i)>=CROP_TYPE_RUIN0
				|| (GetCropTypeMap(j, i)==CROP_TYPE_TREE && GetCropMountMap(j, i)) )
				{
				   DrawCrop(centerx, centery, j, i, DarkTable[darkstep]);
				}
			}
	   }


   // ���� ĳ���͸� �׸���. 
   for(i=0;i<GD.AirCharNumber;i++)
   {
	 id=GD.AirCharID[i].ID;
	 if(GD.CharInfo[id])
	 {
		 CR[id].DecideFont();
		 CR[id].CalculateDrawStartXYInMapCaptureMode();
		 CR[id].m_ssDrawStartX+=(SHORT)(xsize/2);
		 CR[id].m_ssDrawStartY+=200;
		 CR[id].m_cltCharDrawInfo.SetDrawFrame(-1);
		 CR[id].Draw();
	 }
   }
 	

	Capture.MapCapture(MapCaptureBuffer, xsize, ysize);

	delete[] MapCaptureBuffer;

	// ȭ���� ������ �����Ѵ�. 
	ScreenRect.left=0;
	ScreenRect.right=CLIENT_SCREEN_XSIZE-1;
	ScreenRect.top=0;
	ScreenRect.bottom=CLIENT_SCREEN_YSIZE-1;

	clGrp.SetClipArea((SHORT)ScreenRect.left, (SHORT)ScreenRect.top, (SHORT)ScreenRect.right, (SHORT)ScreenRect.bottom);

	clGrp.SetScreenXsize(640);
	clGrp.SetScreenYsize(480);

	return TRUE;
}


// ���� Ÿ���� �׸���. 
BOOL _Map::DrawMap()
{
 
	int i, j;
	SHORT centerx, centery;
	int clipx1, clipy1, clipx2, clipy2;
	SI32 data, temp;

	// ���� ���� ��尡 �����Ǿ� ������ ��ŵ���� �ʰ� ��� �׸���. 
	if(MapEditMode==TRUE)
	    DrawForceSwitch=TRUE;

	///////////////////////////////////////////////////////////////////////////////////
	// ���� ���α׷��� ������ ȭ�� ������ ������Ʈ�Ѵ�.
	for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
		for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				
				if(GD.BlackMap[UserPlayerNumber][j][i] == FOG2_BLACK_STEP)
				{
					if(GD.BlackMap2[j][i] != BlackStatus)
					{
						GD.BlackMap2[j][i] = BlackStatus;
					}
				}
				else if(GD.BlackMap[UserPlayerNumber][j][i] == FOG2_DARK_STEP)
				{
					if(GD.BlackMap2[j][i] != DarkStatus)
					{
						GD.BlackMap2[j][i] = DarkStatus;
					}
				}
				else 
				{
					if(GD.BlackMap2[j][i] != 0)
					{
						GD.BlackMap2[j][i] = 0;
					}
				}
			}
		}

		
		// ���� �帷�� �׸���. 
		for(int count=0;count<2;count++)
		{
			for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
				for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
				{
					if(IsInMapArea(j, i)==TRUE)
					{
						
						SI32 max = 0;
						// �ֺ��� ��Ȳ�� �м��Ѵ�. 
						for(int k =0;k<4;k++)
						{
							data = GD.GetBlackMap2(j, i, k);
							
							if(k==0)
							{
								temp = GD.GetBlackMap2(j-1, i-1, 3);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j,   i-1, 2);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j-1, i,	1);
								if(temp > data)data = temp;
							}
							if(k==1)
							{
								temp = GD.GetBlackMap2(j, i-1, 3);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j+1,   i-1, 2);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j+1, i,	0);
								if(temp > data)data = temp;
							}
							if(k==2)
							{
								temp = GD.GetBlackMap2(j-1, i, 3);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j-1,   i+1, 1);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j, i+1,	0);
								if(temp > data)data = temp;
							}
							
							if(k==3)
							{
								temp = GD.GetBlackMap2(j+1, i, 2);
								
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2( j,   i+1, 1);
								if(temp > data)data = temp;
								temp = GD.GetBlackMap2(j+1, i+1,	0);
								if(temp > data)data = temp;
							}
							
							if(data > max)
							{
								max = data;
							}
							if(data > GD.GetBlackMap2(j, i, k))
							{
								GD.SetBlackMap2(j, i, k, data);
							}
						}
						
						for(k=0;k<4;k++)
						{
							if(GD.GetBlackMap2( j, i, k) < (max-1))
							{
								GD.SetBlackMap2(j, i, k, max-1);
							}
						}
						
					}
				}
		}

	   count=0;
	   for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
		   for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
		   {
			   if(IsInMapArea(j, i)==TRUE)
			   {
				   if(GD.BlackMap2[j][i] != DrawMapScreen[j][i])
				   {
					   DrawMapScreen[j][i] = GD.BlackMap2[j][i];
					   GD.BlackMap2ChangedSwitch[j][i] = TRUE;
					   count++;
				   }
			   }
		   }
/////////////////////////////////////////////////////////////////////////////////////


	if(clGrp.LockSurface(ScreenMapBuffer, CLIENT_SCREEN_XSIZE, CLIENT_SCREEN_YSIZE)==FALSE)return FALSE;
	
	   clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	   clGrp.SetClipArea(0, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1);


	if(MapDrawSwitch==TRUE || DrawForceSwitch==TRUE )
	{

	   if(DrawForceSwitch==TRUE)
	   {
		   DrawForceSwitch=FALSE;

		   for(i=0;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, (UI08)0);
			   }
		   
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX+1 && VirtualMapY==OldVirtualMapY+1)
	   {

		   memmove(ScreenMapBuffer, &ScreenMapBuffer[CLIENT_SCREEN_XSIZE*MAP_TILE_YSIZE], 
			       CLIENT_SCREEN_XSIZE*(CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE)*sizeof(UI08));

		   for(i=MAX_DRAWMAP_SCREEN_YSIZE-1;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   
				   clGrp.FillBox32(0, CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE, 
					   CLIENT_SCREEN_XSIZE-1, 
					   CLIENT_SCREEN_YSIZE-1, (UI08)0);
			   }
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX-1 && VirtualMapY==OldVirtualMapY-1)
	   {
		   memmove(&ScreenMapBuffer[CLIENT_SCREEN_XSIZE*MAP_TILE_YSIZE], ScreenMapBuffer, 
			       CLIENT_SCREEN_XSIZE*(CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE)*sizeof(UI08));

		   for(i=0;i<1;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1,  MAP_TILE_YSIZE-1, 0);
			   }
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX-1 && VirtualMapY==OldVirtualMapY+1)
	   {
		   for(i=0;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[MAP_TILE_XSIZE+i*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		       for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
			   {
			     DrawMapPart[0][j]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(0, 0, MAP_TILE_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
			   }
	   }
	   // ���� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX+1 && VirtualMapY==OldVirtualMapY-1)
	   {
		   for(i=0;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[MAP_TILE_XSIZE+i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		       for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
			   {
			     DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE-1][j]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(CLIENT_SCREEN_XSIZE - MAP_TILE_XSIZE, 0, CLIENT_SCREEN_XSIZE - 1, CLIENT_SCREEN_YSIZE - 1, 0);
			   }
	   }

	   // ������ ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX && VirtualMapY==OldVirtualMapY+2)
	   {
		   for(i=MAP_TILE_YSIZE;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[(i-MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[0][j]=TRUE;
		   }
		   
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][MAX_DRAWMAP_SCREEN_YSIZE-1]=TRUE;
		   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(0, 0, MAP_TILE_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
				   clGrp.FillBox32(0, CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
			   }

	   }
	   // �ϼ��� ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX-2 && VirtualMapY==OldVirtualMapY)
	   {
		   for(i=CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE-1;i>=0;i--)
		   {
			   memmove(&ScreenMapBuffer[(i+MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[0][j]=TRUE;
		   }
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][0]=TRUE;
		   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {

				   clGrp.FillBox32(0, 0, MAP_TILE_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, MAP_TILE_YSIZE-1, 0);
			   }
	   }
	   // �ϵ��� ��ũ�� 
	   else if((VirtualMapX==OldVirtualMapX && VirtualMapY==OldVirtualMapY-2))
	   {
           VirtualMapX=OldVirtualMapX;
		   for(i=CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE-1;i>=0;i--)
		   {
			   memmove(&ScreenMapBuffer[(i+MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE-1][j]=TRUE;
		   }
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][0]=TRUE;
		   }

		   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
		   {
			   
			   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, MAP_TILE_YSIZE-1, 0);
			   clGrp.FillBox32(CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
		   }
	   }
	   // ������ ��ũ�� 
	   else if(VirtualMapX==OldVirtualMapX+2 && VirtualMapY==OldVirtualMapY)
	   {
		   for(i=MAP_TILE_YSIZE;i<CLIENT_SCREEN_YSIZE;i++)
		   {
			   memmove(&ScreenMapBuffer[(i-MAP_TILE_YSIZE)*CLIENT_SCREEN_XSIZE], 
				       &ScreenMapBuffer[i*CLIENT_SCREEN_XSIZE+MAP_TILE_XSIZE], 
			           (CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE)*sizeof(UI08));
		   }

		   for(j=0;j<MAX_DRAWMAP_SCREEN_YSIZE;j++)
		   {
			   DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE-1][j]=TRUE;
		   }
		   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
		   {
			   DrawMapPart[j][MAX_DRAWMAP_SCREEN_YSIZE-1]=TRUE;
		   }
		   
		   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
		   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
		   {
			   
			   clGrp.FillBox32(0, CLIENT_SCREEN_YSIZE-MAP_TILE_YSIZE, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
			   clGrp.FillBox32(CLIENT_SCREEN_XSIZE-MAP_TILE_XSIZE, 0,CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, 0);
		   }
	   }
	   else if(VirtualMapX!=OldVirtualMapX || VirtualMapY!=OldVirtualMapY)
	   {

		   // ������ ������ ��� �׸���. 
		   for(i=0;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
			   for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
				   DrawMapPart[j][i]=TRUE;
			   }

			   if(VirtualMapX < MAX_SCREEN_HALF_SIZE || VirtualMapX > MH.MapXsize - MAX_SCREEN_HALF_SIZE
			   || VirtualMapY < MAX_SCREEN_HALF_SIZE || VirtualMapY > MH.MapYsize - MAX_SCREEN_HALF_SIZE)
			   {
				   clGrp.FillBox32(0, 0, CLIENT_SCREEN_XSIZE-1, CLIENT_SCREEN_YSIZE-1, (UI08)0);
			   }

	   }
	   else 
	   {

		   for(i=0;i<MAX_DRAWMAP_SCREEN_YSIZE;i++)
		       for(j=0;j<MAX_DRAWMAP_SCREEN_XSIZE;j++)
			   {
			     DrawMapPart[j][i]=FALSE;
			   }
	   }
 

		OldVirtualMapX=VirtualMapX;
		OldVirtualMapY=VirtualMapY;


	   for(i = VirtualMapY - MAX_SCREEN_HALF_SIZE;i < VirtualMapY + MAX_SCREEN_HALF_SIZE;i++)
		   for(j = VirtualMapX - MAX_SCREEN_HALF_SIZE;j < VirtualMapX + MAX_SCREEN_HALF_SIZE;j++)
		   {
			   
			// ���������� ����.
			if(IsInMapArea(j, i) == TRUE)
			{
   		        centerx =- VirtualMapX * MAP_TILE_XSIZE_HALF
				         + VirtualMapY * MAP_TILE_XSIZE_HALF
					     +j*MAP_TILE_XSIZE_HALF
					     -i*MAP_TILE_XSIZE_HALF
						 + ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2;

			   centery = -VirtualMapY*MAP_TILE_YSIZE_HALF
				         -VirtualMapX*MAP_TILE_YSIZE_HALF
					     +ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
					     +i*MAP_TILE_YSIZE_HALF 
					     +j*MAP_TILE_YSIZE_HALF ;

			   // ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
			   if(GD.IsCompleteBlackBlackMap2(j, i) == TRUE)
			   {
				   DrawHillBlack(centerx, centery, j, i);
			   }
			   else
			   {
				   // ���� ������ 
				   
				   /*if(GetSeaFile(j/2, i/2))
				   {
					   DrawSea(centerx, centery, j, i);
				   }*/
				   if(GetSeaFile(j, i))
				   {
					   DrawSea(centerx, centery, j, i);
				   }
				   // ��ֹ��� ����� ������ 
				   else if(GetNewBlkFile(j/2, i/2))
				   {
					   DrawNewBlk(centerx, centery, j, i);
				   }
				   // ���� ������ 
				   else if(GetCastleFile(j/2, i/2))
				   {
					   DrawCastle(centerx, centery, j, i);
				   }
				   // �Ϲ� ����� ������ 
				   else
				   {
					   DrawHill(centerx, centery, j, i, FALSE);
				   }
			   }
			   
			   GD.BlackMap2ChangedSwitch[j][i] = FALSE;

			}
			
		}
  	
	}

	MapDrawSwitch=FALSE;
	DrawForceSwitch=FALSE;
    clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);

	return TRUE;
}


// ���� Ÿ���� ������ �� ��Ҹ� �׸���. 
BOOL _Map::DrawObj()
{

	SHORT i, j;
	SHORT id;
	SHORT centerx, centery;
	SI32 type;

	if( pShallowMap )
	{
		// ���� ���� �׸���. 
		for(i=VirtualMapY-MAX_SCREEN_HALF_SIZE;i<VirtualMapY+MAX_SCREEN_HALF_SIZE;i++)
			for(j=VirtualMapX-MAX_SCREEN_HALF_SIZE;j<VirtualMapX+MAX_SCREEN_HALF_SIZE;j++)
			{
				
				centerx=-VirtualMapX*MAP_TILE_XSIZE_HALF
					+VirtualMapY*MAP_TILE_XSIZE_HALF
					+ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2
					+j*MAP_TILE_XSIZE_HALF
					-i*MAP_TILE_XSIZE_HALF;
				
				centery=-VirtualMapY*MAP_TILE_YSIZE/2
					-VirtualMapX*MAP_TILE_YSIZE/2 
					+ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
					+i*(MAP_TILE_YSIZE/2) 
					+j*(MAP_TILE_YSIZE/2);
				
				if(IsInMapArea(j, i)==TRUE)
				{
					
					// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
					if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)continue;
					else
					{
						SHORT darkstep=GD.GetBlackMap2(j, i);
						darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
						
						if( pShallowMap[j + YOff[i]] && GD.Rain.WaterLevel<(MAX_WATER_LEVEL_STEP-1) )
						{
							DrawShallow(centerx, centery, j, i, DarkTable[darkstep]);
						}
					}
				}
				
			}
	}

	// ���ڸ� �׸���. 
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)continue;
				else
				{
					SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
					
					// ��Ȯ�� ���� ����� �׷��ش�. 
					if((GetCropTypeMap(j, i)==CROP_TYPE_TREE || GetCropTypeMap(j, i)==CROP_TYPE_BAMBOO)&& GetCropMountMap(j, i)==0)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
					// ���ڸ� ����ش�. 
					if(GetCropTypeMap(j, i)==CROP_TYPE_POTATO)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
					// �ٴڿ� �ٴ� �Ｚ����� ��´�. 
					else if(GetCropTypeMap(j, i) == CROP_TYPE_AD12)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
					// ȭ�굵 ��´�. 
					else if(GetCropTypeMap(j, i) == CROP_TYPE_VOLCANO)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
					}
				}
			}
		}
	}
		
	// ���ڸ� �׸���. 
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			
			if(IsInMapArea(j, i)==TRUE)
			{
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)continue;
				else
				{
					
					SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;
					
					if( GetMineMap(j, i) >= 0 
						&& GD.IsFriendTo(GetMineMap(j,i), UserPlayerNumber) == TRUE)
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawMine(cx, cy, j, i, DarkTable[darkstep]);
					}
				}
			}
		}
	}
	
	// ��ü�� �׸���.
	for(i=0;i<GD.CharOrderIndex;i++)
	{
		
		id=GD.CharOrder[i];
		
		if(GD.CharInfo[id] && (CR[id].CurrentOrder==ORDER_DEAD || CR[id].CurrentOrder==ORDER_CARCASS))
		{
			CR[id].DecideFont();
			CR[id].CalculateDrawStartXY();
			CR[id].Draw();
		}
	}

	// ������ �׸���.
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			
			if(IsInMapArea(j, i)==TRUE)
			{
				
				
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteWhiteBlackMap2(j, i ) == TRUE
					&& GetTreasureMap(j,i))
				{
					
					SI32 sitype = ((GetTreasureMap(j, i) & 0xff00) >> 8);
					
					SHORT cx, cy;
					GetCenterXY(j, i, cx, cy);
					DrawTreasure(cx, cy, j, i, NULL, sitype);
				}
			}
		}
	}

	// robypark (2004-5-24, 16:51)
	// ���⸦ �׸���. 
	// �ٴ�(Floor) Smoke�� ��� �׷��ش�.
	for(i = 0; i < MAX_SMOKE_NUMBER; i++)
	{
		if (GD.SmokeInfo[i])
		{
			// �׷��� ��ǥ ����
			Smoke_CalculateDrawStartXY(i);

			// �ٴ�(Floor) Smoke�� ��� �׷��ش�.
			if (TRUE == g_Smoke[i].m_bFloorSmoke)
			{
				// �׷��� ��ǥ ����
//				Smoke_CalculateDrawStartXY(i);

				// �׸���
				Smoke_Draw(i);
			}
		}
	}
	
	for(i=(VirtualMapY-MAX_SCREEN_HALF_SIZE);i<(VirtualMapY+MAX_SCREEN_HALF_SIZE);i++)
	{
		for(j=(VirtualMapX-MAX_SCREEN_HALF_SIZE);j<(VirtualMapX+MAX_SCREEN_HALF_SIZE);j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				// ���� ��ο� �ܰ�� �׸��� �ʴ´�. 
				if(GD.IsCompleteBlackBlackMap2(j, i ) == TRUE)	continue;
				else
				{
					SHORT darkstep=GD.GetBlackMap2(j, i);
					darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

					// ĳ���͸� �׸���. 
					// ��ü�� �׸��� �ʴ´�.
					id=GetIDMap(j, i);

					if(GD.CharInfo[id] && CR[id].CurrentOrder!=ORDER_DEAD && CR[id].CurrentOrder!=ORDER_CARCASS)
					{
						CR[id].DecideFont();
						CR[id].CalculateDrawStartXY();
						CR[id].Draw();
					}

					// ������ ��Ĺ����� �׸���. 
					if( GetCropTypeMap(j, i)  == CROP_TYPE_RICE 
					||  GetCropTypeMap(j, i)  >= CROP_TYPE_RUIN0
					|| ((GetCropTypeMap(j, i)  == CROP_TYPE_TREE || GetCropTypeMap(j, i)  == CROP_TYPE_BAMBOO) && GetCropMountMap(j, i)) )
					{
						type = GetCropTypeMap(j, i);
						if(type  != CROP_TYPE_AD12 
						&& type != CROP_TYPE_VOLCANO)
						{
							SHORT cx, cy;
							GetCenterXYByMapCenterXY(j, i, cx, cy);
							DrawCrop(cx, cy, j, i, DarkTable[darkstep]);
						}
					}

					if(GetFireMap(j, i))
					{
						SHORT cx, cy;
						GetCenterXYByMapCenterXY(j, i, cx, cy);
						DrawFire(cx, cy, j, i, DarkTable[darkstep]);
					}
				}

				if(MapEditMode==TRUE)
				{
					SHORT _screenx, _screeny;
					
					if(IsEmpty(j, i)==FALSE)
					{
						
						GetCenterXY(j, i, _screenx, _screeny);
						clGrp.FillBoxC(_screenx - 1, _screeny - 1, _screenx + 1, _screeny + 1, (UI08)COLOR_GREEN);
						
					}
				}



#ifdef _SHOWMAPCOORDINATE 
				// �� ��ǥ�� �߽��� ��ġ�� ǥ���Ѵ�. 
				SHORT _centerx, _centery;
				SHORT _screenx, _screeny;
				SHORT type = GetHillType(j,i);
				{
					//if(CollMap[j][i] & BLOCK_RUIN)
					{
						if(IsInMapArea(j, i)==TRUE)
						{
							_centerx=MapCenterX[j][i];
							_centery=MapCenterY[j][i];

					        TransFromRealDotToScreenDot(_centerx, _centery, _screenx, _screeny);

				           DrawNumber(_screenx-1, _screeny-1, FILE_TEAMNUMBER, 0, j, 0);
					       DrawNumber(_screenx-1, _screeny-1+10, FILE_TEAMNUMBER, 0, i, 0);
						}
					}
				}
#endif
			}
		}
	}
	// ������� for
		
	// �Ѿ��� �׸���. 
	for(i=0;i<MAX_BULLET_NUMBER;i++)
	{
		// robypark (2004/6/12 12:27
		// smoke�� �׷����� ���� �׷����� �Ѿ˵鸸 �׸����� ����
		if ((GD.BulletInfo[i]) && (false == g_Bullet[i].m_bLayerAboveVolcano))
		{
			Bullet_CalculateDrawStartXY(i);
			Bullet_Draw(i);
		}
	}

	// robypark (2004-5-24, 16:51)
	// ���⸦ �׸���. 
	// �ٴ�(Floor) Smoke�� �ƴ� �͸� �׷��ش�.
	for(i = 0; i < MAX_SMOKE_NUMBER; i++)
	{
		if (GD.SmokeInfo[i])
		{
			// �ٴ�(Floor) Smoke�� �ƴ� �͸� �׸���.
			if (FALSE == g_Smoke[i].m_bFloorSmoke)
			{
				// �׷��� ��ǥ ����
//				Smoke_CalculateDrawStartXY(i);

				// �׸���
				Smoke_Draw(i);
			}
		}
	}

	// robypark (2004/6/12 12:27
	// �Ѿ��� �׸���. 
	for(i=0;i<MAX_BULLET_NUMBER;i++)
	{
		// robypark (2004/6/12 12:27
		// smoke�� �׷��� ���Ŀ� �׷����� �Ѿ˵鸸 �׸���
		if ((GD.BulletInfo[i]) && (true == g_Bullet[i].m_bLayerAboveVolcano))
		{
			Bullet_CalculateDrawStartXY(i);
			Bullet_Draw(i);
		}
	}

/*
	// ���⸦ �׸���. 
	for(i=0;i<MAX_SMOKE_NUMBER;i++)
	{
		if(GD.SmokeInfo[i])
		{
			Smoke_CalculateDrawStartXY(i);
			Smoke_Draw(i);
		}
	}
*/
	// ���� ĳ���͸� �׸���. 
	for(i=0;i<GD.AirCharNumber;i++)
	{
		id=GD.AirCharID[i].ID;
		if(GD.CharInfo[id])
		{
			CR[id].DecideFont();
			CR[id].CalculateDrawStartXY();

			CR[id].Draw();
		}
	}


	// ������ �׸���. 
/*	for(i = 0;i < MAX_CLOUD_NUMBER;i++)
	{
		if(GD.Rain.CloudSwitch[i])
		{
			TransFromRealDotToScreenDot((SHORT)GD.Rain.CloudPos[i].x, (SHORT)GD.Rain.CloudPos[i].y, screenx, screeny); 

			sifont =  (i  +  GD.Frame/3) % 15;

			if(clGrp.IsInClipArea(screenx, screeny, 
				screenx + GetFileXsize(FILE_GAME_CLOUD) -1, 
				screeny + GetFileYsize(FILE_GAME_CLOUD) -1)==TRUE)	// hojae_repair
			{
				if(i%2)
					Draw_PutSpriteLightImjinT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append
				else
					Draw_PutSpriteLightImjinRT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append

			}
			else
			{
				if(i%2)
					Draw_PutSpriteLightImjinCT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append
				else
					Draw_PutSpriteLightImjinCRT(screenx, screeny, FILE_GAME_CLOUD, sifont);	// hojae_append

			}
		}
	}
*/

	return TRUE;
}


BOOL _Map::DrawDragBox(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	int min_x, max_x;
	int min_y, max_y;
	RECT rect;

	clGrp.GetClipRect(&rect);

	min_x=max(min(x1, x2), rect.left);
	max_x=min(max(x1, x2), rect.right);
	min_y=max(min(y1, y2), rect.top);
	max_y=min(max(y1, y2), rect.bottom);

	clGrp.BoxC(min_x, min_y, max_x, max_y, 0, 63, 0);

	return TRUE;
}

BOOL _Map::IsInCrop(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT &length)
{
	SHORT file=0;
	SHORT font=0;
	SHORT type=0;
	SHORT vary=0;
	SHORT x, y;

	// ��ľ��, 
	if(IsCrop(mapx, mapy)==FALSE)return FALSE;

	// ��Ȯ�� ���� ���, 
	if(GetCropMountMap(mapx, mapy)==0)return FALSE;

	GetCenterXYByMapCenterXY(mapx, mapy, x, y);

	DecideCropFileNFont(mapx, mapy, file, font);

	// hojae_repair 
	SHORT startx=x-GetFileXsize(file)/2;
	SHORT starty=y-GetFileYsize(file);

	// �����̸�, 
	if(GetCropTypeMap(mapx, mapy)==CROP_TYPE_TREE)
	{
//		starty=y-(GameSpr[file].Header.Ysize-8);
		starty=y-(GetFileYsize(file)-8);	// hojae_repair
	}
	else 
	{
//		starty=y-(GameSpr[file].Header.Ysize-16);
		starty=y-(GetFileYsize(file)-16);	// hojae_repair
	}

	/*
	if(mousex>=startx && mousex<startx+GameSpr[file].Header.Xsize 
	&& mousey>=starty && mousey<starty+GameSpr[file].Header.Ysize )
	{
		length=max(abs(mousex-(startx+GameSpr[file].Header.Xsize/2)), 
			       abs(mousex-(startx+GameSpr[file].Header.Ysize/2)));
	   return TRUE;
	}
	*/
	// hojae_repair
	if(mousex>=startx && mousex<startx+GetFileXsize(file) 
	&& mousey>=starty && mousey<starty+GetFileYsize(file) )
	{
		length=max(abs(mousex-(startx+GetFileXsize(file)/2)), 
			       abs(mousex-(startx+GetFileYsize(file)/2)));
	   return TRUE;
	}

	return FALSE;
}

// ��� ���ϰ� ��Ʈ ��ȣ�� �����Ѵ�. 
void _Map::DecideCropFileNFont(SHORT mapx, SHORT mapy, SHORT &file, SHORT &font)
{
	
	SHORT type=0;
	SHORT vary=0;
	SHORT maxmount=0;
	
	type=GetCropTypeMap(mapx, mapy);
	vary=GetCropVaryMap(mapx, mapy);
	
	
	switch(type)
	{
	case CROP_TYPE_RICE:
        file=FILE_CROP1;
		font=0+(vary)*10+((GD.Frame/5)%8);
		break;
		
	case CROP_TYPE_POTATO:
		file=FILE_CROP0;
		font=5+(vary)*10;
		break;
		
	case CROP_TYPE_TREE:
		file=FV.GetNowFile();
		font = vary;
		/* �̰� �����������..
		maxmount=MAX_TREE_MOUNT;
		if(GetCropMountMap(mapx, mapy)>=maxmount)
			font=vary*10+0;
		else if(GetCropMountMap(mapx, mapy)==0)
			font=vary*10+2;
		else font=vary*10+1;
		*/
		break;
		
	case CROP_TYPE_BAMBOO:
		file=FV.GetNowFile();
		
		maxmount=MAX_BAMBOO_MOUNT;
		
		if(GetCropMountMap(mapx, mapy)>=maxmount)
			font = 80 + vary*10+0;
		else if(GetCropMountMap(mapx, mapy)==0)
			font = 80 + vary*10+2;
		else	font = 80 + vary*10+1;
		break;
		
		
	default:
		switch(type)
		{
		case CROP_TYPE_RUIN0:			file=FILE_RUIN0;				font=0;		break;
		case CROP_TYPE_RUIN1:			file=FILE_RUIN0;				font=1;		break;
		case CROP_TYPE_RUIN2:			file=FILE_RUIN0;				font=2;		break;
		case CROP_TYPE_TOMB1:			file=FILE_RUIN0;				font=3;		break;
		case CROP_TYPE_BROKENTREE1:		file=FILE_RUIN0;				font=4;		break;
		case CROP_TYPE_ODDTREE1:		file=FILE_RUIN0;				font=5;		break;
		case CROP_TYPE_DOLL1:			file=FILE_RUIN1; 				font=0;		break;
		case CROP_TYPE_WELL1:			file=FILE_RUIN1; 				font=1;		break;
		case CROP_TYPE_TUTTLE1:			file=FILE_RUIN0; 				font=8;		break;
		case CROP_TYPE_STONE1:			file=FILE_RUIN0; 				font=6;		break;
		case CROP_TYPE_STONE2:			file=FILE_RUIN3; 				font=0;		break;
		case CROP_TYPE_AD11:			file=FILE_RUIN3; 				font=1;		break;
		case CROP_TYPE_AD12:			file=FILE_RUIN3; 				font=2;		break;
		case CROP_TYPE_AD13:			file=FILE_RUIN3; 				font=3;		break;
		case CROP_TYPE_CASTLETOWER:		file=FILE_RUIN_CASTLETOWER; 	font=0;		break;
		case CROP_TYPE_DESTROYEDCASTLE:	file=FILE_RUIN_DESTROYEDCASTLE; font=0;		break;
		case CROP_TYPE_CASTLEGATE:		file=FILE_RUIN_CASTLEGATE;		font=0;		break;
		case CROP_TYPE_CASTLETILE:		file=FILE_RUIN_CASTLETILE; 		font=0;		break;
		case CROP_TYPE_CASTLELARGEGATE:	file=FILE_RUIN_CASTLELARGEGATE; font=0;		break;
		case CROP_TYPE_VOLCANO:			file=FILE_RUIN4; 				font=0+(GD.Frame/4)%4;		break;
		case CROP_TYPE_TEMPLE:			file=FILE_RUIN0; 				font=7;		break;
		case CROP_TYPE_SIGN1:			file=FILE_RUIN1; 				font=2;		break;
		case CROP_TYPE_FLAGK1:			file=FILE_RUIN2; 				font= 0+(GD.Frame/3)%10; break;
		case CROP_TYPE_FLAGC1:			file=FILE_RUIN2; 				font=10+(GD.Frame/3)%10; break;
		case CROP_TYPE_FLAGJ1:			file=FILE_RUIN2; 				font=20+(GD.Frame/3)%10; break;
			
		default:
			SetMessage("139021904");
			file=FILE_CROP0;
			font=0;
			break;
		}
		
		break;
	}
}

// ��� �׸���. 
void _Map::DrawCrop(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table)
{

	SHORT file=0;
	SHORT font=0;
	SHORT maxmount=0;
	
	if(IsCrop(mapx, mapy)==FALSE)return;


	DecideCropFileNFont(mapx, mapy, file, font);

	if( GetCropTypeMap(mapx, mapy)==CROP_TYPE_TREE )
	{
		//file�� ����ó��
		int treeindex = GetCropTreeMap(mapx, mapy);
		file = FV.m_TreeFiles[treeindex ];
	}

//	SHORT startx=x-GameSpr[file].Header.Xsize/2;
	SHORT startx=x-GetFileXsize(file)/2;	// hojae_repair
	SHORT starty;

	// ������ �볪���̸�. 
	if(GetCropTypeMap(mapx, mapy)==CROP_TYPE_TREE
	|| GetCropTypeMap(mapx, mapy)==CROP_TYPE_BAMBOO)
	{
//		starty=y-(GameSpr[file].Header.Ysize-8);
		starty=y-(GetFileYsize(file)-8);	// hojae_repair
	}
	else
	{
//		starty=y-(GameSpr[file].Header.Ysize-16);
		starty=y-(GetFileYsize(file)-16);	// hojae_repair
	}


//	if(clGrp.IsInClipArea(startx, starty, startx+GameSpr[file].Header.Xsize-1, starty+GameSpr[file].Header.Ysize-1)==TRUE)
	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)	// hojae_repair
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
			/*
			clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
							&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
							*/
			
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_append
			/*
			clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize,GameSpr[file].Header.Ysize,
							&GameSpr[file].Image[GameSpr[file].Header.Start[font]], table);
							*/
		}
	}
	else
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_append
		}
	}

}

// ���ڸ� �׸���. 
void _Map::DrawMine(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table)
{

	SHORT file=FILE_GAME_CANNON0;
	SHORT font=30;
	SHORT maxmount=0;

	/*
	SHORT startx=x-GameSpr[file].Header.Xsize/2;
	SHORT starty=y-(GameSpr[file].Header.Ysize-8);
	*/
	// hojae_repair
	SHORT startx=x-GetFileXsize(file)/2;
	SHORT starty=y-(GetFileYsize(file)-8);

//	if(clGrp.IsInClipArea(startx, starty, startx+GameSpr[file].Header.Xsize-1, starty+GameSpr[file].Header.Ysize-1)==TRUE)
	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)	// hojae_repair
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
			/*
        clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize,  GameSpr[file].Header.Ysize,
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]]);
						*/
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_append
			/*
        clGrp.PutSpriteT(startx, starty, GameSpr[file].Header.Xsize, GameSpr[file].Header.Ysize,
						&GameSpr[file].Image[GameSpr[file].Header.Start[font]], table);
						*/
		}
	}
	else
	{
		if(table == NULL)
		{
			Draw_PutSpriteT(startx, starty, file, font);	// hojae_append
		}
		else
		{
			Draw_PutSpriteT(startx, starty, file, font, table);	// hojae_dd
		}
	}

}



SI32 _Map::GetFireLifeTime(SI32 x, SI32 y)
{
	// ���� ���� ������ ���Ѵ�. 
	return (GetFireMap(x, y) & 0x1f);
}

// ���� ���� ���� �׸���.
void _Map::DrawFire(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char* table)
{

	SHORT file=0;
	SHORT font=0;
	SHORT maxmount=0;
	
	if(GetFireMap(mapx, mapy) == 0)return;

	file  = FILE_GROUNDFIRE;
	font  = (GD.Frame / 3) % 29;


	SHORT startx = x - GetFileXsize(file) / 2;
	SHORT starty = y - GetFileYsize(file) + 26;

	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)	// hojae_repair
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);	// hojae_append
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);	// hojae_append
	}

}


void _Map::DrawTreasure(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table, SHORT type)
{
}



// x, y:2�����ǥ 
BOOL _Map::DrawMousePos(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table, SHORT mode)
{
	
	SHORT startx, starty;
	SHORT file, font;
	int sx1, sy1, sx2, sy2;

	startx = centerx - MAP_TILE_XSIZE_HALF + 1;

	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
 	    starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;

	if( pLevelFile )	file = pLevelFile[x+YOff[y]];
	if( pLevelFont )	font = pLevelFont[x+YOff[y]];

	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize*2)-1, starty+(TileSpr[file].Header.Ysize*3)-1,
		                sx1, sy1, sx2, sy2);

	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;

	// Ÿ���� �׸���. 
	SHORT xcount=TileSpr[file].Header.HRes/TileSpr[file].Header.Xsize;  // ���ٴ� Ÿ���� ���� 
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;

	unsigned char *ctable;

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{

		if(mode==TRUE)
		{
			ctable=TeamLightTable;
		}
		else
		{
			if((GD.Frame/3)%2==0)
			{
				ctable=TeamLightTable;
			}
			else
			{
				ctable=TeamDarkTable[0];
			}
		}

				//��� �ӽ�..
		if( file < 2)
		{
			clGrp.PutSpriteT(startx, starty, 
                    xsize, ysize,
	               &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
			return TRUE;
				   
		}

		//��� �ӽ�..
		if( file < 2)
		{
			clGrp.PutSpriteT(startx, starty, 
                    xsize, ysize,
	               &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
			return TRUE;
				   
		}

		/*���� �Ż�
		clGrp.PutYTileDot(startx, starty, 
                    ysize,
	               &TileSpr[file].Image[TileSpr[file].Header.Start[font]], 
				   ctable);
				   */
	}

		return TRUE;

}


// ���� ��Ҹ� �׸��� �Լ��� 
// ����� �׸���. 
BOOL _Map::DrawHill(SHORT centerx, SHORT centery, SHORT x, SHORT y, SHORT mode)
{
	
	SHORT startx, starty;
	SHORT file, font;
	int sx1, sy1, sx2, sy2;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
		starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;
	
	
	
	if( pLevelFile )	file = pLevelFile[x+YOff[y]];
	if( pLevelFont )	font = pLevelFont[x+YOff[y]];

	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);

	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;

	
    SHORT k, l;
	SHORT drawswitch=FALSE;
	
	for(k=boxy1;k<=boxy2;k++)
		for(l=boxx1;l<=boxx2;l++)
		{
			
			if(DrawMapPart[l][k]==TRUE)
			{
				drawswitch=TRUE;
			}
		}

	 // �׸��� ��尡 �������̸�, 
    if(mode==FALSE)
    {
		if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
		else if(drawswitch==FALSE)return FALSE;
    }

	DrawTile(file, font, x, y, startx, starty);

	return TRUE;

}


void _Map::DrawTile(SI32 file, SI32 font, SI32 x, SI32 y, SI32 startx, SI32 starty)
{
	
	SI32 i;
	unsigned char *table = NULL;

	// Ÿ���� �׸���. 
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;

	// ��ο� ������ ���Ѵ�.

	// ������ ���� ���� ���� ó���� ���� ���� �ʴ´�. 
	if(GD.BlackMap2[x][y]==0)
	{
		table = NULL;
	}
	else
	{
		// ���� ��ο� ���� �������� �Ѵ�. 
		SI32 data = 0;
		SI32 max_data = 0;
		SI32 sum = 0;
		for(i=0;i<4;i++)
		{
			
			data = GD.GetBlackMap2(x, y, i);
			
			if(data > max_data)
			{
				max_data = data;
			}
		}
		
		for(i=0;i<4;i++)
		{
			data = GD.GetBlackMap2(x, y, i);
			if(data == max_data)
			{
				sum += (1<<i);
			}
		}
		if(max_data==0)return ;
		
		//table = FogImage[GetHillType(x, y)][(max_data-1)][fog[sum]];
		table = FogImage[GetHillType(x, y)][(max_data-1)][fog[sum]];	
	}

	//��� �ӽ�..Ÿ�ϸ� �̷����Ѵ�.
	if( file == FileTileHill0 || file == FileTileGrss1 || file == FileTileDiff1 || file == FileTileHill1)
	{
		clGrp.PutSpriteT(startx, starty, 
			xsize, ysize,
			&TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		return;
		
	}

	//���� �Ż�..
	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{

		if(table == NULL)
		{
			clGrp.PutYTileM(startx, starty, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{ 
			clGrp.PutYTileLight3(startx, starty, xsize, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}

	}
	else
	{
		if(table == NULL)
		{
			clGrp.PutYTileCM(startx, starty, xsize, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{ 
			clGrp.PutYTileCLight3(startx, starty, xsize, ysize, &TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}

	}

}

// ���� ��Ҹ� �׸��� �Լ��� 
// ����� �׸���. 
BOOL _Map::DrawHillBlack(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{
	
	SHORT startx, starty;
	SHORT font;
	int sx1, sy1, sx2, sy2;
	SI32 file = 0;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
		starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;
	
	
	
	if( pLevelFile )	file = pLevelFile[x+YOff[y]];
	if( pLevelFont )	font = pLevelFont[x+YOff[y]];

	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);

	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;

	
    SHORT k, l;
	SHORT drawswitch=FALSE;
	
	for(k=boxy1;k<=boxy2;k++)
		for(l=boxx1;l<=boxx2;l++)
		{
			
			if(DrawMapPart[l][k]==TRUE)
			{
				drawswitch=TRUE;
			}
		}

	// ȭ���� ����Ǹ�, �׸���. 
	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
	else if(drawswitch==FALSE)return FALSE;


	// Ÿ���� �׸���. 
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;


     font=GetHillType(x, y);	 	 
	 
	 //��� �ӽ�..
	 /*if( file < 2)
	 {
		 clGrp.PutSpriteT(startx, starty, 
			 xsize, ysize,
			 &TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		 return TRUE;
		 
	 }*/

	if(clGrp.IsInClipArea(startx, starty, startx+xsize-1, starty+ysize-1)==TRUE)
	{

		clGrp.PutYTileM(startx, starty, ysize, &TileSpr[FileTileBlackTile].Image[TileSpr[FileTileBlackTile].Header.Start[font]]);
	}
	else
	{
		clGrp.PutYTileCM(startx, starty, xsize, ysize, &TileSpr[FileTileBlackTile].Image[TileSpr[FileTileBlackTile].Header.Start[font]]);
	}

		return TRUE;

}



// ���� ���� �׸���. 
// x, y:2�����ǥ 
BOOL _Map::DrawShallow(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table)
{
	if( pShallowFont == NULL ) return TRUE;

	SHORT startx, starty;
	SHORT file=FileTileShallow;
	SHORT font;
	
	
	startx=centerx-TileSpr[file].Header.Xsize/2;
	starty=centery-TileSpr[file].Header.Ysize/2-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;//-16;
	
	

	font = (pShallowFont[x + YOff[y]]%4)*4 + GD.Rain.WaterLevel;
	
	if(clGrp.IsInClipArea(startx, starty, startx+TileSpr[file].Header.Xsize-1, starty+TileSpr[file].Header.Ysize-1)==TRUE)
	{
		if(table == NULL)
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}
	}
	else
	{
		if(table == NULL)
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
		}
		else
		{
			clGrp.PutSpriteT(startx, starty, TileSpr[file].Header.Xsize,TileSpr[file].Header.Ysize,
							&TileSpr[file].Image[TileSpr[file].Header.Start[font]], table);
		}
	}
	
	return TRUE;
}


// ���� �׸���. 
// x, y:2�����ǥ 
BOOL _Map::DrawSea(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{

	//SHORT tx=x/2;
	//SHORT ty=y/2;
	SHORT tx=x;
	SHORT ty=y;
	int sx1, sy1, sx2, sy2;
	
	SHORT startx, starty;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty=centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
		starty=centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;
	
	
	SHORT file=GetSeaFile(tx, ty);
	SHORT font=GetSeaFont(tx, ty);
	
	//font+=((y%2)*2);
	//font+=(x%2);
	
	
	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		sx1, sy1, sx2, sy2);
	
	
	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;
	
    SHORT k, l;
	SHORT drawswitch=FALSE;
	
	for(k=boxy1;k<=boxy2;k++)
		for(l=boxx1;l<=boxx2;l++)
		{
			
			if(DrawMapPart[l][k]==TRUE)
			{
				drawswitch=TRUE;
			}
		}
		
	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
	else if(drawswitch==FALSE)return FALSE;
	
	SHORT xsize=TileSpr[file].Header.Xsize;
	SHORT ysize=TileSpr[file].Header.Ysize;

	clGrp.PutSpriteT(startx, starty, 
		xsize, ysize,
		&TileSpr[file].Image[TileSpr[file].Header.Start[font]]);
	//��� ��..
	//DrawTile(file, font, x, y, startx, starty);
		
		return TRUE;
}

// x, y:2�����ǥ 
BOOL _Map::DrawNewBlk(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{

	SHORT tx=x/2;
	SHORT ty=y/2;
	int sx1, sy1, sx2, sy2;

	SHORT startx, starty;

	startx=centerx-MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y)==HILL_PLANE)  // ��� 
	{
		starty = centery-MAP_TILE_YSIZE_HALF-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
 	    starty = centery-MAP_TILE_YSIZE_HALF-(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL;

	
	SHORT file=GetNewBlkFile(tx, ty);
	SHORT font=GetNewBlkFont(tx, ty);
	
	font+=((y%2)*2);
	font+=(x%2);

	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);


	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;
	
    SHORT k, l;
	SHORT drawswitch=FALSE;

	for(k=boxy1;k<=boxy2;k++)
	   for(l=boxx1;l<=boxx2;l++)
	   {
	
		   if(DrawMapPart[l][k]==TRUE)
		  {
			  drawswitch=TRUE;
		  }
	   }

	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
	else if(drawswitch==FALSE)return FALSE;

	DrawTile(file, font, x, y, startx, starty);

   return TRUE;
}


// �����׸���.
// x, y:2�����ǥ 
BOOL _Map::DrawCastle(SHORT centerx, SHORT centery, SHORT x, SHORT y)
{
	SHORT tx = x / 2;
	SHORT ty = y / 2;
	int sx1, sy1, sx2, sy2;

	SHORT startx, starty;

	startx = centerx - MAP_TILE_XSIZE_HALF;
				
	if(GetLevelInfo(x,y) == HILL_PLANE)  // ��� 
	{
		starty = centery - MAP_TILE_YSIZE_HALF - (GetLevel(x,y) - 1) * HEIGHT_PER_LEVEL;
	}
	else                    // ��� 
 	    starty = centery - MAP_TILE_YSIZE_HALF - (GetLevel(x,y))     * HEIGHT_PER_LEVEL;

	
	SHORT file=GetCastleFile(tx, ty);
	SHORT font=GetCastleFont(tx, ty);
	
	font+=((y%2)*2);
	font+=(x%2);

	
	// Ŭ�� ������ ���߾� ���� ��ġ�� �����Ѵ�.
	clGrp.FitToClipArea(startx, starty, startx+(TileSpr[file].Header.Xsize)-1, starty+(TileSpr[file].Header.Ysize)-1, 
		                sx1, sy1, sx2, sy2);


	SHORT boxx1=sx1/MAP_TILE_XSIZE;
	SHORT boxy1=sy1/MAP_TILE_YSIZE;
	SHORT boxx2=sx2/MAP_TILE_XSIZE;
	SHORT boxy2=sy2/MAP_TILE_YSIZE;
	
    SHORT k, l;
	SHORT drawswitch=FALSE;

	for(k=boxy1;k<=boxy2;k++)
	   for(l=boxx1;l<=boxx2;l++)
	   {
	
		   if(DrawMapPart[l][k]==TRUE)
		  {
			  drawswitch=TRUE;
		  }
	   }

	if(GD.BlackMap2ChangedSwitch[x][y] == TRUE){}
	else if(drawswitch==FALSE)return FALSE;

	DrawTile(file, font, x, y, startx, starty);
	   return TRUE;
}

void _Map::DrawBuilding(SHORT kind, SHORT buildx, SHORT buildy, SHORT canbuildswitch, SHORT direction)
{

	SHORT file, font;

	// �Ǽ��� �׸��� �׸���.
	if(kind==0)return ;

	SHORT drawstartx;
	SHORT drawstarty;


	SHORT xsize=KI[kind].IDXsize;
	SHORT ysize=KI[kind].IDYsize;
		
	SHORT centerx, centery;

	// ��ȿ�� �����̸� �׸��� �ʴ´�. 
	if(IsInMapArea(buildx-xsize/2+xsize-1, buildy-ysize/2+ysize-1) == FALSE)return ;


	file = KI[kind].clCharAnimation[1].siFile;
	font = KI[kind].clCharAnimation[1].siFont;

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	if( file )
	{
		if(Return_Image(file) == NULL)
			LoadImgFile(file);
	}

	

	GetCenterXYByMapCenterXY(buildx-xsize/2+xsize-1, buildy-ysize/2+ysize-1,  centerx, centery);

	// hojae_repair
	SHORT drawxsize=GetFileXsize(file);
 	SHORT drawysize=GetFileYsize(file);

	drawstartx=centerx-drawxsize/2;
	drawstarty=centery+KI[kind].BottomIndent-drawysize  + MAP_TILE_YSIZE_HALF;//(drawysize-GameSpr[ KI[kind].File ].Header.Ysize)+KI[kind].BottomIndent;


	//clGrp.BoxC(drawstartx, drawstarty, drawstartx+ drawxsize-1, drawstarty+drawysize-1, 100);

	// �ݴ�� ��´�. 
	if(direction==EAST)
	{
	   if(clGrp.IsInClipArea(drawstartx, drawstarty, drawstartx+drawxsize-1, drawstarty+drawysize-1)==TRUE)
	   {

		    if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
			{
				Draw_PutSpriteDotRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
			}
		    else
			{
				Draw_PutSpriteDotRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
			}
	   }
	   else
	   {
	 	   if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
		   {
			   Draw_PutSpriteDotCRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
		   }
		   else
		   {
			   Draw_PutSpriteDotCRT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
		   }
	   }

	}
	else
	{
	   if(clGrp.IsInClipArea(drawstartx, drawstarty, drawstartx+drawxsize-1, drawstarty+drawysize-1)==TRUE)
	   {

		    if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
			{
				Draw_PutSpriteDotT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
			}
		    else
			{
				Draw_PutSpriteDotT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
			}
	   }
	   else
	   {
	 	   if(canbuildswitch==FALSE && (GD.Frame/3)%2==0)
		   {
			   Draw_PutSpriteDotCT(drawstartx, drawstarty, drawxsize, drawysize, file, font, TeamLightTable);	// hojae_appned
		   }
		   else
		   {
			   Draw_PutSpriteDotCT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
		   }
	   }
	}
}

// �÷��̾� ���� ��ġ�� �׷��ش�. 
void _Map::DrawPlayerStartPoint(SHORT x, SHORT y, SHORT pnumber)
{

	SHORT drawstartx;
	SHORT drawstarty;

	SHORT file=GetValueOfGameSprNumID("FILE_PLAYERSTART");
	SHORT font=0;
		
	SHORT centerx, centery;

	GetCenterXYByMapCenterXY(x, y, centerx, centery);

	/*
	SHORT drawxsize=GameSpr[file].Header.Xsize;
 	SHORT drawysize=GameSpr[file].Header.Ysize;
	*/
	// hojae_repair
	SHORT drawxsize=GetFileXsize(file);
 	SHORT drawysize=GetFileYsize(file);

	drawstartx=centerx-drawxsize/2;
	drawstarty=centery-drawysize/2;

	Draw_PutSpriteT(drawstartx, drawstarty, drawxsize, drawysize, file, font, CTable2);	// hojae_appned
}


#endif