#ifndef _ONLINE_BASELB_H
#define _ONLINE_BASELB_H

#include "Online.h"
#include <Graphics16.h>
#include "..\Gersang\YListBoxControl.h"

// 선택이 가능한 리스트 박스의 컨트롤
class OnlineSelectLB : public _YListBox
{
private:	
	XSPR			*pBorderXspr;	
	XSPR			*pHandleXspr;
	SI32			siHandleY;	

	RECT			rcDragArea;
	BOOL			bDrag;
	UI08			fDisable[MAX_LIST_BOX_SELL];
	COLORREF		m_Color[MAX_LIST_BOX_SELL];		// actdoll (2004/01/13 14:07) : 각 리스트 박스의 컬러 출력을 위한 수치
	BOOL			m_bDrawItemColor;				// actdoll (2004/01/13 14:07) : 아이템 컬러별 찍기를 실행할 것인가 말것인가?
	BOOL			m_bUpDnBttn;


	SI16			siNowMouseX;
	SI16			siNowMouseY;

public:
	VOID			Create(SHORT x, SHORT y, SHORT width, SHORT height, 
							SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight, 
							XSPR *pBorder, XSPR *pHandle, SI16 siHandleAxis, SI16 siHandleTop, SI16 siHandleBottom , BOOL bUpDnBttn = FALSE);			// 리스트 박스를 생성한다.

	VOID			Draw(LPDIRECTDRAWSURFACE7 pSurface, HGDIOBJ hFont = NULL, COLORREF Color = 0, COLORREF DisableColor = 0, BOOL fSelectColor = FALSE, COLORREF SelectColor = 0, COLORREF SelectSellColor = 0);	// 인자로 넘어온 폰트를 사용하여 텍스트를 출력하며 리스트 박스를 그린다.
	VOID			DrawScrollBar(LPDIRECTDRAWSURFACE7 pSurface);																																					// 리스트 박스의 스크롤바를 출력하는 함수. 
	VOID			Action(SI16 siMouseX, SI16 siMouseY, BOOL bMousePushStatus, SI32 *psiSelectSell, BOOL bSelect = TRUE);																												// 각 아이템의 액션을 수행

	VOID			SetItemDisable	(SI32 ItemIndex, BOOL Disable);											// 특정 Item만 Disable함
	VOID	SetItemColor		( SI32 ItemIndex, COLORREF color = RGB(255, 255, 255) );			// actdoll (2004/01/13 14:16) : 특정 아이템만 컬러를 바꿔서 찍음
	void	EnableDrawItemColor	( BOOL bEnable )		{ m_bDrawItemColor = bEnable; }	// actdoll (2004/01/13 14:16) : 특정 아이템만 주어진 컬러로 찍기를 활성화 시킬 것인가?
	void	ClearItemColor		( void );														// actdoll (2004/01/13 14:16) : 세팅된 특정 아이템 컬러를 모두 초기화시킨다.
};

#endif
