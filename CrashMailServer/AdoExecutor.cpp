

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AdoExecutor.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "AdoExecutor.h"

#include <assert.h>
#include "Defines.h"
#include "JOCrashServerConfig.h"
#include "CrashToolServer.h"
#include "..\\JOCommon\\AdoRecordset.h"
#include "..\\JOCommon\\AdoCommand.h"
#include "..\\JOCommon\\Defines.h"

//////////////////////////////////////////////////////////////////////

CAdoExecutor::CAdoExecutor() : 
m_pAdoConnection(NULL), 
m_bInit(FALSE)
{
	Initialize();
}

//////////////////////////////////////////////////////////////////////

CAdoExecutor::CAdoExecutor(CCrashToolServer* pServer) : 
m_pServer(pServer),
m_pAdoConnection(NULL), 
m_bInit(FALSE)
{
	Initialize();
}

//////////////////////////////////////////////////////////////////////

CAdoExecutor::~CAdoExecutor()
{
	Finalize();
}

//////////////////////////////////////////////////////////////////////

void CAdoExecutor::Initialize()
{
	if(m_bInit == TRUE)
		return;

	CoInitialize(NULL);

	m_pAdoConnection = new CAdoConnection;

	CJOCrashServerConfig Config;

	Config.SetConfigFileName(GERSANG_CRASH_SERVER_INFO_FILE);

	strcpy(m_szAdoConnectionString, Config.CrashDBConnectionString());
}

//////////////////////////////////////////////////////////////////////

void CAdoExecutor::Finalize()
{

	if(m_bInit == FALSE)
		return;

	delete m_pAdoConnection;
	m_pAdoConnection = NULL;

	CoUninitialize();
}

//////////////////////////////////////////////////////////////////////

BOOL CAdoExecutor::Connect()
{
	BOOL bRetVal = FALSE;

	if(m_pAdoConnection->Open(m_szAdoConnectionString) == ADO_CONNECT_SUCCEED)
	{
		bRetVal = TRUE;
		printf("Crash DB Connect succeed!!!\n");
	}
	else
	{
		bRetVal = FALSE;
		m_LogWriter.PrintF("Error_ADO DB Connect.Log", 
			"DB Connect failed\n Connection String = %s", m_szAdoConnectionString);
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

BOOL CAdoExecutor::Execution(DWORD dwType, void* pMsg, XClient* pClient /* = NULL*/)
{
	BOOL bRetVal = FALSE;

	switch(dwType)
	{
		case ADO_CRASH_USER_INFO_SAVE : 
			{
				bRetVal = CrashUserInfoSave(pMsg);
			}
			break;
			
		case ADO_CRASH_TOOL_SEARCH_INFO : 
			{
				bRetVal = OnCrashToolSearchInfo(pMsg, pClient);
			}
			break;
		case ADO_CRASH_TOOL_STATISTICS_INFO :
			{
				bRetVal = CrashToolStatisticsInfo(pMsg, pClient);
				break;
			}
		default : 
			{
				bRetVal = FALSE;
			}
			break;
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

SHORT CAdoExecutor::OnCrashToolSearchInfo(void* pMsg, XClient* pClient)
{
	ON_P_REQ_SEARCHCRASHINFO	pInfo = (ON_P_REQ_SEARCHCRASHINFO)pMsg;

	if(pInfo->byAllField == 1)
		return CrashToolSearchInfo(pClient);
	else if(pInfo->byAllField == 0)
		return CrashToolSearchInfo(pInfo, pClient);

	return 0;
}

//////////////////////////////////////////////////////////////////////

SHORT CAdoExecutor::CrashUserInfoSave(void* pMsg)
{
	ON_P_REQ_CRASHINFO	pInfo = (ON_P_REQ_CRASHINFO)pMsg;
	SHORT		snResult = 0;
	char		szCrashAddress[16]	= {0,};
	char		szLineAddress[16]	= {0,};
	
	sprintf(szCrashAddress, "%X", pInfo->nCrashAddress);
	sprintf(szLineAddress, "%X", pInfo->nLineAddress);

	if(m_pAdoConnection->IsOpen() == false)
	{
		if(Connect() == FALSE)
		{
			return snResult;
		}
	}

	CAdoCommand		cAdoCommand;	

	ADO_BEGIN()
		cAdoCommand.Init(m_pAdoConnection->GetActiveConnection(), "SP_CrashUserInfoSave");
		
		cAdoCommand.SetParameterValue(1, pInfo->nCrashType);
		cAdoCommand.SetParameterValue(2, pInfo->szOSType);
		cAdoCommand.SetParameterValue(3, pInfo->szProcessType);
		cAdoCommand.SetParameterValue(4, pInfo->szIP);
		cAdoCommand.SetParameterValue(5, pInfo->nProcessNumber);
		cAdoCommand.SetParameterValue(6, pInfo->nProcessLevel);
		cAdoCommand.SetParameterValue(7, pInfo->nPageSize);
		cAdoCommand.SetParameterValue(8, szCrashAddress);
		cAdoCommand.SetParameterValue(9, szLineAddress);
		cAdoCommand.SetParameterValue(10, pInfo->sTime.nYear);
		cAdoCommand.SetParameterValue(11, pInfo->sTime.nMonth);
		cAdoCommand.SetParameterValue(12, pInfo->sTime.nDay);
		cAdoCommand.SetParameterValue(13, pInfo->sTime.nHour);
		cAdoCommand.SetParameterValue(14, pInfo->sTime.nMinute);
		cAdoCommand.SetParameterValue(15, pInfo->sTime.nSecond);
		cAdoCommand.SetParameterValue(16, pInfo->eTime.nYear);
		cAdoCommand.SetParameterValue(17, pInfo->eTime.nMonth);
		cAdoCommand.SetParameterValue(18, pInfo->eTime.nDay);
		cAdoCommand.SetParameterValue(19, pInfo->eTime.nHour);
		cAdoCommand.SetParameterValue(20, pInfo->eTime.nMinute);
		cAdoCommand.SetParameterValue(21, pInfo->eTime.nSecond);
		cAdoCommand.SetParameterValue(22, pInfo->szNationCode);
		cAdoCommand.SetParameterValue(23, pInfo->szSupposition);
		cAdoCommand.Execute(NULL, NULL, adCmdStoredProc);
		snResult = cAdoCommand.GetParameterValue(0);
	ADO_ON_ERROR(e)
		OnADOError(e, "SP_CrashUserInfoSave");
	ADO_END()

	return snResult;
}

//////////////////////////////////////////////////////////////////////

SHORT CAdoExecutor::CrashToolSearchInfo(XClient* pClient)
{	
	SHORT		snResult = 0;	
	bool		bIsEof = false;	

	if(m_pAdoConnection->IsOpen() == false)
	{
		if(Connect() == FALSE)
		{
			return snResult;
		}
	}

	CAdoCommand		cAdoCommand;
	CAdoRecordset   cAdoRecordset;

	ON_RESP_SEARCHCRASHINFO			sCrashInfo[CRASH_MAX_LOAD_NUMBER];
	ON_RESP_SEARCHCRASHINFO_LIST	sCrashList;
	int	nCount = 0;
	memset(sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO)*CRASH_MAX_LOAD_NUMBER);
	memset(&sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO_LIST));

	ADO_BEGIN()
		cAdoCommand.Init(m_pAdoConnection->GetActiveConnection(), "SP_GetCrashInfo");
		cAdoRecordset.SetRecordset(cAdoCommand.Execute(NULL, NULL, adCmdStoredProc));

		while(cAdoRecordset.IsEof() == false)
		{
			sCrashInfo[nCount].dwID			= (LONG)cAdoRecordset.GetFieldValue("id");
			sCrashInfo[nCount].nCrashType	= cAdoRecordset.GetFieldValue("CrashType");
			strcpy(sCrashInfo[nCount].szCrashContents, (_bstr_t)cAdoRecordset.GetFieldValue("Contents"));
			strcpy(sCrashInfo[nCount].szOSType, (_bstr_t)cAdoRecordset.GetFieldValue("OSType"));
			strcpy(sCrashInfo[nCount].szProcessType, (_bstr_t)cAdoRecordset.GetFieldValue("ProcessType"));
			strcpy(sCrashInfo[nCount].szIP, (_bstr_t)cAdoRecordset.GetFieldValue("IP"));
			sCrashInfo[nCount].nProcessNumber = cAdoRecordset.GetFieldValue("ProcessNumber");
			sCrashInfo[nCount].nProcessLevel = cAdoRecordset.GetFieldValue("ProcessLevel");
			sCrashInfo[nCount].nPageSize = cAdoRecordset.GetFieldValue("PageSize");
			strcpy(sCrashInfo[nCount].szsTime, (_bstr_t)cAdoRecordset.GetFieldValue("StartTime"));
			strcpy(sCrashInfo[nCount].szeTime, (_bstr_t)cAdoRecordset.GetFieldValue("EndTime"));
			strcpy(sCrashInfo[nCount].szCrashAddress, (_bstr_t)cAdoRecordset.GetFieldValue("CrashAddress"));
			strcpy(sCrashInfo[nCount].szLineAddress, (_bstr_t)cAdoRecordset.GetFieldValue("LineAddress"));
			strcpy(sCrashInfo[nCount].szNationCode, (_bstr_t)cAdoRecordset.GetFieldValue("NationCode"));
			strcpy(sCrashInfo[nCount].szSupposition, (_bstr_t)cAdoRecordset.GetFieldValue("Supposition"));

			cAdoRecordset.MoveNext();

			////////////////////////////////////////////////////////////////

			bIsEof = cAdoRecordset.IsEof();
			if(++nCount == CRASH_MAX_LOAD_NUMBER || bIsEof == true)
			{
				if(bIsEof == true)
				{
					sCrashList.byResult		= 1;
					sCrashList.nCount		= nCount;
					sCrashList.byLastList	= 1;
					
					m_pServer->SendCrashInfoList(&sCrashList, 
						sCrashInfo, nCount, pClient);

					Sleep(100);

					break;
				}
				else
				{
					sCrashList.byResult		= 1;
					sCrashList.nCount		= nCount;
					sCrashList.byLastList	= 0;
					
					m_pServer->SendCrashInfoList(&sCrashList, 
						sCrashInfo, nCount, pClient);

					Sleep(100);

					memset(sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO)*CRASH_MAX_LOAD_NUMBER);
					memset(&sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO_LIST));
					nCount = 0;
				}
			}
			////////////////////////////////////////////////////////////////
		}
		
	ADO_ON_ERROR(e)
		sCrashList.byResult		= 0;
		sCrashList.nCount		= 0;
		sCrashList.byLastList	= 0;
		
		m_pServer->SendCrashInfoList(&sCrashList, 
			sCrashInfo, nCount, pClient);
		OnADOError(e, "SP_GetCrashInfo");
	ADO_END()
	

	return snResult;
}

//////////////////////////////////////////////////////////////////////

SHORT CAdoExecutor::CrashToolSearchInfo(ON_P_REQ_SEARCHCRASHINFO pInfo, XClient* pClient)
{
	SHORT		snResult = 0;
	bool		bIsEof = false;
	char		szQuery[2048];
	memset(szQuery, 0, sizeof(szQuery));

	if(m_pAdoConnection->IsOpen() == false)
	{
		if(Connect() == FALSE)
		{
			return snResult;
		}
	}
	
	CAdoRecordset   cAdoRecordset(m_pAdoConnection);

	sprintf(szQuery, "select id, CrashType, Contents, OSType, ProcessType, IP, ProcessNumber, ProcessLevel, PageSize, StartTime, EndTime, CrashAddress, LineAddress, NationCode, Supposition from viewCrashInfo where %s order by LineAddress asc", pInfo->szQuery);

	ON_RESP_SEARCHCRASHINFO			sCrashInfo[CRASH_MAX_LOAD_NUMBER];
	ON_RESP_SEARCHCRASHINFO_LIST	sCrashList;
	int	nCount = 0;
	memset(sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO)*CRASH_MAX_LOAD_NUMBER);
	memset(&sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO_LIST));

	bool bOpen = cAdoRecordset.Open(szQuery, CAdoRecordset::openQuery);

	if(bOpen == false)
	{
		sCrashList.byResult		= 0;
		sCrashList.nCount		= 0;
		sCrashList.byLastList	= 1;
		
		m_pServer->SendCrashInfoList(&sCrashList, 
			sCrashInfo, nCount, pClient);

		return snResult;	
	}

	while(cAdoRecordset.IsEof() == false)
	{
		sCrashInfo[nCount].dwID			= (LONG)cAdoRecordset.GetFieldValue("id");
		sCrashInfo[nCount].nCrashType	= cAdoRecordset.GetFieldValue("CrashType");
		strcpy(sCrashInfo[nCount].szCrashContents, (_bstr_t)cAdoRecordset.GetFieldValue("Contents"));
		strcpy(sCrashInfo[nCount].szOSType, (_bstr_t)cAdoRecordset.GetFieldValue("OSType"));
		strcpy(sCrashInfo[nCount].szProcessType, (_bstr_t)cAdoRecordset.GetFieldValue("ProcessType"));
		strcpy(sCrashInfo[nCount].szIP, (_bstr_t)cAdoRecordset.GetFieldValue("IP"));
		sCrashInfo[nCount].nProcessNumber = cAdoRecordset.GetFieldValue("ProcessNumber");
		sCrashInfo[nCount].nProcessLevel = cAdoRecordset.GetFieldValue("ProcessLevel");
		sCrashInfo[nCount].nPageSize = cAdoRecordset.GetFieldValue("PageSize");
		strcpy(sCrashInfo[nCount].szsTime, (_bstr_t)cAdoRecordset.GetFieldValue("StartTime"));
		strcpy(sCrashInfo[nCount].szeTime, (_bstr_t)cAdoRecordset.GetFieldValue("EndTime"));
		strcpy(sCrashInfo[nCount].szCrashAddress, (_bstr_t)cAdoRecordset.GetFieldValue("CrashAddress"));
		strcpy(sCrashInfo[nCount].szLineAddress, (_bstr_t)cAdoRecordset.GetFieldValue("LineAddress"));
		strcpy(sCrashInfo[nCount].szNationCode, (_bstr_t)cAdoRecordset.GetFieldValue("NationCode"));
		strcpy(sCrashInfo[nCount].szSupposition, (_bstr_t)cAdoRecordset.GetFieldValue("Supposition"));

		cAdoRecordset.MoveNext();

		////////////////////////////////////////////////////////////////

		bIsEof = cAdoRecordset.IsEof();
		if(++nCount == CRASH_MAX_LOAD_NUMBER || bIsEof == true)
		{
			if(bIsEof == true)
			{
				sCrashList.byResult		= 1;
				sCrashList.nCount		= nCount;
				sCrashList.byLastList	= 1;
				
				m_pServer->SendCrashInfoList(&sCrashList, 
					sCrashInfo, nCount, pClient);

				Sleep(100);

				break;
			}
			else
			{
				sCrashList.byResult		= 1;
				sCrashList.nCount		= nCount;
				sCrashList.byLastList	= 0;
				
				m_pServer->SendCrashInfoList(&sCrashList, 
					sCrashInfo, nCount, pClient);

				Sleep(100);

				memset(sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO)*CRASH_MAX_LOAD_NUMBER);
				memset(&sCrashInfo, 0, sizeof(ON_RESP_SEARCHCRASHINFO_LIST));
				nCount = 0;
			}
		}
	}
		////////////////////////////////////////////////////////////////	

	return snResult;
}

//////////////////////////////////////////////////////////////////////

SHORT CAdoExecutor::CrashToolStatisticsInfo(void* pMsg, XClient* pClient)
{	
	ON_P_REQ_CRASHSTATISTICSINFO	pInfo = (ON_P_REQ_CRASHSTATISTICSINFO)pMsg;	
	
	bool		bIsEof = false;	
	char		szQuery[2048];
	memset(szQuery, 0, sizeof(szQuery));

	if(m_pAdoConnection->IsOpen() == false)
	{
		if(Connect() == FALSE)
		{
			return 0;
		}
	}
	
	CAdoRecordset   cAdoRecordset(m_pAdoConnection);

	//////////////////////////////////////////////////////////////////
	// 전체 개수
	ON_RESP_CRASHSTATISTICSINFO	sCrashStatistics;
	memset(&sCrashStatistics, 0, sizeof(ON_RESP_CRASHSTATISTICSINFO));

	//////////////////////////////////////////////////////////////////

	sprintf(szQuery, "select id from CrashInfo");
	bool bOpen = cAdoRecordset.Open(szQuery, CAdoRecordset::openQuery);
	if(bOpen == false)
	{
		sCrashStatistics.byResult		= 0;

		m_pServer->SendCrashStatisticsInfo(&sCrashStatistics, pClient);
		return 0;
	}
	sCrashStatistics.nCrashCount[0] = cAdoRecordset.GetRecordCount();

	//////////////////////////////////////////////////////////////////
	// 전체 통계
	if(pInfo->byAllField == 1)
	{
		for(int n = 1 ; n < CRASH_MAX_CRASH_TYPE_NUMBER ; n++)
		{
			sprintf(szQuery, "select id from CrashInfo where CrashType = %d", n);
			bOpen = cAdoRecordset.Open(szQuery, CAdoRecordset::openQuery);
			if(bOpen == false)
			{
				sCrashStatistics.byResult		= 0;

				m_pServer->SendCrashStatisticsInfo(&sCrashStatistics, pClient);
				return 0;
			}
			sCrashStatistics.nCrashCount[n] = cAdoRecordset.GetRecordCount();
		}
	}	
	//////////////////////////////////////////////////////////////////
	// Crash Type별 통계
	else
	{
		for(int n = 1 ; n < CRASH_MAX_CRASH_TYPE_NUMBER ; n++)
		{
			sprintf(szQuery, "select id from CrashInfo where CrashType = %d %s", n, pInfo->szQuery);
			bOpen = cAdoRecordset.Open(szQuery, CAdoRecordset::openQuery);
			if(bOpen == false)
			{
				sCrashStatistics.byResult		= 0;

				m_pServer->SendCrashStatisticsInfo(&sCrashStatistics, pClient);
				return 0;
			}
			sCrashStatistics.nCrashCount[n] = cAdoRecordset.GetRecordCount();
		}
	}
	//////////////////////////////////////////////////////////////////	

	sCrashStatistics.byResult		= 1;
	m_pServer->SendCrashStatisticsInfo(&sCrashStatistics, pClient);

	return 1;
}

//////////////////////////////////////////////////////////////////////

void CAdoExecutor::OnADOError(_com_error& e, char* szText)
{
	_bstr_t bstrSource		(e.Source());
	_bstr_t bstrDescription	(e.Description());
	char	szSource[256] = {0,};
	char	szDescription[512] = {0,};
	char	szBuf[1024] = {0,};

	strncpy(szSource, bstrSource, 256);
	strncpy(szDescription, bstrDescription, 512);
	
	sprintf(szBuf, "meaning = %lx\nSource = %s\nDescription = %s", 
		szSource, szDescription);

	m_LogWriter.PrintF("AdoError.Log", "DB Error: %s\nCode = %lx\n%s", 
		szText, e.Error(), szBuf );
}

//////////////////////////////////////////////////////////////////////