#ifndef _TCPIP_H
#define _TCPIP_H

//---------------------------------------------------------------------------
#include <winsock.h>
#include "Directives.H"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#define MAX_STRING_SIZE			256
#define TCPIP_MAX_OUT_BUFFER	10240
#define TCPIP_MAX_IN_BUFFER		10240
#define TCPIP_MAX_LOCALBUFFER	(TCPIP_MAX_OUT_BUFFER-8)		// OUT이나 IN 버퍼보다 최소한 8이 작아야한다.
#define BATTLE_PORT				6115
//---------------------------------------------------------------------------
#define MAX_PLAYER			   	8

#define MAX_PACKET_UNIT			128						// 저장하고있을 패킷유닛의 최대 갯수
#define PACKET_HEADER_SIZE		8						// 한 PACK_UNIT의 헤더의 크기
//---------------------------------------------------------------------------

typedef struct
{
	UI32		uiPacketNumber;
	UI16		uiSize;
	UI16		uiCheck;
	char		cPacket[TCPIP_MAX_LOCALBUFFER];
	VOID		Init() {uiPacketNumber=0;uiSize=0;uiCheck=0, memset(cPacket, 0, sizeof(cPacket));};
} PACKET_UNIT;

class _TcpIp
{
public:
//---------------------------------------------------------------------------
// 개인 정보
//---------------------------------------------------------------------------
	int  ServerIPKind;			// 어떤 서버 IP를 별도로 받았는가?
	char ServerIPAddress[30];	// 별도로 접속할 서버 IP
	char FtpIPAddress[30];		// 오토 패치 받을 서버 IP

	// 명령행 인자로부터 서버 IP를 받는다. 실패는 FALSE
	BOOL GetServerAddress(LPSTR  lpCmdLine);

//---------------------------------------------------------------------------
// Time 상태 변수 
//---------------------------------------------------------------------------
	struct tm       *pNewTime;
	time_t			stClock;
	BOOL			ConnectReminderOk;

//---------------------------------------------------------------------------
// 소켓 변수
//---------------------------------------------------------------------------
	fd_set  fdError;
	fd_set  fdPlayer;
	SOCKET  Client;						
	timeval	timeout;

	struct	sockaddr_in Server;
    struct	hostent *Hp;
    unsigned long	Addr;

//---------------------------------------------------------------------------
// 시간 체크 변수
//---------------------------------------------------------------------------
	DWORD	GameStartTime;							// 게임 시작 시간을 체크한다.
	BOOL	bGameStarted;							// 게임이 시작되었다.
	DWORD	GameCreateTime;							// 게임 개설 시간을 체크한다.
	BOOL	bGameCreated;							// 게임이 개설되었다.
	DWORD	GameJoinTime;							// 게임에 들어가는 시간을 체크한다.
	BOOL	bGameJoined;							// 게임에 들어갔다.
	DWORD	GameFindTime;							// 게임을 발견하는 시간을 체크한다.

//---------------------------------------------------------------------------
// 공용 변수
//---------------------------------------------------------------------------
	BOOL	ConnectOK;								// 접속했는가?
	BOOL	bInBufferCheck;
	char 	cInBuffer[TCPIP_MAX_IN_BUFFER];
	SI32	siInBufferLen;							// 버퍼의 데이타 길이

	BOOL	LoopFinishOk;							// SendFinishGame()인가 SendCancelGame()인가?

	// 입출력데이터의 일렬번호
	UI16		uiInPacketNumber;
	UI16		uiOutPacketNumber;
	SI32		siSendPacketUnitNumber;
	SI32		siRecvPacketUnitNumber;

	PACKET_UNIT		stSendPacketUnit[MAX_PACKET_UNIT];
	PACKET_UNIT		stRecvPacketUnit[MAX_PACKET_UNIT];

//---------------------------------------------------------------------------
// 초기화및 처리관련
//---------------------------------------------------------------------------
	_TcpIp();
	~_TcpIp();
	void InitZero			(void);
	BOOL Init				(char IpAddr[]);
	BOOL InitSocket			(void);
	void Close				(void);

//---------------------------------------------------------------------------
// 기본 함수
//---------------------------------------------------------------------------
	SI16 Read				(SI32 siBytesToRead);
	BOOL Write				(const void * cMsg, UI16 iBytes);
	void ResetSets			(void);
	BOOL Readable			(void);
	BOOL Writeable			(void);
	void EraseInBuffer		(UI16 uiBytes);

//---------------------------------------------------------------------------
// 전체 처리 함수
//---------------------------------------------------------------------------
	// 패킷유닛의 번호를 만든다.
	inline SI32  GetNextSendPacketUnitNumber()		{ return (siSendPacketUnitNumber<MAX_PACKET_UNIT)? siSendPacketUnitNumber++ : siSendPacketUnitNumber=0; };
	inline SI32  GetNextRecvPacketUnitNumber()		{ return (siRecvPacketUnitNumber<MAX_PACKET_UNIT)? siRecvPacketUnitNumber++ : siRecvPacketUnitNumber=0; };

//---------------------------------------------------------------------------
// 쓰레드관련
//---------------------------------------------------------------------------
public:
	HANDLE		hThreadHandle;
	BOOL		bCloseThread;
	inline static VOID ThreadProc(void *lpParameter){_TcpIp *pTcpIp=NULL;pTcpIp=static_cast<_TcpIp *>(lpParameter);pTcpIp->Communicate();return;};
	VOID Communicate();
//---------------------------------------------------------------------------
// 수신 처리 함수
//---------------------------------------------------------------------------
public:
	bool Read(char *cMsg, UI32 *siLength);

	bool AnalysisPacketUnit();							// 패킷을 분석하여 처리한다.
	bool AddPacketUnit();								// 패킷리스트에 패킷을 추가한다.
	bool DeletePacketUnit();							// 패킷리스트에서 패킷을 삭제한다.
	void RequestBrokenPacketUnit(SI32 siPacketNumber);	// 깨진 패킷을 다시 전송요청한다.
//---------------------------------------------------------------------------
// 기타 함수
//---------------------------------------------------------------------------
};

#endif
