#include <Main.h>
#include "OnlineWorld.h"
#include "OnlineGoodsListBox-Button.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��ư�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsButton::Create(INT Width, INT Height, CHAR* MenuName, CItemHeader *pItem, MONEY uiPrice, UI16 uiQuantity, cltOnlineWorld *pOnlineWorld)
{
	_Button::Create(0, 0, Width, Height, MenuName, BUTTON_PUT_LEFT, TRUE);
	
	GoodsButton::pItem		=	pItem;
	GoodsButton::uiPrice	=	uiPrice;
	GoodsButton::uiQuantity	=	uiQuantity;
	pMyOnlineWorld			=	pOnlineWorld;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��ư�� ����ϴ� �Լ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	GoodsButton::Put(SI16 siX, SI16 siY, LPDIRECTDRAWSURFACE7 pSurface)
{
	//HDC			hdc;
	//CHAR		szTextBuffer[1024];
	COLORREF	TextColor = RGB(64, 64, 64);
	//SI32		siYOffset;

	if(_Button::IsProcess() == FALSE)
	{
		if(IsSelected() == TRUE)	
			TextColor	=	RGB(255, 10, 250);
	}		
	else
	{
		if(IsSelected() == TRUE)	
			TextColor	=	RGB(255, 10, 250);			
		else if(IsInMouse(siX, siY) == TRUE)	
			TextColor	=	RGB(200, 200, 200);			
	}
/*	
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteCT(siX, siY, pMyOnlineWorld->pItemsInfo->m_Sprite.Header.Xsize, pMyOnlineWorld->pItemsInfo->m_Sprite.Header.Ysize, 
							&pMyOnlineWorld->pItemsInfo->m_Sprite.Image[pItem->m_siSpriteFont]);
		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		SelectObject(hdc, BriefingFont);
		
		SetTextColor(hdc, TextColor);

		siYOffset	=	0;

		sprintf(szTextBuffer, "�̸� : [%s]", pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));			
		TextOut(hdc, siX + 50, siY, szTextBuffer, strlen(szTextBuffer));

		if(SI16(uiPrice) != -1)
		{
			siYOffset	+=	13;

			sprintf(szTextBuffer, "���� : [%d]", uiPrice);
			TextOut(hdc, siX + 50, siY + siYOffset, szTextBuffer, strlen(szTextBuffer));
		}

		if(uiQuantity != -1)
		{
			siYOffset	+=	13;

			sprintf(szTextBuffer, "���� : [%d]", uiQuantity);
			TextOut(hdc, siX + 50, siY + siYOffset, szTextBuffer, strlen(szTextBuffer));		
		}

		pSurface->ReleaseDC(hdc);
	}
*/
}