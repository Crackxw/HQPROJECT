#ifndef	IMJIN2_SAVELOAD_HEADER
#define IMJIN2_SAVELOAD_HEADER

#define	Y_GAMEMENU_SAVELOAD_MAX_SLOT		7

#include "gamemenu.h"

class _SelectMenu
{
protected:
	CHAR		m_MenuName[1024];

	BOOL		m_bCreated;

	RECT		m_Rect[Y_GAME_MENU_MAX_SLOT];

	BOOL		m_IsUsedSlot[Y_GAME_MENU_MAX_SLOT];
	CHAR		m_szSlotName[Y_GAME_MENU_MAX_SLOT][128];

	SHORT		m_x;
	SHORT		m_y;

	SHORT		m_MaxSlot;
	SHORT		m_CurrentIndex;		

	SHORT		m_MenuWidth;
	SHORT		m_MenuHeight;
	SHORT		m_SlotWidth;
	SHORT		m_SlotHeight;

	SHORT		m_GapY;
	SHORT		m_GapX;
protected:
	_Button		m_BOk, m_BCancel;	
public:
	virtual		BOOL		Create(SHORT SlotNum, SHORT x, SHORT y, SHORT MenuWidth, SHORT MenuHeight, 
									SHORT SlotWidth, SHORT SlotHeight, LPSTR MenuName, BOOL bCenter = TRUE);

	virtual		BOOL		IsCurrentMenuUsed();					//현재 선택한 인덱스의 메뉴를 사용하는지 알아본다.
	virtual		VOID		Put();	
	virtual		VOID		Init();
	virtual		VOID		ProcessSelectIndex(BOOL bPush);	
	virtual		SHORT		Action(BOOL bPush);
	
};

class _SaveLoadMenu : public _SelectMenu
{
private:
	XSPR		m_TitleSpr;
	XSPR		m_BarSpr;
	SHORT		m_nSaveOrLoad;			
private:	
public:	
	BOOL		Create();
	VOID		Refresh();
	VOID		Init(SHORT nSaveOrLoad);				// 저장 메뉴인지 불러 오기 메뉴인지를 나눈다.
	VOID		Put();									// 화면에 출력한다.
	VOID		Put(HDC hdc);							// 세이브 한 이름을 화면에 출력한다.
	VOID		Free();									// 할당 받았던 메모리를 해제한다.
	SHORT		GetMenu();								// 세이브 메뉴인지 로드 메뉴인지 알아낸다.
	SHORT		Action(BOOL bPush);
};

VOID		InitSaveMenu();								//세이브 메뉴를 초기화 한다.
VOID		InitLoadMenu(SHORT	ReturnMenu);			//로드 메뉴를 초기화 한다.
SHORT		SaveLoadMenu(BOOL bPush, LPDIRECTDRAWSURFACE7 pSurface);					//세이브와 로드의 주 메뉴이다.
VOID		FreeSaveMenu();								//세이브에서 사용되었던 것을 초기화 한다.
VOID		FreeLoadMenu();								//로드에서 사용되었던 것을 초기화 한다.

#endif