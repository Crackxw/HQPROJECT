// IPartyMgr.cpp: implementation of the IPartyMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "IPartyMgr.h"

#ifdef _VERIFY_PARTY_PACKET
#define OUTPUTDEBUGSTRING(x) OutputDebugString(x)
#else
#define OUTPUTDEBUGSTRING(x)
#endif

inline void TRACE_LOG(char *szFormat, ...)
{
#ifdef _VERIFY_PARTY_PACKET
#pragma message(" _VERIFY_PARTY_PACKET :: TRACE_LOG Function compiled\n")

	char szBuffer[1024];
    char *pArguments;
	
	pArguments=(char*)&szFormat+sizeof(szFormat);
	vsprintf(szBuffer, szFormat, pArguments);

	OUTPUTDEBUGSTRING( szBuffer );

#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParty :: CParty()
{
	m_nCurJoiner	=	0;	// 현재 파티 인원수
	m_uiId			=	0;
};

CParty::~CParty()
{
	// clear 
}

/**




*/
VOID	CParty::Init(UI16 uiPartyId)
{
	SI32	i;

	m_nCurJoiner	=	0;
	m_uiId			=	uiPartyId;
	m_bUsing		=	FALSE;
	
	for( i = 0; i < nMaxPerson; i++ )
	{
		m_members_t[ i ].clear();
	}
}


VOID	CParty::Create( UI16 uiAccount, CHAR* pszId, SI32 siLevel )
{
	m_leader_t.uiAccount    =	uiAccount;
	m_leader_t.siLevel		=	siLevel;

	ZeroMemory( m_leader_t.szId, sizeof( m_leader_t.szId ) );
	strncpy( m_leader_t.szId,  pszId, ON_ID_LENGTH );

	Join( uiAccount, pszId, siLevel );

	m_bUsing = TRUE;
}

/**


*/
BOOL	CParty::Join( UI16 uiAccount, CHAR* pszId, SI32 siLevel )
{
	SI32	i;

	// 현재 파티가 풀 인가?
	if ( CanJoin() == FALSE )								return FALSE;

	for( i = 0; i < nMaxPerson; i++ )
	{
		if( m_members_t[ i ].bUsing == FALSE )
		{
			m_members_t[ i ].uiAccount =	uiAccount;
			m_members_t[ i ].siLevel   =	siLevel;
			m_members_t[ i ].bUsing		=	TRUE;

			ZeroMemory( m_members_t[ i ].szId, sizeof( m_members_t[ i ].szId ) );
			strncpy( m_members_t[ i ].szId, pszId, ON_ID_LENGTH );

			m_nCurJoiner++;

			break;
		}
	}

	return TRUE;
}

/**

	m_nMinPlayerLevelLimit, m_nMaSOPlayerLevelLimit 적용 
	파장 변경

*/
UI16	CParty::Leave( UI16 uiAccount )
{
	SI32	i;

	assert( uiAccount >= 0 && uiAccount < INITIAL_ERROR_VALUE );

	RemoveFromPlayerList(uiAccount);

	// 파티는 2명 이상일경우에만 유효하다.
	// 2명이 되지 않는다면 파티를 삭제한다.
	if( 2 > m_nCurJoiner )
	{
		m_bUsing = FALSE;
		
		IPartyMgr::GetInstacne()->Free( m_uiId );

		Init( m_uiId );

		return INITIAL_ERROR_VALUE;
	}

	if( m_leader_t.uiAccount == uiAccount )
	{
		for( i = 0; i < nMaxPerson; i++ )
		{
			if ( m_members_t[ i ].bUsing )
			{
				memcpy( &m_leader_t, &m_members_t[ i ], sizeof(_party_member_t) );
				return m_leader_t.uiAccount;
			}
		}
	}
	
	return	 INITIAL_ERROR_VALUE;
}


VOID	CParty::RemoveFromPlayerList(UI16 uiAccount)
{
	SI32	i;

	assert( m_nCurJoiner > 0 );

	for( i = 0; i < nMaxPerson; i++ )
	{
		if( m_members_t[ i ].uiAccount == uiAccount )
		{
			TRACE_LOG("RemoveFromPlayerList() 탈퇴한 파티원 , %x \n", uiAccount);

			m_members_t[ i ].clear();
			m_nCurJoiner--;
			break;
		}
	}

#ifdef _VERIFY_PARTY_PACKET

	for(int nIndex=0; nIndex < m_nCurJoiner; nIndex++ )
	{
		TRACE_LOG( "RemoveFromPlayerList 파티원 리스트 : Index %d, uiAccount : %x \n", nIndex, m_members_t[nIndex].uiAccount );
	}

#endif

}

VOID	CParty::GeneratePartyList( UI16* puiMembersAccount, OnPartyInfo* pTargetList, SI32& rCnt )
{
	SI32	siCounter;
	SI32	i;

	assert( pTargetList );	

	for( siCounter = i = 0; i < nMaxPerson; i++ )
	{
		if ( m_members_t[ i ].bUsing == TRUE )
		{
			puiMembersAccount[ siCounter ]		=	m_members_t[ i ].uiAccount;

			// 파티장인가?
			if( m_leader_t.uiAccount == m_members_t[ i ].uiAccount )
			{
				pTargetList[ siCounter ].bIsPartyLeader		=	TRUE;
			}
			else
			{
				pTargetList[ siCounter ].bIsPartyLeader		=	FALSE;
			}			

			// 레벨
			pTargetList[ siCounter ].siLevel	=	m_members_t[ i ].siLevel;
		
			// 이름
			ZeroMemory( pTargetList[ siCounter ].Name, sizeof( pTargetList[ siCounter ].Name ) );
			strncpy( pTargetList[ siCounter ].Name, m_members_t[ i ].szId, ON_ID_LENGTH );
			
			siCounter++;
		}
	}
	
	rCnt = m_nCurJoiner;

#ifdef _VERIFY_PARTY_PACKET

	for(nIndex=0; nIndex < m_nCurJoiner; nIndex++ )
	{
		TRACE_LOG( "GeneratePartyList 파티원 리스트 : Index %d, uiAccount : %x \n", nIndex, puiMembersAccount[nIndex] );
	}

#endif
}

SI32	CParty::GeneratePartyMemberAccount( UI16 *puiMembersAccount )
{
	SI32	siCounter;
	SI32	i;

	for( siCounter = i = 0; i < nMaxPerson; i++ )
	{
		if( m_members_t[ i ].bUsing == FALSE )		continue;			
		
		puiMembersAccount[ siCounter ]		=	m_members_t[ i ].uiAccount;					
		siCounter++;		
	}
	
	return	m_nCurJoiner;
}



IPartyMgr*	IPartyMgr::m_pselfInstance = NULL;
CParty*		IPartyMgr::m_Parties = NULL;

IPartyMgr	g_IPartyMgr_Instance;

IPartyMgr::IPartyMgr()
{
	m_pselfInstance		=	this;

	m_Parties			=	new CParty[ MAX_PARTIES ];

	assert( m_Parties );
	
	Init();
}

IPartyMgr::~IPartyMgr()
{
	if ( m_Parties )
	{
		delete [] m_Parties;
		m_Parties	=	NULL;
	}
}

VOID	IPartyMgr::Init()
{	
	SI32	i;

	for( i = 0; i < MAX_PARTIES; i++ )
	{
		m_Parties[ i ].Init( i );
		
	}
}


UI16	IPartyMgr::Alloc()						
{ 
	UI16	uiRes = INITIAL_ERROR_VALUE;
	SI32	i;
	
	for( i = 0; i < MAX_PARTIES; i++ )
	{
		if ( m_Parties[ i ].IsUsing() == FALSE )
		{
			m_Parties[ i ].SetUseFlag( TRUE );
			
			uiRes = i;
			break;
		}
	}

	assert( uiRes != INITIAL_ERROR_VALUE );

	return uiRes;
}

VOID	IPartyMgr::Free( UI16 uiFree )	
{ 
	assert( uiFree >= 0 && uiFree < MAX_PARTIES );

	m_Parties[ uiFree ].Init( uiFree );
}
	
CParty*		IPartyMgr::Get( UI16 uiId )
{
	if ( uiId >= 0 && uiId < MAX_PARTIES )
		return &m_Parties[ uiId ];

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//

CParty*		IPartyMgr::Find(char* pszName)
{
	return NULL;
}