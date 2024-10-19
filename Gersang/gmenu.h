#ifndef _GMENU_H
#define _GMENU_H

BOOL		InitInitMenu();					//초기 메뉴를 초기화 한다.
SHORT		InitMenu();						//초기 메뉴이다. 메뉴에서 선택되어진 값을 리턴한다.
VOID		FreeInitMenu();

VOID	PlayStaffBackgroundMusic();
VOID	StopMenuBackgroundMusic();
VOID	FreeMenuBackgroundMusic();

extern	BOOL		g_bPush;	
#endif
