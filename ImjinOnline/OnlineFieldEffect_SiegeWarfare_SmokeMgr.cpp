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

// 초기화
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::Init()
{
	ZeroMemory(m_pSiegeWarfare_Smoke_Info, sizeof(SHORT) * MAX_SMOKE_NUMBER);
	ZeroMemory(m_pSiegeWarfare_Smoke_List, sizeof(COnlineFieldEffect_SiegeWarfare_Smoke) * MAX_SMOKE_NUMBER);
}

// Smoke 객체 생성
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
// Smoke 타일 내 그려지는 위치 보정
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::SetSiegeWarfare_DrawPositionRandomizeInTile(SI32 siSmokeIndex, SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY)
{
	m_pSiegeWarfare_Smoke_List[siSmokeIndex].SetSiegeWarfare_DrawPositionRandomizeInTile(scDrawPositionInTileX, scDrawPositionInTileY);
}

// robypark 2005/1/14 18:39
// 그릴 때 캐릭터 중심에 그려지도록 설정하기
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::SetSiegeWarfare_SetDrawInCharacterCenterPos(SI32 siSmokeIndex, UI16 uiUniqueID)
{
	m_pSiegeWarfare_Smoke_List[siSmokeIndex].SetSiegeWarfare_SetDrawInCharacterCenterPos(uiUniqueID);
}

// Smoke 객체들 처리(이동)
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::Action(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		// 유효한 Smoke
		if (0 != m_pSiegeWarfare_Smoke_Info[siIndex])
		{
			m_pSiegeWarfare_Smoke_List[siIndex].CalculateDrawStartXY();

			// Smoke 이동 처리
			if (FALSE == m_pSiegeWarfare_Smoke_List[siIndex].Action())
			{
				// 애니메이션이 끝난 Smoke, 리스트에서 삭제
				DeleteSiegeWarfareSmoke(siIndex);
			}
		}
	}
}

// Smoke 객체들 그리기
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::Draw(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		// 유효한 Smoke
		if (0 != m_pSiegeWarfare_Smoke_Info[siIndex])
		{
			m_pSiegeWarfare_Smoke_List[siIndex].CalculateDrawStartXY();

			// Smoke 그리기
			m_pSiegeWarfare_Smoke_List[siIndex].Draw();
		}
	}
}

// Smoke 객체 리스트 중에서 추가할 수 있는 Smoke 인덱스 얻기
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

// Smoke 객체 리스트에서 삭제
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::DeleteSiegeWarfareSmoke(SI32 siIndex)
{
	m_pSiegeWarfare_Smoke_Info[siIndex] = 0;
}

// 모든 Smoke 객체 리스트 삭제
void COnlineFieldEffect_SiegeWarfare_SmokeMgr::DeleteSiegeWarfareSmokeList(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_SMOKE_NUMBER; siIndex++)
	{
		DeleteSiegeWarfareSmoke(siIndex);
	}
}
