//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

	#include "..\Gersang\maindata.h"
	#include <colortable.h>
	#include <etc.h>
	#include <OnlineWorld.h>

#include <ysmap.h>
#include <MapInterface.h>
#include <charinterface.h>
#include "..\Gersang\DirectX.h"

#define		SMAP_EVENTMARK_MAX_LOOP					1			// �̺�Ʈ ��ũ�� �� �� �����ؼ� ���� ���ΰ�.
#define		SMAP_EVENTMARK_MAX_DELAY_COUNT_FRAME	2	


extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	XSPR 				PannelSpr;
extern	_MainData			GD;									// ������ �ֵ����� 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	XSPR 				TileSpr[];							// ���� ��Ʈ�� Ÿ�� 
extern _Char   CR[];       // ĳ������ ������ 
extern	unsigned char		DarkTable[][256];					// ��ο� �ܰ� 


SHORT	_YSMap::EventMarkFontArray[YSMAP_EVENTMARK_COLOR_MAX][SMAP_EVENTMARK_MAX_FRAME]=
{
	{0, 1, 2, 3, 4, 5, 6, 7, 8},
	{10, 11, 12, 13, 14, 15, 16, 17, 18}
};

_YSMap	YSMap;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-02-22 ���� 10:24:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
_YSMap::_YSMap()
{
	SI32	i;
		
	for(i = 0; i < YSMAP_MAP_YSIZE; i++)	
		StrechYOffSet[i] = i * YSMAP_MAP_XSIZE;
}

void	_YSMap::Init(SHORT MapXsize, SHORT MapYsize)
{
	LPPOINT		lpStrechYSMapPos = NULL;
	SHORT		x, y, tx, ty, mx, my;
	
	//DWORD		dwStartTime = timeGetTime();

	DecideSmapStartXY();

	if(MapXsize > YSMAP_MAP_XSIZE || MapYsize > YSMAP_MAP_YSIZE)
	{
		clGrp.Error("FKJE8567", "small map init function : received wrong element from _YSMap::Init().");
	}

	//lpYSMap�� �ʱ�ȭ �Ѵ�.
	memset(IsEnablePutDot, FALSE, sizeof(IsEnablePutDot));
	memset(ScreenToMap, 0xff, sizeof(ScreenToMap));
	memset(MapToScreen, 0xff, sizeof(MapToScreen));
	//memset(StrechToScreen, 0xff, sizeof(StrechToScreen));
	//memset(StrechToMap, 0xff, sizeof(StrechToMap));	
	memset(PutPosition, 0xff, sizeof(PutPosition));
	

	PutPositionDotNum = 0;
			
	m_MapXsize		= MapXsize;								//���� �� X ������
	m_MapYsize		= MapYsize;								//���� �� Y ������					

	//������ ���� g_MinYSMapSize�� ���� ȭ�鿡 ���� ����� �ֱ� ����
	//����ϰų� Ȯ���� ���ο� ũ�⸦ ��� �´�.
	GetStrechSize();	

	//�簢�� �ڽ��� ũ�⸦ ���Ѵ�.
	GetBoxSize();

	//��Ʈ��ġ �� �� ��ŭ�� �޸𸮸� �Ҵ� �޴´�.	
	lpStrechYSMapPos = new POINT[m_StrechSize.cx * m_StrechSize.cy];	
	
	for(y=0; y < m_StrechSize.cy; y++)
	{
		for(x=0; x < m_StrechSize.cx; x++)
		{			
			mx = m_MapXsize * x / SHORT(m_StrechSize.cx);
			my = m_MapYsize * y / SHORT(m_StrechSize.cy);			

			//������ �÷����Ƿ� �ø� ��ġ�� ��Ȯ�� �˱� ���� ���� ���� �ε����� ������ �д�.
			lpStrechYSMapPos[y * m_StrechSize.cx + x].x = mx;
			lpStrechYSMapPos[y * m_StrechSize.cx + x].y = my;						

			//tx, ty�� ���� ��ũ������ ��ġ�̴�.
			TranslateStrechToReal(x, y, &tx, &ty);									

			//���� �ε����� ȭ���� ��� ��ġ������ �� �� �ִ�.
			MapToScreen[my * YSMAP_MAP_XSIZE + mx].x = BYTE(tx);
			MapToScreen[my * YSMAP_MAP_XSIZE + mx].y = BYTE(ty);

			//ȭ���� ��ġ�� ���� �ε����� �� �� �ִ�.
			ScreenToMap[ty * YSMAP_XSIZE + tx].x = BYTE(mx);
			ScreenToMap[ty * YSMAP_XSIZE + tx].y = BYTE(my);
			
			//StrechToScreen[y * YSMAP_MAP_XSIZE + x].x = BYTE(tx);
			//StrechToScreen[y * YSMAP_MAP_XSIZE + x].y = BYTE(ty);			

			//StrechToMap[y * YSMAP_MAP_XSIZE +x].x = BYTE(mx);
			//StrechToMap[y * YSMAP_MAP_XSIZE +x].y = BYTE(my);									

			if(IsEnablePutDot[tx][ty] == FALSE)
			{
				IsEnablePutDot[tx][ty]	=	TRUE;
				AddPutPosition(tx, ty, mx, my);
			}			
		}
	}	
	
	
	//Ÿ���� ������ ����.	
	CountTileColor();	

	if(lpStrechYSMapPos)
	{
		delete [] lpStrechYSMapPos;
		lpStrechYSMapPos = NULL;
	}	

	ZeroMemory(&m_EventManagement, sizeof(m_EventManagement));

	//clGrp.Error("", "EndTime : [%d]", timeGetTime() - dwStartTime);
}

void _YSMap::GetStrechSize()
{
	SI32	siMaxSizeOfXY;

	siMaxSizeOfXY	=	max(YSMAP_XSIZE, YSMAP_YSIZE);

	//���� ������ ����� ū�� ����.
	if(m_MapXsize < m_MapYsize)			//���̰� �� ũ��.
	{
		m_StrechSize.cy = siMaxSizeOfXY;
		m_StrechSize.cx = (m_MapXsize * m_StrechSize.cy) / m_MapYsize;
	}
	else if(m_MapXsize > m_MapYsize)		//���� �� ũ��.
	{
		m_StrechSize.cx = siMaxSizeOfXY;
		m_StrechSize.cy = (m_MapYsize * m_StrechSize.cx) / m_MapXsize;
	}
	else 
	{
		m_StrechSize.cx = siMaxSizeOfXY;
		m_StrechSize.cy = siMaxSizeOfXY;
	}
} 

void _YSMap::GetBoxSize()
{
	DWORD	TotalMapXsizeInPixel;
	DWORD	TotalMapYsizeInPixel;

	TotalMapXsizeInPixel = (m_MapXsize + m_MapYsize) * clifMap.GetMapTileXsizeHalf();
	TotalMapYsizeInPixel = (m_MapXsize + m_MapYsize) * clifMap.GetMapTileYsizeHalf();

	m_BoxWidth =  SHORT((clifMap.GetMaxDrawMapScreenXsize() * clifMap.GetMapTileXsize()) * YSMAP_XSIZE / TotalMapXsizeInPixel);
	m_BoxHeight = SHORT((clifMap.GetMaxDrawMapScreenYsize() * clifMap.GetMapTileYsize()) * YSMAP_YSIZE / TotalMapYsizeInPixel);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void _YSMap::TranslateStrechToReal(SHORT x, SHORT y, SHORT* tx, SHORT* ty)
/// ���� : Strech�� x, y�� ���� ����� ���� ��ǥ�� ��ȯ�Ѵ�.
/// �߿䵵 : �ſ� �߿��� �Լ�. ���� ȭ����� ��� ���� ��ġ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////////////////
void _YSMap::TranslateStrechToReal(SHORT x, SHORT y, SHORT* tx, SHORT* ty)
{
	*tx = (YSMAP_XSIZE/2) - (y/2) + (x/2);
	*ty = ((x/2) + (y/2)) / 2;		
}

void _YSMap::Draw(int mode)
{	
	SHORT	sx, sy;
	SHORT	mx, my;
	SHORT	YOffSet;
	SHORT	i;
	SHORT darkstep;

	YOffSet = m_siSmapStartY - ((clGrp.GetScreenYsize() - 1) - PannelSpr.Header.Ysize);

	if(mode==TRUE)
	{
	    for(i=0; i < PutPositionDotNum; i++)
		{
		   sx = PutPosition[i].sx;
		   sy = PutPosition[i].sy;

		   mx = PutPosition[i].mx;
		   my = PutPosition[i].my;	

		   darkstep = GD.BlackMap[UserPlayerNumber][mx][my];
		   darkstep = darkstep * (MAX_DARK_STEP - 1) / FOG2_BLACK_STEP ;

	       
		  /* clGrp.Screen8[(i / 40) * clGrp.GetSurfaceWidth() + (i % 40)] = 
						255;*/
		   clGrp.Screen8[(sy + YOffSet) * clGrp.GetSurfaceWidth() + (sx + m_siSmapStartX)] = 
						DarkTable[darkstep][GD.SmallMapBase[mx][my]];										
		}	
	}
	else
	{
	    for(i=0; i < PutPositionDotNum; i++)
		{
		   sx = PutPosition[i].sx;
		   sy = PutPosition[i].sy;		   

	       clGrp.Screen8[(sy + YOffSet) * clGrp.GetSurfaceWidth() + (sx + m_siSmapStartX)] = 
		         COLOR_BLACK;
		}	

	}
}


BOOL _YSMap::IsPtInMap(SHORT mousex, SHORT mousey, SHORT& mapx, SHORT& mapy)
{ 
			
	if((mousex >= m_siSmapStartX) && (mousex <= (YSMAP_XSIZE + m_siSmapStartX)) &&
		(mousey > m_siSmapStartY) && (mousey < (YSMAP_YSIZE + m_siSmapStartY)))
	{		
		mapx = ScreenToMap[(mousey -  m_siSmapStartY) * YSMAP_XSIZE + (mousex - m_siSmapStartX)].x;
		mapy = ScreenToMap[(mousey -  m_siSmapStartY) * YSMAP_XSIZE + (mousex - m_siSmapStartX)].y;	 		

		if(mapx == 0xff || mapy == 0xff)
			return FALSE;
		else			
		{
			if(mapx >= m_MapXsize  || mapy >= m_MapYsize)
			{
				//clGrp.Error("", "%d", (mousey -  m_siSmapStartY) * YSMAP_XSIZE + (mousex - m_siSmapStartX));
				//clGrp.Error("", "%d %d", mapx, mapy);
			}

			return TRUE;		
		}
		
	}
	
	return FALSE;
}

void _YSMap::DrawChar(SHORT x, SHORT y, BYTE color)
{
	//��� ������ ��ġ.
	SHORT	startx=m_siSmapStartX;
	SHORT	starty=m_siSmapStartY - ((clGrp.GetScreenYsize() - 1) - PannelSpr.Header.Ysize);
	SHORT	sx, sy;
	
	startx	 += sx = MapToScreen[StrechYOffSet[y] + x].x;
	starty	 += sy = MapToScreen[StrechYOffSet[y] + x].y;

	clGrp.PutPixel(startx, starty, color);
	
	if(IsEnablePutDot[sx+1][sy]) clGrp.PutPixel(startx+1, starty, color);
	if(IsEnablePutDot[sx][sy+1]) clGrp.PutPixel(startx, starty+1, color);
	if(IsEnablePutDot[sx+1][sy+1]) clGrp.PutPixel(startx+1, starty+1, color);
}

void _YSMap::DrawChar()
{
	SHORT i;
	SHORT id;

	for(i=0;i<GD.CharOrderIndex;i++)
	{
		id=GD.CharOrder[i];

	   if(IsSee(id) && CR[id].m_cltCharDrawInfo.GetCharSmallMapDrawSwitch() == TRUE)
	   {
		   DrawChar(clifChar.GetX(id), clifChar.GetY(id), clifChar.GetTeamColor(id));
	   }
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSMap::SetEventMark(SHORT x, SHORT y, SHORT ColorKindof)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSMap::SetEventMark(INT x, INT y, SHORT ColorKindof)
{
	//���� ��� ������ �̺�Ʈ ��ũ�� ���ٸ�
	if(m_EventManagement.nLoop == 0)	
	{
		m_EventManagement.ColorKindof			=	ColorKindof;		
		m_EventManagement.nLoop					=	SMAP_EVENTMARK_MAX_LOOP;		
		m_EventManagement.PrevCountFrame		=	0;
	
		/*
		m_EventManagement.sx = m_siSmapStartX + MapToScreen[y * YSMAP_MAP_XSIZE + x].x - (GameSpr[FILE_EVENTMARK].Header.Xsize/2);	
		m_EventManagement.sy = m_siSmapStartY + MapToScreen[y * YSMAP_MAP_XSIZE + x].y - (GameSpr[FILE_EVENTMARK].Header.Ysize/2);	
		*/
		// hojae_repair
		m_EventManagement.sx = m_siSmapStartX + MapToScreen[y * YSMAP_MAP_XSIZE + x].x - (GetFileXsize(FILE_EVENTMARK)/2);	
		m_EventManagement.sy = m_siSmapStartY + MapToScreen[y * YSMAP_MAP_XSIZE + x].y - (GetFileYsize(FILE_EVENTMARK)/2);	
	}
}

void _YSMap::DrawBox(SHORT x, SHORT y)
{	
	int	cx, cy, cex, cey;	

	clGrp.GetClipArea(cx, cy, cex, cey);
	clGrp.SetClipFullScreen();

	//�ڽ��� �׸���.
	clGrp.BoxC(m_siSmapStartX + MapToScreen[y * YSMAP_MAP_XSIZE + x].x - (m_BoxWidth/2), 
		 m_siSmapStartY + MapToScreen[y * YSMAP_MAP_XSIZE + x].y - (m_BoxHeight/2), 
		 m_siSmapStartX + MapToScreen[y * YSMAP_MAP_XSIZE + x].x + (m_BoxWidth/2), 
		 m_siSmapStartY + MapToScreen[y * YSMAP_MAP_XSIZE + x].y + (m_BoxHeight/2), 
		 255);	

	//�̺�Ʈ ��ũ�� �׸���.
	if(m_EventManagement.nLoop)
	{
		Draw_PutSpriteT(m_EventManagement.sx, m_EventManagement.sy, FILE_EVENTMARK,
						EventMarkFontArray[m_EventManagement.ColorKindof][m_EventManagement.Frame]);	// hojae_append
		
		if((GD.Frame - m_EventManagement.PrevCountFrame) > SMAP_EVENTMARK_MAX_DELAY_COUNT_FRAME	)
		{
		   m_EventManagement.Frame++;
		   m_EventManagement.PrevCountFrame = GD.Frame;

		}

		if(m_EventManagement.Frame == SMAP_EVENTMARK_MAX_FRAME)
		{
			m_EventManagement.Frame = 0;
			m_EventManagement.nLoop--;		
		}
	}

	clGrp.SetClipArea(cx, cy, cex,cey);
}

void _YSMap::CountTileColor()
{	
	int		i;
		
	for(i=0; i < FileTileBlackTile; i++)	
	{		
		GetMaxColor(i);	
	}	
}

void _YSMap::GetMaxColor(SHORT TileIndex)
{
	SHORT					TileCount;
	SHORT					UsedColor[256];
	SHORT 					i, j, index;			
	_Imjin2TilePattern		pattern;	
	LPBYTE					lpData;
	SHORT					TotalTileNum;
	SHORT					MaxValue;
	SHORT					MaxValueIndex;	

	TotalTileNum = (TileSpr[TileIndex].Header.HRes / TileSpr[TileIndex].Header.Xsize) * 
					(TileSpr[TileIndex].Header.VRes / TileSpr[TileIndex].Header.Ysize);

	for(TileCount=0; TileCount < TotalTileNum; TileCount++)
	{
		ZeroMemory(UsedColor, sizeof(UsedColor));

		if(TileSpr[TileIndex].Image)
		{
			lpData = &TileSpr[TileIndex].Image[TileSpr[TileIndex].Header.Start[TileCount]];
	
			for(i=0; i < TileSpr[TileIndex].Header.Ysize; i++)
			{
				j=0;		

				pattern.ColorKeyNum = *lpData;	
				lpData++;

				//�÷�Ű ��ŭ �̵��Ѵ�.
				j=pattern.ColorKeyNum;

				//��ȿ�� ����Ÿ�� ������ ����.
				pattern.DataNum = *lpData;		
				lpData++;
			
				for(index=0; index < pattern.DataNum; index++)			
					UsedColor[lpData[index]]++;							

				lpData+=pattern.DataNum;						
			}		

			//���� ū ���� ã�´�.
			MaxValue = UsedColor[0];
			MaxValueIndex = 0;
			for(i=1; i < 256; i++)
			{
				if(MaxValue < UsedColor[i])
				{
					if(i != TRANSCOLOR)
					{
						MaxValue = UsedColor[i];
						MaxValueIndex = i;
					}
				}
			}
			
			TileColor[TileIndex][TileCount] = DarkTable[1][MaxValueIndex];			
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void	_YSMap::AddPutPosition(SHORT sx, SHORT sy, SHORT mx, SHORT my)
/// ���� : PutPosition �迭�� ���� ���� ��ġ�� �����ش�.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void	_YSMap::AddPutPosition(SHORT sx, SHORT sy, SHORT mx, SHORT my)
{
	PutPosition[PutPositionDotNum].sx = BYTE(sx);
	PutPosition[PutPositionDotNum].sy = BYTE(sy);
	PutPosition[PutPositionDotNum].mx = BYTE(mx);
	PutPosition[PutPositionDotNum].my = BYTE(my);
	PutPositionDotNum++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void	_YSMap::DecideSmapStartXY()
/// ���� : ��� ���� m_siSmapStartX�� m_siSmapStartY�� ���� �� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
void	_YSMap::DecideSmapStartXY()
{
	m_siSmapStartX	=	15;
	m_siSmapStartY	=	516;
}

