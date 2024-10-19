// OnlineFieldEffect_SiegeWarfare_SmokeMgr.cpp: implementation of the COnlineFieldEffect_SiegeWarfare_SmokeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "OnlineFieldEffect_SiegeWarfare_SmokeMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineFieldEffect_SiegeWarfare_SmokeMgr::COnlineFieldEffect_SiegeWarfare_SmokeMgr()
{
	ZeroMemory(m_pSiegeWarfare_Smoke_Info, sizeof(SHORT) * MAX_SMOKE_NUMBER);
	ZeroMemory(m_pSiegeWarfare_Smoke_List, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke) * MAX_SMOKE_NUMBER);

}

COnlineFieldEffect_SiegeWarfare_SmokeMgr::~COnlineFieldEffect_SiegeWarfare_SmokeMgr()
{

}

// �ʱ�ȭ
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::Init()
{
	ZeroMemory(m_pSiegeWarfare_Smoke_Info, sizeof(SHORT) * MAX_SMOKE_NUMBER);
	ZeroMemory(m_pSiegeWarfare_Smoke_List, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke) * MAX_SMOKE_NUMBER);
}

// Smoke ��ü ����
SI32 COnlineFieldEffect_SiegeWarfare_SmokeMgr::SetSiegeWarfare_Smoke(SHORT ssKind, SHORT ssSmokeX, SHORT ssSmokeY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/, SI32 siEffectSndIdx/* = 0*/, bool bSoundDelayor/* = false*/)
{
	SI32 siEmptyIndex = GetEmptySiegeWarfareSmokeIdx();

	if (0 != siEmptyIndex)
	{
		m_pSiegeWarfare_Smoke_List[siEmptyIndex].Init(ssKind, siEmptyIndex, ssSmokeX, ssSmokeY, uiTargetUniqueID, uiTargetType, ssAnimationDelay, siEffectSndIdx, bSoundDelayor);
		m_pSiegeWarfare_Smoke_Info[siEmptyIndex] = ssKind;

		return siEmptyIndex;
	}

	return 0;
}

// robypark 2004/12/29 16:8
// Smoke Ÿ�� �� �׷����� ��ġ ����
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::SetSiegeWarfare_DrawPositionRandomizeInTile(SI32 siSmokeIndex, SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY)
{
	m_pSiegeWarfare_Smoke_List[siSmokeIndex].SetSiegeWarfare_DrawPositionRandomizeInTile(scDrawPositionInTileX, scDrawPositionInTileY);
}

// robypark 2005/1/14 18:39
// �׸� �� ĳ���� �߽ɿ� �׷������� �����ϱ�
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::SetSiegeWarfare_SetDrawInCharacterCenterPos(SI32 siSmokeIndex, UI16 uiUniqueID)
{
	m_pSiegeWarfare_Smoke_List[siSmokeIndex].SetSiegeWarfare_SetDrawInCharacterCenterPos(uiUniqueID);
}

// Smoke ��ü�� ó��(�̵�)
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::Action(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		// ��ȿ�� Smoke
		if (0 != m_pSiegeWarfare_Smoke_Info[siIndex])
		{
			m_pSiegeWarfare_Smoke_List[siIndex].CalculateDrawStartXY();

			// Smoke �̵� ó��
			if (FALSE == m_pSiegeWarfare_Smoke_List[siIndex].Action())
			{
				// �ִϸ��̼��� ���� Smoke, ����Ʈ���� ����
				DeleteSiegeWarfareSmoke(siIndex);
			}
		}
	}
}

// Smoke ��ü�� �׸���
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::Draw(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		// ��ȿ�� Smoke
		if (0 != m_pSiegeWarfare_Smoke_Info[siIndex])
		{
			m_pSiegeWarfare_Smoke_List[siIndex].CalculateDrawStartXY();

			// Smoke �׸���
			m_pSiegeWarfare_Smoke_List[siIndex].Draw();
		}
	}
}

// Smoke ��ü ����Ʈ �߿��� �߰��� �� �ִ� Smoke �ε��� ���
SI32 COnlineFieldEffect_SiegeWarfare_SmokeMgr::GetEmptySiegeWarfareSmokeIdx(void)
{
	int siIndex;

	for(siIndex = 1;siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		if (0 == m_pSiegeWarfare_Smoke_Info[siIndex])
			return siIndex;
	}

	return 0;
}

// Smoke ��ü ����Ʈ���� ����
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::DeleteSiegeWarfareSmoke(SI32 siIndex)
{
	m_pSiegeWarfare_Smoke_Info[siIndex] = 0;
}

// ��� Smoke ��ü ����Ʈ ����
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::DeleteSiegeWarfareSmokeList(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		DeleteSiegeWarfareSmoke(siIndex);
	}
}
