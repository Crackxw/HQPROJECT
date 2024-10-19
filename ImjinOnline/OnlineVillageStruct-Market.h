#ifndef	_ONLINE_VILLAGEBASESTRUCTUREMARKET_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREMARKET_HEADER

// 정진욱
// 시장

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
	UI32					uiItemID[ON_VILLAGE_ITEM_LISE_MAX];		// 마을에서 거래 가능한 아이템 아이디
	SI16					siSell;									// 사는가 파는가
	__int64					uiMyMoney;								// 현재 나의 돈
	SI32					siSellItemNum;							// 파는 아이템 개수

	SI16					m_siX, m_siY;

	XSPR					m_MainImage, m_ExitImage;
	_Button					m_BExit;								// 나가기 버튼.
	
	POINT					TempptPosition;
	BOOL					m_bLMouseDown;

	POINT					MoveItemPoint;							// Move Item

	SI16					siSelectNPCItemIndex;
	SI16					siSelectPlayerItemIndex;
	SI16					siSelectItemTime;	

	// 자석식 마우스~ -_-
	SI16					siMouseStatus;							// 마우스 상태
	DWORD					dwDelayMouse1;							// 마우스가 다시 눌리는 시간차
	SI16					siDelayTime;							// 마우스 딜레이
	DWORD					dwDelayMouse2;
	
	SI32					siRecvItemNum;
	UI08					uiOldSelect;
	SI16					siTempDrag;
	RECT					m_rtNpcRect, m_rtPlayerRect;
	BOOL					bTotSelect;								// Alt+클릭 물건전체 선택시

public:
	SI16					siStatus;								// 현재 서버에서의 메시지를 기달리는 상태를 나타 낸다.	
	SI16					siDealerKind;
	UI32					uiID;									// 아이템 아이디
	SI16					siDragItem;								// 현재 리스트 컨트롤에서 움직이는 아이템
	SI32					NowTradesmanTalk;						// 현재 상인이 하는말
	BOOL					bExit;
	UI08					uiCurFollower;							// Item 이동시 이동하는 Data를 저장하는 Buffer
	UI16					uiMoveItemID;							// Item 이동시 이동하는 Data를 저장하는 Buffer
	SI16					siMoveItemYetPosition;					// Item 이동시 이동하는 Data를 저장하는 Buffer
	SI16					siMoveItemNewPosition;					// Item 이동시 이동하는 Data를 저장하는 Buffer
	SI32					siBuyItemNum;							// 사는 아이템 개수
	SI16					siBuySlot;								// 사는 아이템이 들어갈 인벤 슬롯

	OnlineListControl		*pPlayerLC;								// 플레이어의 리스트 컨트롤
	OnlineListControl		*pTempLC;								// 플레이어의 임시 리스트 컨트롤
	OnlineListControl		*pNpcLC;								// 상점의 리스트 컨트롤


public:	
	OnlineVillageStructureMarket();														// 생성자.
	virtual	~OnlineVillageStructureMarket();											// 소멸자.
		
	virtual	VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);// 초기화
	virtual	VOID			Free();														// 해제.
	virtual	BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);						// 액션.	
	virtual BOOL			Action();
	VOID					ModifyData(OnChangeItemInfoMsg *pChangedItemInfo);			// Item 가격 수정시
	VOID					InsertData(SI32 , OnlineReceivedItemInfo *);				// 마을 데이타와 나의 데이타를 삽입 한다.
	VOID					SaveInfo();													// 이 마을엥서 취급하는 모든 아이템의 정보를 파일에 저장을 한다.
	UI16					GetVillageCode()	{ return OnlineVillageBaseStructure::uiVillageCode; };

	
private:
	VOID					MoveItem();													// 마우스 상태를 체크해서 아이템의 거래 하는 것을 알아 낸다.
	VOID					AddPlayerPrice( UI32 uiItemID, MONEY Money );
};


#endif
