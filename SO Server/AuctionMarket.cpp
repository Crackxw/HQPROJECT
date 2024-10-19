#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "SOVillage.h"
#include "AuctionList.h"
#include "AuctionMarket.h"
#include "PlayerList.h"
		


//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ 
//-------------------------------------------------------------------------------------------
CAuctionMarket::CAuctionMarket(SOWorld* world): m_pWorld(world)
{			
	m_AuctionList	= new CAuctionList ( AUCTION::MAX_WAITING );		

	m_BidderList	= new CPlayerList  ( AUCTION::MAX_BIDDER );
	m_AudienceList	= new CPlayerList  ( AUCTION::MAX_AUDIENCE );			
	

	clearBidderList();	
	clearAudienceList();
	

	m_nowProgressAuction   = NULL;
		
	m_nowBiddingStartTime  = 0;
	m_nowAuctionStartTime  = 0;
	m_auctionReadyTime	   = 0;
	m_remainTimeListPos	   = 0;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �Ҹ��� 
//-------------------------------------------------------------------------------------------
CAuctionMarket::~CAuctionMarket()
{	
	if( m_AuctionList  != NULL ) delete m_AuctionList;	
	
	if( m_AudienceList != NULL ) delete m_AudienceList;
	if( m_BidderList   != NULL ) delete m_BidderList;
}









//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� �������� ����� ������� ���´� 
//-------------------------------------------------------------------------------------------
SOPlayer* CAuctionMarket::getNowAuctioneer()
{						
	if( m_nowProgressAuction )
	{
		return m_pWorld->GetPlayerByDBAccount( m_nowProgressAuction->auctioneerDBAccount, 
											   m_nowProgressAuction->auctioneerCharSlot );								
	}
	
	return NULL;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : index�� �ش��ϴ� ��� �ϳ��� ���´� 
//-------------------------------------------------------------------------------------------
AUCTION::sBaseInfo* CAuctionMarket::getAuction( DWORD index )
{		
	return m_AuctionList->get(index);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ����� ��� ���� ���´� 
//		   (����� ��� ���� ������ �Ѿ��� ��� -1�� ���� �Ѵ�)
//-------------------------------------------------------------------------------------------
int CAuctionMarket::getNumWait()
{
	return m_AuctionList->getNum() >= AUCTION::MAX_WAITING ? -1 : m_AuctionList->getNum();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� �������� ��Ÿ� ���´� 
//-------------------------------------------------------------------------------------------
AUCTION::sBaseInfo* CAuctionMarket::getNowProgressAuction()
{
	return m_nowProgressAuction;
}



//-------------------------------------------------------------------------------------------
//  Name :
//  Desc : ���� �ð��� ���´� 
//-------------------------------------------------------------------------------------------
SOnlineTime CAuctionMarket::getNowTime()
{
	return m_pWorld->CurrentTime;
}



//-------------------------------------------------------------------------------------------
//  Name :
//  Desc : ��� ���� �ð��� ���´� ( ���� �ð��� ���� ������ ���Ѵ� )
//-------------------------------------------------------------------------------------------
SOnlineTime CAuctionMarket::getAuctionExpireTime()
{
	return m_pWorld->Timer.CheckTime( m_pWorld->CurrentTime, AUCTION::EXPIRE_TERM );
}









//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ο� ��� �߰� 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addNewAuction(AUCTION::sBaseInfo* auct)
{
	return m_AuctionList->add(auct);
}

//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ο� ��� �߰� 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addNewAuction( char* strAuctioneer,
									BYTE  auctioneerCharSlot,
									DWORD auctioneerDBAccount, 
									SOnlineTime registrationDate, 
									SOnlineTime finishedDate,
									char* strBidder,
									BYTE  bidderCharSlot,
									DWORD bidderDBAccount,
									DWORD biddingPrice,
									WORD  numBidding,
									DWORD minPrice, 
									DWORD maxPrice,
									BYTE  numFail,
									BYTE  articleKind,
									char* articleData, 
									WORD  articleDataSize )
{
		
	return m_AuctionList->add( strAuctioneer, auctioneerCharSlot, auctioneerDBAccount, 
							   registrationDate, finishedDate, 
							   strBidder, bidderCharSlot, bidderDBAccount, biddingPrice, numBidding, 
							   minPrice, maxPrice, numFail,
							   articleKind, articleData, articleDataSize );
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��� �ϳ� ���� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::removeAuction( DWORD id )
{	
	m_AuctionList->remove(id);
}
















//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ڸ� �߰��Ѵ� 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addBidder(SOPlayer* pPlayer)
{

	return m_BidderList->add(pPlayer);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ڸ� �����Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::removeBidder(SOPlayer* pPlayer)
{
	// �ְ� �����ڰ� ���� ��� 
	if( pPlayer == m_BidderList->get( m_BidderList->getNum() - 1 ) )
	{		
		setPrevBidderToHighestBidder(); 				
	}
	
	m_BidderList->remove(pPlayer);	
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ����Ʈ�� �ʱ�ȭ �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::clearBidderList()
{
	m_BidderList->clear();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ڰ� �ִ°�? 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::hasBidder()
{
	return m_BidderList->getNum() > 0 ? TRUE : FALSE;
}










//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �������� �߰��Ѵ� 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addAudience(SOPlayer* pPlayer)
{
	return m_AudienceList->add(pPlayer);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �������� �����Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::removeAudience(SOPlayer* pPlayer)
{
	m_AudienceList->remove(pPlayer);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ����Ʈ�� �ʱ�ȭ �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::clearAudienceList()
{
	m_AudienceList->clear();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �������� �ִ°�? 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::hasAudience()
{
	return m_AudienceList->getNum() > 0 ? TRUE : FALSE;
}










//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��� �߰��� DB�� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::DB_addItemAuction( char* strAuctioneer, BYTE auctioneerCharSlot, DWORD auctioneerDBAccount, 
										DWORD minPrice, DWORD maxPrice, BYTE numFail, BYTE articleKind,
										WORD  itemID, WORD itemQuantity, BYTE hasItemFollowerSlot, 
										DWORD feePrice )
{	
	sDB_AddItemAuction s;

	memcpy(s.strAuctioneer, strAuctioneer, LOGIN::NAME_MAX_LENGTH);

	s.auctioneerCharslot	= auctioneerCharSlot;
	s.auctioneerDBAccount   = auctioneerDBAccount;	
	s.maxPrice			    = maxPrice;
	s.minPrice				= minPrice;
	s.numFail				= numFail;
	s.articleKind			= AUCTION::ARTICLE_ITEM;	
	s.itemID				= itemID;
	s.itemQuantity			= itemQuantity;
	s.hasItemFollowerSlot	= hasItemFollowerSlot;
	s.feePrice				= feePrice;
												   
	m_pWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_ADD_ITEM_AUCTION, &s, sizeof(s) );
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��� ������ DB�� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::DB_successfulBidItemAuction( WORD  itemID, 
												  WORD  itemQuantity, 
												  WORD  posInInventory, 
												  BYTE  recvItemFollowerSlot )
{ 
	if( m_nowProgressAuction )
	{
		sDB_SuccessfulBidItemAuction s;
			
		s.auctioneerCharSlot	= m_nowProgressAuction->auctioneerCharSlot;
		s.auctioneerDBAccount	= m_nowProgressAuction->auctioneerDBAccount;			
		s.bidderCharSlot		= m_nowProgressAuction->bidderCharSlot;
		s.bidderDBAccount		= m_nowProgressAuction->bidderDBAccount;
		s.biddingPrice			= m_nowProgressAuction->biddingPrice;		
		s.articleKind			= AUCTION::ARTICLE_ITEM;
		s.itemID				= itemID;
		s.itemQuantity			= itemQuantity;
		s.posInInventory		= posInInventory;
		s.recvItemFollowerSlot	= recvItemFollowerSlot;
	

		m_pWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SUCCESSFULBID_ITEM_AUCTION, &s, sizeof(s) );
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��� ������ DB�� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::DB_failBidItemAuction()
{
	if( m_nowProgressAuction )
	{
		sDB_FailBidItemAuction s;

		s.auctioneerCharSlot	= m_nowProgressAuction->auctioneerCharSlot;
		s.auctioneerDBAccount   = m_nowProgressAuction->auctioneerDBAccount;		


		m_pWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_FAILBID_ITEM_AUCTION, &s, sizeof(s) );
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��� ������ DB�� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::DB_delItemAuction()
{
	if( m_nowProgressAuction )
	{
		sDB_DelItemAuction s;

		s.auctioneerCharSlot	= m_nowProgressAuction->auctioneerCharSlot;
		s.auctioneerDBAccount   = m_nowProgressAuction->auctioneerDBAccount;		


		m_pWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_DEL_ITEM_AUCTION, &s, sizeof(s) );
	}
}












//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� �����ڸ� �ְ� �����ڷ� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::setPrevBidderToHighestBidder()
{
	if( m_nowProgressAuction )
	{					
		SOPlayer* prevBidder = m_BidderList->get( m_BidderList->getNum() - 2 );

		
		if( prevBidder )
		{						
			m_nowProgressAuction->bidderCharSlot  = prevBidder->stAccountInfo.uiSlot;
			m_nowProgressAuction->bidderDBAccount = prevBidder->stAccountInfo.siDBAccount;
			m_nowProgressAuction->biddingPrice	  = prevBidder->getTempBiddingPrice();			
			m_nowProgressAuction->setStrBidder(prevBidder->szID);	
			
			m_nowBiddingStartTime = timeGetTime();	
			
			resetRemainTimeListPos();		
		}

		
		else
		{
			m_nowProgressAuction->bidderCharSlot  = 0;
			m_nowProgressAuction->bidderDBAccount = 0;
			m_nowProgressAuction->biddingPrice	  = 0;		
			m_nowProgressAuction->setStrBidder("");	
			
			m_nowBiddingStartTime  = 0;		

			resetRemainTimeListPos();
		}		


		
		sendReportBidding();
	}
}











//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ �õ� �Ѵ� 
//-------------------------------------------------------------------------------------------
int CAuctionMarket::DoBidding( SOPlayer* pBidder, char* strBidder, BYTE bidderCharSlot,
							   DWORD bidderDBAccount, DWORD auctionUniqID, DWORD biddingPrice )
{	
	if( m_nowProgressAuction )
	{		
		if( auctionUniqID == m_nowProgressAuction->uniqID )	
		{
			 
			
			int result = checkBiddingPrice( biddingPrice > pBidder->GetMoney() ? 0 : biddingPrice );

			if( result >= AUCTION::VALID_BIDDING_PRICE )
			{

				if( addBidder(pBidder) )
				{						
					// ���� ���� ������ ���� �Ѵ� 
					m_nowProgressAuction->bidderCharSlot  = bidderCharSlot;
					m_nowProgressAuction->bidderDBAccount = bidderDBAccount;
					m_nowProgressAuction->biddingPrice	  = biddingPrice;
					m_nowProgressAuction->setStrBidder(strBidder);											
					
					m_nowBiddingStartTime = timeGetTime();
					
					resetRemainTimeListPos();



					// �������� �ӽ÷� ���� �صд� ( �ְ� �����ڰ� ������ ������ ��츦 ���� )
					pBidder->setTempBiddingPrice( biddingPrice );


					// �������� �ְ��� ��� ���� ��Ų�� 						
					if( result == AUCTION::MAX_BIDDING_PRICE ) 
						m_nowBiddingStartTime += AUCTION::BID_LIMIT_TIME;


					m_nowProgressAuction->numBidding++;			

					
					sendReportBidding();					

					return 1;
				}
				else
					return 0;
			}
		}
	}

	return -1;
}





//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ �õ� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::DoFailBid()
{
	if( m_nowProgressAuction )
	{	
		// ���� �� ��� �ּҰ��� 20% ���� ��Ų�� 		
		m_nowProgressAuction->minPrice -= (m_nowProgressAuction->minPrice * AUCTION::MIN_PRICE_DEC);


		if( ++m_nowProgressAuction->numFail < AUCTION::MAX_FAIL )
		{			
			if(!m_AuctionList->add(m_nowProgressAuction))
			{
				// ��� ���� 
				DB_delItemAuction();
			}			
			else
			{
				DB_failBidItemAuction();			
			}			
		}
		else
		{			
			// ��� ���� 
			DB_delItemAuction();
		}

		
		sendReportFailBid();											

		sendNotifyResult(getNowAuctioneer());
		

		// ��� ���� 		
		m_nowProgressAuction = NULL;
		removeAuction(0);					
	}
}




//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ �õ� �Ѵ� 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::DoSuccessfulBid()
{

	if( m_nowProgressAuction )
	{		
		// ������
		SOPlayer* pSuccessfulBidder = m_BidderList->getLast();
		

		if( pSuccessfulBidder && pSuccessfulBidder->GetMoney() >= m_nowProgressAuction->biddingPrice ) 
		{

			switch( m_nowProgressAuction->articleKind ) 
			{

				// ������ �� ��� 
				case AUCTION::ARTICLE_ITEM:
					{

						AUCTION::sArticleItemInfo*  orgItemInfo = (AUCTION::sArticleItemInfo*)m_nowProgressAuction->articleData;						
						AUCTION::sArticleItemInfo2  sendItemInfo;
						short						posInInventory;
						
						
						for( int slot=0; slot < ON_MAX_FOLLOWER_NUM; ++slot )
						{
							
							// �������� �����ڿ��� �߰� �Ѵ� 
							if( pSuccessfulBidder->IFollower.CanAddItem( slot, orgItemInfo->itemID, orgItemInfo->quantity ) == TRUE )
							{
								if( pSuccessfulBidder->IFollower.AddItem( slot, orgItemInfo->itemID, orgItemInfo->quantity, &posInInventory ) ) 				
								{									
									sendItemInfo.hasFollowerSlot  = slot;							
									sendItemInfo.posInInventory   = posInInventory;
									

																	
									SOPlayer* pAuctioneer;  															
									if( pAuctioneer = getNowAuctioneer() )								
									{																		
										// ����� ������ ���� ���� ��Ų�� 
										pAuctioneer->ModifySaving(m_nowProgressAuction->biddingPrice);
										
										// ����� ����� ������ �ش� 
										sendNotifyResult(pAuctioneer);
									}



									
									// �������� ���� ���� ��Ų�� 							
									pSuccessfulBidder->DecreaseMoney(m_nowProgressAuction->biddingPrice, FALSE); 

									
									// ������ �˸��� 							
									sendReportSuccessfulBid( pSuccessfulBidder, 
															 (char*)&sendItemInfo, 
															 sizeof(sendItemInfo) );


									// DB�� ������ ���� �Ѵ� 
									DB_successfulBidItemAuction( orgItemInfo->itemID,
																 orgItemInfo->quantity,
																 sendItemInfo.posInInventory,
																 sendItemInfo.hasFollowerSlot );

		
									// ��� ���� 								
									m_nowProgressAuction = NULL;
									removeAuction(0);				
									

									return TRUE;

								}
							}
						}
					}
					break;

				// �뺴 �� ��� 			
				/*
				case AUCTION::ARTICLE_FOLLOWER:
					{				
						
						AUCTION::sArticleFollowerInfo*   FollowerInfo = (AUCTION::sArticleFollowerInfo*)m_nowProgressAuction->articleData;
						AUCTION::sArticleFollowerInfo2   sendFollowerInfo;
						
						FollowerInfo->slot = pSuccessfulBidder->IFollower.FindEmptyFollowerSlot();

						if( FollowerInfo->slot > -1 )
						{
							// �����ڿ��� �뺴�� �߰��� �ش� 
							if( pSuccessfulBidder->IFollower.InitFollower( FollowerInfo->slot, (OnFollowerInfo*)FollowerInfo, 0, &m_pVillage->clWorld->clItem ) )
							{
								sendFollowerInfo.followerSlot = FollowerInfo->slot;


								// ������� ���� ���� ��Ų�� 
								

								// �������� ���� ���� ��Ų�� 							
								pSuccessfulBidder->DecreaseMoney( m_nowProgressAuction->biddingPrice ); 


								// ������ �˸��� 							
								sendReportSuccessfulBid( pSuccessfulBidder, 
														 (char*)&sendFollowerInfo,
														 sizeof(sendFollowerInfo) );

								// ��� ���� 								
								m_nowProgressAuction = NULL;
								removeAuction(0);				

								return TRUE;
							}
						}										
					}
					break;
				*/
				default:
					break;
			}
		}			
	}

	// ���� ��Ų�� 
	DoFailBid();

	return FALSE;
}









//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �̹� ����� ���� ��Ű� �ִ°�? 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::isAlreadyAddAuction(DWORD auctioneerDBAccount, BYTE auctioneerCharSlot)
{
	for(int i=0; i < m_AuctionList->getNum(); ++i)
	{
		if( m_AuctionList->get(i)->auctioneerDBAccount == auctioneerDBAccount &&
			m_AuctionList->get(i)->auctioneerCharSlot  == auctioneerCharSlot )
		{
			return TRUE;
		}
	}

	return FALSE;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� �ð� ����Ʈ�� ���� ��ġ�� �缳�� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::resetRemainTimeListPos()
{	
	int i = 0;
	while ( AUCTION::REMAIN_TIME_LIST[i] != -1 )
	{
		if( getNowAuctionRemainTime() > AUCTION::REMAIN_TIME_LIST[i] ) 
		{
			m_remainTimeListPos = i; 
			break;
		}

		++i;
	}	
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �������� ��ȿ������ �˻� �Ѵ� 
//-------------------------------------------------------------------------------------------
int CAuctionMarket::checkBiddingPrice(DWORD newBiddingPrice)
{	

	AUCTION::sBaseInfo* _p = m_nowProgressAuction;
		

	// ���ο� ������ 0 ���� �۰ų� ������?
	if( newBiddingPrice <= 0 ) {

		return AUCTION::INVALID_BIDDING_PRICE;
	}

	
	// ���� �������� �̹� �ְ� ������ �ΰ�?
	if( newBiddingPrice == _p->biddingPrice ) {

		return AUCTION::INVALID_BIDDING_PRICE;
	}


	// ���ο� �������� �ּҰ��� �ִ밡�� ���� ��Ű�°�?
	if((newBiddingPrice > _p->maxPrice && _p->maxPrice > 0) ||
	    newBiddingPrice < _p->minPrice ) {		

		return AUCTION::INVALID_BIDDING_PRICE;
	}
	
				
	// ���ο� �������� ���� ������ ���� 10% �̻� ū��?
	if( newBiddingPrice < _p->biddingPrice + (_p->biddingPrice * AUCTION::BID_PRICE_MUL) ) {
		
		return AUCTION::INVALID_BIDDING_PRICE;
	}
		
	
	// ���ο� �������� �ְ� ������ �ΰ�?
	if( newBiddingPrice == _p->maxPrice && _p->maxPrice > 0 ) {
	
		return AUCTION::MAX_BIDDING_PRICE;
	}
							
	
	return AUCTION::VALID_BIDDING_PRICE;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ����� ���� �ð��� ���´� 
//-------------------------------------------------------------------------------------------
DWORD CAuctionMarket::getNowAuctionRemainTime()
{
	DWORD result  = 0;
	DWORD curTime = timeGetTime();	


	if( m_nowBiddingStartTime > 0 && (m_nowBiddingStartTime + AUCTION::BID_LIMIT_TIME) <
									 (m_nowAuctionStartTime + AUCTION::LIMIT_TIME) )				
	{	
		result = (m_nowBiddingStartTime + AUCTION::BID_LIMIT_TIME) - curTime;	
	}
	else
	{
		result = (m_nowAuctionStartTime + AUCTION::LIMIT_TIME) - curTime;
	}

	return result < 0 ? 0 : result;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ����� �߰��� ���� �ð��� �ش� 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::auctionDelay()
{		
	DWORD curTime = timeGetTime();

	if( m_auctionReadyTime == 0 ) m_auctionReadyTime = curTime;		
		
	if( curTime - m_auctionReadyTime <= AUCTION::READY_TIME ) return TRUE;

	m_auctionReadyTime = 0;

	return FALSE;
}







//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::Poll()
{

	DWORD curTime = timeGetTime();
	


	if( m_nowProgressAuction == NULL )
	{		
		
		if( auctionDelay() ) return;
		//if( !hasAudience() ) return;
		if( m_AudienceList->getNum() < 3 ) return;

		
		if( m_nowProgressAuction = m_AuctionList->get(0) ) 
		{			

			m_nowProgressAuction->registrationDate = getNowTime();
			m_nowProgressAuction->finishedDate     = getAuctionExpireTime();			
			
			m_nowAuctionStartTime  = curTime;				
			m_nowBiddingStartTime  = 0;
			m_remainTimeListPos	   = 0;		
					
			clearBidderList();						


			// ���ο� ����� ������ ���� �Ѵ� 
			sendReportNewInfo();	

			sendNotifyAdvertise();
		}
	}
	

	else 
	{							
		
		DWORD	 auctionElapseTime = curTime - m_nowAuctionStartTime;
		
		
		if( m_nowBiddingStartTime > 0 )
		{					
			
			DWORD	 biddingElapseTime = curTime - m_nowBiddingStartTime;	
			
			if( biddingElapseTime >= AUCTION::BID_LIMIT_TIME )
			{
				DoSuccessfulBid();	
			    return;
			}
		}
		
		
		else if( auctionElapseTime >= AUCTION::PREVIEW_TIME ) 
		{
			DoFailBid();
			return;
		}
		

		// ��� ���� �ð��� ������ ��� 
		if( auctionElapseTime >= AUCTION::LIMIT_TIME ) 
		{
			
			if( hasBidder() ) 
			{

				DoSuccessfulBid();	
				return;
			}
			
			else 
			{
								
				DoFailBid();
				return;
			}				
		}

		
		sendReportRemainTime();
	}
}













//-------------------------------------------------------------------------------------------
//  Name : 
//  Desc : ������ �ϰ� �ִ� ����̿��� ��Ÿ� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendNotifyAdvertise()
{	
	if( m_nowProgressAuction )
	{
		AUCTION::sNotifyAdvertise packet;
		memset(&packet, 0, sizeof(packet));

		packet.command		= AUCTION::NOTIFY_ADVERTISE;
		
		packet.articleKind  = m_nowProgressAuction->articleKind;
		packet.setData( m_nowProgressAuction->articleData, 
						m_nowProgressAuction->articleDataSize );


		int packetSize = sizeof(AUCTION::sNotifyAdvertiseFront) + packet.articleDataSize;
								

		for( int i = 1; i < ON_MAX_CONNECTION + 1; ++i )
		{
			if( m_pWorld->pPlayerOnAccount[i] != NULL )
			{			
				m_pWorld->pPlayerOnAccount[i]->SendFieldMsg(ON_OFFICIAL_ANNOUNCEMENT, SO_SFM_ALLCHAR, LPVOID(&packet), LPVOID(packetSize));
				break;
			}
		}
	}
}



//-------------------------------------------------------------------------------------------
//  Name : 
//  Desc : ����ο��� ��� ����� �˷��ش� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendNotifyResult( SOPlayer* pAuctioneer )
{
	if( pAuctioneer && m_nowProgressAuction )
	{
		AUCTION::sNotifyResult packet;
		memset(&packet, 0, sizeof(packet));

		packet.command		= AUCTION::NOTIFY_RESULT;	
		
		packet.biddingPrice = m_nowProgressAuction->biddingPrice;	
		packet.articleKind	= m_nowProgressAuction->articleKind;	
		packet.setData( m_nowProgressAuction->articleData, 
			            m_nowProgressAuction->articleDataSize );


	    pAuctioneer->Write( &packet, sizeof(AUCTION::sNotifyResultFront) + packet.articleDataSize );
	}									
}



//-------------------------------------------------------------------------------------------
//  Name : 
//  Desc : ���� ����� ���� �ð��� �����ش� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendReportRemainTime()
{						
	DWORD  remainTime = getNowAuctionRemainTime();	


	// ���� �ð��� ������ �� ������ Ȯ�� �Ѵ� 
	if(	remainTime < AUCTION::REMAIN_TIME_LIST[ m_remainTimeListPos ] )	
	{						
		AUCTION::sReportRemainTime	packet;
		memset(&packet, 0, sizeof(packet));

		packet.command		=	AUCTION::REPORT_REMAIN_TIME;
		packet.remainTime	=   remainTime;
		
		
		m_AudienceList->write(&packet, sizeof(packet));					
		++m_remainTimeListPos;		
	}
}



//-------------------------------------------------------------------------------------------
//  Name : 
//  Desc : ���ο� ������ ���� �ش� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendReportNewInfo()
{
	if( m_nowProgressAuction )
	{
		AUCTION::sReportNewInfo packet;
		memset(&packet, 0, sizeof(packet));

		packet.command = AUCTION::REPORT_NEW_INFO;


		packet._c.uniqID			  = m_nowProgressAuction->uniqID;
		packet._c.numFail			  = m_nowProgressAuction->numFail;
		packet._c.minPrice			  = m_nowProgressAuction->minPrice;			
		packet._c.maxPrice			  = m_nowProgressAuction->maxPrice;
		packet._c.finishedDate		  = m_nowProgressAuction->finishedDate;
		packet._c.articleKind		  = m_nowProgressAuction->articleKind;		
		packet.setStrAuctioneer( m_nowProgressAuction->strAuctioneer );
		packet.setData( m_nowProgressAuction->articleData, 
					    m_nowProgressAuction->articleDataSize );


		m_AudienceList->write(&packet, sizeof(packet));
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ε鿡�� ������ �˸���  
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendReportFailBid()
{
	if( m_nowProgressAuction )
	{
		AUCTION::sReportFailBid	packet;
		memset(&packet, 0, sizeof(packet));

		packet.command	= AUCTION::REPORT_FAIL_BID;
		packet.uniqID	= m_nowProgressAuction->uniqID;


		m_AudienceList->write(&packet, sizeof(packet));
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ε�(����������)���� ������ �˸��� 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendReportSuccessfulBid(SOPlayer* pSuccessfulBidder, char* articleData, WORD articleDataSize)
{	
	if( pSuccessfulBidder && m_nowProgressAuction )
	{		
		AUCTION::sReportSuccessfulBid packet;
		memset(&packet, 0, sizeof(packet));

		packet.command		= AUCTION::REPORT_SUCCESSFUL_BID;


		packet.uniqID	    = m_nowProgressAuction->uniqID;			
		packet.biddingPrice = m_nowProgressAuction->biddingPrice;
		packet.articleKind	= m_nowProgressAuction->articleKind;		
		packet.setData( articleData, articleDataSize );
	
		
		// �����ڿ��Դ� ����ǰ ������ �ǳ� �ش� 
		m_AudienceList->write( &packet, 12, pSuccessfulBidder );
		pSuccessfulBidder->Write( &packet, sizeof(AUCTION::sReportSuccessfulBidFront) + packet.articleDataSize );		
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �����ε鿡�� ���ο� ������ �˸���  
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendReportBidding()
{
	if( m_nowProgressAuction )
	{		
		AUCTION::sReportBidding packet;
		memset(&packet, 0, sizeof(packet));

		packet.command		 = AUCTION::REPORT_BIDDING;
	

		packet.uniqID		 = m_nowProgressAuction->uniqID;
		packet.biddingPrice	 = m_nowProgressAuction->biddingPrice;	
		packet.setStrBidder(m_nowProgressAuction->strBidder);
		
		
		m_AudienceList->write(&packet, sizeof(packet));
	}
}






