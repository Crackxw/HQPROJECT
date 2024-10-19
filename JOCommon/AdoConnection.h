
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoConnection.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////////////////

#ifndef __ADOCONNECTION_H__
#define __ADOCONNECTION_H__

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "Defines.h"
#include "..\\JOCommon\\LogWriter.h"


//////////////////////////////////////////////////////////////////////////////////

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "EndOfFile")

//////////////////////////////////////////////////////////////////////////////////

#pragma warning (disable: 4146)

//////////////////////////////////////////////////////////////////////////////////

class CAdoConnection{
public : 
	bool Execute(LPCTSTR lpstrExec);
	HRESULT Open(LPCTSTR _lpstrConnection="");
	DWORD GetRecordCount(_RecordsetPtr _pRs);
	bool BeginTrans();
	bool CommitTrans();
	bool RollbackTrans();
	bool SetIsolationLevel(IsolationLevelEnum eLevel);
	bool SetCommandTimeout(long lTimeout);
	bool SetConnectionTimeout(long lTimeout);
	bool SetMode(ConnectModeEnum eMode);

	void SetConnectionString(LPCTSTR lpstrConnection);
	LPCTSTR GetConnectionString();
	LPCTSTR GetLastErrorString();
	DWORD GetLastError();
	_ConnectionPtr GetActiveConnection();
	bool IsOpen() const;

	CAdoConnection();
	~CAdoConnection();

private : 	
	void Close();
	void dump_com_error(_com_error& _e);	

private : 
	_ConnectionPtr m_pConnection;

private : 
	CLogWriter	m_LogWriter;

protected : 
	LPCTSTR	m_strConnection;
	LPCTSTR m_strLastError;
	DWORD m_dwLastError;
#ifdef _DEBUG
	int	m_nTrans;
#endif
};

//////////////////////////////////////////////////////////////////////////////////

#endif