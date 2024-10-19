#ifndef	IMJIN2_YLISTBUTTON_HEADER
#define IMJIN2_YLISTBUTTON_HEADER

#include	<button.h>

struct	strLinkButtonInfo
{
	SHORT	x;
	SHORT	y;
	CHAR	szButtonName[128];
};

struct	strLinkButtonInfoEx
{
	SHORT	x;
	SHORT	y;
	CHAR	szButtonName[128];
	SHORT	Code;
};

class _LinkedPressButton
{
protected:
	SHORT*					m_ListButtonPosData;

protected:	
	static	BOOL	m_bViewListGlobal;		
	BOOL			m_bViewListLocal;						//현재의 버튼을 누르고 있는가.
	SHORT			m_ButtonNum;
	SHORT			m_CurrentIndex;
	
	_Button*		m_lpButton;	
	
	virtual			SHORT			GetButtonInList();
	virtual			SHORT			GetButton();
	virtual			VOID			SetButton(SHORT Index);
public:
	_LinkedPressButton();
	virtual ~_LinkedPressButton();

	virtual		VOID		SetButtonAction(BOOL bAction, UI16 uiButtonID);
	virtual		VOID		SetAction(BOOL bAction);
	virtual		VOID		SetProcess(BOOL bProcess);
	virtual		BOOL		Create(SHORT m_ButtonNum, SHORT x, SHORT y, SHORT Width, SHORT Height, SHORT* lpListbuttonPosData, INT flag);
	virtual		BOOL		Create(SHORT m_ButtonNum, SHORT x, SHORT y, SHORT Width, SHORT Height, strLinkButtonInfo *pLinkButtonInfo, INT flag);

	virtual		BOOL		Create(SHORT x, SHORT y, SHORT Width, SHORT Height, strLinkButtonInfoEx *pLinkButtonInfo, INT flag);	// 버튼의 갯수없이 버튼을 생성한다.

	virtual		VOID		Free();
	virtual		BOOL		Process(BOOL bPush);
	virtual		BOOL		IsValidIndex(SHORT	Index);
	virtual		SHORT		NextButton();			
	virtual		BOOL		IsViewList(SHORT KindOf);
};

class _SelectNationButton : public _LinkedPressButton
{	
protected:	
	static		SHORT	m_NationArray[6];
	
public:
	SHORT		CountId;
	
	virtual	VOID				Create(SHORT YAxis, SHORT x, SHORT y, SHORT Width, SHORT Height);	
	virtual	VOID				Free();		
	virtual	VOID				SetButtonAction(BOOL bAction, SHORT Nation);
	virtual	BOOL				Process(BOOL bPush);
	virtual	VOID				SetNation(SHORT Nation);
	virtual	SHORT				GetNation();	
	virtual	VOID				Put(SHORT PlayerNumber, XSPR* lpXspr);
	virtual	VOID				Put(HDC hdc, SHORT PlayerNumber);
};

class _SelectAllyButton : public _LinkedPressButton
{
protected:
	static		SHORT	m_AllyArray[MAX_MULTI_PLAYER];			
public:

	virtual	VOID				Create(SHORT YAxis, SHORT x, SHORT y, SHORT Width, SHORT Height);
	virtual	VOID				Free();		
	virtual	BOOL				Process(BOOL bPush);
	virtual	VOID				SetAlly(SHORT Ally);
	virtual	SHORT				GetAlly();
	virtual	VOID				Put(SHORT PlayerNumber, XSPR* lpXspr);
	virtual	VOID				Put(HDC hdc, SHORT PlayerNumber);
};

class XSPR;

class _AllyButtonManagement
{
private:	
	XSPR					m_Xspr;		
	_SelectAllyButton		BAllyButton[MAX_MULTI_PLAYER];

	SHORT					m_ViewListIndex;
public:
	virtual	VOID					Create();
	virtual	VOID					Free();
	virtual	BOOL					ViewListProcess();
	virtual	VOID					SetAction(BOOL bAction);	
	virtual	VOID					SetAction(SHORT Index, SHORT bAction);
	virtual	VOID					Set(SHORT* lpAllys);
	virtual	VOID					Get(SHORT* lpAllys);
	virtual	VOID					Put(LPDIRECTDRAWSURFACE7 pSurface);
	virtual	VOID					PutViewList(LPDIRECTDRAWSURFACE7 pSurface);
	virtual	VOID					Process(BOOL bPush, SHORT* GetAllys);
};

class _NationButtonManagement
{
private:		
	XSPR					m_XsprButton;	
	_SelectNationButton		BNationButton[MAX_MULTI_PLAYER];

	SHORT					m_ViewListIndex;
public:
	virtual	VOID					Create();
	virtual	VOID					Free();
	virtual	BOOL					ViewListProcess();	
	virtual	VOID					SetButtonAction(SHORT Index, BOOL bAction, SHORT Nation);
	virtual	VOID					SetButtonAction(BOOL bAction[MAX_MULTI_PLAYER], SHORT Nation);		
	virtual	VOID					SetAction(BOOL bAction);
	virtual	VOID					SetAction(SHORT Index, SHORT bAction);
	virtual	VOID					Set(SHORT* lpAllys);
	virtual	VOID					Get(SHORT* lpAllys);
	virtual	VOID					Put(LPDIRECTDRAWSURFACE7 pSurface);
	virtual	VOID					PutViewList(LPDIRECTDRAWSURFACE7 pSurface);
	virtual	VOID					Process(BOOL bPush, SHORT* GetNations);
};

#endif