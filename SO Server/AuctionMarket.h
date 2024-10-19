#ifndef __CAUCTION_MARKET__
#define __CAUCTION_MARKET__


#include "_AuctionDefine.h"



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 경매소 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CPlayerList;
class CAuctionMarket: public SOVillage_BaseStructure					
{
private:		
	SOWorld* const				m_pWorld;					// 경매소에서 쓰이는 월드 객체 

	CPlayerList*				m_AudienceList;				// 참관인 리스트 		
	CPlayerList*				m_BidderList;				// 입찰자 리스트 

	CAuctionList*				m_AuctionList;				// 경매의 리스트 

	AUCTION::sBaseInfo*			m_nowProgressAuction;		// 현재 진행중인 경매의 정보  
	
	DWORD						m_nowBiddingStartTime;		// 현재 진행중인 입찰의 시작 시간 
	DWORD						m_nowAuctionStartTime;		// 현재 진행중인 경매의 시작 시간 
	DWORD						m_auctionReadyTime;			// 경매 준비 시간 

	int							m_remainTimeListPos;		// 남은 시간 리스트의 현재 위치 
	
	
	
public:
	CAuctionMarket(SOWorld* world);
	~CAuctionMarket();
	
	//---------------------------------------------------------------------
	bool					addAudience(SOPlayer* pPlayer);
	void					removeAudience(SOPlayer* pPlayer);
	void					clearAudienceList();		
	bool					hasAudience();

	//---------------------------------------------------------------------
	bool					addBidder(SOPlayer* pPlayer);	
	void					removeBidder(SOPlayer* pPlayer);	
	void					clearBidderList();
	bool					hasBidder();


	//---------------------------------------------------------------------
	bool					addNewAuction( char* strAuctioneer,
										   BYTE  auctioneerCharSlot,
										   DWORD auctioneerDBAccount, 
										   SOnlineTime registrationDate, 
										   SOnlineTime finishedDate,
										   char* strBidder,
										   BYTE	 bidderCharSlot,
										   DWORD bidderDBAccount,
										   DWORD biddingPrice,
										   WORD  numBidding,
										   DWORD minPrice, 	
										   DWORD maxPrice,
										   BYTE	 numFail,
										   BYTE  articleKind,
										   char* articleData, 
										   WORD  articleDataSize);

	bool					addNewAuction( AUCTION::sBaseInfo* auct );
	void					removeAuction( DWORD id );	


	//---------------------------------------------------------------------
	void					DB_addItemAuction( char* strAuctioneer, BYTE auctioneerCharSlot, DWORD auctioneerDBAccount, 
											   DWORD minPrice, DWORD maxPrice, BYTE numFail, BYTE articleKind,
											   WORD  itemID, WORD itemQuantity, BYTE hasItemFollowerSlot, DWORD feePrice );

	void					DB_successfulBidItemAuction( WORD  itemID, 
														 WORD  itemQuantity, 
														 WORD  posInInventory, 
														 BYTE  recvItemFollowerSlot );
	void					DB_failBidItemAuction();
	void					DB_delItemAuction();
	


	//---------------------------------------------------------------------
	AUCTION::sBaseInfo*		getAuction( DWORD index );				
	AUCTION::sBaseInfo*		getNowProgressAuction();
	int						getNumWait();		
	SOnlineTime				getNowTime();	
	SOnlineTime				getAuctionExpireTime();	
	DWORD					getNowAuctionRemainTime();
	SOPlayer*				getNowAuctioneer();
	

	bool					isAlreadyAddAuction(DWORD auctioneerDBAccount, BYTE auctioneerCharSlot);
	void					resetRemainTimeListPos();	
	int						checkBiddingPrice(DWORD newBiddingPrice);
	bool					auctionDelay();


	bool					DoSuccessfulBid();
	void					DoFailBid();
	int						DoBidding( SOPlayer* pBidder, char* strBidder, BYTE bidderCharSlot, DWORD bidderDBAccount, DWORD auctionUniqID, DWORD biddingPrice );	
	
	void					setPrevBidderToHighestBidder();
		

	void					sendNotifyResult( SOPlayer* pAuctioneer );
	void					sendNotifyAdvertise();

	void					sendReportRemainTime();
	void					sendReportNewInfo();
	void					sendReportBidding();	
	void					sendReportFailBid();
	void					sendReportSuccessfulBid( SOPlayer* pSuccessfulBidder, char* articleData, WORD articleDataSize );	
		
	void					Poll();
};


#endif

 