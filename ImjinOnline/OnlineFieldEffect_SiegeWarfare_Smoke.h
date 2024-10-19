// OnlineFieldEffect_SiegeWarfare_Smoke.h: interface for the COnlineFieldEffect_SiegeWarfare_Smoke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKE_H__F8BC5919_7BA7_405A_B9E6_35B4129FD28C__INCLUDED_)
#define AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKE_H__F8BC5919_7BA7_405A_B9E6_35B4129FD28C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <GSL.h>

#include <main.h>

#include "..\GCOMMON\smoke.h"	// class _SmokeKindInfo 사용을 위한 선언

// 공성전 Smoke 추가는 기존의 전투용 Smoke 추가와 동일한 방법으로 _SmokeKindInfo를 초기화 하는
// InitSmokeInfo()에서 추가해주면 된다.

// robypark 2004/10/22 14:47
// 온라인 필드 전투 이펙트 - 공성전 중의 특수 효과. 폭발, 명중 히트 효과
class COnlineFieldEffect_SiegeWarfare_Smoke  
{
	public:
		COnlineFieldEffect_SiegeWarfare_Smoke();
		virtual ~COnlineFieldEffect_SiegeWarfare_Smoke();

	protected:
		SHORT				m_ssID;
		SHORT				m_ssKind;
		SHORT				m_ssAnimation;
		SHORT				m_ssStep;
		SHORT				m_ssFile, m_ssFont;
		SI32				m_siStepDelay;					// 애니메이션의 속도.
		SHORT				m_ssCurrentOrder;				// 현재 캐릭터가 하고 있는 작업 

		SI32				m_ssX, m_ssY;

		char				m_scDrawSwitch;					// 그려줄 것인가 말것인가?
		char				m_scReverseSwitch;				// 뒤집어서 찍을 것인가?
		SHORT				m_ssDrawXsize, m_ssDrawYsize;	// 그림의 가로 세로 사이즈 
		SHORT				m_ssDrawStartX, m_ssDrawStartY;	// 그림이 그려지는 시작 위치 
		SHORT				m_ssCenterX, m_ssCenterY;
		SI32				m_ssTransparentMode;			// 투명효과를 넣을 것인가 ?
		DWORD				m_uiLastChangedFrame;           // 애니메이션이 변경된 프레임 

		// 이동관련 
		char				m_scMoveFrameNumber;
 
		SHORT				m_ssRealX, m_ssRealY;
		SHORT				m_ssStartFont;

		SHORT				m_ssMaxRepeatNumber;
		SHORT				m_ssRepeatNumber;

		SHORT				m_ssRisingSwitch;
		
		SHORT				m_ssCurrentAnimationDelay;		// 애니메이션 시작 지연 시간의 경과된 값
		SHORT				m_ssAnimationDelay;				// 애니메이션 시작 지연 시간

		// robypark 2004/12/17 17:3
		// 효과음 Index
		SI32				m_siEffectSndIdx;

		// 효과음만 지연 시간 후에 연주하며 그림은 그리지 않는지를 나타내는 플래그
		bool				m_bSoundDelayor;

		// robypark 2004/12/20 17:59
		UI16				m_uiTargetUniqueID;		// 공성전 중에 공격에 대한 Action Msg를 서버에 보내야하는지를 나타내는 플래그이자 
													// 보내야하는 피격자(피격 마을)의 ID
		UI16				m_uiTargetType;			// 피격 대상이 마을인지, 캐릭터인지를 나타낸다.

		// robypark 2004/12/29 16:8
		// Smoke 타일 내 그려지는 위치 보정하기. 항상 같은 자리에만 그려지지 않도록하기 위함.
		SI08				m_scDrawPositionInTileX;
		SI08				m_scDrawPositionInTileY;

		// robypark 2005/1/14 18:39
		// 그릴 때 캐릭터 중심에 그려지도록 하기 위한 변수로 해당 캐릭터 UniqueID
		UI16				m_uiSmokeEffectCharUniqueID;

	public:
		void Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0, SI32 siEffectSndIdx = 0, bool bSoundDelayor = false);
		BOOL Action();
		BOOL Draw();

		// 찍을 위치를 계산한다. 
		void CalculateRealXY();
		void CalculateRealXYNormal();

		void CalculateDrawStartXY();

		void DecideFont();
		void DecideFontNormal();

		// robypark 2004/12/29 16:8
		// Smoke 타일 내 그려지는 위치 보정하기. 항상 같은 자리에만 그려지지 않도록하기 위함.
		void SetSiegeWarfare_DrawPositionRandomizeInTile(SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY);

		// robypark 2005/1/14 18:39
		// 그릴 때 캐릭터 중심에 그려지도록 설정하기
		void SetSiegeWarfare_SetDrawInCharacterCenterPos(UI16 uiUniqueID);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKE_H__F8BC5919_7BA7_405A_B9E6_35B4129FD28C__INCLUDED_)
