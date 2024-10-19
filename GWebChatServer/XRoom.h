#ifndef _XROOM_H
#define _XROOM_H

#include "chatprotocol.h"

#include "../JWCommon/jwbase.h"

class JWList;

class XUser;

class XRoom
{
	friend class XRoomMgr;

public:
	XRoom();
	~XRoom();

	void				CreateRoom( XRoomMgr *pRoomMgr, JWList *pList, UI16 usRoomNum );

	BOOL				IsOpened() { return m_bOpened; };
	
	UI32				MakeRoom( sRequest_MakeRoom *pMakeRoom, XUser *pUser );

	UI32				JoinRoom( sRequest_JoinRoom *pJoinRoom, XUser *pUser ); 

	void				NotifyUserJoined( XUser *pUser );
	void				NotifyUserExited( XUser *pUser, UI16 usExitCode );

	void				GetRoomViewInfo( sRoomViewInfo *pRoomViewInfo );

	UI32				AddUser( XUser *pUser );
	UI32				DelUser( XUser *pUser, UI16 usExitCode );

	void				KickAll();
	UI32				CloseRoom();

	void				SendPacket( char *pPacket );
	void				SendChatMsg( sRequest_ChatMsg *pChatMsg, XUser *pUser );
	UI32				SendWhisper( sRequest_Whisper *pMsg, XUser *pUser );

	UI32				ChangeRoomInfo( sRequest_ChangeRoomInfo *pMsg, XUser *pUser );
	UI32				ChangeRoomMaster( sRequest_ChangeRoomMaster *pMsg, XUser *pUser );
	UI32				KickUserOut( sRequest_KickUserOut *pMsg, XUser *pUser );

	void				SendUserList( XUser *pUser );

	XUser*				GetUserByID( char *szUserID );
	XUser*				GetUserByName( char *szUserID );

private:
	XRoomMgr			*m_pRoomMgr;

	sRoomInfo			m_roomInfo;

	BOOL				m_bOpened;

	JWList				*m_pUserPtrList;

	char				m_pBuf[ JWPACKET_MAX_SIZE ];

};



#endif