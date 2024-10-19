// OnlineFieldEffect_SiegeWarfare_Bullet.h: interface for the COnlineFieldEffect_SiegeWarfare_Bullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLET_H__C8967582_113E_49E8_936F_5B2E0B5B87FE__INCLUDED_)
#define AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLET_H__C8967582_113E_49E8_936F_5B2E0B5B87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <GSL.h>

#include <main.h>

#include "..\GCOMMON\bullet.h"	// class _BulletKindInfo 사용을 위한 선언

// 공성전 총알 추가는 기존의 전투용 총알 추가와 동일한 방법으로 _BulletKindInfo를 초기화 하는
// InitBulletInfo()에서 추가해주면 된다.

// robypark 2004/10/22 10:47
// 공성전 필드 전투에서 공성전 유닛이 발사하는 총알 객체 클래스
// 전투에서 사용하는 class _Bullet를 거의 그대로 사용한다.
class COnlineFieldEffect_SiegeWarfare_Bullet  
{
	public:
		COnlineFieldEffect_SiegeWarfare_Bullet();
		virtual ~COnlineFieldEffect_SiegeWarfare_Bullet();

	protected:
		SHORT		m_ssAnimation;									// 어떤 종류의 애니메이션을 수행하는가?
		SHORT		m_ssDrawSwitch;									// 그려줄 것인가 말것인가?
		SHORT		m_ssReverseSwitch;								// 뒤집어서 찍을 것인가?
		SHORT		m_ssTransparentSwitch;
		SHORT		m_ssHeightSwitch;
		SHORT		m_ssDrawStartX, m_ssDrawStartY;					// 그림이 그려지는 시작 위치 
		SHORT		m_ssDrawXsize,  m_ssDrawYsize;					// 그림의 가로 세로 사이즈 
		SHORT		m_ssFile, m_ssFont;
		SHORT		m_ssAniStep;
		char		m_scRealImageXPercent, m_scRealImageYPercent;  // 실제 이미지가 가로 세로로 각 몇 %를 점하는가?
		SHORT		m_ssRealImageStartX, m_ssRealImageEndX;        // 실제 이미지가 점하는 가로 세로 좌표 
		SHORT		m_ssRealImageStartY, m_ssRealImageEndY; 

		SHORT		m_ssCurrentOrder;								// 현재 캐릭터가 하고 있는 작업 

		SHORT		m_ssID;											// 캐릭터의 식별 번호 
		SHORT		m_ssKind;										// 어떤 종류의 obj인가?
		SHORT		m_ssCenterX, m_ssCenterY;

		SHORT		m_ssStepWidth;									// 이동 단위

		int			m_siTargetDirection;							// 목표의 방위 
		int			m_siTempDirection;								// 16방위로 존재하는가?

		SI32		m_ssX, m_ssY;									// obj의 중심 위치 

		// 이동관련 
		char		m_scMoveFrameNumber;
		SHORT		m_ssMoveFile[9], m_ssMoveFont[9];

		SHORT		m_ssRealX, m_ssRealY;
		SHORT		m_ssRealDX, m_ssRealDY;
		SHORT		m_ssMapSX, m_ssMapSY;
		SHORT		m_ssMapDX, m_ssMapDY;
		SHORT		m_ssOriginalRealDX, m_ssOriginalRealDY;

		SHORT		m_ssCurveBulletRealX, m_ssCurveBulletRealY;
		SHORT		m_ssOldCurveBulletRealX, m_ssOldCurveBulletRealY;
		SHORT		m_ssCurveBulletDrawX, m_ssCurveBulletDrawY;
		SHORT		m_ssCurveSwitch;							// 곡선 이동 총알인가?

		SHORT		m_ssCurveRate;								// 얼마나 곡선을 이루는가?

		SHORT		m_ssMoveStep;
		SHORT		m_ssMaxMoveStep;

		SHORT		m_ssTailSwitch;								// 꼬리를 만들것인지 여부를 알려주는 변수 
		SHORT		m_ssChaseSwitch;							// 추적할 것인가 말것인가?
		SHORT		m_ssChaseRepeatNumber;
		SHORT		m_ssVaryX, m_ssVaryY;

		// 총알 이동 경로 좌표
		SHORT		m_ssCourseX[MAX_SHOOT_RANGE],m_ssCourseY[MAX_SHOOT_RANGE];

		// robypark (2004/5/21 18:44) 화룡출수, 발석거 불 붙은 돌 애니메이션 보간을 위한 추가
		//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
		//		따라 프레임 스킵이 필요하다.
		// 이동 단계에 따른 프레임 번호를 저장하고 있다.
		SHORT		m_snFrameIndex[MAX_SHOOT_RANGE];

		SHORT		m_ssCurrentAnimationDelay;		// 애니메이션 시작 지연 시간의 경과된 값
		SHORT		m_ssAnimationDelay;				// 애니메이션 시작 지연 시간

		// robypark 2004/12/20 17:59
		UI16		m_uiTargetUniqueID;		// 공성전 중에 공격에 대한 Action Msg를 서버에 보내야하는지를 나타내는 플래그이자 
											// 보내야하는 피격자(피격 마을)의 ID
		UI16		m_uiTargetType;			// 피격 대상이 마을인지, 캐릭터인지를 나타낸다.

	public:
		void Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT targetidir, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0);

		// robypark (2004/5/21 18:44) 화룡출수 애니메이션 보간을 위한 추가
		//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
		//		따라 프레임 스킵이 필요하다.
		//  이동 단계에 따른 애니메이션 프레임을 설정한다.
		void CreateFrameIndex_4FireDragonMissile(void);

		// 찍을 위치를 계산한다.
		void CalculateDrawStartXY();

		BOOL CalculateRealXY();
		// 찍어야 할 그림을 선정한다. 
		void DecideFont();
		void DecideFontMove();
		void DecideFontMoveNormal();
		void DecideFontMoveFireTankArrow();

		void SetMoveImage16(SHORT file, SHORT font, SHORT frame);
		void SetMoveImage(SHORT ani, SHORT file, SHORT font);

		void GetCourse();
		BOOL Draw();
		BOOL Action();
		BOOL Hit();
		BOOL HitNormal();

		BOOL HitFragment1(); 

		void SetFileNFont();

		void DecideDirection(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
		void DecideDirectionFireTankArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
		void DecideDirectionNormal(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_BULLET_H__C8967582_113E_49E8_936F_5B2E0B5B87FE__INCLUDED_)
