
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
#include "..\\JOCommon\\AdoConnection.h"

//////////////////////////////////////////////////////////////////////

class CAdoExecutor
{
public : 
	CAdoExecutor();	
	~CAdoExecutor();

public : 
	BOOL			Connect(char* pAdoConnString);
	void			Initialize();
	void			Finalize();
	BOOL			IsOpen();

	BYTE			Execution(DWORD dwType, void* pMsg);

	CAdoConnection*	GetAdoConnection() const { return m_pAdoConnection; }

private : 
	BYTE			CharListUpdate(void* pMsg);
	BYTE			Set84YearBirthdayUser(void* pMsg);
	
	void			OnADOError(_com_error& e, char* szText);

private : 	
	CAdoConnection*		m_pAdoConnection;
	BOOL				m_bInit;

private : 
	CLogWriter			m_LogWriter;

public : 
	char	m_szAdoConnectionString[256];
};

//////////////////////////////////////////////////////////////////////

#endif