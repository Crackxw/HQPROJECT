//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayerMgr.h"
#include "XPlayer.h"
#include <stdexcept>
#include <assert.h>

XPlayerMgr::XPlayerMgr()
{
	m_pXPlayer = NULL;
}

XPlayerMgr::~XPlayerMgr()
{
	if( m_pXPlayer ) delete [] m_pXPlayer;
}

void XPlayerMgr::Create( SI32 siMaxPlayer )
{

	if( m_pXPlayer ) delete [] m_pXPlayer;

	try{
		m_pXPlayer = new XPlayer[ siMaxPlayer ];
	}
	catch(std::bad_alloc){
		assert(0);
	}

	for( SI32 i = 0; i < siMaxPlayer; ++i ) {

		NewObject( (JWObject *)&m_pXPlayer[ i ] );
	}

}
