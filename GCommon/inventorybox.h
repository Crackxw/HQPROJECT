#ifndef _INVENTORYBOX_H
#define _INVENTORYBOX_H

void SetInventoryBoxSwitch(BOOL bMode);
BOOL SetInventoryBox(SI32 siId, SI32 itemAtb);
void DrawInventoryBox(LPDIRECTDRAWSURFACE7 surface);
SI32 OperateInventoryBox();

void SetItemHelp(SI32 goods);

// � ������ �κ��丮 ����Ʈ �ڽ��� ���� �ִ��� Ȯ���Ѵ�. 
SI32 GetInventoryBox();

#endif
