

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: Authorization.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __AUTHORIZATION_H_INCLUDED__
#define __AUTHORIZATION_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include "AuthConnectSocket.h"
#include "_LoginDefine.h"
#include "_AuthDefine.h"

// Forword Declaration.
class CMSSQL;
class CAuthCallback;

//////////////////////////////////////////////////////////////////////

class CAuthorizationData
{
	friend class CAuthorization;
	
public : 
	CAuthorizationData() {}
	CAuthorizationData(const CAuthorizationData& rhs)
	{
		memcpy(&m_sLoginInfo, &rhs.m_sLoginInfo, sizeof(AUTH::sRequestLogin));
		m_sUserID.set(rhs.m_sUserID.orderKey, rhs.m_sUserID.arrayIndex);
		m_sGameServerID.set(rhs.m_sGameServerID.orderKey, rhs.m_sGameServerID.arrayIndex);
		m_dwDBAccount = rhs.m_dwDBAccount;
	}
	~CAuthorizationData() {}

public : 
	CAuthorizationData& operator=(const CAuthorizationData& rhs)
	{
		if(this == &rhs)
			return *this;

		memcpy(&m_sLoginInfo, &rhs.m_sLoginInfo, sizeof(AUTH::sRequestLogin));
		m_sUserID.set(rhs.m_sUserID.orderKey, rhs.m_sUserID.arrayIndex);
		m_sGameServerID.set(rhs.m_sGameServerID.orderKey, rhs.m_sGameServerID.arrayIndex);
		m_dwDBAccount = rhs.m_dwDBAccount;

		return *this;
	}

	void	Set(AUTH::sUniqueID sUserID, AUTH::sUniqueID sGameServerID, 
		AUTH::sRequestLogin sLoginInfo, DWORD dwDBAccount = 0)
	{
		memcpy(&m_sUserID, &sUserID, sizeof(AUTH::sUniqueID));
		memcpy(&m_sGameServerID, &sGameServerID, sizeof(AUTH::sUniqueID));
		memcpy(&m_sLoginInfo, &sLoginInfo, sizeof(AUTH::sRequestLogin));
		m_dwDBAccount = dwDBAccount;
	}

private : 
	AUTH::sRequestLogin	m_sLoginInfo;
	AUTH::sUniqueID		m_sUserID;
	AUTH::sUniqueID		m_sGameServerID;
	DWORD				m_dwDBAccount;
};

//////////////////////////////////////////////////////////////////////

class CAuthorization : public CAuthConnectSocket, public CParentCallback
{
public : 
	CAuthorization(CAuthCallback* pAuthCallback);
	~CAuthorization();

private : 
	static unsigned int WINAPI	Run_Process(void* vpThis);
	unsigned int Process();

private : 

	int  RequestDBLogin	(const char* pAccountID, const char* pPassword, const char* pIP, int nWorldID, DWORD* dwDBAccount);
	int  DB_Login		(const char* pAccountID, const char* pPassword, const char* pIP, WORD  wWorldID, DWORD dwIDNumber, DWORD* pdwAccount);
	BOOL AuthorizationProcess();
	int	 GashReponseCheck(char* pOutStr);
	
public : 
	void StopThread();

protected : 
	BOOL SendAuthorization
		(
			char	szAccountID[LOGIN::ID_MAX_LENGTH],
			char	szPassword[LOGIN::PW_MAX_LENGTH],		
			char	szIPAddress[16]
		);
	void GetPDU(const char* pPDU, SHORT& nResult, char* pOutputString);
	
	virtual void CallbackDisConnect();

public : 
	void PushCmdQueue(CAuthorizationData pData);
	CAuthorizationData PopCmdQueue();
	void ClearAllCmdQueue();
	int	 CmdQueueSize();

	void StartThread();
	void SetAuthorizationInfo(char* szServiceCode, char* szRegion, 
		char* szAuthenticatonFlag, char* szType, char* szDuplicateCheck);
	BOOL DBConnect(char* pDBIP, char* pDBName, char* pID, char* pPassword);

private : 
	BOOL			m_bAuthThreadRun;
	unsigned int	m_uiAuthThreadID;
	HANDLE			m_hAuthBreakEvent;
	HANDLE			m_hAuthExitEvent;
private : 
	// Game server로 send 관련 클래스 포인터.
	CAuthCallback* volatile m_pAuthCallback;

private : 
	std::queue<CAuthorizationData>	m_CmdQueue;
	
private :
	char			m_szServiceCode[7];
	char			m_szRegion[3];
	char			m_szAuthenticationFlag[2];	
	char			m_szType[2];
	char			m_szDuplicateCheck[2];

private : 
	CMSSQL*			m_AccountDB;
};

//////////////////////////////////////////////////////////////////////

#endif	// __AUTHORIZATION_H_INCLUDED__

//////////////////////////////////////////////////////////////////////