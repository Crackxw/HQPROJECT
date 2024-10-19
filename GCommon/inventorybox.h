#ifndef _INVENTORYBOX_H
#define _INVENTORYBOX_H

void SetInventoryBoxSwitch(BOOL bMode);
BOOL SetInventoryBox(SI32 siId, SI32 itemAtb);
void DrawInventoryBox(LPDIRECTDRAWSURFACE7 surface);
SI32 OperateInventoryBox();

void SetItemHelp(SI32 goods);

// 어떤 종류의 인벤토리 리스트 박스가 열려 있는지 확인한다. 
SI32 GetInventoryBox();

#endif
