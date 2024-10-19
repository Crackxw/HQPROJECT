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
	BOOL		m_bEvent;						// ���� �̺�Ʈ. �ƽ�~
	
public:
	SOGameServerParser();						// ������.
	virtual	~SOGameServerParser();				// �Ҹ���.

	BOOL	Load(CHAR *pszFileName);			// ȭ���� �ε��Ѵ�.
	BOOL	Load(void);							// Config file load.	

	CHAR*   GetAuthServerIP();					// ���� ���� IP�� ���´�.
	CHAR*	GetFrontServerIP();					// ����Ʈ ���� IP�� ���´�.

	CHAR*	GetDB1IPString();					// DB1 ��Ʈ�� IP�� ���´�.
	CHAR*	GetDB2IPString();					// DB2 ��Ʈ�� IP�� ���´�.
	CHAR*	GetDBName1String();					// DB1 �̸� ��Ʈ���� ���´�.	
	CHAR*	GetDBName2String();					// DB2 �̸� ��Ʈ���� ���´�.

	CHAR*	GetDBUserString();					// DB ����� ��Ʈ���� ���´�.

	CHAR	GetLanguageCode();					// Language Code�� ����
	WORD	GetWorldID();						// World�� ID�� ���� 

	void	SetConcurrent(WORD concurrent);
	WORD	GetConcurrent();
	WORD	GetNumAllowedConcurrentUser();

	BOOL	IsTestServer();						// Test Server���� ����
	BOOL	IsGameGuard(VOID);
	BOOL	IsGameEvent();
};


#endif
