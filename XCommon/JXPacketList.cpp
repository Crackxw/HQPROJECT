#include "JXPacketList.h"
#include "JXPacket.h"


JXPacketList::JXPacketList()
{
	m_pPacket = NULL;
}

JXPacketList::~JXPacketList()
{
	if( m_pPacket ) delete [] m_pPacket;
}

void JXPacketList::Create( SI32 siMaxPacketNum )
{
	if( m_pPacket ) delete [] m_pPacket;
	
	m_pPacket = new JXPacket[ siMaxPacketNum ];

	for( SI32 i = 0; i < siMaxPacketNum; ++i ) {

		AddToFree( (JXObject *)&m_pPacket[ i ] );
	}

}
