#ifndef __AUTHPROC_H
#define __AUTHPROC_H

#include "..\\JWCommon\\jwbase.h"
#include "../AuthServer/_LoginDefine.h"



#define AUTH_ACT_START	1
#define	AUTH_ACT_STOP	2

#define AUTH_STAT_STOP	1
#define AUTH_STAT_RUN	2



class SOServer;
class SOPlayer;
class CAuthServerProc;



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// class : 
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class cAuthProc
{

public:
	CAuthServerProc*	m_pAuthServer;

	SOServer*			m_pServer;	

	SI32				m_siActionCode;
	HANDLE				m_hEvent;

	SI32				m_siState;

	char				m_pmsg[ 30000 ];


public:
	cAuthProc();
	~cAuthProc();


public:
	void			SetServer( SOServer *pServer ) { m_pServer = pServer; };
	void			setAuthServerProc( CAuthServerProc* pAuthServer ) { m_pAuthServer = pAuthServer; }

	void			Start();
	void			Stop();
	void			Run();

	int				recvRequestLogin   ( SOPlayer* pPlayer, LOGIN::sRequestLogin* pData );
	void			sendResponseLogin  ( SOPlayer* pPlayer, int responseKind );

};

#endif