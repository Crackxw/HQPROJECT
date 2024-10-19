/********************************************************************************************************
	파일명	:	OnlineBooth.h
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2002.05.22
********************************************************************************************************/

#ifndef	_ONLINE_ONLINEBOOTH_HEADER
#define _ONLINE_ONLINEBOOTH_HEADER

#define BOOTH_OPEN_MONEY				10000

#define ON_BENCH_IN_INIT				0
#define ON_BENCH_IN_POLL				1
#define ON_BENCH_IN_BENCH_CLICK			2
#define ON_BENCH_IN_INVENTORY_CLICK		3			// 여기서 인벤토리는 좌판 인벤토리를 말함.
#define ON_BENCH_IN_INPUT_ITEM_INFO		4
#define ON_BENCH_IN_ITEM_ADD			5
#define ON_BENCH_IN_ITEM_WAIT			6
#define ON_BENCH_IN_EXIT_INIT			7
#define ON_BENCH_IN_EXIT				8
#define ON_BENCH_IN_EXIT_WAIT			9

	

#define ON_BENCH_OUT_INIT				0
#define ON_BENCH_OUT_BENCH_CLICK		1
#define ON_BENCH_OUT_INVENTORY_POLL		2
#define ON_BENCH_OUT_INVENTORY_CLICK	3
#define ON_BENCH_OUT_MOUSE_ON_ITEM		4
#define ON_BENCH_OUT_BUY_ITEM			5
#define ON_BENCH_OUT_BUY_FOLLOWER		6
#define ON_BENCH_OUT_BUY_ITEM_OK		7
#define ON_BENCH_OUT_BUY_FOLLOWER_OK	8
#define ON_BENCH_OUT_SOLDIER_INFO		9


#include "OnlineListControl.h"

class OnlineSoldierTrade;
class OnlineBooth 
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	OnlineSoldierTrade					*m_pSoldierTrade;

	_Button								m_BFollower, m_BInvenOk, m_BInvenExit;			// OK & Cancel 버튼(좌판 인벤토리)
	XSPR								m_MainSellSpr, m_MainBuySpr, m_pExitButtonSpr;
	OnlineListControl					*m_pBoothList;

	XSPR								m_InfoSellSpr, m_InfoBuySpr, *m_pSlotSpr;		// 아이템, 용병 정보창, 용병슬롯 테두리
	XSPR								m_InfoLineSpr;
	XSPR								m_OkButtonSpr;
	XSPR								m_MercenarySpr;
	_Button								m_BOk, m_BCancel;								// OK & Cancel 버튼(가격입력창)
	_Button								m_BSoldierInfo;
	_Button								m_BInvenBuyInfo;

	BOOL								m_bActionIn;									// 좌판 안에서 액션
	BOOL								m_bActionOut;									// 좌판 밖에서 액션
	BOOL								m_bMouseDown;									// 마우스 왼쪽 버튼 상태
	BOOL								m_bField;										// 좌판인지 필드의 노점인지 구분
	BOOL								m_bOpen;										// 좌판개설 상태인지 체크
	BOOL								m_bInventory;									// 좌판 인벤토리 뜬 상태인지 체크
	BOOL								m_bFollowerMode;								// 용병선택 팝업창 모드인지 체크
	BOOL								m_bFollowerSlot[ON_MAX_FOLLOWER_NUM];			// 좌판에 그용병을 올렸는지 체크
	BOOL								m_bItemSlot[ON_MAX_FOLLOWER_NUM];				// 좌판에 그용병의 아이템을 올렸는지 체크

	SI32								m_StatusIn;										// 좌판의 액션 상태(좌판안)
	SI32								m_StatusOut;									// 좌판의 액션 상태(좌판밖)	

	SI16								m_siX, m_siY;									// 기준 좌표(좌판 인벤토리)
	SI16								m_siInfoX, m_siInfoY;							// 기준 좌표(아이템 정보창)
	SI16								m_siRet;
	UI08								frame;
	POINT								BenchMousePosition;								// 좌판안에 있을때 마우스 클릭위치
	RECT								m_rtBoothRect, m_rtInvenRect;					// 좌판인벤영역, 인벤토리영역
//	PopUp								*m_pPopUp;										// 용병을 올릴때 필요한 팝업창

	// 공용
	SI16								m_siBoothSlot;									// 좌판 인벤토리에서 선택된 아이템 slot 번호
	SI16								m_siInvenSlot;									// 자기 인벤토리에서 선택된 아이템 slot 번호

	// 파는 경우
	SellItemInfo						m_SellItemInfo[ON_MAX_BOOTH_ITEM];				// 현재 좌판 위에 올려져 있는 아이템(서버랑 동일)
	SellItemInfo						m_TempSellItemInfo[ON_MAX_BOOTH_ITEM];			// 확인 누르기 전에 좌판인벤토리에 삽입하는 아이템

//	OnlineEditControl					*m_pPriceEditBox, *m_pDescEditBox;				// 가격, 설명 에디트 박스
	HOEC								m_hOecPrice,m_hOecDesc;
	MapStoreHeader						*m_pSellStoreHeader;							// 좌판 정보(BoothID)
	UI08								m_uiSellFollowerID;
	MONEY								m_siSellItemPrice;								// m_pPriceEditBox 값을 받음
	CHAR								m_szSellItemPrice[16];
	CHAR								m_szSellItemDesc[ON_MAX_BOOTH_NOTE+1];			// m_pDescEditBox  값을 받음

	// 사는 경우
	UI16								m_uiAccount;									// 노점 개설자의 UniqueID
//	OnlineEditControl					*m_pEditBox;									
	UI16								m_siBuyItemID;									// 구입한 아이템 아이디
	UI16								m_siBuyItemQuantity;							// 아이템 구입 개수
	UI08								m_uiBuyFollowerID;								// 구입한 아이템이 들어갈 용병 인덱스
	MapStoreHeader						*m_pBuyStoreHeader;								// 좌판 정보(BoothID)
	SI16								m_siSelect;

public:
	OnlineMsgBox						*pOnlineMsgBoxBench;
	CHAR								m_szBoothName[256];								// m_pBoothNameEditBox  값을 받음
	CHAR								m_szBuyItemDesc[ON_MAX_BOOTH_ITEM][ON_MAX_BOOTH_NOTE+1];	
	MyItemData							m_FollowerItemData[ON_MAX_FOLLOWER_NUM][ON_MYDATA_MAX_ITEM_NUM];	// 팔고있는 용병의 인벤토리 아이템정보
	SI16								m_siNoField;
	BOOL								m_bEnableSoldier[ON_MAX_FOLLOWER_NUM];
//	BOOL								m_bCheckSoldier[ON_MAX_FOLLOWER_NUM];

public:
	OnlineBooth();
	~OnlineBooth();

	VOID	Init(cltOnlineWorld	*pOnlineWorld);											// 초기화
	VOID	Initialize();
	VOID	ActionBench();
	VOID	ActionInBench();															// 좌판에서 사용되는 모든 계산를 해준다.
	VOID	ActionOutOfBench();															// 좌판 밖에서 물건 구입관련 액션
	VOID	Draw(LPDIRECTDRAWSURFACE7 pSurface);										// 계산된 값으로 출력을 한다.
	VOID	DrawSellerInfo(LPDIRECTDRAWSURFACE7 pSurface);								// 파는 사람 관련 출력
	VOID	DrawBuyerInfo(LPDIRECTDRAWSURFACE7 pSurface);								// 사는 사람 관련 출력

	VOID	Free();																		// 사용 했던 메모리를 해제를 한다.
	VOID	SetActionIn(BOOL bFlag);													// 좌판을 활성화 하거나 죽인다.(좌판안)
	VOID	SetActionOut(BOOL bFlag);													// 좌판을 활성화 하거나 죽인다.(좌판밖)
	VOID	SetActionFieldBooth(BOOL bFlag);											// 필드상의 노점 액션 설정

	VOID	ClearBench();
	VOID	InitTempItemInfo();
	BOOL	ClickBenchIn();																// 좌판클릭(파는사람)
	BOOL	ClickBenchOut(SI32 siItemX, SI32 siItemY);									// 좌판클릭(사는사람)

	VOID	SetActiveSellInfo(BOOL active);												// 파는 아이템 정보 입력창 열고 닫기.
	VOID	SetActiveBuyInfo(BOOL active);												// 사는 아이템 정보 입력창 열고 닫기.

	BOOL	MoveOutBench(SI16 siHeroX, SI16 siHeroY);
	BOOL	WhererU(SI16 siHeroX, SI16 siHeroY);										// 캐릭터가 좌판에 들어가나?
	VOID	SetStatusOut(SI32 kindofAction)		{ m_StatusOut = kindofAction; };		// 액션상태를 정해준다.
	VOID	SetStatusIn (SI32 kindofAction)		{ m_StatusIn = kindofAction; };			// 액션상태를 정해준다.
	SI32	GetStatus()	{ return m_StatusIn;   };
	SI32	GetStatusOut()	{ return m_StatusOut;  };
	BOOL	IsActionIn()	{ return m_bActionIn;  };
	BOOL	IsActionOut()	{ return m_bActionOut; };				
	OnlineListControl	*GetList()	{ return m_pBoothList; };							// 리스트 컨트롤 글래스를 리턴한다.	

	BOOL	ErrorMsg();
	BOOL	CheckSameItem(UI08 uiFollowerID, UI16 uiItemID);							// 좌판에 놓는 아이템 중복검사
	VOID	SetUniqueID(UI16 uiAccount) { m_uiAccount = uiAccount;};					// 노점 개설자의 UniqueID 설정
	VOID	SetBoothSpeech(UI16 uiAccount, OnBaseBoothItem* pOnBaseBoothItem, UI08 uiBoothItemNum);
	BOOL	IsOpen();
	VOID	SetFocusEditBox();
	BOOL	IsField()	{ return m_bField; };
	UI16	GetAccount()	{ return m_uiAccount; };

	VOID	SetPopup();
	VOID	PopupAction();
//	VOID	DrawPopup(LPDIRECTDRAWSURFACE7 pSurface);
	BOOL	GetFollowerMode()	{ return m_bFollowerMode; };
	BOOL    GetBoothSlot()		{ return m_siBoothSlot;};



	void	OnResOpen(BYTE *pMsg);
	void	OnResBuyItem(BYTE *pMsg);
	void	OnResClose(BYTE *pMsg);
	void	OnResBuyFollower(BYTE *pMsg);
	void	OnResChangeItem(BYTE *pMsg);

	void	ItemAddBooth(BYTE *pMsg);
	void	ClearBooth();
	SI16	GetFollowerSlot(SI08 siSlot)	{ return m_bFollowerSlot[siSlot]; };
};

#endif
