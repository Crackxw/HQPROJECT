#ifndef	IMJIN2_YSCROLLSPEED_HEADER
#define	IMJIN2_YSCROLLSPEED_HEADER

#include <button.h>
#include	"YSelectControl.h"

#define		Y_OPTION_SCROLLSPEED_SPR_FILENAME			"yfnt\\mouseinterface.spr"

#define		Y_SCROLLSPEED_SLOW_X						288
#define		Y_SCROLLSPEED_SLOW_Y						156
#define		Y_SCROLLSPEED_MEDIUM_X						342
#define		Y_SCROLLSPEED_MEDIUM_Y						Y_SCROLLSPEED_SLOW_Y
#define		Y_SCROLLSPEED_FAST_X						396
#define		Y_SCROLLSPEED_FAST_Y						Y_SCROLLSPEED_SLOW_Y
#define		Y_SCROLLSPEED_WIDTH							54
#define		Y_SCROLLSPEED_HEIGHT						24

#define		Y_SCROLLSPEED_SLOW_FONTNUM_SEG				6
#define		Y_SCROLLSPEED_MEDIUM_FONTNUM_SEG			9
#define		Y_SCROLLSPEED_FAST_FONTNUM_SEG				12

#define		GET_BUTTON_INDEX_SCROLLSPEED_SLOW(X)		(Y_SCROLLSPEED_SLOW_FONTNUM_SEG + (X))
#define		GET_BUTTON_INDEX_SCROLLSPEED_MEDIUM(X)		(Y_SCROLLSPEED_MEDIUM_FONTNUM_SEG + (X))
#define		GET_BUTTON_INDEX_SCROLLSPEED_FAST(X)		(Y_SCROLLSPEED_FAST_FONTNUM_SEG + (X))

class _YScrollSpeed
{
private:
	static		XSPR	m_Xspr;
	static		enum	{SLOW, MEDIUM, FAST};	

protected:
	_BaseSelectButton	m_BaseSelectButton;
	SHORT				m_nSpeed;	

	virtual BOOL		IsSlow();
	virtual	BOOL		IsMedium();
	virtual	BOOL		IsFast();
	virtual VOID		SetSlow();
	virtual	VOID		SetMedium();
	virtual	VOID		SetFast();
public:
	_YScrollSpeed();
	virtual	~_YScrollSpeed();

	virtual		VOID		Init();
	virtual		VOID		RefreshData();
	virtual		VOID		Action(BOOL bPush);
	virtual		VOID		Draw();
	virtual		VOID		Draw(HDC hdc);
	virtual		VOID		Free();
};


#endif

