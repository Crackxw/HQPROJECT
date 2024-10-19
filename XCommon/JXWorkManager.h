//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXWORKMANAGER_H
#define _JXWORKMANAGER_H

#include "jxdef.h"

class JXPacket;
class JXPacketList;

struct JXWORKINFO
{
	DWORD		dwWorkID;
	BOOL		bDone;
	JXPacket	*pQueryPacket;
	JXPacket	*pReportPacket;
};

class JXWorkManager
{
public:
	JXWorkManager();
	~JXWorkManager();

public:
	void	Create( SI32 siMaxWorkNum );
	void	Clear();

	BOOL	SetQuery( UI16 usPlayer, UI16 usWorkCode, JXPacket *pPacket );

	BOOL	IsWorkDone( UI16 usPlayer, UI16 usWorkCode );
	
	void	DelWorkInfo( UI16 usPlayer, UI16 usWorkCode );

	BOOL	GetQuery( DWORD *pdwWorkID, JXPacket *pPacket );
	BOOL	SetReport( DWORD dwWorkID, JXPacket *pPacket );	



private:
	JXPacketList	*m_pPacketList;
	
	JXWORKINFO		*m_pWorkInfo;

	SI32			m_siWorkNum;
	SI32			m_siMaxWorkNum;

	SI32			m_siCurrentPos;

private:
	CRITICAL_SECTION	m_cs;	

private:
	BOOL			local_bRet;
	DWORD			local_dwWorkID;

};


#endif