#ifndef _ITEM_H
#define _ITEM_H   1




class _Item {
private:
	SHORT Order;
    SHORT Font;
	SHORT MultiSwitch;
	unsigned char  HotKeyCode[2];    // ����Ű 
	char* Text;
	char* Text2;

public:
	void SetItem(SHORT order, SHORT font, SHORT multi, char hotkey1, char hotkey2, char*text, char*text2);
	SHORT GetOrder();
	SHORT GetFont();
	SHORT GetMultiSwitch();
	unsigned char GetHotKeyCode(int vary);
	char* GetText();
	char* GetText2();

	DWORD GetUnique();

};



void ItemInit();
// �޴� �Էµ��� ó���Ѵ�. 
void OperateMenu();
// �޴��� �׸���. 
void DrawMenu(LPDIRECTDRAWSURFACE7 surface);

void DrawGeneralInfo(LPDIRECTDRAWSURFACE7 surface);

// �κ��丮�� ������ ǥ���Ѵ�. 
void DrawInventory(LPDIRECTDRAWSURFACE7 surface);

// ���õ� ������ ������ ���������� ����ϴ� �޴��� �ִ´�.
void PushGlobalMenu();
// ��� �޴��� �����Ѵ�.
void PushGeneralMenu();

// ��� �κ��丮 �޴��� �����Ѵ�.
void PushInventoryMenu(SI16 siId);

// ��� �޴����� �Է� ���� ����� ó���Ѵ�. 
void OperateGeneralMenu(SHORT order);

// ���� �޴��� �����. 
void ClearMenu();


#endif