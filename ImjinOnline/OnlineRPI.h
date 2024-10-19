#ifndef	ON_RPI_HEADER
#define ON_RPI_HEADER

#include "OnlineClient.h"

// 보냈던 메시지를 보관하고 있는 버퍼.
#define	MAX_STORAGE_SEND_ID			200

// 한번에 최대 100명의 플레이어의 ID만 요청할 수 있다.
#define	MAX_REQUEST_PLAYER_ID		100

// 플레이어의 정보를 달라고 요청하는 클래스.
class	OnRPI
{
private:
	UI16	uiStorageSendID[MAX_STORAGE_SEND_ID];


	UI16	uiRequestPlayerNum;
	UI16	uiRequestPlayerID[MAX_REQUEST_PLAYER_ID];

private:
	inline	SI32	FindEmptySlot();			// 비어 있는 슬롯을 찾는다.
	inline	SI32	GetSlot(UI16 uiUniqueID);	// ID가 설정되어 있는 슬롯을 찾는다.

//	cltOnlineWorld	*pMyOnlineWorld;
	OnlineClient	*pMyOnlineClient;
public:
	OnRPI();										// 생성자.	

	VOID	Init(OnlineClient	*pOnlineClient);		// 초기화.
	VOID	Init();									// 초기화.
	VOID	Add(UI16 uiUniqueID);					// 추가.
	VOID	Del(UI16 uiUniqueID);					// 삭제.
	VOID	Send();									// 메시지를 보낸다.
};

#endif
