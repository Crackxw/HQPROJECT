#ifndef	_VILLAGE_WHARF_HEADER
#define	_VILLAGE_WHARF_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"


class	SODatabase;


struct	stWharfData
{
	UI32							uiTotalTicketNum;										// 총 배표 수
	OnTicketInfo					stTicketList[SOVILLAGE_MAX_TICKETCOUNT];				// Ticket List
	TicketHeader					stOTicketList[SOVILLAGE_MAX_TICKETCOUNT];				// Ticket Data List

	SStack<UI16>					WharfUserManager;										// 부두에 있는 사용자들을 관리 하는 객체.
	SStack<UI16>					WharfWaitRoomUserManager;								// 부두대기실에 있는 사용자들을 관리 하는 객체.
};

class	SOVillage_Wharf : public SOVillage_BaseStructure
{
private:
	SOVillage*		m_lpVillage;
	UI16			m_VillageCode;

	stWharfData*	lpstWharfData;

public:
	SOVillage_Wharf();				// 생성자.
	~SOVillage_Wharf();				// 소멸자.
	
	BOOL			Init( UI16 VillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage );						// 초기화.
	VOID			Free();																					// 메모리를 해제한다.	

	BOOL			WharfEnter(SOPlayer *pPlayer);															// 부두에 들어간다.
	BOOL			WharfBuyTicket(SOPlayer *pPlayer, SI32 Kind);											// 부두에서 Ticket을 산다.
	BOOL			WharfExit(SOPlayer *pPlayer);															// 부두를 나간다.
	BOOL			WharfWaitRoomEnter(SOPlayer *pPlayer);													// 부두 대기실에 들어간다.
	BOOL			WharfWaitRoomExit(SOPlayer *pPlayer);													// 부두 대기실에서 나간다.
};


#endif
