#ifndef _XCHATSERVER_H	
#define _XCHATSERVER_H	

#include "../JWCommon/jwbase.h"			
#include "../JWCommon/jwpacket.h"		

#include "XRoomMgr.h"
#include "XUserMgr.h"

#include "XRoom.h"
#include "XUser.h"

class JWIOCPServer;
class JWListen;

class XChatServer
{
public:
	XChatServer();
	~XChatServer();

public:
	void				Init( UI16 usPort );
	void				Start();
	void				Run();
	void				ProcUserPacket( XUser *pUser );
	void				LogOutUser( XUser *pUser );

	JWIOCPServer		*m_iocpserver;
	JWListen			*m_listen;

	XUserMgr			*m_pUserMgr;
	XRoomMgr			*m_pRoomMgr;

	UI16				m_usport;

	char				local_buf[ JWPACKET_MAX_SIZE ];


	DWORD				m_dwTick;

};

#endif

