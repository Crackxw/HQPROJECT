// OnlineFieldEffect_SiegeWarfare_BulletMgr.cpp: implementation of the COnlineFieldEffect_SiegeWarfare_BulletMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "OnlineFieldEffect_SiegeWarfare_BulletMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineFieldEffect_SiegeWarfare_BulletMgr::COnlineFieldEffect_SiegeWarfare_BulletMgr()
{
	ZeroMemory(m_pSiegeWarfare_Bullet_Info, sizeof(SHORT) * MAX_BULLET_NUMBER);
	ZeroMemory(m_pSiegeWarfare_Bullet_List, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet) * MAX_BULLET_NUMBER);
}

COnlineFieldEffect_SiegeWarfare_BulletMgr::~COnlineFieldEffect_SiegeWarfare_BulletMgr()
{
}

// 초기화
void COnlineFieldEffect_SiegeWarfare_BulletMgr::Init()
{
	ZeroMemory(m_pSiegeWarfare_Bullet_Info, sizeof(SHORT) * MAX_BULLET_NUMBER);
	ZeroMemory(m_pSiegeWarfare_Bullet_List, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet) * MAX_BULLET_NUMBER);
}

// 총알 객체 생성
void COnlineFieldEffect_SiegeWarfare_BulletMgr::SetSiegeWarfare_Bullet(SHORT ssKind, SHORT ssShooterX, SHORT ssShooterY, SHORT ssTargetX, SHORT ssTargetY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/)
{
	SI32 siEmptyIndex = GetEmptySiegeWarfareBulletIdx();

	if (0 != siEmptyIndex)
	{
		m_pSiegeWarfare_Bullet_List[siEmptyIndex].Init(ssKind, siEmptyIndex, ssShooterX, ssShooterY, ssTargetX, ssTargetY, ssShooterX, ssShooterY, ssTargetX, ssTargetY, 0, uiTargetUniqueID, uiTargetType, ssAnimationDelay);
		m_pSiegeWarfare_Bullet_Info[siEmptyIndex] = ssKind;
	}
}

// 총알 객체들 처리(이동)
void COnlineFieldEffect_SiegeWarfare_BulletMgr::Action(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		// 유효한 총알
		if (0 != m_pSiegeWarfare_Bullet_Info[siIndex])
		{
			// 총알 이동 처리
			if (FALSE == m_pSiegeWarfare_Bullet_List[siIndex].Action())
			{
				// 목표 지점에 도달한 총알 리스트에서 삭제
				DeleteSiegeWarfareBullet(siIndex);
			}
		}
	}
}

// 총알 객체들 그리기
void COnlineFieldEffect_SiegeWarfare_BulletMgr::Draw(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		// 유효한 총알
		if (0 != m_pSiegeWarfare_Bullet_Info[siIndex])
		{
			m_pSiegeWarfare_Bullet_List[siIndex].CalculateDrawStartXY();

			// 총알 그리기
			m_pSiegeWarfare_Bullet_List[siIndex].Draw();
		}
	}
}

// 총알 객체 리스트 중에서 추가할 수 있는 총알 인덱스 얻기
SI32 COnlineFieldEffect_SiegeWarfare_BulletMgr::GetEmptySiegeWarfareBulletIdx(void)
{
	int siIndex;

	for(siIndex = 1;siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		if (0 == m_pSiegeWarfare_Bullet_Info[siIndex])
			return siIndex;
	}

	return 0;
}

// 총알 객체 리스트에서 삭제
void COnlineFieldEffect_SiegeWarfare_BulletMgr::DeleteSiegeWarfareBullet(SI32 siIndex)
{
	m_pSiegeWarfare_Bullet_Info[siIndex] = 0;
}

// 모든 총알 객체 리스트 삭제
void COnlineFieldEffect_SiegeWarfare_BulletMgr::DeleteSiegeWarfareBulletList(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		DeleteSiegeWarfareBullet(siIndex);
	}
}
