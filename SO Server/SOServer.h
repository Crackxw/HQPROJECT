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
	
	SI32		Poll();								// Server Ŭ������ ��� ���� ó��

	
public:	 	
	VOID		OpenConsole();						// ���� ȭ���� ����Ѵ�.	
	SI32		ProcessConsole();					// Ű ó�� �Ѵ�	
	VOID		CloseConsole();						// ���� ȭ���� ����Ѵ�

	
	VOID		Init();								// �������� �ʱ�ȭ�� �Ѵ�
//	VOID		WriteInfo(char *FileName, const char *Content,...);
	VOID		WriteBinary( char *FileName, const char *pContent, int iSize );
	
	// -- ���� ������ ���� 
	// ���Ӽ����� ���� ó���ð� ���� ���� 
	//-------------------------------------------------------------------------------------------------------------------------
	UI32	iCalcTime, iLoopCount, iLoopTime, iLoopS, iLoopMS;       // Loop Timing Vars

    // �ʴ� Cycle���� �˷��ش�.
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
	SOWorld			*clWorld;				// ���� ó���� Ŭ����	
	SOGate			*clGate;				// ���� ó���� Ŭ����	
	SODatabase		*clDatabase;			// �����ͺ��̽� Ŭ����	
	SODatabase		*clLoginDatabase;		// �α��νÿ� ����� �����ͺ��̽� ��ü
	SODatabase		*clItemDatabase;


	// ShutDown ����
	BOOL			bShutDownOk;


	// Console keyboard ��� ���� 
	bool			bConsoleLock;


	// Frame Per Second
	DWORD			dwCurrentFPS;			// ���� Frame Per Second
	DWORD			dwMaxCurrentFPS;		// �ִ� Frame Per Second
	DWORD			dwMinCurrentFPS;		// ��� Farme Per Second

	// World Frame Per Second
	DWORD			dwWorldCurrentFPS;			// ���� World Frame Per Second
	DWORD			dwWorldMaxCurrentFPS;		// �ִ� World Frame Per Second	
	DWORD			dwWorldMinCurrentFPS;		// ��� World Farme Per Second

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

	// ����͸� ���� ����

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
