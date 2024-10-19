
/************************************************************************************************
	파일명 : OnlineVillageStruct-Inn.h
	담당자 : 이준석(2002.12.3)
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREINN_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREINN_HEADER


// 주점에서의 상태(Main)
#define ON_INN_STATUS_MAIN_START				0
#define ON_INN_STATUS_MAIN_POLL					1
#define ON_INN_STATUS_MAIN_GET_ITEM				2
#define ON_INN_STATUS_MAIN_SEND_ITEM			3
#define ON_INN_STATUS_MAIN_EXIT					4


// 아이템 받기에서의 상태
#define ON_INN_STATUS_GET_ITEM_START			0
#define ON_INN_STATUS_GET_ITEM_POLL				1
#define ON_INN_STATUS_GET_ITEM_GET				2
#define ON_INN_STATUS_GET_ITEM_GET_WAIT			3
#define ON_INN_STATUS_GET_ITEM_EXIT				4

// 아이템 보내기에서의 상태
#define ON_INN_STATUS_DEPOSIT_FOLLOWER_START		0
#define ON_INN_STATUS_DEPOSIT_FOLLOWER_STARTPOOL	1
#define ON_INN_STATUS_DEPOSIT_FOLLOWER_POLL			2
#define ON_INN_STATUS_DEPOSIT_FOLLOWER_SEND			3
#define ON_INN_STATUS_DEPOSIT_FOLLOWER_WAIT			4
#define ON_INN_STATUS_DEPOSIT_FOLLOWER_EXIT			5



// 아이템 보내기 수수료
#define ON_INN_SEND_ITEM_COMMISSION				100000000
#define ON_INN_SEND_ITEM_MAXCOUNT				1



class	OnlineVillageStructureInn : public OnlineVillageBaseStructure
{
private:
	_Button					m_BGetItem, m_BDepositFollower, m_BExit, m_BSubExit;
	BOOL					m_bMouseDown;
	SI16					m_siX, m_siY, m_siInvenX, m_siInvenY;
	POINT					m_ptMouse;

	XSPR					m_MainImage, m_ButtonImage, m_ExitImage, m_InvenImage;
	OnlineListControl		*m_pItemList;
	RECT					m_rtInnRect, m_rtInvenRect;
	BOOL					m_bCommissionOk;
	BOOL					m_bUsableInn;


public:
	SI16					m_siMainStatus, m_siStatus;
	DWORD					m_StartTickCount;

	SI16					m_siInnSlot, m_siInvenSlot;

	// 아이템받기, 보내기
	UI08					m_uiFollowerID;
	UI16					m_uiItemID;
	UI16					m_uiQuantity;

public:
	OnlineVillageStructureInn();																					
	virtual	~OnlineVillageStructureInn();																			
		
	virtual	VOID				Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);
	virtual	VOID				Free();																						
	virtual	BOOL				Poll(LPDIRECTDRAWSURFACE7 pSurface);														
	virtual BOOL				Action();
	OnlineListControl*			GetList()  { return m_pItemList; };

private:
	VOID					Draw(LPDIRECTDRAWSURFACE7 pSurface);
	VOID					DrawGetItem(LPDIRECTDRAWSURFACE7 pSurface);				// 아이템 받기 드로우
	VOID					DrawDepositFollower(LPDIRECTDRAWSURFACE7 pSurface);		// 용병보관 드로우

	VOID					ActionGetItem();										// 아이템 받기 액션
	VOID					ActionDepositFollower();								// 용병보관 액션
};

#endif