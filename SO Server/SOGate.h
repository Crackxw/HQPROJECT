#ifndef SOGATE_H
#define SOGATE_H
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#include "SOCommonHeader.h"
#include "SOUtility.h"
#include "SOServer.h"
#include "SODataBase.h"
#include "SOPlayer.h"

class SOServer;

class SOGate : virtual SOUtility
{
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 소켓 관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:
	// SOGate의 생성자
	SOGate(SOWorld *, SODatabase *, SOServer *);

	// SOGate의 소멸자
	~SOGate();

	// Socket 처리의 Polling 함수
	VOID		Poll(VOID);			
private:
	// Master Socket Set을 초기화하고 MainSocket Descripter들을 설정한다.
	VOID		ResetSocketSets(VOID);
public:
	SOWorld		*clWorld;
	SOServer	*clServer;
	SODatabase	*clDatabase;

	// Server의 첫번째 IP저장
	//char		cServerIP[16];
	// Server IP의 각 부분별 기억
	UI08		h1, h2, h3, h4;
public:
	// Main Socket 생성에 이용될 Socket Discripter
	SI32		siMainSocketA;

	// binding과 새로운 접속자들을 위한 IP를 얻어오는데 사용되는 구조체
//	struct		sockaddr_in stSocketStructA;
	struct		hostent *pHostEnt;

	// 로그인 서버와 UDP연결하기 위한 
	struct		sockaddr_in LogInServerAddr;
	SOCKET		LogInSocket;

	//	DB cache 서버와 연결하기 위한	 
	struct		sockaddr_in DBAddr;
	SOCKET		DBSocket;

	// 새로운 접속을 검사하는데 사용되는 Set
	fd_set		Master;

	// 읽기 쓰기에 사용되는 Set
	fd_set		AllSockets;

	// 에러 검출에 사용되는 Set
	fd_set		ErrSockets;

	// select()에서 기다릴 time out 시간
	timeval		timeout;

	// 셧다운 변수
	SI32		siErrCode;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속 / 해제 관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
public:
	// 접속해있는 총 사용자 수
	SI32		siTotalConnectPlayer;

	// 최대 접속 한 사용자 수
	SI32		siMaxConnectedPlayer;

	// 일렬번호 할당용 변수.
	UI16		uiAccount;

	// 접속 해제하는 플레이어 클레스
	SOPlayer	*clDisconnectPlayer[ON_MAX_CONNECTION];
	SOPlayer	*clKillPlayer[ON_MAX_CONNECTION];

public:
	void		SendLogInfo(UI32 Info);
	void		InitUDP();
	// 접속 해제를 처리한다.
	VOID		Disconnect();
	// 로그 아웃한다.
	VOID		LogOut(SOPlayer *clTempPlayer);

	// DB Cache서버 관련
	SI32		ConnectDBCache();

	// Connect 쓰레드 관련...
	CRITICAL_SECTION	m_csGate;
	BOOL				m_bShotdown;
	VOID				ConnectPoll();
	HANDLE				m_hConnect;
	inline static unsigned __stdcall ConnectThread(void* pArg)
	{
		printf("Connection Thread Start!!!\n");

		SOGate* pGate = NULL;
		pGate = static_cast<SOGate *>(pArg);
		pGate->ConnectPoll();

		return 0;
	}
};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#endif