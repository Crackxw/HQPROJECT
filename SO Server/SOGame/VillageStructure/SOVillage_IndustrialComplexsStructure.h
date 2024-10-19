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

	//stFarmGroup						*m_pFarmGroup;											// �� ������ �ִ� ��� ���� �ü���.
	SOVillage_IndustrialComplex		m_IndustrialComplex[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ];		// �� ������ �ִ� ��� ���� ����.
	
	CHAR							m_cIndustrialComplexKind[ 4 ];								// ��� ������ ����.
	stPlants						*m_pIndustrialComplexInfo;									// ��� ������ ����.
private:

	BOOL			SellFactoryByForce( UI08 uiFarmID, MONEY *pmnMoney );						// �ǹ��� ������ �ȷȴ�.

	DWORD			GetAllIndustrialComplexInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo, 
												UI32 *puiIndustrialComplexInfoNum );			// ��� ��� ������ ������ ���´�.

	VOID			DestroyFactoryNotWork( SOWorld *pWorld );									// �����ϰ� ���� �ʴ� �ǹ��� ���ش�.
public:

	SOVillage_IndustrialComplexs();														// ������.
	~SOVillage_IndustrialComplexs();														// �Ҹ���.

	BOOL							IsValidIndustrialComplexID( SI32 siIndustrialComplexID );	// ��ȿ�� ��� ������ ID���� �˾ƿ´�.
	SOVillage_IndustrialComplex*	GetIndustrialComplex( SI32 siIndustrialComplexID );			// ��� ������ ���´�.	
	SOVillage_IndustrialComplex*	GetIndustrialComplexOwned( SI32 siIndustrialComplexID );	// �÷��̾ �����ϰ� �ִ� ��� ������ ���´�.
	SOVillage_IndustrialComplex*	GetOwnIndustrialComplex( SOPlayer *pPlayer );				// �ش� �÷��̾ �����ϰ� �ִ� ��� ������ ���´�.
	SOVillage_IndustrialComplex*	GetEnteredIndustrialComplex( SOPlayer *pPlayer );			// �ش� �÷��̾ ���ִ� ��� ������ ���´�.
	SI32                            GetEnteredIndustrialComplexIndex(SOPlayer *pPlayer);


	BOOL			EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID );				// �÷��̾ �� �ǹ��� ���Դ�.
	BOOL			LeavePlayer( SOPlayer *pPlayer );											// �÷��̾ �� �ǹ����� ������.

	BOOL			Init( UI16 uiVillageCode, CHAR cIndustrialComplexKind[ 4 ] );				// �ʱ�ȭ.
	VOID			Free();																	// �޸� ����.

	BOOL			SendIndustrialComplexList( SOPlayer *pPlayer );						// ��� �������� ������ �÷��̾�� �����ش�.
//	BOOL			SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID );				// ��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
//	BOOL			SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID );				// ��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.

	stFarmGroup*	GetFactories();														// �� ������ �ִ� ��� ���� �ü����� ���´�.
	stFarmData*		GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot );		// �÷��̾ ������ ����ǹ��� ���´�.	
	
	BOOL			BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost );		// ���� �ü��� �����Ѵ�.	

	BOOL			CanWork( SOPlayer *pPlayer );														// ���� �� �� �ִ��� �˻��Ѵ�.
	
	BOOL			InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
												SI32 siOwnerDBAccount, UI08 uiOwnerDBSSlot, CHAR *pszOwnerName, 
												SI32 m_siMaxConstructibleFactoryNum, SI32 siDayNotWorked );	// DB���� ���� ������ ��� ������ �ʱ�ȭ �Ѵ�.

	BOOL			InitStorageFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.

	BOOL			InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity); // DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.


	BOOL			SellFactory( SOPlayer *pPlayer, SI32 siFactoryID );										// ������ ó���Ѵ�.
	
	BOOL			BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siIndustrialComplexID );					// ��� ������ �����Ѵ�.

	BOOL			Reclamation( SOPlayer *pPlayer, SI32 siIndustrialComplexID, MONEY *pmnCapital );									// ������ ��ô�Ѵ�.
	BOOL			ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital );	// ������ �Ǽ��Ѵ�.
	BOOL			DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );											// ������ �ı��Ѵ�.

	BOOL			EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID );					// ��� ������ ���忡 ����.
	BOOL			LeaveFactory( SOPlayer *pPlayer );										// ��� ������ ���忡�� ������.


	BOOL			BeginWork( SOPlayer *pPlayer );											// ���� �����Ѵ�.
	BOOL			FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg );	// ���忡�� ���� �ߴ�.

	BOOL			ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );	// ������ �ӱ��� �����Ѵ�.

	BOOL			CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );	// ���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.

	BOOL			UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital );		// ������ ���׷��̵� ��Ų��.

	DWORD			ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital );	// �������� �����Ѵ�.

	BOOL			AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);							// â�� �ȿ� �������� �ִ´�.
	BOOL			DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);						// â��ȿ��� �������� �����Ѵ�.

	VOID			DayChanged( SOWorld *pWorld , UI32 uiNPCLabor);																// �Ϸ簡 ������.

	static	VOID	InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );	// ���� ������ �ʱ�ȭ �Ѵ�.		
};

#endif
