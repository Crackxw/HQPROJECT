#ifndef	IMJIN2_MICONTROL_HEADER
#define IMJIN2_MICONTROL_HEADER

#include	"button.h"
#include	"YSelectControl.h"

#define		Y_OPTION_MOUSEINTERFACE_SPR_FILENAME		"yfnt\\mouseinterface.spr"

#define		Y_MOUSEINTERFACE_ONE_X						281
#define		Y_MOUSEINTERFACE_ONE_Y						269
#define		Y_MOUSEINTERFACE_TWO_X						335
#define		Y_MOUSEINTERFACE_TWO_Y						Y_MOUSEINTERFACE_ONE_Y
#define		Y_MOUSEINTERFACE_WIDTH						54
#define		Y_MOUSEINTERFACE_HEIGHT						24

#define		Y_MOUSEINTERFACE_ONE_FONTNUM_SEG			0
#define		Y_MOUSEINTERFACE_TWO_FONTNUM_SEG			3

#define		GET_BUTTON_INDEX_MOUSEINTERFACE_ONE(X)		(Y_MOUSEINTERFACE_ONE_FONTNUM_SEG + (X))
#define		GET_BUTTON_INDEX_MOUSEINTERFACE_TWO(X)		(Y_MOUSEINTERFACE_TWO_FONTNUM_SEG + (X))

class _YMouseInterface
{
private:
	static		enum	{ONE, TWO};	

protected:
	_BaseSelectButton	m_BaseSelectButton;
	SHORT				m_nOneTwo;	
	XSPR				m_XSPR;

	virtual BOOL			IsOne();
	virtual	BOOL			IsTwo();
	virtual	VOID			SetOne();
	virtual	VOID			SetTwo();
public:
	_YMouseInterface();
	virtual	~_YMouseInterface();

	virtual		VOID		Init(SI16 x, SI16 y);
	virtual		VOID		RefreshData();
	virtual		VOID		Action(BOOL bPush);
	virtual		VOID		Draw();
	virtual		VOID		Draw(HDC hdc);
	virtual		VOID		Free();
};

#endif
