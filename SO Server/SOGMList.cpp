#include "SOGMList.h"
#include <stdlib.h>

cltGMManager::cltGMManager()
{
	m_dwCurrentGMUserNum	=	0;
}

DWORD	cltGMManager::ConveterStringIPToNumericIP( CHAR *pszIP )
{
	CHAR	cSeps[]   = ".";
	CHAR	*pToken;
	DWORD	dwIP;
	CHAR	szIP[ 128 ];

	ZeroMemory( szIP, sizeof( szIP ) );
	strncpy( szIP, pszIP, 127 );

	if( ( pToken = strtok( szIP, cSeps ) ) == NULL )				return	0;	
	( ( CHAR* )( &dwIP ) )[ 0 ]		=	atoi( pToken );

	if( ( pToken = strtok( NULL, cSeps ) ) == NULL )				return	0;	
	( ( CHAR* )( &dwIP ) )[ 1 ]		=	atoi( pToken );

	if( ( pToken = strtok( NULL, cSeps ) ) == NULL )				return	0;	
	( ( CHAR* )( &dwIP ) )[ 2 ]		=	atoi( pToken );

	if( ( pToken = strtok( NULL, cSeps ) ) == NULL )				return	0;	
	( ( CHAR* )( &dwIP ) )[ 3 ]		=	atoi( pToken );

	return	dwIP;
}

BOOL	cltGMManager::AddGM( CHAR *pszIP )
{
	DWORD	dwIP;	

	if( m_dwCurrentGMUserNum >= MAX_GM_USER_NUM )					return	FALSE;
	
	dwIP	=	ConveterStringIPToNumericIP( pszIP );

	if( dwIP == 0 )													return	FALSE;
	if( IsGM( pszIP ) == TRUE )										return	FALSE;					// 이미 추가되었는지 검사한다.
	
	m_dwIP[ m_dwCurrentGMUserNum ]	=	dwIP;
	m_dwCurrentGMUserNum++;

	return	TRUE;
	
}

BOOL	cltGMManager::IsGM( CHAR *pszIP )
{
	DWORD	i;
	DWORD	dwIP;

	dwIP	=	ConveterStringIPToNumericIP( pszIP );

	if( dwIP == 0 )													return	FALSE;

	for( i = 0; i < m_dwCurrentGMUserNum; i++ )
	{
		if( m_dwIP[ i ] == dwIP )									return	TRUE;		
	}

	return	FALSE;
}


