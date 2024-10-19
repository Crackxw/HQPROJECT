

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: Accounting.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


//////////////////////////////////////////////////////////////////////

#include "Accounting.h"
#include <assert.h>
#include <process.h>
#include "JOChinaAuthServerConfig.h"

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "../kojimodo/utility/MSSQL.h"

//////////////////////////////////////////////////////////////////////

CAccounting::CAccounting(CAuthCallback* pAuthCallback) : 
CAuthConnectSocket(),
m_bAuthThreadRun(FALSE),
m_uiAuthThreadID(0),
m_hAuthBreakEvent(NULL), 
m_hAuthExitEvent(NULL), 
m_pAuthCallback(pAuthCallback),
m_AccountDB(NULL)
{
	memset(m_szServiceCode, 0, sizeof(m_szServiceCode));
	memset(m_szRegion,		0, sizeof(m_szRegion));
	memset(m_szType,		0, sizeof(m_szType));

	m_hAuthBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hAuthExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_AccountDB = new CMSSQL();

	SetParent(static_cast<CParentCallback*>(this));
}

//////////////////////////////////////////////////////////////////////

CAccounting::~CAccounting()
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

void CAccounting::CallbackDisConnect()
{
	ClearAllCmdQueue();
}

//////////////////////////////////////////////////////////////////////

unsigned int CAccounting::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CAccounting*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CAccounting::Process()
{
	DWORD dwRetVal;

	while(TRUE)
	{
		dwRetVal = WaitForSingleObject(m_hAuthBreakEvent, 50);

		if(dwRetVal == WSA_WAIT_EVENT_0)
		{
			break;
		}

		AccountingProcess();
	}

	SetEvent(m_hAuthExitEvent);	

	return 0;
}

//////////////////////////////////////////////////////////////////////

BOOL CAccounting::AccountingProcess()
{
	char szOutputString[512] = {0,};
	char szPacket[512] = {0,};
	int	 nPacketSize = 512;	
	BYTE byRetVal = GASH_SERVER_UNKNOWN_FAIL;	
	int nResult = LOGIN::UNKNOWN_FAIL;
	BOOL bRetVal = FALSE;
	SHORT snResult = -1;
	FILE* fp;

	int nCount = CmdQueueSize();

	if(nCount > 0)
	{
		CAccountingData AccountingData = PopCmdQueue();

		if(Connect() == TRUE)
		{
			printf("Process Connect[Accounting]\n");
		}

		nResult = DB_Logout(AccountingData.m_sLogoutInfo.dbAccount);
		
		if(SendAccounting(AccountingData.m_sLogoutInfo.id, 
			AccountingData.m_sLogoutInfo.ip, "", "") == TRUE)
		{
			while(true)
			{
				byRetVal = Recv(szPacket, nPacketSize);

				if(byRetVal == GASH_SERVER_REPONSE_OK)
				{
					GetPDU(szPacket, snResult, szOutputString);

					if(snResult == -1)
					{
						bRetVal = FALSE;
						break; 
					}
					else
					{
						nResult = GashReponseCheck(szOutputString);
						if(nResult == LOGIN::OK)
						{
					//		nResult = DB_Logout(AccountingData.m_sLogoutInfo.dbAccount);
							bRetVal = TRUE;
							break;
						}
						else
						{
							bRetVal = FALSE;
							break;
						}
					}
				}
				else if(byRetVal == GASH_SERVER_RECV_READY_ENABLE)
				{
					continue;
				}
				else if(byRetVal == GASH_SERVER_REPONSE_TIMEOUT)
				{
					CAuthSocket::DisConnect();
					
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
		else
		{
			bRetVal = FALSE;
		}

		printf("-- Logout %s(%s):[%d]......End\n", 
			AccountingData.m_sLogoutInfo.id, AccountingData.m_sLogoutInfo.ip, nResult);
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

void CAccounting::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CAccounting*>(this), 0, &m_uiAuthThreadID);

	if(hThread != NULL)
	{
		m_bAuthThreadRun = TRUE;
		CloseHandle(hThread);
		printf("Accounting Process thread start. [IP : %s], [Port : %d]\n", m_szIP, m_wPort);
	}
	else
	{
		printf("Accounting Process thread could not start. because thread create failed. [IP : %s] [Port : %d] [Error code : %d]\n", m_szIP, m_wPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CAccounting::StopThread()
{
	if(m_bAuthThreadRun == FALSE) return;

	SetEvent(m_hAuthBreakEvent);
	m_bAuthThreadRun = FALSE;

	WaitForSingleObject(m_hAuthExitEvent, INFINITE);

	printf("Accounting process thread end...!![IP : %s] [Port : %d]\n", m_szIP, m_wPort);
}

//////////////////////////////////////////////////////////////////////

void CAccounting::PushCmdQueue(CAccountingData Data)
{
	EnterQueueLock();
	m_CmdQueue.push(Data);
	LeaveQueueLock();
}

//////////////////////////////////////////////////////////////////////

CAccountingData CAccounting::PopCmdQueue()
{
	CAccountingData Data;

	EnterQueueLock();
	Data = m_CmdQueue.front();
	m_CmdQueue.pop();
	LeaveQueueLock();

	return Data;
}

//////////////////////////////////////////////////////////////////////

void CAccounting::ClearAllCmdQueue()
{
	EnterQueueLock();
	while(m_CmdQueue.empty() == false)
	{
		m_CmdQueue.pop();
	}
	LeaveQueueLock();
}

//////////////////////////////////////////////////////////////////////

int CAccounting::CmdQueueSize()
{
	EnterQueueLock();
	int nSize = m_CmdQueue.size();
	LeaveQueueLock();
	
	return nSize;
}

//////////////////////////////////////////////////////////////////////

void CAccounting::SetAccountingInfo(char* szServiceCode, 
									char* szRegion, 
									char* szType)
{
	strcpy(m_szServiceCode, szServiceCode);
	strcpy(m_szRegion, szRegion);
	strcpy(m_szType, szType);

	m_LogWriter.PrintF("SetServiceInfo[Accounting].Log", 
		"Service Code : %s, Region : %s, Type : %s\n", 
		m_szServiceCode, m_szRegion, m_szType);
}

//////////////////////////////////////////////////////////////////////

BOOL CAccounting::DBConnect(char* pDBIP, 
							char* pDBName, 
							char* pID, 
							char* pPassword)
{
	return m_AccountDB->Connect(pDBIP, pDBName, pID, pPassword);
}

//////////////////////////////////////////////////////////////////////

BOOL CAccounting::SendAccounting(char	szAccountID[LOGIN::ID_MAX_LENGTH],								 
								 char	szLoginTime[20],
								 char	szLogoutTime[20], 
								 char	szIPAddress[16])
{	
	char szRealPacket[512];
	memset(szRealPacket, 0, sizeof(szRealPacket));

	int nLen = 0;

	nLen += sprintf(szRealPacket, "%s'", m_szServiceCode);
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szRegion);
	nLen += sprintf(szRealPacket+nLen, "%s'", szAccountID);
	nLen += sprintf(szRealPacket+nLen, "%s'", m_szType);
	nLen += sprintf(szRealPacket+nLen, "%s'", szLoginTime);
	nLen += sprintf(szRealPacket+nLen, "%s'", szLogoutTime);
	nLen += sprintf(szRealPacket+nLen, "%s'", szIPAddress);
	nLen += sprintf(szRealPacket+nLen, "%s", "\r\n");

	return Send(szRealPacket, nLen);
}

//////////////////////////////////////////////////////////////////////

void CAccounting::GetPDU(const char* pPDU, SHORT& snResult, char* pOutputString)
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

int CAccounting::DB_Logout(DWORD dbAccount)
{
	int nResult = LOGIN::UNKNOWN_FAIL;	

	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLogoutEnd %d", dbAccount) )
	{		
		if(m_AccountDB->DBResults() == SUCCEED)
		{
			m_AccountDB->DBBind(1, INTBIND,			(DBINT)0, (BYTE*)&nResult);

			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS );							
		}
		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();
	}

	// 상태 변화에 문제가 있을 경우 로그를 남긴다 
	if( nResult == LOGIN::FAIL_CHANGE_STATE )
	{
		m_LogWriter.PrintF("StateChangeError[Accounting].Log", 
			"SPHQ_OnLogoutEnd %d", dbAccount);
	}

	return nResult;
}

//////////////////////////////////////////////////////////////////////

int	 CAccounting::GashReponseCheck(char* pOutStr)
{
	int nRetVal = LOGIN::OK;

	if(strcmp(pOutStr, "Accounting_Request_Processing\r\n") == 0)
	{
		nRetVal = LOGIN::OK;
	}
//	else if(strcmp(pOutStr, "ServiceAccount_Not_Logint") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "Not_Such_Service") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "Service_Not_Available") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "Invalid_Time_Format") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "Invalid_Accounting_Type") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}
//	else if(strcmp(pOutStr, "Unexpected_Error") == 0)
//	{
//		nRetVal = LOGIN::UNKNOWN_FAIL;
//	}

	return nRetVal;
}

//////////////////////////////////////////////////////////////////////