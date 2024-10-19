#include "Client_Chat.h"

/*
	������
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
	�Ҹ���
*/
Client_Chat::~Client_Chat()
{
}

/*
	ä�� ������ �����Ѵ�
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
	ä�� �������� ������ �����Ѵ�
*/
void Client_Chat::Disconnect(void)
{
	if(!m_bConnect) 
		return;
	
	if(closesocket(m_Socket) != SOCKET_ERROR)
		m_bConnect = FALSE;			
}

/*
	ä�� �������� ���� ���θ� Ȯ���Ѵ�
*/
BOOL Client_Chat::IsConnect(void)
{
	return m_bConnect;
}

/*
	ä�ü������� �Է¹��۸� �־��� iByte�� ��ŭ �տ��� ���� �����.
	Parameter : ���� ���� ũ��
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
	ä�ü����� �޽����� �ϰ����۽�Ų��
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
	FD_SET �ʱ�ȭ
*/
void Client_Chat::ResetSets(void)
{
	FD_ZERO(&m_fdError);
	FD_ZERO(&m_fdPlayer);
	FD_SET(m_Socket, &m_fdError);
	FD_SET(m_Socket, &m_fdPlayer);
}

/*
	ä�ü����κ��� Recv�� ��Ŷ�� �ִ��� �˻�
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
	ä�ü����κ��� Send�� �غ� �Ǿ����� �˻�
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
	ä�ü����κ��� ����ũ�⸸ŭ�� �����͸� �о�´�
	Parameter : �о�� ���� ũ�� 
*/
int Client_Chat::Read(int siBytesToRead)
{
	int siByteCount;

	if(!m_bConnect) return FALSE;

	// �Է¹����� ũ�⸦ �ʰ��ϴ��� �˻� 
	if((siBytesToRead + m_iInBufferLen) > CLIENT_CHAT_MAX_IN_BUFFER)
	{
		Disconnect();
		return -1;
	}

	// socket���� ���� �Է¹��ۿ� ������ ����ũ�⸸ŭ �о�´�.
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
	



	if(siByteCount < 0) // �������� ���� ����
	{
		Disconnect();
		return -1;
	}

	m_iInBufferLen += siByteCount;

	return siByteCount;
}

/*
	ä�ü����� ����ũ���� �����͸� ������
	Parameter : ���� �޽���, ���� �޽��� ũ��
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
	ä�θ���Ʈ ��������
	Parameter : ä���ϴ� ����, ä�ο� �ִ� �� ����, ä�� �̸�
*/
void Client_Chat::Get_ChannelList(int *chatuser, int *alluser, char **chname)
{
	char *chat, *all;

	*chname = GetParameter(m_pOutputBuffer, 2, &chat, &all);

	*chatuser = (atoi(chat));
	*alluser = (atoi(all));
}

/*
	ä���̸� ��������
	Parameter : ä�� �̸�
*/
void Client_Chat::Get_Channel(char **chname)
{
	*chname = m_pOutputBuffer;
}

/*
	�ڽ��� �����ϰ� �ִ� ä�ο��� �÷��̾� ��� ��������
	Parameter : ��������, ���� ����
*/
void Client_Chat::Get_JoinPlayer(char **name, char **info)
{
	GetParameter(m_pOutputBuffer, 2, name, info);
}

/*
	�ڽ��� �����ϰ� �ִ� ä�ο��� ���� �÷��̾� ��� ��������
	Parameter : ��������
*/
void Client_Chat::Get_OutPlayer(char **name)
{
	*name = m_pOutputBuffer;
}

/*
	ä�� �޽��� ��������
	Parameter : ü�� �޽���
*/
void Client_Chat::Get_ChatMessage(char **user, char **msg)
{
	*msg = GetParameter(m_pOutputBuffer, 1, user);
}

/*
	��ŷ ��������
	Parameter : ��ŷ ����
*/
void Client_Chat::Get_Rank(char **rank, char **name, char **vic, char **def, char **dis, char **point)
{
	GetParameter(m_pOutputBuffer, 6, rank, name, vic, def, dis, point);
}

/*
	��� ���� ��������
	Parameter : �����, ��� �̸�
*/
void Client_Chat::Get_Guild(char **master, char **name)
{
	GetParameter(m_pOutputBuffer, 2, master, name);
}

/*
	���� �̸��� ��������
	Parameter : �����̸�
*/
void Client_Chat::Get_Name(char **name)
{
	*name = m_pOutputBuffer;
}

/*
	���� �̸��� ��������
	Parameter : �����̸�
	* ���� name�� gametype���� channel���� gamestate���̴� *
*/
void Client_Chat::Get_Where(char **name, char **gamestate, char **gametype, char **channel)
{
	*channel = GetParameter(m_pOutputBuffer, 3, name, gamestate, gametype);
}
/*
	ChatŬ���̾�Ʈ�� ���η���
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
		
		if(strsize == 0) return 0; // ���� \n���� ���� �Դٸ� �ٽ� recvȣ��
		wRet = Cmd(str);
	}

	if(Writable())
		FlushOutBuffer();

	return wRet;
}

/*
	m_cInBuffer�κ��� '\n'���� ������ �� ������ ��������
	m_iInBufferLen�� ���� ���θ�ŭ ���ҽ�Ų��
	Parameter : recv����, recv���� ũ��, �Ľ̵� ���ڿ�, �Ľ̵� ���ڿ� ũ��
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

	// �� ���ڰ� '\n'���ھ��� �Դٸ�
	if(strsize == len)
	{
		strsize = 0;
		return;
	}
}

/*
	��ɾ��� ���ڵ��� ���´�. ' '�� '\t'���� �и��� �޽������� ������
	Parameter : �Ľ��� ���ڿ�, �Ľ��� ����, �Ľ̵� ���ڿ���
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
	�����κ��� �� �޽����� ������� �˻�
	Parameter : �Ľ̵� ���ڿ�, �Ľ̵� ���ڿ� ����
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
	1. �߰��� ä�� ����Ʈ�� �޴´�. str : channel
	2. ä�ξȿ� �ִ� �������� �޽��� ���� str : chatting messege
	3. ä�ξȿ� �ִ� Ư�� �������� �޽��� ���� str : id, chatting messege
	4. ��ŷ�� ����Ѵ� str : rank message
	5. ���Űź��Ѵ� str : ���Űź� ID
	6. ���Űźθ� �����Ѵ� str : ���Űź����� ID
	7. ��忡 ���� str : ����̸�
	8. ��忡 ���Ծȵ� str : ����̸�
	9. ��忡�� Ż���� str : ����̸�
	10. ��忡�� Ż������ ����
	11. ä���� Join�� �����Ǿ�����	str : Channel name
	12. ������ ���ä�ο� ���������� �ϰ��ִ��� ã�� str : ����̸�
*/
void Client_Chat::Cmd_Default(char *str)
{
	memset(m_pOutputBuffer, NULL, sizeof(m_pOutputBuffer));
	wsprintf(m_pOutputBuffer, "%s", str);
}

/*
	�߰��� �÷��̾� ����Ʈ�� �޴´�
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
		wsprintf(temp, "��%s��%s", guild, id);
		wsprintf(m_pOutputBuffer, "%s %s", temp, detail);
	}
	else
		wsprintf(m_pOutputBuffer, "%s %s", id, detail);
}

/*
	������ �÷��̾� ����Ʈ�� �޴´�.
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
		wsprintf(temp, "��%s��%s", guild, id);
		wsprintf(m_pOutputBuffer, "%s", temp);
	}
	else
		wsprintf(m_pOutputBuffer, "%s", id);
}

/*
	�α����� ID�� �����´�
	str : ID
*/
void Client_Chat::Cmd_OkLogin(char *str)
{
	memset(m_pOutputBuffer, NULL, sizeof(m_pOutputBuffer));
	wsprintf(m_pOutputBuffer, "%s", str);

	strcpy(m_pID, str);
}

/*
	�α� �䱸
	parameter : login�����κ��� ���� guid��
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
	ä�� �޽��� ���� �䱸
	Parameter : ä�� �޽���
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
	�ӼӸ� ���� �䱸
	Parameter : �ӼӸ� ��� ID, ä�� �޽���
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
	ä�� ���� �䱸
	Parameter : Channel �̸�
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
	ID ���Űź� �䱸
	Parameter : ���Űź� ID
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
	ID ���Űź� ���� �䱸
	Parameter : �������� ID
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
	ä�� ����Ʈ �䱸
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
	������ ������ �˸�
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
	������ ���� �˸�
	Parameter : ��(1)��(2)��(3), ����
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
	��忡 ������ (������ ����)
	Parameter : ������� ID
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
	���� ID���� ��忡 ������ �䱸(������� ����)
	Parameter : ���� ID

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
	��忡�� Ż����(������ ����)
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
	������ ���� �˸�, ������ ��ҵǾ�����... (������ ������� �ʴ´�.)
	Parameter : ��(1)��(2)��(3), ����
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
	������ ��Ҹ� �˸�
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
	���� Top10 ��ŷ ���
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
	iRank���� 10���� ��ŷ ���
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
	[ID]�� ������ [� ����]���� [� ä��]�� [��������]�� ���θ� �˾ƿ´�.
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
	������ Send�Լ�
	Parameter : ���� �޽���
*/
void Client_Chat::Msg(char *msg)
{
	char str[CLIENT_CHAT_MAX_BUFFER_SIZE];

	memset(str, NULL, sizeof(str));
	wsprintf(str, "%s\n", msg); 
	Write(str, strlen(str));
}

