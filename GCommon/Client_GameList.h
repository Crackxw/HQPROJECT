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

			int		iID;			// �����κ��� �޾ƿ��� Unique ID
			char	cGameName[20];	// ���� ����
			char	cGameVer[10];	// ���� ����
			char	cGameMode[16];	// Demo, Release, DeathMatch, Normal, CTF... etc
			char	cHostName[40];	// ���ӹ� ����
			char	cPassword[20];	// ���ӹ� �н�����
			char	cHostIP[20];	// ���ӹ��� ���� Client�� ip
			int		iHostPort;		// ���ӹ��� ���� Client�� port
			char	cMapName[40];	// ���ӹ��� ���̸�
			int		iNumPlayers;	// ���ӹ濡 ���ӵ� �ο�
			int		iMaxPlayers;	// ���ӹ濡 �����Ҽ� �ִ� �ο�
			char	cHostID[21];	// ���ӹ��� ���������� ID
			char	iState;			// ������ ����(gamestate)
		};
		PacketInfo_struct	m_PacketInfoMine;

	public:
		PacketInfo_struct	m_PacketInfoList[MAX_LIST_NUMBER];

	protected:
		/* m_PacketInfo�� ��Ŷ���·� �ٲ� �����ϱ� ���� ���� */
		char				m_PacketMine[256]; // �ڽ��� Host�� �Ǿ����� ���� ����

		// Join ������ ���� ����
		char				m_PacketList[MAX_LIST_NUMBER][256]; 

		// �ڽ��� ������ ������°�? �ڽ��� ������ٸ� �Ź� heartbeat�� ������
		BOOL				m_bHost;
		BOOL				m_bIPRecv;

#define		NOTHING						0
#define		WAIT						1
#define		GAMELIST_REQUEST			2
#define		GAME_HOST					3
#define		STATE_ERROR					4
		// �ڽ��� ����(Join���� Create����)�� ������
		BYTE				m_cCurrentState;

		// HeartBeat�� ���� ����
		time_t				m_LastTime;
		time_t				m_NowTime;
		char				m_Interval;

		// Receive ����
		char				m_cRecvBuffer[MAX_BUFFER_SIZE_LIST];

		// ���� �� �ּ�
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

		// �ڽ��� ���� ����(�ڽ��� �� ȣ��Ʈ)�� List Server���� ����
		void AddMyGame(int iGamePort, char *cVer, char *cHostName, char *cPassword, 
							char *cMapName, int iPlayer, int iMax, char *cGameMode, char *cHostID);
		// Server���� ���� ����Ʈ �䱸
		void RequestGameList(void); 
		// Server���� �ڽ��� ���� ���� ��û
		void DelMyGame(void); 
		// Server���� �ڽ��� ������ ����Ǿ����� �˸�
		void StartMyGame(void);
		// �ڽ��� ���� ����(�ڽ��� �� ȣ��Ʈ)�� ����� List Server���� ����(�Ϻ��׸� ����)
		void UpdateMyGame(char *cMapName, int iPlayer, int iMax, char *cGameMode);
		// �ڽ��� ���� ����(�ڽ��� �� ȣ��Ʈ)�� ����� List Server���� ����
		void ResetMyGame(char* cVer, char *cHostName, char *cPassword, char *cMapName, int iPlayer, int iMax, char *cGameMode, char *cHostID);

		void Poll(void);

	protected:
		// List Server�� ����
		BOOL MakeSocket(char *pAddress, WORD wPort); 

		inline void MakePacket(void);
		void SendHeartBeat(void); // List Server���� HeartBeat(������Ʈ�� ��������)�� ������
		void SendRoomInfo(void);
		void RecvGameList(char *Buff); // List Server�κ��� Game List�� ���´�
		void RecvServerIP(char *Buff);
		void ParsePacket(short siIndex);

		void ResetSets();
		BOOL Readable();
};

#endif