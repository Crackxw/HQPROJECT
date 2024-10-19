#include "MonitorServer.h"
#include "stdio.h"

#include "..\\XCommon\\JXPacketQueue.h"
#include "..\\XCommon\\JXPacket.h"
#include "..\\XCommon\\JXList.h"

BOOL WinSockInit();

char g_OriginalPath[256];

void main()
{
	// �ð��� ��� ���� ���μ����� ���� �ð��� �����
	printf("�Ż����� ���� Compiled and Linked at %s %s\n",__DATE__,__TIME__);
	Sleep(10000);
	GetCurrentDirectory(256,g_OriginalPath);

	MonitorServer		server;

	if( WinSockInit() == FALSE ) {
		return;
	}

	// ���� �ʱ�ȭ 
	if( server.Init() ) {

		// ���� �õ�
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