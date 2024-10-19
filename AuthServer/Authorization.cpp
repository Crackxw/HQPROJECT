
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: Authorization.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "Authorization.h"
#include <assert.h>
#include <process.h>
#include "AuthCallback.h"
#include "JOChinaAuthServerConfig.h"

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "../kojimodo/utility/MSSQL.h"

//////////////////////////////////////////////////////////////////////

CAuthorization::CAuthorization(CAuthCallback* pAuthCallback) : 
CAuthConnectSocket(),
m_bAuthThreadRun(FALSE),
m_uiAuthThreadID(0),
m_hAuthBreakEvent(NULL), 
m_hAuthExitEvent(NULL), 
m_pAuthCallback(pAuthCallback),
m_AccountDB(NULL)
{
	memset(m_szServiceCode, 0, sizeof(m_szServiceCode));
	memset(m_szRegion, 0, sizeof(m_szRegion));
	memset(m_szAuthenticationFlag, 0, sizeof(m_szAuthenticationFlag));
	memset(m_szType, 0, sizeof(m_szType));
	memset(m_szDuplicateCheck, 0, sizeof(m_szDuplicateCheck));

	m_hAuthBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hAuthExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_AccountDB = new CMSSQL();

	SetParent(static_cast<CParentCallback*>(this));
}

//////////////////////////////////////////////////////////////////////

CAuthorization::~CAuthorization()
{
	if(m_hAuthBreakEvent != NULL)
		CloseHandle(m_hAuthBreakEvent);
	if(m_hAuthExitEvent != NULL)
		CloseHandle(m_hAuthExitEvent);

	ClearAllCmdQueue();
	
	if(m_AccountDB != NULL)
	{
		m_AccountDB->Dissconnect();
		
		delete m_AccountDB;
		m_AccountDB = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::CallbackDisConnect()
{	
	ClearAllCmdQueue();
}

//////////////////////////////////////////////////////////////////////

unsigned int CAuthorization::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CAuthorization*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CAuthorization::Process()
{
	DWORD dwRetVal;

	while(TRUE)
	{
		dwRetVal = WaitForSingleObject(m_hAuthBreakEvent, 50);		

		if(dwRetVal == WSA_WAIT_EVENT_0)
		{
			break;
		}

		AuthorizationProcess();
	}

	SetEvent(m_hAuthExitEvent);	

	return 0;
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthorization::AuthorizationProcess()
{
	int nCount = CmdQueueSize();

	char szOutputString[512] = {0};
	char szPacket[512] = {0,};
	int	 nPacketSize = 512;
	BYTE byRetVal = GASH_SERVER_UNKNOWN_FAIL;
	BOOL bRetVal = FALSE;
	int  nResult = LOGIN::UNKNOWN_FAIL;
	SHORT snResult = -1;
	DWORD dwDBAccount = 0;

	if(nCount > 0)
	{
		CAuthorizationData AuthorData = PopCmdQueue();

		if(Connect() == TRUE)
		{
			printf("Process Connect[Accounting]\n");
		}

		if(SendAuthorization(AuthorData.m_sLoginInfo.id, 
			AuthorData.m_sLoginInfo.password, AuthorData.m_sLoginInfo.ip) == TRUE)
		{
			//m_LogWriter.PrintF("Gash Log.txt",
			//"Sending Success !! id = %s \n",AuthorData.m_sLoginInfo.id);

			while(true)
			{
				byRetVal = Recv(szPacket, nPacketSize);
				if(byRetVal == GASH_SERVER_REPONSE_OK)
				{
					//m_LogWriter.PrintF("Gash Log.txt",
					//"GASH_SERVER_REPONSE_OK !! id = %s \n",AuthorData.m_sLoginInfo.id);

					GetPDU(szPacket, snResult, szOutputString);

					//m_LogWriter.PrintF("Gash Log.txt",
					//" snResult = %d, szOutputString = %s \n",snResult, szOutputString);
					
					if(snResult == -1)
					{
						m_pAuthCallback->CallbackSendResponseLogin
							(
								AuthorData.m_sGameServerID, 
								AuthorData.m_sUserID, 
								nResult, 
								dwDBAccount
							);
 
						bRetVal = TRUE;
						break;
					}
					else
					{
						nResult = GashReponseCheck(szOutputString);

						if(nResult == LOGIN::OK)
						{
					//		m_LogWriter.PrintF("Gash Log.txt",
					//		" LOGIN::OK \n");

							nResult = RequestDBLogin(AuthorData.m_sLoginInfo.id, 
								AuthorData.m_sLoginInfo.password, AuthorData.m_sLoginInfo.ip, 
								AuthorData.m_sLoginInfo.world_id, &dwDBAccount);

							m_pAuthCallback->CallbackSendResponseLogin
								(
									AuthorData.m_sGameServerID, 
									AuthorData.m_sUserID,  
									nResult, 
									dwDBAccount
								);
							bRetVal = TRUE;
							break;
						}
						else
						{
					//		m_LogWriter.PrintF("Gash Log.txt",
					//		" LOGIN::NO \n");

							m_pAuthCallback->CallbackSendResponseLogin
								(
									AuthorData.m_sGameServerID, 
									AuthorData.m_sUserID, 
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
				//	m_LogWriter.PrintF("Gash Log.txt",
				//	"GASH_SERVER_RECV_READY_ENABLE !! id = %s \n",AuthorData.m_sLoginInfo.id);

					continue;
				}
				else if(byRetVal == GASH_SERVER_REPONSE_TIMEOUT ||
						byRetVal == GASH_SERVER_UNKNOWN_FAIL)
				{
					m_pAuthCallback->CallbackSendResponseLogin
						(
							AuthorData.m_sGameServerID, 
							AuthorData.m_sUserID, 
							LOGIN::UNKNOWN_FAIL, 
							dwDBAccount
						);
				//	m_LogWriter.PrintF("Gash Log.txt",
				//	" GASH_SERVER_UNKNOWN_FAIL1 !! id = %s \n",AuthorData.m_sLoginInfo.id);

					if(byRetVal == GASH_SERVER_REPONSE_TIMEOUT)
					{
				//		m_LogWriter.PrintF("Gash Log.txt",
				//		" GASH_SERVER_REPONSE_TIMEOUT !! id = %s \n",AuthorData.m_sLoginInfo.id);
						CAuthSocket::DisConnect();
					}

					bRetVal = FALSE;
					break;
				}				
				else
				{
				//	m_LogWriter.PrintF("Gash Log.txt",
				//	" GASH_SERVER_UNKNOWN_FAIL2 !! id = %s \n",AuthorData.m_sLoginInfo.id);

					bRetVal = FALSE;
					break;
				}
			}
		}
		else
		{
		//	m_LogWriter.PrintF("Gash Log.txt",
		//	"Sending Failed!! id = %s \n",AuthorData.m_sLoginInfo.id);

			m_pAuthCallback->CallbackSendResponseLogin
				(
					AuthorData.m_sGameServerID, 
					AuthorData.m_sUserID, 
					LOGIN::UNKNOWN_FAIL, 
					dwDBAccount
				);
			bRetVal = FALSE;
		}
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CAuthorization*>(this), 0, &m_uiAuthThreadID);

	if(hThread != NULL)
	{
		m_bAuthThreadRun = TRUE;
		CloseHandle(hThread);
		printf("Authorization Process thread start. [IP : %s] [Port : %d]\n", m_szIP, m_wPort);
	}
	else
	{
		printf("Authorization Process thread could not start. because thread create failed. [IP : %s] [Port : %d] [Error Code : %d]\n", m_szIP, m_wPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::StopThread()
{
	if(m_bAuthThreadRun == FALSE) return;

	SetEvent(m_hAuthBreakEvent);
	m_bAuthThreadRun = FALSE;

	WaitForSingleObject(m_hAuthExitEvent, INFINITE);

	printf("Authorization process thread end...!![IP : %s] [Port : %d]\n", m_szIP, m_wPort);
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::PushCmdQueue(CAuthorizationData Data)
{
	EnterQueueLock();
	m_CmdQueue.push(Data);
	LeaveQueueLock();
}

//////////////////////////////////////////////////////////////////////

CAuthorizationData CAuthorization::PopCmdQueue()
{
	CAuthorizationData Data;

	EnterQueueLock();
	Data = m_CmdQueue.front();
	m_CmdQueue.pop();
	LeaveQueueLock();

	return Data;
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::ClearAllCmdQueue()
{
	EnterQueueLock();
	while(m_CmdQueue.empty() == false)
	{
		m_CmdQueue.pop();
	}
	LeaveQueueLock();
}

//////////////////////////////////////////////////////////////////////

int CAuthorization::CmdQueueSize()
{
	EnterQueueLock();
	int nSize = (int)m_CmdQueue.size();
	LeaveQueueLock();

	return nSize;
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::SetAuthorizationInfo(char* szServiceCode, 
										  char* szRegion, 
										  char* szAuthenticatonFlag, 
										  char* szType, 
										  char* szDuplicateCheck)
{
	strncpy(m_szServiceCode, szServiceCode, strlen(szServiceCode)+1);
	strcpy(m_szRegion, szRegion);
	strcpy(m_szAuthenticationFlag, szAuthenticatonFlag);
	strcpy(m_szType, szType);
	strcpy(m_szDuplicateCheck, szDuplicateCheck);

	m_LogWriter.PrintF("SetServiceInfo[Authorization].Log", 
		"Service Code : %s, Region : %s, AuthenticationFlag : %s, Type : %s, DuplicateCheck : %s\n", 
		m_szServiceCode, m_szRegion, m_szAuthenticationFlag, m_szType, m_szDuplicateCheck);
}

//////////////////////////////////////////////////////////////////////

void CAuthorization::GetPDU(const char* pPDU, SHORT& snResult, char* pOutputString)
{
	char szTemp[2] = {0,};
	memcpy(szTemp, pPDU, 1); 
	int nMove = 1;
	int nLen  = 0;
	if(szTemp[0] == '-')
	{
		memcpy(szTemp, pPDU, 2);
		nMove = 2;
	}
	snResult = atoi(szTemp);
	strcpy(pOutputString, (char*)(pPDU + sizeof(char)+nMove));
	//nLen += sprintf(pOutputString, "%s",(char*)(pPDU + sizeof(char)+nMove));
	//sprintf(pOutputString + nLen, "%s","\r\n");

	printf("Result : %d, OutputString : %s\n", snResult, pOutputString);
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthorization::DBConnect(char* pDBIP, 
							   char* pDBName, 
							   char* pID, 
							   char* pPassword)
{
	return m_AccountDB->Connect(pDBIP, pDBName, pID, pPassword);
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthorization::SendAuthorization(char	szAccountID[LOGIN::ID_MAX_LENGTH],
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
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szAuthenticationFlag);
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szType);
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szDuplicateCheck);
	nLen += sprintf(szRealPacket+nLen, "%s'", szIPAddress);
	nLen += sprintf(szRealPacket+nLen, "%s", "\r\n");


	return Send(szRealPacket, nLen);
}

//////////////////////////////////////////////////////////////////////

int	 CAuthorization::GashReponseCheck(char* pOutStr)
{
	int nRetVal = LOGIN::UNKNOWN_FAIL;

	if(strcmp(pOutStr, "ServiceAccount_Authorized\r\n") == 0)
	{
		nRetVal = LOGIN::OK;
	} 
	else if(strcmp(pOutStr, "ServiceAccount_Authorized_Updated\r\n") == 0)
	{
		nRetVal = LOGIN::OK;
	}
	else if(strcmp(pOutStr, "ServiceAccount_Waiting\r\n") == 0)
	{
		nRetVal = LOGIN::OK;
	}
	else if(strcmp(pOutStr, "No_Such_ServiceAccount\r\n") == 0)
	{
		nRetVal = LOGIN::INVALID_ID;
	}
//	else if(strcmp(pOutStr, "Invalid_Authorization_Type") == 0)
//	{
//		nRetVal = LOGIN::OK;
//	}
//	else if(strcmp(pOutStr, "Unexpected_Error") == 0)
//	{
//		nRetVal = LOGIN::OK;
//	}
	else if(strcmp(pOutStr, "Invalid_Length\r\n") == 0)
	{
		nRetVal = LOGIN::INVALID_PASSWORD;
	}
	else if(strcmp(pOutStr, "Invalid_Character\r\n") == 0)
	{
		nRetVal = LOGIN::INVALID_PASSWORD;
	}
//	else if(strcmp(pOutStr, "Encode_Error") == 0)
//	{
//		nRetVal = LOGIN::OK;
//	}
	else if(strcmp(pOutStr, "Wrong_ID_or_Password\r\n") == 0)
	{
		nRetVal = LOGIN::INVALID_PASSWORD;
	}
	else if(strcmp(pOutStr, "ServiceAccount_Locked\r\n") == 0)
	{
		nRetVal = LOGIN::LOCK_SERVICE_ACCOUNT;
	}
//	else if(strcmp(pOutStr, "Service_Not_Available") == 0)
//	{
//		nRetVal = LOGIN::OK;
//	}
	else if(strcmp(pOutStr, "ServiceAccount_Already_Login\r\n") == 0)
	{
		nRetVal = LOGIN::ALREADY_LOGIN;
	}
//	else if(strcmp(pOutStr, "ServiceAccount_Already_Waiting") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "Time_Out") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "No_AuthorizationResult") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}	
	
	return nRetVal;
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

int CAuthorization::RequestDBLogin(const char* pAccountID, 
								    const char* pPassword, 
								    const char* pIP, 
								    const int nWorldID,
									DWORD* dwDBAccount)
{	
	DWORD	dwIDNumber	= 0;
	
	int		nResult		= LOGIN::UNKNOWN_FAIL;
	//-- check account and state change(to LOGIN_START) 
	nResult = DB_Login(pAccountID, pPassword, pIP, nWorldID, dwIDNumber, dwDBAccount);	

	return nResult;
}

int CAuthorization::DB_Login(const char* pAccountID, 
					    const char* pPassword, 
					    const char* pIP, 
					    WORD wWorldID, 
					    DWORD dwIDNumber, 
					    DWORD* pdwAccount)
{
	int nResult = LOGIN::UNKNOWN_FAIL;

	// 사내 서버용
//	if( m_AccountDB->SQLRun(
//		"EXEC SPHQ_OnLoginStart '%s','%s','%s', %d, %d", 
//		pAccountID, pPassword, pIP, wWorldID, pdwAccount) ) 

    // 본 섭용.
	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLoginStart '%s','%s','%d','%s'", 
		pAccountID, pPassword, wWorldID, pIP) )
	{
		if(m_AccountDB->DBResults() == SUCCEED)
		{
			m_AccountDB->DBBind(1, INTBIND,			(DBINT)0, (BYTE*)&nResult);			
			m_AccountDB->DBBind(2, INTBIND,		    (DBINT)0, (BYTE*)pdwAccount);

			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS );	
		}

		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();
	}


	// 상태 변화에 문제가 있을 경우 로그를 남긴다 
	if( nResult == LOGIN::FAIL_CHANGE_STATE )
	{
		m_LogWriter.PrintF("StateChangeError[Authentication].Log", 
			"SPHQ_OnLoginStart %s", pAccountID);
	}
	
	return nResult;
}