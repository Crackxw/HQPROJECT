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
					SHORT OkX, SHORT OkY,										//���̾˷α� �ڽ��� ũ��.				    
				    SHORT TextX,  SHORT TextY,  SHORT TextWidth,  SHORT TextHeight,  
				    CHAR* lpText);	//�ؽ�Ʈ�� ��ġ �� �ؽ�Ʈ.
	VOID	Free();

	VOID	Action(BOOL bPush);
	VOID	Draw(LPDIRECTDRAWSURFACE7 lpSurface);		

	
	BOOL	IsAction();

};

extern	_YInfo	YInfo;
*/
#endif
