
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: GashClient.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "GashClient.h"
#include "JOChinaAuthServerConfig.h"
#include <assert.h>
#include <process.h>

//////////////////////////////////////////////////////////////////////

CGashClient::CGashClient(CAuthCallback* pAuthCallback) :
m_pAuthentication(NULL),
m_pAuthorization(NULL),
m_pAccounting(NULL),
m_bInited(FALSE)
{	
	Initialize(pAuthCallback);
}

//////////////////////////////////////////////////////////////////////

CGashClient::~CGashClient()
{
	Finialize();
}

void CGashClient::LoadConfigFile()
{
	assert(m_bInited != FALSE);

	CJOChinaAuthServerConfig Config;
	Config.SetConfigFileName(GERSANG_GASH_CLIENT_INFO_FILE);

	DWORD dwTimeoutMilliSecond = Config.GetSocketConnectTimeoutSecond()*1000;
//	m_pAuthentication->SetTimeout(dwTimeoutMilliSecond);
	m_pAuthorization->SetTimeout(dwTimeoutMilliSecond);
	m_pAccounting->SetTimeout(dwTimeoutMilliSecond);

	char szIP[32] = {0,};
	WORD wPort = 0;
	char szServiceCode[6] = {0,};
	char szRegion[2] = {0,};
	char szLockFlag[1] = {0,};
	char szAuthenticationFlag[1] = {0,};
	char szType[1] = {0,};
	char szDuplicate[1] = {0,};

//	strcpy(szIP, Config.Authentication_IP());
//	wPort = Config.Authentication_Port();
//	m_pAuthentication->Create(szIP, wPort);

	strcpy(szIP, Config.Authorization_IP());
	wPort = Config.Authorization_Port();
	m_pAuthorization->Create(szIP, wPort);

	strcpy(szIP, Config.Accounting_IP());
	wPort = Config.Accounting_Port();
	m_pAccounting->Create(szIP, wPort);
	

	strcpy(szServiceCode, Config.Authentication_ServiceCode());
	strcpy(szRegion, Config.Authentication_Region());
	strcpy(szLockFlag, Config.Authentication_LockFlag());

	SetAuthenticationInfo
		(
			szServiceCode, 
			szRegion, 
			szLockFlag
		);

	strcpy(szServiceCode, Config.Authorization_ServiceCode());
	strcpy(szRegion, Config.Authorization_Region());
	strcpy(szAuthenticationFlag, Config.Authorization_AuthenticationFlag());
	strcpy(szType, Config.Authorization_Type());
	strcpy(szDuplicate, Config.Authorization_DuplicateCheck());

	SetAuthorizationInfo
		(
			szServiceCode, 
			szRegion, 
			szAuthenticationFlag, 
			szType, 
			szDuplicate
		);

	strcpy(szServiceCode, Config.Accounting_ServiceCode());
	strcpy(szRegion, Config.Accounting_Region());
	strcpy(szType, Config.Accounting_Type());

	SetAccountingInfo
		(
			szServiceCode, 
			szRegion, 
			szType
		);
}

//////////////////////////////////////////////////////////////////////
// 
void CGashClient::Initialize(CAuthCallback* pAuthCallback)
{
	assert(m_bInited == FALSE);
	if(m_bInited == TRUE) return;	

	m_pAuthorization = new CAuthorization(pAuthCallback);
	assert(m_pAuthorization != NULL);

//	m_pAuthentication = new CAuthentication(m_pAuthorization, pAuthCallback);
//	assert(m_pAuthentication != NULL);

	m_pAccounting = new CAccounting(pAuthCallback);
	assert(m_pAccounting != NULL);

	m_bInited = TRUE;
	
	LoadConfigFile();
	
	AllStartConnectThread();
	AllStartProcessThread();
}

//////////////////////////////////////////////////////////////////////

void CGashClient::Finialize()
{
	AllStopConnectThread();
	AllStopProcessThread();

//	if(m_pAuthentication != NULL)
//	{
//		delete m_pAuthentication; 
//		m_pAuthentication = NULL;
//	}
	if(m_pAuthorization != NULL)
	{
		delete m_pAuthorization;
		m_pAuthorization = NULL;
	}
	if(m_pAccounting != NULL)
	{
		delete m_pAccounting;
		m_pAccounting = NULL;
	}
	m_bInited = FALSE;
}

//////////////////////////////////////////////////////////////////////

BOOL CGashClient::DBConnect(char* pDBIP, 
							char* pDBName, 
							char* pID, 
							char* pPassword)
{	
	assert(m_bInited != FALSE);

	BOOL bRetVal = TRUE;
	
	bRetVal &= m_pAuthorization->DBConnect(pDBIP, pDBName, pID, pPassword);
	bRetVal &= m_pAccounting->DBConnect(pDBIP, pDBName, pID, pPassword);
	
	if(bRetVal == TRUE)
	{	
		m_LogWriter.PrintF("Account DB.Log","DB Connect succeed!! [DB IP : %s][DB Catalog : %s][ID : %s][Password : %s]\n", 
		pDBIP, pDBName, pID, pPassword);
	}
	else
	{
		m_LogWriter.PrintF("Account DB.Log","DB Connect failed!! [DB IP : %s][DB Catalog : %s][ID : %s][Password : %s]\n", 
		pDBIP, pDBName, pID, pPassword);
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

void CGashClient::AllStartProcessThread()
{
	assert(m_bInited != FALSE);

//	m_pAuthentication->StartThread();
	m_pAuthorization->StartThread();
	m_pAccounting->StartThread();
}

//////////////////////////////////////////////////////////////////////

void CGashClient::AllStopProcessThread()
{
	assert(m_bInited != FALSE);

//	m_pAuthentication->StopThread();
	m_pAuthorization->StopThread();
	m_pAccounting->StopThread();
}

//////////////////////////////////////////////////////////////////////

void CGashClient::AllStartConnectThread()
{
	assert(m_bInited != FALSE);

//	m_pAuthentication->StartConnectThread();
	m_pAuthorization->StartConnectThread();
	m_pAccounting->StartConnectThread();
}

//////////////////////////////////////////////////////////////////////

void CGashClient::AllStopConnectThread()
{
	assert(m_bInited != FALSE);
	
//	m_pAuthentication->StopConnectThread();
	m_pAuthorization->StopConnectThread();
	m_pAccounting->StopConnectThread();
}

//////////////////////////////////////////////////////////////////////

void CGashClient::CreateGash(char szIP[16], 
							 WORD wPort)
{
	assert(m_bInited != FALSE);
	
//	m_pAuthentication->Create(szIP, wPort);
	m_pAuthorization->Create(szIP, wPort);
	m_pAccounting->Create(szIP, wPort);
}

//////////////////////////////////////////////////////////////////////

void CGashClient::PushCmdQueueAuthentication(WORK::sPacketHeader* pHeader, 
											 AUTH::sRequestLogin* pData)
{
//	assert(m_bInited != FALSE);

//	CAuthenticationData Data;
//	Data.Set(pHeader->userUniqID, pHeader->gameServerUniqID, *pData);

//	m_pAuthentication->PushCmdQueue(Data);
}

//////////////////////////////////////////////////////////////////////

void CGashClient::PushCmdQueueAuthorization(WORK::sPacketHeader* pHeader, 
											AUTH::sRequestLogin* pData)
{
	assert(m_bInited != FALSE);

	CAuthorizationData Data;
	Data.Set(pHeader->userUniqID, pHeader->gameServerUniqID, *pData);

	m_pAuthorization->PushCmdQueue(Data);
}

//////////////////////////////////////////////////////////////////////

void CGashClient::PushCmdQueueAccounting(WORK::sPacketHeader* pHeader, 
										 AUTH::sRequestLogout* pData)
{
	assert(m_bInited != FALSE);

	CAccountingData Data;
	Data.Set(pHeader->userUniqID, pHeader->gameServerUniqID, *pData);

	m_pAccounting->PushCmdQueue(Data);
}

//////////////////////////////////////////////////////////////////////

CAuthentication* CGashClient::GetAuthentication() const
{
	assert(m_bInited != FALSE);

	return m_pAuthentication; 
}

//////////////////////////////////////////////////////////////////////

CAuthorization*	CGashClient::GetAuthorization() const
{ 
	assert(m_bInited != FALSE);

	return m_pAuthorization; 
}

//////////////////////////////////////////////////////////////////////

CAccounting* CGashClient::GetAccounting() const
{
	assert(m_bInited != FALSE);

	return m_pAccounting;
}

//////////////////////////////////////////////////////////////////////

void CGashClient::SetAuthenticationInfo(char* szServiceCode, 
										char* szRegion, 
										char* szLockFlag)
{
//	assert(m_bInited != FALSE);

//	m_pAuthentication->SetAuthenticationInfo(szServiceCode, szRegion, szLockFlag);
}

//////////////////////////////////////////////////////////////////////

void CGashClient::SetAuthorizationInfo(char* szServiceCode, 
									   char* szRegion, 
									   char* szAuthenticatonFlag, 
									   char* szType, 
									   char* szDuplicateCheck)
{
	assert(m_bInited != FALSE);

	m_pAuthorization->SetAuthorizationInfo(szServiceCode, szRegion, szAuthenticatonFlag, szType, szDuplicateCheck);
}

//////////////////////////////////////////////////////////////////////

void CGashClient::SetAccountingInfo(char* szServiceCode, 
									char* szRegion, 
									char* szType)
{
	assert(m_bInited != FALSE);

	m_pAccounting->SetAccountingInfo(szServiceCode, szRegion, szType);
}

//////////////////////////////////////////////////////////////////////