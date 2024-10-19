#ifndef	IMJIN2_OKCANCEL_HEADER
#define IMJIN2_OKCANCEL_HEADER

#include "ycontrol.h"

#define		Y_OKCANCEL_TITLE_TEXTCOLOR			(RGB(254, 254, 254))
#define		Y_OKCANCEL_TEXT_TEXTCOLOR			(RGB(254, 254, 254))

#define		Y_OKCANCEL_ONLY_OK					0
#define		Y_OKCANCEL_ONLY_CANCEL				1
#define		Y_OKCANCEL_OKCANCEL					2

#define		Y_OKCANCEL_OK		1
#define		Y_OKCANCEL_CANCEL	2

#define		Y_OKCANCEL_SMALLBORDER_FILENAME		"yfnt\\okcancelborder.spr"
#define		Y_OKCANCEL_LARGEBORDER_FILENAME		"yfnt\\infoborder.spr"

// 보더의 사이즈
#define		Y_OKCANCEL_SMALLBORDER				0			// 큰 보더.
#define		Y_OKCANCEL_LARGEBORDER				1			// 작은 보더.

// Create를 호출할때의 현재 메뉴.
#define		Y_OKCANCEL_DEFAULTMENU				0			// 보통 메뉴.
#define		Y_OKCANCEL_MAINMENU					1			// 메인 메뉴.
#define		Y_OKCANCEL_BATTLENETMENU			2			// 배틀넷 메뉴.

// 보더 스프라이트 폰트 번호
#define		Y_OKCANCEL_BORDER_DEFAULT_FONT		0			// 보통 메뉴.
#define		Y_OKCANCEL_BORDER_MAINMENU_FONT		1			// 메인 메뉴.
#define		Y_OKCANCEL_BORDER_BATTLEMENU_FONT	1			// 배틀넷 메뉴.

// 버튼 스프라이트 인덱스.
#define		Y_OKCANCEL_BUTTONSPRINDEX_MAIN			BUTTONS_TYPE_06_01
#define		Y_OKCANCEL_BUTTONSPRINDEX_DEFAULT		BUTTONS_TYPE_06_01				// 보통 메시지 박스.
#define		Y_OKCANCEL_BUTTONSPRINDEX_BATTLE		BUTTONS_BATTLENET_BUTTONS		// 배틀넷에서 나오는 메시지 박스.

// Ok 스프라이트 폰트 번호.
#define		Y_OKCANCEL_OK_DEFAULT_FONT			0										// 보통 메뉴.
#define		Y_OKCANCEL_OK_MAINMENU_FONT			0										// 메인 메뉴.
#define		Y_OKCANCEL_OK_BATTLEMENU_FONT		BUTTONS_FONTNUM_BATTLENET_OK			// 배틀넷 메뉴.

// Cancel 스프라이트 폰트 번호.
#define		Y_OKCANCEL_CANCEL_DEFAULT_FONT			1										// 보통 메뉴.
#define		Y_OKCANCEL_CANCEL_MAINMENU_FONT			1										// 메인 메뉴.
#define		Y_OKCANCEL_CANCEL_BATTLEMENU_FONT		BUTTONS_FONTNUM_BATTLENET_CANCEL		// 배틀넷 메뉴.

			
#define		Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(X, Y)	(GET_BUTTON_INDEX_TYPE_01(X, Y))

class XSPR;

class	_YOkCancel : public _YControl
{
private:
	BOOL		m_bAction;
	XSPR		m_TitleSpr;

	SI16		MsiButtonSprIndex;
	SI16		MsiBorderFont;			
	SI16		MsiOkFont;
	SI16		MsiCancelFont;
	SI16		MsiMenu;
	
	SHORT		m_nControl;

	SHORT		m_TextX;
	SHORT		m_TextY;
	SHORT		m_TextWidth;
	SHORT		m_TextHeight;
	
	CHAR		m_szText[1024];

	SI16		MsiBorderSize;

	_Button		m_BOk;
	_Button		m_BCancel;
private:
	VOID		SetAction(BOOL bAction);
public:
	_YOkCancel();
	~_YOkCancel();

	VOID	Create(CHAR* lpText, SHORT nControl = Y_OKCANCEL_OKCANCEL, SI16	siBorderSize = Y_OKCANCEL_SMALLBORDER, SI16 siMenu = Y_OKCANCEL_DEFAULTMENU);
	VOID	Create( SHORT x, SHORT y, 
					SHORT OkX, SHORT OkY,										
				    SHORT CancelX, SHORT CancelY,						   
				    SHORT TextX,  SHORT TextY,  SHORT TextWidth,  SHORT TextHeight,  
				    CHAR* lpText, SHORT nControl = Y_OKCANCEL_OKCANCEL, SI16 siBorderSize = Y_OKCANCEL_SMALLBORDER,
					SI16 siMenu = Y_OKCANCEL_DEFAULTMENU);	
	VOID	Free();

	BOOL	Action(BOOL bPush, SHORT* nOkCancel);
	VOID	Draw(LPDIRECTDRAWSURFACE7 lpSurface);		

	
	BOOL	IsAction();

};


extern	_YOkCancel	YOkCancel;
#endif