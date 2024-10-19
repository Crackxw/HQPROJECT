//	���� ����	:	2002-02-22 ���� 10:25:02 - ������

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
	BYTE			IsEnablePutDot[YSMAP_XSIZE + 1][YSMAP_YSIZE + 1];						//���� ������ ���� �޸�.	
	BYTEPOSITION	PutPosition[YSMAP_XSIZE * YSMAP_YSIZE];									//�ǳڻ��� ������ ���� �� ��ġ�� �����ϰ� �ִ�.																							//�̰ɷ� ��Ȯ�� ������ ���� ���� ��Ȯ�� ���� ��ŭ ���� ��´�.
																							//���� ���� ������ ���� ����ϱ� ���� �ʿ��ϴ�.

	//BYTEPOINT		StrechToScreen[YSMAP_MAP_XSIZE * YSMAP_MAP_YSIZE];
	//BYTEPOINT		StrechToMap[YSMAP_MAP_XSIZE * YSMAP_MAP_YSIZE];
	SHORT			StrechYOffSet[YSMAP_MAP_YSIZE];

	BYTEPOINT		ScreenToMap[YSMAP_XSIZE * YSMAP_YSIZE];
	BYTEPOINT		MapToScreen[YSMAP_MAP_XSIZE * YSMAP_MAP_YSIZE];
	
private:
	SHORT			m_MapXsize;								//���� �� X ������
	SHORT			m_MapYsize;								//���� �� Y ������	
	SIZE			m_StrechSize;							//���� ����� �°� g_MinYSMapSize ����� Ȯ���ϰų� ����� ũ��.	
	SHORT			m_YSMapXUnit;							//���� ������ ����ϹǷ� ��� ���� ���� �� ���� �׷��Ƿ� �پ� �پ� ��� X ����.
	SHORT			m_YSMapYUnit;							//���� ������ ����ϹǷ� ��� ���� ���� �� ���� �׷��Ƿ� �پ� �پ� ��� Y ����.
	SHORT			m_BoxWidth;
	SHORT			m_BoxHeight;

	EVENTMARK		m_EventManagement;
protected:	
	VOID			GetStrechSize();
	VOID			GetBoxSize();
	VOID			TranslateStrechToReal(SHORT x, SHORT y, SHORT* tx, SHORT* ty);
	VOID			DrawChar(SHORT x, SHORT y, BYTE color);
	VOID			GetMaxColor(SHORT TileIndex);	
	VOID			AddPutPosition(SHORT sx, SHORT sy, SHORT mx, SHORT my);						//PutPosition �迭�� ���� ���� ��ġ�� �߰����ش�.	
	VOID			DecideSmapStartXY();														// ��� ���� m_siSmapStartX�� m_siSmapStartY�� ���� �� �����Ѵ�.

public:	
	_YSMap();		// ������.

    BYTE			TileColor[MAX_MAP_TILE_SPR_NUMBER][300];

	VOID			Init(SHORT mapXsize, SHORT mapYsize);	
	VOID			Draw(int mode);	
	VOID			DrawChar();
	VOID			DrawBox(SHORT x, SHORT y);	
	VOID			CountTileColor();	
	VOID			SetEventMark(INT x, INT y, SHORT ColorKindOf = YSMAP_EVENTMARK_COLOR_RED);
	

	// ���콺�� ���� �������� �ִ��� ���θ� �Ǵ��Ѵ�. 
	BOOL			IsPtInMap(SHORT mousex, SHORT mousey, SHORT& mapx, SHORT& mapy);
};


#endif
