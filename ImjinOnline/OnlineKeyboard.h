#ifndef	_ONLINE_KEYBAORD_HEADER
#define _ONLINE_KEYBAORD_HEADER

class	OnlineKeyboard
{
private:
	SI16	siKeyState[256];

public:
	OnlineKeyboard();

	VOID	Init();										// �ʱ�ȭ�� �����Ѵ�.
	VOID	Action();									// �׼�.
	SI16	GetKeyStatus(UI08 uiVKey);					// Ű�� ���¸� ���´�.
	VOID	SetKeyStatus(UI08 uiVKey, SI16 siState);	// Ű�� ���¸� �����Ѵ�.
};

#endif
