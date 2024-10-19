#ifndef _JWLISTEN_H
#define _JWLISTEN_H

#define JWLISTEN_ACT_START		1
#define JWLISTEN_ACT_STOP		2
#define JWLISTEN_ACT_CLOSE		3
#define JWLISTEN_ACT_EXIT		4

#define JWLISTEN_STAT_RUN		100
#define JWLISTEN_STAT_STOP		101
#define JWLISTEN_STAT_CLOSE		102

#include "jwbase.h"

class JWServer;

class JWListen 
{

friend unsigned __stdcall listen_run( LPVOID param );

public:
	JWListen();
	virtual ~JWListen();

public:
	void				SetListenPort( UI16 usPort );

	void				Init( JWServer *pServer, UI16 usPort );
	
	void				Start();
	void				Stop();
	void				Close();

private:
	HANDLE				m_hActionEvent;
	UI32				m_uiActionCode;

	UI32				m_uiState;

	BOOL				m_bInitOk;

	UI16				m_usPort;
	SOCKADDR_IN			m_sockaddr;

	JWServer			*m_pServer;

};

#endif
