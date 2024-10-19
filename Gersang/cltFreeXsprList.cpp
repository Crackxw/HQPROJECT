#include	<GSL.h>

#include "cltFreeXsprList.h"


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Construction
//------------------------------------------------------------------------------------------------------------
cltFreeXsprList::cltFreeXsprList( SI32 siSize )
{
	// list를 queue의 size만큼 생성
	Create( siSize );

	// 동기화 객체 초기화
	InitializeCriticalSection( &m_cs );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Destruction
//------------------------------------------------------------------------------------------------------------
cltFreeXsprList::~cltFreeXsprList()
{
	DeleteCriticalSection( &m_cs );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: 
//------------------------------------------------------------------------------------------------------------
BOOL	cltFreeXsprList::AddXspr( strXsprQueueData *pstData )
{
	BOOL	bRet;

	EnterCriticalSection( &m_cs );

	bRet	=	Add( pstData );

	LeaveCriticalSection( &m_cs );

	return	bRet;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: 
//------------------------------------------------------------------------------------------------------------
BOOL	cltFreeXsprList::DeleteXspr( strXsprQueueData *pstData )
{
	BOOL	bRet;

	EnterCriticalSection( &m_cs );

	bRet	=	Delete( pstData );

	LeaveCriticalSection( &m_cs );

	return	bRet;
}
