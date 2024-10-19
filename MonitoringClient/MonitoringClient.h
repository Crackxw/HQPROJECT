#if !defined(AFX_MONITORINGCLIENT_H__2A82945D_3424_4C6B_9FDD_6F1B87C0B295__INCLUDED_)
#define AFX_MONITORINGCLIENT_H__2A82945D_3424_4C6B_9FDD_6F1B87C0B295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MonitoringClient.h : main header file for MONITORINGCLIENT.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientApp : See MonitoringClient.cpp for implementation.

class CMonitoringClientApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORINGCLIENT_H__2A82945D_3424_4C6B_9FDD_6F1B87C0B295__INCLUDED)
