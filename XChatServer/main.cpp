#include "XChatServer.h"
#include "stdio.h"

BOOL WinSockInit();

void main()
{

	XChatServer		server;

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

