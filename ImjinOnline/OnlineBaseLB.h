#ifndef _ONLINE_BASELB_H
#define _ONLINE_BASELB_H

#include "Online.h"
#include <Graphics16.h>
#include "..\Gersang\YListBoxControl.h"

// ������ ������ ����Ʈ �ڽ��� ��Ʈ��
class OnlineSelectLB : public _YListBox
{
private:	
	XSPR			*pBorderXspr;	
	XSPR			*pHandleXspr;
	SI32			siHandleY;	

	RECT			rcDragArea;
	BOOL			bDrag;
	UI08			fDisable[MAX_LIST_BOX_SELL];
	COLORREF		m_Color[MAX_LIST_BOX_SELL];		// actdoll (2004/01/13 14:07) : �� ����Ʈ �ڽ��� �÷� ����� ���� ��ġ
	BOOL			m_bDrawItemColor;				// actdoll (2004/01/13 14:07) : ������ �÷��� ��⸦ ������ ���ΰ� �����ΰ�?
	BOOL			m_bUpDnBttn;


	SI16			siNowMouseX;
	SI16			siNowMouseY;

public:
	VOID			Create(SHORT x, SHORT y, SHORT width, SHORT height, 
							SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight, 
							XSPR *pBorder, XSPR *pHandle, SI16 siHandleAxis, SI16 siHandleTop, SI16 siHandleBottom , BOOL bUpDnBttn = FALSE);			// ����Ʈ �ڽ��� �����Ѵ�.

	VOID			Draw(LPDIRECTDRAWSURFACE7 pSurface, HGDIOBJ hFont = NULL, COLORREF Color = 0, COLORREF DisableColor = 0, BOOL fSelectColor = FALSE, COLORREF SelectColor = 0, COLORREF SelectSellColor = 0);	// ���ڷ� �Ѿ�� ��Ʈ�� ����Ͽ� �ؽ�Ʈ�� ����ϸ� ����Ʈ �ڽ��� �׸���.
	VOID			DrawScrollBar(LPDIRECTDRAWSURFACE7 pSurface);																																					// ����Ʈ �ڽ��� ��ũ�ѹٸ� ����ϴ� �Լ�. 
	VOID			Action(SI16 siMouseX, SI16 siMouseY, BOOL bMousePushStatus, SI32 *psiSelectSell, BOOL bSelect = TRUE);																												// �� �������� �׼��� ����

	VOID			SetItemDisable	(SI32 ItemIndex, BOOL Disable);											// Ư�� Item�� Disable��
	VOID	SetItemColor		( SI32 ItemIndex, COLORREF color = RGB(255, 255, 255) );			// actdoll (2004/01/13 14:16) : Ư�� �����۸� �÷��� �ٲ㼭 ����
	void	EnableDrawItemColor	( BOOL bEnable )		{ m_bDrawItemColor = bEnable; }	// actdoll (2004/01/13 14:16) : Ư�� �����۸� �־��� �÷��� ��⸦ Ȱ��ȭ ��ų ���ΰ�?
	void	ClearItemColor		( void );														// actdoll (2004/01/13 14:16) : ���õ� Ư�� ������ �÷��� ��� �ʱ�ȭ��Ų��.
};

#endif
