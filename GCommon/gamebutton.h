#ifndef _GAMEBUTTON_H
#define _GAMEBUTTON_H

// �����߿� ���Ǵ� ��ư�� ���� Ŭ���� 
class _GameButton {
public:

	BOOL	bValidSwitch ;

	// ��ư�� Ÿ��. 
	SI32	siButtonType;

	// ��ư ���� ��ġ 
	SI32	siX, siY;
	// ��ư�� ���� ���� ũ�� 
	SI32	siButtonXsize, siButtonYsize;

	// ������ ������ ��ư ��� �׸�. 
	SI32	siPrePressBackFile,		siPrePressBackFont;
	BOOL	bPrePressBackDraw32Switch;

	// ���� ������ ��ư ��� �׸�. 
	SI32	siPressBackFile,			siPressBackFont;
	BOOL	bPressBackDraw32Switch;

	// ���콺�� ��ư ���� ���� ���� ��� �׸�. 
	SI32	siMouseBackFile,			siMouseBackFont;
	BOOL	bMouseBackDraw32Switch;

	// ��ư ���� �׸�. 
	SI32	siButtonImageFile,			siButtonImageFont;
	BOOL	bButtonImageDraw32Switch;

	// ��ư�� ���������� ȿ����.
	SI32	siEffect;

	SI32	siPara1, siPara2;
	SI32	(*pFunc)(SI32, SI32);
	SI32	(*pHelpFunc)(SI32, SI32);

	CHAR	pString[64];
	CHAR	pHelpString[128];

	BOOL	bMouseSwitch;
	BOOL	bLeftPressSwitch;

	void	Init();

	// ��ư�� �����.
	void	SetGameButton(SI32 ButtonType, SI32 X, SI32 Y, SI32 PrePressBackFile,	SI32 PrePressBackFont, BOOL PrePressBackDraw32Switch,
												 SI32 PressBackFile,		SI32 PressBackFont, BOOL PressBackDraw32Switch,
											     SI32 MouseBackFile,		SI32 MouseBackFont, BOOL MouseBackDraw32Switch,
											     SI32 ButtonImageFile,	SI32 ButtonImageFont,   BOOL ButtonImageDraw32Switch,
											     SI32 Effect,
												 SI32 Para1, 
												 SI32 Para2, 
												 SI32 (*Func)(SI32, SI32),
												 SI32 (*HelpFunc)(SI32, SI32) = NULL);

	// ��ư�� �����.
	void	SetGameButton(SI32 ButtonType,			SI32 X,					SI32 Y, 
									 SI32 PrePressBackFile,		SI32 PrePressBackFont,	BOOL PrePressBackDraw32Switch,
									 SI32 PressBackFile,		SI32 PressBackFont,		BOOL PressBackDraw32Switch,
									 SI32 MouseBackFile,		SI32 MouseBackFont,		BOOL MouseBackDraw32Switch,
									 char *pstr,
									 char *phelpstr,
									 SI32 Effect,
									 SI32 Para1, 
									 SI32 Para2, 
									 SI32 (*Func)(SI32, SI32),
									 SI32 (*HelpFunc)(SI32, SI32) = NULL);

	// ��ư�� �׷��ش�. 
	void	Draw(LPDIRECTDRAWSURFACE7 surface);

	// ���콺�� ��ư���� �ִ��� Ȯ���Ѵ�. 
	BOOL	IsMouseInButton();

	void	PUSHEFFECT();


};


#endif
