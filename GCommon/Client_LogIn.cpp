#include <GSL.h>

#include "Client_LogIn.h"

// CD-Key Check�� ���� �ʴ´ٸ� cCDKey���� NULL�� �Է�.
Client_Login::Client_Login(char *cGameName, char *cGameVer, char *cUserType, char *cCDKey)
{
	memset(m_cGameName, 0, sizeof(m_cGameName));
	memset(m_cGameVer, 0, sizeof(m_cGameVer));
	memset(m_cUserType, 0, sizeof(m_cUserType));
	memset(m_cCDKey, 0, sizeof(m_cCDKey));
	memset(m_cID, 0, sizeof(m_cID));
	memset(m_cPass, 0, sizeof(m_cPass));
	
	m_iInBufLen = 0;
	m_iOutBufLen = 0;
	SockOpterr	= 0;
	_Sec	= 0;

	strcpy(m_cGameName, cGameName);
	strcpy(m_cGameVer, cGameVer);
	strcpy(m_cUserType, cUserType);
	
	m_bCDKeyCheck = FALSE;

	if(cCDKey)
	{
		m_bCDKeyCheck = TRUE;
		strcpy(m_cCDKey, cCDKey);
	}
}

Client_Login::~Client_Login()
{
	Disconnect();
	WSACleanup();
}

void Client_Login::Disconnect(void)
{
	if(!m_bConnect) return;

	closesocket(m_Socket);

	m_bConnect = FALSE;
}

BOOL Client_Login::IsConnect(void)
{
	return m_bConnect;
}

BOOL Client_Login::Connect(char *pAddress, WORD wPort)
{	
	WSADATA		WsaData;

	m_bConnect = FALSE;

	if(WSAStartup(0x101, &WsaData) == SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		return FALSE;
	}

	memset(&m_SockAddr, 0, sizeof(SOCKADDR_IN));
	m_SockAddr.sin_addr.s_addr = inet_addr(pAddress); // it's ip address?
	if(m_SockAddr.sin_addr.s_addr == -1) // it's host name? 
	{
		struct hostent *h; 
		h = gethostbyname(pAddress);
		if(!h) return FALSE;
		memcpy(&m_SockAddr.sin_addr.s_addr, h->h_addr, h->h_length);
	}

	m_SockAddr.sin_family = AF_INET;           
	m_SockAddr.sin_port = htons(wPort);
	memset(&(m_SockAddr.sin_zero), NULL, 8);

	_Sec = timeGetTime(); // ���ӽð� �ʰ�üũ ����.
	
	if(connect(m_Socket, (sockaddr*)&m_SockAddr, sizeof(m_SockAddr)))
	{
		closesocket(m_Socket);
		WSACleanup();

		return FALSE;
	}

	m_timeout.tv_sec = 0;
	m_timeout.tv_usec = 0;	

	m_bConnect = TRUE;

	SendGameInfo();

	return TRUE;
}

//���� ���� ����. \\gamename\\kohan\\gamever\\10307\\usertype\\release\\hqcdkey\\zzzzzzzzz\\ 
void Client_Login::SendGameInfo(void)
{
	if(!m_bConnect) return;

	char MsgBuf[128];
	char MsgBuf2[64];
	wsprintf(MsgBuf, "\\gamename\\%s\\gamever\\%s\\usertype\\%s\\",m_cGameName,m_cGameVer,m_cUserType);
	if(m_bCDKeyCheck)
	{
		wsprintf(MsgBuf2,"hqcdkey\\%s\\",m_cCDKey);
		strcat(MsgBuf,MsgBuf2);
	}
	Write(MsgBuf,strlen(MsgBuf));
}

// �α��� �Ѵ�. \logname\%s\logpass\%s\ 
void Client_Login::SendAccount(char *cID, char *cPass)
{
	if(!m_bConnect) return;

	char MsgBuf[128];

	strcpy(m_cID, cID);
	strcpy(m_cPass, cPass);
	wsprintf(MsgBuf, "\\loginname\\%s\\loginpass\\%s\\", cID, cPass);
	Write(MsgBuf, strlen(MsgBuf));
}

// �� ������ �����. \newid\%s\newpass\%s\ 
void Client_Login::SendNewAccount(char *cID, char *cPass)
{
	if(!m_bConnect) return;

	char MsgBuf[128];

	memset(m_cID, 0, sizeof(m_cID));
	memset(m_cPass, 0, sizeof(m_cPass));
	strcpy(m_cID, cID);
	strcpy(m_cPass, cPass);
	wsprintf(MsgBuf, "\\newid\\%s\\newpass\\%s\\", cID, cPass);
	Write(MsgBuf, strlen(MsgBuf));
}

// ��ȣ�� �����Ѵ�. \newid\%s\newpass\%s\ 
void Client_Login::SendChangePassword(char *cID, char *nowPass, char *newPass)
{
	if(!m_bConnect) return;

	char MsgBuf[128];

	memset(m_cID, 0, sizeof(m_cID));
	memset(m_cPass, 0, sizeof(m_cPass));
	strcpy(m_cID, cID);
	strcpy(m_cPass, newPass);
	wsprintf(MsgBuf, "\\changeid\\%s\\nowpassword\\%s\\newpassword\\%s\\", cID, nowPass, newPass);
	Write(MsgBuf, strlen(MsgBuf));
}

// �α׾ƿ��Ѵ�.\logout\%s\ 
void Client_Login::SendLogOut(void)
{
	if(!m_bConnect) return;

	char MsgBuf[64];

	wsprintf(MsgBuf, "\\logout\\%s\\", m_cGameName);
	Write(MsgBuf, strlen(MsgBuf));
}

void Client_Login::Write(char *Msg, int iByte)
{
	if(!m_bConnect) return;

	char cPackMsg[MAX_BUFFER_SIZE];
	short iPackSize = 0;

	memset(cPackMsg, 0, sizeof(cPackMsg));

	PackMessage((char*)Msg, (short)iByte, cPackMsg, &iPackSize);

	memcpy((void *)&m_cOutBuffer[m_iOutBufLen], cPackMsg, iPackSize);
	m_iOutBufLen += iPackSize;
	
	FlushOutBuffer();
}

void Client_Login::FlushOutBuffer(void)
{
	if(!m_bConnect) return;

	int	siBytesSent=0;
	
	if(m_iOutBufLen > 0)
	{
		siBytesSent	= send(m_Socket, m_cOutBuffer, m_iOutBufLen, 0);
		m_iOutBufLen -= siBytesSent;     // ������ ���� Bytes

		if(m_iOutBufLen != 0)
		{
			memmove( &m_cOutBuffer[siBytesSent], m_cOutBuffer, m_iOutBufLen);
		}
	}

	if(siBytesSent < 0) 
	{
		if(m_bConnect)
			Disconnect();
	}
}

void Client_Login::ResetSets(void)	
{ 
	FD_ZERO(&m_fdError);
	FD_ZERO(&m_fdPlayer);
	FD_SET(m_Socket, &m_fdError);
	FD_SET(m_Socket, &m_fdPlayer);
}

BOOL Client_Login::Readable(void)	
{
	ResetSets();

	select(m_Socket, &m_fdPlayer, NULL, &m_fdError, &m_timeout);
	if(FD_ISSET(m_Socket, &m_fdError))
	{
		Disconnect();
		return FALSE;
	}
	else if(FD_ISSET(m_Socket, &m_fdPlayer))
		return TRUE;
	
	return FALSE;
}

BOOL Client_Login::Writable(void)
{
	ResetSets();
	select(m_Socket, NULL, &m_fdPlayer, &m_fdError, &m_timeout);
	if(FD_ISSET(m_Socket, &m_fdError))
	{
		Disconnect();
		return FALSE;
	}
	else if(FD_ISSET(m_Socket, &m_fdPlayer))
		return TRUE;

	return FALSE;
}

void Client_Login::Read(void)
{
	if(!m_bConnect) return;

	int iBytesRecv;

	iBytesRecv = recv(m_Socket, &m_cInBuffer[m_iInBufLen], MAX_BUFFER_SIZE, 0);

	if(iBytesRecv < 0)
	{
		if(m_bConnect)
			Disconnect();
	}
	m_iInBufLen += iBytesRecv;
}

int Client_Login::Poll(void)
{
	int iErrorCode = 0;

	if(Writable())
		FlushOutBuffer();

	if(Readable())
	{		
		Read();		
		_Sec = timeGetTime();
	}

	// 30�ʵ��� �α��� ������ �����Ҽ� ������ ������ ��ư�� �����ش�..����~!!
	//char temp[30];
	//sprintf( temp, "%d - %d = %d", timeGetTime() ,_Sec, timeGetTime() - _Sec );
	//MessageBox( NULL, temp, "", MB_OK|MB_TOPMOST );

	if( timeGetTime() - _Sec > 30000 )
	{
		if( 0 >= m_iInBufLen )
		{
			SockOpterr = -1;
			//BattleMsg.MsgCreate(MSG_FAILCONNECT, Y_OKCANCEL_ONLY_OK, Y_OKCANCEL_LARGEBORDER,
			//				"�������Լ� ������ �����ϴ�.");			
		}
	}	

	if(m_iInBufLen > PACK_HEADER_SIZE)
	{
		char *tokTemp;

		if(UnpackMessage())
		{		
			// success, game_err, version_err, user_err, cdkey_err, 
			if(tokTemp = strstr(m_cMsg, "inforesult"))
				iErrorCode = RecvKind(&tokTemp[10]);

			// success, input_err1, input_err2, present, failed
			else if(tokTemp = strstr(m_cMsg, "loginresult"))
				iErrorCode = RecvLogIn(&tokTemp[11]);

			// success, input_err1, input_err2, present, failed
			else if(tokTemp = strstr(m_cMsg, "entryresult"))
				iErrorCode = RecvNewID(&tokTemp[11]);

			else if(tokTemp = strstr(m_cMsg, "changpwresult"))
				iErrorCode = RecvChangePassword(&tokTemp[13]);

			else
				iErrorCode = ERROR_UNEXPECT_PACKET;
		}
	}

	return iErrorCode;
}

void Client_Login::Get_Notice(char **pNotice)
{
	char *pTemp=NULL;

	pTemp = strstr(m_cMsg, "gamenotice");

	memset(m_cNotice, 0, sizeof(m_cNotice));
	strcpy(m_cNotice, &pTemp[11]);
	m_cNotice[strlen(m_cNotice)-1] = '\n';
	*pNotice = m_cNotice;
}

void Client_Login::Get_ChatServerAddress_Guid(char **pAddress, char **pGuid)
{
	char TempBuf[128];
	char *token;
	char *Temp[8];
	int	i=0, j;

	char *pTemp=NULL;

	pTemp = strstr(m_cMsg, "chatsvrip");

	memset(TempBuf, 0, sizeof(TempBuf));
	strcpy(TempBuf, pTemp);
	token = strtok(TempBuf, "\\");
	
	while(token != NULL) 
	{
		Temp[i] = token;
		token = strtok(NULL, "\\");
		i++;
	}

	for(j=0; j<i; j++)
	{
		if(strstr(Temp[j], "chatsvrip"))
			break;
	}
	memset(m_cChatServerAddress, 0, sizeof(m_cChatServerAddress));
	strcpy(m_cChatServerAddress, Temp[j+1]);
	*pAddress = m_cChatServerAddress;

	for(j; j<i; j++)
	{
		if(strstr(Temp[j], "guid"))
			break;
	}
	memset(m_cGUID, 0, sizeof(m_cGUID));
	strcpy(m_cGUID, Temp[j+1]);
	*pGuid = m_cGUID;
}

// ������ success�� ���;� �Ѵ�. �������� ������ ���.
int Client_Login::RecvKind(char* Buf)
{
	int	i=0;

	while(response[i])
	{
		if(strstr(Buf, response[i]))
			break;
		i++;
	}
	switch(i)
	{
	case 0:
		// success : ���������� ������ Ȯ�εǾ���, �α����� �����ϴ�.
		return SUCCESS_CONNECT;

	case 5:
		// cdkey-err : CDKey�� �߸� �Ǿ���.
		return ERROR_CONNECT_WRONG_CDKEY;

	case 6:
		//user_err : ����� ������ Ʋ�����. ����� release�� demo�� ����.
		return ERROR_CONNECT_WRONG_RELEASE;

	case 7:
		//game_err : �������� �ʴ°���. �̻��� ������ �Է����� ���.
		return ERROR_CONNECT_NOT_SUPPORT_GAME;

	case 8:
		//version_err : ���ӹ����� Ʋ�����, ��ġ�� �޾ƾ��Ѵ�.
		return ERROR_CONNECT_WRONG_VERSION;

	case 9:
		//cdkeyinuse : �õ�Ű�� �̹� �����
		return ERROR_CONNECT_INUSE_CDKEY;

	default:
		//��Ÿ ���� �� ������ ��� ó��.
		//printf("%d, %s\n", i, response[i]);
		return ERROR_UNEXPECT_PACKET;
	}
}

// GUID���� ������ ä�ü����� �����Ѵ�.
int Client_Login::RecvLogIn(char* Buf)
{
	int			i=0;
	//char*		pTemp;

	while(response[i])
	{
		if(strstr(Buf, response[i]))
			break;
		i++;
	}
	// success, input_err1, input_err2, present, failed
	switch(i)
	{
	case 0:
		//success : �������� ��� GUID������ Chatting Server�� ����
		return SUCCESS_LOGIN;

	case 1:
		//failed : ���� �� ������ �߻�������.
		return ERROR_LOGIN_UNEXPECT;

	case 2:
		//present : �Է��� ���̵� �̹� ������ �ϰ��
		return ERROR_LOGIN_ALREADY_CONNECT;

	case 3:
		//input_err1 : ID�Է��� �߸����� ��� ó��
		return ERROR_LOGIN_WRONG_ID_FORMAT;

	case 4:
		//input_err2 : Password�Է��� �߸����� ��� ó��
		return ERROR_LOGIN_WRONG_PW_FORMAT;

	default:
		// ��Ÿ �̻��� ������ �޾����� ó��.(������ ���� �̻��� ��Ŷ�� �޾��� ���)
		return ERROR_UNEXPECT_PACKET;
	}
}


// "success"�� ���� RecvLogIn()�� ���� ������ ����, �׷��� ������ ���� ���
int Client_Login::RecvNewID(char* Buf)
{
	int i=0;

	while(response[i])
	{
		if(strstr(Buf, response[i]))
			break;
		i++;
	}
	switch(i)
	{
	case 0:
		// success
		//SendAccount((char *)&m_cID, (char *)&m_cPass);
		return SUCCESS_NEWACCOUNT;

	case 1:
		// failed : ���� �� ����.
		return ERROR_NEWACCOUNT_UNEXPECT;

	case 2:
		// present : ���̵� ������ ��� ó��
		return ERROR_NEWACCOUNT_ALREADY_EXIST;

	case 3:
		// input_err1 : ID�Է��� �߸����� ��� ó��
		return ERROR_NEWACCOUNT_WRONG_ID_FORMAT;

	case 4:
		// input_err2 : Password�Է��� �߸����� ��� ó��
		return ERROR_NEWACCOUNT_WRONG_PW_FORMAT;

	default:
		// ��Ÿ �̻��� ������ �޾����� ó��.(������ ���� �̻��� ��Ŷ�� �޾��� ���)
		return ERROR_UNEXPECT_PACKET;
	}
}

int Client_Login::RecvChangePassword(char *Buf)
{
	int i=0;

	while(response[i])
	{
		if(strstr(Buf, response[i]))
			break;
		i++;
	}
	switch(i)
	{
	case 0:
		// success
		return SUCCESS_CHANGEPASSWORD;

	case 1:
		// failed : ���� �� ����.
		return ERROR_CHANGEPASSWORD_UNEXPECT;

	case 3:
		// input_err1 : ID�Է��� �߸����� ��� ó��
		return ERROR_CHANGEPASSWORD_WRONG_ID_FORMAT;

	case 4:
		// input_err2 : Password�Է��� �߸����� ��� ó��
		return ERROR_CHANGEPASSWORD_WRONG_PW_FORMAT;

	default:
		// ��Ÿ �̻��� ������ �޾����� ó��.(������ ���� �̻��� ��Ŷ�� �޾��� ���)
		return ERROR_UNEXPECT_PACKET;
	}
	return 0;
}

//������������������������������������������������������������������
//  �޼���  ��ŷ, ����ŷ (��ȣȭ) �κ�.
//������������������������������������������������������������������
VOID Client_Login::PackMessage(char *cSrcMsg, short siSrcSize, char *cPackMsg, short *siPackSize)
{
	int i			= siSrcSize;
	char cCheckSum	= 0;

	if(siSrcSize > MAX_BUFFER_SIZE)
	{
		//printf("�������� �޼����� ������ ũ�⺸�� Ů�ϴ�.\n");
		return;
	}

	// CheckSum�� ���Ѵ�.
	for(i=0; i< siSrcSize; i++)
	{
		cCheckSum ^= (char)cSrcMsg[i];
	}
	*siPackSize = siSrcSize + PACK_HEADER_SIZE;

	//////////////////////////////////////////////////
	// ��Ű¡ �޼����� �����.
	//////////////////////////////////////////////////
	// ��ü �޼��� ũ��
	cPackMsg[0] = LOBYTE(*siPackSize);
	cPackMsg[1] = HIBYTE(*siPackSize);
	cPackMsg[2] = cCheckSum;

	memcpy(&cPackMsg[PACK_HEADER_SIZE], cSrcMsg, siSrcSize);
}

//����������������������������������������������������������������������������
// ���� ��ŷ�� �޼����� Ǭ��.
//����������������������������������������������������������������������������
BOOL Client_Login::UnpackMessage()
{
	int		i			= 0;
	char	cCheckSum	= 0;
	short	siTempSize	= 0;

	// ������ �ʱ�ȭ.
	short	siMsgBufferLen = 0;
	memset(m_cMsg, 0, sizeof(m_cMsg));

	if(m_iInBufLen > 2)
	{
		siTempSize = MAKEWORD(m_cInBuffer[0], m_cInBuffer[1]);
		// �ʿ��� ��Ŷ�� ��� ������ ��쿡�� ó���Ѵ�.
		if(m_iInBufLen >= siTempSize && siTempSize > 0)
		{
			// ���� �޼����� ������ ũ�⺸�� ũ��
			if(siTempSize > MAX_BUFFER_SIZE)
			{
				//printf("�����޼����� ������ ũ�⺸�� Ů�ϴ�.\n");
				EraseInBuffer(siTempSize);
				return FALSE;
			}

			/////////////////////////////////////////////////
			// CheckSum�� ���Ѵ�.
			/////////////////////////////////////////////////
			for(i=PACK_HEADER_SIZE; i<siTempSize; i++) 
			{ 
				cCheckSum ^= m_cInBuffer[i];
			}
			if(m_cInBuffer[2] == cCheckSum)
			{
				siMsgBufferLen = siTempSize - PACK_HEADER_SIZE;
				memcpy(m_cMsg, &m_cInBuffer[PACK_HEADER_SIZE], siMsgBufferLen);
				EraseInBuffer(siTempSize);
				return TRUE;
			}
			else
			{
				// CheckSum���� Ʋ���� ������.
				EraseInBuffer(siTempSize);
			}
		}
		// siTempSize=0�̸� �޼����� ������ ������ ���̹Ƿ� ��� �޼����� �����Ѵ�.
		else if(siTempSize == 0)
		{
			EraseInBuffer(0);
		}
	}
	return FALSE;
}

void Client_Login::EraseInBuffer(int iByte)
{	
	if((iByte == 0) || (iByte >= m_iInBufLen))
	{
		m_iInBufLen = 0;
		memset(m_cInBuffer, 0, MAX_BUFFER_SIZE);
	}
	else
	{
		memmove((void*)m_cInBuffer, (void *)&m_cInBuffer[iByte], m_iInBufLen - iByte);
		//printf("�ڡڡڡ�%dByte�о�´�.�ڡڡڡ�\n",iByte);
		m_iInBufLen -= iByte;
	}
}


