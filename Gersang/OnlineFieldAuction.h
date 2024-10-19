//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 경매창
//	File Name		: OnlineFieldAuction.h
//	Birth Date		: 2002. 10. 19.
//	Creator			: 박 준 태
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용자의 콘솔에서 경매 관련 창을 출력하는 부분입니다.
//
//===================================================================================================

#ifndef	__ONLINE_FIELDAUCTION_HEADER
#define __ONLINE_FIELDAUCTION_HEADER


#include    "..\sccommon\_Auctiondefine.h"

class OnlineListControl;


// 경매 접수 인터페이스
class OnlineReception
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	SI32                                 m_siX,m_siY;                    // 좌표
	SI32                                 m_siStatus;


	XSPR                                 m_BoardSpr;                     // 뒷배경
	XSPR                                 m_SlotSpr;                      // 슬롯 그림
    XSPR                                 m_ValueSpr;                     // 가격입력창
	XSPR                                 *m_pButtonSpr; 
	

	_Button                              m_BOk,m_BCancel;                // 확인 취소 버튼
	
//	OnlineEditControl					 *m_pMinimumPriceEdit;           // 최소가
//	OnlineEditControl					 *m_pMaximumPriceEdit;           // 최대가
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] 신형 에디트 컨트롤 세팅
	HOEC								m_hOecMinPrice;					// 최소가
	HOEC								m_hOecMaxPrice;					// 최대가
	// ===============================================================================

	OnlineListControl					 *m_pAuctionList;                // 경매할 아이템 리스트.

	BOOL                                 m_bActive;                      // 활성화.

	BOOL                                 m_bMouseSwitch;

	int                                  m_siNoField;                    // 노 필드 핸들

	SI32                                 m_siInvenSlot;                  // 어떤 아이템을 올려놓았는가
	UI32                                 m_uiSellFollowerID;             // 아이템을 올려놓은 용병 아이디
	SI32                                 m_siItemCount;                  // 아이템을 얼마나 올려놓았는가.
	MONEY                                m_moSendedMinimumMoney;

public:

	OnlineReception();
	~OnlineReception();

	void                                Init(cltOnlineWorld *pWorld);    // 초기화
	void                                Free();                          // 자원해제

	void                                SetAction(BOOL bActive);       

	void                                SetPos(SI32 siX,SI32 siY);
	
	
	SI32                                GetX(SI32 siX) {return m_siX;};
	SI32                                GetY(SI32 siY) {return m_siY;};

	BOOL                                IsActive() {return m_bActive;};
	
		
	void                                Process();
	void								Draw(LPDIRECTDRAWSURFACE7 pSurface);


	void								WaitingReLay(BYTE *pMsg);
};

// 경매 인터 페이스


class OnlineFieldAuction
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	SI32                                 m_siX,m_siY;                    // 좌표
	SI32                                 m_siStatus;                     // 상태


	SI32                                 m_siTalkDelay;                  // 대사 지속시간
	BOOL                                 m_bKeepTalk;               
	SI32                                 m_siTalk;                 
	CHAR                                 m_szTalk[256];                  // 대사 


	DWORD                                m_dwRemainTime;
	XSPR                                 m_BoardSpr;                     // 뒷배경
	XSPR                                 m_ValueSpr;
	XSPR                                 *m_pButtonSpr;

	_Button                              m_BOk,m_BCancel;                // 확인 취소 버튼

//	OnlineEditControl					 *m_pPriceEdit;                  // 최소가
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] 신형 에디트 컨트롤 세팅
	HOEC								m_hOecPrice;						// 최소가
	// ===============================================================================

	BOOL                                 m_bActive;                      // 활성화.

	BOOL                                 m_bEnter;                       // 관전모드에 들어갔는가?
	BOOL                                 m_bSendEnterMessage;            // 관람 신청 메시지를 보냈음	
	BOOL                                 m_bUnderAuction;                // 경매가 진행중인가?

	BOOL                                 m_bMouseSwitch;

	SI32                                 m_siNoField;

	AUCTION::sCommonInfo                 m_ArticleInfo;
	AUCTION::sArticleItemInfo            m_ItemInfo;
    AUCTION::sArticleFollowerInfo	     m_FollowerInfo;
//	AUCTION::sArticlePropertyInfo		 m_PropertyInfo;

	SI32                                 m_siCurrentBidPrice;

	char                                 m_strMaxBidder[16];              // 현재 최대 입찰가를 제시한 사람.
	SI32                                 m_siChatDelay;                   // 경매 진행인이 떠벌거리는데 필요한 변수.

	SI32                                 m_siAuctionID;                   // 몇번째 경매인이 진행하는 경매에 참여중인가.

public:

	OnlineFieldAuction();
	~OnlineFieldAuction();

	void                                Init(cltOnlineWorld *pWorld);    // 초기화
	void                                OnGameStart();                   // 게임시작할때
	void                                Free();                          // 자원해제

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