#ifndef	_VILLAGE_WHARF_HEADER
#define	_VILLAGE_WHARF_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"


class	SODatabase;


struct	stWharfData
{
	UI32							uiTotalTicketNum;										// �� ��ǥ ��
	OnTicketInfo					stTicketList[SOVILLAGE_MAX_TICKETCOUNT];				// Ticket List
	TicketHeader					stOTicketList[SOVILLAGE_MAX_TICKETCOUNT];				// Ticket Data List

	SStack<UI16>					WharfUserManager;										// �εο� �ִ� ����ڵ��� ���� �ϴ� ��ü.
	SStack<UI16>					WharfWaitRoomUserManager;								// �εδ��ǿ� �ִ� ����ڵ��� ���� �ϴ� ��ü.
};

class	SOVillage_Wharf : public SOVillage_BaseStructure
{
private:
	SOVillage*		m_lpVillage;
	UI16			m_VillageCode;

	stWharfData*	lpstWharfData;

public:
	SOVillage_Wharf();				// ������.
	~SOVillage_Wharf();				// �Ҹ���.
	
	BOOL			Init( UI16 VillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage );						// �ʱ�ȭ.
	VOID			Free();																					// �޸𸮸� �����Ѵ�.	

	BOOL			WharfEnter(SOPlayer *pPlayer);															// �εο� ����.
	BOOL			WharfBuyTicket(SOPlayer *pPlayer, SI32 Kind);											// �εο��� Ticket�� ���.
	BOOL			WharfExit(SOPlayer *pPlayer);															// �εθ� ������.
	BOOL			WharfWaitRoomEnter(SOPlayer *pPlayer);													// �ε� ���ǿ� ����.
	BOOL			WharfWaitRoomExit(SOPlayer *pPlayer);													// �ε� ���ǿ��� ������.
};


#endif
