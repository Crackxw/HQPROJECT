#ifndef IMJIN2_SELECTSTAGE_HEADER
#define IMJIN2_SELECTSTAGE_HEADER

#include "ysaveload.h"

class	_YSelectStage : public _SelectMenu
{
private:
	XSPR		m_TitleSpr;
	XSPR		m_BarSpr;

	BOOL		m_bAction;
	SHORT		m_SelectedNation;
private:
	virtual		VOID	Refresh(SHORT SelectedStage);	
	virtual		BOOL	IsClearedStage(SHORT nStageNum);
public:
	_YSelectStage();
	virtual	~_YSelectStage();

	virtual		BOOL	Create(SHORT SelectedNation);
	virtual		VOID	Free();
	virtual		VOID	Put(LPDIRECTDRAWSURFACE7	lpSurface);	
	virtual		BOOL	Action(BOOL bPush, SHORT* lpStage);
	
	virtual		BOOL	IsAction();
	virtual		VOID	SetAction(BOOL bAction);
	
};

#endif

