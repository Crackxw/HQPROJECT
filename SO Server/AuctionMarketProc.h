#ifndef __CAUCTION_MARKETPROC__
#define __CAUCTION_MARKETPROC__



//������������������������������������������������������������������������������������������������������������������������
//
// class : ��ż� ��Ŷ ó�� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class SOPlayer;
class CAuctionMarketProc
{
private:
	SOPlayer* const m_pPlayer;						// ��żҸ� ����� �÷��̾� ��ü 
	
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