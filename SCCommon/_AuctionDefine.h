#ifndef __AUCTION_DEFINE__
#define	__AUCTION_DEFINE__



namespace AUCTION {

	// �Ҵ���� command ( 9500 ~ 9600 ) 
	enum
	{
		REQUEST_SELL_ITEM			= 9500 ,			// ������ ��Ÿ� ��û �Ѵ� 
		REQUEST_SELL_FOLLOWER			   ,			// �뺴	  ��Ÿ� ��û �Ѵ� 								 
		REQUEST_BIDDING					   ,			// ������ ��û �Ѵ� 
		REQUEST_NUMWAIT					   ,			// ��� ������ ��û�Ѵ� 
		REQUEST_ENTER					   ,			// ��ſ� ������ ��û�Ѵ� 
		REQUEST_LEAVE					   ,			// ��ſ� ������ ��û�Ѵ� 
		
		RESPONSE_SELL_ARTICLE			   ,			// ���(�뺴,������) ��û�� ���� �Ѵ� 				
		RESPONSE_BIDDING				   ,			// ���� ��û�� �����Ѵ� 
	    RESPONSE_NUMWAIT				   ,			// ��� ���� ��û�� �����Ѵ� 
		RESPONSE_ENTER					   ,			// ��� ����   ��û�� �����Ѵ�		

		REPORT_SUCCESSFUL_BID			   ,			// ��ſ� ������ ĳ���͵鿡�� �����Ǿ����� �˸��� 
		REPORT_FAIL_BID					   ,			// ��ſ� ������ ĳ���͵鿡�� �����Ǿ����� �˸��� 
		REPORT_BIDDING					   ,			// ��ſ� ������ ĳ���͵鿡�� �����Ǿ����� �˸��� 
		REPORT_NEW_INFO				   	   ,			// ��Ÿ� �����ϰ� �ִ� ĳ���͵鿡�� ���ο� ��� ������ ������ 
		REPORT_REMAIN_TIME				   ,			// ��Ÿ� �����ϰ� �ִ� ĳ���͵鿡�� ���� �ð��� �˸��� 		
	
		NOTIFY_RESULT					   ,			// ����ο��� ��� ����� �˸���
		NOTIFY_ADVERTISE								// ��Ÿ� ����� �˸���  										
	};

	// ���ǰ ���� 
	enum 
	{
		ARTICLE_ITEM				   = 1 ,			// ������ 
		ARTICLE_FOLLOWER				   ,			// �뺴 
		ARTICLE_PRODUCTION				   ,			// ����� 
	};

	// �������� ���� ���� 
	enum 
	{			
		UNKNOWN_FAIL			       = -99 ,			// �˼����� ���� 
		OK							   = 1   ,			// ���� 		
	};

	// ��� ��û�� ���� ���� ���� 
	enum 
	{
		ALREADY_ADD					   = -98 ,			// �̹� ��û�� ��Ű� �ִ� 
		INVALID_ARTICLE						 ,			// ��ȿ���� ���� ���ǰ�̴� 
		OVER_WAITING_MAX					 ,			// ��� �ϰ� �ִ� ��Ű� �ʹ� ���� ���ο� ��Ÿ� ������ ���� 
		NOT_ENOUGHT_FEE									// �����ᰡ �����Ͽ� ��Ÿ� �߰� �Ҽ� ���� 
	};

	// ��� ���� ��û�� ���� ���� ���� 
	enum
	{
		OVER_AUDIENCE_MAX			   = -98 ,			// �������� �ʹ� ���� ���� �Ҽ� ����
		NOT_HAS											// ������ ���� ������ ���� �������� ��Ű� ���� 
														// (���߿� ��Ű� �߰� �Ǹ� �ڵ����� REPORT_NEW_INFO�� ���� �ش�)
	};

	// ���� ��û�� ���� ���� ���� 
	enum
	{
		OVER_BIDDER_MAX				   = -98 ,			// �����ڰ� �ʹ� ���� ���� �Ҽ� ����
	};

	// ������ �˻翡 ���� ���� ���� 
	enum 
	{		
		INVALID_BIDDING_PRICE		   = -1	 ,			// �ùٸ��� ���� �������� ��û�ߴ� 
		VALID_BIDDING_PRICE				     ,			// ������ ������ �������� ��û�ߴ� 
		MAX_BIDDING_PRICE								// �ִ�			 �������� ��û�ߴ� 
	};




	const int	 MAX_WAITING		   = 300;			// ������� ����� �ִ�� (300��)	
	const int	 EXPIRE_TERM		   = 1;				// ����� �Ϸ� ����       (1��:���ӽð� (= LIMIT_TIME) ) 
	const int	 MAX_AUDIENCE		   = 300;			// �ִ� ������ ��         (300��)
	const int	 MAX_BIDDER			   = 200;			// �ִ� ������ ��		  (200��)	
	const double BID_PRICE_MUL		   = 0.1;			// �������� ������        (���� ������ 10 %)
	
	const int	 READY_TIME			   = 10000;			// ��� �غ� �ð�		  (10��)
	const int	 MAX_FAIL			   = 5;				// �ִ� ���� Ƚ��		  (5ȸ)


	const int	 BID_LIMIT_TIME        = 30000;			// ���� ���� �ð�		  (30��) 
	const int	 LIMIT_TIME			   = 150000;		// ��� ���� �ð�		  (2��30��) 
	const int	 PREVIEW_TIME		   = 30000;			// ��� �ÿ� �ð�		  (30��)

	const int	 MIN_FEE			   = 1000;			// �ּ� ������			  (1000��)	
	const double FEE_RATE			   = 0.1;			// ������ ����			  (�ּҰ��� 10 %)
	const int    FEE_RATE_INT          = 10;				

	const double MIN_PRICE_DEC		   = 0.2;			// �ּҰ��� ���ҷ�		  (���� �ּҰ��� 20 %)


	const DWORD  REMAIN_TIME_LIST[]	   =				// Ŭ���̾�Ʈ�� ������ �ϴ� ���� �ð��� ����Ʈ
	{ 30100, 10100, 5100, 0 };							// 30��, 10��, 5�� 
	






//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ���ǰ(������)�� ���� 
//-------------------------------------------------------------------------------------------
struct sArticleItemInfo								// *��� ������ ���* 
{
	WORD		itemID;								// ������ ID 
	WORD		quantity;							// ������ ���� 

};

struct sArticleItemInfo2							// *������ �߰�(����)�� �������� ���޽� ���*						
{
	BYTE		hasFollowerSlot;					// �� �������� ���� �뺴 ���� 
																			
	WORD		posInInventory;						// �κ��丮���� ��ġ 	
};									



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ���ǰ(�뺴)�� ���� 
//-------------------------------------------------------------------------------------------
struct 	sArticleFollowerInfoFront					// *memcpy�� �պκ��� ������ ����Ҷ� ���*
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

struct sArticleFollowerInfo							// *��� ������ ����ϴ� �뺴����*
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
		
		// ������ ���⸦ ���´� 
		pFollower->GetWearItems(wearItem);				
		memcpy(name, pFollower->Name, sizeof(name));

		// �������� ���´� 
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

struct sArticleFollowerInfo2					// *�뺴�߰�(����)�� �������� ���޽� ���*
{
	BYTE	followerSlot;						// �߰��� �뺴�� ���� 
};





//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : Ŭ���̾�Ʈ�� ���� ��������� �������� �κ�  
//-------------------------------------------------------------------------------------------
struct sCommonInfo
{
	DWORD		uniqID;							// ����� ����ũ ���̵�
	
	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];				
												// ����� ĳ���� �̸� 

	SOnlineTime	finishedDate;					// ��� �Ϸ� �� 
			
	DWORD		minPrice;						// �ּ� ���� 
	DWORD		maxPrice;						// �ִ� ���� 
	 	
	BYTE		numFail;						// ������ Ƚ��

	BYTE		articleKind;					// ���ǰ ����	
	WORD		articleDataSize;	
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : �������� ���̴� ����� ��� ���� 
//-------------------------------------------------------------------------------------------
struct sBaseInfoFront							// *memcpy�� �պκ��� ������ ����Ҷ� ���*
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
	DWORD		uniqID;							// ����� ����ũ ���̵� 	
	
	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];				
												// ����� ĳ���� �̸� 
	BYTE		auctioneerCharSlot;				// ����� ĳ���� ���� 	
	DWORD		auctioneerDBAccount;			// ����� DBAccount 	
		
	SOnlineTime	registrationDate;				// ��� ��� �� 
	SOnlineTime	finishedDate;					// ��� �Ϸ� �� 
	
	char		strBidder[LOGIN::NAME_MAX_LENGTH];	
												// ������ ĳ���� �̸� 	
	BYTE		bidderCharSlot;					// ������ ĳ���� ����
	DWORD		bidderDBAccount;				// �������� DBAccount 
	DWORD		biddingPrice;					// ������ 
	WORD		numBidding;						// ������ Ƚ�� 
		
	DWORD		minPrice;						// �ּ� ���� 
	DWORD		maxPrice;						// �ִ� ���� 
	 	
	BYTE		numFail;						// ������ Ƚ�� 

	BYTE		articleKind;					// ���ǰ ���� 		
	WORD		articleDataSize;		
	char		articleData[3000];


	// ���ǰ�� ����Ÿ  ���� 	
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
	
	
	// ����� �̸� ���� 	
	void		setStrAuctioneer(char* name)
	{
		if( name )
		{
			memset( strAuctioneer, 0, sizeof(strAuctioneer) );
			memcpy( strAuctioneer, name, strlen(name) );
		}
	}
	

	// ������ �̸� ���� 	
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
//	Desc : ���� ��� ������ ��û�Ѵ� 
//-------------------------------------------------------------------------------------------
struct sRequestNumWait
{
	DWORD		command;
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ��� ���� ��û�� �����Ѵ� 
//-------------------------------------------------------------------------------------------
struct sResponseNumWait
{
	DWORD		command;

	int			numWait;					// ��� ����  ( '-1' �� ��� ��� ��û�� �Ҽ� ���� )
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ������ ��� ��û 
//-------------------------------------------------------------------------------------------
struct sRequestSellItem
{
	DWORD	   command;
	
	DWORD	   minPrice;					// �ּҰ� 
	DWORD	   maxPrice;					// �ִ밡 


	WORD	   article_itemID;				// ���ǰ_������ ID 	
	WORD	   article_quantity;			// ���ǰ_���� 
	BYTE	   atricle_hasFollowerSlot;		// ���ǰ_�������� ���� �뺴�� ����
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : �뺴 ��� ��û 
//-------------------------------------------------------------------------------------------
struct sRequestSellFollower
{
	DWORD		command;
		
	DWORD	    minPrice;					// �ּҰ� 
	DWORD	    maxPrice;					// �ִ밡 

	BYTE		article_followerSlot;		// ���ǰ_�뺴 ���� 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ��� ��û�� ���� ���� 
//-------------------------------------------------------------------------------------------
struct sResponseSellArticle
{
	DWORD		command;

	int			responseKind;																																														
};							  



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ��ſ� �����Ͽ� ������ ��û �ϴ� ����ü 
//-------------------------------------------------------------------------------------------
struct sRequestBidding
{
	DWORD		command;
	
	DWORD		auctionUniqID;				// ����� ����ũ ���̵� 						
	DWORD		biddingPrice;				// ���� ���� 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ������ �����ϴ� ����ü 
//-------------------------------------------------------------------------------------------
struct sResponseBidding
{
	DWORD		command;
		
	int			responseKind;															
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ��ż� ������ ��û �Ѵ� 
//-------------------------------------------------------------------------------------------
struct sRequestEnter
{
	DWORD		command;
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ��ż� ���忡 ���� �Ѵ� 
//-------------------------------------------------------------------------------------------
struct sResponseEnterFront					// *memcpy�� �պκ��� ������ ����Ҷ� ���*
{
	DWORD			command;

	int				responseKind;					

	// ���� �������� ����� ���� 
	//------------------------------			
	char			strBidder[LOGIN::NAME_MAX_LENGTH];					
	DWORD			biddingPrice;					

	sCommonInfo		_c;	
};

struct sResponseEnter
{
	DWORD			command;

	int				responseKind;					

	// ���� �������� ����� ���� 
	//------------------------------	
	char			strBidder[LOGIN::NAME_MAX_LENGTH];			
	DWORD			biddingPrice;	

	sCommonInfo		_c;

	char			articleData[3000];


	// ���ǰ�� ����Ÿ  ���� 	
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
	
	
	// ����� �̸� ���� 	
	void		setStrAuctioneer(char* name)
	{
		if( name )
		{
			memset( _c.strAuctioneer, 0, sizeof(_c.strAuctioneer) );
			memcpy( _c.strAuctioneer, name, LOGIN::NAME_MAX_LENGTH );
		}
	}
	

	// ������ �̸� ���� 	
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
//	Desc : ��żҿ��� ������ ��û�Ѵ� 
//-------------------------------------------------------------------------------------------
struct sRequestLeave
{
	DWORD		command;
};









//-------------------------------------------------------------------------------------------
//  Name :
//	Desc :  ��ſ� ������ ����鿡�� ������ �˸��� 
//-------------------------------------------------------------------------------------------
struct sReportBidding
{
	DWORD	    command;

	DWORD		uniqID;						// ������ ����� ����ũ ���̵� 
	DWORD		biddingPrice;				// ������ 
	char		strBidder[LOGIN::NAME_MAX_LENGTH];				
											// ������ ĳ���� �̸� 	
	

	// ������ �̸� ���� 	
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
//	Desc : ���� �ϰ� �ִ� ����鿡�� ������ �˸��� 
//-------------------------------------------------------------------------------------------
struct sReportFailBid
{
	DWORD		command;

	DWORD		uniqID;						// ������ ����� ����ũ ���̵� 
};



//-------------------------------------------------------------------------------------------
//  Name :
//	Desc : ���� �ϰ� �ִ� ����鿡�� ������ �˸��� 
//-------------------------------------------------------------------------------------------
struct sReportSuccessfulBidFront			// *memcpy�� �պκ��� ������ ����Ҷ� ���*
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
	
	DWORD		uniqID;						// ������ ����� ����ũ ���̵� 
	DWORD		biddingPrice;				// ������ 

	BYTE		articleKind;							
	WORD		articleDataSize;						
	char		articleData[3000];						


	// ���ǰ�� ����Ÿ  ���� 	
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
//	Desc : ���ο� ��� ������ �˸��� 
//-------------------------------------------------------------------------------------------
struct sReportNewInfoFront					// *memcpy�� �պκ��� ������ ����Ҷ� ���*
{
	DWORD		command;
				
	// ���ο� ����� ���� 
	//------------------------------
	sCommonInfo _c;		
};

struct sReportNewInfo
{
	DWORD		command;

	// ���ο� ����� ���� 
	//------------------------------
	sCommonInfo _c;

	char		articleData[3000];
	

	// ���ǰ�� ����Ÿ  ���� 	
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
		
	// ����� �̸� ���� 	
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
//	Desc : ���� ����� �����ð��� �˸��� 
//-------------------------------------------------------------------------------------------
struct sReportRemainTime
{
	DWORD	 command;
	
	DWORD	 remainTime;					// ���� �ð� 
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

	// ���ǰ�� ����Ÿ  ���� 	
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
//	Desc : ����ο��� ��� ����� �˸��� 
//-------------------------------------------------------------------------------------------
struct sNotifyResultFront					// *memcpy�� �պκ��� ������ ����Ҷ� ���*
{
	DWORD		command;
	
	DWORD		biddingPrice;				

	BYTE		articleKind;							
	WORD		articleDataSize;						
};

struct sNotifyResult			
{
	DWORD		command;
	
	DWORD		biddingPrice;				// ������ (0�� ��� ����)

	BYTE		articleKind;							
	WORD		articleDataSize;						
	char		articleData[3000];						


	// ���ǰ�� ����Ÿ  ���� 	
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
