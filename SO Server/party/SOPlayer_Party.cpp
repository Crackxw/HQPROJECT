
#include "SOMain.h"
// #include <assert.h>
#include "SOWorld.h"
#include "SOPlayer.h"
#include "../party/IPartyMgr.h"

// 2003.10.7 추가됨, 정성운 
BOOL	SOPlayer::CanJoinParty()
{	
	// 반드시 필드에 있어야 한다.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	
	// 현재 전투 대기중이거나 전투중에 있다면 FALSE를 리턴한다.
	if( GetBattle() != NULL )											return	FALSE;

	// 현재 다른 플레이어와 교역 중이면 FALSE를 리턴한다.
	if( IsTrading() == TRUE )											return	FALSE;

	// 개인 상점 모드이면 FALSE를 리턴한다.
	if( clPrivateMarket.IsOpen() == TRUE )								return	FALSE;

	if( PARTY_STATE_BE_FREE != m_PartyState.nState )					return	FALSE;

	return TRUE;
}

BOOL	SOPlayer::CreateParty()
{
	UI16		uiPartyID;
	CParty		*pNewParty;

	if( ( uiPartyID	= IPartyMgr::GetInstacne()->Alloc() ) == INITIAL_ERROR_VALUE )			return	FALSE;

	if( ( pNewParty = IPartyMgr::GetInstacne()->Get( uiPartyID ) ) == NULL )				return	FALSE;

	pNewParty->Create( uiAccount, szID , IFollower.GetFollowerParameterLevel( 0 ) );

	// 파티에 참여한 상태로 둔다.
	m_PartyState.JoinParty( uiPartyID, TRUE );	

	return TRUE;
}

BOOL	SOPlayer::LeaveParty()
{
	CParty			*pParty;	
	SOPlayer		*pNewLeader;	
	UI16			uiTempPartyID;	
	UI16			uiMembersAccount[ nMaxPerson ];
	UI16			uiNewLeaderAccount;
	OnPartyInfo		PartyInfo[ nMaxPerson ];
	SI32			siPartyMembersNum;
	SI32			i;
	SOPlayer		*pPlayer;	

	// 우선 파티 멤버인지 검사한다.
	if( IsPartyMember() == FALSE )																return	FALSE;	

	uiTempPartyID	=	m_PartyState.uiPartyId;
	if( ( pParty = IPartyMgr::GetInstacne()->Get( uiTempPartyID ) ) == NULL )					return	FALSE;

	// 우선 현재 파티에 가입되어 있는 사람들의 리스트를 다시 얻어온다.
	pParty->GeneratePartyList( uiMembersAccount, PartyInfo, siPartyMembersNum );

	pParty->Leave( uiAccount );	
	
	m_PartyState.Init();

	if( pParty->IsUsing() == TRUE )
	{
		// 아직 파티가 유효하다.

		// 현재 파티에 가입되어 있는 사람들의 리스트를 다시 얻어온다.
		pParty->GeneratePartyList( uiMembersAccount, PartyInfo, siPartyMembersNum );
		
		// 새로운 파티장의 Account를 얻어온다.
		uiNewLeaderAccount		=	pParty->GetLeaderId();

		if( ( pNewLeader = pMyWorld->GetPlayer( uiNewLeaderAccount ) ) == NULL )				return	FALSE;
	
		// 새로운 파티장으로 설정한다.
		pNewLeader->SetPartyLeader();	

		// 나를 제외한 다른 사람에게 파티 리스를 새로이 보내준다.		
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_CUSTOM, LPVOID( ON_RET_OK ), 
						LPVOID( szID ),
						LPVOID( siPartyMembersNum ), 
						LPVOID( uiMembersAccount ), LPVOID( PartyInfo ) );
		
	}
	else
	{
		// 파티가 없어졌다.

		// 나를 제외한 다른 사람에게 파티가 없어졌다고 알려준다.
		for( i = 0; i < siPartyMembersNum; i++ )
		{
			if( uiMembersAccount[ i ] == uiAccount )									continue;			
			
			if( ( pPlayer = pMyWorld->GetPlayer( uiMembersAccount[ i ] ) ) == NULL )	continue;

			pPlayer->PartyDeleted();			
		}
	}

	return	TRUE;
}

UI16	SOPlayer::GetPartyId()
{
	return m_PartyState.uiPartyId;
}

BOOL	SOPlayer::IsPartyLeader()
{
	return	m_PartyState.bLeader;
}

// 내가 상대방에게 파티를 요청할 수 있는지 알아온다.
BOOL	SOPlayer::CanRequestJoinParty()
{
	CParty		*pCurParty;
	
	// 반드시 필드에 있어야 한다.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )								return	FALSE;

	// 현재 전투 대기중이거나 전투중에 있다면 FALSE를 리턴한다.
	if( GetBattle() != NULL )																		return	FALSE;

	// 현재 다른 플레이어와 교역 중이면 FALSE를 리턴한다.
	if( IsTrading() == TRUE )																		return	FALSE;

	// 개인 상점 모드이면 FALSE를 리턴한다.
	if( clPrivateMarket.IsOpen() == TRUE )															return	FALSE;

	// 기존의 만들어진 파티의 파티장과
	// 파티를 생성하려는 사람만이 파티를 생성할 수 있다.
	if( IsPartyLeader() == TRUE )
	{
		// 내가 가입한 파티에 다른 사람이 가입할 수 있는지 알아온다.
		if( ( pCurParty = IPartyMgr::GetInstacne()->Get( m_PartyState.uiPartyId ) ) == NULL )		return	FALSE;
		if( pCurParty->CanJoin() == FALSE )															return	FALSE;	

	}
	else
	{
		// 파티를 새로이 생성하려는 사람이라면 
		// 파티에 관련하여 어느것도 하지 않는 상태이어야 한다.
		if( m_PartyState.GetState() !=  PARTY_STATE_BE_FREE )										return	FALSE;	

		// 파티의 일원이어도 안된다.
		if( IsPartyMember() == TRUE )																return	FALSE;
	}

	return	TRUE;
}


// 현재 파티 요청을 받을 수 있는지 알아온다.
BOOL	SOPlayer::CanReceiveRequestJoinParty()
{
	// 반드시 필드에 있어야 한다.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
		
	// 현재 전투 대기중이거나 전투중에 있다면 FALSE를 리턴한다.
	if( GetBattle() != NULL )											return	FALSE;

	// 현재 다른 플레이어와 교역 중이면 FALSE를 리턴한다.
	if( IsTrading() == TRUE )											return	FALSE;

	// 개인 상점 모드이면 FALSE를 리턴한다.
	if( clPrivateMarket.IsOpen() == TRUE )								return	FALSE;

	// 파티에 관련하여 어느것도 하지 않는 상태여야 한다.
	if( m_PartyState.GetState() !=  PARTY_STATE_BE_FREE )				return	FALSE;

	// 파티의 일원이어도 안된다.
	if( IsPartyMember() == TRUE )										return	FALSE;

	return	TRUE;
}

// 해당 유저에게 파티에 가입 요청할 것을 요청했다.
VOID	SOPlayer::RequestJoinParty( UI16 local_uiAccount )
{
	m_PartyState.SetState( PARTY_STATE_IS_REQUESTED_JOIN, local_uiAccount );
}

// 해당 유저에게 파티에 가입 요청할 것을 받았다.
VOID	SOPlayer::ReceiveRequestJoinParty( UI16 local_uiAccount )
{
	m_PartyState.SetState( PARTY_STATE_IS_RECEIVED_REQUESTE_JOIN, local_uiAccount );	
}

// 파티에 들어간다.
BOOL	SOPlayer::JoinParty( UI16 uiPartyID )
{
	CParty		*pCurParty;

	if( ( pCurParty = IPartyMgr::GetInstacne()->Get( uiPartyID ) ) == NULL )							return	FALSE;

	if( pCurParty->Join( uiAccount, szID, IFollower.GetFollowerParameterLevel( 0 ) ) == FALSE )			return	FALSE;

	// 내 파티 상태를 초기화 한다.
	m_PartyState.JoinParty( uiPartyID );

	return	TRUE;
}

// 파티장이 되었다.
VOID	SOPlayer::SetPartyLeader()
{	
	// 내가 파티장이 되었다.
	m_PartyState.SetPartyLeader();
}

BOOL	SOPlayer::IsPartyMember()
{
	if( m_PartyState.uiPartyId == INITIAL_ERROR_VALUE )								return	FALSE;

	return	TRUE;
}

BOOL	SOPlayer::IsRequestingJoinParty( UI16 uiAccount )
{
	if( m_PartyState.GetState() != PARTY_STATE_IS_REQUESTED_JOIN )					return	FALSE;	

	if( uiAccount != 0 )
	{
		if( uiAccount != m_PartyState.uiTargetPlayerId )							return	FALSE;
	}

	return	TRUE;
}

BOOL	SOPlayer::IsReceivedRequestJoinParty()
{
	if( m_PartyState.GetState() != PARTY_STATE_IS_RECEIVED_REQUESTE_JOIN )			return	FALSE;
	
	return	TRUE;
}

VOID	SOPlayer::PartyDeleted()
{
	// 만약 내가 파티장이고 다른 사람에게 파티에 참여하라고 요청하는 중에
	// 파티가 삭제되었다면 파티 참여 요청을 취소한다.
	if( ( IsPartyLeader() == TRUE ) && ( IsRequestingJoinParty() == TRUE ) )
	{	
		CancelRequestingParty();
	}

	m_PartyState.Init();

	SendFieldMsg( ON_PARTY_DELETED, SO_SFM_ONLYME );
}

BOOL	SOPlayer::AcceptRequestingJoinParty()
{
	SOPlayer			*pPlayer;
	CParty				*pParty;
	OnPartyInfo			pPartyList[ nMaxPerson ];
	UI16				uiMembersAccount[ nMaxPerson ];
	SI32				siPartyMembersNum;

	// 나에게 들어온 파티 신청이 있는지 검사한다.
	if( IsReceivedRequestJoinParty() == FALSE )															return	FALSE;

	// 나에게 파티 신청한 사람을 얻어온다.
	if( ( pPlayer = pMyWorld->GetPlayer( m_PartyState.uiTargetPlayerId ) ) == NULL )					return	FALSE;

	// 나에게 파티를 신청한 사람 역시 나에게 파티를 신청했는지 검사한다.
	if( pPlayer->IsRequestingJoinParty( uiAccount ) == FALSE )											return	FALSE;

	// 상대방이 파티를 가지고 있지 않다면 파티를 새로이 생성한다.
	if( pPlayer->IsPartyLeader() == FALSE )
	{
		if( pPlayer->CreateParty() == FALSE )															return	FALSE;
	}

	// 상대방의 파티에 참여 한다.
	if( JoinParty( pPlayer->m_PartyState.uiPartyId ) == FALSE )											return	FALSE;

	// 나와 상대방의 파티 상태를 초기화한다.
	m_PartyState.SetState( PARTY_STATE_BE_FREE );
	pPlayer->m_PartyState.SetState( PARTY_STATE_BE_FREE );



	// 파티원들의 리스트를 얻어온다.
	if( ( pParty = IPartyMgr::GetInstacne()->Get( pPlayer->m_PartyState.uiPartyId ) ) == NULL )			return	FALSE;
	
	pParty->GeneratePartyList( uiMembersAccount, pPartyList, siPartyMembersNum );
	
	// 모든 파티원들에게 파티원 리스트를 보내준다.
	SendFieldMsg( ON_RESPONSE_ACCEPT_PARTY, SO_SFM_CUSTOM, LPVOID( ON_RET_OK ), 
							LPVOID( siPartyMembersNum ),
							LPVOID( uiMembersAccount ), 							
							LPVOID( pPartyList ), 							
							LPVOID( szID ) );
	

	return	TRUE;	
}


BOOL	SOPlayer::CancelRequestingParty( BOOL bProcessByRequest )
{
	SOPlayer	*pTargetPlayer;

	// 현재 다른 사람에게 파티에 참여하라고 요청중인지 검사한다.
	if( IsRequestingJoinParty() == FALSE )															return	FALSE;

	if( ( pTargetPlayer = pMyWorld->GetPlayer( m_PartyState.uiTargetPlayerId ) ) == NULL )			return	FALSE;

	if( pTargetPlayer->IsReceivedRequestJoinParty() == FALSE )										return	FALSE;	

	// 사용자의 요청에 의하여 취소 되는 것이라면 파티 신청을 거부한다는 메시지를 받으면 안된다.
	if( bProcessByRequest == FALSE )
	{
		// 파티 신청을 거부한다.
		SendFieldMsg( ON_RESPONSE_ACCEPT_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ), 
									LPVOID( NULL ),								
									LPVOID( NULL ),
									LPVOID( NULL ),
									LPVOID( szID ) );
	}

	// 상대방에게 파티 요청이 취소되었다고 알려준다.
	pTargetPlayer->SendFieldMsg( ON_CANCEL_REQUESTED_PARTY, SO_SFM_ONLYME );

	// 두 플레이어의 파티 상태를 초기화 한다.
	m_PartyState.SetState( PARTY_STATE_BE_FREE );
	pTargetPlayer->m_PartyState.SetState( PARTY_STATE_BE_FREE );

	return	TRUE;
	
}

BOOL	SOPlayer::RejectRequestingJoinParty()
{
	SOPlayer	*pPlayer;

	// 나에게 들어온 파티 신청이 있는지 검사한다.
	if( IsReceivedRequestJoinParty() == FALSE )											return	FALSE;

	// 나에게 파티 신청한 사람을 얻어온다.
	if( ( pPlayer = pMyWorld->GetPlayer( m_PartyState.uiTargetPlayerId ) ) == NULL )	return	FALSE;

	// 나에게 파티를 신청한 사람 역시 나에게 파티를 신청했는지 검사한다.
	if( pPlayer->IsRequestingJoinParty( uiAccount ) == FALSE )							return	FALSE;

	// 상대방에게 파티 신청을 거부한다는 메시지를 보낸다.
	pPlayer->SendFieldMsg( ON_RESPONSE_ACCEPT_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ), 
								LPVOID( NULL ),								
								LPVOID( NULL ),
								LPVOID( NULL ),
								LPVOID( szID ) );

	// 나에게는 상대방이 파티 신청을 취소했다는 메시지를 보낸다.
	SendFieldMsg( ON_CANCEL_REQUESTED_PARTY, SO_SFM_ONLYME );


	// 나와 상대방의 파티 상태를 초기화한다.
	m_PartyState.SetState( PARTY_STATE_BE_FREE );
	pPlayer->m_PartyState.SetState( PARTY_STATE_BE_FREE );

	return	TRUE;
}

VOID	SOPlayer::CloseRequestingJoinParty()
{
	// 내가 만약 파티 신청중이라면 신청 중인 파티를 취소한다.
	if( IsRequestingJoinParty() == TRUE )
	{
		CancelRequestingParty();		
	}


	// 내가 만약 파티 신청을 받았다면 이에 거부한다.
	if( IsReceivedRequestJoinParty() == TRUE )
	{
		RejectRequestingJoinParty();
	}
}

BOOL	SOPlayer::PartyChat( CHAR *pszChatMsg )
{
	CHAR		szPartyChatMsg[ 1024 ];
	CParty		*pMyParty;	
	SI32		siPartyMembersNum;
	UI16		uiMembersAccount[ nMaxPerson ];
	SI32		i;
	SOPlayer	*pPlayer;

	// 내가 파티에 가입되어 있는지 검사한다.
	if( IsPartyMember() == FALSE )																			return	FALSE;

	// 파티를 얻어온다.
	if( ( pMyParty = IPartyMgr::GetInstacne()->Get( m_PartyState.uiPartyId ) ) == NULL )					return	FALSE;		
	
	// 파티원들의 리스트를 얻어온다.
	ZeroMemory( uiMembersAccount, sizeof( uiMembersAccount ) );
	if( ( siPartyMembersNum = pMyParty->GeneratePartyMemberAccount( uiMembersAccount ) ) <= 1 )				return	FALSE;	

	// 채팅 메시지는 최대 127 바이트이다.	
	sprintf( szPartyChatMsg, "[ %s ] : %s", szID, pszChatMsg );
	szPartyChatMsg[ 128 ] =	NULL;
	
	for( i = 0; i < nMaxPerson; i++ )
	{
		if( uiMembersAccount[ i ] == uiAccount )										continue;

		if( ( pPlayer = pMyWorld->GetPlayer( uiMembersAccount[ i ] ) ) == NULL )		continue;		
		
		pPlayer->SendFieldMsg( ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID( ON_RET_TEXTMSG_PARTYCHAT ), 	LPVOID( szPartyChatMsg ) );	
	}	
	
	return	TRUE;
}

