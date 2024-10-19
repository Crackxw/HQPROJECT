#ifndef _VILLAGE_BASESTRUCTURE
#define	_VILLAGE_BASESTRUCTURE

class	SOPlayer;

#include "SOCommonHeader.h"
#include "PlayersManager.h"
#include "Stack.h"

class	SOVillage_BaseStructure
{

private:	
	PlayersManager			m_PlayerMgr;				// �� �ǹ��� �� �ִ� �÷��̾� ������			

protected:
	UI16                    m_uiVillageCode;			// �� �ǹ��� ���� �ִ� ������ �ڵ�

public:

	SOVillage_BaseStructure();									// ������.
	virtual	~SOVillage_BaseStructure();							// �Ҹ���.

	virtual	VOID		Init( UI16 uiVillageCode, DWORD dwMaxPlayer );				// �� �ǹ��� �ִ� ���� �� �ִ� ����� ���� �����Ѵ�.
	virtual	VOID		Free();														// �޸𸮸� �����Ѵ�.

	virtual	BOOL		EnterPlayer( SOPlayer *pPlayer );		// �÷��̾ �� �ǹ��� ���Դ�.
	virtual	BOOL		LeavePlayer( SOPlayer *pPlayer );		// �÷��̾ �� �ǹ����� ������.
	
	BOOL		IsEntered( SOPlayer *pPlayer );					// �÷��̾ �� �ǹ��� �����ִ��� �˻��Ѵ�.
	SI32		GetEnteredPlayerList( UI16 *puiPlayerList );	// �� �ǹ��� ���� �ִ� �÷��̾��� ����Ʈ�� ���´�.

	// ���� �� �ǹ��� �����ִ� ��� �÷��̾�� ������ �޽����� ������.
	BOOL		SendMsgToAllPlayer( SI32 siMsg, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL );
};

#endif

