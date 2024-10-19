
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoExecutor.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __ADO_EXECUTOR_H_INCLUDED__
#define __ADO_EXECUTOR_H_INCLUDED__

//////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include <windows.h>
#include "..\\JOCommon\\LogWriter.h"
#include "..\\CrashMailProtocol\\CrashMailProtocol.h"
#include "..\\JOCommon\\AdoConnection.h"

class XClient;
class CCrashToolServer;

//////////////////////////////////////////////////////////////////////

class CAdoExecutor
{
public : 
	CAdoExecutor();
	CAdoExecutor(CCrashToolServer* pServer);
	~CAdoExecutor();

public : 
	BOOL			Connect();	
	void			Initialize();
	void			Finalize();

	BOOL			Execution(DWORD dwType, void* pMsg, XClient* pClient = NULL);

	CAdoConnection*	GetAdoConnection() const { return m_pAdoConnection; }

private : 
	SHORT			CrashUserInfoSave(void* pMsg);


	SHORT			OnCrashToolSearchInfo(void* pMsg, XClient* pClient);
	SHORT			CrashToolSearchInfo(XClient* pClient);
	SHORT			CrashToolSearchInfo
		(
			ON_P_REQ_SEARCHCRASHINFO pInfo, 
			XClient* pClient
		);

	SHORT CrashToolStatisticsInfo(void* pMsg, XClient* pClient);

	
	void			OnADOError(_com_error& e, char* szText);

private : 
	CCrashToolServer*	m_pServer;
	CAdoConnection*		m_pAdoConnection;
	BOOL				m_bInit;

private : 
	CLogWriter			m_LogWriter;

public : 
	char	m_szAdoConnectionString[256];
};

//////////////////////////////////////////////////////////////////////

#endif