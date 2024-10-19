#ifndef	_ONLINE_GOODS_LISTBOX_H
#define _ONLINE_GOODS_LISTBOX_H

// 아이템 리스트 박스에서 보일 수 있는 최대 아이템의 개수
#define		ON_GOODSLISTBOX_MAX_ITEM_NUM		100

#include "Online.h"
#include <Graphics.h>
#include <Button.h>
#include "..\Gersang\YListBoxControl.h"
#include "OnlineGoodsListBox-Button.h"
 
// 물품에 관련된 리스트 박스.
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
	VOID			SetBuySellGoods(SI32 siNewBuySellGoods);			// 아이템 리스트 박스의 아이템을 클릭해서 사거나 팔려고 한다.
public:
	VOID			Create(SHORT x, SHORT y, SHORT width, SHORT height, 
							SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight, SHORT SellWidth, SHORT SellHeight, 
							XSPR *pBorder, XSPR *pHandle, SI16 siHandleAxis, SI16 siHandleTop, SI16 siHandleBottom, cltOnlineWorld *pOnlineWorld);		// 리스트 박스를 생성한다.

	VOID			Action(BOOL bMousePushStatus);
	SHORT			AddSell(CItemHeader *pItem, MONEY uiPrice, UI16 uiQuantity);
	VOID			DelSell(SHORT SellIndex);	

	VOID			SetPosition(SI16 siNewPosition);
	
	VOID			Draw(LPDIRECTDRAWSURFACE7 pSurface, CItem *pItemList);
	VOID			DrawScrollBar(LPDIRECTDRAWSURFACE7 pSurface);

	// GoodsListBox만의 고유 함수들.
	UI16			GetID(SI16 siSellIndex);							// 아이템의 ID를 얻어온다.
	VOID			DecreaseQuantity(UI16 uiID, UI16 uiQuantity);		// 아이템에서 개수를 감소시킨다.	
	VOID			Modify(UI16 uiID, UI16 uiQuantity, MONEY uiPrice);	// 아이템의 ID를 사용하여 해당 아이템의 개수, 가격을 조정한다.

	VOID			SetProcess(BOOL bProcess);							// 아이템 리스트 박스의 작동 여부를 설정한다.
	VOID			SetAction(BOOL bAction);							// 아이템 리스트 박스의 작동 여부를 설정한다.(시각적으로도 나타난다.)
	
	SI32			GetBuySellGoods();									// 아이템 리스트 박스의 아이템을 클릭해서 사거나 팔려고 하는지 알아온다.	
};

#endif
