#ifndef	_ONLINE_MOUSE_H
#define _ONLINE_MOUSE_H

#include <Graphics16.h>
#include "..\EnumData\MouseTypeEnum.h"

// 마우스 상태
#define	ON_MOUSE_ANI_STATUS_NORMAL			0
#define	ON_MOUSE_ANI_STATUS_DOWN			1
#define	ON_MOUSE_ANI_STATUS_UP				2
#define	ON_MOUSE_ANI_STATUS_DISABLED		3
#define	ON_MOUSE_MAX_ANI_STATUS				4	// 총 마우스 상태

// 마우스 커서 그리기 옵션
#define ON_CURSOR_DRAW_OPTION_DISABLED		0	// 비활성화
#define ON_CURSOR_DRAW_OPTION_LOOP			1	// 반복
#define ON_CURSOR_DRAW_OPTION_ONCE			2	// 한번만
#define ON_CURSOR_DRAW_OPTION_ONE_FRAME		3	// 한 장 짜리 인 경우

// 마우스 커서 애니메이션 방법
#define ON_CURSOR_ANIMATION_METHOD_NORMAL			0	// 일반
#define ON_CURSOR_ANIMATION_METHOD_REVERSE			1	// 거꾸로
#define ON_CURSOR_ANIMATION_METHOD_NORMAL_REVERSE	2	// 일반->거꾸로

struct SMOUSE_ANI_STATUS
{
	SI16			m_ssStartFont;			// 커서 시작 폰트 번호
	SI16			m_ssFontNumber;			// 커서 애니메이션 폰트 수
	SI08			m_scAnimationOption;	// 애니메이션 옵션(루프, 한번만, ...)
	SI08			m_scAnimationMethod;	// 애니메이션 방법(일반, 거꾸로, 일반->거꾸로, ...)
	SI08			m_ssFrameDelay;			// 애니메이션 딜레이
};

// 커서 이미지 리소스 관리자
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

// 마우스의 기초 클래스이다.
class	OnlineBaseMouse
{
	protected:			
		SI16				m_siMouseAniStatus;			// 마우스의 상태.	
		SI16				m_siFont;					// 현재 폰트.
		SI16				m_siCurrentFrameDelay;			
		POINT				m_ptHotSpot;
		SI08				m_scCurrentPlayDirection;	// 애니메이션 플레이 방법. normal or reverse
		bool				m_bPlayedOnce;				// 애니메이션 한 번 플레이가 끝났다.
		bool				m_bEnableCursor;			// 커서 사용 가능

		XSPR				*m_psprCursorImg;		// 커서 이미지

		SMOUSE_ANI_STATUS	m_sMouse_ani_status[ON_MOUSE_MAX_ANI_STATUS];
		
	public:
		OnlineBaseMouse();								// 생성자.
		virtual ~OnlineBaseMouse();						// 소멸자.
			
	public:
		VOID	Init(XSPR *psprCursorImage, SMOUSE_ANI_STATUS *psMouse_ani_status, POINT ptHotSpot);	// 초기화.
		VOID	Free();									// 해제.

	public:
		VOID	Action();								// 액션.
		VOID	SetMouse(bool bEnableCurosr);			// 새로이 마우스를 설정한다.
		VOID	Draw(LPPOINT pptMouse);					// 드로우
		VOID	SetAniStatus(SI16 siStatus);			// 마우스 애니메이션 상태를 설정한다.
		SI16	GetAniStatus();							// 마우스 애니메이션 상태를 얻어온다.
		BOOL	IsDoneAnimation();						// 마우스 애니메이션이 끝났는가?
};

#endif
