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

// �ʱ�ȭ
void COnlineFieldEffect_SiegeWarfare_BulletMgr::Init()
{
	ZeroMemory(m_pSiegeWarfare_Bullet_Info, sizeof(SHORT) * MAX_BULLET_NUMBER);
	ZeroMemory(m_pSiegeWarfare_Bullet_List, sizeof(COnlineFieldEffect_SiegeWarfare_Bullet) * MAX_BULLET_NUMBER);
}

// �Ѿ� ��ü ����
void COnlineFieldEffect_SiegeWarfare_BulletMgr::SetSiegeWarfare_Bullet(SHORT ssKind, SHORT ssShooterX, SHORT ssShooterY, SHORT ssTargetX, SHORT ssTargetY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay/* = 0*/)
{
	SI32 siEmptyIndex = GetEmptySiegeWarfareBulletIdx();

	if (0 != siEmptyIndex)
	{
		m_pSiegeWarfare_Bullet_List[siEmptyIndex].Init(ssKind, siEmptyIndex, ssShooterX, ssShooterY, ssTargetX, ssTargetY, ssShooterX, ssShooterY, ssTargetX, ssTargetY, 0, uiTargetUniqueID, uiTargetType, ssAnimationDelay);
		m_pSiegeWarfare_Bullet_Info[siEmptyIndex] = ssKind;
	}
}

// �Ѿ� ��ü�� ó��(�̵�)
void COnlineFieldEffect_SiegeWarfare_BulletMgr::Action(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		// ��ȿ�� �Ѿ�
		if (0 != m_pSiegeWarfare_Bullet_Info[siIndex])
		{
			// �Ѿ� �̵� ó��
			if (FALSE == m_pSiegeWarfare_Bullet_List[siIndex].Action())
			{
				// ��ǥ ������ ������ �Ѿ� ����Ʈ���� ����
				DeleteSiegeWarfareBullet(siIndex);
			}
		}
	}
}

// �Ѿ� ��ü�� �׸���
void COnlineFieldEffect_SiegeWarfare_BulletMgr::Draw(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		// ��ȿ�� �Ѿ�
		if (0 != m_pSiegeWarfare_Bullet_Info[siIndex])
		{
			m_pSiegeWarfare_Bullet_List[siIndex].CalculateDrawStartXY();

			// �Ѿ� �׸���
			m_pSiegeWarfare_Bullet_List[siIndex].Draw();
		}
	}
}

// �Ѿ� ��ü ����Ʈ �߿��� �߰��� �� �ִ� �Ѿ� �ε��� ���
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

// �Ѿ� ��ü ����Ʈ���� ����
void COnlineFieldEffect_SiegeWarfare_BulletMgr::DeleteSiegeWarfareBullet(SI32 siIndex)
{
	m_pSiegeWarfare_Bullet_Info[siIndex] = 0;
}

// ��� �Ѿ� ��ü ����Ʈ ����
void COnlineFieldEffect_SiegeWarfare_BulletMgr::DeleteSiegeWarfareBulletList(void)
{
	SI32 siIndex = 0;
	for (siIndex = 0; siIndex < MAX_BULLET_NUMBER; siIndex++)
	{
		DeleteSiegeWarfareBullet(siIndex);
	}
}
