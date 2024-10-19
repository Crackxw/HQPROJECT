#ifndef	_ONLINE_GOODS_LISTBOX_BUTTON_H
#define _ONLINE_GOODS_LISTBOX_BUTTON_H

#include "Online.h"

#include <Graphics.h>
#include <Button.h>
#include "OnlineItem.h"

// 물품에 관련된 버튼.
class	GoodsButton : public _SelectButton
{
private:
	cltOnlineWorld	*pMyOnlineWorld;	
public:
	CItemHeader		*pItem;								// 아이템에 대한 정보	
	MONEY			uiPrice;							// 아이템의 가격
	UI16			uiQuantity;							// 아이템의 개수

public:
	VOID			Create(INT Width, INT Height, CHAR* MenuName, CItemHeader *pItem, MONEY uiPrice, UI16 uiQuantity, cltOnlineWorld *pOnlineWorld); 
	VOID			Put(SI16 siX, SI16 siY, LPDIRECTDRAWSURFACE7 pSurface);
};


#endif