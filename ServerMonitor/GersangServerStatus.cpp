// GersangServerStatus.cpp: implementation of the CGersangServerStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerMonitor.h"
#include "ServerMonitorDlg.h"
#include "GersangServerMonitor.h"
#include "GersangServerStatus.h"

#include "MonitorProtocol.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CGersangServerStatus::siServerNum;

CGersangServerStatus::CGersangServerStatus()
{
	ZeroMemory(m_szName,MAX_SERVER_NAME);
	m_psiServerStates = NULL;
	m_siUserNum = 0;
	m_siMaxUserNum = 0;
	m_siSameUserCount = 0;
	m_siDifferance = 0;
	m_siMute = 0;
	ZeroMemory(&m_ServerOnTime,sizeof(m_ServerOnTime));



}

CGersangServerStatus::~CGersangServerStatus()
{
	if (m_psiServerStates != NULL)
	{
		delete [] m_psiServerStates;
		m_psiServerStates = NULL;
	}

	


}

void CGersangServerStatus::Init(int myservernum)
{
	m_psiServerStates = new int[CGersangServerStatus::GetServerNum()];
	m_dwHeartBeatTime = 0;
	m_siSameUserCount = 0;
	m_siDifferance = 0;
	m_siGateQueue = 0;
	m_siSubQueue = 0;
	m_siKickCount = -1;
	m_siMyServerNum = myservernum;
	

}


int CGersangServerStatus::SetServerName(const char *szName)
{
#ifdef UNICODE
	mbstowcs(m_szName,szName,strlen(szName));
#else

	strcpy(m_szName,szName);
#endif
	return 1;

}
const TCHAR *CGersangServerStatus::GetServerName()
{

	return m_szName;

}
int CGersangServerStatus::GetState(int serverkind)
{

	if (serverkind >= siServerNum || serverkind < 0) return -1;

	return m_psiServerStates[serverkind];

	
}




void CGersangServerStatus::SetServerNum(int num)
{
	if (num <= 0) num = 0;
	siServerNum = num;
}

void CGersangServerStatus::GetServerInfo(TCHAR *szString)
{

	TCHAR szTemp[256];
	TCHAR szIP[256];

	int a;
	
	
	_stprintf(szString,_T("ServerName : %s\r\n\r\n"),m_szName);

	


		if (m_ServerOnTime.wYear != 0)
		{
			_stprintf(szTemp,_T("server-on time \r\n [%ld-%2.2ld-%2.2ld](%2.2ld:%2.2ld:%2.2ld)\r\n"),m_ServerOnTime.wYear,m_ServerOnTime.wMonth,m_ServerOnTime.wDay,
				m_ServerOnTime.wHour,m_ServerOnTime.wMinute,m_ServerOnTime.wSecond);
			_tcscat(szString,szTemp);
		}



	
	
	
		
			
}


// 이함수는 1초당 한번씩 호출된다.
int CGersangServerStatus::Process()
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();
	int ret;
	if (m_siKickCount >= 0)
	{
		BOOL bSendNotice = FALSE;
		WORD servercode = MAKESERVERCODE(m_siMyServerNum,0);
		char szNotice[256];

		

		if (!(m_siKickCount % 60) && m_siKickCount > 0)
		{
			sprintf(szNotice,"%ld minute remain",m_siKickCount / 60);
			g_GersangServerMonitor.SendNotice(1,&servercode,szNotice);
			


		}
		switch(m_siKickCount)
		{
		case 30:			
		case 10:
		case 9:
		case 8:
		case 7:
		case 6:
		case 5:
		case 4:
		case 3:
		case 2:
		case 1:
			sprintf(szNotice,"%ld seconds remain",m_siKickCount);
			g_GersangServerMonitor.SendNotice(1,&servercode,szNotice);
			break;
		case 0:
			// 0초되면 튕긴다.
			g_GersangServerMonitor.SendLogout(1,&servercode);

			
			break;
		}
		m_siKickCount--;

	}

	if (timeGetTime() - m_dwHeartBeatTime >= 10000)
	{
		if (m_psiServerStates)
		{
			m_psiServerStates[0] = SERVER_STATE_TIMEOUT;
		}
	}
	return 1;

}


