//	수정 일자	:	2002-02-22 오전 10:25:02 - 양정모

#ifndef	IMJIN2_YSMAP_HEADER
#define	IMJIN2_YSMAP_HEADER

#include <myfile.h>


#define		YSMAP_MAP_XSIZE   MAX_MAP_XSIZE
#define		YSMAP_MAP_YSIZE   MAX_MAP_YSIZE
#define		YSMAP_XSIZE		   165
#define		YSMAP_YSIZE		   79


#define		YSMAP_EVENTMARK_COLOR_MAX					2
#define		YSMAP_EVENTMARK_COLOR_RED					0
#define		YSMAP_EVENTMARK_COLOR_GREEN					1

#define		SMAP_EVENTMARK_MAX_FRAME					9

struct	BYTEPOINT
{
	BYTE			x;
	BYTE			y;
};

struct	BYTEPOSITION
{
	BYTE			sx;
	BYTE			sy;
	BYTE			mx;
	BYTE			my;
};	
	
struct	EVENTMARK
{	
	SHORT			ColorKindof;
	SHORT			sx;
	SHORT			sy;	
	BYTE			Frame;
	BYTE			nLoop;
	DWORD			PrevCountFrame;
};

class	_YSMap
{
private:
	static			SHORT	EventMarkFontArray[YSMAP_EVENTMARK_COLOR_MAX][SMAP_EVENTMARK_MAX_FRAME];
	SI32			m_siSmapStartX, m_siSmapStartY;
private:
	SHORT			PutPositionDotNum;
	BYTE			IsEnablePutDot[YSMAP_XSIZE + 1][YSMAP_YSIZE + 1];						//작은 지도를 위한 메모리.	
	BYTEPOSITION	PutPosition[YSMAP_XSIZE * YSMAP_YSIZE];									//판넬상의 지도를 찍어야 할 위치를 저장하고 있다.																							//이걸로 정확히 마름모 지도 점의 정확한 개수 만큼 점을 찍는다.
																							//단지 작은 지도를 빨리 출력하기 위해 필요하다.

	//BYTEPOINT		StrechToScreen[YSMAP_MAP_XSIZE * YSMAP_MAP_YSIZE];
	//BYTEPOINT		StrechToMap[YSMAP_MAP_XSIZE * YSMAP_MAP_YSIZE];
	SHORT			StrechYOffSet[YSMAP_MAP_YSIZE];

	BYTEPOINT		ScreenToMap[YSMAP_XSIZE * YSMAP_YSIZE];
	BYTEPOINT		MapToScreen[YSMAP_MAP_XSIZE * YSMAP_MAP_YSIZE];
	
private:
	SHORT			m_MapXsize;								//실제 맵 X 사이즈
	SHORT			m_MapYsize;								//실제 맵 Y 사이즈	
	SIZE			m_StrechSize;							//지도 사이즈에 맞게 g_MinYSMapSize 사이즈를 확대하거나 축소한 크기.	
	SHORT			m_YSMapXUnit;							//실제 지도를 축소하므로 모든 점을 찍을 수 없다 그러므로 뛰엄 뛰엄 찍는 X 단위.
	SHORT			m_YSMapYUnit;							//실제 지도를 축소하므로 모든 점을 찍을 수 없다 그러므로 뛰엄 뛰엄 찍는 Y 단위.
	SHORT			m_BoxWidth;
	SHORT			m_BoxHeight;

	EVENTMARK		m_EventManagement;
protected:	
	VOID			GetStrechSize();
	VOID			GetBoxSize();
	VOID			TranslateStrechToReal(SHORT x, SHORT y, SHORT* tx, SHORT* ty);
	VOID			DrawChar(SHORT x, SHORT y, BYTE color);
	VOID			GetMaxColor(SHORT TileIndex);	
	VOID			AddPutPosition(SHORT sx, SHORT sy, SHORT mx, SHORT my);						//PutPosition 배열에 찍을 점의 위치를 추가해준다.	
	VOID			DecideSmapStartXY();														// 멤버 변수 m_siSmapStartX와 m_siSmapStartY의 값을 재 설정한다.

public:	
	_YSMap();		// 생성자.

    BYTE			TileColor[MAX_MAP_TILE_SPR_NUMBER][300];

	VOID			Init(SHORT mapXsize, SHORT mapYsize);	
	VOID			Draw(int mode);	
	VOID			DrawChar();
	VOID			DrawBox(SHORT x, SHORT y);	
	VOID			CountTileColor();	
	VOID			SetEventMark(INT x, INT y, SHORT ColorKindOf = YSMAP_EVENTMARK_COLOR_RED);
	

	// 마우스가 작은 지도위에 있는지 여부를 판단한다. 
	BOOL			IsPtInMap(SHORT mousex, SHORT mousey, SHORT& mapx, SHORT& mapy);
};


#endif
