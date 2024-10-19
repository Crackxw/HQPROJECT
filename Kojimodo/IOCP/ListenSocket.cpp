#include <winsock2.h>
#include <stdio.h>

#include "AcceptSocket.h"
#include "ListenSocket.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
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
//	Desc : 소 멸 자 
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
//	Desc : 접속을 처리하는 루프, 입출력을 처리하는 루프, 메인처리루프를 중지 시킨다 
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
//	Desc : 소켓 리스트를 생성 한다 
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
//	Desc : 소켓 리스트를 해제 한다 
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
//	Desc : 소켓의 모든 메인 작업을 처리 한다 
//--------------------------------------------------------------------------------------------
void CListenSocket::ProcessPoll()
{
	
	EnterCriticalSection(&m_csSocketList);
		
	
	for(int i=0; i < MAX_SOCKET; ++i)
	{			
		if( getSocket(i)->processAcceptSock() < 0 ) { 
			
			/* 닫는 작업중 상태로 설정 한다 */
			closingSocket( getSocket(i)->getSockCompletionKey() );		
		}
	}
	
	_CallBack_ProcessPoll();

	LeaveCriticalSection(&m_csSocketList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓들의 접속 요청을 처리한다 
//--------------------------------------------------------------------------------------------
void CListenSocket::ConnectPoll()
{
	SOCKET		 socket;			
	SOCKADDR_IN  sockAddrIn;		

	int	sockAddrSize = sizeof(sockAddrIn);	
	

	WSANETWORKEVENTS event;				
	while(!m_bShutdown)
	{					


		// 이벤트 대기 
		DWORD ret = WSAWaitForMultipleEvents(2, m_wsaConnectEvent, FALSE, WSA_INFINITE, FALSE);	
		
		if(ret == WSA_WAIT_FAILED)
		{ 
			printf("WSAWaitForMultipleEvents() fail!\n");			
			break;
		}			
		
		

		// 접속 이벤트 발생 
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
					// 접속처리 					
					occurConnect(socket, &sockAddrIn);
				}
			}
		}

		// 종료 이벤트 발생 
		else
		{
			// 루프 종료 
			WSAResetEvent(m_wsaConnectEvent[1]);			
			break;
		}
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓으로부터의 입출력완료 요청을 처리한다   
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
		
		// 루프 종료 
		if( completionKey == -1 ) break;

		// 입출력 완료 처리		
		occurIOCompletion(completionKey	, numberOfBytesTransferred, lpOverlapped);
	}	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 받아들인 소켓의 순서를 계산하여 새로운 순서키를 발급 한다 
//--------------------------------------------------------------------------------------------
DWORD CListenSocket::calcSockOrderKey()
{

	return m_sockOrderSeed > 500000 ? m_sockOrderSeed = 1 : ++m_sockOrderSeed;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : SOCKADDR_IN 구조체를 만든다 
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
//	Desc : 관리할 새로운 핸들(소켓)을 IOCP와 연관시킨다
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
//	Desc : 새로운 연결이 발생했을 경우 
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
		// CreateIOCompletionPort 로 socket과 port를 연관시켰지만 아직 
		// 송수신 완료 대기 큐에는 넣어진 상태가 아니기 때문에 직접 소켓 
		// 을 제거해 준다 		
		//==================================================================
		removeSocket(key); //closingSocket(key);		
	}				

	LeaveCriticalSection(&m_csSocketList);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 입출력 완료가 발생했을 경우 
//--------------------------------------------------------------------------------------------
void CListenSocket::occurIOCompletion( DWORD completionKey, 
									   DWORD numberOfBytesTransferred, 
									   LPOVERLAPPED lpOverlapped )
{
	
	int key = completionKey;

	if( key < 0 || key >= MAX_SOCKET )			return;
	if( getSocket(key)->isSockOpen() == FALSE ) return;
		
	

	// 전송량이 0 일 경우 
	if( numberOfBytesTransferred == 0 )
	{			
		removeSocket(key);						
		return;
	}
		

	
	sOverlappedEX* pOverlappedEX = (sOverlappedEX*)lpOverlapped;		
	
	// 수신 완료 발생 
	if(pOverlappedEX->transferMode == 0) 
	{							
		if(getSocket(key)->Recv(numberOfBytesTransferred) < 0)
		{			
			removeSocket(key);
			return;
		}
	}		

	// 송신 완료 발생 
	else if(pOverlappedEX->transferMode == 1)
	{
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 연결된 소켓들의 수를 얻어온다 
//--------------------------------------------------------------------------------------------
int	CListenSocket::getNumSocket()
{
	return m_numSocket;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 인덱스에 해당하는 AcceptSocket을 얻어 온다 
//--------------------------------------------------------------------------------------------
CAcceptSocket* CListenSocket::getSocket(int index)
{
	if( index < 0 || index >= MAX_SOCKET ) return NULL;
	
	return m_SocketList[index];
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 새로운 AcceptSocket을 추가 한다 
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
//	Desc : index에 해당하는 AcceptSocket을 제거한다 
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
//	Desc : index에 해당하는 AcceptSocket의 닫기작업을 시작 한다 
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
