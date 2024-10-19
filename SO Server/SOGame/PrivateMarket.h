#ifndef	PRIVATE_MARKET_HEADER
#define PRIVATE_MARKET_HEADER


class	SOPlayer;

#include "SOCommonHeader.h"

struct	strPMItem			// Private Market Item Class
{		
	MONEY		Price;							// �������� ����.
	CHAR		szNote[ON_MAX_BOOTH_NOTE+1];	// �������� ����.
	UI16		uiFollowerID;					// �ŷ��� �������� ���� �뺴 ID.
	UI16		uiItemID;						// �ŷ��� �������� ID.	
	UI08		uiTradeFollowerSlotID;			// �ŷ��� �뺴�� ID Slot. ( ����� �ŷ� �ȵ� )
	SI08		siType;							// ������ �ŷ����� �뺴 �ŷ����� ����
};

class	cltPrivateMarket
{
private:
	SOPlayer		*m_pOwnerPlayer;

	strPMItem		m_stPMItem[ON_MAX_BOOTH_ITEM];
	BOOL			m_bOpen;
private:
	BOOL		IsValidSlot(UI08 uiSlot);		// ��ȿ�� �������� �˾ƿ´�.	
public:
	VOID	Init(SOPlayer *pOwnerPlayer);					// �ʱ�ȭ.
	VOID	Init();											// �ʱ�ȭ.
	VOID	Open();											// ���� ������ ����.
	VOID	Close();										// ���� ������ �ݴ´�.
	BOOL	IsOpen();										// ���� ������ ���� �ִ°�?	
	
	
	BOOL	AddItem(OnBoothItemForSell	*pBoothItem);		// ���� ������ �������� �߰��Ѵ�.
	//BOOL	AddItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote);		// ���� ������ �������� �߰��Ѵ�.
	BOOL	DelItem(UI08 uiSlotID);																		// ���� �������� �������� �����Ѵ�.
	BOOL	BuyItem(UI08 uiSlotID, UI16 uiQuantity, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *psiMercenarySlot);	// ���� �������� �������� ���.	
	BOOL	ChangeItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote, UI08);	// ���� �������� �Ǹ��ϴ� �������� �ٲ۴�.
	DWORD	GetBaseItems(OnBaseBoothItem *pBaseBoothItem, SI08 *psiItemNum);							// ���� �������� ���� �Ǹ��ϴ� �����۵��� ���´�.	
	DWORD	GetItems(OnBoothItemInfo *pBoothItem, SI08 *psiItemNum, SOPlayer *pPlayer,MyItemData *pFollowerItems, UI16 &numFollowerItems);	// ���� �������� ���� �Ǹ��ϴ� �����۵��� ���´�.	
	
	// ���� �������� �Ǹ��ϴ� �������� ������ ���´�.
	strPMItem*	GetItem(UI08 uiSlotID);				
	strPMItem*	GetItem(UI08 uiSlotID, UI16 uiItemID);


	BOOL	GetItemQuantity(UI08 uiSlotID, UI16 *puiQuantity);											// ���� �������� �Ǹ��ϰ� �ִ� �������� ������ ���´�.
	BOOL	CanBuyItem(UI08 uiSlotID, UI16 uiQuantity, MONEY Money);									// ���� �������� �������� �� �� �ִ°�?	

	BOOL	GetTotalPrice( UI08 uiSlotID, UI16 uiQuantity, MONEY *pPirce );								// ���� �������� �Ǹ��ϰ� �ִ� �������� ������ �˾ƿ´�.
};

#endif 