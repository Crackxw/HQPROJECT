#ifndef	_PLAYER_MANAGER_HEADER
#define	_PLAYER_MANAGER_HEADER

#include "SOCommonHeader.h"
#include "Stack.h"

class	SOPlayer;

class	PlayersManager
{
protected:
	SStack< SOPlayer* >		m_Player;					// �÷��̾��.

public:
	PlayersManager();									// ������.
	~PlayersManager();									// �Ҹ���.

	SOPlayer*		GetFirstPlayer();							// ù��° �÷��̾ ���´�. �ַ� �޽����� �����ֱ� ���� ��ü�� ����ϱ� ���Ͽ� ����Ѵ�.

	virtual	VOID	Init( DWORD dwMaxPlayer );					// �ʱ�ȭ. ����ϴ� �ִ� �÷��̾��� ��.
	virtual	VOID	Free();										// �޸𸮸� �����Ѵ�.

	virtual	BOOL	AddPlayer( SOPlayer *pPlayer );				// �÷��̾ �߰��Ѵ�.
	virtual	BOOL	RemovePlayer( SOPlayer *pPlayer );			// �÷��̾ �����Ѵ�.
	virtual	VOID	RemoveAllPlayer();							// ��� �÷��̾ �����Ѵ�.

	virtual	BOOL	IsAddedPlayer( SOPlayer *pPlayer );				// �÷��̾ �߰��Ǿ����� �˾ƿ´�.
	virtual	SI32	GetAddedPlayerList( UI16 *puiPlayerList );		// �߰��Ǿ� �ִ� �÷��̾� ����Ʈ�� ���´�.
	virtual	SI32	GetAddedPlayerList( SOPlayer **pPlayerList );	// �߰��Ǿ� �ִ� �÷��̾� ����Ʈ�� ���´�.
};

#endif
