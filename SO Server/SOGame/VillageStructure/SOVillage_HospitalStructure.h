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
	SOVillage_Hospital();			// ������.
	~SOVillage_Hospital();			// �Ҹ���.
	
	BOOL			Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage);																	// �ʱ�ȭ.
	VOID			Free();																					// �޸𸮸� �����Ѵ�.	

	BOOL			HospitalGetCureList(SOPlayer *pPlayer);													// �뺴ġ�ῡ ����.
	BOOL			HospitalCureSolder(SOPlayer *pPlayer, UI08 Slot);										// �뺴ġ�ῡ�� �뺴�� ġ���Ѵ�.
	BOOL			HospitalCureAllSolder(SOPlayer *pPlayer);												// �뺴ġ�ῡ�� ��� �뺴�� ġ���Ѵ�.
	SI32			HospitalGetCureMoney(SOPlayer *pPlayer, UI08 Slot, SI32 Discount);						// �뺴 ġ��� ����
	SI32			HospitalGetRevivalMoney(SOPlayer *pPlayer, UI08 Slot, SI32 Discount);					// �뺴 ��Ȱ�� ����	
};


#endif
