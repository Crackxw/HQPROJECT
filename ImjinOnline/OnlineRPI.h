#ifndef	ON_RPI_HEADER
#define ON_RPI_HEADER

#include "OnlineClient.h"

// ���´� �޽����� �����ϰ� �ִ� ����.
#define	MAX_STORAGE_SEND_ID			200

// �ѹ��� �ִ� 100���� �÷��̾��� ID�� ��û�� �� �ִ�.
#define	MAX_REQUEST_PLAYER_ID		100

// �÷��̾��� ������ �޶�� ��û�ϴ� Ŭ����.
class	OnRPI
{
private:
	UI16	uiStorageSendID[MAX_STORAGE_SEND_ID];


	UI16	uiRequestPlayerNum;
	UI16	uiRequestPlayerID[MAX_REQUEST_PLAYER_ID];

private:
	inline	SI32	FindEmptySlot();			// ��� �ִ� ������ ã�´�.
	inline	SI32	GetSlot(UI16 uiUniqueID);	// ID�� �����Ǿ� �ִ� ������ ã�´�.

//	cltOnlineWorld	*pMyOnlineWorld;
	OnlineClient	*pMyOnlineClient;
public:
	OnRPI();										// ������.	

	VOID	Init(OnlineClient	*pOnlineClient);		// �ʱ�ȭ.
	VOID	Init();									// �ʱ�ȭ.
	VOID	Add(UI16 uiUniqueID);					// �߰�.
	VOID	Del(UI16 uiUniqueID);					// ����.
	VOID	Send();									// �޽����� ������.
};

#endif
