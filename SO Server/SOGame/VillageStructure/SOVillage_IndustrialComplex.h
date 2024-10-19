#ifndef	_VILLAGE_FACTORY_INDUSTRIAL_COMPLEX_HEADER
#define	_VILLAGE_FACTORY_INDUSTRIAL_COMPLEX_HEADER

#include "GSCDefine.h"
#include "SOCommonHeader.h"
#include "PlayersManager.h"
#include "SOVillage_IndustrialComplex_Factory.h"

class	SOPlayer;

class	SOVillage_IndustrialComplex
{
private:	
	
	static	CItem					*m_pItem;
	static	SODatabase				*m_pDataBase;
	static	OnlineBuilding			*m_pBuilding;

	UI16					m_uiVillageCode;						// 마을의 ID
	SI32					m_siIndustrialComplexID;				// 산업 단지의 ID

	SI32					m_siOwnerDBAccount;						// 산업 단지의 주인 DBAccount
	UI08					m_uiOwnerDBSlot;						// 산업 단지의 주인 DBSlot	
	CHAR					m_szOwnerName[ ON_ID_LENGTH + 1 ];		// 산업 단지의 주인의 이름.
	
	SI32					m_siMaxConstructibleFactoryNum;			// 최대 건설할 수 있는 공장의 수를 얻어온다.
	SI32					m_siDayNotWorked;						// 작업을 하지 않은 날 수.


	PlayersManager			m_PlayerMgr;					// 산업 단지의 들어와있는 플레이어들.
	//OnFarmList				m_FarmInfo;					// 전송을 위한 구조체.(uiAvailNum,szFarmName,szOwner,Labor)
	//stFarmGroup				*m_pGroup;		
	
	SOVillage_IndustrialComplex_Factory		m_Factory[ ON_MAX_FACTORY_NUM ];		// 공장 수.
	
private:	

	SOVillage_IndustrialComplex_Factory*	GetFactoryPlayerIn( SOPlayer *pPlayer );		// 플레이어가 들어가 있는 공장을 얻어온다.

	BOOL			IsMaterialItemEnoughToProduce( UI16 uiItemID, UI32 uiItemQuantity );	// 아이템을 생산하기 위하여 창고에 충분한 아이템 재료가 있는지 검사한다.
	BOOL			IsMaterialToolEnoughToProduce( UI16 uiItemID );							// 아이템을 생산하기 위하여 창고에 충분한 아이템 도구가 있는지 검사한다.

	BOOL			DeleteMaterialItem( UI16 uiItemID, UI32 uiItemQuantity );				// 이 아이템을 생산하기 위하여 필요한 재료를 삭제한다.
	BOOL			DestroyToolItemRandomly( UI16 uiItemID );								// 이 아이템을 생산하기 위하여 필요한 도구를 랜덤하게 파괴 시킨다.
	

public:
	SOVillage_IndustrialComplex();									// 생성자.
	~SOVillage_IndustrialComplex();									// 소멸자.	

	BOOL					IsValidFactoryID( SI32 siFactoryID );	// 유효한 공장 ID 인가?
	
	VOID										Init(SI32 siIndustrialComplexID, UI16 uiVillageCode);			// 초기화.	
	VOID										InitIndustrialComplex();										// 산업 단지가 팔렸을 시에 해준다.
	VOID										Free();															// 메모리 해제.
	SOVillage_IndustrialComplex_Factory*		GetFactory( SI32 siFactoryID );					// 공장을 얻어온다.
	SOVillage_IndustrialComplex_Factory*		GetFactoryConstructed( SI32 siFactoryID );		// 공장이 건설된 공장 객체를 얻어온다.
	SOVillage_IndustrialComplex_Factory*		GetEnteredFactory( SOPlayer *pPlayer );		// 플레이어가 들어간 공장을 얻어온다.
	SI32                                        GetEnteredFactoryIndex( SOPlayer *pPlayer );

	VOID					InitFromDB( SI32 siOwnerDBAccount, UI08 uiOwnerDBSlot, CHAR *pszOwnerName, 
									SI32 siMaxConstructibleFactoryNum, SI32 siDayNotWorked );	// DB에서 얻어온 정보로 산업 단지를 초기화 한다.

	BOOL					InitStorageFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB에서 얻어온 정보로 창고를 초기화 한다.

	BOOL					InitFactoryFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],	// DB에서 얻어온 정보로 공장을 초기화 한다.
									UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity); 



	BOOL					EnterPlayer( SOPlayer *pPlayer );									// 플레이어가 들어왔다.
	BOOL					LeavePlayer( SOPlayer *pPlayer );									// 플레이어가 나갔다.

	DWORD					GetInfo( OnFarmSlotList* pFactoryInfo, SI32 *psiFactoryNum );		// 산업 단지에 있는 공장들의 정보를 얻어온다.
	DWORD					GetInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo );			// 산업 단지의 정보를 얻어온다.

	BOOL					HasOwner();															// 이 산업단지의 주인이 있는가.
	BOOL					GetOwner( SI32 *psiOwnerDBAccount, UI08 *puiOwnerDBSlot );			// 이 산업단지의 주인을 얻어온다.
	BOOL					IsOwner( SOPlayer *pPlayer );										// 이 산업단지의 주인인지 검사한다.
	BOOL                    ChangeOwner(SOPlayer *pPlayer);                                     // 주인을 다른사람으로 바꾼다.

	BOOL					SellFactory( SOPlayer *pPlayer, SI32 siFactoryID );					// 공장을 처분한다.
	
	BOOL					IsPlayerIn( SOPlayer *pPlayer );											// 플레이어가 이 산업 단지안에 들어와 있는지 검사한다.	

//	BOOL					SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID );						// 산업 단지에 있는 공장의 정보를 플레이어에게 보내준다.
//	BOOL					SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID );						// 산업 단지에 있는 창고의 정보를 플레이어에게 보내준다.	
	

	BOOL					BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siMaxConstructibleFactoryNum);	// 산업 단지를 구입한다.	

	BOOL					Reclamation( SOPlayer *pPlayer, SI32 siMaxFactoryNum );																// 토지를 개척한다.
	BOOL					ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital );	// 공장을 건설한다.
	BOOL					DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );											// 공장을 파괴한다.

	BOOL					CanWork( SOPlayer *pPlayer );													// 공장에서 일을 할 수 있는지 검사한다.
	BOOL					CanWork( SOVillage_IndustrialComplex_Factory *pFactory );						// 공장에서 일을 할 수 있는지 검사한다.

	BOOL					EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID );							// 산업 단지의 공장에 들어간다.
	BOOL					LeaveFactory( SOPlayer *pPlayer );												// 산업 단지의 공장에서 나간다.
	
	BOOL					BeginWork( SOPlayer *pPlayer );													// 일을 시작한다.
	BOOL					FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg );			// 일을 끝마쳤다.

	BOOL					ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );			// 공장의 임금을 변경한다.
	
	BOOL					CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );			// 현재 작업중인 공장에서 작업중인것을 취소한다.
	
	BOOL					UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital );	// 공장을 업그레이드 시킨다.

	DWORD					ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital );	// 아이템을 생산한다.

	BOOL					AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE);				// 창고 안에 아이템을 넣는다.



	BOOL					DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );			// 창고안에서 아이템을 삭제한다.

	BOOL					AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);								// 아무 창고 안에 아이템을 넣는다.
	

	BOOL					CanAddItem( UI16 uiItemID ,UI16 uiItemQuantity,SI32 *StorageID,SI32 * StoragePos);													// 창고 안고 안에 아이템을 넣을 수 있는지 검사한다.	

	VOID					DestroyIndustrialComplexNotWork( MONEY *pmnIndustrialComplexPrice, MONEY mnIndustrialComplexCost);	// 가동중이지 않는 산업 단지는 없앤다.

	VOID					DayChanged( SOPlayer *pPlayer, MONEY mnIndustrialComplexCost );										// 하루가 지났다.

	VOID                    DoNPCWork(UI32 uiNPCWork);

	static	VOID			InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );				// 정적 변수를 초기화 한다.		

	UI32                    GetVillageCode() { return m_uiVillageCode;};
	                          
	VOID                    GetTradeInfo(OnIndustrialComplexTradeInfo *pTradeInfo);
/*
	stFarmData()
	{
		UserManager.Create(ON_MAX_PLANT_PLAYER_NUM);
	}
	
	VOID Init()
	{
		siOwnerDBAcnt		=	0;
		uiOwnerSlot			=	0;
		siWorkDay			=	0;
		FarmInfo.Init();
		FarmInfo.uiAvailSlot=	pPlant->uiInitSlot;
		UserManager.DeleteAll();
		for(int i=0; i<ON_FARM_SLOT_NUM; i++)
		{
			FarmSlot[i].Init();
			FarmSlot[i].nSlotIndex	=	i;
			FarmSlot[i].pFarm		=	this;
		}
	}
	
	~stFarmData()
	{
		UserManager.Release();
	}
		
	stFarmSlot	*GetFactory( SI32 siFactoryIndex )
	{
		if( siFactoryIndex < FarmInfo.uiAvailSlot && siFactoryIndex >= 0)
			return &FarmSlot[ siFactoryIndex ];
		
		return NULL;
	}
*/
};

#endif