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
bool CAuctionList::add( char* strAuctioneer,			// 경매인 캐릭터 이름 
					    BYTE  auctioneerCharSlot,		// 경매인 캐릭터 슬롯 
						DWORD auctioneerDBAccount,		// 경매인 DBAccount
						SOnlineTime registrationDate,	// 경매 등록일 
						SOnlineTime finishedDate,		// 경매 완료일 
						char* strBidder,				// 입찰자 캐릭터 이름 
						BYTE  bidderCharSlot,			// 입찰자 캐릭터 슬롯
						DWORD bidderDBAccount,			// 입찰자 DBAccount
						DWORD biddingPrice,				// 입찰가
						WORD  numBidding,				// 입찰 참여수 						
						DWORD minPrice,					// 경매품의 최저가 
						DWORD maxPrice,					// 경매품의 최고가 
						BYTE  numFail,					// 경매의 유찰된 수 
						BYTE  articleKind,				// 경매품의 종류 
						char* articleData,				// 경매품의 정보 
						WORD  articleDataSize )			// 경매품의 정보 크기 

{
	// 최대 갯수를 넘지 않았다면 
	if( m_numAuction < MAX_AUCTION )
	{
		// 초기화 하고 
		memset( &m_AuctionList[ m_numAuction ], 0, sizeof( AUCTION::sBaseInfo ) );

		
		// 값을 셋팅한다 
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

		// 데이타를 설정 하는데 성공 하였을 경우 
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
		// 값을 복사 하고 
		memcpy( &m_AuctionList[m_numAuction], auct, sizeof( AUCTION::sBaseInfo ) );
		
		// 유니크 아이디만 재설정 해준다 
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
//	Desc : 현재 경매의 갯수를 얻어온다 
//--------------------------------------------------------------------------------------------
int CAuctionList::getNum()
{
	return m_numAuction;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 새로운 유니크 ID를 얻어온다 ('1' 부터 시작 한다)
//--------------------------------------------------------------------------------------------
DWORD CAuctionList::getUniqID()
{
	return m_uniqIDSeed >= 50000 ? m_uniqIDSeed = 1 : ++m_uniqIDSeed;
}