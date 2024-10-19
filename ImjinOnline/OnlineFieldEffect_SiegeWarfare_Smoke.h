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

#include "..\GCOMMON\smoke.h"	// class _SmokeKindInfo ����� ���� ����

// ������ Smoke �߰��� ������ ������ Smoke �߰��� ������ ������� _SmokeKindInfo�� �ʱ�ȭ �ϴ�
// InitSmokeInfo()���� �߰����ָ� �ȴ�.

// robypark 2004/10/22 14:47
// �¶��� �ʵ� ���� ����Ʈ - ������ ���� Ư�� ȿ��. ����, ���� ��Ʈ ȿ��
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
		SI32				m_siStepDelay;					// �ִϸ��̼��� �ӵ�.
		SHORT				m_ssCurrentOrder;				// ���� ĳ���Ͱ� �ϰ� �ִ� �۾� 

		SI32				m_ssX, m_ssY;

		char				m_scDrawSwitch;					// �׷��� ���ΰ� �����ΰ�?
		char				m_scReverseSwitch;				// ����� ���� ���ΰ�?
		SHORT				m_ssDrawXsize, m_ssDrawYsize;	// �׸��� ���� ���� ������ 
		SHORT				m_ssDrawStartX, m_ssDrawStartY;	// �׸��� �׷����� ���� ��ġ 
		SHORT				m_ssCenterX, m_ssCenterY;
		SI32				m_ssTransparentMode;			// ����ȿ���� ���� ���ΰ� ?
		DWORD				m_uiLastChangedFrame;           // �ִϸ��̼��� ����� ������ 

		// �̵����� 
		char				m_scMoveFrameNumber;
 
		SHORT				m_ssRealX, m_ssRealY;
		SHORT				m_ssStartFont;

		SHORT				m_ssMaxRepeatNumber;
		SHORT				m_ssRepeatNumber;

		SHORT				m_ssRisingSwitch;
		
		SHORT				m_ssCurrentAnimationDelay;		// �ִϸ��̼� ���� ���� �ð��� ����� ��
		SHORT				m_ssAnimationDelay;				// �ִϸ��̼� ���� ���� �ð�

		// robypark 2004/12/17 17:3
		// ȿ���� Index
		SI32				m_siEffectSndIdx;

		// ȿ������ ���� �ð� �Ŀ� �����ϸ� �׸��� �׸��� �ʴ����� ��Ÿ���� �÷���
		bool				m_bSoundDelayor;

		// robypark 2004/12/20 17:59
		UI16				m_uiTargetUniqueID;		// ������ �߿� ���ݿ� ���� Action Msg�� ������ �������ϴ����� ��Ÿ���� �÷������� 
													// �������ϴ� �ǰ���(�ǰ� ����)�� ID
		UI16				m_uiTargetType;			// �ǰ� ����� ��������, ĳ���������� ��Ÿ����.

		// robypark 2004/12/29 16:8
		// Smoke Ÿ�� �� �׷����� ��ġ �����ϱ�. �׻� ���� �ڸ����� �׷����� �ʵ����ϱ� ����.
		SI08				m_scDrawPositionInTileX;
		SI08				m_scDrawPositionInTileY;

		// robypark 2005/1/14 18:39
		// �׸� �� ĳ���� �߽ɿ� �׷������� �ϱ� ���� ������ �ش� ĳ���� UniqueID
		UI16				m_uiSmokeEffectCharUniqueID;

	public:
		void Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, UI16 uiTargetUniqueID, UI16 uiTargetType, SHORT ssAnimationDelay = 0, SI32 siEffectSndIdx = 0, bool bSoundDelayor = false);
		BOOL Action();
		BOOL Draw();

		// ���� ��ġ�� ����Ѵ�. 
		void CalculateRealXY();
		void CalculateRealXYNormal();

		void CalculateDrawStartXY();

		void DecideFont();
		void DecideFontNormal();

		// robypark 2004/12/29 16:8
		// Smoke Ÿ�� �� �׷����� ��ġ �����ϱ�. �׻� ���� �ڸ����� �׷����� �ʵ����ϱ� ����.
		void SetSiegeWarfare_DrawPositionRandomizeInTile(SI08 scDrawPositionInTileX, SI08 scDrawPositionInTileY);

		// robypark 2005/1/14 18:39
		// �׸� �� ĳ���� �߽ɿ� �׷������� �����ϱ�
		void SetSiegeWarfare_SetDrawInCharacterCenterPos(UI16 uiUniqueID);
};

#endif // !defined(AFX_ONLINEFIELDEFFECT_SIEGEWARFARE_SMOKE_H__F8BC5919_7BA7_405A_B9E6_35B4129FD28C__INCLUDED_)
