//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "JXWorkManager.h"
#include "JXPacketList.h"
#include "JXPacket.h"

JXWorkManager::JXWorkManager()
{
	m_pWorkInfo = NULL;
	m_pPacketList = NULL;

	m_siWorkNum = 0;
	m_siMaxWorkNum = 0;

	m_siCurrentPos = 0;

	InitializeCriticalSection( &m_cs );
}

JXWorkManager::~JXWorkManager()
{
	DeleteCriticalSection( &m_cs );

	if( m_pWorkInfo ) delete [] m_pWorkInfo;

	if( m_pPacketList ) delete m_pPacketList;

}

void JXWorkManager::Create( SI32 siMaxWorkNum )
{
	EnterCriticalSection( &m_cs );

	if( m_pWorkInfo ) delete [] m_pWorkInfo;

	m_pWorkInfo = new JXWORKINFO[ siMaxWorkNum ];

	memset( m_pWorkInfo, 0, sizeof( JXWORKINFO ) * siMaxWorkNum );

	if( m_pPacketList ) delete [] m_pPacketList;

	m_pPacketList = new JXPacketList;

	m_pPacketList->Create( siMaxWorkNum * 2 );

	m_siMaxWorkNum = siMaxWorkNum;
	m_siWorkNum = 0;	

	m_siCurrentPos = 0;

	LeaveCriticalSection( &m_cs );
}

void JXWorkManager::Clear()
{
	EnterCriticalSection( &m_cs );

	for( SI32 i = 0; i < m_siWorkNum; ++i ) {
		
		if( m_pWorkInfo[ i ].pQueryPacket ) 
			m_pPacketList->Remove( (JXObject *)m_pWorkInfo[ i ].pQueryPacket );
		
		if( m_pWorkInfo[ i ].pReportPacket ) 
			m_pPacketList->Remove( (JXObject *)m_pWorkInfo[ i ].pReportPacket );
	}

	memset( m_pWorkInfo, 0, sizeof( JXWORKINFO ) * m_siMaxWorkNum );

	m_siWorkNum = 0;
	m_siCurrentPos = 0;
	
	LeaveCriticalSection( &m_cs );
}

BOOL JXWorkManager::SetQuery( UI16 usPlayer, UI16 usWorkCode, JXPacket *pPacket )
{
	EnterCriticalSection( &m_cs );

	local_bRet = TRUE;
	local_dwWorkID = MAKELONG( usPlayer, usWorkCode );
	
	JXPacket *pPacket2;

	if( m_siWorkNum == m_siMaxWorkNum ) { local_bRet = FALSE;	goto end; }

	pPacket2 = (JXPacket *)m_pPacketList->PushBack();

	if( pPacket2 == NULL ) { local_bRet = FALSE;	goto end; }

	m_pWorkInfo[ m_siWorkNum ].dwWorkID = local_dwWorkID;		
	m_pWorkInfo[ m_siWorkNum ].bDone = FALSE;			
	m_pWorkInfo[ m_siWorkNum ].pQueryPacket = pPacket2;	
	m_pWorkInfo[ m_siWorkNum ].pReportPacket = NULL;	
	
	++m_siWorkNum;

end:
	LeaveCriticalSection( &m_cs );

	return local_bRet;
}

BOOL JXWorkManager::GetQuery( DWORD *pdwWorkID, JXPacket *pPacket )
{
	EnterCriticalSection( &m_cs );

	local_bRet = FALSE;

	if( m_siWorkNum != 0 &&
		m_pWorkInfo[ m_siCurrentPos ].bDone == FALSE &&
		m_pWorkInfo[ m_siCurrentPos ].pQueryPacket != NULL ) {

		*pdwWorkID = m_pWorkInfo[ m_siCurrentPos ].dwWorkID;
		*pPacket = *(m_pWorkInfo[ m_siCurrentPos ].pQueryPacket);

		++m_siCurrentPos;
		if( m_siCurrentPos == m_siWorkNum ) m_siCurrentPos = 0;

		local_bRet = TRUE;

	}

	LeaveCriticalSection( &m_cs );

	return local_bRet;
}

BOOL JXWorkManager::SetReport( DWORD dwWorkID, JXPacket *pPacket )
{
	EnterCriticalSection( &m_cs );

	local_bRet = TRUE;

	JXPacket *pPacket2;

	for( SI32 i = 0; i < m_siWorkNum; ++i ) {
		if( m_pWorkInfo[ i ].dwWorkID == dwWorkID ) {

			pPacket2 = (JXPacket *)m_pPacketList->PushBack();

			if( pPacket2 == NULL ) { local_bRet = FALSE; goto end; }

			m_pWorkInfo[ i ].pReportPacket = pPacket2;
			m_pWorkInfo[ i ].bDone = TRUE;
		}
	}

end:
	LeaveCriticalSection( &m_cs );

	return TRUE;
}

BOOL JXWorkManager::IsWorkDone( UI16 usPlayer, UI16 usWorkCode )
{
	EnterCriticalSection( &m_cs );

	local_bRet = FALSE;
	local_dwWorkID = MAKELONG( usPlayer, usWorkCode );

	for( SI32 i = 0; i < m_siWorkNum; ++i ) {
		if( m_pWorkInfo[ i ].dwWorkID == local_dwWorkID ) {
			local_bRet = m_pWorkInfo[ i ].bDone;
			goto end;
		}
	}

end:
	LeaveCriticalSection( &m_cs );

	return local_bRet;
}

void JXWorkManager::DelWorkInfo( UI16 usPlayer, UI16 usWorkCode )
{
	EnterCriticalSection( &m_cs );

	local_dwWorkID = MAKELONG( usPlayer, usWorkCode );

	for( SI32 i = 0; i < m_siWorkNum; ++i ) {
		if( m_pWorkInfo[ i ].dwWorkID == local_dwWorkID ) {
			
			memcpy( &m_pWorkInfo[ i ], &m_pWorkInfo[ i + 1 ], 
				sizeof( JXWORKINFO ) * ( m_siWorkNum - i - 1 ) );

			--m_siWorkNum;

			if( i <= m_siCurrentPos ) --m_siCurrentPos;
			if( m_siCurrentPos == m_siWorkNum ) m_siCurrentPos = 0;
		}

	}

	LeaveCriticalSection( &m_cs );
}
