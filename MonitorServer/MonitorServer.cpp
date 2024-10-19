#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"		
#include "..\\XCommon\\JXPacket.h"		
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"
#include "..\\XCommon\\XMonitorCmd.h"
#include "..\\ServerMonitor\\SMSProtocol.h" 
#include "..\\ServerMonitor\\MonitorProtocol.h"


#include "MonitorServer.h"

#include "XClientMgr.h"
#include "XServerMgr.h"

#include "XClient.h"
#include "XServer.h"


BOOL    bRefreshState;
BOOL    bIsServerDead;


void	writeInfoToFile(bool timeStamp, char* filename, char* str, ...)
{
	if( str )
	{
		char body[1024];
		memset(body,0,sizeof(body));
		wvsprintf(body, str, (LPSTR)(&str+1) );

		FILE* fp;		
		if( (fp = fopen(filename,"at")) != NULL )		
		{
			if( timeStamp )
			{
				char time[128];
				char date[128];

				_strtime( time );
				_strdate( date );

				fprintf(fp,"[%s %s] %s\n", date, time, body);
			}
			else
			{
				fprintf(fp,"%s\n", body);
			}

			fclose(fp);
		}
	}
}

unsigned __stdcall CheckProcess( LPVOID param )
{
	HANDLE handle = (HANDLE)param;
	DWORD  dwExitCode;

	while( TRUE ) {

		Sleep( 10 );

		GetExitCodeProcess( handle, &dwExitCode );

		if( dwExitCode != STILL_ACTIVE ) break;
	}

//	writeInfoToFile( TRUE, "log.txt", "ExitCodeProcess : 0x%x\n", dwExitCode );

	return 0;
}


MonitorServer::MonitorServer()// : 
//CJOGersangConfig()
{
 
	m_pGateSocketClient = new JXGateSocket;	
	m_pAcceptListClient = new JXAcceptSocketList;
	m_pConnectListClient = new JXSocketMgr;
	m_pClientMgr = new XClientMgr;

	m_pGateSocketServer = new JXGateSocket;	
	m_pAcceptListServer = new JXAcceptSocketList;
	m_pConnectListServer = new JXSocketMgr;
	m_pServerMgr = new XServerMgr;

	m_pClientToRemove = NULL;
	m_siNumClientToRemove = 0;

	m_pServerToRemove = NULL;
	m_siNumServerToRemove = 0;

	m_hServerProcess = NULL;
	m_bUpdateServerTime = FALSE;

	m_siRefreshCount = 0;

	m_wCurrentUserNum = 0;
	m_bSendUserNum = FALSE;
	m_dwServerProcID = 0;
		
#ifdef __CONFIG_FILE_LOAD__
	// Config ���� ��� ����.
	SetConfigFileName(GERSANG_SERVER_CONFIG_FILE);
#endif

	m_pUser = new XUser[ 50 ];

	ZeroMemory(&m_ServerOnTime,sizeof(m_ServerOnTime));

	for( int i = 0; i < 50; ++i ) {
		m_pUser[ i ].m_usercode = i;
	}
}

MonitorServer::~MonitorServer()
{
	if( m_pClientToRemove ) delete [] m_pClientToRemove;
	if( m_pServerToRemove ) delete [] m_pServerToRemove;
	
	delete m_pGateSocketClient;
	delete m_pAcceptListClient;
	delete m_pConnectListClient;
	delete m_pClientMgr;

	delete m_pGateSocketServer;
	delete m_pAcceptListServer;
	delete m_pConnectListServer;
	delete m_pServerMgr;

	delete [] m_pUser;

}

BOOL MonitorServer::Init()
{

	// Input..

	SI32 siMaxClient = 1;
	SI32 siMaxServer = 1;

	if( m_pClientToRemove ) delete [] m_pClientToRemove;
	if( m_pServerToRemove ) delete [] m_pServerToRemove;

	m_pClientToRemove = new XClient*[ siMaxClient ];
	m_siNumClientToRemove = 0;

	m_pServerToRemove = new XServer*[ siMaxServer ];
	m_siNumServerToRemove = 0;


	// �÷��̾� ���� �ʱ�ȭ 
	m_pAcceptListClient->Create( siMaxClient, 30000, 30000, 30000 );
	m_pGateSocketClient->Create( MONITOR_SERVER_PORT, m_pAcceptListClient );
	m_pClientMgr->Create( siMaxClient );

	// ���� ���� �ʱ�ȭ
	m_pAcceptListServer->Create( siMaxServer, 30000, 30000, 30000 );
	m_pGateSocketServer->Create(MONITOR_GAME_SERVER_PORT, m_pAcceptListServer );
	m_pServerMgr->Create( siMaxServer );

	m_dwRefreshTime = timeGetTime();

#ifdef __CONFIG_FILE_LOAD__
	gethostname(m_szHostName, sizeof(m_szHostName));
#endif

	return TRUE;
}

void MonitorServer::Start()
{
	printf("����������������������������������������������������������������������������\n");
	printf(" �Ż� �¶��� ����͸� ���� Ver 0.1\n");
	printf("����������������������������������������������������������������������������\n");


	// Ŭ���̾�Ʈ
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();


	// ����
	m_pGateSocketServer->StartThread();
	m_pConnectListServer->StartThread();

	m_bServerRun = TRUE;
}

void MonitorServer::Run()
{

	static bOldServerStatus = TRUE;

	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XClient			*pClient;
	XServer			*pServer;
	
	JXPacketQueue	*pPacketQueue = NULL;

	while( m_bServerRun ) {

		Sleep( 50 );

		// ������ �޾Ƶ��� �ű� ���� ����Ʈ�� �� ����Ʈ�� �Űܿ´�

		// (�÷��̾�)
		m_pAcceptListClient->Wait( INFINITE );
		m_pConnectListClient->Wait( INFINITE );


		BEGIN_JXLIST_LOOP( i, m_pAcceptListClient, JXSocket, pSocket ) 

			printf("AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );			
			
			m_pClientMgr->AddClient( pSocket );
			
			FD_SET( pSocket->m_socket, &m_pConnectListClient->m_fdset );
		
		END_JXLIST_LOOP

		m_pConnectListClient->PushFront( m_pAcceptListClient );

		// ���� ����

		for( j = 0; j < m_siNumClientToRemove; ++j ) {
			pClient = m_pClientToRemove[ j ];

			if( pClient->IsValid() == FALSE ) continue;

			FD_CLR( pClient->GetSocket()->m_socket, &m_pConnectListClient->m_fdset );

			pClient->GetSocket()->Disconnect();

			m_pConnectListClient->Remove( (JXObject *)(pClient->GetSocket()) );
			
			m_pClientMgr->DelClient( pClient );

			for( int k = 0; k < 50; ++k ) {
				m_pUser[ k ].Disconnect();
			}

		}
		m_siNumClientToRemove = 0;
		 
		m_pConnectListClient->Release();
		m_pAcceptListClient->Release();
  


		// (������)
		m_pAcceptListServer->Wait( INFINITE );
		m_pConnectListServer->Wait( INFINITE );


		BEGIN_JXLIST_LOOP( i, m_pAcceptListServer, JXSocket, pSocket ) 

			printf("AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );			
			
			m_pServerMgr->AddServer( pSocket );
			
			FD_SET( pSocket->m_socket, &m_pConnectListServer->m_fdset );
		
		END_JXLIST_LOOP

		m_pConnectListServer->PushFront( m_pAcceptListServer );

		// ���� ����

		for( j = 0; j < m_siNumServerToRemove; ++j ) {
			pServer = m_pServerToRemove[ j ];

			if( pServer->IsValid() == FALSE ) continue;

			FD_CLR( pServer->GetSocket()->m_socket, &m_pConnectListServer->m_fdset );

			pServer->GetSocket()->Disconnect();

			m_pConnectListServer->Remove( (JXObject *)(pServer->GetSocket()) );
			
			m_pServerMgr->DelServer( pServer );

		}
		m_siNumServerToRemove = 0;



		m_pConnectListServer->Release();
		m_pAcceptListServer->Release();


		bRefreshState = FALSE;
		DWORD dwNowTime = timeGetTime();
		if (dwNowTime - m_dwRefreshTime >= 7000)
		{
			m_dwRefreshTime = dwNowTime;

			bRefreshState = TRUE;
			bIsServerDead = IsServerDead();

			/*if (bOldServerStatus != bIsServerDead)
			{
				// ������ �����Ÿ�
				if (bIsServerDead)
				{
					writeInfoToFile(TRUE,"OnOffLog.txt","Server ON");

				}
				// ������ �����Ÿ�
				else
				{
					writeInfoToFile(TRUE,"OnOffLog.txt","Server OFF");

				}

				

			}*/

			bOldServerStatus = bIsServerDead;
			
			

			
			// �׾���
			if (bIsServerDead)
			{
				m_wCurrentUserNum = 0;
			}
			m_siRefreshCount++;
			// Ÿ�Ӿƿ� 
			if (m_siRefreshCount >= 5)
			{
				m_wCurrentUserNum = 0;
				m_siRefreshCount = 5;
			}


		}
	


		// Ŭ���̾�Ʈ ���μ���
		BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pClient ) 

			
			ProcPacketClient( pClient );

		END_JXLIST_LOOP


		// ���� ���μ���
		BEGIN_JXLIST_LOOP( i, m_pServerMgr, XServer, pServer ) 

			ProcPacketServer( pServer );

		END_JXLIST_LOOP

		// �ܼ�ó��
		ProcessConsole();
		
	}

}

void MonitorServer::Stop()
{

	m_bServerRun = FALSE;

	printf( "������ �����մϴ�\n" );

}

void MonitorServer::ProcessConsole()
{
	char ch;

	if( kbhit() ) {

		ch = getch();

		switch( ch ) {
		case 27:	// ESC
			Stop();
			break;

		case '1':
			break;

		case '2':
			break;

		}

	}

}


// Ŭ���̾�Ʈ ��Ŷ ó��..
void MonitorServer::ProcPacketClient( XClient *pClient )
{

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

    // ���� ���� �˾ƿ���
	pClient->GetSocket()->IsConnect( &bConnect );

	// ������ ����� �����̸�, �α׾ƿ� ó���� �Ѵ�
	if( bConnect == FALSE ) {

		LogOutClient( pClient );
		return;
	}

	if (bRefreshState)
	{
		char buf[256];
		
		if (bIsServerDead)
		{
				
				*(WORD *)&buf[0] = 4;
			    *(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_DEAD;
				pClient->SendPacket(buf,4);

			// �������� �������ϸ� 0�� �����ش�. 
			if (m_bSendUserNum) {
				*(WORD *)&buf[0] = 6;
				*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_CURRENTNUM;
				*(WORD *)&buf[4] = 0;
				pClient->SendPacket(buf,6);
			}
			m_wCurrentUserNum = 0;

		}
		else
		{
			// ��Ʈ��Ʈ�� ��������(25��)�� �ȿ����� Ÿ�Ӿƿ� ������ ������.
			if (m_siRefreshCount >= 5 )
			{
				
				*(WORD *)&buf[0] = 4;
				*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_TIMEOUT;
				pClient->SendPacket(buf,4);
				
				
			}
			else {
				*(WORD *)&buf[0] = 4;
				*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_LIVE;
				pClient->SendPacket(buf,4);
			}
			
			// �������� �������ϸ� �����ش�.
			if (m_bSendUserNum) {
				*(WORD *)&buf[0] = 6;
				*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_CURRENTNUM;
				*(WORD *)&buf[4] = m_wCurrentUserNum;
				m_wCurrentUserNum = 0;
				pClient->SendPacket(buf,6);
			}

		
			
			
		}
		

	}




	

	// ť ������ ����
	pPacketQueue = pClient->GetSocket()->GetQueue();

    // ť�� �ִ� ��Ŷ ���� ����
	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

    // ��Ŷ ���� ��ŭ �ݺ�
	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		// ��Ŷ ����
		if( pPacketQueue->Dequeue( &local_packet ) == FALSE ) return;

		// ��Ŷ���� Ŀ�ǵ� ����
		local_packet.ReadValue( &local_usCmd, sizeof( local_usCmd ) );

		// ��Ŷ ó��..
		switch( local_usCmd ) {
		case MMC_MASTER_TO_MONITOR:
			UI16 uc;
			UI16 size;
			
			local_packet.ReadValue( &uc, 2 );
			local_packet.ReadValue( &size, 2 );
			local_packet.ReadValue( local_buf, size );

			m_pUser[ uc ].Process( local_buf, size ,pClient,this);
			break;

		case MMC_CLIENT_CONNECT:
			local_packet.ReadValue( &uc, 2 );
			m_pUser[ uc ].Connect(pClient,this);
			break;

		case MMC_CLIENT_DISCONNECT:
			local_packet.ReadValue( &uc, 2 );
			m_pUser[ uc ].Disconnect();
			break;

		///////////////////////////////////////////////////////////////
		// SMS ����
		//
		case SS_SMS_REQ_MSG : 
			{
				SMS_REQ_MSG sSMS;
				memset(&sSMS, 0, sizeof(SMS_REQ_MSG));

				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

				local_packet.ReadValue(&sSMS, sizeof(SMS_REQ_MSG));			

				printf("Recv From MasterServer [%s] -- ", sSMS.szCallMessage);

				/////////////////////////////////////////////////////////
				//  Game Server�� ����
				//
				if(bIsServerDead == FALSE)
				{
					UI16 cmd = SS_SMS_REQ_MSG;
					m_LocalPacketA.ClearPacket();
					m_LocalPacketA.WriteValue( &cmd, 2 );
					m_LocalPacketA.WriteValue( &sSMS, sizeof(SMS_REQ_MSG));					
					m_LocalPacketA.CompletePacket();

					printf("Send To GameServer.");

					int i = 0;
					XServer *pServer;

					BEGIN_JXLIST_LOOP( i, m_pServerMgr, XServer, pServer)
						pServer->GetSocket()->SendPacket(&m_LocalPacketA);
					END_JXLIST_LOOP
				}
				printf("\n");
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
			}
			break;
		///////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////
		// SMS ����
/*		case SS_SMS_RESP_MSG : 
			{
				SMS_RESP_MSG sSMS;
				memset(&sSMS, 0, sizeof(SMS_RESP_MSG));

				local_packet.ReadValue(&sSMS, sizeof(SMS_RESP_MSG));

				UI16 usCmd = SS_SMS_RESP_MSG;		
				m_LocalPacketA.ClearPacket();
				m_LocalPacketA.WriteValue(&usCmd,2);
				m_LocalPacketA.WriteValue(&sSMS,sizeof(SMS_RESP_MSG));
				m_LocalPacketA.CompletePacket();
				SendToServer(&m_LocalPacketA);
			}
			break;
*/
		//////////////////////////////////////////////////////////
		}

	}

}


// ���� ��Ŷ ó��..
void MonitorServer::ProcPacketServer( XServer *pServer )
{

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

    // ���� ���� �˾ƿ���
	pServer->GetSocket()->IsConnect( &bConnect );

	// ������ ����� �����̸�, �α׾ƿ� ó���� �Ѵ�
	if( bConnect == FALSE ) {
		LogOutServer( pServer );
		return;
	}

	// ť ������ ����
	pPacketQueue = pServer->GetSocket()->GetQueue();

    // ť�� �ִ� ��Ŷ ���� ����
	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

    // ��Ŷ ���� ��ŭ �ݺ�
	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		


		// ��Ŷ ����
		if( pPacketQueue->Dequeue( &local_packet ) == FALSE ) return;

		// ��Ŷ���� Ŀ�ǵ� ����
		local_packet.ReadValue( &local_usCmd, sizeof( local_usCmd ) );

		// ��Ŷ ó��..
		switch( local_usCmd ) {		
		case MONITORCMD_CURRENT_QUEUE:
			{
				DWORD gatequeuesize;
				DWORD subqueuesize;

				local_packet.ReadValue( &gatequeuesize, sizeof( gatequeuesize ) );
				local_packet.ReadValue( &subqueuesize, sizeof( subqueuesize ) );
					

				Monitor_Command_Server_Queue_Status data;

				data.GateQueueSize = gatequeuesize;
				data.SubQueueSize = subqueuesize;

				char buf[128];

				*(WORD *)&buf[0] = 4 + sizeof(data);
				*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_QUEUE_STATUS;
				memcpy(&buf[4],&data,sizeof(data));				
						
					
						
				SendToClient(buf,4 + sizeof(data));

			}
			break;
		case MONITORCMD_CURRENT_CONNECT:
			{
				// ������ ���� ��Ŷ�� �ϳ��� �޾����� �������� ī��Ʈ�� 0��
				m_siRefreshCount = 0;

				WORD num;
				DWORD pID;
				switch (local_packet.GetPacketSize())
				{
					// ���Ӽ������� �� ��Ŷ
				case 10:
					local_packet.ReadValue( &pID, sizeof( pID ) );
					local_packet.ReadValue( &num, sizeof( num ) );
					m_wCurrentUserNum = num;
					m_bSendUserNum = TRUE;
					printf("pID : %ld time : %ld\n",pID,timeGetTime());
					break;
					// ��Ÿ���� �� ��Ŷ
				case 8:
					local_packet.ReadValue( &pID, sizeof( pID ) );
					printf("pID : %ld time : %ld\n",pID,timeGetTime());
					break;
					
				}
				// �ڱⰡ �˰� �ִ� ���μ��� ���̵�� �ٸ��� �̹� ���ִ����̹Ƿ� ��ü
				
				if (pID != m_dwServerProcID)
				{
					printf("���μ��� ���̵� �ٸ��Ƿ� ��ü!!!\n");
					// ���� ���μ����� ����ִٸ� �����Ѵ�.
					if (m_hServerProcess)
					{
						TerminateProcess(m_hServerProcess,0);
						CloseHandle(m_hServerProcess);
					}

					m_dwServerProcID = pID;					
					m_hServerProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,m_dwServerProcID);
					if (!m_hServerProcess)
					{
						printf("��ü ����!!! error code : %ld\n",GetLastError());
					}
					else
					{

						UI32 uiThreadID;

						HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, CheckProcess, (LPVOID)m_hServerProcess, 0, &uiThreadID );
						CloseHandle( hThread );

						GetLocalTime(&m_ServerOnTime);
						char buf[260];

						*(WORD *)&buf[0] = 4 + sizeof(SYSTEMTIME);
						*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_ON_TIME;
						memcpy(&buf[4],&m_ServerOnTime,sizeof(SYSTEMTIME));				


						SendToClient(buf,4 + sizeof(SYSTEMTIME));
					}
				}
			}
			break;	
		}
	}
}

void MonitorServer::LogOutClient( XClient *pClient )
{
	m_pClientToRemove[ m_siNumClientToRemove++ ] = pClient;
}

void MonitorServer::LogOutServer( XServer *pServer )
{

	m_pServerToRemove[ m_siNumServerToRemove++ ] = pServer;
}

int MonitorServer::TurnOnServer()
{

	TurnOffServer();
	/*if (m_hServerProcess)
	{
		DWORD ExitCode;
		if (GetExitCodeProcess(m_hServerProcess,&ExitCode))
		{
			if (ExitCode == STILL_ACTIVE)
			{
				printf("������ �̹� �����ֽ��ϴ�. \n");
				return 0;
			}
		}
		CloseHandle(m_hServerProcess);
		


	}*/

	printf("������ ŵ�ϴ�. \n");

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEFILLATTRIBUTE;
	si.dwFillAttribute = FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE;
	si.wShowWindow = SW_MINIMIZE;

	PROCESS_INFORMATION pi;

	char str[256];
#ifndef __CONFIG_FILE_LOAD__
	FILE *fp;	
	sprintf(str,"%s\\serverexec.txt",g_OriginalPath);
	printf("%s\n",str);
	fp = fopen(str,"rt");
	if (fp != NULL) {
		fgets(str,256,fp);
		if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = 0;		
		for (int a = strlen(str) - 1; a > 0; a--)
		{
			if (str[a] == '\\')
			{
				char dir[256];
				strcpy(dir,str);
				dir[a] = 0;
				printf("���� ���丮�� %s�� ����\n",dir);
				SetCurrentDirectory(dir);
				break;

			}

		}
		printf(" %s ����!!\n",str);
	
		if (!CreateProcess(str,NULL,NULL,NULL,NULL,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
		{
			printf("����Ű�� ����\n");
			ZeroMemory(&m_ServerOnTime,sizeof(m_ServerOnTime));
		}
		else {
			CloseHandle(pi.hThread);
			m_hServerProcess = pi.hProcess;
			m_dwServerProcID = pi.dwProcessId;


			GetLocalTime(&m_ServerOnTime);

			char buf[260];
			
			*(WORD *)&buf[0] = 4 + sizeof(SYSTEMTIME);
			*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_ON_TIME;
			memcpy(&buf[4],&m_ServerOnTime,sizeof(SYSTEMTIME));				

			SendToClient(buf,sizeof(SYSTEMTIME) + 4);

			UI32 uiThreadID;

			HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, CheckProcess, (LPVOID)pi.hProcess, 0, &uiThreadID );
			CloseHandle( hThread );
		}

	}
	else
	{
		printf("serverexec.txt�� ������ �����ϴ�\n");
	}
#else // Gersang.ini load.
	strcpy(str, GetGameServerPath());
	if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = 0;
	printf("%s\n",str);

	if((int)strlen(str) > 0)
	{
		for (int a = strlen(str) - 1; a > 0; a--)
			{
				if (str[a] == '\\')
				{
					char dir[256];
					strcpy(dir,str);
					dir[a] = 0;
					printf("���� ���丮�� %s�� ����\n",dir);
					SetCurrentDirectory(dir);
					break;

				}
			}
			printf(" %s ����!!\n",str);
		
			if (!CreateProcess(str,NULL,NULL,NULL,NULL,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
			{
				printf("����Ű�� ����\n");
				ZeroMemory(&m_ServerOnTime,sizeof(m_ServerOnTime));
			}
			else 
			{
				CloseHandle(pi.hThread);
				m_hServerProcess = pi.hProcess;
				m_dwServerProcID = pi.dwProcessId;


				GetLocalTime(&m_ServerOnTime);

				char buf[260];
				
				*(WORD *)&buf[0] = 4 + sizeof(SYSTEMTIME);
				*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_ON_TIME;
				memcpy(&buf[4],&m_ServerOnTime,sizeof(SYSTEMTIME));				

				SendToClient(buf,sizeof(SYSTEMTIME) + 4);

				UI32 uiThreadID;

				HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, CheckProcess, (LPVOID)pi.hProcess, 0, &uiThreadID );
				CloseHandle( hThread );
			}
	}
	else
	{
		printf("Gersang.ini read failed.\n");
	}
#endif

	SetCurrentDirectory(g_OriginalPath);

	return 1;
}

void MonitorServer::TurnOffServer()
{

	printf("������ ���ϴ�. \n");
	if (m_hServerProcess)
	{
		DWORD ExitCode;
		if (GetExitCodeProcess(m_hServerProcess,&ExitCode))
		{
			if (ExitCode == STILL_ACTIVE)
			{
				TerminateProcess(m_hServerProcess,0);
			}
		}
		CloseHandle(m_hServerProcess);
		m_hServerProcess = NULL;
		


	}

}

BOOL MonitorServer::IsServerDead()
{

	
	if (m_hServerProcess)
	{
		DWORD ExitCode;
		
		if (GetExitCodeProcess(m_hServerProcess,&ExitCode))
		{
			if (ExitCode == STILL_ACTIVE)
			{
				return FALSE;
			}
		}
			
	}
	return TRUE;

}

int MonitorServer::SendToServer(JXPacket *pPacket)
{
	int i = 0;
	XServer *pServer;

	BEGIN_JXLIST_LOOP( i, m_pServerMgr, XServer, pServer ) 

			pServer->GetSocket()->SendPacket(pPacket);
		

    END_JXLIST_LOOP

	return 0;

}

int MonitorServer::SendToClient(void *buf,int size)
{
	int i = 0;
	XClient *pClient;

	BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pClient ) 
		
	BOOL bConnect;

	pClient->GetSocket()->IsConnect( &bConnect );
	
	if (bConnect) {
		
		
		pClient->SendPacket((char *)buf,size);
		
	}
	
    END_JXLIST_LOOP

	return 0;

}





