#ifndef __CAUCTION_MARKETPROC__
#define __CAUCTION_MARKETPROC__



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 경매소 패킷 처리 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class SOPlayer;
class CAuctionMarketProc
{
private:
	SOPlayer* const m_pPlayer;						// 경매소를 사용할 플레이어 객체 
	
public:
	CAuctionMarketProc(SOPlayer* owner);
	~CAuctionMarketProc();
			
	void	recvRequestNumWait			( char* data );
	void	recvRequestSellItem			( char* data );
	void	recvRequestSellFollower     ( char* data );
	void	recvRequestEnter			( char* data );
	void	recvRequestLeave			( char* data );
	void	recvRequestBidding			( char* data );

private:
	void	sendResponseNumWait         ( int numWait );	
	void	sendResponseSellArticle		( int which   );
	void	sendResponseEnter			( int which   );
	void	sendResponseBidding			( int which   );

private:	
	CAuctionMarket*		getAuctionMarket();
	char*				getStrID();
	BYTE				getCharSlot();
	DWORD				getDBAccount();
	
	DWORD				checkMinMaxPrice(DWORD minPrice, DWORD maxPrice);

	bool	isValidItem(int followerSlot, int itemID, int itemQuantity);
	bool	isValidFollower(int followerSlot);	

};

#endif