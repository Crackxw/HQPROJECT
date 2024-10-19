// OnlineFieldEffect_SiegeWarfare_BulletMgr.h: interface for the COnlineFieldEffect_SiegeWarfare_BulletMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLETMGR_H__863FD1B2_0CC6_4C83_8E8F_F987212790DE__INCLUDED_)
#define AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLETMGR_H__863FD1B2_0CC6_4C83_8E8F_F987212790DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OnlineFieldEffect_SiegeWarfare_Bullet.h"

// robypark 2004/10/22 10:52
// ������ �ʵ� �������� ������ ������ �߻��ϴ� �Ѿ� ��ü ���� Ŭ����
class COnlineFieldEffect_SiegeWarfare_BulletMgr  
{
	public:
		COnlineFieldEffect_SiegeWarfare_BulletMgr();
		virtual ~COnlineFieldEffect_SiegeWarfare_BulletMgr();

	protected:
		// �Ѿ� ��ü
		COnlineFieldEffect_SiegeWarfare_Bullet						m_pSiegeWarfare_Bullet_List[MAX_BULLET_NUMBER];

		// �Ѿ� ��ü ����Ʈ�� ���� ��ȿ�Ѱ��� ��Ÿ���� �迭
		SHORT														m_pSiegeWarfare_Bullet_Info[MAX_BULLET_NUMBER];

	public:
		// �ʱ�ȭ
//		void Init(cltOnlineWorld *pOnlineWorld, OnlineMap *pOnlineMap);
		void Init(void);

		// �Ѿ� ��ü ����
		void SetSiegeWarfare_Bullet(SHORT ssKind, SHORT ssShooterX, SHORT ssShooterY, SHORT ssTargetX, SHORT ssTargetY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0);

		// �Ѿ� ��ü�� ó��(�̵�)
		void Action(void);

		// �Ѿ� ��ü�� �׸���
		void Draw(void);

	protected:
		// �Ѿ� ��ü ����Ʈ �߿��� �߰��� �� �ִ� �Ѿ� �ε��� ���
		SI32 GetEmptySiegeWarfareBulletIdx(void);

		// �Ѿ� ��ü ����Ʈ���� ����
		void DeleteSiegeWarfareBullet(SI32 siIndex);

		// ��� �Ѿ� ��ü ����Ʈ ����
		void DeleteSiegeWarfareBulletList(void);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLETMGR_H__863FD1B2_0CC6_4C83_8E8F_F987212790DE__INCLUDED_)
