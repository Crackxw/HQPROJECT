
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: Authentication.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __AUTHENTICATION_H_INCLUDED__
#define __AUTHENTICATION_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include "AuthConnectSocket.h"
#include "_LoginDefine.h"
#include "_AuthDefine.h"

// Forword Declaration.
class CAuthorization;
class CAuthCallback;

//////////////////////////////////////////////////////////////////////

class CAuthenticationData
{
	friend class CAuthentication;
	
public : 
	CAuthenticationData() {}
	CAuthenticationData(const CAuthenticationData& rhs)
	{
		memcpy(&m_sLoginInfo, &rhs.m_sLoginInfo, sizeof(AUTH::sRequestLogin));
		m_sUserID.set(rhs.m_sUserID.orderKey, rhs.m_sUserID.arrayIndex);
		m_sGameServerID.set(rhs.m_sGameServerID.orderKey, rhs.m_sGameServerID.arrayIndex);
	}
	~CAuthenticationData() {}

public : 
	CAuthenticationData& operator=(const CAuthenticationData& rhs)
	{
		if(this == &rhs)
			return *this;

		memcpy(&m_sLoginInfo, &rhs.m_sLoginInfo, sizeof(AUTH::sRequestLogin));
		m_sUserID.set(rhs.m_sUserID.orderKey, rhs.m_sUserID.arrayIndex);
		m_sGameServerID.set(rhs.m_sGameServerID.orderKey, rhs.m_sGameServerID.arrayIndex);

		return *this;
	}
	void	Set(AUTH::sUniqueID sUserID, AUTH::sUniqueID sGameServerID, 
		AUTH::sRequestLogin sLoginInfo)
	{
		memcpy(&m_sUserID, &sUserID, sizeof(AUTH::sUniqueID));
		memcpy(&m_sGameServerID, &sGameServerID, sizeof(AUTH::sUniqueID));
		memcpy(&m_sLoginInfo, &sLoginInfo, sizeof(AUTH::sRequestLogin));
	}

private : 
	AUTH::sRequestLogin	m_sLoginInfo;
	AUTH::sUniqueID		m_sUserID;
	AUTH::sUniqueID		m_sGameServerID;
};

//////////////////////////////////////////////////////////////////////

class CAuthentication : public CAuthConnectSocket, public CParentCallback
{
public : 
	CAuthentication(CAuthorization* pAuthorization, CAuthCallback* pAuthCallback);
	~CAuthentication();

private : 
	static unsigned int WINAPI	Run_Process(void* vpThis);
	unsigned int Process();

private : 
	BOOL AuthenticationProcess();
	int	 GashReponseCheck(char* pOutStr);

public : 
	void StopThread();

protected : 
	BOOL SendAuthentication
		(
			char	szAccountID[LOGIN::ID_MAX_LENGTH],
			char	szPassword[LOGIN::PW_MAX_LENGTH],
			char	szIPAddress[16]
		);
	void GetPDU(const char* pPDU, SHORT& snResult, char* pOutputString);

	virtual void CallbackDisConnect();

public : 
	void PushCmdQueue(CAuthenticationData Data);
	CAuthenticationData PopCmdQueue();
	void ClearAllCmdQueue();
	int	 CmdQueueSize();

	void StartThread();
	void SetAuthenticationInfo(char* szServiceCode, char* szRegion, char* szLockFlag);

private : 
	BOOL			m_bAuthThreadRun;
	unsigned int	m_uiAuthThreadID;
	HANDLE			m_hAuthBreakEvent;
	HANDLE			m_hAuthExitEvent;

	private : 
	char			m_szServiceCode[7];
	char			m_szRegion[3];
	char			m_szLockFlag[2];

private : 
	// Authorization 참조용.
	CAuthorization*	m_pAuthorization;

private : 
	// Game server로 send 관련 클래스 포인터.
	CAuthCallback* volatile m_pAuthCallback;

private : 
	std::queue<CAuthenticationData>	m_CmdQueue;
};

//////////////////////////////////////////////////////////////////////

#endif	// __AUTHENTICATION_H_INCLUDED__

//////////////////////////////////////////////////////////////////////