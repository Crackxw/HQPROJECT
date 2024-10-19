#ifndef		IMJIN2_YONOFFCONTROL_HEADER
#define		IMJIN2_YONOFFCONTROL_HEADER


#include	<button.h>
#include	"YSelectControl.h"


#define		Y_OPTION_ONOFF_SPR_FILENAME			"yfnt\\optiononoffs.spr"
#define		Y_CDMUSIC_ONOFF_ON_X				282
#define		Y_CDMUSIC_ONOFF_ON_Y				133
#define		Y_CDMUSIC_ONOFF_OFF_X				324
#define		Y_CDMUSIC_ONOFF_OFF_Y				Y_CDMUSIC_ONOFF_ON_Y
#define		Y_CDMUSIC_ONOFF_WIDTH				42
#define		Y_CDMUSIC_ONOFF_HEIGHT				24

#define		Y_ONOFF_ON_FONTNUM_SEG			0
#define		Y_ONOFF_OFF_FONTNUM_SEG			3

#define		GET_BUTTON_INDEX_ONOFF_ON(X)		(Y_ONOFF_ON_FONTNUM_SEG + (X))
#define		GET_BUTTON_INDEX_ONOFF_OFF(X)		(Y_ONOFF_OFF_FONTNUM_SEG + (X))


//ON, OFF 버튼의 베이스 클래스.
class _OnOffButton 
{
private:
	static		enum	{ON, OFF};

protected:	
	_BaseSelectButton	m_BaseSelectButton;
	SHORT				m_nOnOff;	
	XSPR				m_XSPR;
	
	virtual BOOL		IsOn();
	virtual	BOOL		IsOff();
public:
	_OnOffButton();
	virtual ~_OnOffButton();			

	virtual	VOID		Init(SHORT OnX, SHORT OnY, SHORT OffX, SHORT OffY);
	virtual	VOID		SetOn();
	virtual VOID		SetOff();		
	virtual	VOID		Draw(SHORT FontArray[][3]);
	virtual	VOID		Draw(HDC hdc);
	virtual	VOID		Action(BOOL bPush);
	virtual	VOID		Free();
	virtual	VOID		SetAction(BOOL bAction);
};


// CD 음악 ON, OFF 클래스.
class _YCDOnOff : public _OnOffButton
{
public:	
	_YCDOnOff();	
	~_YCDOnOff();	
	
	virtual		VOID		Init(BOOL bAction, SI16 x, SI16 y);
	virtual		VOID		RefreshData();	
	virtual		VOID		Draw();	
	virtual		VOID		Draw(HDC hdc);
};


#endif
