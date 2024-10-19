#ifndef SOSERVER_H
#define SOSERVER_H

#include "SOCommonHeader.h"
#include "SOUtility.h"
#include "SOGate.h"
#include "SODataBase.h"
#include ".\RegControl\RegControl.h"

class	SOGate;
class	SOUtility;

#include "..\\XCommon\\JXPacket.h"

class JXConnectSocket;
class JXSocketMgr;
class XPlayerMgr;
class XPlayer;
class XMonitor; 

class JWListen;
class JWIOCPServer;

class SOServer  
{

public:	
	SOServer(); 	
	~SOServer();

public:
	
	SI32		Poll();								// Server 클레스의 모든 연산 처리

	
public:	 	
	VOID		OpenConsole();						// 시작 화면을 출력한다.	
	SI32		ProcessConsole();					// 키 처리 한다	
	VOID		CloseConsole();						// 종료 화면을 출력한다

	
	VOID		Init();								// 변수들을 초기화를 한다
//	VOID		WriteInfo(char *FileName, const char *Content,...);
	VOID		WriteBinary( char *FileName, const char *pContent, int iSize );
	
	// -- 현재 쓰이지 않음 
	// 게임서버의 루프 처리시간 측정 관련 
	//-------------------------------------------------------------------------------------------------------------------------
	UI32	iCalcTime, iLoopCount, iLoopTime, iLoopS, iLoopMS;       // Loop Timing Vars

    // 초당 Cycle수를 알려준다.
	inline R64  ServerLoad(){R64 fLastLoad = (CLOCKS_PER_SEC*(1.0/(R64)((R64)iLoopTime/(R64)iLoopCount)));  return fLastLoad;};

	#ifdef _WIN32
	inline void StartMilliTimer(UI32 &Seconds, UI32 &Milliseconds) {struct timeb t;ftime(&t);Seconds = t.time;Milliseconds = t.millitm;};
	inline UI32 CheckMilliTimer(UI32 &Seconds, UI32 &Milliseconds) {struct timeb t;ftime(&t);return(CLOCKS_PER_SEC*(t.time - Seconds) + (t.millitm - Milliseconds));};
	#endif

	#ifdef __linux__
	inline void StartMilliTimer(UI32 &Seconds, UI32 &Milliseconds) {struct timeval t;gettimeofday(&t,NULL);Seconds = t.tv_sec;Milliseconds = t.tv_usec;};
	inline UI32 CheckMilliTimer(UI32 &Seconds, UI32 &Milliseconds) {struct timeval t;gettimeofday(&t,NULL);return(CLOCKS_PER_SEC*(t.tv_sec - Seconds) + (t.tv_usec - Milliseconds));};
	#endif
	//--------------------------------------------------------------------------------------------------------------------------
	

public:
	SOWorld			*clWorld;				// 연산 처리용 클레스	
	SOGate			*clGate;				// 접속 처리용 클레스	
	SODatabase		*clDatabase;			// 데이터베이스 클레스	
	SODatabase		*clLoginDatabase;		// 로그인시에 사용할 데이터베이스 객체
	SODatabase		*clItemDatabase;


	// ShutDown 변수
	BOOL			bShutDownOk;


	// Console keyboard 잠금 변수 
	bool			bConsoleLock;


	// Frame Per Second
	DWORD			dwCurrentFPS;			// 현재 Frame Per Second
	DWORD			dwMaxCurrentFPS;		// 최대 Frame Per Second
	DWORD			dwMinCurrentFPS;		// 취소 Farme Per Second

	// World Frame Per Second
	DWORD			dwWorldCurrentFPS;			// 현재 World Frame Per Second
	DWORD			dwWorldMaxCurrentFPS;		// 최대 World Frame Per Second	
	DWORD			dwWorldMinCurrentFPS;		// 취소 World Farme Per Second

	DWORD			dwSmsLastTime;
	
public:
	void			ProcPacketPlayer( XPlayer *pXPlayer );
	void			ProcPacketMonitor( XMonitor *pMonitor );

	void			NewPlayer( XPlayer *pXPlayer );

	void			LogOutPlayer( UI16 usAccount );
	void			LogOutPlayer( XPlayer *pXPlayer );
	void			__LogOutPlayer( XPlayer *pXPlayer );
	void			LogOutPlayerAll();

	void			CompleteLogOut( XPlayer *pXPlayer );

	void			SendHeartBeat();

	// 모니터링 서버 관련

	JXConnectSocket		*m_pMonitorSocket;
	XMonitor			*m_pMonitor;

	XPlayerMgr			*m_pXPlayerMgr;

	JWListen			*m_pListen;
	JWIOCPServer		*m_pIOCPServer;

public:
	JXPacket			local_packetA;
	JXPacket			local_packetB;
	JXPacket			local_packetC;
	JXPacket			local_packetD;
	JXPacket			local_packetE;

	FLOAT	fFPS					;
	FLOAT	fWriteCPS				;
	FLOAT	fGetDBCPS				;


};


#endif
