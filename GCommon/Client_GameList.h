#ifndef __CLIENTGAMELIST_H__
#define __CLIENTGAMELIST_H__

#include <windows.h>
#include <stdio.h>
#include <time.h>

#define	MAX_LIST_NUMBER			256
#define	MAX_BUFFER_SIZE_LIST	256

static const char *queries[] =
{"gamename","gamever","hostname","password","hostip","hostport","mapname","numplayers","maxplayers", "gamemode","hostid","nlistid","gamestate"};
static const char *gamestate[] = 
{"nothing","waiting","playing","closed"};

class Client_GameList
{
	protected:
		struct PacketInfo_struct
		{
			BOOL	bUsed;

			int		iID;			// 서버로부터 받아오는 Unique ID
			char	cGameName[20];	// 게임 제목
			char	cGameVer[10];	// 게임 버젼
			char	cGameMode[16];	// Demo, Release, DeathMatch, Normal, CTF... etc
			char	cHostName[40];	// 게임방 제목
			char	cPassword[20];	// 게임방 패스워드
			char	cHostIP[20];	// 게임방을 만든 Client의 ip
			int		iHostPort;		// 게임방을 만든 Client의 port
			char	cMapName[40];	// 게임방의 맵이름
			int		iNumPlayers;	// 게임방에 접속된 인원
			int		iMaxPlayers;	// 게임방에 접속할수 있는 인원
			char	cHostID[21];	// 게임방을 개설한자의 ID
			char	iState;			// 게임의 상태(gamestate)
		};
		PacketInfo_struct	m_PacketInfoMine;

	public:
		PacketInfo_struct	m_PacketInfoList[MAX_LIST_NUMBER];

	protected:
		/* m_PacketInfo를 패킷형태로 바꿔 저장하기 위한 버퍼 */
		char				m_PacketMine[256]; // 자신이 Host가 되었을시 게임 정보

		// Join 했을시 게임 정보
		char				m_PacketList[MAX_LIST_NUMBER][256]; 

		// 자신이 게임을 만들었는가? 자신이 만들었다면 매번 heartbeat를 보낸다
		BOOL				m_bHost;
		BOOL				m_bIPRecv;

#define		NOTHING						0
#define		WAIT						1
#define		GAMELIST_REQUEST			2
#define		GAME_HOST					3
#define		STATE_ERROR					4
		// 자신의 상태(Join인지 Create인지)를 전달함
		BYTE				m_cCurrentState;

		// HeartBeat를 위한 설정
		time_t				m_LastTime;
		time_t				m_NowTime;
		char				m_Interval;

		// Receive 버퍼
		char				m_cRecvBuffer[MAX_BUFFER_SIZE_LIST];

		// 소켓 및 주소
		SOCKET				m_Socket;
		sockaddr_in			*m_ServerAddr;
		sockaddr_in			m_SockAddr[2];
		char				m_cServerIP[2][20];

		// FD_SET
		FD_SET				m_fdPlayer;
		FD_SET				m_fdError;
		timeval				m_timeout;

	public:
		Client_GameList(char *pAddress, WORD wPort, char *cGameName);
		~Client_GameList();

		// 자신이 만든 게임(자신이 곧 호스트)을 List Server에게 전송
		void AddMyGame(int iGamePort, char *cVer, char *cHostName, char *cPassword, 
							char *cMapName, int iPlayer, int iMax, char *cGameMode, char *cHostID);
		// Server에게 게임 리스트 요구
		void RequestGameList(void); 
		// Server에게 자신의 게임 삭제 요청
		void DelMyGame(void); 
		// Server에게 자신의 게임이 실행되었음을 알림
		void StartMyGame(void);
		// 자신이 만든 게임(자신이 곧 호스트)을 변경시 List Server에게 전송(일부항목 변경)
		void UpdateMyGame(char *cMapName, int iPlayer, int iMax, char *cGameMode);
		// 자신이 만든 게임(자신이 곧 호스트)을 변경시 List Server에게 전송
		void ResetMyGame(char* cVer, char *cHostName, char *cPassword, char *cMapName, int iPlayer, int iMax, char *cGameMode, char *cHostID);

		void Poll(void);

	protected:
		// List Server로 접속
		BOOL MakeSocket(char *pAddress, WORD wPort); 

		inline void MakePacket(void);
		void SendHeartBeat(void); // List Server에게 HeartBeat(업데이트된 게임정보)를 보낸다
		void SendRoomInfo(void);
		void RecvGameList(char *Buff); // List Server로부터 Game List를 얻어온다
		void RecvServerIP(char *Buff);
		void ParsePacket(short siIndex);

		void ResetSets();
		BOOL Readable();
};

#endif