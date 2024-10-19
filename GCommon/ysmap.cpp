//--------------------------------------------------------------------
//  담당 : 김태곤
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

#define		SMAP_EVENTMARK_MAX_LOOP					1			// 이벤트 마크를 몇 번 루프해서 찍을 것인가.
#define		SMAP_EVENTMARK_MAX_DELAY_COUNT_FRAME	2	


extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	XSPR 				PannelSpr;
extern	_MainData			GD;									// 게임의 주데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	XSPR 				TileSpr[];							// 지도 폰트용 타일 
extern _Char   CR[];       // 캐릭터의 포인터 
extern	unsigned char		DarkTable[][256];					// 어두운 단계 


SHORT	_YSMap::EventMarkFontArray[YSMAP_EVENTMARK_COLOR_MAX][SMAP_EVENTMARK_MAX_FRAME]=
{
	{0, 1, 2, 3, 4, 5, 6, 7, 8},
	{10, 11, 12, 13, 14, 15, 16, 17, 18}
};

_YSMap	YSMap;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-02-22 오전 10:24:54 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	//lpYSMap을 초기화 한다.
	memset(IsEnablePutDot, FALSE, sizeof(IsEnablePutDot));
	memset(ScreenToMap, 0xff, sizeof(ScreenToMap));
	memset(MapToScreen, 0xff, sizeof(MapToScreen));
	//memset(StrechToScreen, 0xff, sizeof(StrechToScreen));
	//memset(StrechToMap, 0xff, sizeof(StrechToMap));	
	memset(PutPosition, 0xff, sizeof(PutPosition));
	

	PutPositionDotNum = 0;
			
	m_MapXsize		= MapXsize;								//실제 맵 X 사이즈
	m_MapYsize		= MapYsize;								//실제 맵 Y 사이즈					

	//위에서 구한 g_MinYSMapSize를 실제 화면에 지도 사이즈에 넣기 위해
	//축소하거나 확대한 새로운 크기를 얻어 온다.
	GetStrechSize();	

	//사각형 박스의 크기를 구한다.
	GetBoxSize();

	//스트레치 한 맵 만큼의 메모리를 할당 받는다.	
	lpStrechYSMapPos = new POINT[m_StrechSize.cx * m_StrechSize.cy];	
	
	for(y=0; y < m_StrechSize.cy; y++)
	{
		for(x=0; x < m_StrechSize.cx; x++)
		{			
			mx = m_MapXsize * x / SHORT(m_StrechSize.cx);
			my = m_MapYsize * y / SHORT(m_StrechSize.cy);			

			//지도를 늘렸으므로 늘린 위치를 정확히 알기 위해 실제 맵의 인덱스를 저장해 둔다.
			lpStrechYSMapPos[y * m_StrechSize.cx + x].x = mx;
			lpStrechYSMapPos[y * m_StrechSize.cx + x].y = my;						

			//tx, ty는 실제 스크린상의 위치이다.
			TranslateStrechToReal(x, y, &tx, &ty);									

			//맵의 인덱스로 화면의 어느 위치인지를 알 수 있다.
			MapToScreen[my * YSMAP_MAP_XSIZE + mx].x = BYTE(tx);
			MapToScreen[my * YSMAP_MAP_XSIZE + mx].y = BYTE(ty);

			//화면의 위치로 맵의 인덱스를 알 수 있다.
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
	
	
	//타일의 개수를 센다.	
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

	//폭과 넓이중 어떤것이 큰지 본다.
	if(m_MapXsize < m_MapYsize)			//높이가 더 크다.
	{
		m_StrechSize.cy = siMaxSizeOfXY;
		m_StrechSize.cx = (m_MapXsize * m_StrechSize.cy) / m_MapYsize;
	}
	else if(m_MapXsize > m_MapYsize)		//폭이 더 크다.
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
/// 함수 : void _YSMap::TranslateStrechToReal(SHORT x, SHORT y, SHORT* tx, SHORT* ty)
/// 목적 : Strech의 x, y를 실제 모니터 상의 좌표로 변환한다.
/// 중요도 : 매우 중요한 함수. 실제 화면상의 모든 점의 위치를 결정한다.
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
	//찍기 시작할 위치.
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
/// 함수 : VOID	_YSMap::SetEventMark(SHORT x, SHORT y, SHORT ColorKindof)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSMap::SetEventMark(INT x, INT y, SHORT ColorKindof)
{
	//현재 어느 곳에도 이벤트 마크가 없다면
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

	//박스를 그린다.
	clGrp.BoxC(m_siSmapStartX + MapToScreen[y * YSMAP_MAP_XSIZE + x].x - (m_BoxWidth/2), 
		 m_siSmapStartY + MapToScreen[y * YSMAP_MAP_XSIZE + x].y - (m_BoxHeight/2), 
		 m_siSmapStartX + MapToScreen[y * YSMAP_MAP_XSIZE + x].x + (m_BoxWidth/2), 
		 m_siSmapStartY + MapToScreen[y * YSMAP_MAP_XSIZE + x].y + (m_BoxHeight/2), 
		 255);	

	//이벤트 마크를 그린다.
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

				//컬러키 만큼 이동한다.
				j=pattern.ColorKeyNum;

				//유효한 데이타의 개수를 센다.
				pattern.DataNum = *lpData;		
				lpData++;
			
				for(index=0; index < pattern.DataNum; index++)			
					UsedColor[lpData[index]]++;							

				lpData+=pattern.DataNum;						
			}		

			//제일 큰 값을 찾는다.
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
/// 함수 : void	_YSMap::AddPutPosition(SHORT sx, SHORT sy, SHORT mx, SHORT my)
/// 목적 : PutPosition 배열에 찍을 점의 위치를 더해준다.
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
/// 함수 : void	_YSMap::DecideSmapStartXY()
/// 목적 : 멤버 변수 m_siSmapStartX와 m_siSmapStartY의 값을 재 설정한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
void	_YSMap::DecideSmapStartXY()
{
	m_siSmapStartX	=	15;
	m_siSmapStartY	=	516;
}

