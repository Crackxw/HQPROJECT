#ifndef	IMJIN2_PROGRESSBAR_CONTROL
#define	IMJIN2_PROGRESSBAR_CONTROL

#include	"YControl.h"

#define		Y_OPTION_PROGRESSBAR_NUM				2
#define		Y_OPTION_PROGRESSBAR_SMALL				0
#define		Y_OPTION_PROGRESSBAR_LARGE				1

#define		Y_OPTION_PROGRESSBAR_SMALL_SPR_FILENAME		"yfnt\\ProgressBar_Small.spr"
#define		Y_PROGRESSBAR_SMALL_SELL_WIDTH				6
#define		Y_PROGRESSBAR_SMALL_SELL_HEIGHT				20

#define		Y_OPTION_PROGRESSBAR_LARGE_SPR_FILENAME		"yfnt\\ProgressBar_Large.spr"
#define		Y_PROGRESSBAR_LARGE_SELL_WIDTH				24
#define		Y_PROGRESSBAR_LARGE_SELL_HEIGHT				20

#define		Y_SOUNDVOLUME_PROGRESSBAR					Y_OPTION_PROGRESSBAR_SMALL
#define		Y_SOUNDVOLUME_PROGRESSBAR_X					284
#define		Y_SOUNDVOLUME_PROGRESSBAR_Y					89
#define		Y_SOUNDVOLUME_PROGRESSBAR_SELL_NUM			20

#define		Y_GAMMA_PROGRESSBAR							Y_OPTION_PROGRESSBAR_SMALL
#define		Y_GAMMA_PROGRESSBAR_X						Y_SOUNDVOLUME_PROGRESSBAR_X
#define		Y_GAMMA_PROGRESSBAR_Y						225
#define		Y_GAMMA_PROGRESSBAR_SELL_NUM				20


class	_YProgressBar : public _YControl
{
private:
	BOOL					MbAction;							//프로그레스바의 활동 여부.
protected:
	SHORT					m_Width;
	SHORT					m_Height;

	SHORT					m_ScrollSX;
	SHORT					m_ScrollEX;
	XSPR					m_XSpr;

protected:
	SHORT					m_CurrentProgress;					// 무조건 0 ~ 100

private:
	BOOL					IsIn(SHORT x, SHORT y);
	SHORT					GetSellIndex(SHORT x);

public:
	_YProgressBar();
	virtual	~_YProgressBar();	
	
	virtual		VOID		Init(SHORT x, SHORT y, SHORT Width, SHORT Height, BOOL bAction = TRUE);
	virtual		VOID		Draw();
	virtual		VOID		Action(BOOL bPush);
	virtual		VOID		RefreshData();

	virtual		VOID		Free();
	virtual		VOID		SetCurrentPos(SHORT Index);
	virtual		SHORT		GetCurrentPos();
};


class _YSoundVolumeProgressBar : public _YProgressBar
{
public:
	_YSoundVolumeProgressBar();
	virtual ~_YSoundVolumeProgressBar();

	virtual		VOID		Init(BOOL bAction, SI16 x, SI16 y, SI16 Width, SI16 Height);
	virtual		VOID		RefreshData();	
};


class _YGammaProgressBar	: public _YProgressBar
{
	SHORT					m_OldGamma;	
public:
	_YGammaProgressBar();
	virtual	~_YGammaProgressBar();

	virtual		VOID		Init(SI16 x, SI16 y, SI16 Width, SI16 Height);	
	virtual		VOID		RefreshData();
	virtual		VOID		RestoreData();
	virtual		VOID		Action(BOOL bPush);
};


#endif
