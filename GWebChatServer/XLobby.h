#ifndef _XLOBBY_H
#define _XLOBBY_H

#include "../JWCommon/jwbase.h"

class JWList;

class XUser;

class XLobby
{
public:
	XLobby();
	~XLobby();

	void				CreateLobby( JWList *pList );	

	void				AddUser( XUser *pUser );
	void				DelUser( XUser *pUser );

	void				SendPacket( char *pPacket );


public:

	JWList				*m_pUserPtrList;

};



#endif