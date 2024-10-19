/* 
	합본을 만들자(임.버 + 천.버 = 버튼)
	표시 목록
	1. 임진록 : 임진록에만 있는 부분
	2. 천  년 : 천년의 신화에만 있는 부분
*/

#ifndef _BUTTON_HEADER
#define	_BUTTON_HEADER

#include "..\Gersang\buttonssprmanagement.h"


#define		BUTTON_PUT_DEFAULT			((DWORD)0)
#define		BUTTON_PUT_LEFT				((WORD)1)
#define		BUTTON_PUT_CENTER			((WORD)2)
#define		BUTTON_PLAYSOUND_ONMOUSE	((WORD)4)
#define		BUTTON_PUT_NOMOVE			((DWORD)1)
#define		BUTTON_PUT_NOBORDER			((DWORD)2)
#define		BUTTON_PUT_NODARK			((DWORD)4)
#define		BUTTON_PUT_FILLCOLOR		((DWORD)8)

#define		BUTTON_VIEW_LIST_GLOBAL		0		// 임진록에만
#define		BUTTON_VIEW_LIST_LOCAL		1		// 임진록에만

#define		BOX_PUT_LEFT				((WORD)1)
#define		BOX_PUT_CENTER				((WORD)2)
#define		BOX_PUT_ORIGINAL			((WORD)3)
#define		BOX_PUT_DEFAULT				((DWORD)0)
#define		BOX_PUT_NOBORDER			((DWORD)1)

#define		GLOBAL_BUTTON_NUM						8	
#define		BUTTON_GLOBAL_DECIDE					0
#define		BUTTON_GLOBAL_BACK						1
#define		BUTTON_GLOBAL_EXIT						2
#define		BUTTON_GLOBAL_SEARCH					3
#define		BUTTON_GLOBAL_CREATE					4
#define		BUTTON_GLOBAL_JOIN						5
#define		BUTTON_GLOBAL_REPLAY					6
#define		BUTTON_GLOBAL_GOPLAY					7

//버튼에 대한 크기 정의
#define		BUTTON_WIDTH						80
#define		BUTTON_HEIGHT						24

//결정하기 버튼에 대한 정의
#define		BUTTON_DECIDE_X						495
#define		BUTTON_DECIDE_Y						419

//돌아가기 버튼에 대한 정의
#define		BUTTON_BACK_X						65
#define		BUTTON_BACK_Y						BUTTON_DECIDE_Y

//종료하기 버튼에 대한 정의
#define		BUTTON_EXIT_X						BUTTON_BACK_X
#define		BUTTON_EXIT_Y						BUTTON_DECIDE_Y

//검색하기 버튼에 대한 정의
#define		BUTTON_SEARCH_X						208
#define		BUTTON_SEARCH_Y						BUTTON_DECIDE_Y

//방만들기 버튼에 대한 정의
#define		BUTTON_CREATE_X						BUTTON_DECIDE_X
#define		BUTTON_CREATE_Y						BUTTON_DECIDE_Y

//참여하기 버튼에 대한 정의
#define		BUTTON_JOIN_X						352
#define		BUTTON_JOIN_Y						BUTTON_DECIDE_Y

//다시보기 버튼에 대한 정의
#define		BUTTON_REPLAY_X						280
#define		BUTTON_REPLAY_Y						BUTTON_DECIDE_Y

//게임하기 버튼에 대한 정의
#define		BUTTON_GOPLAY_X						BUTTON_DECIDE_X						
#define		BUTTON_GOPLAY_Y						BUTTON_DECIDE_Y


//숫자 버튼들의 대한 화일 이름과 폰트 넘버.
#define		BUTTON_NUMBUTTONS_SPR_FILENAME		"yfnt\\lobbynumbutton.spr"
#define		BUTTON_FONTNUM_ONE					0
#define		BUTTON_FONTNUM_TWO					1
#define		BUTTON_FONTNUM_THREE				2
#define		BUTTON_FONTNUM_FOUR					3
#define		BUTTON_FONTNUM_FIVE					4
#define		BUTTON_FONTNUM_SIX					5
#define		BUTTON_FONTNUM_SEVEN				6
#define		BUTTON_FONTNUM_EIGHT				7

//나라 버튼들의 대한 화일 이름과 폰트 넘버.
#define		BUTTON_NATIONBUTTONS_SPR_FILENAME	"yfnt\\lobbynationbutton.spr"
#define		BUTTON_FONTNUM_KOREA				0
#define		BUTTON_FONTNUM_JAPAN				1
#define		BUTTON_FONTNUM_CHINA				2
#define		BUTTON_FONTNUM_TAIWAN				3

class XSPR;

// 기본버튼
class _Button
{
protected:			
	BOOL		m_bAction;											//이 버튼이 활동을 하는 지 나타낸다.	(시각적으로 어두워진다.)
	BOOL		m_bProcess;											//이 버튼을 처리해 주어야 하는지 나타낸다.	m_bAction과의 차이는 출력상의 차이이다.	(시각적으로 변하지 않는다.)
	CHAR		m_MenuName[128];

	SHORT		m_x;
	SHORT		m_y;
	SHORT		m_Width;
	SHORT		m_Height;
	BOOL		m_bSoundPush;	

	BOOL		MbPrevFrameInButton;								//이전 프레임에 마우스가 현재 버튼 위에 있었는지...

	WORD		MuiFlag;

	// robypark 2004/11/15 14:3
	// 버튼 윈도 속성으로 Visible을 추가
	BOOL		m_bVisibleWindow;

protected:
	virtual		VOID		PutText(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, SHORT Width, SHORT Height, LPDIRECTDRAWSURFACE7 lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);

	// 임진록 2 PLUS에서만 사용
	virtual		VOID		PutTextPlus(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutTextPlus(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange = BUTTON_PUT_CENTER);

public:
	_Button();
	_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR *lpMenuName, BOOL bAction = TRUE);
	_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR *lpMenuName, WORD flag, BOOL bAction = TRUE);

	virtual		BOOL		IsAction() { return m_bAction;}
	virtual		BOOL		IsProcess()	{ return m_bProcess;}
	virtual		VOID		Create(INT x, INT y, INT m_Width, INT m_Height, CHAR* MenuName, BOOL bAction = TRUE);	
	virtual		VOID		Create(INT x, INT y, INT m_Width, INT m_Height, CHAR* MenuName, WORD flag, BOOL bAction = TRUE);	

	// 버튼의 위치가 변할시에
	virtual		VOID		SetX(SHORT NewX);
	virtual		VOID		SetY(SHORT NewY);

	virtual		VOID		SetMenuName(CHAR* lpNewMenuName);
	virtual		VOID		SetAction(BOOL bAction);
	virtual		VOID		SetProcess(BOOL bProcess);

	virtual		VOID		SetSelect(BOOL bSelect) {MbPrevFrameInButton = bSelect;}
	virtual		BOOL		IsSelect(VOID) {return MbPrevFrameInButton;}

	// 버튼의 위치가 변하는 것때문에 항상 2개로 이루어져 있음(상 : 버튼 고정시, 하 : 버튼 움직일때)
	virtual		BOOL		IsInMouse(); 
	virtual		BOOL		IsInMouse(INT x, INT y);

	virtual		BOOL		IsPressing(BOOL bPush);
	virtual		BOOL		IsPressing(INT x, INT y, BOOL bPush);

	virtual		BOOL		Process(BOOL bPush);		
	virtual		BOOL		Process(INT x, INT y, BOOL bPush);

	virtual		VOID		Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT);

	virtual		VOID		Put(LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag = BUTTON_PUT_DEFAULT); // 글자의 색
	virtual		VOID		Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag = BUTTON_PUT_DEFAULT);	

	virtual		VOID		Put(XSPR* lpXspr, SHORT FontNumber);
	virtual		VOID		Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT FontNumber);
	
	virtual		VOID		Put1(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put1(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);
	virtual		VOID		Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);

	virtual		VOID		Put(XSPR* lpXspr, SI16 siDefaultFontNum,	SI16 siSelectedFontNum,		SI16 siOnMouseFontNum, 
											  BOOL bDefaultFontTrans,	BOOL bSelectedFontTrans,	BOOL bOnMouseTrans,
												DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	virtual		VOID		Put(SI16 siX, SI16 siY, XSPR* lpXspr, SI16 siDefaultFontNum,	SI16 siSelectedFontNum,		SI16 siOnMouseFontNum, 
																  BOOL bDefaultFontTrans,	BOOL bSelectedFontTrans,	BOOL bOnMouseTrans,
								DWORD PutFlag = BUTTON_PUT_DEFAULT);	
	
	// 버튼이 배경을 포함하고 있을때 : 임진록에만(맵 선택시 보여지는 확인, 취소 버튼에 사용)
	virtual		VOID		Put2(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, SHORT NoActionFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT); 

	virtual		VOID		Put1(HDC hdc, DWORD PutFlag = BUTTON_PUT_DEFAULT);
	virtual		VOID		Put(HDC hdc, DWORD PutFlag = BUTTON_PUT_DEFAULT, COLORREF OffTextColor = RGB(161, 161, 161), COLORREF OnTextColor = RGB(219, 213, 200));
	virtual		VOID		Put1(HDC hdc, SHORT x, SHORT y, DWORD PutFlag = BUTTON_PUT_DEFAULT);
	virtual		VOID		Put(HDC hdc, SHORT x, SHORT y, COLORREF	OffTextColor, COLORREF	OnTextColor, DWORD PutFlag = BUTTON_PUT_DEFAULT);

	virtual		VOID		Put(HDC hdc, SI16 siX, SI16 siY, SI16 siWitdh, SI16 siHeight, DWORD PutFlag = BUTTON_PUT_DEFAULT); // 임진록

	virtual		VOID		PutSpriteButton(SI16 siX, SI16 siY, XSPR *pSpr, SI16 siFont, BOOL bDark, BOOL bTrans);						// 스프라이트를 이용하여 버튼을 찍는다.
	
	// 버튼의 크기를 얻기 위한 함수
	virtual		SHORT		GetX() {return m_x;}
	virtual		SHORT		GetY() {return m_y;}
	virtual		SHORT		GetWidth() {return m_Width;}
	virtual		SHORT		GetHeight() {return m_Height;}	
	virtual		CHAR*		GetMenuName() {return m_MenuName;}
	virtual		RECT		GetSize();
	
	// robypark 2004/11/15 14:5
	// 이 버튼 윈도가 보이는지를 얻어온다.
	BOOL		IsVisibleWindow(void);

	// robypark 2004/11/15 14:5
	// 이 버튼 윈도가 보이는지를 설정한다.
	void		ShowWindow(BOOL bVisible);
}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 선택버튼
class	_SelectButton : public _Button
{
private:
	SHORT			m_TextPutX;
	SHORT			m_TextPutY;

	//마우스 더블 클릭에 관계된 변수들..
	static	BOOL	m_PrevMousePressedTime;		// 임진록
	static	BOOL	m_IsMouseLeftButtonPressed; // 임진록

protected:
	BOOL			m_bSelected;

	virtual	BOOL	IsDoubleClick();			// 임진록
public:	
	_SelectButton();
	_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, BOOL bAction = TRUE);	
	_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, WORD flag, BOOL bAction = TRUE);	
	virtual ~_SelectButton();

	virtual	VOID	Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, BOOL bAction = TRUE);	// 천년
	virtual	VOID	Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, BOOL bAction = TRUE);	// 천년
	virtual	VOID	Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, WORD flag,  BOOL bAction = TRUE); // 천년
	
	virtual	VOID	Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);

	/* 임진록과 천년 다른 부분(7 Line) */
	virtual	VOID	Put(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BUTTON_PUT_DEFAULT, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);	// 임진록
	virtual	VOID	Put(HDC hdc, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);  // 임진록

	virtual	VOID	Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT); // 천년
	virtual	VOID	Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag = BUTTON_PUT_DEFAULT);	// 천년
	virtual	VOID	Put(INT x, INT y, INT TextPutX, INT TextPutY, LPDIRECTDRAWSURFACE7 lpSurface,XSPR *pXspr = NULL, SHORT FontNum = 0, DWORD PutFlag = BUTTON_PUT_DEFAULT, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);	// 천년
	virtual	VOID	Put(INT x, INT y, INT TextPutX, INT TextPutY, HDC hdc, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);  // 천년

	// 임진록 2PLUS에서만 사용할...
	virtual	VOID	PutPlus(HDC hdc, DWORD PutTextFlag = BUTTON_PUT_DEFAULT);  // 임진록2 Plus에서만 사용

	/* 임진록과 천년 다른 부분 - 천년으로 통합 */
	virtual	BOOL	Process(BOOL bPush, BOOL *pbIsDoubleClick = NULL);		// 천년
	virtual	BOOL	Process(INT x, INT y, BOOL bPush, BOOL *pbIsDoubleClick = NULL);	// 천년

	virtual	VOID	SetSelected(BOOL bSelected);	
	virtual	BOOL	IsSelected();
	virtual	VOID	SetTextPutX(SHORT TextPutX);
	virtual	VOID	SetTextPutY(SHORT TextPutY);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 임진록
class _ServiceProviderButton : public _SelectButton
{	

public:	
	_ServiceProviderButton();
	_ServiceProviderButton(SHORT x, SHORT y, SHORT width, SHORT height, char* lpMenuName);	
	virtual ~_ServiceProviderButton();	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 임진록
class _EnumSessionButton : public _SelectButton
{	
public:
	//LPGUID	m_lpGuid;

	_EnumSessionButton();
	_EnumSessionButton(SHORT x, SHORT y, SHORT width, SHORT height, char* lpMenuName);
	virtual ~_EnumSessionButton();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이건 해더만 존재
class _MapFileInfo  
{	
public:
	CHAR		m_szFileName[1024];			// 천년
	SHORT		m_MaxPlayerNum;				// 천년 & 임진록
	DWORD		m_Unique;					// 천년
	SI16		m_siMapXsize;				// 천년
	SI16		m_siMapYsize;				// 천년
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class _SelectMapFileButton : public _SelectButton
{	
public:	
	_MapFileInfo				m_MapFileInfo;

	_SelectMapFileButton();  // 임진록
	_SelectMapFileButton(SHORT x, SHORT y, SHORT width, SHORT height, char* lpMenuName); // 임진록
	virtual ~_SelectMapFileButton();  // 임진록
	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class _Box
{
private:
	SHORT		m_x;
	SHORT		m_y;
	SHORT		m_Width;
	SHORT		m_Height;
	CHAR		m_Name[128];

protected:
	virtual		VOID		PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, WORD TextArrange = BUTTON_PUT_CENTER);
	virtual		VOID		PutText(INT x, INT y, HDC	hdc, CHAR *lpText, WORD TextArrange = BUTTON_PUT_CENTER);
public:
	_Box();
	_Box(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* lpName, WORD PutFlag);
	virtual		~_Box();

	virtual		VOID		Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag);
	virtual		VOID		Put(HDC	hdc, DWORD	PutFlag = BOX_PUT_DEFAULT, WORD PutTextFlag = BOX_PUT_DEFAULT);	    
	virtual		VOID		Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag = BOX_PUT_DEFAULT, WORD PutTextFlag = BOX_PUT_DEFAULT);	

	virtual		SHORT		GetX();
	virtual		SHORT		GetY();	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class _ImageButton	: public _Button
{
	BYTE*		m_lpSampleBuffer;
	BYTE		m_SelectColor;
	SHORT		m_SampleXsize;	
public:
	_ImageButton();
	virtual	~_ImageButton();	

	virtual		VOID	Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* MenuName, BYTE* lpSampleBuffer, BYTE Color, WORD siFlag);	
	virtual		VOID	Create(SHORT x, SHORT y, SHORT Width, SHORT Height, BYTE* lpSampleBuffer, BYTE Color, SHORT nSampleXsize, WORD siFlag);	
	virtual		BOOL	IsInMouse(); 
	virtual		BOOL	Process(BOOL bPush);
	virtual		VOID	Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag);
	virtual		VOID	Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag);
	virtual		VOID	Put(HDC hdc, DWORD PutFlag = BUTTON_PUT_DEFAULT);	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//버튼들을 초기화 한다.
VOID	OnceInitButton();
VOID	SetActionAllButton(BOOL bAction);

//버튼들의 외부변수.
extern	_Button	g_Button[GLOBAL_BUTTON_NUM];
extern	XSPR	g_Buttons_Spr[TOTAL_BUTTONS_KINDOF_NUM];				//버튼을 불러오기 위한 스프라이트 변수.
extern	XSPR	GOverButtonSpr;											// 천년

BOOL	LoadAllButtons();												//모든 버튼의 스프라이트를 로드한다.
VOID	FreeAllButtons();												//모든 버튼의 스프라이트를 해제 한다.
#endif

