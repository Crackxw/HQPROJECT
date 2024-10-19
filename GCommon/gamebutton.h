#ifndef _GAMEBUTTON_H
#define _GAMEBUTTON_H

// 게임중에 사용되는 버튼에 대한 클래스 
class _GameButton {
public:

	BOOL	bValidSwitch ;

	// 버튼의 타입. 
	SI32	siButtonType;

	// 버튼 시작 위치 
	SI32	siX, siY;
	// 버튼의 가로 세로 크기 
	SI32	siButtonXsize, siButtonYsize;

	// 눌리기 이전의 버튼 배경 그림. 
	SI32	siPrePressBackFile,		siPrePressBackFont;
	BOOL	bPrePressBackDraw32Switch;

	// 눌린 상태의 버튼 배경 그림. 
	SI32	siPressBackFile,			siPressBackFont;
	BOOL	bPressBackDraw32Switch;

	// 마우스가 버튼 위에 있을 때의 배경 그림. 
	SI32	siMouseBackFile,			siMouseBackFont;
	BOOL	bMouseBackDraw32Switch;

	// 버튼 내용 그림. 
	SI32	siButtonImageFile,			siButtonImageFont;
	BOOL	bButtonImageDraw32Switch;

	// 버튼이 눌렸을때의 효과음.
	SI32	siEffect;

	SI32	siPara1, siPara2;
	SI32	(*pFunc)(SI32, SI32);
	SI32	(*pHelpFunc)(SI32, SI32);

	CHAR	pString[64];
	CHAR	pHelpString[128];

	BOOL	bMouseSwitch;
	BOOL	bLeftPressSwitch;

	void	Init();

	// 버튼을 만든다.
	void	SetGameButton(SI32 ButtonType, SI32 X, SI32 Y, SI32 PrePressBackFile,	SI32 PrePressBackFont, BOOL PrePressBackDraw32Switch,
												 SI32 PressBackFile,		SI32 PressBackFont, BOOL PressBackDraw32Switch,
											     SI32 MouseBackFile,		SI32 MouseBackFont, BOOL MouseBackDraw32Switch,
											     SI32 ButtonImageFile,	SI32 ButtonImageFont,   BOOL ButtonImageDraw32Switch,
											     SI32 Effect,
												 SI32 Para1, 
												 SI32 Para2, 
												 SI32 (*Func)(SI32, SI32),
												 SI32 (*HelpFunc)(SI32, SI32) = NULL);

	// 버튼을 만든다.
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

	// 버튼을 그려준다. 
	void	Draw(LPDIRECTDRAWSURFACE7 surface);

	// 마우스가 버튼위에 있는지 확인한다. 
	BOOL	IsMouseInButton();

	void	PUSHEFFECT();


};


#endif
