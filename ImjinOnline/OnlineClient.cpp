#include <GSL.h>
#include "OnlineWorld.h"
#include "OnlineClient.h"
#include "Client_LogIn.h"
#include "Utility.h"
#include <assert.h>

//----------------------------------------------------------------------------------------------------------------
//	����	:	������.
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
//	 ����	:	�Ҹ���.
//----------------------------------------------------------------------------------------------------------------
OnlineClient::~OnlineClient()
{
	Close();
	FreeDynamicBuffer();
	if( !m_pMsgBuffer.empty() )		m_pMsgBuffer.clear();

	DeleteCriticalSection(&m_csOut);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	Ŭ���̾�Ʈ�� ������ �ʱ�ȭ �Ѵ�.
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
//	����	:	������ �����Ѵ�.
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

	// socket ����ü�� �ʱ�ȭ�Ѵ�. 
	memset(&Server,0,sizeof(Server));

	// Socket ����ü�� �����Ѵ�.
	Server.sin_family      = AF_INET;          
	Server.sin_addr.s_addr = inet_addr(szIP);           // Socket����ü�� IP�� �����Ѵ�.
	Server.sin_port        = htons(siPort);                 // Socket����ü�� Port��ȣ�� �����Ѵ�. 

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
//	����	:	������ �ʱ�ȭ �Ѵ�.
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

    if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )	// ���� üũ
    {	
        return FALSE;
    }

    return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� ����.
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
//	����	:	���ۿ��� �ش� ���� ��ŭ �޽����� �о�´�.
//----------------------------------------------------------------------------------------------------------------
SI16	OnlineClient::Read(SI32 siBytesToRead)
{
	UI16 uiCount=0;
	//unsigned int iMask[2];
	SI32 siByteCount=0;
	UI16 iTest=0;

	if ( ConnectOK == FALSE ) return FALSE;

	// �Է¹����� ũ�⸦ �ʰ��ϴ��� �˻� 
	if ( (siBytesToRead+siInBufferLen) > ON_MAX_IN_BUFFER )
	{
//		SendLogOut();
		return -1;
	}

	// socket���� ���� �Է¹��ۿ� ������ ����ũ�⸸ŭ �о�´�.
	siByteCount=::recv(Client,&cInBuffer[siInBufferLen],siBytesToRead,0); 
	
	// recv �Լ����� ���� �߻� 
	if ( siByteCount < 0 )
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
		{
			Close();
		}

//		SendLogOut();
		return 0;
	}

	// �ɰ��� ����Ÿ�� �Դٸ� ó���� �����Ѵ�.
//	if ( siByteCount != siBytesToRead )
//		return 0;

	siInBufferLen += siByteCount;

	m_RecvDataSize += siByteCount;

	return siByteCount;
}
 
//----------------------------------------------------------------------------------------------------------------
//	����	:	��� ���� ����Ÿ�� ������.
//----------------------------------------------------------------------------------------------------------------
//	CRITICAL_SECTION	m_csOut, m_csOut;

BOOL	OnlineClient::Write(const VOID * cMsg, UI16 iBytes, BOOL fPW)
{
	char	cPackMsg[ON_MAX_OUT_BUFFER];
	SI16	siPackSize = 0;

	if ( ConnectOK == FALSE ) return FALSE;

	// ���� �޼����� ��Ű¡�Ѵ�.
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
		
		// ��ȣȭ�� ��Ŷ ������� �������ش�.
		// actdoll (2004/12/20 14:02) : �ּ� ó��
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
//	����	:	���� ���ۿ� ����Ÿ�� ���Դ��� �˾ƿ´�.
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
//	����	:	�۽� ���ۿ� ����Ÿ�� ���� �� �ִ��� �˾ƿ´�.
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
//	����	:	�۽� ���ۿ� �ִ� ������ ������ ������.
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
//	����	:	���� ���ۿ� �ִ� ������ ���� �����.
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
//	����	:	
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
//	����	:	�������� ���� �޼����� ������ �ش�.
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
//	����	:	���� ���۸� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::FreeDynamicBuffer()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�� ID�� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineClient::GetID(CHAR *pszID)
{
	strcpy(pszID, szUserID);	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �޼����� ��Ű¡ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineClient::PackMessage(char *cSrcMsg, SI16 siSrcSize, char *cPackMsg, SI16 *siPackSize)
{
	SI32 i			= siSrcSize;
	char cCheckSum	= 0;

	if(siSrcSize > ON_MAX_OUT_BUFFER)
	{
		clGrp.Error("","�������� �޼����� ������ ũ�⺸�� Ů�ϴ�. siSrcSize=%d, buffer=%d", siSrcSize, ON_MAX_OUT_BUFFER);
		return;
	}

	// CheckSum�� ���Ѵ�.
	for(i=0; i< siSrcSize; i++)
	{
		cCheckSum ^= (char)cSrcMsg[i];
	}
	*siPackSize = siSrcSize+ON_PACK_HEADER_SIZE;

	//////////////////////////////////////////////////
	// ��Ű¡ �޼����� �����.
	//////////////////////////////////////////////////
	// ��ü �޼��� ũ��
	cPackMsg[0] = LOBYTE(*siPackSize);
	cPackMsg[1] = HIBYTE(*siPackSize);
	cPackMsg[2] = cCheckSum;

	memcpy(&cPackMsg[ON_PACK_HEADER_SIZE], cSrcMsg, siSrcSize);
}

//����������������������������������������������������������������������������
// ���� ��ŷ�� �޼����� Ǭ��.
//����������������������������������������������������������������������������
bool	OnlineClient::UnpackMessage()
{
	SI32	i			= 0;
	char	cCheckSum	= 0;
	SI16	siTempSize	= 0;

	// ������ �ʱ�ȭ.
	siMsgLengh = 0;
	memset(cMsg, 0, sizeof(cMsg));

	if(siInBufferLen>ON_PACK_HEADER_SIZE)
	{
		siTempSize = MAKEWORD(cInBuffer[0], cInBuffer[1]);
		// �ʿ��� ��Ŷ�� ��� ������ ��쿡�� ó���Ѵ�.
		if(siInBufferLen>=siTempSize && siTempSize>ON_PACK_HEADER_SIZE)
		{
			// ���� �޼����� ������ ũ�⺸�� ũ��
			if(siTempSize > ON_MAX_IN_BUFFER)
			{
				clGrp.Error("","�����޼����� ũ��(%d)�� ������ ũ��(%d)���� Ů�ϴ�.", siTempSize, ON_MAX_IN_BUFFER);
				EraseInBuffer(siTempSize);
				return FALSE;
			}

			// CheckSum�� ����
			siMsgLengh = siTempSize-ON_PACK_HEADER_SIZE;
			memcpy(cMsg, &cInBuffer[ON_PACK_HEADER_SIZE], siMsgLengh);
			EraseInBuffer(siTempSize);
			return TRUE;
			/*
			/////////////////////////////////////////////////
			// CheckSum�� ���Ѵ�.
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
				// CheckSum���� Ʋ���� ������.
				clGrp.Error("","�����޼����� Checksum�� Ʋ���� ������ ���۸� ���ϴ�. Command : [%2x][%2x][%2x][%2x][%2x][%2x][%2x][%2x][%2x][%2x]", (UI08)cInBuffer[0],(UI08)cInBuffer[1],(UI08)cInBuffer[2],(UI08)cInBuffer[3],(UI08)cInBuffer[4],(UI08)cInBuffer[5],(UI08)cInBuffer[6],(UI08)cInBuffer[7],(UI08)cInBuffer[8],(UI08)cInBuffer[9]);
				clGrp.Error("","���� üũ�� : [%2x], ����� üũ�� : [%2x]", cInBuffer[2], cCheckSum);
				EraseInBuffer(0);
			}
			*/
		}
		// siTempSize=0�̸� �޼����� ������ ������ ���̹Ƿ� ��� �޼����� �����Ѵ�.
		else if(siTempSize <= PACK_HEADER_SIZE)
		{
//			clGrp.Error("","�����޼����� �����Ǿ��� ������ ���۸� ���ϴ�.");
			EraseInBuffer(0);
		}
	}
	return FALSE;
}

