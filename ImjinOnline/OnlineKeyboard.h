#ifndef	_ONLINE_KEYBAORD_HEADER
#define _ONLINE_KEYBAORD_HEADER

class	OnlineKeyboard
{
private:
	SI16	siKeyState[256];

public:
	OnlineKeyboard();

	VOID	Init();										// 초기화를 수행한다.
	VOID	Action();									// 액션.
	SI16	GetKeyStatus(UI08 uiVKey);					// 키의 상태를 얻어온다.
	VOID	SetKeyStatus(UI08 uiVKey, SI16 siState);	// 키의 상태를 설정한다.
};

#endif
