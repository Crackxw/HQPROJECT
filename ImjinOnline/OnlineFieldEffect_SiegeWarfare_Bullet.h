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

#include "..\GCOMMON\bullet.h"	// class _BulletKindInfo ����� ���� ����

// ������ �Ѿ� �߰��� ������ ������ �Ѿ� �߰��� ������ ������� _BulletKindInfo�� �ʱ�ȭ �ϴ�
// InitBulletInfo()���� �߰����ָ� �ȴ�.

// robypark 2004/10/22 10:47
// ������ �ʵ� �������� ������ ������ �߻��ϴ� �Ѿ� ��ü Ŭ����
// �������� ����ϴ� class _Bullet�� ���� �״�� ����Ѵ�.
class COnlineFieldEffect_SiegeWarfare_Bullet  
{
	public:
		COnlineFieldEffect_SiegeWarfare_Bullet();
		virtual ~COnlineFieldEffect_SiegeWarfare_Bullet();

	protected:
		SHORT		m_ssAnimation;									// � ������ �ִϸ��̼��� �����ϴ°�?
		SHORT		m_ssDrawSwitch;									// �׷��� ���ΰ� �����ΰ�?
		SHORT		m_ssReverseSwitch;								// ����� ���� ���ΰ�?
		SHORT		m_ssTransparentSwitch;
		SHORT		m_ssHeightSwitch;
		SHORT		m_ssDrawStartX, m_ssDrawStartY;					// �׸��� �׷����� ���� ��ġ 
		SHORT		m_ssDrawXsize,  m_ssDrawYsize;					// �׸��� ���� ���� ������ 
		SHORT		m_ssFile, m_ssFont;
		SHORT		m_ssAniStep;
		char		m_scRealImageXPercent, m_scRealImageYPercent;  // ���� �̹����� ���� ���η� �� �� %�� ���ϴ°�?
		SHORT		m_ssRealImageStartX, m_ssRealImageEndX;        // ���� �̹����� ���ϴ� ���� ���� ��ǥ 
		SHORT		m_ssRealImageStartY, m_ssRealImageEndY; 

		SHORT		m_ssCurrentOrder;								// ���� ĳ���Ͱ� �ϰ� �ִ� �۾� 

		SHORT		m_ssID;											// ĳ������ �ĺ� ��ȣ 
		SHORT		m_ssKind;										// � ������ obj�ΰ�?
		SHORT		m_ssCenterX, m_ssCenterY;

		SHORT		m_ssStepWidth;									// �̵� ����

		int			m_siTargetDirection;							// ��ǥ�� ���� 
		int			m_siTempDirection;								// 16������ �����ϴ°�?

		SI32		m_ssX, m_ssY;									// obj�� �߽� ��ġ 

		// �̵����� 
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
		SHORT		m_ssCurveSwitch;							// � �̵� �Ѿ��ΰ�?

		SHORT		m_ssCurveRate;								// �󸶳� ��� �̷�°�?

		SHORT		m_ssMoveStep;
		SHORT		m_ssMaxMoveStep;

		SHORT		m_ssTailSwitch;								// ������ ��������� ���θ� �˷��ִ� ���� 
		SHORT		m_ssChaseSwitch;							// ������ ���ΰ� �����ΰ�?
		SHORT		m_ssChaseRepeatNumber;
		SHORT		m_ssVaryX, m_ssVaryY;

		// �Ѿ� �̵� ��� ��ǥ
		SHORT		m_ssCourseX[MAX_SHOOT_RANGE],m_ssCourseY[MAX_SHOOT_RANGE];

		// robypark (2004/5/21 18:44) ȭ�����, �߼��� �� ���� �� �ִϸ��̼� ������ ���� �߰�
		//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
		//		���� ������ ��ŵ�� �ʿ��ϴ�.
		// �̵� �ܰ迡 ���� ������ ��ȣ�� �����ϰ� �ִ�.
		SHORT		m_snFrameIndex[MAX_SHOOT_RANGE];

		SHORT		m_ssCurrentAnimationDelay;		// �ִϸ��̼� ���� ���� �ð��� ����� ��
		SHORT		m_ssAnimationDelay;				// �ִϸ��̼� ���� ���� �ð�

		// robypark 2004/12/20 17:59
		UI16		m_uiTargetUniqueID;		// ������ �߿� ���ݿ� ���� Action Msg�� ������ �������ϴ����� ��Ÿ���� �÷������� 
											// �������ϴ� �ǰ���(�ǰ� ����)�� ID
		UI16		m_uiTargetType;			// �ǰ� ����� ��������, ĳ���������� ��Ÿ����.

	public:
		void Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT targetidir, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0);

		// robypark (2004/5/21 18:44) ȭ����� �ִϸ��̼� ������ ���� �߰�
		//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
		//		���� ������ ��ŵ�� �ʿ��ϴ�.
		//  �̵� �ܰ迡 ���� �ִϸ��̼� �������� �����Ѵ�.
		void CreateFrameIndex_4FireDragonMissile(void);

		// ���� ��ġ�� ����Ѵ�.
		void CalculateDrawStartXY();

		BOOL CalculateRealXY();
		// ���� �� �׸��� �����Ѵ�. 
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
