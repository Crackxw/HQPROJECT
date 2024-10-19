#include <stdio.h>
#include <conio.h>

#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"		
#include "..\\XCommon\\JXPacket.h"		
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"
#include "..\\XCommon\\XMonitorCmd.h"

#include "..\\ServerMonitor\\MonitorProtocol.h"
#include "..\\ServerMonitor\\SMSProtocol.h"

#include "MasterServer.h"

#include "XClientMgr.h"
#include "XServerMgr.h"

#include "XClient.h"
#include "XServer.h"

#include "CDBMgr.h"

MasterServer::MasterServer() : 
CJOGersangConfig()
{

	m_tickServer = 0;

	
	//Accept ����
	m_pGateSocketClient = new JXGateSocket;
	
	//Accept ����Ʈ ������ ����
	m_pAcceptListClient = new JXAcceptSocketList;

	//Accept ����Ʈ�� �Ű� ���� ����Ʈ(����� �� ������ ���� ������ ������ ����) �׷��� connection ����Ʈ��� �ϴ� ��¡..
	m_pConnectListClient = new JXSocketMgr;
	m_pClientMgr = new XClientMgr;		//������ �Ϲ������� ���� ������ ���� �������� ������ �ִµ�, connection ����Ʈ���� ���� ������ ������ �ְ�, ���⼭�� ���� ������ ������ ���� �̷��� �ϴ� ������ ���̺귯��ȭ�� �ֵ� �������� ����


	//������ Accept ����Ʈ�� ������ �ְ�, Accpet ����Ʈ�� �ű�� ������ blocking �� �� �� ���̱� ���ؼ���.
	//�Ϲ������� �ٷ� connection ����Ʈ�� ������, �׷��� �ϸ� Accept �Ҷ���, disconnect �Ҷ� blocking�� �ɸ��Ƿ�, �� Ƚ���� ���̱� ���ؼ���.
	//����Ʈ�� ������ �ִٰ� ����Ʈ�� �״�� ������ �ѹ��� ���� blocking �� ���� �� �����Ƿ�..

	m_pConnectServerSocket = new JXConnectSocket;		//�� Monitoring ������ �����ϱ� ���ؼ�..
	
	m_pServerMgr = new XServerMgr;		//Monitoring ���� ��ü�� �Ӽ����� ������ ����

	m_pClientToRemove = NULL;
	m_siNumClientToRemove = 0;

	m_pConnectServerSocket = NULL;

	m_usernum = 0;
	
	m_dwRefreshDB = 0;
}

MasterServer::~MasterServer()
{
	
	if( m_pConnectServerSocket ) 
	{
		delete [] m_pConnectServerSocket;
	}

	delete m_pGateSocketClient;
	delete m_pAcceptListClient;
	delete m_pConnectListClient;
	delete m_pClientMgr;

	delete m_pServerMgr;

//	m_pSMSServer->StopThread();
//	delete m_pSMSServer;

}

BOOL MasterServer::Init()
{

	// Input..

	SI32 siMaxClient = 30;
	SI32 siMaxServer = 100;


	if( m_pClientToRemove ) 
	{
		delete [] m_pClientToRemove;
	}

	m_pClientToRemove = new XClient*[ siMaxClient ];
	m_siNumClientToRemove = 0;

	// �÷��̾� ���� �ʱ�ȭ 
	m_pAcceptListClient->Create( siMaxClient, 30000, 30000, 30000 );
	m_pGateSocketClient->Create( MASTER_SERVER_PORT, m_pAcceptListClient );
	m_pClientMgr->Create( siMaxClient );

	// ���� ���� �ʱ�ȭ
	m_pServerMgr->Create( siMaxServer );

	if( m_pConnectServerSocket ) delete [] m_pConnectServerSocket;

	m_pConnectServerSocket = new JXConnectSocket[ siMaxServer ];


	// ODBC�� �ʱ�ȭ�ϰ� DB�� �����Ѵ�.[1/18/2005]
	if(CDBMgr::GetInstance()->Init())
		CDBMgr::GetInstance()->ConnectDB();
	
	return TRUE;
}

void MasterServer::Start()
{

	printf("����������������������������������������������������������������������������\n");
	printf(" �Ż� �¶��� ����͸� ������ ���� Ver 0.1\n");
	printf("����������������������������������������������������������������������������\n");

	
	BOOL bRetVal = FALSE;

#ifdef __CONFIG_FILE_LOAD__
	bRetVal = LoadConfigFile();

	if( bRetVal == FALSE )
	{
		printf( "Gersang.ini file error!\n" );
		exit( -2 );
	}
#else
	bRetVal = LoadFile();

	if( bRetVal == FALSE )
	{
		printf( "serverlist.txt file error!\n" );
		exit( -2 );
	}
#endif

	ReadUserDB();

	// Ŭ���̾�Ʈ
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();

	//
	int i, j, k = 0;
	for( i = 0; i < m_siServerGroupNum; ++i ) {
		for( j = 0; j < m_siServerNum; ++j ) {

			printf( "connect.. %s %s [%s]\n", m_szServerGroupName[ i ], m_szServerName[ i ],m_szServerIP[ i ][ j ] );

			m_pConnectServerSocket[ k ].StartThread();
			m_pConnectServerSocket[ k ].Create( m_szServerIP[ i ][ j ], MONITOR_SERVER_PORT, 30000, 30000, 30000 );
			m_pConnectServerSocket[ k ].TryConnect();

			m_pServerMgr->AddServer( &m_pConnectServerSocket[ k ] )->m_servercode = MAKESERVERCODE( i, j );

			k++;
		}
	}


	///////////////////////////////////////////////////////////
	// SMS Server ����
//	m_pSMSServer = new CSMSServer(m_pServerMgr);

//	m_pSMSServer->Create();
//	m_pSMSServer->Start();
//	m_pSMSServer->StartThread();
	///////////////////////////////////////////////////////////

	m_bServerRun = TRUE;
	
}

void MasterServer::Run()
{
	int k;


	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XClient			*pClient;
	XServer			*pServer;
	
	JXPacketQueue	*pPacketQueue = NULL;

	while( m_bServerRun ) {

		Sleep(10);

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

			// ����͸� ������ diconnect ������ 
			local_usCmd = MMC_CLIENT_DISCONNECT;
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &pClient->m_uiID, 2 );
			local_packet.CompletePacket();

			m_pServerMgr->SendPacket( &local_packet );
			///////////////////////////////////

			if( pClient->m_bLoginOK == TRUE ) {
				local_usCmd = MMC_REPORT_USER_EXIT;
				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( &pClient->m_user.id, 32 );
				local_packet.WriteValue( &pClient->m_user.name, 32 );
				local_packet.CompletePacket();

				m_pClientMgr->SendPacket( &local_packet );			
			}

			FD_CLR( pClient->GetSocket()->m_socket, &m_pConnectListClient->m_fdset );
			
			pClient->GetSocket()->Disconnect();
			
			m_pConnectListClient->Remove( (JXObject *)(pClient->GetSocket()) );
			
			m_pClientMgr->DelClient( pClient );

		}
		m_siNumClientToRemove = 0;
		
		m_pConnectListClient->Release();
		m_pAcceptListClient->Release();



		// ���� ���� ����
		if( GetTickCount() - m_tickServer > 3000 ) {
			k = 0;
			for( i = 0; i < m_siServerGroupNum; ++i ) {
				for( j = 0; j < m_siServerNum; ++j ) {
					if( m_pConnectServerSocket[ k ].m_bConnect == FALSE )
					{
						printf( "%s %s is Disconnected! Connecting..\n", m_szServerGroupName[ i ], m_szServerName[ j ] );
						m_pConnectServerSocket[ k ].TryConnect();

						CDBMgr::GetInstance()->SendQuary(FALSE, i); //�������¸� DB�� �����Ѵ�.
					}
					k++;
				}
			}
			m_tickServer = GetTickCount();
		}

        //db�� �������¸� �����Ѵ�.
		if( GetTickCount() - m_dwRefreshDB > 1000*60)
		{
			k = 0;
			for( i = 0; i < m_siServerGroupNum; ++i ) 
			{
				for( j = 0; j < m_siServerNum; ++j ) 
				{
					if( m_pConnectServerSocket[ k ].m_bConnect == TRUE )
						CDBMgr::GetInstance()->SendQuary(TRUE, i); //�������¸� DB�� �����Ѵ�.
					
					k++;
				}
			}
			m_dwRefreshDB = GetTickCount();
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

void MasterServer::Stop()
{

	m_bServerRun = FALSE;

	printf( "������ �����մϴ�\n" );

}

void MasterServer::ProcessConsole()
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
void MasterServer::ProcPacketClient( XClient *pClient )
{


	int k;
	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	char id[ 32 ];
	char pass[ 32 ];
	char name[ 32 ];
	char result;

    // ���� ���� �˾ƿ���
	pClient->GetSocket()->IsConnect( &bConnect );

	// ������ ����� �����̸�, �α׾ƿ� ó���� �Ѵ�
	if( bConnect == FALSE ) {
		LogOutClient( pClient );
		return;
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


		// �α��� ���� ���� ���
		if( pClient->m_bLoginOK == FALSE ) {

			if( local_usCmd == MMC_QUERY_LOGIN ) {

				local_packet.ReadValue( id, 32 );
				local_packet.ReadValue( pass, 32 );

				printf("Login id:[%s][%s]\n", id, pass );

				BOOL bValidID = FALSE;
				BOOL bPassOK = FALSE;
				BOOL bConnOK = TRUE;

				// ���̵� �� �н����� üũ
				for( i = 0; i < m_usernum; ++i ) {

					if( _stricmp( id, m_user[ i ].id ) == 0 ) {
						bValidID = TRUE;
					
						if( strcmp( pass, m_user[ i ].password ) == 0 ) {
							bPassOK = TRUE;
							break;
						}
					}
				}

				UI32			ii;
				XClient			*pTClient;

				// �̹� ������ �ִ��� ����
				BEGIN_JXLIST_LOOP( ii, m_pClientMgr, XClient, pTClient ) 
			
						if( _stricmp( pTClient->m_user.id, id ) == 0 ) bConnOK = FALSE;
	
				END_JXLIST_LOOP


				if( bValidID == FALSE ) result = 1;
				else if( bPassOK == FALSE ) result = 2;
				else if( bConnOK == FALSE ) result = 3;
				else {

					result = 0;

					pClient->m_bLoginOK = TRUE;
					strcpy( pClient->m_user.id,  m_user[ i ].id );
					strcpy( pClient->m_user.password,  m_user[ i ].password );
					strcpy( pClient->m_user.name,  m_user[ i ].name );
					pClient->m_user.level = m_user[ i ].level;


					// ����͸� ������ connect ������ 
					local_usCmd = MMC_CLIENT_CONNECT;
					local_packet.ClearPacket();
					local_packet.WriteValue( &local_usCmd, 2 );
					local_packet.WriteValue( &pClient->m_uiID, 2 );
					local_packet.CompletePacket();

					m_pServerMgr->SendPacket( &local_packet );
					///////////////////////////////////

					local_usCmd = MMC_REPORT_USER_ENTER;
					local_packet.ClearPacket();
					local_packet.WriteValue( &local_usCmd, 2 );
					local_packet.WriteValue( &pClient->m_user.id, 32 );
					local_packet.WriteValue( &pClient->m_user.name, 32 );
					local_packet.CompletePacket();

					m_pClientMgr->SendPacket( &local_packet );
				}
				
				if( result != 0 ) LogOutClient( pClient );

				printf( "result: %d\n", result );

				local_usCmd = MMC_REPORT_LOGIN;

				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( &result, 1 );
				local_packet.CompletePacket();

				pClient->GetSocket()->SendPacket( &local_packet );

			} else continue;
		}


		// ��Ŷ ó��..
		switch( local_usCmd ) {
		case MMC_QUERY_SERVER_STRUCT:							// ������ ���� ���� ��û�� ���� ���� 
			local_usCmd = MMC_REPORT_SERVER_STRUCT;
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &m_siServerNum, 4 );
			for( k = 0; k < m_siServerNum; ++k ) {
				local_packet.WriteValue( m_szServerName[ k ], 32 );
			}
			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;

		case MMC_QUERY_WORLD_LIST:								// ���� ���� ��û�� ���� ����
			local_usCmd = MMC_REPORT_WORLD_LIST;
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &m_siServerGroupNum, 4 );
			for( k = 0; k < m_siServerGroupNum; ++k ) {
				local_packet.WriteValue( m_szServerGroupName[ k ], 32 );
			}
			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;

		case MMC_QUERY_USERLIST:								// ��ϵ� ����� ����Ʈ 
			local_usCmd = MMC_REPORT_USERLIST;
			
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &m_usernum, 4 );

			for( k = 0; k < m_usernum; ++k ) {
				local_packet.WriteValue( m_user[ k ].id, 32 );
				local_packet.WriteValue( m_user[ k ].name, 32 );
			}

			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;

		case MMC_QUERY_CONNECTUSERLIST:							// �������� ����� ����Ʈ
			{
				UI32			i;
				XClient			*pTClient;
			
				local_usCmd = MMC_REPORT_CONNECTUSERLIST;
				SI32	n = m_pClientMgr->GetNumberNodes();

				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( &n, 4 );

				BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pTClient ) 
			
					local_packet.WriteValue( pTClient->m_user.id, 32 );
					local_packet.WriteValue( pTClient->m_user.name, 32 );
	
				END_JXLIST_LOOP

				local_packet.CompletePacket();

				pClient->GetSocket()->SendPacket( &local_packet );
			}
			break;

		case MMC_QUERY_ADDUSER:									// ����� �߰�
			local_packet.ReadValue( id, 32 );
			local_packet.ReadValue( pass, 32 );
			local_packet.ReadValue( name, 32 );

			result = 0;
			for( k = 0; k < m_usernum; ++k ) {
				if( _stricmp( m_user[ k ].id, id ) == 0 ) {
					result = 1;
					break;
				}
			}

			if( result == 0 ) {
				strcpy( m_user[ m_usernum ].id, id );
				strcpy( m_user[ m_usernum ].password, pass );
				strcpy( m_user[ m_usernum ].name, name );
				m_usernum++;

				WriteUserDB();
			}

			local_usCmd = MMC_REPORT_ADDUSER;

			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &result, 1 );
			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;
 
		case MMC_QUERY_DELUSER:									// ����� ����
			local_packet.ReadValue( id, 32 );
			break;


		case MMC_QUERY_CHAT:
			{
				char buf[ 1024 ];
				local_packet.ReadString( buf, 1024 );

				local_usCmd = MMC_REPORT_CHAT;
				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( pClient->m_user.id, 32 );
				local_packet.WriteValue( pClient->m_user.name, 32 );
				local_packet.WriteString( buf, 1024 );
				local_packet.CompletePacket();

				m_pClientMgr->SendPacket( &local_packet );
			}
			break;
		case MMC_QUERY_SMS:
			{
				char szMsg[80];

				local_packet.ReadValue(&szMsg,80);
							
				SMS_REQ_MSG sSMS;
				memset(&sSMS, 0, sizeof(SMS_REQ_MSG));

				//m_LocalPacket.ReadValue(&sSMS, sizeof(SMS_REQ_MSG));
				//sSMS.dwCallCount = ntohs(sSMS.dwCallCount);

				strncpy(sSMS.szCallMessage,szMsg,79);
				strcpy(sSMS.szOrgPhone,"00000000000");

				UI16 cmd = SS_SMS_REQ_MSG;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &cmd, 2 );
				local_packetB.WriteValue( &sSMS, sizeof(SMS_REQ_MSG));					
				local_packetB.CompletePacket();

				m_pServerMgr->SendPacket(&local_packetB);

			}
			break;
		case MMC_CLIENT_TO_SERVER:
			{
				UI16 sn;
				UI16 sc[ 1000 ];
				UI16 size;

				local_packet.ReadValue( &sn, 2 );

				for( int i = 0; i < sn; ++i ) {
					local_packet.ReadValue( &sc[ i ], 2 );
				}

				local_packet.ReadValue( &size, 2 );
				local_packet.ReadValue( local_buf, size );

				UI16 cmd = MMC_MASTER_TO_MONITOR;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &cmd, 2 );
				local_packetB.WriteValue( &pClient->m_uiID, 2 );
				local_packetB.WriteValue( &size, 2 );
				local_packetB.WriteValue( local_buf, size );
				local_packetB.CompletePacket();

				for( i = 0; i < sn; ++i )
				{
					m_pServerMgr->SendPacket( sc[ i ], &local_packetB );
				}
			}
			break;
		}
			
	}

}

// ���� ��Ŷ ó��..
void MasterServer::ProcPacketServer( XServer *pServer )
{

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

    // ���� ���� �˾ƿ���
	pServer->GetSocket()->IsConnect( &bConnect );

	// ������ ����� �����̸�, �α׾ƿ� ó���� �Ѵ�
	/*
	if( bConnect == FALSE ) {
		LogOutServer( pServer );
		return;
	}
	*/

	// ť ������ ����
	pPacketQueue = pServer->GetSocket()->GetQueue();

    // ť�� �ִ� ��Ŷ ���� ����
	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

    // ��Ŷ ���� ��ŭ �ݺ�
	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		// ��Ŷ ����
		if( pPacketQueue->Dequeue( &local_packet ) == FALSE ) return;

		//kurt9322@hotmail.com

		// ��Ŷ���� Ŀ�ǵ� ����
		local_packet.ReadValue( &local_usCmd, sizeof( local_usCmd ) );

		// ��Ŷ ó��..
		switch( local_usCmd ) {
		case MMC_MONITOR_TO_MASTER_CLIENT:
			{

				UI16 size;
				UI16 uiID;

				local_packet.ReadValue( &uiID, 2 );
				local_packet.ReadValue( &size, 2 );
				local_packet.ReadValue( local_buf, size );

				local_usCmd = MMC_SERVER_TO_CLIENT;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &pServer->m_servercode, 2 );
				local_packetB.WriteValue( local_buf, size );
				local_packetB.CompletePacket();

				m_pClientMgr->SendPacket( uiID, &local_packetB );

				printf("MMC_MONITOR_TO_MASTER_CLIENT : %d\n", uiID );
			}
			break;
		case MMC_MONITOR_TO_MASTER_ALLCLIENT:
			{
				UI16 size;
				

				local_packet.ReadValue( &size, 2 );
				local_packet.ReadValue( local_buf, size );

				local_usCmd = MMC_SERVER_TO_CLIENT;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &pServer->m_servercode, 2 );
				local_packetB.WriteValue( local_buf, size );
				local_packetB.CompletePacket();

				WORD wTemp = 0;
				memcpy(&wTemp, &local_buf[2], sizeof(WORD));
				if(wTemp == 5)
				{
					int i = pServer->m_servercode / 10;
					CDBMgr::GetInstance()->SendQuary(FALSE, i);
				}

				m_pClientMgr->SendPacket( &local_packetB );
			}
			break;
		///////////////////////////////////////////////////////////
		// SMS ����
//		case SS_SMS_RESP_MSG : 
//			{
//				SMS_RESP_MSG sSMS;
//				memset(&sSMS, 0, sizeof(SMS_RESP_MSG));

//				local_packet.ReadValue(&sSMS, sizeof(SMS_RESP_MSG));
//				m_pSMSServer->SendSMSMsgResult(&sSMS);
//			}
//			break;
		///////////////////////////////////////////////////////////
		}

	}

}

void MasterServer::LogOutClient( XClient *pClient )
{
	m_pClientToRemove[ m_siNumClientToRemove++ ] = pClient;
}

void MasterServer::LogOutServer( XServer *pServer )
{
	m_pServerToRemove[ m_siNumServerToRemove++ ] = pServer;
} 


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : �̹μ�
//  Date : 2004.01.16 PM
//
//	Desc : ���� ������ Config���� �д´�.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
BOOL MasterServer::LoadConfigFile()
{
	m_siServerGroupNum = 1;
	m_siServerNum = 1;

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	SetConfigFileName(GERSANG_SERVER_CONFIG_FILE);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	int nGroupCount = GetMasterGroupCount();
	if	(nGroupCount < 1)	return FALSE;

	m_siServerGroupNum = nGroupCount;

	for(int n = 0; n < nGroupCount ; n++)
	{
		strcpy(m_szServerType[n], GetMasterServerType(n));
		strcpy(m_szServerName[n], GetMasterServerGroupName(n));
		strcpy(m_szServerGroupName[n], GetMasterServerName(n));

		int nServerCount = GetMasterServerCount(n);
		if	(nServerCount < 1)	return FALSE;		

		for(int i = 0 ; i < nServerCount ; i++)
		{			
			strcpy(m_szServerIP[n][i], GetMasterServerIP(n, i));
		}

		m_siServerNum = m_siServerNum < nServerCount ? m_siServerNum : nServerCount;
	}

	return TRUE;
}

BOOL MasterServer::LoadFile()
{

	m_siServerGroupNum = 0;
	m_siServerNum = 0;

	int		tagcode;
	int		i;

	HANDLE	hFile = NULL;
	BOOL	bResult;
	DWORD	dwNumberOfBytesRead;
	char	ch;

	BOOL	bString;

	char	buf[ 2 ][ 256 ];

	buf[ 0 ][ 0 ] = NULL;
	buf[ 1 ][ 0 ] = NULL;
	
	bString = FALSE;

	int		currBufCode = 0;
	int		currBufPos = 0;

	int		state	=	0;			// 0: ������ ����		1: �ǹ� ����	2: �ٹٲ�

	hFile = CreateFile( "serverlist.txt", GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	while( 1 ) {

		bResult = ReadFile( hFile, &ch, 1, &dwNumberOfBytesRead, NULL );

		if( bResult == FALSE || dwNumberOfBytesRead == 0 ) break;

		if( (ch == 9 || ch == ' ' || ch == '=') && !bString )  {
			if( state == 1 ) {
				state = 0;

				buf[ currBufCode ][ currBufPos ] = NULL;

				currBufCode++;
				currBufPos = 0;
			
			}
		} else 
		if( ch == 13 || ch == 10 ) {
			if( state == 2 ) continue;

			if( state == 1 ) {
				buf[ currBufCode ][ currBufPos ] = NULL;				
			}

			/////////////////

			if( strcmp( buf[ 0 ], "[server group]" ) == 0 ) {
				printf("read server group..\n");
				tagcode = 0;
				
			} else if( strcmp( buf[ 0 ], "[server info]" ) == 0 ) {
				printf("read server info..\n");
				if( tagcode == 0 ) {
					tagcode = 1;
				}
				
				m_siServerGroupNum++;

			} else if( tagcode == 0 ) {
		
				strcpy( m_szServerType[ m_siServerNum ], buf[ 0 ] );
				strcpy( m_szServerName[ m_siServerNum ], buf[ 1 ] );

				printf( "%s, %s\n", buf[ 0 ], buf[ 1 ] );

				m_siServerNum++;

			} else if( tagcode == 1 ) {
				
				if( strcmp( buf[ 0 ], "server_name" ) == 0 ) {
					strcpy( m_szServerGroupName[ m_siServerGroupNum - 1 ], buf[ 1 ] );
				} else {

					for( i = 0; i < m_siServerNum; ++i ) {

						if( strcmp( buf[ 0 ], m_szServerType[ i ] ) == 0 ) {
							strcpy( m_szServerIP[ m_siServerGroupNum - 1 ][ i ], buf[ 1 ] );
							break;
						}

					}
				}

			}

			//////////////////

			state = 2;

			currBufCode = 0;
			currBufPos = 0;

			buf[ 0 ][ 0 ] = NULL;
			buf[ 1 ][ 0 ] = NULL;

			bString = FALSE;

		} else {

			if( currBufCode < 2 ) {
				buf[ currBufCode ][ currBufPos++ ] = ch;
			}

			if( ch == '[' || ch == '\"' ) bString = TRUE;

			state = 1;
		}
	}

	CloseHandle( hFile );

	return TRUE;
}

void MasterServer::ReadUserDB()
{

	HANDLE	hFile = NULL;
	BOOL	bResult;
	DWORD	dwNumberOfBytesRead;

	XUser	user;

	strcpy( m_user[ 0 ].id, "administrator" );
	strcpy( m_user[ 0 ].password, "duddnjsgltkfkdgo" );
	strcpy( m_user[ 0 ].name, "Administrator" );
	m_user[ 0 ].level = 1;

	m_usernum = 1;

	hFile = CreateFile( "userdb", GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;	

	while( 1 ) {

		bResult = ReadFile( hFile, user.id, 32, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 32 ) break;

		bResult = ReadFile( hFile, user.password, 32, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 32 ) break;

		bResult = ReadFile( hFile, user.name, 32, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 32 ) break;

		bResult = ReadFile( hFile, &user.level, 1, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 1 ) break;

		m_user[ m_usernum++ ] = user;
	}

	CloseHandle( hFile );
}

void MasterServer::WriteUserDB()
{

	HANDLE	hFile = NULL;
	BOOL	bResult;
	DWORD	dwNumberOfBytesWritten;

	hFile = CreateFile( "userdb", GENERIC_WRITE, FILE_SHARE_READ, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	for( int i = 1; i < m_usernum; ++i ) {

		bResult = WriteFile( hFile, m_user[ i ].id, 32, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 32 ) break;

		bResult = WriteFile( hFile, m_user[ i ].password, 32, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 32 ) break;

		bResult = WriteFile( hFile, m_user[ i ].name, 32, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 32 ) break;

		bResult = WriteFile( hFile, &m_user[ i ].level, 1, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 1 ) break;

	}

	CloseHandle( hFile );

}


void MasterServer::GetIP(SOCKET s, char *szIP)
{
	if(s == INVALID_SOCKET) szIP = NULL;

	int nLen = sizeof(sockaddr_in); 
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockaddr_in));
	
	getpeername(s, (SOCKADDR*)&sockAddr, &nLen); //IP�� ���´�.
	
	memcpy(szIP, &sockAddr.sin_addr.s_addr, 16);
}

int MasterServer::FindServerIndex(SOCKET s)
{
	if(s == INVALID_SOCKET) return -1;

	int i, j;
	char IP[16] = {0,};
	GetIP(s, IP);//���� ��ȣ�� �����Ǹ� ���´�.
	
	if(strlen(IP) > 0)
	{
		for( i = 0; i < m_siServerGroupNum; ++i )
		{
			for( j = 0; j < m_siServerNum; ++j ) 
			{
				if(strcmp(m_szServerIP[i][j], IP) == 0)
				{
					break;
				}
			}
		}
	}

//	i = wServerCode / 10;
//	j = wServerCode % 10;
	
	return i;
}