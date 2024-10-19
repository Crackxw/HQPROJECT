#ifndef	SO_GAMESERVER_PARSER_HEADER
#define SO_GAMESERVER_PARSER_HEADER

#include <windows.h>

#include "..\SCCommon\JOGersangConfig.h"

class	SOGameServerParser : public CJOGersangConfig
{

private:
	CHAR		szAuthServerIP[16];
	CHAR		szFrontServerIP[16];

	CHAR		szDB1IP[16];
	CHAR		szDB2IP[16];
	CHAR		szDBName1[256];
	CHAR		szDBName2[256];

	CHAR		szDBUser[256];
	CHAR		szLanguageCode[4];

	BOOL		m_bTestServer;
	WORD		m_numAllowedConcurrentUser;
	WORD		m_worldID;
	WORD		m_concurrent;
	BOOL		m_bGameGuard;
	BOOL		m_bEvent;						// 게임 이벤트. 아싸~
	
public:
	SOGameServerParser();						// 생성자.
	virtual	~SOGameServerParser();				// 소멸자.

	BOOL	Load(CHAR *pszFileName);			// 화일을 로드한다.
	BOOL	Load(void);							// Config file load.	

	CHAR*   GetAuthServerIP();					// 인증 서버 IP를 얻어온다.
	CHAR*	GetFrontServerIP();					// 프론트 서버 IP를 얻어온다.

	CHAR*	GetDB1IPString();					// DB1 스트링 IP를 얻어온다.
	CHAR*	GetDB2IPString();					// DB2 스트링 IP를 얻어온다.
	CHAR*	GetDBName1String();					// DB1 이름 스트링을 얻어온다.	
	CHAR*	GetDBName2String();					// DB2 이름 스트링을 얻어온다.

	CHAR*	GetDBUserString();					// DB 사용자 스트링을 얻어온다.

	CHAR	GetLanguageCode();					// Language Code를 얻어옴
	WORD	GetWorldID();						// World의 ID를 얻어옴 

	void	SetConcurrent(WORD concurrent);
	WORD	GetConcurrent();
	WORD	GetNumAllowedConcurrentUser();

	BOOL	IsTestServer();						// Test Server인지 구분
	BOOL	IsGameGuard(VOID);
	BOOL	IsGameEvent();
};


#endif
