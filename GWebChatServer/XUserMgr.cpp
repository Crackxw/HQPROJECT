#include "XUserMgr.h"
#include "XUser.h"


XUserMgr::XUserMgr()
{
	m_pUser = NULL;
	m_pUserPtr = NULL;
	m_pUserPtrList = NULL;
}

XUserMgr::~XUserMgr()
{

	if( m_pUser ) delete [] m_pUser;
	
	if( m_pUserPtr ) delete [] m_pUserPtr;

	if( m_pUserPtrList ) delete m_pUserPtrList;
}

void XUserMgr::Create( SI32 siMaxUser )
{
	m_pUser = new XUser[ siMaxUser ];

	m_pUserPtr = new JWObjectPtr[ siMaxUser ];

	m_pUserPtrList = new JWList;

	for( int i = 0; i < siMaxUser; ++i ) {

		NewObject( (JWObject *)&m_pUser[ i ] );
		
		m_pUserPtrList->NewObject( &m_pUserPtr[ i ] );
	}
	
}

XUser* XUserMgr::GetUserByID( char *pID )
{

	UI32 i;
	XUser *pUser;
	JWObjectPtr *p;

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		pUser = (XUser *)( p->GetObject() );

		if( strcmp( pUser->m_userInfo.szID, pID ) == 0 ) return pUser;
	}	

	return NULL;
}

XUser* XUserMgr::GetUserByName( char *pName )
{

	UI32 i;
	XUser *pUser;
	JWObjectPtr *p;

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		pUser = (XUser *)( p->GetObject() );

		if( strcmp( pUser->m_userInfo.szName, pName ) == 0 ) return pUser;
	}	

	return NULL;
}


