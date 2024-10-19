#ifndef _XCHATSERVER_H	
#define _XCHATSERVER_H	

#include "../JWCommon/jwbase.h"			
#include "../JWCommon/jwpacket.h"
#include <tchar.h>		

class JWIOCPServer;
class JWListen;

class JWObjectPtr;
class JWList;

class XPlayerMgr;
class XPlayer;
class XMap;

class XChatServer
{
public:
	XChatServer();
	~XChatServer();

public:
	void				Init( UI16 usPort );
	void				Start();
	void				Run();
	void				ProcPlayerPacket( XPlayer *pPlayer );
	void				LogOutPlayer( XPlayer *pPlayer );
	bool				WriteInfo( char *FileName, const char *Content,... );
	XPlayer*			GetPlayer( char *szID );
	void				GetDateString( char *szDate );
	void				WriteNormalChatLog( XPlayer *pPlayer );
	void				WriteWhisperChatLog( XPlayer *pPlayer, XPlayer *pDestPlayer );

	JWIOCPServer		*m_iocpserver;
	JWListen			*m_listen;

	XPlayerMgr			*m_pPlayerMgr;

	JWObjectPtr			*m_pPlayerPtr;
	JWList				*m_pPlayerPtrList;

	XMap				*m_pMap;

	UI16				m_usport;

	UI16				local_usCmd;
	JWPacket			local_packetA;
	JWPacket			local_packetB;

	char				local_buf[ 1024 ];
	

private : 	
	void				ChatErrorLog(char* pFileName, const char* pLog, ...);
};

#endif

