#ifndef __AUCTION_DEFINE__
#define	__AUCTION_DEFINE__



namespace AUCTION {

	// 할당받은 command ( 9500 ~ 9600 ) 
	enum
	{
		REQUEST_SELL_ITEM			= 9500 ,			// 아이템 경매를 요청 한다 
		REQUEST_SELL_FOLLOWER			   ,			// 용병	  경매를 요청 한다 								 
		REQUEST_BIDDING					   ,			// 입찰을 요청 한다 
		REQUEST_NUMWAIT					   ,			// 경매 대기수를 요청한다 
		REQUEST_ENTER					   ,			// 경매에 입장을 요청한다 
		REQUEST_LEAVE					   ,			// 경매에 퇴장을 요청한다 
		
		RESPONSE_SELL_ARTICLE			   ,			// 경매(용병,아이템) 요청에 응답 한다 				
		RESPONSE_BIDDING				   ,			// 입찰 요청에 응답한다 
	    RESPONSE_NUMWAIT				   ,			// 경매 대기수 요청에 응답한다 
		RESPONSE_ENTER					   ,			// 경매 입장   요청에 응답한다		

		REPORT_SUCCESSFUL_BID			   ,			// 경매에 참여한 캐릭터들에게 낙찰되었음을 알린다 
		REPORT_FAIL_BID					   ,			// 경매에 참여한 캐릭터들에게 유찰되었음을 알린다 
		REPORT_BIDDING					   ,			// 경매에 참여한 캐릭터들에게 입찰되었음을 알린다 
		REPORT_NEW_INFO				   	   ,			// 경매를 관람하고 있는 캐릭터들에게 새로운 경매 정보를 보낸다 
		REPORT_REMAIN_TIME				   ,			// 경매를 관람하고 있는 캐릭터들에게 남은 시간을 알린다 		
	
		NOTIFY_RESULT					   ,			// 경매인에게 경매 결과를 알린다
		NOTIFY_ADVERTISE								// 경매를 광고로 알린다  										
	};

	// 경매품 종류 
	enum 
	{
		ARTICLE_ITEM				   = 1 ,			// 아이템 
		ARTICLE_FOLLOWER				   ,			// 용병 
		ARTICLE_PRODUCTION				   ,			// 생산소 
	};

	// 공통적인 응답 종류 
	enum 
	{			
		UNKNOWN_FAIL			       = -99 ,			// 알수없는 실패 
		OK							   = 1   ,			// 성공 		
	};

	// 경매 요청에 대한 응답 종류 
	enum 
	{
		ALREADY_ADD					   = -98 ,			// 이미 요청된 경매가 있다 
		INVALID_ARTICLE						 ,			// 유효하지 않은 경매품이다 
		OVER_WAITING_MAX					 ,			// 대기 하고 있는 경매가 너무 많아 새로운 경매를 받을수 없다 
		NOT_ENOUGHT_FEE									// 수수료가 부족하여 경매를 추가 할수 없다 
	};

	// 경매 관람 요청에 대한 응답 종류 
	enum
	{
		OVER_AUDIENCE_MAX			   = -98 ,			// 관람인이 너무 많아 관람 할수 없다
		NOT_HAS											// 관람에 성공 했지만 현재 진행중인 경매가 없다 
														// (나중에 경매가 추가 되면 자동으로 REPORT_NEW_INFO를 보내 준다)
	};

	// 입찰 요청에 대한 응답 종류 
	enum
	{
		OVER_BIDDER_MAX				   = -98 ,			// 입찰자가 너무 많아 입찰 할수 없다
	};

	// 입찰가 검사에 대한 응답 종류 
	enum 
	{		
		INVALID_BIDDING_PRICE		   = -1	 ,			// 올바르지 못한 입찰가를 요청했다 
		VALID_BIDDING_PRICE				     ,			// 입찰이 가능한 입찰가를 요청했다 
		MAX_BIDDING_PRICE								// 최대			 입찰가를 요청했다 
	};




	const int	 MAX_WAITING		   = 300;			// 대기중인 경매의 최대수 (300개)	
	const int	 EXPIRE_TERM		   = 1;				// 경매의 완료 기한       (1일:게임시간 (= LIMIT_TIME) ) 
	const int	 MAX_AUDIENCE		   = 300;			// 최대 관람객 수         (300명)
	const int	 MAX_BIDDER			   = 200;			// 최대 입찰자 수		  (200명)	
	const double BID_PRICE_MUL		   = 0.1;			// 입찰가의 증가량        (이전 가격의 10 %)
	
	const int	 READY_TIME			   = 10000;			// 경매 준비 시간		  (10초)
	const int	 MAX_FAIL			   = 5;				// 최대 유찰 횟수		  (5회)


	const int	 BID_LIMIT_TIME        = 30000;			// 입찰 제한 시간		  (30초) 
	const int	 LIMIT_TIME			   = 150000;		// 경매 제한 시간		  (2분30초) 
	const int	 PREVIEW_TIME		   = 30000;			// 경매 시연 시간		  (30초)

	const int	 MIN_FEE			   = 1000;			// 최소 수수료			  (1000원)	
	const double FEE_RATE			   = 0.1;			// 수수료 비율			  (최소가의 10 %)
	const int    FEE_RATE_INT          = 10;				

	const double MIN_PRICE_DEC		   = 0.2;			// 최소가의 감소량		  (이전 최소가의 20 %)


	const DWORD  REMAIN_TIME_LIST[]	   =				// 클라이언트에 보내야 하는 남은 시간의 리스트
	{ 30100, 10100, 5100, 0 };							// 30초, 10초, 5초 
	






//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매품(아이템)의 정보 
//-------------------------------------------------------------------------------------------
struct sArticleItemInfo								// *경매 보관에 사용* 
{
	WORD		itemID;								// 아이템 ID 
	WORD		quantity;							// 아이템 수량 

};

struct sArticleItemInfo2							// *아이템 추가(낙찰)후 유저에게 전달시 사용*						
{
	BYTE		hasFollowerSlot;					// 이 아이템을 지닌 용병 슬롯 
																			
	WORD		posInInventory;						// 인벤토리내의 위치 	
};									



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매품(용병)의 정보 
//-------------------------------------------------------------------------------------------
struct 	sArticleFollowerInfoFront					// *memcpy시 앞부분의 사이즈 계산할때 사용*
{
	UI08		slot;							
	UI16		kind;	
	UI16		numItem;
	
	SI32		Exp;						
	SI32		Level;						
	UI16		Str;						
	UI16		Dex;						
	UI16		Vit;						
	UI16		Int;						
	
	SI32		Life;						
	SI32		Mana;					
	SI16		Bonus;					

	SI32		liftConstant;				

	SI16		equipMinDamage;			
	SI16		equipMaxDamage;			

	SI16		AC;						
	SI16		damageRegist;				
	SI16		magicRegist;			
	
	UI16		wearItem[WEAR_ITEM_NUM];
	CHAR		name[ON_ID_LENGTH];	

}; 

struct sArticleFollowerInfo							// *경매 보관시 사용하는 용병정보*
{
	UI08		slot;							
	UI16		kind;	
	UI16		numItem;
	
	SI32		Exp;						
	SI32		Level;						
	UI16		Str;						
	UI16		Dex;						
	UI16		Vit;						
	UI16		Int;						
	
	SI32		Life;						
	SI32		Mana;					
	SI16		Bonus;					

	SI32		liftConstant;				

	SI16		equipMinDamage;			
	SI16		equipMaxDamage;			

	SI16		AC;						
	SI16		damageRegist;				
	SI16		magicRegist;			
	
	UI16		wearItem[WEAR_ITEM_NUM];
	CHAR		name[ON_ID_LENGTH];				

	MyItemData	hasItem[ON_MYDATA_MAX_ITEM_NUM];
			

	int			set(OnlineFollower* pFollower)
	{				
		memset(this, 0, sizeof(sArticleFollowerInfo));
		
		slot		   = -1;		
		kind		   = pFollower->uiCharID;	
		
		Exp			   = pFollower->ParametaBox.IP.GetCurExp();
		Level		   = pFollower->ParametaBox.IP.GetLevel();
		Str		 	   = pFollower->ParametaBox.IP.GetStr();
		Dex			   = pFollower->ParametaBox.IP.GetDex();
		Vit			   = pFollower->ParametaBox.IP.GetVit();
		Int			   = pFollower->ParametaBox.IP.GetInt();

		Life		   = pFollower->ParametaBox.IP.GetLife();
		Mana		   = pFollower->ParametaBox.IP.GetMana();
		Bonus		   = pFollower->ParametaBox.IP.GetBonus();

		liftConstant   = pFollower->ParametaBox.GetLiftPower();

		equipMinDamage = pFollower->ParametaBox.IP.EquipMinDamage;
		equipMaxDamage = pFollower->ParametaBox.IP.EquipMaxDamage;

		AC			   = pFollower->ParametaBox.IP.GetAC();
		damageRegist   = pFollower->ParametaBox.IP.DamageResist;
		magicRegist	   = pFollower->ParametaBox.IP.MagicResist;
		
		// 장착한 무기를 얻어온다 
		pFollower->GetWearItems(wearItem);				
		memcpy(name, pFollower->Name, sizeof(name));

		// 아이템을 얻어온다 
		for(int i=0; i < ON_MYDATA_MAX_ITEM_NUM; ++i)
		{
			if( pFollower->Items[i].uiID != 0 )
			{
				memcpy(&hasItem[numItem++], &pFollower->Items[i], sizeof(MyItemData));				
			}
		}	
	
		return sizeof(sArticleFollowerInfoFront) + (numItem * sizeof(MyItemData));		
	}
	
};

struct sArticleFollowerInfo2					// *용병추가(낙찰)후 유저에게 전달시 사용*
{
	BYTE	followerSlot;						// 추가된 용병의 슬롯 
};





//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 클라이언트로 보낼 경매정보의 공통적인 부분  
//-------------------------------------------------------------------------------------------
struct sCommonInfo
{
	DWORD		uniqID;							// 경매의 유니크 아이디
	
	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];				
												// 경매인 캐릭터 이름 

	SOnlineTime	finishedDate;					// 경매 완료 일 
			
	DWORD		minPrice;						// 최소 가격 
	DWORD		maxPrice;						// 최대 가격 
	 	
	BYTE		numFail;						// 유찰된 횟수

	BYTE		articleKind;					// 경매품 종류	
	WORD		articleDataSize;	
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 서버에서 쓰이는 경매의 기반 정보 
//-------------------------------------------------------------------------------------------
struct sBaseInfoFront							// *memcpy시 앞부분의 사이즈 계산할때 사용*
{
	DWORD		uniqID;								

	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];	
	BYTE		auctioneerCharSlot;						
	DWORD		auctioneerDBAccount;			
	
	SOnlineTime	registrationDate;				
	SOnlineTime	finishedDate;					
	
	char		strBidder[LOGIN::NAME_MAX_LENGTH];	
	BYTE		bidderCharSlot;						
	DWORD		bidderDBAccount;				
	DWORD		biddingPrice;					
	WORD		numBidding;						
		
	DWORD		minPrice;				
	DWORD	    maxPrice;					
	
	BYTE		numFail;

	BYTE		articleKind;							
	WORD		articleDataSize;
};

struct sBaseInfo								
{
	DWORD		uniqID;							// 경매의 유니크 아이디 	
	
	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];				
												// 경매인 캐릭터 이름 
	BYTE		auctioneerCharSlot;				// 경매인 캐릭터 슬롯 	
	DWORD		auctioneerDBAccount;			// 경매인 DBAccount 	
		
	SOnlineTime	registrationDate;				// 경매 등록 일 
	SOnlineTime	finishedDate;					// 경매 완료 일 
	
	char		strBidder[LOGIN::NAME_MAX_LENGTH];	
												// 입찰자 캐릭터 이름 	
	BYTE		bidderCharSlot;					// 입찰자 캐릭터 슬롯
	DWORD		bidderDBAccount;				// 입찰자의 DBAccount 
	DWORD		biddingPrice;					// 입찰가 
	WORD		numBidding;						// 입찰된 횟수 
		
	DWORD		minPrice;						// 최소 가격 
	DWORD		maxPrice;						// 최대 가격 
	 	
	BYTE		numFail;						// 유찰된 횟수 

	BYTE		articleKind;					// 경매품 종류 		
	WORD		articleDataSize;		
	char		articleData[3000];


	// 경매품의 데이타  설정 	
	bool		setData(char* data, WORD size)
	{
		if( data && size < 3000 )
		{												
			memset(	articleData, 0 , sizeof(articleData) );
			memcpy( articleData, data , size );
			
			articleDataSize = size;

			return TRUE;
		}		

		return FALSE;
	}
	
	
	// 경매인 이름 설정 	
	void		setStrAuctioneer(char* name)
	{
		if( name )
		{
			memset( strAuctioneer, 0, sizeof(strAuctioneer) );
			memcpy( strAuctioneer, name, strlen(name) );
		}
	}
	

	// 입찰자 이름 설정 	
	void		setStrBidder(char* name)
	{
		if( name )
		{
			memset( strBidder, 0, sizeof(strBidder) );
			memcpy( strBidder, name, strlen(name) );
		}
	}
};





//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 현재 경매 대기수를 요청한다 
//-------------------------------------------------------------------------------------------
struct sRequestNumWait
{
	DWORD		command;
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매 대기수 요청에 응답한다 
//-------------------------------------------------------------------------------------------
struct sResponseNumWait
{
	DWORD		command;

	int			numWait;					// 경매 대기수  ( '-1' 일 경우 경매 신청을 할수 없다 )
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 아이템 경매 요청 
//-------------------------------------------------------------------------------------------
struct sRequestSellItem
{
	DWORD	   command;
	
	DWORD	   minPrice;					// 최소가 
	DWORD	   maxPrice;					// 최대가 


	WORD	   article_itemID;				// 경매품_아이템 ID 	
	WORD	   article_quantity;			// 경매품_수량 
	BYTE	   atricle_hasFollowerSlot;		// 경매품_아이템을 지닌 용병의 슬롯
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 용병 경매 요청 
//-------------------------------------------------------------------------------------------
struct sRequestSellFollower
{
	DWORD		command;
		
	DWORD	    minPrice;					// 최소가 
	DWORD	    maxPrice;					// 최대가 

	BYTE		article_followerSlot;		// 경매품_용병 슬롯 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매 요청에 대한 응답 
//-------------------------------------------------------------------------------------------
struct sResponseSellArticle
{
	DWORD		command;

	int			responseKind;																																														
};							  



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매에 참가하여 입찰을 요청 하는 구조체 
//-------------------------------------------------------------------------------------------
struct sRequestBidding
{
	DWORD		command;
	
	DWORD		auctionUniqID;				// 경매의 유니크 아이디 						
	DWORD		biddingPrice;				// 입찰 가격 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 입찰에 응답하는 구조체 
//-------------------------------------------------------------------------------------------
struct sResponseBidding
{
	DWORD		command;
		
	int			responseKind;															
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매소 입장을 요청 한다 
//-------------------------------------------------------------------------------------------
struct sRequestEnter
{
	DWORD		command;
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매소 입장에 응답 한다 
//-------------------------------------------------------------------------------------------
struct sResponseEnterFront					// *memcpy시 앞부분의 사이즈 계산할때 사용*
{
	DWORD			command;

	int				responseKind;					

	// 현재 진행중인 경매의 정보 
	//------------------------------			
	char			strBidder[LOGIN::NAME_MAX_LENGTH];					
	DWORD			biddingPrice;					

	sCommonInfo		_c;	
};

struct sResponseEnter
{
	DWORD			command;

	int				responseKind;					

	// 현재 진행중인 경매의 정보 
	//------------------------------	
	char			strBidder[LOGIN::NAME_MAX_LENGTH];			
	DWORD			biddingPrice;	

	sCommonInfo		_c;

	char			articleData[3000];


	// 경매품의 데이타  설정 	
	bool		setData(char* data, WORD size)
	{
		if( data && size < 3000 )
		{												
			memset(	articleData, 0 , sizeof(articleData) );
			memcpy( articleData, data , size );
			
			_c.articleDataSize = size;

			return TRUE;
		}		

		return FALSE;
	}
	
	
	// 경매인 이름 설정 	
	void		setStrAuctioneer(char* name)
	{
		if( name )
		{
			memset( _c.strAuctioneer, 0, sizeof(_c.strAuctioneer) );
			memcpy( _c.strAuctioneer, name, LOGIN::NAME_MAX_LENGTH );
		}
	}
	

	// 입찰자 이름 설정 	
	void		setStrBidder(char* name)
	{
		if( name )
		{
			memset( strBidder, 0, sizeof(strBidder) );
			memcpy( strBidder, name, LOGIN::NAME_MAX_LENGTH );
		}
	}
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매소에서 퇴장을 요청한다 
//-------------------------------------------------------------------------------------------
struct sRequestLeave
{
	DWORD		command;
};









//-------------------------------------------------------------------------------------------
//  Name :
//	Desc :  경매에 참여한 사람들에게 입찰을 알린다 
//-------------------------------------------------------------------------------------------
struct sReportBidding
{
	DWORD	    command;

	DWORD		uniqID;						// 입찰된 경매의 유니크 아이디 
	DWORD		biddingPrice;				// 입찰가 
	char		strBidder[LOGIN::NAME_MAX_LENGTH];				
											// 입찰자 캐릭터 이름 	
	

	// 입찰자 이름 설정 	
	void		setStrBidder(char* name)
	{
		if( name )
		{
			memset( strBidder, 0, sizeof(strBidder) );
			memcpy( strBidder, name, strlen(name) );
		}
	}
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 관람 하고 있는 사람들에게 유찰을 알린다 
//-------------------------------------------------------------------------------------------
struct sReportFailBid
{
	DWORD		command;

	DWORD		uniqID;						// 유찰된 경매의 유니크 아이디 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 관람 하고 있는 사람들에게 낙찰을 알린다 
//-------------------------------------------------------------------------------------------
struct sReportSuccessfulBidFront			// *memcpy시 앞부분의 사이즈 계산할때 사용*
{
	DWORD		command;

	DWORD		uniqID;	
	DWORD		biddingPrice;				

	BYTE		articleKind;							
	WORD		articleDataSize;						
};

struct sReportSuccessfulBid 
{
	DWORD		command;
	
	DWORD		uniqID;						// 낙찰된 경매의 유니크 아이디 
	DWORD		biddingPrice;				// 낙찰가 

	BYTE		articleKind;							
	WORD		articleDataSize;						
	char		articleData[3000];						


	// 경매품의 데이타  설정 	
	bool		setData(char* data, WORD size)
	{		
		if( data && size < 3000 )
		{												
			memset(	articleData, 0 , sizeof(articleData) );
			memcpy( articleData, data , size );
			
			articleDataSize = size;

			return TRUE;
		}

		return FALSE;
	}
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 새로운 경매 정보를 알린다 
//-------------------------------------------------------------------------------------------
struct sReportNewInfoFront					// *memcpy시 앞부분의 사이즈 계산할때 사용*
{
	DWORD		command;
				
	// 새로운 경매의 정보 
	//------------------------------
	sCommonInfo _c;		
};

struct sReportNewInfo
{
	DWORD		command;

	// 새로운 경매의 정보 
	//------------------------------
	sCommonInfo _c;

	char		articleData[3000];
	

	// 경매품의 데이타  설정 	
	bool		setData(char* data, WORD size)
	{
		if( data && size < 3000 )
		{												
			memset( articleData, 0 , sizeof(articleData) );
			memcpy( articleData, data , size );
			
			_c.articleDataSize = size;

			return TRUE;
		}		

		return FALSE;
	}
		
	// 경매인 이름 설정 	
	void		setStrAuctioneer(char* name)
	{
		if( name )
		{
			memset( _c.strAuctioneer, 0, sizeof(_c.strAuctioneer) );
			memcpy( _c.strAuctioneer, name, strlen(name) );
		}
	}
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 현재 경매의 남은시간을 알린다 
//-------------------------------------------------------------------------------------------
struct sReportRemainTime
{
	DWORD	 command;
	
	DWORD	 remainTime;					// 남은 시간 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 
//-------------------------------------------------------------------------------------------
struct sNotifyAdvertiseFront 
{
	DWORD		command;

	BYTE		articleKind;
	WORD		articleDataSize;
};

struct sNotifyAdvertise
{
	DWORD		command;

	BYTE		articleKind;
	WORD		articleDataSize;
	char		articleData[3000];

	// 경매품의 데이타  설정 	
	bool		setData(char* data, WORD size)
	{		
		if( data && size < 3000 )
		{												
			memset(	articleData, 0 , sizeof(articleData) );
			memcpy( articleData, data , size );
			
			articleDataSize = size;

			return TRUE;
		}		

		return FALSE;
	}
};





//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : 경매인에게 경매 결과를 알린다 
//-------------------------------------------------------------------------------------------
struct sNotifyResultFront					// *memcpy시 앞부분의 사이즈 계산할때 사용*
{
	DWORD		command;
	
	DWORD		biddingPrice;				

	BYTE		articleKind;							
	WORD		articleDataSize;						
};

struct sNotifyResult			
{
	DWORD		command;
	
	DWORD		biddingPrice;				// 낙찰가 (0일 경우 유찰)

	BYTE		articleKind;							
	WORD		articleDataSize;						
	char		articleData[3000];						


	// 경매품의 데이타  설정 	
	bool		setData(char* data, WORD size)
	{		
		if( data && size < 3000 )
		{												
			memset(	articleData, 0 , sizeof(articleData) );
			memcpy( articleData, data , size );
			
			articleDataSize = size;

			return TRUE;
		}		

		return FALSE;
	}
};


static void printfMsg( DWORD msg )
{
	using namespace AUCTION;
	#define Case(a) case(a) : printf( "[" #a "]" "has been sent.\n" ); break;

	switch(msg)
	{
		Case(REQUEST_SELL_ITEM)		
		Case(REQUEST_SELL_FOLLOWER)
		Case(REQUEST_BIDDING)
		Case(REQUEST_NUMWAIT)
		Case(REQUEST_ENTER)	
		Case(REQUEST_LEAVE)
		Case(RESPONSE_SELL_ARTICLE)
		Case(RESPONSE_BIDDING)
	    Case(RESPONSE_NUMWAIT)
		Case(RESPONSE_ENTER)
		Case(REPORT_SUCCESSFUL_BID)
		Case(REPORT_FAIL_BID)
		Case(REPORT_BIDDING)
		Case(REPORT_NEW_INFO)
		Case(REPORT_REMAIN_TIME)
		Case(NOTIFY_RESULT)
		Case(NOTIFY_ADVERTISE)
	}
}

};


#endif
