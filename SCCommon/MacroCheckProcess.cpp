
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: MacroCheckProcess.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

///////////////////////////////////////////////////////////

#include <GSL.h>
#include "MacroCheckProcess.h"
#include "onlineclientcallback.h"
#include <assert.h>
#include <process.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <tchar.h>

///////////////////////////////////////////////////////////

const char *MAJACPROGNAME[3]	= {"ms0701.exe", "dmajak.exe", "vinus 1.0 ゎ陛っ.exe"};
const WORD  MAJACPROGCOUNT		= 3;

///////////////////////////////////////////////////////////

CMacroCheckProcess::CMacroCheckProcess(COnlineClientCallback* pOnlineClient) : 
m_pOnlineClient(pOnlineClient),
m_uiThreadID(0),
m_bThreadRun(FALSE), 
m_hBreakThread(NULL),
m_hExitThread(NULL), 
m_bMacroProg(FALSE), 
m_bIsSendable(FALSE), 
m_bMacroInfoSend(FALSE)
{
	m_hBreakThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hExitThread  = CreateEvent(NULL, FALSE, FALSE, NULL);
}

///////////////////////////////////////////////////////////

CMacroCheckProcess::~CMacroCheckProcess()
{
	StopThread();

	if(m_hBreakThread != NULL)
	{
		CloseHandle(m_hBreakThread);
		m_hBreakThread = NULL;
	}
	
	if(m_hExitThread != NULL)
	{
		CloseHandle(m_hExitThread);
		m_hExitThread = NULL;
	}
}

///////////////////////////////////////////////////////////

unsigned int CMacroCheckProcess::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try
	{
		uiRetVal = reinterpret_cast<CMacroCheckProcess*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...)
	{
		assert(0);
		return uiRetVal;
	}
}

///////////////////////////////////////////////////////////

unsigned int CMacroCheckProcess::Process()
{
	DWORD dwResult = 0;

	while(TRUE)
	{
		dwResult = WaitForSingleObject(m_hBreakThread, 5000);
		if(dwResult == WAIT_OBJECT_0)
		{
			break;
		}

		if(MacroCheckProc() == TRUE)
			 break;
	}

	m_bThreadRun = FALSE;
	SetEvent(m_hExitThread);

	return 0;
}

///////////////////////////////////////////////////////////

BOOL CMacroCheckProcess::MacroCheckProc()
{
	int n = 0;
	for(n = 0 ; n < MAJACPROGCOUNT ; n++)
	{	
		if(IsThereAnyOtherInstance(MAJACPROGNAME[n]) == PROCESS_IS_RUNNING)
		{
			OutputDebugString("Macro using\n");		
			m_bMacroProg = TRUE;
			break;
		}
	}
	
	if(m_bMacroProg == TRUE)
	{
		if(m_bIsSendable == TRUE && m_bMacroInfoSend == FALSE)
		{
			if(m_pOnlineClient->SendMacroUsingUserInfo(MAJACPROGNAME[n]) == TRUE)
			{				
				m_bMacroInfoSend = TRUE;
				return TRUE;
			}
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////

void CMacroCheckProcess::StartThread()
{
	if(m_bThreadRun == TRUE)
		return;

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CMacroCheckProcess*>(this), 0, &m_uiThreadID);

	if(hThread != NULL) 
	{
		OutputDebugString("Start process thread!!\n");
		m_bThreadRun = TRUE;
		CloseHandle(hThread);		
	}
}

///////////////////////////////////////////////////////////

void CMacroCheckProcess::StopThread()
{
	if(m_bThreadRun == FALSE)
		return;

	m_bThreadRun = FALSE;
	SetEvent(m_hBreakThread);

	WaitForSingleObject(m_hExitThread, INFINITE);

	OutputDebugString("Process thread end!!\n");
}

///////////////////////////////////////////////////////////

void CMacroCheckProcess::SetSendAble(BOOL bSendable)
{
	m_bIsSendable = bSendable;
}

///////////////////////////////////////////////////////////

int CMacroCheckProcess::FindProcess(const char *szToTerminate)
{
	typedef DWORD (WINAPI *PEnumProcesses)( DWORD*, UINT, DWORD* );
	typedef DWORD (WINAPI *PEnumProcessModules)( HANDLE, HMODULE*, UINT, DWORD* );
	typedef DWORD (WINAPI *PGetModuleBaseName)( HANDLE, HMODULE, LPTSTR, UINT );

	_TCHAR   szProcessName[MAX_PATH] = _T("unknown");	
    DWORD    processID[1024], cbNeeded, cProcesses;
    UINT i;
	int result = -1;


	HINSTANCE hPsApi = LoadLibrary( _T("PSAPI.DLL") );

	if ( hPsApi == NULL )
		return PROCESS_IS_UNKNOWN;

	PEnumProcessModules EnumProcessModules = (PEnumProcessModules)GetProcAddress( hPsApi, "EnumProcessModules" );
#ifdef UNICODE
	PGetModuleBaseName GetModuleBaseName = (PGetModuleBaseName)GetProcAddress( hPsApi, "GetModuleBaseNameW" );
#else
	PGetModuleBaseName GetModuleBaseName = (PGetModuleBaseName)GetProcAddress( hPsApi, "GetModuleBaseNameA" );
#endif

	PEnumProcesses EnumProcesses = (PEnumProcesses)GetProcAddress( hPsApi, "EnumProcesses" );

	if ( EnumProcessModules == NULL || 
		 GetModuleBaseName == NULL ||
		 EnumProcesses == NULL )
		return PROCESS_IS_UNKNOWN;


  // Get the list of process identifiers.
	
    if ( !EnumProcesses( processID, sizeof(processID), &cbNeeded ) )
		return result;

  // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

  // Print the name and process identifier for each process.

//	ClearProcessList();
    for ( i = 0; i < cProcesses; i++ )
	{
	  // Get a handle to the process.

	    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
										FALSE, 
										processID[i] );
 

	    if ( hProcess )
		{
			HMODULE hMod;

		    if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
	        {
		        GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName) );

//				ProcItemStruct *procItem = new ProcItemStruct;
//				procItem->name = szProcessName;
//				procItem->id = processID[i];
//				this->m_processList.push_back(procItem);

				strcpy(szProcessName, strlwr(szProcessName));
				if(strcmp(szToTerminate, szProcessName) == 0)
				{
					CloseHandle( hProcess );
					return PROCESS_IS_RUNNING;
				}
			}
	    }

		CloseHandle( hProcess );

	}

   return PROCESS_IS_UNKNOWN;
}

///////////////////////////////////////////////////////////

LPCTSTR CMacroCheckProcess::GetProcessName()
{
	return "dmajak.exe";
}

///////////////////////////////////////////////////////////

int CMacroCheckProcess::IsThereAnyOtherInstance(const char* szMacroProgName)
{
	int ret;
	
	ret=FindProcess(szMacroProgName);	

	return ret;
}

///////////////////////////////////////////////////////////

