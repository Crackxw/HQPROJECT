#ifndef _XUSER_H
#define _XUSER_H

#include "../JWCommon/JWObject.h"

#include "chatprotocol.h"

class XRoom;
class JWSocket;

class XUser : public JWObject
{
	friend  class XUserMgr;
	friend	class XRoom;

public:
	XUser();
	virtual ~XUser();

public:
	virtual void		Init();
	virtual void		Close();

	JWSocket*			GetSocket() { return (JWSocket *)GetFriend(); };

	UI32				LogIn( sRequest_LogIn *pLogin );

	void				SetUserType( UI16 usUserType ) { m_userInfo.usUserType = usUserType; };
	BOOL				IsRoomMaster() { return ( m_userInfo.usUserType == USERTYPE_ROOMMASTER ); };

	void				SetRoom( XRoom *pRoom ) { m_pRoom = pRoom; };
	XRoom*				GetRoom() { return m_pRoom; };

	void				GetUserViewInfo( sUserViewInfo *pUserViewInfo );

	void				SendPacket( char *pPacket );
	void				SendChatMsg( sRequest_ChatMsg *pChatMsg );

private:
	sUserInfo			m_userInfo;

	XRoom				*m_pRoom;

	BOOL				m_bLogIn;


};


#endif