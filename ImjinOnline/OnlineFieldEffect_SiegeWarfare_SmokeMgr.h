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
// 온라인 필드 전투 이펙트 - 공성전 중의 특수 효과. 폭발, 명중 히트 효과 관리자
class COnlineFieldEffect_SiegeWarfare_SmokeMgr  
{
	public:
		COnlineFieldEffect_SiegeWarfare_SmokeMgr();
		virtual ~COnlineFieldEffect_SiegeWarfare_SmokeMgr();

	protected:
		// 스모크(효과) 객체 리스트
		COnlineFieldEffect_SiegeWarfare_Smoke				m_pSiegeWarfare_Smoke_List[MAX_SMOKE_NUMBER];

		// 스모크(효과) 정보 리스트
		// 사용하는 것인지 검사
		SHORT												m_pSiegeWarfare_Smoke_Info[MAX_SMOKE_NUMBER];

	public:
		// 초기화
		void Init();

		// Smoke 객체 생성
		SI32 SetSiegeWarfare_Smoke(SHORT ssKind, SHORT ssSmokeX, SHORT ssSmokeY, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0, SI32 siEffectSndIdx = 0, bool bSoundDelayor = false);

		// robypark 2004/12/29 16:8
		// Smoke 타일 내 그려지는 위치 보정
		void SetSiegeWarfare_DrawPositionRandomizeInTile(SI32 siSmokeIndex, SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY);

		// robypark 2005/1/14 18:39
		// 그릴 때 캐릭터 중심에 그려지도록 설정하기
		void SetSiegeWarfare_SetDrawInCharacterCenterPos(SI32 siSmokeIndex, UI16 uiUniqueID);

		// Smoke 객체들 처리(이동)
		void Action(void);

		// Smoke 객체들 그리기
		void Draw(void);

	protected:
		// Smoke 객체 리스트 중에서 추가할 수 있는 Smoke 인덱스 얻기
		SI32 GetEmptySiegeWarfareSmokeIdx(void);

		// Smoke 객체 리스트에서 삭제
		void DeleteSiegeWarfareSmoke(SI32 siIndex);

		// 모든 Smoke 객체 리스트 삭제
		void DeleteSiegeWarfareSmokeList(void);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKEMGR_H__26A4EF4F_0006_4247_8885_1B35E12CFCC2__INCLUDED_)
