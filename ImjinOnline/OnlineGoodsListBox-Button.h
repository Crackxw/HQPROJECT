#ifndef	_ONLINE_GOODS_LISTBOX_BUTTON_H
#define _ONLINE_GOODS_LISTBOX_BUTTON_H

#include "Online.h"

#include <Graphics.h>
#include <Button.h>
#include "OnlineItem.h"

// ��ǰ�� ���õ� ��ư.
class	GoodsButton : public _SelectButton
{
private:
	cltOnlineWorld	*pMyOnlineWorld;	
public:
	CItemHeader		*pItem;								// �����ۿ� ���� ����	
	MONEY			uiPrice;							// �������� ����
	UI16			uiQuantity;							// �������� ����

public:
	VOID			Create(INT Width, INT Height, CHAR* MenuName, CItemHeader *pItem, MONEY uiPrice, UI16 uiQuantity, cltOnlineWorld *pOnlineWorld); 
	VOID			Put(SI16 siX, SI16 siY, LPDIRECTDRAWSURFACE7 pSurface);
};


#endif