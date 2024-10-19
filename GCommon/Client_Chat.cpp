#include "Client_Chat.h"

/*
	생성자
*/
Client_Chat::Client_Chat()
{
	memset(&m_timeout, NULL, sizeof(timeval));
	m_bConnect = FALSE;

	memset(m_cOutBuffer, NULL, sizeof(m_cOutBuffer));
	memset(m_cInBuffer, NULL, sizeof(m_cInBuffer));
	memset(m_pID, NULL, sizeof(m_pID));

	m_iInBufferLen = 0;
	m_iOutBufferLen = 0;

}

/*
	소멸자
*/
Client_Chat::~Client_Chat()
{
}

/*
	채팅 서버에 접속한다
	Paramter : Chat Server Address, port, Guid
*/
BOOL Client_Chat::Connect(char *pAddress, WORD wPort)
{
	WSADATA		WsaData;
	if(WSAStartup(0x101, &WsaData) == SOCKET_ERROR)
		WSACleanup();

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

	if((m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		return FALSE;

	if(connect(m_Socket, (sockaddr *)&m_SockAddr, sizeof(sockaddr_in)) == SOCKET_ERROR)
		return FALSE;

	m_bConnect = TRUE;

	return TRUE;
}

/*
	채팅 서버와의 접속을 종료한다
*/
void Client_Chat::Disconnect(void)
{
	if(!m_bConnect) 
		return;
	
	if(closesocket(m_Socket) != SOCKET_ERROR)
		m_bConnect = FALSE;			
}

/*
	채팅 서버와의 접속 여부를 확인한다
*/
BOOL Client_Chat::IsConnect(void)
{
	return m_bConnect;
}

/*
	채팅서버에서 입력버퍼를 주어진 iByte수 만큼 앞에서 부터 지운다.
	Parameter : 지울 버퍼 크기
*/
void Client_Chat::EraseInBuffer(int iByte)
{
	if((iByte == 0) || (iByte >= m_iInBufferLen))
	{
		m_iInBufferLen = 0;
		memset(m_cInBuffer, 0, sizeof(m_cInBuffer));
	}
	else
	{
		memmove((void*)m_cInBuffer, (void *)&m_cInBuffer[iByte], m_iInBufferLen-iByte);
		m_iInBufferLen -= iByte;
	}
}

/*
	채팅서버로 메시지를 일괄전송시킨다
*/
void Client_Chat::FlushOutBuffer(void)
{
	int BytesSent = 0;

	if(m_iOutBufferLen > 0)
	{
		BytesSent = send(m_Socket, m_cOutBuffer, m_iOutBufferLen, 0);
	}

	if(BytesSent < 0)
	{	
		if(m_bConnect)
			Disconnect();
	}

	else if(BytesSent > 0)
	{
		
		///////////////////////////////
		// Packet Debug
		FILE *file;
		file = fopen("C:\\Send.txt", "a");

		for(int i=0; i<BytesSent; i++)
			fprintf(file, "%c", m_cOutBuffer[i]);
		fprintf(file, "\n");
		fclose(file);
		///////////////////////////////
		
		if(m_iOutBufferLen > BytesSent)
			memmove(m_cOutBuffer, &m_cOutBuffer[BytesSent], m_iOutBufferLen-BytesSent);

		m_iOutBufferLen -= BytesSent;
	}
}

/*
	FD_SET 초기화
*/
void Client_Chat::ResetSets(void)
{
	FD_ZERO(&m_fdError);
	FD_ZERO(&m_fdPlayer);
	FD_SET(m_Socket, &m_fdError);
	FD_SET(m_Socket, &m_fdPlayer);
}

/*
	채팅서버로부터 Recv된 패킷이 있는지 검사
*/
BOOL Client_Chat::Readable(void)	
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

/*
	채팅서버로부터 Send할 준비가 되었는지 검사
*/
BOOL Client_Chat::Writable(void)
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

/*
	채팅서버로부터 일정크기만큼의 데이터를 읽어온다
	Parameter : 읽어올 버퍼 크기 
*/
int Client_Chat::Read(int siBytesToRead)
{
	int siByteCount;

	if(!m_bConnect) return FALSE;

	// 입력버퍼의 크기를 초과하는지 검사 
	if((siBytesToRead + m_iInBufferLen) > CLIENT_CHAT_MAX_IN_BUFFER)
	{
		Disconnect();
		return -1;
	}

	// socket으로 부터 입력버퍼에 지정한 버퍼크기만큼 읽어온다.
	siByteCount = recv(m_Socket, &m_cInBuffer[m_iInBufferLen], siBytesToRead, 0);

	if(siByteCount > 0)
	{
		char temp[256];
		strncpy(temp, &m_cInBuffer[m_iInBufferLen], siByteCount);
		//MessageBox( NULL, temp, "", MB_OK|MB_TOPMOST );
	}


	
	///////////////////////////////
	// Packet Debug
	FILE *file;
	file = fopen("C:\\Recv.txt", "a");

	for(int i=0; i<siByteCount; i++)
		fprintf(file, "%c", m_cInBuffer[m_iInBufferLen+i]);
	fprintf(file, "\n");
	fclose(file);
	///////////////////////////////
	



	if(siByteCount < 0) // 서버와의 접속 끊김
	{
		Disconnect();
		return -1;
	}

	m_iInBufferLen += siByteCount;

	return siByteCount;
}

/*
	채팅서버로 일정크기의 데이터를 보낸다
	Parameter : 보낼 메시지, 보낼 메시지 크기
*/
BOOL Client_Chat::Write(const void * cMsg, int iBytes)
{
	if(!m_bConnect) return FALSE;

	if(m_iOutBufferLen + iBytes > CLIENT_CHAT_MAX_OUT_BUFFER)
	{
		Disconnect();
		return FALSE;
	}

	memcpy(&m_cOutBuffer[m_iOutBufferLen], cMsg, iBytes);

	m_iOutBufferLen += iBytes;

	return TRUE;
}

/*
	채널리스트 가져오기
	Parameter : 채팅하는 유저, 채널에 있는 총 유저, 채널 이름
*/
void Client_Chat::Get_ChannelList(int *chatuser, int *alluser, char **chname)
{
	char *chat, *all;

	*chname = GetParameter(m_pOutputBuffer, 2, &chat, &all);

	*chatuser = (atoi(chat));
	*alluser = (atoi(all));
}

/*
	채널이름 가져오기
	Parameter : 채널 이름
*/
void Client_Chat::Get_Channel(char **chname)
{
	*chname = m_pOutputBuffer;
}

/*
	자신이 참여하고 있는 채널에서 플레이어 목록 가져오기
	Parameter : 유저네임, 유저 정보
*/
void Client_Chat::Get_JoinPlayer(char **name, char **info)
{
	GetParameter(m_pOutputBuffer, 2, name, info);
}

/*
	자신이 참여하고 있는 채널에서 나간 플레이어 목록 가져오기
	Parameter : 유저네임
*/
void Client_Chat::Get_OutPlayer(char **name)
{
	*name = m_pOutputBuffer;
}

/*
	채팅 메시지 가져오기
	Parameter : 체팅 메시지
*/
void Client_Chat::Get_ChatMessage(char **user, char **msg)
{
	*msg = GetParameter(m_pOutputBuffer, 1, user);
}

/*
	랭킹 가져오기
	Parameter : 랭킹 정보
*/
void Client_Chat::Get_Rank(char **rank, char **name, char **vic, char **def, char **dis, char **point)
{
	GetParameter(m_pOutputBuffer, 6, rank, name, vic, def, dis, point);
}

/*
	길드 정보 가져오기
	Parameter : 길드장, 길드 이름
*/
void Client_Chat::Get_Guild(char **master, char **name)
{
	GetParameter(m_pOutputBuffer, 2, master, name);
}

/*
	유저 이름을 가져오기
	Parameter : 유저이름
*/
void Client_Chat::Get_Name(char **name)
{
	*name = m_pOutputBuffer;
}

/*
	유저 이름을 가져오기
	Parameter : 유저이름
	* 현재 name는 gametype으로 channel에서 gamestate중이다 *
*/
void Client_Chat::Get_Where(char **name, char **gamestate, char **gametype, char **channel)
{
	*channel = GetParameter(m_pOutputBuffer, 3, name, gamestate, gametype);
}
/*
	Chat클라이언트의 메인루프
*/
int Client_Chat::Poll(void)
{
	WORD wRet=0;

	if(!m_bConnect) return 0;

	if(Readable())
		if(Read(CLIENT_CHAT_MAX_IN_BUFFER - m_iInBufferLen) == -1)
			return 0;

	if(m_iInBufferLen > 0)
	{
		char str[1024];
		int strsize;

		memset(str, NULL, sizeof(str));
		ReadLine(m_cInBuffer, m_iInBufferLen, str, strsize);
		
		if(strsize == 0) return 0; // 만일 \n문자 없이 왔다면 다시 recv호출
		wRet = Cmd(str);
	}

	if(Writable())
		FlushOutBuffer();

	return wRet;
}

/*
	m_cInBuffer로부터 '\n'으로 구별된 한 라인을 가져오고
	m_iInBufferLen은 읽은 라인만큼 감소시킨다
	Parameter : recv버퍼, recv버퍼 크기, 파싱된 문자열, 파싱된 문자열 크기
*/
void Client_Chat::ReadLine(char *buf, int &len, char *str, int &strsize)
{
	if(len == 0) return; // nothing came.. 

	for(strsize = 0; strsize < len; strsize++)
	{
		if(buf[strsize] == '\n')
		{
			// end of line
			strsize++;

			memcpy(str, buf, strsize);

			memmove(buf, buf + strsize, len - strsize);

			len -= strsize;
			str[strsize-1] = '\0'; 
			return;
		}
	}

	// 온 문자가 '\n'문자없이 왔다면
	if(strsize == len)
	{
		strsize = 0;
		return;
	}
}

/*
	명령어의 인자들을 얻어온다. ' '나 '\t'으로 분리된 메시지들을 가져옴
	Parameter : 파싱할 문자열, 파싱할 개수, 파싱된 문자열들
*/
char *Client_Chat::GetParameter(char *data, int count, ...)
{
    char **str, *tmp;

    va_list args;
    va_start(args, count);

    while(count-- > 0)
    {
        str = (char **)va_arg(args, char **);

		while(*data == ' ' || *data == '\t') 
			data++;

        tmp = data;
        while(*data != '\0' && *data != '\t' && *data != ' ') 
			data++;

        if(*data == '\t' || *data == ' ')
			*data++ = '\0';

        if(str != NULL) *str = tmp;
    }
    va_end(args);

	return data;
}

/*
	서버로부터 온 메시지가 어떤것인지 검색
	Parameter : 파싱된 문자열, 파싱된 문자열 길이
*/
int Client_Chat::Cmd(char *buf)
{
	WORD wRValue;

	char *cmd, *ptr;

	if(*buf != '/') 
		return 0; // line skip
	buf++;

	ptr = GetParameter(buf, 1, &cmd);

	for(wRValue=1; wRValue<CLIENT_CHAT_MAX_COMMAND_NUM; wRValue++)
		if(stricmp(cCommandList[wRValue], cmd)==0)
			break;

	//char temp[256];
	//sprintf( temp, "MsgVal   %d", wRValue );
	//MessageBox( NULL, temp, temp, MB_OK|MB_TOPMOST );

	switch(wRValue)
	{
	case OK_LOGIN:
		Cmd_OkLogin(ptr);
		break;

	case NO_LOGIN:
		break;

	case OK_JOIN:
		Cmd_Default(ptr);
		break;

	case NO_JOIN:
		break;

	case SEND:
	case WHISPER:
	case CHANNEL:
		Cmd_Default(ptr);
		break;

	case PLAYER:
		Cmd_Player(ptr);
		break;

	case RMPLAYER:
		Cmd_RmPlayer(ptr);
		break;

	case OK_SGAME:
	case NO_SGAME:
	case OK_EGAME:
	case NO_EGAME:
	case PING:
		break;

	case OK_RANK:
	case OK_SQUELCH:
	case OK_UNSQUELCH:
	case OK_ADDMEMBER:
	case NO_ADDMEMBER:
	case JOINGUILD:
	case OK_JOINGUILD:
	case NO_JOINGUILD:
	case OK_DELMEMBER:
	case NO_DELMEMBER:
	case WHERE:
		Cmd_Default(ptr);
		break;
	}

	return wRValue;
}

/*
	1. 추가된 채널 리스트를 받는다. str : channel
	2. 채널안에 있는 유저에게 메시지 전달 str : chatting messege
	3. 채널안에 있는 특정 유저에게 메시지 전달 str : id, chatting messege
	4. 랭킹을 출력한다 str : rank message
	5. 수신거부한다 str : 수신거부 ID
	6. 수신거부를 해제한다 str : 수신거부해제 ID
	7. 길드에 가입 str : 길드이름
	8. 길드에 가입안됨 str : 길드이름
	9. 길드에서 탈퇴함 str : 길드이름
	10. 길드에서 탈퇴하지 못함
	11. 채널의 Join이 성공되었을시	str : Channel name
	12. 유저가 어디채널에 무슨게임을 하고있는지 찾음 str : 길드이름
*/
void Client_Chat::Cmd_Default(char *str)
{
	memset(m_pOutputBuffer, NULL, sizeof(m_pOutputBuffer));
	wsprintf(m_pOutputBuffer, "%s", str);
}

/*
	추가된 플레이어 리스트를 받는다
	str : player name, player information
*/
void Client_Chat::Cmd_Player(char *str)
{
	char *guild, *id, *detail;

	GetParameter(str, 3, &guild, &id, &detail);

	*guild++ = NULL;
	guild[strlen(guild)-1] = NULL;

	memset(m_pOutputBuffer, NULL, sizeof(m_pOutputBuffer));
	if(strlen(guild))
	{
		char temp[50];
		memset(temp, NULL, sizeof(temp));
		wsprintf(temp, "【%s】%s", guild, id);
		wsprintf(m_pOutputBuffer, "%s %s", temp, detail);
	}
	else
		wsprintf(m_pOutputBuffer, "%s %s", id, detail);
}

/*
	삭제된 플레이어 리스트를 받는다.
	str : player name
*/
void Client_Chat::Cmd_RmPlayer(char *str)
{
	char *guild, *id;

	GetParameter(str, 2, &guild, &id);

	*guild++ = NULL;
	guild[strlen(guild)-1] = NULL;

	memset(m_pOutputBuffer, NULL, sizeof(m_pOutputBuffer));
	if(strlen(guild))
	{
		char temp[50];
		memset(temp, NULL, sizeof(temp));
		wsprintf(temp, "【%s】%s", guild, id);
		wsprintf(m_pOutputBuffer, "%s", temp);
	}
	else
		wsprintf(m_pOutputBuffer, "%s", id);
}

/*
	로그인후 ID를 가져온다
	str : ID
*/
void Client_Chat::Cmd_OkLogin(char *str)
{
	memset(m_pOutputBuffer, NULL, sizeof(m_pOutputBuffer));
	wsprintf(m_pOutputBuffer, "%s", str);

	strcpy(m_pID, str);
}

/*
	로긴 요구
	parameter : login서버로부터 얻어온 guid값
*/
void Client_Chat::Msg_Login(char *guid, char *game)
{
	if(!guid) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/LOGIN %s %s\n", guid, game); 
	int i = strlen(str);
	Write(str, i);
}

/*
	채팅 메시지 전달 요구
	Parameter : 채팅 메시지
*/
void Client_Chat::Msg_Send(char *msg)
{
	if(!msg) return;
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/SEND %s\n", msg); 
	Write(str, strlen(str));
}

/*
	귓속말 전달 요구
	Parameter : 귓속말 대상 ID, 채팅 메시지
*/
void Client_Chat::Msg_Whisper(char *id, char *msg)
{
	if(!id) return;
	if(!msg) return;
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/WHISPER %s %s\n", id, msg); 
	Write(str, strlen(str));
}

/*
	채널 조인 요구
	Parameter : Channel 이름
*/
void Client_Chat::Msg_Join(char *ch)
{
	if(!ch) return;
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/JOIN %s\n", ch); 
	Write(str, strlen(str));
}

/*
	ID 수신거부 요구
	Parameter : 수신거부 ID
*/
void Client_Chat::Msg_Squelch(char *id)
{
	if(!id) return;
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/SQUELCH %s\n", id); 
	Write(str, strlen(str));
}

/*
	ID 수신거부 해제 요구
	Parameter : 수신해제 ID
*/
void Client_Chat::Msg_UnSquelch(char *id)
{
	if(!id) return;
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/UNSQUELCH %s", id); 
	Write(str, strlen(str));
}

/*
	채널 리스트 요구
*/
void Client_Chat::Msg_Channel(void)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/CHANNEL\n"); 
	Write(str, strlen(str));
}

/*
	게임의 시작을 알림
*/
void Client_Chat::Msg_StartGame(void)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/SGAME\n"); 
	Write(str, strlen(str));
}

/*
	게임의 끝을 알림
	Parameter : 승(1)패(2)디스(3), 점수
*/
void Client_Chat::Msg_EndGame(char cResult, char cPoint)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];
	memset(str, NULL, sizeof(str));

	wsprintf(str, "/EGAME %d %d\n", cResult, cPoint); 

	Write(str, strlen(str));
}

/*
	길드에 가입함 (길드원의 권한)
	Parameter : 길드장의 ID
*/
void Client_Chat::Msg_JoinGuild(char *cGuildMaster)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];
	memset(str, NULL, sizeof(str));

	wsprintf(str, "/JOINGUILD %s\n", cGuildMaster);

	Write(str, strlen(str));
}

/*
	유저 ID에게 길드에 가입을 요구(길드장의 권한)
	Parameter : 유저 ID

*/
void Client_Chat::Msg_AddMember(char *cID)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];
	memset(str, NULL, sizeof(str));

	wsprintf(str, "/ADDMEMBER %s\n", cID);

	Write(str, strlen(str));
}

/*
	길드에서 탈퇴함(길드원의 권한)
*/
void Client_Chat::Msg_DelMember(void)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];
	memset(str, NULL, sizeof(str));

	wsprintf(str, "/DELMEMBER\n");

	Write(str, strlen(str));
}

/*
	게임의 끝을 알림, 게임이 취소되었을때... (점수를 기록하지 않는다.)
	Parameter : 승(1)패(2)디스(3), 점수
*/
void Client_Chat::Msg_EndGame(void)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];
	memset(str, NULL, sizeof(str));

	wsprintf(str, "/EGAME\n");

	Write(str, strlen(str));
}

/*
	게임의 취소를 알림
*/
void Client_Chat::Msg_CancelGame(void)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/CGAME\n"); 
	Write(str, strlen(str));
}

/*
	상위 Top10 랭킹 출력
*/
void Client_Chat::Msg_Rank(void)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/RANK\n"); 
	Write(str, strlen(str));
}

/*
	iRank부터 10명의 랭킹 출력
*/
void Client_Chat::Msg_Rank(int iRank)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/RANK %d\n", iRank); 
	Write(str, strlen(str));
}

/*
	[ID]인 유저가 [어떤 게임]으로 [어떤 채널]에 [게임진행]에 여부를 알아온다.
*/
void Client_Chat::Msg_Where(char *pID)
{
	if(!strlen(m_pID)) return;

	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "/WHERE %s\n", pID); 
	Write(str, strlen(str));
}

/*
	저수준 Send함수
	Parameter : 보낼 메시지
*/
void Client_Chat::Msg(char *msg)
{
	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "%s\n", msg); 
	Write(str, strlen(str));
}

