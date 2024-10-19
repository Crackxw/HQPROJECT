// IPartyMgr.h: interface for the IPartyMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPARTYMGR_H__7860CA5A_15D2_44F1_B5F4_00471A52633E__INCLUDED_)
#define AFX_IPARTYMGR_H__7860CA5A_15D2_44F1_B5F4_00471A52633E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../JWCommon/jwbase.h"
#include "../SCCommon\OnlineCommonStruct.h"
#include <assert.h>

enum	PLAYERS_PARTY_STATE
{
	PARTY_STATE_IS_REQUESTED_JOIN,
	PARTY_STATE_IS_RECEIVED_REQUESTE_JOIN,	
	PARTY_STATE_BE_FREE,

	PARTY_STATE_ERROR
};

const	int		nMaxPerson = 7;					// �ִ� ��Ƽ�� ���� �ο���
const	int		INITIAL_ERROR_VALUE = 65535;

struct  party_state_t
{
	PLAYERS_PARTY_STATE		nState;				// �÷��̾��� ��Ƽ ����
	UI16					uiPartyId;			// ������ ��Ƽ ID
	UI16					uiTargetPlayerId;	// �ӽú��� ( ��û ���� ���, ��û�� �� ����� uiAccount )
	BOOL					bLeader;			// ���� ��Ƽ�� ���� �ΰ�.

	PLAYERS_PARTY_STATE		GetState() { return nState; };

	VOID					SetState( PLAYERS_PARTY_STATE newState, UI16 uiParam = 0) 
	{		
		nState = newState;
		
		switch( nState )
		{	
		case PARTY_STATE_IS_REQUESTED_JOIN:
			uiTargetPlayerId	=	uiParam;
			break;
		
		case PARTY_STATE_IS_RECEIVED_REQUESTE_JOIN:
			uiTargetPlayerId	=	uiParam;
			break;		

		case PARTY_STATE_BE_FREE:
			uiTargetPlayerId	=	INITIAL_ERROR_VALUE;
			break;
		}

	}

	VOID	Init() 
	{
		SetState( PARTY_STATE_BE_FREE );

		uiPartyId	=	INITIAL_ERROR_VALUE;
		bLeader		=	FALSE;
	}

	VOID	JoinParty( UI16 local_uiPartyID, BOOL local_bLeader = FALSE )
	{
		uiPartyId	=	local_uiPartyID;
		bLeader		=	local_bLeader;
	
		SetState( PARTY_STATE_BE_FREE );
	}

	VOID	SetPartyLeader()
	{
		bLeader		=	TRUE;
	}
};


class CParty
{
protected:
	struct _party_member_t
	{
		UI16	uiAccount;
		SI32	siLevel;
		CHAR	szId[ON_ID_LENGTH +1];
		BOOL	bUsing;

		VOID	clear() 
		{ 
			uiAccount = INITIAL_ERROR_VALUE;
			siLevel = INITIAL_ERROR_VALUE;
			memset(szId, 0, ON_ID_LENGTH +1);
			bUsing = FALSE;
		}
	};

public:

	CParty();
	virtual ~CParty();

	VOID	Init(UI16 uiPartyId);


/**
	��Ƽ�� �����Ѵ�. 
	�������� uiAccount, uiCharacterKind
*/	
	VOID	Create( UI16 uiAccount, CHAR* pszId, SI32 siLevel );	
/**
	��Ƽ ���� �������� �����Ѵ�. (����, ��Ƽ�ο�)

 */
	BOOL	CanJoin() { if( m_nCurJoiner < nMaxPerson ) return TRUE; return FALSE; }

/* 
	��Ƽ ���� , Ż�� 
*/
	BOOL	Join( UI16 uiAccount, CHAR* pszId, SI32 siLevel );	
	UI16	Leave( UI16 uiAccount );
	UI16	GetLeaderId() { return m_leader_t.uiAccount; }
	
	BOOL	IsUsing() { return m_bUsing; }
	VOID	SetUseFlag(BOOL bFlag) { m_bUsing = bFlag; }

	VOID	GeneratePartyList( UI16* puiMembersAccount, OnPartyInfo* pTargetList, SI32& rCnt );
	SI32	GeneratePartyMemberAccount( UI16 *puiMembersAccount );

	UI16	GetCount() { return m_nCurJoiner; }

	UI16	m_nCurJoiner;	// ���� ��Ƽ �ο���
	UI16	m_uiId;
	BOOL	m_bUsing;

protected:
	VOID	RemoveFromPlayerList( UI16 uiAccount );

	_party_member_t	m_leader_t;
	_party_member_t	m_members_t[ nMaxPerson ];
};


/**

	Party Manager
 
   
*/

const int MAX_PARTIES = 2500;

class IPartyMgr
{
public:


	IPartyMgr();
	virtual ~IPartyMgr();

	static 
	IPartyMgr* GetInstacne() { return m_pselfInstance; }

	VOID				Init();

	virtual UI16		Alloc();
	virtual VOID		Free(UI16 uiPartyId );
	
	virtual CParty*		Get(UI16 nId);

	virtual	CParty*		Find(char* pszName);

protected:

	static	IPartyMgr*	m_pselfInstance;
	static 	CParty*		m_Parties;
};


#endif // !defined(AFX_IPARTYMGR_H__7860CA5A_15D2_44F1_B5F4_00471A52633E__INCLUDED_)
