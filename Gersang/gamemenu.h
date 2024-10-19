#ifndef _GAMEMENU_H
#define _GAMEMENU_H

#define	Y_GAME_MENU_MAX_SLOT				20

#define	Y_GAME_MENU_INITMAIN				1000
#define	Y_GAME_MENU_MAIN					1001
#define	Y_GAME_MENU_INITSAVE				1002
#define	Y_GAME_MENU_SAVE					1003
#define	Y_GAME_MENU_INITLOAD				1004	
#define	Y_GAME_MENU_LOAD					1005
#define	Y_GAME_MENU_INITOPTION				1006
#define	Y_GAME_MENU_OPTION					1007
#define	Y_GAME_MENU_INITOBJECTIVE			1008
#define	Y_GAME_MENU_OBJECTIVE				1009

#include <button.h>


VOID	InitGameMenu();
VOID	InitGameLoadMenu();
SHORT	GameMenu();
VOID	FreeMainMenu();

extern	VOID		SetExitMenu();
extern	BOOL		IsExitMenu();
extern	VOID		SetGotoPreviousMenu();
extern	BOOL		IsGotoPreviousMenu();
#endif
