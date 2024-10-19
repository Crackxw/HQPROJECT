
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: MacroCheckProcess.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

///////////////////////////////////////////////////////////

#ifndef __MACRO_CHECK_PROCESS_H_INCLUDED__
#define __MACRO_CHECK_PROCESS_H_INCLUDED__

///////////////////////////////////////////////////////////

#include <vector>

///////////////////////////////////////////////////////////

#define PROCESS_IS_UNKNOWN					0
#define PROCESS_IS_RUNNING					1
#define UNABLE_TO_TERMINATE_PROCESS_SOME	602
#define PROCESS_IS_NOT_CURRENTLY_RUNNING	603
#define NO_PERMISSION_TO_TERMINATE_PROCESS	604
#define UNABLE_TO_SEARCH_FOR_PROCESS		605
#define UNABLE_TO_IDENTIFY_SYSTEM_TYPE		606
#define UNSUPPORTED_OS						607
#define INVALID_PROCESS_NAME				632

///////////////////////////////////////////////////////////

class COnlineClientCallback;

class CMacroCheckProcess
{
public : 
	CMacroCheckProcess(COnlineClientCallback* pOnlineClient);
	~CMacroCheckProcess();

private : 
	static unsigned int WINAPI Run_Process(void* vpThis);
	unsigned int Process();
	BOOL MacroCheckProc();
	
public : 
	void	StartThread();
	void	StopThread();

	int		FindProcess(const char *szToTerminate);
	LPCTSTR GetProcessName();
	int		IsThereAnyOtherInstance(const char* szMacroProgName);
	void	SetSendAble(BOOL bSendable);	
private : 
	unsigned int	m_uiThreadID;
	BOOL			m_bThreadRun;

	HANDLE			m_hBreakThread;
	HANDLE			m_hExitThread;

private : 
	BOOL			m_bMacroProg;
	BOOL			m_bIsSendable;
	BOOL			m_bMacroInfoSend;
	
private : 
	COnlineClientCallback* volatile	m_pOnlineClient;
};

///////////////////////////////////////////////////////////

#endif // __MACRO_CHECK_PROCESS_H_INCLUDED__

///////////////////////////////////////////////////////////