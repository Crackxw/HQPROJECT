/********************************************************************************************************
	파일명	:	OnlineTrade.h
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2002.01.04
	수정일	:	2002.01.28
********************************************************************************************************/

#ifndef __ON_TRADE_H
#define __ON_TRADE_H


#include "OnlineListControl.h"
#include "OnlineBaseLB.h"


#define ON_VIL_RI_ASKTRADE					(0x00000001)									// 거래 요청
#define	ON_VIL_RI_RES_TRADE					(0x00000002)									// 거래 요청 받는다.
#define	ON_VIL_RI_PRE_TRADE					(0x00000004)									// 상대방의 아이템을 본다.
#define	ON_VIL_RI_PRE_TRADE_OK				(0x00000008)									// 거래창에 아이템을 올릴 수 가 있다

// 메세지 타입이 알아 본다.
struct OnMsgType
{
	SI16	siType;
};

// 나의 장부의 가격을 내보고 장부의 갯수를 보내 준다.
struct OnMsgDataNum
{
	SI16	siType;
	SI16	siHeaderNum;
	MONEY	siMoney;
};

// 나의 장부의 날자와 마을을 보내줌..( 내가 받을 데이타만 골라서 요청을 한다 )
struct OnReqMsgCheckDate
{
	SI16		siType;
	SI16		siDateNum;

//	UI32		uiVillageID;
//	SAVE_DATE	Date;
};

struct OnResMsgCheckDate
{
	SI16		siType;
	SI16		siDateNum;

//	UI32		uiVillageID;
};


// 실질적인 장부 데이타를 보내 준다.
struct OnMsgData
{
	SI16	siType;
	SI16	siDataNum;

// SAVEFILE_DATAHEADER 구조체가 따라 온다.
};

struct	TradeFriendInfo
{
	CHAR			szCharName[ON_ID_LENGTH+1];
	SI32			siLevel;
	CHAR			szGuildID[ ON_GUILDNAME_LENGTH + 1 ];
	SI32			siGuildClass;
	UI16			uiDestUniqueID;	 // 거래 대상 Unique ID
};


struct PopUp
{
	OnlineSelectLB	Slot;
	XSPR			PopSpr;
	SI16			siSelect;
	BOOL			bType;	
	SI16			siX, siY;
	UI08			Follower[ON_MAX_FOLLOWER_NUM];
};


class OnlineSoldierTrade;
class OnlineMsgBox;
class OnlineListControl;
class OnlineTrade
{
private:
	cltOnlineWorld			*m_pMyOnlineWorld;
	OnlineListControl		*m_pMyList, *m_pYouList;

	_Button					m_BMyMoney, m_BMyOk1, m_BMyOk2;
	_Button					m_BFollower, m_BProperty, m_BExit;

	XSPR					m_MainImage, m_DecideImage, m_AcceptImage, m_MoneyImage, m_FollowerSelImage;
	XSPR					m_But1Image, m_But2Image;

	CHAR					m_szSendMoney[100];
	CHAR					m_szTempSendMoney[100];
	CHAR					m_szResMoney[100];	
	SI32					m_siStatus;													// 거래 상태	
	UI16					m_uiCharID;													// 거래 상대의 유니크 아이디
	BOOL					m_bActive;													// 활성화
	BOOL					m_bMouseDown;
	BOOL					m_bYouOk1, m_bYouOk2, m_bMyOk1, m_bMyOk2;					// Ok1 : 결정, OK2 : 승인
	BOOL					m_bWait;													// 상대방과 거래를 할것인가를 알린다.	
	BOOL					m_bRequest;													// 내가 상대방에게 요청을 하다.
	BOOL					m_bSuccess;													// 장부의 헤더를 교환 성공
	RECT					m_rtCharName;
	SI16					m_siMouseStatus;											// 마우스 상태
	DWORD					m_dwDelayTime, m_dwMsgTime;
	SI16					*m_pYetPos;
	SI16					m_siCount, m_siDelay;
	CHAR					m_szIP[16];
	BOOL					m_bIsFirst;
	BOOL					m_bSendMsg, m_bWaitOkMsg;
	SI16					m_siMsgStatus, m_siNowSendMsgType;
	SI16					m_siRecvTradeNum, m_siFlag;
	MONEY					m_siTradeBookValue;
	SI16					m_siScrollX;
	SI16					m_siX, m_siY, m_siFollowerX, m_siFollowerY;
	SI16					m_siNoField;
	BOOL					m_bProperty;												// 부동산 교환 버튼 상태			
	ItemInfo				m_AddFollower;
	ItemInfo				m_AddProperty;

	UI08					m_uiCurFollower;
	SI16					m_siYetPos;
	SI16					m_siNewPos;
	UI16					m_siDragItem;
	UI16					m_siItemCount;

	SI16					m_bFollowerSlot[ON_MAX_FOLLOWER_NUM];
	POINT					ptMouse;

	MyItemData				m_FollowerItemData[ON_MAX_FOLLOWER_NUM][ON_MYDATA_MAX_ITEM_NUM];

	BOOL					m_bTradeStop;			// 이속성이 TRUE이면 거래 불가능하게 한다.

public:
	OnlineMsgBox			*m_pMsgBox;
	BOOL					m_bRecvTrade;
	SI16					m_siAddType;
	TradeItem				*m_pTradeItem;
	BOOL					m_bPermitTrade;												// 거래 허용
	BOOL					m_bFirstChangeMoney;
	TradeFriendInfo			FriendInfo;
	OnlineSoldierTrade		*m_pSoldier;

public:
	OnlineTrade();
	~OnlineTrade();


	VOID					Init( cltOnlineWorld *pOnlineWorld );
	VOID					Initialize();
	BOOL					IsActive();																	// 활성화 되어 있는가?
	BOOL					IsWait()	{ return m_bWait; };											// 거래 메세지창이 띄워져 있는가?
	VOID					SetActive( BOOL bFlag );													// 활성화 시키거나 죽인다.
	VOID					Action();																	// 액숀
	VOID					Draw( LPDIRECTDRAWSURFACE7 pSurface );										// 드로우
	VOID					Free();
	VOID					ReceiveMoney(MONEY, SI16, UI16, UI08, UI08, MyItemData *pItemInfo=NULL, 
							OnFollowerInfo *pFollower=NULL, OnIndustrialComplexTradeInfo *pProperty=NULL);							// 돈을 받는다.
	VOID					SetOkButton( BOOL bFlag );													// 서버에서 받은 상태 방의 수락 버튼 상태
	VOID					SetCharID(CHAR *pName);														// 거래할 상대의 아이를 받는다.
	OnlineListControl		*GetList();																	// 나의 리스트 컨트롤을 리턴해 준다.
	OnlineListControl		*GetYouList()	{ return m_pYouList; };				
	
	//shit.-_-
	VOID					SetStatus(SI32 siStatus){ m_siStatus = siStatus;};
	VOID					SetMouseStats(BOOL bStats) { m_bMouseDown = bStats;};

	
	
	VOID					SetRequest();
	VOID					AddItem( SI16 siYetPos, SI16 siNewPos, UI16 uiItemID, UI32 uiQuantity, UI08 uiFollower = 0);// 추가된 아이템에 대한 정보를 모은다.
	VOID					Success(BOOL bFlag);														// 아이템 교환이 성공	

	MONEY					GetSendMoney()			{ return _atoi64( m_szSendMoney );	};
	MONEY					GetResMoney()			{ return _atoi64( m_szResMoney );	};

	VOID					SetPushAccept(BOOL bIsMe);													// 수락 버튼을 누를수 있따.
	VOID					ItemAddCancel(BOOL bCancel);												// 마지막에 너은 아이템을 다시 뺀다.
	VOID					SetFriendInfo(CHAR *szGuildName, SI32 siClassInGuildID, SI32 siLevel);

	// robypark 2004/11/24 11:23
	// 거래 상대의 길드명 얻기
	CHAR					*GetTradePartnerGuildName()			{ return FriendInfo.szGuildID;	};

	// robypark 2004/12/7 18:4
	// 거래 상대의 길드내 직급 얻기
	SI32					GetTradePartnerGuildClass() { return FriendInfo.siGuildClass; }

	VOID					ChangeTradeGoods(BYTE *pMsg);

	SI16					GetFollowerSlot(SI08 siSlot)	{ return m_bFollowerSlot[siSlot]; };

private:
	VOID					DrawFollower( LPDIRECTDRAWSURFACE7 pSurface );
};	

#endif