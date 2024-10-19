#ifndef	_ONLINE_MOUSE_H
#define _ONLINE_MOUSE_H

#include <Graphics16.h>
#include "..\EnumData\MouseTypeEnum.h"

// ���콺 ����
#define	ON_MOUSE_ANI_STATUS_NORMAL			0
#define	ON_MOUSE_ANI_STATUS_DOWN			1
#define	ON_MOUSE_ANI_STATUS_UP				2
#define	ON_MOUSE_ANI_STATUS_DISABLED		3
#define	ON_MOUSE_MAX_ANI_STATUS				4	// �� ���콺 ����

// ���콺 Ŀ�� �׸��� �ɼ�
#define ON_CURSOR_DRAW_OPTION_DISABLED		0	// ��Ȱ��ȭ
#define ON_CURSOR_DRAW_OPTION_LOOP			1	// �ݺ�
#define ON_CURSOR_DRAW_OPTION_ONCE			2	// �ѹ���
#define ON_CURSOR_DRAW_OPTION_ONE_FRAME		3	// �� �� ¥�� �� ���

// ���콺 Ŀ�� �ִϸ��̼� ���
#define ON_CURSOR_ANIMATION_METHOD_NORMAL			0	// �Ϲ�
#define ON_CURSOR_ANIMATION_METHOD_REVERSE			1	// �Ųٷ�
#define ON_CURSOR_ANIMATION_METHOD_NORMAL_REVERSE	2	// �Ϲ�->�Ųٷ�

struct SMOUSE_ANI_STATUS
{
	SI16			m_ssStartFont;			// Ŀ�� ���� ��Ʈ ��ȣ
	SI16			m_ssFontNumber;			// Ŀ�� �ִϸ��̼� ��Ʈ ��
	SI08			m_scAnimationOption;	// �ִϸ��̼� �ɼ�(����, �ѹ���, ...)
	SI08			m_scAnimationMethod;	// �ִϸ��̼� ���(�Ϲ�, �Ųٷ�, �Ϲ�->�Ųٷ�, ...)
	SI08			m_ssFrameDelay;			// �ִϸ��̼� ������
};

// Ŀ�� �̹��� ���ҽ� ������
class	CCursorImageMgr
{
	public:
		CCursorImageMgr();
		~CCursorImageMgr();

	public:
		BOOL				LoadCursorImage(void);

		XSPR				*GetCursorImage(ENUM_CURSOR_IMAGE enIndex);

	private:
		XSPR				m_sprCursorImage[ON_CURSOR_IMAGE_MAX];		
};

// ���콺�� ���� Ŭ�����̴�.
class	OnlineBaseMouse
{
	protected:			
		SI16				m_siMouseAniStatus;			// ���콺�� ����.	
		SI16				m_siFont;					// ���� ��Ʈ.
		SI16				m_siCurrentFrameDelay;			
		POINT				m_ptHotSpot;
		SI08				m_scCurrentPlayDirection;	// �ִϸ��̼� �÷��� ���. normal or reverse
		bool				m_bPlayedOnce;				// �ִϸ��̼� �� �� �÷��̰� ������.
		bool				m_bEnableCursor;			// Ŀ�� ��� ����

		XSPR				*m_psprCursorImg;		// Ŀ�� �̹���

		SMOUSE_ANI_STATUS	m_sMouse_ani_status[ON_MOUSE_MAX_ANI_STATUS];
		
	public:
		OnlineBaseMouse();								// ������.
		virtual ~OnlineBaseMouse();						// �Ҹ���.
			
	public:
		VOID	Init(XSPR *psprCursorImage, SMOUSE_ANI_STATUS *psMouse_ani_status, POINT ptHotSpot);	// �ʱ�ȭ.
		VOID	Free();									// ����.

	public:
		VOID	Action();								// �׼�.
		VOID	SetMouse(bool bEnableCurosr);			// ������ ���콺�� �����Ѵ�.
		VOID	Draw(LPPOINT pptMouse);					// ��ο�
		VOID	SetAniStatus(SI16 siStatus);			// ���콺 �ִϸ��̼� ���¸� �����Ѵ�.
		SI16	GetAniStatus();							// ���콺 �ִϸ��̼� ���¸� ���´�.
		BOOL	IsDoneAnimation();						// ���콺 �ִϸ��̼��� �����°�?
};

#endif
