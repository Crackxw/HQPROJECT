//////////////////////////////////////////////////////////////////////
//
// ������ ������ �� ȭ�� ���� ��ܿ� �������� �������̽�
// ��������, ���� ���, ���� ��� ���, ������ ��ư ��
//
// SiegeWarfareUnitUI.h: interface for the CSiegeWarfareUnitUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARFAREUNITUI_H__F380C79B_25D4_433F_8268_DEABF2F2EA9E__INCLUDED_)
#define AFX_SIEGEWARFAREUNITUI_H__F380C79B_25D4_433F_8268_DEABF2F2EA9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<GSL.h>
#include	"GSCDefine.h"
#include	<DDraw.h>
#include	"button.h"
#include	<graphics16.h>

class cltOnlineWorld;
class OnlineMsgBox;

class CSiegeWarfareUnitUI  
{
	public:
		CSiegeWarfareUnitUI();
		virtual ~CSiegeWarfareUnitUI();

	protected:
		// �ʱ�ȭ
		void	Init();

	public:
		// �ʱ�ȭ
		void	Init(cltOnlineWorld *pOnlineWorld);

		// ���� ���̱� ����
		void	ShowWindow(DWORD dwShowWindow);

		// ó��
		BOOL	Action(void);

		// �׸���
		void	Draw(LPDIRECTDRAWSURFACE7 pSurface);

		// �׸���
		void	DrawImage(LPDIRECTDRAWSURFACE7 pSurface);

		// �׸���
		void	DrawText(LPDIRECTDRAWSURFACE7 pSurface);

		// ���� ���̴°�?
		BOOL	IsVisibleWindow(void);

		// ������ �ּ�ȭ �Ǿ��°�?
		BOOL	IsMinimizeWindow(void);

	public:
		// ���� ����
		BOOL	SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow);

		// ���� �ı�
		BOOL	SiegeWarfareUnitUI_DestroyWindow(void);

	protected:
		// ��ư ����
		void	UpdateButton(void);

		// ���ҽ� �б�
		void	LoadResource(void);

		// ���ҽ� ����
		void	FreeResource(void);

	public:
		// �������̽� ��� ����� �� �־����� ������ ����
		void	SetCurrentDelayTimeUsingVillageAttackType(SI32 siDelay);
		void	SetMaxDelayTimeUsingVillageAttackType(SI32 siDelay);

		void	SetVillageCode(UI16 uiVillageCode);			// ���� �ڵ�

	protected:
		cltOnlineWorld		*m_pMyOnlineWorld;
		DWORD				m_dwShowWindow;				// ���� ���̱� �ɼ�

		BOOL				m_bCreatedWindow;			// ���� �����Ǿ��°�?

		BOOL				m_bDrawManaGauge;			// ���� �������� �׷��� �ϳ�?

		// �̹���
		XSPR				m_sprMainInterface;			// �������̽� ���� ��� �̹���
		XSPR				m_sprShowUnitInformation;	// �������̽� ��ư �̹��� - ��� ���� ���� ���� ����
		XSPR				m_sprFunctionBtnImage;		// �������̽� ��ư �̹��� - ��� ��ư
		XSPR				m_sprEnterVillage;			// �������̽� ��ư �̹��� - ���� �������̽��� ���� ������ ����
		XSPR				m_sprTimer;					// �������̽����� ���� ����� ����� �� �ִ����� ǥ���ϴ� Ÿ�̸� �̹���

		XSPR				m_sprStatusMain;			// ���� ���� ���� ���� ���� ��� �̹���
		XSPR				m_sprGaugeRed;				// ������ red
		XSPR				m_sprGaugeBlue;				// ������ blue
		XSPR				m_sprBtnMinimize;			// �ּ�ȭ ��ư �̹���
		XSPR				m_sprPortrait;				// �ʻ�ȭ �̹���

		// ��ư
		_Button				m_btnShowUnitInformation;	// ���� �������̽� ��ư - ��� ���� ���� ���� ���� ��� ��ư
		_Button				m_btnShowEmemyGuildList;	// ���� �������̽� ��ư - ���� ���� ��� ����
		_Button				m_btnDefenceUp;				// ���� �������̽� ��ư - ���� ��� ��ư(���� ���)
		_Button				m_btnEnterVillage;			// ���� �������̽� ��ư - ���� �������̽��� ���� ������ ����

		_Button				m_btnToNormalChar;			// ���� ���ֿ��� �Ϲ� �������� ���� ��û ��ư

		_Button				m_btnMinimize;				// ���� �ּ�ȭ ��ư
		_Button				m_btnRestore;				// ���� ���̱� ���� ��ư

		// �������̽� ��ġ�� ���� No Field Area Handle
		SI32				m_siNoFieldAreaHandle;
		SI32				m_siNoFieldAreaHandle_BtnMinimize;

		// �������̽� ��� ����� �� �־����� ������
		SI32				m_siCurrentDelayTimeUsingVillageAttackType;
		SI32				m_siMaxDelayTimeUsingVillageAttackType;

		UI16				m_uiVillageCode;			// ���� �ڵ�

		char				m_szArmyKindName[64];		// ������ ���� ���� �̸�

		BOOL				m_bMouseDown;

		OnlineMsgBox		*m_pConfirmToGiveUpBattleMsgBox;	// ���� ���� ���⸦ ���ϴ��� ���� ��ȭ ����
};

#endif // !defined(AFX_SIEGEWARFAREUNITUI_H__F380C79B_25D4_433F_8268_DEABF2F2EA9E__INCLUDED_)
