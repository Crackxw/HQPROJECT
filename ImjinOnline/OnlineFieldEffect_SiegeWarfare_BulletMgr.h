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
// 공성전 필드 전투에서 공성전 유닛이 발사하는 총알 객체 관리 클래스
class COnlineFieldEffect_SiegeWarfare_BulletMgr  
{
	public:
		COnlineFieldEffect_SiegeWarfare_BulletMgr();
		virtual ~COnlineFieldEffect_SiegeWarfare_BulletMgr();

	protected:
		// 총알 객체
		COnlineFieldEffect_SiegeWarfare_Bullet						m_pSiegeWarfare_Bullet_List[MAX_BULLET_NUMBER];

		// 총알 객체 리스트의 값이 유효한가를 나타내는 배열
		SHORT														m_pSiegeWarfare_Bullet_Info[MAX_BULLET_NUMBER];

	public:
		// 초기화
//		void Init(cltOnlineWorld *pOnlineWorld, OnlineMap *pOnlineMap);
		void Init(void);

		// 총알 객체 생성
		void SetSiegeWarfare_Bullet(SHORT ssKind, SHORT ssShooterX, SHORT ssShooterY, SHORT ssTargetX, SHORT ssTargetY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0);

		// 총알 객체들 처리(이동)
		void Action(void);

		// 총알 객체들 그리기
		void Draw(void);

	protected:
		// 총알 객체 리스트 중에서 추가할 수 있는 총알 인덱스 얻기
		SI32 GetEmptySiegeWarfareBulletIdx(void);

		// 총알 객체 리스트에서 삭제
		void DeleteSiegeWarfareBullet(SI32 siIndex);

		// 모든 총알 객체 리스트 삭제
		void DeleteSiegeWarfareBulletList(void);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLETMGR_H__863FD1B2_0CC6_4C83_8E8F_F987212790DE__INCLUDED_)
