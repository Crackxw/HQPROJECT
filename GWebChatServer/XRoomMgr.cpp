#include "XRoomMgr.h"

#include "XRoom.h"

#include "XChatServer.h"

XRoomMgr::XRoomMgr()
{
	m_pRoom = NULL;

	m_pServer = NULL;

	m_siNumMaxRoom = 0;

	m_siNumOpenedRoom = 0;
}

XRoomMgr::~XRoomMgr()
{
	if( m_pRoom ) delete [] m_pRoom;
}

void XRoomMgr::Create( XChatServer *pServer, SI32 siNumMaxRoom )
{

	m_pServer = pServer;

	m_siNumMaxRoom = siNumMaxRoom;

	m_siNumOpenedRoom = 0;

	m_emptyRoomList.CreateQueue( siNumMaxRoom );

	m_pRoom = new XRoom[ siNumMaxRoom + 1 ];

	for( int i = 0; i <= siNumMaxRoom; ++i ) {

		if( i > 0 ) m_emptyRoomList.Enqueue( i );

		m_pRoom[ i ].CreateRoom( this, pServer->m_pUserMgr->GetUserPtrList(), i ); 

	}

}

UI32 XRoomMgr::MakeRoom( sRequest_MakeRoom *pMakeRoom, XUser *pUser, UI16 *pOut_usRoomNum )
{
	UI32 result;
	DWORD dwItem;

	if( m_emptyRoomList.GetNumberOfItems() == 0 ) {
		return RESULT_NOMORE_EMPTYROOM;
	}

	m_emptyRoomList.Dequeue( &dwItem );

	result = m_pRoom[ dwItem ].MakeRoom( pMakeRoom, pUser );

	if( result != RESULT_SUCCESS ) {

		m_emptyRoomList.Enqueue( dwItem );

		return result;
	}

	*pOut_usRoomNum = (UI16)dwItem;

	++m_siNumOpenedRoom;

	return RESULT_SUCCESS;
}

UI32 XRoomMgr::JoinRoom( sRequest_JoinRoom *pJoinRoom, XUser *pUser )
{
	if( pJoinRoom->usRoomNum < 1 || pJoinRoom->usRoomNum > m_siNumMaxRoom ) {
		return RESULT_INVALID_ROOMNUM;	
	}

	return m_pRoom[ pJoinRoom->usRoomNum ].JoinRoom( pJoinRoom, pUser );
}

UI32 XRoomMgr::CloseRoom( UI16 usRoomNum )
{

	if( usRoomNum < 1 || usRoomNum > m_siNumMaxRoom ) {
		return RESULT_UNKNOWN_FAILED;
	}

	return CloseRoom( &m_pRoom[ usRoomNum ] );

}

UI32 XRoomMgr::CloseRoom( XRoom *pRoom )
{
	if( pRoom->IsOpened() == FALSE ) return RESULT_UNKNOWN_FAILED;

	pRoom->KickAll();

	pRoom->m_bOpened = FALSE;

	DWORD dwItem = (DWORD)pRoom->m_roomInfo.usRoomNum;

	m_emptyRoomList.Enqueue( dwItem );

	--m_siNumOpenedRoom;

	return RESULT_SUCCESS;

}


void XRoomMgr::SendRoomList( XUser *pUser )
{
	UI16	usCount = (UI16)m_siNumOpenedRoom;

	sResponse_BeginRoomList sendBeginMsg;
	sResponse_EndRoomList sendEndMsg;

	sResponse_RoomViewInfo sendMsg;

	printf( "Room Count : %d\n", usCount );


	// begin message
	sendBeginMsg.packetHeader.command = RESPONSE_BEGIN_ROOMLIST;
	sendBeginMsg.packetHeader.size = sizeof( sendBeginMsg );

	sendBeginMsg.usCount = usCount;

	pUser->SendPacket( (char *)&sendBeginMsg );

	// roomviewinfo

	sendMsg.packetHeader.command = RESPONSE_ROOMVIEWINFO;
	sendMsg.packetHeader.size = sizeof( sendMsg );

	for( UI16 i = 1; i <= m_siNumMaxRoom; ++i ) {

		if( m_pRoom[ i ].IsOpened() ) {
			m_pRoom[ i ].GetRoomViewInfo( &sendMsg.roomViewInfo );

			pUser->SendPacket( (char *)&sendMsg );
		}
	}

	// end message
	sendEndMsg.packetHeader.command = RESPONSE_END_ROOMLIST;
	sendEndMsg.packetHeader.size = sizeof( sendEndMsg );

	pUser->SendPacket( (char *)&sendEndMsg );

}
