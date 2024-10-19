#include "Ip.h"
#include "Client_GameList.h"

/*
	생성자
*/
Client_GameList::Client_GameList(char *pAddress, WORD wPort, char *cGameName)
{
	WSADATA		WsaData;
	if(WSAStartup(0x101, &WsaData) == SOCKET_ERROR)
		WSACleanup();

	memset(&m_PacketInfoMine, 0, sizeof(m_PacketInfoMine));

	m_timeout.tv_sec = 0;
	m_timeout.tv_usec = 0;
	
	m_bHost = FALSE;
	m_Interval = 10;
	m_bIPRecv = FALSE;

	MakeSocket(pAddress, wPort);

	strcpy(m_PacketInfoMine.cGameName, cGameName); 
	
	m_cCurrentState = NOTHING;
}

/*
	소멸자
*/
Client_GameList::~Client_GameList()
{
	if(m_bHost)
		DelMyGame();
	closesocket(m_Socket);
	WSACleanup();
}

/*
	List 서버로  통신하기 위한 소켓 설정
*/
BOOL Client_GameList::MakeSocket(char *pAddress, WORD wPort)
{
	// 연결 할 곳(게임 서버)주소 설정
	m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		return FALSE;
	}

	memset(&m_SockAddr[0], 0, sizeof(SOCKADDR_IN));
	m_SockAddr[0].sin_addr.s_addr = inet_addr(pAddress); // it's ip address?
	if(m_SockAddr[0].sin_addr.s_addr == -1) // it's host name? 
	{
		struct hostent *h; 
		h = gethostbyname(pAddress);
		if(!h) return FALSE;
		memcpy(&m_SockAddr[0].sin_addr.s_addr, h->h_addr, h->h_length);
	}

	m_SockAddr[0].sin_family = AF_INET;           
	m_SockAddr[0].sin_port = htons(wPort);
	memset(&(m_SockAddr[0].sin_zero), NULL, 8);

	m_ServerAddr = &m_SockAddr[0];

	return TRUE;
}

/*
	로그인 List Server로부터 게임 List Server의 IP를 받는다
*/
void Client_GameList::RecvServerIP(char *Buff)
{
	//	m_SockAddr[1] 설정. Buff를 m_cServerIP로
	char temp[32];
	char *pIP;
	strcpy(temp, Buff);
	pIP = strtok(temp, "\\");
	strcpy(m_cServerIP[1], pIP);

	m_SockAddr[1].sin_family = AF_INET;
	m_SockAddr[1].sin_addr.s_addr = inet_addr(m_cServerIP[1]);
	m_SockAddr[1].sin_port = htons(2345);

	m_ServerAddr = &m_SockAddr[1];
	m_Interval = 30;
	m_bIPRecv = TRUE;

	if(m_cCurrentState == GAME_HOST)
	{
		pIP = strtok(NULL, "\\");
		if(pIP)
			m_PacketInfoMine.iID = atoi(pIP);
		m_PacketInfoMine.iState = 1;
		SendRoomInfo();
	}
	if(m_cCurrentState == GAMELIST_REQUEST)
		RequestGameList();
}

/*
	게임 정보를 요청할때 또는 Refresh를 요구할때
*/
void Client_GameList::RequestGameList(void)
{
	char SendBuff[64];

	memset(SendBuff, 0, sizeof(SendBuff));
	wsprintf(SendBuff, "\\status\\%s\\", m_PacketInfoMine.cGameName);
	sendto(m_Socket, SendBuff, strlen(SendBuff), 0, (sockaddr *)m_ServerAddr, sizeof(sockaddr_in));
	// GameList를 받기위해 리스트를 초기화
	for(int i=0; i<MAX_LIST_NUMBER; i++)
		memset(&m_PacketInfoList[i], 0, sizeof(PacketInfo_struct));

	m_cCurrentState = GAMELIST_REQUEST;
}

/*
	리스트 서버에게 HeartBeat를 보낸다
*/
void Client_GameList::SendHeartBeat(void)
{
	char SendBuff[256];

	memset(SendBuff, 0, sizeof(SendBuff));

	if(m_bIPRecv)
		wsprintf(SendBuff, "\\heartbeat\\%d\\nlistid\\%d\\gamestate\\%d\\", m_PacketInfoMine.iHostPort, m_PacketInfoMine.iID, m_PacketInfoMine.iState);
	else
		wsprintf(SendBuff, "\\heartbeat\\%d\\gamename\\%s\\", m_PacketInfoMine.iHostPort, m_PacketInfoMine.cGameName);
	sendto(m_Socket, SendBuff, strlen(SendBuff), 0, (sockaddr*)m_ServerAddr, sizeof(sockaddr_in));
	m_LastTime = m_NowTime;
}

/*
	방을 처음 만들때 호출
*/
void Client_GameList::AddMyGame(int iGamePort, char *cVer, char *cHostName, char *cPassword, 
							char *cMapName, int iPlayer, int iMax, char *cGameMode, char *cHostID)
{	
	m_bHost = TRUE;

	m_PacketInfoMine.bUsed = TRUE;
	m_PacketInfoMine.iHostPort = iGamePort;
	m_PacketInfoMine.iID = 0;
	strcpy(m_PacketInfoMine.cGameVer, cVer);
	strcpy(m_PacketInfoMine.cHostName, cHostName);
	if(cPassword)
		strcpy(m_PacketInfoMine.cPassword, cPassword);
	strcpy(m_PacketInfoMine.cMapName, cMapName);
	m_PacketInfoMine.iNumPlayers = iPlayer;
	m_PacketInfoMine.iMaxPlayers = iMax;
	strcpy(m_PacketInfoMine.cGameMode, cGameMode);
	strcpy(m_PacketInfoMine.cHostID, cHostID);
	m_PacketInfoMine.iState = 1;
	MakePacket();

	time(&m_NowTime);
	m_cCurrentState = GAME_HOST;
	if(!m_bHost)
		m_ServerAddr = &m_SockAddr[0];
	SendHeartBeat();
}

/*
	List Server로 보낼 Packet을 만든다
*/
inline void Client_GameList::MakePacket(void)
{
	if(strlen(m_PacketInfoMine.cPassword))
		wsprintf(m_PacketMine,
			"\\gamename\\%s\\gamever\\%s\\hostname\\%s\\password\\%s\\mapname\\%s\\numplayers\\%d\\maxplayers\\%d\\gamemode\\%s\\hostid\\%s\\nlistid\\%d\\",
			m_PacketInfoMine.cGameName,
			m_PacketInfoMine.cGameVer,
			m_PacketInfoMine.cHostName,
			m_PacketInfoMine.cPassword,
			m_PacketInfoMine.cMapName,
			m_PacketInfoMine.iNumPlayers,
			m_PacketInfoMine.iMaxPlayers,
			m_PacketInfoMine.cGameMode,
			m_PacketInfoMine.cHostID,
			m_PacketInfoMine.iID);
	else
		wsprintf(m_PacketMine,
			"\\gamename\\%s\\gamever\\%s\\hostname\\%s\\mapname\\%s\\numplayers\\%d\\maxplayers\\%d\\gamemode\\%s\\hostid\\%s\\nlistid\\%d\\",
			m_PacketInfoMine.cGameName,
			m_PacketInfoMine.cGameVer,
			m_PacketInfoMine.cHostName,
			m_PacketInfoMine.cMapName,
			m_PacketInfoMine.iNumPlayers,
			m_PacketInfoMine.iMaxPlayers,
			m_PacketInfoMine.cGameMode,
			m_PacketInfoMine.cHostID,
			m_PacketInfoMine.iID);
}

/*
	자신의 방정보를 업데이트 시켰을때 업데이트된 정보를 서버로 보내기
*/
void Client_GameList::UpdateMyGame(char *cMapName, int iPlayer, int iMax, char *cGameMode)
{
	strcpy(m_PacketInfoMine.cMapName, cMapName);
	m_PacketInfoMine.iNumPlayers = iPlayer;
	m_PacketInfoMine.iMaxPlayers = iMax;
	strcpy(m_PacketInfoMine.cGameMode, cGameMode);

	SendRoomInfo();
}

void Client_GameList::ResetMyGame(char* cVer, char *cHostName, char *cPassword, char *cMapName, int iPlayer, int iMax, char *cGameMode, char *cHostID)
{
	strcpy(m_PacketInfoMine.cGameVer, cVer);
	strcpy(m_PacketInfoMine.cHostName, cHostName);
	if(cPassword)
		strcpy(m_PacketInfoMine.cPassword, cPassword);
	strcpy(m_PacketInfoMine.cMapName, cMapName);
	m_PacketInfoMine.iNumPlayers = iPlayer;
	m_PacketInfoMine.iMaxPlayers = iMax;
	strcpy(m_PacketInfoMine.cGameMode, cGameMode);
	strcpy(m_PacketInfoMine.cHostID, cHostID);

	SendRoomInfo();
}

/*

*/
void Client_GameList::SendRoomInfo(void)
{
	if(!m_bIPRecv)
	{
		SendHeartBeat();
		return;
	}
	MakePacket();
	sendto(m_Socket, m_PacketMine, strlen(m_PacketMine), 0, (sockaddr*)m_ServerAddr, sizeof(sockaddr_in));
}

/*
	Playing 중에도 게임 참여가 가능한 게임만 호출(GEARS)
	Playing 중에 게임 참여가 불가능한 게임은 DelMyGame을 호출(SYW2, Syw2Plus, Legend, Kohan)
*/
void Client_GameList::StartMyGame(void)
{
	m_PacketInfoMine.iState = 2;
	SendHeartBeat();
}

/*

*/
void Client_GameList::Poll(void)
{
	sockaddr_in TempAddr;
	int	Size = sizeof(sockaddr_in);

	if(Readable())
	{
		memset(m_cRecvBuffer, 0, MAX_BUFFER_SIZE_LIST);
		recvfrom(m_Socket, m_cRecvBuffer, MAX_BUFFER_SIZE_LIST, 0, (sockaddr*)&TempAddr, &Size);
		if(strstr(m_cRecvBuffer, "gamename"))
			RecvGameList(m_cRecvBuffer);
		if(strstr(m_cRecvBuffer, "serverip"))
			RecvServerIP(&m_cRecvBuffer[10]);
		if(strstr(m_cRecvBuffer, "notcertified"))
		{
			m_ServerAddr = &m_SockAddr[0];
			m_Interval = 10;
			m_bIPRecv = FALSE;
		}
	}

	time(&m_NowTime);
	if((m_bHost == TRUE) && (difftime(m_NowTime, m_LastTime) > m_Interval))
		SendHeartBeat();
}

/*

*/
void Client_GameList::ParsePacket(short siIndex)
{
	int i =0;
	int	j =0;
	int k =0;
	int h =0;
	char *Temp[30];
	char *Parse[11];
	char *token;

	for(h=0; h<11; h++)
		Parse[h] = NULL;
		
	token = strtok(m_PacketList[siIndex], "\\");
	while(token != NULL)
	{
		Temp[i++] = token;
		token = strtok(NULL, "\\");
	}

	for(j=0; j<11; j++)
	{
		for(k=0; k<i; k++)
		{
			if(strstr(Temp[k], queries[j]) != NULL)
			{
				Parse[j] = Temp[k+1];
				break;
			}
		}
	}
	
	if(Parse[0])
		strcpy(m_PacketInfoList[siIndex].cGameName, Parse[0]);	
	if(Parse[1])
		strcpy(m_PacketInfoList[siIndex].cGameVer, Parse[1]);
	if(Parse[2])
		strcpy(m_PacketInfoList[siIndex].cHostName, Parse[2]);
	if(Parse[3])
		strcpy(m_PacketInfoList[siIndex].cPassword, Parse[3]);
	if(Parse[4])
		strcpy(m_PacketInfoList[siIndex].cHostIP, Parse[4]);
	if(Parse[5])
		sscanf(Parse[5], "%d", &m_PacketInfoList[siIndex].iHostPort);
	if(Parse[6])
		strcpy(m_PacketInfoList[siIndex].cMapName, Parse[6]);
	if(Parse[7])
		sscanf(Parse[7], "%d", &m_PacketInfoList[siIndex].iNumPlayers);
	if(Parse[8])
		sscanf(Parse[8], "%d", &m_PacketInfoList[siIndex].iMaxPlayers);
	if(Parse[9])
		strcpy(m_PacketInfoList[siIndex].cGameMode, Parse[9]);
	if(Parse[10])
		strcpy(m_PacketInfoList[siIndex].cHostID, Parse[10]);
	if(Parse[11])
		sscanf(Parse[10], "%d", &m_PacketInfoList[siIndex].iID);
	if(Parse[12])
		sscanf(Parse[11], "%d", &m_PacketInfoList[siIndex].iState);
}

/*

*/
void Client_GameList::RecvGameList(char *Buff)
{
	short wEmpty;

	for(wEmpty=0; wEmpty<MAX_LIST_NUMBER; wEmpty++)
		if(!m_PacketInfoList[wEmpty].bUsed)
			break;

	if(wEmpty==MAX_LIST_NUMBER)
		return;

	memcpy(&m_PacketList[wEmpty], Buff, sizeof(m_PacketList[wEmpty]));
	m_PacketInfoList[wEmpty].bUsed = TRUE;
	ParsePacket(wEmpty);
}

/*
	자신이 만든 게임을 서버 List에서 삭제
*/
void Client_GameList::DelMyGame(void)
{	
	char	SendBuff[256];
	memset(SendBuff, 0, sizeof(SendBuff));

	m_PacketInfoMine.iState = 3;
	SendHeartBeat();
	m_ServerAddr = &m_SockAddr[0];
	SendHeartBeat();

	m_PacketInfoMine.iID = 0;
	m_bHost = FALSE;
	m_bIPRecv = FALSE;
}

inline void Client_GameList::ResetSets(void)
{
	FD_ZERO(&m_fdError);
	FD_ZERO(&m_fdPlayer);
	FD_SET(m_Socket, &m_fdError);
	FD_SET(m_Socket, &m_fdPlayer);
}

inline BOOL Client_GameList::Readable(void)	
{
	ResetSets();

	select(m_Socket, &m_fdPlayer, NULL, &m_fdError, &m_timeout);
	if(FD_ISSET(m_Socket, &m_fdError))
		return FALSE;

	else if(FD_ISSET(m_Socket, &m_fdPlayer))
		return TRUE;
			
	return FALSE;
}
