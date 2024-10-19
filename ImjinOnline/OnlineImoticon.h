#ifndef	___ONLINE_IMOTICON___
#define ___ONLINE_IMOTICON___


// Imoticon Data
class	OnlineImoticon
{
protected:
	cltOnlineWorld*					m_lpWorld;

	BOOL							m_fRenderList;										// Imoticon List를 출력할것인지 Flag
	UI16							m_UseImoticonList[9];								// 사용가능한 Imoticon 처리

	SI16							siNoFieldHandle;									// 마우스 선택방어 부분 Handle

	XSPR*							m_lpPannel;

public:
	OnlineImoticon();																	// 생성자.
	virtual	~OnlineImoticon();															// 소멸자.
		
	virtual	BOOL					Init(cltOnlineWorld* lpWorld);						// 초기화
	virtual	VOID					Free(void);											// 해제.

	virtual BOOL					SetImoticonKey(SI16 ImoticonKey);					// 이모티콘 출력

	virtual BOOL					SetActive(BOOL fActive);							// 이모티콘 Active
	virtual BOOL					IsActive(void);										// Active 여부

	virtual BOOL					Pool(BOOL fLMouseUp);								// 처리
	virtual BOOL					Draw(LPDIRECTDRAWSURFACE7 lpSurface);				// 출력
};	


#endif
