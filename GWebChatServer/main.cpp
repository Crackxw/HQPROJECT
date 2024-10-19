
#include "../JWCommon/jwbase.h"

#include "XChatServer.h"

BOOL WinSockInit();

void main( int argc, char *argv[ ] )
{

	UI16			usPort;
	XChatServer		server;

	if( WinSockInit() == FALSE ) {
		return;
	}

	server.Start();
	
	if( argc < 2 ) {
		printf( "input listen port : " );
		scanf( "%d", &usPort );
	} else {
		usPort = atoi( argv[ 1 ] );
		printf( "listen port (%d)\n", usPort );
	}

	printf( "Init..." );

	// 서버 초기화 
	server.Init( usPort );

	printf( "OK\nServer Started\n" );
	
	server.Run();

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
