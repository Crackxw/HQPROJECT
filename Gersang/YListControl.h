#ifndef	IMJIN2_LISTCONTROL_HEADER
#define IMJIN2_LISTCONTROL_HEADER

#include <button.h>


//기본적인 리스트 콘트롤
#define	Y_LISTCONTROL_MAX_SELLNUM		100
#define Y_LISTCONTROL_DEFAULT			((DWORD)0)
#define	Y_LISTCONTROL_SELECT			((DWORD)1)

//플레이어를 선택하는 콘트롤
#define	Y_CHOOSENAME_X					208
#define	Y_CHOOSENAME_Y					155

#define	Y_CHOOSENAME_SELL_WIDTH			120
#define	Y_CHOOSENAME_SELL_HEIGHT		20

//나라 선택 리스트 콘트롤
#define	Y_CHOOSESTAGE_MAX_SELLNUM		8



class _YListControl	: public _YControl
{
private:
	SHORT				m_SellWidth;
	SHORT				m_SellHeight;
protected:	
	SHORT				m_CurrentTotalSellNum;
	SHORT				m_nSelectedSell;
	
	 BOOL				m_IsUseSell[Y_LISTCONTROL_MAX_SELLNUM];
	_SelectButton		m_BSell[Y_LISTCONTROL_MAX_SELLNUM];


	SHORT				m_ButtonPoint[Y_LISTCONTROL_MAX_SELLNUM];	
	
private:	
	VOID				SetSelect(SHORT Index);

protected:
	virtual	SHORT		GetEmptySell();
public:
	_YListControl();
	virtual	~_YListControl();

	virtual	VOID		Create(SHORT x, SHORT y, SHORT SellWidth, SHORT SellHeight);
	virtual	VOID		Free();	
	virtual	VOID		Action(BOOL bPush, BOOL *lpIsDoubleClicked);
	virtual	SHORT		AddSell(SHORT x, SHORT y, CHAR* Name, DWORD Flag);
	virtual	SHORT		GetSell(CHAR* szName);	
	virtual	SHORT		DelSell();
	virtual	SHORT		RenameSell(CHAR* szName);
	virtual	SHORT		GetSelectedSell();
	virtual	VOID		SetSelectedSell(SHORT NewSell);
	virtual	VOID		Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	virtual	SHORT		GetCurrentTotalSellNum();
	virtual	BOOL		IsSelectedSomething();
};


#endif
