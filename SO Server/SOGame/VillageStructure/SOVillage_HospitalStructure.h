#ifndef	_VILLAGE_HOSPITAL_HEADER
#define	_VILLAGE_HOSPITAL_HEADER


#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"


class	SODatabase;


class	SOVillage_Hospital : public SOVillage_BaseStructure
{
private:
	SOVillage*		m_lpVillage;

public:
	SOVillage_Hospital();			// 생성자.
	~SOVillage_Hospital();			// 소멸자.
	
	BOOL			Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage);																	// 초기화.
	VOID			Free();																					// 메모리를 해제한다.	

	BOOL			HospitalGetCureList(SOPlayer *pPlayer);													// 용병치료에 들어간다.
	BOOL			HospitalCureSolder(SOPlayer *pPlayer, UI08 Slot);										// 용병치료에서 용병을 치료한다.
	BOOL			HospitalCureAllSolder(SOPlayer *pPlayer);												// 용병치료에서 모든 용병을 치료한다.
	SI32			HospitalGetCureMoney(SOPlayer *pPlayer, UI08 Slot, SI32 Discount);						// 용병 치료시 가격
	SI32			HospitalGetRevivalMoney(SOPlayer *pPlayer, UI08 Slot, SI32 Discount);					// 용병 부활시 가격	
};


#endif
