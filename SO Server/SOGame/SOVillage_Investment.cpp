// Type�� -1�̸� ��� ������ Type�� ���ϴ� �Ŵ�.
#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
void		SOVillage::GetVillageInfo(SOPlayer *pPlayer)
{
	pPlayer->SendFieldMsg( ON_RESPONSE_GOVERNMENT_VILLAGEINFO, SO_SFM_ONLYME, LPVOID( szDescription ), LPVOID( GetVillageInOutPlayer() ),
							LPVOID( &vdDefence ), LPVOID( &vdMaxDefence ) );
}
