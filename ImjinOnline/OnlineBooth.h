/********************************************************************************************************
	���ϸ�	:	OnlineBooth.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.05.22
********************************************************************************************************/

#ifndef	_ONLINE_ONLINEBOOTH_HEADER
#define _ONLINE_ONLINEBOOTH_HEADER

#define BOOTH_OPEN_MONEY				10000

#define ON_BENCH_IN_INIT				0
#define ON_BENCH_IN_POLL				1
#define ON_BENCH_IN_BENCH_CLICK			2
#define ON_BENCH_IN_INVENTORY_CLICK		3			// ���⼭ �κ��丮�� ���� �κ��丮�� ����.
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

	_Button								m_BFollower, m_BInvenOk, m_BInvenExit;			// OK & Cancel ��ư(���� �κ��丮)
	XSPR								m_MainSellSpr, m_MainBuySpr, m_pExitButtonSpr;
	OnlineListControl					*m_pBoothList;

	XSPR								m_InfoSellSpr, m_InfoBuySpr, *m_pSlotSpr;		// ������, �뺴 ����â, �뺴���� �׵θ�
	XSPR								m_InfoLineSpr;
	XSPR								m_OkButtonSpr;
	XSPR								m_MercenarySpr;
	_Button								m_BOk, m_BCancel;								// OK & Cancel ��ư(�����Է�â)
	_Button								m_BSoldierInfo;
	_Button								m_BInvenBuyInfo;

	BOOL								m_bActionIn;									// ���� �ȿ��� �׼�
	BOOL								m_bActionOut;									// ���� �ۿ��� �׼�
	BOOL								m_bMouseDown;									// ���콺 ���� ��ư ����
	BOOL								m_bField;										// �������� �ʵ��� �������� ����
	BOOL								m_bOpen;										// ���ǰ��� �������� üũ
	BOOL								m_bInventory;									// ���� �κ��丮 �� �������� üũ
	BOOL								m_bFollowerMode;								// �뺴���� �˾�â ������� üũ
	BOOL								m_bFollowerSlot[ON_MAX_FOLLOWER_NUM];			// ���ǿ� �׿뺴�� �÷ȴ��� üũ
	BOOL								m_bItemSlot[ON_MAX_FOLLOWER_NUM];				// ���ǿ� �׿뺴�� �������� �÷ȴ��� üũ

	SI32								m_StatusIn;										// ������ �׼� ����(���Ǿ�)
	SI32								m_StatusOut;									// ������ �׼� ����(���ǹ�)	

	SI16								m_siX, m_siY;									// ���� ��ǥ(���� �κ��丮)
	SI16								m_siInfoX, m_siInfoY;							// ���� ��ǥ(������ ����â)
	SI16								m_siRet;
	UI08								frame;
	POINT								BenchMousePosition;								// ���Ǿȿ� ������ ���콺 Ŭ����ġ
	RECT								m_rtBoothRect, m_rtInvenRect;					// �����κ�����, �κ��丮����
//	PopUp								*m_pPopUp;										// �뺴�� �ø��� �ʿ��� �˾�â

	// ����
	SI16								m_siBoothSlot;									// ���� �κ��丮���� ���õ� ������ slot ��ȣ
	SI16								m_siInvenSlot;									// �ڱ� �κ��丮���� ���õ� ������ slot ��ȣ

	// �Ĵ� ���
	SellItemInfo						m_SellItemInfo[ON_MAX_BOOTH_ITEM];				// ���� ���� ���� �÷��� �ִ� ������(������ ����)
	SellItemInfo						m_TempSellItemInfo[ON_MAX_BOOTH_ITEM];			// Ȯ�� ������ ���� �����κ��丮�� �����ϴ� ������

//	OnlineEditControl					*m_pPriceEditBox, *m_pDescEditBox;				// ����, ���� ����Ʈ �ڽ�
	HOEC								m_hOecPrice,m_hOecDesc;
	MapStoreHeader						*m_pSellStoreHeader;							// ���� ����(BoothID)
	UI08								m_uiSellFollowerID;
	MONEY								m_siSellItemPrice;								// m_pPriceEditBox ���� ����
	CHAR								m_szSellItemPrice[16];
	CHAR								m_szSellItemDesc[ON_MAX_BOOTH_NOTE+1];			// m_pDescEditBox  ���� ����

	// ��� ���
	UI16								m_uiAccount;									// ���� �������� UniqueID
//	OnlineEditControl					*m_pEditBox;									
	UI16								m_siBuyItemID;									// ������ ������ ���̵�
	UI16								m_siBuyItemQuantity;							// ������ ���� ����
	UI08								m_uiBuyFollowerID;								// ������ �������� �� �뺴 �ε���
	MapStoreHeader						*m_pBuyStoreHeader;								// ���� ����(BoothID)
	SI16								m_siSelect;

public:
	OnlineMsgBox						*pOnlineMsgBoxBench;
	CHAR								m_szBoothName[256];								// m_pBoothNameEditBox  ���� ����
	CHAR								m_szBuyItemDesc[ON_MAX_BOOTH_ITEM][ON_MAX_BOOTH_NOTE+1];	
	MyItemData							m_FollowerItemData[ON_MAX_FOLLOWER_NUM][ON_MYDATA_MAX_ITEM_NUM];	// �Ȱ��ִ� �뺴�� �κ��丮 ����������
	SI16								m_siNoField;
	BOOL								m_bEnableSoldier[ON_MAX_FOLLOWER_NUM];
//	BOOL								m_bCheckSoldier[ON_MAX_FOLLOWER_NUM];

public:
	OnlineBooth();
	~OnlineBooth();

	VOID	Init(cltOnlineWorld	*pOnlineWorld);											// �ʱ�ȭ
	VOID	Initialize();
	VOID	ActionBench();
	VOID	ActionInBench();															// ���ǿ��� ���Ǵ� ��� ��긦 ���ش�.
	VOID	ActionOutOfBench();															// ���� �ۿ��� ���� ���԰��� �׼�
	VOID	Draw(LPDIRECTDRAWSURFACE7 pSurface);										// ���� ������ ����� �Ѵ�.
	VOID	DrawSellerInfo(LPDIRECTDRAWSURFACE7 pSurface);								// �Ĵ� ��� ���� ���
	VOID	DrawBuyerInfo(LPDIRECTDRAWSURFACE7 pSurface);								// ��� ��� ���� ���

	VOID	Free();																		// ��� �ߴ� �޸𸮸� ������ �Ѵ�.
	VOID	SetActionIn(BOOL bFlag);													// ������ Ȱ��ȭ �ϰų� ���δ�.(���Ǿ�)
	VOID	SetActionOut(BOOL bFlag);													// ������ Ȱ��ȭ �ϰų� ���δ�.(���ǹ�)
	VOID	SetActionFieldBooth(BOOL bFlag);											// �ʵ���� ���� �׼� ����

	VOID	ClearBench();
	VOID	InitTempItemInfo();
	BOOL	ClickBenchIn();																// ����Ŭ��(�Ĵ»��)
	BOOL	ClickBenchOut(SI32 siItemX, SI32 siItemY);									// ����Ŭ��(��»��)

	VOID	SetActiveSellInfo(BOOL active);												// �Ĵ� ������ ���� �Է�â ���� �ݱ�.
	VOID	SetActiveBuyInfo(BOOL active);												// ��� ������ ���� �Է�â ���� �ݱ�.

	BOOL	MoveOutBench(SI16 siHeroX, SI16 siHeroY);
	BOOL	WhererU(SI16 siHeroX, SI16 siHeroY);										// ĳ���Ͱ� ���ǿ� ����?
	VOID	SetStatusOut(SI32 kindofAction)		{ m_StatusOut = kindofAction; };		// �׼ǻ��¸� �����ش�.
	VOID	SetStatusIn (SI32 kindofAction)		{ m_StatusIn = kindofAction; };			// �׼ǻ��¸� �����ش�.
	SI32	GetStatus()	{ return m_StatusIn;   };
	SI32	GetStatusOut()	{ return m_StatusOut;  };
	BOOL	IsActionIn()	{ return m_bActionIn;  };
	BOOL	IsActionOut()	{ return m_bActionOut; };				
	OnlineListControl	*GetList()	{ return m_pBoothList; };							// ����Ʈ ��Ʈ�� �۷����� �����Ѵ�.	

	BOOL	ErrorMsg();
	BOOL	CheckSameItem(UI08 uiFollowerID, UI16 uiItemID);							// ���ǿ� ���� ������ �ߺ��˻�
	VOID	SetUniqueID(UI16 uiAccount) { m_uiAccount = uiAccount;};					// ���� �������� UniqueID ����
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
