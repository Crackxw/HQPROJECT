//////////////////////////////////////////////////////////////////////
//
// ������ �������� ������ �� �����ϰ��� �ϴ� ������ �����ִ� �������̽�
//
// SiegeWarfareTransformUIWnd.h: interface for the CSiegeWarfareTransformUIWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARFARETRANSFORMUIWND_H__A1E4CC32_2696_43C6_917F_0B16907FBCF4__INCLUDED_)
#define AFX_SIEGEWARFARETRANSFORMUIWND_H__A1E4CC32_2696_43C6_917F_0B16907FBCF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<GSL.h>
#include	"GSCDefine.h"
#include	<DDraw.h>
#include	"button.h"
#include	<graphics16.h>

class cltOnlineWorld;

struct SAnimationButton
{
	RECT		m_rtDraw;					// ��ư�� �׷��� ��ġ
	XSPR		m_sprAnimation;				// �׷��� image
	SI08		m_scCurrentAnimationIdx;	// ���� �׷����� �ִϸ��̼� �ε���
	SI16		m_ssCurrentFont;			// ���� �׷����� ��Ʈ ��ȣ
	SI08		m_scCurrentDelay;			// �ִϸ��̼� ������
	SI08		m_scGrayed;					// ��Ȱ��ȭ ����
};

class CSiegeWarfareTransformUIWnd  
{
	public:
		CSiegeWarfareTransformUIWnd();
		virtual ~CSiegeWarfareTransformUIWnd();

	public:
		// �ʱ�ȭ
		void	Init(cltOnlineWorld *pOnlineWorld);

		// ���� ���̱� ����
		BOOL	ShowWindow(BOOL bVisibleWindow);

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

		// ������ ��� ���� ���� ����Ʈ ��û ����
		// ON_RESPONSE_LIST_GUILD_UNIT
		UI16	OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	protected:
		// ��ư ����
		void	UpdateButton(void);

		// ���ҽ� �б�
		void	LoadResource(void);

		// ���ҽ� ����
		void	FreeResource(void);

		// ��ư �ִϸ��̼� ����
		void	UpdateAnimation(void);

	public:
		BOOL	IsSendedMsg(void);			// ���� �޽����� ������ �����ߴ°�?

		void	SetSendedMsg(BOOL bSendedMsg);	// ���� �޽����� ������ �����ߴ°��� ����

	protected:
		cltOnlineWorld						*m_pMyOnlineWorld;
		BOOL								m_bVisibleWindow;
		BOOL								m_bSendedMsg;			// ���� �޽����� ������ �����ߴ°�?

		SI32								m_siX;
		SI32								m_siY;

		// ���� ���� ����Ʈ
		SI32								m_siGuildCount[ON_MAX_GUILD_UNIT];			// ��� ���� �����ϴ� ���� ���� ��. ���� ����
		BOOL								m_bDownloadedGuildCount;					// ��� �� ���� ���� ����Ʈ�� �޾ƿԴ°�?

		// No Field Area Idx
		SI32								m_siNoFieldAreaIdx;
/*
		// �����Ϸ��� ���� ĳ���� �̹��� surface�� Stretch ����� ����
		LPDIRECTDRAWSURFACE7				m_pSurfaceSprArmy;

		// ������ �����̽����� ���� �̹������� �� ��Ʈ�� �����ϴ� ũ��
		SI32								m_siPortraitWidth;
		SI32								m_siPortraitHeight;

		// �����̽��� ���� ����(ũ�� �� ��ġ)
		RECT								m_rtSurfaceArmyKind[ON_MAX_GUILD_UNIT];
*/
		// ���� ���� �ؽ�Ʈ �ڽ�
		RECT								m_rtArmyDesc;

		// ��ư��
		_Button								m_btnConfirm;					// ����(Ȯ��)
		_Button								m_btnCancel;					// ���

		// �̹���
		XSPR								m_sprBackground;				// ��� �̹���
		XSPR								m_sprOkCancel;					// ����(Ȯ��), ��� ��ư �̹���
		XSPR								m_sprSelect;					// ������ ���� ���� ���׸�

		// ������ ���� �ε���
		SI32								m_siSelectArmyKindIdx;
		SI32								m_siPrevSelectArmyKindIdx;

		// Ŀ�� ���� �ε���
		SI32								m_siHoverArmyKindIdx;

		// �׷��� ���� ����(ũ�� �� ��ġ)
//		RECT								m_rtArmyKind[ON_MAX_GUILD_UNIT];

		// ���� Ÿ��Ʋ ���ڿ�
		char								m_szTitle[256];

		BOOL								m_bMouseDown;

		SAnimationButton					m_sAnimationButton[ON_MAX_GUILD_UNIT];
};

#endif // !defined(AFX_SIEGEWARFARETRANSFORMUIWND_H__A1E4CC32_2696_43C6_917F_0B16907FBCF4__INCLUDED_)
