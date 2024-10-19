#ifndef _KEYBUFFER_H
#define _KEYBUFFER_H

#define MAX_MULTI_TEST_ORDER_BUFFER   3000
#define MAX_MULTI_TEST_FRAME_NUMBER   50000

#define MAX_KEY_BUFFER  100


struct _KeyBuffer{
	SHORT LeftPressSwitch;
	SHORT RightPressSwitch;
	SHORT Mouse_X;
	SHORT Mouse_Y;
	DWORD wParam;
	char  MoveSwitch;                   // 마우스가 이동하였는가?
};

class _clKeyBuffer{
private:
	int KeyBufferIndex;
	_KeyBuffer KeyBuffer[MAX_KEY_BUFFER];
	SHORT AKey[256];	

public:
	_clKeyBuffer();

	void SetAKey(WPARAM key, BOOL mode);
	BOOL GetAKey(WPARAM key);
	void InitKeyBuffer();
	void PushKeyBuffer(SHORT leftpressswitch, SHORT rightpressswitch,  SHORT mousex, SHORT mousey, DWORD wparam, char moveswitch);
	BOOL PopKeyBuffer(SHORT& leftpressswitch, SHORT& rightpressswitch, SHORT &mousex, SHORT &mousey, DWORD& wparam, char &moveswitch);

};

#endif