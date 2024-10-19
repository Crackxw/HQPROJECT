/******************************************************************************
* comment		:	1. ����� ������ �� Ok, Cancel ��ư
*					2. ������ �޴����� �ʿ��� Ok, Cancel ��ư
******************************************************************************/
#include <GSL.h>
#include <graphics16.h>
#include <main.h>

#include <etc.h>

#include "YOkCancel.h"
#include "yprg.h"
#include "text.h"

extern	_Text 				Text;								// �ؽ�Ʈ 
extern	char 				SavePath[];

_YOkCancel	YOkCancel;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
_YOkCancel::_YOkCancel() : _YControl()
{
	m_bAction			=	FALSE;
	m_TitleSpr.Image	=	NULL;
	
	ZeroMemory(m_szText, sizeof(m_szText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
_YOkCancel::~_YOkCancel()
{
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ok, Cancel ��Ʈ���� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_YOkCancel::Create(CHAR* lpText, SHORT nControl, SI16 siBorderSize, SI16 siMenu)
{
	CHAR	szBorderFileName[2][MAX_PATH] = 
	{
		Y_OKCANCEL_SMALLBORDER_FILENAME,	// ���� ��...
		Y_OKCANCEL_LARGEBORDER_FILENAME		// ��Ʋ�ݿ���..
	};
	CHAR	szFileNameBuffer[1024];
	CHAR	szOk[128], szCancel[128];	
	SI16	siOkButtonX, siOkButtonY, siCancelButtonX, siCancelButtonY;

	// ������ �޴��� ���� ����ؾ� �� ��Ʈ�� �ٸ���.
	// �Ȱ��� �̹������� �ȷ�Ʈ�� �ٸ���.
	// ���� �޴��� �ٸ� �ȷ�Ʈ�� ����Ѵ�.
	// ��Ʋ�� ȭ�鵵 �ٸ� �ȷ�Ʈ�� �ٸ� ������ Ȯ�� , ��� ��ư�� ����Ѵ�.
	MsiMenu				=	siMenu;

	switch(MsiMenu)
	{
	case Y_OKCANCEL_MAINMENU:  // ����	
		MsiBorderFont		=	Y_OKCANCEL_BORDER_MAINMENU_FONT;	// 0�� Ÿ��
		MsiOkFont			=	0;
		MsiCancelFont		=	0;
		MsiButtonSprIndex	=	Y_OKCANCEL_BUTTONSPRINDEX_MAIN;		
		strcpy(szOk, Text.Ok.Get());
		strcpy(szCancel, Text.Cancel.Get());		
		break;
	case Y_OKCANCEL_BATTLENETMENU: // ��Ʋ�ݿ���...
		MsiBorderFont		=	Y_OKCANCEL_BORDER_BATTLEMENU_FONT;	// 1 �� �׸�.

		MsiOkFont			=	Y_OKCANCEL_OK_BATTLEMENU_FONT;	  
		MsiCancelFont		=	Y_OKCANCEL_CANCEL_BATTLEMENU_FONT;
		MsiButtonSprIndex	=	Y_OKCANCEL_BUTTONSPRINDEX_BATTLE;
		strcpy(szOk, Text.Ok.Get());
		strcpy(szCancel, Text.Cancel.Get());
		break;	
	default:					// ������	
		MsiBorderFont		=	Y_OKCANCEL_BORDER_DEFAULT_FONT;  // 0�� Ÿ��
		MsiOkFont			=	1;
		MsiCancelFont		=	1;
		MsiButtonSprIndex	=	Y_OKCANCEL_BUTTONSPRINDEX_DEFAULT;		
		strcpy(szOk, Text.Ok.Get());
		strcpy(szCancel, Text.Cancel.Get());
		break;
	}

	//Border�� ��������Ʈ�� �ε��Ѵ�.
	if(m_TitleSpr.Image)	clGrp.Error("FKJE8567", "YOKCANCEL");
	GetFileNamePath(szBorderFileName[siBorderSize], SavePath, szFileNameBuffer);
	if(clGrp.LoadXspr(szFileNameBuffer, m_TitleSpr) == FALSE)	
		clGrp.Error("FKJE8567", "YOKCANCEL [%s]", szFileNameBuffer);

	m_nControl		=	nControl;

	m_x			=	(clGrp.GetScreenXsize() / 2) - (m_TitleSpr.Header.Xsize / 2);
	m_y			=	(clGrp.GetScreenYsize() / 2) - (m_TitleSpr.Header.Ysize / 2);


	m_Controlx		=	m_TitleSpr.Header.Xsize;	
	m_Controly		=	m_TitleSpr.Header.Ysize;		
	
	// �ؽ�Ʈ�� ����� ��ġ�� ��ư�� ��ġ�� �����Ѵ�.
	switch(siBorderSize)
	{
	case Y_OKCANCEL_SMALLBORDER:				
		m_TextX				=	29;
		m_TextY				=	37;
		
		m_TextWidth			=	180;
		m_TextHeight		=	34;
		
		siOkButtonX			=	106;
		siOkButtonY			=	82;
		
		siCancelButtonX		=	166;
		siCancelButtonY		=	82;
		break;				
	case Y_OKCANCEL_LARGEBORDER:		

		m_TextX				=	33;
		m_TextY				=	43;
		
		m_TextWidth			=	330;
		m_TextHeight		=	84;

		switch( nControl )
		{
		case Y_OKCANCEL_ONLY_CANCEL:
		case Y_OKCANCEL_ONLY_OK:
			siOkButtonX			=	278;	// Ȯ�� ��ư�� ��ġ
			siOkButtonY			=	148;	// Ȯ�� ��ư�� ��ġ
			
			siCancelButtonX		=	278;
			siCancelButtonY		=	148;
			break;					
		case Y_OKCANCEL_OKCANCEL:
			siOkButtonX			=	47;		// Ȯ�� ��ư�� ��ġ
			siOkButtonY			=	148;	// Ȯ�� ��ư�� ��ġ
			
			siCancelButtonX		=	254;
			siCancelButtonY		=	148;
			break;
		default:
			clGrp.Error("", "123123");
		}
		break;	
	default:
		clGrp.Error("FKJE8567", "YOKCANCEL000");
		break;
	}	



	SI32			siTempX[2];
	BOOL			bSet;
	bSet = FALSE;
	// ������2 �Ϻ����� ��� �� �� ū ��ư('Ȯ��' ��ư)�� ����ϹǷ� X��ǥ�� �����ش�.
		#ifdef _LANGUAGE_JAPAN
			siTempX[0] = m_x + siOkButtonX - 10;
			siTempX[1] = m_x + siCancelButtonX - 10;
			bSet = TRUE;
		#endif

	if( bSet == FALSE )
	{
		siTempX[0] = m_x + siOkButtonX;
		siTempX[1] = m_x + siCancelButtonX;
	}

	
	m_BOk.Create(siTempX[0], m_y + siOkButtonY,
				g_Buttons_Spr[MsiButtonSprIndex].Header.Xsize, 
				g_Buttons_Spr[MsiButtonSprIndex].Header.Ysize, 
				szOk);

	m_BCancel.Create(siTempX[1], m_y + siCancelButtonY,
				g_Buttons_Spr[MsiButtonSprIndex].Header.Xsize, 
				g_Buttons_Spr[MsiButtonSprIndex].Header.Ysize, 
				szCancel);

	MsiBorderSize	=	siBorderSize;
	
	ZeroMemory(m_szText, sizeof(m_szText));
	
	if(lpText)		strncpy(m_szText,	lpText, sizeof(m_szText) - 1);		
	SetAction(TRUE);	
}
				  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	����ߴ� �޸𸮸� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID _YOkCancel::Free()
{	
	if(m_TitleSpr.Image)	clGrp.FreeXspr(m_TitleSpr);	
	SetAction(FALSE);		
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	ó���Լ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL _YOkCancel::Action(BOOL bPush, SHORT *lpnOkCancel)
{		
	switch(m_nControl)
	{
	case Y_OKCANCEL_ONLY_OK:
		if(m_BOk.Process(bPush) == TRUE)
		{
			*lpnOkCancel = Y_OKCANCEL_OK;
			return TRUE;
		}
		break;
	case Y_OKCANCEL_ONLY_CANCEL:
		if(m_BCancel.Process(bPush) == TRUE)
		{
			*lpnOkCancel = Y_OKCANCEL_CANCEL;
			return TRUE;
		}
		break;
	case Y_OKCANCEL_OKCANCEL:
		if(m_BOk.Process(bPush) == TRUE)
		{			
			*lpnOkCancel = Y_OKCANCEL_OK;		
			return TRUE;
		}

		if(m_BCancel.Process(bPush) == TRUE)
		{
			*lpnOkCancel = Y_OKCANCEL_CANCEL;
			return TRUE;
		}
		break;
	}	
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	����Լ�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	 _YOkCancel::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	SHORT	nTextWidth, nTextHeight;
	SHORT	sx, sy;
	RECT	rect;
	BOOL	bPutOk = FALSE, bPutCancel = FALSE;	
	
	if(lpSurface)
	{
		//Border�� ���� �׸���.
		if(clGrp.LockSurface(lpSurface) == TRUE)
		{			
			clGrp.PutSpriteT(m_x, m_y, m_TitleSpr.Header.Xsize, m_TitleSpr.Header.Ysize,
						&m_TitleSpr.Image[m_TitleSpr.Header.Start[MsiBorderFont]]);					

			switch(m_nControl)
			{	
			case Y_OKCANCEL_ONLY_OK:
				bPutOk	=	TRUE;
				//��ư�� �׸���.
				m_BOk.Put(						
						&g_Buttons_Spr[MsiButtonSprIndex],
						Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiOkFont, BUTTONS_STATUS_NORMAL),
						Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiOkFont, BUTTONS_STATUS_PRESSED),
						Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiOkFont, BUTTONS_STATUS_ONMOUSE),
						BUTTON_PUT_NOMOVE);
				break;
			case Y_OKCANCEL_ONLY_CANCEL:
				bPutCancel	=	TRUE;
				m_BCancel.Put(			
					&g_Buttons_Spr[MsiButtonSprIndex],
					Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiCancelFont, BUTTONS_STATUS_NORMAL),
					Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiCancelFont, BUTTONS_STATUS_PRESSED),
					Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiCancelFont, BUTTONS_STATUS_ONMOUSE),
					BUTTON_PUT_NOMOVE);	
			
				break;
			case Y_OKCANCEL_OKCANCEL:
				bPutOk		=	TRUE;
				bPutCancel	=	TRUE;
				m_BOk.Put(						
						&g_Buttons_Spr[MsiButtonSprIndex],
						Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiOkFont, BUTTONS_STATUS_NORMAL),
						Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiOkFont, BUTTONS_STATUS_PRESSED),
						Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiOkFont, BUTTONS_STATUS_ONMOUSE),
						BUTTON_PUT_NOMOVE);

				m_BCancel.Put(			
					&g_Buttons_Spr[MsiButtonSprIndex],
					Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiCancelFont, BUTTONS_STATUS_NORMAL),
					Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiCancelFont, BUTTONS_STATUS_PRESSED),
					Y_OKCANCEL_GET_BUTTON_INDEX_TYPE(MsiCancelFont, BUTTONS_STATUS_ONMOUSE),
					BUTTON_PUT_NOMOVE);				
				break;
			}	
			
			
			clGrp.UnlockSurface(lpSurface);
		}

		//Text�� �׸���.		
		
		switch(MsiBorderSize)
		{
		case Y_OKCANCEL_SMALLBORDER:
			PutTextToTextBufferOneLine(m_szText, &nTextWidth, &nTextHeight, Y_OKCANCEL_TEXT_TEXTCOLOR);						
			break;
		case Y_OKCANCEL_LARGEBORDER:			
			PutTextToTextBuffer(m_TextWidth, m_szText, &nTextWidth, &nTextHeight, Y_OKCANCEL_TEXT_TEXTCOLOR);						
			break;
		}			
	

		nTextWidth	=	min(nTextWidth,		m_TextWidth);
		nTextHeight	=	min(nTextHeight,	m_TextHeight);
			
		rect.left	=	0;
		rect.top	=	0;
		rect.right	=	rect.left + nTextWidth;
		rect.bottom	=	rect.top + nTextHeight;

		sx			=	m_x + max(m_TextX, m_TextX + (m_TextWidth / 2) - (nTextWidth / 2));
		sy			=	m_y + max(m_TextY, m_TextY + (m_TextHeight / 2) - (nTextHeight / 2));
		
		HRESULT	hr;

		hr	=	lpSurface->BltFast(sx, sy, lpBriefingTextBuffer, &rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		
					
		if(hr != DD_OK)
		{
			switch(hr)
			{
			case DDERR_EXCEPTION :
				clGrp.Error("", "DDERR_EXCEPTION BltFast Error");
				break;				
			case DDERR_GENERIC  :
				clGrp.Error("", "DDERR_GENERIC BltFast Error");
				break;
			case DDERR_INVALIDOBJECT  :
				clGrp.Error("", "DDERR_INVALIDOBJECT BltFast Error");
				break;
			case DDERR_INVALIDPARAMS  :
				clGrp.Error("", "DDERR_INVALIDPARAMS BltFast Error");
				break;
			case DDERR_INVALIDRECT  :
				clGrp.Error("", "DDERR_INVALIDRECT BltFast Error, [%d][%d][%d][%d]", rect.left, rect.top, rect.right, rect.bottom);
				break;
			case DDERR_NOBLTHW  :
				clGrp.Error("", "DDERR_NOBLTHW BltFast Error");
				break;
			case DDERR_SURFACEBUSY  :
				clGrp.Error("", "DDERR_SURFACEBUSY  BltFast Error");
				break;
			case DDERR_SURFACELOST  :
				clGrp.Error("", "DDERR_SURFACELOST BltFast Error");
				break;
			case DDERR_UNSUPPORTED  :
				clGrp.Error("", "DDERR_UNSUPPORTED BltFast Error");
				break;
			case DDERR_WASSTILLDRAWING :
				clGrp.Error("", "DDERR_WASSTILLDRAWING BltFast Error");
				break;
			default:
				clGrp.Error("", "[%d] [%d] BltFast Error", sx, sy);
			}
		}
	}


	if(bPutOk) 		m_BOk.Put(lpSurface, BUTTON_PUT_NOBORDER | BUTTON_PUT_NOMOVE);	
	if(bPutCancel)	m_BCancel.Put(lpSurface, BUTTON_PUT_NOBORDER | BUTTON_PUT_NOMOVE);		

}		


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ȱ��ȭ ��Ų��.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	 _YOkCancel::SetAction(BOOL bAction)
{
	m_bAction = bAction;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	Ȱ��ȭ �Ǿ� �ִ��� �˾ƿ´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	 _YOkCancel::IsAction()
{
	return	m_bAction;
}

