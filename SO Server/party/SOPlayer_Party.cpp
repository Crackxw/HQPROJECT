
#include "SOMain.h"
// #include <assert.h>
#include "SOWorld.h"
#include "SOPlayer.h"
#include "../party/IPartyMgr.h"

// 2003.10.7 �߰���, ������ 
BOOL	SOPlayer::CanJoinParty()
{	
	// �ݵ�� �ʵ忡 �־�� �Ѵ�.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
	
	// ���� ���� ������̰ų� �����߿� �ִٸ� FALSE�� �����Ѵ�.
	if( GetBattle() != NULL )											return	FALSE;

	// ���� �ٸ� �÷��̾�� ���� ���̸� FALSE�� �����Ѵ�.
	if( IsTrading() == TRUE )											return	FALSE;

	// ���� ���� ����̸� FALSE�� �����Ѵ�.
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

	// ��Ƽ�� ������ ���·� �д�.
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

	// �켱 ��Ƽ ������� �˻��Ѵ�.
	if( IsPartyMember() == FALSE )																return	FALSE;	

	uiTempPartyID	=	m_PartyState.uiPartyId;
	if( ( pParty = IPartyMgr::GetInstacne()->Get( uiTempPartyID ) ) == NULL )					return	FALSE;

	// �켱 ���� ��Ƽ�� ���ԵǾ� �ִ� ������� ����Ʈ�� �ٽ� ���´�.
	pParty->GeneratePartyList( uiMembersAccount, PartyInfo, siPartyMembersNum );

	pParty->Leave( uiAccount );	
	
	m_PartyState.Init();

	if( pParty->IsUsing() == TRUE )
	{
		// ���� ��Ƽ�� ��ȿ�ϴ�.

		// ���� ��Ƽ�� ���ԵǾ� �ִ� ������� ����Ʈ�� �ٽ� ���´�.
		pParty->GeneratePartyList( uiMembersAccount, PartyInfo, siPartyMembersNum );
		
		// ���ο� ��Ƽ���� Account�� ���´�.
		uiNewLeaderAccount		=	pParty->GetLeaderId();

		if( ( pNewLeader = pMyWorld->GetPlayer( uiNewLeaderAccount ) ) == NULL )				return	FALSE;
	
		// ���ο� ��Ƽ������ �����Ѵ�.
		pNewLeader->SetPartyLeader();	

		// ���� ������ �ٸ� ������� ��Ƽ ������ ������ �����ش�.		
		SendFieldMsg( ON_RESPONSE_LEAVE_PARTY, SO_SFM_CUSTOM, LPVOID( ON_RET_OK ), 
						LPVOID( szID ),
						LPVOID( siPartyMembersNum ), 
						LPVOID( uiMembersAccount ), LPVOID( PartyInfo ) );
		
	}
	else
	{
		// ��Ƽ�� ��������.

		// ���� ������ �ٸ� ������� ��Ƽ�� �������ٰ� �˷��ش�.
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

// ���� ���濡�� ��Ƽ�� ��û�� �� �ִ��� �˾ƿ´�.
BOOL	SOPlayer::CanRequestJoinParty()
{
	CParty		*pCurParty;
	
	// �ݵ�� �ʵ忡 �־�� �Ѵ�.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )								return	FALSE;

	// ���� ���� ������̰ų� �����߿� �ִٸ� FALSE�� �����Ѵ�.
	if( GetBattle() != NULL )																		return	FALSE;

	// ���� �ٸ� �÷��̾�� ���� ���̸� FALSE�� �����Ѵ�.
	if( IsTrading() == TRUE )																		return	FALSE;

	// ���� ���� ����̸� FALSE�� �����Ѵ�.
	if( clPrivateMarket.IsOpen() == TRUE )															return	FALSE;

	// ������ ������� ��Ƽ�� ��Ƽ���
	// ��Ƽ�� �����Ϸ��� ������� ��Ƽ�� ������ �� �ִ�.
	if( IsPartyLeader() == TRUE )
	{
		// ���� ������ ��Ƽ�� �ٸ� ����� ������ �� �ִ��� �˾ƿ´�.
		if( ( pCurParty = IPartyMgr::GetInstacne()->Get( m_PartyState.uiPartyId ) ) == NULL )		return	FALSE;
		if( pCurParty->CanJoin() == FALSE )															return	FALSE;	

	}
	else
	{
		// ��Ƽ�� ������ �����Ϸ��� ����̶�� 
		// ��Ƽ�� �����Ͽ� ����͵� ���� �ʴ� �����̾�� �Ѵ�.
		if( m_PartyState.GetState() !=  PARTY_STATE_BE_FREE )										return	FALSE;	

		// ��Ƽ�� �Ͽ��̾ �ȵȴ�.
		if( IsPartyMember() == TRUE )																return	FALSE;
	}

	return	TRUE;
}


// ���� ��Ƽ ��û�� ���� �� �ִ��� �˾ƿ´�.
BOOL	SOPlayer::CanReceiveRequestJoinParty()
{
	// �ݵ�� �ʵ忡 �־�� �Ѵ�.
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return	FALSE;
		
	// ���� ���� ������̰ų� �����߿� �ִٸ� FALSE�� �����Ѵ�.
	if( GetBattle() != NULL )											return	FALSE;

	// ���� �ٸ� �÷��̾�� ���� ���̸� FALSE�� �����Ѵ�.
	if( IsTrading() == TRUE )											return	FALSE;

	// ���� ���� ����̸� FALSE�� �����Ѵ�.
	if( clPrivateMarket.IsOpen() == TRUE )								return	FALSE;

	// ��Ƽ�� �����Ͽ� ����͵� ���� �ʴ� ���¿��� �Ѵ�.
	if( m_PartyState.GetState() !=  PARTY_STATE_BE_FREE )				return	FALSE;

	// ��Ƽ�� �Ͽ��̾ �ȵȴ�.
	if( IsPartyMember() == TRUE )										return	FALSE;

	return	TRUE;
}

// �ش� �������� ��Ƽ�� ���� ��û�� ���� ��û�ߴ�.
VOID	SOPlayer::RequestJoinParty( UI16 local_uiAccount )
{
	m_PartyState.SetState( PARTY_STATE_IS_REQUESTED_JOIN, local_uiAccount );
}

// �ش� �������� ��Ƽ�� ���� ��û�� ���� �޾Ҵ�.
VOID	SOPlayer::ReceiveRequestJoinParty( UI16 local_uiAccount )
{
	m_PartyState.SetState( PARTY_STATE_IS_RECEIVED_REQUESTE_JOIN, local_uiAccount );	
}

// ��Ƽ�� ����.
BOOL	SOPlayer::JoinParty( UI16 uiPartyID )
{
	CParty		*pCurParty;

	if( ( pCurParty = IPartyMgr::GetInstacne()->Get( uiPartyID ) ) == NULL )							return	FALSE;

	if( pCurParty->Join( uiAccount, szID, IFollower.GetFollowerParameterLevel( 0 ) ) == FALSE )			return	FALSE;

	// �� ��Ƽ ���¸� �ʱ�ȭ �Ѵ�.
	m_PartyState.JoinParty( uiPartyID );

	return	TRUE;
}

// ��Ƽ���� �Ǿ���.
VOID	SOPlayer::SetPartyLeader()
{	
	// ���� ��Ƽ���� �Ǿ���.
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
	// ���� ���� ��Ƽ���̰� �ٸ� ������� ��Ƽ�� �����϶�� ��û�ϴ� �߿�
	// ��Ƽ�� �����Ǿ��ٸ� ��Ƽ ���� ��û�� ����Ѵ�.
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

	// ������ ���� ��Ƽ ��û�� �ִ��� �˻��Ѵ�.
	if( IsReceivedRequestJoinParty() == FALSE )															return	FALSE;

	// ������ ��Ƽ ��û�� ����� ���´�.
	if( ( pPlayer = pMyWorld->GetPlayer( m_PartyState.uiTargetPlayerId ) ) == NULL )					return	FALSE;

	// ������ ��Ƽ�� ��û�� ��� ���� ������ ��Ƽ�� ��û�ߴ��� �˻��Ѵ�.
	if( pPlayer->IsRequestingJoinParty( uiAccount ) == FALSE )											return	FALSE;

	// ������ ��Ƽ�� ������ ���� �ʴٸ� ��Ƽ�� ������ �����Ѵ�.
	if( pPlayer->IsPartyLeader() == FALSE )
	{
		if( pPlayer->CreateParty() == FALSE )															return	FALSE;
	}

	// ������ ��Ƽ�� ���� �Ѵ�.
	if( JoinParty( pPlayer->m_PartyState.uiPartyId ) == FALSE )											return	FALSE;

	// ���� ������ ��Ƽ ���¸� �ʱ�ȭ�Ѵ�.
	m_PartyState.SetState( PARTY_STATE_BE_FREE );
	pPlayer->m_PartyState.SetState( PARTY_STATE_BE_FREE );



	// ��Ƽ������ ����Ʈ�� ���´�.
	if( ( pParty = IPartyMgr::GetInstacne()->Get( pPlayer->m_PartyState.uiPartyId ) ) == NULL )			return	FALSE;
	
	pParty->GeneratePartyList( uiMembersAccount, pPartyList, siPartyMembersNum );
	
	// ��� ��Ƽ���鿡�� ��Ƽ�� ����Ʈ�� �����ش�.
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

	// ���� �ٸ� ������� ��Ƽ�� �����϶�� ��û������ �˻��Ѵ�.
	if( IsRequestingJoinParty() == FALSE )															return	FALSE;

	if( ( pTargetPlayer = pMyWorld->GetPlayer( m_PartyState.uiTargetPlayerId ) ) == NULL )			return	FALSE;

	if( pTargetPlayer->IsReceivedRequestJoinParty() == FALSE )										return	FALSE;	

	// ������� ��û�� ���Ͽ� ��� �Ǵ� ���̶�� ��Ƽ ��û�� �ź��Ѵٴ� �޽����� ������ �ȵȴ�.
	if( bProcessByRequest == FALSE )
	{
		// ��Ƽ ��û�� �ź��Ѵ�.
		SendFieldMsg( ON_RESPONSE_ACCEPT_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ), 
									LPVOID( NULL ),								
									LPVOID( NULL ),
									LPVOID( NULL ),
									LPVOID( szID ) );
	}

	// ���濡�� ��Ƽ ��û�� ��ҵǾ��ٰ� �˷��ش�.
	pTargetPlayer->SendFieldMsg( ON_CANCEL_REQUESTED_PARTY, SO_SFM_ONLYME );

	// �� �÷��̾��� ��Ƽ ���¸� �ʱ�ȭ �Ѵ�.
	m_PartyState.SetState( PARTY_STATE_BE_FREE );
	pTargetPlayer->m_PartyState.SetState( PARTY_STATE_BE_FREE );

	return	TRUE;
	
}

BOOL	SOPlayer::RejectRequestingJoinParty()
{
	SOPlayer	*pPlayer;

	// ������ ���� ��Ƽ ��û�� �ִ��� �˻��Ѵ�.
	if( IsReceivedRequestJoinParty() == FALSE )											return	FALSE;

	// ������ ��Ƽ ��û�� ����� ���´�.
	if( ( pPlayer = pMyWorld->GetPlayer( m_PartyState.uiTargetPlayerId ) ) == NULL )	return	FALSE;

	// ������ ��Ƽ�� ��û�� ��� ���� ������ ��Ƽ�� ��û�ߴ��� �˻��Ѵ�.
	if( pPlayer->IsRequestingJoinParty( uiAccount ) == FALSE )							return	FALSE;

	// ���濡�� ��Ƽ ��û�� �ź��Ѵٴ� �޽����� ������.
	pPlayer->SendFieldMsg( ON_RESPONSE_ACCEPT_PARTY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ), 
								LPVOID( NULL ),								
								LPVOID( NULL ),
								LPVOID( NULL ),
								LPVOID( szID ) );

	// �����Դ� ������ ��Ƽ ��û�� ����ߴٴ� �޽����� ������.
	SendFieldMsg( ON_CANCEL_REQUESTED_PARTY, SO_SFM_ONLYME );


	// ���� ������ ��Ƽ ���¸� �ʱ�ȭ�Ѵ�.
	m_PartyState.SetState( PARTY_STATE_BE_FREE );
	pPlayer->m_PartyState.SetState( PARTY_STATE_BE_FREE );

	return	TRUE;
}

VOID	SOPlayer::CloseRequestingJoinParty()
{
	// ���� ���� ��Ƽ ��û���̶�� ��û ���� ��Ƽ�� ����Ѵ�.
	if( IsRequestingJoinParty() == TRUE )
	{
		CancelRequestingParty();		
	}


	// ���� ���� ��Ƽ ��û�� �޾Ҵٸ� �̿� �ź��Ѵ�.
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

	// ���� ��Ƽ�� ���ԵǾ� �ִ��� �˻��Ѵ�.
	if( IsPartyMember() == FALSE )																			return	FALSE;

	// ��Ƽ�� ���´�.
	if( ( pMyParty = IPartyMgr::GetInstacne()->Get( m_PartyState.uiPartyId ) ) == NULL )					return	FALSE;		
	
	// ��Ƽ������ ����Ʈ�� ���´�.
	ZeroMemory( uiMembersAccount, sizeof( uiMembersAccount ) );
	if( ( siPartyMembersNum = pMyParty->GeneratePartyMemberAccount( uiMembersAccount ) ) <= 1 )				return	FALSE;	

	// ä�� �޽����� �ִ� 127 ����Ʈ�̴�.	
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

