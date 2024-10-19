#ifndef	_PLAYER_MANAGER_HEADER
#define	_PLAYER_MANAGER_HEADER

#include "SOCommonHeader.h"
#include "Stack.h"

class	SOPlayer;

class	PlayersManager
{
protected:
	SStack< SOPlayer* >		m_Player;					// 플레이어들.

public:
	PlayersManager();									// 생성자.
	~PlayersManager();									// 소멸자.

	SOPlayer*		GetFirstPlayer();							// 첫번째 플레이어를 얻어온다. 주로 메시지를 보내주기 위한 객체로 사용하기 위하여 사용한다.

	virtual	VOID	Init( DWORD dwMaxPlayer );					// 초기화. 허용하는 최대 플레이어의 수.
	virtual	VOID	Free();										// 메모리를 해제한다.

	virtual	BOOL	AddPlayer( SOPlayer *pPlayer );				// 플레이어를 추가한다.
	virtual	BOOL	RemovePlayer( SOPlayer *pPlayer );			// 플레이어를 삭제한다.
	virtual	VOID	RemoveAllPlayer();							// 모든 플레이어를 삭제한다.

	virtual	BOOL	IsAddedPlayer( SOPlayer *pPlayer );				// 플레이어가 추가되었는지 알아온다.
	virtual	SI32	GetAddedPlayerList( UI16 *puiPlayerList );		// 추가되어 있는 플레이어 리스트를 얻어온다.
	virtual	SI32	GetAddedPlayerList( SOPlayer **pPlayerList );	// 추가되어 있는 플레이어 리스트를 얻어온다.
};

#endif
