/************************************************************************************************
	파일명 : clItemInfoWindow.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	___ITEM_INFO_WINDOW___
#define ___ITEM_INFO_WINDOW___


class _clGame;


#define MAXITEMINFOWINDOW					16


typedef struct
{
	SI16		m_Flag;
	SI16		m_Type;

	SI16		m_Frame;
	SI16		m_MaxFrame;
	SI16		m_XPos;
	SI16		m_YPos;

	UI32		m_Code;
	char		m_TextData[256];
} SItemInfoWindow;


// 숫자 출력 Class
class clItemInfoWindow
{
public:
	SItemInfoWindow		m_ItemInfoWindowData[MAXITEMINFOWINDOW];
	XSPR				m_ItemInfoWindowImage;
	_clGame*			m_lpGame;

public:
	clItemInfoWindow();																		// Constructor
	virtual ~clItemInfoWindow();															// Destroyer

	BOOL		Create(_clGame* lpGame);													// Create
	void		Release(void);																// Release

	void		ClearAllItemInfoWindow(void);															// Clear All Item Info Window
	BOOL		AddItemInfoWindow(SI32 MaxFrame, SI32 x, SI32 y, UI32 ItemCode, char* lpTextData);		// Add Item Info Window
	BOOL		AddETCInfoWindow(SI32 MaxFrame, SI32 x, SI32 y, UI32 ResourceCode, char* lpTextData);	// Add ETC Info Window

	BOOL		Prepare(void);																// Prepare
	BOOL		Render(LPDIRECTDRAWSURFACE7 lpSurface);										// Render
};	


#endif
