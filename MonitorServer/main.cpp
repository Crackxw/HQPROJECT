#include "MonitorServer.h"
#include "stdio.h"

#include "..\\XCommon\\JXPacketQueue.h"
#include "..\\XCommon\\JXPacket.h"
#include "..\\XCommon\\JXList.h"

BOOL WinSockInit();

char g_OriginalPath[256];

void main()
{
	// 시간을 끌어서 이전 프로세스가 죽을 시간을 충분히
	printf("거상모니터 서버 Compiled and Linked at %s %s\n",__DATE__,__TIME__);
	Sleep(10000);
	GetCurrentDirectory(256,g_OriginalPath);

	MonitorServer		server;

	if( WinSockInit() == FALSE ) {
		return;
	}

	// 서버 초기화 
	if( server.Init() ) {

		// 서버 시동
		server.Start();

		server.Run();
	}

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