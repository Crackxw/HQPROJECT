#ifndef _GMENU_H
#define _GMENU_H

BOOL		InitInitMenu();					//�ʱ� �޴��� �ʱ�ȭ �Ѵ�.
SHORT		InitMenu();						//�ʱ� �޴��̴�. �޴����� ���õǾ��� ���� �����Ѵ�.
VOID		FreeInitMenu();

VOID	PlayStaffBackgroundMusic();
VOID	StopMenuBackgroundMusic();
VOID	FreeMenuBackgroundMusic();

extern	BOOL		g_bPush;	
#endif
