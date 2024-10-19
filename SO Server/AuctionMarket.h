#ifndef __CAUCTION_MARKET__
#define __CAUCTION_MARKET__


#include "_AuctionDefine.h"



//������������������������������������������������������������������������������������������������������������������������
//
// class : ��ż� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CPlayerList;
class CAuctionMarket: public SOVillage_BaseStructure					
{
private:		
	SOWorld* const				m_pWorld;					// ��żҿ��� ���̴� ���� ��ü 

	CPlayerList*				m_AudienceList;				// ������ ����Ʈ 		
	CPlayerList*				m_BidderList;				// ������ ����Ʈ 

	CAuctionList*				m_AuctionList;				// ����� ����Ʈ 

	AUCTION::sBaseInfo*			m_nowProgressAuction;		// ���� �������� ����� ����  
	
	DWORD						m_nowBiddingStartTime;		// ���� �������� ������ ���� �ð� 
	DWORD						m_nowAuctionStartTime;		// ���� �������� ����� ���� �ð� 
	DWORD						m_auctionReadyTime;			// ��� �غ� �ð� 

	int							m_remainTimeListPos;		// ���� �ð� ����Ʈ�� ���� ��ġ 
	
	
	
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

 