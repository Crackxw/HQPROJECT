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
#define TCPIP_MAX_LOCALBUFFER	(TCPIP_MAX_OUT_BUFFER-8)		// OUT�̳� IN ���ۺ��� �ּ��� 8�� �۾ƾ��Ѵ�.
#define BATTLE_PORT				6115
//---------------------------------------------------------------------------
#define MAX_PLAYER			   	8

#define MAX_PACKET_UNIT			128						// �����ϰ����� ��Ŷ������ �ִ� ����
#define PACKET_HEADER_SIZE		8						// �� PACK_UNIT�� ����� ũ��
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
// ���� ����
//---------------------------------------------------------------------------
	int  ServerIPKind;			// � ���� IP�� ������ �޾Ҵ°�?
	char ServerIPAddress[30];	// ������ ������ ���� IP
	char FtpIPAddress[30];		// ���� ��ġ ���� ���� IP

	// ����� ���ڷκ��� ���� IP�� �޴´�. ���д� FALSE
	BOOL GetServerAddress(LPSTR  lpCmdLine);

//---------------------------------------------------------------------------
// Time ���� ���� 
//---------------------------------------------------------------------------
	struct tm       *pNewTime;
	time_t			stClock;
	BOOL			ConnectReminderOk;

//---------------------------------------------------------------------------
// ���� ����
//---------------------------------------------------------------------------
	fd_set  fdError;
	fd_set  fdPlayer;
	SOCKET  Client;						
	timeval	timeout;

	struct	sockaddr_in Server;
    struct	hostent *Hp;
    unsigned long	Addr;

//---------------------------------------------------------------------------
// �ð� üũ ����
//---------------------------------------------------------------------------
	DWORD	GameStartTime;							// ���� ���� �ð��� üũ�Ѵ�.
	BOOL	bGameStarted;							// ������ ���۵Ǿ���.
	DWORD	GameCreateTime;							// ���� ���� �ð��� üũ�Ѵ�.
	BOOL	bGameCreated;							// ������ �����Ǿ���.
	DWORD	GameJoinTime;							// ���ӿ� ���� �ð��� üũ�Ѵ�.
	BOOL	bGameJoined;							// ���ӿ� ����.
	DWORD	GameFindTime;							// ������ �߰��ϴ� �ð��� üũ�Ѵ�.

//---------------------------------------------------------------------------
// ���� ����
//---------------------------------------------------------------------------
	BOOL	ConnectOK;								// �����ߴ°�?
	BOOL	bInBufferCheck;
	char 	cInBuffer[TCPIP_MAX_IN_BUFFER];
	SI32	siInBufferLen;							// ������ ����Ÿ ����

	BOOL	LoopFinishOk;							// SendFinishGame()�ΰ� SendCancelGame()�ΰ�?

	// ����µ������� �ϷĹ�ȣ
	UI16		uiInPacketNumber;
	UI16		uiOutPacketNumber;
	SI32		siSendPacketUnitNumber;
	SI32		siRecvPacketUnitNumber;

	PACKET_UNIT		stSendPacketUnit[MAX_PACKET_UNIT];
	PACKET_UNIT		stRecvPacketUnit[MAX_PACKET_UNIT];

//---------------------------------------------------------------------------
// �ʱ�ȭ�� ó������
//---------------------------------------------------------------------------
	_TcpIp();
	~_TcpIp();
	void InitZero			(void);
	BOOL Init				(char IpAddr[]);
	BOOL InitSocket			(void);
	void Close				(void);

//---------------------------------------------------------------------------
// �⺻ �Լ�
//---------------------------------------------------------------------------
	SI16 Read				(SI32 siBytesToRead);
	BOOL Write				(const void * cMsg, UI16 iBytes);
	void ResetSets			(void);
	BOOL Readable			(void);
	BOOL Writeable			(void);
	void EraseInBuffer		(UI16 uiBytes);

//---------------------------------------------------------------------------
// ��ü ó�� �Լ�
//---------------------------------------------------------------------------
	// ��Ŷ������ ��ȣ�� �����.
	inline SI32  GetNextSendPacketUnitNumber()		{ return (siSendPacketUnitNumber<MAX_PACKET_UNIT)? siSendPacketUnitNumber++ : siSendPacketUnitNumber=0; };
	inline SI32  GetNextRecvPacketUnitNumber()		{ return (siRecvPacketUnitNumber<MAX_PACKET_UNIT)? siRecvPacketUnitNumber++ : siRecvPacketUnitNumber=0; };

//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
public:
	HANDLE		hThreadHandle;
	BOOL		bCloseThread;
	inline static VOID ThreadProc(void *lpParameter){_TcpIp *pTcpIp=NULL;pTcpIp=static_cast<_TcpIp *>(lpParameter);pTcpIp->Communicate();return;};
	VOID Communicate();
//---------------------------------------------------------------------------
// ���� ó�� �Լ�
//---------------------------------------------------------------------------
public:
	bool Read(char *cMsg, UI32 *siLength);

	bool AnalysisPacketUnit();							// ��Ŷ�� �м��Ͽ� ó���Ѵ�.
	bool AddPacketUnit();								// ��Ŷ����Ʈ�� ��Ŷ�� �߰��Ѵ�.
	bool DeletePacketUnit();							// ��Ŷ����Ʈ���� ��Ŷ�� �����Ѵ�.
	void RequestBrokenPacketUnit(SI32 siPacketNumber);	// ���� ��Ŷ�� �ٽ� ���ۿ�û�Ѵ�.
//---------------------------------------------------------------------------
// ��Ÿ �Լ�
//---------------------------------------------------------------------------
};

#endif
