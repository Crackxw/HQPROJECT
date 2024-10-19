#ifndef __CAUCTIONLIST__
#define __CAUCTIONLIST__

#include "_AuctionDefine.h"



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 경매 리스트 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CAuctionList
{
private:
	const DWORD					MAX_AUCTION;						// 경매의 최대 수 ( MAX__AUCTON <= 255 )

	DWORD						m_numAuction;						// 리스트에서 경매물의 수 
	DWORD						m_uniqIDSeed;						// 유니크 아이디의 seed

	AUCTION::sBaseInfo*			m_AuctionList;						// 경매 리스트 (배열)
		
public:
	CAuctionList(DWORD maxAuction);
	~CAuctionList();

	int							getNum();	
	AUCTION::sBaseInfo*			get( DWORD index );

	bool						add( AUCTION::sBaseInfo* auct );

	bool						add( char*       strAuctioneer,
									 BYTE		 auctioneerCharSlot,
									 DWORD       auctioneerDBAccount, 
									 SOnlineTime registrationDate, 
									 SOnlineTime finishedDate,
									 char*		 strBidder,
									 BYTE		 bidderCharSlot,	
									 DWORD		 bidderDBAccount,
									 DWORD		 biddingPrice,
									 WORD		 numBidding,									 
									 DWORD		 minPrice, 
									 DWORD		 maxPrice,	
									 BYTE		 numFail,
									 BYTE		 articleKind,
									 char*		 articleData, 
									 WORD		 articleDataSize );

	void					    remove( DWORD index );

private:
	DWORD						getUniqID();
};

#endif