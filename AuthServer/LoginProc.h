#ifndef __LOGINPROC_H
#define __LOGINPROC_H

#include "..\\JWCommon\\jwbase.h"

#define LOGIN_ACT_START 1
#define	LOGIN_ACT_STOP	2

#define LOGIN_STAT_STOP	1
#define LOGIN_STAT_RUN	2

class SOServer;

class cLoginProc
{
public:
	cLoginProc();
	~cLoginProc();

public:
	void	SetServer( SOServer *pServer ) { m_pServer = pServer; };
	void	Start();
	void	Stop();
	void	Run();

public:
	SOServer	*m_pServer;

	SI32		m_siActionCode;
	HANDLE		m_hEvent;

	SI32		m_siState;

	char		m_pmsg[ 30000 ];
};

#endif
