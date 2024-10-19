

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: Accounting.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __ACCOUNTING_H_INCLUDED__
#define __ACCOUNTING_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include "AuthConnectSocket.h"
#include "_LoginDefine.h"
#include "_AuthDefine.h"


// Forword Declaration.
class CMSSQL;
class CAuthCallback;


//////////////////////////////////////////////////////////////////////

class CAccountingData
{
	friend class CAccounting;
	
public : 
	CAccountingData() {}
	CAccountingData(const CAccountingData& rhs)
	{
		memcpy(&m_sLogoutInfo, &rhs.m_sLogoutInfo, sizeof(AUTH::sRequestLogout));
		m_sUserID.set(rhs.m_sUserID.orderKey, rhs.m_sUserID.arrayIndex);
		m_sGameServerID.set(rhs.m_sGameServerID.orderKey, rhs.m_sGameServerID.arrayIndex);
	}
	~CAccountingData() {}

public : 
	CAccountingData& operator=(const CAccountingData& rhs)
	{
		if(this == &rhs)
			return *this;

		memcpy(&m_sLogoutInfo, &rhs.m_sLogoutInfo, sizeof(AUTH::sRequestLogout));
		m_sUserID.set(rhs.m_sUserID.orderKey, rhs.m_sUserID.arrayIndex);
		m_sGameServerID.set(rhs.m_sGameServerID.orderKey, rhs.m_sGameServerID.arrayIndex);

		return *this;
	}

	void	Set(AUTH::sUniqueID sUserID, AUTH::sUniqueID sGameServerID, 
		AUTH::sRequestLogout sLogoutInfo)
	{
		memcpy(&m_sUserID, &sUserID, sizeof(AUTH::sUniqueID));
		memcpy(&m_sGameServerID, &sGameServerID, sizeof(AUTH::sUniqueID));
		memcpy(&m_sLogoutInfo, &sLogoutInfo, sizeof(AUTH::sRequestLogout));
	}

private : 
	AUTH::sRequestLogout	m_sLogoutInfo;
	AUTH::sUniqueID			m_sUserID;
	AUTH::sUniqueID			m_sGameServerID;
};

//////////////////////////////////////////////////////////////////////

class CAccounting : public CAuthConnectSocket, public CParentCallback
{
public : 
	CAccounting(CAuthCallback* pAuthCallback);
	~CAccounting();

private : 
	static unsigned int WINAPI	Run_Process(void* vpThis);
	unsigned int Process();

private : 
	int	DB_Logout(DWORD dbAccount);
	int	GashReponseCheck(char* pOutStr);

	BOOL AccountingProcess();

public : 	
	void StopThread();

protected : 
	BOOL SendAccounting
		(
			char	szAccountID[LOGIN::ID_MAX_LENGTH],
			char	szLoginTime[20],
			char	szLogoutTime[20],
			char	szIPAddress[16]
		);

	void GetPDU(const char* pPDU, SHORT& snResult, char* pOutputString);
	virtual void CallbackDisConnect();
	
public : 
	void PushCmdQueue(CAccountingData Data);
	CAccountingData PopCmdQueue();
	void ClearAllCmdQueue();
	int	 CmdQueueSize();


	void StartThread();
	void SetAccountingInfo(char* szServiceCode, char* szRegion, char* szType);
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
	std::queue<CAccountingData>	m_CmdQueue;

private : 
	char			m_szServiceCode[7];
	char			m_szRegion[3];
	char			m_szType[2];

private : 
	CMSSQL*			m_AccountDB;
};

//////////////////////////////////////////////////////////////////////

#endif	// __ACCOUNTING_H_INCLUDED__

//////////////////////////////////////////////////////////////////////