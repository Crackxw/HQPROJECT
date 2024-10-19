#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "SOVillage.h"
#include "AuctionList.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CAuctionList::CAuctionList(DWORD maxAuction): MAX_AUCTION( maxAuction )
{					
	m_AuctionList = new AUCTION::sBaseInfo[ MAX_AUCTION ];
	
	m_numAuction  = 0;
	m_uniqIDSeed  = 0;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CAuctionList::~CAuctionList()
{
	if( m_AuctionList != NULL ) delete m_AuctionList;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
AUCTION::sBaseInfo* CAuctionList::get( DWORD index )
{
	if( index >= 0 && index < m_numAuction ) return &m_AuctionList[ index ];

	return NULL;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
bool CAuctionList::add( char* strAuctioneer,			// ����� ĳ���� �̸� 
					    BYTE  auctioneerCharSlot,		// ����� ĳ���� ���� 
						DWORD auctioneerDBAccount,		// ����� DBAccount
						SOnlineTime registrationDate,	// ��� ����� 
						SOnlineTime finishedDate,		// ��� �Ϸ��� 
						char* strBidder,				// ������ ĳ���� �̸� 
						BYTE  bidderCharSlot,			// ������ ĳ���� ����
						DWORD bidderDBAccount,			// ������ DBAccount
						DWORD biddingPrice,				// ������
						WORD  numBidding,				// ���� ������ 						
						DWORD minPrice,					// ���ǰ�� ������ 
						DWORD maxPrice,					// ���ǰ�� �ְ� 
						BYTE  numFail,					// ����� ������ �� 
						BYTE  articleKind,				// ���ǰ�� ���� 
						char* articleData,				// ���ǰ�� ���� 
						WORD  articleDataSize )			// ���ǰ�� ���� ũ�� 

{
	// �ִ� ������ ���� �ʾҴٸ� 
	if( m_numAuction < MAX_AUCTION )
	{
		// �ʱ�ȭ �ϰ� 
		memset( &m_AuctionList[ m_numAuction ], 0, sizeof( AUCTION::sBaseInfo ) );

		
		// ���� �����Ѵ� 
		m_AuctionList[m_numAuction].uniqID					= getUniqID();
		m_AuctionList[m_numAuction].auctioneerCharSlot      = auctioneerCharSlot;
		m_AuctionList[m_numAuction].auctioneerDBAccount		= auctioneerDBAccount;
		m_AuctionList[m_numAuction].registrationDate		= registrationDate;
		m_AuctionList[m_numAuction].finishedDate			= finishedDate;
		m_AuctionList[m_numAuction].bidderDBAccount			= bidderDBAccount;
		m_AuctionList[m_numAuction].bidderCharSlot          = bidderCharSlot;
		m_AuctionList[m_numAuction].biddingPrice			= biddingPrice;
		m_AuctionList[m_numAuction].numBidding				= numBidding;
		m_AuctionList[m_numAuction].numFail					= numFail;
		m_AuctionList[m_numAuction].minPrice				= minPrice;		
		m_AuctionList[m_numAuction].maxPrice				= maxPrice;		
		m_AuctionList[m_numAuction].articleKind				= articleKind;
		
		m_AuctionList[m_numAuction].setStrAuctioneer( strAuctioneer );
		m_AuctionList[m_numAuction].setStrBidder( strBidder );

		// ����Ÿ�� ���� �ϴµ� ���� �Ͽ��� ��� 
		if( m_AuctionList[m_numAuction].setData( articleData, articleDataSize ) )
		{		
			++m_numAuction;
			return TRUE;
		}
	}
	
	return FALSE;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
bool CAuctionList::add( AUCTION::sBaseInfo *auct )
{
	if( m_numAuction < MAX_AUCTION && auct != NULL )
	{		
		// ���� ���� �ϰ� 
		memcpy( &m_AuctionList[m_numAuction], auct, sizeof( AUCTION::sBaseInfo ) );
		
		// ����ũ ���̵� �缳�� ���ش� 
		m_AuctionList[m_numAuction].uniqID= getUniqID();		

		++m_numAuction;

		return TRUE;
	}

	return FALSE;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
void CAuctionList::remove( DWORD index )
{	
	if( index >= 0 && index < m_numAuction )
	{			
		int size = sizeof( AUCTION::sBaseInfo ) * (m_numAuction - index - 1);

		if( size > 0 )
		memcpy( &m_AuctionList[index], &m_AuctionList[index+1], size );
				 
		--m_numAuction;
	}
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ����� ������ ���´� 
//--------------------------------------------------------------------------------------------
int CAuctionList::getNum()
{
	return m_numAuction;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ο� ����ũ ID�� ���´� ('1' ���� ���� �Ѵ�)
//--------------------------------------------------------------------------------------------
DWORD CAuctionList::getUniqID()
{
	return m_uniqIDSeed >= 50000 ? m_uniqIDSeed = 1 : ++m_uniqIDSeed;
}