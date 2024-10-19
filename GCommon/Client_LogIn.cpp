#include <GSL.h>

#include "Client_LogIn.h"

// CD-Key Check를 하지 않는다면 cCDKey값을 NULL로 입력.
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

	_Sec = timeGetTime(); // 접속시간 초과체크 변수.
	
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

//게임 정보 전송. \\gamename\\kohan\\gamever\\10307\\usertype\\release\\hqcdkey\\zzzzzzzzz\\ 
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

// 로그인 한다. \logname\%s\logpass\%s\ 
void Client_Login::SendAccount(char *cID, char *cPass)
{
	if(!m_bConnect) return;

	char MsgBuf[128];

	strcpy(m_cID, cID);
	strcpy(m_cPass, cPass);
	wsprintf(MsgBuf, "\\loginname\\%s\\loginpass\\%s\\", cID, cPass);
	Write(MsgBuf, strlen(MsgBuf));
}

// 새 계정을 만든다. \newid\%s\newpass\%s\ 
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

// 암호를 변경한다. \newid\%s\newpass\%s\ 
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

// 로그아웃한다.\logout\%s\ 
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
		m_iOutBufLen -= siBytesSent;     // 보내고 남은 Bytes

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

	// 30초동안 로그인 서버에 접속할수 없으면 나가기 버튼을 보여준다..나가~!!
	//char temp[30];
	//sprintf( temp, "%d - %d = %d", timeGetTime() ,_Sec, timeGetTime() - _Sec );
	//MessageBox( NULL, temp, "", MB_OK|MB_TOPMOST );

	if( timeGetTime() - _Sec > 30000 )
	{
		if( 0 >= m_iInBufLen )
		{
			SockOpterr = -1;
			//BattleMsg.MsgCreate(MSG_FAILCONNECT, Y_OKCANCEL_ONLY_OK, Y_OKCANCEL_LARGEBORDER,
			//				"서버에게서 응답이 없습니다.");			
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

// 오로지 success가 나와야 한다. 나머지는 이유를 출력.
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
		// success : 정상적으로 게임이 확인되었고, 로그인이 가능하다.
		return SUCCESS_CONNECT;

	case 5:
		// cdkey-err : CDKey가 잘못 되었다.
		return ERROR_CONNECT_WRONG_CDKEY;

	case 6:
		//user_err : 사용자 종류가 틀릴경우. 현재는 release와 demo로 구분.
		return ERROR_CONNECT_WRONG_RELEASE;

	case 7:
		//game_err : 지원하지 않는게임. 이상한 게임을 입력했을 경우.
		return ERROR_CONNECT_NOT_SUPPORT_GAME;

	case 8:
		//version_err : 게임버젼이 틀릴경우, 패치를 받아야한다.
		return ERROR_CONNECT_WRONG_VERSION;

	case 9:
		//cdkeyinuse : 시디키를 이미 사용중
		return ERROR_CONNECT_INUSE_CDKEY;

	default:
		//기타 원인 모를 오류일 경우 처리.
		//printf("%d, %s\n", i, response[i]);
		return ERROR_UNEXPECT_PACKET;
	}
}

// GUID값을 가지고 채팅서버에 접속한다.
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
		//success : 성공했을 경우 GUID값으로 Chatting Server에 접속
		return SUCCESS_LOGIN;

	case 1:
		//failed : 원인 모를 오류가 발생했을시.
		return ERROR_LOGIN_UNEXPECT;

	case 2:
		//present : 입력한 아이디가 이미 접속중 일경우
		return ERROR_LOGIN_ALREADY_CONNECT;

	case 3:
		//input_err1 : ID입력을 잘못했을 경우 처리
		return ERROR_LOGIN_WRONG_ID_FORMAT;

	case 4:
		//input_err2 : Password입력을 잘못했을 경우 처리
		return ERROR_LOGIN_WRONG_PW_FORMAT;

	default:
		// 기타 이상한 응답을 받았을때 처리.(서버로 부터 이상한 패킷을 받았을 경우)
		return ERROR_UNEXPECT_PACKET;
	}
}


// "success"일 경우는 RecvLogIn()에 대한 응답이 오고, 그렇지 않으면 이유 출력
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
		// failed : 원인 모를 실패.
		return ERROR_NEWACCOUNT_UNEXPECT;

	case 2:
		// present : 아이디가 존재할 경우 처리
		return ERROR_NEWACCOUNT_ALREADY_EXIST;

	case 3:
		// input_err1 : ID입력을 잘못했을 경우 처리
		return ERROR_NEWACCOUNT_WRONG_ID_FORMAT;

	case 4:
		// input_err2 : Password입력을 잘못했을 경우 처리
		return ERROR_NEWACCOUNT_WRONG_PW_FORMAT;

	default:
		// 기타 이상한 응답을 받았을때 처리.(서버로 부터 이상한 패킷을 받았을 경우)
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
		// failed : 원인 모를 실패.
		return ERROR_CHANGEPASSWORD_UNEXPECT;

	case 3:
		// input_err1 : ID입력을 잘못했을 경우 처리
		return ERROR_CHANGEPASSWORD_WRONG_ID_FORMAT;

	case 4:
		// input_err2 : Password입력을 잘못했을 경우 처리
		return ERROR_CHANGEPASSWORD_WRONG_PW_FORMAT;

	default:
		// 기타 이상한 응답을 받았을때 처리.(서버로 부터 이상한 패킷을 받았을 경우)
		return ERROR_UNEXPECT_PACKET;
	}
	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  메세지  패킹, 언패킹 (암호화) 부분.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID Client_Login::PackMessage(char *cSrcMsg, short siSrcSize, char *cPackMsg, short *siPackSize)
{
	int i			= siSrcSize;
	char cCheckSum	= 0;

	if(siSrcSize > MAX_BUFFER_SIZE)
	{
		//printf("보내려는 메세지가 버퍼의 크기보다 큽니다.\n");
		return;
	}

	// CheckSum을 구한다.
	for(i=0; i< siSrcSize; i++)
	{
		cCheckSum ^= (char)cSrcMsg[i];
	}
	*siPackSize = siSrcSize + PACK_HEADER_SIZE;

	//////////////////////////////////////////////////
	// 패키징 메세지를 만든다.
	//////////////////////////////////////////////////
	// 전체 메세지 크기
	cPackMsg[0] = LOBYTE(*siPackSize);
	cPackMsg[1] = HIBYTE(*siPackSize);
	cPackMsg[2] = cCheckSum;

	memcpy(&cPackMsg[PACK_HEADER_SIZE], cSrcMsg, siSrcSize);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 받은 패킹된 메세지를 푼다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL Client_Login::UnpackMessage()
{
	int		i			= 0;
	char	cCheckSum	= 0;
	short	siTempSize	= 0;

	// 변수의 초기화.
	short	siMsgBufferLen = 0;
	memset(m_cMsg, 0, sizeof(m_cMsg));

	if(m_iInBufLen > 2)
	{
		siTempSize = MAKEWORD(m_cInBuffer[0], m_cInBuffer[1]);
		// 필요한 패킷이 모두 도착한 경우에만 처리한다.
		if(m_iInBufLen >= siTempSize && siTempSize > 0)
		{
			// 받은 메세지가 버퍼의 크기보다 크면
			if(siTempSize > MAX_BUFFER_SIZE)
			{
				//printf("받은메세지가 버퍼의 크기보다 큽니다.\n");
				EraseInBuffer(siTempSize);
				return FALSE;
			}

			/////////////////////////////////////////////////
			// CheckSum을 비교한다.
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
				// CheckSum값이 틀리면 버린다.
				EraseInBuffer(siTempSize);
			}
		}
		// siTempSize=0이면 메세지가 완전히 변질된 것이므로 모든 메세지를 삭제한다.
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
		//printf("★★★★%dByte밀어냈다.★★★★\n",iByte);
		m_iInBufLen -= iByte;
	}
}


