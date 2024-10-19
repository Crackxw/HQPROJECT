#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "SOVillage.h"
#include "AuctionList.h"
#include "AuctionMarket.h"
#include "AuctionMarketProc.h"



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-------------------------------------------------------------------------------------------
CAuctionMarketProc::CAuctionMarketProc(SOPlayer* owner): m_pPlayer(owner)
{
	
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-------------------------------------------------------------------------------------------
CAuctionMarketProc::~CAuctionMarketProc()
{
	
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �÷��̾��� ���ڿ� ���̵�(�̸�)�� ���ؿ´� 
//-------------------------------------------------------------------------------------------
char* CAuctionMarketProc::getStrID()
{	
	return m_pPlayer->szID; 
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �÷��̾��� ĳ���� ������ ���ؿ´� 
//-------------------------------------------------------------------------------------------
BYTE CAuctionMarketProc::getCharSlot()
{
	return m_pPlayer->stAccountInfo.uiSlot;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �÷��̾��� DBAccount�� ���ؿ´� 
//-------------------------------------------------------------------------------------------
DWORD CAuctionMarketProc::getDBAccount()
{	
	return m_pPlayer->stAccountInfo.siDBAccount;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �÷��̾ ��ġ�� ������ ��żҸ� ��´� 
//-------------------------------------------------------------------------------------------
CAuctionMarket* CAuctionMarketProc::getAuctionMarket()
{		
	return m_pPlayer->getAuctionMarket();
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ڷ� �־��� �뺴 ������ ĳ���Ͱ� �ùٸ���?
//-------------------------------------------------------------------------------------------
bool CAuctionMarketProc::isValidFollower(int followerSlot)
{
	if( m_pPlayer->IFollower.IsValidFollower(followerSlot) ) return TRUE;	

	return FALSE;
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �÷��̾��� �� ĳ����(�뺴)�� ���ڷ� �־��� �������� ������ŭ ���ϰ� �ִ°�?
//-------------------------------------------------------------------------------------------
bool CAuctionMarketProc::isValidItem(int followerSlot, int itemID, int itemQuantity)
{	
	if(m_pPlayer->GetItem(followerSlot, itemID))
	{
		if(m_pPlayer->GetItem(followerSlot, itemID)->uiQuantity >= itemQuantity) return TRUE;
	}

	return FALSE;
}













//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��� ������ ��û �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::recvRequestNumWait( char* data )
{
	AUCTION::sRequestNumWait*	packet = (AUCTION::sRequestNumWait*)data;

	if( getAuctionMarket() )
	{
		sendResponseNumWait( getAuctionMarket()->getNumWait() );
		return;
	}
	
	sendResponseNumWait( -1 );
}
//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��� ���� ��û�� �����Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::sendResponseNumWait( int numWait )
{
	AUCTION::sResponseNumWait packet;
	memset(&packet, 0, sizeof(packet));

	packet.command = AUCTION::RESPONSE_NUMWAIT;
	packet.numWait = numWait;


	m_pPlayer->Write( &packet, sizeof(packet) );
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��� �ִ� �ּҰ��� �˻��ϰ� �ùٸ� ��� �� �����Ḧ ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
DWORD CAuctionMarketProc::checkMinMaxPrice( DWORD minPrice, DWORD maxPrice )
{
	if( minPrice < ON_MAX_MONEY && maxPrice < ON_MAX_MONEY )
	{
		DWORD fee = (minPrice * AUCTION::FEE_RATE);

		fee = ((fee <= AUCTION::MIN_FEE) ? AUCTION::MIN_FEE : fee);

		if( m_pPlayer->GetMoney() >= fee) return fee;
	}

	return 0;
}


			
//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �뺴 ��Ÿ� ��û �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::recvRequestSellFollower( char* data )
{
	AUCTION::sRequestSellFollower* packet = (AUCTION::sRequestSellFollower*)data;


	if( getAuctionMarket() )
	{	
		
		// �̹� ����� ���� ��Ű� �ִ°�?
		if( !getAuctionMarket()->isAlreadyAddAuction(getDBAccount(),getCharSlot()) )
		{
			
			if( isValidFollower( packet->article_followerSlot ) )
			{
				// �뺴 ����Ÿ 
				AUCTION::sArticleFollowerInfo articleData;
				int articleDataSize = articleData.set( m_pPlayer->IFollower.GetFollower( packet->article_followerSlot ) );

				// ��� �߰� 
				if( getAuctionMarket()->addNewAuction( getStrID()     ,
													   getCharSlot()  ,
													   getDBAccount() ,
													   SOnlineTime()  ,
													   SOnlineTime()  ,
													   NULL ,
													   0	,
													   0	,
													   0	,
													   0	,
													   packet->minPrice ,
													   packet->maxPrice ,
													   0    ,
													   AUCTION::ARTICLE_FOLLOWER ,
													   (char*)&articleData ,
													   articleDataSize ) )
				{
					// �뺴 ���� 
					m_pPlayer->DeleteFollower( packet->article_followerSlot );				

					sendResponseSellArticle( AUCTION::OK );
					return;
				}
				
				else
				{ sendResponseSellArticle( AUCTION::OVER_WAITING_MAX ); return; }
			}
			
			else
			{ sendResponseSellArticle( AUCTION::INVALID_ARTICLE ); return; }
		}	
		
		else
		{ sendResponseSellArticle( AUCTION::ALREADY_ADD ); return; }
	}
	

	sendResponseSellArticle( AUCTION::UNKNOWN_FAIL );
}
//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��Ÿ� ��û �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::recvRequestSellItem( char* data )
{
	AUCTION::sRequestSellItem* packet = (AUCTION::sRequestSellItem*)data;


	if( getAuctionMarket() )
	{		
		
		// �̹� ����� ���� ��Ű� �ִ°�?
		if( !getAuctionMarket()->isAlreadyAddAuction(getDBAccount(),getCharSlot()) )
		{
					
			DWORD fee = checkMinMaxPrice( packet->minPrice, packet->maxPrice );

			if( fee > 0 )
			{
				m_pPlayer->DecreaseMoney( fee, FALSE );

				if( isValidItem( packet->atricle_hasFollowerSlot, packet->article_itemID, packet->article_quantity ) )
				{
					// ������ ����Ÿ 
					AUCTION::sArticleItemInfo articleData;				
					articleData.itemID   = packet->article_itemID;
					articleData.quantity = packet->article_quantity;

					
										
					// ��� �߰� 
					if( getAuctionMarket()->addNewAuction( getStrID() ,
														   getCharSlot(),
														   getDBAccount() ,
														   SOnlineTime() ,
														   SOnlineTime() ,
														   NULL ,
														   0	,
														   0	,
														   0	,
														   0	,
														   packet->minPrice ,
														   packet->maxPrice	,
														   0	,
														   AUCTION::ARTICLE_ITEM ,
														   (char*)&articleData ,
														   sizeof(articleData) ) )
					{

						// DB�� ���� 					
						getAuctionMarket()->DB_addItemAuction( getStrID(), getCharSlot(), getDBAccount(), 
															   packet->minPrice, packet->maxPrice, 0, AUCTION::ARTICLE_ITEM, 
															   packet->article_itemID , 
															   packet->article_quantity , 
															   packet->atricle_hasFollowerSlot, 
															   fee );

						// ������ ���� 
						m_pPlayer->IFollower.DelItem( packet->atricle_hasFollowerSlot, packet->article_itemID, packet->article_quantity );				
						
						sendResponseSellArticle( AUCTION::OK );
						return;
					}

					else
					{ sendResponseSellArticle( AUCTION::OVER_WAITING_MAX ); return; }
				}

				else
				{ sendResponseSellArticle( AUCTION::INVALID_ARTICLE ); return; }
			}
			else
			{ sendResponseSellArticle( AUCTION::NOT_ENOUGHT_FEE ); return; } 
		}	
		
		else
		{ sendResponseSellArticle( AUCTION::ALREADY_ADD ); return; }
	}


	sendResponseSellArticle( AUCTION::UNKNOWN_FAIL );
}
//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��� ��û�� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::sendResponseSellArticle( int which )
{						 
	AUCTION::sResponseSellArticle packet;
	memset(&packet, 0, sizeof(packet));
		 	
	packet.command		= AUCTION::RESPONSE_SELL_ARTICLE;
	packet.responseKind = which;


	m_pPlayer->Write( &packet, sizeof(packet) );
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��ż� ������ ��û�Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::recvRequestEnter( char* data )
{
	AUCTION::sRequestEnter* packet = (AUCTION::sRequestEnter*)data;

	if( getAuctionMarket() )
	{
		// �������� �߰��ϴµ� ���� �ϰ� 
		if( getAuctionMarket()->addAudience( m_pPlayer ) )
		{

			// ���� �������� ��Ű� ���� ���  
			if( getAuctionMarket()->getNowProgressAuction() )	
			{
				sendResponseEnter( AUCTION::OK );
			}			
			else
			{
				sendResponseEnter( AUCTION::NOT_HAS );							
			}
		}		
		else 
		{ 
			sendResponseEnter( AUCTION::OVER_AUDIENCE_MAX ); 			
		}

		return; 
	}

	sendResponseEnter( AUCTION::UNKNOWN_FAIL );
}
//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��ż� ���� ��û�� �����Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::sendResponseEnter( int which )
{
	AUCTION::sResponseEnter packet;
	memset(&packet, 0, sizeof(packet));

	packet.command		= AUCTION::RESPONSE_ENTER;
	packet.responseKind = which;
	
	
	if( AUCTION::OK == packet.responseKind ) 
	{				
		// ���� �������� ��Ÿ� ��´� 
		AUCTION::sBaseInfo* pInfo = getAuctionMarket()->getNowProgressAuction();

		packet._c.uniqID		= pInfo->uniqID;		
		packet._c.numFail		= pInfo->numFail;		
		packet._c.finishedDate	= pInfo->finishedDate;							
		packet._c.minPrice		= pInfo->minPrice;
		packet._c.maxPrice		= pInfo->maxPrice;

		packet.biddingPrice		= pInfo->biddingPrice;			

		packet._c.articleKind	= pInfo->articleKind;
		packet.setData( pInfo->articleData, pInfo->articleDataSize );

		packet.setStrAuctioneer( pInfo->strAuctioneer );
		packet.setStrBidder( pInfo->strBidder );	
	
		m_pPlayer->Write( &packet, sizeof(AUCTION::sResponseEnterFront) + packet._c.articleDataSize );		
	}
	else
	{
		m_pPlayer->Write( &packet, 8 );
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��ż� ���� ���⸦ ��û �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::recvRequestLeave( char* data )
{
	AUCTION::sRequestLeave* packet = (AUCTION::sRequestLeave*)data;

	if( getAuctionMarket() )
	{
		getAuctionMarket()->removeBidder( m_pPlayer );
		getAuctionMarket()->removeAudience( m_pPlayer );		
	}
}



//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ��û �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::recvRequestBidding( char* data )
{
	AUCTION::sRequestBidding* packet = (AUCTION::sRequestBidding*)data;

	if( getAuctionMarket() )
	{
		
		switch( getAuctionMarket()->DoBidding( m_pPlayer, getStrID(), getCharSlot(), getDBAccount(), 
											   packet->auctionUniqID, packet->biddingPrice ) )
		{		
			case 1:
				sendResponseBidding( AUCTION::OK );
				return;				

			case 0:
				sendResponseBidding( AUCTION::OVER_BIDDER_MAX );
				return;

			default:
				break;
		}

	}

	sendResponseBidding( AUCTION::UNKNOWN_FAIL );
}
//-------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ��û�� ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
void CAuctionMarketProc::sendResponseBidding( int which )
{
	AUCTION::sResponseBidding packet;
	memset(&packet, 0, sizeof(packet));

	packet.command		= AUCTION::RESPONSE_BIDDING;
	packet.responseKind = which;

	m_pPlayer->Write( &packet, sizeof(packet) );
}