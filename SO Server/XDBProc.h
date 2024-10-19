//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _XDBPROC_H
#define _XDBPROC_H

#include "..\\XCommon\\jxdef.h"
#include "..\\XCommon\\JXPacket.h"

class JXWorkManager;
class JXDBController;

class XDBProc
{
friend DWORD WINAPI XDBProc_ThreadFunc( LPVOID param );

public:
	XDBProc();
	~XDBProc();

public:
	void			Create( JXWorkManager *pWorkManager, char *pszServerName, char *pszDatabaseName, char *pszUser, char *pszPassword );
	BOOL			Connect();
	void			StartThread();
	void			StopThread();
	void			Run();

private:
	BOOL			m_bThreadRun;
	DWORD			m_dwThreadId;

	HANDLE			m_hStopEvent;
	HANDLE			m_hExitEvent;

private:
	JXDBController	*m_pDBController;
	JXWorkManager	*m_pWorkManager;

private:
	DWORD			local_dwWorkID;
	JXPacket		local_packet;
};

#endif