/************************************************************************************************
	���ϸ� : OnlineNoticeList.h
	����� : ������
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
	OnlineNoticeList();																		// ������.
	virtual	~OnlineNoticeList();															// �Ҹ���.

	virtual	VOID					Initialize();
	virtual	BOOL					Init(cltOnlineWorld* lpcltOnlineWorld, SI32 MaxNoticeCount, SI32 WaitNoticeCount, SI32 RenderXPos, SI32 RenderYPos, SI32 RenderSize, SI32 DrawCount);		// �ʱ�ȭ
	virtual	VOID					Free(void);												// ����.
	virtual BOOL					SetPosition(SI32 RenderXPos, SI32 RenderYPos, SI32 RenderSize, SI32 DrawCount);		// ��� ��ġ ����

	virtual BOOL					AddNoticeData(char* lpNotice, COLORREF rgb);				// ���� ������ �߰��Ѵ�.

	virtual BOOL					Poll(LPDIRECTDRAWSURFACE7 pSurface);						// Poll
	virtual BOOL					ListSort(void);												// List Sort(���� ����ϴ� Index�� ������� �ٸ� Notice�� ������, ���� Index(�ּ� 0)���� �ϰ�, ������ -1�� �����Ѵ�.)
	virtual SI32					GetNoticeSize(SI32 Index, LPDIRECTDRAWSURFACE7 pSurface);	// Get Notice Draw Size
	virtual BOOL					Draw(HDC hDC);												// Draw
};	


#endif
