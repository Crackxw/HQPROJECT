//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상]
//	File Name		: OnlineBanner.h
//	Birth Date		: 2004. 02. 25.
//	Creator			: 이 윤 석
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용하지 않는 소스입니다.
//		=== Disable Source Files Comment ===
//		기존 소스에서 이 클래스를 사용하는 필수 부분이 전부 주석처리 되어 있었습니다.
//		따라서 실행파일의 규모를 줄이기 위해 해당 코드를 주석처리합니다.
//		나중에 필요한 부분이 있거나 이것 때문에 문제가 발생할 경우 다시 복구시키면 됩니다.
//
//===================================================================================================

/************************************************************************************************
	파일명 : OnlineBanner.h
	담당자 : 이윤석
************************************************************************************************/

/*
#ifndef	___ONLINE_BANNER___
#define ___ONLINE_BANNER___


typedef struct
{
	XSPR				m_BannerImage;
	SI32				m_Delay;			// Banner가 떠 있는 시간, 단위는 초이다.
	SI32				m_FrameDelay;

	char				m_URL[256];
} SBannerData;


// Banner Class
// Banner 0번은 기본 Banner이며, 다른 Banner가 떠 있지 않을때는 계속 떠 있는다.
// Banner 0번을 제외하고는, 1분마다 한번씩 Banner를 순서되로 뛰운다. 뛰운지 Delay만큼 시간이 지나면, 나머지 시간은 기본 Banner로 바꿘다.
class	OnlineBanner
{
private:
	cltOnlineWorld*		m_lpcltOnlineWorld;

	SI32				m_BannerDataCount;
	SBannerData*		m_BannerData;

	SI32				m_NowBanner;
	DWORD				m_BannerShowTimeLast;

	BOOL				m_bMouseDown;

public:
	OnlineBanner();																// 생성자.
	virtual	~OnlineBanner();													// 소멸자.
		
	virtual	BOOL		Init(cltOnlineWorld* lpcltOnlineWorld);					// 초기화
	virtual	VOID		Free(void);												// 해제.

	virtual BOOL		Poll(void);												// Poll
	virtual BOOL		Draw(LPDIRECTDRAWSURFACE7 pSurface);					// Draw
};	


#endif
*/