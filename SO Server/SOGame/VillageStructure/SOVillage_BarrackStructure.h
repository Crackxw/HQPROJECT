#ifndef	_VILLAGE_BARRACK_HEADER
#define	_VILLAGE_BARRACK_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"


class	SODatabase;


class	SOVillage_Barrack : public SOVillage_BaseStructure
{
private:
	SOVillage*		m_lpVillage;

public:
	SOVillage_Barrack();			// ������.
	~SOVillage_Barrack();			// �Ҹ���.
	
	BOOL			Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage);																	// �ʱ�ȭ.
	VOID			Free();																					// �޸𸮸� �����Ѵ�.	

	// �Ʒü� �ȿ����� �Լ���
	BOOL			BarrackGetSolderList(SOPlayer *pPlayer);												// �뺴��뿡 ����.
	BOOL			BarrackBuySolder(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);							// �뺴��뿡�� �뺴�� ����Ѵ�.
	BOOL			BarrackGetDismissalSolderList(SOPlayer *pPlayer);										// �뺴�ذ� ����.
	BOOL			BarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot);									// �뺴�ذ��� �뺴�� �ذ��Ѵ�.
	MONEY			BarrackDismissalSolderMoney(SOPlayer *pPlayer, SI32 Slot);								// �ذ��� �뺴�� ������ ��´�.

	// ����缺�� �ȿ����� �Լ���
	BOOL			GBarrackGetSolderList(SOPlayer *pPlayer);												// ����뺴��뿡 ����.
	BOOL			GBarrackBuySolder(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);							// ����뺴��뿡�� �뺴�� ����Ѵ�.
	BOOL			GBarrackGetChangeJobSolderList(SOPlayer *pPlayer);										// ����뺴������ ����.
	BOOL			GBarrackChangeJobSolder(SOPlayer *pPlayer, SI32 Slot, SI32 Kind);						// ����뺴�������� �뺴�� �����Ѵ�.
	BOOL			GBarrackGetDismissalSolderList(SOPlayer *pPlayer);										// ����뺴�ذ� ����.
	BOOL			GBarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot);									// ����뺴�ذ��� �뺴�� �ذ��Ѵ�.
	BOOL			GBarrack_CheckChangeJob(GeneralHeader* lpGeneralData, SOPlayer *pPlayer, SI32 Slot);	// �뺴�� ����� �ٲܼ� �ִ��� �˻�
	SI32			GBarrack_ChangeJobBonus(SOPlayer *pPlayer, SI32 Slot);									// �뺴�� ����� �ٲܽ� ������ �ִ� Bonus Point
	MONEY			GBarrackDismissalSolderMoney(SOPlayer *pPlayer, SI32 Slot,SI32 siGeneralCode);			// �ذ��� ����� ������ ��´�.

	BOOL			GBarrackGetGeneralExList(SOPlayer *pPlayer);											// 2�� ��� ��뿡 ����.
	BOOL			GBarrackBuyGeneralEx(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);						// 2�� ����� ����Ѵ�.
	BOOL			GBarrackGetChangeGeneralExList(SOPlayer *pPlayer);										// 2�� ��� ������ ����.
	BOOL			GBarrackChangeGeneralEx(SOPlayer *pPlayer,SI32 Slot, SI32 Kind);						// 2�� ����� ����Ѵ�.
	BOOL			GBarrackGetDismissalGeneralExList(SOPlayer *pPlayer);									// 2�� ��� �ذ� ����.
	BOOL			GBarrackSellDismissalGeneralEx(SOPlayer *pPlayer, SI32 Slot);							// 2�� ����� �ذ��Ѵ�.


	SI32			GBarrackGetChangeGeneralExBonus(SOPlayer *pPlayer, SI32 Slot);




	// ���ü� �ȿ����� �Լ���
	BOOL			MBarrackGetMonsterList( SOPlayer *pPlayer );											// ��� ������ ���� ����Ʈ�� �����ش�.
	BOOL			MBarrackBuyMonster( SOPlayer *pPlayer, SI32 Code, BYTE* lpName );						// ���͸� ����Ѵ�.
	BOOL			MBarrackGetDismissMonsterList( SOPlayer *pPlayer );										// �ذ� ������ ���� ����Ʈ�� �����ش�.
	BOOL			MBarrackDismissMonster( SOPlayer *pPlayer, SI32 Slot );									// ���͸� �ذ��Ѵ�.
	MONEY			MBarrackDismissMonsterMoney( SOPlayer *pPlayer, SI32 Slot );							// �ذ��� ������ ������ ��´�.
	SI32			GetRequiredSealingBeadNum( SI16 siNeedLv );
};


#endif
