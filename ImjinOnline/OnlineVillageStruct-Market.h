#ifndef	_ONLINE_VILLAGEBASESTRUCTUREMARKET_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREMARKET_HEADER

// ������
// ����

#include "Online.h"
#include "OnlineVillageBaseStructure.h"
#include "OnlineListControl.h"
#include "OnlineBaseLB.h"



#define	ON_MARKET_STATUS_ENTER_MARKET		1
#define	ON_MARKET_STATUS_RECEIVE_ITEMLIST	2
#define	ON_MARKET_STATUS_RECEIVE_POLL		3
#define ON_MARKET_STATUS_REQUESTMYITEMLIST	4
#define ON_MARKET_STATUS_ITEMSELL_OK		5
#define ON_MARKET_STATUS_ITEMBUY_OK			6
#define	ON_MARKET_STATUS_ITEMBUY_WAIT		7
#define	ON_MARKET_STATUS_ITEMSELL_WAIT		8
#define ON_MARKET_STATUS_RECEIVEMYITEMLIST	9
#define	ON_MARKET_STATUS_LEAVE_MARKET		10
#define	ON_MARKET_STATUS_RESPONSEITEMMOVE	11
#define	ON_MARKET_STATUS_DELAY				12


struct	SAVEFILE_SEARCH;
class	OnlineVillageStructureMarket : public OnlineVillageBaseStructure
{
private:
	UI32					uiItemID[ON_VILLAGE_ITEM_LISE_MAX];		// �������� �ŷ� ������ ������ ���̵�
	SI16					siSell;									// ��°� �Ĵ°�
	__int64					uiMyMoney;								// ���� ���� ��
	SI32					siSellItemNum;							// �Ĵ� ������ ����

	SI16					m_siX, m_siY;

	XSPR					m_MainImage, m_ExitImage;
	_Button					m_BExit;								// ������ ��ư.
	
	POINT					TempptPosition;
	BOOL					m_bLMouseDown;

	POINT					MoveItemPoint;							// Move Item

	SI16					siSelectNPCItemIndex;
	SI16					siSelectPlayerItemIndex;
	SI16					siSelectItemTime;	

	// �ڼ��� ���콺~ -_-
	SI16					siMouseStatus;							// ���콺 ����
	DWORD					dwDelayMouse1;							// ���콺�� �ٽ� ������ �ð���
	SI16					siDelayTime;							// ���콺 ������
	DWORD					dwDelayMouse2;
	
	SI32					siRecvItemNum;
	UI08					uiOldSelect;
	SI16					siTempDrag;
	RECT					m_rtNpcRect, m_rtPlayerRect;
	BOOL					bTotSelect;								// Alt+Ŭ�� ������ü ���ý�

public:
	SI16					siStatus;								// ���� ���������� �޽����� ��޸��� ���¸� ��Ÿ ����.	
	SI16					siDealerKind;
	UI32					uiID;									// ������ ���̵�
	SI16					siDragItem;								// ���� ����Ʈ ��Ʈ�ѿ��� �����̴� ������
	SI32					NowTradesmanTalk;						// ���� ������ �ϴ¸�
	BOOL					bExit;
	UI08					uiCurFollower;							// Item �̵��� �̵��ϴ� Data�� �����ϴ� Buffer
	UI16					uiMoveItemID;							// Item �̵��� �̵��ϴ� Data�� �����ϴ� Buffer
	SI16					siMoveItemYetPosition;					// Item �̵��� �̵��ϴ� Data�� �����ϴ� Buffer
	SI16					siMoveItemNewPosition;					// Item �̵��� �̵��ϴ� Data�� �����ϴ� Buffer
	SI32					siBuyItemNum;							// ��� ������ ����
	SI16					siBuySlot;								// ��� �������� �� �κ� ����

	OnlineListControl		*pPlayerLC;								// �÷��̾��� ����Ʈ ��Ʈ��
	OnlineListControl		*pTempLC;								// �÷��̾��� �ӽ� ����Ʈ ��Ʈ��
	OnlineListControl		*pNpcLC;								// ������ ����Ʈ ��Ʈ��


public:	
	OnlineVillageStructureMarket();														// ������.
	virtual	~OnlineVillageStructureMarket();											// �Ҹ���.
		
	virtual	VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);// �ʱ�ȭ
	virtual	VOID			Free();														// ����.
	virtual	BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);						// �׼�.	
	virtual BOOL			Action();
	VOID					ModifyData(OnChangeItemInfoMsg *pChangedItemInfo);			// Item ���� ������
	VOID					InsertData(SI32 , OnlineReceivedItemInfo *);				// ���� ����Ÿ�� ���� ����Ÿ�� ���� �Ѵ�.
	VOID					SaveInfo();													// �� �������� ����ϴ� ��� �������� ������ ���Ͽ� ������ �Ѵ�.
	UI16					GetVillageCode()	{ return OnlineVillageBaseStructure::uiVillageCode; };

	
private:
	VOID					MoveItem();													// ���콺 ���¸� üũ�ؼ� �������� �ŷ� �ϴ� ���� �˾� ����.
	VOID					AddPlayerPrice( UI32 uiItemID, MONEY Money );
};


#endif
