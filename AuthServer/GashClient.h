
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: GashClient.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __GASH_CLIENT_H_INCLUDED__
#define __GASH_CLIENT_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include "Authentication.h"
#include "Authorization.h"
#include "Accounting.h"
#include "_WorkDefine.h"
#include "AuthCallback.h"

// Forward Declaration.
class CAuthCallback;

//////////////////////////////////////////////////////////////////////

class CGashClient
{
public : 
	CGashClient(CAuthCallback* pAuthCallback);
	~CGashClient();
	
public : 
	void				CreateGash(char szIP[16], WORD wPort);
	void				PushCmdQueueAuthentication(WORK::sPacketHeader* pHeader, AUTH::sRequestLogin* pData);
	void				PushCmdQueueAuthorization(WORK::sPacketHeader* pHeader, AUTH::sRequestLogin* pData);
	void				PushCmdQueueAccounting(WORK::sPacketHeader* pHeader, AUTH::sRequestLogout* pData);

	CAuthentication*	GetAuthentication()	const;
	CAuthorization*		GetAuthorization()	const;
	CAccounting*		GetAccounting()		const;

public : 
	void				AllStartProcessThread();
	void				AllStopProcessThread();

	void				AllStartConnectThread();
	void				AllStopConnectThread();	
	
	BOOL				DBConnect(char* pDBIP, char* pDBName, char* pID, char* pPassword);
	void				SetAuthenticationInfo(char* szServiceCode, char* szRegion, char* szLockFlag);
	void				SetAuthorizationInfo(char* szServiceCode, char* szRegion, char* szAuthenticatonFlag,char* szType, char* szDuplicateCheck);
	void				SetAccountingInfo(char* szServiceCode, char* szRegion, char* szType);

public : 
	void				LoadConfigFile();

private :
	void				Initialize(CAuthCallback* pAuthCallback);
	void				Finialize();

private : 
	CAuthentication*	m_pAuthentication;
	CAuthorization*		m_pAuthorization;
	CAccounting*		m_pAccounting;
	CLogWriter			m_LogWriter;

private : 
	BOOL				m_bInited;
};

//////////////////////////////////////////////////////////////////////

#endif //__GASH_CLIENT_H_INCLUDED__

//////////////////////////////////////////////////////////////////////