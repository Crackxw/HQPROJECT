#ifndef	_ONLINE_GOODS_LISTBOX_H
#define _ONLINE_GOODS_LISTBOX_H

// ������ ����Ʈ �ڽ����� ���� �� �ִ� �ִ� �������� ����
#define		ON_GOODSLISTBOX_MAX_ITEM_NUM		100

#include "Online.h"
#include <Graphics.h>
#include <Button.h>
#include "..\Gersang\YListBoxControl.h"
#include "OnlineGoodsListBox-Button.h"
 
// ��ǰ�� ���õ� ����Ʈ �ڽ�.
class	GoodsListBox : public _YListBox
{
private:		
	SI32			siBuySellGoods;
	XSPR			*pBorderXspr;
	XSPR			*pHandleXspr;
	SI32			siHandleY;	

	GoodsButton		Buttons[ON_GOODSLISTBOX_MAX_ITEM_NUM];
	
	cltOnlineWorld	*pMyOnlineWorld;
protected:
	VOID			SetBuySellGoods(SI32 siNewBuySellGoods);			// ������ ����Ʈ �ڽ��� �������� Ŭ���ؼ� ��ų� �ȷ��� �Ѵ�.
public:
	VOID			Create(SHORT x, SHORT y, SHORT width, SHORT height, 
							SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight, 
							XSPR *pBorder, XSPR *pHandle, SI16 siHandleAxis, SI16 siHandleTop, SI16 siHandleBottom, cltOnlineWorld *pOnlineWorld);		// ����Ʈ �ڽ��� �����Ѵ�.

	VOID			Action(BOOL bMousePushStatus);
	SHORT			AddSell(CItemHeader *pItem, MONEY uiPrice, UI16 uiQuantity);
	VOID			DelSell(SHORT SellIndex);	

	VOID			SetPosition(SI16 siNewPosition);
	
	VOID			Draw(LPDIRECTDRAWSURFACE7 pSurface, CItem *pItemList);
	VOID			DrawScrollBar(LPDIRECTDRAWSURFACE7 pSurface);

	// GoodsListBox���� ���� �Լ���.
	UI16			GetID(SI16 siSellIndex);							// �������� ID�� ���´�.
	VOID			DecreaseQuantity(UI16 uiID, UI16 uiQuantity);		// �����ۿ��� ������ ���ҽ�Ų��.	
	VOID			Modify(UI16 uiID, UI16 uiQuantity, MONEY uiPrice);	// �������� ID�� ����Ͽ� �ش� �������� ����, ������ �����Ѵ�.

	VOID			SetProcess(BOOL bProcess);							// ������ ����Ʈ �ڽ��� �۵� ���θ� �����Ѵ�.
	VOID			SetAction(BOOL bAction);							// ������ ����Ʈ �ڽ��� �۵� ���θ� �����Ѵ�.(�ð������ε� ��Ÿ����.)
	
	SI32			GetBuySellGoods();									// ������ ����Ʈ �ڽ��� �������� Ŭ���ؼ� ��ų� �ȷ��� �ϴ��� �˾ƿ´�.	
};

#endif
