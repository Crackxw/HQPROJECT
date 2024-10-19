//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> ���â
//	File Name		: OnlineFieldAuction.h
//	Birth Date		: 2002. 10. 19.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ������� �ֿܼ��� ��� ���� â�� ����ϴ� �κ��Դϴ�.
//
//===================================================================================================

#ifndef	__ONLINE_FIELDAUCTION_HEADER
#define __ONLINE_FIELDAUCTION_HEADER


#include    "..\sccommon\_Auctiondefine.h"

class OnlineListControl;


// ��� ���� �������̽�
class OnlineReception
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	SI32                                 m_siX,m_siY;                    // ��ǥ
	SI32                                 m_siStatus;


	XSPR                                 m_BoardSpr;                     // �޹��
	XSPR                                 m_SlotSpr;                      // ���� �׸�
    XSPR                                 m_ValueSpr;                     // �����Է�â
	XSPR                                 *m_pButtonSpr; 
	

	_Button                              m_BOk,m_BCancel;                // Ȯ�� ��� ��ư
	
//	OnlineEditControl					 *m_pMinimumPriceEdit;           // �ּҰ�
//	OnlineEditControl					 *m_pMaximumPriceEdit;           // �ִ밡
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
	HOEC								m_hOecMinPrice;					// �ּҰ�
	HOEC								m_hOecMaxPrice;					// �ִ밡
	// ===============================================================================

	OnlineListControl					 *m_pAuctionList;                // ����� ������ ����Ʈ.

	BOOL                                 m_bActive;                      // Ȱ��ȭ.

	BOOL                                 m_bMouseSwitch;

	int                                  m_siNoField;                    // �� �ʵ� �ڵ�

	SI32                                 m_siInvenSlot;                  // � �������� �÷����Ҵ°�
	UI32                                 m_uiSellFollowerID;             // �������� �÷����� �뺴 ���̵�
	SI32                                 m_siItemCount;                  // �������� �󸶳� �÷����Ҵ°�.
	MONEY                                m_moSendedMinimumMoney;

public:

	OnlineReception();
	~OnlineReception();

	void                                Init(cltOnlineWorld *pWorld);    // �ʱ�ȭ
	void                                Free();                          // �ڿ�����

	void                                SetAction(BOOL bActive);       

	void                                SetPos(SI32 siX,SI32 siY);
	
	
	SI32                                GetX(SI32 siX) {return m_siX;};
	SI32                                GetY(SI32 siY) {return m_siY;};

	BOOL                                IsActive() {return m_bActive;};
	
		
	void                                Process();
	void								Draw(LPDIRECTDRAWSURFACE7 pSurface);


	void								WaitingReLay(BYTE *pMsg);
};

// ��� ���� ���̽�


class OnlineFieldAuction
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	SI32                                 m_siX,m_siY;                    // ��ǥ
	SI32                                 m_siStatus;                     // ����


	SI32                                 m_siTalkDelay;                  // ��� ���ӽð�
	BOOL                                 m_bKeepTalk;               
	SI32                                 m_siTalk;                 
	CHAR                                 m_szTalk[256];                  // ��� 


	DWORD                                m_dwRemainTime;
	XSPR                                 m_BoardSpr;                     // �޹��
	XSPR                                 m_ValueSpr;
	XSPR                                 *m_pButtonSpr;

	_Button                              m_BOk,m_BCancel;                // Ȯ�� ��� ��ư

//	OnlineEditControl					 *m_pPriceEdit;                  // �ּҰ�
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
	HOEC								m_hOecPrice;						// �ּҰ�
	// ===============================================================================

	BOOL                                 m_bActive;                      // Ȱ��ȭ.

	BOOL                                 m_bEnter;                       // ������忡 ���°�?
	BOOL                                 m_bSendEnterMessage;            // ���� ��û �޽����� ������	
	BOOL                                 m_bUnderAuction;                // ��Ű� �������ΰ�?

	BOOL                                 m_bMouseSwitch;

	SI32                                 m_siNoField;

	AUCTION::sCommonInfo                 m_ArticleInfo;
	AUCTION::sArticleItemInfo            m_ItemInfo;
    AUCTION::sArticleFollowerInfo	     m_FollowerInfo;
//	AUCTION::sArticlePropertyInfo		 m_PropertyInfo;

	SI32                                 m_siCurrentBidPrice;

	char                                 m_strMaxBidder[16];              // ���� �ִ� �������� ������ ���.
	SI32                                 m_siChatDelay;                   // ��� �������� �����Ÿ��µ� �ʿ��� ����.

	SI32                                 m_siAuctionID;                   // ���° ������� �����ϴ� ��ſ� �������ΰ�.

public:

	OnlineFieldAuction();
	~OnlineFieldAuction();

	void                                Init(cltOnlineWorld *pWorld);    // �ʱ�ȭ
	void                                OnGameStart();                   // ���ӽ����Ҷ�
	void                                Free();                          // �ڿ�����

	void                                SetAction(BOOL bActive);       

	void                                SetPos(SI32 siX,SI32 siY);
	
	
	SI32                                GetX(SI32 siX) {return m_siX;};
	SI32                                GetY(SI32 siY) {return m_siY;};

	BOOL                                IsActive() {return m_bActive;};
	
		
	void                                Process();
	void                                TalkChange();
	void								Draw(LPDIRECTDRAWSURFACE7 pSurface);

	void                                Enter();
	void                                Exit();

	BOOL                                IsEntered() {return m_bEnter;};
	BOOL                                IsUnderAuction() {return m_bUnderAuction;};
	BOOL                                CanBid();
	BOOL								CanWeight();

	void								Bidding(BYTE *pMsg);
	void								ReportReMainTime(BYTE *pMsg);
	void								ReportBidding(BYTE *pMsg);
	void								ReportSuccessful(BYTE *pMsg);
	void								ReportFail(BYTE *pMsg);
	void								ReportNew(BYTE *pMsg);
	void								Enter(BYTE *pMsg);
};

#define AUCTIONTALK_NO_AUCTION                 0
#define AUCTIONTALK_ITEM                       100
#define AUCTIONTALK_ITEM_WEAPON                120 
#define AUCTIONTLAK_ITEM_ARMOR                 140
#define AUCTIONTALK_ITEM_DRESS                 160
#define AUCTIONTALK_ITEM_FOOD                  180
#define AUCTIONTALK_ITEM_MEDICINE              200
#define AUCTIONTALK_ITEM_OTHER                 220
#define AUCTIONTALK_FOLLOWER                   300
#define AUCTIONTALK_PROPERTY                   400
#define AUCTIONTALK_NEW                        500

#define DEFAULT_TALK_DELAY                     180



#endif