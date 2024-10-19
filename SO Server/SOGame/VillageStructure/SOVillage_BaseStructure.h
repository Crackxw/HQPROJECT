#ifndef _VILLAGE_BASESTRUCTURE
#define	_VILLAGE_BASESTRUCTURE

class	SOPlayer;

#include "SOCommonHeader.h"
#include "PlayersManager.h"
#include "Stack.h"

class	SOVillage_BaseStructure
{

private:	
	PlayersManager			m_PlayerMgr;				// 이 건물에 들어가 있는 플레이어 관리자			

protected:
	UI16                    m_uiVillageCode;			// 이 건물이 속해 있는 마을의 코드

public:

	SOVillage_BaseStructure();									// 생성자.
	virtual	~SOVillage_BaseStructure();							// 소멸자.

	virtual	VOID		Init( UI16 uiVillageCode, DWORD dwMaxPlayer );				// 이 건물에 최대 들어올 수 있는 사람의 수를 설정한다.
	virtual	VOID		Free();														// 메모리를 해제한다.

	virtual	BOOL		EnterPlayer( SOPlayer *pPlayer );		// 플레이어가 이 건물에 들어왔다.
	virtual	BOOL		LeavePlayer( SOPlayer *pPlayer );		// 플레이어가 이 건물에서 나갔다.
	
	BOOL		IsEntered( SOPlayer *pPlayer );					// 플레이어가 이 건물에 들어와있는지 검사한다.
	SI32		GetEnteredPlayerList( UI16 *puiPlayerList );	// 이 건물에 들어와 있는 플레이어의 리스트를 얻어온다.

	// 현재 이 건물에 들어와있는 모든 플레이어게 동일한 메시지를 보낸다.
	BOOL		SendMsgToAllPlayer( SI32 siMsg, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL );
};

#endif

