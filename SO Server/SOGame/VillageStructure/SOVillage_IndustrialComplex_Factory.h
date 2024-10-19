#ifndef	_VILLAGE_FACTORY_INDUSTRIAL_COMPLEX_FACTORY_HEADER
#define	_VILLAGE_FACTORY_INDUSTRIAL_COMPLEX_FACTORY_HEADER

#include "GSCDefine.h"
#include "SOCommonHeader.h"
#include "PlayersManager.h"

#define	ON_MAX_FACTORY_PLAYER_NUM		100							// �� ���忡 �� �� �ִ� �ִ� �÷��̾� ��.

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

	UI16				m_uiVillageCode;							// �� ������ �ִ� ������ ID
	SI32				m_siIndustrialComplexID;				// �� ������ �ִ� ��������� ID
	SI32				m_siFactoryID;							// ������ ID.	
	
	UI08				m_uiLevel;								// ������ ����
	CHAR				m_cFactoryKind[ 4 ];					// ������ ����.

	MONEY				m_mnPay;								// �� ������ ���Ҷ��� �ӱ�.
	//UI32				m_uiAmountOfWork;						// �۾���.
	UI32				m_uiRemainAmountOfWork;					// ���� �۾���.
	
	UI16				m_uiProducingItemID;					// �������� ������.
	
	UI32				m_uiMaxProduceItemQuantity;				// �����ؾ� �ϴ� �������� ����.

	BOOL				m_bStore;								// �� ������ â���ΰ�.
		
	PlayersManager		m_PlayerMgr;							// ������ ���� �ִ� �÷��̾��.
	PlayersManager		m_WorkingPlayerMgr;						// ���������� ���� ���� �ϰ� �ִ� �÷��̾��.

	STORAGE_ITEM		m_StorageItem[ ON_MAX_STORAGEITEM ];	// â���� ��쿡 â�� �ȿ� �ִ� ������.


private:
	VOID				LevelUp( SOPlayer *pPlayerCompletedWork );							// ������ ������Ų��.

	BOOL				SendFactoryInfo( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner );	// ��� ������ �ִ� ������ ������ �÷��̾�� �����ش�.

	BOOL				IsUpgradeCompleted();					// ���׷��̵尡 �Ϸ�Ǿ����� �˻��Ѵ�.	

	UI32				GetAmountOfWorkByQuantity( UI16 uiItemID, UI32 uiItemQuantity );		// ������ ������ �뵿������ �ٲ۴�.
	UI32				GetItemQuantityByAmountOfWork( UI16 uiItemID, UI32 *puiAmountOfWork );	// �뵿���� �������� ������ �ٲ۴�.

public:
	STORAGE_ITEM*		GetItem( UI16 uiItemID, SI32 *psiIndex );		// â�� �ִ� �������� ���´�.
private:

	STORAGE_ITEM*		GetEmptyItem( SI32 *psiIndex );					// â�� ����ִ� �������� ���´�.
	
	VOID				SetItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity );	// â�� �ִ� �������� ������ DB�� �����Ѵ�.
	VOID				DelItemInfoToDB( SI32 siItemIndex, UI16 uiItemID, UI16 uiItemQuantity );	// â�� �ִ� �������� ������ DB�� �����Ѵ�.
	VOID				WorkCompleted( SOPlayer *pPlayerCompletedWork );							// ������ �۾��� �Ϸ�Ǿ���.


public:
	SOVillage_IndustrialComplex_Factory();				// ������.
	~SOVillage_IndustrialComplex_Factory();				// �Ҹ���.
		
	VOID	Init( UI16 uiVillageCode, SI32 siIndustrialComplexID, SI32 siFactoryID );				// �ʱ�ȭ.
	VOID	InitFactory();																		// ������ �ʱ�ȭ �Ѵ�.
	VOID	Free();																				// ����.
	VOID	SetStatus( FactoryStatus Status );					// ������ ���¸� �����Ѵ�.

	VOID	InitWork();											// �۾��� �ʱ�ȭ �Ѵ�

	BOOL	DidConstructFactory();								// ������ �Ǽ��Ǿ����� �˻��Ѵ�.

	DWORD	GetInfo( OnFarmSlotList* pFactoryInfo );			// ������ ������ ���´�.

	BOOL	EnterPlayer( SOPlayer *pPlayer, BOOL bIndustrialComplexOwner );					// �÷��̾ ���Դ�.
	BOOL	LeavePlayer( SOPlayer *pPlayer );												// �÷��̾ ������.

	BOOL	InitStorageFromDB( UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, 
						UI16 uiItemID0, UI16 uiItemID1, UI16 uiItemID2, UI16 uiItemID3, UI16 uiItemID4, 
						UI16 uiItemID5, UI16 uiItemID6, UI16 uiItemID7, UI16 uiItemID8, UI16 uiItemID9, 
						UI16 uiQuantity0, UI16 uiQuantity1, UI16 uiQuantity2, UI16 uiQuantity3, UI16 uiQuantity4, 
						UI16 uiQuantity5, UI16 uiQuantity6, UI16 uiQuantity7, UI16 uiQuantity8, UI16 uiQuantity9 );	// DB���� ���� ������ â�� �ʱ�ȭ �Ѵ�.

	BOOL	InitFactoryFromDB( UI08 uiLevel, MONEY mnPay, UI32 uiAmountOfWork, CHAR cFactoryKind[ 4 ],				// DB���� ���� ������ ������ �ʱ�ȭ �Ѵ�.
						UI16 uiProducingItemID, UI32 uiProducedItemQuantity, UI32 uiMaxProduceItemQuantity); 


	BOOL	SellFactory( SOPlayer *pPlayer );												// ������ ó���Ѵ�.

	BOOL	IsPlayerIn( SOPlayer *pPlayer );												// �÷��̾ �� ����ȿ� ���� �ִ��� �˻��Ѵ�.	


	
//	BOOL	SendStorageInfo( SOPlayer *pPlayer );				// ��� ������ �ִ� â���� ������ �÷��̾�� �����ش�.

	BOOL	CanWork();											// �� ���忡�� ���� �� �� �ִ��� �˾ƿ´�. 

	BOOL	IsStorage();										// �� ������ â������ �˾ƿ´�.
	BOOL	IsFactory();										// �� ������ �Ϲ����� �������� �˾ƿ´�.

	UI32	GetAmountOfWork();									// �۾����� ���´�.
	

	BOOL	ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay, MONEY *pmnTotalPay );		// ������ �Ǽ��Ѵ�.	
	VOID	DestroyFactory( SOPlayer *pPlayer, MONEY *pmnCapital );																	// ������ �ı��Ѵ�.

	BOOL	BeginWork( SOPlayer *pPlayer );																			// ���� �����Ѵ�.
	BOOL	FinishWork( SOPlayer *pPlayer, SOVillage_IndustrialComplex *pComplex, OnReqFarmWorkMsg *pMsg );									// ���� �����ƴ�.
	BOOL	FinishWorkByNPC( SOVillage_IndustrialComplex *pComplex,UI32 uiAmountOfWork);									// ���� �����ƴ�.

	BOOL	ChangeFactoryPay( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );									// ������ �ӱ��� �����Ѵ�.

	BOOL	CancelWork( SOPlayer *pPlayer, SI32 siFactoryID, MONEY *pmnCapital );									// ���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.

	BOOL	IsConstructing();																// �ǹ��� �Ǽ������� �˾ƿ´�.
	BOOL	IsUpgrading();																	// ������ ���׷��̵������� �˾ƿ´�.

	BOOL	UpgradeFactory( SOPlayer *pPlayer, MONEY mnPay, MONEY *pmnCapital );			// ������ ���׷��̵� ��Ų��.

	DWORD	ProduceItem( SOPlayer *pPlayer, UI16 uiItemID, UI32 uiItemQuantity, MONEY mnPay, CHAR cNation, MONEY *pmnCapital );		// �������� �����Ѵ�.

	BOOL	HasItem( UI16 uiItemID, UI16 uiItemQuantity );									// â�� �ȿ� ����� �������� �ִ��� �˻��Ѵ�.	

	BOOL	AddItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);									// â�� �ȿ� �������� �ִ´�.
	BOOL	DeleteItem( UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);								// â��ȿ��� �������� �����Ѵ�.	

	BOOL	AddItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);				// â�� �ȿ� �������� �ִ´�.
	BOOL	DeleteItem( SOPlayer *pPlayer, UI16 uiItemID, UI16 uiItemQuantity ,BOOL bSendToDB = TRUE);			// â��ȿ��� �������� �����Ѵ�.	

	
	BOOL	CanAddItem( UI16 uiItemID ,UI16 uiQuantity,SI32 *pStoragePos);													// â�� �ȿ� �������� ���� �� �ִ��� �˻��Ѵ�.

	UI16	GetProducingItemID();															// ���忡�� �������� �������� ID�� ���´�.
	UI32    GetProducingItemNumber() {return m_uiMaxProduceItemQuantity;};
	MONEY   GetPay() {return m_mnPay;};
	const   CHAR	*GetFactoryKind() {return m_cFactoryKind;};


	static	VOID	InitStaticVariable( CItem *pItem, SODatabase *pDataBase, OnlineBuilding *pBuilding );	// ���� ������ �ʱ�ȭ �Ѵ�.		
};

#endif
