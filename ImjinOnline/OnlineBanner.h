//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�]
//	File Name		: OnlineBanner.h
//	Birth Date		: 2004. 02. 25.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ������� �ʴ� �ҽ��Դϴ�.
//		=== Disable Source Files Comment ===
//		���� �ҽ����� �� Ŭ������ ����ϴ� �ʼ� �κ��� ���� �ּ�ó�� �Ǿ� �־����ϴ�.
//		���� ���������� �Ը� ���̱� ���� �ش� �ڵ带 �ּ�ó���մϴ�.
//		���߿� �ʿ��� �κ��� �ְų� �̰� ������ ������ �߻��� ��� �ٽ� ������Ű�� �˴ϴ�.
//
//===================================================================================================

/************************************************************************************************
	���ϸ� : OnlineBanner.h
	����� : ������
************************************************************************************************/

/*
#ifndef	___ONLINE_BANNER___
#define ___ONLINE_BANNER___


typedef struct
{
	XSPR				m_BannerImage;
	SI32				m_Delay;			// Banner�� �� �ִ� �ð�, ������ ���̴�.
	SI32				m_FrameDelay;

	char				m_URL[256];
} SBannerData;


// Banner Class
// Banner 0���� �⺻ Banner�̸�, �ٸ� Banner�� �� ���� �������� ��� �� �ִ´�.
// Banner 0���� �����ϰ��, 1�и��� �ѹ��� Banner�� �����Ƿ� �ٿ��. �ٿ��� Delay��ŭ �ð��� ������, ������ �ð��� �⺻ Banner�� �مR��.
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
	OnlineBanner();																// ������.
	virtual	~OnlineBanner();													// �Ҹ���.
		
	virtual	BOOL		Init(cltOnlineWorld* lpcltOnlineWorld);					// �ʱ�ȭ
	virtual	VOID		Free(void);												// ����.

	virtual BOOL		Poll(void);												// Poll
	virtual BOOL		Draw(LPDIRECTDRAWSURFACE7 pSurface);					// Draw
};	


#endif
*/