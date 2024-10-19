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

	UI16					m_uiVillageCode;						// ������ ID
	SI32					m_siIndustrialComplexID;				// ��� ������ ID

	SI32					m_siOwnerDBAccount;						// ��� ������ ���� DBAccount
	UI08					m_uiOwnerDBSlot;						// ��� ������ ���� DBSlot	
	CHAR					m_szOwnerName[ ON_ID_LENGTH + 1 ];		// ��� ������ ������ �̸�.
	
	SI32					m_siMaxConstructibleFactoryNum;			// �ִ� �Ǽ��� �� �ִ� ������ ���� ���´�.
	SI32					m_siDayNotWorked;						// �۾��� ���� ���� �� ��.


	PlayersManager			m_PlayerMgr;					// ��� ������ �����ִ� �÷��̾��.
	//OnFarmList				m_FarmInfo;					// ������ ���� ����ü.(uiAvailNum,szFarmName,szOwner,Labor)
	//stFarmGroup				*m_pGroup;		
	
	SOVillage_IndustrialComplex_Factory		m_Factory[ ON_MAX_FACTORY_NUM ];		// ���� ��.
	
private:	

	SOVillage_IndustrialComplex_Factory*	GetFactoryPlayerIn( SOPlayer *pPlayer );		// �÷��̾ �� �ִ� ������ ���´�.

	BOOL			IsMaterialItemEnoughToProduce( UI16 uiItemID, UI32 uiItemQuantity );	// �������� �����ϱ� ���Ͽ� â�� ����� ������ ��ᰡ �ִ��� �˻��Ѵ�.
	BOOL			IsMaterialToolEnoughToProduce( UI16 uiItemID );							// �������� �����ϱ� ���Ͽ� â�� ����� ������ ������ �ִ��� �˻��Ѵ�.

	BOOL			DeleteMaterialItem( UI16 uiItemID, UI32 uiItemQuantity );				// �� �������� �����ϱ� ���Ͽ� �ʿ��� ��Ḧ �����Ѵ�.
	BOOL			DestroyToolItemRandomly( UI16 uiItemID );								// �� �������� �����ϱ� ���Ͽ� �ʿ��� ������ �����ϰ� �ı� ��Ų��.
	

public:
	SOVillage_IndustrialComplex();									// ������.
	~SOVillage_IndustrialComplex();									// �Ҹ���.	

	BOOL					IsValidFactoryID( SI32 siFactoryID );	// ��ȿ�� ���� ID �ΰ�?
	
	VOID										Init(SI32 siIndustrialComplexID, UI16 uiVillageCode);			// �ʱ�ȭ.	
	VOID										InitIndustrialComplex();										// ��� ������ �ȷ��� �ÿ� ���ش�.
	VOID										Free();															// �޸� ����.
	SOVillage_IndustrialComplex_Factory*		GetFactory( SI32 siFactoryID );					// ������ ���´�.
	SOVillage_IndustrialComplex_Factory*		GetFactoryConstructed( SI32 siFactoryID );		// ������ �Ǽ��� ���� ��ü�� ���´�.
	SOVillage_IndustrialComplex_Factory*		GetEnteredFactory( SOPlayer *pPlayer );		// �÷��̾ �� ������ ���´�.
	SI32                                        GetEnteredFactoryIndex( SOPlayer *pPlayer );

	VOID					InitFromDB( SI32 siOwnerDBAccount, UI08 uiOwnerDBSlot, CHAR *pszOwnerName, 
									SI32 siMaxConstructibleFactoryNum, SI32 siDayNotWorked );	// DB���� ���� ������ ��� ������ �ʱ�ȭ �Ѵ�.

	BOOL					InitStorageFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
									UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
									UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
									UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
									UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.

	BOOL					InitFactoryFromDB( SI32 siFactoryID, UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],	// DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.
									UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity); 



	BOOL					EnterPlayer( SOPlayer *pPlayer );									// �÷��̾ ���Դ�.
	BOOL					LeavePlayer( SOPlayer *pPlayer );									// �÷��̾ ������.

	DWORD					GetInfo( OnFarmSlotList* pFactoryInfo, SI32 *psiFactoryNum );		// ��� ������ �ִ� ������� ������ ���´�.
	DWORD					GetInfo( OnIndustrialComplexInfo *pIndustrialComplexInfo );			// ��� ������ ������ ���´�.

	BOOL					HasOwner();															// �� ��������� ������ �ִ°�.
	BOOL					GetOwner( SI32 *psiOwnerDBAccount, UI08 *puiOwnerDBSlot );			// �� ��������� ������ ���´�.
	BOOL					IsOwner( SOPlayer *pPlayer );										// �� ��������� �������� �˻��Ѵ�.
	BOOL                    ChangeOwner(SOPlayer *pPlayer);                                     // ������ �ٸ�������� �ٲ۴�.

	BOOL					SellFactory( SOPlayer *pPlayer, SI32 siFactoryID );					// ������ ó���Ѵ�.
	
	BOOL					IsPlayerIn( SOPlayer *pPlayer );											// �÷��̾ �� ��� �����ȿ� ���� �ִ��� �˻��Ѵ�.	

//	BOOL					SendFactoryInfo( SOPlayer *pPlayer, SI32 siFactoryID );						// ��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.
//	BOOL					SendStorageInfo( SOPlayer *pPlayer, SI32 siFactoryID );						// ��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.	
	

	BOOL					BuyIndustrialComplex( SOPlayer *pPlayer, SI32 siMaxConstructibleFactoryNum);	// ��� ������ �����Ѵ�.	

	BOOL					Reclamation( SOPlayer *pPlayer, SI32 siMaxFactoryNum );																// ������ ��ô�Ѵ�.
	BOOL					ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnCapital );	// ������ �Ǽ��Ѵ�.
	BOOL					DestroyFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );											// ������ �ı��Ѵ�.

	BOOL					CanWork( SOPlayer *pPlayer );													// ���忡�� ���� �� �� �ִ��� �˻��Ѵ�.
	BOOL					CanWork( SOVillage_IndustrialComplex_Factory *pFactory );						// ���忡�� ���� �� �� �ִ��� �˻��Ѵ�.

	BOOL					EnterFactory( SOPlayer *pPlayer, SI32 siFactoryID );							// ��� ������ ���忡 ����.
	BOOL					LeaveFactory( SOPlayer *pPlayer );												// ��� ������ ���忡�� ������.
	
	BOOL					BeginWork( SOPlayer *pPlayer );													// ���� �����Ѵ�.
	BOOL					FinishWork( SOPlayer *pPlayer, OnReqFarmWorkMsg *pMsg );			// ���� �����ƴ�.

	BOOL					ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );			// ������ �ӱ��� �����Ѵ�.
	
	BOOL					CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );			// ���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.
	
	BOOL					UpgradeFactory( SOPlayer *pPlayer, SI32 siFactoryID, MONEY mnPay, MONEY *pmnCapital );	// ������ ���׷��̵� ��Ų��.

	DWORD					ProduceItem( SOPlayer *pPlayer, SI32 siFactoryID, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital );	// �������� �����Ѵ�.

	BOOL					AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE);				// â�� �ȿ� �������� �ִ´�.



	BOOL					DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );			// â��ȿ��� �������� �����Ѵ�.

	BOOL					AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);								// �ƹ� â�� �ȿ� �������� �ִ´�.
	

	BOOL					CanAddItem( UI16 uiItemID ,UI16 uiItemQuantity,SI32 *StorageID,SI32 * StoragePos);													// â�� �Ȱ� �ȿ� �������� ���� �� �ִ��� �˻��Ѵ�.	

	VOID					DestroyIndustrialComplexNotWork( MONEY *pmnIndustrialComplexPrice, MONEY mnIndustrialComplexCost);	// ���������� �ʴ� ��� ������ ���ش�.

	VOID					DayChanged( SOPlayer *pPlayer, MONEY mnIndustrialComplexCost );										// �Ϸ簡 ������.

	VOID                    DoNPCWork(UI32 uiNPCWork);

	static	VOID			InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );				// ���� ������ �ʱ�ȭ �Ѵ�.		

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