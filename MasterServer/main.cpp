
#include "MasterServer.h"
 #include "SMS Server.h"
#include <conio.h>

BOOL WinSockInit();
void MainProcess();

void main()
{	
	MasterServer	server;
	CSMSServer*		m_pSMSServer = NULL;

	if( WinSockInit() == FALSE ) {
		return;
	}

	server.Init();

	server.Start();	
	
	///////////////////////////////////////////////////////////
	// SMS Server ฐüทร
	m_pSMSServer = new CSMSServer(server.m_pServerMgr,server.m_pClientMgr);

	m_pSMSServer->Create();
	m_pSMSServer->Start();
	m_pSMSServer->StartThread();

//	MainProcess();
	///////////////////////////////////////////////////////////
	server.Run();

	m_pSMSServer->StopThread();
	delete m_pSMSServer;

	WSACleanup();

}


BOOL WinSockInit()
{

	
	WORD wVersionRequested;
	WSADATA wsaData;
	
	int err;
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	err = WSAStartup( wVersionRequested, &wsaData );
	
	if ( err != 0 ) { 
		return FALSE; 
	} 
 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 2 ) {

		WSACleanup(); 
		return FALSE; 
	}

	return TRUE;
}


void MainProcess()
{
	bool bRun = TRUE;	
	while(bRun)
	{
		Sleep(10);

		if(kbhit())
		{
			switch(getch())
			{
				case '\x1b':
					{						
						bRun = FALSE;

						Sleep(100);
					}
					break;
			}
		}
	}
}

