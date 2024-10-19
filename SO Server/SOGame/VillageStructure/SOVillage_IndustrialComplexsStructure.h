#ifndef	_VILLAGE_FACTORY_HEADER
#define	_VILLAGE_FACTORY_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"
#include "SOVillage_IndustrialComplex.h"

class	CItem;
class	SODatabase;
class	OnlineBuilding;

class	SOVillage_IndustrialComplexs : public SOVillage_BaseStructure
{

private:
	
	static	CItem					*m_pItem;
	static	SODatabase				*m_pDataBase;
	static	OnlineBuilding			*m_pBuilding;

	//stFarmGroup						*m_pFarmGroup;											// 이 마을에 있는 모든 생산 시설물.
	SOVillage_IndustrialComplex		m_IndustrialComplex[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ];		// 이 마을에 있는 모든 생산 단지.
	
	CHAR							m_cIndustrialComplexKind[ 4 ];								// 산업 단지의 종류.
	stPlants						*m_pIndustrialComplexInfo;									// 산업 단지의 정보.
private:

	BOOL			SellFactoryByForce( UI08 uiFarmID, MONEY *pmnMoney );						// 건물이 강제로 팔렸다.

	DWORD			GetAllIndustrialComplexInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo, 
												UI32 *puiIndustrialComplexInfoNum );			// 모든 산업 단지의 정보를 얻어온다.

	VOID			DestroyFactoryNotWork( SOWorld *pWorld );									// 가동하고 있지 않는 건물은 없앤다.
public:

	SOVillage_IndustrialComplexs();														// 생성자.
	~SOVillage_IndustrialComplexs();														// 소멸자.

	BOOL							IsValidIndustrialComplexID( SI32 siIndustrialComplexID );	// 유효한 산업 단지의 ID인지 알아온다.
	SOVillage_IndustrialComplex*	GetIndustrialComplex( SI32 siIndustrialComplexID );			// 산업 단지를 얻어온다.	
	SOVillage_IndustrialComplex*	GetIndustrialComplexOwned( SI32 siIndustrialComplexID );	// 플레이어가 소유하고 있는 산업 단지를 얻어온다.
	SOVillage_IndustrialComplex*	GetOwnIndustrialComplex( SOPlayer *pPlayer );				// 해당 플레이어가 소유하고 있는 산업 단지를 얻어온다.
	SOVillage_IndustrialComplex*	GetEnteredIndustrialComplex( SOPlayer *pPlayer );			// 해당 플레이어가 들어가있는 산업 단지를 얻어온다.
	SI32                            GetEnteredIndustrialComplexIndex(SOPlayer *pPlayer);


	BOOL			EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID );				// 플레이어가 이 건물에 들어왔다.
	BOOL			LeavePlayer( SOPlayer *pPlayer );											// 플레이어가 이 건물에서 나갔다.

	BOOL			Init( UI16 uiVillageCode, CHAR cIndustrialComplexKind[ 4 ] );				// 초기화.
	VOID			Free();																	// 메모리 해제.

	BOOL			SendIndustrialComplexList( SOPlayer *pPlayer );						// 산업 단지들의 정보를 플레이어에게 보내준다.
//	BOOL			SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID );				// 산업 단지에 있는 공장의 정보를 플레이어에게 보내준다.
//	BOOL			SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID );				// 산업 단지에 있는 창고의 정보를 플레이어에게 보내준다.

	stFarmGroup*	GetFactories();														// 이 마을에 있는 모든 생산 시설물을 얻어온다.
	stFarmData*		GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot );		// 플레이어가 소유한 생산건물을 얻어온다.	
	
	BOOL			BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost );		// 생산 시설을 구입한다.	

	BOOL			CanWork( SOPlayer *pPlayer );														// 일을 할 수 있는지 검사한다.
	
	BOOL			InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
												SI32 siOwnerDBAccount, UI08 uiOwnerDBSSlot, CHAR *pszOwnerName, 
												SI32 m_siMaxConstructibleFactoryNum, SI32 siDayNotWorked );	// DB에서 얻어온 정보로 산업 단지를 초기화 한다.

	BOOL			InitStorageFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB에서 얻어온 정보로 창고를 초기화 한다.

	BOOL			InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity); // DB에서 얻어온 정보로 공장을 초기화 한다.


	BOOL			SellFactory( SOPlayer *pPlayer, SI32 siFactoryID );										// 공장을 처분한다.
	
	BOOL			BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siIndustrialComplexID );					// 산업 단지를 구입한다.

	BOOL			Reclamation( SOPlayer *pPlayer, SI32 siIndustrialComplexID, MONEY *pmnCapital );									// 토지를 개척한다.
	BOOL			ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital );	// 공장을 건설한다.
	BOOL			DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );											// 공장을 파괴한다.

	BOOL			EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID );					// 산업 단지의 공장에 들어간다.
	BOOL			LeaveFactory( SOPlayer *pPlayer );										// 산업 단지의 공장에서 나간다.


	BOOL			BeginWork( SOPlayer *pPlayer );											// 일을 시작한다.
	BOOL			FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg );	// 공장에서 일을 했다.

	BOOL			ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );	// 공장의 임금을 변경한다.

	BOOL			CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );	// 현재 작업중인 공장에서 작업중인것을 취소한다.

	BOOL			UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital );		// 공장을 업그레이드 시킨다.

	DWORD			ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital );	// 아이템을 생산한다.

	BOOL			AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);							// 창고 안에 아이템을 넣는다.
	BOOL			DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);						// 창고안에서 아이템을 삭제한다.

	VOID			DayChanged( SOWorld *pWorld , UI32 uiNPCLabor);																// 하루가 지났다.

	static	VOID	InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );	// 정적 변수를 초기화 한다.		
};

#endif
