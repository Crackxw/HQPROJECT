#include "SOMain.h"
#include "SOWorld.h"
#include "SOGate.h"
#include "ReadIP.h"
#include "XPlayer.h"
#include "XPlayerMgr.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// SOGate()의 생성자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOGate::SOGate(SOWorld *clTempWorld, SODatabase *clTempDatabase, SOServer *clTempServer)
{	
	printf("[%d]\n", FD_SETSIZE);

//	char	cHostName[40];
	SI32	iSocketStructSize;	

	// 현재 접속중인 총 사용자 수
	siTotalConnectPlayer	=	0;
	siMaxConnectedPlayer	=	0;
	// 일렬번호 할당용 변수.
	uiAccount				=	0;

	memset(clKillPlayer, 0, ON_MAX_CONNECTION * 4 );
	memset(clDisconnectPlayer, 0, ON_MAX_CONNECTION * 4 );

	// SOServer 클레스의 포인터를 연결한다.
	clServer			= clTempServer;		
	
	// SOWorld 클레스의 포인터를 연결한다.
	clWorld				= clTempWorld;			
	
	// SODatabase 클레스의 포인터를 연결한다.
	clDatabase				= clTempDatabase;
	
	iSocketStructSize	= 0;
	// socket 구조체를 초기화한다.
//	memset(&stSocketStructA, 0, sizeof(stSocketStructA));

	// Internet Family를 초기화한다.
//	stSocketStructA.sin_family = AF_INET;

	// IP 주소를 얻어온다.
//	gethostname(cHostName, 40);

	// Host이름으로부터 IP를 얻어온다.
//	pHostEnt = gethostbyname(cHostName);
		
	// 각각의 IP를 저장한다.
//	h1 = (*pHostEnt).h_addr_list[0][0];
//	h2 = (*pHostEnt).h_addr_list[0][1];
//	h3 = (*pHostEnt).h_addr_list[0][2];
//	h4 = (*pHostEnt).h_addr_list[0][3];

	// IP를 하나의 문자열로 바꾼다.
//	sprintf(cServerIP, "%i.%i.%i.%i", h1, h2, h3, h4);

	// Socket 구조체에 IP를 설정한다.
//	stSocketStructA.sin_addr.s_addr = inet_addr(cServerIP);

	// Socket 구조체에 Port를 설정한다.
//	stSocketStructA.sin_port = htons(CONNECT_PORT);

	// Socket 크기를 설정한다.
	iSocketStructSize	= sizeof(struct sockaddr_in);

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// SOGate의 소멸자
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SOGate::~SOGate()
{
	// 쓰레드 관련....
	m_bShotdown = TRUE;

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Master Socket Set을 초기화하고 MainSocket Descripter들을 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOGate::ResetSocketSets()
{
	return;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속과 해제에 대한 모든 연산 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOGate::Poll(VOID)
{
	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그 아웃 리스트에 등록
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOGate::LogOut(SOPlayer *clTempPlayer)
{
	return;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그 아웃 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID SOGate::Disconnect(VOID)
{
	return;
}
 
void SOGate::InitUDP()
{
	char	LogInServerIP[20];
	SI32	TempPort = 6666;	

	// 로그인 서버이 IP를 얻어온다.	
	
	LogInServerAddr.sin_family = AF_INET;
	// Socket 구조체에 IP를 설정한다.
	LogInServerAddr.sin_addr.s_addr = inet_addr(LogInServerIP);	
	// Socket 구조체에 Port를 설정한다.
	LogInServerAddr.sin_port = htons(TempPort);
	
	// MainSocketA에 socket를 설정한다.
	if((LogInSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		// MainSocket을 설정할 수 없다.
		// Server Shutdown
		siErrCode = 0;
	}
	
}



void SOGate::SendLogInfo(UI32 Info)
{
	BYTE	SendBuf[4];
	UI16	uTemp1, uTemp2;
	
	uTemp1 = LOWORD(Info);
	uTemp2 = HIWORD(Info);
	SendBuf[0] = LOBYTE(uTemp1);
	SendBuf[1] = HIBYTE(uTemp1);
	SendBuf[2] = LOBYTE(uTemp2);
	SendBuf[3] = HIBYTE(uTemp2);

	::sendto(LogInSocket, (char*)SendBuf, 4, 0 , (sockaddr *)&LogInServerAddr, sizeof(sockaddr_in));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// DB 캐시 서버에 클라이언트로서 접속한다. - DB 캐시 서버가 미리 켜 있어야함.
// 작성자 : 이성재
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32 SOGate::ConnectDBCache()
{
	return 2;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Connect를 받는 쓰레드.
// 작성자 : 한동희
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void SOGate::ConnectPoll()
{

}