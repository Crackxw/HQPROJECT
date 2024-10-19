#ifndef _ITEM_H
#define _ITEM_H   1




class _Item {
private:
	SHORT Order;
    SHORT Font;
	SHORT MultiSwitch;
	unsigned char  HotKeyCode[2];    // 단축키 
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
// 메뉴 입력등을 처리한다. 
void OperateMenu();
// 메뉴를 그린다. 
void DrawMenu(LPDIRECTDRAWSURFACE7 surface);

void DrawGeneralInfo(LPDIRECTDRAWSURFACE7 surface);

// 인벤토리의 정보를 표시한다. 
void DrawInventory(LPDIRECTDRAWSURFACE7 surface);

// 선택된 유닛이 없을때 전역적으로 사용하는 메뉴를 넣는다.
void PushGlobalMenu();
// 장수 메뉴를 설정한다.
void PushGeneralMenu();

// 장수 인벤토리 메뉴를 설정한다.
void PushInventoryMenu(SI16 siId);

// 장수 메뉴에서 입력 받은 명령을 처리한다. 
void OperateGeneralMenu(SHORT order);

// 현재 메뉴를 지운다. 
void ClearMenu();


#endif