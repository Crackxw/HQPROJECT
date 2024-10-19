#include <winsock2.h>
#include <stdio.h>

#include "AcceptSocket.h"
#include "ListenSocket.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CListenSocket::CListenSocket(char *ip, int port, int max_socket):
MAX_SOCKET(max_socket), m_bShutdown(FALSE)
{	

	//-----------------------------------------------------------------------------
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if( m_hCompletionPort == NULL ) {

		printf("CreateIoCompletionPort() fail!\n");
		return;
	}
		
	
	//-----------------------------------------------------------------------------
	m_ListenSocket	  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if( m_ListenSocket == INVALID_SOCKET ) {

		printf("socket() fail!\n");
		return;
	}	
	
	
	//-----------------------------------------------------------------------------
	if(SOCKET_ERROR   == bind( m_ListenSocket, 
							  (SOCKADDR*)&makeSockAddrIn(ip, port), 
							  sizeof(SOCKADDR_IN))) {

		printf("bind() fail!\n");
		return;
	}	
	

	//-----------------------------------------------------------------------------
	if(SOCKET_ERROR   == listen(m_ListenSocket, 50)) {

		printf("listen() fail!\n");
		return;
	}

	
	//-----------------------------------------------------------------------------
	m_wsaConnectEvent[0] = WSACreateEvent(); 
	m_wsaConnectEvent[1] = WSACreateEvent(); 

	WSAEventSelect(m_ListenSocket, m_wsaConnectEvent[0], FD_ACCEPT);			

	InitializeCriticalSection(&m_csSocketList);		
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CListenSocket::~CListenSocket()
{
	Shutdown();
			
	WSACloseEvent(m_wsaConnectEvent[0]);
	WSACloseEvent(m_wsaConnectEvent[1]);	

	DeleteCriticalSection(&m_csSocketList);	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ó���ϴ� ����, ������� ó���ϴ� ����, ����ó�������� ���� ��Ų�� 
//--------------------------------------------------------------------------------------------
void CListenSocket::Shutdown()
{

	if(!m_bShutdown)
	{		
		m_bShutdown = TRUE;
								
		SetEvent(m_wsaConnectEvent[1]); 				
		PostQueuedCompletionStatus(m_hCompletionPort, 0, -1, NULL); 
				
		::closesocket(m_ListenSocket);
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ����Ʈ�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CListenSocket::CreateSocketList()
{

	m_SocketList = new CAcceptSocket*[MAX_SOCKET];

	if( m_SocketList ) {
		
		for(int i=0; i < MAX_SOCKET; ++i) {	
			
			m_SocketList[i] = _CallBack_CreateSocket(i);	
			
		}
	}

	m_numSocket		 =  0;
	m_sockOrderSeed  =  0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ����Ʈ�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CListenSocket::ReleaseSocketList()
{

	for(int i=0; i < MAX_SOCKET; ++i) {

		if( m_SocketList[i] ) {

			m_SocketList[i]->SockClose();

			delete m_SocketList[i];
		}
	}

	if( m_SocketList ) delete m_SocketList;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��� ���� �۾��� ó�� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CListenSocket::ProcessPoll()
{
	
	EnterCriticalSection(&m_csSocketList);
		
	
	for(int i=0; i < MAX_SOCKET; ++i)
	{			
		if( getSocket(i)->processAcceptSock() < 0 ) { 
			
			/* �ݴ� �۾��� ���·� ���� �Ѵ� */
			closingSocket( getSocket(i)->getSockCompletionKey() );		
		}
	}
	
	_CallBack_ProcessPoll();

	LeaveCriticalSection(&m_csSocketList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ϵ��� ���� ��û�� ó���Ѵ� 
//--------------------------------------------------------------------------------------------
void CListenSocket::ConnectPoll()
{
	SOCKET		 socket;			
	SOCKADDR_IN  sockAddrIn;		

	int	sockAddrSize = sizeof(sockAddrIn);	
	

	WSANETWORKEVENTS event;				
	while(!m_bShutdown)
	{					


		// �̺�Ʈ ��� 
		DWORD ret = WSAWaitForMultipleEvents(2, m_wsaConnectEvent, FALSE, WSA_INFINITE, FALSE);	
		
		if(ret == WSA_WAIT_FAILED)
		{ 
			printf("WSAWaitForMultipleEvents() fail!\n");			
			break;
		}			
		
		

		// ���� �̺�Ʈ �߻� 
		else if(ret == WSA_WAIT_EVENT_0) 
		{

			WSAResetEvent(m_wsaConnectEvent[0]);
			WSAEnumNetworkEvents(m_ListenSocket, m_wsaConnectEvent[0], &event);


			if ((event.lNetworkEvents & FD_ACCEPT) == FD_ACCEPT)
			{ 								
				if( INVALID_SOCKET == (socket = accept(m_ListenSocket, (SOCKADDR*)&sockAddrIn, &sockAddrSize)) )
				{ 			
					if(WSAGetLastError() != WSAEWOULDBLOCK)											 	
					{ 
						printf("accept() fail!\n");
						break;
					}								
				}
				else
				{			
					// ����ó�� 					
					occurConnect(socket, &sockAddrIn);
				}
			}
		}

		// ���� �̺�Ʈ �߻� 
		else
		{
			// ���� ���� 
			WSAResetEvent(m_wsaConnectEvent[1]);			
			break;
		}
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �������κ����� ����¿Ϸ� ��û�� ó���Ѵ�   
//--------------------------------------------------------------------------------------------
void CListenSocket::IOCompletionPoll()
{
	
	DWORD		  completionKey			   = 0;	
	DWORD		  numberOfBytesTransferred = 0;
	LPOVERLAPPED  lpOverlapped  = NULL;					

	while(!m_bShutdown)
	{				
		if(FALSE == GetQueuedCompletionStatus( m_hCompletionPort, 
			                                   &numberOfBytesTransferred,
											   &completionKey,
											   (LPOVERLAPPED*)&lpOverlapped,
											   INFINITE) )
		{ 
			if(lpOverlapped == NULL)
			{
				if(GetLastError()!=ERROR_IO_PENDING)
				{			
					printf("GetQueuedCompletionStatus() fail!\n");
					return; 
				}
			}
		}
		
		// ���� ���� 
		if( completionKey == -1 ) break;

		// ����� �Ϸ� ó��		
		occurIOCompletion(completionKey	, numberOfBytesTransferred, lpOverlapped);
	}	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �޾Ƶ��� ������ ������ ����Ͽ� ���ο� ����Ű�� �߱� �Ѵ� 
//--------------------------------------------------------------------------------------------
DWORD CListenSocket::calcSockOrderKey()
{

	return m_sockOrderSeed > 500000 ? m_sockOrderSeed = 1 : ++m_sockOrderSeed;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : SOCKADDR_IN ����ü�� ����� 
//--------------------------------------------------------------------------------------------
SOCKADDR_IN CListenSocket::makeSockAddrIn(char* ip, int port)
{

	SOCKADDR_IN sockAddrIn;

	memset(&sockAddrIn, 0, sizeof(sockAddrIn));	
	sockAddrIn.sin_family		= AF_INET;
	sockAddrIn.sin_addr.s_addr  = INADDR_ANY; //inet_addr(ip); // INADDR_ANY or ip
	sockAddrIn.sin_port			= htons(port);	

	return sockAddrIn;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ���ο� �ڵ�(����)�� IOCP�� ������Ų��
//--------------------------------------------------------------------------------------------
bool CListenSocket::associateHandleToIOCompetionPort(HANDLE handle, DWORD key)
{		

	if( (SOCKET)handle == SOCKET_ERROR ) return FALSE;

	HANDLE completionPort = CreateIoCompletionPort(handle, m_hCompletionPort, key, 0);

	if( completionPort != NULL )	
	{ 	
		m_hCompletionPort = completionPort;
		return TRUE;
	}
	else
	{						
		OutputDebugString("CreateIoCompletionPort() fail!\n");							
	}

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ο� ������ �߻����� ��� 
//--------------------------------------------------------------------------------------------
void CListenSocket::occurConnect(SOCKET socket, SOCKADDR_IN *sockAddrIn)
{				
	EnterCriticalSection(&m_csSocketList);

	int key = 0;

	if( (key = addSocket(socket, sockAddrIn)) == -1 ) 
	{
		::closesocket(socket);		
	}
	
	else if( associateHandleToIOCompetionPort((HANDLE)socket, key) == FALSE )
	{
		removeSocket(key);			
	}

	else if( getSocket(key)->Recv(0) == -1 )
	{				
		//==================================================================
		// CreateIOCompletionPort �� socket�� port�� ������������ ���� 
		// �ۼ��� �Ϸ� ��� ť���� �־��� ���°� �ƴϱ� ������ ���� ���� 
		// �� ������ �ش� 		
		//==================================================================
		removeSocket(key); //closingSocket(key);		
	}				

	LeaveCriticalSection(&m_csSocketList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ����� �Ϸᰡ �߻����� ��� 
//--------------------------------------------------------------------------------------------
void CListenSocket::occurIOCompletion( DWORD completionKey, 
									   DWORD numberOfBytesTransferred, 
									   LPOVERLAPPED lpOverlapped )
{
	
	int key = completionKey;

	if( key < 0 || key >= MAX_SOCKET )			return;
	if( getSocket(key)->isSockOpen() == FALSE ) return;
		
	

	// ���۷��� 0 �� ��� 
	if( numberOfBytesTransferred == 0 )
	{			
		removeSocket(key);						
		return;
	}
		

	
	sOverlappedEX* pOverlappedEX = (sOverlappedEX*)lpOverlapped;		
	
	// ���� �Ϸ� �߻� 
	if(pOverlappedEX->transferMode == 0) 
	{							
		if(getSocket(key)->Recv(numberOfBytesTransferred) < 0)
		{			
			removeSocket(key);
			return;
		}
	}		

	// �۽� �Ϸ� �߻� 
	else if(pOverlappedEX->transferMode == 1)
	{
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ����� ���ϵ��� ���� ���´� 
//--------------------------------------------------------------------------------------------
int	CListenSocket::getNumSocket()
{
	return m_numSocket;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ε����� �ش��ϴ� AcceptSocket�� ��� �´� 
//--------------------------------------------------------------------------------------------
CAcceptSocket* CListenSocket::getSocket(int index)
{
	if( index < 0 || index >= MAX_SOCKET ) return NULL;
	
	return m_SocketList[index];
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ο� AcceptSocket�� �߰� �Ѵ� 
//--------------------------------------------------------------------------------------------
int CListenSocket::addSocket(SOCKET socket, SOCKADDR_IN* sockAddrIn)
{
	EnterCriticalSection(&m_csSocketList);


	for(int i=0; i < MAX_SOCKET; ++i)
	{
		if( getSocket(i)->isSockOpen() == FALSE )			
		{					
			
			getSocket(i)->SockOpen( socket, sockAddrIn, calcSockOrderKey() );			
			++m_numSocket;
			
			printf("[%d][%s] New Socket Connected.. ( Total NumSocket: %d ) \n", m_ListenSocket, 
																				 getSocket(i)->getSockIP(),
																				 m_numSocket );
		    			
			LeaveCriticalSection(&m_csSocketList);
			return i;
		}
	}

	LeaveCriticalSection(&m_csSocketList);
	
	return -1;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : index�� �ش��ϴ� AcceptSocket�� �����Ѵ� 
//--------------------------------------------------------------------------------------------
void CListenSocket::removeSocket(int index)
{
	if(index < 0 && index >= MAX_SOCKET) return;
	
	EnterCriticalSection(&m_csSocketList);

	if(getSocket(index)->isSockOpen() == TRUE)
	{
		printf("[%d][%s] Socket Dissconnected.. ( Total NumSocket: %d ) \n", m_ListenSocket, 
																			 getSocket(index)->getSockIP(),
																			 m_numSocket - 1 );       
		getSocket(index)->SockClose();								
		--m_numSocket;		
	}

	LeaveCriticalSection(&m_csSocketList);
}






//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : index�� �ش��ϴ� AcceptSocket�� �ݱ��۾��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CListenSocket::closingSocket(int index)
{					
	EnterCriticalSection(&m_csSocketList);

	if( getSocket(index)->isSockOpen() == TRUE ) 
	{


		printf("[%d][%s] Socket Closing.. ( Total NumSocket: %d ) \n", m_ListenSocket, 
																	   getSocket(index)->getSockIP(),
																	   m_numSocket );
		getSocket(index)->SockClosing();
	}

	LeaveCriticalSection(&m_csSocketList);
}
