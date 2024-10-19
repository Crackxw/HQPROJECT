#ifndef __CAUCTIONLIST__
#define __CAUCTIONLIST__

#include "_AuctionDefine.h"



//������������������������������������������������������������������������������������������������������������������������
//
// class : ��� ����Ʈ Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CAuctionList
{
private:
	const DWORD					MAX_AUCTION;						// ����� �ִ� �� ( MAX__AUCTON <= 255 )

	DWORD						m_numAuction;						// ����Ʈ���� ��Ź��� �� 
	DWORD						m_uniqIDSeed;						// ����ũ ���̵��� seed

	AUCTION::sBaseInfo*			m_AuctionList;						// ��� ����Ʈ (�迭)
		
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