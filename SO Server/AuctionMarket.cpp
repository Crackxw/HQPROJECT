#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "SOVillage.h"
#include "AuctionList.h"
#include "AuctionMarket.h"
#include "PlayerList.h"
		


//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생성자 
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
//	Desc : 소멸자 
//-------------------------------------------------------------------------------------------
CAuctionMarket::~CAuctionMarket()
{	
	if( m_AuctionList  != NULL ) delete m_AuctionList;	
	
	if( m_AudienceList != NULL ) delete m_AudienceList;
	if( m_BidderList   != NULL ) delete m_BidderList;
}









//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 진행중인 경매의 경매인을 얻어온다 
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
//	Desc : index에 해당하는 경매 하나를 얻어온다 
//-------------------------------------------------------------------------------------------
AUCTION::sBaseInfo* CAuctionMarket::getAuction( DWORD index )
{		
	return m_AuctionList->get(index);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 경매의 대기 수를 얻어온다 
//		   (경매의 대기 수가 제한을 넘었을 경우 -1을 리턴 한다)
//-------------------------------------------------------------------------------------------
int CAuctionMarket::getNumWait()
{
	return m_AuctionList->getNum() >= AUCTION::MAX_WAITING ? -1 : m_AuctionList->getNum();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 진행중인 경매를 얻어온다 
//-------------------------------------------------------------------------------------------
AUCTION::sBaseInfo* CAuctionMarket::getNowProgressAuction()
{
	return m_nowProgressAuction;
}



//-------------------------------------------------------------------------------------------
//  Name :
//  Desc : 현재 시간을 얻어온다 
//-------------------------------------------------------------------------------------------
SOnlineTime CAuctionMarket::getNowTime()
{
	return m_pWorld->CurrentTime;
}



//-------------------------------------------------------------------------------------------
//  Name :
//  Desc : 경매 만기 시간을 얻어온다 ( 현재 시간에 기한 시일을 더한다 )
//-------------------------------------------------------------------------------------------
SOnlineTime CAuctionMarket::getAuctionExpireTime()
{
	return m_pWorld->Timer.CheckTime( m_pWorld->CurrentTime, AUCTION::EXPIRE_TERM );
}









//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 새로운 경매 추가 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addNewAuction(AUCTION::sBaseInfo* auct)
{
	return m_AuctionList->add(auct);
}

//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 새로운 경매 추가 
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
//	Desc : 경매 하나 삭제 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::removeAuction( DWORD id )
{	
	m_AuctionList->remove(id);
}
















//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 입찰자를 추가한다 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addBidder(SOPlayer* pPlayer)
{

	return m_BidderList->add(pPlayer);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 입찰자를 제거한다 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::removeBidder(SOPlayer* pPlayer)
{
	// 최고 입찰자가 나갈 경우 
	if( pPlayer == m_BidderList->get( m_BidderList->getNum() - 1 ) )
	{		
		setPrevBidderToHighestBidder(); 				
	}
	
	m_BidderList->remove(pPlayer);	
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 입찰자 리스트를 초기화 한다 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::clearBidderList()
{
	m_BidderList->clear();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 입찰자가 있는가? 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::hasBidder()
{
	return m_BidderList->getNum() > 0 ? TRUE : FALSE;
}










//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 참관인을 추가한다 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::addAudience(SOPlayer* pPlayer)
{
	return m_AudienceList->add(pPlayer);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 참관인을 제거한다 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::removeAudience(SOPlayer* pPlayer)
{
	m_AudienceList->remove(pPlayer);
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 참관인 리스트를 초기화 한다 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::clearAudienceList()
{
	m_AudienceList->clear();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 참관인이 있는가? 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::hasAudience()
{
	return m_AudienceList->getNum() > 0 ? TRUE : FALSE;
}










//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 아이템 경매 추가를 DB에 설정 한다 
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
//	Desc : 아이템 경매 낙찰을 DB에 설정 한다 
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
//	Desc : 아이템 경매 유찰을 DB에 설정 한다 
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
//	Desc : 아이템 경매 삭제를 DB에 설정 한다 
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
//	Desc : 이전 입찰자를 최고 입찰자로 설정 한다 
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
//	Desc : 입찰을 시도 한다 
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
					// 현재 입찰 정보를 변경 한다 
					m_nowProgressAuction->bidderCharSlot  = bidderCharSlot;
					m_nowProgressAuction->bidderDBAccount = bidderDBAccount;
					m_nowProgressAuction->biddingPrice	  = biddingPrice;
					m_nowProgressAuction->setStrBidder(strBidder);											
					
					m_nowBiddingStartTime = timeGetTime();
					
					resetRemainTimeListPos();



					// 입찰가를 임시로 보관 해둔다 ( 최고 입찰자가 입찰을 포기할 경우를 위해 )
					pBidder->setTempBiddingPrice( biddingPrice );


					// 입찰가가 최고가인 경우 낙찰 시킨다 						
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
//	Desc : 유찰을 시도 한다 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::DoFailBid()
{
	if( m_nowProgressAuction )
	{	
		// 유찰 될 경우 최소가를 20% 감소 시킨다 		
		m_nowProgressAuction->minPrice -= (m_nowProgressAuction->minPrice * AUCTION::MIN_PRICE_DEC);


		if( ++m_nowProgressAuction->numFail < AUCTION::MAX_FAIL )
		{			
			if(!m_AuctionList->add(m_nowProgressAuction))
			{
				// 경매 제거 
				DB_delItemAuction();
			}			
			else
			{
				DB_failBidItemAuction();			
			}			
		}
		else
		{			
			// 경매 제거 
			DB_delItemAuction();
		}

		
		sendReportFailBid();											

		sendNotifyResult(getNowAuctioneer());
		

		// 경매 삭제 		
		m_nowProgressAuction = NULL;
		removeAuction(0);					
	}
}




//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 낙찰을 시도 한다 
//-------------------------------------------------------------------------------------------
bool CAuctionMarket::DoSuccessfulBid()
{

	if( m_nowProgressAuction )
	{		
		// 낙찰자
		SOPlayer* pSuccessfulBidder = m_BidderList->getLast();
		

		if( pSuccessfulBidder && pSuccessfulBidder->GetMoney() >= m_nowProgressAuction->biddingPrice ) 
		{

			switch( m_nowProgressAuction->articleKind ) 
			{

				// 아이템 일 경우 
				case AUCTION::ARTICLE_ITEM:
					{

						AUCTION::sArticleItemInfo*  orgItemInfo = (AUCTION::sArticleItemInfo*)m_nowProgressAuction->articleData;						
						AUCTION::sArticleItemInfo2  sendItemInfo;
						short						posInInventory;
						
						
						for( int slot=0; slot < ON_MAX_FOLLOWER_NUM; ++slot )
						{
							
							// 아이템을 낙찰자에게 추가 한다 
							if( pSuccessfulBidder->IFollower.CanAddItem( slot, orgItemInfo->itemID, orgItemInfo->quantity ) == TRUE )
							{
								if( pSuccessfulBidder->IFollower.AddItem( slot, orgItemInfo->itemID, orgItemInfo->quantity, &posInInventory ) ) 				
								{									
									sendItemInfo.hasFollowerSlot  = slot;							
									sendItemInfo.posInInventory   = posInInventory;
									

																	
									SOPlayer* pAuctioneer;  															
									if( pAuctioneer = getNowAuctioneer() )								
									{																		
										// 경매인 전장의 돈을 증가 시킨다 
										pAuctioneer->ModifySaving(m_nowProgressAuction->biddingPrice);
										
										// 경매의 결과를 보고해 준다 
										sendNotifyResult(pAuctioneer);
									}



									
									// 낙찰자의 돈을 감소 시킨다 							
									pSuccessfulBidder->DecreaseMoney(m_nowProgressAuction->biddingPrice, FALSE); 

									
									// 낙찰을 알린다 							
									sendReportSuccessfulBid( pSuccessfulBidder, 
															 (char*)&sendItemInfo, 
															 sizeof(sendItemInfo) );


									// DB에 낙찰을 저장 한다 
									DB_successfulBidItemAuction( orgItemInfo->itemID,
																 orgItemInfo->quantity,
																 sendItemInfo.posInInventory,
																 sendItemInfo.hasFollowerSlot );

		
									// 경매 삭제 								
									m_nowProgressAuction = NULL;
									removeAuction(0);				
									

									return TRUE;

								}
							}
						}
					}
					break;

				// 용병 일 경우 			
				/*
				case AUCTION::ARTICLE_FOLLOWER:
					{				
						
						AUCTION::sArticleFollowerInfo*   FollowerInfo = (AUCTION::sArticleFollowerInfo*)m_nowProgressAuction->articleData;
						AUCTION::sArticleFollowerInfo2   sendFollowerInfo;
						
						FollowerInfo->slot = pSuccessfulBidder->IFollower.FindEmptyFollowerSlot();

						if( FollowerInfo->slot > -1 )
						{
							// 낙찰자에게 용병을 추가해 준다 
							if( pSuccessfulBidder->IFollower.InitFollower( FollowerInfo->slot, (OnFollowerInfo*)FollowerInfo, 0, &m_pVillage->clWorld->clItem ) )
							{
								sendFollowerInfo.followerSlot = FollowerInfo->slot;


								// 경매인의 돈을 증가 시킨다 
								

								// 낙찰자의 돈을 감소 시킨다 							
								pSuccessfulBidder->DecreaseMoney( m_nowProgressAuction->biddingPrice ); 


								// 낙찰을 알린다 							
								sendReportSuccessfulBid( pSuccessfulBidder, 
														 (char*)&sendFollowerInfo,
														 sizeof(sendFollowerInfo) );

								// 경매 삭제 								
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

	// 유찰 시킨다 
	DoFailBid();

	return FALSE;
}









//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이미 등록해 놓은 경매가 있는가? 
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
//	Desc : 남은 시간 리스트의 현재 위치를 재설정 한다 
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
//	Desc : 입찰가가 유효한지를 검사 한다 
//-------------------------------------------------------------------------------------------
int CAuctionMarket::checkBiddingPrice(DWORD newBiddingPrice)
{	

	AUCTION::sBaseInfo* _p = m_nowProgressAuction;
		

	// 새로운 입찰가 0 보다 작거나 같은가?
	if( newBiddingPrice <= 0 ) {

		return AUCTION::INVALID_BIDDING_PRICE;
	}

	
	// 이전 입찰가가 이미 최고 입찰가 인가?
	if( newBiddingPrice == _p->biddingPrice ) {

		return AUCTION::INVALID_BIDDING_PRICE;
	}


	// 새로운 입찰가가 최소가와 최대가를 만족 시키는가?
	if((newBiddingPrice > _p->maxPrice && _p->maxPrice > 0) ||
	    newBiddingPrice < _p->minPrice ) {		

		return AUCTION::INVALID_BIDDING_PRICE;
	}
	
				
	// 새로운 입찰가가 이전 입찰가 보다 10% 이상 큰가?
	if( newBiddingPrice < _p->biddingPrice + (_p->biddingPrice * AUCTION::BID_PRICE_MUL) ) {
		
		return AUCTION::INVALID_BIDDING_PRICE;
	}
		
	
	// 새로운 입찰가가 최고 입찰가 인가?
	if( newBiddingPrice == _p->maxPrice && _p->maxPrice > 0 ) {
	
		return AUCTION::MAX_BIDDING_PRICE;
	}
							
	
	return AUCTION::VALID_BIDDING_PRICE;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 경매의 남은 시간을 얻어온다 
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
//	Desc : 경매의 중간에 지연 시간을 준다 
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


			// 새로운 경매의 정보를 전송 한다 
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
		

		// 경매 제한 시간이 지났을 경우 
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
//  Desc : 게임을 하고 있는 모든이에게 경매를 광고 한다 
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
//  Desc : 경매인에게 경매 결과를 알려준다 
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
//  Desc : 현재 경매의 남은 시간을 보내준다 
//-------------------------------------------------------------------------------------------
void CAuctionMarket::sendReportRemainTime()
{						
	DWORD  remainTime = getNowAuctionRemainTime();	


	// 남은 시간을 보내야 할 때인지 확인 한다 
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
//  Desc : 새로운 정보를 보내 준다 
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
//	Desc : 관람인들에게 유찰을 알린다  
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
//	Desc : 관람인들(낙찰자포함)에게 낙찰을 알린다 
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
	
		
		// 낙찰자에게는 낙찰품 정보를 건네 준다 
		m_AudienceList->write( &packet, 12, pSuccessfulBidder );
		pSuccessfulBidder->Write( &packet, sizeof(AUCTION::sReportSuccessfulBidFront) + packet.articleDataSize );		
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 관람인들에게 새로운 입찰을 알린다  
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






