#ifndef	IMJIN2_CDMESSAGE_HEADER
#define IMJIN2_CDMESSAGE_HEADER

VOID	InitCDMessage();
SHORT	CDMessage();
VOID	FreeCDMessage();

VOID	InitLauncherCDChecker();
SI16	LauncherCDChecker();
VOID	FreeLauncherCDChecker();

VOID	InitDpLaunchNotConnected();
BOOL	DpLaunchNotConnected();
VOID	FreeDpLaunchNotConnected();

#endif