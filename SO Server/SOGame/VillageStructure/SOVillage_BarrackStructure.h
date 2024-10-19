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
	SOVillage_Barrack();			// 생성자.
	~SOVillage_Barrack();			// 소멸자.
	
	BOOL			Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage);																	// 초기화.
	VOID			Free();																					// 메모리를 해제한다.	

	// 훈련소 안에서의 함수들
	BOOL			BarrackGetSolderList(SOPlayer *pPlayer);												// 용병고용에 들어간다.
	BOOL			BarrackBuySolder(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);							// 용병고용에서 용병을 고용한다.
	BOOL			BarrackGetDismissalSolderList(SOPlayer *pPlayer);										// 용병해고에 들어간다.
	BOOL			BarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot);									// 용병해고에서 용병을 해고한다.
	MONEY			BarrackDismissalSolderMoney(SOPlayer *pPlayer, SI32 Slot);								// 해고할 용병의 가격을 얻는다.

	// 장수양성소 안에서의 함수들
	BOOL			GBarrackGetSolderList(SOPlayer *pPlayer);												// 장수용병고용에 들어간다.
	BOOL			GBarrackBuySolder(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);							// 장수용병고용에서 용병을 고용한다.
	BOOL			GBarrackGetChangeJobSolderList(SOPlayer *pPlayer);										// 장수용병전직에 들어간다.
	BOOL			GBarrackChangeJobSolder(SOPlayer *pPlayer, SI32 Slot, SI32 Kind);						// 장수용병전직에서 용병을 전직한다.
	BOOL			GBarrackGetDismissalSolderList(SOPlayer *pPlayer);										// 장수용병해고에 들어간다.
	BOOL			GBarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot);									// 장수용병해고에서 용병을 해고한다.
	BOOL			GBarrack_CheckChangeJob(GeneralHeader* lpGeneralData, SOPlayer *pPlayer, SI32 Slot);	// 용병을 장수로 바꿀수 있는지 검사
	SI32			GBarrack_ChangeJobBonus(SOPlayer *pPlayer, SI32 Slot);									// 용병을 장수로 바꿀시 얻을수 있는 Bonus Point
	MONEY			GBarrackDismissalSolderMoney(SOPlayer *pPlayer, SI32 Slot,SI32 siGeneralCode);			// 해고할 장수의 가격을 얻는다.

	BOOL			GBarrackGetGeneralExList(SOPlayer *pPlayer);											// 2차 장수 고용에 들어간다.
	BOOL			GBarrackBuyGeneralEx(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);						// 2차 장수를 고용한다.
	BOOL			GBarrackGetChangeGeneralExList(SOPlayer *pPlayer);										// 2차 장수 전직에 들어간다.
	BOOL			GBarrackChangeGeneralEx(SOPlayer *pPlayer,SI32 Slot, SI32 Kind);						// 2차 장수를 고용한다.
	BOOL			GBarrackGetDismissalGeneralExList(SOPlayer *pPlayer);									// 2차 장수 해고에 들어간다.
	BOOL			GBarrackSellDismissalGeneralEx(SOPlayer *pPlayer, SI32 Slot);							// 2차 장수를 해고한다.


	SI32			GBarrackGetChangeGeneralExBonus(SOPlayer *pPlayer, SI32 Slot);




	// 조련소 안에서의 함수들
	BOOL			MBarrackGetMonsterList( SOPlayer *pPlayer );											// 고용 가능한 몬스터 리스트를 보내준다.
	BOOL			MBarrackBuyMonster( SOPlayer *pPlayer, SI32 Code, BYTE* lpName );						// 몬스터를 고용한다.
	BOOL			MBarrackGetDismissMonsterList( SOPlayer *pPlayer );										// 해고 가능한 몬스터 리스트를 보내준다.
	BOOL			MBarrackDismissMonster( SOPlayer *pPlayer, SI32 Slot );									// 몬스터를 해고한다.
	MONEY			MBarrackDismissMonsterMoney( SOPlayer *pPlayer, SI32 Slot );							// 해고할 몬스터의 가격을 얻는다.
	SI32			GetRequiredSealingBeadNum( SI16 siNeedLv );
};


#endif
