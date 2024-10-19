//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 입력기 상태 표시기
//	File Name		: OnlineInputStatusControl.h
//	Birth Date		: 2004. 11. 01.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		입력 상태에 대한 표시를 전달하는 클래스입니다.(한글/영문/중문 등)
//		출력 내역은 한/영 등의 입력 언어 표시, 숙어창, 발음창 등을 표시합니다.
//		출력 시 출력단의 가장 상단에 표시되어야 하는 놈입니다.
//		GS_IMEBase가 반드시 포함되어야 합니다.
//
//===================================================================================================

#ifndef	__ONLINE_INPUTSTATUSCONTROL_H__
#define	__ONLINE_INPUTSTATUSCONTROL_H__

class OnlineInputStatusControl
{
private:
	static	cltOnlineWorld*		m_pOnlineWorld;					// OnlineWorld 구조체
	
	static	XSPR				m_sprImeIconBase;				// 스프라이트 - IME 아이콘 밑바닥
	static	XSPR				m_sprImeIcon;					// 스프라이트 - IME 아이콘들
	static	XSPR				m_sprCandidate;					// 스프라이트 - 숙어창 출력용 밑바닥
	static	XSPR				m_sprReadingString;				// 스프라이트 - 발음창 출력용 밑바닥
	static	int					m_iImeIconFontNo;				// 현재 출력할 IME 아이콘의 번호

public:
	OnlineInputStatusControl	( void );
	~OnlineInputStatusControl	( void );

	static	void	Init				( cltOnlineWorld *pOnlineWorld );
	static	void	Free				( void );
	static	void	Draw				( LPDIRECTDRAWSURFACE7 lpSurface );

private:
	static	DWORD	GetCurrentLocaleIconIndex	( void );
	static	DWORD	GetCurrentImeIconIndex		( void );
};

#endif//__ONLINE_INPUTSTATUSCONTROL_H__