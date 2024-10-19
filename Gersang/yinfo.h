#ifndef	IMJIN2_INFO_HEADER
#define	IMJIN2_INFO_HEADER
/*
#include "..\Gersang\YOkCancel.h"

#define		Y_INFO_BORDER_FILENAME		"yfnt\\infoborder.spr"
#define		Y_INFO_GET_BUTTON_INDEX_TYPE(X, Y)	(GET_BUTTON_INDEX_TYPE_01(X, Y))

class	_YInfo : public _YControl
{
private:
	BOOL		m_bAction;
	XSPR		m_TitleSpr;

	SHORT		m_TextX;
	SHORT		m_TextY;
	SHORT		m_TextWidth;
	SHORT		m_TextHeight;
	
	CHAR		m_szText[1024];

	_Button		m_BOk;	
private:
	VOID		SetAction(BOOL bAction);
public:
	_YInfo();
	~_YInfo();

	VOID	Create(CHAR* lpText);
	VOID	Create( SHORT x, SHORT y, 
					SHORT OkX, SHORT OkY,										//다이알로그 박스의 크기.				    
				    SHORT TextX,  SHORT TextY,  SHORT TextWidth,  SHORT TextHeight,  
				    CHAR* lpText);	//텍스트의 위치 및 텍스트.
	VOID	Free();

	VOID	Action(BOOL bPush);
	VOID	Draw(LPDIRECTDRAWSURFACE7 lpSurface);		

	
	BOOL	IsAction();

};

extern	_YInfo	YInfo;
*/
#endif
