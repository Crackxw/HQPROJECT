// OnlineFieldEffect_SiegeWarfare_SmokeMgr.h: interface for the COnlineFieldEffect_SiegeWarfare_SmokeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKEMGR_H__26A4EF4F_0006_4247_8885_1B35E12CFCC2__INCLUDED_)
#define AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKEMGR_H__26A4EF4F_0006_4247_8885_1B35E12CFCC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OnlineFieldEffect_SiegeWarfare_Smoke.h"

// robypark 2004/10/22 15:07
// �¶��� �ʵ� ���� ����Ʈ - ������ ���� Ư�� ȿ��. ����, ���� ��Ʈ ȿ�� ������
class COnlineFieldEffect_SiegeWarfare_SmokeMgr  
{
	public:
		COnlineFieldEffect_SiegeWarfare_SmokeMgr();
		virtual ~COnlineFieldEffect_SiegeWarfare_SmokeMgr();

	protected:
		// ����ũ(ȿ��) ��ü ����Ʈ
		COnlineFieldEffect_SiegeWarfare_Smoke				m_pSiegeWarfare_Smoke_List[MAX_SMOKE_NUMBER];

		// ����ũ(ȿ��) ���� ����Ʈ
		// ����ϴ� ������ �˻�
		SHORT												m_pSiegeWarfare_Smoke_Info[MAX_SMOKE_NUMBER];

	public:
		// �ʱ�ȭ
		void Init();

		// Smoke ��ü ����
		SI32 SetSiegeWarfare_Smoke(SHORT ssKind, SHORT ssSmokeX, SHORT ssSmokeY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0, SI32 siEffectSndIdx = 0, bool bSoundDelayor = false);

		// robypark 2004/12/29 16:8
		// Smoke Ÿ�� �� �׷����� ��ġ ����
		void SetSiegeWarfare_DrawPositionRandomizeInTile(SI32 siSmokeIndex, SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY);

		// robypark 2005/1/14 18:39
		// �׸� �� ĳ���� �߽ɿ� �׷������� �����ϱ�
		void SetSiegeWarfare_SetDrawInCharacterCenterPos(SI32 siSmokeIndex, UI16 uiUniqueID);

		// Smoke ��ü�� ó��(�̵�)
		void Action(void);

		// Smoke ��ü�� �׸���
		void Draw(void);

	protected:
		// Smoke ��ü ����Ʈ �߿��� �߰��� �� �ִ� Smoke �ε��� ���
		SI32 GetEmptySiegeWarfareSmokeIdx(void);

		// Smoke ��ü ����Ʈ���� ����
		void DeleteSiegeWarfareSmoke(SI32 siIndex);

		// ��� Smoke ��ü ����Ʈ ����
		void DeleteSiegeWarfareSmokeList(void);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKEMGR_H__26A4EF4F_0006_4247_8885_1B35E12CFCC2__INCLUDED_)
