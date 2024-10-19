#ifndef	_VILLAGE_FACTORY_INDUSTRIAL_COMPLEX_FACTORY_HEADER
#define	_VILLAGE_FACTORY_INDUSTRIAL_COMPLEX_FACTORY_HEADER

#include "GSCDefine.h"
#include "SOCommonHeader.h"
#include "PlayersManager.h"

#define	ON_MAX_FACTORY_PLAYER_NUM		100							// 한 공장에 들어갈 수 있는 최대 플레이어 수.

class	CItem;
class	SODatabase;
class	OnlineBuilding;
class	SOPlayer;
class   SOVillage_IndustrialComplex;

class	SOVillage_IndustrialComplex_Factory
{
private:

	static	CItem					*m_pItem;
	static	OnlineBuilding			*m_pBuilding;				
	static	SODatabase				*m_pDataBase;

	UI16				m_uiVillageCode;							// 이 공장이 있는 마을의 ID
	SI32				m_siIndustrialComplexID;				// 이 공장이 있는 산업단지의 ID
	SI32				m_siFactoryID;							// 공장의 ID.	
	
	UI08				m_uiLevel;								// 현재의 레벨
	CHAR				m_cFactoryKind[ 4 ];					// 공장의 종류.

	MONEY				m_mnPay;								// 현 공장의 일할때의 임금.
	//UI32				m_uiAmountOfWork;						// 작업량.
	UI32				m_uiRemainAmountOfWork;					// 남은 작업량.
	
	UI16				m_uiProducingItemID;					// 생산중인 아이템.
	
	UI32				m_uiMaxProduceItemQuantity;				// 생산해야 하는 아이템의 수량.

	BOOL				m_bStore;								// 이 공장이 창고인가.
		
	PlayersManager		m_PlayerMgr;							// 공장의 들어와 있는 플레이어들.
	PlayersManager		m_WorkingPlayerMgr;						// 실질적으로 현재 일을 하고 있는 플레이어들.

	STORAGE_ITEM		m_StorageItem[ ON_MAX_STORAGEITEM ];	// 창고일 경우에 창고 안에 있는 아이템.


private:
	VOID				LevelUp( SOPlayer *pPlayerCompletedWork );							// 레벨을 증가시킨다.

	BOOL				SendFactoryInfo( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner );	// 산업 단지에 있는 공장의 정보를 플레이어에게 보내준다.

	BOOL				IsUpgradeCompleted();					// 업그레이드가 완료되었는지 검사한다.	

	UI32				GetAmountOfWorkByQuantity( UI16 uiItemID, UI32 uiItemQuantity );		// 아이템 개수를 노동량으로 바꾼다.
	UI32				GetItemQuantityByAmountOfWork( UI16 uiItemID, UI32 *puiAmountOfWork );	// 노동량을 아이템의 개수로 바꾼다.

public:
	STORAGE_ITEM*		GetItem( UI16 uiItemID, SI32 *psiIndex );		// 창고에 있는 아이템을 얻어온다.
private:

	STORAGE_ITEM*		GetEmptyItem( SI32 *psiIndex );					// 창고에 비어있는 아이템을 얻어온다.
	
	VOID				SetItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity );	// 창고에 있는 아이템의 정보를 DB에 저장한다.
	VOID				DelItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity );	// 창고에 있는 아이템의 정보를 DB에 저장한다.
	VOID				WorkCompleted( SOPlayer *pPlayerCompletedWork );							// 현재의 작업이 완료되었다.


public:
	SOVillage_IndustrialComplex_Factory();				// 생성자.
	~SOVillage_IndustrialComplex_Factory();				// 소멸자.
		
	VOID	Init( UI16 uiVillageCode, SI32 siIndustrialComplexID, SI32 siFactoryID );				// 초기화.
	VOID	InitFactory();																		// 공장을 초기화 한다.
	VOID	Free();																				// 해제.
	VOID	SetStatus( FactoryStatus Status );					// 공장의 상태를 설정한다.

	VOID	InitWork();											// 작업을 초기화 한다

	BOOL	DidConstructFactory();								// 공장이 건설되었는지 검사한다.

	DWORD	GetInfo( OnFarmSlotList* pFactoryInfo );			// 공장의 정보를 얻어온다.

	BOOL	EnterPlayer( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner );					// 플레이어가 들어왔다.
	BOOL	LeavePlayer( SOPlayer *pPlayer );												// 플레이어가 나갔다.

	BOOL	InitStorageFromDB( UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
						UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
						UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
						UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
						UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB에서 얻어온 정보로 창고를 초기화 한다.

	BOOL	InitFactoryFromDB( UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],				// DB에서 얻어온 정보로 공장을 초기화 한다.
						UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity); 


	BOOL	SellFactory( SOPlayer *pPlayer );												// 공장을 처분한다.

	BOOL	IsPlayerIn( SOPlayer *pPlayer );												// 플레이어가 이 공장안에 들어와 있는지 검사한다.	


	
//	BOOL	SendStorageInfo( SOPlayer *pPlayer );				// 산업 단지에 있는 창고의 정보를 플레이어에게 보내준다.

	BOOL	CanWork();											// 이 공장에서 일을 할 수 있는지 알아온다. 

	BOOL	IsStorage();										// 이 공장이 창고인지 알아온다.
	BOOL	IsFactory();										// 이 공장이 일반적인 공장인지 알아온다.

	UI32	GetAmountOfWork();									// 작업량을 얻어온다.
	

	BOOL	ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnTotalPay );		// 공장을 건설한다.	
	VOID	DestroyFactory( SOPlayer *pPlayer, MONEY *pmnCapital );																	// 공장을 파괴한다.

	BOOL	BeginWork( SOPlayer *pPlayer );																			// 일을 시작한다.
	BOOL	FinishWork( SOPlayer *pPlayer, SOVillage_IndustrialComplex *pComplex, OnReqFarmWorkMsg *pMsg );									// 일을 끝마쳤다.
	BOOL	FinishWorkByNPC( SOVillage_IndustrialComplex *pComplex,UI32 uiAmountOfWork);									// 일을 끝마쳤다.

	BOOL	ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );									// 공장의 임금을 변경한다.

	BOOL	CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );									// 현재 작업중인 공장에서 작업중인것을 취소한다.

	BOOL	IsConstructing();																// 건물이 건설중인지 알아온다.
	BOOL	IsUpgrading();																	// 공장을 업그레이드중인지 알아온다.

	BOOL	UpgradeFactory( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );			// 공장을 업그레이드 시킨다.

	DWORD	ProduceItem( SOPlayer *pPlayer, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital );		// 아이템을 생산한다.

	BOOL	HasItem( UI16 uiItemID, UI16 uiItemQuantity );									// 창고 안에 충분한 아이템이 있는지 검사한다.	

	BOOL	AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);									// 창고 안에 아이템을 넣는다.
	BOOL	DeleteItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);								// 창고안에서 아이템을 삭제한다.	

	BOOL	AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);				// 창고 안에 아이템을 넣는다.
	BOOL	DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);			// 창고안에서 아이템을 삭제한다.	

	
	BOOL	CanAddItem( UI16 uiItemID ,UI16 uiQuantity,SI32 *pStoragePos);													// 창고 안에 아이템을 넣을 수 있는지 검사한다.

	UI16	GetProducingItemID();															// 공장에서 생산중인 아이템의 ID를 얻어온다.
	UI32    GetProducingItemNumber() {return m_uiMaxProduceItemQuantity;};
	MONEY   GetPay() {return m_mnPay;};
	const   CHAR	*GetFactoryKind() {return m_cFactoryKind;};


	static	VOID	InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );	// 정적 변수를 초기화 한다.		
};

#endif
