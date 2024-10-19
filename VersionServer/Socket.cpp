// Socket.cpp: implementation of the CSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "Socket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
	생성자.
*/
CSocket::CSocket()
{

}

CSocket::~CSocket()
{

}

VOID CSocket::StartWinsock()
{
	WSADATA	wsaData;
	int		err;
	
	err = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if( err != 0 )
	{
		cout << "윈속2 Version 2.2 초기화 실패" << endl;
		return;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
	{
		cout << "윈속2 Version 2.2 초기화 실패" << endl;
		WSACleanup();
		return; 
	}
}

VOID CSocket::EndWinsock()
{
	WSACleanup();
}

VOID CSocket::InitSocket(int socktype, char *cip, WORD port)
{
	int type;
	int protocol;

	if(socktype == TCP)
	{
		type		= SOCK_STREAM;
		protocol	= IPPROTO_TCP;
	}
	else if(socktype == UDP)
	{
		type		= SOCK_DGRAM;
		protocol	= IPPROTO_UDP;
	}
	else
	{
		return;
	}

	if(cip == NULL)
		m_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		m_sockaddr.sin_addr.s_addr = inet_addr(cip);

	m_sockaddr.sin_port				= htons(port);
	m_sockaddr.sin_family			= AF_INET;

	// 소켓 생성.
	m_sock = socket(AF_INET, type, protocol);
	if(m_sock == INVALID_SOCKET)
	{
		cout << "소켓 생성 실패." << endl;
		ViewErrMsg(WSAGetLastError());
	}
}

VOID CSocket::Bind()
{
	if(bind(m_sock, (SOCKADDR*)&m_sockaddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		ViewErrMsg(WSAGetLastError());
	}
}

VOID CSocket::Listen(int backlog)
{
	if(listen(m_sock, backlog) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		cout << "Listen 실패 errcode:" << err << endl;
	}
}

SOCKET CSocket::Accept()
{
	SOCKET	new_socket	= INVALID_SOCKET;
	int		addrlen	= sizeof(SOCKADDR);

	new_socket = accept(m_sock, (SOCKADDR*)&m_sockaddr, &addrlen);
	return new_socket;
}

BOOL CSocket::Connect()
{
	if(connect(m_sock, (SOCKADDR*)&m_sockaddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		ViewErrMsg(WSAGetLastError());
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

VOID CSocket::ViewErrMsg(int errcode)
{
	// 만약 error 가 IO_PENDING 이라면 정상적인 경우이므로 넘어간다.
	if(errcode == WSA_IO_PENDING)
	{
		return;
	}

	/////////////////////////////////////////
	// 임시로...
	/////////////////////////////////////////
	cout << "ErrroCode : "<< errcode << endl;
	return;
	/////////////////////////////////////////

	char	cMsgBuf[1024];
	char	ErrName[50];

	ZeroMemory(cMsgBuf, 1024);

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errcode,
		MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN), // 한글
		(LPTSTR) &cMsgBuf,
		0,
		NULL 
	);

	switch (errcode)
	{
	case WSAEACCES:
		wsprintf(ErrName, "WSAEACCES");
		break;
	case WSAEADDRINUSE:
		wsprintf(ErrName, "WSAEADDRINUSE");
		break;
	case WSAEADDRNOTAVAIL:
		wsprintf(ErrName, "WSAEADDRNOTAVAIL");
		break;
	case WSAEAFNOSUPPORT:
		wsprintf(ErrName, "WSAEAFNOSUPPORT");
		break;
	case WSAEALREADY:
		wsprintf(ErrName, "WSAEALREADY");
		break;
	case WSAECONNABORTED:
		wsprintf(ErrName, "WSAECONNABORTED");
		break;
	case WSAECONNREFUSED:
		wsprintf(ErrName, "WSAECONNREFUSED");
		break;
	case WSAECONNRESET:
		wsprintf(ErrName, "WSAECONNRESET");
		break;
	case WSAEDESTADDRREQ:
		wsprintf(ErrName, "WSAEDESTADDRREQ");
		break;
	case WSAEFAULT:
		wsprintf(ErrName, "WSAEFAULT");
		break;
	case WSAEHOSTDOWN:
		wsprintf(ErrName, "WSAEHOSTDOWN");
		break;
	case WSAEHOSTUNREACH:
		wsprintf(ErrName, "WSAEHOSTUNREACH");
		break;
	case WSAEINPROGRESS:
		wsprintf(ErrName, "WSAEINPROGRESS");
		break;
	case WSAEINTR:
		wsprintf(ErrName, "WSAEINTR");
		break;
	case WSAEINVAL:
		wsprintf(ErrName, "WSAEINVAL");
		break;
	case WSAEISCONN:
		wsprintf(ErrName, "WSAEISCONN");
		break;
	case WSAEMFILE:
		wsprintf(ErrName, "WSAEMFILE");
		break;
	case WSAEMSGSIZE:
		wsprintf(ErrName, "WSAEMSGSIZE");
		break;
	case WSAENETDOWN:
		wsprintf(ErrName, "WSAENETDOWN");
		break;
	case WSAENETRESET:
		wsprintf(ErrName, "WSAENETRESET");
		break;
	case WSAENETUNREACH:
		wsprintf(ErrName, "WSAENETUNREACH");
		break;
	case WSAENOBUFS:
		wsprintf(ErrName, "WSAENOBUFS");
		break;
	case WSAENOPROTOOPT:
		wsprintf(ErrName, "WSAENOPROTOOPT");
		break;
	case WSAENOTCONN:
		wsprintf(ErrName, "WSAENOTCONN");
		break;
	case WSAENOTSOCK:
		wsprintf(ErrName, "WSAENOTSOCK");
		break;
	case WSAEOPNOTSUPP:
		wsprintf(ErrName, "WSAEOPNOTSUPP");
		break;
	case WSAEPFNOSUPPORT:
		wsprintf(ErrName, "WSAEPFNOSUPPORT");
		break;
	case WSAEPROCLIM:
		wsprintf(ErrName, "WSAEPROCLIM");
		break;
	case WSAEPROTONOSUPPORT:
		wsprintf(ErrName, "WSAEPROTONOSUPPORT");
		break;
	case WSAEPROTOTYPE:
		wsprintf(ErrName, "WSAEPROTOTYPE");
		break;
	case WSAESHUTDOWN:
		wsprintf(ErrName, "WSAESHUTDOWN");
		break;
	case WSAESOCKTNOSUPPORT:
		wsprintf(ErrName, "WSAESOCKTNOSUPPORT");
		break;
	case WSAETIMEDOUT:
		wsprintf(ErrName, "WSAETIMEDOUT");
		break;
	case WSATYPE_NOT_FOUND:
		wsprintf(ErrName, "WSATYPE_NOT_FOUND");
		break;
	case WSAEWOULDBLOCK:
		wsprintf(ErrName, "WSAEWOULDBLOCK");
		break;
	case WSAHOST_NOT_FOUND:
		wsprintf(ErrName, "WSAHOST_NOT_FOUND");
		break;
	case WSA_INVALID_HANDLE:
		wsprintf(ErrName, "WSA_INVALID_HANDLE");
		break;
	case WSA_INVALID_PARAMETER:
		wsprintf(ErrName, "WSA_INVALID_PARAMETER");
		break;
//	case WSAINVALIDPROCTABLE:
//		wsprintf(ErrName, "WSAINVALIDPROCTABLE");
//		break;
//	case WSAINVALIDPROVIDER:
//		wsprintf(ErrName, "WSAINVALIDPROVIDER");
//		break;
	case WSA_IO_INCOMPLETE:
		wsprintf(ErrName, "WSA_IO_INCOMPLETE");
		break;
	case WSA_IO_PENDING:
		wsprintf(ErrName, "WSA_IO_PENDING");
		break;
	case WSA_NOT_ENOUGH_MEMORY:
		wsprintf(ErrName, "WSA_NOT_ENOUGH_MEMORY");
		break;
	case WSANOTINITIALISED:
		wsprintf(ErrName, "WSANOTINITIALISED");
		break;
	case WSANO_DATA:
		wsprintf(ErrName, "WSANO_DATA");
		break;
	case WSANO_RECOVERY:
		wsprintf(ErrName, "WSANO_RECOVERY");
		break;
//	case WSAPROVIDERFAILEDINIT:
//		wsprintf(ErrName, "WSAPROVIDERFAILEDINIT");
//		break;
	case WSASYSCALLFAILURE:
		wsprintf(ErrName, "WSASYSCALLFAILURE");
		break;
	case WSASYSNOTREADY:
		wsprintf(ErrName, "WSASYSNOTREADY");
		break;
	case WSATRY_AGAIN:
		wsprintf(ErrName, "WSATRY_AGAIN");
		break;
	case WSAVERNOTSUPPORTED:
		wsprintf(ErrName, "WSAVERNOTSUPPORTED");
		break;
	case WSAEDISCON:
		wsprintf(ErrName, "WSAEDISCON");
		break;
	case WSA_OPERATION_ABORTED:
		wsprintf(ErrName, "WSA_OPERATION_ABORTED");
		break;
	}

	cout << "WINSOCK ERROR : " << ErrName << " ["<< errcode << "] " << (char*)cMsgBuf << endl;
}

int CSocket::Send(VOID *cBuf, int siBufsize)
{
	int siSendBytes	= 0;

	siSendBytes = send(m_sock, (char*)cBuf, siBufsize, 0);
	if(siSendBytes == SOCKET_ERROR)
	{
		cout << "Send 실패" << endl;
		ViewErrMsg(WSAGetLastError());
	}

	return siSendBytes;
}

int CSocket::Recv(VOID *cBuf, int siBufsize)
{
	int siRecvBytes = 0;

	siRecvBytes = recv(m_sock, (char*)cBuf, siBufsize, 0);
	if(siRecvBytes == SOCKET_ERROR)
	{
		cout << "Recv 실패" << endl;
		ViewErrMsg(WSAGetLastError());
		return 0;
	}
	
	return siRecvBytes;
}

VOID CSocket::ReleaseSocket()
{
	closesocket(m_sock);
}
