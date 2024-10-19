/************************************************************************************************
	파일명 : OnlineNoticeList.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	___ONLINE_NOTICE_LIST___
#define ___ONLINE_NOTICE_LIST___


typedef struct
{
	BOOL				m_fCreate;

	char				m_Notice[256];
	COLORREF			m_RGB;
	SI16				m_DrawCount;
} SNoticeData;


// Notice List Class
class	OnlineNoticeList
{
private:
	cltOnlineWorld*		m_lpcltOnlineWorld;

	SNoticeData*		m_lpNoticeData;
	SI32				m_NoticeDataMaxCount;
	SI32				m_NoticeDataWaitCount;

	SI32				m_RenderXPos;
	SI32				m_RenderYPos;
	SI32				m_RenderSize;
	SI32				m_RenderCount;

	SI32				m_DrawNoticeIndex;
	SI32				m_DrawNoticeScroll;
	SI32				m_DrawNoticeMaxSize;
	DWORD				m_DrawNoticeRenderStartTime;

	HRGN				m_RectHRGN;
	HRGN				m_RectBigHRGN;

public:
	OnlineNoticeList();																		// 생성자.
	virtual	~OnlineNoticeList();															// 소멸자.

	virtual	VOID					Initialize();
	virtual	BOOL					Init(cltOnlineWorld* lpcltOnlineWorld, SI32 MaxNoticeCount, SI32 WaitNoticeCount, SI32 RenderXPos, SI32 RenderYPos, SI32 RenderSize, SI32 DrawCount);		// 초기화
	virtual	VOID					Free(void);												// 해제.
	virtual BOOL					SetPosition(SI32 RenderXPos, SI32 RenderYPos, SI32 RenderSize, SI32 DrawCount);		// 출력 위치 지정

	virtual BOOL					AddNoticeData(char* lpNotice, COLORREF rgb);				// 마을 설명을 추가한다.

	virtual BOOL					Poll(LPDIRECTDRAWSURFACE7 pSurface);						// Poll
	virtual BOOL					ListSort(void);												// List Sort(현재 출력하는 Index가 없을경우 다른 Notice가 있으면, 전의 Index(최소 0)으로 하고, 없으면 -1로 수정한다.)
	virtual SI32					GetNoticeSize(SI32 Index, LPDIRECTDRAWSURFACE7 pSurface);	// Get Notice Draw Size
	virtual BOOL					Draw(HDC hDC);												// Draw
};	


#endif
