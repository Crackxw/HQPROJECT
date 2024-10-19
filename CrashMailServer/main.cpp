


#include <stdio.h>
#include <conio.h>
#include "CrashUserServer.h"
#include "CrashToolServer.h"

void MainProcess();

int main(int argc, char* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	
	int err;
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 )
	{
		return 0;
	}
 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup(); 
		return 0; 
	}

	CCrashUserServer* pCrashUserServer = new CCrashUserServer();
	CCrashToolServer* pCrashToolServer = new CCrashToolServer();

	pCrashUserServer->Create();
	pCrashToolServer->Create();
	pCrashUserServer->Start();
	pCrashToolServer->Start();
	pCrashUserServer->StartThread();
	pCrashToolServer->StartThread();

	printf("\n\n==========================================================\n");
	printf(" Crash Mail Server start. At %s %s\n",__DATE__,__TIME__);
	printf("==========================================================\n");

	MainProcess();

	pCrashUserServer->StopThread();
	pCrashToolServer->StopThread();

	delete pCrashUserServer;
	delete pCrashToolServer;

	WSACleanup();

	return 0;
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

