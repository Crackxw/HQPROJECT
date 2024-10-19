#ifndef	IMJIN2_YSELECTCONTROL_HEADER
#define	IMJIN2_YSELECTCONTROL_HEADER

#include <button.h>

struct	strBaseSelectButtonInfo
{
	RECT		rcButton;
	CHAR		szName[128];
};

class	_BaseSelectButton
{
protected:		
	_SelectButton		*m_lpButtons;
	SHORT				m_nButtonNum;

public:	
	_BaseSelectButton();	
	virtual ~_BaseSelectButton();

	virtual	VOID	SetAction(BOOL bPush);
	virtual	VOID	Create(SHORT ButtonNum, strBaseSelectButtonInfo *pstBaseSelectButtonInfo);
	virtual	SHORT	Process(BOOL bPush);
	virtual	VOID	Draw(XSPR* lpXspr, SHORT FontNum[][3], WORD PutTextFlag = BUTTON_PUT_NOBORDER | BUTTON_PUT_NOMOVE);
	virtual	VOID	Draw(HDC hdc, SI32 siSelectButton);
	virtual	VOID	Free();	
};	


#endif
