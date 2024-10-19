#include <GSL.h>
#include "OnlineWorld.h"
#include "OnlineClient.h"
#include "Client_LogIn.h"
#include "Utility.h"
#include <assert.h>

//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
OnlineClient::OnlineClient()
{
	ZeroMemory(cInBuffer, ON_MAX_IN_BUFFER);
	ZeroMemory(cOutBuffer, ON_MAX_OUT_BUFFER);
	ZeroMemory(&timeout, sizeof(timeval));
	siInBufferLen	=	0;
	siOutBufferLen	=	0;	
	ConnectOK		=	FALSE;
	m_RecvDataSize	= 0;
	m_BlackPigUse	= FALSE;
//	InitializeCriticalSection(m_csIn);
	InitializeCriticalSection(&m_csOut);
}

//----------------------------------------------------------------------------------------------------------------
//	 설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
OnlineClient::~OnlineClient()
{
	Close();
	FreeDynamicBuffer();
	if( !m_pMsgBuffer.empty() )		m_pMsgBuffer.clear();

	DeleteCriticalSection(&m_csOut);
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	클라이언트의 정보를 초기화 한다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::InitZero()
{
	ZeroMemory(cInBuffer, ON_MAX_IN_BUFFER);
	ZeroMemory(cOutBuffer, ON_MAX_OUT_BUFFER);
	siInBufferLen = siOutBufferLen = 0;	

	m_BlackPigUse = FALSE;

	if( !m_pMsgBuffer.empty() )
		m_pMsgBuffer.clear();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	서버에 접속한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::Init( cltOnlineWorld	*pOnlineWorld, char IpDomain[], SI32 siPort, BOOL fPassword)
{
	HOSTENT			*pHostEnt;
	CHAR			szIP[1024];
	int				ErrorCode;

	Close();
	ConnectOK = FALSE;

	pMyOnlineWorld	=	pOnlineWorld;
	if((IpDomain[0] >= '0') && (IpDomain[0] <= '9'))
	{
		// IP
		strcpy(szIP, IpDomain);
	}
	else
	{
		// Domain
		pHostEnt = gethostbyname(IpDomain);
		if(pHostEnt)
		{
			sprintf(szIP, "%i.%i.%i.%i",	BYTE(pHostEnt->h_addr_list[0][0]),BYTE(pHostEnt->h_addr_list[0][1]),
											BYTE(pHostEnt->h_addr_list[0][2]),BYTE(pHostEnt->h_addr_list[0][3]) );
		}
		else
		{
			strcpy(szIP, IpDomain);
		}
	}

	// socket 구조체를 초기화한다. 
	memset(&Server,0,sizeof(Server));

	// Socket 구조체를 설정한다.
	Server.sin_family      = AF_INET;          
	Server.sin_addr.s_addr = inet_addr(szIP);           // Socket구조체에 IP를 설정한다.
	Server.sin_port        = htons(siPort);                 // Socket구조체에 Port번호를 설정한다. 

    if ( (Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
    {
		ErrorCode	=	WSAGetLastError();
		//clGrp.Error("Socket Error", "%d", ErrorCode);
        return FALSE;
    }

	int bufsize = 240 * 1024;
	
	//setsockopt( Client, SOL_SOCKET, SO_RCVBUF, (const char *)bufsize, sizeof( bufsize ) );
	//setsockopt( Client, SOL_SOCKET, SO_SNDBUF, (const char *)bufsize, sizeof( bufsize ) );

	m_RecvDataSize = 0;
    if ( connect(Client, (struct sockaddr *)&Server, sizeof(struct sockaddr_in)) == SOCKET_ERROR )
    {
		if ( connect(Client, (struct sockaddr *)&Server, sizeof(struct sockaddr_in)) == SOCKET_ERROR )
		{
			if ( connect(Client, (struct sockaddr *)&Server, sizeof(struct sockaddr_in)) == SOCKET_ERROR )
			{
				ErrorCode	=	WSAGetLastError();
				//clGrp.Error("Connect Error", "%d", ErrorCode);
				return FALSE;
			}
		}
    }

	ConnectOK = TRUE;

	m_BlackPigUse = fPassword;

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소켓을 초기화 한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::InitSocket()
{
	WORD wVersion;
    WSADATA wsaData;
    int err;

    wVersion = MAKEWORD(2, 2);
    err = WSAStartup(wVersion, &wsaData);

    if ( err != 0 )
    {
        return FALSE;
    }

    if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )	// 버젼 체크
    {	
        return FALSE;
    }

    return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	접속 종료.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::Close()
{
	if ( ConnectOK == FALSE ) return;

	if ( closesocket(Client) != SOCKET_ERROR )
	{
		ConnectOK = FALSE;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	버퍼에서 해당 개수 만큼 메시지를 읽어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineClient::Read(SI32 siBytesToRead)
{
	UI16 uiCount=0;
	//unsigned int iMask[2];
	SI32 siByteCount=0;
	UI16 iTest=0;

	if ( ConnectOK == FALSE ) return FALSE;

	// 입력버퍼의 크기를 초과하는지 검사 
	if ( (siBytesToRead+siInBufferLen) > ON_MAX_IN_BUFFER )
	{
//		SendLogOut();
		return -1;
	}

	// socket으로 부터 입력버퍼에 지정한 버퍼크기만큼 읽어온다.
	siByteCount=::recv(Client,&cInBuffer[siInBufferLen],siBytesToRead,0); 
	
	// recv 함수에서 에러 발생 
	if ( siByteCount < 0 )
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
		{
			Close();
		}

//		SendLogOut();
		return 0;
	}

	// 쪼개진 데이타가 왔다면 처리를 무시한다.
//	if ( siByteCount != siBytesToRead )
//		return 0;

	siInBufferLen += siByteCount;

	m_RecvDataSize += siByteCount;

	return siByteCount;
}
 
//----------------------------------------------------------------------------------------------------------------
//	설명	:	출력 버퍼 데이타를 보낸다.
//----------------------------------------------------------------------------------------------------------------
//	CRITICAL_SECTION	m_csOut, m_csOut;

BOOL	OnlineClient::Write(const VOID * cMsg, UI16 iBytes, BOOL fPW)
{
	char	cPackMsg[ON_MAX_OUT_BUFFER];
	SI16	siPackSize = 0;

	if ( ConnectOK == FALSE ) return FALSE;

	// 보낼 메세지를 패키징한다.
	EnterCriticalSection(&m_csOut);
	PackMessage( (char*)cMsg, (SI16)iBytes, cPackMsg, &siPackSize);

	if(siOutBufferLen + siPackSize == 35 )
	{
		int a  =1;
	}

	if ( siOutBufferLen + siPackSize > ON_MAX_OUT_BUFFER )
	{
//		SendLogOut();
		LeaveCriticalSection(&m_csOut);
		return FALSE;
	}

	if(m_BlackPigUse && fPW)
	{
		char	cPackMsg2[ON_MAX_OUT_BUFFER];

		m_BlackPig.Encode((BYTE*)cPackMsg, (BYTE*)cPackMsg2, m_dwRoundKey);
		
		// 암호화된 패킷 사이즈로 변경해준다.
		// actdoll (2004/12/20 14:02) : 주석 처리
//		siPackSize = m_BlackPig.GetPackSize();

		memcpy((void *)&cOutBuffer[siOutBufferLen], cPackMsg2, siPackSize);
		siOutBufferLen += siPackSize;
	}
	else
	{
		memcpy((void *)&cOutBuffer[siOutBufferLen], cPackMsg, siPackSize);
		siOutBufferLen += siPackSize;
	}
	LeaveCriticalSection(&m_csOut);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	수신 버퍼에 데이타가 들어왔는지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::Readable()
{
	ResetSets();

	select(Client, &fdPlayer, NULL, &fdError, &timeout);

	if ( FD_ISSET(Client, &fdError) )
	{
//		SendLogOut();
		return 2;
	}
	else if ( FD_ISSET(Client, &fdPlayer) )
	{
		return TRUE;
	}

	
	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	송신 버퍼에 데이타를 보낼 수 있는지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineClient::Writeable()
{
	ResetSets();
	select(Client, NULL, &fdPlayer, &fdError, &timeout);
	if ( FD_ISSET(Client, &fdError) )
	{
//		SendLogOut();
		return 2;
	}
	else if ( FD_ISSET(Client, &fdPlayer) )
		return TRUE;

	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	송신 버퍼에 있는 내용을 서버에 보낸다.
//----------------------------------------------------------------------------------------------------------------	
BOOL	OnlineClient::FlushOutBuffer()
{
	SI32 BytesSent = 0;

	if ( siOutBufferLen > 0 )
	{
		BytesSent = send(Client, cOutBuffer, siOutBufferLen, 0);
		if(BytesSent > 100)
		{
			int a = 0;
		}

		if(BytesSent > 0)
		{
			ZeroMemory( cOutBuffer, BytesSent );

			memmove((void*)cOutBuffer, (void*)&cOutBuffer[BytesSent], siOutBufferLen-BytesSent);
			siOutBufferLen -= BytesSent;
		}
		else if ( BytesSent == SOCKET_ERROR )
		{	
			if ( ConnectOK == TRUE )
				Close();
			return FALSE;
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	수신 버퍼에 있는 내용을 전부 지운다.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::EraseInBuffer(UI16 uiBytes)
{
	if ( (uiBytes == 0) || (uiBytes > siInBufferLen) )
	{
		siInBufferLen = 0;
	}
	else
	{
		memmove((void*)cInBuffer,(void *)&cInBuffer[uiBytes],siInBufferLen-uiBytes);
		siInBufferLen -= uiBytes;
	}
	return;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::ResetSets()
{
	FD_ZERO(&fdError);
	FD_ZERO(&fdPlayer);
	FD_SET(Client, &fdError);
	FD_SET(Client, &fdPlayer);
}

VOID	OnlineClient::AddMsgBuffer( DWORD dwMsgType, VOID *pData, SI32 siSize )
{
	if( dwMsgType == ON_HEARTBEAT )		return;	

	siMsgLengh	=	siSize;

	char szTemp[100];
	sprintf( szTemp, "%d\n", dwMsgType );
	OutputDebugString( szTemp );

	OnlineTempMsg	TempMsg;
	memcpy( TempMsg.Data, pData, siSize );
	efficientAddOrUpdata( m_pMsgBuffer, dwMsgType, TempMsg );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	서버에서 받은 메세지를 리턴해 준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BYTE*	OnlineClient::GetMsgBuffer(DWORD dwMsgType)
{
	pair< MsgBuffer::iterator, MsgBuffer::iterator > Find = m_pMsgBuffer.equal_range( dwMsgType );

	if( Find.first->first == dwMsgType )
	{
		memcpy( ReMsg, Find.first->second.Data, sizeof( Find.first->second.Data ) );
		m_pMsgBuffer.erase( Find.first );
		return ReMsg;
	}

	return NULL;
}

UI16	OnlineClient::GetMsgIndex()
{
	return (UI16)m_pMsgBuffer.begin()->first;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	동적 버퍼를 해제한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::FreeDynamicBuffer()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	내 ID를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::GetID(CHAR *pszID)
{
	strcpy(pszID, szUserID);	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	보낼 메세지를 패키징 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineClient::PackMessage(char *cSrcMsg, SI16 siSrcSize, char *cPackMsg, SI16 *siPackSize)
{
	SI32 i			= siSrcSize;
	char cCheckSum	= 0;

	if(siSrcSize > ON_MAX_OUT_BUFFER)
	{
		clGrp.Error("","보내려는 메세지가 버퍼의 크기보다 큽니다. siSrcSize=%d, buffer=%d", siSrcSize, ON_MAX_OUT_BUFFER);
		return;
	}

	// CheckSum을 구한다.
	for(i=0; i< siSrcSize; i++)
	{
		cCheckSum ^= (char)cSrcMsg[i];
	}
	*siPackSize = siSrcSize+ON_PACK_HEADER_SIZE;

	//////////////////////////////////////////////////
	// 패키징 메세지를 만든다.
	//////////////////////////////////////////////////
	// 전체 메세지 크기
	cPackMsg[0] = LOBYTE(*siPackSize);
	cPackMsg[1] = HIBYTE(*siPackSize);
	cPackMsg[2] = cCheckSum;

	memcpy(&cPackMsg[ON_PACK_HEADER_SIZE], cSrcMsg, siSrcSize);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 받은 패킹된 메세지를 푼다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool	OnlineClient::UnpackMessage()
{
	SI32	i			= 0;
	char	cCheckSum	= 0;
	SI16	siTempSize	= 0;

	// 변수의 초기화.
	siMsgLengh = 0;
	memset(cMsg, 0, sizeof(cMsg));

	if(siInBufferLen>ON_PACK_HEADER_SIZE)
	{
		siTempSize = MAKEWORD(cInBuffer[0], cInBuffer[1]);
		// 필요한 패킷이 모두 도착한 경우에만 처리한다.
		if(siInBufferLen>=siTempSize && siTempSize>ON_PACK_HEADER_SIZE)
		{
			// 받은 메세지가 버퍼의 크기보다 크면
			if(siTempSize > ON_MAX_IN_BUFFER)
			{
				clGrp.Error("","받은메세지의 크기(%d)가 버퍼의 크기(%d)보다 큽니다.", siTempSize, ON_MAX_IN_BUFFER);
				EraseInBuffer(siTempSize);
				return FALSE;
			}

			// CheckSum을 무시
			siMsgLengh = siTempSize-ON_PACK_HEADER_SIZE;
			memcpy(cMsg, &cInBuffer[ON_PACK_HEADER_SIZE], siMsgLengh);
			EraseInBuffer(siTempSize);
			return TRUE;
			/*
			/////////////////////////////////////////////////
			// CheckSum을 비교한다.
			/////////////////////////////////////////////////
			for(i=ON_PACK_HEADER_SIZE; i<siTempSize; i++) 
			{ 
				cCheckSum ^= cInBuffer[i];
			}
			if(cInBuffer[2] == cCheckSum)
			{
				siMsgLengh = siTempSize-ON_PACK_HEADER_SIZE;
				memcpy(cMsg, &cInBuffer[ON_PACK_HEADER_SIZE], siMsgLengh);
				EraseInBuffer(siTempSize);
				return TRUE;
			}
			else
			{
				// CheckSum값이 틀리면 버린다.
				clGrp.Error("","받은메세지의 Checksum이 틀리기 때문에 버퍼를 비웁니다. Command : [%2x][%2x][%2x][%2x][%2x][%2x][%2x][%2x][%2x][%2x]", (UI08)cInBuffer[0],(UI08)cInBuffer[1],(UI08)cInBuffer[2],(UI08)cInBuffer[3],(UI08)cInBuffer[4],(UI08)cInBuffer[5],(UI08)cInBuffer[6],(UI08)cInBuffer[7],(UI08)cInBuffer[8],(UI08)cInBuffer[9]);
				clGrp.Error("","받은 체크섬 : [%2x], 계산한 체크섬 : [%2x]", cInBuffer[2], cCheckSum);
				EraseInBuffer(0);
			}
			*/
		}
		// siTempSize=0이면 메세지가 완전히 변질된 것이므로 모든 메세지를 삭제한다.
		else if(siTempSize <= PACK_HEADER_SIZE)
		{
//			clGrp.Error("","받은메세지가 변질되었기 때문에 버퍼를 비웁니다.");
			EraseInBuffer(0);
		}
	}
	return FALSE;
}

