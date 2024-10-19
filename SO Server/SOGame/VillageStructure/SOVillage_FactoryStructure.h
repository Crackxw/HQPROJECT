#ifndef	_VILLAGE_FACTORY_HEADER
#define	_VILLAGE_FACTORY_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"
#include "SOVillage_IndustrialComplex.h"

class	CItem;
class	SODatabase;
class	OnlineBuilding;

class	SOVillage_Factory : public SOVillage_BaseStructure
{

private:
	
	static	CItem					*m_pItem;
	static	SODatabase				*m_pDataBase;
	static	OnlineBuilding			*m_pBuilding;

	//stFarmGroup						*m_pFarmGroup;											// �� ������ �ִ� ��� ���� �ü���.
	SOVillage_IndustrialComplex		m_IndustrialComplex[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ];		// �� ������ �ִ� ��� ���� ����.
	

private:

	BOOL			SellFactoryByForce( UI08 uiFarmID, MONEY *pmnMoney );						// �ǹ��� ������ �ȷȴ�.

	DWORD			GetAllIndustrialComplexInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo, 
												UI32 *puiIndustrialComplexInfoNum );			// ��� ��� ������ ������ ���´�.
public:

	SOVillage_Factory();														// ������.
	~SOVillage_Factory();														// �Ҹ���.

	BOOL							IsValidIndustrialComplexID( SI32 siIndustrialComplexID );	// ��ȿ�� ��� ������ ID���� �˾ƿ´�.
	SOVillage_IndustrialComplex*	GetIndustrialComplex( SI32 siIndustrialComplexID );			// ��� ������ ID�� ���´�.	

	BOOL			EnterPlayer( SOPlayer *pPlayer, SI32 siIndustrialComplexID );		// �÷��̾ �� �ǹ��� ���Դ�.

	BOOL			Init( UI32 uiVillageID );											// �ʱ�ȭ.

	BOOL			SendIndustrialComplexList( SOPlayer *pPlayer );						// ��� �������� ������ �÷��̾�� �����ش�.
	BOOL			SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID );				// ��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
	BOOL			SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID );				// ��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.

	stFarmGroup*	GetFactories();														// �� ������ �ִ� ��� ���� �ü����� ���´�.
	stFarmData*		GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot );		// �÷��̾ ������ ����ǹ��� ���´�.
	
	VOID			DismantleFactoryNotWork( SOWorld *pWorld );											// �����ϰ� ���� �ʴ� �ǹ��� ���ش�.
	BOOL			BuyFactory( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, MONEY *pmnCost );		// ���� �ü��� �����Ѵ�.	

	BOOL			CanWork( UI08 uiComplexID, UI08 uiFarmID );											// ���Ϸ��� �ϴ� ���� ��ȿ�� ������ �˻��Ѵ�.	
	
	BOOL			InitIndustrialComplexFromDB( SI32 siIndustrialComplexID, 
												SI32 siOwnerDBAccount, UI08 uiOwnerDBSSlot, CHAR *pszOwnerName, 
												SI32 m_siMaxConstructibleFactoryNum, SI32 siDayNotWorked );	// DB���� ���� ������ ��� ������ �ʱ�ȭ �Ѵ�.

	BOOL			InitStorageFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.

	BOOL			InitFactoryFromDB( SI32 siIndustrialComplexID, SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],
										UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity, 
										UI08 uiEquipment0Level, UI08 uiEquipment1Level, UI08 uiEquipment2Level, UI08 uiEquipment3Level, UI08 uiEquipment4Level ); // DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.


	BOOL			SellFactory( SOPlayer *pPlayer, SI32 siFactoryID );										// ������ ó���Ѵ�.
	BOOL			WorkedInFactory( SOPlayer *pPlayer, UI32 uiAmountOfWork, MONEY *pmnPay );				// ���忡�� ���� �ߴ�.

	static	VOID	InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );	// ���� ������ �ʱ�ȭ �Ѵ�.		
};

#endif
