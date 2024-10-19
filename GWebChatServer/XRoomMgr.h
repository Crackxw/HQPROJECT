#ifndef _XROOMMGR_H
#define _XROOMMGR_H

#include "chatprotocol.h"

#include "../JWCommon/jwbase.h"
#include "../JWCommon/JWFQueueMISO.h"

class XChatServer;

class JWObjectPtr;
class JWList;

class XLobby;
class XRoom;
class XUser;

class XRoomMgr
{
public:
	XRoomMgr();
	~XRoomMgr();

public:
	
	void				Create( XChatServer *pServer, SI32 siMaxRoom );

	void				SendRoomList( XUser *pUser );

	UI32				MakeRoom( sRequest_MakeRoom *pMakeRoom, XUser *pUser, UI16 *pOut_usRoomNum );
	UI32				JoinRoom( sRequest_JoinRoom *pJoinRoom, XUser *pUser );
	
	UI32				CloseRoom( UI16 usRoomNum );
	UI32				CloseRoom( XRoom *pRoom );

	SI32				GetNumRoom() { return m_siNumOpenedRoom; };


private:
	XLobby				*m_pLobby;
	XRoom				*m_pRoom;

	JWFQueueMISO		m_emptyRoomList;

	XChatServer			*m_pServer;

	SI32				m_siNumMaxRoom;
	SI32				m_siNumOpenedRoom;
	
};



#endif