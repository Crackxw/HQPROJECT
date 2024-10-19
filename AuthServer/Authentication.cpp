
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: Authentication.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "Authentication.h"
#include <assert.h>
#include <process.h>
#include "Authorization.h"
#include "AuthCallback.h"
#include "JOChinaAuthServerConfig.h"

//////////////////////////////////////////////////////////////////////

CAuthentication::CAuthentication(CAuthorization* pAuthorization, CAuthCallback* pAuthCallback) : 
CAuthConnectSocket(),
m_bAuthThreadRun(FALSE),
m_uiAuthThreadID(0),
m_hAuthBreakEvent(NULL), 
m_hAuthExitEvent(NULL), 
m_pAuthorization(pAuthorization),
m_pAuthCallback(pAuthCallback)
{
	memset(m_szServiceCode, 0, sizeof(m_szServiceCode));
	memset(m_szRegion, 0, sizeof(m_szRegion));
	memset(m_szLockFlag, 0, sizeof(m_szLockFlag));
	
	m_hAuthBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hAuthExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

	SetParent(static_cast<CParentCallback*>(this));
}

//////////////////////////////////////////////////////////////////////

CAuthentication::~CAuthentication()
{
	if(m_hAuthBreakEvent != NULL)
		CloseHandle(m_hAuthBreakEvent);
	if(m_hAuthExitEvent != NULL)
		CloseHandle(m_hAuthExitEvent);

	ClearAllCmdQueue();
}

//////////////////////////////////////////////////////////////////////

void CAuthentication::CallbackDisConnect()
{	
	ClearAllCmdQueue();
}

//////////////////////////////////////////////////////////////////////

unsigned int CAuthentication::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CAuthentication*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CAuthentication::Process()
{
	DWORD dwRetVal;	

	while(TRUE)
	{
		dwRetVal = WaitForSingleObject(m_hAuthBreakEvent, 50);

		if(dwRetVal == WSA_WAIT_EVENT_0)
		{
			break;
		}
		
		AuthenticationProcess();
	}

	SetEvent(m_hAuthExitEvent);	

	return 0;
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthentication::AuthenticationProcess()
{
	char szOutputString[512] = {0,};
	char szPacket[512] = {0,};
	int	 nPacketSize = 512;
	BYTE byRetVal = GASH_SERVER_UNKNOWN_FAIL;

	BOOL bRetVal = FALSE;
	DWORD dwDBAccount = 0;
	int		nResult = LOGIN::UNKNOWN_FAIL;
	SHORT	snResult = -1;

	int nCount = CmdQueueSize();
	
	if(nCount > 0)
	{
		CAuthenticationData AuthenData = PopCmdQueue();	
		
		if(Connect() == TRUE)
		{
			printf("Process Connect[Accounting]\n");
		}

		if(SendAuthentication(AuthenData.m_sLoginInfo.id, 
							  AuthenData.m_sLoginInfo.password, 
							  AuthenData.m_sLoginInfo.ip) == TRUE)
		{
			while(true)
			{
				byRetVal = Recv(szPacket, nPacketSize);
				if(byRetVal == GASH_SERVER_REPONSE_OK)
				{
					// Gash response message need parsing.
					// Check rootine need.

					GetPDU(szPacket, snResult, szOutputString);							
					if(snResult == -1)
					{
						m_pAuthCallback->CallbackSendResponseLogin
							(
								AuthenData.m_sGameServerID, 
								AuthenData.m_sUserID, 
								nResult, 
								dwDBAccount
							);

						bRetVal = FALSE;
						break;
					}
					// DB Login execution.
					// if DB Login ok and
					// Authorization command queue push callback function call.								
					else
					{
						nResult = GashReponseCheck(szOutputString);

												if(nResult == LOGIN::OK)
						{
							CAuthorizationData AuthorizationData;
							AuthorizationData.Set(AuthenData.m_sUserID, 
								AuthenData.m_sGameServerID, 
								AuthenData.m_sLoginInfo, dwDBAccount);

							m_pAuthorization->PushCmdQueue(AuthorizationData);

							bRetVal = TRUE;
							break;
						}
						else
						{
							m_pAuthCallback->CallbackSendResponseLogin
							(
								AuthenData.m_sGameServerID, 
								AuthenData.m_sUserID, 
								nResult, 
								dwDBAccount
							);

							bRetVal = FALSE;
							break;
						}												
					}						
				}
				else if(byRetVal == GASH_SERVER_RECV_READY_ENABLE)
				{
					continue;
				}
				else if(byRetVal == GASH_SERVER_REPONSE_TIMEOUT ||
						byRetVal == GASH_SERVER_UNKNOWN_FAIL)
				{
					// To user Gash server response timeout send.
					// callback function call.

					m_pAuthCallback->CallbackSendResponseLogin
						(
							AuthenData.m_sGameServerID, 
							AuthenData.m_sUserID, 
							LOGIN::UNKNOWN_FAIL, 
							dwDBAccount
						);

					if(byRetVal == GASH_SERVER_REPONSE_TIMEOUT)
					{
						CAuthSocket::DisConnect();
					}

					bRetVal = FALSE;
					break;
				}					
				else
				{
					bRetVal = FALSE;
					break;
				}
			}
		}
		else // Send failed.
		{
			m_LogWriter.PrintF("Gash Send Log.txt", 
			"Send Failed GameServerID %s , UserID = %s", 
			AuthenData.m_sGameServerID, AuthenData.m_sUserID);

			m_pAuthCallback->CallbackSendResponseLogin
				(
					AuthenData.m_sGameServerID, 
					AuthenData.m_sUserID, 
					LOGIN::UNKNOWN_FAIL, 
					dwDBAccount
				);

			bRetVal = FALSE;
		}			
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

void CAuthentication::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CAuthentication*>(this), 0, &m_uiAuthThreadID);

	if(hThread != NULL)
	{
		m_bAuthThreadRun = TRUE;
		CloseHandle(hThread);

		printf("Authentication process thread start. [IP : %s], [Port : %d]\n", m_szIP, m_wPort);
	}
	else
	{
		printf("Authentication process thread could not start. because thread create failed. [IP : %s], [Port : %d][Error Code : %d]\n", m_szIP, m_wPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CAuthentication::StopThread()
{
	if(m_bAuthThreadRun == FALSE) return;

	SetEvent(m_hAuthBreakEvent);
	m_bAuthThreadRun = FALSE;

	WaitForSingleObject(m_hAuthExitEvent, INFINITE);

	printf("Authentication process thread end...!![IP : %s], [Port : %d]\n", m_szIP, m_wPort);
}

//////////////////////////////////////////////////////////////////////

void CAuthentication::PushCmdQueue(CAuthenticationData Data)
{
	EnterQueueLock();
	m_CmdQueue.push(Data);
	LeaveQueueLock();
}

//////////////////////////////////////////////////////////////////////

CAuthenticationData CAuthentication::PopCmdQueue()
{
	CAuthenticationData Data;

	EnterQueueLock();
	Data = m_CmdQueue.front();
	m_CmdQueue.pop();
	LeaveQueueLock();

	return Data;
}

//////////////////////////////////////////////////////////////////////

void CAuthentication::ClearAllCmdQueue()
{
	EnterQueueLock();
	while(m_CmdQueue.empty() == false)
	{
		m_CmdQueue.pop();
	}
	LeaveQueueLock();
}

//////////////////////////////////////////////////////////////////////

int CAuthentication::CmdQueueSize()
{
	EnterQueueLock();
	int nSize = (int)m_CmdQueue.size();
	LeaveQueueLock();
	
	return nSize;
}

//////////////////////////////////////////////////////////////////////

void CAuthentication::SetAuthenticationInfo(char* szServiceCode, 
											char* szRegion, 
											char* szLockFlag)
{
	strcpy(m_szServiceCode, szServiceCode);
	strcpy(m_szRegion, szRegion);
	strcpy(m_szLockFlag, szLockFlag);

	m_LogWriter.PrintF("SetServiceInfo[AuthenticationInfo].Log", 
		"Service Code : %s, Region : %s, LockFlag : %s\n", 
		m_szServiceCode, m_szRegion, m_szLockFlag);
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthentication::SendAuthentication(char	szAccountID[LOGIN::ID_MAX_LENGTH], 
										 char	szPassword[LOGIN::PW_MAX_LENGTH], 										 
										 char	szIPAddress[16])
{
	char  szRealPacket[512];
	memset(szRealPacket, 0, sizeof(szRealPacket));
				
	int nLen = 0;

	nLen += sprintf(szRealPacket, "%s'", m_szServiceCode);
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szRegion);
	nLen += sprintf(szRealPacket+nLen, "%s'", szAccountID);
	nLen += sprintf(szRealPacket+nLen, "%s'", szPassword);
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szLockFlag);
	nLen += sprintf(szRealPacket+nLen, "%s'", szIPAddress);
	//nLen += sprintf(szRealPacket+nLen, "%s", "/r/n");

	return Send(szRealPacket, nLen);
}

//////////////////////////////////////////////////////////////////////
 
void CAuthentication::GetPDU(const char* pPDU, SHORT& snResult, char* pOutputString)
{
	char szTemp[2] = {0,};
	memcpy(szTemp, pPDU, 1);
	int nMove = 1;
	if(szTemp[0] == '-')
	{
		memcpy(szTemp, pPDU, 2);
		nMove = 2;
	}
	snResult = atoi(szTemp);
	strcpy(pOutputString, (char*)(pPDU + sizeof(char)+nMove));

	printf("Result : %d, OutputString : %s\n", snResult, pOutputString);
}

//////////////////////////////////////////////////////////////////////

int	 CAuthentication::GashReponseCheck(char* pOutStr)
{
	int nRetVal = LOGIN::UNKNOWN_FAIL;

	if(strcmp(pOutStr, "ServiceAccount_Authenticated") == 0)
	{
		nRetVal = LOGIN::OK;
	}
//	else if(strcmp(pOutStr, "Service_Not_Available") == 0)
//	{	
//		nRetVal = LOGIN::OK;
//	}
//	else if(strcmp(pOutStr, "Encode_Error") == 0)
//	{	
//		nRetVal = LOGIN::OK;
//	}
//	else if(strcmp(pOutStr, "Unexpected_Error") == 0)
//	{		
//		nRetVal = LOGIN::OK;
//	}
	else if(strcmp(pOutStr, "Invalid_Length") == 0)
	{
		nRetVal = LOGIN::INVALID_PASSWORD;
	}
	else if(strcmp(pOutStr, "Invalid_Character") == 0)
	{
		nRetVal = LOGIN::INVALID_PASSWORD;
	}
	else if(strcmp(pOutStr, "Wrong_ID_or_Password") == 0)
	{
		nRetVal = LOGIN::INVALID_PASSWORD;
	}
	else if(strcmp(pOutStr, "No_Such_ServiceAccount") == 0)
	{
		nRetVal = LOGIN::INVALID_ID;
	}
	else if(strcmp(pOutStr, "ServiceAccount_Locked") == 0)
	{
		nRetVal = LOGIN::LOCK_SERVICE_ACCOUNT; 
	}
//	else if(strcmp(pOutStr, "No_Such_Service") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}	
	
	return nRetVal;
}

//////////////////////////////////////////////////////////////////////