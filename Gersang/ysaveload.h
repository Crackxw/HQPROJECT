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

	virtual		BOOL		IsCurrentMenuUsed();					//���� ������ �ε����� �޴��� ����ϴ��� �˾ƺ���.
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
	VOID		Init(SHORT nSaveOrLoad);				// ���� �޴����� �ҷ� ���� �޴������� ������.
	VOID		Put();									// ȭ�鿡 ����Ѵ�.
	VOID		Put(HDC hdc);							// ���̺� �� �̸��� ȭ�鿡 ����Ѵ�.
	VOID		Free();									// �Ҵ� �޾Ҵ� �޸𸮸� �����Ѵ�.
	SHORT		GetMenu();								// ���̺� �޴����� �ε� �޴����� �˾Ƴ���.
	SHORT		Action(BOOL bPush);
};

VOID		InitSaveMenu();								//���̺� �޴��� �ʱ�ȭ �Ѵ�.
VOID		InitLoadMenu(SHORT	ReturnMenu);			//�ε� �޴��� �ʱ�ȭ �Ѵ�.
SHORT		SaveLoadMenu(BOOL bPush, LPDIRECTDRAWSURFACE7 pSurface);					//���̺�� �ε��� �� �޴��̴�.
VOID		FreeSaveMenu();								//���̺꿡�� ���Ǿ��� ���� �ʱ�ȭ �Ѵ�.
VOID		FreeLoadMenu();								//�ε忡�� ���Ǿ��� ���� �ʱ�ȭ �Ѵ�.

#endif