#ifndef _MOUSE_H
#define _MOUSE_H   1

#include <Main.h>

// 마우스의 종류 
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

// 마우스 상태의 종류 
#define MOUSEMODE_MOVE        1      // 이동가능하다.
#define MOUSEMODE_ATTACK      2      // 마우스 밑에 적이 있다. 
#define MOUSEMODE_ATTACKAIR   4      // 마우스 밑에 적이 있다. 
#define MOUSEMODE_CHAR        8      // 마우스 밑에 캐릭터가 있다. 
#define MOUSEMODE_HARVEST    16     // 수확가능 하다.
#define MOUSEMODE_RIDE       32
#define MOUSEMODE_HELPBUILD  64  // 건설중인 건물 짓기를 돕는다. 
#define MOUSEMODE_HEAL      128     // 치료 가능한가?
#define MOUSEMODE_WARP		256
#define MOUSEMODE_GOINTOHQ  512
#define MOUSEMODE_EXTINGUISH  1024
#define MOUSEMODE_MANA		  4096

// 사용자의 입력 정보. 
class _InputDevice{
public:
	SI32	Mouse_X, Mouse_Y;						// 화면상에서 마우스가 있는 좌표 
	BOOL	LeftPressSwitch;						// 왼쪽 버튼이 눌려져 있는지 여부 
	BOOL	LeftPressDoneSwitch;
	BOOL	RightPressSwitch;						// 오른쪽 버튼이 눌려져 있는지 여부 
	BOOL	RightPressDoneSwitch;

	SI32	LeftOneSwich;
	
	BOOL	AltKeyPressSwitch ;						// alt키가 눌렸는지 여부를 알려주는 변수 
	DWORD   PrevLeftUp1Clock, PrevLeftUp2Clock;		// 이전에 마우스 왼쪽 키가 떨어진 시간. 

	BOOL    WheelUp;
	BOOL    WheelDown;

	BOOL   IsWheelUp();
	BOOL   IsWheelDown();

	BOOL   IsDoubleClick();
};

void SetMouse(SHORT mousemode);
// 마우스를 그려준다. 
void DrawMouse();
// 마우스를 그려준다. 
void DrawMouse(LPDIRECTDRAWSURFACE7 surface);

// 마우스를 그려준다. 
void DrawMousePrimary();

void DrawMouseBuffer();
// 지도상에서 마우스의 위치와 움직임을 체크한다. 
BOOL GetMouseMode(SHORT mapx, SHORT mapy);
// 드래깅 된 범위안에서 캐릭터의 아이디와 갯수를 구해서 리턴한다. 
void SelectCharInDrag(_NewID* temp_id, SHORT& index, SHORT dragstartx, SHORT dragstarty, SHORT dragendx, SHORT dragendy, SHORT center_id);
BOOL InputSelectChar(SHORT id, SHORT &solo_id, _NewID* temp_id, SHORT& index);
// mapx, mapy 주변에 있는 곡물을 찾아 가장 가까운 곡물의 위치를 harvestx, harvesty에 넣는다.
BOOL MouseModeHarvest(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT& harvestx, SHORT& harvesty);

#endif
