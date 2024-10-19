
#include "GashServerMgr.h"
#include "GashServer.h"

CGashServerMgr g_GahsServerMgr;

CGashServerMgr::CGashServerMgr()
{
	m_pGashServer = new CGashServer[3];	
	
	m_wPort[0] = 5401;
	m_wPort[1] = 5402;
	m_wPort[2] = 5403;
	
	strncpy(m_szOutputString[0], "ServiceAccount_Authenticated", 256);
	strncpy(m_szOutputString[1], "ServiceAccount_Authorized", 256);
	strncpy(m_szOutputString[2], "Accounting_Request_Processing", 256);
}


CGashServerMgr::~CGashServerMgr()
{
	delete[] m_pGashServer;
}

void CGashServerMgr::Init()
{
	for(int n = 0 ; n < 3 ; n++)
	{
		m_pGashServer[n].Create(m_wPort[n], m_szOutputString[n]);
	}
}

void CGashServerMgr::Start()
{
	for(int n = 0 ; n < 3 ; n++)
	{
		m_pGashServer[n].Start();
	}
}

void CGashServerMgr::StartThread()
{
	for(int n = 0 ; n < 3 ; n++)
	{
		m_pGashServer[n].StartThread();
	}
}

void CGashServerMgr::StopThread()
{
	for(int n = 0 ; n < 3 ; n++)
	{
		m_pGashServer[n].StopThread();
	}
}