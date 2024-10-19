/********************************************************************************************************
	���ϸ�	:	OnlineTrade.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.01.04
	������	:	2002.01.28
********************************************************************************************************/

#ifndef __ON_TRADE_H
#define __ON_TRADE_H


#include "OnlineListControl.h"
#include "OnlineBaseLB.h"


#define ON_VIL_RI_ASKTRADE					(0x00000001)									// �ŷ� ��û
#define	ON_VIL_RI_RES_TRADE					(0x00000002)									// �ŷ� ��û �޴´�.
#define	ON_VIL_RI_PRE_TRADE					(0x00000004)									// ������ �������� ����.
#define	ON_VIL_RI_PRE_TRADE_OK				(0x00000008)									// �ŷ�â�� �������� �ø� �� �� �ִ�

// �޼��� Ÿ���� �˾� ����.
struct OnMsgType
{
	SI16	siType;
};

// ���� ����� ������ ������ ����� ������ ���� �ش�.
struct OnMsgDataNum
{
	SI16	siType;
	SI16	siHeaderNum;
	MONEY	siMoney;
};

// ���� ����� ���ڿ� ������ ������..( ���� ���� ����Ÿ�� ��� ��û�� �Ѵ� )
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


// �������� ��� ����Ÿ�� ���� �ش�.
struct OnMsgData
{
	SI16	siType;
	SI16	siDataNum;

// SAVEFILE_DATAHEADER ����ü�� ���� �´�.
};

struct	TradeFriendInfo
{
	CHAR			szCharName[ON_ID_LENGTH+1];
	SI32			siLevel;
	CHAR			szGuildID[ ON_GUILDNAME_LENGTH + 1 ];
	SI32			siGuildClass;
	UI16			uiDestUniqueID;	 // �ŷ� ��� Unique ID
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
	SI32					m_siStatus;													// �ŷ� ����	
	UI16					m_uiCharID;													// �ŷ� ����� ����ũ ���̵�
	BOOL					m_bActive;													// Ȱ��ȭ
	BOOL					m_bMouseDown;
	BOOL					m_bYouOk1, m_bYouOk2, m_bMyOk1, m_bMyOk2;					// Ok1 : ����, OK2 : ����
	BOOL					m_bWait;													// ����� �ŷ��� �Ұ��ΰ��� �˸���.	
	BOOL					m_bRequest;													// ���� ���濡�� ��û�� �ϴ�.
	BOOL					m_bSuccess;													// ����� ����� ��ȯ ����
	RECT					m_rtCharName;
	SI16					m_siMouseStatus;											// ���콺 ����
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
	BOOL					m_bProperty;												// �ε��� ��ȯ ��ư ����			
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

	BOOL					m_bTradeStop;			// �̼Ӽ��� TRUE�̸� �ŷ� �Ұ����ϰ� �Ѵ�.

public:
	OnlineMsgBox			*m_pMsgBox;
	BOOL					m_bRecvTrade;
	SI16					m_siAddType;
	TradeItem				*m_pTradeItem;
	BOOL					m_bPermitTrade;												// �ŷ� ���
	BOOL					m_bFirstChangeMoney;
	TradeFriendInfo			FriendInfo;
	OnlineSoldierTrade		*m_pSoldier;

public:
	OnlineTrade();
	~OnlineTrade();


	VOID					Init( cltOnlineWorld *pOnlineWorld );
	VOID					Initialize();
	BOOL					IsActive();																	// Ȱ��ȭ �Ǿ� �ִ°�?
	BOOL					IsWait()	{ return m_bWait; };											// �ŷ� �޼���â�� ����� �ִ°�?
	VOID					SetActive( BOOL bFlag );													// Ȱ��ȭ ��Ű�ų� ���δ�.
	VOID					Action();																	// �׼�
	VOID					Draw( LPDIRECTDRAWSURFACE7 pSurface );										// ��ο�
	VOID					Free();
	VOID					ReceiveMoney(MONEY, SI16, UI16, UI08, UI08, MyItemData *pItemInfo=NULL, 
							OnFollowerInfo *pFollower=NULL, OnIndustrialComplexTradeInfo *pProperty=NULL);							// ���� �޴´�.
	VOID					SetOkButton( BOOL bFlag );													// �������� ���� ���� ���� ���� ��ư ����
	VOID					SetCharID(CHAR *pName);														// �ŷ��� ����� ���̸� �޴´�.
	OnlineListControl		*GetList();																	// ���� ����Ʈ ��Ʈ���� ������ �ش�.
	OnlineListControl		*GetYouList()	{ return m_pYouList; };				
	
	//shit.-_-
	VOID					SetStatus(SI32 siStatus){ m_siStatus = siStatus;};
	VOID					SetMouseStats(BOOL bStats) { m_bMouseDown = bStats;};

	
	
	VOID					SetRequest();
	VOID					AddItem( SI16 siYetPos, SI16 siNewPos, UI16 uiItemID, UI32 uiQuantity, UI08 uiFollower = 0);// �߰��� �����ۿ� ���� ������ ������.
	VOID					Success(BOOL bFlag);														// ������ ��ȯ�� ����	

	MONEY					GetSendMoney()			{ return _atoi64( m_szSendMoney );	};
	MONEY					GetResMoney()			{ return _atoi64( m_szResMoney );	};

	VOID					SetPushAccept(BOOL bIsMe);													// ���� ��ư�� ������ �ֵ�.
	VOID					ItemAddCancel(BOOL bCancel);												// �������� ���� �������� �ٽ� ����.
	VOID					SetFriendInfo(CHAR *szGuildName, SI32 siClassInGuildID, SI32 siLevel);

	// robypark 2004/11/24 11:23
	// �ŷ� ����� ���� ���
	CHAR					*GetTradePartnerGuildName()			{ return FriendInfo.szGuildID;	};

	// robypark 2004/12/7 18:4
	// �ŷ� ����� ��峻 ���� ���
	SI32					GetTradePartnerGuildClass() { return FriendInfo.siGuildClass; }

	VOID					ChangeTradeGoods(BYTE *pMsg);

	SI16					GetFollowerSlot(SI08 siSlot)	{ return m_bFollowerSlot[siSlot]; };

private:
	VOID					DrawFollower( LPDIRECTDRAWSURFACE7 pSurface );
};	

#endif