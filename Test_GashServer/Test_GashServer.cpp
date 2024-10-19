// Test_GashServer.cpp : Defines the entry point for the console application.
//

#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include "stdafx.h"
#include "GashServerMgr.h"

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

	g_GahsServerMgr.Init();
	g_GahsServerMgr.Start();
	g_GahsServerMgr.StartThread();

	printf("\n\n==========================================================\n");
	printf(" Test Gash Server start. At %s %s\n",__DATE__,__TIME__);
	printf("==========================================================\n");

	MainProcess();

	g_GahsServerMgr.StopThread();

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

