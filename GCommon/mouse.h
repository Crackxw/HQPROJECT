#ifndef _MOUSE_H
#define _MOUSE_H   1

#include <Main.h>

// ���콺�� ���� 
#define MOUSE_ATTACK         1
#define MOUSE_NORMAL         2
#define MOUSE_CHAR           3
#define MOUSE_HEAL           4
#define MOUSE_HARVEST        5
#define MOUSE_RIDE           6
#define MOUSE_SCROLL_SOUTH   7
#define MOUSE_SCROLL_SW      8
#define MOUSE_SCROLL_WEST    9
#define MOUSE_SCROLL_NW     10
#define MOUSE_SCROLL_NORTH  11
#define MOUSE_SCROLL_NE     12
#define MOUSE_SCROLL_EAST   13
#define MOUSE_SCROLL_SE     14
#define MOUSE_SCROLL_SOUTH_X 15
#define MOUSE_SCROLL_SW_X   16 
#define MOUSE_SCROLL_WEST_X 17
#define MOUSE_SCROLL_NW_X   18 
#define MOUSE_SCROLL_NORTH_X 19
#define MOUSE_SCROLL_NE_X   20
#define MOUSE_SCROLL_EAST_X 21
#define MOUSE_SCROLL_SE_X   22
#define MOUSE_MAGIC         23
#define MOUSE_RESULT        24
#define MOUSE_INITMENU      25
#define MOUSE_SETMULTI      26
#define MOUSE_EXTINGUISH    27
#define MOUSE_CHOOSEMISSION 28
#define MOUSE_SETGAME		29
#define MOUSE_PROVIDER		30
#define MOUSE_SESSION		31
#define MOUSE_CHOOSESCENARIO 32
#define MOUSE_BRIEFING      33
#define MOUSE_TRAINSPOT     34

// ���콺 ������ ���� 
#define MOUSEMODE_MOVE        1      // �̵������ϴ�.
#define MOUSEMODE_ATTACK      2      // ���콺 �ؿ� ���� �ִ�. 
#define MOUSEMODE_ATTACKAIR   4      // ���콺 �ؿ� ���� �ִ�. 
#define MOUSEMODE_CHAR        8      // ���콺 �ؿ� ĳ���Ͱ� �ִ�. 
#define MOUSEMODE_HARVEST    16     // ��Ȯ���� �ϴ�.
#define MOUSEMODE_RIDE       32
#define MOUSEMODE_HELPBUILD  64  // �Ǽ����� �ǹ� ���⸦ ���´�. 
#define MOUSEMODE_HEAL      128     // ġ�� �����Ѱ�?
#define MOUSEMODE_WARP		256
#define MOUSEMODE_GOINTOHQ  512
#define MOUSEMODE_EXTINGUISH  1024
#define MOUSEMODE_MANA		  4096

// ������� �Է� ����. 
class _InputDevice{
public:
	SI32	Mouse_X, Mouse_Y;						// ȭ��󿡼� ���콺�� �ִ� ��ǥ 
	BOOL	LeftPressSwitch;						// ���� ��ư�� ������ �ִ��� ���� 
	BOOL	LeftPressDoneSwitch;
	BOOL	RightPressSwitch;						// ������ ��ư�� ������ �ִ��� ���� 
	BOOL	RightPressDoneSwitch;

	SI32	LeftOneSwich;
	
	BOOL	AltKeyPressSwitch ;						// altŰ�� ���ȴ��� ���θ� �˷��ִ� ���� 
	DWORD   PrevLeftUp1Clock, PrevLeftUp2Clock;		// ������ ���콺 ���� Ű�� ������ �ð�. 

	BOOL    WheelUp;
	BOOL    WheelDown;

	BOOL   IsWheelUp();
	BOOL   IsWheelDown();

	BOOL   IsDoubleClick();
};

void SetMouse(SHORT mousemode);
// ���콺�� �׷��ش�. 
void DrawMouse();
// ���콺�� �׷��ش�. 
void DrawMouse(LPDIRECTDRAWSURFACE7 surface);

// ���콺�� �׷��ش�. 
void DrawMousePrimary();

void DrawMouseBuffer();
// �����󿡼� ���콺�� ��ġ�� �������� üũ�Ѵ�. 
BOOL GetMouseMode(SHORT mapx, SHORT mapy);
// �巡�� �� �����ȿ��� ĳ������ ���̵�� ������ ���ؼ� �����Ѵ�. 
void SelectCharInDrag(_NewID* temp_id, SHORT& index, SHORT dragstartx, SHORT dragstarty, SHORT dragendx, SHORT dragendy, SHORT center_id);
BOOL InputSelectChar(SHORT id, SHORT &solo_id, _NewID* temp_id, SHORT& index);
// mapx, mapy �ֺ��� �ִ� ��� ã�� ���� ����� ��� ��ġ�� harvestx, harvesty�� �ִ´�.
BOOL MouseModeHarvest(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT& harvestx, SHORT& harvesty);

#endif
